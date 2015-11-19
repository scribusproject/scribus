/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Feb  9 2014
	copyright            : (C) 2014 by Franz Schmid
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
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QDebug>

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include <cstdlib>
#include <climits>
#include <limits>

#include "importodg.h"

#include "color.h"
#include "fileloader.h"
#include "third_party/fparser/fparser.hh"
#include "loadsaveplugin.h"
#include "pageitem_table.h"
#include "pagesize.h"
#include "plugins/formatidlist.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "rawimage.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scmimedata.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "selection.h"
#include "third_party/zip/scribus_zip.h"
#include "ui/customfdialog.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "util.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_math.h"


extern SCRIBUS_API ScribusQApp * ScQApp;

OdgPlug::OdgPlug(ScribusDoc* doc, int flags)
{
	tmpSel = new Selection(this, false);
	m_Doc = doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = NULL;
	uz = NULL;
}

QImage OdgPlug::readThumbnail(QString fName)
{
	QImage tmp;
	if (!QFile::exists(fName))
		return QImage();
	progressDialog = NULL;
	uz = new ScZipHandler();
	if (!uz->open(fName))
	{
		delete uz;
		if (progressDialog)
			progressDialog->close();
		return QImage();
	}
	if (uz->contains("Thumbnails/thumbnail.png"))
	{
		QByteArray im;
		if (!uz->read("Thumbnails/thumbnail.png", im))
		{
			delete uz;
			return QImage();
		}
		tmp = QImage::fromData(im);
		int xs = 0;
		int ys = 0;
	/*	if (uz->contains("index.xml"))
		{
			if (uz->read("index.xml", f))
			{
				QDomDocument designMapDom;
				QByteArray f;
				if(designMapDom.setContent(f))
				{
					QDomElement docElem = designMapDom.documentElement();
					for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
					{
						if (drawPag.tagName() == "sl:slprint-info")
						{
							xs = drawPag.attribute("sl:page-width", "0").toInt();
							ys = drawPag.attribute("sl:page-height", "0").toInt();
						}
					}
				}
			}
		}*/
		tmp.setText("XSize", QString("%1").arg(xs));
		tmp.setText("YSize", QString("%1").arg(ys));
	}
	uz->close();
	delete uz;
	return tmp;
}

bool OdgPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	bool ret = false;
	firstPage = true;
	pagecount = 1;
	mpagecount = 0;
	QFileInfo fi = QFileInfo(fName);
	if ( !ScCore->usingGUI() )
	{
		interactive = false;
		showProgress = false;
	}
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
	if ((!(flags & LoadSavePlugin::lfLoadAsPattern)) && (m_Doc->view() != NULL))
		m_Doc->view()->Deselect();
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
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					tmpSel->addItem(Elements.at(dre), true);
				}
				tmpSel->setGroupRect();
				ScElemMimeData* md = ScriXmlDoc::WriteToMimeData(m_Doc, tmpSel);
				m_Doc->itemSelection_DeleteItem(tmpSel);
				m_Doc->view()->updatesOn(true);
				if ((importedColors.count() != 0) && (!((flags & LoadSavePlugin::lfKeepGradients) || (flags & LoadSavePlugin::lfKeepColors) || (flags & LoadSavePlugin::lfKeepPatterns))))
				{
					for (int cd = 0; cd < importedColors.count(); cd++)
					{
						m_Doc->PageColors.remove(importedColors[cd]);
					}
				}
				if ((importedPatterns.count() != 0) && (!(flags & LoadSavePlugin::lfKeepPatterns)))
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
		success = false;
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

OdgPlug::~OdgPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

bool OdgPlug::convert(QString fn)
{
	bool retVal = true;
	importedColors.clear();
	importedPatterns.clear();
	m_Styles.clear();
	m_Layers.clear();
	firstLayer = true;
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}

	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	if ((ext == "fodg") || (ext == "fodp"))
	{
		QByteArray f;
		loadRawText(fn, f);
		QDomDocument designMapDom;
		QString errorMsg = "";
		int errorLine = 0;
		int errorColumn = 0;
		if (!designMapDom.setContent(f, &errorMsg, &errorLine, &errorColumn))
		{
			qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
			return false;
		}
		retVal = parseDocReferenceXML(designMapDom);
	}
	else
	{
		uz = new ScZipHandler();
		if (!uz->open(fn))
		{
			delete uz;
			QByteArray f;
			loadRawText(fn, f);
			QDomDocument designMapDom;
			QString errorMsg = "";
			int errorLine = 0;
			int errorColumn = 0;
			if (designMapDom.setContent(f, &errorMsg, &errorLine, &errorColumn))
			{
				retVal = parseDocReferenceXML(designMapDom);
			}
			else
			{
				qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
				if (progressDialog)
					progressDialog->close();
				return false;
			}
		}
		else
		{
			retVal = false;
			if (uz->contains("styles.xml"))
				retVal = parseStyleSheets("styles.xml");
			if (uz->contains("content.xml"))
				retVal = parseDocReference("content.xml");
			uz->close();
			delete uz;
		}
	}
	if (progressDialog)
		progressDialog->close();
	return retVal;
}

bool OdgPlug::parseStyleSheets(QString designMap)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(designMap, f))
		return false;
	QXmlInputSource xmlSource;
	xmlSource.setData(f);
	QXmlSimpleReader xmlReader;
	xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(&xmlSource, &xmlReader, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return false;
	}
	return parseStyleSheetsXML(designMapDom);
}

