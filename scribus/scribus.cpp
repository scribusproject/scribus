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

#include <QApplication>
#include <QByteArray>
#include <QCloseEvent>
#include <QColor>
#include <QColorDialog>
#include <QCursor>
#include <QDesktopWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QEventLoop>
#include <QFrame>
#include <QIcon>
#include <QInputDialog>
#include <QKeyEvent>
#include <QKeySequence>
#include <QLabel>
#include <QList>
#include <QLocale>
#include <QMouseEvent>
#include <QPixmap>
#include <QRegExp>
#include <QStyleFactory>
#include <QTableWidget>
#include <QTextCodec>
#include <QTranslator>
#include <QWheelEvent>


#include <cstdio>
#include <cstdlib>
#include <cmath>
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




#include "about.h"
#include "aboutplugins.h"
#include "actionmanager.h"
#include "aligndistribute.h"
#include "annot.h"
#include "annota.h"
#include "applytemplatedialog.h"
#include "arrowchooser.h"
#include "autoform.h"
#include "bookmarkpalette.h"
#include "canvasmode.h"
#include "charselect.h"
#include "checkDocument.h"
#include "cmsprefs.h"
#include "collect4output.h"
#include "colorcombo.h"
#include "colorm.h"
#include "commonstrings.h"
#include "contextmenu.h"
#include "cpalette.h"
#include "customfdialog.h"
#include "delpages.h"
#include "desaxe/digester.h"
#include "desaxe/saxXML.h"
#include "desaxe/simple_actions.h"
#include "docinfo.h"
#include "docitemattrprefs.h"
#include "documentchecker.h"
#include "documentinformation.h"
#include "effectsdialog.h"
#include "fileloader.h"
#include "filewatcher.h"
#include "fontcombo.h"
#include "fontprefs.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "gtgettext.h"
#include "guidemanager.h"
#include "helpbrowser.h"
#include "hruler.h"
#include "hyphenator.h"
#include "hysettings.h"
#include "imageinfodialog.h"
#include "insertaframe.h"
#include "inspage.h"
#include "javadocs.h"
#include "langmgr.h"
#include "layers.h"
#include "loremipsum.h"
#include "marginwidget.h"
#include "margindialog.h"
#include "masterpagepalette.h"
//#include "measurements.h"
#include "menumanager.h"
#include "mergedoc.h"
#include "movepage.h"
#include "multipleduplicate.h"
#include "newfile.h"
#include "newtemp.h"
#include "nodeeditpalette.h"
#include "outlinepalette.h"
#include "page.h"
#include "pageitem_imageframe.h"
#include "pageitem_latexframe.h"
#include "pageitem_textframe.h"
#include "pageitemattributes.h"
#include "pagelayout.h"
#include "pagepalette.h"
#include "pageselector.h"
#include "pagesize.h"
#include "patterndialog.h"
#include "pdflib.h"
#include "pdfoptions.h"
#include "pdfopts.h"
#include "picstatus.h"
#include "pluginmanager.h"
#include "plugins/formatidlist.h"
#include "polygonwidget.h"
#include "prefs.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "preview.h"
#include "printdialog.h"
#include "propertiespalette.h"
#include "pslib.h"
#include "query.h"
#include "reformdoc.h"
#include "resourcecollection.h"
#include "sccolorengine.h"
#include "sccombobox.h"
#include "scgtplugin.h"
#include "scmessagebox.h"
#include "scpaths.h"
#include "scprintengine_ps.h"
#include "scraction.h"
#include "scrapbookpalette.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribuswin.h"
#include "search.h"
#include "selection.h"
#include "serializer.h"
#include "smlinestyle.h"
#include "smtextstyles.h"
#include "splash.h"
#include "stencilreader.h"
#include "story.h"
#include "stylemanager.h"
#include "tabcheckdoc.h"
#include "tabguides.h"
#include "tabmanager.h"
#include "tabpdfoptions.h"
#include "tabtools.h"
#include "tabtypography.h"
#include "text/nlsconfig.h"
#include "tocgenerator.h"
#include "tocindexprefs.h"
#include "ui/copypagetomasterpagedialog.h"
#include "ui/edittoolbar.h"
#include "ui/filetoolbar.h"
#include "ui/modetoolbar.h"
#include "ui/pdftoolbar.h"
#include "undogui.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "urllauncher.h"
#include "util.h"
#include "util_formats.h"
#include "util_ghostscript.h"
#include "util_icon.h"
#include "vruler.h"


#if defined(_WIN32)
#include "scdocoutput_ps2.h"
#include "scprintengine_gdi.h"
#endif

using namespace std;

bool previewDinUse;
bool printDinUse;

QString DocDir;


extern ScribusQApp* ScQApp;
extern bool emergencyActivated;


ScribusMainWindow::ScribusMainWindow()
{
	actionManager=0;
	scrMenuMgr=0;
	prefsManager=0;
	formatsManager=0;
	UrlLauncher::instance();
	mainWindowStatusLabel=0;
	ExternalApp=0;
#ifdef Q_WS_MAC
	noIcon = loadIcon("noicon.xpm");
#endif
}

/*
static QCoreApplication::EventFilter origEventFilter = 0;

bool reportFocusChanges(void *message, long *result)
{
	unsigned* data = static_cast<unsigned*>(message);
	if (QApplication::focusWidget())
		qDebug() << QApplication::applicationFilePath() << reinterpret_cast<void*>(QApplication::focusWidget()) << typeid(*QApplication::focusWidget()).name() << QApplication::focusWidget()->objectName() << message << data[0] << data[1] << data[2] << data[3] << data[4] << data[5] << data[6] << data[7];
	else
		qDebug() << QApplication::applicationFilePath() << "no focus" << message << data[0] << data[1] << data[2] << data[3] << data[4] << data[5] << data[6] << data[7];
	return origEventFilter && origEventFilter(message, result);
}
*/

/*
 * retval 0 - ok, 1 - no fonts, ...
 */
int ScribusMainWindow::initScMW(bool primaryMainWindow)
{
	int retVal=0;

	QByteArray stylesheet;
	if (loadRawText(ScPaths::getApplicationDataDir() + "/stylesheet.css", stylesheet))
	{
		qApp->setStyleSheet(QString(stylesheet));
	}

//	origEventFilter = qApp->setEventFilter(reportFocusChanges);
	
	previewDinUse = false;
	printDinUse = false;
	internalCopy = false;
	CurrStED = NULL;
	setWindowTitle( tr("Scribus " VERSION));
	setAttribute(Qt::WA_KeyCompression, false);
	setWindowIcon(loadIcon("AppIcon.png"));
	scrActionGroups.clear();
	scrActions.clear();
	scrRecentFileActions.clear();
	scrRecentPasteActions.clear();
	scrWindowsActions.clear();
	scrLayersActions.clear();
	scrMenuMgr = new MenuManager(menuBar());
	prefsManager = PrefsManager::instance();
	formatsManager = FormatsManager::instance();
	objectSpecificUndo = false;

	undoManager = UndoManager::instance();
	PrefsContext *undoPrefs = prefsManager->prefsFile->getContext("undo");
	undoManager->setUndoEnabled(undoPrefs->getBool("enabled", true));
	tocGenerator = new TOCGenerator();


	initDefaultValues();

	initStatusBar();

	qApp->processEvents();

	BuFromApp = false;

	actionManager = new ActionManager(this);
	actionManager->init(this);
//	if (primaryMainWindow)
//		ScCore->setSplashStatus( tr("Applying User Shortcuts") );
//	prefsManager->applyLoadedShortCuts();
//	initKeyboardShortcuts();
	initMenuBar();
	initToolBars();
 	ScCore->pluginManager->setupPluginActions(this);
 	ScCore->pluginManager->languageChange();
	if (primaryMainWindow)
		ScCore->setSplashStatus( tr("Applying User Shortcuts") );
	prefsManager->applyLoadedShortCuts();
	initKeyboardShortcuts();
	
	resize(610, 600);
	wsp = new QWorkspace( this );
	setCentralWidget( wsp );
	connect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
	//Connect windows cascade and tile actions to the workspace after its created. Only depends on wsp created.
	connect( scrActions["windowsCascade"], SIGNAL(triggered()) , wsp, SLOT(cascade()) );
	connect( scrActions["windowsTile"], SIGNAL(triggered()) , wsp, SLOT(tile()) );
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

	scrActions["helpTooltips"]->setChecked(prefsManager->appPrefs.showToolTips);
	scrActions["stickyTools"]->setChecked(prefsManager->appPrefs.stickyTools);
	ToggleTips();
	propertiesPalette->setFontSize();
	if (scrActions["SaveAsDocumentTemplate"])
		scrActions["SaveAsDocumentTemplate"]->setEnabled(false);

	connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), this, SLOT(removeRecent(QString)));
	connect(this, SIGNAL(TextStyle(const ParagraphStyle&)), propertiesPalette, SLOT(updateStyle(const ParagraphStyle&)));
	connect(this, SIGNAL(TextIFont(QString)), propertiesPalette, SLOT(setFontFace(QString)));
	connect(this, SIGNAL(TextISize(double)), propertiesPalette, SLOT(setSize(double)));
	connect(this, SIGNAL(TextUSval(double)), propertiesPalette, SLOT(setExtra(double)));
	connect(this, SIGNAL(TextStil(int)), propertiesPalette, SLOT(setStil(int)));
	connect(this, SIGNAL(TextScale(double)), propertiesPalette, SLOT(setTScale(double)));
	connect(this, SIGNAL(TextScaleV(double)), propertiesPalette, SLOT(setTScaleV(double)));
	connect(this, SIGNAL(TextBase(double)), propertiesPalette, SLOT(setTBase(double)));
	connect(this, SIGNAL(TextShadow(double, double )), propertiesPalette, SLOT(setShadowOffs(double, double )));
	connect(this, SIGNAL(TextOutline(double)), propertiesPalette, SLOT(setOutlineW(double)));
	connect(this, SIGNAL(TextUnderline(double, double)), propertiesPalette, SLOT(setUnderline(double, double)));
	connect(this, SIGNAL(TextStrike(double, double)), propertiesPalette, SLOT(setStrike(double, double)));
	connect(this, SIGNAL(TextFarben(QString, QString, double, double)), propertiesPalette, SLOT(setActFarben(QString, QString, double, double)));
	connect(ClipB, SIGNAL(dataChanged()), this, SLOT(ClipChange()));
//	connect(ClipB, SIGNAL(selectionChanged()), this, SLOT(ClipChange()));
	setAcceptDrops(true);
	QCoreApplication::instance()->installEventFilter(this);
	
	return retVal;
}


