/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          importdrw.cpp  -  description
                             -------------------
    begin                : Mon Jan 11 2010
    copyright            : (C) 2010 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#include <QByteArray>
#include <QCursor>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMimeData>
#include <QRegExp>
#include <QStack>
#include <QDebug>

#include <cstdlib>

#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "importdrw.h"
#include "loadsaveplugin.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "pageitem_imageframe.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "ui/propertiespalette.h"
#include "rawimage.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"

extern SCRIBUS_API ScribusQApp * ScQApp;

DrwPlug::DrwPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

QImage DrwPlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	double b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	double h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	progressDialog = NULL;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	baseX = m_Doc->currentPage()->xOffset();
	baseY = m_Doc->currentPage()->yOffset();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		if (!thumbRead)
		{
			tmpSel->clear();
			QDir::setCurrent(CurDirP);
			if (Elements.count() > 1)
				m_Doc->groupObjectsList(Elements);
		}
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		QImage tmpImage;
		if (thumbRead)
		{
			tmpImage = thumbnailImage;
			tmpImage.setText("XSize", QString("%1").arg(docWidth));
			tmpImage.setText("YSize", QString("%1").arg(docHeight));
		}
		else
		{
			if (Elements.count() > 0)
			{
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					tmpSel->addItem(Elements.at(dre), true);
				}
				tmpSel->setGroupRect();
				double xs = tmpSel->width();
				double ys = tmpSel->height();
				tmpImage = Elements.at(0)->DrawObj_toImage(500);
				tmpImage.setText("XSize", QString("%1").arg(xs));
				tmpImage.setText("YSize", QString("%1").arg(ys));
			}
		}
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		m_Doc->m_Selection->delaySignalsOff();
		delete m_Doc;
		return tmpImage;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		delete m_Doc;
	}
	return QImage();
}

bool DrwPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double x, y, b, h;
	bool ret = false;
	QFileInfo fi = QFileInfo(fName);
	if ( !ScCore->usingGUI() )
	{
		interactive = false;
		showProgress = false;
	}
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	if ( showProgress )
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		progressDialog = new MultiProgressDialog( tr("Importing: %1").arg(fi.fileName()), CommonStrings::tr_Cancel, mw );
		QStringList barNames, barTexts;
		barNames << "GI";
		barTexts << tr("Analyzing File:");
		QList<bool> barsNumeric;
		barsNumeric << false;
		progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
		progressDialog->setOverallTotalSteps(3);
		progressDialog->setOverallProgress(0);
		progressDialog->setProgress("GI", 0);
		progressDialog->show();
		connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
		qApp->processEvents();
	}
	else
		progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	x = 0.0;
	y = 0.0;
	b = 0.0;
	h = 0.0;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	docWidth = b;
	docHeight = h;
	baseX = 0;
	baseY = 0;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		baseX = 0;
		baseY = 0;
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			baseX = 0;
			baseY = 0;
			baseX = m_Doc->currentPage()->xOffset();
			baseY = m_Doc->currentPage()->yOffset();
		}
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!interactive))
	{
		if (docWidth > docHeight)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		if ((Elements.count() > 0) && (!ret) && (interactive))
		{
			if (flags & LoadSavePlugin::lfScripted)
			{
				bool loadF = m_Doc->isLoading();
				m_Doc->setLoading(false);
				m_Doc->changed();
				m_Doc->setLoading(loadF);
				if (!(flags & LoadSavePlugin::lfLoadAsPattern))
				{
					m_Doc->m_Selection->delaySignalsOn();
					for (int dre=0; dre<Elements.count(); ++dre)
					{
						m_Doc->m_Selection->addItem(Elements.at(dre), true);
					}
					m_Doc->m_Selection->delaySignalsOff();
					m_Doc->m_Selection->setGroupRect();
					m_Doc->view()->updatesOn(true);
				}
			}
			else
			{
				m_Doc->DragP = true;
				m_Doc->DraggedElem = 0;
				m_Doc->DragElements.clear();
				m_Doc->m_Selection->delaySignalsOn();
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					m_Doc->DragElements.append(Elements.at(dre)->ItemNr);
					tmpSel->addItem(Elements.at(dre), true);
				}
				tmpSel->setGroupRect();
				ScriXmlDoc *ss = new ScriXmlDoc();
				ScElemMimeData* md = new ScElemMimeData();
				md->setScribusElem(ss->WriteElem(m_Doc, tmpSel));
				delete ss;
				m_Doc->itemSelection_DeleteItem(tmpSel);
				m_Doc->view()->updatesOn(true);
				if (importedColors.count() != 0)
				{
					for (int cd = 0; cd < importedColors.count(); cd++)
					{
						m_Doc->PageColors.remove(importedColors[cd]);
					}
				}
				if (importedPatterns.count() != 0)
				{
					for (int cd = 0; cd < importedPatterns.count(); cd++)
					{
						m_Doc->docPatterns.remove(importedPatterns[cd]);
					}
				}
				m_Doc->m_Selection->delaySignalsOff();
				// We must copy the TransationSettings object as it is owned
				// by handleObjectImport method afterwards
				TransactionSettings* transacSettings = new TransactionSettings(trSettings);
				m_Doc->view()->handleObjectImport(md, transacSettings);
				m_Doc->DragP = false;
				m_Doc->DraggedElem = 0;
				m_Doc->DragElements.clear();
			}
		}
		else
		{
			m_Doc->changed();
			m_Doc->reformPages();
			if (!(flags & LoadSavePlugin::lfLoadAsPattern))
				m_Doc->view()->updatesOn(true);
		}
		success = true;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->view()->updatesOn(true);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	if (interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
	{
		if ((showProgress) && (!interactive))
			m_Doc->view()->DrawNew();
	}
	return success;
}

DrwPlug::~DrwPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

bool DrwPlug::convert(QString fn)
{
	QString tmp;
	Coords.resize(0);
	Coords.svgInit();
	importedColors.clear();
	importedPatterns.clear();
	DRWGroup gElements;
	gElements.xoffset = 0.0;
	gElements.yoffset = 0.0;
	gElements.nrOfItems = -1;
	gElements.counter = -1;
	groupStack.push(gElements);
	DRWObjectList gList;
	gList.groupX = 0.0;
	gList.groupY = 0.0;
	listStack.push(gList);
	scaleFactor = 0.15;
	lineWidth = 1.0;
	lineColor = "Black";
	fillColor = "Black";
	createObjCode = 0;
	nrOfPoints = 0;
	symbolCount = 0;
	recordCount = 0;
	imageValid = false;
	thumbRead = false;
	currentItem = NULL;
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ts(&f);
		ts.setByteOrder(QDataStream::LittleEndian);
		while (!ts.atEnd())
		{
			quint8 dataS, cmd;
			quint16 dataL;
			uint dataLen;
			int pos = ts.device()->pos();
			ts >> dataS;
			if (dataS == 0xFF)
			{
				ts >> dataL;
				dataLen = dataL;
			}
			else
				dataLen = dataS;
			ts >> cmd;
			decodeCmdData(ts, dataLen, cmd);
			decodeCmd(cmd, pos);
			if (progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
			if (cmd == 254)
				break;
			if ((importerFlags & LoadSavePlugin::lfCreateThumbnail) && (cmd == 11))
				thumbRead = true;
			if ((importerFlags & LoadSavePlugin::lfCreateThumbnail) && (cmd == 27) && (thumbRead))
				break;
		}
		if (Elements.count() == 0)
		{
			if (importedColors.count() != 0)
			{
				for (int cd = 0; cd < importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(importedColors[cd]);
				}
			}
			if (importedPatterns.count() != 0)
			{
				for (int cd = 0; cd < importedPatterns.count(); cd++)
				{
					m_Doc->docPatterns.remove(importedPatterns[cd]);
				}
			}
		}
		f.close();
	}
	if (progressDialog)
		progressDialog->close();
	return true;
}

