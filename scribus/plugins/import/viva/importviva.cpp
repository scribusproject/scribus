/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Sep 29 2013
	copyright            : (C) 2013 by Franz Schmid
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
#include <QUrl>
#include <QDebug>

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include <cstdlib>
#include <climits>
#include <limits>

#include "commonstrings.h"

#include "importviva.h"
#include "loadsaveplugin.h"
#include "pageitem_table.h"
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

#include "ui/customfdialog.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"

extern SCRIBUS_API ScribusQApp * ScQApp;

VivaPlug::VivaPlug(ScribusDoc* doc, int flags)
{
	tmpSel = new Selection(this, false);
	m_Doc = doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = NULL;
}

double VivaPlug::parseUnit(const QString &unit)
{
	bool noUnit = false;
	QString unitval=unit;
	if (unit.right( 2 ) == "pt")
		unitval.replace( "pt", "");
	else if (unit.right( 2 ) == "cm")
		unitval.replace( "cm", "" );
	else if (unit.right( 2 ) == "mm")
		unitval.replace( "mm" , "");
	else if (unit.right( 2 ) == "in")
		unitval.replace( "in", "" );
	else if (unit.right( 2 ) == "px")
		unitval.replace( "px", "" );
	if (unitval == unit)
		noUnit = true;
	double value = ScCLocale::toDoubleC(unitval);
	if (unit.right( 2 ) == "pt")
		value = value;
	else if (unit.right( 2 ) == "cm")
		value = ( value / 2.54 ) * 72;
	else if (unit.right( 2 ) == "mm")
		value = ( value / 25.4 ) * 72;
	else if (unit.right( 2 ) == "in")
		value = value * 72;
	else if (unit.right( 2 ) == "px")
		value = value * 0.8;
	else if (noUnit)
		value = value;
	return value;
}

QImage VivaPlug::readThumbnail(QString fName)
{
	QImage tmp;
	if ( !QFile::exists(fName) )
		return QImage();
	progressDialog = NULL;
	QFileInfo fi = QFileInfo(fName);
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	docWidth = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	docHeight = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
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
	return tmp;
}

bool VivaPlug::readColors(const QString& fNameIn, ColorList & colors)
{
	bool success = false;
	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(1, 1, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
	importedColors.clear();
	QByteArray f;
	loadRawText(fNameIn, f);
	if(designMapDom.setContent(f))
	{
		QDomElement docElem = designMapDom.documentElement();
		for(QDomNode drawPag = docElem.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling() )
		{
			QDomElement dpg = drawPag.toElement();
			if (dpg.tagName() == "vc:colors")
				parseColorsXML(dpg);
		}
	}
	if (importedColors.count() != 0)
	{
		colors = m_Doc->PageColors;
		success = true;
	}
	delete m_Doc;
	return success;
}

bool VivaPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	bool ret = false;
	hasLayers = false;
	firstLayer = true;
	firstPage = true;
	pagecount = 1;
	mpagecount = 0;
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
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	/* Set default Page to size defined in Preferences */
	docWidth = PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth;
	docHeight = PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight;
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
			baseY = m_Doc->currentPage()->yOffset() + m_Doc->currentPage()->height() / 2.0;
		}
	}
	if ((!ret) && (interactive))
	{
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset() + m_Doc->currentPage()->height() / 2.0;
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
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
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
					tmpSel->addItem(Elements.at(dre), true);
				}
				tmpSel->setGroupRect();
				ScElemMimeData* md = ScriXmlDoc::WriteToMimeData(m_Doc, tmpSel);
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
	qApp->restoreOverrideCursor();
	return success;
}

VivaPlug::~VivaPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

bool VivaPlug::convert(QString fn)
{
	Coords.resize(0);
	Coords.svgInit();
	importedColors.clear();
	facingPages = false;
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	bool retVal = true;
	importedColors.clear();
	storyMap.clear();
	QByteArray f;
	loadRawText(fn, f);
	if(designMapDom.setContent(f))
	{
		QDomElement docElem = designMapDom.documentElement();
		for(QDomNode drawPag = docElem.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling() )
		{
			QDomElement dpg = drawPag.toElement();
			if (dpg.tagName() == "vd:settings")
				parseSettingsXML(dpg);
			else if (dpg.tagName() == "vc:colors")
				parseColorsXML(dpg);
			else if (dpg.tagName() == "vs:stylesheets")
				parseStylesheetsXML(dpg);
			else if (dpg.tagName() == "vd:preferences")
				parsePreferencesXML(dpg);
			else if (dpg.tagName() == "vd:layer")
				parseLayerXML(dpg);
			else if (dpg.tagName() == "vd:singleAliasPage")
				parseMasterSpreadXML(dpg);
			else if (dpg.tagName() == "vd:doubleAliasPage")
				parseMasterSpreadXML(dpg);
			else if (dpg.tagName() == "vd:spread")
				parseSpreadXML(dpg);
			else if (dpg.tagName() == "vd:textChains")
				parseTextChainsXML(dpg);
		}
	}
	if (progressDialog)
		progressDialog->close();
	return retVal;
}

void VivaPlug::parseSettingsXML(const QDomElement& grNode)
{
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		topMargin = m_Doc->marginsVal().Top;
		leftMargin = m_Doc->marginsVal().Left;
		rightMargin = m_Doc->marginsVal().Right;
		bottomMargin = m_Doc->marginsVal().Bottom;
		double pgCols = m_Doc->PageSp;
		double pgGap = m_Doc->PageSpa;
		papersize = "Custom";
		QString paperOrien = "portrait";
		for (QDomNode n = grNode.firstChild(); !n.isNull(); n = n.nextSibling() )
		{
			QDomElement e = n.toElement();
			if (e.tagName() == "vd:pageMargins")
			{
				topMargin = parseUnit(e.attribute("vd:top", "0"));
				leftMargin = parseUnit(e.attribute("vd:left", "0"));
				rightMargin = parseUnit(e.attribute("vd:right", "0"));
				bottomMargin = parseUnit(e.attribute("vd:bottom", "0"));
			}
			else if (e.tagName() == "vd:pageColumns")
			{
				pgCols = e.attribute("vd:count", "1").toInt();
				pgGap = parseUnit(e.attribute("vd:distance", "0"));
			}
			else if (e.tagName() == "vd:pageMode")
				facingPages = (e.text() == "doublePage") ? 1 : 0;
			else if (e.tagName() == "vd:pageFormat")
				papersize = e.text();
			else if (e.tagName() == "vd:pageOrientation")
				paperOrien = e.text();
		}
		PageSize ps(papersize);
		if (paperOrien.startsWith("portrait"))
		{
			docWidth = ps.width();
			docHeight = ps.height();
		}
		else
		{
			docHeight = ps.width();
			docWidth = ps.height();
		}
		m_Doc->setPage(docWidth, docHeight, topMargin, leftMargin, rightMargin, bottomMargin, pgCols, pgGap, false, facingPages);
		m_Doc->setPageSize(papersize);
		m_Doc->currentPage()->m_pageSize = papersize;
		m_Doc->currentPage()->setInitialHeight(docHeight);
		m_Doc->currentPage()->setInitialWidth(docWidth);
		m_Doc->currentPage()->setHeight(docHeight);
		m_Doc->currentPage()->setWidth(docWidth);
		m_Doc->currentPage()->initialMargins.Top = topMargin;
		m_Doc->currentPage()->initialMargins.Bottom = bottomMargin;
		m_Doc->currentPage()->initialMargins.Left = leftMargin;
		m_Doc->currentPage()->initialMargins.Right = rightMargin;
		m_Doc->reformPages(true);
	}
}

