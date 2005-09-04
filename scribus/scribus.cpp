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
#include "prefstable.h"
#include "pdfopts.h"
#include "pdflib.h"
#include "inspage.h"
#include "delpages.h"
#include "movepage.h"
#include "helpbrowser.h"
#include "scribusXml.h"
#include "about.h"
#include "pslib.h"
#include "druck.h"
#include "editformats.h"
#include "muster.h"
#include "applytemplatedialog.h"
#include "picstatus.h"
#include "customfdialog.h"
#include "cmsprefs.h"
#include "annot.h"
#include "annota.h"
#include "javadocs.h"
#include "colorm.h"
#include "mpalette.h"
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
#include "tocindexprefs.h"
#include "tocgenerator.h"

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
#include "util.h"
#include "pagesize.h"
#include "loremipsum.h"
#include "marginWidget.h"
#include "margindialog.h"
#include "prefsmanager.h"
#include "pagelayout.h"
#include "commonstrings.h"
#include "preview.h"

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
bool CMSuse;
int IntentMonitor;
int IntentPrinter;
#endif
bool CMSavail;

QString DocDir;
extern ScribusApp* ScApp;
extern ScribusQApp* ScQApp;
extern bool emergencyActivated;

ScribusApp::ScribusApp()
{
	scribusInitialized=false;
	actionManager=NULL;
	scrMenuMgr=NULL;
	undoManager=NULL;
	prefsManager=NULL;
} // ScribusApp::ScribusApp()

/*
 * retval 0 - ok, 1 - no fonts, ...
 */
int ScribusApp::initScribus(bool showSplash, bool showFontInfo, const QString newGuiLanguage)
{
	CommonStrings::languageChange();
	noneString = tr("None");
	int retVal=0;
	ExternalApp = 0;
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
	initDefaultValues();

	actionManager = new ActionManager(this, "actionManager");
	initMenuBar();
	initStatusBar();
	initToolBars();
	qApp->processEvents();

	BuFromApp = false;

	bool haveFonts=initFonts(showFontInfo);
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
		prefsManager->ReadPrefs();
		setSplashStatus( tr("Initializing Story Editor") );
		storyEditor = new StoryEditor(this);

#ifndef _WIN32
		HaveGS = system(prefsManager->ghostscriptExecutable()+" -h > /dev/null 2>&1");
		HavePngAlpha = system(prefsManager->ghostscriptExecutable()+" -sDEVICE=pngalpha -c quit > /dev/null 2>&1");
		HaveTiffSep = system(prefsManager->ghostscriptExecutable()+" -sDEVICE=tiffsep -c quit > /dev/null 2>&1");
#else
		HaveGS = system(getShortPathName(prefsManager->ghostscriptExecutable())+" -h >NUL");
		HavePngAlpha = system(getShortPathName(prefsManager->ghostscriptExecutable())+" -sDEVICE=pngalpha -c quit >NUL");
		HaveTiffSep = system(getShortPathName(prefsManager->ghostscriptExecutable())+" -sDEVICE=tiffsep -c quit >NUL");
#endif
		DocDir = prefsManager->documentDir();

		setSplashStatus( tr("Reading ICC Profiles") );
		CMSavail = false;
		GetCMSProfiles();
		initCMS();

		setSplashStatus( tr("Initializing Hyphenator") );
		initHyphenator();
		setSplashStatus( tr("Reading Scrapbook") );
		initScrapbook();
		setSplashStatus( tr("Setting up Shortcuts") );
		SetShortCut();

		emit prefsChanged();

		connect(fileWatcher, SIGNAL(fileDeleted(QString )), this, SLOT(removeRecent(QString)));
		connect(this, SIGNAL(TextIFont(QString)), this, SLOT(AdjustFontMenu(QString)));
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
	return retVal;
}

void ScribusApp::initSplash(bool showSplash)
{
	if (showSplash)
	{
		splashScreen = new SplashScreen();
		setSplashStatus(QObject::tr("Initializing..."));
	}
	else
		splashScreen = NULL;
}

void ScribusApp::setSplashStatus(const QString& newText)
{
	if (splashScreen != NULL)
		splashScreen->setStatus( newText );
	qApp->processEvents();
}

void ScribusApp::showSplash(bool shown)
{
	if (splashScreen!=NULL && shown!=splashScreen->isShown())
		splashScreen->setShown(shown);
}

bool ScribusApp::splashShowing() const
{
	if (splashScreen != NULL)
		return splashScreen->isShown();
	return false;
}

void ScribusApp::closeSplash()
{
	if (splashScreen!=NULL)
	{
		splashScreen->close();
		delete splashScreen;
		splashScreen = NULL;
	}
}

void ScribusApp::initToolBars()
{
	fileToolBar = new QToolBar( tr("File"), this);
	scrActions["fileNew"]->addTo(fileToolBar);
	scrActions["fileOpen"]->addTo(fileToolBar);
	scrMenuMgr->addMenuToWidgetOfAction("FileOpenRecent", scrActions["fileOpen"]);
	scrActions["fileSave"]->addTo(fileToolBar);
	scrActions["fileClose"]->addTo(fileToolBar);
	scrActions["filePrint"]->addTo(fileToolBar);
	scrActions["toolsPreflightVerifier"]->addTo(fileToolBar);
	scrActions["fileExportAsPDF"]->addTo(fileToolBar);

	editToolBar = new QToolBar( tr("Edit"), this);
	UndoWidget* uWidget = new UndoWidget(editToolBar, "uWidget");
	undoManager->registerGui(uWidget);

	mainToolBar = new WerkToolB(this);
	setDockEnabled(mainToolBar, DockLeft, false);
	setDockEnabled(mainToolBar, DockRight, false);
	mainToolBar->Sichtbar = true;
	mainToolBar->setEnabled(false);
	pdfToolBar = new WerkToolBP(this);
	setDockEnabled(pdfToolBar, DockLeft, false);
	setDockEnabled(pdfToolBar, DockRight, false);
	pdfToolBar->setEnabled(false);
	pdfToolBar->Sichtbar = true;

	connect(mainToolBar, SIGNAL(Schliessen()), this, SLOT(ToggleTools()));
	connect(pdfToolBar, SIGNAL(NewMode(int)), this, SLOT(setAppMode(int)));
	connect(pdfToolBar, SIGNAL(Schliessen()), this, SLOT(TogglePDFTools()));
}

//Returns false when there are no fonts
const bool ScribusApp::initFonts(bool showFontInfo)
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

void ScribusApp::initDefaultValues()
{
	dirs = prefsManager->prefsFile->getContext("dirs");
	HaveDoc = false;
	singleClose = false;
	ScriptRunning = false;
	view = NULL;
	doc = NULL;
	Buffer2 = "";
	unicodeTextEditMode = false;
	unicodeInputCount = 0;
	unicodeInputString = "";
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
}

void ScribusApp::initKeyboardShortcuts()
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

