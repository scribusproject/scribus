/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Jan 14 2012
	copyright            : (C) 2012 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#include <QByteArray>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMimeData>
#include <QRegExp>
#include <QScopedPointer>
#include <QStack>
#include <QUrl>

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include <cstdlib>
#include <climits>
#include <limits>

#include "importidml.h"

#include "commonstrings.h"
#include "guidemanagercore.h"
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

IdmlPlug::IdmlPlug(ScribusDoc* doc, int flags)
{
	tmpSel = new Selection(this, false);
	m_Doc = doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = nullptr;
	m_zip = nullptr;
}

QString IdmlPlug::getNodeValue(QDomNode &baseNode, const QString& path)
{
	QString ret;
	QStringList pathParts = path.split("/", QString::SkipEmptyParts);
	QDomNode n = baseNode.namedItem(pathParts[0]);
	if (n.isNull())
		return QString();

	bool fail = false;
	for (int a = 1; a < pathParts.count(); a++)
	{
		n = n.namedItem(pathParts[a]);
		if (n.isNull())
		{
			fail = true;
			break;
		}
	}
	if (!fail)
	{
		QDomElement e = n.toElement();
		if (!e.isNull())
			ret = e.text();
	}
	return ret;
}

QImage IdmlPlug::readThumbnail(const QString& fName)
{
	QImage tmp;
	QByteArray f;
	if ( !QFile::exists(fName) )
		return QImage();
	QFileInfo fi = QFileInfo(fName);
	QString ext = fi.suffix().toLower();
	if (ext == "idml")
	{
		m_zip = new ScZipHandler();
		if (!m_zip->open(fName))
		{
			delete m_zip;
			m_zip = nullptr;
			return QImage();
		}
		if (m_zip->contains("designmap.xml"))
			m_zip->read("designmap.xml", f);
		delete m_zip;
		m_zip = nullptr;
	}
	else if (ext == "idms")
	{
		loadRawText(fName, f);
	}

	if (f.isEmpty())
		return QImage();

	if (!designMapDom.setContent(f))
		return QImage();

	bool found = false;
	QDomElement docElem = designMapDom.documentElement();
	QString metaD = getNodeValue(docElem, "MetadataPacketPreference/Properties/Contents");
	QDomDocument rdfD;
	rdfD.setContent(metaD);
	QDomElement docElemR = rdfD.documentElement();
	for (QDomNode drawPag = docElemR.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling())
	{
		QDomElement dpg = drawPag.toElement();
		if (dpg.tagName() == "rdf:RDF")
		{
			for (QDomNode drawPag2 = dpg.firstChild(); !drawPag2.isNull(); drawPag2 = drawPag2.nextSibling())
			{
				QDomElement dpg2 = drawPag2.toElement();
				if (dpg2.hasAttribute("xmlns:xmpGImg"))
				{
					QByteArray imgD = getNodeValue(dpg2, "xmp:Thumbnails/rdf:Alt/rdf:li/xmpGImg:image").toLatin1();
					QByteArray inlineImageData = QByteArray::fromBase64(imgD);
					tmp.loadFromData(inlineImageData);
					found = true;
				}
			}
		}
	}
	if (!found)
	{
		progressDialog = nullptr;
		QFileInfo fi = QFileInfo(fName);
		baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
		docWidth = PrefsManager::instance().appPrefs.docSetupPrefs.pageWidth;
		docHeight = PrefsManager::instance().appPrefs.docSetupPrefs.pageHeight;
		m_Doc = new ScribusDoc();
		m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
		m_Doc->setPage(docWidth, docHeight, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->setGUI(false, ScCore->primaryMainWindow(), nullptr);
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset() + m_Doc->currentPage()->height() / 2.0;
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
		QDir::setCurrent(CurDirP);
		m_Doc->DoDrawing = true;
		m_Doc->scMW()->setScriptRunning(false);
		delete m_Doc;
	}
	return tmp;
}

bool IdmlPlug::readColors(const QString& fileName, ColorList & colors)
{
	bool success = false;
	importedColors.clear();

	QByteArray f;
	QFileInfo fi = QFileInfo(fileName);
	QString ext = fi.suffix().toLower();
	if (ext == "idml")
	{
		m_zip = new ScZipHandler();
		if (!m_zip->open(fileName))
		{
			delete m_zip;
			m_zip = nullptr;
			return false;
		}
		if (m_zip->contains("designmap.xml"))
			m_zip->read("designmap.xml", f);
	}
	else if (ext == "idms")
	{
		loadRawText(fileName, f);
	}

	if (f.isEmpty())
	{
		delete m_zip;
		m_zip = nullptr;
		return false;
	}

	if (!designMapDom.setContent(f))
	{
		delete m_zip;
		m_zip = nullptr;
		return false;
	}

	m_Doc = new ScribusDoc();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(1, 1, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), nullptr);

	QDomElement docElem = designMapDom.documentElement();
	if (ext == "idms")
	{
		parseGraphicsXMLNode(docElem);
	}
	else
	{
		for (QDomNode drawPag = docElem.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling())
		{
			QDomElement dpg = drawPag.toElement();
			if (dpg.tagName() == "idPkg:Graphic")
			{
				if (!parseGraphicsXML(dpg))
				{
					delete m_zip;
					m_zip = nullptr;
					return false;
				}
			}
		}
	}

	delete m_zip;
	m_zip = nullptr;

	if (importedColors.count() != 0)
	{
		colors = m_Doc->PageColors;
		success = true;
	}
	delete m_Doc;
	return success;
}

bool IdmlPlug::import(const QString& fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	bool ret = false;
	firstLayer = true;
	firstPage = true;
	pagecount = 1;
	mpagecount = 0;
	QFileInfo fi = QFileInfo(fNameIn);
	if (!ScCore->usingGUI())
	{
		interactive = false;
		showProgress = false;
	}
	baseFile = QDir::cleanPath(QDir::toNativeSeparators(fi.absolutePath()+"/"));
	if (showProgress)
	{
		ScribusMainWindow* mw=(m_Doc==nullptr) ? ScCore->primaryMainWindow() : m_Doc->scMW();
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
		progressDialog = nullptr;
	if (progressDialog)
	{
		progressDialog->setOverallProgress(1);
		qApp->processEvents();
	}
	/* Set default Page to size defined in Preferences */
	docWidth = PrefsManager::instance().appPrefs.docSetupPrefs.pageWidth;
	docHeight = PrefsManager::instance().appPrefs.docSetupPrefs.pageHeight;
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
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != nullptr))
		m_Doc->view()->deselectItems();
	Elements.clear();
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != nullptr))
		m_Doc->view()->updatesOn(false);
	m_Doc->scMW()->setScriptRunning(true);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	QString CurDirP = QDir::currentPath();
	QDir::setCurrent(fi.path());
	if (convert(fNameIn))
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
					if (m_Doc->view() != nullptr)
						m_Doc->view()->updatesOn(true);
				}
			}
			else
			{
				m_Doc->DragP = true;
				m_Doc->DraggedElem = nullptr;
				m_Doc->DragElements.clear();
				m_Doc->m_Selection->delaySignalsOn();
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					tmpSel->addItem(Elements.at(dre), true);
				}
				tmpSel->setGroupRect();
				ScElemMimeData* md = ScriXmlDoc::writeToMimeData(m_Doc, tmpSel);
				m_Doc->itemSelection_DeleteItem(tmpSel);
				m_Doc->view()->updatesOn(true);
				m_Doc->m_Selection->delaySignalsOff();
				// We must copy the TransationSettings object as it is owned
				// by handleObjectImport method afterwards
				TransactionSettings* transacSettings = new TransactionSettings(trSettings);
				m_Doc->view()->handleObjectImport(md, transacSettings);
				m_Doc->DragP = false;
				m_Doc->DraggedElem = nullptr;
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

IdmlPlug::~IdmlPlug()
{
	delete progressDialog;
	delete tmpSel;
}

bool IdmlPlug::convert(const QString& fn)
{
	Coords.resize(0);
	Coords.svgInit();
	importedColors.clear();
	def_fillColor = CommonStrings::None;
	def_strokeColor = CommonStrings::None;
	def_fillGradient = "";
	def_strokeGradient = "";
	def_Blendmode = 0;
	def_fillBlendmode = 0;
	def_strokeBlendmode = 0;
	def_fillTint = 100;
	def_strokeTint = 100;
	def_lineWidth = 0;
	def_Opacity = 0.0;
	def_fillOpacity = 0.0;
	def_strokeOpacity = 0.0;
	def_gradientAngle = 0.0;
	def_gradientLen = 0.0;
	def_gradientX = 0.0;
	def_gradientY = 0.0;
	def_gradientStrokeStartX = 0;
	def_gradientStrokeStartY = 0;
	def_gradientStrokeLength = 0;
	def_gradientStrokeAngle = 0;
	def_Extra = 0;
	def_TExtra = 0;
	def_BExtra = 0;
	def_RExtra = 0;
	def_TextFlow = PageItem::TextFlowDisabled;
	def_TextColumnCount = 1;
	def_TextColumnGutter = 0;
	def_TextColumnFixedWidth = 0;
	def_LeftLineEnd = "None";
	def_RightLineEnd = "None";
	frameLinks.clear();
	frameTargets.clear();
	importedColors.clear();
	colorTranslate.clear();
	importedGradients.clear();
	gradientTranslate.clear();
	gradientTypeMap.clear();
	layerTranslate.clear();
	storyMap.clear();
	styleTranslate.clear();
	charStyleTranslate.clear();
	ObjectStyles.clear();
	if (progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}
	colorTranslate.insert("Swatch/None", CommonStrings::None);
	bool retVal = true;
	bool firstSpread = true;

	QByteArray f;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	if (ext == "idml")
	{
		m_zip = new ScZipHandler();
		if (!m_zip->open(fn))
		{
			delete m_zip;
			m_zip = nullptr;
			return false;
		}
		if (m_zip->contains("designmap.xml"))
			m_zip->read("designmap.xml", f);
	}
	else if (ext == "idms")
	{
		loadRawText(fn, f);
	}

	if (f.isEmpty())
	{
		if (progressDialog)
			progressDialog->close();
		delete m_zip;
		m_zip = nullptr;
		return false;
	}

	if (!designMapDom.setContent(f))
	{
		if (progressDialog)
			progressDialog->close();
		delete m_zip;
		m_zip = nullptr;
		return false;
	}

	QDomElement docElem = designMapDom.documentElement();
	QString activeLayer = docElem.attribute("ActiveLayer");
	if (ext == "idms")
	{
		for (QDomNode drawPag = docElem.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling())
		{
			QDomElement dpg = drawPag.toElement();
			if (dpg.tagName() == "Layer")
			{
				QString layerSelf = dpg.attribute("Self");
				QString layerName = dpg.attribute("Name");
				if (importerFlags & LoadSavePlugin::lfCreateDoc)
				{
					int currentLayer = 0;
					if (!firstLayer)
						currentLayer = m_Doc->addLayer(layerName);
					else
						m_Doc->changeLayerName(currentLayer, layerName);
					m_Doc->setLayerVisible(currentLayer, (dpg.attribute("Visible") == "true"));
					m_Doc->setLayerLocked(currentLayer, (dpg.attribute("Locked") == "true"));
					m_Doc->setLayerPrintable(currentLayer, (dpg.attribute("Printable") == "true"));
					m_Doc->setLayerFlow(currentLayer, (dpg.attribute("IgnoreWrap","") == "true"));
				}
				layerTranslate.insert(layerSelf, layerName);
				firstLayer = false;
			}
		}
		parseFontsXMLNode(docElem);
		parseGraphicsXMLNode(docElem);
		parseStylesXMLNode(docElem);
		parsePreferencesXMLNode(docElem);
		parseSpreadXMLNode(docElem);
		parseStoryXMLNode(docElem);
	}
	else
	{
		for (QDomNode drawPag = docElem.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling())
		{
			QDomElement dpg = drawPag.toElement();
			if (dpg.tagName() == "Layer")
			{
				QString layerSelf = dpg.attribute("Self");
				QString layerName = dpg.attribute("Name");
				if (importerFlags & LoadSavePlugin::lfCreateDoc)
				{
					int currentLayer = 0;
					if (!firstLayer)
						currentLayer = m_Doc->addLayer(layerName);
					else
						m_Doc->changeLayerName(currentLayer, layerName);
					m_Doc->setLayerVisible(currentLayer, (dpg.attribute("Visible") == "true"));
					m_Doc->setLayerLocked(currentLayer, (dpg.attribute("Locked") == "true"));
					m_Doc->setLayerPrintable(currentLayer, (dpg.attribute("Printable") == "true"));
					m_Doc->setLayerFlow(currentLayer, (dpg.attribute("IgnoreWrap","") == "true"));
				}
				layerTranslate.insert(layerSelf, layerName);
				firstLayer = false;
			}
			if (dpg.tagName() == "idPkg:Fonts")
			{
				if (!parseFontsXML(dpg))
				{
					retVal = false;
					break;
				}
			}
			if (dpg.tagName() == "idPkg:Graphic")
			{
				if (!parseGraphicsXML(dpg))
				{
					retVal = false;
					break;
				}
			}
			if (dpg.tagName() == "idPkg:Styles")
			{
				if (!parseStylesXML(dpg))
				{
					retVal = false;
					break;
				}
			}
			if (dpg.tagName() == "idPkg:Preferences")
			{
				if (!parsePreferencesXML(dpg))
				{
					retVal = false;
					break;
				}
			}
			if (dpg.tagName() == "idPkg:MasterSpread")
			{
				if (importerFlags & LoadSavePlugin::lfCreateDoc)
				{
					if (!parseSpreadXML(dpg))
					{
						retVal = false;
						break;
					}
				}
			}
			if (dpg.tagName() == "idPkg:Spread")
			{
				if (!(importerFlags & LoadSavePlugin::lfCreateDoc))
				{
					if (firstSpread)
					{
						parseSpreadXML(dpg);
						firstSpread = false;
					}
				}
				else if (!parseSpreadXML(dpg))
				{
					retVal = false;
					break;
				}
			}
			if (dpg.tagName() == "idPkg:Story")
			{
				if (!parseStoryXML(dpg))
				{
					retVal = false;
					break;
				}
			}
		}
	}
	if (!frameLinks.isEmpty())
	{
		QMap<PageItem*, QString>::Iterator lc;
		for (lc = frameLinks.begin(); lc != frameLinks.end(); ++lc)
		{
			PageItem *Its = lc.key();
			PageItem *Itn = frameTargets[lc.value()];
			if (Its->canBeLinkedTo(Itn))
				Its->link(Itn);
		}
	}
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		if (layerTranslate.contains(activeLayer))
			activeLayer = layerTranslate[activeLayer];
		else
			activeLayer = m_Doc->layerName(0);
		m_Doc->setActiveLayer(activeLayer);
	}

	delete m_zip;
	m_zip = nullptr;

	if (progressDialog)
		progressDialog->close();
	return retVal;
}

