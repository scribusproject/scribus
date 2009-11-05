/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QByteArray>
#include <QCursor>
#include <QDrag>
#include <QFile>
#include <QMimeData>
#include <QRegExp>
#include <QTextCodec>
#include <QDebug>
#include "qtiocompressor.h"

#include <cstdlib>

#include "ui/customfdialog.h"
#include "importxar.h"
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

XarPlug::XarPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

bool XarPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
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
//	parseHeader(fName, x, y, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
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
		baseX = -x;
		baseY = -y;
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			baseX = m_Doc->currentPage()->xOffset() - x;
			baseY = m_Doc->currentPage()->yOffset() - y;
		}
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset() - x;
		baseY = m_Doc->currentPage()->yOffset() - y;
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
			if ((importerFlags & LoadSavePlugin::lfCreateDoc) && (!activeLayer.isEmpty()))
				m_Doc->setActiveLayer(activeLayer);
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

XarPlug::~XarPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

void XarPlug::parseHeader(QString fName, double &x, double &y, double &b, double &h)
{
	QFile f(fName);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ts(&f);
		ts.device()->seek(512);
		qint16 pgX, pgY, pgW, pgH, dummy;
		ts >> dummy >> pgX >> pgY >> pgW >> pgH;
		h = pgW - pgX;
		b = pgH - pgY;
		x = pgY;
		y = pgX;
		f.close();
//		qDebug() << "W" << b << "H" << h;
	}
}