void ScribusApp::initPalettes()
{
	//CB TODO hide the publicly available members of some palettes
	// these must be filtered too as they take control of the palettes events
	outlinePalette = new Tree(this, this);
	connect( scrActions["toolsOutline"], SIGNAL(toggled(bool)) , outlinePalette, SLOT(setPaletteShown(bool)) );
	connect( outlinePalette, SIGNAL(paletteShown(bool)), scrActions["toolsOutline"], SLOT(setOn(bool)));
	outlinePalette->setPrefsContext("OutlinePalette");
	outlinePalette->reportDisplay->installEventFilter(this);
	propertiesPalette = new Mpalette(this);
	connect( scrActions["toolsProperties"], SIGNAL(toggled(bool)) , propertiesPalette, SLOT(setPaletteShown(bool)) );
	connect( propertiesPalette, SIGNAL(paletteShown(bool)), scrActions["toolsProperties"], SLOT(setOn(bool)));
	propertiesPalette->setPrefsContext("PropertiesPalette");
	propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
	propertiesPalette->Cpal->UseTrans(true);
	propertiesPalette->Fonts->RebuildList(0);
	propertiesPalette->installEventFilter(this);
	nodePalette = new NodePalette(this);
	nodePalette->setPrefsContext("NodePalette");
	nodePalette->installEventFilter(this);

	layerPalette = new LayerPalette(this);
	connect( scrActions["toolsLayers"], SIGNAL(toggled(bool)) , layerPalette, SLOT(setPaletteShown(bool)) );
	connect( layerPalette, SIGNAL(paletteShown(bool)), scrActions["toolsLayers"], SLOT(setOn(bool)));
	layerPalette->setPrefsContext("LayerPalette");
	layerPalette->installEventFilter(this);
	layerPalette->Table->installEventFilter(this);
	scrapbookPalette = new Biblio(this);
	connect( scrActions["toolsScrapbook"], SIGNAL(toggled(bool)) , scrapbookPalette, SLOT(setPaletteShown(bool)) );
	connect( scrapbookPalette, SIGNAL(paletteShown(bool)), scrActions["toolsScrapbook"], SLOT(setOn(bool)));
	scrapbookPalette->setPrefsContext("ScrapbookPalette");
	scrapbookPalette->installEventFilter(this);
	pagePalette = new SeitenPal(this);
	connect( scrActions["toolsPages"], SIGNAL(toggled(bool)) , pagePalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsPages"], SIGNAL(toggled(bool)) , this, SLOT(setPagePalette(bool)) );
	connect( pagePalette, SIGNAL(paletteShown(bool)), scrActions["toolsPages"], SLOT(setOn(bool)));
	pagePalette->setPrefsContext("PagePalette");
	pagePalette->installEventFilter(this);
	bookmarkPalette = new BookPalette(this);
	connect( scrActions["toolsBookmarks"], SIGNAL(toggled(bool)) , bookmarkPalette, SLOT(setPaletteShown(bool)) );
	connect( bookmarkPalette, SIGNAL(paletteShown(bool)), scrActions["toolsBookmarks"], SLOT(setOn(bool)));
	bookmarkPalette->setPrefsContext("BookmarkPalette");
	bookmarkPalette->installEventFilter(this);
	measurementPalette = new Measurements(this);
	connect( scrActions["toolsMeasurements"], SIGNAL(toggled(bool)) , measurementPalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsMeasurements"], SIGNAL(toggledData(bool, int)) , this, SLOT(setAppModeByToggle(bool, int)) );
	connect( measurementPalette, SIGNAL(paletteShown(bool)), scrActions["toolsMeasurements"], SLOT(setOn(bool)));
	measurementPalette->setPrefsContext("MeasurementPalette");
	measurementPalette->installEventFilter(this);
	measurementPalette->hide();
	docCheckerPalette = new CheckDocument(this, false);
	connect( scrActions["toolsPreflightVerifier"], SIGNAL(toggled(bool)) , docCheckerPalette, SLOT(setPaletteShown(bool)) );
	connect( docCheckerPalette, SIGNAL(paletteShown(bool)), scrActions["toolsPreflightVerifier"], SLOT(setOn(bool)));
	connect( docCheckerPalette, SIGNAL(paletteShown(bool)), this, SLOT(docCheckToggle(bool)));
	docCheckerPalette->setPrefsContext("DocCheckerPalette");
	docCheckerPalette->installEventFilter(this);
	docCheckerPalette->hide();

	alignDistributePalette = new AlignDistributePalette(this, "AlignDistributePalette", false);
	connect( scrActions["toolsAlignDistribute"], SIGNAL(toggled(bool)) , alignDistributePalette, SLOT(setPaletteShown(bool)) );
	connect( alignDistributePalette, SIGNAL(paletteShown(bool)), scrActions["toolsAlignDistribute"], SLOT(setOn(bool)));
	connect( alignDistributePalette, SIGNAL(documentChanged()), this, SLOT(slotDocCh()));
	alignDistributePalette->setPrefsContext("AlignDistributePalette");
	alignDistributePalette->installEventFilter(this);
	alignDistributePalette->hide();

	undoPalette = new UndoPalette(this, "undoPalette");
	undoPalette->installEventFilter(this);
	undoManager->registerGui(undoPalette);
	connect(undoPalette, SIGNAL(paletteShown(bool)), this, SLOT(setUndoPalette(bool)));
	connect(undoPalette, SIGNAL(objectMode(bool)), this, SLOT(setUndoMode(bool)));

	connect(propertiesPalette, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(propertiesPalette, SIGNAL(NewAbStyle(int)), this, SLOT(setNewAbStyle(int)));
	connect(propertiesPalette, SIGNAL(Stellung(int)), this, SLOT(setItemHoch(int)));
	connect(propertiesPalette, SIGNAL(EditCL()), this, SLOT(ToggleFrameEdit()));
	connect(propertiesPalette, SIGNAL(NewTF(const QString&)), this, SLOT(SetNewFont(const QString&)));
	connect(propertiesPalette, SIGNAL(UpdtGui(int)), this, SLOT(HaveNewSel(int)));
	connect(propertiesPalette->Cpal, SIGNAL(NewPen(QString)), this, SLOT(setPenFarbe(QString)));
	connect(propertiesPalette->Cpal, SIGNAL(NewBrush(QString)), this, SLOT(setBrushFarbe(QString)));
	connect(propertiesPalette->Cpal, SIGNAL(NewPenShade(int)), this, SLOT(setPenShade(int)));
	connect(propertiesPalette->Cpal, SIGNAL(NewBrushShade(int)), this, SLOT(setBrushShade(int)));
	connect(propertiesPalette->Cpal, SIGNAL(NewTrans(double)), this, SLOT(setItemFillTransparency(double)));
	connect(propertiesPalette->Cpal, SIGNAL(NewTransS(double)), this, SLOT(setItemLineTransparency(double)));
	connect(propertiesPalette->Cpal, SIGNAL(NewGradient(int)), this, SLOT(setGradFill(int)));
	connect(propertiesPalette->Cpal->gradEdit->Preview, SIGNAL(gradientChanged()), this, SLOT(updtGradFill()));
	connect(propertiesPalette->Cpal, SIGNAL(gradientChanged()), this, SLOT(updtGradFill()));
	connect(propertiesPalette->Cpal, SIGNAL(QueryItem()), this, SLOT(GetBrushPen()));
	connect(docCheckerPalette, SIGNAL(selectElement(int, int)), this, SLOT(selectItemsFromOutlines(int, int)));
	connect(docCheckerPalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(docCheckerPalette, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
	connect(outlinePalette, SIGNAL(selectElement(int, int, bool)), this, SLOT(selectItemsFromOutlines(int, int, bool)));
	connect(outlinePalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(outlinePalette, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
	connect(propertiesPalette->Spal, SIGNAL(newStyle(int)), this, SLOT(setNewAbStyle(int)));
	connect(propertiesPalette, SIGNAL(EditLSt()), this, SLOT(slotEditLineStyles()));
	connect(nodePalette, SIGNAL(Schliessen()), this, SLOT(NoFrameEdit()));
	connect(layerPalette, SIGNAL(LayerChanged()), this, SLOT(showLayer()));
	connect(pagePalette, SIGNAL(EditTemp(QString)), this, SLOT(manageMasterPages(QString)));
	connect(pagePalette->PageView, SIGNAL(UseTemp(QString, int)), this, SLOT(Apply_MasterPage(QString, int)));
	connect(pagePalette->PageView, SIGNAL(NewPage(int, QString)), this, SLOT(slotNewPageP(int, QString)));
	connect(pagePalette->Trash, SIGNAL(DelPage(int)), this, SLOT(DeletePage2(int)));
	connect(pagePalette, SIGNAL(GotoSeite(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(bookmarkPalette->BView, SIGNAL(MarkMoved()), this, SLOT(StoreBookmarks()));
	connect(bookmarkPalette->BView, SIGNAL(ChangeBMNr(int, int, int)), this, SLOT(ChBookmarks(int, int, int)));
	connect(bookmarkPalette->BView, SIGNAL(SelectElement(int, int)), this, SLOT(selectItemsFromOutlines(int, int)));
}

void ScribusApp::initScrapbook()
{
	QString scrapbookFile = QDir::convertSeparators(PrefsPfad+"/scrap13.scs");
	QFileInfo scrapbookFileInfo = QFileInfo(scrapbookFile);
	if (scrapbookFileInfo.exists())
		scrapbookPalette->readContents(scrapbookFile);
	scrapbookPalette->setScrapbookFileName(scrapbookFile);
	scrapbookPalette->AdjustMenu();
}

const QString ScribusApp::getGuiLanguage()
{
	return guiLanguage;
}

bool ScribusApp::warningVersion(QWidget *parent)
{
	bool retval = false;
	int t = QMessageBox::warning(parent, QObject::tr("Scribus Development Version"), "<qt>" +
								 QObject::tr("You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File->Save As. Are you sure you wish to proceed with this operation?") + "</qt>",
								 CommonStrings::tr_Cancel, QObject::tr("&Proceed"), "", 1, 0);
	if (t == 1)
		retval = true;
	return retval;
}

void ScribusApp::initMenuBar()
{
	QFont tmp;
	RecentDocs.clear();

	scrMenuMgr->createMenu("File", tr("&File"));
	scrMenuMgr->addMenuItem(scrActions["fileNew"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileOpen"], "File");
	recentFileMenuName="FileOpenRecent";
	scrMenuMgr->createMenu(recentFileMenuName, tr("Open &Recent"), "File");
	scrMenuMgr->addMenuSeparator("File");
	scrMenuMgr->addMenuItem(scrActions["fileClose"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileSave"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileSaveAs"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileRevert"], "File");
	scrMenuMgr->addMenuItem(scrActions["fileCollect"], "File");
	scrMenuMgr->addMenuSeparator("File");
	scrMenuMgr->createMenu("FileImport", tr("&Import"), "File");
	scrMenuMgr->addMenuItem(scrActions["fileImportText"], "FileImport");
	scrMenuMgr->addMenuItem(scrActions["fileImportAppendText"], "FileImport");
	scrMenuMgr->addMenuItem(scrActions["fileImportImage"], "FileImport");
	scrMenuMgr->createMenu("FileExport", tr("&Export"), "File");
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
	ColorMenC = new QComboBox(false);
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
	scrMenuMgr->createMenu("ItemConvertTo", tr("C&onvert To"));
	scrMenuMgr->addMenuToMenu("ItemConvertTo", "Item");
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

	scrMenuMgr->createMenu("InsertChar", tr("Character"), "Insert");
	//scrMenuMgr->addMenuToMenu("InsertChar", "Insert");
	scrMenuMgr->addMenuItem(scrActions["specialPageNumber"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialSmartHyphen"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialNonBreakingHyphen"], "InsertChar");
	scrMenuMgr->addMenuSeparator("InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialCopyRight"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialRegdTM"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialTM"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialBullet"], "InsertChar");
	scrMenuMgr->addMenuSeparator("InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialDashEm"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialDashEn"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialDashFigure"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["specialDashQuotation"], "InsertChar");

	scrMenuMgr->createMenu("InsertQuote", tr("Quote"), "Insert");
	//scrMenuMgr->addMenuToMenu("InsertQuote", "Insert");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteApostrophe"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteStraight"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteSingleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteSingleRight"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteDoubleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteDoubleRight"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteSingleReversed"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteDoubleReversed"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteSingleLeftGuillemet"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteSingleRightGuillemet"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteDoubleLeftGuillemet"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteDoubleRightGuillemet"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteLowSingleComma"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteLowDoubleComma"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteDoubleTurnedComma"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteCJKSingleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteCJKSingleRight"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteCJKDoubleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["specialQuoteCJKDoubleRight"], "InsertQuote");

	scrMenuMgr->createMenu("InsertSpace", tr("Space"), "Insert");
	//scrMenuMgr->addMenuToMenu("InsertSpace", "Insert");
	scrMenuMgr->addMenuItem(scrActions["specialNonBreakingSpace"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["specialNewLine"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["specialFrameBreak"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["specialColumnBreak"], "InsertSpace");
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
	scrMenuMgr->createMenu("Windows", tr("&Windows"));
	connect(scrMenuMgr->getLocalPopupMenu("Windows"), SIGNAL(aboutToShow()), this, SLOT(windowsMenuAboutToShow()));
	addDefaultWindowMenuItems();

	//Help menu
	scrMenuMgr->createMenu("Help", tr("&Help"));
	scrMenuMgr->addMenuItem(scrActions["helpAboutScribus"], "Help");
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

void ScribusApp::addDefaultWindowMenuItems()
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


void ScribusApp::initStatusBar()
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

void ScribusApp::setMousePositionOnStatusBar(double xp, double yp)
{
	double xn = xp;
	double yn = yp;
	if (doc->guidesSettings.rulerMode)
	{
		xn -= doc->currentPage->Xoffset;
		yn -= doc->currentPage->Yoffset;
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

void ScribusApp::deleteSelectedTextFromFrame(PageItem *currItem)
{
	int firstSelection = 0;
	bool first = false;
	for (ScText *it = currItem->itemText.first(); it != 0; it = currItem->itemText.next())
	{
		if (it->cselect)
		{
			first = true;
			if ((it->ch == QChar(25)) && (it->cembedded != 0))
			{
				doc->FrameItems.remove(it->cembedded);
				delete it->cembedded;
			}
			currItem->itemText.remove();
			it = currItem->itemText.prev();
			if (it == 0)
				it = currItem->itemText.first();
		}
		if (!first)
			firstSelection++;
	}
	if (currItem->itemText.count() != 0)
	{
		if (currItem->itemText.first()->cselect)
		{
			currItem->itemText.remove();
			currItem->CPos = 0;
		}
		else
			currItem->CPos = firstSelection;
	}
	else
		currItem->CPos = 0;
	currItem->HasSel = false;
	for (uint a = 0; a < doc->FrameItems.count(); ++a)
	{
		doc->FrameItems.at(a)->ItemNr = a;
	}
	DisableTxEdit();
}

void ScribusApp::setTBvals(PageItem *currItem)
{
	if (currItem->itemText.count() != 0)
	{
		int ChPos = QMIN(currItem->CPos, static_cast<int>(currItem->itemText.count()-1));
		doc->CurrentStyle = currItem->itemText.at(ChPos)->cstyle & 1919;
		doc->currentParaStyle = currItem->itemText.at(ChPos)->cab;
		setAbsValue(doc->currentParaStyle);
		propertiesPalette->setAli(doc->currentParaStyle);
		doc->CurrFont = currItem->itemText.at(ChPos)->cfont->SCName;
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

void ScribusApp::wheelEvent(QWheelEvent *w)
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
void ScribusApp::specialActionKeyEvent(QString actionName, int unicodevalue)
{
	if (HaveDoc)
	{
		if (doc->appMode==modeEdit)
		{
			if (view->SelItem.count() == 1)
			{
				struct ScText *hg = new ScText;
				PageItem *currItem = view->SelItem.at(0);
				if (unicodevalue!=-1)
				{
					hg->ch = QString(QChar(unicodevalue));
					hg->cfont = (*doc->AllFonts)[doc->CurrFont];
					hg->csize = doc->CurrFontSize;
					hg->ccolor = doc->CurrTextFill;
					hg->cshade = doc->CurrTextFillSh;
					hg->cstroke = doc->CurrTextStroke;
					hg->cshade2 = doc->CurrTextStrokeSh;
					hg->cscale = doc->CurrTextScale;
					hg->cscalev = doc->CurrTextScaleV;
					hg->cbase = doc->CurrTextBase;
					hg->cshadowx = doc->CurrTextShadowX;
					hg->cshadowy = doc->CurrTextShadowY;
					hg->coutline = doc->CurrTextOutline;
					hg->cunderpos = doc->CurrTextUnderPos;
					hg->cunderwidth = doc->CurrTextUnderWidth;
					hg->cstrikepos = doc->CurrTextStrikePos;
					hg->cstrikewidth = doc->CurrTextStrikeWidth;
					hg->cselect = false;
					hg->cstyle = doc->CurrentStyle;
					hg->cab = doc->currentParaStyle;
					if (!doc->docParagraphStyles[doc->currentParaStyle].Font.isEmpty())
					{
						hg->cfont = (*doc->AllFonts)[doc->docParagraphStyles[doc->currentParaStyle].Font];
						hg->csize = doc->docParagraphStyles[doc->currentParaStyle].FontSize;
					}
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
				else if (actionName=="specialSmartHyphen") //ignore the char as we use an attribute if the text item, for now.
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
  \brief Receive key events from palettes such as palette hiding events. Possibly eaier way but this is cleaner than before. No need to modify all those palettes and each new one in future.
 */
bool ScribusApp::eventFilter( QObject */*o*/, QEvent *e )
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
			retVal=false;
	}
	else
		retVal=false;
	//Return false to pass event to object
	return retVal;
}

void ScribusApp::keyPressEvent(QKeyEvent *k)
{
	QWidgetList windows;
	QWidget* w = NULL;
	struct ScText *hg;
	int kk = k->key();
	int as = k->ascii();
	double altx, alty;
	QString uc = k->text();
	QString cr, Tcha, Twort;
	uint Tcoun;
	int len, pos, c;
	if (keyrep)
		return;
	keyrep = true;
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
	if ((kk == Key_Escape) && (HaveDoc))
	{
		keyrep = false;
		PageItem *currItem;
		if ((view->SelItem.count() != 0))
		{
			currItem = view->SelItem.at(0);
			switch (doc->appMode)
			{
				case modeNormal:
					currItem->Sizing = false;
					if (doc->SubMode != -1)
					{
						view->Deselect(false);
						doc->Items.remove(currItem->ItemNr);
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
						doc->Items.remove(currItem->ItemNr);
					}
					else
					{
						view->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false);
						view->SetPolyClip(currItem, qRound(QMAX(currItem->Pwidth / 2, 1)));
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
					doc->Items.remove(currItem->ItemNr);
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
	if ((HaveDoc) && (!view->LE->hasFocus()) && (!view->PGS->focused()))
	{
		if ((doc->appMode != modeEdit) && (view->SelItem.count() == 0))
		{
			switch (kk)
			{
			case Key_Space:
				keyrep = false;
				if (doc->appMode == modePanning)
					setAppMode(modeNormal);
				else
				{
					setAppMode(modePanning);
					qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
				}
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
		if (view->SelItem.count() != 0)
		{
			PageItem *currItem = view->SelItem.at(0);
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
							if ( buttonState & ShiftButton )
								np = np - FPoint(10.0, 0);
							else if ( buttonState & ControlButton )
								np = np - FPoint(0.1, 0);
							else
								np = np - FPoint(1.0, 0);
							view->MoveClipPoint(currItem, np);
						}
						else
						{
							if ( buttonState & ShiftButton )
								view->moveGroup(-10, 0);
							else if ( buttonState & ControlButton )
								view->moveGroup(-0.1, 0);
							else
								view->moveGroup(-1, 0);
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
							if ( buttonState & ShiftButton )
								np = np + FPoint(10.0, 0);
							else if ( buttonState & ControlButton )
								np = np + FPoint(0.1, 0);
							else
								np = np + FPoint(1.0, 0);
							view->MoveClipPoint(currItem, np);
						}
						else
						{
							if ( buttonState & ShiftButton )
								view->moveGroup(10, 0);
							else if ( buttonState & ControlButton )
								view->moveGroup(0.1, 0);
							else
								view->moveGroup(1, 0);
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
							if ( buttonState & ShiftButton )
								np = np - FPoint(0, 10.0);
							else if ( buttonState & ControlButton )
								np = np - FPoint(0, 0.1);
							else
								np = np - FPoint(0, 1.0);
							view->MoveClipPoint(currItem, np);
						}
						else
						{
							if ( buttonState & ShiftButton )
								view->moveGroup(0, -10);
							else if ( buttonState & ControlButton )
								view->moveGroup(0, -0.1);
							else
								view->moveGroup(0, -1);
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
							if ( buttonState & ShiftButton )
								np = np + FPoint(0, 10.0);
							else if ( buttonState & ControlButton )
								np = np + FPoint(0, 0.1);
							else
								np = np + FPoint(0, 1.0);
							view->MoveClipPoint(currItem, np);
						}
						else
						{
							if ( buttonState & ShiftButton )
								view->moveGroup(0, 10);
							else if ( buttonState & ControlButton )
								view->moveGroup(0, 0.1);
							else
								view->moveGroup(0, 1);
						}
						slotDocCh();
					}
					break;
				default:
					break;
				}
				break;
			case modeEdit:
				int oldPos = currItem->CPos; // 15-mar-2004 jjsa for cursor movement with Shift + Arrow key
				view->oldCp = currItem->CPos;
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					switch (kk)
					{
						case Key_Left:
							if (!currItem->locked())
							{
								if ( buttonState & ShiftButton )
									view->MoveItemI(-10, 0, currItem->ItemNr, true);
								else if ( buttonState & ControlButton )
									view->MoveItemI(-0.1, 0, currItem->ItemNr, true);
								else
									view->MoveItemI(-1, 0, currItem->ItemNr, true);
							}
							break;
						case Key_Right:
							if (!currItem->locked())
							{
								if ( buttonState & ShiftButton )
									view->MoveItemI(10, 0, currItem->ItemNr, true);
								else if ( buttonState & ControlButton )
									view->MoveItemI(0.1, 0, currItem->ItemNr, true);
								else
									view->MoveItemI(1, 0, currItem->ItemNr, true);
							}
							break;
						case Key_Up:
							if (!currItem->locked())
							{
								if ( buttonState & ShiftButton )
									view->MoveItemI(0, -10, currItem->ItemNr, true);
								else if ( buttonState & ControlButton )
									view->MoveItemI(0, -0.1, currItem->ItemNr, true);
								else
									view->MoveItemI(0, -1, currItem->ItemNr, true);
							}
							break;
						case Key_Down:
							if (!currItem->locked())
							{
								if ( buttonState & ShiftButton )
									view->MoveItemI(0, 10, currItem->ItemNr, true);
								else if ( buttonState & ControlButton )
									view->MoveItemI(0, 0.1, currItem->ItemNr, true);
								else
									view->MoveItemI(0, 1, currItem->ItemNr, true);
							}
							break;
					}
				}
				if (currItem->itemType() == PageItem::TextFrame)
				{
					view->slotDoCurs(false);
					switch (kk)
					{
					case Key_Prior:
					case Key_Next:
					case Key_End:
					case Key_Home:
					case Key_Right:
					case Key_Left:
					case Key_Up:
					case Key_Down:
						if ( (buttonState & ShiftButton) == 0 )
							view->deselectAll(currItem);
					}
					/* ISO 14755
					if ((buttonState & ControlButton) && (buttonState & ShiftButton))
					{
						if (!unicodeTextEditMode)
						{
							unicodeTextEditMode=true;
							unicodeInputCount = 0;
							unicodeInputString = "";
							keyrep = false;
						}
						qDebug(QString("%1 %2 %3 %4 %5").arg("uni").arg("c+s").arg(uc).arg(kk).arg(as));
					}
					*/
					if (unicodeTextEditMode)
					{
						int conv = 0;
						bool ok = false;
						unicodeInputString += uc;
						conv = unicodeInputString.toInt(&ok, 16);
						if (!ok)
						{
							unicodeTextEditMode = false;
							unicodeInputCount = 0;
							unicodeInputString = "";
							keyrep = false;
							return;
						}
						unicodeInputCount++;
						if (unicodeInputCount == 4)
						{
							unicodeTextEditMode = false;
							unicodeInputCount = 0;
							unicodeInputString = "";
							if (ok)
							{
								if (currItem->HasSel)
									deleteSelectedTextFromFrame(currItem);
								if (conv < 31)
									conv = 32;
								hg = new ScText;
								hg->ch = QString(QChar(conv));
								hg->cfont = (*doc->AllFonts)[doc->CurrFont];
								hg->csize = doc->CurrFontSize;
								hg->ccolor = doc->CurrTextFill;
								hg->cshade = doc->CurrTextFillSh;
								hg->cstroke = doc->CurrTextStroke;
								hg->cshade2 = doc->CurrTextStrokeSh;
								hg->cscale = doc->CurrTextScale;
								hg->cscalev = doc->CurrTextScaleV;
								hg->cbase = doc->CurrTextBase;
								hg->cshadowx = doc->CurrTextShadowX;
								hg->cshadowy = doc->CurrTextShadowY;
								hg->coutline = doc->CurrTextOutline;
								hg->cunderpos = doc->CurrTextUnderPos;
								hg->cunderwidth = doc->CurrTextUnderWidth;
								hg->cstrikepos = doc->CurrTextStrikePos;
								hg->cstrikewidth = doc->CurrTextStrikeWidth;
								hg->cselect = false;
								hg->cstyle = doc->CurrentStyle;
								hg->cab = doc->currentParaStyle;
								if (!doc->docParagraphStyles[doc->currentParaStyle].Font.isEmpty())
								{
									hg->cfont = (*doc->AllFonts)[doc->docParagraphStyles[doc->currentParaStyle].Font];
									hg->csize = doc->docParagraphStyles[doc->currentParaStyle].FontSize;
								}
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
								setTBvals(currItem);
								view->RefreshItem(currItem);
								keyrep = false;
								return;
							}
						}
						else
						{
							keyrep = false;
							return;
						}
					}
					switch (kk)
					{
					case Key_F12:
						unicodeTextEditMode = true;
						unicodeInputCount = 0;
						unicodeInputString = "";
						keyrep = false;
						return;
						break;
					case Key_Home:
						// go to begin of line
						if ( (pos = currItem->CPos) == 0 )
							break; // at begin of frame
						len = static_cast<int>(currItem->itemText.count());
						if ( pos == len )
							pos--;
						if ( (buttonState & ControlButton) == 0 )
						{
							alty =  currItem->itemText.at(pos)->yp;
							c = currItem->itemText.at(pos)->ch.at(0).latin1();
							if ( c == 13 ) // new line, position is wrong
								if ( --pos > 0 )
									alty =  currItem->itemText.at(pos)->yp;
							// check for yp at actual position
							if ( pos < len )
							{
								altx =  currItem->itemText.at(pos)->yp;
								if ( altx > alty )
								{
									// we was at begin of line
									break;
								}
							}
							while (  pos > 0 && currItem->itemText.at(pos-1)->yp == alty )
								--pos;
							if ( currItem->itemText.at(pos)->ch.at(0).latin1() == 13 )
								++pos;
						}
						else
						{
							// paragraph begin
							if ( pos < len &&
							        currItem->itemText.at(pos)->ch.at(0).latin1() == 13 )
								--pos;
							while(pos > 0 )
								if ( currItem->itemText.at(pos)->ch.at(0).latin1() == 13 )
								{
									++pos;
									break;
								}
								else
									--pos;
						}
						currItem->CPos = pos;
						if ( buttonState & ShiftButton )
							view->ExpandSel(currItem, -1, oldPos);
						break;
					case Key_End:
						// go to end of line
						len = static_cast<int>(currItem->itemText.count());
						if ( currItem->CPos >= len )
							break; // at end of frame
						if ( (buttonState & ControlButton) == 0 )
						{
							if ((currItem->CPos < len) && ((currItem->itemText.at(currItem->CPos)->ch.at(0).latin1() == 13) || (currItem->itemText.at(currItem->CPos)->ch.at(0).latin1() == 28)))
							{
								// at end of paragraph and therefore line
								break;
							}
							QString nextCh = currItem->itemText.at(currItem->CPos)->ch;
							int nextChs = currItem->itemText.at(currItem->CPos)->csize;
							alty =  currItem->itemText.at(currItem->CPos)->yp - currItem->SetZeichAttr(currItem->itemText.at(currItem->CPos), &nextChs, &nextCh);
							double nextY;
							while (currItem->CPos < len-1)
							{
								nextCh = currItem->itemText.at(currItem->CPos+1)->ch;
								nextChs = currItem->itemText.at(currItem->CPos+1)->csize;
								nextY = currItem->itemText.at(currItem->CPos+1)->yp - currItem->SetZeichAttr(currItem->itemText.at(currItem->CPos+1), &nextChs, &nextCh);
								if (fabs(nextY - alty) > 1.0)
									break;
								currItem->CPos++;
								if ( currItem->CPos == len-1)
									break;
							}
							if ( currItem->CPos < len -1 )
								c = currItem->itemText.at(currItem->CPos+1)->ch.at(0).latin1();
							else if ( currItem->CPos == len - 1 )
								c = 13;
							else
								c = 0;
							if (( c == 13 ) || (c = 28))
								currItem->CPos++;
						}
						else
						{
							// go to end of paragraph
							if ( currItem->itemText.at(currItem->CPos)->ch.at(0).latin1() == 13 )
							{
								break;
							}
							pos = currItem->CPos;
							while ( pos < len )
							{
								if ( currItem->itemText.at(pos)->ch.at(0).latin1() == 13 )
									break;
								else
									++pos;
							}
							currItem->CPos = pos;
						}
						if ( buttonState & ShiftButton )
							view->ExpandSel(currItem, 1, oldPos);
						break;
					case Key_Down:
						if (currItem->CPos != static_cast<int>(currItem->itemText.count()))
						{
							alty = currItem->itemText.at(currItem->CPos)->yp;
							altx = currItem->itemText.at(currItem->CPos)->xp;
							do
							{
								currItem->CPos += 1;
								if (currItem->CPos == static_cast<int>(currItem->itemText.count()))
									break;
								if (currItem->itemText.at(currItem->CPos)->yp > alty)
								{
									if (currItem->itemText.at(currItem->CPos)->xp >= altx)
										break;
								}
							}
							while (currItem->CPos < static_cast<int>(currItem->itemText.count()));
							if ( buttonState & ShiftButton )
							{
								if ( buttonState & AltButton )
									currItem->CPos = currItem->itemText.count();
								view->ExpandSel(currItem, 1, oldPos);
							}
							else
								if (currItem->CPos == static_cast<int>(currItem->itemText.count()))
									if (currItem->NextBox != 0)
									{
										if (currItem->NextBox->itemText.count() != 0)
										{
											view->Deselect(true);
											currItem->NextBox->CPos = 0;
											view->SelectItemNr(currItem->NextBox->ItemNr);
											currItem = currItem->NextBox;
										}
									}
						}
						else
						{
							if (currItem->NextBox != 0)
							{
								if (currItem->NextBox->itemText.count() != 0)
								{
									view->Deselect(true);
									currItem->NextBox->CPos = 0;
									view->SelectItemNr(currItem->NextBox->ItemNr);
									currItem = currItem->NextBox;
								}
							}
						}
						if ( currItem->HasSel )
							view->RefreshItem(currItem);
						setTBvals(currItem);
						break;
					case Key_Up:
						if (currItem->CPos > 0)
						{
							if (currItem->CPos == static_cast<int>(currItem->itemText.count()))
								--currItem->CPos;
							alty = currItem->itemText.at(currItem->CPos)->yp;
							altx = currItem->itemText.at(currItem->CPos)->xp;
							if (currItem->CPos > 0)
							{
								do
								{
									--currItem->CPos;
									if (currItem->CPos == 0)
										break;
									if  ( currItem->itemText.at(currItem->CPos)->ch.at(0).latin1() == 13 )
										break;
									if (currItem->itemText.at(currItem->CPos)->yp < alty)
									{
										if (currItem->itemText.at(currItem->CPos)->xp <= altx)
											break;
									}
								}
								while (currItem->CPos > 0);
							}
							if ( buttonState & ShiftButton )
							{
								if ( buttonState & AltButton )
									currItem->CPos = 0;
								view->ExpandSel(currItem, -1, oldPos);
							}
							else
								if (currItem->CPos == 0)
								{
									if (currItem->BackBox != 0)
									{
										view->Deselect(true);
										currItem->BackBox->CPos = currItem->BackBox->itemText.count();
										view->SelectItemNr(currItem->BackBox->ItemNr);
										currItem = currItem->BackBox;
									}
								}
						}
						else
						{
							currItem->CPos = 0;
							if (currItem->BackBox != 0)
							{
								view->Deselect(true);
								currItem->BackBox->CPos = currItem->BackBox->itemText.count();
								view->SelectItemNr(currItem->BackBox->ItemNr);
								currItem = currItem->BackBox;
							}
						}
						if ( currItem->HasSel )
							view->RefreshItem(currItem);
						setTBvals(currItem);
						break;
					case Key_Prior:
						currItem->CPos = 0;
						if ( buttonState & ShiftButton )
							view->ExpandSel(currItem, -1, oldPos);
						setTBvals(currItem);
						break;
					case Key_Next:
						currItem->CPos = static_cast<int>(currItem->itemText.count());
						if ( buttonState & ShiftButton )
							view->ExpandSel(currItem, 1, oldPos);
						setTBvals(currItem);
						break;
					case Key_Left:
						if ( buttonState & ControlButton )
						{
							view->setNewPos(currItem, oldPos, currItem->itemText.count(),-1);
							if ( buttonState & ShiftButton )
								view->ExpandSel(currItem, -1, oldPos);
						}
						else if ( buttonState & ShiftButton )
						{
							--currItem->CPos;
							if ( currItem->CPos < 0 )
								currItem->CPos = 0;
							else
								view->ExpandSel(currItem, -1, oldPos);
						}
						else
						{
							--currItem->CPos;
							if (currItem->CPos < 0)
							{
								currItem->CPos = 0;
								if (currItem->BackBox != 0)
								{
									view->Deselect(true);
									currItem->BackBox->CPos = currItem->BackBox->itemText.count();
									view->SelectItemNr(currItem->BackBox->ItemNr);
									currItem = currItem->BackBox;
								}
							}
						}
						if ((currItem->CPos > 0) && (currItem->CPos == static_cast<int>(currItem->itemText.count())))
						{
							if (currItem->itemText.at(currItem->CPos-1)->cstyle & 4096)
							{
								--currItem->CPos;
								while ((currItem->CPos > 0) && (currItem->itemText.at(currItem->CPos)->cstyle & 4096))
								{
									--currItem->CPos;
									if (currItem->CPos == 0)
										break;
								}
							}
						}
						else
						{
							while ((currItem->CPos > 0) && (currItem->itemText.at(currItem->CPos)->cstyle & 4096))
							{
								--currItem->CPos;
								if (currItem->CPos == 0)
									break;
							}
						}
						if ( currItem->HasSel )
							view->RefreshItem(currItem);
						setTBvals(currItem);
						break;
					case Key_Right:
						if ( buttonState & ControlButton )
						{
							view->setNewPos(currItem, oldPos, currItem->itemText.count(),1);
							if ( buttonState & ShiftButton )
								view->ExpandSel(currItem, 1, oldPos);
						}
						else if ( buttonState & ShiftButton )
						{
							++currItem->CPos;
							if ( currItem->CPos > static_cast<int>(currItem->itemText.count()) )
								--currItem->CPos;
							else
								view->ExpandSel(currItem, 1, oldPos);
						}
						else
						{
							++currItem->CPos; // new position within text ?
							if (currItem->CPos > static_cast<int>(currItem->itemText.count()))
							{
								--currItem->CPos;
								if (currItem->NextBox != 0)
								{
									if (currItem->NextBox->itemText.count() != 0)
									{
										view->Deselect(true);
										currItem->NextBox->CPos = 0;
										view->SelectItemNr(currItem->NextBox->ItemNr);
										currItem = currItem->NextBox;
									}
								}
							}
						}
						if ( currItem->HasSel )
							view->RefreshItem(currItem);
						setTBvals(currItem);
						break;
					case Key_Delete:
						if (currItem->CPos == static_cast<int>(currItem->itemText.count()))
						{
							if (currItem->HasSel)
							{
								deleteSelectedTextFromFrame(currItem);
								setTBvals(currItem);
								view->RefreshItem(currItem);
							}
							keyrep = false;
							return;
						}
						if (currItem->itemText.count() == 0)
						{
							keyrep = false;
							return;
						}
						cr = currItem->itemText.at(currItem->CPos)->ch;
						if (!currItem->HasSel)
							currItem->itemText.at(currItem->CPos)->cselect = true;
						deleteSelectedTextFromFrame(currItem);
						currItem->Tinput = false;
						if ((cr == QChar(13)) && (currItem->itemText.count() != 0))
						{
							view->chAbStyle(currItem, currItem->itemText.at(QMAX(currItem->CPos-1,0))->cab);
							currItem->Tinput = false;
						}
						setTBvals(currItem);
						view->RefreshItem(currItem);
						break;
					case Key_Backspace:
						if (currItem->CPos == 0)
						{
							if (currItem->HasSel)
							{
								deleteSelectedTextFromFrame(currItem);
								setTBvals(currItem);
								view->RefreshItem(currItem);
							}
							break;
						}
						if (currItem->itemText.count() == 0)
							break;
						cr = currItem->itemText.at(QMAX(currItem->CPos-1,0))->ch;
						if (!currItem->HasSel)
						{
							--currItem->CPos;
							currItem->itemText.at(currItem->CPos)->cselect = true;
						}
						deleteSelectedTextFromFrame(currItem);
						currItem->Tinput = false;
						if ((cr == QChar(13)) && (currItem->itemText.count() != 0))
						{
							view->chAbStyle(currItem, currItem->itemText.at(QMAX(currItem->CPos-1,0))->cab);
							currItem->Tinput = false;
						}
						setTBvals(currItem);
						view->RefreshItem(currItem);
						break;
					default:
						if ((currItem->HasSel) && (kk < 0x1000))
							deleteSelectedTextFromFrame(currItem);
//						if ((kk == Key_Tab) || ((kk == Key_Return) && (buttonState & ShiftButton)))
						if (kk == Key_Tab)
						{
							hg = new ScText;
//							if (kk == Key_Return)
//								hg->ch = QString(QChar(28));
							if (kk == Key_Tab)
								hg->ch = QString(QChar(9));
							hg->cfont = (*doc->AllFonts)[doc->CurrFont];
							hg->csize = doc->CurrFontSize;
							hg->ccolor = doc->CurrTextFill;
							hg->cshade = doc->CurrTextFillSh;
							hg->cstroke = doc->CurrTextStroke;
							hg->cshade2 = doc->CurrTextStrokeSh;
							hg->cscale = doc->CurrTextScale;
							hg->cscalev = doc->CurrTextScaleV;
							hg->cbase = doc->CurrTextBase;
							hg->cshadowx = doc->CurrTextShadowX;
							hg->cshadowy = doc->CurrTextShadowY;
							hg->coutline = doc->CurrTextOutline;
							hg->cunderpos = doc->CurrTextUnderPos;
							hg->cunderwidth = doc->CurrTextUnderWidth;
							hg->cstrikepos = doc->CurrTextStrikePos;
							hg->cstrikewidth = doc->CurrTextStrikeWidth;
							hg->cselect = false;
							hg->cstyle = doc->CurrentStyle;
							hg->cab = doc->currentParaStyle;
							if (!doc->docParagraphStyles[doc->currentParaStyle].Font.isEmpty())
							{
								hg->cfont = (*doc->AllFonts)[doc->docParagraphStyles[doc->currentParaStyle].Font];
								hg->csize = doc->docParagraphStyles[doc->currentParaStyle].FontSize;
							}
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
							break;
						}
						if (((uc[0] > QChar(31)) || (as == 13) || (as == 30)) && ((*doc->AllFonts)[doc->CurrFont]->CharWidth.contains(uc[0].unicode())))
						{
							hg = new ScText;
							hg->ch = uc;
							hg->cfont = (*doc->AllFonts)[doc->CurrFont];
							hg->ccolor = doc->CurrTextFill;
							hg->cshade = doc->CurrTextFillSh;
							hg->cstroke = doc->CurrTextStroke;
							hg->cshade2 = doc->CurrTextStrokeSh;
							hg->cscale = doc->CurrTextScale;
							hg->cscalev = doc->CurrTextScaleV;
							hg->csize = doc->CurrFontSize;
							hg->cbase = doc->CurrTextBase;
							hg->cshadowx = doc->CurrTextShadowX;
							hg->cshadowy = doc->CurrTextShadowY;
							hg->coutline = doc->CurrTextOutline;
							hg->cunderpos = doc->CurrTextUnderPos;
							hg->cunderwidth = doc->CurrTextUnderWidth;
							hg->cstrikepos = doc->CurrTextStrikePos;
							hg->cstrikewidth = doc->CurrTextStrikeWidth;
							hg->cextra = 0;
							hg->cselect = false;
							hg->cstyle = doc->CurrentStyle;
							hg->cab = doc->currentParaStyle;
							if (!doc->docParagraphStyles[doc->currentParaStyle].Font.isEmpty())
							{
								hg->cfont = (*doc->AllFonts)[doc->docParagraphStyles[doc->currentParaStyle].Font];
								hg->csize = doc->docParagraphStyles[doc->currentParaStyle].FontSize;
							}
							hg->xp = 0;
							hg->yp = 0;
							hg->PRot = 0;
							hg->PtransX = 0;
							hg->PtransY = 0;
							hg->cembedded = 0;
							currItem->itemText.insert(currItem->CPos, hg);
							currItem->CPos += 1;
							if ((doc->docHyphenator->AutoCheck) && (currItem->CPos > 1))
							{
								Twort = "";
								Tcoun = 0;
								for (int hych = currItem->CPos-1; hych > -1; hych--)
								{
									Tcha = currItem->itemText.at(hych)->ch;
									if (Tcha == " ")
									{
										Tcoun = hych+1;
										break;
									}
									Twort.prepend(Tcha);
								}
								if (!Twort.isEmpty())
								{
									if (doc->docHyphenator->Language != currItem->Language)
										doc->docHyphenator->slotNewDict(currItem->Language);
									doc->docHyphenator->slotHyphenateWord(currItem, Twort, Tcoun);
								}
							}
							currItem->Tinput = true;
							view->RefreshItem(currItem);
						}
						break;
					}
					view->slotDoCurs(true);
					if ((kk == Key_Left) || (kk == Key_Right) || (kk == Key_Up) || (kk == Key_Down))
					{
						keyrep = false;
						return;
					}
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

void ScribusApp::keyReleaseEvent(QKeyEvent *k)
{
	if (k->isAutoRepeat() || !_arrowKeyDown)
		return;
	switch(k->key())
	{
		case Key_Left:
		case Key_Right:
		case Key_Up:
		case Key_Down:
			_arrowKeyDown = false;
			if ((HaveDoc) && (!view->LE->hasFocus()) && (!view->PGS->focused()))
			{
				if ((view->SelItem.count() != 0) && (doc->appMode == modeNormal) && (doc->EditClip) && (view->ClRe != -1))
					view->updateContents();
				for (uint i = 0; i < view->SelItem.count(); ++i)
					view->SelItem.at(i)->checkChanges(true);
				if (view->SelItem.count() > 1 && view->groupTransactionStarted())
					undoManager->commit();
			}
	}
}

void ScribusApp::closeEvent(QCloseEvent *ce)
{
	QWidgetList windows = wsp->windowList();
	ScribusWin* tw;
	if (!windows.isEmpty())
	{
		singleClose = true;
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
		{
			newActWin(windows.at(i));
			tw = ActWin;
			ActWin->close();
			if (tw == ActWin)
			{
				ce->ignore();
				singleClose = false;
				return;
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
	}
	else
	{
		propertiesPalette->hide();
		outlinePalette->hide();
		scrapbookPalette->hide();
		bookmarkPalette->hide();
		layerPalette->hide();
		pagePalette->hide();
		measurementPalette->hide();
		docCheckerPalette->hide();
	}

	if (!emergencyActivated)
		prefsManager->SavePrefs();
	UndoManager::deleteInstance();
	if ((prefsManager->appPrefs.SaveAtQ) && (scrapbookPalette->changed()))
	{
		if (scrapbookPalette->getScrapbookFileName().isEmpty())
			scrapbookPalette->setScrapbookFileName(PrefsPfad+"/scrap13.scs");
		scrapbookPalette->Save();
	}
	if (scrapbookPalette->objectCount() == 0)
		unlink(PrefsPfad+"/scrap13.scs");
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	pluginManager->finalizePlugs();
	exit(0);
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void ScribusApp::parsePagesString(QString pages, std::vector<int>* pageNs, int sourcePageCount)
{
	QString tmp = pages;
	QString token;
	int from, to, pageNr;
	do
	{
		if (tmp.find(",") == -1)
		{
			token = tmp;
			tmp = "";
		}
		else
		{
			token = tmp.left(tmp.find(","));
			tmp = tmp.right(tmp.length() - tmp.find(",") - 1);
		}

		token = token.stripWhiteSpace();
		if (token == "*") // Import all source doc pages
		{
			for (int i = 1; i <= sourcePageCount; ++i)
				pageNs->push_back(i);
		}
		else if (token.find("-") != -1) // import a range of source doc pages
		{
			from = QString(token.left(token.find("-"))).toInt();
			to = QString(token.right(token.length() - token.find("-") - 1)).toInt();
			if ((from != 0) && (to != 0))
			{
				if (from > sourcePageCount)
					from = sourcePageCount;
				if (to > sourcePageCount)
					to = sourcePageCount;
				if (from == to)
					pageNs->push_back(to);
				else if (from < to)
				{
					for (int i = from; i <= to; ++i)
						pageNs->push_back(i);
				}
				else
				{
					for (int i = from; i >= to; --i)
						pageNs->push_back(i);
				}
			}
		}
		else // import single source doc page
		{
			pageNr = token.toInt();
			if ((pageNr > 0) && (pageNr <= sourcePageCount))
				pageNs->push_back(pageNr);
		}
	} while (!tmp.isEmpty());
}

bool ScribusApp::arrowKeyDown()
{
	return _arrowKeyDown;
}

void ScribusApp::startUpDialog()
{
	QString fileName = "";
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
			double pageWidth = dia->Pagebr;
			double pageHeight = dia->Pageho;
			double numberCols = dia->SpinBox10->value();
			bool autoframes = dia->AutoFrame->isChecked();
			int orientation = dia->Orient;
			PageSize *ps2 = new PageSize(dia->ComboBox1->currentText());
			QString pagesize = ps2->getPageName();
			doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->ComboBox3->currentItem(),
							firstPage, orientation, dia->PgNr->value(), pagesize);
			doc->pageSets[facingPages].FirstPage = firstPage;
			if (dia->PgNum->value() > 1)
			{
				for (int pg = 1; pg < dia->PgNum->value(); pg++)
				{
					slotNewPage(pg);
					applyNewMaster( tr("Normal"));
					doc->DocPages = doc->Pages;
				}
				outlinePalette->BuildTree(doc);
				pagePalette->RebuildPage();
				view->GotoPage(0);
			}
			delete ps2;
		}
		else
		{
			if (dia->tabSelected == 1)
			{
				fileName = dia->fileDialog->selectedFile();
				if (!fileName.isEmpty())
				{
					docContext->set("docsopen", fileName.left(fileName.findRev("/")));
					loadDoc(fileName);
				}
			}
			else
			{
				fileName = dia->recentDocList->currentText();
				if (!fileName.isEmpty())
					loadRecent(fileName);
			}
		}
	}
	prefsManager->setShowStartupDialog(!dia->startUpDialog->isChecked());
	delete dia;
	windowsMenuAboutToShow();
	mainWindowStatusLabel->setText( tr("Ready"));
}

bool ScribusApp::slotFileNew()
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
		double pageWidth = dia->Pagebr;
		double pageHeight = dia->Pageho;
		double numberCols = dia->SpinBox10->value();
		bool autoframes = dia->AutoFrame->isChecked();
		int orientation = dia->Orient;
		PageSize *ps2 = new PageSize(dia->ComboBox1->currentText());
		QString pagesize = ps2->getPageName();
		retVal = doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->ComboBox3->currentItem(),
		                firstPage, orientation, dia->PgNr->value(), pagesize);
		doc->pageSets[facingPages].FirstPage = firstPage;
		if (dia->PgNum->value() > 1)
		{
			for (int pg = 1; pg < dia->PgNum->value(); pg++)
			{
				slotNewPage(pg);
				applyNewMaster( tr("Normal"));
				doc->DocPages = doc->Pages;
			}
			outlinePalette->BuildTree(doc);
			pagePalette->RebuildPage();
			view->GotoPage(0);
		}
		mainWindowStatusLabel->setText( tr("Ready"));
		delete ps2;
	}
	else
		retVal = false;
	delete dia;
	windowsMenuAboutToShow();
	return retVal;
}

bool ScribusApp::doFileNew(double width, double h, double tpr, double lr, double rr, double br, double ab, double sp,
                           bool atf, int fp, int einh, int firstleft, int Ori, int SNr, const QString& defaultPageSize)
{
	QString cc;
	if (HaveDoc)
		doc->OpenNodes = outlinePalette->buildReopenVals();
	doc = new ScribusDoc();
	doc->setLoading(true);
	doc->setup(einh, fp, firstleft, Ori, SNr, defaultPageSize, doc->DocName+cc.setNum(DocNr));
	docCheckerPalette->clearErrorList();
	HaveDoc++;
	DocNr++;
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
	w->setView(view);
	ActWin = w;
	doc->WinHan = w;
	w->setCentralWidget(view);
	connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
	connect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));

	//	connect(w, SIGNAL(SaveAndClose()), this, SLOT(DoSaveClose()));
	if ((CMSavail) && (doc->CMSSettings.CMSinUse))
		recalcColors();
	doc->setPage(width, h, tpr, lr, rr, br, sp, ab, atf, fp);
	doc->setLoading(false);
	slotNewPage(0);
	doc->setLoading(true);
	HaveNewDoc();
	doc->DocPages = doc->Pages;
	doc->Pages = doc->MasterPages;
	doc->pageCount = doc->MasterPages.count();
	bool atfb = doc->PageAT;
	doc->PageAT = false;
	doc->masterPageMode = true;
	slotNewPage(0);
	doc->PageAT = atfb;
	doc->MasterNames["Normal"] = 0;
	doc->Pages.at(0)->setPageName("Normal");
	doc->MasterPages = doc->Pages;
	doc->pageCount = doc->DocPages.count();
	doc->Pages = doc->DocPages;
	doc->masterPageMode = false;
	doc->Pages.at(0)->MPageNam = "Normal";
	doc->setModified(false);
	doc->setLoading(false);
	doc->DocItems = doc->Items;
	doc->currentPage = doc->Pages.at(0);
	doc->OpenNodes.clear();

	//Independent finishing tasks after doc setup
	outlinePalette->BuildTree(doc);
	pagePalette->Rebuild();
	bookmarkPalette->BView->clear();
	if ( wsp->windowList().isEmpty() )
		w->showMaximized();
	else
		w->show();
	view->show();
	connect(doc->autoSaveTimer, SIGNAL(timeout()), w, SLOT(slotAutoSave()));
	connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	connect(fileWatcher, SIGNAL(fileChanged(QString)), view, SLOT(updatePict(QString)));
	connect(fileWatcher, SIGNAL(fileDeleted(QString)), view, SLOT(removePict(QString)));
	doc->AutoSave = prefsManager->appPrefs.AutoSave;
	doc->AutoSaveTime = prefsManager->appPrefs.AutoSaveTime;
	if (doc->AutoSave)
		doc->autoSaveTimer->start(doc->AutoSaveTime);
	scrActions["fileSave"]->setEnabled(false);
	undoManager->switchStack(doc->DocName);
	tocGenerator->setDoc(doc);
	return true;
}

void ScribusApp::newView()
{
	ScribusWin* w = new ScribusWin(wsp, doc);
	view = new ScribusView(w, doc);
	view->setScale(prefsManager->displayScale());
	w->setView(view);
	ActWin = w;
	w->setCentralWidget(view);
	alignDistributePalette->setView(view);
	connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
}

bool ScribusApp::DoSaveClose()
{
	return slotFileSave();
}

void ScribusApp::windowsMenuAboutToShow()
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
			scrWindowsActions.insert(docInWindow, new ScrAction( ScrAction::Window, QIconSet(), docInWindow, QKeySequence(), this, docInWindow, i));
			scrWindowsActions[docInWindow]->setToggleAction(true);
			connect( scrWindowsActions[docInWindow], SIGNAL(activatedData(int)), this, SLOT(windowsMenuActivated(int)) );
			scrMenuMgr->addMenuItem(scrWindowsActions[docInWindow], "Windows");
			scrWindowsActions[docInWindow]->setOn(wsp->activeWindow() == windows.at(i));
		}
	}
}

void ScribusApp::newActWin(QWidget *w)
{
	if (w == NULL)
	{
		ActWin = NULL;
		return;
	}
	QString oldDocName = "";
	if (ActWin && ActWin->doc)
		oldDocName = ActWin->doc->DocName;

	ActWin = (ScribusWin*)w;
/*	if (doc != NULL)
	{
		if ((HaveDoc) && (doc != ActWin->doc))
			doc->OpenNodes = outlinePalette->buildReopenVals();
	} */
	docCheckerPalette->clearErrorList();
	QString newDocName = "";
	if (ActWin && ActWin->doc)
		newDocName = ActWin->doc->DocName;
	if (oldDocName != newDocName)
		undoManager->switchStack(newDocName);

	if (view!=NULL)
	{
		actionManager->disconnectNewViewActions();
		disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	}
	doc = ActWin->doc;
	view = ActWin->view;
	actionManager->connectNewViewActions(view);
	connect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	pagePalette->SetView(view);
	alignDistributePalette->setView(view);
	ScribusWin* swin;
	if (!doc->isLoading())
	{
		scanDocument();
		docCheckerPalette->buildErrorList(doc);
		SwitchWin();
		QWidgetList windows = wsp->windowList();
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
		{
			swin = (ScribusWin*)windows.at(i);
			if (swin->muster != NULL)
				swin->muster->hide();
		}
		if (doc->masterPageMode)
			ActWin->muster->show();
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
	if (!doc->masterPageMode)
		pagePalette->Rebuild();
	outlinePalette->BuildTree(doc);
//	outlinePalette->reopenTree(doc->OpenNodes);
/*	bookmarkPalette->BView->NrItems = doc->NrItems;
	bookmarkPalette->BView->First = doc->First;
	bookmarkPalette->BView->Last = doc->Last; */
	RestoreBookMarks();
	if (!doc->isLoading())
	{
		if (view->SelItem.count() != 0)
		{
			HaveNewSel(view->SelItem.at(0)->itemType());
			view->EmitValues(view->SelItem.at(0));
		}
		else
			HaveNewSel(-1);
	}
	tocGenerator->setDoc(doc);
}

void ScribusApp::windowsMenuActivated( int id )
{
	if (HaveDoc)
		doc->OpenNodes = outlinePalette->buildReopenVals();
	QWidget* windowWidget = wsp->windowList().at( id );
	if ( windowWidget )
		windowWidget->showNormal();
	newActWin(windowWidget);
}

bool ScribusApp::SetupDoc()
{
	bool ret = false;
	if (doc->masterPageMode)
		doc->MasterItems = doc->Items;
	else
		doc->DocItems = doc->Items;
	ReformDoc* dia = new ReformDoc(this, doc);
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
		view->GotoPage(doc->currentPage->PageNr);
		view->DrawNew();
		propertiesPalette->ShowCMS();
		pagePalette->RebuildPage();
		slotDocCh();
		ret = true;
	}
	delete dia;
	return ret;
}

void ScribusApp::SwitchWin()
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
	propertiesPalette->Cpal->SetColors(doc->PageColors);
	propertiesPalette->Cpal->ChooseGrad(0);
	ActWin->setCaption(doc->DocName);
	scrActions["shade100"]->setOn(true);
	//ShadeMenu->setItemChecked(ShadeMenu->idAt(11), true);
	propertiesPalette->SetDoc(doc);
	propertiesPalette->updateCList();
	pagePalette->SetView(view);
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
	if (doc->masterPageMode)
	{
		scrActions["pageInsert"]->setEnabled(false);
		scrActions["pageDelete"]->setEnabled(false);
		scrActions["pageCopy"]->setEnabled(false);
		scrActions["pageMove"]->setEnabled(false);
		scrActions["pageApplyMasterPage"]->setEnabled(false);
		scrActions["editMasterPages"]->setEnabled(false);
		scrActions["fileNew"]->setEnabled(false);
		scrActions["fileSave"]->setEnabled(doc->isModified());
		scrActions["fileOpen"]->setEnabled(false);
		scrActions["fileClose"]->setEnabled(false);
		scrActions["fileRevert"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("FileOpenRecent", false);
		pagePalette->DisablePal();
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
		scrActions["fileSaveAs"]->setEnabled(ActWin->MenuStat[3]);
		scrActions["fileCollect"]->setEnabled(ActWin->MenuStat[3]);
		scrActions["fileRevert"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("FileOpenRecent", true);

		bool setter = doc->Pages.count() > 1 ? true : false;
		scrActions["pageDelete"]->setEnabled(setter);
		scrActions["pageMove"]->setEnabled(setter);

		if (doc->isModified())
			slotDocCh(false);
		scrActions["fileSaveAs"]->setEnabled(true);
		scrActions["fileCollect"]->setEnabled(true);
		pagePalette->EnablePal();
	}
}

void ScribusApp::HaveNewDoc()
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
/* Disabled this for 1.3.0 as it doesn't work yet */
/*	scrActions["pageImport"]->setEnabled(true); */
	scrActions["pageImport"]->setEnabled(false);
	//scrActions["toolsPreflightVerifier"]->setEnabled(true);

	if (HaveGS==0)
		scrActions["PrintPreview"]->setEnabled(true);
	if (scrActions["SaveAsDocumentTemplate"])
		scrActions["SaveAsDocumentTemplate"]->setEnabled(true);

	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	scrActions["editPaste"]->setEnabled(!Buffer2.isEmpty());
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

	bool setter = doc->Pages.count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);

	updateColorMenu();
	propertiesPalette->Cpal->SetColors(doc->PageColors);
	propertiesPalette->Cpal->ChooseGrad(0);
	ActWin->setCaption(doc->DocName);
	scrActions["shade100"]->setOn(true);
	propertiesPalette->SetDoc(doc);
	propertiesPalette->updateCList();
	pagePalette->SetView(view);
	propertiesPalette->Spal->setFormats(doc);
	propertiesPalette->SetLineFormats(doc);
	propertiesPalette->startArrow->rebuildList(&doc->arrowStyles);
	propertiesPalette->endArrow->rebuildList(&doc->arrowStyles);
	layerPalette->setLayers(&doc->Layers, doc->activeLayer());
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	doc->currentParaStyle = 0;
	slotChangeUnit(doc->unitIndex());
	doc->docHyphenator = new Hyphenator(this, doc, this);
	buildFontMenu();
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
	connect(view, SIGNAL(SetLineArt(PenStyle, PenCapStyle, PenJoinStyle)), propertiesPalette, SLOT( setLIvalue(PenStyle, PenCapStyle, PenJoinStyle)));
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
	connect(view, SIGNAL(ItemTextSize(int)), propertiesPalette, SLOT(setSize(int)));
	connect(view, SIGNAL(ItemRadius(double)), propertiesPalette, SLOT(setRR(double)));
	connect(view, SIGNAL(Amode(int)), this, SLOT(setAppMode(int)));
	connect(view, SIGNAL(PaintingDone()), this, SLOT(slotSelect()));
	connect(view, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
	connect(view, SIGNAL(MousePos(double, double)), this, SLOT(setMousePositionOnStatusBar(double, double)));
	connect(view, SIGNAL(ItemRadius(double)), propertiesPalette, SLOT(setRR(double)));
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
	connect(view, SIGNAL(LoadPic()), this, SLOT(slotFileOpen()));
	connect(view, SIGNAL(AppendText()), this, SLOT(slotFileAppend()));
	connect(view, SIGNAL(AnnotProps()), this, SLOT(ModifyAnnot()));
	connect(view, SIGNAL(EditGuides()), this, SLOT(ManageGuides()));
	connect(view, SIGNAL(LoadElem(QString, int ,int, bool, bool, ScribusDoc *, ScribusView*)), this, SLOT(slotElemRead(QString, int, int, bool, bool, ScribusDoc *, ScribusView*)));
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
	doc->PDF_Options.BleedBottom = doc->pageMargins.Bottom;
	doc->PDF_Options.BleedTop = doc->pageMargins.Top;
	doc->PDF_Options.BleedLeft = doc->pageMargins.Left;
	doc->PDF_Options.BleedRight = doc->pageMargins.Right;
	doc->CurTimer = NULL;
}

void ScribusApp::HaveNewSel(int Nr)
{
	PageItem *currItem = NULL;
	if (Nr != -1)
	{
		if (view->SelItem.count() != 0)
		{
			currItem = view->SelItem.at(0);
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
	scrActions["editEditWithImageEditor"]->setEnabled(Nr==PageItem::ImageFrame && currItem->PicAvail && currItem->isRaster);
	if (Nr!=PageItem::ImageFrame)
	{
		scrActions["itemImageIsVisible"]->setOn(false);
		scrActions["itemPreviewLow"]->setOn(false);
		scrActions["itemPreviewNormal"]->setOn(false);
		scrActions["itemPreviewFull"]->setOn(false);
	}
	if ((Nr==-1) || (Nr!=-1 && currItem->itemType()!=PageItem::TextFrame))
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
		scrMenuMgr->setMenuEnabled("Style", false);
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
		scrActions["editClearContents"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);
		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);
		scrMenuMgr->clearMenu("Style");

		scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		scrActions["toolsLinkTextFrame"]->setEnabled(false);
		scrActions["toolsEditContents"]->setEnabled(false);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
		scrActions["toolsRotate"]->setEnabled(false);
		scrActions["toolsCopyProperties"]->setEnabled(false);
		propertiesPalette->Cpal->gradientQCombo->setCurrentItem(0);
		outlinePalette->slotShowSelect(doc->currentPage->PageNr, -1);
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
		scrMenuMgr->setMenuEnabled("Style", true);
		scrMenuMgr->setMenuEnabled("Item", true);
		scrMenuMgr->setMenuEnabled("ItemShapes", !(currItem->isTableItem && currItem->isSingleSel));
		scrMenuMgr->setMenuEnabled("ItemConvertTo", true);
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(false);
		scrActions["itemConvertToOutlines"]->setEnabled(false);
		scrActions["itemConvertToPolygon"]->setEnabled(!currItem->isTableItem && doc->appMode != modeEdit);
		scrActions["itemConvertToTextFrame"]->setEnabled(doc->appMode != modeEdit);
		scrMenuMgr->clearMenu("Style");
		scrMenuMgr->addMenuToMenu("Color","Style");
		if (currItem->isRaster)
			scrMenuMgr->addMenuItem(scrActions["styleInvertPict"], "Style");
		scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		scrActions["toolsLinkTextFrame"]->setEnabled(false);
		scrActions["toolsEditContents"]->setEnabled(currItem->ScaleType);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
		scrActions["toolsRotate"]->setEnabled(true);
		scrActions["toolsCopyProperties"]->setEnabled(true);
		scrActions["itemImageIsVisible"]->setOn(currItem->PicArt);

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
		scrMenuMgr->setMenuEnabled("Style", true);
		scrMenuMgr->setMenuEnabled("Item", true);
		scrMenuMgr->setMenuEnabled("ItemShapes", !(currItem->isTableItem && currItem->isSingleSel));
		scrMenuMgr->setMenuEnabled("ItemConvertTo", true);
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(doc->appMode != modeEdit);
		scrActions["itemConvertToOutlines"]->setEnabled(!currItem->isTableItem && doc->appMode != modeEdit);
		scrActions["itemConvertToPolygon"]->setEnabled(!currItem->isTableItem && doc->appMode != modeEdit);
		scrActions["itemConvertToTextFrame"]->setEnabled(false);
		scrMenuMgr->clearMenu("Style");
		scrMenuMgr->addMenuToMenu("Font","Style");
		scrMenuMgr->addMenuToMenu("FontSize","Style");
		scrMenuMgr->addMenuToMenu("TypeEffects","Style");
		scrMenuMgr->addMenuToMenu("Alignment","Style");
		scrMenuMgr->addMenuToMenu("Color","Style");
		scrMenuMgr->addMenuToMenu("Shade","Style");
		scrMenuMgr->addMenuItem(scrActions["styleTabulators"], "Style");

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
			scrActions["toolsEditContents"]->setEnabled(true);
		if (currItem->NextBox == 0)
			scrActions["toolsLinkTextFrame"]->setEnabled(true);
		if (doc->masterPageMode)
			scrActions["toolsLinkTextFrame"]->setEnabled(false);
		if (doc->appMode == modeEdit)
		{
			setTBvals(currItem);
			scrActions["editSelectAll"]->setEnabled(true);
			scrActions["insertGlyph"]->setEnabled(true);
			if (currItem->itemType()==PageItem::TextFrame)
				actionManager->enableUnicodeActions(true);
			view->horizRuler->setItemPosition(currItem->Xpos, currItem->Width);
			if (currItem->lineColor() != "None")
				view->horizRuler->lineCorr = currItem->Pwidth / 2.0;
			else
				view->horizRuler->lineCorr = 0;
			view->horizRuler->ColGap = currItem->ColGap;
			view->horizRuler->Cols = currItem->Cols;
			view->horizRuler->Extra = currItem->Extra;
			view->horizRuler->RExtra = currItem->RExtra;
			view->horizRuler->First = doc->docParagraphStyles[doc->currentParaStyle].First;
			view->horizRuler->Indent = doc->docParagraphStyles[doc->currentParaStyle].Indent;
			if (currItem->imageFlippedH() || (currItem->Reverse))
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
			doc->CurrFont = currItem->IFont;
			doc->CurrFontSize = currItem->ISize;
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
		doc->docParagraphStyles[0].LineSpaMode = currItem->LineSpMode;
		doc->docParagraphStyles[0].LineSpa = currItem->LineSp;
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
		scrMenuMgr->setMenuEnabled("ItemConvertTo", false);
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(false);
		scrActions["itemConvertToOutlines"]->setEnabled(false);
		scrActions["itemConvertToPolygon"]->setEnabled(false);
		scrActions["itemConvertToTextFrame"]->setEnabled(false);

		scrMenuMgr->clearMenu("Style");
		scrMenuMgr->setMenuEnabled("Style", true);
		scrMenuMgr->addMenuToMenu("Font","Style");
		scrMenuMgr->addMenuToMenu("FontSize","Style");
		scrMenuMgr->addMenuToMenu("TypeEffects","Style");
		scrMenuMgr->addMenuToMenu("Color","Style");
		scrMenuMgr->addMenuToMenu("Shade","Style");

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
			doc->CurrFont = currItem->IFont;
			doc->CurrFontSize = currItem->ISize;
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
		scrMenuMgr->setMenuEnabled("Style", true);
		scrMenuMgr->setMenuEnabled("Item", true);
		scrMenuMgr->clearMenu("Style");
		scrMenuMgr->addMenuToMenu("Color","Style");
		scrMenuMgr->addMenuToMenu("Shade","Style");
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
	propertiesPalette->RotationGroup->setButton(doc->RotMode);
	if (view->SelItem.count() > 1)
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
		for (uint bx=0; bx<view->SelItem.count(); ++bx)
		{
			if (view->SelItem.at(bx)->itemType() != PageItem::Polygon)
				hPoly = false;
			if (view->SelItem.at(bx)->Groups.count() != 0)
			{
				if (view->SelItem.at(bx)->Groups.top() != firstElem)
					isGroup = false;
			}
			else
				isGroup = false;
		}
		scrActions["itemGroup"]->setEnabled(!isGroup);
		scrActions["itemCombinePolygons"]->setEnabled(hPoly);
		if (view->SelItem.count() == 2)
		{
			if (((currItem->itemType() == PageItem::TextFrame) || (view->SelItem.at(1)->itemType() == PageItem::TextFrame)) && ((currItem->itemType() == PageItem::PolyLine) || (view->SelItem.at(1)->itemType() == PageItem::PolyLine)))
			{
				PageItem* bx = view->SelItem.at(1);
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
	if (view->SelItem.count() != 0)
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
			scrActions["itemSplitPolygons"]->setEnabled( (currItem->itemType() == PageItem::Polygon) && (currItem->Segments.count() != 0) );
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
	propertiesPalette->NewSel(Nr);
	if (Nr != -1)
	{
		propertiesPalette->SetCurItem(currItem);
		outlinePalette->slotShowSelect(currItem->OwnPage, currItem->ItemNr);
		if (currItem->FrameType == 0)
			SCustom->setPixmap(SCustom->getIconPixmap(0));
		if (currItem->FrameType == 1)
			SCustom->setPixmap(SCustom->getIconPixmap(1));
		if (currItem->FrameType > 3)
			SCustom->setPixmap(SCustom->getIconPixmap(currItem->FrameType-2));
		actionManager->connectNewSelectionActions(view);
	}
}

void ScribusApp::slotDocCh(bool /*reb*/)
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
	if (!doc->masterPageMode)
	{
		scrActions["fileClose"]->setEnabled(true);
		if (doc->hasName)
			scrActions["fileRevert"]->setEnabled(true);
	}

	ActWin->MenuStat[0] = scrActions["fileSave"]->isEnabled();
	ActWin->MenuStat[1] = scrActions["fileClose"]->isEnabled();
	ActWin->MenuStat[2] = scrActions["fileSave"]->isEnabled();
	ActWin->MenuStat[3] = scrActions["fileSaveAs"]->isEnabled();
}

void ScribusApp::updateRecent(QString fn)
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

void ScribusApp::removeRecent(QString fn)
{
	if (RecentDocs.findIndex(fn) != -1)
	{
		RecentDocs.remove(fn);
		if (!fileWatcher->isActive())
			fileWatcher->removeFile(fn);
	}
	rebuildRecentFileMenu();
}

void ScribusApp::loadRecent(QString fn)
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

void ScribusApp::rebuildRecentFileMenu()
{
	for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrRecentFileActions.begin(); it!=scrRecentFileActions.end(); ++it )
		scrMenuMgr->removeMenuItem((*it), recentFileMenuName);

	scrRecentFileActions.clear();
	uint max = QMIN(prefsManager->appPrefs.RecentDCount, RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		QString strippedName=RecentDocs[m];
		strippedName.remove(QDir::separator());
		scrRecentFileActions.insert(strippedName, new ScrAction( ScrAction::RecentFile, QIconSet(), RecentDocs[m], QKeySequence(), this, strippedName));
		connect( scrRecentFileActions[strippedName], SIGNAL(activatedData(QString)), this, SLOT(loadRecent(QString)) );
		scrMenuMgr->addMenuItem(scrRecentFileActions[strippedName], recentFileMenuName);
	}
}

void ScribusApp::rebuildLayersList()
{
	if (HaveDoc)
	{
		for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it0 = scrLayersActions.begin(); it0 != scrLayersActions.end(); ++it0 )
			scrMenuMgr->removeMenuItem((*it0), layerMenuName);
		scrLayersActions.clear();
		uint a;
		QValueList<Layer>::iterator it;
		if (doc->Layers.count() != 0)
		{
			for (a=0; a < doc->Layers.count(); a++)
			{
				for (it = doc->Layers.begin(); it != doc->Layers.end(); ++it)
				{
					scrLayersActions.insert(QString("%1").arg((*it).LNr), new ScrAction( ScrAction::Layer, QIconSet(), (*it).Name, QKeySequence(), this, (*it).Name, (*it).LNr));
					scrLayersActions[QString("%1").arg((*it).LNr)]->setToggleAction(true);
				}
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

void ScribusApp::updateItemLayerList()
{
	if (HaveDoc)
	{
		for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=scrLayersActions.end(); ++it )
		{
			disconnect( (*it), SIGNAL(activatedData(int)), 0, 0 );
			(*it)->setOn(false);
		}
		if (view->SelItem.count()>0 && view->SelItem.at(0))
			scrLayersActions[QString("%1").arg(view->SelItem.at(0)->LayerNr)]->setOn(true);
		for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=scrLayersActions.end(); ++it )
			connect( (*it), SIGNAL(activatedData(int)), this, SLOT(sendToLayer(int)) );
	}
}

void ScribusApp::sendToLayer(int layerNumber)
{
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			if (UndoManager::undoEnabled() && view->SelItem.count() > 1)
				undoManager->beginTransaction();
			QString tooltip = Um::ItemsInvolved + "\n";
			for (uint a = 0; a < view->SelItem.count(); ++a)
			{
				PageItem *currItem = view->SelItem.at(a);
				currItem->setLayer(layerNumber);
				tooltip += "\t" + currItem->getUName() + "\n";
			}
			if (UndoManager::undoEnabled() && view->SelItem.count() > 1)
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

bool ScribusApp::slotDocOpen()
{
	PrefsContext* docContext = prefsManager->prefsFile->getContext("docdirs", false);
	QString docDir = ".";
	QString prefsDocDir=prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		docDir = docContext->get("docsopen", prefsDocDir);
	else
		docDir = docContext->get("docsopen", ".");
	QString formats = "";
#ifdef HAVE_LIBZ
	formats += tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;");
#else
	formats += tr("Documents (*.sla *.scd);;");
#endif
	if (pluginManager->DLLexists(6))
		formats += tr("PostScript Files (*.eps *.EPS *.ps *.PS);;");
	if (pluginManager->DLLexists(10))
#ifdef HAVE_LIBZ
		formats += tr("SVG Images (*.svg *.svgz);;");
#else
		formats += tr("SVG Images (*.svg);;");
#endif
	if (pluginManager->DLLexists(12))
		formats += tr("OpenOffice.org Draw (*.sxd);;");
	formats += tr("All Files (*)");
	QString fileName = CFileDialog( docDir, tr("Open"), formats);
	if (fileName.isEmpty())
		// User cancelled
		return false;
	docContext->set("docsopen", fileName.left(fileName.findRev("/")));
	bool ret = loadDoc(fileName);
	return ret;
}

bool ScribusApp::slotPageImport()
{
	bool ret = false;
	MergeDoc *dia = new MergeDoc(this, false, doc->pageCount, doc->currentPage->PageNr + 1);
	if (dia->exec())
	{
		mainWindowStatusLabel->setText( tr("Importing Pages..."));
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		std::vector<int> pageNs;
		parsePagesString(dia->getPageNumbers(), &pageNs, dia->getPageCounter());
		int startPage, nrToImport;
		bool doIt = true;
		if (doc->masterPageMode)
		{
			if (pageNs.size() > 1)
			{
				loadPage(dia->getFromDoc(), pageNs[0] - 1, false);
			}
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
				startPage = doc->pageCount + 1;
			addNewPages(dia->getImportWherePage(), importWhere, pageNs.size(), doc->pageHeight, doc->pageWidth, doc->PageOri, doc->PageSize, true);
			nrToImport = pageNs.size();
		}
		else
		{
			startPage = doc->currentPage->PageNr + 1;
			nrToImport = pageNs.size();
			if (pageNs.size() > (doc->pageCount - doc->currentPage->PageNr))
			{
				qApp->setOverrideCursor(QCursor(arrowCursor), true);
				QMessageBox mb( tr("Import Page(s)"),
				tr("<p>You are trying to import more pages than there are available "
				   "in the current document counting from the active page.</p>"
				"Choose one of the following:<br>"
				"<ul><li><b>Create</b> missing pages</li>"
				"<li><b>Import</b> pages until the last page</li>"
				"<li><b>Cancel</b></li></ul><br>"),
				QMessageBox::Information,
				QMessageBox::Yes | QMessageBox::Default,
				QMessageBox::No,
				QMessageBox::Cancel | QMessageBox::Escape );
				mb.setButtonText( QMessageBox::Yes, tr("Create") );
				mb.setButtonText( QMessageBox::No, tr("Import") );
				mb.setTextFormat(Qt::RichText);
				switch( mb.exec() ) {
					case QMessageBox::Yes:
						nrToImport = pageNs.size();
						addNewPages(doc->pageCount, 2, pageNs.size() - (doc->pageCount - doc->currentPage->PageNr), doc->pageHeight, doc->pageWidth, doc->PageOri, doc->PageSize, true);
					break;
					case QMessageBox::No:
						nrToImport = doc->pageCount - doc->currentPage->PageNr;
					break;
					case QMessageBox::Cancel:
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

bool ScribusApp::loadPage(QString fileName, int Nr, bool Mpa)
{
	bool ret = false;
	if (!fileName.isEmpty())
	{
		if (!Mpa)
			doc->OpenNodes = outlinePalette->buildReopenVals();
		doc->setLoading(true);
		ScriXmlDoc *ss = new ScriXmlDoc();
		uint cc = doc->Items.count();
		if(!ss->ReadPage(fileName, prefsManager->appPrefs.AvailFonts, doc, view, Nr, Mpa))
		{
			delete ss;
			doc->setLoading(false);
			return false;
		}
		delete ss;
		if (CMSavail)
		{
			if (doc->CMSSettings.CMSinUse)
			{
				recalcColors();
				view->RecalcPictures(&InputProfiles);
			}
		}
		for (uint azz = cc; azz < doc->Items.count(); ++azz)
		{
			PageItem *ite = doc->Items.at(azz);
			if ((ite->itemType() == PageItem::TextFrame) && (ite->isBookmark))
				bookmarkPalette->BView->AddPageItem(ite);
		}
		propertiesPalette->Cpal->SetColors(doc->PageColors);
		propertiesPalette->updateCList();
		propertiesPalette->Spal->setFormats(doc);
		propertiesPalette->SetLineFormats(doc);
		propertiesPalette->startArrow->rebuildList(&doc->arrowStyles);
		propertiesPalette->endArrow->rebuildList(&doc->arrowStyles);
		if (!Mpa)
		{
			outlinePalette->BuildTree(doc);
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
	view->DrawNew();
	return ret;
}

bool ScribusApp::loadDoc(QString fileName)
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
	int id = 0;
	for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
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
		QDir::setCurrent(fi.dirPath(true));
		FileLoader *fl = new FileLoader(FName, this);
		if (fl->TestFile() == -1)
		{
			delete fl;
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			QMessageBox::critical(this, tr("Fatal Error"), tr("File %1 \nis not in an acceptable format").arg(FName), CommonStrings::tr_OK);
			return false;
		}
		bool is12doc=false;
		if (fl->TestFile() == 0)
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
		doc->setActiveLayer(0);
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
		ScApp->ScriptRunning = true;
		if(!fl->LoadFile(this))
		{
			view->close();
			delete fl;
			delete view;
			delete doc;
			delete w;
			ScApp->ScriptRunning = false;
			mainWindowStatusLabel->setText("");
			mainWindowProgressBar->reset();
			ActWin = NULL;
			if (windows.count() != 0)
				newActWin(ActWinOld);
			return false;
		}
		view->unitChange();
		ScApp->ScriptRunning = false;
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
		connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
		if (fl->ReplacedFonts.count() != 0)
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			QString mess = tr("Some fonts used by this document have been substituted:")+"\n\n";
			QMap<QString,QString>::Iterator it;
			for (it = fl->ReplacedFonts.begin(); it != fl->ReplacedFonts.end(); ++it)
			{
				mess += it.key() + tr(" was replaced by: ")+ it.data() +"\n";
			}
			QMessageBox::warning(this, tr("Warning"), mess, 1, 0, 0);
		}
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
				QMessageBox::warning(this, tr("Warning"), mess, 1, 0, 0);
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
				view->RecalcPictures(&InputProfiles);
			}
		}
		else
		{
			doc->CMSSettings.CMSinUse = false;
#ifdef HAVE_CMS
			CMSuse = doc->CMSSettings.CMSinUse;
#endif
		}
		propertiesPalette->Cpal->SetColors(doc->PageColors);
		propertiesPalette->Cpal->ChooseGrad(0);
		if (fl->FileType > 1)
		{
			doc->setName(FName+tr("(converted)"));
			QFileInfo fi(doc->DocName);
			doc->setName(fi.fileName());
		}
		else
			doc->setName(FName);
		doc->masterPageMode = false;
		doc->Language = GetLang(doc->Language);
		HaveNewDoc();
		propertiesPalette->updateCList();
		doc->hasName = true;
		if (doc->MasterPages.count() == 0)
		{
			doc->DocPages = doc->Pages;
			doc->Pages = doc->MasterPages;
			doc->pageCount = doc->MasterPages.count();
			bool atf = doc->PageAT;
			doc->PageAT = false;
			slotNewPage(0);
			doc->PageAT = atf;
			doc->MasterNames["Normal"] = 0;
			doc->Pages.at(0)->setPageName("Normal");
			doc->MasterPages = doc->Pages;
			doc->pageCount = doc->DocPages.count();
			doc->Pages = doc->DocPages;
			doc->masterPageMode = false;
		}
		doc->setLoading(false);
		doc->DocItems = doc->Items;
		doc->RePos = true;
		QPixmap pgPix(10, 10);
		QRect rd = QRect(0,0,9,9);
		ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
		doc->Pages = doc->MasterPages;
		doc->masterPageMode = true;
		doc->Items = doc->MasterItems;
		for (uint azz=0; azz<doc->MasterItems.count(); ++azz)
		{
			PageItem *ite = doc->MasterItems.at(azz);
			// TODO fix that for Groups on Masterpages
//			if (ite->Groups.count() != 0)
//				view->GroupOnPage(ite);
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
			{
				if (ite->itemType() == PageItem::PathText)
				{
					ite->Frame = false;
					view->UpdatePolyClip(ite);
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
		}
//		RestoreBookMarks();
		doc->Pages = doc->DocPages;
		doc->masterPageMode = false;
		doc->Items = doc->DocItems;
		for (uint azz=0; azz<doc->Items.count(); ++azz)
		{
			PageItem *ite = doc->Items.at(azz);
			if (ite->Groups.count() != 0)
				view->GroupOnPage(ite);
			else
				ite->OwnPage = view->OnPage(ite);
			view->setRedrawBounding(ite);
			if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText) && (!ite->Redrawn))
			{
				if (ite->itemType() == PageItem::PathText)
				{
					ite->Frame = false;
					view->UpdatePolyClip(ite);
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
					ite->Frame = false;
					view->UpdatePolyClip(ite);
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
		for (uint p = 0; p < doc->Pages.count(); ++p)
		{
			Apply_MasterPage(doc->Pages.at(p)->MPageNam, p, false);
		}
		if (fl->FileType > 1)
		{
			doc->hasName = false;
			slotFileSaveAs();
		}
		delete fl;
		view->setUpdatesEnabled(true);
		w->setUpdatesEnabled(true);
		if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
			w->showMaximized();
		else
			w->show();
		view->show();
		newActWin(w);
		view->slotDoZoom();
		view->GotoPage(0);
		connect(doc->autoSaveTimer, SIGNAL(timeout()), w, SLOT(slotAutoSave()));
		connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
		connect(fileWatcher, SIGNAL(fileChanged(QString )), view, SLOT(updatePict(QString)));
		connect(fileWatcher, SIGNAL(fileDeleted(QString )), view, SLOT(removePict(QString)));
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
		pagePalette->Vie = 0;
	}
	undoManager->switchStack(doc->DocName);
	pagePalette->Rebuild();
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	undoManager->setUndoEnabled(true);
	return ret;
}

void ScribusApp::slotFileOpen()
{
	if (view->SelItem.count() != 0)
	{
		PageItem *currItem = view->SelItem.at(0);
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
				view->LoadPict(fileName, currItem->ItemNr);
				view->AdjustPictScale(currItem, false);
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				qApp->restoreOverrideCursor();
				view->DrawNew();
				propertiesPalette->Cpal->SetColors(doc->PageColors);
				propertiesPalette->updateCList();
				propertiesPalette->ShowCMS();
				//slotDocCh(); view->LoadPict does this now.
			}
		}
		if (currItem->itemType() == PageItem::TextFrame)
		{
			gtGetText* gt = new gtGetText();
			gt->run(false);
			delete gt;
			if (doc->docHyphenator->AutoCheck)
				doc->docHyphenator->slotHyphenate(currItem);
			for (uint a = 0; a < doc->Items.count(); ++a)
			{
				doc->Items.at(a)->ItemNr = a;
				if (doc->Items.at(a)->isBookmark)
					bookmarkPalette->BView->ChangeItem(doc->Items.at(a)->BMnr, a);
			}
			if (doc->masterPageMode)
				doc->MasterItems = doc->Items;
			else
				doc->DocItems = doc->Items;
			outlinePalette->BuildTree(doc);
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
void ScribusApp::slotFileAppend()
{
	if (view->SelItem.count() != 0)
	{
		gtGetText* gt = new gtGetText();
		gt->run(true);
		delete gt;
		if (doc->docHyphenator->AutoCheck)
			doc->docHyphenator->slotHyphenate(view->SelItem.at(0));
		view->DrawNew();
		slotDocCh();
	}
}

void ScribusApp::slotFileRevert()
{
	if ((doc->hasName) && (doc->isModified()) && (!doc->masterPageMode))
	{
		QString fn = doc->DocName;
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

void ScribusApp::slotAutoSaved()
{
	if (ActWin == sender())
	{
		scrActions["fileSave"]->setEnabled(false);
		ActWin->setCaption(doc->DocName);
	}
}

bool ScribusApp::slotFileSave()
{
	bool ret = false;
	if (doc->hasName)
	{
		//Scribus 1.3.x warning, remove at a later stage
		if (doc->is12doc)
			if (!warningVersion(this))
				return false;

		QString fn = doc->DocName;
		ret = DoFileSave(fn);
		if (!ret)
			QMessageBox::warning(this, tr("Warning"), tr("Cannot write the file: \n%1").arg(fn), CommonStrings::tr_OK);
	}
	else
		ret = slotFileSaveAs();
	return ret;
}

bool ScribusApp::slotFileSaveAs()
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
				QMessageBox::warning(this, tr("Warning"), tr("Cannot write the file: \n%1").arg(fn), CommonStrings::tr_OK);
			else
				doc->PDF_Options.Datei = ""; // #1482 reset the pdf file name
		}
	}
	mainWindowStatusLabel->setText( tr("Ready"));
	return ret;
}

bool ScribusApp::DoFileSave(QString fn)
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

bool ScribusApp::slotFileClose()
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

bool ScribusApp::DoFileClose()
{
	if (doc==storyEditor->currentDocument())
		storyEditor->close();
	actionManager->disconnectNewViewActions();
	disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	if (doc->viewCount > 1)
	{
		doc->viewCount--;
		if(doc->masterPageMode)
		{
			ActWin->muster->close();
			qApp->processEvents();
		}
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
	if(doc->masterPageMode)
	{
		ActWin->muster->close();
		qApp->processEvents();
	}
	setAppMode(modeNormal);
	doc->autoSaveTimer->stop();
	disconnect(doc->autoSaveTimer, SIGNAL(timeout()), doc->WinHan, SLOT(slotAutoSave()));
	disconnect(doc->WinHan, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	disconnect(fileWatcher, SIGNAL(fileChanged(QString )), view, SLOT(updatePict(QString)));
	disconnect(fileWatcher, SIGNAL(fileDeleted(QString )), view, SLOT(removePict(QString)));
	for (uint a = 0; a < doc->Items.count(); ++a)
	{
		PageItem *currItem = doc->Items.at(a);
		if (currItem->PicAvail)
			fileWatcher->removeFile(currItem->Pfile);
	}
	if (CMSavail)
		doc->CloseCMSProfiles();
//	propertiesPalette->NewSel(-1);
	propertiesPalette->UnsetDoc();
	pagePalette->Vie = 0;
	pagePalette->Rebuild();
	propertiesPalette->Spal->setFormats(0);
	propertiesPalette->SetLineFormats(0);
	if (doc->EditClip)
		nodePalette->doc = 0;
	bookmarkPalette->BView->clear();
	bookmarkPalette->BView->NrItems = 0;
	bookmarkPalette->BView->First = 1;
	bookmarkPalette->BView->Last = 0;
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
		propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
		propertiesPalette->Cpal->ChooseGrad(0);
		mainWindowStatusLabel->setText( tr("Ready"));
		PrinterUsed = false;
	}
	view->close();
	delete view;
	outlinePalette->itemMap.clear();
	outlinePalette->pageMap.clear();
	outlinePalette->masterPageMap.clear();
	outlinePalette->masterPageItemMap.clear();
	doc->setLoading(true);
	outlinePalette->reportDisplay->clear();
	layerPalette->ClearInhalt();
	docCheckerPalette->clearErrorList();
	HaveDoc--;
	view = NULL;
	delete doc;
	doc = NULL;
	ActWin = NULL;
	tocGenerator->setDoc(doc);
	return true;
}

void ScribusApp::slotFilePrint()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		scanDocument();
		if ((doc->docItemErrors.count() != 0) || (doc->masterItemErrors.count() != 0))
		{
			if (doc->checkerProfiles[doc->curCheckProfile].ignoreErrors)
			{
				int t = QMessageBox::warning(this, tr("Warning"),
											tr("Scribus has detected some errors.\nConsider using the Pre-flight Checker to correct them"),
											"<qt>"+tr("&Abort"), tr("&Ignore"), 0, 0, 0);
				if (t == 0)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(slotReallyPrint()));
				docCheckerPalette->noButton = false;
				docCheckerPalette->checkMode = 3;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setOn(true);
				return;
			}
		}
	}
	slotReallyPrint();
}

void ScribusApp::slotReallyPrint()
{
	if (!docCheckerPalette->noButton)
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = 0;
		docCheckerPalette->noButton = true;
		docCheckerPalette->ignoreErrors->hide();
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
	Druck *printer = new Druck(this, options.filename, options.printer, PDef.Command, prefsManager->appPrefs.GCRMode, spots);
	printer->setMinMax(1, doc->Pages.count(), doc->currentPage->PageNr+1);
	if (printer->exec())
	{
		ReOrderText(doc, view);
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		options.printer = printer->printerName();
		options.filename = printer->outputFileName();
		options.toFile = printer->outputToFile();
		if (printer->CurrentPage->isChecked())
			options.pageNumbers.push_back(doc->currentPage->PageNr+1);
		else
		{
			if (printer->RadioButton1->isChecked())
				parsePagesString("*", &options.pageNumbers, doc->pageCount);
			else
				parsePagesString(printer->PageNr->text(), &options.pageNumbers, doc->pageCount);
		}
		options.copies = printer->numCopies();
		options.outputSeparations = printer->outputSeparations();
		options.separationName = printer->separationName();
		options.allSeparations = printer->allSeparations();
		if (options.outputSeparations)
			options.useSpotColors = true;
		else
			options.useSpotColors = printer->doSpot;
		options.useColor = printer->color();
		options.mirrorH = printer->MirrorH;
		options.mirrorV = printer->MirrorV;
		options.useICC = printer->ICCinUse;
		options.doGCR = printer->DoGCR;
		options.PSLevel = printer->PSLevel;
		options.setDevParam = printer->doDev;
		PDef.Pname = options.printer;
		PDef.Dname = options.filename;
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
		if (!doPrint(&options))
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			QMessageBox::warning(this, tr("Warning"), tr("Printing failed!"), CommonStrings::tr_OK);
		}
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	delete printer;
	mainWindowStatusLabel->setText( tr("Ready"));
}

/*!
 \fn ScribusApp::doPrint()
 \author Franz Schmid
 \date
 \brief Generate and print PostScript from a doc
 \param options PrintOptions struct to control all settings
 \sa ScribusApp::slotFilePrint()
 \retval True for success
 */
bool ScribusApp::doPrint(PrintOptions *options)
{
	bool retw = false;
	QMap<QString,QFont> ReallyUsed;
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
			dd->CreatePS(doc, view, options->pageNumbers, options->outputSeparations, options->separationName, options->allSeparations,
			               options->useColor, options->mirrorH, options->mirrorV, options->useICC, options->doGCR, options->setDevParam);
			ScColor::UseProf = true;
			if (options->PSLevel != 3)
			{
				// use gs to convert our PS to a lower version
				QString tmp;
				QString opts = "-dDEVICEWIDTHPOINTS=";
				opts += tmp.setNum(doc->pageWidth);
				opts += " -dDEVICEHEIGHTPOINTS=";
				opts += tmp.setNum(doc->pageHeight);
				convertPS2PS(filename, filename + ".tmp", opts, options->PSLevel);
			#ifndef _WIN32
				system("mv \""+filename+".tmp\" \""+filename+"\"");
			#else
				system("move /y \""+filename+".tmp\" \""+filename+"\"");
			#endif
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

void ScribusApp::slotFileQuit()
{
	propertiesPalette->UnsetDoc();
	pluginManager->savePreferences();
	close();
}

void ScribusApp::slotEditCut()
{
	uint a;
	NoFrameEdit();
	QString BufferI = "";
	if ((HaveDoc) && (view->SelItem.count() != 0))
	{
		PageItem *currItem;
		for (uint i = 0; i < view->SelItem.count(); ++i)
		{
			currItem=view->SelItem.at(i);
			if ((currItem->itemType()==PageItem::TextFrame || currItem->itemType()==PageItem::PathText) && currItem==ScApp->storyEditor->currentItem() && doc==ScApp->storyEditor->currentDocument())
			{
					QMessageBox::critical(ScApp, tr("Cannot Cut In-Use Item"), tr("The item %1 is currently being edited by Story Editor. The cut operation will be cancelled").arg(currItem->itemName()), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
					return;
			}
		}
		if (UndoManager::undoEnabled())
		{
			if (view->SelItem.count() > 1)
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Cut,"",Um::ICut);
			else
				undoManager->beginTransaction(view->SelItem.at(0)->getUName(),
											  view->SelItem.at(0)->getUPixmap(), Um::Cut, "", Um::ICut);
		}
		Buffer2 = "<SCRIBUSTEXT>";
		currItem = view->SelItem.at(0);
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
						Buffer2 += nextItem->itemText.at(a)->cfont->SCName+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->csize)+"\t";
						Buffer2 += nextItem->itemText.at(a)->ccolor+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cextra)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshade)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cstyle)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cab)+'\t';
						Buffer2 += nextItem->itemText.at(a)->cstroke+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshade2)+'\t';
						Buffer2 += QString::number(nextItem->itemText.at(a)->cscale)+"\t";
						Buffer2 += QString::number(currItem->itemText.at(a)->cscalev)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cbase)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cshadowx)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cshadowy)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->coutline)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cunderpos)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cunderwidth)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cstrikepos)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cstrikewidth)+'\n';
					}
				}
				deleteSelectedTextFromFrame(nextItem);
				nextItem = nextItem->NextBox;
			}
			view->RefreshItem(currItem);
		}
		else
		{
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			ScriXmlDoc *ss = new ScriXmlDoc();
			BufferI = ss->WriteElem(&view->SelItem, doc, view);
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

void ScribusApp::slotEditCopy()
{
	uint a;
	NoFrameEdit();
	QString BufferI = "";
	if ((HaveDoc) && (view->SelItem.count() != 0))
	{
		Buffer2 = "<SCRIBUSTEXT>";
		PageItem *currItem = view->SelItem.at(0);
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
						Buffer2 += nextItem->itemText.at(a)->cfont->SCName+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->csize)+"\t";
						Buffer2 += nextItem->itemText.at(a)->ccolor+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cextra)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshade)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cstyle)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cab)+"\t";
						Buffer2 += nextItem->itemText.at(a)->cstroke+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cshade2)+"\t";
						Buffer2 += QString::number(nextItem->itemText.at(a)->cscale)+"\t";
						Buffer2 += QString::number(currItem->itemText.at(a)->cscalev)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cbase)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cshadowx)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cshadowy)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->coutline)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cunderpos)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cunderwidth)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cstrikepos)+'\t';
						Buffer2 += QString::number(currItem->itemText.at(a)->cstrikewidth)+'\n';
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
			BufferI = ss->WriteElem(&view->SelItem, doc, view);
			Buffer2 = BufferI;
			delete ss;
		}
		BuFromApp = true;
		ClipB->setText(BufferI);
		scrActions["editPaste"]->setEnabled(true);
	}
}

void ScribusApp::slotEditPaste()
{
	struct ScText *hg;
	NoFrameEdit();
	if (HaveDoc)
	{
		if (Buffer2.isNull())
			return;
		if (doc->appMode == modeEdit)
		{
			PageItem *currItem = view->SelItem.at(0);
			if (currItem->HasSel)
				deleteSelectedTextFromFrame(currItem);
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
						hg->cstroke = "None";
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
				uint ac = doc->Items.count();
				bool isGroup = false;
				double gx, gy, gh, gw;
				FPoint minSize = doc->minCanvasCoordinate;
				FPoint maxSize = doc->maxCanvasCoordinate;
				doc->useRaster = false;
				doc->SnapGuides = false;
				slotElemRead(Buffer2, 0, 0, false, true, doc, view);
				doc->useRaster = savedAlignGrid;
				doc->SnapGuides = savedAlignGuides;
				QPtrList<PageItem> bSel = view->SelItem;
				view->SelItem.clear();
				if (doc->Items.count() - ac > 1)
					isGroup = true;
				for (uint as = ac; as < doc->Items.count(); ++as)
				{
					view->SelItem.append(doc->Items.at(as));
					if (isGroup)
						doc->Items.at(as)->Groups.push(doc->GroupCounter);
				}
				if (isGroup)
					doc->GroupCounter++;
				view->setGroupRect();
				view->getGroupRect(&gx, &gy, &gw, &gh);
				PageItem* currItem3 = doc->Items.at(ac);
				for (uint as = ac; as < doc->Items.count(); ++as)
				{
					PageItem* currItem2 = doc->Items.at(as);
					currItem2->isEmbedded = true;
					currItem2->isAnnotation = false;
					currItem2->isBookmark = false;
					currItem2->gXpos = currItem2->Xpos - gx;
					currItem2->gYpos = currItem2->Ypos - gy;
					currItem2->gWidth = gw;
					currItem2->gHeight = gh;
					currItem2->ItemNr = doc->FrameItems.count();
					doc->FrameItems.append(currItem2);
				}
				uint acc = doc->Items.count();
				for (uint as = ac; as < acc; ++as)
				{
					doc->Items.take(ac);
				}
				if (doc->masterPageMode)
					doc->MasterItems = doc->Items;
				else
					doc->DocItems = doc->Items;
				view->SelItem.clear();
				view->SelItem = bSel;
				view->resizeContents(qRound((maxSize.x() - minSize.x()) * view->getScale()), qRound((maxSize.y() - minSize.y()) * view->getScale()));
				view->scrollBy(qRound((doc->minCanvasCoordinate.x() - minSize.x()) * view->getScale()), qRound((doc->minCanvasCoordinate.y() - minSize.y()) * view->getScale()));
				doc->minCanvasCoordinate = minSize;
				doc->maxCanvasCoordinate = maxSize;
				outlinePalette->BuildTree(doc);
				outlinePalette->reopenTree(doc->OpenNodes);
				hg = new ScText;
				hg->ch = QChar(25);
				hg->cfont = (*doc->AllFonts)[doc->CurrFont];
				hg->csize = doc->CurrFontSize;
				hg->ccolor = doc->CurrTextFill;
				hg->cshade = doc->CurrTextFillSh;
				hg->cstroke = doc->CurrTextStroke;
				hg->cshade2 = doc->CurrTextStrokeSh;
				hg->cscale = doc->CurrTextScale;
				hg->cscalev = doc->CurrTextScaleV;
				hg->cbase = doc->CurrTextBase;
				hg->cshadowx = doc->CurrTextShadowX;
				hg->cshadowy = doc->CurrTextShadowY;
				hg->coutline = doc->CurrTextOutline;
				hg->cunderpos = doc->CurrTextUnderPos;
				hg->cunderwidth = doc->CurrTextUnderWidth;
				hg->cstrikepos = doc->CurrTextStrikePos;
				hg->cstrikewidth = doc->CurrTextStrikeWidth;
				hg->cselect = false;
				hg->cstyle = doc->CurrentStyle;
				hg->cab = doc->currentParaStyle;
				if (!doc->docParagraphStyles[doc->currentParaStyle].Font.isEmpty())
				{
					hg->cfont = (*doc->AllFonts)[doc->docParagraphStyles[doc->currentParaStyle].Font];
					hg->csize = doc->docParagraphStyles[doc->currentParaStyle].FontSize;
				}
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
					ss->GetText(currItem, st, currItem->IFont, currItem->ISize, true);
				delete ss;
			}
			view->RefreshItem(currItem);
		}
		else
		{
			if (Buffer2.startsWith("<SCRIBUSELEM"))
			{
				view->Deselect(true);
				uint ac = doc->Items.count();
				bool savedAlignGrid = doc->useRaster;
				bool savedAlignGuides = doc->SnapGuides;
				doc->useRaster = false;
				doc->SnapGuides = false;
				slotElemRead(Buffer2, 0, 0, false, true, doc, view);
				doc->useRaster = savedAlignGrid;
				doc->SnapGuides = savedAlignGuides;
				for (uint as = ac; as < doc->Items.count(); ++as)
				{
					PageItem* currItem = doc->Items.at(as);
					if (currItem->isBookmark)
						AddBookMark(currItem);
					view->SelectItemNr(as);
				}
			}
		}
		slotDocCh(false);
	}
}

void ScribusApp::SelectAll()
{
	if (doc->appMode == modeEdit)
	{
		PageItem *currItem = view->SelItem.at(0);
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
		for (uint a = 0; a < doc->Items.count(); ++a)
		{
			currItem = doc->Items.at(a);
			if (currItem->LayerNr == doc->activeLayer())
			{
				if (!currItem->Select)
				{
					view->SelItem.append(currItem);
					currItem->Select = true;
					currItem->FrameOnly = true;
					currItem->paintObj();
				}
			}
		}
		if (view->SelItem.count() > 1)
		{
			view->setGroupRect();
			view->paintGroupRect();
			double x, y, w, h;
			view->getGroupRect(&x, &y, &w, &h);
			propertiesPalette->setXY(x, y);
			propertiesPalette->setBH(w, h);
		}
		if (view->SelItem.count() > 0)
		{
			currItem = view->SelItem.at(0);
			view->EmitValues(currItem);
			HaveNewSel(currItem->itemType());
		}
	}
}

void ScribusApp::deselectAll()
{
	if (view!=NULL)
		view->Deselect(true);
}

void ScribusApp::ClipChange()
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
		if (HaveDoc)
		{
			if (cc.startsWith("<SCRIBUSELEM"))
			{
				scrActions["editPaste"]->setEnabled(true);
			}
			else
			{
				if (doc->appMode == modeEdit)
					scrActions["editPaste"]->setEnabled(true);
			}
		}
	}
}

