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
#include <QDebug>

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include <cstdlib>
#include <climits>
#include <limits>

#include "color.h"

#include "importodg.h"
#include "fileloader.h"
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
#include "scgzfile.h"
#include "scmimedata.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "sctextstream.h"
#include "selection.h"
#include "undomanager.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"
#include "plugins/formatidlist.h"

#include "ui/customfdialog.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"

#include "third_party/zip/scribus_zip.h"

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
			return QImage();
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
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}

	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	if (ext == "fodg")
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
			if (progressDialog)
				progressDialog->close();
			return false;
		}
		retVal = false;
		if (uz->contains("styles.xml"))
			retVal = parseStyleSheets("styles.xml");
		if (uz->contains("content.xml"))
			retVal = parseDocReference("content.xml");
		uz->close();
		delete uz;
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
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(f, &errorMsg, &errorLine, &errorColumn))
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
					currStyle.page_layout_name = AttributeValue(spf.attribute("style:page-layout-name"));
				m_Styles.insert(spf.attribute("style:name"), currStyle);
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
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(f, &errorMsg, &errorLine, &errorColumn))
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
		else if (drawPag.tagName() == "office:body")
		{
			for(QDomElement sp = drawPag.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
			{
				if (sp.tagName() == "office:drawing")
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
									m_Doc->currentPage()->initialMargins.Top = topMargin;
									m_Doc->currentPage()->initialMargins.Bottom = bottomMargin;
									m_Doc->currentPage()->initialMargins.Left = leftMargin;
									m_Doc->currentPage()->initialMargins.Right = rightMargin;
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
									m_Doc->currentPage()->initialMargins.Top = topMargin;
									m_Doc->currentPage()->initialMargins.Bottom = bottomMargin;
									m_Doc->currentPage()->initialMargins.Left = leftMargin;
									m_Doc->currentPage()->initialMargins.Right = rightMargin;
									m_Doc->currentPage()->MPageNam = CommonStrings::trMasterPageNormal;
									m_Doc->view()->addPage(pagecount, true);
									pagecount++;
								}
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
		for(QDomElement spd = draw.firstChildElement(); !spd.isNull(); spd = spd.nextSiblingElement() )
		{
			PageItem* ite = parseObj(spd);
			if (ite != NULL)
				GElements.append(ite);
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
	else
		qDebug() << "Unhandled Tag" << draw.tagName();
	return retObj;
}

PageItem* OdgPlug::parseLine(const QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	double x1 = e.attribute( "svg:x1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:x1" ) );
	double y1 = e.attribute( "svg:y1" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:y1" ) );
	double x2 = e.attribute( "svg:x2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:x2" ) );
	double y2 = e.attribute( "svg:y2" ).isEmpty() ? 0.0 : parseUnit( e.attribute( "svg:y2" ) );
	resovleStyle(tmpOStyle, e.attribute("draw:style-name"));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, CommonStrings::None, tmpOStyle.CurrColorStroke, true);
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
		}
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
	resovleStyle(tmpOStyle, e.attribute("draw:style-name"));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke, true);
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
	resovleStyle(tmpOStyle, e.attribute("draw:style-name"));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke, true);
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
	resovleStyle(tmpOStyle, e.attribute("draw:style-name"));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke, true);
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
	resovleStyle(tmpOStyle, e.attribute("draw:style-name"));
	if ((tmpOStyle.fill_type == 0) && (tmpOStyle.stroke_type == 0))
		return retObj;
	int z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, tmpOStyle.LineW, CommonStrings::None, tmpOStyle.CurrColorStroke, true);
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
		}
	}
	return retObj;
}

