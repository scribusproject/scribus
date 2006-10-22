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
#include "scribuscore.h"
#include "scribus.h"
#include "scribus.moc"
#include "newfile.h"
#include "page.h"
#include "query.h"
#include "mdup.h"
#include "multipleduplicate.h"
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
#include "mergedoc.h"
#include "lineformats.h"
#include "story.h"
#include "autoform.h"
#include "tabmanager.h"
#include "search.h"
#include "fontcombo.h"
#include "colorcombo.h"
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
#include "stylemanager.h"
#include "smlinestyle.h"
#include "util.h"
#include "text/nlsconfig.h"
#include "plugins/formatidlist.h"
#include "scgtplugin.h"
#include "stencilreader.h"
#include "langmgr.h"
#include "smtextstyles.h"
#include "insertaframe.h"
#include "patterndialog.h"

#if defined(_WIN32)
#include "scwinprint.h"
#include "scdocoutput_ps2.h"
#endif

using namespace std;

bool previewDinUse;
bool printDinUse;

QString DocDir;

//extern ScribusCore* ScCore;
extern ScribusQApp* ScQApp;
extern bool emergencyActivated;

ScribusMainWindow::ScribusMainWindow()
{
	actionManager=0;
	scrMenuMgr=0;
	prefsManager=0;
	mainWindowStatusLabel=0;
#ifdef Q_WS_MAC
	noIcon = loadIcon("noicon.xpm");
#endif
} // ScribusMainWindow::ScribusMainWindow()

/*
 * retval 0 - ok, 1 - no fonts, ...
 */
int ScribusMainWindow::initScMW(bool primaryMainWindow)
{
	int retVal=0;

//	CommonStrings::languageChange();
	previewDinUse = false;
	printDinUse = false;
//	guiLanguage = newGuiLanguage;
//	initSplash(showSplash);
	setUsesBigPixmaps(true);
	CurrStED = NULL;
	setCaption( tr("Scribus " VERSION));
	setKeyCompression(false);
	setIcon(loadIcon("AppIcon.png"));
	scrActionGroups.clear();
	scrActionGroups.setAutoDelete(true);
	scrActions.clear();
	scrRecentFileActions.clear();
	scrRecentPasteActions.clear();
	scrWindowsActions.clear();
	scrLayersActions.clear();
	scrMenuMgr = new MenuManager(menuBar());
	prefsManager = PrefsManager::instance();
	objectSpecificUndo = false;
	
	undoManager = UndoManager::instance();
	tocGenerator = new TOCGenerator();
	

	initDefaultValues();
	
	initStatusBar();
	
	qApp->processEvents();

	BuFromApp = false;
	
	actionManager = new ActionManager(this, "actionManager");
	actionManager->init(this);
	initMenuBar();
	initToolBars();
	buildFontMenu();
 	ScCore->pluginManager->setupPluginActions(this);
 	ScCore->pluginManager->languageChange();
	initKeyboardShortcuts();
	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Setting up Shortcuts") );
	SetShortCut();

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
	
	prefsManager->setupMainWindow(this);

	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Initializing Story Editor") );
	storyEditor = new StoryEditor(this);

	DocDir = prefsManager->documentDir();


	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Initializing Hyphenator") );
	QString preLang = prefsManager->appPrefs.Language;
	initHyphenator();
	if (Sprachen.contains(preLang))
		prefsManager->appPrefs.Language = preLang;
	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Reading Scrapbook") );
	initScrapbook();

	scrActions["helpTooltips"]->setOn(prefsManager->appPrefs.showToolTips);
	ToggleTips();
	propertiesPalette->setFontSize();
	if (scrActions["SaveAsDocumentTemplate"])
		scrActions["SaveAsDocumentTemplate"]->setEnabled(false);
	
	connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), this, SLOT(removeRecent(QString)));
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
	connect(ClipB, SIGNAL(dataChanged()), this, SLOT(ClipChange()));
	connect(ClipB, SIGNAL(selectionChanged()), this, SLOT(ClipChange()));
	setAcceptDrops(true);
	return retVal;
}

ScribusMainWindow::~ScribusMainWindow()
{
}