void ScribusApp::clearContents()
{
	view->ClearItem();
}

void ScribusApp::EnableTxEdit()
{
	scrActions["editCut"]->setEnabled(true);
	scrActions["editCopy"]->setEnabled(true);
	//scrActions["editClearContents"]->setEnabled(true);
}

void ScribusApp::DisableTxEdit()
{
	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	//scrActions["editClearContents"]->setEnabled(false);
}

void ScribusApp::slotHelpAbout()
{
	About* dia = new About(this);
	dia->exec();
	delete dia;
}

void ScribusApp::slotHelpAboutQt()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void ScribusApp::slotOnlineHelp()
{
	HelpBrowser *dia = new HelpBrowser(this, tr("Scribus Manual"), ScApp->guiLanguage);
	dia->show();
}

void ScribusApp::ToggleTips()
{
	QToolTip::setGloballyEnabled(scrActions["helpTooltips"]->isOn());
}

void ScribusApp::SaveText()
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
		se->PutText(view->SelItem.at(0));
		se->Write(LoadEnc);
		delete se;
	}
}

void ScribusApp::applyNewMaster(QString name)
{
	Apply_MasterPage(name, doc->currentPage->PageNr);
	view->DrawNew();
	slotDocCh();
	pagePalette->Rebuild();
}