bool OdgPlug::parseStyleSheetsXML(QDomDocument &designMapDom)
{
	QDomElement docElem = designMapDom.documentElement();
	for(QDomElement sp = docElem.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
	{
		if (sp.tagName() == "office:font-face-decls")
		{
			for(QDomElement spf = sp.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "style:font-face")
				{
					if (!spf.attribute("style:name").isEmpty())
						m_fontMap.insert(spf.attribute("style:name"), spf.attribute("svg:font-family"));
				}
			}
		}
		else if ((sp.tagName() == "office:styles") || (sp.tagName() == "office:automatic-styles"))
			parseStyles(sp);
		if (sp.tagName() == "office:master-styles")
		{
			DrawStyle currStyle;
			for(QDomElement spf = sp.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "style:master-page")
				{
					currStyle.page_layout_name = AttributeValue(spf.attribute("style:page-layout-name"));
					QString backGroundStyle = spf.attribute("draw:style-name", "");
					m_Styles.insert(spf.attribute("style:name"), currStyle);
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
						m_Doc->setMasterPageMode(true);
						ScPage *oldCur = m_Doc->currentPage();
						ScPage *addedPage = m_Doc->addMasterPage(mpagecount, spf.attribute("style:name"));
						m_Doc->setCurrentPage(addedPage);
						addedPage->MPageNam = "";
						m_Doc->view()->addPage(mpagecount, true);
						baseX = addedPage->xOffset();
						baseY = addedPage->yOffset();
						mpagecount++;
						ObjStyle tmpOStyle;
						resovleStyle(tmpOStyle, spf.attribute("style:name"));
						m_Doc->currentPage()->m_pageSize = "Custom";
						m_Doc->currentPage()->setInitialHeight(tmpOStyle.page_height);
						m_Doc->currentPage()->setInitialWidth(tmpOStyle.page_width);
						m_Doc->currentPage()->setHeight(tmpOStyle.page_height);
						m_Doc->currentPage()->setWidth(tmpOStyle.page_width);
						m_Doc->currentPage()->initialMargins.setTop(tmpOStyle.margin_top);
						m_Doc->currentPage()->initialMargins.setBottom(tmpOStyle.margin_bottom);
						m_Doc->currentPage()->initialMargins.setLeft(tmpOStyle.margin_left);
						m_Doc->currentPage()->initialMargins.setRight(tmpOStyle.margin_right);
						if (!backGroundStyle.isEmpty())
						{
							ObjStyle tmpBStyle;
							resovleStyle(tmpBStyle, backGroundStyle);
							int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX, baseY, tmpOStyle.page_width, tmpOStyle.page_height, 0, tmpBStyle.CurrColorFill, CommonStrings::None);
							PageItem *retObj = m_Doc->Items->at(z);
							finishItem(retObj, tmpBStyle);
						}
						for(QDomElement spm = spf.firstChildElement(); !spm.isNull(); spm = spm.nextSiblingElement() )
						{
							PageItem* retObj = parseObj(spm);
							if (retObj != NULL)
								m_Doc->Items->append(retObj);
						}
						m_Doc->setCurrentPage(oldCur);
						m_Doc->setMasterPageMode(false);
					}
				}
				else if (spf.tagName() == "draw:layer-set")
				{
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
						for(QDomElement spp = spf.firstChildElement(); !spp.isNull(); spp = spp.nextSiblingElement() )
						{
							if (spp.tagName() == "draw:layer")
							{
								QString layerName = spp.attribute("draw:name");
								if (!layerName.isEmpty())
								{
									if (!firstLayer)
									{
										QStringList newNames;
										m_Doc->orderedLayerList(&newNames);
										if (!newNames.contains(layerName))
										{
											int currentLayer = m_Doc->addLayer(layerName);
											m_Layers.insert(layerName, currentLayer);
										}
									}
									else
									{
										m_Doc->changeLayerName(m_Doc->firstLayerID(), layerName);
										m_Layers.insert(layerName, m_Doc->firstLayerID());
									}
									firstLayer = false;
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool OdgPlug::parseDocReference(QString designMap)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(designMap, f))
		return false;
	QXmlInputSource xmlSource;
	xmlSource.setData(f);
	QXmlSimpleReader xmlReader;
	xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(&xmlSource, &xmlReader, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return false;
	}
	return parseDocReferenceXML(designMapDom);
}

bool OdgPlug::parseDocReferenceXML(QDomDocument &designMapDom)
{
	QDomElement docElem = designMapDom.documentElement();
	for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "office:font-face-decls")
		{
			for(QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "style:font-face")
				{
					if (!spf.attribute("style:name").isEmpty())
						m_fontMap.insert(spf.attribute("style:name"), spf.attribute("svg:font-family"));
				}
			}
		}
		else if ((drawPag.tagName() == "office:styles") || (drawPag.tagName() == "office:automatic-styles"))
			parseStyles(drawPag);
		if (drawPag.tagName() == "office:master-styles")
		{
			for(QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "style:master-page")
				{
					DrawStyle currStyle;
					currStyle.page_layout_name = AttributeValue(spf.attribute("style:page-layout-name"));
					m_Styles.insert(spf.attribute("style:name"), currStyle);
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
						m_Doc->setMasterPageMode(true);
						ScPage *oldCur = m_Doc->currentPage();
						ScPage *addedPage = m_Doc->addMasterPage(mpagecount, spf.attribute("style:name"));
						m_Doc->setCurrentPage(addedPage);
						addedPage->MPageNam = "";
						m_Doc->view()->addPage(mpagecount, true);
						baseX = addedPage->xOffset();
						baseY = addedPage->yOffset();
						mpagecount++;
						ObjStyle tmpOStyle;
						resovleStyle(tmpOStyle, spf.attribute("style:name"));
						m_Doc->currentPage()->m_pageSize = "Custom";
						m_Doc->currentPage()->setInitialHeight(tmpOStyle.page_height);
						m_Doc->currentPage()->setInitialWidth(tmpOStyle.page_width);
						m_Doc->currentPage()->setHeight(tmpOStyle.page_height);
						m_Doc->currentPage()->setWidth(tmpOStyle.page_width);
						m_Doc->currentPage()->initialMargins.setTop(tmpOStyle.margin_top);
						m_Doc->currentPage()->initialMargins.setBottom(tmpOStyle.margin_bottom);
						m_Doc->currentPage()->initialMargins.setLeft(tmpOStyle.margin_left);
						m_Doc->currentPage()->initialMargins.setRight(tmpOStyle.margin_right);
						if (!currStyle.page_layout_name.value.isEmpty())
						{
							ObjStyle tmpBStyle;
							resovleStyle(tmpBStyle, currStyle.page_layout_name.value);
							int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX, baseY, tmpOStyle.page_width, tmpOStyle.page_height, 0, tmpBStyle.CurrColorFill, CommonStrings::None);
							PageItem *retObj = m_Doc->Items->at(z);
							finishItem(retObj, tmpBStyle);
						}
						for(QDomElement spm = spf.firstChildElement(); !spm.isNull(); spm = spm.nextSiblingElement() )
						{
							PageItem* retObj = parseObj(spm);
							if (retObj != NULL)
								m_Doc->Items->append(retObj);
						}
						m_Doc->setCurrentPage(oldCur);
						m_Doc->setMasterPageMode(false);
					}
				}
				else if (spf.tagName() == "draw:layer-set")
				{
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
						for(QDomElement spp = spf.firstChildElement(); !spp.isNull(); spp = spp.nextSiblingElement() )
						{
							if (spp.tagName() == "draw:layer")
							{
								QString layerName = spp.attribute("draw:name");
								if (!layerName.isEmpty())
								{
									if (!firstLayer)
									{
										QStringList newNames;
										m_Doc->orderedLayerList(&newNames);
										if (!newNames.contains(layerName))
										{
											int currentLayer = m_Doc->addLayer(layerName);
											m_Layers.insert(layerName, currentLayer);
										}
									}
									else
									{
										m_Doc->changeLayerName(m_Doc->firstLayerID(), layerName);
										m_Layers.insert(layerName, m_Doc->firstLayerID());
									}
									firstLayer = false;
								}
							}
						}
					}
				}
			}
		}
		else if (drawPag.tagName() == "office:body")
		{
			for(QDomElement sp = drawPag.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
			{
				if ((sp.tagName() == "office:drawing") || (sp.tagName() == "office:presentation"))
				{
					for(QDomElement spp = sp.firstChildElement(); !spp.isNull(); spp = spp.nextSiblingElement() )
					{
						if (spp.tagName() == "draw:page")
						{
							ObjStyle tmpOStyle;
							resovleStyle(tmpOStyle, spp.attribute("draw:master-page-name"));
							docWidth = tmpOStyle.page_width;
							docHeight = tmpOStyle.page_height;
							topMargin = tmpOStyle.margin_top;
							leftMargin = tmpOStyle.margin_left;
							rightMargin = tmpOStyle.margin_right;
							bottomMargin = tmpOStyle.margin_bottom;
							if (importerFlags & LoadSavePlugin::lfCreateDoc)
							{
								if (firstPage)
								{
									m_Doc->setPage(docWidth, docHeight, topMargin, leftMargin, rightMargin, bottomMargin, m_Doc->PageSp, m_Doc->PageSpa, false, false);
									m_Doc->setPageSize("Custom");
									m_Doc->currentPage()->m_pageSize = "Custom";
									m_Doc->currentPage()->setInitialHeight(docHeight);
									m_Doc->currentPage()->setInitialWidth(docWidth);
									m_Doc->currentPage()->setHeight(docHeight);
									m_Doc->currentPage()->setWidth(docWidth);
									m_Doc->currentPage()->initialMargins.setTop(topMargin);
									m_Doc->currentPage()->initialMargins.setBottom(bottomMargin);
									m_Doc->currentPage()->initialMargins.setLeft(leftMargin);
									m_Doc->currentPage()->initialMargins.setRight(rightMargin);
									m_Doc->reformPages(true);
								}
								else
								{
									m_Doc->addPage(pagecount);
									m_Doc->currentPage()->m_pageSize = "Custom";
									m_Doc->currentPage()->setInitialHeight(docHeight);
									m_Doc->currentPage()->setInitialWidth(docWidth);
									m_Doc->currentPage()->setHeight(docHeight);
									m_Doc->currentPage()->setWidth(docWidth);
									m_Doc->currentPage()->initialMargins.setTop(topMargin);
									m_Doc->currentPage()->initialMargins.setBottom(bottomMargin);
									m_Doc->currentPage()->initialMargins.setLeft(leftMargin);
									m_Doc->currentPage()->initialMargins.setRight(rightMargin);
									m_Doc->currentPage()->MPageNam = CommonStrings::trMasterPageNormal;
									m_Doc->view()->addPage(pagecount, true);
									pagecount++;
								}
								m_Doc->applyMasterPage(spp.attribute("draw:master-page-name"), m_Doc->currentPageNumber());
							}
							firstPage = false;
							baseX = m_Doc->currentPage()->xOffset();
							baseY = m_Doc->currentPage()->yOffset();
							for(QDomElement spe = spp.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
							{
								PageItem* retObj = parseObj(spe);
								if (retObj != NULL)
								{
									m_Doc->Items->append(retObj);
									Elements.append(retObj);
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}

PageItem* OdgPlug::parseObj(QDomElement &draw)
{
	StoryText itemText;
	itemText.clear();
	itemText.setDoc(m_Doc);
	PageItem *retObj = NULL;
	if (draw.tagName() == "draw:g")
	{
		QList<PageItem*> GElements;
		int gLayer = -1;
		for(QDomElement spd = draw.firstChildElement(); !spd.isNull(); spd = spd.nextSiblingElement() )
		{
			PageItem* ite = parseObj(spd);
			if (ite != NULL)
			{
				GElements.append(ite);
				gLayer = ite->LayerID;
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
			int z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None);
			retObj = m_Doc->Items->at(z);
			retObj->ClipEdited = true;
			retObj->FrameType = 3;
			retObj->setFillEvenOdd(false);
			retObj->OldB2 = retObj->width();
			retObj->OldH2 = retObj->height();
			retObj->updateClip();
			m_Doc->groupObjectsToItem(retObj, GElements);
			retObj->OwnPage = m_Doc->OnPage(retObj);
			m_Doc->GroupOnPage(retObj);
			m_Doc->Items->removeLast();
			if (gLayer > -1)
				retObj->setLayer(gLayer);
		}
	}
	else if (draw.tagName() == "draw:polygon")
		retObj = parsePolygon(draw);
	else if (draw.tagName() == "draw:polyline")
		retObj = parsePolyline(draw);
	else if (draw.tagName() == "draw:path")
		retObj = parsePath(draw);
	else if (draw.tagName() == "draw:rect")
		retObj = parseRect(draw);
	else if (draw.tagName() == "draw:circle" || draw.tagName() == "draw:ellipse")
		retObj = parseEllipse(draw);
	else if (draw.tagName() == "draw:line")
		retObj = parseLine(draw);
	else if (draw.tagName() == "draw:frame")
		retObj = parseFrame(draw);
	else if (draw.tagName() == "draw:measure")
		retObj = parseMeasure(draw);
	else if (draw.tagName() == "draw:custom-shape")
		retObj = parseCustomShape(draw);
	else if (draw.tagName() == "draw:connector")
		retObj = parseConnector(draw);
	else if (draw.tagName() == "office:forms")
		retObj = parseForm(draw);
	else
		qDebug() << "Unhandled Tag" << draw.tagName();
	if (retObj != NULL)
	{
		if (draw.hasAttribute("draw:layer"))
		{
			if (m_Layers.contains(draw.attribute("draw:layer")))
				retObj->setLayer(m_Layers[draw.attribute("draw:layer")]);
		}
	}
	return retObj;
}

PageItem* OdgPlug::parseForm(QDomElement &e)
{
	PageItem *retObj = NULL;
	if (e.hasChildNodes())
		qDebug() << "Unhandled Tag" << e.tagName();
	return retObj;
}

PageItem* OdgPlug::parseConnector(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	if (e.hasAttribute("svg:d"))
	{
		FPointArray pArray;
		pArray.svgInit();
		pArray.parseSVG(e.attribute("svg:d"));
		if (pArray.size() > 3)
		{
			int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, CommonStrings::None, tmpOStyle.CurrColorStroke);
			retObj = m_Doc->Items->at(z);
			retObj->PoLine = pArray.copy();
			QTransform mat;
			mat.scale(72.0 / 2540.0, 72.0 / 2540.0);
			retObj->PoLine.map(mat);
			if (e.hasAttribute("draw:transform"))
				parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
			finishItem(retObj, tmpOStyle);
			m_Doc->Items->removeLast();
			if ((!tmpOStyle.startMarkerName.isEmpty()) || (!tmpOStyle.endMarkerName.isEmpty()))
			{
				QList<PageItem*> GElements;
				GElements.append(retObj);
				PageItem* startArrow = applyStartArrow(retObj, tmpOStyle);
				if (startArrow != NULL)
					GElements.append(startArrow);
				PageItem* endArrow = applyEndArrow(retObj, tmpOStyle);
				if (endArrow != NULL)
					GElements.append(endArrow);
				if (GElements.count() > 1)
					retObj = groupObjects(GElements);
			}
		}
	}
	else if (e.hasAttribute("svg:x1") && e.hasAttribute("svg:x2") && e.hasAttribute("svg:y1") && e.hasAttribute("svg:y2"))
		retObj = parseLine(e);
	return retObj;
}

PageItem* OdgPlug::parseCustomShape(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	QList<PageItem*> GElements;
	double x = parseUnit(e.attribute("svg:x"));
	double y = parseUnit(e.attribute("svg:y")) ;
	double w = parseUnit(e.attribute("svg:width"));
	double h = parseUnit(e.attribute("svg:height"));
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	bool has_Text = false;
	for(QDomElement p = e.firstChildElement(); !p.isNull(); p = p.nextSiblingElement())
	{
		if (p.tagName() == "text:p")
		{
			if (p.hasChildNodes())
				has_Text = true;
		}
	}
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0) && (!has_Text))
		return retObj;
	QPolygonF texAreaPoints;
	for(QDomElement p = e.firstChildElement(); !p.isNull(); p = p.nextSiblingElement())
	{
		if (p.tagName() == "draw:enhanced-geometry")
		{
			FunctionParser fpa;
			double vx = 0;
			double vy = 0;
			double vw = 21600;
			double vh = 21600;
			if (p.hasAttribute("svg:viewBox"))
				parseViewBox(p, &vx, &vy, &vw, &vh);
			if (vw == 0)
				vw = 21600;
			if (vh == 0)
				vh = 21600;
			fpa.AddConstant("top", vy);
			fpa.AddConstant("bottom", vh);
			fpa.AddConstant("left", vx);
			fpa.AddConstant("right", vw);
			fpa.AddConstant("width", vw - vx);
			fpa.AddConstant("height", vh - vy);
			fpa.AddConstant("xstretch", parseUnit(p.attribute("draw:path-stretchpoint-x", "0")));
			fpa.AddConstant("ystretch", parseUnit(p.attribute("draw:path-stretchpoint-y", "0")));
			fpa.AddConstant("hasfill", tmpOStyle.fill_type == 0 ? 0 : 1);
			fpa.AddConstant("hasstroke", tmpOStyle.stroke_type == 0 ? 0 : 1);
			fpa.AddConstant("logheight", vh);
			fpa.AddConstant("logwidth", vw);
			fpa.AddConstant("pi", M_PI);
			QString enhPath = p.attribute("draw:enhanced-path");
			QString textArea = p.attribute("draw:text-areas");
			if (!textArea.isEmpty())
				textArea.append(" ");
			QMap<QString, QString> func_Results;
			QMap<QString, QString> modi_Values;
			QString mods = p.attribute("draw:modifiers");
			ScTextStream Code(&mods, QIODevice::ReadOnly);
			int modCount = 0;
			while (!Code.atEnd())
			{
				double d;
				Code >> d;
				QString modName = QString("Const_%1").arg(modCount);
				fpa.AddConstant(modName.toStdString(), d);
				modi_Values.insert(QString("$%1").arg(modCount), QString("%1").arg(d));
				modCount++;
			}
			if (p.hasChildNodes())
			{
				QMap<QString, QString> formulaMap;
				for(QDomElement f = p.firstChildElement(); !f.isNull(); f = f.nextSiblingElement())
				{
					if (f.tagName() == "draw:equation")
					{
						QString formName = f.attribute("draw:name");
						QString formula = f.attribute("draw:formula", "0");
						formula.replace("$", "Const_");
						formula.replace("?", "Func_");
						formula.replace("if(", "if(0<");
						formulaMap.insert(formName, formula);
					}
				}
				if (!formulaMap.isEmpty())
				{
					int maxTry = formulaMap.count() + 1;
					int actTry = 0;
					bool allResOK = false;
					while (!allResOK)
					{
						allResOK = true;
						QMap<QString, QString>::iterator itf = formulaMap.begin();
						while (itf != formulaMap.end())
						{
							double erg = 0;
							int ret = fpa.Parse(itf.value().toStdString(), "", false);
							if(ret < 0)
							{
								QString formNam = itf.key();
								erg = fpa.Eval(NULL);
								func_Results.insert("?" + formNam + " ", QString("%1 ").arg(erg));
								formNam.prepend("Func_");
								fpa.AddConstant(formNam.toStdString(), erg);
								itf = formulaMap.erase(itf);
							}
							else
							{
								++itf;
								allResOK = false;
							}
						}
						actTry++;
						if (actTry > maxTry)
							break;
						if (formulaMap.isEmpty())
							break;
					}
				}
			}
			if (!modi_Values.isEmpty())
			{
				QMapIterator<QString, QString> it(modi_Values);
				it.toBack();
				while (it.hasPrevious())
				{
					it.previous();
					enhPath.replace(it.key(), it.value());
				}
			}
			if (!func_Results.isEmpty())
			{
				QMapIterator<QString, QString> it(func_Results);
				it.toBack();
				while (it.hasPrevious())
				{
					it.previous();
					enhPath.replace(it.key(), it.value());
					if (!textArea.isEmpty())
						textArea.replace(it.key(), it.value());
				}
			}
			if (enhPath.contains("?"))
				return retObj;
			QTransform mat;
			double sx = (vw != 0.0) ? (w / vw) : w;
			double sy = (vh != 0.0) ? (h / vh) : h;
			mat.scale(sx, sy);
			if (!textArea.isEmpty())
			{
				QStringList points = textArea.replace( QRegExp(","), " ").simplified().split( ' ', QString::SkipEmptyParts );
				texAreaPoints.append(QPointF(ScCLocale::toDoubleC(points[0]), ScCLocale::toDoubleC(points[1])));
				texAreaPoints.append(QPointF(ScCLocale::toDoubleC(points[2]), ScCLocale::toDoubleC(points[3])));
				texAreaPoints = mat.map(texAreaPoints);
			}
			QString shapeType = p.attribute("draw:type");
			QStringList paths = enhPath.split("N", QString::SkipEmptyParts);
			if (!paths.isEmpty())
			{
				for (int a = 0; a < paths.count(); a++)
				{
					FPointArray pArray;
					pArray.svgInit();
					bool filled = true;
					bool stroked = true;
					PageItem::ItemType itype = parseEnhPath(paths[a], pArray, filled, stroked) ? PageItem::PolyLine : PageItem::Polygon;
					if (pArray.size() > 3)
					{
						QString fillC = tmpOStyle.CurrColorFill;
						if (!filled)
							fillC = CommonStrings::None;
						else
						{
							if (shapeType == "can")
							{
								if (a == 1)
									fillC = modifyColor(fillC, false, 110);
							}
							else if (shapeType == "cube")
							{
								if (a == 1)
									fillC = modifyColor(fillC, false, 110);
								else if (a == 2)
									fillC = modifyColor(fillC, true, 120);
							}
							else if (shapeType == "paper")
							{
								if (a == 1)
									fillC = modifyColor(fillC, true, 120);
							}
							else if (shapeType == "smiley")
							{
								if (a == 1)
									fillC = modifyColor(fillC, true, 120);
								else if (a == 2)
									fillC = modifyColor(fillC, true, 120);
							}
							else if (shapeType == "quad-bevel")
							{
								if (a == 1)
									fillC = modifyColor(fillC, false, 110);
								else if (a == 2)
									fillC = modifyColor(fillC, true, 150);
								else if (a == 3)
									fillC = modifyColor(fillC, true, 120);
								else if (a == 4)
									fillC = modifyColor(fillC, false, 120);
							}
							else if (shapeType == "col-60da8460")
							{
								if (a == 1)
									fillC = modifyColor(fillC, true, 150);
								else if (a == 2)
									fillC = modifyColor(fillC, true, 300);
								else if (a == 3)
									fillC = modifyColor(fillC, false, 120);
								else if (a == 4)
									fillC = modifyColor(fillC, false, 120);
								else if (a == 5)
									fillC = modifyColor(fillC, false, 120);
							}
							else if (shapeType == "col-502ad400")
							{
								if (a == 1)
									fillC = modifyColor(fillC, false, 110);
								else if (a == 2)
									fillC = modifyColor(fillC, true, 120);
								else if (a == 3)
									fillC = modifyColor(fillC, false, 120);
								else if (a == 4)
									fillC = modifyColor(fillC, false, 120);
							}
							else if (shapeType == "vertical-scroll")
							{
								if (a == 1)
									fillC = modifyColor(fillC, true, 120);
								else if (a == 2)
									fillC = modifyColor(fillC, true, 120);
							}
							else if (shapeType == "horizontal-scroll")
							{
								if (a == 1)
									fillC = modifyColor(fillC, true, 120);
								else if (a == 2)
									fillC = modifyColor(fillC, true, 120);
							}
						}
						QString strokeC = tmpOStyle.CurrColorStroke;
						if (!stroked)
							strokeC = CommonStrings::None;
						int z = m_Doc->itemAdd(itype, PageItem::Unspecified, baseX + x, baseY + y, w, h, tmpOStyle.LineW, fillC, strokeC);
						retObj = m_Doc->Items->at(z);
						retObj->PoLine = pArray.copy();
						retObj->setFillEvenOdd(true);
						double stretchScale = 1.0;
						bool hasStretch = false;
						if ((w > h) && p.hasAttribute("draw:path-stretchpoint-x"))
						{
							double stretch = parseUnit(p.attribute("draw:path-stretchpoint-x", "0"));
							FPoint tp2(getMaxClipF(&retObj->PoLine));
							stretchScale = w / h;
							double endX = tp2.x() * stretchScale;
							double delX = endX - tp2.x();
							for (int ap = 0; ap < retObj->PoLine.size(); ap++)
							{
								FPoint pt = retObj->PoLine[ap];
								if (pt.x() > stretch)
								{
									if (pt.x() == tp2.x())
										retObj->PoLine[ap].setX(endX);
									else
										retObj->PoLine[ap].setX(pt.x() + delX);
								}
							}
							stretchScale = h / 21600.0;
							hasStretch = true;
						}
						if ((h > w) && p.hasAttribute("draw:path-stretchpoint-y"))
						{
							double stretch = parseUnit(p.attribute("draw:path-stretchpoint-y", "0"));
							FPoint tp2(getMaxClipF(&retObj->PoLine));
							stretchScale = h / w;
							double endY = tp2.y() * stretchScale;
							double delY = endY - tp2.y();
							for (int ap = 0; ap < retObj->PoLine.size(); ap++)
							{
								FPoint pt = retObj->PoLine[ap];
								if (pt.y() > stretch)
								{
									if (pt.y() == tp2.y())
										retObj->PoLine[ap].setY(endY);
									else
										retObj->PoLine[ap].setY(pt.y() + delY);
								}
							}
							stretchScale = w / 21600.0;
							hasStretch = true;
						}
						if (hasStretch)
						{
							QTransform smat;
							smat.scale(stretchScale, stretchScale);
							retObj->PoLine.map(smat);
						}
						else
							retObj->PoLine.map(mat);
						if (e.hasAttribute("draw:transform"))
							parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
						finishItem(retObj, tmpOStyle);
						GElements.append(retObj);
						m_Doc->Items->removeLast();
					}
				}
				if (GElements.count() > 1)
					retObj = groupObjects(GElements);
				if (p.hasAttribute("draw:mirror-horizontal") && (p.attribute("draw:mirror-horizontal") == "true"))
					m_Doc->MirrorPolyH(retObj);
				if (p.hasAttribute("draw:mirror-vertical") && (p.attribute("draw:mirror-vertical") == "true"))
					m_Doc->MirrorPolyV(retObj);
			}
		}
	}
	if (has_Text)
	{
		double r = 0.0;
		if (e.hasAttribute("draw:transform"))
			parseTransform(e.attribute("draw:transform"), &r, &x, &y);
		double tx = x;
		double ty = y;
		double tw = w;
		double th = h;
		if (!texAreaPoints.isEmpty())
		{
			QTransform rmat;
			rmat.rotate(r);
			QPointF rt = rmat.map(texAreaPoints[0]);
			tx += rt.x();
			ty += rt.y();
			tw = texAreaPoints[1].x() - texAreaPoints[0].x();
			th = texAreaPoints[1].y() - texAreaPoints[0].y();
		}
		int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, baseX+tx, baseY+ty, tw, th, 0, CommonStrings::None, CommonStrings::None);
		retObj = m_Doc->Items->at(z);
		retObj->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
		retObj->setTextFlowMode(PageItem::TextFlowDisabled);
		retObj->setVerticalAlignment(tmpOStyle.verticalAlignment);
		finishItem(retObj, tmpOStyle);
		parseText(e, retObj, tmpOStyle);
		if (e.hasAttribute("draw:transform"))
			retObj->setRotation(r, true);
		m_Doc->Items->removeLast();
		GElements.append(retObj);
	}
	if (GElements.count() > 1)
		retObj = groupObjects(GElements);
	return retObj;
}

PageItem* OdgPlug::parseMeasure(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	QList<PageItem*> GElements;
	double x1 = e.attribute( "svg:x1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:x1" ) );
	double y1 = e.attribute( "svg:y1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:y1" ) );
	double x2 = e.attribute( "svg:x2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:x2" ) );
	double y2 = e.attribute( "svg:y2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:y2" ) );
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	if (tmpOStyle.measureDist == 0)
		tmpOStyle.measureDist = tmpOStyle.fontSize;
	QLineF refLine = QLineF(x1, y1, x2, y2);
	QLineF normRef = refLine.normalVector();
	normRef.setLength(tmpOStyle.measureDist);
	double dx = normRef.p2().x() - refLine.p1().x();
	double dy = normRef.p2().y() - refLine.p1().y();
	retObj = parseLine(e);
	if (retObj != NULL)
	{
		retObj->moveBy(dx, dy, true);
		GElements.append(retObj);
	}
	normRef.setLength(tmpOStyle.measureDist + tmpOStyle.fontSize * 1.2);
	if (normRef.length() != 0)
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, CommonStrings::None, tmpOStyle.CurrColorStroke);
		retObj = m_Doc->Items->at(z);
		retObj->PoLine.resize(4);
		retObj->PoLine.setPoint(0, FPoint(x1, y1));
		retObj->PoLine.setPoint(1, FPoint(x1, y1));
		retObj->PoLine.setPoint(2, FPoint(normRef.p2().x(), normRef.p2().y()));
		retObj->PoLine.setPoint(3, FPoint(normRef.p2().x(), normRef.p2().y()));
		if (e.hasAttribute("draw:transform"))
			parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
		finishItem(retObj, tmpOStyle);
		m_Doc->Items->removeLast();
		GElements.append(retObj);
	}
	QLineF refLine2 = QLineF(x2, y2, x1, y1);
	QLineF normRef2 = refLine2.normalVector();
	normRef2.setAngle(normRef2.angle() + 180);
	normRef2.setLength(tmpOStyle.measureDist + tmpOStyle.fontSize / 2.0);
	if (normRef2.length() != 0)
	{
		int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, CommonStrings::None, tmpOStyle.CurrColorStroke);
		retObj = m_Doc->Items->at(z);
		retObj->PoLine.resize(4);
		retObj->PoLine.setPoint(0, FPoint(x2, y2));
		retObj->PoLine.setPoint(1, FPoint(x2, y2));
		retObj->PoLine.setPoint(2, FPoint(normRef2.p2().x(), normRef2.p2().y()));
		retObj->PoLine.setPoint(3, FPoint(normRef2.p2().x(), normRef2.p2().y()));
		if (e.hasAttribute("draw:transform"))
			parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
		finishItem(retObj, tmpOStyle);
		m_Doc->Items->removeLast();
		GElements.append(retObj);
	}
	normRef2.setLength(tmpOStyle.measureDist + tmpOStyle.fontSize * 1.2);
	QLineF textLine = QLineF(normRef.p2(), normRef2.p2());
	if (textLine.length() != 0)
	{
		int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, baseX+normRef.p2().x(), baseY+normRef.p2().y(), textLine.length(), tmpOStyle.fontSize * 1.2, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke);
		retObj = m_Doc->Items->at(z);
		retObj->setFillColor(tmpOStyle.CurrColorFill);
		retObj->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
		retObj->setTextFlowMode(PageItem::TextFlowDisabled);
		finishItem(retObj, tmpOStyle);
		retObj->setRotation(-textLine.angle(), true);
		parseText(e, retObj, tmpOStyle);
		m_Doc->Items->removeLast();
		GElements.append(retObj);
	}
	if (GElements.count() > 1)
		retObj = groupObjects(GElements);
	return retObj;
}

PageItem* OdgPlug::parseLine( QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	double x1 = e.attribute( "svg:x1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:x1" ) );
	double y1 = e.attribute( "svg:y1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:y1" ) );
	double x2 = e.attribute( "svg:x2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:x2" ) );
	double y2 = e.attribute( "svg:y2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:y2" ) );
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	if (tmpOStyle.stroke_type == 0)
		return retObj;
	int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, CommonStrings::None, tmpOStyle.CurrColorStroke);
	retObj = m_Doc->Items->at(z);
	retObj->PoLine.resize(4);
	retObj->PoLine.setPoint(0, FPoint(x1, y1));
	retObj->PoLine.setPoint(1, FPoint(x1, y1));
	retObj->PoLine.setPoint(2, FPoint(x2, y2));
	retObj->PoLine.setPoint(3, FPoint(x2, y2));
	if (e.hasAttribute("draw:transform"))
		parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
	finishItem(retObj, tmpOStyle);
	m_Doc->Items->removeLast();
	if ((!tmpOStyle.startMarkerName.isEmpty()) || (!tmpOStyle.endMarkerName.isEmpty()))
	{
		QList<PageItem*> GElements;
		GElements.append(retObj);
		PageItem* startArrow = applyStartArrow(retObj, tmpOStyle);
		if (startArrow != NULL)
			GElements.append(startArrow);
		PageItem* endArrow = applyEndArrow(retObj, tmpOStyle);
		if (endArrow != NULL)
			GElements.append(endArrow);
		if (GElements.count() > 1)
			retObj = groupObjects(GElements);
	}
	return retObj;
}

