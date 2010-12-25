/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                 	scribusXml.cpp the document xml library for scribus
                             -------------------
    begin                : Sam Jul 14 10:00:00 CEST 2001
    copyright            : (C) 2001 by Christian T�p
    email                : christian.toepp@mr-ct@gmx.de
 ***************************************************************************/

#include "scribusXml.h"

#include <QtAlgorithms>
#include <QApplication>
#include <QByteArray>
#include <QBuffer>
#include <QCursor>
#include <QDir>
#include <QFile>
#include <QRegExp>
#include <QTextCodec>
#include <QTextStream>
#include <QXmlSimpleReader>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

#include <cstdlib>

#include "commonstrings.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_latexframe.h"
#ifdef HAVE_OSG
	#include "pageitem_osgframe.h"
#endif
#include "prefsmanager.h"
#include "scclocale.h"
#include "scribusdoc.h"
#include "selection.h"
#include "text/nlsconfig.h"
#include "units.h"

#include <iostream>

#include "scconfig.h"

#include <zlib.h>
#include "util.h"
#include "util_text.h"
#include "util_math.h"
#include "util_color.h"
#include "sclimits.h"
#include "scpattern.h"
#include "sctextstream.h"
#include "scxmlstreamwriter.h"
#include "scpainter.h"
#include "fileloader.h"
#include "loadsaveplugin.h"
#include "plugins/formatidlist.h"


using namespace std;


