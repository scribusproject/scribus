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
#include <QList>
#include <QMimeData>
#include <QRegExp>
#include <QTextCodec>
#include <QStack>
#include <QDebug>

#include <cstdlib>

#include "commonstrings.h"
#include "customfdialog.h"
#include "importpct.h"
#include "loadsaveplugin.h"
#include "missing.h"
#include "multiprogressdialog.h"
#include "pageitem_imageframe.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "propertiespalette.h"
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

PctPlug::PctPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

bool PctPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double x, y, b, h;
	bool ret = false;
	CustColors.clear();
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
	parseHeader(fName, x, y, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.PageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.PageHeight;
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
	m_Doc->view()->Deselect();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
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
				int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_Doc->toolSettings.dBrush, m_Doc->toolSettings.dPen, true);
				PageItem *neu = m_Doc->Items->takeAt(z);
				m_Doc->Items->insert(lowestItem, neu);
				neu->Groups.push(m_Doc->GroupCounter);
				neu->setItemName( tr("Group%1").arg(neu->Groups.top()));
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
	if ((showProgress) && (!interactive))
		m_Doc->view()->DrawNew();
	return success;
}

PctPlug::~PctPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

void PctPlug::parseHeader(QString fName, double &x, double &y, double &b, double &h)
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

bool PctPlug::convert(QString fn)
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
	currentPoint = QPoint(0, 0);
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
	importedPatterns.clear();
	QList<PageItem*> gElements;
	groupStack.push(gElements);
	currentItemNr = 0;
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
		ts.device()->seek(522);
		quint16 vers = 0;
		ts >> vers;
		while (vers == 0)
		{
			ts >> vers;
			if (vers == 0x00FF)
			{
				if (progressDialog)
					progressDialog->close();
				f.close();
				return false;
			}
		}
		if (vers == 0x1101)
		{
			pctVersion = 1;		// Pict Version 1
			parsePict(ts);
		}
		else
		{
			ts.skipRawData(4);	// skip the next 4 Bytes
			ts >> vers;		// read the version info
//			if (vers == 0x0FFFE)
				pctVersion = 2;	// Pict Extended Version 2
//			else if (vers == 0x0FFFF)
//				pctVersion = 3;	// Pict Version 2
//			else
//			{
//				if (progressDialog)
//					progressDialog->close();
//				f.close();
//				return false;	// bail out, no Mac Pict
//			}
			ts.skipRawData(22);
			parsePict(ts);
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

void PctPlug::parsePict(QDataStream &ts)
{
	while (!ts.atEnd())
	{
		quint16 opCode, dataLen;
		quint8 dataLenByte;
		quint32 dataLenLong;
		if (pctVersion == 1)
		{
			ts >> dataLenByte;
			opCode = dataLenByte;
		}
		else
			ts >> opCode;
		if (((opCode >= 0x0092) && (opCode <= 0x0097)) || ((opCode >= 0x009C) && (opCode <= 0x009F)) || ((opCode >= 0x00A2) && (opCode <= 0x00AF)))
		{
			// Reserved by Apple
//			qDebug() << "Reserved by Apple";
			ts >> dataLen;
			alignStreamToWord(ts, dataLen);
		}
		else if (((opCode >= 0x00B0) && (opCode <= 0x00CF)) || ((opCode >= 0x8000) && (opCode <= 0x80FF)))
		{
			// Reserved by Apple
//			qDebug() << "Reserved by Apple";
		}
		else if (((opCode >= 0x00D0) && (opCode <= 0x00FE)) || ((opCode >= 0x8100) && (opCode <= 0x81FF)))
		{
			// Reserved by Apple
//			qDebug() << "Reserved by Apple";
			ts >> dataLenLong;
			alignStreamToWord(ts, dataLenLong);
		}
		else if (((opCode >= 0x0100) && (opCode <= 0x01FF)) || ((opCode >= 0x02FF) && (opCode <= 0x0BFE)))
		{
			// Reserved by Apple
//			qDebug() << "Reserved by Apple";
			alignStreamToWord(ts, 2);
		}
		else if ((opCode >= 0x0C00) && (opCode <= 0x7EFF))
		{
			// Reserved by Apple
//			qDebug() << "Reserved by Apple";
			alignStreamToWord(ts, 24);
		}
		else if ((opCode >= 0x7F00) && (opCode <= 0x7FFF))
		{
			// Reserved by Apple
//			qDebug() << "Reserved by Apple";
			alignStreamToWord(ts, 254);
		}
		else
		{
			switch (opCode)
			{
				case 0x0000:		// NOP
//					qDebug() << "NOP";
					break;
				case 0x0001:		// Clipping Region
//					qDebug() << "Clipping Region";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen-2);
					break;
				case 0x0002:		// Background Pattern
					handleLineModeEnd();
//					qDebug() << "Background Pattern";
					alignStreamToWord(ts, 8);
					break;
				case 0x0003:		// Text Font
					handleTextFont(ts);
					break;
				case 0x0004:		// Text Style
					handleTextStyle(ts);
					break;
				case 0x0005:		// Text Mode
					handleLineModeEnd();
					ts >> dataLen;
//					qDebug() << "Text Mode" << dataLen;
//					alignStreamToWord(ts, 2);
					break;
				case 0x0006:		// Extra Space
//					qDebug() << "Extra Space";
					alignStreamToWord(ts, 4);
					break;
				case 0x0007:		// Pen Size
					handlePenSize(ts);
					break;
				case 0x0008:		// Pen Mode
					handleLineModeEnd();
					ts >> dataLen;
//					qDebug() << "Pen Mode" << dataLen;
//					alignStreamToWord(ts, 2);
					break;
				case 0x0009:		// Pen Pattern
					handlePenPattern(ts);
					break;
				case 0x000A:		// Fill Pattern
					handleLineModeEnd();
//					qDebug() << "Fill Pattern";
					alignStreamToWord(ts, 8);
					break;
				case 0x000B:		// Oval Size
					handleOvalSize(ts);
					break;
				case 0x000C:		// Origin
//					qDebug() << "Origin";
					alignStreamToWord(ts, 4);
					break;
				case 0x000D:		// Text Size
					handleTextSize(ts);
					break;
				case 0x000E:		// Foreground Color
					handleColor(ts, false);
					break;
				case 0x000F:		// Background Color
					handleColor(ts, true);
					break;
				case 0x0010:		// Text Ratio
					handleLineModeEnd();
//					qDebug() << "Text Ratio";
					alignStreamToWord(ts, 8);
					break;
				case 0x0011:		// Version
//					qDebug() << "Version";
					alignStreamToWord(ts, 1);
					break;
				case 0x0015:		// Fractional pen position
					handleLineModeEnd();
//					qDebug() << "Fractional pen position at" << ts.device()->pos();
					alignStreamToWord(ts, 2);
					break;
				case 0x0016:		// Extra char space
//					qDebug() << "Extra char space";
					alignStreamToWord(ts, 2);
					break;
				case 0x0017:
				case 0x0018:
				case 0x0019:
//					qDebug() << "Reserved by Apple";
					break;
				case 0x001A:		// Foreground color RGB
					handleColorRGB(ts, false);
					break;
				case 0x001B:		// Background color RGB
					handleColorRGB(ts, true);
					break;
				case 0x001C:		// Highlight mode
//					qDebug() << "Highlight mode";
					break;
				case 0x001D:		// Highlight color RGB
//					qDebug() << "Highlight color RGB";
					alignStreamToWord(ts, 6);
					break;
				case 0x001E:		// Use default highlight color
//					qDebug() << "Use default highlight color";
					break;
				case 0x0020:		// Line
					handleLine(ts);
					break;
				case 0x0021:		// Line To
					handleLineFrom(ts);
					break;
				case 0x0022:		// Short Line
					handleShortLine(ts);
					break;
				case 0x0023:		// Short Line To
					handleShortLineFrom(ts);
					break;
				case 0x0024:
				case 0x0025:
				case 0x0026:
				case 0x0027:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen);
					break;
				case 0x0028:		// Long Text
					handleLongText(ts);
					break;
				case 0x0029:		// Text DH
					handleDHText(ts);
					break;
				case 0x002A:		// Text DV
					handleDVText(ts);
					break;
				case 0x002B:		// Text DHV
					handleDHVText(ts);
					break;
				case 0x002C:		// Font Name
					handleFontName(ts);
					break;
				case 0x002D:		// Line justify
					handleLineModeEnd();
//					qDebug() << "Line justify";
					alignStreamToWord(ts, 10);
					break;
				case 0x002E:		// Glyph state
					handleLineModeEnd();
//					qDebug() << "Glyph state";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen);
					break;
				case 0x002F:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen);
					break;
				case 0x0030:		// Frame rect
				case 0x0031:		// Paint rect
				case 0x0032:		// Erase rect
				case 0x0033:		// Invert rect
				case 0x0034:		// Fill rect
					handleShape(ts, opCode);
					break;
				case 0x0035:
				case 0x0036:
				case 0x0037:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					alignStreamToWord(ts, 8);
					break;
				case 0x0038:		// Frame same rect
				case 0x0039:		// Paint same rect
				case 0x003A:		// Erase same rect
				case 0x003B:		// Invert same rect
				case 0x003C:		// Fill same rect
					handleSameShape(ts, opCode);
					break;
				case 0x003D:
				case 0x003E:
				case 0x003F:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					break;
				case 0x0040:		// Frame round rect
				case 0x0041:		// Paint round rect
				case 0x0042:		// Erase round rect
				case 0x0043:		// Invert round rect
				case 0x0044:		// Fill round rect
					handleShape(ts, opCode);
					break;
				case 0x0045:
				case 0x0046:
				case 0x0047:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					alignStreamToWord(ts, 8);
					break;
				case 0x0048:		// Frame same round rect
				case 0x0049:		// Paint same round rect
				case 0x004A:		// Erase same round rect
				case 0x004B:		// Invert same round rect
				case 0x004C:		// Fill same round rect
					handleSameShape(ts, opCode);
					break;
				case 0x004D:
				case 0x004E:
				case 0x004F:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					break;
				case 0x0050:		// Frame oval
				case 0x0051:		// Paint oval
				case 0x0052:		// Erase oval
				case 0x0053:		// Invert oval
				case 0x0054:		// Fill oval
					handleShape(ts, opCode);
					break;
				case 0x0055:
				case 0x0056:
				case 0x0057:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					alignStreamToWord(ts, 8);
					break;
				case 0x0058:		// Frame same oval
				case 0x0059:		// Paint same oval
				case 0x005A:		// Erase same oval
				case 0x005B:		// Invert same oval
				case 0x005C:		// Fill same oval
					handleSameShape(ts, opCode);
					break;
				case 0x005D:
				case 0x005E:
				case 0x005F:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					break;
				case 0x0060:		// Frame arc
					handleLineModeEnd();
//					qDebug() << "Frame arc";
					alignStreamToWord(ts, 12);
					break;
				case 0x0061:		// Paint arc
					handleLineModeEnd();
//					qDebug() << "Paint arc";
					alignStreamToWord(ts, 12);
					break;
				case 0x0062:		// Erase arc
					handleLineModeEnd();
//					qDebug() << "Erase arc";
					alignStreamToWord(ts, 12);
					break;
				case 0x0063:		// Invert arc
					handleLineModeEnd();
//					qDebug() << "Invert arc";
					alignStreamToWord(ts, 12);
					break;
				case 0x0064:		// Fill arc
					handleLineModeEnd();
//					qDebug() << "Fill arc";
					alignStreamToWord(ts, 12);
					break;
				case 0x0065:
				case 0x0066:
				case 0x0067:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					alignStreamToWord(ts, 12);
					break;
				case 0x0068:		// Frame same arc
					handleLineModeEnd();
//					qDebug() << "Frame same arc";
					alignStreamToWord(ts, 4);
					break;
				case 0x0069:		// Paint same arc
					handleLineModeEnd();
//					qDebug() << "Paint same arc";
					alignStreamToWord(ts, 4);
					break;
				case 0x006A:		// Erase same arc
					handleLineModeEnd();
//					qDebug() << "Erase same arc";
					alignStreamToWord(ts, 4);
					break;
				case 0x006B:		// Invert same arc
					handleLineModeEnd();
//					qDebug() << "Invert same arc";
					alignStreamToWord(ts, 4);
					break;
				case 0x006C:		// Fill same arc
					handleLineModeEnd();
//					qDebug() << "Fill same arc";
					alignStreamToWord(ts, 4);
					break;
				case 0x006D:
				case 0x006E:
				case 0x006F:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					alignStreamToWord(ts, 4);
					break;
				case 0x0070:		// Frame poly
				case 0x0071:		// Paint poly
				case 0x0072:		// Erase poly
				case 0x0073:		// Invert poly
				case 0x0074:		// Fill poly
					handlePolygon(ts, opCode);
					break;
				case 0x0075:
				case 0x0076:
				case 0x0077:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen-2);
					break;
				case 0x0078:		// Frame same poly
					handleLineModeEnd();
//					qDebug() << "Frame same poly";
					break;
				case 0x0079:		// Paint same poly
					handleLineModeEnd();
//					qDebug() << "Paint same poly";
					break;
				case 0x007A:		// Erase same poly
					handleLineModeEnd();
//					qDebug() << "Erase same poly";
					break;
				case 0x007B:		// Invert same poly
					handleLineModeEnd();
//					qDebug() << "Invert same poly";
					break;
				case 0x007C:		// Fill same poly
					handleLineModeEnd();
//					qDebug() << "Fill same poly";
					break;
				case 0x007D:
				case 0x007E:
				case 0x007F:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					break;
				case 0x0080:		// Frame region
//					qDebug() << "Frame region";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen-2);
					break;
				case 0x0081:		// Paint region
					handleLineModeEnd();
//					qDebug() << "Paint region";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen-2);
					break;
				case 0x0082:		// Erase region
					handleLineModeEnd();
//					qDebug() << "Erase region";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen-2);
					break;
				case 0x0083:		// Invert region
					handleLineModeEnd();
//					qDebug() << "Invert region";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen-2);
					break;
				case 0x0084:		// Fill region
					handleLineModeEnd();
//					qDebug() << "Fill region";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen-2);
					break;
				case 0x0085:
				case 0x0086:
				case 0x0087:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					ts >> dataLen;
					alignStreamToWord(ts, dataLen-2);
					break;
				case 0x0088:		// Frame same region
					handleLineModeEnd();
//					qDebug() << "Frame same region";
					break;
				case 0x0089:		// Paint same region
					handleLineModeEnd();
//					qDebug() << "Paint same region";
					break;
				case 0x008A:		// Erase same region
					handleLineModeEnd();
//					qDebug() << "Erase same region";
					break;
				case 0x008B:		// Invert same region
					handleLineModeEnd();
//					qDebug() << "Invert same region";
					break;
				case 0x008C:		// Fill same region
					handleLineModeEnd();
//					qDebug() << "Fill same region";
					break;
				case 0x008D:
				case 0x008E:
				case 0x008F:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					break;
				case 0x0090:		// Bits Rect
//					qDebug() << "Bits Rect";
					handlePixmap(ts, opCode);
					break;
				case 0x0091:		// Bits Region
//					qDebug() << "Bits Region";
					handlePixmap(ts, opCode);
					break;
				case 0x0098:		// Pack Bits Rect
//					qDebug() << "Pack Bits Rect";
					handlePixmap(ts, opCode);
					break;
				case 0x0099:		// Pack Bits Region
//					qDebug() << "Pack Bits Region";
					handlePixmap(ts, opCode);
					break;
				case 0x009A:		// Direct Bits Rect
//					qDebug() << "Direct Bits Rect";
					handlePixmap(ts, opCode);
					break;
				case 0x009B:		// Direct Bits Region
//					qDebug() << "Direct Bits Region";
					handlePixmap(ts, opCode);
					break;
				case 0x00A0:		// Short Comment
					handleComment(ts, false);
					break;
				case 0x00A1:		// Long Comment
					handleComment(ts, true);
					break;
				case 0x00FF:		// End of Pict
					handleLineModeEnd();
					if (imageData.size() > 0)
					{
						QImage image;
						image.loadFromData(imageData);
						image = image.convertToFormat(QImage::Format_ARGB32);
						int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, image.width(), image.height(), 0, CommonStrings::None, CommonStrings::None, true);
						PageItem *ite = m_Doc->Items->at(z);
						ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pct_XXXXXX.png");
						ite->tempImageFile->open();
						QString fileName = getLongPathName(ite->tempImageFile->fileName());
						ite->tempImageFile->close();
						ite->isInlineImage = true;
						image.save(fileName, "PNG");
						ite->moveBy(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
						finishItem(ite);
						m_Doc->LoadPict(fileName, z);
						ite->setImageScalingMode(false, false);
					}
//					qDebug() << "End of Pict";
					return;
					break;
				case 0x0200:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					alignStreamToWord(ts, 4);
					break;
				case 0x8200:		// Compressed QuickTime
				case 0x8201:		// Uncompressed QuickTime
					handleQuickTime(ts, opCode);
					break;
				case 0xFFFF:		// Reserved by Apple
//					qDebug() << "Reserved by Apple";
					ts >> dataLenLong;
					alignStreamToWord(ts, dataLenLong);
					break;
				default:
//					qDebug() << QString("Not implemented OpCode: 0x%1 at %2").arg(opCode, 4, 16, QLatin1Char('0')).arg(ts.device()->pos()-2);
					return;
					break;
			}
		}
		if (progressDialog)
		{
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
	}
}

