/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pslib.cpp  -  description
                             -------------------
    begin                : Sat May 26 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pslib.h"

#include <QFileInfo>
#include <QImage>
#include <QColor>
#include <QFontInfo>
#include <QList>
#include <QByteArray>
#include <QList>
#include <cstdlib>
#include <QRegExp>
#include <QBuffer>
#include <QStack>

#include "cmsettings.h"
#include "commonstrings.h"
#include "scconfig.h"
#include "pluginapi.h"
#include "prefsmanager.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scfonts.h"
#include "selection.h"
#include <cmath>
#include "util.h"
#include "util_formats.h"
#include "util_math.h"
#include "multiprogressdialog.h"
#include "scribusapp.h"
#include "scpattern.h"
#include "sccolorengine.h"
#include "scstreamfilter_ascii85.h"
#include "scstreamfilter_flate.h"

#include "text/nlsconfig.h"

PSLib::PSLib(PrintOptions &options, bool psart, SCFonts &AllFonts, QMap<QString, QMap<uint, FPointArray> > DocFonts, ColorList DocColors, bool pdf, bool spot)
{
	Options = options;
	optimization = OptimizeCompat;
	usingGUI=ScCore->usingGUI();
	abortExport=false;
	QString tmp, tmp2, tmp3, tmp4, CHset;
	QStringList wt;
	Seiten = 0;
	User = "";
	Creator = "Scribus" + QString(VERSION);
	Titel = "";
	FillColor = "0.0 0.0 0.0 0.0";
	StrokeColor = "0.0 0.0 0.0 0.0";
	Header = psart ? "%!PS-Adobe-3.0\n" : "%!PS-Adobe-3.0 EPSF-3.0\n";
	BBox = "";
	BBoxH = "";
	Art = psart;
	isPDF = pdf;
	UsedFonts.clear();
	Fonts = "";
	FontDesc = "";
	GraySc = false;
	DoSep = false;
	abortExport = false;
	useSpotColors = spot;
	GrayCalc =  "/setcmykcolor {exch 0.11 mul add exch 0.59 mul add exch 0.3 mul add\n";
	GrayCalc += "               dup 1 gt {pop 1} if 1 exch sub oldsetgray} bind def\n";
	GrayCalc += "/setrgbcolor {0.11 mul exch 0.59 mul add exch 0.3 mul add\n";
	GrayCalc += "              oldsetgray} bind def\n";
	Farben = "";
	FNamen = "";
	CMYKColor cmykValues;
	ColorList::Iterator itf;
	int c, m, y, k;
	bool erst = true;
	colorsToUse = DocColors;
	spotMap.clear();
	colorDesc = "";
	for (itf = DocColors.begin(); itf != DocColors.end(); ++itf)
	{
		if (((DocColors[itf.key()].isSpotColor()) || (DocColors[itf.key()].isRegistrationColor())) && (useSpotColors))
		{
			ScColorEngine::getCMYKValues(DocColors[itf.key()], DocColors.document(), cmykValues);
			cmykValues.getValues(c, m, y, k);
			colorDesc += "/Spot"+PSEncode(itf.key())+" { [ /Separation (";
			if (DocColors[itf.key()].isRegistrationColor())
				colorDesc += "All";
			else
				colorDesc += itf.key();
			colorDesc += ")\n";
			colorDesc += "/DeviceCMYK\n{\ndup "+ToStr(static_cast<double>(c) / 255)+"\nmul exch dup ";
			colorDesc += ToStr(static_cast<double>(m) / 255)+"\nmul exch dup ";
			colorDesc += ToStr(static_cast<double>(y) / 255)+"\nmul exch ";
			colorDesc += ToStr(static_cast<double>(k) / 255)+" mul }] setcolorspace setcolor} bind def\n";
			spotMap.insert(itf.key(), "Spot"+PSEncode(itf.key()));
		}
		if ((itf.key() != "Cyan") && (itf.key() != "Magenta") && (itf.key() != "Yellow") && (itf.key() != "Black") && DocColors[itf.key()].isSpotColor())
		{
			ScColorEngine::getCMYKValues(DocColors[itf.key()], DocColors.document(), cmykValues);
			cmykValues.getValues(c, m, y, k);
			if (!erst)
			{
				Farben += "%%+ ";
				FNamen += "%%+ ";
			}
			Farben += ToStr(static_cast<double>(c) / 255) + " " + ToStr(static_cast<double>(m) / 255) + " ";
			Farben += ToStr(static_cast<double>(y) / 255) + " " + ToStr(static_cast<double>(k) / 255) + " (" + itf.key() + ")\n";
			FNamen += "(" + itf.key() + ")\n";
			erst = false;
		}
	}
	QMap<QString, QMap<uint, FPointArray> >::Iterator it;
	int a = 0;
	for (it = DocFonts.begin(); it != DocFonts.end(); ++it)
	{
/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
		ScFace::FontType type = AllFonts[it.key()].type();

		if ((type == ScFace::TTF) || (AllFonts[it.key()].isOTF()) || (AllFonts[it.key()].subset()))
		{
			QMap<uint, FPointArray>& RealGlyphs(it.value());
			FontDesc += "/"+AllFonts[it.key()].psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+
					" "+IToStr(RealGlyphs.count()+1)+" dict def\n";
			FontDesc += AllFonts[it.key()].psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+" begin\n";
			QMap<uint,FPointArray>::Iterator ig;
			for (ig = RealGlyphs.begin(); ig != RealGlyphs.end(); ++ig)
			{
				FontDesc += "/G"+IToStr(ig.key())+" { newpath\n";
				FPoint np, np1, np2;
				bool nPath = true;
				if (ig.value().size() > 3)
				{
					for (uint poi = 0; poi < ig.value().size()-3; poi += 4)
					{
						if (ig.value().point(poi).x() > 900000)
						{
							FontDesc += "cl\n";
							nPath = true;
							continue;
						}
						if (nPath)
						{
							np = ig.value().point(poi);
							FontDesc += ToStr(np.x()) + " " + ToStr(-np.y()) + " m\n";
							nPath = false;
						}
						np = ig.value().point(poi+1);
						np1 = ig.value().point(poi+3);
						np2 = ig.value().point(poi+2);
						FontDesc += ToStr(np.x()) + " " + ToStr(-np.y()) + " " +
								ToStr(np1.x()) + " " + ToStr(-np1.y()) + " " +
								ToStr(np2.x()) + " " + ToStr(-np2.y()) + " cu\n";
					}
				}
				FontDesc += "cl\n} bind def\n";
			}
			FontDesc += "end\n";
		}
		else
		{
			UsedFonts.insert(it.key(), "/Fo"+IToStr(a));
			Fonts += "/Fo"+IToStr(a)+" /"+AllFonts[it.key()].psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )+" findfont definefont pop\n";
			if (AllFonts[it.key()].embedPs())
			{
				QString tmp;
				if(AllFonts[it.key()].EmbedFont(tmp))
				{
					FontDesc += "%%BeginFont: " + AllFonts[it.key()].psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ) + "\n";
					FontDesc += tmp + "\n%%EndFont\n";
				}
			}
			GListe gl;
			AllFonts[it.key()].glyphNames(gl);
			GlyphsOfFont.insert(it.key(), gl);
			a++;
		}
	}
	Prolog = "%%BeginProlog\n";
	Prolog += "/Scribusdict 100 dict def\n";
	Prolog += "Scribusdict begin\n";
	Prolog += "/sp {showpage} bind def\n";
	Prolog += "/oldsetgray /setgray load def\n";
	Prolog += "/cmyk {setcmykcolor} def\n";
	Prolog += "/m {moveto} bind def\n";
	Prolog += "/l {lineto} bind def\n";
	Prolog += "/li {lineto} bind def\n";
	Prolog += "/cu {curveto} bind def\n";
	Prolog += "/cl {closepath} bind def\n";
	Prolog += "/gs {gsave} bind def\n";
	Prolog += "/gr {grestore} bind def\n";
	Prolog += "/tr {translate} bind def\n";
	Prolog += "/ro {rotate} bind def\n";
	Prolog += "/sh {show} bind def\n";
	Prolog += "/shg {setcmykcolor moveto glyphshow} def\n";
	Prolog += "/shgsp {moveto glyphshow} def\n";
	Prolog += "/sc {scale} bind def\n";
	Prolog += "/se {selectfont} bind def\n";
	Prolog += "/sf {setfont} bind def\n";
	Prolog += "/sw {setlinewidth} bind def\n";
	Prolog += "/f  {findfont} bind def\n";
	Prolog += "/fi {fill} bind def\n";
	Prolog += "/st {stroke} bind def\n";
	Prolog += "/shgf {gs dup scale begin cvx exec fill end gr} bind def\n";
	Prolog += "/shgs {gs dup 1 exch div currentlinewidth mul sw dup scale\n";
	Prolog += "       begin cvx exec st end gr} bind def\n";
	Prolog += "/bEPS {\n";
	Prolog += "    /b4_Inc_state save def\n";
	Prolog += "    /dict_count countdictstack def\n";
	Prolog += "    /op_count count 1 sub def\n";
	Prolog += "    userdict begin\n";
	Prolog += "    /showpage { } def\n";
	Prolog += "    0 setgray 0 setlinecap\n";
	Prolog += "    1 setlinewidth 0 setlinejoin\n";
	Prolog += "    10 setmiterlimit [ ] 0 setdash newpath\n";
	Prolog += "    /languagelevel where\n";
	Prolog += "    {pop languagelevel\n";
	Prolog += "    1 ne\n";
	Prolog += "    {false setstrokeadjust false setoverprint\n";
	Prolog += "    } if } if } bind def\n";
	Prolog += "/eEPS { count op_count sub {pop} repeat\n";
	Prolog += "    countdictstack dict_count sub {end} repeat\n";
	Prolog += "    b4_Inc_state restore } bind def\n";
	Prolog += "    end\n";
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks))
		Prolog += "/rb { [ /Separation (All)\n/DeviceCMYK { dup 0 mul exch dup 0 mul exch dup 0 mul exch 1 mul }\n] setcolorspace setcolor} bind def\n";
	Prolog += "%%EndProlog\n";
}

void PSLib::PutStream(const QString& c)
{
	spoolStream.writeRawData(c.toUtf8().data(), c.length());
}

void PSLib::PutStream(const QByteArray& array, bool hexEnc)
{
	if(hexEnc)
		WriteASCII85Bytes(array);
	else
		spoolStream.writeRawData(array.data(), array.size());
}

void PSLib::PutStream(const char* array, int length, bool hexEnc)
{
	if(hexEnc)
		WriteASCII85Bytes((const unsigned char*) array, length);
	else
		spoolStream.writeRawData(array, length);
}

bool PSLib::PutImageToStream(ScImage& image, int plate)
{
	bool writeSucceed = false;
	ScASCII85EncodeFilter asciiEncode(&spoolStream);
	ScFlateEncodeFilter   flateEncode(&asciiEncode);
	if (flateEncode.openFilter())
	{
		writeSucceed  = image.writePSImageToFilter(&flateEncode, plate);
		writeSucceed &= flateEncode.closeFilter();
	}
	return writeSucceed;
}

bool PSLib::PutImageToStream(ScImage& image, const QByteArray& mask, int plate)
{
	bool writeSucceed = false;
	ScASCII85EncodeFilter asciiEncode(&spoolStream);
	ScFlateEncodeFilter   flateEncode(&asciiEncode);
	if (flateEncode.openFilter())
	{
		writeSucceed  = image.writePSImageToFilter(&flateEncode, mask, plate);
		writeSucceed &= flateEncode.closeFilter();
	}
	return writeSucceed;
}

bool PSLib::PutImageDataToStream(const QByteArray& image)
{
	bool writeSucceed = false;
	ScASCII85EncodeFilter asciiEncode(&spoolStream);
	ScFlateEncodeFilter   flateEncode(&asciiEncode);
	if (flateEncode.openFilter())
	{
		writeSucceed  = flateEncode.writeData(image, image.size());
		writeSucceed &= flateEncode.closeFilter();
	}
	return writeSucceed;
}

bool PSLib::PutInterleavedImageMaskToStream(const QByteArray& image, const QByteArray& mask, bool gray)
{
	int pending = 0;
	int bIndex  = 0;
	unsigned char bytes[1505];
	const unsigned char* imageData = (const unsigned char*) image.constData();
	const unsigned char* maskData  = (const unsigned char*) mask.constData();
	bool  writeSuccess = true;

	int channels = gray ? 1 : 4;
	int pixels   = image.size() / channels;
	assert((image.size() % channels) == 0);
	assert( mask.size() >= pixels );

	ScASCII85EncodeFilter asciiEncode(&spoolStream);
	ScFlateEncodeFilter   flateEncode(&asciiEncode);
	if (!flateEncode.openFilter()) 
		return false;

	for (int i = 0; i < pixels; ++i)
	{
		bIndex = 4 *i;
		bytes[pending++] = maskData [i];
		bytes[pending++] = *imageData++; // cyan/black
		if (channels > 1)
		{
			bytes[pending++] = *imageData++; // magenta
			bytes[pending++] = *imageData++; // yellow
			bytes[pending++] = *imageData++; // green
		}
		if (pending >= 1500)
		{
			writeSuccess &= flateEncode.writeData((const char* ) bytes, pending);
			pending = 0;
		}
	}
	// To close the stream
	if (pending > 0)
		writeSuccess &= flateEncode.writeData((const char* ) bytes, pending);
	writeSuccess &= flateEncode.closeFilter();
	return writeSuccess;
}

void PSLib::WriteASCII85Bytes(const QByteArray& array)
{
	WriteASCII85Bytes((const unsigned char*) array.data(), array.size());
}

void PSLib::WriteASCII85Bytes(const unsigned char* array, int length)
{
	ScASCII85EncodeFilter filter(&spoolStream);
	filter.openFilter();
	filter.writeData((const char*) array, length);
	filter.closeFilter();
}

QString PSLib::ToStr(double c)
{
	QString cc;
	return cc.setNum(c);
}

QString PSLib::IToStr(int c)
{
	QString cc;
	return cc.setNum(c);
}

void PSLib::PS_set_Info(QString art, QString was)
{
	if (art == "Author")
		User = was;
	if (art == "Creator")
		Creator = was;
	if (art == "Title")
		Titel = was;
}

bool PSLib::PS_set_file(QString fn)
{
	Spool.setFileName(fn);
	if (Spool.exists())
		Spool.remove();
	bool ret = Spool.open(QIODevice::WriteOnly);
	spoolStream.setDevice(&Spool);
	return ret;
}

bool PSLib::PS_begin_doc(ScribusDoc *doc, double x, double y, double breite, double hoehe, int numpage, bool doDev, bool sep, bool farb, bool ic, bool gcr, bool over)
{
	m_Doc = doc;
	PutStream(Header);
	PutStream("%%For: " + User + "\n");
	PutStream("%%Title: " + Titel + "\n");
	PutStream("%%Creator: " + Creator + "\n");
	PutStream("%%Pages: " + IToStr(numpage) + "\n");
	BBox = "%%BoundingBox: " + IToStr(qRound(x)) + " " + IToStr(qRound(y)) + " " + IToStr(qRound(breite)) + " " + IToStr(qRound(hoehe)) + "\n";
	BBoxH = "%%HiResBoundingBox: " + ToStr(x) + " " + ToStr(y) + " " + ToStr(breite) + " " + ToStr(hoehe) + "\n";
 // 	if (!Art)
//	{
		PutStream(BBox);
		PutStream(BBoxH);
//	}
	if (!FNamen.isEmpty())
		PutStream("%%DocumentCustomColors: "+FNamen);
	if (!Farben.isEmpty())
		PutStream("%%CMYKCustomColor: "+Farben);
	PutStream("%%LanguageLevel: 3\n");
	PutStream("%%EndComments\n");
	PutStream(Prolog);
	PutStream("%%BeginSetup\n");
	PutStream("/pdfmark where {pop} {userdict /pdfmark /cleartomark load put} ifelse\n");
	if (!FontDesc.isEmpty())
		PutStream(FontDesc);
	if ((!colorDesc.isEmpty()) && (!sep))
		PutStream(colorDesc);
//	PutStream("Scribusdict begin\n");
	PutStream(Fonts);
	if (GraySc)
		PutStream(GrayCalc);
	if (over)
	{
		PutStream("true setoverprint\n");
		PutStream("true setoverprintmode\n");
	}
	QStringList patterns = m_Doc->getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = m_Doc->docPatterns[patterns[c]];
		for (int em = 0; em < pa.items.count(); ++em)
		{
			PageItem* item = pa.items.at(em);
			if ((item->asImageFrame()) && (item->PicAvail) && (!item->Pfile.isEmpty()) && (item->printEnabled()) && (!sep) && (farb))
			{
				if (!PS_ImageData(item, item->Pfile, item->itemName(), item->IProfile, item->UseEmbedded, ic))
					return false;
			}
		}
		PutStream("/Pattern"+patterns[c]+" 8 dict def\n");
		PutStream("Pattern"+patterns[c]+" begin\n");
		PutStream("/PatternType 1 def\n");
		PutStream("/PaintType 1 def\n");
		PutStream("/TilingType 1 def\n");
		PutStream("/BBox [ 0 0 "+ToStr(pa.width)+" "+ToStr(pa.height)+"] def\n");
		PutStream("/XStep "+ToStr(pa.width)+" def\n");
		PutStream("/YStep "+ToStr(pa.height)+" def\n");
		PutStream("/PaintProc {\n");
		QIODevice *spStream = spoolStream.device();
		QByteArray buf;
		// Qt4 QBuffer b(buf);
		QBuffer b(&buf);
		b.open( QIODevice::WriteOnly );
		spoolStream.setDevice(&b);
		QStack<PageItem*> groupStack;
		for (int em = 0; em < pa.items.count(); ++em)
		{
			PageItem* item = pa.items.at(em);
			if (item->isGroupControl)
			{
				PS_save();
				FPointArray cl = item->PoLine.copy();
				QMatrix mm;
				mm.translate(item->gXpos, item->gYpos - pa.height);
				mm.rotate(item->rotation());
				cl.map( mm );
				SetClipPath(&cl);
				PS_closepath();
				PS_clip(true);
				groupStack.push(item->groupsLastItem);
				continue;
			}
			PutStream("{\n");
			PS_save();
			PS_translate(item->gXpos, pa.height - item->gYpos);
			ProcessItem(m_Doc, m_Doc->Pages->at(0), item, 0, sep, farb, ic, gcr, false, true, true);
			PS_restore();
			if (groupStack.count() != 0)
			{
				while (item == groupStack.top())
				{
					PS_restore();
					groupStack.pop();
					if (groupStack.count() == 0)
						break;
				}
			}
			PutStream("} exec\n");
		}
		spoolStream.setDevice(spStream);
		PutStream(buf);
		PutStream("} def\n");
		PutStream("end\n");
	}
//	PutStream("end\n");
//	PutStream("%%EndSetup\n");
	Prolog = "";
	FontDesc = "";
	return true;
}

QString PSLib::PSEncode(QString in)
{
	static QRegExp badchars("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]");
	QString tmp = "";
	tmp = in.simplified().replace( badchars, "_" );
	return tmp;
}

