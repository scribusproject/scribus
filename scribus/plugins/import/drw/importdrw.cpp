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
	interactive = (flags & LoadSavePlugin::lfInteractive);
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
			m_Doc->PageOri = 1;
		else
			m_Doc->PageOri = 0;
		m_Doc->m_pageSize = "Custom";
	}
	Elements.clear();
	FPoint minSize = m_Doc->minCanvasCoordinate;
	FPoint maxSize = m_Doc->maxCanvasCoordinate;
	FPoint cOrigin = m_Doc->view()->canvasOrigin();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->ScriptRunning = true;
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if ((Elements.count() > 1) && (!(importerFlags & LoadSavePlugin::lfCreateDoc)))
		{
			bool isGroup = true;
			int firstElem = -1;
			if (Elements.at(0)->Groups.count() != 0)
				firstElem = Elements.at(0)->Groups.top();
			for (int bx = 0; bx < Elements.count(); ++bx)
			{
				PageItem* bxi = Elements.at(bx);
				if (bxi->Groups.count() != 0)
				{
					if (bxi->Groups.top() != firstElem)
						isGroup = false;
				}
				else
					isGroup = false;
			}
			if (!isGroup)
			{
				double minx = 99999.9;
				double miny = 99999.9;
				double maxx = -99999.9;
				double maxy = -99999.9;
				uint lowestItem = 999999;
				uint highestItem = 0;
				for (int a = 0; a < Elements.count(); ++a)
				{
					Elements.at(a)->Groups.push(m_Doc->GroupCounter);
					PageItem* currItem = Elements.at(a);
					lowestItem = qMin(lowestItem, currItem->ItemNr);
					highestItem = qMax(highestItem, currItem->ItemNr);
					double x1, x2, y1, y2;
					currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
					minx = qMin(minx, x1);
					miny = qMin(miny, y1);
					maxx = qMax(maxx, x2);
					maxy = qMax(maxy, y2);
				}
				double gx = minx;
				double gy = miny;
				double gw = maxx - minx;
				double gh = maxy - miny;
				PageItem *high = m_Doc->Items->at(highestItem);
				int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_Doc->itemToolPrefs.shapeFillColor, m_Doc->itemToolPrefs.shapeLineColor, true);
				PageItem *neu = m_Doc->Items->takeAt(z);
				m_Doc->Items->insert(lowestItem, neu);
				neu->Groups.push(m_Doc->GroupCounter);
				neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
				neu->AutoName = false;
				neu->isGroupControl = true;
				neu->groupsLastItem = high;
				neu->setTextFlowMode(PageItem::TextFlowDisabled);
				for (int a = 0; a < m_Doc->Items->count(); ++a)
				{
					m_Doc->Items->at(a)->ItemNr = a;
				}
				Elements.prepend(neu);
				m_Doc->GroupCounter++;
			}
		}
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->ScriptRunning = false;
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
				m_Doc->m_Selection->delaySignalsOn();
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					m_Doc->m_Selection->addItem(Elements.at(dre), true);
				}
				m_Doc->m_Selection->delaySignalsOff();
				m_Doc->m_Selection->setGroupRect();
				m_Doc->view()->updatesOn(true);
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
				md->setScribusElem(ss->WriteElem(m_Doc, m_Doc->view(), tmpSel));
				delete ss;
				m_Doc->itemSelection_DeleteItem(tmpSel);
				m_Doc->view()->updatesOn(true);
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
			m_Doc->view()->updatesOn(true);
		}
		success = true;
	}
	else
	{
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->ScriptRunning = false;
		m_Doc->view()->updatesOn(true);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	if (interactive)
		m_Doc->setLoading(false);
	//CB If we have a gui we must refresh it if we have used the progressbar
	if ((showProgress) && (!interactive))
		m_Doc->view()->DrawNew();
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
			progressDialog->setProgress("GI", ts.device()->pos());
			if (cmd == 254)
				break;
			qApp->processEvents();
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
/*	if ((recordCount > 687) && (recordCount < 700))
	{
		QFile f(QString("/home/franz/cmddatas%1.bin").arg(recordCount));
		f.open(QIODevice::WriteOnly);
		f.write(cmdData);
		f.close();
	} */
	QDataStream ds(cmdData);
	DRWGradient gradient;
	quint8 data8, chData;
	quint16 data16;
	int index;
	QFont font;
	QString textFont;
	ds.setByteOrder(QDataStream::LittleEndian);
	QString cmdText = QString("Record %1 Type: ").arg(recordCount);
	bool printMSG = false;
	switch (cmd)
	{
		case 1:
			cmdText += QString("DRW Background Color %1").arg(getColor(ds));
			printMSG = false;
			break;
		case 2:
			cmdText += "DRW Facename";
			printMSG = true;
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
			cmdText += "DRW Overlay";
			printMSG = true;
			break;
		case 6:
			cmdText += "DRW Polygon";
			if ((createObjCode == 1) || (createObjCode == 3))
			{
				bool first = true;
				bool first2 = true;
				Coords.resize(0);
				Coords.svgInit();
				QPointF startP;
				for (int a = 0; a < nrOfPoints; a++)
				{
					QPointF coor = getCoordinate(ds);
					if (first)
					{
						Coords.svgMoveTo(coor.x(), coor.y());
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
							if (createObjCode == 1)
								Coords.svgClosePath();
						}
						else
							Coords.svgLineTo(coor.x(), coor.y());
					}
				}
				if (currentItem != NULL)
				{
					currentItem->PoLine = Coords.copy();
					finishItem(currentItem);
				}
				createObjCode = 0;
				currentItem = NULL;
			}
			else if (createObjCode == 2)
			{
				bool first = true;
				Coords.resize(0);
				Coords.svgInit();
				QPointF startP;
				int a = 0;
				while (a < nrOfPoints)
				{
					if (first)
					{
						QPointF coor = getCoordinate(ds);
						a++;
						Coords.svgMoveTo(coor.x(), coor.y());
						startP = coor;
						first = false;
					}
					QPointF p1 = getCoordinate(ds);
					QPointF p2 = getCoordinate(ds);
					QPointF p3 = getCoordinate(ds);
					a += 3;
					Coords.svgCurveToCubic(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
				}
				if (currentItem != NULL)
				{
					currentItem->PoLine = Coords.copy();
					finishItem(currentItem, false);
				}
				createObjCode = 0;
				currentItem = NULL;
			}
			else if (createObjCode == 4)
			{
				bool first = true;
				Coords.resize(0);
				Coords.svgInit();
				QPointF startP;
				int a = 0;
				QPainterPath pa;
				while (a < nrOfPoints)
				{
					if (first)
					{
						QPointF coor = getCoordinate(ds);
						a++;
						pa.moveTo(coor.x(), coor.y());
						startP = coor;
						first = false;
					}
					QPointF p1 = getCoordinate(ds);
					QPointF p2 = getCoordinate(ds);
					a += 2;
					pa.quadTo(p1.x(), p1.y(), p2.x(), p2.y());
				}
				if (currentItem != NULL)
				{
					currentItem->PoLine.fromQPainterPath(pa);
					finishItem(currentItem);
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
				QPainterPath path;
				QString fontN = "Arial";
				if (fontMap.contains(fontID))
					fontN = fontMap[fontID];
				QFont textFont = QFont(fontN, fontSize);
				path.addText( 0, 0, textFont, tx);
				QFontMetrics fm(textFont);
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
//			handlePreviewBitmap(ds);
			break;
		case 14:
			cmdText += "DRW View";
			break;
		case 15:
			cmdText += "DRW Old Grid";
			break;
		case 16:
			cmdText += "DRW Curr Overlay";
			printMSG = true;
			break;
		case 17:
			cmdText += "DRW Visible";
			printMSG = true;
			break;
		case 18:
			cmdText += "DRW Comment";
			break;
		case 19:
			cmdText += "DRW Info";
			break;
		case 20:
			cmdText += "DRW Bitmap";
			printMSG = true;
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
			font = QFont(fontName, 12);
			textFont = QFontInfo(font).family();
			if (textFont != fontName)
			{
				textFont = m_Doc->itemToolPrefs.textFont;
				bool found = false;
				SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
				for ( ; it.hasNext(); it.next())
				{
					QString fam;
					QString fn = it.current().scName();
					int pos = fn.indexOf(" ");
					fam = fn.left(pos);
					if (fam == fontName)
					{
						found = true;
						break;
					}
				}
				if (!found)
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
			fontMap.insert(fontID, fontName);
			cmdText += QString("DRW Font %1").arg(fontName);
			break;
		case 22:
			cmdText += "DRW Grid";
			break;
		case 23:
			cmdText += "DRW Overlay Name";
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
					m_Doc->PageOri = 1;
				else
					m_Doc->PageOri = 0;
				m_Doc->m_pageSize = "Custom";
				m_Doc->changePageMargins(0, 0, 0, 0, docHeight, docWidth, docHeight, docWidth, m_Doc->PageOri, m_Doc->m_pageSize, m_Doc->currentPage()->pageNr(), 0);
				cmdText = QString("DRW Page  Width %1  Height %2").arg(docWidth).arg(docHeight);
			}
			break;
		case 28:
			cmdText += "DRW Pattern";
			printMSG = true;
			break;
		case 29:
			cmdText += "DRW Locked";
			printMSG = true;
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
			cmdText += QString("DRW Gradient  Index: %1 Type: %2 Offsets: %3 %4").arg(index).arg(gradient.type).arg(gradient.xOffset).arg(gradient.yOffset);
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
					//	tmpImage = tmpImage.convertToFormat(QImage::Format_ARGB32);
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
					QString fontN = m_Doc->itemToolPrefs.textFont;
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
			printMSG = true;
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
			break;
		case 255:
			cmdText += QString("DRW Start File Data %1").arg(QString(cmdData.toHex().left(64)));
			break;
		default:
			cmdText += QString("Unknown Cmd-Nr %1  Data %2").arg(cmd).arg(QString(cmdData.toHex().left(64)));
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
						if ((tmpSel->width() != 0) && (tmpSel->height() != 0) && (popped.width != 0) && (popped.height != 0))
						{
							if (tmpSel->width() != popped.width)
								scx = popped.width / tmpSel->width();
							if (tmpSel->height() != popped.height)
								scy = popped.height / tmpSel->height();
							m_Doc->scaleGroup(scx, scy, true, tmpSel);
						}
						QRectF selRect = tmpSel->getGroupRect();
						QPainterPath gesPa;
						bool firstP = true;
						for (uint i = 0; i < selectedItemCount; ++i)
						{
							QPainterPath pa;
							PageItem *item = tmpSel->itemAt(i);
							item->PoLine.translate(item->xPos(), item->yPos());
							if (item->asPolyLine())
								pa = item->PoLine.toQPainterPath(false);
							else
								pa = item->PoLine.toQPainterPath(true);
							if (!pa.isEmpty())
							{
								const QPainterPath::Element &elm = pa.elementAt(0);
								QPointF lastP = gesPa.currentPosition();
								bool conn = false;
								if ((fabs(lastP.x() - elm.x) > 3) || (fabs(lastP.y() - elm.y) > 3))
									conn = true;
								if ((firstP) || (conn))
								{
									gesPa.addPath(pa);
									firstP = false;
								}
								else
									gesPa.connectPath(pa);
							}
						}
						FPointArray res;
						res.fromQPainterPath(gesPa);
						res.translate(-selRect.x(), -selRect.y());
						PageItem *ite = tmpSel->takeItem(0);
						selectedItemCount = tmpSel->count();
						for (uint i = 0; i < selectedItemCount; ++i)
						{
							Elements.removeAll(tmpSel->itemAt(i));
							listStack.top().GElements.removeAll(tmpSel->itemAt(i));
						}
						ite->setXYPos(popped.xoffset + m_Doc->currentPage()->xOffset(), popped.yoffset + m_Doc->currentPage()->yOffset());
						ite->PoLine = res.copy();
						FPoint wh = getMaxClipF(&ite->PoLine);
						ite->setWidthHeight(wh.x(),wh.y());
						ite->setFillColor(popped.fillColor);
						ite->setLineWidth(popped.lineWidth);
						ite->setLineWidth(ite->lineWidth() / qMin(scx, scy));
						handleLineStyle(ite, popped.flags, popped.lineColor);
						handleGradient(ite, popped.patternIndex, popped.fillColor, popped.backColor, QRectF(0, 0, ite->width(), ite->height()));
						groupStack.top().GElements.append(ite);
						listStack.top().GElements.append(ite);
						m_Doc->itemSelection_DeleteItem(tmpSel);
					}
					tmpSel->clear();
				//	cmdText += QString("dropping complex Object Offsets %1 %2\n").arg(cElements.xoffset).arg(cElements.yoffset);
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
					listStack.top().GElements.append(popped.groupItem);
					for (uint i = 0; i < selectedItemCount; ++i)
					{
						PageItem *item = tmpSel->itemAt(i);
						item->Groups.push(m_Doc->GroupCounter);
						listStack.top().GElements.append(item);
					}
					popped.groupItem->Groups.push(m_Doc->GroupCounter);
					if (popped.itemGroupName.isEmpty())
						popped.groupItem->setItemName( tr("Group%1").arg(popped.groupItem->Groups.top()));
					else
						popped.groupItem->setItemName(popped.itemGroupName);
					popped.groupItem->AutoName = false;
					popped.groupItem->isGroupControl = true;
					popped.groupItem->groupsLastItem = tmpSel->itemAt(selectedItemCount - 1);
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
/*	if ((symbolCount > 514) && (symbolCount < 517))
	{
		QFile f(QString("/home/franz/cmddatas%1.bin").arg(symbolCount));
		f.open(QIODevice::WriteOnly);
		f.write(cmdData);
		f.close();
	}*/
	int z;
	quint8 data8, flags, patternIndex, appFlags;
	quint16 dummy, nPoints, nItems;
	double boundingBoxXO, boundingBoxYO, boundingBoxWO, boundingBoxHO, cornerRadius;
	QRectF bBoxO;
	QString backColor;
	QString fillC = CommonStrings::None;
	createObjCode = 0;
	currentItem = NULL;
	ds >> data8;							// reading Symbol Type
// now reading common values
	ds >> flags;
	QPainterPath path;
	QPointF posEnd, posStart;
	QTransform mat;
	QLineF sLin, eLin;
	QPointF position = getCoordinate(ds);
	double boundingBoxX = getValue(ds);
	double boundingBoxY = getValue(ds);
	double boundingBoxW = getValue(ds);
	double boundingBoxH = getValue(ds);
	QRectF bBox = QRectF(QPointF(boundingBoxX, boundingBoxY), QPointF(boundingBoxW, boundingBoxH)).normalized();
	double rotationAngle = getRawValue(ds);
	double scaleX = getRawValue(ds);
	double scaleY = getRawValue(ds);
	double rotS, rotE;
	double posX = baseX + bBox.x() + bX + groupX;
	double posY = baseY + bBox.y() + bY + groupY;
//	double posX = baseX + position.x() + bX + groupX;
//	double posY = baseY + position.y() + bY + groupY;
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
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			posStart = getCoordinate(ds);
			posEnd = getCoordinate(ds);
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			sLin = QLineF(QPointF(boundingBoxWO / 2.0, boundingBoxHO / 2.0), posStart);
			eLin = QLineF(QPointF(boundingBoxWO / 2.0, boundingBoxHO / 2.0), posEnd);
			rotS = sLin.angle();
			rotE = eLin.angle();
			rotE = rotE - rotS;
			path = QPainterPath();
			path.arcMoveTo(QRectF(0, 0, boundingBoxWO, boundingBoxHO), rotS);
			path.arcTo(QRectF(0, 0, boundingBoxWO, boundingBoxHO), rotS, rotE);
			path = mat.map(path);
			bBoxO = path.boundingRect();
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->PoLine.fromQPainterPath(path);
			currentItem->PoLine.translate(-bBoxO.x(), 0);
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			break;
		case 1:
			cmdText += "filled Polygon";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			nrOfPoints = nPoints;
			createObjCode = 1;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			break;
		case 2:
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, posX, posY, bBox.width(), bBox.height(), 0, fillC, fillC, true);
			gList.groupItem = m_Doc->Items->at(z);
			gList.groupX = groupX + bBox.x();
			gList.groupY = groupY + bBox.y();
			gList.width = bBox.width();
			gList.height = bBox.height();
			gList.itemGroupName = "";
			ds.device()->seek(0x26);
			ds >> dummy;
			gList.nrOfItems = dummy;
			gList.counter = 0;
			listStack.push(gList);
			gList.groupItem->ClipEdited = true;
			gList.groupItem->FrameType = 3;
			gList.groupItem->setTextFlowMode(PageItem::TextFlowDisabled);
			gList.groupItem->OldB2 = gList.groupItem->width();
			gList.groupItem->OldH2 = gList.groupItem->height();
			gList.groupItem->updateClip();
			Elements.append(gList.groupItem);
			cmdText += QString("Group  Count %1").arg(dummy);
			break;
		case 3:
			cmdText += "Ellipse";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			cornerRadius = getValue(ds);
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			finishItem(currentItem);
			break;
		case 5:
			cmdText += "Text";
			ds >> dummy;
			ds >> fontID;
			ds >> nrOfChars;
			ds >> fontSize;
			createObjCode = 5;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), 0, lineColor, CommonStrings::None, true);
			currentItem = m_Doc->Items->at(z);
			break;
		case 6:
			cmdText += "Line";
			posEnd = getCoordinate(ds);
			posStart = getCoordinate(ds);
			if ((posEnd.x() < 0) || (posEnd.y() < 0) || (posStart.x() < 0) || (posStart.y() < 0))
			{
				QPointF tmp = posEnd;
				posEnd = posStart;
				posStart = tmp;
			}
			ds >> patternIndex;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			lineWidth = patternIndex * scaleFactor;
			path = QPainterPath();
			path.moveTo(posStart);
			path.lineTo(posEnd);
			bBoxO = path.boundingRect();
			if ((bBoxO.x() < 0) || (bBoxO.y() < 0))
			{
				path = QPainterPath();
				path.moveTo(posEnd);
				path.lineTo(posStart);
			}
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->PoLine.fromQPainterPath(path);
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
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			nrOfPoints = nPoints;
			createObjCode = 3;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			break;
		case 9:
			cmdText += "Pie Wedge";
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
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			cornerRadius = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			if (data8 == 11)
				currentItem->setCornerRadius(cornerRadius);
			handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			break;
		case 13:
			cmdText += "filled Ellipse";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			cornerRadius = getValue(ds);
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			handleGradient(currentItem, patternIndex, fillColor, backColor, bBox);
			break;
		case 14:
			cmdText += "elliptical Arc, clockwise";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			posStart = getCoordinate(ds);
			posEnd = getCoordinate(ds);
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			sLin = QLineF(QPointF(boundingBoxWO / 2.0, boundingBoxHO / 2.0), posStart);
			eLin = QLineF(QPointF(boundingBoxWO / 2.0, boundingBoxHO / 2.0), posEnd);
			rotS = sLin.angle();
			rotE = eLin.angle();
			rotE = rotE - rotS;
			path = QPainterPath();
			path.arcMoveTo(QRectF(0, 0, boundingBoxWO, boundingBoxHO), rotS);
			path.arcTo(QRectF(0, 0, boundingBoxWO, boundingBoxHO), rotS, rotE);
			path = mat.map(path);
			bBoxO = path.boundingRect();
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, posX, posY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->PoLine.fromQPainterPath(path);
			currentItem->PoLine.translate(-bBoxO.x(), 0);
			handleLineStyle(currentItem, flags, lineColor);
			finishItem(currentItem);
			break;
		case 15:
			cmdText += "filled parabolic Arc";
			break;
		case 16:
			cmdText += "filled quadratic Spline";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			nrOfPoints = nPoints;