ScribusMainWindow::~ScribusMainWindow()
{
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
	undoManager->registerGui(uWidget);
	modeToolBar = new ModeToolBar(this);
	pdfToolBar = new PDFToolBar(this);

	addScToolBar(fileToolBar, "ToolBar-File");
	addScToolBar(editToolBar, "ToolBar-Edit");
	addScToolBar(modeToolBar, "ToolBar-Tools");
	addScToolBar(pdfToolBar, "ToolBar-PDF_Tools");

	connect(modeToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarTools"], SLOT(setChecked(bool)));
	connect(scrActions["toolsToolbarPDF"], SIGNAL(toggled(bool)), pdfToolBar, SLOT(setVisible(bool)));
	connect(pdfToolBar, SIGNAL(visibilityChanged(bool)), scrActions["toolsToolbarPDF"], SLOT(setChecked(bool)));
	connect(scrActions["toolsToolbarTools"], SIGNAL(toggled(bool)), modeToolBar, SLOT(setVisible(bool)) );
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
	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrActions.begin(); it!=scrActions.end(); ++it )
	{
		if ((ScrAction*)(it.value())!=NULL)
		{
			QString accelerator=it.value()->shortcut();
			prefsManager->setKeyEntry(it.key(), it.value()->cleanMenuText(), accelerator,0);
		}
		//else
		//	qDebug(it.key());
		//qDebug(QString("|-\n|%1||%2||%3").arg(it.key()).arg(it.value()->cleanMenuText()).arg(QString(it.data()->accel())));
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

	//CB dont need this until we have a doc...
	//propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
	propertiesPalette->Fonts->RebuildList(0);
	propertiesPalette->installEventFilter(this);
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
	scrapbookPalette->installEventFilter(this);
	pagePalette = new PagePalette(this);
	connect( scrActions["toolsPages"], SIGNAL(toggled(bool)) , pagePalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsPages"], SIGNAL(toggled(bool)) , this, SLOT(setPagePalette(bool)) );
	connect( pagePalette, SIGNAL(paletteShown(bool)), scrActions["toolsPages"], SLOT(setChecked(bool)));
	pagePalette->installEventFilter(this);
	bookmarkPalette = new BookPalette(this);
	connect( scrActions["toolsBookmarks"], SIGNAL(toggled(bool)) , bookmarkPalette, SLOT(setPaletteShown(bool)) );
	connect( bookmarkPalette, SIGNAL(paletteShown(bool)), scrActions["toolsBookmarks"], SLOT(setChecked(bool)));
	bookmarkPalette->installEventFilter(this);
//	measurementPalette = new Measurements(this);
//	connect( scrActions["toolsMeasurements"], SIGNAL(toggled(bool)) , measurementPalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsMeasurements"], SIGNAL(toggledData(bool, int)) , this, SLOT(setAppModeByToggle(bool, int)) );
//	connect( measurementPalette, SIGNAL(paletteShown(bool)), scrActions["toolsMeasurements"], SLOT(setChecked(bool)));
//	measurementPalette->installEventFilter(this);
//	measurementPalette->hide();
	docCheckerPalette = new CheckDocument(this, false);
	connect( scrActions["toolsPreflightVerifier"], SIGNAL(toggled(bool)) , docCheckerPalette, SLOT(setPaletteShown(bool)) );
	connect( scrActions["toolsPreflightVerifier"], SIGNAL(toggled(bool)) , this, SLOT(docCheckToggle(bool)) );
	connect( docCheckerPalette, SIGNAL(paletteShown(bool)), scrActions["toolsPreflightVerifier"], SLOT(setChecked(bool)));
	connect( docCheckerPalette, SIGNAL(paletteShown(bool)), this, SLOT(docCheckToggle(bool)));
	docCheckerPalette->installEventFilter(this);
	docCheckerPalette->hide();

	alignDistributePalette = new AlignDistributePalette(this, "AlignDistributePalette", false);
	connect( scrActions["toolsAlignDistribute"], SIGNAL(toggled(bool)) , alignDistributePalette, SLOT(setPaletteShown(bool)) );
	connect( alignDistributePalette, SIGNAL(paletteShown(bool)), scrActions["toolsAlignDistribute"], SLOT(setChecked(bool)));
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
	SMCharacterStyle *tmpCS = new SMCharacterStyle();
	styleManager->addStyle(new SMParagraphStyle(tmpCS->tmpStyles()));
	styleManager->addStyle(tmpCS);
	connect( scrActions["editStyles"], SIGNAL(toggled(bool)), styleManager, SLOT(setPaletteShown(bool)) );
	connect( styleManager, SIGNAL(paletteShown(bool)), scrActions["editStyles"], SLOT(setChecked(bool)));
	styleManager->installEventFilter(this);

	connect(docCheckerPalette, SIGNAL(selectElement(int, int)), this, SLOT(selectItemsFromOutlines(int, int)));
	connect(docCheckerPalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(docCheckerPalette, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
	connect(outlinePalette, SIGNAL(selectElement(int, int, bool)), this, SLOT(selectItemsFromOutlines(int, int, bool)));
	connect(outlinePalette, SIGNAL(selectPage(int)), this, SLOT(selectPagesFromOutlines(int)));
	connect(outlinePalette, SIGNAL(selectMasterPage(QString)), this, SLOT(manageMasterPages(QString)));
	connect(propertiesPalette->paraStyleCombo, SIGNAL(newStyle(const QString&)), this, SLOT(setNewParStyle(const QString&)));
	connect(propertiesPalette->charStyleCombo, SIGNAL(newStyle(const QString&)), this, SLOT(setNewCharStyle(const QString&)));
//	connect(propertiesPalette, SIGNAL(EditLSt()), this, SLOT(slotEditLineStyles()));
	connect(nodePalette, SIGNAL(Schliessen()), this, SLOT(slotSelect()));
	connect(nodePalette, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(layerPalette, SIGNAL(LayerChanged()), this, SLOT(showLayer()));

	connect(bookmarkPalette->BView, SIGNAL(MarkMoved()), this, SLOT(StoreBookmarks()));
	connect(bookmarkPalette->BView, SIGNAL(changed()), this, SLOT(slotDocCh()));
	connect(bookmarkPalette->BView, SIGNAL(SelectElement(PageItem *)), this, SLOT(selectItemsFromOutlines(PageItem *)));
	// guides
	connect(scrActions["pageManageGuides"], SIGNAL(toggled(bool)), guidePalette, SLOT(setPaletteShown(bool)));
	connect(guidePalette, SIGNAL(paletteShown(bool)), scrActions["pageManageGuides"], SLOT(setChecked(bool)));
	// char palette
	connect(scrActions["insertGlyph"], SIGNAL(toggled(bool)), charPalette, SLOT(setPaletteShown(bool)));
	connect(charPalette, SIGNAL(paletteShown(bool)), scrActions["insertGlyph"], SLOT(setChecked(bool)));
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
								 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
	if (t == QMessageBox::Ok)
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
// 	scrMenuMgr->addMenuItem(scrActions["fileImportText2"], "FileImport");
	scrMenuMgr->addMenuItem(scrActions["fileImportAppendText"], "FileImport");
	scrMenuMgr->addMenuItem(scrActions["fileImportImage"], "FileImport");
	scrMenuMgr->createMenu("FileExport", tr("&Export"), "File");
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
	scrMenuMgr->createMenu(recentPasteMenuName, tr("Paste Recent"), "Edit");
	scrMenuMgr->createMenu("EditContents", tr("Contents"), "Edit");
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
	scrMenuMgr->addMenuItem(scrActions["editEditWithLatexEditor"], "Edit");
	scrMenuMgr->addMenuSeparator("Edit");
	scrMenuMgr->addMenuItem(scrActions["editColors"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editPatterns"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editStyles"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editMasterPages"], "Edit");
	scrMenuMgr->addMenuItem(scrActions["editJavascripts"], "Edit");
	scrActions["editUndoAction"]->setEnabled(false);
	scrActions["editRedoAction"]->setEnabled(false);
	scrActions["editActionMode"]->setEnabled(true);
	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	scrActions["editPaste"]->setEnabled(false);
	scrMenuMgr->setMenuEnabled("EditPasteRecent", false);
	scrMenuMgr->setMenuEnabled("EditContents", false);
	scrActions["editCopyContents"]->setEnabled(false);
	scrActions["editPasteContents"]->setEnabled(false);
	scrActions["editPasteContentsAbs"]->setEnabled(false);
	scrActions["editClearContents"]->setEnabled(false);
	scrActions["editSelectAll"]->setEnabled(false);
	scrActions["editDeselectAll"]->setEnabled(false);
	scrActions["editSearchReplace"]->setEnabled(false);
	scrActions["editPatterns"]->setEnabled(false);
 	scrActions["editStyles"]->setEnabled(false);
	scrActions["editMasterPages"]->setEnabled(false);
	scrActions["editJavascripts"]->setEnabled(false);
	scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
	scrActions["editEditWithImageEditor"]->setEnabled(false);
	scrActions["editEditWithLatexEditor"]->setEnabled(false);

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
	scrMenuMgr->addMenuItem(scrActions["itemAdjustFrameToImage"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemExtendedImageProperties"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemUpdateImage"], "Item");
	scrMenuMgr->createMenu("ItemPreviewSettings", tr("Preview Settings"), "Item");
	scrMenuMgr->addMenuItem(scrActions["itemImageIsVisible"], "ItemPreviewSettings");
	scrMenuMgr->addMenuSeparator("ItemPreviewSettings");
	scrMenuMgr->addMenuItem(scrActions["itemPreviewLow"], "ItemPreviewSettings");
	scrMenuMgr->addMenuItem(scrActions["itemPreviewNormal"], "ItemPreviewSettings");
	scrMenuMgr->addMenuItem(scrActions["itemPreviewFull"], "ItemPreviewSettings");
	scrMenuMgr->addMenuSeparator("Item");
	scrMenuMgr->addMenuItem(scrActions["itemAttributes"], "Item");
	scrMenuMgr->createMenu("ItemPDFOptions", tr("&PDF Options"));
	scrMenuMgr->addMenuToMenu("ItemPDFOptions", "Item");
	scrMenuMgr->addMenuItem(scrActions["itemPDFIsAnnotation"], "ItemPDFOptions");
	scrMenuMgr->addMenuItem(scrActions["itemPDFIsBookmark"], "ItemPDFOptions");
	scrMenuMgr->addMenuItem(scrActions["itemPDFAnnotationProps"], "ItemPDFOptions");
	scrMenuMgr->addMenuItem(scrActions["itemPDFFieldProps"], "ItemPDFOptions");
	//scrMenuMgr->createMenu("ItemShapes", tr("&Shape"), "Item");
	scrMenuMgr->createMenu("ItemConvertTo", tr("C&onvert To"), "Item");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToBezierCurve"], "ItemConvertTo");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToImageFrame"], "ItemConvertTo");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToOutlines"], "ItemConvertTo");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToPolygon"], "ItemConvertTo");
	scrMenuMgr->addMenuItem(scrActions["itemConvertToTextFrame"], "ItemConvertTo");

	scrMenuMgr->addMenuItem(scrActions["itemAttachTextToPath"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemDetachTextFromPath"], "Item");
//	scrMenuMgr->createMenu("ItemPathOps", tr("Path Tools"), "Item");
	scrMenuMgr->addMenuItem(scrActions["itemCombinePolygons"], "Item");
	scrMenuMgr->addMenuItem(scrActions["itemSplitPolygons"], "Item");
	//scrMenuMgr->setMenuEnabled("ItemShapes", false);
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
	scrMenuMgr->addMenuItem(scrActions["toolsInsertLatexFrame"], "Insert");
	scrMenuMgr->addMenuSeparator("Insert");
	scrMenuMgr->addMenuItem(scrActions["stickyTools"], "Insert");
	scrMenuMgr->addMenuSeparator("Insert");
	scrMenuMgr->addMenuItem(scrActions["insertGlyph"], "Insert");

	scrMenuMgr->createMenu("InsertChar", tr("&Character"), "Insert");
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

	scrMenuMgr->createMenu("InsertQuote", tr("&Quote"), "Insert");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteApostrophe"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteStraight"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteSingleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteSingleRight"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteDoubleLeft"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteDoubleRight"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteSingleReversed"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteDoubleReversed"], "InsertQuote");
	scrMenuMgr->addMenuSeparator("InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteLowSingleComma"], "InsertQuote");
	scrMenuMgr->addMenuItem(scrActions["unicodeQuoteLowDoubleComma"], "InsertQuote");
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

	scrMenuMgr->createMenu("InsertSpace", tr("S&paces && Breaks"), "Insert");
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
	scrMenuMgr->addMenuItem(scrActions["unicodeZerowidthSpace"], "InsertSpace");

	scrMenuMgr->createMenu("InsertLigature", tr("Liga&ture"), "Insert");
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
	scrMenuMgr->addMenuItem(scrActions["viewFitWidth"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit50"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit75"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit100"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit200"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewFit400"], "View");
	scrMenuMgr->addMenuSeparator("View");
	scrMenuMgr->addMenuItem(scrActions["viewFitPreview"], "View");
	scrMenuMgr->addMenuSeparator("View");
	scrMenuMgr->addMenuItem(scrActions["viewShowMargins"], "View");
	scrMenuMgr->addMenuItem(scrActions["viewShowBleeds"], "View");
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
	connect(scrMenuMgr->getLocalPopupMenu("Extras"), SIGNAL(aboutToShow()), this, SLOT(extrasMenuAboutToShow()));

	//Window menu
	 scrMenuMgr->createMenu("Windows", tr("&Windows"), QString::null, true);
	connect(scrMenuMgr->getLocalPopupMenu("Windows"), SIGNAL(aboutToShow()), this, SLOT(windowsMenuAboutToShow()));
	addDefaultWindowMenuItems();

	//Help menu
	scrMenuMgr->createMenu("Help", tr("&Help"));
	scrMenuMgr->addMenuItem(scrActions["helpManual"], "Help");
	scrMenuMgr->addMenuItem(scrActions["helpManual2"], "Help");
	scrMenuMgr->addMenuSeparator("Help");
	scrMenuMgr->addMenuItem(scrActions["helpTooltips"], "Help");
	scrMenuMgr->addMenuSeparator("Help");
	scrMenuMgr->addMenuItem(scrActions["helpOnlineWWW"], "Help");
	scrMenuMgr->addMenuItem(scrActions["helpOnlineDocs"], "Help");
	scrMenuMgr->addMenuItem(scrActions["helpOnlineWiki"], "Help");
	scrMenuMgr->createMenu("HelpOnlineTutorials", tr("Online &Tutorials"), "Help");
	scrMenuMgr->addMenuItem(scrActions["helpOnlineTutorial1"], "HelpOnlineTutorials");
	scrMenuMgr->addMenuSeparator("Help");
	scrMenuMgr->addMenuItem(scrActions["helpCheckUpdates"], "Help");
	scrMenuMgr->addMenuSeparator("Help");
	scrMenuMgr->addMenuItem(scrActions["helpAboutScribus"], "Help");
	scrMenuMgr->addMenuItem(scrActions["helpAboutPlugins"], "Help");
	scrMenuMgr->addMenuItem(scrActions["helpAboutQt"], "Help");

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
	menuBar()->addSeparator();
	scrMenuMgr->addMenuToMenuBar("Help");

	//Alignment menu
	scrMenuMgr->createMenu("Alignment", tr("&Alignment"));
	scrMenuMgr->addMenuItem(scrActions["alignLeft"], "Alignment");
	scrMenuMgr->addMenuItem(scrActions["alignCenter"], "Alignment");
	scrMenuMgr->addMenuItem(scrActions["alignRight"], "Alignment");
	scrMenuMgr->addMenuItem(scrActions["alignBlock"], "Alignment");
	scrMenuMgr->addMenuItem(scrActions["alignForced"], "Alignment");
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
	mainWindowStatusLabel = new QLabel( "           ", statusBar());
	mainWindowProgressBar = new QProgressBar(statusBar());
	mainWindowProgressBar->setAlignment(Qt::AlignHCenter);
	mainWindowProgressBar->setFixedWidth( 100 );
	mainWindowProgressBar->reset();
	mainWindowXPosLabel = new QLabel( "X-Pos:", statusBar());
	mainWindowYPosLabel = new QLabel( "Y-Pos:", statusBar());
	mainWindowXPosDataLabel = new QLabel( "        ", statusBar());
	mainWindowYPosDataLabel = new QLabel( "        ", statusBar());

	statusBar()->addPermanentWidget(mainWindowStatusLabel, 6);
	statusBar()->addPermanentWidget(mainWindowProgressBar, 0);
	statusBar()->addPermanentWidget(mainWindowXPosLabel, 0);
	statusBar()->addPermanentWidget(mainWindowXPosDataLabel, 1);
	statusBar()->addPermanentWidget(mainWindowYPosLabel, 0);
	statusBar()->addPermanentWidget(mainWindowYPosDataLabel, 1);
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


//AV to be replaced with Selection::update and listener in PropertiesPalette
void ScribusMainWindow::setTBvals(PageItem *currItem)
{
	if (currItem->itemText.length() != 0)
	{
//		int ChPos = qMin(currItem->CPos, static_cast<int>(currItem->itemText.length()-1));
		const ParagraphStyle& currPStyle(currItem->currentStyle());
		setAbsValue(currPStyle.alignment());
		propertiesPalette->setParStyle(currPStyle.parent());
		propertiesPalette->setCharStyle(currItem->currentCharStyle().parent());
		doc->currentStyle = currItem->currentStyle();
		doc->currentStyle.charStyle() = currItem->currentCharStyle();
		emit TextStyle(doc->currentStyle);
		// to go: (av)
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
		view->contentsWheelEvent(w);
/*		int wheelVal=prefsManager->mouseWheelValue();
		if ((w->orientation() != Qt::Vertical) || ( w->modifiers() & Qt::ShiftModifier ))
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
		w->accept();*/
	}
}

//Special keys assigned to actions are stolen by the action and not passed to
//keyPressEvent so process them here.
void ScribusMainWindow::specialActionKeyEvent(const QString& actionName, int unicodevalue)
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

						currItem->itemText.insertChars(currItem->CPos, QString(QChar(unicodevalue)), true);
						currItem->CPos += 1;
//						currItem->Tinput = true;
						currItem->update();
					}
					else if (actionName=="unicodeSmartHyphen") //ignore the char as we use an attribute if the text item, for now.
					{
						// this code is currently dead since unicodeSmartHyphen
						// doesnt have unicodevalue == -1 any more
						if (currItem->CPos-1>0)
						{
#if 0
							StyleFlag fl = currItem->itemText.item(qMax(currItem->CPos-1,0))->effects();
							fl |= ScStyle_HyphenationPossible;
							currItem->itemText.item(qMax(currItem->CPos-1,0))->setEffects(fl);
#else
							currItem->itemText.insertChars(currItem->CPos, QString(SpecialChars::SHYPHEN), true);
							currItem->CPos += 1;
#endif
//							currItem->Tinput = true;
							currItem->update();
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
	if (e->type() == QEvent::ToolTip)
	{
		if (prefsManager->appPrefs.showToolTips)
			return false;
		else
			return true;
	}
	if ( e->type() == QEvent::KeyPress ) {
		QKeyEvent *k = (QKeyEvent *)e;
		int keyMod=0;
		if (k->modifiers() & Qt::ShiftModifier)
			keyMod |= Qt::SHIFT;
		if (k->modifiers() & Qt::ControlModifier)
			keyMod |= Qt::CTRL;
		if (k->modifiers() & Qt::AltModifier)
			keyMod |= Qt::ALT;

		QKeySequence currKeySeq = QKeySequence(k->key() | keyMod);
		if (QString(currKeySeq).isNull())
			return false;
		retVal=true;
		//Palette actions
		if (currKeySeq == scrActions["specialToggleAllPalettes"]->shortcut())
			scrActions["specialToggleAllPalettes"]->activate(QAction::Trigger);
		else
// CB These were moved to ActionManager via the setShortcutContext(Qt::ApplicationShortcut) calls, leaving for notes for now
// 		if (currKeySeq == scrActions["toolsProperties"]->accel())
// 			scrActions["toolsProperties"]->toggle();
// 		else
// 		if (currKeySeq == scrActions["toolsOutline"]->accel())
// 			scrActions["toolsOutline"]->toggle();
// 		else
// 		if (currKeySeq == scrActions["toolsScrapbook"]->accel())
// 			scrActions["toolsScrapbook"]->toggle();
// 		else
// 		if (currKeySeq == scrActions["toolsLayers"]->accel())
// 			scrActions["toolsLayers"]->toggle();
// 		else
// 		if (currKeySeq == scrActions["toolsPages"]->accel())
// 			scrActions["toolsPages"]->toggle();
// 		else
// 		if (currKeySeq == scrActions["toolsBookmarks"]->accel())
// 			scrActions["toolsBookmarks"]->toggle();
// 		else
// 		if (currKeySeq == scrActions["toolsActionHistory"]->accel())
// 			scrActions["toolsActionHistory"]->toggle();
// 		else
// 		if (currKeySeq == scrActions["toolsPreflightVerifier"]->accel())
// 			scrActions["toolsPreflightVerifier"]->toggle();
// 		else
// 		if (currKeySeq == scrActions["toolsAlignDistribute"]->accel())
// 			scrActions["toolsAlignDistribute"]->toggle();
// 		else
		//Edit actions
		if (currKeySeq == scrActions["editStyles"]->shortcut())
			scrActions["editStyles"]->toggle();
		else
		if (currKeySeq == scrActions["editUndoAction"]->shortcut() && scrActions["editUndoAction"]->isEnabled())
			scrActions["editUndoAction"]->activate(QAction::Trigger);
		else
		if (currKeySeq == scrActions["editRedoAction"]->shortcut() && scrActions["editRedoAction"]->isEnabled())
			scrActions["editRedoAction"]->activate(QAction::Trigger);
		else
		//Other actions
		if (currKeySeq == scrActions["fileQuit"]->shortcut())
			scrActions["fileQuit"]->activate(QAction::Trigger);
		else
		//Zoom actions
		if (currKeySeq == scrActions["toolsZoomIn"]->shortcut())
			scrActions["toolsZoomIn"]->activate(QAction::Trigger);
		else
		if (currKeySeq == scrActions["toolsZoomOut"]->shortcut())
			scrActions["toolsZoomOut"]->activate(QAction::Trigger);
		else
			retVal=false;
	}
	else
		retVal=false;
	//Return false to pass event to object
	return retVal;
}


//AV -> CanvasMode
void ScribusMainWindow::keyPressEvent(QKeyEvent *k)
{
	QWidgetList windows;
	QWidget* w = NULL;
	int kk = k->key();
	QString uc = k->text();
// 	QString cr, Tcha, Twort;
	if (HaveDoc)
	{
		if ((doc->appMode == modeMagnifier) && (kk == Qt::Key_Shift))
		{
			qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
			return;
		}
	}
	if (keyrep)
		return;
	keyrep = true;
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
		keyrep = false;
		PageItem *currItem;
		if (doc->m_Selection->count() != 0)
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
						doc->Items->removeAt(currItem->ItemNr);
					}
					else
						view->Deselect(false);
					break;
				case modeEdit:
					break;
				case modeLinkFrames:
				case modeUnlinkFrames:
				case modeRotation:
				case modeEditGradientVectors:
				case modeCopyProperties:
					view->Deselect(false);
				case modePanning:
					break;
				case modeDrawBezierLine:
					currItem->PoLine.resize(currItem->PoLine.size()-2);
					if (currItem->PoLine.size() < 4)
					{
						view->Deselect(false);
						doc->Items->removeAt(currItem->ItemNr);
					}
					else
					{
						doc->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false);
						currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1.0)));
						doc->AdjustItemSize(currItem);
						currItem->ContourLine = currItem->PoLine.copy();
						currItem->ClipEdited = true;
						currItem->FrameType = 3;
						slotDocCh();
					}
					view->FirstPoly = true;
					break;
				default:
					view->Deselect(false);
					doc->Items->removeAt(currItem->ItemNr);
					break;
			}
		}
		doc->DragP = false;
		doc->leaveDrag = false;
		view->stopAllDrags();
		doc->SubMode = -1;
		doc->ElemToLink = NULL;
		slotSelect();
		return;
	}
	Qt::KeyboardModifiers buttonModifiers = k->modifiers();
	/**If we have a doc and we are not changing the page or zoom level in the status bar */
	if ((HaveDoc) && (!view->zoomSpinBox->hasFocus()) && (!view->pageSelector->hasFocus()))
	{
		//Show our context menu
		QKeySequence currKeySeq = QKeySequence(kk | keyMod);
		if (currKeySeq.matches(scrActions["viewShowContextMenu"]->shortcut()) == QKeySequence::ExactMatch)
		{
			ContextMenu* cmen=NULL;
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			if (doc->m_Selection->count() == 0)
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

		if ((doc->appMode != modeEdit) && (doc->m_Selection->count() == 0))
		{
			int pg;
			int wheelVal = prefsManager->mouseWheelValue();
			if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
				wheelVal = qMax(qRound(wheelVal / 10.0), 1);
			switch (kk)
			{
			case Qt::Key_Space:
				keyrep = false;
				if (doc->appMode == modePanning)
					view->requestMode(modeNormal);
				else
					view->requestMode(modePanning);
				return;
				break;
			case Qt::Key_PageUp:
				if (doc->masterPageMode())
					view->scrollBy(0, -prefsManager->mouseWheelValue());
				else
				{
					pg = doc->currentPageNumber();
					if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
						pg--;
					else
						pg -= doc->pageSets[doc->currentPageLayout].Columns;
					if (pg > -1)
						view->GotoPage(pg);
				}
				keyrep = false;
				return;
				break;
			case Qt::Key_PageDown:
				if (doc->masterPageMode())
					view->scrollBy(0, prefsManager->mouseWheelValue());
				else
				{
					pg = doc->currentPageNumber();
					if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
						pg++;
					else
						pg += doc->pageSets[doc->currentPageLayout].Columns;
					if (pg < static_cast<int>(doc->Pages->count()))
						view->GotoPage(pg);
				}
				keyrep = false;
				return;
				break;
			case Qt::Key_Left:
				view->scrollBy(-wheelVal, 0);
				keyrep = false;
				return;
				break;
			case Qt::Key_Right:
				view->scrollBy(wheelVal, 0);
				keyrep = false;
				return;
				break;
			case Qt::Key_Up:
				view->scrollBy(0, -wheelVal);
				keyrep = false;
				return;
				break;
			case Qt::Key_Down:
				view->scrollBy(0, wheelVal);
				keyrep = false;
				return;
				break;
			case Qt::Key_Tab:
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
					outlinePalette->buildReopenVals();
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
				if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.1;
				else if (!(buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=10.0;
				else if ((buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.01;

				moveBy/=doc->unitRatio();//Lets allow movement by the current doc ratio, not only points
			}
			else
			{
				if ((buttonModifiers & Qt::ShiftModifier) && !(buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.1/doc->unitRatio();
				else if (!(buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=1.0/doc->unitRatio();
				else if ((buttonModifiers & Qt::ShiftModifier) && (buttonModifiers & Qt::ControlModifier) && !(buttonModifiers & Qt::AltModifier))
					moveBy=0.01/doc->unitRatio();
			}
			bool resizing=((buttonModifiers & Qt::AltModifier) && !(buttonModifiers & Qt::ControlModifier));
			bool resizingsmaller=(resizing && (buttonModifiers & Qt::ShiftModifier));
			double resizeBy=1.0;
			//CB with control locked out due to the requirement of moveby of 0.01, we cannot support
			//resizeby 10 units unless we move to supporting modifier keys that most people dont have.
			//if (buttonModifiers & Qt::ControlModifier)
			//	resizeBy*=10.0;
			resizeBy/=doc->unitRatio();
			if (resizingsmaller)
				resizeBy*=-1.0;


			PageItem *currItem = doc->m_Selection->itemAt(0);
			switch (doc->appMode)
			{
			case modeNormal:
			case modeEditClip:
				switch (kk)
				{
				case Qt::Key_Backspace:
				case Qt::Key_Delete:
					doc->itemSelection_DeleteItem();
					break;
				case Qt::Key_PageUp:
					if (!currItem->locked())
					{
						view->RaiseItem();
					}
					break;
				case Qt::Key_PageDown:
					if (!currItem->locked())
					{
						view->LowerItem();
					}
					break;
				case Qt::Key_Left:
					if (!currItem->locked())
					{
						if (!resizing)
						{
							if ((doc->appMode == modeEditClip) && (doc->nodeEdit.hasNodeSelected()))
							{
								int storedClRe = doc->nodeEdit.ClRe;
								if ((doc->nodeEdit.SelNode.count() != 0) && (doc->nodeEdit.EdPoints))
								{
									for (int itm = 0; itm < doc->nodeEdit.SelNode.count(); ++itm)
									{
										FPoint np;
										int clRe = doc->nodeEdit.SelNode.at(itm);
										if ((clRe != 0) || ((clRe == 0) && (doc->nodeEdit.SelNode.count() == 1)))
										{
											if (doc->nodeEdit.isContourLine)
												np = currItem->ContourLine.point(clRe);
											else
												np = currItem->PoLine.point(clRe);
											doc->nodeEdit.ClRe = clRe;
											np = np - FPoint(moveBy, 0);
											doc->nodeEdit.moveClipPoint(currItem, np);
										}
									}
								}
								doc->nodeEdit.ClRe = storedClRe;
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
							if(doc->nodeEdit.isContourLine)
								view->TransformPoly(10, 0, resizeBy/unitGetRatioFromIndex(doc->unitIndex()));
							else
							{
								if (resizingsmaller)
								{
									currItem->Sizing = false;
									doc->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr, true);
								}
								else
								{
									doc->MoveItem(-resizeBy, 0, currItem, false);
									currItem->moveImageXYOffsetBy(resizeBy/currItem->imageXScale(), 0);
									currItem->Sizing = false;
									doc->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr, true);
								}
 							}
						}
						currItem->update();
						slotDocCh();
					}
					break;
				case Qt::Key_Right:
					if (!currItem->locked())
					{
						if (!resizing)
						{
							if ((doc->appMode == modeEditClip) && (doc->nodeEdit.hasNodeSelected()))
							{
								int storedClRe = doc->nodeEdit.ClRe;
								if ((doc->nodeEdit.SelNode.count() != 0) && (doc->nodeEdit.EdPoints))
								{
									for (int itm = 0; itm < doc->nodeEdit.SelNode.count(); ++itm)
									{
										FPoint np;
										int clRe = doc->nodeEdit.SelNode.at(itm);
										if ((clRe != 0) || ((clRe == 0) && (doc->nodeEdit.SelNode.count() == 1)))
										{
											if (doc->nodeEdit.isContourLine)
												np = currItem->ContourLine.point(clRe);
											else
												np = currItem->PoLine.point(clRe);
											doc->nodeEdit.ClRe = clRe;
											np = np + FPoint(moveBy, 0);
											doc->nodeEdit.moveClipPoint(currItem, np);
										}
									}
								}
								doc->nodeEdit.ClRe = storedClRe;
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
							if(doc->nodeEdit.isContourLine)
								view->TransformPoly(11, 0, resizeBy/unitGetRatioFromIndex(doc->unitIndex()));
							else
							{
								if (resizingsmaller)
								{
									doc->MoveItem(-resizeBy, 0, currItem, false);
									currItem->moveImageXYOffsetBy(resizeBy/currItem->imageXScale(), 0);
									currItem->Sizing = false;
									doc->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr, true);
								}
								else
								{
									currItem->Sizing = false;
									doc->SizeItem(currItem->width()+resizeBy, currItem->height(), currItem->ItemNr, true);
								}
							}
						}
						currItem->update();
						slotDocCh();
					}
					break;
				case Qt::Key_Up:
					if (!currItem->locked())
					{
						if (!resizing)
						{
							if ((doc->appMode == modeEditClip) && (doc->nodeEdit.hasNodeSelected()))
							{
								int storedClRe = doc->nodeEdit.ClRe;
								if ((doc->nodeEdit.SelNode.count() != 0) && (doc->nodeEdit.EdPoints))
								{
									for (int itm = 0; itm < doc->nodeEdit.SelNode.count(); ++itm)
									{
										FPoint np;
										int clRe = doc->nodeEdit.SelNode.at(itm);
										if ((clRe != 0) || ((clRe == 0) && (doc->nodeEdit.SelNode.count() == 1)))
										{
											if (doc->nodeEdit.isContourLine)
												np = currItem->ContourLine.point(clRe);
											else
												np = currItem->PoLine.point(clRe);
											doc->nodeEdit.ClRe = clRe;
											np = np - FPoint(0, moveBy);
											doc->nodeEdit.moveClipPoint(currItem, np);
										}
									}
								}
								doc->nodeEdit.ClRe = storedClRe;
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
							if(doc->nodeEdit.isContourLine)
								view->TransformPoly(12, 0, resizeBy/unitGetRatioFromIndex(doc->unitIndex()));
							else
							{
								if (resizingsmaller)
								{
									currItem->Sizing = false;
									doc->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr, true);
								}
								else
								{
									doc->MoveItem(0, -resizeBy, currItem, false);
									currItem->moveImageXYOffsetBy(0, resizeBy/currItem->imageYScale());
									currItem->Sizing = false;
									doc->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr, true);
								}
							}
						}
						currItem->update();
						slotDocCh();
					}
					break;
				case Qt::Key_Down:
					if (!currItem->locked())
					{
						if (!resizing)
						{
							if ((doc->appMode == modeEditClip) && (doc->nodeEdit.hasNodeSelected()))
							{
								int storedClRe = doc->nodeEdit.ClRe;
								if ((doc->nodeEdit.SelNode.count() != 0) && (doc->nodeEdit.EdPoints))
								{
									for (int itm = 0; itm < doc->nodeEdit.SelNode.count(); ++itm)
									{
										FPoint np;
										int clRe = doc->nodeEdit.SelNode.at(itm);
										if ((clRe != 0) || ((clRe == 0) && (doc->nodeEdit.SelNode.count() == 1)))
										{
											if (doc->nodeEdit.isContourLine)
												np = currItem->ContourLine.point(clRe);
											else
												np = currItem->PoLine.point(clRe);
											doc->nodeEdit.ClRe = clRe;
											np = np - FPoint(0, -moveBy);
											doc->nodeEdit.moveClipPoint(currItem, np);
										}
									}
								}
								doc->nodeEdit.ClRe = storedClRe;
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
							if(doc->nodeEdit.isContourLine)
								view->TransformPoly(13, 0, resizeBy/unitGetRatioFromIndex(doc->unitIndex()));
							else
							{
								if (resizingsmaller)
								{
									doc->MoveItem(0, -resizeBy, currItem, false);
									currItem->moveImageXYOffsetBy(0, resizeBy/currItem->imageYScale());
									currItem->Sizing = false;
									doc->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr, true);
								}
								else
								{
									currItem->Sizing = false;
									doc->SizeItem(currItem->width(), currItem->height()+resizeBy, currItem->ItemNr, true);
								}
							}
						}
						currItem->update();
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
						case Qt::Key_Left:
							dX=-moveBy;
							break;
						case Qt::Key_Right:
							dX=moveBy;
							break;
						case Qt::Key_Up:
							dY=-moveBy;
							break;
						case Qt::Key_Down:
							dY=moveBy;
							break;
					}
					if (dX!=0.0 || dY!=0.0)
					{
						currItem->moveImageInFrame(dX, dY);
						currItem->update();
					}*/
				}
//FIXME:av				view->oldCp = currItem->CPos;
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
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
			_arrowKeyDown = true;
	}
	keyrep = false;
}

void ScribusMainWindow::keyReleaseEvent(QKeyEvent *k)
{
	//Exit out of panning mode if Control is release while the right mouse button is pressed
	if (HaveDoc)
	{
		if ((doc->appMode == modePanning) && (k->key() == Qt::Key_Control) && (QApplication::mouseButtons() & Qt::RightButton))
			view->requestMode(modeNormal);
	}
	if (HaveDoc)
	{
		if (doc->appMode == modeMagnifier)
			qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
	}
	if (k->isAutoRepeat() || !_arrowKeyDown)
		return;
	switch(k->key())
	{
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
			_arrowKeyDown = false;
			if ((HaveDoc) && (!view->zoomSpinBox->hasFocus()) && (!view->pageSelector->hasFocus()))
			{
				int docSelectionCount=doc->m_Selection->count();
				if ((docSelectionCount != 0) && (doc->appMode == modeEditClip) && (doc->nodeEdit.hasNodeSelected()))
				{
					PageItem *currItem = doc->m_Selection->itemAt(0);
					double xposOrig = currItem->xPos();
					double yposOrig = currItem->yPos();
					doc->AdjustItemSize(currItem);
					if (!doc->nodeEdit.isContourLine)
						currItem->ContourLine.translate(xposOrig - currItem->xPos(),yposOrig - currItem->yPos());
					currItem->update();
				}
				for (int i = 0; i < docSelectionCount; ++i)
					doc->m_Selection->itemAt(i)->checkChanges(true);
				if (docSelectionCount > 1 && view->groupTransactionStarted())
					undoManager->commit();
			}
			break;
	}
}

void ScribusMainWindow::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
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
			slotSelect();
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
//	measurementPalette->hide();
	docCheckerPalette->hide();
	undoPalette->hide();
	alignDistributePalette->hide();
	guidePalette->hide();
	charPalette->hide();

	// Clean up plugins, THEN save prefs to disk
	ScCore->pluginManager->cleanupPlugins();
	if (!prefsManager->appPrefs.persistentScrapbook)
		scrapbookPalette->CleanUpTemp();
	prefsManager->appPrefs.RecentScrapbooks.clear();
	prefsManager->appPrefs.RecentScrapbooks = scrapbookPalette->getOpenScrapbooks();
	if (!emergencyActivated)
		prefsManager->SavePrefs();
	UndoManager::deleteInstance();
	PrefsManager::deleteInstance();
	FormatsManager::deleteInstance();
	UrlLauncher::deleteInstance();
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	qApp->exit(0);
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
	bool docSet = false;
	PrefsContext* docContext = prefsManager->prefsFile->getContext("docdirs", false);
	NewDoc* dia = new NewDoc(this, RecentDocs, true);
	if (dia->exec())
	{
		if (dia->tabSelected == 0)
		{
			int facingPages = dia->choosenLayout;
			int firstPage = dia->firstPage->currentIndex();
			docSet = dia->startDocSetup->isChecked();
			double topMargin = dia->marginGroup->top();
			double bottomMargin = dia->marginGroup->bottom();
			double leftMargin = dia->marginGroup->left();
			double rightMargin = dia->marginGroup->right();
			double columnDistance = dia->Dist;
			double pageWidth = dia->pageWidth;
			double pageHeight = dia->pageHeight;
			double numberCols = dia->numberOfCols->value();
			bool autoframes = dia->autoTextFrame->isChecked();
			int orientation = dia->Orient;
			int pageCount=dia->pageCountSpinBox->value();
			QString pagesize;
			if (dia->pageSizeComboBox->currentText() == CommonStrings::trCustomPageSize)
				pagesize = CommonStrings::customPageSize;
			else
			{
				PageSize ps2(dia->pageSizeComboBox->currentText());
				pagesize = ps2.name();
			}
			doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->unitOfMeasureComboBox->currentIndex(), firstPage, orientation, 1, pagesize, true, pageCount);
			doc->pageSets[facingPages].FirstPage = firstPage;
			doc->bleeds.Bottom = dia->bleedBottom;
			doc->bleeds.Top = dia->bleedTop;
			doc->bleeds.Left = dia->bleedLeft;
			doc->bleeds.Right = dia->bleedRight;
			HaveNewDoc();
			doc->reformPages(true);
		}
		else
		{
			if (dia->tabSelected == 1)
			{
				QString fileName(dia->selectedFile);
				if (!fileName.isEmpty())
				{
					docContext->set("docsopen", fileName.left(fileName.lastIndexOf("/")));
					loadDoc(fileName);
				}
			}
			else
			{
				QString fileName(dia->recentDocListBox->currentItem()->text());
				if (!fileName.isEmpty())
					loadRecent(ScPaths::separatorsToSlashes(fileName));
			}
		}
	}
	prefsManager->setShowStartupDialog(!dia->startUpDialog->isChecked());
	delete dia;
	mainWindowStatusLabel->setText( tr("Ready"));
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
		int facingPages = dia->choosenLayout;
		int firstPage = dia->firstPage->currentIndex();
		docSet = dia->startDocSetup->isChecked();
		double topMargin = dia->marginGroup->top();
		double bottomMargin = dia->marginGroup->bottom();
		double leftMargin = dia->marginGroup->left();
		double rightMargin = dia->marginGroup->right();
		double columnDistance = dia->Dist;
		double pageWidth = dia->pageWidth;
		double pageHeight = dia->pageHeight;
		double numberCols = dia->numberOfCols->value();
		bool autoframes = dia->autoTextFrame->isChecked();
		int orientation = dia->Orient;
		int pageCount=dia->pageCountSpinBox->value();
		QString pagesize;
		if (dia->pageSizeComboBox->currentText() == CommonStrings::trCustomPageSize)
			pagesize = CommonStrings::customPageSize;
		else
		{
			PageSize ps2(dia->pageSizeComboBox->currentText());
			pagesize = ps2.name();
		}
		if (doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->unitOfMeasureComboBox->currentIndex(), firstPage, orientation, 1, pagesize, true, pageCount))
		{
			doc->pageSets[facingPages].FirstPage = firstPage;
			doc->bleeds.Bottom = dia->bleedBottom;
			doc->bleeds.Top = dia->bleedTop;
			doc->bleeds.Left = dia->bleedLeft;
			doc->bleeds.Right = dia->bleedRight;
			mainWindowStatusLabel->setText( tr("Ready"));
			HaveNewDoc();
			doc->reformPages(true);
			retVal = true;
		}
	}
	delete dia;
	if (docSet)
		slotDocSetup();
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
	int createCount=qMax(pageCount,1);
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
		outlinePalette->buildReopenVals();
	undoManager->setUndoEnabled(false);
	MarginStruct margins(topMargin, leftMargin, bottomMargin, rightMargin);
	DocPagesSetup pagesSetup(pageArrangement, firstPageLocation, firstPageNumber, orientation, autoTextFrames, columnDistance, columnCount);
	QString newDocName( tr("Document")+"-"+QString::number(DocNr));
	ScribusDoc *tempDoc = new ScribusDoc();
	if (requiresGUI)
		doc=tempDoc;
	//tempDoc = new ScribusDoc(newDocName, unitindex, pagesize, margins, pagesSetup);
	tempDoc->setLoading(true);
	outlinePalette->setDoc(tempDoc);
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
	tempDoc->docHyphenator->ignoredWords = prefsManager->appPrefs.ignoredWords;
	tempDoc->docHyphenator->specialWords = prefsManager->appPrefs.specialWords;
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
		wsp->addWindow(w);
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
		tempView->cmsToolbarButton->setChecked(tempDoc->HasCMS);
		undoManager->switchStack(tempDoc->DocName);
		styleManager->setDoc(tempDoc);
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
	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrWindowsActions.begin(); it!=scrWindowsActions.end(); ++it )
		scrMenuMgr->removeMenuItem((*it), "Windows");
	scrWindowsActions.clear();
	addDefaultWindowMenuItems();
	QWidgetList windows = wsp->windowList();
	bool windowsListNotEmpty=!windows.isEmpty();
	scrActions["windowsCascade"]->setEnabled(windowsListNotEmpty);
	scrActions["windowsTile"]->setEnabled(windowsListNotEmpty);
	if (windowsListNotEmpty)
	{
		int windowCount=static_cast<int>(windows.count());
		if (windowCount>1)
			scrMenuMgr->addMenuSeparator("Windows");
		for ( int i = 0; i < windowCount; ++i )
		{
			QString docInWindow(windows.at(i)->windowTitle());
			scrWindowsActions.insert(docInWindow, new ScrAction( ScrAction::Window, QPixmap(), QPixmap(), docInWindow, QKeySequence(), this, i));
			scrWindowsActions[docInWindow]->setToggleAction(true);
			connect( scrWindowsActions[docInWindow], SIGNAL(triggeredData(int)), this, SLOT(windowsMenuActivated(int)) );
			if (windowCount>1)
				scrMenuMgr->addMenuItem(scrWindowsActions[docInWindow], "Windows");
			scrWindowsActions[docInWindow]->setChecked(wsp->activeWindow() == windows.at(i));
		}
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
		for (int i = 0; i < doc->Items->count(); ++i)
		{
			if ((doc->Items->at(i)->itemType() == PageItem::ImageFrame) && (!doc->Items->at(i)->asLatexFrame()))
			{
				enablePicManager = true;
				break;
			}
		}
	}
	scrActions["extrasManagePictures"]->setEnabled(enablePicManager);
}

void ScribusMainWindow::newActWin(QWidget *w)
{
	if (w == NULL)
	{
		ActWin = NULL;
		return;
	}
	if (doc!=0 && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	ScribusWin* scw = (ScribusWin*)w;
	if (scw && scw->doc())
		if (!scw->doc()->hasGUI())
			return;
	ActWin = scw;
	if (ActWin->doc()==NULL)
		return;

	if (doc != NULL)
	{
		if ((HaveDoc) && (doc != ActWin->doc()))
			outlinePalette->buildReopenVals();
	}
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
	connect(view, SIGNAL(signalGuideInformation(int, double)), alignDistributePalette, SLOT(setGuide(int, double)));
	if (ScCore->usingGUI())
	{
		connect(doc->m_Selection, SIGNAL(selectionIsMultiple(bool)), propertiesPalette, SLOT( setMultipleSelection(bool)));
		connect(doc->m_Selection, SIGNAL(selectionIsMultiple(bool)), actionManager, SLOT( handleMultipleSelections(bool)));
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
		view->requestMode(doc->appMode);
	}
	view->setFocus();
	wsp->setScrollBarsEnabled(!(w->isMaximized()));
	scrActions["viewShowMargins"]->setChecked(doc->guidesSettings.marginsShown);
	scrActions["viewShowBleeds"]->setChecked(doc->guidesSettings.showBleed);
	scrActions["viewShowFrames"]->setChecked(doc->guidesSettings.framesShown);
	scrActions["viewShowLayerMarkers"]->setChecked(doc->guidesSettings.layerMarkersShown);
	scrActions["viewShowGrid"]->setChecked(doc->guidesSettings.gridShown);
	scrActions["viewShowGuides"]->setChecked(doc->guidesSettings.guidesShown);
	scrActions["viewShowColumnBorders"]->setChecked(doc->guidesSettings.colBordersShown);
	scrActions["viewShowBaseline"]->setChecked(doc->guidesSettings.baseShown);
	scrActions["viewShowImages"]->setChecked(doc->guidesSettings.showPic);
	scrActions["viewShowTextChain"]->setChecked(doc->guidesSettings.linkShown);
	scrActions["viewShowTextControls"]->setChecked(doc->guidesSettings.showControls);
	scrActions["viewShowRulers"]->setChecked(doc->guidesSettings.rulersShown);
	scrActions["viewRulerMode"]->setChecked(doc->guidesSettings.rulerMode);
	scrActions["extrasGenerateTableOfContents"]->setEnabled(doc->hasTOCSetup());
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
		if (doc->m_Selection->count() != 0)
		{
			HaveNewSel(doc->m_Selection->itemAt(0)->itemType());
			doc->m_Selection->itemAt(0)->emitAllToGUI();
		}
		else
			HaveNewSel(-1);
	}
	tocGenerator->setDoc(doc);
	styleManager->setDoc(doc);
}

void ScribusMainWindow::windowsMenuActivated( int id )
{
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
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
		qApp->processEvents();
		doc->recalcPicturesRes(true);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		setStatusBarInfoText("");
		mainWindowProgressBar->reset();
	}
	propertiesPalette->Fonts->RebuildList(doc);
	scrActions["viewShowMargins"]->setChecked(doc->guidesSettings.marginsShown);
	scrActions["viewShowBleeds"]->setChecked(doc->guidesSettings.showBleed);
	scrActions["viewShowFrames"]->setChecked(doc->guidesSettings.framesShown);
	scrActions["viewShowLayerMarkers"]->setChecked(doc->guidesSettings.layerMarkersShown);
	scrActions["viewShowGrid"]->setChecked(doc->guidesSettings.gridShown);
	scrActions["viewShowGuides"]->setChecked(doc->guidesSettings.guidesShown);
	scrActions["viewShowColumnBorders"]->setChecked(doc->guidesSettings.colBordersShown);
	scrActions["viewShowBaseline"]->setChecked(doc->guidesSettings.baseShown);
	scrActions["viewShowImages"]->setChecked(doc->guidesSettings.showPic);
	scrActions["viewShowTextChain"]->setChecked(doc->guidesSettings.linkShown);
	scrActions["viewShowTextControls"]->setChecked(doc->guidesSettings.showControls);
	scrActions["viewShowRulers"]->setChecked(doc->guidesSettings.rulersShown);
	scrActions["viewRulerMode"]->setChecked(doc->guidesSettings.rulerMode);
	scrActions["extrasGenerateTableOfContents"]->setEnabled(doc->hasTOCSetup());
	view->cmsToolbarButton->setChecked(doc->HasCMS);
	//doc emits changed() via this
	doc->setMasterPageMode(true);
	view->reformPages();
	doc->setMasterPageMode(false);
/*	doc->setLoading(true);
	uint pageCount=doc->DocPages.count();
	for (uint c=0; c<pageCount; ++c)
		Apply_MasterPage(doc->DocPages.at(c)->MPageNam, c, false);
	doc->setLoading(false); */
	view->reformPages();
	view->GotoPage(doc->currentPage()->pageNr());
	view->DrawNew();
	propertiesPalette->ShowCMS();
	pagePalette->rebuildPages();
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
	propertiesPalette->Cpal->ChooseGrad(0);
	updateActiveWindowCaption(doc->DocName);
	scrActions["shade100"]->setChecked(true);
	propertiesPalette->setDoc(doc);
	pagePalette->setView(view);
	propertiesPalette->Fonts->RebuildList(doc);
	layerPalette->setDoc(doc);
	guidePalette->setDoc(doc);
	charPalette->setDoc(doc);
	outlinePalette->setDoc(doc);
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	//Do not set this!, it doesnt get valid pointers unless its in EditClip mode and its not
	//if we are switching windows #4357
	//nodePalette->setDoc(doc, view);
	slotChangeUnit(doc->unitIndex(), false);
/*	FIXME: check if this is really superflous now
	if (doc->appMode == modeEditClip)
	{
		doc->appMode = modeNormal;
		view->requestMode(submodeEndNodeEdit);
	} */
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

	if ( ScCore->haveGS() || ScCore->isWinGUI() )
		scrActions["PrintPreview"]->setEnabled(true);

	if (scrActions["SaveAsDocumentTemplate"])
		scrActions["SaveAsDocumentTemplate"]->setEnabled(true);

	scrActions["editCut"]->setEnabled(false);
	scrActions["editCopy"]->setEnabled(false);
	scrActions["editPaste"]->setEnabled((!Buffer2.isEmpty()) || (scrapbookPalette->tempBView->objectMap.count() > 0));
	scrMenuMgr->setMenuEnabled("EditPasteRecent", scrapbookPalette->tempBView->objectMap.count() > 0);
	scrMenuMgr->setMenuEnabled("EditContents", false);
	scrActions["editCopyContents"]->setEnabled(false);
	scrActions["editPasteContents"]->setEnabled(false);
	scrActions["editPasteContentsAbs"]->setEnabled(false);
	scrActions["editSelectAll"]->setEnabled(true);
	scrActions["editDeselectAll"]->setEnabled(false);
	scrActions["editPatterns"]->setEnabled(true);
 	scrActions["editStyles"]->setEnabled(true);
	scrActions["editMasterPages"]->setEnabled(true);
	scrActions["editJavascripts"]->setEnabled(true);

	scrMenuMgr->setMenuEnabled("View", true);
	scrActions["viewSnapToGrid"]->setChecked(doc->useRaster);
	scrActions["viewSnapToGuides"]->setChecked(doc->SnapGuides);
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
	scrActions["toolsInsertLatexFrame"]->setEnabled(true);
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

	//Update palettes
	updateActiveWindowCaption(doc->DocName);
	scrActions["shade100"]->setChecked(true);
	propertiesPalette->setDoc(doc);
	propertiesPalette->Cpal->ChooseGrad(0);
//	propertiesPalette->updateColorList();
	pagePalette->setView(view);
	layerPalette->setDoc(doc);
	guidePalette->setDoc(doc);
	charPalette->setDoc(doc);
	outlinePalette->setDoc(doc);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	rebuildLayersList();
	view->updateLayerMenu();
	view->setLayerMenuText(doc->activeLayerName());
	slotChangeUnit(doc->unitIndex());
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
//	connect(view, SIGNAL(ItemFarben(QString, QString, int, int)), this, SLOT(setCSMenu(QString, QString, int, int)));
//	connect(view, SIGNAL(ItemFarben(QString, QString, int, int)), propertiesPalette->Cpal, SLOT(setActFarben(QString, QString, int, int)));
//	connect(view, SIGNAL(ItemGradient(int)), propertiesPalette->Cpal, SLOT(setActGradient(int)));
//	connect(view, SIGNAL(ItemTrans(double, double)), propertiesPalette->Cpal, SLOT(setActTrans(double, double)));
//	connect(view, SIGNAL(ItemBlend(int, int)), propertiesPalette->Cpal, SLOT(setActBlend(int, int)));
	connect(view, SIGNAL(ItemTextAttr(double)), propertiesPalette, SLOT(setLsp(double)));
	connect(view, SIGNAL(ItemTextUSval(double)), propertiesPalette, SLOT(setExtra(double)));
//	connect(view, SIGNAL(ItemTextCols(int, double)), propertiesPalette, SLOT(setCols(int, double)));
	connect(view, SIGNAL(SetDistValues(double, double, double, double)), propertiesPalette, SLOT(setDvals(double, double, double, double)));
	connect(view, SIGNAL(ItemTextAbs(int)), propertiesPalette, SLOT(setAli(int)));
	connect(view, SIGNAL(ItemTextFont(const QString&)), propertiesPalette, SLOT(setFontFace(const QString&)));
	connect(view, SIGNAL(ItemTextSize(double)), propertiesPalette, SLOT(setSize(double)));
	//connect(view, SIGNAL(ItemRadius(double)), propertiesPalette, SLOT(setRR(double)));
//	connect(view, SIGNAL(Amode(int)), this, SLOT(setAppMode(int)));
	connect(view, SIGNAL(PaintingDone()), this, SLOT(slotSelect()));
	connect(view, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	//connect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
	connect(view, SIGNAL(MousePos(double, double)), this, SLOT(setStatusBarMousePosition(double, double)));
	//connect(view, SIGNAL(ItemRadius(double)), propertiesPalette, SLOT(setRR(double)));
	connect(view, SIGNAL(ItemTextStil(int)), propertiesPalette, SLOT(setStil(int)));
	connect(view, SIGNAL(ItemTextSca(double)), propertiesPalette, SLOT(setTScale(double)));
	connect(view, SIGNAL(ItemTextScaV(double)), propertiesPalette, SLOT(setTScaleV(double)));
	connect(view, SIGNAL(ItemTextBase(double)), propertiesPalette, SLOT(setTBase(double)));
	connect(view, SIGNAL(ItemTextShadow(double, double )), propertiesPalette, SLOT(setShadowOffs(double, double )));
	connect(view, SIGNAL(ItemTextUnderline(double, double)), propertiesPalette, SLOT(setUnderline(double, double)));
	connect(view, SIGNAL(ItemTextStrike(double, double)), propertiesPalette, SLOT(setStrike(double, double)));
	connect(view, SIGNAL(ItemTextOutline(double)), propertiesPalette, SLOT(setOutlineW(double)));
	connect(view, SIGNAL(ItemTextStil(int)), this, SLOT(setStilvalue(int)));
	connect(view, SIGNAL(ItemTextAbs(int)), this, SLOT(setAbsValue(int)));
	connect(view, SIGNAL(ItemTextFarben(QString, QString, double, double)), propertiesPalette, SLOT(setActFarben(QString, QString, double, double)));
	connect(view, SIGNAL(HasTextSel()), this, SLOT(EnableTxEdit()));
	connect(view, SIGNAL(HasNoTextSel()), this, SLOT(DisableTxEdit()));
	connect(view, SIGNAL(CopyItem()), this, SLOT(slotEditCopy()));
	connect(view, SIGNAL(CutItem()), this, SLOT(slotEditCut()));
	connect(view, SIGNAL(LoadPic()), this, SLOT(slotGetContent()));
	connect(view, SIGNAL(StatusPic()), this, SLOT(StatusPic()));
	connect(view, SIGNAL(AppendText()), this, SLOT(slotFileAppend()));
	connect(view, SIGNAL(AnnotProps()), this, SLOT(ModifyAnnot()));
	//connect(view, SIGNAL(EditGuides()), this, SLOT(ManageGuides()));
	connect(view, SIGNAL(LoadElem(QString, double ,double, bool, bool, ScribusDoc *, ScribusView*)), this, SLOT(slotElemRead(QString, double, double, bool, bool, ScribusDoc *, ScribusView*)));
	connect(view, SIGNAL(AddBM(PageItem *)), this, SLOT(AddBookMark(PageItem *)));
	connect(view, SIGNAL(DelBM(PageItem *)), this, SLOT(DelBookMark(PageItem *)));
	connect(view, SIGNAL(RasterPic(bool)), this, SLOT(HaveRaster(bool)));
	connect(view, SIGNAL(DoGroup()), this, SLOT(GroupObj()));
//	connect(view, SIGNAL(EndNodeEdit()), this, SLOT(ToggleFrameEdit()));
	connect(view, SIGNAL(LevelChanged(uint )), propertiesPalette, SLOT(setLevel(uint)));
	connect(view, SIGNAL(callGimp()), this, SLOT(callImageEditor()));
}

void ScribusMainWindow::HaveNewSel(int SelectedType)
{
	bool isRaster = false;
	PageItem *currItem = NULL;
	const uint docSelectionCount=doc->m_Selection->count();
	if (SelectedType != -1)
	{
		if (docSelectionCount != 0)
		{
			PageItem *lowestItem = doc->m_Selection->itemAt(0);
			for (uint a=0; a < docSelectionCount; ++a)
			{
				currItem = doc->m_Selection->itemAt(a);
				if (currItem->ItemNr < lowestItem->ItemNr)
					lowestItem = currItem;
			}
			currItem = lowestItem;
			if ((docSelectionCount == 1) && currItem && currItem->asImageFrame())
				isRaster = currItem->isRaster;
//			doc->m_Selection->removeItem(currItem);
//			doc->m_Selection->prependItem(currItem);
//			currItem = doc->m_Selection->itemAt(0);
			assert(currItem);
//			if (!currItem)
//				SelectedType=-1;
		}
		else
			SelectedType = -1;
	}
	else if (docSelectionCount > 0)
	{
		currItem = doc->m_Selection->itemAt(0);
	}

	assert (docSelectionCount == 0 || currItem != NULL); // help coverity analysis

	actionManager->disconnectNewSelectionActions();
	scrActions["editDeselectAll"]->setEnabled(SelectedType != -1);
	scrActions["itemDetachTextFromPath"]->setEnabled(false);
	charPalette->setEnabled(false, 0);
	scrActions["itemUpdateImage"]->setEnabled(SelectedType==PageItem::ImageFrame && (currItem->PicAvail || currItem->asLatexFrame()));
	scrActions["itemAdjustFrameToImage"]->setEnabled(SelectedType==PageItem::ImageFrame && currItem->PicAvail && !currItem->isTableItem);
	scrActions["itemExtendedImageProperties"]->setEnabled(SelectedType==PageItem::ImageFrame && currItem->PicAvail && currItem->pixm.imgInfo.valid);
	scrMenuMgr->setMenuEnabled("ItemPreviewSettings", SelectedType==PageItem::ImageFrame);
	scrActions["itemImageIsVisible"]->setEnabled(SelectedType==PageItem::ImageFrame);
	scrActions["itemPreviewLow"]->setEnabled(SelectedType==PageItem::ImageFrame);
	scrActions["itemPreviewNormal"]->setEnabled(SelectedType==PageItem::ImageFrame);
	scrActions["itemPreviewFull"]->setEnabled(SelectedType==PageItem::ImageFrame);
	scrActions["styleImageEffects"]->setEnabled(SelectedType==PageItem::ImageFrame && isRaster);
	scrActions["editCopyContents"]->setEnabled(SelectedType==PageItem::ImageFrame && currItem->PicAvail);
	scrActions["editPasteContents"]->setEnabled(SelectedType==PageItem::ImageFrame);
	scrActions["editPasteContentsAbs"]->setEnabled(SelectedType==PageItem::ImageFrame);
	scrActions["editEditWithImageEditor"]->setEnabled(SelectedType==PageItem::ImageFrame && currItem->PicAvail && currItem->isRaster);
	scrActions["editEditWithLatexEditor"]->setEnabled(SelectedType==PageItem::ImageFrame && currItem && currItem->asLatexFrame());
	if (SelectedType!=PageItem::ImageFrame)
	{
		scrActions["itemImageIsVisible"]->setChecked(false);
		scrActions["itemPreviewLow"]->setChecked(false);
		scrActions["itemPreviewNormal"]->setChecked(false);
		scrActions["itemPreviewFull"]->setChecked(false);
	}
	if ((SelectedType==-1) || (SelectedType!=-1 && !currItem->asTextFrame()))
		enableTextActions(&scrActions, false);
	scrActions["insertSampleText"]->setEnabled(false);

	view->horizRuler->ItemPosValid = false;
	view->horizRuler->repaint();
	switch (SelectedType)
	{
	case -1: // None
		scrActions["fileImportText"]->setEnabled(false);
		scrActions["fileImportText2"]->setEnabled(false);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(false);
		scrActions["fileExportText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("Item", false);
		//scrMenuMgr->setMenuEnabled("ItemShapes", false);
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
		scrMenuMgr->setMenuEnabled("EditContents", true);
		scrActions["editClearContents"]->setEnabled(true);
		scrActions["editSearchReplace"]->setEnabled(false);
		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("Item", true);
		//scrMenuMgr->setMenuEnabled("ItemShapes", !(currItem->isTableItem && currItem->isSingleSel));
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
		scrActions["itemImageIsVisible"]->setChecked(currItem->imageShown());
		scrActions["itemPreviewLow"]->setChecked(currItem->pixm.imgInfo.lowResType==scrActions["itemPreviewLow"]->actionInt());
		scrActions["itemPreviewNormal"]->setChecked(currItem->pixm.imgInfo.lowResType==scrActions["itemPreviewNormal"]->actionInt());
		scrActions["itemPreviewFull"]->setChecked(currItem->pixm.imgInfo.lowResType==scrActions["itemPreviewFull"]->actionInt());

		break;
	case PageItem::TextFrame: //Text Frame
		propertiesPalette->Fonts->RebuildList(doc, currItem->isAnnotation());
		scrActions["fileImportText"]->setEnabled(true);
		scrActions["fileImportText2"]->setEnabled(true);
		scrActions["fileImportImage"]->setEnabled(false);
		scrActions["fileImportAppendText"]->setEnabled(true);
		scrActions["fileExportText"]->setEnabled(true);
		scrActions["editCut"]->setEnabled(true);
		scrActions["editCopy"]->setEnabled(true);
		scrMenuMgr->setMenuEnabled("EditContents", true);
		scrActions["editClearContents"]->setEnabled(true);
		scrActions["editSearchReplace"]->setEnabled(currItem->itemText.length() != 0);
		scrActions["extrasHyphenateText"]->setEnabled(currItem->itemText.length() != 0);
		scrActions["extrasDeHyphenateText"]->setEnabled(currItem->itemText.length() != 0);
		scrMenuMgr->setMenuEnabled("Item", true);
		//scrMenuMgr->setMenuEnabled("ItemShapes", !(currItem->isTableItem && currItem->isSingleSel));
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
		if ((currItem->nextInChain() != 0) || (currItem->prevInChain() != 0))
		{
			scrActions["itemConvertToBezierCurve"]->setEnabled(false);
			scrActions["itemConvertToImageFrame"]->setEnabled(false);
//			scrActions["itemConvertToOutlines"]->setEnabled(false);
			scrActions["itemConvertToPolygon"]->setEnabled(false);
			scrActions["itemConvertToTextFrame"]->setEnabled(false);
			scrActions["toolsUnlinkTextFrame"]->setEnabled(true);
			// FIXME: once there's one itemtext per story, always enable editcontents
			if ((currItem->prevInChain() != 0) && (currItem->itemText.length() == 0))
				scrActions["toolsEditContents"]->setEnabled(false);
			else
				scrActions["toolsEditContents"]->setEnabled(true);
		}
		else
		{
			scrActions["toolsEditContents"]->setEnabled(true);
			scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		}
		if (currItem->nextInChain() == 0)
			scrActions["toolsLinkTextFrame"]->setEnabled(true);
//		if (doc->masterPageMode())
//			scrActions["toolsLinkTextFrame"]->setEnabled(false);
		if (doc->appMode == modeEdit)
		{
			setTBvals(currItem);
			scrActions["editSelectAll"]->setEnabled(true);
			charPalette->setEnabled(true, currItem);
			if (currItem->asTextFrame())
				enableTextActions(&scrActions, true, currItem->currentStyle().charStyle().font().scName());
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
			view->horizRuler->TabValues = currItem->currentStyle().tabValues();
			view->horizRuler->repaint();
		}
		else
		{
			doc->currentStyle = currItem->itemText.defaultStyle();
			emit TextStyle(doc->currentStyle);
			// to go: (av)
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
		propertiesPalette->Fonts->RebuildList(doc, currItem->isAnnotation());
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
		//scrMenuMgr->setMenuEnabled("ItemShapes", false);
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
			doc->currentStyle = currItem->itemText.defaultStyle();
			emit TextStyle(doc->currentStyle);
			// to go: (av)
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
		scrMenuMgr->setMenuEnabled("EditContents", false);
		scrActions["editClearContents"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);

		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("Item", true);
		if (SelectedType == 6) //Polygon
		{
			//scrMenuMgr->setMenuEnabled("ItemShapes", true);
			scrMenuMgr->setMenuEnabled("ItemConvertTo", true);
			scrActions["itemConvertToBezierCurve"]->setEnabled(doc->appMode != modeEdit);
			scrActions["itemConvertToImageFrame"]->setEnabled(doc->appMode != modeEdit);
			scrActions["itemConvertToOutlines"]->setEnabled(false);
			scrActions["itemConvertToPolygon"]->setEnabled(false);
			scrActions["itemConvertToTextFrame"]->setEnabled(doc->appMode != modeEdit);
		}
		else if (SelectedType == 5) // Line
		{
			//scrMenuMgr->setMenuEnabled("ItemShapes", false);
			scrMenuMgr->setMenuEnabled("ItemConvertTo", true);
			scrActions["itemConvertToBezierCurve"]->setEnabled(true);
			scrActions["itemConvertToImageFrame"]->setEnabled(false);
			scrActions["itemConvertToOutlines"]->setEnabled(false);
			scrActions["itemConvertToPolygon"]->setEnabled(false);
			scrActions["itemConvertToTextFrame"]->setEnabled(false);
		}
		scrActions["toolsEditContents"]->setEnabled(false);
		scrActions["toolsEditWithStoryEditor"]->setEnabled(false);
		scrActions["toolsUnlinkTextFrame"]->setEnabled(false);
		scrActions["toolsLinkTextFrame"]->setEnabled(false);
//		if (SelectedType != 5)
			scrActions["toolsRotate"]->setEnabled(true);
//		else
//			scrActions["toolsRotate"]->setEnabled(false);
		scrActions["toolsCopyProperties"]->setEnabled(true);
		break;
	}
	doc->CurrentSel = SelectedType;
	rebuildStyleMenu(SelectedType);
	propertiesPalette->RotationGroup->button(doc->RotMode)->setChecked(true);
	if (docSelectionCount > 1)
	{
		scrActions["itemConvertToBezierCurve"]->setEnabled(false);
		scrActions["itemConvertToImageFrame"]->setEnabled(false);
		//scrActions["itemConvertToOutlines"]->setEnabled(false);
		scrActions["itemConvertToPolygon"]->setEnabled(false);
		scrActions["itemConvertToTextFrame"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);

		bool hPoly = false;
		bool isGroup = true;
		int firstElem = -1;
		if (currItem->Groups.count() != 0)
			firstElem = currItem->Groups.top();
		for (uint bx=0; bx < docSelectionCount; ++bx)
		{
			PageItem* bxi=doc->m_Selection->itemAt(bx);
			if ((bxi->asPolygon()) || (bxi->asPolyLine()))
				hPoly = true;
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
			if ((currItem->asTextFrame() && (bx->asPolygon() || bx->asPolyLine())) || (bx->asTextFrame() && (currItem->asPolygon() || currItem->asPolyLine())))
			{
				if ((currItem->nextInChain() == 0) && (currItem->prevInChain() == 0) && (bx->nextInChain() == 0) && (bx->prevInChain() == 0) && (currItem->Groups.count() == 0) && (bx->Groups.count() == 0))
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
			//scrMenuMgr->setMenuEnabled("ItemShapes", false);
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
		scrActions["itemLock"]->setChecked(currItem->locked());
		scrActions["itemLockSize"]->setChecked(currItem->sizeLocked());
		scrActions["itemPrintingEnabled"]->setChecked(currItem->printEnabled());
	}

	//propertiesPalette->NewSel(SelectedType);
	if (SelectedType != -1)
	{
		//propertiesPalette->SetCurItem(currItem);
		outlinePalette->slotShowSelect(currItem->OwnPage, currItem->ItemNr);
		actionManager->connectNewSelectionActions(view, doc);
// 		propertiesPalette->NewSel(SelectedType);
	}
	else
		propertiesPalette->NewSel(SelectedType);

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
			{
				if (SelectedType != -1)
				{
					bool correctAppMode = false;
					if (ai.forAppMode.count() == 0)
						correctAppMode = true;
					else if (ai.forAppMode.contains(doc->appMode))
						correctAppMode = true;
					if (correctAppMode)
					{
						if (ai.needsNumObjects == -1)
							pluginAction->setEnabled(true);
						else
						{
							if (ai.needsNumObjects > 2)
							{
								bool setter = true;
								for (uint bx = 0; bx < docSelectionCount; ++bx)
								{
									if (ai.notSuitableFor.contains(doc->m_Selection->itemAt(bx)->itemType()))
										setter = false;
								}
								pluginAction->setEnabled(setter);
							}
							else
							{
								if (docSelectionCount == static_cast<uint>(ai.needsNumObjects))
								{
									if (ai.needsNumObjects == 2)
									{
										int sel1 = doc->m_Selection->itemAt(0)->itemType();
										int sel2 = doc->m_Selection->itemAt(1)->itemType();
										if (ai.notSuitableFor.contains(sel1))
											pluginAction->setEnabled(false);
										else if (ai.notSuitableFor.contains(sel2))
											pluginAction->setEnabled(false);
										else
										{
											if ((ai.firstObjectType.count() == 0) && (ai.secondObjectType.count() == 0))
												pluginAction->setEnabled(true);
											else if ((ai.firstObjectType.count() == 0) && (ai.secondObjectType.count() != 0))
											{
												if ((ai.secondObjectType.contains(sel2)) || (ai.secondObjectType.contains(sel1)))
													pluginAction->setEnabled(true);
											}
											else if ((ai.firstObjectType.count() != 0) && (ai.secondObjectType.count() == 0))
											{
												if ((ai.firstObjectType.contains(sel2)) || (ai.firstObjectType.contains(sel1)))
													pluginAction->setEnabled(true);
											}
											if (((ai.firstObjectType.contains(sel1)) && (ai.secondObjectType.contains(sel2))) || ((ai.firstObjectType.contains(sel2)) && (ai.secondObjectType.contains(sel1))))
												pluginAction->setEnabled(true);
										}
									}
									else if (!ai.notSuitableFor.contains(SelectedType))
										pluginAction->setEnabled(true);
									else
										pluginAction->setEnabled(false);
								}
								else
									pluginAction->setEnabled(false);
							}
						}
					}
					else
						pluginAction->setEnabled(false);
				}
				else
				{
					bool correctAppMode = false;
					if (ai.forAppMode.count() == 0)
						correctAppMode = true;
					else if (ai.forAppMode.contains(doc->appMode))
						correctAppMode = true;
					if (correctAppMode)
					{
						if ((ai.needsNumObjects == -1) || (ai.needsNumObjects > 2))
							pluginAction->setEnabled(true);
						else
							pluginAction->setEnabled(false);
					}
					else
						pluginAction->setEnabled(false);
				}
			}
		}
	}
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

		bool multiPages = doc->Pages->count() > 1;
		scrActions["pageDelete"]->setEnabled(multiPages);
		scrActions["pageMove"]->setEnabled(multiPages);
	}

	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
}

void ScribusMainWindow::updateRecent(QString fn)
{
	if (RecentDocs.indexOf(fn) == -1)
	{
		RecentDocs.prepend(fn);
		ScCore->fileWatcher->addFile(fn);
	}
	else
	{
		RecentDocs.removeAll(fn);
		RecentDocs.prepend(fn);
	}
	rebuildRecentFileMenu();
}

void ScribusMainWindow::removeRecent(QString fn)
{
	if (RecentDocs.indexOf(fn) != -1)
	{
		RecentDocs.removeAll(fn);
		ScCore->fileWatcher->removeFile(fn);
	}
	rebuildRecentFileMenu();
}

void ScribusMainWindow::loadRecent(QString fn)
{
	QFileInfo fd(fn);
	if (!fd.exists())
	{
		removeRecent(fn);
		return;
	}
	loadDoc(fn);
}

void ScribusMainWindow::rebuildRecentFileMenu()
{
	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrRecentFileActions.begin(); it!=scrRecentFileActions.end(); ++it )
		scrMenuMgr->removeMenuItem((*it), recentFileMenuName);

	scrRecentFileActions.clear();
	uint max = qMin(prefsManager->appPrefs.RecentDCount, RecentDocs.count());
	QString strippedName, localName;
	for (uint m = 0; m < max; ++m)
	{
		strippedName=RecentDocs[m];
		strippedName.remove(QDir::separator());
		localName=QDir::convertSeparators(RecentDocs[m]);
		scrRecentFileActions.insert(strippedName, new ScrAction(ScrAction::RecentFile, QPixmap(), QPixmap(), QString("&%1 %2").arg(m+1).arg(localName), QKeySequence(), this, 0,0.0,RecentDocs[m]));
		connect( scrRecentFileActions[strippedName], SIGNAL(triggeredData(QString)), this, SLOT(loadRecent(QString)) );
		scrMenuMgr->addMenuItem(scrRecentFileActions[strippedName], recentFileMenuName);
	}
}

void ScribusMainWindow::rebuildRecentPasteMenu()
{
	for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrRecentPasteActions.begin(); it!=scrRecentPasteActions.end(); ++it )
		scrMenuMgr->removeMenuItem((*it), recentPasteMenuName);

	scrRecentPasteActions.clear();
	int max = qMin(prefsManager->appPrefs.numScrapbookCopies, scrapbookPalette->tempBView->objectMap.count());
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
			scrRecentPasteActions.insert(strippedName, new ScrAction(ScrAction::RecentPaste, pm, QPixmap(), QString("&%1 %2").arg(m+1).arg(strippedName), QKeySequence(), this, 0,0.0,it.key()));
			connect( scrRecentPasteActions[strippedName], SIGNAL(triggeredData(QString)), this, SLOT(pasteRecent(QString)) );
			scrMenuMgr->addMenuItem(scrRecentPasteActions[strippedName], recentPasteMenuName);
			it--;
		}
	}
}