PageItem* OdgPlug::parseEllipse(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	double x = parseUnit(e.attribute("svg:x"));
	double y = parseUnit(e.attribute("svg:y")) ;
	double w = parseUnit(e.attribute("svg:width"));
	double h = parseUnit(e.attribute("svg:height"));
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke);
	retObj = m_Doc->Items->at(z);
	if (e.hasAttribute("draw:transform"))
		parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
	finishItem(retObj, tmpOStyle);
	m_Doc->Items->removeLast();
	return retObj;
}

PageItem* OdgPlug::parseRect(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	double x = parseUnit(e.attribute("svg:x"));
	double y = parseUnit(e.attribute("svg:y")) ;
	double w = parseUnit(e.attribute("svg:width"));
	double h = parseUnit(e.attribute("svg:height"));
	double corner = parseUnit(e.attribute("draw:corner-radius"));
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke);
	retObj = m_Doc->Items->at(z);
	if (corner != 0)
	{
		retObj->setCornerRadius(corner);
		retObj->SetFrameRound();
		m_Doc->setRedrawBounding(retObj);
	}
	if (e.hasAttribute("draw:transform"))
		parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
	finishItem(retObj, tmpOStyle);
	m_Doc->Items->removeLast();
	return retObj;
}

PageItem* OdgPlug::parsePolygon(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke);
	retObj = m_Doc->Items->at(z);
	retObj->PoLine.resize(0);
	appendPoints(&retObj->PoLine, e, true);
	if (e.hasAttribute("draw:transform"))
		parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
	finishItem(retObj, tmpOStyle);
	m_Doc->Items->removeLast();
	return retObj;
}

PageItem* OdgPlug::parsePolyline(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	if (tmpOStyle.stroke_type == 0)
		return retObj;
	int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, CommonStrings::None, tmpOStyle.CurrColorStroke);
	retObj = m_Doc->Items->at(z);
	retObj->PoLine.resize(0);
	appendPoints(&retObj->PoLine, e, false);
	if (e.hasAttribute("draw:transform"))
		parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
	finishItem(retObj, tmpOStyle);
	m_Doc->Items->removeLast();
	if ((!tmpOStyle.startMarkerName.isEmpty()) || (!tmpOStyle.endMarkerName.isEmpty()))
	{
		QList<PageItem*> GElements;
		GElements.append(retObj);
		PageItem* startArrow = applyStartArrow(retObj, tmpOStyle);
		if (startArrow != NULL)
			GElements.append(startArrow);
		PageItem* endArrow = applyEndArrow(retObj, tmpOStyle);
		if (endArrow != NULL)
			GElements.append(endArrow);
		if (GElements.count() > 1)
			retObj = groupObjects(GElements);
	}
	return retObj;
}

PageItem* OdgPlug::parsePath(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	FPointArray pArray;
	pArray.svgInit();
	PageItem::ItemType itype = pArray.parseSVG(e.attribute("svg:d")) ? PageItem::PolyLine : PageItem::Polygon;
	if (pArray.size() > 3)
	{
		double x = parseUnit(e.attribute("svg:x"));
		double y = parseUnit(e.attribute("svg:y")) ;
		double w = parseUnit(e.attribute("svg:width"));
		double h = parseUnit(e.attribute("svg:height"));
		int z = m_Doc->itemAdd(itype, PageItem::Unspecified, baseX + x, baseY + y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke);
		retObj = m_Doc->Items->at(z);
		retObj->PoLine = pArray.copy();
		QTransform mat;
		double vx = 0;
		double vy = 0;
		double vw = 1;
		double vh = 1;
		parseViewBox(e, &vx, &vy, &vw, &vh);
		double sx = (vw != 0.0) ? (w / vw) : w;
		double sy = (vh != 0.0) ? (h / vh) : h;
		mat.scale(sx, sy);
		retObj->PoLine.map(mat);
		if (e.hasAttribute("draw:transform"))
		{
			FPoint tp2(getMinClipF(&retObj->PoLine));
			retObj->PoLine.translate(-tp2.x(), -tp2.y());
			parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
		}
		finishItem(retObj, tmpOStyle);
		m_Doc->Items->removeLast();
		if (itype == PageItem::PolyLine)
		{
			if ((!tmpOStyle.startMarkerName.isEmpty()) || (!tmpOStyle.endMarkerName.isEmpty()))
			{
				QList<PageItem*> GElements;
				GElements.append(retObj);
				PageItem* startArrow = applyStartArrow(retObj, tmpOStyle);
				if (startArrow != NULL)
					GElements.append(startArrow);
				PageItem* endArrow = applyEndArrow(retObj, tmpOStyle);
				if (endArrow != NULL)
					GElements.append(endArrow);
				if (GElements.count() > 1)
					retObj = groupObjects(GElements);
			}
		}
	}
	return retObj;
}

PageItem* OdgPlug::parseFrame(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	double x = parseUnit(e.attribute("svg:x"));
	double y = parseUnit(e.attribute("svg:y")) ;
	double w = parseUnit(e.attribute("svg:width"));
	double h = parseUnit(e.attribute("svg:height"));
	double r = 0.0;
	if (e.hasAttribute("draw:transform"))
		parseTransform(e.attribute("draw:transform"), &r, &x, &y);
	resovleStyle(tmpOStyle, "standard");
	resovleStyle(tmpOStyle, getStyleName(e));
	QDomElement n = e.firstChildElement();
	if (!n.isNull())
	{
		if (n.tagName() == "draw:text-box" )
		{
			if (n.text().isEmpty())
				return retObj;
			int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke);
			retObj = m_Doc->Items->at(z);
			retObj->setFillColor(tmpOStyle.CurrColorFill);
			retObj->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
			retObj->setTextFlowMode(PageItem::TextFlowDisabled);
			retObj->setVerticalAlignment(tmpOStyle.verticalAlignment);
			if (e.hasAttribute("draw:transform"))
				retObj->setRotation(r, true);
			finishItem(retObj, tmpOStyle);
			parseText(n, retObj, tmpOStyle);
			m_Doc->Items->removeLast();
		}
		else if (n.tagName() == "draw:image" )
		{
			QString imagePath = n.attribute("xlink:href", "");
			if (!imagePath.isEmpty())
			{
				QFileInfo fi(imagePath);
				QString ext = fi.suffix().toLower();
				QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::IMAGESIMGFRAME, 1));
				QStringList formats = formatD.split("|");
				formats.removeAll("pdf");
				QStringList allFormatsV = LoadSavePlugin::getExtensionsForImport(FORMATID_FIRSTUSER);
				if (formats.contains(ext.toUtf8()))
				{
					int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke);
					retObj = m_Doc->Items->at(z);
					if (e.hasAttribute("draw:transform"))
						retObj->setRotation(r, true);
					finishItem(retObj, tmpOStyle);
					QByteArray f;
					if (uz->read(imagePath, f))
					{
						QFileInfo fi(imagePath);
						QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_odg_XXXXXX." + fi.suffix());
						tempFile->setAutoRemove(false);
						if (tempFile->open())
						{
							QString fileName = getLongPathName(tempFile->fileName());
							if (!fileName.isEmpty())
							{
								tempFile->write(f);
								tempFile->close();
								retObj->isInlineImage = true;
								retObj->isTempFile = true;
								retObj->AspectRatio = false;
								retObj->ScaleType   = false;
								m_Doc->loadPict(fileName, retObj);
								retObj->AdjustPictScale();
							}
						}
						delete tempFile;
					}
					m_Doc->Items->removeLast();
				}
				else if (allFormatsV.contains(ext.toUtf8()))
				{
					QByteArray f;
					if (uz->read(imagePath, f))
					{
						QFileInfo fi(imagePath);
						QString ext = fi.suffix();
						if (ext == "wmf")
						{
							if ((f[0] == '\x01') && (f[1] == '\x00') && (f[2] == '\x00') && (f[3] == '\x00') && (f[40] == '\x20') && (f[41] == '\x45') && (f[42] == '\x4D') && (f[43] == '\x46'))
								ext = "emf";
						}
						retObj = getVectorFileFromData(m_Doc, f, ext, baseX + x, baseY + y, w, h);
						if (retObj != NULL)
							m_Doc->Items->removeLast();
					}
				}
			}
			else if (n.hasChildNodes())
			{
				for(QDomElement nc = n.firstChildElement(); !nc.isNull(); nc = nc.nextSiblingElement())
				{
					if (nc.tagName() == "office:binary-data")
					{
						QString ext = "";
						QByteArray buf = QByteArray::fromBase64(nc.text().toLatin1());
						if ((buf[0] == '%') && (buf[1] == '!') && (buf[2] == 'P') && (buf[3] == 'S') && (buf[4] == '-') && (buf[5] == 'A'))
							ext = "eps";
						else if ((buf[0] == '\xC5') && (buf[1] == '\xD0') && (buf[2] == '\xD3') && (buf[3] == '\xC6'))
							ext = "eps";
						else if ((buf[0] == 'G') && (buf[1] == 'I') && (buf[2] == 'F') && (buf[3] == '8'))
							ext = "gif";
						else if ((buf[0] == '\xFF') && (buf[1] == '\xD8') && (buf[2] == '\xFF'))
							ext = "jpg";
						else if ((buf[0] == '%') && (buf[1] == 'P') && (buf[2] == 'D') && (buf[3] == 'F'))
							ext = "pdf";
						else if ((buf[0] == 'P') && (buf[1] == 'G') && (buf[2] == 'F'))
							ext = "pgf";
						else if ((buf[0] == '\x89') && (buf[1] == 'P') && (buf[2] == 'N') && (buf[3] == 'G'))
							ext = "png";
						else if ((buf[0] == '8') && (buf[1] == 'B') && (buf[2] == 'P') && (buf[3] == 'S'))
							ext = "psd";
						else if (((buf[0] == 'I') && (buf[1] == 'I') && (buf[2] == '\x2A')) || ((buf[0] == 'M') && (buf[1] == 'M') && (buf[3] == '\x2A')))
							ext = "tif";
						else if ((buf[0] == '/') && (buf[1] == '*') && (buf[2] == ' ') && (buf[3] == 'X') && (buf[4] == 'P') && (buf[5] == 'M'))
							ext = "xpm";
						else if ((buf[0] == '\xD7') && (buf[1] == '\xCD') && (buf[2] == '\xC6') && (buf[3] == '\x9A'))
							ext = "wmf";
						else if ((buf[0] == '\x01') && (buf[1] == '\x00') && (buf[2] == '\x00') && (buf[3] == '\x00') && (buf[40] == '\x20') && (buf[41] == '\x45') && (buf[42] == '\x4D') && (buf[43] == '\x46'))
							ext = "emf";
						else if ((buf[0] == '<') && (buf[1] == '?') && (buf[2] == 'x') && (buf[3] == 'm') && (buf[4] == 'l'))
							ext = "svg";
						else if ((buf[0] == 'V') && (buf[1] == 'C') && (buf[2] == 'L') && (buf[3] == 'M') && (buf[4] == 'T') && (buf[5] == 'F'))
							ext = "svm";
						if (!ext.isEmpty())
						{
							if ((ext == "eps") || (ext == "wmf") || (ext == "emf") || (ext == "svg") || (ext == "svm"))
							{
								retObj = getVectorFileFromData(m_Doc, buf, ext, baseX + x, baseY + y, w, h);
								if (retObj != NULL)
									m_Doc->Items->removeLast();
							}
							else
							{
								int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke);
								retObj = m_Doc->Items->at(z);
								if (e.hasAttribute("draw:transform"))
									retObj->setRotation(r, true);
								finishItem(retObj, tmpOStyle);
								QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_odg_XXXXXX." + ext);
								tempFile->setAutoRemove(false);
								if (tempFile->open())
								{
									QString fileName = getLongPathName(tempFile->fileName());
									if (!fileName.isEmpty())
									{
										tempFile->write(buf);
										tempFile->close();
										retObj->isInlineImage = true;
										retObj->isTempFile = true;
										retObj->AspectRatio = false;
										retObj->ScaleType   = false;
										m_Doc->loadPict(fileName, retObj);
										retObj->AdjustPictScale();
									}
								}
								delete tempFile;
								m_Doc->Items->removeLast();
							}
						}
					}
				}
			}
		}
	}
	return retObj;
}

void OdgPlug::parseText(QDomElement &elem, PageItem* item, ObjStyle& tmpOStyle)
{
	int posC = 0;
	QString pStyleD = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setDefaultStyle(false);
	newStyle.setParent(pStyleD);
	ParagraphStyle ttx = m_Doc->paragraphStyle(pStyleD);
	CharStyle nstyle = ttx.charStyle();
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	newStyle.setLineSpacing(nstyle.fontSize() / 10.0);
	item->itemText.clear();
	item->itemText.setDefaultStyle(newStyle);
	item->setFirstLineOffset(FLOPFontAscent);
	ObjStyle pStyle = tmpOStyle;
	if (elem.hasAttribute("text:style-name"))
		resovleStyle(pStyle, elem.attribute("text:style-name"));
	for(QDomElement para = elem.firstChildElement(); !para.isNull(); para = para.nextSiblingElement())
	{
		pStyle = tmpOStyle;
		if ((para.tagName() != "text:p") && (para.tagName() != "text:list") && (para.tagName() != "text:h"))
			continue;
		if (para.hasChildNodes())
		{
			if (para.hasAttribute("text:style-name"))
				resovleStyle(pStyle, para.attribute("text:style-name"));
			ParagraphStyle tmpStyle = newStyle;
			applyParagraphStyle(tmpStyle, pStyle);
			double maxFsize = 0.0;
			if (para.firstChildElement().isNull())
			{
				CharStyle tmpCStyle = tmpStyle.charStyle();
				applyCharacterStyle(tmpCStyle, tmpOStyle);
				maxFsize = qMax(maxFsize, tmpOStyle.fontSize);
				QString txt = para.text();
				insertChars(item, txt, tmpStyle, tmpCStyle, posC);
			}
			else
			{
				for(QDomNode spn = para.firstChild(); !spn.isNull(); spn = spn.nextSibling())
				{
					CharStyle tmpCStyle = tmpStyle.charStyle();
					QDomElement sp = spn.toElement();
					ObjStyle cStyle = pStyle;
					if (spn.isElement() && (sp.tagName() == "text:span"))
					{
						if (sp.hasAttribute("text:style-name"))
							resovleStyle(cStyle, sp.attribute("text:style-name"));
					}
					applyCharacterStyle(tmpCStyle, cStyle);
					maxFsize = qMax(maxFsize, cStyle.fontSize);
					QString txt = "";
					if (spn.isElement())
					{
						if (sp.tagName() == "text:span")
						{
							if (sp.tagName() == "text:s")
								txt = " ";
							else if (sp.tagName() == "text:tab")
								txt = SpecialChars::TAB;
							else if (sp.tagName() == "text:line-break")
								txt = SpecialChars::LINEBREAK;
							else
								txt = sp.text();
						}
						else if (sp.tagName() == "text:measure")
						{
							QString kind = sp.attribute("text:kind");
							if (kind == "value")
								txt += sp.text();
							else if (kind == "unit")
								txt += " " + sp.text();
						}
						else if (sp.tagName() == "text:list-item")
						{
							for(QDomElement paral = sp.firstChildElement(); !paral.isNull(); paral = paral.nextSiblingElement())
							{
								ObjStyle plStyle = tmpOStyle;
								if (paral.hasAttribute("text:style-name"))
									resovleStyle(plStyle, paral.attribute("text:style-name"));
								ParagraphStyle tmpStyle = newStyle;
								applyParagraphStyle(tmpStyle, plStyle);
								for(QDomNode spnl = paral.firstChild(); !spnl.isNull(); spnl = spnl.nextSibling())
								{
									CharStyle tmpCStyle = tmpStyle.charStyle();
									QDomElement spl = spnl.toElement();
									ObjStyle clStyle = plStyle;
									if (spnl.isElement() && (spl.tagName() == "text:span"))
									{
										if (spl.hasAttribute("text:style-name"))
											resovleStyle(clStyle, spl.attribute("text:style-name"));
									}
									applyCharacterStyle(tmpCStyle, clStyle);
									maxFsize = qMax(maxFsize, clStyle.fontSize);
									if (spnl.isElement())
									{
										if (spl.tagName() == "text:span")
										{
											if (spl.tagName() == "text:s")
												txt = " ";
											else if (spl.tagName() == "text:tab")
												txt = SpecialChars::TAB;
											else if (spl.tagName() == "text:line-break")
												txt = SpecialChars::LINEBREAK;
											else
												txt = spl.text();
										}
									}
									else if (spnl.isText())
									{
										QDomText t = spnl.toText();
									}
									insertChars(item, txt, tmpStyle, tmpCStyle, posC);
								}
								item->itemText.insertChars(posC, SpecialChars::PARSEP);
								item->itemText.applyStyle(posC, tmpStyle);
								posC = item->itemText.length();
							}
						}
					}
					else if (spn.isText())
					{
						QDomText t = spn.toText();
						txt = t.data();
					}
					insertChars(item, txt, tmpStyle, tmpCStyle, posC);
				}
			}
			if (pStyle.lineHeight < 0.0)
				tmpStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
			else
			{
				tmpStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
				if (pStyle.absLineHeight)
					tmpStyle.setLineSpacing(pStyle.lineHeight);
				else
					tmpStyle.setLineSpacing(pStyle.lineHeight * maxFsize);
			}
			item->itemText.insertChars(posC, SpecialChars::PARSEP);
			item->itemText.applyStyle(posC, tmpStyle);
			posC = item->itemText.length();
		}
		else
		{
			QString txt = para.text();
			ParagraphStyle tmpStyle = newStyle;
			applyParagraphStyle(tmpStyle, tmpOStyle);
			CharStyle tmpCStyle = tmpStyle.charStyle();
			applyCharacterStyle(tmpCStyle, tmpOStyle);
			if (tmpOStyle.lineHeight < 0.0)
				tmpStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
			else
			{
				tmpStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
				if (tmpOStyle.absLineHeight)
					tmpStyle.setLineSpacing(tmpOStyle.lineHeight);
				else
					tmpStyle.setLineSpacing(tmpOStyle.lineHeight * tmpOStyle.fontSize);
			}
			insertChars(item, txt, tmpStyle, tmpCStyle, posC);
			item->itemText.insertChars(posC, SpecialChars::PARSEP);
			item->itemText.applyStyle(posC, tmpStyle);
			posC = item->itemText.length();
		}
	}
	item->itemText.trim();
}

