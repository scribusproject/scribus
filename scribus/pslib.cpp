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

#include <cstdlib>

#include <QFileInfo>
#include <QImage>
#include <QColor>
#include <QFontInfo>
#include <QList>
#include <QByteArray>
#include <QRegExp>
#include <QBuffer>
#include <QStack>

#include "api/api_application.h"
#include "cmsettings.h"
#include "commonstrings.h"
#include "scconfig.h"
#include "pluginapi.h"
#include "pageitemiterator.h"
#include "pageitem_latexframe.h"
#include "pageitem_table.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scfonts.h"
#include "scribusapp.h"
#include "scribusdoc.h"

#include "scribuscore.h"
#include "selection.h"
#include "scpattern.h"
#include "scstreamfilter_ascii85.h"
#include "scstreamfilter_flate.h"
#include "tableutils.h"
#include "text/textlayoutpainter.h"
#include "ui/multiprogressdialog.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"
#include "text/boxes.h"

using namespace TableUtils;

class PSPainter : public TextLayoutPainter
{
	public:
	PSPainter(ScribusDoc* Doc, uint argh, ScPage* page, bool master, PSLib* ps):
		m_argh(argh),
		m_page(page),
		m_master(master),
		m_ps(ps)
	{}

	void drawGlyph(const GlyphCluster& gc) override;
	void drawGlyphOutline(const GlyphCluster& gc, bool fill) override;
	void drawLine(const QPointF& start, const QPointF& end) override;
	void drawRect(const QRectF& rect) override;
	void drawObject(PageItem* item) override;
	void drawObjectDecoration(PageItem* item) override {}

	private:
		uint m_argh;
		ScPage* m_page {nullptr};
		bool m_master;
		PSLib* m_ps {nullptr};

		void applyTransform()
		{
			if (matrix() != QTransform())
			{
				m_ps->PutStream(m_ps->MatrixToStr(1.0, 0.0, 0.0, -1.0, x(), 0.0) + "\n");
				m_ps->PutStream(m_ps->MatrixToStr(matrix().m11(), -matrix().m12(), matrix().m21(), -matrix().m22(), matrix().dx(), -matrix().dy()) + "\n");
				m_ps->PutStream("[0.0 0.0 0.0 0.0 0.0 0.0] concatmatrix\nconcat\n");
			}
		}

};

void PSPainter::drawGlyph(const GlyphCluster& gc)
{
	if (gc.isControlGlyphs() || gc.isEmpty())
		return;

	m_ps->PS_save();
	applyTransform();
	m_ps->PS_translate(x(), -(y() - fontSize()));

	double current_x = 0.0;
	for (const GlyphLayout& gl : gc.glyphs())
	{
		if (gl.glyph >= ScFace::CONTROL_GLYPHS)
		{
			current_x += gl.xadvance * gl.scaleH;
			continue;
		}

		m_ps->PS_save();
		m_ps->PS_translate(gl.xoffset + current_x, -(fontSize() - fontSize() * gl.scaleV) - gl.yoffset);
		if (gl.scaleH != 1.0 || (gl.scaleV != 1.0))
			m_ps->PS_scale(gl.scaleH, gl.scaleV);

		if (fillColor().color != CommonStrings::None)
			m_ps->putColorNoDraw(fillColor().color, fillColor().shade);
		m_ps->PS_showSub(gl.glyph, m_ps->FontSubsetMap[font().scName()], fontSize(), false);
		m_ps->PS_restore();

		current_x += gl.xadvance * gl.scaleH;
	}
	m_ps->PS_restore();
}

void PSPainter::drawGlyphOutline(const GlyphCluster& gc, bool fill)
{
	if (gc.isControlGlyphs() || gc.isEmpty())
		return;
	if (strokeColor().color == CommonStrings::None)
		return;

	double h, s, v, k;
	QVector<double> dum;

	m_ps->PS_save();
	applyTransform();
	m_ps->PS_setlinewidth(strokeWidth());
	m_ps->PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
	m_ps->PS_setdash(Qt::SolidLine, 0, dum);
	m_ps->PS_translate(x(), -(y() - fontSize()));

	double current_x = 0.0;
	for (const GlyphLayout& gl : gc.glyphs())
	{
		if (gl.glyph >= ScFace::CONTROL_GLYPHS)
		{
			current_x += gl.xadvance * gl.scaleH;
			continue;
		}

		m_ps->PS_save();
		FPointArray gly = font().glyphOutline(gl.glyph);
		QTransform chma;
		chma.scale((fontSize() * gc.scaleH()) / 10.0, (fontSize() * gc.scaleV()) / 10.0);
		gly.map(chma);
		m_ps->PS_translate(gl.xoffset + current_x, -(fontSize() - fontSize() * gc.scaleV()) - gl.yoffset);

		if (fill)
		{
			m_ps->PS_save();
			if (gc.scaleH() != 1.0 || gc.scaleV() != 1.0)
				m_ps->PS_scale(gc.scaleH(), gc.scaleV());
			m_ps->putColorNoDraw(fillColor().color, fillColor().shade);
			m_ps->PS_showSub(gl.glyph, m_ps->FontSubsetMap[font().scName()], fontSize(), false);
			m_ps->PS_restore();
		}

		m_ps->SetColor(strokeColor().color, strokeColor().shade, &h, &s, &v, &k);
		m_ps->PS_setcmykcolor_stroke(h, s, v, k);
		m_ps->SetClipPath(gly, true);
		m_ps->PS_closepath();
		m_ps->putColor(strokeColor().color, strokeColor().shade, false);
		m_ps->PS_restore();

		current_x += gl.xadvance * gl.scaleH;
	}

	m_ps->PS_restore();
}

void PSPainter::drawRect(const QRectF& rect)
{
	double h {0.0}, s {0.0}, v {0.0}, k {0.0};
	m_ps->PS_save();
	applyTransform();
	m_ps->PS_moveto(x() + rect.x(), -y() - rect.y());
	m_ps->PS_lineto(x() + rect.x() + rect.width(), -y() - rect.y());
	m_ps->PS_lineto(x() + rect.x() + rect.width(), -y() - rect.y() - rect.height());
	m_ps->PS_lineto(x() + rect.x(), -y() - rect.y() - rect.height());
	m_ps->PS_closepath();
	m_ps->SetColor(fillColor().color, fillColor().shade, &h, &s, &v, &k);
	m_ps->PS_setcmykcolor_fill(h, s, v, k);
	m_ps->putColor(fillColor().color, fillColor().shade, true);
	m_ps->PS_restore();
}

void PSPainter::drawLine(const QPointF& start, const QPointF& end)
{
	QVector<double> dum;
	dum.clear();
	m_ps->PS_save();
	applyTransform();
	if (fillColor().color != CommonStrings::None)
	{
		double h = 0.0;
		double s = 0.0;
		double v = 0.0;
		double k = 0.0;
		m_ps->PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
		m_ps->PS_setdash(Qt::SolidLine, 0, dum);
		m_ps->SetColor(fillColor().color,fillColor().shade, &h, &s, &v, &k);
		m_ps->PS_setcmykcolor_stroke(h, s, v, k);
	}
	m_ps->PS_setlinewidth(strokeWidth());
	m_ps->PS_moveto(x() + start.x(), -y() - start.y());
	m_ps->PS_lineto(x() + end.x(), -y() - end.y());
	m_ps->putColor(fillColor().color, fillColor().shade, false);
	m_ps->PS_restore();
}

void PSPainter::drawObject(PageItem* item)
{
	m_ps->PS_save();
	m_ps->PS_translate(x(), -y());
	applyTransform();
	if (scaleH() != 1 || scaleV() != 1)
		m_ps->PS_scale(scaleH(), scaleV());
	m_ps->ProcessItem(m_page, item, m_argh, m_master, true);
	m_ps->PS_restore();
}

static QString sanitizeItemName(const QString& itemName)
{
	QString sanitizedName = itemName;
	sanitizedName.replace(".", "_");
	return sanitizedName;
}

PSLib::PSLib(ScribusDoc* doc, PrintOptions &options, OutputFormat outputFmt, ColorList *docColors)
	: m_Doc(doc), m_outputFormat(outputFmt)
{
	Options = options;
	Creator = ScribusAPI::getVersionScribus();

	CMYKColorF cmykValues;
	double c, m, y, k;
	int spotCount = 1;
	bool first = true;
	spotMap.clear();
	colorDesc.clear();

	if (docColors != nullptr)
		colorsToUse = *docColors;
	else
		m_Doc->getUsedColors(colorsToUse);
	colorsToUse.setDocument(m_Doc);

	for (auto itf = colorsToUse.begin(); itf != colorsToUse.end(); ++itf)
	{
		if (((itf->isSpotColor()) || (itf->isRegistrationColor())) && (Options.useSpotColors))
		{
			ScColorEngine::getCMYKValues(*itf, colorsToUse.document(), cmykValues);
			cmykValues.getValues(c, m, y, k);
			colorDesc += "/Spot" + QString::number(spotCount) + " { [ /Separation (";
			if (itf->isRegistrationColor())
				colorDesc += "All";
			else
				colorDesc += itf.key();
			colorDesc += ")\n";
			colorDesc += "/DeviceCMYK\n{\ndup " + ToStr(c) + "\nmul exch dup ";
			colorDesc += ToStr(m) + "\nmul exch dup ";
			colorDesc += ToStr(y) + "\nmul exch ";
			colorDesc += ToStr(k) + " mul }] setcolorspace setcolor} bind def\n";
			spotMap.insert(itf.key(), "Spot" + QString::number(spotCount));
			++spotCount;
		}
		if ((itf.key() != "Cyan") && (itf.key() != "Magenta") && (itf.key() != "Yellow") && (itf.key() != "Black") && itf->isSpotColor())
		{
			ScColorEngine::getCMYKValues(colorsToUse[itf.key()], colorsToUse.document(), cmykValues);
			cmykValues.getValues(c, m, y, k);
			if (!first)
			{
				cmykCustomColors += "%%+ ";
				docCustomColors += "%%+ ";
			}
			cmykCustomColors += ToStr(c) + " " + ToStr(m) + " ";
			cmykCustomColors += ToStr(y) + " " + ToStr(k) + " (" + itf.key() + ")\n";
			docCustomColors += "(" + itf.key() + ")\n";
			first = false;
		}
	}

	QMap<QString, QString> psNameMap;
	SCFonts& allFonts = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts;
	QMap<QString, QMap<uint, QString> > docFonts;
	doc->getUsedFonts(docFonts);

	for (auto it = docFonts.begin(); it != docFonts.end(); ++it)
	{
		// Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed
		// Subset also font whose postscript name conflicts with an already used font
		// Subset always now with new boxes code.
		const ScFace &face (allFonts[it.key()]);
		QMap<uint, QString>& usedGlyphs(it.value());
		QString encodedName = face.psName().simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" );

		// Handle possible PostScript name conflict in oft/ttf fonts
		int psNameIndex = 1;
		QString initialName = encodedName;
		while (psNameMap.contains(encodedName))
		{
			encodedName = QString("%1-%2").arg(initialName).arg(psNameIndex);
			++psNameIndex;
		}
		FontDesc += "/" + encodedName + " " + IToStr(usedGlyphs.count() + 1) + " dict def\n";
		FontDesc += encodedName + " begin\n";

		for (auto ig = usedGlyphs.begin(); ig != usedGlyphs.end(); ++ig)
		{
			uint gid = ig.key();
			FPointArray glyphOutline = face.glyphOutline(gid);

			FontDesc += "/G" + IToStr(gid) + " { newpath\n";
			FPoint np, np1, np2;
			bool nPath = true;
			if (glyphOutline.size() > 3)
			{
				for (int poi = 0; poi < glyphOutline.size() - 3; poi += 4)
				{
					if (glyphOutline.isMarker(poi))
					{
						FontDesc += "cl\n";
						nPath = true;
						continue;
					}
					if (nPath)
					{
						np = glyphOutline.point(poi);
						FontDesc += ToStr(np.x()) + " " + ToStr(-np.y()) + " m\n";
						nPath = false;
					}
					np = glyphOutline.point(poi + 1);
					np1 = glyphOutline.point(poi + 3);
					np2 = glyphOutline.point(poi + 2);
					FontDesc += ToStr(np.x()) + " " + ToStr(-np.y()) + " " +
							ToStr(np1.x()) + " " + ToStr(-np1.y()) + " " +
							ToStr(np2.x()) + " " + ToStr(-np2.y()) + " cu\n";
				}
			}
			FontDesc += "cl\n} bind def\n";
		}
		FontDesc += "end\n";
		FontSubsetMap.insert(face.scName(), encodedName);

		psNameMap.insert(encodedName, face.scName());
	}
}

PSLib::~PSLib()
{
	if (progressDialog)
		progressDialog->deleteLater();
}

void PSLib::PutStream(const QString& c)
{
	QByteArray utf8Array = c.toUtf8();
	spoolStream.writeRawData(utf8Array.data(), utf8Array.length());
}

void PSLib::PutStream(const QByteArray& array, bool hexEnc)
{
	if (hexEnc)
		WriteASCII85Bytes(array);
	else
		spoolStream.writeRawData(array.data(), array.size());
}

void PSLib::PutStream(const char* array, int length, bool hexEnc)
{
	if (hexEnc)
		WriteASCII85Bytes((const unsigned char*) array, length);
	else
		spoolStream.writeRawData(array, length);
}

bool PSLib::PutImageToStream(const ScImage& image, int plate)
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

bool PSLib::PutImageToStream(const ScImage& image, const QByteArray& mask, int plate)
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

QString PSLib::ToStr(double c) const
{
	QString cc;
	return cc.setNum(c);
}

QString PSLib::IToStr(int c) const
{
	QString cc;
	return cc.setNum(c);
}

QString PSLib::MatrixToStr(double m11, double m12, double m21, double m22, double x, double y) const
{
	QString cc("[%1 %2 %3 %4 %5 %6]");
	return  cc.arg(m11).arg(m12).arg(m21).arg(m22).arg(x).arg(y);
}

void PSLib::PS_set_Info(const QString& art, const QString& was)
{
	if (art == "Author")
		User = was;
	if (art == "Creator")
		Creator = was;
	if (art == "Title")
		Title = was;
}

bool PSLib::PS_set_file(const QString& fn)
{
	Spool.setFileName(fn);
	if (Spool.exists())
		Spool.remove();
	bool ret = Spool.open(QIODevice::WriteOnly);
	if (!ret)
	{
		PS_Error( tr("Failed to open file for writing : %1").arg(fn) );
		return false;
	}
	spoolStream.setDevice(&Spool);
	return true;
}