void PctPlug::alignStreamToWord(QDataStream &ts, uint len)
{
	ts.skipRawData(len);
	if (pctVersion == 1)
		return;
	uint adj = ts.device()->pos() % 2;
	if (adj != 0)
		ts.skipRawData(1);
}

void PctPlug::handleColor(QDataStream &ts, bool back)
{
	
	handleLineModeEnd();
	QString tmpName = CommonStrings::None;
	ScColor tmp;
	ColorList::Iterator it;
	quint16 Rc, Gc, Bc;
	quint32 colVal;
	ts >> colVal;
//	qDebug() << "Color" << colVal << back;
	switch (colVal)
	{
		case 30:
			Rc = 0xFFFF;
			Gc = 0xFFFF;
			Bc = 0xFFFF;
			break;
		case 33:
			Rc = 0x0000;
			Gc = 0x0000;
			Bc = 0x0000;
			break;
		case 69:
			Rc = 0xFC00;
			Gc = 0xF37D;
			Bc = 0x052F;
			break;
		case 137:
			Rc = 0xF2D7;
			Gc = 0x0856;
			Bc = 0x84EC;
			break;
		case 205:
			Rc = 0xDD6B;
			Gc = 0x08C2;
			Bc = 0x06A2;
			break;
		case 273:
			Rc = 0x0241;
			Gc = 0xAB54;
			Bc = 0xEAFF;
			break;
		case 341:
			Rc = 0x0000;
			Gc = 0x64AF;
			Bc = 0x11B0;
			break;
		case 409:
			Rc = 0x0000;
			Gc = 0x0000;
			Bc = 0xD400;
			break;
		default:
			Rc = 0x0000;
			Gc = 0x0000;
			Bc = 0x0000;
			break;
	}
	int redC, greenC, blueC, hR, hG, hB;
	redC = qRound((Rc / 65535.0) * 255.0);
	greenC = qRound((Gc / 65535.0) * 255.0);
	blueC = qRound((Bc / 65535.0) * 255.0);
	bool found = false;
	QColor c = QColor(redC, greenC, blueC);
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&hR, &hG, &hB);
			if ((redC == hR) && (greenC == hG) && (blueC == hB))
			{
				tmpName = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		tmp.setColorRGB(redC, greenC, blueC);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		tmpName = "FromPict"+c.name();
		m_Doc->PageColors.insert(tmpName, tmp);
		importedColors.append(tmpName);
	}
	if (back)
	{
		CurrColorFill = tmpName;
		backColor = c;
	}
	else
	{
		CurrColorStroke = tmpName;
		foreColor = c;
	}
}