void PSLib::PS_TemplateStart(QString Name)
{
	PutStream("/"+PSEncode(Name)+"\n{\n");
}

void PSLib::PS_UseTemplate(QString Name)
{
	PutStream(PSEncode(Name)+"\n");
}

void PSLib::PS_TemplateEnd()
{
	PutStream("} bind def\n");
}

void PSLib::PS_begin_page(Page* pg, MarginStruct* Ma, bool Clipping)
{
	double bleedRight = 0.0;
	double bleedLeft = 0.0;
	double markOffs = 0.0;
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks))
		markOffs = 20.0 + Options.markOffset;
	if (m_Doc->pageSets[m_Doc->currentPageLayout].Columns == 1)
	{
		bleedRight = Options.bleeds.Right;
		bleedLeft = Options.bleeds.Left;
	}
	else
	{
		if (m_Doc->locationOfPage(pg->pageNr()) == LeftPage)
		{
			bleedRight = Options.bleeds.Right;
			bleedLeft = Options.bleeds.Left;
		}
		else if (m_Doc->locationOfPage(pg->pageNr()) == RightPage)
		{
			bleedRight = Options.bleeds.Left;
			bleedLeft = Options.bleeds.Right;
		}
		else
		{
			bleedRight = Options.bleeds.Left;
			bleedLeft = Options.bleeds.Left;
		}
	}
	double maxBoxX = pg->width()+bleedLeft+bleedRight+markOffs*2.0;
	double maxBoxY = pg->height()+Options.bleeds.Bottom+Options.bleeds.Top+markOffs*2.0;
	Seiten++;
	PutStream("%%Page: " + IToStr(Seiten) + " " + IToStr(Seiten) + "\n");
	PutStream("%%PageOrientation: ");
// when creating EPS files determine the orientation from the bounding box
  	if (!Art)
	{
		if ((pg->width() - Ma->Left - Ma->Right) <= (pg->height() - Ma->Bottom - Ma->Top))
			PutStream("Portrait\n");
		else
			PutStream("Landscape\n");
	}
	else
	{
		if (pg->PageOri == 0)
			PutStream("Portrait\n");
		else
			PutStream("Landscape\n");
	}
	PutStream("%%PageBoundingBox: 0 0 "+IToStr(qRound(maxBoxX))+" "+IToStr(qRound(maxBoxY))+"\n");
	PutStream("%%PageCropBox: "+ToStr(bleedLeft+markOffs)+" "+ToStr(Options.bleeds.Bottom+markOffs)+" "+ToStr(maxBoxX-bleedRight-markOffs*2.0)+" "+ToStr(maxBoxY-Options.bleeds.Top-markOffs*2.0)+"\n");
	PutStream("Scribusdict begin\n");
	PutStream("save\n");
	if (Clipping)
		PutStream(PDev);
  	PutStream("/DeviceCMYK setcolorspace\n");
	if ((Art) && (Options.setDevParam))
  	{
		PutStream("<< /PageSize [ "+ToStr(maxBoxX)+" "+ToStr(maxBoxY)+" ]\n");
		PutStream(">> setpagedevice\n");
	}
	PutStream(ToStr(bleedLeft+markOffs)+" "+ToStr(Options.bleeds.Bottom+markOffs)+" tr\n");
	ActPage = pg;
	if (Clipping)
	{
		PDev = ToStr(Ma->Left) + " " + ToStr(Ma->Bottom) + " m\n";
		PDev += ToStr(pg->width() - Ma->Right) + " " + ToStr(Ma->Bottom) + " li\n";
		PDev += ToStr(pg->width() - Ma->Right) + " " + ToStr(pg->height() - Ma->Top) + " li\n";
		PDev += ToStr(Ma->Left) + " " + ToStr(pg->height() - Ma->Top) + " li cl clip newpath\n";
	}
}

void PSLib::PS_end_page()
{
	PutStream("%%PageTrailer\nrestore\n");
	double markOffs = 0.0;
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks))
		markOffs = 20.0 + Options.markOffset;
	double bleedRight;
	double bleedLeft;
	if (m_Doc->pageSets[m_Doc->currentPageLayout].Columns == 1)
	{
		bleedRight = Options.bleeds.Right;
		bleedLeft = Options.bleeds.Left;
	}
	else
	{
		if (m_Doc->locationOfPage(ActPage->pageNr()) == LeftPage)
		{
			bleedRight = Options.bleeds.Right;
			bleedLeft = Options.bleeds.Left;
		}
		else if (m_Doc->locationOfPage(ActPage->pageNr()) == RightPage)
		{
			bleedRight = Options.bleeds.Left;
			bleedLeft = Options.bleeds.Right;
		}
		else
		{
			bleedRight = Options.bleeds.Left;
			bleedLeft = Options.bleeds.Left;
		}
	}
	double maxBoxX = ActPage->width()+bleedLeft+bleedRight+markOffs*2.0;
	double maxBoxY = ActPage->height()+Options.bleeds.Bottom+Options.bleeds.Top+markOffs*2.0;
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks))
	{
		PutStream("gs\n");
		PS_setlinewidth(0.5);
		PutStream("[] 0 setdash\n");
		PutStream("0 setlinecap\n");
		PutStream("0 setlinejoin\n");
		PutStream("1 rb\n");
		if (Options.cropMarks)
		{
		// Bottom Left
			PutStream("0 "+ToStr(markOffs+Options.bleeds.Bottom)+" m\n");
			PutStream(ToStr(20.0)+" "+ToStr(markOffs+Options.bleeds.Bottom)+" li\n");
			PutStream("st\n");
			PutStream(ToStr(markOffs+bleedLeft)+" 0 m\n");
			PutStream(ToStr(markOffs+bleedLeft)+" 20 li\n");
			PutStream("st\n");
		// Top Left
			PutStream("0 "+ToStr(maxBoxY-Options.bleeds.Top-markOffs)+" m\n");
			PutStream(ToStr(20.0)+" "+ToStr(maxBoxY-Options.bleeds.Top-markOffs)+" li\n");
			PutStream("st\n");
			PutStream(ToStr(markOffs+bleedLeft)+" "+ToStr(maxBoxY)+" m\n");
			PutStream(ToStr(markOffs+bleedLeft)+" "+ToStr(maxBoxY-20.0)+" li\n");
			PutStream("st\n");
		// Bottom Right
			PutStream(ToStr(maxBoxX)+" "+ToStr(markOffs+Options.bleeds.Bottom)+" m\n");
			PutStream(ToStr(maxBoxX-20.0)+" "+ToStr(markOffs+Options.bleeds.Bottom)+" li\n");
			PutStream("st\n");
			PutStream(ToStr(maxBoxX-bleedRight-markOffs)+" "+ToStr(0.0)+" m\n");
			PutStream(ToStr(maxBoxX-bleedRight-markOffs)+" "+ToStr(20.0)+" li\n");
			PutStream("st\n");
		// Top Right
			PutStream(ToStr(maxBoxX)+" "+ToStr(maxBoxY-Options.bleeds.Top-markOffs)+" m\n");
			PutStream(ToStr(maxBoxX-20.0)+" "+ToStr(maxBoxY-Options.bleeds.Top-markOffs)+" li\n");
			PutStream("st\n");
			PutStream(ToStr(maxBoxX-bleedRight-markOffs)+" "+ToStr(maxBoxY)+" m\n");
			PutStream(ToStr(maxBoxX-bleedRight-markOffs)+" "+ToStr(maxBoxY-20.0)+" li\n");
			PutStream("st\n");
		}
		if (Options.bleedMarks)
		{
			PutStream("gs\n");
			PutStream("[3 1 1 1] 0 setdash\n");
		// Bottom Left
			PutStream("0 "+ToStr(markOffs)+" m\n");
			PutStream(ToStr(20.0)+" "+ToStr(markOffs)+" li\n");
			PutStream("st\n");
			PutStream(ToStr(markOffs)+" 0 m\n");
			PutStream(ToStr(markOffs)+" 20 l\n");
			PutStream("st\n");
		// Top Left
			PutStream("0 "+ToStr(maxBoxY-markOffs)+" m\n");
			PutStream(ToStr(20.0)+" "+ToStr(maxBoxY-markOffs)+" li\n");
			PutStream("st\n");
			PutStream(ToStr(markOffs)+" "+ToStr(maxBoxY)+" m\n");
			PutStream(ToStr(markOffs)+" "+ToStr(maxBoxY-20.0)+" li\n");
			PutStream("st\n");
		// Bottom Right
			PutStream(ToStr(maxBoxX)+" "+ToStr(markOffs)+" m\n");
			PutStream(ToStr(maxBoxX-20.0)+" "+ToStr(markOffs)+" li\n");
			PutStream("st\n");
			PutStream(ToStr(maxBoxX-markOffs)+" "+ToStr(0.0)+" m\n");
			PutStream(ToStr(maxBoxX-markOffs)+" "+ToStr(20.0)+" li\n");
			PutStream("st\n");
		// Top Right
			PutStream(ToStr(maxBoxX)+" "+ToStr(maxBoxY-markOffs)+" m\n");
			PutStream(ToStr(maxBoxX-20.0)+" "+ToStr(maxBoxY-markOffs)+" li\n");
			PutStream("st\n");
			PutStream(ToStr(maxBoxX-markOffs)+" "+ToStr(maxBoxY)+" m\n");
			PutStream(ToStr(maxBoxX-markOffs)+" "+ToStr(maxBoxY-20.0)+" li\n");
			PutStream("st\n");
			PutStream("gr\n");
		}
		if (Options.registrationMarks)
		{
			QString regCross = "0 7 m\n14 7 li\n7 0 m\n7 14 li\n13 7 m\n13 10.31383 10.31383 13 7 13 cu\n3.68629 13 1 10.31383 1 7 cu\n1 3.68629 3.68629 1 7 1 cu\n";
			regCross += "10.31383 1 13 3.68629 13 7 cu\ncl\n10.5 7 m\n10.5 8.93307 8.93307 10.5 7 10.5 cu\n5.067 10.5 3.5 8.93307 3.5 7 cu\n";
			regCross += "3.5 5.067 5.067 3.5 7 3.5 cu\n8.93307 3.5 10.5 5.067 10.5 7 cu\ncl\nst\n";
			PutStream("gs\n");
			PutStream(ToStr(maxBoxX / 2.0 - 7.0)+" 3 tr\n");
			PutStream(regCross);
			PutStream("gr\n");
			PutStream("gs\n");
			PutStream("3 "+ToStr(maxBoxY / 2.0 - 7.0)+" tr\n");
			PutStream(regCross);
			PutStream("gr\n");
			PutStream("gs\n");
			PutStream(ToStr(maxBoxX / 2.0 - 7.0)+" "+ToStr(maxBoxY - 17.0)+" tr\n");
			PutStream(regCross);
			PutStream("gr\n");
			PutStream("gs\n");
			PutStream(ToStr(maxBoxX - 17.0)+" "+ToStr(maxBoxY / 2.0 - 7.0)+" tr\n");
			PutStream(regCross);
			PutStream("gr\n");
		}
		if (Options.colorMarks)
		{
			double startX = markOffs+bleedLeft+6.0;
			double startY = maxBoxY - 18.0;
			PutStream("0 0 0 1 cmyk\n");
			double col = 1.0;
			for (int bl = 0; bl < 11; bl++)
			{
				PutStream("0 0 0 "+ToStr(col)+" cmyk\n");
				PutStream(ToStr(startX+bl*14.0)+" "+ToStr(startY)+" 14 14 rectfill\n");
				PutStream("0 0 0 1 cmyk\n");
				PutStream(ToStr(startX+bl*14.0)+" "+ToStr(startY)+" 14 14 rectstroke\n");
				col -= 0.1;
			}
			startX = maxBoxX-bleedRight-markOffs-20.0;
			PutStream("0 0 0 0.5 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 0 0.5 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 0.5 0 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0.5 0 0 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("1 1 0 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("1 0 1 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 1 1 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 0 1 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 1 0 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("1 0 0 0 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX)+" "+ToStr(startY)+" 14 14 rectstroke\n");
		}
		PutStream("gr\n");
	}
	PutStream("sp\n");
	PutStream("end\n");
}

void PSLib::PS_curve(double x1, double y1, double x2, double y2, double x3, double y3)
{
	PutStream(ToStr(x1) + " " + ToStr(y1) + " " + ToStr(x2) + " " + ToStr(y2) + " " + ToStr(x3) + " " + ToStr(y3) + " cu\n");
}

void PSLib::PS_moveto(double x, double y)
{
	PutStream(ToStr(x) + " " + ToStr(y) + " m\n");
}

void PSLib::PS_lineto(double x, double y)
{
	PutStream(ToStr(x) + " " + ToStr(y) + " li\n");
}

void PSLib::PS_closepath()
{
	PutStream("cl\n");
}

void PSLib::PS_translate(double x, double y)
{
	PutStream(ToStr(x) + " " + ToStr(y) + " tr\n");
}

void PSLib::PS_scale(double x, double y)
{
	PutStream(ToStr(x) + " " + ToStr(y) + " sc\n");
}

void PSLib::PS_rotate(double x)
{
	PutStream(ToStr(x) + " ro\n");
}

void PSLib::PS_clip(bool mu)
{
	PutStream( mu ? "eoclip newpath\n" : "clip newpath\n" );
}

void PSLib::PS_save()
{
	PutStream("gs\n");
}

void PSLib::PS_restore()
{
	PutStream("gr\n");
}

void PSLib::PS_setcmykcolor_fill(double c, double m, double y, double k)
{
	FillColor = ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k);
}

void PSLib::PS_setcmykcolor_dummy()
{
	PutStream("0 0 0 0 cmyk\n");
}

void PSLib::PS_setcmykcolor_stroke(double c, double m, double y, double k)
{
	StrokeColor = ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k);
}

void PSLib::PS_setlinewidth(double w)
{
	PutStream(ToStr(w) + " sw\n");
	LineW = w;
}

void PSLib::PS_setdash(Qt::PenStyle st, double offset, QList<double> dash)
{
	if (dash.count() != 0)
	{
		PutStream("[ ");
		QList<double>::iterator it;
		for ( it = dash.begin(); it != dash.end(); ++it )
		{
			PutStream(ToStr(*it)+" ");
		}
		PutStream("] "+ToStr(offset)+" setdash\n");
	}
	else
		PutStream("["+getDashString(st, LineW)+"] 0 setdash\n");
}

void PSLib::PS_setcapjoin(Qt::PenCapStyle ca, Qt::PenJoinStyle jo)
{
	switch (ca)
		{
		case Qt::FlatCap:
			PutStream("0 setlinecap\n");
			break;
		case Qt::SquareCap:
			PutStream("2 setlinecap\n");
			break;
		case Qt::RoundCap:
			PutStream("1 setlinecap\n");
			break;
		default:
			PutStream("0 setlinecap\n");
			break;
		}
	switch (jo)
		{
		case Qt::MiterJoin:
			PutStream("0 setlinejoin\n");
			break;
		case Qt::BevelJoin:
			PutStream("2 setlinejoin\n");
			break;
		case Qt::RoundJoin:
			PutStream("1 setlinejoin\n");
			break;
		default:
			PutStream("0 setlinejoin\n");
			break;
		}
}

void PSLib::PS_selectfont(QString f, double s)
{
	PutStream(UsedFonts[f] + " " + ToStr(s) + " se\n");
}

void PSLib::PS_fill()
{
	if (fillRule)
		PutStream(FillColor + " cmyk eofill\n");
	else
		PutStream(FillColor + " cmyk fill\n");
}

void PSLib::PS_fillspot(QString color, double shade)
{
	if (fillRule)
		PutStream(ToStr(shade / 100.0)+" "+spotMap[color]+" eofill\n");
	else
		PutStream(ToStr(shade / 100.0)+" "+spotMap[color]+" fill\n");
}

void PSLib::PS_strokespot(QString color, double shade)
{
	PutStream(ToStr(shade / 100.0)+" "+spotMap[color]+" st\n");
}

void PSLib::PS_stroke()
{
	PutStream(StrokeColor + " cmyk st\n");
}

void PSLib::PS_fill_stroke()
{
	PS_save();
	PS_fill();
	PS_restore();
	PS_stroke();
}

void PSLib::PS_newpath()
{
	PutStream("newpath\n");
}

