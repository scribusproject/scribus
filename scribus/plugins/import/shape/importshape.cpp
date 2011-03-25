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
#include "ui/customfdialog.h"
#include "importshape.h"
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
#include "sclimits.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"

extern SCRIBUS_API ScribusQApp * ScQApp;

ShapePlug::ShapePlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

QImage ShapePlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	double b, h;
	parseHeader(fName, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
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
		tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (Elements.count() > 1)
			m_Doc->groupObjectsList(Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		QImage tmpImage;
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

bool ShapePlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
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
	parseHeader(fName, b, h);
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
		if (!(flags & LoadSavePlugin::lfLoadAsPattern))
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

ShapePlug::~ShapePlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

void ShapePlug::parseHeader(QString fName, double &b, double &h)
{
	QFile f(fName);
	if (f.open(QIODevice::ReadOnly))
	{
		double minXCoor = 0.0;
		double minYCoor = 0.0;
		double maxXCoor = 0.0;
		double maxYCoor = 0.0;
		QDomDocument docu("scridoc");
		docu.setContent(&f);
		QDomElement elem = docu.documentElement();
		QDomNodeList list = elem.elementsByTagName("svg:svg");
		if (list.count() == 0)
			return;
		QDomElement svg = list.item(0).toElement();
		QDomNode DOC = svg.firstChild();
		Conversion = 1.0;
		bool firstCheck = true;
		parseGroupProperties(DOC, minXCoor, minYCoor, maxXCoor, maxYCoor, firstCheck);
		b = maxXCoor - minXCoor;
		h = maxYCoor - minYCoor;
		Conversion = 100.0 / qMax(b, h);
		b *= Conversion;
		h *= Conversion;
		f.close();
	}
}

bool ShapePlug::convert(QString fn)
{
	importedColors.clear();
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
		QDomDocument docu("scridoc");
		docu.setContent(&f);
		QDomElement elem = docu.documentElement();
		if (elem.tagName() != "shape")
			return false;
		QDomNodeList list = elem.elementsByTagName("svg:svg");
		if (list.count() == 0)
			return false;
		QDomElement svg = list.item(0).toElement();
		QDomNode DOC = svg.firstChild();
		parseGroup(DOC);
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

void ShapePlug::finishItem(PageItem* ite)
{
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(100);
	ite->setLineShade(100);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	m_Doc->AdjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	Elements.append(ite);
	if (groupStack.count() > 0)
		groupStack.top().append(ite);
}

void ShapePlug::parseGroup(QDomNode &DOC)
{
	QString tmp = "";
	QString FillCol = "White";
	QString StrokeCol = "Black";
	QString defFillCol = "White";
	QString defStrokeCol = "Black";
	QColor stroke = Qt::black;
	QColor fill = Qt::white;
	Qt::PenStyle Dash = Qt::SolidLine;
	Qt::PenCapStyle LineEnd = Qt::FlatCap;
	Qt::PenJoinStyle LineJoin = Qt::MiterJoin;
	int fillStyle = 1;
	double strokewidth = 0.1;
	bool poly = false;
	while(!DOC.isNull())
	{
		double x1, y1, x2, y2;
		StrokeCol = defStrokeCol;
		FillCol = defFillCol;
		stroke = Qt::black;
		fill = Qt::white;
		fillStyle = 1;
		strokewidth = 1.0;
		Dash = Qt::SolidLine;
		LineEnd = Qt::FlatCap;
		LineJoin = Qt::MiterJoin;
		FPointArray PoLine;
		PoLine.resize(0);
		QDomElement pg = DOC.toElement();
		QString STag = pg.tagName();
		QString style = pg.attribute( "style", "" ).simplified();
		if (style.isEmpty())
			style = pg.attribute( "svg:style", "" ).simplified();
		QStringList substyles = style.split(';', QString::SkipEmptyParts);
		for( QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it )
		{
			QStringList substyle = (*it).split(':', QString::SkipEmptyParts);
			QString command(substyle[0].trimmed());
			QString params(substyle[1].trimmed());
			if (command == "fill")
			{
				if (!((params == "foreground") || (params == "background") || (params == "fg") || (params == "bg") || (params == "none") || (params == "default") || (params == "inverse")))
				{
					if (params == "nofill")
						FillCol = CommonStrings::None;
					else
					{
						fill.setNamedColor( params );
						FillCol = "FromDia"+fill.name();
						ScColor tmp;
						tmp.fromQColor(fill);
						tmp.setSpotColor(false);
						tmp.setRegistrationColor(false);
						QString fNam = m_Doc->PageColors.tryAddColor(FillCol, tmp);
						if (fNam == FillCol)
							importedColors.append(FillCol);
						FillCol = fNam;
					}
				}
			}
			else if (command == "stroke")
			{
				if (!((params == "foreground") || (params == "background") || (params == "fg") || (params == "bg") || (params == "none") || (params == "default")) || (params == "inverse"))
				{
					stroke.setNamedColor( params );
					StrokeCol = "FromDia"+stroke.name();
					ScColor tmp;
					tmp.fromQColor(stroke);
					tmp.setSpotColor(false);
					tmp.setRegistrationColor(false);
					QString fNam = m_Doc->PageColors.tryAddColor(StrokeCol, tmp);
					if (fNam == StrokeCol)
						importedColors.append(StrokeCol);
					StrokeCol = fNam;
				}
			}
			else if (command == "stroke-width")
				strokewidth = ScCLocale::toDoubleC(params);
			else if( command == "stroke-linejoin" )
			{
				if( params == "miter" )
					LineJoin = Qt::MiterJoin;
				else if( params == "round" )
					LineJoin = Qt::RoundJoin;
				else if( params == "bevel" )
					LineJoin = Qt::BevelJoin;
			}
			else if( command == "stroke-linecap" )
			{
				if( params == "butt" )
					LineEnd = Qt::FlatCap;
				else if( params == "round" )
					LineEnd = Qt::RoundCap;
				else if( params == "square" )
					LineEnd = Qt::SquareCap;
			}
		}
		if (STag == "svg:line")
		{
			x1 = ScCLocale::toDoubleC(pg.attribute("x1")) * Conversion;
			y1 = ScCLocale::toDoubleC(pg.attribute("y1")) * Conversion;
			x2 = ScCLocale::toDoubleC(pg.attribute("x2")) * Conversion;
			y2 = ScCLocale::toDoubleC(pg.attribute("y2")) * Conversion;
			PoLine.addPoint(x1, y1);
			PoLine.addPoint(x1, y1);
			PoLine.addPoint(x2, y2);
			PoLine.addPoint(x2, y2);
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, strokewidth, CommonStrings::None, StrokeCol, true);
			m_Doc->Items->at(z)->PoLine = PoLine.copy();
			finishItem(m_Doc->Items->at(z));
		}
		else if (STag == "svg:rect")
		{
			x1 = ScCLocale::toDoubleC(pg.attribute("x")) * Conversion;
			y1 = ScCLocale::toDoubleC(pg.attribute("y")) * Conversion;
			x2 = ScCLocale::toDoubleC(pg.attribute("width")) * Conversion;
			y2 = ScCLocale::toDoubleC(pg.attribute("height")) * Conversion;
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + x1, baseY + y1, x2, y2, strokewidth, FillCol, StrokeCol, true);
			m_Doc->Items->at(z)->setLineJoin(LineJoin);
			m_Doc->Items->at(z)->setLineEnd(LineEnd);
			finishItem(m_Doc->Items->at(z));
		}
		else if ((STag == "svg:polygon") || (STag == "svg:polyline"))
		{
			bool bFirst = true;
			double x = 0.0;
			double y = 0.0;
			QString points = pg.attribute( "points" ).simplified().replace(',', " ");
			QStringList pointList = points.split(' ', QString::SkipEmptyParts);
			FirstM = true;
			for( QStringList::Iterator it = pointList.begin(); it != pointList.end(); it++ )
			{
				if( bFirst )
				{
					x = ScCLocale::toDoubleC(*(it++));
					y = ScCLocale::toDoubleC(*it);
					svgMoveTo(x * Conversion, y * Conversion);
					bFirst = false;
					WasM = true;
				}
				else
				{
					x = ScCLocale::toDoubleC(*(it++));
					y = ScCLocale::toDoubleC(*it);
					svgLineTo(&PoLine, x * Conversion, y * Conversion);
				}
			}
			if (STag == "svg:polygon")
				svgClosePath(&PoLine);
			if (PoLine.size() < 4)
			{
				DOC = DOC.nextSibling();
				continue;
			}
			int z;
			if (STag == "svg:polygon")
				z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, strokewidth, FillCol, StrokeCol, true);
			else
				z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, strokewidth, CommonStrings::None, StrokeCol, true);
			m_Doc->Items->at(z)->PoLine = PoLine.copy();
			finishItem(m_Doc->Items->at(z));
		}
		else if ((STag == "svg:circle") || (STag == "svg:ellipse"))
		{
			x1 = ScCLocale::toDoubleC(pg.attribute("r")) * Conversion;
			y1 = ScCLocale::toDoubleC(pg.attribute("r")) * Conversion;
			x2 = ScCLocale::toDoubleC(pg.attribute("cx")) * Conversion - x1;
			y2 = ScCLocale::toDoubleC(pg.attribute("cy")) * Conversion - y1;
			x1 *= 2.0;
			y1 *= 2.0;
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x1, baseY + y1, x2, y2, strokewidth, FillCol, StrokeCol, true);
			m_Doc->Items->at(z)->setLineJoin(LineJoin);
			m_Doc->Items->at(z)->setLineEnd(LineEnd);
			finishItem(m_Doc->Items->at(z));
		}
		else if (STag == "svg:path")
		{
			poly = parseSVG( pg.attribute( "d" ), &PoLine );
			if (PoLine.size() < 4)
			{
				DOC = DOC.nextSibling();
				continue;
			}
			int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, strokewidth, FillCol, StrokeCol, true);
			m_Doc->Items->at(z)->PoLine = PoLine.copy();
			finishItem(m_Doc->Items->at(z));
		}
		else if (STag == "svg:g")
		{
			int z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, baseX, baseX, 1, 1, 0, CommonStrings::None, CommonStrings::None, true);
			PageItem *neu = m_Doc->Items->at(z);
			Elements.append(neu);
			if (groupStack.count() > 0)
				groupStack.top().append(neu);
			QList<PageItem*> gElements;
			groupStack.push(gElements);
			QDomNode child = DOC.firstChild();
			parseGroup(child);
			if (gElements.count() == 0)
			{
				groupStack.pop();
				Elements.removeAll(neu);
				groupStack.top().removeAll(neu);
				Selection tmpSelection(m_Doc, false);
				tmpSelection.addItem(neu);
				m_Doc->itemSelection_DeleteItem(&tmpSelection);
				m_Doc->renumberItemsInListOrder();
			}
			else
			{
				QList<PageItem*> gElem = groupStack.pop();
				double minx =  std::numeric_limits<double>::max();
				double miny =  std::numeric_limits<double>::max();
				double maxx = -std::numeric_limits<double>::max();
				double maxy = -std::numeric_limits<double>::max();
				for (int gr = 0; gr < gElements.count(); ++gr)
				{
					PageItem* currItem = gElem.at(gr);
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
				neu->setXYPos(gx, gy, true);
				neu->setWidthHeight(gw, gh, true);
				neu->SetRectFrame();
				neu->Clip = FlattenPath(neu->PoLine, neu->Segments);
				neu->setItemName( tr("Group%1").arg(m_Doc->GroupCounter));
				neu->AutoName = false;
				neu->gXpos = neu->xPos() - gx;
				neu->gYpos = neu->yPos() - gy;
				neu->groupWidth = gw;
				neu->groupHeight = gh;
				for (int gr = 0; gr < gElem.count(); ++gr)
				{
					PageItem* currItem = gElem.at(gr);
					currItem->gXpos = currItem->xPos() - gx;
					currItem->gYpos = currItem->yPos() - gy;
					currItem->gWidth = gw;
					currItem->gHeight = gh;
					neu->groupItemList.append(currItem);
					m_Doc->Items->removeAll(currItem);
					Elements.removeAll(currItem);
				}
				neu->setRedrawBounding();
				neu->setTextFlowMode(PageItem::TextFlowDisabled);
				m_Doc->GroupCounter++;
				m_Doc->renumberItemsInListOrder();
			}
		}
		DOC = DOC.nextSibling();
	}
}

