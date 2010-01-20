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
	scaleFactor = 0.15;
	lineWidth = 1.0;
	lineColor = "Black";
	fillColor = "Black";
	createObjCode = 0;
	nrOfPoints = 0;
	symbolCount = 0;
	recordCount = 0;
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
	QDataStream ds(cmdData);
//	quint8 data8;
	quint16 data16;
	ds.setByteOrder(QDataStream::LittleEndian);
	QString cmdText = "";
	bool printMSG = false;
	switch (cmd)
	{
		case 1:
			cmdText = QString("DRW Background Color %1").arg(getColor(ds));
			break;
		case 2:
			cmdText = "DRW Facename";
			break;
		case 3:
			cmdText = "DRW Version";
			break;
		case 4:
			cmdText = "DRW ID";
			break;
		case 5:
			cmdText = "DRW Overlay";
			break;
		case 6:
			cmdText = "DRW Polygon";
			if (createObjCode == 1)
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
							Coords.svgClosePath();
						}
						else
							Coords.svgLineTo(coor.x(), coor.y());
					}
				}
				if (currentItem != NULL)
				{
				//	Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
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
				//	Coords.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
					currentItem->PoLine = Coords.copy();
					finishItem(currentItem);
				}
				createObjCode = 0;
				currentItem = NULL;
			}
			break;
		case 7:
			cmdText = "";
			decodeSymbol(ds);
			break;
		case 8:
			cmdText = "DRW Text";
			break;
		case 9:
			cmdText = "DRW Color";
			break;
		case 10:
			cmdText = "DRW Color Flag";
			break;
		case 11:
			cmdText = "DRW Preview Bitmap";
//			handlePreviewBitmap(ds);
			break;
		case 14:
			cmdText = "DRW View";
			break;
		case 15:
			cmdText = "DRW Old Grid";
			break;
		case 16:
			cmdText = "DRW Curr Overlay";
			break;
		case 17:
			cmdText = "DRW Visible";
			break;
		case 18:
			cmdText = "DRW Comment";
			break;
		case 19:
			cmdText = "DRW Info";
			break;
		case 20:
			cmdText = "DRW Bitmap";
			break;
		case 21:
			cmdText = "DRW Font";
			break;
		case 22:
			cmdText = "DRW Grid";
			break;
		case 23:
			cmdText = "DRW Overlay Name";
			break;
		case 24:
			cmdText = "DRW Dimensions";
			break;
		case 25:
			ds >> data16;
			scaleFactor = (1.0 / static_cast<double>(data16)) * 72.0;
			cmdText = QString("DRW Resolution %1").arg(data16);
			break;
		case 26:
			cmdText = "DRW Ruler";
			break;
		case 27:
			cmdText = "DRW Page";
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
			cmdText = "DRW Pattern";
			break;
		case 29:
			cmdText = "DRW Locked";
			break;
		case 30:
			cmdText = "DRW Gradient";
			break;
		case 31:
			cmdText = "DRW Text Hdr";
			break;
		case 32:
			cmdText = "DRW Band";
			break;
		case 33:
			ds >> data16;
			cmdText = QString("DRW Symbolversion %1").arg(data16);
			break;
		case 34:
			cmdText = "DRW Text Para";
			break;
		case 35:
			cmdText = "DRW Colortable";
			break;
		case 36:
			cmdText = "DRW Text Extra";
			break;
		case 37:
			cmdText = "DRW Max Link ID";
			break;
		case 44:
			cmdText = "Skip Symbols";
			break;
		case 254:
			cmdText = "DRW EOF";
			printMSG = true;
			break;
		case 255:
			cmdText = "DRW Start File";
			printMSG = true;
			break;
		default:
			cmdText = QString("Unknown Cmd-Nr %1  Data %2").arg(cmd).arg(QString(cmdData.toHex().left(64)));
			break;
	}
	if (printMSG)
	{
		qDebug() << cmdText; // << QString("at %1").arg(pos, 8, 16);
//		qDebug() << "\tData:" << cmdData.toHex().left(32);
	}
}

void DrwPlug::decodeSymbol(QDataStream &ds)
{
	symbolCount++;
	QString cmdText = QString("Symbol %1 Type:").arg(symbolCount);
	bool printMSG = false;
	double bX = 0.0;
	double bY = 0.0;
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
					groupStack.pop();
					cElements = groupStack.top();
					bX = cElements.xoffset;
					bY = cElements.yoffset;
					cmdText += QString("dropping complex Object Offsets %1 %2\n").arg(cElements.xoffset).arg(cElements.yoffset);
				}
				else
					break;
			}
			groupStack.top().counter++;
		}
	}
/*	if ((symbolCount > 53) && (symbolCount < 59))
	{
		QFile f(QString("/home/franz/cmddatas%1.bin").arg(symbolCount));
		f.open(QIODevice::WriteOnly);
		f.write(cmdData);
		f.close();
	} */
	int z;
	quint8 data8, flags, patternIndex, appFlags;
	quint16 dummy, nPoints;
	double boundingBoxXO, boundingBoxYO, boundingBoxWO, boundingBoxHO, cornerRadius;
	QRectF bBoxO;
	QString backColor;
	QString fillC = CommonStrings::None;
	createObjCode = 0;
	currentItem = NULL;
	ds >> data8;							// reading Symbol Type