void ScribusApp::slotNewPageP(int wo, QString templ)
{
	NoFrameEdit();
	view->Deselect(true);
	slotNewPage(wo);
	applyNewMaster(templ);
	if (doc->masterPageMode)
		doc->MasterPages = doc->Pages;
	else
		doc->DocPages = doc->Pages;
	outlinePalette->BuildTree(doc);
	pagePalette->RebuildPage();
}

/** Erzeugt eine neue Seite */
void ScribusApp::slotNewPageM()
{
	NoFrameEdit();
	view->Deselect(true);
	QStringList base;
	InsPage *dia = new InsPage(this, doc, doc->currentPage->PageNr, doc->Pages.count());
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

void ScribusApp::addNewPages(int wo, int where, int numPages, double height, double width, int orient, QString siz, bool mov, QStringList* basedOn)
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
	switch (where)
	{
	case 0:
		wot -= 1;
		for (cc = 0; cc < numPages; ++cc)
		{
			slotNewPage(wot, mov);
			doc->currentPage->initialHeight = height;
			doc->currentPage->initialWidth = width;
			doc->currentPage->PageOri = orient;
			doc->currentPage->PageSize = siz;
			applyNewMaster(base[(doc->currentPage->PageNr+doc->pageSets[doc->currentPageLayout].FirstPage) % doc->pageSets[doc->currentPageLayout].Columns]);
			wot ++;
		}
		break;
	case 1:
		for (cc = 0; cc < numPages; ++cc)
		{
			slotNewPage(wot, mov);
			doc->currentPage->initialHeight = height;
			doc->currentPage->initialWidth = width;
			doc->currentPage->PageOri = orient;
			doc->currentPage->PageSize = siz;
			applyNewMaster(base[(doc->currentPage->PageNr+doc->pageSets[doc->currentPageLayout].FirstPage) % doc->pageSets[doc->currentPageLayout].Columns]);
			wot ++;
		}
		break;
	case 2:
		for (cc = 0; cc < numPages; ++cc)
		{
			slotNewPage(doc->Pages.count(), mov);
			doc->currentPage->initialHeight = height;
			doc->currentPage->initialWidth = width;
			doc->currentPage->PageOri = orient;
			doc->currentPage->PageSize = siz;
			applyNewMaster(base[(doc->currentPage->PageNr+doc->pageSets[doc->currentPageLayout].FirstPage) % doc->pageSets[doc->currentPageLayout].Columns]);
		}
		break;
	}
	pagePalette->RebuildPage();
	view->reformPages(mov);
	view->DrawNew();
	if (doc->masterPageMode)
		doc->MasterPages = doc->Pages;
	else
		doc->DocPages = doc->Pages;
	outlinePalette->BuildTree(doc);
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusApp::slotNewMasterPage(int w)
{
	if (doc->masterPageMode)
		slotNewPage(w);
}

void ScribusApp::slotNewPage(int w, bool mov)
{
	view->addPage(w, mov);
/*	if ((!doc->loading) && (!doc->masterPageMode))
		outlinePalette->BuildTree(doc); */
	bool setter = doc->Pages.count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);
	if ((!doc->isLoading()) && (!doc->masterPageMode))
		AdjustBM();
/*	if ((!doc->loading) && (!doc->masterPageMode))
	{
		AdjustBM();
		if ((doc->PageAT) && (doc->pageCount != 1))
			outlinePalette->slotAddElement(w, 0);
	}
	slotDocCh(!doc->loading); */
}