void OdgPlug::insertChars(PageItem *item, QString &txt, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, int &posC)
{
	if (txt.length() > 0)
	{
		item->itemText.insertChars(posC, txt);
		item->itemText.applyStyle(posC, tmpStyle);
		item->itemText.applyCharStyle(posC, txt.length(), tmpCStyle);
		posC = item->itemText.length();
		txt = "";
	}
}

void OdgPlug::applyCharacterStyle(CharStyle &tmpCStyle, ObjStyle &oStyle)
{
	tmpCStyle.setFont((*m_Doc->AllFonts)[oStyle.fontName]);
	tmpCStyle.setFontSize(oStyle.fontSize * 10);
	tmpCStyle.setFillColor(oStyle.CurrColorText);
	StyleFlag styleEffects = tmpCStyle.effects();
	if ((oStyle.textPos.startsWith("super")) || (oStyle.textPos.startsWith("sub")))
	{
		if (oStyle.textPos.startsWith("super"))
			styleEffects |= ScStyle_Superscript;
		else
			styleEffects |= ScStyle_Subscript;
	}
	if (oStyle.textOutline == "true")
	{
		styleEffects |= ScStyle_Outline;
		tmpCStyle.setOutlineWidth(30);
		tmpCStyle.setFillColor("White");
		tmpCStyle.setStrokeColor(oStyle.CurrColorText);
	}
	if (oStyle.textUnderline)
	{
		styleEffects |= ScStyle_Underline;
		tmpCStyle.setUnderlineOffset(-1);
		tmpCStyle.setUnderlineWidth(-1);
		tmpCStyle.setStrokeColor(oStyle.textUnderlineColor);
	}
	if (oStyle.textStrikeThrough)
	{
		if (oStyle.textUnderlineWords)
			styleEffects |= ScStyle_UnderlineWords;
		else
			styleEffects |= ScStyle_Strikethrough;
		tmpCStyle.setStrikethruOffset(-1);
		tmpCStyle.setStrikethruWidth(-1);
		tmpCStyle.setStrokeColor(oStyle.CurrColorText);
	}
	if (oStyle.textShadow)
	{
		styleEffects |= ScStyle_Shadowed;
		tmpCStyle.setShadowXOffset(30);
		tmpCStyle.setShadowYOffset(-30);
		tmpCStyle.setStrokeColor(oStyle.CurrColorText);
	}
	tmpCStyle.setFeatures(styleEffects.featureList());
}

void OdgPlug::applyParagraphStyle(ParagraphStyle &tmpStyle, ObjStyle &oStyle)
{
	tmpStyle.setAlignment(oStyle.textAlign);
	tmpStyle.setLeftMargin(oStyle.margin_left);
	tmpStyle.setRightMargin(oStyle.margin_right);
	tmpStyle.setFirstIndent(oStyle.textIndent);
	tmpStyle.setGapAfter(oStyle.margin_bottom);
	tmpStyle.setGapBefore(oStyle.margin_top);
}

void OdgPlug::parseTransform(const QString &transform, double *rotation, double *transX, double *transY)
{
	double dx, dy;
	QStringList subtransforms = transform.split(')', QString::SkipEmptyParts);
	QStringList::ConstIterator it = subtransforms.begin();
	QStringList::ConstIterator end = subtransforms.end();
	for (; it != end; ++it)
	{
		QStringList subtransform = (*it).split('(', QString::SkipEmptyParts);
		subtransform[0] = subtransform[0].trimmed().toLower();
		subtransform[1] = subtransform[1].simplified();
		QRegExp reg("[,( ]");
		QStringList params = subtransform[1].split(reg, QString::SkipEmptyParts);
		if(subtransform[0].startsWith(";") || subtransform[0].startsWith(","))
			subtransform[0] = subtransform[0].right(subtransform[0].length() - 1);
		if(subtransform[0] == "rotate")
		{
			*rotation = -parseUnit(params[0]) * 180 / M_PI;
		}
		else if(subtransform[0] == "translate")
		{
			if(params.count() == 2)
			{
				dx = parseUnit(params[0]);
				dy = parseUnit(params[1]);
			}
			else
			{
				dx = parseUnit(params[0]);
				dy = 0.0;
			}
			*transX = dx;
			*transY = dy;
		}
	}
}

void OdgPlug::parseTransform(FPointArray *composite, const QString &transform)
{
	double dx, dy;
	QTransform result;
	QStringList subtransforms = transform.split(')', QString::SkipEmptyParts);
	QStringList::ConstIterator it = subtransforms.begin();
	QStringList::ConstIterator end = subtransforms.end();
	for (; it != end; ++it)
	{
		QStringList subtransform = (*it).split('(', QString::SkipEmptyParts);
		subtransform[0] = subtransform[0].trimmed().toLower();
		subtransform[1] = subtransform[1].simplified();
		QRegExp reg("[,( ]");
		QStringList params = subtransform[1].split(reg, QString::SkipEmptyParts);
		if(subtransform[0].startsWith(";") || subtransform[0].startsWith(","))
			subtransform[0] = subtransform[0].right(subtransform[0].length() - 1);
		if(subtransform[0] == "rotate")
		{
			result = QTransform();
			result.rotate(-parseUnit(params[0]) * 180 / M_PI);
			composite->map(result);
		}
		else if(subtransform[0] == "translate")
		{
			if(params.count() == 2)
			{
				dx = parseUnit(params[0]);
				dy = parseUnit(params[1]);
			}
			else
			{
				dx = parseUnit(params[0]);
				dy =0.0;
			}
			result = QTransform();
			result.translate(dx, dy);
			composite->map(result);
		}
		else if(subtransform[0] == "skewx")
		{
			result = QTransform();
			result.shear(-tan(ScCLocale::toDoubleC(params[0])), 0.0);
			composite->map(result);
		}
		else if(subtransform[0] == "skewy")
		{
			result = QTransform();
			result.shear(0.0, -tan(ScCLocale::toDoubleC(params[0])));
			composite->map(result);
		}
	}
}

void OdgPlug::parseViewBox( const QDomElement& object, double *x, double *y, double *w, double *h )
{
	if( !object.attribute( "svg:viewBox" ).isEmpty() )
	{
		QString viewbox( object.attribute( "svg:viewBox" ) );
		QStringList points = viewbox.replace( QRegExp(","), " ").simplified().split( ' ', QString::SkipEmptyParts );
		*x = ScCLocale::toDoubleC(points[0]);
		*y = ScCLocale::toDoubleC(points[1]);
		*w = ScCLocale::toDoubleC(points[2]);
		*h = ScCLocale::toDoubleC(points[3]);
	}
}

void OdgPlug::appendPoints(FPointArray *composite, const QDomElement& object, bool closePath)
{
	double x = parseUnit(object.attribute("svg:x"));
	double y = parseUnit(object.attribute("svg:y")) ;
	double w = parseUnit(object.attribute("svg:width"));
	double h = parseUnit(object.attribute("svg:height"));
	double vx = 0;
	double vy = 0;
	double vw = 1;
	double vh = 1;
	parseViewBox(object, &vx, &vy, &vw, &vh);
	double sx = (vw != 0.0) ? (w / vw) : w;
	double sy = (vh != 0.0) ? (h / vh) : h;
	QStringList ptList = object.attribute( "draw:points" ).split( ' ', QString::SkipEmptyParts );
	FPoint point, firstP;
	bool bFirst = true;
	for( QStringList::Iterator it = ptList.begin(); it != ptList.end(); ++it )
	{
		point = FPoint(ScCLocale::toDoubleC((*it).section( ',', 0, 0 )), ScCLocale::toDoubleC((*it).section( ',', 1, 1 )));
		if (bFirst)
		{
			composite->addPoint(point);
			composite->addPoint(point);
			firstP = point;
			bFirst = false;
		}
		else
		{
			composite->addPoint(point);
			composite->addPoint(point);
			composite->addPoint(point);
			composite->addPoint(point);
		}
	}
	if (closePath)
	{
		composite->addPoint(firstP);
		composite->addPoint(firstP);
	}
	QTransform mat;
	mat.translate(x, y);
	mat.scale(sx, sy);
	composite->map(mat);
}

void OdgPlug::parseStyles(QDomElement &sp)
{
	for(QDomElement spd = sp.firstChildElement(); !spd.isNull(); spd = spd.nextSiblingElement() )
	{
		if (spd.tagName() == "draw:marker")
		{
			DrawStyle currStyle;
			currStyle.markerPath = AttributeValue(spd.attribute("svg:d", ""));
			currStyle.markerViewBox = AttributeValue(spd.attribute("svg:viewBox", ""));
			QString id = spd.attribute("draw:display-name");
			QString id2 = spd.attribute("draw:name");
			if (id2.isEmpty())
				m_Styles.insert(id, currStyle);
			else
				m_Styles.insert(id2, currStyle);
		}
		else if (spd.tagName() == "draw:stroke-dash")
		{
			DrawStyle currStyle;
			currStyle.stroke_dash_distance = AttributeValue(spd.attribute("draw:distance", ""));
			currStyle.stroke_dash_dots1 = AttributeValue(spd.attribute("draw:dots1", ""));
			currStyle.stroke_dash_dots1_length = AttributeValue(spd.attribute("draw:dots1-length", ""));
			currStyle.stroke_dash_dots2 = AttributeValue(spd.attribute("draw:dots2", ""));
			currStyle.stroke_dash_dots2_length = AttributeValue(spd.attribute("draw:dots2-length", ""));
			currStyle.stroke_dash_style = AttributeValue(spd.attribute("draw:style", ""));
			QString id = spd.attribute("draw:display-name");
			QString id2 = spd.attribute("draw:name");
			if (id2.isEmpty())
				m_Styles.insert(id, currStyle);
			else
				m_Styles.insert(id2, currStyle);
		}
		else if (spd.tagName() == "draw:gradient")
		{
			DrawStyle currStyle;
			currStyle.gradientAngle = AttributeValue(spd.attribute("draw:angle", ""));
			currStyle.gradientBorder = AttributeValue(spd.attribute("draw:border", ""));
			currStyle.gradientEndColor = AttributeValue(spd.attribute("draw:end-color", ""));
			currStyle.gradientEndShade = AttributeValue(spd.attribute("draw:end-intensity", ""));
			currStyle.gradientStartColor = AttributeValue(spd.attribute("draw:start-color", ""));
			currStyle.gradientStartShade = AttributeValue(spd.attribute("draw:start-intensity", ""));
			currStyle.gradientCenterX = AttributeValue(spd.attribute("draw:cx", ""));
			currStyle.gradientCenterY = AttributeValue(spd.attribute("draw:cy", ""));
			currStyle.gradientType = AttributeValue(spd.attribute("draw:style", ""));
			QString id = spd.attribute("draw:display-name");
			QString id2 = spd.attribute("draw:name");
			if (id2.isEmpty())
				m_Styles.insert(id, currStyle);
			else
				m_Styles.insert(id2, currStyle);
		}
		else if (spd.tagName() == "draw:opacity")
		{
			DrawStyle currStyle;
			currStyle.gradientAngle = AttributeValue(spd.attribute("draw:angle", ""));
			currStyle.gradientBorder = AttributeValue(spd.attribute("draw:border", ""));
			currStyle.opacityEnd = AttributeValue(spd.attribute("draw:end", ""));
			currStyle.opacityStart = AttributeValue(spd.attribute("draw:start", ""));
			currStyle.gradientCenterX = AttributeValue(spd.attribute("draw:cx", ""));
			currStyle.gradientCenterY = AttributeValue(spd.attribute("draw:cy", ""));
			currStyle.gradientType = AttributeValue(spd.attribute("draw:style", ""));
			QString id = spd.attribute("draw:display-name");
			QString id2 = spd.attribute("draw:name");
			if (id2.isEmpty())
				m_Styles.insert(id, currStyle);
			else
				m_Styles.insert(id2, currStyle);
		}
		else if (spd.tagName() == "draw:hatch")
		{
			DrawStyle currStyle;
			currStyle.hatchColor = AttributeValue(spd.attribute("draw:color", ""));
			currStyle.hatchDistance = AttributeValue(spd.attribute("draw:distance", ""));
			currStyle.hatchRotation = AttributeValue(spd.attribute("draw:rotation", ""));
			currStyle.hatchStyle = AttributeValue(spd.attribute("draw:style", ""));
			QString id = spd.attribute("draw:display-name");
			QString id2 = spd.attribute("draw:name");
			if (id2.isEmpty())
				m_Styles.insert(id, currStyle);
			else
				m_Styles.insert(id2, currStyle);
		}
		else if (spd.tagName() == "draw:fill-image")
		{
			DrawStyle currStyle;
			currStyle.patternPath = AttributeValue(spd.attribute("xlink:href", ""));
			if (!currStyle.patternPath.valid)
			{
				if (spd.hasChildNodes())
				{
					for(QDomElement nc = spd.firstChildElement(); !nc.isNull(); nc = nc.nextSiblingElement())
					{
						if (nc.tagName() == "office:binary-data")
							currStyle.patternData = AttributeValue(nc.text());
					}
				}
			}
			QString id = spd.attribute("draw:display-name");
			QString id2 = spd.attribute("draw:name");
			if (id2.isEmpty())
				m_Styles.insert(id, currStyle);
			else
				m_Styles.insert(id2, currStyle);
		}
		else if (spd.tagName() == "style:style")
		{
			DrawStyle currStyle;
			for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
			{
				if (spe.tagName() == "style:graphic-properties")
				{
					currStyle.fillMode = AttributeValue(spe.attribute("draw:fill", ""));
					currStyle.CurrColorFill = AttributeValue(spe.attribute("draw:fill-color", ""));
					currStyle.strokeMode = AttributeValue(spe.attribute("draw:stroke", ""));
					currStyle.CurrColorStroke = AttributeValue(spe.attribute("svg:stroke-color", ""));
					currStyle.CurrColorShadow = AttributeValue(spe.attribute("draw:shadow-color", ""));
					currStyle.hasShadow = AttributeValue(spe.attribute("draw:shadow", ""));
					currStyle.shadowX = AttributeValue(spe.attribute("draw:shadow-offset-x", ""));
					currStyle.shadowY = AttributeValue(spe.attribute("draw:shadow-offset-y", ""));
					currStyle.shadowTrans = AttributeValue(spe.attribute("draw:shadow-opacity", ""));
					currStyle.strokeOpacity = AttributeValue(spe.attribute("svg:stroke-opacity", ""));
					currStyle.LineW = AttributeValue(spe.attribute("svg:stroke-width", ""));
					currStyle.fillOpacity = AttributeValue(spe.attribute("draw:opacity", ""));
					currStyle.gradientName = AttributeValue(spe.attribute("draw:fill-gradient-name", ""));
					currStyle.dashName = AttributeValue(spe.attribute("draw:stroke-dash", ""));
					currStyle.startMarkerName = AttributeValue(spe.attribute("draw:marker-start", ""));
					currStyle.startMarkerWidth = AttributeValue(spe.attribute("draw:marker-start-width", ""));
					currStyle.startMarkerCentered = AttributeValue(spe.attribute("draw:marker-start-center", ""));
					currStyle.endMarkerName = AttributeValue(spe.attribute("draw:marker-end", ""));
					currStyle.endMarkerWidth = AttributeValue(spe.attribute("draw:marker-end-width", ""));
					currStyle.endMarkerCentered = AttributeValue(spe.attribute("draw:marker-end-center", ""));
					currStyle.measureDist = AttributeValue(spe.attribute("draw:line-distance"));
					currStyle.patternName = AttributeValue(spe.attribute("draw:fill-image-name", ""));
					currStyle.patternWidth = AttributeValue(spe.attribute("draw:fill-image-width", ""));
					currStyle.patternHeight = AttributeValue(spe.attribute("draw:fill-image-height", ""));
					currStyle.patternX = AttributeValue(spe.attribute("draw:fill-image-ref-point-x", ""));
					currStyle.patternY = AttributeValue(spe.attribute("draw:fill-image-ref-point-y", ""));
					currStyle.patternStretch = AttributeValue(spe.attribute("style:repeat", ""));
					currStyle.hatchName = AttributeValue(spe.attribute("draw:fill-hatch-name", ""));
					currStyle.hatchSolidFill = AttributeValue(spe.attribute("draw:fill-hatch-solid", ""));
					currStyle.opacityName = AttributeValue(spe.attribute("draw:opacity-name", ""));
					currStyle.verticalAlignment = AttributeValue(spe.attribute("draw:textarea-vertical-align", ""));
				}
				else if (spe.tagName() == "style:paragraph-properties")
				{
					currStyle.margin_top = AttributeValue(spe.attribute("fo:margin-top", ""));
					currStyle.margin_bottom = AttributeValue(spe.attribute("fo:margin-bottom", ""));
					currStyle.margin_left = AttributeValue(spe.attribute("fo:margin-left", ""));
					currStyle.margin_right = AttributeValue(spe.attribute("fo:margin-right", ""));
					currStyle.textIndent = AttributeValue(spe.attribute("fo:text-indent", ""));
					currStyle.textAlign = AttributeValue(spe.attribute("fo:text-align", ""));
					currStyle.lineHeight = AttributeValue(spe.attribute("fo:line-height", ""));
				}
				else if (spe.tagName() == "style:text-properties")
				{
					currStyle.fontName = AttributeValue(spe.attribute("style:font-name", ""));
					if (!currStyle.fontName.valid)
						currStyle.fontName = AttributeValue(spe.attribute("fo:font-family", ""));
					currStyle.fontSize = AttributeValue(spe.attribute("fo:font-size", ""));
					currStyle.fontColor = AttributeValue(spe.attribute("fo:color", ""));
					currStyle.textPos = AttributeValue(spe.attribute("style:text-position", ""));
					currStyle.textOutline = AttributeValue(spe.attribute("style:text-outline", ""));
					currStyle.textUnderline = AttributeValue(spe.attribute("style:text-underline-style", ""));
					currStyle.textUnderlineWords = AttributeValue(spe.attribute("style:text-underline-mode", ""));
					currStyle.textUnderlineColor = AttributeValue(spe.attribute("style:text-underline-color", ""));
					currStyle.textStrikeThrough = AttributeValue(spe.attribute("style:text-line-through-style", ""));
					currStyle.textShadow = AttributeValue(spe.attribute("fo:text-shadow", ""));
				}
				else if (spe.tagName() == "style:drawing-page-properties")
				{
					currStyle.fillMode = AttributeValue(spe.attribute("draw:fill", ""));
					currStyle.CurrColorFill = AttributeValue(spe.attribute("draw:fill-color", ""));
					currStyle.patternName = AttributeValue(spe.attribute("draw:fill-image-name", ""));
					currStyle.gradientName = AttributeValue(spe.attribute("draw:fill-gradient-name", ""));
					currStyle.hatchName = AttributeValue(spe.attribute("draw:fill-hatch-name", ""));
					currStyle.hatchSolidFill = AttributeValue(spe.attribute("draw:fill-hatch-solid", ""));
					currStyle.opacityName = AttributeValue(spe.attribute("draw:opacity-name", ""));
				}
			}
			currStyle.parentStyle = AttributeValue(spd.attribute("style:parent-style-name", ""));
			m_Styles.insert(spd.attribute("style:name"), currStyle);
		}
		else if (spd.tagName() == "style:page-layout")
		{
			DrawStyle currStyle;
			for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
			{
				if (spe.tagName() == "style:page-layout-properties")
				{
					currStyle.margin_top = AttributeValue(spe.attribute("fo:margin-top", ""));
					currStyle.margin_bottom = AttributeValue(spe.attribute("fo:margin-bottom", ""));
					currStyle.margin_left = AttributeValue(spe.attribute("fo:margin-left", ""));
					currStyle.margin_right = AttributeValue(spe.attribute("fo:margin-right", ""));
					currStyle.page_width = AttributeValue(spe.attribute("fo:page-width", ""));
					currStyle.page_height = AttributeValue(spe.attribute("fo:page-height", ""));
				}
			}
			currStyle.parentStyle = AttributeValue(spd.attribute("style:parent-style-name", ""));
			m_Styles.insert(spd.attribute("style:name"), currStyle);
		}
	}
}