void PctPlug::handleColorRGB(QDataStream &ts, bool back)
{
	handleLineModeEnd();
	QString tmpName = CommonStrings::None;
	ScColor tmp;
	ColorList::Iterator it;
	quint16 Rc, Gc, Bc;
	int redC, greenC, blueC, hR, hG, hB;
	ts >> Rc >> Gc >> Bc;
	redC = qRound((Rc / 65535.0) * 255.0);
	greenC = qRound((Gc / 65535.0) * 255.0);
	blueC = qRound((Bc / 65535.0) * 255.0);
	bool found = false;
	QColor c = QColor(redC, greenC, blueC);
	for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&hR, &hG, &hB);
			if ((redC == hR) && (greenC == hG) && (blueC == hB))
			{
				tmpName = it.key();
				found = true;
				break;
			}
		}
	}
	if (!found)
	{
		tmp.setColorRGB(redC, greenC, blueC);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		tmpName = "FromPict"+c.name();
		m_Doc->PageColors.insert(tmpName, tmp);
		importedColors.append(tmpName);
	}
	if (back)
	{
		CurrColorFill = tmpName;
		backColor = c;
	}
	else
	{
		CurrColorStroke = tmpName;
		foreColor = c;
	}
}

void PctPlug::handlePenPattern(QDataStream &ts)
{
	handleLineModeEnd();
	patternData.resize(8);
	ts.readRawData(patternData.data(), 8);
	patternMode = false;
	for (int a = 0; a < patternData.size(); a++)
	{
		uchar d = patternData[a];
		if ((d != 0x00) && (d != 0xFF))
		{
			patternMode = true;
			break;
		}
	}
}