void DrwPlug::decodeCmdData(QDataStream &ts, uint dataLen, quint8 cmd)
{
	cmdData.resize(0);
	uint count = 0;
	while (count < dataLen)
	{
		quint8 d;
		ts >> d;
		if ((cmd < 96) || (cmd > 160))
		{
			if (d == 0xFF)
			{
				quint8 val, dd;
				ts >> dd >> val;
				for (uint cc = 0; cc < dd; cc++)
				{
					cmdData.append(val);
					count++;
				}
			}
			else
			{
				cmdData.append(d);
				count++;
			}
		}
		else
		{
			cmdData.append(d);
			count++;
		}
	}
}

void DrwPlug::decodeCmd(quint8 cmd, int pos)
{
	recordCount++;
	bool printMSG = false;
/*	if ((recordCount > 29) && (recordCount < 33))
	{
		QFile f(QString("/home/franz/cmddatas%1.bin").arg(recordCount));
		f.open(QIODevice::WriteOnly);
		f.write(cmdData);
		f.close();
	} */
	QDataStream ds(cmdData);
	DRWGradient gradient;
	QByteArray pattern;
	quint8 data8, chData;
	quint16 data16;
	int index;
	QFont font;
	QString textFont;
	ds.setByteOrder(QDataStream::LittleEndian);
	QString cmdText = QString("Record %1 Type: ").arg(recordCount);
	switch (cmd)
	{
		case 1:
			cmdText += QString("DRW Background Color %1").arg(getColor(ds));
			break;
		case 2:
			cmdText += "DRW Facename";
			break;
		case 3:
			cmdText += QString("DRW Version Data %1").arg(QString(cmdData.toHex().left(64)));
			break;
		case 4:
			cmdText += QString("DRW ID Data %1").arg(QString(cmdData).left(20));
			if (listStack.count() > 0)
				listStack.top().itemGroupName = QString(cmdData);
			break;
		case 5:
			cmdText += QString("DRW Overlay Data %1").arg(QString(cmdData.toHex().left(20)));
			break;
		case 6:
			cmdText += "DRW Polygon";
			if ((createObjCode == 1) || (createObjCode == 3))
			{
				bool first = true;
				bool first2 = true;
				QPointF startP;
				QPainterPath path;
				for (int a = 0; a < nrOfPoints; a++)
				{
					QPointF coor = getCoordinate(ds);
					if (first)
					{
						path.moveTo(coor);
						if (first2)
							startP = coor;
						first = false;
						first2 = false;
					}
					else
					{
						if (coor == startP)
						{
							first = true;
							path.closeSubpath();
						}
						else
							path.lineTo(coor);
					}
				}
				if (currentItem != NULL)
				{
					currentItem->PoLine.fromQPainterPath(path);
					QRectF bBoxO = path.boundingRect();
					if (bBoxO.x() < 0)
						currentItem->PoLine.translate(-bBoxO.x(), 0);
					if (bBoxO.y() < 0)
						currentItem->PoLine.translate(0, -bBoxO.y());
					finishItem(currentItem);
					if (currentItem != NULL)
					{
						handleLineStyle(currentItem, flags, lineColor);
						handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
					}
				}
				createObjCode = 0;
				currentItem = NULL;
			}
			else if (createObjCode == 2)
			{
				bool first = true;
				QPointF startP;
				QPainterPath path;
				int a = 0;
				while (a < nrOfPoints)
				{
					if (first)
					{
						QPointF coor = getCoordinate(ds);
						a++;
						path.moveTo(coor);
						startP = coor;
						first = false;
					}
					QPointF p1 = getCoordinate(ds);
					QPointF p2 = getCoordinate(ds);
					QPointF p3 = getCoordinate(ds);
					a += 3;
					path.cubicTo(p1, p2, p3);
				}
				if (currentItem != NULL)
				{
					currentItem->PoLine.fromQPainterPath(path);
					QRectF bBoxO = path.boundingRect();
					if (bBoxO.x() < 0)
						currentItem->PoLine.translate(-bBoxO.x(), 0);
					if (bBoxO.y() < 0)
						currentItem->PoLine.translate(0, -bBoxO.y());
					finishItem(currentItem);
					if (currentItem != NULL)
					{
						handleLineStyle(currentItem, flags, lineColor);
						handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
					}
				}
				createObjCode = 0;
				currentItem = NULL;
			}
			else if (createObjCode == 4)
			{
				bool first = true;
				QPointF startP;
				int a = 0;
				QPainterPath path;
				while (a < nrOfPoints-1)
				{
					if (first)
					{
						QPointF coor = getCoordinate(ds);
						a++;
						path.moveTo(coor);
						startP = coor;
						first = false;
					}
					QPointF p1 = getCoordinate(ds);
					a++;
					QPointF p2 = getCoordinate(ds);
					a++;
					path.quadTo(p1, p2);
				}
				if (currentItem != NULL)
				{
					currentItem->PoLine.fromQPainterPath(path);
					QRectF bBoxO = path.boundingRect();
					if (bBoxO.x() < 0)
						currentItem->PoLine.translate(-bBoxO.x(), 0);
					if (bBoxO.y() < 0)
						currentItem->PoLine.translate(0, -bBoxO.y());
					finishItem(currentItem);
					if (currentItem != NULL)
					{
						handleLineStyle(currentItem, flags, lineColor);
						handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
					}
				}
				createObjCode = 0;
				currentItem = NULL;
			}
			break;
		case 7:
			cmdText = "";
			decodeSymbol(ds);
			printMSG = false;
			break;
		case 8:
			cmdText += "DRW Text";
			if (createObjCode == 5)
			{
				QString tx = QString(cmdData.left(nrOfChars));
				QStringList parList = tx.split(QChar(13));
				double yp = 0;
				QPainterPath path;
				QString fontN = "Arial";
				if (fontMap.contains(fontID))
					fontN = fontMap[fontID];
				QFont textFont = QFont(fontN, fontSize * 0.8);
				QFontMetrics fm(textFont);
				for (int a = 0; a < parList.size(); a++)
				{
					path.addText( 0, yp, textFont, parList[a].trimmed());
					yp += fm.lineSpacing();
				}
				QTransform txS;
				QRectF bbox = path.boundingRect();
				txS = QTransform();
				txS.scale(scaleFactor, scaleFactor);
				path = txS.map(path);
				txS = QTransform();
				bbox = path.boundingRect();
				txS.translate(-bbox.x(), -bbox.y());
				txS.translate(0, fm.leading() * scaleFactor);
				path = txS.map(path);
				if (currentItem != NULL)
				{
					currentItem->PoLine.fromQPainterPath(path);
					currentItem->setWidth(bbox.width());
					finishItem(currentItem, false);
				}
				createObjCode = 0;
				currentItem = NULL;
			}
			break;
		case 9:
			cmdText += "DRW Color";
			break;
		case 10:
			cmdText += "DRW Color Flag";
			break;
		case 11:
			cmdText += "DRW Preview Bitmap";
			handlePreviewBitmap(ds);
			break;
		case 14:
			cmdText += "DRW View";
			break;
		case 15:
			cmdText += "DRW Old Grid";
			break;
		case 16:
			cmdText += QString("DRW Curr Overlay Data %1").arg(QString(cmdData.toHex().left(20)));
			break;
		case 17:
			cmdText += QString("DRW Visible Data %1").arg(QString(cmdData.toHex().left(20)));
			break;
		case 18:
			cmdText += QString("DRW Comment Data %1").arg(QString(cmdData.toHex().left(20)));
			break;
		case 19:
			cmdText += QString("DRW Info Data %1").arg(QString(cmdData).left(20));
			break;
		case 20:
			cmdText += "DRW Bitmap";
			break;
		case 21:
			ds >> fontID;
			ds.device()->seek(0x13);
			fontName = "";
			ds >> chData;
			while (chData != 0)
			{
				fontName += QChar(chData);
				ds >> chData;
			}
			fontName = fontName.trimmed();
			fontName.replace( QRegExp( "'" ) , QChar( ' ' ) );
			{
				textFont = m_Doc->itemToolPrefs().textFont;
				bool found = false;
				SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
				for ( ; it.hasNext(); it.next())
				{
					QString fn = it.current().scName();
					if (fn == fontName)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					if (importerFlags & LoadSavePlugin::lfCreateThumbnail)
						fontName = PrefsManager::instance()->appPrefs.itemToolPrefs.textFont;
					else
					{
						if (!PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(fontName))
						{
							qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
							MissingFont *dia = new MissingFont(0, fontName, m_Doc);
							dia->exec();
							textFont = dia->getReplacementFont();
							delete dia;
							qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
							PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[fontName] = textFont;
							fontName = textFont;
						}
						else
							fontName = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[fontName];
					}
				}
			}
			fontMap.insert(fontID, fontName);
			cmdText += QString("DRW Font %1").arg(fontName);
			break;
		case 22:
			cmdText += "DRW Grid";
			break;
		case 23:
			cmdText += QString("DRW Overlay Name Data %1").arg(QString(cmdData).left(20));
			printMSG = true;
			break;
		case 24:
			cmdText += "DRW Dimensions";
			break;
		case 25:
			ds >> data16;
			scaleFactor = (1.0 / static_cast<double>(data16)) * 72.0;
			cmdText += QString("DRW Resolution %1").arg(data16);
			break;
		case 26:
			cmdText += "DRW Ruler";
			break;
		case 27:
			cmdText += "DRW Page";
			docWidth = getValue(ds);
			docHeight = getValue(ds);
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
				if (docWidth > docHeight)
					m_Doc->setPageOrientation(1);
				else
					m_Doc->setPageOrientation(0);
				m_Doc->setPageSize("Custom");
				m_Doc->changePageMargins(0, 0, 0, 0, docHeight, docWidth, docHeight, docWidth, m_Doc->pageOrientation(), m_Doc->pageSize(), m_Doc->currentPage()->pageNr(), 0);
				cmdText = QString("DRW Page  Width %1  Height %2").arg(docWidth).arg(docHeight);
			}
			break;
		case 28:
			cmdText += "DRW Pattern";
			ds >> data8;
			pattern.resize(16);
			ds.readRawData(pattern.data(), 16);
			patternDataMap.insert(data8, pattern);
			printMSG = true;
			break;
		case 29:
			cmdText += "DRW Locked";
			break;
		case 30:
			ds >> data8;
			index = data8;
			ds >> data8;
			gradient.type = data8;
			ds >> data8;
			gradient.xOffset = data8 / 100.0;
			ds >> data8;
			gradient.yOffset = data8 / 100.0;
			ds >> data16;
			gradient.angle = data16 / 10.0;
			gradientMap.insert(index, gradient);
			cmdText += QString("DRW Gradient  Index: %1 Type: %2 Offsets: %3 %4 Angle: %5").arg(index).arg(gradient.type).arg(gradient.xOffset).arg(gradient.yOffset).arg(gradient.angle);
			printMSG = true;
			break;
		case 31:
			cmdText += "DRW Text Hdr";
			ds >> data8;					// Version
			ds >> data8;					// vertical alignment
			cmdText += QString(" VAlign %1").arg(data8);
			ds >> data16;					// MemFlags ????
			ds >> data16;					// Textrotation
			ds >> fontID;					// Font Nr
			cmdText += QString(" Font %1").arg(fontID);
			ds >> fontStyle;				// Style
			ds >> fontWidth;				// Width
			ds >> fontSize;					// Height
			cmdText += QString(" Size %1").arg(fontSize);
			ds >> nrOfParagraphs;			// Nr of paragraph records
			paragraphCounter = 0;
			cmdText += QString(" NoPara %1").arg(nrOfParagraphs);
			paragraphList.clear();
			for (quint16 a = 0; a < nrOfParagraphs; a++)
			{
				DRWParagraph para;
				ds >> data16;
				ds >> data16;
				ds >> data16;
				ds >> para.paragraphAlignment;
				ds.skipRawData(18);
				ds >> para.paragraphLen;
				para.paragraphLen -= 17;
				ds.skipRawData(4);
				paragraphList.append(para);
			}
			break;
		case 32:
			cmdText += "DRW Band";
/* For this record the documentation is completly wrong

	offs	meaning
	0		X-Offset
	2		Y-Offset
	4		bytes per row
	6		number of rows stored in this record
	8+		Image Data as raw uncompressed values, rows are aligned to even bytes
*/
			if (imageValid)
			{
				quint16 xoff, yoff, len, count;
				ds >> xoff >> yoff >> len >> count;
				if (bitsPerPixel == 24)
				{
					for (quint16 y = 0; y < count; y++)
					{
						QRgb *q = (QRgb*)(tmpImage.scanLine(yoff + y));
						for (quint16 x = 0; x < imageWidth; x++)
						{
							quint8 r, g, b;
							ds >> r >> g >> b;
							*q = qRgba(r, g, b, 255);
							q++;
						}
						scanLinesRead++;
					}
				}
				else if (bitsPerPixel == 8)
				{
					for (quint16 y = 0; y < count; y++)
					{
						QRgb *q = (QRgb*)(tmpImage.scanLine(yoff + y));
						int pos = ds.device()->pos();
						for (quint16 x = 0; x < imageWidth; x++)
						{
							quint8 r;
							ds >> r;
							*q = qRgba(r, r, r, 255);
							q++;
						}
						QByteArray data;
						data.resize(imageWidth);
						ds.device()->seek(pos);
						ds.readRawData(data.data(), imageWidth);
						ds.device()->seek(pos + len);
						memcpy(tmpImage2.scanLine(yoff + y), data.data(), imageWidth);
						scanLinesRead++;
					}
				}
				if (scanLinesRead >= imageHeight)
				{
					if (currentItem != NULL)
					{
						currentItem->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_drw_XXXXXX.png");
						currentItem->tempImageFile->open();
						QString fileName = getLongPathName(currentItem->tempImageFile->fileName());
						currentItem->tempImageFile->close();
						currentItem->isInlineImage = true;
						tmpImage.save(fileName, "PNG");
						m_Doc->loadPict(fileName, currentItem);
						currentItem->setImageScalingMode(false, false);
					}
					imageValid = false;
					tmpImage = QImage();
				}
			}
			break;
		case 33:
			ds >> data16;
			cmdText += QString("DRW Symbolversion %1").arg(data16);
			break;
		case 34:
			cmdText += "DRW Text Para";
			ds.device()->seek(0x11);
			if (createObjCode == 6)
			{
				if (currentItem != NULL)
				{
					DRWParagraph para = paragraphList.at(paragraphCounter);
					paragraphCounter++;
					ParagraphStyle newStyle;
					newStyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(1));
					newStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(para.paragraphAlignment));
					newStyle.charStyle().setFontSize(fontSize * scaleFactor * 10.0 * 0.8);
					QString fontN(m_Doc->itemToolPrefs().textFont);
					if (fontMap.contains(fontID))
						fontN = fontMap[fontID];
					newStyle.charStyle().setFont((*m_Doc->AllFonts)[fontN]);
					newStyle.charStyle().setFillColor(fontColor);
					newStyle.setLineSpacing(newStyle.charStyle().font().height(fontSize * scaleFactor * 10.0 * 0.8));
					if (para.paragraphLen > 0)
					{
						int pos = currentItem->itemText.length();
						QByteArray data;
						data.resize(para.paragraphLen);
						ds.readRawData(data.data(), para.paragraphLen);
						QString chars = QString(data);
						if (!chars.isEmpty())
						{
							currentItem->itemText.insertChars( -1, chars);
							currentItem->itemText.applyStyle(pos, newStyle);
						}
						if (nrOfParagraphs > 0)
							currentItem->itemText.insertChars(-1, SpecialChars::PARSEP);
					}
				}
			}
			break;
		case 35:
			cmdText += "DRW Colortable";
			if (currentItem != NULL)
			{
				if (currentItem->asImageFrame())
				{
					QString fileName = getLongPathName(currentItem->tempImageFile->fileName());
					if (!fileName.isEmpty())
					{
						QVector<QRgb> colors;
						for (quint16 cc = 0; cc < 255; cc++)	// now reading ColorTable, exactly 1024 bytes
						{
							quint8 r, g, b, a;
							ds >> r >> g >> b >> a;				// values are stored in RGB order
							if ((r == rTrans) && (g == gTrans) && (b == bTrans))
								colors.append(qRgba(r, g, b, 0));
							else
								colors.append(qRgb(r, g, b));
						}
						tmpImage2.setColorTable(colors);
						tmpImage2 = tmpImage2.convertToFormat(QImage::Format_ARGB32);
						tmpImage2.save(fileName, "PNG");
						m_Doc->loadPict(fileName, currentItem, true);
					}
				}
			}
			break;
		case 36:
			cmdText += "DRW Text Extra";
			break;
		case 37:
			cmdText += "DRW Max Link ID";
			break;
		case 44:
			cmdText += "Skip Symbols";
			break;
		case 254:
			cmdText += "DRW EOF";
			decodeSymbol(ds, true);
			printMSG = true;
			break;
		case 255:
			cmdText += QString("DRW Start File");
			printMSG = true;
			break;
		default:
			cmdText += QString("Unknown Cmd-Nr %1  Data %2 Size %3").arg(cmd).arg(QString(cmdData.toHex().left(64))).arg(cmdData.size());
			break;
	}
	printMSG = false;
	if (printMSG)
	{
		qDebug() << cmdText; // << QString("at %1").arg(pos, 8, 16);
//		qDebug() << "\tData:" << cmdData.toHex().left(32);
	}
}

