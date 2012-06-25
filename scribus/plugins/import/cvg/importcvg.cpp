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
#include <QStack>
#include <QDebug>

#include <cstdlib>

#include "commonstrings.h"
#include "customfdialog.h"
#include "importcvg.h"
#include "loadsaveplugin.h"
#include "missing.h"
#include "multiprogressdialog.h"
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

CvgPlug::CvgPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

bool CvgPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double b, h;
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
	b = 0.0;
	h = 0.0;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	parseHeader(fName, b, h);
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
		baseX = 0;
		baseY = 0;
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

CvgPlug::~CvgPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

void CvgPlug::parseHeader(QString fName, double &b, double &h)
{
	QFile f(fName);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ts(&f);
		ts.device()->seek(16);
		quint16 pgY, pgW, pgH;
		ts >> pgY >> pgW >> pgH;
		b = pgW / 72.0;
		h = pgH / 72.0;
		scPg = h / b;
		b = pgY / 72.0;
		h = pgY / 72.0 * scPg;
		f.close();
	}
}

bool CvgPlug::convert(QString fn)
{
	QString tmp;
	CurrColorFill = "White";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	Coords.resize(0);
	Coords.svgInit();
	importedColors.clear();
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
		ts.device()->seek(10);
		quint16 pgX, pgY, pgW, pgH, colorF, dummy;
		bool colorFlag = false;
		ts >> colorF >> dummy;
		if (colorF == 0x044c)
			colorFlag = true;
		ts >> pgX >> pgY >> pgW >> pgH;
		quint32 currentFilePos = 14;
		while (!ts.atEnd())
		{
			quint16 data;
			quint32 lenData;
			ts >> data;
			ts >> lenData;
			currentFilePos = ts.device()->pos();
			if (data == 0x0020)
				qDebug() << "Group" << "Len" << lenData;
			if (data == 0x0004)
			{
				getObjects(ts, colorFlag, lenData - 26);
			}
			ts.device()->seek(currentFilePos + lenData - 6);
			if (progressDialog)
			{
				progressDialog->setProgress("GI", ts.device()->pos());
				qApp->processEvents();
			}
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

void CvgPlug::getObjects(QDataStream &ts, bool color, quint32 lenData)
{
	quint16 obX, obY, obW, obH, fillFlag, lineWidth;
	quint32 counter = 0;
	quint32 colorFill, colorLine;
	int z;
	PageItem *ite;
	Coords.resize(0);
	Coords.svgInit();
	ts >> obX >> obY >> obW >> obH;
	ts >> colorFill >> colorLine;
	ts >> fillFlag >> lineWidth;
	qreal scX = obW / 16384.0;
	qreal scY = obH / 16384.0;
	parseColor(colorFill, colorLine, color, fillFlag);
	while (counter < lenData)
	{
		quint16 opCode, x1, y1, cx1, cy1, cx2, cy2;
		ts >> opCode;
		counter += 2;
		if (opCode == 0x0000)		// moveTo
		{
			ts >> x1 >> y1;
			counter += 4;
			Coords.svgMoveTo(x1 / 72.0 * scX, y1 / 72.0 * scY * scPg);
		}
		else if (opCode == 0x0001)	// lineTo
		{
			ts >> x1 >> y1;
			counter += 4;
			Coords.svgLineTo(x1 / 72.0 * scX, y1 / 72.0 * scY * scPg);
		}
		else if (opCode == 0x0002)	// curveTo
		{
			ts >> cx1 >> cy1 >> cx2 >> cy2 >> x1 >> y1;
			counter += 12;
			Coords.svgCurveToCubic(cx1 / 72.0 * scX, cy1 / 72.0 * scY * scPg, cx2 / 72.0 * scX, cy2 / 72.0 * scY * scPg, x1 / 72.0 * scX, y1 / 72.0 * scY * scPg);
		}
		else if (opCode == 0x000f)
			break;
	}
	if (Coords.size() > 0)
	{
		Coords.svgClosePath();
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX + obX / 72.0, baseY + obY / 72.0 * scPg, 10, 10, lineWidth / 72.0, CurrColorFill, CurrColorStroke, true);
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
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
		if (groupStack.count() != 0)
			groupStack.top().append(ite);
	}
}

void CvgPlug::parseColor(quint32 dataF, quint32 dataS, bool color, quint16 flag)
{
	ScColor tmp;
	ColorList::Iterator it;
	CurrColorFill = CommonStrings::None;
	CurrFillShade = 100.0;
	CurrColorStroke = CommonStrings::None;
	CurrStrokeShade = 100.0;
	QColor c;
	int Rc, Gc, Bc, hR, hG, hB;
	if (color)
	{
		bool found = false;
		if ((flag == 0x0080) || (flag == 0x0200))
		{
			c.setRgb(dataF);
			Rc = c.red();
			Gc = c.green();
			Bc = c.blue();
			for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
			{
				if (it.value().getColorModel() == colorModelRGB)
				{
					it.value().getRGB(&hR, &hG, &hB);
					if ((Rc == hR) && (Gc == hG) && (Bc == hB))
					{
						CurrColorFill = it.key();
						found = true;
						break;
					}
				}
			}
			if (!found)
			{
				tmp.fromQColor(c);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString newColorName = "FromCVG"+c.name();
				m_Doc->PageColors.insert(newColorName, tmp);
				importedColors.append(newColorName);
				CurrColorFill = newColorName;
			}
		}
		if ((flag == 0x0080) || (flag == 0x0100))
		{
			found = false;
			c.setRgb(dataS);
			Rc = c.red();
			Gc = c.green();
			Bc = c.blue();
			for (it = m_Doc->PageColors.begin(); it != m_Doc->PageColors.end(); ++it)
			{
				if (it.value().getColorModel() == colorModelRGB)
				{
					it.value().getRGB(&hR, &hG, &hB);
					if ((Rc == hR) && (Gc == hG) && (Bc == hB))
					{
						CurrColorStroke = it.key();
						found = true;
						break;
					}
				}
			}
			if (!found)
			{
				tmp.fromQColor(c);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString newColorName = "FromCVG"+c.name();
				m_Doc->PageColors.insert(newColorName, tmp);
				importedColors.append(newColorName);
				CurrColorStroke = newColorName;
			}
		}
	}
	else
	{
		if (flag == 0x0080)
		{
			CurrColorFill = "Black";
			CurrFillShade = dataF & 0x000000FF;
			CurrColorStroke = "Black";
			CurrStrokeShade = dataS & 0x000000FF;
		}
		else if (flag == 0x0100)
		{
			CurrColorFill = CommonStrings::None;
			CurrFillShade = dataF & 0x000000FF;
			CurrColorStroke = "Black";
			CurrStrokeShade = dataS & 0x000000FF;
		}
		else
		{
			CurrColorFill = "Black";
			CurrFillShade = dataF & 0x000000FF;
			CurrColorStroke = CommonStrings::None;
			CurrStrokeShade = dataS & 0x000000FF;
		}
	}
}
