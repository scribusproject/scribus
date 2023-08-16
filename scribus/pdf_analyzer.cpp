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

#if (PODOFO_VERSION < PODOFO_MAKE_VERSION(0, 10, 0))
#define IsRealStrict IsReal
#endif

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


PDFAnalyzer::PDFAnalyzer(QString & filename)
 : m_filename(filename)
{
	static bool nameMapInited = false;
	if (!nameMapInited)
	{
		generateKWNameMap();
		nameMapInited = true;
	}

	m_pdfdoc = nullptr;

#if (PODOFO_VERSION < PODOFO_MAKE_VERSION(0, 10, 0))
	PdfError::EnableDebug( false );
#endif
	try
	{
		m_pdfdoc = new PdfMemDocument();
		m_pdfdoc->Load(filename.toLocal8Bit().data());
	}
	catch (PdfError & e)
	{
		qDebug() << "Can't open PDF file" << filename;
		e.PrintErrorMsg();
		return;
	}
}

PDFAnalyzer::~PDFAnalyzer()
{
	delete m_pdfdoc;
}

bool PDFAnalyzer::inspectPDF(int pageNum, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, QList<PDFImage> & imgs)
{
	if (!m_pdfdoc)
		return false;

#if (PODOFO_VERSION >= PODOFO_MAKE_VERSION(0, 10, 0))
	PdfPage* page = &(m_pdfdoc->GetPages().GetPageAt(pageNum));
#else
	PdfPage* page = m_pdfdoc->GetPage(pageNum);
#endif
	return page ? inspectCanvas(page, usedColorSpaces, hasTransparency, usedFonts, imgs) : false;
}

PDFColorSpace PDFAnalyzer::getCSType(PdfObject* cs)
{
	try
	{
		// colorspace is either a name or an array
		if (cs && cs->IsName())
		{
			const PdfName& csName = cs->GetName();
			if (csName == "DeviceGray")
				return CS_DeviceGray;
			if (csName == "DeviceRGB")
				return CS_DeviceRGB;
			if (csName == "DeviceCMYK")
				return CS_DeviceCMYK;
		}
		else if (cs && cs->IsArray())
		{
			PdfArray csArr = cs->GetArray();
			PdfObject csTypePdfName = csArr[0];
			if (csTypePdfName.IsName())
			{
				const PdfName& csTypeName = csTypePdfName.GetName();
				if (csTypeName == "ICCBased")
					return CS_ICCBased;
				if (csTypeName == "CalGray")
					return CS_CalGray;
				if (csTypeName == "CalRGB")
					return CS_CalRGB;
				if (csTypeName == "Lab")
					return CS_Lab;
				if (csTypeName == "Indexed")
				{
					PdfObject base = cs->GetArray()[1];
					PdfObject* pBase = &base;
					if (base.IsReference())
					{
#if (PODOFO_VERSION >= PODOFO_MAKE_VERSION(0, 10, 0))
						PdfDocument* pdfdoc = cs->GetDocument();
						PdfIndirectObjectList& pdf_iol = pdfdoc->GetObjects();
						pBase = pdf_iol.GetObject(base.GetReference());
#else
						pBase = cs->GetOwner()->GetObject(base.GetReference());
#endif
					}
#if (PODOFO_VERSION < PODOFO_MAKE_VERSION(0, 9, 7))
					pBase->SetOwner(cs->GetOwner());
#endif
					return getCSType(pBase);
				}
				if (csTypeName == "Separation")
					return CS_Separation;
				if (csTypeName == "DeviceN")
					return CS_DeviceN;
				if (csTypeName == "Pattern")
					return CS_Pattern;
			}
		}
	}
	catch (PdfError & e)
	{
		qDebug() << "Error in identifying the color type. File:" << m_filename;
		e.PrintErrorMsg();
		return CS_Unknown;
	}
	return CS_Unknown;
}