QString OdgPlug::getStyleName(QDomElement &e)
{
	QString styleName = "standard";
	if (e.hasAttribute("draw:style-name"))
		styleName = e.attribute("draw:style-name");
	else if (e.hasAttribute("presentation:style-name"))
		styleName = e.attribute("presentation:style-name");
	return styleName;
}

void OdgPlug::resovleStyle(ObjStyle &tmpOStyle, QString pAttrs)
{
	if (m_Styles.contains(pAttrs))
	{
		DrawStyle actStyle;
		DrawStyle currStyle = m_Styles[pAttrs];
		QStringList parents;
		while (currStyle.parentStyle.valid)
		{
			if (m_Styles.contains(currStyle.parentStyle.value))
			{
				parents.prepend(currStyle.parentStyle.value);
				currStyle = m_Styles[currStyle.parentStyle.value];
			}
			else
				break;
		}
		parents.append(pAttrs);
		if (!parents.isEmpty())
		{
			for (int p = 0; p < parents.count(); p++)
			{
				currStyle = m_Styles[parents[p]];
				if (currStyle.markerViewBox.valid)
					actStyle.markerViewBox = AttributeValue(currStyle.markerViewBox.value);
				if (currStyle.markerPath.valid)
					actStyle.markerPath = AttributeValue(currStyle.markerPath.value);
				if (currStyle.startMarkerName.valid)
					actStyle.startMarkerName = AttributeValue(currStyle.startMarkerName.value);
				if (currStyle.startMarkerWidth.valid)
					actStyle.startMarkerWidth = AttributeValue(currStyle.startMarkerWidth.value);
				if (currStyle.startMarkerCentered.valid)
					actStyle.startMarkerCentered = AttributeValue(currStyle.startMarkerCentered.value);
				if (currStyle.endMarkerName.valid)
					actStyle.endMarkerName = AttributeValue(currStyle.endMarkerName.value);
				if (currStyle.endMarkerWidth.valid)
					actStyle.endMarkerWidth = AttributeValue(currStyle.endMarkerWidth.value);
				if (currStyle.endMarkerCentered.valid)
					actStyle.endMarkerCentered = AttributeValue(currStyle.endMarkerCentered.value);
				if (currStyle.stroke_dash_distance.valid)
					actStyle.stroke_dash_distance = AttributeValue(currStyle.stroke_dash_distance.value);
				if (currStyle.stroke_dash_dots1.valid)
					actStyle.stroke_dash_dots1 = AttributeValue(currStyle.stroke_dash_dots1.value);
				if (currStyle.stroke_dash_dots1_length.valid)
					actStyle.stroke_dash_dots1_length = AttributeValue(currStyle.stroke_dash_dots1_length.value);
				if (currStyle.stroke_dash_dots2.valid)
					actStyle.stroke_dash_dots2 = AttributeValue(currStyle.stroke_dash_dots2.value);
				if (currStyle.stroke_dash_dots2_length.valid)
					actStyle.stroke_dash_dots2_length = AttributeValue(currStyle.stroke_dash_dots2_length.value);
				if (currStyle.stroke_dash_style.valid)
					actStyle.stroke_dash_style = AttributeValue(currStyle.stroke_dash_style.value);
				if (currStyle.fillMode.valid)
					actStyle.fillMode = AttributeValue(currStyle.fillMode.value);
				if (currStyle.CurrColorFill.valid)
					actStyle.CurrColorFill = AttributeValue(currStyle.CurrColorFill.value);
				if (currStyle.strokeMode.valid)
					actStyle.strokeMode = AttributeValue(currStyle.strokeMode.value);
				if (currStyle.CurrColorStroke.valid)
					actStyle.CurrColorStroke = AttributeValue(currStyle.CurrColorStroke.value);
				if (currStyle.CurrColorShadow.valid)
					actStyle.CurrColorShadow = AttributeValue(currStyle.CurrColorShadow.value);
				if (currStyle.hasShadow.valid)
					actStyle.hasShadow = AttributeValue(currStyle.hasShadow.value);
				if (currStyle.shadowX.valid)
					actStyle.shadowX = AttributeValue(currStyle.shadowX.value);
				if (currStyle.shadowY.valid)
					actStyle.shadowY = AttributeValue(currStyle.shadowY.value);
				if (currStyle.shadowTrans.valid)
					actStyle.shadowTrans = AttributeValue(currStyle.shadowTrans.value);
				if (currStyle.fillOpacity.valid)
					actStyle.fillOpacity = AttributeValue(currStyle.fillOpacity.value);
				if (currStyle.strokeOpacity.valid)
					actStyle.strokeOpacity = AttributeValue(currStyle.strokeOpacity.value);
				if (currStyle.LineW.valid)
					actStyle.LineW = AttributeValue(currStyle.LineW.value);
				if (currStyle.fontName.valid)
					actStyle.fontName = AttributeValue(currStyle.fontName.value);
				if (currStyle.fontSize.valid)
					actStyle.fontSize = AttributeValue(currStyle.fontSize.value);
				if (currStyle.margin_top.valid)
					actStyle.margin_top = AttributeValue(currStyle.margin_top.value);
				if (currStyle.margin_bottom.valid)
					actStyle.margin_bottom = AttributeValue(currStyle.margin_bottom.value);
				if (currStyle.margin_left.valid)
					actStyle.margin_left = AttributeValue(currStyle.margin_left.value);
				if (currStyle.margin_right.valid)
					actStyle.margin_right = AttributeValue(currStyle.margin_right.value);
				if (currStyle.page_width.valid)
					actStyle.page_width = AttributeValue(currStyle.page_width.value);
				if (currStyle.page_height.valid)
					actStyle.page_height = AttributeValue(currStyle.page_height.value);
				if (currStyle.page_layout_name.valid)
					actStyle.page_layout_name = AttributeValue(currStyle.page_layout_name.value);
				if (currStyle.textIndent.valid)
					actStyle.textIndent = AttributeValue(currStyle.textIndent.value);
				if (currStyle.textAlign.valid)
					actStyle.textAlign = AttributeValue(currStyle.textAlign.value);
				if (currStyle.textPos.valid)
					actStyle.textPos = AttributeValue(currStyle.textPos.value);
				if (currStyle.textOutline.valid)
					actStyle.textOutline = AttributeValue(currStyle.textOutline.value);
				if (currStyle.textUnderline.valid)
					actStyle.textUnderline = AttributeValue(currStyle.textUnderline.value);
				if (currStyle.textUnderlineWords.valid)
					actStyle.textUnderlineWords = AttributeValue(currStyle.textUnderlineWords.value);
				if (currStyle.textUnderlineColor.valid)
					actStyle.textUnderlineColor = AttributeValue(currStyle.textUnderlineColor.value);
				if (currStyle.textStrikeThrough.valid)
					actStyle.textStrikeThrough = AttributeValue(currStyle.textStrikeThrough.value);
				if (currStyle.textShadow.valid)
					actStyle.textShadow = AttributeValue(currStyle.textShadow.value);
				if (currStyle.lineHeight.valid)
					actStyle.lineHeight = AttributeValue(currStyle.lineHeight.value);
				if (currStyle.fontColor.valid)
					actStyle.fontColor = AttributeValue(currStyle.fontColor.value);
				if (currStyle.gradientAngle.valid)
					actStyle.gradientAngle = AttributeValue(currStyle.gradientAngle.value);
				if (currStyle.gradientBorder.valid)
					actStyle.gradientBorder = AttributeValue(currStyle.gradientBorder.value);
				if (currStyle.gradientEndColor.valid)
					actStyle.gradientEndColor = AttributeValue(currStyle.gradientEndColor.value);
				if (currStyle.gradientEndShade.valid)
					actStyle.gradientEndShade = AttributeValue(currStyle.gradientEndShade.value);
				if (currStyle.gradientStartColor.valid)
					actStyle.gradientStartColor = AttributeValue(currStyle.gradientStartColor.value);
				if (currStyle.gradientStartShade.valid)
					actStyle.gradientStartShade = AttributeValue(currStyle.gradientStartShade.value);
				if (currStyle.gradientCenterX.valid)
					actStyle.gradientCenterX = AttributeValue(currStyle.gradientCenterX.value);
				if (currStyle.gradientCenterY.valid)
					actStyle.gradientCenterY = AttributeValue(currStyle.gradientCenterY.value);
				if (currStyle.gradientType.valid)
					actStyle.gradientType = AttributeValue(currStyle.gradientType.value);
				if (currStyle.gradientName.valid)
					actStyle.gradientName = AttributeValue(currStyle.gradientName.value);
				if (currStyle.dashName.valid)
					actStyle.dashName = AttributeValue(currStyle.dashName.value);
				if (currStyle.measureDist.valid)
					actStyle.measureDist = AttributeValue(currStyle.measureDist.value);
				if (currStyle.patternName.valid)
					actStyle.patternName = AttributeValue(currStyle.patternName.value);
				if (currStyle.patternPath.valid)
					actStyle.patternPath = AttributeValue(currStyle.patternPath.value);
				if (currStyle.patternData.valid)
					actStyle.patternData = AttributeValue(currStyle.patternData.value);
				if (currStyle.patternWidth.valid)
					actStyle.patternWidth = AttributeValue(currStyle.patternWidth.value);
				if (currStyle.patternHeight.valid)
					actStyle.patternHeight = AttributeValue(currStyle.patternHeight.value);
				if (currStyle.patternX.valid)
					actStyle.patternX = AttributeValue(currStyle.patternX.value);
				if (currStyle.patternY.valid)
					actStyle.patternY = AttributeValue(currStyle.patternY.value);
				if (currStyle.patternStretch.valid)
					actStyle.patternStretch = AttributeValue(currStyle.patternStretch.value);
				if (currStyle.hatchName.valid)
					actStyle.hatchName = AttributeValue(currStyle.hatchName.value);
				if (currStyle.hatchColor.valid)
					actStyle.hatchColor = AttributeValue(currStyle.hatchColor.value);
				if (currStyle.hatchDistance.valid)
					actStyle.hatchDistance = AttributeValue(currStyle.hatchDistance.value);
				if (currStyle.hatchRotation.valid)
					actStyle.hatchRotation = AttributeValue(currStyle.hatchRotation.value);
				if (currStyle.hatchStyle.valid)
					actStyle.hatchStyle = AttributeValue(currStyle.hatchStyle.value);
				if (currStyle.hatchSolidFill.valid)
					actStyle.hatchSolidFill = AttributeValue(currStyle.hatchSolidFill.value);
				if (currStyle.opacityName.valid)
					actStyle.opacityName = AttributeValue(currStyle.opacityName.value);
				if (currStyle.opacityEnd.valid)
					actStyle.opacityEnd = AttributeValue(currStyle.opacityEnd.value);
				if (currStyle.opacityStart.valid)
					actStyle.opacityStart = AttributeValue(currStyle.opacityStart.value);
				if (currStyle.verticalAlignment.valid)
					actStyle.verticalAlignment = AttributeValue(currStyle.verticalAlignment.value);
			}
		}
		tmpOStyle.stroke_dash_distance = -1;
		tmpOStyle.stroke_dash_dots1_length = -1;
		tmpOStyle.stroke_dash_dots2_length = -1;
		tmpOStyle.stroke_dash_dots1 = -1;
		tmpOStyle.stroke_dash_dots2 = -1;
		if (actStyle.stroke_dash_style.valid)
			tmpOStyle.stroke_dash_style = actStyle.stroke_dash_style.value;
		if (actStyle.stroke_dash_distance.valid)
			tmpOStyle.stroke_dash_distance = parseUnit(actStyle.stroke_dash_distance.value);
		if (actStyle.stroke_dash_dots1.valid)
			tmpOStyle.stroke_dash_dots1 = actStyle.stroke_dash_dots1.value.toInt();
		if (actStyle.stroke_dash_dots1_length.valid)
			tmpOStyle.stroke_dash_dots1_length = parseUnit(actStyle.stroke_dash_dots1_length.value);
		if (actStyle.stroke_dash_dots2.valid)
			tmpOStyle.stroke_dash_dots2 = actStyle.stroke_dash_dots2.value.toInt();
		if (actStyle.stroke_dash_dots2_length.valid)
			tmpOStyle.stroke_dash_dots2_length = parseUnit(actStyle.stroke_dash_dots2_length.value);

		if (actStyle.CurrColorFill.valid)
		{
			if (actStyle.fillMode.valid && (actStyle.fillMode.value != "none"))
				tmpOStyle.CurrColorFill = parseColor(actStyle.CurrColorFill.value);
			else
				tmpOStyle.CurrColorFill = CommonStrings::None;
		}
		else
			tmpOStyle.CurrColorFill = CommonStrings::None;
		if (actStyle.fillMode.valid)
		{
			if (actStyle.fillMode.value == "none")
				tmpOStyle.fill_type = 0;
			else if (actStyle.fillMode.value == "solid")
				tmpOStyle.fill_type = 1;
			else if (actStyle.fillMode.value == "gradient")
			{
				tmpOStyle.fill_type = 2;
				if (actStyle.gradientName.valid)
					tmpOStyle.gradientName = actStyle.gradientName.value;
			}
			else if (actStyle.fillMode.value == "bitmap")
			{
				tmpOStyle.fill_type = 3;
				if (actStyle.patternName.valid)
					tmpOStyle.patternName = actStyle.patternName.value;
			}
			else if (actStyle.fillMode.value == "hatch")
			{
				tmpOStyle.fill_type = 4;
				if (actStyle.hatchName.valid)
					tmpOStyle.hatchName = actStyle.hatchName.value;
			}
		}
		if (actStyle.CurrColorStroke.valid)
		{
			if (actStyle.strokeMode.valid && (actStyle.strokeMode.value != "none"))
				tmpOStyle.CurrColorStroke = parseColor(actStyle.CurrColorStroke.value);
			else
				tmpOStyle.CurrColorStroke = CommonStrings::None;
		}
		else
			tmpOStyle.CurrColorStroke = CommonStrings::None;
		if (actStyle.strokeMode.valid)
		{
			if (actStyle.strokeMode.value == "none")
				tmpOStyle.stroke_type = 0;
			else if (actStyle.strokeMode.value == "solid")
				tmpOStyle.stroke_type = 1;
			else if (actStyle.strokeMode.value == "dash")
			{
				tmpOStyle.stroke_type = 2;
				if (actStyle.dashName.valid)
					tmpOStyle.dashName = actStyle.dashName.value;
			}
		}
		if (actStyle.CurrColorShadow.valid)
			tmpOStyle.CurrColorShadow = parseColor(actStyle.CurrColorShadow.value);
		if (actStyle.hasShadow.valid)
			tmpOStyle.hasShadow = actStyle.hasShadow.value == "visible";
		if (actStyle.shadowX.valid)
			tmpOStyle.shadowX = parseUnit(actStyle.shadowX.value);
		if (actStyle.shadowY.valid)
			tmpOStyle.shadowY = parseUnit(actStyle.shadowY.value);
		if (actStyle.shadowTrans.valid)
		{
			double transVal = parseUnit(actStyle.shadowTrans.value);
			if (transVal > 1.0)
				transVal /= 100.0;
			tmpOStyle.shadowTrans = 1.0 - transVal;
		}
		if (actStyle.fillOpacity.valid)
		{
			double transVal = parseUnit(actStyle.fillOpacity.value);
			if (transVal > 1.0)
				transVal /= 100.0;
			tmpOStyle.fillOpacity = 1.0 - transVal;
		}
		if (actStyle.strokeOpacity.valid)
		{
			double transVal = parseUnit(actStyle.strokeOpacity.value);
			if (transVal > 1.0)
				transVal /= 100.0;
			tmpOStyle.strokeOpacity = 1.0 - transVal;
		}
		if (actStyle.LineW.valid)
			tmpOStyle.LineW = parseUnit(actStyle.LineW.value);
		if (actStyle.fontName.valid)
		{
			if (m_fontMap.contains(actStyle.fontName.value))
				tmpOStyle.fontName = m_fontMap[actStyle.fontName.value];
			else
				tmpOStyle.fontName = actStyle.fontName.value;
			if (!PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.contains(tmpOStyle.fontName))
			{
				tmpOStyle.fontName = constructFontName(tmpOStyle.fontName, "");
				m_fontMap[actStyle.fontName.value] = tmpOStyle.fontName;
			}
		}
		if (actStyle.fontSize.valid)
			tmpOStyle.fontSize = parseUnit(actStyle.fontSize.value);
		if (actStyle.fontColor.valid)
			tmpOStyle.CurrColorText = parseColor(actStyle.fontColor.value);
		if (actStyle.margin_top.valid)
			tmpOStyle.margin_top = parseUnit(actStyle.margin_top.value);
		if (actStyle.margin_bottom.valid)
			tmpOStyle.margin_bottom = parseUnit(actStyle.margin_bottom.value);
		if (actStyle.margin_left.valid)
			tmpOStyle.margin_left = parseUnit(actStyle.margin_left.value);
		if (actStyle.margin_right.valid)
			tmpOStyle.margin_right = parseUnit(actStyle.margin_right.value);
		if (actStyle.page_layout_name.valid)
		{
			tmpOStyle.page_layout_name = actStyle.page_layout_name.value;
			if (m_Styles.contains(tmpOStyle.page_layout_name))
			{
				actStyle = m_Styles[tmpOStyle.page_layout_name];
				if (actStyle.page_height.valid)
					tmpOStyle.page_height = parseUnit(actStyle.page_height.value);
				if (actStyle.page_width.valid)
					tmpOStyle.page_width = parseUnit(actStyle.page_width.value);
				if (actStyle.margin_top.valid)
					tmpOStyle.margin_top = parseUnit(actStyle.margin_top.value);
				if (actStyle.margin_bottom.valid)
					tmpOStyle.margin_bottom = parseUnit(actStyle.margin_bottom.value);
				if (actStyle.margin_left.valid)
					tmpOStyle.margin_left = parseUnit(actStyle.margin_left.value);
				if (actStyle.margin_right.valid)
					tmpOStyle.margin_right = parseUnit(actStyle.margin_right.value);
			}
		}
		if (actStyle.textIndent.valid)
			tmpOStyle.textIndent = parseUnit(actStyle.textIndent.value);
		if (actStyle.textAlign.valid)
		{
			QString attValue = actStyle.textAlign.value;
			if (attValue == "left")
				tmpOStyle.textAlign = ParagraphStyle::Leftaligned;
			else if (attValue == "center")
				tmpOStyle.textAlign = ParagraphStyle::Centered;
			else if (attValue == "right")
				tmpOStyle.textAlign = ParagraphStyle::Rightaligned;
			else if (attValue == "justify")
				tmpOStyle.textAlign = ParagraphStyle::Justified;
		}
		if (actStyle.verticalAlignment.valid)
		{
			if (actStyle.verticalAlignment.value == "middle")
				tmpOStyle.verticalAlignment = 1;
			else if (actStyle.verticalAlignment.value == "bottom")
				tmpOStyle.verticalAlignment = 2;
		}
		if (actStyle.textPos.valid)
			tmpOStyle.textPos = actStyle.textPos.value;
		if (actStyle.textOutline.valid)
			tmpOStyle.textOutline = actStyle.textOutline.value;
		if (actStyle.textUnderline.valid)
			tmpOStyle.textUnderline = actStyle.textUnderline.value != "none";
		if (actStyle.textUnderlineWords.valid)
			tmpOStyle.textUnderlineWords = actStyle.textUnderlineWords.value != "continuous";
		if (actStyle.textUnderlineColor.valid)
		{
			if (actStyle.textUnderlineColor.value == "font-color")
				tmpOStyle.textUnderlineColor = tmpOStyle.CurrColorText;
			else
				tmpOStyle.textUnderlineColor = parseColor(actStyle.textUnderlineColor.value);
		}
		if (actStyle.textStrikeThrough.valid)
			tmpOStyle.textStrikeThrough = actStyle.textStrikeThrough.value != "none";
		if (actStyle.textShadow.valid)
			tmpOStyle.textShadow = actStyle.textShadow.value != "none";
		if (actStyle.lineHeight.valid)
		{
			if (actStyle.lineHeight.value == "normal")
				tmpOStyle.lineHeight = -1.0;
			else if (actStyle.lineHeight.value.right(1) != "%")
			{
				tmpOStyle.lineHeight = parseUnit(actStyle.lineHeight.value);
				tmpOStyle.absLineHeight = true;
			}
			else
			{
				tmpOStyle.lineHeight = parseUnit(actStyle.lineHeight.value);
				tmpOStyle.absLineHeight = false;
			}
		}
		if (actStyle.gradientAngle.valid)
			tmpOStyle.gradientAngle = actStyle.gradientAngle.value.toDouble() / 10.0;
		if (actStyle.gradientBorder.valid)
			tmpOStyle.gradientBorder = parseUnit(actStyle.gradientBorder.value);
		if (actStyle.gradientEndColor.valid)
			tmpOStyle.gradientEndColor = parseColor(actStyle.gradientEndColor.value);
		if (actStyle.gradientEndShade.valid)
			tmpOStyle.gradientEndShade = parseUnit(actStyle.gradientEndShade.value) * 100.0;
		if (actStyle.gradientStartColor.valid)
			tmpOStyle.gradientStartColor = parseColor(actStyle.gradientStartColor.value);
		if (actStyle.gradientStartShade.valid)
			tmpOStyle.gradientStartShade = parseUnit(actStyle.gradientStartShade.value) * 100.0;
		if (actStyle.gradientCenterX.valid)
			tmpOStyle.gradientCenterX = parseUnit(actStyle.gradientCenterX.value);
		if (actStyle.gradientCenterY.valid)
			tmpOStyle.gradientCenterY = parseUnit(actStyle.gradientCenterY.value);
		if (actStyle.gradientType.valid)
			tmpOStyle.gradientType = actStyle.gradientType.value;
		if (actStyle.markerViewBox.valid)
		{
			QString viewbox = actStyle.markerViewBox.value;
			QStringList points = viewbox.replace( QRegExp(","), " ").simplified().split( ' ', QString::SkipEmptyParts );
			tmpOStyle.markerViewBox = QRectF(ScCLocale::toDoubleC(points[0]), ScCLocale::toDoubleC(points[1]), ScCLocale::toDoubleC(points[2]), ScCLocale::toDoubleC(points[3]));
		}
		else
			tmpOStyle.markerViewBox = QRectF();
		if (actStyle.markerPath.valid)
		{
			FPointArray mPath;
			mPath.svgInit();
			mPath.parseSVG(actStyle.markerPath.value);
			if (tmpOStyle.markerViewBox != QRect())
			{
				QRectF clipRect = mPath.toQPainterPath(false).boundingRect();
				QTransform mat;
				double vw = tmpOStyle.markerViewBox.width();
				double vh = tmpOStyle.markerViewBox.height();
				double sx = (vw != 0.0) ? (clipRect.width() / vw) : 1;
				double sy = (vh != 0.0) ? (clipRect.height() / vh) : 1;
				mat.scale(sx, sy);
				mPath.map(mat);
			}
			tmpOStyle.markerPath = mPath.toQPainterPath(true);
		}
		if (actStyle.startMarkerName.valid)
			tmpOStyle.startMarkerName = actStyle.startMarkerName.value;
		if (actStyle.startMarkerWidth.valid)
			tmpOStyle.startMarkerWidth = parseUnit(actStyle.startMarkerWidth.value);
		if (actStyle.startMarkerCentered.valid)
			tmpOStyle.startMarkerCentered = actStyle.startMarkerCentered.value == "true";
		if (actStyle.endMarkerName.valid)
			tmpOStyle.endMarkerName = actStyle.endMarkerName.value;
		if (actStyle.endMarkerWidth.valid)
			tmpOStyle.endMarkerWidth = parseUnit(actStyle.endMarkerWidth.value);
		if (actStyle.endMarkerCentered.valid)
			tmpOStyle.endMarkerCentered = actStyle.endMarkerCentered.value == "true";
		if (actStyle.measureDist.valid)
			tmpOStyle.measureDist = parseUnit(actStyle.measureDist.value);
		if (actStyle.patternPath.valid)
			tmpOStyle.patternPath = actStyle.patternPath.value;
		if (actStyle.patternData.valid)
			tmpOStyle.patternData = actStyle.patternData.value.toLatin1();
		if (actStyle.patternWidth.valid)
		{
			tmpOStyle.patternWidth = parseUnit(actStyle.patternWidth.value);
			if (actStyle.patternWidth.value.contains("%"))
				tmpOStyle.patternDim_W_in_Percent = true;
		}
		else
			tmpOStyle.patternWidth = -1;
		if (actStyle.patternHeight.valid)
		{
			tmpOStyle.patternHeight = parseUnit(actStyle.patternHeight.value);
			if (actStyle.patternHeight.value.contains("%"))
				tmpOStyle.patternDim_H_in_Percent = true;
		}
		else
			tmpOStyle.patternHeight = -1;
		if (actStyle.patternX.valid)
			tmpOStyle.patternX = parseUnit(actStyle.patternX.value);
		else
			tmpOStyle.patternX = -1;
		if (actStyle.patternY.valid)
			tmpOStyle.patternY = parseUnit(actStyle.patternY.value);
		else
			tmpOStyle.patternY = -1;
		if (actStyle.patternStretch.valid)
			tmpOStyle.patternStretch = actStyle.patternStretch.value;
		if (actStyle.hatchColor.valid)
			tmpOStyle.hatchColor = parseColor(actStyle.hatchColor.value);
		if (actStyle.hatchDistance.valid)
			tmpOStyle.hatchDistance = parseUnit(actStyle.hatchDistance.value);
		if (actStyle.hatchRotation.valid)
			tmpOStyle.hatchRotation = actStyle.hatchRotation.value.toDouble() / 10.0;
		if (actStyle.hatchStyle.valid)
			tmpOStyle.hatchStyle = actStyle.hatchStyle.value;
		if (actStyle.hatchSolidFill.valid)
			tmpOStyle.hatchSolidFill = actStyle.hatchSolidFill.value == "true";
		if (actStyle.opacityName.valid)
			tmpOStyle.opacityName = actStyle.opacityName.value;
		if (actStyle.opacityEnd.valid)
			tmpOStyle.opacityEnd = parseUnit(actStyle.opacityEnd.value) * 100.0;
		if (actStyle.opacityStart.valid)
			tmpOStyle.opacityStart = parseUnit(actStyle.opacityStart.value) * 100.0;
	}
}