void ScribusMainWindow::pasteRecent(QString fn)
{
	QString data = scrapbookPalette->tempBView->objectMap[fn].Data;
	QFileInfo fi(data);
	if (fi.suffix().toLower() == "sml")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createObjects(f);
		delete pre;
	}
	else if (fi.suffix().toLower() == "shape")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createShape(f);
		delete pre;
	}
	else if (fi.suffix().toLower() == "sce")
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
	for (int as = ac; as < doc->Items->count(); ++as)
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
		for( QMap<QString, QPointer<ScrAction> >::Iterator it0 = scrLayersActions.begin(); it0 != scrLayersActions.end(); ++it0 )
			scrMenuMgr->removeMenuItem((*it0), layerMenuName);
		scrLayersActions.clear();
		ScLayers::iterator it;
		if (doc->Layers.count()!= 0)
		{
			for (it = doc->Layers.begin(); it != doc->Layers.end(); ++it)
			{
				scrLayersActions.insert(QString("%1").arg((*it).LNr), new ScrAction( ScrAction::Layer, QPixmap(), QPixmap(), (*it).Name, QKeySequence(), this, (*it).LNr));
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
		scrLayersActions[QString("%1").arg((*it).LNr)]->setChecked(true);

		for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=scrLayersActions.end(); ++it )
		{
			scrMenuMgr->addMenuItem((*it), layerMenuName);
			connect( (*it), SIGNAL(triggeredData(int)), doc, SLOT(itemSelection_SendToLayer(int)) );
		}
	}
}

