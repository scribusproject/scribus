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

#include "importdrw.h"

#include "commonstrings.h"
#include "loadsaveplugin.h"
#include "pageitem_imageframe.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "rawimage.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scpattern.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "selection.h"
#include "ui/customfdialog.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"


extern SCRIBUS_API ScribusQApp * ScQApp;

DrwPlug::DrwPlug(ScribusDoc* doc, int flags)
{
	m_tmpSel=new Selection(this, false);
	m_Doc=doc;
	m_importerFlags = flags;
	m_interactive = (flags & LoadSavePlugin::lfInteractive);
	m_progressDialog = NULL;
}

QImage DrwPlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	m_baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	double b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	double h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	m_docWidth = b;
	m_docHeight = h;
	m_progressDialog = NULL;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	m_baseX = m_Doc->currentPage()->xOffset();
	m_baseY = m_Doc->currentPage()->yOffset();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		if (!m_thumbRead)
		{
			m_tmpSel->clear();
			QDir::setCurrent(CurDirP);
			if (Elements.count() > 1)
				m_Doc->groupObjectsList(Elements);
		}
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		QImage tmpImage;
		if (m_thumbRead)
		{
			tmpImage = m_thumbnailImage;
			tmpImage.setText("XSize", QString("%1").arg(m_docWidth));
			tmpImage.setText("YSize", QString("%1").arg(m_docHeight));
		}
		else
		{
			if (Elements.count() > 0)
			{
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					m_tmpSel->addItem(Elements.at(dre), true);
				}
				m_tmpSel->setGroupRect();
				double xs = m_tmpSel->width();
				double ys = m_tmpSel->height();
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
	m_interactive = (flags & LoadSavePlugin::lfInteractive);
	m_importerFlags = flags;
	m_cancel = false;
	double b, h;
	bool ret = false;
	QFileInfo fi = QFileInfo(fName);
	if ( !ScCore->usingGUI() )
	{
		m_interactive = false;
		showProgress = false;
	}
	m_baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	if ( showProgress )
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		m_progressDialog = new MultiProgressDialog( tr("Importing: %1").arg(fi.fileName()), CommonStrings::tr_Cancel, mw );
		QStringList barNames, barTexts;
		barNames << "GI";
		barTexts << tr("Analyzing File:");
		QList<bool> barsNumeric;
		barsNumeric << false;
		m_progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
		m_progressDialog->setOverallTotalSteps(3);
		m_progressDialog->setOverallProgress(0);
		m_progressDialog->setProgress("GI", 0);
		m_progressDialog->show();
		connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
		qApp->processEvents();
	}
	else
		m_progressDialog = NULL;
/* Set default Page to size defined in Preferences */
	b = 0.0;
	h = 0.0;
	if (m_progressDialog)
	{
		m_progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
	m_docWidth = b;
	m_docHeight = h;
	m_baseX = 0;
	m_baseY = 0;
	if (!m_interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		m_baseX = 0;
		m_baseY = 0;
		m_baseX = m_Doc->currentPage()->xOffset();
		m_baseY = m_Doc->currentPage()->yOffset();
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			m_baseX = 0;
			m_baseY = 0;
			m_baseX = m_Doc->currentPage()->xOffset();
			m_baseY = m_Doc->currentPage()->yOffset();
		}
	}
	if ((!ret) && (m_interactive))
	{
		m_baseX = m_Doc->currentPage()->xOffset();
		m_baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!m_interactive))
	{
		if (m_docWidth > m_docHeight)
			m_Doc->setPageOrientation(1);
		else
			m_Doc->setPageOrientation(0);
		m_Doc->setPageSize("Custom");
	}
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != NULL))
		m_Doc->view()->Deselect();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != NULL))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		m_tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (!(m_importerFlags & LoadSavePlugin::lfCreateDoc)))
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		if ((Elements.count() > 0) && (!ret) && (m_interactive))
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
					if (m_Doc->view() != NULL)
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
					m_tmpSel->addItem(Elements.at(dre), true);
				}
				m_tmpSel->setGroupRect();
				ScElemMimeData* md = ScriXmlDoc::WriteToMimeData(m_Doc, m_tmpSel);
				m_Doc->itemSelection_DeleteItem(m_tmpSel);
				m_Doc->view()->updatesOn(true);
				if (m_importedColors.count() != 0)
				{
					for (int cd = 0; cd < m_importedColors.count(); cd++)
					{
						m_Doc->PageColors.remove(m_importedColors[cd]);
					}
				}
				if (m_importedPatterns.count() != 0)
				{
					for (int cd = 0; cd < m_importedPatterns.count(); cd++)
					{
						m_Doc->docPatterns.remove(m_importedPatterns[cd]);
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
	if (m_interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if (!(flags & LoadSavePlugin::lfLoadAsPattern))
	{
		if ((showProgress) && (!m_interactive))
			m_Doc->view()->DrawNew();
	}
	qApp->restoreOverrideCursor();
	return success;
}

DrwPlug::~DrwPlug()
{
	if (m_progressDialog)
		delete m_progressDialog;
	delete m_tmpSel;
}

bool DrwPlug::convert(QString fn)
{
	QString tmp;
	m_Coords.resize(0);
	m_Coords.svgInit();
	m_importedColors.clear();
	m_importedPatterns.clear();
	DRWGroup gElements;
	gElements.xoffset = 0.0;
	gElements.yoffset = 0.0;
	gElements.nrOfItems = -1;
	gElements.counter = -1;
	m_groupStack.push(gElements);
	DRWObjectList gList;
	gList.groupX = 0.0;
	gList.groupY = 0.0;
	m_listStack.push(gList);
	m_scaleFactor = 0.15;
	m_lineWidth = 1.0;
	m_lineColor = "Black";
	m_fillColor = "Black";
	m_createObjCode = 0;
	m_nrOfPoints = 0;
	m_symbolCount = 0;
	m_recordCount = 0;
	m_imageValid = false;
	m_thumbRead = false;
	m_currentItem = NULL;
	if(m_progressDialog)
	{
		m_progressDialog->setOverallProgress(2);
		m_progressDialog->setLabel("GI", tr("Generating Items"));
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
			if (m_progressDialog)
			{
				m_progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
			if (cmd == 254)
				break;
			if ((m_importerFlags & LoadSavePlugin::lfCreateThumbnail) && (cmd == 11))
				m_thumbRead = true;
			if ((m_importerFlags & LoadSavePlugin::lfCreateThumbnail) && (cmd == 27) && (m_thumbRead))
				break;
		}
		if (Elements.count() == 0)
		{
			if (m_importedColors.count() != 0)
			{
				for (int cd = 0; cd < m_importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(m_importedColors[cd]);
				}
			}
			if (m_importedPatterns.count() != 0)
			{
				for (int cd = 0; cd < m_importedPatterns.count(); cd++)
				{
					m_Doc->docPatterns.remove(m_importedPatterns[cd]);
				}
			}
		}
		f.close();
	}
	if (m_progressDialog)
		m_progressDialog->close();
	return true;
}

void DrwPlug::decodeCmdData(QDataStream &ts, uint dataLen, quint8 cmd)
{
	m_cmdData.resize(0);
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
					m_cmdData.append(val);
					count++;
				}
			}
			else
			{
				m_cmdData.append(d);
				count++;
			}
		}
		else
		{
			m_cmdData.append(d);
			count++;
		}
	}
}