void PSLib::PS_MultiRadGradient(double w, double h, double x, double y, QList<double> Stops, QStringList Colors, QStringList colorNames, QList<int> colorShades)
{
	bool first = true;
	bool oneSpot1 = false;
	bool oneSpot2 = false;
	bool twoSpot = false;
	int cc = 0;
	int mc = 0;
	int yc = 0;
	int kc = 0;
	CMYKColor cmykValues;
	PutStream( "clipsave\n" );
	PutStream("eoclip\n");
	for (int c = 0; c < Colors.count()-1; ++c)
	{
		oneSpot1 = false;
		oneSpot2 = false;
		twoSpot = false;
		QString spot1 = colorNames[c];
		QString spot2 = colorNames[c+1];
		PutStream("<<\n");
		PutStream("/ShadingType 3\n");
		if (DoSep)
			PutStream("/ColorSpace /DeviceGray\n");
		else
		{
			if (spotMap.contains(colorNames[c]))
				oneSpot1 = true;
			else if  (spotMap.contains(colorNames[c+1]))
				oneSpot2 = true;
			if ((spotMap.contains(colorNames[c])) && (spotMap.contains(colorNames[c+1])))
			{
				oneSpot1 = false;
				oneSpot2 = false;
				twoSpot = true;
			}
			if ((!oneSpot1) && (!oneSpot2) && (!twoSpot) || (!useSpotColors) || (GraySc))
			{
				if (GraySc)
					PutStream("/ColorSpace /DeviceGray\n");
				else
					PutStream("/ColorSpace /DeviceCMYK\n");
			}
			else
			{
				PutStream("/ColorSpace [ /DeviceN [");
				if (oneSpot1)
				{
					PutStream(" /Cyan /Magenta /Yellow /Black ("+spot1+") ]\n");
					ScColorEngine::getCMYKValues(m_Doc->PageColors[colorNames[c]], m_Doc, cmykValues);
					cmykValues.getValues(cc, mc, yc, kc);
				}
				else if (oneSpot2)
				{
					PutStream(" /Cyan /Magenta /Yellow /Black ("+spot2+") ]\n");
					ScColorEngine::getCMYKValues(m_Doc->PageColors[colorNames[c+1]], m_Doc, cmykValues);
					cmykValues.getValues(cc, mc, yc, kc);
				}
				else if (twoSpot)
				{
					PutStream(" ("+spot1+") ("+spot2+") ]\n");
				}
				PutStream("/DeviceCMYK\n");
				PutStream("{\n");
				if (twoSpot)
				{
					ScColorEngine::getCMYKValues(m_Doc->PageColors[colorNames[c]], m_Doc, cmykValues);
					cmykValues.getValues(cc, mc, yc, kc);
					PutStream("exch\n");
					PutStream("dup "+ToStr(static_cast<double>(cc) / 255.0)+" mul exch\n");
					PutStream("dup "+ToStr(static_cast<double>(mc) / 255.0)+" mul exch\n");
					PutStream("dup "+ToStr(static_cast<double>(yc) / 255.0)+" mul exch\n");
					PutStream("dup "+ToStr(static_cast<double>(kc) / 255.0)+" mul exch pop 5 -1 roll\n");
					ScColorEngine::getCMYKValues(m_Doc->PageColors[colorNames[c+1]], m_Doc, cmykValues);
					cmykValues.getValues(cc, mc, yc, kc);
					PutStream("dup "+ToStr(static_cast<double>(cc) / 255.0)+" mul 6 -1 roll add dup 1.0 gt {pop 1.0} if 5 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(mc) / 255.0)+" mul 5 -1 roll add dup 1.0 gt {pop 1.0} if 4 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(yc) / 255.0)+" mul 4 -1 roll add dup 1.0 gt {pop 1.0} if 3 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(kc) / 255.0)+" mul 3 -1 roll add dup 1.0 gt {pop 1.0} if 2 1 roll pop\n");
				}
				else
				{
					PutStream("dup "+ToStr(static_cast<double>(cc) / 255.0)+" mul 6 -1 roll add dup 1.0 gt {pop 1.0} if 5 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(mc) / 255.0)+" mul 5 -1 roll add dup 1.0 gt {pop 1.0} if 4 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(yc) / 255.0)+" mul 4 -1 roll add dup 1.0 gt {pop 1.0} if 3 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(kc) / 255.0)+" mul 3 -1 roll add dup 1.0 gt {pop 1.0} if 2 1 roll pop\n");
				}
				PutStream("} ]\n");
			}
		}
		PutStream("/BBox [0 "+ToStr(h)+" "+ToStr(w)+" 0]\n");
		if (Colors.count() == 2)
			PutStream("/Extend [true true]\n");
		else
		{
			if (first)
				PutStream("/Extend [false true]\n");
			else
			{
				if (c == Colors.count()-2)
					PutStream("/Extend [true false]\n");
				else
					PutStream("/Extend [false false]\n");
			}
		}
		PutStream("/Coords ["+ToStr(x)+" "+ToStr(y)+" "+ToStr(Stops.at(c+1))+" "+ToStr(x)+" "+ToStr(y)+" "+ToStr(Stops.at(c))+"]\n");
		PutStream("/Function\n");
		PutStream("<<\n");
		PutStream("/FunctionType 2\n");
		PutStream("/Domain [0 1]\n");
		if (DoSep)
		{
			int pla = Plate - 1 < 0 ? 3 : Plate - 1;
			QStringList cols1 = Colors[c+1].split(" ", QString::SkipEmptyParts);
			QStringList cols2 = Colors[c].split(" ", QString::SkipEmptyParts);
			PutStream("/C1 ["+ToStr(1-cols1[pla].toDouble())+"]\n");
			PutStream("/C0 ["+ToStr(1-cols2[pla].toDouble())+"]\n");
		}
		else
		{
			if (useSpotColors)
			{
				if (oneSpot1)
				{
					PutStream("/C1 [0 0 0 0 "+ToStr(colorShades[c] / 100.0)+"]\n");
					PutStream("/C0 ["+Colors[c+1]+" 0 ]\n");
				}
				else if (oneSpot2)
				{
					PutStream("/C1 ["+Colors[c]+" 0 ]\n");
					PutStream("/C0 [0 0 0 0 "+ToStr(colorShades[c+1] / 100.0)+"]\n");
				}
				else if (twoSpot)
				{
					PutStream("/C1 ["+ToStr(colorShades[c] / 100.0)+" 0]\n");
					PutStream("/C0 [0 "+ToStr(colorShades[c+1] / 100.0)+"]\n");
				}
				else
				{
					PutStream("/C1 ["+Colors[c]+"]\n");
					PutStream("/C0 ["+Colors[c+1]+"]\n");
				}
			}
			else
			{
				PutStream("/C1 ["+Colors[c]+"]\n");
				PutStream("/C0 ["+Colors[c+1]+"]\n");
			}
		}
		PutStream("/N 1\n");
		PutStream(">>\n");
		PutStream(">>\n");
		PutStream("shfill\n");
		first = false;
	}
	PutStream("cliprestore\n");
}

void PSLib::PS_MultiLinGradient(double w, double h, QList<double> Stops, QStringList Colors, QStringList colorNames, QList<int> colorShades)
{
	bool first = true;
	bool oneSpot1 = false;
	bool oneSpot2 = false;
	bool twoSpot = false;
	int cc = 0;
	int mc = 0;
	int yc = 0;
	int kc = 0;
	CMYKColor cmykValues;
	PutStream( "clipsave\n" );
	PutStream("eoclip\n");
	for (int c = 0; c < Colors.count()-1; ++c)
	{
		oneSpot1 = false;
		oneSpot2 = false;
		twoSpot = false;
		QRegExp badchars("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]");
		QString spot1 = colorNames[c];
		QString spot2 = colorNames[c+1];
		PutStream("<<\n");
		PutStream("/ShadingType 2\n");
		if (DoSep)
			PutStream("/ColorSpace /DeviceGray\n");
		else
		{
			if (spotMap.contains(colorNames[c]))
				oneSpot1 = true;
			else if  (spotMap.contains(colorNames[c+1]))
				oneSpot2 = true;
			if ((spotMap.contains(colorNames[c])) && (spotMap.contains(colorNames[c+1])))
			{
				oneSpot1 = false;
				oneSpot2 = false;
				twoSpot = true;
			}
			if ((!oneSpot1) && (!oneSpot2) && (!twoSpot) || (!useSpotColors) || (GraySc))
			{
				if (GraySc)
					PutStream("/ColorSpace /DeviceGray\n");
				else
					PutStream("/ColorSpace /DeviceCMYK\n");
			}
			else
			{
				PutStream("/ColorSpace [ /DeviceN [");
				if (oneSpot1)
				{
					PutStream(" /Cyan /Magenta /Yellow /Black ("+spot1+") ]\n");
					ScColorEngine::getCMYKValues(m_Doc->PageColors[colorNames[c]], m_Doc, cmykValues);
					cmykValues.getValues(cc, mc, yc, kc);
				}
				else if (oneSpot2)
				{
					PutStream(" /Cyan /Magenta /Yellow /Black ("+spot2+") ]\n");
					ScColorEngine::getCMYKValues(m_Doc->PageColors[colorNames[c+1]], m_Doc, cmykValues);
					cmykValues.getValues(cc, mc, yc, kc);
				}
				else if (twoSpot)
				{
					PutStream(" ("+spot1+") ("+spot2+") ]\n");
				}
				PutStream("/DeviceCMYK\n");
				PutStream("{\n");
				if (twoSpot)
				{
					ScColorEngine::getCMYKValues(m_Doc->PageColors[colorNames[c]], m_Doc, cmykValues);
					cmykValues.getValues(cc, mc, yc, kc);
					PutStream("exch\n");
					PutStream("dup "+ToStr(static_cast<double>(cc) / 255.0)+" mul exch\n");
					PutStream("dup "+ToStr(static_cast<double>(mc) / 255.0)+" mul exch\n");
					PutStream("dup "+ToStr(static_cast<double>(yc) / 255.0)+" mul exch\n");
					PutStream("dup "+ToStr(static_cast<double>(kc) / 255.0)+" mul exch pop 5 -1 roll\n");
					ScColorEngine::getCMYKValues(m_Doc->PageColors[colorNames[c+1]], m_Doc, cmykValues);
					cmykValues.getValues(cc, mc, yc, kc);
					PutStream("dup "+ToStr(static_cast<double>(cc) / 255.0)+" mul 6 -1 roll add dup 1.0 gt {pop 1.0} if 5 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(mc) / 255.0)+" mul 5 -1 roll add dup 1.0 gt {pop 1.0} if 4 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(yc) / 255.0)+" mul 4 -1 roll add dup 1.0 gt {pop 1.0} if 3 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(kc) / 255.0)+" mul 3 -1 roll add dup 1.0 gt {pop 1.0} if 2 1 roll pop\n");
				}
				else
				{
					PutStream("dup "+ToStr(static_cast<double>(cc) / 255.0)+" mul 6 -1 roll add dup 1.0 gt {pop 1.0} if 5 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(mc) / 255.0)+" mul 5 -1 roll add dup 1.0 gt {pop 1.0} if 4 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(yc) / 255.0)+" mul 4 -1 roll add dup 1.0 gt {pop 1.0} if 3 1 roll\n");
					PutStream("dup "+ToStr(static_cast<double>(kc) / 255.0)+" mul 3 -1 roll add dup 1.0 gt {pop 1.0} if 2 1 roll pop\n");
				}
				PutStream("} ]\n");
			}
		}
		PutStream("/BBox [0 "+ToStr(h)+" "+ToStr(w)+" 0]\n");
		if (Colors.count() == 2)
			PutStream("/Extend [true true]\n");
		else
		{
			if (first)
				PutStream("/Extend [true false]\n");
			else
			{
				if (c == Colors.count()-2)
					PutStream("/Extend [false true]\n");
				else
					PutStream("/Extend [false false]\n");
			}
		}
		first = false;
		PutStream("/Coords ["+ToStr(Stops.at(c*2))+"  "+ToStr(Stops.at(c*2+1))+" "+ToStr(Stops.at(c*2+2))+" "+ToStr(Stops.at(c*2+3))+"]\n");
		PutStream("/Function\n");
		PutStream("<<\n");
		PutStream("/FunctionType 2\n");
		PutStream("/Domain [0 1]\n");
		if (DoSep)
		{
			int pla = Plate - 1 < 0 ? 3 : Plate - 1;
			QStringList cols1 = Colors[c].split(" ", QString::SkipEmptyParts);
			QStringList cols2 = Colors[c+1].split(" ", QString::SkipEmptyParts);
			PutStream("/C1 ["+ToStr(1-cols1[pla].toDouble())+"]\n");
			PutStream("/C0 ["+ToStr(1-cols2[pla].toDouble())+"]\n");
		}
		else
		{
			if (useSpotColors)
			{
				if (oneSpot1)
				{
					PutStream("/C0 [0 0 0 0 "+ToStr(colorShades[c] / 100.0)+"]\n");
					PutStream("/C1 ["+Colors[c+1]+" 0 ]\n");
				}
				else if (oneSpot2)
				{
					PutStream("/C0 ["+Colors[c]+" 0 ]\n");
					PutStream("/C1 [0 0 0 0 "+ToStr(colorShades[c+1] / 100.0)+"]\n");
				}
				else if (twoSpot)
				{
					PutStream("/C0 ["+ToStr(colorShades[c] / 100.0)+" 0]\n");
					PutStream("/C1 [0 "+ToStr(colorShades[c+1] / 100.0)+"]\n");
				}
				else
				{
					PutStream("/C0 ["+Colors[c]+"]\n");
					PutStream("/C1 ["+Colors[c+1]+"]\n");
				}
			}
			else
			{
				PutStream("/C0 ["+Colors[c]+"]\n");
				PutStream("/C1 ["+Colors[c+1]+"]\n");
			}
		}
		PutStream("/N 1\n");
		PutStream(">>\n");
		PutStream(">>\n");
		PutStream("shfill\n");
	}
	PutStream("cliprestore\n");
}

void PSLib::PS_show_xyG(QString font, uint glyph, double x, double y, bool spot)
{
	QString Name;
	Name = GlyphsOfFont[font].contains(glyph) ? GlyphsOfFont[font][glyph].second : QString(".notdef");
	if (spot)
		PutStream("/"+Name+" "+ToStr(x)+" "+ToStr(y)+" shgsp\n");
	else
		PutStream("/"+Name+" "+ToStr(x)+" "+ToStr(y)+" "+StrokeColor+" shg\n");
}

void PSLib::PS_show(double x, double y)
{
	PS_moveto(x, y);
	PutStream("/hyphen glyphshow\n");
}

void PSLib::PS_showSub(uint chr, QString font, double size, bool stroke)
{
	PutStream(" (G"+IToStr(chr)+") "+font+" "+ToStr(size / 10.0)+" ");
	PutStream(stroke ? "shgs\n" : "shgf\n");
}

bool PSLib::PS_ImageData(PageItem *c, QString fn, QString Name, QString Prof, bool UseEmbedded, bool UseProf)
{
	bool dummy;
	QByteArray tmp;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(fn);
	if (extensionIndicatesEPS(ext) && (c->pixm.imgInfo.type != ImageType7))
	{
		if (loadRawText(fn, tmp))
		{
			PutStream("currentfile 1 (%ENDEPSDATA) /SubFileDecode filter /ReusableStreamDecode filter\n");
			PutStream("%%BeginDocument: " + fi.fileName() + "\n");
			if (getDouble(QString(tmp.mid(0, 4)), true) == 0xC5D0D3C6)
			{
				char* data = tmp.data();
				uint startPos = getDouble(QString(tmp.mid(4, 4)), false);
				uint length = getDouble(QString(tmp.mid(8, 4)), false);
				PutStream(data+startPos, length, false);
			}
			else
				PutStream(tmp, false);
			PutStream("\n%ENDEPSDATA\n");
			PutStream("%%EndDocument\n");
			PutStream("/"+PSEncode(Name)+"Bild exch def\n");
			return true;
		}
		return false;
	}
	ScImage image;
	QByteArray imgArray;
	image.imgInfo.valid = false;
	image.imgInfo.clipPath = "";
	image.imgInfo.PDSpathData.clear();
	image.imgInfo.layerInfo.clear();
	image.imgInfo.RequestProps = c->pixm.imgInfo.RequestProps;
	image.imgInfo.isRequest = c->pixm.imgInfo.isRequest;
	CMSettings cms(c->doc(), Prof, c->IRender);
	if (!image.LoadPicture(fn, cms, UseEmbedded, UseProf, ScImage::CMYKData, 300, &dummy))
	{
		PS_Error_ImageLoadFailure(fn);
		return false;
	}
	image.applyEffect(c->effectsInUse, colorsToUse, true);
	QByteArray maskArray;
	bool alphaLoaded = image.getAlpha(fn, maskArray, false, true, 300);
	if (!alphaLoaded)
	{
		PS_Error_MaskLoadFailure(fn);
		return false;
	}
	if ((maskArray.size() > 0) && (c->pixm.imgInfo.type != ImageType7))
	{
		PutStream("currentfile /ASCII85Decode filter /FlateDecode filter /ReusableStreamDecode filter\n");
		if (!PutImageToStream(image, maskArray, -1))
		{
			PS_Error_ImageDataWriteFailure();
			return false;
		}
		PutStream("/"+PSEncode(Name)+"Bild exch def\n");
	}
	else
	{
		PutStream("currentfile /ASCII85Decode filter /FlateDecode filter /ReusableStreamDecode filter\n");
		if (!PutImageToStream(image, -1))
		{
			PS_Error_ImageDataWriteFailure();
			return false;
		}
		PutStream("/"+PSEncode(Name)+"Bild exch def\n");
		imgArray.resize(0);
	}
	return true;
}

bool PSLib::PS_image(PageItem *c, double x, double y, QString fn, double scalex, double scaley, QString Prof, bool UseEmbedded, bool UseProf, QString Name)
{
	bool dummy;
	QByteArray tmp;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(fn);
	if (extensionIndicatesEPS(ext) && (c->pixm.imgInfo.type != ImageType7))
	{
		if (loadRawText(fn, tmp))
		{
			PutStream("bEPS\n");
			PutStream(ToStr(PrefsManager::instance()->appPrefs.gs_Resolution / 72.0 * scalex) + " " + ToStr(PrefsManager::instance()->appPrefs.gs_Resolution / 72.0 * scaley) + " sc\n");
			PutStream(ToStr(-c->BBoxX+x * scalex) + " " + ToStr(y * scalex) + " tr\n");
			if (!Name.isEmpty())
			{
				PutStream(PSEncode(Name)+"Bild cvx exec\n");
				PutStream(PSEncode(Name)+"Bild resetfile\n");
			}
			else
			{
      				PutStream("%%BeginDocument: " + fi.fileName() + "\n");
					if (getDouble(QString(tmp.mid(0, 4)), true) == 0xC5D0D3C6)
					{
						char* data = tmp.data();
						uint startPos = getDouble(tmp.mid(4, 4), false);
						uint length = getDouble(tmp.mid(8, 4), false);
						PutStream(data+startPos, length, false);
					}
					else
						PutStream(tmp);
					PutStream("\n%%EndDocument\n");
			}
			PutStream("eEPS\n");
			return true;
		}
		return false;
	}
	else
	{
		ScImage image;
		QByteArray imgArray;
		image.imgInfo.valid = false;
		image.imgInfo.clipPath = "";
		image.imgInfo.PDSpathData.clear();
		image.imgInfo.layerInfo.clear();
		image.imgInfo.RequestProps = c->pixm.imgInfo.RequestProps;
		image.imgInfo.isRequest = c->pixm.imgInfo.isRequest;
		CMSettings cms(c->doc(), Prof, c->IRender);
		int resolution = (c->pixm.imgInfo.type == ImageType7) ? 72 : 300;
		if ( !image.LoadPicture(fn, cms, UseEmbedded, UseProf, ScImage::CMYKData, resolution, &dummy) )
		{
			PS_Error_ImageLoadFailure(fn);
			return false;
		}
		image.applyEffect(c->effectsInUse, colorsToUse, true);
		int w = image.width();
		int h = image.height();
		if (extensionIndicatesPDF(ext))
		{
			scalex *= PrefsManager::instance()->appPrefs.gs_Resolution / 300.0;
			scaley *= PrefsManager::instance()->appPrefs.gs_Resolution / 300.0;
		}
 		PutStream(ToStr(x*scalex) + " " + ToStr(y*scaley) + " tr\n");
 		PutStream(ToStr(qRound(scalex*w)) + " " + ToStr(qRound(scaley*h)) + " sc\n");
 		PutStream(((!DoSep) && (!GraySc)) ? "/DeviceCMYK setcolorspace\n" : "/DeviceGray setcolorspace\n");
		QByteArray maskArray;
		ScImage img2;
		img2.imgInfo.clipPath = "";
		img2.imgInfo.PDSpathData.clear();
		img2.imgInfo.layerInfo.clear();
		img2.imgInfo.RequestProps = c->pixm.imgInfo.RequestProps;
		img2.imgInfo.isRequest = c->pixm.imgInfo.isRequest;
		if (c->pixm.imgInfo.type != ImageType7)
		{
			bool alphaLoaded = img2.getAlpha(fn, maskArray, false, true, 300);
			if (!alphaLoaded)
			{
				PS_Error_MaskLoadFailure(fn);
				return false;
			}
		}
 		if ((maskArray.size() > 0) && (c->pixm.imgInfo.type != ImageType7))
 		{
			int plate = DoSep ? Plate : (GraySc ? -2 : -1);
			// JG - Experimental code using Type3 image instead of patterns
			PutStream("<< /ImageType 3\n");
			PutStream("   /DataDict <<\n");
			PutStream("      /ImageType 1\n");
			PutStream("      /Width  " + IToStr(w) + "\n");
			PutStream("      /Height " + IToStr(h) + "\n");
			PutStream("      /BitsPerComponent 8\n");
			PutStream( (GraySc || DoSep) ? "      /Decode [1 0]\n" : "      /Decode [0 1 0 1 0 1 0 1]\n");
			PutStream("      /ImageMatrix [" + IToStr(w) + " 0 0 " + IToStr(-h) + " 0 " + IToStr(h) + "]\n");
			if (Name.length() > 0)
				PutStream("      /DataSource "+PSEncode(Name)+"Bild\n");
			else
			    PutStream("      /DataSource currentfile /ASCII85Decode filter /FlateDecode filter\n");
			PutStream("      >>\n");
			PutStream("   /MaskDict <<\n");
			PutStream("      /ImageType 1\n");
			PutStream("      /Width  " + IToStr(w) + "\n");
			PutStream("      /Height " + IToStr(h) + "\n");
			PutStream("      /BitsPerComponent 8\n");
			PutStream("      /Decode [1 0]\n");
			PutStream("      /ImageMatrix [" + IToStr(w) + " 0 0 " + IToStr(-h) + " 0 " + IToStr(h) + "]\n");
			PutStream("      >>\n");
			PutStream("   /InterleaveType 1\n");
			PutStream(">>\n");
			PutStream("image\n");
			if (Name.isEmpty())
			{
				if (!PutImageToStream(image, maskArray, plate))
				{
					PS_Error_ImageDataWriteFailure();
					return false;
				}
			}
			else
			{
				PutStream(PSEncode(Name)+"Bild resetfile\n");
				//PutStream(PSEncode(Name)+"Mask resetfile\n");
			}
		}
		else
		{
			PutStream("<< /ImageType 1\n");
			PutStream("   /Width " + IToStr(w) + "\n");
			PutStream("   /Height " + IToStr(h) + "\n");
			PutStream("   /BitsPerComponent 8\n");
			if (DoSep)
				PutStream("   /Decode [1 0]\n");
			else
				PutStream( GraySc ? "   /Decode [1 0]\n" : "   /Decode [0 1 0 1 0 1 0 1]\n");
			PutStream("   /ImageMatrix [" + IToStr(w) + " 0 0 " + IToStr(-h) + " 0 " + IToStr(h) + "]\n");
			if (!Name.isEmpty())
			{
				PutStream("   /DataSource "+PSEncode(Name)+"Bild >>\n");
				PutStream("image\n");
				PutStream(PSEncode(Name)+"Bild resetfile\n");
			}
			else
			{
				int plate = DoSep ? Plate : (GraySc ? -2 : -1);
				PutStream("   /DataSource currentfile /ASCII85Decode filter /FlateDecode filter >>\n");
				PutStream("image\n");
				if (!PutImageToStream(image, plate))
				{
					PS_Error_ImageDataWriteFailure();
					return false;
				}
			}
		}
	}
	return true;
}