void ScribusMainWindow::updateItemLayerList()
{
	if (HaveDoc)
	{
		QMap<QString, QPointer<ScrAction> >::Iterator itend=scrLayersActions.end();
		for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=itend; ++it )
		{
			disconnect( (*it), SIGNAL(triggeredData(int)), 0, 0 );
			(*it)->setChecked(false);
		}
		if (doc->m_Selection->count()>0 && doc->m_Selection->itemAt(0))
			scrLayersActions[QString("%1").arg(doc->m_Selection->itemAt(0)->LayerNr)]->setChecked(true);
		for( QMap<QString, QPointer<ScrAction> >::Iterator it = scrLayersActions.begin(); it!=itend; ++it )
			connect( (*it), SIGNAL(triggeredData(int)), doc, SLOT(itemSelection_SendToLayer(int)) );
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
	docContext->set("docsopen", fileName.left(fileName.lastIndexOf("/")));
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
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
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
			addNewPages(dia->getImportWherePage(), importWhere, nrToImport, doc->pageHeight, doc->pageWidth, doc->PageOri, doc->m_pageSize, true);
		}
		else
		{
			startPage = doc->currentPage()->pageNr() + 1;
			if (nrToImport > (doc->DocPages.count() - doc->currentPage()->pageNr()))
			{
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
						addNewPages(doc->DocPages.count(), 2,
									nrToImport - (doc->DocPages.count() - doc->currentPage()->pageNr()),
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
				qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
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
				mainWindowStatusLabel->setText( tr("Import done"));
			}
			else
			{
				mainWindowStatusLabel->setText( tr("Found nothing to import"));
				doIt = false;
			}
		}
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
//			if ((docItemsCount - oldItemsCount) > 1)
//				ite->Groups.push(doc->GroupCounter);
//	#5386: allow locked imported items to remain locked
// 			if (ite->locked())
// 				ite->setLocked(false);
			if ((ite->asTextFrame()) && (ite->isBookmark))
				AddBookMark(ite);
		}
//		if ((docItemsCount - oldItemsCount) > 1)
//			doc->GroupCounter++;
		propertiesPalette->updateColorList();
		propertiesPalette->paraStyleCombo->setDoc(doc);
		propertiesPalette->charStyleCombo->setDoc(doc);
		propertiesPalette->SetLineFormats(doc);
		propertiesPalette->startArrow->rebuildList(&doc->arrowStyles);
		propertiesPalette->endArrow->rebuildList(&doc->arrowStyles);
		if (!Mpa)
		{
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
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	if (HaveDoc)
		outlinePalette->buildReopenVals();
	bool ret = false;
	QWidgetList windows = wsp->windowList();
	ScribusWin* ActWinOld = NULL;
	if (windows.count() != 0)
		ActWinOld = ActWin;
	bool found = false;
	uint id = 0;
	QString platfName = QDir::convertSeparators(fileName);
	uint windowCount=windows.count();
	for ( uint i = 0; i < windowCount; ++i )
	{
		if (windows.at(i)->windowTitle() == platfName)
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
		QString FName = fi.absoluteFilePath();
		FileLoader *fileLoader = new FileLoader(FName);
		int testResult=fileLoader->TestFile();
		if (testResult == -1)
		{
			delete fileLoader;
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			QMessageBox::critical(this, tr("Fatal Error"), "<qt>"+ tr("File %1 is not in an acceptable format").arg(FName)+"</qt>", CommonStrings::tr_OK);
			return false;
		}
		bool is12doc=false;
		if (testResult == 0)
		{
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			//Scribus 1.3.x warning, remove at a later stage
			is12doc=true;
		}

		ScCore->getCMSProfilesDir(fi.absolutePath()+"/", false, false);
		prefsManager->appPrefs.AvailFonts.AddScalableFonts(fi.absolutePath()+"/", FName);
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
		wsp->addWindow(w);
		w->setUpdatesEnabled(false);
		view->updatesOn(false);
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
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			mainWindowStatusLabel->setText("");
			mainWindowProgressBar->reset();
			ActWin = NULL;
			undoManager->setUndoEnabled(true);
			if (windows.count() != 0)
				newActWin(ActWinOld);
			return false;
		}
		outlinePalette->setDoc(doc);
		fileLoader->informReplacementFonts();
		setCurrentComboItem(view->unitSwitcher, unitGetStrFromIndex(doc->unitIndex()));
		view->unitChange();
		ScriptRunning = false;
		view->Deselect(true);
		mainWindowStatusLabel->setText("");
		mainWindowProgressBar->reset();
		HaveDoc++;
		if (doc->checkerProfiles.count() == 0)
		{
			prefsManager->initDefaultCheckerPrefs(&doc->checkerProfiles);
			doc->curCheckProfile = CommonStrings::PostScript;
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
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				QString mess = tr("Some color profiles used by this document are not installed:")+"\n\n";
				for (int m = 0; m < missing.count(); ++m)
				{
					mess += missing[m] + tr(" was replaced by: ")+replacement[m]+"\n";
				}
				QMessageBox::warning(this, CommonStrings::trWarning, mess, 1, 0, 0);
			}
			doc->SoftProofing = doc->CMSSettings.SoftProofOn;
			doc->Gamut        = doc->CMSSettings.GamutCheck;
			doc->IntentColors = doc->CMSSettings.DefaultIntentColors;
			doc->IntentImages = doc->CMSSettings.DefaultIntentImages;
			if (doc->OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles))
			{
				doc->HasCMS = true;
				doc->PDF_Options.SComp = doc->CMSSettings.ComponentsInput2;
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
			doc->CMSSettings.CMSinUse = false;
		}
//		propertiesPalette->updateColorList();
//		propertiesPalette->Cpal->ChooseGrad(0);
		if (fileLoader->FileType > FORMATID_NATIVEIMPORTEND)
		{
			doc->setName(FName+ tr("(converted)"));
			QFileInfo fi(doc->DocName);
			doc->setName(fi.fileName());
		}
		else
			doc->setName(FName);
		doc->setMasterPageMode(false);
		doc->Language = GetLang(doc->Language);
		HaveNewDoc();
//		propertiesPalette->Cpal->ChooseGrad(0);
//		propertiesPalette->updateCList();
		doc->hasName = true;
		if (doc->MasterPages.count() == 0)
			doc->addMasterPage(0, CommonStrings::masterPageNormal);
		//Add doc sections if we have none
		if (doc->sections.count()==0)
		{
			doc->addSection(-1);
			doc->setFirstSectionFromFirstPageNumber();
		}
		doc->RePos = true;
		doc->setMasterPageMode(true);
		doc->reformPages();
		doc->setLoading(false);
		for (int azz=0; azz<doc->MasterItems.count(); ++azz)
		{
			PageItem *ite = doc->MasterItems.at(azz);
			// TODO fix that for Groups on Masterpages
//			if (ite->Groups.count() != 0)
//				view->GroupOnPage(ite);
//			qDebug(QString("load M: %1 %2 %3").arg(azz).arg((uint)ite).arg(ite->itemType()));
			ite->layout();
		}
//		RestoreBookMarks();
		doc->setMasterPageMode(false);
		int docItemsCount=doc->Items->count();
		for (int azz=0; azz<docItemsCount; ++azz)
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
//			qDebug(QString("load D: %1 %2 %3").arg(azz).arg((uint)ite).arg(ite->itemType()));
			if(ite->nextInChain() == NULL)
				ite->layout();
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
		for (int azz=0; azz<doc->FrameItems.count(); ++azz)
		{
			PageItem *ite = doc->FrameItems.at(azz);
//			qDebug(QString("load F: %1 %2 %3").arg(azz).arg((uint)ite).arg(ite->itemType()));
			if(ite->nextInChain() == NULL)
				ite->layout();
		}
//		if (doc->OldBM)
//			StoreBookmarks();
		doc->RePos = false;
		doc->setModified(false);
		updateRecent(FName);
		mainWindowStatusLabel->setText( tr("Ready"));
		ret = true;
		doc->setLoading(true);
		for (int p = 0; p < doc->DocPages.count(); ++p)
		{
			Apply_MasterPage(doc->DocPages.at(p)->MPageNam, p, false);
		}
		doc->setLoading(false);
/*		if (fileLoader->FileType > FORMATID_NATIVEIMPORTEND)
		{
			doc->hasName = false;
			slotFileSaveAs();
		} */
		delete fileLoader;
		view->updatesOn(true);
		w->setUpdatesEnabled(true);
		disconnect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
		if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
			w->showMaximized();
		else
			w->show();
		view->show();
		newActWin(w);
		doc->setCurrentPage(doc->DocPages.at(0));
		view->cmsToolbarButton->setChecked(doc->HasCMS);
		view->slotDoZoom();
		view->GotoPage(0);
		connect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
		connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
		connect(ScCore->fileWatcher, SIGNAL(fileChanged(QString )), doc, SLOT(updatePict(QString)));
		connect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), doc, SLOT(removePict(QString)));
		connect(undoManager, SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
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
	pagePalette->Rebuild();
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
			QString formatD(FormatsManager::instance()->fileDialogFormatList(FormatsManager::IMAGESIMGFRAME));

			QString docDir = ".";
			QString prefsDocDir=prefsManager->documentDir();
			if (!prefsDocDir.isEmpty())
				docDir = prefsManager->prefsFile->getContext("dirs")->get("images", prefsDocDir);
			else
				docDir = prefsManager->prefsFile->getContext("dirs")->get("images", ".");
			QString fileName = CFileDialog( docDir, tr("Open"), formatD, "", fdShowPreview | fdExistingFiles);
			if (!fileName.isEmpty())
			{
				prefsManager->prefsFile->getContext("dirs")->set("images", fileName.left(fileName.lastIndexOf("/")));
				currItem->EmProfile = "";
				currItem->pixm.imgInfo.isRequest = false;
				currItem->UseEmbedded = true;
				currItem->IProfile = doc->CMSSettings.DefaultImageRGBProfile;
				currItem->IRender = doc->CMSSettings.DefaultIntentImages;
				qApp->changeOverrideCursor( QCursor(Qt::WaitCursor) );
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				doc->LoadPict(fileName, currItem->ItemNr, false, true);
				//view->AdjustPictScale(currItem, false);
				//false was ignored anyway
				currItem->AdjustPictScale();
				propertiesPalette->setLvalue(currItem->imageXScale(), currItem->imageYScale(), currItem->imageXOffset(), currItem->imageYOffset());
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				view->DrawNew();
				propertiesPalette->updateColorList();
				propertiesPalette->ShowCMS();
			}
		}
		if (currItem->asTextFrame())
		{
			gtGetText* gt = new gtGetText(doc);
			ImportSetup impsetup=gt->run();
			if (impsetup.runDialog)
			{
				if (currItem->itemText.length() != 0)
				{
					int t = QMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your text?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
					if (t == QMessageBox::No)
						return;
				}
				gt->launchImporter(impsetup.importer, impsetup.filename, impsetup.textOnly, impsetup.encoding, false);
			}
			delete gt;
			if (doc->docHyphenator->AutoCheck)
				doc->docHyphenator->slotHyphenate(currItem);
			for (int a = 0; a < doc->Items->count(); ++a)
			{
				if (doc->Items->at(a)->isBookmark)
					bookmarkPalette->BView->ChangeText(doc->Items->at(a));
			}
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
	for (int a = 0; a < doc->Items->count(); ++a)
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
		int t = QMessageBox::warning(this, CommonStrings::trWarning, "<qt>" +
								 QObject::tr("The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?") + "</qt>",
								 QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if (t == QMessageBox::No)
			return;

		QString fn(doc->DocName);
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

		QString fn(doc->DocName), savedFileName;
		ret = DoFileSave(fn, &savedFileName);
		if (!ret && !savedFileName.isEmpty())
			QMessageBox::warning(this, CommonStrings::trWarning, tr("Your document was saved to a temporary file and could not be moved: \n%1").arg( QDir::toNativeSeparators(savedFileName) ), CommonStrings::tr_OK);
		else if (!ret)
			QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg( QDir::toNativeSeparators(fn) ), CommonStrings::tr_OK);
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
		wdir = QDir::fromNativeSeparators( fi.path() );
		fna  = QDir::fromNativeSeparators( fi.path()+"/"+fi.baseName()+".sla" );
	}
	else
	{
		QString prefsDocDir=prefsManager->documentDir();
		if (!prefsDocDir.isEmpty())
			wdir = docContext->get("save_as", prefsDocDir);
		else
			wdir = docContext->get("save_as", ".");
		wdir = QDir::fromNativeSeparators( wdir );
		if (wdir.right(1) != "/")
			fna = wdir + "/";
		else
			fna = wdir;
		fna += doc->DocName + ".sla";
	}
	QString fileSpec=tr("Documents (*.sla *.sla.gz);;All Files (*)");
