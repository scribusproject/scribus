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

#include "importsml.h"


#include "commonstrings.h"
#include "loadsaveplugin.h"
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

SmlPlug::SmlPlug(ScribusDoc* doc, int flags)
{
	m_tmpSel=new Selection(this, false);
	m_Doc=doc;
	m_importerFlags = flags;
	m_interactive = (flags & LoadSavePlugin::lfInteractive);
	m_progressDialog = NULL;
}

QImage SmlPlug::readThumbnail(QString fName)
{
	QFileInfo fi = QFileInfo(fName);
	m_baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	double b, h;
	parseHeader(fName, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
		h = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
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
	m_Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	m_Doc->scMW()->setScriptRunning(true);
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fName))
	{
		m_tmpSel->clear();
		QDir::setCurrent(CurDirP);
		if (m_Elements.count() > 1)
			m_Doc->groupObjectsList(m_Elements);
		m_Doc->DoDrawing = true;
		m_Doc->m_Selection->delaySignalsOn();
		QImage tmpImage;
		if (m_Elements.count() > 0)
		{
			for (int dre=0; dre<m_Elements.count(); ++dre)
			{
				m_tmpSel->addItem(m_Elements.at(dre), true);
			}
			m_tmpSel->setGroupRect();
			double xs = m_tmpSel->width();
			double ys = m_tmpSel->height();
			tmpImage = m_Elements.at(0)->DrawObj_toImage(500);
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
	parseHeader(fName, b, h);
	if (b == 0.0)
		b = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	if (h == 0.0)
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
	m_Elements.clear();
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
		if ((m_Elements.count() > 1) && (!(m_importerFlags & LoadSavePlugin::lfCreateDoc)))
			m_Doc->groupObjectsList(m_Elements);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		m_Doc->setLoading(false);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		if ((m_Elements.count() > 0) && (!ret) && (m_interactive))
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
					for (int dre=0; dre<m_Elements.count(); ++dre)
					{
						m_Doc->m_Selection->addItem(m_Elements.at(dre), true);
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
				for (int dre=0; dre<m_Elements.count(); ++dre)
				{
					m_tmpSel->addItem(m_Elements.at(dre), true);
				}
				m_tmpSel->setGroupRect();
				ScElemMimeData* md = ScriXmlDoc::WriteToMimeData(m_Doc, m_tmpSel);
				m_Doc->itemSelection_DeleteItem(m_tmpSel);
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

SmlPlug::~SmlPlug()
{
	if (m_progressDialog)
		delete m_progressDialog;
	delete m_tmpSel;
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
	m_CurrColorFill = "White";
	m_CurrFillShade = 100.0;
	m_CurrColorStroke = "Black";
	m_CurrStrokeShade = 100.0;
	m_LineW = 1.0;
	m_Dash = Qt::SolidLine;
	m_LineEnd = Qt::FlatCap;
	m_LineJoin = Qt::MiterJoin;
	m_fillStyle = 1;
	m_Coords.resize(0);
	m_Coords.svgInit();
	m_importedColors.clear();
	QList<PageItem*> gElements;
	m_groupStack.push(gElements);
	m_currentItemNr = 0;
	if(m_progressDialog)
	{
		m_progressDialog->setOverallProgress(2);
		m_progressDialog->setLabel("GI", tr("Generating Items"));
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
		if (m_Elements.count() == 0)
		{
			if (m_importedColors.count() != 0)
			{
				for (int cd = 0; cd < m_importedColors.count(); cd++)
				{
					m_Doc->PageColors.remove(m_importedColors[cd]);
				}
			}
		}
		f.close();
	}
	if (m_progressDialog)
		m_progressDialog->close();
	return true;
}

void SmlPlug::finishItem(QDomElement &e, PageItem* ite)
{
	ite->ClipEdited = true;
	ite->FrameType = 3;
	ite->setFillShade(m_CurrFillShade);
	ite->setLineShade(m_CurrStrokeShade);
	ite->setLineJoin(m_LineJoin);
	ite->setLineEnd(m_LineEnd);
	ite->setTextFlowMode(PageItem::TextFlowDisabled);
	ite->PoLine.translate(m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset());
	m_Doc->AdjustItemSize(ite);
	ite->OldB2 = ite->width();
	ite->OldH2 = ite->height();
	ite->updateClip();
	ite->setItemName(e.attribute("name"));
	ite->AutoName = false;
	m_Elements.append(ite);
	m_Coords.resize(0);
	m_Coords.svgInit();
}

void SmlPlug::processShapeNode(QDomElement &elem)
{
	m_Coords.resize(0);
	m_Coords.svgInit();
	m_currx = 0.0;
	m_curry = 0.0;
	m_startx = 0.0;
	m_starty = 0.0;
	m_count = 0;
	m_first = true;
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
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, m_baseX + x, m_baseY + y, w, h, m_LineW, m_CurrColorFill, m_CurrColorStroke, true);
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if (typ == "RoundRectangle")
	{
		double x = ScCLocale::toDoubleC(elem.attribute("x"));
		double y = ScCLocale::toDoubleC(elem.attribute("y"));
		double w = ScCLocale::toDoubleC(elem.attribute("w"));
		double h = ScCLocale::toDoubleC(elem.attribute("h"));
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, m_baseX + x, m_baseY + y, w, h, m_LineW, m_CurrColorFill, m_CurrColorStroke, true);
		m_Doc->Items->at(z)->setCornerRadius(qMax(ScCLocale::toDoubleC(elem.attribute("r1")), ScCLocale::toDoubleC(elem.attribute("r2"))));
		m_Doc->Items->at(z)->SetFrameRound();
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if (typ == "Ellipse")
	{
		double x = ScCLocale::toDoubleC(elem.attribute("x"));
		double y = ScCLocale::toDoubleC(elem.attribute("y"));
		double w = ScCLocale::toDoubleC(elem.attribute("w"));
		double h = ScCLocale::toDoubleC(elem.attribute("h"));
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, m_baseX + x, m_baseY + y, w, h, m_LineW, m_CurrColorFill, m_CurrColorStroke, true);
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if ((typ == "Polygon") || (typ == "ClosedPath"))
	{
		int z;
		FPoint s = m_Coords.point(0);
		FPoint e = m_Coords.point(m_Coords.count() - 1);
		if (s == e)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, m_CurrColorFill, m_CurrColorStroke, true);
		else
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, m_CurrColorFill, m_CurrColorStroke, true);
		m_Doc->Items->at(z)->PoLine = m_Coords.copy();
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if ((typ == "Bezier") || (typ == "OpenPath") || (typ == "LineArray") || (typ == "Polyline"))
	{
		int	z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, m_CurrColorFill, m_CurrColorStroke, true);
		m_Doc->Items->at(z)->PoLine = m_Coords.copy();
		finishItem(elem, m_Doc->Items->at(z));
	}
	if (typ == "TextBox")
	{
		double x = ScCLocale::toDoubleC(elem.attribute("x"));
		double y = ScCLocale::toDoubleC(elem.attribute("y"));
		double w = ScCLocale::toDoubleC(elem.attribute("w"));
		double h = ScCLocale::toDoubleC(elem.attribute("h"));
		int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, m_baseX + x, m_baseY + y, w, h, 0, CommonStrings::None, CommonStrings::None, true);
		finishItem(elem, m_Doc->Items->at(z));
	}
	else if (typ == "Line")
	{
		double x = ScCLocale::toDoubleC(elem.attribute("x1"));
		double y = ScCLocale::toDoubleC(elem.attribute("y1"));
		double x1 = ScCLocale::toDoubleC(elem.attribute("x2"));
		double y1 = ScCLocale::toDoubleC(elem.attribute("y2"));
		m_Coords.addPoint(x, y);
		m_Coords.addPoint(x, y);
		m_Coords.addPoint(x1, y1);
		m_Coords.addPoint(x1, y1);
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_baseX, m_baseY, 10, 10, m_LineW, CommonStrings::None, m_CurrColorStroke, true);
		m_Doc->Items->at(z)->PoLine = m_Coords.copy();
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
		m_importedColors.append(newColorName);
	return fNam;
}