bool PSLib::PS_begin_doc(double x, double y, double width, double height, int numpage)
{
	QString header = (m_outputFormat == OutputPS) ? "%!PS-Adobe-3.0\n" : "%!PS-Adobe-3.0 EPSF-3.0\n";
	PutStream(header);
	PutStream("%%For: " + User + "\n");
	PutStream("%%Title: " + Title + "\n");
	PutStream("%%Creator: " + Creator + "\n");
	PutStream("%%Pages: " + IToStr(numpage) + "\n");

	QString bbox, bboxH;
	if ((width <= height) || (m_outputFormat == OutputEPS))
	{
		bbox = "%%BoundingBox: " + IToStr(qRound(x)) + " " + IToStr(qRound(y)) + " " + IToStr(qRound(width)) + " " + IToStr(qRound(height)) + "\n";
		bboxH = "%%HiResBoundingBox: " + ToStr(x) + " " + ToStr(y) + " " + ToStr(width) + " " + ToStr(height) + "\n";
	}
	else
	{
		bbox = "%%BoundingBox: " + IToStr(qRound(x)) + " " + IToStr(qRound(y)) + " " + IToStr(qRound(height)) + " " + IToStr(qRound(width)) + "\n";
		bboxH = "%%HiResBoundingBox: " + ToStr(x) + " " + ToStr(y) + " " + ToStr(height) + " " + ToStr(width) + "\n";
	}
	PutStream(bbox);
	PutStream(bboxH);

	if (!docCustomColors.isEmpty())
		PutStream("%%DocumentCustomColors: " + docCustomColors);
	if (!cmykCustomColors.isEmpty())
		PutStream("%%CMYKCustomColor: " + cmykCustomColors);
	PutStream("%%LanguageLevel: 3\n");
	PutStream("%%EndComments\n");
	if ((m_outputFormat == OutputPS) && Options.setDevParam)
	{
		PutStream("%%BeginDefaults\n");
		if (width <= height)
			PutStream("%%ViewingOrientation: 1 0 0 1\n");
		else
			PutStream("%%ViewingOrientation: 0 1 -1 0\n");
		PutStream("%%EndDefaults\n");
	}
	else if (m_outputFormat == OutputEPS)
	{
		PutStream("%%BeginDefaults\n");
		PutStream("%%ViewingOrientation: 1 0 0 1\n");
		PutStream("%%EndDefaults\n");
	}

	QString prolog = "%%BeginProlog\n";
	prolog += "/Scribusdict 100 dict def\n";
	prolog += "Scribusdict begin\n";
	prolog += "/sp {showpage} bind def\n";
	prolog += "/oldsetgray /setgray load def\n";
	prolog += "/cmyk {setcmykcolor} def\n";
	prolog += "/m {moveto} bind def\n";
	prolog += "/l {lineto} bind def\n";
	prolog += "/li {lineto} bind def\n";
	prolog += "/cu {curveto} bind def\n";
	prolog += "/cl {closepath} bind def\n";
	prolog += "/gs {gsave} bind def\n";
	prolog += "/gr {grestore} bind def\n";
	prolog += "/tr {translate} bind def\n";
	prolog += "/ro {rotate} bind def\n";
	prolog += "/sh {show} bind def\n";
	prolog += "/shg {setcmykcolor moveto glyphshow} def\n";
	prolog += "/shgsp {moveto glyphshow} def\n";
	prolog += "/sc {scale} bind def\n";
	prolog += "/se {selectfont} bind def\n";
	prolog += "/sf {setfont} bind def\n";
	prolog += "/sw {setlinewidth} bind def\n";
	prolog += "/f  {findfont} bind def\n";
	prolog += "/fi {fill} bind def\n";
	prolog += "/st {stroke} bind def\n";
	prolog += "/shgf {gs dup scale begin cvx exec fill end gr} bind def\n";
	prolog += "/shgs {gs dup 1 exch div currentlinewidth mul sw dup scale\n";
	prolog += "       begin cvx exec st end gr} bind def\n";
	prolog += "/bEPS {\n";
	prolog += "    /b4_Inc_state save def\n";
	prolog += "    /dict_count countdictstack def\n";
	prolog += "    /op_count count 1 sub def\n";
	prolog += "    userdict begin\n";
	prolog += "    /showpage { } def\n";
	prolog += "    0 setgray 0 setlinecap\n";
	prolog += "    1 setlinewidth 0 setlinejoin\n";
	prolog += "    10 setmiterlimit [ ] 0 setdash newpath\n";
	prolog += "    /languagelevel where\n";
	prolog += "    {pop languagelevel\n";
	prolog += "    1 ne\n";
	prolog += "    {false setstrokeadjust false setoverprint\n";
	prolog += "    } if } if } bind def\n";
	prolog += "/eEPS { count op_count sub {pop} repeat\n";
	prolog += "    countdictstack dict_count sub {end} repeat\n";
	prolog += "    b4_Inc_state restore } bind def\n";
	prolog += "    end\n";
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks))
		prolog += "/rb { [ /Separation (All)\n/DeviceCMYK { dup 0 mul exch dup 0 mul exch dup 0 mul exch 1 mul }\n] setcolorspace setcolor} bind def\n";
	prolog += "%%EndProlog\n";
	PutStream(prolog);

	PutStream("%%BeginSetup\n");
	if (Options.includePDFMarks)
		PutStream("/pdfmark where {pop} {userdict /pdfmark /cleartomark load put} ifelse\n");
	if (!FontDesc.isEmpty())
		PutStream(FontDesc);
	if ((!colorDesc.isEmpty()) && (!Options.outputSeparations))
		PutStream(colorDesc);
	if (GraySc)
	{
		QString grayCalc =  "/setcmykcolor {exch 0.11 mul add exch 0.59 mul add exch 0.3 mul add\n";
		grayCalc += "               dup 1 gt {pop 1} if 1 exch sub oldsetgray} bind def\n";
		grayCalc += "/setrgbcolor {0.11 mul exch 0.59 mul add exch 0.3 mul add\n";
		grayCalc += "              oldsetgray} bind def\n";
		PutStream(grayCalc);
	}
		
	Optimization optim = m_optimization;
	m_optimization = OptimizeSize;
	QStringList patterns = m_Doc->getPatternDependencyList(m_Doc->getUsedPatterns());
	for (int i = 0; i < patterns.count(); ++i)
	{
		QString patternName(patterns.at(i));
		ScPattern pa = m_Doc->docPatterns[patternName];
		for (PageItemIterator it(pa.items); *it; ++it)
		{
			PageItem* item = *it;
			if ((item->isImageFrame()) && (item->imageIsAvailable) && (!item->Pfile.isEmpty()) && (item->printEnabled()) && (!Options.outputSeparations) && (Options.useColor))
			{
				if (!PS_ImageData(item, item->Pfile, item->itemName(), item->ImageProfile, item->UseEmbedded))
					return false;
			}
		}
		uint patHash = qHash(patternName);
		PutStream("/Pattern" + QString::number(patHash) + " 8 dict def\n");
		PutStream("Pattern" + QString::number(patHash) + " begin\n");
		PutStream("/PatternType 1 def\n");
		PutStream("/PaintType 1 def\n");
		PutStream("/TilingType 1 def\n");
		PutStream("/BBox [ 0 0 " + ToStr(pa.width) + " " + ToStr(pa.height) + "] def\n");
		PutStream("/XStep " + ToStr(pa.width) + " def\n");
		PutStream("/YStep " + ToStr(pa.height) + " def\n");
		PutStream("/PaintProc {\n");
		QIODevice *spStream = spoolStream.device();
		QByteArray buf;
		QBuffer b(&buf);
		b.open( QIODevice::WriteOnly );
		spoolStream.setDevice(&b);
//		PS_translate(pa.items.at(0)->gXpos, -pa.items.at(0)->gYpos);
		for (int j = 0; j < pa.items.count(); ++j)
		{
			PageItem* item = pa.items.at(j);
			PutStream("{\n");
			PS_save();
			PS_translate(item->gXpos, pa.height - item->gYpos);
			ProcessItem(m_Doc->Pages->at(0), item, 0, false, true, true);
			PS_restore();
			PutStream("} exec\n");
		}
		spoolStream.setDevice(spStream);
		PutStream(buf);
		PutStream("} def\n");
		PutStream("end\n");
	}
	m_optimization = optim;

	bool errorOccured = false;
	for (int ap = 0; ap < m_Doc->MasterPages.count() && !abortExport && !errorOccured; ++ap)
	{
		if (progressDialog)
		{
			progressDialog->setOverallProgress(ap);
			progressDialog->setProgress("EMP", ap);
			QCoreApplication::processEvents();
		}
		if (m_Doc->MasterItems.count() <= 0)
			continue;

		ScLayer ll;
		ll.isPrintable = false;
		ll.ID = 0;
		for (int layerLevel = 0; layerLevel < m_Doc->Layers.count() && !abortExport && !errorOccured; ++layerLevel)
		{
			m_Doc->Layers.levelToLayer(ll, layerLevel);
			if (!ll.isPrintable)
				continue;
			for (int api = 0; api < m_Doc->MasterItems.count() && !abortExport; ++api)
			{
				PageItem *it = m_Doc->MasterItems.at(api);
				if (progressDialog)
					QCoreApplication::processEvents();
				if ((it->m_layerID != ll.ID) || (!it->printEnabled()))
					continue;
				double bLeft, bRight, bBottom, bTop;
				GetBleeds(m_Doc->MasterPages.at(ap), bLeft, bRight, bBottom, bTop);
				double x1 = m_Doc->MasterPages.at(ap)->xOffset() - bLeft;
				double y1 = m_Doc->MasterPages.at(ap)->yOffset() - bTop;
				double w1 = m_Doc->MasterPages.at(ap)->width() + bLeft + bRight;
				double h1 = m_Doc->MasterPages.at(ap)->height()+ bBottom + bTop;
				double lw = it->visualLineWidth();
				double x2 = it->BoundingX - lw / 2.0;
				double y2 = it->BoundingY - lw / 2.0;
				double w2 = qMax(it->BoundingW + lw, 1.0);
				double h2 = qMax(it->BoundingH + lw, 1.0);
				if (!QRectF(x2, y2, w2, h2).intersects(QRectF(x1, y1, w1, h1)))
					continue;
				if ((it->OwnPage != m_Doc->MasterPages.at(ap)->pageNr()) && (it->OwnPage != -1))
					continue;
				if ((m_optimization == OptimizeSize) && it->isImageFrame() && it->imageIsAvailable && (!it->Pfile.isEmpty()) && it->printEnabled() && (!Options.outputSeparations) && Options.useColor)
				{
					errorOccured = !PS_ImageData(it, it->Pfile, it->itemName(), it->ImageProfile, it->UseEmbedded);
					if (errorOccured) break;
				}
				PS_TemplateStart(QString("mp_obj_%1_%2").arg(ap).arg(qHash(it)));
				ProcessItem(m_Doc->MasterPages.at(ap), it, ap+1, true);
				PS_TemplateEnd();
			}
			if (errorOccured) break;
		}
	}
	PutStream("%%EndSetup\n");

	return (!errorOccured);
}

QString PSLib::PSEncode(const QString& in) const
{
	static QRegExp badchars("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]");
	QString tmp(in.simplified().replace( badchars, "_" ));
	return tmp;
}

void PSLib::PS_TemplateStart(const QString& Name)
{
	PutStream("/" + PSEncode(Name) + "\n{\n");
}

void PSLib::PS_UseTemplate(const QString& Name)
{
	PutStream(PSEncode(Name) + "\n");
}

void PSLib::PS_TemplateEnd()
{
	PutStream("} bind def\n");
}

void PSLib::PS_begin_page(ScPage* pg, MarginStruct* Ma, bool clipping)
{
	double bleedRight = 0.0;
	double bleedLeft = 0.0;
	double markOffs = 0.0;
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks))
		markOffs = Options.markLength + Options.markOffset;
	if ((Options.registrationMarks) || (Options.colorMarks))
		markOffs = qMax(markOffs, Options.markOffset + 20.0);
	GetBleeds(pg, bleedLeft, bleedRight);
	double maxBoxX = pg->width() + bleedLeft + bleedRight + markOffs * 2.0;
	double maxBoxY = pg->height() + Options.bleeds.bottom() + Options.bleeds.top() + markOffs * 2.0;
	PageIndex++;
	PutStream("%%Page: " + IToStr(PageIndex) + " " + IToStr(PageIndex) + "\n");
	if (m_outputFormat == OutputPS)
	{
		if (pg->orientation() == 0)
		{
			PutStream("%%PageOrientation: Portrait\n");
			PutStream("%%PageBoundingBox: 0 0 " + IToStr(qRound(maxBoxX)) + " " + IToStr(qRound(maxBoxY)) + "\n");
			PutStream("%%PageCropBox: " + ToStr(bleedLeft + markOffs) + " " + ToStr(Options.bleeds.bottom() + markOffs) + " " + ToStr(maxBoxX - bleedRight - markOffs * 2.0) + " " + ToStr(maxBoxY - Options.bleeds.top() - markOffs * 2.0) + "\n");
		}
		else
		{
			PutStream("%%PageOrientation: Landscape\n");
			PutStream("%%PageBoundingBox: 0 0 " + IToStr(qRound(maxBoxY)) + " " + IToStr(qRound(maxBoxX)) + "\n");
			PutStream("%%PageCropBox: " + ToStr(bleedLeft + markOffs) + " " + ToStr(Options.bleeds.bottom() + markOffs) + " " + ToStr(maxBoxY - Options.bleeds.top() - markOffs * 2.0) + " " + ToStr(maxBoxX - bleedRight - markOffs * 2.0) + "\n");
		}
	}
	PutStream("Scribusdict begin\n");
	if ((m_outputFormat == OutputPS) && (Options.setDevParam))
	{
		if (pg->orientation() == 0)
			PutStream("<< /PageSize [ " + ToStr(maxBoxX) + " " + ToStr(maxBoxY) + " ]\n");
		else
			PutStream("<< /PageSize [ " + ToStr(maxBoxY) + " " + ToStr(maxBoxX) + " ]\n");
		PutStream(">> setpagedevice\n");
	}
	PutStream("save\n");
	if ((pg->orientation() == 1) && (m_outputFormat == OutputPS))
		PutStream("90 rotate 0 " + IToStr(qRound(maxBoxY)) + " neg translate\n");
	PutStream("/DeviceCMYK setcolorspace\n");
	// Clip to bleeds
	QString clipStr;
	double bbWidth  = pg->width()  + bleedLeft + bleedRight;
	double bbHeight = pg->height() + Options.bleeds.bottom() + Options.bleeds.top();
	clipStr += ToStr(markOffs) + " " + ToStr(markOffs) + " m\n";
	clipStr += ToStr(markOffs + bbWidth) + " " + ToStr(markOffs) + " li\n";
	clipStr += ToStr(markOffs + bbWidth) + " " + ToStr(markOffs + bbHeight) + " li\n";
	clipStr += ToStr(markOffs) + " " + ToStr(markOffs + bbHeight) + " li cl clip newpath\n";
	PutStream(clipStr);
	// Move to page origin
	PutStream(ToStr(bleedLeft + markOffs) + " " + ToStr(Options.bleeds.bottom() + markOffs) + " tr\n");
	m_currentPage = pg;
	// Clip to margins if requested
	if (clipping)
	{
		QString clipPath = ToStr(Ma->left()) + " " + ToStr(Ma->bottom()) + " m\n";
		clipPath += ToStr(pg->width() - Ma->right()) + " " + ToStr(Ma->bottom()) + " li\n";
		clipPath += ToStr(pg->width() - Ma->right()) + " " + ToStr(pg->height() - Ma->top()) + " li\n";
		clipPath += ToStr(Ma->left()) + " " + ToStr(pg->height() - Ma->top()) + " li cl clip newpath\n";
		PutStream(clipPath);
	}
}