double OdgPlug::parseUnit(const QString &unit)
{
	QString unitval=unit;
	if (unit.isEmpty())
		return 0.0;
	if( unit.right( 2 ) == "pt" )
		unitval.replace( "pt", "" );
	else if( unit.right( 2 ) == "cm" )
		unitval.replace( "cm", "" );
	else if( unit.right( 2 ) == "mm" )
		unitval.replace( "mm" , "" );
	else if( unit.right( 2 ) == "in" )
		unitval.replace( "in", "" );
	else if( unit.right( 2 ) == "px" )
		unitval.replace( "px", "" );
	else if( unit.right( 1 ) == "%" )
		unitval.replace( "%", "" );
	double value = ScCLocale::toDoubleC(unitval);
	if( unit.right( 2 ) == "pt" )
		{}/* value = value; */ //no change
	else if( unit.right( 2 ) == "cm" )
		value = ( value / 2.54 ) * 72;
	else if( unit.right( 2 ) == "mm" )
		value = ( value / 25.4 ) * 72;
	else if( unit.right( 2 ) == "in" )
		value = value * 72;
	else if( unit.right( 2 ) == "px" )
		{}/* value = value; */ //no change
	else if( unit.right( 1 ) == "%" )
		value = value / 100.0;
	return value;
}

const char * OdgPlug::getCoord( const char *ptr, double &number )
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

bool OdgPlug::parseEnhPath(const QString& svgPath, FPointArray &result, bool &fill, bool &stroke)
{
	QString d = svgPath;
	d = d.replace( QRegExp( "," ), " ");
	bool ret = false;
	fill = true;
	stroke = true;
	if( !d.isEmpty() )
	{
		bool xDir = true;
		bool yDir = false;
		double rad2deg = 180.0/M_PI;
		QPainterPath pPath;
		d = d.simplified();
		QByteArray pathData = d.toLatin1();
		const char *ptr = pathData.constData();
		const char *end = pathData.constData() + pathData.length() + 1;
		double tox, toy, x1, y1, x2, y2;
		double px1, py1, px2, py2, px3, py3;
		int moveCount = 0;
		result.svgInit();
		char command = *(ptr++), lastCommand = ' ';
		while( ptr < end )
		{
			if( *ptr == ' ' )
				ptr++;
			switch( command )
			{
			case 'A':
			case 'B':
			case 'V':
			case 'W':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					ptr = getCoord( ptr, px1 );
					ptr = getCoord( ptr, py1 );
					ptr = getCoord( ptr, px2 );
					ptr = getCoord( ptr, py2 );
					ptr = getCoord( ptr, px3 );
					ptr = getCoord( ptr, py3 );
					bool lineTo = ((command == 'A') || (command == 'W'));
					bool clockwise = ((command == 'W') || (command == 'V'));
					QRectF bbox = QRectF(QPointF(tox, toy), QPointF(px1, py1)).normalized();
					QPointF center = bbox.center();
					double rx = 0.5 * bbox.width();
					double ry = 0.5 * bbox.height();
					if (rx == 0)
						rx = 1;
					if (ry == 0)
						ry = 1;
					QPointF startRadialVector = QPointF(px2, py2) - center;
					QPointF endRadialVector = QPointF(px3, py3) - center;
					// convert from ellipse space to unit-circle space
					double x0 = startRadialVector.x() / rx;
					double y0 = startRadialVector.y() / ry;

					double x1 = endRadialVector.x() / rx;
					double y1 = endRadialVector.y() / ry;

					double startAngle = angleFromPoint(QPointF(x0,y0));
					double stopAngle = angleFromPoint(QPointF(x1,y1));

					// we are moving counter-clockwise to the end angle
					double sweepAngle = radSweepAngle(startAngle, stopAngle, clockwise);
					// compute the starting point to draw the line to
					// as the point x3 y3 is not on the ellipse, spec says the point define radial vector
					QPointF startPoint(rx * cos(startAngle), ry * sin(2*M_PI - startAngle));

					// if A or W is first command in enhanced path
					// move to the starting point
					bool isFirstCommandInPath = (pPath.elementCount() == 0);
					bool isFirstCommandInSubpath = lastCommand == 'Z';
					if (lineTo && !isFirstCommandInPath && !isFirstCommandInSubpath)
						pPath.lineTo(center + startPoint);
					else
						pPath.moveTo(center + startPoint);
					arcTo(pPath, pPath.currentPosition(), rx, ry, startAngle * rad2deg, sweepAngle * rad2deg);
					break;
				}
			case 'C':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					pPath.cubicTo(x1, y1, x2, y2, tox, toy);
					break;
				}
			case 'F':
				{
					fill = false;
					break;
				}
			case 'L':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					pPath.lineTo(tox, toy);
					break;
				}
			case 'M':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					pPath.moveTo(tox, toy);
					moveCount++;
					break;
				}
			case 'Q':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					pPath.quadTo(x1, y1, tox, toy);
					break;
				}
			case 'S':
				{
					stroke = false;
					break;
				}
			case 'T':
			case 'U':
				{
					ptr = getCoord(ptr, px1);
					ptr = getCoord(ptr, py1);
					ptr = getCoord(ptr, px2);
					ptr = getCoord(ptr, py2);
					ptr = getCoord(ptr, tox);
					ptr = getCoord(ptr, toy);
					bool lineTo = (command == 'T');
					const QPointF &radii = QPointF(px2, py2);
					const QPointF &angles = QPointF(tox, toy) / (180.0/M_PI);
					QPointF start(radii.x() * cos(angles.x()), -1 * radii.y() * sin(angles.x()));
					double sweepAngle = degSweepAngle(tox, toy, false);
					if (lineTo)
						pPath.lineTo(QPointF(px1, py1) + start);
					else
						pPath.moveTo(QPointF(px1, py1) + start);
					arcTo(pPath, pPath.currentPosition(), radii.x(), radii.y(), tox, sweepAngle);
					break;
				}
			case 'X':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					double rx = tox - pPath.currentPosition().x();
					double ry = toy - pPath.currentPosition().y();
					double startAngle = xDir ? (ry > 0.0 ? 90.0 : 270.0) : (rx < 0.0 ? 0.0 : 180.0);
					double sweepAngle = xDir ? (rx*ry < 0.0 ? 90.0 : -90.0) : (rx*ry > 0.0 ? 90.0 : -90.0);
					arcTo(pPath, pPath.currentPosition(), fabs(rx), fabs(ry), startAngle, sweepAngle);
					xDir = !xDir;
					break;
				}
			case 'Y':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					double rx = tox - pPath.currentPosition().x();
					double ry = toy - pPath.currentPosition().y();
					double startAngle = yDir ? (ry > 0.0 ? 90.0 : 270.0) : (rx < 0.0 ? 0.0 : 180.0);
					double sweepAngle = yDir ? (rx*ry < 0.0 ? 90.0 : -90.0) : (rx*ry > 0.0 ? 90.0 : -90.0);
					arcTo(pPath, pPath.currentPosition(), fabs(rx), fabs(ry), startAngle, sweepAngle);
					yDir = !yDir;
					break;
				}
			case 'Z':
				{
					pPath.closeSubpath();
					break;
				}
			}
			lastCommand = command;
			if(*ptr == '+' || *ptr == '-' || (*ptr >= '0' && *ptr <= '9'))
			{
				// there are still coords in this command
				if(command == 'M')
					command = 'L';
			}
			else
			{
				command = *(ptr++);
				xDir = true;
				yDir = false;
			}
		}
		if ((lastCommand != 'Z') || (moveCount > 1))
			ret = true;
		result.fromQPainterPath(pPath, !ret);
	}
	return ret;
}

double OdgPlug::angleFromPoint(const QPointF &point)
{
	double angle = atan2(point.y(), point.x());
	if (angle < 0.0)
		angle += 2*M_PI;
	return 2*M_PI - angle;
}

double OdgPlug::radSweepAngle(double start, double stop, bool clockwise)
{
	double sweepAngle = stop - start;
	if (fabs(sweepAngle) < 0.1) {
		return 2*M_PI;
	}
	if (clockwise) {
		// we are moving clockwise to the end angle
		if (stop > start)
			sweepAngle = (stop - start) - 2*M_PI;
	} else {
		// we are moving counter-clockwise to the stop angle
		if (start > stop)
			sweepAngle = 2*M_PI - (start-stop);
	}

   return sweepAngle;
}

double OdgPlug::degSweepAngle(double start, double stop, bool clockwise)
{
	double sweepAngle = stop - start;
	if (fabs(sweepAngle) < 0.1) {
		return 360.0;
	}
	if (clockwise) {
		// we are moving clockwise to the end angle
		if (stop > start)
			sweepAngle = (stop - start) - 360.0;
	} else {
		// we are moving counter-clockwise to the stop angle
		if (start > stop)
			sweepAngle = 360.0 - (start-stop);
	}
   return sweepAngle;
}

void OdgPlug::arcTo(QPainterPath &path, QPointF startpoint, double rx, double ry, double startAngle, double sweepAngle)
{
	QPointF curvePoints[12];
	int pointCnt = arcToCurve(rx, ry, startAngle, sweepAngle, startpoint, curvePoints);
	for (int i = 0; i < pointCnt; i += 3)
	{
		path.cubicTo(curvePoints[i], curvePoints[i+1], curvePoints[i+2]);
	}
	return;
}

int OdgPlug::arcToCurve(double rx, double ry, double startAngle, double sweepAngle, const QPointF & offset, QPointF * curvePoints)
{
	int pointCnt = 0;

	// check Parameters
	if (sweepAngle == 0)
		return pointCnt;
	if (sweepAngle > 360)
		sweepAngle = 360;
	else if (sweepAngle < -360)
		sweepAngle = - 360;

	if (rx == 0 || ry == 0) {
		//TODO
	}

	// split angles bigger than 90° so that it gives a good aproximation to the circle
	double parts = ceil(qAbs(sweepAngle / 90.0));

	double sa_rad = startAngle * M_PI / 180.0;
	double partangle = sweepAngle / parts;
	double endangle = startAngle + partangle;
	double se_rad = endangle * M_PI / 180.0;
	double sinsa = sin(sa_rad);
	double cossa = cos(sa_rad);
	double kappa = 4.0 / 3.0 * tan((se_rad - sa_rad) / 4);

	// startpoint is at the last point is the path but when it is closed
	// it is at the first point
	QPointF startpoint(offset);

	//center berechnen
	QPointF center(startpoint - QPointF(cossa * rx, -sinsa * ry));

	//kDebug(30006) <<"kappa" << kappa <<"parts" << parts;;

	for (int part = 0; part < parts; ++part) {
		// start tangent
		curvePoints[pointCnt++] = QPointF(startpoint - QPointF(sinsa * rx * kappa, cossa * ry * kappa));

		double sinse = sin(se_rad);
		double cosse = cos(se_rad);

		// end point
		QPointF endpoint(center + QPointF(cosse * rx, -sinse * ry));
		// end tangent
		curvePoints[pointCnt++] = QPointF(endpoint - QPointF(-sinse * rx * kappa, -cosse * ry * kappa));
		curvePoints[pointCnt++] = endpoint;

		// set the endpoint as next start point
		startpoint = endpoint;
		sinsa = sinse;
		cossa = cosse;
		endangle += partangle;
		se_rad = endangle * M_PI / 180.0;
	}

	return pointCnt;
}