bool PDFAnalyzer::inspectCanvas(PdfCanvas* canvas, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, QList<PDFImage> & imgs)
{
#if (PODOFO_VERSION >= PODOFO_MAKE_VERSION(0, 10, 0))
	// this method can be used to get used color spaces, detect transparency, and get used fonts in either PdfPage or PdfXObject
	PdfDictionary* colorSpacesDict { nullptr };
	PdfDictionary* xObjectsDict { nullptr };
	PdfDictionary* extGStatesDict { nullptr };
	PdfDictionary* fontsDict { nullptr };
	QMap<PdfName, PDFColorSpace> processedNamedCS;
	QMap<PdfName, PDFFont> processedNamedFont;
	QList<PdfName> processedNamedXObj;
	QList<PdfName> processedNamedGS;
	try
	{
		// get hold of a PdfObject pointer of this canvas
		// needed for the finding resources code below to work
		PdfPage* page = dynamic_cast<PdfPage*>(canvas);
		PdfObject* canvasObject = page ? &(page->GetObject()) : &(dynamic_cast<PdfXObject*>(canvas)->GetObject());
		PdfDictionary* canvasDict = (canvasObject && canvasObject->IsDictionary()) ? &(canvasObject->GetDictionary()) : nullptr;

		// find a resource with ColorSpace entry
		PdfResources* canvasRsrc = canvas->GetResources();
		PdfObject* resources = &(canvasRsrc->GetObject());
		for (PdfDictionary* par = canvasDict, *parentDict = nullptr; par && !resources; par = parentDict)
		{
			resources = par->FindKey("Resources");
			PdfObject* parentObj = par->FindKey("Parent");
			parentDict = (parentObj && parentObj->IsDictionary()) ? &(parentObj->GetDictionary()) : nullptr;
		}

		PdfDictionary* resourcesDict = (resources && resources->IsDictionary()) ? &(resources->GetDictionary()) : nullptr;
		PdfObject* colorSpaceRes = resourcesDict ? resourcesDict->FindKey("ColorSpace") : nullptr;
		PdfObject* xObjects = resourcesDict ? resourcesDict->FindKey("XObject") : nullptr;
		PdfObject* extGState = resourcesDict ? resourcesDict->FindKey("ExtGState") : nullptr;
		PdfObject* fontRes = resourcesDict ? resourcesDict->FindKey("Font") : nullptr;

		colorSpacesDict = (colorSpaceRes && colorSpaceRes->IsDictionary()) ? &(colorSpaceRes->GetDictionary()) : nullptr;
		xObjectsDict = (xObjects && xObjects->IsDictionary()) ? &(xObjects->GetDictionary()) : nullptr;
		extGStatesDict = (extGState && extGState->IsDictionary()) ? &(extGState->GetDictionary()) : nullptr;
		fontsDict = (fontRes && fontRes->IsDictionary()) ? &(fontRes->GetDictionary()) : nullptr;

		// getting the transparency group of this content stream (if available)
		PdfObject* transGroup = canvasDict ? canvasDict->FindKey("Group") : nullptr;
		PdfDictionary* transGroupDict = (transGroup && transGroup->IsDictionary()) ? &(transGroup->GetDictionary()) : nullptr;
		if (transGroupDict)
		{
			const PdfObject* subtype = transGroupDict->FindKey("S");
			if (subtype && subtype->GetName() == "Transparency")
			{
				// having transparency group means there's transparency in the PDF
				hasTransparency = true;

				// reporting the color space used in transparency group (Section 7.5.5, PDF 1.6 Spec)
				PdfObject* cs = transGroupDict->FindKey("CS");
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
		qDebug() << "Error in analyzing stream's resources. File:" << m_filename;
		e.PrintErrorMsg();
		return false;
	}

	try
	{
		// start parsing the content stream
		PdfContentReaderArgs tokenizerArgs = { PdfContentReaderFlags::DontFollowXObjectForms };
		PdfContentStreamReader tokenizer(*canvas, tokenizerArgs);
		PdfContent pdfContent;
		PdfVariant var;

		int tokenNumber = 0;
		bool inlineImgDict = false;
		QList<PdfVariant> args;
		QStack<PDFGraphicState> gsStack;
		PDFGraphicState currGS;
		while (tokenizer.TryReadNext(pdfContent))
		{
			++tokenNumber;
			if (pdfContent.Type == PdfContentType::Operator)
			{
				args.clear();
				size_t stackSize = pdfContent.Stack.size();
				for (size_t i = 0; i < stackSize; ++i)
					args.append(pdfContent.Stack[stackSize - 1 - i]);
				switch (pdfContent.Operator)
				{
				case PdfOperator::q:
					gsStack.push(currGS);
					break;
				case PdfOperator::Q:
					currGS = gsStack.pop();
					break;
				case PdfOperator::cm:
				{
					if (args.size() == 6)
					{
						double mt[6];
						for (int i = 0; i < 6; ++i)
						{
							mt[i] = args[i].GetReal();
						}
						QTransform transMatrix(mt[0], mt[1], mt[2], mt[3], mt[4], mt[5]);
						currGS.ctm = transMatrix * currGS.ctm;
					}
				}
				break;
				case PdfOperator::w:
					currGS.lineWidth = args[0].GetReal();
					break;
				case PdfOperator::J:
					currGS.lineCap = args[0].GetNumber();
					break;
				case PdfOperator::j:
					currGS.lineJoin = args[0].GetNumber();
					break;
				case PdfOperator::M:
					currGS.lineJoin = args[0].GetReal();
					break;
				case PdfOperator::d:
				{
					currGS.dashPattern.first.clear();
					PdfArray dashArr = args[0].GetArray();
					for (uint i = 0; i < dashArr.size(); ++i)
						currGS.dashPattern.first.append(dashArr[i].GetNumber());
					currGS.dashPattern.second = args[1].GetNumber();
				}
				break;
				case PdfOperator::g:
					if (!usedColorSpaces.contains(CS_DeviceGray))
						usedColorSpaces.append(CS_DeviceGray);
					currGS.fillCS = CS_DeviceGray;
					currGS.fillColor.clear();
					currGS.fillColor.append(args[0].GetReal());
					break;
				case PdfOperator::G:
					if (!usedColorSpaces.contains(CS_DeviceGray))
						usedColorSpaces.append(CS_DeviceGray);
					currGS.strokeCS = CS_DeviceGray;
					currGS.strokeColor.clear();
					currGS.strokeColor.append(args[0].GetReal());
					break;
				case PdfOperator::rg:
					if (!usedColorSpaces.contains(CS_DeviceRGB))
						usedColorSpaces.append(CS_DeviceRGB);
					currGS.fillCS = CS_DeviceRGB;
					currGS.fillColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case PdfOperator::RG:
					if (!usedColorSpaces.contains(CS_DeviceRGB))
						usedColorSpaces.append(CS_DeviceRGB);
					currGS.strokeCS = CS_DeviceRGB;
					currGS.strokeColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.strokeColor.append(args[i].GetReal());
					break;
				case PdfOperator::k:
					if (!usedColorSpaces.contains(CS_DeviceCMYK))
						usedColorSpaces.append(CS_DeviceCMYK);
					currGS.fillCS = CS_DeviceCMYK;
					currGS.fillColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case PdfOperator::K:
					if (!usedColorSpaces.contains(CS_DeviceCMYK))
						usedColorSpaces.append(CS_DeviceCMYK);
					currGS.strokeCS = CS_DeviceCMYK;
					currGS.strokeColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.strokeColor.append(args[i].GetReal());
					break;
				case PdfOperator::cs:
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
							for (int i = 0; i < 3; ++i)
								currGS.fillColor.append(0);
							if (!usedColorSpaces.contains(CS_DeviceRGB))
								usedColorSpaces.append(CS_DeviceRGB);
						}
						else if (args[0].GetName() == "DeviceCMYK")
						{
							currGS.fillCS = CS_DeviceCMYK;
							currGS.fillColor.clear();
							for (int i = 0; i < 3; ++i)
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
								if (colorSpacesDict && colorSpacesDict->FindKey(args[0].GetName()))
								{
									PdfObject* csEntry = colorSpacesDict->FindKey(args[0].GetName());
									PDFColorSpace retval = getCSType(csEntry);
									if (retval != CS_Unknown && !usedColorSpaces.contains(retval))
										usedColorSpaces.append(retval);
									currGS.fillCS = retval;
									processedNamedCS.insert(args[0].GetName(), retval);
								}
								else
								{
									qDebug() << "Supplied colorspace is undefined! File:" << m_filename;
									return false;
								}
							}
						}
					}
					else
					{
						qDebug() << "Wrong syntax in specifying color space! File:" << m_filename;
						return false;
					}
				}
				break;
				case PdfOperator::CS:
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
							for (int i = 0; i < 3; ++i)
								currGS.strokeColor.append(0);
							if (!usedColorSpaces.contains(CS_DeviceRGB))
								usedColorSpaces.append(CS_DeviceRGB);
						}
						else if (args[0].GetName() == "DeviceCMYK")
						{
							currGS.fillCS = CS_DeviceCMYK;
							currGS.strokeColor.clear();
							for (int i = 0; i < 3; ++i)
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
								if (colorSpacesDict && colorSpacesDict->FindKey(args[0].GetName()))
								{
									PdfObject* csEntry = colorSpacesDict->FindKey(args[0].GetName());
									PDFColorSpace retval = getCSType(csEntry);
									if (retval != CS_Unknown && !usedColorSpaces.contains(retval))
										usedColorSpaces.append(retval);
									currGS.strokeCS = retval;
									processedNamedCS.insert(args[0].GetName(), retval);
								}
								else
								{
									qDebug() << "Supplied colorspace is undefined! File:" << m_filename;
									return false;
								}
							}
						}
					}
					else
					{
						qDebug() << "Wrong syntax in specifying color space! File:" << m_filename;
						return false;
					}
				}
				break;
				case PdfOperator::sc:
					currGS.fillColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case PdfOperator::SC:
					currGS.strokeColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.strokeColor.append(args[i].GetReal());
					break;
				case PdfOperator::scn:
					currGS.fillColor.clear();
					for (int i = 0; i < args.size(); ++i)
					{
						if (args[i].IsRealStrict() || args[i].IsNumber())
							currGS.fillColor.append(args[i].GetReal());
					}
					break;
				case PdfOperator::SCN:
					currGS.strokeColor.clear();
					for (int i = 0; i < args.size(); ++i)
					{
						if (args[i].IsRealStrict() || args[i].IsNumber())
							currGS.strokeColor.append(args[i].GetReal());
					}
					break;
				case PdfOperator::Do: // image or form XObject
					// Handled in PdfContentType::DoXObject
					break;
				case PdfOperator::BI:
					inlineImgDict = true;
					break;
				case PdfOperator::ID:
					if (inlineImgDict)
					{
						PdfName colorspace("ColorSpace");
						PdfName cs("CS");
						if (args.contains(colorspace) || args.contains(cs))
						{
							int csIdx = args.contains(colorspace) ? args.indexOf(colorspace) : args.indexOf(cs);
							if (args[csIdx + 1].IsName())
							{
								PdfName csName = args[csIdx + 1].GetName();
								if ((csName == "G" || csName == "DeviceGray") && !usedColorSpaces.contains(CS_DeviceGray))
									usedColorSpaces.append(CS_DeviceGray);
								else if ((csName == "RGB" || csName == "DeviceRGB") && !usedColorSpaces.contains(CS_DeviceRGB))
									usedColorSpaces.append(CS_DeviceRGB);
								else if ((csName == "CMYK" || csName == "DeviceCMYK") && !usedColorSpaces.contains(CS_DeviceCMYK))
									usedColorSpaces.append(CS_DeviceCMYK);
								else if (!processedNamedCS.contains(csName))
								{
									if (colorSpacesDict && colorSpacesDict->FindKey(csName))
									{
										PdfObject* csEntry = colorSpacesDict->FindKey(csName);
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
							int heightIdx = args.contains(height) ? args.indexOf(height) : args.indexOf(h);
							int widthIdx = args.contains(width) ? args.indexOf(width) : args.indexOf(w);
							double height = args[heightIdx + 1].GetReal();
							double width = args[widthIdx + 1].GetReal();
							PDFImage img;
							img.imgName = "Inline Image";
							img.dpiX = qRound(width / (currGS.ctm.m11() / 72));
							img.dpiY = qRound(height / (currGS.ctm.m22() / 72));
							imgs.append(img);
						}
						inlineImgDict = false;
					}
					break;
				case PdfOperator::gs:
				{
					if (!processedNamedGS.contains(args[0].GetName()))
					{
						if (args.size() == 1 && args[0].IsName() && extGStatesDict)
						{
							PdfObject* extGStateObj = extGStatesDict->FindKey(args[0].GetName());
							if (extGStateObj)
							{
								inspectExtGStateObj(extGStateObj, usedColorSpaces, hasTransparency, usedFonts, currGS);
							}
							else
							{
								qDebug() << "Named graphic state used with gs operator is undefined in current ExtGState. File:" << m_filename;
								return false;
							}
							processedNamedGS.append(args[0].GetName());
						}
						else
						{
							qDebug() << "Wrong syntax in applying extended graphic state (gs operator) or there's no ExtGState defined! File:" << m_filename;
							return false;
						}
					}
				}
				break;
				case PdfOperator::Tf:
				{
					if (processedNamedFont.contains(args[0].GetName()))
					{
						currGS.font.first = processedNamedFont.value(args[0].GetName());
						currGS.font.second = args[1].GetReal();
					}
					else
					{
						if (args.size() == 2 && args[0].IsName() && fontsDict)
						{
							PdfObject* fontObj = fontsDict->FindKey(args[0].GetName());
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
								qDebug() << "The specified font cannot be found in current Resources! File:" << m_filename;
								return false;
							}
						}
						else
						{
							qDebug() << "Wrong syntax in use of Tf operator or there's no Font defined in current Resources dictionary! File:" << m_filename;
							return false;
						}
					}
				}
				break;
				case PdfOperator::Unknown:
				default:
					break;
				}
				args.clear();
			}
			if (pdfContent.Type == PdfContentType::DoXObject)
			{
				args.clear();
				size_t stackSize = pdfContent.Stack.size();
				for (size_t i = 0; i < stackSize; ++i)
					args.append(pdfContent.Stack[stackSize - 1 - i]);
				if (!processedNamedXObj.contains(args[0].GetName()))
				{
					if (args.size() == 1 && args[0].IsName() && xObjectsDict)
					{
						PdfObject* xObject = xObjectsDict->FindKey(args[0].GetName());
						PdfDictionary* xObjectDict = (xObject && xObject->IsDictionary()) ? &(xObject->GetDictionary()) : nullptr;
						PdfObject* subtypeObject = xObjectDict ? xObjectDict->FindKey("Subtype") : nullptr;
						if (subtypeObject && subtypeObject->IsName())
						{
							if (subtypeObject->GetName() == "Image")
							{
								PdfObject* imgColorSpace = xObjectDict->FindKey("ColorSpace");
								if (imgColorSpace)
								{
									PDFColorSpace retval = getCSType(imgColorSpace);
									if (retval != CS_Unknown && !usedColorSpaces.contains(retval))
										usedColorSpaces.append(retval);
								}
								PdfObject* sMaskObj = xObjectDict->FindKey("SMask");
								if (sMaskObj)
									hasTransparency = true;
								PDFImage img;
								img.imgName = args[0].GetName().GetEscapedName().c_str();
								double width = xObjectDict->FindKey("Width")->GetReal();
								double height = xObjectDict->FindKey("Height")->GetReal();
								img.dpiX = qRound(width / (currGS.ctm.m11() / 72));
								img.dpiY = qRound(height / (currGS.ctm.m22() / 72));
								imgs.append(img);
							}
							else if (subtypeObject->GetName() == "Form")
							{
								std::unique_ptr<PdfXObjectForm> xObj;
								PdfXObject::TryCreateFromObject(*xObject, xObj);
								inspectCanvas(xObj.get(), usedColorSpaces, hasTransparency, usedFonts, imgs); // recursive call
							}
						}
						else
						{
							qDebug() << "Supplied external object is undefined! File:" << m_filename;
							return false;
						}
						processedNamedXObj.append(args[0].GetName());
					}
					else
					{
						qDebug() << "Wrong syntax for Do operator or there's no XObject defined! File:" << m_filename;
						return false;
					}
				}
			}
		}
	}
	catch (PdfError & e)
	{
		qDebug() << "Error in parsing content stream File:" << m_filename;
		e.PrintErrorMsg();
		return false;
	}
	return true;
