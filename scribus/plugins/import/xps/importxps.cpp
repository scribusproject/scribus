/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Oct 26 2013
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

#include "importxps.h"
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
#include "xpsimportoptions.h"

#include "third_party/zip/scribus_zip.h"

extern SCRIBUS_API ScribusQApp * ScQApp;

XpsPlug::XpsPlug(ScribusDoc* doc, int flags)
{
	tmpSel = new Selection(this, false);
	m_Doc = doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = NULL;
	uz = NULL;
}

QImage XpsPlug::readThumbnail(QString fName)
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
	}
	bool found = false;
	if (uz->contains("_rels/.rels"))
	{
		QByteArray f;
		QDomDocument designMapDom;
		if (!uz->read("_rels/.rels", f))
			return QImage();
		if(designMapDom.setContent(f))
		{
			QDomElement docElem = designMapDom.documentElement();
			for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
			{
				if (drawPag.tagName() != "Relationship")
					continue;
				if ((!drawPag.hasAttribute("Target")) || (!drawPag.hasAttribute("Type")))
					continue;
				if (drawPag.attribute("Type") != "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail")
					continue;
				QString thumbRef = drawPag.attribute("Target");
				if (thumbRef.startsWith("/"))
					thumbRef = thumbRef.mid(1);
				QByteArray im;
				if (uz->read(thumbRef, im))
				{
					tmp = QImage::fromData(im);
					found = true;
					break;
				}
			}
		}
	}
	if (!found)
	{
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
		baseY = m_Doc->currentPage()->yOffset() + m_Doc->currentPage()->height() / 2.0;
		Elements.clear();
		m_Doc->setLoading(true);
		m_Doc->DoDrawing = false;
		m_Doc->scMW()->setScriptRunning(true);
		QString CurDirP = QDir::currentPath();
		QDir::setCurrent(fi.path());
		importedColors.clear();
		importedPatterns.clear();
		conversionFactor = 72.0 / 96.0;
		loadedFonts.clear();
		linkTargets.clear();
		linkSources.clear();
		if (uz->contains("FixedDocSeq.fdseq"))
			parseDocSequence("FixedDocSeq.fdseq");
		else if (uz->contains("FixedDocumentSequence.fdseq"))
			parseDocSequence("FixedDocumentSequence.fdseq");
		if (Elements.count() > 0)
		{
			tmpSel->clear();
			QDir::setCurrent(CurDirP);
			if (Elements.count() > 1)
				m_Doc->groupObjectsList(Elements);
			m_Doc->DoDrawing = true;
			m_Doc->m_Selection->delaySignalsOn();
			if (Elements.count() > 0)
			{
				for (int dre=0; dre<Elements.count(); ++dre)
				{
					tmpSel->addItem(Elements.at(dre), true);
				}
				tmpSel->setGroupRect();
				double xs = tmpSel->width();
				double ys = tmpSel->height();
				tmp = Elements.at(0)->DrawObj_toImage(500);
				tmp.setText("XSize", QString("%1").arg(xs));
				tmp.setText("YSize", QString("%1").arg(ys));
			}
			m_Doc->scMW()->setScriptRunning(false);
			m_Doc->setLoading(false);
			m_Doc->m_Selection->delaySignalsOff();
			delete m_Doc;
		}
		else
		{
			QDir::setCurrent(CurDirP);
			m_Doc->DoDrawing = true;
			m_Doc->scMW()->setScriptRunning(false);
			delete m_Doc;
		}
	}
	uz->close();
	delete uz;
	return tmp;
}

bool XpsPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
{
	QString fName = fNameIn;
	bool success = false;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	importerFlags = flags;
	cancel = false;
	bool ret = false;
	firstPage = true;
	pagecount = 1;
	QFileInfo fi = QFileInfo(fName);
	m_FileName = fi.fileName();
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

XpsPlug::~XpsPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
	for (int a = 0; a < tempFontFiles.count(); a++)
	{
		QFile::remove(tempFontFiles[a]);
	}
}

bool XpsPlug::convert(QString fn)
{
	bool retVal = true;
	importedColors.clear();
	importedPatterns.clear();
	conversionFactor = 72.0 / 96.0;
	loadedFonts.clear();
	linkTargets.clear();
	linkSources.clear();
	pathResources.clear();
	if(progressDialog)
	{
		progressDialog->setOverallProgress(2);
		progressDialog->setLabel("GI", tr("Generating Items"));
		qApp->processEvents();
	}

	uz = new ScZipHandler();
	if (!uz->open(fn))
	{
		delete uz;
		if (progressDialog)
			progressDialog->close();
	}
	if (uz->contains("FixedDocSeq.fdseq"))
	{
		parseDocSequence("FixedDocSeq.fdseq");
		resolveLinks();
	}
	else if (uz->contains("FixedDocumentSequence.fdseq"))
	{
		parseDocSequence("FixedDocumentSequence.fdseq");
		resolveLinks();
	}
	else
		retVal = false;
	uz->close();
	delete uz;
	if (progressDialog)
		progressDialog->close();
	return retVal;
}

void XpsPlug::parseDocSequence(QString designMap)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(designMap, f))
		return;
	if(designMapDom.setContent(f))
	{
		QString DocumentReference = "";
		QDomElement docElem = designMapDom.documentElement();
		for(QDomNode drawPag = docElem.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling() )
		{
			QDomElement dpg = drawPag.toElement();
			if (dpg.tagName() == "DocumentReference")
			{
				if (dpg.hasAttribute("Source"))
				{
					DocumentReference = dpg.attribute("Source", "");
					if (DocumentReference.startsWith("/"))
						DocumentReference = DocumentReference.mid(1);
					parseDocReference(DocumentReference);
				}
			}
		}
	}
}