void ScribusMainWindow::initToolBars()
{
	fileToolBar = new ScToolBar( tr("File"), "File", this);
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

	mainToolBar = new ModeToolBar(this);
	pdfToolBar = new PDFToolBar(this);
	
	connect(mainToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarTools"], SLOT(setOn(bool)));
	connect(scrActions["toolsToolbarPDF"], SIGNAL(toggled(bool)), pdfToolBar, SLOT(setShown(bool)));
	connect(pdfToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarPDF"], SLOT(setOn(bool)));
	connect(scrActions["toolsToolbarTools"], SIGNAL(toggled(bool)), mainToolBar, SLOT(setShown(bool)) );
}

void ScribusMainWindow::initDefaultValues()
{
	HaveDoc = false;
	ScriptRunning = false;
	view = NULL;
	doc = NULL;
	Buffer2 = "";
	DocNr = 1;
	PrinterUsed = false;
	PDef.Pname = "";
	PDef.Dname = "";
	PDef.Command = "";
	keyrep = false;
	_arrowKeyDown = false;
	ClipB = QApplication::clipboard();
	palettesStatus[0] = false;
	guidesStatus[0] = false;
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
	outlinePalette = new Tree(this);
	outlinePalette->setMainWindow(this);
	connect( scrActions["toolsOutline"], SIGNAL(toggled(bool)) , outlinePalette, SLOT(setPaletteShown(bool)) );
	connect( outlinePalette, SIGNAL(paletteShown(bool)), scrActions["toolsOutline"], SLOT(setOn(bool)));
	propertiesPalette = new Mpalette(ScCore->m_PaletteParent);
	propertiesPalette->setMainWindow(this);
	connect( scrActions["toolsProperties"], SIGNAL(toggled(bool)) , propertiesPalette, SLOT(setPaletteShown(bool)) );
	connect( propertiesPalette, SIGNAL(paletteShown(bool)), scrActions["toolsProperties"], SLOT(setOn(bool)));

	//CB dont need this until we have a doc...
	//propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
	propertiesPalette->Fonts->RebuildList(0);
	propertiesPalette->installEventFilter(this);
	nodePalette = new NodePalette(this);
	nodePalette->installEventFilter(this);
	layerPalette = new LayerPalette(this);
	guidePalette = new GuideManager(this);
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
	
	undoPalette = new UndoPalette(this, "undoPalette");
	undoPalette->installEventFilter(this);
	undoManager->registerGui(undoPalette);
	connect(undoPalette, SIGNAL(paletteShown(bool)), this, SLOT(setUndoPalette(bool)));
	connect(undoPalette, SIGNAL(objectMode(bool)), this, SLOT(setUndoMode(bool)));

	// initializing style manager here too even it's not strictly a palette
	styleManager = new StyleManager(this, "styleManager");
	styleManager->addStyle(new SMLineStyle());
	styleManager->addStyle(new SMParagraphStyle());
	styleManager->addStyle(new SMCharacterStyle());
	connect(styleManager, SIGNAL(closed()), scrActions["editStyles"], SLOT(toggle()));

	connect(docCheckerPalette, SIGNAL(selectElement(int, int)), this, SLOT(selectItemsFromOutlines(int, int)));
	connect(docCheckerPalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(docCheckerPalette, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
	connect(outlinePalette, SIGNAL(selectElement(int, int, bool)), this, SLOT(selectItemsFromOutlines(int, int, bool)));
	connect(outlinePalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(outlinePalette, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
	connect(propertiesPalette->Spal, SIGNAL(newStyle(int)), this, SLOT(setNewParStyle(int)));
//	connect(propertiesPalette, SIGNAL(EditLSt()), this, SLOT(slotEditLineStyles()));
	connect(nodePalette, SIGNAL(Schliessen()), this, SLOT(NoFrameEdit()));
	connect(nodePalette, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(layerPalette, SIGNAL(LayerChanged()), this, SLOT(showLayer()));

	connect(bookmarkPalette->BView, SIGNAL(MarkMoved()), this, SLOT(StoreBookmarks()));
	connect(bookmarkPalette->BView, SIGNAL(changed()), this, SLOT(slotDocCh()));
	connect(bookmarkPalette->BView, SIGNAL(SelectElement(PageItem *)), this, SLOT(selectItemsFromOutlines(PageItem *)));
	// guides
	connect(scrActions["pageManageGuides"], SIGNAL(toggled(bool)), guidePalette, SLOT(setPaletteShown(bool)));
	connect(guidePalette, SIGNAL(paletteShown(bool)), scrActions["pageManageGuides"], SLOT(setOn(bool)));
}

void ScribusMainWindow::initScrapbook()
{
	QString scrapbookFileO = QDir::convertSeparators(prefsManager->preferencesLocation()+"/scrap13.scs");
	QFileInfo scrapbookFileInfoO = QFileInfo(scrapbookFileO);
	if (scrapbookFileInfoO.exists())
	{
		scrapbookPalette->readOldContents(scrapbookFileO, QDir::convertSeparators(prefsManager->preferencesLocation()+"/scrapbook/main"));
		QDir d = QDir();
		d.rename(scrapbookFileO, QDir::convertSeparators(prefsManager->preferencesLocation()+"/scrap13.backup"));
	}
	QString scrapbookTemp = QDir::convertSeparators(prefsManager->preferencesLocation()+"/scrapbook/tmp");
	QFileInfo scrapbookTempInfo = QFileInfo(scrapbookTemp);
	if (scrapbookTempInfo.exists())
		scrapbookPalette->readTempContents(scrapbookTemp);
	QString scrapbookFile = QDir::convertSeparators(prefsManager->preferencesLocation()+"/scrapbook/main");
	QFileInfo scrapbookFileInfo = QFileInfo(scrapbookFile);
	if (scrapbookFileInfo.exists())
		scrapbookPalette->readContents(scrapbookFile);
	scrapbookPalette->setScrapbookFileName(scrapbookFile);
	scrapbookPalette->setOpenScrapbooks(prefsManager->appPrefs.RecentScrapbooks);
	rebuildRecentPasteMenu();
	connect(scrapbookPalette, SIGNAL(updateRecentMenue()), this, SLOT(rebuildRecentPasteMenu()));
}

bool ScribusMainWindow::warningVersion(QWidget *parent)
{
	bool retval = false;
	int t = ScMessageBox::warning(parent, QObject::tr("Scribus Development Version"), "<qt>" +
								 QObject::tr("You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?") + "</qt>",
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
	scrMenuMgr->addMenuItem(scrActions["fileImportText2"], "FileImport");
	scrMenuMgr->addMenuItem(scrActions["fileImportAppendText"], "FileImport");
	scrMenuMgr->addMenuItem(scrActions["fileImportImage"], "FileImport");
	scrMenuMgr->createMenu("FileExport", QIconSet(noIcon), tr("&Export"), "File");
	scrMenuMgr->addMenuItem(scrActions["fileExportText"], "FileExport");
	scrMenuMgr->addMenuItem(scrActions["fileExportAsEPS"], "FileExport");
	scrMenuMgr->addMenuItem(scrActions["fileExportAsPDF"], "FileExport");
	scrMenuMgr->addMenuSeparator("File");
	scrMenuMgr->addMenuItem(scrActions["fileDocSetup"], "File");
	scrMenuMgr->addMenuItem(scrActions["filePreferences"], "File");
	scrMenuMgr->addMenuSeparator("File");
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
	scrActions["fileImportText2"]->setEnabled(false);
	scrActions["fileImportImage"]->setEnabled(false);
	scrActions["fileImportAppendText"]->setEnabled(false);
	scrActions["pageImport"]->setEnabled(false);
	scrActions["fileExportText"]->setEnabled(false);
	scrActions["fileExportAsEPS"]->setEnabled(false);
	scrActions["fileExportAsPDF"]->setEnabled(false);
	scrMenuMgr->setMenuEnabled("FileImport", false);
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
	recentPasteMenuName="EditPasteRecent";
	scrMenuMgr->createMenu(recentPasteMenuName, QIconSet(noIcon), tr("Paste Recent"), "Edit");
	scrMenuMgr->createMenu("EditContents", QPixmap(noIcon), tr("Contents"), "Edit");
	scrMenuMgr->addMenuItem(scrActions["editCopyContents"], "EditContents");
	scrMenuMgr->addMenuItem(scrActions["editPasteContents"], "EditContents");
	scrMenuMgr->addMenuItem(scrActions["editPasteContentsAbs"], "EditContents");
	scrMenuMgr->addMenuItem(scrActions["editClearContents"], "EditContents");
	scrMenuMgr->addMenuSeparator("Edit");
	scrMenuMgr->addMenuItem(scrActions["editSelectAll"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editDeselectAll"], "Edit");
	scrMenuMgr->addMenuSeparator("Edit");
	scrMenuMgr->addMenuItem(scrActions["editSearchReplace"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["toolsEditWithStoryEditor"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editEditWithImageEditor"], "Edit");
	scrMenuMgr->addMenuSeparator("Edit");
	scrMenuMgr->addMenuItem(scrActions["editColors"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editPatterns"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editStyles"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editParaStyles"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editLineStyles"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editMasterPages"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editJavascripts"], "Edit");
	scrActions["editUndoAction"]->setEnabled(false);
	scrActions["editRedoAction"]->setEnabled(false);
	scrActions["editActionMode"]->setEnabled(true);
	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	scrActions["editPaste"]->setEnabled(false);
	scrMenuMgr->setMenuEnabled("EditPasteRecent", false);
	scrActions["editCopyContents"]->setEnabled(false);
	scrActions["editPasteContents"]->setEnabled(false);
	scrActions["editPasteContentsAbs"]->setEnabled(false);
	scrActions["editClearContents"]->setEnabled(false);
	scrActions["editSelectAll"]->setEnabled(false);
	scrActions["editDeselectAll"]->setEnabled(false);
	scrActions["editSearchReplace"]->setEnabled(false);
	scrActions["editPatterns"]->setEnabled(false);
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
	ColorMenC = new ColorCombo(false);
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
	scrMenuMgr->addMenuItem(scrActions["itemSendToPattern"], "Item");
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
	scrActions["itemPrintingEnabled"]->setEnabled(false);
	scrActions["itemImageIsVisible"]->setEnabled(false);
	scrMenuMgr->setMenuEnabled("ItemConvertTo", false);
	scrActions["itemConvertToBezierCurve"]->setEnabled(false);
	scrActions["itemConvertToImageFrame"]->setEnabled(false);
	scrActions["itemConvertToOutlines"]->setEnabled(false);
	scrActions["itemConvertToPolygon"]->setEnabled(false);
	scrActions["itemConvertToTextFrame"]->setEnabled(false);

	//Insert menu
	scrMenuMgr->createMenu("Insert", tr("I&nsert"));
	scrMenuMgr->addMenuItem(scrActions["insertFrame"], "Insert");
	scrMenuMgr->addMenuSeparator("Insert");
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

	scrMenuMgr->createMenu("InsertChar", QPixmap(noIcon), tr("&Character"), "Insert");
	//scrMenuMgr->addMenuToMenu("InsertChar", "Insert");
	scrMenuMgr->addMenuItem(scrActions["unicodePageNumber"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeSmartHyphen"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeNonBreakingHyphen"], "InsertChar");
	scrMenuMgr->addMenuSeparator("InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeCopyRight"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeRegdTM"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeTM"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeSolidus"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeBullet"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeMidpoint"], "InsertChar");
	scrMenuMgr->addMenuSeparator("InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeDashEm"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeDashEn"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeDashFigure"], "InsertChar");
	scrMenuMgr->addMenuItem(scrActions["unicodeDashQuotation"], "InsertChar");

	scrMenuMgr->createMenu("InsertQuote", QPixmap(noIcon), tr("&Quote"), "Insert");
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

	scrMenuMgr->createMenu("InsertSpace", QPixmap(noIcon), tr("S&paces && Breaks"), "Insert");
	scrMenuMgr->addMenuItem(scrActions["unicodeNonBreakingSpace"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeSpaceEN"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeSpaceEM"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeSpaceThin"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeSpaceThick"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeSpaceMid"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeSpaceHair"], "InsertSpace");
	scrMenuMgr->addMenuSeparator("InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeNewLine"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeFrameBreak"], "InsertSpace");
	scrMenuMgr->addMenuItem(scrActions["unicodeColumnBreak"], "InsertSpace");

	scrMenuMgr->createMenu("InsertLigature", QPixmap(noIcon), tr("Liga&ture"), "Insert");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_ff"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_fi"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_fl"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_ffi"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_ffl"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_ft"], "InsertLigature");
	scrMenuMgr->addMenuItem(scrActions["unicodeLigature_st"], "InsertLigature");

	scrMenuMgr->addMenuSeparator("Insert");
	scrMenuMgr->addMenuItem(scrActions["insertSampleText"], "Insert");
	scrActions["insertFrame"]->setEnabled(false);
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
	scrMenuMgr->addMenuItem(scrActions["viewShowLayerMarkers"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowImages"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowGrid"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowGuides"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowColumnBorders"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowBaseline"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowTextChain"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowTextControls"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowRulers"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewRulerMode"], "View");

	scrActions["viewShowRulers"]->setEnabled(false);

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
	mainWindowXPosDataLabel = new QLabel( "        ", statusBar(), "dt");
	mainWindowYPosDataLabel = new QLabel( "        ", statusBar(), "ydt");

	statusBar()->addWidget(mainWindowStatusLabel, 6, true);
	statusBar()->addWidget(mainWindowProgressBar, 0, true);
	statusBar()->addWidget(mainWindowXPosLabel, 0, true);
	statusBar()->addWidget(mainWindowXPosDataLabel, 1, true);
	statusBar()->addWidget(mainWindowYPosLabel, 0, true);
	statusBar()->addWidget(mainWindowYPosDataLabel, 1, true);
}

void ScribusMainWindow::setStatusBarMousePosition(double xp, double yp)
{
	double xn = xp;
	double yn = yp;
	if (doc->guidesSettings.rulerMode)
	{
		xn -= doc->currentPage()->xOffset();
		yn -= doc->currentPage()->yOffset();
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

void ScribusMainWindow::setStatusBarInfoText(QString newText)
{
	if (mainWindowStatusLabel)
		mainWindowStatusLabel->setText(newText);
}

//CB-->Doc
void ScribusMainWindow::setTBvals(PageItem *currItem)
{
	if (currItem->itemText.length() != 0)
	{
//		int ChPos = QMIN(currItem->CPos, static_cast<int>(currItem->itemText.length()-1));
		const ParagraphStyle currPStyle(currItem->currentStyle());
		int currentParaStyle = currPStyle.parent()? findParagraphStyle(doc, *dynamic_cast<const ParagraphStyle*>(currPStyle.parent())) : 0;
		setAbsValue(currPStyle.alignment());
		propertiesPalette->setParStyle(currentParaStyle);
		doc->currentStyle.charStyle() = currItem->currentCharStyle();
		emit TextUnderline(doc->currentStyle.charStyle().underlineOffset(), doc->currentStyle.charStyle().underlineWidth());
		emit TextStrike(doc->currentStyle.charStyle().strikethruOffset(), doc->currentStyle.charStyle().strikethruWidth());
		emit TextShadow(doc->currentStyle.charStyle().shadowXOffset(), doc->currentStyle.charStyle().shadowYOffset());
		emit TextFarben(doc->currentStyle.charStyle().strokeColor(), 
						doc->currentStyle.charStyle().fillColor(), 
						doc->currentStyle.charStyle().strokeShade(), 
						doc->currentStyle.charStyle().fillShade());
		emit TextIFont(doc->currentStyle.charStyle().font().scName());
		emit TextISize(doc->currentStyle.charStyle().fontSize());
		emit TextUSval(doc->currentStyle.charStyle().tracking());
		emit TextStil(doc->currentStyle.charStyle().effects());
		emit TextScale(doc->currentStyle.charStyle().scaleH());
		emit TextScaleV(doc->currentStyle.charStyle().scaleV());
		emit TextBase(doc->currentStyle.charStyle().baselineOffset());
		emit TextOutline(doc->currentStyle.charStyle().outlineWidth());
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
	if (HaveDoc)
	{
		if (doc->appMode==modeEdit)
		{
			if (doc->m_Selection->count() == 1)
			{
				PageItem *currItem = doc->m_Selection->itemAt(0);
				if (currItem!=NULL)
				{
					if (unicodevalue!=-1)
					{
						if (currItem->HasSel && currItem->itemType()==PageItem::TextFrame)
							currItem->asTextFrame()->deleteSelectedTextFromFrame();

						currItem->itemText.insertChars(currItem->CPos, QString(QChar(unicodevalue)));
						currItem->CPos += 1;
						currItem->Tinput = true;
						view->RefreshItem(currItem);
					}
					else if (actionName=="unicodeSmartHyphen") //ignore the char as we use an attribute if the text item, for now.
					{
						if (currItem->CPos-1>0)
						{
#ifndef NLS_PROTO
							StyleFlag fl = currItem->itemText.item(QMAX(currItem->CPos-1,0))->effects();
							fl |= ScStyle_HyphenationPossible;
							currItem->itemText.item(QMAX(currItem->CPos-1,0))->setEffects(fl);
#else
							currItem->itemText.insertChars(currItem->CPos, QString(SpecialChars::SHYPHEN));
							currItem->CPos += 1;
#endif
							currItem->Tinput = true;
							view->RefreshItem(currItem);
						}
					}
				}
			}
		}
	}
}

bool ScribusMainWindow::eventFilter( QObject* /*o*/, QEvent *e )
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
		if (doc->m_Selection->count() != 0)
		{
			currItem = doc->m_Selection->itemAt(0);
			switch (doc->appMode)
			{
				case modeNormal:
					currItem->Sizing = false;
					if (doc->SubMode != -1)
					{
						view->Deselect(false);
						doc->Items->remove(currItem->ItemNr);
					}
					else
						view->Deselect(false);
					break;
				case modeLinkFrames:
				case modeUnlinkFrames:
				case modeEdit:
				case modeRotation:
				case modeEditGradientVectors:
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
						currItem->setPolyClip(qRound(QMAX(currItem->lineWidth() / 2.0, 1)));
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
		view->m_MouseButtonPressed = false;
		doc->DragP = false;
		doc->leaveDrag = false;
		view->operItemMoving = false;
		view->operItemResizing = false;
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

		if ((doc->appMode != modeEdit) && (doc->m_Selection->count() == 0))
		{
			int pg;
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
				if (doc->masterPageMode())
					view->scrollBy(0, -prefsManager->mouseWheelValue());
				else
				{
					pg = doc->currentPageNumber();
					if ((buttonState & ShiftButton) && !(buttonState & ControlButton) && !(buttonState & AltButton))
						pg--;
					else
						pg -= doc->pageSets[doc->currentPageLayout].Columns;
					if (pg > -1)
						view->GotoPage(pg);
				}
				keyrep = false;
				return;
				break;
			case Key_Next:
				if (doc->masterPageMode())
					view->scrollBy(0, prefsManager->mouseWheelValue());
				else
				{
					pg = doc->currentPageNumber();
					if ((buttonState & ShiftButton) && !(buttonState & ControlButton) && !(buttonState & AltButton))
						pg++;
					else
						pg += doc->pageSets[doc->currentPageLayout].Columns;
					if (pg < doc->Pages->count())
						view->GotoPage(pg);
				}
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
		 * -- Use the arrow keys to move an item or group around for !inches:
		 		With no meta, by 1.0 unit
		 		Ctrl, by 10.0 units
		 		Shift by 0.1 units
		 		Ctrl Shift 0.01 units
		 	- For inches:
		 		With no meta, by 1.0 pt
		 		Ctrl, by 1.0 unit
		 		Shift by 0.1 units
		 		Ctrl Shift 0.01 units
		 * -- Use the arrow keys to resize an item:
		 		Alt right arrow, move right side outwards (expand)
		 		Alt left arrow, move left side outwards (expand)
		 		Alt Shift right arrow, move left side inwards (shrink)
		 		Alt Shift left arrow, move right side inwards (shrink)
		 * -- In edit mode of an image frame, use the arrow keys to resize the image:
		 		(flows to pageitem_imageframe for control)
		 		Alt right arrow, move right side of image outwards (expand)
		 		Alt left arrow, move right side inwards (shrink)
		 		Alt down arrow, move bottom side downwards (expand)
		 		Alt up arrow, move top side inwards (shrink)
		 */
		if (doc->m_Selection->count() != 0)
		{
			double moveBy=1.0;
			if (doc->unitIndex()!=SC_INCHES)
			{
				if ((buttonState & ShiftButton) && !(buttonState & ControlButton) && !(buttonState & AltButton))
					moveBy=0.1;
				else if (!(buttonState & ShiftButton) && (buttonState & ControlButton) && !(buttonState & AltButton))
					moveBy=10.0;
				else if ((buttonState & ShiftButton) && (buttonState & ControlButton) && !(buttonState & AltButton))
					moveBy=0.01;
			
				moveBy/=doc->unitRatio();//Lets allow movement by the current doc ratio, not only points
			}
			else
			{
				if ((buttonState & ShiftButton) && !(buttonState & ControlButton) && !(buttonState & AltButton))
					moveBy=0.1/doc->unitRatio();
				else if (!(buttonState & ShiftButton) && (buttonState & ControlButton) && !(buttonState & AltButton))
					moveBy=1.0/doc->unitRatio();
				else if ((buttonState & ShiftButton) && (buttonState & ControlButton) && !(buttonState & AltButton))
					moveBy=0.01/doc->unitRatio();
			}
			bool resizing=((buttonState & AltButton) && !(buttonState & ControlButton));
			bool resizingsmaller=(resizing && (buttonState & ShiftButton));
			double resizeBy=1.0;
			//CB with control locked out due to the requirement of moveby of 0.01, we cannot support
			//resizeby 10 units unless we move to supporting modifier keys that most people dont have.
			//if (buttonState & ControlButton)
			//	resizeBy*=10.0;
			resizeBy/=doc->unitRatio();
			if (resizingsmaller)
				resizeBy*=-1.0;


			PageItem *currItem = doc->m_Selection->itemAt(0);
			switch (doc->appMode)
			{
			case modeNormal:
				switch (kk)
				{
				case Key_Backspace:
				case Key_Delete:
					doc->itemSelection_DeleteItem();
					break;
				case Key_Prior:
					if (!currItem->locked())
					{
						view->RaiseItem();
					}
					break;
				case Key_Next:
					if (!currItem->locked())
					{
						view->LowerItem();
					}
					break;
				case Key_Left:
					if (!currItem->locked())
					{
						if (!resizing)
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
							/* Don't use Grid or Guide Snapping when dragging Items or Groups with the keyboard */
							/* as the user might be trying to fine tune a position */
								bool sav1 = doc->SnapGuides;
								bool sav2 = doc->useRaster;
								doc->SnapGuides = false;
								doc->useRaster = false;
								view->moveGroup(-moveBy, 0);
								doc->SnapGuides = sav1;
								doc->useRaster = sav2;
							}
						}
						else
						{
							//CB If in EditContour mode, allow contour line to be scaled with arrow keys too
							if(view->EditContour)
								view->TransformPoly(10, 0, resizeBy/unitGetRatioFromIndex(doc->unitIndex()));
							else
							{
								if (resizingsmaller)
								{
									currItem->Sizing = false;
									view->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr, true);
								}
								else
								{
									view->MoveItem(-resizeBy, 0, currItem, false);
									currItem->moveImageXYOffsetBy(resizeBy/currItem->imageXScale(), 0);
									currItem->Sizing = false;
									view->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr, true);
								}
 							}
						}
						view->updateContents();
						slotDocCh();
					}
					break;
				case Key_Right:
					if (!currItem->locked())
					{
						if (!resizing)
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
							/* Don't use Grid or Guide Snapping when dragging Items or Groups with the keyboard */
							/* as the user might be trying to fine tune a position */
								bool sav1 = doc->SnapGuides;
								bool sav2 = doc->useRaster;
								doc->SnapGuides = false;
								doc->useRaster = false;
								view->moveGroup(moveBy, 0);
								doc->SnapGuides = sav1;
								doc->useRaster = sav2;
							}
						}
						else
						{
							//CB If in EditContour mode, allow contour line to be scaled with arrow keys too
							if(view->EditContour)
								view->TransformPoly(11, 0, resizeBy/unitGetRatioFromIndex(doc->unitIndex()));
							else
							{
								if (resizingsmaller)
								{
									view->MoveItem(-resizeBy, 0, currItem, false);
									currItem->moveImageXYOffsetBy(resizeBy/currItem->imageXScale(), 0);
									currItem->Sizing = false;
									view->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr, true);
								}
								else
								{
									currItem->Sizing = false;
									view->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr, true);
								}
							}
						}
						view->updateContents();
						slotDocCh();
					}
					break;
				case Key_Up:
					if (!currItem->locked())
					{
						if (!resizing)
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
							/* Don't use Grid or Guide Snapping when dragging Items or Groups with the keyboard */
							/* as the user might be trying to fine tune a position */
								bool sav1 = doc->SnapGuides;
								bool sav2 = doc->useRaster;
								doc->SnapGuides = false;
								doc->useRaster = false;
								view->moveGroup(0, -moveBy);
								doc->SnapGuides = sav1;
								doc->useRaster = sav2;
							}
						}
						else
						{
							//CB If in EditContour mode, allow contour line to be scaled with arrow keys too
							if(view->EditContour)
								view->TransformPoly(12, 0, resizeBy/unitGetRatioFromIndex(doc->unitIndex()));
							else
							{
								if (resizingsmaller)
								{
									currItem->Sizing = false;
									view->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr, true);
								}
								else
								{
									view->MoveItem(0, -resizeBy, currItem, false);
									currItem->moveImageXYOffsetBy(0, resizeBy/currItem->imageYScale());
									currItem->Sizing = false;
									view->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr, true);
								}
							}
						}
						view->updateContents();
						slotDocCh();
					}
					break;
				case Key_Down:
					if (!currItem->locked())
					{
						if (!resizing)
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
							/* Don't use Grid or Guide Snapping when dragging Items or Groups with the keyboard */
							/* as the user might be trying to fine tune a position */
								bool sav1 = doc->SnapGuides;
								bool sav2 = doc->useRaster;
								doc->SnapGuides = false;
								doc->useRaster = false;
								view->moveGroup(0, moveBy);
								doc->SnapGuides = sav1;
								doc->useRaster = sav2;
							}
						}
						else
						{
							//CB If in EditContour mode, allow contour line to be scaled with arrow keys too
							if(view->EditContour)
								view->TransformPoly(13, 0, resizeBy/unitGetRatioFromIndex(doc->unitIndex()));
							else
							{
								if (resizingsmaller)
								{
									view->MoveItem(0, -resizeBy, currItem, false);
									currItem->moveImageXYOffsetBy(0, resizeBy/currItem->imageYScale());
									currItem->Sizing = false;
									view->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr, true);
								}
								else
								{
									currItem->Sizing = false;
									view->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr, true);
								}
							}
						}
						view->updateContents();
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
					currItem->handleModeEditKey(k, keyrep);
					/*
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
						currItem->moveImageInFrame(dX, dY);
						view->updateContents(currItem->getRedrawBounding(view->scale()));
					}*/
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
	if (HaveDoc && (k->state() & ControlButton))
	{
		if ((doc->appMode == modePanning) && (k->state() & Qt::RightButton))
			setAppMode(modeNormal);
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
				uint docSelectionCount=doc->m_Selection->count();
				if ((docSelectionCount != 0) && (doc->appMode == modeNormal) && (doc->EditClip) && (view->ClRe != -1))
					view->updateContents();
				for (uint i = 0; i < docSelectionCount; ++i)
					doc->m_Selection->itemAt(i)->checkChanges(true);
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
		uint windowCount=windows.count();
		for ( uint i = 0; i < windowCount; ++i )
		{
			newActWin(windows.at(i));
			tw = ActWin;
			ActWin->close();
			if (tw == ActWin)
			{
				ce->ignore();
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
	guidePalette->hide();

	// Clean up plugins, THEN save prefs to disk
	ScCore->pluginManager->cleanupPlugins();
	if (!prefsManager->appPrefs.persistentScrapbook)
		scrapbookPalette->CleanUpTemp();
	prefsManager->appPrefs.RecentScrapbooks.clear();
	prefsManager->appPrefs.RecentScrapbooks = scrapbookPalette->getOpenScrapbooks();
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
	NewDoc* dia = new NewDoc(this, RecentDocs, true);
	if (dia->exec())
	{
		if (dia->tabSelected == 0)
		{
			int facingPages = dia->choosenLayout;
			int firstPage = dia->docLayout->firstPage->currentItem();
			double topMargin = dia->marginGroup->top();
			double bottomMargin = dia->marginGroup->bottom();
			double leftMargin = dia->marginGroup->left();
			double rightMargin = dia->marginGroup->right();
			double columnDistance = dia->Dist;
			double pageWidth = dia->pageWidth;
			double pageHeight = dia->pageHeight;
			double numberCols = dia->numberOfCols->value();
			bool autoframes = dia->autoTextFrameGroupBox->isChecked();
			int orientation = dia->Orient;
			int pageCount=dia->pageCountSpinBox->value();
			PageSize ps2(dia->pageSizeComboBox->currentText());
			QString pagesize = ps2.name();
			doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->unitOfMeasureComboBox->currentItem(), firstPage, orientation, 1, pagesize, true, pageCount);
			doc->pageSets[facingPages].FirstPage = firstPage;
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
				QString fileName(dia->recentDocListBox->currentText());
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
	if (HaveDoc && doc->EditClip)
		ToggleFrameEdit();
	bool retVal = false;
	NewDoc* dia = new NewDoc(this, RecentDocs);
	if (dia->exec())
	{
		int facingPages = dia->choosenLayout;
		int firstPage = dia->docLayout->firstPage->currentItem();
		double topMargin = dia->marginGroup->top();
		double bottomMargin = dia->marginGroup->bottom();
		double leftMargin = dia->marginGroup->left();
		double rightMargin = dia->marginGroup->right();
		double columnDistance = dia->Dist;
		double pageWidth = dia->pageWidth;
		double pageHeight = dia->pageHeight;
		double numberCols = dia->numberOfCols->value();
		bool autoframes = dia->autoTextFrameGroupBox->isChecked();
		int orientation = dia->Orient;
		int pageCount=dia->pageCountSpinBox->value();
		PageSize ps2(dia->pageSizeComboBox->currentText());
		if (doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->unitOfMeasureComboBox->currentItem(), firstPage, orientation, 1, ps2.name(), true, pageCount))
		{
			doc->pageSets[facingPages].FirstPage = firstPage;
			mainWindowStatusLabel->setText( tr("Ready"));
			HaveNewDoc();
			retVal = true;
		}
	}
	delete dia;
	return retVal;
}

//TODO move to core, assign doc to doc list, optionally create gui for it
ScribusDoc *ScribusMainWindow::newDoc(double width, double height, double topMargin, double leftMargin, double rightMargin, double bottomMargin, double columnDistance, double columnCount, bool autoTextFrames, int pageArrangement, int unitIndex, int firstPageLocation, int orientation, int firstPageNumber, const QString& defaultPageSize, bool requiresGUI, int pageCount, bool showView)
{
	return doFileNew(width, height, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, columnCount, autoTextFrames, pageArrangement, unitIndex, firstPageLocation, orientation, firstPageNumber, defaultPageSize, requiresGUI, pageCount, showView);
	/* TODO CB finish later this week.
	if (HaveDoc)
		doc->OpenNodes = outlinePalette->buildReopenVals();
	MarginStruct margins(topMargin, leftMargin, bottomMargin, rightMargin);
	DocPagesSetup pagesSetup(pageArrangement, firstPageLocation, firstPageNumber, orientation, autoTextFrames, columnDistance, columnCount);
	QString newDocName(tr("Document")+"-"+QString::number(DocNr));
	doc = new ScribusDoc(newDocName, unitindex, pagesize, margins, pagesSetup);
		doc->setLoading(true);
	doc->setup(unitIndex, pageArrangement, firstPageLocation, orientation, firstPageNumber, defaultPageSize, newDocName);
	HaveDoc++;
	DocNr++;
	if (ScCore->haveCMS() && doc->CMSSettings.CMSinUse)
		recalcColors();
	//CB NOTE should be all done now
	doc->setPage(width, height, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, columnCount, autoTextFrames, pageArrangement);
	doc->setMasterPageMode(false);
	doc->addMasterPage(0, CommonStrings::masterPageNormal);
	int createCount=QMAX(pageCount,1);
	for (int i = 0; i < createCount; ++i)
		doc->addPage(i, CommonStrings::masterPageNormal, true);
	doc->addSection();
	doc->setFirstSectionFromFirstPageNumber();
	doc->setModified(false);
	doc->OpenNodes.clear();
	actionManager->disconnectNewDocActions();
	actionManager->connectNewDocActions(doc);
	//<<View and window code
	ScribusWin* w = new ScribusWin(wsp, doc);
	w->setMainWindow(this);
	if (view!=NULL)
	{
		actionManager->disconnectNewViewActions();
		disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	}
	view = new ScribusView(w, this, doc);
	doc->setCurrentPage(doc->Pages->at(0));
	doc->setGUI(this, view);
	doc->setLoading(false);
	//run after setGUI to set up guidepalette ok
	
	view->setScale(prefsManager->displayScale());
	actionManager->connectNewViewActions(view);
	alignDistributePalette->setDoc(doc);
	docCheckerPalette->clearErrorList();
	w->setView(view);
	ActWin = w;
	doc->WinHan = w;
	w->setCentralWidget(view);
	doc->connectDocSignals(); //Must be before the first reformpages
	view->reformPages(true);
	//>>

	connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	//connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
	connect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	//	connect(w, SIGNAL(SaveAndClose()), this, SLOT(DoSaveClose()));

	//Independent finishing tasks after doc setup
	if (showView)
	{
		if ( wsp->windowList().isEmpty() )
			w->showMaximized();
		else
			w->show();
		view->show();
	}
	connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	connect(ScCore->fileWatcher, SIGNAL(fileChanged(QString)), doc, SLOT(updatePict(QString)));
	connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString)), doc, SLOT(removePict(QString)));
	scrActions["fileSave"]->setEnabled(false);
	undoManager->switchStack(doc->DocName);
	styleManager->currentDoc(doc);
	tocGenerator->setDoc(doc);

	return doc;
	*/
}

ScribusDoc *ScribusMainWindow::doFileNew(double width, double height, double topMargin, double leftMargin, double rightMargin, double bottomMargin, double columnDistance, double columnCount, bool autoTextFrames, int pageArrangement, int unitIndex, int firstPageLocation, int orientation, int firstPageNumber, const QString& defaultPageSize, bool requiresGUI, int pageCount, bool showView)
{
	if (HaveDoc)
	{
		doc->OpenNodes = outlinePalette->buildReopenVals();
	}
	undoManager->setUndoEnabled(false);
	MarginStruct margins(topMargin, leftMargin, bottomMargin, rightMargin);
	DocPagesSetup pagesSetup(pageArrangement, firstPageLocation, firstPageNumber, orientation, autoTextFrames, columnDistance, columnCount);
	QString newDocName( tr("Document")+"-"+QString::number(DocNr));
	ScribusDoc *tempDoc = new ScribusDoc();
	if (requiresGUI)
		doc=tempDoc;
	//tempDoc = new ScribusDoc(newDocName, unitindex, pagesize, margins, pagesSetup);
	tempDoc->setLoading(true);
	tempDoc->setup(unitIndex, pageArrangement, firstPageLocation, orientation, firstPageNumber, defaultPageSize, newDocName);
	if (requiresGUI)
	{
		HaveDoc++;
		DocNr++;
	}
	if (ScCore->haveCMS() && tempDoc->CMSSettings.CMSinUse)
		recalcColors();
	//CB NOTE should be all done now
	tempDoc->setPage(width, height, topMargin, leftMargin, rightMargin, bottomMargin, columnCount, columnDistance, autoTextFrames, pageArrangement);
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
	QWorkspace* qwsp=0;
	if (requiresGUI)
		qwsp=wsp;
	ScribusWin* w = new ScribusWin(qwsp, tempDoc);
	w->setMainWindow(this);
	if (requiresGUI && view!=NULL)
	{
		actionManager->disconnectNewViewActions();
		disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	}
	ScribusView* tempView = new ScribusView(w, this, tempDoc);
	if (requiresGUI)
		view = tempView;
	tempDoc->setCurrentPage(tempDoc->Pages->at(0));
	tempDoc->setGUI(requiresGUI, this, tempView);
	tempDoc->setLoading(false);
	//run after setGUI to set up guidepalette ok
	
	tempView->setScale(prefsManager->displayScale());
	if (requiresGUI)
	{
		actionManager->connectNewViewActions(tempView);
		alignDistributePalette->setDoc(tempDoc);
		docCheckerPalette->clearErrorList();
	}
	w->setView(tempView);
	ActWin = w;
	tempDoc->WinHan = w;
	w->setCentralWidget(tempView);
	if (requiresGUI)
		tempDoc->connectDocSignals(); //Must be before the first reformpages
	tempView->reformPages(true);
	//>>
	if (requiresGUI)
	{
		connect(undoManager, SIGNAL(undoRedoDone()), tempView, SLOT(DrawNew()));
		//connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
		connect(tempView, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
		//	connect(w, SIGNAL(SaveAndClose()), this, SLOT(DoSaveClose()));
	}
	//Independent finishing tasks after tempDoc setup
	if (showView)
	{
		if ( wsp->windowList().isEmpty() )
			w->showMaximized();
		else
			w->show();
		tempView->show();
	}
	if (requiresGUI)
	{
		connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
		connect(ScCore->fileWatcher, SIGNAL(fileChanged(QString)), tempDoc, SLOT(updatePict(QString)));
		connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString)), tempDoc, SLOT(removePict(QString)));
		//scrActions["fileSave"]->setEnabled(false);
		tempView->cmsToolbarButton->setOn(tempDoc->HasCMS);
		undoManager->switchStack(tempDoc->DocName);
		styleManager->currentDoc(tempDoc);
		tocGenerator->setDoc(tempDoc);
	}
	undoManager->setUndoEnabled(true);
	return tempDoc;
}

void ScribusMainWindow::newView()
{
	ScribusWin* w = new ScribusWin(wsp, doc);
	w->setMainWindow(this);
	view = new ScribusView(w, this, doc);
	view->setScale(prefsManager->displayScale());
	w->setView(view);
	ActWin = w;
	w->setCentralWidget(view);
	actionManager->connectNewViewActions(view);
	alignDistributePalette->setDoc(doc);
	connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	view->show();
	//connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
}

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
	if (doc!=0 && doc->EditClip)
		ToggleFrameEdit();
	ScribusWin* scw = (ScribusWin*)w;
	if (scw && scw->doc())
		if (!scw->doc()->hasGUI())
			return;
	ActWin = scw;
	if (ActWin->doc()==NULL)
		return;

/*	if (doc != NULL)
	{
		if ((HaveDoc) && (doc != ActWin->doc))
			doc->OpenNodes = outlinePalette->buildReopenVals();
	} */
	docCheckerPalette->clearErrorList();

	doc = ActWin->doc();
	undoManager->switchStack(doc->DocName);

	if (view!=NULL)
	{
		actionManager->disconnectNewViewActions();
		disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
		if (ScCore->usingGUI())
		{
			disconnect(doc->m_Selection, SIGNAL(selectionIsMultiple(bool)), 0, 0);
			//disconnect(doc->m_Selection, SIGNAL(empty()), 0, 0);
		}
	}

	view = ActWin->view();
	actionManager->connectNewViewActions(view);
	actionManager->disconnectNewDocActions();
	actionManager->connectNewDocActions(doc);
	styleManager->currentDoc(doc);
	connect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	if (ScCore->usingGUI())
	{
		connect(doc->m_Selection, SIGNAL(selectionIsMultiple(bool)), propertiesPalette, SLOT( setMultipleSelection(bool)));
		//connect(doc->m_Selection, SIGNAL(empty()), propertiesPalette, SLOT( unsetItem()));
	}

	pagePalette->setView(view);
	alignDistributePalette->setDoc(doc);
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
	scrActions["viewShowLayerMarkers"]->setOn(doc->guidesSettings.layerMarkersShown);
	scrActions["viewShowGrid"]->setOn(doc->guidesSettings.gridShown);
	scrActions["viewShowGuides"]->setOn(doc->guidesSettings.guidesShown);
	scrActions["viewShowColumnBorders"]->setOn(doc->guidesSettings.colBordersShown);
	scrActions["viewShowBaseline"]->setOn(doc->guidesSettings.baseShown);
	scrActions["viewShowImages"]->setOn(doc->guidesSettings.showPic);
	scrActions["viewShowTextChain"]->setOn(doc->guidesSettings.linkShown);
	scrActions["viewShowTextControls"]->setOn(doc->guidesSettings.showControls);
	scrActions["viewShowRulers"]->setOn(doc->guidesSettings.rulersShown);
	scrActions["viewRulerMode"]->setOn(doc->guidesSettings.rulerMode);
	scrActions["extrasGenerateTableOfContents"]->setEnabled(doc->hasTOCSetup());
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
		if (doc->m_Selection->count() != 0)
		{
			HaveNewSel(doc->m_Selection->itemAt(0)->itemType());
			doc->m_Selection->itemAt(0)->emitAllToGUI();
		}
		else
			HaveNewSel(-1);
	}
	tocGenerator->setDoc(doc);
	styleManager->currentDoc(doc);
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

void ScribusMainWindow::docSetup(ReformDoc* dia)
{
	slotChangeUnit(dia->getSelectedUnit(), false);
	dia->updateDocumentSettings();
	if (dia->imageResolutionChanged())
	{
		setStatusBarInfoText( tr("Updating Pictures"));
		mainWindowProgressBar->reset();
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		qApp->processEvents();
		doc->recalcPicturesRes();
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		setStatusBarInfoText("");
		mainWindowProgressBar->reset();
	}
	FontSub->RebuildList(doc);
	propertiesPalette->Fonts->RebuildList(doc);
	scrActions["viewShowMargins"]->setOn(doc->guidesSettings.marginsShown);
	scrActions["viewShowFrames"]->setOn(doc->guidesSettings.framesShown);
	scrActions["viewShowLayerMarkers"]->setOn(doc->guidesSettings.layerMarkersShown);
	scrActions["viewShowGrid"]->setOn(doc->guidesSettings.gridShown);
	scrActions["viewShowGuides"]->setOn(doc->guidesSettings.guidesShown);
	scrActions["viewShowColumnBorders"]->setOn(doc->guidesSettings.colBordersShown);
	scrActions["viewShowBaseline"]->setOn(doc->guidesSettings.baseShown);
	scrActions["viewShowImages"]->setOn(doc->guidesSettings.showPic);
	scrActions["viewShowTextChain"]->setOn(doc->guidesSettings.linkShown);
	scrActions["viewShowTextControls"]->setOn(doc->guidesSettings.showControls);
	scrActions["viewShowRulers"]->setOn(doc->guidesSettings.rulersShown);
	scrActions["viewRulerMode"]->setOn(doc->guidesSettings.rulerMode);
	scrActions["extrasGenerateTableOfContents"]->setEnabled(doc->hasTOCSetup());
	view->cmsToolbarButton->setOn(doc->HasCMS);
	view->reformPages();
	view->GotoPage(doc->currentPage()->pageNr());
	view->DrawNew();
	propertiesPalette->ShowCMS();
	pagePalette->RebuildPage();
	slotDocCh();
}

bool ScribusMainWindow::slotDocSetup()
{
	bool ret = false;
	ReformDoc* dia = new ReformDoc(this, doc);
	Q_CHECK_PTR(dia);
	if (dia->exec())
	{
		docSetup(dia);
		ret = true;
	}
	delete dia;
	return ret;
}

void ScribusMainWindow::SwitchWin()
{
	updateColorMenu();
	buildFontMenu();
	propertiesPalette->updateColorList();
	propertiesPalette->Cpal->ChooseGrad(0);
	updateActiveWindowCaption(doc->DocName);
	scrActions["shade100"]->setOn(true);
	propertiesPalette->setDoc(doc);
	propertiesPalette->updateCList();
	pagePalette->setView(view);
	propertiesPalette->Spal->setFormats(doc);
	propertiesPalette->SetLineFormats(doc);
	propertiesPalette->startArrow->rebuildList(&doc->arrowStyles);
	propertiesPalette->endArrow->rebuildList(&doc->arrowStyles);
	FontSub->RebuildList(doc);
	propertiesPalette->Fonts->RebuildList(doc);
	layerPalette->setDoc(doc);
	guidePalette->setDoc(doc);
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	//Do not set this!, it doesnt get valid pointers unless its in EditClip mode and its not
	//if we are switching windows #4357
	//nodePalette->setDoc(doc, view);
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
		scrActions["fileSave"]->setEnabled(true);
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
		scrActions["fileSave"]->setEnabled(true);
		//CB TODO  Huh? Why different to the above?, fileMenu->setItemEnabled(M_FileSave, ActWin->MenuStat[2]);
		scrActions["fileSaveAs"]->setEnabled(ActWin->menuStatus(3));
		scrActions["fileCollect"]->setEnabled(ActWin->menuStatus(3));
		scrActions["fileRevert"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("FileOpenRecent", true);

		if (doc->isModified())
			slotDocCh(false);
		else
		{
			bool setter = doc->Pages->count() > 1 ? true : false;
			scrActions["pageDelete"]->setEnabled(setter);
			scrActions["pageMove"]->setEnabled(setter);
		}
		scrActions["fileSaveAs"]->setEnabled(true);
		scrActions["fileCollect"]->setEnabled(true);
		pagePalette->enablePalette(true);
	}
}

void ScribusMainWindow::HaveNewDoc()
{
	scrActions["filePrint"]->setEnabled(true);
 	scrActions["fileSave"]->setEnabled(true);
	scrActions["fileClose"]->setEnabled(true);
	scrActions["fileDocSetup"]->setEnabled(true);
	scrActions["fileRevert"]->setEnabled(false);
	scrActions["fileCollect"]->setEnabled(true);
	scrActions["fileSaveAs"]->setEnabled(true);
	scrMenuMgr->setMenuEnabled("FileImport", true);
	scrMenuMgr->setMenuEnabled("FileExport", true);
	scrActions["fileExportAsEPS"]->setEnabled(true);
	scrActions["fileExportAsPDF"]->setEnabled(true);
	scrActions["pageImport"]->setEnabled(true);
	//scrActions["toolsPreflightVerifier"]->setEnabled(true);

	if ( ScCore->haveGS()==0 || ScCore->isWinGUI() )
		scrActions["PrintPreview"]->setEnabled(true);

	if (scrActions["SaveAsDocumentTemplate"])
		scrActions["SaveAsDocumentTemplate"]->setEnabled(true);

	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	scrActions["editPaste"]->setEnabled((!Buffer2.isEmpty()) || (scrapbookPalette->tempBView->objectMap.count() > 0));
	scrMenuMgr->setMenuEnabled("EditPasteRecent", scrapbookPalette->tempBView->objectMap.count() > 0);
	scrActions["editCopyContents"]->setEnabled(false);
	scrActions["editPasteContents"]->setEnabled(false);
	scrActions["editPasteContentsAbs"]->setEnabled(false);
	scrActions["editSelectAll"]->setEnabled(true);
	scrActions["editDeselectAll"]->setEnabled(false);
	scrActions["editPatterns"]->setEnabled(true);
	scrActions["editParaStyles"]->setEnabled(true);
	scrActions["editLineStyles"]->setEnabled(true);
	scrActions["editMasterPages"]->setEnabled(true);
	scrActions["editJavascripts"]->setEnabled(true);

	scrMenuMgr->setMenuEnabled("View", true);
	scrActions["viewSnapToGrid"]->setOn(doc->useRaster);
	scrActions["viewSnapToGuides"]->setOn(doc->SnapGuides);
	scrActions["viewShowRulers"]->setEnabled(true);

	scrMenuMgr->setMenuEnabled("Insert", true);
	scrActions["insertFrame"]->setEnabled(true);
	//scrMenuMgr->setMenuEnabled("Windows", true);
	scrMenuMgr->setMenuEnabled("Page", true);
	scrMenuMgr->setMenuEnabled("Extras", true);

	scrActions["toolsSelect"]->setEnabled(true);
	scrActions["toolsZoom"]->setEnabled(true);
	scrActions["toolsInsertTextFrame"]->setEnabled(true);
	scrActions["toolsInsertImageFrame"]->setEnabled(true);
	scrActions["toolsInsertTableFrame"]->setEnabled(true);
	scrActions["toolsInsertShape"]->setEnabled(true);
	scrActions["toolsInsertLine"]->setEnabled(true);
	scrActions["toolsInsertBezier"]->setEnabled(true);
	scrActions["toolsInsertFreehandLine"]->setEnabled(true);
	scrActions["toolsInsertPolygon"]->setEnabled(true);
 	scrActions["toolsMeasurements"]->setEnabled(true);
	scrActions["toolsEyeDropper"]->setEnabled(true);
	scrActions["toolsPDFPushButton"]->setEnabled(true);
	scrActions["toolsPDFTextField"]->setEnabled(true);
	scrActions["toolsPDFCheckBox"]->setEnabled(true);
	scrActions["toolsPDFComboBox"]->setEnabled(true);
	scrActions["toolsPDFListBox"]->setEnabled(true);
	scrActions["toolsPDFAnnotText"]->setEnabled(true);
	scrActions["toolsPDFAnnotLink"]->setEnabled(true);

	bool setter = doc->Pages->count() > 1 ? true : false;
	scrActions["pageDelete"]->setEnabled(setter);
	scrActions["pageMove"]->setEnabled(setter);

	updateColorMenu();
	//Update palettes
	propertiesPalette->updateColorList();
	propertiesPalette->Cpal->ChooseGrad(0);
	updateActiveWindowCaption(doc->DocName);
	scrActions["shade100"]->setOn(true);
	propertiesPalette->setDoc(doc);
	propertiesPalette->updateCList();
	pagePalette->setView(view);
	propertiesPalette->Spal->setFormats(doc);
	propertiesPalette->SetLineFormats(doc);
	propertiesPalette->startArrow->rebuildList(&doc->arrowStyles);
	propertiesPalette->endArrow->rebuildList(&doc->arrowStyles);
	layerPalette->setDoc(doc);
	guidePalette->setDoc(doc);
	outlinePalette->setDoc(doc);
	outlinePalette->BuildTree();
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	slotChangeUnit(doc->unitIndex());
	buildFontMenu();
	windowsMenuAboutToShow();
	connect(view, SIGNAL(changeUN(int)), this, SLOT(slotChangeUnit(int)));
	connect(view, SIGNAL(changeLA(int)), layerPalette, SLOT(markActiveLayer(int)));
	connect(view->horizRuler, SIGNAL(MarkerMoved(double, double)), this, SLOT(setStatusBarMousePosition(double, double)));
	connect(view->horizRuler, SIGNAL(DocChanged(bool)), this, SLOT(slotDocCh(bool)));
	connect(view, SIGNAL(ClipPo(double, double)), nodePalette, SLOT(SetXY(double, double)));
	connect(view, SIGNAL(PolyOpen()), nodePalette, SLOT(IsOpen()));
	connect(view, SIGNAL(PStatus(int, uint)), nodePalette, SLOT(PolyStatus(int, uint)));
	connect(view, SIGNAL(ItemPos(double, double)), propertiesPalette, SLOT(setXY(double, double)));
	connect(view, SIGNAL(ItemGeom(double, double)), propertiesPalette, SLOT(setBH(double, double)));
	connect(view, SIGNAL(ChBMText(PageItem *)), this, SLOT(BookMarkTxT(PageItem *)));
	connect(view, SIGNAL(HaveSel(int)), this, SLOT(HaveNewSel(int)));
	connect(view, SIGNAL(SetAngle(double)), propertiesPalette, SLOT(setR(double)));
	connect(view, SIGNAL(SetSizeValue(double)), propertiesPalette, SLOT(setSvalue(double)));
	connect(view, SIGNAL(SetLocalValues(double, double, double, double)), propertiesPalette, SLOT(setLvalue(double, double, double, double)));
	connect(view, SIGNAL(SetLineArt(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle)), propertiesPalette, SLOT( setLIvalue(Qt::PenStyle, Qt::PenCapStyle, Qt::PenJoinStyle)));
	connect(view, SIGNAL(ItemFarben(QString, QString, int, int)), this, SLOT(setCSMenu(QString, QString, int, int)));
	connect(view, SIGNAL(ItemFarben(QString, QString, int, int)), propertiesPalette->Cpal, SLOT(setActFarben(QString, QString, int, int)));
	connect(view, SIGNAL(ItemGradient(int)), propertiesPalette->Cpal, SLOT(setActGradient(int)));
	connect(view, SIGNAL(ItemTrans(double, double)), propertiesPalette->Cpal, SLOT(setActTrans(double, double)));
	connect(view, SIGNAL(ItemBlend(int, int)), propertiesPalette->Cpal, SLOT(setActBlend(int, int)));
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
	connect(view, SIGNAL(MousePos(double, double)), this, SLOT(setStatusBarMousePosition(double, double)));
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
	//connect(view, SIGNAL(EditGuides()), this, SLOT(ManageGuides()));
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
		if (doc->m_Selection->count() != 0)
		{
			uint lowestItem = 999999;
			for (uint a=0; a<doc->m_Selection->count(); ++a)
			{
				currItem = doc->m_Selection->itemAt(a);
				lowestItem = QMIN(lowestItem, currItem->ItemNr);
			}
			currItem = doc->Items->at(lowestItem);
//			doc->m_Selection->removeItem(currItem);
//			doc->m_Selection->prependItem(currItem);
//			currItem = doc->m_Selection->itemAt(0);
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
	scrActions["editPasteContentsAbs"]->setEnabled(Nr==PageItem::ImageFrame);
	scrActions["editEditWithImageEditor"]->setEnabled(Nr==PageItem::ImageFrame && currItem->PicAvail && currItem->isRaster);
	if (Nr!=PageItem::ImageFrame)
	{
		scrActions["itemImageIsVisible"]->setOn(false);
		scrActions["itemPreviewLow"]->setOn(false);
		scrActions["itemPreviewNormal"]->setOn(false);
		scrActions["itemPreviewFull"]->setOn(false);
	}
	if ((Nr==-1) || (Nr!=-1 && !currItem->asTextFrame()))
		actionManager->enableUnicodeActions(&scrActions, false);
	scrActions["insertSampleText"]->setEnabled(false);

	view->horizRuler->ItemPosValid = false;
	view->horizRuler->repaint();
	switch (Nr)
	{
	case -1: // None
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportText2"]->setEnabled(false);
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
		scrActions["itemPrintingEnabled"]->setEnabled(false);
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
		propertiesPalette->setGradientEditMode(false);
		break;
	case PageItem::ImageFrame: //Image Frame
		scrActions["fileImportAppendText"]->setEnabled(false);
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportText2"]->setEnabled(false);
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
		scrActions["fileImportText2"]->setEnabled(true);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(true);
		scrActions["fileExportText"]->setEnabled(true);
		scrActions["editCut"]->setEnabled(true);
		scrActions["editCopy"]->setEnabled(true);
		scrActions["editClearContents"]->setEnabled(true);
		scrActions["editSearchReplace"]->setEnabled(currItem->itemText.length() != 0);
		scrActions["extrasHyphenateText"]->setEnabled(currItem->itemText.length() != 0);
		scrActions["extrasDeHyphenateText"]->setEnabled(currItem->itemText.length() != 0);
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
			scrActions["itemConvertToBezierCurve"]->setEnabled(false);
			scrActions["itemConvertToImageFrame"]->setEnabled(false);
//			scrActions["itemConvertToOutlines"]->setEnabled(false);
			scrActions["itemConvertToPolygon"]->setEnabled(false);
			scrActions["itemConvertToTextFrame"]->setEnabled(false);
			scrActions["toolsUnlinkTextFrame"]->setEnabled(true);
			// FIXME: once there's one itemtext per story, always enable editcontents
			if ((currItem->BackBox != 0) && (currItem->itemText.length() == 0))
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
				actionManager->enableUnicodeActions(&scrActions, true, doc->currentStyle.charStyle().font().scName());
			view->horizRuler->setItem(currItem);
			if (currItem->lineColor() != CommonStrings::None)
				view->horizRuler->lineCorr = currItem->lineWidth() / 2.0;
			else
				view->horizRuler->lineCorr = 0;
			view->horizRuler->ColGap = currItem->ColGap;
			view->horizRuler->Cols = currItem->Cols;
			view->horizRuler->Extra = currItem->textToFrameDistLeft();
			view->horizRuler->RExtra = currItem->textToFrameDistRight();
			view->horizRuler->First = currItem->currentStyle().firstIndent();
			view->horizRuler->Indent = currItem->currentStyle().leftMargin();
			double columnWidth = (currItem->width() - (currItem->columnGap() * (currItem->columns() - 1)) 
				- currItem->textToFrameDistLeft() - currItem->textToFrameDistLeft() 
				- 2*view->horizRuler->lineCorr) / currItem->columns();
			view->horizRuler->RMargin = columnWidth - currItem->currentStyle().rightMargin();
			if (currItem->imageFlippedH() || (currItem->reversed()))
				view->horizRuler->Revers = true;
			else
				view->horizRuler->Revers = false;
			view->horizRuler->ItemPosValid = true;
			if (findParagraphStyle(doc, doc->currentStyle) < 5)
				view->horizRuler->TabValues = currItem->TabValues;
			else
				view->horizRuler->TabValues = doc->currentStyle.tabValues();
			view->horizRuler->repaint();
		}
		else
		{
			doc->currentStyle.charStyle() = currItem->itemText.defaultStyle().charStyle();
			emit TextStrike(doc->currentStyle.charStyle().strikethruOffset(), doc->currentStyle.charStyle().strikethruWidth());
			emit TextUnderline(doc->currentStyle.charStyle().underlineOffset(), doc->currentStyle.charStyle().underlineWidth());
			emit TextShadow(doc->currentStyle.charStyle().shadowXOffset(), doc->currentStyle.charStyle().shadowYOffset());
			emit TextFarben(doc->currentStyle.charStyle().strokeColor(), doc->currentStyle.charStyle().fillColor(), doc->currentStyle.charStyle().strokeShade(), doc->currentStyle.charStyle().fillShade());
			emit TextScale(doc->currentStyle.charStyle().scaleH());
			emit TextScaleV(doc->currentStyle.charStyle().scaleV());
			emit TextBase(doc->currentStyle.charStyle().baselineOffset());
			emit TextOutline(doc->currentStyle.charStyle().outlineWidth());
//			emit TextStil(doc->currentStyle.charStyle().effects());
			setStilvalue(doc->currentStyle.charStyle().effects());
		}

//		doc->docParagraphStyles[0].setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(currItem->lineSpacingMode()));
//		doc->docParagraphStyles[0].setLineSpacing(currItem->lineSpacing());
//		doc->docParagraphStyles[0].setAlignment(currItem->textAlignment);

		break;
	case PageItem::PathText: //Path Text
		scrActions["fileImportText"]->setEnabled(true);
		scrActions["fileImportText2"]->setEnabled(true);
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
			doc->currentStyle.charStyle() = currItem->itemText.defaultStyle().charStyle();
			emit TextStrike(doc->currentStyle.charStyle().strikethruOffset(), doc->currentStyle.charStyle().strikethruWidth());
			emit TextUnderline(doc->currentStyle.charStyle().underlineOffset(), doc->currentStyle.charStyle().underlineWidth());
			emit TextShadow(doc->currentStyle.charStyle().shadowXOffset(), doc->currentStyle.charStyle().shadowYOffset());
			emit TextFarben(doc->currentStyle.charStyle().strokeColor(), doc->currentStyle.charStyle().fillColor(), doc->currentStyle.charStyle().strokeShade(), doc->currentStyle.charStyle().fillShade());
			emit TextScale(doc->currentStyle.charStyle().scaleH());
			emit TextScaleV(doc->currentStyle.charStyle().scaleV());
			emit TextBase(doc->currentStyle.charStyle().baselineOffset());
			emit TextOutline(doc->currentStyle.charStyle().outlineWidth());
//			emit TextStil(doc->currentStyle.charStyle().effects());
			setStilvalue(doc->currentStyle.charStyle().effects());
		}
		break;
	default:
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportText2"]->setEnabled(false);
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
//		if (Nr != 5)
			scrActions["toolsRotate"]->setEnabled(true);
//		else
//			scrActions["toolsRotate"]->setEnabled(false);
		scrActions["toolsCopyProperties"]->setEnabled(true);
		break;
	}
	doc->CurrentSel = Nr;
	rebuildStyleMenu(Nr);
	propertiesPalette->RotationGroup->setButton(doc->RotMode);
	uint docSelectionCount=doc->m_Selection->count();
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
			PageItem* bxi=doc->m_Selection->itemAt(bx);
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
			PageItem* bx=doc->m_Selection->itemAt(1);
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
		//propertiesPalette->textFlowsAroundFrame->setChecked(currItem->textFlowsAroundFrame());
		propertiesPalette->setTextFlowMode(currItem->textFlowMode());
		scrActions["itemLock"]->setEnabled(true);
		scrActions["itemLockSize"]->setEnabled(true);
		scrActions["itemPrintingEnabled"]->setEnabled(true);
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
			scrActions["itemSendToScrapbook"]->setEnabled(!(currItem->isTableItem && currItem->isSingleSel));
			scrActions["itemSendToPattern"]->setEnabled(!(currItem->isTableItem && currItem->isSingleSel));
			scrActions["editCut"]->setEnabled(false);
			scrActions["editClearContents"]->setEnabled(false);
			scrActions["toolsRotate"]->setEnabled(false);
		}
		else
		{
			bool setter=!(currItem->isTableItem && currItem->isSingleSel && currItem->isGroupControl);
			scrActions["itemDuplicate"]->setEnabled(setter);
			scrActions["itemMulDuplicate"]->setEnabled(setter);
			scrActions["itemDelete"]->setEnabled(!currItem->isSingleSel);
			scrActions["itemLowerToBottom"]->setEnabled(setter);
			scrActions["itemRaiseToTop"]->setEnabled(setter);
			scrActions["itemRaise"]->setEnabled(setter);
			scrActions["itemLower"]->setEnabled(setter);
			scrActions["itemSendToScrapbook"]->setEnabled(setter);
			scrActions["itemSendToPattern"]->setEnabled(setter);
		}
		scrActions["itemLock"]->setOn(currItem->locked());
		scrActions["itemLockSize"]->setOn(currItem->sizeLocked());
		scrActions["itemPrintingEnabled"]->setOn(currItem->printEnabled());
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
		actionManager->connectNewSelectionActions(view, doc);
// 		propertiesPalette->NewSel(Nr);
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
		if (doc->m_Selection->count() == 0)
			iT = -1;
		else
		{
			PageItem *currItem = doc->m_Selection->itemAt(0);
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
	updateActiveWindowCaption(doc->DocName + "*");
// 	scrActions["fileSave"]->setEnabled(true);
// 	scrActions["fileSaveAs"]->setEnabled(true);
	scrActions["fileCollect"]->setEnabled(true);
	if (!doc->masterPageMode())
	{
		scrActions["fileClose"]->setEnabled(true);
		if (doc->hasName)
			scrActions["fileRevert"]->setEnabled(true);
	
		bool setter = doc->Pages->count() > 1 ? true : false;
		scrActions["pageDelete"]->setEnabled(setter);
		scrActions["pageMove"]->setEnabled(setter);
	}

	ActWin->setMenuStatus(0, scrActions["fileSave"]->isEnabled());
	ActWin->setMenuStatus(1, scrActions["fileClose"]->isEnabled());
	ActWin->setMenuStatus(2, scrActions["fileSave"]->isEnabled());
	ActWin->setMenuStatus(3, scrActions["fileSaveAs"]->isEnabled());
	
	outlinePalette->BuildTree();
}

void ScribusMainWindow::updateRecent(QString fn)
{
	if (RecentDocs.findIndex(fn) == -1)
	{
		RecentDocs.prepend(fn);
		ScCore->fileWatcher->addFile(fn);
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
		if (!ScCore->fileWatcher->isActive())
			ScCore->fileWatcher->removeFile(fn);
	}
	rebuildRecentFileMenu();
}

void ScribusMainWindow::loadRecent(QString fn)
{
	QFileInfo fd(fn);
	if (!fd.exists())
	{
		//CB Why not just removeRecent()?
		RecentDocs.remove(fn);
		ScCore->fileWatcher->removeFile(fn);
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

void ScribusMainWindow::rebuildRecentPasteMenu()
{
	for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrRecentPasteActions.begin(); it!=scrRecentPasteActions.end(); ++it )
		scrMenuMgr->removeMenuItem((*it), recentPasteMenuName);

	scrRecentPasteActions.clear();
	uint max = QMIN(static_cast<uint>(prefsManager->appPrefs.numScrapbookCopies), scrapbookPalette->tempBView->objectMap.count());
	if (max > 0)
	{
		QMap<QString,BibView::Elem>::Iterator it;
		it = scrapbookPalette->tempBView->objectMap.end();
		it--;
		for (uint m = 0; m < max; ++m)
		{
			QString strippedName = it.key();
			QPixmap pm = it.data().Preview;
			scrRecentPasteActions.insert(strippedName, new ScrAction(ScrAction::RecentPaste, QIconSet(pm), QString("&%1 %2").arg(m+1).arg(strippedName), QKeySequence(), this, strippedName,0,0.0,it.key()));
			connect( scrRecentPasteActions[strippedName], SIGNAL(activatedData(QString)), this, SLOT(pasteRecent(QString)) );
			scrMenuMgr->addMenuItem(scrRecentPasteActions[strippedName], recentPasteMenuName);
			it--;
		}
	}
}

void ScribusMainWindow::pasteRecent(QString fn)
{
	QString data = scrapbookPalette->tempBView->objectMap[fn].Data;
	QFileInfo fi(data);
	if (fi.extension(true).lower() == "sml")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createObjects(f);
		delete pre;
	}
	else if (fi.extension(true).lower() == "shape")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createShape(f);
		delete pre;
	}
	else if (fi.extension(true).lower() == "sce")
	{
		QString f = "";
		loadText(data, &f);
		data = f;
	}
	view->Deselect(true);
	uint ac = doc->Items->count();
	bool savedAlignGrid = doc->useRaster;
	bool savedAlignGuides = doc->SnapGuides;
	doc->useRaster = false;
	doc->SnapGuides = false;
	slotElemRead(data, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
	doc->useRaster = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	for (uint as = ac; as < doc->Items->count(); ++as)
	{
		PageItem* currItem = doc->Items->at(as);
		if (currItem->isBookmark)
			AddBookMark(currItem);
		view->SelectItemNr(as);
	}
		slotDocCh(false);
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
			connect( (*it), SIGNAL(activatedData(int)), doc, SLOT(itemSelection_SendToLayer(int)) );
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
		if (doc->m_Selection->count()>0 && doc->m_Selection->itemAt(0))
			scrLayersActions[QString("%1").arg(doc->m_Selection->itemAt(0)->LayerNr)]->setOn(true);
		for( QMap<QString, QGuardedPtr<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=itend; ++it )
			connect( (*it), SIGNAL(activatedData(int)), doc, SLOT(itemSelection_SendToLayer(int)) );
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
	formats.remove("PDF (*.pdf *.PDF);;");
	QString fileName = CFileDialog( docDir, tr("Open"), formats);
	if (fileName.isEmpty())
		// User cancelled
		return false;
	docContext->set("docsopen", fileName.left(fileName.findRev("/")));
	return loadDoc(fileName);
}

bool ScribusMainWindow::slotPageImport()
{
	Q_ASSERT(!doc->masterPageMode());
	bool ret = false;
	MergeDoc *dia = new MergeDoc(this, false, doc->DocPages.count(), doc->currentPage()->pageNr() + 1);
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
			addNewPages(dia->getImportWherePage(), importWhere, pageNs.size(), doc->pageHeight, doc->pageWidth, doc->PageOri, doc->m_pageSize, true);
			nrToImport = pageNs.size();
		}
		else
		{
			startPage = doc->currentPage()->pageNr() + 1;
			nrToImport = pageNs.size();
			if (pageNs.size() > (doc->DocPages.count() - doc->currentPage()->pageNr()))
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
						addNewPages(doc->DocPages.count(), 2,
									pageNs.size() - (doc->DocPages.count() - doc->currentPage()->pageNr()),
									doc->pageHeight, doc->pageWidth, doc->PageOri, doc->m_pageSize, true);
						break;
					case 1:
						nrToImport = doc->DocPages.count() - doc->currentPage()->pageNr();
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

bool ScribusMainWindow::loadPage(QString fileName, int Nr, bool Mpa, const QString& renamedPageName)
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
		if(!fl->LoadPage(doc, Nr, Mpa, renamedPageName))
		{
			delete fl;
			doc->setLoading(false);
			return false;
		}
		delete fl;
		if (ScCore->haveCMS() && doc->CMSSettings.CMSinUse)
		{
			recalcColors();
			doc->RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK);
		}
		uint docItemsCount=doc->Items->count();
		for (uint i = oldItemsCount; i < docItemsCount; ++i)
		{
			PageItem *ite = doc->Items->at(i);
			if ((docItemsCount - oldItemsCount) > 1)
				ite->Groups.push(doc->GroupCounter);
			if (ite->locked())
				ite->setLocked(false);
			if ((ite->asTextFrame()) && (ite->isBookmark))
				bookmarkPalette->BView->AddPageItem(ite);
		}
		if ((docItemsCount - oldItemsCount) > 1)
			doc->GroupCounter++;
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
		int testResult=fileLoader->TestFile();
		if (testResult == -1)
		{
			delete fileLoader;
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			QMessageBox::critical(this, tr("Fatal Error"), "<qt>"+tr("File %1 is not in an acceptable format").arg(FName)+"</qt>", CommonStrings::tr_OK);
			return false;
		}
		bool is12doc=false;
		if (testResult == 0)
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			//Scribus 1.3.x warning, remove at a later stage
			is12doc=true;
		}

		ScCore->getCMSProfilesDir(fi.dirPath(true)+"/", false);
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
		w->setMainWindow(this);
		view = new ScribusView(w, this, doc);
		doc->setGUI(true, this, view);
		view->setScale(prefsManager->displayScale());
		w->setView(view);
		alignDistributePalette->setDoc(doc);
		ActWin = w;
		doc->WinHan = w;
		w->setCentralWidget(view);
		w->setUpdatesEnabled(false);
		view->setUpdatesEnabled(false);
		doc->SoftProofing = false;
		doc->Gamut = false;
		ScriptRunning = true;
		bool loadSuccess=fileLoader->LoadFile(doc);
		//Do the font replacement check from here, when we have a GUI. TODO do this also somehow without the GUI
		//This also gives the user the opportunity to cancel the load when finding theres a replacement required.
		if (loadSuccess && ScCore->usingGUI())
			loadSuccess=fileLoader->postLoad(doc);
		if(!loadSuccess)
		{
			view->close();
			delete fileLoader;
			delete view;
			delete doc;
			delete w;
			view=NULL;
			doc=NULL;
			ScriptRunning = false;
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			mainWindowStatusLabel->setText("");
			mainWindowProgressBar->reset();
			ActWin = NULL;
			undoManager->setUndoEnabled(true);
			if (windows.count() != 0)
				newActWin(ActWinOld);
			return false;
		}
		fileLoader->informReplacementFonts();
		view->unitSwitcher->setCurrentText(unitGetStrFromIndex(doc->unitIndex()));
		view->unitChange();
		ScriptRunning = false;
		view->Deselect(true);
		mainWindowStatusLabel->setText("");
		mainWindowProgressBar->reset();
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
		if (!doc->CMSSettings.CMSinUse)
			doc->HasCMS = false;
		if ((ScCore->haveCMS()) && (doc->CMSSettings.CMSinUse))
		{
			bool cmsWarning = false;
			QStringList missing;
			QStringList replacement;
			if (!ScCore->InputProfiles.contains(doc->CMSSettings.DefaultImageRGBProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultImageRGBProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile);
				doc->CMSSettings.DefaultImageRGBProfile = prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile;
			}
			if (!ScCore->InputProfilesCMYK.contains(doc->CMSSettings.DefaultImageCMYKProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultImageCMYKProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile);
				doc->CMSSettings.DefaultImageCMYKProfile = prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile;
			}
			if (!ScCore->InputProfiles.contains(doc->CMSSettings.DefaultSolidColorRGBProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultSolidColorRGBProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile);
				doc->CMSSettings.DefaultSolidColorRGBProfile = prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile;
			}
			if (!ScCore->InputProfilesCMYK.contains(doc->CMSSettings.DefaultSolidColorCMYKProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultSolidColorCMYKProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultSolidColorCMYKProfile);
				doc->CMSSettings.DefaultSolidColorCMYKProfile = prefsManager->appPrefs.DCMSset.DefaultSolidColorCMYKProfile;
			}
			if (!ScCore->MonitorProfiles.contains(doc->CMSSettings.DefaultMonitorProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultMonitorProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultMonitorProfile);
				doc->CMSSettings.DefaultMonitorProfile = prefsManager->appPrefs.DCMSset.DefaultMonitorProfile;
			}
			if (!ScCore->PrinterProfiles.contains(doc->CMSSettings.DefaultPrinterProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultPrinterProfile);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultPrinterProfile);
				doc->CMSSettings.DefaultPrinterProfile = prefsManager->appPrefs.DCMSset.DefaultPrinterProfile;
			}
			if (!ScCore->PrinterProfiles.contains(doc->PDF_Options.PrintProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Options.PrintProf);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultPrinterProfile);
				doc->PDF_Options.PrintProf = doc->CMSSettings.DefaultPrinterProfile;
			}
			if (!ScCore->InputProfiles.contains(doc->PDF_Options.ImageProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Options.ImageProf);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile);
				doc->PDF_Options.ImageProf = doc->CMSSettings.DefaultImageRGBProfile;
			}
			if (!ScCore->InputProfiles.contains(doc->PDF_Options.SolidProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Options.SolidProf);
				replacement.append(prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile);
				doc->PDF_Options.SolidProf = doc->CMSSettings.DefaultSolidColorRGBProfile;
			}
			if (cmsWarning)
			{
				qApp->setOverrideCursor(QCursor(arrowCursor), true);
				QString mess = tr("Some ICC profiles used by this document are not installed:")+"\n\n";
				for (uint m = 0; m < missing.count(); ++m)
				{
					mess += missing[m] + tr(" was replaced by: ")+replacement[m]+"\n";
				}
				QMessageBox::warning(this, CommonStrings::trWarning, mess, 1, 0, 0);
			}
			doc->SoftProofing = doc->CMSSettings.SoftProofOn;
			doc->Gamut = doc->CMSSettings.GamutCheck;
			doc->IntentColors = doc->CMSSettings.DefaultIntentColors;
			doc->IntentImages = doc->CMSSettings.DefaultIntentImages;
			if (doc->OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles))
			{
				doc->HasCMS = true;
				doc->PDF_Options.SComp = doc->CMSSettings.ComponentsInput2;
			}
			if (doc->HasCMS)
			{
				recalcColors();
				doc->RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK);
			}
		}
		else
		{
			doc->CMSSettings.CMSinUse = false;
		}
		propertiesPalette->updateColorList();
		propertiesPalette->Cpal->ChooseGrad(0);
		if (fileLoader->FileType > FORMATID_NATIVEIMPORTEND)
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
			doc->addMasterPage(0, CommonStrings::masterPageNormal);
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
			if (ite->asPathText())
			{
				ite->Frame = true;
				ite->updatePolyClip();
				ite->DrawObj(painter, rd);
			}
			else if (ite->asTextFrame())
			{
				if ( ite->BackBox == 0 )
					ite->asTextFrame()->layout();
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
			if (ite->itemType() == PageItem::PathText)
			{
				ite->Frame = true;
				ite->updatePolyClip();
				ite->DrawObj(painter, rd);
			}
			else if (ite->itemType() == PageItem::TextFrame)
			{
				if ( ite->BackBox == 0 )
					ite->asTextFrame()->layout();
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
			if ( ite->itemType() == PageItem::PathText )
			{
				ite->Frame = true;
				ite->updatePolyClip();
				ite->DrawObj(painter, rd);
			}
			else if ( ite->itemType() == PageItem::TextFrame )
			{
				ite->asTextFrame()->layout();
			}
		}
		painter->end();
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
/*		if (fileLoader->FileType > FORMATID_NATIVEIMPORTEND)
		{
			doc->hasName = false;
			slotFileSaveAs();
		} */
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
		doc->setCurrentPage(doc->DocPages.at(0));
		view->cmsToolbarButton->setOn(doc->HasCMS);
		view->slotDoZoom();
		view->GotoPage(0);
		connect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
		connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
		connect(ScCore->fileWatcher, SIGNAL(fileChanged(QString )), doc, SLOT(updatePict(QString)));
		connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), doc, SLOT(removePict(QString)));
		connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
		actionManager->disconnectNewDocActions();
		actionManager->connectNewDocActions(doc);
		doc->connectDocSignals();
		if (doc->AutoSave)
			doc->autoSaveTimer->start(doc->AutoSaveTime);