bool IdmlPlug::parseFontsXML(const QDomElement& grElem)
{
	QDomElement grNode;
	QDomDocument grMapDom;
	if (grElem.hasAttribute("src"))
	{
		QByteArray f2;
		m_zip->read(grElem.attribute("src"), f2);
		if (!grMapDom.setContent(f2))
			return false;
		grNode = grMapDom.documentElement();
	}
	else
	{
		if (!grElem.hasChildNodes())
			return false;
		grNode = grElem;
	}
	parseFontsXMLNode(grNode);
	return true;
}

void IdmlPlug::parseFontsXMLNode(const QDomElement& grNode)
{
	for (QDomNode n = grNode.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement e = n.toElement();
		if (e.tagName() == "FontFamily")
		{
			QString family = e.attribute("Name");
			QMap<QString, QString> styleMap;
			for (QDomNode gr = e.firstChild(); !gr.isNull(); gr = gr.nextSibling())
			{
				QDomElement grs = gr.toElement();
				if (grs.tagName() == "Font")
				{
					QString styleName = grs.attribute("FontStyleName").remove("$ID/");
					QString postName = grs.attribute("PostScriptName").remove("$ID/");
					styleMap.insert(styleName, postName);
				}
			}
			fontTranslateMap.insert(family, styleMap);
		}
	}
}

bool IdmlPlug::parseGraphicsXML(const QDomElement& grElem)
{
	QDomElement grNode;
	QDomDocument grMapDom;
	if (grElem.hasAttribute("src"))
	{
		QByteArray f2;
		m_zip->read(grElem.attribute("src"), f2);
		if (!grMapDom.setContent(f2))
			return false;
		grNode = grMapDom.documentElement();
	}
	else
	{
		if (!grElem.hasChildNodes())
			return false;
		grNode = grElem;
	}
	parseGraphicsXMLNode(grNode);
	return true;
}

void IdmlPlug::parseGraphicsXMLNode(const QDomElement& grNode)
{
	for (QDomNode n = grNode.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement e = n.toElement();
		if (e.tagName() == "Color")
		{
			QString colorSelf = e.attribute("Self");
			QString colorName = e.attribute("Self").remove(0, 6);
			QString colorData = e.attribute("ColorValue");
			QString colorSpace = e.attribute("Space");
			QString colorModel = e.attribute("Model");
			if (colorSpace == "CMYK")
			{
				double c, m, y, k;
				ScColor tmp;
				ScTextStream Code(&colorData, QIODevice::ReadOnly);
				Code >> c >> m >> y >> k;
				tmp.setColorF(c / 100.0, m / 100.0, y / 100.0, k / 100.0);
				tmp.setSpotColor(colorModel == "Spot");
				tmp.setRegistrationColor(colorModel == "Registration");
				QString fNam = m_Doc->PageColors.tryAddColor(colorName, tmp);
				if (fNam == colorName)
					importedColors.append(fNam);
				colorTranslate.insert(colorSelf, fNam);
			}
			else if (colorSpace == "RGB")
			{
				int r, g, b;
				ScColor tmp;
				ScTextStream Code(&colorData, QIODevice::ReadOnly);
				Code >> r >> g >> b;
				tmp.setRgbColor(r, g, b);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor(colorName, tmp);
				if (fNam == colorName)
					importedColors.append(fNam);
				colorTranslate.insert(colorSelf, fNam);
			}
			else if (colorSpace == "LAB")
			{
				double L, a, b;
				ScColor tmp;
				ScTextStream Code(&colorData, QIODevice::ReadOnly);
				Code >> L >> a >> b;
				tmp.setLabColor(L, a, b);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor(colorName, tmp);
				if (fNam == colorName)
					importedColors.append(fNam);
				colorTranslate.insert(colorSelf, fNam);
			}
		}
		else if (e.tagName() == "Gradient")
		{
			QString grSelf = e.attribute("Self");
			QString grName = e.attribute("Self").remove(0, 9);
			int grTyp = (e.attribute("Type") == "Linear") ? 6 : 7;
			VGradient currentGradient = VGradient(VGradient::linear);
			currentGradient.clearStops();
			for (QDomNode gr = e.firstChild(); !gr.isNull(); gr = gr.nextSibling())
			{
				QDomElement grs = gr.toElement();
				if (grs.tagName() == "GradientStop")
				{
					QString stopName = grs.attribute("StopColor");
					double stop = grs.attribute("Location", "0.0").toDouble();
					if (colorTranslate.contains(stopName))
						stopName = colorTranslate[stopName];
					else
						stopName = "Black";
					const ScColor& gradC = m_Doc->PageColors[stopName];
					currentGradient.addStop( ScColorEngine::getRGBColor(gradC, m_Doc), stop / 100.0, 0.5, 1.0, stopName, 100 );
				}
			}
			if (m_Doc->addGradient(grName, currentGradient))
				importedGradients.append(grName);
			gradientTranslate.insert(grSelf, grName);
			gradientTypeMap.insert(grSelf, grTyp);
		}
		else if (e.tagName() == "Tint")
		{
			QString colorSelf = e.attribute("Self");
			QString colorName = e.attribute("Self").remove(0, 5);
			QString baseName = e.attribute("BaseColor", "Black");
			double tint = e.attribute("TintValue", "100").toDouble() / 100.0;
			if (colorTranslate.contains(baseName))
			{
				ScColor tmp = m_Doc->PageColors[colorTranslate[baseName]];
				ScColor res;
				if (tmp.getColorModel() == colorModelCMYK)
				{
					double c, m, y, k;
					tmp.getCMYK(&c, &m, &y, &k);
					res.setColorF(c * tint, m * tint, y * tint, k * tint);
				}
				else
				{
					double r, g, b;
					tmp.getRGB(&r, &g, &b);
					res.setRgbColorF(r * tint, g * tint, b * tint);
				}
				res.setSpotColor(false);
				res.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor(colorName, res);
				if (fNam == colorName)
					importedColors.append(fNam);
				colorTranslate.insert(colorSelf, fNam);
			}
		}
	}
}

bool IdmlPlug::parseStylesXML(const QDomElement& sElem)
{
	QDomElement sNode;
	QDomDocument sMapDom;
	if (sElem.hasAttribute("src"))
	{
		QByteArray f2;
		m_zip->read(sElem.attribute("src"), f2);
		if (!sMapDom.setContent(f2))
			return false;
		sNode = sMapDom.documentElement();
	}
	else
	{
		if (!sElem.hasChildNodes())
			return false;
		sNode = sElem;
	}
	parseStylesXMLNode(sNode);
	return true;
}

void IdmlPlug::parseStylesXMLNode(const QDomElement& sNode)
{
	for (QDomNode n = sNode.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement e = n.toElement();
		if (e.tagName() == "RootCharacterStyleGroup")
		{
			for (QDomNode it = e.firstChild(); !it.isNull(); it = it.nextSibling())
			{
				QDomElement itpg = it.toElement();
				if (itpg.tagName() == "CharacterStyle")
					parseCharacterStyle(itpg);
				else if (itpg.tagName() == "CharacterStyleGroup")
				{
					for (QDomNode its = itpg.firstChild(); !its.isNull(); its = its.nextSibling())
					{
						QDomElement itp = its.toElement();
						if (itp.tagName() == "CharacterStyle")
							parseCharacterStyle(itp);
					}
				}
			}
		}
		if (e.tagName() == "RootParagraphStyleGroup")
		{
			for (QDomNode it = e.firstChild(); !it.isNull(); it = it.nextSibling())
			{
				QDomElement itpg = it.toElement();
				if (itpg.tagName() == "ParagraphStyle")
					parseParagraphStyle(itpg);
				else if (itpg.tagName() == "ParagraphStyleGroup")
				{
					for (QDomNode its = itpg.firstChild(); !its.isNull(); its = its.nextSibling())
					{
						QDomElement itp = its.toElement();
						if (itp.tagName() == "ParagraphStyle")
							parseParagraphStyle(itp);
					}
				}
			}
		}
		if (e.tagName() == "RootObjectStyleGroup")
		{
			for (QDomNode it = e.firstChild(); !it.isNull(); it = it.nextSibling())
			{
				QDomElement itpg = it.toElement();
				if (itpg.tagName() == "ObjectStyle")
					parseObjectStyle(itpg);
				else if (itpg.tagName() == "ObjectStyleGroup")
				{
					for (QDomNode its = itpg.firstChild(); !its.isNull(); its = its.nextSibling())
					{
						QDomElement itp = its.toElement();
						if (itp.tagName() == "ObjectStyle")
							parseObjectStyle(itp);
					}
				}
			}
		}
	}
}

