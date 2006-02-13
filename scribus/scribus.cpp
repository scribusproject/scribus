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

#include <qaccel.h>
#include <qapplication.h>
#include <qeventloop.h>
#include <qcolordialog.h>
#include <qcolor.h>
#include <qiconset.h>
#include <qtextstream.h>
#include <qstylefactory.h>
#include <qregexp.h>
#include <qtextcodec.h>
#include <qcursor.h>
#include <qvbox.h>
#include <qpixmap.h>
#include <qkeysequence.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "scconfig.h"

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <iostream>
#include <signal.h>
#include <string>

#include "sccombobox.h"
#include "scribusapp.h"
#include "scribus.h"
#include "scribus.moc"
#include "newfile.h"
#include "page.h"
#include "query.h"
#include "mdup.h"
#include "docinfo.h"
#include "reformdoc.h"
#include "serializer.h"
#include "aligndistribute.h"
#include "fmitem.h"
#include "fontprefs.h"
#include "prefs.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "pdfopts.h"
#include "pdflib.h"
#include "inspage.h"
#include "delpages.h"
#include "movepage.h"
#include "helpbrowser.h"
#include "scribusXml.h"
#include "about.h"
#include "aboutplugins.h"
#include "pslib.h"
#include "druck.h"
#include "editformats.h"
#include "muster.h"
#include "newtemp.h"
#include "applytemplatedialog.h"
#include "picstatus.h"
#include "customfdialog.h"
#include "cmsprefs.h"
#include "annot.h"
#include "annota.h"
#include "javadocs.h"
#include "colorm.h"
#include "mpalette.h"
#include "cpalette.h"
#include "bookpalette.h"
#include "seiten.h"
#include "layers.h"
#include "frameedit.h"
#include "splash.h"
#include "measurements.h"
#include "gtgettext.h"
#include "fileloader.h"
#include "arrowchooser.h"
#include "tabtypography.h"
#include "tabguides.h"
#include "tabtools.h"
#include "undogui.h"
#include "filewatcher.h"
#include "charselect.h"
#include "checkDocument.h"
#include "tabcheckdoc.h"
#include "tabpdfoptions.h"
#include "docitemattrprefs.h"
#include "pageitemattributes.h"
#include "pageitem_textframe.h"
#include "pageitem_imageframe.h"
#include "tocindexprefs.h"
#include "tocgenerator.h"
#include "collect4output.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hysettings.h"
#include "guidemanager.h"
#include "keymanager.h"
#include "mergedoc.h"
#include "lineformats.h"
#include "story.h"
#include "autoform.h"
#include "tabmanager.h"
#include "search.h"
#include "fontcombo.h"
#include "prefsfile.h"
#include "undomanager.h"
#include "polygonwidget.h"
#include "werktoolb.h"
#include "units.h"
#include "hruler.h"
#include "vruler.h"
#include "pageselector.h"
#include "scraction.h"
#include "menumanager.h"
#include "undostate.h"
#include "tree.h"
#include "scrap.h"
#include "pluginmanager.h"
#include "scpaths.h"
#include "pdfoptions.h"
#include "actionmanager.h"
#include "documentinformation.h"
#include "effectsdialog.h"
#include "documentchecker.h"
#include "gsutil.h"
#include "util.h"
#include "pagesize.h"
#include "loremipsum.h"
#include "marginWidget.h"
#include "margindialog.h"
#include "prefsmanager.h"
#include "pagelayout.h"
#include "commonstrings.h"
#include "preview.h"
#include "scribuswin.h"
#include "hyphenator.h"
#include "scmessagebox.h"
#include "imageinfodialog.h"
#include "selection.h"

#if defined(_WIN32)
#include "scwinprint.h"
#endif

using namespace std;

#ifdef HAVE_CMS
cmsHPROFILE CMSoutputProf;
cmsHPROFILE CMSprinterProf;
cmsHTRANSFORM stdTransG;
cmsHTRANSFORM stdProofG;
cmsHTRANSFORM stdTransImgG;
cmsHTRANSFORM stdProofImgG;
cmsHTRANSFORM stdTransCMYKG;
cmsHTRANSFORM stdProofCMYKG;
cmsHTRANSFORM stdTransRGBG;
cmsHTRANSFORM stdProofGCG;
cmsHTRANSFORM stdProofCMYKGCG;
bool BlackPoint;
bool SoftProofing;
bool Gamut;
bool SCRIBUS_API CMSuse;
int IntentMonitor;
int IntentPrinter;
#endif
bool CMSavail;
bool previewDinUse;
bool printDinUse;

QString DocDir;

extern ScribusQApp* ScQApp;
extern bool emergencyActivated;

ScribusMainWindow::ScribusMainWindow()
{
	scribusInitialized=false;
	actionManager=NULL;
	scrMenuMgr=NULL;
	undoManager=NULL;
	prefsManager=NULL;
#ifdef Q_WS_MAC
	noIcon = loadIcon("noicon.xpm");
#endif
} // ScribusMainWindow::ScribusMainWindow()

/*
 * retval 0 - ok, 1 - no fonts, ...
 */
int ScribusMainWindow::initScribus(bool showSplash, bool showFontInfo, const QString newGuiLanguage, const QString prefsUserFile)
{
	CommonStrings::languageChange();
	noneString = tr("None");
	int retVal=0;
	ExternalApp = 0;
	previewDinUse = false;
	printDinUse = false;
	guiLanguage = newGuiLanguage;
	initSplash(showSplash);
	setUsesBigPixmaps(true);
	CurrStED = NULL;
	setCaption( tr("Scribus " VERSION));
	setKeyCompression(false);
	setIcon(loadIcon("AppIcon.png"));
	scrActionGroups.clear();
	scrActionGroups.setAutoDelete(true);
	scrActions.clear();
	scrRecentFileActions.clear();
	scrWindowsActions.clear();
	scrLayersActions.clear();
	scrMenuMgr = new MenuManager(this->menuBar());

	prefsManager = PrefsManager::instance();
	prefsManager->setup();
	PrefsPfad = prefsManager->preferencesLocation();


	undoManager = UndoManager::instance();
	objectSpecificUndo = false;
	pluginManager = new PluginManager();
	tocGenerator = new TOCGenerator();
	actionManager = new ActionManager(this, "actionManager");

	initDefaultValues();
	initMenuBar();
	initStatusBar();
	initToolBars();
	qApp->processEvents();

	BuFromApp = false;

#ifdef QT_MAC
	bool haveFonts=initFonts(true);
#else
	bool haveFonts=initFonts(showFontInfo);
#endif
	if (!haveFonts)
		retVal=1;
	else
	{
		buildFontMenu();
		prefsManager->initDefaults();
		prefsManager->initDefaultGUIFont(qApp->font());
		prefsManager->initArrowStyles();
		resize(610, 600);
		QVBox* vb = new QVBox( this );
		vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
		wsp = new QWorkspace( vb );
		setCentralWidget( vb );
		connect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
		//Connect windows cascade and tile actions to the workspace after its created. Only depends on wsp created.
		connect( scrActions["windowsCascade"], SIGNAL(activated()) , wsp, SLOT(cascade()) );
		connect( scrActions["windowsTile"], SIGNAL(activated()) , wsp, SLOT(tile()) );

		initPalettes();

		fileWatcher = new FileWatcher(this);

		setSplashStatus( tr("Initializing Plugins") );
		pluginManager->initPlugs();
		setSplashStatus( tr("Initializing Keyboard Shortcuts") );
		initKeyboardShortcuts();
		setSplashStatus( tr("Reading Preferences") );
		if (prefsUserFile.isNull())
			prefsManager->ReadPrefs();
		else
			prefsManager->ReadPrefs(prefsUserFile);
		setSplashStatus( tr("Initializing Story Editor") );
		storyEditor = new StoryEditor(this);

		HaveGS = testGSAvailability();
		HavePngAlpha = testGSDeviceAvailability("pngalpha");
		HaveTiffSep = testGSDeviceAvailability("tiffsep");
		DocDir = prefsManager->documentDir();

		setSplashStatus( tr("Reading ICC Profiles") );
		CMSavail = false;
		GetCMSProfiles();
		initCMS();

		setSplashStatus( tr("Initializing Hyphenator") );
		QString preLang = prefsManager->appPrefs.Language;
		initHyphenator();
		if (Sprachen.contains(preLang))
			prefsManager->appPrefs.Language = preLang;
		setSplashStatus( tr("Reading Scrapbook") );
		initScrapbook();
		setSplashStatus( tr("Setting up Shortcuts") );
		SetShortCut();
		scrActions["helpTooltips"]->setOn(prefsManager->appPrefs.showToolTips);
		ToggleTips();
		emit prefsChanged();

		connect(fileWatcher, SIGNAL(fileDeleted(QString )), this, SLOT(removeRecent(QString)));
		connect(this, SIGNAL(TextIFont(QString)), this, SLOT(AdjustFontMenu(QString)));
		connect(this, SIGNAL(TextIFont(QString)), propertiesPalette, SLOT(setFontFace(QString)));
		connect(this, SIGNAL(TextISize(int)), this, SLOT(setFSizeMenu(int)));
		connect(this, SIGNAL(TextISize(int)), propertiesPalette, SLOT(setSize(int)));
		connect(this, SIGNAL(TextUSval(int)), propertiesPalette, SLOT(setExtra(int)));
		connect(this, SIGNAL(TextStil(int)), propertiesPalette, SLOT(setStil(int)));
		connect(this, SIGNAL(TextScale(int)), propertiesPalette, SLOT(setTScale(int)));
		connect(this, SIGNAL(TextScaleV(int)), propertiesPalette, SLOT(setTScaleV(int)));
		connect(this, SIGNAL(TextBase(int)), propertiesPalette, SLOT(setTBase(int)));
		connect(this, SIGNAL(TextShadow(int, int )), propertiesPalette, SLOT(setShadowOffs(int, int )));
		connect(this, SIGNAL(TextOutline(int)), propertiesPalette, SLOT(setOutlineW(int)));
		connect(this, SIGNAL(TextUnderline(int, int)), propertiesPalette, SLOT(setUnderline(int, int)));
		connect(this, SIGNAL(TextStrike(int, int)), propertiesPalette, SLOT(setStrike(int, int)));
		connect(this, SIGNAL(TextFarben(QString, QString, int, int)), propertiesPalette, SLOT(setActFarben(QString, QString, int, int)));
	}
	closeSplash();
	scribusInitialized=true;
	//pluginManager->languageChange();
	setAcceptDrops(true);
	return retVal;
}

ScribusMainWindow::~ScribusMainWindow()
{
}

void ScribusMainWindow::initSplash(bool showSplash)
{
	if (showSplash)
	{
		splashScreen = new SplashScreen();
		if (splashScreen != NULL && splashScreen->isShown())
			setSplashStatus(QObject::tr("Initializing..."));
	}
	else
		splashScreen = NULL;
}

void ScribusMainWindow::setSplashStatus(const QString& newText)
{
	if (splashScreen != NULL)
		splashScreen->setStatus( newText );
	qApp->processEvents();
}

void ScribusMainWindow::showSplash(bool shown)
{
	if (splashScreen!=NULL && shown!=splashScreen->isShown())
		splashScreen->setShown(shown);
}

bool ScribusMainWindow::splashShowing() const
{
	if (splashScreen != NULL)
		return splashScreen->isShown();
	return false;
}

void ScribusMainWindow::closeSplash()
{
	if (splashScreen!=NULL)
	{
		splashScreen->close();
		delete splashScreen;
		splashScreen = NULL;
	}
}

void ScribusMainWindow::initToolBars()
{
	fileToolBar = new ScToolBar(tr("File"), "File", this);
	scrActions["fileNew"]->addTo(fileToolBar);
	scrActions["fileOpen"]->addTo(fileToolBar);
	scrMenuMgr->addMenuToWidgetOfAction("FileOpenRecent", scrActions["fileOpen"]);
	scrActions["fileSave"]->addTo(fileToolBar);
	scrActions["fileClose"]->addTo(fileToolBar);
	scrActions["filePrint"]->addTo(fileToolBar);
	scrActions["toolsPreflightVerifier"]->addTo(fileToolBar);
	scrActions["fileExportAsPDF"]->addTo(fileToolBar);

	editToolBar = new ScToolBar( tr("Edit"), "Edit", this);
	UndoWidget* uWidget = new UndoWidget(editToolBar, "uWidget");
	undoManager->registerGui(uWidget);

	mainToolBar = new WerkToolB(this);
	mainToolBar->setEnabled(false);
	pdfToolBar = new WerkToolBP(this);
	pdfToolBar->setEnabled(false);

	connect(mainToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarTools"], SLOT(setOn(bool)));
	connect(scrActions["toolsToolbarPDF"], SIGNAL(toggled(bool)), pdfToolBar, SLOT(setShown(bool)));
	connect(pdfToolBar, SIGNAL(NewMode(int)), this, SLOT(setAppMode(int)));
	connect(pdfToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarPDF"], SLOT(setOn(bool)));
	connect(scrActions["toolsToolbarTools"], SIGNAL(toggled(bool)), mainToolBar, SLOT(setShown(bool)) );
	
}

//Returns false when there are no fonts
const bool ScribusMainWindow::initFonts(bool showFontInfo)
{
	setSplashStatus( tr("Searching for Fonts") );
	bool haveFonts=prefsManager->GetAllFonts(showFontInfo);
	if (!haveFonts)
	{
		if (splashScreen!=NULL)
			splashScreen->close(); // 10/10/2004 pv fix #1200
		QString mess = tr("There are no fonts found on your system.");
		mess += "\n" + tr("Exiting now.");
		QMessageBox::critical(this, tr("Fatal Error"), mess, 1, 0, 0);
	}
	else
		setSplashStatus( tr("Font System Initialized") );
	return haveFonts;
}

void ScribusMainWindow::initDefaultValues()
{
	dirs = prefsManager->prefsFile->getContext("dirs");
	HaveDoc = false;
	singleClose = false;
	ScriptRunning = false;
	view = NULL;
	doc = NULL;
	Buffer2 = "";
	DispX = 10;
	DispY = 10;
	DocNr = 1;
	PrinterUsed = false;
	PDef.Pname = "";
	PDef.Dname = "";
	PDef.Command = "";
	keyrep = false;
	_arrowKeyDown = false;
	ClipB = QApplication::clipboard();
	PalettesStat[0] = false;
	GuidesStat[0] = false;

	connect(ClipB, SIGNAL(dataChanged()), this, SLOT(ClipChange()));
	connect(ClipB, SIGNAL(selectionChanged()), this, SLOT(ClipChange()));
}

void ScribusMainWindow::initKeyboardShortcuts()
{
	for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrActions.begin(); it!=scrActions.end(); ++it )
	{
		if ((ScrAction*)(it.data())!=NULL)
		{
			QString accelerator=it.data()->accel();
			prefsManager->setKeyEntry(it.key(), it.data()->cleanMenuText(), accelerator,0);
		}
		//else
		//	qDebug(it.key());
		//qDebug(QString("|-\n|%1||%2||%3").arg(it.key()).arg(it.data()->cleanMenuText()).arg(QString(it.data()->accel())));
	}
}

void ScribusMainWindow::initPalettes()
{
	//CB TODO hide the publicly available members of some palettes
	// these must be filtered too as they take control of the palettes events
	outlinePalette = new Tree(this, this);
	connect( scrActions["toolsOutline"], SIGNAL(toggled(bool)) , outlinePalette, SLOT(setPaletteShown(bool)) );
	connect( outlinePalette, SIGNAL(paletteShown(bool)), scrActions["toolsOutline"], SLOT(setOn(bool)));
	propertiesPalette = new Mpalette(this);
	connect( scrActions["toolsProperties"], SIGNAL(toggled(bool)) , propertiesPalette, SLOT(setPaletteShown(bool)) );
	connect( propertiesPalette, SIGNAL(paletteShown(bool)), scrActions["toolsProperties"], SLOT(setOn(bool)));

	//CB dont need this until we have a doc...
	//propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
	propertiesPalette->Cpal->UseTrans(true);
	propertiesPalette->Fonts->RebuildList(0);
	propertiesPalette->installEventFilter(this);
	nodePalette = new NodePalette(this);
	nodePalette->installEventFilter(this);
	layerPalette = new LayerPalette(this);
	connect( scrActions["toolsLayers"], SIGNAL(toggled(bool)) , layerPalette, SLOT(setPaletteShown(bool)) );
	connect( layerPalette, SIGNAL(paletteShown(bool)), scrActions["toolsLayers"], SLOT(setOn(bool)));
	layerPalette->installEventFilter(this);
	layerPalette->Table->installEventFilter(this);
	scrapbookPalette = new Biblio(this);
	connect( scrActions["toolsScrapbook"], SIGNAL(toggled(bool)) , scrapbookPalette, SLOT(setPaletteShown(bool)) );
	connect( scrapbookPalette, SIGNAL(paletteShown(bool)), scrActions["toolsScrapbook"], SLOT(setOn(bool)));
	scrapbookPalette->installEventFilter(this);
	pagePalette = new PagePalette(this);
	connect( scrActions["toolsPages"], SIGNAL(toggled(bool)) , pagePalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsPages"], SIGNAL(toggled(bool)) , this, SLOT(setPagePalette(bool)) );
	connect( pagePalette, SIGNAL(paletteShown(bool)), scrActions["toolsPages"], SLOT(setOn(bool)));
	pagePalette->installEventFilter(this);
	bookmarkPalette = new BookPalette(this);
	connect( scrActions["toolsBookmarks"], SIGNAL(toggled(bool)) , bookmarkPalette, SLOT(setPaletteShown(bool)) );
	connect( bookmarkPalette, SIGNAL(paletteShown(bool)), scrActions["toolsBookmarks"], SLOT(setOn(bool)));
	bookmarkPalette->installEventFilter(this);
	measurementPalette = new Measurements(this);
	connect( scrActions["toolsMeasurements"], SIGNAL(toggled(bool)) , measurementPalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsMeasurements"], SIGNAL(toggledData(bool, int)) , this, SLOT(setAppModeByToggle(bool, int)) );
	connect( measurementPalette, SIGNAL(paletteShown(bool)), scrActions["toolsMeasurements"], SLOT(setOn(bool)));
	measurementPalette->installEventFilter(this);
	measurementPalette->hide();
	docCheckerPalette = new CheckDocument(this, false);
	connect( scrActions["toolsPreflightVerifier"], SIGNAL(toggled(bool)) , docCheckerPalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsPreflightVerifier"], SIGNAL(toggled(bool)) , this, SLOT(docCheckToggle(bool)) );
	connect( docCheckerPalette, SIGNAL(paletteShown(bool)), scrActions["toolsPreflightVerifier"], SLOT(setOn(bool)));
	connect( docCheckerPalette, SIGNAL(paletteShown(bool)), this, SLOT(docCheckToggle(bool)));
	docCheckerPalette->installEventFilter(this);
	docCheckerPalette->hide();

	alignDistributePalette = new AlignDistributePalette(this, "AlignDistributePalette", false);
	connect( scrActions["toolsAlignDistribute"], SIGNAL(toggled(bool)) , alignDistributePalette, SLOT(setPaletteShown(bool)) );
	connect( alignDistributePalette, SIGNAL(paletteShown(bool)), scrActions["toolsAlignDistribute"], SLOT(setOn(bool)));
	connect( alignDistributePalette, SIGNAL(documentChanged()), this, SLOT(slotDocCh()));
	alignDistributePalette->installEventFilter(this);
	//alignDistributePalette->hide();

	undoPalette = new UndoPalette(this, "undoPalette");
	undoPalette->installEventFilter(this);
	undoManager->registerGui(undoPalette);
	connect(undoPalette, SIGNAL(paletteShown(bool)), this, SLOT(setUndoPalette(bool)));
	connect(undoPalette, SIGNAL(objectMode(bool)), this, SLOT(setUndoMode(bool)));


	connect(docCheckerPalette, SIGNAL(selectElement(int, int)), this, SLOT(selectItemsFromOutlines(int, int)));
	connect(docCheckerPalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(docCheckerPalette, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
	connect(outlinePalette, SIGNAL(selectElement(int, int, bool)), this, SLOT(selectItemsFromOutlines(int, int, bool)));
	connect(outlinePalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(outlinePalette, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
	connect(propertiesPalette->Spal, SIGNAL(newStyle(int)), this, SLOT(setNewAbStyle(int)));
	connect(propertiesPalette, SIGNAL(EditLSt()), this, SLOT(slotEditLineStyles()));
	connect(nodePalette, SIGNAL(Schliessen()), this, SLOT(NoFrameEdit()));
	connect(nodePalette, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(layerPalette, SIGNAL(LayerChanged()), this, SLOT(showLayer()));

	connect(bookmarkPalette->BView, SIGNAL(MarkMoved()), this, SLOT(StoreBookmarks()));
	connect(bookmarkPalette->BView, SIGNAL(ChangeBMNr(int, int, int)), this, SLOT(ChBookmarks(int, int, int)));
	connect(bookmarkPalette->BView, SIGNAL(SelectElement(int, int)), this, SLOT(selectItemsFromOutlines(int, int)));
}

void ScribusMainWindow::initScrapbook()
{
	QString scrapbookFileO = QDir::convertSeparators(PrefsPfad+"/scrap13.scs");
	QFileInfo scrapbookFileInfoO = QFileInfo(scrapbookFileO);
	if (scrapbookFileInfoO.exists())
	{
		scrapbookPalette->readOldContents(scrapbookFileO, QDir::convertSeparators(PrefsPfad+"/scrapbook/main"));
		QDir d = QDir();
		d.rename(scrapbookFileO, QDir::convertSeparators(PrefsPfad+"/scrap13.backup"));
	}
	QString scrapbookFile = QDir::convertSeparators(PrefsPfad+"/scrapbook/main");
	QFileInfo scrapbookFileInfo = QFileInfo(scrapbookFile);
	if (scrapbookFileInfo.exists())
		scrapbookPalette->readContents(scrapbookFile);
	scrapbookPalette->setScrapbookFileName(scrapbookFile);
	scrapbookPalette->AdjustMenu();
}

const QString ScribusMainWindow::getGuiLanguage()
{
	return guiLanguage;
}

bool ScribusMainWindow::warningVersion(QWidget *parent)
{
	bool retval = false;
	int t = ScMessageBox::warning(parent, QObject::tr("Scribus Development Version"), "<qt>" +
								 QObject::tr("You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File->Save As. Are you sure you wish to proceed with this operation?") + "</qt>",
								 CommonStrings::tr_OK, CommonStrings::tr_Cancel, "", 1, 0);
	if (t == 0)
		retval = true;
	return retval;
}

void ScribusMainWindow::initMenuBar()
{
	RecentDocs.clear();

	scrMenuMgr->createMenu("File", tr("&File"));
	scrMenuMgr->addMenuItem(scrActions["fileNew"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileOpen"], "File");
	recentFileMenuName="FileOpenRecent";
	scrMenuMgr->createMenu(recentFileMenuName, QIconSet(noIcon), tr("Open &Recent"), "File");
	scrMenuMgr->addMenuSeparator("File");
	scrMenuMgr->addMenuItem(scrActions["fileClose"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileSave"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileSaveAs"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileRevert"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileCollect"], "File");
	scrMenuMgr->addMenuSeparator("File");
	scrMenuMgr->createMenu("FileImport", QIconSet(noIcon), tr("&Import"), "File");
	scrMenuMgr->addMenuItem(scrActions["fileImportText"], "FileImport");
	scrMenuMgr->addMenuItem(scrActions["fileImportAppendText"], "FileImport");
	scrMenuMgr->addMenuItem(scrActions["fileImportImage"], "FileImport");
	scrMenuMgr->createMenu("FileExport", QIconSet(noIcon), tr("&Export"), "File");
	scrMenuMgr->addMenuItem(scrActions["fileExportText"], "FileExport");
	scrMenuMgr->addMenuItem(scrActions["fileExportAsEPS"], "FileExport");
	scrMenuMgr->addMenuItem(scrActions["fileExportAsPDF"], "FileExport");
	scrMenuMgr->addMenuSeparator("File");
	scrMenuMgr->addMenuItem(scrActions["fileDocSetup"], "File");
	scrMenuMgr->addMenuItem(scrActions["filePrint"], "File");
	scrMenuMgr->addMenuItem(scrActions["PrintPreview"], "File");
	scrMenuMgr->addMenuSeparator("File");
	scrMenuMgr->addMenuItem(scrActions["fileQuit"], "File");

	scrActions["fileClose"]->setEnabled(false);
	scrActions["fileSave"]->setEnabled(false);
	scrActions["fileSaveAs"]->setEnabled(false);
	scrActions["fileRevert"]->setEnabled(false);
	scrActions["fileCollect"]->setEnabled(false);
	scrActions["fileImportText"]->setEnabled(false);
	scrActions["fileImportImage"]->setEnabled(false);
	scrActions["fileImportAppendText"]->setEnabled(false);
	scrActions["pageImport"]->setEnabled(false);
	scrActions["fileExportText"]->setEnabled(false);
	scrActions["fileExportAsEPS"]->setEnabled(false);
	scrActions["fileExportAsPDF"]->setEnabled(false);
	scrMenuMgr->setMenuEnabled("FileExport", false);
	scrActions["fileDocSetup"]->setEnabled(false);
	scrActions["filePrint"]->setEnabled(false);
	scrActions["PrintPreview"]->setEnabled(false);

	scrMenuMgr->createMenu("Edit", tr("&Edit"));
	scrMenuMgr->addMenuItem(scrActions["editUndoAction"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editRedoAction"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editActionMode"], "Edit");
	scrMenuMgr->addMenuSeparator("Edit");
	scrMenuMgr->addMenuItem(scrActions["editCut"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editCopy"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editPaste"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editCopyContents"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editPasteContents"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editClearContents"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editSelectAll"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editDeselectAll"], "Edit");
	scrMenuMgr->addMenuSeparator("Edit");
	scrMenuMgr->addMenuItem(scrActions["editSearchReplace"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["toolsEditWithStoryEditor"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editEditWithImageEditor"], "Edit");
	scrMenuMgr->addMenuSeparator("Edit");
	scrMenuMgr->addMenuItem(scrActions["editColors"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editParaStyles"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editLineStyles"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editMasterPages"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editJavascripts"], "Edit");
	scrMenuMgr->addMenuSeparator("Edit");
	scrMenuMgr->addMenuItem(scrActions["editPreferences"], "Edit");
	scrActions["editUndoAction"]->setEnabled(false);
	scrActions["editRedoAction"]->setEnabled(false);
	scrActions["editActionMode"]->setEnabled(true);
	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	scrActions["editPaste"]->setEnabled(false);
	scrActions["editCopyContents"]->setEnabled(false);
	scrActions["editPasteContents"]->setEnabled(false);
	scrActions["editClearContents"]->setEnabled(false);
	scrActions["editSelectAll"]->setEnabled(false);
	scrActions["editDeselectAll"]->setEnabled(false);
	scrActions["editSearchReplace"]->setEnabled(false);
	scrActions["editParaStyles"]->setEnabled(false);
	scrActions["editLineStyles"]->setEnabled(false);
	scrActions["editMasterPages"]->setEnabled(false);
	scrActions["editJavascripts"]->setEnabled(false);
	scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
	scrActions["editEditWithImageEditor"]->setEnabled(false);

	//Style Menu
	scrMenuMgr->createMenu("Style", tr("St&yle"));
	//Color menu
	// CB TODO
	scrMenuMgr->createMenu("Color", tr("&Color"));
	ColorMenC = new ScComboBox(false);
	ColorMenC->setEditable(false);
	scrMenuMgr->addMenuItem(ColorMenC, "Color");

	//Text size menu
	scrMenuMgr->createMenu("FontSize", tr("&Size"));
	scrActionGroups["fontSize"]->addTo(scrMenuMgr->getLocalPopupMenu("FontSize"));

	//Shade menu
	scrMenuMgr->createMenu("Shade", tr("&Shade"));
	scrActionGroups["shade"]->addTo(scrMenuMgr->getLocalPopupMenu("Shade"));

	//Font menu
	scrMenuMgr->createMenu("Font", tr("&Font"));
	FontMenu = scrMenuMgr->getLocalPopupMenu("Font");

	//Type style menu
	scrMenuMgr->createMenu("TypeEffects", tr("&Effects"));
	scrActionGroups["typeEffects"]->addTo(scrMenuMgr->getLocalPopupMenu("TypeEffects"));

	//Item Menu
	scrMenuMgr->createMenu("Item", tr("&Item"));
	scrMenuMgr->addMenuItem(scrActions["itemDuplicate"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemMulDuplicate"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemDelete"], "Item");
	scrMenuMgr->addMenuSeparator("Item");
	scrMenuMgr->addMenuItem(scrActions["itemGroup"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemUngroup"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemLock"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemLockSize"], "Item");
	scrMenuMgr->addMenuSeparator("Item");
	scrMenuMgr->addMenuItem(scrActions["itemImageIsVisible"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemUpdateImage"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemAdjustFrameToImage"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemExtendedImageProperties"], "Item");
	scrMenuMgr->createMenu("ItemPreviewSettings", tr("Preview Settings"), "Item");
	scrMenuMgr->addMenuItem(scrActions["itemPreviewLow"], "ItemPreviewSettings");
	scrMenuMgr->addMenuItem(scrActions["itemPreviewNormal"], "ItemPreviewSettings");
	scrMenuMgr->addMenuItem(scrActions["itemPreviewFull"], "ItemPreviewSettings");
	scrMenuMgr->addMenuSeparator("Item");
	scrMenuMgr->createMenu("ItemLevel", tr("Level"));
	scrMenuMgr->addMenuToMenu("ItemLevel", "Item");
	scrMenuMgr->addMenuItem(scrActions["itemRaise"], "ItemLevel");
	scrMenuMgr->addMenuItem(scrActions["itemLower"], "ItemLevel");
	scrMenuMgr->addMenuItem(scrActions["itemRaiseToTop"], "ItemLevel");
	scrMenuMgr->addMenuItem(scrActions["itemLowerToBottom"], "ItemLevel");
	scrMenuMgr->createMenu("ItemLayer", tr("Send to La&yer"));
	scrMenuMgr->addMenuToMenu("ItemLayer", "Item");
	layerMenuName="ItemLayer";
	scrMenuMgr->addMenuItem(scrActions["itemSendToScrapbook"], "Item");
	scrMenuMgr->addMenuSeparator("Item");
	scrMenuMgr->addMenuItem(scrActions["itemAttributes"], "Item");
	scrMenuMgr->createMenu("ItemPDFOptions", tr("&PDF Options"));
	scrMenuMgr->addMenuToMenu("ItemPDFOptions", "Item");
	scrMenuMgr->addMenuItem(scrActions["itemPDFIsAnnotation"], "ItemPDFOptions");
	scrMenuMgr->addMenuItem(scrActions["itemPDFIsBookmark"], "ItemPDFOptions");
	scrMenuMgr->addMenuItem(scrActions["itemPDFAnnotationProps"], "ItemPDFOptions");
	scrMenuMgr->addMenuItem(scrActions["itemPDFFieldProps"], "ItemPDFOptions");
	scrMenuMgr->createMenu("ItemShapes", tr("&Shape"), "Item");
	// CB TODO
	//Shape menu
	SCustom = new Autoforms(0);
	scrMenuMgr->addMenuItem(SCustom, "ItemShapes");
	connect(SCustom, SIGNAL(FormSel(int, int, double *)), this, SLOT(MakeFrame(int, int, double *)));
	scrMenuMgr->addMenuItem(scrActions["itemShapeEdit"], "ItemShapes");
	scrMenuMgr->createMenu("ItemConvertTo", QPixmap(noIcon), tr("C&onvert To"), "Item");
	//scrMenuMgr->createMenu("ItemConvertTo", tr("C&onvert To"));
	//scrMenuMgr->addMenuToMenu("ItemConvertTo", "Item");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToBezierCurve"], "ItemConvertTo");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToImageFrame"], "ItemConvertTo");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToOutlines"], "ItemConvertTo");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToPolygon"], "ItemConvertTo");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToTextFrame"], "ItemConvertTo");

	scrMenuMgr->addMenuItem(scrActions["itemAttachTextToPath"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemDetachTextFromPath"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemCombinePolygons"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemSplitPolygons"], "Item");
	scrMenuMgr->setMenuEnabled("ItemShapes", false);
	scrActions["itemGroup"]->setEnabled(false);
	scrActions["itemUngroup"]->setEnabled(false);
	scrActions["itemAttachTextToPath"]->setEnabled(false);
	scrActions["itemDetachTextFromPath"]->setEnabled(false);
	scrActions["itemCombinePolygons"]->setEnabled(false);
	scrActions["itemSplitPolygons"]->setEnabled(false);
	scrActions["itemLock"]->setEnabled(false);
	scrActions["itemLockSize"]->setEnabled(false);
	scrActions["itemImageIsVisible"]->setEnabled(false);
	scrMenuMgr->setMenuEnabled("ItemConvertTo", false);
	scrActions["itemConvertToBezierCurve"]->setEnabled(false);
	scrActions["itemConvertToImageFrame"]->setEnabled(false);
	scrActions["itemConvertToOutlines"]->setEnabled(false);
	scrActions["itemConvertToPolygon"]->setEnabled(false);
	scrActions["itemConvertToTextFrame"]->setEnabled(false);

	//Insert menu
	scrMenuMgr->createMenu("Insert", tr("I&nsert"));
	scrMenuMgr->addMenuItem(scrActions["toolsInsertTextFrame"], "Insert");
	scrMenuMgr->addMenuItem(scrActions["toolsInsertImageFrame"], "Insert");
	scrMenuMgr->addMenuItem(scrActions["toolsInsertTableFrame"], "Insert");
	scrMenuMgr->addMenuItem(scrActions["toolsInsertShape"], "Insert");
	scrMenuMgr->addMenuItem(scrActions["toolsInsertPolygon"], "Insert");
	scrMenuMgr->addMenuItem(scrActions["toolsInsertLine"], "Insert");
	scrMenuMgr->addMenuItem(scrActions["toolsInsertBezier"], "Insert");
	scrMenuMgr->addMenuItem(scrActions["toolsInsertFreehandLine"], "Insert");
	scrMenuMgr->addMenuSeparator("Insert");
	scrMenuMgr->addMenuItem(scrActions["insertGlyph"], "Insert");

	scrMenuMgr->createMenu("InsertChar", QPixmap(noIcon), tr("Character"), "Insert");
	//scrMenuMgr->addMenuToMenu("InsertChar", "Insert");
	scrMenuMgr->addMenuItem(scrActions["unicodePageNumber"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeSmartHyphen"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeNonBreakingHyphen"], "InsertChar");
	scrMenuMgr->addMenuSeparator("InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeCopyRight"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeRegdTM"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeTM"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeBullet"], "InsertChar");
	scrMenuMgr->addMenuSeparator("InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeDashEm"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeDashEn"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeDashFigure"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeDashQuotation"], "InsertChar");

	scrMenuMgr->createMenu("InsertQuote", QPixmap(noIcon), tr("Quote"), "Insert");
	//scrMenuMgr->addMenuToMenu("InsertQuote", "Insert");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteApostrophe"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteStraight"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteSingleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteSingleRight"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteDoubleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteDoubleRight"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteLowSingleComma"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteLowDoubleComma"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteSingleReversed"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteDoubleReversed"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteSingleLeftGuillemet"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteSingleRightGuillemet"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteDoubleLeftGuillemet"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteDoubleRightGuillemet"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteCJKSingleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteCJKSingleRight"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteCJKDoubleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteCJKDoubleRight"], "InsertQuote");

	scrMenuMgr->createMenu("InsertSpace", QPixmap(noIcon), tr("Space"), "Insert");
	scrMenuMgr->addMenuItem(scrActions["unicodeNonBreakingSpace"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeNewLine"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeFrameBreak"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeColumnBreak"], "InsertSpace");
	
	scrMenuMgr->createMenu("InsertLigature", QPixmap(noIcon), tr("Ligature"), "Insert");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_ff"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_fi"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_fl"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_ffi"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_ffl"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_ft"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_st"], "InsertLigature");
	
	scrMenuMgr->addMenuSeparator("Insert");
	scrMenuMgr->addMenuItem(scrActions["insertSampleText"], "Insert");
	scrActions["insertGlyph"]->setEnabled(false);

	//Page menu
	scrMenuMgr->createMenu("Page", tr("&Page"));
	scrMenuMgr->addMenuItem(scrActions["pageInsert"], "Page");
	scrMenuMgr->addMenuItem(scrActions["pageImport"], "Page");
	scrMenuMgr->addMenuItem(scrActions["pageDelete"], "Page");
	scrMenuMgr->addMenuItem(scrActions["pageCopy"], "Page");
	scrMenuMgr->addMenuItem(scrActions["pageMove"], "Page");
	scrMenuMgr->addMenuItem(scrActions["pageApplyMasterPage"], "Page");
	scrMenuMgr->addMenuItem(scrActions["pageCopyToMasterPage"], "Page");
	scrMenuMgr->addMenuItem(scrActions["pageManageGuides"], "Page");
	scrMenuMgr->addMenuItem(scrActions["pageManageMargins"], "Page");
	scrMenuMgr->addMenuSeparator("Page");
	scrMenuMgr->addMenuItem(scrActions["viewSnapToGrid"], "Page");
	scrMenuMgr->addMenuItem(scrActions["viewSnapToGuides"], "Page");
	scrActions["pageDelete"]->setEnabled(false);
	scrActions["pageMove"]->setEnabled(false);

	//View menu
	scrMenuMgr->createMenu("View", tr("&View"));
	scrMenuMgr->addMenuItem(scrActions["viewFitInWindow"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit50"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit75"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit100"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit200"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit20"], "View");
	scrMenuMgr->addMenuSeparator("View");
	scrMenuMgr->addMenuItem(scrActions["viewShowMargins"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowFrames"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowImages"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowGrid"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowGuides"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowBaseline"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowTextChain"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowTextControls"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewRulerMode"], "View");

	//CB If this is viewNewView imeplemented, it should be on the windows menu
//	scrMenuMgr->addMenuItem(scrActions["viewNewView"], "View");

	//Tool menu
	/*
	scrMenuMgr->createMenu("Tools", tr("&Tools"));
	scrMenuMgr->addMenuItem(scrActions["toolsProperties"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsOutline"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsScrapbook"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsLayers"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsPages"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsBookmarks"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsMeasurements"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsActionHistory"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsPreflightVerifier"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsAlignDistribute"], "Tools");
	scrMenuMgr->addMenuSeparator("Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsToolbarTools"], "Tools");
	scrMenuMgr->addMenuItem(scrActions["toolsToolbarPDF"], "Tools");
	//scrActions["toolsPreflightVerifier"]->setEnabled(false);*/

	//Extra menu
	scrMenuMgr->createMenu("Extras", tr("E&xtras"));
	scrMenuMgr->addMenuItem(scrActions["extrasManagePictures"], "Extras");
	scrMenuMgr->addMenuItem(scrActions["extrasHyphenateText"], "Extras");
	scrMenuMgr->addMenuItem(scrActions["extrasDeHyphenateText"], "Extras");
	scrMenuMgr->addMenuItem(scrActions["extrasGenerateTableOfContents"], "Extras");

	scrMenuMgr->setMenuEnabled("Extras", false);
	scrActions["extrasHyphenateText"]->setEnabled(false);
	scrActions["extrasDeHyphenateText"]->setEnabled(false);

	//Window menu
	 scrMenuMgr->createMenu("Windows", tr("&Windows"), QString::null, true);
	connect(scrMenuMgr->getLocalPopupMenu("Windows"), SIGNAL(aboutToShow()), this, SLOT(windowsMenuAboutToShow()));
	addDefaultWindowMenuItems();

	//Help menu
	scrMenuMgr->createMenu("Help", tr("&Help"));
	scrMenuMgr->addMenuItem(scrActions["helpAboutScribus"], "Help");
	scrMenuMgr->addMenuItem(scrActions["helpAboutPlugins"], "Help");
	scrMenuMgr->addMenuItem(scrActions["helpAboutQt"], "Help");
	scrMenuMgr->addMenuSeparator("Help");
	scrMenuMgr->addMenuItem(scrActions["helpTooltips"], "Help");
	scrMenuMgr->addMenuItem(scrActions["helpManual"], "Help");

	scrMenuMgr->addMenuToMenuBar("File");
	scrMenuMgr->addMenuToMenuBar("Edit");
	scrMenuMgr->addMenuToMenuBar("Style");
	scrMenuMgr->setMenuEnabled("Style", false);
	scrMenuMgr->addMenuToMenuBar("Item");
	scrMenuMgr->setMenuEnabled("Item", false);
	scrMenuMgr->addMenuToMenuBar("Insert");
	scrMenuMgr->setMenuEnabled("Insert", false);
	scrMenuMgr->addMenuToMenuBar("Page");
	scrMenuMgr->setMenuEnabled("Page", false);
	scrMenuMgr->addMenuToMenuBar("View");
	scrMenuMgr->setMenuEnabled("View", false);
	//scrMenuMgr->addMenuToMenuBar("Tools");
	scrMenuMgr->addMenuToMenuBar("Extras");
	scrMenuMgr->setMenuEnabled("Extras", false);
	scrMenuMgr->addMenuToMenuBar("Windows");
	//scrMenuMgr->setMenuEnabled("Windows", false);
	menuBar()->insertSeparator();
	scrMenuMgr->addMenuToMenuBar("Help");

	//Alignment menu
	scrMenuMgr->createMenu("Alignment", tr("&Alignment"));
	scrMenuMgr->addMenuItem(scrActions["alignLeft"], "Alignment");
	scrMenuMgr->addMenuItem(scrActions["alignCenter"], "Alignment");
	scrMenuMgr->addMenuItem(scrActions["alignRight"], "Alignment");
	scrMenuMgr->addMenuItem(scrActions["alignBlock"], "Alignment");
	scrMenuMgr->addMenuItem(scrActions["alignForced"], "Alignment");

	connect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
	connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
}

void ScribusMainWindow::addDefaultWindowMenuItems()
{
	scrMenuMgr->clearMenu("Windows");
	scrMenuMgr->addMenuItem(scrActions["windowsCascade"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["windowsTile"], "Windows");
	scrMenuMgr->addMenuSeparator("Windows");

	scrMenuMgr->addMenuItem(scrActions["toolsProperties"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsOutline"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsScrapbook"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsLayers"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsPages"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsBookmarks"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsMeasurements"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsActionHistory"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsPreflightVerifier"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsAlignDistribute"], "Windows");
	scrMenuMgr->addMenuSeparator("Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsToolbarTools"], "Windows");
	scrMenuMgr->addMenuItem(scrActions["toolsToolbarPDF"], "Windows");
}


void ScribusMainWindow::initStatusBar()
{
	mainWindowStatusLabel = new QLabel( "           ", statusBar(), "ft");
	mainWindowProgressBar = new QProgressBar(statusBar(), "p");
	mainWindowProgressBar->setCenterIndicator(true);
	mainWindowProgressBar->setFixedWidth( 100 );
	mainWindowProgressBar->reset();
	mainWindowXPosLabel = new QLabel( "X-Pos:", statusBar(), "xt");
	mainWindowYPosLabel = new QLabel( "Y-Pos:", statusBar(), "yt");
	mainWindowXPosDataLabel = new QLabel( "         ", statusBar(), "dt");
	mainWindowYPosDataLabel = new QLabel( "         ", statusBar(), "ydt");

	statusBar()->addWidget(mainWindowStatusLabel, 3, true);
	statusBar()->addWidget(mainWindowProgressBar, 0, true);
	statusBar()->addWidget(mainWindowXPosLabel, 0, true);
	statusBar()->addWidget(mainWindowXPosDataLabel, 1, true);
	statusBar()->addWidget(mainWindowYPosLabel, 0, true);
	statusBar()->addWidget(mainWindowYPosDataLabel, 1, true);
}

void ScribusMainWindow::setMousePositionOnStatusBar(double xp, double yp)
{
	double xn = xp;
	double yn = yp;
	if (doc->guidesSettings.rulerMode)
	{
		xn -= doc->currentPage->xOffset();
		yn -= doc->currentPage->yOffset();
	}
	xn -= doc->rulerXoffset;
	yn -= doc->rulerYoffset;
	QString suffix=unitGetSuffixFromIndex(doc->unitIndex());
	int multiplier=unitGetDecimalsFromIndex(doc->unitIndex());
	double divisor=static_cast<double>(multiplier);
	int precision=unitGetPrecisionFromIndex(doc->unitIndex());
	QString tmp;
	mainWindowXPosDataLabel->setText(tmp.setNum(qRound(xn*doc->unitRatio() * multiplier) / divisor, 'f', precision) + suffix);
	mainWindowYPosDataLabel->setText(tmp.setNum(qRound(yn*doc->unitRatio() * multiplier) / divisor, 'f', precision) + suffix);
}

void ScribusMainWindow::setTBvals(PageItem *currItem)
{
	if (currItem->itemText.count() != 0)
	{
		int ChPos = QMIN(currItem->CPos, static_cast<int>(currItem->itemText.count()-1));
		doc->CurrentStyle = currItem->itemText.at(ChPos)->cstyle & 1919;
		doc->currentParaStyle = currItem->itemText.at(ChPos)->cab;
		setAbsValue(doc->currentParaStyle);
		propertiesPalette->setAli(doc->currentParaStyle);
		doc->CurrFont = currItem->itemText.at(ChPos)->cfont->scName();
		doc->CurrFontSize = currItem->itemText.at(ChPos)->csize;
		doc->CurrTextFill = currItem->itemText.at(ChPos)->ccolor;
		doc->CurrTextFillSh = currItem->itemText.at(ChPos)->cshade;
		doc->CurrTextStroke = currItem->itemText.at(ChPos)->cstroke;
		doc->CurrTextStrokeSh = currItem->itemText.at(ChPos)->cshade2;
		doc->CurrTextScale = currItem->itemText.at(ChPos)->cscale;
		doc->CurrTextScaleV = currItem->itemText.at(ChPos)->cscalev;
		doc->CurrTextBase = currItem->itemText.at(ChPos)->cbase;
		doc->CurrTextShadowX = currItem->itemText.at(ChPos)->cshadowx;
		doc->CurrTextShadowY = currItem->itemText.at(ChPos)->cshadowy;
		doc->CurrTextOutline = currItem->itemText.at(ChPos)->coutline;
		doc->CurrTextUnderPos = currItem->itemText.at(ChPos)->cunderpos;
		doc->CurrTextUnderWidth = currItem->itemText.at(ChPos)->cunderwidth;
		doc->CurrTextStrikePos = currItem->itemText.at(ChPos)->cstrikepos;
		doc->CurrTextStrikeWidth = currItem->itemText.at(ChPos)->cstrikewidth;
		emit TextUnderline(doc->CurrTextUnderPos, doc->CurrTextUnderWidth);
		emit TextStrike(doc->CurrTextStrikePos, doc->CurrTextStrikeWidth);
		emit TextShadow(doc->CurrTextShadowX, doc->CurrTextShadowY);
		emit TextFarben(doc->CurrTextStroke, doc->CurrTextFill, doc->CurrTextStrokeSh, doc->CurrTextFillSh);
		emit TextIFont(doc->CurrFont);
		emit TextISize(doc->CurrFontSize);
		emit TextUSval(currItem->itemText.at(ChPos)->cextra);
		emit TextStil(doc->CurrentStyle);
		emit TextScale(doc->CurrTextScale);
		emit TextScaleV(doc->CurrTextScaleV);
		emit TextBase(doc->CurrTextBase);
		emit TextOutline(doc->CurrTextOutline);
	}
}

void ScribusMainWindow::wheelEvent(QWheelEvent *w)
{
	if (HaveDoc)
	{
		int wheelVal=prefsManager->mouseWheelValue();
		if ((w->orientation() != Qt::Vertical) || ( w->state() & ShiftButton ))
		{
			if (w->delta() < 0)
				view->scrollBy(wheelVal, 0);
			else
				view->scrollBy(-wheelVal, 0);
		}
		else
		{
			if (w->delta() < 0)
				view->scrollBy(0, wheelVal);
			else
				view->scrollBy(0, -wheelVal);
		}
		w->accept();
	}
}

//Special keys assigned to actions are stolen by the action and not passed to
//keyPressEvent so process them here.
void ScribusMainWindow::specialActionKeyEvent(QString actionName, int unicodevalue)
{
	if (unicodevalue!=-1 && HaveDoc)
	{
		if (doc->appMode==modeEdit)
		{
			if (doc->selection->count() == 1)
			{
				struct ScText *hg = new ScText;
				PageItem *currItem = doc->selection->itemAt(0);
				if (currItem!=NULL)
				{
					if (currItem->HasSel && currItem->itemType()==PageItem::TextFrame)
						currItem->asTextFrame()->deleteSelectedTextFromFrame();

					hg->ch = QString(QChar(unicodevalue));
					doc->setScTextDefaultsFromDoc(hg);
					hg->cselect = false;
					hg->cextra = 0;
					hg->xp = 0;
					hg->yp = 0;
					hg->PRot = 0;
					hg->PtransX = 0;
					hg->PtransY = 0;
					hg->cembedded = 0;
					currItem->itemText.insert(currItem->CPos, hg);
					currItem->CPos += 1;
					currItem->Tinput = true;
					view->RefreshItem(currItem);
				}
				else if (actionName=="unicodeSmartHyphen") //ignore the char as we use an attribute if the text item, for now.
				{
					currItem->itemText.at(QMAX(currItem->CPos-1,0))->cstyle ^= 128;
					currItem->Tinput = true;
					view->RefreshItem(currItem);
				}
			}
		}
	}
}

/*!
  \brief Receive key events from palettes such as palette hiding events. Possibly easier way but this is cleaner than before. No need to modify all those palettes and each new one in future.
 */
bool ScribusMainWindow::eventFilter( QObject */*o*/, QEvent *e )
{
	bool retVal;
	if ( e->type() == QEvent::KeyPress ) {
		QKeyEvent *k = (QKeyEvent *)e;
		int keyMod=0;
		if (k->state() & ShiftButton)
			keyMod |= SHIFT;
		if (k->state() & ControlButton)
			keyMod |= CTRL;
		if (k->state() & AltButton)
			keyMod |= ALT;

		QKeySequence currKeySeq = QKeySequence(k->key() | keyMod);
		if (QString(currKeySeq).isNull())
			return false;
		retVal=true;
		//Palette actions
		if (currKeySeq == scrActions["specialToggleAllPalettes"]->accel())
			scrActions["specialToggleAllPalettes"]->activate();
		else
		if (currKeySeq == scrActions["toolsProperties"]->accel())
			scrActions["toolsProperties"]->toggle();
		else
		if (currKeySeq == scrActions["toolsOutline"]->accel())
			scrActions["toolsOutline"]->toggle();
		else
		if (currKeySeq == scrActions["toolsScrapbook"]->accel())
			scrActions["toolsScrapbook"]->toggle();
		else
		if (currKeySeq == scrActions["toolsLayers"]->accel())
			scrActions["toolsLayers"]->toggle();
		else
		if (currKeySeq == scrActions["toolsPages"]->accel())
			scrActions["toolsPages"]->toggle();
		else
		if (currKeySeq == scrActions["toolsBookmarks"]->accel())
			scrActions["toolsBookmarks"]->toggle();
		else
		if (currKeySeq == scrActions["toolsActionHistory"]->accel())
			scrActions["toolsActionHistory"]->toggle();
		else
		if (currKeySeq == scrActions["toolsPreflightVerifier"]->accel())
			scrActions["toolsPreflightVerifier"]->toggle();
		else
		if (currKeySeq == scrActions["toolsAlignDistribute"]->accel())
			scrActions["toolsAlignDistribute"]->toggle();
		else
		//Undo actions
		if (currKeySeq == scrActions["editUndoAction"]->accel() && scrActions["editUndoAction"]->isEnabled())
			scrActions["editUndoAction"]->activate();
		else
		if (currKeySeq == scrActions["editRedoAction"]->accel() && scrActions["editRedoAction"]->isEnabled())
			scrActions["editRedoAction"]->activate();
		else
		//Other actions
		if (currKeySeq == scrActions["fileQuit"]->accel())
			scrActions["fileQuit"]->activate();
		else
		//Zoom actions
		if (currKeySeq == scrActions["toolsZoomIn"]->accel())
			scrActions["toolsZoomIn"]->activate();
		else
		if (currKeySeq == scrActions["toolsZoomOut"]->accel())
			scrActions["toolsZoomOut"]->activate();
		else
			retVal=false;
	}
	else
		retVal=false;
	//Return false to pass event to object
	return retVal;
}

void ScribusMainWindow::keyPressEvent(QKeyEvent *k)
{
	QWidgetList windows;
	QWidget* w = NULL;
	//struct ScText *hg;
	int kk = k->key();
	//int as = k->ascii();
	//double altx, alty;
	QString uc = k->text();
	QString cr, Tcha, Twort;
	//uint Tcoun;
	//int len, pos, c;
	if (keyrep)
		return;
	keyrep = true;
	int KeyMod;
	switch (k->state())
	{
	case ShiftButton:
		KeyMod = SHIFT;
		break;
	case AltButton:
		KeyMod = ALT;
		break;
	case ControlButton:
		KeyMod = CTRL;
		break;
	default:
		KeyMod = 0;
		break;
	}
	//User presses escape and we have a doc open, and we have an item selected
	if ((kk == Key_Escape) && (HaveDoc))
	{
		keyrep = false;
		PageItem *currItem;
		if (doc->selection->count() != 0)
		{
			currItem = doc->selection->itemAt(0);
			switch (doc->appMode)
			{
				case modeNormal:
					currItem->Sizing = false;
					if (doc->SubMode != -1)
					{
						view->Deselect(false);
						doc->Items->remove(currItem->ItemNr);
					}
					break;
				case modeLinkFrames:
				case modeUnlinkFrames:
				case modeEdit:
				case modeRotation:
						view->Deselect(false);
				case modePanning:
					break;
				case modeDrawBezierLine:
					currItem->PoLine.resize(currItem->PoLine.size()-2);
					if (currItem->PoLine.size() < 4)
					{
						view->Deselect(false);
						doc->Items->remove(currItem->ItemNr);
					}
					else
					{
						view->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false);
						currItem->SetPolyClip(qRound(QMAX(currItem->lineWidth() / 2.0, 1)));
						view->AdjustItemSize(currItem);
						currItem->ContourLine = currItem->PoLine.copy();
						currItem->ClipEdited = true;
						currItem->FrameType = 3;
						slotDocCh();
					}
					view->FirstPoly = true;
					break;
				default:
					view->Deselect(false);
					doc->Items->remove(currItem->ItemNr);
					break;
			}
		}
		view->Mpressed = false;
		doc->DragP = false;
		doc->leaveDrag = false;
		view->Imoved = false;
		view->mCG = false;
		view->MidButt = false;
		doc->SubMode = -1;
		doc->ElemToLink = NULL;
		NoFrameEdit();
		slotSelect();
		return;
	}
	ButtonState buttonState = k->state();
	/**If we have a doc and we are not changing the page or zoom level in the status bar */
	if ((HaveDoc) && (!view->zoomSpinBox->hasFocus()) && (!view->pageSelector->hasFocus()))
	{
		/**
		 * With no item selected we can:
		 * - With space, get into panning mode (modePanning)
		 * - With PageUp, scroll up
		 * - With PageDown, scroll down
		 * - With Tab, change active document windowActivated
		 */

		if ((doc->appMode != modeEdit) && (doc->selection->count() == 0))
		{
			switch (kk)
			{
			case Key_Space:
				keyrep = false;
				if (doc->appMode == modePanning)
					setAppMode(modeNormal);
				else
					setAppMode(modePanning);
				return;
				break;
			case Key_Prior:
				view->scrollBy(0, -prefsManager->mouseWheelValue());
				keyrep = false;
				return;
				break;
			case Key_Next:
				view->scrollBy(0, prefsManager->mouseWheelValue());
				keyrep = false;
				return;
				break;
			case Key_Tab:
				keyrep = false;
				windows = wsp->windowList();
				if (windows.count() > 1)
				{
					for (int i = 0; i < static_cast<int>(windows.count()); ++i)
					{
						if (wsp->activeWindow() == windows.at(i))
						{
							if (i == static_cast<int>(windows.count()-1))
								w = windows.at(0);
							else
								w = windows.at(i+1);
							break;
						}
					}
					doc->OpenNodes = outlinePalette->buildReopenVals();
					docCheckerPalette->clearErrorList();
					if ( w )
						w->showNormal();
					newActWin(w);
				}
				return;
				break;
			}
		}
		/** Now if we have an item selected
		 * - In normal mode we can:
		 * -- Use backspace or delete to delete the item
		 * -- Use PageUp to raise an item
		 * -- Use PageDown to lower an item
		 * -- Use the arrow keys to move an item or group around, with no meta, by 1.0, Ctrl by 0.1, Shift by 10.0.
		 */
		if (doc->selection->count() != 0)
		{
			double moveBy=1.0;
			if (buttonState & ShiftButton)
				moveBy=10.0;
			else if (buttonState & ControlButton)
				moveBy=0.1;

			PageItem *currItem = doc->selection->itemAt(0);
			switch (doc->appMode)
			{
			case modeNormal:
				switch (kk)
				{
				case Key_Backspace:
				case Key_Delete:
					if (!doc->EditClip)
					{
						view->DeleteItem();
						slotDocCh();
					}
					break;
				case Key_Prior:
					if (!currItem->locked())
					{
						view->RaiseItem();
						slotDocCh();
					}
					break;
				case Key_Next:
					if (!currItem->locked())
					{
						view->LowerItem();
						slotDocCh();
					}
					break;
				case Key_Left:
					if (!currItem->locked())
					{
						if ((doc->EditClip) && (view->ClRe != -1))
						{
							FPoint np;
							if (view->EditContour)
								np = currItem->ContourLine.point(view->ClRe);
							else
								np = currItem->PoLine.point(view->ClRe);
							np = np - FPoint(moveBy, 0);
							view->MoveClipPoint(currItem, np);
						}
						else
						{
							view->moveGroup(-moveBy, 0);
						}
						slotDocCh();
					}
					break;
				case Key_Right:
					if (!currItem->locked())
					{
						if ((doc->EditClip) && (view->ClRe != -1))
						{
							FPoint np;
							if (view->EditContour)
								np = currItem->ContourLine.point(view->ClRe);
							else
								np = currItem->PoLine.point(view->ClRe);
							np = np + FPoint(moveBy, 0);
							view->MoveClipPoint(currItem, np);
						}
						else
						{
							view->moveGroup(moveBy, 0);
						}
						slotDocCh();
					}
					break;
				case Key_Up:
					if (!currItem->locked())
					{
						if ((doc->EditClip) && (view->ClRe != -1))
						{
							FPoint np;
							if (view->EditContour)
								np = currItem->ContourLine.point(view->ClRe);
							else
								np = currItem->PoLine.point(view->ClRe);
							np = np - FPoint(0, moveBy);
							view->MoveClipPoint(currItem, np);
						}
						else
						{
							view->moveGroup(0, -moveBy);
						}
						slotDocCh();
					}
					break;
				case Key_Down:
					if (!currItem->locked())
					{
						if ((doc->EditClip) && (view->ClRe != -1))
						{
							FPoint np;
							if (view->EditContour)
								np = currItem->ContourLine.point(view->ClRe);
							else
								np = currItem->PoLine.point(view->ClRe);
							np = np + FPoint(0, moveBy);
							view->MoveClipPoint(currItem, np);
						}
						else
						{
							view->moveGroup(0, moveBy);
						}
						slotDocCh();
					}
					break;
				default:
					break;
				}
				break;
			case modeEdit:
				if (currItem->asImageFrame() && !currItem->locked())
				{
					double dX=0.0,dY=0.0;
					switch (kk)
					{
						case Key_Left:
							dX=-moveBy;
							break;
						case Key_Right:
							dX=moveBy;
							break;
						case Key_Up:
							dY=-moveBy;
							break;
						case Key_Down:
							dY=moveBy;
							break;
					}
					if (dX!=0.0 || dY!=0.0)
					{
						//view->MoveItemI(currItem, dX, dY, true);
						currItem->moveImageInFrame(dX, dY);
						view->updateContents(currItem->getRedrawBounding(view->scale()));
					}
				}
				view->oldCp = currItem->CPos;
				if (currItem->itemType() == PageItem::TextFrame)
				{
					bool kr=keyrep;
					currItem->handleModeEditKey(k, keyrep);
					keyrep=kr;
				}
				slotDocCh(false);
				break;
			}
		}
	}
	switch(kk)
	{
		case Key_Left:
		case Key_Right:
		case Key_Up:
		case Key_Down:
			_arrowKeyDown = true;
	}
	keyrep = false;
}

void ScribusMainWindow::keyReleaseEvent(QKeyEvent *k)
{
	if (k->state() & ControlButton)
	{
		if (HaveDoc)
		{
			if ((doc->appMode == modePanning) && (k->state() & Qt::RightButton))
				setAppMode(modeNormal);
		}
	}
	if (k->isAutoRepeat() || !_arrowKeyDown)
		return;
	switch(k->key())
	{
		case Key_Left:
		case Key_Right:
		case Key_Up:
		case Key_Down:
			_arrowKeyDown = false;
			if ((HaveDoc) && (!view->zoomSpinBox->hasFocus()) && (!view->pageSelector->hasFocus()))
			{
				uint docSelectionCount=doc->selection->count();
				if ((docSelectionCount != 0) && (doc->appMode == modeNormal) && (doc->EditClip) && (view->ClRe != -1))
					view->updateContents();
				for (uint i = 0; i < docSelectionCount; ++i)
					doc->selection->itemAt(i)->checkChanges(true);
				if (docSelectionCount > 1 && view->groupTransactionStarted())
					undoManager->commit();
			}
			break;
	}
}

void ScribusMainWindow::closeEvent(QCloseEvent *ce)
{
	QWidgetList windows = wsp->windowList();
	ScribusWin* tw;
	disconnect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
	if (!windows.isEmpty())
	{
		singleClose = true;
		uint windowCount=windows.count();
		for ( uint i = 0; i < windowCount; ++i )
		{
			newActWin(windows.at(i));
			tw = ActWin;
			ActWin->close();
			if (tw == ActWin)
			{
				ce->ignore();
				singleClose = false;
				connect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
				return;
			}
		}
	}
	propertiesPalette->hide();
	outlinePalette->hide();
	scrapbookPalette->hide();
	bookmarkPalette->hide();
	layerPalette->hide();
	pagePalette->hide();
	measurementPalette->hide();
	docCheckerPalette->hide();
	undoPalette->hide();
	alignDistributePalette->hide();

	// Clean up plugins, THEN save prefs to disk
	pluginManager->cleanupPlugins();
	if (!emergencyActivated)
		prefsManager->SavePrefs();
	UndoManager::deleteInstance();
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	exit(0);
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


bool ScribusMainWindow::arrowKeyDown()
{
	return _arrowKeyDown;
}

void ScribusMainWindow::startUpDialog()
{
	PrefsContext* docContext = prefsManager->prefsFile->getContext("docdirs", false);
	NewDoc* dia = new NewDoc(this, true);
	if (dia->exec())
	{
		if (dia->tabSelected == 0)
		{
			int facingPages = dia->choosenLayout;
			int firstPage = dia->docLayout->firstPage->currentItem();
			double topMargin = dia->GroupRand->RandT;
			double bottomMargin = dia->GroupRand->RandB;
			double leftMargin = dia->GroupRand->RandL;
			double rightMargin = dia->GroupRand->RandR;
			double columnDistance = dia->Dist;
			double pageWidth = dia->pageWidth;
			double pageHeight = dia->pageHeight;
			double numberCols = dia->SpinBox10->value();
			bool autoframes = dia->AutoFrame->isChecked();
			int orientation = dia->Orient;
			int pageCount=dia->PgNum->value();
			PageSize *ps2 = new PageSize(dia->pageSizeComboBox->currentText());
			QString pagesize = ps2->getPageName();
			doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->ComboBox3->currentItem(), firstPage, orientation, 1, pagesize, pageCount);
			doc->pageSets[facingPages].FirstPage = firstPage;
			delete ps2;
			HaveNewDoc();
		}
		else
		{
			if (dia->tabSelected == 1)
			{
				QString fileName(dia->fileDialog->selectedFile());
				if (!fileName.isEmpty())
				{
					docContext->set("docsopen", fileName.left(fileName.findRev("/")));
					loadDoc(fileName);
				}
			}
			else
			{
				QString fileName(dia->recentDocList->currentText());
				if (!fileName.isEmpty())
					loadRecent(fileName);
			}
		}
	}
	prefsManager->setShowStartupDialog(!dia->startUpDialog->isChecked());
	delete dia;
	mainWindowStatusLabel->setText( tr("Ready"));
}

bool ScribusMainWindow::slotFileNew()
{
	bool retVal;
	NewDoc* dia = new NewDoc(this);
	if (dia->exec())
	{
		int facingPages = dia->choosenLayout;
		int firstPage = dia->docLayout->firstPage->currentItem();
		double topMargin = dia->GroupRand->RandT;
		double bottomMargin = dia->GroupRand->RandB;
		double leftMargin = dia->GroupRand->RandL;
		double rightMargin = dia->GroupRand->RandR;
		double columnDistance = dia->Dist;
		double pageWidth = dia->pageWidth;
		double pageHeight = dia->pageHeight;
		double numberCols = dia->SpinBox10->value();
		bool autoframes = dia->AutoFrame->isChecked();
		int orientation = dia->Orient;
		int pageCount=dia->PgNum->value();
		PageSize *ps2 = new PageSize(dia->pageSizeComboBox->currentText());
		QString pagesize = ps2->getPageName();
		retVal = doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->ComboBox3->currentItem(), firstPage, orientation, 1, pagesize, pageCount);
		doc->pageSets[facingPages].FirstPage = firstPage;
		mainWindowStatusLabel->setText( tr("Ready"));
		delete ps2;
		HaveNewDoc();
	}
	else
		retVal = false;
	delete dia;
	return retVal;
}

bool ScribusMainWindow::doFileNew(double width, double h, double tpr, double lr, double rr, double br, double ab, double sp,
                           bool atf, int fp, int einh, int firstleft, int Ori, int SNr, const QString& defaultPageSize, int pageCount)
{
	QString cc;
	if (HaveDoc)
	{
		doc->OpenNodes = outlinePalette->buildReopenVals();
	}
	doc = new ScribusDoc();
	doc->setLoading(true);
	doc->setup(einh, fp, firstleft, Ori, SNr, defaultPageSize, doc->DocName+cc.setNum(DocNr));
	HaveDoc++;
	DocNr++;
	if (CMSavail && doc->CMSSettings.CMSinUse)
		recalcColors();
	doc->setPage(width, h, tpr, lr, rr, br, sp, ab, atf, fp);
	doc->setMasterPageMode(false);
	doc->addMasterPage(0, "Normal");
	int createCount=pageCount;
	if (createCount<=0)
		createCount=1;
	for (int i = 0; i < createCount; ++i)
		doc->addPage(i, "Normal", true);
	doc->addSection();
	doc->setFirstSectionFromFirstPageNumber();
	doc->setModified(false);
	doc->setLoading(false);
	doc->currentPage = doc->Pages->at(0);
	doc->OpenNodes.clear();

	//<<View and window code
	ScribusWin* w = new ScribusWin(wsp, doc);
	if (view!=NULL)
	{
		actionManager->disconnectNewViewActions();
		disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	}
	view = new ScribusView(w, doc);
	view->setScale(prefsManager->displayScale());
	actionManager->connectNewViewActions(view);
	alignDistributePalette->setView(view);
	docCheckerPalette->clearErrorList();
	w->setView(view);
	ActWin = w;
	doc->WinHan = w;
	w->setCentralWidget(view);
	view->reformPages(true);
	//>>

	connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	//connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
	connect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	//	connect(w, SIGNAL(SaveAndClose()), this, SLOT(DoSaveClose()));

	//Independent finishing tasks after doc setup
	if ( wsp->windowList().isEmpty() )
		w->showMaximized();
	else
		w->show();
	view->show();
	connect(doc->autoSaveTimer, SIGNAL(timeout()), w, SLOT(slotAutoSave()));
	connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	connect(fileWatcher, SIGNAL(fileChanged(QString)), view, SLOT(updatePict(QString)));
	connect(fileWatcher, SIGNAL(fileDeleted(QString)), view, SLOT(removePict(QString)));
	connect(doc, SIGNAL(refreshItem(PageItem*)), view, SLOT(RefreshItem(PageItem*)));
	scrActions["fileSave"]->setEnabled(false);
	undoManager->switchStack(doc->DocName);
	tocGenerator->setDoc(doc);
	return true;
}

void ScribusMainWindow::newView()
{
	ScribusWin* w = new ScribusWin(wsp, doc);
	view = new ScribusView(w, doc);
	view->setScale(prefsManager->displayScale());
	w->setView(view);
	ActWin = w;
	w->setCentralWidget(view);
	alignDistributePalette->setView(view);
	connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	//connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
}

/*CB Unnecessary code, used once, lets use the slot directly
bool ScribusMainWindow::DoSaveClose()
{
	return slotFileSave();
}
*/

void ScribusMainWindow::windowsMenuAboutToShow()
{
	for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrWindowsActions.begin(); it!=scrWindowsActions.end(); ++it )
		scrMenuMgr->removeMenuItem((*it), "Windows");
	scrWindowsActions.clear();
	addDefaultWindowMenuItems();
	QWidgetList windows = wsp->windowList();
	bool windowsListNotEmpty=!windows.isEmpty();
	scrActions["windowsCascade"]->setEnabled(windowsListNotEmpty);
	scrActions["windowsTile"]->setEnabled(windowsListNotEmpty);
	if (windowsListNotEmpty)
	{
		scrMenuMgr->addMenuSeparator("Windows");

		int windowCount=static_cast<int>(windows.count());
		for ( int i = 0; i < windowCount; ++i )
		{
			QString docInWindow=windows.at(i)->caption();
			scrWindowsActions.insert(docInWindow, new ScrAction( ScrAction::Window, noIcon, docInWindow, QKeySequence(), this, docInWindow, i));
			scrWindowsActions[docInWindow]->setToggleAction(true);
			connect( scrWindowsActions[docInWindow], SIGNAL(activatedData(int)), this, SLOT(windowsMenuActivated(int)) );
			scrMenuMgr->addMenuItem(scrWindowsActions[docInWindow], "Windows");
			scrWindowsActions[docInWindow]->setOn(wsp->activeWindow() == windows.at(i));
		}
	}
}

void ScribusMainWindow::newActWin(QWidget *w)
{
	if (w == NULL)
	{
		ActWin = NULL;
		return;
	}
	ActWin = (ScribusWin*)w;
	if (ActWin->document()==NULL)
		return;
	QString oldDocName = "";
	if (ActWin && ActWin->document())
		oldDocName = ActWin->document()->DocName;

/*	if (doc != NULL)
	{
		if ((HaveDoc) && (doc != ActWin->doc))
			doc->OpenNodes = outlinePalette->buildReopenVals();
	} */
	docCheckerPalette->clearErrorList();
	QString newDocName = "";
	if (ActWin && ActWin->document())
		newDocName = ActWin->document()->DocName;

	if (oldDocName != newDocName)
		undoManager->switchStack(newDocName);

	if (view!=NULL)
	{
		actionManager->disconnectNewViewActions();
		disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
		if (ScQApp->usingGUI())
			disconnect(doc->selection, SIGNAL(selectionIsMultiple(bool)), 0, 0);
	}
	doc = ActWin->document();
	view = ActWin->view();
	actionManager->connectNewViewActions(view);
	connect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	if (ScQApp->usingGUI())
		connect(doc->selection, SIGNAL(selectionIsMultiple(bool)), propertiesPalette, SLOT( setMultipleSelection(bool)));

	pagePalette->setView(view);
	alignDistributePalette->setView(view);
	if (!doc->isLoading())
	{
		scanDocument();
		docCheckerPalette->buildErrorList(doc);
		SwitchWin();
		QWidgetList windows = wsp->windowList();
		ScribusWin* swin;
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
		{
			swin = (ScribusWin*)windows.at(i);
			if (swin==ActWin && doc->masterPageMode())
				swin->setMasterPagesPaletteShown(true);
			else
				swin->setMasterPagesPaletteShown(false);
		}
		//if (doc->masterPageMode())
		//	ActWin->setMasterPagesPaletteShown(true);
		setAppMode(doc->appMode);
	}
	w->setFocus();
	wsp->setScrollBarsEnabled(!(w->isMaximized()));
	scrActions["viewShowMargins"]->setOn(doc->guidesSettings.marginsShown);
	scrActions["viewShowFrames"]->setOn(doc->guidesSettings.framesShown);
	scrActions["viewShowGrid"]->setOn(doc->guidesSettings.gridShown);
	scrActions["viewShowGuides"]->setOn(doc->guidesSettings.guidesShown);
	scrActions["viewShowBaseline"]->setOn(doc->guidesSettings.baseShown);
	scrActions["viewShowImages"]->setOn(doc->guidesSettings.showPic);
	scrActions["viewShowTextChain"]->setOn(doc->guidesSettings.linkShown);
	scrActions["viewShowTextControls"]->setOn(doc->guidesSettings.showControls);
	scrActions["viewRulerMode"]->setOn(doc->guidesSettings.rulerMode);
	if (!doc->masterPageMode())
		pagePalette->Rebuild();
	outlinePalette->setDoc(doc);
	outlinePalette->BuildTree();
//	outlinePalette->reopenTree(doc->OpenNodes);
/*	bookmarkPalette->BView->NrItems = doc->NrItems;
	bookmarkPalette->BView->First = doc->First;
	bookmarkPalette->BView->Last = doc->Last; */
	RestoreBookMarks();
	if (!doc->isLoading())
	{
		if (doc->selection->count() != 0)
		{
			HaveNewSel(doc->selection->itemAt(0)->itemType());
			doc->selection->itemAt(0)->emitAllToGUI();
		}
		else
			HaveNewSel(-1);
	}
	tocGenerator->setDoc(doc);
}

void ScribusMainWindow::windowsMenuActivated( int id )
{
	if (HaveDoc)
		doc->OpenNodes = outlinePalette->buildReopenVals();
	QWidget* windowWidget = wsp->windowList().at( id );
	if ( windowWidget )
		windowWidget->showNormal();
	newActWin(windowWidget);
}

bool ScribusMainWindow::slotDocSetup()
{
	bool ret = false;
	ReformDoc* dia = new ReformDoc(this, doc);
	Q_CHECK_PTR(dia);
	if (dia->exec())
	{
		slotChangeUnit(dia->getSelectedUnit(), false);
		dia->updateDocumentSettings();
		if (dia->imageResolutionChanged())
			view->RecalcPicturesRes();
		FontSub->RebuildList(doc);
		propertiesPalette->Fonts->RebuildList(doc);
		scrActions["viewShowMargins"]->setOn(doc->guidesSettings.marginsShown);
		scrActions["viewShowFrames"]->setOn(doc->guidesSettings.framesShown);
		scrActions["viewShowGrid"]->setOn(doc->guidesSettings.gridShown);
		scrActions["viewShowGuides"]->setOn(doc->guidesSettings.guidesShown);
		scrActions["viewShowBaseline"]->setOn(doc->guidesSettings.baseShown);
		scrActions["viewShowImages"]->setOn(doc->guidesSettings.showPic);
		scrActions["viewShowTextChain"]->setOn(doc->guidesSettings.linkShown);
		scrActions["viewShowTextControls"]->setOn(doc->guidesSettings.showControls);
		scrActions["viewRulerMode"]->setOn(doc->guidesSettings.rulerMode);
		view->reformPages();
		view->GotoPage(doc->currentPage->pageNr());
		view->DrawNew();
		propertiesPalette->ShowCMS();
		pagePalette->RebuildPage();
		slotDocCh();
		ret = true;
	}
	delete dia;
	return ret;
}

void ScribusMainWindow::SwitchWin()
{
	updateColorMenu();
	buildFontMenu();
#ifdef HAVE_CMS
	SoftProofing = doc->SoftProofing;
	Gamut = doc->Gamut;
	IntentPrinter = doc->IntentPrinter;
	IntentMonitor = doc->IntentMonitor;
	stdProofG = doc->stdProof;
	stdTransG = doc->stdTrans;
	stdProofImgG = doc->stdProofImg;
	stdTransImgG = doc->stdTransImg;
	stdProofCMYKG = doc->stdProofCMYK;
	stdTransCMYKG = doc->stdTransCMYK;
	stdTransRGBG = doc->stdTransRGB;
	stdProofGCG = doc->stdProofGC;
	stdProofCMYKGCG = doc->stdProofCMYKGC;
	CMSoutputProf = doc->DocOutputProf;
	CMSprinterProf = doc->DocPrinterProf;
	CMSuse = doc->CMSSettings.CMSinUse;
#endif
	propertiesPalette->updateColorList();
	propertiesPalette->Cpal->ChooseGrad(0);
	ActWin->setCaption(doc->DocName);
	scrActions["shade100"]->setOn(true);
	//ShadeMenu->setItemChecked(ShadeMenu->idAt(11), true);
	propertiesPalette->setDoc(doc);
	propertiesPalette->updateCList();
	pagePalette->setView(view);
	propertiesPalette->Spal->setFormats(doc);
	propertiesPalette->SetLineFormats(doc);
	propertiesPalette->startArrow->rebuildList(&doc->arrowStyles);
	propertiesPalette->endArrow->rebuildList(&doc->arrowStyles);
	FontSub->RebuildList(doc);
	propertiesPalette->Fonts->RebuildList(doc);
	layerPalette->setLayers(&doc->Layers, doc->activeLayer());
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	doc->currentParaStyle = 0;
	slotChangeUnit(doc->unitIndex(), false);
	if (doc->EditClip)
	{
		doc->EditClip = !doc->EditClip;
		ToggleFrameEdit();
	}
	scrActions["fileClose"]->setEnabled(true);
	if (doc->masterPageMode())
	{
		scrActions["pageInsert"]->setEnabled(false);
		scrActions["pageDelete"]->setEnabled(false);
		scrActions["pageCopy"]->setEnabled(false);
		scrActions["pageMove"]->setEnabled(false);
		scrActions["pageApplyMasterPage"]->setEnabled(false);
		scrActions["pageCopyToMasterPage"]->setEnabled(false);
		scrActions["editMasterPages"]->setEnabled(false);
		scrActions["fileNew"]->setEnabled(false);
		scrActions["fileSave"]->setEnabled(doc->isModified());
		scrActions["fileOpen"]->setEnabled(false);
		scrActions["fileClose"]->setEnabled(false);
		scrActions["fileRevert"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("FileOpenRecent", false);
		pagePalette->enablePalette(false);
	}
	else
	{
		scrMenuMgr->setMenuEnabled("Page", true);
		scrActions["editMasterPages"]->setEnabled(true);
		scrActions["fileNew"]->setEnabled(true);
		scrActions["fileOpen"]->setEnabled(true);
		scrActions["fileClose"]->setEnabled(true);
		scrActions["fileSave"]->setEnabled(doc->isModified());
		//CB TODO  Huh? Why different to the above?, fileMenu->setItemEnabled(M_FileSave, ActWin->MenuStat[2]);
		scrActions["fileSaveAs"]->setEnabled(ActWin->menuStatus(3));
		scrActions["fileCollect"]->setEnabled(ActWin->menuStatus(3));
		scrActions["fileRevert"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("FileOpenRecent", true);

		bool setter = doc->Pages->count() > 1 ? true : false;
		scrActions["pageDelete"]->setEnabled(setter);
		scrActions["pageMove"]->setEnabled(setter);

		if (doc->isModified())
			slotDocCh(false);
		scrActions["fileSaveAs"]->setEnabled(true);
		scrActions["fileCollect"]->setEnabled(true);
		pagePalette->enablePalette(true);
	}
}

void ScribusMainWindow::HaveNewDoc()
{
	scrActions["filePrint"]->setEnabled(true);
	scrActions["fileSave"]->setEnabled(false);
	scrActions["fileClose"]->setEnabled(true);
	scrActions["fileDocSetup"]->setEnabled(true);
	scrActions["fileRevert"]->setEnabled(false);
	scrActions["fileCollect"]->setEnabled(true);
	scrActions["fileSaveAs"]->setEnabled(true);
	scrMenuMgr->setMenuEnabled("FileExport", true);
	scrActions["fileExportAsEPS"]->setEnabled(true);
	scrActions["fileExportAsPDF"]->setEnabled(true);
	scrActions["pageImport"]->setEnabled(true);
	//scrActions["toolsPreflightVerifier"]->setEnabled(true);

	if ( HaveGS==0 || ScQApp->isWinGUI() )
		scrActions["PrintPreview"]->setEnabled(true);

	if (scrActions["SaveAsDocumentTemplate"])
		scrActions["SaveAsDocumentTemplate"]->setEnabled(true);

	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	scrActions["editPaste"]->setEnabled(!Buffer2.isEmpty());
	scrActions["editCopyContents"]->setEnabled(false);
	scrActions["editPasteContents"]->setEnabled(false);
	scrActions["editSelectAll"]->setEnabled(true);
	scrActions["editDeselectAll"]->setEnabled(false);
	scrActions["editParaStyles"]->setEnabled(true);
	scrActions["editLineStyles"]->setEnabled(true);
	scrActions["editMasterPages"]->setEnabled(true);
	scrActions["editJavascripts"]->setEnabled(true);

	scrMenuMgr->setMenuEnabled("View", true);
	scrActions["viewSnapToGrid"]->setOn(doc->useRaster);
	scrActions["viewSnapToGuides"]->setOn(doc->SnapGuides);

	scrMenuMgr->setMenuEnabled("Insert", true);
	//scrMenuMgr->setMenuEnabled("Windows", true);
	scrMenuMgr->setMenuEnabled("Page", true);
	scrMenuMgr->setMenuEnabled("Extras", true);

	mainToolBar->setEnabled(true);
	pdfToolBar->setEnabled(true);

	bool setter = doc->Pages->count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);

	updateColorMenu();
	//Update palettes
	propertiesPalette->updateColorList();
	propertiesPalette->Cpal->ChooseGrad(0);
	ActWin->setCaption(doc->DocName);
	scrActions["shade100"]->setOn(true);
	propertiesPalette->setDoc(doc);
	propertiesPalette->updateCList();
	pagePalette->setView(view);
	propertiesPalette->Spal->setFormats(doc);
	propertiesPalette->SetLineFormats(doc);
	propertiesPalette->startArrow->rebuildList(&doc->arrowStyles);
	propertiesPalette->endArrow->rebuildList(&doc->arrowStyles);
	layerPalette->setLayers(&doc->Layers, doc->activeLayer());
	outlinePalette->setDoc(doc);
	outlinePalette->BuildTree();
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	doc->currentParaStyle = 0;
	slotChangeUnit(doc->unitIndex());
	buildFontMenu();
	windowsMenuAboutToShow();
	connect(view, SIGNAL(changeUN(int)), this, SLOT(slotChangeUnit(int)));
	connect(view, SIGNAL(changeLA(int)), layerPalette, SLOT(markActiveLayer(int)));
	connect(view->horizRuler, SIGNAL(MarkerMoved(double, double)), this, SLOT(setMousePositionOnStatusBar(double, double)));
	connect(view->horizRuler, SIGNAL(DocChanged(bool)), this, SLOT(slotDocCh(bool)));
	connect(view, SIGNAL(ClipPo(double, double)), nodePalette, SLOT(SetXY(double, double)));
	connect(view, SIGNAL(PolyOpen()), nodePalette, SLOT(IsOpen()));
	connect(view, SIGNAL(PStatus(int, uint)), nodePalette, SLOT(PolyStatus(int, uint)));
	connect(view, SIGNAL(ItemPos(double, double)), propertiesPalette, SLOT(setXY(double, double)));
	connect(view, SIGNAL(ItemGeom(double, double)), propertiesPalette, SLOT(setBH(double, double)));
	connect(view, SIGNAL(ChBMText(PageItem *)), this, SLOT(BookMarkTxT(PageItem *)));
	connect(view, SIGNAL(NewBMNr(int, int)), bookmarkPalette->BView, SLOT(ChangeItem(int, int)));
	connect(view, SIGNAL(HaveSel(int)), this, SLOT(HaveNewSel(int)));
	connect(view, SIGNAL(SetAngle(double)), propertiesPalette, SLOT(setR(double)));
	connect(view, SIGNAL(SetSizeValue(double)), propertiesPalette, SLOT(setSvalue(double)));
	connect(view, SIGNAL(SetLocalValues(double, double, double, double)), propertiesPalette, SLOT(setLvalue(double, double, double, double)));
	connect(view, SIGNAL(SetLineArt(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle)), propertiesPalette, SLOT( setLIvalue(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle)));
	connect(view, SIGNAL(ItemFarben(QString, QString, int, int)), this, SLOT(setCSMenu(QString, QString, int, int)));
	connect(view, SIGNAL(ItemFarben(QString, QString, int, int)), propertiesPalette->Cpal, SLOT(setActFarben(QString, QString, int, int)));
	connect(view, SIGNAL(ItemGradient(int)), propertiesPalette->Cpal, SLOT(setActGradient(int)));
	connect(view, SIGNAL(ItemTrans(double, double)), propertiesPalette->Cpal, SLOT(setActTrans(double, double)));
	connect(view, SIGNAL(ItemTextAttr(double)), propertiesPalette, SLOT(setLsp(double)));
	connect(view, SIGNAL(ItemTextUSval(int)), propertiesPalette, SLOT(setExtra(int)));
//	connect(view, SIGNAL(ItemTextCols(int, double)), propertiesPalette, SLOT(setCols(int, double)));
	connect(view, SIGNAL(SetDistValues(double, double, double, double)), propertiesPalette, SLOT(setDvals(double, double, double, double)));
	connect(view, SIGNAL(ItemTextAbs(int)), propertiesPalette, SLOT(setAli(int)));
	connect(view, SIGNAL(ItemTextFont(QString)), this, SLOT(AdjustFontMenu(QString)));
	connect(view, SIGNAL(ItemTextFont(QString)), propertiesPalette, SLOT(setFontFace(QString)));
	connect(view, SIGNAL(ItemTextSize(int)), propertiesPalette, SLOT(setSize(int)));
	//connect(view, SIGNAL(ItemRadius(double)), propertiesPalette, SLOT(setRR(double)));
	connect(view, SIGNAL(Amode(int)), this, SLOT(setAppMode(int)));
	connect(view, SIGNAL(PaintingDone()), this, SLOT(slotSelect()));
	connect(view, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	//connect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
	connect(view, SIGNAL(MousePos(double, double)), this, SLOT(setMousePositionOnStatusBar(double, double)));
	//connect(view, SIGNAL(ItemRadius(double)), propertiesPalette, SLOT(setRR(double)));
	connect(view, SIGNAL(ItemTextStil(int)), propertiesPalette, SLOT(setStil(int)));
	connect(view, SIGNAL(ItemTextSca(int)), propertiesPalette, SLOT(setTScale(int)));
	connect(view, SIGNAL(ItemTextScaV(int)), propertiesPalette, SLOT(setTScaleV(int)));
	connect(view, SIGNAL(ItemTextBase(int)), propertiesPalette, SLOT(setTBase(int)));
	connect(view, SIGNAL(ItemTextShadow(int, int )), propertiesPalette, SLOT(setShadowOffs(int, int )));
	connect(view, SIGNAL(ItemTextUnderline(int, int)), propertiesPalette, SLOT(setUnderline(int, int)));
	connect(view, SIGNAL(ItemTextStrike(int, int)), propertiesPalette, SLOT(setStrike(int, int)));
	connect(view, SIGNAL(ItemTextOutline(int)), propertiesPalette, SLOT(setOutlineW(int)));
	connect(view, SIGNAL(ItemTextSize(int)), this, SLOT(setFSizeMenu(int)));
	connect(view, SIGNAL(ItemTextStil(int)), this, SLOT(setStilvalue(int)));
	connect(view, SIGNAL(ItemTextAbs(int)), this, SLOT(setAbsValue(int)));
	connect(view, SIGNAL(ItemTextFarben(QString, QString, int, int)), propertiesPalette, SLOT(setActFarben(QString, QString, int, int)));
	connect(view, SIGNAL(HasTextSel()), this, SLOT(EnableTxEdit()));
	connect(view, SIGNAL(HasNoTextSel()), this, SLOT(DisableTxEdit()));
	connect(view, SIGNAL(CopyItem()), this, SLOT(slotEditCopy()));
	connect(view, SIGNAL(CutItem()), this, SLOT(slotEditCut()));
	connect(view, SIGNAL(LoadPic()), this, SLOT(slotGetContent()));
	connect(view, SIGNAL(AppendText()), this, SLOT(slotFileAppend()));
	connect(view, SIGNAL(AnnotProps()), this, SLOT(ModifyAnnot()));
	connect(view, SIGNAL(EditGuides()), this, SLOT(ManageGuides()));
	connect(view, SIGNAL(LoadElem(QString, double ,double, bool, bool, ScribusDoc *, ScribusView*)), this, SLOT(slotElemRead(QString, double, double, bool, bool, ScribusDoc *, ScribusView*)));
	connect(view, SIGNAL(AddBM(PageItem *)), this, SLOT(AddBookMark(PageItem *)));
	connect(view, SIGNAL(DelBM(PageItem *)), this, SLOT(DelBookMark(PageItem *)));
	connect(view, SIGNAL(RasterPic(bool)), this, SLOT(HaveRaster(bool)));
	connect(view, SIGNAL(DoGroup()), this, SLOT(GroupObj()));
	connect(view, SIGNAL(EndNodeEdit()), this, SLOT(ToggleFrameEdit()));
	connect(view, SIGNAL(LevelChanged(uint )), propertiesPalette, SLOT(setLevel(uint)));
	connect(view, SIGNAL(callGimp()), this, SLOT(callImageEditor()));
	connect(view, SIGNAL(UpdtObj(uint, uint)), outlinePalette, SLOT(slotUpdateElement(uint, uint)));
	connect(view, SIGNAL(AddObj(PageItem *)), outlinePalette, SLOT(slotAddElement(PageItem *)));
/*	if (!doc->masterPageMode)
	{
		connect(doc->currentPage, SIGNAL(DelObj(uint, uint)), outlinePalette, SLOT(slotRemoveElement(uint, uint)));
		connect(doc->currentPage, SIGNAL(AddObj(uint, uint)), outlinePalette, SLOT(slotAddElement(uint, uint)));
		connect(doc->currentPage, SIGNAL(UpdtObj(uint, uint)), outlinePalette, SLOT(slotUpdateElement(uint, uint)));
		connect(doc->currentPage, SIGNAL(MoveObj(uint, uint, uint)), outlinePalette, SLOT(slotMoveElement(uint, uint, uint)));
	} */
}

void ScribusMainWindow::HaveNewSel(int Nr)
{
	PageItem *currItem = NULL;
	if (Nr != -1)
	{
		if (doc->selection->count() != 0)
		{
			currItem = doc->selection->itemAt(0);
			if (!currItem)
				Nr=-1;
		}
		else
			Nr = -1;
	}
	actionManager->disconnectNewSelectionActions();
	scrActions["editDeselectAll"]->setEnabled(Nr!=-1);
	scrActions["itemDetachTextFromPath"]->setEnabled(false);
	scrActions["insertGlyph"]->setEnabled(false);
	scrActions["itemImageIsVisible"]->setEnabled(Nr==PageItem::ImageFrame);
	scrActions["itemUpdateImage"]->setEnabled(Nr==PageItem::ImageFrame && currItem->PicAvail);
	scrActions["itemAdjustFrameToImage"]->setEnabled(Nr==PageItem::ImageFrame && currItem->PicAvail && !currItem->isTableItem);
	scrActions["itemExtendedImageProperties"]->setEnabled(Nr==PageItem::ImageFrame && currItem->PicAvail && currItem->pixm.imgInfo.valid);
	scrActions["itemPreviewLow"]->setEnabled(Nr==PageItem::ImageFrame);
	scrActions["itemPreviewNormal"]->setEnabled(Nr==PageItem::ImageFrame);
	scrActions["itemPreviewFull"]->setEnabled(Nr==PageItem::ImageFrame);
	scrActions["styleImageEffects"]->setEnabled(Nr==PageItem::ImageFrame);
	scrActions["editCopyContents"]->setEnabled(Nr==PageItem::ImageFrame);
	scrActions["editPasteContents"]->setEnabled(Nr==PageItem::ImageFrame);
	scrActions["editEditWithImageEditor"]->setEnabled(Nr==PageItem::ImageFrame && currItem->PicAvail && currItem->isRaster);
	if (Nr!=PageItem::ImageFrame)
	{
		scrActions["itemImageIsVisible"]->setOn(false);
		scrActions["itemPreviewLow"]->setOn(false);
		scrActions["itemPreviewNormal"]->setOn(false);
		scrActions["itemPreviewFull"]->setOn(false);
	}
	if ((Nr==-1) || (Nr!=-1 && !currItem->asTextFrame()))
		actionManager->enableUnicodeActions(false);
	scrActions["insertSampleText"]->setEnabled(false);

	view->horizRuler->ItemPosValid = false;
	view->horizRuler->repaint();
	switch (Nr)
	{
	case -1: // None
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(false);
		scrActions["fileExportText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("Item", false);
		scrMenuMgr->setMenuEnabled("ItemShapes", false);
		scrMenuMgr->setMenuEnabled("ItemConvertTo", false);
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(false);
		scrActions["itemConvertToOutlines"]->setEnabled(false);
		scrActions["itemConvertToPolygon"]->setEnabled(false);
		scrActions["itemConvertToTextFrame"]->setEnabled(false);
		scrActions["itemLock"]->setEnabled(false);
		scrActions["itemLockSize"]->setEnabled(false);
		scrActions["editCut"]->setEnabled(false);
		scrActions["editCopy"]->setEnabled(false);
		scrActions["editCopyContents"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);
		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);

		scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		scrActions["toolsLinkTextFrame"]->setEnabled(false);
		scrActions["toolsEditContents"]->setEnabled(false);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
		scrActions["toolsRotate"]->setEnabled(false);
		scrActions["toolsCopyProperties"]->setEnabled(false);
		//CB 061005 moved to cpalette choosegrad
		//propertiesPalette->Cpal->gradientQCombo->setCurrentItem(0);
		outlinePalette->slotShowSelect(doc->currentPageNumber(), -1);
		break;
	case PageItem::ImageFrame: //Image Frame
		scrActions["fileImportAppendText"]->setEnabled(false);
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportImage"]->setEnabled(true);
		scrActions["editCut"]->setEnabled(true);
		scrActions["editCopy"]->setEnabled(true);
		scrActions["editClearContents"]->setEnabled(true);
		scrActions["editSearchReplace"]->setEnabled(false);
		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("Item", true);
		scrMenuMgr->setMenuEnabled("ItemShapes", !(currItem->isTableItem && currItem->isSingleSel));
		scrMenuMgr->setMenuEnabled("ItemConvertTo", true);
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(false);
		scrActions["itemConvertToOutlines"]->setEnabled(false);
		scrActions["itemConvertToPolygon"]->setEnabled(!currItem->isTableItem && doc->appMode != modeEdit);
		scrActions["itemConvertToTextFrame"]->setEnabled(doc->appMode != modeEdit);
		scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		scrActions["toolsLinkTextFrame"]->setEnabled(false);
		scrActions["toolsEditContents"]->setEnabled(currItem->ScaleType);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
		scrActions["toolsRotate"]->setEnabled(true);
		scrActions["toolsCopyProperties"]->setEnabled(true);
		scrActions["itemImageIsVisible"]->setOn(currItem->imageShown());
		scrActions["itemPreviewLow"]->setOn(currItem->pixm.imgInfo.lowResType==scrActions["itemPreviewLow"]->actionInt());
		scrActions["itemPreviewNormal"]->setOn(currItem->pixm.imgInfo.lowResType==scrActions["itemPreviewNormal"]->actionInt());
		scrActions["itemPreviewFull"]->setOn(currItem->pixm.imgInfo.lowResType==scrActions["itemPreviewFull"]->actionInt());

		break;
	case PageItem::TextFrame: //Text Frame
		scrActions["fileImportText"]->setEnabled(true);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(true);
		scrActions["fileExportText"]->setEnabled(true);
		scrActions["editCut"]->setEnabled(true);
		scrActions["editCopy"]->setEnabled(true);
		scrActions["editClearContents"]->setEnabled(true);
		scrActions["editSearchReplace"]->setEnabled(currItem->itemText.count() != 0);
		scrActions["extrasHyphenateText"]->setEnabled(true);
		scrActions["extrasDeHyphenateText"]->setEnabled(true);
		scrMenuMgr->setMenuEnabled("Item", true);
		scrMenuMgr->setMenuEnabled("ItemShapes", !(currItem->isTableItem && currItem->isSingleSel));
		scrMenuMgr->setMenuEnabled("ItemConvertTo", true);
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(doc->appMode != modeEdit);
		scrActions["itemConvertToOutlines"]->setEnabled(!currItem->isTableItem && doc->appMode != modeEdit);
		scrActions["itemConvertToPolygon"]->setEnabled(!currItem->isTableItem && doc->appMode != modeEdit);
		scrActions["itemConvertToTextFrame"]->setEnabled(false);

		scrActions["toolsRotate"]->setEnabled(true);
		scrActions["toolsCopyProperties"]->setEnabled(true);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(true);
		scrActions["insertSampleText"]->setEnabled(true);
		if ((currItem->NextBox != 0) || (currItem->BackBox != 0))
		{
			scrActions["toolsUnlinkTextFrame"]->setEnabled(true);
			if ((currItem->BackBox != 0) && (currItem->itemText.count() == 0))
				scrActions["toolsEditContents"]->setEnabled(false);
			else
				scrActions["toolsEditContents"]->setEnabled(true);
		}
		else
		{
			scrActions["toolsEditContents"]->setEnabled(true);
			scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		}
		if (currItem->NextBox == 0)
			scrActions["toolsLinkTextFrame"]->setEnabled(true);
		if (doc->masterPageMode())
			scrActions["toolsLinkTextFrame"]->setEnabled(false);
		if (doc->appMode == modeEdit)
		{
			setTBvals(currItem);
			scrActions["editSelectAll"]->setEnabled(true);
			scrActions["insertGlyph"]->setEnabled(true);
			if (currItem->asTextFrame())
				actionManager->enableUnicodeActions(true);
			view->horizRuler->setItemPosition(currItem->xPos(), currItem->width());
			if (currItem->lineColor() != CommonStrings::None)
				view->horizRuler->lineCorr = currItem->lineWidth() / 2.0;
			else
				view->horizRuler->lineCorr = 0;
			view->horizRuler->ColGap = currItem->ColGap;
			view->horizRuler->Cols = currItem->Cols;
			view->horizRuler->Extra = currItem->textToFrameDistLeft();
			view->horizRuler->RExtra = currItem->textToFrameDistRight();
			view->horizRuler->First = doc->docParagraphStyles[doc->currentParaStyle].First;
			view->horizRuler->Indent = doc->docParagraphStyles[doc->currentParaStyle].Indent;
			if (currItem->imageFlippedH() || (currItem->reversed()))
				view->horizRuler->Revers = true;
			else
				view->horizRuler->Revers = false;
			view->horizRuler->ItemPosValid = true;
			if (doc->currentParaStyle < 5)
				view->horizRuler->TabValues = currItem->TabValues;
			else
				view->horizRuler->TabValues = doc->docParagraphStyles[doc->currentParaStyle].TabValues;
			view->horizRuler->repaint();
		}
		else
		{
			doc->CurrFont = currItem->font();
			doc->CurrFontSize = currItem->fontSize();
			doc->CurrTextFill = currItem->TxtFill;
			doc->CurrTextStroke = currItem->TxtStroke;
			doc->CurrTextStrokeSh = currItem->ShTxtStroke;
			doc->CurrTextFillSh = currItem->ShTxtFill;
			doc->CurrTextScale = currItem->TxtScale;
			doc->CurrTextScaleV = currItem->TxtScaleV;
			doc->CurrTextBase = currItem->TxtBase;
			doc->CurrTextShadowX = currItem->TxtShadowX;
			doc->CurrTextShadowY = currItem->TxtShadowY;
			doc->CurrTextOutline = currItem->TxtOutline;
			doc->CurrTextUnderPos = currItem->TxtUnderPos;
			doc->CurrTextUnderWidth = currItem->TxtUnderWidth;
			doc->CurrTextStrikePos = currItem->TxtStrikePos;
			doc->CurrTextStrikeWidth = currItem->TxtStrikeWidth;
			emit TextStrike(doc->CurrTextStrikePos, doc->CurrTextStrikeWidth);
			emit TextUnderline(doc->CurrTextUnderPos, doc->CurrTextUnderWidth);
			emit TextShadow(doc->CurrTextShadowX, doc->CurrTextShadowY);
			emit TextFarben(doc->CurrTextStroke, doc->CurrTextFill, doc->CurrTextStrokeSh, doc->CurrTextFillSh);
			doc->CurrentStyle = currItem->TxTStyle;
			emit TextStil(doc->CurrentStyle);
			emit TextScale(doc->CurrTextScale);
			emit TextScaleV(doc->CurrTextScaleV);
			emit TextBase(doc->CurrTextBase);
			emit TextOutline(doc->CurrTextOutline);
			setStilvalue(doc->CurrentStyle);
		}
		doc->docParagraphStyles[0].LineSpaMode = currItem->lineSpacingMode();
		doc->docParagraphStyles[0].LineSpa = currItem->lineSpacing();
		doc->docParagraphStyles[0].textAlignment = currItem->textAlignment;
		break;
	case PageItem::PathText: //Path Text
		scrActions["fileImportText"]->setEnabled(true);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(true);
		scrActions["fileExportText"]->setEnabled(true);
		scrActions["editCut"]->setEnabled(true);
		scrActions["editCopy"]->setEnabled(true);
		scrActions["editClearContents"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);
		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("Item", true);
		scrMenuMgr->setMenuEnabled("ItemShapes", false);
		scrActions["itemDetachTextFromPath"]->setEnabled(true);
		scrMenuMgr->setMenuEnabled("ItemConvertTo", true);
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(false);
		scrActions["itemConvertToOutlines"]->setEnabled(true);
		scrActions["itemConvertToPolygon"]->setEnabled(false);
		scrActions["itemConvertToTextFrame"]->setEnabled(false);

		scrActions["toolsRotate"]->setEnabled(true);
		scrActions["toolsCopyProperties"]->setEnabled(true);
		scrActions["toolsEditContents"]->setEnabled(false);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(true);
		scrActions["toolsLinkTextFrame"]->setEnabled(false);
		scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		if (doc->appMode == modeEdit)
			setTBvals(currItem);
		else
		{
			doc->CurrFont = currItem->font();
			doc->CurrFontSize = currItem->fontSize();
			doc->CurrTextFill = currItem->TxtFill;
			doc->CurrTextStroke = currItem->TxtStroke;
			doc->CurrTextStrokeSh = currItem->ShTxtStroke;
			doc->CurrTextFillSh = currItem->ShTxtFill;
			doc->CurrTextScale = currItem->TxtScale;
			doc->CurrTextScaleV = currItem->TxtScaleV;
			doc->CurrTextBase = currItem->TxtBase;
			doc->CurrTextShadowX = currItem->TxtShadowX;
			doc->CurrTextShadowY = currItem->TxtShadowY;
			doc->CurrTextOutline = currItem->TxtOutline;
			doc->CurrTextUnderPos = currItem->TxtUnderPos;
			doc->CurrTextUnderWidth = currItem->TxtUnderWidth;
			doc->CurrTextStrikePos = currItem->TxtStrikePos;
			doc->CurrTextStrikeWidth = currItem->TxtStrikeWidth;
			emit TextStrike(doc->CurrTextStrikePos, doc->CurrTextStrikeWidth);
			emit TextUnderline(doc->CurrTextUnderPos, doc->CurrTextUnderWidth);
			emit TextShadow(doc->CurrTextShadowX, doc->CurrTextShadowY);
			emit TextFarben(doc->CurrTextStroke, doc->CurrTextFill, doc->CurrTextStrokeSh, doc->CurrTextFillSh);
			doc->CurrentStyle = currItem->TxTStyle;
			emit TextStil(doc->CurrentStyle);
			emit TextScale(doc->CurrTextScale);
			emit TextScaleV(doc->CurrTextScaleV);
			emit TextBase(doc->CurrTextBase);
			emit TextOutline(doc->CurrTextOutline);
			setStilvalue(doc->CurrentStyle);
		}
		break;
	default:
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(false);
		scrActions["fileExportText"]->setEnabled(false);
		scrActions["editCut"]->setEnabled(true);
		scrActions["editCopy"]->setEnabled(true);
		scrActions["editClearContents"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);

		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("Item", true);
		if (Nr == 6) //Polygon
		{
			scrMenuMgr->setMenuEnabled("ItemShapes", true);
			scrMenuMgr->setMenuEnabled("ItemConvertTo", true);
			scrActions["itemConvertToBezierCurve"]->setEnabled(doc->appMode != modeEdit);
			scrActions["itemConvertToImageFrame"]->setEnabled(doc->appMode != modeEdit);
			scrActions["itemConvertToOutlines"]->setEnabled(false);
			scrActions["itemConvertToPolygon"]->setEnabled(false);
			scrActions["itemConvertToTextFrame"]->setEnabled(doc->appMode != modeEdit);
		}
		scrActions["toolsEditContents"]->setEnabled(false);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
		scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		scrActions["toolsLinkTextFrame"]->setEnabled(false);
		if (Nr != 5)
			scrActions["toolsRotate"]->setEnabled(true);
		else
			scrActions["toolsRotate"]->setEnabled(false);
		scrActions["toolsCopyProperties"]->setEnabled(true);
		break;
	}
	doc->CurrentSel = Nr;
	rebuildStyleMenu(Nr);
	propertiesPalette->RotationGroup->setButton(doc->RotMode);
	uint docSelectionCount=doc->selection->count();
	if (docSelectionCount > 1)
	{
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(false);
		//scrActions["itemConvertToOutlines"]->setEnabled(false);
		scrActions["itemConvertToPolygon"]->setEnabled(false);
		scrActions["itemConvertToTextFrame"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);

		bool hPoly = true;
		bool isGroup = true;
		int firstElem = -1;
		if (currItem->Groups.count() != 0)
			firstElem = currItem->Groups.top();
		for (uint bx=0; bx<docSelectionCount; ++bx)
		{
			PageItem* bxi=doc->selection->itemAt(bx);
			if (!bxi->asPolygon())
				hPoly = false;
			if (bxi->Groups.count() != 0)
			{
				if (bxi->Groups.top() != firstElem)
					isGroup = false;
			}
			else
				isGroup = false;
		}
		scrActions["itemGroup"]->setEnabled(!isGroup);
		scrActions["itemCombinePolygons"]->setEnabled(hPoly);
		if (docSelectionCount == 2)
		{
			PageItem* bx=doc->selection->itemAt(1);
			if (((currItem->itemType() == PageItem::TextFrame) || (bx->itemType() == PageItem::TextFrame)) && ((currItem->itemType() == PageItem::PolyLine) || (bx->itemType() == PageItem::PolyLine)))
			{
				if ((currItem->NextBox == 0) && (currItem->BackBox == 0) && (bx->NextBox == 0) && (bx->BackBox == 0) && (currItem->Groups.count() == 0))
					scrActions["itemAttachTextToPath"]->setEnabled(true);
			}

		}
	}
	else
	{
		scrActions["itemGroup"]->setEnabled(false);
		scrActions["itemAttachTextToPath"]->setEnabled(false);
		scrActions["itemCombinePolygons"]->setEnabled(false);
	}
	if (docSelectionCount != 0)
	{
		actionManager->setPDFActions(view);
		updateItemLayerList();
		propertiesPalette->textFlowsAroundFrame->setChecked(currItem->textFlowsAroundFrame());
		scrActions["itemLock"]->setEnabled(true);
		scrActions["itemLockSize"]->setEnabled(true);
		if (currItem->Groups.count() != 0)
			scrActions["itemUngroup"]->setEnabled(true);
		else
		{
			scrActions["itemUngroup"]->setEnabled(false);
			scrActions["itemSplitPolygons"]->setEnabled( (currItem->asPolygon()) && (currItem->Segments.count() != 0) );
		}
		if (currItem->locked())
		{
			scrMenuMgr->setMenuEnabled("ItemShapes", false);
			scrMenuMgr->setMenuEnabled("ItemConvertTo", false);
			scrActions["itemConvertToBezierCurve"]->setEnabled(false);
			scrActions["itemConvertToImageFrame"]->setEnabled(false);
			scrActions["itemConvertToOutlines"]->setEnabled(false);
			scrActions["itemConvertToPolygon"]->setEnabled(false);
			scrActions["itemConvertToTextFrame"]->setEnabled(false);
			scrActions["itemSplitPolygons"]->setEnabled(false);
			scrActions["itemAttachTextToPath"]->setEnabled(false);
			scrActions["itemDetachTextFromPath"]->setEnabled(false);
			scrActions["itemCombinePolygons"]->setEnabled(false);
			scrActions["itemDelete"]->setEnabled(false);
			scrActions["itemLowerToBottom"]->setEnabled(false);
			scrActions["itemRaiseToTop"]->setEnabled(false);
			scrActions["itemRaise"]->setEnabled(false);
			scrActions["itemLower"]->setEnabled(false);
			scrActions["itemSendToScrapbook"]->setEnabled(false);
			scrActions["editCut"]->setEnabled(false);
			scrActions["editClearContents"]->setEnabled(false);
			scrActions["toolsRotate"]->setEnabled(false);
		}
		else
		{
			bool setter=!(currItem->isTableItem && currItem->isSingleSel);
			scrActions["itemDuplicate"]->setEnabled(setter);
			scrActions["itemMulDuplicate"]->setEnabled(setter);
			scrActions["itemDelete"]->setEnabled(setter);
			scrActions["itemLowerToBottom"]->setEnabled(setter);
			scrActions["itemRaiseToTop"]->setEnabled(setter);
			scrActions["itemRaise"]->setEnabled(setter);
			scrActions["itemLower"]->setEnabled(setter);
			scrActions["itemSendToScrapbook"]->setEnabled(setter);
		}
		scrActions["itemLock"]->setOn(currItem->locked());
		scrActions["itemLockSize"]->setOn(currItem->sizeLocked());
	}

	//propertiesPalette->NewSel(Nr);
	if (Nr != -1)
	{
		//propertiesPalette->SetCurItem(currItem);
		outlinePalette->slotShowSelect(currItem->OwnPage, currItem->ItemNr);
		if (currItem->FrameType == 0)
			SCustom->setPixmap(SCustom->getIconPixmap(0));
		if (currItem->FrameType == 1)
			SCustom->setPixmap(SCustom->getIconPixmap(1));
		if (currItem->FrameType > 3)
			SCustom->setPixmap(SCustom->getIconPixmap(currItem->FrameType-2));
		actionManager->connectNewSelectionActions(view);
	}
	else
		propertiesPalette->NewSel(Nr);
}

void ScribusMainWindow::rebuildStyleMenu(int itemType)
{
	scrMenuMgr->clearMenu("Style");
	int iT=itemType;
	if (!HaveDoc)
		iT=-1;
	if (iT != -1)
	{
		if (doc->selection->count() == 0)
			iT = -1;
		else
		{
			PageItem *currItem = doc->selection->itemAt(0);
			if (!currItem)
				iT=-1;
		}
	}
	if (iT==PageItem::TextFrame || iT==PageItem::PathText)
	{
		scrMenuMgr->addMenuToMenu("Font","Style");
		scrMenuMgr->addMenuToMenu("FontSize","Style");
		scrMenuMgr->addMenuToMenu("TypeEffects","Style");
		scrMenuMgr->addMenuToMenu("Alignment","Style");
		scrMenuMgr->addMenuToMenu("Color","Style");
		scrMenuMgr->addMenuToMenu("Shade","Style");
		if (itemType==PageItem::TextFrame)
			scrMenuMgr->addMenuItem(scrActions["styleTabulators"], "Style");
		scrMenuMgr->setMenuEnabled("Style", true);
	}
	else
		scrMenuMgr->setMenuEnabled("Style", false);
}

void ScribusMainWindow::slotDocCh(bool /*reb*/)
{
/*	if ((reb) && (!doc->masterPageMode) && (view->SelItem.count() != 0))
	{
		for (uint upd = 0; upd < view->SelItem.count(); ++upd)
			outlinePalette->slotUpdateElement(doc->currentPage->PageNr, view->SelItem.at(upd)->ItemNr);
	} */
	if (!doc->isLoading() && docCheckerPalette->isVisible())
	{
		scanDocument();
		docCheckerPalette->buildErrorList(doc);
	}
	if (!doc->isModified())
		doc->setModified(true);
	ActWin->setCaption( doc->DocName + "*");
	scrActions["fileSave"]->setEnabled(true);
	scrActions["fileSaveAs"]->setEnabled(true);
	scrActions["fileCollect"]->setEnabled(true);
	if (!doc->masterPageMode())
	{
		scrActions["fileClose"]->setEnabled(true);
		if (doc->hasName)
			scrActions["fileRevert"]->setEnabled(true);
	}

	ActWin->setMenuStatus(0, scrActions["fileSave"]->isEnabled());
	ActWin->setMenuStatus(1, scrActions["fileClose"]->isEnabled());
	ActWin->setMenuStatus(2, scrActions["fileSave"]->isEnabled());
	ActWin->setMenuStatus(3, scrActions["fileSaveAs"]->isEnabled());
}

void ScribusMainWindow::updateRecent(QString fn)
{
	if (RecentDocs.findIndex(fn) == -1)
	{
		RecentDocs.prepend(fn);
		fileWatcher->addFile(fn);
	}
	else
	{
		RecentDocs.remove(fn);
		RecentDocs.prepend(fn);
	}
	rebuildRecentFileMenu();
}

void ScribusMainWindow::removeRecent(QString fn)
{
	if (RecentDocs.findIndex(fn) != -1)
	{
		RecentDocs.remove(fn);
		if (!fileWatcher->isActive())
			fileWatcher->removeFile(fn);
	}
	rebuildRecentFileMenu();
}

void ScribusMainWindow::loadRecent(QString fn)
{
	QFileInfo fd(fn);
	if (!fd.exists())
	{
		RecentDocs.remove(fn);
		fileWatcher->removeFile(fn);
		rebuildRecentFileMenu();
		return;
	}
	loadDoc(fn);
}

void ScribusMainWindow::rebuildRecentFileMenu()
{
	for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrRecentFileActions.begin(); it!=scrRecentFileActions.end(); ++it )
		scrMenuMgr->removeMenuItem((*it), recentFileMenuName);

	scrRecentFileActions.clear();
	uint max = QMIN(prefsManager->appPrefs.RecentDCount, RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		QString strippedName=RecentDocs[m];
		strippedName.remove(QDir::separator());
		QString localName(QDir::convertSeparators(RecentDocs[m]));
		scrRecentFileActions.insert(strippedName, new ScrAction(ScrAction::RecentFile, QIconSet(), QString("&%1 %2").arg(m+1).arg(localName), QKeySequence(), this, strippedName,0,0.0,RecentDocs[m]));
		connect( scrRecentFileActions[strippedName], SIGNAL(activatedData(QString)), this, SLOT(loadRecent(QString)) );
		scrMenuMgr->addMenuItem(scrRecentFileActions[strippedName], recentFileMenuName);
	}
}

void ScribusMainWindow::rebuildLayersList()
{
	if (HaveDoc)
	{
		for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it0 = scrLayersActions.begin(); it0 != scrLayersActions.end(); ++it0 )
			scrMenuMgr->removeMenuItem((*it0), layerMenuName);
		scrLayersActions.clear();
		QValueList<Layer>::iterator it;
		if (doc->Layers.count()!= 0)
		{
			for (it = doc->Layers.begin(); it != doc->Layers.end(); ++it)
			{
				scrLayersActions.insert(QString("%1").arg((*it).LNr), new ScrAction( ScrAction::Layer, QIconSet(), (*it).Name, QKeySequence(), this, (*it).Name, (*it).LNr));
				scrLayersActions[QString("%1").arg((*it).LNr)]->setToggleAction(true);
			}
		}
		int currActiveLayer=doc->activeLayer();
		bool found=false;
		for (it = doc->Layers.begin(); it != doc->Layers.end(); ++it)
		{
			if ((*it).LNr == currActiveLayer)
			{
				found=true;
				break;
			}
		}
		Q_ASSERT(found);
		scrLayersActions[QString("%1").arg((*it).LNr)]->setOn(true);

		for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=scrLayersActions.end(); ++it )
		{
			scrMenuMgr->addMenuItem((*it), layerMenuName);
			connect( (*it), SIGNAL(activatedData(int)), this, SLOT(sendToLayer(int)) );
		}
	}
}

void ScribusMainWindow::updateItemLayerList()
{
	if (HaveDoc)
	{
		QMap<QString, QGuardedPtr<ScrAction> >::Iterator itend=scrLayersActions.end();
		for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=itend; ++it )
		{
			disconnect( (*it), SIGNAL(activatedData(int)), 0, 0 );
			(*it)->setOn(false);
		}
		if (doc->selection->count()>0 && doc->selection->itemAt(0))
			scrLayersActions[QString("%1").arg(doc->selection->itemAt(0)->LayerNr)]->setOn(true);
		for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=itend; ++it )
			connect( (*it), SIGNAL(activatedData(int)), this, SLOT(sendToLayer(int)) );
	}
}

void ScribusMainWindow::sendToLayer(int layerNumber)
{
	if (HaveDoc)
	{
		uint docSelectionCount=doc->selection->count();
		if (docSelectionCount != 0)
		{
			if (UndoManager::undoEnabled() && docSelectionCount > 1)
				undoManager->beginTransaction();
			QString tooltip = Um::ItemsInvolved + "\n";
			for (uint a = 0; a < docSelectionCount; ++a)
			{
				PageItem *currItem = doc->selection->itemAt(a);
				currItem->setLayer(layerNumber);
				tooltip += "\t" + currItem->getUName() + "\n";
			}
			if (UndoManager::undoEnabled() && docSelectionCount > 1)
				undoManager->commit(Um::Selection,
									Um::IGroup,
									Um::SendToLayer,
									tooltip,
									Um::ILayerAction);
		}

		view->Deselect(true);
		view->updateContents();
		slotDocCh();
	}
}

bool ScribusMainWindow::slotDocOpen()
{
	PrefsContext* docContext = prefsManager->prefsFile->getContext("docdirs", false);
	QString docDir = ".";
	QString prefsDocDir=prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		docDir = docContext->get("docsopen", prefsDocDir);
	else
		docDir = docContext->get("docsopen", ".");
	QString formats(FileLoader::getLoadFilterString());
	QString fileName = CFileDialog( docDir, tr("Open"), formats);
	if (fileName.isEmpty())
		// User cancelled
		return false;
	docContext->set("docsopen", fileName.left(fileName.findRev("/")));
	bool ret = loadDoc(fileName);
	return ret;
}

bool ScribusMainWindow::slotPageImport()
{
	Q_ASSERT(!doc->masterPageMode());
	bool ret = false;
	MergeDoc *dia = new MergeDoc(this, false, doc->DocPages.count(), doc->currentPage->pageNr() + 1);
	if (dia->exec())
	{
		mainWindowStatusLabel->setText( tr("Importing Pages..."));
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		std::vector<int> pageNs;
		parsePagesString(dia->getPageNumbers(), &pageNs, dia->getPageCounter());
		int startPage, nrToImport;
		bool doIt = true;
		if (doc->masterPageMode())
		{
			if (pageNs.size() > 1)
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
			addNewPages(dia->getImportWherePage(), importWhere, pageNs.size(), doc->pageHeight, doc->pageWidth, doc->PageOri, doc->PageSize, true);
			nrToImport = pageNs.size();
		}
		else
		{
			startPage = doc->currentPage->pageNr() + 1;
			nrToImport = pageNs.size();
			if (pageNs.size() > (doc->DocPages.count() - doc->currentPage->pageNr()))
			{
				qApp->setOverrideCursor(QCursor(arrowCursor), true);
				int scmReturn=ScMessageBox::information(this, tr("Import Page(s)"), "<qt>" +
				QObject::tr("<p>You are trying to import more pages than there are available in the current document counting from the active page.</p>Choose one of the following:<br>"
				"<ul><li><b>Create</b> missing pages</li>"
				"<li><b>Import</b> pages until the last page</li>"
				"<li><b>Cancel</b></li></ul>") + "</qt>",
				QObject::tr("C&reate"),
				QObject::tr("&Import"),
				CommonStrings::tr_Cancel, 2, 2);
				switch( scmReturn )
				{
					case 0:
						nrToImport = pageNs.size();
						addNewPages(doc->DocPages.count(), 2, pageNs.size() - (doc->DocPages.count() - doc->currentPage->pageNr()), doc->pageHeight, doc->pageWidth, doc->PageOri, doc->PageSize, true);
						break;
					case 1:
						nrToImport = doc->DocPages.count() - doc->currentPage->pageNr();
						break;
					case 2:
						doIt = false;
						mainWindowStatusLabel->setText("");
						break;
				}
				qApp->setOverrideCursor(QCursor(waitCursor), true);
			}
		}
		if (doIt)
		{
			if (nrToImport > 0)
			{
				mainWindowProgressBar->reset();
				mainWindowProgressBar->setTotalSteps(nrToImport);
				int counter = startPage;
				for (int i = 0; i < nrToImport; ++i)
				{
					view->GotoPa(counter);
					loadPage(dia->getFromDoc(), pageNs[i] - 1, false);
					counter++;
					mainWindowProgressBar->setProgress(i + 1);
				}
				view->GotoPa(startPage);
				mainWindowProgressBar->reset();
				mainWindowStatusLabel->setText( tr("Import done"));
			}
			else
			{
				mainWindowStatusLabel->setText( tr("Found nothing to import"));
				doIt = false;
			}
		}
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		ret = doIt;
	}
	delete dia;
	return ret;
}

bool ScribusMainWindow::loadPage(QString fileName, int Nr, bool Mpa)
{
	bool ret = false;
	if (!fileName.isEmpty())
	{
		if (!Mpa)
			doc->OpenNodes = outlinePalette->buildReopenVals();
		FileLoader *fl = new FileLoader(fileName);
		if (fl->TestFile() == -1)
		{
			delete fl;
			return false;
		}
		doc->setLoading(true);
		uint oldItemsCount = doc->Items->count();
		if(!fl->LoadPage(Nr, Mpa))
		{
			delete fl;
			doc->setLoading(false);
			return false;
		}
		delete fl;
		if (CMSavail && doc->CMSSettings.CMSinUse)
		{
			recalcColors();
			doc->RecalcPictures(&InputProfiles, &InputProfilesCMYK);
		}
		uint docItemsCount=doc->Items->count();
		for (uint i = oldItemsCount; i < docItemsCount; ++i)
		{
			PageItem *ite = doc->Items->at(i);
			if ((ite->asTextFrame()) && (ite->isBookmark))
				bookmarkPalette->BView->AddPageItem(ite);
		}
		propertiesPalette->updateColorList();
		propertiesPalette->updateCList();
		propertiesPalette->Spal->setFormats(doc);
		propertiesPalette->SetLineFormats(doc);
		propertiesPalette->startArrow->rebuildList(&doc->arrowStyles);
		propertiesPalette->endArrow->rebuildList(&doc->arrowStyles);
		if (!Mpa)
		{
			outlinePalette->BuildTree();
			outlinePalette->reopenTree(doc->OpenNodes);
			scanDocument();
			docCheckerPalette->buildErrorList(doc);
		}
		slotDocCh();
		rebuildLayersList();
		view->updateLayerMenu();
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
	undoManager->setUndoEnabled(false);
	QFileInfo fi(fileName);
	if (!fi.exists())
		return false;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
/*	if (HaveDoc)
		doc->OpenNodes = outlinePalette->buildReopenVals(); */
	bool ret = false;
	QWidgetList windows = wsp->windowList();
	ScribusWin* ActWinOld = NULL;
	if (windows.count() != 0)
		ActWinOld = ActWin;
	bool found = false;
	uint id = 0;
	uint windowCount=windows.count();
	for ( uint i = 0; i < windowCount; ++i )
	{
		if (windows.at(i)->caption() == fileName)
		{
			found = true;
			id = i;
			break;
		}
	}
	if (found)
	{
		windowsMenuActivated(id);
		return true;
	}
	if (!fileName.isEmpty())
	{
		QString FName = fi.absFilePath();
		QString DPath = fi.dirPath(true);
		// Necessary on win32 platform for setCurrent() to succeed
		// if document is located at drive root
		if( !DPath.endsWith("/") )
			DPath += "/";
		QDir::setCurrent(DPath);
		FileLoader *fileLoader = new FileLoader(FName);
		if (fileLoader->TestFile() == -1)
		{
			delete fileLoader;
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			QMessageBox::critical(this, tr("Fatal Error"), "<qt>"+tr("File %1 is not in an acceptable format").arg(FName)+"</qt>", CommonStrings::tr_OK);
			return false;
		}
		bool is12doc=false;
		if (fileLoader->TestFile() == 0)
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			//Scribus 1.3.x warning, remove at a later stage
			is12doc=true;
		}

		prefsManager->appPrefs.AvailFonts.AddScalableFonts(fi.dirPath(true)+"/", FName);
		prefsManager->appPrefs.AvailFonts.updateFontMap();
		doc=new ScribusDoc();
		doc->is12doc=is12doc;
		doc->appMode = modeNormal;
		doc->HasCMS = false;
		//doc->setActiveLayer(0); //CB should not need this, the file load process sets it to ALAYER from the doc
		doc->OpenNodes.clear();
		doc->setLoading(true);
		mainWindowStatusLabel->setText( tr("Loading..."));
		mainWindowProgressBar->reset();
		ScribusWin* w = new ScribusWin(wsp, doc);
		view = new ScribusView(w, doc);
		view->setScale(prefsManager->displayScale());
		w->setView(view);
		alignDistributePalette->setView(view);
		ActWin = w;
		doc->WinHan = w;
		w->setCentralWidget(view);
		w->setUpdatesEnabled(false);
		view->setUpdatesEnabled(false);
#ifdef HAVE_CMS
		doc->SoftProofing = false;
		doc->Gamut = false;
		bool cmsu = CMSuse;
		CMSuse = false;
#endif
		ScMW->ScriptRunning = true;
		bool loadSuccess=fileLoader->LoadFile();
		//Do the font replacement check from here, when we have a GUI. TODO do this also somehow without the GUI
		//This also gives the user the opportunity to cancel the load when finding theres a replacement required.
		if (loadSuccess && ScQApp->usingGUI())
			loadSuccess=fileLoader->postLoad(is12doc);
		if(!loadSuccess)
		{
			view->close();
			delete fileLoader;
			delete view;
			delete doc;
			delete w;
			ScMW->ScriptRunning = false;
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			mainWindowStatusLabel->setText("");
			mainWindowProgressBar->reset();
			ActWin = NULL;
			if (windows.count() != 0)
				newActWin(ActWinOld);
			return false;
		}
		fileLoader->informReplacementFonts();
		view->unitSwitcher->setCurrentText(unitGetStrFromIndex(doc->unitIndex()));
		view->unitChange();
		ScMW->ScriptRunning = false;
		view->Deselect(true);
		mainWindowStatusLabel->setText("");
		mainWindowProgressBar->reset();
#ifdef HAVE_CMS
		CMSuse = cmsu;
#endif
		HaveDoc++;
		if (doc->checkerProfiles.count() == 0)
		{
			prefsManager->initDefaultCheckerPrefs(&doc->checkerProfiles);
			doc->curCheckProfile = tr("PostScript");
		}
		if (doc->PDF_Options.LPISettings.count() == 0)
		{
			struct LPIData lpo;
			lpo.Frequency = 75;
			lpo.SpotFunc = 2;
			lpo.Angle = 105;
			doc->PDF_Options.LPISettings.insert("Cyan", lpo);
			lpo.Angle = 75;
			doc->PDF_Options.LPISettings.insert("Magenta", lpo);
			lpo.Angle = 90;
			doc->PDF_Options.LPISettings.insert("Yellow", lpo);
			lpo.Angle = 45;
			doc->PDF_Options.LPISettings.insert("Black", lpo);
		}
		//connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
		if (!doc->HasCMS)
		{
			doc->CMSSettings.DefaultImageRGBProfile = prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile;
			doc->CMSSettings.DefaultImageCMYKProfile = prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile;
			doc->CMSSettings.DefaultSolidColorProfile = prefsManager->appPrefs.DCMSset.DefaultSolidColorProfile;
			doc->CMSSettings.DefaultMonitorProfile = prefsManager->appPrefs.DCMSset.DefaultMonitorProfile;
			doc->CMSSettings.DefaultPrinterProfile = prefsManager->appPrefs.DCMSset.DefaultPrinterProfile;
			doc->CMSSettings.DefaultIntentPrinter = prefsManager->appPrefs.DCMSset.DefaultIntentPrinter;
			doc->CMSSettings.DefaultIntentMonitor = prefsManager->appPrefs.DCMSset.DefaultIntentMonitor;
			doc->CMSSettings.DefaultIntentImages = prefsManager->appPrefs.DCMSset.DefaultIntentImages;
			doc->CMSSettings.SoftProofOn = prefsManager->appPrefs.DCMSset.SoftProofOn;
			doc->CMSSettings.GamutCheck = prefsManager->appPrefs.DCMSset.GamutCheck;
			doc->CMSSettings.BlackPoint = prefsManager->appPrefs.DCMSset.BlackPoint;
			doc->CMSSettings.CMSinUse = false;
		}
		if ((CMSavail) && (doc->CMSSettings.CMSinUse))
		{
			bool cmsWarning = false;
			QStringList missing;
			QStringList replacement;
			if (!InputProfiles.contains(doc->CMSSettings.DefaultImageRGBProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultImageRGBProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile);
				doc->CMSSettings.DefaultImageRGBProfile = prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile;
			}
			if (!InputProfiles.contains(doc->CMSSettings.DefaultSolidColorProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultSolidColorProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultSolidColorProfile);
				doc->CMSSettings.DefaultSolidColorProfile = prefsManager->appPrefs.DCMSset.DefaultSolidColorProfile;
			}
			if (!MonitorProfiles.contains(doc->CMSSettings.DefaultMonitorProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultMonitorProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultMonitorProfile);
				doc->CMSSettings.DefaultMonitorProfile = prefsManager->appPrefs.DCMSset.DefaultMonitorProfile;
			}
			if (!PrinterProfiles.contains(doc->CMSSettings.DefaultPrinterProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultPrinterProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultPrinterProfile);
				doc->CMSSettings.DefaultPrinterProfile = prefsManager->appPrefs.DCMSset.DefaultPrinterProfile;
			}
			if (!PrinterProfiles.contains(doc->PDF_Options.PrintProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Options.PrintProf);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultPrinterProfile);
				doc->PDF_Options.PrintProf = doc->CMSSettings.DefaultPrinterProfile;
			}
			if (!InputProfiles.contains(doc->PDF_Options.ImageProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Options.ImageProf);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile);
				doc->PDF_Options.ImageProf = doc->CMSSettings.DefaultImageRGBProfile;
			}
			if (!InputProfiles.contains(doc->PDF_Options.SolidProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Options.SolidProf);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultSolidColorProfile);
				doc->PDF_Options.SolidProf = doc->CMSSettings.DefaultSolidColorProfile;
			}
			if ((cmsWarning) && (doc->HasCMS))
			{
				qApp->setOverrideCursor(QCursor(arrowCursor), true);
				QString mess = tr("Some ICC profiles used by this document are not installed:")+"\n\n";
				for (uint m = 0; m < missing.count(); ++m)
				{
					mess += missing[m] + tr(" was replaced by: ")+replacement[m]+"\n";
				}
				QMessageBox::warning(this, CommonStrings::trWarning, mess, 1, 0, 0);
			}
#ifdef HAVE_CMS
			doc->SoftProofing = doc->CMSSettings.SoftProofOn;
			doc->Gamut = doc->CMSSettings.GamutCheck;
			CMSuse = doc->CMSSettings.CMSinUse;
			doc->IntentPrinter = doc->CMSSettings.DefaultIntentPrinter;
			doc->IntentMonitor = doc->CMSSettings.DefaultIntentMonitor;
			SoftProofing = doc->CMSSettings.SoftProofOn;
			Gamut = doc->CMSSettings.GamutCheck;
			IntentPrinter = doc->CMSSettings.DefaultIntentPrinter;
			IntentMonitor = doc->CMSSettings.DefaultIntentMonitor;
			doc->OpenCMSProfiles(InputProfiles, MonitorProfiles, PrinterProfiles);
			CMSuse = doc->CMSSettings.CMSinUse;
			stdProofG = doc->stdProof;
			stdTransG = doc->stdTrans;
			stdProofImgG = doc->stdProofImg;
			stdTransImgG = doc->stdTransImg;
			stdProofCMYKG = doc->stdProofCMYK;
			stdTransCMYKG = doc->stdTransCMYK;
			stdTransRGBG = doc->stdTransRGB;
			stdProofGCG = doc->stdProofGC;
			stdProofCMYKGCG = doc->stdProofCMYKGC;
			CMSoutputProf = doc->DocOutputProf;
			CMSprinterProf = doc->DocPrinterProf;
			if (static_cast<int>(cmsGetColorSpace(doc->DocInputProf)) == icSigRgbData)
				doc->CMSSettings.ComponentsInput2 = 3;
			if (static_cast<int>(cmsGetColorSpace(doc->DocInputProf)) == icSigCmykData)
				doc->CMSSettings.ComponentsInput2 = 4;
			if (static_cast<int>(cmsGetColorSpace(doc->DocInputProf)) == icSigCmyData)
				doc->CMSSettings.ComponentsInput2 = 3;
			if (static_cast<int>(cmsGetColorSpace(doc->DocPrinterProf)) == icSigRgbData)
				doc->CMSSettings.ComponentsPrinter = 3;
			if (static_cast<int>(cmsGetColorSpace(doc->DocPrinterProf)) == icSigCmykData)
				doc->CMSSettings.ComponentsPrinter = 4;
			if (static_cast<int>(cmsGetColorSpace(doc->DocPrinterProf)) == icSigCmyData)
				doc->CMSSettings.ComponentsPrinter = 3;
			doc->PDF_Options.SComp = doc->CMSSettings.ComponentsInput2;
#endif
			if (doc->CMSSettings.CMSinUse)
			{
				recalcColors();
				doc->RecalcPictures(&InputProfiles, &InputProfilesCMYK);
			}
		}
		else
		{
			doc->CMSSettings.CMSinUse = false;
#ifdef HAVE_CMS
			CMSuse = doc->CMSSettings.CMSinUse;
#endif
		}
		propertiesPalette->updateColorList();
		propertiesPalette->Cpal->ChooseGrad(0);
		if (fileLoader->FileType > 1)
		{
			doc->setName(FName+tr("(converted)"));
			QFileInfo fi(doc->DocName);
			doc->setName(fi.fileName());
		}
		else
			doc->setName(FName);
		doc->setMasterPageMode(false);
		doc->Language = GetLang(doc->Language);
		HaveNewDoc();
		propertiesPalette->updateCList();
		doc->hasName = true;
		if (doc->MasterPages.count() == 0)
			doc->addMasterPage(0, "Normal");
		//Add doc sections if we have none
		if (doc->sections.count()==0)
		{
			doc->addSection(-1);
			doc->setFirstSectionFromFirstPageNumber();
		}
		doc->setLoading(false);
		doc->RePos = true;
		QPixmap pgPix(10, 10);
		QRect rd = QRect(0,0,9,9);
		ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
		doc->setMasterPageMode(true);
		for (uint azz=0; azz<doc->MasterItems.count(); ++azz)
		{
			PageItem *ite = doc->MasterItems.at(azz);
			// TODO fix that for Groups on Masterpages
//			if (ite->Groups.count() != 0)
//				view->GroupOnPage(ite);
			if ((ite->asTextFrame()) || (ite->asPathText()))
			{
				if (ite->asPathText())
				{
					ite->Frame = true;
					ite->UpdatePolyClip();
				}
				else
				{
					if ((ite->BackBox != 0) || (ite->NextBox != 0))
					{
						PageItem *nextItem = ite;
						while (nextItem != 0)
						{
							if (nextItem->BackBox != 0)
								nextItem = nextItem->BackBox;
							else
								break;
						}
						ite = nextItem;
					}
				}
				ite->DrawObj(painter, rd);
			}
		}
//		RestoreBookMarks();
		doc->setMasterPageMode(false);
		uint docItemsCount=doc->Items->count();
		for (uint azz=0; azz<docItemsCount; ++azz)
		{
			PageItem *ite = doc->Items->at(azz);
			//CB dont need this as we get it from the loading page in 1.2.x docs. 1.3.x items have this anyway.
			/*
			if (ite->Groups.count() != 0)
				doc->GroupOnPage(ite);
			else
				ite->OwnPage = doc->OnPage(ite);
			*/
			//view->setRedrawBounding(ite);
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText) && (!ite->Redrawn))
			{
				if (ite->itemType() == PageItem::PathText)
				{
					ite->Frame = true;
					ite->UpdatePolyClip();
					ite->DrawObj(painter, rd);
				}
				else
				{
					if ((ite->BackBox != 0) || (ite->NextBox != 0))
					{
						PageItem *nextItem = ite;
						while (nextItem != 0)
						{
							if (nextItem->BackBox != 0)
								nextItem = nextItem->BackBox;
							else
								break;
						}
						ite = nextItem;
						ite->DrawObj(painter, rd);
					}
					else
						ite->DrawObj(painter, rd);
				}
			}
/*			if (doc->OldBM)
			{
				if ((ite->itemType() == PageItem::TextFrame) && (ite->isBookmark))
					bookmarkPalette->BView->AddPageItem(ite);
			}
			else
			{
				if ((ite->itemType() == PageItem::TextFrame) && (ite->isBookmark))
					bookmarkPalette->BView->ChangeItem(ite->BMnr, ite->ItemNr);
			} */
		}
		for (uint azz=0; azz<doc->FrameItems.count(); ++azz)
		{
			PageItem *ite = doc->FrameItems.at(azz);
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
			{
				if (ite->itemType() == PageItem::PathText)
				{
					ite->Frame = true;
					ite->UpdatePolyClip();
				}
				ite->DrawObj(painter, rd);
			}
		}
		delete painter;
//		if (doc->OldBM)
//			StoreBookmarks();
		doc->RePos = false;
		doc->setModified(false);
		updateRecent(FName);
		mainWindowStatusLabel->setText( tr("Ready"));
		ret = true;
		for (uint p = 0; p < doc->DocPages.count(); ++p)
		{
			Apply_MasterPage(doc->DocPages.at(p)->MPageNam, p, false);
		}
		if (fileLoader->FileType > 1)
		{
			doc->hasName = false;
			slotFileSaveAs();
		}
		delete fileLoader;
		view->setUpdatesEnabled(true);
		w->setUpdatesEnabled(true);
		disconnect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
		if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
			w->showMaximized();
		else
			w->show();
		view->show();
		newActWin(w);
		doc->currentPage=doc->DocPages.at(0);
		view->slotDoZoom();
		view->GotoPage(0);
		connect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
		connect(doc->autoSaveTimer, SIGNAL(timeout()), w, SLOT(slotAutoSave()));
		connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
		connect(fileWatcher, SIGNAL(fileChanged(QString )), view, SLOT(updatePict(QString)));
		connect(fileWatcher, SIGNAL(fileDeleted(QString )), view, SLOT(removePict(QString)));
		connect(doc, SIGNAL(refreshItem(PageItem*)), view, SLOT(RefreshItem(PageItem*)));
		connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
		if (doc->AutoSave)
			doc->autoSaveTimer->start(doc->AutoSaveTime);
		scrActions["fileSave"]->setEnabled(false);
		doc->NrItems = bookmarkPalette->BView->NrItems;
		doc->First = bookmarkPalette->BView->First;
		doc->Last = bookmarkPalette->BView->Last;
	}
	else
	{
		pagePalette->setView(0);
	}
	undoManager->switchStack(doc->DocName);
	pagePalette->Rebuild();
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	undoManager->setUndoEnabled(true);
	return ret;
}

bool ScribusMainWindow::postLoadDoc()
{
	//FIXME Just return for now, if we arent using the GUI
	if (!ScQApp->usingGUI())
		return false;
	return true;
}

// This method was once named slotFileOpen(...) but it hasn't had anything to
// do with file->open for a LONG time. It's used for get text / get picture.
void ScribusMainWindow::slotGetContent()
{
	if (doc->selection->count() != 0)
	{
		PageItem *currItem = doc->selection->itemAt(0);
		if (currItem->itemType() == PageItem::ImageFrame)
		{
			QString formats = "";
			QString formatD = tr("All Supported Formats")+" (";
			QString form1 = "";
			QString form2 = "";
			for ( uint i = 0; i < QImageIO::inputFormats().count(); ++i )
			{
				form1 = QString(QImageIO::inputFormats().at(i)).lower();
				form2 = QString(QImageIO::inputFormats().at(i)).upper();
				if (form1 == "jpeg")
				{
					form1 = "jpg";
					form2 = "JPG";
				}
				if ((form1 == "png") || (form1 == "xpm") || (form1 == "gif"))
				{
					formats += form2 + " (*."+form1+" *."+form2+");;";
					formatD += "*."+form1+" *."+form2+" ";
				}
				else if (form1 == "jpg")
				{
					// JPEG is a special case because both .jpg and .jpeg
					// are acceptable extensions.
					formats += "JPEG (*.jpg *.jpeg *.JPG *.JPEG);;";
					formatD += "*.jpg *.jpeg *.JPG *.JPEG ";
				}
			}
#ifdef HAVE_TIFF
			formats += "TIFF (*.tif *.tiff *.TIF *TIFF);;";
			formatD += "*.tif *.tiff *.TIF *TIFF";
#endif
			formats += "PSD (*.psd *.PSD);;";
			formatD += " *.psd *.PSD";
			formats += "EPS (*.eps *.EPS);;PDF (*.pdf *.PDF);;" + tr("All Files (*)");
			formatD += " *.eps *.EPS *.pdf *.PDF";
			formatD += ");;"+formats;
			QString docDir = ".";
			QString prefsDocDir=prefsManager->documentDir();
			if (!prefsDocDir.isEmpty())
				docDir = dirs->get("images", prefsDocDir);
			else
				docDir = dirs->get("images", ".");
			QString fileName = CFileDialog( docDir, tr("Open"), formatD, "", true);
			if (!fileName.isEmpty())
			{
				dirs->set("images", fileName.left(fileName.findRev("/")));
				currItem->EmProfile = "";
				currItem->pixm.imgInfo.isRequest = false;
				currItem->UseEmbedded = true;
				currItem->IProfile = doc->CMSSettings.DefaultImageRGBProfile;
				currItem->IRender = doc->CMSSettings.DefaultIntentImages;
				qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				doc->LoadPict(fileName, currItem->ItemNr);
				//view->AdjustPictScale(currItem, false);
				//false was ignored anyway
				currItem->AdjustPictScale();
				propertiesPalette->setLvalue(currItem->imageXScale(), currItem->imageYScale(), currItem->imageXOffset(), currItem->imageYOffset());
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				qApp->restoreOverrideCursor();
				view->DrawNew();
				propertiesPalette->updateColorList();
				propertiesPalette->updateCList();
				propertiesPalette->ShowCMS();
			}
		}
		if (currItem->asTextFrame())
		{
			gtGetText* gt = new gtGetText();
			gt->run(false);
			delete gt;
			if (doc->docHyphenator->AutoCheck)
				doc->docHyphenator->slotHyphenate(currItem);
			for (uint a = 0; a < doc->Items->count(); ++a)
			{
				doc->Items->at(a)->ItemNr = a;
				if (doc->Items->at(a)->isBookmark)
					bookmarkPalette->BView->ChangeItem(doc->Items->at(a)->BMnr, a);
			}
			outlinePalette->BuildTree();
			view->DrawNew();
			slotDocCh();
		}
	}
}

/*!
 \fn void slotFileAppend()
 \author Franz Schmid
 \date
 \brief Appends a Textfile to the Text in the selected Textframe at the Cursorposition
 \param None
 \retval None
 */
void ScribusMainWindow::slotFileAppend()
{
	if (doc->selection->count() != 0)
	{
		gtGetText* gt = new gtGetText();
		gt->run(true);
		delete gt;
		if (doc->docHyphenator->AutoCheck)
			doc->docHyphenator->slotHyphenate(doc->selection->itemAt(0));
		view->DrawNew();
		slotDocCh();
	}
}

void ScribusMainWindow::slotFileRevert()
{
	if ((doc->hasName) && (doc->isModified()) && (!doc->masterPageMode()))
	{
		int t = ScMessageBox::warning(this, CommonStrings::trWarning, "<qt>" +
								 QObject::tr("The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?") + "</qt>",
								 QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
		if (t == QMessageBox::No)
			return;

		QString fn(doc->DocName);
		QFileInfo fi(fn);
		QDir::setCurrent(fi.dirPath(true));
		doc->setModified(false);
		if (doc==storyEditor->currentDocument())
			storyEditor->close();
		slotFileClose();
		qApp->processEvents();
		loadDoc(fn);
	}
}

void ScribusMainWindow::slotAutoSaved()
{
	if (ActWin == sender())
	{
		scrActions["fileSave"]->setEnabled(false);
		ActWin->setCaption(doc->DocName);
	}
}

bool ScribusMainWindow::slotFileSave()
{
	bool ret = false;
	if (doc->hasName)
	{
		//Scribus 1.3.x warning, remove at a later stage
		if (doc->is12doc && !warningVersion(this))
				return false;

		QString fn(doc->DocName);
		ret = DoFileSave(fn);
		if (!ret)
			QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg(fn), CommonStrings::tr_OK);
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
	//
	bool ret = false;
	QString fna;
	PrefsContext* docContext = prefsManager->prefsFile->getContext("docdirs", false);
	QString wdir = ".";
	if (doc->hasName)
	{
		QFileInfo fi(doc->DocName);
		wdir = fi.dirPath();
		fna = fi.dirPath()+"/"+fi.baseName()+".sla";
	}
	else
	{
		QString prefsDocDir=prefsManager->documentDir();
		if (!prefsDocDir.isEmpty())
			wdir = docContext->get("save_as", prefsDocDir);
		else
			wdir = docContext->get("save_as", ".");
		if (wdir.right(1) != "/")
			fna = wdir + "/";
		else
			fna = wdir;
		fna += doc->DocName + ".sla";
	}
#ifdef HAVE_LIBZ
	QString fileSpec=tr("Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)");
	bool setter=true;
#else
	QString fileSpec=tr("Documents (*.sla *.scd);;All Files (*)");
	bool setter=false;
#endif
	QString fn = CFileDialog( wdir, tr("Save As"), fileSpec, fna, false, false, setter);
	if (!fn.isEmpty())
	{
		docContext->set("save_as", fn.left(fn.findRev("/")));
		if ((fn.endsWith(".sla")) || (fn.endsWith(".sla.gz")))
			fna = fn;
		else
			fna = fn+".sla";
		if (overwrite(this, fna))
		{
			ret = DoFileSave(fna);
			if (!ret)
				QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg(fn), CommonStrings::tr_OK);
			else
				doc->PDF_Options.Datei = ""; // #1482 reset the pdf file name
		}
	}
	mainWindowStatusLabel->setText( tr("Ready"));
	return ret;
}

bool ScribusMainWindow::DoFileSave(QString fn)
{
	fileWatcher->forceScan();
	fileWatcher->stop();
	ReorgFonts();
	mainWindowStatusLabel->setText( tr("Saving..."));
	mainWindowProgressBar->reset();
	bool ret=doc->save(fn);
	qApp->processEvents();
	if (ret)
	{
		ActWin->setCaption(fn);
		undoManager->renameStack(fn);
		scrActions["fileSave"]->setEnabled(false);
		scrActions["fileRevert"]->setEnabled(false);
		updateRecent(fn);
	}
	mainWindowStatusLabel->setText("");
	mainWindowProgressBar->reset();
	fileWatcher->start();
	return ret;
}

bool ScribusMainWindow::slotFileClose()
{
	ScribusWin* tw = ActWin;
	singleClose = false;
	ActWin->close();
	if (tw == ActWin)
		return false;
	else
		return true;
	windowsMenuAboutToShow();
}

bool ScribusMainWindow::DoFileClose()
{
	if (doc==storyEditor->currentDocument())
		storyEditor->close();
	actionManager->disconnectNewViewActions();
	disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	if (doc->viewCount > 1)
	{
		--doc->viewCount;
		closeActiveWindowMasterPageEditor();
		setAppMode(modeNormal);
		disconnect(fileWatcher, SIGNAL(fileChanged(QString )), view, SLOT(updatePict(QString)));
		disconnect(fileWatcher, SIGNAL(fileDeleted(QString )), view, SLOT(removePict(QString)));
		view->close();
		delete view;
		view = NULL;
		doc = NULL;
		ActWin = NULL;
		return true;
	}
	undoManager->removeStack(doc->DocName);
	closeActiveWindowMasterPageEditor();
	setAppMode(modeNormal);
	doc->autoSaveTimer->stop();
	disconnect(doc->autoSaveTimer, SIGNAL(timeout()), doc->WinHan, SLOT(slotAutoSave()));
	disconnect(doc->WinHan, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	disconnect(fileWatcher, SIGNAL(fileChanged(QString )), view, SLOT(updatePict(QString)));
	disconnect(fileWatcher, SIGNAL(fileDeleted(QString )), view, SLOT(removePict(QString)));
	for (uint a = 0; a < doc->Items->count(); ++a)
	{
		PageItem *currItem = doc->Items->at(a);
		if (currItem->PicAvail)
			fileWatcher->removeFile(currItem->Pfile);
	}
	if (CMSavail)
		doc->CloseCMSProfiles();
	//<<Palettes
//	propertiesPalette->NewSel(-1);
	propertiesPalette->unsetDoc();
	pagePalette->setView(0);
	pagePalette->Rebuild();
	propertiesPalette->Spal->setFormats(0);
	propertiesPalette->SetLineFormats(0);
	if (doc->EditClip)
		nodePalette->doc = 0;
	bookmarkPalette->BView->clear();
	bookmarkPalette->BView->NrItems = 0;
	bookmarkPalette->BView->First = 1;
	bookmarkPalette->BView->Last = 0;
	outlinePalette->unsetDoc();
	alignDistributePalette->setView(NULL);
	//>>
	if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
	{
#ifdef HAVE_CMS
		CMSuse = false;
		SoftProofing = prefsManager->appPrefs.DCMSset.SoftProofOn;
		IntentPrinter = prefsManager->appPrefs.DCMSset.DefaultIntentPrinter;
		IntentMonitor = prefsManager->appPrefs.DCMSset.DefaultIntentMonitor;
#endif
		scrActions["fileDocSetup"]->setEnabled(false);
		scrActions["filePrint"]->setEnabled(false);
		scrActions["fileSave"]->setEnabled(false);
		scrActions["fileSaveAs"]->setEnabled(false);
		scrActions["fileRevert"]->setEnabled(false);
		scrActions["fileCollect"]->setEnabled(false);
		scrActions["fileClose"]->setEnabled(false);
		scrActions["PrintPreview"]->setEnabled(false);
		if (scrActions["SaveAsDocumentTemplate"])
			scrActions["SaveAsDocumentTemplate"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("FileExport", false);
		scrActions["fileExportAsPDF"]->setEnabled(false);
		scrActions["fileExportText"]->setEnabled(false);
		scrActions["fileExportAsEPS"]->setEnabled(false);
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(false);
		scrActions["pageImport"]->setEnabled(false);

		scrActions["editUndoAction"]->setEnabled(false);
		scrActions["editRedoAction"]->setEnabled(false);
		scrActions["editCut"]->setEnabled(false);
		scrActions["editCopy"]->setEnabled(false);
		scrActions["editPaste"]->setEnabled(false);
		scrActions["editClearContents"]->setEnabled(false);
		scrActions["editSelectAll"]->setEnabled(false);
		scrActions["editDeselectAll"]->setEnabled(false);
		scrActions["editParaStyles"]->setEnabled(false);
		scrActions["editLineStyles"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);
		scrActions["editMasterPages"]->setEnabled(false);
		scrActions["editJavascripts"]->setEnabled(false);

		//scrActions["toolsPreflightVerifier"]->setEnabled(false);

		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("View", false);
		//scrMenuMgr->setMenuEnabled("Windows", false);
		scrActions["viewSnapToGuides"]->setOn(false);
		scrActions["viewSnapToGrid"]->setOn(false);

		scrMenuMgr->setMenuEnabled("Insert", false);
		scrMenuMgr->setMenuEnabled("Page", false);
		scrMenuMgr->setMenuEnabled("Extras", false);
		scrMenuMgr->setMenuEnabled("Style", false);
		scrMenuMgr->setMenuEnabled("Item", false);
		mainToolBar->setEnabled(false);
		pdfToolBar->setEnabled(false);
		ColorMenC->clear();
		//CB dont need this until we have a doc...
		//propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
		propertiesPalette->Cpal->ChooseGrad(0);
		mainWindowStatusLabel->setText( tr("Ready"));
		PrinterUsed = false;
	}
	QString fName(view->Doc->DocName);
	view->close();
	delete view;
	doc->setLoading(true);
	layerPalette->ClearInhalt();
	docCheckerPalette->buildErrorList(0);
	fileWatcher->removeFile(fName);
	HaveDoc--;
	view = NULL;
	delete doc;
	doc = NULL;
	ActWin = NULL;
	tocGenerator->setDoc(doc);
	if ( HaveDoc == 0 )
	{
		QString prefsDocDir( PrefsManager::instance()->documentDir() );
		if ( QDir().exists(prefsDocDir) )
			QDir::setCurrent( PrefsManager::instance()->documentDir() );
		else
			QDir::setCurrent( QDir::homeDirPath() );
	}
	return true;
}

void ScribusMainWindow::slotFilePrint()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		scanDocument();
		if ((doc->docItemErrors.count() != 0) || (doc->masterItemErrors.count() != 0))
		{
			if (doc->checkerProfiles[doc->curCheckProfile].ignoreErrors)
			{
				int t = ScMessageBox::warning(this, CommonStrings::trWarning,
											"<qt>"+tr("Scribus has detected some errors. Consider using the Preflight Verifier to correct them")+"</qt>",
											tr("&Ignore"), tr("&Abort"), 0, 0, 0);
				if (t == 1)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(slotReallyPrint()));
				docCheckerPalette->setIgnoreEnabled(true);
				docCheckerPalette->checkMode = CheckDocument::checkPrint;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setOn(true);
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
		scrActions["toolsPreflightVerifier"]->setOn(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(slotReallyPrint()));
	}
	PrintOptions options;
	mainWindowStatusLabel->setText( tr("Printing..."));
	if (PrinterUsed)
	{
		options.printer = PDef.Pname;
		options.filename = PDef.Dname;
	}
	else
	{
		options.printer = "";
		if (!doc->DocName.startsWith( tr("Document")))
		{
			QFileInfo fi(doc->DocName);
			options.filename = fi.dirPath()+"/"+fi.baseName()+".ps";
		}
		else
		{
			QDir di = QDir();
			options.filename = di.currentDirPath()+"/"+doc->DocName+".ps";
		}
	}
	options.copies = 1;
	ColorList usedSpots;
	doc->getUsedColors(usedSpots, true);
	QStringList spots = usedSpots.keys();
	Druck *printer = new Druck(this, options.filename, options.printer, PDef.Command, PDef.DevMode, prefsManager->appPrefs.GCRMode, spots);
	printer->setMinMax(1, doc->Pages->count(), doc->currentPage->pageNr()+1);
	printDinUse = true;
	connect(printer, SIGNAL(doPreview()), this, SLOT(doPrintPreview()));
	if (printer->exec())
	{
		ReOrderText(doc, view);
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		options.printer = printer->printerName();
		options.filename = printer->outputFileName();
		options.toFile = printer->outputToFile();
		if (printer->CurrentPage->isChecked())
			options.pageNumbers.push_back(doc->currentPage->pageNr()+1);
		else
		{
			if (printer->RadioButton1->isChecked())
				parsePagesString("*", &options.pageNumbers, doc->DocPages.count());
			else
				parsePagesString(printer->PageNr->text(), &options.pageNumbers, doc->DocPages.count());
		}
		options.copies = printer->numCopies();
		options.outputSeparations = printer->outputSeparations();
		options.separationName = printer->separationName();
		options.allSeparations = printer->allSeparations();
		if (options.outputSeparations)
			options.useSpotColors = true;
		else
			options.useSpotColors = printer->doSpot();
		options.useColor = printer->color();
		options.mirrorH = printer->mirrorHorizontal();
		options.mirrorV = printer->mirrorVertical();
		options.useICC = printer->ICCinUse();
		options.doGCR = printer->doGCR();
		options.PSLevel = printer->PSLevel();
		options.setDevParam = printer->doDev();
		PDef.Pname = options.printer;
		PDef.Dname = options.filename;
		PDef.DevMode = printer->DevMode;
		if (printer->OtherCom->isChecked())
		{
			PDef.Command = printer->Command->text();
			options.printerCommand = printer->Command->text();
			options.useAltPrintCommand = true;
		}
		else
		{
			options.useAltPrintCommand = false;
		}
#ifdef HAVE_CUPS
		options.printerOptions = printer->PrinterOpts;
#endif
#ifndef HAVE_CUPS
		options.printerOptions = QString("");
#endif
		PrinterUsed = true;
#ifdef _WIN32
		SHORT shiftState = GetKeyState( VK_SHIFT );
		bool forceGDI = ( shiftState & 0x8000 ) ? true : false;
		if (options.toFile == false)
		{
			ScWinPrint winPrint;
			done = winPrint.print( doc, options, printer->DevMode, forceGDI );
		}
		else
			done = doPrint(&options);
#else
		done = doPrint(&options);
#endif
		if (!done)
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			QMessageBox::warning(this, CommonStrings::trWarning, tr("Printing failed!"), CommonStrings::tr_OK);
		}
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	printDinUse = false;
	disconnect(printer, SIGNAL(doPreview()), this, SLOT(doPrintPreview()));
	delete printer;
	mainWindowStatusLabel->setText( tr("Ready"));
}

/*!
 \fn ScribusMainWindow::doPrint()
 \author Franz Schmid
 \date
 \brief Generate and print PostScript from a doc
 \param options PrintOptions struct to control all settings
 \sa ScribusMainWindow::slotFilePrint()
 \retval True for success
 */
bool ScribusMainWindow::doPrint(PrintOptions *options)
{
	bool retw = false;
	QMap<QString,int> ReallyUsed;
	QString filename(options->filename);
	ReallyUsed.clear();
	doc->getUsedFonts(&ReallyUsed);
	ColorList usedColors;
	doc->getUsedColors(usedColors);
	fileWatcher->forceScan();
	fileWatcher->stop();
	PSLib *dd = new PSLib(true, prefsManager->appPrefs.AvailFonts, ReallyUsed, usedColors, false, options->useSpotColors);
	if (dd != NULL)
	{
		if (!options->toFile)
			filename = PrefsPfad+"/tmp.ps";
		else
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			if (!overwrite(this, filename))
			{
				delete dd;
				fileWatcher->start();
				return true;
			}
			qApp->setOverrideCursor(QCursor(waitCursor), true);
		}
		bool PSfile = dd->PS_set_file(filename);
		filename = QDir::convertSeparators(filename);
		if (PSfile)
		{
			// Write the PS to a file
			ScColor::UseProf = options->useICC;
			int psCreationRetVal=dd->CreatePS(doc, options->pageNumbers, options->outputSeparations, options->separationName, options->allSeparations,
			               options->useColor, options->mirrorH, options->mirrorV, options->useICC, options->doGCR, options->setDevParam);
			if (psCreationRetVal!=0)
			{
				unlink(filename);
				if (psCreationRetVal==2)
					return true;
				else
					return false;
			}
			ScColor::UseProf = true;
			if (options->PSLevel != 3)
			{
				// use gs to convert our PS to a lower version
				QString tmp;
				QStringList opts;
				opts.append( QString("-dDEVICEWIDTHPOINTS=%1").arg(tmp.setNum(doc->pageWidth)) );
				opts.append( QString("-dDEVICEHEIGHTPOINTS=%1").arg(tmp.setNum(doc->pageHeight)) );
				convertPS2PS(filename, filename + ".tmp", opts, options->PSLevel);
				moveFile( filename + ".tmp", filename );
			}
			if (!options->toFile)
			{
				// print and delete the PS file
				QString cmd;
				if (options->useAltPrintCommand)
				{
					cmd = options->printerCommand + " "+filename;
					system(cmd);
				}
				else
				{
					QString cc;
					cmd = "lpr -P" + options->printer;
					if (options->copies > 1)
						cmd += " -#" + cc.setNum(options->copies);
					cmd += options->printerOptions;
					cmd += " "+filename;
					system(cmd);
				}
				unlink(filename);
			}
			retw = true;
		}
		else
			retw = false;
		delete dd;
	}
	fileWatcher->start();
	return retw;
}

void ScribusMainWindow::slotFileQuit()
{
	propertiesPalette->unsetDoc();
	pluginManager->savePreferences();
	close();
}

void ScribusMainWindow::slotEditCut()
{
	uint a;
	NoFrameEdit();
	QString BufferI = "";
	uint docSelectionCount=doc->selection->count();
	if ((HaveDoc) && (docSelectionCount != 0))
	{
		PageItem *currItem;
		for (uint i = 0; i < docSelectionCount; ++i)
		{
			currItem=doc->selection->itemAt(i);
			if ((currItem->asTextFrame() || currItem->asPathText()) && currItem==ScMW->storyEditor->currentItem() && doc==ScMW->storyEditor->currentDocument())
			{
					QMessageBox::critical(ScMW, tr("Cannot Cut In-Use Item"), tr("The item %1 is currently being edited by Story Editor. The cut operation will be cancelled").arg(currItem->itemName()), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
					return;
			}
		}
		if (UndoManager::undoEnabled())
		{
			if (docSelectionCount > 1)
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Cut,"",Um::ICut);
			else
			{
				PageItem* item=doc->selection->itemAt(0);
				undoManager->beginTransaction(item->getUName(), item->getUPixmap(), Um::Cut, "", Um::ICut);
			}
		}
		Buffer2 = "<SCRIBUSTEXT>";
		currItem = doc->selection->itemAt(0);
		if (doc->appMode == modeEdit)
		{
			if ((currItem->itemText.count() == 0) || (!currItem->HasSel))
				return;
			PageItem *nextItem = currItem;
			while (nextItem != 0)
			{
				if (nextItem->BackBox != 0)
					nextItem = nextItem->BackBox;
				else
					break;
			}
			while (nextItem != 0)
			{
				for (a = 0; a < nextItem->itemText.count(); ++a)
				{
					if (nextItem->itemText.at(a)->cselect)
					{
						if (nextItem->itemText.at(a)->ch == QChar(13))
						{
							Buffer2 += QChar(5);
							BufferI += QChar(10);
						}
						else if (nextItem->itemText.at(a)->ch == QChar(9))
						{
							Buffer2 += QChar(4);
							BufferI += QChar(9);
						}
						else
						{
							Buffer2 += nextItem->itemText.at(a)->ch;
							BufferI += nextItem->itemText.at(a)->ch;
						}
						Buffer2 += "\t";
						Buffer2 += nextItem->itemText.at(a)->cfont->scName()+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->csize)+"\t";
						Buffer2 += nextItem->itemText.at(a)->ccolor+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cextra)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshade)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cstyle)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cab)+'\t';
						Buffer2 += nextItem->itemText.at(a)->cstroke+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshade2)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cscale)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cscalev)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cbase)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshadowx)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshadowy)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->coutline)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cunderpos)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cunderwidth)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cstrikepos)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cstrikewidth)+'\n';
					}
				}
				dynamic_cast<PageItem_TextFrame*>(nextItem)->deleteSelectedTextFromFrame();
				nextItem = nextItem->NextBox;
			}
			view->RefreshItem(currItem);
		}
		else
		{
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			ScriXmlDoc *ss = new ScriXmlDoc();
			BufferI = ss->WriteElem(doc, view, doc->selection);
			Buffer2 = BufferI;
			view->DeleteItem();
		}
		slotDocCh();
		BuFromApp = true;
		ClipB->setText(BufferI);
		scrActions["editPaste"]->setEnabled(true);
		if (UndoManager::undoEnabled())
			undoManager->commit();
	}
}

void ScribusMainWindow::slotEditCopy()
{
	uint a;
	NoFrameEdit();
	QString BufferI = "";
	if ((HaveDoc) && (doc->selection->count() != 0))
	{
		Buffer2 = "<SCRIBUSTEXT>";
		PageItem *currItem = doc->selection->itemAt(0);
		if ((doc->appMode == modeEdit) && (currItem->HasSel))
		{
			Buffer2 += "";
			PageItem *nextItem = currItem;
			while (nextItem != 0)
			{
				if (nextItem->BackBox != 0)
					nextItem = nextItem->BackBox;
				else
					break;
			}
			while (nextItem != 0)
			{
				for (a = 0; a < nextItem->itemText.count(); ++a)
				{
					if (nextItem->itemText.at(a)->cselect)
					{
						if (nextItem->itemText.at(a)->ch == QChar(13))
						{
							Buffer2 += QChar(5);
							BufferI += QChar(10);
						}
						else if (nextItem->itemText.at(a)->ch == QChar(9))
						{
							Buffer2 += QChar(4);
							BufferI += QChar(9);
						}
						else
						{
							Buffer2 += nextItem->itemText.at(a)->ch;
							BufferI += nextItem->itemText.at(a)->ch;
						}
						Buffer2 += "\t";
						Buffer2 += nextItem->itemText.at(a)->cfont->scName()+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->csize)+"\t";
						Buffer2 += nextItem->itemText.at(a)->ccolor+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cextra)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshade)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cstyle)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cab)+"\t";
						Buffer2 += nextItem->itemText.at(a)->cstroke+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshade2)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cscale)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cscalev)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cbase)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshadowx)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshadowy)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->coutline)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cunderpos)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cunderwidth)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cstrikepos)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cstrikewidth)+'\n';
					}
				}
				nextItem = nextItem->NextBox;
			}
		}
		else
		{
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			ScriXmlDoc *ss = new ScriXmlDoc();
			BufferI = ss->WriteElem(doc, view, doc->selection);
			Buffer2 = BufferI;
			delete ss;
		}
		BuFromApp = true;
		ClipB->setText(BufferI);
		scrActions["editPaste"]->setEnabled(true);
	}
}

void ScribusMainWindow::slotEditPaste()
{
	struct ScText *hg;
	NoFrameEdit();
	if (HaveDoc)
	{
		if (Buffer2.isNull())
			return;
		if (doc->appMode == modeEdit)
		{
			PageItem_TextFrame *currItem = dynamic_cast<PageItem_TextFrame*>(doc->selection->itemAt(0));
			if (currItem->HasSel)
				currItem->deleteSelectedTextFromFrame();
			if (Buffer2.startsWith("<SCRIBUSTEXT>"))
			{
				QString Buf = Buffer2.mid(13);
				QTextStream t(&Buf, IO_ReadOnly);
				QString cc;
				while (!t.atEnd())
				{
					cc = t.readLine();
					QStringList wt;
					QStringList::Iterator it;
					wt = QStringList::split("\t", cc);
					it = wt.begin();
					hg = new ScText;
					hg->ch = (*it);
					if (hg->ch == QChar(5))
						hg->ch = QChar(13);
					if (hg->ch == QChar(4))
						hg->ch = QChar(9);
					it++;
					hg->cfont = (*doc->AllFonts)[*it];
					it++;
					hg->csize = (*it).toInt();
					it++;
					hg->ccolor = *it;
					it++;
					hg->cextra = (*it).toInt();
					it++;
					hg->cshade = (*it).toInt();
					hg->cselect = false;
					it++;
					hg->cstyle = (*it).toInt();
					it++;
					hg->cab = (*it).toInt();
					it++;
					if (it == NULL)
						hg->cstroke = CommonStrings::None;
					else
						hg->cstroke = *it;
					it++;
					if (it == NULL)
						hg->cshade2 = 100;
					else
						hg->cshade2 = (*it).toInt();
					it++;
					if (it == NULL)
						hg->cscale = 1000;
					else
						hg->cscale = (*it).toInt();
					it++;
					if (it == NULL)
						hg->cscalev = 1000;
					else
						hg->cscalev = QMIN(QMAX((*it).toInt(), 100), 4000);
					it++;
					hg->cbase = it == NULL ? 0 : (*it).toInt();
					it++;
					hg->cshadowx = it == NULL ? 50 : (*it).toInt();
					it++;
					hg->cshadowy = it == NULL ? -50 : (*it).toInt();
					it++;
					hg->coutline = it == NULL ? 10 : (*it).toInt();
					it++;
					hg->cunderpos = it == NULL ? -1 : (*it).toInt();
					it++;
					hg->cunderwidth = it == NULL ? -1 : (*it).toInt();
					it++;
					hg->cstrikepos = it == NULL ? -1 : (*it).toInt();
					it++;
					hg->cstrikewidth = it == NULL ? -1 : (*it).toInt();
					currItem->itemText.insert(currItem->CPos, hg);
					currItem->CPos += 1;
					hg->PRot = 0;
					hg->PtransX = 0;
					hg->PtransY = 0;
					hg->cembedded = 0;
				}
			}
			else if (Buffer2.startsWith("<SCRIBUSELEM"))
			{
				bool savedAlignGrid = doc->useRaster;
				bool savedAlignGuides = doc->SnapGuides;
				uint ac = doc->Items->count();
				bool isGroup = false;
				double gx, gy, gh, gw;
				FPoint minSize = doc->minCanvasCoordinate;
				FPoint maxSize = doc->maxCanvasCoordinate;
				doc->useRaster = false;
				doc->SnapGuides = false;
				slotElemRead(Buffer2, 0, 0, false, true, doc, view);
				doc->useRaster = savedAlignGrid;
				doc->SnapGuides = savedAlignGuides;
				//int tempList=doc->selection->backupToTempList(0);
				Selection tempSelection(*ScMW->doc->selection);
				doc->selection->clear();
				if (doc->Items->count() - ac > 1)
					isGroup = true;
				for (uint as = ac; as < doc->Items->count(); ++as)
				{
					doc->selection->addItem(doc->Items->at(as));
					if (isGroup)
						doc->Items->at(as)->Groups.push(doc->GroupCounter);
				}
				if (isGroup)
					doc->GroupCounter++;
				view->setGroupRect();
				view->getGroupRect(&gx, &gy, &gw, &gh);
				PageItem* currItem3 = doc->Items->at(ac);
				for (uint as = ac; as < doc->Items->count(); ++as)
				{
					PageItem* currItem2 = doc->Items->at(as);
					currItem2->isEmbedded = true;
					currItem2->setIsAnnotation(false);
					currItem2->isBookmark = false;
					currItem2->gXpos = currItem2->xPos() - gx;
					currItem2->gYpos = currItem2->yPos() - gy;
					currItem2->gWidth = gw;
					currItem2->gHeight = gh;
					currItem2->ItemNr = doc->FrameItems.count();
					doc->FrameItems.append(currItem2);
				}
				uint acc = doc->Items->count();
				for (uint as = ac; as < acc; ++as)
				{
					doc->Items->take(ac);
				}
				doc->selection->clear();
				//doc->selection->restoreFromTempList(0, tempList);
				*ScMW->doc->selection=tempSelection;
				view->resizeContents(qRound((maxSize.x() - minSize.x()) * view->scale()), qRound((maxSize.y() - minSize.y()) * view->scale()));
				view->scrollBy(qRound((doc->minCanvasCoordinate.x() - minSize.x()) * view->scale()), qRound((doc->minCanvasCoordinate.y() - minSize.y()) * view->scale()));
				doc->minCanvasCoordinate = minSize;
				doc->maxCanvasCoordinate = maxSize;
				outlinePalette->BuildTree();
				outlinePalette->reopenTree(doc->OpenNodes);
				hg = new ScText;
				hg->ch = QChar(25);
				doc->setScTextDefaultsFromDoc(hg);
				hg->cselect = false;
				hg->xp = 0;
				hg->yp = 0;
				hg->PRot = 0;
				hg->PtransX = 0;
				hg->PtransY = 0;
				hg->cextra = 0;
				hg->cembedded = currItem3;
				currItem->itemText.insert(currItem->CPos, hg);
				currItem->CPos += 1;
			}
			else
			{
				Serializer *ss = new Serializer("");
				ss->Objekt = Buffer2;
				int st = doc->currentParaStyle;
				if (st > 5)
					ss->GetText(currItem, st, doc->docParagraphStyles[st].Font, doc->docParagraphStyles[st].FontSize, true);
				else
					ss->GetText(currItem, st, currItem->font(), currItem->fontSize(), true);
				delete ss;
			}
			view->RefreshItem(currItem);
		}
		else
		{
			if (Buffer2.startsWith("<SCRIBUSELEM"))
			{
				view->Deselect(true);
				uint ac = doc->Items->count();
				bool savedAlignGrid = doc->useRaster;
				bool savedAlignGuides = doc->SnapGuides;
				doc->useRaster = false;
				doc->SnapGuides = false;
				slotElemRead(Buffer2, doc->currentPage->xOffset(), doc->currentPage->yOffset(), false, true, doc, view);
				doc->useRaster = savedAlignGrid;
				doc->SnapGuides = savedAlignGuides;
				for (uint as = ac; as < doc->Items->count(); ++as)
				{
					PageItem* currItem = doc->Items->at(as);
					if (currItem->isBookmark)
						AddBookMark(currItem);
					view->SelectItemNr(as);
				}
			}
		}
		slotDocCh(false);
	}
}

void ScribusMainWindow::SelectAll()
{
	if (doc->appMode == modeEdit)
	{
		PageItem *currItem = doc->selection->itemAt(0);
		PageItem *nextItem = currItem;
		while (nextItem != 0)
		{
			if (nextItem->BackBox != 0)
				nextItem = nextItem->BackBox;
			else
				break;
		}
		while (nextItem != 0)
		{
			for (uint a = 0; a < nextItem->itemText.count(); ++a)
			{
				nextItem->itemText.at(a)->cselect = true;
				nextItem->HasSel = true;
			}
			nextItem = nextItem->NextBox;
		}
		view->DrawNew();
		EnableTxEdit();
	}
	else
	{
		PageItem *currItem;
		view->Deselect();
		uint docItemsCount=doc->Items->count();
		for (uint a = 0; a < docItemsCount; ++a)
		{
			currItem = doc->Items->at(a);
			if (currItem->LayerNr == doc->activeLayer())
			{
				if (!currItem->isSelected())
				{
					doc->selection->addItem(currItem);
					currItem->FrameOnly = true;
					currItem->paintObj();
				}
			}
		}
		int docSelectionCount=doc->selection->count();
		if (docSelectionCount > 1)
		{
			view->setGroupRect();
			view->paintGroupRect();
			double x, y, w, h;
			view->getGroupRect(&x, &y, &w, &h);
			propertiesPalette->setXY(x, y);
			propertiesPalette->setBH(w, h);
		}
		if (docSelectionCount > 0)
		{
			currItem = doc->selection->itemAt(0);
			HaveNewSel(currItem->itemType());
		}
	}
}

void ScribusMainWindow::deselectAll()
{
	if (view!=NULL)
		view->Deselect(true);
}

void ScribusMainWindow::ClipChange()
{
	QString cc;
#if QT_VERSION  >= 0x030100
	cc = ClipB->text(QClipboard::Selection);
	if (cc.isNull())
		cc = ClipB->text(QClipboard::Clipboard);
#else
	cc = ClipB->text();
#endif

	scrActions["editPaste"]->setEnabled(false);
	if (!cc.isNull())
	{
		if (!BuFromApp)
			Buffer2 = cc;
		BuFromApp = false;
		if (HaveDoc && (cc.startsWith("<SCRIBUSELEM") || doc->appMode == modeEdit))
			scrActions["editPaste"]->setEnabled(true);
	}
}

void ScribusMainWindow::clearContents()
{
	view->ClearItem();
}

void ScribusMainWindow::EnableTxEdit()
{
	scrActions["editCut"]->setEnabled(true);
	scrActions["editCopy"]->setEnabled(true);
	//scrActions["editClearContents"]->setEnabled(true);
}

void ScribusMainWindow::DisableTxEdit()
{
	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	//scrActions["editClearContents"]->setEnabled(false);
}

void ScribusMainWindow::slotHelpAbout()
{
	About* dia = new About(this);
	dia->exec();
	delete dia;
}

void ScribusMainWindow::slotHelpAboutPlugins()
{
	AboutPlugins* dia = new AboutPlugins(this);
	dia->exec();
	delete dia;
}

void ScribusMainWindow::slotHelpAboutQt()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void ScribusMainWindow::slotOnlineHelp()
{
	HelpBrowser *dia = new HelpBrowser(0, tr("Scribus Manual"), ScMW->guiLanguage);
	dia->show();
}

void ScribusMainWindow::ToggleTips()
{
	QToolTip::setGloballyEnabled(scrActions["helpTooltips"]->isOn());
	prefsManager->appPrefs.showToolTips = scrActions["helpTooltips"]->isOn();
}

void ScribusMainWindow::SaveText()
{
	LoadEnc = "";
	QString wdir = ".";
	QString prefsDocDir=prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		wdir = dirs->get("save_text", prefsDocDir);
	else
		wdir = dirs->get("save_text", ".");
	QString fn = CFileDialog( wdir, tr("Save as"), tr("Text Files (*.txt);;All Files(*)"), "", false, false, false, true);
	if (!fn.isEmpty())
	{
		dirs->set("save_text", fn.left(fn.findRev("/")));
		Serializer *se = new Serializer(fn);
		se->PutText(doc->selection->itemAt(0));
		se->Write(LoadEnc);
		delete se;
	}
}

void ScribusMainWindow::applyNewMaster(QString name)
{
	Apply_MasterPage(name, doc->currentPage->pageNr());
	view->reformPages();
	view->DrawNew();
	slotDocCh();
	pagePalette->Rebuild();
}

void ScribusMainWindow::slotNewPageP(int wo, QString templ)
{
	NoFrameEdit();
	view->Deselect(true);
	slotNewPage(wo, templ); //master page is applied now
	//applyNewMaster(templ);
	doc->addPageToSection(wo-1, 1, 1);
	outlinePalette->BuildTree();
	pagePalette->RebuildPage();
}

/** Erzeugt eine neue Seite */
void ScribusMainWindow::slotNewPageM()
{
	NoFrameEdit();
	view->Deselect(true);
	QStringList base;
	InsPage *dia = new InsPage(this, doc, doc->currentPage->pageNr(), doc->Pages->count());
	if (dia->exec())
	{
		base = dia->getMasterPages();
		addNewPages(dia->getWherePage(),
		            dia->getWhere(),
		            dia->getCount(),
					dia->heightMSpinBox->value() / doc->unitRatio(),
					dia->widthMSpinBox->value() / doc->unitRatio(),
					dia->orientationQComboBox->currentItem(),
					dia->prefsPageSizeName,
					dia->moveObjects->isChecked(),
		            &base
		            );
	}
	delete dia;
}

void ScribusMainWindow::addNewPages(int wo, int where, int numPages, double height, double width, int orient, QString siz, bool mov, QStringList* basedOn)
{
	if (UndoManager::undoEnabled())
	{
		undoManager->beginTransaction(doc->DocName, Um::IDocument, (numPages == 1) ? Um::AddPage : Um::AddPages, "", Um::ICreate);
		SimpleState *ss = new SimpleState(Um::AddPage, "", Um::ICreate);
		ss->set("ADD_PAGE", "add_page");
		ss->set("PAGE", wo);
		ss->set("WHERE", where);
		ss->set("COUNT", numPages);
		if (basedOn != NULL)
			ss->set("BASED", basedOn->join("|"));
		else
			ss->set("BASED", tr("Normal"));
		ss->set("HEIGHT", height);
		ss->set("WIDTH", width);
		ss->set("ORIENT", orient);
		ss->set("SIZE", siz);
		ss->set("MOVED", mov);
		undoManager->action(this, ss);
	}
	QStringList base;
	if (basedOn == NULL)
	{
		for (int b = 0; b < doc->currentPageLayout; ++b)
			base.append(tr("Normal"));
	}
	else
		base = *basedOn;
	int cc;
	int wot = wo;
	if (where==0)
		--wot;
	else if (where==2)
		wot=doc->Pages->count();
	for (cc = 0; cc < numPages; ++cc)
	{
		slotNewPage(wot, QString::null, mov); //Avoid the master page application with QString::null
		doc->currentPage->setInitialHeight(height);
		doc->currentPage->setInitialWidth(width);
		doc->currentPage->PageOri = orient;
		doc->currentPage->PageSize = siz;
		//CB If we want to add this master page setting into the slotnewpage call, the pagenumber must be +1 I think
		applyNewMaster(base[(doc->currentPage->pageNr()+doc->pageSets[doc->currentPageLayout].FirstPage) % doc->pageSets[doc->currentPageLayout].Columns]);
		wot ++;
	}
	//Must use wo-1 as the dialog currently returns a page Index +1 due to old numbering scheme
	doc->addPageToSection(wo-1, where, numPages);
	pagePalette->RebuildPage();
	view->reformPages(mov);
	view->DrawNew();
	outlinePalette->BuildTree();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

//signal is disconnected.. unused, and will be deleted.
void ScribusMainWindow::slotNewMasterPage(int w, const QString& name)
{
	if (doc->masterPageMode())
	{
		doc->addMasterPage(w, name);
		view->addPage(w);
		bool setter = doc->MasterPages.count() > 1 ? true : false;
		scrActions["pageDelete"]->setEnabled(setter);
		scrActions["pageMove"]->setEnabled(setter);
	}
}

void ScribusMainWindow::slotNewPage(int w, const QString& masterPageName, bool mov)
{
	doc->addPage(w, masterPageName, true);
	view->addPage(w, mov);
/*	if ((!doc->loading) && (!doc->masterPageMode))
		outlinePalette->BuildTree(doc); */
	bool setter = doc->DocPages.count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);
	if ((!doc->isLoading()) && (!doc->masterPageMode()))
		AdjustBM();
/*	if ((!doc->loading) && (!doc->masterPageMode))
	{
		AdjustBM();
		if ((doc->PageAT) && (doc->pageCount != 1))
			outlinePalette->slotAddElement(w, 0);
	}
	slotDocCh(!doc->loading); */
}


void ScribusMainWindow::duplicateToMasterPage()
{
	NewTm *dia = new NewTm(this, tr("Name:"), tr("Convert Page to Master Page"), doc, tr("New Master Page %1").arg(doc->MasterNames.count()));
	view->Deselect(true);
	if (dia->exec())
	{
		int diaLinksCurrItem=0;
		int diaLinksCount=0;
		if (doc->currentPageLayout != singlePage)
		{
			diaLinksCurrItem=dia->Links->currentItem();
			diaLinksCount=static_cast<int>(dia->Links->count());
		}
		QString masterPageName = dia->Answer->text();
		while (doc->MasterNames.contains(masterPageName) || (masterPageName == "Normal"))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			masterPageName = dia->Answer->text();
		}
		int currentPageNumber=doc->currentPage->pageNr();
		bool ok=doc->copyPageToMasterPage(currentPageNumber, diaLinksCurrItem, diaLinksCount, masterPageName);
		Q_ASSERT(ok); //TODO get a return value in case the copy was not possible
	}
	delete dia;
}

/*!
	\fn void ScribusMainWindow::slotZoom(double zoomFactor)
	\author Craig Bradney
	\date Sun 30 Jan 2005
	\brief Take the ScMW zoom actions and pass the view a %. Actions have whole number values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
	\param zoomFactor Value stored in the ScrAction.
 */
void ScribusMainWindow::slotZoom(double zoomFactor)
{
	double finalZoomFactor;
	//Zoom to Fit
	if (zoomFactor==-100.0)
	{
		double dx = (view->width()-50) / (doc->pageWidth+30);
		double dy = (view->height()-70) / (doc->pageHeight+30);
		finalZoomFactor = (dx > dy) ? dy : dx;
	}
	//Zoom to %
	else
		finalZoomFactor = zoomFactor*prefsManager->displayScale()/100.0;
	view->setScale(finalZoomFactor);
	view->slotDoZoom();
}

void ScribusMainWindow::ToggleAllPalettes()
{
	if (PalettesStat[0])
	{
		PalettesStat[0] = false;
		propertiesPalette->show();
		outlinePalette->show();
		scrapbookPalette->show();
		bookmarkPalette->show();
		pagePalette->show();
		layerPalette->show();
		measurementPalette->show();
		docCheckerPalette->show();
		setPagePalette(PalettesStat[5]);
		setUndoPalette(PalettesStat[8]);
	}
	else
	{
		PalettesStat[1] = propertiesPalette->isVisible();
		PalettesStat[2] = outlinePalette->isVisible();
		PalettesStat[3] = scrapbookPalette->isVisible();
		PalettesStat[4] = layerPalette->isVisible();
		PalettesStat[5] = pagePalette->isVisible();
		PalettesStat[6] = bookmarkPalette->isVisible();
		PalettesStat[7] = measurementPalette->isVisible();
		PalettesStat[8] = undoPalette->isVisible();
		PalettesStat[9] = docCheckerPalette->isVisible();
		propertiesPalette->hide();
		outlinePalette->hide();
		scrapbookPalette->hide();
		bookmarkPalette->hide();
		pagePalette->hide();
		layerPalette->hide();
		measurementPalette->hide();
		docCheckerPalette->hide();
		setPagePalette(false);
		setUndoPalette(false);
		PalettesStat[0] = true;
	}
}

void ScribusMainWindow::toggleCheckPal()
{
	PalettesStat[0] = false;
}

void ScribusMainWindow::setUndoPalette(bool visible)
{
	undoPalette->setPaletteShown(visible);
	scrActions["toolsActionHistory"]->setOn(visible);
}
/*
void ScribusMainWindow::togglePropertiesPalette()
{
	PalettesStat[0] = false;
}

void ScribusMainWindow::toggleOutlinePalette()
{
	PalettesStat[0] = false;
}

void ScribusMainWindow::toggleScrapbookPalette()
{
	PalettesStat[0] = false;
}

void ScribusMainWindow::toggleLayerPalette()
{
	PalettesStat[0] = false;
}
*/
void ScribusMainWindow::setPagePalette(bool visible)
{
	if (!visible)
	{
		prefsManager->appPrefs.SepalT = pagePalette->getThumb();
		prefsManager->appPrefs.SepalN = pagePalette->getNamen();
	}
}

void ScribusMainWindow::togglePagePalette()
{
	setPagePalette(!pagePalette->isVisible());
	PalettesStat[0] = false;
}
/*
void ScribusMainWindow::toggleBookmarkPalette()
{
	PalettesStat[0] = false;
}
*/

void ScribusMainWindow::toggleUndoPalette()
{
	setUndoPalette(!undoPalette->isVisible());
	PalettesStat[0] = false;
}

void ScribusMainWindow::TogglePics()
{
	doc->guidesSettings.showPic = !doc->guidesSettings.showPic;
	for (uint b=0; b<doc->Items->count(); ++b)
	{
		if (doc->Items->at(b)->asImageFrame())
			doc->Items->at(b)->setImageShown(doc->guidesSettings.showPic);
	}
	view->DrawNew();
}

void ScribusMainWindow::ToggleAllGuides()
{
	keyrep=false;
	if (GuidesStat[0])
	{
		GuidesStat[0] = false;
		doc->guidesSettings.marginsShown = GuidesStat[1];
		doc->guidesSettings.framesShown = GuidesStat[2];
		doc->guidesSettings.gridShown = GuidesStat[3];
		doc->guidesSettings.guidesShown = GuidesStat[4];
		doc->guidesSettings.baseShown = GuidesStat[5];
		doc->guidesSettings.linkShown = GuidesStat[6];
		doc->guidesSettings.showControls = GuidesStat[7];
		doc->guidesSettings.rulerMode = GuidesStat[8];
		ToggleMarks();
		ToggleFrames();
		ToggleRaster();
		ToggleGuides();
		ToggleBase();
		ToggleTextLinks();
		ToggleTextControls();
		ToggleRuler();
	}
	else
	{
		GuidesStat[0] = true;
		GuidesStat[1] = !doc->guidesSettings.marginsShown;
		GuidesStat[2] = !doc->guidesSettings.framesShown;
		GuidesStat[3] = !doc->guidesSettings.gridShown;
		GuidesStat[4] = !doc->guidesSettings.guidesShown;
		GuidesStat[5] = !doc->guidesSettings.baseShown;
		GuidesStat[6] = !doc->guidesSettings.linkShown;
		GuidesStat[7] = !doc->guidesSettings.showControls;
		GuidesStat[8] = !doc->guidesSettings.rulerMode;
		doc->guidesSettings.marginsShown = false;
		doc->guidesSettings.framesShown = false;
		doc->guidesSettings.gridShown = false;
		doc->guidesSettings.guidesShown = false;
		doc->guidesSettings.baseShown = false;
		doc->guidesSettings.linkShown = false;
		doc->guidesSettings.showControls = false;
		doc->guidesSettings.rulerMode = false;
		scrActions["viewShowMargins"]->setOn(doc->guidesSettings.marginsShown);
		scrActions["viewShowFrames"]->setOn(doc->guidesSettings.framesShown);
		scrActions["viewShowGrid"]->setOn(doc->guidesSettings.gridShown);
		scrActions["viewShowGuides"]->setOn(doc->guidesSettings.guidesShown);
		scrActions["viewShowBaseline"]->setOn(doc->guidesSettings.baseShown);
		scrActions["viewShowTextChain"]->setOn(doc->guidesSettings.linkShown);
		scrActions["viewShowTextControls"]->setOn(doc->guidesSettings.showControls);
		scrActions["viewRulerMode"]->setOn(doc->guidesSettings.rulerMode);
	}
	view->DrawNew();
}

void ScribusMainWindow::ToggleMarks()
{
	GuidesStat[0] = false;
	doc->guidesSettings.marginsShown = !doc->guidesSettings.marginsShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleFrames()
{
	GuidesStat[0] = false;
	doc->guidesSettings.framesShown = !doc->guidesSettings.framesShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleRaster()
{
	GuidesStat[0] = false;
	doc->guidesSettings.gridShown = !doc->guidesSettings.gridShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleGuides()
{
	GuidesStat[0] = false;
	doc->guidesSettings.guidesShown = !doc->guidesSettings.guidesShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleBase()
{
	GuidesStat[0] = false;
	doc->guidesSettings.baseShown = !doc->guidesSettings.baseShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleTextLinks()
{
	GuidesStat[0] = false;
	doc->guidesSettings.linkShown = !doc->guidesSettings.linkShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleTextControls()
{
	GuidesStat[0] = false;
	doc->guidesSettings.showControls = !doc->guidesSettings.showControls;
	view->DrawNew();
}

void ScribusMainWindow::ToggleRuler()
{
	GuidesStat[0] = false;
	doc->guidesSettings.rulerMode = !doc->guidesSettings.rulerMode;
	view->DrawNew();
}

void ScribusMainWindow::ToggleURaster()
{
	doc->useRaster = !doc->useRaster;
}

void ScribusMainWindow::ToggleUGuides()
{
	doc->SnapGuides = !doc->SnapGuides;
}

void ScribusMainWindow::ToggleFrameEdit()
{
	if (doc->EditClip)
	{
		NoFrameEdit();
	}
	else
	{
		slotSelect();
		nodePalette->setDoc(doc, view);
		nodePalette->MoveN();
		nodePalette->HaveNode(false, false);
		nodePalette->MoveNode->setOn(true);
		nodePalette->show();
		connect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
		doc->EditClipMode = 0;
		doc->EditClip = true;
		scrActions["toolsSelect"]->setEnabled(false);
		scrActions["toolsRotate"]->setEnabled(false);
		scrActions["toolsEditContents"]->setEnabled(false);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
		scrActions["toolsZoom"]->setEnabled(false);
		scrActions["toolsInsertTextFrame"]->setEnabled(false);
		scrActions["toolsInsertImageFrame"]->setEnabled(false);
		scrActions["toolsInsertTableFrame"]->setEnabled(false);
		scrActions["toolsInsertShape"]->setEnabled(false);
		scrActions["toolsInsertLine"]->setEnabled(false);
		scrActions["toolsInsertBezier"]->setEnabled(false);
		scrActions["toolsInsertFreehandLine"]->setEnabled(false);
		scrActions["toolsInsertPolygon"]->setEnabled(false);
		scrActions["toolsLinkTextFrame"]->setEnabled(false);
		scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		scrActions["toolsMeasurements"]->setEnabled(false);
		pdfToolBar->PDFTool->setEnabled(false);
		pdfToolBar->PDFaTool->setEnabled(false);
		scrActions["itemDelete"]->setEnabled(false);
		if (doc->selection->count() != 0)
		{
			PageItem *currItem = doc->selection->itemAt(0);
			view->MarkClip(currItem, currItem->PoLine, true);
			nodePalette->EditCont->setEnabled(currItem->ContourLine.size() != 0);
			nodePalette->ResetCont->setEnabled(false);
			nodePalette->PolyStatus(currItem->itemType(), currItem->PoLine.size());
		}
	}
	scrActions["itemShapeEdit"]->setOn(doc->EditClip);
}

void ScribusMainWindow::NoFrameEdit()
{
	disconnect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
	actionManager->disconnectModeActions();
	nodePalette->hide();
	scrActions["toolsSelect"]->setEnabled(true);
	scrActions["toolsSelect"]->setOn(true);
	scrActions["toolsRotate"]->setEnabled(true);
	scrActions["toolsZoom"]->setEnabled(true);
	scrActions["toolsInsertTextFrame"]->setEnabled(true);
	scrActions["toolsInsertImageFrame"]->setEnabled(true);
	scrActions["toolsInsertTableFrame"]->setEnabled(true);
	scrActions["toolsInsertShape"]->setEnabled(true);
	scrActions["toolsInsertLine"]->setEnabled(true);
	scrActions["toolsInsertBezier"]->setEnabled(true);
	scrActions["toolsInsertFreehandLine"]->setEnabled(true);
	scrActions["toolsInsertPolygon"]->setEnabled(true);
	pdfToolBar->PDFTool->setEnabled(true);
	pdfToolBar->PDFaTool->setEnabled(true);
	scrActions["toolsEditContents"]->setOn(false);
	scrActions["toolsEditWithStoryEditor"]->setOn(false);
	scrActions["toolsMeasurements"]->setEnabled(true);
	scrActions["toolsUnlinkTextFrame"]->setEnabled(true);
	scrActions["itemDelete"]->setEnabled(true);
	scrActions["itemShapeEdit"]->setOn(false);
	if (HaveDoc)
	{
		doc->EditClip = false;
		view->EditContour = false;
		if (doc->selection->count() != 0)
		{
			HaveNewSel(doc->selection->itemAt(0)->itemType());
			view->DrawNew();
		}
		else
			HaveNewSel(-1);
	}
	actionManager->connectModeActions();
}

void ScribusMainWindow::slotSelect()
{
	pdfToolBar->PDFTool->setOn(false);
	pdfToolBar->PDFaTool->setOn(false);
	setAppMode(modeNormal);
}

void ScribusMainWindow::setAppModeByToggle(bool isOn, int newMode)
{
	keyrep=false;
	if (isOn)
		setAppMode(newMode);
	else
		setAppMode(modeNormal);
}

void ScribusMainWindow::setAppMode(int mode)
{
	//disconnect the tools actions so we dont fire them off
	actionManager->disconnectModeActions();
	//set the actions state based on incoming mode
	scrActions["toolsSelect"]->setOn(mode==modeNormal);
	scrActions["toolsInsertTextFrame"]->setOn(mode==modeDrawText);
	scrActions["toolsInsertImageFrame"]->setOn(mode==modeDrawPicture);
	scrActions["toolsInsertTableFrame"]->setOn(mode==modeDrawTable);
	scrActions["toolsInsertShape"]->setOn(mode==modeDrawShapes);
	scrActions["toolsInsertPolygon"]->setOn(mode==modeDrawRegularPolygon);
	scrActions["toolsInsertLine"]->setOn(mode==modeDrawLine);
	scrActions["toolsInsertBezier"]->setOn(mode==modeDrawBezierLine);
	scrActions["toolsInsertFreehandLine"]->setOn(mode==modeDrawFreehandLine);
	scrActions["toolsRotate"]->setOn(mode==modeRotation);
	scrActions["toolsZoom"]->setOn(mode==modeMagnifier);
	scrActions["toolsEditContents"]->setOn(mode==modeEdit);
	scrActions["toolsEditWithStoryEditor"]->setOn(mode==modeStoryEditor);
	scrActions["toolsLinkTextFrame"]->setOn(mode==modeLinkFrames);
	scrActions["toolsUnlinkTextFrame"]->setOn(mode==modeUnlinkFrames);
	scrActions["toolsEyeDropper"]->setOn(mode==modeEyeDropper);
	scrActions["toolsCopyProperties"]->setOn(mode==modeCopyProperties);

	if (HaveDoc)
	{
		PageItem *currItem;
		//setActiveWindow();

		if (doc->selection->count() != 0)
			currItem = doc->selection->itemAt(0);
		else
			currItem = 0;
		int oldMode = doc->appMode;
		doc->appMode = mode;
		if (oldMode == modeMeasurementTool)
			disconnect(view, SIGNAL(MVals(double, double, double, double, double, double, int )), measurementPalette, SLOT(setValues(double, double, double, double, double, double, int )));
		if (mode != modeEdit && doc->CurTimer!=NULL)
		{
			disconnect(doc->CurTimer, SIGNAL(timeout()), view, SLOT(BlinkCurs()));
			doc->CurTimer->stop();
			delete doc->CurTimer;
			doc->CurTimer = NULL;
		}
		if (mode!=modeEdit && oldMode==modeEdit)
			actionManager->restoreActionShortcutsPostEditMode();
		else
		if (mode==modeEdit && oldMode!=modeEdit)
			actionManager->saveActionShortcutsPreEditMode();
		if (oldMode == modeEdit)
		{
			view->zoomSpinBox->setFocusPolicy(QWidget::ClickFocus);
			view->pageSelector->focusPolicy(QWidget::ClickFocus);
			scrActions["editClearContents"]->setEnabled(false);
			scrActions["insertGlyph"]->setEnabled(false);
			view->slotDoCurs(false);
			if (currItem != 0)
			{
				view->RefreshItem(currItem);
				scrMenuMgr->setMenuEnabled("Style", true);
				scrMenuMgr->setMenuEnabled("Item", true);
			}
			view->horizRuler->ItemPosValid = false;
			view->horizRuler->repaint();
		}
		if (mode == modeEdit)
		{
			view->zoomSpinBox->setFocusPolicy(QWidget::NoFocus);
			view->pageSelector->focusPolicy(QWidget::NoFocus);
			if (currItem != 0)
			{
				if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::PathText))
				{
					doc->appMode = 1;
					ToggleFrameEdit();
					return;
				}
				setTBvals(currItem);
				currItem->CPos = 0;
			}
			scrActions["editPaste"]->setEnabled(false);
			scrActions["insertGlyph"]->setEnabled(true);
			if (currItem!=NULL && currItem->asTextFrame())
				actionManager->enableUnicodeActions(true);
			if (!Buffer2.isNull())
			{
//				if (!Buffer2.startsWith("<SCRIBUSELEM"))
//				{
					BuFromApp = false;
					scrActions["editPaste"]->setEnabled(true);
//				}
			}
			view->slotDoCurs(true);
			scrMenuMgr->setMenuEnabled("Item", false);
			scrMenuMgr->setMenuEnabled("Style", false);
			doc->CurTimer = new QTimer(view);
			if (doc->CurTimer!=NULL)
			{
				connect(doc->CurTimer, SIGNAL(timeout()), view, SLOT(BlinkCurs()));
				doc->CurTimer->start(500);
			}
			if (currItem != 0)
			{
				scrActions["editCut"]->setEnabled(currItem->HasSel);
				scrActions["editCopy"]->setEnabled(currItem->HasSel);
				scrActions["editClearContents"]->setEnabled(currItem->HasSel);
				scrActions["editSearchReplace"]->setEnabled(true);

				view->RefreshItem(currItem);
			}
		}
		int docSelectionCount=doc->selection->count();
		if (mode == modeDrawBezierLine)
		{
			if (docSelectionCount != 0)
				view->Deselect(true);
			view->FirstPoly = true;
		}
		if (mode == modeEditGradientVectors)
			propertiesPalette->setGradientEditMode(true);
		if (mode == modeMeasurementTool)
		{
			measurementPalette->show();
			connect(view, SIGNAL(MVals(double, double, double, double, double, double, int)), measurementPalette, SLOT(setValues(double, double, double, double, double, double, int )));
		}
		switch (mode)
		{
			case modeDrawShapes:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")), true);
				break;
			case modeDrawPicture:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")), true);
				break;
			case modeDrawText:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")), true);
				break;
			case modeDrawTable:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawTable.xpm")), true);
				break;
			case modeDrawRegularPolygon:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawPolylineFrame.xpm")), true);
				break;
			case modeMagnifier:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
				break;
			case modePanning:
				qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
				break;
			case modeDrawLine:
			case modeInsertPDFButton:
			case modeInsertPDFTextfield:
			case modeInsertPDFCheckbox:
			case modeInsertPDFCombobox:
			case modeInsertPDFListbox:
			case modeInsertPDFTextAnnotation:
			case modeInsertPDFLinkAnnotation:
			case modeDrawFreehandLine:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				break;
			default:
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			break;
		}
		if (mode == modeDrawShapes)
		{
			doc->SubMode = mainToolBar->SubMode;
			doc->ShapeValues = mainToolBar->ShapeVals;
			doc->ValCount = mainToolBar->ValCount;
			propertiesPalette->SCustom->setPixmap(propertiesPalette->SCustom->getIconPixmap(doc->SubMode));
			SCustom->setPixmap(SCustom->getIconPixmap(doc->SubMode));
		}
		else
			doc->SubMode = -1;
		if (mode == modeNormal)
		{
			pdfToolBar->PDFTool->setOn(false);
			pdfToolBar->PDFaTool->setOn(false);
			propertiesPalette->setGradientEditMode(false);
		}
		if (mode == modeLinkFrames)
			doc->ElemToLink = doc->selection->itemAt(0);
		if ((mode == modeLinkFrames) || (mode == modeUnlinkFrames) || (oldMode == modeLinkFrames) || (oldMode == modeUnlinkFrames))
			view->updateContents();
		if (mode == modeStoryEditor)
		{
			slotStoryEditor();
			slotSelect();
		}
		if (mode == modeEyeDropper)
			grabMouse();
		else
			releaseMouse();
		if (mode == modeCopyProperties)
		{
			if (doc->selection->count() != 0)
			{
				doc->ElemToLink = doc->selection->itemAt(0);
				view->Deselect(true);
				scrActions["toolsCopyProperties"]->setEnabled(true);
			}
		}
		if (mode != modeNormal && mode != modeStoryEditor)
			setActiveWindow();
	}
	actionManager->connectModeActions();
}

void ScribusMainWindow::setMainWindowActive()
{
	setActiveWindow();
	raise();
}

void ScribusMainWindow::setItemTypeStyle(int id)
{
	int b = 0;
	if (id == 0)
	{
		scrActions["typeEffectNormal"]->setOn(true);
		scrActions["typeEffectUnderline"]->setOn(false);
		scrActions["typeEffectUnderlineWords"]->setOn(false);
		scrActions["typeEffectStrikeThrough"]->setOn(false);
		scrActions["typeEffectSmallCaps"]->setOn(false);
		scrActions["typeEffectAllCaps"]->setOn(false);
		scrActions["typeEffectSuperscript"]->setOn(false);
		scrActions["typeEffectSubscript"]->setOn(false);
		scrActions["typeEffectOutline"]->setOn(false);
		scrActions["typeEffectShadow"]->setOn(false);
	}
	else
	{
		scrActions["typeEffectNormal"]->setOn(false);
		if (id == 4)
			scrActions["typeEffectSubscript"]->setOn(false);
		if (id == 5)
			scrActions["typeEffectSuperscript"]->setOn(false);
		if (id == 3)
			scrActions["typeEffectAllCaps"]->setOn(false);
		if (id == 7)
			scrActions["typeEffectSmallCaps"]->setOn(false);
		if (id == 8)
			scrActions["typeEffectUnderline"]->setOn(false);
		if (id == 1)
			scrActions["typeEffectUnderlineWords"]->setOn(false);
		if (scrActions["typeEffectUnderline"]->isOn())
			b |= 8;
		if (scrActions["typeEffectUnderlineWords"]->isOn())
			b |= 512;
		if (scrActions["typeEffectShadow"]->isOn())
			b |= 256;
		if (scrActions["typeEffectStrikeThrough"]->isOn())
			b |= 16;
		if (scrActions["typeEffectAllCaps"]->isOn())
			b |= 32;
		if (scrActions["typeEffectSmallCaps"]->isOn())
			b |= 64;
		if (scrActions["typeEffectSuperscript"]->isOn())
			b |= 1;
		if (scrActions["typeEffectSubscript"]->isOn())
			b |= 2;
		if (scrActions["typeEffectOutline"]->isOn())
			b |= 4;
	}
	setItemHoch(b);
}

void ScribusMainWindow::setStilvalue(int s)
{
	int c = s & 1919;
	doc->CurrentStyle = c;
	scrActions["typeEffectNormal"]->setOn(c==0);
	scrActions["typeEffectSuperscript"]->setOn(c & 1);
	scrActions["typeEffectSubscript"]->setOn(c & 2);
	scrActions["typeEffectOutline"]->setOn(c & 4);
	scrActions["typeEffectUnderline"]->setOn(c & 8);
	scrActions["typeEffectStrikeThrough"]->setOn(c & 16);
	scrActions["typeEffectAllCaps"]->setOn(c & 32);
	scrActions["typeEffectSmallCaps"]->setOn(c & 64);
	scrActions["typeEffectShadow"]->setOn(c & 256);
	scrActions["typeEffectUnderlineWords"]->setOn(c & 512);
	emit TextStil(s);
}

void ScribusMainWindow::setItemHoch(int h)
{
	if (doc->selection->count() != 0)
	{
		//setActiveWindow();
		doc->CurrentStyle = h;
		setStilvalue(doc->CurrentStyle);
		doc->chTyStyle(h);
		slotDocCh();
	}
}

void ScribusMainWindow::AdjustBM()
{
	for (uint b = 0; b < doc->Items->count(); ++b)
	{
		PageItem* bb = doc->Items->at(b);
		if (bb->isBookmark)
		{
			int it = bb->BMnr;
			QListViewItemIterator itn(bookmarkPalette->BView);
			for ( ; itn.current(); ++itn)
			{
				BookMItem *ite = (BookMItem*)itn.current();
				if (ite->ItemNr == it)
				{
					ite->Seite = bb->OwnPage;
					break;
				}
			}
		}
	}
	StoreBookmarks();
}

void ScribusMainWindow::DeletePage2(int pg)
{
	PageItem* ite;
	NoFrameEdit();
	if (doc->Pages->count() == 1)
		return;
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(doc->DocName, Um::IDocument, Um::DeletePage, "", Um::IDelete);
/*	if (!doc->masterPageMode)
		disconnect(doc->currentPage, SIGNAL(DelObj(uint, uint)), outlinePalette, SLOT(slotRemoveElement(uint, uint))); */
	doc->selection->clear();
	for (uint d = 0; d < doc->Items->count(); ++d)
	{
		ite = doc->Items->at(d);
		if (ite->OwnPage == pg)
		{
			ite->setLocked(false);
			ite->isSingleSel = false;
			if (ite->isBookmark)
				DelBookMark(ite);
			ite->isBookmark = false;
			doc->selection->addItem(ite);
		}
	}
	if (doc->selection->count() != 0)
		view->DeleteItem();
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::DeletePage, "", Um::ICreate);
		ss->set("DELETE_PAGE", "delete_page");
		ss->set("PAGENR", pg + 1);
		ss->set("MASTERPAGE", doc->Pages->at(pg)->MPageNam);
		// replace the deleted page in the undostack by a dummy object that will
		// replaced with the "undone" page if user choose to undo the action
		DummyUndoObject *duo = new DummyUndoObject();
		uint id = static_cast<uint>(duo->getUId());
		undoManager->replaceObject(doc->Pages->at(pg)->getUId(), duo);
		ss->set("DUMMY_ID", id);
		undoManager->action(this, ss);
	}
	if (doc->masterPageMode())
		doc->deleteMasterPage(pg);
	else
		doc->deletePage(pg);
	disconnect(view->pageSelector, SIGNAL(GotoPage(int)), view, SLOT(GotoPa(int)));
	view->pageSelector->setMaxValue(doc->Pages->count());
	view->pageSelector->GotoPg(0);
	connect(view->pageSelector, SIGNAL(GotoPage(int)), view, SLOT(GotoPa(int)));
	view->reformPages();
	AdjustBM();
	view->DrawNew();
	doc->OpenNodes.clear();
	outlinePalette->BuildTree();
	bool setter = doc->Pages->count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);
	slotDocCh();
	pagePalette->RebuildPage();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusMainWindow::DeletePage()
{
	NoFrameEdit();
	view->Deselect(true);
	DelPages *dia = new DelPages(this, doc->currentPage->pageNr()+1, doc->Pages->count());
	if (dia->exec())
		DeletePage(dia->getFromPage(), dia->getToPage());
	delete dia;
}

void ScribusMainWindow::DeletePage(int from, int to)
{
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(doc->DocName, Um::IDocument,
									  (from - to == 0) ? Um::DeletePage : Um::DeletePages, "",
									  Um::IDelete);
	PageItem* ite;
	doc->selection->clear();
	for (int a = to - 1; a >= from - 1; a--)
	{
		for (uint d = 0; d < doc->Items->count(); ++d)
		{
			ite = doc->Items->at(d);
			if (ite->OwnPage == a)
			{
				ite->setLocked(false);
				ite->isSingleSel = false;
				if (ite->isBookmark)
					DelBookMark(ite);
				ite->isBookmark = false;
				doc->selection->addItem(ite);
			}
		}
		AdjustBM();
	}
	if (doc->selection->count() != 0)
		view->DeleteItem();
	for (int a = to - 1; a >= from - 1; a--)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::DeletePage, "", Um::ICreate);
			ss->set("DELETE_PAGE", "delete_page");
			ss->set("PAGENR", a + 1);
			ss->set("MASTERPAGE", doc->Pages->at(a)->MPageNam);
			// replace the deleted page in the undostack by a dummy object that will
			// replaced with the "undone" page if user choose to undo the action
			DummyUndoObject *duo = new DummyUndoObject();
			uint id = static_cast<uint>(duo->getUId());
			undoManager->replaceObject(doc->Pages->at(a)->getUId(), duo);
			ss->set("DUMMY_ID", id);
			undoManager->action(this, ss);
		}
		if (doc->masterPageMode())
			doc->deleteMasterPage(a);
		else
			doc->deletePage(a);
		disconnect(view->pageSelector, SIGNAL(GotoPage(int)), view, SLOT(GotoPa(int)));
		view->pageSelector->setMaxValue(doc->Pages->count());
		view->pageSelector->GotoPg(0);
		connect(view->pageSelector, SIGNAL(GotoPage(int)), view, SLOT(GotoPa(int)));
		doc->removePageFromSection(a);
	}
	view->reformPages();
	view->DrawNew();
//	doc->OpenNodes.clear();
	outlinePalette->BuildTree();
	bool setter = doc->Pages->count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);
	slotDocCh();
	pagePalette->RebuildPage();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusMainWindow::MovePage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->currentPage->pageNr()+1, doc->Pages->count(), true);
	if (dia->exec())
	{
//		doc->OpenNodes = outlinePalette->buildReopenVals();
		int from = dia->getFromPage();
		int to = dia->getToPage();
		int wie = dia->getWhere();
		int wo = dia->getWherePage();
		if ((from != wo) || (wie == 2))
		{
			doc->movePage(from-1, to, wo-1, wie);
			view->reformPages();
		}
		slotDocCh();
		view->DrawNew();
		AdjustBM();
		pagePalette->RebuildPage();
		outlinePalette->BuildTree();
		outlinePalette->reopenTree(doc->OpenNodes);
	}
	delete dia;
}

void ScribusMainWindow::CopyPage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->currentPage->pageNr()+1, doc->Pages->count(), false);
	if (dia->exec())
	{
		int pageNumberToCopy=dia->getFromPage()-1;
		int whereToInsert=dia->getWhere();
		int copyCount=dia->getCopyCount();
		int wo = dia->getWherePage();
		doc->copyPage(pageNumberToCopy, wo, whereToInsert, copyCount);
		view->Deselect(true);
		view->DrawNew();
		slotDocCh();
		pagePalette->RebuildPage();
		outlinePalette->BuildTree();
		AdjustBM();
	}
	delete dia;
}

void ScribusMainWindow::changePageMargins()
{
	int lp;
	NoFrameEdit();
	MarginDialog *dia = new MarginDialog(this, doc);
	if (dia->exec())
	{
		if (doc->masterPageMode())
		{
			if (doc->currentPageLayout != singlePage)
			{
				lp = dia->Links->currentItem();
				if (lp == 0)
					lp = 1;
				else if (lp == static_cast<int>(dia->Links->count()-1))
					lp = 0;
				else
					lp++;
			}
			doc->changePageMargins(dia->GroupRand->RandT, dia->GroupRand->RandB, dia->GroupRand->RandL, dia->GroupRand->RandR, dia->pageHeight, dia->pageWidth, dia->pageHeight, dia->pageWidth, dia->orientationQComboBox->currentItem(), dia->prefsPageSizeName, doc->currentPage->pageNr(), lp);
		}
		else
			doc->changePageMargins(dia->GroupRand->RandT, dia->GroupRand->RandB, dia->GroupRand->RandL, dia->GroupRand->RandR, dia->pageHeight, dia->pageWidth, dia->pageHeight, dia->pageWidth, dia->orientationQComboBox->currentItem(), dia->prefsPageSizeName, doc->currentPage->pageNr());
		view->reformPages(dia->moveObjects->isChecked());
		view->DrawNew();
		slotDocCh();
	}
	delete dia;
}

void ScribusMainWindow::setItemFont2(int id)
{
	disconnect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
	SetNewFont(FontSub->text(id));
	FontMenu->activateItemAt(0);
	connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
}

void ScribusMainWindow::setItemFont(int id)
{
	QString nf;
	int a = FontMenu->indexOf(id);
	if (a == 1)
		return;
	disconnect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
	nf = FontID[id];
	SetNewFont(nf);
	connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
}

void ScribusMainWindow::SetNewFont(const QString& nf)
{
	setMainWindowActive();
	QString nf2(nf);
	if (!doc->UsedFonts.contains(nf))
	{
		if (doc->AddFont(nf)) //, prefsManager->appPrefs.AvailFonts[nf]->Font))
		{
			int a = FontMenu->insertItem(new FmItem(nf, prefsManager->appPrefs.AvailFonts[nf]));
			FontID.insert(a, prefsManager->appPrefs.AvailFonts[nf]->scName());
		}
		else
		{
			if (doc->selection->count() != 0)
			{
				PageItem *currItem = doc->selection->itemAt(0);
				nf2 = currItem->font();
			}
			propertiesPalette->Fonts->RebuildList(doc);
			buildFontMenu();
		}
	}
	AdjustFontMenu(nf2);
	doc->ItemFont(nf2);
	doc->CurrFont = nf2;
	slotDocCh();
}

void ScribusMainWindow::AdjustFontMenu(QString nf)
{
	QString df;
	FontSub->setCurrentText(nf);
	//propertiesPalette->Fonts->setCurrentFont(nf);
	for (uint a = 2; a < FontMenu->count(); ++a)
	{
		df = FontID[FontMenu->idAt(a)];
		FontMenu->setItemChecked(FontMenu->idAt(a), (df == nf));
	}
}

void ScribusMainWindow::setItemFSize(int id)
{
	int c = id;
	if (c != -1)
		doc->chFSize(c*10);
	else
	{
		bool ok = false;
		Query* dia = new Query(this, "New", 1, 0, tr("&Size:"), tr("Size"));
		if (dia->exec())
		{
			c = qRound(dia->getEditText().toDouble(&ok));
			if ((ok) && (c < 1025) && (c > 0))
				doc->chFSize(c*10);
			delete dia;
		}
	}
	propertiesPalette->setSize(c*10);
	slotDocCh();
}

void ScribusMainWindow::setFSizeMenu(int size)
{
	if (scrActions[QString("fontSize%1").arg(size/10)])
		scrActions[QString("fontSize%1").arg(size/10)]->setOn(true);
}

void ScribusMainWindow::setItemFarbe(int id)
{
	if (doc->selection->count() != 0)
	{
		PageItem *currItem = doc->selection->itemAt(0);
		if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
			doc->ItemTextBrush(ColorMenC->text(id));
		else
			doc->ItemBrush(ColorMenC->text(id));
	}
	scrMenuMgr->getLocalPopupMenu("Color")->activateItemAt(0);
	slotDocCh();
}

void ScribusMainWindow::setItemShade(int id)
{
	int c = id;
	bool ok = false;
	if (doc->selection->count() != 0)
	{
		PageItem *currItem = doc->selection->itemAt(0);
		if (c != -1)
		{
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				doc->ItemTextBrushS(c);
			else
				doc->ItemBrushShade(c);
		}
		else
		{
			Query* dia = new Query(this, "New", 1, 0, tr("&Shade:"), tr("Shade"));
			if (dia->exec())
			{
				c = dia->getEditText().toInt(&ok);
				if (ok)
				{
					if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
						doc->ItemTextBrushS(c);
					else
						doc->ItemBrushShade(c);
				}
				delete dia;
			}
		}
	}
	slotDocCh();
}

void ScribusMainWindow::setCSMenu(QString , QString l, int  , int ls)
{
	QString la;
	int lb;
	PageItem *currItem;
	if (doc->selection->count() != 0)
	{
		currItem = doc->selection->itemAt(0);
		if ((currItem->asTextFrame()) || (currItem->asPathText()))
		{
			if ((doc->appMode == modeEdit) && (currItem->itemText.count() != 0))
			{
				la = currItem->itemText.at(QMIN(currItem->CPos, static_cast<int>(currItem->itemText.count()-1)))->ccolor;
				lb = currItem->itemText.at(QMIN(currItem->CPos, static_cast<int>(currItem->itemText.count()-1)))->cshade;
			}
			else
			{
				la = currItem->TxtFill;
				lb = currItem->ShTxtFill;
			}
		}
		else
		{
			la = l;
			lb = ls;
		}
	}
	else
	{
		la = l;
		lb = ls;
	}
	if (la == CommonStrings::None)
		la = tr("None");
	for (uint a = 0; a < static_cast<uint>(ColorMenC->count()); ++a)
	{
		if (ColorMenC->text(a) == la)
			ColorMenC->setCurrentItem(a);
	}
	if (scrActions[QString("shade%1").arg(lb)])
		scrActions[QString("shade%1").arg(lb)]->setOn(true);
}

void ScribusMainWindow::slotEditLineStyles()
{
	if (HaveDoc)
	{
		LineFormate *dia = new LineFormate(this, doc);
		connect(dia, SIGNAL(saveStyle(LineFormate *)), this, SLOT(saveLStyles(LineFormate *)));
		if (dia->exec())
			saveLStyles(dia);
		disconnect(dia, SIGNAL(saveStyle(LineFormate *)), this, SLOT(saveLStyles(LineFormate *)));
		delete dia;
	}
}

void ScribusMainWindow::saveLStyles(LineFormate *dia)
{
	PageItem* ite;
	doc->MLineStyles = dia->TempStyles;
	for (uint d = 0; d < doc->DocItems.count(); ++d)
	{
		ite = doc->DocItems.at(d);
		if (!ite->NamedLStyle.isEmpty())
		{
			if (!doc->MLineStyles.contains(ite->NamedLStyle))
				ite->NamedLStyle = dia->Replacement[ite->NamedLStyle];
		}
	}
	for (uint d1 = 0; d1 < doc->MasterItems.count(); ++d1)
	{
		ite = doc->MasterItems.at(d1);
		if (!ite->NamedLStyle.isEmpty())
		{
			if (!doc->MLineStyles.contains(ite->NamedLStyle))
				ite->NamedLStyle = dia->Replacement[ite->NamedLStyle];
		}
	}
	for (uint d1 = 0; d1 < doc->FrameItems.count(); ++d1)
	{
		ite = doc->FrameItems.at(d1);
		if (!ite->NamedLStyle.isEmpty())
		{
			if (!doc->MLineStyles.contains(ite->NamedLStyle))
				ite->NamedLStyle = dia->Replacement[ite->NamedLStyle];
		}
	}
	propertiesPalette->SetLineFormats(doc);
	view->DrawNew();
}

void ScribusMainWindow::slotEditStyles()
{
	if (HaveDoc)
	{
		StilFormate *dia = new StilFormate(this, doc);
		connect(dia, SIGNAL(saveStyle(StilFormate *)), this, SLOT(saveStyles(StilFormate *)));
		if (dia->exec())
			saveStyles(dia);
		disconnect(dia, SIGNAL(saveStyle(StilFormate *)), this, SLOT(saveStyles(StilFormate *)));
		delete dia;
	}
}

void ScribusMainWindow::saveStyles(StilFormate *dia)
{
	QValueList<uint> ers;
	QString nn;
	PageItem* ite = 0;
	bool ff;
	uint nr;
	bool tabEQ = false;
	ers.clear();
	ers.append(0);
	ers.append(1);
	ers.append(2);
	ers.append(3);
	ers.append(4);
	for (uint a=5; a<doc->docParagraphStyles.count(); ++a)
	{
		ff = false;
		nn = doc->docParagraphStyles[a].Vname;
		for (uint b=0; b<dia->TempVorl.count(); ++b)
		{
			if (nn == dia->TempVorl[b].Vname)
			{
				nr = b;
				ff = true;
				break;
			}
		}
		if (ff)
			ers.append(nr);
		else
		{
			for (uint b=0; b<dia->TempVorl.count(); ++b)
			{
				struct PageItem::TabRecord tb;
				tabEQ = false;
				if ((doc->docParagraphStyles[a].TabValues.count() == 0) && (dia->TempVorl[b].TabValues.count() == 0))
					tabEQ = true;
				else
				{
					for (uint t1 = 0; t1 < dia->TempVorl[b].TabValues.count(); t1++)
					{
						tb.tabPosition = dia->TempVorl[b].TabValues[t1].tabPosition;
						tb.tabType = dia->TempVorl[b].TabValues[t1].tabType;
						tb.tabFillChar = dia->TempVorl[b].TabValues[t1].tabFillChar;
						for (uint t2 = 0; t2 < doc->docParagraphStyles[a].TabValues.count(); t2++)
						{
							struct PageItem::TabRecord tb2;
							tb2.tabPosition = doc->docParagraphStyles[a].TabValues[t2].tabPosition;
							tb2.tabType = doc->docParagraphStyles[a].TabValues[t2].tabType;
							tb2.tabFillChar = doc->docParagraphStyles[a].TabValues[t2].tabFillChar;
							if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
							{
								tabEQ = true;
								break;
							}
						}
						if (tabEQ)
							break;
					}
				}
				if ((doc->docParagraphStyles[a].LineSpa == dia->TempVorl[b].LineSpa) &&
					(doc->docParagraphStyles[a].LineSpaMode == dia->TempVorl[b].LineSpaMode) &&
					(doc->docParagraphStyles[a].Indent == dia->TempVorl[b].Indent) &&
					(doc->docParagraphStyles[a].First == dia->TempVorl[b].First) &&
					(doc->docParagraphStyles[a].textAlignment == dia->TempVorl[b].textAlignment) &&
					(doc->docParagraphStyles[a].gapBefore == dia->TempVorl[b].gapBefore) &&
					(doc->docParagraphStyles[a].gapAfter == dia->TempVorl[b].gapAfter) &&
					(doc->docParagraphStyles[a].Font == dia->TempVorl[b].Font) &&
					(tabEQ) && (doc->docParagraphStyles[a].DropDist == dia->TempVorl[b].DropDist) &&
					(doc->docParagraphStyles[a].Drop == dia->TempVorl[b].Drop) &&
					(doc->docParagraphStyles[a].DropLin == dia->TempVorl[b].DropLin) &&
					(doc->docParagraphStyles[a].FontEffect == dia->TempVorl[b].FontEffect) &&
					(doc->docParagraphStyles[a].FColor == dia->TempVorl[b].FColor) &&
					(doc->docParagraphStyles[a].FShade == dia->TempVorl[b].FShade) &&
					(doc->docParagraphStyles[a].SColor == dia->TempVorl[b].SColor) &&
					(doc->docParagraphStyles[a].SShade == dia->TempVorl[b].SShade) &&
					(doc->docParagraphStyles[a].BaseAdj == dia->TempVorl[b].BaseAdj) &&
					(doc->docParagraphStyles[a].txtShadowX == dia->TempVorl[b].txtShadowX) &&
					(doc->docParagraphStyles[a].txtShadowY == dia->TempVorl[b].txtShadowY) &&
					(doc->docParagraphStyles[a].txtOutline == dia->TempVorl[b].txtOutline) &&
					(doc->docParagraphStyles[a].txtUnderPos == dia->TempVorl[b].txtUnderPos) &&
					(doc->docParagraphStyles[a].txtUnderWidth == dia->TempVorl[b].txtUnderWidth) &&
					(doc->docParagraphStyles[a].txtStrikePos == dia->TempVorl[b].txtStrikePos) &&
					(doc->docParagraphStyles[a].txtStrikeWidth == dia->TempVorl[b].txtStrikeWidth) &&
					(doc->docParagraphStyles[a].scaleH == dia->TempVorl[b].scaleH) &&
					(doc->docParagraphStyles[a].scaleV == dia->TempVorl[b].scaleV) &&
					(doc->docParagraphStyles[a].baseOff == dia->TempVorl[b].baseOff) &&
					(doc->docParagraphStyles[a].kernVal == dia->TempVorl[b].kernVal) &&
					(doc->docParagraphStyles[a].FontSize == dia->TempVorl[b].FontSize))
				{
				nr = b;
				ff = true;
				break;
				}
			}
			if (ff)
				ers.append(nr);
			else
			{
				if (dia->ReplaceList.count() != 0)
				{
					QString ne = dia->ReplaceList[nn];
					if (ne == tr("No Style"))
						ers.append(0);
					else
					{
						for (uint b=0; b<dia->TempVorl.count(); ++b)
						{
							if (ne == dia->TempVorl[b].Vname)
							{
								nr = b;
								ff = true;
								break;
							}
						}
						if (ff)
							ers.append(nr);
						else
							ers.append(0);
					}
				}
				else
					ers.append(0);
			}
		}
	}
	uint counter = 0;
	for (uint lc = 0; lc < 3; ++lc)
	{
		switch (lc)
		{
			case 0:
				counter = doc->MasterItems.count();
				break;
			case 1:
				counter = doc->DocItems.count();
				break;
			case 2:
				counter = doc->FrameItems.count();
				break;
		}
		for (uint d=0; d< counter; ++d)
		{
			switch (lc)
			{
				case 0:
					ite = doc->MasterItems.at(d);
					break;
				case 1:
					ite = doc->DocItems.at(d);
					break;
				case 2:
					ite = doc->FrameItems.at(d);
					break;
			}
			if (ite->asTextFrame())
			{
				for (uint e=0; e<ite->itemText.count(); ++e)
				{
					int cabori = ite->itemText.at(e)->cab;
					int cabneu = ers[cabori];
					if (cabori > 4)
					{
						if (cabneu > 0)
						{
							if (ite->itemText.at(e)->cfont == (*doc->AllFonts)[doc->docParagraphStyles[cabori].Font])
								ite->itemText.at(e)->cfont = (*doc->AllFonts)[dia->TempVorl[cabneu].Font];
							if (ite->itemText.at(e)->csize == doc->docParagraphStyles[cabori].FontSize)
								ite->itemText.at(e)->csize = dia->TempVorl[cabneu].FontSize;
							if ((ite->itemText.at(e)->cstyle & 1919 ) == doc->docParagraphStyles[cabori].FontEffect)
							{
								ite->itemText.at(e)->cstyle &= ~1919;
								ite->itemText.at(e)->cstyle |= dia->TempVorl[cabneu].FontEffect;
							}
							if (ite->itemText.at(e)->ccolor == doc->docParagraphStyles[cabori].FColor)
								ite->itemText.at(e)->ccolor = dia->TempVorl[cabneu].FColor;
							if (ite->itemText.at(e)->cshade == doc->docParagraphStyles[cabori].FShade)
								ite->itemText.at(e)->cshade = dia->TempVorl[cabneu].FShade;
							if (ite->itemText.at(e)->cstroke == doc->docParagraphStyles[cabori].SColor)
								ite->itemText.at(e)->cstroke = dia->TempVorl[cabneu].SColor;
							if (ite->itemText.at(e)->cshade2 == doc->docParagraphStyles[cabori].SShade)
								ite->itemText.at(e)->cshade2 = dia->TempVorl[cabneu].SShade;
							if (ite->itemText.at(e)->cshadowx == doc->docParagraphStyles[cabori].txtShadowX)
								ite->itemText.at(e)->cshadowx = dia->TempVorl[cabneu].txtShadowX;
							if (ite->itemText.at(e)->cshadowy == doc->docParagraphStyles[cabori].txtShadowY)
								ite->itemText.at(e)->cshadowy = dia->TempVorl[cabneu].txtShadowY;
							if (ite->itemText.at(e)->coutline == doc->docParagraphStyles[cabori].txtOutline)
								ite->itemText.at(e)->coutline = dia->TempVorl[cabneu].txtOutline;
							if (ite->itemText.at(e)->cunderpos == doc->docParagraphStyles[cabori].txtUnderPos)
								ite->itemText.at(e)->cunderpos = dia->TempVorl[cabneu].txtUnderPos;
							if (ite->itemText.at(e)->cunderwidth == doc->docParagraphStyles[cabori].txtUnderWidth)
								ite->itemText.at(e)->cunderwidth = dia->TempVorl[cabneu].txtUnderWidth;
							if (ite->itemText.at(e)->cstrikepos == doc->docParagraphStyles[cabori].txtStrikePos)
								ite->itemText.at(e)->cstrikepos = dia->TempVorl[cabneu].txtStrikePos;
							if (ite->itemText.at(e)->cstrikewidth == doc->docParagraphStyles[cabori].txtStrikeWidth)
								ite->itemText.at(e)->cstrikewidth = dia->TempVorl[cabneu].txtStrikeWidth;
							if (ite->itemText.at(e)->cscale == doc->docParagraphStyles[cabori].scaleH)
								ite->itemText.at(e)->cscale = dia->TempVorl[cabneu].scaleH;
							if (ite->itemText.at(e)->cscalev == doc->docParagraphStyles[cabori].scaleV)
								ite->itemText.at(e)->cscalev = dia->TempVorl[cabneu].scaleV;
							if (ite->itemText.at(e)->cbase == doc->docParagraphStyles[cabori].baseOff)
								ite->itemText.at(e)->cbase = dia->TempVorl[cabneu].baseOff;
							if (ite->itemText.at(e)->cextra == doc->docParagraphStyles[cabori].kernVal)
								ite->itemText.at(e)->cextra = dia->TempVorl[cabneu].kernVal;
						}
						else
						{
							ite->itemText.at(e)->ccolor = ite->TxtFill;
							ite->itemText.at(e)->cshade = ite->ShTxtFill;
							ite->itemText.at(e)->cstroke = ite->TxtStroke;
							ite->itemText.at(e)->cshade2 = ite->ShTxtStroke;
							ite->itemText.at(e)->csize = ite->fontSize();
							ite->itemText.at(e)->cstyle &= ~1919;
							ite->itemText.at(e)->cstyle |= ite->TxTStyle;
							ite->itemText.at(e)->cshadowx = ite->TxtShadowX;
							ite->itemText.at(e)->cshadowy = ite->TxtShadowY;
							ite->itemText.at(e)->coutline = ite->TxtOutline;
							ite->itemText.at(e)->cunderpos = ite->TxtUnderPos;
							ite->itemText.at(e)->cunderwidth = ite->TxtUnderWidth;
							ite->itemText.at(e)->cstrikepos = ite->TxtStrikePos;
							ite->itemText.at(e)->cstrikewidth = ite->TxtStrikeWidth;
							ite->itemText.at(e)->cscale = ite->TxtScale;
							ite->itemText.at(e)->cscalev = ite->TxtScaleV;
							ite->itemText.at(e)->cbase = ite->TxtBase;
							ite->itemText.at(e)->cextra = ite->ExtraV;
						}
						ite->itemText.at(e)->cab = cabneu;
					}
				}
			}
		}
	}
	if (CurrStED != NULL)
	{
		if (CurrStED->Editor->StyledText.count() != 0)
		{
			for (uint pa = 0; pa < CurrStED->Editor->StyledText.count(); ++pa)
			{
				SEditor::ChList *chars;
				chars = CurrStED->Editor->StyledText.at(pa);
				(*CurrStED->Editor->ParagStyles.at(pa)) = ers[CurrStED->Editor->ParagStyles[pa]];
				int cabneu = 0;
				for (uint e = 0; e < chars->count(); ++e)
				{
					int cabori = chars->at(e)->cab;
					cabneu = ers[cabori];
					if (cabori > 4)
					{
						if (cabneu > 0)
						{
							if (chars->at(e)->cfont == doc->docParagraphStyles[cabori].Font)
								chars->at(e)->cfont = dia->TempVorl[cabneu].Font;
							if (chars->at(e)->csize == doc->docParagraphStyles[cabori].FontSize)
								chars->at(e)->csize = dia->TempVorl[cabneu].FontSize;
							if ((chars->at(e)->cstyle & 1919 ) == doc->docParagraphStyles[cabori].FontEffect)
							{
								chars->at(e)->cstyle &= ~1919;
								chars->at(e)->cstyle |= dia->TempVorl[cabneu].FontEffect;
							}
							if (chars->at(e)->ccolor == doc->docParagraphStyles[cabori].FColor)
								chars->at(e)->ccolor = dia->TempVorl[cabneu].FColor;
							if (chars->at(e)->cshade == doc->docParagraphStyles[cabori].FShade)
								chars->at(e)->cshade = dia->TempVorl[cabneu].FShade;
							if (chars->at(e)->cstroke == doc->docParagraphStyles[cabori].SColor)
								chars->at(e)->cstroke = dia->TempVorl[cabneu].SColor;
							if (chars->at(e)->cshade2 == doc->docParagraphStyles[cabori].SShade)
								chars->at(e)->cshade2 = dia->TempVorl[cabneu].SShade;
							if (chars->at(e)->cshadowx == doc->docParagraphStyles[cabori].txtShadowX)
								chars->at(e)->cshadowx = dia->TempVorl[cabneu].txtShadowX;
							if (chars->at(e)->cshadowy == doc->docParagraphStyles[cabori].txtShadowY)
								chars->at(e)->cshadowy = dia->TempVorl[cabneu].txtShadowY;
							if (chars->at(e)->coutline == doc->docParagraphStyles[cabori].txtOutline)
								chars->at(e)->coutline = dia->TempVorl[cabneu].txtOutline;
							if (chars->at(e)->cunderpos == doc->docParagraphStyles[cabori].txtUnderPos)
								chars->at(e)->cunderpos = dia->TempVorl[cabneu].txtUnderPos;
							if (chars->at(e)->cunderwidth == doc->docParagraphStyles[cabori].txtUnderWidth)
								chars->at(e)->cunderwidth = dia->TempVorl[cabneu].txtUnderWidth;
							if (chars->at(e)->cstrikepos == doc->docParagraphStyles[cabori].txtStrikePos)
								chars->at(e)->cstrikepos = dia->TempVorl[cabneu].txtStrikePos;
							if (chars->at(e)->cstrikewidth == doc->docParagraphStyles[cabori].txtStrikeWidth)
								chars->at(e)->cstrikewidth = dia->TempVorl[cabneu].txtStrikeWidth;
							if (chars->at(e)->cscale == doc->docParagraphStyles[cabori].scaleH)
								chars->at(e)->cscale = dia->TempVorl[cabneu].scaleH;
							if (chars->at(e)->cscalev == doc->docParagraphStyles[cabori].scaleV)
								chars->at(e)->cscalev = dia->TempVorl[cabneu].scaleV;
							if (chars->at(e)->cbase == doc->docParagraphStyles[cabori].baseOff)
								chars->at(e)->cbase = dia->TempVorl[cabneu].baseOff;
							if (chars->at(e)->cextra == doc->docParagraphStyles[cabori].kernVal)
								chars->at(e)->cextra = dia->TempVorl[cabneu].kernVal;
						}
						else
						{
							chars->at(e)->ccolor = ite->TxtFill;
							chars->at(e)->cshade = ite->ShTxtFill;
							chars->at(e)->cstroke = ite->TxtStroke;
							chars->at(e)->cshade2 = ite->ShTxtStroke;
							chars->at(e)->csize = ite->fontSize();
							chars->at(e)->cstyle &= ~1919;
							chars->at(e)->cstyle |= ite->TxTStyle;
							chars->at(e)->cshadowx = ite->TxtShadowX;
							chars->at(e)->cshadowy = ite->TxtShadowY;
							chars->at(e)->coutline = ite->TxtOutline;
							chars->at(e)->cunderpos = ite->TxtUnderPos;
							chars->at(e)->cunderwidth = ite->TxtUnderWidth;
							chars->at(e)->cstrikepos = ite->TxtStrikePos;
							chars->at(e)->cstrikewidth = ite->TxtStrikeWidth;
							chars->at(e)->cscale = ite->TxtScale;
							chars->at(e)->cscalev = ite->TxtScaleV;
							chars->at(e)->cbase = ite->TxtBase;
							chars->at(e)->cextra = ite->ExtraV;
						}
						chars->at(e)->cab = cabneu;
					}
				}
			}
			CurrStED->Editor->currentParaStyle = ers[CurrStED->Editor->currentParaStyle];
		}
	}
	doc->docParagraphStyles = dia->TempVorl;
	if (CurrStED != NULL)
	{
		if (CurrStED->Editor->StyledText.count() != 0)
			CurrStED->Editor->updateAll();
	}
	for (uint a=0; a<doc->docParagraphStyles.count(); ++a)
	{
		if (!doc->docParagraphStyles[a].Font.isEmpty())
		{
			QString nf = doc->docParagraphStyles[a].Font;
			if (!doc->UsedFonts.contains(nf))
			{
				if (doc->AddFont(nf)) //, prefsManager->appPrefs.AvailFonts[nf]->Font))
				{
					int ff = FontMenu->insertItem(new FmItem(nf, prefsManager->appPrefs.AvailFonts[nf]));
					FontID.insert(ff, prefsManager->appPrefs.AvailFonts[nf]->scName());
				}
				else
					doc->docParagraphStyles[a].Font = doc->toolSettings.defFont;
			}
		}
	}
	propertiesPalette->Spal->updateFormatList();
	propertiesPalette->updateColorList();
	propertiesPalette->updateCList();
	disconnect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
	ColorList::Iterator it;
	updateColorMenu();
	view->DrawNew();
	slotDocCh();
}

void ScribusMainWindow::setNewAbStyle(int a)
{
	setActiveWindow();
	if (HaveDoc)
	{
		doc->currentParaStyle = a;
		doc->SetAbStyle(a);
		propertiesPalette->setAli(a);
		PageItem *currItem = doc->selection->itemAt(0);
		setTBvals(currItem);
		slotDocCh();
	}
}

void ScribusMainWindow::setAbsValue(int a)
{
	doc->currentParaStyle = a;
	propertiesPalette->setAli(a);
	QString alignment[] = {"Left", "Center", "Right", "Block", "Forced"};
	for (int b=0; b<5; ++b)
	{
		QString actionName="align"+alignment[b];
		if (scrActions[actionName])
			scrActions[actionName]->setOn(a==b);
	}
}

void ScribusMainWindow::slotEditColors()
{
	ColorList edc;
	if (HaveDoc)
		edc = doc->PageColors;
	else
		edc = prefsManager->colorSet();
	ColorManager* dia = new ColorManager(this, edc, HaveDoc, prefsManager->colorSetName(), prefsManager->appPrefs.CustomColorSets);
	if (dia->exec())
	{
		if (HaveDoc)
		{
			uint c, d;
			QMap<QString,QString> ers;
			PageItem *ite;
			QColor tmpc;
			slotDocCh();
			doc->PageColors = dia->EditColors;
			propertiesPalette->updateColorList();
			propertiesPalette->updateCList();
			updateColorMenu();
			ers = dia->replaceMap;
			if (!ers.isEmpty())
			{
				QMap<QString,QString>::Iterator it;
				for (it = ers.begin(); it != ers.end(); ++it)
				{
					if (it.key() == doc->CurrTextFill)
						doc->CurrTextFill = it.data();
					if (it.key() == doc->CurrTextStroke)
						doc->CurrTextStroke = it.data();
					for (c=0; c<doc->DocItems.count(); ++c)
					{
						ite = doc->DocItems.at(c);
						if ((ite->asTextFrame()) || (ite->asPathText()))
						{
							for (d=0; d<ite->itemText.count(); ++d)
							{
								if (it.key() == ite->itemText.at(d)->ccolor)
									ite->itemText.at(d)->ccolor = it.data();
								if (it.key() == ite->itemText.at(d)->cstroke)
									ite->itemText.at(d)->cstroke = it.data();
							}
						}
						if (it.key() == ite->fillColor())
							ite->setFillColor(it.data());
						if (it.key() == ite->lineColor())
							ite->setLineColor(it.data());
						QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
						for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
						{
							if (it.key() == cstops.at(cst)->name)
							{
								ite->SetFarbe(&tmpc, it.data(), cstops.at(cst)->shade);
								cstops.at(cst)->color = tmpc;
								cstops.at(cst)->name = it.data();
							}
						}
					}
				}
				for (it = ers.begin(); it != ers.end(); ++it)
				{
					for (c=0; c<doc->FrameItems.count(); ++c)
					{
						ite = doc->FrameItems.at(c);
						if ((ite->asTextFrame()) || (ite->asPathText()))
						{
							for (d=0; d<ite->itemText.count(); ++d)
							{
								if (it.key() == ite->itemText.at(d)->ccolor)
									ite->itemText.at(d)->ccolor = it.data();
								if (it.key() == ite->itemText.at(d)->cstroke)
									ite->itemText.at(d)->cstroke = it.data();
							}
						}
						if (it.key() == ite->fillColor())
							ite->setFillColor(it.data());
						if (it.key() == ite->lineColor())
							ite->setLineColor(it.data());
						QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
						for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
						{
							if (it.key() == cstops.at(cst)->name)
							{
								ite->SetFarbe(&tmpc, it.data(), cstops.at(cst)->shade);
								cstops.at(cst)->color = tmpc;
								cstops.at(cst)->name = it.data();
							}
						}
					}
				}
				for (it = ers.begin(); it != ers.end(); ++it)
				{
					for (c=0; c<doc->MasterItems.count(); ++c)
					{
						ite = doc->MasterItems.at(c);
						if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
						{
							for (d=0; d<ite->itemText.count(); ++d)
							{
								if (it.key() == ite->itemText.at(d)->ccolor)
									ite->itemText.at(d)->ccolor = it.data();
								if (it.key() == ite->itemText.at(d)->cstroke)
									ite->itemText.at(d)->cstroke = it.data();
							}
						}
						if (it.key() == ite->fillColor())
							ite->setFillColor(it.data());
						if (it.key() == ite->lineColor())
							ite->setLineColor(it.data());
						QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
						for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
						{
							if (it.key() == cstops.at(cst)->name)
							{
								ite->SetFarbe(&tmpc, it.data(), cstops.at(cst)->shade);
								cstops.at(cst)->color = tmpc;
								cstops.at(cst)->name = it.data();
							}
						}
					}
				}
			}
			doc->updateAllItemQColors();
			view->DrawNew();
		}
		else
		{
			prefsManager->setColorSet(dia->EditColors);
			prefsManager->setColorSetName(dia->getColorSetName());
			propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
		}
	}
	if (!HaveDoc)
		prefsManager->appPrefs.CustomColorSets = dia->customColSet;
	delete dia;
}

void ScribusMainWindow::setPenFarbe(QString farbe)
{
	if (HaveDoc)
	{
		doc->ItemPen(farbe);
		slotDocCh();
	}
}

void ScribusMainWindow::setPenShade(int sh)
{
	//setActiveWindow();
	if (HaveDoc)
	{
		doc->ItemPenShade(sh);
		slotDocCh();
	}
}

void ScribusMainWindow::setBrushFarbe(QString farbe)
{
	if (HaveDoc)
	{
		doc->ItemBrush(farbe);
		slotDocCh();
	}
}

void ScribusMainWindow::setBrushShade(int sh)
{
	//setActiveWindow();
	if (HaveDoc)
	{
		doc->ItemBrushShade(sh);
		slotDocCh();
	}
}

void ScribusMainWindow::setGradFill(int typ)
{
	if (HaveDoc)
	{
		doc->ItemGradFill(typ);
		slotDocCh();
	}
}

void ScribusMainWindow::updtGradFill()
{
	if (HaveDoc)
	{
		if (doc->selection->count() != 0)
		{
			PageItem *currItem = doc->selection->itemAt(0);
			currItem->fill_gradient = propertiesPalette->getFillGradient();
			view->RefreshItem(currItem);
			slotDocCh();
		}
	}
}

void ScribusMainWindow::GetBrushPen()
{
	//What? we come back here from mpalette and then go to the view.. someones kidding

	//why this.. ugh. setActiveWindow();
	//
	if (HaveDoc)
	{
		view->QueryFarben();
		slotDocCh();
	}
}

void ScribusMainWindow::MakeFrame(int f, int c, double *vals)
{
	PageItem *currItem = doc->selection->itemAt(0);
	switch (f)
	{
	case 0:
		currItem->SetRectFrame();
		view->setRedrawBounding(currItem);
		break;
	case 1:
		currItem->SetOvalFrame();
		view->setRedrawBounding(currItem);
		break;
	default:
		currItem->SetFrameShape(c, vals);
		view->setRedrawBounding(currItem);
		currItem->FrameType = f+2;
		break;
	}
	//propertiesPalette->SetCurItem(currItem);
	view->RefreshItem(currItem);
	slotDocCh();
}

void ScribusMainWindow::DeleteObjekt()
{
	if (HaveDoc)
		if (!doc->EditClip)
			view->DeleteItem();
}

void ScribusMainWindow::ObjektDup()
{
	slotSelect();
	bool savedAlignGrid = doc->useRaster;
	bool savedAlignGuides = doc->SnapGuides;
	doc->useRaster = false;
	doc->SnapGuides = false;
	slotEditCopy();
	view->Deselect(true);
	slotEditPaste();
	for (uint b=0; b<doc->selection->count(); ++b)
	{
		doc->selection->itemAt(b)->setLocked(false);
		view->MoveItem(DispX, DispY, doc->selection->itemAt(b));
	}
	doc->useRaster = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
}

void ScribusMainWindow::ObjektDupM()
{
	slotSelect();
	NoFrameEdit();
	Mdup *dia = new Mdup(this, DispX * doc->unitRatio(), DispY * doc->unitRatio(), doc->unitIndex());
	if (dia->exec())
	{
		bool savedAlignGrid = doc->useRaster;
		bool savedAlignGuides = doc->SnapGuides;
		doc->useRaster = false;
		doc->SnapGuides = false;
		int anz = dia->Ncopies->value();
		double dH = dia->ShiftH->value() / doc->unitRatio();
		double dV = dia->ShiftV->value() / doc->unitRatio();
		double dH2 = dH;
		double dV2 = dV;
		int a;
		if (anz>0)
		{
			slotEditCopy();
			view->Deselect(true);
			for (a=0; a<anz; ++a)
			{
				slotEditPaste();
				for (uint b=0; b<doc->selection->count(); ++b)
				{
					PageItem* bItem=doc->selection->itemAt(b);
					bItem->setLocked(false);
					view->MoveItem(dH2, dV2, bItem, true);
				}
				dH2 += dH;
				dV2 += dV;
			}
			DispX = dH;
			DispY = dV;
			slotDocCh();
			view->Deselect(true);
		}
		doc->useRaster = savedAlignGrid;
		doc->SnapGuides = savedAlignGuides;
	}
	delete dia;
}

void ScribusMainWindow::selectItemsFromOutlines(int Page, int Item, bool single)
{
	NoFrameEdit();
	setActiveWindow();
	view->Deselect(true);
	if ((Page != -1) && (Page != static_cast<int>(doc->currentPage->pageNr())))
		view->GotoPage(Page);
	view->SelectItemNr(Item, true, single);
	if (doc->selection->count() != 0)
	{
		PageItem *currItem = doc->selection->itemAt(0);
	 // jjsa 23-05-2004 added for centering of rotated objects
	 	double rotation=currItem->rotation();
		if ( rotation != 0.0 )
		{
			double MPI180=1.0/(180.0*M_PI);
			double y1 = sin(rotation*MPI180) * currItem->width();
			double x1 = cos(rotation*MPI180) * currItem->width();
			double y2 = sin((rotation+90.0)*MPI180) * currItem->height();
			double x2 = cos((rotation+90.0)*MPI180) * currItem->height();
			double mx = currItem->xPos() + ((x1 + x2)/2.0);
			double my = currItem->yPos() + ((y1 + y2)/2.0);
			double viewScale=view->scale();
			if ((qRound((currItem->xPos() + QMAX(x1, x2)) * viewScale) > view->contentsWidth()) ||
				(qRound((currItem->yPos() + QMAX(y1, y2)) * viewScale) > view->contentsHeight()))
				view->resizeContents(QMAX(qRound((currItem->xPos() + QMAX(x1, x2)) * viewScale),
									view->contentsWidth()),
									QMAX(qRound((currItem->yPos() + QMAX(y1, y2)) * viewScale), view->contentsHeight()));
			view->SetCCPo(static_cast<int>(mx), static_cast<int>(my));
		}
		else
		{
			double viewScale=view->scale();
			if ((qRound((currItem->xPos() + currItem->width()) * viewScale) > view->contentsWidth()) ||
				(qRound((currItem->yPos() + currItem->height()) * viewScale) > view->contentsHeight())
				)
				view->resizeContents(QMAX(qRound((currItem->xPos() + currItem->width()) * viewScale), view->contentsWidth()),
									 QMAX(qRound((currItem->yPos() + currItem->height()) * viewScale), view->contentsHeight()));
			view->SetCCPo(static_cast<int>(currItem->xPos() + currItem->width()/2), static_cast<int>(currItem->yPos() + currItem->height()/2));
		}
	}
}

void ScribusMainWindow::selectPagesFromOutlines(int Page)
{
	NoFrameEdit();
	setActiveWindow();
	view->Deselect(true);
	if (Page < 0)
		return;
	view->GotoPage(Page);
}

void ScribusMainWindow::buildFontMenu()
{
	FontID.clear();
	FontMenu->clear();
	int a;
	QString b = " ";
	SCFontsIterator it(prefsManager->appPrefs.AvailFonts);
	FontSub = new FontCombo(0);
	FontMenu->insertItem(FontSub);
	connect(FontSub, SIGNAL(activated(int)), this, SLOT(setItemFont2(int)));
	FontMenu->insertSeparator();
	if (!HaveDoc)
	{
		it.toFirst();
		a = FontMenu->insertItem(new FmItem(it.currentKey(), it.current()));
		FontMenu->setItemChecked(a, true);
		FontID.insert(a, it.current()->scName());
	}
	else
	{
		QMap<QString,int>::Iterator it3;
		for (it3 = doc->UsedFonts.begin(); it3 != doc->UsedFonts.end(); ++it3)
		{
			a = FontMenu->insertItem(new FmItem(it3.key(), prefsManager->appPrefs.AvailFonts[it3.key()]));
			if (it3.key() == doc->toolSettings.defFont)
				FontMenu->setItemChecked(a, true);
			FontID.insert(a, it3.key());
		}
	}
	connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
}

void ScribusMainWindow::slotPrefsOrg()
{
	//reset the appMode so we restore our tools shortcuts
	QString oldGUILanguage=prefsManager->guiLanguage();
	QString oldGUIStyle=prefsManager->guiStyle();
	int oldGUIFontSize=prefsManager->guiFontSize();
	//double oldDisplayScale=prefsManager->displayScale();
	setAppMode(modeNormal);

	Preferences *dia = new Preferences(this);
	if (dia->exec())
	{
		dia->updatePreferences();
		prefsManager->SavePrefs();
		DocDir = prefsManager->documentDir();
		scrapbookPalette->rebuildView();
		scrapbookPalette->AdjustMenu();
		QString newGUILanguage=prefsManager->guiLanguage();
		if (oldGUILanguage!=newGUILanguage)
			ScQApp->changeGUILanguage(newGUILanguage);
		QString newGUIStyle=prefsManager->guiStyle();
		if (oldGUIStyle != newGUIStyle)
			qApp->setStyle(QStyleFactory::create(newGUIStyle));
		int newGUIFontSize=prefsManager->guiFontSize();
		if (oldGUIFontSize!=newGUIFontSize)
		{
			QFont apf = qApp->font();
			apf.setPointSize(prefsManager->appPrefs.AppFontSize);
			qApp->setFont(apf,true);
		}

		//double newDisplayScale=prefsManager->displayScale();
		//if (oldDisplayScale != newDisplayScale)
		//	zChange = true;

		propertiesPalette->Cpal->UseTrans(true);
		FontSub->RebuildList(0);
		propertiesPalette->Fonts->RebuildList(0);

		GetCMSProfiles();
		SetShortCut();
		emit prefsChanged();
	}
	delete dia;
}

void ScribusMainWindow::ShowSubs()
{
	QString mess;
	if (HaveGS != 0)
	{
		mess = tr("The following programs are missing:")+"\n\n";
#ifndef _WIN32
		mess += tr("Ghostscript : You cannot use EPS images or Print Preview")+"\n\n";
#else
		mess += tr("Ghostscript : You cannot use EPS images or PostScript Print Preview")+"\n\n";
#endif
		QMessageBox::warning(this, CommonStrings::trWarning, mess, 1, 0, 0);
	}

	propertiesPalette->startup();
	outlinePalette->startup();
	scrapbookPalette->startup();
	bookmarkPalette->startup();
	pagePalette->startup();
	layerPalette->startup();
	measurementPalette->startup();
	docCheckerPalette->startup();
	alignDistributePalette->startup();
	undoPalette->startup();

	setActiveWindow();
	raise();
}

void ScribusMainWindow::doPrintPreview()
{
	if (docCheckerPalette->isIgnoreEnabled())
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = CheckDocument::checkNULL;
		docCheckerPalette->setIgnoreEnabled(false);
		scrActions["toolsPreflightVerifier"]->setOn(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doPrintPreview()));
	}
	if (HaveDoc)
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
		QString currentPrinter = prefs->get("CurrentPrn");
		if ( PPreview::usePostscriptPreview(currentPrinter) && ( HaveGS != 0 ) )
		{
			QString mess = tr("Ghostscript is missing : Postscript Print Preview is not available")+"\n\n";
			QMessageBox::warning(this, CommonStrings::trWarning, mess, 1, 0, 0);
			return;
		}
		PPreview *dia = new PPreview(this, view, doc, HavePngAlpha, HaveTiffSep, currentPrinter);
		previewDinUse = true;
		connect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
		dia->exec();
		PrefsManager *prefsManager=PrefsManager::instance();
		prefsManager->appPrefs.PrPr_Mode = dia->EnableCMYK->isChecked();
		prefsManager->appPrefs.PrPr_AlphaText = dia->AliasText->isChecked();
		prefsManager->appPrefs.PrPr_AlphaGraphics = dia->AliasGr->isChecked();
		prefsManager->appPrefs.PrPr_Transparency = dia->AliasTr->isChecked();
		if ( HaveTiffSep != 0 || !dia->postscriptPreview )
		{
			prefsManager->appPrefs.PrPr_C = dia->EnableCMYK_C->isChecked();
			prefsManager->appPrefs.PrPr_M = dia->EnableCMYK_M->isChecked();
			prefsManager->appPrefs.PrPr_Y = dia->EnableCMYK_Y->isChecked();
			prefsManager->appPrefs.PrPr_K = dia->EnableCMYK_K->isChecked();
		}
		else
		{
			prefsManager->appPrefs.PrPr_C = dia->flagsVisible["Cyan"]->isChecked();
			prefsManager->appPrefs.PrPr_M = dia->flagsVisible["Magenta"]->isChecked();
			prefsManager->appPrefs.PrPr_Y = dia->flagsVisible["Yellow"]->isChecked();
			prefsManager->appPrefs.PrPr_K = dia->flagsVisible["Black"]->isChecked();
		}
		prefsManager->appPrefs.Gcr_Mode = dia->EnableGCR->isChecked();
		disconnect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
		previewDinUse = false;
		delete dia;
		QFile::remove(PrefsPfad+"/tmp.ps");
		QFile::remove(PrefsPfad+"/sc.png");
		QDir d(PrefsPfad+"/", "sc.tif*", QDir::Name, QDir::Files | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			for (uint dc = 0; dc < d.count(); dc++)
			{
				QFile::remove(PrefsPfad +"/" + d[dc]);
			}
		}
	}
}

void ScribusMainWindow::printPreview()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		scanDocument();
		if ((doc->docItemErrors.count() != 0) || (doc->masterItemErrors.count() != 0))
		{
			if (doc->checkerProfiles[doc->curCheckProfile].ignoreErrors)
			{
				int t = ScMessageBox::warning(this, CommonStrings::trWarning,
											"<qt>"+tr("Scribus has detected some errors. Consider using the Preflight Verifier to correct them")+"</qt>",
											tr("&Ignore"), tr("&Abort"), 0, 0, 0);
				if (t == 1)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doPrintPreview()));
				docCheckerPalette->setIgnoreEnabled(true);
				docCheckerPalette->checkMode = CheckDocument::checkPrintPreview;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setOn(true);
				return;
			}
		}
	}
	doPrintPreview();
}

bool ScribusMainWindow::DoSaveAsEps(QString fn)
{
	QStringList spots;
	bool return_value = true;
	std::vector<int> pageNs;
	pageNs.push_back(doc->currentPage->pageNr()+1);
	ReOrderText(doc, view);
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QMap<QString,int> ReallyUsed;
	ReallyUsed.clear();
	doc->getUsedFonts(&ReallyUsed);
	ColorList usedColors;
	doc->getUsedColors(usedColors);
	fileWatcher->forceScan();
	fileWatcher->stop();
	PSLib *dd = new PSLib(false, prefsManager->appPrefs.AvailFonts, ReallyUsed, usedColors, false, true);
	if (dd != NULL)
	{
		if (dd->PS_set_file(fn))
			dd->CreatePS(doc, /*view, */pageNs, false, tr("All"), spots, true, false, false, true, prefsManager->appPrefs.GCRMode, false);
		else
			return_value = false;
		delete dd;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	fileWatcher->start();
	return return_value;
}

void ScribusMainWindow::SaveAsEps()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		scanDocument();
		if ((doc->docItemErrors.count() != 0) || (doc->masterItemErrors.count() != 0))
		{
			if (doc->checkerProfiles[doc->curCheckProfile].ignoreErrors)
			{
				int t = ScMessageBox::warning(this, CommonStrings::trWarning,
											tr("Scribus detected some errors.\nConsider using the Preflight Verifier  to correct them."),
											tr("&Abort"), tr("&Ignore"), 0, 0, 0);
				if (t == 0)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(reallySaveAsEps()));
				docCheckerPalette->setIgnoreEnabled(true);
				docCheckerPalette->checkMode = CheckDocument::checkEPS;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setOn(true);
				return;
			}
		}
	}
	reallySaveAsEps();
}

void ScribusMainWindow::reallySaveAsEps()
{
	QString fna;
	if (docCheckerPalette->isIgnoreEnabled())
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = CheckDocument::checkNULL;
		docCheckerPalette->setIgnoreEnabled(false);
		scrActions["toolsPreflightVerifier"]->setOn(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(reallySaveAsEps()));
	}
	if (!doc->DocName.startsWith(tr("Document")))
	{
		QFileInfo fi(doc->DocName);
		fna = fi.dirPath() + "/" + getFileNameByPage(doc->currentPage->pageNr(), "eps");
	}
	else
	{
		QDir di = QDir();
		fna = di.currentDirPath() + "/" + getFileNameByPage(doc->currentPage->pageNr(), "eps");
	}
	fna = QDir::convertSeparators(fna);
	QString wdir = ".";
	QString prefsDocDir=prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		wdir = dirs->get("eps", prefsDocDir);
	else
		wdir = dirs->get("eps", ".");
	QString fn = CFileDialog( wdir, tr("Save as"), tr("EPS Files (*.eps);;All Files (*)"), fna, false, false);
	if (!fn.isEmpty())
	{
		dirs->set("eps", fn.left(fn.findRev("/")));
		if (overwrite(this, fn))
		{
			if (!DoSaveAsEps(fn))
				QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg(fn), CommonStrings::tr_OK);
		}
	}
}

bool ScribusMainWindow::getPDFDriver(const QString & fn, const QString & nam, int Components,
									 const std::vector<int> & pageNs, const QMap<int,QPixmap> & thumbs)
{
	fileWatcher->forceScan();
	fileWatcher->stop();
	bool ret = PDFlib(*doc).doExport(fn, nam, Components, pageNs, thumbs);
	fileWatcher->start();
	return ret;
}

void ScribusMainWindow::SaveAsPDF()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		scanDocument();
		if ((doc->docItemErrors.count() != 0) || (doc->masterItemErrors.count() != 0))
		{
			if (doc->checkerProfiles[doc->curCheckProfile].ignoreErrors)
			{
				int t = QMessageBox::warning(this, CommonStrings::trWarning,
											tr("Detected some errors.\nConsider using the Preflight Verifier to correct them"),
											tr("&Abort"), tr("&Ignore"), 0, 0, 0);
				if (t == 0)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doSaveAsPDF()));
				docCheckerPalette->setIgnoreEnabled(true);
				docCheckerPalette->checkMode = CheckDocument::checkPDF;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setOn(true);
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
		scrActions["toolsPreflightVerifier"]->setOn(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doSaveAsPDF()));
	}
/*	if (bookmarkPalette->BView->childCount() == 0)
		doc->PDF_Options.Bookmarks = false; */
	QMap<QString,int> ReallyUsed;
	ReallyUsed.clear();
	doc->getUsedFonts(&ReallyUsed);
	if (doc->PDF_Options.EmbedList.count() != 0)
	{
		QValueList<QString> tmpEm;
		QValueList<QString>::Iterator itef;
		for (itef = doc->PDF_Options.EmbedList.begin(); itef != doc->PDF_Options.EmbedList.end(); ++itef)
		{
			if (ReallyUsed.contains((*itef)))
				tmpEm.append((*itef));
		}
		doc->PDF_Options.EmbedList = tmpEm;
	}
	if (doc->PDF_Options.SubsetList.count() != 0)
	{
		QValueList<QString> tmpEm;
		QValueList<QString>::Iterator itef;
		for (itef = doc->PDF_Options.SubsetList.begin(); itef != doc->PDF_Options.SubsetList.end(); ++itef)
		{
			if (ReallyUsed.contains((*itef)))
				tmpEm.append((*itef));
		}
		doc->PDF_Options.SubsetList = tmpEm;
	}
	PDFExportDialog dia(this, doc->DocName, ReallyUsed, view, doc->PDF_Options, doc->PDF_Options.PresentVals, PDFXProfiles, prefsManager->appPrefs.AvailFonts, doc->unitRatio(), ScMW->PrinterProfiles);
	if (dia.exec())
	{
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		dia.updateDocOptions();
		ReOrderText(doc, view);
		QString pageString(dia.getPagesString());
		std::vector<int> pageNs;
		uint pageNumbersSize;
		QMap<int,QPixmap> thumbs;
		int components=dia.colorSpaceComponents();
		QString nam(dia.cmsDescriptor());
		QString fileName = doc->PDF_Options.Datei;
		parsePagesString(pageString, &pageNs, doc->DocPages.count());
		if (doc->PDF_Options.doMultiFile)
		{
			QFileInfo fi(fileName);
			QString ext = fi.extension( false );
			QString path = fi.dirPath( true );
			QString name = fi.baseName( true );
			uint aa = 0;
			while (aa < pageNs.size())
			{
				ReallyUsed.clear();
				doc->getUsedFonts(&ReallyUsed);
				thumbs.clear();
				std::vector<int> pageNs2;
				pageNs2.clear();
				pageNs2.push_back(pageNs[aa]);
				pageNumbersSize = pageNs2.size();
				QPixmap pm(10,10);
				if (doc->PDF_Options.Thumbnails)
					pm.convertFromImage(view->PageToPixmap(pageNs[aa]-1, 100));
				thumbs.insert(1, pm);
				QString realName = QDir::convertSeparators(path+"/"+name+ tr("-Page%1").arg(pageNs[aa])+"."+ext);
				if (!getPDFDriver(realName, nam, components, pageNs2, thumbs))
				{
					qApp->setOverrideCursor(QCursor(arrowCursor), true);
					QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg(doc->PDF_Options.Datei), CommonStrings::tr_OK);
					return;
				}
				aa++;
			}
		}
		else
		{
			pageNumbersSize = pageNs.size();
			for (uint ap = 0; ap < pageNumbersSize; ++ap)
			{
				QPixmap pm(10,10);
				if (doc->PDF_Options.Thumbnails)
					pm.convertFromImage(view->PageToPixmap(pageNs[ap]-1, 100));
				thumbs.insert(pageNs[ap], pm);
			}
			if (!getPDFDriver(fileName, nam, components, pageNs, thumbs))
			{
				qApp->setOverrideCursor(QCursor(arrowCursor), true);
				QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg(doc->PDF_Options.Datei), CommonStrings::tr_OK);
			}
		}
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
}

void ScribusMainWindow::AddBookMark(PageItem *ite)
{
	bookmarkPalette->BView->AddPageItem(ite);
	StoreBookmarks();
}

void ScribusMainWindow::DelBookMark(PageItem *ite)
{
	bookmarkPalette->BView->DeleteItem(ite->BMnr);
	StoreBookmarks();
}

void ScribusMainWindow::BookMarkTxT(PageItem *ite)
{
	bookmarkPalette->BView->ChangeText(ite);
	StoreBookmarks();
}

void ScribusMainWindow::ChBookmarks(int /*s*/, int /*e*/, int /*n*/)
{
//	view->Pages.at(s)->Items.at(e)->BMnr = n;
}

void ScribusMainWindow::RestoreBookMarks()
{
	QValueList<ScribusDoc::BookMa>::Iterator it2 = doc->BookMarks.begin();
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
			QListViewItemIterator it3(bookmarkPalette->BView);
			for ( ; it3.current(); ++it3)
			{
				ip = (BookMItem*)it3.current();
				if ((*it2).Parent == ip->ItemNr)
				{
					ip2 = ip;
					break;
				}
			}
			if ((*it2).Prev == 0)
			{
				(void) new BookMItem(ip2, &(*it2));
				bookmarkPalette->BView->NrItems++;
			}
			else
			{
				QListViewItemIterator it4(bookmarkPalette->BView);
				for ( ; it4.current(); ++it4)
				{
					ip = (BookMItem*)it4.current();
					if ((*it2).Prev == ip->ItemNr)
					{
						ip3 = ip;
						break;
					}
				}
				(void) new BookMItem(ip2, ip3, &(*it2));
				bookmarkPalette->BView->NrItems++;
			}
		}
	}
	bookmarkPalette->BView->Last = bookmarkPalette->BView->NrItems;
}

void ScribusMainWindow::StoreBookmarks()
{
	doc->BookMarks.clear();
	BookMItem* ip;
	QListViewItemIterator it(bookmarkPalette->BView);
	struct ScribusDoc::BookMa Boma;
	for ( ; it.current(); ++it)
	{
		ip = (BookMItem*)it.current();
		Boma.Title = ip->Titel;
		Boma.Text = ip->text(0);
		Boma.Aktion = ip->Action;
		Boma.ItemNr = ip->ItemNr;
		Boma.Seite = ip->Seite;
		Boma.Element = ip->Element;
		Boma.Parent = ip->Pare;
		Boma.First = ip->First;
		Boma.Prev = ip->Prev;
		Boma.Next = ip->Next;
		Boma.Last = ip->Last;
		doc->BookMarks.append(Boma);
	}
	doc->NrItems = bookmarkPalette->BView->NrItems;
	doc->First = bookmarkPalette->BView->First;
	doc->Last = bookmarkPalette->BView->Last;
}

void ScribusMainWindow::slotElemRead(QString Name, double x, double y, bool art, bool loca, ScribusDoc* docc, ScribusView* vie)
{
	if (doc == docc)
		NoFrameEdit();
	ScriXmlDoc *ss = new ScriXmlDoc();
	if(ss->ReadElem(Name, prefsManager->appPrefs.AvailFonts, docc, x, y, art, loca, prefsManager->appPrefs.GFontSub, vie))
	{
		vie->DrawNew();
		if (doc == docc)
		{
			doc->OpenNodes = outlinePalette->buildReopenVals();
			buildFontMenu();
			propertiesPalette->updateColorList();
			propertiesPalette->updateCList();
			propertiesPalette->Spal->updateFormatList();
			propertiesPalette->SetLineFormats(docc);
			outlinePalette->BuildTree();
			outlinePalette->reopenTree(doc->OpenNodes);
			slotDocCh();
		}
	}
	delete ss;
}

void ScribusMainWindow::slotChangeUnit(int unitIndex, bool draw)
{
	doc->setUnitIndex(unitIndex);
	view->unitSwitcher->setCurrentText(unitGetStrFromIndex(doc->unitIndex()));
	propertiesPalette->unitChange();
	nodePalette->unitChange();
	alignDistributePalette->unitChange();
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

void ScribusMainWindow::manageMasterPages(QString temp)
{
	if (HaveDoc)
	{
		view->Deselect(true);
		if (doc->masterPageMode())
		{
			ActWin->masterPagesPalette()->updateMasterPageList(temp);
			ActWin->masterPagesPalette()->selectMasterPage(temp);
		}
		else
		{
			MasterPagesPalette *dia = new MasterPagesPalette(this, doc, view, temp);
			//connect(dia, SIGNAL(createNew(int)), this, SLOT(slotNewMasterPage(int)));
			connect(dia, SIGNAL(removePage(int )), this, SLOT(DeletePage2(int )));
			connect(dia, SIGNAL(loadPage(QString, int, bool)), this, SLOT(loadPage(QString, int, bool)));
			connect(dia, SIGNAL(finished()), this, SLOT(manageMasterPagesEnd()));
			connect(dia, SIGNAL(docAltered()), outlinePalette, SLOT(BuildTree()));
			connect(dia, SIGNAL(docAltered()), SLOT(slotDocCh()));
			scrActions["pageInsert"]->setEnabled(false);
			scrActions["pageImport"]->setEnabled(false);
			scrActions["pageDelete"]->setEnabled(false);
			scrActions["pageCopy"]->setEnabled(false);
			scrActions["pageMove"]->setEnabled(false);
			scrActions["pageApplyMasterPage"]->setEnabled(false);
			scrActions["pageCopyToMasterPage"]->setEnabled(false);
			scrActions["editMasterPages"]->setEnabled(false);
			ActWin->setMenuStatus(0, scrActions["fileSave"]->isEnabled());
			ActWin->setMenuStatus(1, scrActions["fileRevert"]->isEnabled());
			ActWin->setMenuStatus(2, scrActions["fileSave"]->isEnabled());
			ActWin->setMenuStatus(3, scrActions["fileSaveAs"]->isEnabled());
			scrActions["fileNew"]->setEnabled(false);
			scrActions["fileOpen"]->setEnabled(false);
			scrActions["fileClose"]->setEnabled(false);
			scrMenuMgr->setMenuEnabled("FileOpenRecent", false);
			scrActions["fileRevert"]->setEnabled(false);
			scrActions["fileDocSetup"]->setEnabled(false);
			scrActions["filePrint"]->setEnabled(false);
			scrActions["PrintPreview"]->setEnabled(false);
			pagePalette->enablePalette(false);
			dia->show();
			ActWin->setMasterPagesPalette(dia);
			doc->OpenNodes = outlinePalette->buildReopenVals();
		}
	}
}

void ScribusMainWindow::manageMasterPagesEnd()
{
	view->hideMasterPage();
	setAppMode(modeNormal);
	scrActions["editMasterPages"]->setEnabled(true);
	scrActions["fileNew"]->setEnabled(true);
	scrActions["fileOpen"]->setEnabled(true);
	scrActions["fileClose"]->setEnabled(true);
	scrActions["fileSave"]->setEnabled(doc->isModified());
	scrMenuMgr->setMenuEnabled("FileOpenRecent", true);
	scrActions["fileRevert"]->setEnabled(true);
	scrActions["fileDocSetup"]->setEnabled(true);
	scrActions["filePrint"]->setEnabled(true);
	if ( HaveGS==0 || ScQApp->isWinGUI() )
		scrActions["PrintPreview"]->setEnabled(true);
	scrActions["pageInsert"]->setEnabled(true);
	scrActions["pageCopy"]->setEnabled(true);
	scrActions["pageImport"]->setEnabled(true);
	scrActions["pageApplyMasterPage"]->setEnabled(true);
	scrActions["pageCopyToMasterPage"]->setEnabled(true);
	bool setter = doc->Pages->count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);
	uint pageCount=doc->DocPages.count();
	for (uint c=0; c<pageCount; ++c)
		Apply_MasterPage(doc->DocPages.at(c)->MPageNam, c, false);
	doc->setMasterPageMode(false);
	pagePalette->enablePalette(true);
	pagePalette->RebuildTemp();
	ActWin->setMasterPagesPalette(NULL);
	view->reformPages(false);
	view->DrawNew();
	pagePalette->Rebuild();
	outlinePalette->BuildTree();
//	outlinePalette->reopenTree(doc->OpenNodes);
//	slotDocCh();
}
/*!
 * @brief Apply master pages from the Apply Master Page dialog
 * @todo Make this work with real page numbers, negative numbers and document sections when they are implemented
 */
void ScribusMainWindow::ApplyMasterPage()
{
	Q_ASSERT(!doc->masterPageMode());
	ApplyMasterPageDialog *dia = new ApplyMasterPageDialog(this);
	dia->setup(doc, doc->currentPage->MPageNam);
	if (dia->exec())
	{
		QString masterPageName = dia->getMasterPageName();
		int pageSelection = dia->getPageSelection(); //0=current, 1=even, 2=odd, 3=all
		if (pageSelection==0) //current page only
			Apply_MasterPage(masterPageName, doc->currentPage->pageNr(), false);
		else
		{
			int startPage, endPage;
			if (dia->usingRange())
			{
				startPage=dia->getFromPage()-1; //Pages start from 0, not 1
				endPage=dia->getToPage();
			}
			else
			{
				startPage = pageSelection==1 ? 1 : 0; //if even, startPage is 1 (real page 2)
				endPage=doc->DocPages.count();
			}
			for (int pageNum = startPage; pageNum < endPage; ++pageNum)// +=pageStep)
			{
				//Increment by 1 and not 2 even for even/odd application as user
				//can select to eg apply to even pages with a single odd page selected
				if (pageSelection==1 && pageNum%2!=0) //Even, %2!=0 as 1st page is numbered 0
					Apply_MasterPage(masterPageName, pageNum, false);
				else
				if (pageSelection==2 && pageNum%2==0) //Odd, %2==0 as 1st page is numbered 0
					Apply_MasterPage(masterPageName, pageNum, false);
				else
				if (pageSelection==3) //All
					Apply_MasterPage(masterPageName, pageNum, false);
			}
		}
	}
	view->reformPages();
	view->DrawNew();
	slotDocCh();
	pagePalette->Rebuild();
	delete dia;
}

void ScribusMainWindow::Apply_MasterPage(QString in, int Snr, bool reb)
{
	if (!HaveDoc)
		return;
	doc->applyMasterPage(in, Snr);
	if (reb)
	{
		view->DrawNew();
		slotDocCh();
		pagePalette->Rebuild();
	}
}

void ScribusMainWindow::GroupObj(bool showLockDia)
{
	if (HaveDoc)
	{
		PageItem *currItem;
		PageItem* bb;
		double x, y, w, h;
		int t = -1; // show locked dialog only once
		QString tooltip = Um::ItemsInvolved + "\n";
		uint selectedItemCount=doc->selection->count();
		if (showLockDia)
		{
			for (uint a=0; a<selectedItemCount; ++a)
			{
				if (t == -1 && doc->selection->itemAt(a)->locked())
					t = QMessageBox::warning(this, CommonStrings::trWarning,
											 tr("Some objects are locked."),
											 CommonStrings::tr_Cancel,
											 tr("&Lock All"),
											 tr("&Unlock All"), 0, 0);
				if (t != -1)
					break; // already have an answer free to leave the loop
			}
			if (t == 0)
				return; // user chose cancel -> do not group but return

			for (uint a=0; a<selectedItemCount; ++a)
			{
				currItem = doc->selection->itemAt(a);
				if (currItem->locked())
				{
					for (uint c=0; c<selectedItemCount; ++c)
					{
						bb = doc->selection->itemAt(c);
						bool t1=(t==1);
						bb->setLocked(t1);
						scrActions["itemLock"]->setOn(t1);
						tooltip += "\t" + currItem->getUName() + "\n";
					}
				}
			}
		}

		SimpleState *ss = new SimpleState(Um::Group, tooltip);
		ss->set("GROUP", "group");
		ss->set("itemcount", selectedItemCount);

		for (uint a=0; a<selectedItemCount; ++a)
		{
			currItem = doc->selection->itemAt(a);
			currItem->Groups.push(doc->GroupCounter);
			ss->set(QString("item%1").arg(a), currItem->ItemNr);
		}
		doc->GroupCounter++;
		view->getGroupRect(&x, &y, &w, &h);
		view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
		outlinePalette->BuildTree();
		slotDocCh();
		scrActions["itemAttachTextToPath"]->setEnabled(false);
		scrActions["itemGroup"]->setEnabled(false);
		scrActions["itemUngroup"]->setEnabled(true);
		undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
	}
}

void ScribusMainWindow::UnGroupObj()
{
	if (HaveDoc)
	{
		SimpleState *ss = new SimpleState(Um::Ungroup);
		ss->set("UNGROUP", "ungroup");
		uint docSelectionCount=doc->selection->count();
		ss->set("itemcount", docSelectionCount);
		QString tooltip = Um::ItemsInvolved + "\n";
		PageItem *currItem;
		for (uint a=0; a<docSelectionCount; ++a)
		{
			currItem = doc->selection->itemAt(a);
			currItem->Groups.pop();
			currItem->isTableItem = false;
			currItem->LeftLink = 0;
			currItem->RightLink = 0;
			currItem->TopLink = 0;
			currItem->BottomLink = 0;
			ss->set(QString("item%1").arg(a), currItem->ItemNr);
			tooltip += "\t" + currItem->getUName() + "\n";
		}
		outlinePalette->BuildTree();
		slotDocCh();
		view->Deselect(true);

		undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
	}
}

void ScribusMainWindow::restore(UndoState* state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (ss)
	{
		if (ss->contains("GROUP"))
			restoreGroupping(ss, isUndo);
		else if (ss->contains("UNGROUP"))
			restoreUngroupping(ss, isUndo);
		else if (ss->contains("ADD_PAGE"))
			restoreAddPage(ss, isUndo);
		else if (ss->contains("DELETE_PAGE"))
			restoreDeletePage(ss, isUndo);
	}
}

void ScribusMainWindow::restoreDeletePage(SimpleState *state, bool isUndo)
{
	uint pagenr   = state->getUInt("PAGENR");
	QStringList tmpl = state->get("MASTERPAGE");
	int where, wo;
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
		addNewPages(wo, where, 1, doc->pageHeight, doc->pageWidth, doc->PageOri, doc->PageSize, true, &tmpl);
		UndoObject *tmp =
			undoManager->replaceObject(state->getUInt("DUMMY_ID"), doc->Pages->at(pagenr - 1));
		delete tmp;
	}
	else
	{
		DummyUndoObject *duo = new DummyUndoObject();
		uint id = static_cast<uint>(duo->getUId());
		undoManager->replaceObject(doc->Pages->at(pagenr - 1)->getUId(), duo);
		state->set("DUMMY_ID", id);
		DeletePage(pagenr, pagenr);
	}
}

void ScribusMainWindow::restoreAddPage(SimpleState *state, bool isUndo)
{
	int wo         = state->getInt("PAGE");
	int where      = state->getInt("WHERE");
	int count      = state->getInt("COUNT");
	QStringList based = QStringList::split("|", state->get("BASED"));
	double height = state->getDouble("HEIGHT");
	double width = state->getDouble("WIDTH");
	int orient = state->getInt("ORIENT");
	QString siz = state->get("SIZE");
	bool mov = static_cast<bool>(state->getInt("MOVED"));

	int delFrom, delTo;
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
			undoManager->replaceObject(doc->Pages->at(i)->getUId(), duo);
			state->set(QString("Page%1").arg(i), static_cast<uint>(did));
		}
		NoFrameEdit();
		view->Deselect(true);
		DeletePage(delFrom, delTo);
	}
	else
	{
		addNewPages(wo, where, count, height, width, orient, siz, mov, &based);
		for (int i = delFrom - 1; i < delTo; ++i)
		{
			UndoObject *tmp = undoManager->replaceObject(
					state->getUInt(QString("Page%1").arg(i)), doc->Pages->at(i));
			delete tmp;
		}
	}
}

void ScribusMainWindow::restoreGroupping(SimpleState *state, bool isUndo)
{
	int itemCount = state->getInt("itemcount");
	view->Deselect();
	for (int i = 0; i < itemCount; ++i)
	{
		int itemNr = state->getInt(QString("item%1").arg(i));
		view->SelectItemNr(itemNr);
	}
	if (isUndo)
		UnGroupObj();
	else
		GroupObj(false);
}

void ScribusMainWindow::restoreUngroupping(SimpleState *state, bool isUndo)
{
	int itemCount = state->getInt("itemcount");
	view->Deselect();
	for (int i = 0; i < itemCount; ++i)
	{
		int itemNr = state->getInt(QString("item%1").arg(i));
		view->SelectItemNr(itemNr);
	}
	if (isUndo)
		GroupObj(false);
	else
		UnGroupObj();
}

void ScribusMainWindow::StatusPic()
{
	if (HaveDoc)
	{
		PicStatus *dia = new PicStatus(this, doc, view);
		connect(dia, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
		connect(dia, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
		dia->exec();
		delete dia;
	}
}

QString ScribusMainWindow::CFileDialog(QString wDir, QString caption, QString filter, QString defNa,
                                bool Pre, bool mod, bool comp, bool cod, bool onlyDirs,
                                bool *docom, bool *doFont)
{
	QString retval = "";
	CustomFDialog *dia = new CustomFDialog(this, wDir, caption, filter, Pre, mod, comp, cod, onlyDirs);
	if (!defNa.isEmpty())
	{
		QFileInfo f(defNa);
		dia->setExtension(f.extension(true));
		dia->setZipExtension(f.extension(true) + ".gz");
		dia->setSelection(defNa);
	}
	if (onlyDirs)
	{
		dia->SaveZip->setChecked(*docom);
		dia->WFonts->setChecked(*doFont);
	}
	if (dia->exec() == QDialog::Accepted)
	{
		LoadEnc = "";
		if (!onlyDirs)
			LoadEnc = cod ? dia->TxCodeM->currentText() : QString("");
		else
		{
			*docom = dia->SaveZip->isChecked();
			*doFont = dia->WFonts->isChecked();
		}
		this->repaint();
		qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
		retval = dia->selectedFile();
	}
	delete dia;
	return retval;
}

void ScribusMainWindow::GetCMSProfiles()
{
	QString profDir;
	QStringList profDirs;
	MonitorProfiles.clear();
	PrinterProfiles.clear();
	InputProfiles.clear();
	InputProfilesCMYK.clear();
	QString pfad = ScPaths::instance().libDir();
	pfad += "profiles/";
	profDirs = ScPaths::getSystemProfilesDirs();
	profDirs.prepend( prefsManager->appPrefs.ProfileDir );
	profDirs.prepend( pfad );
	for(unsigned int i = 0; i < profDirs.count(); i++)
	{
		profDir = profDirs[i];
		if(!profDir.isEmpty())
		{
			if(profDir.right(1) != "/")
				profDir += "/";
			GetCMSProfilesDir(profDir);
		}
	}
	if ((!PrinterProfiles.isEmpty()) && (!InputProfiles.isEmpty()) && (!MonitorProfiles.isEmpty()))
		CMSavail = true;
	else
		CMSavail = false;
}

void ScribusMainWindow::GetCMSProfilesDir(QString pfad)
{
#ifdef HAVE_CMS
	QDir d(pfad, "*", QDir::Name, QDir::Files | QDir::Readable | QDir::Dirs | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		QString nam = "";
		const char *Descriptor;
		cmsHPROFILE hIn;

		for (uint dc = 0; dc < d.count(); ++dc)
		{
			QFileInfo fi(pfad + "/" + d[dc]);
			if (fi.isDir() && d[dc][0] != '.')
			{
				GetCMSProfilesDir(fi.filePath()+"/");
				continue;
			}
#ifdef QT_MAC
			QFile f(fi.filePath());
			QByteArray bb(40);
			if (!f.open(IO_ReadOnly)) {
				sDebug(QString("couldn't open %1 as ICC").arg(fi.filePath()));
				continue;
			}
			int len = f.readBlock(bb.data(), 40);
			f.close();
			if (len == 40 && bb[36] == 'a' && bb[37] == 'c' && bb[38] == 's' && bb[39] == 'p')
#else
			QString ext = fi.extension(false).lower();
			if ((ext == "icm") || (ext == "icc"))
#endif
			{
				const QCString profilePath( QString(pfad + d[dc]).local8Bit() );
				hIn = cmsOpenProfileFromFile(profilePath.data(), "r");
				if (hIn == NULL)
					continue;
				Descriptor = cmsTakeProductDesc(hIn);
				nam = QString(Descriptor);
				switch (static_cast<int>(cmsGetDeviceClass(hIn)))
				{
				case icSigInputClass:
				case icSigColorSpaceClass:
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
						InputProfiles[nam] = pfad + d[dc];
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
						InputProfilesCMYK[nam] = pfad + d[dc];
					break;
				case icSigDisplayClass:
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
					{
						MonitorProfiles[nam] = pfad + d[dc];
						InputProfiles[nam] = pfad + d[dc];
					}
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
						InputProfilesCMYK[nam] = pfad + d[dc];
					break;
				case icSigOutputClass:
					PrinterProfiles[nam] = pfad + d[dc];
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
					{
						PDFXProfiles[nam] = pfad + d[dc];
						InputProfilesCMYK[nam] = pfad + d[dc];
					}
					break;
				}
				cmsCloseProfile(hIn);
			}
		}
	}
#endif
}

void ScribusMainWindow::initCMS()
{
	if (CMSavail)
	{
		ProfilesL::Iterator ip;
		if ((prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile.isEmpty()) || (!InputProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile)))
		{
			ip = InputProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile.isEmpty()) || (!InputProfilesCMYK.contains(prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile)))
		{
			ip = InputProfilesCMYK.begin();
			prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultSolidColorProfile.isEmpty()) || (!InputProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultSolidColorProfile)))
		{
			ip = InputProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultSolidColorProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultMonitorProfile.isEmpty()) || (!MonitorProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultMonitorProfile)))
		{
			ip = MonitorProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultMonitorProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultPrinterProfile.isEmpty()) || (!PrinterProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultPrinterProfile)))
		{
			ip = PrinterProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultPrinterProfile = ip.key();
		}
#ifdef HAVE_CMS
		SoftProofing = prefsManager->appPrefs.DCMSset.SoftProofOn;
		CMSuse = false;
		IntentPrinter = prefsManager->appPrefs.DCMSset.DefaultIntentPrinter;
		IntentMonitor = prefsManager->appPrefs.DCMSset.DefaultIntentMonitor;
#endif
	}
}

void ScribusMainWindow::recalcColors(QProgressBar *dia)
{
	if (HaveDoc)
	{
		doc->recalculateColors();
		updateColorMenu(dia);
		propertiesPalette->updateColorList();
		propertiesPalette->updateCList();
	}
}

void ScribusMainWindow::ModifyAnnot()
{
	Q_ASSERT(!doc->masterPageMode());
	if (doc->selection->count() != 0)
	{
		PageItem *currItem = doc->selection->itemAt(0);
		if ((currItem->annotation().Type() == 0) || (currItem->annotation().Type() == 1) || (currItem->annotation().Type() > 9))
		{
			int AnType = currItem->annotation().Type();
			int AnActType = currItem->annotation().ActionType();
			QString AnAction = currItem->annotation().Action();
			QString An_Extern = currItem->annotation().Extern();
			Annota *dia = new Annota(this, currItem, doc->DocPages.count(), static_cast<int>(doc->pageWidth), static_cast<int>(doc->pageHeight), view);
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
			Annot *dia = new Annot(this, currItem, doc->DocPages.count(), static_cast<int>(doc->pageWidth), static_cast<int>(doc->pageHeight), doc->PageColors, view);
			if (dia->exec())
				slotDocCh();
			delete dia;
		}
	}
}

void ScribusMainWindow::SetShortCut()
{
	for (QMap<QString,Keys>::Iterator it = prefsManager->appPrefs.KeyActions.begin(); it != prefsManager->appPrefs.KeyActions.end(); ++it )
	{
		if (!it.data().actionName.isEmpty())
			if (scrActions[it.data().actionName])
				scrActions[it.data().actionName]->setAccel(it.data().keySequence);
	}
}

void ScribusMainWindow::PutScrap()
{
	ScriXmlDoc *ss = new ScriXmlDoc();
	QString objectString = ss->WriteElem(doc, view, doc->selection);
	scrapbookPalette->ObjFromMenu(objectString);
	delete ss;
}

void ScribusMainWindow::changeLayer(int )
{
	view->Deselect(true);
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	view->DrawNew();
}

void ScribusMainWindow::showLayer()
{
	view->DrawNew();
}

//TODO replace with the ScribusDoc::deleteTaggedItems
void ScribusMainWindow::LayerRemove(int l, bool dl)
{
	view->Deselect();
	for (uint b = 0; b < doc->MasterItems.count(); ++b)
	{
		if (doc->MasterItems.at(b)->LayerNr == l)
		{
			if (dl)
			{
				doc->selection->addItem(doc->MasterItems.at(b));
				doc->DocItems.at(b)->setLocked(false);
			}
			else
				doc->MasterItems.at(b)->setLayer(0);
		}
	}
	if (doc->selection->count() != 0)
		view->DeleteItem();
	doc->selection->clear();
	for (uint b = 0; b < doc->DocItems.count(); ++b)
	{
		if (doc->DocItems.at(b)->LayerNr == l)
		{
			if (dl)
			{
				doc->selection->addItem(doc->DocItems.at(b));
				doc->DocItems.at(b)->setLocked(false);
			}
			else
				doc->DocItems.at(b)->setLayer(0);
		}
	}
	if (doc->selection->count() != 0)
		view->DeleteItem();
	rebuildLayersList();
	view->updateLayerMenu();
}

void ScribusMainWindow::UnDoAction()
{
	undoManager->undo(1);
}

void ScribusMainWindow::RedoAction()
{
	undoManager->redo(1);
}

void ScribusMainWindow::initHyphenator()
{
	InstLang.clear();
	//Build our list of hyphenation dictionaries we have in the install dir
	//Grab the language abbreviation from it, get the full language text
	//Insert the name as key and a new string list into the map
	QString hyphDirName = QDir::convertSeparators(ScPaths::instance().libDir() + "dicts/");
	QDir hyphDir(hyphDirName, "*.dic", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((hyphDir.exists()) && (hyphDir.count() != 0))
	{
		LanguageManager langmgr;
		langmgr.init(false);
		QString languageOfHyphFile;
		for (uint dc = 0; dc < hyphDir.count(); ++dc)
		{
			QFileInfo fi(hyphDir[dc]);
			QString fileLangAbbrev=fi.baseName().section('_', 1);
			languageOfHyphFile = langmgr.getLangFromAbbrev(fileLangAbbrev, false);
			InstLang.insert(languageOfHyphFile, QStringList());
		}
	}

	//For each qm file existing, load the file and find the translations of the names
	QString pfad = ScPaths::instance().libDir();
	QDir d2(pfad, "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d2.exists()) && (d2.count() != 0))
	{
		for (uint dc = 0; dc < d2.count(); dc++)
		{
			QFileInfo fi(pfad + d2[dc]);
			QString ext = fi.extension(false).lower();
			if (ext == "qm")
			{
    			QTranslator *trans = new QTranslator(0);
				trans->load(pfad + d2[dc]);

				QString translatedLang;
				for (QMap<QString, QStringList>::Iterator it=InstLang.begin(); it!=InstLang.end(); ++it)
				{
					translatedLang="";
					translatedLang = trans->findMessage("ScribusMainWindow", it.key(), "").translation();
					if (!translatedLang.isEmpty())
						it.data().append(translatedLang);
				}
				delete trans;
			}
		}
	}
	//For each hyphenation file, grab the strings and the hyphenation data.
	QString lang = QString(QTextCodec::locale()).left(2);
	LangTransl.clear();
	prefsManager->appPrefs.Language = "English";
	if ((hyphDir.exists()) && (hyphDir.count() != 0))
	{
		LanguageManager langmgr;
		langmgr.init(false);
		QString datein = "";
		for (uint dc = 0; dc < hyphDir.count(); ++dc)
		{
			QFileInfo fi(hyphDir[dc]);
			QString fileLangAbbrev=fi.baseName().section('_', 1);
			datein = langmgr.getLangFromAbbrev(fileLangAbbrev);
			QString tDatein = datein;
			datein = GetLang(datein);
			LangTransl.insert(datein, tDatein);
			Sprachen.insert(datein, hyphDir[dc]);
			if (fileLangAbbrev == lang)
				prefsManager->appPrefs.Language = datein;
		}
		if (datein.isEmpty())
			prefsManager->appPrefs.Language = "English";
	}
	propertiesPalette->fillLangCombo(LangTransl);
}

QString ScribusMainWindow::GetLang(QString inLang)
{
	QMap<QString, QStringList>::Iterator itlend=InstLang.end();
 	for (QMap<QString, QStringList>::Iterator itl = InstLang.begin(); itl != itlend; ++itl)
	{
		QStringList::Iterator itlrend=itl.data().end();
		for (QStringList::Iterator itlr = itl.data().begin(); itlr != itlrend; ++itlr)
		{
			if ((*itlr) == inLang)
				return itl.key();
		}
	}
	return inLang;
}

void ScribusMainWindow::doHyphenate()
{
	if (HaveDoc)
	{
		uint selectedItemCount=doc->selection->count();
		if (selectedItemCount != 0)
		{
			for (uint i = 0; i < selectedItemCount; ++i)
			{
				PageItem *currItem = doc->selection->itemAt(i);
				if (doc->docHyphenator->Language != currItem->Language)
					doc->docHyphenator->slotNewDict(currItem->Language);
				doc->docHyphenator->slotHyphenate(currItem);
			}
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusMainWindow::doDeHyphenate()
{
	if (HaveDoc)
	{
		uint selectedItemCount=doc->selection->count();
		if (selectedItemCount != 0)
		{
			for (uint i = 0; i < selectedItemCount; ++i)
			{
				PageItem *currItem = doc->selection->itemAt(i);
				doc->docHyphenator->slotDeHyphenate(currItem);
			}
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusMainWindow::ManageGuides()
{
	if (HaveDoc)
	{
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		GuideManager *dia = new GuideManager(this);
		qApp->restoreOverrideCursor();
		dia->exec();
		delete dia;
	}
}

void ScribusMainWindow::setItemFillTransparency(double t)
{
	if (HaveDoc)
	{
		uint selectedItemCount=doc->selection->count();
		if (selectedItemCount != 0)
		{
			for (uint i = 0; i < selectedItemCount; ++i)
			{
				PageItem *currItem = doc->selection->itemAt(i);
				currItem->setFillTransparency(t);
			}
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusMainWindow::setItemLineTransparency(double t)
{
	if (HaveDoc)
	{
		uint selectedItemCount=doc->selection->count();
		if (selectedItemCount != 0)
		{
			for (uint i = 0; i < selectedItemCount; ++i)
			{
				PageItem *currItem = doc->selection->itemAt(i);
				currItem->setLineTransparency(t);
			}
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusMainWindow::ImageEffects()
{
	if (HaveDoc)
	{
		uint docSelectionCount=doc->selection->count();
		if (docSelectionCount != 0)
		{
			PageItem *currItem = doc->selection->itemAt(0);
			EffectsDialog* dia = new EffectsDialog(this, currItem, doc);
			if (dia->exec())
			{
				currItem->effectsInUse = dia->effectsList;
				view->UpdatePic();
			}
			delete dia;
			slotDocCh();
		}
	}
}

QString ScribusMainWindow::Collect(bool compress, bool withFonts, const QString& )
{
	CollectForOutput *c = new CollectForOutput(withFonts, compress);
	Q_CHECK_PTR(c);
	QString ret = c->collect();
	delete c;
	c=NULL;
	return ret;
}

void ScribusMainWindow::ReorgFonts()
{
	doc->reorganiseFonts();
	buildFontMenu();
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

void ScribusMainWindow::scanDocument()
{
	DocumentChecker docChecker;
	docChecker.checkDocument(doc);
}

void ScribusMainWindow::HaveRaster(bool art)
{
	if (art && doc->selection->count() != 0)
	{
		PageItem *currItem = doc->selection->itemAt(0);
		if (currItem->asImageFrame())
		{
			scrMenuMgr->clearMenu("Style");
			scrMenuMgr->addMenuToMenu("Color","Style");
			scrMenuMgr->addMenuItem(scrActions["styleImageEffects"], "Style");
		}
	}
}

void ScribusMainWindow::slotStoryEditor()
{
	if (doc->selection->count() != 0)
	{
		PageItem *currItem = doc->selection->itemAt(0);
		PageItem *currItemSE=storyEditor->currentItem();
		ScribusDoc *currDocSE=storyEditor->currentDocument();
		storyEditor->activFromApp = true;
		//CB shouldnt these be after the if?
		//Why are we resetting the doc and item in this case. My original code didnt do this.
		storyEditor->setCurrentDocumentAndItem(doc, currItem);
		if (currItem==currItemSE && doc==currDocSE)
		{
			storyEditor->show();
			storyEditor->raise();
			return;
		}
		CurrStED = storyEditor;
		connect(storyEditor, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
		connect(storyEditor, SIGNAL(EditSt()), this, SLOT(slotEditStyles()));
		storyEditor->show();
		storyEditor->raise();
	}
}

void ScribusMainWindow::emergencySave()
{
	emergencyActivated=true;
	std::cout << "Calling Emergency Save" << std::endl;
	QWidgetList windows = wsp->windowList();
	if (!windows.isEmpty())
	{
		uint windowCount=windows.count();
		for (uint i=0; i<windowCount ; ++i)
		{
			ActWin = (ScribusWin*)windows.at(i);
			doc = ActWin->document();
			view = ActWin->view();
			doc->setModified(false);
			if (doc->hasName)
			{
				std::cout << "Saving: " << doc->DocName+".emergency" << std::endl;
				doc->autoSaveTimer->stop();
				//disconnect(ActWin, SIGNAL(Schliessen()), ScMW, SLOT(DoFileClose()));
				ScriXmlDoc *ss = new ScriXmlDoc();
				ss->WriteDoc(doc->DocName+".emergency", doc, 0);
				delete ss;
			}
			view->close();
			uint numPages=doc->Pages->count();
			for (uint a=0; a<numPages; ++a)
				delete doc->Pages->at(a);
			delete doc;
			ActWin->close();
		}
	}
}

void ScribusMainWindow::EditTabs()
{
	if (HaveDoc)
	{
		if (doc->selection->count() != 0)
		{
			PageItem *currItem = doc->selection->itemAt(0);
			TabManager *dia = new TabManager(this, doc->unitIndex(), currItem->TabValues, currItem->width());
			if (dia->exec())
			{
				currItem->TabValues = dia->tmpTab;
				view->RefreshItem(currItem);
				slotDocCh();
			}
			delete dia;
		}
	}
}

void ScribusMainWindow::SearchText()
{
	PageItem *currItem = doc->selection->itemAt(0);
	setAppMode(modeEdit);
	currItem->CPos = 0;
	SearchReplace* dia = new SearchReplace(this, doc, currItem);
	connect(dia, SIGNAL(NewFont(const QString&)), this, SLOT(SetNewFont(const QString&)));
	connect(dia, SIGNAL(NewAbs(int)), this, SLOT(setAbsValue(int)));
	dia->exec();
	disconnect(dia, SIGNAL(NewFont(const QString&)), this, SLOT(SetNewFont(const QString&)));
	disconnect(dia, SIGNAL(NewAbs(int)), this, SLOT(setAbsValue(int)));
	delete dia;
	slotSelect();
}

void ScribusMainWindow::imageEditorExited()
{
	int ex = 0;
	if ( ExternalApp != 0 )
	{
		ex = ExternalApp->exitStatus();
		delete ExternalApp;
		ExternalApp = 0;
	}
}

/* call gimp and wait upon completion */
void ScribusMainWindow::callImageEditor()
{
	if (doc->selection->count() != 0)
	{
		QString imageEditorExecutable=prefsManager->imageEditorExecutable();
		if (ExternalApp != 0)
		{
			QMessageBox::information(this, tr("Information"), "<qt>" + tr("The program %1 is already running!").arg(imageEditorExecutable) + "</qt>", 1, 0, 0);
			return;
		}
		PageItem *currItem = doc->selection->itemAt(0);
		if (currItem->PicAvail)
		{
			int index;
			QString imEditor;
			ExternalApp = new QProcess(NULL);
			QStringList cmd;
		#if defined(_WIN32)
			index = imageEditorExecutable.find( ".exe" );
			if ( index >= 0 )
				imEditor = imageEditorExecutable.left( index + 4 );
			imEditor.replace( "\\", "/" );
			cmd.append(imEditor);
			if ( imEditor.length() < imageEditorExecutable.length() )
			{
				int diffLength = imageEditorExecutable.length() - imEditor.length();
				QString cmdStr = imageEditorExecutable.right( diffLength );
				QStringList cmd1 = QStringList::split( " ", cmdStr);
				cmd += cmd1;
			}
		#else
			cmd = QStringList::split(" ", imageEditorExecutable);
			if ( cmd.count() > 0 )
				imEditor = cmd[0];
		#endif
			index = imEditor.findRev( "/" );
			if (index > -1 )
			{
				QString imEditorDir = imEditor.left( index + 1 );
				ExternalApp->setWorkingDirectory( imEditorDir );
			}
			cmd.append(currItem->Pfile);
			ExternalApp->setArguments(cmd);
			if ( !ExternalApp->start() )
			{
				delete ExternalApp;
				ExternalApp = 0;
				QMessageBox::critical(this, CommonStrings::trWarning, "<qt>" + tr("The program %1 is missing!").arg(imageEditorExecutable) + "</qt>", 1, 0, 0);
				return;
			}
			connect(ExternalApp, SIGNAL(processExited()), this, SLOT(imageEditorExited()));
		}
	}
}

void ScribusMainWindow::slotCharSelect()
{
	if ((HaveDoc) && (doc->selection->count() != 0))
	{
		PageItem *currItem = doc->selection->itemAt(0);
		if ((currItem->asTextFrame()) && (doc->appMode == modeEdit))
		{
			CharSelect *dia = new CharSelect(this, currItem);
			dia->exec();
			delete dia;
		}
	}
}

void ScribusMainWindow::setUndoMode(bool isObjectSpecific)
{
	objectSpecificUndo = isObjectSpecific;

	if (!objectSpecificUndo && HaveDoc)
		undoManager->showObject(Um::GLOBAL_UNDO_MODE);
	else if (HaveDoc)
	{
		uint docSelectionCount=doc->selection->count();
		if (docSelectionCount == 1)
			undoManager->showObject(doc->selection->itemAt(0)->getUId());
		else if (docSelectionCount == 0)
			undoManager->showObject(doc->currentPage->getUId());
		else
			undoManager->showObject(Um::NO_UNDO_STACK);
	}
}

bool ScribusMainWindow::isObjectSpecificUndo()
{
	return objectSpecificUndo;
}

void ScribusMainWindow::slotTest()
{
}

void ScribusMainWindow::slotTest2()
{
}

void ScribusMainWindow::getImageInfo()
{
	if ((HaveDoc) && (doc->selection->count() == 1))
	{
		PageItem *pageItem = doc->selection->itemAt(0);
		if (pageItem != NULL)
		{
			if (pageItem->itemType() == PageItem::ImageFrame)
			{
				ImageInfoDialog *dia = new ImageInfoDialog(this, &pageItem->pixm.imgInfo);
				dia->exec();
				delete dia;
			}
		}
	}
}

void ScribusMainWindow::objectAttributes()
{
	if ((HaveDoc) && (doc->selection->count() == 1))
	{
		PageItem *pageItem = doc->selection->itemAt(0);
		if (pageItem!=NULL)
		{
			PageItemAttributes *pageItemAttrs = new PageItemAttributes( this );
			pageItemAttrs->setup(pageItem->getObjectAttributes(), &doc->docItemAttributes);
			//CB TODO Probably want this non modal in the future
			if (pageItemAttrs->exec()==QDialog::Accepted)
				pageItem->setObjectAttributes(pageItemAttrs->getNewAttributes());
			delete pageItemAttrs;
		}
	}
}

void ScribusMainWindow::generateTableOfContents()
{
	if (HaveDoc)
		tocGenerator->generateDefault();
}

void ScribusMainWindow::mouseReleaseEvent(QMouseEvent *m)
{
	bool sendToSuper=true;
	if (HaveDoc)
	{
		if (doc->appMode == modeEyeDropper)
		{
			releaseMouse();
			sendToSuper=false;
			QPixmap pm = QPixmap::grabWindow( QApplication::desktop()->winId(), m->globalPos().x(), m->globalPos().y(), 1, 1);
			QImage i = pm.convertToImage();
			QColor selectedColor=i.pixel(0, 0);
			bool found=false;
			ColorList::Iterator it;
			for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
			{
				if (selectedColor==doc->PageColors[it.key()].getRGBColor())
				{
					found=true;
					break;
				}
			}
			QString colorName=QString::null;
			if (found)
				colorName=it.key();
			else
			{
				bool ok;
				bool nameFound=false;
				QString questionString="<qt>"+tr("The selected color does not exist in the document's color set. Please enter a name for this new color.")+"</qt>";
				do
				{
					colorName = QInputDialog::getText( tr("Color Not Found"), questionString, QLineEdit::Normal, QString::null, &ok, this);
					if (ok)
					{
						if (doc->PageColors.contains(colorName))
							questionString="<qt>"+tr("The name you have selected already exists. Please enter a different name for this new color.")+"</qt>";
						else
							nameFound=true;
					}
				} while (!nameFound && ok);
				if ( ok && !colorName.isEmpty() )
				{
					ScColor newColor(selectedColor.red(), selectedColor.green(), selectedColor.blue());
					doc->PageColors[colorName]=newColor;
					propertiesPalette->updateColorList();
					propertiesPalette->updateCList();
				}
				else
					colorName=QString::null;
			}
			uint docSelectionCount=doc->selection->count();
			if (!colorName.isNull() && docSelectionCount > 0)
			{
				for (uint i = 0; i < docSelectionCount; ++i)
				{
					PageItem *currItem=doc->selection->itemAt(i);
					if (currItem!=NULL)
					{
						if ((m->stateAfter() & Qt::ControlButton) && (currItem->asTextFrame() || currItem->asPathText()))
							doc->ItemTextBrush(colorName); //Text colour
						else
						if (m->stateAfter() & Qt::AltButton) //Line colour
							setPenFarbe(colorName);
						else
							doc->ItemBrush(colorName); //Fill colour
					}
				}
			}
			//propertiesPalette->Cpal->SetColors(ScMW->doc->PageColors);
			//propertiesPalette->updateCList();
			setAppMode(modeNormal);
		}
	}
	if (sendToSuper)
		QMainWindow::mouseReleaseEvent(m);

}

void ScribusMainWindow::insertSampleText()
{
	LoremManager *m = new LoremManager(this, "m", true, 0);
	if (prefsManager->appPrefs.useStandardLI)
		m->insertLoremIpsum("loremipsum.xml", prefsManager->appPrefs.paragraphsLI);
	else
		m->exec();
	delete(m);
	m=NULL;
}

void ScribusMainWindow::languageChange()
{
	if (scribusInitialized)
	{
		CommonStrings::languageChange();
		//Update actions
		if (actionManager!=NULL)
		{
			actionManager->languageChange();
			pluginManager->languageChange();
			initKeyboardShortcuts();
		}
		//Update menu texts
		if (scrMenuMgr!=NULL && !scrMenuMgr->empty())
		{
			scrMenuMgr->setMenuText("File", tr("&File"));
			scrMenuMgr->setMenuText(recentFileMenuName, tr("Open &Recent"));
			scrMenuMgr->setMenuText("FileImport", tr("&Import"));
			scrMenuMgr->setMenuText("FileExport", tr("&Export"));
			scrMenuMgr->setMenuText("Edit", tr("&Edit"));
			scrMenuMgr->setMenuText("Style", tr("St&yle"));
			scrMenuMgr->setMenuText("Color", tr("&Color"));
			scrMenuMgr->setMenuText("FontSize", tr("&Size"));
			scrMenuMgr->setMenuText("Shade", tr("&Shade"));
			scrMenuMgr->setMenuText("Font", tr("&Font"));
			scrMenuMgr->setMenuText("TypeEffects", tr("&Effects"));
			scrMenuMgr->setMenuText("Item", tr("&Item"));
			scrMenuMgr->setMenuText("ItemLevel", tr("&Level"));
			scrMenuMgr->setMenuText("ItemLayer", tr("Send to Layer"));
			scrMenuMgr->setMenuText("ItemPreviewSettings", tr("Previe&w Settings"));
			scrMenuMgr->setMenuText("ItemPDFOptions", tr("&PDF Options"));
			scrMenuMgr->setMenuText("ItemShapes", tr("&Shape"));
			scrMenuMgr->setMenuText("ItemConvertTo", tr("C&onvert To"));
			scrMenuMgr->setMenuText("Insert", tr("I&nsert"));
			scrMenuMgr->setMenuText("InsertChar", tr("Character"));
			scrMenuMgr->setMenuText("InsertQuote", tr("Quote"));
			scrMenuMgr->setMenuText("InsertSpace", tr("Space"));
			scrMenuMgr->setMenuText("Page", tr("&Page"));
			scrMenuMgr->setMenuText("View", tr("&View"));
			scrMenuMgr->setMenuText("Tools", tr("&Tools"));
			scrMenuMgr->setMenuText("Extras", tr("E&xtras"));
			scrMenuMgr->setMenuText("Windows", tr("&Windows"));
			scrMenuMgr->setMenuText("Help", tr("&Help"));
			scrMenuMgr->setMenuText("Alignment", tr("&Alignment"));

			rebuildStyleMenu(HaveDoc ? doc->CurrentSel : -1);
		}
		if (undoManager!=NULL)
			undoManager->languageChange();

		mainWindowXPosLabel->setText( tr("X-Pos:"));
		mainWindowYPosLabel->setText( tr("Y-Pos:"));
		mainWindowXPosDataLabel->setText("         ");
		mainWindowYPosDataLabel->setText("         ");
		mainWindowStatusLabel->setText( tr("Ready"));
		noneString = tr("None");
	}
}

void ScribusMainWindow::setDefaultPrinter(const QString& name, const QString& file, const QString& command)
{
	PDef.Pname = name;
	PDef.Dname = file;
	PDef.Command = command;
}

void ScribusMainWindow::getDefaultPrinter(QString *name, QString *file, QString *command)
{
	*name=PDef.Pname;
	*file=PDef.Dname;
	*command=PDef.Command;
}

const bool ScribusMainWindow::fileWatcherActive()
{
	if (fileWatcher!=NULL)
		return fileWatcher->isActive();
	return false;
}

void ScribusMainWindow::updateColorMenu(QProgressBar* progressBar)
{
	disconnect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
	ColorMenC->clear();
	ColorMenC->insertItem( tr("None"));
	if (HaveDoc)
	{
		if (doc->toolSettings.dBrush == tr("None"))
			ColorMenC->setCurrentItem(0);
		int a = 1;
		ColorList::Iterator itend=doc->PageColors.end();
		for (ColorList::Iterator it = doc->PageColors.begin(); it != itend; ++it)
		{
			QPixmap *pm = getSmallPixmap(doc->PageColors[it.key()].getRawRGBColor());
			ColorMenC->insertItem(*pm, it.key());
			if (it.key() == doc->toolSettings.dBrush)
				ColorMenC->setCurrentItem(a);
			++a;
			if (progressBar != NULL)
				progressBar->setProgress(a);
		}
	}
	connect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
}

void ScribusMainWindow::closeActiveWindowMasterPageEditor()
{
	if (!HaveDoc)
		return;
	if(doc->masterPageMode())
	{
		ActWin->masterPagesPalette()->close();
		qApp->processEvents();
	}
}

void ScribusMainWindow::updateActiveWindowCaption(const QString &newCaption)
{
	if (!HaveDoc)
		return;
	ActWin->setCaption(newCaption);
}

void ScribusMainWindow::dragEnterEvent ( QDragEnterEvent* e)
{
	bool accepted = false;
	if ( e->provides("text/uri-list") && QUriDrag::canDecode(e) )
	{
		QString fileUrl;
		QStringList fileUrls;
		QUriDrag::decodeLocalFiles(e, fileUrls);
		for( uint i = 0; i < fileUrls.count(); ++i )
		{
			fileUrl = fileUrls[i].lower();
			if ( fileUrl.endsWith(".sla") || fileUrl.endsWith(".sla.gz") )
			{
				accepted = true;;
				break;
			}
		}
	}
	e->accept(accepted);
}

void ScribusMainWindow::dropEvent ( QDropEvent * e)
{
	bool accepted = false;
	if ( e->provides("text/uri-list") && QUriDrag::canDecode(e) )
	{
		QString fileUrl;
		QStringList fileUrls;
		QUriDrag::decodeLocalFiles(e, fileUrls);
		for( uint i = 0; i < fileUrls.count(); ++i )
		{
			fileUrl = fileUrls[i].lower();
			if ( fileUrl.endsWith(".sla") || fileUrl.endsWith(".sla.gz") )
			{
				QUrl url( fileUrls[i] );
				QFileInfo fi(url.path());
				if ( fi.exists() )
				{
					accepted = true;
					loadDoc( fi.fileName() );
				}
			}
		}
	}
	e->accept( accepted );
}

void ScribusMainWindow::slotEditCopyContents()
{
	PageItem *currItem=NULL;
	contentsBuffer.contentsFileName="";
	if (HaveDoc && (currItem=doc->selection->itemAt(0))!=NULL)
	{
		if (currItem->itemType()==PageItem::ImageFrame)
		{
			PageItem_ImageFrame* imageItem=currItem->asImageFrame();
			if (imageItem->PicAvail)
			{
				contentsBuffer.sourceType=PageItem::ImageFrame;
				contentsBuffer.contentsFileName=imageItem->Pfile;
				contentsBuffer.LocalScX=imageItem->imageXScale();
				contentsBuffer.LocalScY=imageItem->imageYScale();
				contentsBuffer.LocalX=imageItem->imageXOffset();
				contentsBuffer.LocalY=imageItem->imageYOffset();
				contentsBuffer.inputProfile=imageItem->IProfile;
				contentsBuffer.useEmbedded=imageItem->UseEmbedded;
				contentsBuffer.renderingIntent=imageItem->IRender;
			}
		}
	}
}

void ScribusMainWindow::slotEditPasteContents()
{
	PageItem *currItem=NULL;
	if (HaveDoc && !contentsBuffer.contentsFileName.isEmpty() && (currItem=doc->selection->itemAt(0))!=NULL)
	{
		if (contentsBuffer.sourceType==PageItem::ImageFrame && currItem->itemType()==PageItem::ImageFrame)
		{
			PageItem_ImageFrame* imageItem=currItem->asImageFrame();
			int t=QMessageBox::Yes;
			if (imageItem->PicAvail)
				t = ScMessageBox::warning(this, CommonStrings::trWarning,
										tr("Do you really want to replace your existing image?"),
										QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
			if (t == QMessageBox::Yes)
			{
				imageItem->EmProfile = "";
				imageItem->pixm.imgInfo.isRequest = false;
				imageItem->IProfile = doc->CMSSettings.DefaultImageRGBProfile;
				imageItem->IRender = doc->CMSSettings.DefaultIntentImages;
				qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				doc->loadPict(contentsBuffer.contentsFileName, imageItem);
				imageItem->AdjustPictScale();
				imageItem->setImageXYScale(contentsBuffer.LocalScX, contentsBuffer.LocalScY);
				imageItem->setImageXYOffset(contentsBuffer.LocalX, contentsBuffer.LocalY);
				imageItem->IProfile=contentsBuffer.inputProfile;
				imageItem->UseEmbedded=contentsBuffer.useEmbedded;
				imageItem->IRender=contentsBuffer.renderingIntent;
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				qApp->restoreOverrideCursor();
				view->DrawNew();
				propertiesPalette->updateColorList();
				propertiesPalette->updateCList();
				propertiesPalette->ShowCMS();
			}
		}
	}
}