void DrwPlug::decodeSymbol(QDataStream &ds, bool last)
{
	symbolCount++;
	QString cmdText = QString("Record %1 Symbol %2 Type:").arg(recordCount).arg(symbolCount);
	bool printMSG = false;
	double bX = 0.0;
	double bY = 0.0;
	double groupX = 0.0;
	double groupY = 0.0;
	DRWObjectList gList;
	DRWGroup gElements;
	DRWGroup cElements;
	if (groupStack.count() > 0)
	{
		cElements = groupStack.top();
		bX = cElements.xoffset;
		bY = cElements.yoffset;
		if (cElements.nrOfItems != -1)
		{
			while (groupStack.count() > 1)
			{
				if (cElements.nrOfItems == cElements.counter)
				{
					listStack.pop();
					DRWGroup popped = groupStack.pop();
					cElements = groupStack.top();
					tmpSel->clear();
					for (int dre = 0;  dre < popped.GElements.count(); ++dre)
					{
						tmpSel->addItem(popped.GElements.at(dre), true);
					}
					bX = cElements.xoffset;
					bY = cElements.yoffset;
					uint selectedItemCount = tmpSel->count();
					if (selectedItemCount > 0)
					{
						double scx = 1.0;
						double scy = 1.0;
						QPainterPath gesPa;
						bool firstP = true;
						for (uint i = 0; i < selectedItemCount; ++i)
						{
							QPainterPath pa;
							PageItem *item = tmpSel->itemAt(i);
							item->PoLine.translate(item->xPos(), item->yPos());
							pa = item->PoLine.toQPainterPath(false);
							if (!pa.isEmpty())
							{
								const QPainterPath::Element &elm = pa.elementAt(0);
								QPointF lastP = pa.currentPosition();
								bool conn = false;
								bool conn2 = false;
								if ((fabs(lastP.x() - elm.x) < 1) && (fabs(lastP.y() - elm.y) < 1))
								{
									pa.closeSubpath();
									conn = true;
								}
								if (!gesPa.isEmpty())
								{
									const QPainterPath::Element &elm2 = gesPa.elementAt(0);
									QPointF lastP2 = gesPa.currentPosition();
									if ((fabs(lastP2.x() - elm2.x) < 1) && (fabs(lastP2.y() - elm2.y) < 1))
									{
										gesPa.closeSubpath();
										conn2 = true;
									}
								}
								if ((firstP) || (conn) || (conn2))
								{
									gesPa.addPath(pa);
									firstP = false;
								}
								else
									gesPa.connectPath(pa);
							}
						}
						if (!gesPa.isEmpty())
						{
							QRectF bb = gesPa.controlPointRect();
							if (popped.rotationAngle != 0)
							{
								QTransform mt;
								mt.translate(-bb.x(), -bb.y());
								gesPa = mt.map(gesPa);
								QTransform ma;
								ma.translate(popped.posPivot.x(), popped.posPivot.y());
								ma.rotate(-popped.rotationAngle / 10.0);
								gesPa = ma.map(gesPa);
							}
							bb = gesPa.controlPointRect();
							QTransform mt;
							mt.translate(-bb.x(), -bb.y());
							gesPa = mt.map(gesPa);
							if ((bb.width() != 0) && (bb.height() != 0) && (popped.width != 0) && (popped.height != 0))
							{
								if (bb.width() != popped.width)
									scx = popped.width / bb.width();
								if (bb.height() != popped.height)
									scy = popped.height / bb.height();
								QTransform ms;
								ms.scale(scx, scy);
								gesPa = ms.map(gesPa);
							}
							if (popped.filled)
								gesPa.closeSubpath();
							FPointArray res;
							res.fromQPainterPath(gesPa);
							PageItem *ite = tmpSel->takeItem(0);
							ite->setXYPos(popped.xoffset + baseX, popped.yoffset + baseY);
							ite->PoLine = res.copy();
							FPoint wh = getMaxClipF(&ite->PoLine);
							ite->setWidthHeight(wh.x(),wh.y());
							ite->OldB2 = ite->width();
							ite->OldH2 = ite->height();
							if (popped.filled)
								ite->setFillColor(popped.fillColor);
							else
								ite->setFillColor(CommonStrings::None);
							ite->setLineWidth(popped.lineWidth);
							ite->setLineWidth(ite->lineWidth() / qMin(scx, scy));
							handleLineStyle(ite, popped.flags, popped.lineColor);
							if (popped.filled)
								handleGradient(ite, popped.patternIndex, popped.fillColor, popped.backColor, QRectF(0, 0, ite->width(), ite->height()));
							groupStack.top().GElements.append(ite);
							listStack.top().GElements.append(ite);
						}
						selectedItemCount = tmpSel->count();
						for (uint i = 0; i < selectedItemCount; ++i)
						{
							Elements.removeAll(tmpSel->itemAt(i));
							listStack.top().GElements.removeAll(tmpSel->itemAt(i));
						}
						m_Doc->itemSelection_DeleteItem(tmpSel);
					}
					tmpSel->clear();
				}
				else
					break;
			}
			groupStack.top().counter++;
		}
	}
	if (listStack.count() > 1)
	{
		while (listStack.count() > 1)
		{
			if (listStack.top().nrOfItems == listStack.top().counter)
			{
				DRWObjectList popped = listStack.pop();
				tmpSel->clear();
				for (int dre = 0;  dre < popped.GElements.count(); ++dre)
				{
					tmpSel->addItem(popped.GElements.at(dre), true);
				}
				uint selectedItemCount = tmpSel->count();
				if (selectedItemCount > 0)
				{
					if (popped.rotationAngle != 0)
					{
						PageItem* currItem;
						QTransform ma;
						ma.translate(popped.posPivot.x(), popped.posPivot.y());
						ma.rotate(-popped.rotationAngle / 10.0);
						FPoint n;
						for (int a = 0; a < tmpSel->count(); ++a)
						{
							currItem = tmpSel->itemAt(a);
							n = FPoint(currItem->xPos(), currItem->yPos());
							currItem->setXYPos(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
							currItem->rotateBy(-popped.rotationAngle / 10.0);
						}
					}
					tmpSel->setGroupRect();
					QRectF gr = tmpSel->getGroupRect();
					double dx = popped.groupItem->xPos() - gr.x();
					double dy = popped.groupItem->yPos() - gr.y();
					for (int a = 0; a < tmpSel->count(); ++a)
					{
						tmpSel->itemAt(a)->moveBy(dx, dy);
					}
					tmpSel->setGroupRect();
					if ((popped.scaleX != 0) || (popped.scaleY != 0))
					{
						if ((tmpSel->width() != 0) && (tmpSel->height() != 0) && (popped.width != 0) && (popped.height != 0))
						{
							double scx = 1.0;
							if (tmpSel->width() != popped.width)
								scx = popped.width / tmpSel->width();
							double scy = 1.0;
							if (tmpSel->height() != popped.height)
								scy = popped.height / tmpSel->height();
							m_Doc->scaleGroup(scx, scy, true, tmpSel);
						}
					}
					listStack.top().GElements.append(popped.groupItem);
					for (uint i = 0; i < selectedItemCount; ++i)
					{
						PageItem *item = tmpSel->itemAt(i);
						popped.groupItem->groupItemList.append(item);
						item->gXpos = item->xPos() - popped.groupItem->xPos();
						item->gYpos = item->yPos() - popped.groupItem->yPos();
					}
					if (popped.itemGroupName.isEmpty())
						popped.groupItem->setItemName( tr("Group%1").arg(m_Doc->GroupCounter));
					else
						popped.groupItem->setItemName(popped.itemGroupName);
					popped.groupItem->AutoName = false;
				}
				m_Doc->GroupCounter++;
				tmpSel->clear();
			}
			else
				break;
		}
		listStack.top().counter++;
		groupX = listStack.top().groupX;
		groupY = listStack.top().groupY;
	}
	if (last)
		return;
/*	if ((symbolCount > 31) && (symbolCount < 33))
	{
		QFile f(QString("/home/franz/cmddatas%1.bin").arg(symbolCount));
		f.open(QIODevice::WriteOnly);
		f.write(cmdData);
		f.close();
	} */
	int z;
	quint8 data8, appFlags;
	quint16 dummy, nPoints, nItems;
	double boundingBoxXO, boundingBoxYO, boundingBoxWO, boundingBoxHO, cornerRadius;
	QRectF bBoxO;
	QString fillC = CommonStrings::None;
	createObjCode = 0;
	currentItem = NULL;
	ds >> data8;							// reading Symbol Type
// now reading common values
	ds >> flags;
	QPainterPath path;
	QPointF posEnd, posMid, posStart;
	QTransform mat;
	QLineF sLin, eLin;
	FPoint wh;
	QPointF position = getCoordinate(ds);
	double boundingBoxX = getValue(ds);
	double boundingBoxY = getValue(ds);
	double boundingBoxW = getValue(ds);
	double boundingBoxH = getValue(ds);
	bBox = QRectF(QPointF(boundingBoxX, boundingBoxY), QPointF(boundingBoxW, boundingBoxH)).normalized();
	rotationAngle = getRawValue(ds);
	scaleX = getRawValue(ds);
	scaleY = getRawValue(ds);
	double rotS, rotE;
	double posX = baseX + bBox.x() + bX + groupX;
	double posY = baseY + bBox.y() + bY + groupY;
	lineColor = getColor(ds);
	ds >> dummy;		// handle
	ds >> dummy;		// next
	ds >> dummy;
	ds >> dummy;		// prev
	ds >> dummy;
	switch (data8)			// Symbol type is in the first data byte
	{
		case 0:
			cmdText += "elliptical Arc";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			posStart = getCoordinate(ds);
			posEnd = getCoordinate(ds);
			if (posStart == posEnd)
				break;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			getCommonData(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO)).normalized();
			sLin = QLineF(bBoxO.center(), posStart);
			eLin = QLineF(bBoxO.center(), posEnd);
			rotS = sLin.angle();
			rotE = eLin.angle();
			if (rotS > rotE)
				rotS = rotS - 360;
			rotE = rotE - rotS;
			path = QPainterPath();
			path.arcMoveTo(bBoxO, rotS);
			path.arcTo(bBoxO, rotS, rotE);
			scaleX = 1;
			scaleY = 1;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.controlPointRect();
			currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			break;
		case 1:
			cmdText += "filled Polygon";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			nrOfPoints = nPoints;
			createObjCode = 1;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			break;
		case 2:
			getCommonData(ds);
			ds.device()->seek(0x26);
			ds >> dummy;
			if (dummy > 0)
			{
				z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, posX, posY, bBox.width(), bBox.height(), 0, fillC, fillC, true);
				gList.groupItem = m_Doc->Items->at(z);
				gList.groupX = groupX + bBox.x();
				gList.groupY = groupY + bBox.y();
				gList.width = bBox.width();
				gList.height = bBox.height();
				gList.scaleX = scaleX;
				gList.scaleY = scaleY;
				gList.rotationAngle = rotationAngle;
				gList.nrOfItems = dummy;
				gList.counter = 0;
				gList.posPivot = posPivot;
				gList.itemGroupName = "";
				gList.GElements.clear();
				listStack.push(gList);
				gList.groupItem->ClipEdited = true;
				gList.groupItem->FrameType = 3;
				gList.groupItem->setTextFlowMode(PageItem::TextFlowDisabled);
				gList.groupItem->OldB2 = gList.groupItem->width();
				gList.groupItem->OldH2 = gList.groupItem->height();
				gList.groupItem->updateClip();
				Elements.append(gList.groupItem);
			}
			cmdText += QString("Group  Count %1").arg(dummy);
			break;
		case 3:
			cmdText += "Ellipse";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			cornerRadius = getValue(ds);
			ds >> appFlags;
			getCommonData(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			finishItem(currentItem);
			if (currentItem != NULL)
			{
				handleLineStyle(currentItem, flags, lineColor);
				handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			}
			break;
		case 5:
			cmdText += "Text";
			ds >> dummy;
			ds >> fontID;
			ds >> nrOfChars;
			ds >> fontSize;
			fontColor = lineColor;
			createObjCode = 5;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), 0, lineColor, CommonStrings::None, true);
			currentItem = m_Doc->Items->at(z);
			scaleX = 0;
			break;
		case 6:
			cmdText += "Line";
			posEnd = getCoordinate(ds);
			posStart = getCoordinate(ds);
			if (posStart == posEnd)
				break;
			ds >> patternIndex;
			getCommonData(ds);
			lineWidth = patternIndex * scaleFactor;
			path = QPainterPath();
			path.moveTo(posStart);
			path.lineTo(posEnd);
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.boundingRect();
			currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			break;
		case 8:
			cmdText += "Polyline";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			nrOfPoints = nPoints;
			createObjCode = 3;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			break;
		case 9:
			cmdText += "Pie Wedge";
			printMSG = true;
			break;
		case 10:
		case 11:
			if (data8 == 10)
				cmdText += "Rectangle";
			else
				cmdText += "Rounded Rectangle";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			cornerRadius = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			ds >> appFlags;
			getCommonData(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			if (currentItem != NULL)
			{
				if (data8 == 11)
					currentItem->setCornerRadius(cornerRadius);
				handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			}
			break;
		case 13:
			cmdText += "filled Ellipse";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			cornerRadius = getValue(ds);
			ds >> appFlags;
			getCommonData(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			if (currentItem != NULL)
				handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			break;
		case 14:
			cmdText += "elliptical Arc, clockwise";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			posStart = getCoordinate(ds);
			posEnd = getCoordinate(ds);
			if (posStart == posEnd)
				break;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			getCommonData(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO)).normalized();
			sLin = QLineF(bBoxO.center(), posStart);
			eLin = QLineF(bBoxO.center(), posEnd);
			rotS = sLin.angle();
			rotE = eLin.angle();
			if (rotS < rotE)
				rotS = rotS + 360;
			rotE = rotS - rotE;
			path = QPainterPath();
			path.arcMoveTo(bBoxO, rotS);
			path.arcTo(bBoxO, rotS, -rotE);
			scaleX = 1;
			scaleY = 1;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.controlPointRect();
			currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			break;
		case 15:
			cmdText += "filled parabolic Arc";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			posStart = getCoordinate(ds);
			posMid = getCoordinate(ds);
			posEnd = getCoordinate(ds);
			if (posStart == posEnd)
				break;
			getCommonData(ds);
			path = QPainterPath();
			path.moveTo(posStart);
			path.cubicTo(posMid, posMid, posEnd);
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.controlPointRect();
			currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			finishItem(currentItem);
			if (currentItem != NULL)
			{
				handleLineStyle(currentItem, flags, lineColor);
				handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			}
			break;
		case 16:
			cmdText += "filled quadratic Spline";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			nrOfPoints = nPoints;
			createObjCode = 4;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			break;
		case 17:
		case 20:
			ds >> patternIndex;
			fillColor = getColor(ds);
			ds.device()->seek(0x2B);
			ds >> nItems;
			ds >> dummy;
			ds >> appFlags;
			getCommonData(ds);
			gElements.xoffset = bX + bBox.x() + groupX;
			gElements.yoffset = bY + bBox.y() + groupY;
			gElements.width = bBox.width();
			gElements.height = bBox.height();
			gElements.lineWidth = lineWidth;
			gElements.scaleX = scaleX;
			gElements.scaleY = scaleY;
			gElements.rotationAngle = rotationAngle;
			gElements.posPivot = posPivot;
			if (data8 == 17)
				gElements.filled = true;
			else
				gElements.filled = false;
			gElements.nrOfItems = nItems;
			gElements.counter = 0;
			gElements.patternIndex = patternIndex;
			gElements.flags = flags;
			gElements.fillColor = fillColor;
			gElements.lineColor = lineColor;
			gElements.backColor = backColor;
			groupStack.push(gElements);
			gList.groupX = groupX;
			gList.groupY = groupY;
			gList.width = bBox.width();
			gList.height = bBox.height();
			gList.nrOfItems = 0xFFFF;
			gList.counter = 0;
			listStack.push(gList);
			cmdText += QString("filled complex Object Count %1  Scale %2 %3 LW %4").arg(nItems).arg(scaleX).arg(scaleY).arg(lineWidth);
			break;
		case 18:
			cmdText += "parabolic Arc";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			posStart = getCoordinate(ds);
			posMid = getCoordinate(ds);
			posEnd = getCoordinate(ds);
			if (posStart == posEnd)
				break;
			getCommonData(ds);
			path = QPainterPath();
			path.moveTo(posStart);
			path.cubicTo(posMid, posMid, posEnd);
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.controlPointRect();
			currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			break;
		case 19:
			cmdText += "quadratic Spline";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			nrOfPoints = nPoints;
			createObjCode = 4;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			break;
		case 22:
			cmdText += "Bitmap monochrome";
			ds >> dummy;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			ds >> bitsPerPixel;
			ds >> bytesScanline;
			ds >> planes;
			ds >> imageHeight;
			ds >> imageWidth;
			ds >> dummy;
			ds >> rTrans;
			ds >> gTrans;
			ds >> bTrans;
			getCommonData(ds);
			if ((bitsPerPixel == 24) || (bitsPerPixel == 8))
			{
				z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, posX, posY, bBox.width(), bBox.height(), lineWidth, CommonStrings::None, CommonStrings::None, true);
				currentItem = m_Doc->Items->at(z);
				finishItem(currentItem);
				scanLinesRead = 0;
				tmpImage = QImage(imageWidth, imageHeight, QImage::Format_ARGB32);
				if (bitsPerPixel == 8)
					tmpImage2 = QImage(imageWidth, imageHeight, QImage::Format_Indexed8);
				imageValid = true;
			}
			break;
		case 23:
			cmdText += "Bezier line";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			nrOfPoints = nPoints;
			createObjCode = 2;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			break;
		case 24:
			cmdText += "filled Bezier line";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			nrOfPoints = nPoints;
			createObjCode = 2;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			break;
		case 25:
			cmdText += "Rich Text";
			fontColor = lineColor;
			getCommonData(ds);
			createObjCode = 6;
			z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, posX, posY, bBox.width(), bBox.height(), 0, CommonStrings::None, CommonStrings::None, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
			finishItem(currentItem);
			break;
		case 26:
			cmdText += "virtual Bitmap";
			printMSG = true;
			break;
		case 27:
			cmdText += "simple Clip Path";
			printMSG = true;
			break;
		case 28:
			cmdText += "tiled Clip Path";
			printMSG = true;
			break;
		case 29:
			cmdText += "Path Text";
			printMSG = true;
			break;
		default:
			cmdText += "Unknown";
			break;
	}
	printMSG = false;
	if (printMSG)
	{
		if (currentItem != NULL)
			qDebug() << cmdText << " " << currentItem->itemName();
		else
			qDebug() << cmdText;
//		if (imageValid)
//			qDebug() << "Bits/Pixel" << bitsPerPixel << "Bytes" << bytesScanline << "Planes" << planes << "Height" << imageHeight << "Width" << imageWidth;
//		qDebug() << "Pos" << rotS << " --> " << rotE << " Box " << boundingBoxWO << boundingBoxHO;
//		qDebug() << "Rot" << rotationAngle << "Bounding Box" << bBoxO;
//		qDebug() << "Line" << lineColor << "LWidth" << lineWidth << "Fill" << fillColor;
//		qDebug() << "Scale" << scaleX << " " << scaleY;
//		qDebug() << QString("Flags %1").arg(flags, 8, 2, QChar('0')) << QString("Pattern %1").arg(patternIndex, 2, 16, QChar('0'));
//		if (createObjCode == 4)
//		{
//			qDebug() << "Expecting" << nrOfPoints;
//		}
	}
}