void IdmlPlug::parseObjectStyle(const QDomElement& styleElem)
{
	ObjectStyle nstyle;
	nstyle.fillColor = def_fillColor;
	nstyle.strokeColor = def_strokeColor;
	nstyle.fillGradient = "";
	nstyle.gradientFillStart = QPointF(def_gradientX, def_gradientY);
	nstyle.gradientFillLength = def_gradientLen;
	nstyle.gradientFillAngle = def_gradientAngle;
	nstyle.strokeGradient = "";
	nstyle.gradientStrokeStart = QPointF(def_gradientStrokeStartX, def_gradientStrokeStartY);
	nstyle.gradientStrokeLength = def_gradientStrokeLength;
	nstyle.gradientStrokeAngle = def_gradientStrokeAngle;
	nstyle.lineWidth = def_lineWidth;
	nstyle.fillTint = def_fillTint;
	nstyle.strokeTint = def_strokeTint;
	nstyle.Opacity = def_Opacity;
	nstyle.blendMode = def_Blendmode;
	nstyle.parentStyle = "";
	nstyle.Extra = def_Extra;
	nstyle.TExtra = def_TExtra;
	nstyle.BExtra = def_BExtra;
	nstyle.RExtra = def_RExtra;
	nstyle.TextColumnCount = def_TextColumnCount;
	nstyle.TextColumnGutter = def_TextColumnGutter;
	nstyle.TextFlow = def_TextFlow;
	nstyle.LeftLineEnd = def_LeftLineEnd;
	nstyle.RightLineEnd = def_RightLineEnd;
	for (QDomNode itp = styleElem.firstChild(); !itp.isNull(); itp = itp.nextSibling())
	{
		QDomElement itpr = itp.toElement();
		if (itpr.tagName() == "Properties")
		{
			for (QDomNode itpp = itpr.firstChild(); !itpp.isNull(); itpp = itpp.nextSibling())
			{
				QDomElement i = itpp.toElement();
				if (i.tagName() == "BasedOn")
				{
					QString ps = i.text();
					if (ps != "$ID/[None]")
						nstyle.parentStyle = ps;
				}
			}
		}
		if (itpr.tagName() == "TextWrapPreference")
		{
			if (itpr.hasAttribute("TextWrapMode"))
			{
				if (itpr.attribute("TextWrapMode") == "None")
					nstyle.TextFlow = PageItem::TextFlowDisabled;
				else if (itpr.attribute("TextWrapMode") == "BoundingBoxTextWrap")
					nstyle.TextFlow = PageItem::TextFlowUsesBoundingBox;
				else if (itpr.attribute("TextWrapMode") == "Contour")
					nstyle.TextFlow = PageItem::TextFlowUsesFrameShape;
			}
		}
		else if (itpr.tagName() == "TextFramePreference")
		{
			if (itpr.hasAttribute("TextColumnCount"))
				nstyle.TextColumnCount = itpr.attribute("TextColumnCount").toInt();
			if (itpr.hasAttribute("TextColumnGutter"))
				nstyle.TextColumnGutter = itpr.attribute("TextColumnGutter").toDouble();
			if (itpr.hasAttribute("TextColumnFixedWidth"))
				nstyle.TextColumnFixedWidth = itpr.attribute("TextColumnFixedWidth").toDouble();
			for (QDomNode itpp = itpr.firstChild(); !itpp.isNull(); itpp = itpp.nextSibling())
			{
				QDomElement i = itpp.toElement();
				if (i.tagName() == "Properties")
				{
					for (QDomNode it = i.firstChild(); !it.isNull(); it = it.nextSibling())
					{
						QDomElement itx = it.toElement();
						if (itx.tagName() == "InsetSpacing")
						{
							if (itx.attribute("type") == "unit")
								nstyle.Extra = nstyle.TExtra = nstyle.BExtra = nstyle.RExtra = itx.text().toDouble();
							else if (itx.attribute("type") == "list")
							{
								int cc = 0;
								for (QDomNode ity = itx.firstChild(); !ity.isNull(); ity = ity.nextSibling())
								{
									QDomElement itxx = ity.toElement();
									if (itxx.tagName() == "ListItem")
									{
										double val = itxx.text().toDouble();
										if (cc == 0)
											nstyle.Extra = val;
										else if (cc == 1)
											nstyle.TExtra = val;
										else if (cc == 2)
											nstyle.RExtra = val;
										else if (cc == 3)
											nstyle.BExtra = val;
										cc++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (styleElem.hasAttribute("StrokeColor"))
	{
		QString strokeColor = styleElem.attribute("StrokeColor");
		if (colorTranslate.contains(strokeColor))
			nstyle.strokeColor = colorTranslate[strokeColor];
		else
		{
			if (gradientTranslate.contains(strokeColor))
				nstyle.strokeGradient = gradientTranslate[strokeColor];
		}
	}
	if (styleElem.hasAttribute("FillColor"))
	{
		QString fillColor = styleElem.attribute("FillColor");
		if (colorTranslate.contains(fillColor))
			nstyle.fillColor = colorTranslate[fillColor];
		else
		{
			if (gradientTranslate.contains(fillColor))
				nstyle.fillGradient = gradientTranslate[fillColor];
		}
	}
	if (styleElem.hasAttribute("FillTint"))
	{
		int fillShade = styleElem.attribute("FillTint").toInt();
		if (fillShade != -1)
			nstyle.fillTint = fillShade;
	}
	if (styleElem.hasAttribute("StrokeTint"))
	{
		int strokeShade = styleElem.attribute("StrokeTint").toInt();
		if (strokeShade != -1)
			nstyle.strokeTint = strokeShade;
	}
	if (styleElem.hasAttribute("StrokeWeight"))
		nstyle.lineWidth = styleElem.attribute("StrokeWeight", "0").toDouble();
	if (styleElem.hasAttribute("GradientFillStart"))
	{
		QString fillGStart = styleElem.attribute("GradientFillStart");
		ScTextStream Code(&fillGStart, QIODevice::ReadOnly);
		double gstX, gstY;
		Code >> gstX >> gstY;
		nstyle.gradientFillStart = QPointF(gstX, gstY);
	}
	if (styleElem.hasAttribute("GradientFillLength"))
		nstyle.gradientFillLength = styleElem.attribute("GradientFillLength").toDouble();
	if (styleElem.hasAttribute("GradientFillAngle"))
		nstyle.gradientFillAngle = styleElem.attribute("GradientFillAngle").toDouble();
	if (styleElem.hasAttribute("GradientStrokeStart"))
	{
		QString fillGStart = styleElem.attribute("GradientStrokeStart");
		ScTextStream Code(&fillGStart, QIODevice::ReadOnly);
		double gstX, gstY;
		Code >> gstX >> gstY;
		nstyle.gradientStrokeStart = QPointF(gstX, gstY);
	}
	if (styleElem.hasAttribute("GradientStrokeLength"))
		nstyle.gradientStrokeLength = styleElem.attribute("GradientStrokeLength").toDouble();
	if (styleElem.hasAttribute("GradientStrokeAngle"))
		nstyle.gradientStrokeAngle = styleElem.attribute("GradientStrokeAngle").toDouble();
	if (styleElem.hasAttribute("RightLineEnd"))
		nstyle.RightLineEnd = styleElem.attribute("RightLineEnd");
	if (styleElem.hasAttribute("LeftLineEnd"))
		nstyle.LeftLineEnd = styleElem.attribute("LeftLineEnd");
	QString itemName = styleElem.attribute("Self");
	ObjectStyles.insert(itemName, nstyle);
}

void IdmlPlug::parseCharacterStyle(const QDomElement& styleElem)
{
	CharStyle newStyle;
	newStyle.setDefaultStyle(false);
	newStyle.setName(styleElem.attribute("Name").remove("$ID/"));
	newStyle.setParent(CommonStrings::DefaultCharacterStyle);
	QString fontName = m_Doc->itemToolPrefs().textFont;
	QString fontBaseName = "";
	QString fontStyle = styleElem.attribute("FontStyle", "");
	for (QDomNode itp = styleElem.firstChild(); !itp.isNull(); itp = itp.nextSibling())
	{
		QDomElement itpr = itp.toElement();
		if (itpr.tagName() == "Properties")
		{
			for (QDomNode itpp = itpr.firstChild(); !itpp.isNull(); itpp = itpp.nextSibling())
			{
				QDomElement i = itpp.toElement();
				if (i.tagName() == "AppliedFont")
					fontBaseName = i.text();
				else if (i.tagName() == "BasedOn")
				{
					QString parentStyle = i.text().remove("$ID/");
					if (charStyleTranslate.contains(parentStyle))
						parentStyle = charStyleTranslate[parentStyle];
					if (m_Doc->styleExists(parentStyle))
						newStyle.setParent(parentStyle);
				}
			}
		}
	}
	if ((!fontBaseName.isEmpty()) && (!fontStyle.isEmpty()))
		fontName = constructFontName(fontBaseName, fontStyle);
	newStyle.setFont((*m_Doc->AllFonts)[fontName]);
	readCharStyleAttributes(newStyle, styleElem);
	StyleSet<CharStyle> temp;
	temp.create(newStyle);
	m_Doc->redefineCharStyles(temp, false);
	charStyleTranslate.insert(styleElem.attribute("Self").remove("$ID/"), styleElem.attribute("Name").remove("$ID/"));
}

void IdmlPlug::parseParagraphStyle(const QDomElement& styleElem)
{
	ParagraphStyle newStyle;
	newStyle.erase();
	newStyle.setDefaultStyle(false);
	newStyle.setName(styleElem.attribute("Name").remove("$ID/"));
	newStyle.setParent(CommonStrings::DefaultParagraphStyle);
	QString fontName = m_Doc->itemToolPrefs().textFont;
	QString fontBaseName = "";
	QString fontStyle = styleElem.attribute("FontStyle", "");
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	for (QDomNode itp = styleElem.firstChild(); !itp.isNull(); itp = itp.nextSibling())
	{
		QDomElement itpr = itp.toElement();
		if (itpr.tagName() == "Properties")
		{
			for (QDomNode itpp = itpr.firstChild(); !itpp.isNull(); itpp = itpp.nextSibling())
			{
				QDomElement i = itpp.toElement();
				if (i.tagName() == "AppliedFont")
					fontBaseName = i.text();
				else if (i.tagName() == "BasedOn")
				{
					QString parentStyle = i.text().remove("$ID/");
					if (styleTranslate.contains(parentStyle))
						parentStyle = styleTranslate[parentStyle];
					else
					{
						QString pSty = parentStyle.remove("ParagraphStyle/");
						if (styleParents.contains(pSty))
							styleParents[pSty].append(newStyle.name());
						else
							styleParents.insert(pSty, QStringList() << newStyle.name());
					}
					if (m_Doc->styleExists(parentStyle))
						newStyle.setParent(parentStyle);
				}
				else if (i.tagName() == "Leading")
				{
					if (i.attribute("type") == "unit")
					{
						int lead = i.text().toDouble();
						if (lead != 0)
						{
							newStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
							newStyle.setLineSpacing(lead);
						}
					}
				}
				else if (i.tagName() == "TabList")
				{
					QList<ParagraphStyle::TabRecord> tbs;
					newStyle.resetTabValues();
					for (QDomNode tabl = i.firstChild(); !tabl.isNull(); tabl = tabl.nextSibling())
					{
						QDomElement ta = tabl.toElement();
						if (ta.tagName() == "ListItem")
						{
							ParagraphStyle::TabRecord tb;
							for (QDomNode tal = ta.firstChild(); !tal.isNull(); tal = tal.nextSibling())
							{
								QDomElement tab = tal.toElement();
								QString tabVal = tab.text();
								if (tab.tagName() == "Alignment")
								{
									tb.tabType = 0;
									if (tabVal == "LeftAlign")
										tb.tabType = 0;
									else if (tabVal == "CenterAlign")
										tb.tabType = 4;
									else if (tabVal == "RightAlign")
										tb.tabType = 1;
									else if (tabVal == "Spreadsheet")
										tb.tabType = 3;
								}
								else if (tab.tagName() == "Position")
								{
									tb.tabPosition = tabVal.toDouble();
								}
								else if (tab.tagName() == "Leader")
								{
									tb.tabFillChar = tabVal.isEmpty() ? QChar() : tabVal[0];
								}
								else if (tab.tagName() == "AlignmentCharacter")
								{
									if (tb.tabType == 3)
									{
										if (tabVal.startsWith(","))
											tb.tabType = 4;
									}
								}
							}
							tbs.append(tb);

						}
					}
					if (tbs.count() > 0)
						newStyle.setTabValues(tbs);
				}
			}
		}
	}
	if ((!fontBaseName.isEmpty()) && (!fontStyle.isEmpty()))
		fontName = constructFontName(fontBaseName, fontStyle);
	newStyle.charStyle().setFont((*m_Doc->AllFonts)[fontName]);
	readCharStyleAttributes(newStyle.charStyle(), styleElem);
	readParagraphStyleAttributes(newStyle, styleElem);
	StyleSet<ParagraphStyle>tmp;
	tmp.create(newStyle);
	m_Doc->redefineStyles(tmp, false);
	styleTranslate.insert(styleElem.attribute("Self").remove("$ID/"), styleElem.attribute("Name").remove("$ID/"));
	if (styleParents.contains(newStyle.name()))
	{
		QStringList desList = styleParents[newStyle.name()];
		for (int a = 0; a < desList.count(); a++)
		{
			ParagraphStyle old = m_Doc->paragraphStyle(desList[a]);
			old.setParent(newStyle.name());
			StyleSet<ParagraphStyle>tmp2;
			tmp2.create(old);
			m_Doc->redefineStyles(tmp2, false);
		}
	}
}

bool IdmlPlug::parsePreferencesXML(const QDomElement& prElem)
{
	QDomElement prNode;
	QDomDocument prMapDom;
	if (prElem.hasAttribute("src"))
	{
		QByteArray f2;
		m_zip->read(prElem.attribute("src"), f2);
		if (!prMapDom.setContent(f2))
			return false;
		prNode = prMapDom.documentElement();
	}
	else
	{
		if (!prElem.hasChildNodes())
			return false;
		prNode = prElem;
	}
	parsePreferencesXMLNode(prNode);
	return true;
}

void IdmlPlug::parsePreferencesXMLNode(const QDomElement& prNode)
{
	double topMargin = m_Doc->marginsVal().top();
	double leftMargin = m_Doc->marginsVal().left();
	double rightMargin = m_Doc->marginsVal().right();
	double bottomMargin = m_Doc->marginsVal().bottom();
	double pgCols = m_Doc->PageSp;
	double pgGap = m_Doc->PageSpa;
	double bleedTop = m_Doc->bleeds()->top();
	double bleedLeft = m_Doc->bleeds()->left();
	double bleedRight = m_Doc->bleeds()->right();
	double bleedBottom = m_Doc->bleeds()->bottom();
	facingPages = false;
	for (QDomNode n = prNode.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement e = n.toElement();
		if (e.tagName() == "DocumentPreference")
		{
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				docWidth = e.attribute("PageWidth").toDouble();
				docHeight = e.attribute("PageHeight").toDouble();
				bleedTop = e.attribute("DocumentBleedTopOffset").toDouble();
				bleedLeft = e.attribute("DocumentBleedInsideOrLeftOffset").toDouble();
				bleedRight = e.attribute("DocumentBleedOutsideOrRightOffset").toDouble();
				bleedBottom = e.attribute("DocumentBleedBottomOffset").toDouble();
				facingPages = e.attribute("FacingPages","") == "true";
			}
		}
		if (e.tagName() == "MarginPreference")
		{
			topMargin = e.attribute("Top").toDouble();
			leftMargin = e.attribute("Left").toDouble();
			rightMargin = e.attribute("Right").toDouble();
			bottomMargin = e.attribute("Bottom").toDouble();
			pgCols = e.attribute("ColumnCount").toDouble();
			pgGap = e.attribute("ColumnGutter").toDouble();
		}
		if (e.tagName() == "TransparencyDefaultContainerObject")
		{
			for (QDomNode it = e.firstChild(); !it.isNull(); it = it.nextSibling())
			{
				QDomElement itpg = it.toElement();
				for (QDomNode itp = itpg.firstChild(); !itp.isNull(); itp = itp.nextSibling())
				{
					QDomElement itpr = itp.toElement();
					if (itpr.tagName() == "TransparencySetting")
					{
						def_Opacity = 1.0 - (itpr.attribute("Opacity", "100").toDouble() / 100.0);
						def_Blendmode = convertBlendMode(itpr.attribute("BlendMode", "Normal"));
					}
					if (itpr.tagName() == "StrokeTransparencySetting")
					{
						def_strokeOpacity = 1.0 - (itpr.attribute("Opacity", "100").toDouble() / 100.0);
						def_strokeBlendmode = convertBlendMode(itpr.attribute("BlendMode", "Normal"));
					}
					if (itpr.tagName() == "FillTransparencySetting")
					{
						def_fillOpacity = 1.0 - (itpr.attribute("Opacity", "100").toDouble() / 100.0);
						def_fillBlendmode = convertBlendMode(itpr.attribute("BlendMode", "Normal"));
					}
				}
			}
		}
		if (e.tagName() == "PageItemDefault")
		{
			QString strokeColor = e.attribute("StrokeColor");
			if (colorTranslate.contains(strokeColor))
				def_strokeColor = colorTranslate[strokeColor];
			else
			{
				if (gradientTranslate.contains(strokeColor))
				{
					def_strokeGradient = gradientTranslate[strokeColor];
				}
			}
			QString strokeGStart = e.attribute("GradientStrokeStart", "0 0");
			ScTextStream Code2(&strokeGStart, QIODevice::ReadOnly);
			Code2 >> def_gradientStrokeStartX >> def_gradientStrokeStartY;
			def_gradientStrokeLength = e.attribute("GradientStrokeLength", "0").toDouble();
			def_gradientStrokeAngle = e.attribute("GradientStrokeAngle", "0").toDouble();
			int strokeShade = e.attribute("StrokeTint", "100").toInt();
			if (strokeShade != -1)
				def_strokeTint = strokeShade;
			else
				def_strokeTint = 100;
			QString fillColor = e.attribute("FillColor");
			if (colorTranslate.contains(fillColor))
				def_fillColor = colorTranslate[fillColor];
			else
			{
				if (gradientTranslate.contains(fillColor))
				{
					def_fillGradient = gradientTranslate[fillColor];
				}
			}
			QString fillGStart = e.attribute("GradientFillStart", "0 0");
			ScTextStream Code(&fillGStart, QIODevice::ReadOnly);
			Code >> def_gradientX >> def_gradientY;
			def_gradientLen = e.attribute("GradientFillLength", "0").toDouble();
			def_gradientAngle = e.attribute("GradientFillAngle", "0").toDouble();
			int fillShade = e.attribute("FillTint", "100").toInt();
			if (fillShade != -1)
				def_fillTint = fillShade;
			else
				def_fillTint = 100;
			def_lineWidth = e.attribute("StrokeWeight", "0").toDouble();
			if (e.hasAttribute("RightLineEnd"))
				def_RightLineEnd = e.attribute("RightLineEnd");
			if (e.hasAttribute("LeftLineEnd"))
				def_LeftLineEnd = e.attribute("LeftLineEnd");
		}
		if (e.tagName() == "TextWrapPreference")
		{
			if (e.attribute("TextWrapMode") == "None")
				def_TextFlow = PageItem::TextFlowDisabled;
			else if (e.attribute("TextWrapMode") == "BoundingBoxTextWrap")
				def_TextFlow = PageItem::TextFlowUsesBoundingBox;
			else if (e.attribute("TextWrapMode") == "Contour")
				def_TextFlow = PageItem::TextFlowUsesFrameShape;
		}
		if (e.tagName() == "TextFramePreference")
		{
			if (e.hasAttribute("TextColumnCount"))
				def_TextColumnCount = e.attribute("TextColumnCount").toInt();
			if (e.hasAttribute("TextColumnGutter"))
				def_TextColumnGutter = e.attribute("TextColumnGutter").toDouble();
			if (e.hasAttribute("TextColumnFixedWidth"))
				def_TextColumnFixedWidth = e.attribute("TextColumnFixedWidth").toDouble();
			for (QDomNode itpp = e.firstChild(); !itpp.isNull(); itpp = itpp.nextSibling())
			{
				QDomElement i = itpp.toElement();
				if (i.tagName() == "Properties")
				{
					for (QDomNode it = i.firstChild(); !it.isNull(); it = it.nextSibling())
					{
						QDomElement itx = it.toElement();
						if (itx.tagName() == "InsetSpacing")
						{
							if (itx.attribute("type") == "unit")
								def_Extra = def_TExtra = def_BExtra = def_RExtra = itx.text().toDouble();
							else if (itx.attribute("type") == "list")
							{
								int cc = 0;
								for (QDomNode ity = itx.firstChild(); !ity.isNull(); ity = ity.nextSibling())
								{
									QDomElement itxx = ity.toElement();
									if (itxx.tagName() == "ListItem")
									{
										double val = itxx.text().toDouble();
										if (cc == 0)
											def_Extra = val;
										else if (cc == 1)
											def_TExtra = val;
										else if (cc == 2)
											def_RExtra = val;
										else if (cc == 3)
											def_BExtra = val;
										cc++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (importerFlags & LoadSavePlugin::lfCreateDoc)
	{
		m_Doc->setPage(docWidth, docHeight, topMargin, leftMargin, rightMargin, bottomMargin, pgCols, pgGap, false, facingPages);
		m_Doc->setPageSize("Custom");
		m_Doc->bleeds()->set(bleedTop, bleedLeft, bleedBottom, bleedRight);
		m_Doc->currentPage()->setSize("Custom");
		m_Doc->currentPage()->setInitialHeight(docHeight);
		m_Doc->currentPage()->setInitialWidth(docWidth);
		m_Doc->currentPage()->setHeight(docHeight);
		m_Doc->currentPage()->setWidth(docWidth);
		m_Doc->currentPage()->initialMargins.setTop(topMargin);
		m_Doc->currentPage()->initialMargins.setBottom(bottomMargin);
		m_Doc->currentPage()->initialMargins.setLeft(leftMargin);
		m_Doc->currentPage()->initialMargins.setRight(rightMargin);
		m_Doc->reformPages(true);
		baseX = m_Doc->currentPage()->xOffset();
		baseY = m_Doc->currentPage()->yOffset() + m_Doc->currentPage()->height() / 2.0;
	}
}

bool IdmlPlug::parseSpreadXML(const QDomElement& spElem)
{
	QDomElement spNode;
	QDomDocument spMapDom;
	if (spElem.hasAttribute("src"))
	{
		QByteArray f2;
		m_zip->read(spElem.attribute("src"), f2);
		if (!spMapDom.setContent(f2))
			return false;
		spNode = spMapDom.documentElement();
	}
	else
	{
		if (!spElem.hasChildNodes())
			return false;
		spNode = spElem;
	}
	parseSpreadXMLNode(spNode);
	return true;
}

void IdmlPlug::parseSpreadXMLNode(const QDomElement& spNode)
{
	for (QDomNode n = spNode.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement e = n.toElement();
		if (e.tagName() == "Spread")
		{
			for (QDomNode sp = e.firstChild(); !sp.isNull(); sp = sp.nextSibling())
			{
				QDomElement spe = sp.toElement();
				if (spe.tagName() == "Page")
				{
					if ((importerFlags & LoadSavePlugin::lfCreateDoc) && (!firstPage))
					{
						m_Doc->addPage(pagecount);
						m_Doc->currentPage()->setMasterPageNameNormal();
						m_Doc->currentPage()->setSize("Custom");
						m_Doc->currentPage()->setInitialHeight(docHeight);
						m_Doc->currentPage()->setInitialWidth(docWidth);
						m_Doc->currentPage()->setHeight(docHeight);
						m_Doc->currentPage()->setWidth(docWidth);
						m_Doc->view()->addPage(pagecount, true);
						pagecount++;
					}
					baseX = m_Doc->currentPage()->xOffset();
					baseY = m_Doc->currentPage()->yOffset() + m_Doc->currentPage()->height() / 2.0;
					firstPage = false;
					if ((importerFlags & LoadSavePlugin::lfCreateDoc) && spe.hasAttribute("AppliedMaster"))
					{
						QString mSpr = spe.attribute("AppliedMaster");
						if (masterSpreads.contains(mSpr))
						{
							QString mp = CommonStrings::trMasterPageNormal;
							if (facingPages)
							{
								if ((pagecount % 2 == 0) && (masterSpreads[mSpr].count() > 0))
									mp = mSpr + "_" + masterSpreads[mSpr][0];
								if ((pagecount % 2 == 1) && (masterSpreads[mSpr].count() > 1))
									mp = mSpr + "_" + masterSpreads[mSpr][1];
							}
							else
							{
								if ((masterSpreads[mSpr].count() > 0))
									mp = mSpr + "_" + masterSpreads[mSpr][0];
							}
							m_Doc->applyMasterPage(mp, m_Doc->currentPageNumber());
						}
					}
					for (QDomNode pageNode = spe.firstChild(); !pageNode.isNull(); pageNode = pageNode.nextSibling())
					{
						QDomElement pageElement = pageNode.toElement();
						if (pageElement.tagName() == "Guide" && pageElement.hasAttribute("Location") && pageElement.hasAttribute("Orientation"))
						{
							bool convOk = false;
							double location = pageElement.attribute("Location").toDouble(&convOk);
							if (!convOk)
								continue;
							QString orientation = pageElement.attribute("Orientation");
							if (orientation == "Horizontal")
								m_Doc->currentPage()->guides.addHorizontal(location, GuideManagerCore::Standard);
							else if (pageElement.attribute("Orientation") == "Vertical")
								m_Doc->currentPage()->guides.addVertical(location, GuideManagerCore::Standard);
						}
					}
				}
			}
			if ((facingPages) && (pagecount % 2 == 0))
			{
				baseX = m_Doc->currentPage()->xOffset() + m_Doc->currentPage()->width();
				baseY = m_Doc->currentPage()->yOffset() + m_Doc->currentPage()->height() / 2.0;
			}
			if (!facingPages)
			{
				baseX = m_Doc->currentPage()->xOffset() + m_Doc->currentPage()->width() / 2.0;
				baseY = m_Doc->currentPage()->yOffset() + m_Doc->currentPage()->height() / 2.0;
			}
			for (QDomNode sp = e.firstChild(); !sp.isNull(); sp = sp.nextSibling())
			{
				QDomElement spe = sp.toElement();
				if ((spe.tagName() == "Rectangle") || (spe.tagName() == "Oval") || (spe.tagName() == "GraphicLine") || (spe.tagName() == "Polygon") || (spe.tagName() == "TextFrame") || (spe.tagName() == "Group") || (spe.tagName() == "Button"))
				{
					QList<PageItem*> el = parseItemXML(spe);
					for (int ec = 0; ec < el.count(); ++ec)
					{
						m_Doc->Items->append(el.at(ec));
						Elements.append(el.at(ec));
					}
				}
			}
		}
		else if (e.tagName() == "MasterSpread")
		{
			m_Doc->setMasterPageMode(true);
			QString pageNam = e.attribute("Self");
			QStringList pages;
			ScPage *oldCur = m_Doc->currentPage();
			for (QDomNode sp = e.firstChild(); !sp.isNull(); sp = sp.nextSibling())
			{
				QDomElement spe = sp.toElement();
				if (spe.tagName() == "Page")
				{
					QString itemTrans = spe.attribute("ItemTransform");
					ScTextStream list(&itemTrans, QIODevice::ReadOnly);
					double a, b, c, d, e1, f;
					list >> a >> b >> c >> d >> e1 >> f;
					/* Adding the values directly */
					QTransform transformation(a, b, c, d, e1, f);
					ScPage *addedPage = m_Doc->addMasterPage(mpagecount, pageNam + "_" + spe.attribute("Self"));
					m_Doc->setCurrentPage(addedPage);
					pages.append(spe.attribute("Self"));
					addedPage->clearMasterPageName();
					m_Doc->view()->addPage(mpagecount, true);
					baseX = addedPage->xOffset();
					baseY = addedPage->yOffset() + addedPage->height() / 2.0;
					if (!facingPages)
						baseX = addedPage->xOffset() + addedPage->width() / 2.0;
					else
						baseX = addedPage->xOffset() - transformation.dx();
					for (QDomNode spp = e.firstChild(); !spp.isNull(); spp = spp.nextSibling())
					{
						QDomElement spe = spp.toElement();
						if ((spe.tagName() == "Rectangle") || (spe.tagName() == "Oval") || (spe.tagName() == "GraphicLine") || (spe.tagName() == "Polygon") || (spe.tagName() == "TextFrame") || (spe.tagName() == "Group") || (spe.tagName() == "Button"))
						{
							QList<PageItem*> el = parseItemXML(spe);
							for (int ec = 0; ec < el.count(); ++ec)
							{
								PageItem* ite = el.at(ec);
								int pgi = m_Doc->OnPage(ite);
								if (pgi != -1)
								{
									m_Doc->Items->append(ite);
									Elements.append(ite);
								}
							}
						}
					}
					mpagecount++;
				}
			}
			masterSpreads.insert(pageNam, pages);
			m_Doc->setCurrentPage(oldCur);
			m_Doc->setMasterPageMode(false);
		}
	}
}

QList<PageItem*> IdmlPlug::parseItemXML(const QDomElement& itElem, const QTransform& pTrans)
{
	QList<PageItem*> GElements;
	FPointArray GCoords;
	GCoords.resize(0);
	GCoords.svgInit();
	QString itemTrans = itElem.attribute("ItemTransform");
	ScTextStream list(&itemTrans, QIODevice::ReadOnly);
	double a, b, c, d, e, f;
	list >> a >> b >> c >> d >> e >> f;
	/* Adding the values directly */
	QTransform transformation(a, b, c, d, e, f);
	QString itemName = itElem.attribute("Self");
	QString fillColor = def_fillColor;
	QString fillGradient = "";
	double gstX = def_gradientX;
	double gstY = def_gradientY;
	double gLen = def_gradientLen;
	double gAngle = def_gradientAngle;
	int fillGradientTyp = 6;
	QString strokeColor = def_strokeColor;
	QString strokeGradient = "";
	double gstSX = def_gradientStrokeStartX;
	double gstSY = def_gradientStrokeStartY;
	double gSLen = def_gradientStrokeLength;
	double gSAngle = def_gradientStrokeAngle;
	int strokeGradientTyp = 6;
	double lineWidth = def_lineWidth;
	int fillShade = def_fillTint;
	int strokeShade = def_strokeTint;
	double Opacity = def_Opacity;
	int blendMode = def_Blendmode;
	double Extra = def_Extra;
	double TExtra = def_TExtra;
	double BExtra = def_BExtra;
	double RExtra = def_RExtra;
	int TextColumnCount = def_TextColumnCount;
	double TextColumnGutter = def_TextColumnGutter;
	//double TextColumnFixedWidth = def_TextColumnFixedWidth;
	QString LeftLineEnd = def_LeftLineEnd;
	QString RightLineEnd = def_RightLineEnd;
	QString imageFit = "None";
	PageItem::TextFlowMode textFlow = def_TextFlow;
	if (itElem.hasAttribute("AppliedObjectStyle"))
	{
		QString os = itElem.attribute("AppliedObjectStyle");
		if (os != "n")
		{
			ObjectStyle nstyle;
			nstyle.fillColor = def_fillColor;
			nstyle.strokeColor = def_strokeColor;
			nstyle.fillGradient = "";
			nstyle.gradientFillStart = QPointF(def_gradientX, def_gradientY);
			nstyle.gradientFillLength = def_gradientLen;
			nstyle.gradientFillAngle = def_gradientAngle;
			nstyle.strokeGradient = "";
			nstyle.gradientStrokeStart = QPointF(def_gradientStrokeStartX, def_gradientStrokeStartY);
			nstyle.gradientStrokeAngle = def_gradientStrokeAngle;
			nstyle.gradientStrokeLength = def_gradientStrokeLength;
			nstyle.lineWidth = def_lineWidth;
			nstyle.fillTint = def_fillTint;
			nstyle.strokeTint = def_strokeTint;
			nstyle.Opacity = def_Opacity;
			nstyle.blendMode = def_Blendmode;
			nstyle.Extra = def_Extra;
			nstyle.TExtra = def_TExtra;
			nstyle.BExtra = def_BExtra;
			nstyle.RExtra = def_RExtra;
			nstyle.TextColumnCount = def_TextColumnCount;
			nstyle.TextColumnGutter = def_TextColumnGutter;
			nstyle.TextColumnFixedWidth = def_TextColumnFixedWidth;
			nstyle.TextFlow = def_TextFlow;
			nstyle.parentStyle = "";
			resolveObjectStyle(nstyle, os);
			fillColor = nstyle.fillColor;
			if (!nstyle.fillGradient.isEmpty())
			{
				fillGradient = nstyle.fillGradient;
				fillGradientTyp = gradientTypeMap[fillColor];
			}
			gstX = nstyle.gradientFillStart.x();
			gstY = nstyle.gradientFillStart.y();
			gLen = nstyle.gradientFillLength;
			gAngle = nstyle.gradientFillAngle;
			strokeColor = nstyle.strokeColor;
			if (!nstyle.strokeGradient.isEmpty())
			{
				strokeGradient = nstyle.strokeGradient;
				strokeGradientTyp = gradientTypeMap[strokeColor];
			}
			gstSX = nstyle.gradientStrokeStart.x();
			gstSY = nstyle.gradientStrokeStart.y();
			gSLen = nstyle.gradientStrokeLength;
			gSAngle = nstyle.gradientStrokeAngle;
			lineWidth = nstyle.lineWidth;
			fillShade = nstyle.fillTint;
			strokeShade = nstyle.strokeTint;
			Opacity = nstyle.Opacity;
			blendMode = nstyle.blendMode;
			Extra = nstyle.Extra;
			TExtra = nstyle.TExtra;
			BExtra = nstyle.BExtra;
			RExtra = nstyle.RExtra;
			TextColumnCount = nstyle.TextColumnCount;
			TextColumnGutter = nstyle.TextColumnGutter;
		//	TextColumnFixedWidth = nstyle.TextColumnFixedWidth;
			textFlow = nstyle.TextFlow;
			LeftLineEnd = nstyle.LeftLineEnd;
			RightLineEnd = nstyle.RightLineEnd;
		}
	}
	if (itElem.hasAttribute("FillColor"))
	{
		fillColor = itElem.attribute("FillColor");
		if (colorTranslate.contains(fillColor))
			fillColor = colorTranslate[fillColor];
		else
		{
			if (gradientTranslate.contains(fillColor))
			{
				fillGradientTyp = gradientTypeMap[fillColor];
				fillGradient = gradientTranslate[fillColor];
			}
		}
	}
	if (itElem.hasAttribute("GradientFillStart"))
	{
		QString fillGStart = itElem.attribute("GradientFillStart");
		ScTextStream Code(&fillGStart, QIODevice::ReadOnly);
		Code >> gstX >> gstY;
		gLen = itElem.attribute("GradientFillLength").toDouble();
		gAngle = itElem.attribute("GradientFillAngle").toDouble();
	}
	if (itElem.hasAttribute("StrokeColor"))
	{
		strokeColor = itElem.attribute("StrokeColor");
		if (colorTranslate.contains(strokeColor))
			strokeColor = colorTranslate[strokeColor];
		else
		{
			if (gradientTranslate.contains(strokeColor))
			{
				strokeGradientTyp = gradientTypeMap[strokeColor];
				strokeGradient = gradientTranslate[strokeColor];
			}
		}
	}
	if (itElem.hasAttribute("GradientStrokeStart"))
	{
		QString fillGStart = itElem.attribute("GradientStrokeStart");
		ScTextStream Code(&fillGStart, QIODevice::ReadOnly);
		Code >> gstSX >> gstSY;
		gSLen = itElem.attribute("GradientStrokeLength").toDouble();
		gSAngle = itElem.attribute("GradientStrokeAngle").toDouble();
	}
	if (itElem.hasAttribute("StrokeWeight"))
		lineWidth = itElem.attribute("StrokeWeight").toDouble();
	if (itElem.hasAttribute("FillTint"))
	{
		if (itElem.attribute("FillTint").toInt() != -1)
			fillShade = itElem.attribute("FillTint").toInt();
	}
	if (fillShade < 0)
		fillShade = 100;
	if (itElem.hasAttribute("StrokeTint"))
	{
		if (itElem.attribute("StrokeTint").toInt() != -1)
			strokeShade = itElem.attribute("StrokeTint").toInt();
	}
	if (strokeShade < 0)
		strokeShade = 100;
	if (itElem.hasAttribute("RightLineEnd"))
		RightLineEnd = itElem.attribute("RightLineEnd");
	if (itElem.hasAttribute("LeftLineEnd"))
		LeftLineEnd = itElem.attribute("LeftLineEnd");
	QString forLayer = itElem.attribute("ItemLayer");
	if (layerTranslate.contains(forLayer))
		forLayer = layerTranslate[forLayer];
	else
		forLayer = m_Doc->layerName(0);
	int layerNum = 0;
	ScLayers::iterator itend = m_Doc->Layers.end();
	ScLayers::iterator it;
	for (it = m_Doc->Layers.begin(); it != itend; ++it)
	{
		if (it->Name == forLayer)
		{
			layerNum = it->ID;
			break;
		}
	}
	bool isOpen = false;
	bool isGroup = false;
	bool realGroup = false;
	bool isImage = false;
	bool isPathText = false;
	if (itElem.tagName() == "Group")
		realGroup = true;
	QString imageType = "";
	QByteArray imageData = "";
	QString imageFileName = "";
	QTransform imageTransform;
	double imageDX = 0;
	double imageDY = 0;
	QString storyForPath = "";
	int pathTextType = 0;
	double pathTextStart = 0;
	for (QDomNode it = itElem.firstChild(); !it.isNull(); it = it.nextSibling())
	{
		QDomElement ite = it.toElement();
		if (ite.tagName() == "Properties")
		{
			for (QDomNode itp = ite.firstChild(); !itp.isNull(); itp = itp.nextSibling())
			{
				QDomElement itpg = itp.toElement();
				if (itpg.tagName() == "PathGeometry")
				{
					for (QDomNode itg = itpg.firstChild(); !itg.isNull(); itg = itg.nextSibling())
					{
						QDomElement itgg = itg.toElement();
						if (itgg.tagName() == "GeometryPathType")
						{
							isOpen = (itgg.attribute("PathOpen") == "true");
							for (QDomNode itpp = itgg.firstChild(); !itpp.isNull(); itpp = itpp.nextSibling())
							{
								QDomElement itpa = itpp.toElement();
								if (itpa.tagName() == "PathPointArray")
								{
									bool firstPoint = true;
									QPointF firstBezPoint;
									QPointF firstAncPoint;
									QList<QPointF> pointList;
									for (QDomNode itpap = itpa.firstChild(); !itpap.isNull(); itpap = itpap.nextSibling())
									{
										QDomElement itpo = itpap.toElement();
										if (itpo.tagName() == "PathPointType")
										{
											double x1, y1, x2, y2, x3, y3;
											QString anchor = itpo.attribute("Anchor");
											QString lDir = itpo.attribute("LeftDirection");
											QString rDir = itpo.attribute("RightDirection");
											ScTextStream an(&anchor, QIODevice::ReadOnly);
											an >> x1 >> y1;
											QPointF aP = QPointF(x1, y1);
											ScTextStream lr(&lDir, QIODevice::ReadOnly);
											lr >> x2 >> y2;
											QPointF lP = QPointF(x2, y2);
											ScTextStream rr(&rDir, QIODevice::ReadOnly);
											rr >> x3 >> y3;
											QPointF rP = QPointF(x3, y3);

											if (firstPoint)
											{
												firstBezPoint = lP;
												firstAncPoint = aP;
												pointList.append(aP);
												pointList.append(rP);
												firstPoint = false;
											}
											else
											{
												if (itElem.tagName() == "GraphicLine")
												{
													pointList.append(lP);
													pointList.append(aP);
												}
												else
												{
													pointList.append(lP);
													pointList.append(aP);
													pointList.append(rP);
												}
											}
										}
									}
									if (itElem.tagName() == "GraphicLine")
									{
										if (pointList.count() > 1)
										{
											GCoords.svgMoveTo(pointList[0].x(), pointList[0].y());
											QPointF p1 = pointList[1];
											QPointF p2 = pointList[2];
											QPointF p3 = pointList[3];
											GCoords.svgCurveToCubic(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
										}
									}
									else
									{
										if (isOpen)
										{
											pointList.removeLast();
										}
										else
										{
											pointList.append(firstBezPoint);
											pointList.append(firstAncPoint);
										}
										if (pointList.count() > 1)
										{
											GCoords.svgMoveTo(pointList[0].x(), pointList[0].y());
											for (int a = 1; a < pointList.count(); a += 3)
											{
												QPointF p1 = pointList[a];
												QPointF p2 = pointList[a+1];
												QPointF p3 = pointList[a+2];
												GCoords.svgCurveToCubic(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
											}
										}
									}
								}
							}
							if (!isOpen)
								GCoords.svgClosePath();
						}
					}
				}
			}
		}
		else if ((ite.tagName() == "Rectangle") || (ite.tagName() == "Oval") || (ite.tagName() == "GraphicLine") || (ite.tagName() == "Polygon") || (ite.tagName() == "TextFrame") || (ite.tagName() == "Group") || (ite.tagName() == "Button"))
		{
			isGroup = true;
			QList<PageItem*> el = parseItemXML(ite, transformation * pTrans);
			for (int ec = 0; ec < el.count(); ++ec)
			{
				GElements.append(el.at(ec));
			}
		}
		else if (ite.tagName() == "FrameFittingOption")
		{
			if (ite.hasAttribute("FittingOnEmptyFrame"))
				imageFit = ite.attribute("FittingOnEmptyFrame");
			if (ite.hasAttribute("LeftCrop"))
				imageDX = ite.attribute("LeftCrop").toDouble();
			if (ite.hasAttribute("TopCrop"))
				imageDY = ite.attribute("TopCrop").toDouble();
		}
		else if (ite.tagName() == "TransparencySetting")
		{
			for (QDomNode itp = ite.firstChild(); !itp.isNull(); itp = itp.nextSibling())
			{
				QDomElement itpg = itp.toElement();
				if (itpg.tagName() == "BlendingSetting")
				{
					Opacity = 1.0 - (itpg.attribute("Opacity", "100").toDouble() / 100.0);
					blendMode = convertBlendMode(itpg.attribute("BlendMode", "Normal"));
				}
			}
		}
		else if (ite.tagName() == "TextWrapPreference")
		{
			if (ite.attribute("TextWrapMode") == "None")
				textFlow = PageItem::TextFlowDisabled;
			else if (ite.attribute("TextWrapMode") == "BoundingBoxTextWrap")
				textFlow = PageItem::TextFlowUsesBoundingBox;
			else if (ite.attribute("TextWrapMode") == "Contour")
				textFlow = PageItem::TextFlowUsesFrameShape;
		}
		else if (ite.tagName() == "TextFramePreference")
		{
			if (ite.hasAttribute("TextColumnCount"))
				TextColumnCount = ite.attribute("TextColumnCount").toInt();
			if (ite.hasAttribute("TextColumnGutter"))
				TextColumnGutter = ite.attribute("TextColumnGutter").toDouble();
		//	if (ite.hasAttribute("TextColumnFixedWidth"))
		//		TextColumnFixedWidth = ite.attribute("TextColumnFixedWidth").toDouble();
			for (QDomNode itpp = ite.firstChild(); !itpp.isNull(); itpp = itpp.nextSibling())
			{
				QDomElement i = itpp.toElement();
				if (i.tagName() == "Properties")
				{
					for (QDomNode it = i.firstChild(); !it.isNull(); it = it.nextSibling())
					{
						QDomElement itx = it.toElement();
						if (itx.tagName() == "InsetSpacing")
						{
							if (itx.attribute("type") == "unit")
								Extra = TExtra = BExtra = RExtra = itx.text().toDouble();
							else if (itx.attribute("type") == "list")
							{
								int cc = 0;
								for (QDomNode ity = itx.firstChild(); !ity.isNull(); ity = ity.nextSibling())
								{
									QDomElement itxx = ity.toElement();
									if (itxx.tagName() == "ListItem")
									{
										double val = itxx.text().toDouble();
										if (cc == 0)
											Extra = val;
										else if (cc == 1)
											TExtra = val;
										else if (cc == 2)
											RExtra = val;
										else if (cc == 3)
											BExtra = val;
										cc++;
									}
								}
							}
						}
					}
				}
			}
		}
		else if ((ite.tagName() == "Image") || (ite.tagName() == "EPS") || (ite.tagName() == "PDF") || (ite.tagName() == "PICT"))
		{
			imageType = ite.attribute("ImageTypeName");
			isImage = true;
			QString imageTrans = ite.attribute("ItemTransform", "1 0 0 1 0 0");
			ScTextStream list(&imageTrans, QIODevice::ReadOnly);
			double a, b, c, d, e, f;
			list >> a >> b >> c >> d >> e >> f;
			imageTransform = QTransform(a, b, c, d, e, f) * transformation;
			for (QDomNode itp = ite.firstChild(); !itp.isNull(); itp = itp.nextSibling())
			{
				QDomElement itpg = itp.toElement();
				if (itpg.tagName() == "Properties")
					imageData = QByteArray::fromBase64(getNodeValue(itpg, "Contents").toLatin1());
				if (itpg.tagName() == "Link")
				{
					if (itpg.hasAttribute("LinkResourceURI"))
						imageFileName = itpg.attribute("LinkResourceURI");
				}
			}
		}
		else if (ite.tagName() == "WMF")
		{
			qDebug() << "WMF";
		}
		else if (ite.tagName() == "ImportedPage")
		{
			qDebug() << "ImportedPage";
		}
		else if (ite.tagName() == "TextPath")
		{
			isPathText = true;
			storyForPath = ite.attribute("ParentStory");
			if (ite.attribute("PathEffect") == "RainbowPathEffect")
				pathTextType = 0;
			else if (ite.attribute("PathEffect") == "StairStepPathEffect")
				pathTextType = 1;
			else if (ite.attribute("PathEffect") == "SkewPathEffect")
				pathTextType = 2;
			else if (ite.attribute("PathEffect") == "RibbonPathEffect")			// not implemented in PathText yet
				pathTextType = 0;
			else if (ite.attribute("PathEffect") == "GravityPathEffect")		// not implemented in PathText yet
				pathTextType = 0;
			if (ite.hasAttribute("StartBracket"))
				pathTextStart = ite.attribute("StartBracket").toDouble();
		}
	}
	if (!GCoords.empty())
	{
		int z;
		QTransform finalMat = transformation * pTrans;
		double scX, scY, rot, dx, dy;
		getTransformValuesFromMatrix(finalMat, scX, scY, rot, dx, dy);
		if ((finalMat.m11() < 0) && (finalMat.m12() == 0) && (finalMat.m21() == 0))
		{
			QLineF line = QLineF(0.0, 0.0, 1.0, 0.0);
			line.setAngle(rot);
			QTransform matrix;
			matrix.scale(-1, 0);
			line = matrix.map(line);
			rot = line.angle();
			scX *= -1;
		}
		if ((finalMat.m22() < 0) && (finalMat.m12() == 0) && (finalMat.m21() == 0))
		{
			scY *= -1;
		}
		FPoint grOffset(getMinClipF(&GCoords));
		GCoords.map(finalMat);
		if (isGroup)
		{
			QString pre = "";
			FPointArray gClip;
			if (!realGroup)
			{
				pre = "Group_";
				if (!fillGradient.isEmpty())
					fillColor = CommonStrings::None;
				if (itElem.tagName() == "TextFrame")
				{
					z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, fillColor, strokeColor);
					PageItem* item = m_Doc->Items->at(z);
					QString story = itElem.attribute("ParentStory");
					if (!storyMap.contains(story))
						storyMap.insert(story, item);
					if (itElem.hasAttribute("NextTextFrame"))
					{
						if (itElem.attribute("NextTextFrame") != "n")
							frameLinks.insert(item, itElem.attribute("NextTextFrame"));
					}
					frameTargets.insert(itemName, item);
					item->setTextToFrameDistLeft(Extra);
					item->setTextToFrameDistTop(TExtra);
					item->setTextToFrameDistRight(RExtra);
					item->setTextToFrameDistBottom(BExtra);
					item->setColumns(TextColumnCount);
					item->setColumnGap(TextColumnGutter);
				}
				else if (isPathText)
				{
					z = m_Doc->itemAdd(PageItem::PathText, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, CommonStrings::None, strokeColor);
					if (!storyMap.contains(storyForPath))
						storyMap.insert(storyForPath, m_Doc->Items->at(z));
					PageItem* item = m_Doc->Items->at(z);
					item->setPathTextType(pathTextType);
					item->setTextToFrameDistLeft(pathTextStart);
				}
				else if (isImage)
				{
					z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, fillColor, strokeColor);
				}
				else
				{
					if (isOpen)
						z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, fillColor, strokeColor);
					else
						z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, fillColor, strokeColor);
				}
				PageItem* item = m_Doc->Items->at(z);
				item->PoLine = GCoords.copy();
				double dx = 0;
				double dy = 0;
				if (rot != 0)
				{
					QTransform mr;
					FPoint grOffset2(getMinClipF(&item->PoLine));
					mr.translate(-grOffset2.x(), -grOffset2.y());
					mr.rotate(rot);
					mr.translate(grOffset2.x(), grOffset2.y());
					item->PoLine.map(mr);
					FPoint grOffset3(getMinClipF(&item->PoLine));
					dx = grOffset2.x() - grOffset3.x();
					dy = grOffset2.y() - grOffset3.y();
				}
				item->ClipEdited = true;
				item->FrameType = 3;
				item->setFillColor(fillColor);
				item->setLineColor(strokeColor);
				item->setFillShade(fillShade);
				item->setLineShade(strokeShade);
				item->setFillEvenOdd(false);
				if (!fillGradient.isEmpty())
				{
					QLineF gradientVector = QLineF(gstX, gstY, gstX+1, gstY);
					gradientVector.setLength(gLen);
					gradientVector.setAngle(gAngle);
					gradientVector.translate(-grOffset.x(), -grOffset.y());
					item->fill_gradient = m_Doc->docGradients[fillGradient];
					item->setGradientVector(gradientVector.x1(), gradientVector.y1(), gradientVector.x2(), gradientVector.y2(), gradientVector.x1(), gradientVector.y1(), 1, 0);
					item->setGradient(fillGradient);
					item->setGradientType(fillGradientTyp);
				}
				if (!strokeGradient.isEmpty())
				{
					QLineF gradientVector = QLineF(gstSX, gstSY, gstSX+1, gstSY);
					gradientVector.setLength(gSLen);
					gradientVector.setAngle(gSAngle);
					gradientVector.translate(-grOffset.x(), -grOffset.y());
					item->stroke_gradient = m_Doc->docGradients[strokeGradient];
					item->setStrokeGradientVector(gradientVector.x1(), gradientVector.y1(), gradientVector.x2(), gradientVector.y2(), gradientVector.x1(), gradientVector.y1(), 1, 0);
					item->setStrokeGradient(strokeGradient);
					item->setStrokeGradientType(strokeGradientTyp);
				}
				FPoint wh = getMaxClipF(&item->PoLine);
				item->setWidthHeight(wh.x(),wh.y());
				item->setTextFlowMode(textFlow);
				m_Doc->adjustItemSize(item);
				item->setRotation(-rot, true);
				item->moveBy(dx, dy, true);
				if (isPathText)
				{
					if (isOpen)
					{
						if (scX < 0)
						{
							item->PoLine.reverse();
						}
					}
					else
					{
						if (scX > 0)
						{
							double totalCurveLen = 0;
							for (int segs = 0; segs < item->PoLine.size()-3; segs += 4)
							{
								totalCurveLen += item->PoLine.lenPathSeg(segs);
							}
							item->setTextToFrameDistLeft(totalCurveLen - pathTextStart);
							item->PoLine.reverse();
						}
					}
				}
				item->OldB2 = item->width();
				item->OldH2 = item->height();
				if (item->isPolyLine())
				{
					if (LeftLineEnd != "None")
					{
						if (LeftLineEnd == "SimpleArrowHead")
							item->setStartArrowIndex(2);
						else if (LeftLineEnd == "SimpleWideArrowHead")
							item->setStartArrowIndex(2);
						else if (LeftLineEnd == "TriangleArrowHead")
							item->setStartArrowIndex(8);
						else if (LeftLineEnd == "TriangleWideArrowHead")
							item->setStartArrowIndex(8);
						else if (LeftLineEnd == "BarbedArrowHead")
							item->setStartArrowIndex(23);
						else if (LeftLineEnd == "CurvedArrowHead")
							item->setStartArrowIndex(26);
						else if (LeftLineEnd == "CircleArrowHead")
							item->setStartArrowIndex(17);
						else if (LeftLineEnd == "CircleSolidArrowHead")
							item->setStartArrowIndex(17);
						else if (LeftLineEnd == "Square-ArrowHead")
							item->setStartArrowIndex(5);
						else if (LeftLineEnd == "SquareSolid-ArrowHead")
							item->setStartArrowIndex(5);
						else if (LeftLineEnd == "BarArrowHead")
							item->setStartArrowIndex(35);
					}
					if (RightLineEnd != "None")
					{
						if (RightLineEnd == "SimpleArrowHead")
							item->setEndArrowIndex(2);
						else if (RightLineEnd == "SimpleWideArrowHead")
							item->setEndArrowIndex(2);
						else if (RightLineEnd == "TriangleArrowHead")
							item->setEndArrowIndex(8);
						else if (RightLineEnd == "TriangleWideArrowHead")
							item->setEndArrowIndex(8);
						else if (RightLineEnd == "BarbedArrowHead")
							item->setEndArrowIndex(23);
						else if (RightLineEnd == "CurvedArrowHead")
							item->setEndArrowIndex(26);
						else if (RightLineEnd == "CircleArrowHead")
							item->setEndArrowIndex(17);
						else if (RightLineEnd == "CircleSolidArrowHead")
							item->setEndArrowIndex(17);
						else if (RightLineEnd == "Square-ArrowHead")
							item->setEndArrowIndex(7);
						else if (RightLineEnd == "SquareSolid-ArrowHead")
							item->setEndArrowIndex(7);
						else if (RightLineEnd == "BarArrowHead")
							item->setEndArrowIndex(35);
					}
				}
				item->updateClip();
				item->setItemName(itemName);
				if (importerFlags & LoadSavePlugin::lfCreateDoc)
					item->setLayer(layerNum);
				if ((itElem.tagName() == "Rectangle") && (itElem.attribute("CornerOption") == "RoundedCorner"))
				{
					item->SetRectFrame();
					item->setCornerRadius(itElem.attribute("CornerRadius", "0").toDouble());
					item->SetFrameRound();
					gClip = item->PoLine.copy();
				}
				item->OwnPage = m_Doc->OnPage(item);
				item->ContourLine = item->PoLine.copy();
				GElements.prepend(m_Doc->Items->takeAt(z));
			}
			z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, baseX, baseY, 10, 10, 0, CommonStrings::None, CommonStrings::None);
			PageItem *itemg = m_Doc->Items->at(z);
			double dx = 0;
			double dy = 0;
			if (rot != 0)
			{
				QTransform mr;
				FPoint grOffset2(getMinClipF(&GCoords));
				mr.translate(-grOffset2.x(), -grOffset2.y());
				mr.rotate(rot);
				mr.translate(grOffset2.x(), grOffset2.y());
				GCoords.map(mr);
				FPoint grOffset3(getMinClipF(&GCoords));
				dx = grOffset2.x() - grOffset3.x();
				dy = grOffset2.y() - grOffset3.y();
			}
			itemg->PoLine = GCoords.copy();
			itemg->ClipEdited = true;
			itemg->FrameType = 3;
			FPoint wh = getMaxClipF(&itemg->PoLine);
			itemg->setWidthHeight(wh.x(),wh.y());
			itemg->setTextFlowMode(textFlow);
			m_Doc->adjustItemSize(itemg, true);
			itemg->moveBy(dx, dy, true);
			itemg->setRotation(-rot, true);
			itemg->OldB2 = itemg->width();
			itemg->OldH2 = itemg->height();
			if (!gClip.isEmpty())
				itemg->PoLine = gClip.copy();
			itemg->updateClip();
			itemg->setItemName(pre+itemName);
			itemg->setFillTransparency(Opacity);
			itemg->setLineTransparency(Opacity);
			itemg->setLineBlendmode(blendMode);
			itemg->setFillBlendmode(blendMode);
			itemg->setFillEvenOdd(false);
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
				itemg->setLayer(layerNum);
			itemg->OwnPage = m_Doc->OnPage(itemg);
			itemg->ContourLine = itemg->PoLine.copy();
			m_Doc->Items->takeAt(z);
			m_Doc->groupObjectsToItem(itemg, GElements);
			m_Doc->GroupOnPage(itemg);
		}
		else
		{
			if (!fillGradient.isEmpty())
				fillColor = CommonStrings::None;
			if (itElem.tagName() == "TextFrame")
			{
				z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, fillColor, strokeColor);
				PageItem* item = m_Doc->Items->at(z);
				QString story = itElem.attribute("ParentStory");
				if (!storyMap.contains(story))
					storyMap.insert(story, item);
				if (itElem.hasAttribute("NextTextFrame"))
				{
					if (itElem.attribute("NextTextFrame") != "n")
						frameLinks.insert(item, itElem.attribute("NextTextFrame"));
				}
				frameTargets.insert(itemName, item);
				item->setTextToFrameDistLeft(Extra);
				item->setTextToFrameDistTop(TExtra);
				item->setTextToFrameDistRight(RExtra);
				item->setTextToFrameDistBottom(BExtra);
				item->setColumns(TextColumnCount);
				item->setColumnGap(TextColumnGutter);
			}
			else if (isPathText)
			{
				z = m_Doc->itemAdd(PageItem::PathText, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, CommonStrings::None, strokeColor);
				if (!storyMap.contains(storyForPath))
					storyMap.insert(storyForPath, m_Doc->Items->at(z));
				PageItem* item = m_Doc->Items->at(z);
				item->setPathTextType(pathTextType);
				item->setTextToFrameDistLeft(pathTextStart);
			}
			else if (isImage)
			{
				z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, fillColor, strokeColor);
			}
			else
			{
				if (isOpen)
					z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, fillColor, strokeColor);
				else
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, lineWidth, fillColor, strokeColor);
			}
			PageItem* item = m_Doc->Items->at(z);
			double dx = 0;
			double dy = 0;
			if (rot != 0)
			{
				QTransform mr;
				FPoint grOffset2(getMinClipF(&GCoords));
				mr.translate(-grOffset2.x(), -grOffset2.y());
				mr.rotate(rot);
				mr.translate(grOffset2.x(), grOffset2.y());
				GCoords.map(mr);
				FPoint grOffset3(getMinClipF(&GCoords));
				dx = grOffset2.x() - grOffset3.x();
				dy = grOffset2.y() - grOffset3.y();
			}
			item->PoLine = GCoords.copy();
			item->ClipEdited = true;
			item->FrameType = 3;
			item->setFillColor(fillColor);
			item->setLineColor(strokeColor);
			item->setFillShade(fillShade);
			item->setLineShade(strokeShade);
			item->setFillTransparency(Opacity);
			item->setLineTransparency(Opacity);
			item->setLineBlendmode(blendMode);
			item->setFillBlendmode(blendMode);
			item->setFillEvenOdd(false);
			if (!fillGradient.isEmpty())
			{
				QLineF gradientVector = QLineF(gstX, gstY, gstX+1, gstY);
				gradientVector.setLength(gLen);
				gradientVector.setAngle(gAngle);
				gradientVector.translate(-grOffset.x(), -grOffset.y());
				item->fill_gradient = m_Doc->docGradients[fillGradient];
				item->setGradientVector(gradientVector.x1(), gradientVector.y1(), gradientVector.x2(), gradientVector.y2(), gradientVector.x1(), gradientVector.y1(), 1, 0);
				item->setGradient(fillGradient);
				item->setGradientType(fillGradientTyp);
			}
			if (!strokeGradient.isEmpty())
			{
				QLineF gradientVector = QLineF(gstSX, gstSY, gstSX+1, gstSY);
				gradientVector.setLength(gSLen);
				gradientVector.setAngle(gSAngle);
				gradientVector.translate(-grOffset.x(), -grOffset.y());
				item->stroke_gradient = m_Doc->docGradients[strokeGradient];
				item->setStrokeGradientVector(gradientVector.x1(), gradientVector.y1(), gradientVector.x2(), gradientVector.y2(), gradientVector.x1(), gradientVector.y1(), 1, 0);
				item->setStrokeGradient(strokeGradient);
				item->setStrokeGradientType(strokeGradientTyp);
			}
			FPoint wh = getMaxClipF(&item->PoLine);
			item->setWidthHeight(wh.x(),wh.y());
			item->setTextFlowMode(textFlow);
			m_Doc->adjustItemSize(item);
			item->moveBy(dx, dy, true);
			if (isPathText)
			{
				if (isOpen)
				{
					if (scX < 0)
					{
						item->PoLine.reverse();
					}
				}
				else
				{
					if (scX > 0)
					{
						double totalCurveLen = 0;
						for (int segs = 0; segs < item->PoLine.size()-3; segs += 4)
						{
							totalCurveLen += item->PoLine.lenPathSeg(segs);
						}
						item->setTextToFrameDistLeft(totalCurveLen - pathTextStart);
						item->PoLine.reverse();
					}
				}
			}
			QRectF br = QRectF(0, 0, item->width(), item->height());
			QTransform ma;
			ma.rotate(-rot);
			QRectF br2 = ma.mapRect(br);
			item->moveBy(br.x() - br2.x(), br.y() - br2.y(), true);
			item->setRotation(-rot, true);
			item->OldB2 = item->width();
			item->OldH2 = item->height();
			if (item->isPolyLine())
			{
				if (LeftLineEnd != "None")
				{
					if (LeftLineEnd == "SimpleArrowHead")
						item->setStartArrowIndex(2);
					else if (LeftLineEnd == "SimpleWideArrowHead")
						item->setStartArrowIndex(2);
					else if (LeftLineEnd == "TriangleArrowHead")
						item->setStartArrowIndex(8);
					else if (LeftLineEnd == "TriangleWideArrowHead")
						item->setStartArrowIndex(8);
					else if (LeftLineEnd == "BarbedArrowHead")
						item->setStartArrowIndex(23);
					else if (LeftLineEnd == "CurvedArrowHead")
						item->setStartArrowIndex(26);
					else if (LeftLineEnd == "CircleArrowHead")
						item->setStartArrowIndex(17);
					else if (LeftLineEnd == "CircleSolidArrowHead")
						item->setStartArrowIndex(17);
					else if (LeftLineEnd == "Square-ArrowHead")
						item->setStartArrowIndex(5);
					else if (LeftLineEnd == "SquareSolid-ArrowHead")
						item->setStartArrowIndex(5);
					else if (LeftLineEnd == "BarArrowHead")
						item->setStartArrowIndex(35);
				}
				if (RightLineEnd != "None")
				{
					if (RightLineEnd == "SimpleArrowHead")
						item->setEndArrowIndex(2);
					else if (RightLineEnd == "SimpleWideArrowHead")
						item->setEndArrowIndex(2);
					else if (RightLineEnd == "TriangleArrowHead")
						item->setEndArrowIndex(8);
					else if (RightLineEnd == "TriangleWideArrowHead")
						item->setEndArrowIndex(8);
					else if (RightLineEnd == "BarbedArrowHead")
						item->setEndArrowIndex(23);
					else if (RightLineEnd == "CurvedArrowHead")
						item->setEndArrowIndex(26);
					else if (RightLineEnd == "CircleArrowHead")
						item->setEndArrowIndex(17);
					else if (RightLineEnd == "CircleSolidArrowHead")
						item->setEndArrowIndex(17);
					else if (RightLineEnd == "Square-ArrowHead")
						item->setEndArrowIndex(7);
					else if (RightLineEnd == "SquareSolid-ArrowHead")
						item->setEndArrowIndex(7);
					else if (RightLineEnd == "BarArrowHead")
						item->setEndArrowIndex(35);
				}
			}
			item->updateClip();
			item->setItemName(itemName);
			item->OwnPage = m_Doc->OnPage(item);
			item->ContourLine = item->PoLine.copy();
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
				item->setLayer(layerNum);
			if ((itElem.tagName() == "Rectangle") && (itElem.attribute("CornerOption") == "RoundedCorner"))
			{
				item->SetRectFrame();
				item->setCornerRadius(itElem.attribute("CornerRadius", "0").toDouble());
				item->SetFrameRound();
			}
			if (isImage)
			{
				double scXi, scYi, roti, dxi, dyi;
				getTransformValuesFromMatrix(imageTransform, scXi, scYi, roti, dxi, dyi);
				if (imageData.count() > 0)
				{
					QString imgExt = "";
					if (imageType.contains("EPS", Qt::CaseInsensitive))
						imgExt = "eps";
					else if (imageType.contains("GIF", Qt::CaseInsensitive))
						imgExt = "gif";
					else if (imageType.contains("JPEG", Qt::CaseInsensitive))
						imgExt = "jpg";
					else if (imageType.contains("PDF", Qt::CaseInsensitive))
						imgExt = "pdf";
					else if (imageType.contains("PICT", Qt::CaseInsensitive))
						imgExt = "pict";
					else if (imageType.contains("PNG", Qt::CaseInsensitive))
						imgExt = "png";
					else if (imageType.contains("PSD", Qt::CaseInsensitive))
						imgExt = "psd";
					else if (imageType.contains("Photoshop", Qt::CaseInsensitive))
						imgExt = "psd";
					else if (imageType.contains("TIFF", Qt::CaseInsensitive))
						imgExt = "tif";
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_idml_XXXXXX." + imgExt);
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
							if (imageFit == "None")
								item->ScaleType   = true;
							else if (imageFit == "ContentToFrame")
								item->ScaleType   = false;
							else if (imageFit == "Proportionally")
							{
								item->ScaleType   = false;
								item->AspectRatio = false;
							}
							m_Doc->loadPict(fileName, item);
							item->setImageXYScale(scXi / item->pixm.imgInfo.xres * 72, scYi / item->pixm.imgInfo.xres * 72);
							item->setImageXYOffset(-imageDX * scXi / item->imageXScale(), -imageDY * scXi / item->imageYScale());
							item->setImageRotation(-roti);
							item->adjustPictScale();
						}
					}
					delete tempFile;
				}
				else
				{
					QUrl url = QUrl(imageFileName);
					QString fiNam = url.toLocalFile();
					QFileInfo fi(fiNam);
					QByteArray fileName;
					if (fi.exists())
						fileName = url.toLocalFile().toLocal8Bit();
					else
					{
						fileName = fi.fileName().toLocal8Bit();
						fileName.prepend("./Links/");
						QFileInfo fi2(fileName);
						if (!fi2.exists())
							fileName = fi.fileName().toLocal8Bit();
					}
					item->AspectRatio = true;
					if (imageFit == "None")
						item->ScaleType   = true;
					else if (imageFit == "ContentToFrame")
						item->ScaleType   = false;
					else if (imageFit == "Proportionally")
					{
						item->ScaleType   = false;
						item->AspectRatio = false;
					}
					m_Doc->loadPict(QUrl::fromPercentEncoding(fileName), item);
					item->setImageXYScale(scXi / item->pixm.imgInfo.xres * 72, scYi / item->pixm.imgInfo.xres * 72);
					item->setImageXYOffset(-imageDX * scXi / item->imageXScale(), -imageDY * scXi / item->imageYScale());
					item->setImageRotation(-roti);
					if (imageFit != "None")
						item->adjustPictScale();
				}
			}
			GElements.append(m_Doc->Items->takeAt(z));
		}
	}
	else
	{
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
			int z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None);
			PageItem *item = m_Doc->Items->at(z);
			item->setTextFlowMode(textFlow);
			m_Doc->adjustItemSize(item);
			item->OldB2 = item->width();
			item->OldH2 = item->height();
			item->updateClip();
			item->setItemName(itemName);
			item->setFillTransparency(Opacity);
			item->setLineTransparency(Opacity);
			item->setLineBlendmode(blendMode);
			item->setFillBlendmode(blendMode);
			item->setFillEvenOdd(false);
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
				item->setLayer(layerNum);
			item->OwnPage = m_Doc->OnPage(item);
			m_Doc->Items->takeAt(z);
			m_Doc->groupObjectsToItem(item, GElements);
			m_Doc->GroupOnPage(item);
		}
	}
	return GElements;
}

bool IdmlPlug::parseStoryXML(const QDomElement& stElem)
{
	QDomElement stNode;
	QDomDocument stMapDom;
	if (stElem.hasAttribute("src"))
	{
		QByteArray f2;
		m_zip->read(stElem.attribute("src"), f2);
		if (!stMapDom.setContent(f2))
			return false;
		stNode = stMapDom.documentElement();
	}
	else
	{
		if (!stElem.hasChildNodes())
			return false;
		stNode = stElem;
	}
	parseStoryXMLNode(stNode);
	return true;
}

void IdmlPlug::parseStoryXMLNode(const QDomElement& stNode)
{
	for (QDomNode n = stNode.firstChild(); !n.isNull(); n = n.nextSibling() )
	{
		QDomElement e = n.toElement();
		if (e.tagName() == "Story")
		{
			QString storyName = e.attribute("Self");
			PageItem *item = nullptr;
			if (!storyMap.contains(storyName))
				return;
			item = storyMap[storyName];
			for (QDomNode st = e.firstChild(); !st.isNull(); st = st.nextSibling())
			{
				QDomElement ste = st.toElement();
				if (ste.tagName() == "ParagraphStyleRange")
					parseParagraphStyleRange(ste, item);
				else if (ste.tagName() == "XMLElement")
				{
					for (QDomNode stx = ste.firstChild(); !stx.isNull(); stx = stx.nextSibling())
					{
						QDomElement stxe = stx.toElement();
						if (stxe.tagName() == "ParagraphStyleRange")
							parseParagraphStyleRange(stxe, item);
					}
				}
			}
			item->itemText.trim();
		}
	}
}

void IdmlPlug::parseParagraphStyleRange(QDomElement &ste, PageItem* item)
{
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	if (ste.hasAttribute("AppliedParagraphStyle"))
	{
		pStyle = ste.attribute("AppliedParagraphStyle").remove("$ID/");
		if (styleTranslate.contains(pStyle))
			pStyle = styleTranslate[pStyle];
		else
			pStyle = CommonStrings::DefaultParagraphStyle;
	}
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	// Apply possible override of paragraph style
	readParagraphStyleAttributes(newStyle, ste);
	ParagraphStyle ttx = m_Doc->paragraphStyle(pStyle);
	QString fontBase = ttx.charStyle().font().family();
	QString fontStyle = ttx.charStyle().font().style();
	for (QDomNode stc = ste.firstChild(); !stc.isNull(); stc = stc.nextSibling())
	{
		QDomElement stt = stc.toElement();
		if (stt.tagName() == "CharacterStyleRange")
			parseCharacterStyleRange(stt, item, fontBase, fontStyle, newStyle, item->itemText.length());
		else if (stt.tagName() == "XMLElement")
		{
			for (QDomNode stx = stt.firstChild(); !stx.isNull(); stx = stx.nextSibling())
			{
				QDomElement stxe = stx.toElement();
				if (stxe.tagName() == "CharacterStyleRange")
					parseCharacterStyleRange(stxe, item, fontBase, fontStyle, newStyle, item->itemText.length());
				else if (stxe.tagName() == "XMLElement")
				{
					for (QDomNode stxx = stxe.firstChild(); !stxx.isNull(); stxx = stxx.nextSibling())
					{
						QDomElement stxxe = stxx.toElement();
						if (stxxe.tagName() == "CharacterStyleRange")
							parseCharacterStyleRange(stxxe, item, fontBase, fontStyle, newStyle, item->itemText.length());
					}
				}
			}
		}
	}
	int posT = item->itemText.length();
	if (posT > 0)
	{
		if ((item->itemText.text(posT - 1) != SpecialChars::PARSEP))
			item->itemText.insertChars(posT, SpecialChars::PARSEP);
	}
	item->itemText.applyStyle(posT, newStyle);
}

void IdmlPlug::parseCharacterStyleRange(QDomElement &stt, PageItem* item, QString fontBase, QString fontStyle, ParagraphStyle &newStyle, int posC)
{
	QString data = "";
	bool hasChangedFont = false;
	for (QDomNode stcp = stt.firstChild(); !stcp.isNull(); stcp = stcp.nextSibling())
	{
		QDomElement sp = stcp.toElement();
		if (sp.tagName() == "Properties")
		{
			for (QDomNode spa = sp.firstChild(); !spa.isNull(); spa = spa.nextSibling())
			{
				QDomElement spf = spa.toElement();
				if (spf.tagName() == "AppliedFont")
				{
					fontBase = spf.text();
					hasChangedFont = true;
				}
			}
		}
		else if (sp.tagName() == "Leading")
		{
			if (sp.attribute("type") == "unit")
			{
				int lead = sp.text().toDouble();
				if (lead != 0)
				{
					newStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
					newStyle.setLineSpacing(lead);
				}
			}
		}
	}
	// Apply possible override of character style
	CharStyle nstyle = newStyle.charStyle();
	if (stt.hasAttribute("FontStyle"))
	{
		fontStyle = stt.attribute("FontStyle", "");
		hasChangedFont = true;
	}
	if (stt.hasAttribute("AppliedCharacterStyle"))
	{
		QString cStyle = stt.attribute("AppliedCharacterStyle").remove("$ID/");
		if (cStyle != "CharacterStyle/[No character style]")
		{
			if (charStyleTranslate.contains(cStyle))
			{
				QString pst = nstyle.name();
				cStyle = charStyleTranslate[cStyle];
				nstyle = m_Doc->charStyle(cStyle);
				nstyle.setParent(pst);
			}
		}
	}
	if (hasChangedFont)
	{
		if ((!fontBase.isEmpty()) && (!fontStyle.isEmpty()))
		{
			QString fontName = constructFontName(fontBase, fontStyle);
			nstyle.setFont((*m_Doc->AllFonts)[fontName]);
		}
	}
	readCharStyleAttributes(nstyle, stt);
	for (QDomNode stch = stt.firstChild(); !stch.isNull(); stch = stch.nextSibling())
	{
		QDomElement s = stch.toElement();
		if (s.tagName() == "Content")
		{
			QString ch = "";
			for (QDomNode sh = s.firstChild(); !sh.isNull(); sh = sh.nextSibling())
			{
				QString p = sh.nodeValue();
				if (sh.nodeName() == "#text")
					ch += p;
				else if (p == "18")
					ch += SpecialChars::PAGENUMBER;
			}
			if (!ch.isEmpty())
			{
				for (int tt = 0; tt < ch.length(); ++tt)
				{
					QChar c = ch.at(tt);
					if (c.unicode() == 0x2028)
						ch[tt] = SpecialChars::LINEBREAK;
					if (c.unicode() == 0x202F)
						ch[tt] = SpecialChars::NBSPACE;
				}
				data += ch;
			}
			else
				data += " ";
		}
		else if (s.tagName() == "Br")
		{
			data += SpecialChars::PARSEP;
			item->itemText.insertChars(posC, data);
			item->itemText.applyStyle(posC, newStyle);
			item->itemText.applyCharStyle(posC, data.length(), nstyle);
			data = "";
			posC = item->itemText.length();
		}
		else if ((s.tagName() == "Rectangle") || (s.tagName() == "Oval") || (s.tagName() == "GraphicLine") || (s.tagName() == "Polygon") || (s.tagName() == "TextFrame") || (s.tagName() == "Group") || (s.tagName() == "Button"))
		{
			QTransform m;
			QList<PageItem*> el = parseItemXML(s, m);
			for (int ec = 0; ec < el.count(); ++ec)
			{
				PageItem* currItem = el.at(ec);
				currItem->isEmbedded = true;
				currItem->gXpos = 0;
				currItem->gYpos = 0;
				currItem->gWidth = currItem->width();
				currItem->gHeight = currItem->height();
				int fIndex = m_Doc->addToInlineFrames(currItem);
				item->itemText.insertObject(fIndex);
				item->itemText.applyStyle(posC, newStyle);
				data = "";
				posC = item->itemText.length();
			}
		}
		else if (s.tagName() == "Table")
		{
			QList<double> rowHeights;
			QList<double> colWidths;
			double twidth = 0.0;
			double theight = 0.0;
			for (QDomNode st = s.firstChild(); !st.isNull(); st = st.nextSibling())
			{
				QDomElement sr = st.toElement();
				if (sr.tagName() == "Row")
				{
					theight += sr.attribute("SingleRowHeight", "0").toDouble();
					rowHeights.append(sr.attribute("SingleRowHeight", "0").toDouble());
				}
				if (sr.tagName() == "Column")
				{
					twidth += sr.attribute("SingleColumnWidth", "0").toDouble();
					colWidths.append(sr.attribute("SingleColumnWidth", "0").toDouble());
				}
			}
			m_Doc->dontResize = true;
			int z = m_Doc->itemAdd(PageItem::Table, PageItem::Unspecified, 0, 0, qMin(item->width() - 2, twidth), qMin(item->height() - 2, theight), 0.0, CommonStrings::None, CommonStrings::None);
			PageItem_Table* currItem = m_Doc->Items->takeAt(z)->asTable();
			currItem->insertRows(0, rowHeights.count()-1);
			m_Doc->dontResize = true;
			currItem->insertColumns(0, colWidths.count()-1);
			m_Doc->dontResize = true;
			for (int i = 0; i < rowHeights.count(); i++)
			{
				currItem->resizeRow(i, rowHeights[i]);
			}
			m_Doc->dontResize = true;
			for (int i = 0; i < colWidths.count(); i++)
			{
				currItem->resizeColumn(i, colWidths[i]);
			}
			m_Doc->dontResize = true;
			for (QDomNode st = s.firstChild(); !st.isNull(); st = st.nextSibling())
			{
				QDomElement sr = st.toElement();
				if (sr.tagName() == "Cell")
				{
					QStringList pos = sr.attribute("Name", "0:0").split(":");
					PageItem* itText = currItem->cellAt(pos[1].toInt(), pos[0].toInt()).textFrame();
					if (itText)
					{
						m_Doc->dontResize = true;
						for (QDomNode sct = sr.firstChild(); !sct.isNull(); sct = sct.nextSibling())
						{
							QDomElement spf = sct.toElement();
							if (spf.tagName() == "XMLElement")
							{
								for (QDomNode sctx = spf.firstChild(); !sctx.isNull(); sctx = sctx.nextSibling())
								{
									QDomElement spfx = sctx.toElement();
									if (spfx.tagName() == "ParagraphStyleRange")
										parseParagraphStyleRange(spfx, itText);
								}
							}
							else if (spf.tagName() == "ParagraphStyleRange")
								parseParagraphStyleRange(spf, itText);
						}
					}
				}
			}
			m_Doc->dontResize = true;
			currItem->adjustTableToFrame();
			currItem->adjustFrameToTable();
			currItem->isEmbedded = true;
			currItem->gXpos = 0;
			currItem->gYpos = 0;
			currItem->gWidth = currItem->width();
			currItem->gHeight = currItem->height();
			m_Doc->dontResize = false;
			int fIndex = m_Doc->addToInlineFrames(currItem);
			item->itemText.insertObject(fIndex);
			item->itemText.applyStyle(posC, newStyle);
			data = "";
			posC = item->itemText.length();
		}
		else if (s.tagName() == "XMLElement")
		{
		//	for (QDomNode stx = s.firstChild(); !stx.isNull(); stx = stx.nextSibling())
		//	{
				parseCharacterStyleRange(s, item, fontBase, fontStyle, newStyle, posC);
		//	}
		}
	}
	if (data.count() > 0)
	{
		item->itemText.insertChars(posC, data);
		item->itemText.applyStyle(posC, newStyle);
		item->itemText.applyCharStyle(posC, data.length(), nstyle);
		posC = item->itemText.length();
	}
}

void IdmlPlug::readCharStyleAttributes(CharStyle &newStyle, const QDomElement &styleElem)
{
	if (styleElem.hasAttribute("BaselineShift"))
		newStyle.setBaselineOffset(qRound((styleElem.attribute("BaselineShift","0").toDouble()) * 10));
	if (styleElem.hasAttribute("UnderlineOffset"))
	{
		double offs = styleElem.attribute("UnderlineOffset","0").toDouble();
		if (offs >= 0)
			newStyle.setUnderlineOffset(qRound(offs * 10));
		else
			newStyle.setUnderlineOffset(-1);
	}
	if (styleElem.hasAttribute("UnderlineWidth"))
	{
		double offs = styleElem.attribute("UnderlineWidth","0").toDouble();
		if (offs >= 0)
			newStyle.setUnderlineWidth(qRound(offs * 10));
		else
			newStyle.setUnderlineWidth(-1);
	}
	if (styleElem.hasAttribute("StrikeThroughOffset"))
	{
		double offs = styleElem.attribute("StrikeThroughOffset","0").toDouble();
		if (offs >= 0)
			newStyle.setStrikethruOffset(qRound(offs * 10));
		else
			newStyle.setStrikethruOffset(-1);
	}
	if (styleElem.hasAttribute("StrikeThroughWidth"))
	{
		double offs = styleElem.attribute("StrikeThroughWidth","0").toDouble();
		if (offs >= 0)
			newStyle.setStrikethruWidth(qRound(offs * 10));
		else
			newStyle.setStrikethruWidth(-1);
	}
	if (styleElem.hasAttribute("PointSize"))
	{
		int pointSize = qRound(styleElem.attribute("PointSize", "12").toDouble() * 10);
		if (pointSize > 0)
			newStyle.setFontSize(pointSize);
	}
	if (styleElem.hasAttribute("FillColor"))
	{
		QString fillColor = styleElem.attribute("FillColor");
		if (colorTranslate.contains(fillColor))
			newStyle.setFillColor(colorTranslate[fillColor]);
	}
	if (styleElem.hasAttribute("FillTint"))
	{
		int fillTint = styleElem.attribute("FillTint", "100").toInt();
		if (fillTint != -1)
			newStyle.setFillShade(fillTint);
	}
	StyleFlag styleEffects = newStyle.effects();
	if (styleElem.attribute("Underline") == "true")
		styleEffects |= ScStyle_Underline;
	if (styleElem.attribute("StrikeThru") == "true")
		styleEffects |= ScStyle_Strikethrough;
	if (styleElem.hasAttribute("Capitalization"))
	{
		QString ca = styleElem.attribute("Capitalization");
		if (ca == "AllCaps")
			styleEffects |= ScStyle_AllCaps;
		else if (ca == "SmallCaps")
			styleEffects |= ScStyle_SmallCaps;
	}
	if (styleElem.hasAttribute("Position"))
	{
		QString pa = styleElem.attribute("Position");
		if ((pa == "Superscript") || (pa == "OTSuperscript"))
			styleEffects |= ScStyle_Superscript;
		else if ((pa == "Subscript") || (pa == "OTSubscript"))
			styleEffects |= ScStyle_Subscript;
	}
	newStyle.setFeatures(styleEffects.featureList());
}

void IdmlPlug::readParagraphStyleAttributes(ParagraphStyle &newStyle, const QDomElement &styleElem)
{
	if (styleElem.hasAttribute("LeftIndent"))
		newStyle.setLeftMargin(styleElem.attribute("LeftIndent", "0").toDouble());
	if (styleElem.hasAttribute("FirstLineIndent"))
		newStyle.setFirstIndent(styleElem.attribute("FirstLineIndent", "0").toDouble());
	if (styleElem.hasAttribute("RightIndent"))
		newStyle.setRightMargin(styleElem.attribute("RightIndent", "0").toDouble());
	if (styleElem.hasAttribute("SpaceBefore"))
		newStyle.setGapBefore(styleElem.attribute("SpaceBefore", "0").toDouble());
	if (styleElem.hasAttribute("SpaceAfter"))
		newStyle.setGapAfter(styleElem.attribute("SpaceAfter", "0").toDouble());
	if (styleElem.hasAttribute("DropCapCharacters"))
	{
		newStyle.setHasDropCap(styleElem.attribute("DropCapCharacters", "0").toInt() != 0);
		if (styleElem.hasAttribute("DropCapLines"))
			newStyle.setDropCapLines(styleElem.attribute("DropCapLines", "2").toInt());
	}
	if (styleElem.hasAttribute("Justification"))
	{
		QString align = styleElem.attribute("Justification", "LeftAlign");
		if (align == "LeftAlign")
			newStyle.setAlignment(ParagraphStyle::LeftAligned);
		else if (align == "CenterAlign")
			newStyle.setAlignment(ParagraphStyle::Centered);
		else if (align == "RightAlign")
			newStyle.setAlignment(ParagraphStyle::RightAligned);
		else if ((align == "LeftJustified") || (align == "CenterJustified") || (align == "RightJustified"))
			newStyle.setAlignment(ParagraphStyle::Justified);
		else if (align == "FullyJustified")
			newStyle.setAlignment(ParagraphStyle::Extended);
	}
/*
	if (styleElem.hasAttribute("MinimumGlyphScaling"))
		newStyle.setMinGlyphExtension(styleElem.attribute("MinimumGlyphScaling", "100").toDouble());
	if (styleElem.hasAttribute("MaximumGlyphScaling"))
		newStyle.setMaxGlyphExtension(styleElem.attribute("MaximumGlyphScaling", "100").toDouble());
	if (styleElem.hasAttribute("MinimumWordSpacing"))
		newStyle.setMinWordTracking(styleElem.attribute("MinimumWordSpacing", "100").toDouble());
	if (styleElem.hasAttribute("DesiredWordSpacing"))
		newStyle.charStyle().setWordTracking(styleElem.attribute("DesiredWordSpacing", "100").toDouble());
*/
}

int IdmlPlug::convertBlendMode(const QString& blendName)
{
	int mode = 0;
	if (blendName == "Normal")
		mode = 0;
	else if (blendName == "Darken")
		mode = 1;
	else if (blendName == "Lighten")
		mode = 2;
	else if (blendName == "Multiply")
		mode = 3;
	else if (blendName == "Screen")
		mode = 4;
	else if (blendName == "Overlay")
		mode = 5;
	else if (blendName == "HardLight")
		mode = 6;
	else if (blendName == "SoftLight")
		mode = 7;
	else if (blendName == "Difference")
		mode = 8;
	else if (blendName == "Exclusion")
		mode = 9;
	else if (blendName == "ColorDodge")
		mode = 10;
	else if (blendName == "ColorBurn")
		mode = 11;
	else if (blendName == "Hue")
		mode = 12;
	else if (blendName == "Saturation")
		mode = 13;
	else if (blendName == "Color")
		mode = 14;
	else if (blendName == "Luminosity")
		mode = 15;
	return mode;
}

void IdmlPlug::resolveObjectStyle(ObjectStyle &nstyle, const QString& baseStyleName)
{
	QStringList styles;
	styles.prepend(baseStyleName);
	ObjectStyle style = ObjectStyles[baseStyleName];
	while (!style.parentStyle.isEmpty())
	{
		styles.prepend(style.parentStyle);
		style = ObjectStyles[style.parentStyle];
	}
	for (int a = 0; a < styles.count(); ++a)
	{
		style = ObjectStyles[styles[a]];
		if (style.fillColor != def_fillColor)
			nstyle.fillColor = style.fillColor;
		if (style.strokeColor != def_strokeColor)
			nstyle.strokeColor = style.strokeColor;
		if (style.fillGradient != "")
			nstyle.fillGradient = style.fillGradient;
		if (style.gradientFillStart != QPointF(def_gradientX, def_gradientY))
			nstyle.gradientFillStart = style.gradientFillStart;
		if (style.gradientFillLength != def_gradientLen)
			nstyle.gradientFillLength = style.gradientFillLength;
		if (style.gradientFillAngle != def_gradientAngle)
			nstyle.gradientFillAngle = style.gradientFillAngle;
		if (style.strokeGradient != "")
			nstyle.strokeGradient = style.strokeGradient;
		if (style.gradientStrokeStart != QPointF(def_gradientStrokeStartX, def_gradientStrokeStartY))
			nstyle.gradientStrokeStart = style.gradientStrokeStart;
		if (style.gradientStrokeLength != def_gradientStrokeLength)
			nstyle.gradientStrokeLength = style.gradientStrokeLength;
		if (style.gradientStrokeAngle != def_gradientStrokeAngle)
			nstyle.gradientStrokeAngle = style.gradientStrokeAngle;
		if (style.lineWidth != def_lineWidth)
			nstyle.lineWidth = style.lineWidth;
		if (style.fillTint != def_fillTint)
			nstyle.fillTint = style.fillTint;
		if (style.strokeTint != def_strokeTint)
			nstyle.strokeTint = style.strokeTint;
		if (style.Opacity != def_Opacity)
			nstyle.Opacity = style.Opacity;
		if (style.blendMode != def_Blendmode)
			nstyle.blendMode = style.blendMode;
		if (style.Extra != def_Extra)
			nstyle.Extra = style.Extra;
		if (style.TExtra != def_TExtra)
			nstyle.TExtra = style.TExtra;
		if (style.BExtra != def_BExtra)
			nstyle.BExtra = style.BExtra;
		if (style.RExtra != def_RExtra)
			nstyle.RExtra = style.RExtra;
		if (style.TextColumnCount != def_TextColumnCount)
			nstyle.TextColumnCount = style.TextColumnCount;
		if (style.TextColumnGutter != def_TextColumnGutter)
			nstyle.TextColumnGutter = style.TextColumnGutter;
		if (style.TextColumnFixedWidth != def_TextColumnFixedWidth)
			nstyle.TextColumnFixedWidth = style.TextColumnFixedWidth;
		if (style.TextFlow != def_TextFlow)
			nstyle.TextFlow = style.TextFlow;
		if (style.LeftLineEnd != def_LeftLineEnd)
			nstyle.LeftLineEnd = style.LeftLineEnd;
		if (style.RightLineEnd != def_RightLineEnd)
			nstyle.RightLineEnd = style.RightLineEnd;
	}
}

QString IdmlPlug::constructFontName(const QString& fontBaseName, const QString& fontStyle)
{
	QString fontName = PrefsManager::instance().appPrefs.itemToolPrefs.textFont;
	if (fontTranslateMap.contains(fontBaseName))
	{
		QMap<QString, QString> styleMap = fontTranslateMap[fontBaseName];
		if (styleMap.contains(fontStyle))
		{
			QString postName = styleMap[fontStyle];
			bool found = false;
			SCFontsIterator it(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts);
			for ( ; it.hasNext(); it.next())
			{
				if (it.current().psName() == postName)
				{
					fontName = it.current().scName();
					found = true;
					break;
				}
			}
			if (!found)
			{
				if (importerFlags & LoadSavePlugin::lfCreateThumbnail)
					fontName = PrefsManager::instance().appPrefs.itemToolPrefs.textFont;
				else
				{
					QString family = fontBaseName + " " + fontStyle;
					family = family.remove("$ID/");
					if (!PrefsManager::instance().appPrefs.fontPrefs.GFontSub.contains(family))
					{
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						MissingFont *dia = new MissingFont(nullptr, family, m_Doc);
						dia->exec();
						fontName = dia->getReplacementFont();
						delete dia;
						qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
						PrefsManager::instance().appPrefs.fontPrefs.GFontSub[family] = fontName;
					}
					else
						fontName = PrefsManager::instance().appPrefs.fontPrefs.GFontSub[family];
				}
			}
		}
	}
	return fontName;
}