// 	bool setter=true;
	int optionFlags = fdCompressFile;
	QString fn = CFileDialog( wdir, tr("Save As"), fileSpec, fna, optionFlags);
	if (!fn.isEmpty())
	{
		docContext->set("save_as", fn.left(fn.lastIndexOf("/")));
		if ((fn.endsWith(".sla")) || (fn.endsWith(".sla.gz")))
			fna = fn;
		else
			fna = fn+".sla";
		if (overwrite(this, fna))
		{
			QString savedFileName;
			ret = DoFileSave(fna, &savedFileName);
			if (!ret && !savedFileName.isEmpty())
				QMessageBox::warning(this, CommonStrings::trWarning, tr("Your document was saved to a temporary file and could not be moved: \n%1").arg( QDir::toNativeSeparators(savedFileName) ), CommonStrings::tr_OK);
			else if (!ret)
				QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg( QDir::toNativeSeparators(fn) ), CommonStrings::tr_OK);
			else
				doc->PDF_Options.Datei = ""; // #1482 reset the pdf file name
		}
	}
	mainWindowStatusLabel->setText( tr("Ready"));
	return ret;
}

bool ScribusMainWindow::DoFileSave(const QString& fileName, QString* savedFileName)
{
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	doc->reorganiseFonts();
	mainWindowStatusLabel->setText( tr("Saving..."));
	mainWindowProgressBar->reset();
	bool ret=doc->save(fileName, savedFileName);
	qApp->processEvents();
	if (ret)
	{
		updateActiveWindowCaption(fileName);
		undoManager->renameStack(fileName);
// 		scrActions["fileSave"]->setEnabled(false);
		scrActions["fileRevert"]->setEnabled(false);
		updateRecent(fileName);
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
		slotSelect();
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
	slotSelect();
	doc->autoSaveTimer->stop();
	disconnect(doc->autoSaveTimer, SIGNAL(timeout()), doc->WinHan, SLOT(slotAutoSave()));
	disconnect(doc->WinHan, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	disconnect(ScCore->fileWatcher, SIGNAL(fileChanged(QString )), doc, SLOT(updatePict(QString)));
	disconnect(ScCore->fileWatcher, SIGNAL(fileDeleted(QString )), doc, SLOT(removePict(QString)));
	for (int a = 0; a < doc->DocItems.count(); ++a)
	{
		PageItem *currItem = doc->DocItems.at(a);
		if (currItem->PicAvail)
			ScCore->fileWatcher->removeFile(currItem->Pfile);
	}
	for (int a = 0; a < doc->MasterItems.count(); ++a)
	{
		PageItem *currItem = doc->MasterItems.at(a);
		if (currItem->PicAvail)
			ScCore->fileWatcher->removeFile(currItem->Pfile);
	}
	for (int a = 0; a < doc->FrameItems.count(); ++a)
	{
		PageItem *currItem = doc->FrameItems.at(a);
		if (currItem->PicAvail)
			ScCore->fileWatcher->removeFile(currItem->Pfile);
	}
	QStringList patterns = doc->docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = doc->docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->PicAvail)
				ScCore->fileWatcher->removeFile(currItem->Pfile);
		}
	}
	if (ScCore->haveCMS())
		doc->CloseCMSProfiles();
	//<<Palettes
