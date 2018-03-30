/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scribus.cpp  -  description
                             -------------------
    begin                : Fre Apr  6 21:09:31 CEST 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QAction>
#include <QApplication>
#include <QByteArray>
#include <QCloseEvent>
#include <QColor>
#include <QColorDialog>
#include <QCursor>
#include <QDesktopWidget>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QEventLoop>
#include <QFileDialog>
#include <QFrame>
#include <QFont>
#include <QHBoxLayout>
#include <QIcon>
#include <QInputDialog>
#include <QKeyEvent>
#include <QKeySequence>
#include <QLabel>
#include <QList>
#include <QLocale>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMultiMap>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
//<<QML testing
//#include <QQuickView>
//#include <QQmlEngine>
//#include <QQmlComponent>
//#include <QQuickItem>
//#include <QQmlProperty>
//>>
#include <QRegExp>
#include <QScopedPointer>
#include <QStyleFactory>
#include <QTableWidget>
#include <QTextCodec>
#include <QTranslator>
#include <QWheelEvent>

#ifdef DEBUG_LOAD_TIMES
#include <QDebug>
#include <QTime>
#include <sys/times.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "scconfig.h"

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <iostream>
#include <sstream>
#include <signal.h>
#include <string>


#include "actionmanager.h"
#include "appmodehelper.h"
#include "appmodes.h"
#include "canvasmode.h"
#include "canvasmode_imageimport.h"
#include "commonstrings.h"
#include "desaxe/digester.h"
#include "desaxe/saxXML.h"
#include "desaxe/simple_actions.h"
#include "docinfo.h"
#include "documentchecker.h"
#include "documentinformation.h"
#include "fileloader.h"
#include "filewatcher.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "gtgettext.h"
#include "hyphenator.h"
#include "iconmanager.h"
#include "langmgr.h"
#include "loadsaveplugin.h"
#include "marks.h"
#include "nfttemplate.h"
#include "notesstyles.h"
#include "pageitem_group.h"
#include "pageitem_imageframe.h"
#include "pageitem_latexframe.h"
#include "pageitem_noteframe.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "pagesize.h"
#include "pdflib.h"
#include "pdfoptions.h"
#include "pluginmanager.h"
#include "plugins/formatidlist.h"
#include "plugins/formatidlist.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "pslib.h"
#include "resourcecollection.h"
#include "sccolorengine.h"
#include "scgtplugin.h"
#include "scimagecachemanager.h"
#include "scmimedata.h"
#include "scpage.h"
#include "scpaths.h"
#include "scprintengine_ps.h"
#include "scraction.h"
#include "scribusXml.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribuswin.h"
#include "selection.h"
#include "serializer.h"
#include "styleoptions.h"
#include "tocgenerator.h"
#include "ui/about.h"
#include "ui/aboutplugins.h"
#include "ui/adjustcmsdialog.h"
#include "ui/aligndistribute.h"
#include "ui/annot.h"
#include "ui/annota.h"
#include "ui/applytemplatedialog.h"
#include "ui/arrowchooser.h"
#include "ui/autoform.h"
#include "ui/basepointwidget.h"
#include "ui/bookmarkpalette.h"
#include "ui/charselect.h"
#include "ui/checkDocument.h"
#include "ui/collectforoutput_ui.h"
#include "ui/colorcombo.h"
#include "ui/contextmenu.h"
#include "ui/copypagetomasterpagedialog.h"
#include "ui/cpalette.h"
#include "ui/customfdialog.h"
#include "ui/delpages.h"
#include "ui/downloadspalette.h"
#include "ui/edittoolbar.h"
#include "ui/effectsdialog.h"
#include "ui/filetoolbar.h"
#include "ui/fontcombo.h"
#include "ui/guidemanager.h"
#include "ui/helpbrowser.h"
#include "ui/hruler.h"
#include "ui/imageinfodialog.h"
#include "ui/inlinepalette.h"
#include "ui/insertaframe.h"
#include "ui/inspage.h"
#include "ui/javadocs.h"
#include "ui/layers.h"
#include "ui/loremipsum.h"
#include "ui/marginwidget.h"
#include "ui/mark2item.h"
#include "ui/mark2mark.h"
#include "ui/markanchor.h"
#include "ui/markinsert.h"
#include "ui/marknote.h"
#include "ui/marksmanager.h"
#include "ui/markvariabletext.h"
#include "ui/mergedoc.h"
#include "ui/modetoolbar.h"
#include "ui/movepage.h"
#include "ui/multipleduplicate.h"
#include "ui/newfile.h"
#include "ui/newtemp.h"
#include "ui/nftdialog.h"
#include "ui/nftwidget.h"
#include "ui/nodeeditpalette.h"
#include "ui/notesstyleseditor.h"
#include "ui/outlinepalette.h"
#include "ui/pageitemattributes.h"
#include "ui/pagelayout.h"
#include "ui/pagepalette.h"
#include "ui/pagepropertiesdialog.h"
#include "ui/pageselector.h"
#include "ui/colorsandfills.h"
#include "ui/pdfexportdialog.h"
#include "ui/pdftoolbar.h"
#include "ui/picstatus.h"
#include "ui/polygonwidget.h"
#include "ui/preferencesdialog.h"
#include "ui/preview.h"
#include "ui/printdialog.h"
#include "ui/propertiespalette.h"
#include "ui/propertiespalette_image.h"
#include "ui/propertiespalette_line.h"
#include "ui/propertiespalette_shape.h"
#include "ui/propertiespalette_text.h"
#include "ui/propertiespalette_xyz.h"
#include "ui/query.h"
#include "ui/recoverdialog.h"
#include "ui/replacecolors.h"
#include "ui/resourcemanager.h"
#include "ui/sccombobox.h"
#include "ui/scfilewidget.h"
#include "ui/scmessagebox.h"
#include "ui/scmwmenumanager.h"
#include "ui/scrapbookpalette.h"
#include "ui/scrspinbox.h"
#include "ui/search.h"
#include "ui/selectobjects.h"
#include "ui/smcellstyle.h"
#include "ui/smlinestyle.h"
#include "ui/smtablestyle.h"
#include "ui/smtextstyles.h"
#include "ui/splash.h"
#include "ui/storyeditor.h"
#include "ui/stylemanager.h"
#include "ui/symbolpalette.h"
#include "ui/tabmanager.h"
#include "ui/textpalette.h"
#include "ui/transformdialog.h"
#include "ui/transparencypalette.h"
#include "ui/viewtoolbar.h"
#include "ui/vruler.h"
#include "ui_nftdialog.h"
#include "undogui.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "urllauncher.h"
#include "util.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_ghostscript.h"
#include "util_math.h"

#ifdef HAVE_SVNVERSION
	#include "svnversion.h"
#endif

#ifdef HAVE_OSG
	#include "ui/osgeditor.h"
	#include <osgDB/ReaderWriter>
	#include <osgDB/PluginQuery>
#endif

#if defined(_WIN32)
#include "scdocoutput_ps2.h"
#include "scprintengine_gdi.h"
#endif

#include "sclimits.h"

using namespace std;

bool previewDinUse;
bool printDinUse;

QString DocDir;


extern ScribusQApp* ScQApp;
extern bool emergencyActivated;


ScribusMainWindow::ScribusMainWindow()
{
	actionManager=0;
	appModeHelper=0;
	scrMenuMgr=0;
	m_prefsManager=0;
	m_formatsManager=0;
	resourceManager=0;
	UrlLauncher::instance();
	m_mainWindowStatusLabel=0;
	m_ScriptRunning = 0;
#ifdef Q_OS_MAC
	//commenting this out until this is resolved :https://bugreports.qt.io/browse/QTBUG-44565
	//ScQApp->setAttribute(Qt::AA_DontShowIconsInMenus);
	//noIcon = IconManager::instance()->loadPixmap("noicon.png");
#endif
	m_doc = 0;
	m_tocGenerator = 0;
}

/*
 * retval 0 - ok, 1 - no fonts, ...
 */
int ScribusMainWindow::initScMW(bool primaryMainWindow)
{
	int retVal=0;
	qsrand(1234);
	QByteArray stylesheet;
	if (loadRawText(ScPaths::preferencesDir() + "/stylesheet.css", stylesheet))
	{
		qApp->setStyleSheet(QString(stylesheet));
	}

	qApp->setLayoutDirection(QLocale(ScCore->getGuiLanguage()).textDirection());
	previewDinUse = false;
	printDinUse = false;
	internalCopy = false;
	internalCopyBuffer = "";
	m_doc = new ScribusDoc();
	m_doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_doc->setPage(100, 100, 0, 0, 0, 0, 0, 0, false, false);
	m_doc->addPage(0);
	m_doc->setGUI(false, this, 0);
	CurrStED = NULL;
	QString scribusTitle(tr("Scribus") + " " + QString(VERSION));
#if defined(HAVE_SVNVERSION) && defined(SVNVERSION)
	if (QString(VERSION).contains("svn", Qt::CaseInsensitive))
		scribusTitle.append(QString(" (r%1)").arg(SVNVERSION));
#endif
	setWindowTitle(scribusTitle);
	setAttribute(Qt::WA_KeyCompression, false);
	setAttribute(Qt::WA_InputMethodEnabled, true);
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon.png"));
	setObjectName("MainWindow");
	scrActionGroups.clear();
	scrActions.clear();
	scrRecentFileActions.clear();
	scrRecentPasteActions.clear();
	scrWindowsActions.clear();
	scrLayersActions.clear();
	scrScrapActions.clear();
	actionManager = new ActionManager(this);
	appModeHelper = new AppModeHelper();
	appModeHelper->setup(actionManager, &scrActions, &scrRecentFileActions, &scrWindowsActions, &scrScrapActions, &scrLayersActions, &scrRecentPasteActions);
	scrMenuMgr = new ScMWMenuManager(menuBar(), actionManager);
	m_prefsManager = PrefsManager::instance();
	m_formatsManager = FormatsManager::instance();
	m_objectSpecificUndo = false;

	m_undoManager = UndoManager::instance();
	PrefsContext *undoPrefs = m_prefsManager->prefsFile->getContext("undo");
	m_undoManager->setUndoEnabled(undoPrefs->getBool("enabled", true));
	m_tocGenerator = new TOCGenerator();
	m_marksCount = 0;

	initDefaultValues();
	initStatusBar();

	qApp->processEvents();

	actionManager->init(this);

	initMdiArea();
	initMenuBar();
	createMenuBar();
	initToolBars();
	ScCore->pluginManager->setupPluginActions(this);
	ScCore->pluginManager->enableOnlyStartupPluginActions(this);
	ScCore->pluginManager->languageChange();

	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Applying User Shortcuts") );
	m_prefsManager->applyLoadedShortCuts();
	initKeyboardShortcuts();

	resize(610, 600);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(newActWin(QMdiSubWindow *)));
	//Connect windows cascade and tile actions to the workspace after its created. Only depends on mdiArea created.
	connect( scrActions["windowsCascade"], SIGNAL(triggered()) , mdiArea, SLOT(cascadeSubWindows()) );
	connect( scrActions["windowsTile"], SIGNAL(triggered()) , mdiArea, SLOT(tileSubWindows()) );
	initPalettes();

	m_prefsManager->setupMainWindow(this);

	viewToolBar->previewQualitySwitcher->setCurrentIndex(m_prefsManager->appPrefs.itemToolPrefs.imageLowResType);
	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Initializing Story Editor") );
	storyEditor = new StoryEditor(this);

	DocDir = m_prefsManager->documentDir();

	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Initializing Languages") );
	LanguageManager::instance();

	initHyphenator();
//	QString preLang(m_prefsManager->appPrefs.hyphPrefs.Language);
//	if (!LanguageManager::instance()->getHyphFilename( preLang ).isEmpty() )
//		m_prefsManager->appPrefs.hyphPrefs.Language = preLang;
	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Reading Scrapbook") );
	initScrapbook();

	scrActions["helpTooltips"]->setChecked(m_prefsManager->appPrefs.displayPrefs.showToolTips);
	scrActions["showMouseCoordinates"]->setChecked(m_prefsManager->appPrefs.displayPrefs.showMouseCoordinates);
	scrActions["stickyTools"]->setChecked(m_prefsManager->appPrefs.uiPrefs.stickyTools);
	ToggleTips();
	ToggleMouseTips();
	propertiesPalette->setFontSize();
	textPalette->setFontSize();
	if (scrActions["SaveAsDocumentTemplate"])
		scrActions["SaveAsDocumentTemplate"]->setEnabled(false);

	connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), this, SLOT(removeRecentFromWatcher(QString)));
	connect(ClipB, SIGNAL(dataChanged()), this, SLOT(ClipChange()));
	setAcceptDrops(true);
	QCoreApplication::instance()->installEventFilter(this);
	scrActions["toolsSelect"]->setChecked(true);
	ColorSetManager csm;
	csm.findPaletteLocations();
	csm.findPalettes();
	csm.findUserPalettes();
	QString Cpfad = QDir::toNativeSeparators(ScPaths::applicationDataDir())+"DefaultColors.xml";
	QFile fc(Cpfad);
	if (fc.exists())
		csm.loadPalette(Cpfad, m_doc, m_prefsManager->appPrefs.colorPrefs.DColors, m_prefsManager->appPrefs.defaultGradients, m_prefsManager->appPrefs.defaultPatterns, false);
	else
	{
		if (m_prefsManager->appPrefs.colorPrefs.DColorSet != "Scribus Small")
		{
			QStringList CustomColorSets = csm.userPaletteNames();
			if (CustomColorSets.contains(m_prefsManager->appPrefs.colorPrefs.DColorSet))
				Cpfad = csm.userPaletteFileFromName(m_prefsManager->appPrefs.colorPrefs.DColorSet);
			else
				Cpfad = csm.paletteFileFromName(m_prefsManager->appPrefs.colorPrefs.DColorSet);
			if (!Cpfad.isEmpty())
				csm.loadPalette(Cpfad, m_doc, m_prefsManager->appPrefs.colorPrefs.DColors, m_prefsManager->appPrefs.defaultGradients, m_prefsManager->appPrefs.defaultPatterns, false);
		}
	}
	appModeHelper->setStartupActionsEnabled(false);

	setStyleSheet();

	return retVal;
}


ScribusMainWindow::~ScribusMainWindow()
{
	if (actionManager)
		delete actionManager;
	if (appModeHelper)
		delete appModeHelper;
	if (m_doc)
		delete m_doc;
	if (m_tocGenerator)
		delete m_tocGenerator;
}

void ScribusMainWindow::addScToolBar(ScToolBar *tb, QString name)
{
	if (!scrToolBars.contains(name))
		scrToolBars.insert(name, tb);
	addToolBar(tb);
}

void ScribusMainWindow::initToolBars()
{
	fileToolBar = new FileToolBar(this);
	editToolBar = new EditToolBar(this);
	UndoWidget* uWidget = new UndoWidget(editToolBar, "uWidget");
	m_undoManager->registerGui(uWidget);
	modeToolBar = new ModeToolBar(this);
	pdfToolBar = new PDFToolBar(this);
	viewToolBar = new ViewToolBar(this);

	addScToolBar(fileToolBar, fileToolBar->objectName());
	addScToolBar(editToolBar, editToolBar->objectName());
	addScToolBar(modeToolBar, modeToolBar->objectName());
	addScToolBar(pdfToolBar, pdfToolBar->objectName());
	addScToolBar(viewToolBar, viewToolBar->objectName());
	connect(modeToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarTools"], SLOT(setChecked(bool)));
	connect(scrActions["toolsToolbarPDF"], SIGNAL(toggled(bool)), pdfToolBar, SLOT(setVisible(bool)));
	connect(pdfToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarPDF"], SLOT(setChecked(bool)));
	connect(scrActions["toolsToolbarTools"], SIGNAL(toggled(bool)), modeToolBar, SLOT(setVisible(bool)) );
	connect(viewToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarView"], SLOT(setChecked(bool)));
	connect(scrActions["toolsToolbarView"], SIGNAL(toggled(bool)), viewToolBar, SLOT(setVisible(bool)) );
}

void ScribusMainWindow::setStyleSheet()
{
	QByteArray stylesheet;
	if (loadRawText(ScPaths::instance().libDir() + "scribus.css", stylesheet))
	{
		QString downArrow(IconManager::instance()->pathForIcon("16/go-down.png"));
		QByteArray da;
		da.append(downArrow);
		stylesheet.replace("___downArrow___", da);
		QString toolbararrow(IconManager::instance()->pathForIcon("stylesheet/down_arrow.png"));
		QByteArray tba;
		tba.append(toolbararrow);
		stylesheet.replace("___tb_menu_arrow___", tba);
	}

	layerMenu->setStyleSheet(stylesheet);
	unitSwitcher->setStyleSheet(stylesheet);
	zoomDefaultToolbarButton->setStyleSheet(stylesheet);
	zoomInToolbarButton->setStyleSheet(stylesheet);
	zoomOutToolbarButton->setStyleSheet(stylesheet);
	zoomSpinBox->setStyleSheet(stylesheet);

	fileToolBar->setStyleSheet(stylesheet);
	editToolBar->setStyleSheet(stylesheet);
	modeToolBar->setStyleSheet(stylesheet);
	pdfToolBar->setStyleSheet(stylesheet);
	viewToolBar->setStyleSheet(stylesheet);
}


void ScribusMainWindow::initDefaultValues()
{
	HaveDoc = false;
	view = NULL;
	doc = NULL;
	m_DocNr = 1;
	m_PrinterUsed = false;
	PDef.Pname = "";
	PDef.Dname = "";
	PDef.Command = "";
	m_keyrep = false;
	m__arrowKeyDown = false;
	ClipB = QApplication::clipboard();
	for (int i=0; i<PAL_MAX ; ++i)
		m_palettesStatus[i] = false;
	for (int i=0; i<GS_MAX ; ++i)
		m_guidesStatus[i] = false;
#ifdef HAVE_OSG
	QStringList supportedExts;
	supportedExts << "osg" << "dxf" << "flt" << "ive" << "geo" << "sta" << "stl" << "logo" << "3ds" << "ac" << "obj";
	QStringList realSupportedExts;
	QMap<QString, QString> formats;
	osgDB::FileNameList plugins = osgDB::listAllAvailablePlugins();
	for(osgDB::FileNameList::iterator itr = plugins.begin(); itr != plugins.end(); ++itr)
	{
		osgDB::ReaderWriterInfoList infoList;
		if (QString::fromStdString(*itr).contains("qfont"))
			continue;
		if (osgDB::queryPlugin(*itr, infoList))
		{
			for(osgDB::ReaderWriterInfoList::iterator rwi_itr = infoList.begin(); rwi_itr != infoList.end(); ++rwi_itr)
			{
				osgDB::ReaderWriterInfo& info = *(*rwi_itr);
				osgDB::ReaderWriter::FormatDescriptionMap::iterator fdm_itr;
				for(fdm_itr = info.extensions.begin(); fdm_itr != info.extensions.end(); ++fdm_itr)
				{
					if (supportedExts.contains(QString::fromStdString(fdm_itr->first)))
						formats.insert("*." + QString::fromStdString(fdm_itr->first) + " *." + QString::fromStdString(fdm_itr->first).toUpper(), QString::fromStdString(fdm_itr->second) + " (*." + QString::fromStdString(fdm_itr->first) + " *." + QString::fromStdString(fdm_itr->first).toUpper() + ")");
				}
			}
		}
	}
	realSupportedExts = formats.keys();
	QString docexts = realSupportedExts.join(" ");
	QStringList longList = formats.values();
	QString longDesc = longList.join(";;") + ";;";
	m_osgFilterString = tr("All Supported Formats (%1);;%2All Files (*)").arg(docexts).arg(longDesc);
#endif
}


void ScribusMainWindow::initKeyboardShortcuts()
{
	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrActions.begin(); it!=scrActions.end(); ++it )
	{
		if ((ScrAction*)(it.value())!=NULL)
		{
			QString accelerator = it.value()->shortcut().toString();
			m_prefsManager->setKeyEntry(it.key(), it.value()->cleanMenuText(), accelerator,0);
		}
		//else
		//	qDebug() << it.key();
		//qDebug() << QString("|-\n|%1||%2||%3").arg(it.key()).arg(it.value()->cleanMenuText()).arg(QString(it.data()->accel()));
	}
}


void ScribusMainWindow::initPalettes()
{
	//CB TODO hide the publicly available members of some palettes
	// these must be filtered too as they take control of the palettes events
	outlinePalette = new OutlinePalette(this);
	outlinePalette->setMainWindow(this);
	connect( scrActions["toolsOutline"], SIGNAL(toggled(bool)) , outlinePalette, SLOT(setPaletteShown(bool)) );
	connect( outlinePalette, SIGNAL(paletteShown(bool)), scrActions["toolsOutline"], SLOT(setChecked(bool)));

	propertiesPalette = new PropertiesPalette(this);
	propertiesPalette->setMainWindow(this);
	connect( scrActions["toolsProperties"], SIGNAL(toggled(bool)) , propertiesPalette, SLOT(setPaletteShown(bool)) );
	connect( propertiesPalette, SIGNAL(paletteShown(bool)), scrActions["toolsProperties"], SLOT(setChecked(bool)));
	emit UpdateRequest(reqDefFontListUpdate);
	propertiesPalette->installEventFilter(this);

	textPalette = new TextPalette(this);
	textPalette->setMainWindow(this);
	connect( scrActions["toolsText"], SIGNAL(toggled(bool)) , textPalette, SLOT(setPaletteShown(bool)) );
	connect( textPalette, SIGNAL(paletteShown(bool)), scrActions["toolsText"], SLOT(setChecked(bool)));
	emit UpdateRequest(reqDefFontListUpdate);
	textPalette->installEventFilter(this);
	nodePalette = new NodePalette(this);
	nodePalette->installEventFilter(this);
	layerPalette = new LayerPalette(this);
	guidePalette = new GuideManager(this);
	charPalette = new CharSelect(this);
	connect( scrActions["toolsLayers"], SIGNAL(toggled(bool)) , layerPalette, SLOT(setPaletteShown(bool)) );
	connect( layerPalette, SIGNAL(paletteShown(bool)), scrActions["toolsLayers"], SLOT(setChecked(bool)));
	layerPalette->installEventFilter(this);
	layerPalette->Table->installEventFilter(this);
	scrapbookPalette = new Biblio(this);
	connect( scrActions["toolsScrapbook"], SIGNAL(toggled(bool)) , scrapbookPalette, SLOT(setPaletteShown(bool)) );
	connect( scrapbookPalette, SIGNAL(paletteShown(bool)), scrActions["toolsScrapbook"], SLOT(setChecked(bool)));
	connect( scrapbookPalette, SIGNAL(pasteToActualPage(QString)), this, SLOT(pasteFromScrapbook(QString)));
	connect( scrapbookPalette, SIGNAL(scrapbookListChanged()), this, SLOT(rebuildScrapbookMenu()));
	scrapbookPalette->installEventFilter(this);
	pagePalette = new PagePalette(this);
	connect( scrActions["toolsPages"], SIGNAL(toggled(bool)) , pagePalette, SLOT(setPaletteShown(bool)) );
	connect( pagePalette, SIGNAL(paletteShown(bool)), scrActions["toolsPages"], SLOT(setChecked(bool)));
	pagePalette->installEventFilter(this);
	bookmarkPalette = new BookPalette(this);
	connect( scrActions["toolsBookmarks"], SIGNAL(toggled(bool)) , bookmarkPalette, SLOT(setPaletteShown(bool)) );
	connect( bookmarkPalette, SIGNAL(paletteShown(bool)), scrActions["toolsBookmarks"], SLOT(setChecked(bool)));
	bookmarkPalette->installEventFilter(this);
	downloadsPalette = new DownloadsPalette(this);
	connect( scrActions["toolsDownloads"], SIGNAL(toggled(bool)) , downloadsPalette, SLOT(setPaletteShown(bool)) );
	connect( downloadsPalette, SIGNAL(paletteShown(bool)), scrActions["toolsDownloads"], SLOT(setChecked(bool)));
	downloadsPalette->installEventFilter(this);
	connect( scrActions["toolsMeasurements"], SIGNAL(toggledData(bool, int)) , this, SLOT(setAppModeByToggle(bool, int)) );
	docCheckerPalette = new CheckDocument(this, false);
	connect( scrActions["toolsPreflightVerifier"], SIGNAL(toggled(bool)) , docCheckerPalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsPreflightVerifier"], SIGNAL(toggled(bool)) , this, SLOT(docCheckToggle(bool)) );
	connect( docCheckerPalette, SIGNAL(paletteShown(bool)), scrActions["toolsPreflightVerifier"], SLOT(setChecked(bool)));
	connect( docCheckerPalette, SIGNAL(paletteShown(bool)), this, SLOT(docCheckToggle(bool)));
	docCheckerPalette->installEventFilter(this);
	docCheckerPalette->hide();

	alignDistributePalette = new AlignDistributePalette(this, "AlignDistributePalette");
	connect( scrActions["toolsAlignDistribute"], SIGNAL(toggled(bool)) , alignDistributePalette, SLOT(setPaletteShown(bool)) );
	connect( alignDistributePalette, SIGNAL(paletteShown(bool)), scrActions["toolsAlignDistribute"], SLOT(setChecked(bool)));
	connect( alignDistributePalette, SIGNAL(documentChanged()), this, SLOT(slotDocCh()));
	alignDistributePalette->installEventFilter(this);

	symbolPalette = new SymbolPalette(this);
	symbolPalette->setMainWindow(this);
	connect(scrActions["toolsSymbols"], SIGNAL(toggled(bool)), symbolPalette, SLOT(setPaletteShown(bool)));
	connect(symbolPalette, SIGNAL(paletteShown(bool)), scrActions["toolsSymbols"], SLOT(setChecked(bool)));
	connect(symbolPalette, SIGNAL(startEdit(QString)), this, SLOT(editSymbolStart(QString)));
	connect(symbolPalette, SIGNAL(endEdit()), this, SLOT(editSymbolEnd()));
	connect(symbolPalette, SIGNAL(objectDropped()), this, SLOT(PutToPatterns()));
	symbolPalette->installEventFilter(this);
	symbolPalette->hide();

	inlinePalette = new InlinePalette(this);
	inlinePalette->setMainWindow(this);
	connect(scrActions["toolsInline"], SIGNAL(toggled(bool)), inlinePalette, SLOT(setPaletteShown(bool)));
	connect(inlinePalette, SIGNAL(paletteShown(bool)), scrActions["toolsInline"], SLOT(setChecked(bool)));
	connect(inlinePalette, SIGNAL(startEdit(int)), this, SLOT(editInlineStart(int)));
	connect(inlinePalette, SIGNAL(endEdit()), this, SLOT(editInlineEnd()));
	connect(inlinePalette, SIGNAL(objectDropped(QString)), this, SLOT(PutToInline(QString)));
	inlinePalette->installEventFilter(this);
	inlinePalette->hide();

	undoPalette = new UndoPalette(this, "undoPalette");
	undoPalette->installEventFilter(this);
	m_undoManager->registerGui(undoPalette);
	connect(undoPalette, SIGNAL(paletteShown(bool)), this, SLOT(setUndoPalette(bool)));
	connect(undoPalette, SIGNAL(objectMode(bool)), this, SLOT(setUndoMode(bool)));

	// initializing style manager here too even it's not strictly a palette
	m_styleManager = new StyleManager(this, "styleManager");
	SMCharacterStyle *tmpCS = new SMCharacterStyle();
	m_styleManager->addStyle(new SMParagraphStyle(tmpCS));
	m_styleManager->addStyle(tmpCS);
	m_styleManager->addStyle(new SMTableStyle());
	m_styleManager->addStyle(new SMCellStyle());
	m_styleManager->addStyle(new SMLineStyle());
	connect( scrActions["editStyles"], SIGNAL(toggled(bool)), m_styleManager, SLOT(setPaletteShown(bool)) );
	connect( m_styleManager, SIGNAL(paletteShown(bool)), scrActions["editStyles"], SLOT(setChecked(bool)));
	m_styleManager->installEventFilter(this);

	// initializing mark`s manager
	marksManager = new MarksManager(this, "marksManager");
	connect( scrActions["editMarks"], SIGNAL(toggled(bool)), marksManager, SLOT(setPaletteShown(bool)) );
	connect( marksManager, SIGNAL(paletteShown(bool)), scrActions["editMarks"], SLOT(setChecked(bool)));
	marksManager->installEventFilter(this);
	// initializing notes styles manager
	nsEditor = new NotesStylesEditor(this, "notesStylesEditor");
	connect( scrActions["editNotesStyles"], SIGNAL(toggled(bool)), nsEditor, SLOT(setPaletteShown(bool)) );
	connect( nsEditor, SIGNAL(paletteShown(bool)), scrActions["editNotesStyles"], SLOT(setChecked(bool)));
	nsEditor->installEventFilter(this);

	connect(docCheckerPalette, SIGNAL(selectElementByItem(PageItem *, bool)), this, SLOT(selectItemsFromOutlines(PageItem *, bool)));
	connect(docCheckerPalette, SIGNAL(selectElement(PageItem *, bool, int)), this, SLOT(selectItemFromOutlines(PageItem *, bool, int)));
	connect(docCheckerPalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(docCheckerPalette, SIGNAL(selectMasterPage(QString)), this, SLOT(editMasterPagesStart(QString)));
	connect(outlinePalette, SIGNAL(selectElementByItem(PageItem *, bool)), this, SLOT(selectItemsFromOutlines(PageItem *, bool)));
	connect(outlinePalette, SIGNAL(editElementByItem(PageItem *)), this, SLOT(editItemsFromOutlines(PageItem *)));
	connect(outlinePalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(outlinePalette, SIGNAL(selectMasterPage(QString)), this, SLOT(editMasterPagesStart(QString)));
	connect(nodePalette, SIGNAL(paletteClosed()), this, SLOT(slotSelect()));
	connect(nodePalette, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(layerPalette, SIGNAL(LayerChanged()), this, SLOT(showLayer()));

	connect(bookmarkPalette->BView, SIGNAL(markMoved()), this, SLOT(StoreBookmarks()));
	connect(bookmarkPalette->BView, SIGNAL(changed()), this, SLOT(slotDocCh()));
	connect(bookmarkPalette->BView, SIGNAL(selectElement(PageItem *, bool)), this, SLOT(selectItemsFromOutlines(PageItem *, bool)));
	// guides
	connect(scrActions["pageManageGuides"], SIGNAL(toggled(bool)), guidePalette, SLOT(setPaletteShown(bool)));
	connect(guidePalette, SIGNAL(paletteShown(bool)), scrActions["pageManageGuides"], SLOT(setChecked(bool)));
	// char palette
	connect(scrActions["insertGlyph"], SIGNAL(toggled(bool)), charPalette, SLOT(setPaletteShown(bool)));
	connect(charPalette, SIGNAL(paletteShown(bool)), scrActions["insertGlyph"], SLOT(setChecked(bool)));
}


void ScribusMainWindow::initScrapbook()
{
	QString scrapbookFileO = QDir::toNativeSeparators(m_prefsManager->preferencesLocation()+"/scrap13.scs");
	QFileInfo scrapbookFileInfoO = QFileInfo(scrapbookFileO);
	if (scrapbookFileInfoO.exists())
	{
		scrapbookPalette->readOldContents(scrapbookFileO, ScPaths::scrapbookDir(true) + "main");
		QDir d = QDir();
		d.rename(scrapbookFileO, QDir::toNativeSeparators(m_prefsManager->preferencesLocation()+"/scrap13.backup"));
	}
	QString scrapbookTemp = QDir::toNativeSeparators(ScPaths::scrapbookDir(true) + "tmp");
	QFileInfo scrapbookTempInfo = QFileInfo(scrapbookTemp);
	if (scrapbookTempInfo.exists())
		scrapbookPalette->readTempContents(scrapbookTemp);
	QString scrapbookFile = QDir::toNativeSeparators(ScPaths::scrapbookDir(true) + "main");
	QFileInfo scrapbookFileInfo = QFileInfo(scrapbookFile);
	if (scrapbookFileInfo.exists())
		scrapbookPalette->readContents(scrapbookFile);
	scrapbookPalette->setScrapbookFileName(scrapbookFile);
	ScCore->fileWatcher->addDir(scrapbookFile, true);
	scrapbookPalette->setOpenScrapbooks(m_prefsManager->appPrefs.scrapbookPrefs.RecentScrapbooks);
	rebuildRecentPasteMenu();
	connect(scrapbookPalette, SIGNAL(updateRecentMenue()), this, SLOT(rebuildRecentPasteMenu()));
	connect(ScCore->fileWatcher, SIGNAL(dirChanged(QString )), scrapbookPalette, SLOT(reloadLib(QString )));
	connect(ScCore->fileWatcher, SIGNAL(dirDeleted(QString )), scrapbookPalette, SLOT(closeOnDel(QString )));
}


bool ScribusMainWindow::warningVersion(QWidget *parent)
{
	bool retval = false;
	int t = ScMessageBox::warning(parent, QObject::tr("Scribus Development Version"), "<qt>" +
								 QObject::tr("You are running a development version of Scribus 1.5.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.5.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?") + "</qt>",
								 QMessageBox::Ok | QMessageBox::Cancel,
								 QMessageBox::Cancel,	// GUI default
								 QMessageBox::Ok);	// batch default
	if (t == QMessageBox::Ok)
		retval = true;
	return retval;
}

void ScribusMainWindow::initMdiArea()
{
	mdiArea = new QMdiArea(this);
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	if (m_prefsManager->appPrefs.uiPrefs.useTabs)
	{
		mdiArea->setViewMode(QMdiArea::TabbedView);
		mdiArea->setTabsClosable(true);
		mdiArea->setDocumentMode(true);
	}
	else
		mdiArea->setViewMode(QMdiArea::SubWindowView);
	setCentralWidget(mdiArea);
}

void ScribusMainWindow::initMenuBar()
{
	RecentDocs.clear();
	scrMenuMgr->createMenu("File", ActionManager::defaultMenuNameEntryTranslated("File"));
	scrMenuMgr->addMenuItemString("fileNew", "File");
	scrMenuMgr->addMenuItemString("fileNewFromTemplate", "File");
	scrMenuMgr->addMenuItemString("fileOpen", "File");
	scrMenuMgr->addMenuItemString("FileOpenRecent", "File");
	scrMenuMgr->createMenu("FileOpenRecent", tr("Open &Recent"), "File", false, true);
	scrMenuMgr->addMenuItemString("SEPARATOR", "File");
	scrMenuMgr->addMenuItemString("fileClose", "File");
	scrMenuMgr->addMenuItemString("fileSave", "File");
	scrMenuMgr->addMenuItemString("fileSaveAs", "File");
	scrMenuMgr->addMenuItemString("fileRevert", "File");
	scrMenuMgr->addMenuItemString("fileCollect", "File");
	scrMenuMgr->addMenuItemString("SEPARATOR", "File");
	scrMenuMgr->createMenu("FileImport", tr("&Import"), "File");
	scrMenuMgr->addMenuItemString("FileImport", "File");
	scrMenuMgr->addMenuItemString("fileImportText", "FileImport");
	scrMenuMgr->addMenuItemString("fileImportAppendText", "FileImport");
	scrMenuMgr->addMenuItemString("fileImportImage", "FileImport");
	scrMenuMgr->addMenuItemString("fileImportVector", "FileImport");
	scrMenuMgr->addMenuItemString("FileExport", "File");
	scrMenuMgr->createMenu("FileExport", tr("&Export"), "File");
	scrMenuMgr->addMenuItemString("fileExportText", "FileExport");
	scrMenuMgr->addMenuItemString("fileExportAsEPS", "FileExport");
	scrMenuMgr->addMenuItemString("fileExportAsPDF", "FileExport");
	scrMenuMgr->addMenuItemString("SEPARATOR", "File");
	scrMenuMgr->addMenuItemString("fileDocSetup150", "File");
	scrMenuMgr->addMenuItemString("filePreferences150", "File");
	scrMenuMgr->addMenuItemString("SEPARATOR", "File");
	scrMenuMgr->addMenuItemString("filePrint", "File");
	if ( ScCore->haveGS() || ScCore->isWinGUI() )
		scrMenuMgr->addMenuItemString("PrintPreview", "File");
	scrMenuMgr->addMenuItemString("SEPARATOR", "File");
	scrMenuMgr->addMenuItemString("fileQuit", "File");

	scrMenuMgr->setMenuEnabled("FileImport", false);
	scrMenuMgr->setMenuEnabled("FileExport", false);

	scrMenuMgr->createMenu("Edit", ActionManager::defaultMenuNameEntryTranslated("Edit"));
	scrMenuMgr->addMenuItemString("editUndoAction", "Edit");
	scrMenuMgr->addMenuItemString("editRedoAction", "Edit");
	scrMenuMgr->addMenuItemString("editActionMode", "Edit");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Edit");
	scrMenuMgr->addMenuItemString("editCut", "Edit");
	scrMenuMgr->addMenuItemString("editCopy", "Edit");
	scrMenuMgr->addMenuItemString("editPaste", "Edit");
	scrMenuMgr->createMenu("EditPasteRecent", tr("Paste Recent"), "Edit",false,true);
	scrMenuMgr->createMenu("EditContents", tr("Contents"), "Edit");
	scrMenuMgr->addMenuItemString("editCopyContents", "EditContents");
	scrMenuMgr->addMenuItemString("editPasteContents", "EditContents");
	scrMenuMgr->addMenuItemString("editPasteContentsAbs", "EditContents");
	scrMenuMgr->addMenuItemString("editClearContents", "EditContents");
	scrMenuMgr->addMenuItemString("editTruncateContents", "EditTruncateContents");
	scrMenuMgr->addMenuItemString("itemDelete", "Edit");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Edit");
	scrMenuMgr->addMenuItemString("editSelectAll", "Edit");
	scrMenuMgr->addMenuItemString("editSelectAllOnLayer", "Edit");
	scrMenuMgr->addMenuItemString("editDeselectAll", "Edit");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Edit");
	scrMenuMgr->addMenuItemString("editSearchReplace", "Edit");
	scrMenuMgr->addMenuItemString("toolsEditWithStoryEditor", "Edit");
	scrMenuMgr->addMenuItemString("editEditWithImageEditor", "Edit");
	scrMenuMgr->addMenuItemString("editEditRenderSource", "Edit");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Edit");
	scrMenuMgr->addMenuItemString("editColorsAndFills", "Edit");
	scrMenuMgr->addMenuItemString("editReplaceColors", "Edit");
	scrMenuMgr->addMenuItemString("editStyles", "Edit");
	scrMenuMgr->addMenuItemString("editMarks", "Edit");
	scrMenuMgr->addMenuItemString("editNotesStyles", "Edit");
	scrMenuMgr->addMenuItemString("editMasterPages", "Edit");
	scrMenuMgr->addMenuItemString("editJavascripts", "Edit");
	scrMenuMgr->setMenuEnabled("EditPasteRecent", false);
	scrMenuMgr->setMenuEnabled("EditContents", false);


	//Item Menu
	scrMenuMgr->createMenu("Item", ActionManager::defaultMenuNameEntryTranslated("Item"));
	scrMenuMgr->createMenu("DuplicateTransform", tr("Duplicate/Transform"), "Item");
	scrMenuMgr->addMenuItemString("DuplicateTransform", "Item");
	scrMenuMgr->addMenuItemString("itemDuplicate", "DuplicateTransform");
	scrMenuMgr->addMenuItemString("itemMulDuplicate", "DuplicateTransform");
	scrMenuMgr->addMenuItemString("itemTransform", "DuplicateTransform");
	scrMenuMgr->createMenu("Grouping", tr("Grouping"), "Item");
	scrMenuMgr->addMenuItemString("Grouping", "Item");
	scrMenuMgr->addMenuItemString("itemGroup", "Grouping");
	scrMenuMgr->addMenuItemString("itemUngroup", "Grouping");
	scrMenuMgr->addMenuItemString("itemGroupAdjust", "Grouping");
	scrMenuMgr->createMenu("Locking", tr("Locking"), "Item");
	scrMenuMgr->addMenuItemString("Locking", "Item");
	scrMenuMgr->addMenuItemString("itemLock", "Locking");
	scrMenuMgr->addMenuItemString("itemLockSize", "Locking");
	scrMenuMgr->createMenu("ItemLevel", tr("Level"), "Item");
	scrMenuMgr->addMenuItemString("ItemLevel", "Item");
	scrMenuMgr->addMenuItemString("itemRaise", "ItemLevel");
	scrMenuMgr->addMenuItemString("itemLower", "ItemLevel");
	scrMenuMgr->addMenuItemString("itemRaiseToTop", "ItemLevel");
	scrMenuMgr->addMenuItemString("itemLowerToBottom", "ItemLevel");
	scrMenuMgr->createMenu("ItemLayer", tr("Send to La&yer"), "",false, true);
	scrMenuMgr->addMenuItemString("ItemLayer", "Item");
	scrMenuMgr->createMenu("SendTo", tr("Send to"), "Item");
	scrMenuMgr->addMenuItemString("SendTo", "Item");
	scrMenuMgr->createMenu("ItemSendToScrapbook", tr("Scrapbook"),"",false,true);
	scrMenuMgr->addMenuItemString("ItemSendToScrapbook", "SendTo");
	scrMenuMgr->addMenuItemString("itemSendToPattern", "SendTo");
	scrMenuMgr->addMenuItemString("itemSendToInline", "SendTo");
	scrMenuMgr->createMenu("Adjust", tr("Adjust"), "Item");
	scrMenuMgr->addMenuItemString("Adjust", "Item");
	scrMenuMgr->addMenuItemString("itemAdjustFrameHeightToText", "Adjust");
	scrMenuMgr->addMenuItemString("itemAdjustFrameToImage", "Adjust");
	scrMenuMgr->addMenuItemString("itemAdjustImageToFrame", "Adjust");
	scrMenuMgr->createMenu("Image", tr("Image"), "Item");
	scrMenuMgr->addMenuItemString("Image", "Item");
	scrMenuMgr->addMenuItemString("itemUpdateImage", "Image");
	scrMenuMgr->addMenuItemString("styleImageEffects", "Image");
	scrMenuMgr->addMenuItemString("itemExtendedImageProperties", "Image");
	scrMenuMgr->addMenuItemString("itemToggleInlineImage", "Image");
	scrMenuMgr->createMenu("ItemPreviewSettings", tr("Preview Settings"));
	scrMenuMgr->addMenuItemString("ItemPreviewSettings", "Image");
	scrMenuMgr->addMenuItemString("itemImageIsVisible", "ItemPreviewSettings");
	scrMenuMgr->addMenuItemString("SEPARATOR", "ItemPreviewSettings");
	scrMenuMgr->addMenuItemString("itemPreviewFull", "ItemPreviewSettings");
	scrMenuMgr->addMenuItemString("itemPreviewNormal", "ItemPreviewSettings");
	scrMenuMgr->addMenuItemString("itemPreviewLow", "ItemPreviewSettings");
	scrMenuMgr->createMenu("ItemPDFOptions", tr("&PDF Options"));
	scrMenuMgr->addMenuItemString("ItemPDFOptions", "Item");
	scrMenuMgr->addMenuItemString("itemPDFIsAnnotation", "ItemPDFOptions");
	scrMenuMgr->addMenuItemString("itemPDFIsBookmark", "ItemPDFOptions");
	scrMenuMgr->addMenuItemString("itemPDFAnnotationProps", "ItemPDFOptions");
	scrMenuMgr->addMenuItemString("itemPDFFieldProps", "ItemPDFOptions");
	scrMenuMgr->createMenu("ItemConvertTo", tr("C&onvert to"), "Item");
	scrMenuMgr->addMenuItemString("ItemConvertTo", "Item");
	scrMenuMgr->addMenuItemString("itemConvertToTextFrame", "ItemConvertTo");
	scrMenuMgr->addMenuItemString("itemConvertToImageFrame", "ItemConvertTo");
	scrMenuMgr->addMenuItemString("itemConvertToPolygon", "ItemConvertTo");
	scrMenuMgr->addMenuItemString("itemConvertToBezierCurve", "ItemConvertTo");
	scrMenuMgr->addMenuItemString("itemConvertToOutlines", "ItemConvertTo");
	scrMenuMgr->addMenuItemString("itemConvertToSymbolFrame", "ItemConvertTo");
	scrMenuMgr->createMenu("TextLinking", tr("Text Frame Links"), "Item");
	scrMenuMgr->addMenuItemString("TextLinking", "Item");
	scrMenuMgr->addMenuItemString("toolsLinkTextFrame", "TextLinking");
	scrMenuMgr->addMenuItemString("toolsUnlinkTextFrame", "TextLinking");
	scrMenuMgr->addMenuItemString("toolsUnlinkTextFrameAndCutText", "TextLinking");
	scrMenuMgr->createMenu("ItemPathOps", tr("Path Tools"), "Item");
	scrMenuMgr->addMenuItemString("ItemPathOps", "Item");
	scrMenuMgr->addMenuItemString("itemCombinePolygons", "ItemPathOps");
	scrMenuMgr->addMenuItemString("itemSplitPolygons", "ItemPathOps");
	scrMenuMgr->addMenuItemString("itemAttachTextToPath", "ItemPathOps");
	scrMenuMgr->addMenuItemString("itemDetachTextFromPath", "ItemPathOps");

	scrActions["itemPrintingEnabled"]->setEnabled(false);
	scrMenuMgr->setMenuEnabled("ItemConvertTo", false);

	scrMenuMgr->createMenu("Weld", tr("Welding"), "Item");
	scrMenuMgr->addMenuItemString("Weld", "Item");
	scrMenuMgr->addMenuItemString("itemWeld", "Weld");
	scrMenuMgr->addMenuItemString("itemsUnWeld", "Weld");
	scrMenuMgr->addMenuItemString("itemEditWeld", "Weld");
	scrMenuMgr->addMenuItemString("editMark", "Item");

	//Insert menu
	scrMenuMgr->createMenu("Insert", ActionManager::defaultMenuNameEntryTranslated("Insert"));
	scrMenuMgr->addMenuItemString("insertFrame", "Insert");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertTextFrame", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertImageFrame", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertRenderFrame", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertTable", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertShape", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertPolygon", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertArc", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertSpiral", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertLine", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertBezier", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertFreehandLine", "Insert");
	scrMenuMgr->addMenuItemString("toolsInsertCalligraphicLine", "Insert");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Insert");
	scrMenuMgr->addMenuItemString("stickyTools", "Insert");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Insert");
	scrMenuMgr->addMenuItemString("insertGlyph", "Insert");

	scrMenuMgr->createMenu("InsertChar", tr("&Character"), "Insert");
	scrMenuMgr->addMenuItemString("InsertChar", "Insert");
	scrMenuMgr->addMenuItemString("unicodePageNumber", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodePageCount", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeSoftHyphen", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeNonBreakingHyphen", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeZWJ", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeZWNJ", "InsertChar");
	scrMenuMgr->addMenuItemString("SEPARATOR", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeCopyRight", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeRegdTM", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeTM", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeSolidus", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeBullet", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeMidpoint", "InsertChar");
	scrMenuMgr->addMenuItemString("SEPARATOR", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeDashEm", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeDashEn", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeDashFigure", "InsertChar");
	scrMenuMgr->addMenuItemString("unicodeDashQuotation", "InsertChar");

	scrMenuMgr->createMenu("InsertQuote", tr("&Quote"), "Insert");
	scrMenuMgr->addMenuItemString("InsertQuote", "Insert");
	scrMenuMgr->addMenuItemString("unicodeQuoteApostrophe", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteStraight", "InsertQuote");
	scrMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteSingleLeft", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteSingleRight", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteDoubleLeft", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteDoubleRight", "InsertQuote");
	scrMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteSingleReversed", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteDoubleReversed", "InsertQuote");
	scrMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteLowSingleComma", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteLowDoubleComma", "InsertQuote");
	scrMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteSingleLeftGuillemet", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteSingleRightGuillemet", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteDoubleLeftGuillemet", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteDoubleRightGuillemet", "InsertQuote");
	scrMenuMgr->addMenuItemString("SEPARATOR", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteCJKSingleLeft", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteCJKSingleRight", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteCJKDoubleLeft", "InsertQuote");
	scrMenuMgr->addMenuItemString("unicodeQuoteCJKDoubleRight", "InsertQuote");

	scrMenuMgr->createMenu("InsertSpace", tr("S&paces && Breaks"), "Insert");
	scrMenuMgr->addMenuItemString("InsertSpace", "Insert");
	scrMenuMgr->addMenuItemString("unicodeNonBreakingSpace", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeSpaceEN", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeSpaceEM", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeSpaceThin", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeSpaceThick", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeSpaceMid", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeSpaceHair", "InsertSpace");
	scrMenuMgr->addMenuItemString("SEPARATOR", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeNewLine", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeFrameBreak", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeColumnBreak", "InsertSpace");
	scrMenuMgr->addMenuItemString("unicodeZerowidthSpace", "InsertSpace");

	scrMenuMgr->createMenu("InsertLigature", tr("Liga&ture"), "Insert");
	scrMenuMgr->addMenuItemString("InsertLigature", "Insert");
	scrMenuMgr->addMenuItemString("unicodeLigature_ff", "InsertLigature");
	scrMenuMgr->addMenuItemString("unicodeLigature_fi", "InsertLigature");
	scrMenuMgr->addMenuItemString("unicodeLigature_fl", "InsertLigature");
	scrMenuMgr->addMenuItemString("unicodeLigature_ffi", "InsertLigature");
	scrMenuMgr->addMenuItemString("unicodeLigature_ffl", "InsertLigature");
	scrMenuMgr->addMenuItemString("unicodeLigature_ft", "InsertLigature");
	scrMenuMgr->addMenuItemString("unicodeLigature_st", "InsertLigature");

	scrMenuMgr->addMenuItemString("SEPARATOR", "Insert");
	scrMenuMgr->addMenuItemString("insertSampleText", "Insert");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Insert");
	scrMenuMgr->createMenu("InsertMark", tr("Marks"), "Insert");
	scrMenuMgr->addMenuItemString("InsertMark", "Insert");
	scrMenuMgr->addMenuItemString("insertMarkAnchor", "InsertMark");
	scrMenuMgr->addMenuItemString("insertMarkNote", "InsertMark");
	scrMenuMgr->addMenuItemString("insertMarkItem", "InsertMark");
	scrMenuMgr->addMenuItemString("insertMark2Mark", "InsertMark");
	scrMenuMgr->addMenuItemString("insertMarkVariableText", "InsertMark");

	//Page menu
	scrMenuMgr->createMenu("Page", ActionManager::defaultMenuNameEntryTranslated("Page"));
	scrMenuMgr->addMenuItemString("pageInsert", "Page");
	scrMenuMgr->addMenuItemString("pageImport", "Page");
	scrMenuMgr->addMenuItemString("pageDelete", "Page");
	scrMenuMgr->addMenuItemString("pageCopy", "Page");
	scrMenuMgr->addMenuItemString("pageMove", "Page");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Page");
	scrMenuMgr->addMenuItemString("pageApplyMasterPage", "Page");
	scrMenuMgr->addMenuItemString("pageCopyToMasterPage", "Page");
	scrMenuMgr->addMenuItemString("pageManageGuides", "Page");
	scrMenuMgr->addMenuItemString("pageManageProperties", "Page");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Page");
	scrMenuMgr->addMenuItemString("viewSnapToGrid", "Page");
	scrMenuMgr->addMenuItemString("viewSnapToGuides", "Page");
	scrMenuMgr->addMenuItemString("viewSnapToElements", "Page");

	//View menu
	scrMenuMgr->createMenu("View", ActionManager::defaultMenuNameEntryTranslated("View"));
	scrMenuMgr->createMenu("ViewZoom", tr("Zoom"), "View");
	scrMenuMgr->addMenuItemString("ViewZoom", "View");
	scrMenuMgr->addMenuItemString("viewFitInWindow", "ViewZoom");
	scrMenuMgr->addMenuItemString("viewFitWidth", "ViewZoom");
	scrMenuMgr->addMenuItemString("viewFit50", "ViewZoom");
	scrMenuMgr->addMenuItemString("viewFit75", "ViewZoom");
	scrMenuMgr->addMenuItemString("viewFit100", "ViewZoom");
	scrMenuMgr->addMenuItemString("viewFit200", "ViewZoom");
	scrMenuMgr->addMenuItemString("viewFit400", "ViewZoom");
	scrMenuMgr->createMenu("ViewPreview", tr("Preview"), "View");
	scrMenuMgr->addMenuItemString("ViewPreview", "View");
	scrMenuMgr->addMenuItemString("viewPreviewMode", "ViewPreview");
	scrMenuMgr->createMenu("ViewMeasuring", tr("Measurement"), "View");
	scrMenuMgr->addMenuItemString("ViewMeasuring", "View");
	scrMenuMgr->addMenuItemString("viewShowRulers", "ViewMeasuring");
	scrMenuMgr->addMenuItemString("viewRulerMode", "ViewMeasuring");
	scrMenuMgr->addMenuItemString("showMouseCoordinates", "ViewMeasuring");
	scrMenuMgr->createMenu("ViewTextFrames", tr("Text Frames"), "View");
	scrMenuMgr->addMenuItemString("ViewTextFrames", "View");
	scrMenuMgr->addMenuItemString("viewShowBaseline", "ViewTextFrames");
	scrMenuMgr->addMenuItemString("viewShowColumnBorders", "ViewTextFrames");
	scrMenuMgr->addMenuItemString("viewShowTextChain", "ViewTextFrames");
	scrMenuMgr->addMenuItemString("viewShowTextControls", "ViewTextFrames");
	scrMenuMgr->createMenu("ViewImageFrames", tr("Image Frames"), "View");
	scrMenuMgr->addMenuItemString("ViewImageFrames", "View");
	scrMenuMgr->addMenuItemString("viewShowImages", "ViewImageFrames");
	scrMenuMgr->createMenu("ViewDocument", tr("Document"), "View");
	scrMenuMgr->addMenuItemString("ViewDocument", "View");
	scrMenuMgr->addMenuItemString("viewShowMargins", "ViewDocument");
	scrMenuMgr->addMenuItemString("viewShowBleeds", "ViewDocument");
	scrMenuMgr->addMenuItemString("viewShowFrames", "ViewDocument");
	scrMenuMgr->addMenuItemString("viewShowLayerMarkers", "ViewDocument");
	scrMenuMgr->createMenu("ViewGrids", tr("Grids and Guides"), "View");
	scrMenuMgr->addMenuItemString("ViewGrids", "View");
	scrMenuMgr->addMenuItemString("viewShowGrid", "ViewGrids");
	scrMenuMgr->addMenuItemString("viewShowGuides", "ViewGrids");

	//CB If this is viewNewView imeplemented, it should be on the windows menu
//	scrMenuMgr->addMenuItem(scrActions["viewNewView"], "View");

	// Table menu.
	scrMenuMgr->createMenu("ItemTable", ActionManager::defaultMenuNameEntryTranslated("Table"));
	scrMenuMgr->addMenuItemString("tableInsertRows", "ItemTable");
	scrMenuMgr->addMenuItemString("tableInsertColumns", "ItemTable");
	scrMenuMgr->addMenuItemString("tableDeleteRows", "ItemTable");
	scrMenuMgr->addMenuItemString("tableDeleteColumns", "ItemTable");
	scrMenuMgr->addMenuItemString("SEPARATOR", "ItemTable");
	scrMenuMgr->addMenuItemString("tableMergeCells", "ItemTable");
	scrMenuMgr->addMenuItemString("tableSplitCells", "ItemTable");
	scrMenuMgr->addMenuItemString("SEPARATOR", "ItemTable");
	scrMenuMgr->addMenuItemString("tableSetRowHeights", "ItemTable");
	scrMenuMgr->addMenuItemString("tableSetColumnWidths", "ItemTable");
	scrMenuMgr->addMenuItemString("tableDistributeRowsEvenly", "ItemTable");
	scrMenuMgr->addMenuItemString("tableDistributeColumnsEvenly", "ItemTable");
	scrMenuMgr->addMenuItemString("SEPARATOR", "ItemTable");
	scrMenuMgr->addMenuItemString("tableAdjustFrameToTable", "ItemTable");
	scrMenuMgr->addMenuItemString("tableAdjustTableToFrame", "ItemTable");

	//Extra menu
	scrMenuMgr->createMenu("Extras", ActionManager::defaultMenuNameEntryTranslated("Extras"));
	scrMenuMgr->addMenuItemString("extrasHyphenateText", "Extras");
	scrMenuMgr->addMenuItemString("extrasDeHyphenateText", "Extras");
	scrMenuMgr->addMenuItemString("extrasGenerateTableOfContents", "Extras");
	scrMenuMgr->addMenuItemString("itemUpdateMarks", "Extras");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Extras");
	scrMenuMgr->addMenuItemString("extrasManageImages", "Extras");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Extras");
	scrMenuMgr->addMenuItemString("extrasUpdateDocument", "Extras");
//	Disabled for release as it does nothing useful
//	scrMenuMgr->addMenuItemString("extrasTestQTQuick2_1", "Extras");

	//Window menu
	scrMenuMgr->createMenu("Windows", ActionManager::defaultMenuNameEntryTranslated("Windows"), QString::null, true);

	//Help menu
	scrMenuMgr->createMenu("Help", ActionManager::defaultMenuNameEntryTranslated("Help"));
	scrMenuMgr->addMenuItemString("helpManual", "Help");
	scrMenuMgr->addMenuItemString("helpChat", "Help");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Help");
	scrMenuMgr->addMenuItemString("helpTooltips", "Help");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Help");
	scrMenuMgr->addMenuItemString("helpOnlineWWW", "Help");
	scrMenuMgr->addMenuItemString("helpOnlineDocs", "Help");
	scrMenuMgr->addMenuItemString("helpOnlineWiki", "Help");
	scrMenuMgr->addMenuItemString("HelpOnlineTutorials", "Help");
	scrMenuMgr->addMenuItemString("helpOnlineTutorial1", "Help");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Help");
	scrMenuMgr->addMenuItemString("helpCheckUpdates", "Help");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Help");
	scrMenuMgr->addMenuItemString("helpAboutScribus", "Help");
	scrMenuMgr->addMenuItemString("helpAboutPlugins", "Help");
	scrMenuMgr->addMenuItemString("helpAboutQt", "Help");
}

void ScribusMainWindow::createMenuBar()
{
	scrMenuMgr->addMenuStringToMenuBar("File");
	scrMenuMgr->addMenuItemStringstoMenuBar("File", scrActions);
	scrMenuMgr->addMenuStringToMenuBar("Edit");
	scrMenuMgr->addMenuItemStringstoMenuBar("Edit", scrActions);
	scrMenuMgr->addMenuStringToMenuBar("Item");
	scrMenuMgr->addMenuItemStringstoMenuBar("Item", scrActions);
	scrMenuMgr->addMenuStringToMenuBar("Insert");
	scrMenuMgr->addMenuItemStringstoMenuBar("Insert", scrActions);
	scrMenuMgr->addMenuStringToMenuBar("Page");
	scrMenuMgr->addMenuItemStringstoMenuBar("Page", scrActions);
	scrMenuMgr->addMenuStringToMenuBar("ItemTable");
	scrMenuMgr->addMenuItemStringstoMenuBar("ItemTable", scrActions);
	scrMenuMgr->addMenuStringToMenuBar("Extras");
	scrMenuMgr->addMenuItemStringstoMenuBar("Extras", scrActions);
	scrMenuMgr->addMenuStringToMenuBar("View");
	scrMenuMgr->addMenuItemStringstoMenuBar("View", scrActions);
	scrMenuMgr->addMenuStringToMenuBar("Windows", true);
	addDefaultWindowMenuItems();
	menuBar()->addSeparator();
	scrMenuMgr->addMenuStringToMenuBar("Help");
	scrMenuMgr->addMenuItemStringstoMenuBar("Help", scrActions);
	connect(scrMenuMgr->getLocalPopupMenu("Extras"), SIGNAL(aboutToShow()), this, SLOT(extrasMenuAboutToShow()));
	connect(scrMenuMgr->getLocalPopupMenu("Windows"), SIGNAL(aboutToShow()), this, SLOT(windowsMenuAboutToShow()));

}


void ScribusMainWindow::addDefaultWindowMenuItems()
{
	scrMenuMgr->clearMenu("Windows");
	scrMenuMgr->addMenuItemString("windowsCascade", "Windows");
	scrMenuMgr->addMenuItemString("windowsTile", "Windows");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Windows");
	scrMenuMgr->addMenuItemString("toolsProperties", "Windows");
	scrMenuMgr->addMenuItemString("toolsText", "Windows");
	scrMenuMgr->addMenuItemString("toolsActionHistory", "Windows");
	scrMenuMgr->addMenuItemString("toolsAlignDistribute", "Windows");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Windows");
	scrMenuMgr->addMenuItemString("toolsOutline", "Windows");
	scrMenuMgr->addMenuItemString("toolsPages", "Windows");
	scrMenuMgr->addMenuItemString("toolsLayers", "Windows");
	scrMenuMgr->addMenuItemString("toolsBookmarks", "Windows");
//	scrMenuMgr->addMenuItemString("toolsDownloads", "Windows");
	scrMenuMgr->addMenuItemString("toolsResources", "Windows");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Windows");
	scrMenuMgr->addMenuItemString("toolsScrapbook", "Windows");
	scrMenuMgr->addMenuItemString("toolsSymbols", "Windows");
	scrMenuMgr->addMenuItemString("toolsInline", "Windows");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Windows");
	scrMenuMgr->addMenuItemString("toolsMeasurements", "Windows");
	scrMenuMgr->addMenuItemString("toolsPreflightVerifier", "Windows");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Windows");
	scrMenuMgr->addMenuItemString("toolsToolbarTools", "Windows");
	scrMenuMgr->addMenuItemString("toolsToolbarPDF", "Windows");
	scrMenuMgr->addMenuItemString("toolsToolbarView", "Windows");
	scrMenuMgr->addMenuItemString("SEPARATOR", "Windows");
	scrMenuMgr->addMenuItemStringstoMenuBar("Windows", scrActions);
}


void ScribusMainWindow::initStatusBar()
{
	QFont fo = QFont(font());
	int posi = fo.pointSize() - (ScCore->isWinGUI() ? 1 : 2);
	fo.setPointSize(posi);
	unitSwitcher = new QComboBox( this );
	unitSwitcher->setObjectName("unitSwitcher");
	unitSwitcher->setFocusPolicy(Qt::NoFocus);
	unitSwitcher->setFont(fo);
	int maxUindex = unitGetMaxIndex() - 2;
	for (int i = 0; i <= maxUindex; ++i)
		unitSwitcher->addItem(unitGetStrFromIndex(i));


	QWidget* zoomWidget = new QWidget( statusBar() );
	QHBoxLayout* zoomLayout = new QHBoxLayout( zoomWidget );
	zoomLayout->setMargin(0);
	zoomLayout->setSpacing(1);

	zoomSpinBox = new ScrSpinBox( 1, 32000, zoomWidget, 6 );
	zoomSpinBox->setTabAdvance(false);
	zoomSpinBox->setFont(fo);
	zoomSpinBox->setValue( 100 );
	zoomSpinBox->setSingleStep(10);
	zoomSpinBox->setFocusPolicy(Qt::ClickFocus);
	zoomSpinBox->setSuffix( tr( " %" ) );
	layerMenu = new QComboBox( this );
	layerMenu->setObjectName("layerMenu");
	layerMenu->setEditable(false);
	layerMenu->setFont(fo);
	layerMenu->setFocusPolicy(Qt::NoFocus);
	layerMenu->setSizeAdjustPolicy(QComboBox::AdjustToContents);

	pageSelector = new PageSelector(this, 1);
	pageSelector->setObjectName("pageSelector");
	pageSelector->setFont(fo);
	pageSelector->setFocusPolicy(Qt::ClickFocus);

	zoomDefaultToolbarButton = new QPushButton(zoomWidget);
	zoomDefaultToolbarButton->setFocusPolicy(Qt::NoFocus);
	zoomDefaultToolbarButton->setDefault( false );
	zoomDefaultToolbarButton->setAutoDefault( false );
	zoomOutToolbarButton = new QPushButton(zoomWidget);
	zoomOutToolbarButton->setFocusPolicy(Qt::NoFocus);
	zoomOutToolbarButton->setDefault( false );
	zoomOutToolbarButton->setAutoDefault( false );
	zoomInToolbarButton = new QPushButton(zoomWidget);
	zoomInToolbarButton->setFocusPolicy(Qt::NoFocus);
	zoomInToolbarButton->setDefault( false );
	zoomInToolbarButton->setAutoDefault( false );

	zoomDefaultToolbarButton->setIcon(IconManager::instance()->loadIcon("16/zoom-original.png"));
	zoomOutToolbarButton->setIcon(IconManager::instance()->loadIcon("16/zoom-out.png"));
	zoomInToolbarButton->setIcon(IconManager::instance()->loadIcon("16/zoom-in.png"));


	zoomLayout->addWidget( zoomSpinBox );
	zoomLayout->addWidget( zoomOutToolbarButton );
	zoomLayout->addWidget( zoomDefaultToolbarButton );
	zoomLayout->addWidget( zoomInToolbarButton );

	m_mainWindowStatusLabel = new QLabel( "           ", statusBar());
	m_mainWindowStatusLabel->setFont(fo);
	mainWindowProgressBar = new QProgressBar(statusBar());
	mainWindowProgressBar->setAlignment(Qt::AlignHCenter);
	mainWindowProgressBar->setFixedWidth( 100 );
	mainWindowProgressBar->reset();
	mainWindowXPosLabel = new QLabel( tr("X:"), statusBar());
	mainWindowXPosLabel->setFont(fo);
	mainWindowYPosLabel = new QLabel( tr("Y:"), statusBar());
	mainWindowYPosLabel->setFont(fo);
	mainWindowXPosDataLabel = new QLabel( "", statusBar());
	mainWindowXPosDataLabel->setFont(fo);
	mainWindowYPosDataLabel = new QLabel( "", statusBar());
	mainWindowYPosDataLabel->setFont(fo);
	mainWindowXPosDataLabel->setMinimumWidth(mainWindowXPosDataLabel->fontMetrics().width("99999.999"));
	mainWindowYPosDataLabel->setMinimumWidth(mainWindowYPosDataLabel->fontMetrics().width("99999.999"));
	statusBarLanguageChange();

	layerMenu->setObjectName("layerMenu");
	unitSwitcher->setObjectName("unitSwitcher");
	zoomDefaultToolbarButton->setObjectName("zoomDefaultToolbarButton");
	zoomInToolbarButton->setObjectName("zoomInToolbarButton");
	zoomOutToolbarButton->setObjectName("zoomOutToolbarButton");
	zoomSpinBox->setObjectName("zoomSpinBox");

	statusBar()->setFont(fo);
	statusBar()->addPermanentWidget(m_mainWindowStatusLabel, 5);
	QLabel *s=new QLabel("");
	QLabel *s2=new QLabel("");
	QLabel *s3=new QLabel("");
	statusBar()->addPermanentWidget(s,1);
	statusBar()->addPermanentWidget(s2,1);
	statusBar()->addPermanentWidget(zoomWidget,0);
	statusBar()->addPermanentWidget(pageSelector,0);
	statusBar()->addPermanentWidget(layerMenu,1);
	statusBar()->addPermanentWidget(s3,3);
	statusBar()->addPermanentWidget(mainWindowXPosLabel, 0);
	statusBar()->addPermanentWidget(mainWindowXPosDataLabel, 0);
	statusBar()->addPermanentWidget(mainWindowYPosLabel, 0);
	statusBar()->addPermanentWidget(mainWindowYPosDataLabel, 0);

	statusBar()->addPermanentWidget(unitSwitcher,0);
	statusBar()->addPermanentWidget(mainWindowProgressBar, 0);
	connect(statusBar(), SIGNAL(messageChanged(const QString &)), this, SLOT(setTempStatusBarText(const QString &)));

}


void ScribusMainWindow::setStatusBarMousePosition(double xp, double yp)
{
	if (!HaveDoc)
	{
		mainWindowXPosDataLabel->clear();
		mainWindowYPosDataLabel->clear();
		return;
	}
	if (doc->Pages->count() == 0)
		return;
	double xn = xp;
	double yn = yp;
	if (doc->guidesPrefs().rulerMode)
	{
		xn -= doc->currentPage()->xOffset();
		yn -= doc->currentPage()->yOffset();
	}
	xn -= doc->rulerXoffset;
	yn -= doc->rulerYoffset;
	mainWindowXPosDataLabel->setText(value2String(xn, doc->unitIndex(), true, false));
	mainWindowYPosDataLabel->setText(value2String(yn, doc->unitIndex(), true, false));
}

void ScribusMainWindow::setStatusBarTextPosition(double base, double xp)
{
	if (doc->Pages->count() == 0)
		return;
	mainWindowXPosDataLabel->setText(base + xp >= 0? value2String(xp, doc->unitIndex(), true, true): QString("-"));
	mainWindowYPosDataLabel->setText("-");
}

void ScribusMainWindow::setStatusBarTextSelectedItemInfo()
{
	const int docSelectionCount = doc->m_Selection->count();
	if (docSelectionCount == 0)
	{
		setStatusBarInfoText("");
		return;
	}
	QString widthTxt = value2String(doc->m_Selection->width(), doc->unitIndex(), true, true);
	QString heightTxt = value2String(doc->m_Selection->height(), doc->unitIndex(), true, true);
	if (docSelectionCount == 1)
	{
		QString whatSel = tr("Unknown");
		switch (doc->m_Selection->itemAt(0)->itemType())
		{
			case 2:
				whatSel = CommonStrings::itemType_ImageFrame;
				break;
			case 4:
				whatSel = CommonStrings::itemType_TextFrame;
				break;
			case 5:
				whatSel = CommonStrings::itemType_Line;
				break;
			case 6:
				whatSel = CommonStrings::itemType_Polygon;
				break;
			case 7:
				whatSel = CommonStrings::itemType_Polyline;
				break;
			case 8:
				whatSel = CommonStrings::itemType_PathText;
				break;
			case 9:
				whatSel = CommonStrings::itemType_LatexFrame;
				break;
			case 11:
				whatSel = CommonStrings::itemType_Symbol;
				break;
			case 12:
				whatSel = CommonStrings::itemType_Group;
				break;
			case 13:
				whatSel = CommonStrings::itemType_RegularPolygon;
				break;
			case 14:
				whatSel = CommonStrings::itemType_Arc;
				break;
			case 15:
				whatSel = CommonStrings::itemType_Spiral;
				break;
			case 16:
				whatSel = CommonStrings::itemType_Table;
				break;
			default:
				whatSel = "Unknown";
				break;
		}
		QString txtBody = tr("%1 selected").arg(whatSel) + " : " + tr("Size");
		setStatusBarInfoText( QString("%1 = %3 x %4").arg(txtBody, widthTxt, heightTxt));
	}
	else
	{
		setStatusBarInfoText( tr("%1 Objects selected, Selection Size = %2 x %3").arg(docSelectionCount).arg(widthTxt).arg(heightTxt));
	}
}

void ScribusMainWindow::setTempStatusBarText(const QString &text)
{
	if (m_mainWindowStatusLabel)
	{
		if (text.isEmpty())
			m_mainWindowStatusLabel->setText(m_statusLabelText);
		else
			m_mainWindowStatusLabel->setText(text);
	}
}

void ScribusMainWindow::setStatusBarInfoText(QString newText)
{
	if (m_mainWindowStatusLabel)
		m_mainWindowStatusLabel->setText(newText);
	m_statusLabelText = newText;
}


//AV to be replaced with Selection::update and listener in textPalette
void ScribusMainWindow::setTBvals(PageItem *currItem)
{
	scrActions["editMark"]->setEnabled(false);
	
	PageItem* item  = currItem;
	bool inEditMode = (doc->appMode == modeEdit);
	if (doc->appMode == modeEditTable)
	{
		if (currItem->isTable())
			item = currItem->asTable()->activeCell().textFrame();
		inEditMode = item->isTextFrame();
	}
	if (!item || item->itemText.length() <= 0)
		return;

	const ParagraphStyle& currPStyle(inEditMode ? item->currentStyle() : item->itemText.defaultStyle());
	setAlignmentValue(currPStyle.alignment());

	// Assignment operator does not perform style context assignment
	// Do it in this case, otherwise we might get some crashes if previous
	// text object was deleted or things like that
	const ParagraphStyle& curStyle = item->currentStyle();
	doc->currentStyle.setContext(curStyle.context());
	doc->currentStyle = curStyle;
	if (doc->appMode == modeEdit || doc->appMode == modeEditTable)
		item->currentTextProps(doc->currentStyle);
	else
		doc->currentStyle.charStyle().setStyle(item->currentCharStyle());
	emit TextStyle(doc->currentStyle);
	// to go: (av)
	textPalette->textPal->updateStyle(doc->currentStyle);
	//check if mark in cursor place and enable editMark action
	if (doc->appMode == modeEdit && item->itemText.cursorPosition() < item->itemText.length())
	{
		if (item->itemText.hasMark(item->itemText.cursorPosition()))
		{
			Mark* mark = item->itemText.mark(item->itemText.cursorPosition());
			scrActions["editMark"]->setEnabled(true);
			if ((mark->isType(MARKNoteMasterType) || mark->isType(MARKNoteFrameType)) && (mark->getNotePtr() != NULL))
				nsEditor->setNotesStyle(mark->getNotePtr()->notesStyle());
		}
		else
			scrActions["editMark"]->setEnabled(false);
	}
}

//Special keys assigned to actions are stolen by the action and not passed to
//keyPressEvent so process them here.
void ScribusMainWindow::specialActionKeyEvent(int unicodevalue)
{
	if (!HaveDoc)
		return;
	if (doc->m_Selection->count() != 1)
		return;
	PageItem* selItem = doc->m_Selection->itemAt(0);
	if (((doc->appMode == modeEdit) || (doc->appMode == modeEditTable)) && (selItem->isTextFrame() || selItem->isTable()))
	{
		PageItem_TextFrame *currItem;
		if (doc->appMode == modeEditTable)
			currItem = selItem->asTable()->activeCell().textFrame();
		else
			currItem = selItem->asTextFrame();
		if (currItem!=NULL)
		{
			if (unicodevalue!=-1)
			{
				UndoTransaction activeTransaction;
				if (currItem->HasSel){
					if (UndoManager::undoEnabled())
						activeTransaction = m_undoManager->beginTransaction(Um::Selection, Um::IGroup, Um::ReplaceText, "", Um::IDelete);
					currItem->deleteSelectedTextFromFrame();
				}
				if (UndoManager::undoEnabled())
				{
					SimpleState *ss = dynamic_cast<SimpleState*>(m_undoManager->getLastUndo());
					if (ss && ss->get("ETEA") == "insert_frametext")
						ss->set("TEXT_STR",ss->get("TEXT_STR") + QString(QChar(unicodevalue)));
					else {
						ss = new SimpleState(Um::InsertText,"",Um::ICreate);
						ss->set("INSERT_FRAMETEXT");
						ss->set("ETEA", QString("insert_frametext"));
						ss->set("TEXT_STR", QString(QChar(unicodevalue)));
						ss->set("START", currItem->itemText.cursorPosition());
						UndoObject * undoTarget = currItem;
						if (currItem->isNoteFrame())
						{
							undoTarget = doc;
							ss->set("noteframeName", currItem->getUName());
						}
						m_undoManager->action(undoTarget, ss);
					}
				}
				currItem->itemText.insertChars(QString(QChar(unicodevalue)), true);
				if (activeTransaction)
					activeTransaction.commit();
			}
			else if (unicodevalue==SpecialChars::SHYPHEN.unicode()) //ignore the char as we use an attribute if the text item, for now.
			{
				// this code is currently dead since unicodeSoftHyphen
				// doesn't have unicodevalue == -1 any more
				if (currItem->itemText.cursorPosition() > 1)
				{
#if 0
					StyleFlag fl = currItem->itemText.item(qMax(currItem->CPos-1,0))->effects();
					fl |= ScStyle_HyphenationPossible;
					currItem->itemText.item(qMax(currItem->CPos-1,0))->setEffects(fl);
#else
					if (UndoManager::undoEnabled())
					{
						SimpleState *ss = dynamic_cast<SimpleState*>(m_undoManager->getLastUndo());
						if (ss && ss->get("ETEA") == "insert_frametext")
							ss->set("TEXT_STR",ss->get("TEXT_STR") + QString(SpecialChars::SHYPHEN));
						else {
							ss = new SimpleState(Um::InsertText,"",Um::ICreate);
							ss->set("INSERT_FRAMETEXT");
							ss->set("ETEA", QString("insert_frametext"));
							ss->set("TEXT_STR", QString(SpecialChars::SHYPHEN));
							ss->set("START", currItem->itemText.cursorPosition());
							UndoObject * undoTarget = currItem;
							if (currItem->isNoteFrame())
							{
								undoTarget = doc;
								ss->set("noteframeName", currItem->getUName());
							}
							m_undoManager->action(undoTarget, ss);
						}
					}
					currItem->itemText.insertChars(QString(SpecialChars::SHYPHEN), true);
#endif
				}
			}
			if (doc->appMode == modeEditTable)
				selItem->asTable()->update();
			else
				currItem->update();
		}
	}
}

bool ScribusMainWindow::eventFilter( QObject* /*o*/, QEvent *e )
{
	bool retVal;
	if (e->type() == QEvent::ToolTip)
		return (!m_prefsManager->appPrefs.displayPrefs.showToolTips);

	if ( e->type() == QEvent::KeyPress )
	{
		QKeyEvent *k = dynamic_cast<QKeyEvent *>(e);
		if (!k)
			return false;
		int keyMod=0;
		if (k->modifiers() & Qt::ShiftModifier)
			keyMod |= Qt::SHIFT;
		if (k->modifiers() & Qt::ControlModifier)
			keyMod |= Qt::CTRL;
		if (k->modifiers() & Qt::AltModifier)
			keyMod |= Qt::ALT;

		QKeySequence currKeySeq = QKeySequence(k->key() | keyMod);
		if (QString(currKeySeq.toString()).isNull())
			return false;
		retVal=true;
		//Palette actions
		if (actionManager->compareKeySeqToShortcut(currKeySeq, "specialToggleAllPalettes"))
			scrActions["specialToggleAllPalettes"]->activate(QAction::Trigger);
		else
		if (actionManager->compareKeySeqToShortcut(currKeySeq, "specialToggleAllGuides"))
			scrActions["specialToggleAllGuides"]->activate(QAction::Trigger);
		else
			retVal=false;
	}
	else
		retVal=false;
	//Return false to pass event to object
	return retVal;
}


void ScribusMainWindow::inputMethodEvent ( QInputMethodEvent * event )
{
	//qDebug() << "IMEmw" << event->commitString() << event->preeditString() << "attributes:" << event->attributes().count();
}

QVariant ScribusMainWindow::inputMethodQuery ( Qt::InputMethodQuery query ) const
{
	//qDebug() << "IMQmw" << query;
	return QVariant();
}

//AV -> CanvasMode
void ScribusMainWindow::keyPressEvent(QKeyEvent *k)
{
	QList<QMdiSubWindow *> windows;
	QMdiSubWindow* w = NULL;
	int kk = k->key();
	if (HaveDoc)
	{
		if ((doc->appMode == modeMagnifier) && (kk == Qt::Key_Shift))
		{
			view->setCursor(IconManager::instance()->loadCursor("lupezm.png"));
			return;
		}
	}
	if (m_keyrep)
		return;
	m_keyrep = true;
	int keyMod=0;
	if (k->modifiers() & Qt::ShiftModifier)
		keyMod |= Qt::SHIFT;
	if (k->modifiers() & Qt::ControlModifier)
		keyMod |= Qt::CTRL;
	if (k->modifiers() & Qt::AltModifier)
		keyMod |= Qt::ALT;
	//User presses escape and we have a doc open, and we have an item selected
	if ((kk == Qt::Key_Escape) && (HaveDoc))
	{
		m_keyrep = false;
		PageItem *currItem;
		if (!doc->m_Selection->isEmpty())
		{
			currItem = doc->m_Selection->itemAt(0);
			switch (doc->appMode)
			{
				case modeNormal:
				case modeEditClip:
					currItem->Sizing = false;
					if (doc->SubMode != -1)
					{
						view->Deselect(false);
						doc->Items->removeOne(currItem);
					}
					else
						view->Deselect(false);
					view->cancelGroupTransaction();
					break;
				case modeEdit:
					view->cancelGroupTransaction();
					break;
				case modeCopyProperties:
				case modeEditGradientVectors:
				case modeEditMeshGradient:
				case modeLinkFrames:
				case modeUnlinkFrames:
				case modeRotation:
					view->Deselect(false);
				case modeEditWeldPoint:
				case modeEyeDropper:
				case modeImportObject:
				case modeImportImage:
				case modePanning:
					view->requestMode(modeNormal);
					break;
				case modeDrawBezierLine:
					break;
				default:
					if (currItem->Sizing)
					{
						view->Deselect(false);
						doc->Items->removeOne(currItem);
					}
					break;
			}
		}
		doc->DragP = false;
		doc->leaveDrag = false;
		view->stopAllDrags();
		doc->SubMode = -1;
		doc->ElemToLink = NULL;
		slotSelect();
		if (doc->m_Selection->isEmpty())
			HaveNewSel();
		m_prefsManager->appPrefs.uiPrefs.stickyTools = false;
		scrActions["stickyTools"]->setChecked(false);
		return;
	}
	Qt::KeyboardModifiers buttonModifiers = k->modifiers();
	/**If we have a doc and we are not changing the page or zoom level in the status bar */
	if ((HaveDoc) && (!zoomSpinBox->hasFocus()) && (!pageSelector->hasFocus()))
	{
		//Show our context menu
		if (actionManager->compareKeySeqToShortcut(kk, buttonModifiers, "viewShowContextMenu"))
		{
			ContextMenu* cmen=NULL;
			if (doc->m_Selection->isEmpty())
			{
				//CB We should be able to get this calculated by the canvas.... it is already in m_canvas->globalToCanvas(m->globalPos());
				QPoint p(QCursor::pos() - mapToGlobal(QPoint(0,0)));
				FPoint fp(p.x() / view->scale() + doc->minCanvasCoordinate.x(),
				p.y() / view->scale() + doc->minCanvasCoordinate.y());
				cmen = new ContextMenu(this, doc, fp.x(), fp.y());
			}
			else
				cmen = new ContextMenu(*(doc->m_Selection), this, doc);
			if (cmen)
			{
				setUndoMode(true);
				cmen->exec(QCursor::pos());
				setUndoMode(false);
			}
			delete cmen;
		}


		/**
		 * With no item selected we can:
		 * - With space, get into panning mode (modePanning)
		 * - With PageUp, scroll up
		 * - With PageDown, scroll down
		 * - With Tab, change active document windowActivated
		 */

		if ((doc->appMode != modeEdit) && (doc->m_Selection->isEmpty()))
		{
			int pg;
			int wheelVal = m_prefsManager->mouseWheelJump();
			if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
				wheelVal = qMax(qRound(wheelVal / 10.0), 1);
			switch (kk)
			{
			case Qt::Key_Space:
				m_keyrep = false;
				if (doc->appMode == modePanning)
					view->requestMode(modeNormal);
				else
					view->requestMode(modePanning);
				return;
				break;
			case Qt::Key_PageUp:
				if (doc->masterPageMode() || doc->symbolEditMode() || doc->inlineEditMode())
					view->scrollBy(0, -m_prefsManager->mouseWheelJump());
				else
				{
					pg = doc->currentPageNumber();
					if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
						pg--;
					else
						pg -= doc->pageSets()[doc->pagePositioning()].Columns;
					if (pg > -1)
						view->GotoPage(pg);
				}
				m_keyrep = false;
				return;
				break;
			case Qt::Key_PageDown:
				if (doc->masterPageMode() || doc->symbolEditMode() || doc->inlineEditMode())
					view->scrollBy(0, m_prefsManager->mouseWheelJump());
				else
				{
					pg = doc->currentPageNumber();
					if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
						pg++;
					else
						pg += doc->pageSets()[doc->pagePositioning()].Columns;
					if (pg < static_cast<int>(doc->Pages->count()))
						view->GotoPage(pg);
				}
				m_keyrep = false;
				return;
				break;
			case Qt::Key_Left:
				view->scrollBy(-wheelVal, 0);
				m_keyrep = false;
				return;
				break;
			case Qt::Key_Right:
				view->scrollBy(wheelVal, 0);
				m_keyrep = false;
				return;
				break;
			case Qt::Key_Up:
				view->scrollBy(0, -wheelVal);
				m_keyrep = false;
				return;
				break;
			case Qt::Key_Down:
				view->scrollBy(0, wheelVal);
				m_keyrep = false;
				return;
				break;
			case Qt::Key_Tab:
				if (buttonModifiers == Qt::ControlModifier)
				{
					m_keyrep = false;
					windows = mdiArea->subWindowList();
					if (windows.count() > 1)
					{
						for (int i = 0; i < static_cast<int>(windows.count()); ++i)
						{
							if (mdiArea->activeSubWindow() == windows.at(i))
							{
								if (i == static_cast<int>(windows.count()-1))
									w = windows.at(0);
								else
									w = windows.at(i+1);
								break;
							}
						}
						outlinePalette->buildReopenVals();
						docCheckerPalette->clearErrorList();
						if ( w )
							w->showNormal();
						newActWin(w);
					}
					return;
				}
				break;
			}
		}
	}
	switch(kk)
	{
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
			m__arrowKeyDown = true;
	}
	m_keyrep = false;
}

void ScribusMainWindow::keyReleaseEvent(QKeyEvent *k)
{
	//Exit out of panning mode if Control is release while the right mouse button is pressed
	if (HaveDoc)
	{
		if ((doc->appMode == modePanning) && (k->key() == Qt::Key_Control) && (QApplication::mouseButtons() & Qt::RightButton))
			view->requestMode(modeNormal);

		if (doc->appMode == modeMagnifier)
			view->setCursor(IconManager::instance()->loadCursor("lupez.png"));
	}
	if (k->isAutoRepeat() || !m__arrowKeyDown)
		return;
	switch(k->key())
	{
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
			m__arrowKeyDown = false;
			if ((HaveDoc) && (!zoomSpinBox->hasFocus()) && (!pageSelector->hasFocus()))
			{
				int docSelectionCount=doc->m_Selection->count();
				if ((docSelectionCount != 0) && (doc->appMode == modeEditClip) && (doc->nodeEdit.hasNodeSelected()))
				{
					PageItem *currItem = doc->m_Selection->itemAt(0);
					double xposOrig = currItem->xPos();
					double yposOrig = currItem->yPos();
					doc->adjustItemSize(currItem);
					if (!doc->nodeEdit.isContourLine())
						currItem->ContourLine.translate(xposOrig - currItem->xPos(),yposOrig - currItem->yPos());
					currItem->update();
				}
				for (int i = 0; i < docSelectionCount; ++i)
					doc->m_Selection->itemAt(i)->checkChanges(true);
				if (docSelectionCount > 1 && view->groupTransactionStarted())
					view->endGroupTransaction();
			}
			break;
	}
}

void ScribusMainWindow::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void ScribusMainWindow::closeEvent(QCloseEvent *ce)
{
	//Do not quit if Preferences or new doc window is open
	PreferencesDialog *prefsDialog = findChild<PreferencesDialog *>(QString::fromLocal8Bit("PreferencesDialog"));
	NewDoc *newDocWin = findChild<NewDoc *>(QString::fromLocal8Bit("NewDocumentWindow"));
	if (prefsDialog!=NULL || newDocWin!=NULL)
	{
		ce->ignore();
		return;
	}
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	ScribusWin* tw;
	disconnect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(newActWin(QMdiSubWindow *)));
	if (!windows.isEmpty())
	{
		int windowCount=windows.count();
		for ( int i = 0; i < windowCount; ++i )
		{
			tw = dynamic_cast<ScribusWin *>(windows.at(i));
			QMdiSubWindow *tws = windows.at(i);
			ScribusWin* scw = dynamic_cast<ScribusWin *>(tws->widget());
			if (scw)
			{
				newActWin(windows.at(i));
				tw = ActWin;
				slotSelect();
				tws->close();
				if (tw == ActWin)
				{
					ce->ignore();
					connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(newActWin(QMdiSubWindow *)));
					return;
				}
			}
		}
	}
	fileToolBar->connectPrefsSlot(false);
	editToolBar->connectPrefsSlot(false);
	modeToolBar->connectPrefsSlot(false);
	pdfToolBar->connectPrefsSlot(false);

	m_prefsManager->appPrefs.uiPrefs.tabbedPalettes.clear();
	QList<QTabBar *> bars = findChildren<QTabBar *>(QString());
	for (int i = 0; i < bars.count(); ++i)
	{
		QTabBar *bar = bars[i];
		tabPrefs currentTab;
		for (int ii = 0; ii < bar->count(); ii++)
		{
			currentTab.activeTab = bar->currentIndex();
			QObject *obj = (QObject*)bar->tabData(ii).toULongLong();
			if (obj != NULL)
				currentTab.palettes.append(obj->objectName());
		}
		if (!currentTab.palettes.isEmpty())
			m_prefsManager->appPrefs.uiPrefs.tabbedPalettes.append(currentTab);
	}

	propertiesPalette->hide();
	textPalette->hide();
	outlinePalette->hide();
	scrapbookPalette->hide();
	bookmarkPalette->hide();
	downloadsPalette->hide();
	layerPalette->hide();
	pagePalette->hide();
	docCheckerPalette->hide();
	undoPalette->hide();
	alignDistributePalette->hide();
	guidePalette->hide();
	charPalette->hide();
	symbolPalette->hide();
	inlinePalette->hide();

	// Clean up plugins, THEN save prefs to disk
	ScCore->pluginManager->cleanupPlugins();
	if (!m_prefsManager->appPrefs.scrapbookPrefs.persistentScrapbook)
		scrapbookPalette->CleanUpTemp();
	m_prefsManager->appPrefs.scrapbookPrefs.RecentScrapbooks.clear();
	m_prefsManager->appPrefs.scrapbookPrefs.RecentScrapbooks = scrapbookPalette->getOpenScrapbooks();
	if (!emergencyActivated)
		m_prefsManager->SavePrefs();
	UndoManager::deleteInstance();
	FormatsManager::deleteInstance();
	UrlLauncher::deleteInstance();
//	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	ce->accept();
}

void ScribusMainWindow::requestUpdate(int val)
{
	emit UpdateRequest(val);
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


bool ScribusMainWindow::arrowKeyDown()
{
	return m__arrowKeyDown;
}

QStringList ScribusMainWindow::findRecoverableFile()
{
	QSet<QString> foundFiles;
	QDir::SortFlags sortflags = QDir::Name | QDir::Time;
	QDir::Filters filterflags = QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot;
	if (!m_prefsManager->appPrefs.docSetupPrefs.AutoSaveDir.isEmpty())
	{
		QDir dirAuto(m_prefsManager->appPrefs.docSetupPrefs.AutoSaveDir, "*_emergency_*.sla", sortflags, filterflags);
		QFileInfoList aList = dirAuto.entryInfoList();
		if (aList.count() > 0)
		{
			for (int i = 0; i < aList.count(); i++)
				foundFiles.insert(aList[i].absoluteFilePath());
		}
		QDir dirAuto2(m_prefsManager->appPrefs.docSetupPrefs.AutoSaveDir, "*_autosave_*.sla", sortflags, filterflags);
		QFileInfoList aList2 = dirAuto2.entryInfoList();
		if (aList2.count() > 0)
		{
			for (int i = 0; i < aList2.count(); i++)
				foundFiles.insert(aList2[i].absoluteFilePath());
		}
	}
	QDir dirDoc(m_prefsManager->documentDir(), "*_emergency_*.sla", sortflags, filterflags);
	QFileInfoList dList = dirDoc.entryInfoList();
	for (int i = 0; i < dList.count(); i++)
		foundFiles.insert(dList[i].absoluteFilePath());

	QDir dirDoc2(m_prefsManager->documentDir(), "*_autosave_*.sla", sortflags, filterflags);
	QFileInfoList dList2 = dirDoc2.entryInfoList();
	for (int i = 0; i < dList2.count(); i++)
		foundFiles.insert(dList2[i].absoluteFilePath());

	QDir dirHome(QDir::toNativeSeparators(QDir::homePath()), "*_emergency_*.sla", sortflags, filterflags);
	QFileInfoList hList = dirHome.entryInfoList();
	for (int i = 0; i < hList.count(); i++)
		foundFiles.insert(hList[i].absoluteFilePath());

	QDir dirHome2(QDir::toNativeSeparators(QDir::homePath()), "*_autosave_*.sla", sortflags, filterflags);
	QFileInfoList hList2 = dirHome2.entryInfoList();
	for (int i = 0; i < hList2.count(); i++)
		foundFiles.insert(hList2[i].absoluteFilePath());

	return foundFiles.toList();
}

bool ScribusMainWindow::recoverFile(QStringList foundFiles)
{
	appModeHelper->setStartupActionsEnabled(false);
	bool ret = false;
	RecoverDialog* dia = new RecoverDialog(this, foundFiles);
	if (dia->exec())
	{
		if (!dia->recoverFiles.isEmpty())
		{
			for (int i = 0; i < dia->recoverFiles.count(); ++i)
			{
				loadDoc(dia->recoverFiles[i]);
				doc->setName(dia->recoverNames[i]);
				doc->hasName = true;
				updateActiveWindowCaption(doc->DocName);
				outlinePalette->setDoc(doc);
				if (outlinePalette->isVisible())
					outlinePalette->BuildTree();
			}
			ret = true;
		}
	}
	delete dia;
	return ret;
}

void ScribusMainWindow::startUpDialog()
{
	bool docSet = false;
	PrefsContext* docContext = m_prefsManager->prefsFile->getContext("docdirs", false);
	NewDoc* dia = new NewDoc(this, RecentDocs, true, ScCore->getGuiLanguage());
	if (dia->exec())
	{
		if (dia->tabSelected() == NewDoc::NewDocumentTab)
		{
			int facingPages = dia->choosenLayout();
			int firstPage = dia->firstPage->currentIndex();
			docSet = dia->startDocSetup->isChecked();
			double topMargin = dia->marginGroup->top();
			double bottomMargin = dia->marginGroup->bottom();
			double leftMargin = dia->marginGroup->left();
			double rightMargin = dia->marginGroup->right();
			double columnDistance = dia->distance();
			double pageWidth = dia->pageWidth();
			double pageHeight = dia->pageHeight();
			double numberCols = dia->numberOfCols->value();
			bool autoframes = dia->autoTextFrame->isChecked();
			int orientation = dia->orientation();
			int pageCount=dia->pageCountSpinBox->value();
			QString pagesize;
			if (dia->pageSizeComboBox->currentText() == CommonStrings::trCustomPageSize)
				pagesize = CommonStrings::customPageSize;
			else
			{
				PageSize ps2(dia->pageSizeComboBox->currentText());
				pagesize = ps2.name();
			}
			doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->unitOfMeasureComboBox->currentIndex(), firstPage, orientation, 1, pagesize, true, pageCount, true, dia->marginGroup->getMarginPreset());
			doc->setPageSetFirstPage(facingPages, firstPage);
			doc->bleeds()->set(dia->bleedTop(), dia->bleedLeft(), dia->bleedBottom(), dia->bleedRight());
			HaveNewDoc();
			doc->reformPages(true);
			// Don's disturb user with "save?" dialog just after new doc
			// doc changing should be rewritten maybe... maybe later...
			doc->setModified(false);
			updateActiveWindowCaption(doc->DocName);
		}
		else if (dia->tabSelected() == NewDoc::NewFromTemplateTab)
		{
			QString fileName = QDir::cleanPath(dia->selectedFile());
			if (!fileName.isEmpty() && loadDoc(fileName))
			{
				doc->hasName = false;
				UndoManager::instance()->renameStack(dia->nftGui->currentDocumentTemplate->name);
				doc->DocName = dia->nftGui->currentDocumentTemplate->name;
				updateActiveWindowCaption(QObject::tr("Document Template: ") + dia->nftGui->currentDocumentTemplate->name);
				QDir::setCurrent(PrefsManager::instance()->documentDir());
				removeRecent(fileName);
			}
		}
		else if (dia->tabSelected() == NewDoc::OpenExistingTab)
		{
			QString fileName = dia->selectedFile();
			if (!fileName.isEmpty())
			{
				QFileInfo fi(fileName);
				docContext->set("docsopen", fi.absolutePath());
				loadDoc(fileName);
			}
		}
		else // NewDoc::OpenRecentTab
		{
			QString fileName = dia->selectedFile();
			if (!fileName.isEmpty())
				loadRecent(fileName);
		}
	}
	else
	{
		appModeHelper->setStartupActionsEnabled(false);
	}
	m_prefsManager->setShowStartupDialog(!dia->startUpDialog->isChecked());
	delete dia;
	m_mainWindowStatusLabel->setText( tr("Ready"));
	if (docSet)
		slotDocSetup();
}

bool ScribusMainWindow::slotFileNew()
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	bool retVal = false;
	bool docSet = false;
	NewDoc* dia = new NewDoc(this, RecentDocs);
	if (dia->exec())
	{
		int facingPages = dia->choosenLayout();
		int firstPage = dia->firstPage->currentIndex();
		docSet = dia->startDocSetup->isChecked();
		double topMargin = dia->marginGroup->top();
		double bottomMargin = dia->marginGroup->bottom();
		double leftMargin = dia->marginGroup->left();
		double rightMargin = dia->marginGroup->right();
		double columnDistance = dia->distance();
		double pageWidth = dia->pageWidth();
		double pageHeight = dia->pageHeight();
		double numberCols = dia->numberOfCols->value();
		bool autoframes = dia->autoTextFrame->isChecked();
		int orientation = dia->orientation();
		int pageCount=dia->pageCountSpinBox->value();
		QString pagesize;
		if (dia->pageSizeComboBox->currentText() == CommonStrings::trCustomPageSize)
			pagesize = CommonStrings::customPageSize;
		else
		{
			PageSize ps2(dia->pageSizeComboBox->currentText());
			pagesize = ps2.name();
		}
		if (doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->unitOfMeasureComboBox->currentIndex(), firstPage, orientation, 1, pagesize, true, pageCount, true, dia->marginGroup->getMarginPreset()))
		{
			doc->setPageSetFirstPage(facingPages, firstPage);
			doc->bleeds()->set(dia->bleedTop(), dia->bleedLeft(), dia->bleedBottom(), dia->bleedRight());
			m_mainWindowStatusLabel->setText( tr("Ready"));
			HaveNewDoc();
			doc->reformPages(true);
			retVal = true;
			// Don's disturb user with "save?" dialog just after new doc
			// doc changing should be rewritten maybe... maybe later...
			doc->setModified(false);
			updateActiveWindowCaption(doc->DocName);
		}
	}
	delete dia;
	if (docSet)
		slotDocSetup();
	return retVal;
}

//TODO move to core, assign doc to doc list, optionally create gui for it
ScribusDoc *ScribusMainWindow::newDoc(double width, double height, double topMargin, double leftMargin, double rightMargin, double bottomMargin, double columnDistance, double columnCount, bool autoTextFrames, int pageArrangement, int unitIndex, int firstPageLocation, int orientation, int firstPageNumber, const QString& defaultPageSize, bool requiresGUI, int pageCount, bool showView, int marginPreset)
{
	return doFileNew(width, height, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, columnCount, autoTextFrames, pageArrangement, unitIndex, firstPageLocation, orientation, firstPageNumber, defaultPageSize, requiresGUI, pageCount, showView, marginPreset);
}

ScribusDoc *ScribusMainWindow::doFileNew(double width, double height, double topMargin, double leftMargin, double rightMargin, double bottomMargin, double columnDistance, double columnCount, bool autoTextFrames, int pageArrangement, int unitIndex, int firstPageLocation, int orientation, int firstPageNumber, const QString& defaultPageSize, bool requiresGUI, int pageCount, bool showView, int marginPreset)
{
	if (HaveDoc)
		outlinePalette->buildReopenVals();
	m_undoManager->setUndoEnabled(false);
	MarginStruct margins(topMargin, leftMargin, bottomMargin, rightMargin);
	DocPagesSetup pagesSetup(pageArrangement, firstPageLocation, firstPageNumber, orientation, autoTextFrames, columnDistance, columnCount);
	QString newDocName( tr("Document")+"-"+QString::number(m_DocNr));
	ScribusDoc *tempDoc = new ScribusDoc();
	if (requiresGUI)
		doc=tempDoc;
	tempDoc->setLoading(true);
	outlinePalette->setDoc(tempDoc);
	ColorSetManager csm;
	csm.findPaletteLocations();
	csm.findPalettes();
	csm.findUserPalettes();
	ColorList colorList;
	QHash<QString, VGradient> gradientsList;
	QHash<QString, ScPattern> patternsList;
	QString Cpfad = QDir::toNativeSeparators(ScPaths::applicationDataDir())+"DefaultColors.xml";
	QFile fc(Cpfad);
	if (fc.exists())
	{
		csm.loadPalette(Cpfad, doc, colorList, gradientsList, patternsList, false);
		doc->PageColors = colorList;
		doc->docGradients = gradientsList;
		doc->docPatterns = patternsList;
	}
	else
	{
		if (m_prefsManager->appPrefs.colorPrefs.DColorSet != "Scribus Small")
		{
			QStringList CustomColorSets = csm.userPaletteNames();
			if (CustomColorSets.contains(m_prefsManager->appPrefs.colorPrefs.DColorSet))
				Cpfad = csm.userPaletteFileFromName(m_prefsManager->appPrefs.colorPrefs.DColorSet);
			else
				Cpfad = csm.paletteFileFromName(m_prefsManager->appPrefs.colorPrefs.DColorSet);
			if (!Cpfad.isEmpty())
				csm.loadPalette(Cpfad, doc, colorList, gradientsList, patternsList, false);
			doc->PageColors = colorList;
			doc->docGradients = gradientsList;
			doc->docPatterns = patternsList;
		}
		else
			doc->PageColors = m_prefsManager->appPrefs.colorPrefs.DColors;
	}
	tempDoc->PageColors.ensureDefaultColors();
	tempDoc->setup(unitIndex, pageArrangement, firstPageLocation, orientation, firstPageNumber, defaultPageSize, newDocName);
	if (requiresGUI)
	{
		HaveDoc++;
		m_DocNr++;
	}
	if (ScCore->haveCMS() && tempDoc->cmsSettings().CMSinUse)
		recalcColors();
	//CB NOTE should be all done now
	tempDoc->setPage(width, height, topMargin, leftMargin, rightMargin, bottomMargin, columnCount, columnDistance, autoTextFrames, pageArrangement);
	tempDoc->setMarginPreset(marginPreset);
	tempDoc->setMasterPageMode(false);
	tempDoc->createDefaultMasterPages();
	tempDoc->createNewDocPages(pageCount);
	tempDoc->addSection();
	tempDoc->setFirstSectionFromFirstPageNumber();
	tempDoc->setModified(false);
	tempDoc->OpenNodes.clear();
	if (requiresGUI)
	{
		actionManager->disconnectNewDocActions();
		actionManager->connectNewDocActions(tempDoc);
	}
	//<<View and window code
	QMdiArea* qwsp = 0;
	if (requiresGUI)
		qwsp = mdiArea;
	ScribusWin* w = new ScribusWin(qwsp, tempDoc);
	w->setMainWindow(this);
	if (requiresGUI && view!=NULL)
	{
		actionManager->disconnectNewViewActions();
		disconnect(view, SIGNAL(signalGuideInformation(int, qreal)), alignDistributePalette, SLOT(setGuide(int, qreal)));
	}
	ScribusView* tempView = new ScribusView(w, this, tempDoc);
	if (requiresGUI)
		view = tempView;
	tempDoc->setCurrentPage(tempDoc->Pages->at(0));
	tempDoc->setGUI(requiresGUI, this, tempView);
	if (requiresGUI)
	{
		tempDoc->createHyphenator();
		tempDoc->docHyphenator->ignoredWords = m_prefsManager->appPrefs.hyphPrefs.ignoredWords;
		tempDoc->docHyphenator->specialWords = m_prefsManager->appPrefs.hyphPrefs.specialWords;
	}
	tempDoc->setLoading(false);
	//run after setGUI to set up guidepalette ok

	tempView->setScale(m_prefsManager->displayScale());
	if (requiresGUI)
	{
		//done in newactinw actionManager->connectNewViewActions(tempView);
		alignDistributePalette->setDoc(tempDoc);
		docCheckerPalette->clearErrorList();
		symbolPalette->setDoc(tempDoc);
		inlinePalette->setDoc(tempDoc);
	}
	w->setView(tempView);
	ActWin = w;
	tempDoc->WinHan = w;
	if (requiresGUI)
		tempDoc->connectDocSignals(); //Must be before the first reformpages
	tempView->reformPages(true);
	//>>
	if (requiresGUI)
	{
		w->setSubWin(mdiArea->addSubWindow(w));
			connect(tempView, SIGNAL(signalGuideInformation(int, qreal)), alignDistributePalette, SLOT(setGuide(int, qreal)));
	}
	//Independent finishing tasks after tempDoc setup
	if (showView)
	{
		if ( mdiArea->subWindowList().count() == 1)
			w->showMaximized();
		else
			w->show();
		tempView->show();
		// Seems to fix crash on loading
		ActWin = NULL;
		newActWin(w->getSubWin());
	}
	if (requiresGUI)
	{
		connect(ScCore->fileWatcher, SIGNAL(fileChanged(QString)), tempDoc, SLOT(updatePict(QString)));
		connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString)), tempDoc, SLOT(removePict(QString)));
		connect(ScCore->fileWatcher, SIGNAL(dirChanged(QString )), tempDoc, SLOT(updatePictDir(QString )));
		connect(doc, SIGNAL(updateAutoSaveClock()), view->clockLabel, SLOT(resetTime()));
		view->clockLabel->resetTime();
		scrActions["viewToggleCMS"]->setChecked(tempDoc->HasCMS);
		m_undoManager->switchStack(tempDoc->DocName);
		m_styleManager->setDoc(tempDoc);
		marksManager->setDoc(tempDoc);
		nsEditor->setDoc(tempDoc);
		m_tocGenerator->setDoc(tempDoc);
	}
	m_undoManager->setUndoEnabled(true);
	return tempDoc;
}

void ScribusMainWindow::newFileFromTemplate()
{
	nftdialog* nftdia = new nftdialog(this, ScCore->getGuiLanguage());
	if (nftdia->exec() && nftdia->isTemplateSelected())
	{
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
		nfttemplate* currentTemplate = nftdia->currentTemplate();
		if (loadDoc(QDir::cleanPath(currentTemplate->file)))
		{
			doc->hasName = false;
			UndoManager::instance()->renameStack(currentTemplate->name);
			doc->DocName = currentTemplate->name;
			updateActiveWindowCaption(QObject::tr("Document Template: ") + currentTemplate->name);
			QDir::setCurrent(PrefsManager::instance()->documentDir());
			removeRecent(QDir::cleanPath(currentTemplate->file));
		}
		qApp->restoreOverrideCursor();
	}
	delete nftdia;
}

void ScribusMainWindow::newView()
{
	ScribusWin* w = new ScribusWin(mdiArea, doc);
	w->setMainWindow(this);
	view = new ScribusView(w, this, doc);
	view->setScale(m_prefsManager->displayScale());
	w->setView(view);
	ActWin = w;
	w->setCentralWidget(view);
	actionManager->connectNewViewActions(view);
	alignDistributePalette->setDoc(doc);
	connect(m_undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	view->show();
}

void ScribusMainWindow::windowsMenuAboutToShow()
{
	if (!scrWindowsActions.isEmpty())
	{
		for (QMap<QString, QPointer<ScrAction> >::iterator it = scrWindowsActions.begin(); it != scrWindowsActions.end(); ++it)
		{
			scrMenuMgr->removeMenuItem(it.key(), it.value(), "Windows");
		}
		scrWindowsActions.clear();
	}
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	bool windowsListNotEmpty=!windows.isEmpty();
	scrActions["windowsCascade"]->setEnabled(windowsListNotEmpty);
	scrActions["windowsTile"]->setEnabled(windowsListNotEmpty);
	if (windowsListNotEmpty)
	{
		int windowCount=static_cast<int>(windows.count());
		for ( int i = 0; i < windowCount; ++i )
		{
			QString docInWindow(windows.at(i)->windowTitle());
			scrWindowsActions.insert(docInWindow, new ScrAction( ScrAction::Window, QPixmap(), QPixmap(), docInWindow, QKeySequence(), this, i));
			scrWindowsActions[docInWindow]->setToggleAction(true);
			connect( scrWindowsActions[docInWindow], SIGNAL(triggeredData(int)), this, SLOT(windowsMenuActivated(int)) );
			scrWindowsActions[docInWindow]->setChecked(mdiArea->activeSubWindow() == windows.at(i));
			scrMenuMgr->addMenuItemString(docInWindow, "Windows");
		}
		if (windowCount>1)
			scrMenuMgr->addMenuItemStringstoRememberedMenu("Windows", scrWindowsActions);
	}
}

void ScribusMainWindow::extrasMenuAboutToShow()
{
	// There is only Picture Manager handled now.
	// As it can be opened all the time of the document life.
	// This is only check for availability of any ImageFrame
	// in the doc.
	bool enablePicManager = false;
	if (HaveDoc)
	{
		QList<PageItem*> allItems;
		for (int i = 0; i < doc->Items->count(); ++i)
		{
			PageItem *currItem = doc->Items->at(i);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				PageItem* item = allItems.at(ii);
#ifdef HAVE_OSG
				if ((item->itemType() == PageItem::ImageFrame) && (!((item->asLatexFrame()) || (item->asOSGFrame()))))
#else
				if ((item->itemType() == PageItem::ImageFrame) && (!(item->asLatexFrame())))
#endif
				{
					enablePicManager = true;
					break;
				}
			}
			allItems.clear();
		}
	}
	scrActions["extrasManageImages"]->setEnabled(enablePicManager);
}

void ScribusMainWindow::newActWin(QMdiSubWindow *w)
{
	if (w == NULL)
	{
		if (mdiArea->subWindowList().count() == 0)
			ActWin = NULL;
		return;
	}
	if (w->widget() == NULL)
		return;
	ScribusWin* scw = dynamic_cast<ScribusWin *>(w->widget());
	if (!scw)
		return;
	if (scw && scw->doc())
	{
		if (!scw->doc()->hasGUI())
			return;
	}
	if (scw == ActWin)
		return;
	ActWin = scw;
	if (ActWin->doc() == NULL)
		return;
	if (doc != NULL)
	{
		if (doc->appMode == modeEditClip)
			view->requestMode(submodeEndNodeEdit);
		if ((HaveDoc) && (doc != ActWin->doc()))
			outlinePalette->buildReopenVals();
	}
	docCheckerPalette->clearErrorList();
	if (HaveDoc && (doc != NULL) && doc->hasGUI())
	{
		disconnect(m_undoManager, SIGNAL(undoRedoBegin()), doc, SLOT(undoRedoBegin()));
		disconnect(m_undoManager, SIGNAL(undoRedoDone()) , doc, SLOT(undoRedoDone()));
		disconnect(m_undoManager, SIGNAL(undoRedoDone()) , doc->view(), SLOT(DrawNew()));
		disconnect(doc, SIGNAL(addBookmark(PageItem *)), this, SLOT(AddBookMark(PageItem *)));
		disconnect(doc, SIGNAL(deleteBookmark(PageItem *)), this, SLOT(DelBookMark(PageItem *)));
		unitSwitcher->disconnect();
		zoomSpinBox->disconnect();
		zoomDefaultToolbarButton->disconnect();
		zoomOutToolbarButton->disconnect();
		zoomInToolbarButton->disconnect();
		layerMenu->disconnect();
		disconnect(viewToolBar->previewQualitySwitcher, SIGNAL(activated(int)), this, SLOT(changePreviewQuality(int)));
		disconnect(viewToolBar->visualMenu, SIGNAL(activated(int)), doc->view(), SLOT(switchPreviewVisual(int)));
		pageSelector->disconnect();
		pageSelector->setEnabled(false);
	}
	doc = ActWin->doc();
	m_undoManager->switchStack(doc->DocName);
	if ((doc != NULL) && doc->hasGUI())
	{
		connect(m_undoManager, SIGNAL(undoRedoBegin()), doc, SLOT(undoRedoBegin()));
		connect(m_undoManager, SIGNAL(undoRedoDone()) , doc, SLOT(undoRedoDone()));
		connect(m_undoManager, SIGNAL(undoRedoDone()) , doc->view(), SLOT(DrawNew()));
		connect(doc, SIGNAL(addBookmark(PageItem *)), this, SLOT(AddBookMark(PageItem *)));
		connect(doc, SIGNAL(deleteBookmark(PageItem *)), this, SLOT(DelBookMark(PageItem *)));
		connect(unitSwitcher, SIGNAL(activated(int)), doc->view(), SLOT(ChgUnit(int)));
		connect(zoomSpinBox, SIGNAL(valueChanged(double)), doc->view(), SLOT(setZoom()));
		connect(zoomDefaultToolbarButton, SIGNAL(clicked()), doc->view(), SLOT(slotZoom100()));
		connect(zoomOutToolbarButton, SIGNAL(clicked()), doc->view(), SLOT(slotZoomOut()));
		connect(zoomInToolbarButton, SIGNAL(clicked()), doc->view(), SLOT(slotZoomIn()));
		connect(layerMenu, SIGNAL(activated(int)), doc->view(), SLOT(GotoLa(int)));
		scrActions["viewPreviewMode"]->blockSignals(true);
		scrActions["viewPreviewMode"]->setChecked(doc->drawAsPreview);
		scrActions["viewPreviewMode"]->blockSignals(false);
		appModeHelper->setPreviewMode(doc->drawAsPreview);
		scrActions["viewEditInPreview"]->setEnabled(doc->drawAsPreview);
		scrActions["viewToggleCMS"]->blockSignals(true);
		scrActions["viewToggleCMS"]->setChecked(doc->HasCMS);
		scrActions["viewToggleCMS"]->blockSignals(false);
		viewToolBar->previewQualitySwitcher->setCurrentIndex(doc->previewQuality());
		connect(viewToolBar->previewQualitySwitcher, SIGNAL(activated(int)), this, SLOT(changePreviewQuality(int)));
		viewToolBar->visualMenu->setCurrentIndex(doc->previewVisual);
		connect(viewToolBar->visualMenu, SIGNAL(activated(int)), doc->view(), SLOT(switchPreviewVisual(int)));
		viewToolBar->setDoc(doc);
		pageSelector->setMaximum(doc->masterPageMode() ? 1 : doc->Pages->count());
		slotSetCurrentPage(doc->currentPageNumber());
		connect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(setCurrentPage(int)));
		pageSelector->setEnabled(true);
	}
	if (view!=NULL)
	{
		actionManager->disconnectNewViewActions();
		disconnect(view, SIGNAL(signalGuideInformation(int, qreal)), alignDistributePalette, SLOT(setGuide(int, qreal)));
		if (ScCore->usingGUI())
			disconnect(doc->m_Selection, SIGNAL(selectionIsMultiple(bool)), 0, 0);
	}
	view = ActWin->view();
	bool b = zoomSpinBox->blockSignals(true);
	zoomSpinBox->setValue(view->scale() * 100.0 / PrefsManager::instance()->appPrefs.displayPrefs.displayScale);
	zoomSpinBox->blockSignals(b);
	actionManager->connectNewViewActions(view);
	actionManager->disconnectNewDocActions();
	actionManager->connectNewDocActions(doc);
	connect(view, SIGNAL(signalGuideInformation(int, qreal)), alignDistributePalette, SLOT(setGuide(int, qreal)));
	if (ScCore->usingGUI())
		connect(doc->m_Selection, SIGNAL(selectionIsMultiple(bool)), actionManager, SLOT( handleMultipleSelections(bool)));
	pagePalette->setView(view);
	alignDistributePalette->setDoc(doc);
	if (!doc->isLoading())
	{
		SwitchWin();
		view->requestMode(doc->appMode);
	}
	view->setFocus();
	view->setRulersShown(doc->guidesPrefs().rulersShown);
	scrActions["viewShowMargins"]->setChecked(doc->guidesPrefs().marginsShown);
	scrActions["viewShowBleeds"]->setChecked(doc->guidesPrefs().showBleed);
	scrActions["viewShowFrames"]->setChecked(doc->guidesPrefs().framesShown);
	scrActions["viewShowLayerMarkers"]->setChecked(doc->guidesPrefs().layerMarkersShown);
	scrActions["viewShowGrid"]->setChecked(doc->guidesPrefs().gridShown);
	scrActions["viewShowGuides"]->setChecked(doc->guidesPrefs().guidesShown);
	scrActions["viewShowColumnBorders"]->setChecked(doc->guidesPrefs().colBordersShown);
	scrActions["viewShowBaseline"]->setChecked(doc->guidesPrefs().baselineGridShown);
	scrActions["viewShowImages"]->setChecked(doc->guidesPrefs().showPic);
	scrActions["viewShowTextChain"]->setChecked(doc->guidesPrefs().linkShown);
	scrActions["viewShowTextControls"]->setChecked(doc->guidesPrefs().showControls);
	scrActions["viewShowRulers"]->setChecked(doc->guidesPrefs().rulersShown);
	scrActions["viewRulerMode"]->setChecked(doc->guidesPrefs().rulerMode);
	scrActions["extrasGenerateTableOfContents"]->setEnabled(doc->hasTOCSetup());
	scrActions["extrasUpdateDocument"]->setEnabled(true);
	if (!doc->masterPageMode())
		pagePalette->Rebuild();
	outlinePalette->setDoc(doc);
	if (outlinePalette->isVisible())
	{
		outlinePalette->BuildTree(false);
		outlinePalette->reopenTree();
	}
	RestoreBookMarks();
	if (!doc->isLoading())
	{
		HaveNewSel();
		if (!doc->m_Selection->isEmpty())
			doc->m_Selection->itemAt(0)->emitAllToGUI();
	}
	docCheckerPalette->setDoc(doc);
	m_tocGenerator->setDoc(doc);
	m_styleManager->setDoc(doc);
	marksManager->setDoc(doc);
	nsEditor->setDoc(doc);
	symbolPalette->setDoc(doc);
	inlinePalette->setDoc(doc);
	modeToolBar->setDoc(doc);
	// Give plugins a chance to react on changing the current document
	PluginManager& pluginManager(PluginManager::instance());
	QStringList pluginNames(pluginManager.pluginNames(false));
	ScPlugin* plugin;
	QString pName;
	for (int i = 0; i < pluginNames.count(); ++i)
	{
		pName = pluginNames.at(i);
		plugin = pluginManager.getPlugin(pName, true);
		Q_ASSERT(plugin); // all the returned names should represent loaded plugins
		plugin->setDoc(doc);
	}
}

void ScribusMainWindow::windowsMenuActivated(int id)
{
	QMdiSubWindow* windowWidget = mdiArea->subWindowList().at( id );
	if (windowWidget)
	{
		if (windowWidget->isShaded() || windowWidget->isMinimized())
			windowWidget->showNormal();
		windowWidget->raise();
	}
	newActWin(windowWidget);
}


void ScribusMainWindow::SwitchWin()
{
	updateActiveWindowCaption(doc->DocName);
	propertiesPalette->setDoc(doc);
	textPalette->setDoc(doc);
	marksManager->setDoc(doc);
	nsEditor->setDoc(doc);
	pagePalette->setView(view);
	layerPalette->setDoc(doc);
	guidePalette->setDoc(doc);
	charPalette->setDoc(doc);
	outlinePalette->setDoc(doc);
	symbolPalette->setDoc(doc);
	inlinePalette->setDoc(doc);
	rebuildLayersList();
	updateLayerMenu();
	//Do not set this!, it doesn't get valid pointers unless its in EditClip mode and its not
	//if we are switching windows #4357
	//nodePalette->setDoc(doc, view);
	slotChangeUnit(doc->unitIndex(), false);
/*	FIXME: check if this is really superflous now
	if (doc->appMode == modeEditClip)
	{
		doc->appMode = modeNormal;
		view->requestMode(submodeEndNodeEdit);
	} */
	appModeHelper->mainWindowSwitchWin(doc);

	if (doc->masterPageMode() || doc->symbolEditMode() || doc->inlineEditMode())
		pagePalette->enablePalette(false);
	else
	{
		if (doc->isModified())
			slotDocCh(false);
		pagePalette->enablePalette(true);
		setPreviewToolbar();
	}
}

void ScribusMainWindow::HaveNewDoc()
{
	appModeHelper->mainWindowHasNewDoc(doc, (ScMimeData::clipboardHasScribusData()) || (scrapbookPalette->tempHasContents()));

	//Update palettes
	updateActiveWindowCaption(doc->DocName);
	propertiesPalette->setDoc(doc);
	textPalette->setDoc(doc);
	nsEditor->setDoc(doc);

	marksManager->setDoc(doc);
	symbolPalette->setDoc(doc);
	inlinePalette->setDoc(doc);
	pagePalette->setView(view);
	layerPalette->setDoc(doc);
	guidePalette->setDoc(doc);
	charPalette->setDoc(doc);
	outlinePalette->setDoc(doc);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	rebuildLayersList();
	updateLayerMenu();
	slotChangeUnit(doc->unitIndex());
	windowsMenuAboutToShow();

	// #9275 : scripter must call HaveNewDoc() in case new doc has been created in a script
	// We may consequently have to call HaveNewDoc several times for the same doc.
	// Use Qt::UniqueConnection here to avoid multiple identical signal connections
	connect(view, SIGNAL(changeUN(int)), this, SLOT(slotChangeUnit(int)), Qt::UniqueConnection);
	connect(view, SIGNAL(changeLA(int)), layerPalette, SLOT(markActiveLayer(int)), Qt::UniqueConnection);
	connect(this, SIGNAL(changeLayers(int)), layerPalette, SLOT(markActiveLayer(int)), Qt::UniqueConnection);
	connect(view->horizRuler, SIGNAL(MarkerMoved(double, double)), this, SLOT(setStatusBarTextPosition(double, double)), Qt::UniqueConnection);
	connect(view->horizRuler, SIGNAL(DocChanged(bool)), this, SLOT(slotDocCh(bool)), Qt::UniqueConnection);
	connect(view, SIGNAL(ClipPo(double, double)), nodePalette, SLOT(SetXY(double, double)), Qt::UniqueConnection);
	connect(view, SIGNAL(PolyOpen()), nodePalette, SLOT(IsOpen()), Qt::UniqueConnection);
	connect(view, SIGNAL(PStatus(int, uint)), nodePalette, SLOT(PolyStatus(int, uint)), Qt::UniqueConnection);
	connect(view, SIGNAL(ItemGeom()), propertiesPalette->xyzPal, SLOT(handleSelectionChanged()), Qt::UniqueConnection);
	connect(view, SIGNAL(ChBMText(PageItem *)), this, SLOT(BookMarkTxT(PageItem *)), Qt::UniqueConnection);
	connect(view, SIGNAL(HaveSel()), this, SLOT(HaveNewSel()), Qt::UniqueConnection);
	connect(view, SIGNAL(PaintingDone()), this, SLOT(slotSelect()), Qt::UniqueConnection);
	connect(view, SIGNAL(DocChanged()), this, SLOT(slotDocCh()), Qt::UniqueConnection);
	connect(view, SIGNAL(MousePos(double, double)), this, SLOT(setStatusBarMousePosition(double, double)), Qt::UniqueConnection);
	connect(view, SIGNAL(ItemCharStyle(const CharStyle&)), textPalette->textPal, SLOT(updateCharStyle(const CharStyle&)), Qt::UniqueConnection);
	connect(view, SIGNAL(ItemTextEffects(int)), this, SLOT(setStyleEffects(int)), Qt::UniqueConnection);
	connect(view, SIGNAL(ItemTextAlign(int))  , this, SLOT(setAlignmentValue(int)), Qt::UniqueConnection);
	connect(view, SIGNAL(HasTextSel()), this, SLOT(EnableTxEdit()), Qt::UniqueConnection);
	connect(view, SIGNAL(HasNoTextSel()), this, SLOT(DisableTxEdit()), Qt::UniqueConnection);
	connect(view, SIGNAL(CopyItem()), this, SLOT(slotEditCopy()), Qt::UniqueConnection);
	connect(view, SIGNAL(CutItem()), this, SLOT(slotEditCut()), Qt::UniqueConnection);
	connect(view, SIGNAL(LoadPic()), this, SLOT(slotGetContent()), Qt::UniqueConnection);
	connect(view, SIGNAL(StatusPic()), this, SLOT(StatusPic()), Qt::UniqueConnection);
	connect(view, SIGNAL(AppendText()), this, SLOT(slotFileAppend()), Qt::UniqueConnection);
	connect(view, SIGNAL(AnnotProps()), this, SLOT(ModifyAnnot()), Qt::UniqueConnection);
	connect(view, SIGNAL(LoadElem(QString, double ,double, bool, bool, ScribusDoc *, ScribusView*)), this, SLOT(slotElemRead(QString, double, double, bool, bool, ScribusDoc *, ScribusView*)), Qt::UniqueConnection);
	connect(view, SIGNAL(AddBM(PageItem *)), this, SLOT(AddBookMark(PageItem *)), Qt::UniqueConnection);
	connect(view, SIGNAL(DelBM(PageItem *)), this, SLOT(DelBookMark(PageItem *)), Qt::UniqueConnection);
	connect(view, SIGNAL(DoGroup()), this, SLOT(GroupObj()), Qt::UniqueConnection);
	connect(view, SIGNAL(callGimp()), this, SLOT(callImageEditor()), Qt::UniqueConnection);
}

void ScribusMainWindow::HaveNewSel()
{
	if (doc == NULL)
		return;
	int SelectedType = -1;
	PageItem *currItem = NULL;
	const int docSelectionCount = doc->m_Selection->count();
	if (docSelectionCount > 0)
	{
		currItem = doc->m_Selection->itemAt(0);
		SelectedType = currItem->itemType();
	}
	assert (docSelectionCount == 0 || currItem != NULL); // help coverity analysis

	setStatusBarTextSelectedItemInfo();

	actionManager->disconnectNewSelectionActions();

	charPalette->setEnabled(false, 0);
	view->horizRuler->textMode(false);
	view->horizRuler->update();
	if (!doc->inAnEditMode())
		appModeHelper->enableActionsForSelection(this, doc);

	switch (SelectedType)
	{
	case -1: // None
		outlinePalette->slotShowSelect(doc->currentPageNumber(), NULL);
		propertiesPalette->setGradientEditMode(false);
		break;
	case PageItem::TextFrame: //Text Frame
		if (doc->appMode == modeEdit)
		{
			setTBvals(currItem);
			charPalette->setEnabled(true, currItem);
			if (currItem->asTextFrame())
			{
				appModeHelper->enableTextActions(true, currItem->currentStyle().charStyle().font().scName());
				currItem->asTextFrame()->toggleEditModeActions();
			}
			view->horizRuler->setItem(currItem);
			view->horizRuler->update();
		}
		else
		{
			const ParagraphStyle& curStyle = currItem->itemText.defaultStyle();
			doc->currentStyle.setContext(curStyle.context());
			doc->currentStyle = curStyle;
			emit TextStyle(doc->currentStyle);
			// to go: (av)
			textPalette->textPal->updateStyle(doc->currentStyle);
			setStyleEffects(doc->currentStyle.charStyle().effects());
		}
		break;
	case PageItem::Table:
		if (doc->appMode == modeEditTable)
		{
			charPalette->setEnabled(true, currItem);
			PageItem *cellItem = currItem->asTable()->activeCell().textFrame();
			setTBvals(cellItem);
			appModeHelper->enableTextActions(true, cellItem->currentCharStyle().font().scName());
		}
		break;
	case PageItem::PathText: //Path Text
		if (doc->appMode == modeEdit)
			setTBvals(currItem);
		else
		{
			const ParagraphStyle& curStyle = currItem->itemText.defaultStyle();
			doc->currentStyle.setContext(curStyle.context());
			doc->currentStyle = curStyle;
			textPalette->textPal->showParStyle(doc->currentStyle.parent());
			textPalette->textPal->showCharStyle(doc->currentStyle.charStyle().parent());
			emit TextStyle(doc->currentStyle);
			// to go: (av)
			textPalette->textPal->updateStyle(doc->currentStyle);
			setStyleEffects(doc->currentStyle.charStyle().effects());
		}
		break;
	}
	doc->CurrentSel = SelectedType;
	propertiesPalette->xyzPal->basePointWidget->setCheckedId(doc->rotationMode());

	if (docSelectionCount != 0)
	{
		actionManager->setPDFActions(view);
		updateItemLayerList();
		rebuildScrapbookMenu();
		propertiesPalette->setTextFlowMode(currItem->textFlowMode());
	}

	if (SelectedType != -1)
	{
		outlinePalette->slotShowSelect(currItem->OwnPage, currItem);
		actionManager->connectNewSelectionActions(view, doc);
	}

	PluginManager& pluginManager(PluginManager::instance());
	QStringList pluginNames(pluginManager.pluginNames(false));
	ScPlugin* plugin;
	ScActionPlugin* ixplug;
	ScrAction* pluginAction = 0;
	QString pName;
	for (int i = 0; i < pluginNames.count(); ++i)
	{
		pName = pluginNames.at(i);
		plugin = pluginManager.getPlugin(pName, true);
		Q_ASSERT(plugin); // all the returned names should represent loaded plugins
		if (plugin->inherits("ScActionPlugin"))
		{
			ixplug = dynamic_cast<ScActionPlugin*>(plugin);
			Q_ASSERT(ixplug);
			ScActionPlugin::ActionInfo ai(ixplug->actionInfo());
			pluginAction = ScCore->primaryMainWindow()->scrActions[ai.name];
			if (pluginAction != 0)
				pluginAction->setEnabled(ixplug->handleSelection(doc, SelectedType));
		}
	}
}

void ScribusMainWindow::slotDocCh(bool /*reb*/)
{
	if (!doc->isModified())
		doc->setModified(true);
	updateActiveWindowCaption(doc->DocName + "*");
	if (!doc->masterPageMode())
	{
		if (!doc->symbolEditMode() && !doc->inlineEditMode())
		{
			if (doc->hasName)
				scrActions["fileRevert"]->setEnabled(true);
			bool multiPages = doc->DocPages.count() > 1;
			scrActions["pageDelete"]->setEnabled(multiPages);
			scrActions["pageMove"]->setEnabled(multiPages);
			scrActions["fileCollect"]->setEnabled(true);
		}
		scrActions["fileClose"]->setEnabled(true);
	}

	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	// Give plugins a chance to react on changes in the current document
	PluginManager& pluginManager(PluginManager::instance());
	QStringList pluginNames(pluginManager.pluginNames(false));
	ScPlugin* plugin;
	QString pName;
	for (int i = 0; i < pluginNames.count(); ++i)
	{
		pName = pluginNames.at(i);
		plugin = pluginManager.getPlugin(pName, true);
		Q_ASSERT(plugin); // all the returned names should represent loaded plugins
		plugin->changedDoc(doc);
	}
	if (doc->flag_NumUpdateRequest)
	{
		doc->setupNumerations();
		emit UpdateRequest(reqNumUpdate);
	}
	while (doc->flag_Renumber)
	{
		doc->updateNumbers();
		if (!doc->flag_Renumber)
			doc->regionsChanged()->update(QRect());
	}
	if (m_marksCount != doc->marksList().count() || doc->notesChanged() || doc->flag_updateEndNotes || doc->flag_updateMarksLabels)
	{
		bool sendUpdateReqest = false;
		if (m_marksCount != doc->marksList().count() || doc->flag_updateMarksLabels)
			sendUpdateReqest = true;
		m_marksCount = doc->marksList().count();
		doc->updateMarks(doc->notesChanged());
		doc->updateChangedEndNotesFrames();
		if (sendUpdateReqest)
			emit UpdateRequest(reqMarksUpdate);
		doc->setNotesChanged(false);
		doc->flag_updateEndNotes = false;
		doc->flag_updateMarksLabels = false;
	}
}

void ScribusMainWindow::updateRecent(QString fn)
{
	if (RecentDocs.indexOf(fn) != -1)
		RecentDocs.removeAll(fn);
	RecentDocs.prepend(fn);
	rebuildRecentFileMenu();
}

void ScribusMainWindow::removeRecent(QString fn, bool fromFileWatcher)
{
	if (RecentDocs.indexOf(fn) != -1)
	{
		RecentDocs.removeAll(fn);
		//#9845: if (!fromFileWatcher)
		//#9845:	ScCore->fileWatcher->removeFile(fn);
	}
	rebuildRecentFileMenu();
}

void ScribusMainWindow::removeRecentFromWatcher(QString filename)
{
	removeRecent(filename, true);
}

void ScribusMainWindow::loadRecent(QString filename)
{
	QFileInfo fd(filename);
	if (!fd.exists())
		removeRecent(filename);
	else
		loadDoc(filename);
}

void ScribusMainWindow::rebuildRecentFileMenu()
{
	scrMenuMgr->clearMenuStrings("FileOpenRecent");
	scrRecentFileActions.clear();
	uint max = qMin(m_prefsManager->appPrefs.uiPrefs.recentDocCount, RecentDocs.count());
	QString strippedName, localName;
	for (uint m = 0; m < max; ++m)
	{
		strippedName = localName = QDir::toNativeSeparators(RecentDocs[m]);
		strippedName.remove(QDir::separator());
		strippedName.prepend(QString("%1").arg(m+1, 2, 10, QChar('0')));
		scrRecentFileActions.insert(strippedName, new ScrAction(ScrAction::RecentFile, QPixmap(), QPixmap(), QString("%1 &%2").arg(m+1).arg(localName.replace("&","&&")), QKeySequence(), this, RecentDocs[m]));
		connect( scrRecentFileActions[strippedName], SIGNAL(triggeredData(QString)), this, SLOT(loadRecent(QString)) );
		scrMenuMgr->addMenuItemString(strippedName, "FileOpenRecent");
	}
	scrMenuMgr->addMenuItemStringstoRememberedMenu("FileOpenRecent", scrRecentFileActions);
	fileToolBar->rebuildRecentFileMenu();
}

void ScribusMainWindow::rebuildRecentPasteMenu()
{
	scrMenuMgr->clearMenuStrings("EditPasteRecent");

	scrRecentPasteActions.clear();
	int max = qMin(m_prefsManager->appPrefs.scrapbookPrefs.numScrapbookCopies, scrapbookPalette->tempBView->objectMap.count());
	if (max > 0)
	{
		QMap<QString,BibView::Elem>::Iterator it;
		it = scrapbookPalette->tempBView->objectMap.end();
		it--;
		QString strippedName;
		for (int m = 0; m < max; ++m)
		{
			strippedName = it.key();
			QPixmap pm = it.value().Preview;
			scrRecentPasteActions.insert(strippedName, new ScrAction(ScrAction::RecentPaste, pm, QPixmap(), QString("&%1 %2").arg(m+1).arg(strippedName), QKeySequence(), this, it.key()));
			connect( scrRecentPasteActions[strippedName], SIGNAL(triggeredData(QString)), this, SLOT(pasteRecent(QString)) );
			scrMenuMgr->addMenuItemString(strippedName, "EditPasteRecent");
			it--;
		}
		scrMenuMgr->addMenuItemStringstoRememberedMenu("EditPasteRecent", scrRecentPasteActions);
	}
}

void ScribusMainWindow::rebuildScrapbookMenu()
{
	scrMenuMgr->clearMenuStrings("ItemSendToScrapbook");
	scrScrapActions.clear();
	if (!HaveDoc)
		return;
	QStringList scrapNames = scrapbookPalette->getOpenScrapbooksNames();
	scrapNames.removeAt(1);
	for (int i = 0; i < scrapNames.count(); ++i)
	{
		ScrAction *act = new ScrAction( ScrAction::DataInt, QPixmap(), QPixmap(), scrapNames[i], QKeySequence(), this, i);
		scrScrapActions.insert(scrapNames[i], act);
		connect(act, SIGNAL(triggeredData(int)), this, SLOT(PutScrap(int)));
		scrMenuMgr->addMenuItemString(scrapNames[i], "ItemSendToScrapbook");
	}
	scrMenuMgr->addMenuItemStringstoRememberedMenu("ItemSendToScrapbook", scrScrapActions);
}

void ScribusMainWindow::pasteFromScrapbook(QString fn)
{
	if (!HaveDoc)
		return;
	view->dragX = 0;
	view->dragY = 0;
	doPasteRecent(scrapbookPalette->activeBView->objectMap[fn].Data);
}

void ScribusMainWindow::pasteRecent(QString fn)
{
	doPasteRecent(scrapbookPalette->tempBView->objectMap[fn].Data);
}

void ScribusMainWindow::doPasteRecent(QString data)
{
	if (!HaveDoc)
		return;
	QFileInfo fi(data);
	QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::RASTORIMAGES, 1));
	QStringList rasterFiles = formatD.split("|");
	QStringList vectorFiles = LoadSavePlugin::getExtensionsForPreview(FORMATID_FIRSTUSER);
	if (vectorFiles.contains(fi.suffix().toLower()))
	{
		FileLoader *fileLoader = new FileLoader(data);
		int testResult = fileLoader->testFile();
		delete fileLoader;
		if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
		{
			const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
			if( fmt )
			{
				fmt->loadFile(data, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
			}
		}
		if (!doc->m_Selection->isEmpty())
		{
			double x2, y2, w, h;
			doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
			doc->moveGroup(doc->currentPage()->xOffset() - x2, doc->currentPage()->yOffset() - y2);
			emit UpdateRequest(reqColorsUpdate|reqTextStylesUpdate|reqLineStylesUpdate);
		}
	}
	else if (rasterFiles.contains(fi.suffix().toLower()))
	{
		int z = doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), 1, 1, doc->itemToolPrefs().shapeLineWidth, doc->itemToolPrefs().imageFillColor, doc->itemToolPrefs().imageStrokeColor);
		PageItem *b = doc->Items->at(z);
		b->LayerID = doc->activeLayer();
		doc->loadPict(data, b);
		b->setWidth(static_cast<double>(b->OrigW * 72.0 / b->pixm.imgInfo.xres));
		b->setHeight(static_cast<double>(b->OrigH * 72.0 / b->pixm.imgInfo.yres));
		b->OldB2 = b->width();
		b->OldH2 = b->height();
		b->updateClip();
		b->AdjustPictScale();
	}
	else
	{
		UndoTransaction pasteAction;
		if(UndoManager::undoEnabled())
			pasteAction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Create,"",Um::ICreate);
		view->Deselect(true);
		int docItemCount = doc->Items->count();
		bool savedAlignGrid = doc->SnapGrid;
		bool savedAlignGuides = doc->SnapGuides;
		bool savedAlignElement = doc->SnapElement;
		doc->SnapGrid = false;
		doc->SnapGuides = false;
		doc->SnapElement = false;
		if ((view->dragX == 0.0) && (view->dragY == 0.0))
			slotElemRead(data, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), true, true, doc, view);
		else
			slotElemRead(data, view->dragX, view->dragY, true, false, doc, view);
		doc->SnapGrid = savedAlignGrid;
		doc->SnapGuides = savedAlignGuides;
		doc->SnapElement = savedAlignElement;
		Selection tmpSelection(this, false);
		tmpSelection.copy(*doc->m_Selection, true);
		for (int i = docItemCount; i < doc->Items->count(); ++i)
		{
			PageItem* currItem = doc->Items->at(i);
			doc->setRedrawBounding(currItem);
			tmpSelection.addItem(currItem, true);
			if (currItem->isBookmark)
				AddBookMark(currItem);
		}
		doc->m_Selection->copy(tmpSelection, false);
		if (pasteAction)
			pasteAction.commit();
	}
	slotDocCh(false);
	doc->regionsChanged()->update(QRectF());
	view->dragX = 0;
	view->dragY = 0;
}

void ScribusMainWindow::importVectorFile()
{
	QStringList formats;
	QString allFormats = tr("All Supported Formats")+" (";
	int fmtCode = FORMATID_FIRSTUSER;
	const FileFormat *fmt = LoadSavePlugin::getFormatById(fmtCode);
	while (fmt != 0)
	{
		if (fmt->load)
		{
			formats.append(fmt->filter);
			int an = fmt->filter.indexOf("(");
			int en = fmt->filter.indexOf(")");
			while (an != -1)
			{
				allFormats += fmt->filter.mid(an+1, en-an-1)+" ";
				an = fmt->filter.indexOf("(", en);
				en = fmt->filter.indexOf(")", an);
			}
		}
		fmtCode++;
		fmt = LoadSavePlugin::getFormatById(fmtCode);
	}
	allFormats += "*.sce *.SCE);;";
	formats.append("Scribus Objects (*.sce *.SCE)");
	formats.sort(Qt::CaseInsensitive);
	allFormats += formats.join(";;");
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("pastefile", ".");
	CustomFDialog dia(this, wdir, tr("Open"), allFormats, fdExistingFiles | fdDisableOk);
	QString fileName("");
	if (dia.exec() == QDialog::Accepted)
	{
		fileName = dia.selectedFile();
		if (fileName.isEmpty())
			return;
	}
	else
		return;

	PrefsManager::instance()->prefsFile->getContext("dirs")->set("pastefile", fileName.left(fileName.lastIndexOf("/")));
	QFileInfo fi(fileName);
	QString suffix = fi.suffix().toLower();
	if ((suffix == "sce") || (suffix == "shape"))
	{
		QList<QUrl> urls;
		QMimeData* md = new QMimeData();
		urls.append( QUrl::fromLocalFile(fileName) );
		md->setUrls(urls);
		QDrag* dr = new QDrag(this);
		dr->setMimeData(md);
		const QPixmap& dragCursor = IconManager::instance()->loadPixmap("dragpix.png");
		dr->setPixmap(dragCursor);
		dr->exec();
	}
	else
	{
		FileLoader *fileLoader = new FileLoader(fileName);
		int testResult = fileLoader->testFile();
		delete fileLoader;
		if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
		{
			const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
			if (fmt)
			{
				doc->dontResize = true;
				fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive);
				doc->dontResize = false;
			}
		}
	}
	requestUpdate(reqColorsUpdate | reqSymbolsUpdate | reqLineStylesUpdate | reqTextStylesUpdate);
}

void ScribusMainWindow::rebuildLayersList()
{
	if (!HaveDoc)
		return;
	scrMenuMgr->clearMenuStrings("ItemLayer");
	scrLayersActions.clear();
	ScLayers::iterator it;
	if (doc->Layers.count()!= 0)
	{
		for (it = doc->Layers.begin(); it != doc->Layers.end(); ++it)
		{
			scrLayersActions.insert(QString("%1").arg((*it).ID), new ScrAction( ScrAction::Layer, QPixmap(), QPixmap(), (*it).Name, QKeySequence(), this, (*it).ID));
			scrLayersActions[QString("%1").arg((*it).ID)]->setToggleAction(true);
			QPixmap pm(20,15);
			pm.fill((*it).markerColor);
			scrLayersActions[QString("%1").arg((*it).ID)]->setIcon(pm);
		}
	}
	int currActiveLayer=doc->activeLayer();
	bool found=false;
	for (it = doc->Layers.begin(); it != doc->Layers.end(); ++it)
	{
		if ((*it).ID == currActiveLayer)
		{
			found=true;
			break;
		}
	}
	Q_ASSERT(found);
	scrLayersActions[QString("%1").arg((*it).ID)]->setChecked(true);

	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=scrLayersActions.end(); ++it )
	{
		scrMenuMgr->addMenuItemString(it.key(), "ItemLayer");
		connect( (*it), SIGNAL(triggeredData(int)), doc, SLOT(itemSelection_SendToLayer(int)) );
	}
	scrMenuMgr->addMenuItemStringstoRememberedMenu("ItemLayer", scrLayersActions);
}

void ScribusMainWindow::updateItemLayerList()
{
	if (!HaveDoc)
		return;
	QMap<QString, QPointer<ScrAction> >::Iterator itend=scrLayersActions.end();
	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=itend; ++it )
	{
		disconnect( (*it), SIGNAL(triggeredData(int)), 0, 0 );
		(*it)->setChecked(false);
	}
	if (!doc->m_Selection->isEmpty() && doc->m_Selection->itemAt(0))
		scrLayersActions[QString("%1").arg(doc->m_Selection->itemAt(0)->LayerID)]->setChecked(true);
	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=itend; ++it )
		connect( (*it), SIGNAL(triggeredData(int)), doc, SLOT(itemSelection_SendToLayer(int)) );
}

void ScribusMainWindow::updateColorLists()
{
	if (!HaveDoc)
		return;
	requestUpdate(reqColorsUpdate | reqLineStylesUpdate);
	m_styleManager->updateColorList();
}

bool ScribusMainWindow::slotFileOpen()
{
	PrefsContext* docContext = m_prefsManager->prefsFile->getContext("docdirs", false);
	QString docDir(".");
	QString prefsDocDir(m_prefsManager->documentDir());
	if (!prefsDocDir.isEmpty())
		docDir = docContext->get("docsopen", prefsDocDir);
	else
		docDir = docContext->get("docsopen", ".");
	QString formats(FileLoader::getLoadFilterString());
	QString fileName(CFileDialog( docDir, tr("Open"), formats, "", fdDisableOk));
	if (fileName.isEmpty()) // User cancelled
		return false;
	QFileInfo fi(fileName);
	docContext->set("docsopen", fi.absolutePath());
	return loadDoc(fileName);
}

bool ScribusMainWindow::slotPageImport()
{
	Q_ASSERT(!doc->masterPageMode());
	
	QScopedPointer<MergeDoc> dia(new MergeDoc(this, false, doc->DocPages.count(), doc->currentPage()->pageNr() + 1));
	if (!dia->exec())
		return false;

	bool ret = false;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::ImportPage, Um::IGroup, Um::ImportPage, 0, Um::ILock);

	m_mainWindowStatusLabel->setText( tr("Importing Pages..."));
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	std::vector<int> pageNs;
	parsePagesString(dia->getPageNumbers(), &pageNs, dia->getPageCounter());
	int startPage=0, nrToImport=pageNs.size();
	bool doIt = true;
	if (doc->masterPageMode())
	{
		if (nrToImport > 1)
			loadPage(dia->getFromDoc(), pageNs[0] - 1, false);
		doIt = false;
	}
	else if (dia->getCreatePageChecked())
	{
		int importWhere=dia->getImportWhere();
		if (importWhere == 0)
			startPage = dia->getImportWherePage();
		else if (importWhere == 1)
			startPage = dia->getImportWherePage() + 1;
		else
			startPage = doc->DocPages.count() + 1;
		addNewPages(dia->getImportWherePage(), importWhere, nrToImport, doc->pageHeight(), doc->pageWidth(), doc->pageOrientation(), doc->pageSize(), true);
	}
	else
	{
		startPage = doc->currentPage()->pageNr() + 1;
		if (nrToImport > (doc->DocPages.count() - doc->currentPage()->pageNr()))
		{
			qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));
			ScMessageBox msgBox;
			msgBox.setIcon(QMessageBox::Information);
			msgBox.setText(tr("Import Page(s)"));
			msgBox.setInformativeText("<qt>" +
			QObject::tr("<p>You are trying to import more pages than there are available in the current document counting from the active page.</p>Choose one of the following:"
			"<ul><li><b>Create</b> missing pages</li>"
			"<li><b>Import</b> pages until the last page</li>"
			"<li><b>Cancel</b></li></ul>") + "</qt>");
			QPushButton *createButton = msgBox.addButton(tr("C&reate"), QMessageBox::AcceptRole);
			QPushButton *importButton = msgBox.addButton(tr("&Import"), QMessageBox::AcceptRole);
			QPushButton *cancelButton = msgBox.addButton(CommonStrings::tr_Cancel, QMessageBox::RejectRole);
			msgBox.setDefaultButton(cancelButton);
			msgBox.setDefaultBatchButton(createButton);
			msgBox.exec();
			if (msgBox.clickedButton() == createButton)
			{
				addNewPages(doc->DocPages.count(), 2,
							nrToImport - (doc->DocPages.count() - doc->currentPage()->pageNr()),
							doc->pageHeight(), doc->pageWidth(), doc->pageOrientation(), doc->pageSize(), true);
			}
			else if (msgBox.clickedButton() == importButton)
			{
				nrToImport = doc->DocPages.count() - doc->currentPage()->pageNr();
			}
			else
			{
				doIt = false;
				m_mainWindowStatusLabel->setText("");
			}
			qApp->restoreOverrideCursor();
		}
	}
	if (doIt)
	{
		if (nrToImport > 0)
		{
			mainWindowProgressBar->reset();
			mainWindowProgressBar->setMaximum(nrToImport);
			int counter = startPage;
			for (int i = 0; i < nrToImport; ++i)
			{
				view->GotoPa(counter);
				loadPage(dia->getFromDoc(), pageNs[i] - 1, false);
				counter++;
				mainWindowProgressBar->setValue(i + 1);
			}
			view->GotoPa(startPage);
			mainWindowProgressBar->reset();
			m_mainWindowStatusLabel->setText( tr("Import done"));
		}
		else
		{
			m_mainWindowStatusLabel->setText( tr("Found nothing to import"));
			doIt = false;
		}
	}
	qApp->restoreOverrideCursor();
	ret = doIt;

	if (activeTransaction)
		activeTransaction.commit();
	return ret;
}

bool ScribusMainWindow::loadPage(QString fileName, int Nr, bool Mpa, const QString& renamedPageName)
{
	bool ret = false;
	if (!fileName.isEmpty())
	{
		FileLoader *fl = new FileLoader(fileName);
		if (fl->testFile() == -1)
		{
			delete fl;
			return false;
		}
		doc->setLoading(true);
		int oldItemsCount = doc->Items->count();
		if(!fl->loadPage(doc, Nr, Mpa, renamedPageName))
		{
			delete fl;
			doc->setLoading(false);
			return false;
		}
		delete fl;
		if (ScCore->haveCMS() && doc->cmsSettings().CMSinUse)
		{
			recalcColors();
			doc->RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK);
		}
		int docItemsCount=doc->Items->count();
		for (int i = oldItemsCount; i < docItemsCount; ++i)
		{
			PageItem *ite = doc->Items->at(i);
			if ((ite->asTextFrame()) && (ite->isBookmark))
				AddBookMark(ite);
		}
		propertiesPalette->updateColorList();
		textPalette->updateColorList();
		emit UpdateRequest(reqArrowStylesUpdate | reqLineStylesUpdate | reqStyleComboDocUpdate | reqInlinePalUpdate);
		symbolPalette->updateSymbolList();
		slotDocCh();
		rebuildLayersList();
		updateLayerMenu();
		layerPalette->rebuildList();
		doc->setLoading(false);
		ret = true;
	}
	if (!Mpa)
		pagePalette->Rebuild();
	view->reformPages();
	view->DrawNew();
	return ret;
}

bool ScribusMainWindow::loadDoc(QString fileName)
{
#ifdef DEBUG_LOAD_TIMES
	QTime t;
	struct tms tms1, tms2;
	t.start();
	times(&tms1);
#endif
	
	QFileInfo fi(fileName);
	if (!fi.exists())
	{
		ScMessageBox::warning(this, CommonStrings::trWarning, tr("File does not exist on the specified path :\n%1").arg(QDir::toNativeSeparators(fileName)), QMessageBox::Ok);
		return false;
	}
	
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	if (HaveDoc)
		outlinePalette->buildReopenVals();
	bool ret = false;
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	ScribusWin* ActWinOld = NULL;
	if (windows.count() != 0)
	{
		ActWinOld = ActWin;
	}

	// PV - 5780: Scribus doesn't track what documents are already opened
	// The goal of this part of code is to disallow user to open one
	// doc multiple times.
	QString FName = fi.absoluteFilePath();
	QString platfName(QDir::toNativeSeparators(FName));
	int windowCount = windows.count();
	for (int i = 0; i < windowCount; ++i)
	{
		QString docNameUnmodified(windows.at(i)->windowTitle());
		ScribusWin * mx = qobject_cast<ScribusWin*>(windows.at(i)->widget());
		if (mx && mx->doc()->isModified() && docNameUnmodified.endsWith("*"))
			docNameUnmodified.resize(docNameUnmodified.length() - 1);

		if (docNameUnmodified == platfName)
		{
			qApp->restoreOverrideCursor();
			ScMessageBox::information(this, tr("Document is already opened"),
			                         tr("This document is already in use."
			                            "You'll be switched into its window now."));
			windowsMenuActivated(i);
			return true;
		}
	}
	UndoBlocker undoBlocker;
	if (!fileName.isEmpty())
	{
		FileLoader *fileLoader = new FileLoader(FName);
		int testResult = fileLoader->testFile();
		if (testResult == -1)
		{
			delete fileLoader;
			qApp->restoreOverrideCursor();
			QString title = tr("Fatal Error") ;
			QString msg = "<qt>"+ tr("File %1 is not in an acceptable format").arg(FName)+"</qt>";
			QString infoMsg = "<qt>" + tr("The file may be damaged or may have been produced in a later version of Scribus.") + "</qt>";
			ScMessageBox msgBox(QMessageBox::Critical, title, msg, QMessageBox::Ok | QMessageBox::Help, this);
			msgBox.setInformativeText(infoMsg);
			int i=msgBox.exec();
			if (i==QMessageBox::Help)
					slotOnlineHelp("", "fileproblems.html");
			return false;
		}
		bool is12doc=false;
		if (testResult == 0)
		{
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			//Scribus 1.3.x warning, remove at a later stage
			is12doc=true;
		}
		QDir docProfileDir(fi.absolutePath() + "/profiles");
		ScCore->getCMSProfilesDir(fi.absolutePath()+"/", false, false);
		if (docProfileDir.exists())
			ScCore->getCMSProfilesDir(fi.absolutePath()+"/profiles", false, false);

		m_prefsManager->appPrefs.fontPrefs.AvailFonts.AddScalableFonts(fi.absolutePath()+"/", FName);
		QDir docFontDir(fi.absolutePath() + "/fonts");
		if (docFontDir.exists())
			m_prefsManager->appPrefs.fontPrefs.AvailFonts.AddScalableFonts(fi.absolutePath()+"/fonts", FName);
		QDir docFontDir2(fi.absolutePath() + "/Fonts");
		if (docFontDir2.exists())
			m_prefsManager->appPrefs.fontPrefs.AvailFonts.AddScalableFonts(fi.absolutePath()+"/Fonts", FName);
		QDir docFontDir3(fi.absolutePath() + "/Document fonts");
		if (docFontDir3.exists())
			m_prefsManager->appPrefs.fontPrefs.AvailFonts.AddScalableFonts(fi.absolutePath()+"/Document fonts", FName);
		m_prefsManager->appPrefs.fontPrefs.AvailFonts.updateFontMap();
		if (view != NULL)
		{
			actionManager->disconnectNewViewActions();
			disconnect(view, SIGNAL(signalGuideInformation(int, qreal)), alignDistributePalette, SLOT(setGuide(int, qreal)));
		}
		doc=new ScribusDoc();
		doc->saveFilePermissions(QFile::permissions(fileName));
		doc->is12doc=is12doc;
		doc->appMode = modeNormal;
		doc->HasCMS = false;
		doc->OpenNodes.clear();
		doc->setLoading(true);
		m_mainWindowStatusLabel->setText( tr("Loading..."));
		mainWindowProgressBar->reset();
		ScribusWin* w = new ScribusWin(mdiArea, doc);
		w->setMainWindow(this);
		view = new ScribusView(w, this, doc);
		doc->setGUI(true, this, view);
		view->setScale(m_prefsManager->displayScale());
		w->setView(view);
		alignDistributePalette->setDoc(doc);
		ActWin = w;
		doc->WinHan = w;
		w->setSubWin(mdiArea->addSubWindow(w));
		w->setUpdatesEnabled(false);
		view->updatesOn(false);
		doc->SoftProofing = false;
		doc->Gamut = false;
		setScriptRunning(true);
		bool loadSuccess = fileLoader->loadFile(doc);
		//Do the font replacement check from here, when we have a GUI. TODO do this also somehow without the GUI
		//This also gives the user the opportunity to cancel the load when finding theres a replacement required.
		if (loadSuccess && ScCore->usingGUI())
			loadSuccess = fileLoader->postLoad(doc);
		if (!loadSuccess)
		{
			view->close();
			delete fileLoader;
			delete doc;
			doc=NULL;
			mdiArea->removeSubWindow(w->getSubWin());
			delete w;
			view=NULL;
			doc=NULL;
			setScriptRunning(false);
			qApp->restoreOverrideCursor();
			m_mainWindowStatusLabel->setText("");
			mainWindowProgressBar->reset();
			ActWin = NULL;
			if (windows.count() != 0)
			{
				newActWin(ActWinOld->getSubWin());
				if (ActWin)
				{
					if ((mdiArea->subWindowList().isEmpty()) || (mdiArea->subWindowList().count() == 1))
						ActWin->showMaximized();
				}
			}
			return false;
		}
		symbolPalette->setDoc(doc);
		outlinePalette->setDoc(doc);
		fileLoader->informReplacementFonts();
		setCurrentComboItem(unitSwitcher, unitGetStrFromIndex(doc->unitIndex()));
		bool b = zoomSpinBox->blockSignals(true);
		zoomSpinBox->setValue(view->scale());
		zoomSpinBox->blockSignals(b);
		view->unitChange();
		setScriptRunning(false);
		view->Deselect(true);
		m_mainWindowStatusLabel->setText("");
		mainWindowProgressBar->reset();
		HaveDoc++;
		if (doc->checkerProfiles().count() == 0)
		{
			m_prefsManager->initDefaultCheckerPrefs(doc->checkerProfiles());
			doc->setCurCheckProfile(CommonStrings::PDF_1_4);
		}
		m_prefsManager->insertMissingCheckerProfiles(doc->checkerProfiles());
		if (doc->pdfOptions().LPISettings.count() == 0)
		{
			struct LPIData lpo;
			lpo.Frequency = 133;
			lpo.SpotFunc = 3;
			lpo.Angle = 105;
			doc->pdfOptions().LPISettings.insert("Cyan", lpo);
			lpo.Angle = 75;
			doc->pdfOptions().LPISettings.insert("Magenta", lpo);
			lpo.Angle = 90;
			doc->pdfOptions().LPISettings.insert("Yellow", lpo);
			lpo.Angle = 45;
			doc->pdfOptions().LPISettings.insert("Black", lpo);
		}
		if (!doc->cmsSettings().CMSinUse)
			doc->HasCMS = false;
		if ((ScCore->haveCMS()) && (doc->cmsSettings().CMSinUse))
		{
			QString missing, replacement;
			QMultiMap<QString, QString> missingMap;
			if (!ScCore->InputProfiles.contains(doc->cmsSettings().DefaultImageRGBProfile))
			{
				missing = doc->cmsSettings().DefaultImageRGBProfile;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->cmsSettings().DefaultImageRGBProfile = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile;
			}
			if (!ScCore->InputProfilesCMYK.contains(doc->cmsSettings().DefaultImageCMYKProfile))
			{
				missing = doc->cmsSettings().DefaultImageCMYKProfile;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultImageCMYKProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->cmsSettings().DefaultImageCMYKProfile = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultImageCMYKProfile;
			}
			if (!ScCore->InputProfiles.contains(doc->cmsSettings().DefaultSolidColorRGBProfile))
			{
				missing = doc->cmsSettings().DefaultSolidColorRGBProfile;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->cmsSettings().DefaultSolidColorRGBProfile = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
			}
			if (!ScCore->InputProfilesCMYK.contains(doc->cmsSettings().DefaultSolidColorCMYKProfile))
			{
				missing = doc->cmsSettings().DefaultSolidColorCMYKProfile;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->cmsSettings().DefaultSolidColorCMYKProfile = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile;
			}
			if (!ScCore->MonitorProfiles.contains(doc->cmsSettings().DefaultMonitorProfile))
			{
				missing = doc->cmsSettings().DefaultMonitorProfile;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->cmsSettings().DefaultMonitorProfile = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultMonitorProfile;
			}
			if (!ScCore->PrinterProfiles.contains(doc->cmsSettings().DefaultPrinterProfile))
			{
				missing = doc->cmsSettings().DefaultPrinterProfile;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->cmsSettings().DefaultPrinterProfile = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile;
			}
			if (!ScCore->PrinterProfiles.contains(doc->pdfOptions().PrintProf))
			{
				missing = doc->pdfOptions().PrintProf;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultPrinterProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->pdfOptions().PrintProf = doc->cmsSettings().DefaultPrinterProfile;
			}
			if (!ScCore->InputProfiles.contains(doc->pdfOptions().ImageProf))
			{
				missing = doc->pdfOptions().ImageProf;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultImageRGBProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->pdfOptions().ImageProf = doc->cmsSettings().DefaultImageRGBProfile;
			}
			if (!ScCore->InputProfiles.contains(doc->pdfOptions().SolidProf))
			{
				missing = doc->pdfOptions().SolidProf;
				replacement = m_prefsManager->appPrefs.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
				if (!missingMap.contains(missing, replacement))
					missingMap.insert(missing, replacement);
				doc->pdfOptions().SolidProf = doc->cmsSettings().DefaultSolidColorRGBProfile;
			}
			if (missingMap.count() > 0)
			{
				QMultiMap<QString, QString>::const_iterator it;
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				QString mess = tr("Some color profiles used by this document are not installed:")+"\n\n";
				for (it = missingMap.begin(); it != missingMap.end(); ++it)
				{
					mess += it.key() + tr(" was replaced by: ") + it.value() +"\n";
				}
				ScMessageBox::warning(this, CommonStrings::trWarning, mess);
			}
			doc->SoftProofing = doc->cmsSettings().SoftProofOn;
			doc->Gamut        = doc->cmsSettings().GamutCheck;
			doc->IntentColors = doc->cmsSettings().DefaultIntentColors;
			doc->IntentImages = doc->cmsSettings().DefaultIntentImages;
			if (doc->OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles))
			{
				doc->HasCMS = true;
				doc->pdfOptions().SComp = doc->cmsSettings().ComponentsInput2;
			}
			else
			{
				doc->SetDefaultCMSParams();
				doc->HasCMS = false;
			}
			if (doc->HasCMS)
			{
				recalcColors();
				doc->RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK);
			}
		}
		else
		{
			doc->cmsSettings().CMSinUse = false;
		}
		if (fileLoader->fileType() > FORMATID_NATIVEIMPORTEND)
		{
			doc->setName(FName+ tr("(converted)"));
			QFileInfo fi(doc->DocName);
			doc->setName(fi.fileName());
			doc->isConverted = true;
		}
		else
			doc->setName(FName);
		doc->setMasterPageMode(false);
		doc->createHyphenator();
		HaveNewDoc();
		doc->hasName = true;
		if (doc->MasterPages.count() == 0)
		{
			ScPage *docPage = doc->Pages->at(0);
			ScPage *addedPage = doc->addMasterPage(0, CommonStrings::masterPageNormal);
			addedPage->m_pageSize = docPage->m_pageSize;
			addedPage->setInitialHeight(docPage->height());
			addedPage->setInitialWidth(docPage->width());
			addedPage->setHeight(docPage->height());
			addedPage->setWidth(docPage->width());
			addedPage->initialMargins = docPage->initialMargins;
			addedPage->LeftPg = docPage->LeftPg;
			addedPage->setOrientation(docPage->orientation());
		}
		//Add doc sections if we have none
		if (doc->sections().count()==0)
		{
			doc->addSection(-1);
			doc->setFirstSectionFromFirstPageNumber();
		}
		doc->RePos = true;
		doc->setMasterPageMode(true);
		doc->reformPages();
		doc->refreshGuides();
		doc->setLoading(false);
		for (QList<PageItem*>::iterator itm = doc->MasterItems.begin(); itm != doc->MasterItems.end(); ++itm)
		{
			PageItem* ite = *itm;
			// TODO fix that for Groups on Masterpages
//			if (ite->Groups.count() != 0)
//				view->GroupOnPage(ite);
//			qDebug() << QString("load M: %1 %2 %3").arg(azz).arg((uint)ite).arg(ite->itemType());
			ite->layout();
		}
		doc->setMasterPageMode(false);
		/*QTime t;
		t.start();*/
		doc->flag_Renumber = false;
		for (QList<PageItem*>::iterator iti = doc->Items->begin(); iti != doc->Items->end(); ++iti)
		{
			PageItem* ite = *iti;
			if((ite->nextInChain() == NULL) && !ite->isNoteFrame())  //do not layout notes frames
				ite->layout();
		}
		if (!doc->marksList().isEmpty())
		{
			doc->setLoading(true);
			doc->updateMarks(true);
			doc->setLoading(false);
		}
		for (QHash<int, PageItem*>::iterator itf = doc->FrameItems.begin(); itf != doc->FrameItems.end(); ++itf)
		{
			PageItem *ite = itf.value();
//			qDebug() << QString("load F: %1 %2 %3").arg(azz).arg((uint)ite).arg(ite->itemType());
			if(ite->nextInChain() == NULL)
				ite->layout();
		}
		/*qDebug("Time elapsed: %d ms", t.elapsed());*/
		doc->RePos = false;
		doc->setModified(false);
		inlinePalette->setDoc(doc);
		updateRecent(FName);
		m_mainWindowStatusLabel->setText( tr("Ready"));
		ret = true;
		doc->setLoading(true);
		for (int p = 0; p < doc->DocPages.count(); ++p)
		{
			Apply_MasterPage(doc->DocPages.at(p)->MPageNam, p, false);
		}
		view->reformPages(false);
		doc->setLoading(false);
		delete fileLoader;
		view->updatesOn(true);
		w->setUpdatesEnabled(true);
		disconnect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(newActWin(QMdiSubWindow *)));
		if ((mdiArea->subWindowList().isEmpty()) || (mdiArea->subWindowList().count() == 1))
			w->showMaximized();
		else
			w->show();
		view->show();
		// Seems to fix crash on loading
		ActWin = NULL;
		newActWin(w->getSubWin());
		doc->updateNumbers(true);
		emit UpdateRequest(reqNumUpdate);
		doc->setCurrentPage(doc->DocPages.at(0));
		scrActions["viewToggleCMS"]->setChecked(doc->HasCMS);
		view->zoom();
		view->GotoPage(0);
		connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(newActWin(QMdiSubWindow *)));
		connect(ScCore->fileWatcher, SIGNAL(fileChanged(QString )), doc, SLOT(updatePict(QString)));
		connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), doc, SLOT(removePict(QString)));
		connect(ScCore->fileWatcher, SIGNAL(dirChanged(QString )), doc, SLOT(updatePictDir(QString )));
		connect(m_undoManager, SIGNAL(undoRedoBegin()), doc, SLOT(undoRedoBegin()));
		connect(m_undoManager, SIGNAL(undoRedoDone()), doc, SLOT(undoRedoDone()));
		connect(m_undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
		doc->connectDocSignals();
		if (doc->autoSave())
			doc->autoSaveTimer->start(doc->autoSaveTime());
		connect(doc, SIGNAL(updateAutoSaveClock()), view->clockLabel, SLOT(resetTime()));
		view->clockLabel->resetTime();
		doc->NrItems = bookmarkPalette->BView->NrItems;
		doc->First = bookmarkPalette->BView->First;
		doc->Last = bookmarkPalette->BView->Last;
		if (doc->drawAsPreview)
			view->togglePreview(true);
	}
	else
	{
		pagePalette->setView(0);
	}

	m_undoManager->switchStack(doc->DocName);
	pagePalette->Rebuild();
	qApp->restoreOverrideCursor();
	doc->setModified(false);
	foreach (NotesStyle* NS, doc->m_docNotesStylesList)
		doc->updateNotesFramesStyles(NS);
#ifdef DEBUG_LOAD_TIMES
	times(&tms2);
	double ticks = sysconf(_SC_CLK_TCK);
	double user  = (tms2.tms_utime - tms1.tms_utime)/ticks;
	double sys   = (tms2.tms_stime - tms1.tms_stime)/ticks;
	double cuser = (tms2.tms_cutime - tms1.tms_cutime)/ticks;
	double csys  = (tms2.tms_cstime - tms1.tms_cstime)/ticks;
	qDebug("loaded document in %.3f seconds (%.3f user + %.3f sys = %.3f sec, child %.3f user + %.3f sys = %.3f sec)",
		t.elapsed()/1000.0, user, sys, user + sys, cuser, csys, cuser + csys);
#endif
	return ret;
}

bool ScribusMainWindow::postLoadDoc()
{
	//FIXME Just return for now, if we arent using the GUI
	if (!ScCore->usingGUI())
		return false;
	return true;
}

// This method was once named slotFileOpen(...) but it hasn't had anything to
// do with file->open for a LONG time. It's used for get text / get picture.
void ScribusMainWindow::slotGetContent()
{
	if (!doc->m_Selection->isEmpty())
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
		if (currItem->itemType() == PageItem::ImageFrame)
		{
			QString formatD(FormatsManager::instance()->fileDialogFormatList(FormatsManager::IMAGESIMGFRAME));

			QString docDir = ".";
			QString prefsDocDir=m_prefsManager->documentDir();
			if (!prefsDocDir.isEmpty())
				docDir = m_prefsManager->prefsFile->getContext("dirs")->get("images", prefsDocDir);
			else
				docDir = m_prefsManager->prefsFile->getContext("dirs")->get("images", ".");

			QStringList fileNames;
			fileNames.clear();
			CustomFDialog *dia = new CustomFDialog(qApp->activeWindow(), docDir, tr("Open"), formatD, fdShowPreview | fdExistingFilesI | fdDisableOk);
			if (dia->exec() == QDialog::Accepted)
				fileNames = dia->fileDialog->selectedFiles();
			delete dia;
			//QStringList fileNames = CFileDialog( docDir, tr("Open"), formatD, "", fdShowPreview | fdExistingFiles);
			if (!fileNames.isEmpty())
			{
				m_prefsManager->prefsFile->getContext("dirs")->set("images", fileNames[0].left(fileNames[0].lastIndexOf("/")));
				view->requestMode(modeImportImage);
				CanvasMode_ImageImport* cii=dynamic_cast<CanvasMode_ImageImport*>(view->canvasMode());
				if (cii)
					cii->setImageList(fileNames);
			}
		}
		else if (currItem->asTextFrame())
		{
			gtGetText* gt = new gtGetText(doc);
			ImportSetup impsetup=gt->run();
			if (impsetup.runDialog)
			{
				if (currItem->itemText.length() != 0)
				{
					int t = ScMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your text?"),
								QMessageBox::Yes | QMessageBox::No,
								QMessageBox::No,	// GUI default
								QMessageBox::Yes);	// batch default
					if (t == QMessageBox::No)
					{
						delete gt;
						return;
					}
				}
				gt->launchImporter(impsetup.importer, impsetup.filename, impsetup.textOnly, impsetup.encoding, false, impsetup.prefixNames);
			}
			delete gt;
			if (doc->docHyphenator->AutoCheck)
				doc->docHyphenator->slotHyphenate(currItem);
			for (int a = 0; a < doc->Items->count(); ++a)
			{
				if (doc->Items->at(a)->isBookmark)
					bookmarkPalette->BView->changeText(doc->Items->at(a));
			}
			if (!impsetup.textOnly)
				doc->flag_NumUpdateRequest = true;
			view->DrawNew();
			slotDocCh();
			m_styleManager->setDoc(doc);
			marksManager->setDoc(doc);
			nsEditor->setDoc(doc);
			inlinePalette->unsetDoc();
			inlinePalette->setDoc(doc);
			if (outlinePalette->isVisible())
				outlinePalette->BuildTree();
			propertiesPalette->updateColorList();
			textPalette->updateColorList();
			emit UpdateRequest(reqArrowStylesUpdate | reqLineStylesUpdate | reqStyleComboDocUpdate | reqInlinePalUpdate);
			symbolPalette->updateSymbolList();
		}
	}
}

void ScribusMainWindow::updateFromDrop()
{
	m_styleManager->setDoc(doc);
	marksManager->setDoc(doc);
	nsEditor->setDoc(doc);
	inlinePalette->unsetDoc();
	inlinePalette->setDoc(doc);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	propertiesPalette->updateColorList();
	textPalette->updateColorList();
	emit UpdateRequest(reqArrowStylesUpdate | reqLineStylesUpdate | reqStyleComboDocUpdate | reqInlinePalUpdate);
	symbolPalette->updateSymbolList();
}

void ScribusMainWindow::slotGetContent2() // kk2006
{
	if (doc->m_Selection->isEmpty())
		return; // nothing to do, no selection

	PageItem *currItem = doc->m_Selection->itemAt(0);

	if (!currItem->asTextFrame())
		return; // not a text frame

	ScGTPluginManager::instance()->run();
	if (doc->docHyphenator->AutoCheck)
		doc->docHyphenator->slotHyphenate(currItem);
	for (int a = 0; a < doc->Items->count(); ++a)
	{
		if (doc->Items->at(a)->isBookmark)
			bookmarkPalette->BView->changeText(doc->Items->at(a));
	}
	view->DrawNew();
	slotDocCh();
}

void ScribusMainWindow::slotGetClipboardImage()
{
	if (HaveDoc && (!doc->m_Selection->isEmpty()) && (QApplication::clipboard()->mimeData()->hasImage()))
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
		if (currItem->itemType() == PageItem::ImageFrame)
		{
			int t = QMessageBox::Yes;
			if (currItem->imageIsAvailable)
				t = ScMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to replace your existing image?"),
							QMessageBox::Yes | QMessageBox::No,
							QMessageBox::No,	// GUI default
							QMessageBox::Yes);	// batch default
			if (t == QMessageBox::Yes)
			{
				QImage img = QApplication::clipboard()->image();
				if (!img.isNull())
				{
					currItem->EmProfile = "";
					currItem->pixm.imgInfo.isRequest = false;
					currItem->UseEmbedded = true;
					currItem->IProfile = doc->cmsSettings().DefaultImageRGBProfile;
					currItem->IRender = doc->cmsSettings().DefaultIntentImages;
					qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
					qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
					QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX.png");
					tempFile->setAutoRemove(false);
					tempFile->open();
					QString fileName = getLongPathName(tempFile->fileName());
					tempFile->close();
					delete tempFile;
					currItem->isInlineImage = true;
					currItem->isTempFile = true;
					currItem->Pfile = fileName;
					img.save(fileName, "PNG");
					doc->loadPict(fileName, currItem, false, true);
					propertiesPalette->imagePal->showScaleAndOffset(currItem->imageXScale(), currItem->imageYScale(), currItem->imageXOffset(), currItem->imageYOffset());
					qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
					view->DrawNew();
					emit UpdateRequest(reqColorsUpdate | reqCmsOptionsUpdate);
					currItem->emitAllToGUI();
					qApp->restoreOverrideCursor();
				}
			}
		}
	}
}

void ScribusMainWindow::toogleInlineState()
{
	if (!HaveDoc)
		return;
	if (doc->m_Selection->isEmpty())
		return;
	PageItem *currItem = doc->m_Selection->itemAt(0);
	if (currItem->itemType() != PageItem::ImageFrame)
		return;
	if (!currItem->imageIsAvailable)
		return;
	if (currItem->isImageInline())
	{
		QFileInfo fiB(currItem->Pfile);

		PrefsContext* docContext = m_prefsManager->prefsFile->getContext("docdirs", false);
		QString wdir = ".";
		if (doc->hasName)
		{
			QFileInfo fi(doc->DocName);
			wdir = QDir::fromNativeSeparators( fi.path() );
		}
		else
		{
			QString prefsDocDir = m_prefsManager->documentDir();
			if (!prefsDocDir.isEmpty())
				wdir = docContext->get("place_as", prefsDocDir);
			else
				wdir = docContext->get("place_as", ".");
			wdir = QDir::fromNativeSeparators( wdir );
		}
		QString fileName = CFileDialog(wdir, tr("Filename and Path for Image"), tr("All Files (*)"), fiB.fileName(), fdHidePreviewCheckBox);
		if (!fileName.isEmpty())
		{
			if (ScCore->fileWatcher->files().contains(currItem->Pfile) != 0)
				ScCore->fileWatcher->removeFile(currItem->Pfile);
			docContext->set("place_as", fileName.left(fileName.lastIndexOf("/")));
			if (overwrite(this, fileName))
			{
				currItem->makeImageExternal(fileName);
				ScCore->fileWatcher->addFile(currItem->Pfile);
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				doc->loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
			}
		}
	}
	else
	{
		if (ScCore->fileWatcher->files().contains(currItem->Pfile) != 0)
			ScCore->fileWatcher->removeFile(currItem->Pfile);
		currItem->makeImageInline();
		ScCore->fileWatcher->addFile(currItem->Pfile);
		bool fho = currItem->imageFlippedH();
		bool fvo = currItem->imageFlippedV();
		doc->loadPict(currItem->Pfile, currItem, true);
		currItem->setImageFlippedH(fho);
		currItem->setImageFlippedV(fvo);
	}
	scrActions["itemToggleInlineImage"]->setChecked(currItem->isImageInline());
}

void ScribusMainWindow::slotFileAppend()
{
	if (!doc->m_Selection->isEmpty())
	{
		gtGetText* gt = new gtGetText(doc);
		ImportSetup impsetup=gt->run();
		if (impsetup.runDialog)
		{
			gt->launchImporter(impsetup.importer, impsetup.filename, impsetup.textOnly, impsetup.encoding, true, impsetup.prefixNames);
		}
		delete gt;
		//CB Hyphenating now emits doc changed, plus we change lang as appropriate
		if (doc->docHyphenator->AutoCheck)
			doc->itemSelection_DoHyphenate();
		view->DrawNew();
		//slotDocCh();
	}
}

void ScribusMainWindow::slotFileRevert()
{
	if ((doc->hasName) && (doc->isModified()) && (!doc->masterPageMode()) && (!doc->isConverted))
	{
		ScribusWin* tw = ActWin;
		int t = ScMessageBox::warning(this, CommonStrings::trWarning, "<qt>" +
								 QObject::tr("The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?") + "</qt>",
								 QMessageBox::Yes | QMessageBox::No,
								 QMessageBox::No,	// GUI default
								 QMessageBox::Yes);	// batch default
		if (t == QMessageBox::No)
			return;

		mdiArea->setActiveSubWindow(tw->getSubWin());
		ActWin = tw;
		QString fn(doc->DocName);
		doc->setModified(false);
		if (doc==storyEditor->currentDocument())
			storyEditor->close();
		slotFileClose();
		qApp->processEvents();
		loadDoc(fn);
		m_undoManager->clearStack();
	}
}

bool ScribusMainWindow::slotFileSave()
{
	bool ret = false;
	if ((doc->hasName) && (!doc->isConverted))
	{
		//Scribus 1.3.x warning, remove at a later stage
		if (doc->is12doc && !warningVersion(this))
			return false;

		QString fn(doc->DocName), savedFileName;
		ret = DoFileSave(fn, &savedFileName);
		if (!ret && !savedFileName.isEmpty())
			ScMessageBox::warning(this, CommonStrings::trWarning, tr("Your document was saved to a temporary file and could not be moved: \n%1").arg( QDir::toNativeSeparators(savedFileName) ));
		else if (!ret)
			ScMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg( QDir::toNativeSeparators(fn) ));
	}
	else
		ret = slotFileSaveAs();
	return ret;
}

bool ScribusMainWindow::slotFileSaveAs()
{
	//Scribus 1.3.x warning, remove at a later stage
	if (doc->is12doc)
		if (!warningVersion(this))
			return false;
	//Turn off the warnings once the docs is saved.
	doc->is12doc=false;
	bool ret = false;
	QString filename;
	PrefsContext* docContext = m_prefsManager->prefsFile->getContext("docdirs", false);
	QString wdir = ".";
	if (doc->hasName)
	{
		QFileInfo fi(doc->DocName);
		QString completeBaseName = fi.completeBaseName();
		if (completeBaseName.endsWith(".sla", Qt::CaseInsensitive))
			completeBaseName.chop(4);
		else if (completeBaseName.endsWith(".gz", Qt::CaseInsensitive))
			completeBaseName.chop(3);
		wdir = QDir::fromNativeSeparators( fi.path() );
		filename  = QDir::fromNativeSeparators( fi.path()+"/"+completeBaseName+".sla" );
	}
	else
	{
		QString prefsDocDir=m_prefsManager->documentDir();
		if (!prefsDocDir.isEmpty())
			wdir = docContext->get("save_as", prefsDocDir);
		else
			wdir = docContext->get("save_as", ".");
		wdir = QDir::fromNativeSeparators( wdir );
		if (wdir.right(1) != "/")
			filename = wdir + "/";
		else
			filename = wdir;
		filename += doc->DocName + ".sla";
	}
	bool saveCompressed=m_prefsManager->appPrefs.docSetupPrefs.saveCompressed;
	if (saveCompressed)
		filename.append(".gz");

	QString fileSpec = tr("Documents (*.sla *.sla.gz);;All Files (*)");
	int optionFlags = fdCompressFile | fdHidePreviewCheckBox;
	QString fn = CFileDialog( wdir, tr("Save As"), fileSpec, filename, optionFlags, &saveCompressed);
	if (!fn.isEmpty())
	{
		docContext->set("save_as", fn.left(fn.lastIndexOf("/")));
		if ((fn.endsWith(".sla")) || (fn.endsWith(".sla.gz")))
			filename = fn;
		else
			filename = fn+".sla";
		if (overwrite(this, filename))
		{
			QString savedFileName;
			ret = DoFileSave(filename, &savedFileName);
			if (!ret && !savedFileName.isEmpty())
				ScMessageBox::warning(this, CommonStrings::trWarning, tr("Your document was saved to a temporary file and could not be moved: \n%1").arg( QDir::toNativeSeparators(savedFileName) ));
			else if (!ret)
				ScMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg( QDir::toNativeSeparators(fn) ));
			else
				doc->pdfOptions().fileName = ""; // #1482 reset the pdf file name
		}
	}
	m_mainWindowStatusLabel->setText( tr("Ready"));
	return ret;
}

bool ScribusMainWindow::DoFileSave(const QString& fileName, QString* savedFileName)
{
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	doc->reorganiseFonts();
	m_mainWindowStatusLabel->setText( tr("Saving..."));
	mainWindowProgressBar->reset();
	bool ret=doc->save(fileName, savedFileName);
	qApp->processEvents();
	if (ret)
	{
		updateActiveWindowCaption(fileName);
		m_undoManager->renameStack(fileName);
		scrActions["fileRevert"]->setEnabled(false);
		updateRecent(fileName);
	}
	m_mainWindowStatusLabel->setText("");
	mainWindowProgressBar->reset();
	ScCore->fileWatcher->start();
	return ret;
}

bool ScribusMainWindow::slotFileClose()
{
	ScribusWin* tw = ActWin;
	mdiArea->closeActiveSubWindow();
	windowsMenuAboutToShow();
	return (tw != ActWin);
}

bool ScribusMainWindow::DoFileClose()
{
	slotEndSpecialEdit();
	view->Deselect(false);
	if (doc==storyEditor->currentDocument())
		storyEditor->close();
	actionManager->disconnectNewDocActions();
	actionManager->disconnectNewViewActions();
	disconnect(view, SIGNAL(signalGuideInformation(int, qreal)), alignDistributePalette, SLOT(setGuide(int, qreal)));
	m_undoManager->removeStack(doc->DocName);
	closeActiveWindowMasterPageEditor();
	slotSelect();
	doc->autoSaveTimer->stop();
	doc->disconnectDocSignals();
	disconnect(ScCore->fileWatcher, SIGNAL(fileChanged(QString )), doc, SLOT(updatePict(QString)));
	disconnect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), doc, SLOT(removePict(QString)));
	disconnect(ScCore->fileWatcher, SIGNAL(dirChanged(QString )), doc, SLOT(updatePictDir(QString )));
	if (ScCore->haveCMS())
		doc->CloseCMSProfiles();
	//<<Palettes
	propertiesPalette->unsetDoc();
	textPalette->unsetDoc();
	inlinePalette->unsetDoc();
	symbolPalette->unsetDoc();
	pagePalette->setView(0);
	pagePalette->Rebuild();
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	bookmarkPalette->BView->clear();
	bookmarkPalette->BView->NrItems = 0;
	bookmarkPalette->BView->First = 1;
	bookmarkPalette->BView->Last = 0;
	outlinePalette->unsetDoc();
	alignDistributePalette->setDoc(NULL);
	//>>

	if ((mdiArea->subWindowList().isEmpty()) || (mdiArea->subWindowList().count() == 1))
	{
		PluginManager& pluginManager(PluginManager::instance());
		pluginManager.enableOnlyStartupPluginActions(this);
		appModeHelper->mainWindowCloseLastDoc();
		m_mainWindowStatusLabel->setText( tr("Ready"));
		m_PrinterUsed = false;
	}
	// Give plugins a chance to react on closing the document
	PluginManager& pluginManager(PluginManager::instance());
	QStringList pluginNames(pluginManager.pluginNames(false));
	ScPlugin* plugin;
	QString pName;
	for (int i = 0; i < pluginNames.count(); ++i)
	{
		pName = pluginNames.at(i);
		plugin = pluginManager.getPlugin(pName, true);
		Q_ASSERT(plugin); // all the returned names should represent loaded plugins
		plugin->unsetDoc();
	}
	view->close();
	//CB Yes, we are setting it to NULL without deleting it. ActWin(ScribusWin) owns the view
	//due to it being the central widget and will delete it at the correct moment from its own pointer.
	view = NULL;
	doc->setLoading(true);
	guidePalette->setDoc(0);
	charPalette->setDoc(0);
	m_tocGenerator->setDoc(0);
	m_styleManager->setDoc(0);
	marksManager->setDoc(0);
	nsEditor->setDoc(0);
	layerPalette->ClearInhalt();
	docCheckerPalette->buildErrorList(0);
	HaveDoc--;
	delete doc;
	doc = NULL;
	ActWin = NULL;
	if ( HaveDoc == 0 )
	{
		QString prefsDocDir( PrefsManager::instance()->documentDir() );
		if ( QDir().exists(prefsDocDir) )
			QDir::setCurrent( PrefsManager::instance()->documentDir() );
		else
			QDir::setCurrent( QDir::homePath() );
	}
	pageSelector->disconnect();
	pageSelector->setMaximum(1);
	pageSelector->setEnabled(false);
	updateLayerMenu();
	updateTableMenuActions();
	rebuildScrapbookMenu();
	mainWindowXPosDataLabel->clear();
	mainWindowYPosDataLabel->clear();
	//not running view's togglePreview as we don't want to affect the doc settings.
	scrActions["viewPreviewMode"]->setChecked(false);
	appModeHelper->setPreviewMode(false);
	return true;
}

void ScribusMainWindow::slotFilePrint()
{
	if (doc->checkerProfiles()[doc->curCheckProfile()].autoCheck)
	{
		if (scanDocument())
		{
			if (doc->checkerProfiles()[doc->curCheckProfile()].ignoreErrors)
			{
				int t = ScMessageBox::warning(this, CommonStrings::trWarning,
											"<qt>"+ tr("Scribus has detected some errors. Consider using the Preflight Verifier to correct them")+"</qt>",
											QMessageBox::Abort | QMessageBox::Ignore,
											QMessageBox::NoButton,	// GUI default
											QMessageBox::Ignore);	// batch default
				if (t == QMessageBox::Abort)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(slotReallyPrint()));
				docCheckerPalette->setIgnoreEnabled(true);
				docCheckerPalette->checkMode = CheckDocument::checkPrint;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setChecked(true);
				return;
			}
		}
	}
	slotReallyPrint();
}

void ScribusMainWindow::slotReallyPrint()
{
	bool done;
	if (docCheckerPalette->isIgnoreEnabled())
	{
		docCheckerPalette->setIgnoreEnabled(false);
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = CheckDocument::checkNULL;
		scrActions["toolsPreflightVerifier"]->setChecked(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(slotReallyPrint()));
	}
	QString printError;
	PrintOptions options;
	m_mainWindowStatusLabel->setText( tr("Printing..."));
	if (doc->Print_Options.firstUse)
	{
		doc->Print_Options.printer.clear();
		if (!doc->DocName.startsWith( tr("Document")))
		{
			QFileInfo fi(doc->DocName);
			QString completeBaseName = fi.completeBaseName();
			if (completeBaseName.endsWith(".sla", Qt::CaseInsensitive))
				if (completeBaseName.length() > 4) completeBaseName.chop(4);
			if (completeBaseName.endsWith(".gz", Qt::CaseInsensitive))
				if (completeBaseName.length() > 3) completeBaseName.chop(3);
			doc->Print_Options.filename = fi.path()+"/"+completeBaseName+".ps";
		}
		else
		{
			QDir di = QDir();
			doc->Print_Options.filename = di.currentPath()+"/"+doc->DocName+".ps";
		}
	}
	doc->Print_Options.copies = 1;
	ColorList usedSpots;
	doc->getUsedColors(usedSpots, true);
	QStringList spots = usedSpots.keys();
	PrintDialog *printer = new PrintDialog(this, doc, doc->Print_Options, m_prefsManager->appPrefs.printerPrefs.GCRMode, spots);
	printer->setMinMax(1, doc->Pages->count(), doc->currentPage()->pageNr()+1);
	printDinUse = true;
	connect(printer, SIGNAL(doPreview()), this, SLOT(doPrintPreview()));
	if (printer->exec())
	{
		ReOrderText(doc, view);
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
		doc->Print_Options.pageNumbers.clear();
		if (printer->doPrintCurrentPage())
			doc->Print_Options.pageNumbers.push_back(doc->currentPage()->pageNr()+1);
		else
		{
			if (printer->doPrintAll())
				parsePagesString("*", &doc->Print_Options.pageNumbers, doc->DocPages.count());
			else
				parsePagesString(printer->getPageString(), &doc->Print_Options.pageNumbers, doc->DocPages.count());
		}
		m_PrinterUsed = true;
		done = doPrint(doc->Print_Options, printError);
		qApp->restoreOverrideCursor();
		if (!done)
		{
			QString message = tr("Printing failed!");
			if (!printError.isEmpty())
				message += QString("\n%1").arg(printError);
			ScMessageBox::warning(this, CommonStrings::trWarning, message);
		}
		else
			doc->Print_Options.firstUse = false;
		getDefaultPrinter(PDef.Pname, PDef.Pname, PDef.Command);
	}
	printDinUse = false;
	disconnect(printer, SIGNAL(doPreview()), this, SLOT(doPrintPreview()));
	delete printer;
	m_mainWindowStatusLabel->setText( tr("Ready"));
}

void ScribusMainWindow::slotEndSpecialEdit()
{
	if (doc->symbolEditMode())
		editSymbolEnd();
	else if (doc->inlineEditMode())
		editInlineEnd();
	else if (doc->masterPageMode())
		editMasterPagesEnd();
}

bool ScribusMainWindow::doPrint(PrintOptions &options, QString& error)
{
	bool printDone = false;
	QString filename(options.filename);
	if (options.toFile)
	{
		qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));
		if (!overwrite(this, filename))
		{
			qApp->restoreOverrideCursor();
			return true;
		}
		qApp->restoreOverrideCursor();
	}
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	ScPrintEngine* prnEngine = NULL;
#if defined(_WIN32)
	SHORT shiftState = GetKeyState( VK_SHIFT );
	bool  forceGDI = ( shiftState & 0x8000 ) ? true : false;
	if (doc->Print_Options.toFile)
		prnEngine = dynamic_cast<ScPrintEngine*>(new ScPrintEngine_PS());
	else
	{
		ScPrintEngine_GDI* gdiEngine = new ScPrintEngine_GDI();
		gdiEngine->setForceGDI( forceGDI );
		prnEngine = dynamic_cast<ScPrintEngine*>(gdiEngine);
	}
#else
	prnEngine = dynamic_cast<ScPrintEngine*>(new ScPrintEngine_PS());
#endif
	if (prnEngine)
	{
		printDone = prnEngine->print(*doc, options);
		if (!printDone)
			error = prnEngine->errorMessage();
		delete prnEngine;
	}
	else
		error = tr( "Print engine initialization failed");
	ScCore->fileWatcher->start();
	return printDone;
}

void ScribusMainWindow::slotFileQuit()
{
	ScCore->pluginManager->savePreferences();
	close();
}

void ScribusMainWindow::slotEditCut()
{
	if (!HaveDoc)
		return;
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	if (doc->m_Selection->isEmpty())
		return;
	int docSelectionCount=doc->m_Selection->count();
	UndoTransaction activeTransaction;
	PageItem *currItem;
	for (int i = 0; i < docSelectionCount; ++i)
	{
		currItem=doc->m_Selection->itemAt(i);
		if ((currItem->asTextFrame() || currItem->asPathText()) && currItem==storyEditor->currentItem() && doc==storyEditor->currentDocument())
		{
				ScMessageBox::critical(this, tr("Cannot Cut In-Use Item"), tr("The item %1 is currently being edited by Story Editor. The cut operation will be cancelled").arg(currItem->itemName()));
				return;
		}
	}
	if (UndoManager::undoEnabled())
	{
		if (docSelectionCount > 1)
			activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Cut,"",Um::ICut);
		else
		{
			PageItem* item=doc->m_Selection->itemAt(0);
			activeTransaction = m_undoManager->beginTransaction(item->getUName(), item->getUPixmap(), Um::Cut, "", Um::ICut);
		}
	}
	currItem = doc->m_Selection->itemAt(0);
	if (((doc->appMode == modeEdit) || (doc->appMode == modeEditTable)) && (currItem->isTextFrame() || currItem->isTable()))
	{
		PageItem_TextFrame *cItem;
		if (doc->appMode == modeEditTable)
			cItem = currItem->asTable()->activeCell().textFrame();
		else
			cItem = currItem->asTextFrame();
		if (cItem->HasSel)
		{
			if ((cItem->itemText.length() == 0) || (!cItem->HasSel))
				return;
			StoryText itemText(doc);
			itemText.setDefaultStyle(cItem->itemText.defaultStyle());
			itemText.insert(0, cItem->itemText, true);
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			xmlStream.beginDoc();
			itemText.saxx(xmlStream, "SCRIBUSTEXT");
			xmlStream.endDoc();
			std::string xml(xmlString.str());
			ScTextMimeData* mimeData = new ScTextMimeData();
			mimeData->setScribusText (QByteArray(xml.c_str(), xml.length()));
			mimeData->setText( itemText.text(0, itemText.length()) ) ;
			QApplication::clipboard()->setMimeData(mimeData, QClipboard::Clipboard);
			cItem->deleteSelectedTextFromFrame();
			if (doc->appMode == modeEditTable)
				currItem->asTable()->update();
			else
				cItem->update();
		}
	}
	else
	{
		if ((currItem->isSingleSel) && (currItem->isGroup()))
			return;
		ScriXmlDoc ss;
		QString BufferS = ss.WriteElem(doc, doc->m_Selection);
		if ((m_prefsManager->appPrefs.scrapbookPrefs.doCopyToScrapbook) && (!internalCopy))
		{
			scrapbookPalette->ObjFromCopyAction(BufferS, currItem->itemName());
			rebuildRecentPasteMenu();
		}
		ScElemMimeData* mimeData = new ScElemMimeData();
		mimeData->setScribusElem(BufferS);
		mimeData->setText(BufferS);
		QApplication::clipboard()->setMimeData(mimeData, QClipboard::Clipboard);
		for (int i=0; i < doc->m_Selection->count(); ++i)
		{
			PageItem* frame = doc->m_Selection->itemAt(i);
			if (frame->asTextFrame() && frame->prevInChain() == NULL)
				frame->clearContents();
		}
		doc->itemSelection_DeleteItem();
	}
	slotDocCh();
	scrActions["editPaste"]->setEnabled(true);
	scrMenuMgr->setMenuEnabled("EditPasteRecent", scrapbookPalette->tempBView->objectMap.count() != 0);
	if (activeTransaction)
		activeTransaction.commit();
}

void ScribusMainWindow::slotEditCopy()
{
	if (!HaveDoc)
		return;
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	if (doc->m_Selection->isEmpty())
		return;
	PageItem *currItem = doc->m_Selection->itemAt(0);
	if (((doc->appMode == modeEdit) || (doc->appMode == modeEditTable)) && (currItem->isTextFrame() || currItem->isTable()))
	{
		PageItem_TextFrame *cItem;
		if (doc->appMode == modeEditTable)
			cItem = currItem->asTable()->activeCell().textFrame();
		else
			cItem = currItem->asTextFrame();
		if (cItem->HasSel)
		{
			StoryText itemText(doc);
			itemText.setDefaultStyle(cItem->itemText.defaultStyle());
			itemText.insert(0, cItem->itemText, true);
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			xmlStream.beginDoc();
			itemText.saxx(xmlStream, "SCRIBUSTEXT");
			xmlStream.endDoc();
			std::string xml(xmlString.str());
			ScTextMimeData* mimeData = new ScTextMimeData();
			mimeData->setScribusText( QByteArray(xml.c_str(), xml.length()) );
			mimeData->setText( itemText.text(0, itemText.length()) );
			QApplication::clipboard()->setMimeData(mimeData, QClipboard::Clipboard);
		}
	}
	else
	{
		if ((currItem->isSingleSel) && (currItem->isGroup()))
			return;
		//do not copy notes frames
		if (doc->m_Selection->count() ==1 && currItem->isNoteFrame())
			return;
		//deselect notesframes
		Selection tempSelection(*(doc->m_Selection));
		for (int i = 0; i < doc->m_Selection->count(); ++i)
		{
			if (doc->m_Selection->itemAt(i)->isNoteFrame())
				tempSelection.removeItem(doc->m_Selection->itemAt(i));
		}
		if (tempSelection.count() < doc->m_Selection->count())
			*(doc->m_Selection) = tempSelection;

		ScriXmlDoc ss;
		QString BufferS = ss.WriteElem(doc, doc->m_Selection);
		if (!internalCopy)
		{
			if ((m_prefsManager->appPrefs.scrapbookPrefs.doCopyToScrapbook) && (!internalCopy))
			{
				scrapbookPalette->ObjFromCopyAction(BufferS, currItem->itemName());
				rebuildRecentPasteMenu();
			}
			ScElemMimeData* mimeData = new ScElemMimeData();
			mimeData->setScribusElem(BufferS);
			mimeData->setText(BufferS);
			QApplication::clipboard()->setMimeData(mimeData, QClipboard::Clipboard);
		}
		else
			internalCopyBuffer = BufferS;
	}
	if (!internalCopy)
	{
		scrActions["editPaste"]->setEnabled(true);
		scrMenuMgr->setMenuEnabled("EditPasteRecent", scrapbookPalette->tempBView->objectMap.count() != 0);
	}
}

void ScribusMainWindow::slotEditPaste()
{
	if (!HaveDoc)
		return;
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	UndoTransaction activeTransaction;
	if (!ScMimeData::clipboardHasScribusData() && (!internalCopy))
		return;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(doc->currentPage()->getUName(), 0, Um::Paste, "", Um::IPaste);
	PageItem* selItem = doc->m_Selection->itemAt(0);
	if (((doc->appMode == modeEdit) || (doc->appMode == modeEditTable)) && selItem && (selItem->isTextFrame() || selItem->isTable()))
	{
		PageItem_TextFrame *currItem;
		if (doc->appMode == modeEditTable)
			currItem = selItem->asTable()->activeCell().textFrame();
		else
			currItem = selItem->asTextFrame();
		assert(currItem != NULL);
		if (currItem->HasSel)
		{
			//removing marks and notes from selected text
//				if (currItem->isTextFrame() && !currItem->asTextFrame()->removeMarksFromText(!ScCore->usingGUI()))
//					return;
			currItem->deleteSelectedTextFromFrame();
		}

		if (ScMimeData::clipboardHasScribusText())
		{
			Serializer *textSerializer = doc->textSerializer();
			textSerializer->reset();
			textSerializer->store<ScribusDoc>("<scribusdoc>", doc);

			QByteArray xml = ScMimeData::clipboardScribusText();
			textSerializer->parseMemory(xml, xml.length());

			StoryText* story = textSerializer->result<StoryText>();

			//avoid pasting notes marks into notes frames
			if (currItem->isNoteFrame())
			{
				story->setDoc(doc);
				for (int pos=story->length() -1; pos >= 0; --pos)
				{
					if (story->hasMark(pos) && (story->mark(pos)->isNoteType()))
						story->removeChars(pos,1);
				}
			}
			if (UndoManager::undoEnabled())
			{
				ScItemState<StoryText> *is = new ScItemState<StoryText>(Um::Paste);
				is->set("PASTE_TEXT");
				is->set("START", currItem->itemText.cursorPosition());
				is->setItem(*story);
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.insert(*story);

			delete story;
		}
		else if (ScMimeData::clipboardHasScribusElem() || ScMimeData::clipboardHasScribusFragment())
		{
			bool savedAlignGrid = doc->SnapGrid;
			bool savedAlignGuides = doc->SnapGuides;
			bool savedAlignElement = doc->SnapElement;
			int ac = doc->Items->count();
			bool isGroup = false;
			double gx, gy, gh, gw;
			FPoint minSize = doc->minCanvasCoordinate;
			FPoint maxSize = doc->maxCanvasCoordinate;
			doc->SnapGrid = false;
			doc->SnapGuides = false;
			doc->SnapElement = false;
			// HACK #6541 : undo does not handle text modification => do not record embedded item creation
			// if embedded item is deleted, undo system will not be aware of its deletion => crash - JG
			m_undoManager->setUndoEnabled(false);
			QString buffer  = ScMimeData::clipboardScribusElem();
			slotElemRead(buffer, 0, 0, false, true, doc, view);
			// update style lists:
			m_styleManager->setDoc(doc);
			propertiesPalette->unsetDoc();
			propertiesPalette->setDoc(doc);
			textPalette->unsetDoc();
			textPalette->setDoc(doc);
			marksManager->setDoc(doc);
			nsEditor->setDoc(doc);
			symbolPalette->unsetDoc();
			symbolPalette->setDoc(doc);

			doc->SnapGrid = savedAlignGrid;
			doc->SnapGuides = savedAlignGuides;
			doc->SnapElement = savedAlignElement;
			Selection tempSelection(*doc->m_Selection);
			doc->m_Selection->clear();
			if (doc->Items->count() - ac > 1)
				isGroup = true;
			doc->m_Selection->delaySignalsOn();
			for (int as = ac; as < doc->Items->count(); ++as)
			{
				doc->m_Selection->addItem(doc->Items->at(as));
			}
			if (isGroup)
				doc->GroupCounter++;
			doc->m_Selection->setGroupRect();
			doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			PageItem* currItem3 = doc->Items->at(ac);
			currItem3->isEmbedded = true;
			currItem3->setIsAnnotation(false);
			currItem3->isBookmark = false;
			currItem3->gXpos = currItem3->xPos() - gx;
			currItem3->gYpos = currItem3->yPos() - gy;
			currItem3->gWidth = gw;
			currItem3->gHeight = gh;
			int fIndex = doc->addToInlineFrames(currItem3);
			int acc = doc->Items->count();
			for (int as = ac; as < acc; ++as)
			{
				doc->Items->takeAt(ac);
			}
			doc->m_Selection->clear();
			*doc->m_Selection=tempSelection;
			doc->minCanvasCoordinate = minSize;
			doc->maxCanvasCoordinate = maxSize;
			if (outlinePalette->isVisible())
				outlinePalette->BuildTree();
			m_undoManager->setUndoEnabled(true);
			if (UndoManager::undoEnabled())
			{
				SimpleState *is = new SimpleState(Um::Paste,"",Um::IPaste);
				is->set("PASTE_INLINE");
				is->set("START", currItem->itemText.cursorPosition());
				is->set("INDEX", fIndex);
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.insertObject(fIndex);
			doc->m_Selection->delaySignalsOff();
			inlinePalette->unsetDoc();
			inlinePalette->setDoc(doc);
		}
		else if (ScMimeData::clipboardHasKnownData())
		{
			bool savedAlignGrid = doc->SnapGrid;
			bool savedAlignGuides = doc->SnapGuides;
			bool savedAlignElement = doc->SnapElement;
			FPoint minSize = doc->minCanvasCoordinate;
			FPoint maxSize = doc->maxCanvasCoordinate;
			doc->SnapGrid = false;
			doc->SnapGuides = false;
			doc->SnapElement = false;
			QString ext = ScMimeData::clipboardKnownDataExt();
			QByteArray bitsBits = ScMimeData::clipboardKnownDataData();
			double x0 = (view->contentsX() / view->scale()) + ((view->visibleWidth() / 2.0) / view->scale());
			double y0 = (view->contentsY() / view->scale()) + ((view->visibleHeight() / 2.0) / view->scale());
			PageItem *retObj = getVectorFileFromData(doc, bitsBits, ext, x0, y0);
			if (retObj != NULL)
			{
				double x = (view->contentsX() / view->scale()) + ((view->visibleWidth() / 2.0) / view->scale()) - (retObj->width() / 2.0);
				double y = (view->contentsY() / view->scale()) + ((view->visibleHeight() / 2.0) / view->scale()) - (retObj->height() / 2.0);
				retObj->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
				retObj->setXYPos(x, y, true);
				m_styleManager->setDoc(doc);
				propertiesPalette->unsetDoc();
				propertiesPalette->setDoc(doc);
				textPalette->unsetDoc();
				textPalette->setDoc(doc);
				marksManager->setDoc(doc);
				nsEditor->setDoc(doc);
				symbolPalette->unsetDoc();
				symbolPalette->setDoc(doc);
				doc->SnapGrid = savedAlignGrid;
				doc->SnapGuides = savedAlignGuides;
				doc->SnapElement = savedAlignElement;
				Selection tempSelection(*doc->m_Selection);
				doc->m_Selection->clear();
				doc->m_Selection->delaySignalsOn();
				doc->m_Selection->addItem(retObj);
				doc->m_Selection->setGroupRect();
				double gx, gy, gh, gw;
				doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				retObj->isEmbedded = true;
				retObj->setIsAnnotation(false);
				retObj->isBookmark = false;
				retObj->gXpos = retObj->xPos() - gx;
				retObj->gYpos = retObj->yPos() - gy;
				retObj->gWidth = gw;
				retObj->gHeight = gh;
				int fIndex = doc->addToInlineFrames(retObj);
				doc->Items->removeAll(retObj);
				doc->m_Selection->clear();
				*doc->m_Selection=tempSelection;
				doc->minCanvasCoordinate = minSize;
				doc->maxCanvasCoordinate = maxSize;
				if (outlinePalette->isVisible())
					outlinePalette->BuildTree();
				m_undoManager->setUndoEnabled(true);
				if (UndoManager::undoEnabled())
				{
					SimpleState *is = new SimpleState(Um::Paste, "", Um::IPaste);
					is->set("PASTE_INLINE");
					is->set("START", currItem->itemText.cursorPosition());
					is->set("INDEX", fIndex);
					m_undoManager->action(currItem, is);
				}
				currItem->itemText.insertObject(fIndex);
				doc->m_Selection->delaySignalsOff();
				inlinePalette->unsetDoc();
				inlinePalette->setDoc(doc);
			}
		}
		else
		{
			// K.I.S.S.:
			QString text = QApplication::clipboard()->text(QClipboard::Clipboard);
			text = text.replace("\r\n", SpecialChars::PARSEP);
			text = text.replace('\n', SpecialChars::PARSEP);
			if (UndoManager::undoEnabled())
			{
				SimpleState *is = new SimpleState(Um::Paste, "", Um::IPaste);
				is->set("PASTE_PLAINTEXT");
				is->set("START", currItem->itemText.cursorPosition());
				is->set("TEXT", text);
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.insertChars(text, true);
		}
		if (doc->appMode == modeEditTable)
			selItem->asTable()->update();
		else
			currItem->update();
		view->DrawNew();
	}
	else
	{
		if (ScMimeData::clipboardHasScribusElem() || ScMimeData::clipboardHasScribusFragment() || internalCopy)
		{
			view->Deselect(true);
			int docItemCount = doc->Items->count();
			bool savedAlignGrid = doc->SnapGrid;
			bool savedAlignGuides = doc->SnapGuides;
			bool savedAlignElement = doc->SnapElement;
			doc->SnapGrid = false;
			doc->SnapGuides = false;
			doc->SnapElement = false;
			if (internalCopy)
				slotElemRead(internalCopyBuffer, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
			else
			{
				QString buffer  = ScMimeData::clipboardScribusElem();
				slotElemRead(buffer, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
			}
			// update style lists:
			m_styleManager->setDoc(doc);
			propertiesPalette->unsetDoc();
			propertiesPalette->setDoc(doc);
			textPalette->unsetDoc();
			textPalette->setDoc(doc);
			marksManager->setDoc(doc);
			nsEditor->setDoc(doc);
			symbolPalette->unsetDoc();
			symbolPalette->setDoc(doc);
			inlinePalette->unsetDoc();
			inlinePalette->setDoc(doc);

			doc->SnapGrid = savedAlignGrid;
			doc->SnapGuides = savedAlignGuides;
			doc->SnapElement = savedAlignElement;
			doc->m_Selection->delaySignalsOn();
			for (int i = docItemCount; i < doc->Items->count(); ++i)
			{
				PageItem* currItem = doc->Items->at(i);
				if (currItem->isBookmark)
					AddBookMark(currItem);
				doc->m_Selection->addItem(currItem);
			}
			doc->m_Selection->delaySignalsOff();
			if (doc->m_Selection->count() > 1)
				doc->m_Selection->setGroupRect();
		}
		else if (ScMimeData::clipboardHasKnownData())
		{
			QString ext = ScMimeData::clipboardKnownDataExt();
			QByteArray bitsBits = ScMimeData::clipboardKnownDataData();
			double x0 = (view->contentsX() / view->scale()) + ((view->visibleWidth() / 2.0) / view->scale());
			double y0 = (view->contentsY() / view->scale()) + ((view->visibleHeight() / 2.0) / view->scale());
			PageItem *retObj = getVectorFileFromData(doc, bitsBits, ext, x0, y0);
			if (retObj != NULL)
			{
				double x = (view->contentsX() / view->scale()) + ((view->visibleWidth() / 2.0) / view->scale()) - (retObj->width() / 2.0);
				double y = (view->contentsY() / view->scale()) + ((view->visibleHeight() / 2.0) / view->scale()) - (retObj->height() / 2.0);
				retObj->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
				retObj->setXYPos(x, y, true);
			}
		}
		view->DrawNew();
	}
	if (activeTransaction)
		activeTransaction.commit();
	if (doc->notesChanged())
		doc->notesFramesUpdate();
	slotDocCh(false);
}

//CB-->Doc ?????
void ScribusMainWindow::SelectAllOnLayer()
{
	ColorList UsedC;
	doc->getUsedColors(UsedC);
	QScopedPointer<selectDialog> dia(new selectDialog(this, UsedC, doc->unitIndex()));
	if (!dia->exec())
		return;

	PageItem *currItem;
	view->Deselect();
	int docItemsCount = doc->Items->count();
	int docCurrentPage = doc->currentPageNumber();
	doc->m_Selection->delaySignalsOn();
	int range = dia->getSelectionRange();
	for (int i = 0; i < docItemsCount; ++i)
	{
		currItem = doc->Items->at(i);
		if ((currItem->LayerID == doc->activeLayer()) && (!doc->layerLocked(currItem->LayerID)))
		{
			if ((range == 0) && (currItem->OwnPage != docCurrentPage))
				continue;
			if ((range == 2) && (currItem->OwnPage != -1))
				continue;
			if (dia->useAttributes())
			{
				bool useType = false;
				bool useFill = false;
				bool useLine = false;
				bool useLWidth = false;
				bool usePrint = false;
				bool useLocked = false;
				bool useResize = false;
				dia->getUsedAttributes(useType, useFill, useLine, useLWidth, usePrint, useLocked, useResize);
				int Type = 0;
				QString fill;
				QString line;
				double LWidth = 0.0;
				bool Print = false;
				bool Locked = false;
				bool Resize = false;
				dia->getUsedAttributesValues(Type, fill, line, LWidth, Print, Locked, Resize);
				LWidth = LWidth / doc->unitRatio();
				if ((useType) && (Type != currItem->realItemType()))
					continue;
				if ((useFill) && ((fill != currItem->fillColor()) || (currItem->GrType != 0)))
					continue;
				if ((useLine) && (line != currItem->lineColor()))
					continue;
				if ((useLWidth) && ((LWidth != currItem->lineWidth()) || (currItem->lineColor() == CommonStrings::None)))
					continue;
				if ((usePrint) && (Print != currItem->printEnabled()))
					continue;
				if ((useLocked) && (Locked != currItem->locked()))
					continue;
				if ((useResize) && (Resize != currItem->sizeLocked()))
					continue;
				doc->m_Selection->addItem(currItem);
			}
			else
				doc->m_Selection->addItem(currItem);
		}
	}
	doc->m_Selection->delaySignalsOff();
	if (doc->m_Selection->count() > 1)
		doc->m_Selection->setGroupRect();
	view->DrawNew();
}

void ScribusMainWindow::SelectAll(bool docWideSelect)
{
	if (doc->appMode == modeEdit || doc->appMode == modeEditTable)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
		if (doc->appMode == modeEditTable)
			currItem = currItem->asTable()->activeCell().textFrame();
		PageItem *nextItem = currItem;
		nextItem->itemText.selectAll();
		while (nextItem != 0)
		{
			if (nextItem->prevInChain() != 0)
				nextItem = nextItem->prevInChain();
			else
				break;
		}
		while (nextItem != 0)
		{
			nextItem->HasSel = true;
			nextItem = nextItem->nextInChain();
		}
		EnableTxEdit();
	}
	else
	{
		PageItem *currItem;
		view->Deselect();
		doc->m_Selection->delaySignalsOn();
		int docItemsCount=doc->Items->count();
		int docCurrentPage=doc->currentPageNumber();
		for (int i = 0; i < docItemsCount; ++i)
		{
			currItem = doc->Items->at(i);
			if (((currItem->LayerID == doc->activeLayer()) || (doc->layerSelectable(currItem->LayerID))) && (!doc->layerLocked(currItem->LayerID)))
			{
				if (docWideSelect)
					doc->m_Selection->addItem(currItem);
				else
				{
					if (currItem->OwnPage==docCurrentPage)
						doc->m_Selection->addItem(currItem);
				}
			}
		}
		doc->m_Selection->delaySignalsOff();
		if (doc->m_Selection->count() > 1)
			doc->m_Selection->setGroupRect();
	}
	view->DrawNew();
}

void ScribusMainWindow::deselectAll()
{
	if (!HaveDoc)
		return;
	if (doc->inAnEditMode())
	{
		if (doc->m_Selection->isEmpty())
			return;
		PageItem *currItem = doc->m_Selection->itemAt(0);
		if (doc->appMode == modeEditTable)
			currItem = currItem->asTable()->activeCell().textFrame();
		if (currItem->asTextFrame())
		{
			currItem->itemText.deselectAll();
			doc->regionsChanged()->update(currItem->getBoundingRect());
		}
		else
		{
			doc->view()->Deselect(true);
			doc->view()->requestMode(modeNormal);
		}
	}
	else if (view != NULL)
		view->Deselect(true);
}

void ScribusMainWindow::ClipChange()
{
	bool textFrameEditMode = false;
	bool tableEditMode = false;
	bool hasScribusData = ScMimeData::clipboardHasScribusElem() || ScMimeData::clipboardHasScribusFragment();
	bool hasExternalData = ScMimeData::clipboardHasKnownData();
	if (HaveDoc && !doc->m_Selection->isEmpty())
	{
		PageItem *currItem = NULL;
		currItem = doc->m_Selection->itemAt(0);
		textFrameEditMode  = ((doc->appMode == modeEdit) && (currItem->asTextFrame()));
		tableEditMode = ((doc->appMode == modeEditTable) && (currItem->asTable()));
	}
	scrActions["editPaste"]->setEnabled(HaveDoc && (hasScribusData || textFrameEditMode || tableEditMode || hasExternalData));
}

void ScribusMainWindow::EnableTxEdit()
{
	scrActions["editCut"]->setEnabled(true);
	scrActions["editCopy"]->setEnabled(true);
}

void ScribusMainWindow::DisableTxEdit()
{
	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
}

void ScribusMainWindow::slotHelpAbout()
{
	About dia(this);
	dia.exec();
}

void ScribusMainWindow::slotHelpAboutPlugins()
{
	AboutPlugins dia(this);
	dia.exec();
}

void ScribusMainWindow::slotHelpAboutQt()
{
	ScMessageBox::aboutQt(this, tr("About Qt"));
}

void ScribusMainWindow::slotHelpCheckUpdates()
{
	About dia(this, About::CheckUpdates);
	dia.exec();
}

void ScribusMainWindow::slotOnlineHelp(const QString & jumpToSection, const QString & jumpToFile)
{
	if (!m_helpBrowser)
	{
		m_helpBrowser = new HelpBrowser(0, tr("Scribus Manual"), ScCore->getGuiLanguage(), jumpToSection, jumpToFile);
		connect(m_helpBrowser, SIGNAL(closed()), this, SLOT(slotOnlineHelpClosed()));
	}
	else //just set the requested page
	{
		if (!jumpToSection.isNull() || !jumpToFile.isNull())
		{
			m_helpBrowser->jumpToHelpSection(jumpToSection, jumpToFile, true);
		}
	}
	slotRaiseOnlineHelp();
}

void ScribusMainWindow::slotRaiseOnlineHelp()
{
	if (m_helpBrowser)
		m_helpBrowser->show();
}

void ScribusMainWindow::slotOnlineHelpClosed()
{
	if (m_helpBrowser)
		m_helpBrowser->deleteLater();
}

void ScribusMainWindow::slotResourceManager()
{
	if (!resourceManager) // in case its allocated???? maybe can remove in future
	{
		resourceManager=new ResourceManager(this);
		resourceManager->exec();
		resourceManager->deleteLater();
		resourceManager=NULL;
	}
}

void ScribusMainWindow::ToggleTips()
{
	m_prefsManager->appPrefs.displayPrefs.showToolTips = scrActions["helpTooltips"]->isChecked();
}

void ScribusMainWindow::ToggleMouseTips()
{
	m_prefsManager->appPrefs.displayPrefs.showMouseCoordinates = scrActions["showMouseCoordinates"]->isChecked();
}

void ScribusMainWindow::SaveText()
{
	LoadEnc = "";
	QString wdir = ".";
	QString prefsDocDir=m_prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		wdir = m_prefsManager->prefsFile->getContext("dirs")->get("save_text", prefsDocDir);
	else
		wdir = m_prefsManager->prefsFile->getContext("dirs")->get("save_text", ".");
	QString fn = CFileDialog( wdir, tr("Save As"), tr("Text Files (*.txt);;All Files (*)"), "", fdShowCodecs|fdHidePreviewCheckBox);
	if (!fn.isEmpty())
	{
		m_prefsManager->prefsFile->getContext("dirs")->set("save_text", fn.left(fn.lastIndexOf("/")));
		const StoryText& story (doc->m_Selection->itemAt(0)->itemText);
		Serializer::writeWithEncoding(fn, LoadEnc, story.text(0, story.length()));
	}
}

void ScribusMainWindow::applyNewMaster(QString name)
{
	Apply_MasterPage(name, doc->currentPage()->pageNr(), false);
	view->reformPages();
	view->DrawNew();
	pagePalette->Rebuild();
}

void ScribusMainWindow::slotNewPageP(int wo, QString templ)
{
	if (!HaveDoc)
		return;
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	view->Deselect(true);
	int where = 1;
	if (wo == 0)
		where = 0;
	else if (wo == doc->Pages->count())
		where = 2;
	slotNewPage(wo, templ); //master page is applied now
	//applyNewMaster(templ);
	if (where == 2)
		doc->addPageToSection(wo, where, 1);
	else
		doc->addPageToSection(wo+1, where, 1);

	doc->updateEndnotesFrames();
	doc->changed();
	updateGUIAfterPagesChanged();
}

/** Erzeugt eine neue Seite */
void ScribusMainWindow::slotNewPageM()
{
	if (!HaveDoc)
		return;
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	view->Deselect(true);
	InsPage *dia = new InsPage(this, doc, doc->currentPage()->pageNr(), doc->Pages->count());
	if (dia->exec())
	{
		QStringList base(dia->getMasterPages());
		double height=dia->heightSpinBox->value() / doc->unitRatio();
		double width=dia->widthSpinBox->value() / doc->unitRatio();
		int orientation=dia->orientationQComboBox->currentIndex();
		addNewPages(dia->getWherePage(), dia->getWhere(), dia->getCount(), height, width, orientation, 
			dia->prefsPageSizeName, dia->moveObjects->isChecked(), &base, dia->overrideMPSizingCheckBox->checkState()==Qt::Checked);
	}
	delete dia;
}

void ScribusMainWindow::addNewPages(int wo, int where, int numPages, double height, double width, int orient, QString siz, bool mov, QStringList* basedOn, bool overrideMasterPageSizing)
{
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
	{
		activeTransaction = m_undoManager->beginTransaction(doc->getUName(), Um::IDocument, (numPages == 1) ? Um::AddPage : Um::AddPages, "", Um::ICreate);
		SimpleState *ss = new SimpleState(Um::AddPage, "", Um::ICreate);
		ss->set("ADD_PAGE");
		ss->set("PAGE", wo);
		ss->set("WHERE", where);
		ss->set("COUNT", numPages);
		ss->set("MASTER_PAGE_MODE",  doc->masterPageMode());
		if (basedOn != NULL)
			ss->set("BASED", basedOn->join("|"));
		else
		{
			int setcol = doc->pageSets()[doc->pagePositioning()].Columns;
			if (setcol == 1)
				ss->set("BASED", CommonStrings::trMasterPageNormal);
			else if (setcol == 2)
				ss->set("BASED", CommonStrings::trMasterPageNormalLeft+"|"+ CommonStrings::trMasterPageNormalRight);
			else if ((setcol == 3) || (setcol == 4))
				ss->set("BASED", CommonStrings::trMasterPageNormalLeft+"|"+ CommonStrings::trMasterPageNormalMiddle+"|"+ CommonStrings::trMasterPageNormalRight);
		}
		ss->set("HEIGHT", height);
		ss->set("WIDTH", width);
		ss->set("ORIENT", orient);
		ss->set("SIZE", siz);
		ss->set("MOVED", mov);
		m_undoManager->action(this, ss);
	}
	// disable recording of undo actions related to new page creating
	// and object moving related to that
	m_undoManager->setUndoEnabled(false);

	QStringList base;
	if (basedOn != NULL)
	{
		base = *basedOn;
		// #10211 case when restoring page deletion, basedOn contains only masterpage name
		if (base.count() == 1)
		{
			int setcol = doc->pageSets()[doc->pagePositioning()].Columns;
			while (base.count() < setcol)
				base.append (base.at(0));
		}
	}
	if (base.empty())
	{
		int setcol = doc->pageSets()[doc->pagePositioning()].Columns;
		if (setcol == 1)
			base.append( CommonStrings::trMasterPageNormal);
		else if (setcol == 2)
		{
			base.append( CommonStrings::trMasterPageNormalLeft);
			base.append( CommonStrings::trMasterPageNormalRight);
		}
		else if (setcol == 3)
		{
			base.append( CommonStrings::trMasterPageNormalLeft);
			base.append( CommonStrings::trMasterPageNormalMiddle);
			base.append( CommonStrings::trMasterPageNormalRight);
		}
		else if (setcol == 4)
		{
			base.append( CommonStrings::trMasterPageNormalLeft);
			base.append( CommonStrings::trMasterPageNormalMiddle);
			base.append( CommonStrings::trMasterPageNormalMiddle);
			base.append( CommonStrings::trMasterPageNormalRight);
		}
	}
	int cc;
	int wot = wo;
	if (where==0)
		--wot;
	else if (where==2)
		wot=doc->Pages->count();
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	view->updatesOn(false);
	const PageSet& pageSet = doc->pageSets()[doc->pagePositioning()];
	ScPage* currentPage = doc->currentPage();
	for (cc = 0; cc < numPages; ++cc)
	{
		slotNewPage(wot, base[(wot + pageSet.FirstPage) % pageSet.Columns], mov); //Avoid the master page application with QString::null
//		slotNewPage(wot, QString::null, mov); //Avoid the master page application with QString::null
		//CB: #8212: added overrideMasterPageSizing, but keeping default to true for other calls for now, off for calls from InsPage
		if (overrideMasterPageSizing)
		{	
			doc->currentPage()->setInitialHeight(height);
			doc->currentPage()->setInitialWidth(width);
			doc->currentPage()->setOrientation(orient);
			doc->currentPage()->m_pageSize = siz;
		}
		//CB If we want to add this master page setting into the slotnewpage call, the pagenumber must be +1 I think
	//Apply_MasterPage(base[(doc->currentPage()->pageNr()+doc->pageSets[doc->currentPageLayout].FirstPage) % doc->pageSets[doc->currentPageLayout].Columns],
//						 doc->currentPage()->pageNr(), false); // this Apply_MasterPage avoids DreawNew and PagePalette->ReBuild, which is much faster for 100 pp :-)
		wot ++;
	}
	doc->setCurrentPage(currentPage);
	view->updatesOn(true);
	qApp->restoreOverrideCursor();
	//Use wo, the dialog currently returns a page Index +1 due to old numbering scheme, function now does the -1 as required
	doc->changed();
	doc->addPageToSection(wo, where, numPages);
	doc->reformPages();
	doc->updateEndnotesFrames();
	updateGUIAfterPagesChanged();

	m_undoManager->setUndoEnabled(true);

	if (activeTransaction)
	{
		activeTransaction.commit();
	}
}

void ScribusMainWindow::slotNewMasterPage(int w, const QString& name)
{
	if (doc->masterPageMode())
	{
		doc->addMasterPage(w, name);
		view->addPage(w);
	}
}

void ScribusMainWindow::slotNewPage(int w, const QString& masterPageName, bool mov)
{
	doc->addPage(w, masterPageName, true);
	view->addPage(w, mov);
	slotSetCurrentPage(w);
}


void ScribusMainWindow::duplicateToMasterPage()
{
	if (!HaveDoc)
		return;
	view->Deselect(true);
	int pageLocationIndex=-1;
	int pageLocationCount=0;
	if (doc->pagePositioning() != singlePage)
	{
		QStringList locationEntries;
		QList<PageSet> pageSet(doc->pageSets());
		for(QStringList::Iterator pNames = pageSet[doc->pagePositioning()].pageNames.begin(); pNames != pageSet[doc->pagePositioning()].pageNames.end(); ++pNames )
		{
			locationEntries << CommonStrings::translatePageSetLocString(*pNames);
		}
		pageLocationIndex=doc->columnOfPage(doc->currentPageNumber());
		pageLocationCount=locationEntries.count();
	}

	CopyPageToMasterPageDialog copyDialog(doc->MasterNames.count(), doc->pageSets()[doc->pagePositioning()].pageNames, pageLocationIndex, this);
	if (copyDialog.exec())
	{
		bool copyFromMaster=false;
		QString masterPageName;
		int pageLocation=0;
		copyDialog.values(masterPageName, copyFromMaster, pageLocation);
		bool badMasterPageName = doc->MasterNames.contains(masterPageName);
		badMasterPageName |= (masterPageName == CommonStrings::masterPageNormal);
		badMasterPageName |= (masterPageName == CommonStrings::trMasterPageNormal);
		badMasterPageName |= (masterPageName == CommonStrings::trMasterPageNormalLeft);
		badMasterPageName |= (masterPageName == CommonStrings::trMasterPageNormalMiddle);
		badMasterPageName |= (masterPageName == CommonStrings::trMasterPageNormalRight);
		badMasterPageName |=  masterPageName.isEmpty();
		while (badMasterPageName)
		{
			if (!copyDialog.exec())
				return;
			copyDialog.values(masterPageName, copyFromMaster, pageLocation);
			badMasterPageName = doc->MasterNames.contains(masterPageName);
			badMasterPageName |= (masterPageName == CommonStrings::masterPageNormal);
			badMasterPageName |= (masterPageName == CommonStrings::trMasterPageNormal);
			badMasterPageName |= (masterPageName == CommonStrings::trMasterPageNormalLeft);
			badMasterPageName |= (masterPageName == CommonStrings::trMasterPageNormalMiddle);
			badMasterPageName |= (masterPageName == CommonStrings::trMasterPageNormalRight);
			badMasterPageName |=  masterPageName.isEmpty();
		}
		int currentPageNumber=doc->currentPage()->pageNr();
		bool ok=doc->copyPageToMasterPage(currentPageNumber, pageLocation, pageLocationCount, masterPageName, copyFromMaster);
		Q_ASSERT(ok); //TODO get a return value in case the copy was not possible
		pagePalette->Rebuild();
	}
}

void ScribusMainWindow::slotZoom(double zoomFactor)
{
	double finalZoomFactor=0.0;
	//Zoom to Fit
	if (zoomFactor == -100.0)
	{
		finalZoomFactor = (view->height()-70) / (doc->currentPage()->height()+30);
	}
	else if (zoomFactor == -200.0)
	{
		finalZoomFactor = (view->width()-50) / (doc->currentPage()->width()+30);
	}
	//Zoom to %
	else
		finalZoomFactor = zoomFactor*m_prefsManager->displayScale()/100.0;

	if (finalZoomFactor == view->scale())
		return;

	int x = qRound(qMax(view->contentsX() / view->scale(), 0.0));
	int y = qRound(qMax(view->contentsY() / view->scale(), 0.0));
	int w = qRound(qMin(view->visibleWidth() / view->scale(), doc->currentPage()->width()));
	int h = qRound(qMin(view->visibleHeight() / view->scale(), doc->currentPage()->height()));

	if (zoomFactor == -200.0)
		view->rememberOldZoomLocation(qRound(doc->currentPage()->xOffset() + doc->currentPage()->width() / 2.0), h / 2 + y);
	else if(zoomFactor == -100.0)
		view->rememberOldZoomLocation(w / 2 + x, qRound(doc->currentPage()->yOffset() + doc->currentPage()->height() / 2.0));
	else
		view->rememberOldZoomLocation(w / 2 + x, h / 2 + y);

	view->zoom(finalZoomFactor);
}

void ScribusMainWindow::ToggleStickyTools()
{
	m_prefsManager->appPrefs.uiPrefs.stickyTools = !m_prefsManager->appPrefs.uiPrefs.stickyTools;
	scrActions["stickyTools"]->setChecked(m_prefsManager->appPrefs.uiPrefs.stickyTools);
	if (HaveDoc && doc->appMode!=modeNormal && !m_prefsManager->appPrefs.uiPrefs.stickyTools)
		view->requestMode(modeNormal);
}

void ScribusMainWindow::ToggleAllPalettes()
{
	if (m_palettesStatus[PAL_ALL])
	{
		m_palettesStatus[PAL_ALL] = false;
		if (m_palettesStatus[PAL_PROPERTIES])
			propertiesPalette->show();
		if (m_palettesStatus[PAL_TEXT])
			textPalette->show();
		if (m_palettesStatus[PAL_OUTLINE])
			outlinePalette->show();
		if (m_palettesStatus[PAL_SCRAPBOOK])
			scrapbookPalette->show();
		if (m_palettesStatus[PAL_LAYER])
			layerPalette->show();
		if (m_palettesStatus[PAL_PAGE])
			pagePalette->show();
		if (m_palettesStatus[PAL_BOOKMARK])
			bookmarkPalette->show();
		if (m_palettesStatus[PAL_VERIFIER])
			docCheckerPalette->show();
		if (m_palettesStatus[PAL_DOWNLOADS])
			downloadsPalette->show();
		setUndoPalette(m_palettesStatus[PAL_UNDO]);
	}
	else
	{
		m_palettesStatus[PAL_PROPERTIES] = propertiesPalette->isVisible();
		m_palettesStatus[PAL_TEXT] = textPalette->isVisible();
		m_palettesStatus[PAL_OUTLINE] = outlinePalette->isVisible();
		m_palettesStatus[PAL_SCRAPBOOK] = scrapbookPalette->isVisible();
		m_palettesStatus[PAL_LAYER] = layerPalette->isVisible();
		m_palettesStatus[PAL_PAGE] = pagePalette->isVisible();
		m_palettesStatus[PAL_BOOKMARK] = bookmarkPalette->isVisible();
		m_palettesStatus[PAL_UNDO] = undoPalette->isVisible();
		m_palettesStatus[PAL_VERIFIER] = docCheckerPalette->isVisible();
		m_palettesStatus[PAL_DOWNLOADS] = downloadsPalette->isVisible();
		propertiesPalette->hide();
		textPalette->hide();
		outlinePalette->hide();
		scrapbookPalette->hide();
		bookmarkPalette->hide();
		pagePalette->hide();
		layerPalette->hide();
		docCheckerPalette->hide();
		downloadsPalette->hide();
		setUndoPalette(false);
		m_palettesStatus[PAL_ALL] = true;
	}
}

void ScribusMainWindow::toggleCheckPal()
{
	m_palettesStatus[PAL_ALL] = false;
}

void ScribusMainWindow::setUndoPalette(bool visible)
{
	undoPalette->setPaletteShown(visible);
	scrActions["toolsActionHistory"]->setChecked(visible);
}

void ScribusMainWindow::togglePagePalette()
{
	m_palettesStatus[PAL_ALL] = false;
}

void ScribusMainWindow::toggleUndoPalette()
{
	setUndoPalette(!undoPalette->isVisible());
	m_palettesStatus[PAL_ALL] = false;
}

void ScribusMainWindow::toggleImageVisibility()
{
	if (!doc)
		return;
	doc->guidesPrefs().showPic = !doc->guidesPrefs().showPic;
	QList<PageItem*> allItems;
	for (int i = 0; i < doc->DocItems.count(); ++i)
	{
		PageItem *currItem = doc->DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			PageItem* item = allItems.at(j);
			if (item->asImageFrame())
				item->setImageVisible(doc->guidesPrefs().showPic);
		}
	}
	for (int i=0; i<doc->MasterItems.count(); ++i)
	{
		PageItem *currItem = doc->MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			PageItem* item = allItems.at(j);
			if (item->asImageFrame())
				item->setImageVisible(doc->guidesPrefs().showPic);
		}
	}
	view->DrawNew();
}

void ScribusMainWindow::ToggleAllGuides()
{
	if (!doc)
		return;
	m_keyrep=false;
	if (m_guidesStatus[GS_ALL])
	{
		m_guidesStatus[GS_ALL] = false;
		doc->guidesPrefs().marginsShown = m_guidesStatus[GS_MARGINS];
		doc->guidesPrefs().framesShown = m_guidesStatus[GS_FRAMES];
		doc->guidesPrefs().gridShown = m_guidesStatus[GS_GRID];
		doc->guidesPrefs().guidesShown = m_guidesStatus[GS_GUIDES];
		doc->guidesPrefs().baselineGridShown = m_guidesStatus[GS_BASELINE];
		doc->guidesPrefs().linkShown = m_guidesStatus[GS_LINKS];
		doc->guidesPrefs().showControls = m_guidesStatus[GS_CONTROLS];
		doc->guidesPrefs().rulerMode = m_guidesStatus[GS_RULERMODE];
		doc->guidesPrefs().rulersShown = m_guidesStatus[GS_RULERS];
		doc->guidesPrefs().colBordersShown = m_guidesStatus[GS_COLUMNBORDERS];
		doc->guidesPrefs().layerMarkersShown = m_guidesStatus[GS_LAYERMARKERS] ;
		doc->guidesPrefs().showBleed = m_guidesStatus[GS_BLEED];
		toggleMarks();
		toggleFrames();
		toggleLayerMarkers();
		toggleGrid();
		toggleGuides();
		toggleColumnBorders();
		toggleBase();
		toggleTextLinks();
		toggleTextControls();
		toggleRulerMode();
		toggleRulers();
		toggleBleeds();
	}
	else
	{
		m_guidesStatus[GS_ALL] = true;
		m_guidesStatus[GS_MARGINS] = !doc->guidesPrefs().marginsShown;
		m_guidesStatus[GS_FRAMES] = !doc->guidesPrefs().framesShown;
		m_guidesStatus[GS_GRID] = !doc->guidesPrefs().gridShown;
		m_guidesStatus[GS_GUIDES] = !doc->guidesPrefs().guidesShown;
		m_guidesStatus[GS_BASELINE] = !doc->guidesPrefs().baselineGridShown;
		m_guidesStatus[GS_LINKS] = !doc->guidesPrefs().linkShown;
		m_guidesStatus[GS_CONTROLS] = !doc->guidesPrefs().showControls;
		m_guidesStatus[GS_RULERMODE] = !doc->guidesPrefs().rulerMode;
		m_guidesStatus[GS_RULERS] = !doc->guidesPrefs().rulersShown;
		m_guidesStatus[GS_COLUMNBORDERS] = !doc->guidesPrefs().colBordersShown;
		m_guidesStatus[GS_LAYERMARKERS] = !doc->guidesPrefs().layerMarkersShown;
		m_guidesStatus[GS_BLEED] = !doc->guidesPrefs().showBleed;
		doc->guidesPrefs().marginsShown = false;
		doc->guidesPrefs().framesShown = false;
		doc->guidesPrefs().gridShown = false;
		doc->guidesPrefs().guidesShown = false;
		doc->guidesPrefs().baselineGridShown = false;
		doc->guidesPrefs().linkShown = false;
		doc->guidesPrefs().showControls = false;
		doc->guidesPrefs().rulerMode = false;
		doc->guidesPrefs().rulersShown = false;
		doc->guidesPrefs().colBordersShown = false;
		doc->guidesPrefs().layerMarkersShown = false;
		doc->guidesPrefs().showBleed = false;
		view->setRulersShown(doc->guidesPrefs().rulersShown);
	}
	scrActions["viewShowMargins"]->setChecked(doc->guidesPrefs().marginsShown);
	scrActions["viewShowBleeds"]->setChecked(doc->guidesPrefs().showBleed);
	scrActions["viewShowFrames"]->setChecked(doc->guidesPrefs().framesShown);
	scrActions["viewShowLayerMarkers"]->setChecked(doc->guidesPrefs().layerMarkersShown);
	scrActions["viewShowGrid"]->setChecked(doc->guidesPrefs().gridShown);
	scrActions["viewShowGuides"]->setChecked(doc->guidesPrefs().guidesShown);
	scrActions["viewShowColumnBorders"]->setChecked(doc->guidesPrefs().colBordersShown);
	scrActions["viewShowBaseline"]->setChecked(doc->guidesPrefs().baselineGridShown);
	scrActions["viewShowTextChain"]->setChecked(doc->guidesPrefs().linkShown);
	scrActions["viewShowTextControls"]->setChecked(doc->guidesPrefs().showControls);
	scrActions["viewShowRulers"]->setChecked(doc->guidesPrefs().rulersShown);
	scrActions["viewRulerMode"]->setChecked(doc->guidesPrefs().rulerMode);
	view->DrawNew();
}

void ScribusMainWindow::toggleMarks()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().marginsShown = !doc->guidesPrefs().marginsShown;
	view->DrawNew();
}

void ScribusMainWindow::toggleBleeds()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().showBleed = !doc->guidesPrefs().showBleed;
	view->DrawNew();
}

void ScribusMainWindow::toggleFrames()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().framesShown = !doc->guidesPrefs().framesShown;
	view->DrawNew();
}

void ScribusMainWindow::toggleLayerMarkers()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().layerMarkersShown = !doc->guidesPrefs().layerMarkersShown;
	view->DrawNew();
}

void ScribusMainWindow::toggleGrid()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().gridShown = !doc->guidesPrefs().gridShown;
	view->DrawNew();
}

void ScribusMainWindow::toggleGuides()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().guidesShown = !doc->guidesPrefs().guidesShown;
	view->DrawNew();
}

void ScribusMainWindow::toggleColumnBorders()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().colBordersShown = !doc->guidesPrefs().colBordersShown;
	view->DrawNew();
}

void ScribusMainWindow::toggleBase()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().baselineGridShown = !doc->guidesPrefs().baselineGridShown;
	view->DrawNew();
}

void ScribusMainWindow::toggleTextLinks()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().linkShown = !doc->guidesPrefs().linkShown;
	view->DrawNew();
}

void ScribusMainWindow::toggleTextControls()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().showControls = !doc->guidesPrefs().showControls;
	view->DrawNew();
}

void ScribusMainWindow::toggleRulers()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().rulersShown = !doc->guidesPrefs().rulersShown;
	view->setRulersShown(doc->guidesPrefs().rulersShown);
}

void ScribusMainWindow::toggleRulerMode()
{
	if (!doc)
		return;
	m_guidesStatus[GS_ALL] = false;
	doc->guidesPrefs().rulerMode = !doc->guidesPrefs().rulerMode;
	if (doc->guidesPrefs().rulerMode)
	{
		doc->rulerXoffset = 0;
		doc->rulerYoffset = 0;
	}
	else
	{
		doc->rulerXoffset += doc->currentPage()->xOffset();
		doc->rulerYoffset += doc->currentPage()->yOffset();
	}
	if (doc->m_Selection->count()==1)
	{
		PageItem* currItem=doc->m_Selection->itemAt(0);
		if (currItem!=NULL)
			currItem->emitAllToGUI();
	}
	//TODO emit from selection, handle group widths
	view->DrawNew();
}

void ScribusMainWindow::toggleSnapGrid()
{
	if (!doc)
		return;
	doc->SnapGrid = !doc->SnapGrid;
	slotDocCh();
}

void ScribusMainWindow::toggleSnapGuides()
{
	if (!doc)
		return;
	doc->SnapGuides = !doc->SnapGuides;
	slotDocCh();
}

void ScribusMainWindow::toggleSnapElements()
{
	if (!doc)
		return;
	doc->SnapElement = !doc->SnapElement;
	slotDocCh();
}

void ScribusMainWindow::SetSnapElements(bool b)
{
	if(doc && doc->SnapElement != b)
		toggleSnapElements();
}


void ScribusMainWindow::toggleNodeEdit()
{
	if (!doc)
		return;
	int i = modeEditClip;
	if (doc->appMode == modeEditClip)
		i = submodeEndNodeEdit;
	view->requestMode(i);
}

void ScribusMainWindow::enablePalettes(bool b)
{
	if(doc->appMode == modeEdit) //Keep Palettes enabled when editing text
		return;
	layerPalette->setEnabled(b);
	outlinePalette->setEnabled(b);
	guidePalette->setEnabled(b);
	scrapbookPalette->setEnabled(b);
	pagePalette->setEnabled(b);
	bookmarkPalette->setEnabled(b);
	docCheckerPalette->setEnabled(b);
	inlinePalette->setEnabled(b);
	symbolPalette->setEnabled(b);
	alignDistributePalette->setEnabled(b);
	downloadsPalette->setEnabled(b);
}

void ScribusMainWindow::ToggleFrameEdit()
{
	if (!doc)
		return;

	nodePalette->setDoc(doc, view);
	nodePalette->MoveN();
	nodePalette->HaveNode(false, false);
	nodePalette->MoveNode->setChecked(true);
	nodePalette->show();
//	qDebug() << "nodepalette show:" << nodePalette->geometry();
	connect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
	connect(view, SIGNAL(PolyStatus(int, uint)), nodePalette, SLOT(PolyStatus(int, uint)));
	doc->nodeEdit.reset();
	appModeHelper->setFrameEditMode(true);
	enablePalettes(false);
	m_styleManager->setEnabled(false);
	pageSelector->setEnabled(false);
	layerMenu->setEnabled(false);
	if (!doc->m_Selection->isEmpty())
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
		nodePalette->EditCont->setEnabled(currItem->ContourLine.size() != 0);
		nodePalette->ResetCont->setEnabled(false);
		nodePalette->ResetContClip->setEnabled(false);
		nodePalette->PolyStatus(currItem->itemType(), currItem->PoLine.size());
		nodePalette->setDefaults(currItem);
		if ((currItem->asImageFrame()) && (currItem->imageClip.size() != 0))
		{
			nodePalette->ResetContClip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
			nodePalette->ResetContClip->show();
			nodePalette->ResetShape2Clip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
			nodePalette->ResetShape2Clip->show();
			nodePalette->layout()->activate();
			nodePalette->resize(QSize(170, 380).expandedTo(nodePalette->minimumSizeHint()));
		}
		else
		{
			nodePalette->ResetContClip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(6), static_cast<QSizePolicy::Policy>(6)));
			nodePalette->ResetShape2Clip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(6), static_cast<QSizePolicy::Policy>(6)));
			nodePalette->layout()->activate();
			nodePalette->ResetContClip->hide();
			nodePalette->ResetShape2Clip->hide();
			nodePalette->layout()->activate();
			nodePalette->resize(QSize(170, 380).expandedTo(nodePalette->minimumSizeHint()));
		}
	}
}

void ScribusMainWindow::NoFrameEdit()
{
	disconnect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
	disconnect(view, SIGNAL(PolyStatus(int, uint)), nodePalette, SLOT(PolyStatus(int, uint)));
	actionManager->disconnectModeActions();
	nodePalette->setDoc(0,0);
	nodePalette->hide();
	appModeHelper->setFrameEditMode(false);
	scrActions["toolsSelect"]->setChecked(true);
	scrActions["toolsEditContents"]->setChecked(false);
	scrActions["toolsEditWithStoryEditor"]->setChecked(false);
	enablePalettes(true);
	m_styleManager->setEnabled(true);
	pageSelector->setEnabled(true);
	layerMenu->setEnabled(true);
	if (HaveDoc)
	{
		doc->nodeEdit.reset();
		HaveNewSel();
		if (!doc->m_Selection->isEmpty())
		{
			doc->m_Selection->itemAt(0)->emitAllToGUI();
			view->DrawNew();
		}
	}
	actionManager->connectModeActions();
}

/** This is the safest method to return to modeNormal
*/
void ScribusMainWindow::slotSelect()
{
	if (doc)
		view->requestMode(modeNormal);
	else
		appModeHelper->resetApplicationMode(this, modeNormal);
}

void ScribusMainWindow::setAppModeByToggle(bool isOn, int newMode)
{
	m_keyrep=false;

	if (newMode==modeDrawLatex && !m_prefsManager->renderFrameConfigured())
	{
		ScMessageBox::critical(this, "Render Frames Not Configured", "Your Render Frame configuration seems to be invalid. Please check the settings in the External Tools section of the Preferences dialog.");
		return;
	}

	if (doc && isOn)
		view->requestMode(newMode);
	else
		slotSelect();
}

void ScribusMainWindow::setMainWindowActive()
{
	activateWindow();
	if (!scriptIsRunning())
		raise();
}

void ScribusMainWindow::setItemTypeStyle(int id)
{
	int b = 0;
	if (id == 0)
	{
		scrActions["typeEffectNormal"]->setChecked(true);
		scrActions["typeEffectUnderline"]->setChecked(false);
		scrActions["typeEffectUnderlineWords"]->setChecked(false);
		scrActions["typeEffectStrikeThrough"]->setChecked(false);
		scrActions["typeEffectSmallCaps"]->setChecked(false);
		scrActions["typeEffectAllCaps"]->setChecked(false);
		scrActions["typeEffectSuperscript"]->setChecked(false);
		scrActions["typeEffectSubscript"]->setChecked(false);
		scrActions["typeEffectOutline"]->setChecked(false);
		scrActions["typeEffectShadow"]->setChecked(false);
	}
	else
	{
		scrActions["typeEffectNormal"]->setChecked(false);
		if (id == 4)
			scrActions["typeEffectSubscript"]->setChecked(false);
		if (id == 5)
			scrActions["typeEffectSuperscript"]->setChecked(false);
		if (id == 3)
			scrActions["typeEffectAllCaps"]->setChecked(false);
		if (id == 7)
			scrActions["typeEffectSmallCaps"]->setChecked(false);
		if (id == 8)
			scrActions["typeEffectUnderline"]->setChecked(false);
		if (id == 1)
			scrActions["typeEffectUnderlineWords"]->setChecked(false);
		if (scrActions["typeEffectUnderline"]->isChecked())
			b |= 8;
		if (scrActions["typeEffectUnderlineWords"]->isChecked())
			b |= 512;
		if (scrActions["typeEffectShadow"]->isChecked())
			b |= 256;
		if (scrActions["typeEffectStrikeThrough"]->isChecked())
			b |= 16;
		if (scrActions["typeEffectAllCaps"]->isChecked())
			b |= 32;
		if (scrActions["typeEffectSmallCaps"]->isChecked())
			b |= 64;
		if (scrActions["typeEffectSuperscript"]->isChecked())
			b |= 1;
		if (scrActions["typeEffectSubscript"]->isChecked())
			b |= 2;
		if (scrActions["typeEffectOutline"]->isChecked())
			b |= 4;
	}
	setItemEffects(b);
}

void ScribusMainWindow::setStyleEffects(int s)
{
	int i = s & 1919;
	scrActions["typeEffectNormal"]->setChecked(i==0);
	scrActions["typeEffectSuperscript"]->setChecked(i & 1);
	scrActions["typeEffectSubscript"]->setChecked(i & 2);
	scrActions["typeEffectOutline"]->setChecked(i & 4);
	scrActions["typeEffectUnderline"]->setChecked(i & 8);
	scrActions["typeEffectStrikeThrough"]->setChecked(i & 16);
	scrActions["typeEffectAllCaps"]->setChecked(i & 32);
	scrActions["typeEffectSmallCaps"]->setChecked(i & 64);
	scrActions["typeEffectShadow"]->setChecked(i & 256);
	scrActions["typeEffectUnderlineWords"]->setChecked(i & 512);
	emit TextEffects(s);
}

void ScribusMainWindow::setItemEffects(int h)
{
	if (doc->m_Selection->isEmpty())
		return;
	setStyleEffects(h);
	doc->itemSelection_SetEffects(h);
}

//CB-->Doc partly
void ScribusMainWindow::deletePage2(int pg)
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	view->Deselect(true);
	if (doc->Pages->count() == 1)
		return;
	deletePage(pg+1, pg+1);
}

void ScribusMainWindow::deletePage()
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	view->Deselect(true);
	DelPages *dia = new DelPages(this, doc->currentPage()->pageNr()+1, doc->Pages->count());
	if (dia->exec())
		deletePage(dia->getFromPage(), dia->getToPage());
	delete dia;
}

void ScribusMainWindow::deletePage(int from, int to)
{
	UndoTransaction activeTransaction;
	assert( from > 0 );
	assert( from <= to );
	assert( to <= static_cast<int>(doc->Pages->count()) );
	int oldPg = doc->currentPageNumber();
	guidePalette->setDoc(NULL);
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(doc->DocName, Um::IDocument,
														  (from - to == 0) ? Um::DeletePage : Um::DeletePages, "",
														  Um::IDelete);
	PageItem* ite;
	doc->m_Selection->clear();
	Selection tmpSelection(this, false);
	for (int a = to - 1; a >= from - 1; a--)
	{
		for (int d = 0; d < doc->Items->count(); ++d)
		{
			ite = doc->Items->at(d);
			//do not delete notes frames
			if (ite->isAutoNoteFrame())
				continue;
			if (ite->OwnPage == a)
			{
				ite->setLocked(false);
				ite->isSingleSel = false;
				if (ite->isBookmark)
					DelBookMark(ite);
				ite->isBookmark = false;
				tmpSelection.addItem(ite);
			}
		}
		ScPage *page = doc->Pages->at(a); // need to remove guides too to get their undo/redo actions working
		page->guides.clearHorizontals(GuideManagerCore::Standard);
		page->guides.clearHorizontals(GuideManagerCore::Auto);
		page->guides.clearVerticals(GuideManagerCore::Standard);
		page->guides.clearVerticals(GuideManagerCore::Auto);
	}
	if (tmpSelection.count() != 0)
		doc->itemSelection_DeleteItem(&tmpSelection);
	bool b = pageSelector->blockSignals(true);
	view->updatesOn(false);
	for (int a = to - 1; a >= from - 1; a--)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::DeletePage, "", Um::ICreate);
			ss->set("DELETE_PAGE");
			ss->set("PAGENR", a + 1);
			ss->set("PAGENAME",   doc->Pages->at(a)->pageName());
			ss->set("MASTERPAGE", doc->Pages->at(a)->MPageNam);
			ss->set("MASTER_PAGE_MODE",  doc->masterPageMode());
			// replace the deleted page in the undostack by a dummy object that will
			// replaced with the "undone" page if user choose to undo the action
			DummyUndoObject *duo = new DummyUndoObject();
			uint id = static_cast<uint>(duo->getUId());
			m_undoManager->replaceObject(doc->Pages->at(a)->getUId(), duo);
			ss->set("DUMMY_ID", id);
			m_undoManager->action(this, ss);
		}
		bool isMasterPage = !(doc->Pages->at(a)->pageName().isEmpty());
		if (doc->masterPageMode())
			doc->deleteMasterPage(a);
		else
			doc->deletePage(a);
		if (!isMasterPage) // Master pages are not added to sections when created
			doc->removePageFromSection(a);
	}
	pageSelector->setMaximum(doc->Pages->count());
	pageSelector->blockSignals(b);
	m_undoManager->setUndoEnabled(false); // ugly hack to disable object moving when undoing page deletion
	view->reformPagesView();
	m_undoManager->setUndoEnabled(true); // ugly hack continues
	view->updatesOn(true);
	view->GotoPage(qMin(doc->Pages->count()-1, oldPg));
	doc->updateEndnotesFrames();
	updateGUIAfterPagesChanged();
	doc->rebuildMasterNames();
	pagePalette->rebuildMasters();
	if (activeTransaction)
		activeTransaction.commit();
}

void ScribusMainWindow::movePage()
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	int currentPage = doc->currentPage()->pageNr();
	QScopedPointer<MovePages> dia(new MovePages(this, currentPage + 1, doc->Pages->count(), true));
	if (!dia->exec())
		return;
	int from = dia->getFromPage();
	int to = dia->getToPage();
	int wie = dia->getWhere();
	int wo = dia->getWherePage();
	if (wie == 3)
	{
		doc->swapPage(from-1, wo-1);
		updateGUIAfterPagesChanged();
		return;
	}
	if ((wie < 2) && (from <= wo) && (wo <= to))
		return;
	if (wie == 2 && to == signed(doc->Pages->count()))
		return;
	doc->movePage(from-1, to, wo-1, wie);
	updateGUIAfterPagesChanged();
	doc->updateEndnotesFrames();
}

void ScribusMainWindow::copyPage()
{
	if (!HaveDoc)
		return;
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	QScopedPointer<MovePages> dia(new MovePages(this, doc->currentPage()->pageNr()+1, doc->Pages->count(), false));
	if (!dia->exec())
		return;
	int pageNumberToCopy=dia->getFromPage()-1;
	int whereToInsert=dia->getWhere();
	int copyCount=dia->getCopyCount();
	int wo = dia->getWherePage();
	doc->copyPage(pageNumberToCopy, wo, whereToInsert, copyCount);
	view->Deselect(true);
	doc->updateEndnotesFrames();
	updateGUIAfterPagesChanged();
	slotDocCh();
}

void ScribusMainWindow::changePageProperties()
{
	if (!HaveDoc)
		return;
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	QString currPageMasterPageName(doc->currentPage()->MPageNam);
	QScopedPointer<PagePropertiesDialog> dia(new PagePropertiesDialog(this, doc));
	if (!dia->exec())
		return;

	int orientation = dia->getPageOrientation();
	double pageHeight = dia->getPageHeight();
	double pageWidth = dia->getPageWidth();
	QString pageSizeName = dia->getPrefsPageSizeName();
	int lp=0;
	if (doc->masterPageMode() && doc->pagePositioning() != singlePage)
		lp = dia->pageOrder();
	doc->changePageProperties(dia->top(), dia->bottom(), dia->left(), dia->right(),
							pageHeight, pageWidth, pageHeight, pageWidth, orientation,
							pageSizeName, dia->getMarginPreset(), dia->getMoveObjects(), doc->currentPage()->pageNr(), lp);
	if (!doc->masterPageMode() && dia->masterPage() != currPageMasterPageName)
		Apply_MasterPage(dia->masterPage(), doc->currentPage()->pageNr());
	doc->updateEndnotesFrames();
}

void ScribusMainWindow::SetNewFont(const QString& nf)
{
	setMainWindowActive();
	doc->itemSetFont(nf);
}

void ScribusMainWindow::setItemFontSize(int fontSize)
{
	int fs=fontSize;
	if (fs != -1)
		doc->itemSelection_SetFontSize(fs*10);
	else
	{
		bool ok = false;
		Query dia(this, "New", 1, tr("&Size:"), tr("Size"));
		if (dia.exec())
		{
			fs = qRound(dia.getEditText().toDouble(&ok));
			if (ok && (fs < 1025) && (fs > 0))
				doc->itemSelection_SetFontSize(fs*10);
		}
	}
	textPalette->textPal->showFontSize(fs*10);
}

void ScribusMainWindow::setItemLanguage(QString language)
{
	Query dia(this, "New", 1, tr("&Language:"), tr("Language"));
	if (dia.exec())
	{
		doc->itemSelection_SetLanguage(language);
	}

	textPalette->textPal->showLanguage(language);
}

//CB-->Doc
void ScribusMainWindow::setNewAlignment(int i)
{
	if (!HaveDoc)
		return;
	doc->itemSelection_SetAlignment(i);
	textPalette->textPal->showAlignment(i);
	PageItem *currItem = doc->m_Selection->itemAt(0);
	setTBvals(currItem);
}

void ScribusMainWindow::setNewDirection(int i)
{
	if (!HaveDoc)
		return;
	doc->itemSelection_SetDirection(i);
	textPalette->textPal->showDirection(i);
	PageItem *currItem = doc->m_Selection->itemAt(0);
	setTBvals(currItem);
}

void ScribusMainWindow::setNewParStyle(const QString& name)
{
	if (!HaveDoc)
		return;
	doc->itemSelection_SetNamedParagraphStyle(name);
	PageItem *currItem = doc->m_Selection->itemAt(0);
	setTBvals(currItem);
}

void ScribusMainWindow::setNewCharStyle(const QString& name)
{
	if (!HaveDoc)
		return;
	doc->itemSelection_SetNamedCharStyle(name);
	PageItem *currItem = doc->m_Selection->itemAt(0);
	setTBvals(currItem);
}

void ScribusMainWindow::setAlignmentValue(int i)
{
	textPalette->textPal->showAlignment(i);
	QString alignment[] = {"Left", "Center", "Right", "Block", "Forced"};
	for (int j=0; j<5; ++j)
	{
		QString actionName="align"+alignment[j];
		if (scrActions[actionName])
			scrActions[actionName]->setChecked(i==j);
	}
}

void ScribusMainWindow::setDirectionValue(int i)
{
	textPalette->textPal->showDirection(i);
	QString direction[] = {"Left", "Right"};
	for (int j=0; j<2; ++j)
	{
		QString actionName="direction"+direction[j];
		if (scrActions[actionName])
			scrActions[actionName]->setChecked(i==j);
	}
}

//CB-->??
void ScribusMainWindow::MakeFrame(int f, int c, double *vals)
{
	PageItem *currItem = doc->m_Selection->itemAt(0);
	switch (f)
	{
		case 0:
			currItem->SetRectFrame();
			doc->setRedrawBounding(currItem);
			break;
		case 1:
			currItem->SetOvalFrame();
			doc->setRedrawBounding(currItem);
			break;
		default:
			currItem->SetFrameShape(c, vals);
			doc->setRedrawBounding(currItem);
			currItem->FrameType = f+2;
			break;
	}
	currItem->update();
	slotDocCh();
}

void ScribusMainWindow::duplicateItem()
{
	slotSelect();
	bool savedAlignGrid = doc->SnapGrid;
	bool savedAlignGuides = doc->SnapGuides;
	bool savedAlignElement = doc->SnapElement;
	internalCopy = true;
	doc->SnapGrid  = false;
	doc->SnapGuides = false;
	doc->SnapElement = false;

	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = m_undoManager->beginTransaction(Um::Selection, Um::IPolygon, Um::Duplicate, "", Um::IMultipleDuplicate);

	ItemMultipleDuplicateData mdData;
	memset(&mdData, 0, sizeof(mdData));
	mdData.type = 0;
	mdData.copyCount = 1;
	mdData.copyShiftOrGap = 0;
	mdData.copyShiftGapH = doc->opToolPrefs().dispX * doc->unitRatio();
	mdData.copyShiftGapV = doc->opToolPrefs().dispY * doc->unitRatio();

	int oldItemCount = doc->Items->count();
	doc->m_Selection->delaySignalsOn();
	doc->itemSelection_MultipleDuplicate(mdData);
	view->Deselect(true);
	for (int i = oldItemCount; i < doc->Items->count(); ++i)
	{
		PageItem* item = doc->Items->at(i);
		doc->m_Selection->addItem(item);
	}
	doc->m_Selection->delaySignalsOff();

	if (trans)
		trans.commit();
	doc->SnapGrid  = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	doc->SnapElement = savedAlignElement;
	internalCopy = false;
	internalCopyBuffer = "";
	view->DrawNew();
}

void ScribusMainWindow::duplicateItemMulti()
{
	if (!HaveDoc)
		return;
	slotSelect();
	if (doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	internalCopy = true;
	QScopedPointer<MultipleDuplicate> dia(new MultipleDuplicate(doc->unitIndex(), this));
	if (dia->exec())
	{
		ItemMultipleDuplicateData mdData;
		dia->getMultiplyData(mdData);
		doc->itemSelection_MultipleDuplicate(mdData);
	}
	internalCopy = false;
}

void ScribusMainWindow::editItemsFromOutlines(PageItem *ite)
{
	if (ite->locked())
		return;
	if (!doc->m_Selection->isEmpty())
	{
		if (doc->m_Selection->itemAt(0) != ite)
			selectItemsFromOutlines(ite, ite->isGroup());
	}
	if (ite->asLatexFrame())
	{
		if (ite->imageVisible())
			view->requestMode(modeEdit);
	}
#ifdef HAVE_OSG
	else if (ite->asOSGFrame())
		view->requestMode(submodeEditExternal);
#endif
	else if ((ite->itemType() == PageItem::Polygon) || (ite->itemType() == PageItem::PolyLine) || (ite->itemType() == PageItem::Group) || (ite->itemType() == PageItem::ImageFrame) || (ite->itemType() == PageItem::PathText))
	{
		if (ite->itemType() == PageItem::ImageFrame)
		{
			if (ite->Pfile.isEmpty())
				view->requestMode(submodeLoadPic);
			else if (!ite->imageIsAvailable)
				view->requestMode(submodeStatusPic);
			else if (ite->imageVisible())
				view->requestMode(modeEdit);
		}
		else if (ite->itemType() == PageItem::TextFrame)
			view->requestMode(modeEdit);
		else
		{
			view->requestMode(modeEditClip);
			scrActions["itemUngroup"]->setEnabled(false);
			scrActions["itemGroupAdjust"]->setEnabled(false);
		}
	}
	else if (ite->itemType() == PageItem::TextFrame)
	{
		if (ite->isAnnotation())
		{
			view->requestMode(submodeAnnotProps);
		}
		else if (doc->appMode != modeEdit)
		{
			view->requestMode(modeEdit);
		}
	}
	else if (ite->asSymbolFrame())
	{
		if (!doc->symbolEditMode())
			view->requestMode(submodeEditSymbol);
	}
	else if (ite->asArc())
		view->requestMode(modeEditArc);
	else if (ite->asRegularPolygon())
		view->requestMode(modeEditPolygon);
	else if (ite->asSpiral())
		view->requestMode(modeEditSpiral);
	else if (ite->asTable())
		view->requestMode(modeEditTable);
}

void ScribusMainWindow::selectItemsFromOutlines(PageItem* ite, bool single, int position)
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	activateWindow();
	view->Deselect(true);
	if (!doc->symbolEditMode() && !doc->inlineEditMode())
	{
		int itemPage = ite->OwnPage;
		PageItem* parentItem = ite->Parent;
		while (parentItem && parentItem->isGroup())
		{
			itemPage = parentItem->OwnPage;
			parentItem = parentItem->Parent;
		}
		if ((itemPage != -1) && (itemPage != static_cast<int>(doc->currentPage()->pageNr())))
			view->GotoPage(itemPage);
	}
	doc->m_Selection->delaySignalsOn();
	view->SelectItem(ite, true, single);
	doc->m_Selection->delaySignalsOff();

	if (doc->m_Selection->isEmpty())
		return;
	doc->m_Selection->connectItemToGUI();

	PageItem *currItem = doc->m_Selection->itemAt(0);
	QTransform itemTrans = currItem->getTransform();
	double xOffset=0.0,yOffset=0.0;
	switch (position)
	{
		case 1: //top left
			break;
		default: //center
			xOffset = currItem->width() / 2.0;
			yOffset = currItem->height() / 2.0;
			break;
	}

	QPointF point = itemTrans.map(QPointF(xOffset, yOffset));
	view->SetCCPo(point.x(), point.y());
}

void ScribusMainWindow::selectItemFromOutlines(PageItem *ite, bool single, int cPos)
{
	selectItemsFromOutlines(ite, single);
	if (doc->m_Selection->isEmpty())
		return;
	PageItem *currItem = doc->m_Selection->itemAt(0);
	if (currItem->isTextFrame())
	{
		view->requestMode(modeEdit);
		currItem->itemText.setCursorPosition(cPos);
		currItem->update();
	}
}

void ScribusMainWindow::selectPagesFromOutlines(int Page)
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	activateWindow();
	view->Deselect(true);
	if (Page < 0)
		return;
	view->GotoPage(Page);
}

void ScribusMainWindow::slotPrefsOrg()
{
	QString oldMonitorProfile(ScCore->monitorProfile.productDescription());
	slotSelect();
	struct ApplicationPrefs oldPrefs(m_prefsManager->appPrefs);
	PreferencesDialog prefsDialog(this, oldPrefs);
	int prefsResult=prefsDialog.exec();
	if (prefsResult==QDialog::Accepted)
	{
		struct ApplicationPrefs newPrefs(prefsDialog.prefs());
		m_prefsManager->setNewPrefs(newPrefs);
		m_prefsManager->applyLoadedShortCuts();

		//TODO: and the other dirs?
		DocDir = m_prefsManager->documentDir();
		if (oldPrefs.pathPrefs.documents != newPrefs.pathPrefs.documents)
		{
			PrefsContext* docContext = m_prefsManager->prefsFile->getContext("docdirs", false);
			docContext->set("docsopen", newPrefs.pathPrefs.documents);
		}

		ScQApp->neverSplash(!m_prefsManager->appPrefs.uiPrefs.showSplashOnStartup);

		QString newUILanguage = m_prefsManager->uiLanguage();
		if (oldPrefs.uiPrefs.language != newUILanguage || ScQApp->currGUILanguage()!=newUILanguage)
			ScQApp->changeGUILanguage(newUILanguage);
		m_prefsManager->appPrefs.uiPrefs.language = ScQApp->currGUILanguage();
		QString newUIStyle = m_prefsManager->guiStyle();
		if (oldPrefs.uiPrefs.style != newUIStyle)
		{
			if (newUIStyle.isEmpty())
				qApp->setStyle(m_prefsManager->guiSystemStyle());
			else
				qApp->setStyle(QStyleFactory::create(newUIStyle));
		}
		int newUIFontSize = m_prefsManager->guiFontSize();
		if (oldPrefs.uiPrefs.applicationFontSize != newUIFontSize)
		{
			QFont apf = qApp->font();
			apf.setPointSize(newUIFontSize);
			qApp->setFont(apf);
		}
		emit UpdateRequest(reqDefFontListUpdate);
		if (m_prefsManager->appPrefs.uiPrefs.useTabs)
		{
			mdiArea->setViewMode(QMdiArea::TabbedView);
			mdiArea->setTabsClosable(true);
			mdiArea->setDocumentMode(true);
		}
		else
			mdiArea->setViewMode(QMdiArea::SubWindowView);
		bool shadowChanged = oldPrefs.displayPrefs.showPageShadow != m_prefsManager->showPageShadow();
		QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
		if (!windows.isEmpty())
		{
			int windowCount=windows.count();
			for ( int i = 0; i < windowCount; ++i )
			{
				QWidget* w = windows.at(i)->widget();
				ScribusWin* scw = dynamic_cast<ScribusWin *>(w);
				if (!scw)
					qFatal("ScribusMainWindow::slotPrefsOrg !scw");
				ScribusView* scw_v = scw->view();
				if (oldPrefs.displayPrefs.displayScale != m_prefsManager->displayScale())
				{
					int x = qRound(qMax(scw_v->contentsX() / scw_v->scale(), 0.0));
					int y = qRound(qMax(scw_v->contentsY() / scw_v->scale(), 0.0));
					int w = qRound(qMin(scw_v->visibleWidth() / scw_v->scale(), scw->doc()->currentPage()->width()));
					int h = qRound(qMin(scw_v->visibleHeight() / scw_v->scale(), scw->doc()->currentPage()->height()));
					scw_v->rememberOldZoomLocation(w / 2 + x,h / 2 + y);
					scw_v->zoom((scw_v->scale() / oldPrefs.displayPrefs.displayScale) * m_prefsManager->displayScale());
					zoomSpinBox->setMaximum(doc->opToolPrefs().magMax);
				}
				if (shadowChanged)
					scw->view()->DrawNew();
			}
		}

		QString newMonitorProfile(newPrefs.colorPrefs.DCMSset.DefaultMonitorProfile);
		if (oldMonitorProfile != newMonitorProfile)
		{
			bool success = false;
			if (ScCore->MonitorProfiles.contains(newMonitorProfile))
			{
				QString profilePath = ScCore->MonitorProfiles[newMonitorProfile];
				ScColorProfile newProfile = ScCore->defaultEngine.openProfileFromFile(profilePath);
				if (!newProfile.isNull())
				{
					ScCore->monitorProfile = newProfile;
					success = true;
				}
			}
			if (!success)
			{
				newPrefs.colorPrefs.DCMSset.DefaultMonitorProfile = oldMonitorProfile;
				m_prefsManager->setNewPrefs(newPrefs);
				QString message = tr("An error occurred while opening monitor profile.\nFormer monitor profile will be used." );
				if (ScCore->usingGUI())
					ScMessageBox::warning(this, CommonStrings::trWarning, message);
				else
					qWarning( "%s", message.toLocal8Bit().data() );
			}
		}
		ScImageCacheManager & icm = ScImageCacheManager::instance();
		icm.setEnabled(newPrefs.imageCachePrefs.cacheEnabled);
		icm.setMaxCacheSizeMiB(newPrefs.imageCachePrefs.maxCacheSizeMiB);
		icm.setMaxCacheEntries(newPrefs.imageCachePrefs.maxCacheEntries);
		icm.setCompressionLevel(newPrefs.imageCachePrefs.compressionLevel);

		m_prefsManager->SavePrefs();
	}
}

void ScribusMainWindow::slotDocSetup()
{
	if (!doc)
		return;
	struct ApplicationPrefs oldDocPrefs(doc->prefsData());
	PreferencesDialog prefsDialog(this, oldDocPrefs, doc);
	int prefsResult = prefsDialog.exec();
	if (prefsResult != QDialog::Accepted)
		return;

	struct ApplicationPrefs newDocPrefs(prefsDialog.prefs());
	bool resizePages, resizeMasterPages, resizePageMargins, resizeMasterPageMargins;
	prefsDialog.getResizeDocumentPages(resizePages, resizeMasterPages, resizePageMargins, resizeMasterPageMargins);
	doc->setNewPrefs(newDocPrefs, oldDocPrefs, resizePages, resizeMasterPages, resizePageMargins, resizeMasterPageMargins);

	slotChangeUnit(doc->unitIndex(), false);

	if (oldDocPrefs.itemToolPrefs.imageLowResType!=newDocPrefs.itemToolPrefs.imageLowResType)
	{
		setStatusBarInfoText( tr("Updating Images"));
		mainWindowProgressBar->reset();
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
		qApp->processEvents();
		doc->recalcPicturesRes(true);
		qApp->restoreOverrideCursor();
		setStatusBarInfoText("");
		mainWindowProgressBar->reset();
		viewToolBar->setDoc(doc);
	}
	if (oldDocPrefs.typoPrefs != newDocPrefs.typoPrefs)
	{
		doc->invalidateAll();
	}
	emit UpdateRequest(reqDocFontListUpdate);
	scrActions["viewShowMargins"]->setChecked(doc->guidesPrefs().marginsShown);
	scrActions["viewShowBleeds"]->setChecked(doc->guidesPrefs().showBleed);
	scrActions["viewShowFrames"]->setChecked(doc->guidesPrefs().framesShown);
	scrActions["viewShowLayerMarkers"]->setChecked(doc->guidesPrefs().layerMarkersShown);
	scrActions["viewShowGrid"]->setChecked(doc->guidesPrefs().gridShown);
	scrActions["viewShowGuides"]->setChecked(doc->guidesPrefs().guidesShown);
	scrActions["viewShowColumnBorders"]->setChecked(doc->guidesPrefs().colBordersShown);
	scrActions["viewShowBaseline"]->setChecked(doc->guidesPrefs().baselineGridShown);
	scrActions["viewShowImages"]->setChecked(doc->guidesPrefs().showPic);
	scrActions["viewShowTextChain"]->setChecked(doc->guidesPrefs().linkShown);
	scrActions["viewShowTextControls"]->setChecked(doc->guidesPrefs().showControls);
	scrActions["viewShowRulers"]->setChecked(doc->guidesPrefs().rulersShown);
	scrActions["viewRulerMode"]->setChecked(doc->guidesPrefs().rulerMode);
	scrActions["extrasGenerateTableOfContents"]->setEnabled(doc->hasTOCSetup());
	scrActions["extrasUpdateDocument"]->setEnabled(true);
	scrActions["viewToggleCMS"]->setChecked(doc->HasCMS);
	view->setRulersShown(doc->guidesPrefs().rulersShown);
	//doc emits changed() via this
	doc->setMasterPageMode(true);
	view->reformPages();
	doc->setMasterPageMode(false);
	view->reformPages();
	view->GotoPage(doc->currentPage()->pageNr());
	view->DrawNew();
	pagePalette->rebuildPages();
	emit UpdateRequest(reqCmsOptionsUpdate);
	doc->changed();
	modeToolBar->setDoc(doc);
}

int ScribusMainWindow::ShowSubs()
{
	propertiesPalette->startup();
	textPalette->startup();
	outlinePalette->startup();
	scrapbookPalette->startup();
	bookmarkPalette->startup();
	downloadsPalette->startup();
	pagePalette->startup();
	layerPalette->startup();
	docCheckerPalette->startup();
	alignDistributePalette->startup();
	undoPalette->startup();
	guidePalette->startup();
	inlinePalette->startup();
	charPalette->startup();
	m_styleManager->startup();
	marksManager->startup();
	nsEditor->startup();
	symbolPalette->startup();
#if QT_VERSION < 0x050600
	if (!m_prefsManager->appPrefs.uiPrefs.tabbedPalettes.isEmpty())
	{
		for (int a = 0; a < m_prefsManager->appPrefs.uiPrefs.tabbedPalettes.count(); a++)
		{
			QStringList actTab = m_prefsManager->appPrefs.uiPrefs.tabbedPalettes[a].palettes;
			QDockWidget *container = findChild<QDockWidget *>(actTab[0]);
			if (container)
			{
				for (int i = 1; i < actTab.count(); i++)
				{
					QDockWidget *content = findChild<QDockWidget *>(actTab[i]);
					if (content)
						tabifyDockWidget(container, content);
				}
			}
			QList<QTabBar *> bars = findChildren<QTabBar *>(QString());
			bool found = false;
			for (int i = 0; i < bars.count(); ++i)
			{
				QTabBar *bar = bars[i];
				for (int ii = 0; ii < bar->count(); ii++)
				{
					QObject *obj = (QObject*)bar->tabData(ii).toULongLong();
					if (obj != NULL)
					{
						if (obj->objectName() == container->objectName())
						{
							if (m_prefsManager->appPrefs.uiPrefs.tabbedPalettes[a].activeTab > -1)
							{
								bar->setCurrentIndex(m_prefsManager->appPrefs.uiPrefs.tabbedPalettes[a].activeTab);
								found = true;
								break;
							}
						}
					}
				}
				if (found)
					break;
			}
		}
	}
#else
	if (!m_prefsManager->appPrefs.uiPrefs.tabbedPalettes.isEmpty())
	{
		for (int a = 0; a < m_prefsManager->appPrefs.uiPrefs.tabbedPalettes.count(); a++)
		{
			QStringList actTab = m_prefsManager->appPrefs.uiPrefs.tabbedPalettes[a].palettes;
			QDockWidget *container = findChild<QDockWidget *>(actTab[0]);
			QList<QTabBar *> bars = findChildren<QTabBar *>(QString());
			bool found = false;
			for (int i = 0; i < bars.count(); ++i)
			{
				QTabBar *bar = bars[i];
				for (int ii = 0; ii < bar->count(); ii++)
				{
					QObject *obj = (QObject*)bar->tabData(ii).toULongLong();
					if (obj != NULL)
					{
						if (obj->objectName() == container->objectName())
						{
							if (m_prefsManager->appPrefs.uiPrefs.tabbedPalettes[a].activeTab > -1)
							{
								bar->setCurrentIndex(m_prefsManager->appPrefs.uiPrefs.tabbedPalettes[a].activeTab);
								found = true;
								break;
							}
						}
					}
				}
				if (found)
					break;
			}
		}
	}
#endif
	move(m_prefsManager->appPrefs.uiPrefs.mainWinSettings.xPosition, m_prefsManager->appPrefs.uiPrefs.mainWinSettings.yPosition);
	resize(m_prefsManager->appPrefs.uiPrefs.mainWinSettings.width, m_prefsManager->appPrefs.uiPrefs.mainWinSettings.height);

	// init the toolbars
	fileToolBar->initVisibility();
	editToolBar->initVisibility();
	modeToolBar->initVisibility();
	pdfToolBar->initVisibility();

	activateWindow();
	if (!scriptIsRunning())
		raise();
	return 0;
}

void ScribusMainWindow::doPrintPreview()
{
	if (!HaveDoc)
		return;
	if (!( ScCore->haveGS() || ScCore->isWinGUI() ))
		return;
	if (docCheckerPalette->isIgnoreEnabled())
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = CheckDocument::checkNULL;
		docCheckerPalette->setIgnoreEnabled(false);
		scrActions["toolsPreflightVerifier"]->setChecked(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doPrintPreview()));
	}
	PrefsContext* prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
	QString currentPrinter(prefs->get("CurrentPrn"));
	PrintEngine currentEngine = (PrintEngine) prefs->get("CurrentPrnEngine", "3").toInt();
	if ( PPreview::usePostscriptPreview(currentPrinter, currentEngine) && ( !ScCore->haveGS() ) )
	{
		QString mess(tr("Ghostscript is missing : PostScript Print Preview is not available")+"\n\n");
		ScMessageBox::warning(this, CommonStrings::trWarning, mess);
		return;
	}
	PPreview *dia = new PPreview(this, view, doc, currentPrinter, currentEngine);
	previewDinUse = true;
	connect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
	dia->exec();
	PrefsManager *prefsManager=PrefsManager::instance();
	prefsManager->appPrefs.printPreviewPrefs.PrPr_Mode = dia->EnableCMYK->isChecked();
	prefsManager->appPrefs.printPreviewPrefs.PrPr_AntiAliasing = dia->AntiAlias->isChecked();
	prefsManager->appPrefs.printPreviewPrefs.PrPr_Transparency = dia->AliasTr->isChecked();
	if ( !ScCore->haveTIFFSep() || !dia->postscriptPreview )
	{
		prefsManager->appPrefs.printPreviewPrefs.PrPr_C = dia->EnableCMYK_C->isChecked();
		prefsManager->appPrefs.printPreviewPrefs.PrPr_M = dia->EnableCMYK_M->isChecked();
		prefsManager->appPrefs.printPreviewPrefs.PrPr_Y = dia->EnableCMYK_Y->isChecked();
		prefsManager->appPrefs.printPreviewPrefs.PrPr_K = dia->EnableCMYK_K->isChecked();
	}
	else
	{
		prefsManager->appPrefs.printPreviewPrefs.PrPr_C = dia->flagsVisible["Cyan"]->isChecked();
		prefsManager->appPrefs.printPreviewPrefs.PrPr_M = dia->flagsVisible["Magenta"]->isChecked();
		prefsManager->appPrefs.printPreviewPrefs.PrPr_Y = dia->flagsVisible["Yellow"]->isChecked();
		prefsManager->appPrefs.printPreviewPrefs.PrPr_K = dia->flagsVisible["Black"]->isChecked();
		prefsManager->appPrefs.printPreviewPrefs.PrPr_InkCoverage = dia->EnableInkCover->isChecked();
		prefsManager->appPrefs.printPreviewPrefs.PrPr_InkThreshold = dia->CoverThresholdValue->value();
	}
	disconnect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
	previewDinUse = false;
	delete dia;
	QFile::remove(prefsManager->preferencesLocation()+"/tmp.ps");
	QFile::remove(prefsManager->preferencesLocation()+"/sc.png");
	QDir d(prefsManager->preferencesLocation()+"/", "sc.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint i = 0; i < d.count(); i++)
			QFile::remove(prefsManager->preferencesLocation() +"/" + d[i]);
	}
}

void ScribusMainWindow::printPreview()
{
	if (doc->checkerProfiles()[doc->curCheckProfile()].autoCheck)
	{
		if (scanDocument())
		{
			if (doc->checkerProfiles()[doc->curCheckProfile()].ignoreErrors)
			{
				int t = ScMessageBox::warning(this, CommonStrings::trWarning,
											"<qt>"+ tr("Scribus has detected some errors. Consider using the Preflight Verifier to correct them")+"</qt>",
											QMessageBox::Abort | QMessageBox::Ignore,
											QMessageBox::NoButton,	// GUI default
											QMessageBox::Ignore);	// batch default
				if (t == QMessageBox::Abort)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doPrintPreview()));
				docCheckerPalette->setIgnoreEnabled(true);
				docCheckerPalette->checkMode = CheckDocument::checkPrintPreview;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setChecked(true);
				return;
			}
		}
	}
	doPrintPreview();
}

bool ScribusMainWindow::DoSaveAsEps(QString fn, QString& error)
{
	QStringList spots;
	bool return_value = true;
	ReOrderText(doc, view);
	QMap<QString, QMap<uint, FPointArray> > ReallyUsed;
	ReallyUsed.clear();
	doc->getUsedFonts(ReallyUsed);
	ColorList usedColors;
	doc->getUsedColors(usedColors);
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	PrintOptions options;
	options.pageNumbers.push_back(doc->currentPage()->pageNr()+1);
	options.outputSeparations = false;
	options.separationName = tr("All");
	options.allSeparations = spots;
	options.useColor = true;
	options.mirrorH = false;
	options.mirrorV = false;
	options.doGCR = m_prefsManager->appPrefs.printerPrefs.GCRMode;
	options.setDevParam = false;
	options.doClip = true;
	options.cropMarks = false;
	options.bleedMarks = false;
	options.registrationMarks = false;
	options.colorMarks = false;
	options.markLength = 20.0;
	options.markOffset = 0.0;
	options.bleeds.set(0, 0, 0, 0);
	PSLib *pslib = new PSLib(options, false, m_prefsManager->appPrefs.fontPrefs.AvailFonts, ReallyUsed, usedColors, false, true);
	if (pslib != NULL)
	{
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
		if (pslib->PS_set_file(fn))
		{
			int psRet = pslib->CreatePS(doc, options);
			if (psRet == 1)
			{
				error = pslib->errorMessage();
				return_value = false;
			}
		}
		else
			return_value = false;
		delete pslib;
		qApp->restoreOverrideCursor();
	}
	ScCore->fileWatcher->start();
	return return_value;
}

void ScribusMainWindow::SaveAsEps()
{
	if (doc->checkerProfiles()[doc->curCheckProfile()].autoCheck)
	{
		if (scanDocument())
		{
			if (doc->checkerProfiles()[doc->curCheckProfile()].ignoreErrors)
			{
				int t = ScMessageBox::warning(this, CommonStrings::trWarning,
											tr("Scribus detected some errors.\nConsider using the Preflight Verifier  to correct them."),
											QMessageBox::Abort | QMessageBox::Ignore,
											QMessageBox::NoButton,	// GUI default,
											QMessageBox::Ignore);	// batch default
				if (t == QMessageBox::Abort)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(reallySaveAsEps()));
				docCheckerPalette->setIgnoreEnabled(true);
				docCheckerPalette->checkMode = CheckDocument::checkEPS;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setChecked(true);
				return;
			}
		}
	}
	reallySaveAsEps();
}

void ScribusMainWindow::reallySaveAsEps()
{
	QString filename;
	if (docCheckerPalette->isIgnoreEnabled())
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = CheckDocument::checkNULL;
		docCheckerPalette->setIgnoreEnabled(false);
		scrActions["toolsPreflightVerifier"]->setChecked(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(reallySaveAsEps()));
	}
	if (!doc->DocName.startsWith( tr("Document")))
	{
		QFileInfo fi(doc->DocName);
		if (!doc->m_Selection->isEmpty())
			filename = fi.path() + "/" + fi.completeBaseName() + "_selection.eps";
		else
			filename = fi.path() + "/" + getFileNameByPage(doc, doc->currentPage()->pageNr(), "eps");
	}
	else
	{
		QDir di = QDir();
		if (!doc->m_Selection->isEmpty())
			filename = di.currentPath() + "/" + doc->DocName + "_selection.eps";
		else
			filename = di.currentPath() + "/" + getFileNameByPage(doc, doc->currentPage()->pageNr(), "eps");
	}
	filename = QDir::toNativeSeparators(filename);
	QString wdir = ".";
	QString prefsDocDir=m_prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		wdir = m_prefsManager->prefsFile->getContext("dirs")->get("eps", prefsDocDir);
	else
		wdir = m_prefsManager->prefsFile->getContext("dirs")->get("eps", ".");
	QString fn = CFileDialog( wdir, tr("Save As"), tr("%1;;All Files (*)").arg(m_formatsManager->extensionsForFormat(FormatsManager::EPS)), filename, fdHidePreviewCheckBox | fdNone);
	if (!fn.isEmpty())
	{
		m_prefsManager->prefsFile->getContext("dirs")->set("eps", fn.left(fn.lastIndexOf("/")));
		if (overwrite(this, fn))
		{
			QString epsError;
			if (!DoSaveAsEps(fn, epsError))
			{
				QString message = tr("Cannot write the file: \n%1").arg(fn);
				if (!epsError.isEmpty())
					message += QString("\n%1").arg(epsError);
				ScMessageBox::warning(this, CommonStrings::trWarning, message);
			}
		}
	}
}

bool ScribusMainWindow::getPDFDriver(const QString &filename, const QString &name, int components, const std::vector<int> & pageNumbers,
									 const QMap<int, QImage>& thumbs, QString& error, bool* cancelled)
{
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	PDFlib pdflib(*doc);
	bool ret = pdflib.doExport(filename, name, components, pageNumbers, thumbs);
	if (!ret)
		error = pdflib.errorMessage();
	if (cancelled)
		*cancelled = pdflib.exportAborted();
	ScCore->fileWatcher->start();
	return ret;
}

void ScribusMainWindow::SaveAsPDF()
{
	if (doc->checkerProfiles()[doc->curCheckProfile()].autoCheck)
	{
		if (scanDocument())
		{
			if (doc->checkerProfiles()[doc->curCheckProfile()].ignoreErrors)
			{
				int t = ScMessageBox::warning(this, CommonStrings::trWarning,
											tr("Detected some errors.\nConsider using the Preflight Verifier to correct them"),
											QMessageBox::Abort | QMessageBox::Ignore,
											QMessageBox::NoButton,	// GUI default
											QMessageBox::Ignore);	// batch default
				if (t == QMessageBox::Abort)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doSaveAsPDF()));
				docCheckerPalette->setIgnoreEnabled(true);
				docCheckerPalette->checkMode = CheckDocument::checkPDF;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setChecked(true);
				return;
			}
		}
	}
	doSaveAsPDF();
}

void ScribusMainWindow::doSaveAsPDF()
{
	if (docCheckerPalette->isIgnoreEnabled())
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = CheckDocument::checkNULL;
		docCheckerPalette->setIgnoreEnabled(false);
		scrActions["toolsPreflightVerifier"]->setChecked(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doSaveAsPDF()));
	}
	QMap<QString, int> ReallyUsed = doc->reorganiseFonts();
	if (doc->pdfOptions().EmbedList.count() != 0)
	{
		QList<QString> tmpEm;
		QList<QString>::Iterator itef;
		for (itef = doc->pdfOptions().EmbedList.begin(); itef != doc->pdfOptions().EmbedList.end(); ++itef)
		{
			if (ReallyUsed.contains((*itef)))
				tmpEm.append((*itef));
		}
		doc->pdfOptions().EmbedList = tmpEm;
	}
	if (doc->pdfOptions().SubsetList.count() != 0)
	{
		QList<QString> tmpEm;
		QList<QString>::Iterator itef;
		for (itef = doc->pdfOptions().SubsetList.begin(); itef != doc->pdfOptions().SubsetList.end(); ++itef)
		{
			if (ReallyUsed.contains((*itef)))
				tmpEm.append((*itef));
		}
		doc->pdfOptions().SubsetList = tmpEm;
	}
	MarginStruct optBleeds(doc->pdfOptions().bleeds);
	PDFExportDialog dia(this, doc->DocName, ReallyUsed, view, doc->pdfOptions(), ScCore->PDFXProfiles, m_prefsManager->appPrefs.fontPrefs.AvailFonts, ScCore->PrinterProfiles);
	if (!dia.exec())
		return;

	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	dia.updateDocOptions();
	doc->pdfOptions().firstUse = false;
	ReOrderText(doc, view);
	QString pageString(dia.getPagesString());
	std::vector<int> pageNs;
	uint pageNumbersSize;
	QMap<int, QImage> allThumbs, thumbs;
	int components = dia.colorSpaceComponents();
	QString nam(dia.cmsDescriptor());
	QString fileName = doc->pdfOptions().fileName;
	QString errorMsg;
	parsePagesString(pageString, &pageNs, doc->DocPages.count());
	if (doc->pdfOptions().useDocBleeds)
		doc->pdfOptions().bleeds = *doc->bleeds();

	// If necssary, generate thumbnails in one go : if color management is enabled
	// we gain lots of time by avoiding multiple color management settings change
	// and hence multiple reloading of images
	bool cmsCorr = false;
	if (doc->pdfOptions().Thumbnails &&
		doc->cmsSettings().CMSinUse &&
		doc->cmsSettings().GamutCheck)
	{
		cmsCorr = true;
		doc->cmsSettings().GamutCheck = false;
		doc->enableCMS(true);
	}
	pageNumbersSize = pageNs.size();
	for (uint i = 0; i < pageNumbersSize; ++i)
	{
		QImage thumb(10, 10, QImage::Format_ARGB32_Premultiplied);
		if (doc->pdfOptions().Thumbnails)
		{
			// No need to load full res images for drawing small thumbnail
			PageToPixmapFlags flags = Pixmap_DontReloadImages | Pixmap_DrawWhiteBackground;
			thumb = view->PageToPixmap(pageNs[i] - 1, 100, flags);
		}
		allThumbs.insert(pageNs[i], thumb);
	}
	if (cmsCorr)
	{
		doc->cmsSettings().GamutCheck = true;
		doc->enableCMS(true);
	}

	if (doc->pdfOptions().doMultiFile)
	{
		bool cancelled = false;
		QFileInfo fi(fileName);
		QString ext = fi.suffix();
		QString path = fi.path();
		QString name = fi.completeBaseName();
		uint aa = 0;
		while (aa < pageNs.size() && !cancelled)
		{
			thumbs.clear();
			std::vector<int> pageNs2;
			pageNs2.clear();
			pageNs2.push_back(pageNs[aa]);
			pageNumbersSize = pageNs2.size();
			QImage thumb(10, 10, QImage::Format_ARGB32_Premultiplied);
			if (doc->pdfOptions().Thumbnails)
				thumb = allThumbs[pageNs[aa]];
			thumbs.insert(1, thumb);
			QString realName = QDir::toNativeSeparators(path+"/"+name+ tr("-Page%1").arg(pageNs[aa], 3, 10, QChar('0'))+"."+ext);
			if (!getPDFDriver(realName, nam, components, pageNs2, thumbs, errorMsg, &cancelled))
			{
				qApp->restoreOverrideCursor();
				QString message = tr("Cannot write the file: \n%1").arg(doc->pdfOptions().fileName);
				if (!errorMsg.isEmpty())
					message = QString("%1\n%2").arg(message, errorMsg);
				ScMessageBox::warning(this, CommonStrings::trWarning, message);
				return;
			}
			aa++;
		}
	}
	else
	{
		if (!getPDFDriver(fileName, nam, components, pageNs, allThumbs, errorMsg))
		{
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			QString message = tr("Cannot write the file: \n%1").arg(doc->pdfOptions().fileName);
			if (!errorMsg.isEmpty())
				message = QString("%1\n%2").arg(message, errorMsg);
			ScMessageBox::warning(this, CommonStrings::trWarning, message);
		}
	}
	if (doc->pdfOptions().useDocBleeds)
		doc->pdfOptions().bleeds = optBleeds;
	qApp->restoreOverrideCursor();
	if (errorMsg.isEmpty() && doc->pdfOptions().openAfterExport && !doc->pdfOptions().doMultiFile)
	{
		QString pdfViewer(PrefsManager::instance()->appPrefs.extToolPrefs.pdfViewerExecutable);
		if (pdfViewer.isEmpty())
		{
			pdfViewer = QFileDialog::getOpenFileName(this, tr("Locate your PDF viewer"), QString::null, QString::null);
			if (!QFileInfo::exists(pdfViewer))
				pdfViewer="";
			PrefsManager::instance()->appPrefs.extToolPrefs.pdfViewerExecutable=pdfViewer;
		}
		if (!pdfViewer.isEmpty())
		{
			QStringList args;
			args << QDir::toNativeSeparators(doc->pdfOptions().fileName);
			QProcess::startDetached(pdfViewer, args);
		}
	}
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::AddBookMark(PageItem *ite)
{
	bookmarkPalette->BView->addPageItem(ite);
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::DelBookMark(PageItem *ite)
{
	bookmarkPalette->BView->deleteItem(ite);
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::BookMarkTxT(PageItem *ite)
{
	bookmarkPalette->BView->changeText(ite);
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::RestoreBookMarks()
{
	QList<ScribusDoc::BookMa>::Iterator it2 = doc->BookMarks.begin();
	bookmarkPalette->BView->clear();
	bookmarkPalette->BView->NrItems = 0;
	bookmarkPalette->BView->First = 1;
	bookmarkPalette->BView->Last = 0;
	if (doc->BookMarks.count() == 0)
		return;
	BookMItem* ip;
	BookMItem* ip2 = NULL;
	BookMItem* ip3 = NULL;
	BookMItem *ite = new BookMItem(bookmarkPalette->BView, &(*it2));
	bookmarkPalette->BView->NrItems++;
	++it2;
	for( ; it2 != doc->BookMarks.end(); ++it2 )
	{
		if ((*it2).Parent == 0)
		{
			ite = new BookMItem(bookmarkPalette->BView, ite, &(*it2));
			bookmarkPalette->BView->NrItems++;
		}
		else
		{
			QTreeWidgetItemIterator it3(bookmarkPalette->BView);
			while (*it3)
			{
				ip = (BookMItem*)(*it3);
				if ((*it2).Parent == ip->ItemNr)
				{
					ip2 = ip;
					break;
				}
				++it3;
			}
			if ((*it2).Prev == 0)
			{
				(void) new BookMItem(ip2, &(*it2));
				bookmarkPalette->BView->NrItems++;
			}
			else
			{
				QTreeWidgetItemIterator it4(bookmarkPalette->BView);
				while (*it4)
				{
					ip = (BookMItem*)(*it4);
					if ((*it2).Prev == ip->ItemNr)
					{
						ip3 = ip;
						break;
					}
					++it4;
				}
				(void) new BookMItem(ip2, ip3, &(*it2));
				bookmarkPalette->BView->NrItems++;
			}
		}
	}
	bookmarkPalette->BView->Last = bookmarkPalette->BView->NrItems;
	bookmarkPalette->BView->rebuildTree();
}

QStringList ScribusMainWindow::scrapbookNames()
{
	return scrapbookPalette->getOpenScrapbooksNames();
}

void ScribusMainWindow::updateLayerMenu()
{
	bool b = layerMenu->blockSignals(true);
	layerMenu->clear();
	if (doc==NULL)
	{
		layerMenu->blockSignals(b);
		return;
	}

	QStringList newNames;
	doc->orderedLayerList(&newNames);
	for (QStringList::Iterator it=newNames.begin(); it!=newNames.end(); ++it)
	{
		QPixmap pm(20,15);
		pm.fill(doc->Layers.layerByName(*it)->markerColor);
		layerMenu->addItem(pm, *it);
	}

	if (layerMenu->count() != 0)
	{
		QString layerName = doc->activeLayerName();
		setCurrentComboItem(layerMenu, layerName);
	}

	layerMenu->blockSignals(b);
}


void ScribusMainWindow::GotoLa(int l)
{
	if (!HaveDoc)
		return;
	int level = doc->layerCount()-l-1;
	int layerID=doc->layerIDFromLevel(level);
	if (layerID==-1)
		return;
	doc->setActiveLayer(layerID);
	changeLayer(doc->activeLayer());
	emit changeLayers(layerID);
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::StoreBookmarks()
{
	doc->BookMarks.clear();
	BookMItem* ip;
	QTreeWidgetItemIterator it(bookmarkPalette->BView);
	struct ScribusDoc::BookMa Boma;
	while (*it)
	{
		ip = (BookMItem*)(*it);
		Boma.Title = ip->Title;
		Boma.Text = ip->text(0);
		Boma.Aktion = ip->Action;
		Boma.ItemNr = ip->ItemNr;
		Boma.PageObject = ip->PageObject;
//		Boma.Seite = ip->Seite;
// 		Boma.Element = ip->Element;
		Boma.Parent = ip->Pare;
		Boma.First = ip->First;
		Boma.Prev = ip->Prev;
		Boma.Next = ip->Next;
		Boma.Last = ip->Last;
		doc->BookMarks.append(Boma);
		++it;
	}
	doc->NrItems = bookmarkPalette->BView->NrItems;
	doc->First = bookmarkPalette->BView->First;
	doc->Last = bookmarkPalette->BView->Last;
}

void ScribusMainWindow::slotElemRead(QString xml, double x, double y, bool art, bool loca, ScribusDoc* docc, ScribusView* vie)
{
	if (doc == docc && docc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);

	ScriXmlDoc ss;
	if(ss.ReadElem(xml, m_prefsManager->appPrefs.fontPrefs.AvailFonts, docc, x, y, art, loca, m_prefsManager->appPrefs.fontPrefs.GFontSub))
	{
		vie->DrawNew();
		if (doc == docc)
		{
			emit UpdateRequest(reqColorsUpdate | reqTextStylesUpdate | reqLineStylesUpdate);
			slotDocCh();
		}
	}
}

void ScribusMainWindow::slotChangeUnit(int unitIndex, bool draw)
{
	// Hack
	QByteArray stylesheet;
	if (loadRawText(ScPaths::preferencesDir() + "/stylesheet.css", stylesheet))
	{
		qApp->setStyleSheet(QString(stylesheet));
	}
	doc->setUnitIndex(unitIndex);
	setCurrentComboItem(unitSwitcher, unitGetStrFromIndex(doc->unitIndex()));
	view->unitChange();
	propertiesPalette->unitChange();
	textPalette->unitChange();
	nodePalette->unitChange();
	alignDistributePalette->unitChange();
	guidePalette->setupPage();
	m_styleManager->unitChange();
	if (draw)
		view->DrawNew();
}

void ScribusMainWindow::ManageJava()
{
	JavaDocs *dia = new JavaDocs(this, doc, view);
	connect(dia, SIGNAL(docChanged(bool)), this, SLOT(slotDocCh(bool )));
	dia->exec();
	disconnect(dia, SIGNAL(docChanged(bool)), this, SLOT(slotDocCh(bool )));
	delete dia;
}

void ScribusMainWindow::editSelectedSymbolStart()
{
	if (doc->m_Selection->count() > 0)
		editSymbolStart(doc->m_Selection->itemAt(0)->pattern());
}

void ScribusMainWindow::editSymbolStart(QString temp)
{
	if (!HaveDoc || !doc->docPatterns.contains(temp))
		return;
	if (doc->symbolEditMode())
	{
		QString editedSymbol = doc->getEditedSymbol();
		if (editedSymbol == temp)
			return;
		editSymbolEnd();
	}
	m_WasAutoSave = doc->autoSave();
	if (m_WasAutoSave)
	{
		doc->autoSaveTimer->stop();
		doc->setAutoSave(false);
	}
	view->Deselect(true);
	m_storedPageNum = doc->currentPageNumber();
	m_storedViewXCoor = view->contentsX();
	m_storedViewYCoor = view->contentsY();
	m_storedViewScale = view->scale();
	doc->stored_minCanvasCoordinate = doc->minCanvasCoordinate;
	doc->stored_maxCanvasCoordinate = doc->maxCanvasCoordinate;
	view->showSymbolPage(temp);
	appModeHelper->setSymbolEditMode(true, doc);
	pagePalette->enablePalette(false);
	layerPalette->setEnabled(false);
	patternsDependingOnThis.clear();
	QStringList mainPatterns = doc->docPatterns.keys();
	for (int a = 0; a < mainPatterns.count(); a++)
	{
		if (mainPatterns[a] != temp)
		{
			QStringList subPatterns;
			subPatterns = doc->getUsedPatternsHelper(mainPatterns[a], subPatterns);
			if (subPatterns.contains(temp))
				patternsDependingOnThis.prepend(mainPatterns[a]);
		}
	}
	patternsDependingOnThis.prepend(temp);
	symbolPalette->editingStart(patternsDependingOnThis);
	propertiesPalette->Cpal->hideEditedPatterns(patternsDependingOnThis);
	propertiesPalette->Tpal->hideEditedPatterns(patternsDependingOnThis);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree(false);
	updateActiveWindowCaption( tr("Editing Symbol: %1").arg(temp));
}

void ScribusMainWindow::editSymbolEnd()
{
	doc->minCanvasCoordinate = doc->stored_minCanvasCoordinate;
	doc->maxCanvasCoordinate = doc->stored_maxCanvasCoordinate;
	view->setScale(m_storedViewScale);
	view->hideSymbolPage();
	if (m_WasAutoSave)
	{
		doc->setAutoSave(true);
		doc->restartAutoSaveTimer();
	}
	slotSelect();
	appModeHelper->setSymbolEditMode(false, doc);

	if ( ScCore->haveGS() || ScCore->isWinGUI() )
		scrActions["PrintPreview"]->setEnabled(true);
	pagePalette->enablePalette(true);
	pagePalette->rebuildMasters();
	view->setScale(m_storedViewScale);
	// #12857 : the number of pages may change when undoing/redoing
	// page addition/deletion while in edit mode, so take some extra
	// care so that storedPageNum is in appropriate range
	m_storedPageNum = qMin(m_storedPageNum, doc->DocPages.count() - 1);
	doc->setCurrentPage(doc->DocPages.at(m_storedPageNum));
	view->setContentsPos(static_cast<int>(m_storedViewXCoor * m_storedViewScale), static_cast<int>(m_storedViewYCoor * m_storedViewScale));
	view->DrawNew();
	pagePalette->Rebuild();
	propertiesPalette->updateColorList();
	textPalette->updateColorList();
	symbolPalette->editingFinished();
	layerPalette->setEnabled(true);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree(false);
	updateActiveWindowCaption(doc->DocName);
}

void ScribusMainWindow::editInlineStart(int id)
{
	if (!HaveDoc)
		return;
	m_WasAutoSave = doc->autoSave();
	if (m_WasAutoSave)
	{
		doc->autoSaveTimer->stop();
		doc->setAutoSave(false);
	}
	if (doc->m_Selection->count() == 1)
		doc->currentEditedTextframe = doc->m_Selection->itemAt(0);
	else
		doc->currentEditedTextframe = NULL;
	view->Deselect(true);
	m_storedPageNum = doc->currentPageNumber();
	m_storedViewXCoor = view->contentsX();
	m_storedViewYCoor = view->contentsY();
	m_storedViewScale = view->scale();
	doc->stored_minCanvasCoordinate = doc->minCanvasCoordinate;
	doc->stored_maxCanvasCoordinate = doc->maxCanvasCoordinate;
	view->showInlinePage(id);
	appModeHelper->setInlineEditMode(true, doc);
	pagePalette->enablePalette(false);
	layerPalette->setEnabled(false);
	inlinePalette->editingStart(id);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree(false);
	updateActiveWindowCaption( tr("Editing Inline Item"));
}

void ScribusMainWindow::editInlineEnd()
{
	doc->minCanvasCoordinate = doc->stored_minCanvasCoordinate;
	doc->maxCanvasCoordinate = doc->stored_maxCanvasCoordinate;
	view->setScale(m_storedViewScale);
	view->hideInlinePage();
	if (m_WasAutoSave)
	{
		doc->setAutoSave(true);
		doc->restartAutoSaveTimer();
	}
	slotSelect();
	appModeHelper->setInlineEditMode(false, doc);
	pagePalette->enablePalette(true);
	pagePalette->rebuildMasters();
	view->setScale(m_storedViewScale);
	// #12857 : the number of pages may change when undoing/redoing
	// page addition/deletion while in edit mode, so take some extra
	// care so that storedPageNum is in appropriate range
	m_storedPageNum = qMin(m_storedPageNum, doc->DocPages.count() - 1);
	doc->setCurrentPage(doc->DocPages.at(m_storedPageNum));
	view->setContentsPos(static_cast<int>(m_storedViewXCoor * m_storedViewScale), static_cast<int>(m_storedViewYCoor * m_storedViewScale));
	doc->invalidateAll();
//	if (doc->currentEditedTextframe != NULL)
//		doc->currentEditedTextframe->invalidateLayout();
	doc->currentEditedTextframe = NULL;
	view->DrawNew();
	pagePalette->Rebuild();
	propertiesPalette->unsetItem();
	propertiesPalette->updateColorList();
	textPalette->unsetItem();
	textPalette->updateColorList();
	inlinePalette->editingFinished();
	layerPalette->setEnabled(true);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree(false);
	updateActiveWindowCaption(doc->DocName);
}

void ScribusMainWindow::editMasterPagesStart(QString temp)
{
	if (!HaveDoc)
		return;
	m_pagePalVisible = pagePalette->isVisible();
	QString mpName;
	if (temp.isEmpty())
		mpName = doc->currentPage()->MPageNam;
	else
		mpName = temp;
	view->Deselect(true);
	if (doc->drawAsPreview)
	{
		view->togglePreview(false);
		scrActions["viewPreviewMode"]->setChecked(false);
	}
	m_WasAutoSave = doc->autoSave();
	if (m_WasAutoSave)
	{
		doc->autoSaveTimer->stop();
		doc->setAutoSave(false);
	}

	if (doc->masterPageMode())
	{
		pagePalette->startMasterPageMode(mpName);
		return;
	}

	m_storedPageNum = doc->currentPageNumber();
	m_storedViewXCoor = view->contentsX();
	m_storedViewYCoor = view->contentsY();
	m_storedViewScale = view->scale();
	doc->stored_minCanvasCoordinate = doc->minCanvasCoordinate;
	doc->stored_maxCanvasCoordinate = doc->maxCanvasCoordinate;

	pagePalette->startMasterPageMode(mpName);
	if (!pagePalette->isVisible())
	{
		pagePalette->show();
		scrActions["toolsPages"]->setChecked(true);
	}
	appModeHelper->setMasterPageEditMode(true, doc);
}

void ScribusMainWindow::editMasterPagesEnd()
{
	view->setScale(m_storedViewScale);
	doc->minCanvasCoordinate = doc->stored_minCanvasCoordinate;
	doc->maxCanvasCoordinate = doc->stored_maxCanvasCoordinate;
	view->hideMasterPage();
	if (m_WasAutoSave)
	{
		doc->setAutoSave(true);
		doc->restartAutoSaveTimer();
	}
	slotSelect();
	appModeHelper->setMasterPageEditMode(false, doc);
	int pageCount=doc->DocPages.count();
	for (int i=0; i<pageCount; ++i)
		Apply_MasterPage(doc->DocPages.at(i)->MPageNam, i, false);

	pagePalette->endMasterPageMode();
	if (pagePalette->isFloating())
	{
		pagePalette->setVisible(m_pagePalVisible);
		scrActions["toolsPages"]->setChecked(m_pagePalVisible);
	}
	// #12857 : the number of pages may change when undoing/redoing
	// page addition/deletion while in edit mode, so take some extra
	// care so that storedPageNum is in appropriate range
	m_storedPageNum = qMin(m_storedPageNum, doc->DocPages.count() - 1);
	doc->setCurrentPage(doc->DocPages.at(m_storedPageNum));
	doc->minCanvasCoordinate = doc->stored_minCanvasCoordinate;
	doc->maxCanvasCoordinate = doc->stored_maxCanvasCoordinate;
	doc->setLoading(true);
	view->reformPages(false);
	view->setContentsPos(static_cast<int>(m_storedViewXCoor * m_storedViewScale), static_cast<int>(m_storedViewYCoor * m_storedViewScale));
	doc->setLoading(false);
	view->DrawNew();
}

void ScribusMainWindow::ApplyMasterPage()
{
	Q_ASSERT(!doc->masterPageMode());

	QScopedPointer<ApplyMasterPageDialog> dia(new ApplyMasterPageDialog(this));
	dia->setup(doc, doc->currentPage()->MPageNam);
	if (!dia->exec())
		return;

	QString masterPageName = dia->getMasterPageName();
	int pageSelection = dia->getPageSelection(); //0=current, 1=even, 2=odd, 3=all
	if (pageSelection == 0) //current page only
		Apply_MasterPage(masterPageName, doc->currentPage()->pageNr(), false);
	else
	{
		int startPage, endPage;
		if (dia->usingRange())
		{
			startPage = dia->getFromPage()-1; //Pages start from 0, not 1
			endPage = dia->getToPage();
		}
		else
		{
			startPage = pageSelection==1 ? 1 : 0; //if even, startPage is 1 (real page 2)
			endPage = doc->DocPages.count();
		}
		for (int pageNum = startPage; pageNum < endPage; ++pageNum)// +=pageStep)
		{
			//Increment by 1 and not 2 even for even/odd application as user
			//can select to eg apply to even pages with a single odd page selected
			if (pageSelection == 1 && (pageNum %2 != 0)) //Even, %2!=0 as 1st page is numbered 0
				Apply_MasterPage(masterPageName, pageNum, false);
			else
			if (pageSelection == 2 && (pageNum %2 == 0)) //Odd, %2==0 as 1st page is numbered 0
				Apply_MasterPage(masterPageName, pageNum, false);
			else
			if (pageSelection == 3) //All
				Apply_MasterPage(masterPageName, pageNum, false);
		}
	}

	view->reformPages();
	view->DrawNew();
	pagePalette->Rebuild();
	// #9476 : call setupPage with false arg to setup only guidePalette GUI
	// Otherwise setupPage() will apply guides to current page, doesn't need that, 
	// Apply_MasterPage() has already done it
	guidePalette->setupPage(false);
}

void ScribusMainWindow::Apply_MasterPage(QString pageName, int pageNumber, bool reb)
{
	if (!HaveDoc)
		return;
	doc->applyMasterPage(pageName, pageNumber);
	if (reb)
		view->DrawNew();
}

//CB-->Doc
void ScribusMainWindow::GroupObj(bool showLockDia)
{
	if (!HaveDoc)
		return;
	Selection* itemSelection = doc->m_Selection;
	if (itemSelection->count() < 2)
		return;
	bool lockObject = false;
	bool modifyLock = false;
	int selectedItemCount=itemSelection->count();
	if (showLockDia)
	{
		int lockedCount=0;
		for (int i=0; i<selectedItemCount; ++i)
		{
			if (itemSelection->itemAt(i)->locked())
				++lockedCount;
		}
		if (lockedCount!=0 && lockedCount!=selectedItemCount)
		{
			ScMessageBox msgBox;
			QPushButton *abortButton = msgBox.addButton(QMessageBox::Cancel);
			QPushButton *lockButton = msgBox.addButton(tr("&Lock All"), QMessageBox::AcceptRole);
			msgBox.addButton(tr("&Unlock All"), QMessageBox::AcceptRole);
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setWindowTitle(CommonStrings::trWarning);
			msgBox.setText( tr("Some objects are locked."));
			msgBox.setDefaultBatchButton(lockButton);
			msgBox.exec();
			if (msgBox.clickedButton() == abortButton)
				return;
			else if (msgBox.clickedButton() == lockButton)
				lockObject = true;
			modifyLock = true;
		}
	}
	doc->itemSelection_GroupObjects(modifyLock, lockObject);
}

//CB-->Doc
void ScribusMainWindow::UnGroupObj()
{
	if (HaveDoc)
		doc->itemSelection_UnGroupObjects();
}

void ScribusMainWindow::AdjustGroupObj()
{
	if (HaveDoc)
		doc->itemSelection_resizeGroupToContents();
}

void ScribusMainWindow::restore(UndoState* state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (ss)
	{
		if (ss->contains("ADD_PAGE"))
			restoreAddPage(ss, isUndo);
		else if (ss->contains("DELETE_PAGE"))
			restoreDeletePage(ss, isUndo);
	}
}

void ScribusMainWindow::restoreDeletePage(SimpleState *state, bool isUndo)
{
	int where, wo;
	int pagenr = state->getUInt("PAGENR");
	QStringList tmpl;
	tmpl << state->get("MASTERPAGE");
	QString pageName = state->get("PAGENAME");
	bool savedMasterPageMode = state->getBool("MASTER_PAGE_MODE");
	bool currMasterPageMode=doc->masterPageMode();
	if (currMasterPageMode!=savedMasterPageMode)
		doc->setMasterPageMode(savedMasterPageMode);
	if (pagenr == 1)
	{
		where = 0;
		wo = 1;
	}
	else if (pagenr > doc->Pages->count())
	{
		where = 2;
		wo = doc->Pages->count();
	}
	else
	{
		where = 1;
		wo = pagenr - 1;
	}

	if (isUndo)
	{
		if (savedMasterPageMode)
		{
			slotNewMasterPage(wo, pageName);
		}
		else
		{
			addNewPages(wo, where, 1, doc->pageHeight(), doc->pageWidth(), doc->pageOrientation(), doc->pageSize(), true, &tmpl);
		}
		UndoObject *tmp =
			m_undoManager->replaceObject(state->getUInt("DUMMY_ID"), doc->Pages->at(pagenr - 1));
		delete tmp;
	}
	else
	{
		DummyUndoObject *duo = new DummyUndoObject();
		uint id = static_cast<uint>(duo->getUId());
		m_undoManager->replaceObject(doc->Pages->at(pagenr - 1)->getUId(), duo);
		state->set("DUMMY_ID", id);
		deletePage(pagenr, pagenr);
	}
	if (currMasterPageMode!=savedMasterPageMode)
		doc->setMasterPageMode(currMasterPageMode);
	doc->rebuildMasterNames();
	pagePalette->updateMasterPageList();
	pagePalette->rebuildPages();
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
}

void ScribusMainWindow::restoreAddPage(SimpleState *state, bool isUndo)
{
	if (!HaveDoc)
		return;
	int wo    = state->getInt("PAGE");
	int where = state->getInt("WHERE");
	int count = state->getInt("COUNT");
	QStringList based = state->get("BASED").split("|", QString::SkipEmptyParts);
	double height = state->getDouble("HEIGHT");
	double width = state->getDouble("WIDTH");
	int orient = state->getInt("ORIENT");
	QString siz = state->get("SIZE");
	bool mov = static_cast<bool>(state->getInt("MOVED"));
	bool savedMasterPageMode = state->getBool("MASTER_PAGE_MODE");

	int delFrom = 0;
	int delTo = 0;
	bool currMasterPageMode=doc->masterPageMode();
	if (currMasterPageMode!=savedMasterPageMode)
		doc->setMasterPageMode(savedMasterPageMode);
	switch (where)
	{
		case 0:
			delTo = wo + count - 1;
			delFrom = delTo - count + 1;
			break;
		case 1:
			delFrom = wo + 1;
			delTo = wo + count;
			break;
		case 2:
			delTo = doc->Pages->count();
			delFrom = doc->Pages->count() - count + 1;
			if (!isUndo)
			{
				delFrom += count;
				delTo   += count;
			}
			break;
	}

	if (isUndo)
	{
		for (int i = delFrom - 1; i < delTo; ++i)
		{
			DummyUndoObject *duo = new DummyUndoObject();
			ulong did = duo->getUId();
			m_undoManager->replaceObject(doc->Pages->at(i)->getUId(), duo);
			state->set(QString("Page%1").arg(i), static_cast<uint>(did));
		}
		if (doc->appMode == modeEditClip)
			view->requestMode(submodeEndNodeEdit);
		view->Deselect(true);
		deletePage(delFrom, delTo);
	}
	else
	{
		if (savedMasterPageMode)
		{
			assert (count == 1);
			slotNewMasterPage(wo, based[0]);
		}
		else
		{
			addNewPages(wo, where, count, height, width, orient, siz, mov, &based);
		}
		for (int i = delFrom - 1; i < delTo; ++i)
		{
			UndoObject *tmp = m_undoManager->replaceObject(state->getUInt(QString("Page%1").arg(i)), doc->Pages->at(i));
			delete tmp;
		}
	}
	if (currMasterPageMode!=savedMasterPageMode)
		doc->setMasterPageMode(currMasterPageMode);
	doc->rebuildMasterNames();
	pagePalette->updateMasterPageList();
	pagePalette->rebuildPages();
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
}

void ScribusMainWindow::restoreGrouping(SimpleState *state, bool isUndo)
{
	int itemCount = state->getInt("itemcount");
	view->Deselect();
	for (int i = 0; i < itemCount; ++i)
	{
		int itemNr = doc->getItemNrfromUniqueID(state->getUInt(QString("item%1").arg(i)));
		if (doc->Items->at(itemNr)->uniqueNr == state->getUInt(QString("item%1").arg(i)))
			view->SelectItemNr(itemNr);
	}
	if (isUndo)
		UnGroupObj();
	else
		GroupObj(false);
}

void ScribusMainWindow::restoreUngrouping(SimpleState *state, bool isUndo)
{
	int itemCount = state->getInt("itemcount");
	view->Deselect();
	for (int i = 0; i < itemCount; ++i)
	{
		int itemNr = doc->getItemNrfromUniqueID(state->getUInt(QString("item%1").arg(i)));
		if (doc->Items->at(itemNr)->uniqueNr == state->getUInt(QString("item%1").arg(i)))
			view->SelectItemNr(itemNr);
	}
	if (isUndo)
		GroupObj(false);
	else
		UnGroupObj();
}

void ScribusMainWindow::StatusPic()
{
	if (!HaveDoc)
		return;
	PicStatus *dia = new PicStatus(this, doc);
	connect(dia, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(dia, SIGNAL(selectMasterPage(QString)), this, SLOT(editMasterPagesStart(QString)));
	connect(dia, SIGNAL(selectElementByItem(PageItem *, bool, int )), this, SLOT(selectItemsFromOutlines(PageItem *, bool, int)));
	dia->exec();
	delete dia;
}

QString ScribusMainWindow::CFileDialog(QString workingDirectory, QString dialogCaption, QString fileFilter, QString defaultFilename, int optionFlags, bool *useCompression, bool *useFonts, bool *useProfiles)
{
	// changed from "this" to qApp->activeWindow() to be sure it will be opened
	// with the current active window as parent. E.g. it won't hide StoryEditor etc. -- PV
	CustomFDialog *dia = new CustomFDialog(qApp->activeWindow(), workingDirectory, dialogCaption, fileFilter, optionFlags);
	if (!defaultFilename.isEmpty())
	{
		QFileInfo f(defaultFilename);
		dia->setExtension(f.suffix());
		dia->setZipExtension(f.suffix() + ".gz");
		dia->setSelection(defaultFilename);
		if (useCompression != NULL && dia->SaveZip != NULL)
			dia->SaveZip->setChecked(*useCompression);
	}
	if (optionFlags & fdDirectoriesOnly)
	{
		if (useCompression != NULL && dia->SaveZip != NULL)
			dia->SaveZip->setChecked(*useCompression);
		if (useFonts != NULL)
			dia->WithFonts->setChecked(*useFonts);
		if (useProfiles != NULL)
			dia->WithProfiles->setChecked(*useProfiles);
	}
	QString retval("");
	if (dia->exec() == QDialog::Accepted)
	{
		LoadEnc = "";
		if (!(optionFlags & fdDirectoriesOnly))
		{
			LoadEnc = (optionFlags & fdShowCodecs) ? dia->TxCodeM->currentText() : QString("");
			if (optionFlags & fdCompressFile)
			{
				if (dia->SaveZip->isChecked())
					dia->handleCompress();
			}
		}
		else
		{
			if (useCompression != NULL && dia->SaveZip != NULL)
				*useCompression = dia->SaveZip->isChecked();
			if (useFonts != NULL)
				*useFonts = dia->WithFonts->isChecked();
			if (useProfiles != NULL)
				*useProfiles = dia->WithProfiles->isChecked();
		}
		this->repaint();
		retval = dia->selectedFile();
		qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	}
	delete dia;
	return retval;
}



void ScribusMainWindow::recalcColors(QProgressBar *dia)
{
	if (!HaveDoc)
		return;
	doc->recalculateColors();
	propertiesPalette->updateColorList();
	textPalette->updateColorList();
}

void ScribusMainWindow::ModifyAnnot()
{
	if (doc->m_Selection->isEmpty())
		return;

	PageItem *currItem = doc->m_Selection->itemAt(0);
	if ((currItem->annotation().Type() == 0) || (currItem->annotation().Type() == 1) || ((currItem->annotation().Type() > 9) && (currItem->annotation().Type() < 13)))
	{
		int AnType = currItem->annotation().Type();
		int AnActType = currItem->annotation().ActionType();
		QString AnAction = currItem->annotation().Action();
		QString An_Extern = currItem->annotation().Extern();
		Annota *dia = new Annota(this, currItem, doc->DocPages.count(), static_cast<int>(doc->pageWidth()), static_cast<int>(doc->pageHeight()), view);
		if (dia->exec())
			slotDocCh();
		else
		{
			currItem->annotation().setType(AnType);
			currItem->annotation().setActionType(AnActType);
			currItem->annotation().setAction(AnAction);
			currItem->annotation().setExtern(An_Extern);
		}
		delete dia;
	}
	else
	{
		ScAnnot *dia = new ScAnnot(this, currItem, doc->DocPages.count(), static_cast<int>(doc->pageWidth()), static_cast<int>(doc->pageHeight()), doc->PageColors, view);
		if (dia->exec())
			slotDocCh();
		delete dia;
	}
	currItem->update();
}

void ScribusMainWindow::SetShortCut()
{
	for (QMap<QString,Keys>::Iterator it = m_prefsManager->appPrefs.keyShortcutPrefs.KeyActions.begin(); it != m_prefsManager->appPrefs.keyShortcutPrefs.KeyActions.end(); ++it )
	{
		if (!it.value().actionName.isEmpty())
			if (scrActions[it.value().actionName])
				scrActions[it.value().actionName]->setShortcut(it.value().keySequence);
	}
}

void ScribusMainWindow::PutScrap(int scID)
{
	if (!HaveDoc)
		return;
	if (doc->m_Selection->isEmpty())
		return;
	ScriXmlDoc ss;
	QString objectString = ss.WriteElem(doc, doc->m_Selection);
	QDomDocument docu("scridoc");
	docu.setContent(objectString);
	QDomElement elem = docu.documentElement();
	QDomNode DOC = elem.firstChild();
	bool first = true;
	DOC = elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg = DOC.toElement();
		if(pg.tagName() == "ITEM")
		{
			if (first)
				pg.setAttribute("ANNAME", doc->m_Selection->itemAt(0)->itemName());
			first = false;
		}
		DOC = DOC.nextSibling();
	}
	objectString = docu.toString();
	scrapbookPalette->ObjFromMainMenu(objectString, scID);
 }

void ScribusMainWindow::changeLayer(int )
{
	if (doc->appMode == modeEdit)
		slotSelect();
	else if (doc->appMode == modeEditClip)
		NoFrameEdit();
	view->Deselect(true);
	rebuildLayersList();
	layerPalette->rebuildList();
	layerPalette->markActiveLayer();
	updateLayerMenu();
	view->DrawNew();
	bool setter = !doc->layerLocked( doc->activeLayer() );
	scrMenuMgr->setMenuEnabled("EditPasteRecent", ((scrapbookPalette->tempBView->objectMap.count() > 0) && (setter)));
	scrMenuMgr->setMenuEnabled("Insert", setter);
	scrMenuMgr->setMenuEnabled("ItemLayer", doc->layerCount() > 1);
	appModeHelper->changeLayer(doc, (ScMimeData::clipboardHasScribusData() || (scrapbookPalette->tempHasContents())));
}

void ScribusMainWindow::setLayerMenuText(const QString &layerName)
{
	bool b = layerMenu->blockSignals(true);
	if (layerMenu->count() != 0)
		setCurrentComboItem(layerMenu, layerName);
	layerMenu->blockSignals(b);
}

void ScribusMainWindow::showLayer()
{
	view->DrawNew();
}

//TODO: use this only from this class, or just from doc->setcurrentpage
void ScribusMainWindow::slotSetCurrentPage(int Seite)
{
	if (scriptIsRunning())
		return;
	pageSelector->blockSignals(true);
	pageSelector->setMaximum(doc->masterPageMode() ? 1 : doc->Pages->count());
	if ((!doc->isLoading()) && (!doc->masterPageMode()))
		pageSelector->setGUIForPage(Seite);
	pageSelector->blockSignals(false);
}

void ScribusMainWindow::setCurrentPage(int p)
{
	doc->view()->Deselect();
	int p0=p-1; //p is what the user sees.. p0 is our count from 0
	doc->setCurrentPage(doc->Pages->at(p0));
	if (scriptIsRunning())
		return;
	slotSetCurrentPage(p0);
	doc->view()->SetCPo(doc->currentPage()->xOffset() - 10, doc->currentPage()->yOffset() - 10);
	HaveNewSel();
	doc->view()->setFocus();
}

void ScribusMainWindow::initHyphenator()
{
	//Build our list of hyphenation dictionaries we have in the install dir
	//Grab the language abbreviation from it, get the full language text
	//Insert the name as key and a new string list into the map
//	QString hyphDirName = QDir::toNativeSeparators(ScPaths::instance().dictDir()+"/hyph/");
//	QDir hyphDir(hyphDirName, "hyph*.dic", QDir::Name, QDir::Files | QDir::NoSymLinks);
//IL	if ((hyphDir.exists()) && (hyphDir.count() != 0))
//IL	{
// 		LanguageManager langmgr;
// 		langmgr.init(false);
//IL		QString languageOfHyphFile;
//IL		for (uint dc = 0; dc < hyphDir.count(); ++dc)
//IL		{
//IL			QFileInfo fi(hyphDir[dc]);
//IL			QString fileLangAbbrev=fi.baseName().section('_', 1);
//IL			InstLang.insert(fileLangAbbrev, QStringList());
//<<hunspell
//			languageOfHyphFile = LanguageManager::instance()->getLangFromAbbrev(fileLangAbbrev, false);
//			InstLang.insert(languageOfHyphFile, QStringList());
//>>hunspell
//IL		}
//IL	}

	//For each qm file existing, load the file and find the translations of the names
	QString transPath = ScPaths::instance().translationDir();
	QDir transDir(transPath, "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((transDir.exists()) && (transDir.count() != 0))
	{
		for (uint i = 0; i < transDir.count(); i++)
		{
			QFileInfo fi(transPath + transDir[i]);
			QString ext(fi.suffix().toLower());
			if (ext == "qm")
			{
 //IL   			QTranslator *trans = new QTranslator(0);
//IL				trans->load(pfad + d2[dc]);

//IL				QString translatedLang;
//IL				for (QMap<QString, QStringList>::Iterator it=InstLang.begin(); it!=InstLang.end(); ++it)
//IL				{
//IL					translatedLang="";
//IL					translatedLang = trans->translate("QObject", LanguageManager::instance()->getLangFromAbbrev(it.key(), false).toLocal8Bit().data(), "");
//IL					if (!translatedLang.isEmpty())
//IL						it.value().append(translatedLang);
//IL				}
//IL				delete trans;
			}
		}
	}
	//For each hyphenation file, grab the strings and the hyphenation data.
//	QString lang = QString(QLocale::system().name()).left(2);
//	m_prefsManager->appPrefs.hyphPrefs.Language = "en_GB";
//	if (!LanguageManager::instance()->getHyphFilename(lang).isEmpty() )
//		m_prefsManager->appPrefs.hyphPrefs.Language = lang;

/*
	if ((hyphDir.exists()) && (hyphDir.count() != 0))
	{
		LanguageManager *langmgr(LanguageManager::instance());
// 		langmgr.init(false);
		QString tLang = "";
		for (uint dc = 0; dc < hyphDir.count(); ++dc)
		{
			QFileInfo fi(hyphDir[dc]);
			QString fileLangAbbrev = fi.baseName().section('_', 1);
			tLang = langmgr->getLangFromAbbrev(fileLangAbbrev);
//IL			LangTransl.insert(fileLangAbbrev, tLang);
			langmgr->addHyphLang(fileLangAbbrev, hyphDir[dc]);
			if (fileLangAbbrev == lang)
				prefsManager->appPrefs.hyphPrefs.Language = fileLangAbbrev;
		}
		if (tLang.isEmpty())
			prefsManager->appPrefs.hyphPrefs.Language = "en_GB";
	}
	*/
}

void ScribusMainWindow::ImageEffects()
{
	if (!HaveDoc)
		return;
	if (doc->m_Selection->isEmpty())
		return;
	PageItem *currItem = doc->m_Selection->itemAt(0);
	EffectsDialog* dia = new EffectsDialog(this, currItem, doc);
	if (dia->exec())
		doc->itemSelection_ApplyImageEffects(dia->effectsList);
	delete dia;
}

QString ScribusMainWindow::fileCollect(bool compress, bool withFonts, const bool withProfiles, const QString& )
{
	if ((doc->hasName) && doc->DocName.endsWith(".gz"))
		compress=true;
	CollectForOutput_UI c(this, doc, QString::null, withFonts, withProfiles, compress);
	QString newFileName;
	QString errorMsg=c.collect(newFileName);
	qDebug()<<errorMsg;
	return newFileName;
}

void ScribusMainWindow::docCheckToggle(bool visible)
{
	if (!visible)
	{
		if (docCheckerPalette->checkMode == CheckDocument::checkPDF)
			disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doSaveAsPDF()));
		if (docCheckerPalette->checkMode == CheckDocument::checkEPS)
			disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(reallySaveAsEps()));
		if (docCheckerPalette->checkMode == CheckDocument::checkPrint)
			disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(slotReallyPrint()));
		if (docCheckerPalette->checkMode == CheckDocument::checkPrintPreview)
			disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doPrintPreview()));
		docCheckerPalette->setIgnoreEnabled(false);
		docCheckerPalette->checkMode = CheckDocument::checkNULL;
	}
	else
	{
		if (HaveDoc)
		{
			scanDocument();
			docCheckerPalette->buildErrorList(doc);
		}
	}
}

bool ScribusMainWindow::scanDocument()
{
	DocumentChecker dc;
	return dc.checkDocument(doc);
}

void ScribusMainWindow::slotStoryEditor(bool fromTable)
{
	if (doc->m_Selection->isEmpty())
		return;

	PageItem *currItem = doc->m_Selection->itemAt(0);
	PageItem *i2 = currItem;
	if (fromTable)
		i2 = currItem->asTable()->activeCell().textFrame();
	PageItem *currItemSE = storyEditor->currentItem();
	ScribusDoc *currDocSE = storyEditor->currentDocument();
	storyEditor->activFromApp = true;
	//CB shouldn't these be after the if?
	//Why are we resetting the doc and item in this case. My original code didn't do this.
	storyEditor->setCurrentDocumentAndItem(doc, i2);
	if (i2 == currItemSE && doc == currDocSE)
	{
		storyEditor->show();
		storyEditor->raise();
		return;
	}
	CurrStED = storyEditor;
	connect(storyEditor, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	storyEditor->show();
	storyEditor->raise();
}

void ScribusMainWindow::emergencySave()
{
	emergencyActivated=true;
	if (!m_prefsManager || !m_prefsManager->appPrefs.miscPrefs.saveEmergencyFile)
		return;
	std::cout << "Calling Emergency Save" << std::endl;
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	if (windows.isEmpty())
		return;

	int windowCount = windows.count();
	for (int i=0; i<windowCount ; ++i)
	{
		ActWin = dynamic_cast<ScribusWin*>(windows.at(i)->widget());
		doc = ActWin->doc();
		view = ActWin->view();
		doc->autoSaveTimer->stop();
		doc->setMasterPageMode(false);
		doc->setModified(false);
		QString base = tr("Document");
		QString path = m_prefsManager->documentDir();
		QString fileName;
		if (doc->hasName)
		{
			QFileInfo fi(doc->DocName);
			base = fi.baseName();
			path = fi.absolutePath();
		}
		QDateTime dat = QDateTime::currentDateTime();
		if ((!doc->prefsData().docSetupPrefs.AutoSaveLocation) && (!doc->prefsData().docSetupPrefs.AutoSaveDir.isEmpty()))
			path = doc->prefsData().docSetupPrefs.AutoSaveDir;
		fileName = QDir::cleanPath(path + "/" + base + QString("_emergency_%1.sla").arg(dat.toString("dd_MM_yyyy_hh_mm")));
		std::cout << "Saving: " << fileName.toStdString() << std::endl;
		FileLoader fl(fileName);
		fl.saveFile(fileName, doc, 0);
		view->close();
		int numPages=doc->Pages->count();
		for (int j=0; j<numPages; ++j)
			delete doc->Pages->at(j);
		delete doc;
		ActWin->close();
	}
}

void ScribusMainWindow::EditTabs()
{
	if (!HaveDoc || doc->m_Selection->isEmpty())
		return;

	PageItem *currItem = doc->m_Selection->itemAt(0);
	TabManager *dia = new TabManager(this, doc->unitIndex(), currItem->itemText.defaultStyle().tabValues(), currItem->width());
	if (dia->exec())
	{
		ParagraphStyle newTabs(currItem->itemText.defaultStyle());
		newTabs.setTabValues(dia->tmpTab);
		currItem->itemText.setDefaultStyle(newTabs);
		currItem->update();
		slotDocCh();
	}
	delete dia;
}

void ScribusMainWindow::SearchText()
{
	PageItem *currItem = doc->m_Selection->itemAt(0);
	view->requestMode(modeEdit);
	currItem->itemText.setCursorPosition(0);
	SearchReplace* dia = new SearchReplace(this, doc, currItem);
	connect(dia, SIGNAL(NewFont(const QString&)), this, SLOT(SetNewFont(const QString&)));
	connect(dia, SIGNAL(NewAbs(int)), this, SLOT(setAlignmentValue(int)));
	dia->exec();
	dia->disconnect();
	delete dia;
	slotSelect();
}

/* call gimp and wait upon completion */
void ScribusMainWindow::callImageEditor()
{
	if (doc->m_Selection->isEmpty())
		return;

	//NOTE to reviewers: I added my code to this function,
	// - as it performs a similar function,
	// - when the frame is a latex frame it makes only sense
	//   to run a latex editor
	// - IMHO ScribusMainWindow has way to many slots already
	// - my code here is short and without sideeffects
	PageItem *currItem = doc->m_Selection->itemAt(0);
	if (currItem->asLatexFrame())
	{
		currItem->asLatexFrame()->runEditor();
		return; //Don't process the functions for imageframes!
	}
#ifdef HAVE_OSG
	if (currItem->asOSGFrame())
	{
		OSGEditorDialog *dia = new OSGEditorDialog(this, currItem->asOSGFrame(), m_osgFilterString);
		dia->exec();
		return;
	}
#endif
	QString imageEditorExecutable=m_prefsManager->imageEditorExecutable();
	if (currItem->imageIsAvailable)
	{
		bool startFailed=false;
	#ifdef Q_OS_OSX
		QString osxcmd(imageEditorExecutable);
		if (osxcmd.endsWith(".app"))
			osxcmd.prepend("open -a \"");
		else
			osxcmd.prepend("\"");
		osxcmd.append("\" \"");
		osxcmd.append(QDir::toNativeSeparators(currItem->Pfile));
		osxcmd.append("\"");
		if (!QProcess::startDetached(osxcmd))
			startFailed=true;
	#else
		QStringList cmd(QDir::toNativeSeparators(currItem->Pfile));
		if (!QProcess::startDetached(QDir::fromNativeSeparators(imageEditorExecutable), cmd))
			startFailed=true;
	#endif
		if (startFailed)
			ScMessageBox::critical(this, CommonStrings::trWarning, "<qt>" + tr("The program %1 is missing or failed to open").arg(imageEditorExecutable) + "</qt>");
	}
}

void ScribusMainWindow::slotCharSelect()
{
	charPalette->setVisible(scrActions["insertGlyph"]->isChecked());
}

void ScribusMainWindow::setUndoMode(bool isObjectSpecific)
{
	m_objectSpecificUndo = isObjectSpecific;

	if (!m_objectSpecificUndo && HaveDoc)
		m_undoManager->showObject(Um::GLOBAL_UNDO_MODE);
	else if (HaveDoc)
	{
		int docSelectionCount=doc->m_Selection->count();
		if (docSelectionCount == 1)
			m_undoManager->showObject(doc->m_Selection->itemAt(0)->getUId());
		else if (docSelectionCount == 0)
			m_undoManager->showObject(doc->currentPage()->getUId());
		else
			m_undoManager->showObject(Um::NO_UNDO_STACK);
	}
}

bool ScribusMainWindow::isObjectSpecificUndo()
{
	return m_objectSpecificUndo;
}

void ScribusMainWindow::getImageInfo()
{
	if ((!HaveDoc) || (doc->m_Selection->count() != 1))
		return;

	PageItem *pageItem = doc->m_Selection->itemAt(0);
	if (pageItem == NULL)
		return;
	if (pageItem->itemType() == PageItem::ImageFrame)
	{
		ImageInfoDialog *dia = new ImageInfoDialog(this, &pageItem->pixm.imgInfo);
		dia->exec();
		delete dia;
	}
}

void ScribusMainWindow::objectAttributes()
{
	if ((!HaveDoc) || (doc->m_Selection->count() != 1))
		return;

	PageItem *pageItem = doc->m_Selection->itemAt(0);
	if (pageItem == NULL)
		return;
	PageItemAttributes *pageItemAttrs = new PageItemAttributes( this );
	pageItemAttrs->setup(pageItem->getObjectAttributes(), &doc->itemAttributes());
	//CB TODO Probably want this non modal in the future
	if (pageItemAttrs->exec() == QDialog::Accepted)
		pageItem->setObjectAttributes(pageItemAttrs->getNewAttributes());
	delete pageItemAttrs;
}

void ScribusMainWindow::generateTableOfContents()
{
	if (HaveDoc)
		m_tocGenerator->generateDefault();
}

void ScribusMainWindow::updateDocument()
{
	if (!HaveDoc)
		return;
	doc->updateNumbers(true);
	doc->updateMarks(true);
	doc->regionsChanged()->update(QRect());
	emit UpdateRequest(reqNumUpdate);
}

void ScribusMainWindow::insertSampleText()
{
	if (!HaveDoc)
		return;
	LoremManager loremMgr(doc, this);
	if (m_prefsManager->appPrefs.miscPrefs.useStandardLI)
	{
		loremMgr.insertLoremIpsum("la.xml", m_prefsManager->appPrefs.miscPrefs.paragraphsLI);
		return;
	}

	if (loremMgr.exec())
		loremMgr.insertLoremIpsum(loremMgr.getName(), loremMgr.paragraphCount(), loremMgr.randomize());
}

void ScribusMainWindow::languageChange()
{
	if (!ScCore->initialized())
		return;

	//Update colours in case someone has a translated None colour in their preference settings
	//before changing the tr_NoneColor to the new value. See #9267, #5529
	m_prefsManager->languageChange();
	CommonStrings::languageChange();
	LanguageManager::instance()->languageChange();
	qApp->setLayoutDirection(QLocale(ScCore->getGuiLanguage()).textDirection());
	//Update actions
	if (actionManager!=NULL)
	{
		actionManager->languageChange();
		ScCore->pluginManager->languageChange();
		initKeyboardShortcuts();
	}
	//Update menu texts
	if (scrMenuMgr!=NULL && !scrMenuMgr->empty())
		scrMenuMgr->languageChange();
	if (m_undoManager!=NULL)
		m_undoManager->languageChange();
	statusBarLanguageChange();
	viewToolBar->languageChange();
}

void ScribusMainWindow::statusBarLanguageChange()
{
	zoomSpinBox->setToolTip( tr("Current zoom level"));
	zoomDefaultToolbarButton->setToolTip( tr("Zoom to 100%"));
	zoomOutToolbarButton->setToolTip( tr("Zoom out by the stepping value in Tools preferences"));
	zoomInToolbarButton->setToolTip( tr("Zoom in by the stepping value in Tools preferences"));
	layerMenu->setToolTip( tr("Select the current layer"));
	unitSwitcher->setToolTip( tr("Select the current unit"));
	mainWindowXPosLabel->setText( tr("X:"));
	mainWindowYPosLabel->setText( tr("Y:"));
	mainWindowXPosDataLabel->setText("         ");
	mainWindowYPosDataLabel->setText("         ");
	m_mainWindowStatusLabel->setText( tr("Ready"));
}

void ScribusMainWindow::setDefaultPrinter(const QString& name, const QString& file, const QString& command)
{
	PDef.Pname = name;
	PDef.Dname = file;
	PDef.Command = command;
}

void ScribusMainWindow::getDefaultPrinter(QString& name, QString& file, QString& command)
{
	name=PDef.Pname;
	file=PDef.Dname;
	command=PDef.Command;
}

void ScribusMainWindow::closeActiveWindowMasterPageEditor()
{
	if (!HaveDoc)
		return;
	if(!doc->masterPageMode())
		return;
	editMasterPagesEnd();
	qApp->processEvents();
}

void ScribusMainWindow::updateActiveWindowCaption(const QString &newCaption)
{
	if (!HaveDoc)
		return;
	ActWin->setWindowTitle(QDir::toNativeSeparators(newCaption));
}

void ScribusMainWindow::dragEnterEvent ( QDragEnterEvent* e)
{
	bool accepted = false;
	if ( e->mimeData()->hasFormat("text/uri-list"))
	{
		QString fileUrl;
		QList<QUrl> fileUrls = e->mimeData()->urls();
		for( int i = 0; i < fileUrls.count(); ++i )
		{
			fileUrl = fileUrls[i].toLocalFile().toLower();
			if (fileUrl.endsWith(".sla") || fileUrl.endsWith(".sla.gz") || fileUrl.endsWith(".shape") || fileUrl.endsWith(".sce"))
			{
				accepted = true;
				break;
			}
			else
			{
				QUrl url( fileUrls[i] );
				FileLoader *fileLoader = new FileLoader(url.path());
				int testResult = fileLoader->testFile();
				delete fileLoader;
				if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
				{
					accepted = true;
					break;
				}
			}
		}
	}
	else if (e->mimeData()->hasText())
	{
		QString text = e->mimeData()->text();
		if ((text.startsWith("<SCRIBUSELEM")) || (text.startsWith("SCRIBUSELEMUTF8")))
			accepted = true;
	}
	if (accepted)
		e->accept();
}

void ScribusMainWindow::dropEvent ( QDropEvent * e)
{
	bool accepted = false;
	if (e->mimeData()->hasFormat("text/uri-list"))
	{
		QString fileUrl;
		QList<QUrl> fileUrls = e->mimeData()->urls();
		for (int i = 0; i < fileUrls.count(); ++i)
		{
			fileUrl = fileUrls[i].toLocalFile().toLower();
			if (fileUrl.endsWith(".sla") || fileUrl.endsWith(".sla.gz"))
			{
				QUrl url( fileUrls[i] );
				QFileInfo fi(url.toLocalFile());
				if (fi.exists())
				{
					accepted = true;
					loadDoc( fi.absoluteFilePath() );
				}
			}
			else if (fileUrl.endsWith(".sce"))
			{
				QUrl url( fileUrls[i] );
				QFileInfo fi(url.toLocalFile());
				if (fi.exists())
				{
					accepted = true;
					QString data;
					QByteArray cf;
					loadRawText(url.toLocalFile(), cf);
					data = QString::fromUtf8(cf.data());
					double gx, gy, gw, gh;
					ScriXmlDoc ss;
					if (ss.ReadElemHeader(data, false, &gx, &gy, &gw, &gh))
					{
						doFileNew(gw, gh, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
						HaveNewDoc();
						doc->reformPages(true);
						slotElemRead(data, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, false, doc, view);
						slotDocCh(false);
						doc->regionsChanged()->update(QRectF());
					}
				}
			}
			else
			{
				QUrl url( fileUrls[i] );
				FileLoader *fileLoader = new FileLoader(url.toLocalFile());
				int testResult = fileLoader->testFile();
				delete fileLoader;
				if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
				{
					QFileInfo fi(url.toLocalFile());
					if (fi.exists())
					{
						accepted = true;
						loadDoc( fi.absoluteFilePath() );
					}
				}
			}
		}
	}
	else
	{
		if (e->mimeData()->hasText())
		{
			QString text = e->mimeData()->text();
			if ((text.startsWith("<SCRIBUSELEM")) || (text.startsWith("SCRIBUSELEMUTF8")))
			{
				double gx, gy, gw, gh;
				ScriXmlDoc ss;
				if (ss.ReadElemHeader(text, false, &gx, &gy, &gw, &gh))
				{
					doFileNew(gw, gh, 0, 0, 0, 0, 0, 0, false, false, 0, false, 0, 1, "Custom", true);
					HaveNewDoc();
					doc->reformPages(true);
					slotElemRead(text, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, false, doc, view);
					slotDocCh(false);
					doc->regionsChanged()->update(QRectF());
				}
				accepted = true;
			}
		}
	}
	if (accepted)
		e->accept();
}

void ScribusMainWindow::slotEditCopyContents()
{
	PageItem *currItem = NULL;
	if (!HaveDoc || (currItem = doc->m_Selection->itemAt(0)) == NULL)
		return;
	if (currItem->itemType() != PageItem::ImageFrame)
		return;

	PageItem_ImageFrame* imageItem = currItem->asImageFrame();
	if (!imageItem->imageIsAvailable)
		return;
	contentsBuffer.contentsFileName = "";
	contentsBuffer.sourceType = PageItem::ImageFrame;
	contentsBuffer.contentsFileName = imageItem->Pfile;
	contentsBuffer.LocalScX = imageItem->imageXScale();
	contentsBuffer.LocalScY = imageItem->imageYScale();
	contentsBuffer.LocalX   = imageItem->imageXOffset();
	contentsBuffer.LocalY   = imageItem->imageYOffset();
	contentsBuffer.LocalRot = imageItem->imageRotation();
	contentsBuffer.ItemX   = imageItem->xPos();
	contentsBuffer.ItemY   = imageItem->yPos();
	contentsBuffer.effects = imageItem->effectsInUse;
	contentsBuffer.inputProfile = imageItem->IProfile;
	contentsBuffer.useEmbedded  = imageItem->UseEmbedded;
	contentsBuffer.renderingIntent = imageItem->IRender;
}

void ScribusMainWindow::slotEditPasteContents(int absolute)
{
	if (!HaveDoc || contentsBuffer.contentsFileName.isEmpty())
		return;
	PageItem *currItem = NULL;
	if ((currItem = doc->m_Selection->itemAt(0)) == NULL)
		return;
	if (contentsBuffer.sourceType != PageItem::ImageFrame || currItem->itemType() != PageItem::ImageFrame)
		return;

	PageItem_ImageFrame* imageItem=currItem->asImageFrame();
	int t=QMessageBox::Yes;
	if (imageItem->imageIsAvailable)
		t = ScMessageBox::warning(this, CommonStrings::trWarning,
								tr("Do you really want to replace your existing image?"),
								QMessageBox::Yes | QMessageBox::No,
								QMessageBox::No,	// GUI default
								QMessageBox::Yes);	// batch default
	if (t != QMessageBox::Yes)
		return;

	imageItem->EmProfile = "";
	imageItem->pixm.imgInfo.isRequest = false;
	imageItem->IProfile = doc->cmsSettings().DefaultImageRGBProfile;
	imageItem->IRender  = doc->cmsSettings().DefaultIntentImages;
	imageItem->effectsInUse = contentsBuffer.effects;
	qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
	qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	doc->loadPict(contentsBuffer.contentsFileName, imageItem);
	imageItem->setImageXYScale(contentsBuffer.LocalScX, contentsBuffer.LocalScY);
	if (absolute==0)
		imageItem->setImageXYOffset(contentsBuffer.LocalX, contentsBuffer.LocalY);
	else
		imageItem->setImageXYOffset(
		((contentsBuffer.ItemX-imageItem->xPos()) / contentsBuffer.LocalScX)+contentsBuffer.LocalX,
		((contentsBuffer.ItemY-imageItem->yPos()) / contentsBuffer.LocalScY)+contentsBuffer.LocalY);
	imageItem->setImageRotation(contentsBuffer.LocalRot);
	imageItem->IProfile=contentsBuffer.inputProfile;
	imageItem->UseEmbedded=contentsBuffer.useEmbedded;
	imageItem->IRender=contentsBuffer.renderingIntent;
	qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	view->DrawNew();
	propertiesPalette->updateColorList();
	textPalette->updateColorList();
	emit UpdateRequest(reqCmsOptionsUpdate);
	currItem->emitAllToGUI();
	qApp->restoreOverrideCursor();
}

void ScribusMainWindow::slotInsertFrame()
{
	if (!HaveDoc)
		return;

	view->requestMode(modeNormal);
	if (!doc->m_Selection->isEmpty())
		view->Deselect(false);

	InsertAFrame dia(this, doc);
	if (dia.exec())
	{
		InsertAFrameData iafData;
		dia.getNewFrameProperties(iafData);
		doc->itemAddUserFrame(iafData);
	}
}

void ScribusMainWindow::slotItemTransform()
{
	if (!HaveDoc)
		return;
	if (doc->m_Selection->isEmpty())
		return;

	TransformDialog td(this, doc);
	if (td.exec() == 0)
		return;
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = m_undoManager->beginTransaction(Um::Selection,Um::IPolygon,Um::Transform,"",Um::IMove);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	int count=td.getCount();
	QTransform matrix(td.getTransformMatrix());
	int basepoint=td.getBasepoint();
	doc->itemSelection_Transform(count, matrix, basepoint);
	qApp->restoreOverrideCursor();
	if (trans)
	{
		trans.commit();
	}
}

void ScribusMainWindow::PutToInline(QString buffer)
{
	if (!HaveDoc)
		return;
	Selection tempSelection(*doc->m_Selection);
	bool savedAlignGrid = doc->SnapGrid;
	bool savedAlignGuides = doc->SnapGuides;
	bool savedAlignElement = doc->SnapElement;
	int ac = doc->Items->count();
	bool isGroup = false;
	double gx, gy, gh, gw;
	FPoint minSize = doc->minCanvasCoordinate;
	FPoint maxSize = doc->maxCanvasCoordinate;
	doc->SnapGrid  = false;
	doc->SnapGuides = false;
	doc->SnapElement = false;
	bool wasUndo = m_undoManager->undoEnabled();
	m_undoManager->setUndoEnabled(false);
	slotElemRead(buffer, 0, 0, false, true, doc, view);
	doc->SnapGrid  = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	doc->SnapElement = savedAlignElement;
	doc->m_Selection->clear();
	if (doc->Items->count() - ac > 1)
		isGroup = true;
	doc->m_Selection->delaySignalsOn();
	for (int as = ac; as < doc->Items->count(); ++as)
	{
		doc->m_Selection->addItem(doc->Items->at(as));
	}
	if (isGroup)
		doc->GroupCounter++;
	doc->m_Selection->setGroupRect();
	doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
	PageItem* currItem3 = doc->Items->at(ac);
	currItem3->isEmbedded = true;
	currItem3->setIsAnnotation(false);
	currItem3->isBookmark = false;
	currItem3->gXpos = currItem3->xPos() - gx;
	currItem3->gYpos = currItem3->yPos() - gy;
	currItem3->gWidth = gw;
	currItem3->gHeight = gh;
	doc->addToInlineFrames(currItem3);
	int acc = doc->Items->count();
	for (int as = ac; as < acc; ++as)
	{
		doc->Items->takeAt(ac);
	}
	doc->m_Selection->clear();
	doc->m_Selection->delaySignalsOff();
	*doc->m_Selection=tempSelection;
	doc->minCanvasCoordinate = minSize;
	doc->maxCanvasCoordinate = maxSize;
	m_undoManager->setUndoEnabled(wasUndo);
	inlinePalette->unsetDoc();
	inlinePalette->setDoc(doc);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	view->Deselect(false);
}

void ScribusMainWindow::PutToInline()
{
	if (!HaveDoc)
		return;
	Selection tempSelection(*doc->m_Selection);
	bool savedAlignGrid = doc->SnapGrid;
	bool savedAlignGuides = doc->SnapGuides;
	bool savedAlignElement = doc->SnapElement;
	int ac = doc->Items->count();
	bool isGroup = false;
	double gx, gy, gh, gw;
	FPoint minSize = doc->minCanvasCoordinate;
	FPoint maxSize = doc->maxCanvasCoordinate;
	doc->SnapGrid  = false;
	doc->SnapGuides = false;
	doc->SnapElement = false;
	bool wasUndo = m_undoManager->undoEnabled();
	m_undoManager->setUndoEnabled(false);
	internalCopy = true;
	slotEditCopy();
	slotElemRead(internalCopyBuffer, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
	internalCopy = false;
	doc->SnapGrid  = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	doc->SnapElement = savedAlignElement;
	doc->m_Selection->clear();
	if (doc->Items->count() - ac > 1)
		isGroup = true;
	doc->m_Selection->delaySignalsOn();
	for (int as = ac; as < doc->Items->count(); ++as)
	{
		doc->m_Selection->addItem(doc->Items->at(as));
	}
	if (isGroup)
		doc->GroupCounter++;
	doc->m_Selection->setGroupRect();
	doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
	PageItem* currItem3 = doc->Items->at(ac);
	currItem3->isEmbedded = true;
	currItem3->setIsAnnotation(false);
	currItem3->isBookmark = false;
	currItem3->gXpos = currItem3->xPos() - gx;
	currItem3->gYpos = currItem3->yPos() - gy;
	currItem3->gWidth = gw;
	currItem3->gHeight = gh;
	doc->addToInlineFrames(currItem3);
	int acc = doc->Items->count();
	for (int as = ac; as < acc; ++as)
	{
		doc->Items->takeAt(ac);
	}
	doc->m_Selection->clear();
	doc->m_Selection->delaySignalsOff();
	*doc->m_Selection=tempSelection;
	doc->minCanvasCoordinate = minSize;
	doc->maxCanvasCoordinate = maxSize;
	m_undoManager->setUndoEnabled(wasUndo);
	inlinePalette->unsetDoc();
	inlinePalette->setDoc(doc);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	view->Deselect(false);
}

void ScribusMainWindow::PutToPatterns()
{
	if (!HaveDoc)
		return;
	QString patternName = "Pattern_"+doc->m_Selection->itemAt(0)->itemName();
	patternName = patternName.trimmed().simplified().replace(" ", "_");
	bool savedAlignGrid = doc->SnapGrid;
	bool savedAlignGuides = doc->SnapGuides;
	bool savedAlignElement = doc->SnapElement;
	int ac = doc->Items->count();
	FPoint minSize = doc->minCanvasCoordinate;
	FPoint maxSize = doc->maxCanvasCoordinate;
	doc->SnapGrid  = false;
	doc->SnapGuides = false;
	doc->SnapElement = false;
	m_undoManager->setUndoEnabled(false);
	internalCopy = true;
	slotEditCopy();
	slotElemRead(internalCopyBuffer, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
	internalCopy = false;
	doc->SnapGrid  = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	doc->SnapElement = savedAlignElement;
	doc->m_Selection->clear();
	view->Deselect(true);
	PageItem* currItem;
	doc->m_Selection->delaySignalsOn();
	for (int as = ac; as < doc->Items->count(); ++as)
	{
		doc->m_Selection->addItem(doc->Items->at(as));
	}
	if (doc->Items->count() - ac > 1)
		currItem = doc->groupObjectsSelection(doc->m_Selection);
	else
		currItem = doc->m_Selection->itemAt(0);
	QList<PageItem*> allItems;
	if (currItem->isGroup())
		allItems = currItem->getAllChildren();
	else
		allItems.append(currItem);
	QStringList results;
	for (int ii = 0; ii < allItems.count(); ii++)
	{
		PageItem *item = allItems.at(ii);
		if ((!results.contains(item->pattern())) && ((item->GrType == 8) || (item->itemType() == PageItem::Symbol)))
			results.append(item->pattern());
		if (!item->strokePattern().isEmpty())
		{
			if (!results.contains(item->strokePattern()))
				results.append(item->strokePattern());
		}
		if (!item->patternMask().isEmpty())
		{
			if (!results.contains(item->patternMask()))
				results.append(item->patternMask());
		}
	}
	patternsDependingOnThis.clear();
	QStringList mainPatterns = doc->docPatterns.keys();
	for (int i = 0; i < results.count(); i++)
	{
		QString temp(results[i]);
		for (int j = 0; j < mainPatterns.count(); j++)
		{
			if (mainPatterns[j] != temp)
			{
				QStringList subPatterns;
				subPatterns = doc->getUsedPatternsHelper(mainPatterns[j], subPatterns);
				if (subPatterns.contains(temp))
					patternsDependingOnThis.prepend(mainPatterns[j]);
			}
		}
		patternsDependingOnThis.prepend(temp);
	}
	allItems.clear();
	Query dia(this, "tt", 1, tr("&Name:"), tr("New Entry"));
	dia.setEditText(patternName, true);
	dia.setForbiddenList(patternsDependingOnThis);
	dia.setTestList(doc->docPatterns.keys());
	dia.setCheckMode(true);
	if (dia.exec())
		patternName = dia.getEditText();
	else
	{
		doc->m_Selection->clear();
		doc->m_Selection->delaySignalsOff();
		doc->Items->removeAll(currItem);
		delete currItem;
		doc->minCanvasCoordinate = minSize;
		doc->maxCanvasCoordinate = maxSize;
		if (outlinePalette->isVisible())
			outlinePalette->BuildTree();
		m_undoManager->setUndoEnabled(true);
		return;
	}
	ScPattern pat = ScPattern();
	pat.setDoc(doc);
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	double x1, x2, y1, y2;
	currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
	minx = qMin(minx, x1);
	miny = qMin(miny, y1);
	maxx = qMax(maxx, x2);
	maxy = qMax(maxy, y2);
	pat.pattern = currItem->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
	pat.width = maxx - minx;
	pat.height = maxy - miny;
	pat.items.append(currItem);
	// #11274 : OwnPage is not meaningful for pattern items
	// We set consequently pattern item's OwnPage to -1
	QList<PageItem*> patternItems = pat.items;
	while (patternItems.count() > 0)
	{
		PageItem* patItem = patternItems.takeAt(0);
		if (patItem->isGroup())
			patternItems += patItem->groupItemList;
		patItem->OwnPage = -1;
	}
	if (doc->docPatterns.contains(patternName))
		doc->docPatterns.remove(patternName);
	currItem->gXpos = currItem->xPos() - minx;
	currItem->gYpos = currItem->yPos() - miny;
	currItem->setXYPos(currItem->gXpos, currItem->gYpos, true);
	doc->addPattern(patternName, pat);
	doc->Items->removeAll(currItem);
	doc->m_Selection->clear();
	doc->m_Selection->delaySignalsOff();
	propertiesPalette->updateColorList();
	textPalette->updateColorList();
	symbolPalette->updateSymbolList();
	emit UpdateRequest(reqColorsUpdate);
	doc->minCanvasCoordinate = minSize;
	doc->maxCanvasCoordinate = maxSize;
	view->DrawNew();
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	m_undoManager->setUndoEnabled(true);
}

void ScribusMainWindow::ConvertToSymbol()
{
	if (!HaveDoc)
		return;
	if (doc->m_Selection->isEmpty())
		return;
	QString patternName("Pattern_"+doc->m_Selection->itemAt(0)->itemName());
	patternName = patternName.trimmed().simplified().replace(" ", "_");
	Query dia(this, "tt", 1, tr("&Name:"), tr("New Entry"));
	dia.setEditText(patternName, true);
	patternsDependingOnThis.clear();
	dia.setForbiddenList(patternsDependingOnThis);
	dia.setTestList(doc->docPatterns.keys());
	dia.setCheckMode(true);
	if (!dia.exec())
		return;
	patternName = dia.getEditText();
	m_undoManager->setUndoEnabled(false);
	doc->itemSelection_convertItemsToSymbol(patternName);
	propertiesPalette->updateColorList();
	textPalette->updateColorList();
	symbolPalette->updateSymbolList();
	emit UpdateRequest(reqColorsUpdate);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	view->DrawNew();
	m_undoManager->setUndoEnabled(true);
}

void ScribusMainWindow::manageColorsAndFills()
{
	ColorList edc;
	QHash<QString, VGradient> *Gradients;
	QHash<QString, ScPattern> *docPatterns;
	ScribusDoc* tmpDoc;
	if (HaveDoc)
	{
		Gradients = &doc->docGradients;
		edc = doc->PageColors;
		docPatterns = &doc->docPatterns;
		tmpDoc = doc;
	}
	else
	{
		Gradients = &m_prefsManager->appPrefs.defaultGradients;
		edc = m_prefsManager->colorSet();
		docPatterns = &m_prefsManager->appPrefs.defaultPatterns;
		tmpDoc = m_doc;
		doc = m_doc;
	}
	m_undoManager->setUndoEnabled(false);
	ColorsAndFillsDialog *dia = new ColorsAndFillsDialog(this, Gradients, edc, m_prefsManager->colorSetName(), docPatterns, tmpDoc, this);
	if (dia->exec())
	{
		if (HaveDoc)
		{
			slotDocCh();
			doc->PageColors = dia->m_colorList;
			if (dia->replaceColorMap.isEmpty())
			{
				// invalidate all charstyles, as replaceNamedResources() won't do it if all maps are empty
				const StyleSet<CharStyle> dummy;
				doc->redefineCharStyles(dummy, false);
			}
			else
			{
				ResourceCollection colorrsc;
				colorrsc.mapColors(dia->replaceColorMap);
				// Update tools colors
				PrefsManager::replaceToolColors(doc->itemToolPrefs(), colorrsc.colors());
				// Update objects and styles colors
				doc->replaceNamedResources(colorrsc);
				// Temporary code until LineStyle is effectively used
				doc->replaceLineStyleColors(dia->replaceColorMap);
			}
			doc->recalculateColors();
			doc->recalcPicturesRes();
			doc->setGradients(dia->dialogGradients);
			if (!dia->replaceMap.isEmpty())
			{
				ResourceCollection gradrsc;
				gradrsc.mapPatterns(dia->replaceMap);
				doc->replaceNamedResources(gradrsc);
			}
			doc->setPatterns(dia->dialogPatterns);
			if (!dia->replaceMapPatterns.isEmpty())
			{
				ResourceCollection colorrsc;
				colorrsc.mapPatterns(dia->replaceMapPatterns);
				doc->replaceNamedResources(colorrsc);
			}
			symbolPalette->updateSymbolList();
			updateColorLists();
			if (!doc->m_Selection->isEmpty())
				doc->m_Selection->itemAt(0)->emitAllToGUI();
			view->DrawNew();
		}
		else
		{
			// Update tools colors if needed
			m_prefsManager->replaceToolColors(dia->replaceColorMap);
			m_prefsManager->setColorSet(dia->m_colorList);
			propertiesPalette->Cpal->setColors(m_prefsManager->colorSet());
			m_prefsManager->appPrefs.defaultGradients = dia->dialogGradients;
			m_prefsManager->appPrefs.defaultPatterns = dia->dialogPatterns;
			QString Cpfad = QDir::toNativeSeparators(ScPaths::applicationDataDir())+"DefaultColors.xml";
			const FileFormat *fmt = LoadSavePlugin::getFormatById(FORMATID_SLA150EXPORT);
			if (fmt)
			{
				ScribusDoc *s_doc = new ScribusDoc();
				s_doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
				s_doc->setPage(100, 100, 0, 0, 0, 0, 0, 0, false, false);
				s_doc->addPage(0);
				s_doc->setGUI(false, this, 0);
				s_doc->PageColors = dia->m_colorList;
				s_doc->setGradients(dia->dialogGradients);
				s_doc->setPatterns(dia->dialogPatterns);
				fmt->setupTargets(s_doc, 0, this, mainWindowProgressBar, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
				fmt->savePalette(Cpfad);
				delete s_doc;
			}
			m_prefsManager->setColorSetName(dia->getColorSetName());
			doc = NULL;
		}
	}
	if (!HaveDoc)
		doc = NULL;
	delete dia;
	m_undoManager->setUndoEnabled(true);
}

void ScribusMainWindow::slotReplaceColors()
{
	if (!HaveDoc)
		return;

	ColorList UsedC;
	doc->getUsedColors(UsedC);
	QScopedPointer<replaceColorsDialog> dia2(new replaceColorsDialog(this, doc->PageColors, UsedC));
	if (!dia2->exec())
		return;

	ResourceCollection colorrsc;
	colorrsc.mapColors(dia2->replaceMap);
	PrefsManager::replaceToolColors(doc->itemToolPrefs(), colorrsc.colors());
	doc->replaceNamedResources(colorrsc);
	doc->replaceLineStyleColors(dia2->replaceMap);
	doc->recalculateColors();
	doc->recalcPicturesRes();
	requestUpdate(reqColorsUpdate | reqLineStylesUpdate);
	m_styleManager->updateColorList();
	if (!doc->m_Selection->isEmpty())
		doc->m_Selection->itemAt(0)->emitAllToGUI();
	view->DrawNew();
}

void ScribusMainWindow::updateGUIAfterPagesChanged()
{
	view->DrawNew();
	pagePalette->rebuildPages();
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
}

void ScribusMainWindow::updateTableMenuActions()
{
	appModeHelper->updateTableMenuActions(doc);
}

void ScribusMainWindow::insertMark(MarkType mType)
{
	if (!HaveDoc)
		return;
	if (doc->m_Selection->count() != 1)
		return;
	if  (doc->appMode != modeEdit)
		return;

	UndoTransaction trans;
	PageItem* currItem = doc->m_Selection->itemAt(0);
	if (!currItem->isTextFrame())
		return;
	if (currItem->HasSel)
	{
		if (UndoManager::instance()->undoEnabled())
			trans = m_undoManager->beginTransaction(Um::Selection,Um::IDelete,Um::Delete,"",Um::IDelete);
		//inserting mark replace some selected text
		currItem->asTextFrame()->deleteSelectedTextFromFrame();
	}
	ScItemsState* is = NULL;
	if (insertMarkDialog(currItem->asTextFrame(), mType, is))
	{
		Mark* mrk = currItem->itemText.mark(currItem->itemText.cursorPosition() -1);
		view->updatesOn(false);
		currItem->invalidateLayout();
		currItem->layout();
		if (mType == MARKNoteMasterType)
		{
			doc->setNotesChanged(true);
			if (mrk->getNotePtr()->isEndNote())
				doc->flag_updateEndNotes = true;
			doc->setCursor2MarkPos(mrk->getNotePtr()->noteMark());
			nsEditor->setNotesStyle(mrk->getNotePtr()->notesStyle());
		}
		doc->changed();
		if (is != NULL)
			is->set("label", mrk->label);
		view->updatesOn(true);
		view->DrawNew();
	}
	if (trans)
		trans.commit();
}

void ScribusMainWindow::slotEditMark()
{
	if (!HaveDoc)
		return;
	if (doc->m_Selection->count() != 1)
		return;
	if  (doc->appMode != modeEdit)
		return;
	PageItem * currItem = doc->m_Selection->itemAt(0);
	if (currItem->itemText.cursorPosition() < currItem->itemText.length())
	{
		if (currItem->itemText.hasMark(currItem->itemText.cursorPosition()))
		{
			Mark* mark = currItem->itemText.mark(currItem->itemText.cursorPosition());
			if (editMarkDlg(mark, currItem->asTextFrame()))
			{
				if (mark->isType(MARKVariableTextType))
					doc->flag_updateMarksLabels = true;
				else
					currItem->invalid = true;
				//doc->updateMarks();
				doc->changed();
				doc->regionsChanged()->update(QRectF());
				view->DrawNew();
			}
			if (mark->isNoteType())
				nsEditor->setNotesStyle(mark->getNotePtr()->notesStyle());
		}
	}
}

void ScribusMainWindow::slotUpdateMarks()
{
	if (!HaveDoc)
		return;
	if (doc->marksList().isEmpty())
		return;
	if (doc->updateMarks(true))
	{
		doc->changed();
		doc->regionsChanged()->update(QRectF());
	}
}

void ScribusMainWindow::slotInsertMarkNote()
{
	if (!HaveDoc)
		return;
	if (doc->m_docNotesStylesList.count() == 1)
	{ //fast insert note with the only default notes style avaiable
		PageItem* currItem = doc->m_Selection->itemAt(0);
		Q_ASSERT(currItem->isTextFrame() && !currItem->isNoteFrame());
		UndoTransaction trans;
		if (currItem->HasSel)
		{
			if (UndoManager::instance()->undoEnabled())
				trans = m_undoManager->beginTransaction(Um::Selection,Um::IDelete,Um::Delete,"",Um::IDelete);
			//inserting mark replace some selected text
			currItem->asTextFrame()->deleteSelectedTextFromFrame();
		}
		NotesStyle* nStyle = doc->m_docNotesStylesList.at(0);
		QString label = "NoteMark_" + nStyle->name();
		if (nStyle->range() == NSRsection)
			label += " in section " + doc->getSectionNameForPageIndex(currItem->OwnPage) + " page " + QString::number(currItem->OwnPage +1);
		else if (nStyle->range() == NSRpage)
			label += " on page " + QString::number(currItem->OwnPage +1);
		else if (nStyle->range() == NSRstory)
			label += " in " + currItem->firstInChain()->itemName();
		else if (nStyle->range() == NSRframe)
			label += " in frame" + currItem->itemName();
		if (doc->getMark(label + "_1", MARKNoteMasterType) != NULL)
			getUniqueName(label,doc->marksLabelsList(MARKNoteMasterType), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
		else
			label = label + "_1";
		Mark* mrk = doc->newMark();
		mrk->label = label;
		mrk->setType(MARKNoteMasterType);
		mrk->setNotePtr(doc->newNote(nStyle));
		mrk->getNotePtr()->setMasterMark(mrk);
		mrk->setString("");
		mrk->OwnPage = currItem->OwnPage;
		currItem->itemText.insertMark(mrk);
		currItem->invalidateLayout();
		currItem->layout();
		if (mrk->getNotePtr()->isEndNote())
			doc->flag_updateEndNotes = true;
		doc->regionsChanged()->update(QRectF());
		doc->changed();
		doc->setCursor2MarkPos(mrk->getNotePtr()->noteMark());
		if (UndoManager::undoEnabled())
		{
			ScItemsState* is = new ScItemsState(UndoManager::InsertNote);
			is->set("ETEA", mrk->label);
			is->set("MARK", QString("new"));
			is->set("label", mrk->label);
			is->set("type", (int) MARKNoteMasterType);
			is->set("strtxt", QString(""));
			is->set("nStyle", nStyle->name());
			is->set("at", currItem->itemText.cursorPosition() -1);
			is->insertItem("inItem", currItem);
			m_undoManager->action(doc, is);
		}
		if (trans)
			trans.commit();
	}
	else
		insertMark(MARKNoteMasterType);
}

bool ScribusMainWindow::insertMarkDialog(PageItem_TextFrame* currItem, MarkType mrkType, ScItemsState* &is)
{
	if (doc->masterPageMode() && (mrkType != MARKVariableTextType))
		//avoid inserting in master pages other marks than Variable Text
		return false;
	
	MarkInsert* insertMDialog = NULL;
	switch (mrkType)
	{
	case MARKAnchorType:
		insertMDialog = (MarkInsert*) new MarkAnchor(this);
		break;
	case MARKVariableTextType:
		insertMDialog = (MarkInsert*) new MarkVariableText(doc->marksList(), this);
		break;
	case MARK2ItemType:
		insertMDialog = (MarkInsert*) new Mark2Item(this);
		break;
	case MARK2MarkType:
		insertMDialog = (MarkInsert*) new Mark2Mark(doc->marksList(), NULL, this);
		break;
	case MARKNoteMasterType:
		insertMDialog = (MarkInsert*) new MarkNote(doc->m_docNotesStylesList, this);
		break;
	case MARKIndexType:
		break;
	default:
		break;
	}
	if (insertMDialog == NULL)
	{
		qDebug() << "Dialog not implemented for such marks type " << mrkType;
		return false;
	}
	bool docWasChanged = false;
	
	insertMDialog->setWindowTitle(tr("Insert new ") + insertMDialog->windowTitle());
	if (insertMDialog->exec())
	{
		Mark* mrk = NULL;
		Mark oldMark;
		MarkData markdata;
		if (currItem != NULL)
			markdata.itemName = currItem->itemName();
		QString label = "", text = "";
		NotesStyle* NStyle = NULL;
		bool insertExistedMark = false;
		switch (mrkType)
		{
		case MARKAnchorType:
			//only gets label for new mark
			insertMDialog->values(label);
			if (label.isEmpty())
				label = tr("Anchor mark");
			markdata.itemPtr = currItem;
			break;
		case MARKVariableTextType:
			mrk = insertMDialog->values(label, text);
			if ((mrk == NULL) && (text.isEmpty()))
				return false; //FIX ME here user should be warned that inserting of mark fails and why
			if (label.isEmpty())
				label = tr("Mark with <%1> variable text").arg(text);
			markdata.strtxt = text;
			break;
		case MARK2ItemType:
			insertMDialog->values(label, markdata.itemPtr);
			if (markdata.itemPtr == NULL)
				return false; //FIX ME here user should be warned that inserting of mark fails and why
			if (label.isEmpty())
				label = tr("Mark to %1 item").arg(markdata.itemPtr->itemName());
			markdata.strtxt = QString::number(markdata.itemPtr->OwnPage +1);
			break;
		case MARK2MarkType:
			//gets pointer to referenced mark
			Mark* mrkPtr;
			insertMDialog->values(label, mrkPtr);
			if (mrkPtr == NULL)
				return false; //FIX ME here user should be warned that inserting of mark fails and why
			if (label.isEmpty())
				label = tr("Mark to %1 mark").arg(mrkPtr->label);
			markdata.strtxt = QString::number(mrkPtr->OwnPage +1);
			markdata.destmarkName = mrkPtr->label;
			markdata.destmarkType = mrkPtr->getType();
			break;
		case MARKNoteMasterType:
			//gets pointer to chosen notes style
			NStyle = insertMDialog->values();
			if (NStyle == NULL)
				return false;

			markdata.notePtr = doc->newNote(NStyle);
			label = "NoteMark_" + NStyle->name();
			if (NStyle->range() == NSRsection)
				label += " in section " + doc->getSectionNameForPageIndex(currItem->OwnPage) + " page " + QString::number(currItem->OwnPage +1);
			else if (NStyle->range() == NSRpage)
				label += " on page " + QString::number(currItem->OwnPage +1);
			else if (NStyle->range() == NSRstory)
				label += " in " + currItem->firstInChain()->itemName();
			else if (NStyle->range() == NSRframe)
				label += " in frame" + currItem->itemName();
			break;
		case MARKIndexType:
				return false;
			break;
		default:
				return false;
			break;
		}
		if (mrk == NULL)
		{
			//check if label for new mark can be used as is
			if (mrkType == MARKNoteMasterType)
			{
				if (doc->getMark(label + "_1", mrkType) != NULL)
					getUniqueName(label,doc->marksLabelsList(mrkType), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
				else
					label = label + "_1";
			}
			else
				getUniqueName(label,doc->marksLabelsList(mrkType), "_");
			mrk = doc->newMark();
			mrk->setValues(label, currItem->OwnPage, mrkType, markdata);
		}
		else
		{ // that must be variable text mark
			oldMark = *mrk;
			mrk->setString(markdata.strtxt);
			mrk->label = label;
			insertExistedMark = true;
			doc->flag_updateMarksLabels = true;
		}

		currItem->itemText.insertMark(mrk);
		mrk->OwnPage = currItem->OwnPage;


		if (mrkType == MARKNoteMasterType)
		{
			mrk->getNotePtr()->setMasterMark(mrk);
			mrk->setString("");
		}

		if (UndoManager::undoEnabled())
		{
			if (mrk->isType(MARKNoteMasterType))
				is = new ScItemsState(UndoManager::InsertNote);
			else if (insertExistedMark && ((oldMark.label != mrk->label) || (oldMark.getString() != mrk->getString())))
				is = new ScItemsState(UndoManager::EditMark);
			else
				is = new ScItemsState(UndoManager::InsertMark);
			is->set("ETEA", mrk->label);
			is->set("label", mrk->label);
			is->set("type", (int) mrk->getType());
			if (insertExistedMark)
			{
				is->set("MARK", QString("insert_existing"));
				if (mrk->label != oldMark.label)
				{
					is->set("labelOLD", oldMark.label);
					is->set("labelNEW", mrk->label);
					doc->flag_updateMarksLabels = true;
				}
				if (oldMark.getString() != mrk->getString())
				{
					is->set("strOLD", oldMark.getString());
					is->set("strNEW", mrk->getString());
				}
			}
			else
			{
				is->set("MARK", QString("new"));
				is->set("strtxt", mrk->getString());
				if (mrk->isType(MARK2MarkType))
				{
					QString dName;
					MarkType dType;
					mrk->getMark(dName, dType);
					is->set("dName", dName);
					is->set("dType", (int) dType);
				}
				if (mrk->isType(MARK2ItemType))
					is->insertItem("itemPtr", mrk->getItemPtr());
				if (mrk->isType(MARKNoteMasterType))
					is->set("nStyle", mrk->getNotePtr()->notesStyle()->name());
			}
			is->set("at", currItem->itemText.cursorPosition() -1);
			if (currItem->isNoteFrame())
				is->set("noteframeName", currItem->getUName());
			else
				is->insertItem("inItem", currItem);
			m_undoManager->action(doc, is);
			docWasChanged = true;
		}
	}
	delete insertMDialog;
	return docWasChanged;
}

bool ScribusMainWindow::editMarkDlg(Mark *mrk, PageItem_TextFrame* currItem)
{
	MarkInsert* editMDialog = NULL;
	switch (mrk->getType())
	{
		case MARKAnchorType:
			editMDialog = (MarkInsert*) new MarkAnchor(this);
			editMDialog->setValues(mrk->label);
			break;
		case MARKVariableTextType:
			if (currItem == NULL)
				//invoked from Marks Manager
				editMDialog = dynamic_cast<MarkInsert*>(new MarkVariableText(mrk, this));
			else
				//invoked from mark`s entry in text
				editMDialog = dynamic_cast<MarkInsert*>(new MarkVariableText(doc->marksList(), this));
			editMDialog->setValues(mrk->label, mrk->getString());
			break;
		case MARK2ItemType:
			editMDialog = (MarkInsert*) new Mark2Item(this);
			editMDialog->setValues(mrk->label, mrk->getItemPtr());
			break;
		case MARK2MarkType:
			{
				editMDialog = (MarkInsert*) new Mark2Mark(doc->marksList(), mrk, this);
				QString l;
				MarkType t;
				mrk->getMark(l,t);
				Mark* m = doc->getMark(l,t);
				editMDialog->setValues(mrk->label, m);
			}
			break;
		case MARKNoteMasterType:
			{
				//invoking editing note mark from master text
				//so we go to edit note
				TextNote* note = mrk->getNotePtr();
				if (note == NULL)
				{
					qFatal("ScribusMainWindow::editMarkDlg - found note master mark with null pointer to note");
					return false;
				}
				Mark* noteMark = note->noteMark();
				doc->setCursor2MarkPos(noteMark);
			}
			break;
		case MARKNoteFrameType:
			{
				//invoking editing mark from note frame
				//so we go to master text
				TextNote* note = mrk->getNotePtr();
				if (note == NULL)
				{
					qFatal("ScribusMainWindow::editMarkDlg - found note frame mark with null pointer to note");
					return false;
				}
				Mark* masterMark = note->masterMark();
				doc->setCursor2MarkPos(masterMark);
			}
			break;
		case MARKIndexType:
			return false;
			break;
		default:
			break;
	}
	if (editMDialog == NULL) return false;

	bool docWasChanged = false;

	editMDialog->setWindowTitle(tr("Edit %1").arg(editMDialog->windowTitle()));
	if (editMDialog->exec())
	{
		Mark oldMark = *mrk;
		Mark* Mrk = NULL;
		MarkData markdata;
		if (currItem != NULL)
			markdata.itemName = currItem->itemName();
		QString label, text;
		QString oldStr = mrk->getString();
		bool newMark = false;
		bool replaceMark = false;
		switch (mrk->getType())
		{
			case MARKAnchorType:
				//only gets label for new mark
				editMDialog->values(label);
				if (label.isEmpty())
					label = tr("Anchor mark");
				if (mrk->label != label)
				{
					getUniqueName(label,doc->marksLabelsList(mrk->getType()), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
					mrk->label = label;
					emit UpdateRequest(reqMarksUpdate);
				}
				break;
			case MARKVariableTextType:
				Mrk = editMDialog->values(label, text);
				if (text.isEmpty())
					return false; //FIX ME here user should be warned that editing of mark fails and why
				if (label.isEmpty())
					label = tr("Mark with <%1> variable text").arg(text);
				if (Mrk != NULL)
				{
					if (Mrk != mrk)
					{
						currItem->itemText.replaceMark(currItem->itemText.cursorPosition(), Mrk);
						mrk = Mrk;
						oldMark = *mrk;
						replaceMark = true;
					}
					if (mrk->label != label)
					{
						getUniqueName(label,doc->marksLabelsList(mrk->getType()), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
						mrk->label = label;
						emit UpdateRequest(reqMarksUpdate);
					}
					if (text != oldStr)
					{
						mrk->setString(text);
						docWasChanged = true;
					}
				}
				else
				{
					markdata.strtxt = text;
					mrk = doc->newMark();
					getUniqueName(label,doc->marksLabelsList(mrk->getType()), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
					mrk->setValues(label, currItem->OwnPage, MARKVariableTextType, markdata);
					currItem->itemText.replaceMark(currItem->itemText.cursorPosition(), mrk);
					docWasChanged = true;
					newMark = true;
				}
				break;
			case MARK2ItemType:
				editMDialog->values(label, markdata.itemPtr);
				if (markdata.itemPtr == NULL)
					return false; //FIX ME here user should be warned that inserting of mark fails and why
				if (label.isEmpty())
					label = tr("Mark to %1 item").arg(markdata.itemPtr->itemName());
				if (markdata.itemPtr != mrk->getItemPtr())
				{
					mrk->setItemPtr(markdata.itemPtr);
					mrk->setString(doc->getSectionPageNumberForPageIndex(markdata.itemPtr->OwnPage));
					docWasChanged = true;
				}
				if (mrk->label != label)
				{
					getUniqueName(label,doc->marksLabelsList(mrk->getType()), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
					mrk->label = label;
				}
				break;
			case MARK2MarkType:
				{
					//gets pointer to referenced mark
					Mark* mrkPtr = NULL;
					editMDialog->values(label, mrkPtr);
					if (mrkPtr == NULL)
						return false; //FIX ME here user should be warned that inserting of mark fails and why
					if (label.isEmpty())
						label = tr("Mark to %1 mark").arg(mrkPtr->label);
					QString destLabel = mrkPtr->label;
					MarkType destType = mrkPtr->getType();
					if (markdata.destmarkName != destLabel || markdata.destmarkType != destType)
					{
						mrk->setMark(mrkPtr);
						mrk->setString(doc->getSectionPageNumberForPageIndex(mrkPtr->OwnPage));
						docWasChanged = true;
					}
					if (mrk->label != label)
					{
						getUniqueName(label,doc->marksLabelsList(mrk->getType()), "_"); //FIX ME here user should be warned that inserted mark`s label was changed
						mrk->label = label;
					}
				}
				break;
			case MARKNoteMasterType:
				break;
			case MARKIndexType:
				break;
			default:
				break;
		}
		if (UndoManager::undoEnabled())
		{
			ScItemsState* is = NULL;
			if (newMark || replaceMark)
				is = new ScItemsState(UndoManager::InsertMark);
			else
				is = new ScItemsState(UndoManager::EditMark);
			is->set("ETEA", mrk->label);
			if (currItem != NULL)
			{
				is->set("at", currItem->itemText.cursorPosition()-1);
				if (currItem->isNoteFrame())
					is->set("noteframeName", currItem->getUName());
				else
					is->insertItem("inItem", currItem);
			}
			is->set("label", mrk->label);
			is->set("type", (int) mrk->getType());
			is->set("strtxt", mrk->getString());
			if (newMark)
			{
				is->set("MARK", QString("new"));
				if (mrk->isType(MARK2MarkType))
				{
					QString dName;
					MarkType dType;
					mrk->getMark(dName, dType);
					is->set("dName", dName);
					is->set("dType", (int) dType);
				}
				if (mrk->isType(MARK2ItemType))
					is->insertItem("itemPtr", mrk->getItemPtr());
				if (mrk->isType(MARKNoteMasterType))
					is->set("nStyle", mrk->getNotePtr()->notesStyle()->name());
				doc->flag_updateMarksLabels = true;
			}
			else
			{
				if (replaceMark)
					is->set("MARK", QString("replace"));
				else
					is->set("MARK", QString("edit"));
				if (mrk->label != oldMark.label)
				{
					is->set("labelOLD", oldMark.label);
					is->set("labelNEW", mrk->label);
					doc->flag_updateMarksLabels = true;
				}
				if (mrk->getString() != oldMark.getString())
				{
					is->set("strtxtOLD", oldMark.getString());
					is->set("strtxtNEW", mrk->getString());
				}
				if (mrk->isType(MARK2MarkType))
				{
					QString dNameOLD;
					MarkType dTypeOLD;
					oldMark.getMark(dNameOLD, dTypeOLD);
					QString dName;
					MarkType dType;
					mrk->getMark(dName, dType);
					if (dName != dNameOLD || dType != dTypeOLD)
					{
						is->set("dNameOLD", dNameOLD);
						is->set("dTypeOLD", (int) dTypeOLD);
						is->set("dNameNEW", dName);
						is->set("dTypeNEW", (int) dType);
					}
				}
				if (mrk->isType(MARK2ItemType) && mrk->getItemPtr() != oldMark.getItemPtr())
				{
					is->insertItem("itemPtrOLD", oldMark.getItemPtr());
					is->insertItem("itemPtrNEW", mrk->getItemPtr());
				}
			}
			m_undoManager->action(doc, is);
		}
	}
	delete editMDialog;
	return docWasChanged;
}

void ScribusMainWindow::setPreviewToolbar()
{
	modeToolBar->setEnabled(!doc->drawAsPreview);
	editToolBar->setEnabled(!doc->drawAsPreview);
	pdfToolBar->setEnabled(!doc->drawAsPreview);
	symbolPalette->setEnabled(!doc->drawAsPreview);
	inlinePalette->setEnabled(!doc->drawAsPreview);
	undoPalette->setEnabled(!doc->drawAsPreview);
	outlinePalette->setEnabled(!(doc->drawAsPreview && !doc->editOnPreview));
	propertiesPalette->setEnabled(!(doc->drawAsPreview && !doc->editOnPreview));
	textPalette->setEnabled(!(doc->drawAsPreview && !doc->editOnPreview));
	scrMenuMgr->setMenuEnabled("Edit", !doc->drawAsPreview);
	scrMenuMgr->setMenuEnabled("Item", !doc->drawAsPreview);
	scrMenuMgr->setMenuEnabled("Insert", !doc->drawAsPreview);
	scrMenuMgr->setMenuEnabled("Page", !doc->drawAsPreview);
	scrMenuMgr->setMenuEnabled("Extras", !doc->drawAsPreview);
	HaveNewSel();
}

/*
void ScribusMainWindow::testQT_slot1(QString s)
{
	qDebug()<<"Signal data:"<<s;
}

void ScribusMainWindow::testQT_slot2(double d)
{
	qDebug()<<"Signal data:"<<d;
}

void ScribusMainWindow::testQT_slot3(int i)
{
	qDebug()<<"Signal data:"<<i;
}

void ScribusMainWindow::testQT_slot4()
{
	qDebug()<<"Signal data empty but received";
	QObject *rootObject = dynamic_cast<QObject*>(m_qqview->rootObject());
	QObject *q_xSpinBox = rootObject->findChild<QObject*>("xSpinBox");
	//if (q_xSpinBox==sender())
	{
		qDebug()<<"qov"<<q_xSpinBox->property("value").toDouble();
	}
	m_qqview->close();
	m_qqview->deleteLater();
}*/

void ScribusMainWindow::changePreviewQuality(int index)
{
	if (!HaveDoc)
		return;
	doc->allItems_ChangePreviewResolution(index);
	doc->view()->DrawNew();
}