PageItem* OdgPlug::parsePath(QDomElement &e)
{
	ObjStyle tmpOStyle;
	PageItem *retObj = NULL;
	resovleStyle(tmpOStyle, e.attribute("draw:style-name"));
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
		int z = m_Doc->itemAdd(itype, PageItem::Unspecified, baseX + x, baseY + y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke, true);
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
		QRectF clipRect = retObj->PoLine.toQPainterPath(false).boundingRect();
		FPoint tp2(clipRect.left(), clipRect.top());
		retObj->PoLine.translate(-tp2.x(), -tp2.y());
		if (e.hasAttribute("draw:transform"))
			parseTransform(&retObj->PoLine, e.attribute("draw:transform"));
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
				}
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
	resovleStyle(tmpOStyle, e.attribute("draw:style-name"));
	QDomElement n = e.firstChildElement();
	if (!n.isNull())
	{
		if (n.tagName() == "draw:text-box" )
		{
			if (n.text().isEmpty())
				return retObj;
			int z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke, true);
			retObj = m_Doc->Items->at(z);
			retObj->setTextToFrameDist(0.0, 0.0, 0.0, 0.0);
			retObj->setTextFlowMode(PageItem::TextFlowDisabled);
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
					int z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX+x, baseY+y, w, h, tmpOStyle.LineW, tmpOStyle.CurrColorFill, tmpOStyle.CurrColorStroke, true);
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
						QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_odg_XXXXXX." + fi.suffix());
						tempFile->setAutoRemove(false);
						if (tempFile->open())
						{
							QString fileName = getLongPathName(tempFile->fileName());
							if (!fileName.isEmpty())
							{
								tempFile->write(f);
								tempFile->close();
								FileLoader *fileLoader = new FileLoader(fileName);
								int testResult = fileLoader->testFile();
								delete fileLoader;
								if (testResult != -1)
								{
									const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
									if( fmt )
									{
										m_Doc->m_Selection->clear();
										fmt->setupTargets(m_Doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
										fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
										if (m_Doc->m_Selection->count() > 0)
										{
											retObj = m_Doc->groupObjectsSelection();
											retObj->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
											retObj->setXYPos(baseX + x, baseY + y, true);
											retObj->setWidthHeight(w, h, true);
											retObj->updateClip();
										}
										m_Doc->m_Selection->clear();
										m_Doc->Items->removeLast();
									}
								}
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
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setDefaultStyle(false);
	newStyle.setParent(pStyle);
	ParagraphStyle ttx = m_Doc->paragraphStyle(pStyle);
	CharStyle nstyle = ttx.charStyle();
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	newStyle.setLineSpacing(nstyle.fontSize() / 10.0);
	item->itemText.clear();
	item->itemText.setDefaultStyle(newStyle);
	item->setFirstLineOffset(FLOPFontAscent);
	for(QDomElement para = elem.firstChildElement(); !para.isNull(); para = para.nextSiblingElement())
	{
		if (para.hasAttributes() && para.hasChildNodes())
		{
			ObjStyle pStyle;
			resovleStyle(pStyle, para.attribute("text:style-name"));
			ParagraphStyle tmpStyle = newStyle;
			tmpStyle.setAlignment(pStyle.textAlign);
			tmpStyle.setLeftMargin(pStyle.margin_left);
			tmpStyle.setRightMargin(pStyle.margin_right);
			tmpStyle.setFirstIndent(pStyle.textIndent);
			tmpStyle.setGapAfter(pStyle.margin_bottom);
			tmpStyle.setGapBefore(pStyle.margin_top);
			double maxFsize = 0.0;
			for(QDomElement sp = para.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement())
			{
				ObjStyle cStyle;
				resovleStyle(cStyle, sp.attribute("text:style-name"));
				CharStyle tmpCStyle = tmpStyle.charStyle();
				tmpCStyle.setFont((*m_Doc->AllFonts)[cStyle.fontName]);
				tmpCStyle.setFontSize(cStyle.fontSize * 10);
				tmpCStyle.setFillColor(cStyle.CurrColorText);
				maxFsize = qMax(maxFsize, cStyle.fontSize);
				QString txt = sp.text().trimmed();
				if (txt.length() > 0)
				{
					item->itemText.insertChars(posC, txt);
					item->itemText.applyStyle(posC, tmpStyle);
					item->itemText.applyCharStyle(posC, txt.length(), tmpCStyle);
					posC = item->itemText.length();
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
			QString txt = para.text().trimmed();
			ParagraphStyle tmpStyle = newStyle;
			tmpStyle.setAlignment(tmpOStyle.textAlign);
			tmpStyle.setLeftMargin(tmpOStyle.margin_left);
			tmpStyle.setRightMargin(tmpOStyle.margin_right);
			tmpStyle.setFirstIndent(tmpOStyle.textIndent);
			tmpStyle.setGapAfter(tmpOStyle.margin_bottom);
			tmpStyle.setGapBefore(tmpOStyle.margin_top);
			CharStyle tmpCStyle = tmpStyle.charStyle();
			tmpCStyle.setFont((*m_Doc->AllFonts)[tmpOStyle.fontName]);
			tmpCStyle.setFontSize(tmpOStyle.fontSize * 10);
			tmpCStyle.setFillColor(tmpOStyle.CurrColorText);
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
			if (txt.length() > 0)
			{
				item->itemText.insertChars(posC, txt);
				item->itemText.applyStyle(posC, tmpStyle);
				item->itemText.applyCharStyle(posC, txt.length(), tmpCStyle);
				posC = item->itemText.length();
			}
			item->itemText.insertChars(posC, SpecialChars::PARSEP);
			item->itemText.applyStyle(posC, tmpStyle);
			posC = item->itemText.length();
		}
	}
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
					currStyle.strokeOpacity = AttributeValue(spe.attribute("svg:stroke-opacity", ""));
					currStyle.LineW = AttributeValue(spe.attribute("svg:stroke-width", ""));
					currStyle.fillOpacity = AttributeValue(spe.attribute("draw:opacity", ""));
					currStyle.gradientName = AttributeValue(spe.attribute("draw:fill-gradient-name", ""));
					currStyle.dashName = AttributeValue(spe.attribute("draw:stroke-dash", ""));
					currStyle.startMarkerName = AttributeValue(spe.attribute("draw:marker-start", ""));
					currStyle.startMarkerWidth = AttributeValue(spe.attribute("draw:marker-start-width", ""));
					currStyle.endMarkerName = AttributeValue(spe.attribute("draw:marker-end", ""));
					currStyle.endMarkerWidth = AttributeValue(spe.attribute("draw:marker-end-width", ""));
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
				if (currStyle.endMarkerName.valid)
					actStyle.endMarkerName = AttributeValue(currStyle.endMarkerName.value);
				if (currStyle.endMarkerWidth.valid)
					actStyle.endMarkerWidth = AttributeValue(currStyle.endMarkerWidth.value);
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
				tmpOStyle.fill_type = 3;
			else if (actStyle.fillMode.value == "hatch")
				tmpOStyle.fill_type = 4;
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
		if (actStyle.fillOpacity.valid)
			tmpOStyle.fillOpacity = 1.0 - parseUnit(actStyle.fillOpacity.value);
		if (actStyle.strokeOpacity.valid)
			tmpOStyle.strokeOpacity = 1.0 - parseUnit(actStyle.strokeOpacity.value);
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
		if (actStyle.endMarkerName.valid)
			tmpOStyle.endMarkerName = actStyle.endMarkerName.value;
		if (actStyle.endMarkerWidth.valid)
			tmpOStyle.endMarkerWidth = parseUnit(actStyle.endMarkerWidth.value);
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
		value = value;
	else if( unit.right( 2 ) == "cm" )
		value = ( value / 2.54 ) * 72;
	else if( unit.right( 2 ) == "mm" )
		value = ( value / 25.4 ) * 72;
	else if( unit.right( 2 ) == "in" )
		value = value * 72;
	else if( unit.right( 2 ) == "px" )
		value = value;
	else if( unit.right( 1 ) == "%" )
		value = value / 100.0;
	return value;
}

PageItem* OdgPlug::addClip(PageItem* retObj, ObjStyle &obState)
{
	/*if (!obState.clipPath.isEmpty())
	{
		int z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, baseX, baseY, 10, 10, 0, CommonStrings::None, CommonStrings::None, true);
		PageItem *itemg = m_Doc->Items->at(z);
		itemg->PoLine.fromQPainterPath(obState.clipPath);
		FPoint wh = getMaxClipF(&itemg->PoLine);
		itemg->setWidthHeight(wh.x(),wh.y());
		m_Doc->AdjustItemSize(itemg, true);
		itemg->ClipEdited = true;
		itemg->FrameType = 3;
		itemg->setFillEvenOdd(false);
		itemg->OldB2 = itemg->width();
		itemg->OldH2 = itemg->height();
		itemg->updateClip();
		itemg->OwnPage = m_Doc->OnPage(itemg);
		itemg->ContourLine = itemg->PoLine.copy();
		QList<PageItem*> GElements;
		GElements.append(retObj);
		m_Doc->groupObjectsToItem(itemg, GElements);
		m_Doc->resizeGroupToContents(itemg);
		m_Doc->GroupOnPage(itemg);
		retObj = itemg;
		m_Doc->Items->removeLast();
	}*/
	return retObj;
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
					QPointF refP = QPointF(br.width() / 2.0, 0);
					QTransform m;
					m.translate(br.width() / 2.0, br.height() / 2.0);
					m.rotate(r + 90);
					m.translate(-br.width() / 2.0, -br.height() / 2.0);
					m.scale(EndArrowWidth / br.width(), EndArrowWidth / br.width());
					EndArrow.map(m);
					refP = m.map(refP);
					QPainterPath pa2 = EndArrow.toQPainterPath(true);
					QRectF br2 = pa2.boundingRect();
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
					int zS = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, obState.CurrColorStroke, CommonStrings::None, true);
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
					QPointF refP = QPointF(br.width() / 2.0, 0);
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
					int zE = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, 0, obState.CurrColorStroke, CommonStrings::None, true);
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
	item->setFillEvenOdd(false);
	item->OldB2 = item->width();
	item->OldH2 = item->height();
	item->updateClip();
	item->OwnPage = m_Doc->OnPage(item);
	if (item->isTextFrame())
	{
		item->setFillColor(obState.CurrColorFill);
		item->setLineColor(obState.CurrColorStroke);
	}
	item->setFillTransparency(obState.fillOpacity);
	item->setLineTransparency(obState.strokeOpacity);
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
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), 0.0, 0.5, 1.0, gStyle.gradientEndColor, gStyle.gradientEndShade);
			const ScColor& gradC2 = m_Doc->PageColors[gStyle.gradientStartColor];
			item->fill_gradient.addStop(ScColorEngine::getRGBColor(gradC2, m_Doc), 1.0 - gStyle.gradientBorder, 0.5, 1.0, gStyle.gradientStartColor, gStyle.gradientStartShade);
			FPoint cp = FPoint(item->width() * gStyle.gradientCenterX, item->height()* gStyle.gradientCenterY);
			double gLenW = item->width() / 2.0;
			double gLenH = item->height() / 2.0;
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
/*	if (m_StyleSheets.contains(m_currentStyleSheet))
	{
		StyleSheet currSH = m_StyleSheets[m_currentStyleSheet];
		if (!obState.layoutStyleRef.isEmpty())
		{
			if (currSH.m_layoutStyles.contains(obState.layoutStyleRef))
			{
				LayoutStyle actStyle;
				LayoutStyle currStyle = currSH.m_layoutStyles[obState.layoutStyleRef];
				QStringList parents;
				while (currStyle.parentStyle.valid)
				{
					if (currSH.m_layoutStyles.contains(currStyle.parentStyle.value))
					{
						parents.prepend(currStyle.parentStyle.value);
						currStyle = currSH.m_layoutStyles[currStyle.parentStyle.value];
					}
					else
						break;
				}
				parents.append(obState.layoutStyleRef);
				double textMarginLeft = 0.0;
				double textMarginRight = 0.0;
				double textMarginTop = 0.0;
				double textMarginBottom = 0.0;
				double textColumnGap = 0.0;
				int textColumnCount = 1;
				if (!parents.isEmpty())
				{
					for (int p = 0; p < parents.count(); p++)
					{
						currStyle = currSH.m_layoutStyles[parents[p]];
						if (currStyle.Extra.valid)
							actStyle.Extra = AttributeValue(currStyle.Extra.value);
						if (currStyle.RExtra.valid)
							actStyle.RExtra = AttributeValue(currStyle.RExtra.value);
						if (currStyle.TExtra.valid)
							actStyle.TExtra = AttributeValue(currStyle.TExtra.value);
						if (currStyle.BExtra.valid)
							actStyle.BExtra = AttributeValue(currStyle.BExtra.value);
						if (currStyle.TextColumnGutter.valid)
							actStyle.TextColumnGutter = AttributeValue(currStyle.TextColumnGutter.value);
						if (currStyle.TextColumnCount.valid)
							actStyle.TextColumnCount = AttributeValue(currStyle.TextColumnCount.value);
					}
				}
				if (actStyle.Extra.valid)
					textMarginLeft = actStyle.Extra.value.toDouble();
				if (actStyle.RExtra.valid)
					textMarginRight = actStyle.RExtra.value.toDouble();
				if (actStyle.TExtra.valid)
					textMarginTop = actStyle.TExtra.value.toDouble();
				if (actStyle.BExtra.valid)
					textMarginBottom = actStyle.BExtra.value.toDouble();
				if (actStyle.TextColumnGutter.valid)
					textColumnGap = actStyle.TextColumnGutter.value.toDouble();
				if (actStyle.TextColumnCount.valid)
					textColumnCount = actStyle.TextColumnCount.value.toInt();
				item->setTextToFrameDist(textMarginLeft, textMarginRight, textMarginTop, textMarginBottom);
				item->setColumns(textColumnCount);
				item->setColumnGap(textColumnGap);
			}
		}
		if (!obState.styleRef.isEmpty())
		{
			if (currSH.m_objStyles.contains(obState.styleRef))
			{
				ObjStyle actStyle;
				ObjStyle currStyle = currSH.m_objStyles[obState.styleRef];
				QStringList parents;
				while (currStyle.parentStyle.valid)
				{
					if (currSH.m_objStyles.contains(currStyle.parentStyle.value))
					{
						parents.prepend(currStyle.parentStyle.value);
						currStyle = currSH.m_objStyles[currStyle.parentStyle.value];
					}
					else
						break;
				}
				parents.append(obState.styleRef);
				if (!parents.isEmpty())
				{
					for (int p = 0; p < parents.count(); p++)
					{
						currStyle = currSH.m_objStyles[parents[p]];
						if (currStyle.CurrColorFill.valid)
							actStyle.CurrColorFill = AttributeValue(currStyle.CurrColorFill.value);
						if (currStyle.CurrColorStroke.valid)
							actStyle.CurrColorStroke = AttributeValue(currStyle.CurrColorStroke.value);
						if (currStyle.fillOpacity.valid)
							actStyle.fillOpacity = AttributeValue(currStyle.fillOpacity.value);
						if (currStyle.strokeOpacity.valid)
							actStyle.strokeOpacity = AttributeValue(currStyle.strokeOpacity.value);
						if (currStyle.opacity.valid)
							actStyle.opacity = AttributeValue(currStyle.opacity.value);
						if (currStyle.LineW.valid)
							actStyle.LineW = AttributeValue(currStyle.LineW.value);
						if (currStyle.CapStyle.valid)
							actStyle.CapStyle = AttributeValue(currStyle.CapStyle.value);
						if (currStyle.JoinStyle.valid)
							actStyle.JoinStyle = AttributeValue(currStyle.JoinStyle.value);
					}
				}
				if (actStyle.CurrColorFill.valid)
					item->setFillColor(actStyle.CurrColorFill.value);
				if (actStyle.CurrColorStroke.valid)
					item->setLineColor(actStyle.CurrColorStroke.value);
				if (actStyle.fillOpacity.valid)
					item->setFillTransparency(actStyle.fillOpacity.value.toDouble());
				if (actStyle.strokeOpacity.valid)
					item->setLineTransparency(actStyle.strokeOpacity.value.toDouble());
				if (actStyle.LineW.valid)
					item->setLineWidth(actStyle.LineW.value.toDouble());
				if (actStyle.CapStyle.valid)
				{
					if (actStyle.CapStyle.value == "butt")
						item->setLineEnd(Qt::FlatCap);
					else if (actStyle.CapStyle.value == "round")
						item->setLineEnd(Qt::RoundCap);
					else if (actStyle.CapStyle.value == "square")
						item->setLineEnd(Qt::SquareCap);
					else
						item->setLineEnd(Qt::FlatCap);
				}
				if (actStyle.JoinStyle.valid)
				{
					if (actStyle.JoinStyle.value == "miter")
						item->setLineJoin(Qt::MiterJoin);
					else if (actStyle.JoinStyle.value == "round")
						item->setLineJoin(Qt::RoundJoin);
					else if (actStyle.JoinStyle.value == "bevel")
						item->setLineJoin(Qt::BevelJoin);
					else
						item->setLineJoin(Qt::MiterJoin);
				}
			}
		}
	}*/
/*	double xp = item->xPos() - m_Doc->currentPage()->xOffset();
	double yp = item->yPos() - m_Doc->currentPage()->yOffset();
	if (obState.fillGradientTyp != 0)
	{
		item->fill_gradient = obState.currentGradient;
		item->setGradientVector(obState.gradientStart.x() - xp, obState.gradientStart.y() - yp, obState.gradientEnd.x() - xp, obState.gradientEnd.y() - yp, obState.gradientFocus.x() - xp, obState.gradientFocus.y() - yp, obState.gradientScale, 0);
		item->setGradientType(obState.fillGradientTyp);
	}
	else if (!obState.patternName.isEmpty())
	{
		item->setPattern(obState.patternName);
		item->GrType = 8;
	}
	if (obState.maskTyp != 0)
	{
		item->setMaskGradient(obState.gradientMask);
		item->setMaskVector(obState.maskStart.x() - xp, obState.maskStart.y() - yp, obState.maskEnd.x() - xp, obState.maskEnd.y() - yp, obState.maskFocus.x() - xp, obState.maskFocus.y() - yp, obState.maskScale, 0);
		item->setMaskType(obState.maskTyp);
	}
	if (!obState.patternMask.isEmpty())
	{
		item->setPatternMask(obState.patternMask);
		item->setMaskType(obState.maskTyp);
	}
	if (obState.strokeTyp != 0)
	{
		item->setStrokeGradient(obState.gradientStroke);
		item->setStrokeGradientVector(obState.strokeStart.x() - xp, obState.strokeStart.y() - yp, obState.strokeEnd.x() - xp, obState.strokeEnd.y() - yp, obState.strokeFocus.x() - xp, obState.strokeFocus.y() - yp, obState.strokeScale, 0);
		item->setStrokeGradientType(obState.strokeTyp);
	}
	if (!obState.patternStroke.isEmpty())
	{
		item->GrTypeStroke = 8;
		item->setStrokePattern(obState.patternStroke);
	}
	if (!obState.DashPattern.isEmpty())
	{
		item->setDashOffset(obState.DashOffset);
		QVector<double> pattern(obState.DashPattern.count());
		for (int i = 0; i < obState.DashPattern.count(); ++i)
		{
			pattern[i] = obState.DashPattern[i] * obState.LineW;
		}
		item->setDashes(pattern);
	}*/
}