PageItem* OdgPlug::groupObjects(QList<PageItem *> &GElements)
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
	PageItem* retObj = m_Doc->Items->at(z);
	retObj->ClipEdited = true;
	retObj->FrameType = 3;
	retObj->setFillEvenOdd(false);
	retObj->OldB2 = retObj->width();
	retObj->OldH2 = retObj->height();
	retObj->updateClip();
	m_Doc->groupObjectsToItem(retObj, GElements);
	retObj->OwnPage = m_Doc->OnPage(retObj);
	m_Doc->GroupOnPage(retObj);
	m_Doc->Items->removeLast();
	return retObj;
}

QString OdgPlug::modifyColor(QString name, bool darker, int amount)
{
	const ScColor& col = m_Doc->PageColors[name];
	QColor c = ScColorEngine::getShadeColorProof(col, m_Doc, 100);
	QColor mo;
	if (darker)
		mo = c.darker(amount);
	else
		mo = c.lighter(amount);
	ScColor tmp;
	tmp.fromQColor(mo);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString fNam = m_Doc->PageColors.tryAddColor("FromOdg"+mo.name(), tmp);
	if (fNam == "FromOdg"+mo.name())
		importedColors.append(fNam);
	return fNam;
}

QColor OdgPlug::parseColorN( const QString &rgbColor )
{
	int r, g, b;
	keywordToRGB( rgbColor, r, g, b );
	return QColor( r, g, b );
}

QString OdgPlug::parseColor( const QString &s )
{
	QColor c;
	QString ret = CommonStrings::None;
	if ((s == "") || s.isEmpty())
		return ret;
	if( s.startsWith( "rgb(" ) )
	{
		QString parse = s.trimmed();
		QStringList colors = parse.split( ',', QString::SkipEmptyParts );
		QString r = colors[0].right( ( colors[0].length() - 4 ) );
		QString g = colors[1];
		QString b = colors[2].left( ( colors[2].length() - 1 ) );
		if( r.contains( "%" ) )
		{
			r.chop(1);
			r = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(r) ) / 100.0 ) ) );
		}
		if( g.contains( "%" ) )
		{
			g.chop(1);
			g = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(g) ) / 100.0 ) ) );
		}
		if( b.contains( "%" ) )
		{
			b.chop(1);
			b = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(b) ) / 100.0 ) ) );
		}
		c = QColor(r.toInt(), g.toInt(), b.toInt());
	}
	else
	{
		QString rgbColor = s.trimmed();
		if( rgbColor.startsWith( "#" ) )
			c.setNamedColor( rgbColor );
		else
			c = parseColorN( rgbColor );
	}
	ScColor tmp;
	tmp.fromQColor(c);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString fNam = m_Doc->PageColors.tryAddColor("FromOdg"+c.name(), tmp);
	if (fNam == "FromOdg"+c.name())
		importedColors.append(fNam);
	ret = fNam;
	return ret;
}

QString OdgPlug::constructFontName(QString fontBaseName, QString fontStyle)
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

QPointF OdgPlug::intersectBoundingRect(PageItem *item, QLineF gradientVector)
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

PageItem* OdgPlug::applyStartArrow(PageItem* ite, ObjStyle &obState)
{
	PageItem *iteS = NULL;
	if (!obState.startMarkerName.isEmpty())
	{
		ObjStyle mStyle;
		resovleStyle(mStyle, obState.startMarkerName);
		QPainterPath pa = mStyle.markerPath;
		FPointArray EndArrow;
		EndArrow.fromQPainterPath(pa);
		QRectF br = pa.boundingRect();
		double EndArrowWidth = obState.startMarkerWidth;
		if (EndArrowWidth > 0)
		{
			FPoint Start = ite->PoLine.point(0);
			for (int xx = 1; xx < ite->PoLine.size(); xx += 2)
			{
				FPoint Vector = ite->PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					QPointF refP;
					if (obState.startMarkerCentered)
						refP = QPointF(br.width() / 2.0, br.height() / 2.0);
					else
						refP = QPointF(br.width() / 2.0, 0);
					QTransform m;
					m.translate(br.width() / 2.0, br.height() / 2.0);
					m.rotate(r + 90);
					m.translate(-br.width() / 2.0, -br.height() / 2.0);
					m.scale(EndArrowWidth / br.width(), EndArrowWidth / br.width());
					EndArrow.map(m);
					refP = m.map(refP);
					QPainterPath pa2 = EndArrow.toQPainterPath(true);
					QTransform m2;
					FPoint grOffset2(getMinClipF(&EndArrow));
					m2.translate(-grOffset2.x(), -grOffset2.y());
					EndArrow.map(m2);
					refP = m2.map(refP);
					EndArrow.translate(-refP.x(), -refP.y());
					QTransform arrowTrans;
					arrowTrans.translate(-m_Doc->currentPage()->xOffset(), -m_Doc->currentPage()->yOffset());
					arrowTrans.translate(Start.x() + ite->xPos(), Start.y() + ite->yPos());
					EndArrow.map(arrowTrans);
					int zS = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, obState.CurrColorStroke, CommonStrings::None);
					iteS = m_Doc->Items->at(zS);
					iteS->PoLine = EndArrow.copy();
					iteS->ClipEdited = true;
					iteS->FrameType = 3;
					FPoint wh = getMaxClipF(&iteS->PoLine);
					iteS->setWidthHeight(wh.x(), wh.y());
					m_Doc->AdjustItemSize(iteS, true);
					iteS->setFillEvenOdd(false);
					iteS->OldB2 = iteS->width();
					iteS->OldH2 = iteS->height();
					iteS->updateClip();
					iteS->OwnPage = m_Doc->OnPage(iteS);
					iteS->setFillTransparency(obState.strokeOpacity);
					m_Doc->Items->removeLast();
					break;
				}
			}
		}

	}
	return iteS;
}

PageItem* OdgPlug::applyEndArrow(PageItem* ite, ObjStyle &obState)
{
	PageItem *iteS = NULL;
	if (!obState.endMarkerName.isEmpty())
	{
		ObjStyle mStyle;
		resovleStyle(mStyle, obState.endMarkerName);
		double EndArrowWidth = obState.endMarkerWidth;
		QPainterPath pa = mStyle.markerPath;
		FPointArray EndArrow;
		EndArrow.fromQPainterPath(pa);
		QRectF br = pa.boundingRect();
		if (EndArrowWidth > 0)
		{
			FPoint End = ite->PoLine.point(ite->PoLine.size()-2);
			for (uint xx = ite->PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = ite->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
					QPointF refP;
					if (obState.endMarkerCentered)
						refP = QPointF(br.width() / 2.0, br.height() / 2.0);
					else
						refP = QPointF(br.width() / 2.0, 0);
					QTransform m;
					m.translate(br.width() / 2.0, br.height() / 2.0);
					m.rotate(r + 90);
					m.translate(-br.width() / 2.0, -br.height() / 2.0);
					m.scale(EndArrowWidth / br.width(), EndArrowWidth / br.width());
					EndArrow.map(m);
					refP = m.map(refP);
					QTransform m2;
					FPoint grOffset2(getMinClipF(&EndArrow));
					m2.translate(-grOffset2.x(), -grOffset2.y());
					EndArrow.map(m2);
					refP = m2.map(refP);
					EndArrow.translate(-refP.x(), -refP.y());
					QTransform arrowTrans;
					arrowTrans.translate(-m_Doc->currentPage()->xOffset(), -m_Doc->currentPage()->yOffset());
					arrowTrans.translate(End.x() + ite->xPos(), End.y() + ite->yPos());
					EndArrow.map(arrowTrans);
					int zE = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, obState.CurrColorStroke, CommonStrings::None);
					iteS = m_Doc->Items->at(zE);
					iteS->PoLine = EndArrow.copy();
					iteS->ClipEdited = true;
					iteS->FrameType = 3;
					FPoint wh = getMaxClipF(&iteS->PoLine);
					iteS->setWidthHeight(wh.x(), wh.y());
					m_Doc->AdjustItemSize(iteS, true);
					iteS->setFillEvenOdd(false);
					iteS->OldB2 = iteS->width();
					iteS->OldH2 = iteS->height();
					iteS->updateClip();
					iteS->OwnPage = m_Doc->OnPage(iteS);
					iteS->setFillTransparency(obState.strokeOpacity);
					m_Doc->Items->removeLast();
					break;
				}
			}
		}
	}
	return iteS;
}