// now reading common values
	ds >> flags;
	QPointF position = getCoordinate(ds);
	double boundingBoxX = getValue(ds);
	double boundingBoxY = getValue(ds);
	double boundingBoxW = getValue(ds);
	double boundingBoxH = getValue(ds);
	QRectF bBox = QRectF(QPointF(boundingBoxX, boundingBoxY), QPointF(boundingBoxW, boundingBoxH));
	double rotationAngle = getRawValue(ds);
	double scaleX = getRawValue(ds);
	double scaleY = getRawValue(ds);
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
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			ds >> dummy;
			ds >> dummy;
			lineWidth = getValue(ds);
			nrOfPoints = nPoints;
			createObjCode = 1;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX + bBox.x() + bX, baseY + bBox.y() + bY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			break;
		case 2:
			cmdText += "Group";
			break;
		case 3:
			cmdText += "Ellipse";
			ds >> patternIndex;
			fillColor = getColor(ds);
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			cornerRadius = getValue(ds);
			ds >> appFlags;
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			ds >> dummy;
			ds >> dummy;
			lineWidth = getValue(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + bBox.x() + bX, baseY + bBox.y() + bY, bBox.width(), bBox.height(), lineWidth, CommonStrings::None, lineColor, true);
			currentItem = m_Doc->Items->at(z);
		//	currentItem->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(currentItem);
			break;
		case 5:
			cmdText += "Text";
			break;
		case 6:
			cmdText += "Line";
			break;
		case 8:
			cmdText += "Polyline";
			ds >> patternIndex;
			fillColor = getColor(ds);
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			ds >> dummy;
			ds >> dummy;
			lineWidth = getValue(ds);
			nrOfPoints = nPoints;
			createObjCode = 1;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX + bBox.x() + bX, baseY + bBox.y() + bY, bBox.width(), bBox.height(), lineWidth, CommonStrings::None, lineColor, true);
			currentItem = m_Doc->Items->at(z);
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
			boundingBoxXO = getValue(ds);
			boundingBoxYO = getValue(ds);
			boundingBoxWO = getValue(ds);
			boundingBoxHO = getValue(ds);
			cornerRadius = getValue(ds);
			bBoxO = QRectF(QPointF(boundingBoxXO, boundingBoxYO), QPointF(boundingBoxWO, boundingBoxHO));
			ds >> appFlags;
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			ds >> dummy;
			ds >> dummy;
			lineWidth = getValue(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bBox.x() + bX, baseY + bBox.y() + bY, bBox.width(), bBox.height(), lineWidth, fillC, CommonStrings::None, true);
			currentItem = m_Doc->Items->at(z);
		//	currentItem->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(currentItem);
			if (data8 == 11)
				currentItem->setCornerRadius(cornerRadius);
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
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			ds >> dummy;
			ds >> dummy;
			lineWidth = getValue(ds);
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + bBox.x() + bX, baseY + bBox.y() + bY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
		//	currentItem->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
			finishItem(currentItem);
			break;
		case 14:
			cmdText += "elliptical Arc, clockwise";
			break;
		case 15:
			cmdText += "filled parabolic Arc";
			break;
		case 16:
			cmdText += "filled quadratic Spline";
			break;
		case 17:
			ds.device()->seek(0x2B);
			ds >> dummy;
			gElements.xoffset = bX + bBox.x();
			gElements.yoffset = bY + bBox.y();
			gElements.nrOfItems = dummy;
			gElements.counter = 0;
			groupStack.push(gElements);
			cmdText += QString("filled complex Object Count %1  Offsets %2 %3").arg(dummy).arg(gElements.xoffset).arg(gElements.yoffset);
			break;
		case 18:
			cmdText += "parabolic Arc";
			break;
		case 19:
			cmdText += "quadratic Spline";
			break;
		case 20:
			cmdText += "complex Polyline";
			break;
		case 22:
			cmdText += "Bitmap monochrome";
			break;
		case 23:
			cmdText += "Bezier line";
			ds >> patternIndex;
			fillColor = getColor(ds);
			ds >> dummy;
			ds >> nPoints;
			ds >> appFlags;
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			ds >> dummy;
			ds >> dummy;
			lineWidth = getValue(ds);
			nrOfPoints = nPoints;
			createObjCode = 2;
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX + bBox.x() + bX, baseY + bBox.y() + bY, bBox.width(), bBox.height(), lineWidth, CommonStrings::None, lineColor, true);
			currentItem = m_Doc->Items->at(z);
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
			backColor = getColor(ds);
			lineWidth = getValue(ds);
			ds >> dummy;
			ds >> dummy;
			lineWidth = getValue(ds);
			nrOfPoints = nPoints;
			createObjCode = 2;
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX + bBox.x() + bX, baseY + bBox.y() + bY, bBox.width(), bBox.height(), lineWidth, fillC, lineColor, true);
			currentItem = m_Doc->Items->at(z);
			break;
		case 25:
			cmdText += "Rich Text";
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
		qDebug() << cmdText;
//		qDebug() << "Pos" << position << "Box" << bBox;
//		qDebug() << "Rot" << rotationAngle << "Bounding Box" << bBoxO;
//		qDebug() << "Line" << lineColor << "LWidth" << lineWidth << "Fill" << fillColor;
//		qDebug() << "Scale" << scaleX << " " << scaleY;
//		qDebug() << QString("Flags %1").arg(flags, 8, 2, QChar('0'));
//		qDebug() << QString("Pattern %1").arg(patternIndex, 2, 16, QChar('0'));
//		if (createObjCode == 1)
//			qDebug() << "Expecting" << nrOfPoints;
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

void DrwPlug::finishItem(PageItem* ite)
{
	ite->ClipEdited = true;
	ite->FrameType = 3;
//	FPoint wh = getMaxClipF(&ite->PoLine);
//	ite->setWidthHeight(wh.x(),wh.y());
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
//	m_Doc->AdjustItemSize(ite);
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
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	Elements.append(ite);
	if (groupStack.count() != 0)
	{
		DRWGroup cElements = groupStack.top();
		cElements.GElements.append(ite);
	}
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