void XpsPlug::parseDocReference(QString designMap)
{
	QByteArray f;
	QFileInfo fi(designMap);
	QString path = fi.path();
	if (uz->read(designMap, f))
	{
		QDomDocument designMapDom;
		if(designMapDom.setContent(f))
		{
			QString PageReference = "";
			QDomElement docElem = designMapDom.documentElement();
			if (importerFlags & LoadSavePlugin::lfCreateThumbnail)
			{
				QDomNodeList pgList = docElem.childNodes();
				QDomNode drawPag = pgList.item(0);
				QDomElement dpg = drawPag.toElement();
				if (dpg.tagName() == "PageContent")
				{
					if (dpg.hasAttribute("Source"))
					{
						PageReference = dpg.attribute("Source", "");
						if (PageReference.startsWith("/"))
						{
							PageReference = PageReference.mid(1);
							parsePageReference(PageReference);
						}
						else
						{
							if (!PageReference.startsWith(path))
							{
								PageReference = path + "/" + PageReference;
								PageReference = QDir::cleanPath(PageReference);
							}
							parsePageReference(PageReference);
						}
					}
				}
			}
			else
			{
				std::vector<int> pageNs;
				QString pageString = "*";
				int pgCount = 0;
				int maxPages = docElem.childNodes().count();
				if (((interactive) || (importerFlags & LoadSavePlugin::lfCreateDoc)) && (maxPages > 1))
				{
					if (progressDialog)
						progressDialog->hide();
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					XpsImportOptions *optImp = new XpsImportOptions(ScCore->primaryMainWindow());
					optImp->setUpOptions(m_FileName, 1, maxPages, interactive);
					optImp->exec();
					pageString = optImp->getPagesString();
					delete optImp;
					qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
					if (progressDialog)
						progressDialog->show();
					qApp->processEvents();
				}
				parsePagesString(pageString, &pageNs, maxPages);
				if (pageString != "*")
					maxPages = pageNs.size();
				if (progressDialog)
				{
					progressDialog->setTotalSteps("GI", maxPages);
					progressDialog->setProgress("GI", pgCount);
					qApp->processEvents();
				}
				QDomNodeList pgList = docElem.childNodes();
				for (uint ap = 0; ap < pageNs.size(); ++ap)
				{
					QDomNode drawPag = pgList.item(pageNs[ap] - 1);
					QDomElement dpg = drawPag.toElement();
					if (dpg.tagName() == "PageContent")
					{
						if (dpg.hasAttribute("Source"))
						{
							PageReference = dpg.attribute("Source", "");
							if (PageReference.startsWith("/"))
							{
								PageReference = PageReference.mid(1);
								parsePageReference(PageReference);
							}
							else
							{
								if (!PageReference.startsWith(path))
								{
									PageReference = path + "/" + PageReference;
									PageReference = QDir::cleanPath(PageReference);
								}
								parsePageReference(PageReference);
							}
						}
					}
					pgCount++;
					if (progressDialog)
					{
						progressDialog->setProgress("GI", pgCount);
						qApp->processEvents();
					}
				}
			}
		}
	}
}