// 		scrActions["fileSave"]->setEnabled(false);
		doc->NrItems = bookmarkPalette->BView->NrItems;
		doc->First = bookmarkPalette->BView->First;
		doc->Last = bookmarkPalette->BView->Last;
	}
	else
	{
		pagePalette->setView(0);
	}
	undoManager->switchStack(doc->DocName);
	styleManager->currentDoc(doc);
	pagePalette->Rebuild();
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	undoManager->setUndoEnabled(true);
	doc->setModified(false);
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
	if (doc->m_Selection->count() != 0)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
		if (currItem->itemType() == PageItem::ImageFrame)
		{
			QString formatD(setupImageFormats());
			QString docDir = ".";
			QString prefsDocDir=prefsManager->documentDir();
			if (!prefsDocDir.isEmpty())
				docDir = prefsManager->prefsFile->getContext("dirs")->get("images", prefsDocDir);
			else
				docDir = prefsManager->prefsFile->getContext("dirs")->get("images", ".");
			QString fileName = CFileDialog( docDir, tr("Open"), formatD, "", fdShowPreview | fdExistingFiles);
			if (!fileName.isEmpty())
			{
				prefsManager->prefsFile->getContext("dirs")->set("images", fileName.left(fileName.findRev("/")));
				currItem->EmProfile = "";
				currItem->pixm.imgInfo.isRequest = false;
				currItem->UseEmbedded = true;
				currItem->IProfile = doc->CMSSettings.DefaultImageRGBProfile;
				currItem->IRender = doc->CMSSettings.DefaultIntentImages;
				qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				doc->LoadPict(fileName, currItem->ItemNr, false, true);
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
			gtGetText* gt = new gtGetText(doc);
			ImportSetup impsetup=gt->run();
			if (impsetup.runDialog)
			{
				gt->launchImporter(impsetup.importer, impsetup.filename, impsetup.textOnly, impsetup.encoding, false);
			}
			delete gt;
			if (doc->docHyphenator->AutoCheck)
				doc->docHyphenator->slotHyphenate(currItem);
			for (uint a = 0; a < doc->Items->count(); ++a)
			{
				if (doc->Items->at(a)->isBookmark)
					bookmarkPalette->BView->ChangeText(doc->Items->at(a));
			}
//			outlinePalette->BuildTree();
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusMainWindow::slotGetContent2() // kk2006
{
	if (doc->m_Selection->count() == 0)
		return; // nothing to do, no selection

	PageItem *currItem = doc->m_Selection->itemAt(0);

	if (!currItem->asTextFrame())
		return; // not a text frame

	ScGTPluginManager::instance()->run();
	if (doc->docHyphenator->AutoCheck)
		doc->docHyphenator->slotHyphenate(currItem);
	for (uint a = 0; a < doc->Items->count(); ++a)
	{
		if (doc->Items->at(a)->isBookmark)
			bookmarkPalette->BView->ChangeText(doc->Items->at(a));
	}
	view->DrawNew();
	slotDocCh();
}

void ScribusMainWindow::slotFileAppend()
{
	if (doc->m_Selection->count() != 0)
	{
		gtGetText* gt = new gtGetText(doc);
		ImportSetup impsetup=gt->run();
		if (impsetup.runDialog)
		{
			gt->launchImporter(impsetup.importer, impsetup.filename, impsetup.textOnly, impsetup.encoding, true);
		}
		delete gt;
		//CB Hyphenating now emits doc changed, plus we change lang as appropriate
		if (doc->docHyphenator->AutoCheck)
			doc->itemSelection_DoHyphenate();
			//doc->docHyphenator->slotHyphenate(doc->m_Selection->itemAt(0));
		view->DrawNew();
		//slotDocCh();
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
		updateActiveWindowCaption(doc->DocName);
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
	QString fileSpec=tr("Documents (*.sla *.sla.gz);;All Files (*)");
	bool setter=true;
	int optionFlags = fdCompressFile;
#else
	QString fileSpec=tr("Documents (*.sla);;All Files (*)");
	int optionFlags = fdNone;
#endif
	QString fn = CFileDialog( wdir, tr("Save As"), fileSpec, fna, optionFlags);
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
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	ReorgFonts();
	mainWindowStatusLabel->setText( tr("Saving..."));
	mainWindowProgressBar->reset();
	bool ret=doc->save(fn);
	qApp->processEvents();
	if (ret)
	{
		updateActiveWindowCaption(fn);
		undoManager->renameStack(fn);
// 		scrActions["fileSave"]->setEnabled(false);
		scrActions["fileRevert"]->setEnabled(false);
		updateRecent(fn);
	}
	mainWindowStatusLabel->setText("");
	mainWindowProgressBar->reset();
	ScCore->fileWatcher->start();
	return ret;
}

bool ScribusMainWindow::slotFileClose()
{
	ScribusWin* tw = ActWin;
	ActWin->close();
	if (tw == ActWin)
		return false;
	else
		return true;
	windowsMenuAboutToShow();
}

bool ScribusMainWindow::DoFileClose()
{
	view->Deselect(false);
	if (doc==storyEditor->currentDocument())
		storyEditor->close();
	actionManager->disconnectNewDocActions();
	actionManager->disconnectNewViewActions();
	disconnect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	if (doc->viewCount > 1)
	{
		--doc->viewCount;
		closeActiveWindowMasterPageEditor();
		setAppMode(modeNormal);
		disconnect(ScCore->fileWatcher, SIGNAL(fileChanged(QString )), doc, SLOT(updatePict(QString)));
		disconnect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), doc, SLOT(removePict(QString)));
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
	disconnect(ScCore->fileWatcher, SIGNAL(fileChanged(QString )), doc, SLOT(updatePict(QString)));
	disconnect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), doc, SLOT(removePict(QString)));
	for (uint a = 0; a < doc->Items->count(); ++a)
	{
		PageItem *currItem = doc->Items->at(a);
		if (currItem->PicAvail)
			ScCore->fileWatcher->removeFile(currItem->Pfile);
	}
	if (ScCore->haveCMS())
		doc->CloseCMSProfiles();
	//<<Palettes
//	propertiesPalette->NewSel(-1);
	propertiesPalette->unsetDoc();
	pagePalette->setView(0);
	pagePalette->Rebuild();
	propertiesPalette->Spal->setFormats(0);
	propertiesPalette->SetLineFormats(0);
	if (doc->EditClip)
		NoFrameEdit();
	bookmarkPalette->BView->clear();
	bookmarkPalette->BView->NrItems = 0;
	bookmarkPalette->BView->First = 1;
	bookmarkPalette->BView->Last = 0;
	outlinePalette->unsetDoc();
	alignDistributePalette->setDoc(NULL);
	//>>
	if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
	{
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
		scrMenuMgr->setMenuEnabled("FileImport", false);
		scrMenuMgr->setMenuEnabled("FileExport", false);
		scrActions["fileExportAsPDF"]->setEnabled(false);
		scrActions["fileExportText"]->setEnabled(false);
		scrActions["fileExportAsEPS"]->setEnabled(false);
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportText2"]->setEnabled(false);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(false);
		scrActions["pageImport"]->setEnabled(false);

		scrActions["editUndoAction"]->setEnabled(false);
		scrActions["editRedoAction"]->setEnabled(false);
		scrActions["editCut"]->setEnabled(false);
		scrActions["editCopy"]->setEnabled(false);
		scrActions["editPaste"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("EditPasteRecent", false);
		scrActions["editClearContents"]->setEnabled(false);
		scrActions["editSelectAll"]->setEnabled(false);
		scrActions["editDeselectAll"]->setEnabled(false);
		scrActions["editPatterns"]->setEnabled(false);
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
		scrActions["viewShowRulers"]->setEnabled(false);

		scrMenuMgr->setMenuEnabled("Insert", false);
		scrActions["insertFrame"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("Page", false);
		scrMenuMgr->setMenuEnabled("Extras", false);
		scrMenuMgr->setMenuEnabled("Style", false);
		scrMenuMgr->setMenuEnabled("Item", false);
		
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
		scrActions["toolsCopyProperties"]->setEnabled(false);
		scrActions["toolsEyeDropper"]->setEnabled(false);
		scrActions["toolsPDFPushButton"]->setEnabled(false);
		scrActions["toolsPDFTextField"]->setEnabled(false);
		scrActions["toolsPDFCheckBox"]->setEnabled(false);
		scrActions["toolsPDFComboBox"]->setEnabled(false);
		scrActions["toolsPDFListBox"]->setEnabled(false);
		scrActions["toolsPDFAnnotText"]->setEnabled(false);
		scrActions["toolsPDFAnnotLink"]->setEnabled(false);
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
	guidePalette->setDoc(0);
	layerPalette->ClearInhalt();
	docCheckerPalette->buildErrorList(0);
	ScCore->fileWatcher->removeFile(fName);
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
	styleManager->currentDoc(0);
	return true;
}

void ScribusMainWindow::slotFilePrint()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		if (scanDocument())
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
	Druck *printer = new Druck(this, doc, options.filename, options.printer, PDef.Command, PDef.DevMode, prefsManager->appPrefs.GCRMode, spots);
	printer->setMinMax(1, doc->Pages->count(), doc->currentPage()->pageNr()+1);
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
			options.pageNumbers.push_back(doc->currentPage()->pageNr()+1);
		else
		{
			if (printer->RadioButton1->isChecked())
				parsePagesString("*", &options.pageNumbers, doc->DocPages.count());
			else
				parsePagesString(printer->pageNr->text(), &options.pageNumbers, doc->DocPages.count());
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
		options.doClip = printer->doClip();
		options.doGCR = printer->doGCR();
		options.PSLevel = printer->PSLevel();
		options.setDevParam = printer->doDev();
		options.doOverprint = printer->doOverprint();
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

bool ScribusMainWindow::doPrint(PrintOptions *options)
{
	bool retw = false;
	QMap<QString, QMap<uint, FPointArray> > ReallyUsed;
	QString filename(options->filename);
	ReallyUsed.clear();
	doc->getUsedFonts(ReallyUsed);
	ColorList usedColors;
	doc->getUsedColors(usedColors);
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	PSLib *dd = new PSLib(true, prefsManager->appPrefs.AvailFonts, ReallyUsed, usedColors, false, options->useSpotColors);
	if (dd != NULL)
	{
		if (!options->toFile)
			filename = prefsManager->preferencesLocation()+"/tmp.ps";
		else
		{
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			if (!overwrite(this, filename))
			{
				delete dd;
				ScCore->fileWatcher->start();
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
			               options->useColor, options->mirrorH, options->mirrorV, options->useICC, options->doGCR, options->setDevParam, options->doClip, options->doOverprint);
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
	ScCore->fileWatcher->start();
	return retw;
}

void ScribusMainWindow::slotFileQuit()
{
	propertiesPalette->unsetDoc();
	ScCore->pluginManager->savePreferences();
	close();
}

void ScribusMainWindow::slotEditCut()
{
	int a;
	NoFrameEdit();
	QString BufferI = "";
	uint docSelectionCount=doc->m_Selection->count();
	if ((HaveDoc) && (docSelectionCount != 0))
	{
		PageItem *currItem;
		for (uint i = 0; i < docSelectionCount; ++i)
		{
			currItem=doc->m_Selection->itemAt(i);
			if ((currItem->asTextFrame() || currItem->asPathText()) && currItem==storyEditor->currentItem() && doc==storyEditor->currentDocument())
			{
					QMessageBox::critical(this, tr("Cannot Cut In-Use Item"), tr("The item %1 is currently being edited by Story Editor. The cut operation will be cancelled").arg(currItem->itemName()), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
					return;
			}
		}
		if (UndoManager::undoEnabled())
		{
			if (docSelectionCount > 1)
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Cut,"",Um::ICut);
			else
			{
				PageItem* item=doc->m_Selection->itemAt(0);
				undoManager->beginTransaction(item->getUName(), item->getUPixmap(), Um::Cut, "", Um::ICut);
			}
		}
		Buffer2 = "<SCRIBUSTEXT>";
		currItem = doc->m_Selection->itemAt(0);
		if (doc->appMode == modeEdit)
		{
			if ((currItem->itemText.length() == 0) || (!currItem->HasSel))
				return;
			PageItem *nextItem = currItem;
			while (nextItem != 0)
			{
				if (nextItem->BackBox != 0)
					nextItem = nextItem->BackBox;
				else
					break;
			}
			if (nextItem != 0)
			{
				for (a = 0; a < nextItem->itemText.length(); ++a)
				{
					if (nextItem->itemText.selected(a))
					{
						if (nextItem->itemText.text(a) == QChar(13))
						{
							Buffer2 += QChar(5);
							BufferI += QChar(10);
						}
						else if (nextItem->itemText.text(a) == QChar(9))
						{
							Buffer2 += QChar(4);
							BufferI += QChar(9);
						}
						else
						{
							Buffer2 += nextItem->itemText.text(a);
							BufferI += nextItem->itemText.text(a);
						}
						Buffer2 += "\t";
						Buffer2 += nextItem->itemText.charStyle(a).font().scName()+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).fontSize())+"\t";
						Buffer2 += nextItem->itemText.charStyle(a).fillColor()+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).tracking())+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).fillShade())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).effects())+'\t';
						Buffer2 += QString::number(findParagraphStyle(doc, nextItem->itemText.paragraphStyle(a)))+'\t';
						Buffer2 += nextItem->itemText.charStyle(a).strokeColor()+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).strokeShade())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).scaleH())+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).scaleV())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).baselineOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).shadowXOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).shadowYOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).outlineWidth())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).underlineOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).underlineWidth())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).strikethruOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).strikethruWidth())+'\n';
					}
				}
				dynamic_cast<PageItem_TextFrame*>(nextItem)->deleteSelectedTextFromFrame();
				nextItem = nextItem->NextBox;
			}
			view->RefreshItem(currItem);
		}
		else
		{
			if (((currItem->isSingleSel) && (currItem->isGroupControl)) || ((currItem->isSingleSel) && (currItem->isTableItem)))
				return;
			ScriXmlDoc *ss = new ScriXmlDoc();
			BufferI = ss->WriteElem(doc, view, doc->m_Selection);
			Buffer2 = BufferI;
			if (prefsManager->appPrefs.doCopyToScrapbook)
			{
				scrapbookPalette->ObjFromCopyAction(Buffer2);
				rebuildRecentPasteMenu();
			}
			doc->itemSelection_DeleteItem();
			delete ss;
		}
		slotDocCh();
		BuFromApp = true;
		ClipB->setText(BufferI);
		scrActions["editPaste"]->setEnabled(true);
		scrMenuMgr->setMenuEnabled("EditPasteRecent", scrapbookPalette->tempBView->objectMap.count() != 0);
		if (UndoManager::undoEnabled())
			undoManager->commit();
	}
}