void VivaPlug::parseColorsXML(const QDomElement& grNode)
{
	for (QDomNode n = grNode.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement e = n.toElement();
		if (e.tagName() == "vc:color")
		{
			QString colorName = e.attribute("vc:name");
			if (e.hasChildNodes())
			{
				bool seenSpot = false;
				bool seenRegC = false;
				ScColor tmp;
				for(QDomNode gr = e.firstChild(); !gr.isNull(); gr = gr.nextSibling() )
				{
					QDomElement grs = gr.toElement();
					if (grs.tagName() == "vc:rgb")
					{
						int r = grs.attribute("vc:red", "0").toInt();
						int g = grs.attribute("vc:green", "0").toInt();
						int b = grs.attribute("vc:blue", "0").toInt();
						tmp.setColorRGB(r, g, b);
						break;
					}
					else if (grs.tagName() == "vc:cmyk")
					{
						int c = grs.attribute("vc:cyan", "0").toInt();
						int m = grs.attribute("vc:magenta", "0").toInt();
						int y = grs.attribute("vc:yellow", "0").toInt();
						int k = grs.attribute("vc:key", "0").toInt();
						tmp.setColor(c, m, y, k);
						break;
					}
					else if (grs.tagName() == "vc:hsv")
					{
						int h = grs.attribute("vc:hue", "0").toInt();
						int s = grs.attribute("vc:saturation", "0").toInt();
						int v = grs.attribute("vc:value", "0").toInt();
						QColor qc;
						qc.setHsv(h, s, v);
						tmp.fromQColor(qc);
						tmp = ScColorEngine::convertToModel(tmp, m_Doc, colorModelRGB);
						break;
					}
					else if (grs.tagName() == "vc:registrationColor")
					{
						seenRegC = true;
						int r = e.attribute("vc:red", "0").toInt();
						int g = e.attribute("vc:green", "0").toInt();
						int b = e.attribute("vc:blue", "0").toInt();
						tmp.setColorRGB(r, g, b);
						break;
					}
					else if (grs.tagName() == "vc:spotColor")
						seenSpot = true;
				}
				tmp.setSpotColor(seenSpot);
				tmp.setRegistrationColor(seenRegC);
				QString fNam = m_Doc->PageColors.tryAddColor(colorName, tmp);
				if (fNam == colorName)
					importedColors.append(fNam);
				colorTranslate.insert(colorName, fNam);
			}
			else
			{
				int r = e.attribute("vc:red", "0").toInt();
				int g = e.attribute("vc:green", "0").toInt();
				int b = e.attribute("vc:blue", "0").toInt();
				ScColor tmp;
				tmp.setColorRGB(r, g, b);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor(colorName, tmp);
				if (fNam == colorName)
					importedColors.append(fNam);
				colorTranslate.insert(colorName, fNam);
			}
		}
		else if (e.tagName() == "vc:gradient")
		{
			QString grName = e.attribute("vc:name");
			QString grSelf = grName;
			int grTyp = 6;
			if (e.attribute("vc:type") == "linear")
				grTyp =6;
			else if (e.attribute("vc:type") == "radial")
				grTyp = 7;
			else if (e.attribute("vc:type") == "rectangle")
				grTyp = 10;
			VGradient currentGradient = VGradient(VGradient::linear);
			currentGradient.clearStops();
			for(QDomNode gr = e.firstChild(); !gr.isNull(); gr = gr.nextSibling() )
			{
				QDomElement grs = gr.toElement();
				if ((grs.tagName() == "vc:firstColor") || (grs.tagName() == "vc:secondColor"))
				{
					QString stopName = grs.attribute("vc:name");
					int tint = 0;
					if (grs.attribute("vc:density") == "transparent")
						tint = 0;
					else
						tint = grs.attribute("vc:density").toInt();
					if (colorTranslate.contains(stopName))
						stopName = colorTranslate[stopName];
					else
						stopName = "Black";
					const ScColor& gradC = m_Doc->PageColors[stopName];
					if (grTyp == 6)
					{
						if (grs.tagName() == "vc:firstColor")
							currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), 0.0, 0.5, 1.0, stopName, tint );
						else
							currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), 1.0, 0.5, 1.0, stopName, tint );
					}
					else
					{
						if (grs.tagName() == "vc:firstColor")
							currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), 1.0, 0.5, 1.0, stopName, tint );
						else
							currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), 0.0, 0.5, 1.0, stopName, tint );
					}
				}
			}
			if (m_Doc->addGradient(grName, currentGradient))
				importedGradients.append(grName);
			gradientTranslate.insert(grSelf, grName);
			gradientTypeMap.insert(grName, grTyp);
		}
	}
	return;
}

void VivaPlug::parsePreferencesXML(const QDomElement& spNode)
{
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		for (QDomNode n = spNode.firstChild(); !n.isNull(); n = n.nextSibling() )
		{
			QDomElement e = n.toElement();
			if (e.tagName() == "vd:text")
			{
				for(QDomNode spo = e.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
				{
					QDomElement eo = spo.toElement();
					if (eo.tagName() == "vd:superscriptVerticalOffset")
						m_Doc->typographicPrefs().valueSuperScript = eo.text().toInt();
					else if (eo.tagName() == "vd:superscriptCharacterHeight")
						m_Doc->typographicPrefs().scalingSuperScript = eo.text().toInt();
					else if (eo.tagName() == "vd:subscriptVerticalOffset")
						m_Doc->typographicPrefs().valueSubScript = eo.text().toInt();
					else if (eo.tagName() == "vd:subscriptCharacterHeight")
						m_Doc->typographicPrefs().scalingSubScript = eo.text().toInt();
					else if (eo.tagName() == "vd:smallCapsCharacterHeight")
						m_Doc->typographicPrefs().valueSmallCaps = eo.text().toInt();
				}
			}
		}
	}
}

void VivaPlug::parseLayerXML(const QDomElement& spNode)
{
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		QString layerName = spNode.attribute("vd:name");
		bool printable = true;
		bool visible = true;
		bool locked = false;
		bool flow = false;
		int rc = 0;
		int gc = 0;
		int bc = 0;
		for (QDomNode n = spNode.firstChild(); !n.isNull(); n = n.nextSibling() )
		{
			QDomElement e = n.toElement();
			if (e.tagName() == "vd:print")
				printable = e.text() == "true";
			if (e.tagName() == "vd:hidden")
				visible = e.text() == "false";
			if (e.tagName() == "vd:locked")
				locked = e.text() == "true";
			if (e.tagName() == "vd:keepRunaround")
				flow = e.text() == "true";
			if (e.tagName() == "vd:color")
			{
				rc = e.attribute("vd:red", "0").toInt();
				gc = e.attribute("vd:green", "0").toInt();
				bc = e.attribute("vd:blue", "0").toInt();
			}
		}
		int currentLayer = m_Doc->activeLayer();
		if (!firstLayer)
			currentLayer = m_Doc->addLayer(layerName);
		else
			m_Doc->changeLayerName(currentLayer, layerName);
		m_Doc->setLayerVisible(currentLayer, visible);
		m_Doc->setLayerLocked(currentLayer, locked);
		m_Doc->setLayerPrintable(currentLayer, printable);
		m_Doc->setLayerFlow(currentLayer, flow);
		m_Doc->setLayerMarker(currentLayer, QColor(rc, gc, bc));
	}
	firstLayer = false;
}

void VivaPlug::parseMasterSpreadXML(const QDomElement& spNode)
{
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		bool firstSpread = true;
		m_Doc->setMasterPageMode(true);
		ScPage *oldCur = m_Doc->currentPage();
		for (QDomNode n = spNode.firstChild(); !n.isNull(); n = n.nextSibling() )
		{
			QString pageNam = spNode.attribute("vd:name");
			QDomElement e = n.toElement();
			if (e.tagName() == "vd:aliasPage")
			{
				if (spNode.tagName() == "vd:doubleAliasPage")
				{
					mspreadTypes.insert(pageNam, 1);
					if (firstSpread)
						pageNam += "_Left";
					else
						pageNam += "_Right";
				}
				else
					mspreadTypes.insert(pageNam, 0);
				ScPage *addedPage = m_Doc->addMasterPage(mpagecount, pageNam);
				m_Doc->setCurrentPage(addedPage);
				addedPage->MPageNam = "";
				m_Doc->view()->addPage(mpagecount, true);
				baseX = addedPage->xOffset();
				baseY = addedPage->yOffset();
				mpagecount++;
				for(QDomNode spo = e.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
				{
					QDomElement eo = spo.toElement();
					if (eo.tagName() == "vo:object")
					{
						PageItem* ite = parseObjectXML(eo);
						if (ite != NULL)
						{
							m_Doc->Items->append(ite);
							Elements.append(ite);
						}
					}
				}
				firstSpread = false;
			}
		}
		m_Doc->setCurrentPage(oldCur);
		m_Doc->setMasterPageMode(false);
	}
}

void VivaPlug::parseSpreadXML(const QDomElement& spNode)
{
	for (QDomNode n = spNode.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement e = n.toElement();
		if (e.tagName() == "vd:page")
		{
			if ((importerFlags & LoadSavePlugin::lfCreateDoc) && (!firstPage))
			{
				m_Doc->addPage(pagecount);
				m_Doc->currentPage()->m_pageSize = papersize;
				m_Doc->currentPage()->setInitialHeight(docHeight);
				m_Doc->currentPage()->setInitialWidth(docWidth);
				m_Doc->currentPage()->setHeight(docHeight);
				m_Doc->currentPage()->setWidth(docWidth);
				m_Doc->currentPage()->initialMargins.Top = topMargin;
				m_Doc->currentPage()->initialMargins.Bottom = bottomMargin;
				m_Doc->currentPage()->initialMargins.Left = leftMargin;
				m_Doc->currentPage()->initialMargins.Right = rightMargin;
				m_Doc->currentPage()->MPageNam = CommonStrings::trMasterPageNormal;
				m_Doc->view()->addPage(pagecount, true);
				pagecount++;
			}
			baseX = m_Doc->currentPage()->xOffset();
			baseY = m_Doc->currentPage()->yOffset();
			for(QDomNode sp = e.firstChild(); !sp.isNull(); sp = sp.nextSibling() )
			{
				QDomElement spe = sp.toElement();
				if (spe.tagName() == "vd:content")
				{
					for(QDomNode spo = spe.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "vo:object")
						{
							PageItem* ite = parseObjectXML(eo);
							if (ite != NULL)
							{
								m_Doc->Items->append(ite);
								Elements.append(ite);
							}
						}
					}
				}
				else if (spe.tagName() == "vd:column")
				{
					if ((importerFlags & LoadSavePlugin::lfCreateDoc) && (firstPage))
					{
						if (spe.text() == "1")
							m_Doc->setPageSetFirstPage(m_Doc->pagePositioning(), 0);
						else
							m_Doc->setPageSetFirstPage(m_Doc->pagePositioning(), 1);
						m_Doc->reformPages(false);
						baseX = m_Doc->currentPage()->xOffset();
						baseY = m_Doc->currentPage()->yOffset();
					}
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
						QString mpage = e.attribute("vd:aliasPageName");
						int mType = mspreadTypes[mpage];
						if (mType == 1)
						{
							if (facingPages == 1)
							{
								if (spe.text() == "1")
									mpage += "_Left";
								else
									mpage += "_Right";
							}
						}
						m_Doc->applyMasterPage(mpage, m_Doc->currentPageNumber());
					}
				}
			}
			firstPage = false;
		}
	}
}