#else
	// this method can be used to get used color spaces, detect transparency, and get used fonts in either PdfPage or PdfXObject
	PdfObject* colorSpaceRes { nullptr };
	PdfObject* xObjects { nullptr };
	PdfObject* transGroup { nullptr };
	PdfObject* extGState { nullptr };
	PdfObject* fontRes { nullptr };
	QMap<PdfName, PDFColorSpace> processedNamedCS;
	QMap<PdfName, PDFFont> processedNamedFont;
	QList<PdfName> processedNamedXObj;
	QList<PdfName> processedNamedGS;
	try
	{
		// get hold of a PdfObject pointer of this canvas
		// needed for the finding resources code below to work
		PdfPage* page = dynamic_cast<PdfPage*>(canvas);
		PdfObject* canvasObject = page ? (page->GetObject()) : ((dynamic_cast<PdfXObject*>(canvas))->GetObject());

		// find a resource with ColorSpace entry
		PdfObject* resources = canvas->GetResources();
		for (PdfObject* par = canvasObject; par && !resources; par = par->GetIndirectKey("Parent"))
		{
			resources = par->GetIndirectKey("Resources");
		}
		colorSpaceRes = resources ? resources->GetIndirectKey("ColorSpace") : nullptr;
		xObjects = resources ? resources->GetIndirectKey("XObject") : nullptr;
		extGState = resources ? resources->GetIndirectKey("ExtGState") : nullptr;
		fontRes = resources ? resources->GetIndirectKey("Font") : nullptr;

		// getting the transparency group of this content stream (if available)
		transGroup = canvasObject ? canvasObject->GetIndirectKey("Group") : nullptr;
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
		qDebug() << "Error in analyzing stream's resources. File:" << m_filename;
		e.PrintErrorMsg();
		return false;
	}

	try
	{
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
				switch (kwNameMap.value(kw, KW_Undefined))
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
						for (int i = 0; i < 6; ++i)
						{
							mt[i] = args[i].GetReal();
						}
						QTransform transMatrix(mt[0], mt[1], mt[2], mt[3], mt[4], mt[5]);
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
					for (uint i = 0; i < dashArr.size(); ++i)
						currGS.dashPattern.first.append(dashArr[i].GetNumber());
					currGS.dashPattern.second = args[1].GetNumber();
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
					for (int i = 0; i < args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case KW_RG:
					if (!usedColorSpaces.contains(CS_DeviceRGB))
						usedColorSpaces.append(CS_DeviceRGB);
					currGS.strokeCS = CS_DeviceRGB;
					currGS.strokeColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.strokeColor.append(args[i].GetReal());
					break;
				case KW_k:
					if (!usedColorSpaces.contains(CS_DeviceCMYK))
						usedColorSpaces.append(CS_DeviceCMYK);
					currGS.fillCS = CS_DeviceCMYK;
					currGS.fillColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case KW_K:
					if (!usedColorSpaces.contains(CS_DeviceCMYK))
						usedColorSpaces.append(CS_DeviceCMYK);
					currGS.strokeCS = CS_DeviceCMYK;
					currGS.strokeColor.clear();
					for (int i = 0; i < args.size(); ++i)
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
							for (int i = 0; i < 3; ++i)
								currGS.fillColor.append(0);
							if (!usedColorSpaces.contains(CS_DeviceRGB))
								usedColorSpaces.append(CS_DeviceRGB);
						}
						else if (args[0].GetName() == "DeviceCMYK")
						{
							currGS.fillCS = CS_DeviceCMYK;
							currGS.fillColor.clear();
							for (int i = 0; i < 3; ++i)
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
									qDebug() << "Supplied colorspace is undefined! File:" << m_filename;
									return false;
								}
							}
						}
					}
					else
					{
						qDebug() << "Wrong syntax in specifying color space! File:" << m_filename;
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
							for (int i = 0; i < 3; ++i)
								currGS.strokeColor.append(0);
							if (!usedColorSpaces.contains(CS_DeviceRGB))
								usedColorSpaces.append(CS_DeviceRGB);
						}
						else if (args[0].GetName() == "DeviceCMYK")
						{
							currGS.fillCS = CS_DeviceCMYK;
							currGS.strokeColor.clear();
							for (int i = 0; i < 3; ++i)
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
									qDebug() << "Supplied colorspace is undefined! File:" << m_filename;
									return false;
								}
							}
						}
					}
					else
					{
						qDebug() << "Wrong syntax in specifying color space! File:" << m_filename;
						return false;
					}
					}
					break;
				case KW_sc:
					currGS.fillColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.fillColor.append(args[i].GetReal());
					break;
				case KW_SC:
					currGS.strokeColor.clear();
					for (int i = 0; i < args.size(); ++i)
						currGS.strokeColor.append(args[i].GetReal());
					break;
				case KW_scn:
					currGS.fillColor.clear();
					for (int i = 0; i < args.size(); ++i)
					{
						if (args[i].IsReal() || args[i].IsNumber())
							currGS.fillColor.append(args[i].GetReal());
					}
					break;
				case KW_SCN:
					currGS.strokeColor.clear();
					for (int i = 0; i < args.size(); ++i)
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
							PdfObject* subtypeObject = xObject ? xObject->GetIndirectKey("Subtype") : nullptr;
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
								qDebug() << "Supplied external object is undefined! File:" << m_filename;
								return false;
							}
							processedNamedXObj.append(args[0].GetName());
						}
						else
						{
							qDebug() << "Wrong syntax for Do operator or there's no XObject defined! File:" << m_filename;
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
							int csIdx = args.contains(colorspace) ? args.indexOf(colorspace) : args.indexOf(cs);
							if (args[csIdx + 1].IsName())
							{
								PdfName csName = args[csIdx + 1].GetName();
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
							int heightIdx = args.contains(height) ? args.indexOf(height) : args.indexOf(h);
							int widthIdx = args.contains(width) ? args.indexOf(width) : args.indexOf(w);
							double height = args[heightIdx + 1].GetReal();
							double width = args[widthIdx + 1].GetReal();
							PDFImage img;
							img.imgName = "Inline Image";
							img.dpiX = qRound(width / (currGS.ctm.m11() / 72));
							img.dpiY = qRound(height / (currGS.ctm.m22() / 72));
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
								qDebug() << "Named graphic state used with gs operator is undefined in current ExtGState. File:" << m_filename;
								return false;
							}
							processedNamedGS.append(args[0].GetName());
						}
						else
						{
							qDebug() << "Wrong syntax in applying extended graphic state (gs operator) or there's no ExtGState defined! File:" << m_filename;
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
								qDebug() << "The specified font cannot be found in current Resources! File:" << m_filename;
								return false;
							}
						}
						else
						{
							qDebug() << "Wrong syntax in use of Tf operator or there's no Font defined in current Resources dictionary! File:" << m_filename;
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
		qDebug() << "Error in parsing content stream File:" << m_filename;
		e.PrintErrorMsg();
		return false;
	}
	return true;