void XpsPlug::parsePageReference(QString designMap)
{
	QByteArray f;
	QFileInfo fi(designMap);
	QString path = fi.path();
	if (uz->read(designMap, f))
	{
		QDomDocument designMapDom;
		if(designMapDom.setContent(f))
		{
			QDomElement docElem = designMapDom.documentElement();
			docWidth = docElem.attribute("Width", QString("%1").arg(PrefsManager::instance()->appPrefs.docSetupPrefs.pageWidth)).toDouble() * conversionFactor;
			docHeight = docElem.attribute("Height", QString("%1").arg(PrefsManager::instance()->appPrefs.docSetupPrefs.pageHeight)).toDouble() * conversionFactor;
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				if (firstPage)
				{
					topMargin = m_Doc->marginsVal().Top;
					leftMargin = m_Doc->marginsVal().Left;
					rightMargin = m_Doc->marginsVal().Right;
					bottomMargin = m_Doc->marginsVal().Bottom;
					double pgCols = m_Doc->PageSp;
					double pgGap = m_Doc->PageSpa;
					m_Doc->setPage(docWidth, docHeight, topMargin, leftMargin, rightMargin, bottomMargin, pgCols, pgGap, false, false);
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
			for(QDomNode drawPag = docElem.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling() )
			{
				QDomElement dpg = drawPag.toElement();
				if ((dpg.tagName() == "Path") || (dpg.tagName() == "Glyphs") || (dpg.tagName() == "Canvas"))
				{
					PageItem* item = parseObjectXML(dpg, path);
					if (item != NULL)
					{
						m_Doc->Items->append(item);
						Elements.append(item);
					}
				}
				else if (dpg.tagName() == "FixedPage.Resources")
				{
					for(QDomNode sp = dpg.firstChild(); !sp.isNull(); sp = sp.nextSibling() )
					{
						QDomElement spe = sp.toElement();
						if (spe.tagName() == "ResourceDictionary")
						{
							if (spe.hasAttribute("Source"))
							{
								QString resFile = spe.attribute("Source", "");
								if (resFile.startsWith("/"))
								{
									resFile = resFile.mid(1);
									parseResourceFile(resFile);
								}
								else
								{
									if (!resFile.startsWith(path))
									{
										resFile = path + "/" + resFile;
										resFile = QDir::cleanPath(resFile);
									}
									parseResourceFile(resFile);
								}
							}
							else if (spe.hasChildNodes())
							{
								for(QDomElement dpgp = spe.firstChildElement(); !dpgp.isNull(); dpgp = dpgp.nextSiblingElement() )
								{
									if (dpgp.tagName() == "PathGeometry")
									{
										Coords.resize(0);
										Coords.svgInit();
										QString pdata = "";
										QString key = dpg.attribute("x:Key");
										if (dpg.hasAttribute("Figures"))
											pdata = dpg.attribute("Figures");
										else if (dpg.hasChildNodes())
											pdata = parsePathGeometryXML(dpg);
										if (!pdata.isEmpty())
										{
											bool currentPathClosed = Coords.parseSVG(pdata);
											Coords.scale(conversionFactor, conversionFactor);
											QPainterPath path = Coords.toQPainterPath(!currentPathClosed);
											if (dpg.attribute("FillRule") == "NonZero")
												path.setFillRule(Qt::WindingFill);
											pathResources.insert(key, path);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

PageItem* XpsPlug::parseObjectXML(QDomElement &dpg, QString path)
{
	PageItem *retObj = NULL;
	ObjState obState;
	obState.currentPath = QPainterPath();
	obState.clipPath = QPainterPath();
	obState.transform = QTransform();
	obState.CurrColorFill = CommonStrings::None;
	obState.fillOpacity = 0.0;
	obState.CurrColorStroke = CommonStrings::None;
	obState.strokeOpacity = 0.0;
	obState.LineW = 1.0;
	obState.fillGradientTyp = 0;
	obState.gradientScale = 1.0;
	obState.maskTyp = 0;
	obState.strokeTyp = 0;
	obState.imagePath = "";
	obState.itemType = 0;
	obState.patternName = "";
	obState.patternMask = "";
	obState.CapStyle = Qt::FlatCap;
	obState.JoinStyle = Qt::MiterJoin;
	obState.DashOffset = 0;
	obState.DashPattern.clear();
	QString itemName = "";
	QString itemTarget = "";
	if (dpg.hasAttribute("Name"))
		itemName = dpg.attribute("Name");
	if (dpg.hasAttribute("FixedPage.NavigateUri"))
	{
		QString iTg = dpg.attribute("FixedPage.NavigateUri");
		QStringList tg = iTg.split("#");
		if (tg.count() > 1)
			itemTarget = tg[1];
	}
	if (dpg.hasAttribute("RenderTransform"))
	{
		QString trans = dpg.attribute("RenderTransform", "1 0 0 1 0 0");
		trans.replace(",", " ");
		ScTextStream list(&trans, QIODevice::ReadOnly);
		double a, b, c, d, e, f;
		list >> a >> b >> c >> d >> e >> f;
		obState.transform = QTransform(a, b, c, d, e * conversionFactor, f * conversionFactor);
	}
	if (dpg.hasAttribute("Fill"))
		obState.CurrColorFill = handleColor(dpg.attribute("Fill", "#FF000000"), obState.fillOpacity);
	if (dpg.hasAttribute("Stroke"))
		obState.CurrColorStroke = handleColor(dpg.attribute("Stroke", "#FF000000"), obState.strokeOpacity);
	if (dpg.hasAttribute("Opacity"))
	{
		double opacity = dpg.attribute("Opacity", "1.0").toDouble();
		obState.fillOpacity = 1.0 - ((1.0 - obState.fillOpacity) * opacity);
		obState.strokeOpacity = 1.0 - ((1.0 - obState.strokeOpacity) * opacity);
	}
	if (dpg.hasAttribute("StrokeThickness"))
		obState.LineW = dpg.attribute("StrokeThickness", "1.0").toDouble() * conversionFactor;
	if (dpg.hasAttribute("StrokeDashCap"))
	{
		if (dpg.attribute("StrokeDashCap") == "Flat")
			obState.CapStyle = Qt::FlatCap;
		else if (dpg.attribute("StrokeDashCap") == "Round")
			obState.CapStyle = Qt::RoundCap;
		else if (dpg.attribute("StrokeDashCap") == "Square")
			obState.CapStyle = Qt::SquareCap;
		else
			obState.CapStyle = Qt::FlatCap;
	}
	if (dpg.hasAttribute("StrokeLineJoin"))
	{
		if (dpg.attribute("StrokeLineJoin") == "Miter")
			obState.JoinStyle = Qt::MiterJoin;
		else if (dpg.attribute("StrokeLineJoin") == "Round")
			obState.JoinStyle = Qt::RoundJoin;
		else if (dpg.attribute("StrokeLineJoin") == "Bevel")
			obState.JoinStyle = Qt::BevelJoin;
		else
			obState.JoinStyle = Qt::MiterJoin;
	}
	if (dpg.hasAttribute("StrokeDashOffset"))
		obState.DashOffset = dpg.attribute("StrokeDashOffset", "0.0").toDouble();
	if (dpg.hasAttribute("StrokeDashArray"))
	{
		QString trans = dpg.attribute("StrokeDashArray", "");
		if (!trans.isEmpty())
		{
			ScTextStream list(&trans, QIODevice::ReadOnly);
			while (!list.atEnd())
			{
				double d;
				list >> d;
				obState.DashPattern.append(d);
			}
		}
	}
	if (dpg.hasAttribute("Clip"))
	{
		QString cdata = dpg.attribute("Clip");
		if (!cdata.startsWith("{"))
		{
			Coords.resize(0);
			Coords.svgInit();
			Coords.parseSVG(cdata);
			Coords.scale(conversionFactor, conversionFactor);
			obState.clipPath = Coords.toQPainterPath(true);
			obState.clipPath = obState.transform.map(obState.clipPath);
		}
		else
		{
			cdata.remove("{StaticResource ");
			cdata.remove("}");
			cdata = cdata.trimmed();
			if (pathResources.contains(cdata))
				obState.clipPath = obState.transform.map(pathResources[cdata]);
		}
	}
	if (dpg.tagName() == "Glyphs")
	{
		if (dpg.hasChildNodes())
		{
			for(QDomNode sp = dpg.firstChild(); !sp.isNull(); sp = sp.nextSibling() )
			{
				QDomElement spe = sp.toElement();
				if (spe.tagName() == "Glyphs.Fill")
					parseFillXML(spe, path, obState);
				else if (spe.tagName() == "Glyphs.OpacityMask")
					parseOpacityXML(spe, path, obState);
				else if (spe.tagName() == "Glyphs.Clip")
					parsePathDataXML(spe, obState, true);
			}
		}
		QString utfString = dpg.attribute("UnicodeString");
		if (utfString.startsWith("{}"))
			utfString = utfString.mid(2);
		QString Indices = dpg.attribute("Indices");
		QString tstStr = utfString.trimmed();
		if (tstStr.isEmpty() && Indices.isEmpty())
			return retObj;
		QString fontUrl = dpg.attribute("FontUri");
		if (fontUrl.startsWith("/"))
		{
			fontUrl = fontUrl.mid(1);
		}
		else
		{
			if (!fontUrl.startsWith(path))
			{
				fontUrl = path + "/" + fontUrl;
				fontUrl = QDir::cleanPath(fontUrl);
			}
		}
		ScFace iteFont = loadFontByName(fontUrl);
		if (iteFont.usable())
		{
			double fontSize = dpg.attribute("FontRenderingEmSize", "0").toDouble() * conversionFactor;
			bool bold = false;
			bool italic = false;
			if (dpg.hasAttribute("StyleSimulations"))
			{
				bold = dpg.attribute("StyleSimulations") == "BoldSimulation";
				italic = dpg.attribute("StyleSimulations") == "ItalicSimulation";
				if (dpg.attribute("StyleSimulations") == "BoldItalicSimulation")
				{
					bold = true;
					italic = true;
				}
			}
			double fontSizeEM = fontSize;
			if (fontSize > 1)
			{
				double xPos = dpg.attribute("OriginX", "0").toDouble() * conversionFactor;
				double yPos = dpg.attribute("OriginY", "0").toDouble() * conversionFactor;
				QPointF origin(xPos, yPos);
				QStringList glIndices = Indices.split(";");
				double asc = iteFont.ascent() - iteFont.descent();
				fontSize /= asc;
				int glInd = 0;
				if (!utfString.isEmpty())
				{
					for (int sti = 0; sti < utfString.length(); sti++)
					{
						QChar utfChar = utfString[sti];
						if (iteFont.canRender(utfChar))
						{
							uint chr = utfChar.unicode();
							uint gl = iteFont.char2CMap(chr);
							double adv = iteFont.glyphWidth(gl, fontSize);
							double offX = 0;
							double offY = 0;
							FPointArray pts;
							if (Indices.isEmpty())
							{
								if (!utfChar.isSpace())
									pts = iteFont.glyphOutline(gl, fontSize);
							}
							else
							{
								if (glInd < glIndices.count())
								{
									QStringList glyInd = glIndices[glInd].split(",");
									if (glyInd.count() > 1)
									{
										if (glyInd.count() > 2)
											offX = glyInd[2].toDouble() * fontSizeEM / 100.0;
										if (glyInd.count() > 3)
											offY = glyInd[3].toDouble() * fontSizeEM / 100.0;
										if (!glyInd[1].isEmpty())
											adv = glyInd[1].toDouble() * fontSizeEM / 100.0;
									}
									if (glyInd.count() > 0)
									{
										if (!glyInd[0].isEmpty())
										{
											if (glyInd[0].startsWith("("))
											{
												int r = glyInd[0].indexOf(")");
												QString comb = glyInd[0].mid(1, r-1);
												QStringList combInd = comb.split(":");
												int advUtf = combInd[0].toInt() - 1;
												sti += advUtf;
												glyInd[0].remove(0, r+1);
											}
											uint gli = glyInd[0].toUInt();
											pts = iteFont.glyphOutline(gli, fontSize);
											if (((glyInd.count() > 1) && (glyInd[1].isEmpty())) || (glyInd.count() == 1))
												adv = iteFont.glyphWidth(gli, fontSize);
										}
										else if (!utfChar.isSpace())
											pts = iteFont.glyphOutline(gl, fontSize);
									}
								}
							}
							if (pts.count() > 3)
							{
								QTransform cht;
								cht.scale(0.1, 0.1);
								pts.map(cht);
								pts.translate(0, -fontSize);
								if (bold)
								{
									QTransform bot;
									bot.scale(1.02, 1.02);
									pts.map(bot);
									adv *= 1.02;
								}
								if (italic)
								{
									QTransform bot;
									bot.shear(-0.3491, 0);
									pts.map(bot);
								}
								pts.translate(origin.x() + offX, origin.y() + offY);
								obState.currentPath.addPath(pts.toQPainterPath(true));
							}
							origin += QPointF(adv, 0);
						}
						else
						{
							QFont eFont(iteFont.family());
							eFont.setPointSizeF(fontSize);
							eFont.setBold(bold);
							eFont.setItalic(italic);
							QFontMetricsF ft(eFont);
							obState.currentPath.addText(origin, eFont, utfChar);
							origin += QPointF(ft.width(utfChar), 0);
						}
						glInd++;
					}
				}
				else if (!Indices.isEmpty())
				{
					double adv = 0;
					double offX = 0;
					double offY = 0;
					for (int glInd = 0; glInd < glIndices.count(); glInd++)
					{
						FPointArray pts;
						QStringList glyInd = glIndices[glInd].split(",");
						if (glyInd.count() > 1)
						{
							if (glyInd.count() > 2)
								offX = glyInd[2].toDouble() * fontSizeEM / 100.0;
							if (glyInd.count() > 3)
								offY = glyInd[3].toDouble() * fontSizeEM / 100.0;
							if (!glyInd[1].isEmpty())
								adv = glyInd[1].toDouble() * fontSizeEM / 100.0;
						}
						if (glyInd.count() > 0)
						{
							if (!glyInd[0].isEmpty())
							{
								if (glyInd[0].startsWith("("))
								{
									int r = glyInd[0].indexOf(")");
									QString comb = glyInd[0].mid(1, r-1);
									QStringList combInd = comb.split(":");
									int advUtf = combInd[0].toInt() - 1;
									glInd += advUtf;
									glyInd[0].remove(0, r+1);
								}
								uint gli = glyInd[0].toUInt();
								pts = iteFont.glyphOutline(gli, fontSize);
								if (((glyInd.count() > 1) && (glyInd[1].isEmpty())) || (glyInd.count() == 1))
									adv = iteFont.glyphWidth(gli, fontSize);
							}
						}
						if (pts.count() > 3)
						{
							QTransform cht;
							cht.scale(0.1, 0.1);
							pts.map(cht);
							pts.translate(0, -fontSize);
							if (bold)
							{
								QTransform bot;
								bot.scale(1.02, 1.02);
								pts.map(bot);
								adv *= 1.02;
							}
							if (italic)
							{
								QTransform bot;
								bot.shear(-0.3491, 0);
								pts.map(bot);
							}
							pts.translate(origin.x() + offX, origin.y() + offY);
							obState.currentPath.addPath(pts.toQPainterPath(true));
						}
						origin += QPointF(adv, 0);
					}
				}
				if (!obState.currentPath.isEmpty())
				{
					obState.currentPath = obState.transform.map(obState.currentPath);
					retObj = createItem(dpg, obState);
					if (!obState.clipPath.isEmpty())
						retObj = addClip(retObj, obState);
				}
			}
		}
	}
	else if (dpg.tagName() == "Path")
	{
		bool pathFromChild = false;
		if (dpg.hasChildNodes())
		{
			for(QDomNode sp = dpg.firstChild(); !sp.isNull(); sp = sp.nextSibling() )
			{
				QDomElement spe = sp.toElement();
				if (spe.tagName() == "Path.Fill")
					parseFillXML(spe, path, obState);
				else if (spe.tagName() == "Path.OpacityMask")
					parseOpacityXML(spe, path, obState);
				else if (spe.tagName() == "Path.Stroke")
					parseStrokeXML(spe, path, obState);
				else if (spe.tagName() == "Path.RenderTransform")
				{
					for(QDomNode obg = spe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
					{
						QDomElement eog = obg.toElement();
						if (eog.tagName() == "MatrixTransform")
						{
							QString trans = eog.attribute("Matrix", "1 0 0 1 0 0");
							trans.replace(",", " ");
							ScTextStream list(&trans, QIODevice::ReadOnly);
							double a, b, c, d, e, f;
							list >> a >> b >> c >> d >> e >> f;
							obState.transform = QTransform(a, b, c, d, e * conversionFactor, f * conversionFactor);
						}
					}
				}
				else if (spe.tagName() == "Path.Data")
				{
					parsePathDataXML(spe, obState);
					pathFromChild = true;
				}
				else if (spe.tagName() == "Path.Clip")
					parsePathDataXML(spe, obState, true);
			}
		}
		if (!pathFromChild)
		{
			QString pdata = dpg.attribute("Data");
			if (!pdata.startsWith("{"))
			{
				Coords.resize(0);
				Coords.svgInit();
				obState.currentPathClosed = Coords.parseSVG(pdata);
				Coords.scale(conversionFactor, conversionFactor);
				obState.currentPath = Coords.toQPainterPath(!obState.currentPathClosed);
				if (obState.currentPath.isEmpty())
					return retObj;
				obState.currentPath = obState.transform.map(obState.currentPath);
			}
			else
			{
				pdata.remove("{StaticResource ");
				pdata.remove("}");
				pdata = pdata.trimmed();
				if (pathResources.contains(pdata))
					obState.currentPath = obState.transform.map(pathResources[pdata]);
				if (obState.currentPath.isEmpty())
					return retObj;
			}
		}
		if (obState.currentPath.isEmpty())
			return retObj;
		retObj = createItem(dpg, obState);
		if (!obState.clipPath.isEmpty())
			retObj = addClip(retObj, obState);
	}
	else if (dpg.tagName() == "Canvas")
	{
		QList<PageItem*> GElements;
		for(QDomNode sp = dpg.firstChild(); !sp.isNull(); sp = sp.nextSibling() )
		{
			QDomElement spe = sp.toElement();
			if (spe.tagName() == "Canvas.RenderTransform")
			{
				for(QDomNode obg = spe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
				{
					QDomElement eog = obg.toElement();
					if (eog.tagName() == "MatrixTransform")
					{
						QString trans = eog.attribute("Matrix", "1 0 0 1 0 0");
						trans.replace(",", " ");
						ScTextStream list(&trans, QIODevice::ReadOnly);
						double a, b, c, d, e, f;
						list >> a >> b >> c >> d >> e >> f;
						obState.transform = QTransform(a, b, c, d, e * conversionFactor, f * conversionFactor);
					}
				}
			}
			else if ((spe.tagName() == "Path") || (spe.tagName() == "Glyphs") || (spe.tagName() == "Canvas"))
			{
				PageItem* ite = parseObjectXML(spe, path);
				if (ite != NULL)
					GElements.append(ite);
			}
			else if (spe.tagName() == "Canvas.OpacityMask")
				parseOpacityXML(spe, path, obState);
			else if (spe.tagName() == "Canvas.Resources")
			{
				for(QDomNode obg = spe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
				{
					QDomElement eog = obg.toElement();
					if (eog.tagName() == "ResourceDictionary")
					{
						if (eog.hasAttribute("Source"))
						{
							QString resFile = eog.attribute("Source", "");
							if (resFile.startsWith("/"))
							{
								resFile = resFile.mid(1);
								parseResourceFile(resFile);
							}
							else
							{
								if (!resFile.startsWith(path))
								{
									resFile = path + "/" + resFile;
									resFile = QDir::cleanPath(resFile);
								}
								parseResourceFile(resFile);
							}
						}
						else if (eog.hasChildNodes())
						{
							for(QDomElement dpgp = eog.firstChildElement(); !dpgp.isNull(); dpgp = dpgp.nextSiblingElement() )
							{
								if (dpgp.tagName() == "PathGeometry")
								{
									Coords.resize(0);
									Coords.svgInit();
									QString pdata = "";
									QString key = dpg.attribute("x:Key");
									if (dpg.hasAttribute("Figures"))
										pdata = dpg.attribute("Figures");
									else if (dpg.hasChildNodes())
										pdata = parsePathGeometryXML(dpg);
									if (!pdata.isEmpty())
									{
										bool currentPathClosed = Coords.parseSVG(pdata);
										Coords.scale(conversionFactor, conversionFactor);
										QPainterPath path = Coords.toQPainterPath(!currentPathClosed);
										if (dpg.attribute("FillRule") == "NonZero")
											path.setFillRule(Qt::WindingFill);
										pathResources.insert(key, path);
									}
								}
							}
						}
					}
				}
			}
			else if (spe.tagName() == "Canvas.Clip")
				parsePathDataXML(spe, obState, true);
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
			double gx = qMin(baseX, minx);
			double gy = qMin(baseY, miny);
			double gw = maxx - qMin(baseX, minx);
			double gh = maxy - qMin(baseY, miny);
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
				retObj->PoLine.scale(obState.transform.m11(), obState.transform.m22());
				FPoint wh = getMaxClipF(&retObj->PoLine);
				retObj->setWidthHeight(wh.x(),wh.y());
				m_Doc->AdjustItemSize(retObj, true);
				retObj->OldB2 = retObj->width();
				retObj->OldH2 = retObj->height();
				if ((obState.transform.dx() != 0.0) || (obState.transform.dy() != 0))
					retObj->moveBy((double)(obState.transform.dx()), (double)(obState.transform.dy()), true);
				if (obState.maskTyp != 0)
				{
					double xp = retObj->xPos() - m_Doc->currentPage()->xOffset();
					double yp = retObj->yPos() - m_Doc->currentPage()->yOffset();
					retObj->setMaskGradient(obState.gradientMask);
					retObj->setMaskVector(obState.maskStart.x() - xp, obState.maskStart.y() - yp, obState.maskEnd.x() - xp, obState.maskEnd.y() - yp, obState.maskFocus.x() - xp, obState.maskFocus.y() - yp, obState.maskScale, 0);
					retObj->setMaskType(obState.maskTyp);
				}
				if ((obState.transform.m12() != 0.0) || (obState.transform.m21() != 0))
				{
					QLineF line = obState.transform.map(QLineF(0.0, 0.0, 1.0, 0.0));
					retObj->setRotation(-line.angle(), true);
				}
				m_Doc->resizeGroupToContents(retObj);
				retObj->OwnPage = m_Doc->OnPage(retObj);
				m_Doc->GroupOnPage(retObj);
				m_Doc->Items->removeLast();
				if (!obState.clipPath.isEmpty())
					retObj = addClip(retObj, obState);
			}
		}
	}
	if (retObj != NULL)
	{
		if (!itemName.isEmpty())
		{
			linkTargets.insert(itemName, retObj);
			retObj->setItemName(itemName);
		}
		if ((!itemTarget.isEmpty()) && (dpg.tagName() != "Canvas"))
		{
			linkSources.insert(retObj, itemTarget);
			retObj->setIsAnnotation(true);
			retObj->annotation().setType(Annotation::Link);
			retObj->annotation().setZiel(m_Doc->currentPage()->pageNr());
			retObj->annotation().setAction("0 0");
			retObj->setTextFlowMode(PageItem::TextFlowDisabled);
		}
	}
	return retObj;
}

void XpsPlug::parseOpacityXML(QDomElement &spe, QString path, ObjState &obState)
{
	ObjState opaState;
	opaState.CurrColorFill = CommonStrings::None;
	opaState.fillOpacity = 0.0;
	opaState.fillGradientTyp = 0;
	opaState.gradientScale = 1.0;
	opaState.imagePath = "";
	opaState.patternName = "";
	parseFillXML(spe, path, opaState);
	if (opaState.fillGradientTyp != 0)
	{
		obState.gradientMask = opaState.currentGradient;
		obState.maskStart = opaState.gradientStart;
		obState.maskEnd = opaState.gradientEnd;
		obState.maskFocus = opaState.gradientFocus;
		obState.maskScale = opaState.gradientScale;
		obState.maskTyp = opaState.fillGradientTyp == 6 ? 1 : 3;
	}
	if (!opaState.patternName.isEmpty())
	{
		obState.patternMask = opaState.patternName;
		obState.maskTyp = 3;
	}
}

void XpsPlug::parseStrokeXML(QDomElement &spe, QString path, ObjState &obState)
{
	ObjState opaState;
	opaState.CurrColorFill = CommonStrings::None;
	opaState.fillOpacity = 0.0;
	opaState.fillGradientTyp = 0;
	opaState.gradientScale = 1.0;
	opaState.imagePath = "";
	opaState.patternName = "";
	parseFillXML(spe, path, opaState);
	if (opaState.fillGradientTyp != 0)
	{
		obState.gradientStroke = opaState.currentGradient;
		obState.strokeStart = opaState.gradientStart;
		obState.strokeEnd = opaState.gradientEnd;
		obState.strokeFocus = opaState.gradientFocus;
		obState.strokeScale = opaState.gradientScale;
		obState.strokeTyp = opaState.fillGradientTyp;
	}
	if (!opaState.patternName.isEmpty())
		obState.patternStroke = opaState.patternName;
}

void XpsPlug::parseFillXML(QDomElement &spe, QString path, ObjState &obState)
{
	for(QDomNode obg = spe.firstChild(); !obg.isNull(); obg = obg.nextSibling() )
	{
		QDomElement eog = obg.toElement();
		if (eog.tagName() == "SolidColorBrush")
			obState.CurrColorFill = handleColor(eog.attribute("Color", "#FF000000"), obState.fillOpacity);
		else if (eog.tagName() == "ImageBrush")
		{
			obState.imagePath = eog.attribute("ImageSource", "");
			if (obState.imagePath.startsWith("/"))
			{
				obState.imagePath = obState.imagePath.mid(1);
			}
			else
			{
				if (!obState.imagePath.startsWith(path))
				{
					obState.imagePath = path + "/" + obState.imagePath;
					obState.imagePath = QDir::cleanPath(obState.imagePath);
				}
			}
			if (eog.hasAttribute("Opacity"))
			{
				double opacity = eog.attribute("Opacity", "1.0").toDouble();
				obState.fillOpacity = 1.0 - ((1.0 - obState.fillOpacity) * opacity);
				obState.strokeOpacity = 1.0 - ((1.0 - obState.strokeOpacity) * opacity);
			}
			obState.itemType = 1;
		}
		else if (eog.tagName() == "LinearGradientBrush")
		{
			obState.currentGradient = VGradient(VGradient::linear);
			obState.currentGradient.clearStops();
			double gen_opacity = 1.0;
			if (eog.hasAttribute("Opacity"))
				gen_opacity = eog.attribute("Opacity", "1.0").toDouble();
			for(QDomNode gr = eog.firstChild(); !gr.isNull(); gr = gr.nextSibling() )
			{
				QDomElement grs = gr.toElement();
				if (grs.tagName() == "LinearGradientBrush.GradientStops")
				{
					for(QDomNode spo = grs.firstChild(); !spo.isNull(); spo = spo.nextSibling() )
					{
						QDomElement eo = spo.toElement();
						if (eo.tagName() == "GradientStop")
						{
							double opacity = 1.0;
							QString stopName = handleColor(eo.attribute("Color", "#FF000000"), opacity);
							double stopOffset = eo.attribute("Offset", "0.0").toDouble();
							const ScColor& gradC = m_Doc->PageColors[stopName];
							obState.currentGradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), stopOffset, 0.5, 1.0 - (opacity * gen_opacity), stopName, 100);
						}
					}
				}
			}
			QString vectS = eog.attribute("StartPoint", "0,0");
			vectS.replace(",", " ");
			ScTextStream list(&vectS, QIODevice::ReadOnly);
			double x, y;
			list >> x >> y;
			obState.gradientStart = QPointF(x * conversionFactor, y * conversionFactor);
			obState.gradientFocus = obState.gradientStart;
			QString vectE = eog.attribute("EndPoint", "0,0");
			vectE.replace(",", " ");
			ScTextStream listE(&vectE, QIODevice::ReadOnly);
			double x2, y2;
			listE >> x2 >> y2;
			obState.gradientEnd = QPointF(x2 * conversionFactor, y2 * conversionFactor);
			obState.gradientScale = 1.0;
			obState.fillGradientTyp = 6;
		}
		else if (eog.tagName() == "RadialGradientBrush")
		{
			obState.currentGradient = VGradient(VGradient::radial);
			obState.currentGradient.clearStops();
			double gen_opacity = 1.0;
			if (eog.hasAttribute("Opacity"))
				gen_opacity = eog.attribute("Opacity", "1.0").toDouble();
			for(QDomElement grs = eog.firstChildElement(); !grs.isNull(); grs = grs.nextSiblingElement())
			{
				if (grs.tagName() == "RadialGradientBrush.GradientStops")
				{
					for(QDomElement eo = grs.firstChildElement(); !eo.isNull(); eo = eo.nextSiblingElement() )
					{
						if (eo.tagName() == "GradientStop")
						{
							double opacity = 1.0;
							QString stopName = handleColor(eo.attribute("Color", "#FF000000"), opacity);
							double stopOffset = eo.attribute("Offset", "0.0").toDouble();
							const ScColor& gradC = m_Doc->PageColors[stopName];
							obState.currentGradient.addStop(ScColorEngine::getRGBColor(gradC, m_Doc), stopOffset, 0.5, 1.0 - (opacity * gen_opacity), stopName, 100);
						}
					}
				}
			}
			QString vectS = eog.attribute("Center", "0,0");
			vectS.replace(",", " ");
			ScTextStream list(&vectS, QIODevice::ReadOnly);
			double x, y;
			list >> x >> y;
			obState.gradientStart = QPointF(x * conversionFactor, y * conversionFactor);
			QString vectE = eog.attribute("GradientOrigin", "0,0");
			vectE.replace(",", " ");
			ScTextStream listE(&vectE, QIODevice::ReadOnly);
			double x2, y2;
			listE >> x2 >> y2;
			obState.gradientFocus = QPointF(x2 * conversionFactor, y2 * conversionFactor);
			double rx = eog.attribute("RadiusX", "1").toDouble() * conversionFactor;
			double ry = eog.attribute("RadiusY", "1").toDouble() * conversionFactor;
			obState.gradientEnd = QPointF(obState.gradientStart.x() + rx, obState.gradientStart.y());
			obState.gradientScale = rx / ry;
			obState.fillGradientTyp = 7;
		}
		else if (eog.tagName() == "VisualBrush")
		{
			QString viewb = eog.attribute("Viewbox", "0,0,1,1");
			viewb.replace(",", " ");
			ScTextStream listb(&viewb, QIODevice::ReadOnly);
			double Viewbox_x1, Viewbox_y1, Viewbox_x2, Viewbox_y2;
			listb >> Viewbox_x1 >> Viewbox_y1 >> Viewbox_x2 >> Viewbox_y2;
			QString vectE = eog.attribute("Viewport", "0,0,1,1");
			vectE.replace(",", " ");
			ScTextStream listE(&vectE, QIODevice::ReadOnly);
			double Viewport_x1, Viewport_y1, Viewport_x2, Viewport_y2;
			listE >> Viewport_x1 >> Viewport_y1 >> Viewport_x2 >> Viewport_y2;
			double vw = (Viewport_x2 * conversionFactor) / (Viewbox_x2 - Viewbox_x1);
			double vh = (Viewport_y2 * conversionFactor) / (Viewbox_y2 - Viewbox_y1);
			for(QDomElement grs = eog.firstChildElement(); !grs.isNull(); grs = grs.nextSiblingElement())
			{
				if (grs.tagName() == "VisualBrush.Visual")
				{
					for(QDomElement eo = grs.firstChildElement(); !eo.isNull(); eo = eo.nextSiblingElement() )
					{
						if ((eo.tagName() == "Path") || (eo.tagName() == "Glyphs") || (eo.tagName() == "Canvas"))
						{
							PageItem* item = parseObjectXML(eo, path);
							if (item != NULL)
							{
								m_Doc->SizeItem((item->width() / conversionFactor) * vw, (item->height() / conversionFactor) * vh, item, false, true, false);
								ScPattern pat = ScPattern();
								pat.setDoc(m_Doc);
								m_Doc->DoDrawing = true;
								QImage tmpImg = item->DrawObj_toImage(qMax(item->width(), item->height()));
								if (!tmpImg.isNull())
								{
									QImage retImg = QImage(qRound(Viewport_x2 * conversionFactor), qRound(Viewport_y2 * conversionFactor), QImage::Format_ARGB32_Premultiplied);
									retImg.fill( qRgba(0, 0, 0, 0) );
									QPainter p;
									p.begin(&retImg);
									p.drawImage(0, 0, tmpImg);
									p.end();
									pat.pattern = retImg;
									pat.xoffset = 0;
									pat.yoffset = 0;
									m_Doc->DoDrawing = false;
									pat.width = Viewport_x2 * conversionFactor;
									pat.height = Viewport_y2 * conversionFactor;
									item->gXpos = 0;
									item->gYpos = 0;
									item->setXYPos(item->gXpos, item->gYpos, true);
									pat.items.append(item);
									obState.patternName = QString("Pattern_from_XPS_%1").arg(m_Doc->docPatterns.count() + 1);
									m_Doc->addPattern(obState.patternName, pat);
									importedPatterns.append(obState.patternName);
								}
							}
						}
					}
				}
			}
		}
	}
}

void XpsPlug::parsePathDataXML(QDomElement &spe, ObjState &obState, bool forClip)
{
	Coords.resize(0);
	Coords.svgInit();
	QString svgString = "";
	bool windFill = false;
	for(QDomElement dpgp = spe.firstChildElement(); !dpgp.isNull(); dpgp = dpgp.nextSiblingElement() )
	{
		if (dpgp.tagName() == "PathGeometry")
			svgString += parsePathGeometryXML(dpgp);
		if (dpgp.attribute("FillRule") == "NonZero")
			windFill = true;
	}
	bool currentPathClosed = Coords.parseSVG(svgString);
	Coords.scale(conversionFactor, conversionFactor);
	if (forClip)
	{
		obState.clipPath = Coords.toQPainterPath(!currentPathClosed);
		if (windFill)
			obState.clipPath.setFillRule(Qt::WindingFill);
	}
	else
	{
		obState.currentPathClosed = currentPathClosed;
		obState.currentPath = Coords.toQPainterPath(!obState.currentPathClosed);
		if (windFill)
			obState.currentPath.setFillRule(Qt::WindingFill);
	}
}

QString XpsPlug::parsePathGeometryXML(QDomElement &spe)
{
	QString svgString = "";
	for(QDomElement dpg = spe.firstChildElement(); !dpg.isNull(); dpg = dpg.nextSiblingElement() )
	{
		if (dpg.tagName() == "PathFigure")
		{
			if (dpg.hasAttribute("StartPoint"))
				svgString += "M " + dpg.attribute("StartPoint") + " ";
			for(QDomElement dp = dpg.firstChildElement(); !dp.isNull(); dp = dp.nextSiblingElement() )
			{
				if (dp.tagName() == "PolyLineSegment")
					svgString += "L " + dp.attribute("Points") + " ";
				else if (dp.tagName() == "PolyQuadraticBezierSegment")
					svgString += "Q " + dp.attribute("Points") + " ";
				else if (dp.tagName() == "PolyBezierSegment")
					svgString += "C " + dp.attribute("Points") + " ";
				else if (dp.tagName() == "ArcSegment")
				{
					svgString += "A " + dp.attribute("Size") + " "  + dp.attribute("RotationAngle") + " ";
					if (dp.hasAttribute("IsLargeArc"))
					{
						if (dp.attribute("IsLargeArc").toLower() == "true")
							svgString += "1 ";
						else
							svgString += "0 ";
					}
					else
						svgString += "0 ";
					if (dp.hasAttribute("SweepDirection"))
					{
						if (dp.attribute("SweepDirection").toLower() == "counterclockwise")
							svgString += "0 ";
						else
							svgString += "1 ";
					}
					else
						svgString += "0 ";
					svgString += dp.attribute("Point") + " ";
				}
			}
			if (dpg.hasAttribute("IsClosed") && (dpg.attribute("IsClosed").toLower() == "true"))
				svgString += "Z ";
		}
	}
	return svgString;
}

void XpsPlug::parseResourceFile(QString resFile)
{
	QByteArray f;
	if (uz->read(resFile, f))
	{
		QDomDocument designMapDom;
		if(designMapDom.setContent(f))
		{
			QDomElement docElem = designMapDom.documentElement();
			for(QDomNode drawPag = docElem.firstChild(); !drawPag.isNull(); drawPag = drawPag.nextSibling() )
			{
				QDomElement dpg = drawPag.toElement();
				if (dpg.tagName() == "PathGeometry")
				{
					Coords.resize(0);
					Coords.svgInit();
					QString pdata = "";
					QString key = dpg.attribute("x:Key");
					if (dpg.hasAttribute("Figures"))
						pdata = dpg.attribute("Figures");
					else if (dpg.hasChildNodes())
						pdata = parsePathGeometryXML(dpg);
					if (!pdata.isEmpty())
					{
						bool currentPathClosed = Coords.parseSVG(pdata);
						Coords.scale(conversionFactor, conversionFactor);
						QPainterPath path = Coords.toQPainterPath(!currentPathClosed);
						if (dpg.attribute("FillRule") == "NonZero")
							path.setFillRule(Qt::WindingFill);
						pathResources.insert(key, path);
					}
				}
			}
		}
	}
}

void XpsPlug::resolveLinks()
{
	if (!linkSources.isEmpty())
	{
		QHash<PageItem*, QString>::iterator it;
		for (it = linkSources.begin(); it != linkSources.end(); ++it)
		{
			PageItem* linkS = it.key();
			QString target = it.value();
			if (linkTargets.contains(target))
			{
				PageItem* linkT = linkTargets[target];
				if (linkT != NULL)
				{
					int op = linkT->OwnPage;
					if (op > -1)
					{
						QTransform tf = linkT->getTransform();
						double xp = tf.dx() - m_Doc->Pages->at(op)->xOffset();
						double yp = tf.dy() - m_Doc->Pages->at(op)->yOffset();
						linkS->annotation().setZiel(linkT->OwnPage);
						linkS->annotation().setActionType(2);
						linkS->annotation().setAction(QString("%0 %1").arg(qRound(xp)).arg(qRound(m_Doc->Pages->at(op)->height() - yp)));
					}
				}
			}
		}
	}
}

PageItem* XpsPlug::addClip(PageItem* retObj, ObjState &obState)
{
	if (!obState.clipPath.isEmpty())
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
	}
	return retObj;
}

PageItem* XpsPlug::createItem(QDomElement &dpg, ObjState &obState)
{
	int z = -1;
	PageItem* retObj = NULL;
	if (!obState.currentPath.isEmpty())
	{
		if (obState.itemType == 0)
		{
			if (dpg.hasAttribute("FixedPage.NavigateUri"))
				z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, baseX, baseY, 10, 10, obState.LineW, obState.CurrColorFill, CommonStrings::None, true);
			else
			{
				if (!obState.currentPathClosed)
					z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX, baseY, 10, 10, obState.LineW, obState.CurrColorFill, obState.CurrColorStroke, true);
				else
					z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX, baseY, 10, 10, obState.LineW, obState.CurrColorFill, obState.CurrColorStroke, true);
			}
			retObj = m_Doc->Items->at(z);
			finishItem(retObj, obState);
			retObj = m_Doc->Items->takeAt(z);
		}
		else if (obState.itemType == 1)
		{
			z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, baseX, baseY, 10, 10, obState.LineW, obState.CurrColorFill, obState.CurrColorStroke, true);
			retObj = m_Doc->Items->at(z);
			finishItem(retObj, obState);
			if (!obState.imagePath.isEmpty())
			{
				QByteArray f;
				if (uz->read(obState.imagePath, f))
				{
					QFileInfo fi(obState.imagePath);
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_xps_XXXXXX." + fi.suffix());
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
			}
			retObj = m_Doc->Items->takeAt(z);
		}
	}
	return retObj;
}

void XpsPlug::finishItem(PageItem* item, ObjState &obState)
{
	item->PoLine.fromQPainterPath(obState.currentPath, !obState.currentPathClosed);
	FPoint wh = getMaxClipF(&item->PoLine);
	item->setWidthHeight(wh.x(),wh.y());
	m_Doc->AdjustItemSize(item, true);
	item->ClipEdited = true;
	item->FrameType = 3;
	item->setFillEvenOdd(false);
	item->OldB2 = item->width();
	item->OldH2 = item->height();
	item->updateClip();
	item->OwnPage = m_Doc->OnPage(item);
	item->ContourLine = item->PoLine.copy();
	item->setFillColor(obState.CurrColorFill);
	item->setFillTransparency(obState.fillOpacity);
	item->setLineColor(obState.CurrColorStroke);
	item->setLineTransparency(obState.strokeOpacity);
	item->setLineWidth(obState.LineW);
	item->setLineEnd(obState.CapStyle);
	item->setLineJoin(obState.JoinStyle);
	double xp = item->xPos() - m_Doc->currentPage()->xOffset();
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
	}
}

QString XpsPlug::handleColor(QString rgbColor, double &opacity)
{
	QString fNam = CommonStrings::None;
	QString alpha = "FF";
	if (rgbColor.startsWith( "sc#" ))
	{
		QColor c;
		rgbColor = rgbColor.remove(0,3);
		QStringList co = rgbColor.split(",");
		if (co.size() == 3)
		{
			rgbColor.replace(",", " ");
			ScTextStream list(&rgbColor, QIODevice::ReadOnly);
			double r, g, b;
			list >> r >> g >> b;
			c.setRgbF(r, g, b);
		}
		else if (co.size() == 4)
		{
			rgbColor.replace(",", " ");
			ScTextStream list(&rgbColor, QIODevice::ReadOnly);
			double r, g, b;
			list >> opacity >> r >> g >> b;
			c.setRgbF(r, g, b);
		}
		else
		{
			opacity = 0;
			return fNam;
		}
		ScColor tmp;
		tmp.fromQColor(c);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString newColorName = "FromXPS"+c.name();
		fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
		if (fNam == newColorName)
			importedColors.append(newColorName);
	}
	else if (rgbColor.startsWith( "#" ))
	{
		QColor c;
		if (rgbColor.length() == 9)
		{
			alpha = rgbColor.mid(1,2);
			bool ok;
			int hex = alpha.toInt(&ok, 16);
			opacity = 1.0 - (hex / 255.0);
			rgbColor = rgbColor.remove(1,2);
		}
		else
			opacity = 0;
		c.setNamedColor(rgbColor);
		ScColor tmp;
		tmp.fromQColor(c);
		tmp.setSpotColor(false);
		tmp.setRegistrationColor(false);
		QString newColorName = "FromXPS"+c.name();
		fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
		if (fNam == newColorName)
			importedColors.append(newColorName);
	}
	return fNam;
}

int XpsPlug::hex2int(char hex)
{
	QChar hexchar = QLatin1Char(hex);
	int v;
	if (hexchar.isDigit())
		v = hexchar.digitValue();
	else if (hexchar >= QLatin1Char('A') && hexchar <= QLatin1Char('F'))
		v = hexchar.cell() - 'A' + 10;
	else if (hexchar >= QLatin1Char('a') && hexchar <= QLatin1Char('f'))
		v = hexchar.cell() - 'a' + 10;
	else
		v = -1;
	return v;
}

bool XpsPlug::parseGUID( const QString &guidString, unsigned short guid[16])
{
	if (guidString.length() <= 35)
		return false;
	// Maps bytes to positions in guidString
	const static int indexes[] = {6, 4, 2, 0, 11, 9, 16, 14, 19, 21, 24, 26, 28, 30, 32, 34};
	for (int i = 0; i < 16; i++)
	{
		int hex1 = hex2int(guidString[indexes[i]].cell());
		int hex2 = hex2int(guidString[indexes[i]+1].cell());
		if ((hex1 < 0) || (hex2 < 0))
			return false;
		guid[i] = hex1 * 16 + hex2;
	}
	return true;
}


ScFace XpsPlug::loadFontByName(const QString &fileName)
{
	ScFace t;
	if (loadedFonts.contains(fileName))
		return loadedFonts[fileName];
	QByteArray fontData;
	if (!uz->read(fileName, fontData))
		return t;
	QTemporaryFile *tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_zip_XXXXXX.dat");
	if (tempImageFile == NULL)
		return t;
	tempImageFile->setAutoRemove(false);
	tempImageFile->open();
	QString fname = getLongPathName(tempImageFile->fileName());
	tempImageFile->close();
	delete tempImageFile;
	tempFontFiles.append(fname);
	QFileInfo fi(fileName);
	QString ext = fi.suffix().toLower();
	if (ext.startsWith("od"))
	{
		const QString baseName = fi.baseName();
		unsigned short guid[16];
		if (!parseGUID(baseName, guid))
			return t;
		else
		{
			if (fontData.length() < 32)
			{
				qDebug() << "Font file is too small";
				return t;
			}
			else
			{
				// Obfuscation - xor bytes in font binary with bytes from guid (font's filename)
				const static int mapping[] = {15, 14, 13, 12, 11, 10, 9, 8, 6, 7, 4, 5, 0, 1, 2, 3};
				for (int i = 0; i < 16; i++)
				{
					fontData[i] = fontData[i] ^ guid[mapping[i]];
					fontData[i+16] = fontData[i+16] ^ guid[mapping[i]];
				}
			}
		}
	}
	QFile ft(fname);
	if (ft.open(QIODevice::WriteOnly))
	{
		ft.write(fontData);
		ft.close();
		t = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.LoadScalableFont(fname);
		loadedFonts.insert(fileName, t);
		return t;
	}
	return t;
}