void VivaPlug::parseTextChainsXML(const QDomElement& obNode)
{
	if (storyMap.isEmpty())
		return;
	QDomElement eo = obNode.toElement();
	for(QDomNode ob = eo.firstChild(); !ob.isNull(); ob = ob.nextSibling() )
	{
		QDomElement obe = ob.toElement();
		if (obe.tagName() == "vd:sequence")
		{
			QList<PageItem*> GElements;
			GElements.clear();
			for(QDomNode obg = obe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
			{
				QDomElement eog = obg.toElement();
				if (eog.tagName() == "vd:object")
				{
					QString id = eog.attribute("vd:id");
					if (storyMap.contains(id))
						GElements.append(storyMap[id]);
				}
			}
			if (GElements.count() > 1)
			{
				PageItem *first = GElements[0];
				for (int a = 1; a < GElements.count(); a++)
				{
					PageItem *next = GElements[a];
					first->link(next);
					next->setColumns(first->columns());
					next->setColumnGap(first->columnGap());
					first = next;
				}
			}
		}
	}
}

PageItem* VivaPlug::parseObjectXML(const QDomElement& obNode)
{
	PageItem *retObj = NULL;
	QDomElement eo = obNode.toElement();
	QString id = eo.attribute("vo:id");
	for(QDomNode ob = eo.firstChild(); !ob.isNull(); ob = ob.nextSibling() )
	{
		QDomElement obe = ob.toElement();
		if (obe.tagName() == "vo:groupObject")
		{
			QList<PageItem*> GElements;
			double ob_xpos = 0;
			double ob_ypos = 0;
			for(QDomNode obg = obe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
			{
				QDomElement eog = obg.toElement();
				if (eog.tagName() == "vo:object")
					GElements.append(parseObjectXML(eog));
				else if (eog.tagName() == "vo:transformation")
				{
					for(QDomNode spo = eog.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "vo:translationX")
							ob_xpos = parseUnit(eo.text());
						else if (eo.tagName() == "vo:translationY")
							ob_ypos = parseUnit(eo.text());
					}
				}
			}
			if (GElements.count() > 0)
			{
				double minx =  std::numeric_limits<double>::max();
				double miny =  std::numeric_limits<double>::max();
				double maxx = -std::numeric_limits<double>::max();
				double maxy = -std::numeric_limits<double>::max();
				for (int ep = 0; ep < GElements.count(); ++ep)
				{
					PageItem* currItem = GElements.at(ep);
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
				int z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None, true);
				if (z >= 0)
				{
					retObj = m_Doc->Items->at(z);
					retObj->ClipEdited = true;
					retObj->FrameType = 3;
					retObj->setFillEvenOdd(false);
					retObj->OldB2 = retObj->width();
					retObj->OldH2 = retObj->height();
					retObj->updateClip();
					m_Doc->groupObjectsToItem(retObj, GElements);
					retObj->moveBy(ob_xpos, ob_ypos, true);
					m_Doc->AdjustItemSize(retObj);
					retObj->OwnPage = m_Doc->OnPage(retObj);
					m_Doc->GroupOnPage(retObj);
					m_Doc->Items->removeLast();
				}
			}
		}
		else if (obe.tagName() == "vo:graphicObject")
			retObj = parseObjectDetailsXML(obe, 0);
		else if (obe.tagName() == "vo:pictureObject")
			retObj = parseObjectDetailsXML(obe, 1);
		else if (obe.tagName() == "vo:textObject")
		{
			retObj = parseObjectDetailsXML(obe, 2);
			storyMap.insert(id, retObj);
		}
	}
	return retObj;
}

PageItem* VivaPlug::parseObjectDetailsXML(const QDomElement& obNode, int baseType)
{
	PageItem *retObj = NULL;
	QDomElement eo = obNode.toElement();
	double ob_width = 0;
	double ob_height = 0;
	double ob_xpos = 0;
	double ob_ypos = 0;
	double ob_rotation = 0;
	QString fillColor = CommonStrings::None;
	QString strokeColor = CommonStrings::None;
	QString fillGradient = "";
	int fillGradientTyp = 6;
	int fillTint = 100;
	int strokeTint = 100;
	double lineWidth = 0;
	double fillOpacity = 0.0;
	double strokeOpacity = 0.0;
	double imageOpacity = 0.0;
	double gradientAngle = 0.0;
	double imageXoffs = 0;
	double imageYoffs = 0;
	double imageScaleX = 1.0;
	double imageScaleY = 1.0;
	double cornerRadius = 0.0;
	int ob_type = 0;
	bool printable = false;
	bool locked = false;
	bool resizeable = false;
	FPointArray Path;
	Path.resize(0);
	QString imageFile = "";
	QByteArray imageData;
	imageData.resize(0);
	QVector<double> DashValues;
	DashValues.clear();
	int LineStyle = 0;
	int ArrowLeft = 1;
	int ArrowRight = 1;
	double textMarginLeft = 0.0;
	double textMarginRight = 0.0;
	double textMarginTop = 0.0;
	double textMarginBottom = 0.0;
	double textColumnGap = 0.0;
	int textColumnCount = 1;
	StoryText itemText;
	itemText.clear();
	PageItem::TextFlowMode textFlow = PageItem::TextFlowDisabled;
	for(QDomNode ob = eo.firstChild(); !ob.isNull(); ob = ob.nextSibling() )
	{
		QDomElement obe = ob.toElement();
		if ((obe.tagName() == "vo:rectangle") || (obe.tagName() == "vo:oval") || (obe.tagName() == "vo:polygon") || (obe.tagName() == "vo:line") || (obe.tagName() == "vo:polyline"))
		{
			if (obe.tagName() == "vo:rectangle")
				ob_type = 0;
			if (obe.tagName() == "vo:oval")
				ob_type = 1;
			if (obe.tagName() == "vo:polygon")
				ob_type = 2;
			if (obe.tagName() == "vo:line")
				ob_type = 3;
			if (obe.tagName() == "vo:polyline")
				ob_type = 4;
			for(QDomNode obg = obe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
			{
				QDomElement eog = obg.toElement();
				if (eog.tagName() == "vo:size")
				{
					for(QDomNode spo = eog.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "vo:width")
							ob_width = parseUnit(eo.text());
						else if (eo.tagName() == "vo:height")
							ob_height = parseUnit(eo.text());
					}
				}
				else if (eog.tagName() == "vo:transformation")
				{
					for(QDomNode spo = eog.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "vo:translationX")
							ob_xpos = parseUnit(eo.text());
						else if (eo.tagName() == "vo:translationY")
							ob_ypos = parseUnit(eo.text());
						else if (eo.tagName() == "vo:rotation")
							ob_rotation = eo.text().toDouble();
					}
				}
				else if (eog.tagName() == "vo:design")
				{
					for(QDomNode spo = eog.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "vo:lineColor")
							strokeColor = colorTranslate[eo.text()];
						else if (eo.tagName() == "vo:fillColor")
						{
							if (colorTranslate.contains(eo.text()))
								fillColor = colorTranslate[eo.text()];
							else if (gradientTranslate.contains(eo.text()))
							{
								fillGradient = gradientTranslate[eo.text()];
								fillGradientTyp = gradientTypeMap[fillGradient];
							}
						}
						else if (eo.tagName() == "vo:lineDensity")
						{
							if (eo.text() == "transparent")
								strokeTint = 0;
							else if (eo.text() == "opaque")
								strokeTint = 100;
							else
								strokeTint = eo.text().toInt();
						}
						else if (eo.tagName() == "vo:fillDensity")
						{
							if (eo.text() == "transparent")
								fillTint = 0;
							else if (eo.text() == "opaque")
								fillTint = 100;
							else
								fillTint = eo.text().toInt();
						}
						else if (eo.tagName() == "vo:lineWidth")
							lineWidth = parseUnit(eo.text());
						else if (eo.tagName() == "vo:lineStyle")
						{
							QString Lstyle = eo.text();
							if (Lstyle != "Solid")
							{
								if (Lstyle == "2")
									DashValues << 3 << 2;
								else if (Lstyle == "3")
									LineStyle = 15;
								else if (Lstyle == "4")
									DashValues << 5 << 2.2 << 1 << 2.2;
								else if (Lstyle == "5")
									LineStyle = 6;
								else if (Lstyle == "6")
									DashValues << 2 << 3.1;
								else if ((Lstyle == "7") || (Lstyle == "10"))
									LineStyle = 21;
								else if (Lstyle == "8")
									DashValues << 4 << 3.2 << 0.2 << 3.2;
								else if (Lstyle == "9")
									DashValues << 0.2 << 2;
							}
						}
						else if (eo.tagName() == "vo:startCap")
						{
							ArrowLeft = eo.text().toInt();
							if (ArrowLeft == 2)
								ArrowLeft = 3;
							else if (ArrowLeft == 3)
								ArrowLeft = 18;
							else if (ArrowLeft == 4)
								ArrowLeft = 6;
						}
						else if (eo.tagName() == "vo:endCap")
						{
							ArrowRight = eo.text().toInt();
							if (ArrowRight == 2)
								ArrowRight = 3;
							else if (ArrowRight == 3)
								ArrowRight = 18;
							else if (ArrowRight == 4)
								ArrowRight = 6;
						}
						else if (eo.tagName() == "vo:blendAngle")
							gradientAngle = eo.text().toDouble();
						else if (eo.tagName() == "vo:fillOpacity")
						{
							if (eo.text() == "transparent")
								fillOpacity = 1;
							else if (eo.text() == "opaque")
								fillOpacity = 0;
							else
								fillOpacity = 1.0 - (eo.text().toDouble() / 100.0);
						}
						else if (eo.tagName() == "vo:lineOpacity")
						{
							if (eo.text() == "transparent")
								strokeOpacity = 1;
							else if (eo.text() == "opaque")
								strokeOpacity = 0;
							else
								strokeOpacity = 1.0 - (eo.text().toDouble() / 100.0);
						}
						else if (eo.tagName() == "vo:baseOpacity")
						{
							double opa = 0.0;
							if (eo.text() == "transparent")
								opa = 1;
							else if (eo.text() == "opaque")
								opa = 0;
							else
								opa = 1.0 - (eo.text().toDouble() / 100.0);
							fillOpacity = 1.0 - ((1.0 - fillOpacity) * (1.0 - opa));
							strokeOpacity = 1.0 - ((1.0 - strokeOpacity) * (1.0 - opa));
						}
						else if (eo.tagName() == "vo:cornerRadius")
							cornerRadius = parseUnit(eo.text());
					}
				}
				else if (eog.tagName() == "vo:points")
				{
					Path.resize(0);
					bool hasBefore = false;
					bool hasAfter = false;
					triplePoint triPoint;
					QList<triplePoint> tPoints;
					for(QDomNode spo = eog.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "vo:point")
						{
							FPoint pp = FPoint(parseUnit(eo.attribute("vo:x", "0")), parseUnit(eo.attribute("vo:y", "0")));
							if (eo.hasAttribute("vo:control"))
							{
								if ((eo.attribute("vo:control") == "constraint-before") || (eo.attribute("vo:control") == "before"))
									hasBefore = true;
								else if ((eo.attribute("vo:control") == "constraint-after") || (eo.attribute("vo:control") == "after"))
									hasAfter = true;
							}
							if ((hasBefore) && ((eo.attribute("vo:control") == "constraint-before") || (eo.attribute("vo:control") == "before")))
								triPoint.beforePolyPoint = pp;
							else if (hasAfter && hasBefore)
							{
								triPoint.afterPolyPoint = pp;
								tPoints.append(triPoint);
								hasBefore = false;
								hasAfter = false;
							}
							else if (hasBefore)
								triPoint.PolyPoint = pp;
							else
							{
								triPoint.beforePolyPoint = pp;
								triPoint.PolyPoint = pp;
								triPoint.afterPolyPoint = pp;
								tPoints.append(triPoint);
								hasBefore = false;
								hasAfter = false;
							}
						}
					}
					if (ob_type == 2)
					{
						Path.addPoint(tPoints[0].PolyPoint);
						Path.addPoint(tPoints[0].afterPolyPoint);
						for (int ppx = 1; ppx < tPoints.count(); ppx++)
						{
							Path.addPoint(tPoints[ppx].PolyPoint);
							Path.addPoint(tPoints[ppx].beforePolyPoint);
							Path.addPoint(tPoints[ppx].PolyPoint);
							Path.addPoint(tPoints[ppx].afterPolyPoint);
						}
						Path.addPoint(tPoints[0].PolyPoint);
						Path.addPoint(tPoints[0].beforePolyPoint);
					}
					else if (ob_type == 3)
					{
						Path.addPoint(tPoints[0].PolyPoint);
						Path.addPoint(tPoints[0].PolyPoint);
						Path.addPoint(tPoints[1].PolyPoint);
						Path.addPoint(tPoints[1].PolyPoint);
					}
					else if (ob_type == 4)
					{
						Path.addPoint(tPoints[0].PolyPoint);
						Path.addPoint(tPoints[0].afterPolyPoint);
						for (int ppx = 1; ppx < tPoints.count()-1; ppx++)
						{
							Path.addPoint(tPoints[ppx].PolyPoint);
							Path.addPoint(tPoints[ppx].beforePolyPoint);
							Path.addPoint(tPoints[ppx].PolyPoint);
							Path.addPoint(tPoints[ppx].afterPolyPoint);
						}
						Path.addPoint(tPoints[tPoints.count()-1].PolyPoint);
						Path.addPoint(tPoints[tPoints.count()-1].beforePolyPoint);
					}
				}
				else if (eog.tagName() == "vo:shadow")
				{
				}
			}
		}
		else if (obe.tagName() == "vo:properties")
		{
			for(QDomNode obg = obe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
			{
				QDomElement eog = obg.toElement();
				if (eog.tagName() == "vo:printable")
					printable = eog.text() == "true";
				else if (eog.tagName() == "vo:protected")
					locked = eog.text() == "true";
				else if (eog.tagName() == "vo:fixed")
					resizeable = eog.text() == "true";
			}
		}
		else if (obe.tagName() == "vo:runaround")
		{
			if ((obe.attribute("vo:mode") == "all") || (obe.attribute("vo:mode") == "left") || (obe.attribute("vo:mode") == "right"))
			{
				textFlow = PageItem::TextFlowUsesFrameShape;
				for(QDomNode obg = obe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
				{
					QDomElement eog = obg.toElement();
					if (eog.tagName() == "vo:shape")
					{
						textFlow = PageItem::TextFlowUsesFrameShape;
						break;
					}
					if (eog.tagName() == "vo:block")
					{
						textFlow = PageItem::TextFlowUsesBoundingBox;
						break;
					}
				}
			}
		}
		else if (obe.tagName() == "vo:content")
		{
			for(QDomNode obg = obe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
			{
				QDomElement eog = obg.toElement();
				if (eog.tagName() == "vo:filePath")
				{
					QString fileP = eog.text();
					QFileInfo fp(fileP);
					if (fp.exists())
						imageFile = fileP;
					else
					{
						QString  fileName = fp.fileName();
						QFileInfo fi2(fileName);
						if (fi2.exists())
							imageFile = fileName;
						else
							imageFile = fileP;
					}
				}
				else if (eog.tagName() == "vo:fillOpacity")
				{
					if (eog.text() == "transparent")
						imageOpacity = 1;
					else if (eog.text() == "opaque")
						imageOpacity = 0;
					else
						imageOpacity = 1.0 - (eog.text().toDouble() / 100.0);
				}
				else if (eog.tagName() == "vo:transformation")
				{
					for(QDomNode spo = eog.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "vo:translationX")
							imageXoffs = parseUnit(eo.text());
						else if (eo.tagName() == "vo:translationY")
							imageYoffs = parseUnit(eo.text());
						else if (eo.tagName() == "vo:scaleHorizontal")
							imageScaleX = eo.text().toDouble() / 100.0;
						else if (eo.tagName() == "vo:scaleVertical")
							imageScaleY = eo.text().toDouble() / 100.0;
					}
				}
				else if (eog.tagName() == "vo:preview")
					imageData = QByteArray::fromBase64(eog.text().toLatin1());
				else if (eog.tagName() == "vo:areaStructure")
				{
					for(QDomNode spo = eog.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "uni:indents")
						{
							for(QDomNode stx = eo.firstChild(); !stx.isNull(); stx = stx.nextSibling() )
							{
								QDomElement stxe = stx.toElement();
								if (stxe.tagName() == "uni:left")
									textMarginLeft = parseUnit(stxe.text());
								else if (stxe.tagName() == "uni:right")
									textMarginRight = parseUnit(stxe.text());
								else if (stxe.tagName() == "uni:top")
									textMarginTop = parseUnit(stxe.text());
								else if (stxe.tagName() == "uni:bottom")
									textMarginBottom = parseUnit(stxe.text());
							}
						}
						else if (eo.tagName() == "uni:columns")
						{
							for(QDomNode stx = eo.firstChild(); !stx.isNull(); stx = stx.nextSibling() )
							{
								QDomElement stxe = stx.toElement();
								if (stxe.tagName() == "uni:distance")
									textColumnGap = parseUnit(stxe.text());
								else if (stxe.tagName() == "uni:count")
									textColumnCount = stxe.text().toInt();
							}
						}
					}
				}
				else if (eog.tagName() == "vtr:vivaText")
				{
					itemText.setDoc(m_Doc);
					parseTextXML(eog, itemText, textColumnCount, textColumnGap);
				}
			}
		}
	}
	int z = -1;
	if (baseType == 0)
	{
		if (ob_type == 0)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
		else if (ob_type == 1)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
		else if (ob_type == 2)
			z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
		else if (ob_type == 3)
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
		else if (ob_type == 4)
			z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
	}
	else if (baseType == 1)
	{
		if (ob_type == 0)
			z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
		else if (ob_type == 1)
			z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Ellipse, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
		else if (ob_type == 2)
			z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
	}
	else if (baseType == 2)
	{
		if (ob_type == 0)
			z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
		else if (ob_type == 1)
			z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Ellipse, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
		else if (ob_type == 2)
			z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, baseX, baseY, ob_width, ob_height, lineWidth, fillColor, strokeColor, true);
	}
	if (z >= 0)
	{
		PageItem* item = m_Doc->Items->at(z);
		item->setTextFlowMode(textFlow);
		if ((ob_type == 2) || (ob_type == 3) || (ob_type == 4))
		{
			item->PoLine = Path.copy();
			FPoint wh = getMaxClipF(&item->PoLine);
			item->setWidthHeight(wh.x(),wh.y());
			m_Doc->AdjustItemSize(item, true);
		}
		item->ClipEdited = true;
		if ((cornerRadius != 0) && (item->frameType() == PageItem::Rectangle))
		{
			item->setCornerRadius(cornerRadius);
			item->SetFrameRound();
		}
		else
			item->FrameType = 3;
		if (fillTint == 0)
		{
			item->setFillColor(CommonStrings::None);
			item->setFillShade(100);
		}
		else
		{
			item->setFillColor(fillColor);
			item->setFillShade(fillTint);
		}
		if (strokeTint == 0)
		{
			item->setLineColor(CommonStrings::None);
			item->setLineShade(100);
		}
		else
		{
			item->setLineColor(strokeColor);
			item->setLineShade(strokeTint);
		}
		if (baseType == 1)
			item->setFillTransparency(imageOpacity);
		else
			item->setFillTransparency(fillOpacity);
		item->setLineTransparency(strokeOpacity);
		if (!fillGradient.isEmpty())
		{
			QLineF gradientVectorE;
			gradientVectorE.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorE.setAngle(gradientAngle);
			gradientVectorE.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradEnd = intersectBoundingRect(item, gradientVectorE);
			QLineF gradientVectorS;
			gradientVectorS.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorS.setAngle(gradientAngle + 180);
			gradientVectorS.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradStart = intersectBoundingRect(item, gradientVectorS);
			item->fill_gradient = m_Doc->docGradients[fillGradient];
			if (fillGradientTyp == 6)
			{
				item->setGradientVector(gradStart.x(), gradStart.y(), gradEnd.x(), gradEnd.y(), gradStart.x(), gradStart.y(), 1, 0);
				item->setGradient(fillGradient);
				item->setGradientType(fillGradientTyp);
			}
			else if (fillGradientTyp == 7)
			{
				QList<VColorStop*> colorStops = item->fill_gradient.colorStops();
				QString gradColor2Str = colorStops[0]->name;
				QColor gradColor2 = colorStops[0]->color;
				QString gradColor1Str = colorStops[1]->name;
				QColor gradColor1 = colorStops[1]->color;
				item->meshGradientPatches.clear();
				FPoint center = FPoint(item->width() / 2.0, item->height() / 2.0);
				meshPoint cP;
				cP.resetTo(center);
				cP.transparency = 1.0;
				cP.shade = 100;
				cP.colorName = gradColor2Str;
				cP.color = gradColor2;
				for (int poi = 0; poi < item->PoLine.size()-3; poi += 4)
				{
					meshGradientPatch patch;
					patch.BL = cP;
					patch.BR = cP;
					if (item->PoLine.point(poi).x() > 900000)
						continue;
					meshPoint tL;
					tL.resetTo(item->PoLine.point(poi));
					tL.controlRight = item->PoLine.point(poi + 1);
					tL.transparency = 1.0;
					tL.shade = 100;
					tL.colorName = gradColor1Str;
					tL.color = gradColor1;
					meshPoint tR;
					tR.resetTo(item->PoLine.point(poi + 2));
					tR.controlLeft = item->PoLine.point(poi + 3);
					tR.transparency = 1.0;
					tR.shade = 100;
					tR.colorName = gradColor1Str;
					tR.color = gradColor1;
					patch.TL = tL;
					patch.TR = tR;
					item->meshGradientPatches.append(patch);
				}
				item->GrType = 12;
			}
			else
			{
				item->GrControl1 = FPoint(0, 0);
				item->GrControl2 = FPoint(item->width(), 0);
				item->GrControl3 = FPoint(item->width(), item->height());
				item->GrControl4 = FPoint(0, item->height());
				if (gradientAngle != 0)
				{
					QLineF gradientVector = QLineF(item->width() / 2.0, item->height() / 2.0, 0, 0);
					gradientVector.setLength(gradientVector.length() + 1);
					gradientVector.setAngle(gradientVector.angle() + gradientAngle);
					QPointF g1 = intersectBoundingRect(item, gradientVector);
					item->GrControl1 = FPoint(g1.x(), g1.y());
					gradientVector = QLineF(item->width() / 2.0, item->height() / 2.0, item->width(), 0);
					gradientVector.setLength(gradientVector.length() + 1);
					gradientVector.setAngle(gradientVector.angle() + gradientAngle);
					g1 = intersectBoundingRect(item, gradientVector);
					item->GrControl2 = FPoint(g1.x(), g1.y());
					gradientVector = QLineF(item->width() / 2.0, item->height() / 2.0, item->width(), item->height());
					gradientVector.setLength(gradientVector.length() + 1);
					gradientVector.setAngle(gradientVector.angle() + gradientAngle);
					g1 = intersectBoundingRect(item, gradientVector);
					item->GrControl3 = FPoint(g1.x(), g1.y());
					gradientVector = QLineF(item->width() / 2.0, item->height() / 2.0, 0, item->height());
					gradientVector.setLength(gradientVector.length() + 1);
					gradientVector.setAngle(gradientVector.angle() + gradientAngle);
					g1 = intersectBoundingRect(item, gradientVector);
					item->GrControl4 = FPoint(g1.x(), g1.y());
				}
				item->GrControl5 = FPoint(item->width() / 2.0, item->height() / 2.0);
				item->setGradient(fillGradient);
				item->setGradientType(fillGradientTyp);
			}
		}
		item->setDashOffset(0);
		if (!DashValues.isEmpty())
		{
			QVector<double> pattern(DashValues.count());
			for (int i = 0; i < DashValues.count(); ++i)
			{
				pattern[i] = DashValues[i] * lineWidth;
			}
			item->setDashes(pattern);
		}
		else
			item->setLineStyle(Qt::PenStyle(LineStyle));
		if (ArrowLeft > 1)
			item->setStartArrowIndex(ArrowLeft);
		if (ArrowRight > 1)
			item->setEndArrowIndex(ArrowRight);
		item->setRotation(-ob_rotation, true);
		item->moveBy(ob_xpos, ob_ypos, true);
		item->setFillEvenOdd(false);
		item->OldB2 = item->width();
		item->OldH2 = item->height();
		item->updateClip();
		item->OwnPage = m_Doc->OnPage(item);
		item->ContourLine = item->PoLine.copy();
		if (baseType == 1)
		{
			item->AspectRatio = true;
			item->ScaleType   = false;
			bool erf = m_Doc->loadPict(imageFile, item);
			if (!erf)
			{
				if (!imageData.isEmpty())
				{
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_viva_XXXXXX.png");
					tempFile->setAutoRemove(false);
					if (tempFile->open())
					{
						QString fileName = getLongPathName(tempFile->fileName());
						if (!fileName.isEmpty())
						{
							tempFile->write(imageData);
							tempFile->close();
							item->isInlineImage = true;
							item->isTempFile = true;
							item->AspectRatio = true;
							item->ScaleType   = false;
							m_Doc->loadPict(fileName, item);
						}
					}
					delete tempFile;
				}
			}
			item->setImageXYScale(imageScaleX / item->pixm.imgInfo.xres * 72, imageScaleX / item->pixm.imgInfo.xres * 72);
			item->setImageXYOffset(imageXoffs / item->imageXScale(), imageYoffs / item->imageYScale());
		}
		else if (baseType == 2)
		{
			item->setTextToFrameDist(textMarginLeft, textMarginRight, textMarginTop, textMarginBottom);
			item->setColumns(textColumnCount);
			item->setColumnGap(textColumnGap);
			if (itemText.length() > 0)
				item->itemText.append(itemText);
		}
		item->setLocked(locked);
		item->setSizeLocked(resizeable);
		item->setPrintEnabled(printable);
		retObj = m_Doc->Items->takeAt(z);
	}
	return retObj;
}

