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
#include "importsml.h"
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

SmlPlug::SmlPlug(ScribusDoc* doc, int flags)
{
	tmpSel=new Selection(this, false);
	m_Doc=doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
}

QImage SmlPlug::readThumbnail(QString fName)
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

bool SmlPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
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

SmlPlug::~SmlPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

void SmlPlug::parseHeader(QString fName, double &b, double &h)
{
	QFile f(fName);
	if (f.open(QIODevice::ReadOnly))
	{
		QDomDocument docu("scridoc");
		docu.setContent(&f);
		QDomElement elem = docu.documentElement();
		QDomNode node = elem.firstChild();
		while(!node.isNull())
		{
			QDomElement pg = node.toElement();
			if (pg.tagName() == "Dimensions")
			{
				b = ScCLocale::toDoubleC(pg.attribute("w"), 50.0);
				h = ScCLocale::toDoubleC(pg.attribute("h"), 50.0);
				break;
			}
			node = node.nextSibling();
		}
		f.close();
	}
}

bool SmlPlug::convert(QString fn)
{
	QString tmp;
	CurrColorFill = "White";
	CurrFillShade = 100.0;
	CurrColorStroke = "Black";
	CurrStrokeShade = 100.0;
	LineW = 1.0;
	Dash = Qt::SolidLine;
	LineEnd = Qt::FlatCap;
	LineJoin = Qt::MiterJoin;
	fillStyle = 1;
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
		QDomDocument docu("scridoc");
		docu.setContent(&f);
		QDomElement elem = docu.documentElement();
		if (elem.tagName() != "KivioShapeStencil")
			return false;
		QDomNode node = elem.firstChild();
		while(!node.isNull())
		{
			QDomElement pg = node.toElement();
			if (pg.tagName() == "KivioShape")
				processShapeNode(pg);
			node = node.nextSibling();
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

void SmlPlug::finishItem(QDomElement &e, PageItem* ite)
{
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(CurrFillShade);
	ite->setLineShade(CurrStrokeShade);
	ite->setLineJoin(LineJoin);
	ite->setLineEnd(LineEnd);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	m_Doc->AdjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	ite->setItemName(e.attribute("name"));
	ite->AutoName = false;
	Elements.append(ite);
	Coords.resize(0);
	Coords.svgInit();
}

void SmlPlug::processShapeNode(QDomElement &elem)
{
	Coords.resize(0);
	Coords.svgInit();
	currx = 0.0;
	curry = 0.0;
	startx = 0.0;
	starty = 0.0;
	count = 0;
	first = true;
	if (elem.hasChildNodes())
	{
		QDomNode node = elem.firstChild();
		while(!node.isNull())
		{
			QDomElement pg = node.toElement();
			if (pg.tagName() == "KivioLineStyle")
				processStrokeNode(pg);
			else if (pg.tagName() == "KivioFillStyle")
				processFillNode(pg);
			else if (pg.tagName() == "KivioPoint")
				processPointNode(pg);
			else if (pg.tagName() == "Line")
				processLineNode(pg);
			node = node.nextSibling();
		}
	}
	QString typ = elem.attribute("type");
	if (typ == "Rectangle")
	{
		double x = ScCLocale::toDoubleC(elem.attribute("x"));
		double y = ScCLocale::toDoubleC(elem.attribute("y"));
		double w = ScCLocale::toDoubleC(elem.attribute("w"));
		double h = ScCLocale::toDoubleC(elem.attribute("h"));
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX + x, baseY + y, w, h, LineW, CurrColorFill, CurrColorStroke, true);
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if (typ == "Ellipse")
	{
		double x = ScCLocale::toDoubleC(elem.attribute("x"));
		double y = ScCLocale::toDoubleC(elem.attribute("y"));
		double w = ScCLocale::toDoubleC(elem.attribute("w"));
		double h = ScCLocale::toDoubleC(elem.attribute("h"));
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX + x, baseY + y, w, h, LineW, CurrColorFill, CurrColorStroke, true);
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if ((typ == "Polygon") || (typ == "ClosedPath"))
	{
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		m_Doc->Items->at(z)->PoLine = Coords.copy();
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if ((typ == "Bezier") || (typ == "OpenPath") || (typ == "LineArray") || (typ == "Polyline"))
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CurrColorFill, CurrColorStroke, true);
		m_Doc->Items->at(z)->PoLine = Coords.copy();
		finishItem(elem, m_Doc->Items->at(z));
	}
	if (typ == "TextBox")
	{
		double x = ScCLocale::toDoubleC(elem.attribute("x"));
		double y = ScCLocale::toDoubleC(elem.attribute("y"));
		double w = ScCLocale::toDoubleC(elem.attribute("w"));
		double h = ScCLocale::toDoubleC(elem.attribute("h"));
		int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, baseX + x, baseY + y, w, h, 0, CommonStrings::None, CommonStrings::None, true);
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if (typ == "Line")
	{
		double x = ScCLocale::toDoubleC(elem.attribute("x1"));
		double y = ScCLocale::toDoubleC(elem.attribute("y1"));
		double x1 = ScCLocale::toDoubleC(elem.attribute("x2"));
		double y1 = ScCLocale::toDoubleC(elem.attribute("y2"));
		Coords.addPoint(x, y);
		Coords.addPoint(x, y);
		Coords.addPoint(x1, y1);
		Coords.addPoint(x1, y1);
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, LineW, CommonStrings::None, CurrColorStroke, true);
		m_Doc->Items->at(z)->PoLine = Coords.copy();
		finishItem(elem, m_Doc->Items->at(z));
	}
}