void PSLib::PS_plate(int nr, QString name)
{
	switch (nr)
	{
		case 0:
			PutStream("%%PlateColor Black\n");
			PutStream("/setcmykcolor {exch pop exch pop exch pop 1 exch sub oldsetgray} bind def\n");
			PutStream("/setrgbcolor {pop pop pop 1 oldsetgray} bind def\n");
			break;
		case 1:
			PutStream("%%PlateColor Cyan\n");
			PutStream("/setcmykcolor {pop pop pop 1 exch sub oldsetgray} bind def\n");
			PutStream("/setrgbcolor {pop pop oldsetgray} bind def\n");
			break;
		case 2:
			PutStream("%%PlateColor Magenta\n");
			PutStream("/setcmykcolor {pop pop exch pop 1 exch sub oldsetgray} bind def\n");
			PutStream("/setrgbcolor {pop exch pop oldsetgray} bind def\n");
			break;
		case 3:
			PutStream("%%PlateColor Yellow\n");
			PutStream("/setcmykcolor {pop exch pop exch pop 1 exch sub oldsetgray} bind def\n");
			PutStream("/setrgbcolor {exch pop exch pop oldsetgray} bind def\n");
			break;
		default:
			PutStream("%%PlateColor "+name+"\n");
			PutStream("/setcmykcolor {exch 0.11 mul add exch 0.59 mul add exch 0.3 mul add dup 1 gt {pop 1} if 1 exch sub oldsetgray} bind def\n");
			PutStream("/setrgbcolor {0.11 mul exch 0.59 mul add exch 0.3 mul add oldsetgray} bind def\n");
			break;
	}
	Plate = nr;
	currentSpot = name;
	DoSep = true;
}

void PSLib::PS_setGray()
{
	GraySc = true;
}

void PSLib::PDF_Bookmark(QString text, uint Seite)
{
	PutStream("[/Title ("+text+") /Page "+IToStr(Seite)+" /View [/Fit]\n");
	PutStream("/OUT pdfmark\n");
	isPDF = true;
}

void PSLib::PDF_Annotation(QString text, double x, double y, double b, double h)
{
	PutStream("[ /Rect [ "+ToStr(static_cast<int>(x))+" "+ToStr(static_cast<int>(y))
			+" "+ToStr(static_cast<int>(b))+" "+ToStr(static_cast<int>(h))+" ]\n");
	PutStream("  /Contents ("+text+")\n  /Open false\n");
	PutStream("/ANN pdfmark\n");
	isPDF = true;
}


void PSLib::PS_close()
{
	PutStream("%%Trailer\n");
//	PutStream("end\n");
	PutStream("%%EOF\n");
	Spool.close();
}

void PSLib::PS_insert(QString i)
{
	PutStream(i);
}

void PSLib::PS_Error(const QString& message)
{
	ErrorMessage = message;
	if (!ScCore->usingGUI())
		qDebug(message.toLocal8Bit().data());
}

void PSLib::PS_Error_ImageDataWriteFailure(void)
{
	PS_Error( tr("Failed to write data for an image"));
}

void PSLib::PS_Error_ImageLoadFailure(const QString& fileName)
{
	PS_Error( tr("Failed to load an image : %1").arg(fileName) );
}

void PSLib::PS_Error_MaskLoadFailure(const QString& fileName)
{
	PS_Error( tr("Failed to load an image mask : %1").arg(fileName) );
}

void PSLib::PS_Error_InsufficientMemory(void)
{
	PS_Error( tr("Insufficient memory for processing an image"));
}