ScriXmlDoc::ScriXmlDoc()
{
}
#if 0
void ScriXmlDoc::SetItemProps(ScXmlStreamWriter& writer, ScribusDoc *doc, PageItem* item, const QString& baseDir, bool newFormat)
{
	double xf, yf;
	QString tmp, tmpy;
	if (newFormat)
		writer.writeAttribute("OwnPage", item->OwnPage);
	const ParagraphStyle& defaultStyle = item->itemText.defaultStyle();
	const CharStyle&      charStyle    = item->itemText.defaultStyle().charStyle();
	writer.writeAttribute("PTYPE"    ,item->realItemType());
	writer.writeAttribute("XPOS"     ,item->xPos() - doc->currentPage()->xOffset());
	writer.writeAttribute("YPOS"     ,item->yPos() - doc->currentPage()->yOffset());
	writer.writeAttribute("gXpos"    ,item->gXpos);
	writer.writeAttribute("gYpos"    ,item->gYpos);
	writer.writeAttribute("WIDTH"    ,item->width());
	writer.writeAttribute("HEIGHT"   ,item->height());
	writer.writeAttribute("RADRECT"  ,item->cornerRadius());
	writer.writeAttribute("FRTYPE"   ,item->FrameType);
	writer.writeAttribute("CLIPEDIT" ,item->ClipEdited ? "1" : "0");
	writer.writeAttribute("PWIDTH"   ,item->lineWidth());
	writer.writeAttribute("PCOLOR"   ,item->fillColor());
	writer.writeAttribute("PCOLOR2"  ,item->lineColor());
	writer.writeAttribute("IFONT"    ,charStyle.font().scName());
	writer.writeAttribute("ISIZE"    ,charStyle.fontSize() / 10.0);
	writer.writeAttribute("TXTFILL"  ,charStyle.fillColor());
	writer.writeAttribute("TXTSTROKE",charStyle.strokeColor());
	writer.writeAttribute("TXTSTRSH" ,charStyle.strokeShade());
	writer.writeAttribute("TXTFILLSH",charStyle.fillShade());
	writer.writeAttribute("TXTSCALE" ,charStyle.scaleH() / 10.0);
	writer.writeAttribute("TXTSCALEV",charStyle.scaleV() / 10.0);
	writer.writeAttribute("TXTBASE"  ,charStyle.baselineOffset() / 10.0);
	writer.writeAttribute("TXTSHX"   ,charStyle.shadowXOffset() / 10.0);
	writer.writeAttribute("TXTSHY"   ,charStyle.shadowYOffset() / 10.0);
	writer.writeAttribute("TXTOUT"   ,charStyle.outlineWidth() / 10.0);
	writer.writeAttribute("TXTULP"   ,charStyle.underlineOffset() / 10.0);
	writer.writeAttribute("TXTULW"   ,charStyle.underlineWidth() / 10.0);
	writer.writeAttribute("TXTSTP"   ,charStyle.strikethruOffset() / 10.0);
	writer.writeAttribute("TXTSTW"   ,charStyle.strikethruWidth() / 10.0);
	writer.writeAttribute("TXTKERN"  ,charStyle.tracking());
	writer.writeAttribute("TXTSTYLE" ,charStyle.effects());
	writer.writeAttribute("COLUMNS"  ,item->columns());
	writer.writeAttribute("COLGAP"   ,item->columnGap());
	writer.writeAttribute("NAMEDLST" ,item->NamedLStyle);
	writer.writeAttribute("SHADE"    ,item->fillShade());
	writer.writeAttribute("SHADE2"   ,item->lineShade());
	writer.writeAttribute("GRTYP"    ,item->GrType);
	writer.writeAttribute("GRTYPS"   ,item->GrTypeStroke);
	writer.writeAttribute("GRTYPM"   ,item->GrMask);
	writer.writeAttribute("ROT"      ,item->rotation());
	writer.writeAttribute("PLINEART" ,item->PLineArt);
	writer.writeAttribute("PLINEEND" ,item->PLineEnd);
	writer.writeAttribute("PLINEJOIN",item->PLineJoin);
	writer.writeAttribute("LINESP"   ,defaultStyle.lineSpacing());
	writer.writeAttribute("LINESPMode",defaultStyle.lineSpacingMode());
	writer.writeAttribute("LOCALSCX" ,item->imageXScale());
	writer.writeAttribute("LOCALSCY" ,item->imageYScale());
	writer.writeAttribute("LOCALX"   ,item->imageXOffset());
	writer.writeAttribute("LOCALY"   ,item->imageYOffset());
	writer.writeAttribute("LOCALROT" ,item->imageRotation());
	writer.writeAttribute("PICART"   ,item->imageShown() ? "1" : "0");
	writer.writeAttribute("PLTSHOW"  ,item->PoShow ? "1" : "0");
	writer.writeAttribute("BASEOF"   ,item->BaseOffs);
	writer.writeAttribute("textPathType",item->textPathType);
	writer.writeAttribute("textPathFlipped",item->textPathFlipped ? "1" : "0");
	writer.writeAttribute("FLIPPEDH" , item->imageFlippedH() ? "1" : "0");
	writer.writeAttribute("FLIPPEDV" , item->imageFlippedV() ? "1" : "0");
	writer.writeAttribute("SCALETYPE",item->ScaleType ? "1" : "0");
	writer.writeAttribute("RATIO"    ,item->AspectRatio ? "1" : "0");
	writer.writeAttribute("PRINTABLE",item->printEnabled() ? "1" : "0");
	if(item->isAnnotation())
	{
		writer.writeAttribute("ANNOTATION","1");
		writer.writeAttribute("ANTYPE"  , item->annotation().Type());
		writer.writeAttribute("ANACTION", item->annotation().Action());
		writer.writeAttribute("ANEACT"  , item->annotation().E_act());
		writer.writeAttribute("ANXACT"  , item->annotation().X_act());
		writer.writeAttribute("ANDACT"  , item->annotation().D_act());
		writer.writeAttribute("ANFOACT" , item->annotation().Fo_act());
		writer.writeAttribute("ANBLACT" , item->annotation().Bl_act());
		writer.writeAttribute("ANKACT"  , item->annotation().K_act());
		writer.writeAttribute("ANFACT"  , item->annotation().F_act());
		writer.writeAttribute("ANVACT"  , item->annotation().V_act());
		writer.writeAttribute("ANCACT"  , item->annotation().C_act());
		if (item->annotation().ActionType() == 8)
			writer.writeAttribute("ANEXTERN", item->annotation().Extern());
		else
			writer.writeAttribute("ANEXTERN", Path2Relative(item->annotation().Extern(), baseDir));
		writer.writeAttribute("ANZIEL"  , item->annotation().Ziel());
		writer.writeAttribute("ANACTYP" , item->annotation().ActionType());
		writer.writeAttribute("ANTOOLTIP", item->annotation().ToolTip());
		writer.writeAttribute("ANBWID"  , item->annotation().Bwid());
		writer.writeAttribute("ANBSTY"  , item->annotation().Bsty());
		writer.writeAttribute("ANFEED"  , item->annotation().Feed());
		writer.writeAttribute("ANFLAG"  , item->annotation().Flag());
		writer.writeAttribute("ANFONT"  , item->annotation().Font());
		writer.writeAttribute("ANFORMAT", item->annotation().Format());
		writer.writeAttribute("ANROLL"  , item->annotation().RollOver());
		writer.writeAttribute("ANDOWN"  , item->annotation().Down());
		writer.writeAttribute("ANVIS"   , item->annotation().Vis());
		writer.writeAttribute("ANMC"    , item->annotation().MaxChar());
		writer.writeAttribute("ANCHK"   , item->annotation().IsChk() ? "1" : "0");
		writer.writeAttribute("ANAA"    , item->annotation().AAact() ? "1" : "0");
		writer.writeAttribute("ANCHKS"  , item->annotation().ChkStil());
		writer.writeAttribute("ANBCOL"  , item->annotation().borderColor());
		writer.writeAttribute("ANHTML"  , item->annotation().HTML() ? "1" : "0");
		writer.writeAttribute("ANICON"  , item->annotation().UseIcons() ? "1" : "0");
		writer.writeAttribute("ANPLACE" , item->annotation().IPlace() ? "1" : "0");
		writer.writeAttribute("ANSCALE" , item->annotation().ScaleW() ? "1" : "0");
	}
	else
		writer.writeAttribute("ANNOTATION","0");
	writer.writeAttribute("ANNAME", !item->AutoName ? item->itemName() : QString(""));
	// "TEXTFLOWMODE" succeed to "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes
	writer.writeAttribute("TEXTFLOWMODE", (int) item->textFlowMode() );
	// Set "TEXTFLOW" "TEXTFLOW2" and "TEXTFLOW3" attributes for compatibility
	// with versions prior to 1.3.4
	writer.writeAttribute("TEXTFLOW" , item->textFlowAroundObject()    ? "1" : "0");
	writer.writeAttribute("TEXTFLOW2", item->textFlowUsesBoundingBox() ? "1" : "0");
	writer.writeAttribute("TEXTFLOW3", item->textFlowUsesContourLine() ? "1" : "0");
	writer.writeAttribute("AUTOTEXT" , item->isAutoText ? "1" : "0");
	writer.writeAttribute("EXTRA"    , item->textToFrameDistLeft());
	writer.writeAttribute("TEXTRA"   , item->textToFrameDistTop());
	writer.writeAttribute("BEXTRA"   , item->textToFrameDistBottom());
	writer.writeAttribute("REXTRA"   , item->textToFrameDistRight());
	writer.writeAttribute("FLOP"	,item->firstLineOffset());
#ifdef HAVE_OSG
	if (((item->asImageFrame() && !(item->asLatexFrame() || item->asOSGFrame())) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
#else
	if (((item->asImageFrame() && !(item->asLatexFrame())) || (item->asTextFrame())) && (!item->Pfile.isEmpty()))
#endif
	{
		writer.writeAttribute("ImageRes", item->pixm.imgInfo.lowResType);
		writer.writeAttribute("Pagenumber", item->pixm.imgInfo.actualPageNumber);
		if (item->isInlineImage)
		{
			writer.writeAttribute("PFILE", "");
			writer.writeAttribute("isInlineImage", static_cast<int>(item->isInlineImage));
			QFileInfo inlFi(item->Pfile);
			writer.writeAttribute("inlineImageExt", inlFi.suffix());
			QFile inFil(item->Pfile);
			if (inFil.open(QIODevice::ReadOnly))
			{
				QByteArray ba = qCompress(inFil.readAll()).toBase64();
				writer.writeAttribute("ImageData", QString(ba));
				inFil.close();
			}
		}
		else
			writer.writeAttribute("PFILE",Path2Relative(item->Pfile, baseDir));
	}
#ifdef HAVE_OSG
	else if (item->asOSGFrame())
	{
		if (!item->Pfile.isEmpty())
		{
			writer.writeAttribute("PFILE", "");
			writer.writeAttribute("isInlineImage", static_cast<int>(item->isInlineImage));
			QFileInfo inlFi(item->Pfile);
			writer.writeAttribute("inlineImageExt", inlFi.suffix());
			QFile inFil(item->Pfile);
			if (inFil.open(QIODevice::ReadOnly))
			{
				QByteArray ba = qCompress(inFil.readAll()).toBase64();
				writer.writeAttribute("ImageData", QString(ba));
				inFil.close();
			}
			PageItem_OSGFrame *osgframe = item->asOSGFrame();
			writer.writeAttribute("modelFile", Path2Relative(osgframe->modelFile, baseDir));
			writer.writeAttribute("currentViewName", osgframe->currentView);
		}
	}
#endif
	if (!item->Pfile2.isEmpty())
		writer.writeAttribute("PFILE2",Path2Relative(item->Pfile2, baseDir));
	else
		writer.writeAttribute("PFILE2","");
	if (!item->Pfile3.isEmpty())
		writer.writeAttribute("PFILE3",Path2Relative(item->Pfile3, baseDir));
	else
		writer.writeAttribute("PFILE3","");
	writer.writeAttribute("PRFILE"     , item->IProfile);
	writer.writeAttribute("EPROF"      , item->EmProfile);
	writer.writeAttribute("IRENDER"    , item->IRender);
	writer.writeAttribute("EMBEDDED"   , item->UseEmbedded ? "1" : "0");
	if (item->asImageFrame())
	{
		if (item->OverrideCompressionMethod)
			writer.writeAttribute("COMPRESSIONMETHOD", item->CompressionMethodIndex);
		if (item->OverrideCompressionQuality)
			writer.writeAttribute("COMPRESSIONQUALITY", item->CompressionQualityIndex);
	}
	writer.writeAttribute("LOCK"       , item->locked() ? "1" : "0");
	writer.writeAttribute("LOCKR"      , item->sizeLocked() ? "1" : "0");
	writer.writeAttribute("REVERS"     , item->reversed() ? "1" : "0");
	writer.writeAttribute("TransValue" , item->fillTransparency());
	writer.writeAttribute("TransValueS", item->lineTransparency());
	writer.writeAttribute("TransBlend" , item->fillBlendmode());
	writer.writeAttribute("TransBlendS", item->lineBlendmode());
	writer.writeAttribute("isTableItem", static_cast<int>(item->isTableItem));
	writer.writeAttribute("TopLine"    , static_cast<int>(item->TopLine));
	writer.writeAttribute("LeftLine"   , static_cast<int>(item->LeftLine));
	writer.writeAttribute("RightLine"  , static_cast<int>(item->RightLine));
	writer.writeAttribute("BottomLine" , static_cast<int>(item->BottomLine));
	if (item->isTableItem)
	{
		if (item->TopLink != 0)
			writer.writeAttribute("TopLINK", item->TopLink->ItemNr);
		else
			writer.writeAttribute("TopLINK", "-1");
		if (item->LeftLink != 0)
			writer.writeAttribute("LeftLINK", item->LeftLink->ItemNr);
		else
			writer.writeAttribute("LeftLINK", "-1");
		if (item->RightLink != 0)
			writer.writeAttribute("RightLINK", item->RightLink->ItemNr);
		else
			writer.writeAttribute("RightLINK", "-1");
		if (item->BottomLink != 0)
			writer.writeAttribute("BottomLINK", item->BottomLink->ItemNr);
		else
			writer.writeAttribute("BottomLINK", "-1");
		writer.writeAttribute("OwnLINK", item->ItemNr);
	}
	writer.writeAttribute("NUMDASH", item->DashValues.count());
	QString dlp = "";
	QVector<double>::Iterator dax;
	for (dax = item->DashValues.begin(); dax != item->DashValues.end(); ++dax)
		dlp += tmp.setNum((*dax)) + " ";
	writer.writeAttribute("DASHS"  , dlp);
	writer.writeAttribute("DASHOFF", item->DashOffset);
	writer.writeAttribute("NUMPO"  , item->PoLine.size());
	QString polp = "";
	for (uint nxx=0; nxx<item->PoLine.size(); ++nxx)
	{
		item->PoLine.point(nxx, &xf, &yf);
		polp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	writer.writeAttribute("POCOOR", polp);
	writer.writeAttribute("NUMCO" , item->ContourLine.size());
	QString colp = "";
	for (uint nxx=0; nxx<item->ContourLine.size(); ++nxx)
	{
		item->ContourLine.point(nxx, &xf, &yf);
		colp += tmp.setNum(xf) + " " + tmpy.setNum(yf) + " ";
	}
	writer.writeAttribute("COCOOR"  , colp);
	writer.writeAttribute("LANGUAGE", charStyle.language());
	writer.writeAttribute("startArrowIndex", item->startArrowIndex());
	writer.writeAttribute("endArrowIndex"  , item->endArrowIndex());
	writer.writeAttribute("startArrowScale", item->startArrowScale());
	writer.writeAttribute("endArrowScale"  , item->endArrowScale());
}
#endif
bool ScriXmlDoc::ReadElemHeader(QString file, bool isFile, double *x, double *y, double *w, double *h)
{
	QString ff = "";
	if (isFile)
	{
		QByteArray f;
		if (!loadRawText(file, f))
			return false;
		if (f.left(16) == "<SCRIBUSELEMUTF8")
			ff = QString::fromUtf8(f.data());
		else
			ff = f;
	}
	else
		ff  = file;

	bool succeed = false;
	QStringRef tName;
	QXmlStreamReader sReader(ff);
	QXmlStreamReader::TokenType tType;
	while (!sReader.atEnd() && !sReader.hasError())
	{
		tType = sReader.readNext();
		if (tType == QXmlStreamReader::StartElement)
		{
			tName = sReader.name();
			if ((tName == "SCRIBUSELEM") || (tName == "SCRIBUSELEMUTF8"))
			{
				QXmlStreamAttributes attrs = sReader.attributes();;
				QString attx = attrs.value("XP").toString();
				QString atty = attrs.value("YP").toString();
				QString attw = attrs.value("W").toString();
				QString atth = attrs.value("H").toString();
				*x = (attx.isEmpty()) ? 0.0 : ScCLocale::toDoubleC(attx);
				*y = (atty.isEmpty()) ? 0.0 : ScCLocale::toDoubleC(atty);
				*w = (attw.isEmpty()) ? 0.0 : ScCLocale::toDoubleC(attw);
				*h = (atth.isEmpty()) ? 0.0 : ScCLocale::toDoubleC(atth);
				succeed = true;
			}
		}
	}
	return (succeed && !sReader.hasError());
}

bool ScriXmlDoc::ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, double Xp, double Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub)
{
	// Do not suppose the existence of layer with id = 0
	// return ReadElemToLayer(fileName, avail, doc, Xp, Yp, Fi, loc, FontSub, view, 0);
	return ReadElemToLayer(fileName, avail, doc, Xp, Yp, Fi, loc, FontSub, doc->activeLayer());
}

bool ScriXmlDoc::ReadElemToLayer(QString fileName, SCFonts &avail, ScribusDoc *doc, double Xp, double Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub, int toLayer)
{
	QString ff;
	QString fileDir = QDir::homePath();
	if (Fi)
	{
		QByteArray f;
		if (!loadRawText(fileName, f))
			return false;
		if (f.left(16) == "<SCRIBUSELEMUTF8")
			ff = QString::fromUtf8(f.data());
		else
			ff = f;
		fileDir = QFileInfo(fileName).absolutePath();
	}
	else
	{
		ff = fileName;
	}
	// In case ff contains some old broken scribus xml
	ff.replace(QChar(5), SpecialChars::PARSEP);
	ff.replace(QChar(4), SpecialChars::TAB);
	ff.replace(QChar(0), QChar(32));
	ff.replace("&#x5;", SpecialChars::PARSEP);
	ff.replace("&#x4;", SpecialChars::TAB);
	const FileFormat *fmt = LoadSavePlugin::getFormatById(FORMATID_SLA150IMPORT);
	if (fmt)
	{
		fmt->setupTargets(doc, 0, doc->scMW(), 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
		fmt->loadElements(ff, fileDir, toLayer, Xp, Yp, loc);
		return true;
	}
	return false;
}

QString ScriXmlDoc::WriteElem(ScribusDoc *doc, Selection* selection)
{
	if (selection->count()==0)
		return "";
	double xp, yp, wp, hp;
	QString tmp, tmpy;
	PageItem *item;
	QString documentStr = "";
	item = selection->itemAt(0);
	QList<PageItem*> emG;
	emG.clear();
	for (int cor = 0; cor < selection->count(); ++cor)
	{
		emG.append(selection->itemAt(cor));
	}
	double selectionWidth = 0;
	double selectionHeight = 0;
	if (selection->isMultipleSelection())
	{
		double gx, gy, gw, gh;
		selection->getGroupRect(&gx, &gy, &gw, &gh);
		xp = gx - doc->currentPage()->xOffset();
		yp = gy - doc->currentPage()->yOffset();
		wp = gw;
		hp = gh;
		selection->getVisualGroupRect(&gx, &gy, &selectionWidth, &selectionHeight);
	}
	else
	{
		if (item->rotation() != 0)
		{
			double minx =  std::numeric_limits<double>::max();
			double miny =  std::numeric_limits<double>::max();
			double maxx = -std::numeric_limits<double>::max();
			double maxy = -std::numeric_limits<double>::max();
			double xpo = item->xPos() - doc->currentPage()->xOffset();
			double ypo = item->yPos() - doc->currentPage()->yOffset();
			FPointArray pb(4);
			pb.setPoint(0, FPoint(xpo, ypo));
			pb.setPoint(1, FPoint(item->width(), 0.0, xpo, ypo, item->rotation(), 1.0, 1.0));
			pb.setPoint(2, FPoint(item->width(), item->height(), xpo, ypo, item->rotation(), 1.0, 1.0));
			pb.setPoint(3, FPoint(0.0, item->height(), xpo, ypo, item->rotation(), 1.0, 1.0));
			for (uint pc = 0; pc < 4; ++pc)
			{
				minx = qMin(minx, pb.point(pc).x());
				miny = qMin(miny, pb.point(pc).y());
				maxx = qMax(maxx, pb.point(pc).x());
				maxy = qMax(maxy, pb.point(pc).y());
			}
			wp = maxx - minx;
			hp = maxy - miny;
		}
		else
		{
			wp = item->width();
			hp = item->height();
		}
		selectionWidth = item->visualWidth();
		selectionHeight = item->visualHeight();
		xp = item->xPos() - doc->currentPage()->xOffset();
		yp = item->yPos() - doc->currentPage()->yOffset();
	}
	double scaleI = 50.0 / qMax(selectionWidth, selectionHeight);
	QImage retImg = QImage(50, 50, QImage::Format_ARGB32_Premultiplied);
	retImg.fill( qRgba(0, 0, 0, 0) );
	ScPainter *painter = new ScPainter(&retImg, retImg.width(), retImg.height(), 1, 0);
	painter->setZoomFactor(scaleI);
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		painter->save();
		double x = embedded->xPos();
		double y = embedded->yPos();
		embedded->setXYPos(embedded->xPos()- doc->currentPage()->xOffset() - xp, embedded->yPos()- doc->currentPage()->yOffset() - yp, true);
		embedded->invalid = true;
		embedded->DrawObj(painter, QRectF());
		embedded->setXYPos(x, y, true);
		painter->restore();
	}
	delete painter;
	QBuffer buffer;
	buffer.open(QIODevice::WriteOnly);
	retImg.save(&buffer, "PNG");
	QByteArray ba = buffer.buffer().toBase64();
	buffer.close();
	const FileFormat *fmt = LoadSavePlugin::getFormatById(FORMATID_SLA150EXPORT);
	if (fmt)
	{
		fmt->setupTargets(doc, 0, doc->scMW(), 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
		documentStr = fmt->saveElements(xp, yp, wp, hp, selection, ba);
	}
	return documentStr;
}
#if 0
QString ScriXmlDoc::WriteElem(ScribusDoc *doc, Selection* selection)
{
	if (selection->count()==0)
		return "";
	double xp, yp;
	QString tmp, tmpy;
	PageItem *item;
	QString documentStr;
	QString baseDir = QDir::homePath();
	item = selection->itemAt(0);
	QList<uint> ELL;
	QList<PageItem*> emG;
	emG.clear();
	for (int cor=0; cor<selection->count(); ++cor)
	{
		ELL.append(selection->itemAt(cor)->ItemNr);
		emG.append(selection->itemAt(cor));
	}
	qSort(ELL);
	documentStr.reserve(524288);
	ScXmlStreamWriter writer(&documentStr);
	writer.setAutoFormatting(true);
	writer.writeStartElement("SCRIBUSELEMUTF8");
	double selectionWidth = 0;
	double selectionHeight = 0;
	if (selection->isMultipleSelection())
	{
		double gx, gy, gw, gh;
		selection->getGroupRect(&gx, &gy, &gw, &gh);
		xp = gx - doc->currentPage()->xOffset();
		yp = gy - doc->currentPage()->yOffset();
		writer.writeAttribute("W", gw);
		writer.writeAttribute("H", gh);
		selection->getVisualGroupRect(&gx, &gy, &selectionWidth, &selectionHeight);
	}
	else
	{
		if (item->rotation() != 0)
		{
			double minx =  std::numeric_limits<double>::max();
			double miny =  std::numeric_limits<double>::max();
			double maxx = -std::numeric_limits<double>::max();
			double maxy = -std::numeric_limits<double>::max();
			double xpo = item->xPos() - doc->currentPage()->xOffset();
			double ypo = item->yPos() - doc->currentPage()->yOffset();
			FPointArray pb(4);
			pb.setPoint(0, FPoint(xpo, ypo));
			pb.setPoint(1, FPoint(item->width(), 0.0, xpo, ypo, item->rotation(), 1.0, 1.0));
			pb.setPoint(2, FPoint(item->width(), item->height(), xpo, ypo, item->rotation(), 1.0, 1.0));
			pb.setPoint(3, FPoint(0.0, item->height(), xpo, ypo, item->rotation(), 1.0, 1.0));
			for (uint pc = 0; pc < 4; ++pc)
			{
				minx = qMin(minx, pb.point(pc).x());
				miny = qMin(miny, pb.point(pc).y());
				maxx = qMax(maxx, pb.point(pc).x());
				maxy = qMax(maxy, pb.point(pc).y());
			}
			writer.writeAttribute("W", maxx - minx);
			writer.writeAttribute("H", maxy - miny);
		}
		else
		{
			writer.writeAttribute("W", item->width());
			writer.writeAttribute("H", item->height());
		}
		selectionWidth = item->visualWidth();
		selectionHeight = item->visualHeight();
		xp = item->xPos() - doc->currentPage()->xOffset();
		yp = item->yPos() - doc->currentPage()->yOffset();
	}
	writer.writeAttribute("XP", xp);
	writer.writeAttribute("YP", yp);
	writer.writeAttribute("COUNT",   selection->count());
	writer.writeAttribute("Version", QString(VERSION));

	double scaleI = 50.0 / qMax(selectionWidth, selectionHeight);
	QImage retImg = QImage(50, 50, QImage::Format_ARGB32_Premultiplied);
	retImg.fill( qRgba(0, 0, 0, 0) );
	ScPainter *painter = new ScPainter(&retImg, retImg.width(), retImg.height(), 1, 0);
	painter->setZoomFactor(scaleI);
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		painter->save();
		double x = embedded->xPos();
		double y = embedded->yPos();
		embedded->setXYPos(embedded->xPos()- doc->currentPage()->xOffset() - xp, embedded->yPos()- doc->currentPage()->yOffset() - yp, true);
		embedded->invalid = true;
		embedded->DrawObj(painter, QRectF());
		embedded->setXYPos(x, y, true);
		painter->restore();
	}
	delete painter;
	QBuffer buffer;
	buffer.open(QIODevice::WriteOnly);
	retImg.save(&buffer, "PNG");
	QByteArray ba = buffer.buffer().toBase64();
	buffer.close();
	writer.writeAttribute("previewData", QString(ba));

	QMap<QString,int>::Iterator itf;
	for (itf = doc->UsedFonts.begin(); itf != doc->UsedFonts.end(); ++itf)
	{
		writer.writeStartElement("FONT");
		writer.writeAttribute("NAME",itf.key());
		writer.writeEndElement();
	}
	ColorList usedColors;
	doc->getUsedColors(usedColors);
	ColorList::Iterator itc;
	for (itc = usedColors.begin(); itc != usedColors.end(); ++itc)
	{
		writer.writeStartElement("COLOR");
		writer.writeAttribute("NAME",itc.key());
		if (itc.value().getColorModel() == colorModelRGB)
			writer.writeAttribute("RGB" , itc.value().nameRGB());
		else
			writer.writeAttribute("CMYK", itc.value().nameCMYK());
		writer.writeAttribute("Spot"    , itc.value().isSpotColor() ? "1" : "0");
		writer.writeAttribute("Register", itc.value().isRegistrationColor() ? "1" : "0");
		writer.writeEndElement();
	}
	QMap<int, ParagraphStyle> UsedStyles;
	QMap<int, int> UsedMapped2Saved;
	int NewStylesNum = 5;
	UsedStyles.clear();
	UsedMapped2Saved.clear();

	ParagraphStyle vg;
	for (int co=0; co < selection->count(); ++co)
	{
		item = doc->Items->at(ELL[co]);
		if (item->itemText.defaultStyle().hasParent())
		{
			int parstyle = findParagraphStyle(doc, item->itemText.defaultStyle().parent());
			if (parstyle >= 0) UsedStyles[parstyle] = doc->paragraphStyles()[parstyle];
		}
		if (((item->asTextFrame()) || (item->asPathText())) && (item->itemText.length() != 0))
		{
#ifndef NLS_PROTO
			for (int tx = 0; tx < item->itemText.length(); ++tx)
			{
				if (item->itemText.text(tx) == SpecialChars::PARSEP)
				{
					vg = item->itemText.paragraphStyle(tx);
					if (vg.hasParent())
					{
						int pindex = findParagraphStyle(doc, vg.parent());
						if (pindex >= 0) UsedStyles[pindex] = doc->paragraphStyles()[pindex];
					}
				}
			}
#endif
		}
	}
	QList<int> StyleNumb = UsedStyles.keys();
	qSort(StyleNumb);
	for (int ff = 0; ff < StyleNumb.count(); ++ff)
	{
		int actSt = StyleNumb.at(ff);
		UsedMapped2Saved.insert(actSt, NewStylesNum);
		NewStylesNum++;
		QMap<int, ParagraphStyle>::ConstIterator paraStyle = UsedStyles.find(actSt);
		writer.writeStartElement("STYLE");
		writer.writeAttribute("NAME",  paraStyle->name());
		writer.writeAttribute("ALIGN", static_cast<int>(paraStyle->alignment()));
		writer.writeAttribute("LINESPMode", static_cast<int>(paraStyle->lineSpacingMode()));
		//CB #2738:
		//UsedStyles[actSt].LineSpa is something like this is using automatic from the font:
		//10.34912109375000000000. Default attribute value is then 10.3491 which then becomes //10.34909999999999996589 and then does not compare. This fixes, should we change our
		//default precision?
		writer.writeAttribute("LINESP", QString::number(paraStyle->lineSpacing(),'f',13));
		writer.writeAttribute("INDENT", paraStyle->leftMargin());
		writer.writeAttribute("FIRST",  paraStyle->firstIndent());
		writer.writeAttribute("VOR",    paraStyle->gapBefore());
		writer.writeAttribute("NACH",   paraStyle->gapAfter());
		writer.writeAttribute("FONT",   paraStyle->charStyle().font().scName());
		writer.writeAttribute("FONTSIZE", paraStyle->charStyle().fontSize() / 10.0);
		writer.writeAttribute("DROP",     static_cast<int>(paraStyle->hasDropCap()));
		writer.writeAttribute("DROPLIN",  paraStyle->dropCapLines());
		writer.writeAttribute("DROPDIST", paraStyle->dropCapOffset());
		writer.writeAttribute("EFFECT",   paraStyle->charStyle().effects());
		
		writer.writeAttribute("FCOLOR", paraStyle->charStyle().fillColor());
		writer.writeAttribute("FSHADE", paraStyle->charStyle().fillShade());
		writer.writeAttribute("SCOLOR", paraStyle->charStyle().strokeColor());
		writer.writeAttribute("SSHADE", paraStyle->charStyle().strokeShade());
		writer.writeAttribute("BASE",   static_cast<int>(paraStyle->lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing));
		writer.writeAttribute("TXTSHX", paraStyle->charStyle().shadowXOffset() / 10.0);
		writer.writeAttribute("TXTSHY", paraStyle->charStyle().shadowYOffset() / 10.0);
		writer.writeAttribute("TXTOUT", paraStyle->charStyle().outlineWidth() / 10.0);
		writer.writeAttribute("TXTULP", paraStyle->charStyle().underlineOffset() / 10.0);
		writer.writeAttribute("TXTULW", paraStyle->charStyle().underlineWidth() / 10.0);
		writer.writeAttribute("TXTSTP", paraStyle->charStyle().strikethruOffset() / 10.0);
		writer.writeAttribute("TXTSTW", paraStyle->charStyle().strikethruWidth() / 10.0);
		writer.writeAttribute("SCALEH", paraStyle->charStyle().scaleH() / 10.0);
		writer.writeAttribute("SCALEV", paraStyle->charStyle().scaleV() / 10.0);
		writer.writeAttribute("BASEO",  paraStyle->charStyle().baselineOffset() / 10.0);
		writer.writeAttribute("KERN",   paraStyle->charStyle().tracking() / 10.0);
		for (int a = 0; a < paraStyle->tabValues().count(); ++a)
		{
			writer.writeEmptyElement("Tabs");
			writer.writeAttribute("Type", paraStyle->tabValues().at(a).tabType);
			writer.writeAttribute("Pos",  paraStyle->tabValues().at(a).tabPosition);
			QString tabCh = "";
			if (!paraStyle->tabValues().at(a).tabFillChar.isNull())
				tabCh = QString(paraStyle->tabValues().at(a).tabFillChar);
			writer.writeAttribute("Fill", tabCh);
		}
		writer.writeEndElement();
	}

	QMap<QString,multiLine>::Iterator itMU;
	for (itMU = doc->MLineStyles.begin(); itMU != doc->MLineStyles.end(); ++itMU)
	{
		writer.writeStartElement("MultiLine");
		writer.writeAttribute("Name",itMU.key());
		multiLine ml = itMU.value();
		multiLine::iterator itMU2;
		for (itMU2 = ml.begin(); itMU2 != ml.end(); ++itMU2)
		{
			writer.writeStartElement("SubLine");
			writer.writeAttribute("Color"   , (*itMU2).Color);
			writer.writeAttribute("Shade"   , (*itMU2).Shade);
			writer.writeAttribute("Dash"    , (*itMU2).Dash);
			writer.writeAttribute("LineEnd" , (*itMU2).LineEnd);
			writer.writeAttribute("LineJoin", (*itMU2).LineJoin);
			writer.writeAttribute("Width"   , (*itMU2).Width);
			writer.writeEndElement();
		}
		writer.writeEndElement();
	}
	QMap<QString, VGradient>::Iterator itGrad;
	for (itGrad = doc->docGradients.begin(); itGrad != doc->docGradients.end(); ++itGrad)
	{
		writer.writeStartElement("Gradient");
		writer.writeAttribute("Name",itGrad.key());
		VGradient gra = itGrad.value();
		QList<VColorStop*> cstops = gra.colorStops();
		for (uint cst = 0; cst < gra.Stops(); ++cst)
		{
			writer.writeEmptyElement("CSTOP");
			writer.writeAttribute("RAMP", cstops.at(cst)->rampPoint);
			writer.writeAttribute("NAME", cstops.at(cst)->name);
			writer.writeAttribute("SHADE", cstops.at(cst)->shade);
			writer.writeAttribute("TRANS", cstops.at(cst)->opacity);
		}
		writer.writeEndElement();
	}
	QMap<int, ArrowDesc> usedArrows;
	QMap<int, ArrowDesc>::Iterator itar;
	struct ArrowDesc arrow;
	for (int co=0; co<selection->count(); ++co)
	{
		item = doc->Items->at(ELL[co]);
		int startIndex = item->startArrowIndex();
		int endIndex = item->endArrowIndex();
		if (startIndex != 0)
		{
			arrow.points = doc->arrowStyles().at(startIndex-1).points.copy();
			arrow.name = doc->arrowStyles().at(startIndex-1).name;
			usedArrows.insert(startIndex, arrow);
		}
		if (endIndex != 0)
		{
			arrow.points = doc->arrowStyles().at(endIndex-1).points.copy();
			arrow.name = doc->arrowStyles().at(endIndex-1).name;
			usedArrows.insert(endIndex, arrow);
		}
	}
	if (usedArrows.count() != 0)
	{
		for (itar = usedArrows.begin(); itar != usedArrows.end(); ++itar)
		{
			if (itar.value().userArrow)
			{
				writer.writeStartElement("Arrows");
				writer.writeAttribute("NumPoints", itar.value().points.size());
				QString arp = "";
				double xa, ya;
				for (uint nxx = 0; nxx < itar.value().points.size(); ++nxx)
				{
					itar.value().points.point(nxx, &xa, &ya);
					arp += tmp.setNum(xa) + " " + tmpy.setNum(ya) + " ";
				}
				writer.writeAttribute("Points", arp);
				writer.writeAttribute("Name"  , itar.value().name);
				writer.writeAttribute("Index" , itar.key());
				writer.writeEndElement();
			}
		}
	}
	QStringList patterns = doc->getPatternDependencyList(doc->getUsedPatternsSelection(selection));
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern& pa = doc->docPatterns[patterns[c]];
		writer.writeStartElement("Pattern");
		writer.writeAttribute ("Name"  , patterns[c]);
		writer.writeAttribute ("scaleX", pa.scaleX);
		writer.writeAttribute ("scaleY", pa.scaleY);
		writer.writeAttribute("width" , pa.width);
		writer.writeAttribute ("height", pa.height);
		writer.writeAttribute("xoffset", pa.xoffset);
		writer.writeAttribute("yoffset", pa.yoffset);
		for (int o = 0; o < pa.items.count(); o++)
		{
			QDir::setCurrent(QDir::homePath());
			item = pa.items.at(o);
			writer.writeStartElement("PatternItem");
			WriteObject(writer, doc, item, baseDir, UsedMapped2Saved);
			writer.writeEndElement();
		}
		writer.writeEndElement();
	}
	for (int co=0; co<selection->count(); ++co)
	{
		QDir::setCurrent(QDir::homePath());
		item = doc->Items->at(ELL[co]);
		writer.writeStartElement("ITEM");
		WriteObject(writer, doc, item, baseDir, UsedMapped2Saved);
		writer.writeEndElement();
	}
	writer.writeEndElement();
	writer.writeEndDocument();
	return documentStr;
}
void ScriXmlDoc::WriteObject(ScXmlStreamWriter& writer, ScribusDoc *doc, PageItem *item, const QString& baseDir, QMap<int, int> &UsedMapped2Saved)
{
	QString text, tmp, tmpy;
	QString CurDirP = QDir::currentPath();
	const ParagraphStyle& defaultStyle = item->itemText.defaultStyle();
	int textAlignment = item->itemText.defaultStyle().alignment();
	writer.writeAttribute("ALIGN", textAlignment);
	SetItemProps(writer, doc, item, baseDir, false);
	//writer.writeAttribute("LOCK", "0");
	//writer.writeAttribute("XPOS",  item->xPos() - doc->currentPage()->xOffset());
	//writer.writeAttribute("YPOS",  item->yPos() - doc->currentPage()->yOffset());
	writer.writeAttribute("BOOKMARK"   , item->isBookmark ? "1" : "0");
	writer.writeAttribute("fillRule"   , item->fillRule ? "1" : "0");
	writer.writeAttribute("doOverprint", item->doOverprint ? "1" : "0");
	writer.writeAttribute("ImageClip"  , item->pixm.imgInfo.usedPath);
	writer.writeAttribute("BACKITEM", "-1");
	writer.writeAttribute("BACKPAGE", "-1");
	writer.writeAttribute("NEXTITEM", "-1");
	writer.writeAttribute("NEXTPAGE", "-1");

	if (item->GrType != 0 && item->GrType != 8)
	{
		writer.writeAttribute("GRSTARTX", item->GrStartX);
		writer.writeAttribute("GRSTARTY", item->GrStartY);
		writer.writeAttribute("GRENDX"  , item->GrEndX);
		writer.writeAttribute("GRENDY"  , item->GrEndY);
		writer.writeAttribute("GRFOCALX", item->GrFocalX);
		writer.writeAttribute("GRFOCALY", item->GrFocalY);
		writer.writeAttribute("GRSCALE" , item->GrScale);
		writer.writeAttribute("GRSKEW"  , item->GrSkew);
		if ((item->GrType == 9) || (item->GrType == 10))
		{
			writer.writeAttribute("GRC1X"   , item->GrControl1.x());
			writer.writeAttribute("GRC1Y"   , item->GrControl1.y());
			writer.writeAttribute("GRCOLP1" , item->GrColorP1);
			writer.writeAttribute("GRC2X"   , item->GrControl2.x());
			writer.writeAttribute("GRC2Y"   , item->GrControl2.y());
			writer.writeAttribute("GRCOLP2" , item->GrColorP2);
			writer.writeAttribute("GRC3X"   , item->GrControl3.x());
			writer.writeAttribute("GRC3Y"   , item->GrControl3.y());
			writer.writeAttribute("GRCOLP3" , item->GrColorP3);
			writer.writeAttribute("GRC4X"   , item->GrControl4.x());
			writer.writeAttribute("GRC4Y"   , item->GrControl4.y());
			writer.writeAttribute("GRCOLP4" , item->GrColorP4);
			writer.writeAttribute("GRCOLT1" , item->GrCol1transp);
			writer.writeAttribute("GRCOLT2" , item->GrCol2transp);
			writer.writeAttribute("GRCOLT3" , item->GrCol3transp);
			writer.writeAttribute("GRCOLT4" , item->GrCol4transp);
			writer.writeAttribute("GRCOLS1" , item->GrCol1Shade);
			writer.writeAttribute("GRCOLS2" , item->GrCol1Shade);
			writer.writeAttribute("GRCOLS3" , item->GrCol1Shade);
			writer.writeAttribute("GRCOLS4" , item->GrCol1Shade);
			writer.writeAttribute("GRC5X"   , item->GrControl5.x());
			writer.writeAttribute("GRC5Y"   , item->GrControl5.y());
		}
	}
	if (item->GrTypeStroke > 0)
	{
		writer.writeAttribute("GRSTARTXS", item->GrStrokeStartX);
		writer.writeAttribute("GRSTARTYS", item->GrStrokeStartY);
		writer.writeAttribute("GRENDXS"  , item->GrStrokeEndX);
		writer.writeAttribute("GRENDYS"  , item->GrStrokeEndY);
		writer.writeAttribute("GRFOCALXS", item->GrStrokeFocalX);
		writer.writeAttribute("GRFOCALYS", item->GrStrokeFocalY);
		writer.writeAttribute("GRSCALES" , item->GrStrokeScale);
		writer.writeAttribute("GRSKEWS"  , item->GrStrokeSkew);
	}

	if (item->effectsInUse.count() != 0)
	{
		for (int a = 0; a < item->effectsInUse.count(); ++a)
		{
			writer.writeStartElement("ImageEffect");
			writer.writeAttribute("Code" , item->effectsInUse.at(a).effectCode);
			writer.writeAttribute("Param", item->effectsInUse.at(a).effectParameters);
			writer.writeEndElement();
		}
	}
	if (item->itemText.defaultStyle().tabValues().count() != 0)
	{
		for (int a = 0; a < item->itemText.defaultStyle().tabValues().count(); ++a)
		{
			writer.writeStartElement("Tabs");
			writer.writeAttribute("Type", (defaultStyle.tabValues().at(a)).tabType);
			writer.writeAttribute("Pos" , (defaultStyle.tabValues().at(a)).tabPosition);
			QString tabCh = "";
			if (!(defaultStyle.tabValues().at(a)).tabFillChar.isNull())
				tabCh = QString((defaultStyle.tabValues().at(a)).tabFillChar);
			writer.writeAttribute("Fill", tabCh);
			writer.writeEndElement();
		}
	}
	if (((item->asImageFrame()) || (item->asTextFrame())) && (!item->Pfile.isEmpty()) && (item->pixm.imgInfo.layerInfo.count() != 0) && (item->pixm.imgInfo.isRequest))
	{
		QMap<int, ImageLoadRequest>::iterator it2;
		for (it2 = item->pixm.imgInfo.RequestProps.begin(); it2 != item->pixm.imgInfo.RequestProps.end(); ++it2)
		{
			writer.writeStartElement("PSDLayer");
			writer.writeAttribute("Layer"  , it2.key());
			writer.writeAttribute("Visible", it2.value().visible ? "1" : "0");
			writer.writeAttribute("useMask", it2.value().useMask ? "1" : "0");
			writer.writeAttribute("Opacity", it2.value().opacity);
			writer.writeAttribute("Blend"  , it2.value().blend);
			writer.writeEndElement();
		}
	}
	if (item->GrType != 0)
	{
		if (item->GrType == 8)
		{
			writer.writeAttribute("pattern", item->pattern());
			double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY;
			item->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
			bool mirrorX, mirrorY;
			item->patternFlip(mirrorX, mirrorY);
			writer.writeAttribute("pScaleX"  , patternScaleX);
			writer.writeAttribute("pScaleY"  , patternScaleY);
			writer.writeAttribute("pOffsetX" , patternOffsetX);
			writer.writeAttribute("pOffsetY" , patternOffsetY);
			writer.writeAttribute("pRotation", patternRotation);
			writer.writeAttribute("pSkewX"   , patternSkewX);
			writer.writeAttribute("pSkewY"   , patternSkewY);
			writer.writeAttribute("pMirrorX" , mirrorX);
			writer.writeAttribute("pMirrorY" , mirrorY);
		}
		else
		{
			if (item->GrType == 11)
			{
				writer.writeAttribute("GMAY", item->meshGradientArray[0].count());
				writer.writeAttribute("GMAX", item->meshGradientArray.count());
				for (int grow = 0; grow < item->meshGradientArray.count(); grow++)
				{
					for (int gcol = 0; gcol < item->meshGradientArray[grow].count(); gcol++)
					{
						meshPoint mp = item->meshGradientArray[grow][gcol];
						writer.writeStartElement("MPoint");
						writer.writeAttribute("GX", mp.gridPoint.x());
						writer.writeAttribute("GY", mp.gridPoint.y());
						writer.writeAttribute("CTX", mp.controlTop.x());
						writer.writeAttribute("CTY", mp.controlTop.y());
						writer.writeAttribute("CBX", mp.controlBottom.x());
						writer.writeAttribute("CBY", mp.controlBottom.y());
						writer.writeAttribute("CLX", mp.controlLeft.x());
						writer.writeAttribute("CLY", mp.controlLeft.y());
						writer.writeAttribute("CRX", mp.controlRight.x());
						writer.writeAttribute("CRY", mp.controlRight.y());
						writer.writeAttribute("NAME", mp.colorName);
						writer.writeAttribute("SHADE", mp.shade);
						writer.writeAttribute("TRANS", mp.transparency);
						writer.writeEndElement();
					}
				}
			}
			if ((item->GrType != 9) && (item->GrType != 11))
			{
				if (item->gradient().isEmpty())
				{
					QList<VColorStop*> cstops = item->fill_gradient.colorStops();
					for (uint cst = 0; cst < item->fill_gradient.Stops(); ++cst)
					{
						writer.writeStartElement("CSTOP");
						writer.writeAttribute("RAMP" , cstops.at(cst)->rampPoint);
						writer.writeAttribute("NAME" , cstops.at(cst)->name);
						writer.writeAttribute("SHADE", cstops.at(cst)->shade);
						writer.writeAttribute("TRANS", cstops.at(cst)->opacity);
						writer.writeEndElement();
					}
				}
				else
					writer.writeAttribute("GRNAME"  , item->gradient());
			}
		}
	}
	if (item->GrTypeStroke > 0)
	{
		if (item->strokeGradient().isEmpty())
		{
			QList<VColorStop*> cstops = item->stroke_gradient.colorStops();
			for (uint cst = 0; cst < item->stroke_gradient.Stops(); ++cst)
			{
				writer.writeStartElement("S_CSTOP");
				writer.writeAttribute("RAMP" , cstops.at(cst)->rampPoint);
				writer.writeAttribute("NAME" , cstops.at(cst)->name);
				writer.writeAttribute("SHADE", cstops.at(cst)->shade);
				writer.writeAttribute("TRANS", cstops.at(cst)->opacity);
				writer.writeEndElement();
			}
		}
		else
			writer.writeAttribute("GRNAMES"  , item->strokeGradient());
	}
	if (!item->strokePattern().isEmpty())
	{
		writer.writeAttribute("patternS", item->strokePattern());
		double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace;
		item->strokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
		bool mirrorX, mirrorY, atPath;
		item->strokePatternFlip(mirrorX, mirrorY);
		atPath = item->isStrokePatternToPath();
		writer.writeAttribute("pScaleXS", patternScaleX);
		writer.writeAttribute("pScaleYS", patternScaleY);
		writer.writeAttribute("pOffsetXS", patternOffsetX);
		writer.writeAttribute("pOffsetYS", patternOffsetY);
		writer.writeAttribute("pRotationS", patternRotation);
		writer.writeAttribute("pSkewXS"   , patternSkewX);
		writer.writeAttribute("pSkewYS"   , patternSkewY);
		writer.writeAttribute("pSpaceS"   , patternSpace);
		writer.writeAttribute("pMirrorXS" , mirrorX);
		writer.writeAttribute("pMirrorYS" , mirrorY);
		writer.writeAttribute("pAtPathS"  , atPath);
	}
	if (item->GrMask > 0)
	{
		if (item->gradientMask().isEmpty())
		{
			QList<VColorStop*> cstops = item->mask_gradient.colorStops();
			for (uint cst = 0; cst < item->mask_gradient.Stops(); ++cst)
			{
				writer.writeStartElement("M_CSTOP");
				writer.writeAttribute("RAMP" , cstops.at(cst)->rampPoint);
				writer.writeAttribute("NAME" , cstops.at(cst)->name);
				writer.writeAttribute("SHADE", cstops.at(cst)->shade);
				writer.writeAttribute("TRANS", cstops.at(cst)->opacity);
				writer.writeEndElement();
			}
		}
		else
			writer.writeAttribute("GRNAMEM"  , item->gradientMask());
		writer.writeAttribute("GRSTARTXM", item->GrMaskStartX);
		writer.writeAttribute("GRSTARTYM", item->GrMaskStartY);
		writer.writeAttribute("GRENDXM", item->GrMaskEndX);
		writer.writeAttribute("GRENDYM", item->GrMaskEndY);
		writer.writeAttribute("GRFOCALXM", item->GrMaskFocalX);
		writer.writeAttribute("GRFOCALYM", item->GrMaskFocalY);
		writer.writeAttribute("GRSCALEM" , item->GrMaskScale);
		writer.writeAttribute("GRSKEWM" , item->GrMaskSkew);
	}
	if (!item->patternMask().isEmpty())
	{
		writer.writeAttribute("patternM", item->patternMask());
		double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY;
		item->maskTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
		bool mirrorX, mirrorY;
		item->maskFlip(mirrorX, mirrorY);
		writer.writeAttribute("pScaleXM", patternScaleX);
		writer.writeAttribute("pScaleYM", patternScaleY);
		writer.writeAttribute("pOffsetXM", patternOffsetX);
		writer.writeAttribute("pOffsetYM", patternOffsetY);
		writer.writeAttribute("pRotationM", patternRotation);
		writer.writeAttribute("pSkewXM"   , patternSkewX);
		writer.writeAttribute("pSkewYM"   , patternSkewY);
		writer.writeAttribute("pMirrorXM" , mirrorX);
		writer.writeAttribute("pMirrorYM" , mirrorY);
	}
	if (item->asLatexFrame())
	{
		PageItem_LatexFrame *latexitem = item->asLatexFrame();
		writer.writeAttribute("LatexDpi", latexitem->dpi());
		writer.writeAttribute("LatexConfig", latexitem->configFile());
		writer.writeAttribute("LatexUsePreamble", QString::number(static_cast<int>(latexitem->usePreamble())));
		writer.writeTextElement("LATEX-SOURCE", item->asLatexFrame()->formula());
	}
	if (item->asSymbolFrame())
		writer.writeAttribute("pattern", item->pattern());
	if (item->asGroupFrame())
	{
		writer.writeAttribute("groupWidth", item->groupWidth);
		writer.writeAttribute("groupHeight", item->groupHeight);
		for (int co = 0; co < item->groupItemList.count(); ++co)
		{
			PageItem* gitem = item->groupItemList.at(co);
			writer.writeStartElement("ITEM");
			WriteObject(writer, doc, gitem, baseDir, UsedMapped2Saved);
			writer.writeEndElement();
		}
	}
#ifdef HAVE_OSG
	if (item->asOSGFrame())
	{
		PageItem_OSGFrame *osgitem = item->asOSGFrame();
		if (!item->Pfile.isEmpty())
		{
			QHash<QString, PageItem_OSGFrame::viewDefinition>::iterator itv;
			for (itv = osgitem->viewMap.begin(); itv != osgitem->viewMap.end(); ++itv)
			{
				writer.writeStartElement("OSGViews");
				writer.writeAttribute("viewName", itv.key());
				writer.writeAttribute("angleFOV", itv.value().angleFOV);
				QString trackM = "";
				for (uint matx = 0; matx < 4; ++matx)
				{
					for (uint maty = 0; maty < 4; ++maty)
					{
						trackM += tmp.setNum(itv.value().trackerMatrix(matx, maty))+" ";
					}
				}
				writer.writeAttribute("trackM", trackM);
				QString trackC = "";
				trackC += tmp.setNum(itv.value().trackerCenter[0])+" ";
				trackC += tmp.setNum(itv.value().trackerCenter[1])+" ";
				trackC += tmp.setNum(itv.value().trackerCenter[2]);
				writer.writeAttribute("trackC", trackC);
				QString cameraP = "";
				cameraP += tmp.setNum(itv.value().cameraPosition[0])+" ";
				cameraP += tmp.setNum(itv.value().cameraPosition[1])+" ";
				cameraP += tmp.setNum(itv.value().cameraPosition[2]);
				writer.writeAttribute("cameraP", cameraP);
				QString cameraU = "";
				cameraU += tmp.setNum(itv.value().cameraUp[0])+" ";
				cameraU += tmp.setNum(itv.value().cameraUp[1])+" ";
				cameraU += tmp.setNum(itv.value().cameraUp[2]);
				writer.writeAttribute("cameraU", cameraU);
				writer.writeAttribute("trackerDist", itv.value().trackerDist);
				writer.writeAttribute("trackerSize", itv.value().trackerSize);
				writer.writeAttribute("illumination", itv.value().illumination);
				writer.writeAttribute("rendermode", itv.value().rendermode);
				writer.writeAttribute("trans", itv.value().addedTransparency);
				writer.writeAttribute("colorAC", itv.value().colorAC.name());
				writer.writeAttribute("colorFC", itv.value().colorFC.name());
				writer.writeEndElement();
			}
		}
	}
#endif
	QDir::setCurrent(CurDirP);
	WriteITEXTs(writer, doc, item);
}

void ScriXmlDoc::WriteITEXTs(ScXmlStreamWriter &writer, ScribusDoc *doc, PageItem *item)
{
	CharStyle lastStyle;
	int lastPos = 0;
	QString text, pstylename;

	for( int k = 0; k < item->itemText.length(); ++k)
	{
		const CharStyle& style(item->itemText.charStyle(k));
		QChar ch = item->itemText.text(k);
		pstylename = QString();
		if (ch == SpecialChars::PARSEP)
			pstylename = item->itemText.paragraphStyle(k).parent();
		if (style != lastStyle || ch == SpecialChars::PARSEP ||
			ch == SpecialChars::PAGENUMBER ||
			ch == SpecialChars::PAGECOUNT)
		{
			if(k - lastPos > 0)
			{
				text = item->itemText.text(lastPos, k - lastPos);
				writer.writeEmptyElement("ITEXT");
				writer.writeAttribute("CH"     ,text);
				WriteLegacyCStyle(writer, lastStyle);
				writer.writeAttribute("PSTYLE" ,pstylename);
				lastPos = k;
			}
			lastStyle = style;
		}
		if (ch == SpecialChars::PARSEP)
		{
			WritePStyle(writer, item->itemText.paragraphStyle(k), "PARA");
			lastPos = k + 1;
		}
		else if (ch == SpecialChars::PAGENUMBER) 
		{
			writer.writeEmptyElement("var");
			writer.writeAttribute("name", "pgno");
			WriteLegacyCStyle(writer, lastStyle);
			lastPos = k + 1;
		}
		else if (ch == SpecialChars::PAGECOUNT) 
		{
			writer.writeEmptyElement("var");
			writer.writeAttribute("name", "pgco");
			WriteLegacyCStyle(writer, lastStyle);
			lastPos = k + 1;
		}
	}
	// write pending chars
	if ( item->itemText.length() - lastPos > 0)
	{
		writer.writeEmptyElement("ITEXT");
		text = item->itemText.text(lastPos, item->itemText.length() - lastPos);
		writer.writeAttribute("CH"     ,text);
		WriteLegacyCStyle(writer, lastStyle); 
		writer.writeAttribute("PSTYLE" ,pstylename);
	}
	// paragraphstyle for trailing chars
	if ((item->itemText.length() > 0) && (item->itemText.text(item->itemText.length()-1) != SpecialChars::PARSEP) )
		WritePStyle(writer, item->itemText.paragraphStyle(item->itemText.length()), "PARA");
}

void ScriXmlDoc::WriteLegacyCStyle (ScXmlStreamWriter& writer, const CharStyle& style)
{
	if ( ! style.isInhFont())	
		writer.writeAttribute("CFONT", style.font().scName());
	if ( ! style.isInhFontSize())
		writer.writeAttribute("CSIZE", style.fontSize() / 10.0);
	if ( ! style.isInhFillColor())
		writer.writeAttribute("CCOLOR", style.fillColor());
	if ( ! style.isInhFillShade())
		writer.writeAttribute("CSHADE", style.fillShade());
	if ( ! style.isInhStrokeColor())
		writer.writeAttribute("CSTROKE", style.strokeColor());
	if ( ! style.isInhStrokeShade())
		writer.writeAttribute("CSHADE2", style.strokeShade());
	if ( ! style.isInhShadowXOffset())
		writer.writeAttribute("CSHX", style.shadowXOffset() / 10.0);
	if ( ! style.isInhShadowYOffset())
		writer.writeAttribute("CSHY", style.shadowYOffset() / 10.0);
	if ( ! style.isInhOutlineWidth())
		writer.writeAttribute("COUT", style.outlineWidth() / 10.0);
	if ( ! style.isInhUnderlineOffset())
		writer.writeAttribute("CULP", style.underlineOffset() / 10.0);
	if ( ! style.isInhUnderlineWidth())
		writer.writeAttribute("CULW", style.underlineWidth() / 10.0);
	if ( ! style.isInhStrikethruOffset())
		writer.writeAttribute("CSTP", style.strikethruOffset() / 10.0);
	if ( ! style.isInhStrikethruWidth())
		writer.writeAttribute("CSTW", style.strikethruWidth() / 10.0);
	if ( ! style.isInhScaleH())
		writer.writeAttribute("CSCALE", style.scaleH() / 10.0);
	if ( ! style.isInhScaleV())
		writer.writeAttribute("CSALEV", style.scaleV() / 10.0);
	writer.writeAttribute("CSTYLE", style.effects() & 2047);
}

void ScriXmlDoc::WriteCStyle (ScXmlStreamWriter& writer, const CharStyle& style)
{
	if ( ! style.name().isEmpty() )
		writer.writeAttribute("CNAME", style.name());
	if ( ! style.parent().isEmpty() )
		writer.writeAttribute("CPARENT", style.parent());
	if ( ! style.isInhFont())	
		writer.writeAttribute("FONT", style.font().scName());
	if ( ! style.isInhFontSize())
		writer.writeAttribute("FONTSIZE", style.fontSize() / 10.0);
	if ( ! style.isInhFeatures())
		writer.writeAttribute("FEATURES", style.features().join(" "));
	if ( ! style.isInhFillColor())
		writer.writeAttribute("FCOLOR", style.fillColor());
	if ( ! style.isInhFillShade())
		writer.writeAttribute("FSHADE", style.fillShade());
	if ( ! style.isInhStrokeColor())
		writer.writeAttribute("SCOLOR", style.strokeColor());
	if ( ! style.isInhStrokeShade())
		writer.writeAttribute("SSHADE", style.strokeShade());
	if ( ! style.isInhShadowXOffset())
		writer.writeAttribute("TXTSHX", style.shadowXOffset() / 10.0);
	if ( ! style.isInhShadowYOffset())
		writer.writeAttribute("TXTSHY", style.shadowYOffset() / 10.0);
	if ( ! style.isInhOutlineWidth())
		writer.writeAttribute("TXTOUT", style.outlineWidth() / 10.0);
	if ( ! style.isInhUnderlineOffset())
		writer.writeAttribute("TXTULP", style.underlineOffset() / 10.0);
	if ( ! style.isInhUnderlineWidth())
		writer.writeAttribute("TXTULW", style.underlineWidth() / 10.0);
	if ( ! style.isInhStrikethruOffset())
		writer.writeAttribute("TXTSTP", style.strikethruOffset() / 10.0);
	if ( ! style.isInhStrikethruWidth())
		writer.writeAttribute("TXTSTW", style.strikethruWidth() / 10.0);
	if ( ! style.isInhScaleH())
		writer.writeAttribute("SCALEH", style.scaleH() / 10.0);
	if ( ! style.isInhScaleV())
		writer.writeAttribute("SCALEV", style.scaleV() / 10.0);
	if ( ! style.isInhBaselineOffset())
		writer.writeAttribute("BASEO", style.baselineOffset() / 10.0);
	if ( ! style.isInhTracking())
		writer.writeAttribute("KERN", style.tracking() / 10.0); 
	if ( ! style.isInhWordTracking())
		writer.writeAttribute("wordTrack", style.wordTracking());
	if ( ! style.shortcut().isEmpty() )
		writer.writeAttribute("SHORTCUT", style.shortcut()); // shortcuts won't be inherited
}

void ScriXmlDoc::WritePStyle (ScXmlStreamWriter& writer, const ParagraphStyle& style, const QString& nodeName)
{
	writer.writeStartElement(nodeName);
	if ( ! style.name().isEmpty() )
		writer.writeAttribute("NAME", style.name());
	if ( ! style.parent().isEmpty())
		writer.writeAttribute("PARENT", style.parent());
	
	if ( ! style.isInhAlignment())
		writer.writeAttribute("ALIGN", style.alignment());
	if ( ! style.isInhLineSpacingMode())
		writer.writeAttribute("LINESPMode", style.lineSpacingMode());
	if ( ! style.isInhLineSpacing())
		writer.writeAttribute("LINESP", style.lineSpacing());
	if ( ! style.isInhLeftMargin())
		writer.writeAttribute("INDENT", style.leftMargin());
	if ( ! style.isInhRightMargin())
		writer.writeAttribute("RMARGIN", style.rightMargin());
	if ( ! style.isInhFirstIndent())
		writer.writeAttribute("FIRST", style.firstIndent());
	if ( ! style.isInhGapBefore())
		writer.writeAttribute("VOR", style.gapBefore());
	if ( ! style.isInhGapAfter())
		writer.writeAttribute("NACH", style.gapAfter());
	if ( ! style.isInhHasDropCap())
		writer.writeAttribute("DROP", static_cast<int>(style.hasDropCap()));
	if ( ! style.isInhDropCapLines())
		writer.writeAttribute("DROPLIN", style.dropCapLines());
	if ( ! style.isInhDropCapOffset())
		writer.writeAttribute("DROPDIST", style.dropCapOffset());
	if ( ! style.isInhOpticalMargins())
		writer.writeAttribute("OpticalMargins", style.opticalMargins());
	if ( ! style.isInhHyphenationMode())
		writer.writeAttribute("HyphenationMode", style.hyphenationMode());
	if ( ! style.isInhMinWordTracking())
		writer.writeAttribute("MinWordTrack", style.minWordTracking());
	if ( ! style.isInhMinGlyphExtension())
		writer.writeAttribute("MinGlyphShrink", style.minGlyphExtension());
	if ( ! style.isInhMaxGlyphExtension())
		writer.writeAttribute("MaxGlyphExtend", style.maxGlyphExtension());

	if ( ! style.shortcut().isEmpty() )
		writer.writeAttribute("PSHORTCUT", style.shortcut()); // shortcuts won't be inherited

	WriteCStyle(writer, style.charStyle());

	if ( ! style.isInhTabValues())
	{
		for (int a = 0; a < style.tabValues().count(); ++a)
		{
			writer.writeEmptyElement("Tabs");
			writer.writeAttribute("Type", (style.tabValues().at(a)).tabType);
			writer.writeAttribute("Pos", (style.tabValues().at(a)).tabPosition);
			QString tabCh = "";
			if (!(style.tabValues().at(a)).tabFillChar.isNull())
				tabCh = QString((style.tabValues().at(a)).tabFillChar);
			writer.writeAttribute("Fill", tabCh);
		}
	}

	writer.writeEndElement();
}
#endif