bool XarPlug::convert(QString fn)
{
	QString tmp;
	CurrColorFill = "White";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	patternMode = false;
	patternData.resize(0);
	backColor = Qt::white;
	foreColor = Qt::black;
	Coords.resize(0);
	Coords.svgInit();
	LineW = 1.0;
	currentPointT = QPoint(0, 0);
	ovalSize = QPoint(0, 0);
	fontMap.clear();
	currentTextSize = 12;
	currentFontID = 0;
	currentFontStyle = 0;
	imageData.resize(0);
	lineMode = false;
	skipOpcode = false;
	postscriptMode = false;
	textIsPostScript = false;
	importedColors.clear();
	firstLayer = true;
	activeLayer = "";
	currentLayer = 0;
	QList<PageItem*> gElements;
	groupStack.push(gElements);
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		oldDocItemCount = m_Doc->Items->count();
		int fSize = (int) f.size();
		if (progressDialog)
		{
			progressDialog->setTotalSteps("GI", fSize);
			qApp->processEvents();
		}
		QDataStream ts(&f);
		ts.setByteOrder(QDataStream::LittleEndian);
		parseXar(ts);
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

void XarPlug::parseXar(QDataStream &ts)
{
	XarStyle *gc = new XarStyle;
	m_gc.push( gc );
	quint32 id;
	ts >> id;
	if (id != 0x41524158)
		return;
	ts >> id;
	if (id != 0x0A0DA3A3)
		return;
	recordCounter = 0;
	while (!ts.atEnd())
	{
		quint32 opCode, dataLen;
		ts >> opCode;
		ts >> dataLen;
		recordCounter++;
		if (opCode == 30)
		{
			ts.skipRawData(dataLen);
			quint64 pos = ts.device()->pos();
			QtIOCompressor compressor(ts.device(), 6, 1);
			compressor.setStreamFormat(QtIOCompressor::RawZipFormat);
			compressor.open(QIODevice::ReadOnly);
			QDataStream tsc(&compressor);
			tsc.setByteOrder(QDataStream::LittleEndian);
			tsc.device()->seek(pos);
			while (!tsc.atEnd())
			{
				tsc >> opCode;
				tsc >> dataLen;
				recordCounter++;
				if (opCode == 31)
				{
					tsc.skipRawData(dataLen);
					break;
				}
				handleTags(opCode, dataLen, tsc);
			}
			ts.skipRawData(dataLen+1);
		}
		else
			handleTags(opCode, dataLen, ts);
		if (progressDialog)
		{
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
}

void XarPlug::handleTags(quint32 tag, quint32 dataLen, QDataStream &ts)
{
	bool closed = false;
//	qDebug() << QString("OpCode: %1 Data Len %2").arg(tag).arg(dataLen, 8, 16, QLatin1Char('0'));
	if (tag == 0)
	{
		delete( m_gc.pop() );
		qDebug() << "Stack dropped to" << m_gc.count();
	}
	else if (tag == 1)
	{
		addGraphicContext();
		qDebug() << "Stack pushed to" << m_gc.count();
	}
	else if (tag == 10)
		addToAtomic(dataLen, ts);
	else if (tag == 45)
		handleSpreadInfo(ts);
	else if (tag == 48)
		handleLayerInfo(ts);
	else if (tag == 50)
		handleColorRGB(ts);
	else if (tag == 51)
		handleComplexColor(ts);
/*	else if ((tag == 61) || (tag == 62) || (tag == 63))
	{
		QImage image;
		QByteArray data;
		data.resize(dataLen);
		ts.readRawData(data.data(), dataLen);
		image.loadFromData(data);
		int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, image.width(), image.height(), 0, m_Doc->itemToolPrefs.imageFillColor, CommonStrings::None, true);
		PageItem *ite = m_Doc->Items->at(z);
		ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_xar_XXXXXX.png");
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
		ite->setFillShade(CurrFillShade);
		ite->setLineShade(CurrStrokeShade);
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->AdjustItemSize(ite);
		ite->OldB2 = ite->width();
		ite->OldH2 = ite->height();
		ite->updateClip();
		Elements.append(ite);
	} */
	else if (tag == 114)
	{
		closed = handlePathRel(ts, dataLen);
		if (closed)
			createPolygonItem(1);
		else
			createPolylineItem(1);
	}
	else if (tag == 115)
	{
		closed = handlePathRel(ts, dataLen);
		if (closed)
			createPolygonItem(0);
		else
			createPolylineItem(0);
	}
	else if (tag == 116)
	{
		closed = handlePathRel(ts, dataLen);
		if (closed)
			createPolygonItem(2);
		else
			createPolylineItem(2);
	}
	else if (tag == 150)
		handleFlatFill(ts);
	else if (tag == 151)
		handleLineColor(ts);
	else if (tag == 152)
		handleLineWidth(ts);
	else
	{
//		qDebug() << QString("OpCode: %1 Data Len %2").arg(tag).arg(dataLen, 8, 16, QLatin1Char('0'));
		ts.skipRawData(dataLen);
	}
}

void XarPlug::handleLineColor(QDataStream &ts)
{
	XarStyle *gc = m_gc.top();
	qint32 val;
	ts >> val;
	if (val > 0)
	{
		if (XarColorMap.contains(val))
		{
			gc->StrokeCol = XarColorMap[val].name;
			if (gc->Elements.count() > 0)
			{
				for (int a = 0; a < gc->Elements.count(); a++)
				{
					gc->Elements.at(a)->setLineColor(gc->StrokeCol);
				}
			}
		}
	}
}

void XarPlug::handleFlatFill(QDataStream &ts)
{
	XarStyle *gc = m_gc.top();
	qint32 val;
	ts >> val;
	if (val > 0)
	{
		if (XarColorMap.contains(val))
		{
			gc->FillCol = XarColorMap[val].name;
			if (gc->Elements.count() > 0)
			{
				for (int a = 0; a < gc->Elements.count(); a++)
				{
					gc->Elements.at(a)->setFillColor(gc->FillCol);
				}
			}
		}
	}
}

void XarPlug::handleLineWidth(QDataStream &ts)
{
	XarStyle *gc = m_gc.top();
	quint32 val;
	ts >> val;
	gc->LWidth = val / 1000.0;
	if (gc->Elements.count() > 0)
	{
		for (int a = 0; a < gc->Elements.count(); a++)
		{
			gc->Elements.at(a)->setLineWidth(gc->LWidth);
		}
	}
}

void XarPlug::createPolygonItem(int type)
{
	int z;
	XarStyle *gc = m_gc.top();
	if (type == 0)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, CommonStrings::None, gc->StrokeCol, true);
	else if (type == 1)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, gc->FillCol, CommonStrings::None, true);
	else if (type == 2)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
	finishItem(z);
}

void XarPlug::createPolylineItem(int type)
{
	int z;
	XarStyle *gc = m_gc.top();
	if (type == 0)
		z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, CommonStrings::None, gc->StrokeCol, true);
	else if (type == 1)
		z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, gc->FillCol, CommonStrings::None, true);
	else if (type == 2)
		z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, gc->LWidth, gc->FillCol, gc->StrokeCol, true);
	finishItem(z);
}

