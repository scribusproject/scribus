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

#include <cmath>
#include <cstdlib>

#include "commonstrings.h"
#include "customfdialog.h"
#include "importxfig.h"
#include "loadsaveplugin.h"
#include "missing.h"
#include "multiprogressdialog.h"
#include "pagesize.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
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

XfigPlug::XfigPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

bool XfigPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	double x, y, b, h;
	bool ret = false;
	CustColors.clear();
	importedColors.insert(0, "Black");
	importedColors.insert(1, "Blue");
	importedColors.insert(2, "Green");
	importedColors.insert(3, "Cyan");
	importedColors.insert(4, "Red");
	importedColors.insert(5, "Magenta");
	importedColors.insert(6, "Yellow");
	importedColors.insert(7, "White");
	importedColors.insert(8, "Blue4");
	importedColors.insert(9, "Blue3");
	importedColors.insert(10, "Blue2");
	importedColors.insert(11, "LtBlue");
	importedColors.insert(12, "Green4");
	importedColors.insert(13, "Green3");
	importedColors.insert(14, "Green2");
	importedColors.insert(15, "Cyan4");
	importedColors.insert(16, "Cyan3");
	importedColors.insert(17, "Cyan2");
	importedColors.insert(18, "Red4");
	importedColors.insert(19, "Red3");
	importedColors.insert(20, "Red2");
	importedColors.insert(21, "Magenta4");
	importedColors.insert(22, "Magenta3");
	importedColors.insert(23, "Magenta2");
	importedColors.insert(24, "Brown4");
	importedColors.insert(25, "Brown3");
	importedColors.insert(26, "Brown2");
	importedColors.insert(27, "Pink4");
	importedColors.insert(28, "Pink3");
	importedColors.insert(29, "Pink2");
	importedColors.insert(30, "Pink");
	importedColors.insert(31, "Gold");
	CustColors.insert("Blue", ScColor(255, 0, 0, 0));
	CustColors.insert("Blue4", ScColor(0, 0, 144));
	CustColors.insert("Blue3", ScColor(0, 0, 176));
	CustColors.insert("Blue2", ScColor(0, 0, 208));
	CustColors.insert("LtBlue", ScColor(135, 206, 255));
	CustColors.insert("Cyan", ScColor(255, 0, 0, 0));
	CustColors.insert("Cyan4", ScColor(0, 144, 144));
	CustColors.insert("Cyan3", ScColor(0, 176, 176));
	CustColors.insert("Cyan2", ScColor(0, 208, 208));
	CustColors.insert("Green", ScColor(255, 0, 255, 0));
	CustColors.insert("Green4", ScColor(0, 144, 0));
	CustColors.insert("Green3", ScColor(0, 176, 0));
	CustColors.insert("Green2", ScColor(0, 208, 0));
	CustColors.insert("Red", ScColor(0, 255, 255, 0));
	CustColors.insert("Red4", ScColor(144, 0, 0));
	CustColors.insert("Red3", ScColor(176, 0, 0));
	CustColors.insert("Red2", ScColor(208, 0, 0));
	CustColors.insert("Yellow", ScColor(0, 0, 255, 0));
	CustColors.insert("Magenta", ScColor(0, 255, 0, 0));
	CustColors.insert("Magenta4", ScColor(144, 0, 144));
	CustColors.insert("Magenta3", ScColor(176, 0, 176));
	CustColors.insert("Magenta2", ScColor(208, 0, 208));
	CustColors.insert("Brown4", ScColor(128, 48, 0));
	CustColors.insert("Brown3", ScColor(160, 64, 0));
	CustColors.insert("Brown2", ScColor(192, 96, 0));
	CustColors.insert("Pink4", ScColor(255, 128, 128));
	CustColors.insert("Pink3", ScColor(255, 160, 160));
	CustColors.insert("Pink2", ScColor(255, 192, 192));
	CustColors.insert("Pink", ScColor(255, 224, 224));
	CustColors.insert("Gold", ScColor(255, 215, 0));
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
	docX = x;
	docY = y;
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.PageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.PageHeight;
	docWidth = b - x;
	docHeight = h - y;
	baseX = 0;
	baseY = 0;
	if (!interactive || (flags & LoadSavePlugin::lfInsertPage))
	{
		m_Doc->setPage(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->view()->addPage(0, true);
		baseX = 0;
		baseY = 0;
	}
	else
	{
		if (!m_Doc || (flags & LoadSavePlugin::lfCreateDoc))
		{
			m_Doc=ScCore->primaryMainWindow()->doFileNew(b-x, h-y, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
			ScCore->primaryMainWindow()->HaveNewDoc();
			ret = true;
			baseX = 0;
			baseY = 0;
		}
	}
	if (flags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->documentInfo.setAuthor(docCreator);
		m_Doc->documentInfo.setPublisher(docOrganisation);
		m_Doc->documentInfo.setTitle(docTitle);
		m_Doc->documentInfo.setDate(docDate+" "+docTime);
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset();
	}
	if ((ret) || (!interactive))
	{
		if (b-x > h-y)
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
				int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_Doc->toolSettings.dBrush, m_Doc->toolSettings.dPen, true);
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
/*#ifndef Q_WS_MAC*/
// see #2196
				m_Doc->itemSelection_DeleteItem(tmpSel);
/*#else
				qDebug("xfigimport: leaving items on page");
#endif*/
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

XfigPlug::~XfigPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

bool XfigPlug::parseHeader(QString fName, double &x, double &y, double &b, double &h)
{
	QString tmp, BBox, tmp2, FarNam;
	ScColor cc;
	bool found = false;
	QFile f(fName);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ts(&f);
		QString version = readLinefromDataStream(ts);
		if (!version.startsWith("#FIG 3"))
		{
			f.close();
			return false;
		}
		QString orientation = readLinefromDataStream(ts);
		QString justification = readLinefromDataStream(ts);
		QString units = readLinefromDataStream(ts);
		QString papersize = readLinefromDataStream(ts);
		PageSize ps(papersize);
		if (orientation.startsWith("Landscape"))
		{
			h = ps.width();
			b = ps.height();
		}
		else
		{
			b = ps.width();
			h = ps.height();
		}
		f.close();
		found = true;
	}
	return found;
}

void XfigPlug::parseColor(QString data)
{
	if (data.isEmpty())
		return;
	int colorNum, dummy;
	QString colorValues;
	ScColor tmp;
	ScTextStream Code(&data, QIODevice::ReadOnly);
	Code >> dummy >> colorNum >> colorValues;
	tmp.setNamedColor(colorValues);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString namPrefix = "FromXfig%1-"+colorValues;
	CustColors.insert(namPrefix.arg(colorNum), tmp);
	importedColors.insert(colorNum, namPrefix.arg(colorNum));
}

void XfigPlug::useColor(int colorNum, int area_fill, bool forFill)
{
	QString color = CommonStrings::None;
	int shade = 100;
	if (forFill)
	{
		if (area_fill == -1)
			color = CommonStrings::None;
		else
		{
			if ((colorNum == -1) || (colorNum == 0))
			{
				if (area_fill == 0)
				{
					color = "White";
					shade = 100;
				}
				else if (area_fill == 20)
				{
					color = "Black";
					shade = 100;
				}
				else if ((area_fill > 0) && (area_fill < 20))
				{
					color = "Black";
					shade = qRound(100.0 / 19.0 * area_fill);
				}
			}
			else if (colorNum == 7)
			{
				if (area_fill == 0)
				{
					color = "Black";
					shade = 100;
				}
				else if (area_fill == 20)
				{
					color = "White";
					shade = 100;
				}
				else if ((area_fill > 0) && (area_fill < 20))
				{
					color = "Black";
					shade = qRound(100.0 / 19.0 * (20 - area_fill));
				}
			}
			else
			{
				if (importedColors.contains(colorNum))
				{
					color = importedColors[colorNum];
					if (!m_Doc->PageColors.contains(color))
						m_Doc->PageColors.insert(color, CustColors[color]);
					int sat = area_fill - 20;
					if ((sat > 0) && (sat < 20))
						shade = qRound(100.0 / 19.0 * sat);
				}
				else
					color = CommonStrings::None;
			}
		}
		CurrColorFill = color;
		CurrFillShade = shade;
	}
	else
	{
		if (area_fill == -1)
			CurrColorStroke = CommonStrings::None;
		else
		{
			if ((colorNum == -1) || (colorNum == 0))
				CurrColorStroke = "Black";
			else
			{
				if (importedColors.contains(colorNum))
				{
					CurrColorStroke = importedColors[colorNum];
					if (!m_Doc->PageColors.contains(CurrColorStroke))
						m_Doc->PageColors.insert(CurrColorStroke, CustColors[CurrColorStroke]);
				}
				else
					CurrColorStroke = CommonStrings::None;
			}
		}
	}
}

QVector<double> XfigPlug::getDashValues(double linewidth, int code)
{
	QVector<double> tmp;
	if (code == 1)
	{
		tmp << qMax(4.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
	}
	else if (code == 2)
	{
		tmp << qMax(1.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
	}
	else if (code == 3)
	{
		tmp << qMax(4.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
		tmp << qMax(1.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
	}
	else if (code == 4)
	{
		tmp << qMax(4.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
		tmp << qMax(1.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
		tmp << qMax(1.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
	}
	else if (code == 5)
	{
		tmp << qMax(4.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
		tmp << qMax(1.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
		tmp << qMax(1.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
		tmp << qMax(1.0 * linewidth, 0.1);
		tmp << qMax(2.0 * linewidth, 0.1);
	}
	return tmp;
}

void XfigPlug::processArrows(int forward_arrow, QString fArrowData, int backward_arrow, QString bArrowData, int depth, PageItem *ite)
{
	int		arrow_typeAF;			// (enumeration type)
	int		arrow_styleAF;			// (enumeration type)
	float	arrow_thicknessAF;		// (1/80 inch)
	float	arrow_widthAF;			// (Fig units)
	float	arrow_heightAF;			// (Fig units)
	int		arrow_typeAB;			// (enumeration type)
	int		arrow_styleAB;			// (enumeration type)
	float	arrow_thicknessAB;		// (1/80 inch)
	float	arrow_widthAB;			// (Fig units)
	float	arrow_heightAB;			// (Fig units)
	FPointArray arrow;
	int z = -1;
	PageItem::ItemType iteType;
	if (forward_arrow == 1)
	{
		arrow.resize(0);
		ScTextStream CodeAF(&fArrowData, QIODevice::ReadOnly);
		CodeAF >> arrow_typeAF >> arrow_styleAF >> arrow_thicknessAF >> arrow_widthAF >> arrow_heightAF;
		arrow_widthAF = fig2Pts(arrow_widthAF);
		arrow_heightAF = fig2Pts(arrow_heightAF);
		arrow_thicknessAF = arrow_thicknessAF / 80.0 * 72.0;
		FPoint End = ite->PoLine.point(ite->PoLine.size()-2);
		for (uint xx = ite->PoLine.size()-1; xx > 0; xx -= 2)
		{
			FPoint Vector = ite->PoLine.point(xx);
			if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
			{
				double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
				QMatrix arrowTrans;
				if (arrow_typeAF == 0)
					arrow.parseSVG("M -1, -0.5 L 0, 0 L -1, 0.5");
				else if (arrow_typeAF == 1)
					arrow.parseSVG("M -1, -0.5 L 0, 0 L -1, 0.5 z");
				else if (arrow_typeAF == 2)
					arrow.parseSVG("M -1, -0.5 L 0, 0 L -1, 0.5 L -0.7 0 z");
				else if (arrow_typeAF == 3)
					arrow.parseSVG("M -0.7, -0.5 L 0, 0 L -0.7, 0.5 L -1 0 z");
				arrowTrans.translate(End.x(), End.y());
				arrowTrans.rotate(r);
				arrowTrans.scale(arrow_heightAF, arrow_widthAF);
				arrow.map(arrowTrans);
				break;
			}
		}
		QString fillC = "White";
		if (arrow_styleAF == 1)
			fillC = CurrColorStroke;
		if (arrow_typeAF == 0)
		{
			fillC = CommonStrings::None;
			iteType = PageItem::PolyLine;
		}
		else
			iteType = PageItem::Polygon;
		z = m_Doc->itemAdd(iteType, PageItem::Unspecified, ite->xPos(), ite->yPos(), 10, 10, arrow_thicknessAF, fillC, CurrColorStroke, true);
		if (z >= 0)
		{
			PageItem *item = m_Doc->Items->at(z);
			item->PoLine = arrow.copy();
			item->ClipEdited = true;
			item->FrameType = 3;
			item->setFillShade(CurrFillShade);
			item->setLineShade(CurrStrokeShade);
			FPoint wh = getMaxClipF(&item->PoLine);
			item->setWidthHeight(wh.x(),wh.y());
			item->setTextFlowMode(PageItem::TextFlowDisabled);
			m_Doc->AdjustItemSize(item);
			item->setWidthHeight(qMax(item->width(), 1.0), qMax(item->height(), 1.0));
			depthMap.insert(999 - depth, currentItemNr);
			currentItemNr++;
		}
	}
	if (backward_arrow == 1)
	{
		arrow.resize(0);
		ScTextStream CodeAB(&bArrowData, QIODevice::ReadOnly);
		CodeAB >> arrow_typeAB >> arrow_styleAB >> arrow_thicknessAB >> arrow_widthAB >> arrow_heightAB;
		arrow_widthAB = fig2Pts(arrow_widthAB);
		arrow_heightAB = fig2Pts(arrow_heightAB);
		arrow_thicknessAB = arrow_thicknessAB / 80.0 * 72.0;
		FPointArray arrow;
		FPoint Start = ite->PoLine.point(0);
		for (uint xx = 1; xx < ite->PoLine.size(); xx += 2)
		{
			FPoint Vector = ite->PoLine.point(xx);
			if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
			{
				double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
				QMatrix arrowTrans;
				if (arrow_typeAB == 0)
					arrow.parseSVG("M -1, -0.5 L 0, 0 L -1, 0.5");
				else if (arrow_typeAB == 1)
					arrow.parseSVG("M -1, -0.5 L 0, 0 L -1, 0.5 z");
				else if (arrow_typeAB == 2)
					arrow.parseSVG("M -1, -0.5 L 0, 0 L -1, 0.5 L -0.7 0 z");
				else if (arrow_typeAB == 3)
					arrow.parseSVG("M -0.7, -0.5 L 0, 0 L -0.7, 0.5 L -1 0 z");
				arrowTrans.translate(Start.x(), Start.y());
				arrowTrans.rotate(r);
				arrowTrans.scale(arrow_heightAB, arrow_widthAB);
				arrow.map(arrowTrans);
				break;
			}
		}
		QString fillC = "White";
		if (arrow_styleAB == 1)
			fillC = CurrColorStroke;
		if (arrow_typeAB == 0)
		{
			fillC = CommonStrings::None;
			iteType = PageItem::PolyLine;
		}
		else
			iteType = PageItem::Polygon;
		z = m_Doc->itemAdd(iteType, PageItem::Unspecified, ite->xPos(), ite->yPos(), 10, 10, arrow_thicknessAB, fillC, CurrColorStroke, true);
		if (z >= 0)
		{
			PageItem *item = m_Doc->Items->at(z);
			item->PoLine = arrow.copy();
			item->ClipEdited = true;
			item->FrameType = 3;
			item->setFillShade(CurrFillShade);
			item->setLineShade(CurrStrokeShade);
			FPoint wh = getMaxClipF(&item->PoLine);
			item->setWidthHeight(wh.x(),wh.y());
			item->setTextFlowMode(PageItem::TextFlowDisabled);
			m_Doc->AdjustItemSize(item);
			item->setWidthHeight(qMax(item->width(), 1.0), qMax(item->height(), 1.0));
			depthMap.insert(999 - depth, currentItemNr);
			currentItemNr++;
		}
	}
}

void XfigPlug::processPolyline(QDataStream &ts, QString data)
{
	QString tmp = data;
	QString fArrowData = "";
	QString bArrowData = "";
	int		command;
	int		subtype;				// (1: polyline, 2: box, 3: polygon, 4: arc-box, 5: imported-picture bounding-box)
	int		line_style;				// (enumeration type)
	int		thickness;				// (1/80 inch)
	int		pen_color;				// (enumeration type, pen color)
	int		fill_color;				// (enumeration type, fill color)
	int		depth;					// (enumeration type)
	int		pen_style;				// (pen style, not used)
	int		area_fill;				// (enumeration type, -1 = no fill)
	double	style_val;				// (1/80 inch)
	int		join_style; 			// (enumeration type)
	int		cap_style;				// (enumeration type, only used for POLYLINE)
	int		radius;					// (1/80 inch, radius of arc-boxes)
	int		forward_arrow;			// (0: off, 1: on)
	int		backward_arrow;			// (0: off, 1: on)
	int		npoints;				// (number of points in line)
	int pointsRead = 0;
	int imgFlipped;
	QString imgFile;
	double x, y;
	Coords.resize(0);
	Coords.svgInit();
	bool first = true;
	ScTextStream Code(&tmp, QIODevice::ReadOnly);
	Code >> command >> subtype >> line_style >> thickness >> pen_color >> fill_color >> depth >> pen_style;
	Code >> area_fill >> style_val >> join_style >> cap_style >> radius >> forward_arrow >> backward_arrow >> npoints;
	if (forward_arrow == 1)
		fArrowData = readLinefromDataStream(ts);
	if (backward_arrow == 1)
		bArrowData = readLinefromDataStream(ts);
	if (subtype == 5)
	{
		tmp = readLinefromDataStream(ts);
		ScTextStream imf(&tmp, QIODevice::ReadOnly);
		imf >> imgFlipped >> imgFile;
	}
	while (!ts.atEnd())
	{
		tmp = readLinefromDataStream(ts);
		ScTextStream pts(&tmp, QIODevice::ReadOnly);
		while (!pts.atEnd())
		{
			pts >> x >> y;
			x = fig2Pts(x) - docX;
			y = fig2Pts(y) - docY;
			if (first)
			{
				Coords.svgMoveTo(x, y);
				first = false;
			}
			else
				Coords.svgLineTo(x, y);
			pointsRead++;
		}
		if (pointsRead == npoints)
		{
			if (npoints == 1)
				Coords.svgLineTo(x, y);
			break;
		}
	}
	useColor(pen_color, 0, false);
	useColor(fill_color, area_fill, true);
	LineW = thickness / 80.0 * 72.0;
	int z = -1;
	PageItem *ite;
	if (subtype == 1)
		z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
	else if ((subtype == 2) || (subtype == 3) || (subtype == 4))
	{
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		Coords.svgClosePath();
	}
	else if (subtype == 5)
	{
		z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		Coords.svgClosePath();
	}
	else
		z = -1;
	if (z >= 0)
	{
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillShade(CurrFillShade);
		ite->setLineShade(CurrStrokeShade);
		if (line_style > 0)
			ite->setDashes(getDashValues(LineW, line_style));
		if (subtype == 1)
		{
			if (join_style == 0)
				ite->setLineJoin(Qt::MiterJoin);
			else if (join_style == 1)
				ite->setLineJoin(Qt::RoundJoin);
			else if (join_style == 2)
				ite->setLineJoin(Qt::BevelJoin);
			if (cap_style == 0)
				ite->setLineEnd(Qt::FlatCap);
			else if (cap_style == 1)
				ite->setLineEnd(Qt::RoundCap);
			else if (cap_style == 2)
				ite->setLineEnd(Qt::SquareCap);
		}
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->AdjustItemSize(ite);
		ite->setWidthHeight(qMax(ite->width(), 1.0), qMax(ite->height(), 1.0));
		if (subtype == 4)
		{
			ite->setCornerRadius(radius / 80.0 * 72.0);
			ite->FrameType = 2;
			ite->ClipEdited = false;
			ite->updateClip();
		}
		if (subtype == 5)
		{
			ite->Pfile = baseFile + "/" + imgFile;
			m_Doc->loadPict(ite->Pfile, ite, false);
			ite->setImageXYScale(72.0 / 80.0, 72.0 / 80.0);
			ite->setImageXYOffset(0, 0);
		}
		depthMap.insert(999 - depth, currentItemNr);
		currentItemNr++;
		if ((ite->itemType() == PageItem::PolyLine) && ((forward_arrow == 1) || (backward_arrow == 1)))
			processArrows(forward_arrow, fArrowData, backward_arrow, bArrowData, depth, ite);
	}
}

void XfigPlug::processSpline(QDataStream &ts, QString data)
{
	QString tmp = data;
	QString fArrowData = "";
	QString bArrowData = "";
	int		command;
	int		subtype;				// 0: open approximated spline
									// 1: closed approximated spline
									// 2: open   interpolated spline
									// 3: closed interpolated spline
									// 4: open   x-spline 
									// 5: closed x-spline
	int		line_style;				// (enumeration type)
	int		thickness;				// (1/80 inch)
	int		pen_color;				// (enumeration type, pen color)
	int		fill_color;				// (enumeration type, fill color)
	int		depth;					// (enumeration type)
	int		pen_style;				// (pen style, not used)
	int		area_fill;				// (enumeration type, -1 = no fill)
	double	style_val;				// (1/80 inch)
	int		cap_style;				// (enumeration type, only used for POLYLINE)
	int		forward_arrow;			// (0: off, 1: on)
	int		backward_arrow;			// (0: off, 1: on)
	int		npoints;				// (number of points in line)
	int pointsRead = 0;
	double x, y;
	Coords.resize(0);
	Coords.svgInit();
	bool first = true;
	ScTextStream Code(&tmp, QIODevice::ReadOnly);
	Code >> command >> subtype >> line_style >> thickness >> pen_color >> fill_color >> depth >> pen_style;
	Code >> area_fill >> style_val >> cap_style >> forward_arrow >> backward_arrow >> npoints;
	if (forward_arrow == 1)
		fArrowData = readLinefromDataStream(ts);
	if (backward_arrow == 1)
		bArrowData = readLinefromDataStream(ts);
	while (!ts.atEnd())
	{
		tmp = readLinefromDataStream(ts);
		ScTextStream pts(&tmp, QIODevice::ReadOnly);
		while (!pts.atEnd())
		{
			pts >> x >> y;
			x = fig2Pts(x) - docX;
			y = fig2Pts(y) - docY;
			if (first)
			{
				Coords.svgMoveTo(x, y);
				first = false;
			}
			else
				Coords.svgLineTo(x, y);
			pointsRead++;
		}
		if (pointsRead == npoints)
		{
			if (npoints == 1)
				Coords.svgLineTo(x, y);
			break;
		}
	}
	pointsRead = 0;
	while (!ts.atEnd())
	{
		tmp = readLinefromDataStream(ts);
		ScTextStream pts(&tmp, QIODevice::ReadOnly);
		while (!pts.atEnd())
		{
			pts >> x;
			pointsRead++;
		}
		if (pointsRead == npoints)
			break;
	}
	useColor(pen_color, 0, false);
	useColor(fill_color, area_fill, true);
	LineW = thickness / 80.0 * 72.0;
	int z = -1;
	PageItem *ite;
	if ((subtype == 0) || (subtype == 2) || (subtype == 4))
		z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
	else if ((subtype == 1) || (subtype == 3) || (subtype == 5))
	{
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		Coords.svgClosePath();
	}
	if (z >= 0)
	{
		ite = m_Doc->Items->at(z);
		ite->PoLine = Coords.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillShade(CurrFillShade);
		ite->setLineShade(CurrStrokeShade);
		if (line_style > 0)
			ite->setDashes(getDashValues(LineW, line_style));
		if ((subtype == 0) || (subtype == 2) || (subtype == 4))
		{
			if (cap_style == 0)
				ite->setLineEnd(Qt::FlatCap);
			else if (cap_style == 1)
				ite->setLineEnd(Qt::RoundCap);
			else if (cap_style == 2)
				ite->setLineEnd(Qt::SquareCap);
		}
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->AdjustItemSize(ite);
		ite->setWidthHeight(qMax(ite->width(), 1.0), qMax(ite->height(), 1.0));
		depthMap.insert(999 - depth, currentItemNr);
		currentItemNr++;
		if ((ite->itemType() == PageItem::PolyLine) && ((forward_arrow == 1) || (backward_arrow == 1)))
			processArrows(forward_arrow, fArrowData, backward_arrow, bArrowData, depth, ite);
	}
}

void XfigPlug::processArc(QDataStream &ts, QString data)
{
	QString tmp = data;
	QString fArrowData = "";
	QString bArrowData = "";
	int		command;
	int		subtype;				// (1: open ended arc, 2: pie-wedge (closed))
	int		line_style;				// (enumeration type)
	int		thickness;				// (1/80 inch)
	int		pen_color;				// (enumeration type, pen color)
	int		fill_color;				// (enumeration type, fill color)
	int		depth;					// (enumeration type)
	int		pen_style;				// (pen style, not used)
	int		area_fill;				// (enumeration type, -1 = no fill)
	double	style_val;				// (1/80 inch)
	int		cap_style;				// (enumeration type, only used for POLYLINE)
	int		direction;				// (always 1)
	int		forward_arrow;			// (0: off, 1: on)
	int		backward_arrow;			// (0: off, 1: on)
	double	center_x, center_y;		// (center of the arc)
	int		x1, y1;					// (Fig units, the 1st point the user entered)
	int		x2, y2;					// (Fig units, the 2nd point)
	int		x3, y3;					// (Fig units, the last point)
	ScTextStream Code(&tmp, QIODevice::ReadOnly);
	Code >> command >> subtype >> line_style >> thickness >> pen_color >> fill_color >> depth >> pen_style;
	Code >> area_fill >> style_val >> cap_style >> direction >> forward_arrow >> backward_arrow;
	Code >> center_x >> center_y >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
	if (forward_arrow == 1)
		fArrowData = readLinefromDataStream(ts);
	if (backward_arrow == 1)
		bArrowData = readLinefromDataStream(ts);
	useColor(pen_color, 0, false);
	useColor(fill_color, area_fill, true);
	LineW = thickness / 80.0 * 72.0;
	int z = -1;
	PageItem *ite;
	if (subtype == 1)
		z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
	else if (subtype == 2)
		z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
	else
		z = -1;
	center_x = fig2Pts(center_x) - docX;
	center_y = fig2Pts(center_y) - docY;
	double x1R = fig2Pts(x1) - docX;
	double y1R = fig2Pts(y1) - docY;
	double x3R = fig2Pts(x3) - docX;
	double y3R = fig2Pts(y3) - docY;
	double r1 = distance(x1R - center_x, y1R - center_y);
	double x0 = center_x - r1;
	double y0 = center_y - r1;
	QRectF bBox = QRectF(x0, y0, r1 * 2.0, r1 * 2.0);
	double angle1 = -xy2Deg(x1R - center_x, y1R - center_y);
	double angle2 = -xy2Deg(x3R - center_x, y3R - center_y);
	double sweep;
	if (direction == 0)
	{
		sweep = angle1 - angle2;
		if (sweep < 0.0)
			sweep = 360.0 + sweep;
	}
	else
	{
		sweep = angle2 - angle1;
		if (sweep < 0.0)
			sweep = 360.0 + sweep;
		double an = angle1;
		angle1 = angle2;
		angle2 = an;
	}
	QPainterPath aPath;
	if (subtype == 1)
	{
		aPath.arcMoveTo(bBox, angle2);
		aPath.arcTo(bBox, angle2, sweep);
	}
	else
	{
		aPath.moveTo(center_x, center_y);
		aPath.arcTo(bBox, angle2, sweep);
		aPath.lineTo(center_x, center_y);
	}
	FPointArray points;
	points.fromQPainterPath(aPath);

	if (z >= 0)
	{
		ite = m_Doc->Items->at(z);
		ite->PoLine = points.copy();
		ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
		ite->ClipEdited = true;
		ite->FrameType = 3;
		ite->setFillShade(CurrFillShade);
		ite->setLineShade(CurrStrokeShade);
		if (line_style > 0)
			ite->setDashes(getDashValues(LineW, line_style));
		if (subtype == 1)
		{
			if (cap_style == 0)
				ite->setLineEnd(Qt::FlatCap);
			else if (cap_style == 1)
				ite->setLineEnd(Qt::RoundCap);
			else if (cap_style == 2)
				ite->setLineEnd(Qt::SquareCap);
		}
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->AdjustItemSize(ite);
		ite->setWidthHeight(qMax(ite->width(), 1.0), qMax(ite->height(), 1.0));
		depthMap.insert(999 - depth, currentItemNr);
		currentItemNr++;
		if ((ite->itemType() == PageItem::PolyLine) && ((forward_arrow == 1) || (backward_arrow == 1)))
		{
			if (direction == 1)
				processArrows(forward_arrow, fArrowData, backward_arrow, bArrowData, depth, ite);
			else
				processArrows(backward_arrow, bArrowData, forward_arrow, fArrowData, depth, ite);
		}
	}
}

void XfigPlug::processEllipse(QString data)
{
	QString tmp = data;
	int		command;			// (always 1)
	int		subtype;			// (1: ellipse defined by radii
								//  2: ellipse defined by diameters
								//  3: circle defined by radius
								//  4: circle defined by diameter)
	int		line_style;			// (enumeration type)
	int		thickness;			// (1/80 inch)
	int		pen_color;			// (enumeration type, pen color)
	int		fill_color;			// (enumeration type, fill color)
	int		depth;				// (enumeration type)
	int		pen_style;			// (pen style, not used)
	int		area_fill;			// (enumeration type, -1 = no fill)
	double	style_val;			// (1/80 inch)
	int		direction;			// (always 1)
	double	angle;				// (radians, the angle of the x-axis)
	int		center_x, center_y;	// (Fig units)
	int		radius_x, radius_y;	// (Fig units)
	int		start_x, start_y;	// (Fig units; the 1st point entered)
	int		end_x, end_y;		// (Fig units; the last point entered)
	double x, y, w, h;
	ScTextStream Code(&tmp, QIODevice::ReadOnly);
	Code >> command >> subtype >> line_style >> thickness >> pen_color >> fill_color >> depth >> pen_style;
	Code >> area_fill >> style_val >> direction >> angle >> center_x >> center_y >> radius_x >> radius_y;
	Code >> start_x >> start_y >> end_x >> end_y;
	useColor(pen_color, 0, false);
	useColor(fill_color, area_fill, true);
	LineW = thickness / 80.0 * 72.0;
	w = fig2Pts(radius_x);
	h = fig2Pts(radius_y);
	x = fig2Pts(center_x) - w;
	y = fig2Pts(center_y) - h;
	w *= 2.0;
	h *= 2.0;
	x -= docX;
	x += m_Doc->currentPage()->xOffset();
	y -= docY;
	y += m_Doc->currentPage()->yOffset();
	int z = -1;
	PageItem *ite;
	z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x, baseY + y, w, h, LineW, CurrColorFill, CurrColorStroke, true);
	if (z >= 0)
	{
		ite = m_Doc->Items->at(z);
		ite->setFillShade(CurrFillShade);
		ite->setLineShade(CurrStrokeShade);
		if (line_style > 0)
			ite->setDashes(getDashValues(LineW, line_style));
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		int rot = m_Doc->RotMode;
		m_Doc->RotMode = 2;
		m_Doc->RotateItem(-angle * 180.0 / M_PI, ite);
		m_Doc->RotMode = rot;
		depthMap.insert(999 - depth, currentItemNr);
		currentItemNr++;
	}
}

QString XfigPlug::cleanText(QString text)
{
	QString ret = "";
	QString tmp = "";
	bool sep = false;
	int sepcount = 0;
	for (int a = 1; a < text.count(); ++a)
	{
		QString ch = text.mid(a,1);
		if (sep)
		{
			if (ch == "\\")
			{
				ret += ch;
				sep = false;
			}
			else
			{
				tmp += ch;
				sepcount++;
				if (sepcount == 3)
				{
					sep = false;
					bool ok = false;
					if (tmp != "001")
					{
						int code = tmp.toInt(&ok, 8);
						if (ok)
							ret += QChar(code);
					}
					tmp = "";
				}
			}
		}
		else
		{
			if (ch == "\\")
			{
				sep = true;
				sepcount = 0;
			}
			else
				ret += ch;
		}
	}
	return ret;
}

void XfigPlug::processText(QString data)
{
	QString tmp = data;
	int		command;			// (always 4)
	int		subtype;			// (0: Left justified
								//  1: Center justified
								//  2: Right justified)
	int		color;				// (enumeration type)
	int		depth;				// (enumeration type)
	int		pen_style;			// (enumeration , not used)
	int		font;				// (enumeration type)
	float	font_size;			// (font size in points)
	float	angle;				// (radians, the angle of the text)
	int		font_flags;			// (bit vector)
	float	height;				// (Fig units)
	float	length;				// (Fig units)
	int		xT, yT;				// (Fig units, coordinate of the origin
								//  of the string.  If sub_type = 0, it is
								//  the lower left corner of the string.
								//  If sub_type = 1, it is the lower
								//  center.  Otherwise it is the lower
								//  right corner of the string.)
	QString text;
	double x, y, w, h;
	ScTextStream Code(&tmp, QIODevice::ReadOnly);
	Code >> command >> subtype >> color >> depth >> pen_style >> font >> font_size >> angle;
	Code >> font_flags >> height >> length >> xT >> yT;
	text = Code.readAll();
	text = cleanText(text);
	FPointArray textPath;
	QPainterPath painterPath;
	QString TFont = m_Doc->toolSettings.defFont;
	QFont::Weight weight = QFont::Normal;
	bool isItalic = false;
	if (font_flags & 4)
	{
		switch (font)
		{
			case 0:
				TFont = "Times Roman";
				weight = QFont::Normal;
				break;
			case 1:
				TFont = "Times Italic";
				weight = QFont::Normal;
				isItalic = true;
				break;
			case 2:
				TFont = "Times Bold";
				weight = QFont::Bold;
				break;
			case 3:
				TFont = "Times Bold Italic";
				weight = QFont::Bold;
				isItalic = true;
				break;
			case 4:
				TFont = "AvantGarde Book";
				weight = QFont::Normal;
				break;
			case 5:
				TFont = "AvantGarde Book Oblique";
				weight = QFont::Normal;
				break;
			case 6:
				TFont = "AvantGarde Demi";
				weight = QFont::DemiBold;
				break;
			case 7:
				TFont = "AvantGarde Demi Oblique";
				weight = QFont::DemiBold;
				break;
			case 8:
				TFont = "Bookman Light";
				weight = QFont::Light;
				break;
			case 9:
				TFont = "Bookman Light Italic";
				weight = QFont::Light;
				isItalic = true;
				break;
			case 10:
				TFont = "Bookman Demi";
				weight = QFont::DemiBold;
				break;
			case 11:
				TFont = "Bookman Demi Italic";
				weight = QFont::DemiBold;
				isItalic = true;
				break;
			case 12:
				TFont = "Courier";
				weight = QFont::Normal;
				break;
			case 13:
				TFont = "Courier Oblique";
				weight = QFont::Normal;
				break;
			case 14:
				TFont = "Courier Bold";
				weight = QFont::Bold;
				break;
			case 15:
				TFont = "Courier Bold Oblique";
				weight = QFont::Bold;
				break;
			case 16:
				TFont = "Helvetica";
				weight = QFont::Normal;
				break;
			case 17:
				TFont = "Helvetica Oblique";
				weight = QFont::Normal;
				break;
			case 18:
				TFont = "Helvetica Bold";
				weight = QFont::Bold;
				break;
			case 19:
				TFont = "Helvetica Bold Oblique";
				weight = QFont::Bold;
				break;
			case 20:
				TFont = "Helvetica Narrow";
				weight = QFont::Normal;
				break;
			case 21:
				TFont = "Helvetica Narrow Oblique";
				weight = QFont::Normal;
				break;
			case 22:
				TFont = "Helvetica Narrow Bold";
				weight = QFont::Bold;
				break;
			case 23:
				TFont = "Helvetica Narrow Bold Oblique";
				weight = QFont::Bold;
				break;
			case 24:
				TFont = "New Century Schoolbook Roman";
				weight = QFont::Normal;
				break;
			case 25:
				TFont = "New Century Schoolbook Italic";
				weight = QFont::Normal;
				isItalic = true;
				break;
			case 26:
				TFont = "New Century Schoolbook Bold";
				weight = QFont::Bold;
				break;
			case 27:
				TFont = "New Century Schoolbook Bold Italic";
				weight = QFont::Bold;
				isItalic = true;
				break;
			case 28:
				TFont = "Palatino Roman";
				weight = QFont::Normal;
				break;
			case 29:
				TFont = "Palatino Italic";
				weight = QFont::Normal;
				isItalic = true;
				break;
			case 30:
				TFont = "Palatino Bold";
				weight = QFont::Bold;
				break;
			case 31:
				TFont = "Palatino Bold Italic";
				weight = QFont::Bold;
				isItalic = true;
				break;
			case 32:
				TFont = "Symbol";
				weight = QFont::Normal;
				break;
			case 33:
				TFont = "Zapf Chancery Medium Italic";
				weight = QFont::Normal;
				isItalic = true;
				break;
			case 34:
				TFont = "Zapf Dingbats";
				weight = QFont::Normal;
				break;
			default:
				TFont = m_Doc->toolSettings.defFont;
				weight = QFont::Normal;
				break;
		}
	}
	else
	{
		switch (font)
		{
    		case 1: // Roman
				TFont = "Times";
				weight = QFont::Normal;
				break;
			case 2: // Bold
				TFont = "Times";
				weight = QFont::Bold;
				break;
			case 3: // Italic
				TFont = "Times";
				weight = QFont::Normal;
				isItalic = true;
				break;
			case 4: // Sans Serif
				TFont = "Helvetica";
				weight = QFont::Normal;
				break;
			case 5: // Typewriter
				TFont = "Courier";
				weight = QFont::Normal;
				break;
			default:
				TFont = m_Doc->toolSettings.defFont;
				weight = QFont::Normal;
				break;
		}
	}
	w = fig2Pts(length);
	h = fig2Pts(height);
	x = fig2Pts(xT);
	y = fig2Pts(yT);
	x -= docX;
	x += m_Doc->currentPage()->xOffset();
	y -= docY;
	y += m_Doc->currentPage()->yOffset();
	QFont tf = QFont(TFont, 10, weight, isItalic);
	tf.setPointSizeF(font_size / 80.0 * 72.0);
	painterPath.addText( 0, 0, tf, text );
	QRectF br = painterPath.boundingRect();
	if ((angle == 0) && (br.width() > w))
	{
		QMatrix m;
		m.scale(w / br.width(), w / br.width());
		painterPath = m.map(painterPath);
	}
	textPath.fromQPainterPath(painterPath);
	useColor(color, 0, false);
	int z = -1;
	PageItem *ite;
	z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX + x, baseY + y, w, h, 0, CurrColorStroke, CommonStrings::None, true);
	if (z >= 0)
	{
		ite = m_Doc->Items->at(z);
		ite->setRotation(-angle * 180.0 / M_PI);
		ite->PoLine = textPath.copy();
		ite->ClipEdited = true;
		ite->FrameType = 3;
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->AdjustItemSize(ite);
		ite->setWidthHeight(qMax(ite->width(), 1.0), qMax(ite->height(), 1.0));
		if (subtype == 1)
			m_Doc->MoveRotated(ite, FPoint(-br.width() / 2.0, 0.0));
		else if (subtype == 2)
			m_Doc->MoveRotated(ite, FPoint(-br.width(), 0.0));
		depthMap.insert(999 - depth, currentItemNr);
		currentItemNr++;
	}
}

void XfigPlug::processData(QDataStream &ts, QString data)
{
	QString tmp = data;
	int command, subtype;
	ScTextStream Code(&tmp, QIODevice::ReadOnly);
	Code >> command >> subtype;
	switch (command)
	{
		case 0:
			parseColor(data);
			break;
		case 1:
			processEllipse(data);
			break;
		case 2:
			processPolyline(ts, data);
			break;
		case 3:
			processSpline(ts, data);
			break;
		case 4:
			processText(data);
			break;
		case 5:
			processArc(ts, data);
			break;
		case 6:
//			qDebug() << "Xfig-Import:: Unsupported Compound object";
			break;
		case -6:
//			qDebug() << "Xfig-Import:: Unsupported End of Compound object";
			break;
	}
}

void XfigPlug::resortItems()
{
	int ac = m_Doc->Items->count();
	QList<PageItem*> itemList;
	for (int as = oldDocItemCount; as < ac; ++as)
	{
		itemList.append(m_Doc->Items->takeAt(oldDocItemCount));
	}
	QList<int> keylist = depthMap.uniqueKeys();
	int keysCount = keylist.count();
	int currentLayer = 0;
	for (int it = 0; it < keysCount; ++it)
	{
		if ((importerFlags & LoadSavePlugin::lfCreateDoc) && (it > 0))
			currentLayer = m_Doc->addLayer(QString("Layer %1").arg(it), true);
		QList<int> elems = depthMap.values(keylist.at(it));
		qSort(elems);
		int itemsCount = elems.count();
		for (int i = 0; i < itemsCount; ++i)
		{
			PageItem* ite = itemList.at(elems.at(i));
			Elements.append(ite);
			m_Doc->Items->append(ite);
			ite->ItemNr = m_Doc->Items->count()-1;
			if ((importerFlags & LoadSavePlugin::lfCreateDoc) && (it > 0))
				ite->LayerNr = currentLayer;
		}
	}
}


double XfigPlug::fig2Pts(double in)
{
	return in / 1200.0 * 72.0;
}

bool XfigPlug::convert(QString fn)
{
	QString tmp;
	CurrColorFill = "White";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	patternMode = false;
	patternX1 = 0.0;
	patternY1 = 0.0;
	patternX2 = 0.0;
	patternY2 = 0.0;
	Coords.resize(0);
	Coords.svgInit();
	clipCoords.resize(0);
	clipCoords.svgInit();
	currentPatternName = "";
	currentPatternX = 0.0;
	currentPatternY = 0.0;
	currentPatternXScale = 1.0;
	currentPatternYScale = 1.0;
	currentPatternRotation = 0.0;
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
		QString version = readLinefromDataStream(ts);
		QString orientation = readLinefromDataStream(ts);
		QString justification = readLinefromDataStream(ts);
		QString units = readLinefromDataStream(ts);
		QString papersize = readLinefromDataStream(ts);
		QString magnification = readLinefromDataStream(ts);
		QString multiple = readLinefromDataStream(ts);
		QString transparent = readLinefromDataStream(ts);
		tmp = readLinefromDataStream(ts);
		if (tmp.startsWith("#"))
			tmp = readLinefromDataStream(ts);
		QString resolution = tmp;
		while (!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp.startsWith("#"))
				continue;
			processData(ts, tmp);
			progressDialog->setProgress("GI", ts.device()->pos());
			qApp->processEvents();
		}
		resortItems();
	}
	if (progressDialog)
		progressDialog->close();
	return true;
}