void PctPlug::handlePolygon(QDataStream &ts, quint16 opCode)
{
//	qDebug() << "Handle Polygon";
	handleLineModeEnd();
	quint16 polySize;
	ts >> polySize;				// read polygon size
	ts.skipRawData(8);			// skip bounding rect;
	polySize -= 14;				// subtract size count, bounding rect and first point from size
	qint16 x, y;
	ts >> y >> x;
	Coords.resize(0);
	Coords.svgInit();
	PageItem *ite;
	Coords.svgMoveTo(x, y);
	for(unsigned i = 0; i < polySize; i += 4)
	{
		ts >> y >> x;
		Coords.svgLineTo(x, y);
	}
	if (Coords.size() > 0)
	{
		int z;
		if (opCode == 0x0070)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
		else if (opCode == 0x0071)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorStroke, CommonStrings::None, true);
//		else if (opCode == 0x0072)
//			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CommonStrings::None, true);
		else if (opCode == 0x0074)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorStroke, CommonStrings::None, true);
		else
		{
//			qDebug() << QString("Not implemented OpCode: 0x%1").arg(opCode, 4, 16, QLatin1Char('0'));
			return;
		}
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite);
		if ((patternMode) && (opCode != 0x0070))
			setFillPattern(ite);
	}
}

void PctPlug::handleShape(QDataStream &ts, quint16 opCode)
{
	handleLineModeEnd();
	QRect bounds = readRect(ts);
//	qDebug() << QString("Handle Rect/Oval 0x%1").arg(opCode, 4, 16, QLatin1Char('0'));
	int z;
	PageItem *ite;
	if (opCode == 0x0030)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, LineW, CommonStrings::None, CurrColorStroke, true);
	else if (opCode == 0x0031)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
//	else if (opCode == 0x0032)
//		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorFill, CommonStrings::None, true);
	else if (opCode == 0x0034)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
	else if (opCode == 0x0040)
	{
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, LineW, CommonStrings::None, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->setCornerRadius(qMax(ovalSize.x(), ovalSize.y()));
		ite->SetFrameRound();
		m_Doc->setRedrawBounding(ite);
	}
	else if (opCode == 0x0041)
	{
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
		ite = m_Doc->Items->at(z);
		ite->setCornerRadius(qMax(ovalSize.x(), ovalSize.y()));
		ite->SetFrameRound();
		m_Doc->setRedrawBounding(ite);
	}
//	else if (opCode == 0x0042)
//	{
//		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorFill, CommonStrings::None, true);
//		ite = m_Doc->Items->at(z);
//		ite->setCornerRadius(qMax(ovalSize.x(), ovalSize.y()));
//		ite->SetFrameRound();
//		m_Doc->setRedrawBounding(ite);
//	}
	else if (opCode == 0x0044)
	{
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
		ite = m_Doc->Items->at(z);
		ite->setCornerRadius(qMax(ovalSize.x(), ovalSize.y()));
		ite->SetFrameRound();
		m_Doc->setRedrawBounding(ite);
	}
	else if (opCode == 0x0050)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, LineW, CommonStrings::None, CurrColorStroke, true);
	else if (opCode == 0x0051)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
//	else if (opCode == 0x0052)
//		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorFill, CommonStrings::None, true);
	else if (opCode == 0x0054)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + bounds.x(), baseY + bounds.y(), bounds.width() - 1, bounds.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
	else
	{
//		qDebug() << QString("Not implemented OpCode: 0x%1").arg(opCode, 4, 16, QLatin1Char('0'));
		return;
	}
	ite = m_Doc->Items->at(z);
	ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	currRect = bounds;
	currRectItemNr = z;
	currRectType = 0;
	if (opCode > 0x0044)
		currRectType = 1;
	finishItem(ite);
	if ((patternMode) && ((opCode != 0x0030) || (opCode != 0x0040) || (opCode != 0x0050)))
		setFillPattern(ite);
}

void PctPlug::handleSameShape(QDataStream &ts, quint16 opCode)
{
//	qDebug() << QString("Handle Same Rect/Oval 0x%1").arg(opCode, 4, 16, QLatin1Char('0'));
	int rectType = 0;
	if (opCode > 0x0050)
		rectType = 1;
	handleLineModeEnd();
	int z;
	PageItem *ite;
	if (currRectType == rectType)
	{
		ite = m_Doc->Items->at(currRectItemNr);
		if ((opCode == 0x0038) || (opCode == 0x0048) || (opCode == 0x0058))
		{
			ite->setLineColor(CurrColorStroke);
			ite->setLineWidth(LineW);
		}
//		else if ((opCode == 0x003A) || (opCode == 0x004A) || (opCode == 0x005A))
//			ite->setFillColor(CurrColorFill);
		else
			ite->setFillColor(CurrColorStroke);
	}
	else
	{
		if (opCode == 0x0038)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, LineW, CommonStrings::None, CurrColorStroke, true);
		else if (opCode == 0x0039)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
//		else if (opCode == 0x003A)
//			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorFill, CommonStrings::None, true);
		else if (opCode == 0x003C)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
		else if (opCode == 0x0048)
		{
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CommonStrings::None, CurrColorStroke, true);
			ite = m_Doc->Items->at(z);
			ite->setCornerRadius(qMax(ovalSize.x(), ovalSize.y()));
			ite->SetFrameRound();
			m_Doc->setRedrawBounding(ite);
		}
		else if (opCode == 0x0049)
		{
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
			ite = m_Doc->Items->at(z);
			ite->setCornerRadius(qMax(ovalSize.x(), ovalSize.y()));
			ite->SetFrameRound();
			m_Doc->setRedrawBounding(ite);
		}