int PSLib::CreatePS(ScribusDoc* Doc, PrintOptions &options)
{
	bool errorOccured = false;
	Options = options;
	std::vector<int> &pageNs = options.pageNumbers;
	bool sep = options.outputSeparations;
	QString SepNam = options.separationName;
	QStringList spots = options.allSeparations;
	bool farb = options.useColor;
	bool Hm = options.mirrorH;
	bool Vm = options.mirrorV;
	bool Ic = options.useICC;
	bool gcr = options.doGCR;
	bool doDev = options.setDevParam;
	bool doClip = options.doClip;
	bool over = options.doOverprint;
	QStack<PageItem*> groupStack;
	int sepac;
	int pagemult;
	doOverprint = over;
	if ((sep) && (SepNam == QObject::tr("All")))
		pagemult = spots.count();
	else
		pagemult = 1;
	QList<double> dum;
	double gx = 0.0;
	double gy = 0.0;
	double gw = 0.0;
	double gh = 0.0;;
	dum.clear();
	PS_set_Info("Author", Doc->documentInfo.getAuthor());
	PS_set_Info("Title", Doc->documentInfo.getTitle());
	if (!farb)
		PS_setGray();
	applyICC = Ic;
	if ((Doc->HasCMS) && (ScCore->haveCMS()) && (applyICC))
		solidTransform = cmsCreateTransform(Doc->DocInputCMYKProf, TYPE_CMYK_16, Doc->DocPrinterProf, TYPE_CMYK_16, Doc->IntentColors, 0);
	else
		applyICC = false;
	if (usingGUI)
	{
		QString title=QObject::tr("Exporting PostScript File");
		if (Art)
			title=QObject::tr("Printing File");
		progressDialog=new MultiProgressDialog(title, CommonStrings::tr_Cancel, Doc->scMW());
		if (progressDialog==0)
			usingGUI=false;
		else
		{
			QStringList barNames, barTexts;
			barNames << "EMP" << "EP";
			barTexts << tr("Processing Master Page:") << tr("Exporting Page:");
			QList<bool> barsNumeric;
			barsNumeric << true << true;
			progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
			progressDialog->setOverallTotalSteps(pageNs.size()+Doc->MasterPages.count());
			progressDialog->setTotalSteps("EMP", Doc->MasterPages.count());
			progressDialog->setTotalSteps("EP", pageNs.size());
			progressDialog->setOverallProgress(0);
			progressDialog->setProgress("EMP", 0);
			progressDialog->setProgress("EP", 0);
			progressDialog->show();
			connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
			ScQApp->processEvents();
		}
	}
	//if ((!Art) && (view->SelItem.count() != 0))
	uint docSelectionCount=Doc->m_Selection->count();
	if ((!Art) && (docSelectionCount != 0))
	{
		double minx = 99999.9;
		double miny = 99999.9;
		double maxx = -99999.9;
		double maxy = -99999.9;
		for (uint ep = 0; ep < docSelectionCount; ++ep)
		{
			//PageItem* currItem = view->SelItem.at(ep);
			PageItem* currItem = Doc->m_Selection->itemAt(ep);
			double lw = currItem->lineWidth() / 2.0;
			if (currItem->rotation() != 0)
			{
				FPointArray pb;
				pb.resize(0);
				pb.addPoint(FPoint(currItem->xPos()-lw, currItem->yPos()-lw));
				pb.addPoint(FPoint(currItem->width()+lw*2.0, -lw, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
				pb.addPoint(FPoint(currItem->width()+lw*2.0, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
				pb.addPoint(FPoint(-lw, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
				for (uint pc = 0; pc < 4; ++pc)
				{
					minx = qMin(minx, pb.point(pc).x());
					miny = qMin(miny, pb.point(pc).y());
					maxx = qMax(maxx, pb.point(pc).x());
					maxy = qMax(maxy, pb.point(pc).y());
				}
			}
			else
			{
				minx = qMin(minx, currItem->xPos()-lw);
				miny = qMin(miny, currItem->yPos()-lw);
				maxx = qMax(maxx, currItem->xPos()-lw + currItem->width()+lw*2.0);
				maxy = qMax(maxy, currItem->yPos()-lw + currItem->height()+lw*2.0);
			}
		}
		gx = minx;
		gy = miny;
		gw = maxx - minx;
		gh = maxy - miny;
		int pgNum = pageNs[0]-1;
		gx -= Doc->Pages->at(pgNum)->xOffset();
		gy -= Doc->Pages->at(pgNum)->yOffset();
		errorOccured = !PS_begin_doc(Doc, gx, Doc->pageHeight - (gy+gh), gx + gw, Doc->pageHeight - gy, 1*pagemult, false, sep, farb, Ic, gcr, over);
	}
	else
	{
		uint a;
		double maxWidth = 0.0;
		double maxHeight = 0.0;
		for (uint aa = 0; aa < pageNs.size(); ++aa)
		{
			a = pageNs[aa]-1;
			maxWidth = qMax(Doc->Pages->at(a)->width(), maxWidth);
			maxHeight = qMax(Doc->Pages->at(a)->height(), maxHeight);
		}
		errorOccured = !PS_begin_doc(Doc, 0.0, 0.0, maxWidth, maxHeight, pageNs.size()*pagemult, doDev, sep, farb, Ic, gcr, over);
	}
	int ap=0;
	for (; ap < Doc->MasterPages.count() && !abortExport && !errorOccured; ++ap)
	{
		progressDialog->setOverallProgress(ap);
		progressDialog->setProgress("EMP", ap);
		ScQApp->processEvents();
		if (Doc->MasterItems.count() != 0)
		{
			int Lnr = 0;
			ScLayer ll;
			ll.isPrintable = false;
			ll.LNr = 0;
			for (int lam = 0; lam < Doc->Layers.count() && !abortExport && !errorOccured; ++lam)
			{
				Doc->Layers.levelToLayer(ll, Lnr);
				if (ll.isPrintable)
				{
					for (int api = 0; api < Doc->MasterItems.count() && !abortExport; ++api)
					{
						QString tmps;
						PageItem *it = Doc->MasterItems.at(api);
						if (usingGUI)
							ScQApp->processEvents();
						if ((it->LayerNr != ll.LNr) || (!it->printEnabled()))
							continue;
/*						int x = static_cast<int>(Doc->MasterPages.at(ap)->xOffset());
						int y = static_cast<int>(Doc->MasterPages.at(ap)->yOffset());
						int w = static_cast<int>(Doc->MasterPages.at(ap)->width());
						int h = static_cast<int>(Doc->MasterPages.at(ap)->height());
						double ilw=it->lineWidth();
						int x2 = static_cast<int>(it->BoundingX - ilw / 2.0);
						int y2 = static_cast<int>(it->BoundingY - ilw / 2.0);
						int w2 = static_cast<int>(it->BoundingW + ilw);
						int h2 = static_cast<int>(it->BoundingH + ilw);
						if (!QRect(x, y, w, h).intersects(QRect(x2, y2, w2, h2)))
							continue; */
						double x = Doc->MasterPages.at(ap)->xOffset();
						double y = Doc->MasterPages.at(ap)->yOffset();
						double w = Doc->MasterPages.at(ap)->width();
						double h1 = Doc->MasterPages.at(ap)->height();
						double ilw = it->lineWidth();
						double x2 = it->BoundingX - ilw / 2.0;
						double y2 = it->BoundingY - ilw / 2.0;
						double w2 = it->BoundingW + ilw;
						double h2 = it->BoundingH + ilw;
						if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h1, y2+h2 )))
							continue;
						if ((it->OwnPage != static_cast<int>(Doc->MasterPages.at(ap)->pageNr())) && (it->OwnPage != -1))
							continue;
						if ((optimization == OptimizeSize) && it->asImageFrame() && it->PicAvail && (!it->Pfile.isEmpty()) && it->printEnabled() && (!sep) && farb)
						{
							errorOccured = !PS_ImageData(it, it->Pfile, it->itemName(), it->IProfile, it->UseEmbedded, Ic);
							if (errorOccured) break;
						}
						PS_TemplateStart(Doc->MasterPages.at(ap)->pageName() + tmps.setNum(it->ItemNr));
						ProcessItem(Doc, Doc->MasterPages.at(ap), it, ap+1, sep, farb, Ic, gcr, true);
						PS_TemplateEnd();
					}
				}
				Lnr++;
				if (errorOccured) break;
			}
		}
		if (errorOccured) break;
	}
	sepac = 0;
	uint aa = 0;
	uint a;	
	PutStream("%%EndSetup\n");
	while (aa < pageNs.size() && !abortExport && !errorOccured)
	{
		progressDialog->setProgress("EP", aa);
		progressDialog->setOverallProgress(ap+aa);
		if (usingGUI)
			ScQApp->processEvents();
		a = pageNs[aa]-1;
		//if ((!Art) && (view->SelItem.count() != 0))
		if ((!Art) && (Doc->m_Selection->count() != 0))
		{
			MarginStruct Ma;
			Ma.Left = gx;
			Ma.Top = gy;
			Ma.Bottom = Doc->Pages->at(a)->height() - (gy + gh);
			Ma.Right = Doc->Pages->at(a)->width() - (gx + gw);
			PS_begin_page(Doc->Pages->at(a), &Ma, true);
		}
		else
			PS_begin_page(Doc->Pages->at(a), &Doc->Pages->at(a)->Margins, doClip);
		if (Hm)
		{
			PS_translate(Doc->Pages->at(a)->width(), 0);
			PS_scale(-1, 1);
		}
		if (Vm)
		{
			PS_translate(0, Doc->Pages->at(a)->height());
			PS_scale(1, -1);
		}
		if (sep)
		{
			if (SepNam == QObject::tr("Black"))
				PS_plate(0);
			else if (SepNam == QObject::tr("Cyan"))
				PS_plate(1);
			else if (SepNam == QObject::tr("Magenta"))
				PS_plate(2);
			else if (SepNam == QObject::tr("Yellow"))
				PS_plate(3);
			else if (SepNam == QObject::tr("All"))
				PS_plate(sepac, spots[sepac]);
			else
				PS_plate(4, SepNam);
		}
		if (!Doc->Pages->at(a)->MPageNam.isEmpty() && !abortExport && !errorOccured)
		{
			int h, s, v, k;
			QByteArray chstrc;
			QString chstr;
			int Lnr = 0;
			ScLayer ll;
			ll.isPrintable = false;
			ll.LNr = 0;
			Page* mPage = Doc->MasterPages.at(Doc->MasterNames[Doc->Pages->at(a)->MPageNam]);
			if (Doc->MasterItems.count() != 0)
			{
				for (int lam = 0; lam < Doc->Layers.count() && !abortExport && !errorOccured; ++lam)
				{
					Doc->Layers.levelToLayer(ll, Lnr);
					if (ll.isPrintable)
					{
						for (int am = 0; am < Doc->Pages->at(a)->FromMaster.count() && !abortExport; ++am)
						{
							QString tmps;
							PageItem *ite = Doc->Pages->at(a)->FromMaster.at(am);
							if (usingGUI)
								ScQApp->processEvents();
							if ((ite->LayerNr != ll.LNr) || (!ite->printEnabled()))
								continue;
							if (ite->isGroupControl)
							{
								PS_save();
								FPointArray cl = ite->PoLine.copy();
								QMatrix mm;
								mm.translate(ite->xPos() - mPage->xOffset(), (ite->yPos() - mPage->yOffset()) - Doc->Pages->at(a)->height());
								mm.rotate(ite->rotation());
								cl.map( mm );
								SetClipPath(&cl);
								PS_closepath();
								PS_clip(true);
								groupStack.push(ite->groupsLastItem);
								continue;
							}
							if (!(ite->asTextFrame()) && !(ite->asImageFrame()))
								PS_UseTemplate(Doc->Pages->at(a)->MPageNam + tmps.setNum(ite->ItemNr));
							else if (ite->asImageFrame())
							{
								PS_save();
								// JG : replace what seems mostly duplicate code by corresponding function call (#3936)
								errorOccured = !ProcessItem(Doc, mPage, ite, a, sep, farb, Ic, gcr, false, false, true);
								if (errorOccured) break;
								PS_restore();
							}
							else if (ite->asTextFrame())
							{
								PS_save();
								if (!doOverprint)
								{
									if (ite->doOverprint)
									{
										PutStream("true setoverprint\n");
										PutStream("true setoverprintmode\n");
									}
								}
								if (ite->fillColor() != CommonStrings::None)
								{
									SetColor(ite->fillColor(), ite->fillShade(), &h, &s, &v, &k, gcr);
									PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								PS_translate(ite->xPos() - mPage->xOffset(), mPage->height() - (ite->yPos() - mPage->yOffset()));
								if (ite->rotation() != 0)
									PS_rotate(-ite->rotation());
								if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
								{
									SetClipPath(&ite->PoLine);
									PS_closepath();
									if (ite->GrType != 0)
										HandleGradient(ite, ite->width(), ite->height(), gcr);
									else
										putColor(ite->fillColor(), ite->fillShade(), true);
								}
								if (ite->imageFlippedH())
								{
									PS_translate(ite->width(), 0);
									PS_scale(-1, 1);
								}
								if (ite->imageFlippedV())
								{
									PS_translate(0, -ite->height());
									PS_scale(1, -1);
								}
								setTextSt(Doc, ite, gcr, a, mPage, sep, farb, Ic, true);
								if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty())) && (!ite->isTableItem))
								{
									if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
									{
										SetColor(ite->lineColor(), ite->lineShade(), &h, &s, &v, &k, gcr);
										PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										PS_setlinewidth(ite->lineWidth());
										PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
										PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
										SetClipPath(&ite->PoLine);
										PS_closepath();
										putColor(ite->lineColor(), ite->lineShade(), false);
									}
									else
									{
										multiLine ml = Doc->MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
										{
											if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
											{
												SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
												PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												PS_setlinewidth(ml[it].Width);
												PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
												PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
												SetClipPath(&ite->PoLine);
												PS_closepath();
												putColor(ml[it].Color, ml[it].Shade, false);
											}
										}
									}
								}
								PS_restore();
							}
							if (groupStack.count() != 0)
							{
								while (ite == groupStack.top())
								{
									PS_restore();
									groupStack.pop();
									if (groupStack.count() == 0)
										break;
								}
							}
						}
					}
					for (int am = 0; am < Doc->Pages->at(a)->FromMaster.count() && !abortExport; ++am)
					{
						PageItem *ite = Doc->Pages->at(a)->FromMaster.at(am);
						if (!ite->isTableItem)
							continue;
						if ((ite->lineColor() != CommonStrings::None) || (ite->lineWidth() == 0.0))
							continue;
						if (ite->printEnabled())
						{
							PS_save();
							if (!doOverprint)
							{
								if (ite->doOverprint)
								{
									PutStream("true setoverprint\n");
									PutStream("true setoverprintmode\n");
								}
							}
							if (ite->lineColor() != CommonStrings::None)
							{
								SetColor(ite->lineColor(), ite->lineShade(), &h, &s, &v, &k, gcr);
								PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							}
							PS_setlinewidth(ite->lineWidth());
							PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
							PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
							PS_translate(ite->xPos() - mPage->xOffset(), mPage->height() - (ite->yPos() - mPage->yOffset()));
							if (ite->rotation() != 0)
								PS_rotate(-ite->rotation());
							if ((ite->TopLine) || (ite->RightLine) || (ite->BottomLine) || (ite->LeftLine))
							{
								if (ite->TopLine)
								{
									PS_moveto(0, 0);
									PS_lineto(ite->width(), 0);
								}
								if (ite->RightLine)
								{
									PS_moveto(ite->width(), 0);
									PS_lineto(ite->width(), -ite->height());
								}
								if (ite->BottomLine)
								{
									PS_moveto(0, -ite->height());
									PS_lineto(ite->width(), -ite->height());
								}
								if (ite->LeftLine)
								{
									PS_moveto(0, 0);
									PS_lineto(0, -ite->height());
								}
								putColor(ite->lineColor(), ite->lineShade(), false);
							}
							PS_restore();
						}
					}
					Lnr++;
				}
				if (abortExport || errorOccured) break;
			}
		}
		if (!abortExport && !errorOccured)
			ProcessPage(Doc, Doc->Pages->at(a), a+1, sep, farb, Ic, gcr);
		if (!abortExport && !errorOccured)
			PS_end_page();
		if (sep)
		{
			if (SepNam != QObject::tr("All"))
				aa++;
			else
			{
				if (sepac == static_cast<int>(spots.count()-1))
				{
					aa++;
					sepac = 0;
				}
				else
					sepac++;
			}
		}
		else
			aa++;
	}
	PS_close();
	if (usingGUI) progressDialog->close();
	if ((Doc->HasCMS) && (ScCore->haveCMS()) && (applyICC))
		cmsDeleteTransform(solidTransform);
	if (errorOccured)
		return 1;
	else if (abortExport)
		return 2; //CB Lets leave 1 for general error condition
	return 0; 
}

bool PSLib::ProcessItem(ScribusDoc* Doc, Page* a, PageItem* c, uint PNr, bool sep, bool farb, bool ic, bool gcr, bool master, bool embedded, bool useTemplate)
{
	double tsz;
	int h, s, v, k;
	int d;
	ScText *hl;
	QList<double> dum;
	dum.clear();
	QString tmps, chstr;
	if (c->printEnabled())
	{
		fillRule = true;
		PS_save();
		if (!doOverprint)
		{
			if (c->doOverprint)
			{
				PutStream("true setoverprint\n");
				PutStream("true setoverprintmode\n");
			}
		}
		if (c->fillColor() != CommonStrings::None)
		{
			SetColor(c->fillColor(), c->fillShade(), &h, &s, &v, &k, gcr);
			PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
		}
		if (c->lineColor() != CommonStrings::None)
		{
			SetColor(c->lineColor(), c->lineShade(), &h, &s, &v, &k, gcr);
			PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
		}
		PS_setlinewidth(c->lineWidth());
		PS_setcapjoin(c->PLineEnd, c->PLineJoin);
		PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
		if (!embedded)
		{
			PS_translate(c->xPos() - a->xOffset(), a->height() - (c->yPos() - a->yOffset()));
		}
		if (c->rotation() != 0)
			PS_rotate(-c->rotation());
		switch (c->itemType())
		{
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
			if (master)
				break;
			if ((c->fillColor() != CommonStrings::None) || (c->GrType != 0))
			{
				SetClipPath(&c->PoLine);
				PS_closepath();
				if ((c->GrType != 0) && (master == false))
					HandleGradient(c, c->width(), c->height(), gcr);
				else
					putColor(c->fillColor(), c->fillShade(), true);
				PS_newpath();
			}
			PS_save();
			if (c->imageClip.size() != 0)
			{
				SetClipPath(&c->imageClip);
				PS_closepath();
				PS_clip(true);
			}
			SetClipPath(&c->PoLine);
			PS_closepath();
			PS_clip(true);
			if (c->imageFlippedH())
			{
				PS_translate(c->width(), 0);
				PS_scale(-1, 1);
			}
			if (c->imageFlippedV())
			{
				PS_translate(0, -c->height());
				PS_scale(1, -1);
			}
			if ((c->PicAvail) && (!c->Pfile.isEmpty()))
			{
				bool imageOk = false;
				PS_translate(0, -c->BBoxH*c->imageYScale());
				if ((optimization == OptimizeSize) && (((!a->pageName().isEmpty()) && !sep && farb) || useTemplate))
					imageOk = PS_image(c, c->imageXOffset(), -c->imageYOffset(), c->Pfile, c->imageXScale(), c->imageYScale(), c->IProfile, c->UseEmbedded, ic, c->itemName());
				else
					imageOk = PS_image(c, c->imageXOffset(), -c->imageYOffset(), c->Pfile, c->imageXScale(), c->imageYScale(), c->IProfile, c->UseEmbedded, ic);
				if (!imageOk) return false;
			}
			PS_restore();
			if (((c->lineColor() != CommonStrings::None) || (!c->NamedLStyle.isEmpty())) && (!c->isTableItem))
			{
				if ((c->NamedLStyle.isEmpty()) && (c->lineWidth() != 0.0))
				{
					SetColor(c->lineColor(), c->lineShade(), &h, &s, &v, &k, gcr);
					PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
					PS_setlinewidth(c->lineWidth());
					PS_setcapjoin(c->PLineEnd, c->PLineJoin);
					PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
					SetClipPath(&c->PoLine);
					PS_closepath();
					putColor(c->lineColor(), c->lineShade(), false);
				}
				else
				{
					multiLine ml = Doc->MLineStyles[c->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
						{
							SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
							PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							PS_setlinewidth(ml[it].Width);
							PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
							PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
							SetClipPath(&c->PoLine);
							PS_closepath();
							putColor(ml[it].Color, ml[it].Shade, false);
						}
					}
				}
			}
			break;
		case PageItem::TextFrame:
			if (master)
				break;
			if (c->isBookmark)
			{
				QString bm = "";
				QString cc;
				for (int d = 0; d < c->itemText.length(); ++d)
				{
					if ((c->itemText.text(d) == QChar(13)) || (c->itemText.text(d) == QChar(10)) || (c->itemText.text(d) == QChar(28)))
						break;
					bm += "\\"+cc.setNum(qMax(c->itemText.text(d).unicode(), (ushort) 32), 8);
				}
				PDF_Bookmark(bm, a->pageNr()+1);
			}
			if (c->isAnnotation())
			{
				QString bm = "";
				QString cc;
				for (int d = 0; d < c->itemText.length(); ++d)
				{
					bm += "\\"+cc.setNum(qMax(c->itemText.text(d).unicode(), (ushort) 32), 8);
				}
				PDF_Annotation(bm, 0, 0, c->width(), -c->height());
				break;
			}
			if ((c->fillColor() != CommonStrings::None) || (c->GrType != 0))
			{
				SetClipPath(&c->PoLine);
				PS_closepath();
				if ((c->GrType != 0) && (master == false))
					HandleGradient(c, c->width(), c->height(), gcr);
				else
					putColor(c->fillColor(), c->fillShade(), true);
			}
			if (c->imageFlippedH())
			{
				PS_translate(c->width(), 0);
				PS_scale(-1, 1);
			}
			if (c->imageFlippedV())
			{
				PS_translate(0, -c->height());
				PS_scale(1, -1);
			}
			setTextSt(Doc, c, gcr, PNr-1, a, sep, farb, ic, master);
			if (((c->lineColor() != CommonStrings::None) || (!c->NamedLStyle.isEmpty())) && (!c->isTableItem))
			{
				SetColor(c->lineColor(), c->lineShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				PS_setlinewidth(c->lineWidth());
				PS_setcapjoin(c->PLineEnd, c->PLineJoin);
				PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
				if ((c->NamedLStyle.isEmpty()) && (c->lineWidth() != 0.0))
				{
					SetClipPath(&c->PoLine);
					PS_closepath();
					putColor(c->lineColor(), c->lineShade(), false);
				}
				else
				{
					multiLine ml = Doc->MLineStyles[c->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
						{
							SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
							PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							PS_setlinewidth(ml[it].Width);
							PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
							PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
							SetClipPath(&c->PoLine);
							PS_closepath();
							putColor(ml[it].Color, ml[it].Shade, false);
						}
					}
				}
			}
			break;
		case PageItem::Line:
			if ((c->NamedLStyle.isEmpty()) && (c->lineWidth() != 0.0))
			{
				PS_moveto(0, 0);
				PS_lineto(c->width(), 0);
				putColor(c->lineColor(), c->lineShade(), false);
			}
			else
			{
				multiLine ml = Doc->MLineStyles[c->NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
					{
						SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
						PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
						PS_setlinewidth(ml[it].Width);
						PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
						PS_moveto(0, 0);
						PS_lineto(c->width(), 0);
						putColor(ml[it].Color, ml[it].Shade, false);
					}
				}
			}
			if (c->startArrowIndex() != 0)
			{
				QMatrix arrowTrans;
				FPointArray arrow = Doc->arrowStyles.at(c->startArrowIndex()-1).points.copy();
				arrowTrans.translate(0, 0);
				arrowTrans.scale(c->lineWidth(), c->lineWidth());
				arrowTrans.scale(-1,1);
				arrow.map(arrowTrans);
				SetColor(c->lineColor(), c->lineShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				PS_newpath();
				SetClipPath(&arrow);
				PS_closepath();
				putColor(c->lineColor(), c->lineShade(), true);
			}
			if (c->endArrowIndex() != 0)
			{
				QMatrix arrowTrans;
				FPointArray arrow = Doc->arrowStyles.at(c->endArrowIndex()-1).points.copy();
				arrowTrans.translate(c->width(), 0);
				arrowTrans.scale(c->lineWidth(), c->lineWidth());
				arrow.map(arrowTrans);
				SetColor(c->lineColor(), c->lineShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				PS_newpath();
				SetClipPath(&arrow);
				PS_closepath();
				putColor(c->lineColor(), c->lineShade(), true);
			}
			break;
		/* OBSOLETE CR 2005-02-06
		case 1:
		case 3:
		*/
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
			if ((c->fillColor() != CommonStrings::None) || (c->GrType != 0))
			{
				SetClipPath(&c->PoLine);
				PS_closepath();
				fillRule = c->fillRule;
				if (c->GrType != 0)
					HandleGradient(c, c->width(), c->height(), gcr);
				else
					putColor(c->fillColor(), c->fillShade(), true);
			}
			if ((c->lineColor() != CommonStrings::None) || (!c->NamedLStyle.isEmpty()))
			{
				if ((c->NamedLStyle.isEmpty()) && (c->lineWidth() != 0.0))
				{
					SetClipPath(&c->PoLine);
					PS_closepath();
					putColor(c->lineColor(), c->lineShade(), false);
				}
				else
				{
					multiLine ml = Doc->MLineStyles[c->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
						{
							SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
							PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							PS_setlinewidth(ml[it].Width);
							PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
							PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
							SetClipPath(&c->PoLine);
							PS_closepath();
							putColor(ml[it].Color, ml[it].Shade, false);
						}
					}
				}
			}
			break;
		case PageItem::PolyLine:
			if ((c->fillColor() != CommonStrings::None) || (c->GrType != 0))
			{
				SetClipPath(&c->PoLine);
				PS_closepath();
				if (c->GrType != 0)
					HandleGradient(c, c->width(), c->height(), gcr);
				else
					putColor(c->fillColor(), c->fillShade(), true);
				PS_newpath();
			}
			if ((c->lineColor() != CommonStrings::None) || (!c->NamedLStyle.isEmpty()))
			{
				if ((c->NamedLStyle.isEmpty()) && (c->lineWidth() != 0.0))
				{
					SetClipPath(&c->PoLine, false);
					putColor(c->lineColor(), c->lineShade(), false);
				}
				else
				{
					multiLine ml = Doc->MLineStyles[c->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
						{
							SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
							PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							PS_setlinewidth(ml[it].Width);
							PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
							PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
							SetClipPath(&c->PoLine, false);
							putColor(ml[it].Color, ml[it].Shade, false);
						}
					}
				}
			}
			if (c->startArrowIndex() != 0)
			{
				FPoint Start = c->PoLine.point(0);
				for (uint xx = 1; xx < c->PoLine.size(); xx += 2)
				{
					FPoint Vector = c->PoLine.point(xx);
					if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
					{
						double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
						QMatrix arrowTrans;
						FPointArray arrow = Doc->arrowStyles.at(c->startArrowIndex()-1).points.copy();
						arrowTrans.translate(Start.x(), Start.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(c->lineWidth(), c->lineWidth());
						arrow.map(arrowTrans);
						SetColor(c->lineColor(), c->lineShade(), &h, &s, &v, &k, gcr);
						PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
						PS_newpath();
						SetClipPath(&arrow);
						PS_closepath();
						putColor(c->lineColor(), c->lineShade(), true);
						break;
					}
				}
			}
			if (c->endArrowIndex() != 0)
			{
				FPoint End = c->PoLine.point(c->PoLine.size()-2);
				for (uint xx = c->PoLine.size()-1; xx > 0; xx -= 2)
				{
					FPoint Vector = c->PoLine.point(xx);
					if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
					{
						double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
						QMatrix arrowTrans;
						FPointArray arrow = Doc->arrowStyles.at(c->endArrowIndex()-1).points.copy();
						arrowTrans.translate(End.x(), End.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(c->lineWidth(), c->lineWidth());
						arrow.map(arrowTrans);
						SetColor(c->lineColor(), c->lineShade(), &h, &s, &v, &k, gcr);
						PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
						PS_newpath();
						SetClipPath(&arrow);
						PS_closepath();
						putColor(c->lineColor(), c->lineShade(), true);
						break;
					}
				}
			}
			break;
		case PageItem::PathText:
			if (c->PoShow)
			{
				if (c->PoLine.size() > 3)
				{
					PS_save();
					if ((c->NamedLStyle.isEmpty()) && (c->lineWidth() != 0.0))
					{
						SetClipPath(&c->PoLine, false);
						putColor(c->lineColor(), c->lineShade(), false);
					}
					else
					{
						multiLine ml = Doc->MLineStyles[c->NamedLStyle];
						for (int it = ml.size()-1; it > -1; it--)
						{
							if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
							{
								SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
								PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								PS_setlinewidth(ml[it].Width);
								PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
								PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
								SetClipPath(&c->PoLine, false);
								putColor(ml[it].Color, ml[it].Shade, false);
							}
						}
					}
					PS_restore();
				}
			}
#ifndef NLS_PROTO
			for (d = c->firstInFrame(); d <= c->lastInFrame(); ++d)
			{
				hl = c->itemText.item(d);
				const CharStyle & style(c->itemText.charStyle(d));
				if ((hl->ch == QChar(13)) || (hl->ch == QChar(30)) || (hl->ch == QChar(32)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
					continue;
				tsz = style.fontSize();
				chstr = hl->ch;
				if (hl->ch == QChar(29))
					chstr = " ";
				if (hl->ch == QChar(0xA0))
					chstr = " ";
				if (style.effects() & 32)
				{
					if (chstr.toUpper() != chstr)
						chstr = chstr.toUpper();
				}
				if (style.effects() & 64)
				{
					if (chstr.toUpper() != chstr)
					{
						tsz = style.fontSize() * Doc->typographicSettings.valueSmallCaps / 100;
						chstr = chstr.toUpper();
					}
				}
				if (style.effects() & 1)
					tsz = style.fontSize() * Doc->typographicSettings.scalingSuperScript / 100;
				if (style.effects() & 2)
					tsz = style.fontSize() * Doc->typographicSettings.scalingSuperScript / 100;
				if (style.fillColor() != CommonStrings::None)
				{
					SetColor(style.fillColor(), style.fillShade(), &h, &s, &v, &k, gcr);
					PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				}
				if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
				{
					PS_save();
					PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(-hl->PRot) + " " + ToStr(0) + "]\n");
					if (c->textPathFlipped)
					{
						PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(0) + " " + ToStr(0) + "]\n");
						PutStream("["+ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + "] concatmatrix\n");
					}
					if (c->textPathType == 0)
						PutStream("["+ToStr(hl->PtransX) + " " + ToStr(-hl->PtransY) + " " + ToStr(-hl->PtransY) + " " + ToStr(-hl->PtransX) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
					else if (c->textPathType == 1)
						PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
					else if (c->textPathType == 2)
					{
						double a = 1;
						double b = -1;
						if (hl->PtransX < 0)
						{
							a = -1;
							b = 1;
						}
						if (fabs(hl->PtransX) > 0.1)
							PutStream("["+ToStr(a) + " " + ToStr((hl->PtransY / hl->PtransX) * b) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
						else
							PutStream("["+ToStr(a) + " " + ToStr(4) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
					}
					PutStream("["+ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + "] concatmatrix\nconcat\n");
//					PS_translate(0, (tsz / 10.0));
					if (c->BaseOffs != 0)
						PS_translate(0, -c->BaseOffs);
					if (style.scaleH() != 1000)
						PS_scale(style.scaleH() / 1000.0, 1);
					QList<PageItem*> emG = hl->embedded.getGroupedItems();
					QStack<PageItem*> groupStack;
					for (int em = 0; em < emG.count(); ++em)
					{
						PageItem* embedded = emG.at(em);
						if (embedded->isGroupControl)
						{
							PS_save();
							FPointArray cl = embedded->PoLine.copy();
							QMatrix mm;
							mm.translate(embedded->gXpos * (style.scaleH() / 1000.0), ((embedded->gHeight * (style.scaleV() / 1000.0)) - embedded->gYpos * (style.scaleV() / 1000.0)) * -1);
							if (style.baselineOffset() != 0)
								mm.translate(0, embedded->gHeight * (style.baselineOffset() / 1000.0));
							if (style.scaleH() != 1000)
								mm.scale(style.scaleH() / 1000.0, 1);
							if (style.scaleV() != 1000)
								mm.scale(1, style.scaleV() / 1000.0);
							mm.rotate(embedded->rotation());
							cl.map( mm );
							SetClipPath(&cl);
							PS_closepath();
							PS_clip(true);
							groupStack.push(embedded->groupsLastItem);
							continue;
						}
						PS_save();
						PS_translate(embedded->gXpos * (style.scaleH() / 1000.0), ((embedded->gHeight * (style.scaleV() / 1000.0)) - embedded->gYpos * (style.scaleV() / 1000.0)));
						if (style.baselineOffset() != 0)
							PS_translate(0, embedded->gHeight * (style.baselineOffset() / 1000.0));
						if (style.scaleH() != 1000)
							PS_scale(style.scaleH() / 1000.0, 1);
						if (style.scaleV() != 1000)
							PS_scale(1, style.scaleV() / 1000.0);
						ProcessItem(Doc, a, embedded, PNr, sep, farb, ic, gcr, master, true);
						PS_restore();
						if (groupStack.count() != 0)
						{
							while (embedded == groupStack.top())
							{
								PS_restore();
								groupStack.pop();
								if (groupStack.count() == 0)
									break;
							}
						}
					}
					PS_restore();
					continue;
				}
				/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
				ScFace::FontType type = style.font().type();
				if ((type == ScFace::TTF) ||  (style.font().isOTF()) || (style.font().subset()))
				{
//					uint chr = chstr[0].unicode();
					uint chr = style.font().char2CMap(chstr[0]);
					if ((style.font().canRender(chstr[0])) && (chr != 32))
					{
						PS_save();
						if (style.fillColor() != CommonStrings::None)
						{
							SetColor(style.fillColor(), style.fillShade(), &h, &s, &v, &k, gcr);
							PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(-hl->PRot) + " " + ToStr(0) + "]\n");
							if (c->textPathFlipped)
							{
								PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(0) + " " + ToStr(0) + "]\n");
								PutStream("["+ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + "] concatmatrix\n");
							}
							if (c->textPathType == 0)
								PutStream("["+ToStr(hl->PtransX) + " " + ToStr(-hl->PtransY) + " " + ToStr(-hl->PtransY) + " " + ToStr(-hl->PtransX) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
							else if (c->textPathType == 1)
								PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
							else if (c->textPathType == 2)
							{
								double a = 1;
								double b = -1;
								if (hl->PtransX < 0)
								{
									a = -1;
									b = 1;
								}
								if (fabs(hl->PtransX) > 0.1)
									PutStream("["+ToStr(a) + " " + ToStr((hl->PtransY / hl->PtransX) * b) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
								else
									PutStream("["+ToStr(a) + " " + ToStr(4) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
							}
							PutStream("["+ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + "] concatmatrix\nconcat\n");
							PS_translate(0, (tsz / 10.0));
							if (c->BaseOffs != 0)
								PS_translate(0, -c->BaseOffs);
							if (style.scaleH() != 1000)
								PS_scale(style.scaleH() / 1000.0, 1);
							if ((colorsToUse[style.fillColor()].isSpotColor()) && (!DoSep))
								PutStream(ToStr(style.fillShade() / 100.0)+" "+spotMap[style.fillColor()]);
							else
								PutStream(FillColor + " cmyk");
							PS_showSub(chr, style.font().psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ), tsz / 10.0, false);
							if ((style.effects() & ScStyle_Outline))
							{
								if ((style.strokeColor() != CommonStrings::None) && ((tsz * style.outlineWidth() / 10000.0) != 0))
								{
									PS_save();
									PS_setlinewidth(tsz * style.outlineWidth() / 10000.0);
									SetColor(style.strokeColor(), style.strokeShade(), &h, &s, &v, &k, gcr);
									PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									if ((colorsToUse[style.strokeColor()].isSpotColor()) && (!DoSep))
										PutStream(ToStr(style.strokeShade() / 100.0)+" "+spotMap[style.strokeColor()]);
									else
										PutStream(StrokeColor + " cmyk");
									PS_showSub(chr, style.font().psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ), tsz / 10.0, true);
									PS_restore();
								}
							}
						}
						PS_restore();
					}
				}
				else
				{
					uint glyph = hl->glyph.glyph;
					PS_selectfont(style.font().replacementName(), tsz / 10.0);
					PS_save();
					PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(-hl->PRot) + " " + ToStr(0) + "]\n");
					if (c->textPathFlipped)
					{
						PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(0) + " " + ToStr(0) + "]\n");
						PutStream("["+ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + "] concatmatrix\n");
					}
					if (c->textPathType == 0)
						PutStream("["+ToStr(hl->PtransX) + " " + ToStr(-hl->PtransY) + " " + ToStr(-hl->PtransY) + " " + ToStr(-hl->PtransX) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
					else if (c->textPathType == 1)
						PutStream("["+ToStr(1) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
					else if (c->textPathType == 2)
					{
						double a = 1;
						double b = -1;
						if (hl->PtransX < 0)
						{
							a = -1;
							b = 1;
						}
						if (fabs(hl->PtransX) > 0.1)
							PutStream("["+ToStr(a) + " " + ToStr((hl->PtransY / hl->PtransX) * b) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
						else
							PutStream("["+ToStr(a) + " " + ToStr(4) + " " + ToStr(0) + " " + ToStr(-1) + " " + ToStr(hl->glyph.xoffset) + " " + ToStr(-hl->glyph.yoffset) + "]\n");
					}
					PutStream("["+ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + " " + ToStr(0) + "] concatmatrix\nconcat\n");
					if (c->BaseOffs != 0)
						PS_translate(0, -c->BaseOffs);
					if ((colorsToUse[style.fillColor()].isSpotColor()) && (!DoSep))
					{
						PutStream(ToStr(style.fillShade() / 100.0)+" "+spotMap[style.fillColor()]);
						PS_show_xyG(style.font().replacementName(), glyph, 0, 0, true);
					}
					else
						PS_show_xyG(style.font().replacementName(), glyph, 0, 0, false);
					if ((style.effects() & ScStyle_Outline))
					{
						if ((style.strokeColor() != CommonStrings::None) && ((tsz * style.outlineWidth() / 10000.0) != 0))
						{
							uint gl = style.font().char2CMap(chstr[0]);
							FPointArray gly = style.font().glyphOutline(gl);
							QMatrix chma;
							chma.scale(tsz / 100.0, tsz / 100.0);
							gly.map(chma);
							PS_save();
							PS_setlinewidth(tsz * style.outlineWidth() / 10000.0);
							PS_translate(0,  tsz / 10.0);
							SetColor(style.strokeColor(), style.strokeShade(), &h, &s, &v, &k, gcr);
							PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							SetClipPath(&gly);
							PS_closepath();
							putColor(style.strokeColor(), style.strokeShade(), false);
							PS_restore();
						}
					}
					PS_restore();
				}
			}
#endif
			break;
		default:
			break;
		}
		PS_restore();
	}
	return true;
}

void PSLib::ProcessPage(ScribusDoc* Doc, Page* a, uint PNr, bool sep, bool farb, bool ic, bool gcr)
{
	int b;
	int h, s, v, k;
	QByteArray chstrc;
	QString chstr, chglyph, tmp;
	PageItem *c;
	QList<PageItem*> PItems;
	QStack<PageItem*> groupStack;
	int Lnr = 0;
	ScLayer ll;
	ll.isPrintable = false;
	ll.LNr = 0;
	PItems = (a->pageName().isEmpty()) ? Doc->DocItems : Doc->MasterItems;
	for (int la = 0; la < Doc->Layers.count(); ++la)
	{
		Doc->Layers.levelToLayer(ll, Lnr);
		if (ll.isPrintable && !abortExport)
		{
			for (b = 0; b < PItems.count() && !abortExport; ++b)
			{
				c = PItems.at(b);
				if (usingGUI)
					ScQApp->processEvents();
				if (c->LayerNr != ll.LNr)
					continue;
				if ((!a->pageName().isEmpty()) && (c->asTextFrame()))
					continue;
				if ((!a->pageName().isEmpty()) && (c->asImageFrame()) && ((sep) || (!farb)))
					continue;
				//if ((!Art) && (view->SelItem.count() != 0) && (!c->Select))
				if ((!Art) && (!c->isSelected()) && (Doc->m_Selection->count() != 0))
					continue;
				double x = a->xOffset();
				double y = a->yOffset();
				double w = a->width();
				double h1 = a->height();
				double ilw = c->lineWidth();
				double x2 = c->BoundingX - ilw / 2.0;
				double y2 = c->BoundingY - ilw / 2.0;
				double w2 = c->BoundingW + ilw;
				double h2 = c->BoundingH + ilw;
				if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h1, y2+h2 )))
					continue;
				if (c->ChangedMasterItem)
					continue;
				if ((!a->pageName().isEmpty()) && (c->OwnPage != static_cast<int>(a->pageNr())) && (c->OwnPage != -1))
					continue;
				if (c->isGroupControl)
				{
					PS_save();
					FPointArray cl = c->PoLine.copy();
					QMatrix mm;
					mm.translate(c->xPos() - a->xOffset(), (c->yPos() - a->yOffset()) - a->height());
					mm.rotate(c->rotation());
					cl.map( mm );
					SetClipPath(&cl);
					PS_closepath();
					PS_clip(true);
					groupStack.push(c->groupsLastItem);
					continue;
				}
				ProcessItem(Doc, a, c, PNr, sep, farb, ic, gcr, false);
				if (groupStack.count() != 0)
				{
					while (c == groupStack.top())
					{
						PS_restore();
						groupStack.pop();
						if (groupStack.count() == 0)
							break;
					}
				}
			}
		}
		for (b = 0; b < PItems.count() && !abortExport; ++b)
		{
			c = PItems.at(b);
			if (usingGUI)
				ScQApp->processEvents();
			if (c->LayerNr != ll.LNr)
				continue;
			if ((!a->pageName().isEmpty()) && (c->asTextFrame()))
				continue;
			if ((!a->pageName().isEmpty()) && (c->asImageFrame()) && ((sep) || (!farb)))
				continue;
			double x = a->xOffset();
			double y = a->yOffset();
			double w = a->width();
			double h1 = a->height();
			double ilw=c->lineWidth();
			double x2 = c->BoundingX - ilw / 2.0;
			double y2 = c->BoundingY - ilw / 2.0;
			double w2 = c->BoundingW + ilw;
			double h2 = c->BoundingH + ilw;
			if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h1, y2+h2 )))
				continue;
			if (c->ChangedMasterItem)
				continue;
			if (!c->isTableItem)
				continue;
			if ((c->lineColor() != CommonStrings::None) || (c->lineWidth() == 0.0))
				continue;
			if ((!a->pageName().isEmpty()) && (c->OwnPage != static_cast<int>(a->pageNr())) && (c->OwnPage != -1))
				continue;
			if (c->printEnabled())
			{
				PS_save();
				if (!doOverprint)
				{
					if (c->doOverprint)
					{
						PutStream("true setoverprint\n");
						PutStream("true setoverprintmode\n");
					}
				}
				if (c->lineColor() != CommonStrings::None)
				{
					SetColor(c->lineColor(), c->lineShade(), &h, &s, &v, &k, gcr);
					PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				}
				PS_setlinewidth(c->lineWidth());
				PS_setcapjoin(c->PLineEnd, c->PLineJoin);
				PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
				PS_translate(c->xPos() - a->xOffset(), a->height() - (c->yPos() - a->yOffset()));
				if (c->rotation() != 0)
					PS_rotate(-c->rotation());
				if ((c->TopLine) || (c->RightLine) || (c->BottomLine) || (c->LeftLine))
				{
					if (c->TopLine)
					{
						PS_moveto(0, 0);
						PS_lineto(c->width(), 0);
					}
					if (c->RightLine)
					{
						PS_moveto(c->width(), 0);
						PS_lineto(c->width(), -c->height());
					}
					if (c->BottomLine)
					{
						PS_moveto(0, -c->height());
						PS_lineto(c->width(), -c->height());
					}
					if (c->LeftLine)
					{
						PS_moveto(0, 0);
						PS_lineto(0, -c->height());
					}
					putColor(c->lineColor(), c->lineShade(), false);
				}
				PS_restore();
			}
		}
		Lnr++;
	}
}

void PSLib::HandleGradient(PageItem *c, double w, double h, bool gcr)
{
	int ch,cs,cv,ck;
	double StartX = 0;
	double StartY = 0;
	double EndX = 0;
	double EndY =0;
	ScPattern *pat;
	QMatrix patternMatrix;
	double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
	QList<VColorStop*> cstops = c->fill_gradient.colorStops();
	switch (c->GrType)
	{
/*		case 1:
			StartX = 0;
			StartY = h / 2.0;
			EndX = w;
			EndY = h / 2.0;
			break;
		case 2:
			StartX = w / 2.0;
			StartY = 0;
			EndX = w / 2.0;
			EndY = h;
			break;
		case 3:
			StartX = 0;
			StartY = 0;
			EndX = w;
			EndY = h;
			break;
		case 4:
			StartX = 0;
			StartY = h;
			EndX = w;
			EndY = 0;
			break;
		case 5:
			StartX = w / 2.0;
			StartY = h / 2.0;
			if (w >= h)
			{
				EndX = w;
				EndY = h / 2.0;
			}
			else
			{
				EndX = w / 2.0;
				EndY = h;
			}
			break; */
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			StartX = c->GrStartX;
			StartY = c->GrStartY;
			EndX = c->GrEndX;
			EndY = c->GrEndY;
			break;
		case 8:
			pat = &m_Doc->docPatterns[c->pattern()];
			c->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
			patternMatrix.translate(patternOffsetX, -patternOffsetY);
			patternMatrix.rotate(-patternRotation);
			patternMatrix.scale(pat->scaleX, pat->scaleY);
			patternMatrix.scale(patternScaleX / 100.0 , patternScaleY / 100.0);
			PutStream("Pattern"+c->pattern()+" ["+ToStr(patternMatrix.m11())+" "+ToStr(patternMatrix.m12())+" "+ToStr(patternMatrix.m21())+" "+ToStr(patternMatrix.m22())+" "+ToStr(patternMatrix.dx())+" "+ToStr(patternMatrix.dy())+"] makepattern setpattern\n");
			if (fillRule)
				PutStream("eofill\n");
			else
				PutStream("fill\n");
			return;
			break;
	}
	QList<double> StopVec;
	QStringList Gcolors;
	QStringList colorNames;
	QList<int> colorShades;
	QString hs,ss,vs,ks;
	double lastStop = -1.0;
	double actualStop = 0.0;
	bool isFirst = true;
	if ((c->GrType == 5) || (c->GrType == 7))
	{
		StopVec.clear();
		for (uint cst = 0; cst < c->fill_gradient.Stops(); ++cst)
		{
			actualStop = cstops.at(cst)->rampPoint;
			if ((actualStop != lastStop) || (isFirst))
			{
				isFirst = false;
				lastStop = actualStop;
				StopVec.prepend(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2))*cstops.at(cst)->rampPoint);
				SetColor(cstops.at(cst)->name, cstops.at(cst)->shade, &ch, &cs, &cv, &ck, gcr);
				QString GCol;
				if (GraySc)
					GCol = hs.setNum((255.0 - qMin(0.3 * ch + 0.59 * cs + 0.11 * cv + ck, 255.0))  / 255.0);
				else
					GCol = hs.setNum(ch / 255.0)+" "+ss.setNum(cs / 255.0)+" "+vs.setNum(cv / 255.0)+" "+ks.setNum(ck / 255.0);
				Gcolors.prepend(GCol);
				colorNames.prepend(cstops.at(cst)->name);
				colorShades.prepend(cstops.at(cst)->shade);
			}
		}
		PS_MultiRadGradient(w, -h, StartX, -StartY, StopVec, Gcolors, colorNames, colorShades);
	}
	else
	{
		StopVec.clear();
		for (uint cst = 0; cst < c->fill_gradient.Stops(); ++cst)
		{
			actualStop = cstops.at(cst)->rampPoint;
			if ((actualStop != lastStop) || (isFirst))
			{
				isFirst = false;
				lastStop = actualStop;
				double x = (1 - cstops.at(cst)->rampPoint) * StartX + cstops.at(cst)->rampPoint * EndX;
				double y = (1 - cstops.at(cst)->rampPoint) * StartY + cstops.at(cst)->rampPoint * EndY;
				StopVec.append(x);
				StopVec.append(-y);
				SetColor(cstops.at(cst)->name, cstops.at(cst)->shade, &ch, &cs, &cv, &ck, gcr);
				QString GCol;
				if (GraySc)
					GCol = hs.setNum((255.0 - qMin(0.3 * ch + 0.59 * cs + 0.11 * cv + ck, 255.0))  / 255.0);
				else
					GCol = hs.setNum(ch / 255.0)+" "+ss.setNum(cs / 255.0)+" "+vs.setNum(cv / 255.0)+" "+ks.setNum(ck / 255.0);
				Gcolors.append(GCol);
				colorNames.append(cstops.at(cst)->name);
				colorShades.append(cstops.at(cst)->shade);
			}
		}
		PS_MultiLinGradient(w, -h, StopVec, Gcolors, colorNames, colorShades);
	}
}

void PSLib::SetColor(const QString& farb, double shade, int *h, int *s, int *v, int *k, bool gcr)
{
	ScColor& col = m_Doc->PageColors[farb];
	SetColor(col, shade, h, s, v, k, gcr);
}

void PSLib::SetColor(const ScColor& farb, double shade, int *h, int *s, int *v, int *k, bool gcr)
{
	int h1, s1, v1, k1;
	h1 = *h;
	s1 = *s;
	v1 = *v;
	k1 = *k;
	ScColor tmp(farb);
	if (farb.getColorModel() == colorModelRGB)
		tmp = ScColorEngine::convertToModel(farb, m_Doc, colorModelCMYK);
	if ((gcr) && (!farb.isRegistrationColor()))
		ScColorEngine::applyGCR(tmp, m_Doc);
	tmp.getCMYK(&h1, &s1, &v1, &k1);
	if ((m_Doc->HasCMS) && (ScCore->haveCMS()) && (applyICC))
	{
		h1 = qRound(h1 * shade / 100.0);
		s1 = qRound(s1 * shade / 100.0);
		v1 = qRound(v1 * shade / 100.0);
		k1 = qRound(k1 * shade / 100.0);
		WORD inC[4];
		WORD outC[4];
		inC[0] = h1 * 257;
		inC[1] = s1 * 257;
		inC[2] = v1 * 257;
		inC[3] = k1 * 257;
		cmsDoTransform(solidTransform, inC, outC, 1);
		*h= outC[0] / 257;
		*s = outC[1] / 257;
		*v = outC[2] / 257;
		*k = outC[3] / 257;
	}
	else
	{
		*h = qRound(h1 * shade / 100.0);
		*s = qRound(s1 * shade / 100.0);
		*v = qRound(v1 * shade / 100.0);
		*k = qRound(k1 * shade / 100.0);
	}
}

void PSLib::setTextSt(ScribusDoc* Doc, PageItem* ite, bool gcr, uint argh, Page* pg, bool sep, bool farb, bool ic, bool master)
{
//	qDebug(QString("pslib setTextSt: ownPage=%1 pageNr=%2 OnMasterPage=%3;").arg(ite->OwnPage).arg(pg->pageNr()).arg(ite->OnMasterPage));
	int savedOwnPage = ite->OwnPage;
	ite->OwnPage = argh;
	ite->layout();
	ite->OwnPage = savedOwnPage;
#ifndef NLS_PROTO
	ScText *hl;
	int tabCc = 0;
	QList<ParagraphStyle::TabRecord> tTabValues;
	double tabDist = ite->textToFrameDistLeft();
	if (ite->lineColor() != CommonStrings::None)
		tabDist += ite->lineWidth() / 2.0;

	for (uint ll=0; ll < ite->itemText.lines(); ++ll) {
		LineSpec ls = ite->itemText.line(ll);
		tabDist = ls.x;
		double CurX = ls.x;

		for (int d = ls.firstItem; d <= ls.lastItem; ++d)
		{
			hl = ite->itemText.item(d);
			const CharStyle & cstyle(ite->itemText.charStyle(d));
			const ParagraphStyle& pstyle(ite->itemText.paragraphStyle(d));
			
//			if ((hl->ch[0] == QChar(13)) || (hl->ch[0] == QChar(10)) || (hl->ch[0] == QChar(28)) || (hl->ch[0] == QChar(27)) || (hl->ch[0] == QChar(26)))
//				continue;
			if (hl->effects() & 4096)
				continue;
			tTabValues = pstyle.tabValues();
			if (hl->effects() & 16384)
				tabCc = 0;
			if ((hl->ch == SpecialChars::TAB) && (tTabValues.count() != 0))
			{
				if ((tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
				{
					ScText hl2;
					static_cast<CharStyle&>(hl2) = static_cast<const CharStyle&>(*hl);
					const GlyphLayout * const gl = hl->glyph.more;
					double scale = gl ? gl->scaleV : 1.0;
					double wt    = cstyle.font().charWidth(tTabValues[tabCc].tabFillChar, cstyle.fontSize() * scale / 10.0);
					double len   = hl->glyph.xadvance;
					int coun     = static_cast<int>(len / wt);
					double sPos  = CurX - len + cstyle.fontSize() / 10.0 * 0.7 + 1;
					hl2.ch = tTabValues[tabCc].tabFillChar;
					hl2.setTracking(0);
					hl2.setScaleH(1000);
					hl2.setScaleV(1000);
					hl2.glyph.glyph   = cstyle.font().char2CMap(tTabValues[tabCc].tabFillChar);
					hl2.glyph.yoffset = hl->glyph.yoffset;
					for (int cx = 0; cx < coun; ++cx)
					{
						hl2.glyph.xoffset =  sPos + wt * cx;
						if ((hl2.effects() & 256) && (hl2.strokeColor() != CommonStrings::None))
						{
							ScText hl3;
							static_cast<CharStyle&>(hl3) = static_cast<const CharStyle&>(hl2);
							hl3.ch = hl2.ch;
							hl3.glyph.glyph = hl2.glyph.glyph;
							hl3.setFillColor(hl2.strokeColor());
							hl3.glyph.yoffset = hl2.glyph.yoffset - (hl2.fontSize() * hl2.shadowYOffset() / 10000.0);
							hl3.glyph.xoffset = hl2.glyph.xoffset + (hl2.fontSize() * hl2.shadowXOffset() / 10000.0);
							setTextCh(Doc, ite, CurX, ls.y, gcr, argh, d, &hl3, pstyle, pg, sep, farb, ic, master);
						}
						setTextCh(Doc, ite, CurX, ls.y, gcr, argh, d, &hl2, pstyle, pg, sep, farb, ic, master);
					}
					tabCc++;
				}
				else
				{
					tabCc++;
				}
			}
			if (hl->ch == SpecialChars::TAB) {
				CurX += hl->glyph.wide();
				continue;
			}
			if ((cstyle.effects() & ScStyle_Shadowed) && (cstyle.strokeColor() != CommonStrings::None))
			{
				ScText hl2;
				static_cast<CharStyle&>(hl2) = static_cast<const CharStyle&>(*hl);
				hl2.ch = hl->ch;
				hl2.glyph.glyph = hl->glyph.glyph;
				hl2.setFillColor(cstyle.strokeColor());
				hl2.glyph.yoffset = hl->glyph.yoffset - (cstyle.fontSize() * cstyle.shadowYOffset() / 10000.0);
				hl2.glyph.xoffset = hl->glyph.xoffset + (cstyle.fontSize() * cstyle.shadowXOffset() / 10000.0);
				hl2.glyph.scaleH = hl->glyph.scaleH;
				hl2.glyph.scaleV = hl->glyph.scaleV;
				
				setTextCh(Doc, ite, CurX, ls.y, gcr, argh, d, &hl2, pstyle, pg, sep, farb, ic, master);
			}
			setTextCh(Doc, ite, CurX, ls.y, gcr, argh, d, hl, pstyle, pg, sep, farb, ic, master);
			if (hl->ch == SpecialChars::OBJECT)
			{
				InlineFrame& embedded(const_cast<InlineFrame&>(hl->embedded));
				CurX += (embedded.getItem()->gWidth + embedded.getItem()->lineWidth());
			}
			else
				CurX += hl->glyph.wide();
			tabDist = CurX;
		}
	}
#endif
}

void PSLib::setTextCh(ScribusDoc* Doc, PageItem* ite, double x, double y, bool gcr, uint argh, uint doh, ScText *hl, const ParagraphStyle& pstyle, Page* pg, bool sep, bool farb, bool ic, bool master)
{
#if 0
//#ifndef NLS_PROTO
	QString chstr;
	int h, s, v, k, tsz;
	double wideR;
	QList<double> dum;
	dum.clear();
	chstr = hl->ch;
	tsz = hl->fontSize();
	if (hl->effects() & 2048)
	{
		if (pstyle.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing())
			tsz = qRound(10 * ((Doc->typographicSettings.valueBaseGrid *  (pstyle.dropCapLines()-1)+(hl->font().ascent(pstyle.charStyle().fontSize() / 10.0))) / (hl->font().realCharHeight(chstr[0], 10))));
		else
		{
			if (pstyle.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
				tsz = qRound(10 * ((pstyle.lineSpacing() *  (pstyle.dropCapLines()-1)+(hl->font().ascent(pstyle.charStyle().fontSize() / 10.0))) / (hl->font().realCharHeight(chstr[0], 10))));
			else
			{
				double currasce = hl->font().height(pstyle.charStyle().fontSize());
				tsz = qRound(10 * ((currasce * (pstyle.dropCapLines()-1)+(hl->font().ascent(pstyle.charStyle().fontSize() / 10.0))) / hl->font().realCharHeight(chstr[0], 10)));
			}
		}
	}
	if ((hl->ch == QChar(25)) && (hl->embedded.hasItem()))
	{
		QList<PageItem*> emG = hl->embedded.getGroupedItems();

		for (uint em = 0; em < emG.count(); ++em)
		{
			PageItem* embedded = emG.at(em);
			PS_save();
			PS_translate(x+hl->glyph.xoffset + embedded->gXpos * (hl->scaleH() / 1000.0), (y+hl->glyph.yoffset - (embedded->gHeight * (hl->scaleV() / 1000.0)) + embedded->gYpos * (hl->scaleV() / 1000.0)) * -1);
			if (hl->baselineOffset() != 0)
				PS_translate(0, embedded->gHeight * (hl->baselineOffset() / 1000.0));
			if (hl->scaleH() != 1000)
				PS_scale(hl->scaleH() / 1000.0, 1);
			if (hl->scaleV() != 1000)
				PS_scale(1, hl->scaleV() / 1000.0);
			ProcessItem(Doc, pg, embedded, argh, sep, farb, ic, gcr, master, true);
			PS_restore();
		}
		return;
	}
	if (hl->ch == QChar(29))
		chstr = " ";
	if (hl->ch == QChar(24))
		chstr = "-";
	if (hl->ch == QChar(0xA0))
		chstr = " ";
	if (hl->ch == QChar(30))
	{
		//FIXME Stop duplicating PageItem::ExpandToken code!!!
		if (Doc->masterPageMode())
			chstr = "#";
		else
		{
			uint zae = 0;
			uint za2 = doh;
			do
			{
				if (za2 == 0)
					break;
				za2--;
			}
			while (ite->itemText.text(za2) == QChar(30));
			if (ite->itemText.text(za2) != QChar(30))
				za2++;
			while (ite->itemText.text(za2+zae) == QChar(30))
			{
				zae++;
				if ( ! ite->frameDisplays(za2+zae) )
					break;
			}
			QString out="%1";
			QString out2;
			//CB Section numbering
			//out2 = out.arg(a+Doc->FirstPnum, -zae);
			out2=out.arg(Doc->getSectionPageNumberForPageIndex(argh), -(int)zae);
			chstr = out2.mid(doh-za2, 1);
		}
	}
	if (hl->effects() & 32)
	{
		if (chstr.toUpper() != chstr)
			chstr = chstr.toUpper();
	}
	if (hl->effects() & 64)
	{
		if (chstr.toUpper() != chstr)
		{
			tsz = hl->fontSize() * Doc->typographicSettings.valueSmallCaps / 100;
			chstr = chstr.toUpper();
		}
	}
	if (hl->effects() & 1)
		tsz = hl->fontSize() * Doc->typographicSettings.scalingSuperScript / 100;
	if (hl->effects() & 2)
		tsz = hl->fontSize() * Doc->typographicSettings.scalingSuperScript / 100;
	/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
	ScFace::FontType ftype = hl->font().type();
	if ((ftype == ScFace::TTF) || (hl->font().isOTF()) || (hl->font().subset()))
	{
		uint chr = chstr[0].unicode();
		if ((hl->font().canRender(chstr[0])) && (chr != 32))
		{
			PS_save();
			if (ite->reversed())
			{
				PS_translate(x+hl->glyph.xoffset, (y+hl->glyph.yoffset - (tsz / 10.0)) * -1);
				PS_scale(-1, 1);
				if (ite->frameDisplays(doh+1))
				{
					QString ctx = ite->itemText.text(doh+1, 1);
					if (ctx == QChar(29))
						ctx = " ";
					if (ctx == QChar(0xA0))
						ctx = " ";
					wideR = - hl->font().charWidth(chstr[0], tsz, ctx[0]) * (hl->scaleH() / 1000.0);
				}
				else
					wideR = - hl->font().charWidth(chstr[0], tsz) * (hl->scaleH() / 1000.0);
				PS_translate(wideR, 0);
			}
			else
				PS_translate(x+hl->glyph.xoffset, (y+hl->glyph.yoffset - (tsz / 10.0)) * -1);
			if (hl->baselineOffset() != 0)
				PS_translate(0, (hl->fontSize() / 10.0) * (hl->baselineOffset() / 1000.0));
			if (hl->scaleH() != 100)
				PS_scale(hl->scaleH() / 1000.0, 1);
			if (hl->scaleV() != 100)
			{
				PS_translate(0, -((tsz / 10.0) - (tsz / 10.0) * (hl->scaleV() / 1000.0)));
				PS_scale(1, hl->scaleV() / 1000.0);
			}
			if (hl->fillColor() != CommonStrings::None)
			{
				SetColor(hl->fillColor(), hl->fillShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				if ((colorsToUse[hl->fillColor()].isSpotColor()) && (!DoSep))
					PutStream(ToStr(hl->fillShade() / 100.0)+" "+spotMap[hl->fillColor()]);
				else
					PutStream(FillColor + " cmyk");
						PS_showSub(chr, hl->font().psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ), tsz / 10.0, false);
			}
			PS_restore();
		}
	}
	else
	{
		PS_selectfont(hl->font().replacementName(), tsz / 10.0);
		PS_save();
		PS_translate(x+hl->glyph.xoffset, -y-hl->glyph.yoffset);
		if (ite->reversed())
		{
			int chs = hl->fontSize();
			GlyphLayout dummy;
			ite->layoutGlyphs(*hl, chstr, dummy);
			// chs = ??? FIXME
			PS_scale(-1, 1);
			if (ite->frameDisplays(doh+1))
			{
				QString ctx = ite->itemText.text(doh+1, 1);
				if (ctx[0] == QChar(29))
					ctx = " ";
				if (ctx[0] == QChar(0xA0))
					ctx = " ";
				wideR = - hl->font().charWidth(chstr[0], chs, ctx[0]) * (hl->scaleH() / 1000.0);
				PS_translate(wideR, 0);
			}
			else
			{
				wideR = -hl->font().charWidth(chstr[0], chs) * (hl->scaleH() / 1000.0);
				PS_translate(wideR, 0);
			}
		}
		if (hl->baselineOffset() != 0)
			PS_translate(0, (hl->fontSize() / 10.0) * (hl->baselineOffset() / 1000.0));
		if (hl->scaleH() != 1000)
			PS_scale(hl->scaleH() / 1000.0, 1);
		if (hl->scaleV() != 1000)
			PS_scale(1, hl->scaleV() / 1000.0);
		if (hl->fillColor() != CommonStrings::None)
		{
			SetColor(hl->fillColor(), hl->fillShade(), &h, &s, &v, &k, gcr);
			PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
			if ((colorsToUse[hl->fillColor()].isSpotColor()) && (!DoSep))
			{
				PutStream(ToStr(hl->fillShade() / 100.0)+" "+spotMap[hl->fillColor()]);
				PS_show_xyG(hl->font().replacementName(), chstr, 0, 0, true);
			}
			else
				PS_show_xyG(hl->font().replacementName(), chstr, 0, 0, false);
		}
		PS_restore();
	}
	if ((hl->effects() & 4) && (chstr != QChar(13)))
	{
		if (hl->font().canRender(chstr[0]))
		{
			uint gl = hl->font().char2CMap(chstr[0]);
			FPointArray gly = hl->font().glyphOutline(gl);
			QMatrix chma, chma2, chma3;
			chma.scale(tsz / 100.0, tsz / 100.0);
			chma2.scale(hl->scaleH() / 1000.0, hl->scaleV() / 1000.0);
			if (hl->baselineOffset() != 0)
				chma3.translate(0, -(hl->fontSize() / 10.0) * (hl->baselineOffset() / 1000.0));
			gly.map(chma * chma2 * chma3);
			if (ite->reversed())
			{
				chma = QMatrix();
				chma.scale(-1, 1);
				chma.translate(wideR, 0);
				gly.map(chma);
			}
			if ((hl->strokeColor() != CommonStrings::None) && ((tsz * hl->outlineWidth() / 10000.0) != 0))
			{
				PS_save();
				PS_setlinewidth(tsz * hl->outlineWidth() / 10000.0);
				PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
				PS_setdash(Qt::SolidLine, 0, dum);
				PS_translate(x+hl->glyph.xoffset, (y+hl->glyph.yoffset - (tsz / 10.0)) * -1);
				if (hl->scaleV() != 1000)
					PS_translate(0, -((tsz / 10.0) - (tsz / 10.0) * (hl->scaleV() / 1000.0)));
				SetColor(hl->strokeColor(), hl->strokeShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				SetClipPath(&gly);
				PS_closepath();
				putColor(hl->strokeColor(), hl->strokeShade(), false);
				PS_restore();
			}
		}
	}
	if ((hl->effects() & 16) && (chstr != QChar(13)))
	{
		double Ulen = hl->font().charWidth(chstr[0], hl->fontSize()) * (hl->scaleH() / 1000.0);
		double Upos, lw, kern;
		if (hl->effects() & 16384)
			kern = 0;
		else
			kern = hl->fontSize() * hl->tracking() / 10000.0;
		if ((hl->strikethruOffset() != -1) || (hl->strikethruWidth() != -1))
		{
			if (hl->strikethruOffset() != -1)
				Upos = (hl->strikethruOffset() / 1000.0) * (hl->font().ascent(hl->fontSize() / 10.0));
			else
				Upos = hl->font().strikeoutPos(hl->fontSize() / 10.0);
			if (hl->strikethruWidth() != -1)
				lw = (hl->strikethruWidth() / 1000.0) * (hl->fontSize() / 10.0);
			else
				lw = qMax(hl->font().strokeWidth(hl->fontSize() / 10.0), 1);
		}
		else
		{
			Upos = hl->font().strikeoutPos(hl->fontSize() / 10.0);
			lw = qMax(hl->font().strokeWidth(hl->fontSize() / 10.0), 1);
		}
		if (hl->baselineOffset() != 0)
			Upos += (hl->fontSize() / 10.0) * (hl->baselineOffset() / 1000.0);
		if (hl->fillColor() != CommonStrings::None)
		{
			PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
			PS_setdash(Qt::SolidLine, 0, dum);
			SetColor(hl->fillColor(), hl->fillShade(), &h, &s, &v, &k, gcr);
			PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
		}
		PS_setlinewidth(lw);
		PS_moveto(x+hl->glyph.xoffset-kern, -y-hl->glyph.yoffset+Upos);
		PS_lineto(x+hl->glyph.xoffset+Ulen, -y-hl->glyph.yoffset+Upos);
		putColor(hl->fillColor(), hl->fillShade(), false);
	}
	if (((hl->effects() & 8) && (chstr != QChar(13)))  || ((hl->effects() & 512) && (!chstr[0].isSpace())))
	{
		double Ulen = hl->font().charWidth(chstr[0], hl->fontSize()) * (hl->scaleH() / 1000.0);
		double Upos, lw, kern;
		if (hl->effects() & 16384)
			kern = 0;
		else
			kern = hl->fontSize() * hl->tracking() / 10000.0;
		if ((hl->underlineOffset() != -1) || (hl->underlineWidth() != -1))
		{
			if (hl->underlineOffset() != -1)
				Upos = (hl->underlineOffset() / 1000.0) * (hl->font().descent(hl->fontSize() / 10.0));
			else
				Upos = hl->font().underlinePos(hl->fontSize() / 10.0);
			if (hl->underlineWidth() != -1)
				lw = (hl->underlineWidth() / 1000.0) * (hl->fontSize() / 10.0);
			else
				lw = qMax(hl->font().strokeWidth(hl->fontSize() / 10.0), 1);
		}
		else
		{
			Upos = hl->font().underlinePos(hl->fontSize() / 10.0);
			lw = qMax(hl->font().strokeWidth(hl->fontSize() / 10.0), 1);
		}
		if (hl->baselineOffset() != 0)
			Upos += (hl->fontSize() / 10.0) * (hl->baselineOffset() / 1000.0);
		if (hl->fillColor() != CommonStrings::None)
		{
			PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
			PS_setdash(Qt::SolidLine, 0, dum);
			SetColor(hl->fillColor(), hl->fillShade(), &h, &s, &v, &k, gcr);
			PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
		}
		PS_setlinewidth(lw);
		PS_moveto(x+hl->glyph.xoffset-kern, -y-hl->glyph.yoffset+Upos);
		PS_lineto(x+hl->glyph.xoffset+Ulen, -y-hl->glyph.yoffset+Upos);
		putColor(hl->fillColor(), hl->fillShade(), false);
	}
	if (hl->effects() & 8192)
	{
		int chs = hl->fontSize();
		GlyphLayout dummy;
		ite->layoutGlyphs(*hl, chstr, dummy);
		// chs = ???
		double wide = hl->font().charWidth(chstr[0], chs) * (hl->scaleH() / 1000.0);
		chstr = "-";
		if (hl->font().canRender(chstr[0]))
		{
			uint gl = hl->font().char2CMap(chstr[0]);
			FPointArray gly = hl->font().glyphOutline(gl);
			QMatrix chma;
			chma.scale(tsz / 100.0, tsz / 100.0);
			gly.map(chma);
			chma = QMatrix();
			chma.scale(hl->scaleH() / 1000.0, hl->scaleV() / 1000.0);
			gly.map(chma);
			if (hl->fillColor() != CommonStrings::None)
			{
				PS_save();
				PS_newpath();
				PS_translate(x + hl->glyph.xoffset+wide, (y + hl->glyph.yoffset - (tsz / 10.0)) * -1);
				SetColor(hl->fillColor(), hl->fillShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				SetClipPath(&gly);
				PS_closepath();
				putColor(hl->fillColor(), hl->fillShade(), true);
				PS_restore();
			}
		}
	}
#else
	const CharStyle & cstyle(*hl);
	const GlyphLayout & glyphs(hl->glyph);
	uint glyph = glyphs.glyph;

	int h, s, v, k;
	double tsz;
	double wideR = 0.0;
	QList<double> dum;
	dum.clear();

	QString chstr = hl->ch;

	tsz = cstyle.fontSize();

/*	if (cstyle.effects() & ScStyle_DropCap)
	{
//		QString chstr; // dummy, FIXME: replace by glyph
		if (pstyle.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
			tsz = qRound(10 * ((Doc->typographicSettings.valueBaseGrid *  (pstyle.dropCapLines()-1)+(cstyle.font().ascent(pstyle.charStyle().fontSize() / 10.0))) / (cstyle.font().realCharHeight(chstr[0], 10))));
		else
		{
			if (pstyle.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
				tsz = qRound(10 * ((pstyle.lineSpacing() *  (pstyle.dropCapLines()-1)+(cstyle.font().ascent(pstyle.charStyle().fontSize() / 10.0))) / (cstyle.font().realCharHeight(chstr[0], 10))));
			else
			{
				double currasce = cstyle.font().height(pstyle.charStyle().fontSize());
				tsz = qRound(10 * ((currasce * (pstyle.dropCapLines()-1)+(cstyle.font().ascent(pstyle.charStyle().fontSize() / 10.0))) / cstyle.font().realCharHeight(chstr[0], 10)));
			}
		}
	}
	*/
	if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
	{
		QList<PageItem*> emG = hl->embedded.getGroupedItems();
		QStack<PageItem*> groupStack;
		for (int em = 0; em < emG.count(); ++em)
		{
			PageItem* embedded = emG.at(em);
			if (embedded->isGroupControl)
			{
				PS_save();
				FPointArray cl = embedded->PoLine.copy();
				QMatrix mm;
				mm.translate(x + hl->glyph.xoffset + embedded->gXpos * (cstyle.scaleH() / 1000.0), (y + hl->glyph.yoffset - (embedded->gHeight * (cstyle.scaleV() / 1000.0)) + embedded->gYpos * (cstyle.scaleV() / 1000.0)));
				if (cstyle.baselineOffset() != 0)
					mm.translate(0, embedded->gHeight * (cstyle.baselineOffset() / 1000.0));
				if (cstyle.scaleH() != 1000)
					mm.scale(cstyle.scaleH() / 1000.0, 1);
				if (cstyle.scaleV() != 1000)
					mm.scale(1, cstyle.scaleV() / 1000.0);
				mm.rotate(embedded->rotation());
				cl.map( mm );
				SetClipPath(&cl);
				PS_closepath();
				PS_clip(true);
				groupStack.push(embedded->groupsLastItem);
				continue;
			}
			PS_save();
			PS_translate(x + hl->glyph.xoffset + embedded->gXpos * (cstyle.scaleH() / 1000.0), (y + hl->glyph.yoffset - (embedded->gHeight * (cstyle.scaleV() / 1000.0)) + embedded->gYpos * (cstyle.scaleV() / 1000.0)) * -1);
			if (cstyle.baselineOffset() != 0)
				PS_translate(0, embedded->gHeight * (cstyle.baselineOffset() / 1000.0));
			if (cstyle.scaleH() != 1000)
				PS_scale(cstyle.scaleH() / 1000.0, 1);
			if (cstyle.scaleV() != 1000)
				PS_scale(1, cstyle.scaleV() / 1000.0);
			ProcessItem(Doc, pg, embedded, argh, sep, farb, ic, gcr, master, true);
			PS_restore();
			if (groupStack.count() != 0)
			{
				while (embedded == groupStack.top())
				{
					PS_restore();
					groupStack.pop();
					if (groupStack.count() == 0)
						break;
				}
			}
		}
		return;
	}

	if (glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBSPACE.unicode()) ||
		glyph == (ScFace::CONTROL_GLYPHS + 32)) 
	{
		glyph = cstyle.font().char2CMap(QChar(' '));
		chstr = " ";
	}
	else if (glyph == (ScFace::CONTROL_GLYPHS + SpecialChars::NBHYPHEN.unicode()))
	{
		glyph = cstyle.font().char2CMap(QChar('-'));
		chstr = "-";
	}
	
	if (glyph < ScFace::CONTROL_GLYPHS)
	{
		if (((cstyle.effects() & ScStyle_Underline) && !SpecialChars::isBreak(chstr[0])) //FIXME && (chstr != QChar(13)))  
			|| ((cstyle.effects() & ScStyle_UnderlineWords) && !chstr[0].isSpace() && !SpecialChars::isBreak(chstr[0])))
		{
	//		double Ulen = cstyle.font().glyphWidth(glyph, cstyle.fontSize()) * glyphs.scaleH;
			double Ulen = glyphs.xadvance;
			double Upos, lw, kern;
			if (cstyle.effects() & ScStyle_StartOfLine)
				kern = 0;
			else
				kern = cstyle.fontSize() * cstyle.tracking() / 10000.0;
			if ((cstyle.underlineOffset() != -1) || (cstyle.underlineWidth() != -1))
			{
				if (cstyle.underlineOffset() != -1)
					Upos = (cstyle.underlineOffset() / 1000.0) * (cstyle.font().descent(cstyle.fontSize() / 10.0));
				else
					Upos = cstyle.font().underlinePos(cstyle.fontSize() / 10.0);
				if (cstyle.underlineWidth() != -1)
					lw = (cstyle.underlineWidth() / 1000.0) * (cstyle.fontSize() / 10.0);
				else
					lw = qMax(cstyle.font().strokeWidth(cstyle.fontSize() / 10.0), 1.0);
			}
			else
			{
				Upos = cstyle.font().underlinePos(cstyle.fontSize() / 10.0);
				lw = qMax(cstyle.font().strokeWidth(cstyle.fontSize() / 10.0), 1.0);
			}
			if (cstyle.baselineOffset() != 0)
				Upos += (cstyle.fontSize() / 10.0) * (cstyle.baselineOffset() / 1000.0);
			if (cstyle.fillColor() != CommonStrings::None)
			{
				PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
				PS_setdash(Qt::SolidLine, 0, dum);
				SetColor(cstyle.fillColor(), cstyle.fillShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
			}
			PS_setlinewidth(lw);
			if (cstyle.effects() & ScStyle_Subscript)
			{
				PS_moveto(x + glyphs.xoffset-kern, -y - glyphs.yoffset+Upos);
				PS_lineto(x + glyphs.xoffset+Ulen, -y - glyphs.yoffset+Upos);
			}
			else
			{
				PS_moveto(x + glyphs.xoffset-kern, -y + Upos);
				PS_lineto(x + glyphs.xoffset+Ulen, -y + Upos);
			}
			putColor(cstyle.fillColor(), cstyle.fillShade(), false);
		}
		/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
		ScFace::FontType ftype = cstyle.font().type();
		if ((ftype == ScFace::TTF) || (cstyle.font().isOTF()) || (cstyle.font().subset()))
		{
			if (glyph != 0 && glyph != cstyle.font().char2CMap(QChar(' ')) && (!SpecialChars::isBreak(chstr[0])))
			{
				PS_save();
				if (ite->reversed())
				{
					PS_translate(x + hl->glyph.xoffset, (y + hl->glyph.yoffset - (tsz / 10.0)) * -1);
					PS_scale(-1, 1);
					PS_translate(-glyphs.xadvance, 0);
				}
				else
					PS_translate(x + glyphs.xoffset, (y + glyphs.yoffset - (cstyle.fontSize() / 10.0)) * -1);
				if (cstyle.baselineOffset() != 0)
					PS_translate(0, (cstyle.fontSize() / 10.0) * (cstyle.baselineOffset() / 1000.0));
				if (glyphs.scaleH != 1.0)
					PS_scale(glyphs.scaleH, 1);
				if (glyphs.scaleV != 1.0)
				{
					PS_translate(0, -((tsz / 10.0) - (tsz / 10.0) * (glyphs.scaleV)));
					PS_scale(1, glyphs.scaleV);
				}
				if (cstyle.fillColor() != CommonStrings::None)
				{
					SetColor(cstyle.fillColor(), cstyle.fillShade(), &h, &s, &v, &k, gcr);
					PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
					if ((colorsToUse[cstyle.fillColor()].isSpotColor()) && (!DoSep) && (useSpotColors))
						PutStream(ToStr(cstyle.fillShade() / 100.0)+" "+spotMap[cstyle.fillColor()]);
					else
						PutStream(FillColor + " cmyk");
					PS_showSub(glyph, cstyle.font().psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ), tsz / 10.0, false);
				}
				PS_restore();
			}
		}
		else if (glyph != 0)
		{
			PS_selectfont(cstyle.font().replacementName(), tsz / 10.0);
			PS_save();
			PS_translate(x + glyphs.xoffset, -y - glyphs.yoffset);
			if (ite->reversed())
			{
				PS_scale(-1, 1);
				PS_translate(glyphs.xadvance, 0);
			}
			if (cstyle.baselineOffset() != 0)
				PS_translate(0, (cstyle.fontSize() / 10.0) * (cstyle.baselineOffset() / 1000.0));
			if (glyphs.scaleH != 1.0 || glyphs.scaleV != 1.0)
				PS_scale(glyphs.scaleH, glyphs.scaleV);
			if (cstyle.fillColor() != CommonStrings::None)
			{
				SetColor(cstyle.fillColor(), cstyle.fillShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				if ((colorsToUse[cstyle.fillColor()].isSpotColor()) && (!DoSep) && (useSpotColors))
				{
					PutStream(ToStr(cstyle.fillShade() / 100.0)+" "+spotMap[cstyle.fillColor()]);
					PS_show_xyG(cstyle.font().replacementName(), glyph, 0, 0, true);
				}
				else
					PS_show_xyG(cstyle.font().replacementName(), glyph, 0, 0, false);
			}
			PS_restore();
		}
		if ((cstyle.effects() & ScStyle_Outline) || glyph == 0)//&& (chstr != QChar(13)))
		{
//		if (cstyle.font().canRender(chstr[0]))
			{
				FPointArray gly = cstyle.font().glyphOutline(glyph);
				QMatrix chma, chma2, chma3;
				chma.scale(tsz / 100.0, tsz / 100.0);
				chma2.scale(glyphs.scaleH, glyphs.scaleV);
				if (cstyle.baselineOffset() != 0)
					chma3.translate(0, -(cstyle.fontSize() / 10.0) * (cstyle.baselineOffset() / 1000.0));
				gly.map(chma * chma2 * chma3);
				if (ite->reversed())
				{
					chma = QMatrix();
					chma.scale(-1, 1);
					chma.translate(wideR, 0);
					gly.map(chma);
				}
				if ((cstyle.strokeColor() != CommonStrings::None) && ((tsz * cstyle.outlineWidth() / 10000.0) != 0))
				{
					PS_save();
					PS_setlinewidth(tsz * cstyle.outlineWidth() / 10000.0);
					PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
					PS_setdash(Qt::SolidLine, 0, dum);
					PS_translate(x + glyphs.xoffset, (y + glyphs.yoffset - (tsz / 10.0)) * -1);
					PS_translate(0, -((tsz / 10.0) - (tsz / 10.0) * glyphs.scaleV));
					SetColor(cstyle.strokeColor(), cstyle.strokeShade(), &h, &s, &v, &k, gcr);
					PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
					SetClipPath(&gly);
					PS_closepath();
					putColor(cstyle.strokeColor(), cstyle.strokeShade(), false);
					PS_restore();
				}
			}
		}
		if ((cstyle.effects() & ScStyle_Strikethrough))//&& (chstr != QChar(13)))
		{
			//		double Ulen = cstyle.font().glyphWidth(glyph, cstyle.fontSize()) * glyphs.scaleH;
			double Ulen = glyphs.xadvance;
			double Upos, lw, kern;
			if (cstyle.effects() & 16384)
				kern = 0;
			else
				kern = cstyle.fontSize() * cstyle.tracking() / 10000.0;
			if ((cstyle.strikethruOffset() != -1) || (cstyle.strikethruWidth() != -1))
			{
				if (cstyle.strikethruOffset() != -1)
					Upos = (cstyle.strikethruOffset() / 1000.0) * (cstyle.font().ascent(cstyle.fontSize() / 10.0));
				else
					Upos = cstyle.font().strikeoutPos(cstyle.fontSize() / 10.0);
				if (cstyle.strikethruWidth() != -1)
					lw = (cstyle.strikethruWidth() / 1000.0) * (cstyle.fontSize() / 10.0);
				else
					lw = qMax(cstyle.font().strokeWidth(cstyle.fontSize() / 10.0), 1.0);
			}
			else
			{
				Upos = cstyle.font().strikeoutPos(cstyle.fontSize() / 10.0);
				lw = qMax(cstyle.font().strokeWidth(cstyle.fontSize() / 10.0), 1.0);
			}
			if (cstyle.baselineOffset() != 0)
				Upos += (cstyle.fontSize() / 10.0) * (cstyle.baselineOffset() / 1000.0);
			if (cstyle.fillColor() != CommonStrings::None)
			{
				PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
				PS_setdash(Qt::SolidLine, 0, dum);
				SetColor(cstyle.fillColor(), cstyle.fillShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
			}
			PS_setlinewidth(lw);
			PS_moveto(x + glyphs.xoffset-kern, -y-glyphs.yoffset+Upos);
			PS_lineto(x + glyphs.xoffset+Ulen, -y-glyphs.yoffset+Upos);
			putColor(cstyle.fillColor(), cstyle.fillShade(), false);
		}
	}
	if (glyphs.more) {
		// ugly hack until setTextCh interface is changed
		ScText hl2(*hl);
		hl2.glyph = *glyphs.more;
		setTextCh(Doc, ite, x + glyphs.xadvance, y, gcr, argh, doh, &hl2, pstyle, pg, sep, farb, ic, master);
		// don't let hl2's destructor delete these!
		hl2.glyph.more = 0;
	}
/*	if (cstyle.effects() & ScStyle_SmartHyphenVisible)
	{
		int chs = cstyle.fontSize();
//		double wide = cstyle.font().charWidth(chstr[0], chs) * (cstyle.scaleH() / 1000.0);
//		chstr = "-";
//		if (cstyle.font().canRender(chstr[0]))
		{
			FPointArray gly = cstyle.font().glyphOutline(glyph);
			QMatrix chma;
			chma.scale(tsz / 100.0, tsz / 100.0);
			gly.map(chma);
			chma = QMatrix();
			chma.scale(glyphs.scaleH, glyphs.scaleV);
			gly.map(chma);
			if (cstyle.fillColor() != CommonStrings::None)
			{
				PS_save();
				PS_newpath();
				PS_translate(x + glyphs.xoffset + glyphs.xadvance, (y + glyphs.yoffset - (tsz / 10.0)) * -1);
				SetColor(cstyle.fillColor(), cstyle.fillShade(), &h, &s, &v, &k, gcr);
				PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				SetClipPath(&gly);
				PS_closepath();
				putColor(cstyle.fillColor(), cstyle.fillShade(), true);
				PS_restore();
			}
		}
	}*/
#endif
}

void PSLib::putColor(const QString& color, double shade, bool fill)
{
	if (fill)
	{
		if (((colorsToUse[color].isSpotColor()) || (colorsToUse[color].isRegistrationColor())) && (useSpotColors))
		{
			if (!DoSep)
				PS_fillspot(color, shade);
			else
			{
				if ((color == currentSpot) || (colorsToUse[color].isRegistrationColor()))
				{
					if (fillRule)
						PutStream("0 0 0 "+ToStr(shade / 100.0)+" cmyk eofill\n");
					else
						PutStream("0 0 0 "+ToStr(shade / 100.0)+" cmyk fill\n");
				}
			}
		}
		else
		{
			if (DoSep)
			{
				if ((Plate == 0) || (Plate == 1) || (Plate == 2) || (Plate == 3))
					PS_fill();
			}
			else
				PS_fill();
		}
	}
	else
	{
		if (((colorsToUse[color].isSpotColor()) || (colorsToUse[color].isRegistrationColor())) && (useSpotColors))
		{
			if (!DoSep)
				PS_strokespot(color, shade);
			else
			{
				if ((color == currentSpot) || (colorsToUse[color].isRegistrationColor()))
					PutStream("0 0 0 "+ToStr(shade / 100.0)+" cmyk st\n");
			}
		}
		else
		{
			if (DoSep)
			{
				if ((Plate == 0) || (Plate == 1) || (Plate == 2) || (Plate == 3))
					PS_stroke();
			}
			else
				PS_stroke();
		}
	}
}

void PSLib::SetClipPath(FPointArray *c, bool poly)
{
	FPoint np, np1, np2, np3;
	bool nPath = true;
	if (c->size() > 3)
	{
		for (uint poi=0; poi<c->size()-3; poi += 4)
		{
			if (c->point(poi).x() > 900000)
			{
				if (poly)
					PS_closepath();
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = c->point(poi);
				PS_moveto(np.x(), -np.y());
				nPath = false;
			}
			np = c->point(poi);
			np1 = c->point(poi+1);
			np2 = c->point(poi+3);
			np3 = c->point(poi+2);
			if ((np == np1) && (np2 == np3))
				PS_lineto(np3.x(), -np3.y());
			else
				PS_curve(np1.x(), -np1.y(), np2.x(), -np2.y(), np3.x(), -np3.y());
/*			np = c->point(poi+1);
			np1 = c->point(poi+3);
			np2 = c->point(poi+2);
			PS_curve(np.x(), -np.y(), np1.x(), -np1.y(), np2.x(), -np2.y()); */
		}
	}
}

const QString& PSLib::errorMessage(void)
{
	return ErrorMessage;
}

void PSLib::cancelRequested()
{
	abortExport=true;
}