void XarPlug::finishItem(int z)
{
	PageItem *ite = m_Doc->Items->at(z);
	ite->PoLine = Coords.copy();
	ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	ite->ClipEdited = true;
	ite->FrameType = 3;
	FPoint wh = getMaxClipF(&ite->PoLine);
	ite->setWidthHeight(wh.x(),wh.y());
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	m_Doc->AdjustItemSize(ite);
	Elements.append(ite);
	XarStyle *gc = m_gc.top();
	gc->Elements.append(ite);
}

bool XarPlug::handlePathRel(QDataStream &ts, quint32 len)
{
	quint32 count = len / 9;
	qint32 x, y;
	quint8  verb, val;
	double co1, co2, cx1, cy1, cx2, cy2, cx3, cy3;
	FPoint currentPoint;
	int bezCount = 0;
	bool closed = false;
	Coords.resize(0);
	Coords.svgInit();
	for (uint a = 0; a < count; a++)
	{
		ts >> verb;
		ts >> val;
		x = val;
		ts >> val;
		y = val;
		ts >> val;
		x = (x << 8) | val;
		ts >> val;
		y = (y << 8) | val;
		ts >> val;
		x = (x << 8) | val;
		ts >> val;
		y = (y << 8) | val;
		ts >> val;
		x = (x << 8) | val;
		ts >> val;
		y = (y << 8) | val;
		co1 = x / 1000.0;
		co2 = y / 1000.0;
		switch (verb)
		{
			case 6:
				Coords.svgMoveTo(co1, docHeight - co2);
				currentPoint = FPoint(co1, co2);
				break;
			case 2:
			case 3:
				currentPoint = FPoint(currentPoint.x() - co1, currentPoint.y() - co2);
				Coords.svgLineTo(currentPoint.x(), docHeight - currentPoint.y());
				if (verb == 3)
				{
					closed = true;
					Coords.svgClosePath();
				}
				break;
			case 4:
			case 5:
				if (bezCount == 0)
				{
					cx1 = currentPoint.x() - co1;
					cy1 = currentPoint.y() - co2;
					currentPoint = FPoint(cx1, cy1);
					bezCount++;
				}
				else if (bezCount == 1)
				{
					cx2 = currentPoint.x() - co1;
					cy2 = currentPoint.y() - co2;
					currentPoint = FPoint(cx2, cy2);
					bezCount++;
				}
				else if (bezCount == 2)
				{
					cx3 = currentPoint.x() - co1;
					cy3 = currentPoint.y() - co2;
					currentPoint = FPoint(cx3, cy3);
					Coords.svgCurveToCubic(cx1, docHeight - cy1, cx2, docHeight - cy2, cx3, docHeight - cy3);
					if (verb == 5)
					{
						closed = true;
						Coords.svgClosePath();
					}
					bezCount = 0;
				}
				break;
		}
	}
	return closed;
}