//		else if (opCode == 0x004A)
//		{
//			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorFill, CommonStrings::None, true);
//			ite = m_Doc->Items->at(z);
//			ite->setCornerRadius(qMax(ovalSize.x(), ovalSize.y()));
//			ite->SetFrameRound();
//			m_Doc->setRedrawBounding(ite);
//		}
		else if (opCode == 0x004C)
		{
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
			ite = m_Doc->Items->at(z);
			ite->setCornerRadius(qMax(ovalSize.x(), ovalSize.y()));
			ite->SetFrameRound();
			m_Doc->setRedrawBounding(ite);
		}
		else if (opCode == 0x0058)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, LineW, CommonStrings::None, CurrColorStroke, true);
		else if (opCode == 0x0059)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
//		else if (opCode == 0x005A)
//			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorFill, CommonStrings::None, true);
		else if (opCode == 0x005C)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + currRect.x(), baseY + currRect.y(), currRect.width() - 1, currRect.height() - 1, 0, CurrColorStroke, CommonStrings::None, true);
		else
		{
//			qDebug() << QString("Not implemented OpCode: 0x%1").arg(opCode, 4, 16, QLatin1Char('0'));
			return;
		}
		ite = m_Doc->Items->at(z);
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite);
	}
	if ((patternMode) && ((opCode != 0x0038) || (opCode != 0x0048) || (opCode != 0x0058)))
		setFillPattern(ite);
}

void PctPlug::handleFontName(QDataStream &ts)
{
	handleLineModeEnd();
	quint16 dataLen, fontID;
	quint8 nameLen;
	ts >> dataLen >> fontID;
	ts >> nameLen;
	QByteArray fontRawName;
	fontRawName.resize(nameLen);
	ts.readRawData(fontRawName.data(), nameLen);
	QString fontName = fontRawName;
	fontName = fontName.simplified();
	SCFonts fonts = PrefsManager::instance()->appPrefs.AvailFonts;
	SCFontsIterator it(fonts);
	for ( ; it.hasNext() ; it.next())
	{
		
		if (fonts[it.currentKey()].scName().simplified() == fontName)
		{
			fontName = fonts[it.currentKey()].family();
			break;
		}
	}
	fontMap.insert(fontID, fontName);
	alignStreamToWord(ts, 0);
//	qDebug() << "Handle FontName" << fontName << "ID" << fontID;
}

void PctPlug::handleTextSize(QDataStream &ts)
{
	handleLineModeEnd();
	quint16 fontSize;
	ts >> fontSize;
	currentTextSize = fontSize;
//	qDebug() << "Handle Text Size" << fontSize;
}

void PctPlug::handleTextFont(QDataStream &ts)
{
	handleLineModeEnd();
	quint16 fontID;
	ts >> fontID;
	currentFontID = fontID;
//	qDebug() << "Handle Text Font" << fontID;
}

void PctPlug::handleTextStyle(QDataStream &ts)
{
	handleLineModeEnd();
	quint8 style;
	ts >> style;
	alignStreamToWord(ts, 0);
	currentFontStyle = style;
//	qDebug() << "Text Style" << style;
}

void PctPlug::handleLongText(QDataStream &ts)
{
	handleLineModeEnd();
	quint8 textLen;
	qint16 x, y;
	ts >> y >> x;
	ts >> textLen;
	QByteArray text;
	text.resize(textLen);
	ts.readRawData(text.data(), textLen);
	if (!textIsPostScript)
	{
		currentPointT = QPoint(x, y);
		createTextPath(text);
//		qDebug() << "Handle Long Text at" << x << y << text;
	}
	alignStreamToWord(ts, 0);
}

void PctPlug::handleDHText(QDataStream &ts)
{
	handleLineModeEnd();
	quint8 textLen, dh;
	ts >> dh >> textLen;
	QByteArray text;
	text.resize(textLen);
	ts.readRawData(text.data(), textLen);
	if (!textIsPostScript)
	{
		QPoint s = currentPointT;
		currentPointT = QPoint(s.x()+dh, s.y());
		createTextPath(text);
//		qDebug() << "Handle DH Text at" << currentPointT << text;
	}
	alignStreamToWord(ts, 0);
}

void PctPlug::handleDVText(QDataStream &ts)
{
	handleLineModeEnd();
	quint8 textLen, dv;
	ts >> dv >> textLen;
	QByteArray text;
	text.resize(textLen);
	ts.readRawData(text.data(), textLen);
	if (!textIsPostScript)
	{
		QPoint s = currentPointT;
		currentPointT = QPoint(s.x(), s.y()+dv);
		createTextPath(text);
//		qDebug() << "Handle DV Text at" << currentPointT << text;
	}
	alignStreamToWord(ts, 0);
}

void PctPlug::handleDHVText(QDataStream &ts)
{
	handleLineModeEnd();
	quint8 textLen, dv, dh;
	ts >> dh >> dv >> textLen;
	QByteArray text;
	text.resize(textLen);
	ts.readRawData(text.data(), textLen);
	if (!textIsPostScript)
	{
		QPoint s = currentPointT;
		currentPointT = QPoint(s.x()+dh, s.y()+dv);
		createTextPath(text);
//		qDebug() << "Handle DHV Text" << dh << dv << "->" << currentPointT << text;
	}
	alignStreamToWord(ts, 0);
}

void PctPlug::createTextPath(QByteArray textString)
{
	QTextCodec *codec = QTextCodec::codecForName("Apple Roman");
	QString string = codec->toUnicode(textString);
	QFont textFont;
	if (!fontMap.contains(currentFontID))
		textFont = QFont();
	else
	{
		QString fontName = fontMap[currentFontID];
		textFont = QFont(fontName, currentTextSize);
		QFontInfo inf(textFont);
//		qDebug() << "Using Font" << inf.family() << "for" << fontName;
	}
	textFont.setPixelSize(currentTextSize);
	if (currentFontStyle & 1)
		textFont.setBold(true);
	if (currentFontStyle & 2)
		textFont.setItalic(true);
	if (currentFontStyle & 4)
		textFont.setUnderline(true);
	FPointArray textPath;
	QPainterPath painterPath;
	painterPath.addText( currentPointT.x(), currentPointT.y(), textFont, string);
	textPath.fromQPainterPath(painterPath);
	if (textPath.size() > 0)
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, CurrColorStroke, CommonStrings::None, true);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = textPath;
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite);
		if (patternMode)
			setFillPattern(ite);
	}
}

void PctPlug::handlePenSize(QDataStream &ts)
{
//	qDebug() << "Handle Pen Size";
	handleLineModeEnd();
	quint16 x, y;
	ts >> y >> x;
	LineW = qMax(x, y);
}