//			createObjCode = 4;
//			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX + bBox.x() + bX + groupX, baseY + bBox.y() + bY + groupY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
//			currentItem = m_Doc->Items->at(z);
			break;
		case 17:
			ds >> patternIndex;
			fillColor = getColor(ds);
			ds.device()->seek(0x2B);
			ds >> nItems;
			ds >> dummy;
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			gElements.xoffset = bX + bBox.x() + groupX;
			gElements.yoffset = bY + bBox.y() + groupY;
			gElements.width = bBox.width();
			gElements.height = bBox.height();
			gElements.lineWidth = lineWidth;
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
			cmdText += QString("filled complex Object Count %1  Fill %2 Line %3 LW %4").arg(nItems).arg(fillColor).arg(lineColor).arg(lineWidth);
			break;
		case 18:
			cmdText += "parabolic Arc";
			break;
		case 19:
			cmdText += "quadratic Spline";
			ds >> patternIndex;
			fillColor = getColor(ds);
			if (patternIndex != 0)
				fillC = fillColor;
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			nrOfPoints = nPoints;
//			createObjCode = 4;
//			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX + bBox.x() + bX + groupX, baseY + bBox.y() + bY + groupY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
//			currentItem = m_Doc->Items->at(z);
			break;
		case 20:
			cmdText += "complex Polyline";
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
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
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
			if (groupStack.count() > 1)
			{
				if (groupStack.top().patternIndex != 0)
					fillC = groupStack.top().fillColor;
			}
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			ds >> dummy;
			ds >> dummy;
			ds >> dummy;
			ds.device()->seek(0x38);
			backColor = getColor(ds);
			lineWidth = getValue(ds);
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
			createObjCode = 6;
			z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, posX, posY, bBox.width(), bBox.height(), 0, CommonStrings::None, CommonStrings::None, true);
			currentItem = m_Doc->Items->at(z);
			currentItem->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
			finishItem(currentItem);
			break;
		case 26:
			cmdText += "virtual Bitmap";
			break;
		case 27:
			cmdText += "simple Clip Path";
			break;
		case 28:
			cmdText += "tiled Clip Path";
			break;
		case 29:
			cmdText += "Path Text";
			break;
		default:
			cmdText += "Unknown";
			break;
	}
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
//		qDebug() << "Gradfill Item:" << currentItem->itemName() << QString("Pattern %1").arg(patternIndex, 2, 16, QChar('0'));
		quint8 ind = patternIndex - 0x40;
		DRWGradient grad;
		if (gradientMap.contains(ind))
		{
			grad = gradientMap[ind];
			double xoff = bBox.width() * grad.xOffset;
			double yoff = bBox.height() * grad.yOffset;
			VGradient FillGradient = VGradient(VGradient::linear);
			FillGradient.clearStops();
			const ScColor& gradC1 = m_Doc->PageColors[fillColor];
			FillGradient.addStop( ScColorEngine::getRGBColor(gradC1, m_Doc), 0.0, 0.5, 1.0, fillColor, 100 );
			const ScColor& gradC2 = m_Doc->PageColors[backColor];
			FillGradient.addStop( ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0, 0.5, 1.0, backColor, 100 );
			if (grad.type == 1)
			{
				currentItem->GrType = 6;
				currentItem->fill_gradient = FillGradient;
				currentItem->setGradientVector(bBox.width() / 2.0, 0, bBox.width() / 2.0, yoff, 0, 0, 1, 0);
			}
			else
			{
				currentItem->GrType = 7;
				currentItem->fill_gradient = FillGradient;
				currentItem->setGradientVector(xoff, yoff, 0.0, 0.0, xoff, yoff, 1, 0);
			}
		}
	}
}