//	propertiesPalette->NewSel(-1);
	propertiesPalette->unsetDoc();
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
 		scrActions["editStyles"]->setEnabled(false);
		scrActions["editSearchReplace"]->setEnabled(false);
		scrActions["editMasterPages"]->setEnabled(false);
		scrActions["editJavascripts"]->setEnabled(false);

		//scrActions["toolsPreflightVerifier"]->setEnabled(false);

		scrActions["extrasHyphenateText"]->setEnabled(false);
		scrActions["extrasDeHyphenateText"]->setEnabled(false);
		scrMenuMgr->setMenuEnabled("View", false);
		//scrMenuMgr->setMenuEnabled("Windows", false);
		scrActions["viewSnapToGuides"]->setChecked(false);
		scrActions["viewSnapToGrid"]->setChecked(false);
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
		scrActions["toolsInsertLatexFrame"]->setEnabled(false);
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
	charPalette->setDoc(0);
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
			QDir::setCurrent( QDir::homePath() );
	}
	styleManager->setDoc(0);
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
				int t = QMessageBox::warning(this, CommonStrings::trWarning,
											"<qt>"+ tr("Scribus has detected some errors. Consider using the Preflight Verifier to correct them")+"</qt>",
											QMessageBox::Abort | QMessageBox::Ignore);
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
	mainWindowStatusLabel->setText( tr("Printing..."));
	if (doc->Print_Options.firstUse)
	{
		doc->Print_Options.printer = "";
		if (!doc->DocName.startsWith( tr("Document")))
		{
			QFileInfo fi(doc->DocName);
			doc->Print_Options.filename = fi.path()+"/"+fi.baseName()+".ps";
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
	PrintDialog *printer = new PrintDialog(this, doc, doc->Print_Options.filename, doc->Print_Options.printer, PDef.Command, doc->Print_Options.devMode, prefsManager->appPrefs.GCRMode, spots);
	printer->setMinMax(1, doc->Pages->count(), doc->currentPage()->pageNr()+1);
	printDinUse = true;
	connect(printer, SIGNAL(doPreview()), this, SLOT(doPrintPreview()));
	if (printer->exec())
	{
		ReOrderText(doc, view);
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
		doc->Print_Options.pageNumbers.clear();
		if (printer->CurrentPage->isChecked())
			doc->Print_Options.pageNumbers.push_back(doc->currentPage()->pageNr()+1);
		else
		{
			if (printer->RadioButton1->isChecked())
				parsePagesString("*", &doc->Print_Options.pageNumbers, doc->DocPages.count());
			else
				parsePagesString(printer->pageNr->text(), &doc->Print_Options.pageNumbers, doc->DocPages.count());
		}
		PrinterUsed = true;
		done = doPrint(doc->Print_Options, printError);
		if (!done)
		{
			QString message = tr("Printing failed!");
			if (!printError.isEmpty())
				message += QString("\n%1").arg(printError);
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			QMessageBox::warning(this, CommonStrings::trWarning, message, CommonStrings::tr_OK);
		}
		else
			doc->Print_Options.firstUse = false;
		getDefaultPrinter(PDef.Pname, PDef.Pname, PDef.Command);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	printDinUse = false;
	disconnect(printer, SIGNAL(doPreview()), this, SLOT(doPrintPreview()));
	delete printer;
	mainWindowStatusLabel->setText( tr("Ready"));
}

bool ScribusMainWindow::doPrint(PrintOptions &options, QString& error)
{
	bool printDone = false;
	QString filename(options.filename);
	if (options.toFile)
	{
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		if (!overwrite(this, filename))
		{
			return true;
		}
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
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
	propertiesPalette->unsetDoc();
	ScCore->pluginManager->savePreferences();
	close();
}


static bool hasXMLRootElem(const QString& buffer, const QString& elemtag)
{
	return buffer.lastIndexOf(elemtag, 50 + elemtag.length()) >= 0;
}


void ScribusMainWindow::slotEditCut()
{
//	int a;
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
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
			StoryText itemText(doc);
			itemText.setDefaultStyle(currItem->itemText.defaultStyle());
			itemText.insert(0, currItem->itemText, true);

			BufferI = itemText.text(0, itemText.length());

			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			xmlStream.beginDoc();
			itemText.saxx(xmlStream, "SCRIBUSTEXT");
			xmlStream.endDoc();
			std::string xml(xmlString.str());
			Buffer2 = QString::fromUtf8(xml.c_str(), xml.length());

			/*			PageItem *nextItem = currItem;
			while (nextItem != 0)
			{
				if (nextItem->prevInChain() != 0)
					nextItem = nextItem->prevInChain();
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
				nextItem = nextItem->nextInChain();
			}
			*/
			dynamic_cast<PageItem_TextFrame*>(currItem)->deleteSelectedTextFromFrame();
			currItem->update();
		}
		else
		{
			if (((currItem->isSingleSel) && (currItem->isGroupControl)) || ((currItem->isSingleSel) && (currItem->isTableItem)))
				return;

			// old version:
			ScriXmlDoc *ss = new ScriXmlDoc();
			Buffer2 = ss->WriteElem(doc, view, doc->m_Selection);

			// new version:
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
//			qDebug(QString("call serializer: %1").arg((ulong) & (doc->m_Selection)));
			Serializer::serializeObjects(*doc->m_Selection, xmlStream);
			std::string xml(xmlString.str());
			BufferI = QString::fromUtf8(xml.c_str(), xml.length());

			if (prefsManager->appPrefs.doCopyToScrapbook)
			{
				scrapbookPalette->ObjFromCopyAction(Buffer2);
				rebuildRecentPasteMenu();
			}
			Buffer2 = BufferI;
			for (int i=0; i < doc->m_Selection->count(); ++i)
			{
				PageItem* frame = doc->m_Selection->itemAt(i);
				if (frame->asTextFrame() && frame->prevInChain() == NULL)
					frame->clearContents();
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
//	int a;
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	QString BufferI = "";
	if ((HaveDoc) && (doc->m_Selection->count() != 0))
	{
		Buffer2 = "<SCRIBUSTEXT/>";
		PageItem *currItem = doc->m_Selection->itemAt(0);
		if ((doc->appMode == modeEdit) && (currItem->HasSel))
		{
			StoryText itemText(doc);
			itemText.setDefaultStyle(currItem->itemText.defaultStyle());
			itemText.insert(0, currItem->itemText, true);

			BufferI = itemText.text(0, itemText.length());

			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			xmlStream.beginDoc();
			itemText.saxx(xmlStream, "SCRIBUSTEXT");
			xmlStream.endDoc();
			std::string xml(xmlString.str());
			Buffer2 = QString::fromUtf8(xml.c_str(), xml.length());
//			qDebug(Buffer2);

/*			PageItem *nextItem = currItem;
			while (nextItem != 0)
			{
				if (nextItem->prevInChain() != 0)
					nextItem = nextItem->prevInChain();
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
				nextItem = nextItem->nextInChain();
			}
			*/
		}
		else
		{
			if (((currItem->isSingleSel) && (currItem->isGroupControl)) || ((currItem->isSingleSel) && (currItem->isTableItem)))
				return;

			// old version:
			ScriXmlDoc *ss = new ScriXmlDoc();
			Buffer2 = ss->WriteElem(doc, view, doc->m_Selection);

			// new version:
			std::ostringstream xmlString;
			SaxXML xmlStream(xmlString);
			Serializer::serializeObjects(*doc->m_Selection, xmlStream);
			std::string xml(xmlString.str());
			BufferI = QString::fromUtf8(xml.c_str(), xml.length());

#ifdef DESAXE_DEBUG
			// debug:
			SaxXML tmpfile1("tmp-scribus1.xml", true);
			Serializer::serializeObjects(*doc->m_Selection, tmpfile1);
			Serializer digester(*doc);
			QFile file ("tmp-scribus1.xml");
			Selection objects = digester.deserializeObjects(file);
			SaxXML tmpfile2("tmp-scribus2.xml", true);
			Serializer::serializeObjects(objects, tmpfile2);
			doc->itemSelection_DeleteItem(&objects);
#endif

			if ((prefsManager->appPrefs.doCopyToScrapbook) && (!internalCopy))
			{
				scrapbookPalette->ObjFromCopyAction(Buffer2);
				rebuildRecentPasteMenu();
			}
			Buffer2 = BufferI;
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
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	if (HaveDoc)
	{
		if (Buffer2.isNull())
			return;
		if (UndoManager::undoEnabled())
			undoManager->beginTransaction(doc->currentPage()->getUName(), 0, Um::Paste, "", Um::IPaste);
		if (doc->appMode == modeEdit && doc->m_Selection->itemAt(0))
		{
			PageItem_TextFrame *currItem = dynamic_cast<PageItem_TextFrame*>(doc->m_Selection->itemAt(0));
			assert(currItem != NULL);
			if (currItem->HasSel)
				currItem->deleteSelectedTextFromFrame();

			if (currItem->CPos < 0)
				currItem->CPos = 0;
			if (currItem->CPos > currItem->itemText.length())
				currItem->CPos = currItem->itemText.length();

			if (hasXMLRootElem(Buffer2, "<SCRIBUSTEXT"))
			{
				Serializer dig(*doc);
				dig.store<ScribusDoc>("<scribusdoc>", doc);
				StoryText::desaxeRules("/", dig, "SCRIBUSTEXT");
				dig.addRule("/SCRIBUSTEXT", desaxe::Result<StoryText>());

				QByteArray xml( Buffer2.toUtf8() );
				dig.parseMemory(xml, xml.length());

				StoryText* story = dig.result<StoryText>();

				currItem->itemText.insert(currItem->CPos, *story);
				currItem->CPos += story->length();

				delete story;

/*				QString Buf = Buffer2.mid(13);
				QTextStream t(&Buf, IO_ReadOnly);
				QString cc;
				while (!t.atEnd())
				{
					cc = t.readLine();
					QStringList wt;
					QStringList::Iterator it;
					wt = QStringList::split("\t", cc);
					it = wt.begin();
					CharStyle nstyle;
					QString ch = (*it);
					if (ch == QChar(5))
						ch = SpecialChars::PARSEP;
					if (ch == QChar(4))
						ch = SpecialChars::TAB;
						Don't copy inline frames for now, as this is a very complicated thing.
						We need to figure out a good way to copy inline frames, this must
						be able to preserve them across documents. No idea how to solve
						that yet. *--/
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
					nstyle.setFeatures(static_cast<StyleFlag>((*it).toInt()).featureList());
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
						nstyle.setScaleV(qMin(qMax((*it).toInt(), 100), 4000));
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
						currItem->itemText.applyStyle(currItem->CPos, doc->paragraphStyles()[cab]);
					}
					else {
						currItem->itemText.applyCharStyle(currItem->CPos, 1, nstyle);
					}
					currItem->CPos += 1;
				}
*/
			}
			else if (hasXMLRootElem(Buffer2, "<SCRIBUSELEM") || hasXMLRootElem(Buffer2, "<SCRIBUSFRAGMENT"))
			{
				bool savedAlignGrid = doc->useRaster;
				bool savedAlignGuides = doc->SnapGuides;
				int ac = doc->Items->count();
				bool isGroup = false;
				double gx, gy, gh, gw;
				FPoint minSize = doc->minCanvasCoordinate;
				FPoint maxSize = doc->maxCanvasCoordinate;
				doc->useRaster = false;
				doc->SnapGuides = false;
				// HACK #6541 : undo does not handle text modification => do not record embedded item creation
				// if embedded item is deleted, undo system will not be aware of its deletion => crash - JG
				bool undoEnabled = undoManager->undoEnabled();
				undoManager->setUndoEnabled(false);

				if (hasXMLRootElem(Buffer2, "<SCRIBUSELEM"))
					slotElemRead(Buffer2, 0, 0, false, true, doc, view);
				else
					Serializer(*doc).deserializeObjects(Buffer2.toUtf8());

				// update style lists:
				styleManager->setDoc(doc);
				propertiesPalette->unsetDoc();
				propertiesPalette->setDoc(doc);

				doc->useRaster = savedAlignGrid;
				doc->SnapGuides = savedAlignGuides;
				//int tempList=doc->m_Selection->backupToTempList(0);
				Selection tempSelection(*doc->m_Selection);
				doc->m_Selection->clear();
				if (doc->Items->count() - ac > 1)
					isGroup = true;
				for (int as = ac; as < doc->Items->count(); ++as)
				{
					doc->m_Selection->addItem(doc->Items->at(as));
					if (isGroup)
						doc->Items->at(as)->Groups.push(doc->GroupCounter);
				}
				if (isGroup)
					doc->GroupCounter++;
				doc->m_Selection->setGroupRect();
				doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				PageItem* currItem3 = doc->Items->at(ac);
				for (int as = ac; as < doc->Items->count(); ++as)
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
				int acc = doc->Items->count();
				for (int as = ac; as < acc; ++as)
				{
					doc->Items->takeAt(ac);
				}
				doc->m_Selection->clear();
				//doc->m_Selection->restoreFromTempList(0, tempList);
				*doc->m_Selection=tempSelection;
//				view->resizeContents(qRound((maxSize.x() - minSize.x()) * view->scale()), qRound((maxSize.y() - minSize.y()) * view->scale()));
//				view->scrollBy(qRound((doc->minCanvasCoordinate.x() - minSize.x()) * view->scale()), qRound((doc->minCanvasCoordinate.y() - minSize.y()) * view->scale()));
				doc->minCanvasCoordinate = minSize;
				doc->maxCanvasCoordinate = maxSize;
				if (outlinePalette->isVisible())
					outlinePalette->BuildTree();
				currItem->itemText.insertObject(currItem->CPos, currItem3);
				currItem->CPos += 1;
				undoManager->setUndoEnabled(undoEnabled);
			}
			else
			{
				// K.I.S.S.:
				currItem->itemText.insertChars(currItem->CPos, Buffer2, true);
			}
			currItem->update();
		}
		else
		{
			if (hasXMLRootElem(Buffer2, "<SCRIBUSELEM") || hasXMLRootElem(Buffer2, "<SCRIBUSFRAGMENT"))
			{
				view->Deselect(true);
				uint ac = doc->Items->count();
				bool savedAlignGrid = doc->useRaster;
				bool savedAlignGuides = doc->SnapGuides;
				doc->useRaster = false;
				doc->SnapGuides = false;
//				qDebug(Buffer2);
				if (hasXMLRootElem(Buffer2, "<SCRIBUSELEM"))
					slotElemRead(Buffer2, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
				else
				{
					Selection pastedObjects = Serializer(*doc).deserializeObjects(Buffer2.toUtf8());
					for (int i=0; i < pastedObjects.count(); ++i)
						pastedObjects.itemAt(i)->LayerNr = doc->activeLayer();

					/*double x = doc->currentPage()->xOffset();
					double y = doc->currentPage()->yOffset();
					for (uint i=0; i < pastedObjects.count(); ++i)
						if (! pastedObjects.itemAt(i)->isEmbedded)
						{
						//	const Page* pg = doc->Pages->at(doc->OnPage(pastedObjects.itemAt(i)));
						//	if (!pg)
						//		pg = doc->Pages->at(doc->Pages->count() - 1);
						//	if (pg)
						//		pastedObjects.itemAt(i)->moveBy(x - pg->xOffset(), y - pg->yOffset(), true);
							qDebug(QString("move pasted: %1 %2,%3 + %4,%5").arg((ulong)pastedObjects.itemAt(i)).arg(x).arg(y).arg(pastedObjects.itemAt(i)->xPos()).arg(pastedObjects.itemAt(i)->yPos()));
							pastedObjects.itemAt(i)->moveBy(x, y, true);
						}
					*/
				}

				// update style lists:
				styleManager->setDoc(doc);
				propertiesPalette->unsetDoc();
				propertiesPalette->setDoc(doc);

				doc->useRaster = savedAlignGrid;
				doc->SnapGuides = savedAlignGuides;
				for (int as = ac; as < doc->Items->count(); ++as)
				{
					PageItem* currItem = doc->Items->at(as);
					if (currItem->isBookmark)
						AddBookMark(currItem);
					doc->m_Selection->addItem(currItem);
				}
				int docSelectionCount=doc->m_Selection->count();
				if (docSelectionCount > 1)
				{
					doc->m_Selection->setGroupRect();
//					view->paintGroupRect();
					double x, y, w, h;
					doc->m_Selection->getGroupRect(&x, &y, &w, &h);
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
			if (nextItem->prevInChain() != 0)
				nextItem = nextItem->prevInChain();
			else
				break;
		}
		while (nextItem != 0)
		{
			nextItem->itemText.selectAll();
			nextItem->HasSel = true;
			nextItem = nextItem->nextInChain();
		}
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
				doc->m_Selection->addItem(currItem);
			}
		}
		int docSelectionCount=doc->m_Selection->count();
		if (docSelectionCount > 1)
		{
			double x, y, w, h;
			doc->m_Selection->setGroupRect();
			doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			propertiesPalette->setXY(x, y);
			propertiesPalette->setBH(w, h);
		}
		if (docSelectionCount > 0)
		{
			currItem = doc->m_Selection->itemAt(0);
			HaveNewSel(currItem->itemType());
		}
	}
	view->DrawNew();
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
//	cc = ClipB->text(QClipboard::Selection);
//	if (cc.isNull())
	cc = ClipB->text(QClipboard::Clipboard);
#else
	cc = ClipB->text();
#endif

	if (!cc.isNull())
	{
		if (!BuFromApp)
			Buffer2 = cc;
		BuFromApp = false;
	}
	scrActions["editPaste"]->setEnabled(HaveDoc &&
										(hasXMLRootElem(Buffer2, "<SCRIBUSELEM")
										 || hasXMLRootElem(Buffer2, "<SCRIBUSFRAGMENT")
										 || doc->appMode == modeEdit));
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

void ScribusMainWindow::slotHelpCheckUpdates()
{
	About* dia = new About(this, About::CheckUpdates);
	dia->exec();
	delete dia;
}

void ScribusMainWindow::slotOnlineHelp()
{
	helpBrowser = new HelpBrowser(0, tr("Scribus Manual"), ScCore->getGuiLanguage());
	connect(helpBrowser, SIGNAL(closed()), this, SLOT(slotOnlineHelpClosed()));
	helpBrowser->show();
}

void ScribusMainWindow::slotOnlineHelpClosed()
{
	delete helpBrowser;
}

void ScribusMainWindow::ToggleTips()
{
	//qt4 consume in event filter QToolTip::setGloballyEnabled(scrActions["helpTooltips"]->isChecked());
	prefsManager->appPrefs.showToolTips = scrActions["helpTooltips"]->isChecked();
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
		prefsManager->prefsFile->getContext("dirs")->set("save_text", fn.left(fn.lastIndexOf("/")));
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
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	view->Deselect(true);
	slotNewPage(wo, templ); //master page is applied now
	//applyNewMaster(templ);
	doc->addPageToSection(wo, 1, 1);
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	pagePalette->rebuildPages();
}

/** Erzeugt eine neue Seite */
void ScribusMainWindow::slotNewPageM()
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	view->Deselect(true);
	QStringList base;
	InsPage *dia = new InsPage(this, doc, doc->currentPage()->pageNr(), doc->Pages->count());
	if (dia->exec())
	{
		base = dia->getMasterPages();
		addNewPages(dia->getWherePage(),
		            dia->getWhere(),
		            dia->getCount(),
					dia->heightSpinBox->value() / doc->unitRatio(),
					dia->widthSpinBox->value() / doc->unitRatio(),
					dia->orientationQComboBox->currentIndex(),
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
		undoManager->beginTransaction(doc->getUName(), Um::IDocument, (numPages == 1) ? Um::AddPage : Um::AddPages, "", Um::ICreate);
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
	//Use wo, the dialog currently returns a page Index +1 due to old numbering scheme, function now does the -1 as required
	doc->addPageToSection(wo, where, numPages);
	pagePalette->rebuildPages();
	view->reformPages(mov);
	view->DrawNew();
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();

	undoManager->setUndoEnabled(true);

	if (UndoManager::undoEnabled())
		undoManager->commit();
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
}


void ScribusMainWindow::duplicateToMasterPage()
{
	if (!HaveDoc)
		return;
	view->Deselect(true);
	int pageLocationIndex=-1;
	int pageLocationCount=0;
	if (doc->currentPageLayout != singlePage)
	{
		QStringList locationEntries;
		for(QStringList::Iterator pNames = doc->pageSets[doc->currentPageLayout].pageNames.begin(); pNames != doc->pageSets[doc->currentPageLayout].pageNames.end(); ++pNames )
		{
			locationEntries << CommonStrings::translatePageSetLocString(*pNames);
		}
		if (doc->currentPage()->LeftPg == 1)
			pageLocationIndex=0;
		else if (doc->currentPage()->LeftPg == 0)
			pageLocationIndex=locationEntries.count()-1;
		else
			pageLocationIndex=doc->currentPage()->LeftPg-1;
		pageLocationCount=locationEntries.count();
	}

	CopyPageToMasterPageDialog copyDialog(doc->MasterNames.count(), doc->pageSets[doc->currentPageLayout].pageNames, pageLocationIndex, this);
	if (copyDialog.exec())
	{
		bool copyFromMaster=false;
		QString masterPageName;
		int pageLocation=0;
		copyDialog.values(masterPageName, copyFromMaster, pageLocation);
		while (doc->MasterNames.contains(masterPageName) || (masterPageName == CommonStrings::masterPageNormal))
		{
			if (!copyDialog.exec())
				return;
			copyDialog.values(masterPageName, copyFromMaster, pageLocation);
		}
		int currentPageNumber=doc->currentPage()->pageNr();
		bool ok=doc->copyPageToMasterPage(currentPageNumber, pageLocation, pageLocationCount, masterPageName, copyFromMaster);
		Q_ASSERT(ok); //TODO get a return value in case the copy was not possible
		pagePalette->Rebuild();
	}
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
	else if (zoomFactor==-200.0)
	{
		double dx = (view->width()-50) / (doc->currentPage()->width()+30);
		double dy = (view->height()-70) / (doc->currentPage()->height()+30);
		finalZoomFactor = (dx < dy) ? dy : dx;
	}
	//Zoom to %
	else
		finalZoomFactor = zoomFactor*prefsManager->displayScale()/100.0;
	if (finalZoomFactor == view->scale())
		return;
	int x = qRound(qMax(view->contentsX() / view->scale(), 0.0));
	int y = qRound(qMax(view->contentsY() / view->scale(), 0.0));
	int w = qRound(qMin(view->visibleWidth() / view->scale(), doc->currentPage()->width()));
	int h = qRound(qMin(view->visibleHeight() / view->scale(), doc->currentPage()->height()));
	if (zoomFactor==-200.0)
		view->rememberOldZoomLocation(qRound(doc->currentPage()->xOffset() + doc->currentPage()->width() / 2.0), qRound(doc->currentPage()->yOffset() + doc->currentPage()->height() / 2.0));
	else
		view->rememberOldZoomLocation(w / 2 + x,h / 2 + y);
	view->setScale(finalZoomFactor);
	view->slotDoZoom();
}

void ScribusMainWindow::ToggleStickyTools()
{
	prefsManager->appPrefs.stickyTools = !prefsManager->appPrefs.stickyTools;
	scrActions["stickyTools"]->setChecked(prefsManager->appPrefs.stickyTools);
	if (HaveDoc && doc->appMode!=modeNormal && !prefsManager->appPrefs.stickyTools)
		view->requestMode(modeNormal);
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
//		if (palettesStatus[7])
//			measurementPalette->show();
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
//		palettesStatus[7] = measurementPalette->isVisible();
		palettesStatus[8] = undoPalette->isVisible();
		palettesStatus[9] = docCheckerPalette->isVisible();
		propertiesPalette->hide();
		outlinePalette->hide();
		scrapbookPalette->hide();
		bookmarkPalette->hide();
		pagePalette->hide();
		layerPalette->hide();
//		measurementPalette->hide();
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
	scrActions["toolsActionHistory"]->setChecked(visible);
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
	if (doc)
	{
		doc->guidesSettings.showPic = !doc->guidesSettings.showPic;
		for (int b=0; b<doc->Items->count(); ++b)
		{
			if (doc->Items->at(b)->asImageFrame())
				doc->Items->at(b)->setImageShown(doc->guidesSettings.showPic);
		}
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleAllGuides()
{
	if (!doc)
		return;
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
		doc->guidesSettings.layerMarkersShown = guidesStatus[11] ;
		doc->guidesSettings.showBleed = guidesStatus[12] ;
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
		ToggleBleeds();
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
		guidesStatus[12] = !doc->guidesSettings.showBleed;
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
		doc->guidesSettings.showBleed = false;
		view->setRulersShown(doc->guidesSettings.rulersShown);
	}
	scrActions["viewShowMargins"]->setChecked(doc->guidesSettings.marginsShown);
	scrActions["viewShowBleeds"]->setChecked(doc->guidesSettings.showBleed);
	scrActions["viewShowFrames"]->setChecked(doc->guidesSettings.framesShown);
	scrActions["viewShowLayerMarkers"]->setChecked(doc->guidesSettings.layerMarkersShown);
	scrActions["viewShowGrid"]->setChecked(doc->guidesSettings.gridShown);
	scrActions["viewShowGuides"]->setChecked(doc->guidesSettings.guidesShown);
	scrActions["viewShowColumnBorders"]->setChecked(doc->guidesSettings.colBordersShown);
	scrActions["viewShowBaseline"]->setChecked(doc->guidesSettings.baseShown);
	scrActions["viewShowTextChain"]->setChecked(doc->guidesSettings.linkShown);
	scrActions["viewShowTextControls"]->setChecked(doc->guidesSettings.showControls);
	scrActions["viewShowRulers"]->setChecked(doc->guidesSettings.rulersShown);
	scrActions["viewRulerMode"]->setChecked(doc->guidesSettings.rulerMode);
	view->DrawNew();
}

void ScribusMainWindow::ToggleMarks()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.marginsShown = !doc->guidesSettings.marginsShown;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleBleeds()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.showBleed = !doc->guidesSettings.showBleed;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleFrames()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.framesShown = !doc->guidesSettings.framesShown;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleLayerMarkers()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.layerMarkersShown = !doc->guidesSettings.layerMarkersShown;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleRaster()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.gridShown = !doc->guidesSettings.gridShown;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleGuides()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.guidesShown = !doc->guidesSettings.guidesShown;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleColumnBorders()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.colBordersShown = !doc->guidesSettings.colBordersShown;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleBase()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.baseShown = !doc->guidesSettings.baseShown;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleTextLinks()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.linkShown = !doc->guidesSettings.linkShown;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleTextControls()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.showControls = !doc->guidesSettings.showControls;
		view->DrawNew();
	}
}

void ScribusMainWindow::ToggleRulers()
{
	if (doc)
	{
		guidesStatus[0] = false;
		doc->guidesSettings.rulersShown = !doc->guidesSettings.rulersShown;
		view->setRulersShown(doc->guidesSettings.rulersShown);
	}
}

void ScribusMainWindow::ToggleRulerMode()
{
	if (doc)
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
}

void ScribusMainWindow::ToggleURaster()
{
	if (doc)
	{
		doc->useRaster = !doc->useRaster;
		slotDocCh();
	}
}

void ScribusMainWindow::ToggleUGuides()
{
	if (doc)
	{
		doc->SnapGuides = !doc->SnapGuides;
		slotDocCh();
	}
}


void ScribusMainWindow::toggleNodeEdit()
{
	if (!doc)
		return;

	if (doc->appMode == modeEditClip)
	{
		view->requestMode(submodeEndNodeEdit);
	}
	else
	{
		view->requestMode(modeEditClip);
	}
}

void ScribusMainWindow::ToggleFrameEdit()
{
	if (!doc)
		return;
	if (doc->appMode == modeEditClip)
	{
		NoFrameEdit();
	}
	else
	{
		//CB Enable/Disable undo in frame edit mode
// 		undoManager->setUndoEnabled(false);
		scrActions["editUndoAction"]->setEnabled(false);
		scrActions["editRedoAction"]->setEnabled(false);
//done elsewhere now		slotSelect();
		nodePalette->setDoc(doc, view);
		nodePalette->MoveN();
		nodePalette->HaveNode(false, false);
		nodePalette->MoveNode->setChecked(true);
		nodePalette->show();
		qDebug() << "nodepalette show:" << nodePalette->geometry();
		connect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
		doc->nodeEdit.reset();
//done elsewhere now		doc->appMode = modeEditClip;
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
		scrActions["toolsInsertLatexFrame"]->setEnabled(false);
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
		layerPalette->setEnabled(false);
		outlinePalette->setEnabled(false);
		guidePalette->setEnabled(false);
		scrapbookPalette->setEnabled(false);
		pagePalette->setEnabled(false);
		bookmarkPalette->setEnabled(false);
		docCheckerPalette->setEnabled(false);
		styleManager->setEnabled(false);
		alignDistributePalette->setEnabled(false);
		view->pageSelector->setEnabled(false);
		view->layerMenu->setEnabled(false);
		if (doc->m_Selection->count() != 0)
		{
			PageItem *currItem = doc->m_Selection->itemAt(0);
//			view->MarkClip(currItem, currItem->PoLine, true);
			nodePalette->EditCont->setEnabled(currItem->ContourLine.size() != 0);
			nodePalette->ResetCont->setEnabled(false);
			nodePalette->ResetContClip->setEnabled(false);
			nodePalette->PolyStatus(currItem->itemType(), currItem->PoLine.size());
			if ((currItem->asImageFrame()) && (currItem->imageClip.size() != 0))
			{
				nodePalette->ResetContClip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3)));
				nodePalette->ResetContClip->show();
				nodePalette->layout()->activate();
				nodePalette->resize(QSize(170, 380).expandedTo(nodePalette->minimumSizeHint()));
			}
			else
			{
				nodePalette->ResetContClip->setSizePolicy(QSizePolicy(static_cast<QSizePolicy::Policy>(6), static_cast<QSizePolicy::Policy>(6)));
				nodePalette->layout()->activate();
				nodePalette->ResetContClip->hide();
				nodePalette->layout()->activate();
				nodePalette->resize(QSize(170, 380).expandedTo(nodePalette->minimumSizeHint()));
			}
		}
	}
	scrActions["itemShapeEdit"]->setChecked(doc->appMode == modeEditClip);
}

void ScribusMainWindow::NoFrameEdit()
{
	disconnect(view, SIGNAL(HavePoint(bool, bool)), nodePalette, SLOT(HaveNode(bool, bool)));
	actionManager->disconnectModeActions();
	nodePalette->setDoc(0,0);
	nodePalette->hide();
	qDebug() << "nodepalette hide";
	scrActions["toolsSelect"]->setEnabled(true);
	scrActions["toolsSelect"]->setChecked(true);
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
	scrActions["toolsInsertLatexFrame"]->setEnabled(true);
	scrActions["toolsPDFPushButton"]->setEnabled(true);
	scrActions["toolsPDFTextField"]->setEnabled(true);
	scrActions["toolsPDFCheckBox"]->setEnabled(true);
	scrActions["toolsPDFComboBox"]->setEnabled(true);
	scrActions["toolsPDFListBox"]->setEnabled(true);
	scrActions["toolsPDFAnnotText"]->setEnabled(true);
	scrActions["toolsPDFAnnotLink"]->setEnabled(true);
	scrActions["toolsEditContents"]->setChecked(false);
	scrActions["toolsEditWithStoryEditor"]->setChecked(false);
	scrActions["toolsMeasurements"]->setEnabled(true);
	scrActions["toolsCopyProperties"]->setEnabled(true);
	scrActions["toolsEyeDropper"]->setEnabled(true);
	scrActions["toolsUnlinkTextFrame"]->setEnabled(true);
	scrActions["itemDelete"]->setEnabled(true);
	scrActions["itemShapeEdit"]->setChecked(false);
	layerPalette->setEnabled(true);
	outlinePalette->setEnabled(true);
	guidePalette->setEnabled(true);
	scrapbookPalette->setEnabled(true);
	pagePalette->setEnabled(true);
	bookmarkPalette->setEnabled(true);
	docCheckerPalette->setEnabled(true);
	styleManager->setEnabled(true);
	alignDistributePalette->setEnabled(true);
	view->pageSelector->setEnabled(true);
	view->layerMenu->setEnabled(true);
// 	bool tmpClip = doc->EditClip; // for enabling undo if exiting shape edit mode
	if (HaveDoc)
	{
// done elsewhere now:		doc->appMode = modeNormal;
		doc->nodeEdit.reset();
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

/** This is the safest method to return to modeNormal
*/
void ScribusMainWindow::slotSelect()
{
	if (doc)
		view->requestMode(modeNormal);
	else
		setAppMode(modeNormal);
}

void ScribusMainWindow::setAppModeByToggle(bool isOn, int newMode)
{
	keyrep=false;
	if (doc && isOn)
		view->requestMode(newMode);
	else
		slotSelect();
}

void ScribusMainWindow::setAppMode(int mode)
{
	assert(mode < submodeFirstSubmode);

	//disconnect the tools actions so we dont fire them off
	actionManager->disconnectModeActions();
	//set the actions state based on incoming mode
	scrActions["toolsSelect"]->setChecked(mode==modeNormal);
	scrActions["toolsInsertTextFrame"]->setChecked(mode==modeDrawText);
	scrActions["toolsInsertImageFrame"]->setChecked(mode==modeDrawPicture);
	scrActions["toolsInsertTableFrame"]->setChecked(mode==modeDrawTable);
	scrActions["toolsInsertShape"]->setChecked(mode==modeDrawShapes);
	scrActions["toolsInsertPolygon"]->setChecked(mode==modeDrawRegularPolygon);
	scrActions["toolsInsertLine"]->setChecked(mode==modeDrawLine);
	scrActions["toolsInsertBezier"]->setChecked(mode==modeDrawBezierLine);
	scrActions["toolsInsertFreehandLine"]->setChecked(mode==modeDrawFreehandLine);
	scrActions["toolsInsertLatexFrame"]->setChecked(mode==modeDrawLatex);
	scrActions["toolsRotate"]->setChecked(mode==modeRotation);
	scrActions["toolsZoom"]->setChecked(mode==modeMagnifier);
	scrActions["toolsEditContents"]->setChecked(mode==modeEdit);
	scrActions["toolsEditWithStoryEditor"]->setChecked(mode==modeStoryEditor);
	scrActions["toolsLinkTextFrame"]->setChecked(mode==modeLinkFrames);
	scrActions["toolsUnlinkTextFrame"]->setChecked(mode==modeUnlinkFrames);
	scrActions["toolsEyeDropper"]->setChecked(mode==modeEyeDropper);
	scrActions["toolsMeasurements"]->setChecked(mode==modeMeasurementTool);
	scrActions["toolsCopyProperties"]->setChecked(mode==modeCopyProperties);
	scrActions["toolsPDFPushButton"]->setChecked(mode==modeInsertPDFButton);
	scrActions["toolsPDFTextField"]->setChecked(mode==modeInsertPDFTextfield);
	scrActions["toolsPDFCheckBox"]->setChecked(mode==modeInsertPDFCheckbox);
	scrActions["toolsPDFComboBox"]->setChecked(mode==modeInsertPDFCombobox);
	scrActions["toolsPDFListBox"]->setChecked(mode==modeInsertPDFListbox);
	scrActions["toolsPDFAnnotText"]->setChecked(mode==modeInsertPDFTextAnnotation);
	scrActions["toolsPDFAnnotLink"]->setChecked(mode==modeInsertPDFLinkAnnotation);

	if (HaveDoc)
	{
		PageItem *currItem=0;
		if (doc->m_Selection->count() != 0)
			currItem = doc->m_Selection->itemAt(0);
		int oldMode = doc->appMode;
		if (oldMode == modeEditClip && mode != modeEditClip)
			NoFrameEdit();
		else if (oldMode != modeEditClip && mode == modeEditClip)
			ToggleFrameEdit();
		doc->appMode = mode;
//		if (oldMode == modeMeasurementTool)
//			disconnect(view, SIGNAL(MVals(double, double, double, double, double, double, int )), measurementPalette, SLOT(setValues(double, double, double, double, double, double, int )));
/*		if (mode != modeEdit && doc->CurTimer!=NULL)
		{
			disconnect(doc->CurTimer, SIGNAL(timeout()), view, SLOT(blinkCursor()));
			doc->CurTimer->stop();
			delete doc->CurTimer;
			doc->CurTimer = NULL;
		} */
		if (mode!=modeEdit && oldMode==modeEdit)
			actionManager->restoreActionShortcutsPostEditMode();
		else
		if (mode==modeEdit && oldMode!=modeEdit)
			actionManager->saveActionShortcutsPreEditMode();
		if (oldMode == modeEdit)
		{
			view->zoomSpinBox->setFocusPolicy(Qt::ClickFocus);
			view->pageSelector->setFocusPolicy(Qt::ClickFocus);
			scrActions["editClearContents"]->setEnabled(false);
			charPalette->setEnabled(false, 0);
//			view->slotDoCurs(false);
			if (currItem != 0)
			{
				currItem->update();
				scrMenuMgr->setMenuEnabled("Style", true);
				scrMenuMgr->setMenuEnabled("Item", true);
			}
			view->horizRuler->ItemPosValid = false;
			view->horizRuler->repaint();
		}
		if (mode == modeEdit)
		{
//			view->zoomSpinBox->setFocusPolicy(QWidget::NoFocus);
// 			view->pageSelector->setFocusPolicy(QWidget::NoFocus);
			if (currItem != 0)
			{
//				if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::PathText))
//				{
//					doc->appMode = modeNormal;
//					view->requestMode(modeEditClip);
//					return;
//				}
				setTBvals(currItem);
				currItem->CPos = 0;
			}
			scrActions["editPaste"]->setEnabled(false);
			charPalette->setEnabled(true, currItem);
			if (currItem!=NULL && currItem->asTextFrame())
				enableTextActions(&scrActions, true, currItem->currentCharStyle().font().scName());
			if (!Buffer2.isNull())
			{
//				if (!hasXMLRootElem(Buffer2, "<SCRIBUSELEM"))
//				{
					BuFromApp = false;
					scrActions["editPaste"]->setEnabled(true);
//				}
			}
//			view->slotDoCurs(true);
			scrMenuMgr->setMenuEnabled("Item", false);
			scrMenuMgr->setMenuEnabled("Style", false);
/*			doc->CurTimer = new QTimer(view);
			if (doc->CurTimer!=NULL)
			{
				connect(doc->CurTimer, SIGNAL(timeout()), view, SLOT(blinkCursor()));
				doc->CurTimer->start(500);
			} */
			if (currItem != 0)
			{
				scrActions["editCut"]->setEnabled(currItem->HasSel);
				scrActions["editCopy"]->setEnabled(currItem->HasSel);
				scrActions["editClearContents"]->setEnabled(currItem->HasSel);
				scrActions["editSearchReplace"]->setEnabled(true);

				currItem->update();
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
//		if (mode == modeMeasurementTool)
//		{
//			measurementPalette->show();
//			connect(view, SIGNAL(MVals(double, double, double, double, double, double, int)), measurementPalette, SLOT(setValues(double, double, double, double, double, double, int )));
//		}
		switch (mode)
		{
			case modeDrawShapes:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")));
				break;
			case modeDrawPicture:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")));
				break;
			case modeDrawLatex:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawLatexFrame.xpm")));
				break;
			case modeDrawText:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")));
				break;
			case modeDrawTable:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawTable.xpm")));
				break;
			case modeDrawRegularPolygon:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawPolylineFrame.xpm")));
				break;
			case modeMagnifier:
				if (docSelectionCount!=0)
					view->Deselect(true);
				view->Magnify = true;
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
				break;
			case modePanning:
				qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
				break;
			case modeDrawLine:
			case modeDrawBezierLine:
				qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
				break;
			case modeDrawFreehandLine:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawFreeLine.png"), 0, 32));
				break;
			case modeEyeDropper:
				qApp->changeOverrideCursor(QCursor(loadIcon("colorpickercursor.png"), 0, 32));
				break;
			case modeInsertPDFButton:
			case modeInsertPDFTextfield:
			case modeInsertPDFCheckbox:
			case modeInsertPDFCombobox:
			case modeInsertPDFListbox:
			case modeInsertPDFTextAnnotation:
			case modeInsertPDFLinkAnnotation:
				if (docSelectionCount!=0)
					view->Deselect(true);
				qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
				break;
			case modeMeasurementTool:
			case modeEditGradientVectors:
				qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
				break;
			default:
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			break;
		}
		if (mode == modeDrawShapes)
		{
			doc->SubMode = modeToolBar->SubMode;
			doc->ShapeValues = modeToolBar->ShapeVals;
			doc->ValCount = modeToolBar->ValCount;
			propertiesPalette->SCustom->setIcon(propertiesPalette->SCustom->getIconPixmap(doc->SubMode));
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
			doc->regionsChanged()->update(QRect());

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
			activateWindow();
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
				{
					if (doc->m_Selection->count() != 0)
					{
						bool correctAppMode = false;
						if (ai.forAppMode.count() == 0)
							correctAppMode = true;
						else if (ai.forAppMode.contains(doc->appMode))
							correctAppMode = true;
						if (correctAppMode)
						{
							if (ai.needsNumObjects == -1)
								pluginAction->setEnabled(true);
							else
							{
								if (ai.needsNumObjects > 2)
								{
									bool setter = true;
									for (int bx = 0; bx < doc->m_Selection->count(); ++bx)
									{
										if (ai.notSuitableFor.contains(doc->m_Selection->itemAt(bx)->itemType()))
											setter = false;
									}
									pluginAction->setEnabled(setter);
								}
								else
								{
									if (doc->m_Selection->count() == ai.needsNumObjects)
									{
										if (ai.needsNumObjects == 2)
										{
											int sel1 = doc->m_Selection->itemAt(0)->itemType();
											int sel2 = doc->m_Selection->itemAt(1)->itemType();
											if (ai.notSuitableFor.contains(sel1))
												pluginAction->setEnabled(false);
											else if (ai.notSuitableFor.contains(sel2))
												pluginAction->setEnabled(false);
											else
											{
												if ((ai.firstObjectType.count() == 0) && (ai.secondObjectType.count() == 0))
													pluginAction->setEnabled(true);
												else if ((ai.firstObjectType.count() == 0) && (ai.secondObjectType.count() != 0))
												{
													if ((ai.secondObjectType.contains(sel2)) || (ai.secondObjectType.contains(sel1)))
														pluginAction->setEnabled(true);
												}
												else if ((ai.firstObjectType.count() != 0) && (ai.secondObjectType.count() == 0))
												{
													if ((ai.firstObjectType.contains(sel2)) || (ai.firstObjectType.contains(sel1)))
														pluginAction->setEnabled(true);
												}
												if (((ai.firstObjectType.contains(sel1)) && (ai.secondObjectType.contains(sel2))) || ((ai.firstObjectType.contains(sel2)) && (ai.secondObjectType.contains(sel1))))
													pluginAction->setEnabled(true);
											}
										}
										else if (!ai.notSuitableFor.contains(doc->m_Selection->itemAt(0)->itemType()))
											pluginAction->setEnabled(true);
										else
											pluginAction->setEnabled(false);
									}
									else
										pluginAction->setEnabled(false);
								}
							}
						}
						else
							pluginAction->setEnabled(false);
					}
					else
					{
						bool correctAppMode = false;
						if (ai.forAppMode.count() == 0)
							correctAppMode = true;
						else if (ai.forAppMode.contains(doc->appMode))
							correctAppMode = true;
						if (correctAppMode)
						{
							if ((ai.needsNumObjects == -1) || (ai.needsNumObjects > 2))
								pluginAction->setEnabled(true);
							else
								pluginAction->setEnabled(false);
						}
						else
							pluginAction->setEnabled(false);
					}
				}
			}
		}
	}
	actionManager->connectModeActions();
}

void ScribusMainWindow::setMainWindowActive()
{
	activateWindow();
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
	setItemHoch(b);
}

void ScribusMainWindow::setStilvalue(int s)
{
	int c = s & 1919;
//	doc->currentStyle.charStyle().setFeatures(static_cast<StyleFlag>(c).featureList());
	scrActions["typeEffectNormal"]->setChecked(c==0);
	scrActions["typeEffectSuperscript"]->setChecked(c & 1);
	scrActions["typeEffectSubscript"]->setChecked(c & 2);
	scrActions["typeEffectOutline"]->setChecked(c & 4);
	scrActions["typeEffectUnderline"]->setChecked(c & 8);
	scrActions["typeEffectStrikeThrough"]->setChecked(c & 16);
	scrActions["typeEffectAllCaps"]->setChecked(c & 32);
	scrActions["typeEffectSmallCaps"]->setChecked(c & 64);
	scrActions["typeEffectShadow"]->setChecked(c & 256);
	scrActions["typeEffectUnderlineWords"]->setChecked(c & 512);
	emit TextStil(s);
}

void ScribusMainWindow::setItemHoch(int h)
{
	if (doc->m_Selection->count() != 0)
	{
//		doc->currentStyle.charStyle().setFeatures(static_cast<StyleFlag>(h).featureList());
		setStilvalue(h);
		doc->itemSelection_SetEffects(h);
	}
}

//CB-->Doc partly
void ScribusMainWindow::DeletePage2(int pg)
{
	PageItem* ite;
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	if (doc->Pages->count() == 1)
		return;
	int oldPg = doc->currentPageNumber();
	guidePalette->setDoc(NULL);
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(doc->DocName, Um::IDocument, Um::DeletePage, "", Um::IDelete);
/*	if (!doc->masterPageMode)
		disconnect(doc->currentPage, SIGNAL(DelObj(uint, uint)), outlinePalette, SLOT(slotRemoveElement(uint, uint))); */
	doc->m_Selection->clear();
	Selection tmpSelection(this, false);
	for (int d = 0; d < doc->Items->count(); ++d)
	{
		ite = doc->Items->at(d);
		if (ite->OwnPage == pg)
		{
			ite->setLocked(false);
			ite->isSingleSel = false;
			if (ite->isBookmark)
				DelBookMark(ite);
			ite->isBookmark = false;
			tmpSelection.addItem(ite);
		}
	}
	if (tmpSelection.count() != 0)
		doc->itemSelection_DeleteItem(&tmpSelection);
	Page *page = doc->Pages->at(pg); // need to remove guides too to get their undo/redo actions working
	page->guides.clearHorizontals(GuideManagerCore::Standard);
	page->guides.clearHorizontals(GuideManagerCore::Auto);
	page->guides.clearVerticals(GuideManagerCore::Standard);
	page->guides.clearVerticals(GuideManagerCore::Auto);
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::DeletePage, "", Um::ICreate);
		ss->set("DELETE_PAGE", "delete_page");
		ss->set("PAGENR", pg + 1);
		ss->set("PAGENAME", doc->Pages->at(pg)->pageName());
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
	view->pageSelector->setMaximum(doc->Pages->count());
	view->pageSelector->GotoPg(0);
	connect(view->pageSelector, SIGNAL(GotoPage(int)), view, SLOT(GotoPa(int)));
	undoManager->setUndoEnabled(false); // ugly hack to prevent object moving when undoing page deletion
	view->reformPages();
	undoManager->setUndoEnabled(true); // ugly hack continues
	view->GotoPage(qMin(doc->Pages->count()-1, oldPg));
	view->DrawNew();
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	//CB done by doc::reformpages
	//slotDocCh();
	doc->rebuildMasterNames();
	pagePalette->rebuildPages();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusMainWindow::DeletePage()
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	view->Deselect(true);
	DelPages *dia = new DelPages(this, doc->currentPage()->pageNr()+1, doc->Pages->count());
	if (dia->exec())
		DeletePage(dia->getFromPage(), dia->getToPage());
	delete dia;
}

void ScribusMainWindow::DeletePage(int from, int to)
{
	assert( from > 0 );
	assert( from <= to );
	assert( to <= static_cast<int>(doc->Pages->count()) );
	int oldPg = doc->currentPageNumber();
	guidePalette->setDoc(NULL);
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(doc->DocName, Um::IDocument,
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
		Page *page = doc->Pages->at(a); // need to remove guides too to get their undo/redo actions working
		page->guides.clearHorizontals(GuideManagerCore::Standard);
		page->guides.clearHorizontals(GuideManagerCore::Auto);
		page->guides.clearVerticals(GuideManagerCore::Standard);
		page->guides.clearVerticals(GuideManagerCore::Auto);
	}
	if (tmpSelection.count() != 0)
		doc->itemSelection_DeleteItem(&tmpSelection);
	for (int a = to - 1; a >= from - 1; a--)
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::DeletePage, "", Um::ICreate);
			ss->set("DELETE_PAGE", "delete_page");
			ss->set("PAGENR", a + 1);
			ss->set("PAGENAME",   doc->Pages->at(a)->pageName());
			ss->set("MASTERPAGE", doc->Pages->at(a)->MPageNam);
			// replace the deleted page in the undostack by a dummy object that will
			// replaced with the "undone" page if user choose to undo the action
			DummyUndoObject *duo = new DummyUndoObject();
			uint id = static_cast<uint>(duo->getUId());
			undoManager->replaceObject(doc->Pages->at(a)->getUId(), duo);
			ss->set("DUMMY_ID", id);
			undoManager->action(this, ss);
		}
		bool isMasterPage = !(doc->Pages->at(a)->pageName().isEmpty());
		if (doc->masterPageMode())
			doc->deleteMasterPage(a);
		else
			doc->deletePage(a);
		disconnect(view->pageSelector, SIGNAL(GotoPage(int)), view, SLOT(GotoPa(int)));
		view->pageSelector->setMaximum(doc->Pages->count());
		view->pageSelector->GotoPg(0);
		connect(view->pageSelector, SIGNAL(GotoPage(int)), view, SLOT(GotoPa(int)));
		if (!isMasterPage) // Master pages are not added to sections when created
			doc->removePageFromSection(a);
	}
	undoManager->setUndoEnabled(false); // ugly hack to disable object moving when undoing page deletion
	view->reformPages();
	undoManager->setUndoEnabled(true); // ugly hack continues
	view->GotoPage(qMin(doc->Pages->count()-1, oldPg));
	view->DrawNew();
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	doc->rebuildMasterNames();
	pagePalette->rebuildPages();
	pagePalette->rebuildMasters();
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusMainWindow::MovePage()
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	MovePages *dia = new MovePages(this, doc->currentPage()->pageNr()+1, doc->Pages->count(), true);
	if (dia->exec())
	{
		int from = dia->getFromPage();
		int to = dia->getToPage();
		int wie = dia->getWhere();
		int wo = dia->getWherePage();
		if (from != wo || (wie == 2 && to != signed(doc->Pages->count()) ) )
		{
			doc->movePage(from-1, to, wo-1, wie);
			view->reformPages();
			view->DrawNew();
			pagePalette->rebuildPages();
			if (outlinePalette->isVisible())
				outlinePalette->BuildTree();
		}
	}
	delete dia;
}

void ScribusMainWindow::CopyPage()
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
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
		pagePalette->rebuildPages();
		if (outlinePalette->isVisible())
			outlinePalette->BuildTree();
	}
	delete dia;
}

void ScribusMainWindow::changePageMargins()
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
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

void ScribusMainWindow::SetNewFont(const QString& nf)
{
	setMainWindowActive();
	QString nf2(nf);
	if (!doc->UsedFonts.contains(nf))
	{
		if (doc->AddFont(nf)) //, prefsManager->appPrefs.AvailFonts[nf]->Font))
		{
		}
		else
		{//CB FIXME: to doc?
			if (doc->m_Selection->count() != 0)
			{
				PageItem *currItem = doc->m_Selection->itemAt(0);
				nf2 = currItem->currentCharStyle().font().scName();
			}
		}
	}
	doc->itemSelection_SetFont(nf2);
//	doc->currentStyle.charStyle().setFont((*doc->AllFonts)[nf2]);
	view->DrawNew();
	slotDocCh();
}

void ScribusMainWindow::setItemFSize(int id)
{
	int c = id;
	if (c != -1)
		doc->itemSelection_SetFontSize(c*10);
	else
	{
		bool ok = false;
		Query* dia = new Query(this, "New", 1, 0, tr("&Size:"), tr("Size"));
		if (dia->exec())
		{
			c = qRound(dia->getEditText().toDouble(&ok));
			if ((ok) && (c < 1025) && (c > 0))
				doc->itemSelection_SetFontSize(c*10);
		}
		delete dia;
	}
	propertiesPalette->setSize(c*10);
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
				doc->itemSelection_SetFillShade(c);
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
						doc->itemSelection_SetFillShade(c);
					else
						doc->ItemBrushShade(c);
				}
			}
			delete dia;
		}
	}
	slotDocCh();
}