void DrwPlug::handleLineStyle(PageItem* currentItem, quint8 flags, QString lineColor)
{
	if ((flags & 0x0F) == 5)
		currentItem->setLineColor(CommonStrings::None);
	else
		currentItem->setLineColor(lineColor);
	if ((flags & 0x0F) == 0)
		currentItem->setLineStyle(Qt::SolidLine);
	else if ((flags & 0x0F) == 1)
		currentItem->setLineStyle(Qt::DashLine);
	else if ((flags & 0x0F) == 2)
		currentItem->setLineStyle(Qt::DotLine);
	else if ((flags & 0x0F) == 3)
		currentItem->setLineStyle(Qt::DashDotLine);
	else
		currentItem->setLineStyle(Qt::SolidLine);
}

void DrwPlug::handleGradient(PageItem* currentItem, quint8 patternIndex, QString fillColor, QString backColor, QRectF bBox)
{
	if ((fillColor == CommonStrings::None) || (backColor == CommonStrings::None))
		return;
	if ((patternIndex > 0x40) && (patternIndex < 0x80))
	{
		quint8 ind = patternIndex - 0x40;
		DRWGradient grad;
		if (gradientMap.contains(ind))
		{
			grad = gradientMap[ind];
			if (grad.xOffset > 1)
				grad.xOffset -= 1;
			if (grad.yOffset > 1)
				grad.yOffset -= 1;
			double xoff = bBox.width() * grad.xOffset;
			double yoff = bBox.height() * grad.yOffset;
			VGradient FillGradient = VGradient(VGradient::linear);
			FillGradient.clearStops();
			const ScColor& gradC1 = m_Doc->PageColors[fillColor];
			FillGradient.addStop( ScColorEngine::getRGBColor(gradC1, m_Doc), 0.0, 0.5, 1.0, fillColor, 100 );
			const ScColor& gradC2 = m_Doc->PageColors[backColor];
			if (grad.type == 1)
			{
				FillGradient.addStop( ScColorEngine::getRGBColor(gradC2, m_Doc), qMin(grad.yOffset, 0.99), 0.5, 1.0, backColor, 100 );
				FillGradient.addStop( ScColorEngine::getRGBColor(gradC1, m_Doc), 1.0, 0.5, 1.0, fillColor, 100 );
				currentItem->GrType = 6;
				currentItem->fill_gradient = FillGradient;
				currentItem->setGradientVector(bBox.width() / 2.0, 0, bBox.width() / 2.0, bBox.height(), 0, 0, 1, 0);
			}
			else
			{
				FillGradient.addStop( ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0, 0.5, 1.0, backColor, 100 );
				currentItem->GrType = 7;
				currentItem->fill_gradient = FillGradient;
				currentItem->setGradientVector(xoff, yoff, qMax(bBox.width(), bBox.height()), qMax(bBox.width(), bBox.height()), xoff, yoff, 1, 0);
			}
		}
	}
	else if (((patternIndex > 0x80) && (patternIndex < 0xC0)) || (patternIndex > 0xC0))
	{
		int ind;
		if (patternIndex > 0xC0)
			ind = patternIndex - 0xC0;
		else
			ind = patternIndex - 0x80;
		if (patternDataMap.contains(ind))
		{
			QColor back = ScColorEngine::getRGBColor(m_Doc->PageColors[fillColor], m_Doc);
			QColor fore = ScColorEngine::getRGBColor(m_Doc->PageColors[backColor], m_Doc);
			QString patNa = QString("%1%2%3").arg(back.name()).arg(fore.name()).arg(ind);
			QString patternName;
			if (!patternMap.contains(patNa))
			{
				uint oldNum = m_Doc->TotalItems;
				QByteArray data = patternDataMap[ind];
				QVector<QRgb> colors;
				int offs = 0;
				QImage image;
				if (patternIndex > 0xC0)
				{
					colors.append(qRgb(255,255,255));
					colors.append(back.rgb());
					image = QImage(16, 8, QImage::Format_Mono);
					image.setColorTable(colors);
					for (int rr = 0; rr < 8; rr++)
					{
						uchar *q = (uchar*)(image.scanLine(rr));
						*q++ = data[offs++];
						*q++ = data[offs++];
					}
					image = image.scaled(96, 48);
				}
				else
				{
					colors.append(back.rgb());
					colors.append(fore.rgb());
					image = QImage(8, 8, QImage::Format_Mono);
					image.setColorTable(colors);
					for (int rr = 0; rr < 8; rr++)
					{
						uchar *q = (uchar*)(image.scanLine(rr));
						*q++ = data[offs++];
						offs++;
					}
					image = image.scaled(48, 48);
				}
				image = image.convertToFormat(QImage::Format_ARGB32);
				ScPattern pat = ScPattern();
				pat.setDoc(m_Doc);
				PageItem* newItem = new PageItem_ImageFrame(m_Doc, 0, 0, 1, 1, 0, CommonStrings::None, CommonStrings::None);
				newItem->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_drw_XXXXXX.png");
				newItem->tempImageFile->open();
				QString fileName = getLongPathName(newItem->tempImageFile->fileName());
				newItem->tempImageFile->close();
				newItem->isInlineImage = true;
				image.setDotsPerMeterY(2834);
				image.setDotsPerMeterX(2834);
				image.save(fileName, "PNG");
				if (newItem->loadImage(fileName, false, 72, false))
				{
					pat.width = image.width();
					pat.height = image.height();
					pat.scaleX = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
					pat.scaleY = (72.0 / newItem->pixm.imgInfo.xres) * newItem->pixm.imgInfo.lowResScale;
					pat.pattern = newItem->pixm.qImage().copy();
					newItem->setWidth(pat.pattern.width());
					newItem->setHeight(pat.pattern.height());
					newItem->SetRectFrame();
					newItem->gXpos = 0.0;
					newItem->gYpos = 0.0;
					newItem->gWidth = pat.pattern.width();
					newItem->gHeight = pat.pattern.height();
					pat.items.append(newItem);
					newItem->ItemNr = pat.items.count();
				}
				patternName = "Pattern_"+newItem->itemName();
				patternName = patternName.trimmed().simplified().replace(" ", "_");
				m_Doc->addPattern(patternName, pat);
				patternMap.insert(patNa, patternName);
				m_Doc->TotalItems = oldNum;
			}
			else
				patternName = patternMap[patNa];
			importedPatterns.append(patternName);
			currentItem->setPattern(patternName);
			currentItem->setPatternTransform(16.6666, 16.6666, 0, 0, 0, 0, 0);
			currentItem->GrType = 8;
		}
	}
}