void DrwPlug::handlePreviewBitmap(QDataStream &ds)
{
	quint16 w, h, offs, flag, dummy;
	ds >> offs;					// read Offset to ColorTable
	ds >> dummy;
	ds >> w;					// read Width
	ds >> dummy;
	ds >> h;					// read Height;
	ds >> dummy;
	ds >> dummy;				// might indicate image type 1 = indexed
	ds >> dummy;				// might indicate bits used for data
	ds >> flag;					// flag for image data, 0 = uncompressed, 1 = compressed
	ds.device()->seek(offs);	// jump to start of ColorTable
	QVector<QRgb> colors;
	for (quint16 cc = 0; cc < 256; cc++)	// now reading ColorTable, exactly 1024 bytes
	{
		quint8 r, g, b, a;
		ds >> b >> g >> r >> a;				// values are stored in BGR order
		colors.append(qRgb(r, g, b));
	}
	QImage image = QImage(w, h, QImage::Format_Indexed8);
	image.setColorTable(colors);
	for (quint16 yy = 0; yy < h; yy++)		// now reading Image, data are index values to ColorTable
	{
		QByteArray data;
		data.resize(w);
		if (flag == 0)							// raw uncompressed data
		{
			ds.readRawData(data.data(), w);
			uint adj = ds.device()->pos() % 4;	// scanline data is aligned to 32bit words
			if (adj != 0)
				ds.skipRawData(adj);
		}
		else if (flag == 1)						// data is compressed
		{
			data.resize(0);
			quint16 count = 0;
			quint8 dd, val;
			while (count < w)
			{
				ds >> dd;								// read byte count
				if (dd != 0)
				{
					ds >> val;							// read value
					for (quint8 r = 0; r < dd; r++)
					{
						data.append(val);
					}
					count += dd;
				}
				else									// if byte count == 0, next byte is a counter
				{										// of uncompressed bytes to copy to result
					ds >> dd;
					for (quint8 r = 0; r < dd; r++)
					{
						ds >> val;
						data.append(val);
					}
					count += dd;
				}
			}
			ds >> dummy;
		}
		memcpy(image.scanLine(yy), data.data(), w);
	}
	image = image.convertToFormat(QImage::Format_ARGB32);
	image = image.mirrored(false, true);	// image data is upside down

	int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, image.width(), image.height(), 0, m_Doc->itemToolPrefs.imageFillColor, CommonStrings::None, true);
	PageItem *ite = m_Doc->Items->at(z);
	ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_drw_XXXXXX.png");
	ite->tempImageFile->open();
	QString fileName = getLongPathName(ite->tempImageFile->fileName());
	ite->tempImageFile->close();
	ite->isInlineImage = true;
	image.save(fileName, "PNG");
	ite->moveBy(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	m_Doc->LoadPict(fileName, z);
	ite->setImageScalingMode(false, false);
	ite->ClipEdited = true;
	ite->FrameType = 3;
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(),wh.y());
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_Doc->AdjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	Elements.append(ite);
}