void ScribusMainWindow::setCSMenu()
{
	QString la = CommonStrings::tr_NoneColor;
	int lb = 100;
	PageItem *currItem;
	if (doc->m_Selection->count() != 0)
	{
		currItem = doc->m_Selection->itemAt(0);
		if ((currItem->asTextFrame()) || (currItem->asPathText()))
		{
			if ((doc->appMode == modeEdit) && (currItem->itemText.length() != 0))
			{
				la = currItem->itemText.charStyle(qMin(currItem->CPos, static_cast<int>(currItem->itemText.length()-1))).fillColor();
				lb = qRound(currItem->itemText.charStyle(qMin(currItem->CPos, static_cast<int>(currItem->itemText.length()-1))).fillShade());
			}
			else
			{
				la = currItem->itemText.defaultStyle().charStyle().fillColor();
				lb = qRound(currItem->itemText.defaultStyle().charStyle().fillShade());
			}
		}
		else
		{
			la = currItem->fillColor();
			lb = qRound(currItem->fillShade());
		}
	}
	if (la == CommonStrings::None)
		la = CommonStrings::tr_NoneColor;
	if (scrActions[QString("shade%1").arg(lb)])
		scrActions[QString("shade%1").arg(lb)]->setChecked(true);
}

#if 0
//CB still called from SE
void ScribusMainWindow::saveStyles(StilFormate *dia)
{
	QList<uint> ers;
	QString nn;
// 	PageItem* ite = 0;
	bool ff;
	uint nr;
	ers.clear();
	/*FIXME:NLS
	ers.append(0);
	ers.append(1);
	ers.append(2);
	ers.append(3);
	ers.append(4);
	for (uint a=5; a<doc->docParagraphStyles.count(); ++a)
*/
	for (uint a=0; a<doc->paragraphStyles().count(); ++a)
	{
		ff = false;
		nn = doc->paragraphStyles()[a].name();
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
				if (doc->paragraphStyles()[a].equiv(dia->TempVorl[b]))
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
// 	uint counter = 0;
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
/*		if (CurrStED->Editor->StyledText.count() != 0)
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
								chars->at(e)->charStyle.setFeatures(fl.featureList());
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

			}
			CurrStED->Editor->currentParaStyle = ers[CurrStED->Editor->currentParaStyle];
		}
	*/
	}
	doc->redefineStyles(dia->TempVorl);
	if (CurrStED != NULL)
	{
		if (CurrStED->Editor->StyledText.length() != 0)
			CurrStED->Editor->updateAll();
	}
	for (uint a=0; a<doc->paragraphStyles().count(); ++a)
	{
		if (!doc->paragraphStyles()[a].charStyle().font().isNone())
		{
			QString nf = doc->paragraphStyles()[a].charStyle().font().scName();
			if (!doc->UsedFonts.contains(nf))
			{
				if (doc->AddFont(nf)) //, prefsManager->appPrefs.AvailFonts[nf]->Font))
				{
				}
//				else
//FIXME					doc->paragraphStyles()[a].charStyle().setFont((prefsManager->appPrefs.AvailFonts[doc->toolSettings.defFont]));
			}
		}
	}

	propertiesPalette->paraStyleCombo->updateFormatList();
	propertiesPalette->charStyleCombo->updateFormatList();
	propertiesPalette->updateColorList();
	view->DrawNew();
	slotDocCh();
}
#endif
//CB-->Doc
void ScribusMainWindow::setNewAlignment(int a)
{
	if (HaveDoc)
	{
//		doc->currentStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(a));
		doc->itemSelection_SetAlignment(a);
		propertiesPalette->setAli(a);
		PageItem *currItem = doc->m_Selection->itemAt(0);
		setTBvals(currItem);
	}
}

void ScribusMainWindow::setNewParStyle(const QString& name)
{
	if (HaveDoc)
	{
/*		if (name.isEmpty())
		{
			doc->itemSelection_SetNamedParagraphStyle(name);
			doc->itemSelection_EraseParagraphStyle();
		}
		else */
			doc->itemSelection_SetNamedParagraphStyle(name);
		PageItem *currItem = doc->m_Selection->itemAt(0);
		setTBvals(currItem);
	}
}

void ScribusMainWindow::setNewCharStyle(const QString& name)
{
	if (HaveDoc)
	{
/*		if (name.isEmpty())
		{
			doc->itemSelection_SetNamedCharStyle(name);
			doc->itemSelection_EraseCharStyle();
		}
		else */
			doc->itemSelection_SetNamedCharStyle(name);
		PageItem *currItem = doc->m_Selection->itemAt(0);
		setTBvals(currItem);
	}
}

void ScribusMainWindow::setAbsValue(int a)
{
//	doc->currentStyle = doc->docParagraphStyles[a];
//	doc->currentStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(a<5 ? a : 0));
	propertiesPalette->setAli(a);
	QString alignment[] = {"Left", "Center", "Right", "Block", "Forced"};
	for (int b=0; b<5; ++b)
	{
		QString actionName="align"+alignment[b];
		if (scrActions[actionName])
			scrActions[actionName]->setChecked(a==b);
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
			QColor tmpc;
			slotDocCh();
			doc->PageColors = dia->EditColors;
			if (dia->replaceMap.isEmpty())
			{
				// invalidate all charstyles, as replaceNamedResources() wont do it if all maps are empty
				const StyleSet<CharStyle> dummy;
				doc->redefineCharStyles(dummy, false);
			}
			else
			{
				ResourceCollection colorrsc;
				colorrsc.mapColors(dia->replaceMap);
				// Update tools colors
				PrefsManager::replaceToolColors(doc->toolSettings, colorrsc.colors());
				// Update objects and styles colors
				doc->replaceNamedResources(colorrsc);
				// Temporary code until LineStyle is effectively used
				doc->replaceLineStyleColors(dia->replaceMap);
			}
			doc->recalculateColors();
			doc->recalcPicturesRes();
			propertiesPalette->updateColorList();
			//3102: update the line styles in PP too
			propertiesPalette->SetLineFormats(doc);
			styleManager->updateColorList();
			if (doc->m_Selection->count() != 0)
				doc->m_Selection->itemAt(0)->emitAllToGUI();
			view->DrawNew();
		}
		else
		{
			// Update tools colors if needed
			prefsManager->replaceToolColors(dia->replaceMap);
			prefsManager->setColorSet(dia->EditColors);
			prefsManager->setColorSetName(dia->getColorSetName());
			propertiesPalette->Cpal->SetColors(prefsManager->colorSet());
		}
	}
	if (!HaveDoc)
		prefsManager->appPrefs.CustomColorSets = dia->customColSet;
	delete dia;
}

void ScribusMainWindow::updtGradFill()
{
	if (!HaveDoc)
		return;
	VGradient vg(propertiesPalette->getFillGradient());
	doc->itemSelection_SetFillGradient(vg);
}
/*
//CB-->Doc
void ScribusMainWindow::GetBrushPen()
{
	//What? we come back here from mpalette and then go to the view.. someones kidding
	if (!HaveDoc)
		return;
	view->QueryFarben();
	//CB We dont need to set the doc changed just to find the colour values...
	//slotDocCh();
}
*/
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
	currItem->update();
	slotDocCh();
}

void ScribusMainWindow::duplicateItem()
{
	slotSelect();
	bool savedAlignGrid = doc->useRaster;
	bool savedAlignGuides = doc->SnapGuides;
	internalCopy = true;
	doc->useRaster = false;
	doc->SnapGuides = false;
	slotEditCopy();
	view->Deselect(true);
	slotEditPaste();
	for (int b=0; b<doc->m_Selection->count(); ++b)
	{
		doc->m_Selection->itemAt(b)->setLocked(false);
		doc->MoveItem(doc->toolSettings.dispX, doc->toolSettings.dispY, doc->m_Selection->itemAt(b));
	}
	doc->useRaster = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	internalCopy = false;
	view->DrawNew();
}

void ScribusMainWindow::duplicateItemMulti()
{
	if (!HaveDoc)
		return;
	slotSelect();
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	internalCopy = true;
	MultipleDuplicate *dia = new MultipleDuplicate(doc->unitIndex(), this);
	if (dia->exec())
	{
		ItemMultipleDuplicateData mdData;
		dia->getMultiplyData(mdData);
		doc->itemSelection_MultipleDuplicate(mdData);
	}
	internalCopy = false;
	delete dia;
}

void ScribusMainWindow::selectItemsFromOutlines(PageItem* ite)
{
	int d = doc->Items->indexOf(ite);
	selectItemsFromOutlines(ite->OwnPage, d, true);
}

void ScribusMainWindow::selectItemsFromOutlines(int Page, int Item, bool single)
{
	if (HaveDoc && doc->appMode == modeEditClip)
		view->requestMode(submodeEndNodeEdit);
	activateWindow();
	view->Deselect(true);
	if ((Page != -1) && (Page != static_cast<int>(doc->currentPage()->pageNr())))
		view->GotoPage(Page);
	doc->m_Selection->setIsGUISelection(false);
	view->SelectItemNr(Item, true, single);
	doc->m_Selection->setIsGUISelection(true);
	doc->m_Selection->connectItemToGUI();
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
//			double viewScale=view->scale();
//			if ((qRound((currItem->xPos() + qMax(x1, x2)) * viewScale) > view->contentsWidth()) ||
//				(qRound((currItem->yPos() + qMax(y1, y2)) * viewScale) > view->contentsHeight()))
//				view->resizeContents(qMax(qRound((currItem->xPos() + qMax(x1, x2)) * viewScale),
//									view->contentsWidth()),
//									qMax(qRound((currItem->yPos() + qMax(y1, y2)) * viewScale), view->contentsHeight()));
			view->SetCCPo(mx, my);
		}
		else
		{
//			double viewScale=view->scale();
//			if ((qRound((currItem->xPos() + currItem->width()) * viewScale) > view->contentsWidth()) ||
//				(qRound((currItem->yPos() + currItem->height()) * viewScale) > view->contentsHeight())
//				)
//				view->resizeContents(qMax(qRound((currItem->xPos() + currItem->width()) * viewScale), view->contentsWidth()),
//									 qMax(qRound((currItem->yPos() + currItem->height()) * viewScale), view->contentsHeight()));
			view->SetCCPo(currItem->xPos() + currItem->width() / 2.0, currItem->yPos() + currItem->height() / 2.0);
		}
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

void ScribusMainWindow::prefsOrg(Preferences *dia)
{
	//reset the appMode so we restore our tools shortcuts
	QString oldGUILanguage = prefsManager->guiLanguage();
	QString oldGUIStyle = prefsManager->guiStyle();
	int oldGUIFontSize = prefsManager->guiFontSize();
	double oldDisplayScale = prefsManager->displayScale();
	dia->updatePreferences();
	prefsManager->SavePrefs();
	DocDir = prefsManager->documentDir();
//		scrapbookPalette->rebuildView();
//		scrapbookPalette->AdjustMenu();
	QString newGUILanguage = prefsManager->guiLanguage();
	if (oldGUILanguage != newGUILanguage || ScQApp->currGUILanguage()!=newGUILanguage)
		ScQApp->changeGUILanguage(newGUILanguage);
	QString newGUIStyle = prefsManager->guiStyle();
// 	if (oldGUIStyle != newGUIStyle)
// 	{
		if (newGUIStyle == "")
			qApp->setStyle(prefsManager->guiSystemStyle());
		else
			qApp->setStyle(QStyleFactory::create(newGUIStyle));
		qApp->setPalette(qApp->style()->standardPalette());
// 	}
	int newGUIFontSize = prefsManager->guiFontSize();
	if (oldGUIFontSize != newGUIFontSize)
	{
		QFont apf = qApp->font();
		apf.setPointSize(prefsManager->appPrefs.AppFontSize);
		qApp->setFont(apf);
	}
	propertiesPalette->Fonts->RebuildList(0);
	ScCore->getCMSProfiles(false);
	ScCore->recheckGS();
	prefsManager->applyLoadedShortCuts();
	QWidgetList windows = wsp->windowList();
	if (!windows.isEmpty())
	{
		int windowCount=static_cast<int>(windows.count());
		for ( int i = 0; i < windowCount; ++i )
		{
			QWidget* w = windows.at(i);
			ScribusWin* scw = (ScribusWin*)w;
			scw->view()->setScale((scw->view()->scale() / oldDisplayScale) * prefsManager->displayScale());
			scw->view()->slotDoZoom();
		}
	}
}

void ScribusMainWindow::slotPrefsOrg()
{
	slotSelect();

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
	if (!ScCore->haveGS())
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
//	measurementPalette->startup();
	docCheckerPalette->startup();
	alignDistributePalette->startup();
	undoPalette->startup();
	guidePalette->startup();
	charPalette->startup();
	styleManager->startup();

	// init the toolbars
// 	fileToolBar->initVisibility();
// 	editToolBar->initVisibility();
// 	modeToolBar->initVisibility();
// 	pdfToolBar->initVisibility();

	activateWindow();
	raise();
}

void ScribusMainWindow::doPrintPreview()
{
	if (docCheckerPalette->isIgnoreEnabled())
	{
		docCheckerPalette->hide();
		docCheckerPalette->checkMode = CheckDocument::checkNULL;
		docCheckerPalette->setIgnoreEnabled(false);
		scrActions["toolsPreflightVerifier"]->setChecked(false);
		disconnect(docCheckerPalette, SIGNAL(ignoreAllErrors()), this, SLOT(doPrintPreview()));
	}
	if (HaveDoc)
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getContext("print_options");
		QString currentPrinter    = prefs->get("CurrentPrn");
		PrintEngine currentEngine = (PrintEngine) prefs->get("CurrentPrnEngine", "3").toInt();
		if ( PPreview::usePostscriptPreview(currentPrinter, currentEngine) && ( !ScCore->haveGS() ) )
		{
			QString mess = tr("Ghostscript is missing : Postscript Print Preview is not available")+"\n\n";
			QMessageBox::warning(this, CommonStrings::trWarning, mess, 1, 0, 0);
			return;
		}
		PPreview *dia = new PPreview(this, view, doc, currentPrinter, currentEngine);
		previewDinUse = true;
		connect(dia, SIGNAL(doPrint()), this, SLOT(slotReallyPrint()));
		dia->exec();
		PrefsManager *prefsManager=PrefsManager::instance();
		prefsManager->appPrefs.PrPr_Mode = dia->EnableCMYK->isChecked();
		prefsManager->appPrefs.PrPr_AntiAliasing = dia->AntiAlias->isChecked();
		prefsManager->appPrefs.PrPr_Transparency = dia->AliasTr->isChecked();
		if ( !ScCore->haveTIFFSep() || !dia->postscriptPreview )
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
				int t = QMessageBox::warning(this, CommonStrings::trWarning,
											"<qt>"+ tr("Scribus has detected some errors. Consider using the Preflight Verifier to correct them")+"</qt>",
											QMessageBox::Abort | QMessageBox::Ignore);
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
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
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
	options.useICC = true;
	options.doGCR = prefsManager->appPrefs.GCRMode;
	options.setDevParam = false;
	options.doClip = true;
	options.doOverprint = false;
	options.cropMarks = false;
	options.bleedMarks = false;
	options.registrationMarks = false;
	options.colorMarks = false;
	options.markOffset = 0.0;
	options.bleeds.Top = 0.0;
	options.bleeds.Left = 0.0;
	options.bleeds.Right = 0.0;
	options.bleeds.Bottom = 0.0;
	PSLib *dd = new PSLib(options, false, prefsManager->appPrefs.AvailFonts, ReallyUsed, usedColors, false, true);
	if (dd != NULL)
	{
		if (dd->PS_set_file(fn))
		{
			int psRet = dd->CreatePS(doc, options);
			if (psRet == 1)
			{
				error = dd->errorMessage();
				return_value = false;
			}
		}
		else
			return_value = false;
		delete dd;
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
				int t = QMessageBox::warning(this, CommonStrings::trWarning,
											tr("Scribus detected some errors.\nConsider using the Preflight Verifier  to correct them."),
											QMessageBox::Abort | QMessageBox::Ignore);
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
	QString fna;
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
		if (doc->m_Selection->count() != 0)
			fna = fi.path() + "/" + fi.completeBaseName() + "_selection.eps";
		else
			fna = fi.path() + "/" + getFileNameByPage(doc, doc->currentPage()->pageNr(), "eps");
	}
	else
	{
		QDir di = QDir();
		if (doc->m_Selection->count() != 0)
			fna = di.currentPath() + "/" + doc->DocName + "_selection.eps";
		else
			fna = di.currentPath() + "/" + getFileNameByPage(doc, doc->currentPage()->pageNr(), "eps");
	}
	fna = QDir::convertSeparators(fna);
	QString wdir = ".";
	QString prefsDocDir=prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		wdir = prefsManager->prefsFile->getContext("dirs")->get("eps", prefsDocDir);
	else
		wdir = prefsManager->prefsFile->getContext("dirs")->get("eps", ".");
	QString fn = CFileDialog( wdir, tr("Save as"), tr("%1;;All Files (*)").arg(formatsManager->extensionsForFormat(FormatsManager::EPS)), fna, fdNone);
	if (!fn.isEmpty())
	{
		prefsManager->prefsFile->getContext("dirs")->set("eps", fn.left(fn.lastIndexOf("/")));
		if (overwrite(this, fn))
		{
			QString epsError;
			if (!DoSaveAsEps(fn, epsError))
			{
				QString message = tr("Cannot write the file: \n%1").arg(fn);
				if (!epsError.isEmpty())
					message += QString("\n%1").arg(epsError);
				QMessageBox::warning(this, CommonStrings::trWarning, message, CommonStrings::tr_OK);
			}
		}
	}
}