void DrwPlug::handlePreviewBitmap(QDataStream &ds)
{
	/* create a fake BMP header section */
	QByteArray header;
	header.resize(14);
	header.fill(0);
	QDataStream hs(&header, QIODevice::ReadWrite);
	hs.setByteOrder(QDataStream::LittleEndian);
	quint16 size;
	size = 0x4D42;
	hs << size;
	size = cmdData.size() + 14;
	hs << size;
	header.append(cmdData);
	thumbnailImage.loadFromData(header, "BMP");
}

QString DrwPlug::handleColor(ScColor &color, QString proposedName)
{
	QString tmpName = m_Doc->PageColors.tryAddColor(proposedName, color);
	if (tmpName == proposedName)
		importedColors.append(tmpName);
	return tmpName;
}


void DrwPlug::getCommonData(QDataStream &ds)
{
	quint16 dummy;
	ds.device()->seek(0x38);
	backColor = getColor(ds);
	lineWidth = getValue(ds);
	ds >> dummy;
	posPivot = getCoordinate(ds);
}

QString DrwPlug::getColor(QDataStream &ds)
{
	quint8 r, g, b, a;
	ds >> r >> g >> b >> a;
	ScColor color = ScColor(r, g, b);
	return handleColor(color, "FromDRW"+color.name());
}