/*!
	\fn void ScribusApp::slotZoom(double zoomFactor)
	\author Craig Bradney
	\date Sun 30 Jan 2005
	\brief Take the ScApp zoom actions and pass the view a %. Actions have whole number values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
	\param zoomFactor Value stored in the ScrAction.
 */
void ScribusApp::slotZoom(double zoomFactor)
{
	double finalZoomFactor;
	//Zoom to Fit
	if (zoomFactor==-100.0)
	{
		double dx = (view->width()-50) / (doc->pageWidth+30);
		double dy = (view->height()-70) / (doc->pageHeight+30);
		if (dx > dy)
			finalZoomFactor=dy;
		else
			finalZoomFactor=dx;
	}
	//Zoom to %
	else
		finalZoomFactor = zoomFactor*prefsManager->displayScale()/100.0;

	view->setScale(finalZoomFactor);
	view->slotDoZoom();
}

void ScribusApp::ToggleAllPalettes()
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

void ScribusApp::toggleCheckPal()
{
	PalettesStat[0] = false;
}

void ScribusApp::setUndoPalette(bool visible)
{
	undoPalette->setShown(visible);
	scrActions["toolsActionHistory"]->setOn(visible);
}
/*
void ScribusApp::togglePropertiesPalette()
{
	PalettesStat[0] = false;
}

void ScribusApp::toggleOutlinePalette()
{
	PalettesStat[0] = false;
}

void ScribusApp::toggleScrapbookPalette()
{
	PalettesStat[0] = false;
}

void ScribusApp::toggleLayerPalette()
{
	PalettesStat[0] = false;
}
*/
void ScribusApp::setPagePalette(bool visible)
{
	if (!visible)
	{
		prefsManager->appPrefs.SepalT = pagePalette->masterPageList->Thumb;
		prefsManager->appPrefs.SepalN = pagePalette->PageView->Namen;
	}
}

void ScribusApp::togglePagePalette()
{
	setPagePalette(!pagePalette->isVisible());
	PalettesStat[0] = false;
}
/*
void ScribusApp::toggleBookmarkPalette()
{
	PalettesStat[0] = false;
}
*/

void ScribusApp::toggleUndoPalette()
{
	setUndoPalette(!undoPalette->isVisible());
	PalettesStat[0] = false;
}

void ScribusApp::setTools(bool visible)
{
	mainToolBar->setShown(visible);
	mainToolBar->Sichtbar = visible;
	scrActions["toolsToolbarTools"]->setOn(visible);
}

void ScribusApp::ToggleTools()
{
	setTools(!mainToolBar->Sichtbar);
}

void ScribusApp::setPDFTools(bool visible)
{
	pdfToolBar->setShown(visible);
	pdfToolBar->Sichtbar = visible;
	scrActions["toolsToolbarPDF"]->setOn(visible);
}

void ScribusApp::TogglePDFTools()
{
	setPDFTools(!pdfToolBar->Sichtbar);
}

void ScribusApp::TogglePics()
{
	doc->guidesSettings.showPic = !doc->guidesSettings.showPic;
	for (uint b=0; b<doc->Items.count(); ++b)
	{
		if (doc->Items.at(b)->itemType() == PageItem::ImageFrame)
			doc->Items.at(b)->PicArt = doc->guidesSettings.showPic;
	}
	view->DrawNew();
}

void ScribusApp::ToggleAllGuides()
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

void ScribusApp::ToggleMarks()
{
	GuidesStat[0] = false;
	doc->guidesSettings.marginsShown = !doc->guidesSettings.marginsShown;
	view->DrawNew();
}

void ScribusApp::ToggleFrames()
{
	GuidesStat[0] = false;
	doc->guidesSettings.framesShown = !doc->guidesSettings.framesShown;
	view->DrawNew();
}

void ScribusApp::ToggleRaster()
{
	GuidesStat[0] = false;
	doc->guidesSettings.gridShown = !doc->guidesSettings.gridShown;
	view->DrawNew();
}

void ScribusApp::ToggleGuides()
{
	GuidesStat[0] = false;
	doc->guidesSettings.guidesShown = !doc->guidesSettings.guidesShown;
	view->DrawNew();
}

void ScribusApp::ToggleBase()
{
	GuidesStat[0] = false;
	doc->guidesSettings.baseShown = !doc->guidesSettings.baseShown;
	view->DrawNew();
}