void ScribusMainWindow::slotEditCopy()
{
	int a;
	NoFrameEdit();
	QString BufferI = "";
	if ((HaveDoc) && (doc->m_Selection->count() != 0))
	{
		Buffer2 = "<SCRIBUSTEXT>";
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
			if (nextItem != 0)
			{
				for (a = 0; a < nextItem->itemText.length(); ++a)
				{
					if (nextItem->itemText.selected(a))
					{
						if (nextItem->itemText.text(a) == SpecialChars::PARSEP)
						{
							Buffer2 += QChar(5);
							BufferI += QChar(10);
						}
						else if (nextItem->itemText.text(a) == SpecialChars::TAB)
						{
							Buffer2 += QChar(4);
							BufferI += QChar(9);
						}
						else
						{
							Buffer2 += nextItem->itemText.text(a);
							BufferI += nextItem->itemText.text(a);
						}
						Buffer2 += "\t";
						Buffer2 += nextItem->itemText.charStyle(a).font().scName()+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).fontSize())+"\t";
						Buffer2 += nextItem->itemText.charStyle(a).fillColor()+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).tracking())+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).fillShade())+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).effects())+"\t";
						Buffer2 += QString::number(findParagraphStyle(doc, nextItem->itemText.paragraphStyle(a)))+"\t";
						Buffer2 += nextItem->itemText.charStyle(a).strokeColor()+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).strokeShade())+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).scaleH())+"\t";
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).scaleV())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).baselineOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).shadowXOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).shadowYOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).outlineWidth())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).underlineOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).underlineWidth())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).strikethruOffset())+'\t';
						Buffer2 += QString::number(nextItem->itemText.charStyle(a).strikethruWidth())+'\n';
					}
				}
				nextItem = nextItem->NextBox;
			}
		}
		else
		{
			if (((currItem->isSingleSel) && (currItem->isGroupControl)) || ((currItem->isSingleSel) && (currItem->isTableItem)))
				return;
			ScriXmlDoc *ss = new ScriXmlDoc();
			BufferI = ss->WriteElem(doc, view, doc->m_Selection);
			Buffer2 = BufferI;
			if (prefsManager->appPrefs.doCopyToScrapbook)
			{
				scrapbookPalette->ObjFromCopyAction(Buffer2);
				rebuildRecentPasteMenu();
			}
			delete ss;
		}
		BuFromApp = true;
		ClipB->setText(BufferI);
		scrActions["editPaste"]->setEnabled(true);
		scrMenuMgr->setMenuEnabled("EditPasteRecent", scrapbookPalette->tempBView->objectMap.count() != 0);
	}
}