void PSLib::PS_end_page()
{
	PutStream("%%PageTrailer\nrestore\n");
	double markOffs = 0.0;
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks))
		markOffs = Options.markLength + Options.markOffset;
	if ((Options.registrationMarks) || (Options.colorMarks))
		markOffs = qMax(markOffs, Options.markOffset + 20.0);
	double markDelta = markOffs - (Options.markLength + Options.markOffset);
	double bleedRight, bleedLeft;
	GetBleeds(m_currentPage, bleedLeft, bleedRight);
	double maxBoxX = m_currentPage->width() + bleedLeft + bleedRight + markOffs * 2.0;
	double maxBoxY = m_currentPage->height() + Options.bleeds.bottom() + Options.bleeds.top() + markOffs * 2.0;
	PutStream("gs\n");
	if ((m_currentPage->orientation() == 1) && (m_outputFormat == OutputPS))
		PutStream("90 rotate 0 " + IToStr(qRound(maxBoxY)) + " neg translate\n");
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
			PutStream(ToStr(markDelta) + " " + ToStr(markOffs + Options.bleeds.bottom()) + " m\n");
			PutStream(ToStr(markDelta + Options.markLength) + " " + ToStr(markOffs + Options.bleeds.bottom()) + " li\n");
			PutStream("st\n");
			PutStream(ToStr(markOffs + bleedLeft) + " " + ToStr(markDelta) + " m\n");
			PutStream(ToStr(markOffs + bleedLeft) + " " + ToStr(markDelta + Options.markLength) + " li\n");
			PutStream("st\n");
		// Top Left
			PutStream(ToStr(markDelta) + " " + ToStr(maxBoxY - Options.bleeds.top() - markOffs) + " m\n");
			PutStream(ToStr(markDelta + Options.markLength) + " " + ToStr(maxBoxY - Options.bleeds.top() - markOffs) + " li\n");
			PutStream("st\n");
			PutStream(ToStr(markOffs + bleedLeft) + " " + ToStr(maxBoxY - markDelta) + " m\n");
			PutStream(ToStr(markOffs + bleedLeft) + " " + ToStr(maxBoxY - markDelta - Options.markLength)  + " li\n");
			PutStream("st\n");
		// Bottom Right
			PutStream(ToStr(maxBoxX - markDelta) + " " + ToStr(markOffs + Options.bleeds.bottom()) + " m\n");
			PutStream(ToStr(maxBoxX - markDelta - Options.markLength) + " " + ToStr(markOffs + Options.bleeds.bottom()) + " li\n");
			PutStream("st\n");
			PutStream(ToStr(maxBoxX - bleedRight - markOffs) + " " +  ToStr(markDelta) + " m\n");
			PutStream(ToStr(maxBoxX - bleedRight - markOffs) + " " +  ToStr(markDelta + Options.markLength)  + " li\n");
			PutStream("st\n");
		// Top Right
			PutStream(ToStr(maxBoxX - markDelta) + " " + ToStr(maxBoxY - Options.bleeds.top() - markOffs) + " m\n");
			PutStream(ToStr(maxBoxX - markDelta - Options.markLength) + " " + ToStr(maxBoxY - Options.bleeds.top() - markOffs) + " li\n");
			PutStream("st\n");
			PutStream(ToStr(maxBoxX - bleedRight - markOffs) + " " +  ToStr(maxBoxY - markDelta) + " m\n");
			PutStream(ToStr(maxBoxX - bleedRight - markOffs) + " " +  ToStr(maxBoxY - markDelta - Options.markLength)  + " li\n");
			PutStream("st\n");
		}
		if (Options.bleedMarks)
		{
			PutStream("gs\n");
			PutStream("[3 1 1 1] 0 setdash\n");
		// Bottom Left
			PutStream(ToStr(markDelta) + " " + ToStr(markOffs) + " m\n");
			PutStream(ToStr(markDelta + Options.markLength) + " " + ToStr(markOffs) + " li\n");
			PutStream("st\n");
			PutStream(ToStr(markOffs) + " " + ToStr(markDelta) + " m\n");
			PutStream(ToStr(markOffs) + " " + ToStr(markDelta + Options.markLength) + " li\n");
			PutStream("st\n");
		// Top Left
			PutStream(ToStr(markDelta) + " " + ToStr(maxBoxY - markOffs) + " m\n");
			PutStream(ToStr(markDelta + Options.markLength) + " " + ToStr(maxBoxY - markOffs) + " li\n");
			PutStream("st\n");
			PutStream(ToStr(markOffs) + " " + ToStr(maxBoxY - markDelta) + " m\n");
			PutStream(ToStr(markOffs) + " " + ToStr(maxBoxY - markDelta - Options.markLength) + " li\n");
			PutStream("st\n");
		// Bottom Right
			PutStream(ToStr(maxBoxX - markDelta) + " " + ToStr(markOffs) + " m\n");
			PutStream(ToStr(maxBoxX - markDelta - Options.markLength) + " " + ToStr(markOffs) + " li\n");
			PutStream("st\n");
			PutStream(ToStr(maxBoxX - markOffs) + " " + ToStr(markDelta) + " m\n");
			PutStream(ToStr(maxBoxX - markOffs) + " " + ToStr(markDelta + Options.markLength) + " li\n");
			PutStream("st\n");
		// Top Right
			PutStream(ToStr(maxBoxX - markDelta) + " " + ToStr(maxBoxY - markOffs) + " m\n");
			PutStream(ToStr(maxBoxX - markDelta - Options.markLength) + " " + ToStr(maxBoxY - markOffs) + " li\n");
			PutStream("st\n");
			PutStream(ToStr(maxBoxX - markOffs) + " " + ToStr(maxBoxY - markDelta) + " m\n");
			PutStream(ToStr(maxBoxX - markOffs) + " " + ToStr(maxBoxY - markDelta - Options.markLength) + " li\n");
			PutStream("st\n");
			PutStream("gr\n");
		}
		if (Options.registrationMarks)
		{
			double regDelta  = markOffs - Options.markOffset;
			QString regCross = "0 7 m\n14 7 li\n7 0 m\n7 14 li\n13 7 m\n13 10.31383 10.31383 13 7 13 cu\n3.68629 13 1 10.31383 1 7 cu\n1 3.68629 3.68629 1 7 1 cu\n";
			regCross += "10.31383 1 13 3.68629 13 7 cu\ncl\n10.5 7 m\n10.5 8.93307 8.93307 10.5 7 10.5 cu\n5.067 10.5 3.5 8.93307 3.5 7 cu\n";
			regCross += "3.5 5.067 5.067 3.5 7 3.5 cu\n8.93307 3.5 10.5 5.067 10.5 7 cu\ncl\nst\n";
			PutStream("gs\n");
			PutStream(ToStr(maxBoxX / 2.0 - 7.0) + " " + ToStr(regDelta - 17) + " tr\n");
			PutStream(regCross);
			PutStream("gr\n");
			PutStream("gs\n");
			PutStream(ToStr(regDelta - 17) + " " + ToStr(maxBoxY / 2.0 - 7.0) + " tr\n");
			PutStream(regCross);
			PutStream("gr\n");
			PutStream("gs\n");
			PutStream(ToStr(maxBoxX / 2.0 - 7.0) + " " + ToStr(maxBoxY - regDelta + 3.0) + " tr\n");
			PutStream(regCross);
			PutStream("gr\n");
			PutStream("gs\n");
			PutStream(ToStr(maxBoxX - regDelta + 3.0) + " " + ToStr(maxBoxY / 2.0 - 7.0) + " tr\n");
			PutStream(regCross);
			PutStream("gr\n");
		}
		if (Options.colorMarks)
		{
			double startX = markOffs + bleedLeft + 6.0;
			double startY = maxBoxY - markOffs + Options.markOffset + 2.0;
			PutStream("0 0 0 1 cmyk\n");
			double col = 1.0;
			for (int bl = 0; bl < 11; bl++)
			{
				PutStream("0 0 0 " + ToStr(col) + " cmyk\n");
				PutStream(ToStr(startX + bl * 14.0) + " " + ToStr(startY) + " 14 14 rectfill\n");
				PutStream("0 0 0 1 cmyk\n");
				PutStream(ToStr(startX + bl * 14.0) + " " + ToStr(startY) + " 14 14 rectstroke\n");
				col -= 0.1;
			}
			startX = maxBoxX - bleedRight - markOffs - 20.0;
			PutStream("0 0 0 0.5 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 0 0.5 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 0.5 0 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0.5 0 0 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("1 1 0 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("1 0 1 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 1 1 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 0 1 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("0 1 0 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
			startX -= 14.0;
			PutStream("1 0 0 0 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectfill\n");
			PutStream("0 0 0 1 cmyk\n");
			PutStream(ToStr(startX) + " " + ToStr(startY) + " 14 14 rectstroke\n");
		}
		PutStream("gr\n");
	}
	PutStream("gr\n");
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
	PutStream(mu ? "eoclip newpath\n" : "clip newpath\n");
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

void PSLib::PS_setdash(Qt::PenStyle st, double offset, QVector<double> dash)
{
	if (dash.count() != 0)
	{
		PutStream("[ ");
		for (double dashValue : dash)
		{
			PutStream(ToStr(dashValue) + " ");
		}
		PutStream("] " + ToStr(offset) + " setdash\n");
	}
	else
		PutStream("[" + getDashString(st, LineW) + "] 0 setdash\n");
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

void PSLib::PS_fill()
{
	if (fillRule)
		PutStream(FillColor + " cmyk eofill\n");
	else
		PutStream(FillColor + " cmyk fill\n");
}

void PSLib::PS_fillspot(const QString& color, double shade)
{
	if (fillRule)
		PutStream(ToStr(shade / 100.0) + " " + spotMap[color] + " eofill\n");
	else
		PutStream(ToStr(shade / 100.0) + " " + spotMap[color] + " fill\n");
}

void PSLib::PS_strokespot(const QString& color, double shade)
{
	PutStream(ToStr(shade / 100.0) + " " + spotMap[color] + " st\n");
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

void PSLib::PS_show(double x, double y)
{
	PS_moveto(x, y);
	PutStream("/hyphen glyphshow\n");
}

void PSLib::PS_showSub(uint chr, const QString& font, double size, bool stroke)
{
	PutStream(" (G" + IToStr(chr) + ") " + font + " " + ToStr(size / 10.0) + " ");
	PutStream(stroke ? "shgs\n" : "shgf\n");
}

bool PSLib::PS_ImageData(PageItem *item, const QString& fn, const QString& Name, const QString& Prof, bool UseEmbedded)
{
	bool dummy;
	QByteArray tmp;
	QFileInfo fi(fn);
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(fn);
	if (extensionIndicatesEPS(ext) && (item->pixm.imgInfo.type != ImageType7))
	{
		if (loadRawText(fn, tmp))
		{
			PutStream("currentfile 1 (%ENDEPSDATA) /SubFileDecode filter /ReusableStreamDecode filter\n");
			PutStream("%%BeginDocument: " + fi.fileName() + "\n");
			if (getDouble(tmp.mid(0, 4), true) == 0xC5D0D3C6)
			{
				const char* data = tmp.data();
				uint startPos = getDouble(tmp.mid(4, 4), false);
				uint length = getDouble(tmp.mid(8, 4), false);
				PutStream(data + startPos, length, false);
			}
			else
				PutStream(tmp, false);
			PutStream("\n%ENDEPSDATA\n");
			PutStream("%%EndDocument\n");
			PutStream("/" + PSEncode(Name) + "Bild exch def\n");
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
	image.imgInfo.RequestProps = item->pixm.imgInfo.RequestProps;
	image.imgInfo.isRequest = item->pixm.imgInfo.isRequest;
	CMSettings cms(item->doc(), Prof, item->ImageIntent);
	cms.allowColorManagement(true);
	cms.setUseEmbeddedProfile(UseEmbedded);
	if (!image.loadPicture(fn, item->pixm.imgInfo.actualPageNumber, cms, ScImage::CMYKData, 300, &dummy))
	{
		PS_Error_ImageLoadFailure(fn);
		return false;
	}
	image.applyEffect(item->effectsInUse, colorsToUse, true);
	QByteArray maskArray;
	if (item->pixm.imgInfo.type != ImageType7)
	{
		bool alphaLoaded = image.getAlpha(fn, item->pixm.imgInfo.actualPageNumber, maskArray, false, true, 300);
		if (!alphaLoaded)
		{
			PS_Error_MaskLoadFailure(fn);
			return false;
		}
	}
	if ((maskArray.size() > 0) && (item->pixm.imgInfo.type != ImageType7))
	{
		PutStream("currentfile /ASCII85Decode filter /FlateDecode filter /ReusableStreamDecode filter\n");
		if (!PutImageToStream(image, maskArray, -1))
		{
			PS_Error_ImageDataWriteFailure();
			return false;
		}
		PutStream("/" + PSEncode(Name) + "Bild exch def\n");
	}
	else
	{
		PutStream("currentfile /ASCII85Decode filter /FlateDecode filter /ReusableStreamDecode filter\n");
		if (!PutImageToStream(image, -1))
		{
			PS_Error_ImageDataWriteFailure();
			return false;
		}
		PutStream("/" + PSEncode(Name) + "Bild exch def\n");
		imgArray.resize(0);
	}
	return true;
}

bool PSLib::PS_image(PageItem *item, double x, double y, const QString& fn, double scalex, double scaley, const QString& Prof, bool UseEmbedded, const QString& Name)
{
	bool dummy;
	QByteArray tmp;

	QFileInfo fi(fn);
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(fn);
	if (extensionIndicatesEPS(ext) && (item->pixm.imgInfo.type != ImageType7))
	{
		if (loadRawText(fn, tmp))
		{
			PutStream("bEPS\n");
			PutStream(ToStr(PrefsManager::instance().appPrefs.extToolPrefs.gs_Resolution / 72.0 * scalex) + " " + ToStr(PrefsManager::instance().appPrefs.extToolPrefs.gs_Resolution / 72.0 * scaley) + " sc\n");
			PutStream(ToStr(-item->BBoxX+x * scalex) + " " + ToStr(y * scalex) + " tr\n");
			int h = item->pixm.height();
			PutStream("0 " + ToStr(h*scaley) + " tr\n");
			PutStream(ToStr(-item->imageRotation()) + " ro\n");
			PutStream("0 " + ToStr(-h*scaley) + " tr\n");
			if (!Name.isEmpty())
			{
				PutStream(PSEncode(Name) + "Bild cvx exec\n");
				PutStream(PSEncode(Name) + "Bild resetfile\n");
			}
			else
			{
				PutStream("%%BeginDocument: " + fi.fileName() + "\n");
				if (getDouble(tmp.mid(0, 4), true) == 0xC5D0D3C6)
				{
					const char* data = tmp.data();
					uint startPos = getDouble(tmp.mid(4, 4), false);
					uint length = getDouble(tmp.mid(8, 4), false);
					PutStream(data + startPos, length, false);
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

	ScImage image;
	image.imgInfo.valid = false;
	image.imgInfo.clipPath = "";
	image.imgInfo.PDSpathData.clear();
	image.imgInfo.layerInfo.clear();
	image.imgInfo.RequestProps = item->pixm.imgInfo.RequestProps;
	image.imgInfo.isRequest = item->pixm.imgInfo.isRequest;
	CMSettings cms(item->doc(), Prof, item->ImageIntent);
	cms.allowColorManagement(true);
	cms.setUseEmbeddedProfile(UseEmbedded);
	int resolution = 300;
	if (item->isLatexFrame())
		resolution = item->asLatexFrame()->realDpi();
	else if (item->pixm.imgInfo.type == ImageType7)
		resolution = 72;
	//	int resolution = (item->pixm.imgInfo.type == ImageType7) ? 72 : 300;
	if ( !image.loadPicture(fn, item->pixm.imgInfo.actualPageNumber, cms, ScImage::CMYKData, resolution, &dummy) )
	{
		PS_Error_ImageLoadFailure(fn);
		return false;
	}
	image.applyEffect(item->effectsInUse, colorsToUse, true);
	int w = image.width();
	int h = image.height();
	PutStream(ToStr(x*scalex) + " " + ToStr(y*scaley) + " tr\n");
	PutStream("0 " + ToStr(h*scaley) + " tr\n");
	PutStream(ToStr(-item->imageRotation()) + " ro\n");
	PutStream("0 " + ToStr(-h*scaley) + " tr\n");
	if ((extensionIndicatesPDF(ext)) && (!item->isLatexFrame()))
	{
		scalex *= PrefsManager::instance().appPrefs.extToolPrefs.gs_Resolution / 300.0;
		scaley *= PrefsManager::instance().appPrefs.extToolPrefs.gs_Resolution / 300.0;
	}
	//	PutStream(ToStr(x*scalex) + " " + ToStr(y*scaley) + " tr\n");
	PutStream(ToStr(qRound(scalex*w)) + " " + ToStr(qRound(scaley*h)) + " sc\n");
	PutStream(((!DoSep) && (!GraySc)) ? "/DeviceCMYK setcolorspace\n" : "/DeviceGray setcolorspace\n");
	QByteArray maskArray;
	ScImage img2;
	img2.imgInfo.clipPath = "";
	img2.imgInfo.PDSpathData.clear();
	img2.imgInfo.layerInfo.clear();
	img2.imgInfo.RequestProps = item->pixm.imgInfo.RequestProps;
	img2.imgInfo.isRequest = item->pixm.imgInfo.isRequest;
	if (item->pixm.imgInfo.type != ImageType7)
	{
		bool alphaLoaded = img2.getAlpha(fn, item->pixm.imgInfo.actualPageNumber, maskArray, false, true, resolution);
		if (!alphaLoaded)
		{
			PS_Error_MaskLoadFailure(fn);
			return false;
		}
	}
	if ((maskArray.size() > 0) && (item->pixm.imgInfo.type != ImageType7))
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
			PutStream("      /DataSource " + PSEncode(Name) + "Bild\n");
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
			PutStream(PSEncode(Name) + "Bild resetfile\n");
			//PutStream(PSEncode(Name) + "Mask resetfile\n");
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
			PutStream("   /DataSource " + PSEncode(Name) + "Bild >>\n");
			PutStream("image\n");
			PutStream(PSEncode(Name) + "Bild resetfile\n");
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
	return true;
}


void PSLib::PS_plate(int nr, const QString& name)
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
			PutStream("%%PlateColor " + name + "\n");
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

void PSLib::PDF_Bookmark(const QString& text, uint Seite)
{
	PutStream("[/Title (" + text + ") /Page " + IToStr(Seite) + " /View [/Fit]\n");
	PutStream("/OUT pdfmark\n");
}

void PSLib::PDF_Annotation(PageItem *item, const QString& text, double x, double y, double b, double h)
{
	PutStream("[\n/Rect [ " + ToStr(x) + " " + ToStr(y)  + " " + ToStr(b) + " " + ToStr(h) + " ]\n");
	switch (item->annotation().Type())
	{
		case 0:
		case Annotation::Text:
			PutStream("/Subtype /Text\n");
			PutStream("/Contents (" + text + ")\n/Open false\n");
			break;
		case 1:
		case Annotation::Link:
			PutStream("/Subtype /Link\n");
			if (item->annotation().ActionType() == Annotation::Action_GoTo)
			{
				PutStream("/Page " + QString::number(item->annotation().Ziel() + 1) + "\n");
				PutStream("/View [ /XYZ " + item->annotation().Action() + "]\n");
			}
			if (item->annotation().ActionType() == Annotation::Action_GoToR_FileRel)
			{
				QFileInfo fiBase(Spool.fileName());
				QString baseDir = fiBase.absolutePath();
				PutStream("/Action /GoToR\n");
				PutStream("/File (" + Path2Relative(item->annotation().Extern(), baseDir) + ")\n");
				PutStream("/Page " + QString::number(item->annotation().Ziel() + 1) + "\n");
				PutStream("/View [ /XYZ " + item->annotation().Action() + "]\n");
			}
			if (item->annotation().ActionType() == Annotation::Action_URI)
			{
			/* The PDFMark docs say that for URI actions should contain an entry /Subtype /URI
			   but tests with Ghostscript shows that only /S /URI works. Don't know if that is
			   an error in the docs or a bug in Ghostscript
				PutStream("/Action << /Subtype /URI /URI (" + item->annotation().Extern() + ") >>\n");
			*/
				PutStream("/Action << /S /URI /URI (" + item->annotation().Extern() + ") >>\n");
			}
			if (item->annotation().ActionType() == Annotation::Action_GoToR_FileAbs)
			{
				PutStream("/Action /GoToR\n");
				PutStream("/File (" + item->annotation().Extern() + ")\n");
				PutStream("/Page " + QString::number(item->annotation().Ziel() + 1) + "\n");
				PutStream("/View [ /XYZ " + item->annotation().Action() + "]\n");
			}
			break;
		default:
			break;
	}
	if ((item->annotation().Type() < 2) || (item->annotation().Type() > 9))
		PutStream("/Border [ 0 0 0 ]\n");
	PutStream("/Title (" + sanitizeItemName(item->itemName()) + ")\n");
	PutStream("/ANN pdfmark\n");
}


void PSLib::PS_close()
{
	PutStream("%%Trailer\n");
//	PutStream("end\n");
	PutStream("%%EOF\n");
	Spool.close();
}

void PSLib::PS_insert(const QString& i)
{
	PutStream(i);
}

void PSLib::PS_Error(const QString& message)
{
	ErrorMessage = message;
	if (!ScCore->usingGUI())
		qDebug("%s", message.toLocal8Bit().data());
}

void PSLib::PS_Error_ImageDataWriteFailure()
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

void PSLib::PS_Error_InsufficientMemory()
{
	PS_Error( tr("Insufficient memory for processing an image"));
}

int PSLib::createPS(const QString& outputFileName)
{
	bool errorOccured = false;

	if (!PS_set_file(outputFileName))
		return 1;

	std::vector<int> &pageNs = Options.pageNumbers;
	bool outputSep = Options.outputSeparations;
	QString separationName = Options.separationName;
	QStringList separations = Options.allSeparations;
	bool Hm = Options.mirrorH;
	bool Vm = Options.mirrorV;
	bool doClip = Options.doClip;
	int sepac;
	int pagemult;
	if (outputSep && (separationName == "All"))
		pagemult = separations.count();
	else
		pagemult = 1;
//	QVector<double> dum;
	double gx = 0.0;
	double gy = 0.0;
	double gw = 0.0;
	double gh = 0.0;
	PS_set_Info("Author", m_Doc->documentInfo().author());
	PS_set_Info("Title", m_Doc->documentInfo().title());
	if (!Options.useColor)
		PS_setGray();
	if ((m_Doc->HasCMS) && (ScCore->haveCMS()))
		solidTransform = m_Doc->colorEngine.createTransform(m_Doc->DocInputCMYKProf, Format_CMYK_16, m_Doc->DocPrinterProf, Format_CMYK_16, m_Doc->IntentColors, 0);
	else
		solidTransform = ScColorTransform();
	if (ScCore->usingGUI())
	{
		QString title = QObject::tr("Exporting PostScript File");
		if (m_outputFormat == OutputPS)
			title = QObject::tr("Printing File");
		progressDialog = new MultiProgressDialog(title, CommonStrings::tr_Cancel, m_Doc->scMW());
		if (progressDialog)
		{
			QStringList barNames, barTexts;
			barNames << "EMP" << "EP";
			barTexts << tr("Processing Master Page:") << tr("Exporting Page:");
			QList<bool> barsNumeric;
			barsNumeric << true << true;
			progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
			progressDialog->setOverallTotalSteps(pageNs.size()+m_Doc->MasterPages.count());
			progressDialog->setTotalSteps("EMP", m_Doc->MasterPages.count());
			progressDialog->setTotalSteps("EP", pageNs.size());
			progressDialog->setOverallProgress(0);
			progressDialog->setProgress("EMP", 0);
			progressDialog->setProgress("EP", 0);
			progressDialog->show();
			connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
			QCoreApplication::processEvents();
		}
	}
	uint docSelectionCount = m_Doc->m_Selection->count();
	if ((m_outputFormat == OutputEPS) && (docSelectionCount != 0))
	{
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		for (uint ep = 0; ep < docSelectionCount; ++ep)
		{
			const PageItem* currItem = m_Doc->m_Selection->itemAt(ep);
			double lw = currItem->lineWidth() / 2.0;
			if (currItem->rotation() != 0)
			{
				FPointArray pb;
				pb.addPoint(FPoint(currItem->xPos() - lw, currItem->yPos() - lw));
				pb.addPoint(FPoint(currItem->width() + lw * 2.0, -lw, currItem->xPos() - lw, currItem->yPos() - lw, currItem->rotation(), 1.0, 1.0));
				pb.addPoint(FPoint(currItem->width() + lw * 2.0, currItem->height() + lw * 2.0, currItem->xPos() - lw, currItem->yPos() - lw, currItem->rotation(), 1.0, 1.0));
				pb.addPoint(FPoint(-lw, currItem->height() + lw * 2.0, currItem->xPos() - lw, currItem->yPos() - lw, currItem->rotation(), 1.0, 1.0));
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
				minx = qMin(minx, currItem->xPos() - lw);
				miny = qMin(miny, currItem->yPos() - lw);
				maxx = qMax(maxx, currItem->xPos() - lw + currItem->width() + lw * 2.0);
				maxy = qMax(maxy, currItem->yPos() - lw + currItem->height()+ lw * 2.0);
			}
		}
		gx = minx;
		gy = miny;
		gw = maxx - minx;
		gh = maxy - miny;
		int pgNum = pageNs[0] - 1;
		gx -= m_Doc->Pages->at(pgNum)->xOffset();
		gy -= m_Doc->Pages->at(pgNum)->yOffset();
		errorOccured = !PS_begin_doc(gx, m_Doc->pageHeight() - (gy + gh), gx + gw, m_Doc->pageHeight() - gy, 1 * pagemult);
	}
	else
	{
		double maxWidth = 0.0;
		double maxHeight = 0.0;
		for (size_t i = 0; i < pageNs.size(); ++i)
		{
			int pgNum = pageNs[i] - 1;
			maxWidth = qMax(m_Doc->Pages->at(pgNum)->width(), maxWidth);
			maxHeight = qMax(m_Doc->Pages->at(pgNum)->height(), maxHeight);
		}
		errorOccured = !PS_begin_doc(0.0, 0.0, maxWidth, maxHeight, pageNs.size() * pagemult);
	}
	
	sepac = 0;
	uint aa = 0;
	uint a;
	while (aa < pageNs.size() && !abortExport && !errorOccured)
	{
		if (progressDialog)
		{
			progressDialog->setProgress("EP", aa);
			progressDialog->setOverallProgress(aa + m_Doc->MasterPages.count());
			ScQApp->processEvents();
		}
		a = pageNs[aa]-1;
		ScPage* page = m_Doc->Pages->at(a);
		if ((m_outputFormat == OutputEPS) && (m_Doc->m_Selection->count() != 0))
		{
			MarginStruct Ma;
			Ma.setLeft(gx);
			Ma.setTop(gy);
			Ma.setBottom(page->height() - (gy + gh));
			Ma.setRight(page->width() - (gx + gw));
			PS_begin_page(page, &Ma, true);
		}
		else
			PS_begin_page(page, &page->Margins, doClip);
		if (Hm)
		{
			PS_translate(page->width(), 0);
			PS_scale(-1, 1);
		}
		if (Vm)
		{
			PS_translate(0, page->height());
			PS_scale(1, -1);
		}
		if (outputSep)
		{
			if (separationName == "Black")
				PS_plate(0);
			else if (separationName == "Cyan")
				PS_plate(1);
			else if (separationName == "Magenta")
				PS_plate(2);
			else if (separationName == "Yellow")
				PS_plate(3);
			else if (separationName == "All")
				PS_plate(sepac, separations[sepac]);
			else
				PS_plate(4, separationName);
		}
		ScLayer ll;
		ll.isPrintable = false;
		for (int lam = 0; lam < m_Doc->Layers.count() && !abortExport && !errorOccured; ++lam)
		{
			m_Doc->Layers.levelToLayer(ll, lam);
			if (!ll.isPrintable)
				continue;
			if (!page->masterPageNameEmpty() && !abortExport && !errorOccured)
			{
				errorOccured |= !ProcessMasterPageLayer(page, ll, a + 1);
			}
			if (!abortExport && !errorOccured)
			{
				errorOccured |= !ProcessPageLayer(page, ll, a + 1);
			}
		}
		if (!abortExport && !errorOccured)
			PS_end_page();
		if (outputSep)
		{
			if (separationName != "All")
				aa++;
			else
			{
				if (sepac == (separations.count() - 1))
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
	if (progressDialog)
		progressDialog->close();
	if (errorOccured)
		return 1;
	if (abortExport)
		return 2; //CB Lets leave 1 for general error condition
	return 0;
}

bool PSLib::ProcessItem(ScPage* page, PageItem* item, uint PNr, bool master, bool embedded, bool useTemplate)
{
	double h, s, v, k;
	QVector<double> dum;

	if (!item->printEnabled())
		return true;

	fillRule = true;
	PS_save();
	if (item->doOverprint)
	{
		PutStream("true setoverprint\n");
		PutStream("true setoverprintmode\n");
	}
	if (item->fillColor() != CommonStrings::None)
	{
		SetColor(item->fillColor(), item->fillShade(), &h, &s, &v, &k);
		PS_setcmykcolor_fill(h, s, v, k);
	}
	if (item->lineColor() != CommonStrings::None)
	{
		SetColor(item->lineColor(), item->lineShade(), &h, &s, &v, &k);
		PS_setcmykcolor_stroke(h, s, v, k);
	}
	PS_setlinewidth(item->lineWidth());
	PS_setcapjoin(item->PLineEnd, item->PLineJoin);
	PS_setdash(item->PLineArt, item->DashOffset, item->DashValues);
	if (!embedded)
	{
		PS_translate(item->xPos() - page->xOffset(), page->height() - (item->yPos() - page->yOffset()));
	}
	if (item->rotation() != 0)
		PS_rotate(-item->rotation());
	switch (item->itemType())
	{
	case PageItem::ImageFrame:
	case PageItem::LatexFrame:
		if (master)
			break;
		if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
		{
			SetClipPath(item->PoLine);
			PS_closepath();
			if (item->GrType == Gradient_Hatch)
				PS_HatchFill(item);
			else if ((item->GrType != 0) && (!master))
				HandleGradientFillStroke(item, false);
			else
				putColor(item->fillColor(), item->fillShade(), true);
			PS_newpath();
		}
		PS_save();
		SetPathAndClip(item->PoLine, true);
		if (item->imageFlippedH())
		{
			PS_translate(item->width(), 0);
			PS_scale(-1, 1);
		}
		if (item->imageFlippedV())
		{
			PS_translate(0, -item->height());
			PS_scale(1, -1);
		}
		if (!item->imageClip.empty())
			SetPathAndClip(item->imageClip, true);
		if ((item->imageIsAvailable) && (!item->Pfile.isEmpty()))
		{
			bool imageOk = false;
			PS_translate(0, -item->BBoxH * item->imageYScale());
			if ((m_optimization == OptimizeSize) && (((!page->pageNameEmpty()) && !Options.outputSeparations && Options.useColor) || useTemplate))
				imageOk = PS_image(item, item->imageXOffset(), -item->imageYOffset(), item->Pfile, item->imageXScale(), item->imageYScale(), item->ImageProfile, item->UseEmbedded, item->itemName());
			else
				imageOk = PS_image(item, item->imageXOffset(), -item->imageYOffset(), item->Pfile, item->imageXScale(), item->imageYScale(), item->ImageProfile, item->UseEmbedded);
			if (!imageOk) return false;
		}
		PS_restore();
		if ((item->lineColor() != CommonStrings::None) || (!item->NamedLStyle.isEmpty()) || (!item->strokePattern().isEmpty()) || (item->GrTypeStroke > 0))
		{
			if (item->NamedLStyle.isEmpty()) // && (item->lineWidth() != 0.0))
			{
				const ScPattern* strokePattern = m_Doc->checkedPattern(item->strokePattern());
				if (strokePattern && item->patternStrokePath)
				{
					QPainterPath path = item->PoLine.toQPainterPath(false);
					HandleBrushPattern(item, path, page, PNr, master);
				}
				else
				{
					PS_setlinewidth(item->lineWidth());
					PS_setcapjoin(item->PLineEnd, item->PLineJoin);
					PS_setdash(item->PLineArt, item->DashOffset, item->DashValues);
					SetClipPath(item->PoLine);
					PS_closepath();
					if (strokePattern)
						HandleStrokePattern(item);
					else if (item->GrTypeStroke > 0)
						HandleGradientFillStroke(item);
					else if (item->lineColor() != CommonStrings::None)
					{
						SetColor(item->lineColor(), item->lineShade(), &h, &s, &v, &k);
						PS_setcmykcolor_stroke(h, s, v, k);
						putColor(item->lineColor(), item->lineShade(), false);
					}
				}
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				for (int it = ml.size() - 1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) // && (ml[it].Width != 0))
					{
						SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
						PS_setcmykcolor_stroke(h, s, v, k);
						PS_setlinewidth(ml[it].Width);
						PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
						SetClipPath(item->PoLine);
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
		if ((item->isBookmark || item->isAnnotation()) && (!Options.includePDFMarks))
			break;
		if (item->isBookmark)
		{
			QString bm;
			QString cc;
			for (int d = 0; d < item->itemText.length(); ++d)
			{
				if ((item->itemText.text(d) == QChar(13)) || (item->itemText.text(d) == QChar(10)) || (item->itemText.text(d) == QChar(28)))
					break;
				bm += "\\" + cc.setNum(qMax(item->itemText.text(d).unicode(), (ushort) 32), 8);
			}
			PDF_Bookmark(bm, page->pageNr()+1);
		}
		if (item->isAnnotation())
		{
			if ((item->annotation().Type() == 0) || (item->annotation().Type() == 1) || (item->annotation().Type() == Annotation::Text) || (item->annotation().Type() == Annotation::Link))
			{
				QString bm;
				QString cc;
				for (int d = 0; d < item->itemText.length(); ++d)
				{
					bm += "\\" + cc.setNum(qMax(item->itemText.text(d).unicode(), (ushort) 32), 8);
				}
				PDF_Annotation(item, bm, 0, 0, item->width(), -item->height());
			}
			break;
		}
		if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
		{
			SetClipPath(item->PoLine);
			PS_closepath();
			if (item->GrType == Gradient_Hatch)
				PS_HatchFill(item);
			else if ((item->GrType != 0) && (!master))
				HandleGradientFillStroke(item, false);
			else
				putColor(item->fillColor(), item->fillShade(), true);
		}
		if (item->imageFlippedH())
		{
			PS_translate(item->width(), 0);
			PS_scale(-1, 1);
		}
		if (item->imageFlippedV())
		{
			PS_translate(0, -item->height());
			PS_scale(1, -1);
		}
		if (item->itemText.length() != 0)
			setTextSt(item, PNr - 1, page, master);
		if ((item->lineColor() != CommonStrings::None) || (!item->NamedLStyle.isEmpty()) || (!item->strokePattern().isEmpty()) || (item->GrTypeStroke > 0))
		{
			PS_setlinewidth(item->lineWidth());
			PS_setcapjoin(item->PLineEnd, item->PLineJoin);
			PS_setdash(item->PLineArt, item->DashOffset, item->DashValues);
			if (item->NamedLStyle.isEmpty()) // && (item->lineWidth() != 0.0))
			{
				const ScPattern* strokePattern = m_Doc->checkedPattern(item->strokePattern());
				if (strokePattern && item->patternStrokePath)
				{
					QPainterPath path = item->PoLine.toQPainterPath(false);
					HandleBrushPattern(item, path, page, PNr, master);
				}
				else
				{
					SetClipPath(item->PoLine);
					PS_closepath();
					if (strokePattern)
						HandleStrokePattern(item);
					else if (item->GrTypeStroke > 0)
						HandleGradientFillStroke(item);
					else if (item->lineColor() != CommonStrings::None)
					{
						SetColor(item->lineColor(), item->lineShade(), &h, &s, &v, &k);
						PS_setcmykcolor_stroke(h, s, v, k);
						putColor(item->lineColor(), item->lineShade(), false);
					}
				}
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				for (int it = ml.size() - 1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
					{
						SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
						PS_setcmykcolor_stroke(h, s, v, k);
						PS_setlinewidth(ml[it].Width);
						PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
						SetClipPath(item->PoLine);
						PS_closepath();
						putColor(ml[it].Color, ml[it].Shade, false);
					}
				}
			}
		}
		break;
	case PageItem::Line:
		if (item->NamedLStyle.isEmpty()) // && (item->lineWidth() != 0.0))
		{
			const ScPattern* strokePattern = m_Doc->checkedPattern(item->strokePattern());
			if (strokePattern)
			{
				if (item->patternStrokePath)
				{
					QPainterPath guidePath;
					guidePath.moveTo(0, 0);
					guidePath.lineTo(item->width(), 0);
					HandleBrushPattern(item, guidePath, page, PNr, master);
				}
				else
				{
					PS_moveto(0, 0);
					PS_lineto(item->width(), 0);
					HandleStrokePattern(item);
				}
			}
			else if (item->GrTypeStroke > 0)
			{
				PS_moveto(0, 0);
				PS_lineto(item->width(), 0);
				HandleGradientFillStroke(item);
			}
			else if (item->lineColor() != CommonStrings::None)
			{
				PS_moveto(0, 0);
				PS_lineto(item->width(), 0);
				putColor(item->lineColor(), item->lineShade(), false);
			}
		}
		else
		{
			multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
			for (int it = ml.size() - 1; it > -1; it--)
			{
				if (ml[it].Color == CommonStrings::None)
					continue;
				SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
				PS_setcmykcolor_stroke(h, s, v, k);
				PS_setlinewidth(ml[it].Width);
				PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
				PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
				PS_moveto(0, 0);
				PS_lineto(item->width(), 0);
				putColor(ml[it].Color, ml[it].Shade, false);
			}
		}
		if (item->startArrowIndex() != 0)
		{
			QTransform arrowTrans;
			arrowTrans.scale(-1,1);
			arrowTrans.scale(item->startArrowScale() / 100.0, item->startArrowScale() / 100.0);
			drawArrow(item, arrowTrans, item->startArrowIndex());
		}
		if (item->endArrowIndex() != 0)
		{
			QTransform arrowTrans;
			arrowTrans.translate(item->width(), 0);
			arrowTrans.scale(item->endArrowScale() / 100.0, item->endArrowScale() / 100.0);
			drawArrow(item, arrowTrans, item->endArrowIndex());
		}
		break;
	/* OBSOLETE CR 2005-02-06
	case 1:
	case 3:
	*/
	case PageItem::ItemType1:
	case PageItem::ItemType3:
	case PageItem::Polygon:
	case PageItem::RegularPolygon:
	case PageItem::Arc:
		if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
		{
			SetClipPath(item->PoLine);
			PS_closepath();
			fillRule = item->fillRule;
			if (item->GrType == Gradient_Hatch)
				PS_HatchFill(item);
			else if (item->GrType != 0)
				HandleGradientFillStroke(item, false);
			else
				putColor(item->fillColor(), item->fillShade(), true);
		}
		if ((item->lineColor() != CommonStrings::None) || (!item->NamedLStyle.isEmpty()) || (!item->strokePattern().isEmpty()) || (item->GrTypeStroke > 0))
		{
			if (item->NamedLStyle.isEmpty()) //&& (item->lineWidth() != 0.0))
			{
				const ScPattern* strokePattern = m_Doc->checkedPattern(item->strokePattern());
				if (strokePattern && (item->patternStrokePath))
				{
					QPainterPath path = item->PoLine.toQPainterPath(false);
					HandleBrushPattern(item, path, page, PNr, master);
				}
				else
				{
					SetClipPath(item->PoLine);
					PS_closepath();
					if (strokePattern)
						HandleStrokePattern(item);
					else if (item->GrTypeStroke > 0)
						HandleGradientFillStroke(item);
					else if (item->lineColor() != CommonStrings::None)
						putColor(item->lineColor(), item->lineShade(), false);
				}
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				for (int it = ml.size() - 1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
					{
						SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
						PS_setcmykcolor_stroke(h, s, v, k);
						PS_setlinewidth(ml[it].Width);
						PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
						SetClipPath(item->PoLine);
						PS_closepath();
						putColor(ml[it].Color, ml[it].Shade, false);
					}
				}
			}
		}
		break;
	case PageItem::PolyLine:
	case PageItem::Spiral:
		if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
		{
			SetClipPath(item->PoLine);
			PS_closepath();
			fillRule = item->fillRule;
			if (item->GrType == Gradient_Hatch)
				PS_HatchFill(item);
			else if (item->GrType != 0)
				HandleGradientFillStroke(item, false);
			else
				putColor(item->fillColor(), item->fillShade(), true);
			PS_newpath();
		}
		if ((item->lineColor() != CommonStrings::None) || (!item->NamedLStyle.isEmpty()) || (!item->strokePattern().isEmpty()) || (item->GrTypeStroke > 0))
		{
			if (item->NamedLStyle.isEmpty()) //&& (item->lineWidth() != 0.0))
			{
				const ScPattern* strokePattern = m_Doc->checkedPattern(item->strokePattern());
				if (strokePattern && (item->patternStrokePath))
				{
					QPainterPath path = item->PoLine.toQPainterPath(false);
					HandleBrushPattern(item, path, page, PNr, master);
				}
				else
				{
					SetClipPath(item->PoLine, false);
					if (strokePattern)
						HandleStrokePattern(item);
					else if (item->GrTypeStroke > 0)
						HandleGradientFillStroke(item);
					else if (item->lineColor() != CommonStrings::None)
						putColor(item->lineColor(), item->lineShade(), false);
				}
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				for (int it = ml.size() - 1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
					{
						SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
						PS_setcmykcolor_stroke(h, s, v, k);
						PS_setlinewidth(ml[it].Width);
						PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
						SetClipPath(item->PoLine, false);
						putColor(ml[it].Color, ml[it].Shade, false);
					}
				}
			}
		}
		if (item->startArrowIndex() != 0)
		{
			FPoint Start = item->PoLine.point(0);
			for (int xx = 1; xx < item->PoLine.size(); xx += 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y() - Vector.y(), Start.x() - Vector.x()) * (180.0 / M_PI);
					QTransform arrowTrans;
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(item->startArrowScale() / 100.0, item->startArrowScale() / 100.0);
					drawArrow(item, arrowTrans, item->startArrowIndex());
					break;
				}
			}
		}
		if (item->endArrowIndex() != 0)
		{
			FPoint End = item->PoLine.point(item->PoLine.size() - 2);
			for (uint xx = item->PoLine.size() - 1; xx > 0; xx -= 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y() - Vector.y(),End.x() - Vector.x())*(180.0/M_PI);
					QTransform arrowTrans;
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(item->endArrowScale() / 100.0, item->endArrowScale() / 100.0);
					drawArrow(item, arrowTrans, item->endArrowIndex());
					break;
				}
			}
		}
		break;
	case PageItem::PathText:
		if (master)
			break;
		if (item->PoShow && (item->PoLine.size() > 3))
		{
			PS_save();
			if (item->NamedLStyle.isEmpty()) //&& (item->lineWidth() != 0.0))
			{
				const ScPattern* strokePattern = m_Doc->checkedPattern(item->strokePattern());
				if (strokePattern && (item->patternStrokePath))
				{
					QPainterPath path = item->PoLine.toQPainterPath(false);
					HandleBrushPattern(item, path, page, PNr, master);
				}
				else
				{
					SetClipPath(item->PoLine, false);
					if (strokePattern)
						HandleStrokePattern(item);
					else if (item->GrTypeStroke > 0)
						HandleGradientFillStroke(item);
					else if (item->lineColor() != CommonStrings::None)
						putColor(item->lineColor(), item->lineShade(), false);
				}
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				for (int it = ml.size() - 1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
					{
						SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
						PS_setcmykcolor_stroke(h, s, v, k);
						PS_setlinewidth(ml[it].Width);
						PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
						SetClipPath(item->PoLine, false);
						putColor(ml[it].Color, ml[it].Shade, false);
					}
				}
			}
			PS_restore();
		}
		if (item->itemText.length() != 0)
			setTextSt(item, PNr-1, page, master);
		break;
	case PageItem::Symbol:
		if (m_Doc->docPatterns.contains(item->pattern()))
		{
			ScPattern pat = m_Doc->docPatterns[item->pattern()];
			PS_save();
			SetPathAndClip(item->PoLine, item->fillRule);
			if (item->imageFlippedH())
			{
				PS_translate(item->width(), 0);
				PS_scale(-1, 1);
			}
			if (item->imageFlippedV())
			{
				PS_translate(0, -item->height());
				PS_scale(1, -1);
			}
			PS_scale(item->width() / pat.width, item->height() / pat.height);
			PS_translate(0, -item->height());
//			PS_translate(pat.items.at(0)->gXpos, -pat.items.at(0)->gYpos);
			for (int em = 0; em < pat.items.count(); ++em)
			{
				PageItem* embed = pat.items.at(em);
				PS_save();
				PS_translate(embed->gXpos, item->height() - embed->gYpos);
				ProcessItem(page, embed, PNr, master, true, useTemplate);
				PS_restore();
			}
			PS_restore();
		}
		break;
	case PageItem::Group:
		if (master)
			break;
		PS_save();
		if (item->groupClipping())
			SetPathAndClip(item->PoLine, item->fillRule);
		if (item->imageFlippedH())
		{
			PS_translate(item->width(), 0);
			PS_scale(-1, 1);
		}
		if (item->imageFlippedV())
		{
			PS_translate(0, -item->height());
			PS_scale(1, -1);
		}
		PS_scale(item->width() / item->groupWidth, item->height() / item->groupHeight);
		PS_translate(0, -item->height());
		for (int em = 0; em < item->groupItemList.count(); ++em)
		{
			PageItem* embed = item->groupItemList.at(em);
			PS_save();
			PS_translate(embed->gXpos, item->height() - embed->gYpos);
			ProcessItem(page, embed, PNr, master, true, useTemplate);
			PS_restore();
		}
		PS_restore();
		break;
	case PageItem::Table:
		if (master)
			break;
		PS_save();
		PS_translate(item->asTable()->gridOffset().x(), -item->asTable()->gridOffset().y());
		// Paint table fill.
		if (item->asTable()->fillColor() != CommonStrings::None)
		{
			int lastCol = item->asTable()->columns() - 1;
			int lastRow = item->asTable()->rows() - 1;
			double x = item->asTable()->columnPosition(0);
			double y = item->asTable()->rowPosition(0);
			double width = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol) - x;
			double height = item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow) - y;
			putColorNoDraw(item->asTable()->fillColor(), item->asTable()->fillShade());
			PutStream("0 0 " + ToStr(width) + " " + ToStr(-height) + " rectfill\n");
		}
		// Pass 1: Paint cell fills.
		for (int row = 0; row < item->asTable()->rows(); ++row)
		{
			int colSpan = 0;
			for (int col = 0; col < item->asTable()->columns(); col += colSpan)
			{
				TableCell cell = item->asTable()->cellAt(row, col);
				if (row == cell.row())
				{
					QString colorName = cell.fillColor();
					if (colorName != CommonStrings::None)
					{
						PS_save();
						putColorNoDraw(colorName, cell.fillShade());
						int row = cell.row();
						int col = cell.column();
						int lastRow = row + cell.rowSpan() - 1;
						int lastCol = col + cell.columnSpan() - 1;
						double x = item->asTable()->columnPosition(col);
						double y = item->asTable()->rowPosition(row);
						double width = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol) - x;
						double height = item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow) - y;
						PutStream(ToStr(x) + " " + ToStr(-y) + " " + ToStr(width) + " " + ToStr(-height) + " rectfill\n");
						PS_restore();
					}
				}
				colSpan = cell.columnSpan();
			}
		}
		// Pass 2: Paint vertical borders.
		for (int row = 0; row < item->asTable()->rows(); ++row)
		{
			int colSpan = 0;
			for (int col = 0; col < item->asTable()->columns(); col += colSpan)
			{
				TableCell cell = item->asTable()->cellAt(row, col);
				if (row == cell.row())
				{
					const int lastRow = cell.row() + cell.rowSpan() - 1;
					const int lastCol = cell.column() + cell.columnSpan() - 1;
					const double borderX = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol);
					QPointF start(borderX, 0.0);
					QPointF end(borderX, 0.0);
					QPointF startOffsetFactors, endOffsetFactors;
					int startRow, endRow;
					for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
					{
						TableCell rightCell = item->asTable()->cellAt(row, lastCol + 1);
						startRow = qMax(cell.row(), rightCell.row());
						endRow = qMin(lastRow, rightCell.isValid() ? rightCell.row() + rightCell.rowSpan() - 1 : lastRow);
						TableCell topLeftCell = item->asTable()->cellAt(startRow - 1, lastCol);
						TableCell topRightCell = item->asTable()->cellAt(startRow - 1, lastCol + 1);
						TableCell bottomRightCell = item->asTable()->cellAt(endRow + 1, lastCol + 1);
						TableCell bottomLeftCell = item->asTable()->cellAt(endRow + 1, lastCol);
						TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
						resolveBordersVertical(topLeftCell, topRightCell, cell, rightCell, bottomLeftCell, bottomRightCell,
							&topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, item->asTable());
						if (border.isNull())
							continue; // Quit early if the border to paint is null.
						start.setY(item->asTable()->rowPosition(startRow));
						end.setY(item->asTable()->rowPosition(endRow) + item->asTable()->rowHeight(endRow));
						joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
						paintBorder(border, start, end, startOffsetFactors, endOffsetFactors);
					}
					if (col == 0)
					{
						const int lastRow = cell.row() + cell.rowSpan() - 1;
						const int firstCol = cell.column();
						const double borderX = item->asTable()->columnPosition(firstCol);
						QPointF start(borderX, 0.0);
						QPointF end(borderX, 0.0);
						QPointF startOffsetFactors, endOffsetFactors;
						int startRow, endRow;
						for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
						{
							TableCell leftCell = item->asTable()->cellAt(row, firstCol - 1);
							startRow = qMax(cell.row(), leftCell.row());
							endRow = qMin(lastRow, leftCell.isValid() ? leftCell.row() + leftCell.rowSpan() - 1 : lastRow);
							TableCell topLeftCell = item->asTable()->cellAt(startRow - 1, firstCol - 1);
							TableCell topRightCell = item->asTable()->cellAt(startRow - 1, firstCol);
							TableCell bottomRightCell = item->asTable()->cellAt(lastRow + 1, firstCol);
							TableCell bottomLeftCell = item->asTable()->cellAt(lastRow + 1, firstCol - 1);
							TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
							resolveBordersVertical(topLeftCell, topRightCell, leftCell, cell, bottomLeftCell, bottomRightCell,
								&topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, item->asTable());
							if (border.isNull())
								continue; // Quit early if the border to paint is null.
							start.setY(item->asTable()->rowPosition(startRow));
							end.setY(item->asTable()->rowPosition(endRow) + item->asTable()->rowHeight(endRow));
							joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
							paintBorder(border, start, end, startOffsetFactors, endOffsetFactors);
						}
					}
				}
				colSpan = cell.columnSpan();
			}
		}
		// Pass 3: Paint horizontal borders.
		for (int row = 0; row < item->asTable()->rows(); ++row)
		{
			int colSpan = 0;
			for (int col = 0; col < item->asTable()->columns(); col += colSpan)
			{
				TableCell cell = item->asTable()->cellAt(row, col);
				if (row == cell.row())
				{
					const int lastRow = cell.row() + cell.rowSpan() - 1;
					const int lastCol = cell.column() + cell.columnSpan() - 1;
					const double borderY = (item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow));
					QPointF start(0.0, borderY);
					QPointF end(0.0, borderY);
					QPointF startOffsetFactors, endOffsetFactors;
					int startCol, endCol;
					for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
					{
						TableCell bottomCell = item->asTable()->cellAt(lastRow + 1, col);
						startCol = qMax(cell.column(), bottomCell.column());
						endCol = qMin(lastCol, bottomCell.isValid() ? bottomCell.column() + bottomCell.columnSpan() - 1 : lastCol);
						TableCell topLeftCell = item->asTable()->cellAt(lastRow, startCol - 1);
						TableCell topRightCell = item->asTable()->cellAt(lastRow, endCol + 1);
						TableCell bottomRightCell = item->asTable()->cellAt(lastRow + 1, endCol + 1);
						TableCell bottomLeftCell = item->asTable()->cellAt(lastRow + 1, startCol - 1);
						TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
						resolveBordersHorizontal(topLeftCell, cell, topRightCell, bottomLeftCell, bottomCell,
											bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, item->asTable());
						if (border.isNull())
							continue; // Quit early if the border is null.
						start.setX(item->asTable()->columnPosition(startCol));
						end.setX(item->asTable()->columnPosition(endCol) + item->asTable()->columnWidth(endCol));
						joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
						paintBorder(border, start, end, startOffsetFactors, endOffsetFactors);
					}
					if (row == 0)
					{
						const int firstRow = cell.row();
						const int lastCol = cell.column() + cell.columnSpan() - 1;
						const double borderY = item->asTable()->rowPosition(firstRow);
						QPointF start(0.0, borderY);
						QPointF end(0.0, borderY);
						QPointF startOffsetFactors, endOffsetFactors;
						int startCol, endCol;
						for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
						{
							TableCell topCell = item->asTable()->cellAt(firstRow - 1, col);
							startCol = qMax(cell.column(), topCell.column());
							endCol = qMin(lastCol, topCell.isValid() ? topCell.column() + topCell.columnSpan() - 1 : lastCol);
							TableCell topLeftCell = item->asTable()->cellAt(firstRow - 1, startCol - 1);
							TableCell topRightCell = item->asTable()->cellAt(firstRow - 1, endCol + 1);
							TableCell bottomRightCell = item->asTable()->cellAt(firstRow, endCol + 1);
							TableCell bottomLeftCell = item->asTable()->cellAt(firstRow, startCol - 1);
							TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
							resolveBordersHorizontal(topLeftCell, topCell, topRightCell, bottomLeftCell, cell,
														bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, item->asTable());
							if (border.isNull())
								continue; // Quit early if the border is null.
							start.setX(item->asTable()->columnPosition(startCol));
							end.setX(item->asTable()->columnPosition(endCol) + item->asTable()->columnWidth(endCol));
							joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
							paintBorder(border, start, end, startOffsetFactors, endOffsetFactors);
						}
					}
				}
				colSpan = cell.columnSpan();
			}
		}
		// Pass 4: Paint cell content.
		for (int row = 0; row < item->asTable()->rows(); ++row)
		{
			for (int col = 0; col < item->asTable()->columns(); col ++)
			{
				TableCell cell = item->asTable()->cellAt(row, col);
				if (cell.row() == row && cell.column() == col)
				{
					PageItem* textFrame = cell.textFrame();
					PS_save();
					PS_translate(cell.contentRect().x(), -cell.contentRect().y());
					ProcessItem(page, textFrame, PNr, master, true);
					PS_restore();
				}
			}
		}
		PS_restore();
		break;
	default:
		break;
	}
	PS_restore();

	return true;
}


void PSLib::paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end, const QPointF& startOffsetFactors, const QPointF& endOffsetFactors)
{
	PS_save();
	QPointF lineStart, lineEnd;
	QVector<double> DashValues;
	for (const TableBorderLine& line : border.borderLines())
	{
		if (line.color() == CommonStrings::None)
			continue;
		lineStart.setX(start.x() + line.width() * startOffsetFactors.x());
		lineStart.setY(start.y() + line.width() * startOffsetFactors.y());
		lineEnd.setX(end.x() + line.width() * endOffsetFactors.x());
		lineEnd.setY(end.y() + line.width() * endOffsetFactors.y());
		PS_moveto(lineStart.x(), -lineStart.y());
		PS_lineto(lineEnd.x(), -lineEnd.y());
		PS_setlinewidth(line.width());
		getDashArray(line.style(), qMax(line.width(), 1.0), DashValues);
		PS_setdash(line.style(), 0, DashValues);
		double h, s, v, k;
		SetColor(line.color(), line.shade(), &h, &s, &v, &k);
		PS_setcmykcolor_stroke(h, s, v, k);
		PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
		putColor(line.color(), line.shade(), false);
	}
	PS_restore();
}

void PSLib::ProcessPage(ScPage* page, uint PNr)
{
	PageItem *item;
	QList<PageItem*> pageItems = (page->pageNameEmpty()) ? m_Doc->DocItems : m_Doc->MasterItems;

	ScLayer ll;
	ll.isPrintable = false;
	ll.ID = 0;

	for (int layerLevel = 0; layerLevel < m_Doc->Layers.count(); ++layerLevel)
	{
		m_Doc->Layers.levelToLayer(ll, layerLevel);
		if (!ll.isPrintable)
			continue;
		for (int i = 0; i < pageItems.count() && !abortExport; ++i)
		{
			item = pageItems.at(i);
			if (progressDialog)
				ScQApp->processEvents();
			if (item->m_layerID != ll.ID)
				continue;
			if ((!page->pageNameEmpty()) && (item->isTextContainer()))
				continue;
			if ((!page->pageNameEmpty()) && (item->isImageFrame()) && ((Options.outputSeparations) || (!Options.useColor)))
				continue;
			if ((m_outputFormat == OutputEPS) && (!item->isSelected()) && (m_Doc->m_Selection->count() != 0))
				continue;
			double bLeft, bRight, bBottom, bTop;
			GetBleeds(page, bLeft, bRight, bBottom, bTop);
			double x1 = page->xOffset() - bLeft;
			double y1 = page->yOffset() - bTop;
			double w1 = page->width() + bLeft + bRight;
			double h1 = page->height() + bBottom + bTop;
			double lw = item->visualLineWidth();
			double x2 = item->BoundingX - lw / 2.0;
			double y2 = item->BoundingY - lw / 2.0;
			double w2 = qMax(item->BoundingW + lw, 1.0);
			double h2 = qMax(item->BoundingH + lw, 1.0);
			if (!QRectF(x2, y2, w2, h2).intersects(QRectF(x1, y1, w1, h1)))
				continue;
			if (item->ChangedMasterItem)
				continue;
			if ((!page->pageNameEmpty()) && (item->OwnPage != page->pageNr()) && (item->OwnPage != -1))
				continue;
			ProcessItem(page, item, PNr, false);
		}
	}
}

bool PSLib::ProcessMasterPageLayer(ScPage* page, ScLayer& layer, uint PNr)
{
	bool success = true;
	double h, s, v, k;
	QVector<double> dum;

	if (!layer.isPrintable)
		return true;

	ScPage* mPage = m_Doc->MasterPages.at(m_Doc->MasterNames[page->masterPageName()]);
	for (int am = 0; am < page->FromMaster.count() && !abortExport; ++am)
	{
		PageItem *ite = page->FromMaster.at(am);
		if (progressDialog)
			QCoreApplication::processEvents();
		if ((ite->m_layerID != layer.ID) || (!ite->printEnabled()))
			continue;
		if (!ite->isTextContainer())
		{
			int mpIndex = m_Doc->MasterNames[page->masterPageName()];
			PS_UseTemplate(QString("mp_obj_%1_%2").arg(mpIndex).arg(qHash(ite)));
		}
		else if (!ite->isTextFrame())
		{
			PS_save();
			// JG : replace what seems mostly duplicate code by corresponding function call (#3936)
			success &= ProcessItem(mPage, ite, PNr, false, false, true);
			PS_restore();
		}
		else // if (ite->isTextFrame())
		{
			PS_save();
			if (ite->doOverprint)
			{
				PutStream("true setoverprint\n");
				PutStream("true setoverprintmode\n");
			}
			if (ite->fillColor() != CommonStrings::None)
			{
				SetColor(ite->fillColor(), ite->fillShade(), &h, &s, &v, &k);
				PS_setcmykcolor_fill(h, s, v, k);
			}
			PS_translate(ite->xPos() - mPage->xOffset(), mPage->height() - (ite->yPos() - mPage->yOffset()));
			if (ite->rotation() != 0)
				PS_rotate(-ite->rotation());
			if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
			{
				SetClipPath(ite->PoLine);
				PS_closepath();
				if (ite->GrType == Gradient_Hatch)
					PS_HatchFill(ite);
				else if (ite->GrType != 0)
					HandleGradientFillStroke(ite, false);
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
			if (ite->itemText.length() != 0)
				setTextSt(ite, PNr - 1, mPage, true);
			if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
			{
				PS_setlinewidth(ite->lineWidth());
				PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
				PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
				if (ite->NamedLStyle.isEmpty()) // && (ite->lineWidth() != 0.0))
				{
					const ScPattern* strokePattern = m_Doc->checkedPattern(ite->strokePattern());
					if (strokePattern && (ite->patternStrokePath))
					{
						QPainterPath path = ite->PoLine.toQPainterPath(false);
						HandleBrushPattern(ite, path, page, PNr, true);
					}
					else
					{
						SetClipPath(ite->PoLine);
						PS_closepath();
						if (strokePattern)
							HandleStrokePattern(ite);
						else if (ite->GrTypeStroke > 0)
							HandleGradientFillStroke(ite);
						else if (ite->lineColor() != CommonStrings::None)
						{
							SetColor(ite->lineColor(), ite->lineShade(), &h, &s, &v, &k);
							PS_setcmykcolor_stroke(h, s, v, k);
							putColor(ite->lineColor(), ite->lineShade(), false);
						}
					}
				}
				else
				{
					multiLine ml = m_Doc->docLineStyles[ite->NamedLStyle];
					for (int it = ml.size() - 1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
							PS_setcmykcolor_stroke(h, s, v, k);
							PS_setlinewidth(ml[it].Width);
							PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
							PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
							SetClipPath(ite->PoLine);
							PS_closepath();
							putColor(ml[it].Color, ml[it].Shade, false);
						}
					}
				}
			}
			PS_restore();
		}
		if (!success)
			break;
	}
	return success;
}

bool PSLib::ProcessPageLayer(ScPage* page, ScLayer& layer, uint PNr)
{
	bool success = true;
	if (!layer.isPrintable || abortExport)
		return true;

	QList<PageItem*> items;
	items = (page->pageNameEmpty()) ? m_Doc->DocItems : m_Doc->MasterItems;

	for (int i = 0; i < items.count() && !abortExport; ++i)
	{
		PageItem *item = items.at(i);
		if (progressDialog)
			ScQApp->processEvents();
		if (item->m_layerID != layer.ID)
			continue;
		if ((!page->pageNameEmpty()) && (item->isTextContainer()))
			continue;
		if ((!page->pageNameEmpty()) && (item->isImageFrame()) && ((Options.outputSeparations) || (!Options.useColor)))
			continue;
		if ((m_outputFormat == OutputEPS) && (!item->isSelected()) && (m_Doc->m_Selection->count() != 0))
			continue;
		double bLeft, bRight, bBottom, bTop;
		GetBleeds(page, bLeft, bRight, bBottom, bTop);
		double x1 = page->xOffset() - bLeft;
		double y1 = page->yOffset() - bTop;
		double w1 = page->width() + bLeft + bRight;
		double h1 = page->height() + bBottom + bTop;
		double lw = item->visualLineWidth();
		double x2 = item->BoundingX - lw / 2.0;
		double y2 = item->BoundingY - lw / 2.0;
		double w2 = qMax(item->BoundingW + lw, 1.0);
		double h2 = qMax(item->BoundingH + lw, 1.0);
		if (!QRectF(x2, y2, w2, h2).intersects(QRectF(x1, y1, w1, h1)))
			continue;
		if (item->ChangedMasterItem)
			continue;
		if ((!page->pageNameEmpty()) && (item->OwnPage != page->pageNr()) && (item->OwnPage != -1))
			continue;
		success &= ProcessItem(page, item, PNr, false);
		if (!success)
			break;
	}
	return success;
}


void PSLib::HandleBrushPattern(PageItem *item, QPainterPath &path, ScPage* page, uint PNr, bool master)
{
	ScPattern pat = m_Doc->docPatterns[item->strokePattern()];
	double pLen = path.length() - ((pat.width / 2.0) * item->patternStrokeTransfrm.scaleX);
	double adv = pat.width * item->patternStrokeTransfrm.scaleX * item->patternStrokeTransfrm.space;
	double xpos = item->patternStrokeTransfrm.offsetX * item->patternStrokeTransfrm.scaleX;
	while (xpos < pLen)
	{
		double currPerc = path.percentAtLength(xpos);
		double currAngle = path.angleAtPercent(currPerc);
		if (currAngle <= 180.0)
			currAngle *= -1.0;
		else
			currAngle = 360.0 - currAngle;
		QPointF currPoint = path.pointAtPercent(currPerc);
		PS_save();
		PS_translate(currPoint.x(), -currPoint.y());
		PS_rotate(-currAngle);
		QTransform trans;
		trans.translate(0.0, -item->patternStrokeTransfrm.offsetY);
		trans.rotate(-item->patternStrokeTransfrm.rotation);
		trans.shear(item->patternStrokeTransfrm.skewX, -item->patternStrokeTransfrm.skewY);
		trans.scale(item->patternStrokeTransfrm.scaleX, item->patternStrokeTransfrm.scaleY);
		trans.translate(-pat.width / 2.0, -pat.height / 2.0);
		if (item->patternStrokeMirrorX)
		{
			trans.translate(pat.width, 0);
			trans.scale(-1, 1);
		}
		if (item->patternStrokeMirrorY)
		{
			trans.translate(0, pat.height);
			trans.scale(1, -1);
		}
		PutStream( MatrixToStr(trans.m11(), trans.m12(), trans.m21(), trans.m22(), trans.dx(), trans.dy()) + " concat\n");
		for (int em = 0; em < pat.items.count(); ++em)
		{
			PageItem* embedded = pat.items.at(em);
			PS_save();
			PS_translate(embedded->gXpos, embedded->gHeight - embedded->gYpos);
			ProcessItem(page, embedded, PNr, master, true);
			PS_restore();
		}
		xpos += adv;
		PS_restore();
	}
}

void PSLib::HandleStrokePattern(PageItem *item)
{
	ScPattern *pat;
	QTransform patternMatrix;
	pat = &m_Doc->docPatterns[item->strokePattern()];
	uint patHash = qHash(item->strokePattern());
	const ScStrokePatternTransform& patternTrans = item->strokePatternTransform();
	patternMatrix.translate(-item->lineWidth() / 2.0, item->lineWidth() / 2.0);
	patternMatrix.translate(patternTrans.offsetX, -patternTrans.offsetY);
	patternMatrix.rotate(-patternTrans.rotation);
	patternMatrix.shear(patternTrans.skewX, -patternTrans.skewY);
	patternMatrix.scale(pat->scaleX, pat->scaleY);
	patternMatrix.scale(patternTrans.scaleX, patternTrans.scaleY);
	bool mirrorX, mirrorY;
	item->strokePatternFlip(mirrorX, mirrorY);
	if (mirrorX)
		patternMatrix.scale(-1, 1);
	if (mirrorY)
		patternMatrix.scale(1, -1);
	PutStream("Pattern" + QString::number(patHash) + " [" + ToStr(patternMatrix.m11()) + " " + ToStr(patternMatrix.m12()) + " " + ToStr(patternMatrix.m21()) + " " + ToStr(patternMatrix.m22()) + " " + ToStr(patternMatrix.dx()) + " " + ToStr(patternMatrix.dy()) + "] makepattern setpattern\n");
	PutStream("stroke\n");
}

void PSLib::HandleMeshGradient(PageItem* item)
{
	QString hs,ss,vs,ks;
	double ch,cs,cv,ck;
	QStringList cols;
	QStringList colorValues;
	QStringList spotColorSet;
	QStringList tmpAddedColors;
	tmpAddedColors.clear();
	QList<int> colsSh;
	for (int grow = 0; grow < item->meshGradientArray.count(); grow++)
	{
		for (int gcol = 0; gcol < item->meshGradientArray[grow].count(); gcol++)
		{
			MeshPoint mp1 = item->meshGradientArray[grow][gcol];
			cols.append(mp1.colorName);
			if (!m_Doc->PageColors.contains(mp1.colorName))
			{
				if (!tmpAddedColors.contains(mp1.colorName))
				{
					tmpAddedColors.append(mp1.colorName);
					ScColor tmp;
					tmp.setSpotColor(false);
					tmp.setRegistrationColor(false);
					tmp.fromQColor(mp1.color);
					m_Doc->PageColors.insert(mp1.colorName, tmp);
				}
			}
			colsSh.append(mp1.shade);
			if (spotMap.contains(mp1.colorName))
			{
				if (!spotColorSet.contains(mp1.colorName))
					spotColorSet.append(mp1.colorName);
			}
		}
	}
	for (int ac = 0; ac < cols.count(); ac++)
	{
		QString colorVal;
		if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)) && (!GraySc))
		{
			if (spotColorSet.contains(cols.at(ac)))
			{
				colorVal = "0 0 0 0";
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == cols.at(ac))
						colorVal += " " + ToStr(colsSh[ac] / 100.0);
					else
						colorVal += " 0";
				}
			}
			else
			{
				SetColor(cols.at(ac), colsSh.at(ac), &ch, &cs, &cv, &ck);
				colorVal += hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					colorVal += " 0";
				}
			}
			colorValues.append(colorVal);
		}
		else
		{
			SetColor(cols.at(ac), colsSh.at(ac), &ch, &cs, &cv, &ck);
			if (GraySc)
				colorVal += hs.setNum(1.0 - qMin(0.3 * ch + 0.59 * cs + 0.11 * cv + ck, 1.0));
			else
				colorVal += hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
			if (DoSep)
			{
				int pla = Plate - 1 < 0 ? 3 : Plate - 1;
				QStringList cols2 = colorVal.split(" ", Qt::SkipEmptyParts);
				colorVal = ToStr(1 - ScCLocale::toDoubleC(cols2[pla]));
			}
			colorValues.append(colorVal);
		}
	}
	for (int grow = 0; grow < item->meshGradientArray.count() - 1; grow++)
	{
		PutStream("gs\n");
		PutStream("<<\n");
		PutStream("/PatternType 2\n");
		PutStream("/Shading\n");
		PutStream("<<\n");
		PutStream("/ShadingType 7\n");
		if (DoSep || GraySc)
		{
			PutStream("/ColorSpace /DeviceGray\n");
		}
		else if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			PutStream("/ColorSpace [ /DeviceN [/Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutStream(" (" + spotColorSet.at(sc) + ")");
			}
			PutStream("]\n");
			PutStream("/DeviceCMYK\n");
			PutStream("{\n");
			int maxSp = spotColorSet.count() - 1;
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				double cc(0), mc(0), yc(0), kc(0);
				CMYKColorF cmykValues;
				ScColorEngine::getCMYKValues(m_Doc->PageColors[spotColorSet.at(maxSp - sc)], m_Doc, cmykValues);
				cmykValues.getValues(cc, mc, yc, kc);
				if (sc == 0)
					PutStream("dup " + ToStr(cc) + " mul ");
				else
					PutStream(IToStr(sc * 4 + 1) + " -1 roll dup " + ToStr(cc) + " mul ");
				PutStream("exch dup " + ToStr(mc) + " mul ");
				PutStream("exch dup " + ToStr(yc) + " mul ");
				PutStream("exch " + ToStr(kc) + " mul\n");
			}
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutStream("8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n");
			}
			PutStream("} ]\n");
		}
		else
		{
			PutStream("/ColorSpace /DeviceCMYK\n");
		}
		PutStream("/DataSource [\n");
		QString vertStream;
		QTextStream vst(&vertStream, QIODevice::WriteOnly);
		quint8 flg = 0;
		for (int gcol = 0; gcol < item->meshGradientArray[grow].count() - 1; gcol++)
		{
			MeshPoint mp1 = item->meshGradientArray[grow][gcol];
			MeshPoint mp2 = item->meshGradientArray[grow][gcol+1];
			MeshPoint mp3 = item->meshGradientArray[grow+1][gcol+1];
			MeshPoint mp4 = item->meshGradientArray[grow+1][gcol];
			int colInd1 = grow * item->meshGradientArray[grow].count() + gcol;
			int colInd2 = grow * item->meshGradientArray[grow].count() + gcol + 1;
			int colInd3 = (grow + 1) * item->meshGradientArray[grow].count() + gcol + 1;
			int colInd4 = (grow + 1) * item->meshGradientArray[grow].count() + gcol;
			vst << flg << "\n";
			vst << mp4.gridPoint.x() << " " << -mp4.gridPoint.y() << " " << mp4.controlTop.x() << " " << -mp4.controlTop.y() << " " << mp1.controlBottom.x() << " " << -mp1.controlBottom.y() << "\n";
			vst << mp1.gridPoint.x() << " " << -mp1.gridPoint.y() << " " << mp1.controlRight.x() << " " << -mp1.controlRight.y() << " " << mp2.controlLeft.x() << " " << -mp2.controlLeft.y() << "\n";
			vst << mp2.gridPoint.x() << " " << -mp2.gridPoint.y() << " " << mp2.controlBottom.x() << " " << -mp2.controlBottom.y() << " " << mp3.controlTop.x() << " " << -mp3.controlTop.y() << "\n";
			vst << mp3.gridPoint.x() << " " << -mp3.gridPoint.y() << " " << mp3.controlLeft.x() << " " << -mp3.controlLeft.y() << " " << mp4.controlRight.x() << " " << -mp4.controlRight.y() << "\n";
			vst << mp4.controlColor.x() << " " << -mp4.controlColor.y() << "\n";
			vst << mp1.controlColor.x() << " " << -mp1.controlColor.y() << "\n";
			vst << mp2.controlColor.x() << " " << -mp2.controlColor.y() << "\n";
			vst << mp3.controlColor.x() << " " << -mp3.controlColor.y() << "\n";
			vst << colorValues[colInd4] << " " << colorValues[colInd1] << " " << colorValues[colInd2] << " " << colorValues[colInd3] << "\n";
		}
		PutStream(vertStream);
		PutStream("]\n");
		PutStream(">>\n");
		PutStream(">>\n");
		PutStream("[1 0 0 1 0 0] makepattern\n");
		PutStream("setpattern\n");
		if (fillRule)
			PutStream("eofill\n");
		else
			PutStream("fill\n");
		PutStream("gr\n");
	}
	PS_newpath();
	if (tmpAddedColors.count() != 0)
	{
		for (int cd = 0; cd < tmpAddedColors.count(); cd++)
		{
			m_Doc->PageColors.remove(tmpAddedColors[cd]);
		}
	}
}