void ScribusApp::ToggleTextLinks()
{
	GuidesStat[0] = false;
	doc->guidesSettings.linkShown = !doc->guidesSettings.linkShown;
	view->DrawNew();
}

void ScribusApp::ToggleTextControls()
{
	GuidesStat[0] = false;
	doc->guidesSettings.showControls = !doc->guidesSettings.showControls;
	view->DrawNew();
}

void ScribusApp::ToggleRuler()
{
	GuidesStat[0] = false;
	doc->guidesSettings.rulerMode = !doc->guidesSettings.rulerMode;
	view->DrawNew();
}

void ScribusApp::ToggleURaster()
{
	doc->useRaster = !doc->useRaster;
}

void ScribusApp::ToggleUGuides()
{
	doc->SnapGuides = !doc->SnapGuides;
}

void ScribusApp::ToggleFrameEdit()
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
		if (view->SelItem.count() != 0)
		{
			PageItem *currItem = view->SelItem.at(0);
			view->MarkClip(currItem, currItem->PoLine, true);
			nodePalette->EditCont->setEnabled(currItem->ContourLine.size() != 0);
			nodePalette->ResetCont->setEnabled(false);
			nodePalette->PolyStatus(currItem->itemType(), currItem->PoLine.size());
		}
	}
	scrActions["itemShapeEdit"]->setOn(doc->EditClip);
}

void ScribusApp::NoFrameEdit()
{
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
		if (view->SelItem.count() != 0)
		{
			HaveNewSel(view->SelItem.at(0)->itemType());
			view->DrawNew();
		}
		else
			HaveNewSel(-1);
	}
	actionManager->connectModeActions();
}

void ScribusApp::slotSelect()
{
	pdfToolBar->PDFTool->setOn(false);
	pdfToolBar->PDFaTool->setOn(false);
	setAppMode(modeNormal);
}

void ScribusApp::setAppModeByToggle(bool isOn, int newMode)
{
	keyrep=false;
	if (isOn)
		setAppMode(newMode);
	else
		setAppMode(modeNormal);
}

void ScribusApp::setAppMode(int mode)
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

	PageItem *currItem;
	setActiveWindow();
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
			currItem = view->SelItem.at(0);
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
			view->LE->setFocusPolicy(QWidget::ClickFocus);
			view->PGS->focusPolicy(QWidget::ClickFocus);
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
			view->LE->setFocusPolicy(QWidget::NoFocus);
			view->PGS->focusPolicy(QWidget::NoFocus);
			if (currItem != 0)
			{
				if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::PathText))
				{
					doc->appMode = 1;
					ToggleFrameEdit();
					return;
				}
				setTBvals(currItem);
			}
			scrActions["editPaste"]->setEnabled(false);
			scrActions["insertGlyph"]->setEnabled(true);
			if (currItem!=NULL && currItem->itemType()==PageItem::TextFrame)
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
		if (mode == modeDrawBezierLine)
		{
			if (view->SelItem.count() != 0)
				view->Deselect(true);
			view->FirstPoly = true;
		}
		if (mode == modeEditGradientVectors)
			propertiesPalette->Cpal->gradEditButton->setOn(true);
		if (mode == modeMeasurementTool)
		{
			measurementPalette->show();
			connect(view, SIGNAL(MVals(double, double, double, double, double, double, int)), measurementPalette, SLOT(setValues(double, double, double, double, double, double, int )));
		}
		switch (mode)
		{
			case modeDrawShapes:
				if (view->SelItem.count() != 0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")), true);
				break;
			case modeDrawPicture:
				if (view->SelItem.count() != 0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")), true);
				break;
			case modeDrawText:
				if (view->SelItem.count() != 0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")), true);
				break;
			case modeDrawTable:
				if (view->SelItem.count() != 0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawTable.xpm")), true);
				break;
			case modeDrawRegularPolygon:
				if (view->SelItem.count() != 0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("DrawPolylineFrame.xpm")), true);
				break;
			case modeMagnifier:
				if (view->SelItem.count() != 0)
					view->Deselect(true);
				qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
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
				if (view->SelItem.count() != 0)
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
			propertiesPalette->Cpal->gradEditButton->setOn(false);
		}
		if (mode == modeLinkFrames)
			doc->ElemToLink = view->SelItem.at(0);
		if ((mode == modeLinkFrames) || (mode == modeUnlinkFrames))
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
			if (view->SelItem.count() != 0)
			{
				doc->ElemToLink = view->SelItem.at(0);
				view->Deselect(true);
				scrActions["toolsCopyProperties"]->setEnabled(true);
			}
		}
	}
	actionManager->connectModeActions();
}

void ScribusApp::setMainWindowActive()
{
	setActiveWindow();
	raise();
}

void ScribusApp::setItemTypeStyle(int id)
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

void ScribusApp::setStilvalue(int s)
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

void ScribusApp::setItemHoch(int h)
{
	if (view->SelItem.count() != 0)
	{
		//setActiveWindow();
		doc->CurrentStyle = h;
		setStilvalue(doc->CurrentStyle);
		view->chTyStyle(h);
		slotDocCh();
	}
}

void ScribusApp::AdjustBM()
{
	for (uint b = 0; b < doc->Items.count(); ++b)
	{
		PageItem* bb = doc->Items.at(b);
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

void ScribusApp::DeletePage2(int pg)
{
	PageItem* ite;
	NoFrameEdit();
	if (doc->Pages.count() == 1)
		return;
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(doc->DocName, Um::IDocument, Um::DeletePage, "", Um::IDelete);
/*	if (!doc->masterPageMode)
		disconnect(doc->currentPage, SIGNAL(DelObj(uint, uint)), outlinePalette, SLOT(slotRemoveElement(uint, uint))); */
	view->SelItem.clear();
	for (uint d = 0; d < doc->Items.count(); ++d)
	{
		ite = doc->Items.at(d);
		if (ite->OwnPage == pg)
		{
			ite->setLocked(false);
			ite->isSingleSel = false;
			if (ite->isBookmark)
				DelBookMark(ite);
			ite->isBookmark = false;
			view->SelItem.append(ite);
		}
	}
	if (view->SelItem.count() != 0)
		view->DeleteItem();
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::DeletePage, "", Um::ICreate);
		ss->set("DELETE_PAGE", "delete_page");
		ss->set("PAGENR", pg + 1);
		ss->set("MASTERPAGE", doc->Pages.at(pg)->MPageNam);
		// replace the deleted page in the undostack by a dummy object that will
		// replaced with the "undone" page if user choose to undo the action
		DummyUndoObject *duo = new DummyUndoObject();
		uint id = static_cast<uint>(duo->getUId());
		undoManager->replaceObject(doc->Pages.at(pg)->getUId(), duo);
		ss->set("DUMMY_ID", id);
		undoManager->action(this, ss);
	}
	view->delPage(pg);
	view->reformPages();
	AdjustBM();
	view->DrawNew();
	doc->OpenNodes.clear();
	outlinePalette->BuildTree(doc);
	bool setter = doc->Pages.count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);
	slotDocCh();
	pagePalette->RebuildPage();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusApp::DeletePage()
{
	NoFrameEdit();
	view->Deselect(true);
	DelPages *dia = new DelPages(this, doc->currentPage->PageNr+1, doc->Pages.count());
	if (dia->exec())
		DeletePage(dia->getFromPage(), dia->getToPage());
	delete dia;
}

void ScribusApp::DeletePage(int from, int to)
{
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(doc->DocName, Um::IDocument,
									  (from - to == 0) ? Um::DeletePage : Um::DeletePages, "",
									  Um::IDelete);
	PageItem* ite;
	view->SelItem.clear();
	for (int a = to - 1; a >= from - 1; a--)
	{
		for (uint d = 0; d < doc->Items.count(); ++d)
		{
			ite = doc->Items.at(d);
			if (ite->OwnPage == a)
			{
				ite->setLocked(false);
				ite->isSingleSel = false;
				if (ite->isBookmark)
					DelBookMark(ite);
				ite->isBookmark = false;
				view->SelItem.append(ite);
			}
		}
		AdjustBM();
	}
	if (view->SelItem.count() != 0)
		view->DeleteItem();
	for (int a = to - 1; a >= from - 1; a--)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::DeletePage, "", Um::ICreate);
			ss->set("DELETE_PAGE", "delete_page");
			ss->set("PAGENR", a + 1);
			ss->set("MASTERPAGE", doc->Pages.at(a)->MPageNam);
			// replace the deleted page in the undostack by a dummy object that will
			// replaced with the "undone" page if user choose to undo the action
			DummyUndoObject *duo = new DummyUndoObject();
			uint id = static_cast<uint>(duo->getUId());
			undoManager->replaceObject(doc->Pages.at(a)->getUId(), duo);
			ss->set("DUMMY_ID", id);
			undoManager->action(this, ss);
		}
		view->delPage(a);
	}
	view->reformPages();
	view->DrawNew();
//	doc->OpenNodes.clear();
	outlinePalette->BuildTree(doc);
	bool setter = doc->Pages.count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);
	slotDocCh();
	pagePalette->RebuildPage();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusApp::MovePage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->currentPage->PageNr+1, doc->Pages.count(), true);
	if (dia->exec())
	{
//		doc->OpenNodes = outlinePalette->buildReopenVals();
		int from = dia->getFromPage();
		int to = dia->getToPage();
		int wie = dia->getWhere();
		int wo = dia->getWherePage();
		if (from != wo)
			view->movePage(from-1, to, wo-1, wie);
		slotDocCh();
		view->DrawNew();
		AdjustBM();
		pagePalette->RebuildPage();
		outlinePalette->BuildTree(doc);
		outlinePalette->reopenTree(doc->OpenNodes);
	}
	delete dia;
}

void ScribusApp::CopyPage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->currentPage->PageNr+1, doc->Pages.count(), false);
	if (dia->exec())
	{
		doc->setLoading(true);
		Page* from = doc->Pages.at(dia->getFromPage()-1);
		int wo = dia->getWherePage();
		switch (dia->getWhere())
		{
		case 0:
			slotNewPage(wo-1);
			break;
		case 1:
			slotNewPage(wo);
			break;
		case 2:
			slotNewPage(doc->Pages.count());
			break;
		}
		Page* Ziel = doc->currentPage;
		Ziel->initialHeight = from->Height;
		Ziel->initialWidth = from->Width;
		Ziel->PageOri = from->PageOri;
		Ziel->PageSize = from->PageSize;
		Ziel->initialMargins.Top = from->Margins.Top;
		Ziel->initialMargins.Bottom = from->Margins.Bottom;
		Ziel->initialMargins.Left = from->Margins.Left;
		Ziel->initialMargins.Right = from->Margins.Right;
		QMap<int,int> TableID;
		QPtrList<PageItem> TableItems;
		TableID.clear();
		TableItems.clear();
		uint oldItems = doc->Items.count();
		for (uint ite = 0; ite < oldItems; ++ite)
		{
			if (doc->Items.at(ite)->OwnPage == static_cast<int>(from->PageNr))
			{
				doc->Items.at(ite)->copyToCopyPasteBuffer(&Buffer);
				Buffer.Xpos = Buffer.Xpos - from->Xoffset + Ziel->Xoffset;
				Buffer.Ypos = Buffer.Ypos - from->Yoffset + Ziel->Yoffset;
				view->PasteItem(&Buffer, true, true);
				PageItem* Neu = doc->Items.at(doc->Items.count()-1);
				Neu->OnMasterPage = "";
				if (doc->Items.at(ite)->isBookmark)
					AddBookMark(Neu);
				if (Neu->isTableItem)
				{
					TableItems.append(Neu);
					TableID.insert(ite, Neu->ItemNr);
				}
			}
		}
		if (TableItems.count() != 0)
		{
			for (uint ttc = 0; ttc < TableItems.count(); ++ttc)
			{
				PageItem* ta = TableItems.at(ttc);
				if (ta->TopLinkID != -1)
					ta->TopLink = doc->Items.at(TableID[ta->TopLinkID]);
				else
					ta->TopLink = 0;
				if (ta->LeftLinkID != -1)
					ta->LeftLink = doc->Items.at(TableID[ta->LeftLinkID]);
				else
					ta->LeftLink = 0;
				if (ta->RightLinkID != -1)
					ta->RightLink = doc->Items.at(TableID[ta->RightLinkID]);
				else
					ta->RightLink = 0;
				if (ta->BottomLinkID != -1)
					ta->BottomLink = doc->Items.at(TableID[ta->BottomLinkID]);
				else
					ta->BottomLink = 0;
			}
		}
		Apply_MasterPage(from->MPageNam, Ziel->PageNr, false);
		if (from->YGuides.count() != 0)
		{
			for (uint y = 0; y < from->YGuides.count(); ++y)
			{
				if (Ziel->YGuides.contains(from->YGuides[y]) == 0)
					Ziel->YGuides.append(from->YGuides[y]);
			}
			qHeapSort(Ziel->YGuides);
		}
		if (from->XGuides.count() != 0)
		{
			for (uint x = 0; x < from->XGuides.count(); ++x)
			{
				if (Ziel->XGuides.contains(from->XGuides[x]) == 0)
					Ziel->XGuides.append(from->XGuides[x]);
			}
			qHeapSort(Ziel->XGuides);
		}
		view->Deselect(true);
		doc->setLoading(false);
		view->reformPages();
		view->DrawNew();
		slotDocCh();
		pagePalette->RebuildPage();
		outlinePalette->BuildTree(doc);
		AdjustBM();
	}
	delete dia;
}

void ScribusApp::changePageMargins()
{
	NoFrameEdit();
	MarginDialog *dia = new MarginDialog(this, doc);
	if (dia->exec())
	{
		doc->changePageMargins(dia->GroupRand->RandT, dia->GroupRand->RandB, dia->GroupRand->RandL, dia->GroupRand->RandR, dia->pageHeight, dia->pageWidth, dia->pageHeight, dia->pageWidth, dia->orientationQComboBox->currentItem(), dia->prefsPageSizeName, doc->currentPage->PageNr);
		view->reformPages(dia->moveObjects->isChecked());
		view->DrawNew();
		slotDocCh();
	}
	delete dia;
}

void ScribusApp::setItemFont2(int id)
{
	disconnect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
	SetNewFont(FontSub->text(id));
	FontMenu->activateItemAt(0);
	connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
}

void ScribusApp::setItemFont(int id)
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

void ScribusApp::SetNewFont(const QString& nf)
{
	setMainWindowActive();
	QString nf2(nf);
	if (!doc->UsedFonts.contains(nf))
	{
		if (doc->AddFont(nf, prefsManager->appPrefs.AvailFonts[nf]->Font))
		{
			int a = FontMenu->insertItem(new FmItem(nf, prefsManager->appPrefs.AvailFonts[nf]->Font));
			FontID.insert(a, prefsManager->appPrefs.AvailFonts[nf]->SCName);
		}
		else
		{
			if (view->SelItem.count() != 0)
			{
				PageItem *currItem = view->SelItem.at(0);
				nf2 = currItem->IFont;
			}
			propertiesPalette->Fonts->RebuildList(doc);
			buildFontMenu();
		}
	}
	AdjustFontMenu(nf2);
	view->ItemFont(nf2);
	doc->CurrFont = nf2;
	slotDocCh();
}

void ScribusApp::AdjustFontMenu(QString nf)
{
	QString df;
	FontSub->setCurrentText(nf);
	propertiesPalette->Fonts->setCurrentFont(nf);
	for (uint a = 2; a < FontMenu->count(); ++a)
	{
		df = FontID[FontMenu->idAt(a)];
		FontMenu->setItemChecked(FontMenu->idAt(a), (df == nf));
	}
}

void ScribusApp::setItemFSize(int id)
{
	int c = id;
	if (c != -1)
		view->chFSize(c*10);
	else
	{
		bool ok = false;
		Query* dia = new Query(this, "New", 1, 0, tr("&Size:"), tr("Size"));
		if (dia->exec())
		{
			c = qRound(dia->getEditText().toDouble(&ok));
			if ((ok) && (c < 1025) && (c > 0))
				view->chFSize(c*10);
			delete dia;
		}
	}
	propertiesPalette->setSize(c*10);
	slotDocCh();
}

void ScribusApp::setFSizeMenu(int size)
{
	if (scrActions[QString("fontSize%1").arg(size/10)])
		scrActions[QString("fontSize%1").arg(size/10)]->setOn(true);
}

void ScribusApp::setItemFarbe(int id)
{
	if (view->SelItem.count() != 0)
	{
		PageItem *currItem = view->SelItem.at(0);
		if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
			view->ItemTextBrush(ColorMenC->text(id));
		else
			view->ItemBrush(ColorMenC->text(id));
	}
	scrMenuMgr->getLocalPopupMenu("Color")->activateItemAt(0);
	slotDocCh();
}

void ScribusApp::setItemShade(int id)
{
	int c = id;
	bool ok = false;
	if (view->SelItem.count() != 0)
	{
		PageItem *currItem = view->SelItem.at(0);
		if (c != -1)
		{
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				view->ItemTextBrushS(c);
			else
				view->ItemBrushShade(c);
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
						view->ItemTextBrushS(c);
					else
						view->ItemBrushShade(c);
				}
				delete dia;
			}
		}
	}
	slotDocCh();
}

void ScribusApp::setCSMenu(QString , QString l, int  , int ls)
{
	uint a;
	QString la;
	int lb;
	PageItem *currItem;
	if (view->SelItem.count() != 0)
	{
		currItem = view->SelItem.at(0);
		if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
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
	if (la == "None")
		la = tr("None");
	for (a = 0; a < static_cast<uint>(ColorMenC->count()); ++a)
	{
		if (ColorMenC->text(a) == la)
			ColorMenC->setCurrentItem(a);
	}
	if (scrActions[QString("shade%1").arg(lb)])
		scrActions[QString("shade%1").arg(lb)]->setOn(true);
}

void ScribusApp::slotEditLineStyles()
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

void ScribusApp::saveLStyles(LineFormate *dia)
{
	if (doc->masterPageMode)
		doc->MasterItems = doc->Items;
	else
		doc->DocItems = doc->Items;
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

void ScribusApp::slotEditStyles()
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

void ScribusApp::saveStyles(StilFormate *dia)
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
	if (doc->masterPageMode)
		doc->MasterItems = doc->Items;
	else
		doc->DocItems = doc->Items;
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
				ers.append(0);
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
				counter = doc->Items.count();
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
					ite = doc->Items.at(d);
					break;
				case 2:
					ite = doc->FrameItems.at(d);
					break;
			}
			if (ite->itemType() == PageItem::TextFrame)
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
							ite->itemText.at(e)->csize = ite->ISize;
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
							chars->at(e)->csize = ite->ISize;
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
				if (doc->AddFont(nf, prefsManager->appPrefs.AvailFonts[nf]->Font))
				{
					int ff = FontMenu->insertItem(new FmItem(nf, prefsManager->appPrefs.AvailFonts[nf]->Font));
					FontID.insert(ff, prefsManager->appPrefs.AvailFonts[nf]->SCName);
				}
				else
					doc->docParagraphStyles[a].Font = doc->toolSettings.defFont;
			}
		}
	}
	propertiesPalette->Spal->updateFormatList();
	propertiesPalette->Cpal->SetColors(doc->PageColors);
	propertiesPalette->updateCList();
	disconnect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
	ColorList::Iterator it;
	updateColorMenu();
	view->DrawNew();
	slotDocCh();
}

void ScribusApp::setNewAbStyle(int a)
{
	setActiveWindow();
	if (HaveDoc)
	{
		doc->currentParaStyle = a;
		view->SetAbStyle(a);
		propertiesPalette->setAli(a);
		PageItem *currItem = view->SelItem.at(0);
		setTBvals(currItem);
		slotDocCh();
	}
}

void ScribusApp::setAbsValue(int a)
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

void ScribusApp::slotEditColors()
{
	ColorList edc;
	if (HaveDoc)
		edc = doc->PageColors;
	else
		edc = prefsManager->colorSet();
	Farbmanager* dia = new Farbmanager(this, edc, HaveDoc, prefsManager->colorSetName(), prefsManager->appPrefs.CustomColorSets);
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
			propertiesPalette->Cpal->SetColors(doc->PageColors);
			propertiesPalette->updateCList();
			updateColorMenu();
			ers = dia->Ersatzliste;
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
						{
							ite->setFillColor(it.data());
							if (ite->fillColor() != "None")
								ite->fillQColor = doc->PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
						}
						if (it.key() == ite->lineColor())
						{
							ite->setLineColor(it.data());
							if (ite->lineColor() != "None")
								ite->strokeQColor = doc->PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
						}
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
						{
							ite->setFillColor(it.data());
							if (ite->fillColor() != "None")
								ite->fillQColor = doc->PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
						}
						if (it.key() == ite->lineColor())
						{
							ite->setLineColor(it.data());
							if (ite->lineColor() != "None")
								ite->strokeQColor = doc->PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
						}
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
						{
							ite->setFillColor(it.data());
							if (ite->fillColor() != "None")
								ite->fillQColor = doc->PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
						}
						if (it.key() == ite->lineColor())
						{
							ite->setLineColor(it.data());
							if (ite->lineColor() != "None")
								ite->strokeQColor = doc->PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
						}
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
			for (c=0; c<doc->DocItems.count(); ++c)
			{
				ite = doc->DocItems.at(c);
				if (ite->lineColor() != "None")
					ite->strokeQColor = doc->PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
				if (ite->fillColor() != "None")
					ite->fillQColor = doc->PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
			}
			for (c=0; c<doc->MasterItems.count(); ++c)
			{
				ite = doc->MasterItems.at(c);
				if (ite->lineColor() != "None")
					ite->strokeQColor = doc->PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
				if (ite->fillColor() != "None")
					ite->fillQColor = doc->PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
			}
			for (c=0; c<doc->FrameItems.count(); ++c)
			{
				ite = doc->FrameItems.at(c);
				if (ite->lineColor() != "None")
					ite->strokeQColor = doc->PageColors[ite->lineColor()].getShadeColorProof(ite->lineShade());
				if (ite->fillColor() != "None")
					ite->fillQColor = doc->PageColors[ite->fillColor()].getShadeColorProof(ite->fillShade());
			}
			view->DrawNew();
		}
		else
		{
			prefsManager->setColorSet(dia->EditColors);
			prefsManager->setColorSetName(dia->LoadColSet->text());
			propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
		}
	}
	if (!HaveDoc)
		prefsManager->appPrefs.CustomColorSets = dia->CColSet;
	delete dia;
}

void ScribusApp::setPenFarbe(QString farbe)
{
	if (HaveDoc)
	{
		view->ItemPen(farbe);
		slotDocCh();
	}
}

void ScribusApp::setPenShade(int sh)
{
	//setActiveWindow();
	if (HaveDoc)
	{
		view->ItemPenShade(sh);
		slotDocCh();
	}
}

void ScribusApp::setBrushFarbe(QString farbe)
{
	if (HaveDoc)
	{
		view->ItemBrush(farbe);
		slotDocCh();
	}
}

void ScribusApp::setBrushShade(int sh)
{
	//setActiveWindow();
	if (HaveDoc)
	{
		view->ItemBrushShade(sh);
		slotDocCh();
	}
}

void ScribusApp::setGradFill(int typ)
{
	if (HaveDoc)
	{
		view->ItemGradFill(typ);
		slotDocCh();
	}
}

void ScribusApp::updtGradFill()
{
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			PageItem *currItem = view->SelItem.at(0);
			currItem->fill_gradient = propertiesPalette->Cpal->gradEdit->Preview->fill_gradient;
			view->RefreshItem(currItem);
			slotDocCh();
		}
	}
}

void ScribusApp::GetBrushPen()
{
	setActiveWindow();
	if (HaveDoc)
	{
		view->QueryFarben();
		slotDocCh();
	}
}