void PctPlug::handleOvalSize(QDataStream &ts)
{
//	qDebug() << "Handle Oval Size";
	handleLineModeEnd();
	quint16 x, y;
	ts >> y >> x;
	ovalSize = QPoint(x, y);
}

void PctPlug::handleShortLine(QDataStream &ts)
{
	quint16 x, y;
	qint8 dh, dv;
	ts >> y >> x;
	ts >> dh >> dv;
	if ((dh == 0) && (dv == 0))
	{
		handleLineModeEnd();
		Coords.svgMoveTo(x, y);
		currentPoint = QPoint(x, y);
		return;
	}
	QPoint s = QPoint(x, y);
	if (currentPoint != s)
	{
		handleLineModeEnd();
		Coords.svgMoveTo(x, y);
	}
	Coords.svgLineTo(x+dh, y+dv);
	currentPoint = QPoint(x+dh, y+dv);
	lineMode = true;
//	qDebug() << "Handle Short Line" << x << y << "+" << dh << dv << "->" << currentPoint;
}

void PctPlug::handleShortLineFrom(QDataStream &ts)
{
	qint8 dh, dv;
	ts >> dh >> dv;
	if ((dh == 0) && (dv == 0))
		return;
	QPoint s = currentPoint;
	if (Coords.size() == 0)
		Coords.svgMoveTo(s.x(), s.y());
	Coords.svgLineTo(s.x()+dh, s.y()+dv);
	currentPoint = QPoint(s.x()+dh, s.y()+dv);
	lineMode = true;
//	qDebug() << "Handle Short Line from" << dh << dv << "->" << currentPoint;
}

void PctPlug::handleLine(QDataStream &ts)
{
	qint16 x1, x2, y1, y2;
	ts >> y1 >> x1;
	ts >> y2 >> x2;
	QPoint s = QPoint(x1, y1);
	if (currentPoint != s)
	{
		handleLineModeEnd();
		Coords.svgMoveTo(x1, y1);
	}
	Coords.svgLineTo(x2, y2);
	currentPoint = QPoint(x2, y2);
	lineMode = true;
//	qDebug() << "Handle Line" << x1 << y1 << "->" << currentPoint;
}

void PctPlug::handleLineFrom(QDataStream &ts)
{
	qint16 x, y;
	ts >> y >> x;
	if ((x == 0) && (y == 0))
		return;
	QPoint s = currentPoint;
	if (Coords.size() == 0)
		Coords.svgMoveTo(s.x(), s.y());
	Coords.svgLineTo(x, y);
	currentPoint = QPoint(x, y);
	lineMode = true;
//	qDebug() << "Handle Line from" << s << "->" << currentPoint;
}

void PctPlug::handlePixmap(QDataStream &ts, quint16 opCode)
{
	handleLineModeEnd();
	quint16 bytesPerLine, packType, pixel_type, bits_per_pixel, component_count, component_size;
	quint32 packSize, horizontal_resolution, vertical_resolution, color_table, plane_bytes;
	if ((opCode == 0x009A) || (opCode == 0x009B))
		ts.skipRawData(4);
	ts >> bytesPerLine;
	QRect bounds = readRect(ts);
	bool isPixmap = bytesPerLine & 0x8000;
	bytesPerLine &= 0x7FFF;
//	qDebug() << "Bytes per Line" << bytesPerLine << "Pixmap" << isPixmap;
//	qDebug() << "Bounds" << bounds.right() - bounds.left() << bounds.bottom() - bounds.top();
	QVector<QRgb> colors;
	if (isPixmap)
	{
		ts.skipRawData(2);	// skip Version info, always 0
		ts >> packType;
		ts >> packSize;
		ts >> horizontal_resolution >> vertical_resolution;
		ts >> pixel_type >> bits_per_pixel >> component_count >> component_size;
		ts >> plane_bytes >> color_table;
		ts.skipRawData(4);
//		qDebug() << "Pack Type" << packType;
//		qDebug() << "Pack Size" << packSize;
//		qDebug() << "Pixel Type" << pixel_type;
//		qDebug() << "Bits per Pixel" << bits_per_pixel;
//		qDebug() << "Component Count" << component_count << "Size" << component_size;
	// now reading color Table
		if ((opCode != 0x009A) && (opCode != 0x009B))
		{
			quint32 ct_seed;
			quint16 ct_flags, ct_size;
			ts >> ct_seed;
			ts >> ct_flags >> ct_size;
//			qDebug() << "ColorTable has" << ct_size << "Entries";
			for (quint16 cc = 0; cc < ct_size+1; cc++)
			{
				quint16 cev, cRed, cGreen, cBlue;
				ts >> cev >> cRed >> cGreen >> cBlue;
				colors.append(qRgb(cRed, cGreen, cBlue));
			}
		}
	}
// reading scrRect
	QRect scrRect = readRect(ts);
//	qDebug() << "Src Rect" << scrRect;
// reading dstRect
	QRect dstRect = readRect(ts);
//	qDebug() << "Dst Rect" << dstRect;
	ts.skipRawData(2);
	if ((opCode == 0x0091) || (opCode == 0x0099) || (opCode == 0x009B))
	{
		quint16 dataLen;
		ts >> dataLen;
		alignStreamToWord(ts, dataLen-2);
	}
	quint16 pixRows = bounds.bottom() - bounds.top();
	quint16 pixCols = bounds.right() - bounds.left();
	quint16 imgRows = dstRect.bottom() - dstRect.top();
	quint16 imgCols = dstRect.right() - dstRect.left();
	QImage image;
	if (isPixmap)
	{
		if (component_count == 1)
		{
			image = QImage(pixCols, pixRows, QImage::Format_Indexed8);
			image.setColorTable(colors);
		}
		else
			image = QImage(pixCols, pixRows, QImage::Format_ARGB32);
	}
	else
		image = QImage(pixCols, pixRows, QImage::Format_Mono);
	for (quint16 rr = 0; rr < pixRows; rr++)
	{
		quint16 pixByteCount;
		if (bytesPerLine < 250)
		{
			quint8 byteCount;
			ts >> byteCount;
			pixByteCount = byteCount;
		}
		else
			ts >> pixByteCount;
		if (!skipOpcode)
		{
			QByteArray data;
			data.resize(pixByteCount);
			ts.readRawData(data.data(), pixByteCount);
			int twoByte = 1;
			if (component_size == 5)
				twoByte = 2;
			QByteArray img;
			if (bytesPerLine < 8)
				img = data;
			else
				img = decodeRLE(data, bytesPerLine, twoByte);
			if ((opCode == 0x0098) || (opCode == 0x0099))
			{
				if (!isPixmap)
				{
					memcpy(image.scanLine(rr), img.data(), bytesPerLine);
				}
				else if (component_count == 1)
				{
					if (component_size == 4)
					{
						uchar *q = (uchar*)(image.scanLine(rr));
						for (int xx = 0; xx < img.size(); xx++)
						{
							uchar i = (img[xx] >> 4) & 0x0F;
							uchar j = img[xx] & 0x0F;
							*q++ = i;
							*q++ = j;
						}
					}
					else
						memcpy(image.scanLine(rr), img.data(), bytesPerLine);
				}
			}
			else if ((opCode == 0x009A) || (opCode == 0x009B))
			{
				if (component_size == 5)
				{
					QRgb *q = (QRgb*)(image.scanLine(rr));
					int imgDcount = 0;
					for (quint16 xx = 0; xx < pixCols; xx++)
					{
						uchar i = img[imgDcount++];
						uchar j = img[imgDcount++];
						quint16 r = (i & 0x7c) << 1;
						quint16 g = ((i & 0x03) << 6) | ((j & 0xe0) >> 2);
						quint16 b = (j & 0x1f) << 3;
						*q++ = qRgba(r, g, b, 255);
					}
				}
				else if (component_size == 8)
				{
					QRgb *q = (QRgb*)(image.scanLine(rr));
					for (uint xx = 0; xx < (uint) pixCols; xx++)
					{
						uchar r, g, b;
						uchar a = 255;
						if (component_count == 3)
						{
							r = img[xx];
							g = img[xx + pixCols];
							b = img[xx + 2 * pixCols];
						}
						else if (component_count == 4)
						{
							a = 255 - img[xx];
							r = img[xx + pixCols];
							g = img[xx + 2 * pixCols];
							b = img[xx + 3 * pixCols];
						}
						*q++ = qRgba(r, g, b, a);
					}
				}
			}
		}
		else
		{
			ts.skipRawData(pixByteCount);
		}
	}
	if (skipOpcode)
	{
		image.loadFromData(imageData);
		isPixmap = true;
		imageData.resize(0);
	}
	if ((component_size == 8) || (component_size == 1) || (component_size == 5) || (component_size == 4) || (!isPixmap) || (skipOpcode))
	{
		image = image.convertToFormat(QImage::Format_ARGB32);
		if (!isPixmap)
			image.invertPixels();
		int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX + dstRect.left(), baseY + dstRect.top(), imgCols, imgRows, 0, CommonStrings::None, CommonStrings::None, true);
		PageItem *ite = m_Doc->Items->at(z);
		ite->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pct_XXXXXX.png");
		ite->tempImageFile->open();
		QString fileName = getLongPathName(ite->tempImageFile->fileName());
		ite->tempImageFile->close();
		ite->isInlineImage = true;
		image.save(fileName, "PNG");
		ite->moveBy(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite);
		m_Doc->LoadPict(fileName, z);
		ite->setImageScalingMode(false, false);
		skipOpcode = false;
	}
	alignStreamToWord(ts, 0);
}