void ShapePlug::parseGroupProperties(QDomNode &DOC, double &minXCoor, double &minYCoor, double &maxXCoor, double &maxYCoor, bool &firstCheck)
{
	QString FillCol = "White";
	QString StrokeCol = "Black";
	QColor stroke = Qt::black;
	QColor fill = Qt::white;
	while(!DOC.isNull())
	{
		double x1, y1, x2, y2;
		FPointArray PoLine;
		PoLine.resize(0);
		QDomElement pg = DOC.toElement();
		QString STag = pg.tagName();
		if (STag == "svg:line")
		{
			x1 = ScCLocale::toDoubleC(pg.attribute("x1")) * Conversion;
			y1 = ScCLocale::toDoubleC(pg.attribute("y1")) * Conversion;
			x2 = ScCLocale::toDoubleC(pg.attribute("x2")) * Conversion;
			y2 = ScCLocale::toDoubleC(pg.attribute("y2")) * Conversion;
			PoLine.addPoint(x1, y1);
			PoLine.addPoint(x1, y1);
			PoLine.addPoint(x2, y2);
			PoLine.addPoint(x2, y2);
		}
		else if (STag == "svg:rect")
		{
			x1 = ScCLocale::toDoubleC(pg.attribute("x")) * Conversion;
			y1 = ScCLocale::toDoubleC(pg.attribute("y")) * Conversion;
			x2 = ScCLocale::toDoubleC(pg.attribute("width")) * Conversion;
			y2 = ScCLocale::toDoubleC(pg.attribute("height")) * Conversion;
			static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
									1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
									0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x2 * rect[a];
				double ya = y2 * rect[a+1];
				double xb = x2 * rect[a+2];
				double yb = y2 * rect[a+3];
				PoLine.addPoint(x1+xa, y1+ya);
				PoLine.addPoint(x1+xb, y1+yb);
			}
		}
		else if ((STag == "svg:polygon") || (STag == "svg:polyline"))
		{
			bool bFirst = true;
			double x = 0.0;
			double y = 0.0;
			QString points = pg.attribute( "points" ).simplified().replace(',', " ");
			QStringList pointList = points.split(' ', QString::SkipEmptyParts);
			FirstM = true;
			for( QStringList::Iterator it1 = pointList.begin(); it1 != pointList.end(); it1++ )
			{
				if( bFirst )
				{
					x = ScCLocale::toDoubleC(*(it1++));
					y = ScCLocale::toDoubleC(*it1);
					svgMoveTo(x * Conversion, y * Conversion);
					bFirst = false;
					WasM = true;
				}
				else
				{
					x = ScCLocale::toDoubleC(*(it1++));
					y = ScCLocale::toDoubleC(*it1);
					svgLineTo(&PoLine, x * Conversion, y * Conversion);
				}
			}
			if (STag == "svg:polygon")
				svgClosePath(&PoLine);
			if (PoLine.size() < 4)
			{
				DOC = DOC.nextSibling();
				continue;
			}
		}
		else if (STag == "svg:circle")
		{
			x1 = ScCLocale::toDoubleC(pg.attribute("r")) * Conversion;
			y1 = ScCLocale::toDoubleC(pg.attribute("r")) * Conversion;
			x2 = ScCLocale::toDoubleC(pg.attribute("cx")) * Conversion - x1;
			y2 = ScCLocale::toDoubleC(pg.attribute("cy")) * Conversion - y1;
			x1 *= 2.0;
			y1 *= 2.0;
			static double rect[] = {1.0, 0.5, 1.0, 0.77615235,0.5, 1.0, 0.77615235, 1.0,
									0.5, 1.0, 0.22385765, 1.0, 0.0, 0.5, 0.0, 0.77615235,
									0.0, 0.5, 0.0, 0.22385765, 0.5, 0.0, 0.22385765, 0.0,
									0.5, 0.0, 0.77615235, 0.0, 1.0, 0.5, 1.0, 0.22385765};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x1 * rect[a];
				double ya = y1 * rect[a+1];
				double xb = x1 * rect[a+2];
				double yb = y1 * rect[a+3];
				PoLine.addPoint(x2+xa, y2+ya);
				PoLine.addPoint(x2+xb, y2+yb);
			}
		}
		else if (STag == "svg:ellipse")
		{
			x1 = ScCLocale::toDoubleC(pg.attribute("rx")) * Conversion;
			y1 = ScCLocale::toDoubleC(pg.attribute("ry")) * Conversion;
			x2 = ScCLocale::toDoubleC(pg.attribute("cx")) * Conversion - x1;
			y2 = ScCLocale::toDoubleC(pg.attribute("cy")) * Conversion - y1;
			x1 *= 2.0;
			y1 *= 2.0;
			static double rect[] = {1.0, 0.5, 1.0, 0.77615235,0.5, 1.0, 0.77615235, 1.0,
									0.5, 1.0, 0.22385765, 1.0, 0.0, 0.5, 0.0, 0.77615235,
									0.0, 0.5, 0.0, 0.22385765, 0.5, 0.0, 0.22385765, 0.0,
									0.5, 0.0, 0.77615235, 0.0, 1.0, 0.5, 1.0, 0.22385765};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x1 * rect[a];
				double ya = y1 * rect[a+1];
				double xb = x1 * rect[a+2];
				double yb = y1 * rect[a+3];
				PoLine.addPoint(x2+xa, y2+ya);
				PoLine.addPoint(x2+xb, y2+yb);
			}
		}
		else if (STag == "svg:path")
		{
			parseSVG( pg.attribute( "d" ), &PoLine );
			if (PoLine.size() < 4)
			{
				DOC = DOC.nextSibling();
				continue;
			}
		}
		else if (STag == "svg:g")
		{
			QDomNode child = DOC.firstChild();
			parseGroupProperties(child, minXCoor, minYCoor, maxXCoor, maxYCoor, firstCheck);
		}
		if (PoLine.size() < 4)
		{
			DOC = DOC.nextSibling();
			continue;
		}
		FPoint tp2(getMinClipF(&PoLine));
		PoLine.translate(-tp2.x(), -tp2.y());
		FPoint wh(getMaxClipF(&PoLine));
		if (firstCheck)
		{
			minXCoor = tp2.x();
			minYCoor = tp2.y();
			maxXCoor = tp2.x() + wh.x();
			maxYCoor = tp2.y() + wh.y();
			firstCheck = false;
		}
		else
		{
			minXCoor = qMin(minXCoor, tp2.x());
			minYCoor = qMin(minYCoor, tp2.y());
			maxXCoor = qMax(maxXCoor, tp2.x() + wh.x());
			maxYCoor = qMax(maxYCoor, tp2.y() + wh.y());
		}
		DOC = DOC.nextSibling();
	}
}