void DrwPlug::finishItem(PageItem* ite, bool scale)
{
	if (ite->PoLine.size() < 4)
	{
		tmpSel->clear();
		tmpSel->addItem(ite, true);
		m_Doc->itemSelection_DeleteItem(tmpSel);
		currentItem = NULL;
		createObjCode = 0;
		tmpSel->clear();
		return;
	}
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	if (rotationAngle != 0)
	{
		QTransform ma;
		ma.translate(posPivot.x(), posPivot.y());
		ma.rotate(-rotationAngle / 10.0);
		ite->PoLine.map(ma);
		FPoint tp2(getMinClipF(&ite->PoLine));
		ite->PoLine.translate(-tp2.x(), -tp2.y());
	}
	if (scale)
	{
		if ((scaleX != 0) || (scaleY != 0))
		{
			QPainterPath pa = ite->PoLine.toQPainterPath(true);
			QRectF bb = pa.controlPointRect();
			double scx = 1.0;
			double scy = 1.0;
			if ((bb.width() != 0.0) && (ite->width() != 0.0))
				scx = ite->width() / bb.width();
			else
				scx = 1.0;
			if ((bb.height() != 0.0) && (ite->height() != 0.0))
				scy = ite->height() / bb.height();
			else
				scy = 1.0;
			ite->PoLine.scale(scx, scy);
			ite->setLineWidth(ite->lineWidth() / qMin(scx, scy));
		}
	}
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	Elements.append(ite);
	if (groupStack.count() > 1)
		groupStack.top().GElements.append(ite);
	if (listStack.count() > 1)
		listStack.top().GElements.append(ite);
	Coords.resize(0);
	Coords.svgInit();
}

double DrwPlug::getValue(QDataStream &ds)
{
	qint16 data16;
	ds >> data16;
	return data16 * scaleFactor;
}

double DrwPlug::getRawValue(QDataStream &ds)
{
	qint16 data16;
	ds >> data16;
	return static_cast<double>(data16);
}

QPointF DrwPlug::getCoordinate(QDataStream &ds)
{
	qint16 x, y;
	ds >> x >> y;
	return QPointF(x * scaleFactor, y * scaleFactor);
}