void ScribusApp::MakeFrame(int f, int c, double *vals)
{
	PageItem *currItem = view->SelItem.at(0);
	switch (f)
	{
	case 0:
		view->SetRectFrame(currItem);
		break;
	case 1:
		view->SetOvalFrame(currItem);
		break;
	default:
		view->SetFrameShape(currItem, c, vals);
		currItem->FrameType = f+2;
		break;
	}
	propertiesPalette->SetCurItem(currItem);
	view->RefreshItem(currItem);
	slotDocCh();
}

void ScribusApp::DeleteObjekt()
{
	 if (HaveDoc)
	 	if (!doc->EditClip)
			view->DeleteItem();
}

void ScribusApp::ObjektDup()
{
	slotSelect();
	bool savedAlignGrid = doc->useRaster;
	bool savedAlignGuides = doc->SnapGuides;
	doc->useRaster = false;
	doc->SnapGuides = false;
	slotEditCopy();
	view->Deselect(true);
	slotEditPaste();
	for (uint b=0; b<view->SelItem.count(); ++b)
	{
		view->SelItem.at(b)->setLocked(false);
		view->SelItem.at(b)->Xpos;
		view->SelItem.at(b)->Ypos;
		view->MoveItem(DispX, DispY, view->SelItem.at(b));
	}
	doc->useRaster = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
}

void ScribusApp::ObjektDupM()
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
				for (uint b=0; b<view->SelItem.count(); ++b)
				{
					view->SelItem.at(b)->setLocked(false);
					view->SelItem.at(b)->Xpos;
					view->SelItem.at(b)->Ypos;
					view->MoveItem(dH2, dV2, view->SelItem.at(b), true);
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

void ScribusApp::selectItemsFromOutlines(int Page, int Item, bool single)
{
	NoFrameEdit();
	setActiveWindow();
	view->Deselect(true);
	if ((Page != -1) && (Page != static_cast<int>(doc->currentPage->PageNr)))
		view->GotoPage(Page);
	view->SelectItemNr(Item, true, single);
	if (view->SelItem.count() != 0)
	{
		PageItem *currItem = view->SelItem.at(0);
	 // jjsa 23-05-2004 added for centering of rotated objects
		if ( currItem->Rot != 0.0 )
		{
			double y1 = sin(currItem->Rot/180.*M_PI) * currItem->Width;
			double x1 = cos(currItem->Rot/180.*M_PI) * currItem->Width;
			double y2 = sin((currItem->Rot+90.)/180*M_PI) * currItem->Height;
			double x2 = cos((currItem->Rot+90.)/180*M_PI) * currItem->Height;
			double mx = currItem->Xpos + ((x1 + x2)/2.0);
			double my = currItem->Ypos + ((y1 + y2)/2.0);
			double viewScale=view->getScale();
			if ((qRound((currItem->Xpos + QMAX(x1, x2)) * viewScale) > view->contentsWidth()) ||
				(qRound((currItem->Ypos + QMAX(y1, y2)) * viewScale) > view->contentsHeight()))
				view->resizeContents(QMAX(qRound((currItem->Xpos + QMAX(x1, x2)) * viewScale), view->contentsWidth()),
														  QMAX(qRound((currItem->Ypos + QMAX(y1, y2)) * viewScale), view->contentsHeight()));
			view->SetCCPo(static_cast<int>(mx), static_cast<int>(my));
		}
		else
		{
			double viewScale=view->getScale();
			if ((qRound((currItem->Xpos + currItem->Width) * viewScale) > view->contentsWidth()) ||
				(qRound((currItem->Ypos + currItem->Height) * viewScale) > view->contentsHeight())
				)
				view->resizeContents(QMAX(qRound((currItem->Xpos + currItem->Width) * viewScale), view->contentsWidth()),
									 QMAX(qRound((currItem->Ypos + currItem->Height) * viewScale), view->contentsHeight()));
			view->SetCCPo(static_cast<int>(currItem->Xpos + currItem->Width/2), static_cast<int>(currItem->Ypos + currItem->Height/2));
		}
	}
}

void ScribusApp::selectPagesFromOutlines(int Page)
{
	NoFrameEdit();
	setActiveWindow();
	view->Deselect(true);
	if (Page < 0)
		return;
	view->GotoPage(Page);
}

void ScribusApp::buildFontMenu()
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
		a = FontMenu->insertItem(new FmItem(it.currentKey(), it.current()->Font));
		FontMenu->setItemChecked(a, true);
		FontID.insert(a, it.current()->SCName);
	}
	else
	{
		QMap<QString,QFont>::Iterator it3;
		for (it3 = doc->UsedFonts.begin(); it3 != doc->UsedFonts.end(); ++it3)
		{
			a = FontMenu->insertItem(new FmItem(it3.key(), it3.data()));
			if (it3.key() == doc->toolSettings.defFont)
				FontMenu->setItemChecked(a, true);
			FontID.insert(a, it3.key());
		}
	}
	connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
}

void ScribusApp::slotPrefsOrg()
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
		/*
		double newDisplayScale=prefsManager->displayScale();
		if (oldDisplayScale != newDisplayScale)
			zChange = true;
		*/
		propertiesPalette->Cpal->UseTrans(true);
		FontSub->RebuildList(0);
		propertiesPalette->Fonts->RebuildList(0);

		GetCMSProfiles();
		SetShortCut();
		emit prefsChanged();
		QWidgetList windows = wsp->windowList();
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
		{
			QWidget* w = wsp->windowList().at( i );
			ScribusWin* swin = (ScribusWin*)w;
			swin->doc->guidesSettings.before = prefsManager->appPrefs.guidesSettings.before;
			swin->doc->marginColored = prefsManager->appPrefs.marginColored;
		}
	}
	delete dia;
}

void ScribusApp::ShowSubs()
{
	QString mess;
	if (HaveGS != 0)
	{
		mess = tr("The following programs are missing:")+"\n\n";
		if (HaveGS != 0)
			mess += tr("Ghostscript : You cannot use EPS images or Print Preview")+"\n\n";
		QMessageBox::warning(this, tr("Warning"), mess, 1, 0, 0);
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

	setTools(prefsManager->appPrefs.mainToolBarSettings.visible);
	setPDFTools(prefsManager->appPrefs.pdfToolBarSettings.visible);
	setActiveWindow();
	raise();
}

void ScribusApp::doPrintPreview()
{
	if (!docCheckerPalette->noButton)
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = 0;
		docCheckerPalette->noButton = true;
		docCheckerPalette->ignoreErrors->hide();
		scrActions["toolsPreflightVerifier"]->setOn(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doPrintPreview()));
	}
	if (HaveDoc)
	{
		PPreview *dia = new PPreview(this, view, doc, HavePngAlpha, HaveTiffSep);
		connect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
		dia->exec();
		PrefsManager *prefsManager=PrefsManager::instance();
		prefsManager->appPrefs.PrPr_Mode = dia->EnableCMYK->isChecked();
		prefsManager->appPrefs.PrPr_AlphaText = dia->AliasText->isChecked();
		prefsManager->appPrefs.PrPr_AlphaGraphics = dia->AliasGr->isChecked();
		prefsManager->appPrefs.PrPr_Transparency = dia->AliasTr->isChecked();
		prefsManager->appPrefs.PrPr_C = dia->EnableCMYK_C->isChecked();
		prefsManager->appPrefs.PrPr_M = dia->EnableCMYK_M->isChecked();
		prefsManager->appPrefs.PrPr_Y = dia->EnableCMYK_Y->isChecked();
		prefsManager->appPrefs.PrPr_K = dia->EnableCMYK_K->isChecked();
		prefsManager->appPrefs.Gcr_Mode = dia->EnableGCR->isChecked();
		disconnect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
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

void ScribusApp::printPreview()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		scanDocument();
		if ((doc->docItemErrors.count() != 0) || (doc->masterItemErrors.count() != 0))
		{
			if (doc->checkerProfiles[doc->curCheckProfile].ignoreErrors)
			{
				int t = QMessageBox::warning(this, tr("Warning"),
											tr("Scribus detected some errors.\nConsider using the Preflight Verifier  to correct them."),
											tr("&Abort"), tr("&Ignore"), 0, 0, 0);
				if (t == 0)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doPrintPreview()));
				docCheckerPalette->noButton = false;
				docCheckerPalette->checkMode = 2;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setOn(true);
				return;
			}
		}
	}
	doPrintPreview();
}

bool ScribusApp::DoSaveAsEps(QString fn)
{
	QStringList spots;
	bool return_value = true;
	std::vector<int> pageNs;
	pageNs.push_back(doc->currentPage->PageNr+1);
	ReOrderText(doc, view);
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QMap<QString,QFont> ReallyUsed;
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
			dd->CreatePS(doc, view, pageNs, false, tr("All"), spots, true, false, false, true, prefsManager->appPrefs.GCRMode, false);
		else
			return_value = false;
		delete dd;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	fileWatcher->start();
	return return_value;
}

void ScribusApp::SaveAsEps()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		scanDocument();
		if ((doc->docItemErrors.count() != 0) || (doc->masterItemErrors.count() != 0))
		{
			if (doc->checkerProfiles[doc->curCheckProfile].ignoreErrors)
			{
				int t = QMessageBox::warning(this, tr("Warning"),
											tr("Scribus detected some errors.\nConsider using the Preflight Verifier  to correct them."),
											tr("&Abort"), tr("&Ignore"), 0, 0, 0);
				if (t == 0)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(reallySaveAsEps()));
				docCheckerPalette->noButton = false;
				docCheckerPalette->checkMode = 2;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setOn(true);
				return;
			}
		}
	}
	reallySaveAsEps();
}

void ScribusApp::reallySaveAsEps()
{
	QString fna;
	if (!docCheckerPalette->noButton)
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = 0;
		docCheckerPalette->noButton = true;
		docCheckerPalette->ignoreErrors->hide();
		scrActions["toolsPreflightVerifier"]->setOn(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(reallySaveAsEps()));
	}
	if (!doc->DocName.startsWith(tr("Document")))
	{
		QFileInfo fi(doc->DocName);
		fna = fi.dirPath() + "/" + getFileNameByPage(doc->currentPage->PageNr, "eps");
	}
	else
	{
		QDir di = QDir();
		fna = di.currentDirPath() + "/" + getFileNameByPage(doc->currentPage->PageNr, "eps");
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
				QMessageBox::warning(this, tr("Warning"), tr("Cannot write the file: \n%1").arg(fn), CommonStrings::tr_OK);
		}
	}
}

bool ScribusApp::getPDFDriver(QString fn, QString nam, int Components, std::vector<int> &pageNs, QMap<int,QPixmap> thumbs)
{
	bool ret = false;
	PDFlib *dia = new PDFlib(doc);
	fileWatcher->forceScan();
	fileWatcher->stop();
	ret = dia->doExport(fn, nam, Components, pageNs, thumbs, mainWindowProgressBar);
	fileWatcher->start();
	delete dia;
	return ret;
}

void ScribusApp::SaveAsPDF()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		scanDocument();
		if ((doc->docItemErrors.count() != 0) || (doc->masterItemErrors.count() != 0))
		{
			if (doc->checkerProfiles[doc->curCheckProfile].ignoreErrors)
			{
				int t = QMessageBox::warning(this, tr("Warning"),
											tr("Detected some errors.\nConsider using the Preflight Verifier to correct them"),
											tr("&Abort"), tr("&Ignore"), 0, 0, 0);
				if (t == 0)
					return;
			}
			else
			{
				connect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doSaveAsPDF()));
				docCheckerPalette->noButton = false;
				docCheckerPalette->checkMode = 1;
				docCheckerPalette->buildErrorList(doc);
				docCheckerPalette->show();
				scrActions["toolsPreflightVerifier"]->setOn(true);
				return;
			}
		}
	}
	doSaveAsPDF();
}

void ScribusApp::doSaveAsPDF()
{
	if (!docCheckerPalette->noButton)
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = 0;
		docCheckerPalette->noButton = true;
		docCheckerPalette->ignoreErrors->hide();
		scrActions["toolsPreflightVerifier"]->setOn(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doSaveAsPDF()));
	}
/*	if (bookmarkPalette->BView->childCount() == 0)
		doc->PDF_Options.Bookmarks = false; */
	QMap<QString,QFont> ReallyUsed;
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
	PDF_Opts *dia = new PDF_Opts(this, doc->DocName, ReallyUsed, view, &doc->PDF_Options, doc->PDF_Options.PresentVals, &PDFXProfiles, prefsManager->appPrefs.AvailFonts, doc->unitRatio(), &ScApp->PrinterProfiles);
	if (dia->exec())
	{
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		dia->updateDocOptions();
		ReOrderText(doc, view);
		QString pageString(dia->getPagesString());
		std::vector<int> pageNs;
		uint pageNumbersSize;
		QMap<int,QPixmap> thumbs;
		int components=dia->colorSpaceComponents();
		QString nam(dia->cmsDescriptor());
		QString fileName = doc->PDF_Options.Datei;
		parsePagesString(pageString, &pageNs, doc->pageCount);
		if (doc->PDF_Options.doMultiFile)
		{
			QFileInfo fi(fileName);
			QString ext = fi.extension( false );
			QString path = fi.dirPath( true );
			QString name = fi.baseName( true );
			uint aa = 0;
			while (aa < pageNs.size())
			{
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
					QMessageBox::warning(this, tr("Warning"), tr("Cannot write the file: \n%1").arg(doc->PDF_Options.Datei), CommonStrings::tr_OK);
					delete dia;
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
				QMessageBox::warning(this, tr("Warning"), tr("Cannot write the file: \n%1").arg(doc->PDF_Options.Datei), CommonStrings::tr_OK);
			}
		}
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	delete dia;
}

void ScribusApp::AddBookMark(PageItem *ite)
{
	bookmarkPalette->BView->AddPageItem(ite);
	StoreBookmarks();
}

void ScribusApp::DelBookMark(PageItem *ite)
{
	bookmarkPalette->BView->DeleteItem(ite->BMnr);
	StoreBookmarks();
}

void ScribusApp::BookMarkTxT(PageItem *ite)
{
	bookmarkPalette->BView->ChangeText(ite);
	StoreBookmarks();
}

void ScribusApp::ChBookmarks(int /*s*/, int /*e*/, int /*n*/)
{
//	view->Pages.at(s)->Items.at(e)->BMnr = n;
}

void ScribusApp::RestoreBookMarks()
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

void ScribusApp::StoreBookmarks()
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

void ScribusApp::slotElemRead(QString Name, int x, int y, bool art, bool loca, ScribusDoc* docc, ScribusView* vie)
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
			propertiesPalette->Cpal->SetColors(docc->PageColors);
			propertiesPalette->updateCList();
			propertiesPalette->Spal->updateFormatList();
			propertiesPalette->SetLineFormats(docc);
			outlinePalette->BuildTree(doc);
			outlinePalette->reopenTree(doc->OpenNodes);
			slotDocCh();
		}
	}
	delete ss;
}

void ScribusApp::slotChangeUnit(int unitIndex, bool draw)
{
	doc->setUnitIndex(unitIndex);
	view->unitSwitcher->setCurrentText(unitGetStrFromIndex(doc->unitIndex()));
	propertiesPalette->UnitChange();
	alignDistributePalette->unitChange();
	if (draw)
		view->DrawNew();
}

void ScribusApp::ManageJava()
{
	JavaDocs *dia = new JavaDocs(this, doc, view);
	connect(dia, SIGNAL(docChanged(bool)), this, SLOT(slotDocCh(bool )));
	dia->exec();
	disconnect(dia, SIGNAL(docChanged(bool)), this, SLOT(slotDocCh(bool )));
	delete dia;
}

void ScribusApp::manageMasterPages(QString temp)
{
	if (HaveDoc)
	{
		view->Deselect(true);
		if (doc->masterPageMode)
		{
			ActWin->muster->updateMasterPageList(temp);
			ActWin->muster->selectMasterPage(temp);
		}
		else
		{
			MasterPagesPalette *dia = new MasterPagesPalette(this, doc, view, temp);
			connect(dia, SIGNAL(createNew(int)), this, SLOT(slotNewMasterPage(int)));
			connect(dia, SIGNAL(removePage(int )), this, SLOT(DeletePage2(int )));
			connect(dia, SIGNAL(loadPage(QString, int, bool)), this, SLOT(loadPage(QString, int, bool)));
			connect(dia, SIGNAL(finished()), this, SLOT(manageMasterPagesEnd()));
			connect(dia, SIGNAL(docAltered(ScribusDoc* )), outlinePalette, SLOT(BuildTree(ScribusDoc* )));
			connect(dia, SIGNAL(docAltered(ScribusDoc*)), SLOT(slotDocCh()));
			scrActions["pageInsert"]->setEnabled(false);
			scrActions["pageDelete"]->setEnabled(false);
			scrActions["pageCopy"]->setEnabled(false);
			scrActions["pageMove"]->setEnabled(false);
			scrActions["pageApplyMasterPage"]->setEnabled(false);
			scrActions["editMasterPages"]->setEnabled(false);
			ActWin->MenuStat[0] = scrActions["fileSave"]->isEnabled();
			ActWin->MenuStat[1] = scrActions["fileRevert"]->isEnabled();
			ActWin->MenuStat[2] = scrActions["fileSave"]->isEnabled();
			ActWin->MenuStat[3] = scrActions["fileSaveAs"]->isEnabled();
			scrActions["fileNew"]->setEnabled(false);
			scrActions["fileOpen"]->setEnabled(false);
			scrActions["fileClose"]->setEnabled(false);
			scrMenuMgr->setMenuEnabled("FileOpenRecent", false);
			scrActions["fileRevert"]->setEnabled(false);
			scrActions["fileDocSetup"]->setEnabled(false);
			scrActions["filePrint"]->setEnabled(false);
			scrActions["PrintPreview"]->setEnabled(false);
			pagePalette->DisablePal();
			dia->show();
			ActWin->muster = dia;
			doc->OpenNodes = outlinePalette->buildReopenVals();
		}
	}
}

void ScribusApp::manageMasterPagesEnd()
{
	view->hideMasterPage();
	scrActions["editMasterPages"]->setEnabled(true);
	scrActions["fileNew"]->setEnabled(true);
	scrActions["fileOpen"]->setEnabled(true);
	scrActions["fileClose"]->setEnabled(true);
	scrActions["fileSave"]->setEnabled(doc->isModified());
	scrMenuMgr->setMenuEnabled("FileOpenRecent", true);
	scrActions["fileRevert"]->setEnabled(true);
	scrActions["fileDocSetup"]->setEnabled(true);
	scrActions["filePrint"]->setEnabled(true);
	if (HaveGS==0)
		scrActions["PrintPreview"]->setEnabled(true);
	scrActions["pageInsert"]->setEnabled(true);
	scrActions["pageCopy"]->setEnabled(true);
	scrActions["pageApplyMasterPage"]->setEnabled(true);
	bool setter = doc->Pages.count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);
	uint pageCount=doc->Pages.count();
	for (uint c=0; c<pageCount; ++c)
		Apply_MasterPage(doc->Pages.at(c)->MPageNam, c, false);
	doc->masterPageMode = false;
	pagePalette->EnablePal();
	pagePalette->RebuildTemp();
	ActWin->muster = NULL;
	view->DrawNew();
	pagePalette->Rebuild();
	outlinePalette->BuildTree(doc);
//	outlinePalette->reopenTree(doc->OpenNodes);
//	slotDocCh();
}
/*!
 * @brief Apply master pages from the Apply Master Page dialog
 * @todo Make this work with real page numbers, negative numbers and document sections when they are implemented
 */