void XarPlug::handleLayerInfo(QDataStream &ts)
{
	quint16 charC = 0;
	quint8 layerFlags;
	ts >> layerFlags;
	ts >> charC;
	QString XarName = "";
	while (charC != 0)
	{
		XarName += QChar(charC);
		ts >> charC;
	}
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (!firstLayer)
			currentLayer = m_Doc->addLayer(XarName, true);
		else
			m_Doc->changeLayerName(currentLayer, XarName);
		m_Doc->setLayerVisible(currentLayer, layerFlags & 1);
		m_Doc->setLayerLocked(currentLayer, layerFlags & 2);
		m_Doc->setLayerPrintable(currentLayer, layerFlags & 4);
		firstLayer = false;
		if (layerFlags & 8)
			activeLayer = XarName;
	}
}

void XarPlug::handleSpreadInfo(QDataStream &ts)
{
	quint32 pgWidth, pgHeight, margin, bleed;
	quint8 spreadFlags;
	ts >> pgWidth >> pgHeight >> margin >> bleed;
	ts >> spreadFlags;
	double w = pgWidth / 1000.0;
	double h = pgHeight / 1000.0;
//	double m = margin / 1000.0;
//	double b = bleed / 1000.0;
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->setPage(w, h, 0, 0, 0, 0, 0, 0, false, false);
		if (w > h)
			m_Doc->PageOri = 1;
		else
			m_Doc->PageOri = 0;
		m_Doc->m_pageSize = "Custom";
		m_Doc->changePageMargins(0, 0, 0, 0, h, w, h, w, m_Doc->PageOri, m_Doc->m_pageSize, m_Doc->currentPage()->pageNr(), 0);
	}
	docHeight = h;
	docWidth = w;
}

void XarPlug::handleComplexColor(QDataStream &ts)
{
	QString tmpName = CommonStrings::None;
	ScColor tmp;
	ColorList::Iterator it;
	quint8 Rc, Gc, Bc, colorModel, colorType;
	quint16 charC = 0;
	quint32 EntryIndex, component1, component2, component3, component4;
	qint32 colorRef;
	int hR, hG, hB;
	ts >> Rc >> Gc >> Bc >> colorModel >> colorType;
	ts >> EntryIndex;
	ts >> colorRef;
	ts >> component1 >> component2 >> component3 >> component4;
	ts >> charC;
	QString XarName = "";
	while (charC != 0)
	{
		XarName += QChar(charC);
		ts >> charC;
	}
/*	QString colM = "";
	switch (colorModel)
	{
		case 2:
			colM = "RGB";
			break;
		case 3:
			colM = "CMYK";
			break;
		case 4:
			colM = "HSV";
			break;
		case 5:
			colM = "Gray";
			break;
	}
	QString colT = "";
	switch (colorType)
	{
		case 0:
			colT = "Normal";
			break;
		case 1:
			colT = "Spot";
			break;
		case 2:
			colT = "Tint";
			break;
		case 3:
			colT = "Linked";
			break;
		case 4:
			colT = "Shade";
			break;
	} */
	double c1 = decodeColorComponent(component1);
	double c2 = decodeColorComponent(component2);
	double c3 = decodeColorComponent(component3);
	double c4 = decodeColorComponent(component4);
//	qDebug() << "Record" << recordCounter << "Complex Color" << XarName << colM << colT << colorRef;
//	qDebug() << "\t\tComponents" << c1 << c2 << c3 << c4;
	bool found = false;
	QColor c = QColor(Rc, Gc, Bc);
	if ((colorType == 0) || (colorType == 1))
	{
		if (colorModel == 3)
		{
			int Cc = qRound(c1 * 255);
			int Mc = qRound(c2 * 255);
			int Yc = qRound(c3 * 255);
			int Kc = qRound(c4 * 255);
			int hC, hM, hY, hK;
			tmp.setColor(Cc, Mc, Yc, Kc);
			for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
			{
				if (it.value().getColorModel() == colorModelCMYK)
				{
					it.value().getCMYK(&hC, &hM, &hY, &hK);
					if ((Cc == hC) && (Mc == hM) && (Yc == hY) && (Kc == hK))
					{
						tmpName = it.key();
						found = true;
						break;
					}
				}
			}
			if (!found)
			{
				if (colorType == 1)
					tmp.setSpotColor(true);
				else
					tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				if (XarName.isEmpty())
					tmpName = "FromXara"+c.name();
				else
					tmpName = XarName;
				m_Doc->PageColors.insert(tmpName, tmp);
				importedColors.append(tmpName);
			}
		}
		else
		{
			for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
			{
				if (it.value().getColorModel() == colorModelRGB)
				{
					it.value().getRGB(&hR, &hG, &hB);
					if ((Rc == hR) && (Gc == hG) && (Bc == hB))
					{
						tmpName = it.key();
						found = true;
						break;
					}
				}
			}
			if (!found)
			{
				tmp.setColorRGB(Rc, Gc, Bc);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				if (XarName.isEmpty())
					tmpName = "FromXara"+c.name();
				else
					tmpName = XarName;
				m_Doc->PageColors.insert(tmpName, tmp);
				importedColors.append(tmpName);
			}
		}
	}
	else
	{
		for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
		{
			if (it.value().getColorModel() == colorModelRGB)
			{
				it.value().getRGB(&hR, &hG, &hB);
				if ((Rc == hR) && (Gc == hG) && (Bc == hB))
				{
					tmpName = it.key();
					found = true;
					break;
				}
			}
		}
		if (!found)
		{
			tmp.setColorRGB(Rc, Gc, Bc);
			tmp.setSpotColor(false);
			tmp.setRegistrationColor(false);
			if (XarName.isEmpty())
				tmpName = "FromXara"+c.name();
			else
				tmpName = XarName;
			m_Doc->PageColors.insert(tmpName, tmp);
			importedColors.append(tmpName);
		}
	}
	XarColor color;
	color.colorType = colorType;
	color.colorModel = colorModel;
	color.colorRef = colorRef;
	color.component1 = c1;
	color.component2 = c2;
	color.component3 = c3;
	color.component1 = c4;
	color.name = tmpName;
	XarColorMap.insert(recordCounter, color);
}