bool ScribusMainWindow::getPDFDriver(const QString & fn, const QString & nam, int Components,
									 const std::vector<int> & pageNs, const QMap<int,QPixmap> & thumbs, QString& error)
{
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	PDFlib pdflib(*doc);
	bool ret = pdflib.doExport(fn, nam, Components, pageNs, thumbs);
	if (!ret)
		error = pdflib.errorMessage();
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
											QMessageBox::Abort | QMessageBox::Ignore);
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
/*	if (bookmarkPalette->BView->childCount() == 0)
		doc->PDF_Options.Bookmarks = false; */
	QMap<QString, int> ReallyUsed = doc->UsedFonts;
	if (doc->PDF_Options.EmbedList.count() != 0)
	{
		QList<QString> tmpEm;
		QList<QString>::Iterator itef;
		for (itef = doc->PDF_Options.EmbedList.begin(); itef != doc->PDF_Options.EmbedList.end(); ++itef)
		{
			if (ReallyUsed.contains((*itef)))
				tmpEm.append((*itef));
		}
		doc->PDF_Options.EmbedList = tmpEm;
	}
	if (doc->PDF_Options.SubsetList.count() != 0)
	{
		QList<QString> tmpEm;
		QList<QString>::Iterator itef;
		for (itef = doc->PDF_Options.SubsetList.begin(); itef != doc->PDF_Options.SubsetList.end(); ++itef)
		{
			if (ReallyUsed.contains((*itef)))
				tmpEm.append((*itef));
		}
		doc->PDF_Options.SubsetList = tmpEm;
	}
	MarginStruct optBleeds = doc->PDF_Options.bleeds;
	PDFExportDialog dia(this, doc->DocName, ReallyUsed, view, doc->PDF_Options, doc->PDF_Options.PresentVals, ScCore->PDFXProfiles, prefsManager->appPrefs.AvailFonts, doc->unitRatio(), ScCore->PrinterProfiles);
	if (dia.exec())
	{
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
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
		QString errorMsg;
		parsePagesString(pageString, &pageNs, doc->DocPages.count());
		if (doc->PDF_Options.useDocBleeds)
			doc->PDF_Options.bleeds = doc->bleeds;

		if (doc->PDF_Options.doMultiFile)
		{
			QFileInfo fi(fileName);
			QString ext = fi.suffix();
			QString path = fi.path();
			QString name = fi.completeBaseName();
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
					pm=QPixmap::fromImage(view->PageToPixmap(pageNs[aa]-1, 100));
				thumbs.insert(1, pm);
				QString realName = QDir::convertSeparators(path+"/"+name+ tr("-Page%1").arg(pageNs[aa], 3, 10, QChar('0'))+"."+ext);
				if (!getPDFDriver(realName, nam, components, pageNs2, thumbs, errorMsg))
				{
					qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					QString message = tr("Cannot write the file: \n%1").arg(doc->PDF_Options.Datei);
					if (!errorMsg.isEmpty())
						message = QString("%1\n%2").arg(message).arg(errorMsg);
					QMessageBox::warning(this, CommonStrings::trWarning, message, CommonStrings::tr_OK);
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
					pm=QPixmap::fromImage(view->PageToPixmap(pageNs[ap]-1, 100));
				thumbs.insert(pageNs[ap], pm);
			}
			if (!getPDFDriver(fileName, nam, components, pageNs, thumbs, errorMsg))
			{
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				QString message = tr("Cannot write the file: \n%1").arg(doc->PDF_Options.Datei);
				if (!errorMsg.isEmpty())
					message = QString("%1\n%2").arg(message).arg(errorMsg);
				QMessageBox::warning(this, CommonStrings::trWarning, message, CommonStrings::tr_OK);
			}
		}
		if (doc->PDF_Options.useDocBleeds)
			doc->PDF_Options.bleeds = optBleeds;
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::AddBookMark(PageItem *ite)
{
	bookmarkPalette->BView->AddPageItem(ite);
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::DelBookMark(PageItem *ite)
{
	bookmarkPalette->BView->DeleteItem(ite);
}

//CB-->Doc, stop _storing_ bookmarks in the palette
void ScribusMainWindow::BookMarkTxT(PageItem *ite)
{
	bookmarkPalette->BView->ChangeText(ite);
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

	ScriXmlDoc *ss = new ScriXmlDoc();
	if(ss->ReadElem(xml, prefsManager->appPrefs.AvailFonts, docc, x, y, art, loca, prefsManager->appPrefs.GFontSub, vie))
	{
		vie->DrawNew();
		if (doc == docc)
		{
			propertiesPalette->updateColorList();
			propertiesPalette->paraStyleCombo->updateFormatList();
			propertiesPalette->charStyleCombo->updateFormatList();
			propertiesPalette->SetLineFormats(docc);
			slotDocCh();
		}
	}
	// delete ss;
}

void ScribusMainWindow::slotChangeUnit(int unitIndex, bool draw)
{
	// Hack
	QByteArray stylesheet;
	if (loadRawText(ScPaths::getApplicationDataDir() + "/stylesheet.css", stylesheet))
	{
		qApp->setStyleSheet(QString(stylesheet));
	}

	doc->setUnitIndex(unitIndex);
	setCurrentComboItem(view->unitSwitcher, unitGetStrFromIndex(doc->unitIndex()));
	propertiesPalette->unitChange();
	nodePalette->unitChange();
	alignDistributePalette->unitChange();
	guidePalette->setupPage();
	styleManager->unitChange();
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
		}
	}
}

void ScribusMainWindow::manageMasterPagesEnd()
{
	view->setScale(storedViewScale);
	view->hideMasterPage();
	slotSelect();
	scrActions["editMasterPages"]->setEnabled(true);
	scrActions["fileNew"]->setEnabled(true);
	scrActions["fileOpen"]->setEnabled(true);
	scrActions["fileClose"]->setEnabled(true);
	scrActions["fileSave"]->setEnabled(true);
	scrMenuMgr->setMenuEnabled("FileOpenRecent", true);
	scrActions["fileRevert"]->setEnabled(true);
	scrActions["fileDocSetup"]->setEnabled(true);
	scrActions["filePrint"]->setEnabled(true);
	if ( ScCore->haveGS() || ScCore->isWinGUI() )
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
	pagePalette->rebuildMasters();
	ActWin->setMasterPagesPalette(NULL);
	doc->setCurrentPage(doc->DocPages.at(storedPageNum));
	view->reformPages(false);
	view->setContentsPos(static_cast<int>(storedViewXCoor * storedViewScale), static_cast<int>(storedViewYCoor * storedViewScale));
	view->DrawNew();
	pagePalette->Rebuild();
//	if (outlinePalette->isVisible())
//		outlinePalette->BuildTree();
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
	//CB done by doc::reformpages
	//slotDocCh();
	pagePalette->Rebuild();
	guidePalette->setupPage();
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
//		pagePalette->Rebuild();
	}
}

//CB-->Doc
void ScribusMainWindow::GroupObj(bool showLockDia)
{
	if (HaveDoc)
	{
		Selection* itemSelection = doc->m_Selection;
		if (itemSelection->count() < 2)
			return;
		bool lockObject = false;
		bool modifyLock = false;
		uint selectedItemCount=itemSelection->count();
		if (showLockDia)
		{
			uint lockedCount=0;
			for (uint a=0; a<selectedItemCount; ++a)
			{
				if (itemSelection->itemAt(a)->locked())
					++lockedCount;
			}
			if (lockedCount!=0 && lockedCount!=selectedItemCount)
			{
				QMessageBox msgBox;
				QPushButton *abortButton = msgBox.addButton(QMessageBox::Cancel);
				QPushButton *lockButton = msgBox.addButton(tr("&Lock All"), QMessageBox::AcceptRole);
				QPushButton *unlockButton = msgBox.addButton(tr("&Unlock All"), QMessageBox::AcceptRole);
				msgBox.setIcon(QMessageBox::Warning);
				msgBox.setWindowTitle(CommonStrings::trWarning);
				msgBox.setText( tr("Some objects are locked."));
				msgBox.exec();
				if (msgBox.clickedButton() == abortButton)
					return;
				else if (msgBox.clickedButton() == lockButton)
					lockObject = true;
				modifyLock = true;
				unlockButton = NULL;	// just to silence the compiler
			}
		}
		doc->itemSelection_GroupObjects(modifyLock, lockObject);
	}
}

//CB-->Doc
void ScribusMainWindow::UnGroupObj()
{
	if (HaveDoc)
		doc->itemSelection_UnGroupObjects();
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
	int where, wo;
	int pagenr = state->getUInt("PAGENR");
	QStringList tmpl;
	tmpl << state->get("MASTERPAGE");
	QString pageName = state->get("PAGENAME");
	bool oldPageMode = doc->masterPageMode();
	if (!pageName.isEmpty() && !oldPageMode) // We try do undo a master page deletion in standard mode
		doc->setMasterPageMode(true);
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
		if (doc->masterPageMode())
		{
			slotNewMasterPage(wo, pageName);
		}
		else
		{
			addNewPages(wo, where, 1, doc->pageHeight, doc->pageWidth, doc->PageOri, doc->m_pageSize, true, &tmpl);
		}
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
	if (!pageName.isEmpty() && !oldPageMode)
	{
		doc->setMasterPageMode(oldPageMode);
		doc->rebuildMasterNames();
		pagePalette->rebuildMasters();
	}
	if (doc->masterPageMode() && !pageName.isEmpty())
		ActWin->masterPagesPalette()->updateMasterPageList();
	pagePalette->rebuildPages();
}

void ScribusMainWindow::restoreAddPage(SimpleState *state, bool isUndo)
{
	int wo    = state->getInt("PAGE");
	int where = state->getInt("WHERE");
	int count = state->getInt("COUNT");
	QStringList based = state->get("BASED").split("|", QString::SkipEmptyParts);
	double height = state->getDouble("HEIGHT");
	double width = state->getDouble("WIDTH");
	int orient = state->getInt("ORIENT");
	QString siz = state->get("SIZE");
	bool mov = static_cast<bool>(state->getInt("MOVED"));

	int delFrom = 0;
	int delTo = 0;
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
		if (HaveDoc && doc->appMode == modeEditClip)
			view->requestMode(submodeEndNodeEdit);
		view->Deselect(true);
		DeletePage(delFrom, delTo);
	}
	else
	{
		if (doc->masterPageMode())
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
		int itemNr = doc->getItemNrfromUniqueID(state->getUInt(QString("item%1").arg(i)));
		if (doc->Items->at(itemNr)->uniqueNr == state->getUInt(QString("item%1").arg(i)))
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
			lowestItem = qMin(lowestItem, currItem->ItemNr);
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
		int itemNr = doc->getItemNrfromUniqueID(state->getUInt(QString("item%1").arg(i)));
		if (doc->Items->at(itemNr)->uniqueNr == state->getUInt(QString("item%1").arg(i)))
		{
			if (isUndo)
				doc->Items->at(itemNr)->isTableItem = static_cast<bool>(state->getInt(QString("tableitem%1").arg(i)));
			view->SelectItemNr(itemNr);
		}
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
			lowestItem = qMin(lowestItem, currItem->ItemNr);
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
		connect(dia, SIGNAL(selectElement(int, int, bool)), this, SLOT(selectItemsFromOutlines(int, int, bool)));
//		connect(dia, SIGNAL(refreshItem(PageItem*)), view, SLOT(RefreshItem(PageItem*)));
		dia->exec();
		delete dia;
	}
}

QString ScribusMainWindow::CFileDialog(QString wDir, QString caption, QString filter, QString defNa,
                                int optionFlags, bool *docom, bool *doFont, bool *doProfiles)
{
	QString retval = "";
	// changed from "this" to qApp->activeWindow() to be sure it will be opened
	// with the current active window as parent. E.g. it won't hide StoryEditor etc. -- PV
	CustomFDialog *dia = new CustomFDialog(qApp->activeWindow(), wDir, caption, filter, optionFlags);
	if (!defNa.isEmpty())
	{
		QFileInfo f(defNa);
		dia->setExtension(f.completeSuffix());
		dia->setZipExtension(f.completeSuffix() + ".gz");
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
			*docom = dia->SaveZip->isChecked();
			*doFont = dia->WithFonts->isChecked();
			*doProfiles = dia->WithProfiles->isChecked();
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
	if (HaveDoc)
	{
		doc->recalculateColors();
		propertiesPalette->updateColorList();
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
		currItem->update();
	}
}

void ScribusMainWindow::SetShortCut()
{
	for (QMap<QString,Keys>::Iterator it = prefsManager->appPrefs.KeyActions.begin(); it != prefsManager->appPrefs.KeyActions.end(); ++it )
	{
		if (!it.value().actionName.isEmpty())
			if (scrActions[it.value().actionName])
				scrActions[it.value().actionName]->setShortcut(it.value().keySequence);
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
	scrActions["toolsInsertLatexFrame"]->setEnabled(setter);
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
	QString hyphDirName = QDir::convertSeparators(ScPaths::instance().dictDir());
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
	QString pfad = ScPaths::instance().translationDir();
	QDir d2(pfad, "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d2.exists()) && (d2.count() != 0))
	{
		for (uint dc = 0; dc < d2.count(); dc++)
		{
			QFileInfo fi(pfad + d2[dc]);
			QString ext = fi.suffix().toLower();
			if (ext == "qm")
			{
    			QTranslator *trans = new QTranslator(0);
				trans->load(pfad + d2[dc]);

				QString translatedLang;
				for (QMap<QString, QStringList>::Iterator it=InstLang.begin(); it!=InstLang.end(); ++it)
				{
					translatedLang="";
					translatedLang = trans->translate("ScribusMainWindow", it.key().toLocal8Bit().data(), "");
					if (!translatedLang.isEmpty())
						it.value().append(translatedLang);
				}
				delete trans;
			}
		}
	}
	//For each hyphenation file, grab the strings and the hyphenation data.
	QString lang = QString(QLocale::system().name()).left(2);
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
		QStringList::Iterator itlrend=itl.value().end();
		for (QStringList::Iterator itlr = itl.value().begin(); itlr != itlrend; ++itlr)
		{
			if ((*itlr) == inLang)
				return itl.key();
		}
	}
	return inLang;
}

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
	return DocumentChecker().checkDocument(doc);
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
// 		connect(storyEditor, SIGNAL(EditSt()), this, SLOT(slotEditStyles()));
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
				std::cout << "Saving: " << doc->DocName.toStdString() << ".emergency" << std::endl;
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
	}
}

void ScribusMainWindow::SearchText()
{
	PageItem *currItem = doc->m_Selection->itemAt(0);
	view->requestMode(modeEdit);
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

void ScribusMainWindow::imageEditorExited(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
	if ( ExternalApp != 0 )
	{
		delete ExternalApp;
		ExternalApp = 0;
	}
}

/* call gimp and wait upon completion */
void ScribusMainWindow::callImageEditor()
{
	if (doc->m_Selection->count() != 0)
	{
		//NOTE to reviewers: I added my code to this function,
		// - as it performs a similar function,
		// - when the frame is a latex frame it makes only sense
		//   to run a latex editor
		// - IMHO ScribusMainWindow has way to many slots already
		// - my code here is short and without sideeffects
		PageItem *currItem = doc->m_Selection->itemAt(0);
		if (currItem->asLatexFrame()) {
			currItem->asLatexFrame()->runEditor();
			return; //Don't process the functions for imageframes!
		}

		QString imageEditorExecutable=prefsManager->imageEditorExecutable();
		if (ExternalApp != 0)
		{
			QString ieExe = QDir::convertSeparators(imageEditorExecutable);
			QMessageBox::information(this, tr("Information"), "<qt>" + tr("The program %1 is already running!").arg(ieExe) + "</qt>", 1, 0, 0);
			return;
		}
		if (currItem->PicAvail)
		{
			int index;
			QString imEditor;
			ExternalApp = new QProcess(NULL);
			QStringList cmd;
		#if defined(_WIN32)
			index = imageEditorExecutable.indexOf( ".exe" );
			if ( index >= 0 )
				imEditor = imageEditorExecutable.left( index + 4 );
			imEditor.replace( "\\", "/" );
			if ( imEditor.length() < imageEditorExecutable.length() )
			{
				int diffLength = imageEditorExecutable.length() - imEditor.length();
				QString cmdStr = imageEditorExecutable.right( diffLength );
				QStringList cmd1 = cmdStr.split( " ", QString::SkipEmptyParts);
				cmd += cmd1;
			}
		#else
			cmd = imageEditorExecutable.split(" ", QString::SkipEmptyParts);
			if ( cmd.count() > 0 )
				imEditor = cmd[0];
		#endif
			index = imEditor.lastIndexOf( "/" );
			if (index > -1 )
			{
				QString imEditorDir = imEditor.left( index + 1 );
				ExternalApp->setWorkingDirectory( imEditorDir );
			}
			cmd.append(QDir::convertSeparators(currItem->Pfile));
			ExternalApp->start(imEditor, cmd);
			if (!ExternalApp->waitForStarted())
			{
				delete ExternalApp;
				ExternalApp = 0;
				QMessageBox::critical(this, CommonStrings::trWarning, "<qt>" + tr("The program %1 is missing!").arg(imageEditorExecutable) + "</qt>", 1, 0, 0);
				return;
			}
			connect(ExternalApp, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(imageEditorExited(int, QProcess::ExitStatus)));
		}
	}
}

void ScribusMainWindow::slotCharSelect()
{
	charPalette->show();
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
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			releaseMouse();
			sendToSuper=false;
			QPixmap pm = QPixmap::grabWindow( QApplication::desktop()->winId(), m->globalPos().x(), m->globalPos().y(), 1, 1);
			QImage i = pm.toImage();
			QColor selectedColor=i.pixel(0, 0);
			bool found=false;
			ColorList::Iterator it;
			for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
			{
				if (selectedColor== ScColorEngine::getRGBColor(it.value(), doc))
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
				QString questionString="<qt>"+ tr("The selected color does not exist in the document's color set. Please enter a name for this new color.")+"</qt>";
				do
				{
					colorName = QInputDialog::getText(this, tr("Color Not Found"), questionString, QLineEdit::Normal, QString::null, &ok);
					if (ok)
					{
						if (doc->PageColors.contains(colorName))
							questionString="<qt>"+ tr("The name you have selected already exists. Please enter a different name for this new color.")+"</qt>";
						else
							nameFound=true;
					}
				} while (!nameFound && ok);
				if ( ok && !colorName.isEmpty() )
				{
					ScColor newColor(selectedColor.red(), selectedColor.green(), selectedColor.blue());
					doc->PageColors[colorName]=newColor;
					propertiesPalette->updateColorList();
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
						if ((m->modifiers() & Qt::ControlModifier) && (currItem->asTextFrame() || currItem->asPathText()))
							doc->itemSelection_SetFillColor(colorName); //Text colour
						else
						if (m->modifiers() & Qt::AltModifier) //Line colour
							doc->ItemPen(colorName);
						else
							doc->ItemBrush(colorName); //Fill colour
					}
				}
			}
			//propertiesPalette->Cpal->SetColors(ScMW->doc->PageColors);
			//propertiesPalette->updateCList();
			slotSelect();
		}
	}
	if (sendToSuper)
		QMainWindow::mouseReleaseEvent(m);

}

void ScribusMainWindow::insertSampleText()
{
	LoremManager *m = new LoremManager(doc, this);
	if (prefsManager->appPrefs.useStandardLI)
		m->insertLoremIpsum("la.xml", prefsManager->appPrefs.paragraphsLI);
	else
		m->exec();
	delete(m);
	m=NULL;
}

void ScribusMainWindow::languageChange()
{
	qDebug() << "void ScribusMainWindow::languageChange()";
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
			scrMenuMgr->setText("File", tr("&File"));
			scrMenuMgr->setText(recentFileMenuName, tr("Open &Recent"));
			scrMenuMgr->setText(recentPasteMenuName, tr("Paste Recent"));
			scrMenuMgr->setText("FileImport", tr("&Import"));
			scrMenuMgr->setText("FileExport", tr("&Export"));
			scrMenuMgr->setText("Edit", tr("&Edit"));
			scrMenuMgr->setText("EditContents", tr("Contents"));
			scrMenuMgr->setText("Style", tr("St&yle"));
			scrMenuMgr->setText("Color", tr("&Color"));
			scrMenuMgr->setText("FontSize", tr("&Size"));
			scrMenuMgr->setText("Shade", tr("&Shade"));
			scrMenuMgr->setText("Font", tr("&Font"));
			scrMenuMgr->setText("TypeEffects", tr("&Effects"));
			scrMenuMgr->setText("Item", tr("&Item"));
			scrMenuMgr->setText("ItemLevel", tr("&Level"));
			scrMenuMgr->setText("ItemLayer", tr("Send to Layer"));
			scrMenuMgr->setText("ItemPreviewSettings", tr("Previe&w Settings"));
			scrMenuMgr->setText("ItemPDFOptions", tr("&PDF Options"));
			//scrMenuMgr->setText("ItemShapes", tr("&Shape"));
			scrMenuMgr->setText("ItemConvertTo", tr("C&onvert To"));
//			scrMenuMgr->setText("ItemPathOps", tr("Path Tools"));
			scrMenuMgr->setText("Insert", tr("I&nsert"));
			scrMenuMgr->setText("InsertChar", tr("Character"));
			scrMenuMgr->setText("InsertQuote", tr("Quote"));
			scrMenuMgr->setText("InsertSpace", tr("Space"));
			scrMenuMgr->setText("InsertLigature", tr("Liga&ture"));
			scrMenuMgr->setText("Page", tr("&Page"));
			scrMenuMgr->setText("View", tr("&View"));
			scrMenuMgr->setText("Tools", tr("&Tools"));
			scrMenuMgr->setText("Extras", tr("E&xtras"));
			scrMenuMgr->setText("Windows", tr("&Windows"));
			scrMenuMgr->setText("Help", tr("&Help"));
			scrMenuMgr->setText("Alignment", tr("&Alignment"));
			scrMenuMgr->setText("HelpOnlineTutorials", tr("Online &Tutorials"));

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
	ActWin->setWindowTitle(QDir::convertSeparators(newCaption));
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
			if ( fileUrl.endsWith(".sla") || fileUrl.endsWith(".sla.gz") )
			{
				accepted = true;
				break;
			}
			else
			{
				QUrl url( fileUrls[i] );
				FileLoader *fileLoader = new FileLoader(url.path());
				int testResult = fileLoader->TestFile();
				delete fileLoader;
				if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
				{
					accepted = true;
					break;
				}
			}
		}
	}
	if (accepted)
		e->accept();
}

void ScribusMainWindow::dropEvent ( QDropEvent * e)
{
	bool accepted = false;
	if ( e->mimeData()->hasFormat("text/uri-list"))
	{
		QString fileUrl;
		QList<QUrl> fileUrls = e->mimeData()->urls();
		for( int i = 0; i < fileUrls.count(); ++i )
		{
			fileUrl = fileUrls[i].toLocalFile().toLower();
			if ( fileUrl.endsWith(".sla") || fileUrl.endsWith(".sla.gz") )
			{
				QUrl url( fileUrls[i] );
				QFileInfo fi(url.path());
				if ( fi.exists() )
				{
					accepted = true;
					loadDoc( fi.absoluteFilePath() );
				}
			}
			else
			{
				QUrl url( fileUrls[i] );
				FileLoader *fileLoader = new FileLoader(url.path());
				int testResult = fileLoader->TestFile();
				delete fileLoader;
				if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
				{
					QFileInfo fi(url.path());
					if ( fi.exists() )
					{
						accepted = true;
						loadDoc( fi.absoluteFilePath() );
					}
				}
			}
		}
	}
	if (accepted)
		e->accept();
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
				contentsBuffer.sourceType = PageItem::ImageFrame;
				contentsBuffer.contentsFileName = imageItem->Pfile;
				contentsBuffer.LocalScX = imageItem->imageXScale();
				contentsBuffer.LocalScY = imageItem->imageYScale();
				contentsBuffer.LocalX   = imageItem->imageXOffset();
				contentsBuffer.LocalY   = imageItem->imageYOffset();
				contentsBuffer.ItemX   = imageItem->xPos();
				contentsBuffer.ItemY   = imageItem->yPos();
				contentsBuffer.effects = imageItem->effectsInUse;
				contentsBuffer.inputProfile = imageItem->IProfile;
				contentsBuffer.useEmbedded  = imageItem->UseEmbedded;
				contentsBuffer.renderingIntent = imageItem->IRender;
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
				t = QMessageBox::warning(this, CommonStrings::trWarning,
										tr("Do you really want to replace your existing image?"),
										QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			if (t == QMessageBox::Yes)
			{
				imageItem->EmProfile = "";
				imageItem->pixm.imgInfo.isRequest = false;
				imageItem->IProfile = doc->CMSSettings.DefaultImageRGBProfile;
				imageItem->IRender  = doc->CMSSettings.DefaultIntentImages;
				imageItem->effectsInUse = contentsBuffer.effects;
				qApp->changeOverrideCursor( QCursor(Qt::WaitCursor) );
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
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
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				view->DrawNew();
				propertiesPalette->updateColorList();
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
	undoManager->setUndoEnabled(false);
	QString patternName = "Pattern_"+doc->m_Selection->itemAt(0)->itemName();
	patternName = patternName.trimmed().simplified().replace(" ", "_");
	ScriXmlDoc *ss = new ScriXmlDoc();
	QString objectString = ss->WriteElem(doc, view, doc->m_Selection);
	int ac = doc->Items->count();
	uint oldNum = doc->TotalItems;
	bool savedAlignGrid = doc->useRaster;
	bool savedAlignGuides = doc->SnapGuides;
	doc->useRaster = false;
	doc->SnapGuides = false;
	slotElemRead(objectString, doc->currentPage()->xOffset(), doc->currentPage()->yOffset(), false, true, doc, view);
	doc->useRaster = savedAlignGrid;
	doc->SnapGuides = savedAlignGuides;
	int ae = doc->Items->count();
	ScPattern pat = ScPattern();
	pat.setDoc(doc);
	PageItem* currItem = doc->Items->at(ac);
	pat.pattern = currItem->DrawObj_toImage();
	pat.width = currItem->gWidth;
	pat.height = currItem->gHeight;
	for (int as = ac; as < ae; ++as)
	{
		pat.items.append(doc->Items->takeAt(ac));
	}
	doc->addPattern(patternName, pat);
	propertiesPalette->updateColorList();
	delete ss;
	if (outlinePalette->isVisible())
		outlinePalette->BuildTree();
	doc->TotalItems = oldNum;
	undoManager->setUndoEnabled(true);
}

void ScribusMainWindow::managePatterns()
{
	if (HaveDoc)
	{
		undoManager->setUndoEnabled(false);
		QMap<QString, ScPattern> docPatterns(doc->docPatterns);
		PatternDialog *dia = new PatternDialog(this, &docPatterns, doc, this);
		if (dia->exec())
		{
			doc->setPatterns(dia->dialogPatterns);
			propertiesPalette->updateColorList();
			view->DrawNew();
			undoManager->setUndoEnabled(true);
		}
		delete dia;
		undoManager->setUndoEnabled(true);
	}
}

void ScribusMainWindow::enableTextActions(QMap<QString, QPointer<ScrAction> > *actionMap, bool enabled, const QString& fontName)
{
	actionManager->enableUnicodeActions(actionMap, enabled, fontName);
	scrMenuMgr->setMenuEnabled("InsertChar", enabled);
	scrMenuMgr->setMenuEnabled("InsertQuote", enabled);
	scrMenuMgr->setMenuEnabled("InsertSpace", enabled);
	scrMenuMgr->setMenuEnabled("InsertLigature", enabled);
}