void PSLib::HandlePatchMeshGradient(PageItem* item)
{
	QString hs,ss,vs,ks;
	double ch,cs,cv,ck;
	QStringList cols;
	QStringList colorValues;
	QStringList spotColorSet;
	QList<int> colsSh;
	for (int col = 0; col < item->meshGradientPatches.count(); col++)
	{
		meshGradientPatch patch = item->meshGradientPatches[col];
		MeshPoint mp1 = patch.TL;
		cols.append(mp1.colorName);
		colsSh.append(mp1.shade);
		if (spotMap.contains(mp1.colorName))
		{
			if (!spotColorSet.contains(mp1.colorName))
				spotColorSet.append(mp1.colorName);
		}
		MeshPoint mp2 = patch.TR;
		cols.append(mp2.colorName);
		colsSh.append(mp2.shade);
		if (spotMap.contains(mp2.colorName))
		{
			if (!spotColorSet.contains(mp2.colorName))
				spotColorSet.append(mp2.colorName);
		}
		MeshPoint mp3 = patch.BR;
		cols.append(mp3.colorName);
		colsSh.append(mp3.shade);
		if (spotMap.contains(mp3.colorName))
		{
			if (!spotColorSet.contains(mp3.colorName))
				spotColorSet.append(mp3.colorName);
		}
		MeshPoint mp4 = patch.BL;
		cols.append(mp4.colorName);
		colsSh.append(mp4.shade);
		if (spotMap.contains(mp4.colorName))
		{
			if (!spotColorSet.contains(mp4.colorName))
				spotColorSet.append(mp4.colorName);
		}
	}
	for (int ac = 0; ac < cols.count(); ac++)
	{
		QString colorVal;
		if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)) && (!GraySc))
		{
			if (spotColorSet.contains(cols.at(ac)))
			{
				colorVal = "0 0 0 0";
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == cols.at(ac))
						colorVal += " " + ToStr(colsSh[ac] / 100.0);
					else
						colorVal += " 0";
				}
			}
			else
			{
				SetColor(cols.at(ac), colsSh.at(ac), &ch, &cs, &cv, &ck);
				colorVal += hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					colorVal += " 0";
				}
			}
			colorValues.append(colorVal);
		}
		else
		{
			SetColor(cols.at(ac), colsSh.at(ac), &ch, &cs, &cv, &ck);
			if (GraySc)
				colorVal += hs.setNum(1.0 - qMin(0.3 * ch + 0.59 * cs + 0.11 * cv + ck, 1.0));
			else
				colorVal += hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
			if (DoSep)
			{
				int pla = Plate - 1 < 0 ? 3 : Plate - 1;
				QStringList cols2 = colorVal.split(" ", Qt::SkipEmptyParts);
				colorVal = ToStr(1 - ScCLocale::toDoubleC(cols2[pla]));
			}
			colorValues.append(colorVal);
		}
	}
	for (int col = 0; col < item->meshGradientPatches.count(); col++)
	{
		PutStream("gs\n");
		PutStream("<<\n");
		PutStream("/PatternType 2\n");
		PutStream("/Shading\n");
		PutStream("<<\n");
		PutStream("/ShadingType 7\n");
		if (DoSep || GraySc)
		{
			PutStream("/ColorSpace /DeviceGray\n");
		}
		else if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			PutStream("/ColorSpace [ /DeviceN [/Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutStream(" (" + spotColorSet.at(sc) + ")");
			}
			PutStream("]\n");
			PutStream("/DeviceCMYK\n");
			PutStream("{\n");
			int maxSp = spotColorSet.count() - 1;
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				double cc(0), mc(0), yc(0), kc(0);
				CMYKColorF cmykValues;
				ScColorEngine::getCMYKValues(m_Doc->PageColors[spotColorSet.at(maxSp - sc)], m_Doc, cmykValues);
				cmykValues.getValues(cc, mc, yc, kc);
				if (sc == 0)
					PutStream("dup " + ToStr(cc) + " mul ");
				else
					PutStream(IToStr(sc * 4 + 1) + " -1 roll dup " + ToStr(cc) + " mul ");
				PutStream("exch dup " + ToStr(mc) + " mul ");
				PutStream("exch dup " + ToStr(yc) + " mul ");
				PutStream("exch " + ToStr(kc) + " mul\n");
			}
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutStream("8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n");
			}
			PutStream("} ]\n");
		}
		else
		{
			PutStream("/ColorSpace /DeviceCMYK\n");
		}
		PutStream("/DataSource [\n");
		QString vertStream;
		QTextStream vst(&vertStream, QIODevice::WriteOnly);
		quint8 flg = 0;
		for (int col2 = col; col2 < item->meshGradientPatches.count(); col2++)
		{
			col = col2;
			meshGradientPatch patch = item->meshGradientPatches[col2];
			MeshPoint mp1 = patch.TL;
			MeshPoint mp2 = patch.TR;
			MeshPoint mp3 = patch.BR;
			MeshPoint mp4 = patch.BL;
			int colInd1 = 4 * col2;
			int colInd2 = 4 * col2 + 1;
			int colInd3 = 4 * col2 + 2;
			int colInd4 = 4 * col2 + 3;
			vst << flg << "\n";
			vst << mp4.gridPoint.x() << " " << -mp4.gridPoint.y() << " " << mp4.controlTop.x() << " " << -mp4.controlTop.y() << " " << mp1.controlBottom.x() << " " << -mp1.controlBottom.y() << "\n";
			vst << mp1.gridPoint.x() << " " << -mp1.gridPoint.y() << " " << mp1.controlRight.x() << " " << -mp1.controlRight.y() << " " << mp2.controlLeft.x() << " " << -mp2.controlLeft.y() << "\n";
			vst << mp2.gridPoint.x() << " " << -mp2.gridPoint.y() << " " << mp2.controlBottom.x() << " " << -mp2.controlBottom.y() << " " << mp3.controlTop.x() << " " << -mp3.controlTop.y() << "\n";
			vst << mp3.gridPoint.x() << " " << -mp3.gridPoint.y() << " " << mp3.controlLeft.x() << " " << -mp3.controlLeft.y() << " " << mp4.controlRight.x() << " " << -mp4.controlRight.y() << "\n";
			vst << mp4.controlColor.x() << " " << -mp4.controlColor.y() << "\n";
			vst << mp1.controlColor.x() << " " << -mp1.controlColor.y() << "\n";
			vst << mp2.controlColor.x() << " " << -mp2.controlColor.y() << "\n";
			vst << mp3.controlColor.x() << " " << -mp3.controlColor.y() << "\n";
			vst << colorValues[colInd4] << " " << colorValues[colInd1] << " " << colorValues[colInd2] << " " << colorValues[colInd3] << "\n";
			if ((col % 1000) == 0)
			{
				break;
			}
		}
		PutStream(vertStream);
		PutStream("]\n");
		PutStream(">>\n");
		PutStream(">>\n");
		PutStream("[1 0 0 1 0 0] makepattern\n");
		PutStream("setpattern\n");
		if (fillRule)
			PutStream("eofill\n");
		else
			PutStream("fill\n");
		PutStream("gr\n");
	}
	PS_newpath();
}

