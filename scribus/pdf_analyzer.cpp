/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  pdf_analyzer.cpp  -  can be used to report color
						  and font usages and detect transparency in an
						  embedded PDF
							 -------------------
	begin                : July 2009
	author				 : Thach Tran <tranngocthachs@gmail.com>, (C) 2009
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtDebug>
#include <QHash>
#include <QStack>
#include "pdf_analyzer.h"

#ifdef HAVE_PODOFO
using namespace PoDoFo;

static QHash<QString, PDFContentStreamKeyword> kwNameMap;

// we gonna need a map from string values to the defined enum of pdf keywords
// this will be used to switch the keyword we encounter while parsing pdf's page content
static void generateKWNameMap()
{
	kwNameMap.insert("k",	KW_k);
	kwNameMap.insert("K",	KW_K);
	kwNameMap.insert("rg",	KW_rg);
	kwNameMap.insert("RG",	KW_RG);
	kwNameMap.insert("g",	KW_g);
	kwNameMap.insert("G",	KW_G);
	kwNameMap.insert("cs",	KW_cs);
	kwNameMap.insert("CS",	KW_CS);
	kwNameMap.insert("sc",	KW_sc);
	kwNameMap.insert("SC",	KW_SC);
	kwNameMap.insert("scn",	KW_scn);
	kwNameMap.insert("SCN",	KW_SCN);
	kwNameMap.insert("Do",	KW_Do);
	kwNameMap.insert("BI",	KW_BI);
	kwNameMap.insert("ID",	KW_ID);
	kwNameMap.insert("EI",	KW_EI);
	kwNameMap.insert("gs",	KW_gs);
	kwNameMap.insert("Tf",	KW_Tf);
	kwNameMap.insert("cm",	KW_cm);
	kwNameMap.insert("q",	KW_q);
	kwNameMap.insert("w",	KW_w);
	kwNameMap.insert("J",	KW_J);
	kwNameMap.insert("j",	KW_j);
	kwNameMap.insert("M",	KW_M);
	kwNameMap.insert("d",	KW_d);
	kwNameMap.insert("Q",	KW_Q);
}


PDFAnalyzer::PDFAnalyzer(QString & filename) : QObject()
{
	static bool nameMapInited = false;
	if (!nameMapInited)
	{
		generateKWNameMap();
		nameMapInited = true;
	}

	PdfError::EnableDebug( false );
	try {
		m_doc = new PdfMemDocument(filename.toLocal8Bit().data());
	}
	catch (PdfError & e)
	{
		qDebug() << "Can't open the file";
		e.PrintErrorMsg();
		return;
	}
}

PDFAnalyzer::~PDFAnalyzer()
{
	delete m_doc;
}

bool PDFAnalyzer::inspectPDF(int pageNum, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, QList<PDFImage> & imgs)
{
	PdfPage* page = m_doc->GetPage(pageNum);
	return page?inspectCanvas(page, usedColorSpaces, hasTransparency, usedFonts, imgs):false;
}