void XarPlug::handleColorRGB(QDataStream &ts)
{
	QString tmpName = CommonStrings::None;
	ScColor tmp;
	ColorList::Iterator it;
	quint8 Rc, Gc, Bc;
	int hR, hG, hB;
	ts >> Rc >> Gc >> Bc;
	bool found = false;
	QColor c = QColor(Rc, Gc, Bc);
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&hR, &hG, &hB);
			if ((Rc == hR) && (Gc == hG) && (Bc == hB))
			{
				tmpName = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		tmp.setColorRGB(Rc, Gc, Bc);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		tmpName = "FromXara"+c.name();
		m_Doc->PageColors.insert(tmpName, tmp);
		importedColors.append(tmpName);
	}
}

double XarPlug::decodeColorComponent(quint32 data)
{
	double ret = 0.0;
	char man = (data & 0xFF000000) >> 24;
	if (man >= 0)
	{
		ret = (data & 0x00FFFFFF) / 16777215.0;
		ret = (ret + man);
	}
	else
	{
		ret = (~data & 0x00FFFFFF) / 16777215.0;
		ret = (ret + ~man) * -1;
	}
	return ret;
}

void XarPlug::addToAtomic(quint32 dataLen, QDataStream &ts)
{
	quint32 l = dataLen / 4;
	quint32 val;
	for (quint32 a = 0; a < l; a++)
	{
		ts >> val;
		atomicTags.append(val);
	}
}

void XarPlug::addGraphicContext()
{
	XarStyle *gc = new XarStyle;
	if ( m_gc.top() )
		*gc = *( m_gc.top() );
	gc->Elements.clear();
	m_gc.push( gc );
}
