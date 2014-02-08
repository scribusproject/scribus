/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Mon Dez 30 2013
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

#include "importpages.h"
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
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"

#include "ui/customfdialog.h"
#include "ui/missing.h"
#include "ui/multiprogressdialog.h"
#include "ui/propertiespalette.h"

#include "third_party/zip/scribus_zip.h"

extern SCRIBUS_API ScribusQApp * ScQApp;

PagesPlug::PagesPlug(ScribusDoc* doc, int flags)
{
	tmpSel = new Selection(this, false);
	m_Doc = doc;
	importerFlags = flags;
	interactive = (flags & LoadSavePlugin::lfInteractive);
	progressDialog = NULL;
	uz = NULL;
}

QImage PagesPlug::readThumbnail(QString fName)
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
	if (uz->contains("QuickLook/Thumbnail.jpg"))
	{
		QByteArray im;
		if (!uz->read("QuickLook/Thumbnail.jpg", im))
			return QImage();
		tmp = QImage::fromData(im);
		QDomDocument designMapDom;
		QByteArray f;
		int xs = 0;
		int ys = 0;
		if (uz->contains("index.xml"))
		{
			if (uz->read("index.xml", f))
			{
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
		}
		tmp.setText("XSize", QString("%1").arg(xs));
		tmp.setText("YSize", QString("%1").arg(ys));
	}
/*	else
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
	}*/
	uz->close();
	delete uz;
	return tmp;
}

bool PagesPlug::import(QString fNameIn, const TransactionSettings& trSettings, int flags, bool showProgress)
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

PagesPlug::~PagesPlug()
{
	if (progressDialog)
		delete progressDialog;
	delete tmpSel;
}

bool PagesPlug::convert(QString fn)
{
	bool retVal = true;
	importedColors.clear();
	importedPatterns.clear();
	m_StyleSheets.clear();
	m_currentStyleSheet = "";
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
		return false;
	}
	retVal = false;
	if (uz->contains("index.xml"))
		retVal = parseDocReference("index.xml", false);
	else if (uz->contains("index.xml.gz"))
		retVal = parseDocReference("index.xml.gz", true);
	uz->close();
	delete uz;
	if (progressDialog)
		progressDialog->close();
	return retVal;
}