QString DrwPlug::handleColor(ScColor &color, QString proposedName)
{
	QString tmpName = CommonStrings::None;
	int hR, hG, hB, hK;
	int pR, pG, pB, pC, pM, pY, pK;
	bool found = false;
	ColorList::Iterator it;
	if (color.getColorModel() == colorModelRGB)
		color.getRGB(&pR, &pG, &pB);
	else
		color.getCMYK(&pC, &pM, &pY, &pK);
	if ((!proposedName.isEmpty()) && ((proposedName == "White") || (proposedName == "Black") || (m_Doc->PageColors.contains(proposedName))))
		tmpName = proposedName;
	else
	{
		if (color.getColorModel() == colorModelRGB)
		{
			for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
			{
				if (it.value().getColorModel() == colorModelRGB)
				{
					it.value().getRGB(&hR, &hG, &hB);
					if ((pR == hR) && (pG == hG) && (pB == hB))
					{
						tmpName = it.key();
						found = true;
						break;
					}
				}
			}
			if (!found)
			{
				m_Doc->PageColors.insert(proposedName, color);
				importedColors.append(proposedName);
				tmpName = proposedName;
			}
		}
		else
		{
			for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
			{
				it.value().getCMYK(&hR, &hG, &hB, &hK);
				if ((pC == hR) && (pM == hG) && (pY == hB) && (pK == hK))
				{
					tmpName = it.key();
					found = true;
					break;
				}
			}
			if (!found)
			{
				m_Doc->PageColors.insert(proposedName, color);
				importedColors.append(proposedName);
				tmpName = proposedName;
			}
		}
	}
	return tmpName;
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
	ite->ClipEdited = true;
	ite->FrameType = 3;
//	FPoint wh = getMaxClipF(&ite->PoLine);
//	ite->setWidthHeight(wh.x(),wh.y());
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
//	m_Doc->AdjustItemSize(ite);
	if (scale)
	{
		QPainterPath pa = ite->PoLine.toQPainterPath(true);
		QRectF bb = pa.boundingRect();
		double scx = 1.0;
		double scy = 1.0;
		if ((bb.width() == 0.0) || (ite->width() == 0.0))
			scx = 1.0;
		else
			scx = ite->width() / bb.width();
		if ((bb.height() == 0.0) || (ite->height() == 0.0))
			scy = 1.0;
		else
			scy = ite->height() / bb.height();
//	qDebug() << scx << scy;
		ite->PoLine.scale(scx, scy);
		ite->setLineWidth(ite->lineWidth() / qMin(scx, scy));
	}
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	Elements.append(ite);
	if (groupStack.count() != 0)
		groupStack.top().GElements.append(ite);
	if (listStack.count() != 0)
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