void SmlPlug::processStrokeNode(QDomElement &elem)
{
	m_CurrColorStroke = processColor(elem);
	m_LineW = ScCLocale::toDoubleC(elem.attribute("width"), 1.0);
	m_LineJoin = Qt::PenJoinStyle(elem.attribute("joinStyle", "0").toInt());
	m_Dash = Qt::PenStyle(elem.attribute("pattern", "1").toInt());
	m_LineEnd = Qt::PenCapStyle(elem.attribute("capStyle", "0").toInt());
}

void SmlPlug::processFillNode(QDomElement &elem)
{
	m_fillStyle = elem.attribute("colorStyle","1").toInt();
	if (m_fillStyle == 0)
		m_CurrColorFill = CommonStrings::None;
	else
		m_CurrColorFill = processColor(elem);
}

void SmlPlug::processLineNode(QDomElement &elem)
{
	double x = ScCLocale::toDoubleC(elem.attribute("x1"));
	double y = ScCLocale::toDoubleC(elem.attribute("y1"));
	double x1 = ScCLocale::toDoubleC(elem.attribute("x2"));
	double y1 = ScCLocale::toDoubleC(elem.attribute("y2"));
	if (!m_first)
		m_Coords.setMarker();
	m_Coords.addPoint(x, y);
	m_Coords.addPoint(x, y);
	m_Coords.addPoint(x1, y1);
	m_Coords.addPoint(x1, y1);
	m_first = false;
}

void SmlPlug::processPointNode(QDomElement &elem)
{
	double x = ScCLocale::toDoubleC(elem.attribute("x"));
	double y = ScCLocale::toDoubleC(elem.attribute("y"));
	if (m_first)
	{
		m_currx = x;
		m_curry = y;
		m_startx = x;
		m_starty = y;
		m_first = false;
		if (elem.attribute("type") == "bezier")
			m_count = 0;
		else
			m_count = -1;
	}
	else
	{
		if (elem.attribute("type") != "bezier")
		{
			m_Coords.addPoint(m_currx, m_curry);
			m_Coords.addPoint(m_currx, m_curry);
			m_Coords.addPoint(x, y);
			m_Coords.addPoint(x, y);
			m_currx = x;
			m_curry = y;
		}
		else
		{
			if (m_count == -1)
			{
				if (FPoint(m_currx, m_curry) != FPoint(x, y))
				{
					m_Coords.addPoint(m_currx, m_curry);
					m_Coords.addPoint(m_currx, m_curry);
					m_Coords.addPoint(x, y);
					m_Coords.addPoint(x, y);
				}
				m_currx = x;
				m_curry = y;
				m_count++;
			}
			else if (m_count == 0)
			{
				m_Coords.addPoint(m_currx, m_curry);
				m_Coords.addPoint(x, y);
				m_count++;
			}
			else if (m_count == 1)
			{
				m_currx = x;
				m_curry = y;
				m_count++;
			}
			else if (m_count == 2)
			{
				m_Coords.addPoint(x, y);
				m_Coords.addPoint(m_currx, m_curry);
				m_currx = x;
				m_curry = y;
				m_count = -1;
			}
		}
	}
}