bool PagesPlug::parseDocReference(QString designMap, bool compressed)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(designMap, f))
		return false;
	if (compressed)
	{
		QTemporaryFile *tmpFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_zip_XXXXXX.dat");
		if (tmpFile == NULL)
			return false;
		tmpFile->open();
		QString fname = getLongPathName(tmpFile->fileName());
		tmpFile->write(f);
		tmpFile->close();
		if (!ScGzFile::readFromFile(fname, f))
		{
			delete tmpFile;
			return false;
		}
		delete tmpFile;
	}
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(f, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return false;
	}
	papersize = "Custom";
	QDomElement docElem = designMapDom.documentElement();
	for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "sl:slprint-info")
		{
			docWidth = drawPag.attribute("sl:page-width", "0").toDouble();
			docHeight = drawPag.attribute("sl:page-height", "0").toDouble();
			for(QDomElement sp = drawPag.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
			{
				if (sp.tagName() == "sf:page-margins")
				{
					topMargin = sp.attribute("sf:top", "0").toDouble();
					leftMargin = sp.attribute("sf:left", "0").toDouble();
					rightMargin = sp.attribute("sf:right", "0").toDouble();
					bottomMargin = sp.attribute("sf:bottom", "0").toDouble();
					pgCols = m_Doc->PageSp;
					pgGap = m_Doc->PageSpa;
				}
			}
			if (importerFlags & LoadSavePlugin::lfCreateDoc)
			{
				m_Doc->setPage(docWidth, docHeight, topMargin, leftMargin, rightMargin, bottomMargin, pgCols, pgGap, false, false);
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
			baseX = m_Doc->currentPage()->xOffset();
			baseY = m_Doc->currentPage()->yOffset();
		}
		else if (drawPag.tagName() == "sl:section-prototypes")
		{
			for(QDomElement sp = drawPag.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
			{
				if (sp.tagName() == "sl:prototype")
				{
					QString pageNam = sp.attribute("sl:name");
					if (!pageNam.isEmpty())
					{
						if (importerFlags & LoadSavePlugin::lfCreateDoc)
						{
							m_Doc->setMasterPageMode(true);
							ScPage *oldCur = m_Doc->currentPage();
							ScPage *addedPage = m_Doc->addMasterPage(mpagecount, pageNam);
							m_Doc->setCurrentPage(addedPage);
							addedPage->MPageNam = "";
							m_Doc->view()->addPage(mpagecount, true);
							baseX = addedPage->xOffset();
							baseY = addedPage->yOffset();
							mpagecount++;
							for(QDomElement spd = sp.firstChildElement(); !spd.isNull(); spd = spd.nextSiblingElement() )
							{
								if (spd.tagName() == "sl:stylesheet")
								{
									parseStyleSheets(spd);
									m_currentStyleSheet = spd.attribute("sfa:ID");
								}
								else if (spd.tagName() == "sl:drawables")
								{
									for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
									{
										if (spe.tagName() == "sl:page-group")
											parsePageReference(spe);
									}
								}
							}
							m_Doc->setCurrentPage(oldCur);
							m_Doc->setMasterPageMode(false);
						}
					}
				}
			}
		}
		else if (drawPag.tagName() == "sl:stylesheet")
		{
			parseStyleSheets(drawPag);
			m_currentStyleSheet = drawPag.attribute("sfa:ID");
		}
		else if (drawPag.tagName() == "sl:drawables")
		{
			for(QDomElement sp = drawPag.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
			{
				if (sp.tagName() == "sl:page-group")
				{
					qDebug() << "Reading Page" << sp.attribute("sl:page");
					if (importerFlags & LoadSavePlugin::lfCreateDoc)
					{
						if (firstPage)
						{
							topMargin = m_Doc->marginsVal().Top;
							leftMargin = m_Doc->marginsVal().Left;
							rightMargin = m_Doc->marginsVal().Right;
							bottomMargin = m_Doc->marginsVal().Bottom;
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
					parsePageReference(sp);
				}
			}
		}
		else if (drawPag.tagName() == "sf:text-storage")
		{
			for(QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "sf:stylesheet-ref")
				{
					m_currentStyleSheet = spf.attribute("sfa:IDREF");
				}
				else if (spf.tagName() == "sf:text-body")
				{
					int txPage = 0;
					StoryText itemText;
					itemText.clear();
					itemText.setDoc(m_Doc);
					int posC = 0;
					QString pStyle = CommonStrings::DefaultParagraphStyle;
					ParagraphStyle newStyle;
					newStyle.setDefaultStyle(false);
					newStyle.setParent(pStyle);
					ParagraphStyle ttx = m_Doc->paragraphStyle(pStyle);
					CharStyle nstyle = ttx.charStyle();
					newStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
					newStyle.setLineSpacing(nstyle.fontSize() / 10.0);
					itemText.setDefaultStyle(newStyle);
					for(QDomElement spg = spf.firstChildElement(); !spg.isNull(); spg = spg.nextSiblingElement() )
					{
						if (spg.tagName() == "sf:container-hint")
						{
							txPage = spg.attribute("sf:page-index").toInt();
						}
						else if (spg.tagName() == "sf:p")
						{
							ParagraphStyle tmpStyle = newStyle;
							if (!spg.attribute("sf:style").isEmpty())
							{
								if (m_Doc->styleExists(spg.attribute("sf:style")))
									tmpStyle = m_Doc->paragraphStyle(spg.attribute("sf:style"));
								else
								{
									tmpStyle.setName(spg.attribute("sf:style"));
									applyParagraphAttrs(tmpStyle, tmpStyle.charStyle(), spg.attribute("sf:style"));
									StyleSet<ParagraphStyle>tmp;
									tmp.create(tmpStyle);
									m_Doc->redefineStyles(tmp, false);
								}
							}
							int totalCount = 0;
							if (spg.hasChildNodes())
							{
								for(QDomElement sph = spg.firstChildElement(); !sph.isNull(); sph = sph.nextSiblingElement() )
								{
									totalCount += sph.text().length();
									if (sph.tagName() == "sf:container-hint")
									{
										txPage = sph.attribute("sf:page-index").toInt();
									}
								}
							}
							int count = spg.text().length();
							QString tgText = spg.text();
							tgText = tgText.left(count - totalCount);
							if (tgText.length() > 0)
							{
								itemText.insertChars(posC, tgText);
								itemText.applyStyle(posC, tmpStyle);
								itemText.applyCharStyle(posC, count - totalCount, tmpStyle.charStyle());
								posC = itemText.length();
							}
							if (spg.hasChildNodes())
							{
								for(QDomElement sph = spg.firstChildElement(); !sph.isNull(); sph = sph.nextSiblingElement() )
								{
									if (sph.tagName() == "sf:span")
									{
										CharStyle tmpCStyle = tmpStyle.charStyle();
										applyCharAttrs(tmpCStyle, sph.attribute("sf:style"));
										int count = sph.text().length();
										if (count > 0)
										{
											itemText.insertChars(posC, sph.text());
											itemText.applyStyle(posC, tmpStyle);
											itemText.applyCharStyle(posC, count, tmpCStyle);
											posC = itemText.length();
										}
									}
								}
							}
							itemText.insertChars(posC, SpecialChars::PARSEP);
							itemText.applyStyle(posC, tmpStyle);
							posC = itemText.length();
						}
					}
				}
			}
		}
	}
	return true;
}

void PagesPlug::parseStyleSheets(QDomElement &drawPag)
{
	QString sheetName = drawPag.attribute("sfa:ID");
	StyleSheet styleSH;
	for(QDomElement sp = drawPag.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
	{
		if ((sp.tagName() == "sf:styles") || (sp.tagName() == "sf:anon-styles"))
		{
			for(QDomElement spd = sp.firstChildElement(); !spd.isNull(); spd = spd.nextSiblingElement() )
			{
				if (spd.tagName() == "sf:characterstyle")
				{
					ChrStyle currStyle;
					for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
					{
						if (spe.tagName() == "sf:property-map")
						{
							for(QDomElement spf = spe.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
							{
								if (spf.tagName() == "sf:fontSize")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:number")
											currStyle.fontSize = AttributeValue(spg.attribute("sfa:number", "1"));
									}
								}
								else if (spf.tagName() == "sf:fontName")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:string")
											currStyle.fontName = AttributeValue(spg.attribute("sfa:string"));
									}
								}
								else if (spf.tagName() == "sf:fontColor")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:color")
										{
											QString type = spg.attribute("xsi:type");
											if (type == "sfa:calibrated-white-color-type")
											{
											//	currStyle.fontColor = AttributeValue("White");
												QColor c;
												double w = spg.attribute("sfa:w", "1").toDouble();
												c.setRgbF(w, w, w);
												ScColor tmp;
												tmp.fromQColor(c);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+c.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.fontColor = AttributeValue(fNam);
											}
											else if (type == "sfa:calibrated-rgb-color-type")
											{
												QColor c;
												double r = spg.attribute("sfa:r", "1").toDouble();
												double g = spg.attribute("sfa:g", "1").toDouble();
												double b = spg.attribute("sfa:b", "1").toDouble();
												c.setRgbF(r, g, b);
												ScColor tmp;
												tmp.fromQColor(c);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+c.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.fontColor = AttributeValue(fNam);
											}
											else if (type == "sfa:device-cmyk-color-type")
											{
												int c = qRound(spg.attribute("sfa:c", "1").toDouble() * 255);
												int m = qRound(spg.attribute("sfa:m", "1").toDouble() * 255);
												int y = qRound(spg.attribute("sfa:y", "1").toDouble() * 255);
												int k = qRound(spg.attribute("sfa:k", "1").toDouble() * 255);
												ScColor tmp;
												tmp.setColor(c, m, y, k);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+tmp.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.fontColor = AttributeValue(fNam);
											}
										}
									}
								}
							}
						}
					}
					if (!spd.attribute("sf:parent-ident").isEmpty())
						currStyle.parentStyle = AttributeValue(spd.attribute("sf:parent-ident"));
					QString id = spd.attribute("sfa:ID");
					QString i = spd.attribute("sf:ident");
					QString i2 = spd.attribute("sf:name");
					if (!id.isEmpty())
						styleSH.m_chrStyles.insert(id, currStyle);
					else if (!i.isEmpty())
						styleSH.m_chrStyles.insert(i, currStyle);
					else if (!i2.isEmpty())
						styleSH.m_chrStyles.insert(i2, currStyle);
				}
				if (spd.tagName() == "sf:paragraphstyle")
				{
					ParStyle currStyle;
					for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
					{
						if (spe.tagName() == "sf:property-map")
						{
							for(QDomElement spf = spe.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
							{
								if (spf.tagName() == "sf:fontSize")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:number")
											currStyle.fontSize = AttributeValue(spg.attribute("sfa:number", "1"));
									}
								}
								else if (spf.tagName() == "sf:fontName")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:string")
											currStyle.fontName = AttributeValue(spg.attribute("sfa:string"));
									}
								}
								else if (spf.tagName() == "sf:alignment")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:number")
											currStyle.justification = AttributeValue(spg.attribute("sfa:number", "1"));
									}
								}
								else if (spf.tagName() == "sf:fontColor")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:color")
										{
											QString type = spg.attribute("xsi:type");
											if (type == "sfa:calibrated-white-color-type")
											{
											//	currStyle.fontColor = AttributeValue("White");
												QColor c;
												double w = spg.attribute("sfa:w", "1").toDouble();
												c.setRgbF(w, w, w);
												ScColor tmp;
												tmp.fromQColor(c);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+c.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.fontColor = AttributeValue(fNam);
											}
											else if (type == "sfa:calibrated-rgb-color-type")
											{
												QColor c;
												double r = spg.attribute("sfa:r", "1").toDouble();
												double g = spg.attribute("sfa:g", "1").toDouble();
												double b = spg.attribute("sfa:b", "1").toDouble();
												c.setRgbF(r, g, b);
												ScColor tmp;
												tmp.fromQColor(c);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+c.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.fontColor = AttributeValue(fNam);
											}
											else if (type == "sfa:device-cmyk-color-type")
											{
												int c = qRound(spg.attribute("sfa:c", "1").toDouble() * 255);
												int m = qRound(spg.attribute("sfa:m", "1").toDouble() * 255);
												int y = qRound(spg.attribute("sfa:y", "1").toDouble() * 255);
												int k = qRound(spg.attribute("sfa:k", "1").toDouble() * 255);
												ScColor tmp;
												tmp.setColor(c, m, y, k);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+tmp.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.fontColor = AttributeValue(fNam);
											}
										}
									}
								}
							}
						}
					}
					if (!spd.attribute("sf:parent-ident").isEmpty())
						currStyle.parentStyle = AttributeValue(spd.attribute("sf:parent-ident"));
					QString id = spd.attribute("sfa:ID");
					QString i = spd.attribute("sf:ident");
					QString i2 = spd.attribute("sf:name");
					if (!id.isEmpty())
						styleSH.m_parStyles.insert(id, currStyle);
					else if (!i.isEmpty())
						styleSH.m_parStyles.insert(i, currStyle);
					else if (!i2.isEmpty())
						styleSH.m_parStyles.insert(i2, currStyle);
				}
				else if (spd.tagName() == "sf:layoutstyle")
				{
					LayoutStyle currStyle;
					for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
					{
						if (spe.tagName() == "sf:property-map")
						{
							for(QDomElement spf = spe.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
							{
								if (spf.tagName() == "sf:padding")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:padding")
										{
											currStyle.Extra = AttributeValue(spg.attribute("sf:left", "0"));
											currStyle.RExtra = AttributeValue(spg.attribute("sf:right", "0"));
											currStyle.TExtra = AttributeValue(spg.attribute("sf:top", "0"));
											currStyle.BExtra = AttributeValue(spg.attribute("sf:bottom", "0"));
										}
									}
								}
							}
						}
					}
					if (!spd.attribute("sf:parent-ident").isEmpty())
						currStyle.parentStyle = AttributeValue(spd.attribute("sf:parent-ident"));
					QString id = spd.attribute("sfa:ID");
					QString i = spd.attribute("sf:ident");
					QString i2 = spd.attribute("sf:name");
					if (!id.isEmpty())
						styleSH.m_layoutStyles.insert(id, currStyle);
					else if (!i.isEmpty())
						styleSH.m_layoutStyles.insert(i, currStyle);
					else if (!i2.isEmpty())
						styleSH.m_layoutStyles.insert(i2, currStyle);
				}
				else if (spd.tagName() == "sf:graphic-style")
				{
					ObjStyle currStyle;
					for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
					{
						if (spe.tagName() == "sf:property-map")
						{
							for(QDomElement spf = spe.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
							{
								if (spf.tagName() == "sf:stroke")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:stroke")
										{
											currStyle.LineW = AttributeValue(spg.attribute("sf:width", "1"));
											if (spg.hasAttribute("sf:cap"))
												currStyle.CapStyle = AttributeValue(spg.attribute("sf:cap"));
											if (spg.hasAttribute("sf:join"))
												currStyle.JoinStyle = AttributeValue(spg.attribute("sf:join"));
											QDomElement sph = spg.firstChildElement();
											if (!sph.isNull())
											{
												if (sph.tagName() == "sf:color")
												{
													QString type = sph.attribute("xsi:type");
													currStyle.strokeOpacity  = AttributeValue(QString("%1").arg(1.0 - sph.attribute("sfa:a", "1").toDouble()));
													if (type == "sfa:calibrated-white-color-type")
													{
													//	currStyle.CurrColorStroke = AttributeValue("White");
														QColor c;
														double w = sph.attribute("sfa:w", "1").toDouble();
														c.setRgbF(w, w, w);
														ScColor tmp;
														tmp.fromQColor(c);
														tmp.setSpotColor(false);
														tmp.setRegistrationColor(false);
														QString newColorName = "FromPages"+c.name();
														QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
														if (fNam == newColorName)
															importedColors.append(newColorName);
														currStyle.CurrColorStroke = AttributeValue(fNam);
													}
													else if (type == "sfa:calibrated-rgb-color-type")
													{
														QColor c;
														double r = sph.attribute("sfa:r", "1").toDouble();
														double g = sph.attribute("sfa:g", "1").toDouble();
														double b = sph.attribute("sfa:b", "1").toDouble();
														c.setRgbF(r, g, b);
														ScColor tmp;
														tmp.fromQColor(c);
														tmp.setSpotColor(false);
														tmp.setRegistrationColor(false);
														QString newColorName = "FromPages"+c.name();
														QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
														if (fNam == newColorName)
															importedColors.append(newColorName);
														currStyle.CurrColorStroke = AttributeValue(fNam);
													}
													else if (type == "sfa:device-cmyk-color-type")
													{
														int c = qRound(sph.attribute("sfa:c", "1").toDouble() * 255);
														int m = qRound(sph.attribute("sfa:m", "1").toDouble() * 255);
														int y = qRound(sph.attribute("sfa:y", "1").toDouble() * 255);
														int k = qRound(sph.attribute("sfa:k", "1").toDouble() * 255);
														ScColor tmp;
														tmp.setColor(c, m, y, k);
														tmp.setSpotColor(false);
														tmp.setRegistrationColor(false);
														QString newColorName = "FromPages"+tmp.name();
														QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
														if (fNam == newColorName)
															importedColors.append(newColorName);
														currStyle.CurrColorStroke = AttributeValue(fNam);
													}
												}
											}
										}
									}
								}
								else if (spf.tagName() == "sf:fill")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:color")
										{
											QString type = spg.attribute("xsi:type");
											currStyle.fillOpacity = AttributeValue(QString("%1").arg(1.0 - spg.attribute("sfa:a", "1").toDouble()));
											if (type == "sfa:calibrated-white-color-type")
											{
											//	currStyle.CurrColorFill = AttributeValue("White");
												QColor c;
												double w = spg.attribute("sfa:w", "1").toDouble();
												c.setRgbF(w, w, w);
												ScColor tmp;
												tmp.fromQColor(c);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+c.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.CurrColorFill = AttributeValue(fNam);
											}
											else if (type == "sfa:calibrated-rgb-color-type")
											{
												QColor c;
												double r = spg.attribute("sfa:r", "1").toDouble();
												double g = spg.attribute("sfa:g", "1").toDouble();
												double b = spg.attribute("sfa:b", "1").toDouble();
												c.setRgbF(r, g, b);
												ScColor tmp;
												tmp.fromQColor(c);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+c.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.CurrColorFill = AttributeValue(fNam);
											}
											else if (type == "sfa:device-cmyk-color-type")
											{
												int c = qRound(spg.attribute("sfa:c", "1").toDouble() * 255);
												int m = qRound(spg.attribute("sfa:m", "1").toDouble() * 255);
												int y = qRound(spg.attribute("sfa:y", "1").toDouble() * 255);
												int k = qRound(spg.attribute("sfa:k", "1").toDouble() * 255);
												ScColor tmp;
												tmp.setColor(c, m, y, k);
												tmp.setSpotColor(false);
												tmp.setRegistrationColor(false);
												QString newColorName = "FromPages"+tmp.name();
												QString fNam = m_Doc->PageColors.tryAddColor(newColorName, tmp);
												if (fNam == newColorName)
													importedColors.append(newColorName);
												currStyle.CurrColorFill = AttributeValue(fNam);
											}
										}
										else if (spg.tagName() == "sf:null")
											currStyle.CurrColorFill = AttributeValue(CommonStrings::None);
									}
								}
								else if (spf.tagName() == "sf:opacity")
								{
									QDomElement spg = spf.firstChildElement();
									if (!spg.isNull())
									{
										if (spg.tagName() == "sf:number")
											currStyle.opacity = AttributeValue(QString("%1").arg(1.0 - spg.attribute("sfa:number", "1").toDouble()));
									}
								}
							}
						}
					}
					if (!spd.attribute("sf:parent-ident").isEmpty())
						currStyle.parentStyle = AttributeValue(spd.attribute("sf:parent-ident"));
					QString id = spd.attribute("sfa:ID");
					QString i = spd.attribute("sf:ident");
					QString i2 = spd.attribute("sf:name");
					if (!id.isEmpty())
						styleSH.m_objStyles.insert(id, currStyle);
					else if (!i.isEmpty())
						styleSH.m_objStyles.insert(i, currStyle);
					else if (!i2.isEmpty())
						styleSH.m_objStyles.insert(i2, currStyle);
				}
			}
		}
	}
	m_StyleSheets.insert(sheetName, styleSH);
}