#endif
}

void PDFAnalyzer::inspectExtGStateObj(PdfObject* extGStateObj, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, PDFGraphicState & currGS)
{
#if (PODOFO_VERSION >= PODOFO_MAKE_VERSION(0, 10, 0))
	PdfDictionary* extGStateDict = extGStateObj->IsDictionary() ? &(extGStateObj->GetDictionary()) : nullptr;
	PdfObject* bmObj = extGStateDict ? extGStateDict->FindKey("BM") : nullptr;
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
		for (uint i = 0; i < arr.GetSize(); ++i)
			currGS.blendModes.append(arr[i].GetName().GetEscapedName().c_str());
		if (arr[0].IsName() && !(arr[0].GetName() == "Normal" || arr[0].GetName() == "Compatible"))
			hasTransparency = true;
	}

	PdfObject* caObj = extGStateDict ? extGStateDict->FindKey("ca") : nullptr;
	if (caObj && (caObj->IsRealStrict() || caObj->IsNumber()))
	{
		currGS.fillAlphaConstant = caObj->GetReal();
		if (caObj->GetReal() < 1)
			hasTransparency = true;
	}

	PdfObject* cAObj = extGStateDict ? extGStateDict->FindKey("CA") : nullptr;
	if (cAObj && (cAObj->IsRealStrict() || cAObj->IsNumber()))
	{
		if (cAObj->GetReal() < 1)
			hasTransparency = true;
	}

	PdfObject* sMaskObj = extGStateDict ? extGStateDict->FindKey("SMask") : nullptr;
	if (sMaskObj && !(sMaskObj->IsName() && sMaskObj->GetName() == "None"))
		hasTransparency = true;

	PdfObject* fontObj = extGStateDict ? extGStateDict->FindKey("Font") : nullptr;
	if (fontObj && fontObj->IsArray())
	{
		PdfArray arr = fontObj->GetArray();
		if (arr[0].IsReference())
		{
			PdfReference ref = arr[0].GetReference();
			PdfObject* fontObject = m_pdfdoc->GetObjects().GetObject(ref);
			if (fontObject)
			{
				PDFFont font = getFontInfo(fontObject);
				usedFonts.append(font);
				currGS.font.first = font;
				currGS.font.second = arr[1].GetReal();
			}
		}
	}

	PdfObject* lwObj = extGStateDict ? extGStateDict->FindKey("LW") : nullptr;
	if (lwObj)
		currGS.lineWidth = lwObj->GetReal();

	PdfObject* lcObj = extGStateDict ? extGStateDict->FindKey("LC") : nullptr;
	if (lcObj)
		currGS.lineCap = lcObj->GetNumber();

	PdfObject* ljObj = extGStateDict ? extGStateDict->FindKey("LJ") : nullptr;
	if (ljObj)
		currGS.lineJoin = ljObj->GetNumber();

	PdfObject* mlObj = extGStateDict ? extGStateDict->FindKey("ML") : nullptr;
	if (mlObj)
		currGS.miterLimit = mlObj->GetReal();

	PdfObject* dObj = extGStateDict ? extGStateDict->FindKey("D") : nullptr;
	if (dObj)
	{
		PdfObject dObjA = dObj->GetArray()[0];
		PdfArray dashArr = dObjA.GetArray();
		currGS.dashPattern.first.clear();
		for (uint i = 0; i < dashArr.GetSize(); ++i)
			currGS.dashPattern.first.append(dashArr[i].GetNumber());
		PdfObject dObjB = dObj->GetArray()[1];
		currGS.dashPattern.second = dObjB.GetNumber();
	}