void ScribusMainWindow::slotEditPaste()
{
	NoFrameEdit();
	if (HaveDoc)
	{
		if (Buffer2.isNull())
			return;
		if (UndoManager::undoEnabled())
			undoManager->beginTransaction(doc->currentPage()->getUName(), 0, Um::Paste, "", Um::IPaste);
		if (doc->appMode == modeEdit)
		{
			PageItem_TextFrame *currItem = dynamic_cast<PageItem_TextFrame*>(doc->m_Selection->itemAt(0));
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
#if 0
					ScText *hg;
					hg = new ScText;
					hg->ch = (*it);
					if (hg->ch == QChar(5))
						hg->ch = QChar(13);
					if (hg->ch == QChar(4))
						hg->ch = QChar(9);
/* 	Don't copy inline frames for now, as this is a very complicated thing.
		We need to figure out a good way to copy inline frames, this must
		be able to preserve them across documents. No idea how to solve
		that yet. */
					if (hg->ch == QChar(25))
						hg->ch = QChar(32);
					it++;
					hg->setFont((*doc->AllFonts)[*it]);
					it++;
					hg->setFontSize((*it).toInt());
					it++;
					hg->setFillColor(*it);
					it++;
					hg->setTracking((*it).toInt());
					it++;
					hg->setFillShade((*it).toInt());
					it++;
					hg->setEffects(static_cast<StyleFlag>((*it).toInt()));
					it++;
//					hg->cab = (*it).toInt();
					it++;
					if (it == NULL)
						hg->setStrokeColor(CommonStrings::None);
					else
						hg->setStrokeColor(*it);
					it++;
					if (it == NULL)
						hg->setStrokeShade(100);
					else
						hg->setStrokeShade((*it).toInt());
					it++;
					if (it == NULL)
						hg->setScaleH(1000);
					else
						hg->setScaleH((*it).toInt());
					it++;
					if (it == NULL)
						hg->setScaleV(1000);
					else
						hg->setScaleV(QMIN(QMAX((*it).toInt(), 100), 4000));
					it++;
					hg->setBaselineOffset(it == NULL ? 0 : (*it).toInt());
					it++;
					hg->setShadowXOffset(it == NULL ? 50 : (*it).toInt());
					it++;
					hg->setShadowYOffset(it == NULL ? -50 : (*it).toInt());
					it++;
					hg->setOutlineWidth(it == NULL ? 10 : (*it).toInt());
					it++;
					hg->setUnderlineOffset(it == NULL ? -1 : (*it).toInt());
					it++;
					hg->setUnderlineWidth(it == NULL ? -1 : (*it).toInt());
					it++;
					hg->setStrikethruOffset(it == NULL ? -1 : (*it).toInt());
					it++;
					hg->setStrikethruWidth(it == NULL ? -1 : (*it).toInt());
					currItem->itemText.insertChars(currItem->CPos, hg);
					currItem->CPos += 1;
					hg->PRot = 0;
					hg->PtransX = 0;
					hg->PtransY = 0;
					hg->cembedded = 0;
#else
					CharStyle nstyle;
					QString ch = (*it);
					if (ch == QChar(5))
						ch = SpecialChars::PARSEP;
					if (ch == QChar(4))
						ch = SpecialChars::TAB;
					/* 	Don't copy inline frames for now, as this is a very complicated thing.
						We need to figure out a good way to copy inline frames, this must
						be able to preserve them across documents. No idea how to solve
						that yet. */
					if (ch == SpecialChars::OBJECT)
						ch = QChar(32);
					it++;
					nstyle.setFont((*doc->AllFonts)[*it]);
					it++;
					nstyle.setFontSize((*it).toInt());
					it++;
					nstyle.setFillColor(*it);
					it++;
					nstyle.setTracking((*it).toInt());
					it++;
					nstyle.setFillShade((*it).toInt());
					it++;
					nstyle.setEffects(static_cast<StyleFlag>((*it).toInt()));
					it++;
					int cab = (*it).toInt();
					it++;
					if (it == NULL)
						nstyle.setStrokeColor(CommonStrings::None);
					else
						nstyle.setStrokeColor(*it);
					it++;
					if (it == NULL)
						nstyle.setStrokeShade(100);
					else
						nstyle.setStrokeShade((*it).toInt());
					it++;
					if (it == NULL)
						nstyle.setScaleH(1000);
					else
						nstyle.setScaleH((*it).toInt());
					it++;
					if (it == NULL)
						nstyle.setScaleV(1000);
					else
						nstyle.setScaleV(QMIN(QMAX((*it).toInt(), 100), 4000));
					it++;
					nstyle.setBaselineOffset(it == NULL ? 0 : (*it).toInt());
					it++;
					nstyle.setShadowXOffset(it == NULL ? 50 : (*it).toInt());
					it++;
					nstyle.setShadowYOffset(it == NULL ? -50 : (*it).toInt());
					it++;
					nstyle.setOutlineWidth(it == NULL ? 10 : (*it).toInt());
					it++;
					nstyle.setUnderlineOffset(it == NULL ? -1 : (*it).toInt());
					it++;
					nstyle.setUnderlineWidth(it == NULL ? -1 : (*it).toInt());
					it++;
					nstyle.setStrikethruOffset(it == NULL ? -1 : (*it).toInt());
					it++;
					nstyle.setStrikethruWidth(it == NULL ? -1 : (*it).toInt());
					currItem->itemText.insertChars(currItem->CPos, ch);
					if (ch == SpecialChars::PARSEP) {
						currItem->itemText.applyStyle(currItem->CPos, doc->docParagraphStyles[cab]);
					}
					else {						
						currItem->itemText.applyCharStyle(currItem->CPos, 1, nstyle);
					}
					currItem->CPos += 1;
#endif
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
				//int tempList=doc->m_Selection->backupToTempList(0);
				Selection tempSelection(*doc->m_Selection);
				doc->m_Selection->clear();
				if (doc->Items->count() - ac > 1)
					isGroup = true;
				for (uint as = ac; as < doc->Items->count(); ++as)
				{
					doc->m_Selection->addItem(doc->Items->at(as));
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
				doc->m_Selection->clear();
				//doc->m_Selection->restoreFromTempList(0, tempList);
				*doc->m_Selection=tempSelection;
				view->resizeContents(qRound((maxSize.x() - minSize.x()) * view->scale()), qRound((maxSize.y() - minSize.y()) * view->scale()));
				view->scrollBy(qRound((doc->minCanvasCoordinate.x() - minSize.x()) * view->scale()), qRound((doc->minCanvasCoordinate.y() - minSize.y()) * view->scale()));
				doc->minCanvasCoordinate = minSize;
				doc->maxCanvasCoordinate = maxSize;
				outlinePalette->BuildTree();
				outlinePalette->reopenTree(doc->OpenNodes);
				currItem->itemText.insertObject(currItem->CPos, currItem3);
				currItem->CPos += 1;
			}
			else
			{
				Serializer *ss = new Serializer("");
				ss->Objekt = Buffer2;
//FIXME: that st business doesn't look right
				int st = findParagraphStyle(doc, doc->currentStyle);
				if (st > 5)
					ss->GetText(currItem, st, doc->docParagraphStyles[st].charStyle().font().scName(), doc->docParagraphStyles[st].charStyle().fontSize(), true);
				else
					ss->GetText(currItem, st, currItem->currentCharStyle().font().scName(), currItem->currentCharStyle().fontSize(), true);
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
				slotElemRead(Buffer2, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
				doc->useRaster = savedAlignGrid;
				doc->SnapGuides = savedAlignGuides;
				for (uint as = ac; as < doc->Items->count(); ++as)
				{
					PageItem* currItem = doc->Items->at(as);
					if (currItem->isBookmark)
						AddBookMark(currItem);
					doc->m_Selection->addItem(currItem);
				}
				int docSelectionCount=doc->m_Selection->count();
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
					HaveNewSel(doc->m_Selection->itemAt(0)->itemType());
			}
			view->DrawNew();
		}
		if (UndoManager::undoEnabled())
			undoManager->commit();
		slotDocCh(false);
	}
}

//CB-->Doc ?????
void ScribusMainWindow::SelectAll()
{
	if (doc->appMode == modeEdit)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
			nextItem->itemText.selectAll();
			nextItem->HasSel = true;
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
			if ((currItem->LayerNr == doc->activeLayer()) && (!doc->layerLocked(currItem->LayerNr)))
			{
				if (!currItem->isSelected())
				{
					doc->m_Selection->addItem(currItem);
					currItem->FrameOnly = true;
					currItem->paintObj();
				}
			}
		}
		int docSelectionCount=doc->m_Selection->count();
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
			currItem = doc->m_Selection->itemAt(0);
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
	HelpBrowser *dia = new HelpBrowser(0, tr("Scribus Manual"), ScCore->getGuiLanguage());
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
		wdir = prefsManager->prefsFile->getContext("dirs")->get("save_text", prefsDocDir);
	else
		wdir = prefsManager->prefsFile->getContext("dirs")->get("save_text", ".");
	QString fn = CFileDialog( wdir, tr("Save as"), tr("Text Files (*.txt);;All Files(*)"), "", fdShowCodecs);
	if (!fn.isEmpty())
	{
		prefsManager->prefsFile->getContext("dirs")->set("save_text", fn.left(fn.findRev("/")));
		Serializer *se = new Serializer(fn);
		se->PutText(doc->m_Selection->itemAt(0));
		se->Write(LoadEnc);
		delete se;
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
	InsPage *dia = new InsPage(this, doc, doc->currentPage()->pageNr(), doc->Pages->count());
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
		{
			int setcol = doc->pageSets[doc->currentPageLayout].Columns;
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
		undoManager->action(this, ss);
	}
	// disable recording of undo actions related to new page creating
	// and object moving related to that
	undoManager->setUndoEnabled(false);

	QStringList base;
	if (basedOn == NULL)
	{
		int setcol = doc->pageSets[doc->currentPageLayout].Columns;
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
		slotNewPage(wot, base[(wot+doc->pageSets[doc->currentPageLayout].FirstPage) % doc->pageSets[doc->currentPageLayout].Columns], mov); //Avoid the master page application with QString::null
//		slotNewPage(wot, QString::null, mov); //Avoid the master page application with QString::null
		doc->currentPage()->setInitialHeight(height);
		doc->currentPage()->setInitialWidth(width);
		doc->currentPage()->PageOri = orient;
		doc->currentPage()->m_pageSize = siz;
		//CB If we want to add this master page setting into the slotnewpage call, the pagenumber must be +1 I think
	//Apply_MasterPage(base[(doc->currentPage()->pageNr()+doc->pageSets[doc->currentPageLayout].FirstPage) % doc->pageSets[doc->currentPageLayout].Columns],
//						 doc->currentPage()->pageNr(), false); // this Apply_MasterPage avoids DreawNew and PagePalette->ReBuild, which is much faster for 100 pp :-)
		wot ++;
	}
	//Must use wo-1 as the dialog currently returns a page Index +1 due to old numbering scheme
	doc->addPageToSection(wo-1, where, numPages);
	pagePalette->RebuildPage();
	view->reformPages(mov);
	view->DrawNew();
	outlinePalette->BuildTree();

	undoManager->setUndoEnabled(true);

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
	}
}

void ScribusMainWindow::slotNewPage(int w, const QString& masterPageName, bool mov)
{
	doc->addPage(w, masterPageName, true);
	view->addPage(w, mov);
/*	if ((!doc->loading) && (!doc->masterPageMode))
		outlinePalette->BuildTree(doc); */
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
		while (doc->MasterNames.contains(masterPageName) || (masterPageName == CommonStrings::masterPageNormal))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			masterPageName = dia->Answer->text();
		}
		int currentPageNumber=doc->currentPage()->pageNr();
		bool ok=doc->copyPageToMasterPage(currentPageNumber, diaLinksCurrItem, diaLinksCount, masterPageName);
		Q_ASSERT(ok); //TODO get a return value in case the copy was not possible
	}
	delete dia;
}

void ScribusMainWindow::slotZoom(double zoomFactor)
{
	double finalZoomFactor;
	//Zoom to Fit
	if (zoomFactor==-100.0)
	{
		double dx = (view->width()-50) / (doc->currentPage()->width()+30);
		double dy = (view->height()-70) / (doc->currentPage()->height()+30);
		finalZoomFactor = (dx > dy) ? dy : dx;
	}
	//Zoom to %
	else
		finalZoomFactor = zoomFactor*prefsManager->displayScale()/100.0;
	if (finalZoomFactor == view->scale())
		return;
	int x = qRound(QMAX(view->contentsX() / view->scale(), 0));
	int y = qRound(QMAX(view->contentsY() / view->scale(), 0));
	int w = qRound(QMIN(view->visibleWidth() / view->scale(), doc->currentPage()->width()));
	int h = qRound(QMIN(view->visibleHeight() / view->scale(), doc->currentPage()->height()));
	view->rememberPreviousSettings(w / 2 + x,h / 2 + y);
	view->setScale(finalZoomFactor);
	view->slotDoZoom();
}

void ScribusMainWindow::ToggleAllPalettes()
{
	if (palettesStatus[0])
	{
		palettesStatus[0] = false;
		if (palettesStatus[1])
			propertiesPalette->show();
		if (palettesStatus[2])
			outlinePalette->show();
		if (palettesStatus[3])
			scrapbookPalette->show();
		if (palettesStatus[4])
			layerPalette->show();
		if (palettesStatus[5])
			pagePalette->show();
		if (palettesStatus[6])
			bookmarkPalette->show();
		if (palettesStatus[7])
			measurementPalette->show();
		if (palettesStatus[9])
			docCheckerPalette->show();
		setPagePalette(palettesStatus[5]);
		setUndoPalette(palettesStatus[8]);
	}
	else
	{
		palettesStatus[1] = propertiesPalette->isVisible();
		palettesStatus[2] = outlinePalette->isVisible();
		palettesStatus[3] = scrapbookPalette->isVisible();
		palettesStatus[4] = layerPalette->isVisible();
		palettesStatus[5] = pagePalette->isVisible();
		palettesStatus[6] = bookmarkPalette->isVisible();
		palettesStatus[7] = measurementPalette->isVisible();
		palettesStatus[8] = undoPalette->isVisible();
		palettesStatus[9] = docCheckerPalette->isVisible();
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
		palettesStatus[0] = true;
	}
}

void ScribusMainWindow::toggleCheckPal()
{
	palettesStatus[0] = false;
}

void ScribusMainWindow::setUndoPalette(bool visible)
{
	undoPalette->setPaletteShown(visible);
	scrActions["toolsActionHistory"]->setOn(visible);
}
/*
void ScribusMainWindow::togglePropertiesPalette()
{
	palettesStatus[0] = false;
}

void ScribusMainWindow::toggleOutlinePalette()
{
	palettesStatus[0] = false;
}

void ScribusMainWindow::toggleScrapbookPalette()
{
	palettesStatus[0] = false;
}

void ScribusMainWindow::toggleLayerPalette()
{
	palettesStatus[0] = false;
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
	palettesStatus[0] = false;
}
/*
void ScribusMainWindow::toggleBookmarkPalette()
{
	palettesStatus[0] = false;
}
*/

void ScribusMainWindow::toggleUndoPalette()
{
	setUndoPalette(!undoPalette->isVisible());
	palettesStatus[0] = false;
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
	if (guidesStatus[0])
	{
		guidesStatus[0] = false;
		doc->guidesSettings.marginsShown = guidesStatus[1];
		doc->guidesSettings.framesShown = guidesStatus[2];
		doc->guidesSettings.gridShown = guidesStatus[3];
		doc->guidesSettings.guidesShown = guidesStatus[4];
		doc->guidesSettings.baseShown = guidesStatus[5];
		doc->guidesSettings.linkShown = guidesStatus[6];
		doc->guidesSettings.showControls = guidesStatus[7];
		doc->guidesSettings.rulerMode = guidesStatus[8];
		doc->guidesSettings.rulersShown = guidesStatus[9];
		doc->guidesSettings.colBordersShown = guidesStatus[10];
		ToggleMarks();
		ToggleFrames();
		ToggleLayerMarkers();
		ToggleRaster();
		ToggleGuides();
		ToggleColumnBorders();
		ToggleBase();
		ToggleTextLinks();
		ToggleTextControls();
		ToggleRulerMode();
		ToggleRulers();
	}
	else
	{
		guidesStatus[0] = true;
		guidesStatus[1] = !doc->guidesSettings.marginsShown;
		guidesStatus[2] = !doc->guidesSettings.framesShown;
		guidesStatus[3] = !doc->guidesSettings.gridShown;
		guidesStatus[4] = !doc->guidesSettings.guidesShown;
		guidesStatus[5] = !doc->guidesSettings.baseShown;
		guidesStatus[6] = !doc->guidesSettings.linkShown;
		guidesStatus[7] = !doc->guidesSettings.showControls;
		guidesStatus[8] = !doc->guidesSettings.rulerMode;
		guidesStatus[9] = !doc->guidesSettings.rulersShown;
		guidesStatus[10] = !doc->guidesSettings.colBordersShown;
		guidesStatus[11] = !doc->guidesSettings.layerMarkersShown;
		doc->guidesSettings.marginsShown = false;
		doc->guidesSettings.framesShown = false;
		doc->guidesSettings.gridShown = false;
		doc->guidesSettings.guidesShown = false;
		doc->guidesSettings.baseShown = false;
		doc->guidesSettings.linkShown = false;
		doc->guidesSettings.showControls = false;
		doc->guidesSettings.rulerMode = false;
		doc->guidesSettings.rulersShown = false;
		doc->guidesSettings.colBordersShown = false;
		doc->guidesSettings.layerMarkersShown = false;
		view->setRulersShown(doc->guidesSettings.rulersShown);
	}
	scrActions["viewShowMargins"]->setOn(doc->guidesSettings.marginsShown);
	scrActions["viewShowFrames"]->setOn(doc->guidesSettings.framesShown);
	scrActions["viewShowLayerMarkers"]->setOn(doc->guidesSettings.layerMarkersShown);
	scrActions["viewShowGrid"]->setOn(doc->guidesSettings.gridShown);
	scrActions["viewShowGuides"]->setOn(doc->guidesSettings.guidesShown);
	scrActions["viewShowColumnBorders"]->setOn(doc->guidesSettings.colBordersShown);
	scrActions["viewShowBaseline"]->setOn(doc->guidesSettings.baseShown);
	scrActions["viewShowTextChain"]->setOn(doc->guidesSettings.linkShown);
	scrActions["viewShowTextControls"]->setOn(doc->guidesSettings.showControls);
	scrActions["viewShowRulers"]->setOn(doc->guidesSettings.rulersShown);
	scrActions["viewRulerMode"]->setOn(doc->guidesSettings.rulerMode);
	view->DrawNew();
}

