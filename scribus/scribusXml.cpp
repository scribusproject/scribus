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
	QMap<int, PageItem*> emMap;
	emG.clear();
	for (int cor = 0; cor < selection->count(); ++cor)
	{
		emMap.insert(selection->itemAt(cor)->ItemNr, selection->itemAt(cor));
	}
	emG = emMap.values();
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