void PctPlug::handleQuickTime(QDataStream &ts, quint16 opCode)
{
//	qDebug() << "Handle QuickTime Data";
	quint32 dataLenLong, matteSize, maskSize, dataLen;
	quint16 mode;
	ts >> dataLenLong;
	uint pos = ts.device()->pos();
	handleLineModeEnd();
	alignStreamToWord(ts, 38);		// Skip version and Matrix information
	ts >> matteSize;
	QRect matteRect = readRect(ts);
	if (opCode == 0x8200)
	{
		ts >> mode;
		QRect srcRect = readRect(ts);
		alignStreamToWord(ts, 4);
		ts >> maskSize;
		if (matteSize != 0)
		{
			ts >> dataLen;
			alignStreamToWord(ts, dataLen);
			alignStreamToWord(ts, matteSize);
		}
		if (maskSize != 0)
		{
			ts >> dataLen;
			alignStreamToWord(ts, dataLen);
			alignStreamToWord(ts, maskSize);
		}
		quint32 cType, vendor, dummyLong, imgDataSize;
		quint16 width, height, dummyShort;
		ts >> dataLen;
		ts >> cType;
		if (cType == 0x6A706567)
		{
			ts >> dummyLong;
			ts >> dummyShort;
			ts >> dummyShort;
			ts >> dummyShort;
			ts >> dummyShort;
			ts >> vendor;
			ts >> dummyLong;
			ts >> dummyLong;
			ts >> width;
			ts >> height;
			ts >> dummyLong;
			ts >> dummyLong;
			ts >> imgDataSize;
			alignStreamToWord(ts, 38);
			imageData.resize(imgDataSize);
			ts.readRawData(imageData.data(), imgDataSize);
			skipOpcode = true;
		}
	}
	else
	{
		if (matteSize != 0)
		{
			ts >> dataLen;
			alignStreamToWord(ts, dataLen);
			alignStreamToWord(ts, matteSize);
		}
		ts >> mode;
		handlePixmap(ts, mode);
		skipOpcode = true;
	}
	ts.device()->seek(pos + dataLenLong);
//	qDebug() << "File Pos" << ts.device()->pos();
}