void ScribusMainWindow::ToggleMarks()
{
	guidesStatus[0] = false;
	doc->guidesSettings.marginsShown = !doc->guidesSettings.marginsShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleFrames()
{
	guidesStatus[0] = false;
	doc->guidesSettings.framesShown = !doc->guidesSettings.framesShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleLayerMarkers()
{
	guidesStatus[0] = false;
	doc->guidesSettings.layerMarkersShown = !doc->guidesSettings.layerMarkersShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleRaster()
{
	guidesStatus[0] = false;
	doc->guidesSettings.gridShown = !doc->guidesSettings.gridShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleGuides()
{
	guidesStatus[0] = false;
	doc->guidesSettings.guidesShown = !doc->guidesSettings.guidesShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleColumnBorders()
{
	guidesStatus[0] = false;
	doc->guidesSettings.colBordersShown = !doc->guidesSettings.colBordersShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleBase()
{
	guidesStatus[0] = false;
	doc->guidesSettings.baseShown = !doc->guidesSettings.baseShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleTextLinks()
{
	guidesStatus[0] = false;
	doc->guidesSettings.linkShown = !doc->guidesSettings.linkShown;
	view->DrawNew();
}

void ScribusMainWindow::ToggleTextControls()
{
	guidesStatus[0] = false;
	doc->guidesSettings.showControls = !doc->guidesSettings.showControls;
	view->DrawNew();
}

void ScribusMainWindow::ToggleRulers()
{
	guidesStatus[0] = false;
	doc->guidesSettings.rulersShown = !doc->guidesSettings.rulersShown;
	view->setRulersShown(doc->guidesSettings.rulersShown);
}

void ScribusMainWindow::ToggleRulerMode()
{
	guidesStatus[0] = false;
	doc->guidesSettings.rulerMode = !doc->guidesSettings.rulerMode;
	if (doc->guidesSettings.rulerMode)
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

void ScribusMainWindow::ToggleURaster()
{
	doc->useRaster = !doc->useRaster;
	slotDocCh();
}

void ScribusMainWindow::ToggleUGuides()
{
	doc->SnapGuides = !doc->SnapGuides;
	slotDocCh();
}

void ScribusMainWindow::ToggleFrameEdit()
{
	if (doc->EditClip)
	{
		NoFrameEdit();
	}
	else
	{
		//CB Enable/Disable undo in frame edit mode
// 		undoManager->setUndoEnabled(false);
		scrActions["editUndoAction"]->setEnabled(false);
		scrActions["editRedoAction"]->setEnabled(false);
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
		scrActions["toolsCopyProperties"]->setEnabled(false);
		scrActions["toolsEyeDropper"]->setEnabled(false);
		scrActions["toolsPDFPushButton"]->setEnabled(false);
		scrActions["toolsPDFTextField"]->setEnabled(false);
		scrActions["toolsPDFCheckBox"]->setEnabled(false);
		scrActions["toolsPDFComboBox"]->setEnabled(false);
		scrActions["toolsPDFListBox"]->setEnabled(false);
		scrActions["toolsPDFAnnotText"]->setEnabled(false);
		scrActions["toolsPDFAnnotLink"]->setEnabled(false);
		scrActions["itemDelete"]->setEnabled(false);
		if (doc->m_Selection->count() != 0)
		{
			PageItem *currItem = doc->m_Selection->itemAt(0);
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
	nodePalette->setDoc(0,0);
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
	scrActions["toolsPDFPushButton"]->setEnabled(true);
	scrActions["toolsPDFTextField"]->setEnabled(true);
	scrActions["toolsPDFCheckBox"]->setEnabled(true);
	scrActions["toolsPDFComboBox"]->setEnabled(true);
	scrActions["toolsPDFListBox"]->setEnabled(true);
	scrActions["toolsPDFAnnotText"]->setEnabled(true);
	scrActions["toolsPDFAnnotLink"]->setEnabled(true);
	scrActions["toolsEditContents"]->setOn(false);
	scrActions["toolsEditWithStoryEditor"]->setOn(false);
	scrActions["toolsMeasurements"]->setEnabled(true);
	scrActions["toolsCopyProperties"]->setEnabled(true);
	scrActions["toolsEyeDropper"]->setEnabled(true);
	scrActions["toolsUnlinkTextFrame"]->setEnabled(true);
	scrActions["itemDelete"]->setEnabled(true);
	scrActions["itemShapeEdit"]->setOn(false);
// 	bool tmpClip = doc->EditClip; // for enabling undo if exiting shape edit mode
	if (HaveDoc)
	{
		doc->EditClip = false;
		view->EditContour = false;
		if (doc->m_Selection->count() != 0)
		{
			HaveNewSel(doc->m_Selection->itemAt(0)->itemType());
			view->DrawNew();
		}
		else
			HaveNewSel(-1);
	}
	actionManager->connectModeActions();
// 	if (tmpClip)
// 		undoManager->setUndoEnabled(true);
}

void ScribusMainWindow::slotSelect()
{
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
	scrActions["toolsPDFPushButton"]->setOn(mode==modeInsertPDFButton);
	scrActions["toolsPDFTextField"]->setOn(mode==modeInsertPDFTextfield);
	scrActions["toolsPDFCheckBox"]->setOn(mode==modeInsertPDFCheckbox);
	scrActions["toolsPDFComboBox"]->setOn(mode==modeInsertPDFCombobox);
	scrActions["toolsPDFListBox"]->setOn(mode==modeInsertPDFListbox);
	scrActions["toolsPDFAnnotText"]->setOn(mode==modeInsertPDFTextAnnotation);
	scrActions["toolsPDFAnnotLink"]->setOn(mode==modeInsertPDFLinkAnnotation);

	if (HaveDoc)
	{
		PageItem *currItem=0;
		if (doc->m_Selection->count() != 0)
			currItem = doc->m_Selection->itemAt(0);
		int oldMode = doc->appMode;
		doc->appMode = mode;
		if (oldMode == modeMeasurementTool)
			disconnect(view, SIGNAL(MVals(double, double, double, double, double, double, int )), measurementPalette, SLOT(setValues(double, double, double, double, double, double, int )));
		if (mode != modeEdit && doc->CurTimer!=NULL)
		{
			disconnect(doc->CurTimer, SIGNAL(timeout()), view, SLOT(blinkCursor()));
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
				actionManager->enableUnicodeActions(&scrActions, true, doc->currentStyle.charStyle().font().scName());
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
				connect(doc->CurTimer, SIGNAL(timeout()), view, SLOT(blinkCursor()));
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
		int docSelectionCount=doc->m_Selection->count();
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
			propertiesPalette->setGradientEditMode(false);
		}
		if (mode == modeLinkFrames)
			doc->ElemToLink = doc->m_Selection->itemAt(0);
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
			if (doc->m_Selection->count() != 0)
			{
				doc->ElemToLink = doc->m_Selection->itemAt(0);
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
	doc->currentStyle.charStyle().setEffects(static_cast<StyleFlag>(c));
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
	if (doc->m_Selection->count() != 0)
	{
		doc->currentStyle.charStyle().setEffects(static_cast<StyleFlag>(h));
		setStilvalue(h);
		doc->chTyStyle(h);
	}
}

//CB-->Doc partly
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
	doc->m_Selection->clear();
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
			doc->m_Selection->addItem(ite);
		}
	}
	if (doc->m_Selection->count() != 0)
		doc->itemSelection_DeleteItem();
	Page *page = doc->Pages->at(pg); // need to remove guides too to get their undo/redo actions working
	/* PV - guides refactoring
    for (uint i = 0; i < page->YGuides.count(); ++i)
		page->removeYGuide(static_cast<int>(i));
	for (uint i = 0; i < page->XGuides.count(); ++i)
		page->removeXGuide(static_cast<int>(i));
	*/
	page->guides.clearHorizontals(GuideManagerCore::Standard);
	page->guides.clearHorizontals(GuideManagerCore::Auto);
	page->guides.clearVerticals(GuideManagerCore::Standard);
	page->guides.clearVerticals(GuideManagerCore::Auto);
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
	undoManager->setUndoEnabled(false); // ugly hack to prevent object moving when undoing page deletion
	view->reformPages();
	undoManager->setUndoEnabled(true); // ugly hack continues
	view->DrawNew();
	doc->OpenNodes.clear();
	outlinePalette->BuildTree();
	slotDocCh();
	pagePalette->RebuildPage();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusMainWindow::DeletePage()
{
	NoFrameEdit();
	view->Deselect(true);
	DelPages *dia = new DelPages(this, doc->currentPage()->pageNr()+1, doc->Pages->count());
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
	doc->m_Selection->clear();
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
				doc->m_Selection->addItem(ite);
			}
		}
		Page *page = doc->Pages->at(a); // need to remove guides too to get their undo/redo actions working
        /* PV - guides refactoring
		for (uint i = 0; i < page->YGuides.count(); ++i)
			page->removeYGuide(static_cast<int>(i));
		for (uint i = 0; i < page->XGuides.count(); ++i)
			page->removeXGuide(static_cast<int>(i));
		*/
		page->guides.clearHorizontals(GuideManagerCore::Standard);
		page->guides.clearHorizontals(GuideManagerCore::Auto);
		page->guides.clearVerticals(GuideManagerCore::Standard);
		page->guides.clearVerticals(GuideManagerCore::Auto);
	}
	if (doc->m_Selection->count() != 0)
		doc->itemSelection_DeleteItem();
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
	undoManager->setUndoEnabled(false); // ugly hack to disable object moving when undoing page deletion
	view->reformPages();
	undoManager->setUndoEnabled(true); // ugly hack continues
	view->DrawNew();
//	doc->OpenNodes.clear();
	outlinePalette->BuildTree();
	pagePalette->RebuildPage();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusMainWindow::MovePage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->currentPage()->pageNr()+1, doc->Pages->count(), true);
	if (dia->exec())
	{
//		doc->OpenNodes = outlinePalette->buildReopenVals();
		int from = dia->getFromPage();
		int to = dia->getToPage();
		int wie = dia->getWhere();
		int wo = dia->getWherePage();
		if (from != wo || (wie == 2 && to != signed(doc->Pages->count()) ) )
		{
			doc->movePage(from-1, to, wo-1, wie);
			view->reformPages();
			view->DrawNew();
			pagePalette->RebuildPage();
			outlinePalette->BuildTree();
			outlinePalette->reopenTree(doc->OpenNodes);
		}
	}
	delete dia;
}

void ScribusMainWindow::CopyPage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->currentPage()->pageNr()+1, doc->Pages->count(), false);
	if (dia->exec())
	{
		int pageNumberToCopy=dia->getFromPage()-1;
		int whereToInsert=dia->getWhere();
		int copyCount=dia->getCopyCount();
		int wo = dia->getWherePage();
		doc->copyPage(pageNumberToCopy, wo, whereToInsert, copyCount);
		view->Deselect(true);
		view->DrawNew();
		pagePalette->RebuildPage();
		outlinePalette->BuildTree();
	}
	delete dia;
}

void ScribusMainWindow::changePageMargins()
{
	NoFrameEdit();
	QString Nam = doc->currentPage()->MPageNam;
	MarginDialog *dia = new MarginDialog(this, doc);
	if (dia->exec())
	{
		int orientation = dia->getPageOrientation();
		double ph = dia->getPageHeight();
		double pw = dia->getPageWidth();
		QString sizeName = dia->getpPrefsPageSizeName();
		if (doc->masterPageMode())
		{
			int lp=0;
			if (doc->currentPageLayout != singlePage)
				lp = dia->pageOrder();
			doc->changePageMargins(dia->top(), dia->bottom(),
								   dia->left(), dia->right(),
								   ph, pw, ph, pw, orientation,
								   sizeName, doc->currentPage()->pageNr(), lp);
		}
		else
		{
			doc->changePageMargins(dia->top(), dia->bottom(),
								   dia->left(), dia->right(),
								   ph, pw, ph, pw, orientation,
								   sizeName, doc->currentPage()->pageNr());
			if (dia->masterPage() != Nam)
				Apply_MasterPage(dia->masterPage(), doc->currentPage()->pageNr());
		}
		view->reformPages(dia->getMoveObjects());
		view->DrawNew();
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
			FontID.insert(a, prefsManager->appPrefs.AvailFonts[nf].scName());
		}
		else
		{//CB FIXME: to doc?
			if (doc->m_Selection->count() != 0)
			{
				PageItem *currItem = doc->m_Selection->itemAt(0);
				nf2 = currItem->currentCharStyle().font().scName();
			}
			propertiesPalette->Fonts->RebuildList(doc);
			buildFontMenu();
		}
	}
	AdjustFontMenu(nf2);
	doc->ItemFont(nf2);
	doc->currentStyle.charStyle().setFont((*doc->AllFonts)[nf2]);
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
		}
		delete dia;
	}
	propertiesPalette->setSize(c*10);
	slotDocCh();
}

void ScribusMainWindow::setFSizeMenu(int size)
{
	if (scrActions[QString("fontSize%1").arg(size/10)])
		scrActions[QString("fontSize%1").arg(size/10)]->setOn(true);
}

//CB-->Doc
void ScribusMainWindow::setItemFarbe(int id)
{
	if (doc->m_Selection->count() != 0)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
		if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
			doc->ItemTextBrush(ColorMenC->text(id));
		else
			doc->ItemBrush(ColorMenC->text(id));
	}
	scrMenuMgr->getLocalPopupMenu("Color")->activateItemAt(0);
	slotDocCh();
}

//CB-->Doc partly
void ScribusMainWindow::setItemShade(int id)
{
	int c = id;
	bool ok = false;
	if (doc->m_Selection->count() != 0)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
			}
			delete dia;
		}
	}
	slotDocCh();
}