#else
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
		for (uint i = 0; i < arr.GetSize(); ++i)
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
			PdfObject* fontObject = m_pdfdoc->GetObjects().GetObject(ref);
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
		for (uint i = 0; i < dashArr.GetSize(); ++i)
			currGS.dashPattern.first.append(dashArr[i].GetNumber());
		PdfObject dObjB = dObj->GetArray()[1];
		currGS.dashPattern.second = dObjB.GetNumber();
	}
#endif
}

PDFFont PDFAnalyzer::getFontInfo(PdfObject* fontObj)
{
#if (PODOFO_VERSION >= PODOFO_MAKE_VERSION(0, 10, 0))
	PDFFont currFont;
	const PdfDictionary* fontDict = fontObj->IsDictionary() ? &(fontObj->GetDictionary()) : nullptr;
	if (!fontDict)
		return currFont;

	const PdfObject* subtype = fontDict->FindKey("Subtype");
	if (!subtype || !subtype->IsName())
		return currFont;

	const PdfObject* fontDesc = fontDict->FindKey("FontDescriptor");
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
		fontDesc = nullptr;
	}
	else if (subtype->GetName() == "Type0")
	{
		const PdfObject* descendantFonts = fontDict->FindKey("DescendantFonts");
		if (descendantFonts && descendantFonts->IsArray())
		{
			const PdfReference& refDescFont = descendantFonts->GetArray()[0].GetReference();
			PdfObject* descendantFont = descendantFonts->GetDocument()->GetObjects().GetObject(refDescFont);
			PdfDictionary* descendantFontDict = (descendantFont && descendantFont->IsDictionary()) ? &(descendantFont->GetDictionary()) : nullptr;
			const PdfObject* subtypeDescFont = descendantFontDict->FindKey("Subtype");
			fontDesc = &(descendantFontDict->MustGetKey("FontDescriptor"));
			if (subtypeDescFont && subtypeDescFont->IsName())
			{
				if (subtypeDescFont->GetName() == "CIDFontType0")
					currFont.fontType = F_CIDFontType0;
				else if (subtypeDescFont->GetName() == "CIDFontType2")
					currFont.fontType = F_CIDFontType2;
			}
		}
	}

	const PdfDictionary* fontDescDict = (fontDesc && fontDesc->IsDictionary()) ? &(fontDesc->GetDictionary()) : nullptr;
	if (fontDescDict)
	{
		const PdfObject* fontFile = fontDescDict->FindKey("FontFile");
		const PdfObject* fontFile2 = fontDescDict->FindKey("FontFile2");
		const PdfObject* fontFile3 = fontDescDict->FindKey("FontFile3");
		if (fontFile && fontFile->HasStream())
			currFont.isEmbedded = true;
		if (fontFile2 && fontFile2->HasStream())
			currFont.isEmbedded = true;
		if (fontFile3 && fontFile3->HasStream())
		{
			currFont.isEmbedded = true;
			const PdfDictionary* fontFile3Dict = fontFile3->IsDictionary() ? &(fontFile3->GetDictionary()) : nullptr;
			const PdfObject* ff3Subtype = fontFile3Dict ? fontFile3Dict->FindKey("Subtype") : nullptr;
			if (ff3Subtype && ff3Subtype->IsName() && ff3Subtype->GetName() == "OpenType")
				currFont.isOpenType = true;
		}
	}
	return currFont;
#else
	PDFFont currFont;
	PdfObject* subtype = fontObj->GetIndirectKey("Subtype");
	if (!subtype || !subtype->IsName())
		return currFont;

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
		fontDesc = nullptr;
	}
	else if (subtype->GetName() == "Type0")
	{
		PdfObject* descendantFonts = fontObj->GetIndirectKey("DescendantFonts");
		if (descendantFonts && descendantFonts->IsArray())
		{
			PdfReference refDescFont = descendantFonts->GetArray()[0].GetReference();
			PdfObject* descendantFont = descendantFonts->GetOwner()->GetObject(refDescFont);
			PdfObject* subtypeDescFont = descendantFont->GetIndirectKey("Subtype");
			fontDesc = descendantFont->MustGetIndirectKey("FontDescriptor");
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
	return currFont;
#endif
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