QString SmlPlug::processColor(QDomElement &elem)
{
	QString colnam = elem.attribute("color","#ffffff");
	QColor stroke;
	stroke.setNamedColor("#"+colnam.right(6));
	ScColor tmp;
	tmp.fromQColor(stroke);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString newColorName = "FromSML"+tmp.name();
	QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
	if (fNam == newColorName)
		importedColors.append(newColorName);
	return fNam;
}

void SmlPlug::processStrokeNode(QDomElement &elem)
{
	CurrColorStroke = processColor(elem);
	LineW = ScCLocale::toDoubleC(elem.attribute("width"), 1.0);
	LineJoin = Qt::PenJoinStyle(elem.attribute("joinStyle", "0").toInt());
	Dash = Qt::PenStyle(elem.attribute("pattern", "1").toInt());
	LineEnd = Qt::PenCapStyle(elem.attribute("capStyle", "0").toInt());
}

void SmlPlug::processFillNode(QDomElement &elem)
{
	fillStyle = elem.attribute("colorStyle","1").toInt();
	if (fillStyle == 0)
		CurrColorFill = CommonStrings::None;
	else
		CurrColorFill = processColor(elem);
}

void SmlPlug::processLineNode(QDomElement &elem)
{
	double x = ScCLocale::toDoubleC(elem.attribute("x1"));
	double y = ScCLocale::toDoubleC(elem.attribute("y1"));
	double x1 = ScCLocale::toDoubleC(elem.attribute("x2"));
	double y1 = ScCLocale::toDoubleC(elem.attribute("y2"));
	if (!first)
		Coords.setMarker();
	Coords.addPoint(x, y);
	Coords.addPoint(x, y);
	Coords.addPoint(x1, y1);
	Coords.addPoint(x1, y1);
	first = false;
}

void SmlPlug::processPointNode(QDomElement &elem)
{
	double x = ScCLocale::toDoubleC(elem.attribute("x"));
	double y = ScCLocale::toDoubleC(elem.attribute("y"));
	if (first)
	{
		currx = x;
		curry = y;
		startx = x;
		starty = y;
		first = false;
		if (elem.attribute("type") == "bezier")
			count = 0;
		else
			count = -1;
	}
	else
	{
		if (elem.attribute("type") != "bezier")
		{
			Coords.addPoint(currx, curry);
			Coords.addPoint(currx, curry);
			Coords.addPoint(x, y);
			Coords.addPoint(x, y);
			currx = x;
			curry = y;
		}
		else
		{
			if (count == -1)
			{
				if (FPoint(currx, curry) != FPoint(x, y))
				{
					Coords.addPoint(currx, curry);
					Coords.addPoint(currx, curry);
					Coords.addPoint(x, y);
					Coords.addPoint(x, y);
				}
				currx = x;
				curry = y;
				count++;
			}
			else if (count == 0)
			{
				Coords.addPoint(currx, curry);
				Coords.addPoint(x, y);
				count++;
			}
			else if (count == 1)
			{
				currx = x;
				curry = y;
				count++;
			}
			else if (count == 2)
			{
				Coords.addPoint(x, y);
				Coords.addPoint(currx, curry);
				currx = x;
				curry = y;
				count = -1;
			}
		}
	}
}