void PagesPlug::parsePageReference(QDomElement &drawPag)
{
	for(QDomElement draw = drawPag.firstChildElement(); !draw.isNull(); draw = draw.nextSiblingElement() )
	{
		PageItem* retObj = parseObjReference(draw);
		if (retObj != NULL)
		{
			m_Doc->Items->append(retObj);
			Elements.append(retObj);
		}
	}
}

PageItem* PagesPlug::parseObjReference(QDomElement &draw)
{
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
	obState.xPos = 0.0;
	obState.yPos = 0.0;
	obState.width = 0.0;
	obState.height = 0.0;
	obState.rotation = 0.0;
	obState.styleRef = "";
	obState.layoutStyleRef = "";
	StoryText itemText;
	itemText.clear();
	itemText.setDoc(m_Doc);
	int z = -1;
	PageItem *retObj = NULL;
	if (draw.tagName() == "sf:group")
	{
		QList<PageItem*> GElements;
		for(QDomElement spd = draw.firstChildElement(); !spd.isNull(); spd = spd.nextSiblingElement() )
		{
			if (spd.tagName() == "sf:geometry")
			{
				obState.rotation = spd.attribute("sf:angle", "0").toDouble();
				for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
				{
					if (spe.tagName() == "sf:position")
					{
						obState.xPos = spe.attribute("sfa:x", "0").toDouble();
						obState.yPos = spe.attribute("sfa:y", "0").toDouble();
					}
					else if (spe.tagName() == "sf:size")
					{
						obState.width = spe.attribute("sfa:w", "0").toDouble();
						obState.height = spe.attribute("sfa:h", "0").toDouble();
					}
				}
			}
			else if ((spd.tagName() == "sf:media") || (spd.tagName() == "sf:drawable-shape") || (spd.tagName() == "sf:group"))
			{
				PageItem* ite = parseObjReference(spd);
				if (ite != NULL)
					GElements.append(ite);
			}
		}
		if (GElements.count() > 0)
		{
			QTransform m;
			m.translate(obState.width / 2.0, obState.height / 2.0);
			m.rotate(-obState.rotation);
			m.translate(-obState.width / 2.0, -obState.height / 2.0);
			QPolygonF po;
			po.append(QPointF(0, 0));
			po.append(QPointF(obState.width, 0));
			po.append(QPointF(obState.width, obState.height));
			po.append(QPointF(0, obState.height));
			po = m.map(po);
			QRectF br = po.boundingRect();
			po.translate(-br.x(), -br.y());
			po.translate(obState.xPos, obState.yPos);
			for (int ep = 0; ep < GElements.count(); ++ep)
			{
				GElements.at(ep)->moveBy(po[0].x(), po[0].y(), true);
			}
			z = m_Doc->itemAdd(PageItem::Group, PageItem::Rectangle, baseX + po[0].x(), baseY + po[0].y(), obState.width, obState.height, 0, CommonStrings::None, CommonStrings::None, true);
			retObj = m_Doc->Items->at(z);
			retObj->ClipEdited = true;
			retObj->FrameType = 3;
			retObj->setFillEvenOdd(false);
			retObj->OldB2 = retObj->width();
			retObj->OldH2 = retObj->height();
			retObj->updateClip();
			m_Doc->groupObjectsToItem(retObj, GElements);
			retObj->setRotation(-obState.rotation, true);
			retObj->OwnPage = m_Doc->OnPage(retObj);
			m_Doc->GroupOnPage(retObj);
			m_Doc->Items->removeLast();
		}
	}
	else if ((draw.tagName() == "sf:media") || (draw.tagName() == "sf:drawable-shape"))
	{
		for(QDomElement spd = draw.firstChildElement(); !spd.isNull(); spd = spd.nextSiblingElement() )
		{
			if (spd.tagName() == "sf:geometry")
			{
				obState.rotation = spd.attribute("sf:angle", "0").toDouble();
				for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
				{
					if (spe.tagName() == "sf:position")
					{
						obState.xPos = spe.attribute("sfa:x", "0").toDouble();
						obState.yPos = spe.attribute("sfa:y", "0").toDouble();
					}
					else if (spe.tagName() == "sf:size")
					{
						obState.width = spe.attribute("sfa:w", "0").toDouble();
						obState.height = spe.attribute("sfa:h", "0").toDouble();
					}
				}
			}
			else if (spd.tagName() == "sf:style")
			{
				for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
				{
					if (spe.tagName() == "sf:graphic-style-ref")
					{
						obState.styleRef = spe.attribute("sfa:IDREF");
					}
				}
			}
			else if (spd.tagName() == "sf:path")
			{
				for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
				{
					if ((spe.tagName() == "sf:bezier-path") || (spe.tagName() == "sf:editable-bezier-path"))
					{
						for(QDomElement spf = spe.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
						{
							if (spf.tagName() == "sf:bezier")
							{
								QString pdata = spf.attribute("sfa:path");
								if (!pdata.isEmpty())
								{
									Coords.resize(0);
									Coords.svgInit();
									obState.currentPathClosed = Coords.parseSVG(pdata);
									obState.currentPath = Coords.toQPainterPath(!obState.currentPathClosed);
								}
							}
						}
					}
				}
			}
			else if (spd.tagName() == "sf:wrap")
			{
				for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
				{
					if (spe.tagName() == "sf:path")
					{
						QString pdata = spe.attribute("sfa:path");
						if (!pdata.isEmpty())
						{
							Coords.resize(0);
							Coords.svgInit();
							obState.currentPathClosed = Coords.parseSVG(pdata);
							obState.clipPath = Coords.toQPainterPath(!obState.currentPathClosed);
						}
					}
				}
			}
			else if (spd.tagName() == "sf:content")
			{
				for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
				{
					if (spe.tagName() == "sf:image-media")
					{
						for(QDomElement spf = spe.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
						{
							if (spf.tagName() == "sf:filtered-image")
							{
								for(QDomElement spg = spf.firstChildElement(); !spg.isNull(); spg = spg.nextSiblingElement() )
								{
									if (spg.tagName() == "sf:unfiltered")
									{
										for(QDomElement sph = spg.firstChildElement(); !sph.isNull(); sph = sph.nextSiblingElement() )
										{
											if (sph.tagName() == "sf:data")
											{
												obState.imagePath = sph.attribute("sf:path");
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else if (spd.tagName() == "sf:text")
			{
				obState.layoutStyleRef = spd.attribute("sf:layoutstyle");
				for(QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
				{
					if (spe.tagName() == "sf:text-storage")
					{
						for(QDomElement spf = spe.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
						{
							if (spf.tagName() == "sf:text-body")
							{
								int posC = 0;
								QString pStyle = CommonStrings::DefaultParagraphStyle;
								ParagraphStyle newStyle;
								newStyle.setDefaultStyle(false);
								newStyle.setParent(pStyle);
								ParagraphStyle ttx = m_Doc->paragraphStyle(pStyle);
								CharStyle nstyle = ttx.charStyle();
								newStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
								newStyle.setLineSpacing(nstyle.fontSize() / 10.0);
								itemText.setDefaultStyle(newStyle);
								for(QDomElement spg = spf.firstChildElement(); !spg.isNull(); spg = spg.nextSiblingElement() )
								{
									if (spg.tagName() == "sf:p")
									{
										ParagraphStyle tmpStyle = newStyle;
										if (!spg.attribute("sf:style").isEmpty())
										{
											if (m_Doc->styleExists(spg.attribute("sf:style")))
												tmpStyle = m_Doc->paragraphStyle(spg.attribute("sf:style"));
											else
											{
												tmpStyle.setName(spg.attribute("sf:style"));
												applyParagraphAttrs(tmpStyle, tmpStyle.charStyle(), spg.attribute("sf:style"));
												StyleSet<ParagraphStyle>tmp;
												tmp.create(tmpStyle);
												m_Doc->redefineStyles(tmp, false);
											}
										}
										int totalCount = 0;
										if (spg.hasChildNodes())
										{
											for(QDomElement sph = spg.firstChildElement(); !sph.isNull(); sph = sph.nextSiblingElement() )
											{
												totalCount += sph.text().length();
											}
										}
										int count = spg.text().length();
										QString tgText = spg.text();
										tgText = tgText.left(count - totalCount);
										if (tgText.length() > 0)
										{
											itemText.insertChars(posC, tgText);
											itemText.applyStyle(posC, tmpStyle);
											itemText.applyCharStyle(posC, count - totalCount, tmpStyle.charStyle());
											posC = itemText.length();
										}
										if (spg.hasChildNodes())
										{
											for(QDomElement sph = spg.firstChildElement(); !sph.isNull(); sph = sph.nextSiblingElement() )
											{
												if (sph.tagName() == "sf:span")
												{
													CharStyle tmpCStyle = tmpStyle.charStyle();
													applyCharAttrs(tmpCStyle, sph.attribute("sf:style"));
													int count = sph.text().length();
													if (count > 0)
													{
														itemText.insertChars(posC, sph.text());
														itemText.applyStyle(posC, tmpStyle);
														itemText.applyCharStyle(posC, count, tmpCStyle);
														posC = itemText.length();
													}
												}
											}
										}
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
		if (draw.tagName() == "sf:media")
		{
			QTransform m;
			m.translate(obState.width / 2.0, obState.height / 2.0);
			m.rotate(-obState.rotation);
			m.translate(-obState.width / 2.0, -obState.height / 2.0);
			QPolygonF po;
			po.append(QPointF(0, 0));
			po.append(QPointF(obState.width, 0));
			po.append(QPointF(obState.width, obState.height));
			po.append(QPointF(0, obState.height));
			po = m.map(po);
			QRectF br = po.boundingRect();
			po.translate(-br.x(), -br.y());
			po.translate(obState.xPos, obState.yPos);
			z = m_Doc->itemAdd(PageItem::ImageFrame, PageItem::Rectangle, baseX + po[0].x(), baseY + po[0].y(), obState.width, obState.height, obState.LineW, obState.CurrColorFill, obState.CurrColorStroke, true);
			retObj = m_Doc->Items->at(z);
			if (!obState.clipPath.isEmpty())
			{
				QRectF br2 = obState.clipPath.boundingRect();
				QTransform m;
				m.translate(br2.x(), br2.y());
				m.translate(br2.width() / 2.0, br2.height() / 2.0);
				m.rotate(obState.rotation);
				m.translate(-br2.width() / 2.0, -br2.height() / 2.0);
				obState.clipPath = m.map(obState.clipPath);
				FPointArray pa;
				pa.fromQPainterPath(obState.clipPath, true);
				QRectF clipRect = obState.clipPath.boundingRect();
				FPoint tp2(clipRect.left(), clipRect.top());
				pa.translate(-tp2.x(), -tp2.y());
				pa.translate(-(clipRect.width() - obState.width) / 2.0, -(clipRect.height() - obState.height) / 2.0);
				retObj->ContourLine = pa;
				retObj->setTextFlowMode(PageItem::TextFlowUsesContourLine);
			}
			retObj->setRotation(-obState.rotation, true);
			finishItem(retObj, obState);
			retObj = m_Doc->Items->takeAt(z);
			if (!obState.imagePath.isEmpty())
			{
				QByteArray f;
				if (uz->read(obState.imagePath, f))
				{
					QFileInfo fi(obState.imagePath);
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_pages_XXXXXX." + fi.suffix());
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
		}
		else if (draw.tagName() == "sf:drawable-shape")
		{
			QTransform m;
			m.translate(obState.width / 2.0, obState.height / 2.0);
			m.rotate(-obState.rotation);
			m.translate(-obState.width / 2.0, -obState.height / 2.0);
			QPolygonF po;
			po.append(QPointF(0, 0));
			po.append(QPointF(obState.width, 0));
			po.append(QPointF(obState.width, obState.height));
			po.append(QPointF(0, obState.height));
			po = m.map(po);
			QRectF br = po.boundingRect();
			po.translate(-br.x(), -br.y());
			po.translate(obState.xPos, obState.yPos);
			if (itemText.length() > 0)
				z = m_Doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, baseX + po[0].x(), baseY + po[0].y(), obState.width, obState.height, obState.LineW, obState.CurrColorFill, obState.CurrColorStroke, true);
			else if (obState.currentPathClosed)
				z = m_Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, baseX + po[0].x(), baseY + po[0].y(), obState.width, obState.height, obState.LineW, obState.CurrColorFill, obState.CurrColorStroke, true);
			else
				z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, baseX + po[0].x(), baseY + po[0].y(), obState.width, obState.height, obState.LineW, obState.CurrColorFill, obState.CurrColorStroke, true);
			retObj = m_Doc->Items->at(z);
			if (!obState.currentPath.isEmpty())
			{
				retObj->ClipEdited = true;
				retObj->FrameType = 3;
				FPointArray pa;
				pa.fromQPainterPath(obState.currentPath, !obState.currentPathClosed);
				retObj->PoLine = pa;
			}
			else
				retObj->SetRectFrame();
			retObj->setRotation(-obState.rotation, true);
			if (itemText.length() > 0)
			{
				retObj->itemText.append(itemText);
			}
			finishItem(retObj, obState);
			retObj = m_Doc->Items->takeAt(z);
		}
	}
	return retObj;
}

PageItem* PagesPlug::addClip(PageItem* retObj, ObjState &obState)
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

void PagesPlug::applyParagraphAttrs(ParagraphStyle &newStyle, CharStyle &tmpCStyle, QString pAttrs)
{
	if (!m_StyleSheets.contains(m_currentStyleSheet))
		return;
	StyleSheet currSH = m_StyleSheets[m_currentStyleSheet];
	if (currSH.m_parStyles.contains(pAttrs))
	{
		ParStyle actStyle;
		ParStyle currStyle = currSH.m_parStyles[pAttrs];
		QStringList parents;
		while (currStyle.parentStyle.valid)
		{
			if (currSH.m_parStyles.contains(currStyle.parentStyle.value))
			{
				parents.prepend(currStyle.parentStyle.value);
				currStyle = currSH.m_parStyles[currStyle.parentStyle.value];
			}
			else
				break;
		}
		parents.append(pAttrs);
		if (!parents.isEmpty())
		{
			for (int p = 0; p < parents.count(); p++)
			{
				currStyle = currSH.m_parStyles[parents[p]];
				if (currStyle.fontName.valid)
					actStyle.fontName = AttributeValue(currStyle.fontName.value);
				if (currStyle.fontSize.valid)
					actStyle.fontSize = AttributeValue(currStyle.fontSize.value);
				if (currStyle.fontColor.valid)
					actStyle.fontColor = AttributeValue(currStyle.fontColor.value);
				if (currStyle.justification.valid)
					actStyle.justification = AttributeValue(currStyle.justification.value);
			}
		}
		if (actStyle.fontName.valid)
			qDebug() << "Font" << actStyle.fontName.value;
	//		textMarginLeft = actStyle.fontName.value;
		if (actStyle.fontSize.valid)
			tmpCStyle.setFontSize(actStyle.fontSize.value.toInt() * 10);
		if (actStyle.fontColor.valid)
			tmpCStyle.setFillColor(actStyle.fontColor.value);
		if (actStyle.justification.valid)
		{
			if (actStyle.justification.value == "0")
				newStyle.setAlignment(ParagraphStyle::Leftaligned);
			else if (actStyle.justification.value == "1")
				newStyle.setAlignment(ParagraphStyle::Rightaligned);
			else if (actStyle.justification.value == "2")
				newStyle.setAlignment(ParagraphStyle::Centered);
			else if (actStyle.justification.value == "3")
				newStyle.setAlignment(ParagraphStyle::Justified);
			else if (actStyle.justification.value == "4")
				newStyle.setAlignment(ParagraphStyle::Extended);
		}
	}
}

void PagesPlug::applyCharAttrs(CharStyle &tmpCStyle, QString pAttrs)
{
	if (!m_StyleSheets.contains(m_currentStyleSheet))
		return;
	StyleSheet currSH = m_StyleSheets[m_currentStyleSheet];
	if (currSH.m_chrStyles.contains(pAttrs))
	{
		ChrStyle actStyle;
		ChrStyle currStyle = currSH.m_chrStyles[pAttrs];
		QStringList parents;
		while (currStyle.parentStyle.valid)
		{
			if (currSH.m_chrStyles.contains(currStyle.parentStyle.value))
			{
				parents.prepend(currStyle.parentStyle.value);
				currStyle = currSH.m_chrStyles[currStyle.parentStyle.value];
			}
			else
				break;
		}
		parents.append(pAttrs);
		if (!parents.isEmpty())
		{
			for (int p = 0; p < parents.count(); p++)
			{
				currStyle = currSH.m_chrStyles[parents[p]];
				if (currStyle.fontName.valid)
					actStyle.fontName = AttributeValue(currStyle.fontName.value);
				if (currStyle.fontSize.valid)
					actStyle.fontSize = AttributeValue(currStyle.fontSize.value);
				if (currStyle.fontColor.valid)
					actStyle.fontColor = AttributeValue(currStyle.fontColor.value);
			}
		}
		if (actStyle.fontName.valid)
			qDebug() << "Font" << actStyle.fontName.value;
	//	if (actStyle.fontName.valid)
	//		textMarginLeft = actStyle.fontName.value;
		if (actStyle.fontSize.valid)
			tmpCStyle.setFontSize(actStyle.fontSize.value.toInt() * 10);
		if (actStyle.fontColor.valid)
			tmpCStyle.setFillColor(actStyle.fontColor.value);
	}
}

void PagesPlug::finishItem(PageItem* item, ObjState &obState)
{
	item->ClipEdited = true;
	item->FrameType = 3;
	item->setFillEvenOdd(false);
	item->OldB2 = item->width();
	item->OldH2 = item->height();
	item->updateClip();
	item->OwnPage = m_Doc->OnPage(item);
	if (m_StyleSheets.contains(m_currentStyleSheet))
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
	}
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