void PSLib::HandleDiamondGradient(PageItem* item)
{
	QString hs,ss,vs,ks;
	double ch,cs,cv,ck;
	QStringList cols;
	QStringList colorValues;
	QStringList spotColorSet;
	QList<int> colsSh;
	VGradient gradient;
	if (!(item->gradient().isEmpty()) && (m_Doc->docGradients.contains(item->gradient())))
		gradient = m_Doc->docGradients[item->gradient()];
	else
		gradient = item->fill_gradient;
	gradient.setRepeatMethod(item->getGradientExtend());
	QList<VColorStop*> colorStops = gradient.colorStops();
	QList<double> qStopRampPoints;
	for (int cst = 0; cst < gradient.stops(); ++cst)
	{
		if (cst == 0)
		{
			if (colorStops[cst]->rampPoint > 0)
			{
				qStopRampPoints.append(0);
				cols.append(colorStops.at(cst)->name);
				colsSh.append(colorStops.at(cst)->shade);
				if (spotMap.contains(colorStops.at(cst)->name))
				{
					if (!spotColorSet.contains(colorStops.at(cst)->name))
						spotColorSet.append(colorStops.at(cst)->name);
				}
			}
		}
		cols.append(colorStops.at(cst)->name);
		colsSh.append(colorStops.at(cst)->shade);
		if (spotMap.contains(colorStops.at(cst)->name))
		{
			if (!spotColorSet.contains(colorStops.at(cst)->name))
				spotColorSet.append(colorStops.at(cst)->name);
		}
		qStopRampPoints.append(colorStops.at(cst)->rampPoint);
	}
	for (int ac = 0; ac < cols.count(); ac++)
	{
		QString colorVal;
		if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)) && (!GraySc))
		{
			if (spotColorSet.contains(cols.at(ac)))
			{
				colorVal = "0 0 0 0";
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == cols.at(ac))
						colorVal += " " + ToStr(colsSh[ac] / 100.0);
					else
						colorVal += " 0";
				}
			}
			else
			{
				SetColor(cols.at(ac), colsSh.at(ac), &ch, &cs, &cv, &ck);
				colorVal += hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					colorVal += " 0";
				}
			}
			colorValues.append(colorVal);
		}
		else
		{
			SetColor(cols.at(ac), colsSh.at(ac), &ch, &cs, &cv, &ck);
			if (GraySc)
				colorVal += hs.setNum(1.0 - qMin(0.3 * ch + 0.59 * cs + 0.11 * cv + ck, 1.0));
			else
				colorVal += hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
			if (DoSep)
			{
				int pla = Plate - 1 < 0 ? 3 : Plate - 1;
				QStringList cols2 = colorVal.split(" ", Qt::SkipEmptyParts);
				colorVal = ToStr(1 - ScCLocale::toDoubleC(cols2[pla]));
			}
			colorValues.append(colorVal);
		}
	}
	PutStream("<<\n");
	PutStream("/PatternType 2\n");
	PutStream("/Shading\n");
	PutStream("<<\n");
	PutStream("/ShadingType 6\n");
	if (DoSep || GraySc)
		PutStream("/ColorSpace /DeviceGray\n");
	else if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
	{
		PutStream("/ColorSpace [ /DeviceN [/Cyan /Magenta /Yellow /Black");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutStream(" (" + spotColorSet.at(sc) + ")");
		}
		PutStream("]\n");
		PutStream("/DeviceCMYK\n");
		PutStream("{\n");
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			double cc(0), mc(0), yc(0), kc(0);
			CMYKColorF cmykValues;
			ScColorEngine::getCMYKValues(m_Doc->PageColors[spotColorSet.at(maxSp - sc)], m_Doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				PutStream("dup " + ToStr(cc) + " mul ");
			else
				PutStream(IToStr(sc * 4 + 1) + " -1 roll dup " + ToStr(cc) + " mul ");
			PutStream("exch dup " + ToStr(mc) + " mul ");
			PutStream("exch dup " + ToStr(yc) + " mul ");
			PutStream("exch " + ToStr(kc) + " mul\n");
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutStream("8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n");
		}
		PutStream("} ]\n");
	}
	else
		PutStream("/ColorSpace /DeviceCMYK\n");
	PutStream("/Background [" + colorValues[colorValues.count() - 1] + "]\n");
	PutStream("/DataSource [\n");
	QPointF centerP(item->GrControl5.x(), -item->GrControl5.y());
	QLineF edge1(centerP, QPointF(item->GrControl1.x(), -item->GrControl1.y()));
	QLineF edge2(centerP, QPointF(item->GrControl2.x(), -item->GrControl2.y()));
	QLineF edge3(centerP, QPointF(item->GrControl3.x(), -item->GrControl3.y()));
	QLineF edge4(centerP, QPointF(item->GrControl4.x(), -item->GrControl4.y()));
	for (int offset = 1; offset < qStopRampPoints.count(); ++offset)
	{
		QLineF e1 = edge1;
		QLineF e1s = edge1;
		QLineF e2 = edge2;
		QLineF e2s = edge2;
		QLineF e3 = edge3;
		QLineF e3s = edge3;
		QLineF e4 = edge4;
		QLineF e4s = edge4;
		e1.setLength(edge1.length() * qStopRampPoints[ offset ]);
		e2.setLength(edge2.length() * qStopRampPoints[ offset ]);
		e3.setLength(edge3.length() * qStopRampPoints[ offset ]);
		e4.setLength(edge4.length() * qStopRampPoints[ offset ]);
		e1s.setLength(edge1.length() * qStopRampPoints[ offset - 1 ]);
		e2s.setLength(edge2.length() * qStopRampPoints[ offset - 1 ]);
		e3s.setLength(edge3.length() * qStopRampPoints[ offset - 1 ]);
		e4s.setLength(edge4.length() * qStopRampPoints[ offset - 1 ]);
		if (offset == 1)
		{
			PutStream("0\n");
			PutStream(ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(e1.x2()) + " " + ToStr(e1.y2()) + "\n");
			PutStream(ToStr(e1.x2()) + " " + ToStr(e1.y2()) + " " + ToStr(e1.x2()) + " " + ToStr(e1.y2()) + " " + ToStr(e2.x2()) + " " + ToStr(e2.y2()) + "\n");
			PutStream(ToStr(e2.x2()) + " " + ToStr(e2.y2()) + " " + ToStr(e2.x2()) + " " + ToStr(e2.y2()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + "\n");
			PutStream(ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + "\n");
			PutStream(colorValues[0] + " " + colorValues[1] + " " + colorValues[1] + " " + colorValues[0] + "\n");
			PutStream("0\n");
			PutStream(ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + "\n");
			PutStream(ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(e2.x2()) + " " + ToStr(e2.y2()) + "\n");
			PutStream(ToStr(e2.x2()) + " " + ToStr(e2.y2()) + " " + ToStr(e2.x2()) + " " + ToStr(e2.y2()) + " " + ToStr(e3.x2()) + " " + ToStr(e3.y2()) + "\n");
			PutStream(ToStr(e3.x2()) + " " + ToStr(e3.y2()) + " " + ToStr(e3.x2()) + " " + ToStr(e3.y2()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + "\n");
			PutStream(colorValues[0] + " " + colorValues[0] + " " + colorValues[1] + " " + colorValues[1] + "\n");
			PutStream("0\n");
			PutStream(ToStr(e4.x2()) + " " + ToStr(e4.y2()) + " " + ToStr(e4.x2()) + " " + ToStr(e4.y2()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + "\n");
			PutStream(ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + "\n");
			PutStream(ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(e3.x2()) + " " + ToStr(e3.y2()) + "\n");
			PutStream(ToStr(e3.x2()) + " " + ToStr(e3.y2()) + " " + ToStr(e3.x2()) + " " + ToStr(e3.y2()) + " " + ToStr(e4.x2()) + " " + ToStr(e4.y2()) + "\n");
			PutStream(colorValues[1] + " " + colorValues[0] + " " + colorValues[0] + " " + colorValues[1] + "\n");
			PutStream("0\n");
			PutStream(ToStr(e4.x2()) + " " + ToStr(e4.y2()) + " " + ToStr(e4.x2()) + " " + ToStr(e4.y2()) + " " + ToStr(e1.x2()) + " " + ToStr(e1.y2()) + "\n");
			PutStream(ToStr(e1.x2()) + " " + ToStr(e1.y2()) + " " + ToStr(e1.x2()) + " " + ToStr(e1.y2()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + "\n");
			PutStream(ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + "\n");
			PutStream(ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(centerP.x()) + " " + ToStr(centerP.y()) + " " + ToStr(e4.x2()) + " " + ToStr(e4.y2()) + "\n");
			PutStream(colorValues[1] + " " + colorValues[1] + " " + colorValues[0] + " " + colorValues[0] + "\n");
		}
		else
		{
			PutStream("0\n");
			PutStream(ToStr(e1s.x2()) + " " + ToStr(e1s.y2()) + " " + ToStr(e1s.x2()) + " " + ToStr(e1s.y2()) + " " + ToStr(e1.x2()) + " " + ToStr(e1.y2()) + "\n");
			PutStream(ToStr(e1.x2()) + " " + ToStr(e1.y2()) + " " + ToStr(e1.x2()) + " " + ToStr(e1.y2()) + " " + ToStr(e2.x2()) + " " + ToStr(e2.y2()) + "\n");
			PutStream(ToStr(e2.x2()) + " " + ToStr(e2.y2()) + " " + ToStr(e2.x2()) + " " + ToStr(e2.y2()) + " " + ToStr(e2s.x2()) + " " + ToStr(e2s.y2()) + "\n");
			PutStream(ToStr(e2s.x2()) + " " + ToStr(e2s.y2()) + " " + ToStr(e2s.x2()) + " " + ToStr(e2s.y2()) + " " + ToStr(e1s.x2()) + " " + ToStr(e1s.y2()) + "\n");
			PutStream(colorValues[offset - 1] + " " + colorValues[offset] + " " + colorValues[offset] + " " + colorValues[offset - 1] + "\n");
			PutStream("0\n");
			PutStream(ToStr(e3s.x2()) + " " + ToStr(e3s.y2()) + " " + ToStr(e3s.x2()) + " " + ToStr(e3s.y2()) + " " + ToStr(e2s.x2()) + " " + ToStr(e2s.y2()) + "\n");
			PutStream(ToStr(e2s.x2()) + " " + ToStr(e2s.y2()) + " " + ToStr(e2s.x2()) + " " + ToStr(e2s.y2()) + " " + ToStr(e2.x2()) + " " + ToStr(e2.y2()) + "\n");
			PutStream(ToStr(e2.x2()) + " " + ToStr(e2.y2()) + " " + ToStr(e2.x2()) + " " + ToStr(e2.y2()) + " " + ToStr(e3.x2()) + " " + ToStr(e3.y2()) + "\n");
			PutStream(ToStr(e3.x2()) + " " + ToStr(e3.y2()) + " " + ToStr(e3.x2()) + " " + ToStr(e3.y2()) + " " + ToStr(e3s.x2()) + " " + ToStr(e3s.y2()) + "\n");
			PutStream(colorValues[offset - 1] + " " + colorValues[offset - 1] + " " + colorValues[offset] + " " + colorValues[offset] + "\n");
			PutStream("0\n");
			PutStream(ToStr(e4.x2()) + " " + ToStr(e4.y2()) + " " + ToStr(e4.x2()) + " " + ToStr(e4.y2()) + " " + ToStr(e4s.x2()) + " " + ToStr(e4s.y2()) + "\n");
			PutStream(ToStr(e4s.x2()) + " " + ToStr(e4s.y2()) + " " + ToStr(e4s.x2()) + " " + ToStr(e4s.y2()) + " " + ToStr(e3s.x2()) + " " + ToStr(e3s.y2()) + "\n");
			PutStream(ToStr(e3s.x2()) + " " + ToStr(e3s.y2()) + " " + ToStr(e3s.x2()) + " " + ToStr(e3s.y2()) + " " + ToStr(e3.x2()) + " " + ToStr(e3.y2()) + "\n");
			PutStream(ToStr(e3.x2()) + " " + ToStr(e3.y2()) + " " + ToStr(e3.x2()) + " " + ToStr(e3.y2()) + " " + ToStr(e4.x2()) + " " + ToStr(e4.y2()) + "\n");
			PutStream(colorValues[offset] + " " + colorValues[offset - 1] + " " + colorValues[offset - 1] + " " + colorValues[offset] + "\n");
			PutStream("0\n");
			PutStream(ToStr(e4.x2()) + " " + ToStr(e4.y2()) + " " + ToStr(e4.x2()) + " " + ToStr(e4.y2()) + " " + ToStr(e1.x2()) + " " + ToStr(e1.y2()) + "\n");
			PutStream(ToStr(e1.x2()) + " " + ToStr(e1.y2()) + " " + ToStr(e1.x2()) + " " + ToStr(e1.y2()) + " " + ToStr(e1s.x2()) + " " + ToStr(e1s.y2()) + "\n");
			PutStream(ToStr(e1s.x2()) + " " + ToStr(e1s.y2()) + " " + ToStr(e1s.x2()) + " " + ToStr(e1s.y2()) + " " + ToStr(e4s.x2()) + " " + ToStr(e4s.y2()) + "\n");
			PutStream(ToStr(e4s.x2()) + " " + ToStr(e4s.y2()) + " " + ToStr(e4s.x2()) + " " + ToStr(e4s.y2()) + " " + ToStr(e4.x2()) + " " + ToStr(e4.y2()) + "\n");
			PutStream(colorValues[offset] + " " + colorValues[offset] + " " + colorValues[offset - 1] + " " + colorValues[offset - 1] + "\n");
		}
	}
	PutStream("]\n");
	PutStream(">>\n");
	PutStream(">>\n");
	PutStream("[1 0 0 1 0 0] makepattern setpattern\n");
	if (fillRule)
		PutStream("eofill\n");
	else
		PutStream("fill\n");
}

void PSLib::HandleTensorGradient(PageItem* item)
{
	QString GCol;
	QString hs,ss,vs,ks;
	double ch,cs,cv,ck;
	QStringList cols;
	QStringList spotColorSet;
	QList<int> colsSh;
	cols << item->GrColorP4 << item->GrColorP1 << item->GrColorP2 << item->GrColorP3;
	colsSh << item->GrCol4Shade << item->GrCol1Shade << item->GrCol2Shade << item->GrCol3Shade;
	for (int cst = 0; cst < cols.count(); ++cst)
	{
		if (spotMap.contains(cols.at(cst)))
		{
			if (!spotColorSet.contains(cols.at(cst)))
				spotColorSet.append(cols.at(cst));
		}
	}
	PutStream("<<\n");
	PutStream("/PatternType 2\n");
	PutStream("/Shading\n");
	PutStream("<<\n");
	PutStream("/ShadingType 7\n");
	if (DoSep || GraySc)
		PutStream("/ColorSpace /DeviceGray\n");
	else if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
	{
		PutStream("/ColorSpace [ /DeviceN [/Cyan /Magenta /Yellow /Black");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutStream(" (" + spotColorSet.at(sc) + ")");
		}
		PutStream("]\n");
		PutStream("/DeviceCMYK\n");
		PutStream("{\n");
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			double cc(0), mc(0), yc(0), kc(0);
			CMYKColorF cmykValues;
			ScColorEngine::getCMYKValues(m_Doc->PageColors[spotColorSet.at(maxSp - sc)], m_Doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				PutStream("dup " + ToStr(cc) + " mul ");
			else
				PutStream(IToStr(sc * 4 + 1) + " -1 roll dup " + ToStr(cc) + " mul ");
			PutStream("exch dup " + ToStr(mc) + " mul ");
			PutStream("exch dup " + ToStr(yc) + " mul ");
			PutStream("exch " + ToStr(kc) + " mul\n");
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutStream("8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n");
		}
		PutStream("} ]\n");
	}
	else
		PutStream("/ColorSpace /DeviceCMYK\n");
	PutStream("/DataSource [0\n");
	PutStream("0 " + ToStr(-item->height()) + " 0 " + ToStr(-item->height()) + " 0 0 0 0 0 0 " + ToStr(item->width()) + " 0 " + ToStr(item->width()) + " 0 " + ToStr(item->width()) + " 0\n");
	PutStream(ToStr(item->width()) + " " + ToStr(-item->height()) + " " + ToStr(item->width()) + " " + ToStr(-item->height()) + " " + ToStr(item->width()) + " " + ToStr(-item->height()) + "\n");
	PutStream("0 " + ToStr(-item->height()) + "\n");
	PutStream(ToStr(item->GrControl1.x()) + " " + ToStr(-item->GrControl1.y()) + "\n");
	PutStream(ToStr(item->GrControl4.x()) + " " + ToStr(-item->GrControl4.y()) + "\n");
	PutStream(ToStr(item->GrControl3.x()) + " " + ToStr(-item->GrControl3.y()) + "\n");
	PutStream(ToStr(item->GrControl2.x()) + " " + ToStr(-item->GrControl2.y()) + "\n");
	for (int ac = 0; ac < cols.count(); ac++)
	{
		if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)) && (!GraySc))
		{
			if (spotColorSet.contains(cols.at(ac)))
			{
				PutStream("0 0 0 0");
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == cols.at(ac))
						PutStream(" " + ToStr(colsSh[ac] / 100.0));
					else
						PutStream(" 0");
				}
			}
			else
			{
				SetColor(cols.at(ac), colsSh.at(ac), &ch, &cs, &cv, &ck);
				GCol = hs.setNum(ch)  + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
				PutStream(GCol);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					PutStream(" 0");
				}
			}
			PutStream("\n");
		}
		else
		{
			SetColor(cols.at(ac), colsSh.at(ac), &ch, &cs, &cv, &ck);
			if (GraySc)
				GCol = hs.setNum(1.0 - qMin(0.3 * ch + 0.59 * cs + 0.11 * cv + ck, 1.0));
			else
				GCol = hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
			if (DoSep)
			{
				int pla = Plate - 1 < 0 ? 3 : Plate - 1;
				QStringList cols2 = GCol.split(" ", Qt::SkipEmptyParts);
				PutStream(ToStr(1 - ScCLocale::toDoubleC(cols2[pla])) + "\n");
			}
			else
				PutStream(GCol + "\n");
		}
	}
	PutStream("]\n");
	PutStream(">>\n");
	PutStream(">>\n");
	PutStream("[1 0 0 1 0 0] makepattern setpattern\n");
	if (fillRule)
		PutStream("eofill\n");
	else
		PutStream("fill\n");
}