void OdgPlug::finishItem(PageItem* item, ObjStyle &obState)
{
	item->ClipEdited = true;
	item->FrameType = 3;
	FPoint wh = getMaxClipF(&item->PoLine);
	item->setWidthHeight(wh.x(), wh.y());
	item->Clip = FlattenPath(item->PoLine, item->Segments);
	m_Doc->AdjustItemSize(item, true);
	item->OldB2 = item->width();
	item->OldH2 = item->height();
	item->updateClip();
	item->OwnPage = m_Doc->OnPage(item);
	item->setFillTransparency(obState.fillOpacity);
	item->setLineTransparency(obState.strokeOpacity);
	item->setStartArrowIndex(0);
	item->setEndArrowIndex(0);
	if (obState.stroke_type == 2)
	{
		ObjStyle dStyle;
		resovleStyle(dStyle, obState.dashName);
		item->DashValues.clear();
		double gap = 0;
		if (dStyle.stroke_dash_distance < 0)
			gap = item->lineWidth();
		else
			gap = dStyle.stroke_dash_distance;
		int dots1 = dStyle.stroke_dash_dots1;
		double dots1len = item->lineWidth();
		if (dStyle.stroke_dash_dots1_length < 0)
			dots1len = item->lineWidth();
		else
			dots1len = dStyle.stroke_dash_dots1_length;
		int dots2 = dStyle.stroke_dash_dots2;
		double dots2len = item->lineWidth();
		if (dStyle.stroke_dash_dots2_length < 0)
			dots2len = item->lineWidth();
		else
			dots2len = dStyle.stroke_dash_dots2_length;
		for (int i = 0; i < dots1; i++)
		{
			item->DashValues << qMax(dots1len, 0.1) << qMax(gap, 0.1);
		}
		for (int j = 0; j < dots2; j++)
		{
			item->DashValues << qMax(dots2len, 0.1) << qMax(gap, 0.1);
		}
	}
	if (obState.fill_type == 2)
	{
		ObjStyle gStyle;
		resovleStyle(gStyle, obState.gradientName);
		if (gStyle.gradientType == "linear")
		{
			double angle = gStyle.gradientAngle + 90;
			item->fill_gradient = VGradient(VGradient::linear);
			item->fill_gradient.clearStops();
			item->fill_gradient.setRepeatMethod( VGradient::none );
			const ScColor& gradC = m_Doc->PageColors[gStyle.gradientEndColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), 0.0, 0.5, 1.0, gStyle.gradientEndColor, gStyle.gradientEndShade);
			const ScColor& gradC2 = m_Doc->PageColors[gStyle.gradientStartColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0 - gStyle.gradientBorder, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			QLineF gradientVectorE;
			gradientVectorE.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorE.setAngle(angle);
			gradientVectorE.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradEnd = intersectBoundingRect(item, gradientVectorE);
			QLineF gradientVectorS;
			gradientVectorS.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorS.setAngle(angle + 180);
			gradientVectorS.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradStart = intersectBoundingRect(item, gradientVectorS);
			item->setGradientVector(gradStart.x(), gradStart.y(), gradEnd.x(), gradEnd.y(), gradStart.x(), gradStart.y(), 1, 0);
			item->setGradientType(6);
		}
		else if (gStyle.gradientType == "axial")
		{
			double angle = gStyle.gradientAngle + 90;
			item->fill_gradient = VGradient(VGradient::linear);
			item->fill_gradient.clearStops();
			item->fill_gradient.setRepeatMethod( VGradient::none );
			const ScColor& gradC = m_Doc->PageColors[gStyle.gradientEndColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), 0.0 + (gStyle.gradientBorder / 2.0), 0.5, 1.0, gStyle.gradientEndColor, gStyle.gradientEndShade);
			const ScColor& gradC2 = m_Doc->PageColors[gStyle.gradientStartColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC2, m_Doc), 0.5, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), 1.0 - (gStyle.gradientBorder / 2.0), 0.5, 1.0, gStyle.gradientEndColor, gStyle.gradientEndShade);
			QLineF gradientVectorE;
			gradientVectorE.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorE.setAngle(angle);
			gradientVectorE.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradEnd = intersectBoundingRect(item, gradientVectorE);
			QLineF gradientVectorS;
			gradientVectorS.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorS.setAngle(angle + 180);
			gradientVectorS.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradStart = intersectBoundingRect(item, gradientVectorS);
			item->setGradientVector(gradStart.x(), gradStart.y(), gradEnd.x(), gradEnd.y(), gradStart.x(), gradStart.y(), 1, 0);
			item->setGradientType(6);
		}
		else if (gStyle.gradientType == "radial")
		{
			item->fill_gradient = VGradient(VGradient::radial);
			item->fill_gradient.clearStops();
			item->fill_gradient.setRepeatMethod( VGradient::none );
			const ScColor& gradC = m_Doc->PageColors[gStyle.gradientEndColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), 0.0, 0.5, 1.0, gStyle.gradientEndColor, gStyle.gradientEndShade);
			const ScColor& gradC2 = m_Doc->PageColors[gStyle.gradientStartColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0 - gStyle.gradientBorder, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			item->GrType = 7;
			item->GrStartX = item->width() * gStyle.gradientCenterX;
			item->GrStartY = item->height()* gStyle.gradientCenterY;
			item->GrFocalX = item->width() * gStyle.gradientCenterX;
			item->GrFocalY = item->height()* gStyle.gradientCenterY;
			if (item->width() >= item->height())
			{
				item->GrEndX = item->width();
				item->GrEndY = item->height() / 2.0;
			}
			else
			{
				item->GrEndX = item->width() / 2.0;
				item->GrEndY = item->height();
			}
			item->updateGradientVectors();
		}
		else if (gStyle.gradientType == "ellipsoid")
		{
			item->fill_gradient = VGradient(VGradient::radial);
			item->fill_gradient.clearStops();
			item->fill_gradient.setRepeatMethod( VGradient::none );
			const ScColor& gradC = m_Doc->PageColors[gStyle.gradientEndColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), 0.0, 0.5, 1.0, gStyle.gradientEndColor, gStyle.gradientEndShade);
			const ScColor& gradC2 = m_Doc->PageColors[gStyle.gradientStartColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0 - gStyle.gradientBorder, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			item->GrType = 7;
			item->GrStartX = item->width() * gStyle.gradientCenterX;
			item->GrStartY = item->height()* gStyle.gradientCenterY;
			item->GrFocalX = item->width() * gStyle.gradientCenterX;
			item->GrFocalY = item->height()* gStyle.gradientCenterY;
			if (item->width() >= item->height())
			{
				item->GrEndX = item->width();
				item->GrEndY = item->height() / 2.0;
			}
			else
			{
				item->GrEndX = item->width() / 2.0;
				item->GrEndY = item->height();
			}
			QLineF gradientVectorE = QLineF(item->GrStartX, item->GrStartY, item->GrEndX, item->GrEndY);
			gradientVectorE.setAngle(gStyle.gradientAngle);
			item->GrEndX = gradientVectorE.p2().x();
			item->GrEndY = gradientVectorE.p2().y();
			item->updateGradientVectors();
		}
		else if (gStyle.gradientType == "square")
		{
			item->fill_gradient = VGradient(VGradient::radial);
			item->fill_gradient.clearStops();
			item->fill_gradient.setRepeatMethod( VGradient::none );
			const ScColor& gradC = m_Doc->PageColors[gStyle.gradientEndColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), 0.0, 0.5, 1.0, gStyle.gradientEndColor, gStyle.gradientEndShade);
			const ScColor& gradC2 = m_Doc->PageColors[gStyle.gradientStartColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0 - gStyle.gradientBorder, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			if (gStyle.gradientBorder != 0)
				item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			FPoint cp = FPoint(item->width() * gStyle.gradientCenterX, item->height()* gStyle.gradientCenterY);
			double gLen = qMin(item->width(), item->height()) / 2.0;
			QLineF p1 = QLineF(cp.x(), cp.y(), cp.x() - gLen, cp.y() - gLen);
			p1.setAngle(p1.angle() + gStyle.gradientAngle);
			QLineF p2 = QLineF(cp.x(), cp.y(), cp.x() + gLen, cp.y() - gLen);
			p2.setAngle(p2.angle() + gStyle.gradientAngle);
			QLineF p3 = QLineF(cp.x(), cp.y(), cp.x() + gLen, cp.y() + gLen);
			p3.setAngle(p3.angle() + gStyle.gradientAngle);
			QLineF p4 = QLineF(cp.x(), cp.y(), cp.x() - gLen, cp.y() + gLen);
			p4.setAngle(p4.angle() + gStyle.gradientAngle);
			item->setDiamondGeometry(FPoint(p1.p2().x(), p1.p2().y()), FPoint(p2.p2().x(), p2.p2().y()), FPoint(p3.p2().x(), p3.p2().y()), FPoint(p4.p2().x(), p4.p2().y()), cp);
			item->GrType = 10;
		}
		else if (gStyle.gradientType == "rectangular")
		{
			item->fill_gradient = VGradient(VGradient::radial);
			item->fill_gradient.clearStops();
			item->fill_gradient.setRepeatMethod( VGradient::none );
			const ScColor& gradC = m_Doc->PageColors[gStyle.gradientEndColor];
			QColor gradColor1 = ScColorEngine::getRGBColor(gradC, m_Doc);
			item->fill_gradient.addStop(gradColor1, 0.0, 0.5, 1.0, gStyle.gradientEndColor, gStyle.gradientEndShade);
			const ScColor& gradC2 = m_Doc->PageColors[gStyle.gradientStartColor];
			QColor gradColor2 = ScColorEngine::getRGBColor(gradC2, m_Doc);
			item->fill_gradient.addStop(gradColor2, 1.0 - gStyle.gradientBorder, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			if (gStyle.gradientBorder != 0)
				item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			FPoint cp = FPoint(item->width() * gStyle.gradientCenterX, item->height()* gStyle.gradientCenterY);
			double gLenW = item->width() / 2.0;
			double gLenH = item->height() / 2.0;

			QPointF P1 = QPointF(0.0, 0.0);
			QPointF P2 = QPointF(item->width(), 0.0);
			QPointF P3 = QPointF(item->width(), item->height());
			QPointF P4 = QPointF(0.0, item->height());
			QLineF L1 = QLineF(0.0, 0.0, item->width(), 0.0);
			L1.setAngle(-45);
			QLineF LCW = QLineF(0.0, item->height() / 2.0, item->width(), item->height() / 2.0);
			QPointF P5;
			LCW.intersect(L1, &P5);
			QPointF P6 = QPointF(item->width() - P5.x(), P5.y());
			QPolygonF pPoints;
			pPoints << P1 << P2 << P3 << P4 << P5 << P6;
			QTransform mat;
			pPoints.translate(-item->width() / 2.0, -item->height() / 2.0);
			mat.translate(item->width() * gStyle.gradientCenterX, item->height()* gStyle.gradientCenterY);
			mat.rotate(-gStyle.gradientAngle);
			mat.scale(1.0 - gStyle.gradientBorder, 1.0 - gStyle.gradientBorder);
			pPoints = mat.map(pPoints);
			P1 = pPoints[0];
			P2 = pPoints[1];
			P3 = pPoints[2];
			P4 = pPoints[3];
			P5 = pPoints[4];
			P6 = pPoints[5];
/*
			QPointF cpL = QPointF(item->width() * gStyle.gradientCenterX, item->height()* gStyle.gradientCenterY);
			double lineLen = sqrt(gLenW * gLenW + gLenH * gLenH) * 2.0;
			QLineF iLineP1 = QLineF(cpL, P1);
			iLineP1.setLength(lineLen);
			P1 = intersectBoundingRect(item, iLineP1);
			QLineF iLineP2 = QLineF(cpL, P2);
			iLineP2.setLength(lineLen);
			P2 = intersectBoundingRect(item, iLineP2);
			QLineF iLineP3 = QLineF(cpL, P3);
			iLineP3.setLength(lineLen);
			P3 = intersectBoundingRect(item, iLineP3);
			QLineF iLineP4 = QLineF(cpL, P4);
			iLineP4.setLength(lineLen);
			P4 = intersectBoundingRect(item, iLineP4);
			item->setDiamondGeometry(FPoint(P1.x(), P1.y()), FPoint(P2.x(), P2.y()), FPoint(P3.x(), P3.y()), FPoint(P4.x(), P4.y()), cp);
			item->GrType = 10;
*/
/*
			item->meshGradientPatches.clear();
			meshGradientPatch patch1;
			meshPoint outer;
			outer.resetTo(FPoint(P1.x(), P1.y()));
			outer.transparency = 1.0;
			outer.shade = gStyle.gradientStartShade;
			outer.colorName = gStyle.gradientStartColor;
			outer.color = gradColor2;
			patch1.TL = outer;
			outer.resetTo(FPoint(P2.x(), P2.y()));
			patch1.TR = outer;
			meshPoint inner;
			inner.resetTo(FPoint(P6.x(), P6.y()));
			inner.transparency = 1.0;
			inner.shade = gStyle.gradientEndShade;
			inner.colorName = gStyle.gradientEndColor;
			inner.color = gradColor1;
			patch1.BR = inner;
			inner.resetTo(FPoint(P5.x(), P5.y()));
			patch1.BL = inner;
			item->meshGradientPatches.append(patch1);

			outer.resetTo(FPoint(P2.x(), P2.y()));
			patch1.TL = outer;
			outer.resetTo(FPoint(P3.x(), P3.y()));
			patch1.TR = outer;
			inner.resetTo(FPoint(P6.x(), P6.y()));
			patch1.BL = inner;
			patch1.BR = inner;
			item->meshGradientPatches.append(patch1);

			inner.resetTo(FPoint(P5.x(), P5.y()));
			patch1.TL = inner;
			inner.resetTo(FPoint(P6.x(), P6.y()));
			patch1.TR = inner;
			outer.resetTo(FPoint(P4.x(), P4.y()));
			patch1.BL = outer;
			outer.resetTo(FPoint(P3.x(), P3.y()));
			patch1.BR = outer;
			item->meshGradientPatches.append(patch1);

			outer.resetTo(FPoint(P4.x(), P4.y()));
			patch1.BL = outer;
			outer.resetTo(FPoint(P1.x(), P1.y()));
			patch1.TL = outer;
			inner.resetTo(FPoint(P5.x(), P5.y()));
			patch1.BR = inner;
			patch1.TR = inner;
			item->meshGradientPatches.append(patch1);
			item->GrType = 12;
*/

			QLineF p1 = QLineF(cp.x(), cp.y(), cp.x() - gLenW, cp.y() - gLenH);
			p1.setAngle(p1.angle() + gStyle.gradientAngle);
			QLineF p2 = QLineF(cp.x(), cp.y(), cp.x() + gLenW, cp.y() - gLenH);
			p2.setAngle(p2.angle() + gStyle.gradientAngle);
			QLineF p3 = QLineF(cp.x(), cp.y(), cp.x() + gLenW, cp.y() + gLenH);
			p3.setAngle(p3.angle() + gStyle.gradientAngle);
			QLineF p4 = QLineF(cp.x(), cp.y(), cp.x() - gLenW, cp.y() + gLenH);
			p4.setAngle(p4.angle() + gStyle.gradientAngle);
			item->setDiamondGeometry(FPoint(p1.p2().x(), p1.p2().y()), FPoint(p2.p2().x(), p2.p2().y()), FPoint(p3.p2().x(), p3.p2().y()), FPoint(p4.p2().x(), p4.p2().y()), cp);
			item->GrType = 10;
		}
	}
	else if (obState.fill_type == 3)
	{
		ObjStyle gStyle;
		resovleStyle(gStyle, obState.patternName);
		QString patternName = "Pattern_" + obState.patternName;
		if (m_Doc->docPatterns.contains(patternName))
		{
			ScPattern pat = m_Doc->docPatterns[patternName];
			double sy = 100.0;
			double sx = 100.0;
			double dx = 0;
			double dy = 0;
			if (obState.patternStretch == "stretch")
			{
				sx = item->width() / pat.width * 100;
				sy = item->height() / pat.height * 100;
			}
			else
			{
				if (obState.patternDim_H_in_Percent)
					sy = obState.patternHeight * 100.0;
				else
				{
					if (obState.patternHeight > 0.0)
						sy = obState.patternHeight / pat.height * 100.0;
				}
				if (obState.patternDim_W_in_Percent)
					sx = obState.patternWidth * 100.0;
				else
				{
					if (obState.patternWidth > 0.0)
						sx = obState.patternWidth / pat.width * 100.0;
				}
				if (obState.patternX > 0.0)
					dx = pat.width * obState.patternX;
				if (obState.patternY > 0.0)
					dy = pat.height * obState.patternY;
			}
			item->setPatternTransform(sx, sy, dx, dy, 0, 0, 0);
			item->setPattern(patternName);
			item->GrType = 8;
		}
		else
		{
			if (!gStyle.patternPath.isEmpty())
			{
				QByteArray f;
				if (uz->read(gStyle.patternPath, f))
				{
					QFileInfo fi(gStyle.patternPath);
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_odg_XXXXXX." + fi.suffix());
					tempFile->setAutoRemove(false);
					if (tempFile->open())
					{
						QString fileName = getLongPathName(tempFile->fileName());
						if (!fileName.isEmpty())
						{
							tempFile->write(f);
							tempFile->close();
							ScPattern pat = ScPattern();
							pat.setDoc(m_Doc);
							int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, 0, 0, 1, 1, 0, CommonStrings::None, CommonStrings::None);
							PageItem* newItem = m_Doc->Items->at(z);
							m_Doc->loadPict(fileName, newItem);
							m_Doc->Items->takeAt(z);
							newItem->isInlineImage = true;
							newItem->isTempFile = true;
							pat.width = newItem->pixm.qImage().width();
							pat.height = newItem->pixm.qImage().height();
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
							patternName = patternName.trimmed().simplified().replace(" ", "_");
							m_Doc->addPattern(patternName, pat);
							item->setPattern(patternName);
							double sy = 100.0;
							double sx = 100.0;
							double dx = 0;
							double dy = 0;
							if (obState.patternStretch == "stretch")
							{
								sx = item->width() / pat.width * 100;
								sy = item->height() / pat.height * 100;
							}
							else
							{
								if (obState.patternDim_H_in_Percent)
									sy = obState.patternHeight * 100.0;
								else
								{
									if (obState.patternHeight > 0.0)
										sy = obState.patternHeight / pat.height * 100.0;
								}
								if (obState.patternDim_W_in_Percent)
									sx = obState.patternWidth * 100.0;
								else
								{
									if (obState.patternWidth > 0.0)
										sx = obState.patternWidth / pat.width * 100.0;
								}
								if (obState.patternX > 0.0)
									dx = pat.width * obState.patternX;
								if (obState.patternY > 0.0)
									dy = pat.height * obState.patternY;
							}
							item->setPatternTransform(sx, sy, dx, dy, 0, 0, 0);
							item->GrType = 8;
						}
					}
					delete tempFile;
				}
			}
			else if (!gStyle.patternData.isEmpty())
			{
				QString ext = "";
				QByteArray buf = QByteArray::fromBase64(gStyle.patternData);
				if ((buf[0] == '%') && (buf[1] == '!') && (buf[2] == 'P') && (buf[3] == 'S') && (buf[4] == '-') && (buf[5] == 'A'))
					ext = "eps";
				else if ((buf[0] == '\xC5') && (buf[1] == '\xD0') && (buf[2] == '\xD3') && (buf[3] == '\xC6'))
					ext = "eps";
				else if ((buf[0] == 'G') && (buf[1] == 'I') && (buf[2] == 'F') && (buf[3] == '8'))
					ext = "gif";
				else if ((buf[0] == '\xFF') && (buf[1] == '\xD8') && (buf[2] == '\xFF'))
					ext = "jpg";
				else if ((buf[0] == 'P') && (buf[1] == 'G') && (buf[2] == 'F'))
					ext = "pgf";
				else if ((buf[0] == '\x89') && (buf[1] == 'P') && (buf[2] == 'N') && (buf[3] == 'G'))
					ext = "png";
				else if ((buf[0] == '8') && (buf[1] == 'B') && (buf[2] == 'P') && (buf[3] == 'S'))
					ext = "psd";
				else if (((buf[0] == 'I') && (buf[1] == 'I') && (buf[2] == '\x2A')) || ((buf[0] == 'M') && (buf[1] == 'M') && (buf[3] == '\x2A')))
					ext = "tif";
				else if ((buf[0] == '/') && (buf[1] == '*') && (buf[2] == ' ') && (buf[3] == 'X') && (buf[4] == 'P') && (buf[5] == 'M'))
					ext = "xpm";
				else if ((buf[0] == 'V') && (buf[1] == 'C') && (buf[2] == 'L') && (buf[3] == 'M') && (buf[4] == 'T') && (buf[5] == 'F'))
					ext = "svm";
				if (!ext.isEmpty())
				{
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_odg_XXXXXX." + ext);
					tempFile->setAutoRemove(false);
					if (tempFile->open())
					{
						QString fileName = getLongPathName(tempFile->fileName());
						if (!fileName.isEmpty())
						{
							tempFile->write(buf);
							tempFile->close();
							ScPattern pat = ScPattern();
							pat.setDoc(m_Doc);
							int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, 0, 0, 1, 1, 0, CommonStrings::None, CommonStrings::None);
							PageItem* newItem = m_Doc->Items->at(z);
							m_Doc->loadPict(fileName, newItem);
							m_Doc->Items->takeAt(z);
							newItem->isInlineImage = true;
							newItem->isTempFile = true;
							pat.width = newItem->pixm.qImage().width();
							pat.height = newItem->pixm.qImage().height();
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
							patternName = patternName.trimmed().simplified().replace(" ", "_");
							m_Doc->addPattern(patternName, pat);
							item->setPattern(patternName);
							double sy = 100.0;
							double sx = 100.0;
							double dx = 0;
							double dy = 0;
							if (obState.patternStretch == "stretch")
							{
								sx = item->width() / pat.width * 100;
								sy = item->height() / pat.height * 100;
							}
							else
							{
								if (obState.patternDim_H_in_Percent)
									sy = obState.patternHeight * 100.0;
								else
								{
									if (obState.patternHeight > 0.0)
										sy = obState.patternHeight / pat.height * 100.0;
								}
								if (obState.patternDim_W_in_Percent)
									sx = obState.patternWidth * 100.0;
								else
								{
									if (obState.patternWidth > 0.0)
										sx = obState.patternWidth / pat.width * 100.0;
								}
								if (obState.patternX > 0.0)
									dx = pat.width * obState.patternX;
								if (obState.patternY > 0.0)
									dy = pat.height * obState.patternY;
							}
							item->setPatternTransform(sx, sy, dx, dy, 0, 0, 0);
							item->GrType = 8;
						}
					}
					delete tempFile;
				}
			}
		}
	}
	else if (obState.fill_type == 4)
	{
		ObjStyle gStyle;
		resovleStyle(gStyle, obState.hatchName);
		int hatchS = 0;
		if (gStyle.hatchStyle == "double")
			hatchS = 1;
		else if (gStyle.hatchStyle == "triple")
			hatchS = 2;
		item->setHatchParameters(hatchS, gStyle.hatchDistance, gStyle.hatchRotation, obState.hatchSolidFill, obState.CurrColorFill, gStyle.hatchColor);
		item->GrType = 14;
	}
	if (!obState.opacityName.isEmpty())
	{
		ObjStyle gStyle;
		resovleStyle(gStyle, obState.opacityName);
		if (gStyle.gradientType == "linear")
		{
			double angle = gStyle.gradientAngle + 90;
			VGradient maskGradient;
			maskGradient = VGradient(VGradient::linear);
			maskGradient.clearStops();
			maskGradient.setRepeatMethod( VGradient::none );
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityStart), 0.0, 0.5, 1.0, "Black", gStyle.opacityStart);
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityEnd), 1.0 - gStyle.gradientBorder, 0.5, 1.0, "Black", gStyle.opacityEnd);
			QLineF gradientVectorE;
			gradientVectorE.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorE.setAngle(angle);
			gradientVectorE.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradEnd = intersectBoundingRect(item, gradientVectorE);
			QLineF gradientVectorS;
			gradientVectorS.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorS.setAngle(angle + 180);
			gradientVectorS.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradStart = intersectBoundingRect(item, gradientVectorS);
			item->setMaskGradient(maskGradient);
			item->setMaskVector(gradStart.x(), gradStart.y(), gradEnd.x(), gradEnd.y(), gradStart.x(), gradStart.y(), 1, 0);
			item->setMaskType(4);
		}
		else if (gStyle.gradientType == "axial")
		{
			double angle = gStyle.gradientAngle + 90;
			VGradient maskGradient;
			maskGradient = VGradient(VGradient::linear);
			maskGradient.clearStops();
			maskGradient.setRepeatMethod( VGradient::none );
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityEnd), 1.0  + (gStyle.gradientBorder / 2.0), 0.5, 1.0, "Black", gStyle.opacityEnd);
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityStart), 0.0, 0.5, 1.0, "Black", gStyle.opacityStart);
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityEnd), 1.0 - (gStyle.gradientBorder / 2.0), 0.5, 1.0, "Black", gStyle.opacityEnd);
			QLineF gradientVectorE;
			gradientVectorE.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorE.setAngle(angle);
			gradientVectorE.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradEnd = intersectBoundingRect(item, gradientVectorE);
			QLineF gradientVectorS;
			gradientVectorS.setP1(QPointF(item->width() / 2.0, item->height() / 2.0));
			gradientVectorS.setAngle(angle + 180);
			gradientVectorS.setLength(sqrt(item->width() * item->width() + item->height() * item->height()) / 2.0 + 1.0);
			QPointF gradStart = intersectBoundingRect(item, gradientVectorS);
			item->setMaskGradient(maskGradient);
			item->setMaskVector(gradStart.x(), gradStart.y(), gradEnd.x(), gradEnd.y(), gradStart.x(), gradStart.y(), 1, 0);
			item->setGradientType(4);
		}
		else if (gStyle.gradientType == "radial")
		{
			VGradient maskGradient;
			maskGradient = VGradient(VGradient::radial);
			maskGradient.clearStops();
			maskGradient.setRepeatMethod( VGradient::none );
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityStart), 0.0, 0.5, 1.0, "Black", gStyle.opacityStart);
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityEnd), 1.0 - gStyle.gradientBorder, 0.5, 1.0, "Black", gStyle.opacityEnd);
			double GrStartX = item->width() * gStyle.gradientCenterX;
			double GrStartY = item->height()* gStyle.gradientCenterY;
			double GrEndX = 0;
			double GrEndY = 0;
			if (item->width() >= item->height())
			{
				GrEndX = item->width();
				GrEndY = item->height() / 2.0;
			}
			else
			{
				GrEndX = item->width() / 2.0;
				GrEndY = item->height();
			}
			item->setMaskGradient(maskGradient);
			item->setMaskVector(GrStartX, GrStartY, GrEndX, GrEndY, GrStartX, GrStartY, 1, 0);
			item->setMaskType(5);
		}
		else if (gStyle.gradientType == "ellipsoid")
		{
			VGradient maskGradient;
			maskGradient = VGradient(VGradient::radial);
			maskGradient.clearStops();
			maskGradient.setRepeatMethod( VGradient::none );
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityStart), 0.0, 0.5, 1.0, "Black", gStyle.opacityStart);
			maskGradient.addStop(ScColorEngine::getShadeColorProof(m_Doc->PageColors["Black"], m_Doc, gStyle.opacityEnd), 1.0 - gStyle.gradientBorder, 0.5, 1.0, "Black", gStyle.opacityEnd);
			double GrStartX = item->width() * gStyle.gradientCenterX;
			double GrStartY = item->height()* gStyle.gradientCenterY;
			double GrEndX = 0;
			double GrEndY = 0;
			if (item->width() >= item->height())
			{
				GrEndX = item->width();
				GrEndY = item->height() / 2.0;
			}
			else
			{
				GrEndX = item->width() / 2.0;
				GrEndY = item->height();
			}
			QLineF gradientVectorE = QLineF(GrStartX, GrStartY, GrEndX, GrEndY);
			gradientVectorE.setAngle(gStyle.gradientAngle);
			GrEndX = gradientVectorE.p2().x();
			GrEndY = gradientVectorE.p2().y();
			item->setMaskGradient(maskGradient);
			item->setMaskVector(GrStartX, GrStartY, GrEndX, GrEndY, GrStartX, GrStartY, 1, 0);
			item->setMaskType(5);
		}
	}
	if (obState.hasShadow)
	{
		item->setHasSoftShadow(true);
		item->setSoftShadowColor(obState.CurrColorShadow);
		item->setSoftShadowXOffset(obState.shadowX);
		item->setSoftShadowYOffset(obState.shadowY);
		item->setSoftShadowBlurRadius(0);
		item->setSoftShadowShade(100);
		item->setSoftShadowOpacity(obState.shadowTrans);
		item->setSoftShadowBlendMode(0);
		item->setSoftShadowErasedByObject(false);
		item->setSoftShadowHasObjectTransparency(false);
	}
}