void ScribusApp::ApplyMasterPage()
{

	ApplyMasterPageDialog *dia = new ApplyMasterPageDialog(this);
	dia->setup(doc, doc->currentPage->MPageNam);
	if (dia->exec())
	{
		QString masterPageName = dia->getMasterPageName();
		int pageSelection = dia->getPageSelection(); //0=current, 1=even, 2=odd, 3=all
		if (pageSelection==0) //current page only
			Apply_MasterPage(masterPageName, doc->currentPage->PageNr, false);
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
				endPage=doc->pageCount;
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
	view->DrawNew();
	slotDocCh();
	pagePalette->Rebuild();
	delete dia;
}

void ScribusApp::Apply_MasterPage(QString in, int Snr, bool reb)
{
	doc->applyMasterPage(in, Snr);
	if (reb)
	{
		view->DrawNew();
		slotDocCh();
		pagePalette->Rebuild();
	}
}

void ScribusApp::GroupObj(bool showLockDia)
{
	if (HaveDoc)
	{
		PageItem *currItem;
		PageItem* bb;
		double x, y, w, h;
		int t = -1; // show locked dialog only once
		QString tooltip = Um::ItemsInvolved + "\n";
		uint selectedItemCount=view->SelItem.count();
		if (showLockDia)
		{
			for (uint a=0; a<selectedItemCount; ++a)
			{
				if (t == -1 && view->SelItem.at(a)->locked())
					t = QMessageBox::warning(this, tr("Warning"),
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
				currItem = view->SelItem.at(a);
				if (currItem->locked())
				{
					for (uint c=0; c<selectedItemCount; ++c)
					{
						bb = view->SelItem.at(c);
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
			currItem = view->SelItem.at(a);
			currItem->Groups.push(doc->GroupCounter);
			ss->set(QString("item%1").arg(a), currItem->ItemNr);
		}
		doc->GroupCounter++;
		view->getGroupRect(&x, &y, &w, &h);
		view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
		outlinePalette->BuildTree(doc);
		slotDocCh();
		scrActions["itemAttachTextToPath"]->setEnabled(false);
		scrActions["itemGroup"]->setEnabled(false);
		scrActions["itemUngroup"]->setEnabled(true);
		undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
	}
}

void ScribusApp::UnGroupObj()
{
	if (HaveDoc)
	{
		SimpleState *ss = new SimpleState(Um::Ungroup);
		ss->set("UNGROUP", "ungroup");
		ss->set("itemcount", view->SelItem.count());
		QString tooltip = Um::ItemsInvolved + "\n";
		PageItem *currItem;
		for (uint a=0; a<view->SelItem.count(); ++a)
		{
			currItem = view->SelItem.at(a);
			currItem->Groups.pop();
			currItem->isTableItem = false;
			currItem->LeftLink = 0;
			currItem->RightLink = 0;
			currItem->TopLink = 0;
			currItem->BottomLink = 0;
			ss->set(QString("item%1").arg(a), currItem->ItemNr);
			tooltip += "\t" + currItem->getUName() + "\n";
		}
		outlinePalette->BuildTree(doc);
		slotDocCh();
		view->Deselect(true);

		undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
	}
}

void ScribusApp::restore(UndoState* state, bool isUndo)
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

void ScribusApp::restoreDeletePage(SimpleState *state, bool isUndo)
{
	uint pagenr   = state->getUInt("PAGENR");
	QStringList tmpl = state->get("MASTERPAGE");
	int where, wo;
	if (pagenr == 1)
	{
		where = 0;
		wo = 1;
	}
	else if (pagenr > doc->Pages.count())
	{
		where = 2;
		wo = doc->Pages.count();
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
			undoManager->replaceObject(state->getUInt("DUMMY_ID"), doc->Pages.at(pagenr - 1));
		delete tmp;
	}
	else
	{
		DummyUndoObject *duo = new DummyUndoObject();
		uint id = static_cast<uint>(duo->getUId());
		undoManager->replaceObject(doc->Pages.at(pagenr - 1)->getUId(), duo);
		state->set("DUMMY_ID", id);
		DeletePage(pagenr, pagenr);
	}
}

void ScribusApp::restoreAddPage(SimpleState *state, bool isUndo)
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
			delTo = doc->Pages.count();
			delFrom = doc->Pages.count() - count + 1;
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
			undoManager->replaceObject(doc->Pages.at(i)->getUId(), duo);
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
					state->getUInt(QString("Page%1").arg(i)), doc->Pages.at(i));
			delete tmp;
		}
	}
}

void ScribusApp::restoreGroupping(SimpleState *state, bool isUndo)
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

void ScribusApp::restoreUngroupping(SimpleState *state, bool isUndo)
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

void ScribusApp::StatusPic()
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

QString ScribusApp::CFileDialog(QString wDir, QString caption, QString filter, QString defNa,
                                bool Pre, bool mod, bool comp, bool cod, bool onlyDirs,
                                bool *docom, bool *doFont)
{
	QString retval = "";
	CustomFDialog *dia = new CustomFDialog(this, wDir, caption, filter, Pre, mod, comp, cod, onlyDirs);
	if (!defNa.isEmpty())
		dia->setSelection(defNa);
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

void ScribusApp::GetCMSProfiles()
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

void ScribusApp::GetCMSProfilesDir(QString pfad)
{
#ifdef HAVE_CMS
	QDir d(pfad, "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		QString nam = "";
		const char *Descriptor;
		cmsHPROFILE hIn;

		for (uint dc = 0; dc < d.count(); ++dc)
		{
			QFileInfo fi(pfad + d[dc]);
			QString ext = fi.extension(false).lower();
			if ((ext == "icm") || (ext == "icc"))
			{
				hIn = cmsOpenProfileFromFile(pfad + d[dc], "r");
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

void ScribusApp::initCMS()
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

void ScribusApp::recalcColors(QProgressBar *dia)
{
	if (HaveDoc)
	{
		doc->recalculateColors();
		updateColorMenu(dia);
		propertiesPalette->Cpal->SetColors(doc->PageColors);
		propertiesPalette->updateCList();
	}
}

void ScribusApp::ModifyAnnot()
{
	if (view->SelItem.count() != 0)
	{
		PageItem *currItem = view->SelItem.at(0);
		if ((currItem->AnType == 0) || (currItem->AnType == 1) || (currItem->AnType > 9))
		{
			int AnType = currItem->AnType;
			int AnActType = currItem->AnActType;
			QString AnAction = currItem->AnAction;
			QString An_Extern = currItem->An_Extern;
			Annota *dia = new Annota(this, currItem, doc->pageCount, static_cast<int>(doc->pageWidth), static_cast<int>(doc->pageHeight), view);
			if (dia->exec())
				slotDocCh();
			else
			{
				currItem->AnType = AnType;
				currItem->AnActType = AnActType;
				currItem->AnAction = AnAction;
				currItem->An_Extern = An_Extern;
			}
			delete dia;
		}
		else
		{
			Annot *dia = new Annot(this, currItem, doc->pageCount, static_cast<int>(doc->pageWidth), static_cast<int>(doc->pageHeight), doc->PageColors, view);
			if (dia->exec())
				slotDocCh();
			delete dia;
		}
	}
}

void ScribusApp::SetShortCut()
{
	for (QMap<QString,Keys>::Iterator it = prefsManager->appPrefs.KeyActions.begin(); it != prefsManager->appPrefs.KeyActions.end(); ++it )
	{
		if (!it.data().actionName.isEmpty())
			if (scrActions[it.data().actionName])
				scrActions[it.data().actionName]->setAccel(it.data().keySequence);
	}
}

void ScribusApp::PutScrap()
{
	ScriXmlDoc *ss = new ScriXmlDoc();
	QString objectString = ss->WriteElem(&(view->SelItem), doc, view);
	scrapbookPalette->ObjFromMenu(objectString);
	delete ss;
}

void ScribusApp::changeLayer(int )
{
	view->Deselect(true);
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	view->DrawNew();
}

void ScribusApp::showLayer()
{
	view->DrawNew();
}

//TODO replace with the ScribusDoc::deleteTaggedItems
void ScribusApp::LayerRemove(int l, bool dl)
{
	if (doc->masterPageMode)
		doc->MasterPages = doc->Pages;
	else
		doc->DocPages = doc->Pages;
	view->Deselect();
	for (uint b = 0; b < doc->MasterItems.count(); ++b)
	{
		if (doc->MasterItems.at(b)->LayerNr == l)
		{
			if (dl)
			{
				view->SelItem.append(doc->MasterItems.at(b));
				doc->DocItems.at(b)->setLocked(false);
			}
			else
				doc->MasterItems.at(b)->setLayer(0);
		}
	}
	if (view->SelItem.count() != 0)
		view->DeleteItem();
	view->SelItem.clear();
	for (uint b = 0; b < doc->DocItems.count(); ++b)
	{
		if (doc->DocItems.at(b)->LayerNr == l)
		{
			if (dl)
			{
				view->SelItem.append(doc->DocItems.at(b));
				doc->DocItems.at(b)->setLocked(false);
			}
			else
				doc->DocItems.at(b)->setLayer(0);
		}
	}
	if (view->SelItem.count() != 0)
		view->DeleteItem();
	rebuildLayersList();
	view->updateLayerMenu();
}

void ScribusApp::UnDoAction()
{
	undoManager->undo(1);
}

void ScribusApp::RedoAction()
{
	undoManager->redo(1);
}

void ScribusApp::initHyphenator()
{
	QString pfad = ScPaths::instance().libDir();
	QStringList L_Afrikaans;
	QStringList L_Bulgarian;
	QStringList L_Catalan;
	QStringList L_Croatian;
	QStringList L_Czech;
	QStringList L_Danish;
	QStringList L_Dutch;
	QStringList L_English;
	QStringList L_Finnish;
	QStringList L_French;
	QStringList L_German;
	QStringList L_Greek;
	QStringList L_Hungarian;
	QStringList L_Irish;
	QStringList L_Italian;
	QStringList L_Lithuanian;
	QStringList L_Polish;
	QStringList L_Portuguese;
	QStringList L_Portuguese_BR;
	QStringList L_Russian;
	QStringList L_Slovak;
	QStringList L_Slovenian;
	QStringList L_Spanish;
	QStringList L_Swedish;
	QStringList L_Ukrainian;
	QDir d2(pfad, "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d2.exists()) && (d2.count() != 0))
	{
		for (uint dc = 0; dc < d2.count(); dc++)
		{
			QFileInfo fi(pfad + d2[dc]);
			QString ext = fi.extension(false).lower();
			QString ext2 = fi.extension(true).lower();
			if (ext == "qm")
			{
    			QTranslator *trans = new QTranslator(0);
				trans->load(pfad + d2[dc]);
				QString translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Croatian", "").translation();
				if (!translatedLang.isEmpty())
					L_Croatian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "German", "").translation();
				if (!translatedLang.isEmpty())
					L_German.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Polish", "").translation();
				if (!translatedLang.isEmpty())
					L_Polish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "English", "").translation();
				if (!translatedLang.isEmpty())
					L_English.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Spanish", "").translation();
				if (!translatedLang.isEmpty())
					L_Spanish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Italian", "").translation();
				if (!translatedLang.isEmpty())
					L_Italian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "French", "").translation();
				if (!translatedLang.isEmpty())
					L_French.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Russian", "").translation();
				if (!translatedLang.isEmpty())
					L_Russian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Danish", "").translation();
				if (!translatedLang.isEmpty())
					L_Danish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Slovak", "").translation();
				if (!translatedLang.isEmpty())
					L_Slovak.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Hungarian", "").translation();
				if (!translatedLang.isEmpty())
					L_Hungarian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Czech", "").translation();
				if (!translatedLang.isEmpty())
					L_Czech.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Dutch", "").translation();
				if (!translatedLang.isEmpty())
					L_Dutch.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Portuguese", "").translation();
				if (!translatedLang.isEmpty())
					L_Portuguese.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Portuguese (BR)", "").translation();
				if (!translatedLang.isEmpty())
					L_Portuguese_BR.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Ukrainian", "").translation();
				if (!translatedLang.isEmpty())
					L_Ukrainian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Greek", "").translation();
				if (!translatedLang.isEmpty())
					L_Greek.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Catalan", "").translation();
				if (!translatedLang.isEmpty())
					L_Catalan.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Finnish", "").translation();
				if (!translatedLang.isEmpty())
					L_Finnish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Irish", "").translation();
				if (!translatedLang.isEmpty())
					L_Irish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Lithuanian", "").translation();
				if (!translatedLang.isEmpty())
					L_Lithuanian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Swedish", "").translation();
				if (!translatedLang.isEmpty())
					L_Swedish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Slovenian", "").translation();
				if (!translatedLang.isEmpty())
					L_Slovenian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Afrikaans", "").translation();
				if (!translatedLang.isEmpty())
					L_Afrikaans.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Bulgarian", "").translation();
				if (!translatedLang.isEmpty())
					L_Bulgarian.append(translatedLang);
				delete trans;
			}
		}
	}
	InstLang.insert("Afrikaans", L_Afrikaans);
	InstLang.insert("Bulgarian", L_Bulgarian);
	InstLang.insert("Catalan", L_Catalan);
	InstLang.insert("Croatian", L_Croatian);
	InstLang.insert("Czech", L_Czech);
	InstLang.insert("Danish", L_Danish);
	InstLang.insert("Dutch", L_Dutch);
	InstLang.insert("English", L_English);
	InstLang.insert("Finnish", L_Finnish);
	InstLang.insert("French", L_French);
	InstLang.insert("German", L_German);
	InstLang.insert("Greek", L_Greek);
	InstLang.insert("Hungarian", L_Hungarian);
	InstLang.insert("Irish", L_Irish);
	InstLang.insert("Italian", L_Italian);
	InstLang.insert("Lithuanian", L_Lithuanian);
	InstLang.insert("Polish", L_Polish);
	InstLang.insert("Portuguese (BR)", L_Portuguese_BR);
	InstLang.insert("Portuguese", L_Portuguese);
	InstLang.insert("Russian", L_Russian);
	InstLang.insert("Slovak", L_Slovak);
	InstLang.insert("Slovenian", L_Slovenian);
	InstLang.insert("Spanish", L_Spanish);
	InstLang.insert("Swedish", L_Swedish);
	InstLang.insert("Ukrainian", L_Ukrainian);
	QString lang = QString(QTextCodec::locale()).left(2);
	LangTransl.clear();
	prefsManager->appPrefs.Language = "English";
	pfad = QDir::convertSeparators(ScPaths::instance().libDir() + "dicts/");
	QDir d(pfad, "*.dic", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		LanguageManager langmgr;
		langmgr.init(false);
		QString datein = "";
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			QFileInfo fi(d[dc]);
			QString fileLangAbbrev=fi.baseName().section('_', 1);
			datein = langmgr.getLangFromAbbrev(fileLangAbbrev);
			QString tDatein = datein;
			datein = GetLang(datein);
			LangTransl.insert(datein, tDatein);
			Sprachen.insert(datein, d[dc]);
			if (fileLangAbbrev == lang)
				prefsManager->appPrefs.Language = datein;
		}
		if (datein.isEmpty())
			prefsManager->appPrefs.Language = "English";
	}
	propertiesPalette->fillLangCombo(LangTransl);
}

QString ScribusApp::GetLang(QString inLang)
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

void ScribusApp::doHyphenate()
{
	if (HaveDoc)
	{
		uint selectedItemCount=view->SelItem.count();
		if (selectedItemCount != 0)
		{
			for (uint i = 0; i < selectedItemCount; ++i)
			{
				PageItem *currItem = view->SelItem.at(i);
				if (doc->docHyphenator->Language != currItem->Language)
					doc->docHyphenator->slotNewDict(currItem->Language);
				doc->docHyphenator->slotHyphenate(currItem);
			}
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusApp::doDeHyphenate()
{
	if (HaveDoc)
	{
		uint selectedItemCount=view->SelItem.count();
		if (selectedItemCount != 0)
		{
			for (uint i = 0; i < selectedItemCount; ++i)
			{
				PageItem *currItem = view->SelItem.at(i);
				doc->docHyphenator->slotDeHyphenate(currItem);
			}
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusApp::ManageGuides()
{
	if (HaveDoc)
	{
		GuideManager *dia = new GuideManager(this);
		dia->exec();
		delete dia;
	}
}

void ScribusApp::setItemFillTransparency(double t)
{
	if (HaveDoc)
	{
		uint selectedItemCount=view->SelItem.count();
		if (selectedItemCount != 0)
		{
			for (uint i = 0; i < selectedItemCount; ++i)
			{
				PageItem *currItem = view->SelItem.at(i);
				currItem->setFillTransparency(t);
			}
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusApp::setItemLineTransparency(double t)
{
	if (HaveDoc)
	{
		uint selectedItemCount=view->SelItem.count();
		if (selectedItemCount != 0)
		{
			for (uint i = 0; i < selectedItemCount; ++i)
			{
				PageItem *currItem = view->SelItem.at(i);
				currItem->setLineTransparency(t);
			}
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusApp::ImageEffects()
{
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			PageItem *currItem = view->SelItem.at(0);
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

QString ScribusApp::Collect(bool compress, bool withFonts, const QString& newDirectory)
{
	QString retVal("");
	QString CurDirP(QDir::currentDirPath());
	bool compressR = compress;
	bool withFontsR = withFonts;
	QString s(newDirectory);
	if (s.isNull() && ScQApp->usingGUI())
	{
		QString wdir(".");
		QString prefsDocDir=prefsManager->documentDir();
		if (!prefsDocDir.isEmpty())
			wdir = dirs->get("collect", prefsDocDir);
		else
			wdir = dirs->get("collect", ".");
		s = CFileDialog(wdir, tr("Choose a Directory"), "", "", false, false, false, false, true, &compressR, &withFontsR);
	}

	if (!s.isEmpty())
	{
		fileWatcher->forceScan();
		fileWatcher->stop();
		if(s.right(1) != "/")
			s += "/";
		dirs->set("collect", s.left(s.findRev("/",-2)));
		QFileInfo fi(s);
		QString fn(s);
		if (fi.exists())
		{
			if (fi.isDir() && fi.isWritable())
			{
				if (doc->hasName)
				{
					QFileInfo fis(doc->DocName);
					fn += fis.fileName();
				}
				else
					fn += doc->DocName+".sla";
				doc->hasName = true;
				if (compressR)
				{
					if (!fn.endsWith(".gz"))
						fn += ".gz";
				}
				else
				{
					if (fn.endsWith(".gz"))
						fn = fn.remove(".gz");
				}
				if (!overwrite(this, fn))
				{
					retVal = "";
					return retVal;
				}
				retVal = s;
				mainWindowStatusLabel->setText( tr("Collecting..."));
				if(!doc->collectForOutput(fn, withFontsR))
				{
					QMessageBox::warning(this, tr("Warning"), tr("Cannot collect all files for output for file:\n%1").arg(fn), CommonStrings::tr_OK);
					retVal = "";
				}
			}
		}
		QDir::setCurrent(CurDirP);
		if (!retVal.isEmpty())
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
	}
	return retVal;
}

void ScribusApp::ReorgFonts()
{
	doc->reorganiseFonts();
	buildFontMenu();
}

void ScribusApp::docCheckToggle(bool visible)
{
	if (docCheckerPalette->checkMode == 1)
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doSaveAsPDF()));
	if (docCheckerPalette->checkMode == 2)
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(reallySaveAsEps()));
	if (docCheckerPalette->checkMode == 3)
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(slotReallyPrint()));
	if (!visible)
	{
		docCheckerPalette->ignoreErrors->hide();
		docCheckerPalette->checkMode = 0;
		docCheckerPalette->noButton = true;
	}
}

void ScribusApp::scanDocument()
{
	DocumentChecker docChecker;
	docChecker.checkDocument(doc);
}

void ScribusApp::HaveRaster(bool art)
{
	if (art && view->SelItem.count() != 0)
	{
		PageItem *currItem = view->SelItem.at(0);
		if (currItem->itemType() == PageItem::ImageFrame)
		{
			scrMenuMgr->clearMenu("Style");
			scrMenuMgr->addMenuToMenu("Color","Style");
			scrMenuMgr->addMenuItem(scrActions["styleInvertPict"], "Style");
		}
	}
}

void ScribusApp::slotStoryEditor()
{
	if (view->SelItem.count() != 0)
	{
		PageItem *currItem = view->SelItem.at(0);
		PageItem *currItemSE=storyEditor->currentItem();
		ScribusDoc *currDocSE=storyEditor->currentDocument();
		if (currItem==currItemSE && doc==currDocSE)
		{
			storyEditor->activFromApp = true;
			storyEditor->setCurrentDocumentAndItem(doc, currItem);
			storyEditor->activFromApp = true;
			storyEditor->show();
			storyEditor->activFromApp = true;
			storyEditor->raise();
			storyEditor->activFromApp = true;
			return;
		}
		storyEditor->activFromApp = true;
		storyEditor->setCurrentDocumentAndItem(doc, currItem);
		CurrStED = storyEditor;
		connect(storyEditor, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
		connect(storyEditor, SIGNAL(EditSt()), this, SLOT(slotEditStyles()));
		storyEditor->activFromApp = true;
		storyEditor->show();
		storyEditor->activFromApp = true;
		storyEditor->raise();
		storyEditor->activFromApp = true;
	}
}

void ScribusApp::emergencySave()
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
			doc = ActWin->doc;
			view = ActWin->view;
			doc->setModified(false);
			if (doc->hasName)
			{
				std::cout << "Saving: " << doc->DocName+".emergency" << std::endl;
				doc->autoSaveTimer->stop();
				disconnect(ActWin, SIGNAL(Schliessen()), ScApp, SLOT(DoFileClose()));
				ScriXmlDoc *ss = new ScriXmlDoc();
				ss->WriteDoc(doc->DocName+".emergency", doc, 0);
				delete ss;
			}
			view->close();
			uint numPages=doc->Pages.count();
			for (uint a=0; a<numPages; ++a)
				delete doc->Pages.at(a);
			delete doc;
			ActWin->close();
		}
	}
}

void ScribusApp::EditTabs()
{
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			PageItem *currItem = view->SelItem.at(0);
			TabManager *dia = new TabManager(this, doc->unitIndex(), currItem->TabValues, currItem->Width);
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

void ScribusApp::SearchText()
{
	PageItem *currItem = view->SelItem.at(0);
	setAppMode(modeEdit);
	currItem->CPos = 0;
	SearchReplace* dia = new SearchReplace(this, doc, currItem);
	connect(dia, SIGNAL(NewFont(QString)), this, SLOT(SetNewFont(QString)));
	connect(dia, SIGNAL(NewAbs(int)), this, SLOT(setAbsValue(int)));
	dia->exec();
	disconnect(dia, SIGNAL(NewFont(QString)), this, SLOT(SetNewFont(QString)));
	disconnect(dia, SIGNAL(NewAbs(int)), this, SLOT(setAbsValue(int)));
	delete dia;
	slotSelect();
}

void ScribusApp::imageEditorExited()
{
	int ex = 0;
	if ( ExternalApp != 0 )
	{
		ex = ExternalApp->exitStatus();
		delete ExternalApp;
		ExternalApp = 0;
	}
}

/* call gimp and wait uppon completion */
void ScribusApp::callImageEditor()
{
	if (view->SelItem.count() != 0)
	{
		QString imageEditorExecutable=prefsManager->imageEditorExecutable();
		if (ExternalApp != 0)
		{
			QMessageBox::information(this, tr("Information"), "<qt>" + tr("The program %1 is already running!").arg(imageEditorExecutable) + "</qt>", 1, 0, 0);
			return;
		}
		PageItem *currItem = view->SelItem.at(0);
		if (currItem->PicAvail)
		{
			ExternalApp = new QProcess(NULL);
			QStringList cmd = QStringList::split(" ", imageEditorExecutable);
			cmd.append(currItem->Pfile);
			ExternalApp->setArguments(cmd);
			if ( !ExternalApp->start() )
			{
				delete ExternalApp;
				ExternalApp = 0;
				QMessageBox::critical(this, tr("Warning"), "<qt>" + tr("The program %1 is missing!").arg(imageEditorExecutable) + "</qt>", 1, 0, 0);
				return;
			}
			connect(ExternalApp, SIGNAL(processExited()), this, SLOT(imageEditorExited()));
		}
	}
}

void ScribusApp::slotCharSelect()
{
	if ((HaveDoc) && (view->SelItem.count() != 0))
	{
		PageItem *currItem = view->SelItem.at(0);
		if ((currItem->itemType() == PageItem::TextFrame) && (doc->appMode == modeEdit))
		{
			CharSelect *dia = new CharSelect(this, currItem, this);
			dia->exec();
			delete dia;
		}
	}
}

void ScribusApp::setUndoMode(bool isObjectSpecific)
{
	objectSpecificUndo = isObjectSpecific;

	if (!objectSpecificUndo && HaveDoc)
		undoManager->showObject(Um::GLOBAL_UNDO_MODE);
	else if (HaveDoc)
	{
		if (view->SelItem.count() == 1)
			undoManager->showObject(view->SelItem.at(0)->getUId());
		else if (view->SelItem.count() == 0)
			undoManager->showObject(doc->currentPage->getUId());
		else
			undoManager->showObject(Um::NO_UNDO_STACK);
	}
}

bool ScribusApp::isObjectSpecificUndo()
{
	return objectSpecificUndo;
}

void ScribusApp::slotTest()
{
}

void ScribusApp::slotTest2()
{
}

void ScribusApp::objectAttributes()
{
	if ((HaveDoc) && (view->SelItem.count() == 1))
	{
		PageItem *pageItem = view->SelItem.at(0);
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

void ScribusApp::generateTableOfContents()
{
	if (HaveDoc)
		tocGenerator->generateDefault();
}

void ScribusApp::mouseReleaseEvent(QMouseEvent *m)
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
					propertiesPalette->Cpal->SetColors(ScApp->doc->PageColors);
					propertiesPalette->updateCList();
				}
				else
					colorName=QString::null;
			}
			if (!colorName.isNull() && view->SelItem.count() > 0)
			{
				for (uint i = 0; i < view->SelItem.count(); ++i)
				{
					if (view->SelItem.at(i)!=NULL)
					{
						PageItem *currItem=view->SelItem.at(i);
						if ((m->stateAfter() & Qt::ControlButton) && (currItem->itemType() == PageItem::TextFrame || currItem->itemType() == PageItem::PathText))
							view->ItemTextBrush(colorName); //Text colour
						else
						if (m->stateAfter() & Qt::AltButton) //Line colour
							setPenFarbe(colorName);
						else
							view->ItemBrush(colorName); //Fill colour
					}
				}
			}
			//propertiesPalette->Cpal->SetColors(ScApp->doc->PageColors);
			//propertiesPalette->updateCList();
			setAppMode(modeNormal);
		}
	}
	if (sendToSuper)
		QMainWindow::mouseReleaseEvent(m);

}

void ScribusApp::insertSampleText()
{
	LoremManager *m = new LoremManager(this, "m", true, 0);
	if (prefsManager->appPrefs.useStandardLI)
		m->insertLoremIpsum("loremipsum.xml", prefsManager->appPrefs.paragraphsLI);
	else
		m->exec();
	delete(m);
}

void ScribusApp::languageChange()
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

void ScribusApp::setDefaultPrinter(const QString& name, const QString& file, const QString& command)
{
	PDef.Pname = name;
	PDef.Dname = file;
	PDef.Command = command;
}

void ScribusApp::getDefaultPrinter(QString *name, QString *file, QString *command)
{
	*name=PDef.Pname;
	*file=PDef.Dname;
	*command=PDef.Command;
}

const bool ScribusApp::fileToolBarVisible()
{
	return fileToolBar->isVisible();
}

const bool ScribusApp::editToolBarVisible()
{
	return editToolBar->isVisible();
}

const bool ScribusApp::mainToolBarVisible()
{
	return mainToolBar->isVisible();
}

const bool ScribusApp::pdfToolBarVisible()
{
	return pdfToolBar->isVisible();
}

const bool ScribusApp::fileWatcherActive()
{
	if (fileWatcher!=NULL)
		return fileWatcher->isActive();
	return false;
}

void ScribusApp::updateColorMenu(QProgressBar* progressBar)
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
			a++;
			if (progressBar != NULL)
				progressBar->setProgress(a);
		}
	}
	connect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
}