void DrwPlug::decodeCmd(quint8 cmd, int pos)
{
	m_recordCount++;
	bool printMSG = false;
/*	if ((recordCount > 29) && (recordCount < 33))
	{
		QFile f(QString("/home/franz/cmddatas%1.bin").arg(recordCount));
		f.open(QIODevice::WriteOnly);
		f.write(cmdData);
		f.close();
	} */
	QDataStream ds(m_cmdData);
	DRWGradient gradient;
	QByteArray pattern;
	quint8 data8, chData;
	quint16 data16;
	int index;
	QFont font;
	QString textFont;
	ds.setByteOrder(QDataStream::LittleEndian);
	QString cmdText = QString("Record %1 Type: ").arg(m_recordCount);
	switch (cmd)
	{
		case 1:
			cmdText += QString("DRW Background Color %1").arg(getColor(ds));
			break;
		case 2:
			cmdText += "DRW Facename";
			break;
		case 3:
			cmdText += QString("DRW Version Data %1").arg(QString(m_cmdData.toHex().left(64)));
			break;
		case 4:
			cmdText += QString("DRW ID Data %1").arg(QString(m_cmdData).left(20));
			if (m_listStack.count() > 0)
				m_listStack.top().itemGroupName = QString(m_cmdData);
			break;
		case 5:
			cmdText += QString("DRW Overlay Data %1").arg(QString(m_cmdData.toHex().left(20)));
			break;
		case 6:
			cmdText += "DRW Polygon";
			if ((m_createObjCode == 1) || (m_createObjCode == 3))
			{
				bool first = true;
				bool first2 = true;
				QPointF startP;
				QPainterPath path;
				for (int a = 0; a < m_nrOfPoints; a++)
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
				if (m_currentItem != NULL)
				{
					m_currentItem->PoLine.fromQPainterPath(path);
					QRectF bBoxO = path.boundingRect();
					if (bBoxO.x() < 0)
						m_currentItem->PoLine.translate(-bBoxO.x(), 0);
					if (bBoxO.y() < 0)
						m_currentItem->PoLine.translate(0, -bBoxO.y());
					finishItem(m_currentItem);
					if (m_currentItem != NULL)
					{
						handleLineStyle(m_currentItem, m_flags, m_lineColor);
						handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
					}
				}
				m_createObjCode = 0;
				m_currentItem = NULL;
			}
			else if (m_createObjCode == 2)
			{
				bool first = true;
				QPointF startP;
				QPainterPath path;
				int a = 0;
				while (a < m_nrOfPoints)
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
				if (m_currentItem != NULL)
				{
					m_currentItem->PoLine.fromQPainterPath(path);
					QRectF bBoxO = path.boundingRect();
					if (bBoxO.x() < 0)
						m_currentItem->PoLine.translate(-bBoxO.x(), 0);
					if (bBoxO.y() < 0)
						m_currentItem->PoLine.translate(0, -bBoxO.y());
					finishItem(m_currentItem);
					if (m_currentItem != NULL)
					{
						handleLineStyle(m_currentItem, m_flags, m_lineColor);
						handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
					}
				}
				m_createObjCode = 0;
				m_currentItem = NULL;
			}
			else if (m_createObjCode == 4)
			{
				bool first = true;
				QPointF startP;
				int a = 0;
				QPainterPath path;
				while (a < m_nrOfPoints-1)
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
				if (m_currentItem != NULL)
				{
					m_currentItem->PoLine.fromQPainterPath(path);
					QRectF bBoxO = path.boundingRect();
					if (bBoxO.x() < 0)
						m_currentItem->PoLine.translate(-bBoxO.x(), 0);
					if (bBoxO.y() < 0)
						m_currentItem->PoLine.translate(0, -bBoxO.y());
					finishItem(m_currentItem);
					if (m_currentItem != NULL)
					{
						handleLineStyle(m_currentItem, m_flags, m_lineColor);
						handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
					}
				}
				m_createObjCode = 0;
				m_currentItem = NULL;
			}
			break;
		case 7:
			cmdText = "";
			decodeSymbol(ds);
			printMSG = false;
			break;
		case 8:
			cmdText += "DRW Text";
			if (m_createObjCode == 5)
			{
				QString tx = QString(m_cmdData.left(m_nrOfChars));
				QStringList parList = tx.split(QChar(13));
				double yp = 0;
				QPainterPath path;
				QString fontN = "Arial";
				if (m_fontMap.contains(m_fontID))
					fontN = m_fontMap[m_fontID];
				QFont textFont = QFont(fontN, m_fontSize * 0.8);
				QFontMetrics fm(textFont);
				for (int a = 0; a < parList.size(); a++)
				{
					path.addText( 0, yp, textFont, parList[a].trimmed());
					yp += fm.lineSpacing();
				}
				QTransform txS;
				QRectF bbox = path.boundingRect();
				txS = QTransform();
				txS.scale(m_scaleFactor, m_scaleFactor);
				path = txS.map(path);
				txS = QTransform();
				bbox = path.boundingRect();
				txS.translate(-bbox.x(), -bbox.y());
				txS.translate(0, fm.leading() * m_scaleFactor);
				path = txS.map(path);
				if (m_currentItem != NULL)
				{
					m_currentItem->PoLine.fromQPainterPath(path);
					m_currentItem->setWidth(bbox.width());
					finishItem(m_currentItem, false);
				}
				m_createObjCode = 0;
				m_currentItem = NULL;
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
			cmdText += QString("DRW Curr Overlay Data %1").arg(QString(m_cmdData.toHex().left(20)));
			break;
		case 17:
			cmdText += QString("DRW Visible Data %1").arg(QString(m_cmdData.toHex().left(20)));
			break;
		case 18:
			cmdText += QString("DRW Comment Data %1").arg(QString(m_cmdData.toHex().left(20)));
			break;
		case 19:
			cmdText += QString("DRW Info Data %1").arg(QString(m_cmdData).left(20));
			break;
		case 20:
			cmdText += "DRW Bitmap";
			break;
		case 21:
			ds >> m_fontID;
			ds.device()->seek(0x13);
			m_fontName = "";
			ds >> chData;
			while (chData != 0)
			{
				m_fontName += QChar(chData);
				ds >> chData;
			}
			m_fontName = m_fontName.trimmed();
			m_fontName.replace( QRegExp( "'" ) , QChar( ' ' ) );
			{
				textFont = m_Doc->itemToolPrefs().textFont;
				bool found = false;
				SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
				for ( ; it.hasNext(); it.next())
				{
					QString fn = it.current().scName();
					if (fn == m_fontName)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					if (m_importerFlags & LoadSavePlugin::lfCreateThumbnail)
						m_fontName = PrefsManager::instance()->appPrefs.itemToolPrefs.textFont;
					else
					{
						if (!PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(m_fontName))
						{
							qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
							MissingFont *dia = new MissingFont(0, m_fontName, m_Doc);
							dia->exec();
							textFont = dia->getReplacementFont();
							delete dia;
							qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
							PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[m_fontName] = textFont;
							m_fontName = textFont;
						}
						else
							m_fontName = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[m_fontName];
					}
				}
			}
			m_fontMap.insert(m_fontID, m_fontName);
			cmdText += QString("DRW Font %1").arg(m_fontName);
			break;
		case 22:
			cmdText += "DRW Grid";
			break;
		case 23:
			cmdText += QString("DRW Overlay Name Data %1").arg(QString(m_cmdData).left(20));
			printMSG = true;
			break;
		case 24:
			cmdText += "DRW Dimensions";
			break;
		case 25:
			ds >> data16;
			m_scaleFactor = (1.0 / static_cast<double>(data16)) * 72.0;
			cmdText += QString("DRW Resolution %1").arg(data16);
			break;
		case 26:
			cmdText += "DRW Ruler";
			break;
		case 27:
			cmdText += "DRW Page";
			m_docWidth = getValue(ds);
			m_docHeight = getValue(ds);
			if (m_importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				m_Doc->setPage(m_docWidth, m_docHeight, 0, 0, 0, 0, 0, 0, false, false);
				if (m_docWidth > m_docHeight)
					m_Doc->setPageOrientation(1);
				else
					m_Doc->setPageOrientation(0);
				m_Doc->setPageSize("Custom");
				m_Doc->changePageProperties(0, 0, 0, 0, m_docHeight, m_docWidth, m_docHeight, m_docWidth, m_Doc->pageOrientation(), m_Doc->pageSize(), m_Doc->currentPage()->pageNr(), 0);
				cmdText = QString("DRW Page  Width %1  Height %2").arg(m_docWidth).arg(m_docHeight);
			}
			break;
		case 28:
			cmdText += "DRW Pattern";
			ds >> data8;
			pattern.resize(16);
			ds.readRawData(pattern.data(), 16);
			m_patternDataMap.insert(data8, pattern);
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
			m_gradientMap.insert(index, gradient);
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
			ds >> m_fontID;					// Font Nr
			cmdText += QString(" Font %1").arg(m_fontID);
			ds >> m_fontStyle;				// Style
			ds >> m_fontWidth;				// Width
			ds >> m_fontSize;					// Height
			cmdText += QString(" Size %1").arg(m_fontSize);
			ds >> m_nrOfParagraphs;			// Nr of paragraph records
			m_paragraphCounter = 0;
			cmdText += QString(" NoPara %1").arg(m_nrOfParagraphs);
			m_paragraphList.clear();
			for (quint16 a = 0; a < m_nrOfParagraphs; a++)
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
				m_paragraphList.append(para);
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
			if (m_imageValid)
			{
				quint16 xoff, yoff, len, count;
				ds >> xoff >> yoff >> len >> count;
				if (m_bitsPerPixel == 24)
				{
					for (quint16 y = 0; y < count; y++)
					{
						QRgb *q = (QRgb*)(m_tmpImage.scanLine(yoff + y));
						for (quint16 x = 0; x < m_imageWidth; x++)
						{
							quint8 r, g, b;
							ds >> r >> g >> b;
							*q = qRgba(r, g, b, 255);
							q++;
						}
						m_scanLinesRead++;
					}
				}
				else if (m_bitsPerPixel == 8)
				{
					for (quint16 y = 0; y < count; y++)
					{
						QRgb *q = (QRgb*)(m_tmpImage.scanLine(yoff + y));
						int pos = ds.device()->pos();
						for (quint16 x = 0; x < m_imageWidth; x++)
						{
							quint8 r;
							ds >> r;
							*q = qRgba(r, r, r, 255);
							q++;
						}
						QByteArray data;
						data.resize(m_imageWidth);
						ds.device()->seek(pos);
						ds.readRawData(data.data(), m_imageWidth);
						ds.device()->seek(pos + len);
						memcpy(m_tmpImage2.scanLine(yoff + y), data.data(), m_imageWidth);
						m_scanLinesRead++;
					}
				}
				if (m_scanLinesRead >= m_imageHeight)
				{
					if (m_currentItem != NULL)
					{
						QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_drw_XXXXXX.png");
						tempFile->setAutoRemove(false);
						tempFile->open();
						QString fileName = getLongPathName(tempFile->fileName());
						tempFile->close();
						m_currentItem->isInlineImage = true;
						m_currentItem->isTempFile = true;
						m_tmpImage.save(fileName, "PNG");
						m_Doc->loadPict(fileName, m_currentItem);
						delete tempFile;
						m_currentItem->setImageScalingMode(false, false);
					}
					m_imageValid = false;
					m_tmpImage = QImage();
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
			if (m_createObjCode == 6)
			{
				if (m_currentItem != NULL)
				{
					DRWParagraph para = m_paragraphList.at(m_paragraphCounter);
					m_paragraphCounter++;
					ParagraphStyle newStyle;
					newStyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(1));
					newStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(para.paragraphAlignment));
					newStyle.charStyle().setFontSize(m_fontSize * m_scaleFactor * 10.0 * 0.8);
					QString fontN(m_Doc->itemToolPrefs().textFont);
					if (m_fontMap.contains(m_fontID))
						fontN = m_fontMap[m_fontID];
					newStyle.charStyle().setFont((*m_Doc->AllFonts)[fontN]);
					newStyle.charStyle().setFillColor(m_fontColor);
					newStyle.setLineSpacing(newStyle.charStyle().font().height(m_fontSize * m_scaleFactor * 10.0 * 0.8));
					if (para.paragraphLen > 0)
					{
						int pos = m_currentItem->itemText.length();
						QByteArray data;
						data.resize(para.paragraphLen);
						ds.readRawData(data.data(), para.paragraphLen);
						QString chars = QString(data);
						if (!chars.isEmpty())
						{
							m_currentItem->itemText.insertChars( -1, chars);
							m_currentItem->itemText.applyStyle(pos, newStyle);
						}
						if (m_nrOfParagraphs > 0)
							m_currentItem->itemText.insertChars(-1, SpecialChars::PARSEP);
					}
				}
			}
			break;
		case 35:
			cmdText += "DRW Colortable";
			if (m_currentItem != NULL)
			{
				if (m_currentItem->asImageFrame())
				{
					QString fileName = m_currentItem->Pfile;
					if (!fileName.isEmpty())
					{
						QVector<QRgb> colors;
						for (quint16 cc = 0; cc < 255; cc++)	// now reading ColorTable, exactly 1024 bytes
						{
							quint8 r, g, b, a;
							ds >> r >> g >> b >> a;				// values are stored in RGB order
							if ((r == m_rTrans) && (g == m_gTrans) && (b == m_bTrans))
								colors.append(qRgba(r, g, b, 0));
							else
								colors.append(qRgb(r, g, b));
						}
						m_tmpImage2.setColorTable(colors);
						m_tmpImage2 = m_tmpImage2.convertToFormat(QImage::Format_ARGB32);
						m_tmpImage2.save(fileName, "PNG");
						m_Doc->loadPict(fileName, m_currentItem, true);
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
			cmdText += QString("Unknown Cmd-Nr %1  Data %2 Size %3").arg(cmd).arg(QString(m_cmdData.toHex().left(64))).arg(m_cmdData.size());
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
	m_symbolCount++;
	QString cmdText = QString("Record %1 Symbol %2 Type:").arg(m_recordCount).arg(m_symbolCount);
	bool printMSG = false;
	double bX = 0.0;
	double bY = 0.0;
	double groupX = 0.0;
	double groupY = 0.0;
	DRWObjectList gList;
	DRWGroup gElements;
	DRWGroup cElements;
	if (m_groupStack.count() > 0)
	{
		cElements = m_groupStack.top();
		bX = cElements.xoffset;
		bY = cElements.yoffset;
		if (cElements.nrOfItems != -1)
		{
			while (m_groupStack.count() > 1)
			{
				if (cElements.nrOfItems == cElements.counter)
				{
					m_listStack.pop();
					DRWGroup popped = m_groupStack.pop();
					cElements = m_groupStack.top();
					m_tmpSel->clear();
					for (int dre = 0;  dre < popped.GElements.count(); ++dre)
					{
						m_tmpSel->addItem(popped.GElements.at(dre), true);
					}
					bX = cElements.xoffset;
					bY = cElements.yoffset;
					uint selectedItemCount = m_tmpSel->count();
					if (selectedItemCount > 0)
					{
						double scx = 1.0;
						double scy = 1.0;
						QPainterPath gesPa;
						bool firstP = true;
						for (uint i = 0; i < selectedItemCount; ++i)
						{
							QPainterPath pa;
							PageItem *item = m_tmpSel->itemAt(i);
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
							PageItem *ite = m_tmpSel->takeItem(0);
							ite->setXYPos(popped.xoffset + m_baseX, popped.yoffset + m_baseY);
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
							m_groupStack.top().GElements.append(ite);
							m_listStack.top().GElements.append(ite);
						}
						selectedItemCount = m_tmpSel->count();
						for (uint i = 0; i < selectedItemCount; ++i)
						{
							Elements.removeAll(m_tmpSel->itemAt(i));
							m_listStack.top().GElements.removeAll(m_tmpSel->itemAt(i));
						}
						m_Doc->itemSelection_DeleteItem(m_tmpSel);
					}
					m_tmpSel->clear();
				}
				else
					break;
			}
			m_groupStack.top().counter++;
		}
	}
	if (m_listStack.count() > 1)
	{
		while (m_listStack.count() > 1)
		{
			if (m_listStack.top().nrOfItems == m_listStack.top().counter)
			{
				DRWObjectList popped = m_listStack.pop();
				m_tmpSel->clear();
				for (int dre = 0;  dre < popped.GElements.count(); ++dre)
				{
					m_tmpSel->addItem(popped.GElements.at(dre), true);
				}
				uint selectedItemCount = m_tmpSel->count();
				if (selectedItemCount > 0)
				{
					if (popped.rotationAngle != 0)
					{
						PageItem* currItem;
						QTransform ma;
						ma.translate(popped.posPivot.x(), popped.posPivot.y());
						ma.rotate(-popped.rotationAngle / 10.0);
						FPoint n;
						for (int a = 0; a < m_tmpSel->count(); ++a)
						{
							currItem = m_tmpSel->itemAt(a);
							n = FPoint(currItem->xPos(), currItem->yPos());
							currItem->setXYPos(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
							currItem->rotateBy(-popped.rotationAngle / 10.0);
						}
					}
					m_tmpSel->setGroupRect();
					QRectF gr = m_tmpSel->getGroupRect();
					double dx = popped.groupItem->xPos() - gr.x();
					double dy = popped.groupItem->yPos() - gr.y();
					for (int a = 0; a < m_tmpSel->count(); ++a)
					{
						m_tmpSel->itemAt(a)->moveBy(dx, dy);
					}
					m_tmpSel->setGroupRect();
					if ((popped.scaleX != 0) || (popped.scaleY != 0))
					{
						if ((m_tmpSel->width() != 0) && (m_tmpSel->height() != 0) && (popped.width != 0) && (popped.height != 0))
						{
							double scx = 1.0;
							if (m_tmpSel->width() != popped.width)
								scx = popped.width / m_tmpSel->width();
							double scy = 1.0;
							if (m_tmpSel->height() != popped.height)
								scy = popped.height / m_tmpSel->height();
							m_Doc->scaleGroup(scx, scy, true, m_tmpSel);
						}
					}
					m_listStack.top().GElements.append(popped.groupItem);
					for (uint i = 0; i < selectedItemCount; ++i)
					{
						PageItem *item = m_tmpSel->itemAt(i);
						popped.groupItem->groupItemList.append(item);
						item->gXpos = item->xPos() - popped.groupItem->xPos();
						item->gYpos = item->yPos() - popped.groupItem->yPos();
						item->Parent = popped.groupItem;
						if (m_groupStack.count() > 0)
							m_groupStack.top().GElements.removeAll(m_tmpSel->itemAt(i));
						Elements.removeAll(m_tmpSel->itemAt(i));
						m_Doc->Items->removeAll(m_tmpSel->itemAt(i));
					}
					if (popped.itemGroupName.isEmpty())
						popped.groupItem->setItemName( tr("Group%1").arg(m_Doc->GroupCounter));
					else
						popped.groupItem->setItemName(popped.itemGroupName);
					popped.groupItem->AutoName = false;
					popped.groupItem->groupWidth = m_tmpSel->width();
					popped.groupItem->groupHeight = m_tmpSel->height();
				}
				m_Doc->GroupCounter++;
				m_tmpSel->clear();
			}
			else
				break;
		}
		m_listStack.top().counter++;
		groupX = m_listStack.top().groupX;
		groupY = m_listStack.top().groupY;
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
	m_createObjCode = 0;
	m_currentItem = NULL;
	ds >> data8;							// reading Symbol Type
// now reading common values
	ds >> m_flags;
	QPainterPath path;
	QPointF posEnd, posMid, posStart;
	QLineF sLin, eLin;
	getCoordinate(ds);						// don't remove dummy parameters
	double boundingBoxX = getValue(ds);
	double boundingBoxY = getValue(ds);
	double boundingBoxW = getValue(ds);
	double boundingBoxH = getValue(ds);
	m_bBox = QRectF(QPointF(boundingBoxX, boundingBoxY), QPointF(boundingBoxW, boundingBoxH)).normalized();
	m_rotationAngle = getRawValue(ds);
	m_scaleX = getRawValue(ds);
	m_scaleY = getRawValue(ds);
	double rotS, rotE;
	double posX = m_baseX + m_bBox.x() + bX + groupX;
	double posY = m_baseY + m_bBox.y() + bY + groupY;
	m_lineColor = getColor(ds);
	ds >> dummy;		// handle
	ds >> dummy;		// next
	ds >> dummy;
	ds >> dummy;		// prev
	ds >> dummy;
	switch (data8)			// Symbol type is in the first data byte
	{
		case 0:
			cmdText += "elliptical Arc";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
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
			m_scaleX = 1;
			m_scaleY = 1;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			m_currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.controlPointRect();
			m_currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			finishItem(m_currentItem);
			break;
		case 1:
			cmdText += "filled Polygon";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			m_nrOfPoints = nPoints;
			m_createObjCode = 1;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
			break;
		case 2:
			getCommonData(ds);
			ds.device()->seek(0x26);
			ds >> dummy;
			if (dummy > 0)
			{
				z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, posX, posY, m_bBox.width(), m_bBox.height(), 0, fillC, fillC, true);
				gList.groupItem = m_Doc->Items->at(z);
				gList.groupX = groupX + m_bBox.x();
				gList.groupY = groupY + m_bBox.y();
				gList.width = m_bBox.width();
				gList.height = m_bBox.height();
				gList.scaleX = m_scaleX;
				gList.scaleY = m_scaleY;
				gList.rotationAngle = m_rotationAngle;
				gList.nrOfItems = dummy;
				gList.counter = 0;
				gList.posPivot = m_posPivot;
				gList.itemGroupName = "";
				gList.GElements.clear();
				m_listStack.push(gList);
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
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			cornerRadius = getValue(ds);
			ds >> appFlags;
			getCommonData(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			finishItem(m_currentItem);
			if (m_currentItem != NULL)
			{
				handleLineStyle(m_currentItem, m_flags, m_lineColor);
				handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
			}
			break;
		case 5:
			cmdText += "Text";
			ds >> dummy;
			ds >> m_fontID;
			ds >> m_nrOfChars;
			ds >> m_fontSize;
			m_fontColor = m_lineColor;
			m_createObjCode = 5;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), 0, m_lineColor, CommonStrings::None, true);
			m_currentItem = m_Doc->Items->at(z);
			m_scaleX = 0;
			break;
		case 6:
			cmdText += "Line";
			posEnd = getCoordinate(ds);
			posStart = getCoordinate(ds);
			if (posStart == posEnd)
				break;
			ds >> m_patternIndex;
			getCommonData(ds);
			m_lineWidth = m_patternIndex * m_scaleFactor;
			path = QPainterPath();
			path.moveTo(posStart);
			path.lineTo(posEnd);
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			m_currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.boundingRect();
			m_currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			finishItem(m_currentItem);
			break;
		case 8:
			cmdText += "Polyline";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			m_nrOfPoints = nPoints;
			m_createObjCode = 3;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
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
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			cornerRadius = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			ds >> appFlags;
			getCommonData(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			finishItem(m_currentItem);
			if (m_currentItem != NULL)
			{
				if (data8 == 11)
					m_currentItem->setCornerRadius(cornerRadius);
				handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
			}
			break;
		case 13:
			cmdText += "filled Ellipse";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			cornerRadius = getValue(ds);
			ds >> appFlags;
			getCommonData(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			finishItem(m_currentItem);
			if (m_currentItem != NULL)
				handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
			break;
		case 14:
			cmdText += "elliptical Arc, clockwise";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
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
			m_scaleX = 1;
			m_scaleY = 1;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			m_currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.controlPointRect();
			m_currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			finishItem(m_currentItem);
			break;
		case 15:
			cmdText += "filled parabolic Arc";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			posStart = getCoordinate(ds);
			posMid = getCoordinate(ds);
			posEnd = getCoordinate(ds);
			if (posStart == posEnd)
				break;
			getCommonData(ds);
			path = QPainterPath();
			path.moveTo(posStart);
			path.cubicTo(posMid, posMid, posEnd);
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			m_currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.controlPointRect();
			m_currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			finishItem(m_currentItem);
			if (m_currentItem != NULL)
			{
				handleLineStyle(m_currentItem, m_flags, m_lineColor);
				handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
			}
			break;
		case 16:
			cmdText += "filled quadratic Spline";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			m_nrOfPoints = nPoints;
			m_createObjCode = 4;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
			break;
		case 17:
		case 20:
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			ds.device()->seek(0x2B);
			ds >> nItems;
			ds >> dummy;
			ds >> appFlags;
			getCommonData(ds);
			gElements.xoffset = bX + m_bBox.x() + groupX;
			gElements.yoffset = bY + m_bBox.y() + groupY;
			gElements.width = m_bBox.width();
			gElements.height = m_bBox.height();
			gElements.lineWidth = m_lineWidth;
			gElements.scaleX = m_scaleX;
			gElements.scaleY = m_scaleY;
			gElements.rotationAngle = m_rotationAngle;
			gElements.posPivot = m_posPivot;
			if (data8 == 17)
				gElements.filled = true;
			else
				gElements.filled = false;
			gElements.nrOfItems = nItems;
			gElements.counter = 0;
			gElements.patternIndex = m_patternIndex;
			gElements.flags = m_flags;
			gElements.fillColor = m_fillColor;
			gElements.lineColor = m_lineColor;
			gElements.backColor = m_backColor;
			m_groupStack.push(gElements);
			gList.groupX = groupX;
			gList.groupY = groupY;
			gList.width = m_bBox.width();
			gList.height = m_bBox.height();
			gList.nrOfItems = 0xFFFF;
			gList.counter = 0;
			m_listStack.push(gList);
			cmdText += QString("filled complex Object Count %1  Scale %2 %3 LW %4").arg(nItems).arg(m_scaleX).arg(m_scaleY).arg(m_lineWidth);
			break;
		case 18:
			cmdText += "parabolic Arc";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			posStart = getCoordinate(ds);
			posMid = getCoordinate(ds);
			posEnd = getCoordinate(ds);
			if (posStart == posEnd)
				break;
			getCommonData(ds);
			path = QPainterPath();
			path.moveTo(posStart);
			path.cubicTo(posMid, posMid, posEnd);
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			m_currentItem->PoLine.fromQPainterPath(path);
			bBoxO = path.controlPointRect();
			m_currentItem->PoLine.translate(-bBoxO.x(), -bBoxO.y());
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			finishItem(m_currentItem);
			break;
		case 19:
			cmdText += "quadratic Spline";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			m_nrOfPoints = nPoints;
			m_createObjCode = 4;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			break;
		case 22:
			cmdText += "Bitmap monochrome";
			ds >> dummy;
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			ds >> m_bitsPerPixel;
			ds >> m_bytesScanline;
			ds >> m_planes;
			ds >> m_imageHeight;
			ds >> m_imageWidth;
			ds >> dummy;
			ds >> m_rTrans;
			ds >> m_gTrans;
			ds >> m_bTrans;
			getCommonData(ds);
			if ((m_bitsPerPixel == 24) || (m_bitsPerPixel == 8))
			{
				z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, CommonStrings::None, CommonStrings::None, true);
				m_currentItem = m_Doc->Items->at(z);
				finishItem(m_currentItem);
				m_scanLinesRead = 0;
				m_tmpImage = QImage(m_imageWidth, m_imageHeight, QImage::Format_ARGB32);
				if (m_bitsPerPixel == 8)
					m_tmpImage2 = QImage(m_imageWidth, m_imageHeight, QImage::Format_Indexed8);
				m_imageValid = true;
			}
			break;
		case 23:
			cmdText += "Bezier line";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			m_nrOfPoints = nPoints;
			m_createObjCode = 2;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			break;
		case 24:
			cmdText += "filled Bezier line";
			ds >> m_patternIndex;
			m_fillColor = getColor(ds);
			if (m_patternIndex != 0)
				fillC = m_fillColor;
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			getCommonData(ds);
			m_nrOfPoints = nPoints;
			m_createObjCode = 2;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, posX, posY, m_bBox.width(), m_bBox.height(), m_lineWidth, fillC, m_lineColor, true);
			m_currentItem = m_Doc->Items->at(z);
			handleLineStyle(m_currentItem, m_flags, m_lineColor);
			handleGradient(m_currentItem, m_patternIndex, m_fillColor, m_backColor, m_bBox);
			break;
		case 25:
			cmdText += "Rich Text";
			m_fontColor = m_lineColor;
			getCommonData(ds);
			m_createObjCode = 6;
			z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, posX, posY, m_bBox.width(), m_bBox.height(), 0, CommonStrings::None, CommonStrings::None, true);
			m_currentItem = m_Doc->Items->at(z);
			m_currentItem->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
			finishItem(m_currentItem);
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
		if (m_currentItem != NULL)
			qDebug() << cmdText << " " << m_currentItem->itemName();
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
		if (m_gradientMap.contains(ind))
		{
			grad = m_gradientMap[ind];
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
		if (m_patternDataMap.contains(ind))
		{
			QColor back = ScColorEngine::getRGBColor(m_Doc->PageColors[fillColor], m_Doc);
			QColor fore = ScColorEngine::getRGBColor(m_Doc->PageColors[backColor], m_Doc);
			QString patNa = QString("%1%2%3").arg(back.name()).arg(fore.name()).arg(ind);
			QString patternName;
			if (!m_patternMap.contains(patNa))
			{
				uint oldNum = m_Doc->TotalItems;
				QByteArray data = m_patternDataMap[ind];
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
				QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_drw_XXXXXX.png");
				tempFile->setAutoRemove(false);
				tempFile->open();
				QString fileName = getLongPathName(tempFile->fileName());
				tempFile->close();
				delete tempFile;
				newItem->isInlineImage = true;
				newItem->isTempFile = true;
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
				}
				patternName = "Pattern_"+newItem->itemName();
				patternName = patternName.trimmed().simplified().replace(" ", "_");
				m_Doc->addPattern(patternName, pat);
				m_patternMap.insert(patNa, patternName);
				m_Doc->TotalItems = oldNum;
			}
			else
				patternName = m_patternMap[patNa];
			m_importedPatterns.append(patternName);
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
	size = m_cmdData.size() + 14;
	hs << size;
	header.append(m_cmdData);
	m_thumbnailImage.loadFromData(header, "BMP");
}

QString DrwPlug::handleColor(ScColor &color, QString proposedName)
{
	QString tmpName = m_Doc->PageColors.tryAddColor(proposedName, color);
	if (tmpName == proposedName)
		m_importedColors.append(tmpName);
	return tmpName;
}


void DrwPlug::getCommonData(QDataStream &ds)
{
	quint16 dummy;
	ds.device()->seek(0x38);
	m_backColor = getColor(ds);
	m_lineWidth = getValue(ds);
	ds >> dummy;
	m_posPivot = getCoordinate(ds);
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
		m_tmpSel->clear();
		m_tmpSel->addItem(ite, true);
		m_Doc->itemSelection_DeleteItem(m_tmpSel);
		m_currentItem = NULL;
		m_createObjCode = 0;
		m_tmpSel->clear();
		return;
	}
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	if (m_rotationAngle != 0)
	{
		QTransform ma;
		ma.translate(m_posPivot.x(), m_posPivot.y());
		ma.rotate(-m_rotationAngle / 10.0);
		ite->PoLine.map(ma);
		FPoint tp2(getMinClipF(&ite->PoLine));
		ite->PoLine.translate(-tp2.x(), -tp2.y());
	}
	if (scale)
	{
		if ((m_scaleX != 0) || (m_scaleY != 0))
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
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(),wh.y());
	m_Doc->AdjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	Elements.append(ite);
	if (m_groupStack.count() > 1)
		m_groupStack.top().GElements.append(ite);
	if (m_listStack.count() > 1)
		m_listStack.top().GElements.append(ite);
	m_Coords.resize(0);
	m_Coords.svgInit();
}

double DrwPlug::getValue(QDataStream &ds)
{
	qint16 data16;
	ds >> data16;
	return data16 * m_scaleFactor;
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
	return QPointF(x * m_scaleFactor, y * m_scaleFactor);
}