void PctPlug::handleComment(QDataStream &ts, bool longComment)
{
	quint16 commentCode;
	handleLineModeEnd();
	ts >> commentCode;
	switch (commentCode)
	{
		case 100:			// picAppComment
//			qDebug() << "Comment type: picAppComment";
			break;
		case 130:			// picDwgBeg
//			qDebug() << "Comment type: picDwgBeg";
			break;
		case 131:			// picDwgEnd
//			qDebug() << "Comment type: picDwgEnd";
			break;
		case 140:			// picGrpBeg
//			qDebug() << "Comment type: picGrpBeg";
			break;
		case 141:			// picGrpEnd
//			qDebug() << "Comment type: picGrpEnd";
			break;
		case 142:			// picBitBeg
//			qDebug() << "Comment type: picBitBeg";
			break;
		case 143:			// picBitEnd
//			qDebug() << "Comment type: picBitEnd";
			break;
		case 150:			// TextBegin
//			qDebug() << "Comment type: TextBegin";
			break;
		case 151:			// TextEnd
//			qDebug() << "Comment type: TextEnd";
			break;
		case 152:			// StringBegin
//			qDebug() << "Comment type: StringBegin";
			break;
		case 153:			// StringEnd
//			qDebug() << "Comment type: StringEnd";
			break;
		case 154:			// TextCenter
//			qDebug() << "Comment type: TextCenter";
			break;
		case 155:			// LineLayoutOff
//			qDebug() << "Comment type: LineLayoutOff";
			break;
		case 156:			// LineLayoutOn
//			qDebug() << "Comment type: LineLayoutOn";
			break;
		case 157:			// ClientLineLayout
//			qDebug() << "Comment type: ClientLineLayout";
			break;
		case 160:			// PolyBegin
//			qDebug() << "Comment type: PolyBegin";
			break;
		case 161:			// PolyEnd
//			qDebug() << "Comment type: PolyEnd";
			break;
		case 163:			// PolyIgnore
//			qDebug() << "Comment type: PolyIgnore";
			break;
		case 164:			// PolySmooth
//			qDebug() << "Comment type: PolySmooth";
			break;
		case 165:			// PolyClose
//			qDebug() << "Comment type: PolyClose";
			break;
		case 170:			// picArrw1 Arrowhead on 2nd point of line
//			qDebug() << "Comment type: picArrw1";
			break;
		case 171:			// picArrw2 Arrowhead on 1nd point of line
//			qDebug() << "Comment type: picArrw2";
			break;
		case 172:			// picArrw3 Arrowhead on both endpoints
//			qDebug() << "Comment type: picArrw3";
			break;
		case 173:			// picArrwEnd End of arrowhead comment
//			qDebug() << "Comment type: picArrwEnd";
			break;
		case 180:			// DashedLine
//			qDebug() << "Comment type: DashedLine";
			break;
		case 181:			// DashedStop
//			qDebug() << "Comment type: DashedStop";
			break;
		case 182:			// SetLineWidth
//			qDebug() << "Comment type: SetLineWidth";
			break;
		case 190:			// PostScriptBegin
			postscriptMode = true;
//			qDebug() << "Comment type: PostScriptBegin";
			break;
		case 191:			// PostScriptEnd
			postscriptMode = false;
			textIsPostScript = false;
//			qDebug() << "Comment type: PostScriptEnd";
			break;
		case 192:			// PostScriptHandle
//			qDebug() << "Comment type: PostScriptHandle";
			break;
		case 193:			// PostScriptFile
//			qDebug() << "Comment type: PostScriptFile";
			break;
		case 194:			// TextIsPostScript
			textIsPostScript = true;
//			qDebug() << "Comment type: TextIsPostScript";
			break;
		case 195:			// ResourcePS
//			qDebug() << "Comment type: ResourcePS";
			break;
		case 196:			// PSBeginNoSave
//			qDebug() << "Comment type: PSBeginNoSave";
			break;
		case 200:			// RotateBegin
//			qDebug() << "Comment type: RotateBegin";
			break;
		case 201:			// RotateEnd
//			qDebug() << "Comment type: RotateEnd";
			break;
		case 210:			// FormsPrinting
//			qDebug() << "Comment type: FormsPrinting";
			break;
		case 211:			// EndFormsPrinting
//			qDebug() << "Comment type: EndFormsPrinting";
			break;
		case 220:			// CMBeginProfile
//			qDebug() << "Comment type: CMBeginProfile";
			break;
		case 221:			// CMEndProfile
//			qDebug() << "Comment type: CMEndProfile";
			break;
		case 222:			// CMEnableMatching
//			qDebug() << "Comment type: CMEnableMatching";
			break;
		case 223:			// CMDisableMatching
//			qDebug() << "Comment type: CMDisableMatching";
			break;
		default:
//			qDebug() << "Unknown Pict-Comment" << commentCode;
			break;
	}
	if (longComment)
	{
		quint16 dataLen;
		ts >> dataLen;
		alignStreamToWord(ts, dataLen);
	}
}

QRect PctPlug::readRect(QDataStream &ts)
{
	qint16 RectX, RectY, RectW, RectH;
	ts >> RectX >> RectY >> RectW >> RectH;
	return QRect(QPoint(RectY, RectX), QPoint(RectH, RectW));
}

QByteArray PctPlug::decodeRLE(QByteArray &in, quint16 bytesPerLine, int multByte)
{
	QByteArray ret = QByteArray(bytesPerLine, ' ');
	uchar *ptrOut, *ptrIn;
	ptrOut = (uchar*)ret.data();
	ptrIn = (uchar*)in.data();
	quint16 count = 0;
	uchar c, c2;
	quint16 len;
	while( count < in.size() )
	{
		c = *ptrIn++;
		count++;
		len = c;
		if( len < 128 )
		{
			// Copy next len+1 bytes literally.
			len++;
			len *= multByte;
			while( len != 0 )
			{
				*ptrOut++ = *ptrIn++;
				len--;
				count++;
				if (multByte == 2)
				{
					*ptrOut++ = *ptrIn++;
					len--;
					count++;
				}
			}
		}
		else if( len > 128 )
		{
			// Next -len+1 bytes in the dest are replicated from next source byte.
			// (Interpret len as a negative 8-bit int.)
			len ^= 0xFF;
			len += 2;
			len *= multByte;
			if (multByte == 2)
			{
				c = *ptrIn++;
				count++;
				c2 = *ptrIn++;
				count++;
				while( len != 0 )
				{
					*ptrOut++ = c;
					*ptrOut++ = c2;
					len--;
					len--;
				}
			}
			else
			{
				c = *ptrIn++;
				count++;
				while( len != 0 )
				{
					*ptrOut++ = c;
					len--;
				}
			}
		}
		else if( len == 128 )
		{
			// No-op.
		}
	}
	return ret;
}

void PctPlug::setFillPattern(PageItem* ite)
{
	uint oldNum = m_Doc->TotalItems;
	QString patternName;
	quint32 patDat1, patDat2;
	QDataStream bu(&patternData, QIODevice::ReadOnly);
	bu >> patDat1 >> patDat2;
	QString patNa = QString("%1%2%3%4").arg(backColor.name()).arg(foreColor.name()).arg(patDat1, 8, 16, QLatin1Char('0')).arg(patDat2, 8, 16, QLatin1Char('0'));
	if (!patternMap.contains(patNa))
	{
		QImage image = QImage(8, 8, QImage::Format_Mono);
		QVector<QRgb> colors;
		colors.append(backColor.rgb());
		colors.append(foreColor.rgb());
		image.setColorTable(colors);
		for (int rr = 0; rr < 8; rr++)
		{
			uchar *q = (uchar*)(image.scanLine(rr));
			*q = patternData[rr];
		}
		image = image.convertToFormat(QImage::Format_ARGB32);
		ScPattern pat = ScPattern();
		pat.setDoc(m_Doc);
		PageItem* newItem = new PageItem_ImageFrame(m_Doc, 0, 0, 1, 1, 0, CommonStrings::None, CommonStrings::None);
		newItem->tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pct_XXXXXX.png");
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
		importedPatterns.append(patternName);
		patternMap.insert(patNa, patternName);
	}
	else
		patternName = patternMap[patNa];
	ite->setPattern(patternName);
	ite->GrType = 8;
	m_Doc->TotalItems = oldNum;
//	qDebug() << "Using Pattern" << patternName << "internal Name" << patNa;
}

void PctPlug::handleLineModeEnd()
{
	if ((Coords.size() > 3) && lineMode)
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
		PageItem *ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		finishItem(ite);
	}
	Coords.resize(0);
	Coords.svgInit();
	lineMode = false;
}

void PctPlug::finishItem(PageItem* ite)
{
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
	lastCoords = Coords;
	Coords.resize(0);
	Coords.svgInit();
}