double ShapePlug::parseUnit(const QString &unit)
{
	QString sCM(unitGetUntranslatedStrFromIndex(SC_CM));
	QString sMM(unitGetUntranslatedStrFromIndex(SC_MM));
	QString sIN(unitGetUntranslatedStrFromIndex(SC_IN));
	QString sPT(unitGetUntranslatedStrFromIndex(SC_PT));
	QString sPX("px");
	
	bool noUnit = false;
	QString unitval(unit);
	if( unit.right( 2 ) == sPT )
		unitval.replace( sPT, "" );
	else if( unit.right( 2 ) == sCM )
		unitval.replace( sCM, "" );
	else if( unit.right( 2 ) == sMM )
		unitval.replace( sMM , "" );
	else if( unit.right( 2 ) == sIN )
		unitval.replace( sIN, "" );
	else if( unit.right( 2 ) == sPX )
		unitval.replace( sPX, "" );
	if (unitval == unit)
		noUnit = true;
	double value = ScCLocale::toDoubleC(unitval);
	if( unit.right( 2 ) == sPT )
		value = value;
	else if( unit.right( 2 ) == sCM )
	{
		value = cm2pts(value);
		Conversion = 1/unitGetRatioFromIndex(SC_CM);
	}
	else if( unit.right( 2 ) == sMM )
	{
		value = mm2pts(value);
		Conversion = 1/unitGetRatioFromIndex(SC_MM);
	}
	else if( unit.right( 2 ) == sIN )
	{
		value = in2pts(value);
		Conversion = 1/unitGetRatioFromIndex(SC_IN);
	}
	else if( unit.right( 2 ) == sPX )
	{
		value = value * 0.8;
		Conversion = 0.8;
	}
	else if(noUnit)
		value = value;
	return value;
}