void VivaPlug::parseTextXML(const QDomElement& obNode, StoryText &itemText, int &textColumnCount, double &textColumnGap)
{
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setDefaultStyle(false);
	newStyle.setParent(pStyle);
	ParagraphStyle ttx = m_Doc->paragraphStyle(pStyle);
	CharStyle nstyle = ttx.charStyle();
	newStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
	newStyle.setLineSpacing(nstyle.fontSize() / 10.0);
	itemText.setDefaultStyle(newStyle);
	int posC = 0;
	for(QDomNode spo = obNode.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
	{
		QDomElement eo = spo.toElement();
		if (eo.tagName() == "vs:stylesheets")
			parseStylesheetsXML(eo);
		else if (eo.tagName() == "vs:attribute-sets")
			parseAttributeSetsXML(eo);
		else if (eo.tagName() == "vt:story")
		{
			if (eo.hasAttribute("vt:story-attribute-set"))
			{
				applyParagraphAttrs(newStyle, AttributeSets[eo.attribute("vt:story-attribute-set")]);
				applyCharacterAttrs(newStyle.charStyle(), newStyle, AttributeSets[eo.attribute("vt:story-attribute-set")]);
			}
			for(QDomNode stx = eo.firstChild(); !stx.isNull(); stx = stx.nextSibling() )
			{
				QDomElement stxe = stx.toElement();
				if (stxe.tagName() == "vt:chapter")
				{
					for(QDomNode st = stxe.firstChild(); !st.isNull(); st = st.nextSibling() )
					{
						QDomElement ste = st.toElement();
						if (ste.tagName() == "vt:layout")
						{
							if (ste.hasAttribute("vt:layout-attribute-set"))
							{
								AttributeSet attrs = AttributeSets[ste.attribute("vt:layout-attribute-set")];
								if (attrs.columnCount.valid)
									textColumnCount = attrs.columnCount.value.toInt();
								if (attrs.columnGutter.valid)
									textColumnGap = attrs.columnGutter.value.toDouble();
							}
							for(QDomNode stc = ste.firstChild(); !stc.isNull(); stc = stc.nextSibling() )
							{
								QDomElement stce = stc.toElement();
								if (stce.tagName() == "vt:p")
								{
									ParagraphStyle tmpStyle = newStyle;
									if (stce.hasAttribute("vt:paragraph-attribute-set"))
									{
										if (AttributeSets.contains(stce.attribute("vt:paragraph-attribute-set")))
											applyParagraphAttrs(tmpStyle, AttributeSets[stce.attribute("vt:paragraph-attribute-set")]);
										else if (m_Doc->styleExists(stce.attribute("vt:paragraph-attribute-set")))
											tmpStyle = m_Doc->paragraphStyle(stce.attribute("vt:paragraph-attribute-set"));
									}
									for(QDomNode stces = stce.firstChild(); !stces.isNull(); stces = stces.nextSibling() )
									{
										QDomElement stcet = stces.toElement();
										if (stcet.tagName() == "vt:span")
										{
											CharStyle tmpCStyle = tmpStyle.charStyle();
											if (stcet.hasAttribute("vt:character-attribute-set"))
												applyCharacterAttrs(tmpCStyle, tmpStyle, AttributeSets[stcet.attribute("vt:character-attribute-set")]);
											for(QDomNode stcesp = stcet.firstChild(); !stcesp.isNull(); stcesp = stcesp.nextSibling() )
											{
												QDomElement stcespt = stcesp.toElement();
												int count = stcespt.text().length();
												if (stcespt.tagName() == "vt:plain-text")
													itemText.insertChars(posC, stcespt.text());
												else if (stcespt.tagName() == "vt:lf")
												{
													count = 1;
													itemText.insertChars(posC, SpecialChars::LINEBREAK);
												}
												else if (stcespt.tagName() == "vt:soft-hyphen")
												{
													count = 1;
													itemText.insertChars(posC, SpecialChars::SHYPHEN);
												}
												else if (stcespt.tagName() == "vt:space")
												{
													if (stcespt.attribute("vt:br") == "false")
													{
														count = 1;
														itemText.insertChars(posC, SpecialChars::NBSPACE);
													}
												}
												else if (stcespt.tagName() == "vt:variable")
												{
													if (stcespt.attribute("vt:type") == "Viva Technology/Page Number")
													{
														count = 1;
														itemText.insertChars(posC, SpecialChars::PAGENUMBER);
													}
													else if (stcespt.attribute("vt:type") == "Viva Technology/Number of Pages")
													{
														count = 1;
														itemText.insertChars(posC, SpecialChars::PAGECOUNT);
													}
												/*	else if (stcespt.attribute("vt:type") == "Viva Technology/TextEngine/Date")
													{
														QDomNode anc = stcespt.firstChild();
														QDomElement anco = anc.toElement();
														if (anco.tagName() == "uni:data")
														{
															QByteArray data;
															if (anco.hasAttribute("uni:encoder") && (anco.attribute("uni:encoder") == "base64"))
																data = QByteArray::fromBase64(anco.text().toLatin1());
															else
																data = anco.text().toLatin1();
															QByteArray decomdata;
															if (anco.hasAttribute("uni:compressor") && (anco.attribute("uni:compressor") == "zlib/deflate"))
																decomdata = qUncompress(data);
															else
																decomdata = data;
														}
													} */
												}
												else if (stcespt.tagName() == "vt:anchoring-object")
												{
													for(QDomNode anc = stcespt.firstChild(); !anc.isNull(); anc = anc.nextSibling() )
													{
														QDomElement anco = anc.toElement();
														if (anco.tagName() == "vo:object")
														{
															PageItem* item = parseObjectXML(anco);
															if (item != NULL)
															{
																item->isEmbedded = true;
																item->gXpos = 0;
																item->gYpos = 0;
																item->gWidth = item->width();
																item->gHeight = item->height();
																int fIndex = m_Doc->addToInlineFrames(item);
																itemText.insertObject(fIndex);
															}
														}
													}
													count = 1;
												}
												else if (stcespt.tagName() == "vt:tab")
												{
													count = 1;
													itemText.insertChars(posC, SpecialChars::TAB);
												}
												if (count != 0)
												{
													itemText.applyStyle(posC, tmpStyle);
													itemText.applyCharStyle(posC, count, tmpCStyle);
												}
												posC = itemText.length();
											}
										}
									}
									if (stce.attribute("vt:break") == "paragraph-break")
										itemText.insertChars(posC, SpecialChars::PARSEP);
									else if (stce.attribute("vt:break") == "layout-column-break")
										itemText.insertChars(posC, SpecialChars::COLBREAK);
									else if (stce.attribute("vt:break") == "area-break")
										itemText.insertChars(posC, SpecialChars::FRAMEBREAK);
									else
										itemText.insertChars(posC, SpecialChars::PARSEP);
									itemText.applyStyle(posC, tmpStyle);
									posC = itemText.length();
								}
							}
						}
					}
				}
			}
		}
	}
}

void VivaPlug::parseAttributeSetXML(const QDomElement& obNode, AttributeSet &attrs)
{
	if (obNode.tagName() == "vs:template")
		attrs.parentStyle = obNode.text();
	for(QDomNode stx = obNode.firstChild(); !stx.isNull(); stx = stx.nextSibling() )
	{
		QDomElement stxe = stx.toElement();
		if (stxe.tagName() == "vta:font")
		{
			if (stxe.hasAttribute("vta:family"))
				attrs.fontFamily = AttributeValue(stxe.attribute("vta:family"));
			if (stxe.hasAttribute("vta:style"))
				attrs.fontStyle = AttributeValue(stxe.attribute("vta:style"));
		}
		else if (stxe.tagName() == "vta:font-size")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.fontSize = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:character-color")
		{
			if (stxe.hasAttribute("vta:name"))
				attrs.fontColor = AttributeValue(colorTranslate[stxe.attribute("vta:name")]);
		}
		else if (stxe.tagName() == "vta:character-color-density")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.fontColorDensity = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:character-type")
		{
			if (stxe.hasAttribute("vta:type"))
				attrs.fontEffect = AttributeValue(stxe.attribute("vta:type"));
		}
		else if (stxe.tagName() == "vta:character-placement")
		{
			if (stxe.hasAttribute("vta:type"))
				attrs.placement = AttributeValue(stxe.attribute("vta:type"));
		}
		else if (stxe.tagName() == "vta:underline")
		{
			attrs.underline = AttributeValue("true");
			attrs.underlineOffset = AttributeValue(stxe.attribute("vta:offset"));
			QDomNode ul = stxe.firstChild();
			if (!ul.isNull())
			{
				QDomElement ule = ul.toElement();
				if (ule.tagName() == "vta:pen")
					attrs.underlineWidth = AttributeValue(ule.attribute("vta:width"));
			}
		}
		else if (stxe.tagName() == "vta:strikethrough")
		{
			attrs.strikethrough = AttributeValue("true");
			attrs.strikethroughOffset = AttributeValue(stxe.attribute("vta:offset"));
			QDomNode ul = stxe.firstChild();
			if (!ul.isNull())
			{
				QDomElement ule = ul.toElement();
				if (ule.tagName() == "vta:pen")
					attrs.strikethroughWidth = AttributeValue(ule.attribute("vta:width"));
			}
		}
		else if (stxe.tagName() == "vta:character-outline")
		{
			if (stxe.attribute("vta:active") == "true")
			{
				attrs.outline = AttributeValue("true");
				QDomNode ul = stxe.firstChild();
				if (!ul.isNull())
				{
					QDomElement ule = ul.toElement();
					if (ule.tagName() == "vta:pen")
					{
						attrs.outlineWidth = AttributeValue(ule.attribute("vta:width"));
						attrs.outlineColor = AttributeValue(colorTranslate[ule.attribute("vta:color")]);
					}
				}
			}
		}
		else if (stxe.tagName() == "vta:character-width-scale")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.widthScale = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:character-height-scale")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.heightScale = AttributeValue(stxe.attribute("vta:value"));
		}
		else if ((stxe.tagName() == "vta:character-spacing") || (stxe.tagName() == "vta:kerning"))
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.spacing = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:baseline-shift")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.baselineOffset = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:writing-justification")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.justification = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:paragraph-distance-before")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.gapbefore = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:paragraph-distance-after")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.gapafter = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:line-distance")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.lineSpacing = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:paragraph-indent")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.firstLineIndent = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:paragraph-begin-indent")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.indent = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:paragraph-end-indent")
		{
			if (stxe.hasAttribute("vta:value"))
				attrs.rightIndent = AttributeValue(stxe.attribute("vta:value"));
		}
		else if (stxe.tagName() == "vta:layout-columns")
		{
			int columnCount = 0;
			double columnGutter = 0.0;
			for(QDomNode stc = stxe.firstChild(); !stc.isNull(); stc = stc.nextSibling() )
			{
				QDomElement stce = stc.toElement();
				if (stce.tagName() == "vta:column")
				{
					columnCount++;
					columnGutter = qMax(columnGutter, parseUnit(stce.attribute("vta:gutter", "0")));
				}
			}
			if (columnCount > 0)
			{
				attrs.columnCount = AttributeValue(QString("%1").arg(columnCount));
				attrs.columnGutter = AttributeValue(QString("%1").arg(columnGutter));
			}
		}
		else if (stxe.tagName() == "vta:initials")
		{
			if (stxe.attribute("vta:active") == "true")
			{
				attrs.dropCaps = AttributeValue("true");
				attrs.dropCapsDist = AttributeValue(stxe.attribute("vta:distance-to-text", "0"));
				for(QDomNode stc = stxe.firstChild(); !stc.isNull(); stc = stc.nextSibling() )
				{
					QDomElement stce = stc.toElement();
					if (stce.tagName() == "vta:size")
					{
						QDomNode ul = stce.firstChild();
						if (!ul.isNull())
						{
							QDomElement ule = ul.toElement();
							if (ule.tagName() == "vta:dynamic-lines")
							{
								attrs.dropCapsLines = AttributeValue(ule.attribute("vta:maximum-lines", "1"));
							}
						}
					}
				}
			}
		}
		else if (stxe.tagName() == "vta:tabulators")
		{
			QString tabs = "";
			for(QDomNode stc = stxe.firstChild(); !stc.isNull(); stc = stc.nextSibling() )
			{
				QDomElement stce = stc.toElement();
				if (stce.tagName() == "vta:tabulator")
				{
					if (stce.hasAttribute("vta:position"))
						tabs += stce.attribute("vta:position");
					tabs += "\t";
					if (stce.hasAttribute("vta:alignment"))
						tabs += stce.attribute("vta:alignment");
					tabs += "\t";
					if (stce.hasAttribute("vta:filler"))
						tabs += stce.attribute("vta:filler");
				}
				tabs += "\n";
			}
			attrs.tabulators = AttributeValue(tabs);
		}
	}
	if ((attrs.fontFamily.valid) && (attrs.fontStyle.valid))
		attrs.fontFullName = AttributeValue(constructFontName(attrs.fontFamily.value, attrs.fontStyle.value));
}