void PSLib::HandleGradientFillStroke(PageItem *item, bool stroke, bool forArrow)
{
	double StartX, StartY, EndX, EndY, FocalX, FocalY, Gscale, Gskew;
	int GType;
	VGradient gradient;
	QList<double> StopVec;
	QStringList colorNames;
	QList<int> colorShades;
	QStringList spotColorSet;
	if (stroke)
	{
		GType = item->GrTypeStroke;
		StartX = item->GrStrokeStartX;
		StartY = item->GrStrokeStartY;
		EndX = item->GrStrokeEndX;
		EndY = item->GrStrokeEndY;
		FocalX = item->GrStrokeFocalX;
		FocalY = item->GrStrokeFocalY;
		Gscale = item->GrStrokeScale;
		Gskew = item->GrStrokeSkew;
		if (!(item->strokeGradient().isEmpty()) && (m_Doc->docGradients.contains(item->strokeGradient())))
			gradient = m_Doc->docGradients[item->strokeGradient()];
		else
			gradient = item->stroke_gradient;
		gradient.setRepeatMethod(item->getStrokeGradientExtend());
	}
	else
	{
		GType = item->GrType;
		StartX = item->GrStartX;
		StartY = item->GrStartY;
		EndX = item->GrEndX;
		EndY = item->GrEndY;
		FocalX = item->GrFocalX;
		FocalY = item->GrFocalY;
		Gscale = item->GrScale;
		Gskew = item->GrSkew;
		if (!(item->gradient().isEmpty()) && (m_Doc->docGradients.contains(item->gradient())))
			gradient = m_Doc->docGradients[item->gradient()];
		else
			gradient = item->fill_gradient;
		gradient.setRepeatMethod(item->getGradientExtend());
		if (GType == Gradient_Pattern)
		{
			QTransform patternMatrix;
			ScPattern *pat = &m_Doc->docPatterns[item->pattern()];
			uint patHash = qHash(item->pattern());
			const ScPatternTransform& patternTrans = item->patternTransform();
			patternMatrix.translate(patternTrans.offsetX, -patternTrans.offsetY);
			patternMatrix.rotate(-patternTrans.rotation);
			patternMatrix.shear(patternTrans.skewX, -patternTrans.skewY);
			patternMatrix.scale(pat->scaleX, pat->scaleY);
			patternMatrix.scale(patternTrans.scaleX, patternTrans.scaleY);
			bool mirrorX, mirrorY;
			item->patternFlip(mirrorX, mirrorY);
			if (mirrorX)
				patternMatrix.scale(-1, 1);
			if (mirrorY)
				patternMatrix.scale(1, -1);
			PutStream("Pattern" + QString::number(patHash) + " [" + ToStr(patternMatrix.m11()) + " " + ToStr(patternMatrix.m12()) + " " + ToStr(patternMatrix.m21()) + " " + ToStr(patternMatrix.m22()) + " " + ToStr(patternMatrix.dx()) + " " + ToStr(patternMatrix.dy()) + "] makepattern setpattern\n");
			if (fillRule)
				PutStream("eofill\n");
			else
				PutStream("fill\n");
			return;
		}
		if (GType == Gradient_4Colors)
		{
			HandleTensorGradient(item);
			return;
		}
		if (GType == Gradient_Diamond)
		{
			HandleDiamondGradient(item);
			return;
		}
		if ((GType == Gradient_Mesh) || (GType == Gradient_Conical))
		{
			HandleMeshGradient(item);
			return;
		}
		if (GType == Gradient_PatchMesh)
		{
			HandlePatchMeshGradient(item);
			return;
		}
	}
	QList<VColorStop*> cstops = gradient.colorStops();
	PutStream("<<\n");
	PutStream("/PatternType 2\n");
	PutStream("/Shading\n");
	PutStream("<<\n");
	for (int cst = 0; cst < gradient.stops(); ++cst)
	{
		double actualStop = cstops.at(cst)->rampPoint;
		if ((cst == 0) && (actualStop != 0.0))
		{
			StopVec.append(0.0);
			colorNames.append(cstops.at(cst)->name);
			colorShades.append(cstops.at(cst)->shade);
		}
		StopVec.append(actualStop);
		colorNames.append(cstops.at(cst)->name);
		colorShades.append(cstops.at(cst)->shade);
		if (spotMap.contains(cstops.at(cst)->name))
		{
			if (!spotColorSet.contains(cstops.at(cst)->name))
				spotColorSet.append(cstops.at(cst)->name);
		}
		if ((cst == gradient.stops()-1) && (actualStop < 1.0))
		{
			StopVec.append(1.0);
			colorNames.append(cstops.at(cst)->name);
			colorShades.append(cstops.at(cst)->shade);
		}
	}
	if (GType == Gradient_Linear)
		PutStream("/ShadingType 2\n");
	else
		PutStream("/ShadingType 3\n");
	if (DoSep || GraySc)
		PutStream("/ColorSpace /DeviceGray\n");
	else if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
	{
		PutStream("/ColorSpace [ /DeviceN [/Cyan /Magenta /Yellow /Black");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutStream(" (" + spotColorSet.at(sc) + ")");
		}
		PutStream("]\n");
		PutStream("/DeviceCMYK\n");
		PutStream("{\n");
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			double cc(0), mc(0), yc(0), kc(0);
			CMYKColorF cmykValues;
			ScColorEngine::getCMYKValues(m_Doc->PageColors[spotColorSet.at(maxSp - sc)], m_Doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				PutStream("dup " + ToStr(cc) + " mul ");
			else
				PutStream(IToStr(sc * 4 + 1) + " -1 roll dup " + ToStr(cc) + " mul ");
			PutStream("exch dup " + ToStr(mc) + " mul ");
			PutStream("exch dup " + ToStr(yc) + " mul ");
			PutStream("exch " + ToStr(kc) + " mul\n");
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutStream("8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n");
		}
		PutStream("} ]\n");
	}
	else
		PutStream("/ColorSpace /DeviceCMYK\n");
	if (gradient.repeatMethod() == VGradient::none)
		PutStream("/Extend [false false]\n");
	else
		PutStream("/Extend [true true]\n");
	if (GType == Gradient_Linear)
		PutStream("/Coords [" + ToStr(StartX) + " " + ToStr(-StartY) + " " + ToStr(EndX) + " " + ToStr(-EndY) + "]\n");
	else
		PutStream("/Coords [" + ToStr(FocalX) + " " + ToStr(-FocalY) + " 0.0 " + ToStr(StartX) + " " + ToStr(-StartY) + " " + ToStr(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2))) + "]\n");
	PutStream("/Function\n");
	PutStream("<<\n");
	PutStream("/FunctionType 3\n");
	PutStream("/Domain [0 1]\n");
	if (StopVec.count() > 2)
	{
		PutStream("/Bounds [");
		QString bctx;
		for (int bc = 1; bc < StopVec.count() - 1; bc++)
		{
			bctx += ToStr(StopVec.at(bc)) + " ";
		}
		PutStream(bctx.trimmed() + "]\n");
	}
	else
		PutStream("/Bounds []\n");
	QString entx;
	PutStream("/Functions\n");
	PutStream("[\n");
	for (int cc = 0; cc < colorNames.count() - 1; cc++)
	{
		QString GCol, GCol2;
		QString hs,ss,vs,ks;
		double ch,cs,cv,ck;
		entx += "0 1 ";
		PutStream("<<\n");
		PutStream("/FunctionType 2\n");
		PutStream("/Domain [0 1]\n");
		if ((Options.useSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)) && (!GraySc))
		{
			if (spotColorSet.contains(colorNames.at(cc)))
			{
				PutStream("/C0 [0 0 0 0");
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == colorNames.at(cc))
						PutStream(" " + ToStr(colorShades[cc] / 100.0));
					else
						PutStream(" 0");
				}
			}
			else
			{
				SetColor(colorNames.at(cc), colorShades.at(cc), &ch, &cs, &cv, &ck);
				GCol = hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
				PutStream("/C0 [" + GCol);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					PutStream(" 0");
				}
			}
			PutStream("]\n");
			if (spotColorSet.contains(colorNames.at(cc+1)))
			{
				PutStream("/C1 [0 0 0 0");
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == colorNames.at(cc+1))
						PutStream(" " + ToStr(colorShades[cc+1] / 100.0));
					else
						PutStream(" 0");
				}
			}
			else
			{
				SetColor(colorNames.at(cc+1), colorShades.at(cc+1), &ch, &cs, &cv, &ck);
				GCol2 = hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
				PutStream("/C1 [" + GCol2);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					PutStream(" 0");
				}
			}
			PutStream("]\n");
		}
		else
		{
			SetColor(colorNames.at(cc), colorShades.at(cc), &ch, &cs, &cv, &ck);
			if (GraySc)
				GCol = hs.setNum(1.0 - qMin(0.3 * ch + 0.59 * cs + 0.11 * cv + ck, 1.0));
			else
				GCol = hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
			SetColor(colorNames.at(cc+1), colorShades.at(cc+1), &ch, &cs, &cv, &ck);
			if (GraySc)
				GCol2 = hs.setNum(1.0 - qMin(0.3 * ch + 0.59 * cs + 0.11 * cv + ck, 1.0));
			else
				GCol2 = hs.setNum(ch) + " " + ss.setNum(cs) + " " + vs.setNum(cv) + " " + ks.setNum(ck);
			if (DoSep)
			{
				int pla = Plate - 1 < 0 ? 3 : Plate - 1;
				QStringList cols1 = GCol.split(" ", Qt::SkipEmptyParts);
				QStringList cols2 = GCol2.split(" ", Qt::SkipEmptyParts);
				PutStream("/C1 [" + ToStr(1 - ScCLocale::toDoubleC(cols1[pla])) + "]\n");
				PutStream("/C0 [" + ToStr(1 - ScCLocale::toDoubleC(cols2[pla])) + "]\n");
			}
			else
			{
				PutStream("/C0 [" + GCol + "]\n");
				PutStream("/C1 [" + GCol2  + "]\n");
			}
		}
		PutStream("/N 1\n");
		PutStream(">>\n");
	}
	PutStream("]\n");
	PutStream("/Encode [" + entx.trimmed() + "]\n");
	PutStream(">>\n");
	PutStream(">>\n");
	PutStream(">>\n");
	QTransform qmatrix;
	if (Gskew == 90)
		Gskew = 1;
	else if (Gskew == 180)
		Gskew = 0;
	else if (Gskew == 270)
		Gskew = -1;
	else if (Gskew == 390)
		Gskew = 0;
	else
		Gskew = tan(M_PI / 180.0 * Gskew);
	if (GType == Gradient_Linear)
	{
		qmatrix.translate(StartX, -StartY);
		qmatrix.shear(Gskew, 0);
		qmatrix.translate(-StartX, StartY);
	}
	else
	{
		double rotEnd = xy2Deg(EndX - StartX, EndY - StartY);
		qmatrix.translate(StartX, -StartY);
		qmatrix.rotate(-rotEnd);
		qmatrix.shear(-Gskew, 0);
		qmatrix.translate(0, -StartY * (1.0 - Gscale));
		qmatrix.translate(-StartX, StartY);
		qmatrix.scale(1, Gscale);
	}
	PutStream("[" + ToStr(qmatrix.m11()) + " " + ToStr(qmatrix.m12()) + " " + ToStr(qmatrix.m21()) + " " + ToStr(qmatrix.m22()) + " " + ToStr(qmatrix.dx()) + " " + ToStr(qmatrix.dy()) + "] makepattern setpattern\n");