const char * ShapePlug::getCoord( const char *ptr, double &number )
{
	int integer, exponent;
	double decimal, frac;
	int sign, expsign;

	exponent = 0;
	integer = 0;
	frac = 1.0;
	decimal = 0;
	sign = 1;
	expsign = 1;

	// read the sign
	if(*ptr == '+')
		ptr++;
	else if(*ptr == '-')
	{
		ptr++;
		sign = -1;
	}

	// read the integer part
	while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		integer = (integer * 10) + *(ptr++) - '0';
	if(*ptr == '.') // read the decimals
	{
		ptr++;
		while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
			decimal += (*(ptr++) - '0') * (frac *= 0.1);
	}

	if(*ptr == 'e' || *ptr == 'E') // read the exponent part
	{
		ptr++;

		// read the sign of the exponent
		if(*ptr == '+')
			ptr++;
		else if(*ptr == '-')
		{
			ptr++;
			expsign = -1;
		}

		exponent = 0;
		while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		{
			exponent *= 10;
			exponent += *ptr - '0';
			ptr++;
		}
	}
	number = integer + decimal;
	number *= sign * pow( static_cast<double>(10), static_cast<double>( expsign * exponent ) );
	// skip the following space
	if(*ptr == ' ')
		ptr++;

	return ptr;
}