void VivaPlug::parseAttributeSetsXML(const QDomElement& obNode)
{
	for(QDomNode spo = obNode.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
	{
		QDomElement eo = spo.toElement();
		AttributeSet attrs;
		parseAttributeSetXML(eo, attrs);
		if (eo.hasAttribute("vs:paragraph-style"))
			attrs.applyedParStyle = AttributeValue(eo.attribute("vs:paragraph-style"));
		AttributeSets.insert(eo.attribute("vs:name"), attrs);
	}
}

void VivaPlug::parseStylesheetsXML(const QDomElement& obNode)
{
	for(QDomNode spo = obNode.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
	{
		QDomElement eo = spo.toElement();
		if (eo.tagName() == "vs:paragraphStylesheet")
		{
			ParagraphStyle newStyle;
			newStyle.erase();
			newStyle.setDefaultStyle(false);
			newStyle.setName(eo.attribute("vs:name"));
			newStyle.setParent(CommonStrings::DefaultParagraphStyle);
			ParagraphStyle ttx = m_Doc->paragraphStyle(CommonStrings::DefaultParagraphStyle);
			CharStyle nstyle = ttx.charStyle();
			newStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
			newStyle.setLineSpacing(nstyle.fontSize() / 10.0);
			AttributeSet attrs;
			for(QDomNode stx = eo.firstChild(); !stx.isNull(); stx = stx.nextSibling() )
			{
				QDomElement stxe = stx.toElement();
				parseAttributeSetXML(stxe, attrs);
			}
			applyParagraphAttrs(newStyle, attrs);
			applyCharacterAttrs(newStyle.charStyle(), newStyle, attrs);
			StyleSet<ParagraphStyle>tmp;
			tmp.create(newStyle);
			m_Doc->redefineStyles(tmp, false);
		}
	}
}

void VivaPlug::applyParagraphAttrs(ParagraphStyle &newStyle, AttributeSet &pAttrs)
{
	if (pAttrs.applyedParStyle.valid)
		newStyle = m_Doc->paragraphStyle(pAttrs.applyedParStyle.value);
	if (pAttrs.parentStyle.valid)
		newStyle.setParent(pAttrs.parentStyle.value);
	if (pAttrs.justification.valid)
	{
		if (pAttrs.justification.value == "left")
			newStyle.setAlignment(ParagraphStyle::Leftaligned);
		else if (pAttrs.justification.value == "right")
			newStyle.setAlignment(ParagraphStyle::Rightaligned);
		else if (pAttrs.justification.value == "center")
			newStyle.setAlignment(ParagraphStyle::Centered);
		else if (pAttrs.justification.value == "justified")
			newStyle.setAlignment(ParagraphStyle::Justified);
		else if (pAttrs.justification.value == "forced-justified")
			newStyle.setAlignment(ParagraphStyle::Extended);
	}
	if (pAttrs.gapbefore.valid)
		newStyle.setGapBefore(parseUnit(pAttrs.gapbefore.value));
	if (pAttrs.gapafter.valid)
		newStyle.setGapAfter(parseUnit(pAttrs.gapafter.value));
	if (pAttrs.lineSpacing.valid)
	{
		newStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
		newStyle.setLineSpacing(parseUnit(pAttrs.lineSpacing.value));
	}
	else
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	if (pAttrs.firstLineIndent.valid)
		newStyle.setFirstIndent(parseUnit(pAttrs.firstLineIndent.value));
	if (pAttrs.indent.valid)
	{
		newStyle.setLeftMargin(parseUnit(pAttrs.indent.value));
		newStyle.setFirstIndent(newStyle.firstIndent() - parseUnit(pAttrs.indent.value));
	}
	if (pAttrs.rightIndent.valid)
		newStyle.setRightMargin(parseUnit(pAttrs.rightIndent.value));
	if (pAttrs.dropCaps.valid)
	{
		newStyle.setHasDropCap(true);
		if (pAttrs.dropCapsLines.valid)
			newStyle.setDropCapLines(pAttrs.dropCapsLines.value.toInt());
		if (pAttrs.dropCapsDist.valid)
			newStyle.setParEffectOffset(parseUnit(pAttrs.dropCapsDist.value));
	}
	if (pAttrs.tabulators.valid)
	{
		QList<ParagraphStyle::TabRecord> tbs;
		newStyle.resetTabValues();
		QStringList tabList = pAttrs.tabulators.value.split("\n");
		for (int a = 0; a < tabList.count(); a++)
		{
			QString tbv = tabList[a];
			if (!tbv.isEmpty())
			{
				ParagraphStyle::TabRecord tb;
				QStringList tab = tbv.split("\t");
				tb.tabPosition = parseUnit(tab[0]);
				tb.tabType = 0;
				if (tab[1] == "begin")
					tb.tabType = 0;
				else if (tab[1] == "center")
					tb.tabType = 4;
				else if (tab[1] == "end")
					tb.tabType = 1;
				else if (tab[1] == "character")
					tb.tabType = 3;
				tb.tabFillChar = tab[2].isEmpty() ? QChar() : tab[2][0];
				tbs.append(tb);
			}
		}
		if (tbs.count() > 0)
			newStyle.setTabValues(tbs);
	}
}

void VivaPlug::applyCharacterAttrs(CharStyle &tmpCStyle, ParagraphStyle &newStyle, AttributeSet &pAttrs)
{
	if (pAttrs.fontSize.valid)
	{
		tmpCStyle.setFontSize(pAttrs.fontSize.value.toInt() * 10);
		if (pAttrs.lineSpacing.valid)
		{
			if (pAttrs.fontSize.value.toInt() > parseUnit(pAttrs.lineSpacing.value))
				newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		}
	}
	if (pAttrs.fontFullName.valid)
		tmpCStyle.setFont((*m_Doc->AllFonts)[pAttrs.fontFullName.value]);
	if (pAttrs.fontColor.valid)
		tmpCStyle.setFillColor(pAttrs.fontColor.value);
	if (pAttrs.fontColorDensity.valid)
	{
		int tint = 100;
		if (pAttrs.fontColorDensity.value == "transparent")
			tint = 0;
		else
			tint = pAttrs.fontColorDensity.value.toInt();
		tmpCStyle.setFillShade(tint);
	}
	if (pAttrs.underline.valid)
	{
		if (pAttrs.underlineWidth.valid)
			tmpCStyle.setUnderlineWidth(qRound(parseUnit(pAttrs.underlineWidth.value) * 1000 / (tmpCStyle.fontSize() / 10.0)));
		else
			tmpCStyle.setUnderlineWidth(-1);
		if (pAttrs.underlineOffset.valid)
		{
			if (parseUnit(pAttrs.underlineOffset.value) != 0)
				tmpCStyle.setUnderlineOffset(qRound(parseUnit(pAttrs.underlineOffset.value) * 1000 / (tmpCStyle.fontSize() / 10.0)));
			else
				tmpCStyle.setUnderlineOffset(-1);
		}
		else
			tmpCStyle.setUnderlineOffset(-1);
		StyleFlag styleEffects = tmpCStyle.effects();
		styleEffects |= ScStyle_Underline;
		tmpCStyle.setFeatures(styleEffects.featureList());
	}
	if (pAttrs.strikethrough.valid)
	{
		if (pAttrs.strikethroughWidth.valid)
			tmpCStyle.setStrikethruWidth(qRound(parseUnit(pAttrs.strikethroughWidth.value) * 1000 / (tmpCStyle.fontSize() / 10.0)));
		else
			tmpCStyle.setStrikethruWidth(-1);
		if (pAttrs.strikethroughOffset.valid)
		{
			if (parseUnit(pAttrs.strikethroughOffset.value) != 0)
				tmpCStyle.setStrikethruOffset(qRound(parseUnit(pAttrs.strikethroughOffset.value) * 1000 / (tmpCStyle.fontSize() / 10.0)));
			else
				tmpCStyle.setStrikethruOffset(-1);
		}
		else
			tmpCStyle.setStrikethruOffset(-1);
		StyleFlag styleEffects = tmpCStyle.effects();
		styleEffects |= ScStyle_Strikethrough;
		tmpCStyle.setFeatures(styleEffects.featureList());
	}
	if (pAttrs.outline.valid)
	{
		if (pAttrs.outlineWidth.valid)
			tmpCStyle.setOutlineWidth(qRound(parseUnit(pAttrs.outlineWidth.value) * 1000 / (tmpCStyle.fontSize() / 10.0)));
		else
			tmpCStyle.setOutlineWidth(-1);
		if (pAttrs.outlineColor.valid)
			tmpCStyle.setStrokeColor(pAttrs.outlineColor.value);
		StyleFlag styleEffects = tmpCStyle.effects();
		styleEffects |= ScStyle_Outline;
		tmpCStyle.setFeatures(styleEffects.featureList());
	}
	if (pAttrs.placement.valid)
	{
		StyleFlag styleEffects = tmpCStyle.effects();
		if ((pAttrs.placement.value == "superscript") || (pAttrs.placement.value == "index"))
			styleEffects |= ScStyle_Superscript;
		else if (pAttrs.placement.value == "subscript")
			styleEffects |= ScStyle_Subscript;
		tmpCStyle.setFeatures(styleEffects.featureList());
	}
	if (pAttrs.fontEffect.valid)
	{
		StyleFlag styleEffects = tmpCStyle.effects();
		if (pAttrs.fontEffect.value == "caps")
			styleEffects |= ScStyle_AllCaps;
		else if (pAttrs.fontEffect.value == "small-caps")
			styleEffects |= ScStyle_SmallCaps;
		tmpCStyle.setFeatures(styleEffects.featureList());
	}
	if (pAttrs.widthScale.valid)
		tmpCStyle.setScaleH(pAttrs.widthScale.value.toInt() * 10);
	if (pAttrs.heightScale.valid)
		tmpCStyle.setScaleV(pAttrs.heightScale.value.toInt() * 10);
	if (pAttrs.spacing.valid)
		tmpCStyle.setTracking(pAttrs.spacing.value.toInt() * 10);
	if (pAttrs.baselineOffset.valid)
	{
		if (parseUnit(pAttrs.baselineOffset.value) != 0)
			tmpCStyle.setBaselineOffset(qRound(parseUnit(pAttrs.baselineOffset.value) * 1000 / (tmpCStyle.fontSize() / 10.0)));
	}
}

QString VivaPlug::constructFontName(QString fontBaseName, QString fontStyle)
{
	QString fontName = "";
	bool found = false;
	SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (fontBaseName.toLower() == it.current().family().toLower())
		{
			// found the font family, now go for the style
			QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[it.current().family()];
			slist.sort();
			if (slist.count() > 0)
			{
				for (int a = 0; a < slist.count(); a++)
				{
					if (fontStyle.toLower() == slist[a].toLower())
					{
						found = true;
						fontName = it.current().family() + " " + slist[a];
						break;
					}
				}
				if (!found)
				{
					int reInd = slist.indexOf("Regular");
					if (reInd < 0)
						fontName = it.current().family() + " " + slist[0];
					else
						fontName = it.current().family() + " " + slist[reInd];
					found = true;
				}
			}
			else
			{
				fontName = it.current().family();
				found = true;
			}
			break;
		}
	}
	if (!found)
	{
		if (importerFlags & LoadSavePlugin::lfCreateThumbnail)
			fontName = PrefsManager::instance()->appPrefs.itemToolPrefs.textFont;
		else
		{
			QString family = fontBaseName;
			if (!fontStyle.isEmpty())
				family += " " + fontStyle;
			if (!PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(family))
			{
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				MissingFont *dia = new MissingFont(0, family, m_Doc);
				dia->exec();
				fontName = dia->getReplacementFont();
				delete dia;
				qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
				PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family] = fontName;
			}
			else
				fontName = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family];
		}
	}
	return fontName;
}

QPointF VivaPlug::intersectBoundingRect(PageItem *item, QLineF gradientVector)
{
	QPointF interPoint;
	QPointF gradEnd;
	if (gradientVector.intersect(QLineF(0, 0, item->width(), 0), &interPoint) == QLineF::BoundedIntersection)
		gradEnd = interPoint;
	else if (gradientVector.intersect(QLineF(item->width(), 0, item->width(), item->height()), &interPoint) == QLineF::BoundedIntersection)
		gradEnd = interPoint;
	else if (gradientVector.intersect(QLineF(item->width(), item->height(), 0, item->height()), &interPoint) == QLineF::BoundedIntersection)
		gradEnd = interPoint;
	else if (gradientVector.intersect(QLineF(0, item->height(), 0, 0), &interPoint) == QLineF::BoundedIntersection)
		gradEnd = interPoint;
	return gradEnd;
}