//	PutStream("[1 0 0 1 0 0] makepattern setpattern\n");
	if (forArrow)
	{
		if (fillRule)
			PutStream("eofill\n");
		else
			PutStream("fill\n");
	}
	else
	{
		if (stroke)
			PutStream("stroke\n");
		else
		{
			if (fillRule)
				PutStream("eofill\n");
			else
				PutStream("fill\n");
		}
	}
}

void PSLib::PS_HatchFill(PageItem *currItem)
{
	PS_save();
	QVector<double> dum;
	double h, s, v, k;
	PS_setlinewidth(1);
	PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
	PS_setdash(Qt::SolidLine, 0, dum);
	if ((currItem->hatchBackground != CommonStrings::None) && (currItem->hatchUseBackground))
	{
		SetColor(currItem->hatchBackground, 100, &h, &s, &v, &k);
		PS_setcmykcolor_fill(h, s, v, k);
		putColor(currItem->hatchBackground, 100, true);
		SetPathAndClip(currItem->PoLine, currItem->fillRule);
	}
	else
		PS_clip(currItem->fillRule);
	if (currItem->hatchForeground != CommonStrings::None)
	{
		SetColor(currItem->hatchForeground, 100, &h, &s, &v, &k);
		PS_setcmykcolor_stroke(h, s, v, k);
	}
	PS_translate(currItem->width() / 2.0, -currItem->height() / 2.0);
	double lineLen = sqrt((currItem->width() / 2.0) * (currItem->width() / 2.0) + (currItem->height() / 2.0) * (currItem->height() / 2.0));
	double dist = 0.0;
	PS_save();
	PS_rotate(currItem->hatchAngle);
	while (dist < lineLen)
	{
		PS_moveto(-lineLen, dist);
		PS_lineto(lineLen, dist);
		putColor(currItem->hatchForeground, 100, false);
		if (dist > 0)
		{
			PS_moveto(-lineLen, -dist);
			PS_lineto(lineLen, -dist);
			putColor(currItem->hatchForeground, 100, false);
		}
		dist += currItem->hatchDistance;
	}
	PS_restore();
	dist = 0.0;
	if ((currItem->hatchType == 1) || (currItem->hatchType == 2))
	{
		PS_save();
		PS_rotate(currItem->hatchAngle + 90);
		while (dist < lineLen)
		{
			PS_moveto(-lineLen, dist);
			PS_lineto(lineLen, dist);
			putColor(currItem->hatchForeground, 100, false);
			if (dist > 0)
			{
				PS_moveto(-lineLen, -dist);
				PS_lineto(lineLen, -dist);
				putColor(currItem->hatchForeground, 100, false);
			}
			dist += currItem->hatchDistance;
		}
		PS_restore();
	}
	dist = 0.0;
	if (currItem->hatchType == 2)
	{
		PS_save();
		PS_rotate(currItem->hatchAngle - 45);
		while (dist < lineLen)
		{
			PS_moveto(-lineLen, dist * sqrt(2.0));
			PS_lineto(lineLen, dist * sqrt(2.0));
			putColor(currItem->hatchForeground, 100, false);
			if (dist > 0)
			{
				PS_moveto(-lineLen, -dist * sqrt(2.0));
				PS_lineto(lineLen, -dist * sqrt(2.0));
				putColor(currItem->hatchForeground, 100, false);
			}
			dist += currItem->hatchDistance;
		}
		PS_restore();
	}
	PS_restore();
	if (currItem->lineColor() != CommonStrings::None)
	{
		SetColor(currItem->lineColor(), currItem->lineShade(), &h, &s, &v, &k);
		PS_setcmykcolor_stroke(h, s, v, k);
	}
}

