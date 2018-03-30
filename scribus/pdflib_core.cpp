/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  pdflib_core.cpp  -  description
							 -------------------
	begin                : Sat Jan 19 2002
	copyright            : (C) 2002 by Franz Schmid
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

#include "pdflib_core.h"

#include "scconfig.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <cstdlib>
#include <string>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "rc4.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QList>
#include <QPainterPath>
#include <QRect>
#include <QRegExp>
#include <QScopedPointer>
#include <QStack>
#include <QString>
#include <QTemporaryFile>
#include <QTextCodec>
#include <QtXml>
#include <QUuid>

#include "cmsettings.h"
#include "commonstrings.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "pageitem_group.h"
#include "pageitem_table.h"
#include "pdfoptions.h"
#include "prefscontext.h"
#include "prefsmanager.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scfonts.h"
#include "text/textlayoutpainter.h"
#include "fonts/cff.h"
#include "fonts/sfnt.h"
#include "scpage.h"
#include "scpaths.h"
#include "scpattern.h"

#include "scribuscore.h"
#include "scribusdoc.h"
#include "scstreamfilter_flate.h"
#include "scstreamfilter_rc4.h"
#include "tableutils.h"
#include "util.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_math.h"
#include "util_ghostscript.h"
#include "text/boxes.h"

#ifdef HAVE_OSG
	#include "third_party/prc/exportPRC.h"
#endif

#include "ui/bookmwin.h"
#include "ui/bookmarkpalette.h"
#include "ui/multiprogressdialog.h"

using namespace std;
using namespace TableUtils;

#if defined(_WIN32)
#undef GetObject
#endif

static inline QByteArray FToStr(double c)
{
	double v = c;
	if (fabs(c) < 0.0000001)
		v = 0.0;
	return QByteArray::number(v, 'f', 5);
}

class PdfPainter: public TextLayoutPainter
{
	QByteArray m_glyphBuffer;
	QByteArray m_pathBuffer;
//	PageItem* m_item;
	QMap<QString, PdfFont>  m_UsedFontsP;
	PDFLibCore *m_pdf;
	uint m_PNr;
	const ScPage* m_page;

	QByteArray m_prevFontName;
	int        m_prevFontSize;

	QByteArray transformToStr(QTransform tr)
	{
		return FToStr(tr.m11()) + " " + FToStr(-tr.m12()) + " " + FToStr(-tr.m21()) + " " + FToStr(tr.m22()) + " " + FToStr(tr.dx()) + " " + FToStr(-tr.dy());
	}

public:
	PdfPainter(PageItem *ite, PDFLibCore *pdf, uint num, const ScPage* pag) :
		m_glyphBuffer(),
		m_pathBuffer(),
//		m_item(ite),
		m_pdf(pdf),
		m_PNr(num),
		m_page(pag),
		m_prevFontSize(-1)
	{}

	~PdfPainter() {}

	void drawGlyph(const GlyphCluster& gc)
	{
		if (gc.isControlGlyphs() || gc.isEmpty())
			return;
			
		double current_x = 0.0;
		foreach (const GlyphLayout& gl, gc.glyphs()) {
			PdfFont pdfFont = m_pdf->UsedFontsP[font().replacementName()];
			QByteArray StrokeColor;
			QByteArray FillColor;

			if (strokeColor().color != CommonStrings::None)
				StrokeColor = m_pdf->putColor(strokeColor().color, strokeColor().shade, false);
			if (fillColor().color != CommonStrings::None)
				FillColor = m_pdf->putColor(fillColor().color, fillColor().shade, true);

			QTransform transform = matrix();
			if (pdfFont.method == Use_XForm)
			{
				if (!FillColor.isEmpty())
					m_pathBuffer += FillColor;

				m_pathBuffer += "q\n";
				m_pathBuffer += transformToStr(transform) + " cm\n";
				m_pathBuffer += FToStr(fontSize()) + " 0 0 " + FToStr(fontSize()) + " " + FToStr(x() + gl.xoffset + current_x) + " " + FToStr((y() - fontSize() + gl.yoffset) * -1) + " cm\n";

				if (gl.scaleV != 1.0)
					m_pathBuffer += "1 0 0 1 0 " + FToStr(((fontSize() - fontSize() * gl.scaleV) / fontSize()) * -1) + " cm\n";
				m_pathBuffer += FToStr(qMax(gl.scaleH, 0.1)) + " 0 0 " + FToStr(qMax(gl.scaleV, 0.1)) + " 0 0 cm\n";

				if (!FillColor.isEmpty())
					m_pathBuffer += pdfFont.name + "_gl" + Pdf::toPdf(gl.glyph) + " Do\n";
				m_pathBuffer += "Q\n";
			}
			else
			{
				uint gid = gl.glyph;
				uint fontNr = 65535;

				switch (pdfFont.encoding)
				{
				case  Encode_256:
					gid = pdfFont.glyphmap[gid];
					fontNr = gid / 256;
					gid = gid % 256;
					break;
				case Encode_224:
					fontNr = gid / 224;
					gid = gid % 224 + 32;
					break;
				case Encode_Subset:
					gid = pdfFont.glyphmap[gid];
					break;
				case Encode_IdentityH:
					break;
				}

				QByteArray pdfFontName = pdfFont.name;
				if (fontNr != 65535)
				{
					pdfFontName += "S";
					pdfFontName += Pdf::toPdf(fontNr);
				}

				if ((pdfFontName != m_prevFontName) || (fontSize() != m_prevFontSize))
				{
					m_glyphBuffer += pdfFontName + " " + FToStr(fontSize()) + " Tf\n";
					m_prevFontName = pdfFontName;
					m_prevFontSize = fontSize();
				}

				if (!StrokeColor.isEmpty())
					m_glyphBuffer += StrokeColor;

				if (!FillColor.isEmpty())
					m_glyphBuffer += FillColor;

				m_glyphBuffer += "0 Tr\n";

				transform.translate(x() + gl.xoffset + current_x , y() + gl.yoffset);
				transform.scale(qMax(gl.scaleH, 0.1), qMax(gl.scaleV, 0.1));
				m_glyphBuffer += transformToStr(transform) + " Tm\n";

				if (pdfFont.method != Use_Type3 || !FillColor.isEmpty())
				{
					switch (pdfFont.encoding)
					{
					case Encode_224:
					case Encode_256:
						m_glyphBuffer += Pdf::toHexString8(gid) + " Tj\n";
						break;
					case Encode_IdentityH:
					default:
						m_glyphBuffer += Pdf::toHexString16(gid) + " Tj\n";
						break;
					}
				}
			}
			current_x += gl.xadvance;
		}
	}

	void drawGlyphOutline(const GlyphCluster& gc, bool fill)
	{
		if (gc.isControlGlyphs() || gc.isEmpty())
			return;

		double current_x = 0.0;
		foreach (const GlyphLayout& gl, gc.glyphs()) {
			PdfFont pdfFont = m_pdf->UsedFontsP[font().replacementName()];
			QByteArray StrokeColor;
			QByteArray FillColor;
			QTransform transform = matrix();

			if (strokeColor().color != CommonStrings::None)
				StrokeColor = m_pdf->putColor(strokeColor().color, strokeColor().shade, false);
			if (fill && fillColor().color != CommonStrings::None)
				FillColor = m_pdf->putColor(fillColor().color, fillColor().shade, true);

			if (pdfFont.method == Use_XForm)
			{
				if (!StrokeColor.isEmpty())
				{
					m_pathBuffer += FToStr(strokeWidth() / fontSize()) + " w\n[] 0 d\n0 J\n0 j\n";
					m_pathBuffer += StrokeColor;
				}

				if (!FillColor.isEmpty())
					m_pathBuffer += FillColor;

				m_pathBuffer += "q\n";
				m_pathBuffer += transformToStr(transform) + " cm\n";
				m_pathBuffer += FToStr(fontSize()) + " 0 0 " + FToStr(fontSize()) + " " + FToStr(x() + gl.xoffset) + " " + FToStr((y() - fontSize() + gl.yoffset) * -1) + " cm\n";

				if (gc.scaleV() != 1.0)
					m_pathBuffer += "1 0 0 1 0 " + FToStr(((fontSize() - fontSize() * (gc.scaleV())) / fontSize()) * -1) + " cm\n";

				m_pathBuffer += FToStr(qMax(gc.scaleH(), 0.1)) + " 0 0 " + FToStr(qMax(gc.scaleV(), 0.1)) + " 0 0 cm\n";

				if (!FillColor.isEmpty())
					m_pathBuffer += pdfFont.name + "_gl" + Pdf::toPdf(gl.glyph) + " Do\n";

				FPointArray outline = font().glyphOutline(gl.glyph);
				QTransform mat;
				mat.scale(0.1, 0.1);
				outline.map(mat);
				bool nPath = true;
				FPoint np;
				if (outline.size() > 3)
				{
					for (int poi = 0; poi < outline.size() - 3; poi += 4)
					{
						if (outline.isMarker(poi))
						{
							m_pathBuffer += "h\n";
							nPath = true;
							continue;
						}

						if (nPath)
						{
							np = outline.point(poi);
							m_pathBuffer += FToStr(np.x()) + " " + FToStr(-np.y()) + " m\n";
							nPath = false;
						}

						np = outline.point(poi + 1);
						m_pathBuffer += FToStr(np.x()) + " " + FToStr(-np.y()) + " ";
						np = outline.point(poi + 3);
						m_pathBuffer += FToStr(np.x()) + " " + FToStr(-np.y()) + " ";
						np = outline.point(poi + 2);
						m_pathBuffer += FToStr(np.x()) + " " + FToStr(-np.y()) + " c\n";
					}
				}
				m_pathBuffer += "h s\n";
				m_pathBuffer += "Q\n";
			}
			else
			{
				if (!StrokeColor.isEmpty())
					m_pathBuffer += StrokeColor;

				uint gid = gl.glyph;
				uint fontNr = 65535;
				switch (pdfFont.encoding)
				{
				case  Encode_256:
					gid = pdfFont.glyphmap[gid];
					fontNr = gid / 256;
					gid = gid % 256;
					break;
				case Encode_224:
					fontNr = gid / 224;
					gid = gid % 224 + 32;
					break;
				case Encode_Subset:
					gid = pdfFont.glyphmap[gid];
					break;
				case Encode_IdentityH:
					break;
				}

				QByteArray pdfFontName = pdfFont.name;
				if (fontNr != 65535)
				{
					pdfFontName += "S";
					pdfFontName += Pdf::toPdf(fontNr);
				}

				if ((pdfFontName != m_prevFontName) || (fontSize() != m_prevFontSize))
				{
					m_glyphBuffer += pdfFontName + " " + FToStr(fontSize()) + " Tf\n";
					m_prevFontName = pdfFontName;
					m_prevFontSize = fontSize();
				}

				if (!StrokeColor.isEmpty())
					m_glyphBuffer += StrokeColor;

				if (!FillColor.isEmpty())
					m_glyphBuffer += FillColor;

				if (pdfFont.method == Use_Type3 && !StrokeColor.isEmpty())
				{
					m_pathBuffer += "q\n";
					m_pathBuffer += FToStr(strokeWidth()) + " w\n[] 0 d\n0 J\n0 j\n";

					transform.scale(fontSize(), fontSize());
					transform.translate(x() + gl.xoffset + current_x, y() + gl.yoffset);
					if (gc.scaleV() != 1.0)
						transform.translate(0, ((fontSize() - fontSize() * gc.scaleV()) / fontSize()) * -1);
					transform.scale(qMax(gc.scaleH(), 0.1), qMax(gc.scaleV(), 0.1));
					m_pathBuffer += transformToStr(transform) + " cm\n";

					/* paint outline */
					FPointArray outline = font().glyphOutline(gl.glyph);
					QTransform mat;
					mat.scale(0.1, 0.1);
					outline.map(mat);
					bool nPath = true;
					FPoint np;
					if (outline.size() > 3)
					{
						for (int poi = 0; poi < outline.size() - 3; poi += 4)
						{
							if (outline.isMarker(poi))
							{
								m_pathBuffer += "h\n";
								nPath = true;
								continue;
							}

							if (nPath)
							{
								np = outline.point(poi);
								m_pathBuffer += FToStr(np.x()) + " " + FToStr(-np.y()) + " m\n";
								nPath = false;
							}

							np = outline.point(poi + 1);
							m_pathBuffer += FToStr(np.x()) + " " + FToStr(-np.y()) + " ";
							np = outline.point(poi + 3);
							m_pathBuffer += FToStr(np.x()) + " " + FToStr(-np.y()) + " ";
							np = outline.point(poi + 2);
							m_pathBuffer += FToStr(np.x()) + " " + FToStr(-np.y()) + " c\n";
						}
					}
					m_pathBuffer += "h s\n";
					m_pathBuffer += "Q\n";
				}
				else
				{
					m_glyphBuffer += FToStr(strokeWidth()) + " w ";
					if (fill)
						m_glyphBuffer += "2 Tr\n";
					else
						m_glyphBuffer += "1 Tr\n";
				}

				transform.translate(x() + gl.xoffset + current_x, y() + gl.yoffset);
				transform.scale(qMax(gc.scaleH(), 0.1), qMax(gc.scaleV(), 0.1));
				m_glyphBuffer += transformToStr(transform) + " Tm\n";

				if (pdfFont.method != Use_Type3 || !FillColor.isEmpty())
				{
					switch (pdfFont.encoding)
					{
					case Encode_224:
					case Encode_256:
						m_glyphBuffer += Pdf::toHexString8(gid) + " Tj\n";
						break;
					case Encode_IdentityH:
					default:
						m_glyphBuffer += Pdf::toHexString16(gid) + " Tj\n";
						break;
					}
				}
			}
			current_x += gl.xadvance;
		}
	}

	void drawLine(QPointF start, QPointF end)
	{
		QTransform transform = matrix();
		transform.translate(x(), y());
		m_pathBuffer += "q\n";
		m_pathBuffer += transformToStr(transform) + " cm\n";
		m_pathBuffer += m_pdf->putColor(strokeColor().color, strokeColor().shade, false);
		m_pathBuffer += FToStr(strokeWidth())+" w\n";
		m_pathBuffer += FToStr(start.x()) + " " + FToStr(-start.y()) + " m\n";
		m_pathBuffer += FToStr(end.x()) + " " + FToStr(-end.y()) + " l\n";
		m_pathBuffer += "S\n";
		m_pathBuffer += "Q\n";
	}

	QByteArray getBuffer()
	{
		return "BT\n" + m_glyphBuffer + "ET\n" + m_pathBuffer;
	}

	void drawRect(QRectF rect)
	{
		QTransform transform = matrix();
//		transform.translate(x(), y());
		double rectX = x() + rect.x();
		double rectY = -y() - rect.y();
		m_glyphBuffer += "q\n";
		m_glyphBuffer += transformToStr(transform) + " cm\n";
		m_glyphBuffer += "n\n";
		m_glyphBuffer += m_pdf->putColor(fillColor().color, fillColor().shade, true);
		m_glyphBuffer += m_pdf->putColor(strokeColor().color, strokeColor().shade, false);
		m_glyphBuffer += FToStr(rectX) + " " + FToStr(rectY) + " m\n";
		m_glyphBuffer += FToStr(rectX + rect.width()) + " " + FToStr(rectY) + " l\n";
		m_glyphBuffer += FToStr(rectX + rect.width()) + " " + FToStr(rectY - rect.height()) + " l\n";
		m_glyphBuffer += FToStr(rectX) + " " + FToStr(rectY - rect.height()) + " l\n";
		m_glyphBuffer += "h\nf\n";
		m_glyphBuffer += "Q\n";
	}

	void drawObject(PageItem* embedded)
	{
		m_glyphBuffer += "ET\n"+m_pathBuffer;
		m_pathBuffer = "";

		m_pathBuffer += "q\n";
		m_pathBuffer += FToStr(scaleH()) + " 0 0 " + FToStr(scaleV()) + " " + FToStr(x() + embedded->gXpos) + " " + FToStr(-(y() + embedded->gYpos)) + " cm\n";

		QByteArray output;
		if (!m_pdf->PDF_ProcessItem(output, embedded, m_page, m_PNr, true))
			return;
		m_pathBuffer += output;
		m_pathBuffer += "Q\n";
		m_glyphBuffer += m_pathBuffer+"\n";
		m_pathBuffer = "";

		m_prevFontName.clear();
		m_prevFontSize = -1;
		m_glyphBuffer += "BT\n";
	}
};

PDFLibCore::PDFLibCore(ScribusDoc & docu)
	: QObject(&docu),
	doc(docu),
	ActPageP(0),
	Options(doc.pdfOptions()),
	Bvie(0),
	ucs2Codec(0),
//	ObjCounter(7),
	ResNam("RE"),
	ResCount(0),
	NDnam("LI"),
	NDnum(0),
//	KeyGen(""),
//	OwnerKey(""),
//	UserKey(""),
//	FileID(""),
//	EncryKey(""),
//	Encrypt(0),
//	KeyLen(5),
	colorsToUse(),
	spotNam("Spot"),
	spotCount(0),
	progressDialog(0),
	abortExport(false),
	usingGUI(ScCore->usingGUI()),
	bleedDisplacementX(0),
	bleedDisplacementY(0)
{
//	KeyGen.resize(32);
//	OwnerKey.resize(32);
//	UserKey.resize(32);
//	FileID.resize(16);
//	EncryKey.resize(5);
	Catalog.Outlines = 2;
	Catalog.PageTree = 3;
	Catalog.Dest = 4;
	Outlines.First = 0;
	Outlines.Last = 0;
	Outlines.Count = 0;
	pageData.ObjNum = 0;
	pageData.Thumb = 0;
//	int kg_array[] = {0x28, 0xbf, 0x4e, 0x5e, 0x4e, 0x75, 0x8a, 0x41, 0x64, 0x00, 0x4e, 0x56, 0xff, 0xfa,
//			  0x01, 0x08, 0x2e, 0x2e, 0x00, 0xb6, 0xd0, 0x68, 0x3e, 0x80, 0x2f, 0x0c, 0xa9, 0xfe,
//			  0x64, 0x53, 0x69, 0x7a};
//	for (int a = 0; a < 32; ++a)
//		KeyGen[a] = kg_array[a];
	if (usingGUI)
	{
		progressDialog = new MultiProgressDialog( tr("Saving PDF"), CommonStrings::tr_Cancel, doc.scMW());
		Q_CHECK_PTR(progressDialog);
		QStringList barNames, barTexts;
		barNames << "EMP" << "EP" << "ECPI";
		barTexts << tr("Exporting Master Page:") << tr("Exporting Page:") << tr("Exporting Items on Current Page:");
		QList<bool> barsNumeric;
		barsNumeric << true << true << false;
		progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
		connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
	}
}

PDFLibCore::~PDFLibCore()
{
	delete progressDialog;
}


bool PDFLibCore::PDF_IsPDFX()
{
	if (Options.Version == PDFOptions::PDFVersion_X1a)
		return true;
	if (Options.Version == PDFOptions::PDFVersion_X3)
		return true;
	if (Options.Version == PDFOptions::PDFVersion_X4)
		return true;
	return false;
}

bool PDFLibCore::PDF_IsPDFX(PDFOptions::PDFVersion ver)
{
	if (ver == PDFOptions::PDFVersion_X1a)
		return true;
	if (ver == PDFOptions::PDFVersion_X3)
		return true;
	if (ver == PDFOptions::PDFVersion_X4)
		return true;
	return false;
}

bool PDFLibCore::doExport(const QString& fn, const QString& nam, int Components,
					  const std::vector<int> & pageNs, const QMap<int, QImage> & thumbs)
{
	QImage thumb;
	bool ret = false, error = false;
	int  pc_exportpages=0;
	int  pc_exportmasterpages=0;
	if (usingGUI)
		progressDialog->show();
	QMap<QString, QMap<uint, FPointArray> > usedFonts;
	usedFonts.clear();
	doc.getUsedFonts(usedFonts);
	ucs2Codec = QTextCodec::codecForName("ISO-10646-UCS-2");
	if (!ucs2Codec)
		ucs2Codec = QTextCodec::codecForName("UTF-16");
	if (!ucs2Codec)
	{
		PDF_Error( tr("Qt build miss both \"UTF-16\" and \"ISO-10646-UCS-2\" text codecs, pdf export is not possible") );
		return false;
	}
	if (PDF_Begin_Doc(fn, PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts, usedFonts, doc.scMW()->bookmarkPalette->BView))
	{
		QMap<int, int> pageNsMpa;
		for (uint a = 0; a < pageNs.size(); ++a)
		{
			pageNsMpa.insert(doc.MasterNames[doc.DocPages.at(pageNs[a]-1)->MPageNam], 0);
		}
		if (usingGUI)
		{
			progressDialog->setOverallTotalSteps(pageNsMpa.count()+pageNs.size());
			progressDialog->setTotalSteps("EMP", pageNsMpa.count());
			progressDialog->setTotalSteps("EP", pageNs.size());
			progressDialog->setOverallProgress(0);
			progressDialog->setProgress("EMP", 0);
			progressDialog->setProgress("EP", 0);
		}
		for (int ap = 0; ap < doc.MasterPages.count() && !abortExport; ++ap)
		{
			if (doc.MasterItems.count() != 0)
			{
				if (pageNsMpa.contains(ap))
				{
					qApp->processEvents();
					if (!PDF_TemplatePage(doc.MasterPages.at(ap)))
						error = abortExport = true;
					++pc_exportmasterpages;
				}
			}
			if (usingGUI)
			{
				progressDialog->setProgress("EMP", pc_exportmasterpages);
				progressDialog->setOverallProgress(pc_exportmasterpages+pc_exportpages);
			}
		}
		for (uint a = 0; a < pageNs.size() && !abortExport; ++a)
		{
			if (doc.pdfOptions().Thumbnails)
				thumb = thumbs[pageNs[a]];
			qApp->processEvents();
			if (abortExport) break;

			PDF_Begin_Page(doc.DocPages.at(pageNs[a]-1), thumb);
			qApp->processEvents();
			if (abortExport) break;

			if (!PDF_ProcessPage(doc.DocPages.at(pageNs[a]-1), pageNs[a]-1, doc.pdfOptions().doClip))
				error = abortExport = true;
			qApp->processEvents();
			if (abortExport) break;

			PDF_End_Page();
			pc_exportpages++;
			if (usingGUI)
			{
				progressDialog->setProgress("EP", pc_exportpages);
				progressDialog->setOverallProgress(pc_exportmasterpages+pc_exportpages);
			}
		}
		ret = true;//Even when aborting we return true. Don't want that "couldnt write msg"
		if (!abortExport)
		{
			if (PDF_IsPDFX(doc.pdfOptions().Version))
				ret = PDF_End_Doc(ScCore->PrinterProfiles[doc.pdfOptions().PrintProf], nam, Components);
			else
				ret = PDF_End_Doc();
		}
		else
			closeAndCleanup();
	}
	if (usingGUI)
		progressDialog->close();
	return (ret && !error);
}

const QString& PDFLibCore::errorMessage(void) const
{
	return ErrorMessage;
}

bool PDFLibCore::exportAborted(void) const
{
	return abortExport;
}

//#define StartObj(n) writer.startObj((n))
#define PutDoc(s) writer.write(s)
//#define newObject() writer.newObject()


//void PDFLibCore::StartObj(PdfId nr)
//{
//	for (int i=XRef.size(); i < nr; ++i)
//		XRef.append(0);
//	XRef[nr-1] = bytesWritten();
//	PutDoc(Pdf::toPdf(nr)+ " 0 obj\n");
//}

//// Encode a string for inclusion in a
//// PDF (literal) .
//static QByteArray PDFEncode(const QString & in)
//{
//	QByteArray tmp("");
//	for (int d = 0; d < in.length(); ++d)
//	{
//		QChar cc(in.at(d));
//		if ((cc == '(') || (cc == ')') || (cc == '\\'))
//			tmp += '\\';
//		else if ((cc == '\r') || (cc == '\n'))
//		{
//			tmp += (cc == '\r') ? "\\r" : "\\n";
//			continue;
//		}
//		tmp += cc;
//	}
//	return tmp;
//}

static QByteArray blendMode(int code)
{
	switch (code)
	{
		case 0:
			return("Normal");
			break;
		case 1:
			return("Darken");
			break;
		case 2:
			return("Lighten");
			break;
		case 3:
			return("Multiply");
			break;
		case 4:
			return("Screen");
			break;
		case 5:
			return("Overlay");
			break;
		case 6:
			return("HardLight");
			break;
		case 7:
			return("SoftLight");
			break;
		case 8:
			return("Difference");
			break;
		case 9:
			return("Exclusion");
			break;
		case 10:
			return("ColorDodge");
			break;
		case 11:
			return("ColorBurn");
			break;
		case 12:
			return("Hue");
			break;
		case 13:
			return("Saturation");
			break;
		case 14:
			return("Color");
			break;
		case 15:
			return("Luminosity");
			break;
		default:
			return "";
	}
}
//
//QByteArray PDFLibCore::EncodeUTF16(const QString &in)
//{
//	QString tmp = in;
//	QByteArray cres = ucs2Codec->fromUnicode( tmp );
//#ifndef WORDS_BIGENDIAN
//	// on little endian systems we need to swap bytes:
//	uchar sw;
//	for(int d = 0; d < cres.size()-1; d += 2)
//	{
//		sw = cres[d];
//		cres[d] = cres[d+1];
//		cres[d+1] = sw;
//	}
//#endif
//	return cres;
//}

QByteArray PDFLibCore::EncStream(const QByteArray & in, PdfId ObjNum)
{
	if (in.length() < 1)
		return QByteArray();
	else if (!Options.Encrypt)
		return in;
	else
		return writer.encryptBytes(in, ObjNum);
//	rc4_context_t rc4;
//	QByteArray tmp(in);
//	QByteArray us(tmp.length(), ' ');
//	QByteArray ou(tmp.length(), ' ');
//	for (int a = 0; a < tmp.length(); ++a)
//		us[a] = (tmp.at(a));
//	QByteArray step1 = writer.ComputeRC4Key(ObjNum);
//	rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), qMin(KeyLen+5, 16));
//	rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()), reinterpret_cast<uchar*>(ou.data()), tmp.length());
//	QString uk = "";
//	for (int cl = 0; cl < tmp.length(); ++cl)
//		uk += QChar(ou[cl]);
//	return ou;
}

QByteArray PDFLibCore::EncString(const QByteArray & in, PdfId ObjNum)
{
	if (in.length() < 1)
		return "<>";
	if (!Options.Encrypt)
	{
		//tmp = "(" + PDFEncode(in) + ")";
		return Pdf::toLiteralString(in);
	}
	
//	rc4_context_t rc4;
//	QByteArray us(in.length(), ' ');
//	QByteArray ou(in.length(), ' ');
//	for (int a = 0; a < in.length(); ++a)
//		us[a] = static_cast<uchar>(QChar(in.at(a)).cell());
//	QByteArray step1 = writer.ComputeRC4Key(ObjNum);
//	rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), qMin(KeyLen+5, 16));
//	rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()), reinterpret_cast<uchar*>(ou.data()), in.length());
//	QString uk = "";
//	for (int cl = 0; cl < in.length(); ++cl)
//		uk += QChar(ou[cl]);
	
	return Pdf::toHexString(writer.encryptBytes(in, ObjNum));
}

QByteArray PDFLibCore::EncStringUTF16(const QString & in, PdfId ObjNum)
{
	if (in.length() < 1)
		return "<>";
	if (!Options.Encrypt)
	{
		QByteArray us = Pdf::toUTF16(in);
//		QString uk = "";
//		for (int cl = 0; cl < us.size(); ++cl)
//			uk += QChar(us[cl]);
		return Pdf::toHexString(us);
	}
//	rc4_context_t rc4;
	QByteArray us = Pdf::toUTF16(in);
//	QByteArray ou(us.size(), ' ');
//	QByteArray step1 = writer.ComputeRC4Key(ObjNum);
//	rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), qMin(KeyLen+5, 16));
//	rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()), reinterpret_cast<uchar*>(ou.data()), ou.size());
//	QString uk = "";
//	for (int cl = 0; cl < ou.size(); ++cl)
//		uk += QChar(ou[cl]);
	QByteArray tmp = Pdf::toHexString(writer.encryptBytes(us, ObjNum));
	return tmp;
}

bool PDFLibCore::EncodeArrayToStream(const QByteArray& in, PdfId ObjNum)
{
	if (in.size() < 1)
		return true;
	bool succeed = false;
	if (Options.Encrypt)
	{
		ScStreamFilter* rc4Encode = writer.openStreamFilter(true, ObjNum);
		if (rc4Encode->openFilter())
		{
			succeed  = rc4Encode->writeData(in.data(), in.size());
			succeed &= rc4Encode->closeFilter();
		}
		delete rc4Encode;
	}
	else
		writer.write(in);
	return (writer.getOutStream().status() == QDataStream::Ok);
}

int PDFLibCore::WriteImageToStream(ScImage& image, PdfId ObjNum, ColorSpaceEnum format, bool precal)
{
	bool fromCmyk, succeed = false;
	int  bytesWritten = 0;

	ScStreamFilter* rc4Encode = writer.openStreamFilter(Options.Encrypt, ObjNum);
	if (rc4Encode->openFilter())
	{
		switch (format)
		{
			case ColorSpaceMonochrome :
				fromCmyk = !Options.UseRGB && !Options.isGrayscale && !(doc.HasCMS && Options.UseProfiles2);
				succeed = image.writeMonochromeDataToFilter(rc4Encode, fromCmyk); break;
			case ColorSpaceGray :
				succeed = image.writeGrayDataToFilter(rc4Encode, precal); break;
			case ColorSpaceCMYK :
				succeed = image.writeCMYKDataToFilter(rc4Encode); break;
			default :
				succeed = image.writeRGBDataToFilter(rc4Encode); break;
		}
		succeed &= rc4Encode->closeFilter();
		bytesWritten = rc4Encode->writtenToStream();
		delete rc4Encode;
	}
	return (succeed ? bytesWritten : 0);
}

int PDFLibCore::WriteJPEGImageToStream(ScImage& image, const QString& fn, PdfId ObjNum, int quality, ColorSpaceEnum format,
										 bool sameFile, bool precal)
{
	bool succeed = true;
	int  bytesWritten = 0;
	QFileInfo fInfo(fn);
	QString   ext = fInfo.suffix().toLower();
	QString   jpgFileName, tmpFile;
	if (extensionIndicatesJPEG(ext) && sameFile)
		jpgFileName = fn;
	else
	{
		tmpFile  = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc.jpg");
		if (format == ColorSpaceGray && (!precal))
			image.convertToGray();
		if (image.convert2JPG(tmpFile, quality, format == ColorSpaceCMYK, format == ColorSpaceGray))
			jpgFileName = tmpFile;
	}
	if (jpgFileName.isEmpty())
		return 0;
	if (Options.Encrypt)
	{
		succeed = false;
		ScStreamFilter* rc4Encode = writer.openStreamFilter(true, ObjNum);
		if (rc4Encode->openFilter())
		{
			succeed  = copyFileToFilter(jpgFileName, *rc4Encode);
			succeed &= rc4Encode->closeFilter();
			bytesWritten = rc4Encode->writtenToStream();
		}
		delete rc4Encode;
	}
	else
	{
		succeed &= copyFileToStream(jpgFileName, writer.getOutStream());
		QFileInfo jpgInfo(jpgFileName);
		bytesWritten = jpgInfo.size();
	}
	if (!tmpFile.isEmpty() && QFile::exists(tmpFile))
		QFile::remove(tmpFile);
	return (succeed ? bytesWritten : 0);
}

int PDFLibCore::WriteFlateImageToStream(ScImage& image, PdfId ObjNum, ColorSpaceEnum format, bool precal)
{
	bool fromCmyk, succeed = false;
	int  bytesWritten = 0;
	ScStreamFilter* rc4Encode = writer.openStreamFilter(Options.Encrypt, ObjNum);
	ScFlateEncodeFilter flateEncode(rc4Encode);
	if (flateEncode.openFilter())
	{
		switch (format)
		{
			case ColorSpaceMonochrome :
				fromCmyk = !Options.UseRGB && !Options.isGrayscale && !(doc.HasCMS && Options.UseProfiles2);
				succeed = image.writeMonochromeDataToFilter(&flateEncode, fromCmyk); break;
			case ColorSpaceGray :
				succeed = image.writeGrayDataToFilter(&flateEncode, precal); break;
			case ColorSpaceCMYK :
				succeed = image.writeCMYKDataToFilter(&flateEncode); break;
			default :
				succeed = image.writeRGBDataToFilter(&flateEncode); break;
		}
		succeed &= flateEncode.closeFilter();
		bytesWritten = flateEncode.writtenToStream();
	}
	delete rc4Encode;
	return (succeed ? bytesWritten : 0);
}



bool PDFLibCore::PDF_Begin_Doc(const QString& fn, SCFonts &AllFonts, const QMap<QString, QMap<uint, FPointArray> >& DocFonts, BookMView* vi)
{
	if (!writer.open(fn))
		return false;
	
	inPattern = 0;
	Bvie = vi;
	BookMinUse = false;
	UsedFontsP.clear();
	UsedFontsF.clear();
	
	writer.writeHeader(Options.Version);
	
//	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
//		ObjCounter = 10;
//	else
//		ObjCounter = 9;
//	switch (Options.Version)
//	{
//		case PDFOptions::PDFVersion_X1a:
//		case PDFOptions::PDFVersion_X3:
//		case PDFOptions::PDFVersion_13:
//			PutDoc("%PDF-1.3\n");
//			break;
//		case PDFOptions::PDFVersion_14:
//			PutDoc("%PDF-1.4\n");
//			break;
//		case PDFOptions::PDFVersion_X4:
//		case PDFOptions::PDFVersion_15:
//			PutDoc("%PDF-1.5\n");
//			break;
//	}
//	if (PDF_IsPDFX())
//		ObjCounter++;
//	PutDoc("%\xc7\xec\x8f\xa2\n");

	PDF_Begin_Catalog();
	PDF_Begin_MetadataAndEncrypt();
	PDF_Begin_WriteUsedFonts(AllFonts, PDF_Begin_FindUsedFonts(AllFonts, DocFonts));
	PDF_Begin_Colors();
	PDF_Begin_Layers();
	
	return true;
}

void PDFLibCore::PDF_Begin_Catalog()
{
	writer.startObj(writer.CatalogObj);
	PutDoc("<<\n/Type /Catalog\n");
	PutDoc("/Pages " + Pdf::toObjRef(writer.PagesObj) + "\n");
	if (!PDF_IsPDFX())
	{
		if (doc.useAcroFormFields())
		{
			writer.AcroFormObj = writer.newObject();
			PutDoc("/AcroForm " + Pdf::toObjRef(writer.AcroFormObj) + "\n");
		}
		if (doc.useAnnotations())
		{
			writer.DestsObj = writer.newObject();
			PutDoc("/Dests " + Pdf::toObjRef(writer.DestsObj) + "\n");
		}
		if (doc.JavaScripts.count() > 0)
		{
			writer.NamesObj = writer.newObject();
			PutDoc("/Names " + Pdf::toObjRef(writer.NamesObj) + "\n");
		}
	}
	if (Options.Bookmarks)
	{
		writer.OutlinesObj = writer.newObject();
		PutDoc("/Outlines " + Pdf::toObjRef(writer.OutlinesObj) + "\n");
	}
	if (Options.Articles)
	{
		writer.ThreadsObj = writer.newObject();
		PutDoc("/Threads " + Pdf::toObjRef(writer.ThreadsObj) + "\n");
	}
	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
	{
		writer.OCPropertiesObj = writer.newObject();
		PutDoc("/OCProperties " + Pdf::toObjRef(writer.OCPropertiesObj) + "\n");
	}
	if (PDF_IsPDFX())
	{
		writer.OutputIntentObj = writer.newObject();
		PutDoc("/OutputIntents [ " +  Pdf::toObjRef(writer.OutputIntentObj) + " ]\n");
	}
	if ((Options.Version == PDFOptions::PDFVersion_X4))
	{
		writer.MetaDataObj = writer.newObject();
		PutDoc("/Metadata "+ Pdf::toObjRef(writer.MetaDataObj) + "\n");
	}
	PutDoc("/PageLayout ");
	switch (Options.PageLayout)
	{
		case PDFOptions::SinglePage:
			PutDoc("/SinglePage\n");
			break;
		case PDFOptions::OneColumn:
			PutDoc("/OneColumn\n");
			break;
		case PDFOptions::TwoColumnLeft:
			PutDoc("/TwoColumnLeft\n");
			break;
		case PDFOptions::TwoColumnRight:
			PutDoc("/TwoColumnRight\n");
			break;
	}
	if (Options.displayBookmarks)
		PutDoc("/PageMode /UseOutlines\n");
	else if (Options.displayFullscreen)
		PutDoc("/PageMode /FullScreen\n");
	else if (Options.displayThumbs)
		PutDoc("/PageMode /UseThumbs\n");
	else if ((Options.Version == PDFOptions::PDFVersion_15) && (Options.displayLayers))
			PutDoc("/PageMode /UseOC\n");
	if (!Options.openAction.isEmpty())
	{
		writer.OpenActionObj = writer.newObject();
		PutDoc("/OpenAction << /S /JavaScript /JS " + Pdf::toPdf(writer.OpenActionObj) + " 0 R >>\n");
	}
	QDateTime dt = QDateTime::currentDateTimeUtc();
	Datum = Pdf::toDateString(dt);
//    "D:";
//	tmp.sprintf("%4d", d.year());
//	tmp.replace(QRegExp(" "), "0");
//	Datum += tmp;
//	tmp.sprintf("%2d", d.month());
//	tmp.replace(QRegExp(" "), "0");
//	Datum += tmp;
//	tmp.sprintf("%2d", d.day());
//	tmp.replace(QRegExp(" "), "0");
//	Datum += tmp;
//	tmp = dt.time().toString();
//	tmp.replace(QRegExp(":"), "");
//	Datum += tmp;
//	Datum += "Z";

	// only include XMP to PDF/X-4 at the moment, could easily be extended to include it to any PDF
	if (Options.Version == PDFOptions::PDFVersion_X4)
		generateXMP(dt.toString("yyyy-MM-ddThh:mm:ssZ"));

/* The following code makes the resulting PDF "Reader enabled" in Acrobat Reader 8
   but sadly it doesn't work with newer version, because its based on a bug in AR 8
	PutDoc("/Perms\n");
	PutDoc("<<\n");
	PutDoc("/UR3\n");
	PutDoc("<<\n");
	PutDoc("/M ("+Datum+")\n");
	PutDoc("/Name (Scribus "+QString(VERSION)+")\n");
	PutDoc("/Reference [\n");
	PutDoc("<<\n");
	PutDoc("/TransformParams\n");
	PutDoc("<<\n");
	PutDoc("/Type /TransformParams\n");
	PutDoc("/V /2.2\n");
	PutDoc("/Document [/FullSave]\n");
	PutDoc("/Annots [/Create/Delete/Modify/Copy/Import/Export]\n");
	PutDoc("/Form [/Add/Delete/FillIn/Import/Export/SubmitStandalone/SpawnTemplate]\n");
	PutDoc("/Signature [/Modify]\n");
	PutDoc(">>\n");
	PutDoc("/TransformMethod /UR3\n");
	PutDoc("/Type /SigRef\n");
	PutDoc(">>\n");
	PutDoc("]\n");
	PutDoc("/Type /Sig\n");
	PutDoc(">>\n");
	PutDoc(">>\n");
*/

	PutDoc("/ViewerPreferences\n<<\n/PageDirection ");
	PutDoc( Options.Binding == 0 ? "/L2R\n" : "/R2L\n");
	if (Options.hideToolBar)
		PutDoc("/HideToolbar true\n");
	if (Options.hideMenuBar)
		PutDoc("/HideMenubar true\n");
	if (Options.fitWindow)
		PutDoc("/FitWindow true\n");
	PutDoc(" >>\n>>");
	writer.endObj(writer.CatalogObj);
}

void PDFLibCore::PDF_Begin_MetadataAndEncrypt()
{
	QString IDg(Datum);
	IDg += Options.fileName;
	IDg += "Scribus "+QString(VERSION);
	IDg += "Scribus PDF Library "+QString(VERSION);
	IDg += doc.documentInfo().title();
	IDg += doc.documentInfo().author();
	IDg += "/False";
	writer.setFileId(Pdf::toPdfDocEncoding(IDg));
	if (Options.Encrypt)
	{
		writer.setEncryption((Options.Version == PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_15), Pdf::toPdfDocEncoding(Options.PassOwner), Pdf::toPdfDocEncoding(Options.PassUser), Options.Permissions);
	}
	writer.startObj(writer.InfoObj);
	PutDoc("<<\n/Creator " + EncString(QByteArray("Scribus ") + VERSION, writer.InfoObj) + "\n");
	PutDoc("/Producer " + EncString(QByteArray("Scribus PDF Library ") + VERSION, writer.InfoObj) + "\n");
	QString docTitle = doc.documentInfo().title();
	if ((PDF_IsPDFX()) && (docTitle.isEmpty()))
		PutDoc("/Title " + EncStringUTF16(doc.DocName, writer.InfoObj) + "\n");
	else
		PutDoc("/Title " + EncStringUTF16(doc.documentInfo().title(), writer.InfoObj) + "\n");
	PutDoc("/Author " + EncStringUTF16(doc.documentInfo().author(), writer.InfoObj) + "\n");
	PutDoc("/Subject " + EncStringUTF16(doc.documentInfo().subject(), writer.InfoObj) + "\n");
	PutDoc("/Keywords " + EncStringUTF16(doc.documentInfo().keywords(), writer.InfoObj) + "\n");
	PutDoc("/CreationDate " + EncString(Datum, writer.InfoObj) + "\n");
	PutDoc("/ModDate " + EncString(Datum, writer.InfoObj) + "\n");
	if (Options.Version == PDFOptions::PDFVersion_X1a)
	{
		PutDoc("/GTS_PDFXVersion (PDF/X-1:2001)\n");
		PutDoc("/GTS_PDFXConformance (PDF/X-1a:2001)\n");
	}
	if (Options.Version == PDFOptions::PDFVersion_X3)
		PutDoc("/GTS_PDFXVersion (PDF/X-3:2002)\n");
	if (Options.Version == PDFOptions::PDFVersion_X4)
		PutDoc("/GTS_PDFXVersion (PDF/X-4)\n");
	PutDoc("/Trapped /False\n>>");
	writer.endObj(writer.InfoObj);
}

QMap<QString, QMap<uint, FPointArray> >
PDFLibCore::PDF_Begin_FindUsedFonts(SCFonts &AllFonts, const QMap<QString, QMap<uint, FPointArray> >& DocFonts)
{
	QMap<QString, QMap<uint, FPointArray> > ReallyUsed;
	ReallyUsed.clear();
	PageItem* pgit;
	QMap<int, QByteArray> ind2PDFabr;
	const QByteArray tmpf[] = {"/Courier", "/Courier-Bold", "/Courier-Oblique", "/Courier-BoldOblique",
												"/Helvetica", "/Helvetica-Bold", "/Helvetica-Oblique", "/Helvetica-BoldOblique",
												"/Times-Roman", "/Times-Bold", "/Times-Italic", "/Times-BoldItalic",
												"/ZapfDingbats", "/Symbol"};
	size_t ar = sizeof(tmpf) / sizeof(*tmpf);
	for (uint ax = 0; ax < ar; ++ax)
		ind2PDFabr[ax] = tmpf[ax];

	QList<PageItem*> allItems = doc.FrameItems.values();
	while (allItems.count() > 0)
	{
		pgit = allItems.takeFirst();
		if (pgit->isGroup() || pgit->isTable())
		{
			allItems = pgit->getChildren() + allItems;
			continue;
		}
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= Annotation::Button) && (annotType <= Annotation::Listbox) && (annotType != Annotation::Checkbox));
				if ((pgit->annotation().Type() == Annotation::Checkbox) || (pgit->annotation().Type() == Annotation::RadioButton))
					StdFonts.insert("/ZapfDingbats", "");
				if (pgit->itemText.length() > 0 || mustEmbed)
				{
					if (Options.Version < PDFOptions::PDFVersion_14)
						StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
					QString replacementName = pgit->itemText.defaultStyle().charStyle().font().replacementName();
					ReallyUsed.insert(replacementName, DocFonts[replacementName]);
				}
			}
			uint start = pgit->isTextFrame() ? (uint) pgit->firstInFrame() : 0;
			uint stop  = pgit->isTextFrame() ? (uint) pgit->lastInFrame() + 1 : (uint) pgit->itemText.length();
			for (uint e = start; e < stop; ++e)
			{
				QString replacementName = pgit->itemText.charStyle(e).font().replacementName();
				ReallyUsed.insert(replacementName, DocFonts[replacementName]);
			}
		}
	}

	allItems = doc.MasterItems;
	while (allItems.count() > 0)
	{
		pgit = allItems.takeFirst();
		if (pgit->isGroup() || pgit->isTable())
		{
			allItems = pgit->getChildren() + allItems;
			continue;
		}
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= Annotation::Button) && (annotType <= Annotation::Listbox) && (annotType != Annotation::Checkbox));
				if ((pgit->annotation().Type() == Annotation::Checkbox) || (pgit->annotation().Type() == Annotation::RadioButton))
					StdFonts.insert("/ZapfDingbats", "");
				if (pgit->itemText.length() > 0 || mustEmbed)
				{
					if (Options.Version < PDFOptions::PDFVersion_14)
						StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
					QString replacementName = pgit->itemText.defaultStyle().charStyle().font().replacementName();
					ReallyUsed.insert(replacementName, DocFonts[replacementName]);
				}
			}
			uint start = pgit->isTextFrame() ? (uint) pgit->firstInFrame() : 0;
			uint stop  = pgit->isTextFrame() ? (uint) pgit->lastInFrame() + 1 : (uint) pgit->itemText.length();
			for (uint e = start; e < stop; ++e)
			{
				QString replacementName = pgit->itemText.charStyle(e).font().replacementName();
				ReallyUsed.insert(replacementName, DocFonts[replacementName]);
			}
		}
	}

	allItems = *(doc.Items);
	while (allItems.count() > 0)
	{
		pgit = allItems.takeFirst();
		if (pgit->isGroup() || pgit->isTable())
		{
			allItems = pgit->getChildren() + allItems;
			continue;
		}
		if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
		{
			if (pgit->isAnnotation())
			{
				int annotType  = pgit->annotation().Type();
				bool mustEmbed = ((annotType >= Annotation::Button) && (annotType <= Annotation::Listbox) && (annotType != Annotation::Checkbox));
				if ((pgit->annotation().Type() == Annotation::Checkbox) || (pgit->annotation().Type() == Annotation::RadioButton))
					StdFonts.insert("/ZapfDingbats", "");
				if (pgit->itemText.length() > 0 || mustEmbed)
				{
					if (Options.Version < PDFOptions::PDFVersion_14)
						StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
					QString replacementName = pgit->itemText.defaultStyle().charStyle().font().replacementName();
					ReallyUsed.insert(replacementName, DocFonts[replacementName]);
				}
			}
			uint start = pgit->isTextFrame() ? (uint) pgit->firstInFrame() : 0;
			uint stop  = pgit->isTextFrame() ? (uint) pgit->lastInFrame() + 1 : (uint) pgit->itemText.length();
			for (uint e = start; e < stop; ++e)
			{
				QString replacementName = pgit->itemText.charStyle(e).font().replacementName();
				ReallyUsed.insert(replacementName, DocFonts[replacementName]);
			}
		}
	}
/*	if (Options.docInfoMarks)
	{
		StdFonts.insert("/Helvetica", "");
	} */
	QStringList patterns = doc.getPatternDependencyList(doc.getUsedPatterns());
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = doc.docPatterns[patterns[c]];
		allItems = pa.items;
		while (allItems.count() > 0)
		{
			pgit = allItems.takeFirst();
			if (pgit->isGroup() || pgit->isTable())
			{
				allItems = pgit->getChildren() + allItems;
				continue;
			}
			if ((pgit->itemType() == PageItem::TextFrame) || (pgit->itemType() == PageItem::PathText))
			{
				if (pgit->isAnnotation())
				{
					if ((pgit->annotation().Type() == Annotation::Checkbox) || (pgit->annotation().Type() == Annotation::RadioButton))
						StdFonts.insert("/ZapfDingbats", "");
					if (pgit->itemText.length() > 0)
					{
						if (Options.Version < PDFOptions::PDFVersion_14)
							StdFonts.insert(ind2PDFabr[pgit->annotation().Font()], "");
						QString replacementName = pgit->itemText.defaultStyle().charStyle().font().replacementName();
						ReallyUsed.insert(replacementName, DocFonts[replacementName]);
					}
				}
				uint start = pgit->isTextFrame() ? (uint) pgit->firstInFrame() : 0;
				uint stop  = pgit->isTextFrame() ? (uint) pgit->lastInFrame() + 1 : (uint) pgit->itemText.length();
				for (uint e = start; e < stop; ++e)
				{
					QString replacementName = pgit->itemText.charStyle(e).font().replacementName();
					ReallyUsed.insert(replacementName, DocFonts[replacementName]);
				}
			}
		}
	}
	return ReallyUsed;
}

static QByteArray sanitizeFontName(QString fn)
{
	return Pdf::toPdfDocEncoding(fn.replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ));
}

static QList<Pdf::Resource> asColorSpace(QList<PdfICCD> iccCSlist)
{
	QList<Pdf::Resource> result;
	foreach (const Pdf::Resource& r, iccCSlist)
		result.append(r);
	return result;
}

static QList<Pdf::Resource> asColorSpace(QList<PdfSpotC> spotMapValues)
{
	QList<Pdf::Resource> result;
	foreach (const Pdf::Resource& r, spotMapValues)
		result.append(r);
	return result;
}

void PDFLibCore::PDF_WriteStandardFonts()
{
	int a = 0;
	QMap<QString, QString>::Iterator itStd;
	for (itStd = StdFonts.begin(); itStd != StdFonts.end(); ++itStd)
	{
		PdfId fontObject = writer.newObject();
		writer.startObj(fontObject);
		PutDoc("<<\n/Type /Font\n/Subtype /Type1\n");
		PutDoc("/Name /FoStd"+Pdf::toPdf(a)+"\n");
		PutDoc("/BaseFont "+Pdf::toPdfDocEncoding(itStd.key())+"\n");
		if (itStd.key() != "/ZapfDingbats")
		{
			PutDoc("/Encoding << \n");
			PutDoc("/Differences [ \n");
			PutDoc("24 /breve /caron /circumflex /dotaccent /hungarumlaut /ogonek /ring /tilde\n");
			PutDoc("39 /quotesingle 96 /grave 128 /bullet /dagger /daggerdbl /ellipsis /emdash /endash /florin /fraction /guilsinglleft /guilsinglright\n");
			PutDoc("/minus /perthousand /quotedblbase /quotedblleft /quotedblright /quoteleft /quoteright /quotesinglbase /trademark /fi /fl /Lslash /OE /Scaron\n");
			PutDoc("/Ydieresis /Zcaron /dotlessi /lslash /oe /scaron /zcaron 160 /Euro 164 /currency 166 /brokenbar 168 /dieresis /copyright /ordfeminine 172 /logicalnot\n");
			PutDoc("/.notdef /registered /macron /degree /plusminus /twosuperior /threesuperior /acute /mu 183 /periodcentered /cedilla /onesuperior /ordmasculine\n");
			PutDoc("188 /onequarter /onehalf /threequarters 192 /Agrave /Aacute /Acircumflex /Atilde /Adieresis /Aring /AE /Ccedilla /Egrave /Eacute /Ecircumflex\n");
			PutDoc("/Edieresis /Igrave /Iacute /Icircumflex /Idieresis /Eth /Ntilde /Ograve /Oacute /Ocircumflex /Otilde /Odieresis /multiply /Oslash\n");
			PutDoc("/Ugrave /Uacute /Ucircumflex /Udieresis /Yacute /Thorn /germandbls /agrave /aacute /acircumflex /atilde /adieresis /aring /ae /ccedilla\n");
			PutDoc("/egrave /eacute /ecircumflex /edieresis /igrave /iacute /icircumflex /idieresis /eth /ntilde /ograve /oacute /ocircumflex /otilde /odieresis\n");
			PutDoc("/divide /oslash /ugrave /uacute /ucircumflex /udieresis /yacute /thorn /ydieresis\n");
			PutDoc("] >>\n");
		}
		PutDoc(">>");
		writer.endObj(fontObject);
		pageData.FObjects["FoStd"+Pdf::toPdf(a)] = fontObject;
		itStd.value() = "FoStd"+Pdf::toPdf(a);
		a++;
	}
}


PdfFont PDFLibCore::PDF_WriteType3Font(const QByteArray& name, ScFace& face, const QMap<uint,FPointArray>& RealGlyphs)
{
	PdfFont result;
	result.name = Pdf::toName(name);
	result.method = Use_Type3;
	result.encoding = Encode_256;

	// TrueType font rasterizers use non-zero winding file rule,
	// vs even-odd for current Postscript font rasterizers
	// Note: OTF CFF2 will also use non-zero winding rule, so
	// change code below when adding CCF2 support to Scribus
	// ref: https://www.microsoft.com/typography/OTSpec/cff2.htm
	bool useNonZeroRule = (face.type() == ScFace::TTF);

	uint SubFonts = 0;
	int glyphCount = 0;
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();

	QList<uint> glyphWidths;
	QList<QByteArray> charProcs;
	QStringList toUnicodeMaps;
	QList<int> toUnicodeMapsCount;
	QString toUnicodeMap = "";
	int toUnicodeMapCounter = 0;
	QByteArray encoding = "<< /Type /Encoding\n/Differences [ 0\n";
	QByteArray fon;
	QMap<uint, uint> glyphMapping;

	ScFace::FaceEncoding gl;
	face.glyphNames(gl);

	QMap<uint,FPointArray>::ConstIterator ig;
	for (ig = RealGlyphs.cbegin(); ig != RealGlyphs.cend(); ++ig)
	{
		FPoint np, np1, np2;
		bool nPath = true;
		fon.resize(0);
		if (ig.value().size() > 3)
		{
			FPointArray gly = ig.value();
			QTransform mat;
			mat.scale(100.0, -100.0);
			gly.map(mat);
			gly.translate(0, 1000);
			for (int poi = 0; poi < gly.size()-3; poi += 4)
			{
				if (gly.isMarker(poi))
				{
					fon += "h\n";
					nPath = true;
					continue;
				}
				if (nPath)
				{
					np = gly.point(poi);
					fon += FToStr(np.x())+" "+FToStr(np.y())+" m\n";
					nPath = false;
				}
				np = gly.point(poi+1);
				np1 = gly.point(poi+3);
				np2 = gly.point(poi+2);
				fon += FToStr(np.x()) + " " + FToStr(np.y()) + " " + FToStr(np1.x()) + " " + FToStr(np1.y()) + " " + FToStr(np2.x()) + " " + FToStr(np2.y()) + " c\n";
			}
			fon += useNonZeroRule? "h f\n" : "h f*\n";
			np = getMinClipF(&gly);
			np1 = getMaxClipF(&gly);
		}
		else
		{
			// 13654: make evince emit warning about "Syntax Error: No current point in closepath"
			// PDF specification is not quite clear about that, but a single "h" operator might not
			// be a valid way to make an empty path
			// fon = "h";
			np = FPoint(0, 0);
			np1 = FPoint(0, 0);
		}
		fon.prepend(Pdf::toPdf(qRound(np1.x())) + " 0 "+Pdf::toPdf(qRound(np.x()))+" "+Pdf::toPdf(qRound(np.y()))+" "+Pdf::toPdf(qRound(np1.x()))+ " "+Pdf::toPdf(qRound(np1.y()))+" d1\n");
		minx = qMin(minx, np.x());
		miny = qMin(miny, np.y());
		maxx = qMax(maxx, np1.x());
		maxy = qMax(maxy, np1.y());
		glyphWidths.append(qRound(np1.x()));

		PdfId charProcObject = writer.newObject();
		const ScFace::GlyphEncoding& glEncoding = gl[ig.key()];
		charProcs.append(Pdf::toName(glEncoding.glyphName)+" "+Pdf::toPdf(charProcObject)+" 0 R\n");
		encoding += Pdf::toName(glEncoding.glyphName)+" ";
		glyphMapping.insert(ig.key(), glyphCount + SubFonts * 256);
		writer.startObj(charProcObject);
		if (Options.Compress)
			fon = CompressArray(fon);
		PutDoc("<< /Length "+Pdf::toPdf(fon.length()+1));
		if (Options.Compress)
			PutDoc("\n/Filter /FlateDecode");
		PutDoc("\n>>\nstream\n"+EncStream(fon, charProcObject)+"\nendstream");
		writer.endObj(charProcObject);

		QString tmp;
		tmp.sprintf("%02X", glyphCount);
		toUnicodeMap += "<" + Pdf::toAscii(tmp) + "> <" + Pdf::toAscii(glEncoding.toUnicode) + ">\n";
		toUnicodeMapCounter++;
		if (toUnicodeMapCounter == 100)
		{
			toUnicodeMaps.append(toUnicodeMap);
			toUnicodeMapsCount.append(toUnicodeMapCounter);
			toUnicodeMap = "";
			toUnicodeMapCounter = 0;
		}

		glyphCount++;
		int glyphsLeft = RealGlyphs.count() - SubFonts * 256;
		if ((glyphCount > 255) || (glyphCount == glyphsLeft))
		{
			PdfId fontWidths = writer.newObject();
			writer.startObj(fontWidths);
			PutDoc("[ ");
			for (int ww = 0; ww < glyphWidths.count(); ++ww)
			{
				PutDoc(Pdf::toPdf(glyphWidths[ww])+" ");
			}
			PutDoc("]");
			writer.endObj(fontWidths);
			PdfId fontCharProcs = writer.newObject();
			writer.startObj(fontCharProcs);
			PutDoc("<<\n");
			for (int ww = 0; ww < charProcs.count(); ++ww)
			{
				PutDoc(charProcs[ww]);
			}
			PutDoc(">>");
			writer.endObj(fontCharProcs);
			PdfId fontEncoding = writer.newObject();
			writer.startObj(fontEncoding);
			PutDoc(encoding);
			PutDoc("]\n");
			PutDoc(">>");
			writer.endObj(fontEncoding);

			if (toUnicodeMapCounter != 0)
			{
				toUnicodeMaps.append(toUnicodeMap);
				toUnicodeMapsCount.append(toUnicodeMapCounter);
			}

			QByteArray toUnicodeMapStream = "";
			toUnicodeMapStream += "/CIDInit /ProcSet findresource begin\n";
			toUnicodeMapStream += "12 dict begin\n";
			toUnicodeMapStream += "begincmap\n";
			toUnicodeMapStream += "/CIDSystemInfo <<\n";
			toUnicodeMapStream += "/Registry (Adobe)\n";
			toUnicodeMapStream += "/Ordering (UCS)\n";
			toUnicodeMapStream += "/Supplement 0\n";
			toUnicodeMapStream += ">> def\n";
			toUnicodeMapStream += "/CMapName /Adobe-Identity-UCS def\n";
			toUnicodeMapStream += "/CMapType 2 def\n";
			toUnicodeMapStream += "1 begincodespacerange\n";
			toUnicodeMapStream += "<0000> <FFFF>\n";
			toUnicodeMapStream += "endcodespacerange\n";
			for (int uniC = 0; uniC < toUnicodeMaps.count(); uniC++)
			{
				toUnicodeMapStream += Pdf::toPdf(toUnicodeMapsCount[uniC]);
				toUnicodeMapStream += " beginbfchar\n";
				toUnicodeMapStream += toUnicodeMaps[uniC];
				toUnicodeMapStream += "endbfchar\n";
			}
			toUnicodeMapStream += "endcmap\n";
			toUnicodeMapStream += "CMapName currentdict /CMap defineresource pop\n";
			toUnicodeMapStream += "end\n";
			toUnicodeMapStream += "end\n";
			PdfId fontToUnicode = WritePDFStream(toUnicodeMapStream);

			PdfId font3Object = writer.newObject();
			writer.startObj(font3Object);
			PutDoc("<<\n/Type /Font\n/Subtype /Type3\n");
			PutDoc("/Name /"+name+"S"+Pdf::toPdf(SubFonts)+"\n");
			PutDoc("/FirstChar 0\n");
			PutDoc("/LastChar "+Pdf::toPdf(glyphCount-1)+"\n");
			PutDoc("/Widths "+Pdf::toPdf(fontWidths)+" 0 R\n");
			PutDoc("/CharProcs "+Pdf::toPdf(fontCharProcs)+" 0 R\n");
			PutDoc("/FontBBox ["+Pdf::toPdf(qRound(minx))+" "+Pdf::toPdf(qRound(miny))+" "+Pdf::toPdf(qRound(maxx))+ " "+Pdf::toPdf(qRound(maxy))+"]\n");
			PutDoc("/FontMatrix [0.001 0 0 0.001 0 0]\n");
			PutDoc("/Encoding "+Pdf::toPdf(fontEncoding)+" 0 R\n");
			PutDoc("/ToUnicode " + Pdf::toPdf(fontToUnicode) + " 0 R\n");
			PutDoc(">>");
			writer.endObj(font3Object);
			pageData.FObjects[name+"S"+Pdf::toPdf(SubFonts)] = font3Object;
			charProcs.clear();
			glyphWidths.clear();
			toUnicodeMaps.clear();
			toUnicodeMapsCount.clear();
			toUnicodeMap.clear();
			toUnicodeMapCounter = 0;
			glyphCount = 0;
			++SubFonts;
			minx =  std::numeric_limits<double>::max();
			miny =  std::numeric_limits<double>::max();
			maxx = -std::numeric_limits<double>::max();
			maxy = -std::numeric_limits<double>::max();
			encoding = "<< /Type /Encoding\n/Differences [ 0\n";
		}
	}
	result.glyphmap = glyphMapping;
	return result;
}


PdfFont PDFLibCore::PDF_WriteGlyphsAsXForms(const QByteArray& fontName, ScFace& face, const QMap<uint,FPointArray>& RealGlyphs)
{
	PdfFont result;
	result.name = Pdf::toName(fontName);
	result.method = Use_XForm;
	result.encoding = Encode_224;

	// TrueType font rasterizers use non-zero winding file rule,
	// vs even-odd for current Postscript font rasterizers
	// Note: OTF CFF2 will also use non-zero winding rule, so
	// change code below when adding CCF2 support to Scribus
	// ref: https://www.microsoft.com/typography/OTSpec/cff2.htm
	bool useNonZeroRule = (face.type() == ScFace::TTF);

	QByteArray fon;
	QMap<uint,FPointArray>::ConstIterator ig;
	for (ig = RealGlyphs.cbegin(); ig != RealGlyphs.cend(); ++ig)
	{
		FPoint np, np1, np2;
		bool nPath = true;
		fon.resize(0);
		if (ig.value().size() > 3)
		{
			FPointArray gly = ig.value();
			QTransform mat;
			mat.scale(0.1, 0.1);
			gly.map(mat);
			for (int poi = 0; poi < gly.size()-3; poi += 4)
			{
				if (gly.isMarker(poi))
				{
					fon += "h\n";
					nPath = true;
					continue;
				}
				if (nPath)
				{
					np = gly.point(poi);
					fon += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
					nPath = false;
				}
				np = gly.point(poi+1);
				np1 = gly.point(poi+3);
				np2 = gly.point(poi+2);
				fon += FToStr(np.x()) + " " + FToStr(-np.y()) + " " +
				FToStr(np1.x()) + " " + FToStr(-np1.y()) + " " +
				FToStr(np2.x()) + " " + FToStr(-np2.y()) + " c\n";
			}
			fon += useNonZeroRule? "h f\n" : "h f*\n";
			np = getMinClipF(&gly);
			np1 = getMaxClipF(&gly);
		}
		else
		{
			// 13654: make evince emit warning about "Syntax Error: No current point in closepath"
			// PDF specification is not quite clear about that, but a single "h" operator might not
			// be a valid way to make an empty path
			fon = " " /*"h"*/;
			np = FPoint(0, 0);
			np1 = FPoint(0, 0);
		}
		PdfId fontGlyphXForm = writer.newObject();
		writer.startObj(fontGlyphXForm);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
		PutDoc("/BBox [ "+FToStr(np.x())+" "+FToStr(-np.y())+" "+FToStr(np1.x())+ " "+FToStr(-np1.y())+" ]\n");
		PutDoc("/Resources << /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
		PutDoc(">>\n");
		if (Options.Compress)
			fon = CompressArray(fon);
		PutDoc("/Length "+Pdf::toPdf(fon.length()+1));
		if (Options.Compress)
			PutDoc("\n/Filter /FlateDecode");
		PutDoc(" >>\nstream\n"+EncStream(fon, fontGlyphXForm)+"\nendstream");
		writer.endObj(fontGlyphXForm);
		pageData.XObjects[fontName + "_gl" + Pdf::toPdf(ig.key())] = fontGlyphXForm;
	}
	return result;
}

PdfId PDFLibCore::PDF_EmbedFontObject(const QByteArray& font, const QByteArray& subtype)

{
	PdfId embeddedFontObject = writer.newObject();
	writer.startObj(embeddedFontObject);
	int len = font.length();
	QByteArray ttf = (Options.Compress? CompressArray(font) : font);
	//qDebug() << QString("sfnt data: size=%1 compressed=%2").arg(len).arg(bb.length());
	PutDoc("<<\n/Length " + Pdf::toPdf(ttf.length() + 1) + "\n");
	PutDoc("/Length1 " + Pdf::toPdf(len) + "\n");
	if (subtype.size() > 0)
		PutDoc("/Subtype " + subtype);
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n");
	EncodeArrayToStream(ttf, embeddedFontObject);
	PutDoc("\nendstream");
	writer.endObj(embeddedFontObject);
	return embeddedFontObject;
}

QByteArray PDFLibCore::PDF_GenerateSubsetTag(const QByteArray& fontName, QList<uint> usedGlyphs)
{
	uint hash, mod;
	QVector<uint> glyphVec = usedGlyphs.toVector();

	hash = qHashBits(fontName.constData(), fontName.size(), 0);
	hash = qHashBits(glyphVec.constData(), glyphVec.size() * sizeof(uint), hash);

	QByteArray subsetTag(6, (char) 0);
	for (int i = 0; i < 6; ++i)
	{
		mod = hash % 26;
		subsetTag[i] = 'A' + mod;
		hash = (hash - mod) / 26;
	}

	return subsetTag;
}

PdfId PDFLibCore::PDF_WriteFontDescriptor(const QByteArray& fontName, ScFace& face, ScFace::FontFormat fformat, PdfId embeddedFontObject)
{
	PdfId fontDescriptor = writer.newObject();
	writer.startObj(fontDescriptor);
	// TODO: think about QByteArray ScFace::getFontDescriptor() -- AV
	PutDoc("<<\n/Type /FontDescriptor\n");
	PutDoc("/FontName "  + Pdf::toName(fontName) + "\n");
	PutDoc("/FontBBox [ " + Pdf::toAscii(face.pdfFontBBoxAsString()) + " ]\n");
	PutDoc("/Flags ");
	//FIXME: isItalic() should be queried from ScFace, not from Qt -- AV
	//QFontInfo fo = QFontInfo(it.data());
	int pfl = 0;
	if (face.isFixedPitch())
		pfl = pfl ^ 1;
	//if (fo.italic())
	if (face.italicAngleAsString() != "0")
		pfl = pfl ^ 64;
	//			pfl = pfl ^ 4;
	pfl = pfl ^ 32;
	PutDoc(Pdf::toPdf(pfl)+"\n");
	PutDoc("/Ascent "      + Pdf::toAscii(face.pdfAscentAsString())+"\n");
	PutDoc("/Descent "     + Pdf::toAscii(face.pdfDescentAsString())+"\n");
	PutDoc("/CapHeight "   + Pdf::toAscii(face.pdfCapHeightAsString())+"\n");
	PutDoc("/ItalicAngle " + Pdf::toAscii(face.italicAngleAsString())+"\n");
	PutDoc("/StemV 1\n");
	if (embeddedFontObject != 0)
	{
		if (fformat == ScFace::SFNT || fformat == ScFace::TTCF)
		{
			if (face.type() == ScFace::OTF)
			{
				PutDoc("/FontFile3 "+Pdf::toPdf(embeddedFontObject)+" 0 R\n");
			}
			else
			{
				PutDoc("/FontFile2 "+Pdf::toPdf(embeddedFontObject)+" 0 R\n");
			}
		}
		else if (fformat == ScFace::PFB)
			PutDoc("/FontFile "+Pdf::toPdf(embeddedFontObject)+" 0 R\n");
		else if (fformat == ScFace::PFA)
			PutDoc("/FontFile "+Pdf::toPdf(embeddedFontObject)+" 0 R\n");
	}
	PutDoc(">>");
	writer.endObj(fontDescriptor);
	return fontDescriptor;
}

PdfFont PDFLibCore::PDF_EncodeCidFont(const QByteArray& fontName, ScFace& face, const QByteArray& baseFont, PdfId fontDes, const ScFace::FaceEncoding& gl, const QMap<uint,uint> glyphmap  )
{
	PdfFont result;
	result.name = Pdf::toName(fontName);
	result.method = glyphmap.isEmpty()? Use_Embedded : Use_Subset;
	result.encoding = glyphmap.isEmpty()? Encode_IdentityH : Encode_Subset;
	result.glyphmap = glyphmap;
	
	PdfId fontWidths2 = writer.newObject();
	writer.startObj(fontWidths2);
	QList<QByteArray> toUnicodeMaps;
	QList<int> toUnicodeMapsCount;
	QByteArray toUnicodeMap = "";
	int toUnicodeMapCounter = 0;
	
	PutDoc("[ ");
	QList<uint> keys = gl.uniqueKeys();
	QList<uint>::iterator git;
	bool seenNotDef = false;
	for (git = keys.begin(); git != keys.end(); ++git)
	{
		uint gid = result.encoding == Encode_Subset? glyphmap[*git] : *git;
		if (gid > 0 || !seenNotDef)
		{
			seenNotDef |= (gid == 0);
			PutDoc(Pdf::toPdf(gid)+" ["+Pdf::toPdf(static_cast<int>(face.glyphWidth(*git)* 1000))+"] " );
			QString tmp, tmp2;
			tmp.sprintf("%04X", gid);
			tmp2 = gl.value(*git).toUnicode;
			toUnicodeMap += "<" + Pdf::toAscii(tmp)+ "> <" + Pdf::toAscii(tmp2) + ">\n";
			toUnicodeMapCounter++;
			if (toUnicodeMapCounter == 100)
			{
				toUnicodeMaps.append(toUnicodeMap);
				toUnicodeMapsCount.append(toUnicodeMapCounter);
				toUnicodeMap = "";
				toUnicodeMapCounter = 0;
			}
		}
	}
	PutDoc("]");
	writer.endObj(fontWidths2);
	if (toUnicodeMapCounter != 0)
	{
		toUnicodeMaps.append(toUnicodeMap);
		toUnicodeMapsCount.append(toUnicodeMapCounter);
	}
	QByteArray toUnicodeMapStream = "";
	toUnicodeMapStream += "/CIDInit /ProcSet findresource begin\n";
	toUnicodeMapStream += "12 dict begin\n";
	toUnicodeMapStream += "begincmap\n";
	toUnicodeMapStream += "/CIDSystemInfo <<\n";
	toUnicodeMapStream += "/Registry (Adobe)\n";
	toUnicodeMapStream += "/Ordering (UCS)\n";
	toUnicodeMapStream += "/Supplement 0\n";
	toUnicodeMapStream += ">> def\n";
	toUnicodeMapStream += "/CMapName /Adobe-Identity-UCS def\n";
	toUnicodeMapStream += "/CMapType 2 def\n";
	toUnicodeMapStream += "1 begincodespacerange\n";
	toUnicodeMapStream += "<0000> <FFFF>\n";
	toUnicodeMapStream += "endcodespacerange\n";
	for (int uniC = 0; uniC < toUnicodeMaps.count(); uniC++)
	{
		toUnicodeMapStream += Pdf::toPdf(toUnicodeMapsCount[uniC]);
		toUnicodeMapStream += " beginbfchar\n";
		toUnicodeMapStream += toUnicodeMaps[uniC];
		toUnicodeMapStream += "endbfchar\n";
	}
	toUnicodeMapStream += "endcmap\n";
	toUnicodeMapStream += "CMapName currentdict /CMap defineresource pop\n";
	toUnicodeMapStream += "end\n";
	toUnicodeMapStream += "end\n";
	PdfId fontToUnicode2 = WritePDFStream(toUnicodeMapStream);
	PdfId fontObject2 = writer.newObject();
	writer.startObj(fontObject2);
	PutDoc("<<\n/Type /Font\n/Subtype /Type0\n");
	PutDoc("/Name " + Pdf::toName(fontName) + "\n");
	PutDoc("/BaseFont "+ Pdf::toName(baseFont) +"\n");
	PutDoc("/Encoding /Identity-H\n");
	PutDoc("/ToUnicode "+Pdf::toPdf(fontToUnicode2)+" 0 R\n");
	PutDoc("/DescendantFonts [");
	PutDoc("<</Type /Font");
	if (face.type() == ScFace::OTF)
		PutDoc("/Subtype /CIDFontType0");
	else
		PutDoc("/Subtype /CIDFontType2");
	PutDoc("/BaseFont " + Pdf::toName(baseFont));
	PutDoc("/FontDescriptor " + Pdf::toPdf(fontDes)+ " 0 R");
	PutDoc("/CIDSystemInfo <</Ordering(Identity)/Registry(Adobe)/Supplement 0>>");
	PutDoc("/DW 1000");
	PutDoc("/W "+Pdf::toPdf(fontWidths2)+" 0 R");
	PutDoc("/CIDToGIDMap /Identity");
	PutDoc(">>"); // close CIDFont dictionary
	PutDoc("]\n"); // close DescendantFonts array
	PutDoc(">>");
	writer.endObj(fontObject2);
	pageData.FObjects[fontName] = fontObject2;
	return result;
}


PdfFont PDFLibCore::PDF_EncodeSimpleFont(const QByteArray& fontName, ScFace& face, const QByteArray& baseFont, const QByteArray& subtype, bool isEmbedded, PdfId fontDes, const ScFace::FaceEncoding& gl)
{
	PdfFont result;
	result.name = Pdf::toName(fontName);
	result.method = isEmbedded? Use_Embedded : Use_System;
	result.encoding = Encode_224;
	
	int nglyphs = 0;
	ScFace::FaceEncoding::ConstIterator gli;
	for (gli = gl.cbegin(); gli != gl.cend(); ++gli)
	{
		if (gli.key() > static_cast<uint>(nglyphs))
			nglyphs = gli.key();
	}
	++nglyphs;
	//			qDebug() << QString("pdflib: nglyphs %1 max %2").arg(nglyphs).arg(face.maxGlyph());
	
	uint Fcc = nglyphs / 224;
	if ((nglyphs % 224) != 0)
		Fcc += 1;
	for (uint Fc = 0; Fc < Fcc; ++Fc)
	{
		PdfId fontWidths2 = writer.newObject();
		writer.startObj(fontWidths2);
		int chCount = 32;
		PutDoc("[ 0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 ");
		for (int ww = 32; ww < 256; ++ww)
		{
			uint glyph = 224 * Fc + ww - 32;
			if (gl.contains(glyph))
				PutDoc(Pdf::toPdf(static_cast<int>(face.glyphWidth(glyph)* 1000))+" ");
			else
				PutDoc("0 ");
			chCount++;
			if (signed(glyph) == nglyphs-1)
				break;
		}
		PutDoc("]");
		writer.endObj(fontWidths2);
		PdfId fontEncoding2 = writer.newObject();
		writer.startObj(fontEncoding2);
		QStringList toUnicodeMaps;
		QList<int> toUnicodeMapsCount;
		QString toUnicodeMap = "";
		int toUnicodeMapCounter = 0;
		PutDoc("<< /Type /Encoding\n");
		PutDoc("/Differences [ \n");
		int crc = 0;
		bool startOfSeq = true;
		for (int ww2 = 32; ww2 < 256; ++ww2)
		{
			uint glyph = 224 * Fc + ww2 - 32;
			ScFace::FaceEncoding::ConstIterator glIt = gl.find(glyph);
			if (glIt != gl.cend() && !glIt.value().glyphName.isEmpty())
			{
				const ScFace::GlyphEncoding& glEncoding = glIt.value();
				if (startOfSeq)
				{
					PutDoc(Pdf::toPdf(ww2)+" ");
					startOfSeq = false;
				}
				PutDoc(Pdf::toName(glEncoding.glyphName)+" ");
				QString tmp, tmp2;
				tmp.sprintf("%02X", ww2);
				tmp2 = glEncoding.toUnicode;
				toUnicodeMap += "<" + Pdf::toAscii(tmp) + "> <" + Pdf::toAscii(tmp2) + ">\n";
				//QString("<%1> <%2>\n").arg(tmp).arg((tmp2));
				toUnicodeMapCounter++;
				if (toUnicodeMapCounter == 100)
				{
					toUnicodeMaps.append(toUnicodeMap);
					toUnicodeMapsCount.append(toUnicodeMapCounter);
					toUnicodeMap = "";
					toUnicodeMapCounter = 0;
				}
				crc++;
			}
			else
			{
				startOfSeq = true;
			}
			if (signed(glyph) == nglyphs-1)
				break;
			if (crc > 8)
			{
				PutDoc("\n");
				crc = 0;
			}
		}
		if (toUnicodeMapCounter != 0)
		{
			toUnicodeMaps.append(toUnicodeMap);
			toUnicodeMapsCount.append(toUnicodeMapCounter);
		}
		PutDoc("]\n");
		PutDoc(">>");
		writer.endObj(fontEncoding2);
		QByteArray toUnicodeMapStream = "";
		toUnicodeMapStream += "/CIDInit /ProcSet findresource begin\n";
		toUnicodeMapStream += "12 dict begin\n";
		toUnicodeMapStream += "begincmap\n";
		toUnicodeMapStream += "/CIDSystemInfo <<\n";
		toUnicodeMapStream += "/Registry (Adobe)\n";
		toUnicodeMapStream += "/Ordering (UCS)\n";
		toUnicodeMapStream += "/Supplement 0\n";
		toUnicodeMapStream += ">> def\n";
		toUnicodeMapStream += "/CMapName /Adobe-Identity-UCS def\n";
		toUnicodeMapStream += "/CMapType 2 def\n";
		toUnicodeMapStream += "1 begincodespacerange\n";
		toUnicodeMapStream += "<0000> <FFFF>\n";
		toUnicodeMapStream += "endcodespacerange\n";
		for (int uniC = 0; uniC < toUnicodeMaps.count(); uniC++)
		{
			toUnicodeMapStream += Pdf::toPdf(toUnicodeMapsCount[uniC]);
			toUnicodeMapStream += " beginbfchar\n";
			toUnicodeMapStream += toUnicodeMaps[uniC];
			toUnicodeMapStream += "endbfchar\n";
		}
		toUnicodeMapStream += "endcmap\n";
		toUnicodeMapStream += "CMapName currentdict /CMap defineresource pop\n";
		toUnicodeMapStream += "end\n";
		toUnicodeMapStream += "end\n";
		PdfId fontToUnicode2 = WritePDFStream(toUnicodeMapStream);
		PdfId fontObject2 = writer.newObject();
		writer.startObj(fontObject2);
		PutDoc("<<\n/Type /Font\n/Subtype ");
		PutDoc(subtype + "\n");
		PutDoc("/Name "+Pdf::toName(fontName)+"S"+Pdf::toPdf(Fc)+"\n");
		PutDoc("/BaseFont "+ Pdf::toName(baseFont) + "\n");
		PutDoc("/FirstChar 0\n");
		PutDoc("/LastChar "+Pdf::toPdf(chCount-1)+"\n");
		PutDoc("/Widths "+Pdf::toPdf(fontWidths2)+" 0 R\n");
		PutDoc("/Encoding "+Pdf::toPdf(fontEncoding2)+" 0 R\n");
		PutDoc("/ToUnicode "+Pdf::toPdf(fontToUnicode2)+" 0 R\n");
		PutDoc("/FontDescriptor "+Pdf::toPdf(fontDes)+" 0 R\n");
		PutDoc(">>");
		writer.endObj(fontObject2);
		pageData.FObjects[fontName + "S"+Pdf::toPdf(Fc)] = fontObject2;
	} // for(Fc)

	return result;
}

PdfFont PDFLibCore::PDF_EncodeFormFont(const QByteArray& fontName, ScFace& face, const QByteArray& baseFont, const QByteArray& subtype, PdfId fontDes)
{
	PdfFont formFont;
	formFont.name = Pdf::toName(fontName) + "Form";
	formFont.usage = Used_in_Forms;
	
	PdfId fontWidthsForm = writer.newObject();
	writer.startObj(fontWidthsForm);
	PutDoc("[ 0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 ");
	for (uint ww = 32; ww < 256; ++ww)
	{
		int unicode = Pdf::fromPDFDocEncoding(ww);
		uint glyph  = face.char2CMap(unicode);
		if (glyph > 0)
			PutDoc(Pdf::toPdf(static_cast<int>(face.glyphWidth(glyph)* 1000))+" ");
		else
			PutDoc("0 ");
	}
	PutDoc("]");
	writer.endObj(fontWidthsForm);

	PdfId fontObjectForm = writer.newObject();
	writer.startObj(fontObjectForm);
	PutDoc("<<\n/Type /Font\n/Subtype ");
	PutDoc(subtype + "\n");
	PutDoc("/Name " + formFont.name + "\n");
	PutDoc("/BaseFont "+ Pdf::toName(baseFont) +"\n");
	PutDoc("/Encoding << \n");
	PutDoc("/Differences [ \n");
	PutDoc("24 /breve /caron /circumflex /dotaccent /hungarumlaut /ogonek /ring /tilde\n");
	PutDoc("39 /quotesingle 96 /grave 128 /bullet /dagger /daggerdbl /ellipsis /emdash /endash /florin /fraction /guilsinglleft /guilsinglright\n");
	PutDoc("/minus /perthousand /quotedblbase /quotedblleft /quotedblright /quoteleft /quoteright /quotesinglbase /trademark /fi /fl /Lslash /OE /Scaron\n");
	PutDoc("/Ydieresis /Zcaron /dotlessi /lslash /oe /scaron /zcaron 160 /Euro 164 /currency 166 /brokenbar 168 /dieresis /copyright /ordfeminine 172 /logicalnot\n");
	PutDoc("/.notdef /registered /macron /degree /plusminus /twosuperior /threesuperior /acute /mu 183 /periodcentered /cedilla /onesuperior /ordmasculine\n");
	PutDoc("188 /onequarter /onehalf /threequarters 192 /Agrave /Aacute /Acircumflex /Atilde /Adieresis /Aring /AE /Ccedilla /Egrave /Eacute /Ecircumflex\n");
	PutDoc("/Edieresis /Igrave /Iacute /Icircumflex /Idieresis /Eth /Ntilde /Ograve /Oacute /Ocircumflex /Otilde /Odieresis /multiply /Oslash\n");
	PutDoc("/Ugrave /Uacute /Ucircumflex /Udieresis /Yacute /Thorn /germandbls /agrave /aacute /acircumflex /atilde /adieresis /aring /ae /ccedilla\n");
	PutDoc("/egrave /eacute /ecircumflex /edieresis /igrave /iacute /icircumflex /idieresis /eth /ntilde /ograve /oacute /ocircumflex /otilde /odieresis\n");
	PutDoc("/divide /oslash /ugrave /uacute /ucircumflex /udieresis /yacute /thorn /ydieresis\n");
	PutDoc("] >>\n");
	PutDoc("/FirstChar 0\n");
	PutDoc("/LastChar 255\n");
	PutDoc("/Widths "+Pdf::toPdf(fontWidthsForm)+" 0 R\n");
	PutDoc("/FontDescriptor "+Pdf::toPdf(fontDes)+" 0 R\n");
	PutDoc(">>");
	writer.endObj(fontObjectForm);

	pageData.FObjects[fontName + "Form"] = fontObjectForm;
	UsedFontsF.insert(face.replacementName(), formFont);

	return formFont;
}

/*
static void dumpFont(QString name, QByteArray data)
{
	QFile file(name);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream out(&file);
		out.writeRawData(data.data(), data.length());
		file.close();
	}
}

static void dumpCFF(QString name, cff::CFF font)
{
	QFile file(name);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream out(&file);
		font.dump(out);
		file.close();
	}
}
*/


PdfFont PDFLibCore::PDF_WriteTtfSubsetFont(const QByteArray& fontName, ScFace& face, const QMap<uint,FPointArray>& RealGlyphs)
{
	QByteArray font;
	face.rawData(font);
	/*dumpFont(face.psName() + ".ttf", font);*/
	QList<ScFace::gid_type> glyphs = RealGlyphs.uniqueKeys();
	glyphs.removeAll(0);
	glyphs.prepend(0);
	QByteArray subset = sfnt::subsetFace(font, glyphs);
	/*dumpFont(face.psName()+"subs.ttf", subset);*/
	QByteArray baseFont   = sanitizeFontName(face.psName());
	QByteArray subsetTag  = PDF_GenerateSubsetTag(baseFont, glyphs);
	QByteArray subsetName = subsetTag + '+' + baseFont;
	PdfId embeddedFontObj = PDF_EmbedFontObject(subset, QByteArray());
	PdfId fontDes = PDF_WriteFontDescriptor(subsetName, face, face.format(), embeddedFontObj);
	
	ScFace::FaceEncoding fullEncoding;
	QMap<uint,uint> glyphmap;
	face.glyphNames(fullEncoding);
	for (int i = 0; i < glyphs.length(); ++i)
	{
		glyphmap[glyphs[i]] = i;
		//qDebug() << glyphs[i] << " --> " << i << QChar(fullEncoding[glyphs[i]].charcode);
	}
	
	PdfFont result = PDF_EncodeCidFont(fontName, face, subsetName, fontDes, fullEncoding, glyphmap);
	return result;
}


PdfFont PDFLibCore::PDF_WriteCffSubsetFont(const QByteArray& fontName, ScFace& face, const QMap<uint,FPointArray>& RealGlyphs)
{
//	QByteArray sfnt; //TEST
//	face.rawData(sfnt);
//	QByteArray cff = sfnt::getTable(sfnt, "CFF ");
//	dumpFont(fontName, cff);
//	cff::CFF cfffont(cff);
//	cfffont.dump();
//	QByteArray subsetfont = cff::subsetFace(cff, it.value().keys());
//	dumpFont(it.key() + "subs.cff", subsetfont);
//	cff::CFF subset(subsetfont);
//	subset.dump();
//	PDF_WriteFontDescriptor(fontName, face, fformat, 0);
//	// END
	
	QByteArray font, data;
	face.rawData(data);
	font = sfnt::getTable(data, "CFF ");
	/*dumpFont(face.psName() + ".cff", font);*/
	QList<ScFace::gid_type> glyphs = RealGlyphs.uniqueKeys();
	glyphs.removeAll(0);
	glyphs.prepend(0);
	QByteArray subset = cff::subsetFace(font, glyphs);
	/*dumpFont(face.psName()+"subs.cff", subset);*/
	QByteArray baseFont   = sanitizeFontName(face.psName());
	QByteArray subsetTag  = PDF_GenerateSubsetTag(baseFont, glyphs);
	QByteArray subsetName = subsetTag + '+' + baseFont;
	PdfId embeddedFontObj = PDF_EmbedFontObject(subset, "/CIDFontType0C");
	PdfId fontDes = PDF_WriteFontDescriptor(subsetName, face, face.format(), embeddedFontObj);

	ScFace::FaceEncoding fullEncoding;
	QMap<uint,uint> glyphmap;
	face.glyphNames(fullEncoding);
	for (int i = 0; i < glyphs.length(); ++i)
	{
		glyphmap[glyphs[i]] = i;
		//qDebug() << glyphs[i] << " --> " << i << QChar(fullEncoding[glyphs[i]].charcode);
	}
	
	PdfFont result = PDF_EncodeCidFont(fontName, face, subsetName, fontDes, fullEncoding, glyphmap);
	return result;
}


PdfId PDFLibCore::PDF_EmbedType1AsciiFontObject(const QByteArray& fon)
{
	QByteArray fon2;
	PdfId embeddedFontObject = writer.newObject();
	writer.startObj(embeddedFontObject);
	int len1 = fon.indexOf("eexec")+5;
	fon2 = fon.left(len1) + "\n";
	int len2 = fon.indexOf("0000000000000000000000000");
	if (len2 == -1)
		len2 = fon.length()+1;
	//			  QString tm;
	//			  uint value;
	//			  bool ok = true;
	//				int count = 0;
	//				for (int xx = len1; xx < len2-1; ++xx)
	//				{
	//					tm = fon.at(xx);
	//					if ((tm == QChar(13)) || (tm == QChar(10)))
	//						continue;
	//					xx++;
	//					count++;
	//					tm += fon.at(xx);
	//					value = tm.toUInt(&ok, 16);
	//					fon2 += char(value);
	//				}
	QByteArray hexData = QByteArray::fromHex(fon.mid(len1, len2-len1));
	fon2 += hexData;
	fon2 += fon.mid(len2);
	if (Options.Compress)
		fon2 = CompressArray(fon2);
	PutDoc("<<\n/Length "+Pdf::toPdf(fon2.length()+1)+"\n");
	PutDoc("/Length1 "+Pdf::toPdf(len1+1)+"\n");
	PutDoc("/Length2 "+Pdf::toPdf(hexData.length())+"\n");
	if(static_cast<int>(fon.length()-len2) == -1)
		PutDoc("/Length3 0\n");
	else
		PutDoc("/Length3 "+Pdf::toPdf(fon.length()-len2)+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(fon2, embeddedFontObject)+"\nendstream");
	writer.endObj(embeddedFontObject);
	return embeddedFontObject;
}


PdfId PDFLibCore::PDF_EmbedType1BinaryFontObject(const QByteArray& bb)
{
	PdfId embeddedFontObject = writer.newObject();
	QByteArray fon;
	writer.startObj(embeddedFontObject);
	int posi;
	for (posi = 6; posi < bb.size(); ++posi)
	{
		if ((bb[posi] == static_cast<char>(0x80)) && (static_cast<int>(bb[posi+1]) == 2))
			break;
		fon += bb[posi];
	}
	int len1 = fon.length();
	int ulen;
	ulen = bb[posi+2] & 0xff;
	ulen |= (bb[posi+3] << 8) & 0xff00;
	ulen |= (bb[posi+4] << 16) & 0xff0000;
	ulen |= (bb[posi+5] << 24) & 0xff000000;
	if (ulen > bb.size())
		ulen = bb.size()-7;
	posi += 6;
	for (int j = 0; j < ulen; ++j)
		fon += bb[posi++];
	posi += 6;
	int len2 = fon.length()-len1;
	for (int j = posi; j < bb.size(); ++j)
	{
		if ((bb[j] == static_cast<char>(0x80)) && (static_cast<int>(bb[j+1]) == 3))
			break;
		if (bb[j] == '\r')
			fon += "\n";
		else
			fon += bb[j];
	}
	int len3 = fon.length()-len2-len1;
	if (Options.Compress)
		fon = CompressArray(fon);
	PutDoc("<<\n/Length "+Pdf::toPdf(fon.length()+1)+"\n");
	PutDoc("/Length1 "+Pdf::toPdf(len1)+"\n");
	PutDoc("/Length2 "+Pdf::toPdf(len2)+"\n");
	PutDoc("/Length3 "+Pdf::toPdf(len3)+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(fon,embeddedFontObject)+"\nendstream");
	writer.endObj(embeddedFontObject);
	return embeddedFontObject;
}


PdfId PDFLibCore::PDF_EmbedFontObject(const QString& name, ScFace& face)
{
	PdfId embeddedFontObject = 0;
	if (Options.EmbedList.contains(name))
	{
		ScFace::FontFormat fformat = face.format();
		QByteArray bb;
		face.rawData(bb);
		if (fformat == ScFace::PFB)
		{
			embeddedFontObject = PDF_EmbedType1BinaryFontObject(bb);
		}
		if (fformat == ScFace::PFA)
		{
			embeddedFontObject = PDF_EmbedType1AsciiFontObject(bb);
		}
		if (fformat == ScFace::SFNT || fformat == ScFace::TTCF)
		{
			QByteArray subtype;
			if (face.type() == ScFace::OTF)
			{
				subtype = "/CIDFontType0C";
				QByteArray cff_bb = sfnt::getTable(bb, "CFF ");
				//dumpCFF(face.psName() + ".cff", cff::CFF(cff_bb));
				bb = cff::extractFace(cff_bb, 0);
				//dumpCFF(face.psName() + "full.cff", cff::CFF(bb));
			}
			embeddedFontObject = PDF_EmbedFontObject(bb, subtype);
		}
	}
	return embeddedFontObject;
}


void PDFLibCore::PDF_Begin_WriteUsedFonts(SCFonts &AllFonts, const QMap<QString, QMap<uint, FPointArray> >& ReallyUsed)
{
	qDebug() << "embed list:" << QStringList(Options.EmbedList).join(", ");
	qDebug() << "subset list:" << QStringList(Options.SubsetList).join(", ");
	qDebug() << "outline list:" << QStringList(Options.OutlineList).join(", ");
	QMap<QString,QMap<uint, FPointArray> >::ConstIterator it;
	int a = 0;
	for (it = ReallyUsed.cbegin(); it != ReallyUsed.cend(); ++it)
	{
		ScFace& face(AllFonts[it.key()]);
		ScFace::FontFormat fformat = face.format();
		PdfFont pdfFont;
		QByteArray fontName = QByteArray("Fo") + Pdf::toPdf(a);
		
		QMap<uint, FPointArray> usedGlyphs = it.value();
		if (usedGlyphs.count() <= 0)
			continue;
		
		qDebug() << "pdf font" << it.key();
		if (Options.OutlineList.contains(it.key()))
		{
			pdfFont = PDF_WriteGlyphsAsXForms(fontName, face, usedGlyphs);
		}
		else
		{
			if (Options.SubsetList.contains(it.key()))
			{
				if (fformat == ScFace::SFNT || fformat == ScFace::TTCF)
				{
					if (face.type() == ScFace::TTF)
					{
						pdfFont = PDF_WriteTtfSubsetFont(fontName, face, usedGlyphs);
					}
					else if (face.isCIDKeyed())
					{
						pdfFont = PDF_WriteType3Font(fontName, face, usedGlyphs);
					}
					else
					{
						pdfFont = PDF_WriteCffSubsetFont(fontName, face, usedGlyphs);
					}
				}
				else
				{
					pdfFont = PDF_WriteType3Font(fontName, face, usedGlyphs);
				}
			}
			else
			{
				PdfId embeddedFontObject = PDF_EmbedFontObject(it.key(), face);
				
				QByteArray baseFont  = sanitizeFontName(face.psName());
				PdfId fontDescriptor = PDF_WriteFontDescriptor(baseFont, face, fformat, embeddedFontObject);
				
				ScFace::FaceEncoding gl;
				face.glyphNames(gl);

				// #14550 : for TTF fonts, we already avoid using the Postscript glyph name table
				// as it is notoriously unreliable. We hence retrieve glyph names using unicode cmap
				// and adobe glyph names uniXXXX convention. Unfortunately we may still not get all
				// required glyph names and some glyphs may not have a name anyway. So check we have
				// ps names for all glyphs we need and if not, then use CID encoding
				bool hasNeededGlyphNames = face.hasNames() && (gl.count() >= usedGlyphs.count());
				if ((fformat == ScFace::SFNT || fformat == ScFace::TTCF))
				{
					QMap<uint, FPointArray>::const_iterator it;
					for (it = usedGlyphs.begin(); it != usedGlyphs.end(); ++it)
					{
						int glyphIndex = it.key();
						hasNeededGlyphNames &= gl.contains(glyphIndex);
						if (!hasNeededGlyphNames)
							break;
					}
				}
				
				QByteArray subtype = (fformat == ScFace::SFNT || fformat == ScFace::TTCF) ? "/TrueType" : "/Type1";
				
				if ((face.isSymbolic() || !hasNeededGlyphNames || Options.Version == PDFOptions::PDFVersion_X4 || face.type() == ScFace::OTF) &&
					(fformat == ScFace::SFNT || fformat == ScFace::TTCF))
				{
					pdfFont = PDF_EncodeCidFont(fontName, face, baseFont, fontDescriptor, gl, QMap<uint,uint>());
				}
				else
				{
					pdfFont = PDF_EncodeSimpleFont(fontName, face, baseFont, subtype, embeddedFontObject != 0, fontDescriptor, gl);
				}

				if ((Options.Version != PDFOptions::PDFVersion_X4) && (face.type() != ScFace::OTF))
					PDF_EncodeFormFont(fontName, face, baseFont, subtype, fontDescriptor);
			}
			pdfFont.usage = Used_in_Content;
		}
		a++;
		QString meth;
		switch (pdfFont.method)
		{
			case Use_System:
				meth = "Systemfont (no embedding)"; break;
			case Use_Embedded:
				meth = "Embed"; break;
			case Use_Subset:
				meth = "Subset"; break;
			case Use_Type3:
				meth = "Subset as Type3 font"; break;
			case Use_XForm:
				meth = "Outline (PDF XForm)"; break;
			default:
				meth = "?"; break;
		}
		qDebug() << pdfFont.name << "uses method" << meth << "and encoding" << pdfFont.encoding;
		UsedFontsP.insert(it.key(), pdfFont);
	}
}

void PDFLibCore::PDF_Begin_Colors()
{
	if (Options.UseLPI)
	{
		uint halftones = writer.newObject();
		writer.startObj(halftones);
		PutDoc("<<\n/Type /Halftone\n/HalftoneType 5\n");
		QMap<QString,LPIData>::const_iterator itlp;
		for (itlp = Options.LPISettings.constBegin(); itlp != Options.LPISettings.constEnd(); ++itlp)
		{
			PutDoc(Pdf::toName(itlp.key()) + "\n<<\n/Type /Halftone\n/HalftoneType 1\n/Frequency ");
			PutDoc(Pdf::toPdf(itlp.value().Frequency)+"\n/Angle "+Pdf::toPdf(itlp.value().Angle)+"\n/SpotFunction ");
			QByteArray func ("");
			switch (itlp.value().SpotFunc)
			{
				case 0:
					func = "/SimpleDot";
					break;
				case 1:
					func = "/Line";
					break;
				case 2:
					func = "/Round";
					break;
				case 3:
					func = "/Ellipse";
					break;
				default:
					func = "/SimpleDot";
					break;
			}
			PutDoc(func + "\n>>\n");
		}
		PutDoc("/Default\n<<\n/Type /Halftone\n/HalftoneType 1\n/Frequency 50\n/Angle 45\n/SpotFunction /Round\n>>\n");
		PutDoc(">>");
		writer.endObj(halftones);
		HTName = ResNam+Pdf::toPdf(ResCount);
		Transpar[HTName] = writeGState("/HT "+Pdf::toPdf(halftones)+" 0 R\n");
		ResCount++;
	}
	if ((doc.HasCMS) && (Options.UseProfiles) && (Options.Version != PDFOptions::PDFVersion_X1a))
	{
		PdfId iccProfileObject = writer.newObject();
		writer.startObj(iccProfileObject);
		QByteArray dataP;
		PdfICCD dataD;
		loadRawBytes(ScCore->InputProfiles[Options.SolidProf], dataP);
		PutDoc("<<\n");
		if (Options.Compress)
		{
			QByteArray compData = CompressArray(dataP);
			if (compData.size() > 0)
			{
				PutDoc("/Filter /FlateDecode\n");
				dataP = compData;
			}
		}
		PutDoc("/Length "+Pdf::toPdf(dataP.size()+1)+"\n");
		PutDoc("/N "+Pdf::toPdf(Options.SComp)+"\n");
		PutDoc(">>\nstream\n");
		EncodeArrayToStream(dataP, iccProfileObject);
		PutDoc("\nendstream");
		writer.endObj(iccProfileObject);
		PdfId iccColorspace = writer.newObject();
		writer.startObj(iccColorspace);
		dataD.ResName = ResNam+Pdf::toPdf(ResCount);
		dataD.ICCArray = "[ /ICCBased "+Pdf::toPdf(iccProfileObject)+" 0 R ]";
		dataD.ResNum = iccColorspace;
		dataD.components = Options.SComp;
		ICCProfiles[Options.SolidProf] = dataD;
		PutDoc("[ /ICCBased "+Pdf::toPdf(iccProfileObject)+" 0 R ]");
		writer.endObj(iccColorspace);
		ResCount++;
	}
	if (((Options.isGrayscale == false) && (Options.UseRGB == false)) && (Options.UseSpotColors))
	{
		doc.getUsedColors(colorsToUse);
		ColorList::Iterator itf;
		for (itf = colorsToUse.begin(); itf != colorsToUse.end(); ++itf)
		{
			const ScColor& colorToUse = itf.value();
			if ((colorToUse.isSpotColor()) || (colorToUse.isRegistrationColor()))
			{
				CMYKColor cmykValues;
				int cc, cm, cy, ck;
				PdfSpotC spotD;
				ScColorEngine::getCMYKValues(colorToUse, &doc, cmykValues);
				cmykValues.getValues(cc, cm, cy, ck);
				QByteArray colorDesc = "{\ndup "+FToStr(static_cast<double>(cc) / 255)+"\nmul exch dup ";
				colorDesc += FToStr(static_cast<double>(cm) / 255)+"\nmul exch dup ";
				colorDesc += FToStr(static_cast<double>(cy) / 255)+"\nmul exch ";
				colorDesc += FToStr(static_cast<double>(ck) / 255)+" mul }";
				PdfId separationFunction = writer.newObject();
				writer.startObj(separationFunction);
				PutDoc("<<\n/FunctionType 4\n");
				PutDoc("/Domain [0.0 1.0]\n");
				PutDoc("/Range [0.0 1.0 0.0 1.0 0.0 1.0 0.0 1.0]\n");
				PutDoc("/Length "+Pdf::toPdf(colorDesc.length()+1)+"\n");
				PutDoc(">>\nstream\n"+EncStream(colorDesc, separationFunction)+"\nendstream");
				writer.endObj(separationFunction);
				PdfId separationColorspace= writer.newObject();
				writer.startObj(separationColorspace);
				PutDoc("[ /Separation ");
				if (colorsToUse[itf.key()].isRegistrationColor())
					PutDoc("/All");
				else
					PutDoc(Pdf::toName(itf.key().simplified()));
				PutDoc(" /DeviceCMYK "+Pdf::toObjRef(separationFunction)+" ]");
				writer.endObj(separationColorspace);
				spotD.ResName = spotNam+Pdf::toPdf(spotCount);
				spotD.ResNum = separationColorspace;
				spotMap.insert(itf.key(), spotD);
				spotCount++;
			}
		}
	}
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
	{
		PdfSpotC spotD;
		PdfId registrationColorspace = writer.newObject();
		writer.startObj(registrationColorspace);
		PutDoc("[ /Separation /All /DeviceCMYK\n");
		PutDoc("<<\n/FunctionType 2\n");
		PutDoc("/Domain [0.0 1.0]\n");
		PutDoc("/Range [0.0 1.0 0.0 1.0 0.0 1.0 0.0 1.0]\n");
		PutDoc("/C0 [0 0 0 0] \n");
		PutDoc("/C1 [1 1 1 1] \n");
		PutDoc("/N 1\n");
		PutDoc(">>\n]");
		writer.endObj(registrationColorspace);
		spotD.ResName = spotNam+Pdf::toPdf(spotCount);
		spotD.ResNum = registrationColorspace;
		spotMapReg.insert("Register", spotD);
		spotCount++;
	}
}

void PDFLibCore::PDF_Begin_Layers()
{
	if ( ((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
	{
		ScLayer ll;
		PdfOCGInfo ocg;
		ll.isPrintable = false;
		ll.ID = 0;
		int Lnr = 0;
		QByteArray ocgNam("oc");
		uint docLayersCount=doc.Layers.count();
		for (uint la = 0; la < docLayersCount; ++la)
		{
			PdfId optionalContent = writer.newObject();
			doc.Layers.levelToLayer(ll, Lnr);
			ocg.Name = ocgNam + Pdf::toPdf(ll.ID);
			ocg.ObjNum = optionalContent;
			ocg.visible = ll.isViewable;
			OCGEntries.insert(ll.Name, ocg);
			writer.startObj(optionalContent);
			PutDoc("<<\n");
			PutDoc("/Type /OCG\n");
			PutDoc("/Name ");
			PutDoc(EncStringUTF16(ll.Name, optionalContent));
			PutDoc("\n");
			PutDoc("/Usage <</Print <</PrintState ");
			if (ll.isPrintable)
				PutDoc("/ON");
			else
				PutDoc("/OFF");
			PutDoc(">> /View <</ViewState ");
			if (ll.isViewable)
				PutDoc("/ON");
			else
				PutDoc("/OFF");
			PutDoc(">>>>");
			PutDoc("\n");
			PutDoc(">>");
			writer.endObj(optionalContent);
			Lnr++;
		}
	}
}

bool PDFLibCore::PDF_TemplatePage(const ScPage* pag, bool )
{
	QByteArray tmpOut;
	ActPageP = pag;
	PageItem* ite;
	QList<PageItem*> PItems;
	int Lnr = 0;
	ScLayer ll;
	ll.isPrintable = false;
	ll.ID = 0;
	Content = "";

	double bLeft, bRight, bBottom, bTop;
	getBleeds(pag, bLeft, bRight, bBottom, bTop);

	pageData.AObjects.clear();

	for (int la = 0; la < doc.Layers.count(); ++la)
	{
		doc.Layers.levelToLayer(ll, Lnr);
		PItems = doc.MasterItems;
		if ((ll.isPrintable) || (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers)))
		{
			if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
				PutPage("/OC /" + OCGEntries[ll.Name].Name + " BDC\n");
			for (int a = 0; a < PItems.count(); ++a)
			{
				Content = "";
				ite =PItems.at(a);
				if (ite->LayerID != ll.ID)
					continue;
				double x  = pag->xOffset() - bLeft;
				double y  = pag->yOffset() - bTop;
				double w  = pag->width() + bLeft + bRight;
				double h1 = pag->height() + bBottom + bTop;
				double ilw= ite->visualLineWidth();
				double x2 = ite->BoundingX - ilw / 2.0;
				double y2 = ite->BoundingY - ilw / 2.0;
				double w2 = ite->BoundingW + ilw;
				double h2 = ite->BoundingH + ilw;
				if (!QRectF(x2, y2, w2, h2).intersects(QRectF(x, y, w, h1)))
					continue;
				if (ite->ChangedMasterItem)
					continue;
				if ((!pag->pageName().isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
					continue;
				PutPage("q\n");
				if ((ite->doOverprint) && (!Options.UseRGB))
				{
					QByteArray ShName = ResNam + Pdf::toPdf(ResCount);
					ResCount++;
					Transpar[ShName] = writeGState("/OP true\n"
												   "/op true\n"
												   "/OPM 1\n");
					PutPage(Pdf::toName(ShName) + " gs\n");
				}
/* Bookmarks on Master Pages do not make any sense */
//				if ((ite->isBookmark) && (Options.Bookmarks))
//					PDF_Bookmark(ite, pag->height() - (ite->yPos() - pag->yOffset()));
				if (!ite->printEnabled() || ((ite->itemType() == PageItem::TextFrame) && (!pag->pageName().isEmpty())))
				{
					PutPage("Q\n");
					continue;
				}
				if (ite->fillColor() != CommonStrings::None)
					PutPage(putColor(ite->fillColor(), ite->fillShade(), true));
				if (ite->lineColor() != CommonStrings::None)
					PutPage(putColor(ite->lineColor(), ite->lineShade(), false));
				Content += FToStr(fabs(ite->lineWidth()))+" w\n";
				if (ite->DashValues.count() != 0)
				{
					PutPage("[ ");
					QVector<double>::iterator it;
					for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
					{
						double da = *it;
						// #8758: Custom dotted lines don't export properly to pdf
						// Null values have to be exported if line end != flat
						if ((da != 0) || (ite->lineEnd() != Qt::FlatCap))
							PutPage(Pdf::toPdf(da)+" ");
					}
					PutPage("] " + Pdf::toPdf(ite->DashOffset)+" d\n");
				}
				else
					PutPage("[" + Pdf::toAscii(getDashString(ite->PLineArt, ite->lineWidth())) + "] 0 d\n");
				switch (ite->PLineEnd)
				{
					case Qt::FlatCap:
						PutPage("0 J\n");
						break;
					case Qt::SquareCap:
						PutPage("2 J\n");
						break;
					case Qt::RoundCap:
						PutPage("1 J\n");
						break;
					default:
						PutPage("0 J\n");
						break;
				}
				switch (ite->PLineJoin)
				{
					case Qt::MiterJoin:
						PutPage("0 j\n");
						break;
					case Qt::BevelJoin:
						PutPage("2 j\n");
						break;
					case Qt::RoundJoin:
						PutPage("1 j\n");
						break;
					default:
						PutPage("0 j\n");
						break;
				}
				PutPage("1 0 0 1 "+FToStr(ite->xPos() - pag->xOffset())+" "+FToStr(pag->height() - (ite->yPos()  - pag->yOffset()))+" cm\n");
				if (ite->rotation() != 0)
				{
					double sr = sin(-ite->rotation()* M_PI / 180.0);
					double cr = cos(-ite->rotation()* M_PI / 180.0);
					if ((cr * cr) < 0.000001)
						cr = 0;
					if ((sr * sr) < 0.000001)
						sr = 0;
					PutPage(FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+" 0 0 cm\n");
				}
				PutPage(PDF_PutSoftShadow(ite,pag));
				switch (ite->itemType())
				{
					case PageItem::ImageFrame:
					case PageItem::LatexFrame:
						PutPage("q\n");
						// Same functions as for ImageFrames work for LatexFrames too
						if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
						{
							PutPage(PDF_TransparenzFill(ite));
						}
						if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
						{
							if (ite->GrType != 0)
							{
								if (ite->GrType == 8)
								{
									if (!PDF_PatternFillStroke(tmpOut, ite))
										return false;
								}
								else if (ite->GrType == 14)
								{
									if (!PDF_HatchFill(tmpOut, ite))
										return false;
								}
								else
								{
									if (!PDF_GradientFillStroke(tmpOut, ite))
										return false;
								}
								if (!tmpOut.isEmpty())
								{
									PutPage("q\n");
									if (ite->GrType == 14)
										PutPage(tmpOut);
									else
									{
										PutPage(tmpOut);
										PutPage(SetClipPath(ite));
										PutPage(ite->fillRule ? "h\nf*\n" : "h\nf\n");
									}
									PutPage("Q\n");
								}
							}
							else
							{
								if (ite->fillColor() != CommonStrings::None)
								{
									PutPage(SetClipPath(ite));
									PutPage(ite->fillRule ? "h\nf*\n" : "h\nf\n");
								}
							}
						}
						PutPage("q\n");
						PutPage(SetPathAndClip(ite, true));
						if (ite->imageFlippedH())
							PutPage("-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n");
						if (ite->imageFlippedV())
							PutPage("1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n");
						if (ite->imageClip.size() != 0)
							PutPage(SetImagePathAndClip(ite));
						if ((ite->imageIsAvailable) && (!ite->Pfile.isEmpty()))
						{
							if (!PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), false, ite->IProfile, ite->UseEmbedded, ite->IRender, &tmpOut))
								return false;
							PutPage(tmpOut);
						}
						PutPage("Q\n");
						PutPage("Q\n");
						if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0)))
						{
							if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
								PutPage(PDF_TransparenzStroke(ite));
							if (ite->NamedLStyle.isEmpty())
							{
								ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
								if (strokePattern)
								{
									if (ite->patternStrokePath)
									{
										QPainterPath path = ite->PoLine.toQPainterPath(false);
										PutPage(handleBrushPattern(ite, path, pag, pag->pageNr()));
									}
									else
									{
										if (!PDF_PatternFillStroke(tmpOut, ite, 1))
											return false;
										PutPage(SetClipPath(ite));
										PutPage(tmpOut);
										PutPage("h\nS\n");
									}
								}
								else if (ite->GrTypeStroke > 0)
								{
									if (!PDF_GradientFillStroke(tmpOut, ite, true))
										return false;
									PutPage("q\n");
									PutPage(tmpOut);
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
									PutPage("Q\n");
								}
								else if (ite->lineColor() != CommonStrings::None)
								{
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
								}
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
									{
										PutPage(setStrokeMulti(&ml[it]));
										PutPage(SetClipPath(ite));
										PutPage("h\nS\n");
									}
								}
							}
						}
						break;
					case PageItem::TextFrame:
						break;
					case PageItem::Line:
						if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
							PutPage(PDF_TransparenzStroke(ite));
						if (ite->NamedLStyle.isEmpty())
						{
							ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
							if (strokePattern)
							{
								if (ite->patternStrokePath)
								{
									QPainterPath path;
									path.moveTo(0, 0);
									path.lineTo(ite->width(), 0);
									PutPage(handleBrushPattern(ite, path, pag, pag->pageNr()));
								}
								else
								{
									if (!PDF_PatternFillStroke(tmpOut, ite, 1))
										return false;
									PutPage(tmpOut);
									PutPage("0 0 m\n");
									PutPage(FToStr(ite->width())+" 0 l\n");
									PutPage("S\n");
								}
							}
							else if (ite->GrTypeStroke > 0)
							{
								if (!PDF_GradientFillStroke(tmpOut, ite, true))
									return false;
								PutPage("q\n");
								PutPage(tmpOut);
								PutPage("0 0 m\n");
								PutPage(FToStr(ite->width())+" 0 l\n");
								PutPage("S\nQ\n");
							}
							else if (ite->lineColor() != CommonStrings::None)
							{
								PutPage("0 0 m\n");
								PutPage(FToStr(ite->width())+" 0 l\n");
								PutPage("S\n");
							}
						}
						else
						{
							multiLine ml = doc.MLineStyles[ite->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
								if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
								{
									PutPage(setStrokeMulti(&ml[it]));
									PutPage("0 0 m\n");
									PutPage(FToStr(ite->width())+" 0 l\n");
									PutPage("S\n");
								}
							}
						}
						if (ite->startArrowIndex() != 0)
						{
							QTransform arrowTrans;
							arrowTrans.scale(-1,1);
							arrowTrans.scale(ite->startArrowScale() / 100.0, ite->startArrowScale() / 100.0);
							PutPage(drawArrow(ite, arrowTrans, ite->startArrowIndex()));
						}
						if (ite->endArrowIndex() != 0)
						{
							QTransform arrowTrans;
							arrowTrans.translate(ite->width(), 0);
							arrowTrans.scale(ite->endArrowScale() / 100.0, ite->endArrowScale() / 100.0);
							PutPage(drawArrow(ite, arrowTrans, ite->endArrowIndex()));
						}
						break;
					case PageItem::ItemType1:
					case PageItem::ItemType3:
					case PageItem::Polygon:
					case PageItem::RegularPolygon:
					case PageItem::Arc:
						PutPage("q\n");
						if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
						{
							PutPage(PDF_TransparenzFill(ite));
						}
						if (ite->GrType != 0)
						{
							if (ite->GrType == 8)
							{
								if (!PDF_PatternFillStroke(tmpOut, ite))
									return false;
							}
							else if (ite->GrType == 14)
							{
								if (!PDF_HatchFill(tmpOut, ite))
									return false;
							}
							else
							{
								if (!PDF_GradientFillStroke(tmpOut, ite))
									return false;
							}
							if (!tmpOut.isEmpty())
							{
								if (ite->GrType == 14)
									PutPage(tmpOut);
								else
								{
									PutPage(tmpOut);
									PutPage(SetClipPath(ite));
									PutPage(ite->fillRule ? "h\nf*\n" : "h\nf\n");
								}
							}
						}
						else
						{
							if (ite->fillColor() != CommonStrings::None)
							{
								PutPage(SetClipPath(ite));
								PutPage(ite->fillRule ? "h\nf*\n" : "h\nf\n");
							}
						}
						PutPage("Q\n");
						if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
						{
							if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
								PutPage(PDF_TransparenzStroke(ite));
							if ((ite->NamedLStyle.isEmpty()) && (ite->lineWidth() != 0.0))
							{
								ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
								if (strokePattern)
								{
									if (ite->patternStrokePath)
									{
										QPainterPath path = ite->PoLine.toQPainterPath(false);
										PutPage(handleBrushPattern(ite, path, pag, pag->pageNr()));
									}
									else
									{
										if (!PDF_PatternFillStroke(tmpOut, ite, 1))
											return false;
										PutPage(tmpOut);
										PutPage("h\nS\n");
									}
								}
								else if (ite->GrTypeStroke > 0)
								{
									if (!PDF_GradientFillStroke(tmpOut, ite, true))
										return false;
									PutPage("q\n");
									PutPage(tmpOut);
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
									PutPage("Q\n");
								}
								else if (ite->lineColor() != CommonStrings::None)
								{
									PutPage(SetClipPath(ite));
									PutPage("h\nS\n");
								}
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
									{
										PutPage(setStrokeMulti(&ml[it]));
										PutPage(SetClipPath(ite));
										PutPage("h\nS\n");
									}
								}
							}
						}
						break;
					case PageItem::PolyLine:
					case PageItem::Spiral:
						if (ite->PoLine.size() > 3) // && ((ite->PoLine.point(0) != ite->PoLine.point(1)) || (ite->PoLine.point(2) != ite->PoLine.point(3))))
						{
							PutPage("q\n");
							if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
							{
								PutPage(PDF_TransparenzFill(ite));
							}
							if (ite->GrType != 0)
							{
								if (ite->GrType == 8)
								{
									if (!PDF_PatternFillStroke(tmpOut, ite))
										return false;
								}
								else if (ite->GrType == 14)
								{
									if (!PDF_HatchFill(tmpOut, ite))
										return false;
								}
								else
								{
									if (!PDF_GradientFillStroke(tmpOut, ite))
										return false;
								}
								if (!tmpOut.isEmpty())
								{
									if (ite->GrType == 14)
										PutPage(tmpOut);
									else
									{
										PutPage(tmpOut);
										PutPage(SetClipPath(ite));
										PutPage(ite->fillRule ? "h\nf*\n" : "h\nf\n");
									}
								}
							}
							else
							{
								if (ite->fillColor() != CommonStrings::None)
								{
									PutPage(SetClipPath(ite));
									PutPage(ite->fillRule ? "h\nf*\n" : "h\nf\n");
								}
							}
							PutPage("Q\n");
						}
						if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
						{
							if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
								PutPage(PDF_TransparenzStroke(ite));
							if (ite->NamedLStyle.isEmpty())
							{
								ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
								if (strokePattern)
								{
									if (ite->patternStrokePath)
									{
										QPainterPath path = ite->PoLine.toQPainterPath(false);
										PutPage(handleBrushPattern(ite, path, pag, pag->pageNr()));
									}
									else
									{
										if (!PDF_PatternFillStroke(tmpOut, ite, 1))
											return false;
										PutPage(tmpOut);
										PutPage(SetClipPath(ite, false));
										PutPage("S\n");
									}
								}
								else if (ite->GrTypeStroke > 0)
								{
									if (!PDF_GradientFillStroke(tmpOut, ite, true))
										return false;
									PutPage("q\n");
									PutPage(tmpOut);
									PutPage(SetClipPath(ite, false));
									PutPage("S\n");
									PutPage("Q\n");
								}
								else if (ite->lineColor() != CommonStrings::None)
								{
									PutPage(SetClipPath(ite, false));
									PutPage("S\n");
								}
							}
							else
							{
								multiLine ml = doc.MLineStyles[ite->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
									{
										PutPage(setStrokeMulti(&ml[it]));
										PutPage(SetClipPath(ite, false));
										PutPage("S\n");
									}
								}
							}
						}
						if (ite->startArrowIndex() != 0)
						{
							FPoint Start = ite->PoLine.point(0);
							for (int xx = 1; xx < ite->PoLine.size(); xx += 2)
							{
								FPoint Vector = ite->PoLine.point(xx);
								if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
								{
									double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
									QTransform arrowTrans;
									arrowTrans.translate(Start.x(), Start.y());
									arrowTrans.rotate(r);
									arrowTrans.scale(ite->startArrowScale() / 100.0, ite->startArrowScale() / 100.0);
									PutPage(drawArrow(ite, arrowTrans, ite->startArrowIndex()));
									break;
								}
							}
						}
						if (ite->endArrowIndex() != 0)
						{
							FPoint End = ite->PoLine.point(ite->PoLine.size()-2);
							for (uint xx = ite->PoLine.size()-1; xx > 0; xx -= 2)
							{
								FPoint Vector = ite->PoLine.point(xx);
								if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
								{
									double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
									QTransform arrowTrans;
									arrowTrans.translate(End.x(), End.y());
									arrowTrans.rotate(r);
									arrowTrans.scale(ite->endArrowScale() / 100.0, ite->endArrowScale() / 100.0);
									PutPage(drawArrow(ite, arrowTrans, ite->endArrowIndex()));
									break;
								}
							}
						}
						break;
					case PageItem::PathText:
					case PageItem::OSGFrame:
					case PageItem::Table:
						break;
					case PageItem::Symbol:
						if (doc.docPatterns.contains(ite->pattern()))
						{
							QByteArray tmpD = "";
							ScPattern pat = doc.docPatterns[ite->pattern()];
							PutPage("q\n");
							PutPage(SetPathAndClip(ite));
							if (ite->imageFlippedH())
								PutPage("-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n");
							if (ite->imageFlippedV())
								PutPage("1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n");
							QTransform trans;
							trans.scale(ite->width() / pat.width, ite->height() / pat.height);
							trans.translate(0.0, -ite->height());
					//		trans.translate(pat.items.at(0)->gXpos, -pat.items.at(0)->gYpos);
							PutPage(FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n");
							for (int em = 0; em < pat.items.count(); ++em)
							{
								PageItem* embedded = pat.items.at(em);
								tmpD += "q\n";
								tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
								QByteArray output;
								if (!PDF_ProcessItem(output, embedded, pag, pag->pageNr(), true))
									return false;
								tmpD += output;
								tmpD += "Q\n";
							}
							if (Options.Version >= PDFOptions::PDFVersion_14 || Options.Version == PDFOptions::PDFVersion_X4)
								PutPage(Write_TransparencyGroup(ite->fillTransparency(), ite->fillBlendmode(), tmpD, ite));
							else
								PutPage(tmpD);
							PutPage("Q\n");
						}
						break;
					case PageItem::Group:
						if (ite->groupItemList.count() > 0)
						{
							QByteArray tmpD;
							PutPage("q\n");
							if (ite->groupClipping())
								PutPage(SetPathAndClip(ite));
							if (ite->imageFlippedH())
								PutPage("-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n");
							if (ite->imageFlippedV())
								PutPage("1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n");
							QTransform trans;
							trans.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
							trans.translate(0.0, -ite->height());
							PutPage(FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n");
							groupStackPos.push(QPointF(ite->xPos(), ite->height()));
							for (int em = 0; em < ite->groupItemList.count(); ++em)
							{
								PageItem* embedded = ite->groupItemList.at(em);
								tmpD += "q\n";
								tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
								QByteArray output;
								patternStackPos.push(QPointF(embedded->gXpos, ite->height() - embedded->gYpos));
								inPattern++; // We are not really exporting a pattern, but that fix gradient export
								if (!PDF_ProcessItem(output, embedded, pag, pag->pageNr(), true, true))
									return false;
								inPattern--;
								patternStackPos.pop();
								tmpD += output;
								tmpD += "Q\n";
							}
							groupStackPos.pop();
							if (Options.Version >= PDFOptions::PDFVersion_14 || Options.Version == PDFOptions::PDFVersion_X4)
								PutPage(Write_TransparencyGroup(ite->fillTransparency(), ite->fillBlendmode(), tmpD, ite));
							else
								PutPage(Write_FormXObject(tmpD, ite));
							PutPage("Q\n");
						}
						break;
					case PageItem::Multiple:
						Q_ASSERT(false);
						break;
					default:
						break;
				}
				PutPage("Q\n");
				PdfId templateObject = writer.newObject();
				writer.startObj(templateObject);
				PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
				double bleedRight = 0.0;
				double bleedLeft  = 0.0;
				getBleeds(ActPageP, bleedLeft, bleedRight);
				double maxBoxX = ActPageP->width()+bleedRight+bleedLeft;
				double maxBoxY = ActPageP->height()+Options.bleeds.top()+Options.bleeds.bottom();
				PutDoc("/BBox [ "+FToStr(-bleedLeft)+" "+FToStr(-Options.bleeds.bottom())+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" ]\n");
//				PutDoc("/BBox [ 0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" ]\n");
				
				Pdf::ResourceDictionary dict;
				dict.XObject.unite(pageData.ImgObjects);
				dict.XObject.unite(pageData.XObjects);
				dict.Font = pageData.FObjects;
				dict.Shading = Shadings;
				dict.Pattern = Patterns;
				dict.ExtGState = Transpar;
				dict.ColorSpace.append(asColorSpace(ICCProfiles.values()));
				dict.ColorSpace.append(asColorSpace(spotMap.values()));
				writer.write("/Resources ");
				writer.write(dict);
				
				if (Options.Compress)
					Content = CompressArray(Content);
				PutDoc("/Length "+Pdf::toPdf(Content.length()+1));
				if (Options.Compress)
					PutDoc("\n/Filter /FlateDecode");
				PutDoc(" >>\nstream\n"+EncStream(Content, templateObject)+"\nendstream");
				writer.endObj(templateObject);
				
				int pIndex = doc.MasterPages.indexOf((ScPage* const) pag) + 1;
				QByteArray name = QByteArray("master_page_obj_%1_%2")
				                            .replace("%1", Pdf::toPdf(pIndex))
				                            .replace("%2", Pdf::toPdf(qHash(ite)));
				pageData.XObjects[name] = templateObject;
			}
			if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
				PutPage("EMC\n");
		}
		Lnr++;
	}
	return true;
}



void PDFLibCore::PDF_Begin_Page(const ScPage* pag, const QImage& thumb)
{
	ActPageP = pag;
	Content = "";
	pageData.AObjects.clear();
	pageData.radioButtonList.clear();
	if (Options.Thumbnails)
	{
		ScImage img(thumb);
		bool compDataAvail = false;
		QByteArray array = img.ImageToArray();
		if (Options.Compress)
		{
			QByteArray compArray = CompressArray(array);
			if (compArray.size() > 0)
			{
				array = compArray;
				compDataAvail = true;
			}
		}
		PdfId thumbnail = writer.newObject();
		writer.startObj(thumbnail);
		PutDoc("<<\n/Width "+Pdf::toPdf(img.width())+"\n");
		PutDoc("/Height "+Pdf::toPdf(img.height())+"\n");
		PutDoc("/ColorSpace /DeviceRGB\n/BitsPerComponent 8\n");

		PutDoc("/Length "+Pdf::toPdf(array.size()+1)+"\n");
		if (Options.Compress && compDataAvail)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n");
		EncodeArrayToStream(array, thumbnail);
		PutDoc("\nendstream");
		writer.endObj(thumbnail);
		pageData.Thumb = thumbnail;
	}
}

void PDFLibCore::PDF_End_Page()
{
	if (!pageData.radioButtonList.isEmpty())
		PDF_RadioButtons();
	uint PgNr =  ActPageP->pageNr();
	double markOffs = 0.0;
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
		markOffs = Options.markLength + Options.markOffset;
	if ((Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
		markOffs = qMax(markOffs, Options.markOffset + 20.0);
	double markDelta = markOffs - (Options.markLength + Options.markOffset);
	double bleedRight, bleedLeft;
	getBleeds(ActPageP, bleedLeft, bleedRight);
	double maxBoxX = ActPageP->width()+bleedLeft+bleedRight+markOffs*2.0;
	double maxBoxY = ActPageP->height()+Options.bleeds.bottom()+Options.bleeds.top()+markOffs*2.0;
	// (JG) Fix #5977 and #6075 (invalid restore)
	//PutPage("Q\n");
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
	{
		PutPage("0.5 w 0 j 0 J [] 0 d\n");
		PutPage("/"+spotMapReg["Register"].ResName+" CS 1 SCN\n");
		if (Options.cropMarks)
		{
		// Bottom Left
			PutPage(FToStr(markDelta)+" "+FToStr(markOffs+Options.bleeds.bottom())+" m\n");
			PutPage(FToStr(markDelta+Options.markLength)+" "+FToStr(markOffs+Options.bleeds.bottom())+" l\n");
			PutPage("S\n");
			PutPage(FToStr(markOffs+bleedLeft)+" "+FToStr(markDelta)+" m\n");
			PutPage(FToStr(markOffs+bleedLeft)+" "+FToStr(markDelta+Options.markLength)+" l\n");
			PutPage("S\n");
		// Top Left
			PutPage(FToStr(markDelta)+" "+FToStr(maxBoxY-Options.bleeds.top()-markOffs)+" m\n");
			PutPage(FToStr(markDelta+Options.markLength)+" "+FToStr(maxBoxY-Options.bleeds.top()-markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(markOffs+bleedLeft)+" "+FToStr(maxBoxY-markDelta)+" m\n");
			PutPage(FToStr(markOffs+bleedLeft)+" "+FToStr(maxBoxY-markDelta-Options.markLength) +" l\n");
			PutPage("S\n");
		// Bottom Right
			PutPage(FToStr(maxBoxX-markDelta)+" "+FToStr(markOffs+Options.bleeds.bottom())+" m\n");
			PutPage(FToStr(maxBoxX-markDelta-Options.markLength)+" "+FToStr(markOffs+Options.bleeds.bottom())+" l\n");
			PutPage("S\n");
			PutPage(FToStr(maxBoxX-bleedRight-markOffs)+" "+ FToStr(markDelta)+" m\n");
			PutPage(FToStr(maxBoxX-bleedRight-markOffs)+" "+ FToStr(markDelta+Options.markLength) +" l\n");
			PutPage("S\n");
		// Top Right
			PutPage(FToStr(maxBoxX-markDelta)+" "+FToStr(maxBoxY-Options.bleeds.top()-markOffs)+" m\n");
			PutPage(FToStr(maxBoxX-markDelta-Options.markLength)+" "+FToStr(maxBoxY-Options.bleeds.top()-markOffs)+" l\n");
			PutPage("S\n");
 			PutPage(FToStr(maxBoxX-bleedRight-markOffs)+" "+ FToStr(maxBoxY-markDelta)+" m\n");
			PutPage(FToStr(maxBoxX-bleedRight-markOffs)+" "+ FToStr(maxBoxY-markDelta-Options.markLength) +" l\n");
			PutPage("S\n");
		}
		if (Options.bleedMarks)
		{
			PutPage("q\n");
			PutPage("[3 1 1 1] 0 d\n");
		// Bottom Left
			PutPage(FToStr(markDelta)+" "+FToStr(markOffs)+" m\n");
			PutPage(FToStr(markDelta+Options.markLength)+" "+FToStr(markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(markOffs)+" "+FToStr(markDelta)+" m\n");
			PutPage(FToStr(markOffs)+" "+FToStr(markDelta+Options.markLength)+" l\n");
			PutPage("S\n");
		// Top Left
			PutPage(FToStr(markDelta)+" "+FToStr(maxBoxY-markOffs)+" m\n");
			PutPage(FToStr(markDelta+Options.markLength)+" "+FToStr(maxBoxY-markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(markOffs)+" "+FToStr(maxBoxY-markDelta)+" m\n");
			PutPage(FToStr(markOffs)+" "+FToStr(maxBoxY-markDelta-Options.markLength)+" l\n");
			PutPage("S\n");
		// Bottom Right
			PutPage(FToStr(maxBoxX-markDelta)+" "+FToStr(markOffs)+" m\n");
			PutPage(FToStr(maxBoxX-markDelta-Options.markLength)+" "+FToStr(markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(maxBoxX-markOffs)+" "+FToStr(markDelta)+" m\n");
			PutPage(FToStr(maxBoxX-markOffs)+" "+FToStr(markDelta+Options.markLength)+" l\n");
			PutPage("S\n");
		// Top Right
			PutPage(FToStr(maxBoxX-markDelta)+" "+FToStr(maxBoxY-markOffs)+" m\n");
			PutPage(FToStr(maxBoxX-markDelta-Options.markLength)+" "+FToStr(maxBoxY-markOffs)+" l\n");
			PutPage("S\n");
			PutPage(FToStr(maxBoxX-markOffs)+" "+FToStr(maxBoxY-markDelta)+" m\n");
			PutPage(FToStr(maxBoxX-markOffs)+" "+FToStr(maxBoxY-markDelta-Options.markLength)+" l\n");
			PutPage("S\n");
			PutPage("Q\n");
		}
		if (Options.registrationMarks)
		{
			double regDelta  = markOffs - Options.markOffset;
			QByteArray regCross = "0 7 m\n14 7 l\nh\n7 0 m\n7 14 l\nh\n13 7 m\n13 10.31383 10.31383 13 7 13 c\n3.68629 13 1 10.31383 1 7 c\n1 3.68629 3.68629 1 7 1 c\n";
			regCross += "10.31383 1 13 3.68629 13 7 c\nh\n10.5 7 m\n10.5 8.93307 8.93307 10.5 7 10.5 c\n5.067 10.5 3.5 8.93307 3.5 7 c\n";
			regCross += "3.5 5.067 5.067 3.5 7 3.5 c\n8.93307 3.5 10.5 5.067 10.5 7 c\nh\nS\n";
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(maxBoxX / 2.0 - 7.0)+" "+FToStr(regDelta - 17)+" cm\n");
			PutPage(regCross);
			PutPage("Q\n");
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(regDelta - 17)+" "+FToStr(maxBoxY / 2.0 - 7.0)+" cm\n");
			PutPage(regCross);
			PutPage("Q\n");
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(maxBoxX / 2.0 - 7.0)+" "+FToStr(maxBoxY - regDelta + 3.0)+" cm\n");
			PutPage(regCross);
			PutPage("Q\n");
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(maxBoxX - regDelta + 3.0)+" "+FToStr(maxBoxY / 2.0 - 7.0)+" cm\n");
			PutPage(regCross);
			PutPage("Q\n");
		}
		if (Options.colorMarks)
		{
			double startX = markOffs+bleedLeft+6.0;
			double startY = maxBoxY - markOffs + Options.markOffset + 2.0;
			PutPage("0 0 0 1 K\n");
			double col = 1.0;
			for (int bl = 0; bl < 11; bl++)
			{
				PutPage("0 0 0 "+FToStr(col)+" k\n");
				PutPage(FToStr(startX+bl*14.0)+" "+FToStr(startY)+" 14 14 re B\n");
				col -= 0.1;
			}
			if (!Options.isGrayscale)
			{
				startX = maxBoxX-bleedRight-markOffs-20.0;
				PutPage("0 0 0 0.5 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 0 0.5 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 0.5 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0.5 0 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("1 1 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("1 0 1 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 1 1 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 0 0 1 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 0 1 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("0 1 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
				startX -= 14.0;
				PutPage("1 0 0 0 k\n");
				PutPage(FToStr(startX)+" "+FToStr(startY)+" 14 14 re B\n");
			}
		}
		if (Options.docInfoMarks)
		{
//			QString tmp = "";
//			double startX = markOffs+bleedLeft+10.0;
			FPointArray  textPath;
			QPainterPath painter1, painter2;
			QFont   infoFont("Helvetica", 7);
			double  startX = markOffs + bleedLeft + 10.0;
			double  startY = markOffs - Options.markOffset - 14;
			QString docTitle = doc.documentInfo().title();
			if (docTitle.isEmpty())
			{
				QFileInfo fi(doc.DocName);
				docTitle = fi.fileName();
			}
//			docTitle += "  "+ tr("Page:")+" "+tmp.setNum(PgNr+1);
			docTitle += "  "+ tr("Page:")+" "+ Pdf::toPdf(PgNr+1);
			PutPage("/"+spotMapReg["Register"].ResName+" cs 1 scn\n");
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(startX)+" "+FToStr(startY)+" cm\n");
			painter1.addText( QPointF(0.0,0.0), infoFont, docTitle );
			textPath.fromQPainterPath(painter1);
			PutPage(SetClipPathArray(&textPath, true));
			PutPage("h\nf*\n");
			PutPage("Q\n");
			QDate d = QDate::currentDate();
			QString docDate = tr("Date:")+" "+d.toString(Qt::TextDate);
			PutPage("q\n");
			PutPage("1 0 0 1 "+FToStr(maxBoxX / 2.0 + 20.0)+" "+FToStr(startY)+" cm\n");
			painter2.addText( QPointF(0.0,0.0), infoFont, docDate );
			textPath.fromQPainterPath(painter2);
			PutPage(SetClipPathArray(&textPath, true));
			PutPage("h\nf*\n");
			PutPage("Q\n");
		}
	}
	pageData.ObjNum = WritePDFStream(Content);
	int Gobj = 0;
	if ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4))
	{
		Gobj = writer.newObject();
		writer.startObj(Gobj);
		PutDoc("<< /S /Transparency\n");
		if (Options.UseRGB)
			PutDoc("/CS /DeviceRGB\n");
		else
		{
			if (Options.isGrayscale)
				PutDoc("/CS /DeviceGray\n");
			else
			{
				if ((doc.HasCMS) && (Options.UseProfiles))
					PutDoc("/CS " + ICCProfiles[Options.SolidProf].ICCArray + "\n");
				else
					PutDoc("/CS /DeviceCMYK\n");
			}
		}
		PutDoc(">>");
		writer.endObj(Gobj);
	}
	PdfId pageObject = writer.newObject();
	writer.startObj(pageObject);
	PutDoc("<<\n/Type /Page\n/Parent " + Pdf::toObjRef(writer.PagesObj) + "\n");
	PutDoc("/MediaBox [0 0 "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+"]\n");
	PutDoc("/BleedBox ["+FToStr(markOffs)+" "+FToStr(markOffs)+" "+FToStr(maxBoxX-markOffs)+" "+FToStr(maxBoxY-markOffs)+"]\n");
	PutDoc("/CropBox [0 0 "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+"]\n");
	PutDoc("/TrimBox ["+FToStr(bleedLeft+markOffs)+" "+FToStr(Options.bleeds.bottom()+markOffs)+" "+FToStr(maxBoxX-bleedRight-markOffs)+" "+FToStr(maxBoxY-Options.bleeds.top()-markOffs)+"]\n");
	if (Options.Version >= PDFOptions::PDFVersion_13) // PDF/X forbids having both art and trim box!
		PutDoc("/ArtBox ["+FToStr(bleedLeft+markOffs)+" "+FToStr(Options.bleeds.bottom()+markOffs)+" "+FToStr(maxBoxX-bleedRight-markOffs)+" "+FToStr(maxBoxY-Options.bleeds.top()-markOffs)+"]\n");
	PutDoc("/Rotate "+Pdf::toPdf(Options.RotateDeg)+"\n");
	PutDoc("/Contents "+Pdf::toPdf(pageData.ObjNum)+" 0 R\n");
	if ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) // && (Transpar.count() != 0))
		PutDoc("/Group "+Pdf::toPdf(Gobj)+" 0 R\n");
	if (Options.Thumbnails)
		PutDoc("/Thumb "+Pdf::toPdf(pageData.Thumb)+" 0 R\n");
	if (pageData.AObjects.count() != 0)
	{
		PutDoc("/Annots [ ");
		for (int b = 0; b < pageData.AObjects.count(); ++b)
			PutDoc(Pdf::toPdf(pageData.AObjects[b])+" 0 R ");
		PutDoc("]\n");
	}
	if (Options.PresentMode)
	{
		if (ActPageP->PresentVals.pageViewDuration > 0)
			PutDoc("/Dur "+Pdf::toPdf(ActPageP->PresentVals.pageViewDuration)+"\n");
		if (ActPageP->PresentVals.effectType != 0)
		{
			PutDoc("/Trans << /Type /Trans\n");
			PutDoc("/D "+Pdf::toPdf(ActPageP->PresentVals.pageEffectDuration)+"\n");
			switch (ActPageP->PresentVals.effectType)
			{
				case 1:
					PutDoc("/S /Blinds\n");
					PutDoc(ActPageP->PresentVals.Dm == 0 ? "/Dm /H\n" : "/Dm /V\n");
					break;
				case 2:
					PutDoc("/S /Box\n");
					PutDoc(ActPageP->PresentVals.M == 0 ? "/M /I\n" : "/M /O\n");
					break;
				case 3:
					PutDoc("/S /Dissolve\n");
					break;
				case 4:
					PutDoc("/S /Glitter\n");
					PutDoc("/Di ");
					switch (ActPageP->PresentVals.Di)
					{
						case 0:
							PutDoc("0");
							break;
						case 1:
							PutDoc("270");
							break;
						case 4:
							PutDoc("315");
							break;
						default:
							PutDoc("0");
							break;
					}
					PutDoc("\n");
					break;
				case 5:
					PutDoc("/S /Split\n");
					PutDoc(ActPageP->PresentVals.Dm == 0 ? "/Dm /H\n" : "/Dm /V\n");
					PutDoc(ActPageP->PresentVals.M == 0 ? "/M /I\n" : "/M /O\n");
					break;
				case 6:
					PutDoc("/S /Wipe\n");
					PutDoc("/Di ");
					switch (ActPageP->PresentVals.Di)
					{
						case 0:
							PutDoc("0");
							break;
						case 1:
							PutDoc("270");
							break;
						case 2:
							PutDoc("90");
							break;
						case 3:
							PutDoc("180");
							break;
						default:
							PutDoc("0");
							break;
					}
					PutDoc("\n");
					break;
				case 7:
					PutDoc("/S /Push\n");
					PutDoc("/Di ");
					switch (ActPageP->PresentVals.Di)
					{
						case 0:
							PutDoc("0");
							break;
						case 1:
							PutDoc("270");
							break;
						case 2:
							PutDoc("90");
							break;
						case 3:
							PutDoc("180");
							break;
						default:
							PutDoc("0");
							break;
					}
					PutDoc("\n");
					break;
				case 8:
					PutDoc("/S /Cover\n");
					PutDoc("/Di ");
					switch (ActPageP->PresentVals.Di)
					{
						case 0:
							PutDoc("0");
							break;
						case 1:
							PutDoc("270");
							break;
						case 2:
							PutDoc("90");
							break;
						case 3:
							PutDoc("180");
							break;
						default:
							PutDoc("0");
							break;
					}
					PutDoc("\n");
					break;
				case 9:
					PutDoc("/S /Uncover\n");
					PutDoc("/Di ");
					switch (ActPageP->PresentVals.Di)
					{
						case 0:
							PutDoc("0");
							break;
						case 1:
							PutDoc("270");
							break;
						case 2:
							PutDoc("90");
							break;
						case 3:
							PutDoc("180");
							break;
						default:
							PutDoc("0");
							break;
					}
					PutDoc("\n");
					break;
				case 10:
					PutDoc("/S /Fade\n");
					break;
			}
			PutDoc(">>\n");
		}
	}
	PutDoc(">>");
	writer.endObj(pageObject);
	PageTree.Kids.append(pageObject);
	PageTree.KidsMap[ActPageP->pageNr()] = pageObject;
}


void PDFLibCore::writeXObject(uint objNr, QByteArray dictionary, QByteArray stream)
{
	writer.startObj(objNr);
	PutDoc("<<");
	PutDoc(dictionary);
	PutDoc(">>\nstream\n");
	EncodeArrayToStream(stream, objNr);
	PutDoc("\nendstream");
	writer.endObj(objNr);
}


PdfId PDFLibCore::writeObject(QByteArray type, QByteArray dictionary)
{
	PdfId result = writer.newObject();
	writer.startObj(result);
	PutDoc("<<");
	if (!type.isEmpty())
		PutDoc("/Type " + type + "\n");
	PutDoc(dictionary);
	PutDoc(">>");
	writer.endObj(result);
	return result;
}


bool PDFLibCore::PDF_ProcessPage(const ScPage* pag, uint PNr, bool clip)
{
	ActPageP = pag;
	ScLayer ll;
	ll.isPrintable = false;
	if (Options.UseLPI)
		PutPage(Pdf::toName(HTName) + " gs\n");
	double bleedRight  = 0.0;
	double bleedLeft   = 0.0;
	double bleedBottom = 0.0;
	double bleedTop    = 0.0;
	double markOffs    = 0.0;
	bleedDisplacementX = 0.0;
	bleedDisplacementY = 0.0;
	PutPage("q\n"); // Save
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
		markOffs = Options.markLength + Options.markOffset;
	if ((Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
		markOffs = qMax(markOffs, Options.markOffset + 20.0);
	// #8773 - incorrect page position if MPageNam.isEmpty()
	/*if (!pag->MPageNam.isEmpty())
	{*/
		getBleeds(ActPageP, bleedLeft, bleedRight, bleedBottom, bleedTop);
		PutPage("1 0 0 1 "+FToStr(bleedLeft+markOffs)+" "+FToStr(Options.bleeds.bottom()+markOffs)+" cm\n");
		bleedDisplacementX = bleedLeft+markOffs;
		bleedDisplacementY = Options.bleeds.bottom()+markOffs;
	/*}*/
	//#9385 : clip to BleedBox
	if ((Options.cropMarks) || (Options.bleedMarks) || (Options.registrationMarks) || (Options.colorMarks) || (Options.docInfoMarks))
	{
		double bbWidth  = ActPageP->width()  + bleedLeft + bleedRight;
		double bbHeight = ActPageP->height() + bleedBottom + bleedTop;
		const char* B = " ";
		PutPage(FToStr(-bleedLeft) +B+ FToStr(-bleedBottom) +B+
				FToStr(bbWidth) +B+ FToStr(bbHeight) + " re W n\n");
	}
	if ( (Options.MirrorH) && (!pag->MPageNam.isEmpty()) )
		PutPage("-1 0 0 1 "+FToStr(ActPageP->width())+" 0 cm\n");
	if ( (Options.MirrorV) && (!pag->MPageNam.isEmpty()) )
		PutPage("1 0 0 -1 0 "+FToStr(ActPageP->height())+" cm\n");
	if (clip)
	{
		double maxBoxX = ActPageP->width() - ActPageP->Margins.right() - ActPageP->Margins.left();
		double maxBoxY = ActPageP->height() - ActPageP->Margins.top() - ActPageP->Margins.bottom();
		PutPage(FToStr(ActPageP->Margins.left())+" "+FToStr(ActPageP->Margins.bottom())+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" re W n\n");
	//	PutPage("0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" re W n\n");
	}
	//CB *2 because the Pitems count loop runs twice.. y.. dunno.
	if (usingGUI && pag->pageName().isEmpty())
		progressDialog->setProgress("ECPI", 0, doc.DocItems.count()*2);
	for (int lam = 0; lam < doc.Layers.count() && !abortExport; ++lam)
	{
		ll.isPrintable = false;
		doc.Layers.levelToLayer(ll, lam);
		if (!PDF_ProcessMasterElements(ll, pag, PNr))
			return false;
		if (!PDF_ProcessPageElements(ll, pag, PNr))
			return false;
	}
	PutPage("Q\n"); // Restore
	return true;
}

bool PDFLibCore::PDF_ProcessMasterElements(const ScLayer& layer, const ScPage* pag, uint PNr)
{
	PageItem* ite;
	QByteArray content, output;
//	QList<PageItem*> PItems;

	if (pag->MPageNam.isEmpty())
		return true;
	if (doc.MasterItems.count() <= 0)
		return true;
	const ScPage* mPage = doc.MasterPages.at(doc.MasterNames[doc.DocPages.at(PNr)->MPageNam]);
	int   mPageIndex  = doc.MasterPages.indexOf((ScPage* const) mPage) + 1;

	if (!Options.MirrorH)
		PutPage("1 0 0 1 0 0 cm\n");
	if ((layer.isPrintable) || (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers)))
	{
		if ((((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4))) && (Options.useLayers))
			PutPage("/OC /"+OCGEntries[layer.Name].Name+" BDC\n");
		for (int am = 0; am < pag->FromMaster.count() && !abortExport; ++am)
		{
			ite = pag->FromMaster.at(am);
			if (usingGUI)
				qApp->processEvents();
			if ((ite->LayerID != layer.ID) || (!ite->printEnabled()))
				continue;
			if ((!pag->pageName().isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
				continue;
			QByteArray name = QByteArray("/master_page_obj_%1_%2")
			                             .replace("%1", Pdf::toPdf(mPageIndex))
			                             .replace("%2", Pdf::toPdf(qHash(ite)));
			if ((!ite->asTextFrame()) && (!ite->asPathText()) && (!ite->asTable()))
			{
				if (((layer.transparency != 1) || (layer.blendMode != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
					content += (name + " Do\n");
				else
					PutPage(name + " Do\n");
			}
			else
			{
				double oldX = ite->xPos();
				double oldY = ite->yPos();
				double OldBX = ite->BoundingX;
				double OldBY = ite->BoundingY;
				ite->setXPos(ite->xPos() - mPage->xOffset() + pag->xOffset(), true);
				ite->setYPos(ite->yPos() - mPage->yOffset() + pag->yOffset(), true);
				if (!PDF_ProcessItem(output, ite, pag, pag->pageNr()))
					return false;
				if (((layer.transparency != 1) || (layer.blendMode != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
					content += output;
				else
					PutPage(output);
				ite->setXYPos(oldX, oldY, true);
				ite->BoundingX = OldBX;
				ite->BoundingY = OldBY;
			}
		}
		// Couldn't we use Write_TransparencyGroup() here?
		if (((layer.transparency != 1) || (layer.blendMode != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) ||(Options.Version == PDFOptions::PDFVersion_X4)))
		{
			PdfId Gobj = writer.newObject();
			writer.startObj(Gobj);
			PutDoc("<< /Type /Group\n");
			PutDoc("/S /Transparency\n");
			PutDoc("/I false\n");
			PutDoc("/K false\n");
			PutDoc(">>");
			writer.endObj(Gobj);
			QByteArray ShName = ResNam+QByteArray::number(ResCount);
			ResCount++;
			Transpar[ShName] = writeGState("/CA "+FToStr(layer.transparency)+"\n"
										   + "/ca "+FToStr(layer.transparency)+"\n"
										   + "/SMask /None\n/AIS false\n/OPM 1\n"
										   + "/BM /" + blendMode(layer.blendMode) + "\n");
			PdfId formObject = writer.newObject();
			writer.startObj(formObject);
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
			double bleedRight = 0.0;
			double bleedLeft  = 0.0;
			getBleeds(ActPageP, bleedLeft, bleedRight);
			double maxBoxX = ActPageP->width()+bleedRight+bleedLeft;
			double maxBoxY = ActPageP->height()+Options.bleeds.top()+Options.bleeds.bottom();
			PutDoc("/BBox [ "+FToStr(-bleedLeft)+" "+FToStr(-Options.bleeds.bottom())+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" ]\n");
			PutDoc("/Group "+QByteArray::number(Gobj)+" 0 R\n");
			if (Options.Compress)
				content = CompressArray(content);
			PutDoc("/Length "+QByteArray::number(content.length()+1));
			if (Options.Compress)
				PutDoc("\n/Filter /FlateDecode");
			PutDoc(" >>\nstream\n"+EncStream(content, formObject)+"\nendstream");
			writer.endObj(formObject);
			QByteArray name = ResNam+QByteArray::number(ResCount);
			ResCount++;
			pageData.XObjects[name] = formObject;
			PutPage("q\n");
			PutPage("/"+ShName+" gs\n");
			PutPage("/"+name+" Do\n");
			PutPage("Q\n");
		}
		if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
			PutPage("EMC\n");
	}
	return true;
}

bool PDFLibCore::PDF_ProcessPageElements(const ScLayer& layer, const ScPage* pag, uint PNr)
{
	PageItem* ite;
	QByteArray output;
	QList<PageItem*> PItems;

	int pc_exportpagesitems = usingGUI ? progressDialog->progress("ECPI") : 0;
	PItems = (pag->pageName().isEmpty()) ? doc.DocItems : doc.MasterItems;
	if ((layer.isPrintable) || (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers)))
	{
		QByteArray inh = "";
		if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
			PutPage("/OC /"+OCGEntries[layer.Name].Name+" BDC\n");
		for (int a = 0; a < PItems.count() && !abortExport; ++a)
		{
			ite = PItems.at(a);
			if (ite->LayerID != layer.ID)
				continue;
			if (usingGUI)
			{
				progressDialog->setProgress("ECPI", ++pc_exportpagesitems);
				qApp->processEvents();
			}
			if (!PDF_ProcessItem(output, ite, pag, PNr))
				return false;
			if (((layer.transparency != 1) || (layer.blendMode != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
				inh += output;
			else
				PutPage(output);
		}
		// Couldn't we use Write_TransparencyGroup() here?
		if (((layer.transparency != 1) || (layer.blendMode != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) ||(Options.Version == PDFOptions::PDFVersion_X4)))
		{
			int Gobj = writer.newObject();
			writer.startObj(Gobj);
			PutDoc("<< /Type /Group\n");
			PutDoc("/S /Transparency\n");
			PutDoc("/I false\n");
			PutDoc("/K false\n");
			PutDoc(">>");
			writer.endObj(Gobj);
			QByteArray ShName = ResNam+Pdf::toPdf(ResCount);
			ResCount++;
			Transpar[ShName] = writeGState("/CA "+FToStr(layer.transparency)+"\n"
										   + "/ca "+FToStr(layer.transparency)+"\n"
										   + "/SMask /None\n/AIS false\n/OPM 1\n"
										   + "/BM /" + blendMode(layer.blendMode) + "\n");
			PdfId formObject = writer.newObject();
			writer.startObj(formObject);
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
			double bleedRight = 0.0;
			double bleedLeft  = 0.0;
			getBleeds(ActPageP, bleedLeft, bleedRight);
			double maxBoxX = ActPageP->width()+bleedRight+bleedLeft;
			double maxBoxY = ActPageP->height()+Options.bleeds.top()+Options.bleeds.bottom();
			PutDoc("/BBox [ "+FToStr(-bleedLeft)+" "+FToStr(-Options.bleeds.bottom())+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" ]\n");
			PutDoc("/Group "+Pdf::toPdf(Gobj)+" 0 R\n");
			if (Options.Compress)
				inh = CompressArray(inh);
			PutDoc("/Length "+Pdf::toPdf(inh.length()+1));
			if (Options.Compress)
				PutDoc("\n/Filter /FlateDecode");
			PutDoc(" >>\nstream\n"+EncStream(inh, formObject)+"\nendstream");
			writer.endObj(formObject);
			QByteArray name = Pdf::toPdfDocEncoding(layer.Name.simplified().replace(QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_")) + Pdf::toPdf(layer.ID) + Pdf::toPdf(PNr);
			pageData.XObjects[name] = formObject;
			PutPage("q\n");
			PutPage(Pdf::toName(ShName) + " gs\n");
			PutPage(Pdf::toName(name) + " Do\n");
			PutPage("Q\n");
		}
		if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
			PutPage("EMC\n");
	}
	return true;
}

QByteArray PDFLibCore::Write_FormXObject(QByteArray &data, PageItem *controlItem)
{
	QByteArray retString = "";
	PdfId formObject = writer.newObject();
	writer.startObj(formObject);
	PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
	double bleedRight = 0.0;
	double bleedLeft  = 0.0;
	getBleeds(ActPageP, bleedLeft, bleedRight);
	double maxBoxX = ActPageP->width()+bleedRight+bleedLeft;
	double maxBoxY = ActPageP->height()+Options.bleeds.top()+Options.bleeds.bottom();
	if (controlItem != NULL)
	{
		double groupW, groupH;
		if (controlItem->isGroup())
		{
			groupW = std::max(ActPageP->width(), std::max(controlItem->groupWidth,  controlItem->width()));
			groupH = std::max(controlItem->groupHeight, controlItem->height());
			PutDoc("/BBox [ "+FToStr(0)+" "+FToStr(-groupH)+" "+FToStr(groupW)+" "+FToStr(groupH)+" ]\n");
		}
		if (controlItem->isSymbol())
		{
			ScPattern pat = doc.docPatterns[controlItem->pattern()];
			groupW = std::max(pat.width,  controlItem->width());
			groupH = std::max(pat.height, controlItem->height());
			PutDoc("/BBox [ "+FToStr(0)+" "+FToStr(-groupH)+" "+FToStr(groupW)+" "+FToStr(groupH)+" ]\n");
		}
	}
	else
		PutDoc("/BBox [ "+FToStr(-bleedLeft)+" "+FToStr(-Options.bleeds.bottom())+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" ]\n");
	PutDoc("/Resources ");
	Pdf::ResourceDictionary dict;
	dict.XObject.unite(pageData.ImgObjects);
	dict.XObject.unite(pageData.XObjects);
	dict.Font = pageData.FObjects;
	dict.Shading = Shadings;
	dict.Pattern = Patterns;
	dict.ExtGState = Transpar;
	dict.ColorSpace.append(asColorSpace(ICCProfiles.values()));
	dict.ColorSpace.append(asColorSpace(spotMap.values()));
	writer.write(dict);

	if (Options.Compress)
		data = CompressArray(data);
	PutDoc("/Length "+QByteArray::number(data.length()+1));
	if (Options.Compress)
		PutDoc("\n/Filter /FlateDecode");
	PutDoc(" >>\nstream\n"+EncStream(data, formObject)+"\nendstream");
	writer.endObj(formObject);
	QByteArray name = ResNam+QByteArray::number(ResCount);
	ResCount++;
	pageData.XObjects[name] = formObject;
	retString += "q\n";
	retString += "/"+name+" Do\n";
	retString += "Q\n";
	return retString;
}

QByteArray PDFLibCore::Write_TransparencyGroup(double trans, int blend, QByteArray &data, PageItem *controlItem)
{
	QByteArray ShName = "";
	QByteArray retString = "";
	PdfId Gobj = writer.newObject();
	writer.startObj(Gobj);
	PutDoc("<< /Type /Group\n");
	PutDoc("/S /Transparency\n");
	if (controlItem->isGroup())
		PutDoc("/I true\n");
	else
		PutDoc("/I false\n");
	PutDoc("/K false\n");
	PutDoc(">>");
	writer.endObj(Gobj);
	if (controlItem != NULL)
	{
		retString += "q\n";
		retString += PDF_TransparenzFill(controlItem);
	}
	else
	{
		ShName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		Transpar[ShName] = writeGState("/CA "+FToStr(1.0 - trans)+"\n"
									+ "/ca "+FToStr(1.0 - trans)+"\n"
									+ "/SMask /None\n/AIS false\n/OPM 1\n"
									+ "/BM /" + blendMode(blend) + "\n");
		retString += "q\n";
		retString += Pdf::toName(ShName) + " gs\n";
	}
	PdfId formObject = writer.newObject();
	writer.startObj(formObject);
	PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1\n");
	double bleedRight = 0.0;
	double bleedLeft  = 0.0;
	getBleeds(ActPageP, bleedLeft, bleedRight);
	double maxBoxX = ActPageP->width()+bleedRight+bleedLeft;
	double maxBoxY = ActPageP->height()+Options.bleeds.top()+Options.bleeds.bottom();
	if (controlItem != NULL)
	{
		double groupW, groupH;
		if (controlItem->isGroup())
		{
			groupW = std::max(ActPageP->width(), std::max(controlItem->groupWidth,  controlItem->width()));
			groupH = std::max(controlItem->groupHeight, controlItem->height());
			PutDoc("/BBox [ "+FToStr(0)+" "+FToStr(-groupH)+" "+FToStr(groupW)+" "+FToStr(groupH)+" ]\n");
		}
		if (controlItem->isSymbol())
		{
			ScPattern pat = doc.docPatterns[controlItem->pattern()];
			groupW = std::max(pat.width,  controlItem->width());
			groupH = std::max(pat.height, controlItem->height());
			PutDoc("/BBox [ "+FToStr(0)+" "+FToStr(-groupH)+" "+FToStr(groupW)+" "+FToStr(groupH)+" ]\n");
		}
	}
	else
		PutDoc("/BBox [ "+FToStr(-bleedLeft)+" "+FToStr(-Options.bleeds.bottom())+" "+FToStr(maxBoxX)+" "+FToStr(maxBoxY)+" ]\n");
	PutDoc("/Group "+Pdf::toObjRef(Gobj)+"\n");
	PutDoc("/Resources ");
	Pdf::ResourceDictionary dict;
	dict.XObject.unite(pageData.ImgObjects);
	dict.XObject.unite(pageData.XObjects);
	dict.Font = pageData.FObjects;
	dict.Shading = Shadings;
	dict.Pattern = Patterns;
	dict.ExtGState = Transpar;
	dict.ColorSpace.append(asColorSpace(ICCProfiles.values()));
	dict.ColorSpace.append(asColorSpace(spotMap.values()));
	writer.write(dict);

	if (Options.Compress)
		data = CompressArray(data);
	PutDoc("/Length "+Pdf::toPdf(data.length()+1));
	if (Options.Compress)
		PutDoc("\n/Filter /FlateDecode");
	PutDoc(" >>\nstream\n" + EncStream(data, formObject) + "\nendstream");
	writer.endObj(formObject);
	QByteArray name = ResNam+Pdf::toPdf(ResCount);
	ResCount++;
	pageData.XObjects[name] = formObject;
	retString += Pdf::toName(name) + " Do\n";
	retString += "Q\n";
	return retString;
}

QByteArray PDFLibCore::PDF_PutSoftShadow(PageItem* ite, const ScPage *pag)
{
	if ((Options.Version < PDFOptions::PDFVersion_14 && Options.Version != PDFOptions::PDFVersion_X4) || !ite->hasSoftShadow() || ite->softShadowColor() == CommonStrings::None || !ite->printEnabled())
		return "";
	double maxSize;
	QByteArray tmp("q\n");
	double softShadowDPI = Options.Resolution;
	int pixelRadius = qRound(ite->softShadowBlurRadius() / 72.0 * softShadowDPI);
	tmp += "1 0 0 1 " + FToStr(ite->softShadowXOffset() - ite->softShadowBlurRadius()) + " " + FToStr(-(ite->softShadowYOffset() + ite->softShadowBlurRadius()))+" cm\n";
	if (ite->isPathText())
		ite->updatePolyClip();
	tmp += "1 0 0 1 " + FToStr(-(ite->xPos() - ite->visualXPos())) + " " + FToStr(ite->yPos() - ite->visualYPos())+" cm\n";
	tmp += "1 0 0 1 0 " + FToStr(-ite->visualHeight())+" cm\n";
	tmp += FToStr(ite->visualWidth() + 2 * ite->softShadowBlurRadius()) + " 0 0 " + FToStr(ite->visualHeight() + 2 * ite->softShadowBlurRadius())+" 0 0 cm\n" ;
	maxSize = qMax(ite->visualWidth(), ite->visualHeight());
	maxSize = qMin(3000.0, maxSize * (softShadowDPI / 72.0));
	bool saveControl = ite->doc()->guidesPrefs().showControls;
	ite->doc()->guidesPrefs().showControls = false;
	bool savedShadow = ite->hasSoftShadow();
	ite->setHasSoftShadow(false);
	double transF = ite->fillTransparency();
	double transS = ite->lineTransparency();
	ite->setFillTransparency(0.0);
	ite->setLineTransparency(0.0);
	QImage imgA = ite->DrawObj_toImage(maxSize, PageItem::NoRotation);
	ite->setFillTransparency(transF);
	ite->setLineTransparency(transS);
	QImage imgC = imgA.copy(-pixelRadius, -pixelRadius, imgA.width() + 2 * pixelRadius, imgA.height() + 2 * pixelRadius); // Add border
	ScPainter *p = new ScPainter(&imgC, imgC.width(), imgC.height(), 1, 0);
	p->setZoomFactor(softShadowDPI / 72.0);
	p->save();
	p->blur(pixelRadius);
	p->restore();
	p->end();
	delete p;
	if (ite->softShadowErasedByObject())
	{
		ScPainter *p = new ScPainter(&imgC, imgC.width(), imgC.height(), 1, 0);
		p->translate(pixelRadius, pixelRadius);
		p->translate(-ite->softShadowXOffset() * (softShadowDPI / 72.0), -ite->softShadowYOffset() * (softShadowDPI / 72.0));
		p->beginLayer(1.0, 18);
		p->drawImage(&imgA);
		p->endLayer();
		p->end();
		delete p;
	}

	ite->doc()->guidesPrefs().showControls = saveControl;
	ite->setHasSoftShadow(savedShadow);
	ScImage img = imgC.alphaChannel().convertToFormat(QImage::Format_RGB32);
	PdfId maskObj = writer.newObject();
	writer.startObj(maskObj);
	PutDoc("<<\n/Type /XObject\n/Subtype /Image\n");
	PutDoc("/Width "+Pdf::toPdf(img.width())+"\n");
	PutDoc("/Height "+Pdf::toPdf(img.height())+"\n");
	PutDoc("/ColorSpace /DeviceGray\n");
	PutDoc("/BitsPerComponent 8\n");
	uint lengthObj = writer.newObject();
	PutDoc("/Length "+Pdf::toPdf(lengthObj)+" 0 R\n");
	PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n");
	int bytesWritten = WriteFlateImageToStream(img, maskObj, ColorSpaceGray, false);
	PutDoc("\nendstream");
	writer.endObj(maskObj);
	writer.startObj(lengthObj);
	PutDoc("    " + Pdf::toPdf(bytesWritten));
	writer.endObj(lengthObj);

	PdfId colObj = writer.newObject();
	writer.startObj(colObj);
	PutDoc("<<\n/Type /XObject\n/Subtype /Image\n");
	PutDoc("/Width 1\n");
	PutDoc("/Height 1\n");
	PutDoc("/Interpolate false\n");
	PutDoc("/BitsPerComponent 8\n");
	PutDoc("/SMask "+Pdf::toPdf(maskObj)+" 0 R\n");

	ScImage col(1,1);
	QByteArray colstr = SetColor(ite->softShadowColor(), ite->softShadowShade());
	if (Options.isGrayscale)
	{
		double gf;
		int g;
		QTextStream ts(&colstr, QIODevice::ReadOnly);
		ts >> gf;
		g = qRound(gf * 255);
		col.imgInfo.colorspace = ColorSpaceGray;
		col.qImagePtr()->setPixel(0,0,qRgba(g,g,g,255));
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/Length 1\n");
		PutDoc(">>\nstream\n");
		WriteImageToStream(col, colObj, ColorSpaceGray, true);
		PutDoc("\nendstream");
	}
	else if (Options.UseRGB)
	{
		double r,g,b;
		QTextStream ts(&colstr, QIODevice::ReadOnly);
		ts >> r;
		ts >> g;
		ts >> b;
		col.imgInfo.colorspace = ColorSpaceRGB;
		col.qImagePtr()->setPixel(0, 0, qRgba(qRound(r*255), qRound(g*255), qRound(b*255), 255));
		PutDoc("/ColorSpace /DeviceRGB\n");
		PutDoc("/Length 3\n");
		PutDoc(">>\nstream\n");
		WriteImageToStream(col, colObj, ColorSpaceRGB, false);
		PutDoc("\nendstream");
	}
	else //CMYK
	{
		double c,m,y,k;
		QTextStream ts(&colstr, QIODevice::ReadOnly);
		ts >> c;
		ts >> m;
		ts >> y;
		ts >> k;
		col.imgInfo.colorspace = ColorSpaceCMYK;
		col.qImagePtr()->setPixel(0, 0, qRgba(qRound(c*255), qRound(m*255), qRound(y*255), qRound(k*255)));
		PutDoc("/ColorSpace /DeviceCMYK\n");
		PutDoc("/Length 4\n");
		PutDoc(">>\nstream\n");
		WriteImageToStream(col, colObj, ColorSpaceCMYK, false);
		PutDoc("\nendstream");
	}
	writer.endObj(colObj);
	QByteArray colRes = ResNam+Pdf::toPdf(ResCount);
	pageData.ImgObjects[colRes] = colObj;
	ResCount++;

	QByteArray ShName = ResNam+Pdf::toPdf(ResCount);
	ResCount++;
	if (ite->softShadowHasObjectTransparency())
	{
		Transpar[ShName] = writeGState("/ca "+FToStr(1.0 - ite->fillTransparency())+"\n"
									   + "/AIS false\n/OPM 1\n"
									   + "/BM /" + blendMode(ite->softShadowBlendMode()) + "\n");
	}
	else
	{
		Transpar[ShName] = writeGState("/ca "+FToStr(1.0 - ite->softShadowOpacity())+"\n"
									   + "/AIS false\n/OPM 1\n"
									   + "/BM /" + blendMode(ite->softShadowBlendMode()) + "\n");
	}
	tmp += Pdf::toName(ShName) + " gs\n";

	tmp += Pdf::toName(colRes) + " Do Q\n";
	return tmp;
}

/**
 * Fill this.output with the QByteArray representation of the item.
 * Checks if the item can be represented in the chosen PDF version, based on some of its caracteristics
 * and delegates the work to PDF_Image, PDF_GradientFillStroke, PDF_PatternFillStroke and so on for
 * filling this.output.
 * Returns false if the content can't be represented in the chosen PDF version,
 * if the image can't be loaded
 */
bool PDFLibCore::PDF_ProcessItem(QByteArray& output, PageItem* ite, const ScPage* pag, uint PNr, bool embedded, bool pattern)
{
	QByteArray tmp(""), tmpOut;
	if (ite->isGroup())
		ite->asGroupFrame()->adjustXYPosition();
	ite->setRedrawBounding();
	double bLeft, bRight, bBottom, bTop;
	getBleeds(pag, bLeft, bRight, bBottom, bTop);
	output.resize(0);
	if (!pattern)
	{
		double x1 = pag->xOffset() - bLeft;
		double y1 = pag->yOffset() - bTop;
		double w1 = pag->width()   + bLeft + bRight;
		double h1 = pag->height()  + bBottom + bTop;
		double lw = ite->visualLineWidth();
		double x2 = ite->BoundingX - lw / 2.0;
		double y2 = ite->BoundingY - lw / 2.0;
		double w2 = qMax(ite->BoundingW + lw, 1.0);
		double h2 = qMax(ite->BoundingH + lw, 1.0);
//		qDebug() << QString("pdflib process item: pagename=%1 ownpage=%2 pagenr=%3 changedMP=%4").arg(pag->pageName()).arg(ite->OwnPage).arg(pag->pageNr()).arg(ite->ChangedMasterItem);
//		qDebug() << QString("pdflib process item: x1=%1 x2=%2 y1=%3 y2=%4 w1=%5 w2=%6 h1=%7 h2=%8 lw=%9").arg(x1).arg(x2).arg(y1).arg(y2).arg(w1).arg(w2).arg(h1).arg(h2).arg(lw);
		if (!(QRectF(x2, y2, w2, h2).intersects(QRectF(x1, y1, w1, h1))) && !embedded)
		{
			output = tmp;
			return true;
		}
//		qDebug() << "bb test done";
		if (ite->ChangedMasterItem)
		{
			output = tmp;
			return true;
		}
		if ((!pag->pageName().isEmpty()) && (ite->OwnPage != static_cast<int>(pag->pageNr())) && (ite->OwnPage != -1))
		{
			output = tmp;
			return true;
		}
	}

	tmp += "q\n";
	if ((ite->doOverprint) && (!Options.UseRGB))
	{
		QByteArray ShName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		Transpar[ShName] = writeGState("/OP true\n"
									   "/op true\n"
									   "/OPM 1\n");
		tmp += Pdf::toName(ShName) + " gs\n";
	}
//	if (((ite->fillTransparency() != 0) || (ite->lineTransparency() != 0)) && (Options.Version >= PDFOptions::PDFVersion_14))
//		tmp += PDF_Transparenz(ite);
	if ((ite->isBookmark) && (Options.Bookmarks))
		PDF_Bookmark(ite, pag->height() - (ite->yPos() - pag->yOffset()));
	if (!pattern)
	{
		if (!ite->printEnabled() || ((ite->itemType() == PageItem::TextFrame) && (!pag->pageName().isEmpty())))
		{
//			qDebug() << "Q exit";
			tmp += "Q\n";
			output = tmp;
			return true;
		}
	}
	if (ite->fillColor() != CommonStrings::None)
		tmp += putColor(ite->fillColor(), ite->fillShade(), true);
	if (ite->lineColor() != CommonStrings::None)
		tmp += putColor(ite->lineColor(), ite->lineShade(), false);
	tmp += FToStr(fabs(ite->lineWidth()))+" w\n";
	if (ite->DashValues.count() != 0)
	{
		tmp += "[ ";
		QVector<double>::iterator it;
		for ( it = ite->DashValues.begin(); it != ite->DashValues.end(); ++it )
		{
			double da = *it;
			// #8758: Custom dotted lines don't export properly to pdf
			// Null values have to be exported if line end != flat
			if ((da != 0) || (ite->lineEnd() != Qt::FlatCap))
				tmp += Pdf::toPdf(da)+" ";
		}
		tmp += "] "+Pdf::toPdf(ite->DashOffset)+" d\n";
	}
	else
		tmp += "[" + Pdf::toAscii(getDashString(ite->PLineArt, ite->lineWidth())) + "] 0 d\n";
	switch (ite->PLineEnd)
	{
		case Qt::FlatCap:
			tmp += "0 J\n";
			break;
		case Qt::SquareCap:
			tmp += "2 J\n";
			break;
		case Qt::RoundCap:
			tmp += "1 J\n";
			break;
		default:
			tmp += "0 J\n";
			break;
	}
	switch (ite->PLineJoin)
	{
		case Qt::MiterJoin:
			tmp += "0 j\n";
			break;
		case Qt::BevelJoin:
			tmp += "2 j\n";
			break;
		case Qt::RoundJoin:
			tmp += "1 j\n";
			break;
		default:
			tmp += "0 j\n";
			break;
	}
	if (!embedded)
	{
		tmp += "1 0 0 1 "+FToStr(ite->xPos() - pag->xOffset())+" "+FToStr(pag->height() - (ite->yPos()  - pag->yOffset()))+" cm\n";
	}
	if (ite->rotation() != 0)
	{
		double sr = sin(-ite->rotation()* M_PI / 180.0);
		double cr = cos(-ite->rotation()* M_PI / 180.0);
		if ((cr * cr) < 0.000001)
			cr = 0;
		if ((sr * sr) < 0.000001)
			sr = 0;
		tmp += FToStr(cr)+" "+FToStr(sr)+" "+FToStr(-sr)+" "+FToStr(cr)+" 0 0 cm\n";
	}
	tmp += PDF_PutSoftShadow(ite,pag);
	switch (ite->itemType())
	{
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
#ifdef HAVE_OSG
			if (ite->asOSGFrame())
			{
				if (Options.Version != PDFOptions::PDFVersion_X3)
				{
					if (!PDF_3DAnnotation(ite, PNr))
						return false;
				}
				break;
			}
#endif
			tmp += "q\n";
			// Same functions as for ImageFrames work for LatexFrames too
			if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4) ))
			{
				tmp += PDF_TransparenzFill(ite);
			}
			if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
			{
				if (ite->GrType != 0)
				{
					if (ite->GrType == 8)
					{
						if (!PDF_PatternFillStroke(tmpOut, ite))
							return false;
					}
					else if (ite->GrType == 14)
					{
						if (!PDF_HatchFill(tmpOut, ite))
							return false;
					}
					else
					{
						if (!PDF_GradientFillStroke(tmpOut, ite))
							return false;
					}
					if (!tmpOut.isEmpty())
					{
						tmp += "q\n";
						if (ite->GrType == 14)
							tmp += tmpOut;
						else
						{
							tmp += tmpOut;
							tmp += SetClipPath(ite);
							tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
						}
						tmp += "Q\n";
					}
				}
				else
				{
					if (ite->fillColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
					}
				}
			}
			tmp += "q\n";
			tmp += SetPathAndClip(ite, true);
			if (ite->imageFlippedH())
				tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
			if (ite->imageFlippedV())
				tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
			if (ite->imageClip.size() != 0)
				tmp += SetImagePathAndClip(ite);
			if ((ite->imageIsAvailable) && (!ite->Pfile.isEmpty()))
			{
				if (!PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), false, ite->IProfile, ite->UseEmbedded, ite->IRender, &tmpOut))
					return false;
				tmp += tmpOut;
			}
			tmp += "Q\n";
			tmp += "Q\n";
			if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0)))
			{
				if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4) ))
					tmp += PDF_TransparenzStroke(ite);
				if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
				{
					ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
					if (strokePattern)
					{
						if (ite->patternStrokePath)
						{
							QPainterPath path = ite->PoLine.toQPainterPath(false);
							tmp += handleBrushPattern(ite, path, pag, PNr);
						}
						else
						{
							tmp += SetClipPath(ite);
							if (!PDF_PatternFillStroke(tmpOut, ite, 1))
								return false;
							tmp += tmpOut;
							tmp += "h\nS\n";
						}
					}
					else if (ite->GrTypeStroke > 0)
					{
						if (!PDF_GradientFillStroke(tmpOut, ite, true))
							return false;
						tmp += "q\n";
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
						tmp += "Q\n";
					}
					else if (ite->lineColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							tmp += setStrokeMulti(&ml[it]);
							tmp += SetClipPath(ite);
							tmp += "h\nS\n";
						}
					}
				}
			}
			break;
		case PageItem::TextFrame:
			if ((ite->isAnnotation()) && (!PDF_IsPDFX()))
			{
				if (ite->annotation().Type() == Annotation::RadioButton)
				{
					pageData.radioButtonList.append(ite);
					break;
				}
				if (!PDF_Annotation(ite, PNr))
					return false;
				break;
			}
			tmp += "q\n";
			if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4) ))
			{
				tmp += PDF_TransparenzFill(ite);
			}
			if ((ite->fillColor() != CommonStrings::None) || (ite->GrType != 0))
			{
				if (ite->GrType != 0)
				{
					if (ite->GrType == 8)
					{
						if (!PDF_PatternFillStroke(tmpOut, ite))
							return false;
					}
					else if (ite->GrType == 14)
					{
						if (!PDF_HatchFill(tmpOut, ite))
							return false;
					}
					else
					{
						if (!PDF_GradientFillStroke(tmpOut, ite))
							return false;
					}
					if (!tmpOut.isEmpty())
					{
						tmp += "q\n";
						if (ite->GrType == 14)
							tmp += tmpOut;
						else
						{
							tmp += tmpOut;
							tmp += SetClipPath(ite);
							tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
						}
						tmp += "Q\n";
					}
				}
				else
				{
					if (ite->fillColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
					}
				}
			}
			tmp += "q\n";
			if (ite->imageFlippedH())
				tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
			if (ite->imageFlippedV())
				tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
			if (ite->itemText.length() != 0)
				tmp += setTextSt(ite, PNr, pag);
			tmp += "Q\n";
			tmp += "Q\n";
			if (((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0)))
			{
				if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4) ))
					tmp += PDF_TransparenzStroke(ite);
				if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
				{
					ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
					if (strokePattern)
					{
						if (ite->patternStrokePath)
						{
							QPainterPath path = ite->PoLine.toQPainterPath(false);
							tmp += handleBrushPattern(ite, path, pag, PNr);
						}
						else
						{
							tmp += SetClipPath(ite);
							if (!PDF_PatternFillStroke(tmpOut, ite, 1))
								return false;
							tmp += tmpOut;
							tmp += "h\nS\n";
						}
					}
					else if (ite->GrTypeStroke > 0)
					{
						if (!PDF_GradientFillStroke(tmpOut, ite, true))
							return false;
						tmp += "q\n";
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
						tmp += "Q\n";
					}
					else if (ite->lineColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							tmp += setStrokeMulti(&ml[it]);
							tmp += SetClipPath(ite);
							tmp += "h\nS\n";
						}
					}
				}
			}
			break;
		case PageItem::Line:
			if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
				tmp += PDF_TransparenzStroke(ite);
			if (ite->NamedLStyle.isEmpty())
			{
				ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
				if (strokePattern)
				{
					if (ite->patternStrokePath)
					{
						QPainterPath path;
						path.moveTo(0, 0);
						path.lineTo(ite->width(), 0);
						tmp += handleBrushPattern(ite, path, pag, PNr);
					}
					else
					{
						if (!PDF_PatternFillStroke(tmpOut, ite, 1))
							return false;
						tmp += tmpOut;
						tmp += "0 0 m\n";
						tmp += FToStr(ite->width())+" 0 l\n";
						tmp += "S\n";
					}
				}
				else if (ite->GrTypeStroke > 0)
				{
					if (!PDF_GradientFillStroke(tmpOut, ite, true))
						return false;
					tmp += "q\n";
					tmp += tmpOut;
					tmp += "0 0 m\n";
					tmp += FToStr(ite->width())+" 0 l\n";
					tmp += "S\n";
					tmp += "Q\n";
				}
				else if (ite->lineColor() != CommonStrings::None)
				{
					tmp += "0 0 m\n";
					tmp += FToStr(ite->width())+" 0 l\n";
					tmp += "S\n";
				}
			}
			else
			{
				multiLine ml = doc.MLineStyles[ite->NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
					{
						tmp += setStrokeMulti(&ml[it]);
						tmp += "0 0 m\n";
						tmp += FToStr(ite->width())+" 0 l\n";
						tmp += "S\n";
					}
				}
			}
			if (ite->startArrowIndex() != 0)
			{
				QTransform arrowTrans;
				arrowTrans.scale(-1,1);
				arrowTrans.scale(ite->startArrowScale() / 100.0, ite->startArrowScale() / 100.0);
				tmp += drawArrow(ite, arrowTrans, ite->startArrowIndex());
			}
			if (ite->endArrowIndex() != 0)
			{
				QTransform arrowTrans;
				arrowTrans.translate(ite->width(), 0);
				arrowTrans.scale(ite->endArrowScale() / 100.0, ite->endArrowScale() / 100.0);
				tmp += drawArrow(ite, arrowTrans, ite->endArrowIndex());
			}
			break;
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
		case PageItem::Arc:
			tmp += "q\n";
			if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
			{
				tmp += PDF_TransparenzFill(ite);
			}
			if (ite->GrType != 0)
			{
				if (ite->GrType == 8)
				{
					if (!PDF_PatternFillStroke(tmpOut, ite))
						return false;
				}
				else if (ite->GrType == 14)
				{
					if (!PDF_HatchFill(tmpOut, ite))
						return false;
				}
				else
				{
					if (!PDF_GradientFillStroke(tmpOut, ite))
						return false;
				}
				if (!tmpOut.isEmpty())
				{
					if (ite->GrType == 14)
						tmp += tmpOut;
					else
					{
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
					}
				}
			}
			else
			{
				if (ite->fillColor() != CommonStrings::None)
				{
					tmp += SetClipPath(ite);
					tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
				}
			}
			tmp += "Q\n";
			if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
			{
				if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
					tmp += PDF_TransparenzStroke(ite);
				if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
				{
					ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
					if (strokePattern)
					{
						if (ite->patternStrokePath)
						{
							QPainterPath path = ite->PoLine.toQPainterPath(false);
							tmp += handleBrushPattern(ite, path, pag, PNr);
						}
						else
						{
							tmp += SetClipPath(ite);
							if (!PDF_PatternFillStroke(tmpOut, ite, 1))
								return false;
							tmp += tmpOut;
							tmp += "h\nS\n";
						}
					}
					else if (ite->GrTypeStroke > 0)
					{
						if (!PDF_GradientFillStroke(tmpOut, ite, true))
							return false;
						tmp += "q\n";
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
						tmp += "Q\n";
					}
					else if (ite->lineColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
					}
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							tmp += setStrokeMulti(&ml[it]);
							tmp += SetClipPath(ite);
							tmp += "h\nS\n";
						}
					}
				}
			}
			break;
		case PageItem::PolyLine:
		case PageItem::Spiral:
			if (ite->PoLine.size() > 3)  // && ((ite->PoLine.point(0) != ite->PoLine.point(1)) || (ite->PoLine.point(2) != ite->PoLine.point(3))))
			{
				tmp += "q\n";
				if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
				{
					tmp += PDF_TransparenzFill(ite);
				}
				if (ite->GrType != 0)
				{
					if (ite->GrType == 8)
					{
						if (!PDF_PatternFillStroke(tmpOut, ite))
							return false;
					}
					else if (ite->GrType == 14)
					{
						if (!PDF_HatchFill(tmpOut, ite))
							return false;
					}
					else
					{
						if (!PDF_GradientFillStroke(tmpOut, ite))
							return false;
					}
					if (!tmpOut.isEmpty())
					{
						if (ite->GrType == 14)
							tmp += tmpOut;
						else
						{
							tmp += tmpOut;
							tmp += SetClipPath(ite);
							tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
						}
					}
				}
				else
				{
					if (ite->fillColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite);
						tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
					}
				}
				tmp += "Q\n";
			}
			if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
			{
				if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
					tmp += PDF_TransparenzStroke(ite);
				if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
				{
					ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
					if (strokePattern)
					{
						if (ite->patternStrokePath)
						{
							QPainterPath path = ite->PoLine.toQPainterPath(false);
							tmp += handleBrushPattern(ite, path, pag, PNr);
						}
						else
						{
							tmp += SetClipPath(ite, false);
							if (!PDF_PatternFillStroke(tmpOut, ite, 1))
								return false;
							tmp += tmpOut;
							tmp += "S\n";
						}
					}
					else if (ite->GrTypeStroke > 0)
					{
						if (!PDF_GradientFillStroke(tmpOut, ite, true))
							return false;
						tmp += "q\n";
						tmp += tmpOut;
						tmp += SetClipPath(ite);
						tmp += "h\nS\n";
						tmp += "Q\n";
					}
					else if (ite->lineColor() != CommonStrings::None)
					{
						tmp += SetClipPath(ite, false);
						tmp += "S\n";
					}
				}
				else
				{
					multiLine ml = doc.MLineStyles[ite->NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
						{
							tmp += setStrokeMulti(&ml[it]);
							tmp += SetClipPath(ite, false);
							tmp += "S\n";
						}
					}
				}
			}
			if (ite->startArrowIndex() != 0)
			{
				FPoint Start = ite->PoLine.point(0);
				for (int xx = 1; xx < ite->PoLine.size(); xx += 2)
				{
					FPoint Vector = ite->PoLine.point(xx);
					if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
					{
						double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
						QTransform arrowTrans;
						arrowTrans.translate(Start.x(), Start.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(ite->startArrowScale() / 100.0, ite->startArrowScale() / 100.0);
						tmp += drawArrow(ite, arrowTrans, ite->startArrowIndex());
						break;
					}
				}
			}
			if (ite->endArrowIndex() != 0)
			{
				FPoint End = ite->PoLine.point(ite->PoLine.size()-2);
				for (uint xx = ite->PoLine.size()-1; xx > 0; xx -= 2)
				{
					FPoint Vector = ite->PoLine.point(xx);
					if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
					{
						double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
						QTransform arrowTrans;
						arrowTrans.translate(End.x(), End.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(ite->endArrowScale() / 100.0, ite->endArrowScale() / 100.0);
						tmp += drawArrow(ite, arrowTrans, ite->endArrowIndex());
						break;
					}
				}
			}
			break;
		case PageItem::PathText:
			if (ite->PoShow)
			{
				if (ite->PoLine.size() > 3)
				{
					tmp += "q\n";
					if ((ite->lineColor() != CommonStrings::None) || (!ite->NamedLStyle.isEmpty()) || (!ite->strokePattern().isEmpty()) || (ite->GrTypeStroke > 0))
					{
						if (((ite->lineTransparency() != 0) || (ite->lineBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
							tmp += PDF_TransparenzStroke(ite);
						if (ite->NamedLStyle.isEmpty()) //&& (ite->lineWidth() != 0.0))
						{
							ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
							if (strokePattern)
							{
								if (ite->patternStrokePath)
								{
									QPainterPath path = ite->PoLine.toQPainterPath(false);
									tmp += handleBrushPattern(ite, path, pag, PNr);
								}
								else
								{
									tmp += SetClipPath(ite, false);
									if (!PDF_PatternFillStroke(tmpOut, ite, 1))
										return false;
									tmp += tmpOut;
									tmp += "S\n";
								}
							}
							else if (ite->GrTypeStroke > 0)
							{
								if (!PDF_GradientFillStroke(tmpOut, ite, true))
									return false;
								tmp += "q\n";
								tmp += tmpOut;
								tmp += SetClipPath(ite, false);
								tmp += "S\n";
								tmp += "Q\n";
							}
							else if (ite->lineColor() != CommonStrings::None)
							{
								tmp += SetClipPath(ite, false);
								tmp += "S\n";
							}
						}
						else
						{
							multiLine ml = doc.MLineStyles[ite->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
								if (ml[it].Color != CommonStrings::None) //&& (ml[it].Width != 0))
								{
									tmp += setStrokeMulti(&ml[it]);
									tmp += SetClipPath(ite, false);
									tmp += "S\n";
								}
							}
						}
					}
					tmp += "Q\n";
				}
			}
			if (((ite->GrMask > 0) || (ite->fillTransparency() != 0) || (ite->fillBlendmode() != 0)) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
			{
				if (ite->GrMask > 0)
					tmp += "q\n";
				tmp += PDF_TransparenzFill(ite);
			}
			if (ite->itemText.length() != 0)
				tmp += setTextSt(ite, PNr, pag);
			if (ite->GrMask > 0)
				tmp += "Q\n";
			break;
		case PageItem::Symbol:
			if (doc.docPatterns.contains(ite->pattern()))
			{
				QByteArray tmpD;
				ScPattern pat = doc.docPatterns[ite->pattern()];
				tmp += "q\n";
				tmp += SetPathAndClip(ite);
				if (ite->imageFlippedH())
					tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
				if (ite->imageFlippedV())
					tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
				QTransform trans;
				trans.scale(ite->width() / pat.width, ite->height() / pat.height);
				trans.translate(0.0, -ite->height());
	//			trans.translate(pat.items.at(0)->gXpos, -pat.items.at(0)->gYpos);
				tmp += FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n";
				groupStackPos.push(QPointF(0, ite->height()));
				for (int em = 0; em < pat.items.count(); ++em)
				{
					PageItem* embedded = pat.items.at(em);
					tmpD += "q\n";
					tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
					QByteArray output;
					if (!PDF_ProcessItem(output, embedded, pag, PNr, true))
						return false;
					tmpD += output;
					tmpD += "Q\n";
				}
				groupStackPos.pop();
				if (Options.Version >= PDFOptions::PDFVersion_14 || Options.Version == PDFOptions::PDFVersion_X4)
					tmp += Write_TransparencyGroup(ite->fillTransparency(), ite->fillBlendmode(), tmpD, ite);
				else
					tmp += tmpD;
				tmp += "Q\n";
			}
			break;
		case PageItem::Group:
			if (ite->groupItemList.count() > 0)
			{
				QByteArray tmpD;
				tmp += "q\n";
				if (ite->groupClipping())
					tmp += SetPathAndClip(ite);
				if (ite->imageFlippedH())
					tmp += "-1 0 0 1 "+FToStr(ite->width())+" 0 cm\n";
				if (ite->imageFlippedV())
					tmp += "1 0 0 -1 0 "+FToStr(-ite->height())+" cm\n";
				QTransform trans;
				trans.scale(ite->width() / ite->groupWidth, ite->height() / ite->groupHeight);
				trans.translate(0.0, -ite->height());
				tmp += FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n";
				groupStackPos.push(QPointF(ite->xPos(), ite->height()));
				for (int em = 0; em < ite->groupItemList.count(); ++em)
				{
					PageItem* embedded = ite->groupItemList.at(em);
					tmpD += "q\n";
					tmpD +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(ite->height() - embedded->gYpos)+" cm\n";
					QByteArray output;
					if (inPattern > 0)
						patternStackPos.push(QPointF(embedded->gXpos, ite->height() - embedded->gYpos));
					if (!PDF_ProcessItem(output, embedded, pag, PNr, true, pattern))
						return false;
					if (inPattern > 0)
						patternStackPos.pop();
					tmpD += output;
					tmpD += "Q\n";
				}
				groupStackPos.pop();
				if (Options.Version >= PDFOptions::PDFVersion_14 || Options.Version == PDFOptions::PDFVersion_X4)
					tmp += Write_TransparencyGroup(ite->fillTransparency(), ite->fillBlendmode(), tmpD, ite);
				else
					tmp += Write_FormXObject(tmpD, ite);
				tmp += "Q\n";
			}
			break;
		case PageItem::Table:
			tmp += "q\n";
			tmp +=  "1 0 0 1 "+FToStr(ite->asTable()->gridOffset().x())+" "+FToStr(-ite->asTable()->gridOffset().y())+" cm\n";
			// Paint table fill.
			if (ite->asTable()->fillColor() != CommonStrings::None)
			{
				int lastCol = ite->asTable()->columns() - 1;
				int lastRow = ite->asTable()->rows() - 1;
				double x = ite->asTable()->columnPosition(0);
				double y = ite->asTable()->rowPosition(0);
				double width = ite->asTable()->columnPosition(lastCol) + ite->asTable()->columnWidth(lastCol) - x;
				double height = ite->asTable()->rowPosition(lastRow) + ite->asTable()->rowHeight(lastRow) - y;
				tmp += putColor(ite->asTable()->fillColor(), ite->asTable()->fillShade(), true);
				tmp += "0 0 "+FToStr(width)+" "+FToStr(-height)+" re\n";
				tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
			}
			// Pass 1: Paint cell fills.
			for (int row = 0; row < ite->asTable()->rows(); ++row)
			{
				int colSpan = 0;
				for (int col = 0; col < ite->asTable()->columns(); col += colSpan)
				{
					TableCell cell = ite->asTable()->cellAt(row, col);
					if (row == cell.row())
					{
						QString colorName = cell.fillColor();
						if (colorName != CommonStrings::None)
						{
							tmp += "q\n";
							tmp += putColor(colorName, cell.fillShade(), true);
							int row = cell.row();
							int col = cell.column();
							int lastRow = row + cell.rowSpan() - 1;
							int lastCol = col + cell.columnSpan() - 1;
							double x = ite->asTable()->columnPosition(col);
							double y = ite->asTable()->rowPosition(row);
							double width = ite->asTable()->columnPosition(lastCol) + ite->asTable()->columnWidth(lastCol) - x;
							double height = ite->asTable()->rowPosition(lastRow) + ite->asTable()->rowHeight(lastRow) - y;
							tmp += FToStr(x)+" "+FToStr(-y)+" "+FToStr(width)+" "+FToStr(-height)+" re\n";
							tmp += (ite->fillRule ? "h\nf*\n" : "h\nf\n");
							tmp += "Q\n";
						}
					}
					colSpan = cell.columnSpan();
				}
			}
			// Pass 2: Paint vertical borders.
			for (int row = 0; row < ite->asTable()->rows(); ++row)
			{
				int colSpan = 0;
				for (int col = 0; col < ite->asTable()->columns(); col += colSpan)
				{
					TableCell cell = ite->asTable()->cellAt(row, col);
					if (row == cell.row())
					{
						const int lastRow = cell.row() + cell.rowSpan() - 1;
						const int lastCol = cell.column() + cell.columnSpan() - 1;
						const double borderX = ite->asTable()->columnPosition(lastCol) + ite->asTable()->columnWidth(lastCol);
						QPointF start(borderX, 0.0);
						QPointF end(borderX, 0.0);
						QPointF startOffsetFactors, endOffsetFactors;
						int startRow, endRow;
						for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
						{
							TableCell rightCell = ite->asTable()->cellAt(row, lastCol + 1);
							startRow = qMax(cell.row(), rightCell.row());
							endRow = qMin(lastRow, rightCell.isValid() ? rightCell.row() + rightCell.rowSpan() - 1 : lastRow);
							TableCell topLeftCell = ite->asTable()->cellAt(startRow - 1, lastCol);
							TableCell topRightCell = ite->asTable()->cellAt(startRow - 1, lastCol + 1);
							TableCell bottomRightCell = ite->asTable()->cellAt(endRow + 1, lastCol + 1);
							TableCell bottomLeftCell = ite->asTable()->cellAt(endRow + 1, lastCol);
							TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
							resolveBordersVertical(topLeftCell, topRightCell, cell, rightCell, bottomLeftCell, bottomRightCell,
								&topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, ite->asTable());
							if (border.isNull())
								continue; // Quit early if the border to paint is null.
							start.setY(ite->asTable()->rowPosition(startRow));
							end.setY((ite->asTable()->rowPosition(endRow) + ite->asTable()->rowHeight(endRow)));
							joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
							tmp += paintBorder(border, start, end, startOffsetFactors, endOffsetFactors);
						}
						if (col == 0)
						{
							const int lastRow = cell.row() + cell.rowSpan() - 1;
							const int firstCol = cell.column();
							const double borderX = ite->asTable()->columnPosition(firstCol);
							QPointF start(borderX, 0.0);
							QPointF end(borderX, 0.0);
							QPointF startOffsetFactors, endOffsetFactors;
							int startRow, endRow;
							for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
							{
								TableCell leftCell = ite->asTable()->cellAt(row, firstCol - 1);
								startRow = qMax(cell.row(), leftCell.row());
								endRow = qMin(lastRow, leftCell.isValid() ? leftCell.row() + leftCell.rowSpan() - 1 : lastRow);
								TableCell topLeftCell = ite->asTable()->cellAt(startRow - 1, firstCol - 1);
								TableCell topRightCell = ite->asTable()->cellAt(startRow - 1, firstCol);
								TableCell bottomRightCell = ite->asTable()->cellAt(lastRow + 1, firstCol);
								TableCell bottomLeftCell = ite->asTable()->cellAt(lastRow + 1, firstCol - 1);
								TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
								resolveBordersVertical(topLeftCell, topRightCell, leftCell, cell, bottomLeftCell, bottomRightCell,
									&topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, ite->asTable());
								if (border.isNull())
									continue; // Quit early if the border to paint is null.
								start.setY(ite->asTable()->rowPosition(startRow));
								end.setY((ite->asTable()->rowPosition(endRow) + ite->asTable()->rowHeight(endRow)));
								joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
								tmp += paintBorder(border, start, end, startOffsetFactors, endOffsetFactors);
							}
						}
					}
					colSpan = cell.columnSpan();
				}
			}
			// Pass 3: Paint horizontal borders.
			for (int row = 0; row < ite->asTable()->rows(); ++row)
			{
				int colSpan = 0;
				for (int col = 0; col < ite->asTable()->columns(); col += colSpan)
				{
					TableCell cell = ite->asTable()->cellAt(row, col);
					if (row == cell.row())
					{
						const int lastRow = cell.row() + cell.rowSpan() - 1;
						const int lastCol = cell.column() + cell.columnSpan() - 1;
						const double borderY = (ite->asTable()->rowPosition(lastRow) + ite->asTable()->rowHeight(lastRow));
						QPointF start(0.0, borderY);
						QPointF end(0.0, borderY);
						QPointF startOffsetFactors, endOffsetFactors;
						int startCol, endCol;
						for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
						{
							TableCell bottomCell = ite->asTable()->cellAt(lastRow + 1, col);
							startCol = qMax(cell.column(), bottomCell.column());
							endCol = qMin(lastCol, bottomCell.isValid() ? bottomCell.column() + bottomCell.columnSpan() - 1 : lastCol);
							TableCell topLeftCell = ite->asTable()->cellAt(lastRow, startCol - 1);
							TableCell topRightCell = ite->asTable()->cellAt(lastRow, endCol + 1);
							TableCell bottomRightCell = ite->asTable()->cellAt(lastRow + 1, endCol + 1);
							TableCell bottomLeftCell = ite->asTable()->cellAt(lastRow + 1, startCol - 1);
							TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
							resolveBordersHorizontal(topLeftCell, cell, topRightCell, bottomLeftCell, bottomCell,
											  bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, ite->asTable());
							if (border.isNull())
								continue; // Quit early if the border is null.
							start.setX(ite->asTable()->columnPosition(startCol));
							end.setX(ite->asTable()->columnPosition(endCol) + ite->asTable()->columnWidth(endCol));
							joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
							tmp += paintBorder(border, start, end, startOffsetFactors, endOffsetFactors);
						}
						if (row == 0)
						{
							const int firstRow = cell.row();
							const int lastCol = cell.column() + cell.columnSpan() - 1;
							const double borderY = ite->asTable()->rowPosition(firstRow);
							QPointF start(0.0, borderY);
							QPointF end(0.0, borderY);
							QPointF startOffsetFactors, endOffsetFactors;
							int startCol, endCol;
							for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
							{
								TableCell topCell = ite->asTable()->cellAt(firstRow - 1, col);
								startCol = qMax(cell.column(), topCell.column());
								endCol = qMin(lastCol, topCell.isValid() ? topCell.column() + topCell.columnSpan() - 1 : lastCol);
								TableCell topLeftCell = ite->asTable()->cellAt(firstRow - 1, startCol - 1);
								TableCell topRightCell = ite->asTable()->cellAt(firstRow - 1, endCol + 1);
								TableCell bottomRightCell = ite->asTable()->cellAt(firstRow, endCol + 1);
								TableCell bottomLeftCell = ite->asTable()->cellAt(firstRow, startCol - 1);
								TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
								resolveBordersHorizontal(topLeftCell, topCell, topRightCell, bottomLeftCell, cell,
														 bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, ite->asTable());
								if (border.isNull())
									continue; // Quit early if the border is null.
								start.setX(ite->asTable()->columnPosition(startCol));
								end.setX(ite->asTable()->columnPosition(endCol) + ite->asTable()->columnWidth(endCol));
								joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
								tmp += paintBorder(border, start, end, startOffsetFactors, endOffsetFactors);
							}
						}
					}
					colSpan = cell.columnSpan();
				}
			}
			// Pass 4: Paint cell content.
			for (int row = 0; row < ite->asTable()->rows(); ++row)
			{
				for (int col = 0; col < ite->asTable()->columns(); col ++)
				{
					TableCell cell = ite->asTable()->cellAt(row, col);
					if (cell.row() == row && cell.column() == col)
					{
						PageItem* textFrame = cell.textFrame();
						tmp += "q\n";
						tmp +=  "1 0 0 1 "+FToStr(cell.contentRect().x())+" "+FToStr(-cell.contentRect().y())+" cm\n";
						QByteArray output;
						PDF_ProcessItem(output, textFrame, pag, PNr, true);
						tmp += output;
						tmp += "Q\n";
					}
				}
			}
			tmp += "Q\n";
			break;
		case PageItem::Multiple:
			Q_ASSERT(false);
			break;
		default:
			break;
	}
	tmp += "Q\n";
	output = tmp;
	return true;
}

QByteArray PDFLibCore::paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end, const QPointF& startOffsetFactors, const QPointF& endOffsetFactors)
{
	QByteArray tmp;
	tmp = "";
	tmp += "q\n";
	QPointF lineStart, lineEnd;
	QVector<double> DashValues;
	foreach (const TableBorderLine& line, border.borderLines())
	{
		if (line.color() == CommonStrings::None)
			continue;
		lineStart.setX(start.x() + line.width() * startOffsetFactors.x());
		lineStart.setY(start.y() + line.width() * startOffsetFactors.y());
		lineEnd.setX(end.x() + line.width() * endOffsetFactors.x());
		lineEnd.setY(end.y() + line.width() * endOffsetFactors.y());
		tmp += FToStr(lineStart.x())+" "+FToStr(-lineStart.y())+" m\n";
		tmp += FToStr(lineEnd.x())+" "+FToStr(-lineEnd.y())+" l\n";
		tmp += putColor(line.color(), line.shade(), false);
		tmp += FToStr(fabs(line.width()))+" w\n";
		getDashArray(line.style(), qMax(line.width(), 1.0), DashValues);
		if (DashValues.count() != 0)
		{
			tmp += "[ ";
			QVector<double>::iterator it;
			for ( it = DashValues.begin(); it != DashValues.end(); ++it )
			{
				double da = *it;
				if (da != 0)
					tmp += Pdf::toPdf(da)+" ";
			}
			tmp += "] 0 d\n";
		}
		else
			tmp += "[" + Pdf::toAscii(getDashString(line.style(), line.width())) + "] 0 d\n";
		tmp += "0 J 0 j S\n";
	}
	tmp += "Q\n";
	return tmp;
}

QByteArray PDFLibCore::handleBrushPattern(PageItem* ite, QPainterPath &path, const ScPage* pag, uint PNr)
{
	QByteArray tmp;
	tmp = "";
	ScPattern pat = doc.docPatterns[ite->strokePattern()];
	double pLen = path.length() - ((pat.width / 2.0) * (ite->patternStrokeScaleX / 100.0));
	double adv = pat.width * ite->patternStrokeScaleX / 100.0 * ite->patternStrokeSpace;
	double xpos = ite->patternStrokeOffsetX * ite->patternStrokeScaleX / 100.0;
	while (xpos < pLen)
	{
		double currPerc = path.percentAtLength(xpos);
		double currAngle = path.angleAtPercent(currPerc);
		if (currAngle <= 180.0)
			currAngle *= -1.0;
		else
			currAngle = 360.0 - currAngle;
		QPointF currPoint = path.pointAtPercent(currPerc);
		tmp += "q\n";
		QTransform base;
		base.translate(currPoint.x(), -currPoint.y());
		base.rotate(-currAngle);
		tmp += FToStr(base.m11())+" "+FToStr(base.m12())+" "+FToStr(base.m21())+" "+FToStr(base.m22())+" "+FToStr(base.dx())+" "+FToStr(base.dy())+" cm\n";
		QTransform trans;
		trans.translate(0.0, -ite->patternStrokeOffsetY);
		trans.rotate(-ite->patternStrokeRotation);
		trans.shear(ite->patternStrokeSkewX, -ite->patternStrokeSkewY);
		trans.scale(ite->patternStrokeScaleX / 100.0, ite->patternStrokeScaleY / 100.0);
		trans.translate(-pat.width / 2.0, -pat.height / 2.0);
		if (ite->patternStrokeMirrorX)
		{
			trans.translate(pat.width, 0);
			trans.scale(-1, 1);
		}
		if (ite->patternStrokeMirrorY)
		{
			trans.translate(0, pat.height);
			trans.scale(1, -1);
		}
		tmp += FToStr(trans.m11())+" "+FToStr(trans.m12())+" "+FToStr(trans.m21())+" "+FToStr(trans.m22())+" "+FToStr(trans.dx())+" "+FToStr(trans.dy())+" cm\n";
		for (int em = 0; em < pat.items.count(); ++em)
		{
			PageItem* embedded = pat.items.at(em);
			tmp += "q\n";
			tmp +=  "1 0 0 1 "+FToStr(embedded->gXpos)+" "+FToStr(embedded->gHeight - embedded->gYpos)+" cm\n";
			QByteArray output;
			if (!PDF_ProcessItem(output, embedded, pag, PNr, true))
				return "";
			tmp += output;
			tmp += "Q\n";
		}
		tmp += "Q\n";
		xpos += adv;
	}
	return tmp;
}

QByteArray PDFLibCore::drawArrow(PageItem *ite, QTransform &arrowTrans, int arrowIndex)
{
	QByteArray tmp = "";
	FPointArray arrow = doc.arrowStyles().at(arrowIndex-1).points.copy();
	if (ite->NamedLStyle.isEmpty())
	{
		if (ite->lineWidth() != 0.0)
			arrowTrans.scale(ite->lineWidth(), ite->lineWidth());
	}
	else
	{
		multiLine ml = doc.MLineStyles[ite->NamedLStyle];
		if (ml[ml.size()-1].Width != 0.0)
			arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
	}
	arrow.map(arrowTrans);
	if ((ite->lineTransparency() != 0) && ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)))
	{
		QByteArray ShName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		Transpar[ShName] = writeGState("/CA "+FToStr(1.0 - ite->lineTransparency())+"\n"
									   + "/ca "+FToStr(1.0 - ite->lineTransparency())+"\n"
									   + "/SMask /None\n/AIS false\n/OPM 1\n"
									   + "/BM /Normal\n");
		tmp += Pdf::toName(ShName) + " gs\n";
	}
	if (ite->NamedLStyle.isEmpty())
	{
		ScPattern* strokePattern = doc.checkedPattern(ite->strokePattern());
		if (strokePattern)
		{
			tmp += SetClipPathArray(&arrow);
			QByteArray tmpOut;
			PDF_PatternFillStroke(tmpOut, ite, 1, true);
			tmp += tmpOut;
			tmp += "h\nf*\n";
		}
		else if (ite->GrTypeStroke > 0)
		{
			tmp += SetClipPathArray(&arrow);
			QByteArray tmpOut;
			PDF_GradientFillStroke(tmpOut, ite, true, true);
			tmp += "q\n";
			tmp += tmpOut;
			tmp += "h\nf*\nQ\n";
		}
		else if (ite->lineColor() != CommonStrings::None)
		{
			tmp += putColor(ite->lineColor(), ite->lineShade(), true);
			tmp += SetClipPathArray(&arrow);
			tmp += "h\nf*\n";
		}
	}
	else
	{
		multiLine ml = doc.MLineStyles[ite->NamedLStyle];
		if (ml[0].Color != CommonStrings::None)
		{
			tmp += putColor(ml[0].Color, ml[0].Shade, true);
			tmp += SetClipPathArray(&arrow);
			tmp += "h\nf*\n";
		}
		for (int it = ml.size()-1; it > 0; it--)
		{
			if (ml[it].Color != CommonStrings::None)
			{
				tmp += setStrokeMulti(&ml[it]);
				tmp += SetClipPathArray(&arrow);
				tmp += "h\nS\n";
			}
		}
	}
	return tmp;
}

QByteArray PDFLibCore::putColor(const QString& color, double shade, bool fill)
{
	QByteArray tmp = "";
	QByteArray colString = SetColor(color, shade);
	ScColor tmpC;
	tmpC = doc.PageColors[color];
	if (((tmpC.isSpotColor()) || (tmpC.isRegistrationColor())) && ((Options.isGrayscale == false) && (Options.UseRGB == false))  && (Options.UseSpotColors))
	{
		if ((color != CommonStrings::None) && (spotMap.contains(color)))
		{
			if (fill)
			{
				tmp += "/"+spotMap[color].ResName+" cs\n";
				tmp += FToStr(shade / 100.0)+" scn\n";
			}
			else
			{
				tmp += "/"+spotMap[color].ResName+" CS\n";
				tmp += FToStr(shade / 100.0)+" SCN\n";
			}
		}
		return tmp;
	}
	if (Options.isGrayscale)
	{
		if (color != CommonStrings::None)
		{
			if (fill)
				tmp += colString+" g\n";
			else
				tmp += colString+" G\n";
		}
		return tmp;
	}
	if (Options.UseRGB)
	{
		if (color != CommonStrings::None)
		{
			if (fill)
				tmp += colString+" rg\n";
			else
				tmp += colString+" RG\n";
		}
	}
	else
	{
		if ((doc.HasCMS) && (Options.UseProfiles))
		{
			if (tmpC.getColorModel() == colorModelCMYK)
			{
				if (color != CommonStrings::None)
				{
					if (fill)
						tmp += colString+" k\n";
					else
						tmp += colString+" K\n";
				}
			}
			else
			{
				QByteArray tmp2[] = {"/Perceptual", "/RelativeColorimetric", "/Saturation", "/AbsoluteColorimetric"};
				tmp += tmp2[Options.Intent]+ " ri\n";
				if (color != CommonStrings::None)
				{
					if (fill)
					{
						tmp += "/"+ICCProfiles[Options.SolidProf].ResName+" cs\n";
						tmp += colString+" scn\n";
					}
					else
					{
						tmp += "/"+ICCProfiles[Options.SolidProf].ResName+" CS\n";
						tmp += colString+" SCN\n";
					}
				}
			}
		}
		else
		{
			if (color != CommonStrings::None)
			{
				if (fill)
					tmp += colString+" k\n";
				else
					tmp += colString+" K\n";
			}
		}
	}
	return tmp;
}

/*CB 2982: cache code is borked somehow, original function is above
QByteArray PDFLibCore::putColor(const QString & colorName, int shade, bool fill)
{
	// Cache of last foreground and background colours We cache fg and bg
	// separately because they're alternated so much.  The primary purpose of
	// this cache is to avoid re-caculating the fg and bg colors on each char
	// of text when the color doens't change.
	static QString lastFGColorName, lastFGOutput, lastBGColorName, lastBGOutput;
	static int lastFGShade = -1, lastBGShade = -1;
	if (fill && colorName == lastBGColorName && shade == lastBGShade)
		return lastBGOutput;
	else if (colorName == lastFGColorName && shade == lastFGShade)
		return lastFGOutput;
	// Cache miss, generate the color
	else if (fill)
	{
		lastBGColorName = colorName;
		lastBGShade = shade;
		lastBGOutput = putColorUncached(colorName, shade, fill);
		return lastBGOutput;
	}
	else
	{
		lastFGColorName = colorName;
		lastFGShade = shade;
		lastFGOutput = putColorUncached(colorName, shade, fill);
		return lastFGOutput;
	}
}
*/

QByteArray PDFLibCore::putColorUncached(const QString& color, int shade, bool fill)
{
	ScColor tmpC(doc.PageColors[color]);
	if (((tmpC.isSpotColor()) || (tmpC.isRegistrationColor())) && ((Options.isGrayscale == false) && (Options.UseRGB == false))  && (Options.UseSpotColors))
	{
		QByteArray tmpSpot("");
		if ((color != CommonStrings::None) && (spotMap.contains(color)))
		{
			if (fill)
			{
				tmpSpot += "/"+spotMap[color].ResName+" cs\n";
				tmpSpot += FToStr(shade / 100.0)+" scn\n";
			}
			else
			{
				tmpSpot += "/"+spotMap[color].ResName+" CS\n";
				tmpSpot += FToStr(shade / 100.0)+" SCN\n";
			}
		}
		return tmpSpot;
	}
	QByteArray colString(SetColor(color, shade));
	if (Options.isGrayscale)
	{
		QByteArray tmpGray("");
		if (color != CommonStrings::None)
		{
			if (fill)
				tmpGray += colString+" g\n";
			else
				tmpGray += colString+" G\n";
		}
		return tmpGray;
	}
	QByteArray tmp("");
	if (Options.UseRGB)
	{
		if (color != CommonStrings::None)
		{
			if (fill)
				tmp += colString+" rg\n";
			else
				tmp += colString+" RG\n";
		}
	}
	else
	{
		if ((doc.HasCMS) && (Options.UseProfiles))
		{
			if (tmpC.getColorModel() == colorModelCMYK)
			{
				if (color != CommonStrings::None)
				{
					if (fill)
						tmp += colString+" k\n";
					else
						tmp += colString+" K\n";
				}
			}
			else
			{
				QByteArray tmp2[] = {"/Perceptual", "/RelativeColorimetric", "/Saturation", "/AbsoluteColorimetric"};
				tmp += tmp2[Options.Intent]+ " ri\n";
				if (color != CommonStrings::None)
				{
					if (fill)
					{
						tmp += "/"+ICCProfiles[Options.SolidProf].ResName+" cs\n";
						tmp += colString+" scn\n";
					}
					else
					{
						tmp += "/"+ICCProfiles[Options.SolidProf].ResName+" CS\n";
						tmp += colString+" SCN\n";
					}
				}
			}
		}
		else
		{
			if (color != CommonStrings::None)
			{
				if (fill)
					tmp += colString+" k\n";
				else
					tmp += colString+" K\n";
			}
		}
	}
	return tmp;
}

QByteArray PDFLibCore::setStrokeMulti(struct SingleLine *sl)
{
	QByteArray tmp(
			putColor(sl->Color, sl->Shade, false) +
			FToStr(sl->Width)+" w\n"
			);
	QByteArray Ds =  Pdf::toAscii(getDashString(sl->Dash, sl->Width));
	tmp += Ds.isEmpty() ? "[] 0 d\n" : "[" +  Ds + "] 0 d\n";
	switch (static_cast<Qt::PenCapStyle>(sl->LineEnd))
	{
		case Qt::FlatCap:
			tmp += "0 J\n";
			break;
		case Qt::SquareCap:
			tmp += "2 J\n";
			break;
		case Qt::RoundCap:
			tmp += "1 J\n";
			break;
		default:
			tmp += "0 J\n";
			break;
	}
	switch (static_cast<Qt::PenJoinStyle>(sl->LineJoin))
	{
		case Qt::MiterJoin:
			tmp += "0 j\n";
			break;
		case Qt::BevelJoin:
			tmp += "2 j\n";
			break;
		case Qt::RoundJoin:
			tmp += "1 j\n";
			break;
		default:
			tmp += "0 j\n";
			break;
	}
	return tmp;
}

// Return a PDF substring representing a PageItem's text
QByteArray PDFLibCore::setTextSt(PageItem *ite, uint PNr, const ScPage* pag)
{
	int savedOwnPage = ite->OwnPage;
	ite->OwnPage = PNr;
	ite->layout();
	ite->OwnPage = savedOwnPage;
	
	PdfPainter p(ite, this, PNr, pag);
	ite->textLayout.renderBackground(&p);
	ite->textLayout.render(&p);
	return p.getBuffer();
}

QByteArray PDFLibCore::SetColor(const QString& farbe, double Shade)
{
	const ScColor& col = doc.PageColors[farbe];
	return SetColor(col, Shade);
}

QByteArray PDFLibCore::SetColor(const ScColor& farbe, double Shade)
{
	QByteArray tmp;
	RGBColorF rgb;
	CMYKColorF cmyk;
	double h, s, v, k;
	ScColor tmpC(farbe);
	if (Options.isGrayscale)
	{
		bool kToGray = false;
		if (tmpC.getColorModel() == colorModelCMYK)
		{
			ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
			cmyk.getValues(h, s, v, k);
			kToGray = (h == 0 && s == 0 && v == 0);
		}
		if (kToGray)
			tmp = FToStr(1.0 - k);
		else
		{
			ScColorEngine::getShadeColorRGB(tmpC, &doc, rgb, Shade);
			rgb.getValues(h, s, v);
			tmp = FToStr(0.3 * h + 0.59 * s + 0.11 * v);
		}
		return tmp;
	}
	if (Options.UseRGB)
	{
		ScColorEngine::getShadeColorRGB(tmpC, &doc, rgb, Shade);
		rgb.getValues(h, s, v);
		tmp = FToStr(h) + " " + FToStr(s) + " " + FToStr(v);
	}
	else
	{
		if ((doc.HasCMS) && (Options.UseProfiles))
		{
			if (tmpC.getColorModel() == colorModelCMYK)
			{
				ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
				cmyk.getValues(h, s, v, k);
				tmp = FToStr(h) + " "+ FToStr(s) + " " + FToStr(v) + " " + FToStr(k);
			}
			else
			{
				if (Options.SComp == 3)
				{
					ScColorEngine::getShadeColorRGB(tmpC, &doc, rgb, Shade);
					rgb.getValues(h, s, v);
					tmp = FToStr(h) + " " + FToStr(s) + " " + FToStr(v);
				}
				else
				{
					ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
					cmyk.getValues(h, s, v, k);
					tmp = FToStr(h) + " " + FToStr(s) + " " + FToStr(v) + " " + FToStr(k);
				}
			}
		}
		else
		{
			ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
			cmyk.getValues(h, s, v, k);
			tmp = FToStr(h) + " " + FToStr(s) + " " + FToStr(v) + " " + FToStr(k);
		}
	}
	return tmp;
}

QByteArray PDFLibCore::SetGradientColor(const QString& farbe, double Shade)
{
	QByteArray tmp;
	RGBColor rgb;
	CMYKColor cmyk;
	int h, s, v, k;
	if (farbe == CommonStrings::None)
	{
		if (Options.isGrayscale)
			tmp = "0.0";
		if (Options.UseRGB)
			tmp = "0.0 0.0 0.0";
		else
		{
			if ((doc.HasCMS) && (Options.UseProfiles))
			{
				if (Options.SComp == 3)
					tmp = "0.0 0.0 0.0";
				else
					tmp = "0.0 0.0 0.0 0.0";
			}
			else
				tmp = "0.0 0.0 0.0 0.0";
		}
		return tmp;
	}
	ScColor tmpC(doc.PageColors[farbe]);
	QColor tmpR;
	if (Options.isGrayscale)
	{
		bool kToGray = false;
		if (tmpC.getColorModel() == colorModelCMYK)
		{
			ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
			cmyk.getValues(h, s, v, k);
			kToGray = (h == 0 && s == 0 && v == 0);
		}
		if (kToGray)
			tmp = FToStr(1.0 - k / 255.0);
		else
		{
			tmpR = ScColorEngine::getShadeColor(tmpC, &doc, Shade);
			tmpR.getRgb(&h, &s, &v);
			tmp = FToStr((0.3 * h + 0.59 * s + 0.11 * v) / 255.0);
		}
		return tmp;
	}
	if (Options.UseRGB)
	{
		tmpR = ScColorEngine::getShadeColor(tmpC, &doc, Shade);
		tmpR.getRgb(&h, &s, &v);
		tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
	}
	else
	{
		if ((doc.HasCMS) && (Options.UseProfiles))
		{
			if (Options.SComp == 3)
			{
				ScColorEngine::getShadeColorRGB(tmpC, &doc, rgb, Shade);
				rgb.getValues(h, s, v);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0);
			}
			else
			{
				ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
				cmyk.getValues(h, s, v, k);
				tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
			}
		}
		else
		{
			ScColorEngine::getShadeColorCMYK(tmpC, &doc, cmyk, Shade);
			cmyk.getValues(h, s, v, k);
			tmp = FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" "+FToStr(k / 255.0);
		}
	}
	return tmp;
}

QByteArray PDFLibCore::SetClipPath(PageItem *ite, bool poly)
{
	QByteArray tmp;
	FPoint np, np1, np2, np3, np4, firstP;
	bool nPath = true;
	bool first = true;
	if (ite->PoLine.size() <= 3)
		return tmp;

	for (int poi=0; poi<ite->PoLine.size()-3; poi += 4)
	{
		if (ite->PoLine.isMarker(poi))
		{
			nPath = true;
			continue;
		}
		if (nPath)
		{
			np = ite->PoLine.point(poi);
			if ((!first) && (poly) && (np4 == firstP))
				tmp += "h\n";
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
			nPath = false;
			first = false;
			firstP = np;
			np4 = np;
		}
		np = ite->PoLine.point(poi);
		np1 = ite->PoLine.point(poi+1);
		np2 = ite->PoLine.point(poi+3);
		np3 = ite->PoLine.point(poi+2);
		if ((np == np1) && (np2 == np3))
			tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" l\n";
		else
		{
			tmp += FToStr(np1.x())+" "+FToStr(-np1.y())+" ";
			tmp += FToStr(np2.x())+" "+FToStr(-np2.y())+" ";
			tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" c\n";
		}
		np4 = np3;
	}
	return tmp;
}

QByteArray PDFLibCore::SetClipPathArray(FPointArray *ite, bool poly)
{
	QByteArray tmp;
	FPoint np, np1, np2, np3, np4, firstP;
	bool nPath = true;
	bool first = true;
	if (ite->size() <= 3)
		return tmp;

	for (int poi=0; poi<ite->size()-3; poi += 4)
	{
		if (ite->isMarker(poi))
		{
			nPath = true;
			continue;
		}
		if (nPath)
		{
			np = ite->point(poi);
			if ((!first) && (poly) && (np4 == firstP))
				tmp += "h\n";
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
			nPath = false;
			first = false;
			firstP = np;
			np4 = np;
		}
		np = ite->point(poi);
		np1 = ite->point(poi+1);
		np2 = ite->point(poi+3);
		np3 = ite->point(poi+2);
		if ((np == np1) && (np2 == np3))
			tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" l\n";
		else
		{
			tmp += FToStr(np1.x())+" "+FToStr(-np1.y())+" ";
			tmp += FToStr(np2.x())+" "+FToStr(-np2.y())+" ";
			tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" c\n";
		}
		np4 = np3;
	}
	return tmp;
}

QByteArray PDFLibCore::SetClipPathImage(PageItem *ite)
{
	QByteArray tmp;
	if (ite->imageClip.size() <= 3)
		return tmp;

	bool nPath = true;
	for (int poi=0; poi<ite->imageClip.size()-3; poi += 4)
	{
		if (ite->imageClip.isMarker(poi))
		{
			tmp += "h\n";
			nPath = true;
			continue;
		}
		FPoint np, np1, np2, np3;
		if (nPath)
		{
			np = ite->imageClip.point(poi);
			tmp += FToStr(np.x())+" "+FToStr(-np.y())+" m\n";
			nPath = false;
		}
		np = ite->imageClip.point(poi);
		np1 = ite->imageClip.point(poi+1);
		np2 = ite->imageClip.point(poi+3);
		np3 = ite->imageClip.point(poi+2);
		if ((np == np1) && (np2 == np3))
			tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" l\n";
		else
		{
			tmp += FToStr(np1.x())+" "+FToStr(-np1.y())+" ";
			tmp += FToStr(np2.x())+" "+FToStr(-np2.y())+" ";
			tmp += FToStr(np3.x())+" "+FToStr(-np3.y())+" c\n";
		}
	}
	return tmp;
}

QByteArray PDFLibCore::SetImagePathAndClip(PageItem *item)
{
	QByteArray tmp = SetClipPathImage(item);
	if (tmp.length() > 0)
		tmp += "h W* n\n";
	return tmp;
}

QByteArray PDFLibCore::SetPathAndClip(PageItem *item)
{
	QByteArray tmp = SetClipPath(item);
	if (tmp.length() > 0)
		tmp += (item->fillRule ? "h W* n\n" : "h W n\n");
	return tmp;
}

QByteArray PDFLibCore::SetPathAndClip(PageItem *item, bool fillRule)
{
	QByteArray tmp = SetClipPath(item);
	if (tmp.length() > 0)
		tmp += (fillRule ? "h W* n\n" : "h W n\n");
	return tmp;
}

void PDFLibCore::getBleeds(const ScPage* page, double &left, double &right)
{
	MarginStruct values;
	doc.getBleeds(page, Options.bleeds, values);
	left   = values.left();
	right  = values.right();
}

void PDFLibCore::getBleeds(const ScPage* page, double &left, double &right, double &bottom, double& top)
{
	MarginStruct values;
	doc.getBleeds(page, Options.bleeds, values);
	left   = values.left();
	right  = values.right();
	bottom = values.bottom();
	top    = values.top();
}

QByteArray PDFLibCore::PDF_TransparenzFill(PageItem *currItem)
{
	QByteArray ShName = ResNam+Pdf::toPdf(ResCount);
	ResCount++;
	QByteArray tmp;
	QByteArray GXName;
	double scaleX = 1.0;
	double scaleY = 1.0;
	if (currItem->itemType() == PageItem::Symbol)
	{
		ScPattern pat = doc.docPatterns[currItem->pattern()];
		scaleX = 1.0 / (currItem->width() / pat.width);
		scaleY = 1.0 / (currItem->height() / pat.height);
	}
	if ((currItem->GrMask == 1) || (currItem->GrMask == 2) || (currItem->GrMask == 4) || (currItem->GrMask == 5))
	{
		QList<double> StopVec;
		QList<double> TransVec;
		VGradient gradient;
		double StartX, StartY, EndX, EndY, FocalX, FocalY, Gscale, Gskew;
		int GType;
		GType = currItem->GrMask;
		StartX = currItem->GrMaskStartX;
		StartY = currItem->GrMaskStartY;
		EndX = currItem->GrMaskEndX;
		EndY = currItem->GrMaskEndY;
		FocalX = currItem->GrMaskFocalX;
		FocalY = currItem->GrMaskFocalY;
		Gscale = currItem->GrMaskScale;
		Gskew = currItem->GrMaskSkew;
		if (!(currItem->gradientMask().isEmpty()) && (doc.docGradients.contains(currItem->gradientMask())))
			gradient = doc.docGradients[currItem->gradientMask()];
		else
			gradient = currItem->mask_gradient;
		QList<VColorStop*> cstops = gradient.colorStops();
		QTransform mpa;
		if (currItem->isGroup())
		{
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), currItem->height());
			mpa.rotate(-currItem->rotation());
		}
		else if (currItem->itemType() == PageItem::Symbol)
		{
			mpa.translate(0, currItem->height() * scaleY);
			mpa.scale(scaleX, scaleY);
		}
		else
		{
			mpa.translate(bleedDisplacementX, bleedDisplacementY);
			if (currItem->isImageFrame())
			{
				mpa.translate(currItem->imageXOffset() * currItem->imageXScale(), -currItem->imageYOffset() * currItem->imageYScale());
				mpa.rotate(-currItem->imageRotation());
			}
		}
		// #11761 : cause trouble when exporting transparency gradient
		/*else
			mpa.rotate(-currItem->rotation());*/
		if (Gskew == 90)
			Gskew = 1;
		else if (Gskew == 180)
			Gskew = 0;
		else if (Gskew == 270)
			Gskew = -1;
		else if (Gskew == 360)
			Gskew = 0;
		else
			Gskew = tan(M_PI / 180.0 * Gskew);
		if ((GType == 1) || (GType == 4))
		{
			mpa.translate(StartX, -StartY);
			mpa.shear(Gskew, 0);
			mpa.translate(-StartX, StartY);
		}
		else
		{
			double rotEnd = xy2Deg(EndX - StartX, EndY - StartY);
			mpa.translate(StartX, -StartY);
			mpa.rotate(-rotEnd);
			mpa.shear(-Gskew, 0);
			mpa.translate(0, -StartY * (1.0 - Gscale));
			mpa.translate(-StartX, StartY);
			mpa.scale(1, Gscale);
		}
		QColor qStopColor;
		for (uint cst = 0; cst < gradient.Stops(); ++cst)
		{
			double actualStop = cstops.at(cst)->rampPoint;
			qStopColor = cstops.at(cst)->color;
			double a = cstops.at(cst)->opacity;
			double r, g, b;
			qStopColor.getRgbF(&r, &g, &b);
			if ((GType == 4) || (GType == 5))
				a = /* 1.0 - */ (0.3 * r + 0.59 * g + 0.11 * b);
			if ((cst == 0) && (actualStop != 0.0))
			{
				StopVec.append(0.0);
				TransVec.append(a);
			}
			StopVec.append(actualStop);
			TransVec.append(a);
			if ((cst == gradient.Stops()-1) && (actualStop < 1.0))
			{
				StopVec.append(1.0);
				TransVec.append(a);
			}
		}
		PdfId patObject = writer.newObject();
		writer.startObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
		PutDoc("/Shading\n");
		PutDoc("<<\n");
		if ((GType == 1) || (GType == 4))
			PutDoc("/ShadingType 2\n");
		else
			PutDoc("/ShadingType 3\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		if (gradient.repeatMethod() == VGradient::none)
			PutDoc("/Extend [false false]\n");
		else
			PutDoc("/Extend [true true]\n");
		if ((GType == 1) || (GType == 4))
			PutDoc("/Coords ["+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(EndX)+" "+FToStr(-EndY)+"]\n");
		else
			PutDoc("/Coords ["+FToStr(FocalX)+" "+FToStr(-FocalY)+" 0.0 "+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2)))+"]\n");
		PutDoc("/Function\n");
		PutDoc("<<\n");
		PutDoc("/FunctionType 3\n");
		PutDoc("/Domain [0 1]\n");
		if (StopVec.count() > 2)
		{
			PutDoc("/Bounds [");
			QByteArray bctx = "";
			for (int bc = 1; bc < StopVec.count() - 1; bc++)
			{
				bctx += FToStr(StopVec.at(bc))+" ";
			}
			PutDoc(bctx.trimmed()+"]\n");
		}
		else
			PutDoc("/Bounds []\n");
		QByteArray entx = "";
		PutDoc("/Functions\n");
		PutDoc("[\n");
		for (int cc = 0; cc < TransVec.count() - 1; cc++)
		{
			entx += "0 1 ";
			PutDoc("<<\n");
			PutDoc("/FunctionType 2\n");
			PutDoc("/Domain [0 1]\n");
			PutDoc("/C0 ["+FToStr(TransVec.at(cc))+"]\n");
			PutDoc("/C1 ["+FToStr(TransVec.at(cc+1))+"]\n");
			PutDoc("/N 1\n");
			PutDoc(">>\n");
		}
		PutDoc("]\n");
		PutDoc("/Encode ["+entx.trimmed()+"]\n");
		PutDoc(">>\n");
		PutDoc(">>\n");
		PutDoc(">>\n");
		writer.endObj(patObject);
		Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
		
		PdfId formObject = writer.newObject();
		writer.startObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = currItem->lineWidth();
		if ((currItem->isGroup()) || (currItem->itemType() == PageItem::Symbol))
			PutDoc("/BBox [0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" ]\n");
		else
			PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(currItem->width()+lw)+" "+FToStr(-(currItem->height()+lw))+" ]\n");
		PutDoc("/Resources ");
		Pdf::ResourceDictionary dict;
		dict.Pattern = Patterns;
		dict.XObject.unite(pageData.ImgObjects);
		dict.XObject.unite(pageData.XObjects);
		writer.write(dict);
		PutDoc("\n");
		
		QByteArray stre = "q\n";
		if (currItem->isGroup())
		{
			QTransform mpa;
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), currItem->height());
			mpa.rotate(-currItem->rotation());
			stre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+" cm\n";
		}
		else if (currItem->itemType() == PageItem::Symbol)
		{
			QTransform mpa;
			mpa.translate(0, currItem->height() * scaleY);
			mpa.scale(scaleX, scaleY);
			stre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+" cm\n";
		}
		stre += SetClipPath(currItem)+"h\n";
		stre += FToStr(fabs(currItem->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+Pdf::toPdf(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressArray(stre);
		PutDoc("/Length "+Pdf::toPdf(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream");
		writer.endObj(formObject);
		pageData.XObjects[ResNam+Pdf::toPdf(ResCount)] = formObject;
		ResCount++;
		GXName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+Pdf::toPdf(formObject)+" 0 R >>\n/AIS false\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		tmp = Pdf::toName(GXName) + " gs\n";
	}
	else if ((currItem->GrMask == 3) || (currItem->GrMask == 6) || (currItem->GrMask == 7) || (currItem->GrMask == 8))
	{
		QByteArray tmpOut = "";
		PDF_PatternFillStroke(tmpOut, currItem, 2);
		PdfId formObject = writer.newObject();
		writer.startObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency ");
		if (Options.UseRGB)
			PutDoc("/CS /DeviceRGB");
		else
		{
			if (Options.isGrayscale)
				PutDoc("/CS /DeviceGray");
			else
			{
				if ((doc.HasCMS) && (Options.UseProfiles))
					PutDoc("/CS "+ICCProfiles[Options.SolidProf].ICCArray);
				else
					PutDoc("/CS /DeviceCMYK");
			}
		}
		PutDoc(" >>\n");
		if ((currItem->isGroup()) || (currItem->itemType() == PageItem::Symbol))
			PutDoc("/BBox [0 0 "+FToStr(ActPageP->width())+" "+FToStr(ActPageP->height())+" ]\n");
		else
			PutDoc("/BBox [0 0 "+FToStr(currItem->width())+" "+FToStr(-(currItem->height()))+" ]\n");
		PutDoc("/Resources ");
		Pdf::ResourceDictionary dict;
		dict.Pattern = Patterns;
		dict.XObject.unite(pageData.ImgObjects);
		dict.XObject.unite(pageData.XObjects);
		writer.write(dict);
		PutDoc("\n");
		
		QByteArray stre = "q\n";
		if ((currItem->isGroup()) || (currItem->itemType() == PageItem::Symbol))
		{
			QTransform mpa;
			mpa.translate(0, currItem->height() * scaleY);
			mpa.rotate(-currItem->rotation());
			mpa.scale(scaleX, scaleY);
			stre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+" cm\n";
		}
		stre += SetClipPath(currItem)+"h\n";
		stre += FToStr(fabs(currItem->lineWidth()))+" w\n";
		stre += tmpOut+" f*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressArray(stre);
		PutDoc("/Length "+Pdf::toPdf(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream");
		writer.endObj(formObject);
		pageData.XObjects[ResNam+Pdf::toPdf(ResCount)] = formObject;
		ResCount++;
		GXName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		if (currItem->GrMask == 6)
			Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+Pdf::toPdf(formObject)+" 0 R >>\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		else if (currItem->GrMask == 7)
			Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+Pdf::toPdf(formObject)+" 0 R /BC [ 1 1 1 ] /TR << /FunctionType 2 /Domain [ 0 1 ] /Range [ 0 1 ] /C0 [ 1 ] /C1 [ 0 ] /N 1 >> >>\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n/AIS true\n");
		else if (currItem->GrMask == 3)
			Transpar[GXName] = writeGState("/SMask << /S /Alpha /G "+Pdf::toPdf(formObject)+" 0 R >>\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		else if (currItem->GrMask == 8)
			Transpar[GXName] = writeGState("/SMask << /S /Alpha /G "+Pdf::toPdf(formObject)+" 0 R /BC [ 1 1 1 ] /TR << /FunctionType 2 /Domain [ 0 1 ] /Range [ 0 1 ] /C0 [ 1 ] /C1 [ 0 ] /N 1 >> >>\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		tmp = Pdf::toName(GXName)+" gs\n";
	}
	else
	{
		if (currItem->GrType == 14)
			Transpar[ShName] = writeGState("/CA "+FToStr(1.0 - currItem->fillTransparency())+"\n/ca "+FToStr(1.0 - currItem->fillTransparency())+"\n/SMask /None\n/AIS false\n/OPM 1\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		else
			Transpar[ShName] = writeGState("/ca "+FToStr(1.0 - currItem->fillTransparency())+"\n/SMask /None\n/AIS false\n/OPM 1\n/BM /" + blendMode(currItem->fillBlendmode()) + "\n");
		tmp = Pdf::toName(ShName) + " gs\n";
	}
	return tmp;
}

QByteArray PDFLibCore::PDF_TransparenzStroke(PageItem *currItem)
{
	QByteArray ShName = ResNam+Pdf::toPdf(ResCount);
	ResCount++;
	Transpar[ShName] = writeGState("/CA "+FToStr(1.0 - currItem->lineTransparency())+"\n"
								   + "/SMask /None\n/AIS false\n/OPM 1\n"
								   + "/BM /" + blendMode(currItem->lineBlendmode()) + "\n");
	return Pdf::toName(ShName) + " gs\n";
}

bool PDFLibCore::PDF_HatchFill(QByteArray& output, PageItem *currItem)
{
	output += "q\n1 w\n[] 0 d\n0 J\n0 j\n";
	if ((currItem->hatchBackground != CommonStrings::None) && (currItem->hatchUseBackground))
	{
		output += putColor(currItem->hatchBackground, 100, true);
		output += SetClipPath(currItem);
		output += (currItem->fillRule ? "h\nf*\n" : "h\nf\n");
	}
	if (currItem->hatchForeground != CommonStrings::None)
		output += putColor(currItem->hatchForeground, 100, false);
	output += SetPathAndClip(currItem);
	QTransform mpa;
	mpa.translate(currItem->width() / 2.0, -currItem->height() / 2.0);
	output += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+" cm\n";
	double lineLen = sqrt((currItem->width() / 2.0) * (currItem->width() / 2.0) + (currItem->height() / 2.0) * (currItem->height() / 2.0));
	double dist = 0.0;
	output += "q\n";
	QTransform mp;
	mp.rotate(currItem->hatchAngle);
	output += FToStr(mp.m11())+" "+FToStr(mp.m12())+" "+FToStr(mp.m21())+" "+FToStr(mp.m22())+" "+FToStr(mp.dx())+" "+FToStr(mp.dy())+" cm\n";
	while (dist < lineLen)
	{
		output += FToStr(-lineLen) + " " + FToStr(dist) + " m\n";
		output += FToStr(lineLen)+ " " + FToStr(dist) + " l\n";
		output += "S\n";
		if (dist > 0)
		{
			output += FToStr(-lineLen) + " " + FToStr(-dist) + " m\n";
			output += FToStr(lineLen)+ " " + FToStr(-dist) + " l\n";
			output += "S\n";
		}
		dist += currItem->hatchDistance;
	}
	output += "Q\n";
	dist = 0;
	if ((currItem->hatchType == 1) || (currItem->hatchType == 2))
	{
		output += "q\n";
		QTransform mp;
		mp.rotate(currItem->hatchAngle + 90);
		output += FToStr(mp.m11())+" "+FToStr(mp.m12())+" "+FToStr(mp.m21())+" "+FToStr(mp.m22())+" "+FToStr(mp.dx())+" "+FToStr(mp.dy())+" cm\n";
		while (dist < lineLen)
		{
			output += FToStr(-lineLen) + " " + FToStr(dist) + " m\n";
			output += FToStr(lineLen)+ " " + FToStr(dist) + " l\n";
			output += "S\n";
			if (dist > 0)
			{
				output += FToStr(-lineLen) + " " + FToStr(-dist) + " m\n";
				output += FToStr(lineLen)+ " " + FToStr(-dist) + " l\n";
				output += "S\n";
			}
			dist += currItem->hatchDistance;
		}
		output += "Q\n";
	}
	if (currItem->hatchType == 2)
	{
		dist = 0;
		output += "q\n";
		QTransform mp;
		mp.rotate(currItem->hatchAngle - 45);
		output += FToStr(mp.m11())+" "+FToStr(mp.m12())+" "+FToStr(mp.m21())+" "+FToStr(mp.m22())+" "+FToStr(mp.dx())+" "+FToStr(mp.dy())+" cm\n";
		while (dist < lineLen)
		{
			output += FToStr(-lineLen) + " " + FToStr(dist * sqrt(2.0)) + " m\n";
			output += FToStr(lineLen)+ " " + FToStr(dist * sqrt(2.0)) + " l\n";
			output += "S\n";
			if (dist > 0)
			{
				output += FToStr(-lineLen) + " " + FToStr(-dist * sqrt(2.0)) + " m\n";
				output += FToStr(lineLen)+ " " + FToStr(-dist * sqrt(2.0)) + " l\n";
				output += "S\n";
			}
			dist += currItem->hatchDistance;
		}
		output += "Q\n";
	}
	output += "Q\n";
	return true;
}

bool PDFLibCore::PDF_PatternFillStroke(QByteArray& output, PageItem *currItem, int kind, bool forArrow)
{
	QByteArray tmp2 = "", tmpOut;
	ScPattern *pat = NULL;
	if (kind == 0)
	{
		QString itemPattern = currItem->pattern();
		pat = doc.checkedPattern(itemPattern);
		if (!pat)
		{
			if (currItem->fillColor() != CommonStrings::None)
				output += putColor(currItem->fillColor(), currItem->fillShade(), true);
			return true;
		}
	}
	else if (kind == 1)
	{
		QString strokePattern = currItem->strokePattern();
		pat = doc.checkedPattern(strokePattern);
		if (!pat)
		{
			if (currItem->lineColor() != CommonStrings::None)
			{
				output += putColor(currItem->lineColor(), currItem->lineShade(), true);
				output += FToStr(fabs(currItem->lineWidth()))+" w\n";
			}
			return true;
		}
	}
	else if (kind == 2)
	{
		QString patternMask = currItem->patternMask();
		pat = doc.checkedPattern(patternMask);
		if (!pat)
			return true;
	}
	else
		return false;
	for (int em = 0; em < pat->items.count(); ++em)
	{
		PageItem* item = pat->items.at(em);
		tmp2 += "q\n";
		tmp2 +=  "1 0 0 1 "+FToStr(item->gXpos)+" "+FToStr(-(item->gYpos - pat->height))+" cm\n";
		item->setXYPos(item->xPos() + ActPageP->xOffset(), item->yPos() + ActPageP->yOffset(), true);
		patternStackPos.push(QPointF(item->gXpos, -(item->gYpos - pat->height)));
		inPattern++;
		if (!PDF_ProcessItem(tmpOut, item, doc.DocPages.at(0), 0, true, true))
			return false;
		tmp2 += tmpOut;
		item->setXYPos(item->xPos() - ActPageP->xOffset(), item->yPos() - ActPageP->yOffset(), true);
		inPattern--;
		patternStackPos.pop();
		tmp2 += "Q\n";
	}
	if (Options.Compress)
		tmp2 = CompressArray(tmp2);
	PdfId patObject = writer.newObject();
	writer.startObj(patObject);
	PutDoc("<< /Type /Pattern\n");
	PutDoc("/PatternType 1\n");
	PutDoc("/PaintType 1\n");
	PutDoc("/TilingType 1\n");
	PutDoc("/BBox [ 0 0 "+FToStr(pat->width)+" "+FToStr(pat->height)+" ]\n");
	double scaleX = 1.0;
	double scaleY = 1.0;
	if (currItem->itemType() == PageItem::Symbol)
	{
		ScPattern pat = doc.docPatterns[currItem->pattern()];
		scaleX = 1.0 / (currItem->width() / pat.width);
		scaleY = 1.0 / (currItem->height() / pat.height);
	}
	QTransform mpa;
	if ((inPattern == 0) && (kind != 2))
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(currItem->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - currItem->gYpos);
			mpa.rotate(-currItem->rotation());
		}
		else
		{
			mpa.translate(bleedDisplacementX, bleedDisplacementY);
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), ActPageP->height() - (currItem->yPos() - ActPageP->yOffset()));
			mpa.rotate(-currItem->rotation());
		}
	}
	double patternScaleX=0.0, patternScaleY=0.0, patternOffsetX=0.0, patternOffsetY=0.0, patternRotation=0.0, patternSkewX=0.0, patternSkewY=0.0, patternSpace=0.0;
	bool mirrorX=false, mirrorY=false;
	if (kind == 0)
	{
		currItem->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
		currItem->patternFlip(mirrorX, mirrorY);
	}
	else if (kind == 1)
	{
		currItem->strokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
		currItem->strokePatternFlip(mirrorX, mirrorY);
	}
	else if (kind == 2)
	{
		if ((currItem->isGroup()) || (currItem->itemType() == PageItem::Symbol))
		{
			mpa.translate(0, currItem->height() * scaleY);
			mpa.scale(scaleX, scaleY);
		}
		currItem->maskTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
		currItem->maskFlip(mirrorX, mirrorY);
	}
	if (kind == 1)
		mpa.translate(-currItem->lineWidth() / 2.0, currItem->lineWidth() / 2.0);
	// #13649, bad pattern placement: improve the situation, does not totally fix it in complex cases
	if ((kind == 2) && currItem->isImageFrame())
	{
		mpa.translate(currItem->imageXOffset() * currItem->imageXScale(), -currItem->imageYOffset() * currItem->imageYScale());
		mpa.rotate(-currItem->imageRotation());
	}
	mpa.translate(patternOffsetX, -patternOffsetY);
	if (currItem->itemType() == PageItem::Symbol)
		mpa.rotate(patternRotation);
	else
		mpa.rotate(-patternRotation);
	mpa.shear(patternSkewX, -patternSkewY);
	mpa.scale(pat->scaleX, pat->scaleY);
	mpa.scale(patternScaleX / 100.0 , patternScaleY / 100.0);
	if (mirrorX)
		mpa.scale(-1, 1);
	if (mirrorY)
		mpa.scale(1, -1);
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/XStep "+FToStr(pat->width)+"\n");
	PutDoc("/YStep "+FToStr(pat->height)+"\n");
	PutDoc("/Resources ");
	
	Pdf::ResourceDictionary dict;
	dict.XObject.unite(pageData.ImgObjects);
	dict.XObject.unite(pageData.XObjects);
	dict.Font = pageData.FObjects;
	dict.Shading = Shadings;
	dict.Pattern = Patterns;
	dict.ExtGState = Transpar;
	dict.ColorSpace.append(asColorSpace(ICCProfiles.values()));
	dict.ColorSpace.append(asColorSpace(spotMap.values()));
	writer.write(dict);
	
	PutDoc("/Length "+Pdf::toPdf(tmp2.length()));
	if (Options.Compress)
		PutDoc("\n/Filter /FlateDecode");
	PutDoc(" >>\nstream\n"+EncStream(tmp2, patObject)+"\nendstream");
	writer.endObj(patObject);
	Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
	QByteArray tmp;
	if ((forArrow) || (kind != 1))
	{
		tmp = "/Pattern cs\n";
		tmp += "/Pattern"+Pdf::toPdf(patObject)+" scn\n";
	}
	else
	{
		tmp = "/Pattern CS\n";
		tmp += "/Pattern"+Pdf::toPdf(patObject)+" SCN\n";
	}
	ResCount++;
	output = tmp;
	return true;
}

quint32 PDFLibCore::encode32dVal(double val)
{
	quint32 res = static_cast<quint32>(((val - (-40000.0)) / 80000.0) * 0xFFFFFFFF);
	return res;
}

quint16 PDFLibCore::encode16dVal(double val)
{
	quint16 m = val * 0xFFFF;
	return m;
}

void PDFLibCore::encodeColor(QDataStream &vs, QString colName, int colShade, QStringList &spotColorSet, bool spotMode)
{
	if (spotMode)
	{
		if (spotColorSet.contains(colName))
		{
			vs << encode16dVal(0.0) << encode16dVal(0.0) << encode16dVal(0.0) << encode16dVal(0.0);
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				if (spotColorSet.at(sc) == colName)
					vs << encode16dVal(colShade / 100.0);
				else
					vs << encode16dVal(0.0);
			}
		}
		else
		{
			QList<QByteArray> gcol = SetGradientColor(colName, colShade).split(' ');
			for (int gcs = 0; gcs < gcol.count(); gcs++)
			{
				vs << encode16dVal(gcol[gcs].toDouble());
			}
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				vs << encode16dVal(0.0);
			}
		}
	}
	else
	{
		QList<QByteArray> gcol = SetGradientColor(colName, colShade).split(' ');
		for (int gcs = 0; gcs < gcol.count(); gcs++)
		{
			vs << encode16dVal(gcol[gcs].toDouble());
		}
	}
}

bool PDFLibCore::PDF_MeshGradientFill(QByteArray& output, PageItem *c)
{
	QList<double> StopVec;
	QList<double> TransVec;
	QStringList Gcolors;
	QStringList colorNames;
	QList<int> colorShades;
	QStringList spotColorSet;
	QStringList tmpAddedColors;
	bool spotMode = false;
	bool transparencyFound = false;
	StopVec.clear();
	TransVec.clear();
	Gcolors.clear();
	colorNames.clear();
	colorShades.clear();
	tmpAddedColors.clear();
	for (int grow = 0; grow < c->meshGradientArray.count(); grow++)
	{
		for (int gcol = 0; gcol < c->meshGradientArray[grow].count(); gcol++)
		{
			MeshPoint mp1 = c->meshGradientArray[grow][gcol];
			colorNames.append(mp1.colorName);
			if (!doc.PageColors.contains(mp1.colorName))
			{
				if (!tmpAddedColors.contains(mp1.colorName))
				{
					tmpAddedColors.append(mp1.colorName);
					ScColor tmp;
					tmp.setSpotColor(false);
					tmp.setRegistrationColor(false);
					tmp.fromQColor(mp1.color);
					doc.PageColors.insert(mp1.colorName, tmp);
				}
			}
			colorShades.append(mp1.shade);
			TransVec.append(mp1.transparency);
			if (mp1.transparency != 1.0)
				transparencyFound = true;
			if (spotMap.contains(mp1.colorName))
			{
				if (!spotColorSet.contains(mp1.colorName))
					spotColorSet.append(mp1.colorName);
			}
			Gcolors.append(SetGradientColor(mp1.colorName, mp1.shade));
		}
	}
	QByteArray TRes;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		PdfId shadeObjectT = writer.newObject();
		writer.startObj(shadeObjectT);
		PutDoc("<<\n");
		PutDoc("/ShadingType 7\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/BitsPerCoordinate 32\n");
		PutDoc("/BitsPerComponent 16\n");
		PutDoc("/BitsPerFlag 8\n");
		QByteArray vertStreamT;
		QDataStream vst(&vertStreamT, QIODevice::WriteOnly);
		vst.setByteOrder(QDataStream::BigEndian);
		quint8 flg = 0;
		for (int grow = 0; grow < c->meshGradientArray.count()-1; grow++)
		{
			for (int gcol = 0; gcol < c->meshGradientArray[grow].count()-1; gcol++)
			{
				MeshPoint mp1 = c->meshGradientArray[grow][gcol];
				MeshPoint mp2 = c->meshGradientArray[grow][gcol+1];
				MeshPoint mp3 = c->meshGradientArray[grow+1][gcol+1];
				MeshPoint mp4 = c->meshGradientArray[grow+1][gcol];
				int colInd1 = grow * c->meshGradientArray[grow].count() + gcol;
				int colInd2 = grow * c->meshGradientArray[grow].count() + gcol + 1;
				int colInd3 = (grow + 1) * c->meshGradientArray[grow].count() + gcol + 1;
				int colInd4 = (grow + 1) * c->meshGradientArray[grow].count() + gcol;
				vst << flg;
				vst << encode32dVal(mp4.gridPoint.x()) << encode32dVal(-mp4.gridPoint.y()) << encode32dVal(mp4.controlTop.x()) << encode32dVal(-mp4.controlTop.y()) << encode32dVal(mp1.controlBottom.x()) << encode32dVal(-mp1.controlBottom.y());
				vst << encode32dVal(mp1.gridPoint.x()) << encode32dVal(-mp1.gridPoint.y()) << encode32dVal(mp1.controlRight.x()) << encode32dVal(-mp1.controlRight.y()) << encode32dVal(mp2.controlLeft.x()) << encode32dVal(-mp2.controlLeft.y());
				vst << encode32dVal(mp2.gridPoint.x()) << encode32dVal(-mp2.gridPoint.y()) << encode32dVal(mp2.controlBottom.x()) << encode32dVal(-mp2.controlBottom.y()) << encode32dVal(mp3.controlTop.x()) << encode32dVal(-mp3.controlTop.y());
				vst << encode32dVal(mp3.gridPoint.x()) << encode32dVal(-mp3.gridPoint.y()) << encode32dVal(mp3.controlLeft.x()) << encode32dVal(-mp3.controlLeft.y()) << encode32dVal(mp4.controlRight.x()) << encode32dVal(-mp4.controlRight.y());
				vst << encode32dVal(mp4.controlColor.x()) << encode32dVal(-mp4.controlColor.y());
				vst << encode32dVal(mp1.controlColor.x()) << encode32dVal(-mp1.controlColor.y());
				vst << encode32dVal(mp2.controlColor.x()) << encode32dVal(-mp2.controlColor.y());
				vst << encode32dVal(mp3.controlColor.x()) << encode32dVal(-mp3.controlColor.y());
				vst << encode16dVal(TransVec[colInd4]) << encode16dVal(TransVec[colInd1]) << encode16dVal(TransVec[colInd2]) << encode16dVal(TransVec[colInd3]);
			}
		}
		PutDoc("/Decode [-40000 40000 -40000 40000 0 1]\n");
		QByteArray dat = "";
		for (int vd = 0; vd < vertStreamT.count(); vd++)
		{
			dat += vertStreamT[vd];
		}
		if (Options.Compress)
			dat = CompressArray(dat);
		PutDoc("/Length "+Pdf::toPdf(dat.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(dat, shadeObjectT)+"\nendstream");
		writer.endObj(shadeObjectT);
		
		PdfId patObject = writer.newObject();
		writer.startObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Shading "+Pdf::toPdf(shadeObjectT)+" 0 R\n");
		PutDoc(">>");
		writer.endObj(patObject);
		Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
		PdfId formObject = writer.newObject();
		writer.startObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = c->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(c->width()+lw)+" "+FToStr(-(c->height()+lw))+" ]\n");
		PutDoc("/Resources ");
		
		Pdf::ResourceDictionary dict;
		dict.Pattern = Patterns;
		writer.write(dict);

		QByteArray stre = "q\n"+SetClipPath(c)+"h\n";
		stre += FToStr(fabs(c->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+Pdf::toPdf(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressArray(stre);
		PutDoc("/Length "+Pdf::toPdf(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream");
		writer.endObj(shadeObjectT);
		pageData.XObjects[ResNam+Pdf::toPdf(ResCount)] = formObject;
		ResCount++;
		QByteArray GXName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+Pdf::toPdf(formObject)+" 0 R >>\n/BM /Normal\n");
		TRes = GXName;
	}
	QByteArray entx = "";
	PdfId spotObject = 0;
	PdfId shadeObject = writer.newObject();
	writer.startObj(shadeObject);
	PutDoc("<<\n");
	PutDoc("/ShadingType 7\n");
	if (Options.UseRGB)
	{
		PutDoc("/ColorSpace /DeviceRGB\n");
		entx = "0 1 0 1 0 1";
	}
	else if (Options.isGrayscale)
	{
		PutDoc("/ColorSpace /DeviceGray\n");
		entx = "0 1";
	}
	else if ((doc.HasCMS) && (Options.UseProfiles))
	{
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
		entx = "0 1 0 1 0 1";
	}
	else
	{
		entx = "0 1 0 1 0 1 0 1";
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = writer.newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc(" " + Pdf::toName(spotColorSet.at(sc).simplified()));
				       // " /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
				entx += " 0 1";
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(Pdf::toPdf(spotObject)+" 0 R\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	PutDoc("/BitsPerCoordinate 32\n");
	PutDoc("/BitsPerComponent 16\n");
	PutDoc("/BitsPerFlag 8\n");
	QByteArray vertStream;
	QDataStream vs(&vertStream, QIODevice::WriteOnly);
	vs.setByteOrder(QDataStream::BigEndian);
	quint8 flg = 0;
	for (int grow = 0; grow < c->meshGradientArray.count()-1; grow++)
	{
		for (int gcol = 0; gcol < c->meshGradientArray[grow].count()-1; gcol++)
		{
			MeshPoint mp1 = c->meshGradientArray[grow][gcol];
			MeshPoint mp2 = c->meshGradientArray[grow][gcol+1];
			MeshPoint mp3 = c->meshGradientArray[grow+1][gcol+1];
			MeshPoint mp4 = c->meshGradientArray[grow+1][gcol];
			int colInd1 = grow * c->meshGradientArray[grow].count() + gcol;
			int colInd2 = grow * c->meshGradientArray[grow].count() + gcol + 1;
			int colInd3 = (grow + 1) * c->meshGradientArray[grow].count() + gcol + 1;
			int colInd4 = (grow + 1) * c->meshGradientArray[grow].count() + gcol;
			vs << flg;
			vs << encode32dVal(mp4.gridPoint.x()) << encode32dVal(-mp4.gridPoint.y()) << encode32dVal(mp4.controlTop.x()) << encode32dVal(-mp4.controlTop.y()) << encode32dVal(mp1.controlBottom.x()) << encode32dVal(-mp1.controlBottom.y());
			vs << encode32dVal(mp1.gridPoint.x()) << encode32dVal(-mp1.gridPoint.y()) << encode32dVal(mp1.controlRight.x()) << encode32dVal(-mp1.controlRight.y()) << encode32dVal(mp2.controlLeft.x()) << encode32dVal(-mp2.controlLeft.y());
			vs << encode32dVal(mp2.gridPoint.x()) << encode32dVal(-mp2.gridPoint.y()) << encode32dVal(mp2.controlBottom.x()) << encode32dVal(-mp2.controlBottom.y()) << encode32dVal(mp3.controlTop.x()) << encode32dVal(-mp3.controlTop.y());
			vs << encode32dVal(mp3.gridPoint.x()) << encode32dVal(-mp3.gridPoint.y()) << encode32dVal(mp3.controlLeft.x()) << encode32dVal(-mp3.controlLeft.y()) << encode32dVal(mp4.controlRight.x()) << encode32dVal(-mp4.controlRight.y());
			vs << encode32dVal(mp4.controlColor.x()) << encode32dVal(-mp4.controlColor.y());
			vs << encode32dVal(mp1.controlColor.x()) << encode32dVal(-mp1.controlColor.y());
			vs << encode32dVal(mp2.controlColor.x()) << encode32dVal(-mp2.controlColor.y());
			vs << encode32dVal(mp3.controlColor.x()) << encode32dVal(-mp3.controlColor.y());
			encodeColor(vs, colorNames[colInd4], colorShades[colInd4], spotColorSet, spotMode);
			encodeColor(vs, colorNames[colInd1], colorShades[colInd1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[colInd2], colorShades[colInd2], spotColorSet, spotMode);
			encodeColor(vs, colorNames[colInd3], colorShades[colInd3], spotColorSet, spotMode);
		}
	}
	PutDoc("/Decode [-40000 40000 -40000 40000 "+entx+"]\n");
	QByteArray dat = "";
	for (int vd = 0; vd < vertStream.count(); vd++)
	{
		dat += vertStream[vd];
	}
	if (Options.Compress)
		dat = CompressArray(dat);
	PutDoc("/Length "+Pdf::toPdf(dat.length())+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(dat, shadeObject)+"\nendstream");
	writer.endObj(shadeObject);
	
	PdfId patObject = writer.newObject();
	writer.startObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	QTransform mpa;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(c->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - c->gYpos);
			mpa.rotate(-c->rotation());
		}
		else
		{
			mpa.translate(bleedDisplacementX, bleedDisplacementY);
			mpa.translate(c->xPos() - ActPageP->xOffset(), ActPageP->height() - (c->yPos() - ActPageP->yOffset()));
			mpa.rotate(-c->rotation());
		}
	}
	else
	{
		if (patternStackPos.count() != 0)
			mpa.translate(patternStackPos.top().x(), patternStackPos.top().y());
	}
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading "+Pdf::toPdf(shadeObject)+" 0 R\n");
	PutDoc(">>");
	writer.endObj(patObject);
	Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
	if (spotMode)
	{
		QByteArray colorDesc;
		writer.startObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			double cc(0), mc(0), yc(0), kc(0);
			CMYKColorF cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup " + FToStr(cc) + " mul ";
			else
				colorDesc += Pdf::toPdf(sc*4 + 1) + " -1 roll dup " + FToStr(cc) + " mul ";
			colorDesc += "exch dup " + FToStr(mc) + " mul ";
			colorDesc += "exch dup " + FToStr(yc) + " mul ";
			colorDesc += "exch " + FToStr(kc) + " mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+Pdf::toPdf(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream");
		writer.endObj(spotObject);
	}
	QByteArray tmp;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += Pdf::toName(TRes) + " gs\n";
	tmp += "/Pattern cs\n";
	tmp += "/Pattern"+Pdf::toPdf(patObject)+" scn\n";
	output = tmp;
	if (tmpAddedColors.count() != 0)
	{
		for (int cd = 0; cd < tmpAddedColors.count(); cd++)
		{
			doc.PageColors.remove(tmpAddedColors[cd]);
		}
	}
	return true;
}

bool PDFLibCore::PDF_PatchMeshGradientFill(QByteArray& output, PageItem *c)
{
	QList<double> StopVec;
	QList<double> TransVec;
	QStringList Gcolors;
	QStringList colorNames;
	QList<int> colorShades;
	QStringList spotColorSet;
	bool spotMode = false;
	bool transparencyFound = false;
	StopVec.clear();
	TransVec.clear();
	Gcolors.clear();
	colorNames.clear();
	colorShades.clear();
	for (int col = 0; col < c->meshGradientPatches.count(); col++)
	{
		meshGradientPatch patch = c->meshGradientPatches[col];
		MeshPoint mp1 = patch.TL;
		colorNames.append(mp1.colorName);
		colorShades.append(mp1.shade);
		TransVec.append(mp1.transparency);
		if (mp1.transparency != 1.0)
			transparencyFound = true;
		if (spotMap.contains(mp1.colorName))
		{
			if (!spotColorSet.contains(mp1.colorName))
				spotColorSet.append(mp1.colorName);
		}
		Gcolors.append(SetGradientColor(mp1.colorName, mp1.shade));
		MeshPoint mp2 = patch.TR;
		colorNames.append(mp2.colorName);
		colorShades.append(mp2.shade);
		TransVec.append(mp2.transparency);
		if (mp2.transparency != 1.0)
			transparencyFound = true;
		if (spotMap.contains(mp2.colorName))
		{
			if (!spotColorSet.contains(mp2.colorName))
				spotColorSet.append(mp2.colorName);
		}
		Gcolors.append(SetGradientColor(mp2.colorName, mp2.shade));
		MeshPoint mp3 = patch.BR;
		colorNames.append(mp3.colorName);
		colorShades.append(mp3.shade);
		TransVec.append(mp3.transparency);
		if (mp3.transparency != 1.0)
			transparencyFound = true;
		if (spotMap.contains(mp3.colorName))
		{
			if (!spotColorSet.contains(mp3.colorName))
				spotColorSet.append(mp3.colorName);
		}
		Gcolors.append(SetGradientColor(mp3.colorName, mp3.shade));
		MeshPoint mp4 = patch.BL;
		colorNames.append(mp4.colorName);
		colorShades.append(mp4.shade);
		TransVec.append(mp4.transparency);
		if (mp4.transparency != 1.0)
			transparencyFound = true;
		if (spotMap.contains(mp4.colorName))
		{
			if (!spotColorSet.contains(mp4.colorName))
				spotColorSet.append(mp4.colorName);
		}
		Gcolors.append(SetGradientColor(mp4.colorName, mp4.shade));
	}
	QByteArray TRes("");
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		PdfId shadeObjectT = writer.newObject();
		writer.startObj(shadeObjectT);
		PutDoc("<<\n");
		PutDoc("/ShadingType 7\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/BitsPerCoordinate 32\n");
		PutDoc("/BitsPerComponent 16\n");
		PutDoc("/BitsPerFlag 8\n");
		QByteArray vertStreamT;
		QDataStream vst(&vertStreamT, QIODevice::WriteOnly);
		vst.setByteOrder(QDataStream::BigEndian);
		quint8 flg = 0;
		for (int col = 0; col < c->meshGradientPatches.count(); col++)
		{
			meshGradientPatch patch = c->meshGradientPatches[col];
			MeshPoint mp1 = patch.TL;
			MeshPoint mp2 = patch.TR;
			MeshPoint mp3 = patch.BR;
			MeshPoint mp4 = patch.BL;
			int colInd1 = 4 * col;
			int colInd2 = 4 * col + 1;
			int colInd3 = 4 * col + 2;
			int colInd4 = 4 * col + 3;
			vst << flg;
			vst << encode32dVal(mp4.gridPoint.x()) << encode32dVal(-mp4.gridPoint.y()) << encode32dVal(mp4.controlTop.x()) << encode32dVal(-mp4.controlTop.y()) << encode32dVal(mp1.controlBottom.x()) << encode32dVal(-mp1.controlBottom.y());
			vst << encode32dVal(mp1.gridPoint.x()) << encode32dVal(-mp1.gridPoint.y()) << encode32dVal(mp1.controlRight.x()) << encode32dVal(-mp1.controlRight.y()) << encode32dVal(mp2.controlLeft.x()) << encode32dVal(-mp2.controlLeft.y());
			vst << encode32dVal(mp2.gridPoint.x()) << encode32dVal(-mp2.gridPoint.y()) << encode32dVal(mp2.controlBottom.x()) << encode32dVal(-mp2.controlBottom.y()) << encode32dVal(mp3.controlTop.x()) << encode32dVal(-mp3.controlTop.y());
			vst << encode32dVal(mp3.gridPoint.x()) << encode32dVal(-mp3.gridPoint.y()) << encode32dVal(mp3.controlLeft.x()) << encode32dVal(-mp3.controlLeft.y()) << encode32dVal(mp4.controlRight.x()) << encode32dVal(-mp4.controlRight.y());
			vst << encode32dVal(mp4.controlColor.x()) << encode32dVal(-mp4.controlColor.y());
			vst << encode32dVal(mp1.controlColor.x()) << encode32dVal(-mp1.controlColor.y());
			vst << encode32dVal(mp2.controlColor.x()) << encode32dVal(-mp2.controlColor.y());
			vst << encode32dVal(mp3.controlColor.x()) << encode32dVal(-mp3.controlColor.y());
			vst << encode16dVal(TransVec[colInd4]) << encode16dVal(TransVec[colInd1]) << encode16dVal(TransVec[colInd2]) << encode16dVal(TransVec[colInd3]);
		}
		PutDoc("/Decode [-40000 40000 -40000 40000 0 1]\n");
		QByteArray dat = "";
		for (int vd = 0; vd < vertStreamT.count(); vd++)
		{
			dat += vertStreamT[vd];
		}
		if (Options.Compress)
			dat = CompressArray(dat);
		PutDoc("/Length "+Pdf::toPdf(dat.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(dat, shadeObjectT)+"\nendstream");
		writer.endObj(shadeObjectT);
		PdfId patObject = writer.newObject();
		writer.startObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Shading "+Pdf::toPdf(shadeObjectT)+" 0 R\n");
		PutDoc(">>");
		writer.endObj(patObject);
		Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
		PdfId formObject = writer.newObject();
		writer.startObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = c->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(c->width()+lw)+" "+FToStr(-(c->height()+lw))+" ]\n");
		PutDoc("/Resources ");
		
		Pdf::ResourceDictionary dict;
		dict.Pattern = Patterns;
		writer.write(dict);
		
		QByteArray stre = "q\n"+SetClipPath(c)+"h\n";
		stre += FToStr(fabs(c->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+Pdf::toPdf(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressArray(stre);
		PutDoc("/Length "+Pdf::toPdf(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream");
		writer.endObj(formObject);
		pageData.XObjects[ResNam+Pdf::toPdf(ResCount)] = formObject;
		ResCount++;
		QByteArray GXName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+Pdf::toPdf(formObject)+" 0 R >>\n/BM /Normal\n");
		TRes = GXName;
	}
	QByteArray entx = "";
	PdfId spotObject = 0;
	PdfId shadeObject = writer.newObject();
	writer.startObj(shadeObject);
	PutDoc("<<\n");
	PutDoc("/ShadingType 7\n");
	if (Options.UseRGB)
	{
		PutDoc("/ColorSpace /DeviceRGB\n");
		entx = "0 1 0 1 0 1";
	}
	else if (Options.isGrayscale)
	{
		PutDoc("/ColorSpace /DeviceGray\n");
		entx = "0 1";
	}
	else if ((doc.HasCMS) && (Options.UseProfiles))
	{
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
		entx = "0 1 0 1 0 1";
	}
	else
	{
		entx = "0 1 0 1 0 1 0 1";
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = writer.newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc(" " + Pdf::toName(spotColorSet.at(sc).simplified()));
				       //" /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
				entx += " 0 1";
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(Pdf::toObjRef(spotObject) + "\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	PutDoc("/BitsPerCoordinate 32\n");
	PutDoc("/BitsPerComponent 16\n");
	PutDoc("/BitsPerFlag 8\n");
	QByteArray vertStream;
	QDataStream vs(&vertStream, QIODevice::WriteOnly);
	vs.setByteOrder(QDataStream::BigEndian);
	quint8 flg = 0;
	for (int col = 0; col < c->meshGradientPatches.count(); col++)
	{
		meshGradientPatch patch = c->meshGradientPatches[col];
		MeshPoint mp1 = patch.TL;
		MeshPoint mp2 = patch.TR;
		MeshPoint mp3 = patch.BR;
		MeshPoint mp4 = patch.BL;
		int colInd1 = 4 * col;
		int colInd2 = 4 * col + 1;
		int colInd3 = 4 * col + 2;
		int colInd4 = 4 * col + 3;
		vs << flg;
		vs << encode32dVal(mp4.gridPoint.x()) << encode32dVal(-mp4.gridPoint.y()) << encode32dVal(mp4.controlTop.x()) << encode32dVal(-mp4.controlTop.y()) << encode32dVal(mp1.controlBottom.x()) << encode32dVal(-mp1.controlBottom.y());
		vs << encode32dVal(mp1.gridPoint.x()) << encode32dVal(-mp1.gridPoint.y()) << encode32dVal(mp1.controlRight.x()) << encode32dVal(-mp1.controlRight.y()) << encode32dVal(mp2.controlLeft.x()) << encode32dVal(-mp2.controlLeft.y());
		vs << encode32dVal(mp2.gridPoint.x()) << encode32dVal(-mp2.gridPoint.y()) << encode32dVal(mp2.controlBottom.x()) << encode32dVal(-mp2.controlBottom.y()) << encode32dVal(mp3.controlTop.x()) << encode32dVal(-mp3.controlTop.y());
		vs << encode32dVal(mp3.gridPoint.x()) << encode32dVal(-mp3.gridPoint.y()) << encode32dVal(mp3.controlLeft.x()) << encode32dVal(-mp3.controlLeft.y()) << encode32dVal(mp4.controlRight.x()) << encode32dVal(-mp4.controlRight.y());
		vs << encode32dVal(mp4.controlColor.x()) << encode32dVal(-mp4.controlColor.y());
		vs << encode32dVal(mp1.controlColor.x()) << encode32dVal(-mp1.controlColor.y());
		vs << encode32dVal(mp2.controlColor.x()) << encode32dVal(-mp2.controlColor.y());
		vs << encode32dVal(mp3.controlColor.x()) << encode32dVal(-mp3.controlColor.y());
		encodeColor(vs, colorNames[colInd4], colorShades[colInd4], spotColorSet, spotMode);
		encodeColor(vs, colorNames[colInd1], colorShades[colInd1], spotColorSet, spotMode);
		encodeColor(vs, colorNames[colInd2], colorShades[colInd2], spotColorSet, spotMode);
		encodeColor(vs, colorNames[colInd3], colorShades[colInd3], spotColorSet, spotMode);
	}
	PutDoc("/Decode [-40000 40000 -40000 40000 "+entx+"]\n");
	QByteArray dat = "";
	for (int vd = 0; vd < vertStream.count(); vd++)
	{
		dat += vertStream[vd];
	}
	if (Options.Compress)
		dat = CompressArray(dat);
	PutDoc("/Length "+Pdf::toPdf(dat.length())+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(dat, shadeObject)+"\nendstream");
	writer.endObj(shadeObject);
	PdfId patObject = writer.newObject();
	writer.startObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	QTransform mpa;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(c->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - c->gYpos);
			mpa.rotate(-c->rotation());
		}
		else
		{
			mpa.translate(bleedDisplacementX, bleedDisplacementY);
			mpa.translate(c->xPos() - ActPageP->xOffset(), ActPageP->height() - (c->yPos() - ActPageP->yOffset()));
			mpa.rotate(-c->rotation());
		}
	}
	else
	{
		if (patternStackPos.count() != 0)
			mpa.translate(patternStackPos.top().x(), patternStackPos.top().y());
	}
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading "+Pdf::toPdf(shadeObject)+" 0 R\n");
	PutDoc(">>");
	writer.endObj(patObject);
	Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
	if (spotMode)
	{
		QByteArray colorDesc;
		writer.startObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			double cc(0), mc(0), yc(0), kc(0);
			CMYKColorF cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup " + FToStr(cc) + " mul ";
			else
				colorDesc += Pdf::toPdf(sc*4 + 1) + " -1 roll dup " + FToStr(cc) + " mul ";
			colorDesc += "exch dup " + FToStr(mc) + " mul ";
			colorDesc += "exch dup " + FToStr(yc) + " mul ";
			colorDesc += "exch " + FToStr(kc) + " mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+Pdf::toPdf(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream");
		writer.endObj(spotObject);
	}
	QByteArray tmp;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += Pdf::toName(TRes) + " gs\n";
	tmp += "/Pattern cs\n";
	tmp += "/Pattern"+Pdf::toPdf(patObject)+" scn\n";
	output = tmp;
	return true;
}

bool PDFLibCore::PDF_DiamondGradientFill(QByteArray& output, PageItem *c)
{
	QList<double> StopVec;
	QList<double> TransVec;
	QStringList Gcolors;
	QStringList colorNames;
	QList<int> colorShades;
	QStringList spotColorSet;
	VGradient gradient;
	bool spotMode = false;
	bool transparencyFound = false;
	if (!(c->gradient().isEmpty()) && (doc.docGradients.contains(c->gradient())))
		gradient = doc.docGradients[c->gradient()];
	else
		gradient = c->fill_gradient;
	QList<VColorStop*> cstops = gradient.colorStops();
	StopVec.clear();
	TransVec.clear();
	Gcolors.clear();
	colorNames.clear();
	colorShades.clear();
	for (uint cst = 0; cst < gradient.Stops(); ++cst)
	{
		double actualStop = cstops.at(cst)->rampPoint;
		if (cst == 0)
		{
			StopVec.append(0);
			colorNames.append(cstops.at(cst)->name);
			colorShades.append(cstops.at(cst)->shade);
			TransVec.append(cstops.at(cst)->opacity);
			if (spotMap.contains(cstops.at(cst)->name))
			{
				if (!spotColorSet.contains(cstops.at(cst)->name))
					spotColorSet.append(cstops.at(cst)->name);
			}
			Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
		}
		StopVec.append(actualStop);
		colorNames.append(cstops.at(cst)->name);
		colorShades.append(cstops.at(cst)->shade);
		TransVec.append(cstops.at(cst)->opacity);
		if (cstops.at(cst)->opacity != 1.0)
			transparencyFound = true;
		if (spotMap.contains(cstops.at(cst)->name))
		{
			if (!spotColorSet.contains(cstops.at(cst)->name))
				spotColorSet.append(cstops.at(cst)->name);
		}
		Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
	}
	QPointF cP = QPointF(c->GrControl5.x(), -c->GrControl5.y());
	QLineF edge1 = QLineF(cP, QPointF(c->GrControl1.x(), -c->GrControl1.y()));
	QLineF edge2 = QLineF(cP, QPointF(c->GrControl2.x(), -c->GrControl2.y()));
	QLineF edge3 = QLineF(cP, QPointF(c->GrControl3.x(), -c->GrControl3.y()));
	QLineF edge4 = QLineF(cP, QPointF(c->GrControl4.x(), -c->GrControl4.y()));
	QByteArray TRes("");
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		PdfId shadeObjectT = writer.newObject();
		writer.startObj(shadeObjectT);
		PutDoc("<<\n");
		PutDoc("/ShadingType 6\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/BitsPerCoordinate 32\n");
		PutDoc("/BitsPerComponent 16\n");
		PutDoc("/BitsPerFlag 8\n");
		PutDoc("/Background [1]\n");
		QByteArray vertStreamT;
		QDataStream vst(&vertStreamT, QIODevice::WriteOnly);
		vst.setByteOrder(QDataStream::BigEndian);
		quint8 flg = 0;
		for (int offset = 1; offset < StopVec.count(); ++offset)
		{
			QLineF e1 = edge1;
			QLineF e1s = edge1;
			QLineF e2 = edge2;
			QLineF e2s = edge2;
			QLineF e3 = edge3;
			QLineF e3s = edge3;
			QLineF e4 = edge4;
			QLineF e4s = edge4;
			e1.setLength(edge1.length() * StopVec[ offset ]);
			e2.setLength(edge2.length() * StopVec[ offset ]);
			e3.setLength(edge3.length() * StopVec[ offset ]);
			e4.setLength(edge4.length() * StopVec[ offset ]);
			e1s.setLength(edge1.length() * StopVec[ offset - 1 ]);
			e2s.setLength(edge2.length() * StopVec[ offset - 1 ]);
			e3s.setLength(edge3.length() * StopVec[ offset - 1 ]);
			e4s.setLength(edge4.length() * StopVec[ offset - 1 ]);
			if (offset == 1)
			{
				vst << flg;
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
				vst << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
				vst << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode16dVal(TransVec[0]) << encode16dVal(TransVec[1]) << encode16dVal(TransVec[1]) << encode16dVal(TransVec[0]);
				vst << flg;
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
				vst << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
				vst << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode16dVal(TransVec[0]) << encode16dVal(TransVec[0]) << encode16dVal(TransVec[1]) << encode16dVal(TransVec[1]);
				vst << flg;
				vst << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
				vst << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
				vst << encode16dVal(TransVec[1]) << encode16dVal(TransVec[0]) << encode16dVal(TransVec[0]) << encode16dVal(TransVec[1]);
				vst << flg;
				vst << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
				vst << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
				vst << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
				vst << encode16dVal(TransVec[1]) << encode16dVal(TransVec[1]) << encode16dVal(TransVec[0]) << encode16dVal(TransVec[0]);
			}
			else
			{
				vst << flg;
				vst << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
				vst << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
				vst << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2());
				vst << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2());
				vst << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset-1]);
				vst << flg;
				vst << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2());
				vst << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
				vst << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
				vst << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2());
				vst << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset]);
				vst << flg;
				vst << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2());
				vst << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2());
				vst << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
				vst << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
				vst << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset]);
				vst << flg;
				vst << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
				vst << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2());
				vst << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2());
				vst << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
				vst << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset]) << encode16dVal(TransVec[offset-1]) << encode16dVal(TransVec[offset-1]);
			}
		}
		PutDoc("/Decode [-40000 40000 -40000 40000 0 1]\n");
		QByteArray dat = "";
		for (int vd = 0; vd < vertStreamT.count(); vd++)
		{
			dat += vertStreamT[vd];
		}
		if (Options.Compress)
			dat = CompressArray(dat);
		PutDoc("/Length "+Pdf::toPdf(dat.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(dat, shadeObjectT)+"\nendstream");
		writer.endObj(shadeObjectT);
		PdfId patObject = writer.newObject();
		writer.startObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Shading "+Pdf::toPdf(shadeObjectT)+" 0 R\n");
		PutDoc(">>");
		writer.endObj(patObject);
		Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
		PdfId formObject = writer.newObject();
		writer.startObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = c->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(c->width()+lw)+" "+FToStr(-(c->height()+lw))+" ]\n");
		PutDoc("/Resources ");
		
		Pdf::ResourceDictionary dict;
		dict.Pattern = Patterns;
		writer.write(dict);
		
		QByteArray stre = "q\n"+SetClipPath(c)+"h\n";
		stre += FToStr(fabs(c->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+Pdf::toPdf(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressArray(stre);
		PutDoc("/Length "+Pdf::toPdf(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream");
		writer.endObj(formObject);
		pageData.XObjects[ResNam+Pdf::toPdf(ResCount)] = formObject;
		ResCount++;
		QByteArray GXName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+Pdf::toPdf(formObject)+" 0 R >>\n/BM /Normal\n");
		TRes = GXName;
	}
	QByteArray entx = "";
	PdfId spotObject = 0;
	PdfId shadeObject = writer.newObject();
	writer.startObj(shadeObject);
	PutDoc("<<\n");
	PutDoc("/ShadingType 6\n");
	if (Options.UseRGB)
	{
		PutDoc("/ColorSpace /DeviceRGB\n");
		entx = "0 1 0 1 0 1";
	}
	else if (Options.isGrayscale)
	{
		PutDoc("/ColorSpace /DeviceGray\n");
		entx = "0 1";
	}
	else if ((doc.HasCMS) && (Options.UseProfiles))
	{
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
		entx = "0 1 0 1 0 1";
	}
	else
	{
		entx = "0 1 0 1 0 1 0 1";
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = writer.newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc(" " + Pdf::toName(spotColorSet.at(sc).simplified()));
				       //" /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
				entx += " 0 1";
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(Pdf::toPdf(spotObject)+" 0 R\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	PutDoc("/Background [" + Pdf::toAscii(Gcolors.last()) + "]\n");
	PutDoc("/BitsPerCoordinate 32\n");
	PutDoc("/BitsPerComponent 16\n");
	PutDoc("/BitsPerFlag 8\n");
	QByteArray vertStream;
	QDataStream vs(&vertStream, QIODevice::WriteOnly);
	vs.setByteOrder(QDataStream::BigEndian);
	quint8 flg = 0;
	for (int offset = 1; offset < StopVec.count(); ++offset)
	{
		QLineF e1 = edge1;
		QLineF e1s = edge1;
		QLineF e2 = edge2;
		QLineF e2s = edge2;
		QLineF e3 = edge3;
		QLineF e3s = edge3;
		QLineF e4 = edge4;
		QLineF e4s = edge4;
		e1.setLength(edge1.length() * StopVec[ offset ]);
		e2.setLength(edge2.length() * StopVec[ offset ]);
		e3.setLength(edge3.length() * StopVec[ offset ]);
		e4.setLength(edge4.length() * StopVec[ offset ]);
		e1s.setLength(edge1.length() * StopVec[ offset - 1 ]);
		e2s.setLength(edge2.length() * StopVec[ offset - 1 ]);
		e3s.setLength(edge3.length() * StopVec[ offset - 1 ]);
		e4s.setLength(edge4.length() * StopVec[ offset - 1 ]);
		if (offset == 1)
		{
			vs << flg;
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
			vs << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
			vs << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
			vs << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
			vs << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
			vs << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
			vs << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y());
			vs << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(cP.x()) << encode32dVal(cP.y()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[1], colorShades[1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
			encodeColor(vs, colorNames[0], colorShades[0], spotColorSet, spotMode);
		}
		else
		{
			vs << flg;
			vs << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
			vs << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
			vs << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2());
			vs << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2());
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2());
			vs << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2s.x2()) << encode32dVal(e2s.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2());
			vs << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e2.x2()) << encode32dVal(e2.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
			vs << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2());
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2());
			vs << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2());
			vs << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3s.x2()) << encode32dVal(e3s.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2());
			vs << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e3.x2()) << encode32dVal(e3.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			vs << flg;
			vs << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2());
			vs << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1.x2()) << encode32dVal(e1.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2());
			vs << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e1s.x2()) << encode32dVal(e1s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2());
			vs << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4s.x2()) << encode32dVal(e4s.y2()) << encode32dVal(e4.x2()) << encode32dVal(e4.y2());
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset], colorShades[offset], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
			encodeColor(vs, colorNames[offset-1], colorShades[offset-1], spotColorSet, spotMode);
		}
	}
	PutDoc("/Decode [-40000 40000 -40000 40000 "+entx+"]\n");
	QByteArray dat = "";
	for (int vd = 0; vd < vertStream.count(); vd++)
	{
		dat += vertStream[vd];
	}
	if (Options.Compress)
		dat = CompressArray(dat);
	PutDoc("/Length "+Pdf::toPdf(dat.length())+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(dat, shadeObject)+"\nendstream");
	writer.endObj(shadeObject);
	PdfId patObject = writer.newObject();
	writer.startObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	QTransform mpa;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(c->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - c->gYpos);
			mpa.rotate(-c->rotation());
		}
		else
		{
			mpa.translate(bleedDisplacementX, bleedDisplacementY);
			mpa.translate(c->xPos() - ActPageP->xOffset(), ActPageP->height() - (c->yPos() - ActPageP->yOffset()));
			mpa.rotate(-c->rotation());
		}
	}
	else
	{
		if (patternStackPos.count() != 0)
			mpa.translate(patternStackPos.top().x(), patternStackPos.top().y());
	}
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading "+Pdf::toPdf(shadeObject)+" 0 R\n");
	PutDoc(">>");
	writer.endObj(patObject);
	Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
	if (spotMode)
	{
		QByteArray colorDesc;
		writer.startObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			double cc(0), mc(0), yc(0), kc(0);
			CMYKColorF cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup " + FToStr(cc) + " mul ";
			else
				colorDesc += Pdf::toPdf(sc*4 + 1) + " -1 roll dup " + FToStr(cc) + " mul ";
			colorDesc += "exch dup " + FToStr(mc) + " mul ";
			colorDesc += "exch dup " + FToStr(yc) + " mul ";
			colorDesc += "exch " + FToStr(kc) + " mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+Pdf::toPdf(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream");
		writer.endObj(spotObject);
	}
	QByteArray tmp;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += Pdf::toName(TRes) + " gs\n";
	tmp += "/Pattern cs\n";
	tmp += "/Pattern"+Pdf::toPdf(patObject)+" scn\n";
	output = tmp;
	return true;

}

bool PDFLibCore::PDF_TensorGradientFill(QByteArray& output, PageItem *c)
{
	QList<int> colorShades;
	QStringList spotColorSet;
	QStringList colorNames;
	QStringList Gcolors;
	bool spotMode = false;
	bool transparencyFound = false;
	colorNames << c->GrColorP4 << c->GrColorP1 << c->GrColorP2 << c->GrColorP3;
	colorShades << c->GrCol4Shade << c->GrCol1Shade << c->GrCol2Shade << c->GrCol3Shade;
	for (int cst = 0; cst < colorNames.count(); ++cst)
	{
		if (cst == 0)
		{
			if (c->GrCol4transp != 1.0)
				transparencyFound = true;
		}
		else if (cst == 1)
		{
			if (c->GrCol1transp != 1.0)
				transparencyFound = true;
		}
		else if (cst == 2)
		{
			if (c->GrCol2transp != 1.0)
				transparencyFound = true;
		}
		else if (cst == 3)
		{
			if (c->GrCol3transp != 1.0)
				transparencyFound = true;
		}
		if (spotMap.contains(colorNames.at(cst)))
		{
			if (!spotColorSet.contains(colorNames.at(cst)))
				spotColorSet.append(colorNames.at(cst));
		}
		Gcolors.append(SetGradientColor(colorNames.at(cst), colorShades[cst]));
	}
	QByteArray TRes("");
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		PdfId shadeObjectT = writer.newObject();
		writer.startObj(shadeObjectT);
		PutDoc("<<\n");
		PutDoc("/ShadingType 7\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		PutDoc("/BitsPerCoordinate 32\n");
		PutDoc("/BitsPerComponent 16\n");
		PutDoc("/BitsPerFlag 8\n");
		QByteArray vertStreamT;
		QDataStream vst(&vertStreamT, QIODevice::WriteOnly);
		vst.setByteOrder(QDataStream::BigEndian);
		quint8 flg = 0;
		vst << flg;
		quint32 val = 0;
		vst << encode32dVal(val) << encode32dVal(-c->height());
		vst << encode32dVal(val) << encode32dVal(-c->height());
		vst << encode32dVal(val) << encode32dVal(val);
		vst << encode32dVal(val) << encode32dVal(val);
		vst << encode32dVal(val) << encode32dVal(val);
		vst << encode32dVal(c->width()) << encode32dVal(val);
		vst << encode32dVal(c->width()) << encode32dVal(val);
		vst << encode32dVal(c->width()) << encode32dVal(val);
		vst << encode32dVal(c->width()) << encode32dVal(-c->height());
		vst << encode32dVal(c->width()) << encode32dVal(-c->height());
		vst << encode32dVal(c->width()) << encode32dVal(-c->height());
		vst << encode32dVal(val) << encode32dVal(-c->height());
		vst << encode32dVal(c->GrControl1.x()) << encode32dVal(-c->GrControl1.y());
		vst << encode32dVal(c->GrControl4.x()) << encode32dVal(-c->GrControl4.y());
		vst << encode32dVal(c->GrControl3.x()) << encode32dVal(-c->GrControl3.y());
		vst << encode32dVal(c->GrControl2.x()) << encode32dVal(-c->GrControl2.y());
		vst << encode16dVal(c->GrCol4transp) << encode16dVal(c->GrCol1transp) << encode16dVal(c->GrCol2transp) << encode16dVal(c->GrCol3transp);
		PutDoc("/Decode [-40000 40000 -40000 40000 0 1]\n");
		QByteArray dat = "";
		for (int vd = 0; vd < vertStreamT.count(); vd++)
		{
			dat += vertStreamT[vd];
		}
		if (Options.Compress)
			dat = CompressArray(dat);
		PutDoc("/Length "+Pdf::toPdf(dat.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(dat, shadeObjectT)+"\nendstream");
		writer.endObj(shadeObjectT);
		PdfId patObject = writer.newObject();
		writer.startObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Shading "+Pdf::toPdf(shadeObjectT)+" 0 R\n");
		PutDoc(">>");
		writer.endObj(patObject);
		Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
		PdfId formObject = writer.newObject();
		writer.startObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = c->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(c->width()+lw)+" "+FToStr(-(c->height()+lw))+" ]\n");
		PutDoc("/Resources ");
		
		Pdf::ResourceDictionary dict;
		dict.Pattern = Patterns;
		writer.write(dict);
		
		QByteArray stre = "q\n"+SetClipPath(c)+"h\n";
		stre += FToStr(fabs(c->lineWidth()))+" w\n";
		stre += "/Pattern cs\n";
		stre += "/Pattern"+Pdf::toPdf(patObject)+" scn\nf*\n";
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressArray(stre);
		PutDoc("/Length "+Pdf::toPdf(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream");
		writer.endObj(formObject);
		pageData.XObjects[ResNam+Pdf::toPdf(ResCount)] = formObject;
		ResCount++;
		QByteArray GXName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+Pdf::toPdf(formObject)+" 0 R >>\n/BM /Normal\n");
		TRes = GXName;
	}
	QByteArray entx = "";
	PdfId spotObject = 0;
	PdfId shadeObject = writer.newObject();
	writer.startObj(shadeObject);
	PutDoc("<<\n");
	PutDoc("/ShadingType 7\n");
	if (Options.UseRGB)
	{
		PutDoc("/ColorSpace /DeviceRGB\n");
		entx = "0 1 0 1 0 1";
	}
	else if (Options.isGrayscale)
	{
		PutDoc("/ColorSpace /DeviceGray\n");
		entx = "0 1";
	}
	else if ((doc.HasCMS) && (Options.UseProfiles))
	{
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
		entx = "0 1 0 1 0 1";
	}
	else
	{
		entx = "0 1 0 1 0 1 0 1";
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = writer.newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc(" " + Pdf::toName(spotColorSet.at(sc).simplified()));
				       //" /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
				entx += " 0 1";
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(Pdf::toPdf(spotObject)+" 0 R\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	PutDoc("/BitsPerCoordinate 32\n");
	PutDoc("/BitsPerComponent 16\n");
	PutDoc("/BitsPerFlag 8\n");
	QByteArray vertStream;
	QDataStream vs(&vertStream, QIODevice::WriteOnly);
	vs.setByteOrder(QDataStream::BigEndian);
	quint8 flg = 0;
	vs << flg;
	quint32 val = 0;
	vs << encode32dVal(val) << encode32dVal(-c->height());
	vs << encode32dVal(val) << encode32dVal(-c->height());
	vs << encode32dVal(val) << encode32dVal(val);
	vs << encode32dVal(val) << encode32dVal(val);
	vs << encode32dVal(val) << encode32dVal(val);
	vs << encode32dVal(c->width()) << encode32dVal(val);
	vs << encode32dVal(c->width()) << encode32dVal(val);
	vs << encode32dVal(c->width()) << encode32dVal(val);
	vs << encode32dVal(c->width()) << encode32dVal(-c->height());
	vs << encode32dVal(c->width()) << encode32dVal(-c->height());
	vs << encode32dVal(c->width()) << encode32dVal(-c->height());
	vs << encode32dVal(val) << encode32dVal(-c->height());
	vs << encode32dVal(c->GrControl1.x()) << encode32dVal(-c->GrControl1.y());
	vs << encode32dVal(c->GrControl4.x()) << encode32dVal(-c->GrControl4.y());
	vs << encode32dVal(c->GrControl3.x()) << encode32dVal(-c->GrControl3.y());
	vs << encode32dVal(c->GrControl2.x()) << encode32dVal(-c->GrControl2.y());
	for (int cc = 0; cc < colorNames.count(); cc++)
	{
		if (spotMode)
		{
			if (spotColorSet.contains(colorNames.at(cc)))
			{
				vs << encode16dVal(0.0) << encode16dVal(0.0) << encode16dVal(0.0) << encode16dVal(0.0);
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == colorNames.at(cc))
						vs << encode16dVal(colorShades[cc] / 100.0);
					else
						vs << encode16dVal(0.0);
				}
			}
			else
			{
				QStringList gcol = Gcolors[cc].split(" ");
				for (int gcs = 0; gcs < gcol.count(); gcs++)
				{
					vs << encode16dVal(gcol[gcs].toDouble());
				}
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					vs << encode16dVal(0.0);
				}
			}
		}
		else
		{
			QStringList gcol = Gcolors[cc].split(" ");
			for (int gcs = 0; gcs < gcol.count(); gcs++)
			{
				vs << encode16dVal(gcol[gcs].toDouble());
			}
		}
	}
	PutDoc("/Decode [-40000 40000 -40000 40000 "+entx+"]\n");
	QByteArray dat = "";
	for (int vd = 0; vd < vertStream.count(); vd++)
	{
		dat += vertStream[vd];
	}
	if (Options.Compress)
		dat = CompressArray(dat);
	PutDoc("/Length "+Pdf::toPdf(dat.length())+"\n");
	if (Options.Compress)
		PutDoc("/Filter /FlateDecode\n");
	PutDoc(">>\nstream\n"+EncStream(dat, shadeObject)+"\nendstream");
	writer.endObj(shadeObject);
	PdfId patObject = writer.newObject();
	writer.startObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	QTransform mpa;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(c->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - c->gYpos);
			mpa.rotate(-c->rotation());
		}
		else
		{
			mpa.translate(bleedDisplacementX, bleedDisplacementY);
			mpa.translate(c->xPos() - ActPageP->xOffset(), ActPageP->height() - (c->yPos() - ActPageP->yOffset()));
			mpa.rotate(-c->rotation());
		}
	}
	else
	{
		if (patternStackPos.count() != 0)
			mpa.translate(patternStackPos.top().x(), patternStackPos.top().y());
	}
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading "+Pdf::toPdf(shadeObject)+" 0 R\n");
	PutDoc(">>");
	writer.endObj(patObject);
	Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
	if (spotMode)
	{
		QByteArray colorDesc;
		writer.startObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			double cc(0), mc(0), yc(0), kc(0);
			CMYKColorF cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup " + FToStr(cc) + " mul ";
			else
				colorDesc += Pdf::toPdf(sc*4 + 1) + " -1 roll dup " + FToStr(cc) + " mul ";
			colorDesc += "exch dup " + FToStr(mc) + " mul ";
			colorDesc += "exch dup " + FToStr(yc) + " mul ";
			colorDesc += "exch " + FToStr(kc) + " mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+Pdf::toPdf(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream");
		writer.endObj(spotObject);
	}
	QByteArray tmp;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += Pdf::toName(TRes) + " gs\n";
	tmp += "/Pattern cs\n";
	tmp += "/Pattern"+Pdf::toPdf(patObject)+" scn\n";
	output = tmp;
	return true;
}

bool PDFLibCore::PDF_GradientFillStroke(QByteArray& output, PageItem *currItem, bool stroke, bool forArrow)
{
	QList<double> StopVec;
	QList<double> TransVec;
	QStringList Gcolors;
	QStringList colorNames;
	QList<int> colorShades;
	QStringList spotColorSet;
	VGradient gradient;
	double StartX, StartY, EndX, EndY, FocalX, FocalY, Gscale, Gskew;
	int GType;
	if (stroke)
	{
		GType = currItem->GrTypeStroke;
		StartX = currItem->GrStrokeStartX;
		StartY = currItem->GrStrokeStartY;
		EndX = currItem->GrStrokeEndX;
		EndY = currItem->GrStrokeEndY;
		FocalX = currItem->GrStrokeFocalX;
		FocalY = currItem->GrStrokeFocalY;
		Gscale = currItem->GrStrokeScale;
		Gskew = currItem->GrStrokeSkew;
		if (!(currItem->strokeGradient().isEmpty()) && (doc.docGradients.contains(currItem->strokeGradient())))
			gradient = doc.docGradients[currItem->strokeGradient()];
		else
			gradient = currItem->stroke_gradient;
		gradient.setRepeatMethod(currItem->getStrokeGradientExtend());
	}
	else
	{
		GType = currItem->GrType;
		if (GType == 9)
			return PDF_TensorGradientFill(output, currItem);
		else if (GType == 10)
			return PDF_DiamondGradientFill(output, currItem);
		else if ((GType == 11) || (GType == 13))
			return PDF_MeshGradientFill(output, currItem);
		else if (GType == 12)
			return PDF_PatchMeshGradientFill(output, currItem);
		StartX = currItem->GrStartX;
		StartY = currItem->GrStartY;
		EndX = currItem->GrEndX;
		EndY = currItem->GrEndY;
		FocalX = currItem->GrFocalX;
		FocalY = currItem->GrFocalY;
		Gscale = currItem->GrScale;
		Gskew = currItem->GrSkew;
		if (!(currItem->gradient().isEmpty()) && (doc.docGradients.contains(currItem->gradient())))
			gradient = doc.docGradients[currItem->gradient()];
		else
			gradient = currItem->fill_gradient;
		gradient.setRepeatMethod(currItem->getGradientExtend());
	}
	QList<VColorStop*> cstops = gradient.colorStops();
	StopVec.clear();
	TransVec.clear();
	Gcolors.clear();
	colorNames.clear();
	colorShades.clear();
	QTransform mpa;
	bool spotMode = false;
	if (inPattern == 0)
	{
		if (groupStackPos.count() != 0)
		{
			mpa.translate(currItem->xPos() - groupStackPos.top().x(), groupStackPos.top().y() - currItem->gYpos);
			mpa.rotate(-currItem->rotation());
		}
		else
		{
			mpa.translate(bleedDisplacementX, bleedDisplacementY);
			mpa.translate(currItem->xPos() - ActPageP->xOffset(), ActPageP->height() - (currItem->yPos() - ActPageP->yOffset()));
			mpa.rotate(-currItem->rotation());
		}
	}
	else
	{
		if (patternStackPos.count() != 0)
			mpa.translate(patternStackPos.top().x(), patternStackPos.top().y());
	}
	if (Gskew == 90)
		Gskew = 1;
	else if (Gskew == 180)
		Gskew = 0;
	else if (Gskew == 270)
		Gskew = -1;
	else if (Gskew == 360)
		Gskew = 0;
	else
		Gskew = tan(M_PI / 180.0 * Gskew);
	if (GType == 6)
	{
		mpa.translate(StartX, -StartY);
		mpa.shear(Gskew, 0);
		mpa.translate(-StartX, StartY);
	}
	else
	{
		double rotEnd = xy2Deg(EndX - StartX, EndY - StartY);
		mpa.translate(StartX, -StartY);
		mpa.rotate(-rotEnd);
		mpa.shear(-Gskew, 0);
		mpa.translate(0, -StartY * (1.0 - Gscale));
		mpa.translate(-StartX, StartY);
		mpa.scale(1, Gscale);
	}
	double lastStop = -1.0;
	double actualStop = 0.0;
	bool   isFirst = true, transparencyFound = false;
	for (uint cst = 0; cst < gradient.Stops(); ++cst)
	{
		actualStop = cstops.at(cst)->rampPoint;
		if ((actualStop == lastStop) && (!isFirst))
			continue;
		if ((isFirst) && (actualStop != 0.0))
		{
			StopVec.append(0.0);
			colorNames.append(cstops.at(cst)->name);
			colorShades.append(cstops.at(cst)->shade);
			if (cstops.at(cst)->name == CommonStrings::None)
				TransVec.append(0);
			else
				TransVec.append(cstops.at(cst)->opacity);
			Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
		}
		isFirst = false;
		StopVec.append(actualStop);
		colorNames.append(cstops.at(cst)->name);
		colorShades.append(cstops.at(cst)->shade);
		if (cstops.at(cst)->name == CommonStrings::None)
			TransVec.append(0);
		else
			TransVec.append(cstops.at(cst)->opacity);
		if (cstops.at(cst)->opacity != 1.0)
			transparencyFound = true;
		if (spotMap.contains(cstops.at(cst)->name))
		{
			if (!spotColorSet.contains(cstops.at(cst)->name))
				spotColorSet.append(cstops.at(cst)->name);
		}
		Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
		if ((cst == gradient.Stops()-1) && (actualStop < 1.0))
		{
			StopVec.append(1.0);
			colorNames.append(cstops.at(cst)->name);
			colorShades.append(cstops.at(cst)->shade);
			if (cstops.at(cst)->name == CommonStrings::None)
				TransVec.append(0);
			else
				TransVec.append(cstops.at(cst)->opacity);
			Gcolors.append(SetGradientColor(cstops.at(cst)->name, cstops.at(cst)->shade));
		}
		lastStop = actualStop;
	}
	QByteArray TRes;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
	{
		QTransform mpM;
		//#12058: cause problems with rotated objects
		//mpM.rotate(-currItem->rotation());
		if (GType == 6)
		{
			mpM.translate(StartX, -StartY);
			mpM.shear(Gskew, 0);
			mpM.translate(-StartX, StartY);
		}
		else
		{
			double rotEnd = xy2Deg(EndX - StartX, EndY - StartY);
			mpM.translate(StartX, -StartY);
			mpM.rotate(-rotEnd);
			mpM.shear(-Gskew, 0);
			mpM.translate(0, -StartY * (1.0 - Gscale));
			mpM.translate(-StartX, StartY);
			mpM.scale(1, Gscale);
		}
		PdfId patObject = writer.newObject();
		writer.startObj(patObject);
		PutDoc("<<\n/Type /Pattern\n");
		PutDoc("/PatternType 2\n");
		PutDoc("/Matrix ["+FToStr(mpM.m11())+" "+FToStr(mpM.m12())+" "+FToStr(mpM.m21())+" "+FToStr(mpM.m22())+" "+FToStr(mpM.dx())+" "+FToStr(mpM.dy())+"]\n");
		PutDoc("/Shading\n");
		PutDoc("<<\n");
		if (GType == 6)
			PutDoc("/ShadingType 2\n");
		else
			PutDoc("/ShadingType 3\n");
		PutDoc("/ColorSpace /DeviceGray\n");
		if (gradient.repeatMethod() == VGradient::none)
			PutDoc("/Extend [false false]\n");
		else
			PutDoc("/Extend [true true]\n");
		if (GType == 6)
			PutDoc("/Coords ["+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(EndX)+" "+FToStr(-EndY)+"]\n");
		else
			PutDoc("/Coords ["+FToStr(FocalX)+" "+FToStr(-FocalY)+" 0.0 "+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2)))+"]\n");
		PutDoc("/Function\n");
		PutDoc("<<\n");
		PutDoc("/FunctionType 3\n");
		PutDoc("/Domain [0 1]\n");
		if (StopVec.count() > 2)
		{
			PutDoc("/Bounds [");
			QByteArray bctx = "";
			for (int bc = 1; bc < StopVec.count() - 1; bc++)
			{
				bctx += FToStr(StopVec.at(bc))+" ";
			}
			PutDoc(bctx.trimmed()+"]\n");
		}
		else
			PutDoc("/Bounds []\n");
		QByteArray entx = "";
		PutDoc("/Functions\n");
		PutDoc("[\n");
		for (int cc = 0; cc < TransVec.count() - 1; cc++)
		{
			entx += "0 1 ";
			PutDoc("<<\n");
			PutDoc("/FunctionType 2\n");
			PutDoc("/Domain [0 1]\n");
			PutDoc("/C0 ["+FToStr(TransVec.at(cc))+"]\n");
			PutDoc("/C1 ["+FToStr(TransVec.at(cc+1))+"]\n");
			PutDoc("/N 1\n");
			PutDoc(">>\n");
		}
		PutDoc("]\n");
		PutDoc("/Encode ["+entx.trimmed()+"]\n");
		PutDoc(">>\n");
		PutDoc(">>\n");
		PutDoc(">>\n");
		writer.endObj(patObject);
		Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
		PdfId formObject = writer.newObject();
		writer.startObj(formObject);
		PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
		PutDoc("/FormType 1\n");
		PutDoc("/Group << /S /Transparency /CS /DeviceGray >>\n");
		double lw = currItem->lineWidth();
		PutDoc("/BBox ["+FToStr(-lw / 2.0)+" "+FToStr(lw / 2.0)+" "+FToStr(currItem->width()+lw)+" "+FToStr(-(currItem->height()+lw))+" ]\n");
		PutDoc("/Resources ");
		
		Pdf::ResourceDictionary dict;
		dict.Pattern = Patterns;
		writer.write(dict);

		QByteArray stre = "q\n";
		if (currItem->isLine())
			stre += "0 0 m\n" + FToStr(currItem->width()) +" 0 l\n";
		else
			stre += SetClipPath(currItem) + "h\n";
		stre += FToStr(fabs(currItem->lineWidth()))+" w\n";
		if ((forArrow) || (!stroke))
		{
			stre += "/Pattern cs\n";
			stre += "/Pattern"+Pdf::toPdf(patObject)+" scn\nf*\n";
		}
		else
		{
			stre += "/Pattern CS\n";
			stre += "/Pattern"+Pdf::toPdf(patObject)+" SCN\nS\n";
		}
		stre += "Q\n";
		if (Options.Compress)
			stre = CompressArray(stre);
		PutDoc("/Length "+Pdf::toPdf(stre.length())+"\n");
		if (Options.Compress)
			PutDoc("/Filter /FlateDecode\n");
		PutDoc(">>\nstream\n"+EncStream(stre, formObject)+"\nendstream");
		writer.endObj(formObject);
		pageData.XObjects[ResNam+Pdf::toPdf(ResCount)] = formObject;
		ResCount++;
		QByteArray GXName = ResNam+Pdf::toPdf(ResCount);
		ResCount++;
		int bmCode = stroke ? currItem->lineBlendmode() : currItem->fillBlendmode();
		Transpar[GXName] = writeGState("/SMask << /S /Luminosity /G "+Pdf::toPdf(formObject)+" 0 R >>\n/BM /" + blendMode(bmCode) + "\n");
		TRes = GXName;
	}
	PdfId patObject = writer.newObject();
	PdfId spotObject = 0;
	writer.startObj(patObject);
	PutDoc("<<\n/Type /Pattern\n");
	PutDoc("/PatternType 2\n");
	PutDoc("/Matrix ["+FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" "+FToStr(mpa.dx())+" "+FToStr(mpa.dy())+"]\n");
	PutDoc("/Shading\n");
	PutDoc("<<\n");
	if (GType == 6)
		PutDoc("/ShadingType 2\n");
	else
		PutDoc("/ShadingType 3\n");
	if (Options.UseRGB)
		PutDoc("/ColorSpace /DeviceRGB\n");
	else if (Options.isGrayscale)
		PutDoc("/ColorSpace /DeviceGray\n");
	else if ((doc.HasCMS) && (Options.UseProfiles))
		PutDoc("/ColorSpace "+ICCProfiles[Options.SolidProf].ICCArray+"\n");
	else
	{
		if ((Options.UseSpotColors) && ((spotColorSet.count() > 0) && (spotColorSet.count() < 28)))
		{
			spotObject = writer.newObject();
			PutDoc("/ColorSpace [ /DeviceN [ /Cyan /Magenta /Yellow /Black");
			for (int sc = 0; sc < spotColorSet.count(); sc++)
			{
				PutDoc("" + Pdf::toName(spotColorSet.at(sc).simplified()));
				       //" /"+spotColorSet.at(sc).simplified().replace("#", "#23").replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "#20" ));
			}
			PutDoc(" ]\n");
			PutDoc("/DeviceCMYK\n");
			PutDoc(Pdf::toPdf(spotObject)+" 0 R\n");
			PutDoc("]\n");
			spotMode = true;
		}
		else
			PutDoc("/ColorSpace /DeviceCMYK\n");
	}
	if (gradient.repeatMethod() == VGradient::none)
		PutDoc("/Extend [false false]\n");
	else
		PutDoc("/Extend [true true]\n");
	if (GType == 6)
		PutDoc("/Coords ["+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(EndX)+" "+FToStr(-EndY)+"]\n");
	else
		PutDoc("/Coords ["+FToStr(FocalX)+" "+FToStr(-FocalY)+" 0.0 "+FToStr(StartX)+" "+FToStr(-StartY)+" "+FToStr(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2)))+"]\n");
	PutDoc("/Function\n");
	PutDoc("<<\n");
	PutDoc("/FunctionType 3\n");
	PutDoc("/Domain [0 1]\n");
	if (StopVec.count() > 2)
	{
		PutDoc("/Bounds [");
		QByteArray bctx = "";
		for (int bc = 1; bc < StopVec.count() - 1; bc++)
		{
			bctx += FToStr(StopVec.at(bc))+" ";
		}
		PutDoc(bctx.trimmed()+"]\n");
	}
	else
		PutDoc("/Bounds []\n");
	QByteArray entx = "";
	PutDoc("/Functions\n");
	PutDoc("[\n");
	for (int cc = 0; cc < colorNames.count() - 1; cc++)
	{
		entx += "0 1 ";
		PutDoc("<<\n");
		PutDoc("/FunctionType 2\n");
		PutDoc("/Domain [0 1]\n");
		if (spotMode)
		{
			if (spotColorSet.contains(colorNames.at(cc)))
			{
				PutDoc("/C0 [0 0 0 0");
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == colorNames.at(cc))
						PutDoc(" "+FToStr(colorShades[cc] / 100.0));
					else
						PutDoc(" 0");
				}
			}
			else
			{
				PutDoc("/C0 [" + Pdf::toAscii(Gcolors[cc]));
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					PutDoc(" 0");
				}
			}
			PutDoc("]\n");
			if (spotColorSet.contains(colorNames.at(cc+1)))
			{
				PutDoc("/C1 [0 0 0 0");
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					if (spotColorSet.at(sc) == colorNames.at(cc+1))
						PutDoc(" "+FToStr(colorShades[cc+1] / 100.0));
					else
						PutDoc(" 0");
				}
			}
			else
			{
				PutDoc("/C1 [" + Pdf::toAscii(Gcolors[cc+1]));
				for (int sc = 0; sc < spotColorSet.count(); sc++)
				{
					PutDoc(" 0");
				}
			}
			PutDoc("]\n");
		}
		else
		{
			PutDoc("/C0 [" + Pdf::toAscii(Gcolors[cc]) + "]\n");
			PutDoc("/C1 [" + Pdf::toAscii(Gcolors[cc+1]) + "]\n");
		}
		PutDoc("/N 1\n");
		PutDoc(">>\n");
	}
	PutDoc("]\n");
	PutDoc("/Encode ["+entx.trimmed()+"]\n");
	PutDoc(">>\n");
	PutDoc(">>\n");
	PutDoc(">>\n");
	writer.endObj(patObject);
	Patterns.insert("Pattern"+Pdf::toPdf(patObject), patObject);
	if (spotMode)
	{
		QByteArray colorDesc;
		writer.startObj(spotObject);
		PutDoc("<<\n/FunctionType 4\n");
		PutDoc("/Domain [0 1 0 1 0 1 0 1");
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			PutDoc(" 0 1");
		}
		PutDoc("]\n");
		colorDesc = "{\n";
		int maxSp = spotColorSet.count() - 1;
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			double cc(0), mc(0), yc(0), kc(0);
			CMYKColorF cmykValues;
			ScColorEngine::getCMYKValues(doc.PageColors[spotColorSet.at(maxSp - sc)], &doc, cmykValues);
			cmykValues.getValues(cc, mc, yc, kc);
			if (sc == 0)
				colorDesc += "dup " + FToStr(cc) + " mul ";
			else
				colorDesc += Pdf::toPdf(sc*4 + 1) + " -1 roll dup " + FToStr(cc) + " mul ";
			colorDesc += "exch dup " + FToStr(mc) + " mul ";
			colorDesc += "exch dup " + FToStr(yc) + " mul ";
			colorDesc += "exch " + FToStr(kc) + " mul\n";
		}
		for (int sc = 0; sc < spotColorSet.count(); sc++)
		{
			colorDesc += "8 -1 roll 5 -1 roll add 7 -1 roll 5 -1 roll add 6 -1 roll 5 -1 roll add 5 -1 roll 5 -1 roll add\n";
		}
		colorDesc += "}\n";
		PutDoc("/Range [0 1 0 1 0 1 0 1]\n");
		PutDoc("/Length "+Pdf::toPdf(colorDesc.length()+1)+"\n");
		PutDoc(">>\nstream\n"+EncStream(colorDesc, spotObject)+"\nendstream");
		writer.endObj(spotObject);
	}
	QByteArray tmp;
	if (((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4)) && (transparencyFound))
		tmp += Pdf::toName(TRes) + " gs\n";
	if ((forArrow) || (!stroke))
	{
		tmp += "/Pattern cs\n";
		tmp += "/Pattern"+Pdf::toPdf(patObject)+" scn\n";
	}
	else
	{
		tmp += "/Pattern CS\n";
		tmp += "/Pattern"+Pdf::toPdf(patObject)+" SCN\n";
	}
	output = tmp;
	return true;
}

#ifdef HAVE_OSG
bool PDFLibCore::PDF_3DAnnotation(PageItem *ite, uint)
{
	QStringList lightModes;
	lightModes << "None" << "Headlamp" << "White" << "Day" << "Night" << "Hard" << "Primary";
	lightModes << "Blue" << "Red" << "Cube" << "CAD" << "Artwork";
	QStringList renderModes;
	renderModes << "Solid" << "SolidWireframe" << "Transparent" << "TransparentWireframe";
	renderModes << "BoundingBox" << "TransparentBoundingBox" << "TransparentBoundingBoxOutline";
	renderModes << "Wireframe" << "ShadedWireframe" << "HiddenWireframe" << "Vertices";
	renderModes << "ShadedVertices" << "Illustration" << "SolidOutline" << "ShadedIllustration";
	QTemporaryFile *tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_osg_XXXXXX.prc");
	tempImageFile->open();
	QString imgName = getLongPathName(tempImageFile->fileName());
	tempImageFile->close();
	PRCExporter *exprc = new PRCExporter();
	exprc->convertFile(imgName, ite->asOSGFrame());
	delete exprc;
	double x = ite->xPos() - ActPageP->xOffset();
	double y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
	double x2 = x+ite->width();
	double y2 = y-ite->height();
	PageItem_OSGFrame *osgframe = ite->asOSGFrame();
	QList<uint> viewList;
	PdfId viewObj = 0;
	QHash<QString, PageItem_OSGFrame::viewDefinition>::iterator itv;
	for (itv = osgframe->viewMap.begin(); itv != osgframe->viewMap.end(); ++itv)
	{
		PdfId viewObjL = writer.newObject();
		viewList.append(viewObjL);
		if (osgframe->currentView == itv.key())
			viewObj = viewObjL;
		writer.startObj(viewObjL);
		PutDoc("<<\n/Type /3DView\n");
		PutDoc("/MS /M\n");
		PutDoc("/C2W ["+osgframe->getPDFMatrix(itv.key()).toLatin1()+" ]\n");
		PutDoc("/LS << /Subtype /" + lightModes[itv.value().illumination].toLatin1() + " >>\n");
		PutDoc("/RM\n<<\n");
		PutDoc("/Subtype /" + renderModes[itv.value().rendermode].toLatin1() + "\n");
		PutDoc("/AC [/DeviceRGB " + FToStr(itv.value().colorAC.redF()) + " " + FToStr(itv.value().colorAC.greenF()) + " " + FToStr(itv.value().colorAC.blueF()) + "]\n");
		PutDoc("/FC [/DeviceRGB " + FToStr(itv.value().colorFC.redF()) + " " + FToStr(itv.value().colorFC.greenF()) + " " + FToStr(itv.value().colorFC.blueF()) + "]\n");
		PutDoc("/O " + FToStr(1.0 - itv.value().addedTransparency) + "\n");
		PutDoc(">>\n");
		PutDoc("/P << /FOV "+FToStr(itv.value().angleFOV)+" /PS /Min /Subtype /P >>\n");
		PutDoc("/CO "+FToStr(osgframe->distanceToObj)+"\n");
		if (ite->fillColor() != CommonStrings::None)
		{
			PutDoc("/BG << /Type /3DBG\n");
			PutDoc("/C [ "+SetColor(ite->fillColor(), ite->fillShade())+" ]\n>>\n");
		}
		PutDoc("/XN ("+Pdf::toAscii(itv.key())+")\n");
		PutDoc("/IN ("+Pdf::toAscii(itv.key())+")\n");
		PutDoc(">>");
		writer.endObj(viewObjL);
	}
	PdfId appearanceObj = writer.newObject();
	writer.startObj(appearanceObj);
	PutDoc("<<\n/Type /3D\n");
	PutDoc("/Subtype /PRC\n");
	PutDoc("/VA [");
	for (int vl = 0; vl < viewList.count(); vl++)
	{
		PutDoc(Pdf::toPdf(viewList.at(vl))+" 0 R ");
	}
	PutDoc("]\n");
	QByteArray dataP;
	loadRawBytes(imgName, dataP);
	if ((Options.CompressMethod != PDFOptions::Compression_None) && Options.Compress)
	{
		QByteArray compData = CompressArray(dataP);
		if (compData.size() > 0)
		{
			PutDoc("/Filter /FlateDecode\n");
			dataP = compData;
		}
	}
	PutDoc("/Length "+Pdf::toPdf(dataP.size()+1)+"\n");
	PutDoc(">>\nstream\n");
	EncodeArrayToStream(dataP, appearanceObj);
	PutDoc("\nendstream");
	writer.endObj(appearanceObj);
	PdfId appearanceObj1 = writer.newObject();
	if (!ite->Pfile.isEmpty())
	{
		PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true);
		QByteArray cc = Pdf::toPdf(ite->pixm.width())+" 0 0 "+Pdf::toPdf(ite->pixm.height())+" 0 0 cm\n";
		cc += Pdf::toName(ResNam+"I"+Pdf::toPdf(ResCount-1)) + " Do";
		PDF_xForm(appearanceObj1, ite->pixm.width(), ite->pixm.height(), cc);
	}
	PdfId annotationObj = writer.newObject();
	writer.startObj(annotationObj);
	pageData.AObjects.append(annotationObj);
	PutDoc("<<\n/Type /Annot\n");
	PutDoc("/Subtype /3D\n");
	PutDoc("/F 4\n");
	PutDoc("/3DD "+Pdf::toPdf(appearanceObj)+" 0 R\n");
	PutDoc("/3DV "+Pdf::toPdf(viewObj)+" 0 R\n");
	PutDoc("/3DA <<\n/A /PV\n/TB true\n/NP true\n>>\n");
	QByteArray onState = Pdf::toName(ite->itemName().replace(".", "_" ));
	PutDoc("/AS "+onState+"\n");
	PutDoc("/AP << /N <<\n" + onState + " " + Pdf::toPdf(appearanceObj1)+" 0 R >> >>\n");
	PutDoc("/Rect [ "+FToStr(x+bleedDisplacementX)+" "+FToStr(y2+bleedDisplacementY)+" "+FToStr(x2+bleedDisplacementX)+" "+FToStr(y+bleedDisplacementY)+" ]\n");
	PutDoc(">>");
	writer.endObj(annotationObj);
	delete tempImageFile;
	return true;
}
#endif

void PDFLibCore::PDF_RadioButtons()
{
	QHash<PageItem*, QList<PageItem*> > rbMap;
	for (int a = 0; a < pageData.radioButtonList.count(); a++)
	{
		PageItem* pa = pageData.radioButtonList[a]->Parent;
		if (rbMap.contains(pa))
			rbMap[pa].append(pageData.radioButtonList[a]);
		else
		{
			QList<PageItem*> aList;
			aList.append(pageData.radioButtonList[a]);
			rbMap.insert(pa, aList);
		}
	}
	QHash<PageItem*, QList<PageItem*> >::Iterator it;
	for (it = rbMap.begin(); it != rbMap.end(); ++it)
	{
		QList<PageItem*> bList = it.value();
		QList<PdfId> kidsList;
		PdfId parentObject = writer.newObject();
		QByteArray onState = "";
		QByteArray anTitle;
		if (it.key() == 0)
			anTitle = "Page" + Pdf::toPdf(ActPageP->pageNr() + 1);
		else
			anTitle = Pdf::toPdfDocEncoding(it.key()->itemName().replace(".", "_" ));
		for (int a = 0; a < bList.count(); a++)
		{
			PdfId kid = PDF_RadioButton(bList[a], parentObject, anTitle);
			kidsList.append(kid);
			if (bList[a]->annotation().IsChk())
				onState = Pdf::toName(bList[a]->itemName().replace(".", "_" ));
		}
		writer.startObj(parentObject);
		pageData.AObjects.append(parentObject);
		pageData.FormObjects.append(parentObject);
		PutDoc("<<\n/Type /Annot\n");
		PutDoc("/Subtype /Widget\n");
		PutDoc("/FT /Btn\n");
		PutDoc("/T " + EncString(anTitle, parentObject) + "\n");
		PutDoc("/Contents " + EncStringUTF16(anTitle, parentObject) + "\n");
		PutDoc("/Ff "+Pdf::toPdf(Annotation::Flag_Radio | Annotation::Flag_NoToggleToOff)+"\n");
		PutDoc("/V "+onState+"\n");
		PutDoc("/DV "+onState+"\n");
		PutDoc("/Kids\n[\n");
		for (int a = 0; a < kidsList.count(); a++)
		{
			PutDoc(Pdf::toPdf(kidsList[a]) + " 0 R\n");
		}
		PutDoc("]\n");
		PutDoc("/Rect [0 0 0 0]\n");
		PutDoc(">>");
		writer.endObj(parentObject);
	}
}

PdfId PDFLibCore::PDF_RadioButton(PageItem* ite, PdfId parent, QString parentName)
{
	QMap<int, QByteArray> ind2PDFabr;
	static const QByteArray bifonts[] = {"/Courier", "/Courier-Bold", "/Courier-Oblique", "/Courier-BoldOblique",
												"/Helvetica", "/Helvetica-Bold", "/Helvetica-Oblique", "/Helvetica-BoldOblique",
												"/Times-Roman", "/Times-Bold", "/Times-Italic", "/Times-BoldItalic",
												"/ZapfDingbats", "/Symbol"};
	static const size_t ar = sizeof(bifonts) / sizeof(*bifonts);
	for (uint a = 0; a < ar; ++a)
		ind2PDFabr[a] = bifonts[a];
	double x = ite->xPos() - ActPageP->xOffset();
	double y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
	double x2 = x+ite->width();
	double y2 = y-ite->height();
	QByteArray cc;
	PdfId annotationObj = writer.newObject();
	PdfId actionObj = 0;
	if ((ite->annotation().Type() > 1) && ((ite->annotation().ActionType() == Annotation::Action_JavaScript) || (ite->annotation().AAact())) && (!ite->annotation().Action().isEmpty()))
		actionObj = WritePDFString(ite->annotation().Action());
	PdfId AActionObj = writeActions(ite->annotation(), annotationObj);
	writer.startObj(annotationObj);
	pageData.AObjects.append(annotationObj);
	PutDoc("<<\n/Type /Annot\n");
	PutDoc("/Subtype /Widget\n");
	PutDoc("/Parent " + Pdf::toPdf(parent) + " 0 R\n");
	PutDoc("/Contents " + EncStringUTF16(parentName, annotationObj) + "\n");
	if (!ite->annotation().ToolTip().isEmpty())
		PutDoc("/TU " + EncStringUTF16(ite->annotation().ToolTip(), annotationObj) + "\n");
	PutDoc("/F ");
	QByteArray mm[] = {"4", "2", "0", "32"};
	PutDoc(mm[ite->annotation().Vis()]);
	PutDoc("\n");
	QByteArray cnx = Pdf::toName(StdFonts["/ZapfDingbats"]);
	cnx += " "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf";
	if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
		cnx += " "+ putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
	if (ite->fillColor() != CommonStrings::None)
		cnx += " "+ putColor(ite->fillColor(), ite->fillShade(), false);
	PutDoc("/DA " + EncString(cnx, annotationObj) + "\n");
	int flg = ite->annotation().Flag();
	if (Options.Version == PDFOptions::PDFVersion_13)
		flg = flg & 522247;
	PutDoc("/Ff "+Pdf::toPdf(flg)+"\n");
	PutDoc("/FT /Btn\n");
	PutDoc("/BS << /Type /Border /W ");
	PutDoc(ite->annotation().borderColor() != CommonStrings::None ? Pdf::toPdf(ite->annotation().Bwid()) : "0");
	PutDoc(" /S /");
	const QByteArray xb[] = {"S", "D", "U", "B", "I"};
	PutDoc(xb[ite->annotation().Bsty()]);
	PutDoc(" >>\n");
	PutDoc("/MK << ");
	PutDoc("/BG [ 1 1 1 ] ");
	if (ite->annotation().borderColor() != CommonStrings::None)
		PutDoc("/BC [ "+SetColor(ite->annotation().borderColor(), 100)+" ] ");
	if (ite->rotation() != 0)
		PutDoc("/R "+Pdf::toPdf((abs(static_cast<int>(ite->rotation())) / 90)*90)+" ");
	PutDoc(">>\n");
	QByteArray onState = Pdf::toName(ite->itemName().replace(".", "_" ));
	if (ite->annotation().IsChk())
		PutDoc("/AS "+onState+"\n");
	else
		PutDoc("/AS /Off\n");
	PdfId appearanceObj1 = writer.newObject();
	PdfId appearanceObj2 = writer.newObject();
	PutDoc("/AP << /N <<\n");
	PutDoc(onState + " " + Pdf::toObjRef(appearanceObj1)+"\n");
	PutDoc("/Off " + Pdf::toObjRef(appearanceObj2)+"\n");
	PutDoc(">> >>\n");
	if ((ite->annotation().ActionType() != Annotation::Action_None) || (ite->annotation().AAact()))
	{
		if (ite->annotation().ActionType() == Annotation::Action_JavaScript)
		{
			if (!ite->annotation().Action().isEmpty())
			{
				PutDoc("/A << /Type /Action /S /JavaScript /JS " + Pdf::toPdf(actionObj) + " 0 R >>\n");
			}
		}
		if (ite->annotation().AAact())
		{
			if (!ite->annotation().Action().isEmpty())
			{
				PutDoc("/A << /Type /Action /S /JavaScript /JS " + Pdf::toPdf(actionObj) + " 0 R >>\n");
			}
			PutDoc("/AA " + Pdf::toPdf(AActionObj) + " 0 R\n");
		}
	}
	switch (((abs(static_cast<int>(ite->rotation())) / 90)*90))
	{
		case 0:
			break;
		case 90:
			x = ite->xPos() - ActPageP->xOffset();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			x2 = x + ite->height();
			y = y2 + ite->width();
			break;
		case 180:
			x = ite->xPos() - ActPageP->xOffset() - ite->width();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			x2 = ite->xPos() - ActPageP->xOffset();
			y = y2 + ite->height();
			break;
		case 270:
			x = ite->xPos() - ActPageP->xOffset() - ite->height();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset()) - ite->width();
			x2 = ite->xPos() - ActPageP->xOffset();
			y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			break;
	}
	PutDoc("/Rect [ "+FToStr(x+bleedDisplacementX)+" "+FToStr(y2+bleedDisplacementY)+" "+FToStr(x2+bleedDisplacementX)+" "+FToStr(y+bleedDisplacementY)+" ]\n");
	PutDoc(">>");
	writer.endObj(annotationObj);
	cc = createBorderAppearance(ite);
	if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
		cc += putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
	else if (ite->fillColor() != CommonStrings::None)
		cc += putColor(ite->fillColor(), ite->fillShade(), true);
	QPainterPath clp2;
	double siz = qMin(ite->width(), ite->height()) * 0.4;
	QRectF sizR(0, 0, siz, siz);
	sizR.moveCenter(QPointF(ite->width() / 2.0, ite->height() / 2.0));
	clp2.addEllipse(sizR);
	FPointArray clpArr2;
	clpArr2.fromQPainterPath(clp2);
	clpArr2.translate(0, -ite->height());
	cc += SetClipPathArray(&clpArr2, true);
	cc += "h\nf\nQ\n";
	PDF_xForm(appearanceObj1, ite->width(), ite->height(), cc);
	cc = createBorderAppearance(ite);
	PDF_xForm(appearanceObj2, ite->width(), ite->height(), cc);
	return annotationObj;
}

bool PDFLibCore::PDF_Annotation(PageItem *ite, uint PNr)
{
	ScImage img;
	ScImage img2;
	ScImage img3;
	QMap<int, QByteArray> ind2PDFabr;
	static const QByteArray bifonts[] = {"/Courier", "/Courier-Bold", "/Courier-Oblique", "/Courier-BoldOblique",
												"/Helvetica", "/Helvetica-Bold", "/Helvetica-Oblique", "/Helvetica-BoldOblique",
												"/Times-Roman", "/Times-Bold", "/Times-Italic", "/Times-BoldItalic",
												"/ZapfDingbats", "/Symbol"};
	static const size_t ar = sizeof(bifonts) / sizeof(*bifonts);
	for (uint a = 0; a < ar; ++a)
		ind2PDFabr[a] = bifonts[a];
	double x = ite->xPos() - ActPageP->xOffset();
	double y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
	double x2 = x+ite->width();
	double y2 = y-ite->height();
	QString bmUtf16("");
	if (!((ite->itemText.length() == 1) && (ite->itemText.text(0, 1) == QChar(13))))
	{
		// #6823 EncStringUTF16() perform the string encoding by its own
		// via EncodeUTF16() so bmUtf16 must not encoded before
		for (uint d = 0; d < static_cast<uint>(ite->itemText.length()); ++d)
		{
			QString cc = ite->itemText.text(d, 1);
			bmUtf16 += (cc == QChar(13) ? QChar(10) : cc);
		}
	}
	QByteArray anTitle = Pdf::toPdfDocEncoding(ite->itemName().replace(".", "_" ));
	QStringList bmstUtf16 = bmUtf16.split(QChar(10), QString::SkipEmptyParts);
	const QByteArray m[] = {"4", "5", "F", "l", "H", "n"};
	QByteArray ct(m[ite->annotation().ChkStil()]);
	PdfId annotationObj = writer.newObject();
	PdfId appearanceObj = 0;
	PdfId appearanceObj1 = 0;
	PdfId appearanceObj2 = 0;
	PdfId icon1Obj = 0;
	PdfId icon2Obj = 0;
	PdfId icon3Obj = 0;
	PdfId actionObj = 0;
	if ((ite->annotation().Type() > 1) && ((ite->annotation().ActionType() == Annotation::Action_JavaScript) || (ite->annotation().AAact())) && (!ite->annotation().Action().isEmpty()))
		actionObj = WritePDFString(ite->annotation().Action());
	PdfId AActionObj = writeActions(ite->annotation(), annotationObj);
	writer.startObj(annotationObj);
	pageData.AObjects.append(annotationObj);
	QByteArray onState = Pdf::toName(ite->itemName().replace(".", "_" ));
	PutDoc("<<\n/Type /Annot\n");
	switch (ite->annotation().Type())
	{
		case 0:
		case Annotation::Text:
			PutDoc("/Subtype /Text\n");
			PutDoc("/Contents " + EncStringUTF16(bmUtf16, annotationObj) + "\n");
			PutDoc("/Open " );
			if (ite->annotation().IsAnOpen())
				PutDoc("true\n");
			else
				PutDoc("false\n");
			PutDoc("/Name /");
			switch (ite->annotation().Icon())
			{
				case Annotation::Icon_Note:
					PutDoc("Note\n");
					break;
				case Annotation::Icon_Comment:
					PutDoc("Comment\n");
					break;
				case Annotation::Icon_Key:
					PutDoc("Key\n");
					break;
				case Annotation::Icon_Help:
					PutDoc("Help\n");
					break;
				case Annotation::Icon_NewParagraph:
					PutDoc("NewParagraph\n");
					break;
				case Annotation::Icon_Paragraph:
					PutDoc("Paragraph\n");
					break;
				case Annotation::Icon_Insert:
					PutDoc("Insert\n");
					break;
				case Annotation::Icon_Cross:
					PutDoc("Cross\n");
					break;
				case Annotation::Icon_Circle:
					PutDoc("Circle\n");
					break;
				default:
					PutDoc("Note\n");
					break;
			}
			break;
		case 1:
		case Annotation::Link:
			PutDoc("/Subtype /Link\n");
			if (ite->annotation().ActionType() == Annotation::Action_GoTo)
			{
				PutDoc("/Dest /"+NDnam+Pdf::toPdf(NDnum)+"\n");
				PdfDest de;
				de.Name = NDnam+Pdf::toPdf(NDnum);
				de.PageNr = ite->annotation().Ziel();
				de.Act = ite->annotation().Action();
				NamedDest.append(de);
				NDnum++;
			}
			if (ite->annotation().ActionType() == Annotation::Action_GoToR_FileRel)
			{
				PutDoc("/A << /Type /Action /S /GoToR\n/F " + EncString(Pdf::toPdfDocEncoding(Path2Relative(ite->annotation().Extern(), baseDir)), annotationObj) + "\n");
				PutDoc("/D ["+Pdf::toPdf(ite->annotation().Ziel())+" /XYZ "+Pdf::toPdfDocEncoding(ite->annotation().Action())+"]\n>>\n");
			}
			if (ite->annotation().ActionType() == Annotation::Action_URI)
				PutDoc("/A << /Type /Action /S /URI\n/URI " + EncString(Pdf::toPdfDocEncoding(ite->annotation().Extern()), annotationObj) + "\n>>\n");
			if (ite->annotation().ActionType() == Annotation::Action_GoToR_FileAbs)
			{
				PutDoc("/A << /Type /Action /S /GoToR\n/F " + EncString(Pdf::toPdfDocEncoding(ite->annotation().Extern()), annotationObj) + "\n");
				PutDoc("/D ["+Pdf::toPdf(ite->annotation().Ziel())+" /XYZ " + Pdf::toPdfDocEncoding(ite->annotation().Action())+"]\n>>\n");
			}
			break;
		case Annotation::Button:
		case Annotation::Textfield:
		case Annotation::Checkbox:
		case Annotation::Combobox:
		case Annotation::Listbox:
			pageData.FormObjects.append(annotationObj);
			PutDoc("/Subtype /Widget\n");
			PutDoc("/T " + EncString(anTitle, annotationObj) + "\n");
			if (!ite->annotation().ToolTip().isEmpty())
				PutDoc("/TU " + EncStringUTF16(ite->annotation().ToolTip(), annotationObj) + "\n");
			PutDoc("/F ");
			QByteArray mm[] = {"4", "2", "0", "32"};
			PutDoc(mm[ite->annotation().Vis()]);
			PutDoc("\n");
			PutDoc("/BS << /Type /Border /W ");
			PutDoc(ite->annotation().borderColor() != CommonStrings::None ? Pdf::toPdf(ite->annotation().Bwid()) : "0");
			PutDoc(" /S /");
			const QByteArray x[] = {"S", "D", "U", "B", "I"};
			PutDoc(x[ite->annotation().Bsty()]);
			PutDoc(" >>\n");
			QByteArray cnx;
			if (ite->annotation().Type() == Annotation::Checkbox)
				cnx += Pdf::toName(StdFonts["/ZapfDingbats"]);
			else
			{
				if (Options.Version < PDFOptions::PDFVersion_14)
					cnx += Pdf::toName(StdFonts[ind2PDFabr[ite->annotation().Font()]]);
				else
					cnx += UsedFontsF[ite->itemText.defaultStyle().charStyle().font().replacementName()].name;
//					cnx += UsedFontsP[ite->itemText.defaultStyle().charStyle().font().replacementName()]+"Form";
			}
			cnx += " "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf";
			if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
				cnx += " "+ putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
			if (ite->fillColor() != CommonStrings::None)
				cnx += " "+ putColor(ite->fillColor(), ite->fillShade(), false);
			PutDoc("/DA " + EncString(cnx, annotationObj) + "\n");
			int flg = ite->annotation().Flag();
			if (Options.Version == PDFOptions::PDFVersion_13)
				flg = flg & 522247;
			PutDoc("/Ff "+Pdf::toPdf(flg)+"\n");
			QByteArray xs[] = {"N", "I", "O", "P"};
			switch (ite->annotation().Type())
			{
				case Annotation::Button:
					PutDoc("/FT /Btn\n");
					PutDoc("/H /");
					PutDoc(xs[ite->annotation().Feed()]);
					PutDoc("\n");
					PutDoc("/Q 0\n");
					appearanceObj = writer.newObject();
					PutDoc("/AP << /N "+Pdf::toPdf(appearanceObj)+" 0 R >>\n");
					break;
				case Annotation::Textfield:
					PutDoc("/FT /Tx\n");
					PutDoc("/V " + EncStringUTF16(bmUtf16, annotationObj) + "\n");
					PutDoc("/DV "+ EncStringUTF16(bmUtf16, annotationObj) + "\n");
					PutDoc("/Q "+Pdf::toPdf(qMin(ite->itemText.defaultStyle().alignment(), ParagraphStyle::Rightaligned))+"\n");
					appearanceObj = writer.newObject();
					PutDoc("/AP << /N "+Pdf::toPdf(appearanceObj)+" 0 R >>\n");
					if (ite->annotation().MaxChar() != -1)
						PutDoc("/MaxLen "+Pdf::toPdf(ite->annotation().MaxChar())+"\n");
					break;
				case Annotation::Checkbox:
					PutDoc("/FT /Btn\n");
					if (ite->annotation().IsChk())
						PutDoc("/V "+onState+"\n/DV "+onState+"\n/AS "+onState+"\n");
					else
						PutDoc("/V /Off\n/DV /Off\n/AS /Off\n");
					appearanceObj1 = writer.newObject();
					appearanceObj2 = writer.newObject();
					PutDoc("/AP << /N <<\n");
					PutDoc(onState + " " + Pdf::toPdf(appearanceObj1)+" 0 R\n");
					PutDoc("/Off " + Pdf::toPdf(appearanceObj2)+" 0 R\n");
					PutDoc(">> >>\n");
					break;
				case Annotation::Combobox:
				case Annotation::Listbox:
					cnx = EncStringUTF16(bmstUtf16.count() > 0 ? bmstUtf16[0] : "", annotationObj);
					PutDoc("/FT /Ch\n");
					PutDoc("/V " + cnx + "\n");
					PutDoc("/DV " + cnx + "\n");
					PutDoc("/Opt [ ");
					for (int bmc = 0; bmc < bmstUtf16.count(); ++bmc)
						PutDoc(EncStringUTF16(bmstUtf16[bmc], annotationObj)+"\n");
					PutDoc("]\n");
					appearanceObj = writer.newObject();
					PutDoc("/AP << /N "+Pdf::toPdf(appearanceObj)+" 0 R >>\n");
					break;
			}
			PutDoc("/MK << ");
			if ((ite->annotation().Type() == Annotation::Combobox) || (ite->annotation().Type() == Annotation::Listbox))
			{
				PutDoc("/BG [ 1 1 1 ] ");
				if (ite->annotation().borderColor() != CommonStrings::None)
					PutDoc("/BC [ "+SetColor(ite->annotation().borderColor(), 100)+" ] ");
			}
			else
			{
				if (ite->fillColor() != CommonStrings::None)
					PutDoc("/BG [ "+SetColor(ite->fillColor(), ite->fillShade())+" ] ");
				if (ite->annotation().borderColor() != CommonStrings::None)
					PutDoc("/BC [ "+SetColor(ite->annotation().borderColor(), 100)+" ] ");
			}
			PdfId IconOb = 0;
			switch (ite->annotation().Type())
			{
				case Annotation::Button:
					PutDoc("/CA " + EncStringUTF16(bmUtf16, annotationObj) + " ");
					if (!ite->annotation().RollOver().isEmpty())
						PutDoc("/RC " + EncStringUTF16(ite->annotation().RollOver(), annotationObj) + " ");
					if (!ite->annotation().Down().isEmpty())
						PutDoc("/AC " + EncStringUTF16(ite->annotation().Down(), annotationObj) + " ");
					if (ite->annotation().UseIcons())
					{
						if (!ite->Pfile.isEmpty())
						{
							IconOb += ite->pixm.hasAlpha() ? 3 : 2;
							icon1Obj = writer.newObject();
							PutDoc("/I "+Pdf::toPdf(icon1Obj)+" 0 R ");
						}
						if (!ite->Pfile2.isEmpty())
						{
							CMSettings cms(ite->doc(), "", Intent_Perceptual);
							cms.allowColorManagement(false);
							img.loadPicture(ite->Pfile2, 1, cms, ScImage::RGBData, 72);
							QByteArray im;
							img3.getAlpha(ite->Pfile2, 1, im, true, false);
							IconOb += !im.isEmpty() ? 3 : 2;
							im.resize(0);
							icon2Obj = writer.newObject();
							PutDoc("/IX "+Pdf::toPdf(icon2Obj)+" 0 R ");
						}
						if (!ite->Pfile3.isEmpty())
						{
							CMSettings cms(ite->doc(), "", Intent_Perceptual);
							cms.allowColorManagement(false);
							img2.loadPicture(ite->Pfile3, 1, cms, ScImage::RGBData, 72);
							QByteArray im;
							img3.getAlpha(ite->Pfile3, 1, im, true, false);
							IconOb += !im.isEmpty() ? 3 : 2;
							im.resize(0);
							icon3Obj = writer.newObject();
							PutDoc("/RI "+Pdf::toObjRef(icon3Obj)+" ");
						}
						PutDoc("/TP "+Pdf::toPdf(ite->annotation().IPlace())+" ");
						PutDoc("/IF << /SW /");
						QByteArray x[] = {"A", "S", "B", "N"};
						PutDoc(x[ite->annotation().ScaleW()]);
						PutDoc(" /S /");
						PutDoc(ite->imageXScale() != ite->imageYScale() ? "A" : "P");
						PutDoc(" /A [ ");
						if ((ite->width()/ite->imageXScale() - ite->pixm.width()) != 0)
						{
							if (ite->annotation().ScaleW() == 3)
								PutDoc(FToStr(qMax(ite->imageXOffset() / (ite->width()/ite->imageXScale() - ite->pixm.width()), 0.01))+" ");
							else
								PutDoc("0.5 ");
						}
						else
							PutDoc("0 ");
						if ((ite->height()/ite->imageYScale() - ite->pixm.height()) != 0)
						{
							if (ite->annotation().ScaleW() == 3)
								PutDoc(FToStr(1.0 - qMax(ite->imageYOffset() / (ite->height()/ite->imageYScale() - ite->pixm.height()), 0.01)));
							else
								PutDoc("0.5");
						}
						else
							PutDoc("0");
						PutDoc(" ] >> ");
					}
					break;
				case Annotation::Textfield:
				case Annotation::Combobox:
				case Annotation::Listbox:
					break;
				case Annotation::Checkbox:
					PutDoc("/CA " + EncString(ct, annotationObj) + " ");
					break;
			}
			if (ite->rotation() != 0)
				PutDoc("/R "+Pdf::toPdf((abs(static_cast<int>(ite->rotation())) / 90)*90)+" ");
			PutDoc(">>\n");
			if ((ite->annotation().ActionType() != Annotation::Action_None) || (ite->annotation().AAact()))
			{
				if (ite->annotation().ActionType() == Annotation::Action_GoToR_FileRel)
				{
					PutDoc("/A << /Type /Action /S /GoToR\n/F " + EncString(Pdf::toPdfDocEncoding(Path2Relative(ite->annotation().Extern(), baseDir)), annotationObj) + "\n");
					PutDoc("/D ["+Pdf::toPdf(ite->annotation().Ziel())+" /XYZ "+ Pdf::toPdfDocEncoding(ite->annotation().Action())+"]\n>>\n");
				}
				if (ite->annotation().ActionType() == Annotation::Action_GoToR_FileAbs)
				{
					PutDoc("/A << /Type /Action /S /GoToR\n/F " + EncString(Pdf::toPdfDocEncoding(ite->annotation().Extern()), annotationObj) + "\n");
					PutDoc("/D ["+Pdf::toPdf(ite->annotation().Ziel())+" /XYZ "+ Pdf::toPdfDocEncoding(ite->annotation().Action())+"]\n>>\n");
				}
				if (ite->annotation().ActionType() == Annotation::Action_ImportData)
					PutDoc("/A << /Type /Action /S /ImportData\n/F " + EncString(Pdf::toPdfDocEncoding(ite->annotation().Action()), annotationObj) + " >>\n");
				if (ite->annotation().ActionType() == Annotation::Action_ResetForm)
					PutDoc("/A << /Type /Action /S /ResetForm >>\n");
				if (ite->annotation().ActionType() == Annotation::Action_SubmitForm)
				{
					PutDoc("/A << /Type /Action /S /SubmitForm\n/F << /FS /URL /F " + EncString(Pdf::toPdfDocEncoding(ite->annotation().Action()), annotationObj) + " >>\n");
//					if (ite->annotation().HTML())
//						PutDoc("/Flags 4");
					switch (ite->annotation().HTML())
					{
					case 1:
					  // HTML
					  PutDoc("/Flags 4"); // bit 3 on
					  break;
					case 2:
					  // XFDF
					  PutDoc("/Flags 32"); // bit 6 on
					  break;
					case 3:
					  // PDF
					  PutDoc("/Flags 256"); // bit 9 on
					  break;
					case 0:
					default:
					  // FDF
					  // bit 3 off
					  break;
					}
					PutDoc(">>\n");
				}
				if (ite->annotation().ActionType() == Annotation::Action_JavaScript)
				{
					if (!ite->annotation().Action().isEmpty())
					{
						PutDoc("/A << /Type /Action /S /JavaScript /JS " + Pdf::toPdf(actionObj) + " 0 R >>\n");
					}
				}
				if (ite->annotation().AAact())
				{
					if (!ite->annotation().Action().isEmpty())
					{
						PutDoc("/A << /Type /Action /S /JavaScript /JS " + Pdf::toPdf(actionObj) + " 0 R >>\n");
					}
					PutDoc("/AA " + Pdf::toPdf(AActionObj) + " 0 R\n");
				}
				if (ite->annotation().ActionType() == Annotation::Action_GoTo)
				{
					PutDoc("/A << /Type /Action /S /GoTo /D /" + NDnam + Pdf::toPdf(NDnum) + " >>\n");
					PdfDest de;
					de.Name = NDnam+Pdf::toPdf(NDnum);
					de.PageNr = ite->annotation().Ziel();
					de.Act = ite->annotation().Action();
					NamedDest.append(de);
					NDnum++;
				}
				if (ite->annotation().ActionType() == Annotation::Action_Named)
				{
					if (!ite->annotation().Action().isEmpty())
					{
						PutDoc("/A << /Type /Action /S /Named /N /" + Pdf::toPdfDocEncoding(ite->annotation().Action()) + " >>\n");
					}
				}
			}
			break;
		}
	if ((ite->annotation().Type() < 2) || ((ite->annotation().Type() > Annotation::Listbox) && (ite->annotation().Type() < Annotation::Annot3D)))
		PutDoc("/Border [ 0 0 0 ]\n");
	switch (((abs(static_cast<int>(ite->rotation())) / 90)*90))
	{
		case 0:
			break;
		case 90:
			x = ite->xPos() - ActPageP->xOffset();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			x2 = x + ite->height();
			y = y2 + ite->width();
			break;
		case 180:
			x = ite->xPos() - ActPageP->xOffset() - ite->width();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			x2 = ite->xPos() - ActPageP->xOffset();
			y = y2 + ite->height();
			break;
		case 270:
			x = ite->xPos() - ActPageP->xOffset() - ite->height();
			y2 = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset()) - ite->width();
			x2 = ite->xPos() - ActPageP->xOffset();
			y = ActPageP->height() - (ite->yPos()  - ActPageP->yOffset());
			break;
	}
	PutDoc("/Rect [ "+FToStr(x+bleedDisplacementX)+" "+FToStr(y2+bleedDisplacementY)+" "+FToStr(x2+bleedDisplacementX)+" "+FToStr(y+bleedDisplacementY)+" ]\n");
	PutDoc(">>");
	writer.endObj(annotationObj);
	// write icons
	if ((ite->annotation().Type() == Annotation::Button) && (ite->annotation().UseIcons()))
	{
		if (!ite->Pfile.isEmpty())
		{
			if (!PDF_Image(ite, ite->Pfile, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true))
				return false;
			QByteArray cc = Pdf::toPdf(ite->pixm.width())+" 0 0 "+Pdf::toPdf(ite->pixm.height())+" 0 0 cm\n";
			cc += Pdf::toName(ResNam+"I"+Pdf::toPdf(ResCount-1)) + " Do";
			PDF_xForm(icon1Obj, ite->pixm.width(), ite->pixm.height(), cc);
		}
		if (!ite->Pfile2.isEmpty())
		{
			if (!PDF_Image(ite, ite->Pfile2, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true))
				return false;
			QByteArray cc = Pdf::toPdf(img.width())+" 0 0 "+Pdf::toPdf(img.height())+" 0 0 cm\n";
			cc += Pdf::toName(ResNam+"I"+Pdf::toPdf(ResCount-1)) + " Do";
			PDF_xForm(icon2Obj, img.width(), img.height(), cc);
		}
		if (!ite->Pfile3.isEmpty())
		{
			if (!PDF_Image(ite, ite->Pfile3, ite->imageXScale(), ite->imageYScale(), ite->imageXOffset(), -ite->imageYOffset(), true))
				return false;
			QByteArray cc = Pdf::toPdf(img2.width())+" 0 0 "+Pdf::toPdf(img2.height())+" 0 0 cm\n";
			cc += Pdf::toName(ResNam+"I"+Pdf::toPdf(ResCount-1)) + " Do";
			PDF_xForm(icon3Obj, img2.width(), img2.height(), cc);
		}
	}
	// write Appearance?
	if (ite->annotation().Type() == Annotation::Button)
	{
		QByteArray cc;
		cc += "q 1 g\n";
		cc += "0 0 "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += createBorderAppearance(ite);
		cc += "BT\n";
		if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
			cc += putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
		if (Options.Version < PDFOptions::PDFVersion_14)
			cc += "/"+StdFonts[ind2PDFabr[ite->annotation().Font()]];
		else
			cc += UsedFontsF[ite->itemText.defaultStyle().charStyle().font().replacementName()].name;
//			cc += UsedFontsP[ite->itemText.defaultStyle().charStyle().font().replacementName()]+"Form";
		cc += " "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf\n";
		cc += "1 0 0 1 0 0 Tm\n0 0 Td\n";
		if (bmstUtf16.count() > 0)
			cc += EncStringUTF16(bmstUtf16[0], annotationObj);
		cc += " Tj\nET\n";
		cc += "Q\n";
		PDF_xForm(appearanceObj, ite->width(), ite->height(), cc);
	}
	if (ite->annotation().Type() == Annotation::Textfield)
	{
		QByteArray cc;
		if (ite->fillColor() != CommonStrings::None)
			cc += putColor(ite->fillColor(), ite->fillShade(), false);
		cc += FToStr(x)+" "+FToStr(y2)+" "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += createBorderAppearance(ite);
		cc += "/Tx BMC\nBT\n";
		if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
			cc += putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
		if (Options.Version < PDFOptions::PDFVersion_14)
			cc += "/"+StdFonts[ind2PDFabr[ite->annotation().Font()]];
		else
			cc += UsedFontsF[ite->itemText.defaultStyle().charStyle().font().replacementName()].name;
//			cc += UsedFontsP[ite->itemText.defaultStyle().charStyle().font().replacementName()]+"Form";
		cc += " "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf\n";
		if (bmstUtf16.count() > 1)
		{
			cc += "1 0 0 1 0 0 Tm\n0 0 Td\n";
			for (int mz = 0; mz < bmstUtf16.count(); ++mz)
			{
				cc += EncStringUTF16(bmstUtf16[mz], annotationObj);
				cc += " Tj\nT*\n";
			}
			cc += "ET\nEMC";
		}
		else
			cc += "1 0 0 1 0 0 Tm\n0 0 Td\n" + EncStringUTF16(bmUtf16, annotationObj) + " Tj\nET\nEMC";
		PDF_xForm(appearanceObj, ite->width(), ite->height(), cc);
	}
	if (ite->annotation().Type() == Annotation::Checkbox)
	{
		QByteArray cc;
		cc += "q\n1 g\n";
		cc += "0 0 "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += createBorderAppearance(ite);
		cc += "BT\n";
		if (ite->itemText.defaultStyle().charStyle().fillColor() != CommonStrings::None)
			cc += putColor(ite->itemText.defaultStyle().charStyle().fillColor(), ite->itemText.defaultStyle().charStyle().fillShade(), true);
		cc += "/"+StdFonts["/ZapfDingbats"]+" "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf\n";
		cc += Pdf::toPdf(ite->annotation().Bwid())+" "+Pdf::toPdf(ite->annotation().Bwid())+" Td\n("+ct+") Tj\nET\nQ";
		PDF_xForm(appearanceObj1, ite->width(), ite->height(), cc);
		cc = "";
		cc += "q\n1 g\n";
		cc += "0 0 "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += createBorderAppearance(ite);
		cc += "Q\n";
		PDF_xForm(appearanceObj2, ite->width(), ite->height(), cc);
	}
	if ((ite->annotation().Type() == Annotation::Combobox) || (ite->annotation().Type() == Annotation::Listbox))
	{
		QByteArray cc;
		cc += "1 g\n";
		cc += "0 0 "+FToStr(x2-x)+" "+FToStr(y-y2)+" re\nf\n";
		cc += createBorderAppearance(ite);
		cc += "/Tx BMC\nq\nBT\n";
		cc += "0 g\n";
		if (Options.Version < PDFOptions::PDFVersion_14)
			cc += "/"+StdFonts[ind2PDFabr[ite->annotation().Font()]];
		else
			cc += UsedFontsF[ite->itemText.defaultStyle().charStyle().font().replacementName()].name;
//			cc += UsedFontsP[ite->itemText.defaultStyle().charStyle().font().replacementName()]+"Form";
		cc += " "+FToStr(ite->itemText.defaultStyle().charStyle().fontSize() / 10.0)+" Tf\n";
		cc += "1 0 0 1 0 0 Tm\n0 0 Td\n";
		if (ite->annotation().Type() == Annotation::Listbox)
		{
			if (bmstUtf16.count() > 1)
			{
				for (int mz = 0; mz < bmstUtf16.count(); ++mz)
				{
					cc += EncStringUTF16(bmstUtf16[mz], annotationObj);
					cc += " Tj\nT*\n";
				}
				cc += "ET\nEMC";
			}
			else
				cc += EncStringUTF16(bmUtf16, annotationObj) + " Tj\nET\nEMC";
		}
		else
		{
			if (bmstUtf16.count() > 0)
				cc += EncStringUTF16(bmstUtf16[0], annotationObj) + " Tj\n";
			cc += "ET\nQ\nEMC";
		}
		PDF_xForm(appearanceObj, ite->width(), ite->height(), cc);
	}
	return true;
}

QByteArray PDFLibCore::createBorderAppearance(PageItem *ite)
{
	double dx = ite->width();
	double dy = ite->height();
	QByteArray ret = "";
	if (ite->annotation().borderColor() == CommonStrings::None)
		return ret;
	if (ite->annotation().Bwid() == 0)
		return ret;
	QColor tmp;
	ite->SetQColor(&tmp, ite->annotation().borderColor(), 100);
	ret += "q\n";
	if (ite->annotation().Type() == Annotation::RadioButton)
	{
		if ((ite->annotation().Bsty() == 0) || (ite->annotation().Bsty() == 1))
		{
			ret += putColor(ite->annotation().borderColor(), 100, false);
			ret += Pdf::toPdf(ite->annotation().Bwid())+" w\n";
			ret += "0 J\n";
			ret += "0 j\n";
			if (ite->annotation().Bsty() == 1)
				ret += "["+Pdf::toPdf(ite->annotation().Bwid()*4)+" "+Pdf::toPdf(ite->annotation().Bwid()*2)+"] 0 d\n";
			double bwh = ite->annotation().Bwid() / 2.0;
			double rad = qMin(ite->width() - ite->annotation().Bwid(), ite->height() - ite->annotation().Bwid());
			QPainterPath clp;
			clp.addEllipse(QRectF(bwh, bwh, rad, rad));
			FPointArray clpArr;
			clpArr.fromQPainterPath(clp);
			clpArr.translate(0, -ite->height());
			ret += SetClipPathArray(&clpArr, true);
			ret += "h\nS\n";
		}
		else if ((ite->annotation().Bsty() == 3) || (ite->annotation().Bsty() == 4))
		{
			QColor shade;
			QColor light;
			if (ite->annotation().Bsty() == 4)
			{
				shade.setRgbF(tmp.redF() * 0.5, tmp.greenF() * 0.5, tmp.blueF() * 0.5);
				light.setRgbF(tmp.redF() * 0.5 + 0.5, tmp.greenF() * 0.5 + 0.5, tmp.blueF() * 0.5 + 0.5);
			}
			else
			{
				light.setRgbF(tmp.redF() * 0.5, tmp.greenF() * 0.5, tmp.blueF() * 0.5);
				shade.setRgbF(tmp.redF() * 0.5 + 0.5, tmp.greenF() * 0.5 + 0.5, tmp.blueF() * 0.5 + 0.5);
			}
			ret += "0 J\n";
			ret += "0 j\n";
			ret += putColor(ite->annotation().borderColor(), 100, false);
			double bwh = ite->annotation().Bwid() / 2.0;
			int h, s, v;
			double cx = dx / 2.0;
			double cy = dy / 2.0;
			double rb = 0.5 * (dx < dy ? dx : dy);
			double r = rb - 0.25 * ite->annotation().Bwid();
			double bzc = 0.55228475;
			ret += Pdf::toPdf(bwh)+" w\n";
			ret += FToStr(cx + r)+" "+FToStr(cy)+" m\n";
			ret += FToStr(cx + r)+" "+FToStr(cy + bzc * r)+" "+FToStr(cx + bzc * r)+" "+FToStr(cy + r)+" "+FToStr(cx)+" "+FToStr(cy + r)+" c\n";
			ret += FToStr(cx - bzc * r)+" "+FToStr(cy + r)+" "+FToStr(cx - r)+" "+FToStr(cy + bzc * r)+" "+FToStr(cx - r)+" "+FToStr(cy)+" c\n";
			ret += FToStr(cx - r)+" "+FToStr(cy - bzc * r)+" "+FToStr(cx - bzc * r)+" "+FToStr(cy - r)+" "+FToStr(cx)+" "+FToStr(cy - r)+" c\n";
			ret += FToStr(cx + bzc * r)+" "+FToStr(cy - r)+" "+FToStr(cx + r)+" "+FToStr(cy - bzc * r)+" "+FToStr(cx + r)+" "+FToStr(cy)+" c\n";
			ret += "h\nS\n";
			r = rb - 0.73 * ite->annotation().Bwid();
			double r2 = r / 1.414213562;
			shade.getRgb(&h, &s, &v);
			ret += FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" RG\n";
			ret += FToStr(cx + r2)+" "+FToStr(cy + r2)+" m\n";
			ret += FToStr(cx + (1 - bzc) * r2)+" "+FToStr(cy + (1 + bzc) * r2)+" "+FToStr(cx - (1 - bzc) * r2)+" "+FToStr(cy + (1 + bzc) * r2)+" "+FToStr(cx - r2)+" "+FToStr(cy + r2)+" c\n";
			ret += FToStr(cx - (1 + bzc) * r2)+" "+FToStr(cy + (1 - bzc) * r2)+" "+FToStr(cx - (1 + bzc) * r2)+" "+FToStr(cy - (1 - bzc) * r2)+" "+FToStr(cx - r2)+" "+FToStr(cy - r2)+" c\n";
			ret += "S\n";
			light.getRgb(&h, &s, &v);
			ret += FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" RG\n";
			ret += FToStr(cx - r2)+" "+FToStr(cy - r2)+" m\n";
			ret += FToStr(cx - (1 - bzc) * r2)+" "+FToStr(cy - (1 + bzc) * r2)+" "+FToStr(cx + (1 - bzc) * r2)+" "+FToStr(cy - (1 + bzc) * r2)+" "+FToStr(cx + r2)+" "+FToStr(cy - r2)+" c\n";
			ret += FToStr(cx + (1 + bzc) * r2)+" "+FToStr(cy - (1 - bzc) * r2)+" "+FToStr(cx + (1 + bzc) * r2)+" "+FToStr(cy + (1 - bzc) * r2)+" "+FToStr(cx + r2)+" "+FToStr(cy + r2)+" c\n";
			ret += "S\n";
		}
	}
	else
	{
		if ((ite->annotation().Bsty() == 0) || (ite->annotation().Bsty() == 1))
		{
			ret += putColor(ite->annotation().borderColor(), 100, false);
			ret += Pdf::toPdf(ite->annotation().Bwid())+" w\n";
			ret += "0 J\n";
			ret += "0 j\n";
			if (ite->annotation().Bsty() == 1)
				ret += "["+Pdf::toPdf(ite->annotation().Bwid()*4)+" "+Pdf::toPdf(ite->annotation().Bwid()*2)+"] 0 d\n";
			double bwh = ite->annotation().Bwid() / 2.0;
			QPainterPath clp;
			clp.addRect(QRectF(bwh, bwh, ite->width() - ite->annotation().Bwid(), ite->height() - ite->annotation().Bwid()));
			FPointArray clpArr;
			clpArr.fromQPainterPath(clp);
			clpArr.translate(0, -ite->height());
			ret += SetClipPathArray(&clpArr, true);
			ret += "h\nS\n";
		}
		else if ((ite->annotation().Bsty() == 3) || (ite->annotation().Bsty() == 4))
		{
			QColor shade;
			QColor light;
			if (ite->annotation().Bsty() == 4)
			{
				shade.setRgbF(tmp.redF() * 0.5, tmp.greenF() * 0.5, tmp.blueF() * 0.5);
				light.setRgbF(tmp.redF() * 0.5 + 0.5, tmp.greenF() * 0.5 + 0.5, tmp.blueF() * 0.5 + 0.5);
			}
			else
			{
				light.setRgbF(tmp.redF() * 0.5, tmp.greenF() * 0.5, tmp.blueF() * 0.5);
				shade.setRgbF(tmp.redF() * 0.5 + 0.5, tmp.greenF() * 0.5 + 0.5, tmp.blueF() * 0.5 + 0.5);
			}
			int h, s, v;
			shade.getRgb(&h, &s, &v);
			ret += FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" rg\n";
			ret += "0 w\n";
			ret += "0 0 m\n";
			ret += "0 "+FToStr(dy)+" l\n";
			ret += FToStr(dx)+" "+FToStr(dy)+" l\n";
			ret += FToStr(dx - ite->annotation().Bwid())+" "+FToStr(dy - ite->annotation().Bwid())+" l\n";
			ret += FToStr(ite->annotation().Bwid())+" "+FToStr(dy - ite->annotation().Bwid())+" l\n";
			ret += FToStr(ite->annotation().Bwid())+" "+FToStr(ite->annotation().Bwid())+" l\n";
			ret += "h\nf\n";
			light.getRgb(&h, &s, &v);
			ret += FToStr(h / 255.0)+" "+FToStr(s / 255.0)+" "+FToStr(v / 255.0)+" rg\n";
			ret += "0 0 m\n";
			ret += FToStr(dx)+" 0 l\n";
			ret += FToStr(dx)+" "+FToStr(dy)+" l\n";
			ret += FToStr(dx - ite->annotation().Bwid())+" "+FToStr(dy - ite->annotation().Bwid())+" l\n";
			ret += FToStr(dx - ite->annotation().Bwid())+" "+FToStr(ite->annotation().Bwid())+" l\n";
			ret += FToStr(ite->annotation().Bwid())+" "+FToStr(ite->annotation().Bwid())+" l\n";
			ret += "h\nf\n";
		}
	}
	ret += "Q\n";
	return ret;
}

PdfId PDFLibCore::writeActions(const Annotation&	annot, PdfId annotationObj)
{
	// write actions
	if ((annot.Type() > 1) && (annot.AAact()))
	{
		PdfId E = 0;
		if (!annot.E_act().isEmpty())
			E = WritePDFString(annot.E_act());
		PdfId X = 0;
		if (!annot.X_act().isEmpty())
			X = WritePDFString(annot.X_act());
		PdfId D = 0;
		if (!annot.D_act().isEmpty())
			D = WritePDFString(annot.D_act());
		PdfId Fo = 0;
		if (!annot.Fo_act().isEmpty())
			Fo = WritePDFString(annot.Fo_act());
		PdfId Bl = 0;
		if (!annot.Bl_act().isEmpty())
			Bl = WritePDFString(annot.Bl_act());
		PdfId K = 0;
		PdfId F = 0;
		PdfId V = 0;
		PdfId C = 0;
		if ((annot.Type() == Annotation::Textfield) || (annot.Type() == Annotation::Combobox) || (annot.Type() == Annotation::Listbox))
		{
			if (!annot.K_act().isEmpty())
				K = WritePDFString(annot.K_act());
			if (!annot.F_act().isEmpty())
				F = WritePDFString(annot.F_act());
			if (!annot.V_act().isEmpty())
				V = WritePDFString(annot.V_act());
			if (!annot.C_act().isEmpty())
			{
				C = WritePDFString(annot.C_act());
				CalcFields.append(annotationObj);
			}
		}

		PdfId result = writer.newObject();
		writer.startObj(result);
		PutDoc("<<\n");
		if (E)
		{
			PutDoc("/E << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(E)+" 0 R >>\n");
		}
		if (X)
		{
			PutDoc("/X << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(X)+" 0 R >>\n");
		}
		if (D)
		{
			PutDoc("/D << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(D)+" 0 R >>\n");
		}
		if (Fo)
		{
			PutDoc("/Fo << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(Fo)+" 0 R >>\n");
		}
		if (Bl)
		{
			PutDoc("/Bl << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(Bl)+" 0 R >>\n");
		}
		if ((annot.Type() == Annotation::Textfield) || (annot.Type() == Annotation::Combobox) || (annot.Type() == Annotation::Listbox))
		{
			if (K)
			{
				PutDoc("/K << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(K) + " 0 R >>\n");
			}
			if (F)
			{
				PutDoc("/F << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(F) + " 0 R >>\n");
			}
			if (V)
			{
				PutDoc("/V << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(V)+" 0 R >>\n");
			}
			if (C)
			{
				PutDoc("/C << /Type /Action /S /JavaScript /JS "+Pdf::toPdf(C)+" 0 R >>\n");
			}
		}
		PutDoc(">>");
		writer.endObj(result);
		return result;
	}
	return 0;
}

PdfId PDFLibCore::WritePDFStream(const QByteArray& cc)
{
	PdfId result = writer.newObject();
	return WritePDFStream(cc, result);
}

PdfId PDFLibCore::WritePDFStream(const QByteArray& cc, PdfId objId)
{
	QByteArray tmp(cc);
	if (Options.Compress)
		tmp = CompressArray(tmp);
	writer.startObj(objId);
	PutDoc("<< /Length " + Pdf::toPdf(tmp.length()));  // moeglicherweise +1
	if (Options.Compress)
		PutDoc("\n/Filter /FlateDecode");
	PutDoc(" >>\nstream\n" + EncStream(tmp, objId) + "\nendstream");
	writer.endObj(objId);
	return objId;
}

PdfId PDFLibCore::WritePDFString(const QString& cc)
{
	QByteArray tmp;
	for (int i = 0; i < cc.length(); ++i)
	{
		uchar pdfChar = Pdf::toPdfDocEncoding(cc[i]);
		if ((pdfChar != 0) || cc[i].isNull())
			tmp += pdfChar;
		else
		{
			tmp += "\\u";
			tmp += toHex(cc[i].row());
			tmp += toHex(cc[i].cell());
		}
	}
	return WritePDFStream(tmp);
}

PdfId PDFLibCore::WritePDFString(const QString& cc, PdfId objId)
{
	QByteArray tmp;
	for (int i = 0; i < cc.length(); ++i)
	{
		uchar pdfChar = Pdf::toPdfDocEncoding(cc[i]);
		if ((pdfChar != 0) || cc[i].isNull())
			tmp += pdfChar;
		else
		{
			tmp += "\\u";
			tmp += toHex(cc[i].row());
			tmp += toHex(cc[i].cell());
		}
	}
	return WritePDFStream(tmp, objId);
}

void PDFLibCore::PDF_xForm(PdfId objNr, double w, double h, QByteArray im)
{
	writer.startObj(objNr);
	PutDoc("<<\n/Type /XObject\n/Subtype /Form\n");
	PutDoc("/BBox [ 0 0 "+FToStr(w)+" "+FToStr(h)+" ]\n");
	PutDoc("/Resources ");
	
	Pdf::ResourceDictionary dict;
	dict.XObject.unite(pageData.ImgObjects);
	dict.XObject.unite(pageData.XObjects);
	dict.Font = pageData.FObjects;
	writer.write(dict);

	PutDoc("/Length "+Pdf::toPdf(im.length())+"\n");
	PutDoc(">>\nstream\n"+EncStream(im, objNr)+"\nendstream");
	writer.endObj(objNr);
	pageData.XObjects[ResNam+Pdf::toPdf(ResCount)] = objNr;
	ResCount++;
}

void PDFLibCore::PDF_Form(const QByteArray& im) // unused? - av
{
	PdfId form = writer.newObject();
	writer.startObj(form);
	PutDoc("<<\n");
	PutDoc("/Resources ");
	
	Pdf::ResourceDictionary dict;
	dict.Font = pageData.FObjects;
	writer.write(dict);

	PutDoc("/Length "+Pdf::toPdf(im.length())+"\n");
	PutDoc(">>\nstream\n"+EncStream(im, form)+"\nendstream");
	writer.endObj(form);
}

void PDFLibCore::PDF_Bookmark(PageItem *currItem, double ypos)
{
	Bvie->setAction(currItem, "/XYZ 0 "+FToStr(ypos)+" 0");
	BookMinUse = true;
}

bool PDFLibCore::PDF_EmbeddedPDF(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN, ShIm& imgInfo, bool &fatalError)
{
	if (!Options.embedPDF)
		return false;
	fatalError = false;
	
#ifdef HAVE_PODOFO
	// Try to catch potential pdf parsing exceptions early
	// so we can use the raster fallback if needed
	QScopedPointer<PoDoFo::PdfMemDocument> doc;
	try
	{
		PoDoFo::PdfError::EnableDebug( false );
		PoDoFo::PdfError::EnableLogging( false );
		doc.reset(new PoDoFo::PdfMemDocument(fn.toLocal8Bit().data()));
	}
	catch(PoDoFo::PdfError& e)
	{
		qDebug() << "PoDoFo error, falling back to raster!";
		e.PrintErrorMsg();
		return false;
	}

	try
	{
		PoDoFo::PdfPage*   page      = doc->GetPage(qMin(qMax(1, c->pixm.imgInfo.actualPageNumber), c->pixm.imgInfo.numberOfPages) - 1);
		PoDoFo::PdfObject* contents  = page? page->GetContents() : NULL;
		PoDoFo::PdfObject* resources = page? page->GetResources() : NULL;
		for (PoDoFo::PdfObject* par = page->GetObject(); par && !resources; par = par->GetIndirectKey("Parent"))
		{
			resources = par->GetIndirectKey("Resources");
		}
		if (contents && contents->GetDataType() ==  PoDoFo::ePdfDataType_Dictionary)
		{
			PoDoFo::PdfStream* stream = contents->GetStream();
			QMap<PoDoFo::PdfReference, PdfId> importedObjects;
			QList<PoDoFo::PdfReference> referencedObjects;
			PoDoFo::PdfObject* nextObj;
			PdfId xObj = writer.newObject();
			PdfId xResources = writer.newObject();
			PdfId xParents = 0;
			importedObjects[page->GetObject()->Reference()] = xObj;
			writer.startObj(xObj);
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1");
			PoDoFo::PdfRect pageRect = page->GetArtBox(); // Because scimagedataloader_pdf use ArtBox
			int rotation = page->GetRotation();
			double imgWidth  = (rotation == 90 || rotation == 270) ? pageRect.GetHeight() : pageRect.GetWidth();
			double imgHeight = (rotation == 90 || rotation == 270) ? pageRect.GetWidth() : pageRect.GetHeight();
			QTransform pageM;
			pageM.translate(pageRect.GetLeft(), pageRect.GetBottom());
			pageM.rotate(rotation);
			if (rotation == 90)
				pageM.translate(0.0, -imgHeight);
			else if (rotation == 180)
				pageM.translate(-imgWidth, -imgHeight);
			else if (rotation == 270)
				pageM.translate(-imgWidth, 0.0);
			pageM.scale(imgWidth, imgHeight);
			pageM = pageM.inverted();
			PutDoc("\n/BBox [" + Pdf::toPdf(pageRect.GetLeft()));
			PutDoc(" " + Pdf::toPdf(pageRect.GetBottom()));
			PutDoc(" " + Pdf::toPdf(pageRect.GetLeft() + pageRect.GetWidth()));
			PutDoc(" " + Pdf::toPdf(pageRect.GetBottom() + pageRect.GetHeight()));
			PutDoc("]");
			PutDoc("\n/Matrix [" + Pdf::toPdf(pageM.m11()) + " "
								 + Pdf::toPdf(pageM.m12()) + " "
								 + Pdf::toPdf(pageM.m21()) + " "
								 + Pdf::toPdf(pageM.m22()) + " "
								 + Pdf::toPdf(pageM.dx())  + " "
								 + Pdf::toPdf(pageM.dy())  + " ");
			PutDoc("]");
			PutDoc("\n/Resources " + Pdf::toPdf(xResources) + " 0 R");
			nextObj = page->GetObject()->GetIndirectKey("Group");
			if (nextObj)
			{
				PutDoc("\n/Group "); // PDF 1.4
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
			/*
			PoDoFo::PdfObject parents = page->GetObject()->GetIndirectKey("StructParents");
			if (parents)
			{
				xParents = writer.newObject();
				PutDoc("\n/StructParents " + Pdf::toPdf(xParents)); // required if page uses structured content
			}
			*/
			char * mbuffer = NULL;
			long mlen = 0;

#if (PODOFO_VERSION >= PODOFO_MAKE_VERSION(0, 7, 99))
			// seems more complicated at first, but in fact it makes the code more stable wrt podofo changes
			PoDoFo::PdfMemoryOutputStream oStream(1);
			stream->GetCopy(&oStream);
			oStream.Close();
			mlen = oStream.GetLength();
			mbuffer = oStream.TakeBuffer();
#else
			stream->GetCopy(&mbuffer, &mlen);
#endif
			if (mbuffer[mlen-1] == '\n')
				--mlen;
			PutDoc("\n/Length " + Pdf::toPdf(static_cast<qlonglong>(mlen)));
			nextObj = contents->GetIndirectKey("Filter");
			if (nextObj)
			{
				PutDoc("\n/Filter ");
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
			nextObj = contents->GetIndirectKey("DecodeParms");
			if (nextObj)
			{
				PutDoc("\n/DecodeParms ");
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
			PutDoc("\n>>\nstream\n");
			{
				QByteArray buffer = QByteArray::fromRawData(mbuffer, mlen);
				EncodeArrayToStream(buffer, xObj);
			}  // disconnect QByteArray from raw data
			free (mbuffer);
			PutDoc("\nendstream");
			writer.endObj(xObj);
			// write resources
			if (resources)
			{
				copyPoDoFoObject(resources, xResources, importedObjects);
			}
			else
			{
				writer.startObj(xResources);
				PutDoc("<< >>");
				writer.endObj(xResources);
			}
			if (xParents)
			{
				// create structured parents
			}
			// write referenced objects
			PoDoFo::PdfVecObjects* allObjects = contents->GetOwner();
			for (int i=0; i < referencedObjects.size();  ++i)
			{
				nextObj = allObjects->GetObject(referencedObjects[i]);
				copyPoDoFoObject(nextObj, importedObjects[nextObj->Reference()], importedObjects);
			}

			pageData.ImgObjects[ResNam+"I"+Pdf::toPdf(ResCount)] = xObj;
			imgInfo.ResNum = ResCount;
			ResCount++;
			// Avoid a divide-by-zero if width/height are less than 1 point:
			imgInfo.Width  = qMax(1, (int) imgWidth);
			imgInfo.Height = qMax(1, (int) imgHeight);
			imgInfo.xa  = sx * imgWidth / imgInfo.Width;
			imgInfo.ya  = sy * imgHeight / imgInfo.Height;
			// Width/Height are integers and may not exactly equal pageRect.GetWidth()/
			// pageRect.GetHeight(). Adjust scale factor to compensate for the difference.
			imgInfo.sxa = sx * imgWidth / imgInfo.Width;
			imgInfo.sya = sy * imgHeight / imgInfo.Height;

			return true;
		}
		else if (contents && contents->GetDataType() ==  PoDoFo::ePdfDataType_Array)//Page contents might be an array
		{
			QMap<PoDoFo::PdfReference, PdfId> importedObjects;
			QList<PoDoFo::PdfReference> referencedObjects;
			PoDoFo::PdfObject* nextObj;
			PdfId xObj = writer.newObject();
			PdfId xResources = writer.newObject();
			PdfId xParents = 0;
			importedObjects[page->GetObject()->Reference()] = xObj;
			writer.startObj(xObj);
			PutDoc("<<\n/Type /XObject\n/Subtype /Form\n/FormType 1");
			PoDoFo::PdfRect pageRect = page->GetArtBox(); // Because scimagedataloader_pdf use ArtBox
			int rotation = page->GetRotation();
			double imgWidth  = (rotation == 90 || rotation == 270) ? pageRect.GetHeight() : pageRect.GetWidth();
			double imgHeight = (rotation == 90 || rotation == 270) ? pageRect.GetWidth() : pageRect.GetHeight();
			QTransform pageM;
			pageM.translate(pageRect.GetLeft(), pageRect.GetBottom());
			pageM.rotate(rotation);
			if (rotation == 90)
				pageM.translate(0.0, -imgHeight);
			else if (rotation == 180)
				pageM.translate(-imgWidth, -imgHeight);
			else if (rotation == 270)
				pageM.translate(-imgWidth, 0.0);
			pageM.scale(imgWidth, imgHeight);
			pageM = pageM.inverted();
			PutDoc("\n/BBox [" + Pdf::toPdf(pageRect.GetLeft()));
			PutDoc(" " + Pdf::toPdf(pageRect.GetBottom()));
			PutDoc(" " + Pdf::toPdf(pageRect.GetLeft() + pageRect.GetWidth()));
			PutDoc(" " + Pdf::toPdf(pageRect.GetBottom() + pageRect.GetHeight()));
			PutDoc("]");
			PutDoc("\n/Matrix [" + Pdf::toPdf(pageM.m11()) + " "
								 + Pdf::toPdf(pageM.m12()) + " "
								 + Pdf::toPdf(pageM.m21()) + " "
								 + Pdf::toPdf(pageM.m22()) + " "
								 + Pdf::toPdf(pageM.dx())  + " "
								 + Pdf::toPdf(pageM.dy())  + " ");
			PutDoc("]");
			PutDoc("\n/Resources " + Pdf::toPdf(xResources) + " 0 R");
			nextObj = page->GetObject()->GetIndirectKey("Group");
			if (nextObj)
			{
				PutDoc("\n/Group "); // PDF 1.4
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}

			char * mbuffer = NULL;
			long mlen = 0;
			// copied from podofoimpose
			PoDoFo::PdfMemoryOutputStream outMemStream ( 1 );
//			PoDoFo::PdfFilteredEncodeStream outMemStream (outMemStreamRaw, ePdfFilter_FlateDecode, false);
			PoDoFo::PdfArray carray(page->GetContents()->GetArray());
			for(unsigned int ci = 0; ci < carray.size(); ++ci)
			{
				if(carray[ci].HasStream())
				{
					carray[ci].GetStream()->GetFilteredCopy ( &outMemStream );
				}
				else if(carray[ci].IsReference())
				{
					nextObj = doc->GetObjects().GetObject(carray[ci].GetReference());

					while(nextObj != NULL)
					{

						if(nextObj->IsReference())
						{
							nextObj = doc->GetObjects().GetObject(nextObj->GetReference());
						}
						else if(nextObj->HasStream())
						{
							nextObj->GetStream()->GetFilteredCopy ( &outMemStream );
							break;
						}
					}
				}
			}
			// end of copy
			mlen = outMemStream.GetLength();
			mbuffer = outMemStream.TakeBuffer();
//			if (mbuffer[mlen-1] == '\n')
//				--mlen;
			PutDoc("\n/Length " + Pdf::toPdf(static_cast<qlonglong>(mlen)));
/*
 nextObj = contents->GetIndirectKey("Filter");
			if (nextObj)
			{
				PutDoc("\n/Filter ");
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
			nextObj = contents->GetIndirectKey("DecodeParms");
			if (nextObj)
			{
				PutDoc("\n/DecodeParms ");
				copyPoDoFoDirect(nextObj, referencedObjects, importedObjects);
			}
*/
			PutDoc("\n>>\nstream\n");
			{
				QByteArray buffer = QByteArray::fromRawData(mbuffer, mlen);
				EncodeArrayToStream(buffer, xObj);
			}  // disconnect QByteArray from raw data
			free (mbuffer);
			PutDoc("\nendstream");
			writer.endObj(xObj);
			// write resources
			if (resources)
			{
				copyPoDoFoObject(resources, xResources, importedObjects);
			}
			else
			{
				writer.startObj(xResources);
				PutDoc("<< >>");
				writer.endObj(xResources);
			}
			if (xParents)
			{
				// create structured parents
			}
			// write referenced objects
			PoDoFo::PdfVecObjects* allObjects = contents->GetOwner();
			for (int i=0; i < referencedObjects.size();  ++i)
			{
				nextObj = allObjects->GetObject(referencedObjects[i]);
				copyPoDoFoObject(nextObj, importedObjects[nextObj->Reference()], importedObjects);
			}

			pageData.ImgObjects[ResNam+"I"+Pdf::toPdf(ResCount)] = xObj;
			imgInfo.ResNum = ResCount;
			ResCount++;
			// Avoid a divide-by-zero if width/height are less than 1 point:
			imgInfo.Width  = qMax(1, (int) imgWidth);
			imgInfo.Height = qMax(1, (int) imgHeight);
			imgInfo.xa  = sx * imgWidth / imgInfo.Width;
			imgInfo.ya  = sy * imgHeight / imgInfo.Height;
			// Width/Height are integers and may not exactly equal pageRect.GetWidth()/
			// pageRect.GetHeight(). Adjust scale factor to compensate for the difference.
			imgInfo.sxa = sx * imgWidth / imgInfo.Width;
			imgInfo.sya = sy * imgHeight / imgInfo.Height;

			return true;
		}

	}
	catch(PoDoFo::PdfError& e)
	{
		fatalError = true;
		qDebug() << "PoDoFo error!";
		e.PrintErrorMsg();
		return false;
	}
#endif
	return false;
}


#if HAVE_PODOFO

void PDFLibCore::copyPoDoFoDirect(const PoDoFo::PdfVariant* obj, QList<PoDoFo::PdfReference>& referencedObjects, QMap<PoDoFo::PdfReference, PdfId>& importedObjects)
{
	switch (obj->GetDataType())
	{
		case PoDoFo::ePdfDataType_Reference:
		{
			const PoDoFo::PdfReference reference(obj->GetReference());
			PdfId objNr;
			if (!importedObjects.contains(reference))
			{
				objNr = writer.newObject();
				importedObjects[reference] = objNr;
				referencedObjects.append(reference);
			}
			else
			{
				objNr = importedObjects[reference];
			}
			PutDoc(" " + Pdf::toPdf(objNr) + " 0 R");
			break;
		}
		case PoDoFo::ePdfDataType_Array:
		{
			const PoDoFo::PdfArray& array(obj->GetArray());
			PutDoc("[");
			for (uint i=0; i < array.size(); ++i)
				copyPoDoFoDirect( &(array[i]), referencedObjects, importedObjects);
			PutDoc("]");	
			break;
		}
		case PoDoFo::ePdfDataType_Dictionary:
		{
			const PoDoFo::PdfDictionary& dict(obj->GetDictionary());
			const PoDoFo::TKeyMap keys = dict.GetKeys();
			PutDoc("<<");
			for (PoDoFo::TCIKeyMap k=keys.begin(); k != keys.end(); ++k)
			{
				std::string str("\n/" + k->first.GetEscapedName());
				PutDoc(QByteArray::fromRawData(str.data(), str.size()));
				copyPoDoFoDirect(k->second, referencedObjects, importedObjects);
			}
			PutDoc(" >>");
			break;
		}
		default:
		{
			std::string str;
			obj->ToString(str);
			str = " " + str;
			PutDoc(QByteArray::fromRawData(str.data(), str.size()));
		}
	}

}

void PDFLibCore::copyPoDoFoObject(const PoDoFo::PdfObject* obj, PdfId scObjID, QMap<PoDoFo::PdfReference, PdfId>& importedObjects)
{
	PoDoFo::PdfVecObjects* allObjects = obj->GetOwner();
	QList<PoDoFo::PdfReference> referencedObjects;
	writer.startObj(scObjID);
	copyPoDoFoDirect(obj, referencedObjects, importedObjects);
	if (obj->HasStream())
	{
		const PoDoFo::PdfStream* stream = obj->GetStream();
		char * mbuffer = NULL;
		long mlen = 0;

#if (PODOFO_VERSION >= PODOFO_MAKE_VERSION(0, 7, 99))
		// seems more complicated at first, but in fact it makes the code more stable wrt podofo changes
		PoDoFo::PdfMemoryOutputStream oStream(1);
		stream->GetCopy(&oStream);
		oStream.Close();
		mlen = oStream.GetLength();
		mbuffer = oStream.TakeBuffer();
#else
		stream->GetCopy(&mbuffer, &mlen);
#endif
		if (mbuffer[mlen-1] == '\n')
			--mlen;
		PutDoc("\nstream\n");
		{
			QByteArray buffer = QByteArray::fromRawData(mbuffer, mlen);
			EncodeArrayToStream(buffer, scObjID);
		}  // disconnect QByteArray from raw data
		free (mbuffer);
		PutDoc("\nendstream");
	}
	PutDoc("");
	writer.endObj(scObjID);
	// recurse:
	for (int i=0; i < referencedObjects.size();  ++i)
	{
		PoDoFo::PdfObject* nextObj = allObjects->GetObject(referencedObjects[i]);
		copyPoDoFoObject(nextObj, importedObjects[nextObj->Reference()], importedObjects);
	}
}
#endif

/*
 * Helper function to transition for booleans to ColorSpaceEnum. This
 * should be removed once output format is specified directly.
 */
static ColorSpaceEnum getOutputType(const bool gray, const bool cmyk)
{
	if (gray)
		return ColorSpaceGray;
	if (cmyk)
		return ColorSpaceCMYK;
	return ColorSpaceRGB;
}

/**
 * Add the image item to this.output
 * Returns false if the image can't be read or if it can't be added to this.output
*/
bool PDFLibCore::PDF_Image(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN, const QString& Profil, bool Embedded, eRenderIntent Intent, QByteArray* output)
{
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(fn);
	ScImage img;
	QString BBox;
	QChar  tc;
	bool   found = false;
	bool   alphaM = false;
	bool   realCMYK = false;
	bool   bitmapFromGS = false;
	bool   isEmbeddedPDF = false;
	bool   hasGrayProfile = false;
	bool   avoidPDFXOutputIntentProf = false;
	QString profInUse = Profil;
	int    afl = Options.Resolution;
	double ax, ay, a2, a1;
	int    origWidth = 1;
	int    origHeight = 1;
	ShIm   ImInfo;
	ImInfo.ResNum = 0;
	ImInfo.sxa = 0;
	ImInfo.sya = 0;
	if (Options.RecalcPic)
		ImInfo.reso = Options.PicRes / 72.0;
	else
		ImInfo.reso = Options.Resolution / 72.0;
	ImInfo.Width = 0;
	ImInfo.Height = 0;
	ImInfo.Page = 0;
	ImInfo.xa = x;
	ImInfo.ya = y;
	ImInfo.origXsc = c->imageXScale();
	ImInfo.origYsc = c->imageYScale();
	ShIm   ImInfo2;
	ImInfo2.origXsc = c->imageXScale();
	ImInfo2.origYsc = c->imageYScale();
	if (SharedImages.contains(fn))
		ImInfo2 = SharedImages[fn];
	if ((!SharedImages.contains(fn))
		 || (fromAN)
		 || (c->asLatexFrame())
		 || (c->effectsInUse.count() != 0)
		 || ((ImInfo2.origXsc != ImInfo.origXsc) || (ImInfo2.origYsc != ImInfo.origYsc))
		 || (ImInfo2.RequestProps != c->pixm.imgInfo.RequestProps)
		 || (ImInfo2.Page != c->pixm.imgInfo.actualPageNumber))
	{
		bool imageLoaded = false;
		bool fatalError  = false;
		QString pdfFile = fn;
		if ((extensionIndicatesPDF(ext) || ((extensionIndicatesEPSorPS(ext)) && (c->pixm.imgInfo.type != ImageType7))) && c->effectsInUse.count() == 0)
		{
			if (extensionIndicatesEPSorPS(ext))
			{
				QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc.pdf");
				QStringList opts;
				opts.append("-dEPSCrop");
				if (Options.Version >= PDFOptions::PDFVersion_14)
					opts.append( "-dCompatibilityLevel=1.4" );
				else
					opts.append( "-dCompatibilityLevel=1.3" );
/*				These options don't seem to work
				if ((Options.UseRGB) || ((doc.HasCMS) && (Options.UseProfiles2)) || (Options.isGrayscale))
					opts.append( "-sProcessColorModel=/DeviceRGB" );
				else
					opts.append( "-sProcessColorModel=/DeviceCMYK" ); */
				opts.append( "-dAutoRotatePages=/None" ); // #14289: otherwise EPS might come out rotated
				if (convertPS2PDF(fn, tmpFile, opts) == 0)
				{
					imageLoaded = PDF_EmbeddedPDF(c, tmpFile, sx, sy, x, y, fromAN, ImInfo, fatalError);
					QFile::remove(tmpFile);
				}
				pdfFile = tmpFile;
			}
			else
				imageLoaded = PDF_EmbeddedPDF(c, fn, sx, sy, x, y, fromAN, ImInfo, fatalError);
			isEmbeddedPDF = true;
			ImInfo.Page = c->pixm.imgInfo.actualPageNumber;
		}
		if (!imageLoaded && extensionIndicatesPDF(ext) && c->effectsInUse.count() == 0 && Options.embedPDF)
		{
			if (fatalError)
			{
				PDF_Error( tr("Failed to embed the PDF file : %1.\n"
				              "Please disable experimental PDF and PS embedding and try again.").arg(pdfFile) );
				return false;
			}
			qDebug() << "Failed to embed the PDF file";
		}
		// no embedded PDF:
		if (!imageLoaded)
		{
			if ((extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext)) && (c->pixm.imgInfo.type != ImageType7))
			{
				bitmapFromGS = true;
				if (Options.RecalcPic)
				{
					afl = qMin(Options.PicRes, Options.Resolution);
					ImInfo.reso = afl / 72.0;
				}
				else
					afl = Options.Resolution;
				if (ext == "pdf")
				{
					CMSettings cms(c->doc(), Profil, Intent);
					cms.setUseEmbeddedProfile(Embedded);
					if (Options.UseRGB)
						imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
					else
					{
						if ((doc.HasCMS) && (Options.UseProfiles2))
							imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
						else
						{
							if (Options.isGrayscale)
								imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
							else
								imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::CMYKData, afl);
						}
					}
				}
				else
				{
					QFile f(fn);
					if (f.open(QIODevice::ReadOnly))
					{
						QDataStream ts(&f);
						while (!ts.atEnd())
						{
							QString tmp = readLineFromDataStream(ts);
							if (tmp.startsWith("%%BoundingBox:"))
							{
								found = true;
								BBox = tmp.remove("%%BoundingBox:");
							}
							if (!found)
							{
								if (tmp.startsWith("%%BoundingBox"))
								{
									found = true;
									BBox = tmp.remove("%%BoundingBox");
								}
							}
							if (tmp.startsWith("%%EndComments"))
								break;
						}
						// BBox is not used...
						
						f.close();
						if (found)
						{
							CMSettings cms(c->doc(), Profil, Intent);
							cms.setUseEmbeddedProfile(Embedded);
							if (Options.UseRGB)
								imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
							else
							{
								if ((doc.HasCMS) && (Options.UseProfiles2))
									imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
								else
								{
									if (Options.isGrayscale)
										imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, afl);
									else
										imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::CMYKData, afl);
								}
							}
						}
					}
				}
				if (!imageLoaded)
				{
					PDF_Error_ImageLoadFailure(fn);
					return false;
				}
				if (Options.RecalcPic)
				{
					ImInfo.sxa = sx * (1.0 / ImInfo.reso);
					ImInfo.sya = sy * (1.0 / ImInfo.reso);
				}
			}
			// not PS/PDF
			else
			{
				img.imgInfo.valid = false;
				img.imgInfo.clipPath = "";
				img.imgInfo.PDSpathData.clear();
				img.imgInfo.layerInfo.clear();
				img.imgInfo.RequestProps = c->pixm.imgInfo.RequestProps;
				img.imgInfo.isRequest = c->pixm.imgInfo.isRequest;
				CMSettings cms(c->doc(), Profil, Intent);
				cms.setUseEmbeddedProfile(Embedded);
				if (Options.UseRGB)
					imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 72, &realCMYK);
				else
				{
					if ((doc.HasCMS) && (Options.UseProfiles2))
						imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RawData, 72, &realCMYK);
					else
					{
						if (Options.isGrayscale)
							imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 72, &realCMYK);
						else
							imageLoaded = img.loadPicture(fn, c->pixm.imgInfo.actualPageNumber, cms, ScImage::CMYKData, 72, &realCMYK);
					}
				}
				if (!imageLoaded)
				{
					PDF_Error_ImageLoadFailure(fn);
					return false;
				}
				if ((Options.RecalcPic) && (Options.PicRes < (qMax(72.0 / c->imageXScale(), 72.0 / c->imageYScale()))))
				{
					double afl = Options.PicRes;
					a2 = (72.0 / sx) / afl;
					a1 = (72.0 / sy) / afl;
					origWidth = img.width();
					origHeight = img.height();
					ax = img.width() / a2;
					ay = img.height() / a1;
					// #10510 : do not use scaled() here, may cause display problem 
					// with acrobat reader if image contains some transparency
					img.scaleImage(qRound(ax), qRound(ay));
					ImInfo.sxa = sx * a2;
					ImInfo.sya = sy * a1;
				}
				ImInfo.reso = 1;
			}
			bool hasColorEffect = false;
			if (c->effectsInUse.count() != 0)
			{
				for (int a = 0; a < c->effectsInUse.count(); ++a)
				{
					if (c->effectsInUse.at(a).effectCode == ScImage::EF_COLORIZE)
						hasColorEffect = true;
					if (c->effectsInUse.at(a).effectCode == ScImage::EF_DUOTONE)
						hasColorEffect = true;
					if (c->effectsInUse.at(a).effectCode == ScImage::EF_TRITONE)
						hasColorEffect = true;
					if (c->effectsInUse.at(a).effectCode == ScImage::EF_QUADTONE)
						hasColorEffect = true;
				}
			}
			if ((doc.HasCMS) && (Options.UseProfiles2))
			{
				if (!ICCProfiles.contains(Profil))
				{
					ScImage img3;
					int components = 0;
					QByteArray dataP;
					if ((Embedded) && (!Options.EmbeddedI))
						img3.getEmbeddedProfile(fn, &dataP, &components);
					if ((dataP.isEmpty()) || ((img.imgInfo.colorspace == ColorSpaceGray) && (hasColorEffect) && (components == 1)))
					{
						if (img.imgInfo.colorspace == ColorSpaceCMYK)
						{
							QString profilePath;
							if (Embedded && ScCore->InputProfilesCMYK.contains(Options.ImageProf))
							{
								profilePath = ScCore->InputProfilesCMYK[Options.ImageProf];
								profInUse = Options.ImageProf;
							}
							else if (ScCore->InputProfilesCMYK.contains(Profil))
								profilePath = ScCore->InputProfilesCMYK[Profil];
							else
							{
								profilePath = ScCore->InputProfilesCMYK[c->doc()->cmsSettings().DefaultImageCMYKProfile];
								profInUse = c->doc()->cmsSettings().DefaultImageCMYKProfile;
							}
							loadRawBytes(profilePath, dataP);
							components = 4;
						}
						else
						{
							QString profilePath;
							if (Embedded && ScCore->InputProfiles.contains(Options.ImageProf))
							{
								profilePath = ScCore->InputProfiles[Options.ImageProf];
								profInUse = Options.ImageProf;
							}
							else if (ScCore->InputProfiles.contains(Profil))
								profilePath = ScCore->InputProfiles[Profil];
							else
							{
								profilePath = ScCore->InputProfiles[c->doc()->cmsSettings().DefaultImageRGBProfile];
								profInUse = c->doc()->cmsSettings().DefaultImageRGBProfile;
							}
							loadRawBytes(profilePath, dataP);
							components = 3;
						}
					}
					// PDF-X/4 requires that CMYK images using the same profile as PDF/X output intent
					// do not be tagged with an ICC profile so they can go through color conversion
					// pipeline without alteration
					if (Options.Version == PDFOptions::PDFVersion_X4)
						avoidPDFXOutputIntentProf = (profInUse == Options.PrintProf);
					if (!ICCProfiles.contains(profInUse) && !avoidPDFXOutputIntentProf)
					{
						PdfICCD dataD;
						PdfId embeddedProfile = writer.newObject();
						writer.startObj(embeddedProfile);
						PutDoc("<<\n");
						if ((Options.CompressMethod != PDFOptions::Compression_None) && Options.Compress)
						{
							QByteArray compData = CompressArray(dataP);
							if (compData.size() > 0)
							{
								PutDoc("/Filter /FlateDecode\n");
								dataP = compData;
							}
						}
						PutDoc("/Length "+Pdf::toPdf(dataP.size()+1)+"\n");
						PutDoc("/N "+Pdf::toPdf(components)+"\n");
						PutDoc(">>\nstream\n");
						EncodeArrayToStream(dataP, embeddedProfile);
						PutDoc("\nendstream");
						writer.endObj(embeddedProfile);
						PdfId profileResource = writer.newObject();
						writer.startObj(profileResource);
						dataD.ResName = ResNam+Pdf::toPdf(ResCount);
						dataD.ICCArray = "[ /ICCBased "+Pdf::toPdf(embeddedProfile)+" 0 R ]";
						dataD.ResNum = profileResource;
						dataD.components = components;
						ICCProfiles[profInUse] = dataD;
						PutDoc("[ /ICCBased "+Pdf::toPdf(embeddedProfile)+" 0 R ]");
						writer.endObj(profileResource);
						ResCount++;
					}
					if (components == 1)
						hasGrayProfile = true;
				}
				else
				{
					if (ICCProfiles[Profil].components == 1)
					{
						if ((img.imgInfo.colorspace == ColorSpaceGray) && (hasColorEffect))
						{
							profInUse = c->doc()->cmsSettings().DefaultImageRGBProfile;
							if (!ICCProfiles.contains(profInUse))
							{
								int components = 3;
								PdfId embeddedProfile = writer.newObject();
								writer.startObj(embeddedProfile);
								QByteArray dataP;
								PdfICCD dataD;
								loadRawBytes(ScCore->InputProfiles[c->doc()->cmsSettings().DefaultImageRGBProfile], dataP);
								components = 3;
								PutDoc("<<\n");
								if ((Options.CompressMethod != PDFOptions::Compression_None) && Options.Compress)
								{
									QByteArray compData = CompressArray(dataP);
									if (compData.size() > 0)
									{
										PutDoc("/Filter /FlateDecode\n");
										dataP = compData;
									}
								}
								PutDoc("/Length "+Pdf::toPdf(dataP.size()+1)+"\n");
								PutDoc("/N "+Pdf::toPdf(components)+"\n");
								PutDoc(">>\nstream\n");
								EncodeArrayToStream(dataP, embeddedProfile);
								PutDoc("\nendstream");
								writer.endObj(embeddedProfile);
								PdfId profileResource = writer.newObject();
								writer.startObj(profileResource);
								dataD.ResName = ResNam+Pdf::toPdf(ResCount);
								dataD.ICCArray = "[ /ICCBased "+Pdf::toPdf(embeddedProfile)+" 0 R ]";
								dataD.ResNum = profileResource;
								dataD.components = components;
								ICCProfiles[profInUse] = dataD;
								PutDoc("[ /ICCBased "+Pdf::toPdf(embeddedProfile)+" 0 R ]");
								writer.endObj(profileResource);
								ResCount++;
							}
						}
						else
							hasGrayProfile = true;
					}
				}
			}
			QByteArray im2;
			ScImage img2;
			img2.imgInfo.clipPath = "";
			img2.imgInfo.PDSpathData.clear();
			img2.imgInfo.layerInfo.clear();
			img2.imgInfo.RequestProps = c->pixm.imgInfo.RequestProps;
			img2.imgInfo.isRequest = c->pixm.imgInfo.isRequest;
			if (c->pixm.imgInfo.type == ImageType7)
				alphaM = false;
			else
			{
				bool gotAlpha = false;
				bool pdfVer14 = (Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4);
				gotAlpha = img2.getAlpha(fn, c->pixm.imgInfo.actualPageNumber, im2, true, pdfVer14, afl, img.width(), img.height());
				if (!gotAlpha)
				{
					PDF_Error_MaskLoadFailure(fn);
					return false;
				}
				alphaM = !im2.isEmpty();
			}
			bool imgE = false;
			if ((Options.UseRGB) || (Options.isGrayscale))
				imgE = false;
			else
			{
				if ((Options.UseProfiles2) && (img.imgInfo.colorspace != ColorSpaceCMYK))
					imgE = false;
				else
					imgE = true;
			}
			origWidth = img.width();
			origHeight = img.height();
			img.applyEffect(c->effectsInUse, c->doc()->PageColors, imgE);
			if (!((Options.RecalcPic) && (Options.PicRes < (qMax(72.0 / c->imageXScale(), 72.0 / c->imageYScale())))))
			{
				ImInfo.sxa = sx * (1.0 / ImInfo.reso);
				ImInfo.sya = sy * (1.0 / ImInfo.reso);
			}
			PdfId maskObj = 0;
			if (alphaM)
			{
				bool compAlphaAvail = false;
				maskObj = writer.newObject();
				writer.startObj(maskObj);
				PutDoc("<<\n/Type /XObject\n/Subtype /Image\n");
				if (Options.CompressMethod != PDFOptions::Compression_None)
				{
					QByteArray compAlpha = CompressArray(im2);
					if (compAlpha.size() > 0)
					{
						im2 = compAlpha;
						compAlphaAvail = true;
					}
				}
				if ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4))
				{
					PutDoc("/Width "+Pdf::toPdf(origWidth)+"\n");
					PutDoc("/Height "+Pdf::toPdf(origHeight)+"\n");
					PutDoc("/ColorSpace /DeviceGray\n");
					PutDoc("/BitsPerComponent 8\n");
					PutDoc("/Length "+Pdf::toPdf(im2.size())+"\n");
				}
				else
				{
					PutDoc("/Width "+Pdf::toPdf(origWidth)+"\n");
					PutDoc("/Height "+Pdf::toPdf(origHeight)+"\n");
					PutDoc("/ImageMask true\n/BitsPerComponent 1\n");
					PutDoc("/Length "+Pdf::toPdf(im2.size())+"\n");
				}
				if ((Options.CompressMethod != PDFOptions::Compression_None) && compAlphaAvail)
					PutDoc("/Filter /FlateDecode\n");
				PutDoc(">>\nstream\n");
				EncodeArrayToStream(im2, maskObj);
				PutDoc("\nendstream");
				writer.endObj(maskObj);
				pageData.ImgObjects[ResNam+"I"+Pdf::toPdf(ResCount)] = maskObj;
				ResCount++;
			}
			PdfId imageObj = writer.newObject();
			writer.startObj(imageObj);
			PutDoc("<<\n/Type /XObject\n/Subtype /Image\n");
			PutDoc("/Width "+Pdf::toPdf(img.width())+"\n");
			PutDoc("/Height "+Pdf::toPdf(img.height())+"\n");
			enum PDFOptions::PDFCompression compress_method = Options.CompressMethod;
 			enum PDFOptions::PDFCompression cm = Options.CompressMethod;
			bool exportToCMYK = false, exportToGrayscale = false, jpegUseOriginal = false;
			if (!Options.UseRGB && !(doc.HasCMS && Options.UseProfiles2 && !realCMYK))
			{
				exportToGrayscale = Options.isGrayscale;
				if (exportToGrayscale)
					exportToCMYK      = !Options.isGrayscale;
				else
					exportToCMYK      = !Options.UseRGB;
			}
			if (c->OverrideCompressionMethod)
				compress_method = cm = (enum PDFOptions::PDFCompression) c->CompressionMethodIndex;
			if (img.imgInfo.colorspace == ColorSpaceMonochrome && (c->effectsInUse.count() == 0))
			{
				compress_method = (compress_method != PDFOptions::Compression_None) ? PDFOptions::Compression_ZIP : compress_method;
				cm = compress_method;
			}
			if (extensionIndicatesJPEG(ext) && (cm != PDFOptions::Compression_None))
			{
				if (((Options.UseRGB || Options.UseProfiles2) && (cm == PDFOptions::Compression_Auto) && (c->effectsInUse.count() == 0) && (img.imgInfo.colorspace == ColorSpaceRGB)) && (!img.imgInfo.progressive) && (!((Options.RecalcPic) && (Options.PicRes < (qMax(72.0 / c->imageXScale(), 72.0 / c->imageYScale()))))))
				{
					// #12961 : we must not rely on PDF viewers taking exif infos into account
					// So if JPEG orientation is non default, do not use the original file
					jpegUseOriginal = (img.imgInfo.exifInfo.orientation == 1);
					cm = PDFOptions::Compression_JPEG;
				}
				// We can't unfortunately use directly cmyk jpeg files. Otherwise we have to use the /Decode argument in image
				// dictionary, which we do not quite want as this argument is simply ignored by some rips and software
				// amongst which photoshop and illustrator
				/*else if (((!Options.UseRGB) && (!Options.isGrayscale) && (!Options.UseProfiles2)) && (cm== 0) && (c->effectsInUse.count() == 0) && (img.imgInfo.colorspace == ColorSpaceCMYK) && (!((Options.RecalcPic) && (Options.PicRes < (qMax(72.0 / c->imageXScale(), 72.0 / c->imageYScale()))))) && (!img.imgInfo.progressive))
				{
					jpegUseOriginal = false;
					exportToCMYK = true;
					cm = PDFOptions::Compression_JPEG;
				}*/
				else
				{
					if (compress_method == PDFOptions::Compression_JPEG)
					{
						if (realCMYK || !((Options.UseRGB) || (Options.UseProfiles2)))
						{
							exportToGrayscale = Options.isGrayscale;
							if (exportToGrayscale)
								exportToCMYK      = !Options.isGrayscale;
							else
								exportToCMYK      = !Options.UseRGB;
						}
						cm = PDFOptions::Compression_JPEG;
					}
					else
						cm = PDFOptions::Compression_ZIP;
				}
			}
			else
			{
				if ((compress_method == PDFOptions::Compression_JPEG) || (compress_method == PDFOptions::Compression_Auto))
				{
					if (realCMYK || !((Options.UseRGB) || (Options.UseProfiles2)))
					{
						exportToGrayscale = Options.isGrayscale;
						if (exportToGrayscale)
							exportToCMYK      = !Options.isGrayscale;
						else
							exportToCMYK      = !Options.UseRGB;
					}
					cm = PDFOptions::Compression_JPEG;
					/*if (compress_method == PDFOptions::Compression_Auto)
					{
						QFileInfo fi(tmpFile);
						if (fi.size() < im.size())
						{
							im.resize(0);
							if (!loadRawBytes(tmpFile, im))
								return false;
							cm = PDFOptions::Compression_JPEG;
						}
						else
							cm = PDFOptions::Compression_ZIP;
					}*/
				}
			}
			if ((hasGrayProfile) && (doc.HasCMS) && (Options.UseProfiles2) && (!hasColorEffect))
				exportToGrayscale = true;
			int bytesWritten = 0;
			// Fixme: outType variable should be set directly in the if/else maze above.
			ColorSpaceEnum outType;
			if (img.imgInfo.colorspace == ColorSpaceMonochrome && c->effectsInUse.count() == 0)
				outType = ColorSpaceMonochrome;
			else
				outType = getOutputType(exportToGrayscale, exportToCMYK);
			if ((outType != ColorSpaceMonochrome) && (doc.HasCMS) && (Options.UseProfiles2) && (!avoidPDFXOutputIntentProf))
			{
				PutDoc("/ColorSpace "+ICCProfiles[profInUse].ICCArray+"\n");
				PutDoc("/Intent /");
				int inte2 = Intent;
				if (Options.EmbeddedI)
					inte2 = Options.Intent2;
				static const QByteArray cmsmode[] = {"Perceptual", "RelativeColorimetric", "Saturation", "AbsoluteColorimetric"};
				PutDoc(cmsmode[inte2] + "\n");
			}
			else
			{
				switch (outType)
				{
					case ColorSpaceMonochrome :
					case ColorSpaceGray : PutDoc("/ColorSpace /DeviceGray\n"); break;
					case ColorSpaceCMYK : PutDoc("/ColorSpace /DeviceCMYK\n"); break;
					default : PutDoc("/ColorSpace /DeviceRGB\n"); break;
				}
			}
			if (outType == ColorSpaceMonochrome)
				PutDoc("/BitsPerComponent 1\n");
			else
				PutDoc("/BitsPerComponent 8\n");
			PdfId lengthObj = writer.newObject();
			PutDoc("/Length "+Pdf::toPdf(lengthObj)+" 0 R\n");
			if (cm == PDFOptions::Compression_JPEG)
				PutDoc("/Filter /DCTDecode\n");
			else if (cm != PDFOptions::Compression_None)
				PutDoc("/Filter /FlateDecode\n");
//			if (exportToCMYK && (cm == PDFOptions::Compression_JPEG))
//				PutDoc("/Decode [1 0 1 0 1 0 1 0]\n");
			if (alphaM)
			{
				if ((Options.Version >= PDFOptions::PDFVersion_14) || (Options.Version == PDFOptions::PDFVersion_X4))
					PutDoc("/SMask "+Pdf::toPdf(maskObj)+" 0 R\n");
				else
					PutDoc("/Mask "+Pdf::toPdf(maskObj)+" 0 R\n");
			}
			PutDoc(">>\nstream\n");
			if (cm == PDFOptions::Compression_JPEG) // Fixme: should not do this with monochrome images?
			{
				int quality = c->OverrideCompressionQuality ? c->CompressionQualityIndex : Options.Quality;
				if (c->OverrideCompressionQuality)
					jpegUseOriginal = false;
				bytesWritten = WriteJPEGImageToStream(img, fn, imageObj, quality, outType, jpegUseOriginal, (!hasColorEffect && hasGrayProfile));
			}
			else if (cm == PDFOptions::Compression_ZIP)
				bytesWritten = WriteFlateImageToStream(img, imageObj, outType, (!hasColorEffect && hasGrayProfile));
			else
				bytesWritten = WriteImageToStream(img, imageObj, outType, (!hasColorEffect && hasGrayProfile));
			PutDoc("\nendstream");
			writer.endObj(imageObj);
			if (bytesWritten <= 0)
			{
				PDF_Error_ImageWriteFailure(fn);
				return false;
			}
			writer.startObj(lengthObj);
			PutDoc("    " + Pdf::toPdf(bytesWritten));
			writer.endObj(lengthObj);
			pageData.ImgObjects[ResNam+"I"+Pdf::toPdf(ResCount)] = imageObj;
			ImInfo.ResNum = ResCount;
			ImInfo.Width = img.width();
			ImInfo.Height = img.height();
			ImInfo.xa = sx;
			ImInfo.ya = sy;
			ImInfo.RequestProps = c->pixm.imgInfo.RequestProps;
		} // not embedded PDF
		if ((c->effectsInUse.count() == 0) && (!SharedImages.contains(fn)))
			SharedImages.insert(fn, ImInfo);
		ResCount++;
	}
	else
	{
		ImInfo = SharedImages[fn];
		ImInfo.sxa *= sx / ImInfo.xa;
		ImInfo.sya *= sy / ImInfo.ya;
		/*
		ImRes = SharedImages[fn].ResNum;
		ImWid = SharedImages[fn].Width;
		ImHei = SharedImages[fn].Height;
		aufl = SharedImages[fn].reso;
		sxn = SharedImages[fn].sxa * sx / SharedImages[fn].xa;
		syn = SharedImages[fn].sya * sy / SharedImages[fn].ya;
		*/
	}
	QByteArray embedPre = "";
	if ((bitmapFromGS) || (isEmbeddedPDF)) // compensate gsResolution setting
	{
		if (isEmbeddedPDF)
		{
			// #9268 : per specs default color space is grayscale
			/*if (Options.isGrayscale)
				embedPre = "0 g 0 G";
			else if (Options.UseRGB)
				embedPre = "0 0 0 rg 0 0 0 RG";
			else
				embedPre = "0 0 0 0 k 0 0 0 0 K";*/
			embedPre  = "0 g 0 G";
			embedPre += " 1 w 0 J 0 j [] 0 d\n"; // add default graphics stack parameters pdftex relies on them
		}
		if (c->asLatexFrame())
		{
			ImInfo.sxa *= 1.0 / c->imageXScale();
			ImInfo.sya *= 1.0 / c->imageYScale();
		}
		else
		{
			ImInfo.sxa *= PrefsManager::instance()->appPrefs.extToolPrefs.gs_Resolution / 72.0;
			ImInfo.sya *= PrefsManager::instance()->appPrefs.extToolPrefs.gs_Resolution / 72.0;
		}
	}
	if (!fromAN && output)
	{
		if (c->imageRotation())
		{
			embedPre += "1 0 0 1 "+FToStr(x*sx)+" "+FToStr((-ImInfo.Height*ImInfo.sya+y*sy))+" cm\n";
			QTransform mpa;
			mpa.rotate(-c->imageRotation());
			embedPre += "1 0 0 1 0 "+FToStr(ImInfo.Height*ImInfo.sya)+" cm\n";
			embedPre += FToStr(mpa.m11())+" "+FToStr(mpa.m12())+" "+FToStr(mpa.m21())+" "+FToStr(mpa.m22())+" 0 0 cm\n";
			embedPre += "1 0 0 1 0 "+FToStr(-ImInfo.Height*ImInfo.sya)+" cm\n";
			embedPre += FToStr(ImInfo.Width*ImInfo.sxa)+" 0 0 "+FToStr(ImInfo.Height*ImInfo.sya)+" 0 0 cm\n";
		}
		else
			embedPre += FToStr(ImInfo.Width*ImInfo.sxa)+" 0 0 "+FToStr(ImInfo.Height*ImInfo.sya)+" "+FToStr(x*sx)+" "+FToStr((-ImInfo.Height*ImInfo.sya+y*sy))+" cm\n";
		*output = embedPre + Pdf::toName(ResNam+"I"+Pdf::toPdf(ImInfo.ResNum)) + " Do\n";
//		*output = QString(embedPre + FToStr(ImInfo.Width*ImInfo.sxa)+" 0 0 "+FToStr(ImInfo.Height*ImInfo.sya)+" "+FToStr(x*sx)+" "+FToStr((-ImInfo.Height*ImInfo.sya+y*sy))+" cm\n/"+ResNam+"I"+Pdf::toPdf(ImInfo.ResNum)+" Do\n");
	}
	else if (output)
		*output = QByteArray("");
	return true;
}

bool PDFLibCore::PDF_End_Doc(const QString& PrintPr, const QString& Name, int Components)
{
	PDF_End_Bookmarks();
	PDF_End_Resources();
	PDF_End_Outlines();
	PDF_End_PageTree();
	PDF_End_NamedDests();
	PDF_End_FormObjects();
	PDF_End_JavaScripts();
	PDF_End_Articles();
	PDF_End_Layers();
	PDF_End_OutputProfile(PrintPr, Name, Components);
	PDF_End_Metadata();
	return PDF_End_XRefAndTrailer();
}

void PDFLibCore::PDF_End_Bookmarks()
{
	if (writer.OutlinesObj == 0)
		return;
	
	QByteArray Inhal;
	QMap<int, QByteArray> Inha;
	if ((Bvie->topLevelItemCount() != 0) && (Options.Bookmarks) && (BookMinUse))
	{
		BookMItem* ip = (BookMItem*) Bvie->topLevelItem(0);
		PdfId Basis = writer.objectCounter() - 1;
		Outlines.Count = Bvie->topLevelItemCount();
		Outlines.First = ip->ItemNr + Basis;
		Outlines.Last  = ((BookMItem*) Bvie->topLevelItem(Outlines.Count - 1))->ItemNr + Basis;
		QTreeWidgetItemIterator it(Bvie);
		while (*it)
		{
			ip = (BookMItem*)(*it);
			QString encText = ip->text(0);
			Inhal.clear();
			Inhal += "<<\n/Title " + EncStringUTF16(encText, ip->ItemNr+Basis) + "\n";
			if (ip->Pare == 0)
				Inhal += "/Parent 3 0 R\n";
			else
				Inhal += "/Parent "+Pdf::toPdf(ip->Pare+Basis)+" 0 R\n";
			if (ip->Prev != 0)
				Inhal += "/Prev "+Pdf::toPdf(ip->Prev+Basis)+" 0 R\n";
			if (ip->Next != 0)
				Inhal += "/Next "+Pdf::toPdf(ip->Next+Basis)+" 0 R\n";
			if (ip->First != 0)
				Inhal += "/First "+Pdf::toPdf(ip->First+Basis)+" 0 R\n";
			if (ip->Last != 0)
				Inhal += "/Last "+Pdf::toPdf(ip->Last+Basis)+" 0 R\n";
			if (ip->childCount())
				Inhal += "/Count -"+Pdf::toPdf(ip->childCount())+"\n";
			if ((ip->PageObject->OwnPage != -1) && PageTree.KidsMap.contains(ip->PageObject->OwnPage))
			{
				QByteArray action = Pdf::toPdfDocEncoding(ip->Action);
				if (action.isEmpty())
				{
					const ScPage* page = doc.DocPages.at(ip->PageObject->OwnPage);
					double actionPos = page->height() - (ip->PageObject->yPos() - page->yOffset());
					action = "/XYZ 0 " + Pdf::toPdf(actionPos) + " 0";
				}
				Inhal += "/Dest ["+Pdf::toPdf(PageTree.KidsMap[ip->PageObject->OwnPage])+" 0 R "+action+"]\n";
			}
			Inhal += ">>";
			Inha[ip->ItemNr] = Inhal;
			++it;
		}
		QMap<int,QByteArray> ::ConstIterator contentIt;
		writer.reserveObjects(Inha.count());
		for (contentIt = Inha.begin(); contentIt != Inha.end(); ++contentIt)
		{
			int itemNr = contentIt.key();
			writer.startObj(itemNr + Basis);
			PutDoc(contentIt.value());
			writer.endObj(itemNr + Basis);
		}
	}
}

void PDFLibCore::PDF_End_Resources()
{
	writer.ResourcesObj = writer.newObject();
	writer.startObj(writer.ResourcesObj);
	
	Pdf::ResourceDictionary dict;
	dict.XObject.unite(pageData.ImgObjects);
	dict.XObject.unite(pageData.XObjects);
	dict.Font = pageData.FObjects;
	dict.Shading = Shadings;
	dict.Pattern = Patterns;
	dict.ExtGState = Transpar;
	dict.ColorSpace.append(asColorSpace(ICCProfiles.values()));
	dict.ColorSpace.append(asColorSpace(spotMap.values()));
	dict.ColorSpace.append(asColorSpace(spotMapReg.values()));

	dict.Properties.clear();
	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4))&& (Options.useLayers))
	{
		ScLayer ll;
		ll.isPrintable = false;
		ll.ID = 0;
		int Lnr = 0;
		for (int la = 0; la < doc.Layers.count(); ++la)
		{
			doc.Layers.levelToLayer(ll, la);
			dict.Properties[OCGEntries[ll.Name].Name] = OCGEntries[ll.Name].ObjNum;
//			PutDoc("/"+OCGEntries[ll.Name].Name + " " + Pdf::toObjRef(OCGEntries[ll.Name].ObjNum) + "\n");
			Lnr++;
		}
	}

	writer.write(dict);
	writer.endObj(writer.ResourcesObj);
}

void PDFLibCore::PDF_End_Outlines()
{
	if (writer.OutlinesObj == 0)
		return;

	writer.startObj(writer.OutlinesObj);
	PutDoc("<<\n/Type /Outlines\n");
	PutDoc("/Count "+Pdf::toPdf(Outlines.Count)+"\n");
	if ((Bvie->topLevelItemCount() != 0) && (Options.Bookmarks))
	{
		PutDoc("/First "+Pdf::toPdf(Outlines.First)+" 0 R\n");
		PutDoc("/Last "+Pdf::toPdf(Outlines.Last)+" 0 R\n");
	}
	PutDoc(">>");
	writer.endObj(writer.OutlinesObj);
}

void PDFLibCore::PDF_End_PageTree()
{
	writer.startObj(writer.PagesObj);
	PutDoc("<<\n/Type /Pages\n/Kids [");
	for (int i = 0; i < PageTree.Kids.count(); ++i)
	{
		PdfId objId = PageTree.Kids.at(i);
		PutDoc(Pdf::toPdf(objId) + " 0 R ");
	}
	PutDoc("]\n");
	PutDoc("/Count "+ Pdf::toPdf(PageTree.Kids.count()) + "\n");
	PutDoc("/Resources "+Pdf::toPdf(writer.ResourcesObj)+" 0 R\n");
	PutDoc(">>");
	writer.endObj(writer.PagesObj);
}

void PDFLibCore::PDF_End_NamedDests()
{
	if (writer.DestsObj == 0)
		return;

	writer.startObj(writer.DestsObj);
	PutDoc("<<\n");
	if (NamedDest.count() != 0)
	{
		QList<PdfDest>::Iterator vt;
		for (vt = NamedDest.begin(); vt != NamedDest.end(); ++vt)
		{
			if (PageTree.KidsMap.contains(vt->PageNr))
				PutDoc(Pdf::toName(vt->Name) + " [" + Pdf::toObjRef(PageTree.KidsMap[vt->PageNr])
				       + " /XYZ " + Pdf::toPdfDocEncoding(vt->Act) + "]\n");
		}
	}
	PutDoc(">>");
	writer.endObj(writer.DestsObj);
}

void PDFLibCore::PDF_End_FormObjects()
{
	if (writer.AcroFormObj == 0)
		return;

	writer.startObj(writer.AcroFormObj);
	PutDoc("<<\n");
	PutDoc("/Fields [ ");
	if (pageData.FormObjects.count() != 0)
	{
		for (int fo = 0; fo < pageData.FormObjects.count(); ++fo)
			PutDoc(Pdf::toObjRef(pageData.FormObjects[fo])+" ");
	}
	PutDoc("]\n");
	if (CalcFields.count() != 0)
	{
		PutDoc("/CO [ ");
		for (int foc = 0; foc < CalcFields.count(); ++foc)
			PutDoc(Pdf::toObjRef(CalcFields[foc]) + " ");
		PutDoc("]\n");
	}
	if ((pageData.FormObjects.count() != 0) || (CalcFields.count() != 0))
		PutDoc("/NeedAppearances true\n/DR " + Pdf::toObjRef(writer.ResourcesObj) + "\n");
	PutDoc(">>");
	writer.endObj(writer.AcroFormObj);
}

void PDFLibCore::PDF_End_JavaScripts()
{
	if (writer.OpenActionObj != 0)
		WritePDFString("this." + Options.openAction + "()", writer.OpenActionObj);

	if (writer.NamesObj == 0)
		return;

	PdfId jsNameTreeObj = 0;
	if (doc.JavaScripts.count() != 0)
	{
		QList<PdfId> Fjav;
		QMap<QString,QString>::Iterator itja0;
		for (itja0 = doc.JavaScripts.begin(); itja0 != doc.JavaScripts.end(); ++itja0)
			Fjav.append(WritePDFString(itja0.value()));
		QMap<QString,QString>::Iterator itja;
		int i = 0;
		for (itja = doc.JavaScripts.begin(); itja != doc.JavaScripts.end(); ++itja)
		{
			PdfId Fjav1 = writer.startObj();
			Fjav.append(Fjav1);
			PutDoc("<< /S /JavaScript /JS "+Pdf::toPdf(Fjav[i])+" 0 R >>");
			writer.endObj(Fjav1);
			++i;
		}
		jsNameTreeObj = writer.startObj();
		PutDoc("<< /Names [ ");
		QMap<QString,QString>::Iterator itja2;
		for (itja2 = doc.JavaScripts.begin(); itja2 != doc.JavaScripts.end(); ++itja2)
		{
			PutDoc(EncString(Pdf::toPdfDocEncoding(itja2.key()), jsNameTreeObj) + " " + Pdf::toObjRef(Fjav[i]) + " ");
			++i;
		}
		PutDoc("] >>");
		writer.endObj(jsNameTreeObj);
	}

	writer.startObj(writer.NamesObj);
	PutDoc("<< ");
	if (doc.JavaScripts.count() != 0)
		PutDoc("/JavaScript "+Pdf::toPdf(jsNameTreeObj)+" 0 R");
	PutDoc(" >>");
	writer.endObj(writer.NamesObj);
}

void PDFLibCore::PDF_End_Articles()
{
	if (writer.ThreadsObj == 0)
		return;

	QList<PdfId> Threads;
	QList<PdfBead> Beads;
	PdfId currentThreadObj = 0;

	for (int ele = 0; ele < doc.Items->count(); ++ele)
	{
		PageItem* tel = doc.Items->at(ele);
		if ((tel->asTextFrame()) && (tel->prevInChain() == 0) && (tel->nextInChain() != 0) &&
				(!tel->inPdfArticle))
		{
			Beads.clear();
			PdfBead bd;
			if (currentThreadObj == 0)
				currentThreadObj = writer.newObject();
				
			PdfId fir = currentThreadObj + 1;
			PdfId ccb = currentThreadObj + 1;
			bd.Parent = currentThreadObj;
			while (tel->nextInChain() != 0)
			{
				if ((tel->OwnPage != -1) && PageTree.KidsMap.contains(tel->OwnPage))
				{
					bd.Next = ccb + 1;
					bd.Prev = ccb - 1;
					ccb++;
					bd.Page = PageTree.KidsMap[tel->OwnPage];
					bd.Rect = QRect(static_cast<int>(tel->xPos() - doc.DocPages.at(tel->OwnPage)->xOffset()),
								static_cast<int>(doc.DocPages.at(tel->OwnPage)->height() - (tel->yPos()  - doc.DocPages.at(tel->OwnPage)->yOffset())),
								static_cast<int>(tel->width()),
								static_cast<int>(tel->height()));
					Beads.append(bd);
				}
				tel->inPdfArticle = true;
				tel = tel->nextInChain();
			}
			bd.Next = ccb + 1;
			bd.Prev = ccb - 1;
			if ((tel->OwnPage != -1) && PageTree.KidsMap.contains(tel->OwnPage))
			{
				bd.Page = PageTree.KidsMap[tel->OwnPage];
				bd.Rect = QRect(static_cast<int>(tel->xPos() - doc.DocPages.at(tel->OwnPage)->xOffset()),
							static_cast<int>(doc.DocPages.at(tel->OwnPage)->height() - (tel->yPos()  - doc.DocPages.at(tel->OwnPage)->yOffset())),
							static_cast<int>(tel->width()),
							static_cast<int>(tel->height()));
				Beads.append(bd);
			}
			tel->inPdfArticle = true;
			if (Beads.count() > 0)
			{
				writer.startObj(currentThreadObj);
				Threads.append(currentThreadObj);
				PutDoc("<< /Type /Thread\n");
				PutDoc("   /F "+Pdf::toPdf(fir)+" 0 R\n");
				PutDoc(">>");
				writer.endObj(currentThreadObj);
					
				Beads[0].Prev = fir + Beads.count()-1;
				Beads.last().Next = fir;
				
				for (int beac = 0; beac < Beads.count(); ++beac)
				{
					PdfId beadObj = writer.startObj();
					PutDoc("<< /Type /Bead\n");
					PutDoc("   /T "+Pdf::toPdf(Beads[beac].Parent)+" 0 R\n");
					PutDoc("   /N "+Pdf::toPdf(Beads[beac].Next)+" 0 R\n");
					PutDoc("   /V "+Pdf::toPdf(Beads[beac].Prev)+" 0 R\n");
					PutDoc("   /P "+Pdf::toPdf(Beads[beac].Page)+" 0 R\n");
					PutDoc("   /R [ "+Pdf::toPdf(Beads[beac].Rect.x())+" "+
						    Pdf::toPdf(Beads[beac].Rect.y())+" ");
					PutDoc(Pdf::toPdf(Beads[beac].Rect.bottomRight().x())+" "+Pdf::toPdf(Beads[beac].Rect.y()-Beads[beac].Rect.height())+" ]\n");
					PutDoc(">>");
					writer.endObj(beadObj);
				}
				currentThreadObj = 0;
			}
		}
	}
	for (int ele = 0; ele < doc.Items->count(); ++ele)
		doc.Items->at(ele)->inPdfArticle = false;

	writer.startObj(writer.ThreadsObj);
	PutDoc("[ ");
	for (int th = 0; th < Threads.count(); ++th)
		PutDoc(Pdf::toObjRef(Threads[th])+" ");
	PutDoc("]");
	writer.endObj(writer.ThreadsObj);
}

void PDFLibCore::PDF_End_Layers()
{
	if (((Options.Version == PDFOptions::PDFVersion_15) || (Options.Version == PDFOptions::PDFVersion_X4)) && (Options.useLayers))
	{
		QList<QByteArray> lay;
		writer.startObj(writer.OCPropertiesObj);
		PutDoc("<<\n");
		PutDoc("/D << /Order [ ");
		ScLayer ll;
		ll.isPrintable = false;
		ll.ID = 0;
		int Lnr = 0;
		for (int la = 0; la < doc.Layers.count(); ++la)
		{
			doc.Layers.levelToLayer(ll, la);
			if (ll.isEditable)
				lay.prepend(Pdf::toObjRef(OCGEntries[ll.Name].ObjNum)+" ");
			Lnr++;
		}
		for (int layc = 0; layc < lay.count(); ++layc)
		{
			if (Options.Version != PDFOptions::PDFVersion_X4)
				PutDoc(lay[layc]);
		}
		PutDoc("]\n");
		if (Options.Version == PDFOptions::PDFVersion_X4)
		{
			PutDoc("/BaseState /ON\n");
			QString occdName = "Default";
			PutDoc("/Name "+Pdf::toLiteralString(occdName)+"\n");
		}
		PutDoc("/OFF [ ");
		QHash<QString, PdfOCGInfo>::Iterator itoc;
		for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
		{
			if (!itoc.value().visible)
				PutDoc(Pdf::toObjRef(itoc.value().ObjNum) + " ");
		}
		PutDoc("]\n");
		if (Options.Version != PDFOptions::PDFVersion_X4)
		{
			PutDoc("/AS [<</Event /Print /OCGs [ ");
			for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
			{
				PutDoc(Pdf::toObjRef(itoc.value().ObjNum) + " ");
			}
			PutDoc("] /Category [/Print]>> <</Event /View /OCGs [");
			for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
			{
				PutDoc(Pdf::toObjRef(itoc.value().ObjNum) + " ");
			}
			PutDoc("] /Category [/View]>>]\n");
		}
		PutDoc(">>\n");
		PutDoc("/OCGs [ ");
		for (itoc = OCGEntries.begin(); itoc != OCGEntries.end(); ++itoc)
		{
			PutDoc(Pdf::toObjRef(itoc.value().ObjNum)+" ");
		}
		PutDoc("]\n");
		PutDoc(">>");
		writer.endObj(writer.OCPropertiesObj);
	}
}

void PDFLibCore::PDF_End_OutputProfile(const QString& PrintPr, const QString& Name, int Components)
{
	if (PDF_IsPDFX())
	{
		PdfId profileObj =writer.startObj();
		QByteArray dataP;
		loadRawBytes(PrintPr, dataP);
		PutDoc("<<\n");
		if (Options.Compress)
		{
			QByteArray compData = CompressArray(dataP);
			if (compData.size() > 0)
			{
				PutDoc("/Filter /FlateDecode\n");
				dataP = compData;
			}
		}
		PutDoc("/Length "+Pdf::toPdf(dataP.size()+1)+"\n");
		PutDoc("/N "+Pdf::toPdf(Components)+"\n");
		PutDoc(">>\nstream\n");
		PutDoc(dataP);
		PutDoc("\nendstream");
		writer.endObj(profileObj);

//		if ((Options.Version == PDFOptions::PDFVersion_X4) && (Options.useLayers))
//		{
//			XRef[9] = bytesWritten();
//			PutDoc("10 0 obj\n");
//		}
//		if ((Options.Version == PDFOptions::PDFVersion_X3) || (Options.Version == PDFOptions::PDFVersion_X1a) || ((Options.Version == PDFOptions::PDFVersion_X4) && !(Options.useLayers)))
//		{
//			XRef[8] = bytesWritten();
//			PutDoc("9 0 obj\n");
//		}
		
		writer.startObj(writer.OutputIntentObj);

		PutDoc("<<\n/Type /OutputIntent\n/S /GTS_PDFX\n");
		PutDoc("/DestOutputProfile " + Pdf::toObjRef(profileObj) + "\n");
		PutDoc("/OutputConditionIdentifier (Custom)\n");
		PutDoc("/Info " + Pdf::toLiteralString(Options.Info) + "\n");
		PutDoc("/OutputCondition " + Pdf::toLiteralString(Name) + "\n");
		PutDoc(">>");
		writer.endObj(writer.OutputIntentObj);
	}
}

void PDFLibCore::PDF_End_Metadata()
{
	if (Options.Version == PDFOptions::PDFVersion_X4)
	{
//		if (Options.useLayers) // OCProperties dictionary was included as '9 0 obj', OutputIntents was included as '10 0 obj'
//		{
//			XRef[10] = bytesWritten();
//			PutDoc("11 0 obj\n");
//		}
//		else // There was no OCProperties dictionary
//		{
//			XRef[9] = bytesWritten();
//			PutDoc("10 0 obj\n");
//		}
		writer.startObj(writer.MetaDataObj);
		PutDoc("<<\n");
		PutDoc("/Length "+Pdf::toPdf(xmpPacket.size()+1)+"\n");
		PutDoc("/Type /Metadata\n");
		PutDoc("/Subtype /XML\n");
		PutDoc(">>\nstream\n");
		PutDoc(xmpPacket);
		PutDoc("\nendstream");
		writer.endObj(writer.MetaDataObj);
	}
}

bool PDFLibCore::PDF_End_XRefAndTrailer()
{
	writer.writeXrefAndTrailer();
	return closeAndCleanup();
}

void PDFLibCore::generateXMP(const QString& timeStamp)
{
	/*
		This is a rather 'manual' way to generate XMP. Since only a few basic properties are included here,
		this method tries to build XMP from the generic XML's API of Qt and follow the XMP's spec very closely.
		A better (and less typing) way to support XMP is to use an XMP's API such as Exiv2 or Exempi
	*/
	QDomDocument xmpDoc;
	QDomProcessingInstruction xpacket = xmpDoc.createProcessingInstruction("xpacket", "begin=\"\" id=\"W5M0MpCehiHzreSzNTczkc9d\"");
	xmpDoc.appendChild(xpacket);
	QString xNS = "adobe:ns:meta/";
	QDomElement xmpmeta = xmpDoc.createElementNS(xNS, "x:xmpmeta");
	xmpmeta.setAttributeNS(xNS, "x:xmptk", "Scribus PDF Library "+QString(VERSION));
	xmpDoc.appendChild(xmpmeta);
	QString rdfNS = "http://www.w3.org/1999/02/22-rdf-syntax-ns#";
	QDomElement rdf = xmpDoc.createElementNS(rdfNS, "rdf:RDF");
	xmpmeta.appendChild(rdf);
	QDomElement desc = xmpDoc.createElement("rdf:Description");
	desc.setAttribute("rdf:about", "");

	QDomElement descXMP = desc.cloneNode().toElement();
	rdf.appendChild(descXMP);
	QString xmpNS = "http://ns.adobe.com/xap/1.0/";
	descXMP.setAttributeNS(xmpNS, "xmp:CreateDate", timeStamp);
	descXMP.setAttribute("xmp:ModifyDate", timeStamp);
	descXMP.setAttribute("xmp:MetadataDate", timeStamp);
	descXMP.setAttribute("xmp:CreatorTool", "Scribus "+QString(VERSION));

	QDomElement descPDF = desc.cloneNode().toElement();
	rdf.appendChild(descPDF);
	QString pdfNS = "http://ns.adobe.com/pdf/1.3/";
	descPDF.setAttributeNS(pdfNS, "pdf:Producer", "Scribus PDF Library "+QString(VERSION));
	descPDF.setAttribute("pdf:Trapped", "False");
	descPDF.setAttribute("pdf:Keywords", doc.documentInfo().keywords());

	QDomElement descDC = desc.cloneNode().toElement();
	rdf.appendChild(descDC);
	QString dcNS = "http://purl.org/dc/elements/1.1/";
	descDC.setAttributeNS(dcNS, "dc:format", "application/pdf");
	QDomElement title = xmpDoc.createElement("dc:title");
	descDC.appendChild(title);
	QDomElement alt1 = xmpDoc.createElement("rdf:Alt");
	title.appendChild(alt1);
	QDomElement li1 = xmpDoc.createElement("rdf:li");
	li1.setAttribute("xml:lang", "x-default");
	alt1.appendChild(li1);
	QString docTitle = doc.documentInfo().title();
	if ((PDF_IsPDFX()) && (docTitle.isEmpty()))
		docTitle = doc.DocName;
	li1.appendChild(xmpDoc.createTextNode(docTitle));
	QDomElement creator = xmpDoc.createElement("dc:creator");
	descDC.appendChild(creator);
	QDomElement seq = xmpDoc.createElement("rdf:Seq");
	creator.appendChild(seq);
	QDomElement li2 = xmpDoc.createElement("rdf:li");
	seq.appendChild(li2);
	li2.appendChild(xmpDoc.createTextNode(doc.documentInfo().author()));
	// Subject's entry in Document Info dictionary is actually dc:description in XMP, not dc:subject.
	QDomElement description = xmpDoc.createElement("dc:description");
	descDC.appendChild(description);
	QDomElement alt2 = xmpDoc.createElement("rdf:Alt");
	description.appendChild(alt2);
	QDomElement li3 = xmpDoc.createElement("rdf:li");
	li3.setAttribute("xml:lang", "x-default");
	alt2.appendChild(li3);
	li3.appendChild(xmpDoc.createTextNode(doc.documentInfo().subject()));

	if (PDF_IsPDFX())
	{
		QDomElement descPDFXID = desc.cloneNode().toElement();
		rdf.appendChild(descPDFXID);
		QString pdfxidNS = "http://www.npes.org/pdfx/ns/id/";
		if (Options.Version == PDFOptions::PDFVersion_X1a)
		{
			descPDFXID.setAttributeNS(pdfxidNS, "pdfx:GTS_PDFXConformance", "PDF/X-1a:2001");
			descPDFXID.setAttribute("pdfx:GTS_PDFXVersion", "PDF/X-1:2001");
		}
		else if (Options.Version == PDFOptions::PDFVersion_X3)
			descPDFXID.setAttributeNS(pdfxidNS, "pdfxid:GTS_PDFXVersion", "PDF/X-3");
		else if (Options.Version == PDFOptions::PDFVersion_X4)
			descPDFXID.setAttributeNS(pdfxidNS, "pdfxid:GTS_PDFXVersion", "PDF/X-4");
	}

	QDomElement descXMPMM = desc.cloneNode().toElement();
	rdf.appendChild(descXMPMM);
	QString xmpmmNS = "http://ns.adobe.com/xap/1.0/mm/";
	QString uuid = QUuid::createUuid().toString();
	// remove the enclosing braces
	uuid.remove(0, 1);
	uuid.chop(1);
	descXMPMM.setAttributeNS(xmpmmNS, "xmpMM:DocumentID", "uuid:"+uuid);
	descXMPMM.setAttribute("xmpMM:RenditionClass", "default");
	descXMPMM.setAttribute("xmpMM:VersionID", 1);

	xmpPacket.append(xmpDoc.toByteArray(4));
	QByteArray tenSpaces = "          ";
	for (int i=0; i<25; ++i)
	{
		for (int j=0; j<10; ++j)
			xmpPacket.append(tenSpaces);
		xmpPacket.append("\n");
	}
	xmpPacket.append("<?xpacket end='w'?>");
}

void PDFLibCore::PDF_Error(const QString& errorMsg)
{
	ErrorMessage = errorMsg;
	if (!ScCore->usingGUI())
		qDebug("%s", errorMsg.toLocal8Bit().data());
}

void PDFLibCore::PDF_Error_WriteFailure(void)
{
	PDF_Error( tr("A write error occurred, please check available disk space") );
}

void PDFLibCore::PDF_Error_ImageLoadFailure(const QString& fileName)
{
	PDF_Error( tr("Failed to load an image : %1").arg(fileName) );
}

void PDFLibCore::PDF_Error_ImageWriteFailure(const QString& fileName)
{
	PDF_Error( tr("Failed to write an image : %1").arg(fileName) );
}

void PDFLibCore::PDF_Error_MaskLoadFailure(const QString& fileName)
{
	PDF_Error( tr("Failed to load an image mask : %1").arg(fileName) );
}

void PDFLibCore::PDF_Error_InsufficientMemory(void)
{
	PDF_Error( tr("Insufficient memory for processing an image"));
}

bool PDFLibCore::closeAndCleanup()
{
	bool writeSucceed = writer.close(abortExport);
	if (!writeSucceed)
		PDF_Error_WriteFailure();

	pageData.XObjects.clear();
	pageData.ImgObjects.clear();
	pageData.FObjects.clear();
	pageData.AObjects.clear();
	pageData.FormObjects.clear();
	CalcFields.clear();
	Shadings.clear();
	Transpar.clear();
	ICCProfiles.clear();
	return writeSucceed;
}

void PDFLibCore::cancelRequested()
{
	abortExport=true;
}