void ScribusMainWindow::setCSMenu(QString , QString l, int  , int ls)
{
	QString la;
	int lb;
	PageItem *currItem;
	if (doc->m_Selection->count() != 0)
	{
		currItem = doc->m_Selection->itemAt(0);
		if ((currItem->asTextFrame()) || (currItem->asPathText()))
		{
			if ((doc->appMode == modeEdit) && (currItem->itemText.length() != 0))
			{
				la = currItem->itemText.charStyle(QMIN(currItem->CPos, static_cast<int>(currItem->itemText.length()-1))).fillColor();
				lb = currItem->itemText.charStyle(QMIN(currItem->CPos, static_cast<int>(currItem->itemText.length()-1))).fillShade();
			}
			else
			{
				la = currItem->itemText.defaultStyle().charStyle().fillColor();
				lb = currItem->itemText.defaultStyle().charStyle().fillShade();
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
		la = CommonStrings::NoneColor;
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
	ers.clear();
	ers.append(0);
	ers.append(1);
	ers.append(2);
	ers.append(3);
	ers.append(4);
	for (uint a=5; a<doc->docParagraphStyles.count(); ++a)
	{
		ff = false;
		nn = doc->docParagraphStyles[a].name();
		for (uint b=0; b<dia->TempVorl.count(); ++b)
		{
			if (nn == dia->TempVorl[b].name())
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
				if (doc->docParagraphStyles[a].equiv(dia->TempVorl[b]))
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
							if (ne == dia->TempVorl[b].name())
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
/*
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
				CharStyle lastStyle;
				int lastStyleStart = 0;
				int lastParaStyle = -1;
				for (int e=0; e<ite->itemText.length(); ++e)
				{
					const ParagraphStyle origStyle(ite->itemText.paragraphStyle(e));
					int cabori = findParagraphStyle(doc, origStyle);
					assert (cabori >= 0 && cabori < doc->docParagraphStyles.count() );
					int cabneu = ers[cabori];
					assert (cabneu >= 0 && cabneu < dia->TempVorl.count() );
					CharStyle newStyle;
					if (cabori > 4)
					{
						if (cabneu > 0)
						{
							if (ite->itemText.charStyle(e).font() == doc->docParagraphStyles[cabori].charStyle().font())
								newStyle.setFont(dia->TempVorl[cabneu].charStyle().font());
							if (ite->itemText.charStyle(e).fontSize() == doc->docParagraphStyles[cabori].charStyle().fontSize())
								newStyle.setFontSize(dia->TempVorl[cabneu].charStyle().fontSize());
							if ((ite->itemText.charStyle(e).effects() & 1919 ) == doc->docParagraphStyles[cabori].charStyle().effects())
							{
								StyleFlag fl = static_cast<StyleFlag>(ite->itemText.charStyle(e).effects() & ~1919);
								fl |= dia->TempVorl[cabneu].charStyle().effects();
								newStyle.setEffects(fl);
							}
							if (ite->itemText.charStyle(e).fillColor() == doc->docParagraphStyles[cabori].charStyle().fillColor())
								newStyle.setFillColor(dia->TempVorl[cabneu].charStyle().fillColor());
							if (ite->itemText.charStyle(e).fillShade() == doc->docParagraphStyles[cabori].charStyle().fillShade())
								newStyle.setFillShade(dia->TempVorl[cabneu].charStyle().fillShade());
							if (ite->itemText.charStyle(e).strokeColor() == doc->docParagraphStyles[cabori].charStyle().strokeColor())
								newStyle.setStrokeColor(dia->TempVorl[cabneu].charStyle().strokeColor());
							if (ite->itemText.charStyle(e).strokeShade() == doc->docParagraphStyles[cabori].charStyle().strokeShade())
								newStyle.setStrokeShade(dia->TempVorl[cabneu].charStyle().strokeShade());
							if (ite->itemText.charStyle(e).shadowXOffset() == doc->docParagraphStyles[cabori].charStyle().shadowXOffset())
								newStyle.setShadowXOffset(dia->TempVorl[cabneu].charStyle().shadowXOffset());
							if (ite->itemText.charStyle(e).shadowYOffset() == doc->docParagraphStyles[cabori].charStyle().shadowYOffset())
								newStyle.setShadowYOffset(dia->TempVorl[cabneu].charStyle().shadowYOffset());
							if (ite->itemText.charStyle(e).outlineWidth() == doc->docParagraphStyles[cabori].charStyle().outlineWidth())
								newStyle.setOutlineWidth(dia->TempVorl[cabneu].charStyle().outlineWidth());
							if (ite->itemText.charStyle(e).underlineOffset() == doc->docParagraphStyles[cabori].charStyle().underlineOffset())
								newStyle.setUnderlineOffset(dia->TempVorl[cabneu].charStyle().underlineOffset());
							if (ite->itemText.charStyle(e).underlineWidth() == doc->docParagraphStyles[cabori].charStyle().underlineWidth())
								newStyle.setUnderlineWidth(dia->TempVorl[cabneu].charStyle().underlineWidth());
							if (ite->itemText.charStyle(e).strikethruOffset() == doc->docParagraphStyles[cabori].charStyle().strikethruOffset())
								newStyle.setStrikethruOffset(dia->TempVorl[cabneu].charStyle().strikethruOffset());
							if (ite->itemText.charStyle(e).strikethruWidth() == doc->docParagraphStyles[cabori].charStyle().strikethruWidth())
								newStyle.setStrikethruWidth(dia->TempVorl[cabneu].charStyle().strikethruWidth());
							if (ite->itemText.charStyle(e).scaleH() == doc->docParagraphStyles[cabori].charStyle().scaleH())
								newStyle.setScaleH(dia->TempVorl[cabneu].charStyle().scaleH());
							if (ite->itemText.charStyle(e).scaleV() == doc->docParagraphStyles[cabori].charStyle().scaleV())
								newStyle.setScaleV(dia->TempVorl[cabneu].charStyle().scaleV());
							if (ite->itemText.charStyle(e).baselineOffset() == doc->docParagraphStyles[cabori].charStyle().baselineOffset())
								newStyle.setBaselineOffset(dia->TempVorl[cabneu].charStyle().baselineOffset());
							if (ite->itemText.charStyle(e).tracking() == doc->docParagraphStyles[cabori].charStyle().tracking())
								newStyle.setTracking(dia->TempVorl[cabneu].charStyle().tracking());
						}
						else
						{
							newStyle = ite->itemText.defaultStyle().charStyle();
								//.cstyle & static_cast<StyleFlag>(~1919);
							//newStyle.cstyle |= static_cast<StyleFlag>(ite->TxTStyle);
						}
						if (newStyle != lastStyle || lastParaStyle != cabneu) {
							ite->itemText.applyCharStyle(lastStyleStart, e-lastStyleStart, lastStyle);
							lastStyle = newStyle;
							lastStyleStart = e;
							lastParaStyle = cabneu;
						}
						if (ite->itemText.text(e) == SpecialChars::PARSEP && cabneu >= 0) {
							ite->itemText.applyStyle(e, dia->TempVorl[cabneu]);
						}
					}
					else if (lastParaStyle >= 0) {
						ite->itemText.applyCharStyle(lastStyleStart, e-lastStyleStart, lastStyle);
						lastStyle = newStyle;
						lastStyleStart = e;
						lastParaStyle = -1;
					}
				}
				if (ite->itemText.length() > 0) {
					ite->itemText.applyCharStyle(lastStyleStart, ite->itemText.length()-lastStyleStart, lastStyle);
					if (lastParaStyle >=0 )
						ite->itemText.applyStyle(ite->itemText.length()-1, dia->TempVorl[lastParaStyle]);
				}
			}
		}
	}
 */
	if (CurrStED != NULL)
	{
		if (CurrStED->Editor->StyledText.count() != 0)
		{
			for (uint pa = 0; pa < CurrStED->Editor->StyledText.count(); ++pa)
			{
				SEditor::ChList *chars;
				chars = CurrStED->Editor->StyledText.at(pa);
				(*CurrStED->Editor->ParagStyles.at(pa)) = ers[CurrStED->Editor->ParagStyles[pa]];
				/*
				int cabneu = 0;
				for (uint e = 0; e < chars->count(); ++e)
				{
					int cabori = chars->at(e)->cab;
					assert (cabore >= 0 && cabori < doc->docParagraphStyles.count());
					cabneu = ers[cabori];
					assert (cabneu >= 0 && cabneu < dia->TempVorl.count() );
					if (cabori > 4)
					{
						if (cabneu > 0)
						{
							if (chars->at(e)->charStyle.font().scName() == doc->docParagraphStyles[cabori].charStyle().font().scName())
								chars->at(e)->charStyle.setFont(dia->TempVorl[cabneu].charStyle().font());
							if (chars->at(e)->charStyle.fontSize() == doc->docParagraphStyles[cabori].charStyle().fontSize())
								chars->at(e)->charStyle.setFontSize(dia->TempVorl[cabneu].charStyle().fontSize());
							if ((chars->at(e)->charStyle.effects() & static_cast<StyleFlag>(1919) ) == doc->docParagraphStyles[cabori].charStyle().effects())
							{
								StyleFlag fl = chars->at(e)->charStyle.effects();
								fl&= static_cast<StyleFlag>(~1919);
								fl |= dia->TempVorl[cabneu].charStyle().effects();
								chars->at(e)->charStyle.setEffects(fl);
							}
							if (chars->at(e)->charStyle.fillColor() == doc->docParagraphStyles[cabori].charStyle().fillColor())
								chars->at(e)->charStyle.setFillColor(dia->TempVorl[cabneu].charStyle().fillColor());
							if (chars->at(e)->charStyle.fillShade() == doc->docParagraphStyles[cabori].charStyle().fillShade())
								chars->at(e)->charStyle.setFillShade(dia->TempVorl[cabneu].charStyle().fillShade());
							if (chars->at(e)->charStyle.strokeColor() == doc->docParagraphStyles[cabori].charStyle().strokeColor())
								chars->at(e)->charStyle.setStrokeColor(dia->TempVorl[cabneu].charStyle().strokeColor());
							if (chars->at(e)->charStyle.strokeShade() == doc->docParagraphStyles[cabori].charStyle().strokeShade())
								chars->at(e)->charStyle.setStrokeShade(dia->TempVorl[cabneu].charStyle().strokeShade());
							if (chars->at(e)->charStyle.shadowXOffset() == doc->docParagraphStyles[cabori].charStyle().shadowXOffset())
								chars->at(e)->charStyle.setShadowXOffset(dia->TempVorl[cabneu].charStyle().shadowXOffset());
							if (chars->at(e)->charStyle.shadowYOffset() == doc->docParagraphStyles[cabori].charStyle().shadowYOffset())
								chars->at(e)->charStyle.setShadowYOffset(dia->TempVorl[cabneu].charStyle().shadowYOffset());
							if (chars->at(e)->charStyle.outlineWidth() == doc->docParagraphStyles[cabori].charStyle().outlineWidth())
								chars->at(e)->charStyle.setOutlineWidth(dia->TempVorl[cabneu].charStyle().outlineWidth());
							if (chars->at(e)->charStyle.underlineOffset() == doc->docParagraphStyles[cabori].charStyle().underlineOffset())
								chars->at(e)->charStyle.setUnderlineOffset(dia->TempVorl[cabneu].charStyle().underlineOffset());
							if (chars->at(e)->charStyle.underlineWidth() == doc->docParagraphStyles[cabori].charStyle().underlineWidth())
								chars->at(e)->charStyle.setUnderlineWidth(dia->TempVorl[cabneu].charStyle().underlineWidth());
							if (chars->at(e)->charStyle.strikethruOffset() == doc->docParagraphStyles[cabori].charStyle().strikethruOffset())
								chars->at(e)->charStyle.setStrikethruOffset(dia->TempVorl[cabneu].charStyle().strikethruOffset());
							if (chars->at(e)->charStyle.strikethruWidth() == doc->docParagraphStyles[cabori].charStyle().strikethruWidth())
								chars->at(e)->charStyle.setStrikethruWidth(dia->TempVorl[cabneu].charStyle().strikethruWidth());
							if (chars->at(e)->charStyle.scaleH() == doc->docParagraphStyles[cabori].charStyle().scaleH())
								chars->at(e)->charStyle.setScaleH(dia->TempVorl[cabneu].charStyle().scaleH());
							if (chars->at(e)->charStyle.scaleV() == doc->docParagraphStyles[cabori].charStyle().scaleV())
								chars->at(e)->charStyle.setScaleV(dia->TempVorl[cabneu].charStyle().scaleV());
							if (chars->at(e)->charStyle.baselineOffset() == doc->docParagraphStyles[cabori].charStyle().baselineOffset())
								chars->at(e)->charStyle.setBaselineOffset(dia->TempVorl[cabneu].charStyle().baselineOffset());
							if (chars->at(e)->charStyle.tracking() == doc->docParagraphStyles[cabori].charStyle().tracking())
								chars->at(e)->charStyle.setTracking(dia->TempVorl[cabneu].charStyle().tracking());
						}
						else
						{
							chars->at(e)->charStyle = ite->itemText.defaultStyle().charStyle();
						}
						chars->at(e)->cab = cabneu;
					}
				}
				 */
			}
			CurrStED->Editor->currentParaStyle = ers[CurrStED->Editor->currentParaStyle];
		}
	}
	doc->docParagraphStyles.redefine(dia->TempVorl);
	if (CurrStED != NULL)
	{
		if (CurrStED->Editor->StyledText.count() != 0)
			CurrStED->Editor->updateAll();
	}
	for (uint a=0; a<doc->docParagraphStyles.count(); ++a)
	{
		if (!doc->docParagraphStyles[a].charStyle().font().isNone())
		{
			QString nf = doc->docParagraphStyles[a].charStyle().font().scName();
			if (!doc->UsedFonts.contains(nf))
			{
				if (doc->AddFont(nf)) //, prefsManager->appPrefs.AvailFonts[nf]->Font))
				{
					int ff = FontMenu->insertItem(new FmItem(nf, prefsManager->appPrefs.AvailFonts[nf]));
					FontID.insert(ff, prefsManager->appPrefs.AvailFonts[nf].scName());
				}
				else
					doc->docParagraphStyles[a].charStyle().setFont((prefsManager->appPrefs.AvailFonts[doc->toolSettings.defFont]));
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

//CB-->Doc
void ScribusMainWindow::setNewAlignment(int a)
{
	if (HaveDoc)
	{
		doc->currentStyle = doc->docParagraphStyles[a];
		doc->itemSelection_SetParagraphStyle(a);
		propertiesPalette->setAli(a);
		PageItem *currItem = doc->m_Selection->itemAt(0);
		setTBvals(currItem);
	}
}

void ScribusMainWindow::setNewParStyle(int a)
{
	if (HaveDoc)
	{
		doc->currentStyle = doc->docParagraphStyles[a];
		doc->itemSelection_SetParagraphStyle(a);
		PageItem *currItem = doc->m_Selection->itemAt(0);
		setTBvals(currItem);
	}
}

void ScribusMainWindow::setAbsValue(int a)
{
//	doc->currentStyle = doc->docParagraphStyles[a];
	doc->currentStyle.setAlignment(a<5 ? a : 0);
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
	ColorManager* dia = new ColorManager(this, edc, doc, prefsManager->colorSetName(), prefsManager->appPrefs.CustomColorSets);
	if (dia->exec())
	{
		if (HaveDoc)
		{
			uint c;
			int d;
			QMap<QString,QString> ers;
			PageItem *ite;
			QColor tmpc;
			slotDocCh();
			doc->PageColors = dia->EditColors;
			ers = dia->replaceMap;
			if (!ers.isEmpty())
			{
				QMap<QString,QString>::Iterator it;
				for (it = ers.begin(); it != ers.end(); ++it)
				{
					if (it.key() == doc->currentStyle.charStyle().fillColor())
						doc->currentStyle.charStyle().setFillColor(it.data());
					if (it.key() == doc->currentStyle.charStyle().strokeColor())
						doc->currentStyle.charStyle().setStrokeColor(it.data());
					for (c=0; c<doc->DocItems.count(); ++c)
					{
						ite = doc->DocItems.at(c);
						if ((ite->asTextFrame()) || (ite->asPathText()))
						{
							CharStyle lastStyle;
							int lastStyleStart = 0;
							for (d=0; d<ite->itemText.length(); ++d)
							{
								CharStyle newStyle;
								if (it.key() == ite->itemText.charStyle(d).fillColor())
									newStyle.setFillColor(it.data());
								if (it.key() == ite->itemText.charStyle(d).strokeColor())
									newStyle.setStrokeColor(it.data());
								if (newStyle != lastStyle) {
									ite->itemText.applyCharStyle(lastStyleStart, d-lastStyleStart, lastStyle);
									lastStyle = newStyle;
									lastStyleStart = d;
								}
							}
							ite->itemText.applyCharStyle(lastStyleStart, ite->itemText.length()-lastStyleStart, lastStyle);
						}
						if (it.key() == ite->fillColor())
							ite->setFillColor(it.data());
						if (it.key() == ite->lineColor())
							ite->setLineColor(it.data());
						QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
						for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
						{
							if (it.key() == cstops.at(cst)->name)
								cstops.at(cst)->name = it.data();
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
							CharStyle lastStyle;
							int lastStyleStart = 0;
							for (d=0; d<ite->itemText.length(); ++d)
							{
								CharStyle newStyle;
								if (it.key() == ite->itemText.charStyle(d).fillColor())
									newStyle.setFillColor(it.data());
								if (it.key() == ite->itemText.charStyle(d).strokeColor())
									newStyle.setStrokeColor(it.data());
								if (newStyle != lastStyle) {
									ite->itemText.applyCharStyle(lastStyleStart, d-lastStyleStart, lastStyle);
									lastStyle = newStyle;
									lastStyleStart = d;
								}
								ite->itemText.applyCharStyle(lastStyleStart, ite->itemText.length()-lastStyleStart, lastStyle);
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
								cstops.at(cst)->name = it.data();
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
							CharStyle lastStyle;
							int lastStyleStart = 0;
							for (d=0; d<ite->itemText.length(); ++d)
							{
								CharStyle newStyle;
								if (it.key() == ite->itemText.charStyle(d).fillColor())
									newStyle.setFillColor(it.data());
								if (it.key() == ite->itemText.charStyle(d).strokeColor())
									newStyle.setStrokeColor(it.data());
								if (newStyle != lastStyle) {
									ite->itemText.applyCharStyle(lastStyleStart, d-lastStyleStart, lastStyle);
									lastStyle = newStyle;
									lastStyleStart = d;
								}
							}
							ite->itemText.applyCharStyle(lastStyleStart, ite->itemText.length()-lastStyleStart, lastStyle);
						}
						if (it.key() == ite->fillColor())
							ite->setFillColor(it.data());
						if (it.key() == ite->lineColor())
							ite->setLineColor(it.data());
						QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
						for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
						{
							if (it.key() == cstops.at(cst)->name)
								cstops.at(cst)->name = it.data();
						}
					}
				}
				for (it = ers.begin(); it != ers.end(); ++it)
				{
					QStringList patterns = doc->docPatterns.keys();
					for (uint c = 0; c < patterns.count(); ++c)
					{
						ScPattern pa = doc->docPatterns[patterns[c]];
						for (uint o = 0; o < pa.items.count(); o++)
						{
							PageItem *ite = pa.items.at(o);
							if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
							{
								CharStyle lastStyle;
								int lastStyleStart = 0;
								for (d=0; d<ite->itemText.length(); ++d)
								{
									CharStyle newStyle;
									if (it.key() == ite->itemText.charStyle(d).fillColor())
										newStyle.setFillColor(it.data());
									if (it.key() == ite->itemText.charStyle(d).strokeColor())
										newStyle.setStrokeColor(it.data());
									if (newStyle != lastStyle) {
										ite->itemText.applyCharStyle(lastStyleStart, d-lastStyleStart, lastStyle);
										lastStyle = newStyle;
										lastStyleStart = d;
									}
								}
								ite->itemText.applyCharStyle(lastStyleStart, ite->itemText.length()-lastStyleStart, lastStyle);
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
									cstops.at(cst)->name = it.data();
								}
							}
						}
					}
				}
			}
			doc->recalculateColors();
			doc->recalcPicturesRes();
			propertiesPalette->updateColorList();
			propertiesPalette->updateCList();
			updateColorMenu();
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

void ScribusMainWindow::slotStyleManager()
{
	styleManager->currentDoc(HaveDoc ? doc : 0);
	styleManager->setShown(!styleManager->isVisible());
}

void ScribusMainWindow::updtGradFill()
{
	if (!HaveDoc)
		return;
	VGradient vg(propertiesPalette->getFillGradient());
	doc->itemSelection_SetFillGradient(vg);
}

//CB-->Doc
void ScribusMainWindow::GetBrushPen()
{
	//What? we come back here from mpalette and then go to the view.. someones kidding
	//why this.. ugh. setActiveWindow();
	//
	if (!HaveDoc)
		return;
	view->QueryFarben();
	slotDocCh();
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
	//propertiesPalette->SetCurItem(currItem);
	view->RefreshItem(currItem);
	slotDocCh();
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
	for (uint b=0; b<doc->m_Selection->count(); ++b)
	{
		doc->m_Selection->itemAt(b)->setLocked(false);
		view->MoveItem(doc->toolSettings.dispX, doc->toolSettings.dispY, doc->m_Selection->itemAt(b));
	}
	doc->useRaster = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
}

void ScribusMainWindow::ObjektDupM()
{
	if (!HaveDoc)
		return;
	slotSelect();
	NoFrameEdit();
	MultipleDuplicate *dia = new MultipleDuplicate(doc->unitIndex(), this);
	if (dia->exec())
	{
		ItemMultipleDuplicateData mdData;
		dia->getMultiplyData(mdData);
		doc->itemSelection_MultipleDuplicate(mdData);
	}
	delete dia;
}

void ScribusMainWindow::selectItemsFromOutlines(PageItem* ite)
{
	int d = doc->Items->findRef(ite);
	selectItemsFromOutlines(ite->OwnPage, d, true);
}

void ScribusMainWindow::selectItemsFromOutlines(int Page, int Item, bool single)
{
	NoFrameEdit();
	setActiveWindow();
	view->Deselect(true);
	if ((Page != -1) && (Page != static_cast<int>(doc->currentPage()->pageNr())))
		view->GotoPage(Page);
	view->SelectItemNr(Item, true, single);
	if (doc->m_Selection->count() != 0)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
	SCFontsIterator it(prefsManager->appPrefs.AvailFonts);
	FontSub = new FontCombo(0);
	FontMenu->insertItem(FontSub);
	connect(FontSub, SIGNAL(activated(int)), this, SLOT(setItemFont2(int)));
	FontMenu->insertSeparator();
	int a=0;
	if (!HaveDoc)
	{
		a = FontMenu->insertItem(new FmItem(it.currentKey(), it.current()));
		FontMenu->setItemChecked(a, true);
		FontID.insert(a, it.current().scName());
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

void ScribusMainWindow::prefsOrg(Preferences *dia)
{
	//reset the appMode so we restore our tools shortcuts
	QString oldGUILanguage = prefsManager->guiLanguage();
	QString oldGUIStyle = prefsManager->guiStyle();
	int oldGUIFontSize = prefsManager->guiFontSize();
	//double oldDisplayScale=prefsManager->displayScale();
	dia->updatePreferences();
	prefsManager->SavePrefs();
	DocDir = prefsManager->documentDir();
//		scrapbookPalette->rebuildView();
//		scrapbookPalette->AdjustMenu();
	QString newGUILanguage = prefsManager->guiLanguage();
	if (oldGUILanguage != newGUILanguage || ScQApp->currGUILanguage()!=newGUILanguage)
		ScQApp->changeGUILanguage(newGUILanguage);
	QString newGUIStyle = prefsManager->guiStyle();
	if (oldGUIStyle != newGUIStyle)
		qApp->setStyle(QStyleFactory::create(newGUIStyle));
	int newGUIFontSize = prefsManager->guiFontSize();
	if (oldGUIFontSize != newGUIFontSize)
	{
		QFont apf = qApp->font();
		apf.setPointSize(prefsManager->appPrefs.AppFontSize);
		qApp->setFont(apf,true);
	}
	FontSub->RebuildList(0);
	propertiesPalette->Fonts->RebuildList(0);
	ScCore->getCMSProfiles();
	ScCore->recheckGS();
	SetShortCut();
}

void ScribusMainWindow::slotPrefsOrg()
{
	setAppMode(modeNormal);

	Preferences *dia = new Preferences(this);
	if (dia->exec())
	{
		prefsOrg(dia);
	}
	delete dia;
}

void ScribusMainWindow::ShowSubs()
{
	QString mess;
	if (ScCore->haveGS() != 0)
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
	guidePalette->startup();

	// init the toolbars
	fileToolBar->initVisibility();
	editToolBar->initVisibility();
	mainToolBar->initVisibility();
	pdfToolBar->initVisibility();

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
		if ( PPreview::usePostscriptPreview(currentPrinter) && ( ScCore->haveGS() != 0 ) )
		{
			QString mess = tr("Ghostscript is missing : Postscript Print Preview is not available")+"\n\n";
			QMessageBox::warning(this, CommonStrings::trWarning, mess, 1, 0, 0);
			return;
		}
		PPreview *dia = new PPreview(this, view, doc, ScCore->havePNGAlpha(), ScCore->haveTIFFSep(), currentPrinter);
		previewDinUse = true;
		connect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
		dia->exec();
		PrefsManager *prefsManager=PrefsManager::instance();
		prefsManager->appPrefs.PrPr_Mode = dia->EnableCMYK->isChecked();
		prefsManager->appPrefs.PrPr_AntiAliasing = dia->AntiAlias->isChecked();
		prefsManager->appPrefs.PrPr_Transparency = dia->AliasTr->isChecked();
		if ( ScCore->haveTIFFSep() != 0 || !dia->postscriptPreview )
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
//		prefsManager->appPrefs.Gcr_Mode = dia->EnableGCR->isChecked();
//		prefsManager->appPrefs.doOverprint = dia->EnableOverprint->isChecked();
		disconnect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
		previewDinUse = false;
		delete dia;
		QFile::remove(prefsManager->preferencesLocation()+"/tmp.ps");
		QFile::remove(prefsManager->preferencesLocation()+"/sc.png");
		QDir d(prefsManager->preferencesLocation()+"/", "sc.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			for (uint dc = 0; dc < d.count(); dc++)
			{
				QFile::remove(prefsManager->preferencesLocation() +"/" + d[dc]);
			}
		}
	}
}

void ScribusMainWindow::printPreview()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		if (scanDocument())
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
	pageNs.push_back(doc->currentPage()->pageNr()+1);
	ReOrderText(doc, view);
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QMap<QString, QMap<uint, FPointArray> > ReallyUsed;
	ReallyUsed.clear();
	doc->getUsedFonts(ReallyUsed);
	ColorList usedColors;
	doc->getUsedColors(usedColors);
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	PSLib *dd = new PSLib(false, prefsManager->appPrefs.AvailFonts, ReallyUsed, usedColors, false, true);
	if (dd != NULL)
	{
		if (dd->PS_set_file(fn))
			dd->CreatePS(doc, pageNs, false, tr("All"), spots, true, false, false, true, prefsManager->appPrefs.GCRMode, false, true);
		else
			return_value = false;
		delete dd;
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	ScCore->fileWatcher->start();
	return return_value;
}

void ScribusMainWindow::SaveAsEps()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		if (scanDocument())
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
	if (!doc->DocName.startsWith( tr("Document")))
	{
		QFileInfo fi(doc->DocName);
		fna = fi.dirPath() + "/" + getFileNameByPage(doc, doc->currentPage()->pageNr(), "eps");
	}
	else
	{
		QDir di = QDir();
		fna = di.currentDirPath() + "/" + getFileNameByPage(doc, doc->currentPage()->pageNr(), "eps");
	}
	fna = QDir::convertSeparators(fna);
	QString wdir = ".";
	QString prefsDocDir=prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		wdir = prefsManager->prefsFile->getContext("dirs")->get("eps", prefsDocDir);
	else
		wdir = prefsManager->prefsFile->getContext("dirs")->get("eps", ".");
	QString fn = CFileDialog( wdir, tr("Save as"), tr("EPS Files (*.eps);;All Files (*)"), fna, fdNone);
	if (!fn.isEmpty())
	{
		prefsManager->prefsFile->getContext("dirs")->set("eps", fn.left(fn.findRev("/")));
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
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	bool ret = PDFlib(*doc).doExport(fn, nam, Components, pageNs, thumbs);
	ScCore->fileWatcher->start();
	return ret;
}

void ScribusMainWindow::SaveAsPDF()
{
	if (doc->checkerProfiles[doc->curCheckProfile].autoCheck)
	{
		if (scanDocument())
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
	QMap<QString, int> ReallyUsed = doc->UsedFonts;
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
	PDFExportDialog dia(this, doc->DocName, ReallyUsed, view, doc->PDF_Options, doc->PDF_Options.PresentVals, ScCore->PDFXProfiles, prefsManager->appPrefs.AvailFonts, doc->unitRatio(), ScCore->PrinterProfiles);
	if (dia.exec())
	{
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		dia.updateDocOptions();
		doc->PDF_Options.firstUse = false;
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

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::AddBookMark(PageItem *ite)
{
	bookmarkPalette->BView->AddPageItem(ite);
	StoreBookmarks();
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::DelBookMark(PageItem *ite)
{
	bookmarkPalette->BView->DeleteItem(ite);
	StoreBookmarks();
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::BookMarkTxT(PageItem *ite)
{
	bookmarkPalette->BView->ChangeText(ite);
	StoreBookmarks();
}

//CB-->Doc, stop _storing_ bookmarks in the palette
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

//CB-->Doc, stop _storing_ bookmarks in the palette
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
		Boma.PageObject = ip->PageObject;
//		Boma.Seite = ip->Seite;
// 		Boma.Element = ip->Element;
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
	guidePalette->setupPage();
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
			storedPageNum = doc->currentPageNumber();
			storedViewXCoor = view->contentsX();
			storedViewYCoor = view->contentsY();
			storedViewScale = view->scale();
			MasterPagesPalette *dia = new MasterPagesPalette(this, doc, view, temp);
			//connect(dia, SIGNAL(createNew(int)), this, SLOT(slotNewMasterPage(int)));
			connect(dia, SIGNAL(removePage(int )), this, SLOT(DeletePage2(int )));
			//connect(dia, SIGNAL(loadPage(QString, int, bool)), this, SLOT(loadPage(QString, int, bool)));
			connect(dia, SIGNAL(finished()), this, SLOT(manageMasterPagesEnd()));
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
	view->setScale(storedViewScale);
	view->hideMasterPage();
	setAppMode(modeNormal);
	scrActions["editMasterPages"]->setEnabled(true);
	scrActions["fileNew"]->setEnabled(true);
	scrActions["fileOpen"]->setEnabled(true);
	scrActions["fileClose"]->setEnabled(true);
	scrActions["fileSave"]->setEnabled(true);
	scrMenuMgr->setMenuEnabled("FileOpenRecent", true);
	scrActions["fileRevert"]->setEnabled(true);
	scrActions["fileDocSetup"]->setEnabled(true);
	scrActions["filePrint"]->setEnabled(true);
	if ( ScCore->haveGS()==0 || ScCore->isWinGUI() )
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
//	doc->setMasterPageMode(false);
	pagePalette->enablePalette(true);
	pagePalette->RebuildTemp();
	ActWin->setMasterPagesPalette(NULL);
	doc->setCurrentPage(doc->DocPages.at(storedPageNum));
	view->reformPages(false);
	view->setContentsPos(static_cast<int>(storedViewXCoor * storedViewScale), static_cast<int>(storedViewYCoor * storedViewScale));
	view->DrawNew();
	pagePalette->Rebuild();
	outlinePalette->BuildTree();
//	outlinePalette->reopenTree(doc->OpenNodes);
//	slotDocCh();
}

void ScribusMainWindow::ApplyMasterPage()
{
	Q_ASSERT(!doc->masterPageMode());
	ApplyMasterPageDialog *dia = new ApplyMasterPageDialog(this);
	dia->setup(doc, doc->currentPage()->MPageNam);
	if (dia->exec())
	{
		QString masterPageName = dia->getMasterPageName();
		int pageSelection = dia->getPageSelection(); //0=current, 1=even, 2=odd, 3=all
		if (pageSelection==0) //current page only
			Apply_MasterPage(masterPageName, doc->currentPage()->pageNr(), false);
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

void ScribusMainWindow::Apply_MasterPage(QString pageName, int pageNumber, bool reb)
{
	if (!HaveDoc)
		return;
	doc->applyMasterPage(pageName, pageNumber);
	if (reb)
	{
		view->DrawNew();
		pagePalette->Rebuild();
	}
}

//CB-->Doc
void ScribusMainWindow::GroupObj(bool showLockDia)
{
	if (HaveDoc)
	{
		PageItem *currItem;
		PageItem* bb;
		double x, y, w, h;
		int t = -1; // show locked dialog only once
		QString tooltip = Um::ItemsInvolved + "\n";
		uint selectedItemCount=doc->m_Selection->count();
		if (showLockDia)
		{
			for (uint a=0; a<selectedItemCount; ++a)
			{
				if (t == -1 && doc->m_Selection->itemAt(a)->locked())
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
				currItem = doc->m_Selection->itemAt(a);
				if (currItem->locked())
				{
					for (uint c=0; c<selectedItemCount; ++c)
					{
						bb = doc->m_Selection->itemAt(c);
						bool t1=(t==1);
						bb->setLocked(t1);
						scrActions["itemLock"]->setOn(t1);
						tooltip += "\t" + currItem->getUName() + "\n";
					}
				}
			}
		}
		view->getGroupRect(&x, &y, &w, &h);
		uint lowestItem = 999999;
		uint highestItem = 0;
		for (uint a=0; a<selectedItemCount; ++a)
		{
			currItem = doc->m_Selection->itemAt(a);
			currItem->gXpos = currItem->xPos() - x;
			currItem->gYpos = currItem->yPos() - y;
			currItem->gWidth = w;
			currItem->gHeight = h;
			lowestItem = QMIN(lowestItem, currItem->ItemNr);
			highestItem = QMAX(highestItem, currItem->ItemNr);
		}
		double minx = 99999.9;
		double miny = 99999.9;
		double maxx = -99999.9;
		double maxy = -99999.9;
		for (uint ep = 0; ep < selectedItemCount; ++ep)
		{
			PageItem* currItem = doc->m_Selection->itemAt(ep);
			double lw = currItem->lineWidth() / 2.0;
			if (currItem->rotation() != 0)
			{
				FPointArray pb;
				pb.resize(0);
				pb.addPoint(FPoint(currItem->xPos()-lw, currItem->yPos()-lw));
				pb.addPoint(FPoint(currItem->width()+lw*2.0, -lw, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
				pb.addPoint(FPoint(currItem->width()+lw*2.0, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
				pb.addPoint(FPoint(-lw, currItem->height()+lw*2.0, currItem->xPos()-lw, currItem->yPos()-lw, currItem->rotation(), 1.0, 1.0));
				for (uint pc = 0; pc < 4; ++pc)
				{
					minx = QMIN(minx, pb.point(pc).x());
					miny = QMIN(miny, pb.point(pc).y());
					maxx = QMAX(maxx, pb.point(pc).x());
					maxy = QMAX(maxy, pb.point(pc).y());
				}
			}
			else
			{
				minx = QMIN(minx, currItem->xPos()-lw);
				miny = QMIN(miny, currItem->yPos()-lw);
				maxx = QMAX(maxx, currItem->xPos()-lw + currItem->width()+lw*2.0);
				maxy = QMAX(maxy, currItem->yPos()-lw + currItem->height()+lw*2.0);
			}
		}
		double gx = minx;
		double gy = miny;
		double gw = maxx - minx;
		double gh = maxy - miny;
		PageItem *high = doc->Items->at(highestItem);
		undoManager->setUndoEnabled(false);
		int z = doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, doc->toolSettings.dBrush, doc->toolSettings.dPen, true);
		PageItem *neu = doc->Items->take(z);
		doc->Items->insert(lowestItem, neu);
//		neu->Groups.push(doc->GroupCounter);
		neu->setItemName( tr("Group%1").arg(doc->GroupCounter));
		neu->AutoName = false;
		neu->isGroupControl = true;
		neu->groupsLastItem = high;
		undoManager->setUndoEnabled(true);

		QMap<int, uint> ObjOrder;
		for (uint c = 0; c < selectedItemCount; ++c)
		{
			currItem = doc->m_Selection->itemAt(c);
			ObjOrder.insert(currItem->ItemNr, c);
			int d = doc->Items->findRef(currItem);
			doc->Items->take(d);
		}
		QValueList<uint> Oindex = ObjOrder.values();
		for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
		{
			doc->Items->insert(lowestItem+1, doc->m_Selection->itemAt(Oindex[c]));
		}

		for (uint a = 0; a < doc->Items->count(); ++a)
		{
			doc->Items->at(a)->ItemNr = a;
		}
		doc->m_Selection->prependItem(neu);
		selectedItemCount=doc->m_Selection->count();
		SimpleState *ss = new SimpleState(Um::Group, tooltip);
		ss->set("GROUP", "group");
		ss->set("itemcount", selectedItemCount-1);

		for (uint a=0; a<selectedItemCount; ++a)
		{
			currItem = doc->m_Selection->itemAt(a);
			currItem->Groups.push(doc->GroupCounter);
			if (a != 0)
				ss->set(QString("item%1").arg(a), currItem->ItemNr);
		}
		doc->GroupCounter++;
		view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
		outlinePalette->BuildTree();
		slotDocCh();
		scrActions["itemAttachTextToPath"]->setEnabled(false);
		scrActions["itemGroup"]->setEnabled(false);
		scrActions["itemUngroup"]->setEnabled(true);
		undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
	}
}

//CB-->Doc
void ScribusMainWindow::UnGroupObj()
{
	if (HaveDoc)
	{
		uint docSelectionCount=doc->m_Selection->count();
		PageItem *currItem;
		uint lowestItem = 999999;
		for (uint a=0; a<docSelectionCount; ++a)
		{
			currItem = doc->m_Selection->itemAt(a);
			currItem->Groups.pop();
/*			currItem->LeftLink = 0;
			currItem->RightLink = 0;
			currItem->TopLink = 0;
			currItem->BottomLink = 0; */
			lowestItem = QMIN(lowestItem, currItem->ItemNr);
		}
		if (doc->Items->at(lowestItem)->isGroupControl)
		{
			doc->m_Selection->removeItem(doc->Items->at(lowestItem));
			doc->Items->remove(lowestItem);
			for (uint a = 0; a < doc->Items->count(); ++a)
			{
				doc->Items->at(a)->ItemNr = a;
			}
		}
		docSelectionCount = doc->m_Selection->count();
		SimpleState *ss = new SimpleState(Um::Ungroup);
		ss->set("UNGROUP", "ungroup");
		ss->set("itemcount", docSelectionCount);
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint a=0; a<docSelectionCount; ++a)
		{
			currItem = doc->m_Selection->itemAt(a);
			ss->set(QString("item%1").arg(a), currItem->ItemNr);
			ss->set(QString("tableitem%1").arg(a), currItem->isTableItem);
			tooltip += "\t" + currItem->getUName() + "\n";
			currItem->isTableItem = false;
		}
		view->Deselect(true);
		outlinePalette->BuildTree();
		slotDocCh();

		undoManager->action(this, ss, Um::SelectionGroup, Um::IGroup);
	}
}

void ScribusMainWindow::restore(UndoState* state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (ss)
	{
		if (ss->contains("GROUP"))
			restoreGrouping(ss, isUndo);
		else if (ss->contains("UNGROUP"))
			restoreUngrouping(ss, isUndo);
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
		addNewPages(wo, where, 1, doc->pageHeight, doc->pageWidth, doc->PageOri, doc->m_pageSize, true, &tmpl);
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
	int wo    = state->getInt("PAGE");
	int where = state->getInt("WHERE");
	int count = state->getInt("COUNT");
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

void ScribusMainWindow::restoreGrouping(SimpleState *state, bool isUndo)
{
	int itemCount = state->getInt("itemcount");
	view->Deselect();
	for (int i = 0; i < itemCount; ++i)
	{
		int itemNr = state->getInt(QString("item%1").arg(i));
		view->SelectItemNr(itemNr);
	}
	if (isUndo)
	{
		uint docSelectionCount=doc->m_Selection->count();
		PageItem *currItem;
		uint lowestItem = 999999;
		for (uint a=0; a<docSelectionCount; ++a)
		{
			currItem = doc->m_Selection->itemAt(a);
			lowestItem = QMIN(lowestItem, currItem->ItemNr);
		}
		if ((lowestItem > 0) && (doc->Items->at(lowestItem-1)->Groups.count() != 0))
		{
			if (doc->Items->at(lowestItem-1)->Groups.top() == doc->m_Selection->itemAt(0)->Groups.top())
			{
				view->SelectItemNr(lowestItem-1);
			}
		}
		UnGroupObj();
	}
	else
		GroupObj(false);
}

void ScribusMainWindow::restoreUngrouping(SimpleState *state, bool isUndo)
{
	int itemCount = state->getInt("itemcount");
	view->Deselect();
	for (int i = 0; i < itemCount; ++i)
	{
		int itemNr = state->getInt(QString("item%1").arg(i));
		if (isUndo)
			doc->Items->at(itemNr)->isTableItem = static_cast<bool>(state->getInt(QString("tableitem%1").arg(i)));
		view->SelectItemNr(itemNr);
	}
	if (isUndo)
		GroupObj(false);
	else
	{
		uint docSelectionCount=doc->m_Selection->count();
		PageItem *currItem;
		uint lowestItem = 999999;
		for (uint a=0; a<docSelectionCount; ++a)
		{
			currItem = doc->m_Selection->itemAt(a);
			lowestItem = QMIN(lowestItem, currItem->ItemNr);
		}
		if ((lowestItem > 0) && (doc->Items->at(lowestItem-1)->Groups.count() != 0))
		{
			if (doc->Items->at(lowestItem-1)->Groups.top() == doc->m_Selection->itemAt(0)->Groups.top())
			{
				view->SelectItemNr(lowestItem-1);
			}
		}
		UnGroupObj();
	}
}

void ScribusMainWindow::StatusPic()
{
	if (HaveDoc)
	{
		PicStatus *dia = new PicStatus(this, doc);
		connect(dia, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
		connect(dia, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
		dia->exec();
		delete dia;
	}
}

QString ScribusMainWindow::CFileDialog(QString wDir, QString caption, QString filter, QString defNa,
                                int optionFlags, bool *docom, bool *doFont, bool *doProfiles)
{
	QString retval = "";
	CustomFDialog *dia = new CustomFDialog(this, wDir, caption, filter, optionFlags);
	if (!defNa.isEmpty())
	{
		QFileInfo f(defNa);
		dia->setExtension(f.extension(true));
		dia->setZipExtension(f.extension(true) + ".gz");
		dia->setSelection(defNa);
	}
	if (optionFlags & fdDirectoriesOnly)
	{
		dia->SaveZip->setChecked(*docom);
		dia->WithFonts->setChecked(*doFont);
		dia->WithProfiles->setChecked(*doProfiles);
	}
	if (dia->exec() == QDialog::Accepted)
	{
		LoadEnc = "";
		if (!(optionFlags & fdDirectoriesOnly))
			LoadEnc = (optionFlags & fdShowCodecs) ? dia->TxCodeM->currentText() : QString("");
		else
		{
			*docom = dia->SaveZip->isChecked();
			*doFont = dia->WithFonts->isChecked();
			*doProfiles = dia->WithProfiles->isChecked();
		}
		this->repaint();
		qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
		retval = dia->selectedFile();
	}
	delete dia;
	return retval;
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
	if (doc->m_Selection->count() != 0)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
	QString objectString = ss->WriteElem(doc, view, doc->m_Selection);
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
	bool setter = !doc->layerLocked( doc->activeLayer() );
	scrActions["editPaste"]->setEnabled(((!Buffer2.isEmpty()) || (scrapbookPalette->tempBView->objectMap.count() > 0)) && (setter));
	scrMenuMgr->setMenuEnabled("EditPasteRecent", ((scrapbookPalette->tempBView->objectMap.count() > 0) && (setter)));
	scrActions["editSelectAll"]->setEnabled(setter);
	scrActions["editDeselectAll"]->setEnabled(false);
	scrMenuMgr->setMenuEnabled("Insert", setter);
	scrActions["insertFrame"]->setEnabled(setter);
	scrActions["toolsSelect"]->setEnabled(setter);
	scrActions["toolsInsertTextFrame"]->setEnabled(setter);
	scrActions["toolsInsertImageFrame"]->setEnabled(setter);
	scrActions["toolsInsertTableFrame"]->setEnabled(setter);
	scrActions["toolsInsertShape"]->setEnabled(setter);
	scrActions["toolsInsertLine"]->setEnabled(setter);
	scrActions["toolsInsertBezier"]->setEnabled(setter);
	scrActions["toolsInsertFreehandLine"]->setEnabled(setter);
	scrActions["toolsInsertPolygon"]->setEnabled(setter);
	scrActions["toolsPDFPushButton"]->setEnabled(setter);
	scrActions["toolsPDFTextField"]->setEnabled(setter);
	scrActions["toolsPDFCheckBox"]->setEnabled(setter);
	scrActions["toolsPDFComboBox"]->setEnabled(setter);
	scrActions["toolsPDFListBox"]->setEnabled(setter);
	scrActions["toolsPDFAnnotText"]->setEnabled(setter);
	scrActions["toolsPDFAnnotLink"]->setEnabled(setter);
}

void ScribusMainWindow::showLayer()
{
	view->DrawNew();
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
//	propertiesPalette->fillLangCombo(LangTransl);
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

/* PV - guides refactoring
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
}*/

void ScribusMainWindow::ImageEffects()
{
	if (HaveDoc)
	{
		if (doc->m_Selection->count() != 0)
		{
			PageItem *currItem = doc->m_Selection->itemAt(0);
			EffectsDialog* dia = new EffectsDialog(this, currItem, doc);
			if (dia->exec())
				doc->itemSelection_ApplyImageEffects(dia->effectsList);
			delete dia;
		}
	}
}

QString ScribusMainWindow::Collect(bool compress, bool withFonts, const bool withProfiles, const QString& )
{
	CollectForOutput c(doc, withFonts, withProfiles, compress);
	return c.collect();
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

bool ScribusMainWindow::scanDocument()
{
	DocumentChecker docChecker;
	return docChecker.checkDocument(doc);
}

void ScribusMainWindow::HaveRaster(bool art)
{
	if (art && doc->m_Selection->count() != 0)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
	if (doc->m_Selection->count() != 0)
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
			doc = ActWin->doc();
			view = ActWin->view();
			doc->setModified(false);
			if (doc->hasName)
			{
				std::cout << "Saving: " << doc->DocName+".emergency" << std::endl;
				doc->autoSaveTimer->stop();
				//disconnect(ActWin, SIGNAL(Schliessen()), ScMW, SLOT(DoFileClose()));
				FileLoader fl(doc->DocName+".emergency");
				fl.SaveFile(doc->DocName+".emergency", doc, 0);
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
		if (doc->m_Selection->count() != 0)
		{
			PageItem *currItem = doc->m_Selection->itemAt(0);
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
	PageItem *currItem = doc->m_Selection->itemAt(0);
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
	if (doc->m_Selection->count() != 0)
	{
		QString imageEditorExecutable=prefsManager->imageEditorExecutable();
		if (ExternalApp != 0)
		{
			QMessageBox::information(this, tr("Information"), "<qt>" + tr("The program %1 is already running!").arg(imageEditorExecutable) + "</qt>", 1, 0, 0);
			return;
		}
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
			cmd.append(QDir::convertSeparators(currItem->Pfile));
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
	if ((HaveDoc) && (doc->m_Selection->count() != 0))
	{
		PageItem *currItem = doc->m_Selection->itemAt(0);
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
		uint docSelectionCount=doc->m_Selection->count();
		if (docSelectionCount == 1)
			undoManager->showObject(doc->m_Selection->itemAt(0)->getUId());
		else if (docSelectionCount == 0)
			undoManager->showObject(doc->currentPage()->getUId());
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
	if ((HaveDoc) && (doc->m_Selection->count() == 1))
	{
		PageItem *pageItem = doc->m_Selection->itemAt(0);
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
	if ((HaveDoc) && (doc->m_Selection->count() == 1))
	{
		PageItem *pageItem = doc->m_Selection->itemAt(0);
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
			uint docSelectionCount=doc->m_Selection->count();
			if (!colorName.isNull() && docSelectionCount > 0)
			{
				for (uint i = 0; i < docSelectionCount; ++i)
				{
					PageItem *currItem=doc->m_Selection->itemAt(i);
					if (currItem!=NULL)
					{
						if ((m->stateAfter() & Qt::ControlButton) && (currItem->asTextFrame() || currItem->asPathText()))
							doc->ItemTextBrush(colorName); //Text colour
						else
						if (m->stateAfter() & Qt::AltButton) //Line colour
							doc->ItemPen(colorName);
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
	LoremManager *m = new LoremManager(doc, this, "m", true, 0);
	if (prefsManager->appPrefs.useStandardLI)
		m->insertLoremIpsum("loremipsum.xml", prefsManager->appPrefs.paragraphsLI);
	else
		m->exec();
	delete(m);
	m=NULL;
}

void ScribusMainWindow::languageChange()
{
	if (ScCore->initialized())
	{
		CommonStrings::languageChange();
		//Update actions
		if (actionManager!=NULL)
		{
			actionManager->languageChange();
			ScCore->pluginManager->languageChange();
			initKeyboardShortcuts();
		}
		//Update menu texts
		if (scrMenuMgr!=NULL && !scrMenuMgr->empty())
		{
			scrMenuMgr->setMenuText("File", tr("&File"));
			scrMenuMgr->setMenuText(recentFileMenuName, tr("Open &Recent"));
			scrMenuMgr->setMenuText(recentPasteMenuName, tr("Paste Recent"));
			scrMenuMgr->setMenuText("FileImport", tr("&Import"));
			scrMenuMgr->setMenuText("FileExport", tr("&Export"));
			scrMenuMgr->setMenuText("Edit", tr("&Edit"));
			scrMenuMgr->setMenuText("EditContents", tr("Contents"));
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
			scrMenuMgr->setMenuText("InsertLigature", tr("Liga&ture"));
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

void ScribusMainWindow::updateColorMenu(QProgressBar* progressBar)
{
	disconnect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
	ColorMenC->clear();
	ColorMenC->insertItem(CommonStrings::NoneColor);
	if (HaveDoc)
	{
		if (doc->toolSettings.dBrush == CommonStrings::NoneColor)
			ColorMenC->setCurrentItem(0);
		int a = 1;
		ColorList::Iterator itend=doc->PageColors.end();
		for (ColorList::Iterator it = doc->PageColors.begin(); it != itend; ++it)
		{
			ColorMenC->insertSmallItem( doc->PageColors[it.key()], it.key() );
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
	ActWin->setCaption(QDir::convertSeparators(newCaption));
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
					loadDoc( fi.absFilePath() );
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
	if (HaveDoc && (currItem=doc->m_Selection->itemAt(0))!=NULL)
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
				contentsBuffer.ItemX=imageItem->xPos();
				contentsBuffer.ItemY=imageItem->yPos();
				contentsBuffer.inputProfile=imageItem->IProfile;
				contentsBuffer.useEmbedded=imageItem->UseEmbedded;
				contentsBuffer.renderingIntent=imageItem->IRender;
			}
		}
	}
}

void ScribusMainWindow::slotEditPasteContents(int absolute)
{
	PageItem *currItem=NULL;
	if (HaveDoc && !contentsBuffer.contentsFileName.isEmpty() && (currItem=doc->m_Selection->itemAt(0))!=NULL)
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
				if (absolute==0)
					imageItem->setImageXYOffset(contentsBuffer.LocalX, contentsBuffer.LocalY);
				else
					imageItem->setImageXYOffset(
					((contentsBuffer.ItemX-imageItem->xPos()) / contentsBuffer.LocalScX)+contentsBuffer.LocalX,
					((contentsBuffer.ItemY-imageItem->yPos()) / contentsBuffer.LocalScY)+contentsBuffer.LocalY);
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

void ScribusMainWindow::slotInsertFrame()
{
	if (HaveDoc)
	{
		if (doc->m_Selection->count() != 0)
			view->Deselect(false);
		InsertAFrame *dia = new InsertAFrame(this, doc);
		if (dia->exec())
		{
			InsertAFrameData iafData;
			dia->getNewFrameProperties(iafData);
			doc->itemAddUserFrame(iafData);
		}
		delete dia;
	}
}

void ScribusMainWindow::PutToPatterns()
{
	QString patternName = "Pattern_"+doc->m_Selection->itemAt(0)->itemName();
	patternName = patternName.stripWhiteSpace().simplifyWhiteSpace().replace(" ", "_");
	ScriXmlDoc *ss = new ScriXmlDoc();
	QString objectString = ss->WriteElem(doc, view, doc->m_Selection);
	uint ac = doc->Items->count();
	bool savedAlignGrid = doc->useRaster;
	bool savedAlignGuides = doc->SnapGuides;
	doc->useRaster = false;
	doc->SnapGuides = false;
	slotElemRead(objectString, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
	doc->useRaster = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	uint ae = doc->Items->count();
	ScPattern pat = ScPattern();
	pat.setDoc(doc);
	PageItem* currItem = doc->Items->at(ac);
	pat.pattern = currItem->DrawObj_toImage();
	pat.width = currItem->gWidth;
	pat.height = currItem->gHeight;
	for (uint as = ac; as < ae; ++as)
	{
		pat.items.append(doc->Items->take(ac));
	}
	doc->addPattern(patternName, pat);
	propertiesPalette->updateColorList();
	delete ss;
}

void ScribusMainWindow::managePatterns()
{
	if (HaveDoc)
	{
		bool undoState = UndoManager::undoEnabled();
		undoManager->setUndoEnabled(false);
		QMap<QString, ScPattern> docPatterns(doc->docPatterns);
		PatternDialog *dia = new PatternDialog(this, &docPatterns, doc, this);
		if (dia->exec())
		{
			doc->setPatterns(dia->dialogPatterns);
			propertiesPalette->updateColorList();
			view->DrawNew();
			undoManager->setUndoEnabled(undoState);
		}
		delete dia;
		undoManager->setUndoEnabled(undoState);
	}
}