void PSLib::drawArrow(PageItem *ite, QTransform &arrowTrans, int arrowIndex)
{
	double h, s, v, k;
	QVector<double> dum;
	FPointArray arrow = m_Doc->arrowStyles().at(arrowIndex-1).points.copy();
	if (ite->NamedLStyle.isEmpty())
	{
		if (ite->lineWidth() != 0.0)
			arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
	}
	else
	{
		multiLine ml = m_Doc->docLineStyles[ite->NamedLStyle];
		if (ml[ml.size() - 1].Width != 0.0)
			arrowTrans.scale(ml[ml.size() - 1].Width, ml[ml.size() - 1].Width);
	}
	arrow.map(arrowTrans);
	if (ite->NamedLStyle.isEmpty())
	{
		const ScPattern* strokePattern = m_Doc->checkedPattern(ite->strokePattern());
		if (strokePattern)
		{
			PS_newpath();
			SetClipPath(arrow);
			PS_closepath();
			QTransform patternMatrix;
			ScPattern *pat = &m_Doc->docPatterns[ite->strokePattern()];
			uint patHash = qHash(ite->strokePattern());
			const ScStrokePatternTransform& patternTrans = ite->strokePatternTransform();
			patternMatrix.translate(patternTrans.offsetX, -patternTrans.offsetY);
			patternMatrix.rotate(-patternTrans.rotation);
			patternMatrix.shear(patternTrans.skewX, -patternTrans.skewY);
			patternMatrix.scale(pat->scaleX, pat->scaleY);
			patternMatrix.scale(patternTrans.scaleX, patternTrans.scaleY);
			bool mirrorX, mirrorY;
			ite->strokePatternFlip(mirrorX, mirrorY);
			if (mirrorX)
				patternMatrix.scale(-1, 1);
			if (mirrorY)
				patternMatrix.scale(1, -1);
			PutStream("Pattern" + QString::number(patHash) + " [" + ToStr(patternMatrix.m11()) + " " + ToStr(patternMatrix.m12()) + " " + ToStr(patternMatrix.m21()) + " " + ToStr(patternMatrix.m22()) + " " + ToStr(patternMatrix.dx()) + " " + ToStr(patternMatrix.dy()) + "] makepattern setpattern\n");
			if (fillRule)
				PutStream("eofill\n");
			else
				PutStream("fill\n");
		}
		else if (ite->GrTypeStroke > 0)
		{
			PS_newpath();
			SetClipPath(arrow);
			PS_closepath();
			HandleGradientFillStroke(ite, true, true);
		}
		else if (ite->lineColor() != CommonStrings::None)
		{
			SetColor(ite->lineColor(), ite->lineShade(), &h, &s, &v, &k);
			PS_setcmykcolor_fill(h, s, v, k);
			PS_newpath();
			SetClipPath(arrow);
			PS_closepath();
			putColor(ite->lineColor(), ite->lineShade(), true);
		}
	}
	else
	{
		multiLine ml = m_Doc->docLineStyles[ite->NamedLStyle];
		if (ml[0].Color != CommonStrings::None)
		{
			SetColor(ml[0].Color, ml[0].Shade, &h, &s, &v, &k);
			PS_setcmykcolor_fill(h, s, v, k);
			PS_newpath();
			SetClipPath(arrow);
			PS_closepath();
			putColor(ite->lineColor(), ite->lineShade(), true);
		}
		for (int it = ml.size() - 1; it > 0; it--)
		{
			if (ml[it].Color != CommonStrings::None)
			{
				SetColor(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
				PS_setcmykcolor_stroke(h, s, v, k);
				PS_setlinewidth(ml[it].Width);
				PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
				PS_setdash(Qt::SolidLine, 0, dum);
				SetClipPath(arrow);
				PS_closepath();
				putColor(ml[it].Color, ml[it].Shade, false);
			}
		}
	}
}

void PSLib::SetColor(const QString& color, double shade, double *h, double *s, double *v, double *k)
{
	if (color == CommonStrings::None)
	{
		*h = 0;
		*s = 0;
		*v = 0;
		*k = 0;
		return;
	}
	const ScColor& col = m_Doc->PageColors[color];
	SetColor(col, shade, h, s, v, k);
}

void PSLib::SetColor(const ScColor& color, double shade, double *c, double *m, double *y, double *k)
{
	double c1 = *c, m1 = *m, y1 = *y, k1 = *k;
	ScColor tmp(color);
	if (color.getColorModel() != colorModelCMYK)
		tmp = ScColorEngine::convertToModel(color, m_Doc, colorModelCMYK);
	if ((Options.doGCR) && (!color.isRegistrationColor()))
		ScColorEngine::applyGCR(tmp, m_Doc);
	tmp.getCMYK(&c1, &m1, &y1, &k1);
	if (m_Doc->HasCMS && ScCore->haveCMS() && solidTransform)
	{
		c1 = c1 * shade / 100.0;
		m1 = m1 * shade / 100.0;
		y1 = y1 * shade / 100.0;
		k1 = k1 * shade / 100.0;
		unsigned short inC[4];
		unsigned short outC[4];
		inC[0] = c1 * 65535.0;
		inC[1] = m1 * 65535.0;
		inC[2] = y1 * 65535.0;
		inC[3] = k1 * 65535.0;
		solidTransform.apply(inC, outC, 1);
		*c = outC[0] / 65535.0;
		*m = outC[1] / 65535.0;
		*y = outC[2] / 65535.0;
		*k = outC[3] / 65535.0;
	}
	else
	{
		*c = c1 * shade / 100.0;
		*m = m1 * shade / 100.0;
		*y = y1 * shade / 100.0;
		*k = k1 * shade / 100.0;
	}
}

/**
 * @brief PSLib::setTextSt
 * @param ite   the text item to set
 * @param argh  current page number
 * @param pg    current page
 * @param master true if setting master page item
 */
void PSLib::setTextSt(PageItem* ite, uint argh, ScPage* pg, bool master)
{
	int savedOwnPage = ite->OwnPage;
	ite->OwnPage = argh;
	ite->layout();
	ite->OwnPage = savedOwnPage;

	PSPainter p(m_Doc, argh, pg, master, this);
	ite->textLayout.renderBackground(&p);
	ite->textLayout.render(&p);
}

void PSLib::putColor(const QString& colorName, double shade, bool fill)
{
	const ScColor& color(colorsToUse[colorName]);
	if (fill)
	{
		if (((color.isSpotColor()) || (color.isRegistrationColor())) && (Options.useSpotColors))
		{
			if (!DoSep)
				PS_fillspot(colorName, shade);
			else
			{
				if ((colorName == currentSpot) || (color.isRegistrationColor()))
				{
					if (fillRule)
						PutStream("0 0 0 " + ToStr(shade / 100.0) + " cmyk eofill\n");
					else
						PutStream("0 0 0 " + ToStr(shade / 100.0) + " cmyk fill\n");
				}
				else
				{
					if (fillRule)
						PutStream("0 0 0 0 cmyk eofill\n");
					else
						PutStream("0 0 0 0 cmyk fill\n");
				}
			}
		}
		else
		{
			if (!DoSep || (Plate == 0) || (Plate == 1) || (Plate == 2) || (Plate == 3))
				PS_fill();
			else if (fillRule)
				PutStream("0 0 0 0 cmyk eofill\n");
			else
				PutStream("0 0 0 0 cmyk fill\n");
		}
	}
	else
	{
		if (((color.isSpotColor()) || (color.isRegistrationColor())) && (Options.useSpotColors))
		{
			if (!DoSep)
				PS_strokespot(colorName, shade);
			else
			{
				if ((colorName == currentSpot) || (color.isRegistrationColor()))
					PutStream("0 0 0 " + ToStr(shade / 100.0) + " cmyk st\n");
			}
		}
		else
		{
			if (!DoSep || (Plate == 0) || (Plate == 1) || (Plate == 2) || (Plate == 3))
				PS_stroke();
			else
				PutStream("0 0 0 0 cmyk st\n");
		}
	}
}

void PSLib::putColorNoDraw(const QString& colorName, double shade)
{
	const ScColor& color(colorsToUse[colorName]);
	if (((color.isSpotColor()) || (color.isRegistrationColor())) && (Options.useSpotColors))
	{
		if (!DoSep)
			PutStream(ToStr(shade / 100.0) + " " + spotMap[colorName] + "\n");
		else if ((colorName == currentSpot) || (color.isRegistrationColor()))
			PutStream("0 0 0 " + ToStr(shade / 100.0) + " cmyk\n");
		else
			PutStream("0 0 0 0 cmyk\n");
	}
	else
	{
		double c = 0, m = 0, y = 0, k = 0;
		SetColor(color, shade, &c, &m, &y, &k);
		if (!DoSep || (Plate == 0 || Plate == 1 || Plate == 2 || Plate == 3))
			PutStream(ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k) + " cmyk\n");
		else
			PutStream("0 0 0 0 cmyk\n");
	}
}

void PSLib::GetBleeds(ScPage* page, double& left, double& right)
{
	MarginStruct values;
	m_Doc->getBleeds(page, Options.bleeds, values);
	left   = values.left();
	right  = values.right();
}

void PSLib::GetBleeds(ScPage* page, double& left, double& right, double& bottom, double& top)
{
	MarginStruct values;
	m_Doc->getBleeds(page, Options.bleeds, values);
	left   = values.left();
	right  = values.right();
	bottom = values.bottom();
	top    = values.top();
}

void PSLib::SetClipPath(const FPointArray &points, bool poly)
{
	FPoint np, np1, np2, np3, np4, firstP;
	bool nPath = true;
	bool first = true;
	if (points.size() <= 3)
		return;

	for (int poi=0; poi < points.size() - 3; poi += 4)
	{
		if (points.isMarker(poi))
		{
			nPath = true;
			continue;
		}
		if (nPath)
		{
			np = points.point(poi);
			if (!first && poly && (np4 == firstP))
				PS_closepath();
			PS_moveto(np.x(), -np.y());
			nPath = false;
			first = false;
			firstP = np;
			np4 = np;
		}
		np = points.point(poi);
		np1 = points.point(poi + 1);
		np2 = points.point(poi + 3);
		np3 = points.point(poi + 2);
		if ((np == np1) && (np2 == np3))
			PS_lineto(np3.x(), -np3.y());
		else
			PS_curve(np1.x(), -np1.y(), np2.x(), -np2.y(), np3.x(), -np3.y());
		np4 = np3;
	}
}

void PSLib::SetPathAndClip(const FPointArray &path, bool clipRule)
{
	if (path.size() > 3)
	{
		SetClipPath(path);
		PS_closepath();
		PS_clip(clipRule);
	}
}

const QString& PSLib::errorMessage()
{
	return ErrorMessage;
}

void PSLib::cancelRequested()
{
	abortExport = true;
}