bool ShapePlug::parseSVG( const QString &s, FPointArray *ite )
{
	QString d = s;
	d = d.replace( QRegExp( "," ), " ");
	bool ret = false;
	if( !d.isEmpty() )
	{
		d = d.simplified();
		QByteArray pathData = d.toLatin1();
		const char *ptr = pathData.constData();
		const char *end = pathData.constData() + pathData.length() + 1;
		double contrlx, contrly, curx, cury, subpathx, subpathy, tox, toy, x1, y1, x2, y2, xc, yc;
		double px1, py1, px2, py2, px3, py3;
		bool relative;
		FirstM = true;
		char command = *(ptr++), lastCommand = ' ';
		subpathx = subpathy = curx = cury = contrlx = contrly = 0.0;
		while( ptr < end )
		{
			if( *ptr == ' ' )
				ptr++;
			relative = false;
			switch( command )
			{
			case 'm':
				relative = true;
			case 'M':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					WasM = true;
					subpathx = curx = relative ? curx + tox : tox;
					subpathy = cury = relative ? cury + toy : toy;
					svgMoveTo(curx, cury );
					break;
				}
			case 'l':
				relative = true;
			case 'L':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'h':
				{
					ptr = getCoord( ptr, tox );
					tox *= Conversion;
					curx = curx + tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'H':
				{
					ptr = getCoord( ptr, tox );
					tox *= Conversion;
					curx = tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'v':
				{
					ptr = getCoord( ptr, toy );
					toy *= Conversion;
					cury = cury + toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'V':
				{
					ptr = getCoord( ptr, toy );
					toy *= Conversion;
					cury = toy;
					svgLineTo(ite,  curx, cury );
					break;
				}
			case 'z':
			case 'Z':
				{
					curx = subpathx;
					cury = subpathy;
					svgClosePath(ite);
					break;
				}
			case 'c':
				relative = true;
			case 'C':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x1 *= Conversion;
					y1 *= Conversion;
					x2 *= Conversion;
					y2 *= Conversion;
					px1 = relative ? curx + x1 : x1;
					py1 = relative ? cury + y1 : y1;
					px2 = relative ? curx + x2 : x2;
					py2 = relative ? cury + y2 : y2;
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x2 : x2;
					contrly = relative ? cury + y2 : y2;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 's':
				relative = true;
			case 'S':
				{
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x2 *= Conversion;
					y2 *= Conversion;
					px1 = 2 * curx - contrlx;
					py1 = 2 * cury - contrly;
					px2 = relative ? curx + x2 : x2;
					py2 = relative ? cury + y2 : y2;
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x2 : x2;
					contrly = relative ? cury + y2 : y2;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 'q':
				relative = true;
			case 'Q':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x1 *= Conversion;
					y1 *= Conversion;
					px1 = relative ? (curx + 2 * (x1 + curx)) * (1.0 / 3.0) : (curx + 2 * x1) * (1.0 / 3.0);
					py1 = relative ? (cury + 2 * (y1 + cury)) * (1.0 / 3.0) : (cury + 2 * y1) * (1.0 / 3.0);
					px2 = relative ? ((curx + tox) + 2 * (x1 + curx)) * (1.0 / 3.0) : (tox + 2 * x1) * (1.0 / 3.0);
					py2 = relative ? ((cury + toy) + 2 * (y1 + cury)) * (1.0 / 3.0) : (toy + 2 * y1) * (1.0 / 3.0);
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x1 : (tox + 2 * x1) * (1.0 / 3.0);
					contrly = relative ? cury + y1 : (toy + 2 * y1) * (1.0 / 3.0);
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 't':
				relative = true;
			case 'T':
				{
					ptr = getCoord(ptr, tox);
					ptr = getCoord(ptr, toy);
					tox *= Conversion;
					toy *= Conversion;
					xc = 2 * curx - contrlx;
					yc = 2 * cury - contrly;
					px1 = relative ? (curx + 2 * xc) * (1.0 / 3.0) : (curx + 2 * xc) * (1.0 / 3.0);
					py1 = relative ? (cury + 2 * yc) * (1.0 / 3.0) : (cury + 2 * yc) * (1.0 / 3.0);
					px2 = relative ? ((curx + tox) + 2 * xc) * (1.0 / 3.0) : (tox + 2 * xc) * (1.0 / 3.0);
					py2 = relative ? ((cury + toy) + 2 * yc) * (1.0 / 3.0) : (toy + 2 * yc) * (1.0 / 3.0);
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = xc;
					contrly = yc;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			}
			lastCommand = command;
			if(*ptr == '+' || *ptr == '-' || (*ptr >= '0' && *ptr <= '9'))
			{
				// there are still coords in this command
				if(command == 'M')
					command = 'L';
				else if(command == 'm')
					command = 'l';
			}
			else
				command = *(ptr++);

			if( lastCommand != 'C' && lastCommand != 'c' &&
			        lastCommand != 'S' && lastCommand != 's' &&
			        lastCommand != 'Q' && lastCommand != 'q' &&
			        lastCommand != 'T' && lastCommand != 't')
			{
				contrlx = curx;
				contrly = cury;
			}
		}
		if ((lastCommand != 'z') && (lastCommand != 'Z'))
			ret = true;
		if (ite->size() > 2)
		{
			if ((ite->point(0).x() == ite->point(ite->size()-2).x()) && (ite->point(0).y() == ite->point(ite->size()-2).y()))
				ret = false;
		}
	}
	return ret;
}

void ShapePlug::svgMoveTo(double x1, double y1)
{
	CurrX = x1;
	CurrY = y1;
	StartX = x1;
	StartY = y1;
	PathLen = 0;
}

void ShapePlug::svgLineTo(FPointArray *i, double x1, double y1)
{
	if ((!FirstM) && (WasM))
	{
		i->setMarker();
		PathLen += 4;
	}
	FirstM = false;
	WasM = false;
	if (i->size() > 3)
	{
		FPoint b1 = i->point(i->size()-4);
		FPoint b2 = i->point(i->size()-3);
		FPoint b3 = i->point(i->size()-2);
		FPoint b4 = i->point(i->size()-1);
		FPoint n1 = FPoint(CurrX, CurrY);
		FPoint n2 = FPoint(x1, y1);
		if ((b1 == n1) && (b2 == n1) && (b3 == n2) && (b4 == n2))
			return;
	}
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x1, y1));
	CurrX = x1;
	CurrY = y1;
	PathLen += 4;
}

void ShapePlug::svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3)
{
	if ((!FirstM) && (WasM))
	{
		i->setMarker();
		PathLen += 4;
	}
	FirstM = false;
	WasM = false;
	if (PathLen > 3)
	{
		FPoint b1 = i->point(i->size()-4);
		FPoint b2 = i->point(i->size()-3);
		FPoint b3 = i->point(i->size()-2);
		FPoint b4 = i->point(i->size()-1);
		FPoint n1 = FPoint(CurrX, CurrY);
		FPoint n2 = FPoint(x1, y1);
		FPoint n3 = FPoint(x3, y3);
		FPoint n4 = FPoint(x2, y2);
		if ((b1 == n1) && (b2 == n2) && (b3 == n3) && (b4 == n4))
			return;
	}
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x3, y3));
	i->addPoint(FPoint(x2, y2));
	CurrX = x3;
	CurrY = y3;
	PathLen += 4;
}

void ShapePlug::svgClosePath(FPointArray *i)
{
	if (PathLen > 2)
	{
		if ((PathLen == 4) || (i->point(i->size()-2).x() != StartX) || (i->point(i->size()-2).y() != StartY))
		{
			i->addPoint(i->point(i->size()-2));
			i->addPoint(i->point(i->size()-3));
			i->addPoint(FPoint(StartX, StartY));
			i->addPoint(FPoint(StartX, StartY));
		}
	}
}