PDFColorSpace PDFAnalyzer::getCSType(PdfObject* cs)
{
	try {
		// colorspace is either a name or an array
		if (cs && cs->IsName())
		{
			PdfName csName = cs->GetName();
			if (csName == "DeviceGray")
				return CS_DeviceGray;
			else if (csName == "DeviceRGB")
				return CS_DeviceRGB;
			else if (csName == "DeviceCMYK")
				return CS_DeviceCMYK;
		}
		else if (cs && cs->IsArray())
		{
			PdfArray csArr = cs->GetArray();
			PdfObject csTypePdfName = csArr[0];
			if (csTypePdfName.IsName())
			{
				PdfName csTypeName = csTypePdfName.GetName();
				if (csTypeName == "ICCBased")
					return CS_ICCBased;
				else if (csTypeName == "CalGray")
					return CS_CalGray;
				else if (csTypeName == "CalRGB")
					return CS_CalRGB;
				else if (csTypeName == "Lab")
					return CS_Lab;
				else if (csTypeName == "Indexed")
				{
					PdfObject base = cs->GetArray()[1];
					PdfObject* pBase = &base;
					if (base.IsReference())
					{
						pBase = cs->GetOwner()->GetObject(base.GetReference());
					}
					pBase->SetOwner(cs->GetOwner());
					return getCSType(pBase);
				}
				else if (csTypeName == "Separation")
					return CS_Separation;
				else if (csTypeName == "DeviceN")
					return CS_DeviceN;
				else if (csTypeName == "Pattern")
					return CS_Pattern;
			}
		}
	}
	catch (PdfError & e)
	{
		qDebug() << "Error in identifying the color type";
		e.PrintErrorMsg();
		return CS_Unknown;
	}
	return CS_Unknown;
}
bool PDFAnalyzer::inspectCanvas(PdfCanvas* canvas, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, QList<PDFImage> & imgs)
{
	// this method can be used to get used color spaces, detect transparency, and get used fonts in either PdfPage or PdfXObject
	PdfObject* colorSpaceRes;
	PdfObject* xObjects;
	PdfObject* transGroup;
	PdfObject* extGState;
	PdfObject* fontRes;
	QMap<PdfName, PDFColorSpace> processedNamedCS;
	QMap<PdfName, PDFFont> processedNamedFont;
	QList<PdfName> processedNamedXObj;
	QList<PdfName> processedNamedGS;
	try {
		// get hold of a PdfObject pointer of this canvas
		// needed for the finding resources code below to work
		PdfPage* page = dynamic_cast<PdfPage*>(canvas);
		PdfObject* canvasObject = page?(page->GetObject()):((dynamic_cast<PdfXObject*>(canvas))->GetObject());

		// find a resource with ColorSpace entry
		PdfObject* resources = canvas->GetResources();
		for (PdfObject* par = canvasObject; par && !resources; par = par->GetIndirectKey("Parent"))
		{
			resources = par->GetIndirectKey("Resources");
		}
		colorSpaceRes = resources?resources->GetIndirectKey("ColorSpace"):NULL;
		xObjects = resources?resources->GetIndirectKey("XObject"):NULL;
		extGState = resources?resources->GetIndirectKey("ExtGState"):NULL;
		fontRes = resources?resources->GetIndirectKey("Font"):NULL;

		// getting the transparency group of this content stream (if available)
		transGroup = canvasObject?canvasObject->GetIndirectKey("Group"):NULL;
		if (transGroup)
		{
			PdfObject* subtype = transGroup->GetIndirectKey("S");
			if (subtype && subtype->GetName() == "Transparency")
			{
				// having transparency group means there's transparency in the PDF
				hasTransparency = true;

				// reporting the color space used in transparency group (Section 7.5.5, PDF 1.6 Spec)
				PdfObject* cs = transGroup->GetIndirectKey("CS");
				if (cs)
				{
					PDFColorSpace retval = getCSType(cs);
					if (retval != CS_Unknown && !usedColorSpaces.contains(retval))
						usedColorSpaces.append(retval);
				}
			}
		}
	}
	catch (PdfError & e)
	{
		qDebug() << "Error in analyzing stream's resources.";
		e.PrintErrorMsg();
		return false;
	}

	try {
		// start parsing the content stream
		PdfContentsTokenizer tokenizer(canvas);
		EPdfContentsType t;
		const char * kwText;
		PdfVariant var;
		bool readToken;

		int tokenNumber = 0;
		QList<PdfVariant> args;
		bool inlineImgDict = false;
		QStack<PDFGraphicState> gsStack;
		PDFGraphicState currGS;
		while ((readToken = tokenizer.ReadNext(t, kwText, var)))
		{
			++tokenNumber;
			if (t == ePdfContentsType_Variant)
			{
				args.append(var);
			}
			else if (t == ePdfContentsType_Keyword)
			{
				QString kw(kwText);
				switch(kwNameMap.value(kw, KW_Undefined))
				{
				case KW_q:
					gsStack.push(currGS);
					break;
				case KW_Q:
					currGS = gsStack.pop();
					break;
				case KW_cm:
					{
					if (args.size() == 6)
					{
						double mt[6];
						for (int i=0; i<6; ++i)
						{
							mt[i] = args[i].GetReal();
						}
						QMatrix transMatrix(mt[0], mt[1], mt[2], mt[3], mt[4], mt[5]);
						currGS.ctm = transMatrix*currGS.ctm;
					}
					}
					break;
				case KW_w:
					currGS.lineWidth = args[0].GetReal();
					break;
				case KW_J:
					currGS.lineCap = args[0].GetNumber();
					break;
				case KW_j:
					currGS.lineJoin = args[0].GetNumber();
					break;
				case KW_M:
					currGS.lineJoin = args[0].GetReal();
					break;
				case KW_d:
					{
					currGS.dashPattern.first.clear();
					PdfArray dashArr = args[0].GetArray();
					for (uint i=0; i<dashArr.size(); ++i)
						currGS.dashPattern.first.append(dashArr[i].GetNumber());
					currGS.dashPattern.second = args[0].GetNumber();
					}
					break;
				case KW_g:
					if (!usedColorSpaces.contains(CS_DeviceGray))
						usedColorSpaces.append(CS_DeviceGray);
					currGS.fillCS = CS_DeviceGray;
					currGS.fillColor.clear();
					currGS.fillColor.append(args[0].GetReal());
					break;
				case KW_G:
					if (!usedColorSpaces.contains(CS_DeviceGray))
						usedColorSpaces.append(CS_DeviceGray);
					currGS.strokeCS = CS_DeviceGray;
					currGS.strokeColor.clear();
					currGS.strokeColor.append(args[0].GetReal());
					break;
				case KW_rg:
					if (!usedColorSpaces.contains(CS_DeviceRGB))
						usedColorSpaces.append(CS_DeviceRGB);
					currGS.fillCS = CS_DeviceRGB;
					currGS.fillColor.clear();
					for (int i=0; i<args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case KW_RG:
					if (!usedColorSpaces.contains(CS_DeviceRGB))
						usedColorSpaces.append(CS_DeviceRGB);
					currGS.strokeCS = CS_DeviceRGB;
					currGS.strokeColor.clear();
					for (int i=0; i<args.size(); ++i)
						currGS.strokeColor.append(args[i].GetReal());
					break;
				case KW_k:
					if (!usedColorSpaces.contains(CS_DeviceCMYK))
						usedColorSpaces.append(CS_DeviceCMYK);
					currGS.fillCS = CS_DeviceCMYK;
					currGS.fillColor.clear();
					for (int i=0; i<args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case KW_K:
					if (!usedColorSpaces.contains(CS_DeviceCMYK))
						usedColorSpaces.append(CS_DeviceCMYK);
					currGS.strokeCS = CS_DeviceCMYK;
					currGS.strokeColor.clear();
					for (int i=0; i<args.size(); ++i)
						currGS.strokeColor.append(args[i].GetReal());
					break;
				case KW_cs:
					{
					if (args.size() == 1 && args[0].IsName())
					{
						if (args[0].GetName() == "DeviceGray")
						{
							currGS.fillCS = CS_DeviceGray;
							currGS.fillColor.clear();
							currGS.fillColor.append(0);
							if (!usedColorSpaces.contains(CS_DeviceGray))
								usedColorSpaces.append(CS_DeviceGray);
						}
						else if (args[0].GetName() == "DeviceRGB")
						{
							currGS.fillCS = CS_DeviceRGB;
							currGS.fillColor.clear();
							for (int i=0; i<3; ++i)
								currGS.fillColor.append(0);
							if (!usedColorSpaces.contains(CS_DeviceRGB))
								usedColorSpaces.append(CS_DeviceRGB);
						}
						else if (args[0].GetName() == "DeviceCMYK")
						{
							currGS.fillCS = CS_DeviceCMYK;
							currGS.fillColor.clear();
							for (int i=0; i<3; ++i)
								currGS.fillColor.append(0);
							currGS.fillColor.append(1);
							if (!usedColorSpaces.contains(CS_DeviceCMYK))
								usedColorSpaces.append(CS_DeviceCMYK);
						}
						else if (args[0].GetName() == "Pattern")
						{
							currGS.fillCS = CS_Pattern;
							if (!usedColorSpaces.contains(CS_Pattern))
								usedColorSpaces.append(CS_Pattern);
						}
						else
						{
							if (processedNamedCS.contains(args[0].GetName()))
							{
								currGS.fillCS = processedNamedCS.value(args[0].GetName());
							}
							else
							{
								if (colorSpaceRes && colorSpaceRes->GetIndirectKey(args[0].GetName()))
								{
									PdfObject* csEntry = colorSpaceRes->GetIndirectKey(args[0].GetName());
									PDFColorSpace retval = getCSType(csEntry);
									if (retval != CS_Unknown && !usedColorSpaces.contains(retval))
										usedColorSpaces.append(retval);
									currGS.fillCS = retval;
									processedNamedCS.insert(args[0].GetName(), retval);
								}
								else
								{
									qDebug() << "Supplied colorspace is undefined!";
									return false;
								}
							}
						}
					}
					else
					{
						qDebug() << "Wrong syntax in specifying color space!";
						return false;
					}
					}
					break;
				case KW_CS:
					{
					if (args.size() == 1 && args[0].IsName())
					{
						if (args[0].GetName() == "DeviceGray")
						{
							currGS.strokeCS = CS_DeviceGray;
							currGS.strokeColor.clear();
							currGS.strokeColor.append(0);
							if (!usedColorSpaces.contains(CS_DeviceGray))
								usedColorSpaces.append(CS_DeviceGray);
						}
						else if (args[0].GetName() == "DeviceRGB")
						{
							currGS.fillCS = CS_DeviceRGB;
							currGS.strokeColor.clear();
							for (int i=0; i<3; ++i)
								currGS.strokeColor.append(0);
							if (!usedColorSpaces.contains(CS_DeviceRGB))
								usedColorSpaces.append(CS_DeviceRGB);
						}
						else if (args[0].GetName() == "DeviceCMYK")
						{
							currGS.fillCS = CS_DeviceCMYK;
							currGS.strokeColor.clear();
							for (int i=0; i<3; ++i)
								currGS.strokeColor.append(0);
							currGS.strokeColor.append(1);
							if (!usedColorSpaces.contains(CS_DeviceCMYK))
								usedColorSpaces.append(CS_DeviceCMYK);
						}
						else if (args[0].GetName() == "Pattern")
						{
							currGS.fillCS = CS_Pattern;
							if (!usedColorSpaces.contains(CS_Pattern))
								usedColorSpaces.append(CS_Pattern);
						}
						else
						{
							if (processedNamedCS.contains(args[0].GetName()))
							{
								currGS.strokeCS = processedNamedCS.value(args[0].GetName());
							}
							else
							{
								if (colorSpaceRes && colorSpaceRes->GetIndirectKey(args[0].GetName()))
								{
									PdfObject* csEntry = colorSpaceRes->GetIndirectKey(args[0].GetName());
									PDFColorSpace retval = getCSType(csEntry);
									if (retval != CS_Unknown && !usedColorSpaces.contains(retval))
										usedColorSpaces.append(retval);
									currGS.strokeCS = retval;
									processedNamedCS.insert(args[0].GetName(), retval);
								}
								else
								{
									qDebug() << "Supplied colorspace is undefined!";
									return false;
								}
							}
						}
					}
					else
					{
						qDebug() << "Wrong syntax in specifying color space!";
						return false;
					}
					}
					break;
				case KW_sc:
					currGS.fillColor.clear();
					for (int i=0; i<args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case KW_SC:
					currGS.strokeColor.clear();
					for (int i=0; i<args.size(); ++i)
						currGS.strokeColor.append(args[i].GetReal());
					break;
				case KW_scn:
					currGS.fillColor.clear();
					for (int i=0; i<args.size(); ++i)
					{
						if (args[i].IsReal() || args[i].IsNumber())
							currGS.fillColor.append(args[i].GetReal());
					}
					break;
				case KW_SCN:
					currGS.strokeColor.clear();
					for (int i=0; i<args.size(); ++i)
					{
						if (args[i].IsReal() || args[i].IsNumber())
							currGS.strokeColor.append(args[i].GetReal());
					}
					break;
				case KW_Do: // image or form XObject
					{
					if (!processedNamedXObj.contains(args[0].GetName()))
					{
						if (args.size() == 1 && args[0].IsName() && xObjects)
						{
							PdfObject* xObject = xObjects->GetIndirectKey(args[0].GetName());
							PdfObject* subtypeObject = xObject?xObject->GetIndirectKey("Subtype"):NULL;
							if (subtypeObject && subtypeObject->IsName())
							{
								if (subtypeObject->GetName() == "Image")
								{
									PdfObject* imgColorSpace = xObject->GetIndirectKey("ColorSpace");
									if (imgColorSpace)
									{
										PDFColorSpace retval = getCSType(imgColorSpace);
										if (retval != CS_Unknown && !usedColorSpaces.contains(retval))
											usedColorSpaces.append(retval);
									}
									PdfObject* sMaskObj = xObject->GetIndirectKey("SMask");
									if (sMaskObj)
										hasTransparency = true;
									PDFImage img;
									img.imgName = args[0].GetName().GetEscapedName().c_str();
									double width = xObject->GetIndirectKey("Width")->GetReal();
									double height = xObject->GetIndirectKey("Height")->GetReal();
									img.dpiX = qRound(width/(currGS.ctm.m11()/72));
									img.dpiY = qRound(height/(currGS.ctm.m22()/72));
									imgs.append(img);
								}
								else if (subtypeObject->GetName() == "Form")
								{
									PdfXObject xObj(xObject);
									inspectCanvas(&xObj, usedColorSpaces, hasTransparency, usedFonts, imgs); // recursive call
								}
							}
							else
							{
								qDebug() << "Supplied external object is undefined!";
								return false;
							}
							processedNamedXObj.append(args[0].GetName());
						}
						else
						{
							qDebug() << "Wrong syntax for Do operator or there's no XObject defined!";
							return false;
						}

					}
					}
					break;
				case KW_BI:
					inlineImgDict = true;
					break;
				case KW_ID:
					if (inlineImgDict)
					{
						PdfName colorspace("ColorSpace");
						PdfName cs("CS");
						if (args.contains(colorspace) || args.contains(cs))
						{
							int csIdx = args.contains(colorspace)?args.indexOf(colorspace):args.indexOf(cs);
							if (args[csIdx+1].IsName())
							{
								PdfName csName = args[csIdx+1].GetName();
								if ((csName == "G" || csName == "DeviceGray") && !usedColorSpaces.contains(CS_DeviceGray))
									usedColorSpaces.append(CS_DeviceGray);
								else if ((csName == "RGB" || csName == "DeviceRGB") && !usedColorSpaces.contains(CS_DeviceRGB))
									usedColorSpaces.append(CS_DeviceRGB);
								else if ((csName == "CMYK" || csName == "DeviceCMYK") && !usedColorSpaces.contains(CS_DeviceCMYK))
									usedColorSpaces.append(CS_DeviceCMYK);
								else if (!processedNamedCS.contains(csName))
								{
									if (colorSpaceRes && colorSpaceRes->GetIndirectKey(csName))
									{
										PdfObject* csEntry = colorSpaceRes->GetIndirectKey(csName);
										if (csEntry)
										{
											PDFColorSpace retval = getCSType(csEntry);
											if (retval != CS_Unknown && !usedColorSpaces.contains(retval))
												usedColorSpaces.append(retval);
											processedNamedCS.insert(csName, retval);
										}
									}
									else
									{
										qDebug() << "Supplied colorspace for inline image is undefined!";
										return false;
									}
								}
							}
						}
						PdfName height("Height");
						PdfName h("H");
						PdfName width("Width");
						PdfName w("W");
						if ((args.contains(height) || args.contains(h)) && (args.contains(width) || args.contains(w)))
						{
							int heightIdx = args.contains(height)?args.indexOf(height):args.indexOf(h);
							int widthIdx = args.contains(width)?args.indexOf(width):args.indexOf(w);
							double height = args[heightIdx+1].GetReal();
							double width = args[widthIdx+1].GetReal();
							PDFImage img;
							img.imgName = "Inline Image";
							img.dpiX = qRound(width/(currGS.ctm.m11()/72));
							img.dpiY = qRound(height/(currGS.ctm.m22()/72));
							imgs.append(img);
						}
						inlineImgDict = false;
					}
					break;
				case KW_gs:
					{
					if (!processedNamedGS.contains(args[0].GetName()))
					{
						if (args.size() == 1 && args[0].IsName() && extGState)
						{
							PdfObject* extGStateObj = extGState->GetIndirectKey(args[0].GetName());
							if (extGStateObj)
							{
								inspectExtGStateObj(extGStateObj, usedColorSpaces, hasTransparency, usedFonts, currGS);
							}
							else
							{
								qDebug() << "Named graphic state used with gs operator is undefined in current ExtGState";
								return false;
							}
							processedNamedGS.append(args[0].GetName());
						}
						else
						{
							qDebug() << "Wrong syntax in applying extended graphic state (gs operator) or there's no ExtGState defined!";
							return false;
						}
					}
					}
					break;
				case KW_Tf:
					{
					if (processedNamedFont.contains(args[0].GetName()))
					{
						currGS.font.first = processedNamedFont.value(args[0].GetName());
						currGS.font.second = args[1].GetReal();
					}
					else
					{
						if (args.size() == 2 && args[0].IsName() && fontRes)
						{
							PdfObject* fontObj = fontRes->GetIndirectKey(args[0].GetName());
							if (fontObj)
							{
								PDFFont retval = getFontInfo(fontObj);
								usedFonts.append(retval);
								processedNamedFont.insert(args[0].GetName(), retval);
								currGS.font.first = retval;
								currGS.font.second = args[1].GetReal();
							}
							else
							{
								qDebug() << "The specified font cannot be found in current Resources!";
								return false;
							}
						}
						else
						{
							qDebug() << "Wrong syntax in use of Tf operator or there's no Font defined in current Resources dictionary!";
							return false;
						}
					}
					}
					break;
				case KW_Undefined:
				default:
					break;
				}
				args.clear();
			}
		}
	}
	catch (PdfError & e)
	{
		qDebug() << "Error in parsing content stream";
		e.PrintErrorMsg();
		return false;
	}
	return true;
}
void PDFAnalyzer::inspectExtGStateObj(PdfObject* extGStateObj, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, PDFGraphicState & currGS)
{
	PdfObject* bmObj = extGStateObj->GetIndirectKey("BM");
	if (bmObj && bmObj->IsName())
	{
		currGS.blendModes.clear();
		currGS.blendModes.append(bmObj->GetName().GetEscapedName().c_str());
		if (!(bmObj->GetName() == "Normal" || bmObj->GetName() == "Compatible"))
			hasTransparency = true;
	}
	else if (bmObj && bmObj->IsArray())
	{
		PdfArray arr = bmObj->GetArray();
		currGS.blendModes.clear();
		for(uint i=0; i<arr.GetSize(); ++i)
			currGS.blendModes.append(arr[i].GetName().GetEscapedName().c_str());
		if (arr[0].IsName() && !(arr[0].GetName() == "Normal" || arr[0].GetName() == "Compatible"))
			hasTransparency = true;
	}
	PdfObject* caObj = extGStateObj->GetIndirectKey("ca");
	if (caObj && (caObj->IsReal() || caObj->IsNumber()))
	{
		currGS.fillAlphaConstant = caObj->GetReal();
		if (caObj->GetReal() < 1)
			hasTransparency = true;
	}
	PdfObject* cAObj = extGStateObj->GetIndirectKey("CA");
	if (cAObj && (cAObj->IsReal() || cAObj->IsNumber()))
	{
		if (cAObj->GetReal() < 1)
		hasTransparency = true;
	}
	PdfObject* sMaskObj = extGStateObj->GetIndirectKey("SMask");
	if (sMaskObj && !(sMaskObj->IsName() && sMaskObj->GetName() == "None"))
		hasTransparency = true;
	PdfObject* fontObj = extGStateObj->GetIndirectKey("Font");
	if (fontObj && fontObj->IsArray())
	{
		PdfArray arr = fontObj->GetArray();
		if (arr[0].IsReference())
		{
			PdfReference ref = arr[0].GetReference();
			PdfObject* fontObject = m_doc->GetObjects().GetObject(ref);
			if (fontObject)
			{
				PDFFont font = getFontInfo(fontObject);
				usedFonts.append(font);
				currGS.font.first = font;
				currGS.font.second = arr[1].GetReal();
			}

		}
	}
	PdfObject* lwObj = extGStateObj->GetIndirectKey("LW");
	if (lwObj)
		currGS.lineWidth = lwObj->GetReal();
	PdfObject* lcObj = extGStateObj->GetIndirectKey("LC");
	if (lcObj)
		currGS.lineCap = lcObj->GetNumber();
	PdfObject* ljObj = extGStateObj->GetIndirectKey("LJ");
	if (ljObj)
		currGS.lineJoin = ljObj->GetNumber();
	PdfObject* mlObj = extGStateObj->GetIndirectKey("ML");
	if (mlObj)
		currGS.miterLimit = mlObj->GetReal();
	PdfObject* dObj = extGStateObj->GetIndirectKey("D");
	if (dObj)
	{
		PdfObject dObjA = dObj->GetArray()[0];
		PdfArray dashArr = dObjA.GetArray();
		currGS.dashPattern.first.clear();
		for (uint i=0; i<dashArr.GetSize(); ++i)
			currGS.dashPattern.first.append(dashArr[i].GetNumber());
		PdfObject dObjB = dObj->GetArray()[1];
		currGS.dashPattern.second = dObjB.GetNumber();
	}
}
PDFFont PDFAnalyzer::getFontInfo(PdfObject* fontObj)
{
	PDFFont currFont;
	PdfObject* subtype = fontObj->GetIndirectKey("Subtype");
	if (subtype && subtype->IsName())
	{
		PdfObject* fontDesc = fontObj->GetIndirectKey("FontDescriptor");
		if (subtype->GetName() == "Type1")
			currFont.fontType = F_Type1;
		else if (subtype->GetName() == "MMType1")
			currFont.fontType = F_MMType1;
		else if (subtype->GetName() == "TrueType")
			currFont.fontType = F_TrueType;
		else if (subtype->GetName() == "Type3")
		{
			currFont.fontType = F_Type3;
			currFont.isEmbedded = true;
			fontDesc = NULL;
		}
		else if (subtype->GetName() == "Type0")
		{
			PdfObject* descendantFonts = fontObj->GetIndirectKey("DescendantFonts");
			if (descendantFonts && descendantFonts->IsArray())
			{
				PdfObject descendantFont = descendantFonts->GetArray()[0];
				descendantFont.SetOwner(descendantFonts->GetOwner());
				PdfObject* subtypeDescFont = descendantFont.GetIndirectKey("Subtype");
				fontDesc = descendantFont.MustGetIndirectKey("FontDescriptor");
				if (subtypeDescFont && subtypeDescFont->IsName())
				{
					if (subtypeDescFont->GetName() == "CIDFontType0")
						currFont.fontType = F_CIDFontType0;
					else if (subtypeDescFont->GetName() == "CIDFontType2")
						currFont.fontType = F_CIDFontType2;
				}
			}
		}
		if (fontDesc)
		{
			PdfObject* fontFile = fontDesc->GetIndirectKey("FontFile");
			PdfObject* fontFile2 = fontDesc->GetIndirectKey("FontFile2");
			PdfObject* fontFile3 = fontDesc->GetIndirectKey("FontFile3");
			if (fontFile && fontFile->HasStream())
				currFont.isEmbedded = true;
			if (fontFile2 && fontFile2->HasStream())
				currFont.isEmbedded = true;
			if (fontFile3 && fontFile3->HasStream())
			{
				currFont.isEmbedded = true;
				PdfObject* ff3Subtype = fontFile3->GetIndirectKey("Subtype");
				if (ff3Subtype && ff3Subtype->IsName() && ff3Subtype->GetName() == "OpenType")
					currFont.isOpenType = true;
			}
		}
	}
	return currFont;
}
#else
PDFAnalyzer::PDFAnalyzer(QString & filename) : QObject()
{
}
PDFAnalyzer::~PDFAnalyzer()
{
}
bool PDFAnalyzer::inspectPDF(int pageNum, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, QList<PDFImage> & imgs)
{
	return false;
}
#endif
