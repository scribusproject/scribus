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
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <dlfcn.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <string>
#include "scribus.h"
#include "scribus.moc"
#include "newfile.h"
#include "page.h"
#include "query.h"
#include "mdup.h"
#include "docinfo.h"
#include "reformdoc.h"
#include "serializer.h"
#include "align.h"
#include "fmitem.h"
#include "fontprefs.h"
#include "libprefs/prefs.h"
#include "prefstable.h"
#include "pdfopts.h"
#include "inspage.h"
#include "delpages.h"
#include "movepage.h"
#include "helpbrowser.h"
#include "scribusXml.h"
#include "libabout/about.h"
#include "libpostscript/pslib.h"
#include "druck.h"
#include "editformats.h"
#include "muster.h"
#include "applytemplate.h"
#include "picstatus.h"
#include "customfdialog.h"
#include "cmsprefs.h"
#include "annot.h"
#include "annota.h"
#include "javadocs.h"
#include "colorm.h"
#include "mpalette.h"
#include "measurements.h"
#include "gtgettext.h"
#include "fileloader.h"
#include "arrowchooser.h"
#include "tabtypography.h"
#include "tabguides.h"
#include "undogui.h"
#include "filewatcher.h"
#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

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

extern QPixmap loadIcon(QString nam);
extern bool overwrite(QWidget *parent, QString filename);
extern void CopyPageItem(struct CopyPasteBuffer *Buffer, PageItem *b);
extern void ReOrderText(ScribusDoc *doc, ScribusView *view);
extern int copyFile(QString source, QString target);
extern int moveFile(QString source, QString target);

using namespace std;

#ifdef HAVE_CMS
cmsHPROFILE CMSoutputProf;
cmsHPROFILE CMSprinterProf;
cmsHTRANSFORM stdTrans;
cmsHTRANSFORM stdProof;
cmsHTRANSFORM stdTransImg;
cmsHTRANSFORM stdProofImg;
bool SoftProofing;
bool Gamut;
bool CMSuse;
int IntentMonitor;
int IntentPrinter;
#endif
bool CMSavail;
ProfilesL InputProfiles;
double UmReFaktor;
QString DocDir;
ScribusApp* ScApp;
PrefsFile* prefsFile;


ScribusApp::ScribusApp()
{} // ScribusApp::ScribusApp()

/*
 * retval 0 - ok, 1 - no fonts, ...
 */
int ScribusApp::initScribus(bool showSplash, const QString newGuiLanguage)
{
	int retVal=0;
	ExternalApp = 0;
	guiLanguage = newGuiLanguage;
	initSplash(showSplash);

	ScApp = this;
	CurrStED = NULL;
	setCaption( tr("Scribus " VERSION));
	setKeyCompression(false);
	setIcon(loadIcon("AppIcon.png"));
	MenuItemsFile.clear();

	initMenuBar();
	initStatusBar();
	initToolBars();

	qApp->processEvents();

	BuFromApp = false;

	PrefsPfad = getPreferencesLocation();
	prefsFile = new PrefsFile(QDir::convertSeparators(PrefsPfad + "/prefs13.xml"));
	convertToXMLPreferences(PrefsPfad);
	
	initFonts();

	if (NoFonts)
		retVal=1;
	else
	{
		initDefaultValues();
		buildFontMenu();
		initDefaultPrefs();
		initArrowStyles();
		initKeyboardShortcuts();

		resize(610, 600);
		QVBox* vb = new QVBox( this );
		vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
		wsp = new QWorkspace( vb );
		setCentralWidget( vb );
		connect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));

		initPalettes();

		fileWatcher = new FileWatcher(this);
		if (splashScreen != NULL)
			splashScreen->setStatus( tr("Reading Preferences"));
		qApp->processEvents();
		ReadPrefs();

		HaveGS = system(Prefs.gs_exe+" -h > /dev/null 2>&1");
		HavePngAlpha = system(Prefs.gs_exe+" -sDEVICE=pngalpha -c quit > /dev/null 2>&1");
		DocDir = Prefs.DocDir;

		if (splashScreen != NULL)
			splashScreen->setStatus( tr("Getting ICC Profiles"));
		CMSavail = false;
		GetCMSProfiles();
		initCMS();

		if (splashScreen != NULL)
			splashScreen->setStatus( tr("Init Hyphenator"));
		qApp->processEvents();
		initHyphenator();

		if (splashScreen != NULL)
			splashScreen->setStatus( tr("Setting up Shortcuts"));
		qApp->processEvents();

		SetShortCut();

		if (splashScreen != NULL)
			splashScreen->setStatus( tr("Reading Scrapbook"));
		initScrapbook();

		if (splashScreen != NULL)
			splashScreen->setStatus( tr("Initializing Plugins"));
		qApp->processEvents();
		initPlugs();
		connect(fileWatcher, SIGNAL(fileDeleted(QString )), this, SLOT(RemoveRecent(QString)));
		connect(this, SIGNAL(TextIFont(QString)), this, SLOT(AdjustFontMenu(QString)));
		connect(this, SIGNAL(TextISize(int)), this, SLOT(setFSizeMenu(int)));
		connect(this, SIGNAL(TextISize(int)), Mpal, SLOT(setSize(int)));
		connect(this, SIGNAL(TextUSval(double)), Mpal, SLOT(setExtra(double)));
		connect(this, SIGNAL(TextStil(int)), Mpal, SLOT(setStil(int)));
		connect(this, SIGNAL(TextScale(int)), Mpal, SLOT(setTScale(int)));
		connect(this, SIGNAL(TextFarben(QString, QString, int, int)), Mpal, SLOT(setActFarben(QString, QString, int, int)));

		initCrashHandler();
	}
	closeSplash();
	return retVal;
}

void ScribusApp::initSplash(bool showSplash)
{
	if (showSplash)
	{
		splashScreen = new SplashScreen();
		splashScreen->setStatus(QObject::tr("Initializing..."));
	}
	else
		splashScreen = NULL;
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
	WerkTools2 = new QToolBar( tr("File"), this);
	DatNeu = new QToolButton(loadIcon("DateiNeu.xpm"), tr("Create a new Document"), QString::null, this, SLOT(slotFileNew()), WerkTools2);
	DatOpe = new QToolButton(loadIcon("DateiOpen.xpm"), tr("Open a Document"), QString::null, this, SLOT(slotDocOpen()), WerkTools2);
	DatSav = new QToolButton(loadIcon("DateiSave2.png"), tr("Save the current Document"), QString::null, this, SLOT(slotFileSave()), WerkTools2);
	DatClo = new QToolButton(loadIcon("DateiClose.png"), tr("Close the current Document"), QString::null, this, SLOT(slotFileClose()), WerkTools2);
	DatPri = new QToolButton(loadIcon("DateiPrint.xpm"), tr("Print the current Document"), QString::null, this, SLOT(slotFilePrint()), WerkTools2);
	DatPDF = new QToolButton(loadIcon("acrobat.png"), tr("Save the current Document as PDF"), QString::null, this, SLOT(SaveAsPDF()), WerkTools2);
	DatSav->setEnabled(false);
	DatClo->setEnabled(false);
	DatPri->setEnabled(false);
	DatPDF->setEnabled(false);
	DatOpe->setPopup(recentMenu);

	editToolBar = new QToolBar(tr("Edit"), this);
	UndoWidget* uWidget = new UndoWidget(editToolBar, "uWidget");
	UndoManager::instance()->registerGui(uWidget);
	
	WerkTools = new WerkToolB(this);
	setDockEnabled(WerkTools, DockLeft, false);
	setDockEnabled(WerkTools, DockRight, false);
	WerkTools->Sichtbar = true;
	WerkTools->setEnabled(false);
	WerkToolsP = new WerkToolBP(this);
	setDockEnabled(WerkToolsP, DockLeft, false);
	setDockEnabled(WerkToolsP, DockRight, false);
	WerkToolsP->setEnabled(false);
	WerkToolsP->Sichtbar = true;

	connect(WerkTools, SIGNAL(NewMode(int)), this, SLOT(ModeFromTB(int)));
	connect(WerkTools, SIGNAL(Schliessen()), this, SLOT(ToggleTools()));
	connect(WerkToolsP, SIGNAL(NewMode(int)), this, SLOT(ModeFromTB(int)));
	connect(WerkToolsP, SIGNAL(Schliessen()), this, SLOT(TogglePDFTools()));
}

void ScribusApp::initFonts()
{
	if (splashScreen!=NULL) {
		splashScreen->setStatus( tr("Searching for Fonts"));
		qApp->processEvents();
	}
	NoFonts=GetAllFonts();
	if (NoFonts)
	{
		if (splashScreen!=NULL)
			splashScreen->close(); // 10/10/2004 pv fix #1200
		QString mess = tr("There are no Postscript fonts on your system");
		mess += "\n" + tr("Exiting now");
		QMessageBox::critical(this, tr("Fatal Error"), mess, 1, 0, 0);
	}
	else
	if (splashScreen!=NULL) 
	{
		splashScreen->setStatus( tr("Font System Initialized"));
		qApp->processEvents();
	}
}

void ScribusApp::initDefaultPrefs()
{
	/** Default font and size **/
	SCFontsIterator it(Prefs.AvailFonts);
	Prefs.DefFont = it.currentKey();
	Prefs.DefSize = 120;
	Prefs.AppFontSize = qApp->font().pointSize();

	/** Default colours **/
	Prefs.DColors.clear();
	QString pfadC = LIBDIR;
	QString pfadC2 = pfadC + "rgbscribus.txt";
	QFile fiC(pfadC2);
	if (!fiC.exists())
	{
		Prefs.DColors.insert("White", CMYKColor(0, 0, 0, 0));
		Prefs.DColors.insert("Black", CMYKColor(0, 0, 0, 255));
		Prefs.DColors.insert("Blue", CMYKColor(255, 255, 0, 0));
		Prefs.DColors.insert("Cyan", CMYKColor(255, 0, 0, 0));
		Prefs.DColors.insert("Green", CMYKColor(255, 0, 255, 0));
		Prefs.DColors.insert("Red", CMYKColor(0, 255, 255, 0));
		Prefs.DColors.insert("Yellow", CMYKColor(0, 0, 255, 0));
		Prefs.DColors.insert("Magenta", CMYKColor(0, 255, 0, 0));
		Prefs.DColorSet = "Scribus-Small";
	}
	else
	{
		if (fiC.open(IO_ReadOnly))
		{
			QString ColorEn, Cname;
			int Rval, Gval, Bval;
			QTextStream tsC(&fiC);
			ColorEn = tsC.readLine();
			while (!tsC.atEnd())
			{
				ColorEn = tsC.readLine();
				QTextStream CoE(&ColorEn, IO_ReadOnly);
				CoE >> Rval;
				CoE >> Gval;
				CoE >> Bval;
				CoE >> Cname;
				CMYKColor tmp;
				tmp.setColorRGB(Rval, Gval, Bval);
				Prefs.DColors.insert(Cname, tmp);
			}
			fiC.close();
		}
		Prefs.DColorSet = "X11 RGB-Set";
	}

	Prefs.Wheelval = 40;
	Prefs.guidesSettings.marginsShown = true;
	Prefs.guidesSettings.framesShown = true;
	Prefs.guidesSettings.gridShown = false;
	Prefs.guidesSettings.guidesShown = false;
	Prefs.guidesSettings.baseShown = false;
	Prefs.guidesSettings.showPic = true;
	Prefs.guidesSettings.linkShown = false;
	Prefs.guidesSettings.grabRad = 4;
	Prefs.guidesSettings.guideRad = 10;
	Prefs.guidesSettings.minorGrid = 20;
	Prefs.guidesSettings.majorGrid = 100;
	Prefs.guidesSettings.minorColor = QColor(green);
	Prefs.guidesSettings.majorColor = QColor(green);
	Prefs.guidesSettings.margColor = QColor(blue);
	Prefs.guidesSettings.guideColor = QColor(darkBlue);
	Prefs.guidesSettings.baseColor = QColor(lightGray);
	Prefs.typographicSetttings.valueSuperScript = 33;
	Prefs.typographicSetttings.scalingSuperScript = 100;
	Prefs.typographicSetttings.valueSubScript = 33;
	Prefs.typographicSetttings.scalingSubScript = 100;
	Prefs.typographicSetttings.valueSmallCaps = 75;
	Prefs.typographicSetttings.autoLineSpacing = 20;
	Prefs.typographicSetttings.valueBaseGrid = 14.4;
	Prefs.typographicSetttings.offsetBaseGrid = 0.0;
	Prefs.GUI = "Default";
	Prefs.Dpen = "Black";
	Prefs.Dbrush = "Black";
	Prefs.Dshade = 100;
	Prefs.Dshade2 = 100;
	Prefs.DLineArt = SolidLine;
	Prefs.Dwidth = 1;
	Prefs.DpenLine = "Black";
	Prefs.DpenText = "Black";
	Prefs.DstrokeText = "Black";
	Prefs.DpapColor = QColor(white);
	Prefs.DCols = 1;
	Prefs.DGap = 0.0;
	Prefs.DshadeLine = 100;
	Prefs.DLstyleLine = SolidLine;
	Prefs.DwidthLine = 1;
	Prefs.DstartArrow = 0;
	Prefs.DendArrow = 0;
	Prefs.MagMin = 10;
	Prefs.MagMax = 3200;
	Prefs.MagStep = 25;
	Prefs.DbrushPict = "White";
	Prefs.ShadePict = 100;
	Prefs.ScaleX = 1;
	Prefs.ScaleY = 1;
	Prefs.Before = true;
	Prefs.docUnitIndex = 0;
	Prefs.PolyC = 4;
	Prefs.PolyF = 0.5;
	Prefs.PolyS = false;
	Prefs.PolyFd = 0;
	Prefs.PolyR = 0;
	Prefs.mainToolBarSettings.visible = true;
	Prefs.pdfToolBarSettings.visible = true;
	Prefs.mPaletteSettings.visible = false;
	Prefs.measurePalSettings.visible = false;
	Prefs.treePalSettings.visible = false;
	Prefs.scrapPalSettings.visible = false;
	Prefs.layerPalSettings.visible = false;
	Prefs.bookmPalSettings.visible = false;
	Prefs.pagePalSettings.visible = false;
	Prefs.measurePalSettings.xPosition = 0;
	Prefs.measurePalSettings.yPosition = 0;
	Prefs.treePalSettings.xPosition = 0;
	Prefs.treePalSettings.yPosition = 0;
	Prefs.scrapPalSettings.xPosition = 0;
	Prefs.scrapPalSettings.yPosition = 0;
	Prefs.scrapPalSettings.width = 100;
	Prefs.scrapPalSettings.height = 200;
	Prefs.pagePalSettings.xPosition = 0;
	Prefs.pagePalSettings.yPosition = 0;
	Prefs.bookmPalSettings.xPosition = 0;
	Prefs.bookmPalSettings.yPosition = 0;
	Prefs.layerPalSettings.xPosition = 0;
	Prefs.layerPalSettings.yPosition = 0;
	Prefs.PSize = 40;
	Prefs.SaveAtQ = true;
	Prefs.ClipMargin = true;
	Prefs.GCRMode = true;
	Prefs.RecentDocs.clear();
	Prefs.RecentDCount = 5;
	Prefs.marginColored = false;
	Prefs.PageFormat = 4;
	Prefs.Ausrichtung = 0;
	Prefs.PageBreite = 595;
	Prefs.PageHoehe = 842;
	Prefs.RandOben = 9;
	Prefs.RandUnten = 40;
	Prefs.RandLinks = 9;
	Prefs.RandRechts = 9;
	Prefs.DoppelSeiten = false;
	Prefs.ErsteLinks = false;
	Prefs.ScaleType = true;
	Prefs.AspectRatio = true;
	Prefs.MinWordLen = 3;
	Prefs.HyCount = 2;
	Prefs.Language = "";
	Prefs.Automatic = true;
	Prefs.AutoCheck = false;
	Prefs.AutoSave = false;
	Prefs.AutoSaveTime = 600000;
	Prefs.DisScale = 1.0;
	Prefs.DocDir = QDir::homeDirPath();
	Prefs.ProfileDir = "";
	Prefs.ScriptDir = "";
	Prefs.TemplateDir = "";
	Prefs.CustomColorSets.clear();
	Prefs.PrPr_Mode = false;
	Prefs.Gcr_Mode = true;
	Prefs.PrPr_AlphaText = false;
	Prefs.PrPr_AlphaGraphics = false;
	Prefs.PrPr_Transparency = false;
	Prefs.PrPr_C = true;
	Prefs.PrPr_M = true;
	Prefs.PrPr_Y = true;
	Prefs.PrPr_K = true;
	Prefs.gimp_exe = "gimp";
	Prefs.gs_antiGraph = true;
	Prefs.gs_antiText = true;
	Prefs.gs_exe = "gs";
	Prefs.STEcolor = QColor(white);
	Prefs.STEfont = font().toString();
	Prefs.DCMSset.DefaultMonitorProfile = "";
	Prefs.DCMSset.DefaultPrinterProfile = "";
	Prefs.DCMSset.DefaultInputProfile = "";
	Prefs.DCMSset.DefaultInputProfile2 = "";
	Prefs.DCMSset.CMSinUse = false;
	Prefs.DCMSset.SoftProofOn = false;
	Prefs.DCMSset.GamutCheck = false;
	Prefs.DCMSset.BlackPoint = true;
	Prefs.DCMSset.DefaultIntentMonitor = 1;
	Prefs.DCMSset.DefaultIntentMonitor2 = 1;
	Prefs.DCMSset.DefaultIntentPrinter = 0;
	Prefs.GFontSub.clear();
	Prefs.ScratchLeft = 100;
	Prefs.ScratchRight = 100;
	Prefs.ScratchTop = 20;
	Prefs.ScratchBottom = 20;
}


void ScribusApp::initDefaultValues()
{
	dirs = prefsFile->getContext("dirs");
	HaveDoc = 0;
	singleClose = false;
	ScriptRunning = false;
	DLLReturn = "";
	DLLinput = "";
	view = NULL;
	doc = NULL;
	Buffer2 = "";
	UniCinp = false;
	UniCinC = 0;
	UniCinS = "";
	DispX = 10;
	DispY = 10;
	DocNr = 1;
	UmReFaktor = 1.0;
	PrinterUsed = false;
	PDef.Pname = "";
	PDef.Dname = "";
	PDef.Command = "";
	keyrep = false;
	ClipB = QApplication::clipboard();
	PalettesStat[0] = false;
	GuidesStat[0] = false;

	connect(ClipB, SIGNAL(dataChanged()), this, SLOT(ClipChange()));
}

void ScribusApp::initKeyboardShortcuts()
{
	SetKeyEntry(56, tr("Smart Hyphen"), 0, CTRL+Key_Minus);
	SetKeyEntry(57, tr("Align Left"), 0, CTRL+Key_L);
	SetKeyEntry(58, tr("Align Right"), 0, CTRL+Key_R);
	SetKeyEntry(59, tr("Align Center"), 0, CTRL+Key_E);
	SetKeyEntry(60, tr("Insert Page Number"), 0, CTRL+Key_NumberSign);
	SetKeyEntry(68, tr("Non Breaking Space"), 0, CTRL+Key_Space);
}

void ScribusApp::initArrowStyles()
{
	struct ArrowDesc arrow;
	FPointArray points;
	QWMatrix arrowScaling;
	arrowScaling.scale(0.5, 0.5);
	arrow.name = "Arrow1L";
	arrow.userArrow = false;
	points.addQuadPoint(0, 0, 0, 0, 0, 0, 0, 0);
	points.addQuadPoint(-5, -5, -5, -5, -5, -5, -5, -5);
	points.addQuadPoint(12, 0, 12, 0, 12, 0, 12, 0);
	points.addQuadPoint(-5, 5, -5, 5, -5, 5, -5, 5);
	points.addQuadPoint(0, 0, 0, 0, 0, 0, 0, 0);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "Arrow1M";
	points.map(arrowScaling);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "Arrow1S";
	points.map(arrowScaling);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "SquareL";
	points.resize(0);
	points.addQuadPoint(-5, -5, -5, -5, -5, -5, -5, -5);
	points.addQuadPoint(5, -5, 5, -5, 5, -5, 5, -5);
	points.addQuadPoint(5, 5, 5, 5, 5, 5, 5, 5);
	points.addQuadPoint(-5, 5, -5, 5, -5, 5, -5, 5);
	points.addQuadPoint(-5, -5, -5, -5, -5, -5, -5, -5);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "SquareM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "SquareS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "TriangleInL";
	points.resize(0);
	points.addQuadPoint(5.77, 0, 5.77, 0, 5.77, 0, 5.77, 0);
	points.addQuadPoint(-2.88, 5, -2.88, 5, -2.88, 5, -2.88, 5);
	points.addQuadPoint(-2.88, -5, -2.88, -5, -2.88, -5, -2.88, -5);
	points.addQuadPoint(5.77, 0, 5.77, 0, 5.77, 0, 5.77, 0);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "TriangleInM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "TriangleInS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "TriangleOutL";
	points.resize(0);
	points.addQuadPoint(-5.77, 0, -5.77, 0, -5.77, 0, -5.77, 0);
	points.addQuadPoint(2.88, 5, 2.88, 5, 2.88, 5, 2.88, 5);
	points.addQuadPoint(2.88, -5, 2.88, -5, 2.88, -5, 2.88, -5);
	points.addQuadPoint(-5.77, 0, -5.77, 0, -5.77, 0, -5.77, 0);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "TriangleOutM";
	points.map(arrowScaling);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
	arrow.name = "TriangleOutS";
	points.map(arrowScaling);
	arrow.points = points.copy();
	Prefs.arrowStyles.append(arrow);
}

void ScribusApp::initPalettes()
{
	Tpal = new Tree(this, 0);
	Mpal = new Mpalette(this, &Prefs);
	Mpal->Cpal->SetColors(Prefs.DColors);
	Mpal->Cpal->UseTrans(true);
	Mpal->Fonts->RebuildList(&Prefs, 0);
	Npal = new NodePalette(this);
	Lpal = new LayerPalette(this);
	ScBook = new Biblio(this, &Prefs);
	Sepal = new SeitenPal(this);
	BookPal = new BookPalette(this);
	MaPal = new Measurements(this);
	MaPal->hide();

	undoPalette = new UndoPalette(this, "undoPalette");
	UndoManager::instance()->registerGui(undoPalette);

	connect(MaPal, SIGNAL(Schliessen(bool)), this, SLOT(setMapal(bool)));
	connect(Mpal, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(Mpal, SIGNAL(NewAbStyle(int)), this, SLOT(setNewAbStyle(int)));
	connect(Mpal, SIGNAL(BackHome()), this, SLOT(Aktiv()));
	connect(Mpal, SIGNAL(Stellung(int)), this, SLOT(setItemHoch(int)));
	connect(Mpal, SIGNAL(Schliessen()), this, SLOT(ToggleMpal()));
	connect(Mpal, SIGNAL(EditCL()), this, SLOT(ToggleFrameEdit()));
	connect(Mpal, SIGNAL(NewTF(QString)), this, SLOT(SetNewFont(QString)));
	connect(Mpal, SIGNAL(UpdtGui(int)), this, SLOT(HaveNewSel(int)));
	connect(Mpal->Cpal, SIGNAL(NewPen(QString)), this, SLOT(setPenFarbe(QString)));
	connect(Mpal->Cpal, SIGNAL(NewBrush(QString)), this, SLOT(setBrushFarbe(QString)));
	connect(Mpal->Cpal, SIGNAL(NewPenShade(int)), this, SLOT(setPenShade(int)));
	connect(Mpal->Cpal, SIGNAL(NewBrushShade(int)), this, SLOT(setBrushShade(int)));
	connect(Mpal->Cpal, SIGNAL(NewTrans(double)), this, SLOT(SetTranspar(double)));
	connect(Mpal->Cpal, SIGNAL(NewTransS(double)), this, SLOT(SetTransparS(double)));
	connect(Mpal->Cpal, SIGNAL(NewGradient(int)), this, SLOT(setGradFill(int)));
	connect(Mpal->Cpal->gradEdit->Preview, SIGNAL(gradientChanged()), this, SLOT(updtGradFill()));
	connect(Mpal->Cpal, SIGNAL(gradientChanged()), this, SLOT(updtGradFill()));
	connect(Mpal->Cpal, SIGNAL(QueryItem()), this, SLOT(GetBrushPen()));
	connect(Tpal, SIGNAL(Schliessen()), this, SLOT(ToggleTpal()));
	connect(Tpal, SIGNAL(CloseMpal()), this, SLOT(ToggleMpal()));
	connect(Tpal, SIGNAL(CloseSpal()), this, SLOT(ToggleBpal()));
	connect(Tpal, SIGNAL(SelectElement(int, int)), this, SLOT(SelectFromOutl(int, int)));
	connect(Tpal, SIGNAL(SelectSeite(int)), this, SLOT(SelectFromOutlS(int)));
	connect(Tpal, SIGNAL(ToggleAllPalettes()), this, SLOT(ToggleAllPalettes()));
	connect(Mpal->Spal, SIGNAL(newStyle(int)), this, SLOT(setNewAbStyle(int)));
	connect(Mpal, SIGNAL(EditLSt()), this, SLOT(slotEditLineStyles()));
	connect(Mpal, SIGNAL(ToggleAllPalettes()), this, SLOT(ToggleAllPalettes()));
	connect(Mpal, SIGNAL(CloseTpal()), this, SLOT(ToggleTpal()));
	connect(Mpal, SIGNAL(CloseBpal()), this, SLOT(ToggleBpal()));
	connect(Npal, SIGNAL(Schliessen()), this, SLOT(NoFrameEdit()));
	connect(Lpal, SIGNAL(LayerActivated(int)), this, SLOT(changeLayer(int)));
	connect(Lpal, SIGNAL(LayerRemoved(int, bool)), this, SLOT(LayerRemove(int, bool)));
	connect(Lpal, SIGNAL(LayerChanged()), this, SLOT(showLayer()));
	connect(Lpal, SIGNAL(Schliessen()), this, SLOT(ToggleLpal()));
	connect(Lpal->Table, SIGNAL(Schliessen()), this, SLOT(ToggleLpal()));
	connect(Lpal->Table, SIGNAL(ToggleAllPalettes()), this, SLOT(ToggleAllPalettes()));
	connect(Sepal, SIGNAL(Schliessen()), this, SLOT(ToggleSepal()));
	connect(Sepal, SIGNAL(EditTemp(QString)), this, SLOT(ManageTemp(QString)));
	connect(Sepal->PageView, SIGNAL(UseTemp(QString, int)), this, SLOT(Apply_Temp(QString, int)));
	connect(Sepal->PageView, SIGNAL(NewPage(int, QString)), this, SLOT(slotNewPageP(int, QString)));
	connect(Sepal->Trash, SIGNAL(DelPage(int)), this, SLOT(DeletePage2(int)));
	connect(Sepal, SIGNAL(GotoSeite(int)), this, SLOT(SelectFromOutlS(int)));
	connect(Sepal, SIGNAL(ToggleAllPalettes()), this, SLOT(ToggleAllPalettes()));
	connect(BookPal->BView, SIGNAL(MarkMoved()), this, SLOT(StoreBookmarks()));
	connect(BookPal->BView, SIGNAL(ChangeBMNr(int, int, int)), this, SLOT(ChBookmarks(int, int, int)));
	connect(BookPal->BView, SIGNAL(SelectElement(int, int)), this, SLOT(SelectFromOutl(int, int)));
	connect(BookPal, SIGNAL(Schliessen()), this, SLOT(ToggleBookpal()));
	connect(BookPal, SIGNAL(ToggleAllPalettes()), this, SLOT(ToggleAllPalettes()));
}

void ScribusApp::initScrapbook()
{
	QString scrapbookFile = PrefsPfad+"/scrap13.scs";
	QFileInfo scrapbookFileInfo = QFileInfo(scrapbookFile);
	if (scrapbookFileInfo.exists())
		ScBook->BibWin->ReadContents(scrapbookFile);
	ScBook->ScFilename = scrapbookFile;
	ScBook->AdjustMenu();
	connect(ScBook, SIGNAL(Schliessen()), this, SLOT(ToggleBpal()));
	connect(ScBook->BibWin, SIGNAL(ToggleAllPalettes()), this, SLOT(ToggleAllPalettes()));
	connect(ScBook->BibWin, SIGNAL(Schliessen()), this, SLOT(ToggleBpal()));
	connect(ScBook->BibWin, SIGNAL(CloseTpal()), this, SLOT(ToggleTpal()));
	connect(ScBook->BibWin, SIGNAL(CloseMpal()), this, SLOT(ToggleMpal()));
}

void ScribusApp::initCrashHandler()
{
	typedef void (*HandlerType)(int);
	HandlerType handler	= 0;
	handler = ScribusApp::defaultCrashHandler;
	if (!handler)
		handler = SIG_DFL;
	sigset_t mask;
	sigemptyset(&mask);
#ifdef SIGSEGV
	signal (SIGSEGV, handler);
	sigaddset(&mask, SIGSEGV);
#endif
#ifdef SIGFPE
	signal (SIGFPE, handler);
	sigaddset(&mask, SIGFPE);
#endif
#ifdef SIGILL
	signal (SIGILL, handler);
	sigaddset(&mask, SIGILL);
#endif
#ifdef SIGABRT
	signal (SIGABRT, handler);
	sigaddset(&mask, SIGABRT);
#endif
	sigprocmask(SIG_UNBLOCK, &mask, 0);
}

const QString ScribusApp::getGuiLanguage()
{
	return guiLanguage;
}

/*!
 \fn QString ScribusApp::getPreferencesLocation()
 \author Craig Bradney
 \date Thu 18 Nov 2004
 \brief Get the user's preference file location. Rename any existing old preferences files
 \param None
 \retval QString Location of the user's preferences
 */

QString ScribusApp::getPreferencesLocation()
{
	QString Pff = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus");
	QFileInfo Pffi = QFileInfo(Pff);
	QString PrefsPfad;
	//If we are using ~/.scribus
	if (Pffi.exists())
	{
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = QDir::homeDirPath();
	}
	else // Move to using ~/.scribus/scribus.* from ~/.scribus.*
	{
		QDir prefsDirectory = QDir();
		prefsDirectory.mkdir(Pff);
		PrefsPfad = Pff;
		QString oldPR = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus.rc");
		QFileInfo oldPi = QFileInfo(oldPR);
		if (oldPi.exists())
			moveFile(oldPR, Pff+"/scribus.rc");
		QString oldPR2 = QDir::convertSeparators(QDir::homeDirPath()+"/.scribusfont.rc");
		QFileInfo oldPi2 = QFileInfo(oldPR2);
		if (oldPi2.exists())
			moveFile(oldPR2, Pff+"/scribusfont.rc");
		QString oldPR3 = QDir::convertSeparators(QDir::homeDirPath()+"/.scribusscrap.scs");
		QFileInfo oldPi3 = QFileInfo(oldPR3);
		if (oldPi3.exists())
			moveFile(oldPR3, Pff+"/scrap.scs");
	}

	//Now make copies for 1.3 use and leave the old ones alone for <1.3.0 usage
	QString oldPR =QDir::convertSeparators(PrefsPfad+"/scribus.rc");
	QString oldPR2=QDir::convertSeparators(PrefsPfad+"/scribusfont.rc");
	QString oldPR3=QDir::convertSeparators(PrefsPfad+"/scrap.scs");
	QString oldPR4=QDir::convertSeparators(PrefsPfad+"/prefs.xml");	
	QString newPR =QDir::convertSeparators(PrefsPfad+"/scribus13.rc");
	QString newPR2=QDir::convertSeparators(PrefsPfad+"/scribusfont13.rc");
	QString newPR3=QDir::convertSeparators(PrefsPfad+"/scrap13.scs");
	QString newPR4=QDir::convertSeparators(PrefsPfad+"/prefs13.xml");
		
	bool existsOldPR =QFile::exists(oldPR);
	bool existsOldPR2=QFile::exists(oldPR2);
	bool existsOldPR3=QFile::exists(oldPR3);
	bool existsOldPR4=QFile::exists(oldPR4);
	bool existsNewPR =QFile::exists(newPR);
	bool existsNewPR2=QFile::exists(newPR2);
	bool existsNewPR3=QFile::exists(newPR3);
	bool existsNewPR4=QFile::exists(newPR4);	
	
	//Only check for these two as they will be autocreated if they dont exist.
	if( (existsOldPR && !existsNewPR) || (existsOldPR4 && !existsNewPR4) )
	{
		if (splashScreen)
			splashScreen->hide();
		if ( (QMessageBox::question( this, tr("Migrate Old Scribus Settings?"),
				tr("Scribus has detected existing Scribus 1.2 preferences files.\n"
					"Do you want to migrate them to the new Scribus version?"),
				QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton))==QMessageBox::Yes )
		{
			if (existsOldPR && !existsNewPR)
				copyFile(oldPR, newPR);
			if (existsOldPR2 && !existsNewPR2)
				copyFile(oldPR2, newPR2);
			if (existsOldPR3 && !existsNewPR3)
				copyFile(oldPR3, newPR3);
			if (existsOldPR4 && !existsNewPR4)
				copyFile(oldPR4, newPR4);
		}
		if (splashScreen)
			splashScreen->show();
	}
	return PrefsPfad;
}

/*!
 \fn QString ScribusApp::convertToXMLPreferences(QString prefsLocation)
 \author Craig Bradney
 \date Sun 09 Jan 2005
 \brief Convert 1.2 prefs to 1.3 prefs
 \param prefsLocation Location of user preferences
 \retval None
 */
void ScribusApp::convertToXMLPreferences(QString prefsLocation)
{

}

void ScribusApp::initMenuBar()
{
	int MenID;
	QFont tmp;
	recentMenu = new QPopupMenu();
	RecentDocs.clear();
	fileMenu=new QPopupMenu();
	M_NewFile = fileMenu->insertItem(loadIcon("DateiNeu16.png"), tr("&New"), this, SLOT(slotFileNew()), CTRL+Key_N);
	fid13 = fileMenu->insertItem(loadIcon("DateiOpen16.png"), tr("&Open..."), this, SLOT(slotDocOpen()), CTRL+Key_O);
	fid14 = fileMenu->insertItem( tr("Open &Recent"), recentMenu);
	SetKeyEntry(0, tr("New"), M_NewFile, CTRL+Key_N);
	SetKeyEntry(1, tr("Open..."), fid13, CTRL+Key_O);
	fileMenu->insertSeparator();
	fid1 = fileMenu->insertItem(loadIcon("DateiClos16.png"), tr("&Close"), this, SLOT(slotFileClose()), CTRL+Key_W);
	SetKeyEntry(2, tr("Close"), fid1, CTRL+Key_W);
	fileMenu->setItemEnabled(fid1, 0);
	MenuItemsFile.append(fid1);
	fid4 = fileMenu->insertItem(loadIcon("DateiSave16.png"), tr("&Save"), this, SLOT(slotFileSave()), CTRL+Key_S);
	SetKeyEntry(3, tr("Save"), fid4, CTRL+Key_S);
	fileMenu->setItemEnabled(fid4, 0);
	M_SaveAs = fileMenu->insertItem( loadIcon("filesaveas.png"), tr("Save &As..."), this, SLOT(slotFileSaveAs()));
	SetKeyEntry(4, tr("Save as..."), M_SaveAs, 0);
	fileMenu->setItemEnabled(M_SaveAs, 0);
	fid52 = fileMenu->insertItem(loadIcon("revert.png"), tr("Re&vert to Saved"), this, SLOT(slotFileRevert()));
	fileMenu->setItemEnabled(fid52, 0);
	fid51 = fileMenu->insertItem( tr("Collect for O&utput..."), this, SLOT(Collect()));
	fileMenu->setItemEnabled(fid51, 0);
	MenuItemsFile.append(fid51);
	fileMenu->insertSeparator();
	importMenu = new QPopupMenu();
	fid2 = importMenu->insertItem( tr("&Get Text/Picture..."), this, SLOT(slotFileOpen()));
	importMenu->setItemEnabled(fid2, 0);
	fid2aa = importMenu->insertItem( tr("Append &Text..."), this, SLOT(slotFileAppend()));
	importMenu->setItemEnabled(fid2aa, 0);
	fid2a = importMenu->insertItem( tr("Import &Page(s)..."), this, SLOT(slotDocMerge()));
	importMenu->setItemEnabled(fid2a, 0);
	fileMenu->insertItem( tr("&Import"), importMenu);
	exportMenu = new QPopupMenu();
	fid3 = exportMenu->insertItem( tr("Save &Text..."), this, SLOT(SaveText()));
	exportMenu->setItemEnabled(fid3, 0);
	fid8 = exportMenu->insertItem( tr("Save Page as &EPS..."), this, SLOT(SaveAsEps()));
	exportMenu->setItemEnabled(fid8, 0);
	fid10 = exportMenu->insertItem( tr("Save as P&DF..."), this, SLOT(SaveAsPDF()));
	exportMenu->setItemEnabled(fid10, 0);
	fid11 = fileMenu->insertItem( tr("&Export"), exportMenu);
	fileMenu->setItemEnabled(fid11, 0);
	fileMenu->insertSeparator();
	fid6 = fileMenu->insertItem(loadIcon("documentinfo.png"), tr("Document &Information..."), this, SLOT(InfoDoc()), CTRL+Key_I);
	fileMenu->setItemEnabled(fid6, 0);
	MenuItemsFile.append(fid6);
	SetKeyEntry(5, tr("Document Info..."), fid6, CTRL+Key_I);
	fid7 = fileMenu->insertItem( tr("Document &Setup..."), this, SLOT(SetupDoc()));
	fileMenu->setItemEnabled(fid7, 0);
	MenuItemsFile.append(fid7);
	SetKeyEntry(6, tr("Document Setup..."), fid7, 0);
	M_Print = fileMenu->insertItem(loadIcon("DateiPrint16.png"), tr("&Print..."), this, SLOT(slotFilePrint()), CTRL+Key_P);
	fileMenu->setItemEnabled(M_Print, 0);
	MenuItemsFile.append(M_Print);
	SetKeyEntry(7, tr("Print..."), M_Print, CTRL+Key_P);
	fileMenu->insertSeparator();
	MenID = fileMenu->insertItem(loadIcon("exit.png"), tr("&Quit"), this, SLOT(slotFileQuit()), CTRL+Key_Q);
	SetKeyEntry(8, tr("Quit"), MenID, CTRL+Key_Q);
	editMenu = new QPopupMenu();
	edUndo = editMenu->insertItem( tr("&Undo"), this, SLOT(UnDoAction()), CTRL+Key_Z);
	edRedo = editMenu->insertItem(tr("&Redo"), this, SLOT(RedoAction()), CTRL+SHIFT+Key_Z);
	editMenu->insertSeparator();
	edid1 = editMenu->insertItem(loadIcon("editcut.png"), tr("Cu&t"), this , SLOT(slotEditCut()), CTRL+Key_X);
	edid2 = editMenu->insertItem(loadIcon("editcopy.png"), tr("&Copy"), this , SLOT(slotEditCopy()), CTRL+Key_C);
	edid3 = editMenu->insertItem(loadIcon("editpaste.png"), tr("&Paste"), this , SLOT(slotEditPaste()), CTRL+Key_V);
	edid4 = editMenu->insertItem(loadIcon("editdelete.png"), tr("C&lear"), this, SLOT(DeleteText()));
	edid5 = editMenu->insertItem( tr("Select &All"), this, SLOT(SelectAll()), CTRL+Key_A);
	SetKeyEntry(9, tr("Cut"), edid1, CTRL+Key_X);
	SetKeyEntry(10, tr("Copy"), edid2, CTRL+Key_C);
	SetKeyEntry(11, tr("Paste"), edid3, CTRL+Key_V);
	SetKeyEntry(12, tr("Clear"), edid4, 0);
	SetKeyEntry(13, tr("Select all"), edid5, CTRL+Key_A);
	editMenu->insertSeparator();
	Sear = editMenu->insertItem(loadIcon("find16.png"),  tr("&Search/Replace..."), this, SLOT(SearchText()));
	editMenu->insertSeparator();
	MenID = editMenu->insertItem( tr("C&olors..."), this , SLOT(slotEditColors()));
	SetKeyEntry(14, tr("Colors..."), MenID, 0);
	edid6 = editMenu->insertItem( tr("&Paragraph Styles..."), this , SLOT(slotEditStyles()));
	edid6a = editMenu->insertItem( tr("&Line Styles..."), this , SLOT(slotEditLineStyles()));
	SetKeyEntry(15, tr("Styles..."), edid6, 0);
	tman = editMenu->insertItem( tr("&Templates..."), this, SLOT(ManageTemp()));
	SetKeyEntry(16, tr("Templates..."), tman, 0);
	jman = editMenu->insertItem( tr("&Javascripts..."), this, SLOT(ManageJava()));
	MenID = editMenu->insertItem( tr("P&references..."), this , SLOT(slotPrefsOrg()));
	MenID = editMenu->insertItem( tr("&Fonts..."), this , SLOT(slotFontOrg()));
	SetKeyEntry(17, tr("Fonts..."), MenID, 0);
	editMenu->setItemEnabled(edUndo, 0);
	editMenu->setItemEnabled(edRedo, 0);
	editMenu->setItemEnabled(edid1, 0);
	editMenu->setItemEnabled(edid2, 0);
	editMenu->setItemEnabled(edid3, 0);
	editMenu->setItemEnabled(edid4, 0);
	editMenu->setItemEnabled(edid5, 0);
	editMenu->setItemEnabled(edid6, 0);
	editMenu->setItemEnabled(edid6a, 0);
	editMenu->setItemEnabled(Sear, 0);
	editMenu->setItemEnabled(tman, 0);
	editMenu->setItemEnabled(jman, 0);
	StilMenu = new QPopupMenu();
	ObjMenu = new QPopupMenu();
	SetKeyEntry(19, tr("Select New Font"), 0, 0);
	ODup = ObjMenu->insertItem( tr("D&uplicate"), this, SLOT(ObjektDup()), CTRL+Key_D);
	SetKeyEntry(20, tr("Duplicate"), ODup, CTRL+Key_D);
	OMDup = ObjMenu->insertItem( tr("&Multiple Duplicate"), this, SLOT(ObjektDupM()));
	SetKeyEntry(21, tr("Multiple Duplicate"), OMDup, 0);
	Loesch = ObjMenu->insertItem( tr("&Delete"), this, SLOT(DeleteObjekt()), CTRL+Key_K);
	SetKeyEntry(22, tr("Delete"), Loesch, CTRL+Key_K);
	ObjMenu->insertSeparator();
	Gr = ObjMenu->insertItem( tr("&Group"), this, SLOT(GroupObj()), CTRL+Key_G);
	SetKeyEntry(23, tr("Group"), Gr, CTRL+Key_G);
	UnGr = ObjMenu->insertItem( tr("&Ungroup"), this, SLOT(UnGroupObj()),CTRL+Key_U);
	SetKeyEntry(24, tr("Un-group"), UnGr, CTRL+Key_U);
	LockOb = ObjMenu->insertItem( tr("&Lock"), this, SLOT(ToggleObjLock()), CTRL+Key_F);
	ObjMenu->insertSeparator();
	OBack = ObjMenu->insertItem( tr("Send to &Back"), this, SLOT(Objekt2Back()));
	SetKeyEntry(25, tr("Send to Back"), OBack, 0);
	OFront = ObjMenu->insertItem( tr("Bring to &Front"), this, SLOT(Objekt2Front()));
	SetKeyEntry(26, tr("Bring to Front"), OFront, 0);
	OLower = ObjMenu->insertItem( tr("&Lower"), this, SLOT(ObjektLower()));
	SetKeyEntry(27, tr("Lower"), OLower, 0);
	ORaise = ObjMenu->insertItem( tr("&Raise"), this, SLOT(ObjektRaise()));
	SetKeyEntry(28, tr("Raise"), ORaise, 0);
	DistM = ObjMenu->insertItem( tr("Distribute/&Align..."), this, SLOT(ObjektAlign()));
	SetKeyEntry(29, tr("Distribute/Align..."), DistM, 0);
	ObjMenu->insertSeparator();
	ShapeMenu = new QPopupMenu();
	SCustom = new Autoforms(0);
	ShapeMenu->insertItem(SCustom);
	connect(SCustom, SIGNAL(FormSel(int, int, double *)), this, SLOT(MakeFrame(int, int, double *)));
	ShapeEdit = ShapeMenu->insertItem( tr("&Edit Shape"), this, SLOT(ToggleFrameEdit()));
	ShapeM = ObjMenu->insertItem( tr("&Shape"), ShapeMenu);
	PfadT = ObjMenu->insertItem( tr("&Attach Text to Path"), this, SLOT(Pfadtext()));
	PfadDT = ObjMenu->insertItem( tr("&Detach Text from Path"), this, SLOT(noPfadtext()));
	PfadV = ObjMenu->insertItem( tr("&Combine Polygons"), this, SLOT(UniteOb()));
	PfadS = ObjMenu->insertItem( tr("Split &Polygons"), this, SLOT(SplitUniteOb()));
	PfadTP = ObjMenu->insertItem( tr("C&onvert to Outlines"), this, SLOT(TraceText()));
	ObjMenu->setItemEnabled(ShapeM, 0);
	ObjMenu->setItemEnabled(DistM, 0);
	ObjMenu->setItemEnabled(Gr, 0);
	ObjMenu->setItemEnabled(UnGr, 0);
	ObjMenu->setItemEnabled(PfadT, 0);
	ObjMenu->setItemEnabled(PfadDT, 0);
	ObjMenu->setItemEnabled(PfadV, 0);
	ObjMenu->setItemEnabled(PfadS, 0);
	ObjMenu->setItemEnabled(LockOb, 0);
	ObjMenu->setItemEnabled(PfadTP, 0);
	pageMenu = new QPopupMenu();
	MenID = pageMenu->insertItem( tr("&Insert..."), this, SLOT(slotNewPageM()));
	SetKeyEntry(30, tr("Insert..."), MenID, 0);
	pgmd = pageMenu->insertItem( tr("&Delete..."), this, SLOT(DeletePage()));
	SetKeyEntry(31, tr("Delete..."), pgmd, 0);
	MenID = pageMenu->insertItem( tr("&Copy")+"...", this, SLOT(CopyPage()));
	SetKeyEntry(61, tr("Copy")+"...", MenID, 0);
	pgmv = pageMenu->insertItem( tr("&Move..."), this, SLOT(MovePage()));
	SetKeyEntry(32, tr("Move..."), pgmv, 0);
	MenID = pageMenu->insertItem( tr("&Apply Template..."), this, SLOT(ApplyTemp()));
	SetKeyEntry(33, tr("Apply Template..."), MenID, 0);
	MenID = pageMenu->insertItem( tr("Manage &Guides..."), this, SLOT(ManageGuides()));
	SetKeyEntry(49, tr("Manage Guides..."), MenID, 0);
	pageMenu->setItemEnabled(pgmd, 0);
	pageMenu->setItemEnabled(pgmv, 0);
	viewMenu=new QPopupMenu();
	MenID = viewMenu->insertItem( tr("&Fit in Window"), this, SLOT(slotZoomFit()), CTRL+Key_0);
	SetKeyEntry(34, tr("Fit in Window"), MenID, CTRL+Key_0);
	MenID = viewMenu->insertItem("&50%", this, SLOT(slotZoom50()));
	SetKeyEntry(35, tr("50%"), MenID, 0);
	MenID = viewMenu->insertItem("&75%", this, SLOT(slotZoom75()));
	SetKeyEntry(36, tr("75%"), MenID, 0);
	MenID = viewMenu->insertItem( tr("&100%"), this, SLOT(slotZoom100()), CTRL+Key_1);
	SetKeyEntry(37, tr("100%"), MenID, CTRL+Key_1);
	MenID = viewMenu->insertItem("&200%", this, SLOT(slotZoom200()));
	SetKeyEntry(38, tr("200%"), MenID, 0);
	MenID = viewMenu->insertItem( tr("&Thumbnails"), this, SLOT(slotZoom20()));
	SetKeyEntry(39, tr("Thumbnails"), MenID, 0);
	viewMenu->insertSeparator();
	Markers = viewMenu->insertItem( tr("Show &Margins"), this, SLOT(ToggleMarks()));
	SetKeyEntry(40, tr("Hide Margins"), Markers, 0);
	viewMenu->setItemChecked(Markers, true);
	FrameDr = viewMenu->insertItem( tr("Show &Frames"), this, SLOT(ToggleFrames()));
	SetKeyEntry(41, tr("Hide Frames"), FrameDr, 0);
	viewMenu->setItemChecked(FrameDr, true);
	Bilder = viewMenu->insertItem( tr("Show &Images"), this, SLOT(TogglePics()));
	viewMenu->setItemChecked(Bilder, true);
	SetKeyEntry(42, tr("Hide Images"), Bilder, 0);
	Ras = viewMenu->insertItem( tr("Show &Grid"), this, SLOT(ToggleRaster()));
	SetKeyEntry(43, tr("Show Grid"), Ras, 0);
	viewMenu->setItemChecked(Ras, false);
	Guide = viewMenu->insertItem( tr("Show G&uides"), this, SLOT(ToggleGuides()));
	viewMenu->setItemChecked(Guide, true);
	Base = viewMenu->insertItem( tr("Show &Baseline Grid"), this, SLOT(ToggleBase()));
	viewMenu->setItemChecked(Base, false);
	textLinks = viewMenu->insertItem( tr("Show &Text Chain"), this, SLOT(ToggleTextLinks()));
	viewMenu->setItemChecked(textLinks, false);
	viewMenu->insertSeparator();
	uRas = viewMenu->insertItem( tr("Sn&ap to Grid"), this, SLOT(ToggleURaster()));
	SetKeyEntry(44, tr("Snap to Grid"), uRas, 0);
	uGuide = viewMenu->insertItem( tr("Sna&p to Guides"), this, SLOT(ToggleUGuides()));
	toolMenu=new QPopupMenu();
	viewMpal = toolMenu->insertItem( tr("&Properties"), this, SLOT(ToggleMpal()));
	SetKeyEntry(46, tr("Properties"), viewMpal, 0);
	viewTpal = toolMenu->insertItem( tr("&Outline"), this, SLOT(ToggleTpal()));
	SetKeyEntry(47, tr("Outline"), viewTpal, 0);
	viewBpal = toolMenu->insertItem( tr("&Scrapbook"), this, SLOT(ToggleBpal()));
	SetKeyEntry(48, tr("Scrapbook"), viewBpal, 0);
	viewLpal = toolMenu->insertItem( tr("&Layers"), this, SLOT(ToggleLpal()));
	viewSepal = toolMenu->insertItem( tr("P&age Palette"), this, SLOT(ToggleSepal()));
	viewBopal = toolMenu->insertItem( tr("&Bookmarks"), this, SLOT(ToggleBookpal()));
	viewUndoPalette = toolMenu->insertItem(tr("&Undo History"), this, SLOT(ToggleUndoPalette()));
	toolbarMenuTools = toolMenu->insertItem( tr("&Tools"), this, SLOT(ToggleTools()));
	toolbarMenuPDFTools = toolMenu->insertItem( tr("P&DF Tools"), this, SLOT(TogglePDFTools()));
	SetKeyEntry(45, tr("Tools"), toolbarMenuTools, 0);
	SetKeyEntry(55, tr("Tooltips"), tip, 0);
	extraMenu=new QPopupMenu();
	MenID = extraMenu->insertItem( tr("&Manage Pictures"), this, SLOT(StatusPic()));
	SetKeyEntry(51, tr("Manage Pictures"), MenID, 0);
	hyph = extraMenu->insertItem( tr("&Hyphenate Text"), this, SLOT(doHyphenate()));
	extraMenu->setItemEnabled(hyph, 0);
	SetKeyEntry(50, tr("Hyphenate Text"), hyph, 0);

	windowsMenu = new QPopupMenu();
	windowsMenu->setCheckable( true );
	connect(windowsMenu, SIGNAL(aboutToShow()), this, SLOT(windowsMenuAboutToShow()));
	helpMenu=new QPopupMenu();
	MenID = helpMenu->insertItem( tr("&About Scribus"), this, SLOT(slotHelpAbout()));
	SetKeyEntry(52, tr("About Scribus"), MenID, 0);
	MenID = helpMenu->insertItem( tr("About &Qt"), this, SLOT(slotHelpAboutQt()));
	SetKeyEntry(53, tr("About Qt"), MenID, 0);
	helpMenu->insertSeparator();
	tip = helpMenu->insertItem( tr("Toolti&ps"), this, SLOT(ToggleTips()));
	tipsOn = true;
	helpMenu->setItemChecked(tip, tipsOn);
	MenID = helpMenu->insertItem( tr("Scribus &Manual..."), this, SLOT(slotOnlineHelp()));
	SetKeyEntry(54, tr("Online-Help..."), MenID, 0);

/*	editMenu->insertItem( tr("Test"), this, SLOT(slotTest()));
	editMenu->insertItem( tr("Test2"), this, SLOT(slotTest2()));
*/
	menuBar()->insertItem( tr("&File"), fileMenu);
	menuBar()->insertItem( tr("&Edit"), editMenu);
	Stm = menuBar()->insertItem( tr("St&yle"), StilMenu);
	Obm = menuBar()->insertItem( tr("&Item"), ObjMenu);
	pgmm = menuBar()->insertItem( tr("&Page"), pageMenu);
	menuBar()->setItemEnabled(Stm, 0);
	menuBar()->setItemEnabled(Obm, 0);
	menuBar()->setItemEnabled(pgmm, 0);
	ViMen = menuBar()->insertItem( tr("&View"), viewMenu);
	menuBar()->setItemEnabled(ViMen, 0);
	menuBar()->insertItem( tr("&Tools"), toolMenu);
	exmn = menuBar()->insertItem( tr("E&xtras"), extraMenu);
	menuBar()->setItemEnabled(exmn, 0);
	WinMen = menuBar()->insertItem( tr("&Windows"), windowsMenu );
	menuBar()->setItemEnabled(WinMen, 0);
	menuBar()->insertSeparator();
	menuBar()->insertItem( tr("&Help"), helpMenu);
	AliMenu = new QPopupMenu();
	AliMenu->insertItem( tr("&Left"));
	AliMenu->insertItem( tr("&Center"));
	AliMenu->insertItem( tr("&Right"));
	AliMenu->insertItem( tr("&Block"));
	AliMenu->insertItem( tr("&Forced"));
	ColorMenu = new QPopupMenu();
	ColorMenC = new QComboBox(false);
	ColorMenC->setEditable(false);
	ColorMenu->insertItem(ColorMenC);
	SizeTMenu = new QPopupMenu();
	SizeTMenu->insertItem( tr("&Other..."));
	QString ar_sizes[] = {" 7", " 9", "10", "11", "12", "14", "18", "24", "36", "48", "60", "72"};
	size_t f_size = sizeof(ar_sizes) / sizeof(*ar_sizes);
	for (uint s = 0; s < f_size; ++s)
		SizeTMenu->insertItem(ar_sizes[s] + tr(" pt"));
	ShadeMenu = new QPopupMenu();
	ShadeMenu->insertItem( tr("&Other..."));
	ShadeMenu->insertItem("0 %");
	ShadeMenu->insertItem("10 %");
	ShadeMenu->insertItem("20 %");
	ShadeMenu->insertItem("30 %");
	ShadeMenu->insertItem("40 %");
	ShadeMenu->insertItem("50 %");
	ShadeMenu->insertItem("60 %");
	ShadeMenu->insertItem("70 %");
	ShadeMenu->insertItem("80 %");
	ShadeMenu->insertItem("90 %");
	ShadeMenu->insertItem("100 %");
	FontMenu = new QPopupMenu();
	TypeStyleMenu = new QPopupMenu();
	TypeStyleMenu->insertItem( tr("Normal"));
	tmp = qApp->font();
	tmp = qApp->font();
	tmp.setUnderline(true);
	TypeStyleMenu->insertItem(new FmItem( tr("Underline"), tmp));
	tmp = qApp->font();
	tmp.setStrikeOut(true);
	TypeStyleMenu->insertItem(new FmItem( tr("Strikethru"), tmp));
	TypeStyleMenu->insertItem( tr("Small Caps"));
	TypeStyleMenu->insertItem( tr("Superscript"));
	TypeStyleMenu->insertItem( tr("Subscript"));
	TypeStyleMenu->insertItem( tr("Outlined"));

	SetKeyEntry(61, tr("Attach Text to Path"), PfadT, 0);
	SetKeyEntry(62, tr("Show Layers"), viewLpal, 0);
	SetKeyEntry(63, tr("Javascripts..."), jman, 0);
	SetKeyEntry(64, tr("Undo"), edUndo, CTRL+Key_Z);
	SetKeyEntry(65, tr("Redo"), edRedo, CTRL+SHIFT+Key_Z);
	SetKeyEntry(66, tr("Show Page Palette"), viewSepal, 0);
	SetKeyEntry(67, tr("Lock/Unlock"), LockOb, CTRL+Key_F);

	connect(editMenu, SIGNAL(aboutToShow()), this, SLOT(refreshUndoRedoItems()));
	connect(UndoManager::instance(), SIGNAL(newAction(UndoObject*, UndoState*)),
	        this, SLOT(refreshUndoRedoItems()));
	connect(UndoManager::instance(), SIGNAL(undoRedoDone()), this, SLOT(refreshUndoRedoItems()));
	connect(toolMenu, SIGNAL(aboutToShow()), this, SLOT(refreshUndoRedoItems()));
	connect(recentMenu, SIGNAL(activated(int)), this, SLOT(LoadRecent(int)));
	connect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
	connect(ShadeMenu, SIGNAL(activated(int)), this, SLOT(setItemShade(int)));
	connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
	connect(SizeTMenu, SIGNAL(activated(int)), this, SLOT(setItemFSize(int)));
	connect(TypeStyleMenu, SIGNAL(activated(int)), this, SLOT(setItemTypeStyle(int)));
	connect(AliMenu, SIGNAL(activated(int)), this, SLOT(setItemTextAli(int)));
}

void ScribusApp::initStatusBar()
{
	FMess = new QLabel( "           ", statusBar(), "ft");
	FProg = new QProgressBar(statusBar(), "p");
	FProg->setCenterIndicator(true);
	FProg->setFixedWidth( 100 );
	FProg->reset();
	XMess = new QLabel( tr("X-Pos:"), statusBar(), "xt");
	YMess = new QLabel( tr("Y-Pos:"), statusBar(), "yt");
	XDat = new QLabel( "         ", statusBar(), "dt");
	YDat = new QLabel( "         ", statusBar(), "ydt");

	statusBar()->addWidget(FMess, 3, true);
	statusBar()->addWidget(FProg, 0, true);
	statusBar()->addWidget(XMess, 0, true);
	statusBar()->addWidget(XDat, 1, true);
	statusBar()->addWidget(YMess, 0, true);
	statusBar()->addWidget(YDat, 1, true);
}

void ScribusApp::ReportMP(double xp, double yp)
{
	QString suffix=unitGetSuffixFromIndex(doc->docUnitIndex);
	int multiplier=unitGetDecimalsFromIndex(doc->docUnitIndex);
	double divisor = static_cast<double>(multiplier);
	int precision=precision = unitGetPrecisionFromIndex(doc->docUnitIndex);
	QString tmp;
	XDat->setText(tmp.setNum(qRound(xp*UmReFaktor * multiplier) / divisor, 'f', precision) + suffix);
	YDat->setText(tmp.setNum(qRound(yp*UmReFaktor * multiplier) / divisor, 'f', precision) + suffix);
}

void ScribusApp::SetKeyEntry(int Nr, QString text, int Men, int KeyC)
{
	Keys ke;
	ke.Name = text;
	ke.MenuID = Men;
	ke.KeyID = KeyC;
	Prefs.KeyActions.insert(Nr, ke);
}

void ScribusApp::DeleteSel(PageItem *b)
{
	int FirstSel = 0;
	bool first = false;
	for (ScText *it = b->itemText.first(); it != 0; it = b->itemText.next())
	{
		if (it->cselect)
		{
			first = true;
			b->itemText.remove();
			it = b->itemText.prev();
			if (it == 0)
				it = b->itemText.first();
		}
		if (!first)
			FirstSel++;
	}
	if (b->itemText.count() != 0)
	{
		if (b->itemText.first()->cselect)
		{
			b->itemText.remove();
			b->CPos = 0;
		}
		else
			b->CPos = FirstSel;
	}
	else
		b->CPos = 0;
	b->HasSel = false;
	DisableTxEdit();
}

void ScribusApp::setTBvals(PageItem *b)
{
	if (b->itemText.count() != 0)
	{
		int ChPos = QMIN(b->CPos, static_cast<int>(b->itemText.count()-1));
		doc->CurrentStyle = b->itemText.at(ChPos)->cstyle & 127;
		doc->currentParaStyle = b->itemText.at(ChPos)->cab;
		setAbsValue(doc->currentParaStyle);
		Mpal->setAli(doc->currentParaStyle);
		doc->CurrFont = b->itemText.at(ChPos)->cfont;
		doc->CurrFontSize = b->itemText.at(ChPos)->csize;
		doc->CurrTextFill = b->itemText.at(ChPos)->ccolor;
		doc->CurrTextFillSh = b->itemText.at(ChPos)->cshade;
		doc->CurrTextStroke = b->itemText.at(ChPos)->cstroke;
		doc->CurrTextStrokeSh = b->itemText.at(ChPos)->cshade2;
		doc->CurrTextScale = b->itemText.at(ChPos)->cscale;
		emit TextFarben(doc->CurrTextStroke, doc->CurrTextFill, doc->CurrTextStrokeSh, doc->CurrTextFillSh);
		emit TextIFont(doc->CurrFont);
		emit TextISize(doc->CurrFontSize);
		emit TextUSval(b->itemText.at(ChPos)->cextra);
		emit TextStil(doc->CurrentStyle);
		emit TextScale(doc->CurrTextScale);
	}
}

void ScribusApp::wheelEvent(QWheelEvent *w)
{
	if (HaveDoc)
	{
		if ((w->orientation() != Qt::Vertical) || ( w->state() & ShiftButton ))
		{
			if (w->delta() < 0)
				view->scrollBy(Prefs.Wheelval, 0);
			else
				view->scrollBy(-Prefs.Wheelval, 0);
		}
		else
		{
			if (w->delta() < 0)
				view->scrollBy(0, Prefs.Wheelval);
			else
				view->scrollBy(0, -Prefs.Wheelval);
		}
		w->accept();
	}
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
		KeyMod = 0x00200000;
		break;
	case AltButton:
		KeyMod = 0x00800000;
		break;
	case ControlButton:
		KeyMod = 0x00400000;
		break;
	default:
		KeyMod = 0;
		break;
	}
	if (kk == Key_F10)
	{
		keyrep = false;
		ToggleAllPalettes();
		return;
	}
	if ((kk == Key_F11) && (HaveDoc))
	{
		keyrep = false;
		ToggleAllGuides();
		return;
	}
	if ((kk == Key_Escape) && (HaveDoc))
	{
		keyrep = false;
		PageItem *b;
		if ((view->SelItem.count() != 0))
		{
			b = view->SelItem.at(0);
			switch (doc->appMode)
			{
				case NormalMode:
					b->Sizing = false;
					if (doc->SubMode != -1)
					{
						view->Deselect(false);
//						if (!doc->TemplateMode)
//							Tpal->slotRemoveElement(doc->currentPage->PageNr, b->ItemNr);
						doc->Items.remove(b->ItemNr);
					}
					break;
				case EditMode:
					break;
				case DrawBezierLine:
					b->PoLine.resize(b->PoLine.size()-2);
					if (b->PoLine.size() < 4)
					{
						view->Deselect(false);
//						if (!doc->TemplateMode)
//							Tpal->slotRemoveElement(doc->currentPage->PageNr, b->ItemNr);
						doc->Items.remove(b->ItemNr);
					}
					else
					{
						view->SizeItem(b->PoLine.WidthHeight().x(), b->PoLine.WidthHeight().y(), b->ItemNr, false, false);
						view->SetPolyClip(b, qRound(QMAX(b->Pwidth / 2, 1)));
						view->AdjustItemSize(b);
						b->ContourLine = b->PoLine.copy();
						b->ClipEdited = true;
						b->FrameType = 3;
						slotDocCh();
					}
					view->FirstPoly = true;
					break;
				default:
					view->Deselect(false);
//					if (!doc->TemplateMode)
//						Tpal->slotRemoveElement(doc->currentPage->PageNr, b->ItemNr);
					doc->Items.remove(b->ItemNr);
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
		NoFrameEdit();
		slotSelect();
		return;
	}
	ButtonState buttonState = k->state();
	if ((HaveDoc) && (!view->LE->hasFocus()) && (!view->PGS->PageCombo->hasFocus()))
	{
		if ((doc->appMode != EditMode) && (view->SelItem.count() == 0))
		{
			switch (kk)
			{
			case Key_Space:
				keyrep = false;
				if (doc->appMode == PanningMode)
					setAppMode(NormalMode);
				else
				{
					setAppMode(PanningMode);
					qApp->setOverrideCursor(QCursor(loadIcon("HandC.xpm")), true);
				}
				return;
				break;
			case Key_Prior:
				view->scrollBy(0, -Prefs.Wheelval);
				keyrep = false;
				return;
				break;
			case Key_Next:
				view->scrollBy(0, Prefs.Wheelval);
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
					doc->OpenNodes = Tpal->buildReopenVals();
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
			PageItem *b = view->SelItem.at(0);
			if (kk == Key_F9)
			{
				keyrep = false;
				if (doc->appMode == EditMode)
					setAppMode(NormalMode);
				else
					setAppMode(EditMode);
				return;
			}
			switch (doc->appMode)
			{
			case NormalMode:
				switch (kk)
				{
				case Key_Backspace:
				case Key_Delete:
					if (!doc->EditClip)
						view->DeleteItem();
					break;
				case Key_Prior:
					if (!b->Locked)
						view->RaiseItem();
					break;
				case Key_Next:
					if (!b->Locked)
						view->LowerItem();
					break;
				case Key_Left:
					if (!b->Locked)
					{
						if ( buttonState & ShiftButton )
							view->moveGroup(-10, 0);
						else if ( buttonState & ControlButton )
							view->moveGroup(-0.1, 0);
						else
							view->moveGroup(-1, 0);
					}
					break;
				case Key_Right:
					if (!b->Locked)
					{
						if ( buttonState & ShiftButton )
							view->moveGroup(10, 0);
						else if ( buttonState & ControlButton )
							view->moveGroup(0.1, 0);
						else
							view->moveGroup(1, 0);
					}
					break;
				case Key_Up:
					if (!b->Locked)
					{
						if ( buttonState & ShiftButton )
							view->moveGroup(0, -10);
						else if ( buttonState & ControlButton )
							view->moveGroup(0, -0.1);
						else
							view->moveGroup(0, -1);
					}
					break;
				case Key_Down:
					if (!b->Locked)
					{
						if ( buttonState & ShiftButton )
							view->moveGroup(0, 10);
						else if ( buttonState & ControlButton )
							view->moveGroup(0, 0.1);
						else
							view->moveGroup(0, 1);
					}
					break;
				default:
					if (b->PType == 4)
					{
						if ((kk + KeyMod) == Prefs.KeyActions[59].KeyID)
						{
							setNewAbStyle(1);
							b->Tinput = true;
							view->RefreshItem(b);
						}
						if ((kk + KeyMod) == Prefs.KeyActions[58].KeyID)
						{
							setNewAbStyle(2);
							b->Tinput = true;
							view->RefreshItem(b);
						}
						if ((kk + KeyMod) == Prefs.KeyActions[57].KeyID)
						{
							setNewAbStyle(0);
							b->Tinput = true;
							view->RefreshItem(b);
						}
					}
					break;
				}
				slotDocCh();
				break;
			case EditMode:
				int oldPos = b->CPos; // 15-mar-2004 jjsa for cursor movement with Shift + Arrow key
				view->oldCp = b->CPos;
				if (b->PType == 4)
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
							view->deselectAll(b);
					}
					if (UniCinp)
					{
						int conv = 0;
						bool ok = false;
						UniCinS += uc;
						conv = UniCinS.toInt(&ok, 16);
						if (!ok)
						{
							UniCinp = false;
							UniCinC = 0;
							UniCinS = "";
							keyrep = false;
							return;
						}
						UniCinC++;
						if (UniCinC == 4)
						{
							UniCinp = false;
							UniCinC = 0;
							UniCinS = "";
							if (ok)
							{
								if (b->HasSel)
									DeleteSel(b);
								if (conv < 31)
									conv = 32;
								hg = new ScText;
								hg->ch = QString(QChar(conv));
								hg->cfont = doc->CurrFont;
								hg->csize = doc->CurrFontSize;
								hg->ccolor = doc->CurrTextFill;
								hg->cshade = doc->CurrTextFillSh;
								hg->cstroke = doc->CurrTextStroke;
								hg->cshade2 = doc->CurrTextStrokeSh;
								hg->cscale = doc->CurrTextScale;
								hg->cselect = false;
								hg->cstyle = doc->CurrentStyle;
								hg->cab = doc->currentParaStyle;
								if (doc->docParagraphStyles[doc->currentParaStyle].Font != "")
								{
									hg->cfont = doc->docParagraphStyles[doc->currentParaStyle].Font;
									hg->csize = doc->docParagraphStyles[doc->currentParaStyle].FontSize;
								}
								hg->cextra = 0;
								hg->xp = 0;
								hg->yp = 0;
								hg->PRot = 0;
								hg->PtransX = 0;
								hg->PtransY = 0;
								b->itemText.insert(b->CPos, hg);
								b->CPos += 1;
								b->Tinput = true;
								setTBvals(b);
								view->RefreshItem(b);
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
						UniCinp = true;
						UniCinC = 0;
						UniCinS = "";
						keyrep = false;
						return;
						break;
					case Key_Home:
						// go to begin of line
						if ( (pos = b->CPos) == 0 )
							break; // at begin of frame
						len = static_cast<int>(b->itemText.count());
						if ( pos == len )
							pos--;
						if ( (buttonState & ControlButton) == 0 )
						{
							alty =  b->itemText.at(pos)->yp;
							c = b->itemText.at(pos)->ch.at(0).latin1();
							if ( c == 13 ) // new line, position is wrong
								if ( --pos > 0 )
									alty =  b->itemText.at(pos)->yp;
							// check for yp at actual position
							if ( pos < len )
							{
								altx =  b->itemText.at(pos)->yp;
								if ( altx > alty )
								{
									// we was at begin of line
									break;
								}
							}
							while (  pos > 0 && b->itemText.at(pos-1)->yp == alty )
								pos--;
							if ( b->itemText.at(pos)->ch.at(0).latin1() == 13 )
								pos++;
						}
						else
						{
							// paragraph begin
							if ( pos < len &&
							        b->itemText.at(pos)->ch.at(0).latin1() == 13 )
								pos--;
							while(pos > 0 )
								if ( b->itemText.at(pos)->ch.at(0).latin1() == 13 )
								{
									pos++;
									break;
								}
								else
									pos--;
						}
						b->CPos = pos;
						if ( buttonState & ShiftButton )
							view->ExpandSel(b, -1, oldPos);
						break;
					case Key_End:
						// go to end of line
						len = static_cast<int>(b->itemText.count());
						if ( b->CPos >= len )
							break; // at end of frame
						if ( (buttonState & ControlButton) == 0 )
						{
							if ((b->CPos < len) && ((b->itemText.at(b->CPos)->ch.at(0).latin1() == 13) || (b->itemText.at(b->CPos)->ch.at(0).latin1() == 28)))
							{
								// at end of paragraph and therefore line
								break;
							}
							alty =  b->itemText.at(b->CPos)->yp;
							while (  b->CPos < len-1 &&  b->itemText.at(b->CPos+1)->yp == alty )
								b->CPos++;

							if ( b->CPos < len -1 )
								c = b->itemText.at(b->CPos+1)->ch.at(0).latin1();
							else if ( b->CPos == len - 1 )
								c = 13;
							else
								c = 0;
							if (( c == 13 ) || (c = 28))
								b->CPos++;
						}
						else
						{
							// go to end of paragraph
							if ( b->itemText.at(b->CPos)->ch.at(0).latin1() == 13 )
							{
								break;
							}
							pos = b->CPos;
							while ( pos < len )
							{
								if ( b->itemText.at(pos)->ch.at(0).latin1() == 13 )
									break;
								else
									pos++;
							}
							b->CPos = pos;
						}
						if ( buttonState & ShiftButton )
							view->ExpandSel(b, 1, oldPos);
						break;
					case Key_Down:
						if (b->CPos != static_cast<int>(b->itemText.count()))
						{
							alty = b->itemText.at(b->CPos)->yp;
							altx = b->itemText.at(b->CPos)->xp;
							do
							{
								b->CPos += 1;
								if (b->CPos == static_cast<int>(b->itemText.count()))
									break;
								if (b->itemText.at(b->CPos)->yp > alty)
								{
									if (b->itemText.at(b->CPos)->xp >= altx)
										break;
								}
							}
							while (b->CPos < static_cast<int>(b->itemText.count()));
							if ( buttonState & ShiftButton )
							{
								if ( buttonState & AltButton )
									b->CPos = b->itemText.count();
								view->ExpandSel(b, 1, oldPos);
							}
							else
								if (b->CPos == static_cast<int>(b->itemText.count()))
									if (b->NextBox != 0)
									{
										if (b->NextBox->itemText.count() != 0)
										{
											view->Deselect(true);
											b->NextBox->CPos = 0;
											view->SelectItemNr(b->NextBox->ItemNr);
											b = b->NextBox;
										}
									}
						}
						else
						{
							if (b->NextBox != 0)
							{
								if (b->NextBox->itemText.count() != 0)
								{
									view->Deselect(true);
									b->NextBox->CPos = 0;
									view->SelectItemNr(b->NextBox->ItemNr);
									b = b->NextBox;
								}
							}
						}
						if ( b->HasSel )
							view->RefreshItem(b);
						setTBvals(b);
						break;
					case Key_Up:
						if (b->CPos > 0)
						{
							if (b->CPos == static_cast<int>(b->itemText.count()))
								b->CPos -= 1;
							alty = b->itemText.at(b->CPos)->yp;
							altx = b->itemText.at(b->CPos)->xp;
							if (b->CPos > 0)
							{
								do
								{
									b->CPos -= 1;
									if (b->CPos == 0)
										break;
									if (b->itemText.at(b->CPos)->yp < alty)
									{
										if (b->itemText.at(b->CPos)->xp <= altx)
											break;
									}
								}
								while (b->CPos > 0);
							}
							if ( buttonState & ShiftButton )
							{
								if ( buttonState & AltButton )
									b->CPos = 0;
								view->ExpandSel(b, -1, oldPos);
							}
							else
								if (b->CPos == 0)
								{
									if (b->BackBox != 0)
									{
										view->Deselect(true);
										b->BackBox->CPos = b->BackBox->itemText.count();
										view->SelectItemNr(b->BackBox->ItemNr);
										b = b->BackBox;
									}
								}
						}
						else
						{
							b->CPos = 0;
							if (b->BackBox != 0)
							{
								view->Deselect(true);
								b->BackBox->CPos = b->BackBox->itemText.count();
								view->SelectItemNr(b->BackBox->ItemNr);
								b = b->BackBox;
							}
						}
						if ( b->HasSel )
							view->RefreshItem(b);
						setTBvals(b);
						break;
					case Key_Prior:
						b->CPos = 0;
						if ( buttonState & ShiftButton )
							view->ExpandSel(b, -1, oldPos);
						setTBvals(b);
						break;
					case Key_Next:
						b->CPos = static_cast<int>(b->itemText.count());
						if ( buttonState & ShiftButton )
							view->ExpandSel(b, 1, oldPos);
						setTBvals(b);
						break;
					case Key_Left:
						if ( buttonState & ControlButton )
						{
							view->setNewPos(b, oldPos, b->itemText.count(),-1);
							if ( buttonState & ShiftButton )
								view->ExpandSel(b, -1, oldPos);
						}
						else if ( buttonState & ShiftButton )
						{
							b->CPos--;
							if ( b->CPos < 0 )
								b->CPos = 0;
							else
								view->ExpandSel(b, -1, oldPos);
						}
						else
						{
							b->CPos -= 1;
							if (b->CPos < 0)
							{
								b->CPos = 0;
								if (b->BackBox != 0)
								{
									view->Deselect(true);
									b->BackBox->CPos = b->BackBox->itemText.count();
									view->SelectItemNr(b->BackBox->ItemNr);
									b = b->BackBox;
								}
							}
						}
						if ((b->CPos > 0) && (b->CPos == static_cast<int>(b->itemText.count())))
						{
							if (b->itemText.at(b->CPos-1)->cstyle & 256)
							{
								b->CPos -= 1;
								while ((b->CPos > 0) && (b->itemText.at(b->CPos)->cstyle & 256))
								{
									b->CPos--;
									if (b->CPos == 0)
										break;
								}
							}
						}
						else
						{
							while ((b->CPos > 0) && (b->itemText.at(b->CPos)->cstyle & 256))
							{
								b->CPos--;
								if (b->CPos == 0)
									break;
							}
						}
						if ( b->HasSel )
							view->RefreshItem(b);
						setTBvals(b);
						break;
					case Key_Right:
						if ( buttonState & ControlButton )
						{
							view->setNewPos(b, oldPos, b->itemText.count(),1);
							if ( buttonState & ShiftButton )
								view->ExpandSel(b, 1, oldPos);
						}
						else if ( buttonState & ShiftButton )
						{
							b->CPos++;
							if ( b->CPos > static_cast<int>(b->itemText.count()) )
								b->CPos--;
							else
								view->ExpandSel(b, 1, oldPos);
						}
						else
						{
							b->CPos += 1; // new position within text ?
							if (b->CPos > static_cast<int>(b->itemText.count()))
							{
								b->CPos -= 1;
								if (b->NextBox != 0)
								{
									if (b->NextBox->itemText.count() != 0)
									{
										view->Deselect(true);
										b->NextBox->CPos = 0;
										view->SelectItemNr(b->NextBox->ItemNr);
										b = b->NextBox;
									}
								}
							}
						}
						if ( b->HasSel )
							view->RefreshItem(b);
						setTBvals(b);
						break;
					case Key_Delete:
						if (b->CPos == static_cast<int>(b->itemText.count()))
						{
							if (b->HasSel)
							{
								DeleteSel(b);
								setTBvals(b);
								view->RefreshItem(b);
							}
							keyrep = false;
							return;
						}
						if (b->itemText.count() == 0)
						{
							keyrep = false;
							return;
						}
						cr = b->itemText.at(b->CPos)->ch;
						if (b->HasSel)
							DeleteSel(b);
						else
							b->itemText.remove(b->CPos);
						b->Tinput = false;
						if ((cr == QChar(13)) && (b->itemText.count() != 0))
						{
							view->chAbStyle(b, b->itemText.at(QMAX(b->CPos-1,0))->cab);
							b->Tinput = false;
						}
						setTBvals(b);
						view->RefreshItem(b);
						break;
					case Key_Backspace:
						if (b->CPos == 0)
						{
							if (b->HasSel)
							{
								DeleteSel(b);
								setTBvals(b);
								view->RefreshItem(b);
							}
							keyrep = false;
							return;
						}
						if (b->itemText.count() == 0)
						{
							keyrep = false;
							return;
						}
						cr = b->itemText.at(QMAX(b->CPos-1,0))->ch;
						if (b->HasSel)
							DeleteSel(b);
						else
						{
							b->CPos -= 1;
							b->itemText.remove(b->CPos);
						}
						b->Tinput = false;
						if ((cr == QChar(13)) && (b->itemText.count() != 0))
						{
							view->chAbStyle(b, b->itemText.at(QMAX(b->CPos-1,0))->cab);
							b->Tinput = false;
						}
						setTBvals(b);
						view->RefreshItem(b);
						break;
					default:
						if ((b->HasSel) && (kk < 0x1000))
							DeleteSel(b);
						if ((kk == Key_Tab)
						        || ((kk == Key_Return) && (buttonState & ShiftButton))
						        || ((kk + KeyMod) == Prefs.KeyActions[60].KeyID)
						        || ((kk + KeyMod) == Prefs.KeyActions[67].KeyID))
						{
							hg = new ScText;
							if ((kk + KeyMod) == Prefs.KeyActions[60].KeyID)
								hg->ch = QString(QChar(30));
							else if (kk == Key_Return)
								hg->ch = QString(QChar(28));
							else if (kk == Key_Tab)
								hg->ch = QString(QChar(9));
							else
								hg->ch = QString(QChar(29));
							hg->cfont = doc->CurrFont;
							hg->csize = doc->CurrFontSize;
							hg->ccolor = doc->CurrTextFill;
							hg->cshade = doc->CurrTextFillSh;
							hg->cstroke = doc->CurrTextStroke;
							hg->cshade2 = doc->CurrTextStrokeSh;
							hg->cscale = doc->CurrTextScale;
							hg->cselect = false;
							hg->cstyle = doc->CurrentStyle;
							hg->cab = doc->currentParaStyle;
							if (doc->docParagraphStyles[doc->currentParaStyle].Font != "")
							{
								hg->cfont = doc->docParagraphStyles[doc->currentParaStyle].Font;
								hg->csize = doc->docParagraphStyles[doc->currentParaStyle].FontSize;
							}
							hg->cextra = 0;
							hg->xp = 0;
							hg->yp = 0;
							hg->PRot = 0;
							hg->PtransX = 0;
							hg->PtransY = 0;
							b->itemText.insert(b->CPos, hg);
							b->CPos += 1;
							b->Tinput = true;
							view->RefreshItem(b);
							break;
						}
						if ((kk + KeyMod) == Prefs.KeyActions[56].KeyID)
						{
							b->itemText.at(QMAX(b->CPos-1,0))->cstyle ^= 128;
							b->Tinput = true;
							view->RefreshItem(b);
							break;
						}
						if ((kk + KeyMod) == Prefs.KeyActions[59].KeyID)
						{
							setNewAbStyle(1);
							b->Tinput = true;
							view->RefreshItem(b);
							break;
						}
						if ((kk + KeyMod) == Prefs.KeyActions[57].KeyID)
						{
							setNewAbStyle(0);
							b->Tinput = true;
							view->RefreshItem(b);
							break;
						}
						if ((kk + KeyMod) == Prefs.KeyActions[58].KeyID)
						{
							setNewAbStyle(2);
							b->Tinput = true;
							view->RefreshItem(b);
							break;
						}
						if (((uc[0] > QChar(31)) || (as == 13) || (as == 30)) && ((*doc->AllFonts)[doc->CurrFont]->CharWidth.contains(uc[0].unicode())))
						{
							hg = new ScText;
							hg->ch = uc;
							hg->cfont = doc->CurrFont;
							hg->ccolor = doc->CurrTextFill;
							hg->cshade = doc->CurrTextFillSh;
							hg->cstroke = doc->CurrTextStroke;
							hg->cshade2 = doc->CurrTextStrokeSh;
							hg->cscale = doc->CurrTextScale;
							hg->csize = doc->CurrFontSize;
							hg->cextra = 0;
							hg->cselect = false;
							hg->cstyle = doc->CurrentStyle;
							hg->cab = doc->currentParaStyle;
							if (doc->docParagraphStyles[doc->currentParaStyle].Font != "")
							{
								hg->cfont = doc->docParagraphStyles[doc->currentParaStyle].Font;
								hg->csize = doc->docParagraphStyles[doc->currentParaStyle].FontSize;
							}
							hg->xp = 0;
							hg->yp = 0;
							hg->PRot = 0;
							hg->PtransX = 0;
							hg->PtransY = 0;
							b->itemText.insert(b->CPos, hg);
							b->CPos += 1;
							if ((doc->docHyphenator->AutoCheck) && (b->CPos > 1))
							{
								Twort = "";
								Tcoun = 0;
								for (int hych = b->CPos-1; hych > -1; hych--)
								{
									Tcha = b->itemText.at(hych)->ch;
									if (Tcha == " ")
									{
										Tcoun = hych+1;
										break;
									}
									Twort.prepend(Tcha);
								}
								if (Twort != "")
								{
									if (doc->docHyphenator->Language != b->Language)
										doc->docHyphenator->slotNewDict(b->Language);
									doc->docHyphenator->slotHyphenateWord(b, Twort, Tcoun);
								}
							}
							b->Tinput = true;
							view->RefreshItem(b);
						}
						break;
					}
					if (b->itemText.count() != 0)
						if (b->itemText.at(QMAX(b->CPos-1, 0))->yp != 0)
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
	keyrep = false;
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
		SavePrefs();
		delete prefsFile;
		UndoManager::deleteInstance();
		if ((Prefs.SaveAtQ) && (ScBook->Changed == true))
		{
			if (ScBook->ScFilename.isEmpty())
				ScBook->ScFilename = PrefsPfad+"/scrap13.scs";
			ScBook->Save();
		}
		if (ScBook->BibWin->Objekte.count() == 0)
			unlink(PrefsPfad+"/scrap13.scs");
		Prefs.AvailFonts.~SCFonts();
		FinalizePlugs();
		exit(0);
	}
	else
	{
		SavePrefs();
		delete prefsFile;
		UndoManager::deleteInstance();
		if ((Prefs.SaveAtQ) && (ScBook->Changed == true))
		{
			if (ScBook->ScFilename.isEmpty())
				ScBook->ScFilename = PrefsPfad+"/scrap13.scs";
			ScBook->Save();
		}
		if (ScBook->BibWin->Objekte.count() == 0)
			unlink(PrefsPfad+"/scrap13.scs");
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		Prefs.AvailFonts.~SCFonts();
		FinalizePlugs();
		exit(0);
	}
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
	} while (tmp != "");
}

bool ScribusApp::slotFileNew()
{
	bool retVal;
	NewDoc* dia = new NewDoc(this, &Prefs);
	if (dia->exec())
	{
		bool facingPages, autoframes;
		double pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, numberCols, columnDistance;
		topMargin = dia->Top;
		leftMargin = dia->Left;
		rightMargin = dia->Right;
		bottomMargin = dia->Bottom;
		columnDistance = dia->Dist;
		pageWidth = dia->Pagebr;
		pageHeight = dia->Pageho;
		numberCols = dia->SpinBox10->value();
		autoframes = dia->AutoFrame->isChecked();
		facingPages = dia->Doppelseiten->isChecked();
		int orientation = dia->Orient;
		QString pagesize = dia->ComboBox1->currentText();
		retVal = doFileNew(pageWidth, pageHeight, topMargin, leftMargin, rightMargin, bottomMargin, columnDistance, numberCols, autoframes, facingPages, dia->ComboBox3->currentItem(),
		                dia->ErsteSeite->isChecked(), orientation, dia->PgNr->value(), pagesize);
		FMess->setText( tr("Ready"));
	}
	else
		retVal = false;
	delete dia;
	return retVal;
}

bool ScribusApp::doFileNew(double b, double h, double tpr, double lr, double rr, double br, double ab, double sp,
                           bool atf, bool fp, int einh, bool firstleft, int Ori, int SNr, QString PageSize)
{
	QString cc;
	if (HaveDoc)
		doc->OpenNodes = Tpal->buildReopenVals();
	doc = new ScribusDoc(&Prefs);
	doc->docUnitIndex = einh;
	if (fp)
		doc->FirstPageLeft = firstleft;
	doc->PageOri = Ori;
	doc->PageSize = PageSize;
	doc->FirstPnum = SNr;
	doc->DocName = doc->DocName+cc.setNum(DocNr);
	doc->HasCMS = true;
	doc->CMSSettings.DefaultInputProfile = Prefs.DCMSset.DefaultInputProfile;
	doc->CMSSettings.DefaultInputProfile2 = Prefs.DCMSset.DefaultInputProfile2;
	doc->CMSSettings.DefaultMonitorProfile = Prefs.DCMSset.DefaultMonitorProfile;
	doc->CMSSettings.DefaultPrinterProfile = Prefs.DCMSset.DefaultPrinterProfile;
	doc->CMSSettings.DefaultIntentPrinter = Prefs.DCMSset.DefaultIntentPrinter;
	doc->CMSSettings.DefaultIntentMonitor = Prefs.DCMSset.DefaultIntentMonitor;
	doc->CMSSettings.DefaultIntentMonitor2 = Prefs.DCMSset.DefaultIntentMonitor2;
	doc->CMSSettings.SoftProofOn = Prefs.DCMSset.SoftProofOn;
	doc->CMSSettings.GamutCheck = Prefs.DCMSset.GamutCheck;
	doc->CMSSettings.BlackPoint = Prefs.DCMSset.BlackPoint;
	doc->CMSSettings.CMSinUse = Prefs.DCMSset.CMSinUse;
	doc->PDF_Optionen.SolidProf = doc->CMSSettings.DefaultInputProfile2;
	doc->PDF_Optionen.ImageProf = doc->CMSSettings.DefaultInputProfile;
	doc->PDF_Optionen.PrintProf = doc->CMSSettings.DefaultPrinterProfile;
	doc->PDF_Optionen.Intent = doc->CMSSettings.DefaultIntentMonitor;
	doc->PDF_Optionen.Intent2 = doc->CMSSettings.DefaultIntentMonitor2;
	struct LPIData lpo;
	lpo.Frequency = 75;
	lpo.SpotFunc = 2;
	lpo.Angle = 105;
	doc->PDF_Optionen.LPISettings.insert("Cyan", lpo);
	lpo.Angle = 75;
	doc->PDF_Optionen.LPISettings.insert("Magenta", lpo);
	lpo.Angle = 90;
	doc->PDF_Optionen.LPISettings.insert("Yellow", lpo);
	lpo.Angle = 45;
	doc->PDF_Optionen.LPISettings.insert("Black", lpo);
	doc->ActiveLayer = 0;
	HaveDoc++;
	DocNr++;
	doc->appMode = NormalMode;
	doc->PageColors = Prefs.DColors;
	doc->loading = true;
	ScribusWin* w = new ScribusWin(wsp, doc);
	view = new ScribusView(w, doc, &Prefs);
	view->Scale = 1.0*Prefs.DisScale;
	w->setView(view);
	ActWin = w;
	doc->WinHan = w;
	w->setCentralWidget(view);
	connect(UndoManager::instance(), SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
	connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
	//	connect(w, SIGNAL(SaveAndClose()), this, SLOT(DoSaveClose()));
	if (CMSavail)
	{
#ifdef HAVE_CMS
		w->SoftProofing = Prefs.DCMSset.SoftProofOn;
		w->Gamut = Prefs.DCMSset.GamutCheck;
		CMSuse = Prefs.DCMSset.CMSinUse;
		w->IntentPrinter = Prefs.DCMSset.DefaultIntentPrinter;
		w->IntentMonitor = Prefs.DCMSset.DefaultIntentMonitor;
		SoftProofing = Prefs.DCMSset.SoftProofOn;
		Gamut = Prefs.DCMSset.GamutCheck;
		IntentPrinter = Prefs.DCMSset.DefaultIntentPrinter;
		IntentMonitor = Prefs.DCMSset.DefaultIntentMonitor;
		w->OpenCMSProfiles(InputProfiles, MonitorProfiles, PrinterProfiles);
		stdProof = w->stdProof;
		stdTrans = w->stdTrans;
		stdProofImg = w->stdProofImg;
		stdTransImg = w->stdTransImg;
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
		doc->PDF_Optionen.SComp = doc->CMSSettings.ComponentsInput2;
#endif
		if (Prefs.DCMSset.CMSinUse)
			RecalcColors();
	}
	doc->setPage(b, h, tpr, lr, rr, br, sp, ab, atf, fp);
	doc->loading = false;
	slotNewPage(0);
	doc->loading = true;
	HaveNewDoc();
	doc->DocPages = doc->Pages;
	doc->Pages = doc->MasterPages;
	doc->PageC = doc->MasterPages.count();
	bool atfb = doc->PageAT;
	doc->PageAT = false;
	slotNewPage(0);
	doc->PageAT = atfb;
	doc->MasterNames["Normal"] = 0;
	doc->Pages.at(0)->PageNam = "Normal";
	doc->MasterPages = doc->Pages;
	doc->PageC = doc->DocPages.count();
	doc->Pages = doc->DocPages;
	doc->MasterP = false;
	doc->Pages.at(0)->MPageNam = "Normal";
	doc->setUnModified();
	doc->loading = false;
	doc->DocItems = doc->Items;
	doc->currentPage = doc->Pages.at(0);
	doc->OpenNodes.clear();
//	Tpal->BuildTree(view);
//	Sepal->Rebuild();
	BookPal->BView->clear();
	if ( wsp->windowList().isEmpty() )
		w->showMaximized();
	else
		w->show();
	view->show();
	connect(doc->ASaveTimer, SIGNAL(timeout()), w, SLOT(slotAutoSave()));
	connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	connect(fileWatcher, SIGNAL(fileChanged(QString )), view, SLOT(updatePict(QString)));
	connect(fileWatcher, SIGNAL(fileDeleted(QString )), view, SLOT(removePict(QString)));
	doc->AutoSave = Prefs.AutoSave;
	doc->AutoSaveTime = Prefs.AutoSaveTime;
	if (doc->AutoSave)
		doc->ASaveTimer->start(Prefs.AutoSaveTime);
	DatSav->setEnabled(false);
	fileMenu->setItemEnabled(fid4, 0);
	UndoManager::instance()->switchStack(doc->DocName);

	return true;
}

bool ScribusApp::DoSaveClose()
{
	return slotFileSave();
}

void ScribusApp::windowsMenuAboutToShow()
{
	windowsMenu->clear();
	int cascadeId = windowsMenu->insertItem( tr("&Cascade"), wsp, SLOT(cascade() ) );
	int tileId = windowsMenu->insertItem( tr("&Tile"), wsp, SLOT(tile() ) );
	if ( wsp->windowList().isEmpty() )
	{
		windowsMenu->setItemEnabled( cascadeId, false );
		windowsMenu->setItemEnabled( tileId, false );
	}
	windowsMenu->insertSeparator();
	QWidgetList windows = wsp->windowList();
	for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
	{
		int id = windowsMenu->insertItem(windows.at(i)->caption(), this, SLOT(windowsMenuActivated(int)));
		windowsMenu->setItemParameter( id, i );
		windowsMenu->setItemChecked( id, wsp->activeWindow() == windows.at(i) );
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
			doc->OpenNodes = Tpal->buildReopenVals();
	} */
	QString newDocName = "";
	if (ActWin && ActWin->doc)
		newDocName = ActWin->doc->DocName;
	if (oldDocName != newDocName)
		UndoManager::instance()->switchStack(newDocName);

	doc = ActWin->doc;
	view = ActWin->view;
	Sepal->SetView(view);
	ScribusWin* swin;
	if (!doc->loading)
	{
		SwitchWin();
		QWidgetList windows = wsp->windowList();
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
		{
			swin = (ScribusWin*)windows.at(i);
			if (swin->muster != NULL)
				swin->muster->hide();
		}
		if (doc->TemplateMode)
			ActWin->muster->show();
		setAppMode(doc->appMode);
	}
	w->setFocus();
	if (w->isMaximized())
		wsp->setScrollBarsEnabled(false);
	else
		wsp->setScrollBarsEnabled(true);
	viewMenu->setItemChecked(Markers, doc->guidesSettings.marginsShown);
	viewMenu->setItemChecked(FrameDr, doc->guidesSettings.framesShown);
	viewMenu->setItemChecked(Ras, doc->guidesSettings.gridShown);
	viewMenu->setItemChecked(Guide, doc->guidesSettings.guidesShown);
	viewMenu->setItemChecked(Base, doc->guidesSettings.baseShown);
	viewMenu->setItemChecked(Bilder, doc->guidesSettings.showPic);
	viewMenu->setItemChecked(textLinks, doc->guidesSettings.linkShown);
//	if (!doc->TemplateMode)
//		Sepal->Rebuild();
//	Tpal->BuildTree(view);
//	Tpal->reopenTree(doc->OpenNodes);
	BookPal->BView->NrItems = ActWin->NrItems;
	BookPal->BView->First = ActWin->First;
	BookPal->BView->Last = ActWin->Last;
	RestoreBookMarks();
	if (!doc->loading)
	{
		if (view->SelItem.count() != 0)
		{
			HaveNewSel(view->SelItem.at(0)->PType);
			view->EmitValues(view->SelItem.at(0));
		}
		else
			HaveNewSel(-1);
	}
}

void ScribusApp::windowsMenuActivated( int id )
{
	if (HaveDoc)
		doc->OpenNodes = Tpal->buildReopenVals();
	QWidget* w = wsp->windowList().at( id );
	if ( w )
		w->showNormal();
	newActWin(w);
}

bool ScribusApp::SetupDoc()
{
	bool fp = doc->PageFP;
	double tpr2, lr2, rr2, br2;
	bool ret = false;
	ReformDoc* dia = new ReformDoc(this, doc, &Prefs);
	if (dia->exec())
	{
		slotChangeUnit(dia->unitCombo->currentItem(), false);
		tpr2 = dia->topR->value() / UmReFaktor;
		lr2 = dia->leftR->value() / UmReFaktor;
		rr2 = dia->rightR->value() / UmReFaktor;
		br2 = dia->bottomR->value() / UmReFaktor;
		fp = dia->facingPages->isChecked();
		if (fp)
			doc->FirstPageLeft = dia->firstPage->isChecked();
		doc->FirstPnum = dia->pageNumber->value();
		doc->resetPage(tpr2, lr2, rr2, br2, fp);
		doc->Before = dia->inBackground->isChecked();
		doc->marginColored = dia->checkUnprintable->isChecked();
		doc->papColor = dia->colorPaper;
		doc->guidesSettings.marginsShown = dia->tabGuides->checkMargin->isChecked();
		doc->guidesSettings.framesShown = dia->tabGuides->checkFrame->isChecked();
		doc->guidesSettings.gridShown = dia->tabGuides->checkGrid->isChecked();
		doc->guidesSettings.guidesShown = dia->tabGuides->checkGuides->isChecked();
		doc->guidesSettings.baseShown = dia->tabGuides->checkBaseline->isChecked();
		doc->guidesSettings.showPic = dia->tabGuides->checkPictures->isChecked();
		doc->guidesSettings.linkShown = dia->tabGuides->checkLink->isChecked();
		doc->guidesSettings.grabRad = dia->tabGuides->grabDistance->value();
		doc->guidesSettings.guideRad = dia->tabGuides->snapDistance->value() / UmReFaktor;
		doc->guidesSettings.minorGrid = dia->tabGuides->minorSpace->value() / UmReFaktor;
		doc->guidesSettings.majorGrid = dia->tabGuides->majorSpace->value() / UmReFaktor;
		doc->guidesSettings.minorColor = dia->tabGuides->colorMinorGrid;
		doc->guidesSettings.majorColor = dia->tabGuides->colorMajorGrid;
		doc->guidesSettings.margColor = dia->tabGuides->colorMargin;
		doc->guidesSettings.guideColor = dia->tabGuides->colorGuides;
		doc->guidesSettings.baseColor = dia->tabGuides->colorBaselineGrid;
		doc->typographicSetttings.valueSuperScript = dia->tabTypo->superDisplacement->value();
		doc->typographicSetttings.scalingSuperScript = dia->tabTypo->superScaling->value();
		doc->typographicSetttings.valueSubScript = dia->tabTypo->subDisplacement->value();
		doc->typographicSetttings.scalingSubScript = dia->tabTypo->subScaling->value();
		doc->typographicSetttings.valueSmallCaps = dia->tabTypo->capsScaling->value();
		doc->typographicSetttings.autoLineSpacing = dia->tabTypo->autoLine->value();
		doc->typographicSetttings.valueBaseGrid = dia->tabTypo->baseGrid->value() / UmReFaktor;
		doc->typographicSetttings.offsetBaseGrid = dia->tabTypo->baseOffset->value() / UmReFaktor;
		doc->Dfont = dia->fontComboText->currentText();
		doc->Dsize = dia->sizeComboText->currentText().left(2).toInt() * 10;
		doc->DstrokeText = dia->colorComboStrokeText->currentText();
		if (doc->DstrokeText == tr("None"))
			doc->DstrokeText = "None";
		doc->DpenText = dia->colorComboText->currentText();
		if (doc->DpenText == tr("None"))
			doc->DpenText = "None";
		doc->DCols = dia->columnsText->value();
		doc->DGap = dia->gapText->value() / UmReFaktor;
		doc->Dpen = dia->colorComboLineShape->currentText();
		if (doc->Dpen == tr("None"))
			doc->Dpen = "None";
		doc->Dbrush = dia->comboFillShape->currentText();
		if (doc->Dbrush == tr("None"))
			doc->Dbrush = "None";
		doc->Dshade = dia->shadingFillShape->value();
		doc->Dshade2 = dia->shadingLineShape->value();
		switch (dia->comboStyleShape->currentItem())
		{
		case 0:
			doc->DLineArt = SolidLine;
			break;
		case 1:
			doc->DLineArt = DashLine;
			break;
		case 2:
			doc->DLineArt = DotLine;
			break;
		case 3:
			doc->DLineArt = DashDotLine;
			break;
		case 4:
			doc->DLineArt = DashDotDotLine;
			break;
		}
		doc->Dwidth = dia->lineWidthShape->value();
		doc->DstartArrow = dia->startArrow->currentItem();
		doc->DendArrow = dia->endArrow->currentItem();
		doc->MagMin = dia->minimumZoom->value();
		doc->MagMax = dia->maximumZoom->value();
		doc->MagStep = dia->zoomStep->value();
		doc->DpenLine = dia->colorComboLine->currentText();
		if (doc->DpenLine == tr("None"))
			doc->DpenLine = "None";
		doc->DshadeLine = dia->shadingLine->value();
		switch (dia->comboStyleLine->currentItem())
		{
		case 0:
			doc->DLstyleLine = SolidLine;
			break;
		case 1:
			doc->DLstyleLine = DashLine;
			break;
		case 2:
			doc->DLstyleLine = DotLine;
			break;
		case 3:
			doc->DLstyleLine = DashDotLine;
			break;
		case 4:
			doc->DLstyleLine = DashDotDotLine;
			break;
		}
		doc->DwidthLine = dia->lineWidthLine->value();
		doc->DbrushPict = dia->comboFillImage->currentText();
		if (doc->DbrushPict == tr("None"))
			doc->DbrushPict = "None";
		doc->ShadePict = dia->shadingFillImage->value();
		doc->ScaleX = static_cast<double>(dia->scalingHorizontal->value()) / 100.0;
		doc->ScaleY = static_cast<double>(dia->scalingVertical->value()) / 100.0;
		doc->ScaleType = dia->buttonGroup3->isChecked();
		doc->AspectRatio = dia->checkRatioImage->isChecked();
		dia->polyWidget->getValues(&doc->PolyC, &doc->PolyFd, &doc->PolyF, &doc->PolyS, &doc->PolyR);
		doc->ScratchBottom = dia->bottomScratch->value() / UmReFaktor;
		doc->ScratchLeft = dia->leftScratch->value() / UmReFaktor;
		doc->ScratchRight = dia->rightScratch->value() / UmReFaktor;
		doc->ScratchTop = dia->topScratch->value() / UmReFaktor;
		doc->AutoSave = dia->groupAutoSave->isChecked();
		doc->AutoSaveTime = dia->autoSaveTime->value() * 60 * 1000;
		if (doc->AutoSave)
		{
			doc->ASaveTimer->stop();
			doc->ASaveTimer->start(doc->AutoSaveTime);
		}
		doc->docHyphenator->slotNewDict(dia->tabHyphenator->language->currentText());
		doc->docHyphenator->slotNewSettings(dia->tabHyphenator->wordLen->value(), 
																	!dia->tabHyphenator->verbose->isChecked(), 
																	dia->tabHyphenator->input->isChecked(),
																	dia->tabHyphenator->maxCount->value());
		if (CMSavail)
		{
			dia->tabColorManagement->setValues();
			if (dia->tabColorManagement->changed)
			{
				int cc = Prefs.DColors.count();
				FMess->setText( tr("Adjusting Colors"));
				FProg->reset();
				cc = doc->PageColors.count() + view->CountElements();
				FProg->setTotalSteps(cc);
#ifdef HAVE_CMS
				doc->HasCMS = doc->CMSSettings.CMSinUse;
				ActWin->SoftProofing = doc->CMSSettings.SoftProofOn;
				ActWin->Gamut = doc->CMSSettings.GamutCheck;
				CMSuse = doc->CMSSettings.CMSinUse;
				ActWin->IntentPrinter = doc->CMSSettings.DefaultIntentPrinter;
				ActWin->IntentMonitor = doc->CMSSettings.DefaultIntentMonitor;
				SoftProofing = doc->CMSSettings.SoftProofOn;
				Gamut = doc->CMSSettings.GamutCheck;
				IntentPrinter = doc->CMSSettings.DefaultIntentPrinter;
				IntentMonitor = doc->CMSSettings.DefaultIntentMonitor;
				qApp->setOverrideCursor(QCursor(waitCursor), true);
				ActWin->CloseCMSProfiles();
				ActWin->OpenCMSProfiles(InputProfiles, MonitorProfiles, PrinterProfiles);
				stdProof = ActWin->stdProof;
				stdTrans = ActWin->stdTrans;
				stdProofImg = ActWin->stdProofImg;
				stdTransImg = ActWin->stdTransImg;
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
				doc->PDF_Optionen.SComp = doc->CMSSettings.ComponentsInput2;
				doc->PDF_Optionen.SolidProf = doc->CMSSettings.DefaultInputProfile2;
				doc->PDF_Optionen.ImageProf = doc->CMSSettings.DefaultInputProfile;
				doc->PDF_Optionen.PrintProf = doc->CMSSettings.DefaultPrinterProfile;
				doc->PDF_Optionen.Intent = doc->CMSSettings.DefaultIntentMonitor;
				RecalcColors(FProg);
				view->RecalcPictures(&InputProfiles, FProg);
#endif
				FProg->setProgress(cc);
				qApp->setOverrideCursor(QCursor(arrowCursor), true);
				FMess->setText("");
				FProg->reset();
			}
		}
		viewMenu->setItemChecked(Markers, doc->guidesSettings.marginsShown);
		viewMenu->setItemChecked(FrameDr, doc->guidesSettings.framesShown);
		viewMenu->setItemChecked(Ras, doc->guidesSettings.gridShown);
		viewMenu->setItemChecked(Guide, doc->guidesSettings.guidesShown);
		viewMenu->setItemChecked(Base, doc->guidesSettings.baseShown);
		viewMenu->setItemChecked(Bilder, doc->guidesSettings.showPic);
		viewMenu->setItemChecked(textLinks, doc->guidesSettings.linkShown);
		for (uint b=0; b<doc->Items.count(); ++b)
		{
			if (doc->Items.at(b)->PType == 2)
				doc->Items.at(b)->PicArt = doc->guidesSettings.showPic;
		}
		view->reformPages();
		view->GotoPage(doc->currentPage->PageNr);
		view->DrawNew();
		Mpal->ShowCMS();
//		Sepal->RebuildPage();
		slotDocCh();
		ret = true;
		doc->PDF_Optionen.BleedBottom = doc->PageM.Bottom;
		doc->PDF_Optionen.BleedTop = doc->PageM.Top;
		doc->PDF_Optionen.BleedLeft = doc->PageM.Left;
		doc->PDF_Optionen.BleedRight = doc->PageM.Right;
	}
	delete dia;
	return ret;
}

void ScribusApp::SwitchWin()
{
	int a;
	ColorList::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	a = 0;
	ColorMenC->clear();
	ColorMenC->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		ColorMenC->insertItem(pm, it.key());
		if (it.key() == doc->Dbrush)
			ColorMenC->setCurrentItem(a);
		a++;
	}
	buildFontMenu();
#ifdef HAVE_CMS
	SoftProofing = ActWin->SoftProofing;
	Gamut = ActWin->Gamut;
	IntentPrinter = ActWin->IntentPrinter;
	IntentMonitor = ActWin->IntentMonitor;
	stdProof = ActWin->stdProof;
	stdTrans = ActWin->stdTrans;
	stdProofImg = ActWin->stdProofImg;
	stdTransImg = ActWin->stdTransImg;
	CMSoutputProf = doc->DocOutputProf;
	CMSprinterProf = doc->DocPrinterProf;
#endif
	Mpal->Cpal->SetColors(doc->PageColors);
	Mpal->Cpal->ChooseGrad(0);
	ActWin->setCaption(doc->DocName);
	ShadeMenu->setItemChecked(ShadeMenu->idAt(11), true);
	Mpal->SetDoc(doc);
	Mpal->updateCList();
	Sepal->SetView(view);
	Mpal->Spal->setFormats(doc);
	Mpal->SetLineFormats(doc);
	Mpal->startArrow->rebuildList(&doc->arrowStyles);
	Mpal->endArrow->rebuildList(&doc->arrowStyles);
	FontSub->RebuildList(&Prefs, doc);
	Mpal->Fonts->RebuildList(&Prefs, doc);
	Lpal->setLayers(&doc->Layers, &doc->ActiveLayer);
	view->LaMenu();
	view->setLayMenTxt(doc->ActiveLayer);
	doc->currentParaStyle = 0;
	slotChangeUnit(doc->docUnitIndex, false);
	if (doc->EditClip)
	{
		doc->EditClip = !doc->EditClip;
		ToggleFrameEdit();
	}
	DatClo->setEnabled(true);
	if (doc->TemplateMode)
	{
		for (uint a=0; a<5; ++a)
		{
			pageMenu->setItemEnabled(pageMenu->idAt(a), 0);
		}
		editMenu->setItemEnabled(tman, 0);
		DatNeu->setEnabled(false);
		DatSav->setEnabled(doc->isModified());
		DatOpe->setEnabled(false);
		DatClo->setEnabled(false);
		fileMenu->setItemEnabled(fid1, 0);
		fileMenu->setItemEnabled(fid4, doc->isModified());
		fileMenu->setItemEnabled(fid52, 0);
		fileMenu->setItemEnabled(M_NewFile, 0);
		fileMenu->setItemEnabled(fid13, 0);
		fileMenu->setItemEnabled(fid14, 0);
		Sepal->DisablePal();
	}
	else
	{
		menuBar()->setItemEnabled(pgmm, 1);
		editMenu->setItemEnabled(tman, 1);
		DatNeu->setEnabled(true);
		DatOpe->setEnabled(true);
		DatClo->setEnabled(true);
		DatSav->setEnabled(doc->isModified());
		fileMenu->setItemEnabled(fid1, 1);
		fileMenu->setItemEnabled(fid4, ActWin->MenuStat[2]);
		fileMenu->setItemEnabled(M_SaveAs, ActWin->MenuStat[3]);
		fileMenu->setItemEnabled(fid51, ActWin->MenuStat[3]);
		fileMenu->setItemEnabled(fid52, 0);
		fileMenu->setItemEnabled(M_NewFile, 1);
		fileMenu->setItemEnabled(fid13, 1);
		fileMenu->setItemEnabled(fid14, 1);
		if (doc->Pages.count() > 1)
		{
			pageMenu->setItemEnabled(pgmd, 1);
			pageMenu->setItemEnabled(pgmv, 1);
		}
		else
		{
			pageMenu->setItemEnabled(pgmd, 0);
			pageMenu->setItemEnabled(pgmv, 0);
		}
		if (doc->isModified())
			slotDocCh(false);
		fileMenu->setItemEnabled(M_SaveAs, 1);
		fileMenu->setItemEnabled(fid51, 1);
		Sepal->EnablePal();
	}
}

void ScribusApp::HaveNewDoc()
{
	int a;
	DatPri->setEnabled(true);
	DatPDF->setEnabled(true);
	DatSav->setEnabled(false);
	QValueList<int>::iterator itm;
	for (itm = MenuItemsFile.begin(); itm != MenuItemsFile.end(); ++itm )
	{
		fileMenu->setItemEnabled((*itm), 1);
	}
	fileMenu->setItemEnabled(M_SaveAs, 1);
	fileMenu->setItemEnabled(fid11, 1);
	fileMenu->setItemEnabled(fid4, 0);
	fileMenu->setItemEnabled(fid52, 0);
	exportMenu->setItemEnabled(fid8, 1);
	importMenu->setItemEnabled(fid2a, 1);
	exportMenu->setItemEnabled(fid10, 1);
	editMenu->setItemEnabled(edid1, 0);
	editMenu->setItemEnabled(edid2, 0);
	if (Buffer2 != "")
		editMenu->setItemEnabled(edid3, 1);
	else
		editMenu->setItemEnabled(edid3, 0);
	editMenu->setItemEnabled(edid5, 1);
	editMenu->setItemEnabled(edid6, 1);
	editMenu->setItemEnabled(edid6a, 1);
	menuBar()->setItemEnabled(ViMen, 1);
	menuBar()->setItemEnabled(WinMen, 1);
	viewMenu->setItemChecked(uGuide, doc->SnapGuides);
	viewMenu->setItemChecked(uRas, doc->useRaster);
	menuBar()->setItemEnabled(pgmm, 1);
	menuBar()->setItemEnabled(exmn, 1);
	WerkTools->setEnabled(true);
	WerkToolsP->setEnabled(true);
	int setter = 0;
	if (doc->Pages.count() > 1)
		setter = 1;
	pageMenu->setItemEnabled(pgmd, setter);
	pageMenu->setItemEnabled(pgmv, setter);
	editMenu->setItemEnabled(tman, 1);
	editMenu->setItemEnabled(jman, 1);
	ColorList::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	a = 0;
	ColorMenC->clear();
	ColorMenC->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		ColorMenC->insertItem(pm, it.key());
		if (it.key() == doc->Dbrush)
			ColorMenC->setCurrentItem(a);
		a++;
	}
	Mpal->Cpal->SetColors(doc->PageColors);
	Mpal->Cpal->ChooseGrad(0);
	ActWin->setCaption(doc->DocName);
	ShadeMenu->setItemChecked(ShadeMenu->idAt(11), true);
	Mpal->SetDoc(doc);
	Mpal->updateCList();
	Sepal->SetView(view);
	Mpal->Spal->setFormats(doc);
	Mpal->SetLineFormats(doc);
	Mpal->startArrow->rebuildList(&doc->arrowStyles);
	Mpal->endArrow->rebuildList(&doc->arrowStyles);
	Lpal->setLayers(&doc->Layers, &doc->ActiveLayer);
	view->LaMenu();
	view->setLayMenTxt(doc->ActiveLayer);
	doc->currentParaStyle = 0;
	slotChangeUnit(doc->docUnitIndex);
	doc->docHyphenator = new Hyphenator(this, doc, this);
	buildFontMenu();
	connect(view, SIGNAL(changeUN(int)), this, SLOT(slotChangeUnit(int)));
	connect(view, SIGNAL(changeLA(int)), Lpal, SLOT(MarkActiveLayer(int)));
	connect(view->HR, SIGNAL(MarkerMoved(double, double)), this, SLOT(ReportMP(double, double)));
	connect(view->HR, SIGNAL(DocChanged(bool)), this, SLOT(slotDocCh(bool)));
	connect(view, SIGNAL(ClipPo(double, double)), Npal, SLOT(SetXY(double, double)));
	connect(view, SIGNAL(PolyOpen()), Npal, SLOT(IsOpen()));
	connect(view, SIGNAL(PStatus(int, uint)), Npal, SLOT(PolyStatus(int, uint)));
	connect(view, SIGNAL(ItemPos(double, double)), Mpal, SLOT(setXY(double, double)));
	connect(view, SIGNAL(ItemGeom(double, double)), Mpal, SLOT(setBH(double, double)));
// 	connect(view, SIGNAL(UndoAvail()), this, SLOT(CanUndo()));
	connect(view, SIGNAL(ChBMText(PageItem *)), this, SLOT(BookMarkTxT(PageItem *)));
	connect(view, SIGNAL(NewBMNr(int, int)), BookPal->BView, SLOT(ChangeItem(int, int)));
	connect(view, SIGNAL(HaveSel(int)), this, SLOT(HaveNewSel(int)));
	connect(view, SIGNAL(SetAngle(double)), Mpal, SLOT(setR(double)));
	connect(view, SIGNAL(SetSizeValue(double)), Mpal, SLOT(setSvalue(double)));
	connect(view, SIGNAL(SetLocalValues(double, double, double, double)), Mpal, SLOT(setLvalue(double, double, double, double)));
	connect(view, SIGNAL(SetLineArt(PenStyle, PenCapStyle, PenJoinStyle)), Mpal, SLOT( setLIvalue(PenStyle, PenCapStyle, PenJoinStyle)));
	connect(view, SIGNAL(ItemFarben(QString, QString, int, int)), this, SLOT(setCSMenu(QString, QString, int, int)));
	connect(view, SIGNAL(ItemFarben(QString, QString, int, int)), Mpal->Cpal, SLOT(setActFarben(QString, QString, int, int)));
	connect(view, SIGNAL(ItemGradient(int)), Mpal->Cpal, SLOT(setActGradient(int)));
	connect(view, SIGNAL(ItemTrans(double, double)), Mpal->Cpal, SLOT(setActTrans(double, double)));
	connect(view, SIGNAL(ItemTextAttr(double)), Mpal, SLOT(setLsp(double)));
	connect(view, SIGNAL(ItemTextUSval(double)), Mpal, SLOT(setExtra(double)));
//	connect(view, SIGNAL(ItemTextCols(int, double)), Mpal, SLOT(setCols(int, double)));
	connect(view, SIGNAL(SetDistValues(double, double, double, double)), Mpal, SLOT(setDvals(double, double, double, double)));
	connect(view, SIGNAL(ItemTextAbs(int)), Mpal, SLOT(setAli(int)));
	connect(view, SIGNAL(ItemTextFont(QString)), this, SLOT(AdjustFontMenu(QString)));
	connect(view, SIGNAL(ItemTextSize(int)), Mpal, SLOT(setSize(int)));
	connect(view, SIGNAL(ItemRadius(double)), Mpal, SLOT(setRR(double)));
	connect(view, SIGNAL(Amode(int)), this, SLOT(setAppMode(int)));
	connect(view, SIGNAL(PaintingDone()), this, SLOT(slotSelect()));
/*	connect(doc->currentPage, SIGNAL(DocChanged()), this, SLOT(slotDocCh())); */
	connect(view, SIGNAL(HavePoint(bool, bool)), Npal, SLOT(HaveNode(bool, bool)));
	connect(view, SIGNAL(MousePos(double, double)), this, SLOT(ReportMP(double, double)));
	connect(view, SIGNAL(ItemRadius(double)), Mpal, SLOT(setRR(double)));
	connect(view, SIGNAL(ItemTextStil(int)), Mpal, SLOT(setStil(int)));
	connect(view, SIGNAL(ItemTextSca(int)), Mpal, SLOT(setTScale(int)));
	connect(view, SIGNAL(ItemTextSize(int)), this, SLOT(setFSizeMenu(int)));
	connect(view, SIGNAL(ItemTextStil(int)), this, SLOT(setStilvalue(int)));
	connect(view, SIGNAL(ItemTextAbs(int)), this, SLOT(setAbsValue(int)));
	connect(view, SIGNAL(ItemTextFarben(QString, QString, int, int)), Mpal, SLOT(setActFarben(QString, QString, int, int)));
	connect(view, SIGNAL(HasTextSel()), this, SLOT(EnableTxEdit()));
	connect(view, SIGNAL(HasNoTextSel()), this, SLOT(DisableTxEdit()));
	connect(view, SIGNAL(CopyItem()), this, SLOT(slotEditCopy()));
	connect(view, SIGNAL(CutItem()), this, SLOT(slotEditCut()));
	connect(view, SIGNAL(LoadPic()), this, SLOT(slotFileOpen()));
	connect(view, SIGNAL(AppendText()), this, SLOT(slotFileAppend()));
	connect(view, SIGNAL(AnnotProps()), this, SLOT(ModifyAnnot()));
	connect(view, SIGNAL(ToScrap(QString)), this, SLOT(PutScrap(QString)));
	connect(view, SIGNAL(EditGuides()), this, SLOT(ManageGuides()));
	connect(view, SIGNAL(LoadElem(QString, int ,int, bool, bool, ScribusDoc *, ScribusView*)), this, SLOT(slotElemRead(QString, int, int, bool, bool, ScribusDoc *, ScribusView*)));
/*	connect(doc->currentPage, SIGNAL(AddBM(PageItem *)), this, SLOT(AddBookMark(PageItem *)));
	connect(doc->currentPage, SIGNAL(DelBM(PageItem *)), this, SLOT(DelBookMark(PageItem *))); */
	connect(view, SIGNAL(RasterPic(bool)), this, SLOT(HaveRaster(bool)));
	connect(view, SIGNAL(EditText()), this, SLOT(slotStoryEditor()));
	connect(view, SIGNAL(DoGroup()), this, SLOT(GroupObj()));
	connect(view, SIGNAL(DoUnGroup()), this, SLOT(UnGroupObj()));
	connect(view, SIGNAL(EndNodeEdit()), this, SLOT(ToggleFrameEdit()));
	connect(view, SIGNAL(LevelChanged(uint )), Mpal, SLOT(setLevel(uint)));
	connect(view, SIGNAL(callGimp()), this, SLOT(CallGimp()));
/*	if (!doc->TemplateMode)
	{
		connect(doc->currentPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
		connect(doc->currentPage, SIGNAL(AddObj(uint, uint)), Tpal, SLOT(slotAddElement(uint, uint)));
		connect(doc->currentPage, SIGNAL(UpdtObj(uint, uint)), Tpal, SLOT(slotUpdateElement(uint, uint)));
		connect(doc->currentPage, SIGNAL(MoveObj(uint, uint, uint)), Tpal, SLOT(slotMoveElement(uint, uint, uint)));
	} */
	doc->PDF_Optionen.BleedBottom = doc->PageM.Bottom;
	doc->PDF_Optionen.BleedTop = doc->PageM.Top;
	doc->PDF_Optionen.BleedLeft = doc->PageM.Left;
	doc->PDF_Optionen.BleedRight = doc->PageM.Right;
	doc->CurTimer = 0;
}

void ScribusApp::HaveNewSel(int Nr)
{
	PageItem *b = NULL;
	if (Nr != -1)
		b = view->SelItem.at(0);
	ObjMenu->setItemEnabled(PfadDT, 0);
	if (PluginMap.contains(1))
		extraMenu->setItemEnabled(PluginMap[1].MenuID, 0);
	view->HR->ItemPosValid = false;
	view->HR->repX = false;
	view->HR->repaint();
	switch (Nr)
	{
	case -1:
		importMenu->changeItem(fid2, tr("Get Text/Picture..."));
		importMenu->setItemEnabled(fid2, 0);
		importMenu->setItemEnabled(fid2aa, 0);
		exportMenu->setItemEnabled(fid3, 0);
		menuBar()->setItemEnabled(Stm, 0);
		menuBar()->setItemEnabled(Obm, 0);
		ObjMenu->setItemEnabled(ShapeM, 0);
		ObjMenu->setItemEnabled(PfadTP, 0);
		ObjMenu->setItemEnabled(LockOb, 0);
		editMenu->setItemEnabled(edid1, 0);
		editMenu->setItemEnabled(edid2, 0);
		editMenu->setItemEnabled(edid4, 0);
		editMenu->setItemEnabled(Sear, 0);
		extraMenu->setItemEnabled(hyph, 0);
		StilMenu->clear();
		WerkTools->KetteAus->setEnabled(false);
		WerkTools->KetteEin->setEnabled(false);
		WerkTools->Textedit->setEnabled(false);
		WerkTools->Textedit2->setEnabled(false);
		WerkTools->Rotiere->setEnabled(false);
		Mpal->Cpal->gradientQCombo->setCurrentItem(0);
//		Tpal->slotShowSelect(doc->currentPage->PageNr, -1);
		break;
	case 2:
		importMenu->changeItem(fid2, tr("Get Picture..."));
		importMenu->setItemEnabled(fid2aa, 0);
		importMenu->setItemEnabled(fid2, 1);
		editMenu->setItemEnabled(edid1, 1);
		editMenu->setItemEnabled(edid2, 1);
		editMenu->setItemEnabled(edid4, 0);
		editMenu->setItemEnabled(Sear, 0);
		extraMenu->setItemEnabled(hyph, 0);
		menuBar()->setItemEnabled(Stm, 1);
		menuBar()->setItemEnabled(Obm, 1);
		if ((b->isTableItem) && (b->isSingleSel))
			ObjMenu->setItemEnabled(ShapeM, 0);
		else
			ObjMenu->setItemEnabled(ShapeM, 1);
		ObjMenu->setItemEnabled(PfadTP, 0);
		StilMenu->clear();
		StilMenu->insertItem( tr("&Color"), ColorMenu);
		if (b->isRaster)
			StilMenu->insertItem( tr("&Invert"), this, SLOT(InvertPict()));
		WerkTools->KetteAus->setEnabled(false);
		WerkTools->KetteEin->setEnabled(false);
		WerkTools->Textedit->setEnabled(b->ScaleType);
		WerkTools->Textedit2->setEnabled(false);
		WerkTools->Rotiere->setEnabled(true);
		break;
	case 4:
		importMenu->changeItem(fid2, tr("&Get Text..."));
		importMenu->setItemEnabled(fid2, 1);
		importMenu->setItemEnabled(fid2aa, 1);
		exportMenu->setItemEnabled(fid3, 1);
		editMenu->setItemEnabled(edid1, 1);
		editMenu->setItemEnabled(edid2, 1);
		editMenu->setItemEnabled(edid4, 0);
		if (b->itemText.count() == 0)
			editMenu->setItemEnabled(Sear, 0);
		else
			editMenu->setItemEnabled(Sear, 1);
		extraMenu->setItemEnabled(hyph, 1);
		menuBar()->setItemEnabled(Stm, 1);
		menuBar()->setItemEnabled(Obm, 1);
		if ((b->isTableItem) && (b->isSingleSel))
			ObjMenu->setItemEnabled(ShapeM, 0);
		else
			ObjMenu->setItemEnabled(ShapeM, 1);
		ObjMenu->setItemEnabled(PfadTP, 1);
		StilMenu->clear();
		StilMenu->insertItem( tr("&Font"), FontMenu);
		StilMenu->insertItem( tr("&Size"), SizeTMenu);
		StilMenu->insertItem( tr("&Effects"), TypeStyleMenu);
		StilMenu->insertItem( tr("&Alignment"), AliMenu);
		StilMenu->insertItem( tr("&Color"), ColorMenu);
		StilMenu->insertItem( tr("&Shade"), ShadeMenu);
		StilMenu->insertItem( tr("&Tabulators..."), this, SLOT(EditTabs()));
		WerkTools->Rotiere->setEnabled(true);
		WerkTools->Textedit2->setEnabled(true);
		if ((b->NextBox != 0) || (b->BackBox != 0))
		{
			WerkTools->KetteAus->setEnabled(true);
			if ((b->BackBox != 0) && (b->itemText.count() == 0))
				WerkTools->Textedit->setEnabled(false);
			else
				WerkTools->Textedit->setEnabled(true);
		}
		else
			WerkTools->Textedit->setEnabled(true);
		if (b->NextBox == 0)
			WerkTools->KetteEin->setEnabled(true);
		if (doc->MasterP)
			WerkTools->KetteEin->setEnabled(false);
		if (doc->appMode == EditMode)
		{
			setTBvals(b);
			editMenu->setItemEnabled(edid5, 1);
			if (PluginMap.contains(1))
				extraMenu->setItemEnabled(PluginMap[1].MenuID, 1);
			view->HR->ItemPos = b->Xpos;
			view->HR->ItemEndPos = b->Xpos+b->Width;
			if (b->Pcolor2 != "None")
				view->HR->lineCorr = b->Pwidth / 2.0;
			else
				view->HR->lineCorr = 0;
			view->HR->ColGap = b->ColGap;
			view->HR->Cols = b->Cols;
			view->HR->Extra = b->Extra;
			view->HR->RExtra = b->RExtra;
			view->HR->First = doc->docParagraphStyles[doc->currentParaStyle].First;
			view->HR->Indent = doc->docParagraphStyles[doc->currentParaStyle].Indent;
			if ((b->flippedH % 2 != 0) || (b->Reverse))
				view->HR->Revers = true;
			else
				view->HR->Revers = false;
			view->HR->ItemPosValid = true;
			view->HR->repX = false;
			if (doc->currentParaStyle < 5)
				view->HR->TabValues = b->TabValues;
			else
				view->HR->TabValues = doc->docParagraphStyles[doc->currentParaStyle].TabValues;
			view->HR->repaint();
		}
		else
		{
			doc->CurrFont = b->IFont;
			doc->CurrFontSize = b->ISize;
			doc->CurrTextFill = b->TxtFill;
			doc->CurrTextStroke = b->TxtStroke;
			doc->CurrTextStrokeSh = b->ShTxtStroke;
			doc->CurrTextFillSh = b->ShTxtFill;
			doc->CurrTextScale = b->TxtScale;
			emit TextFarben(doc->CurrTextStroke, doc->CurrTextFill, doc->CurrTextStrokeSh, doc->CurrTextFillSh);
			doc->CurrentStyle = b->TxTStyle;
			emit TextStil(doc->CurrentStyle);
			emit TextScale(doc->CurrTextScale);
			setStilvalue(doc->CurrentStyle);
		}
		doc->docParagraphStyles[0].LineSpa = b->LineSp;
		doc->docParagraphStyles[0].textAlignment = b->textAlignment;
		break;
	case 8:
		importMenu->changeItem(fid2, tr("Get Text..."));
		importMenu->setItemEnabled(fid2, 1);
		importMenu->setItemEnabled(fid2aa, 1);
		exportMenu->setItemEnabled(fid3, 1);
		editMenu->setItemEnabled(edid1, 1);
		editMenu->setItemEnabled(edid2, 1);
		editMenu->setItemEnabled(edid4, 0);
		editMenu->setItemEnabled(Sear, 0);
		extraMenu->setItemEnabled(hyph, 0);
		menuBar()->setItemEnabled(Stm, 1);
		menuBar()->setItemEnabled(Obm, 1);
		ObjMenu->setItemEnabled(ShapeM, 0);
		ObjMenu->setItemEnabled(PfadDT, 1);
		ObjMenu->setItemEnabled(PfadTP, 0);
		StilMenu->clear();
		StilMenu->insertItem( tr("Font"), FontMenu);
		StilMenu->insertItem( tr("Size"), SizeTMenu);
		StilMenu->insertItem( tr("Style"), TypeStyleMenu);
		StilMenu->insertItem( tr("Color"), ColorMenu);
		StilMenu->insertItem( tr("Shade"), ShadeMenu);
		WerkTools->Rotiere->setEnabled(true);
		WerkTools->Textedit->setEnabled(false);
		WerkTools->Textedit2->setEnabled(true);
		WerkTools->KetteEin->setEnabled(false);
		WerkTools->KetteAus->setEnabled(false);
		if (doc->appMode == EditMode)
			setTBvals(b);
		else
		{
			doc->CurrFont = b->IFont;
			doc->CurrFontSize = b->ISize;
			doc->CurrTextFill = b->TxtFill;
			doc->CurrTextStroke = b->TxtStroke;
			doc->CurrTextStrokeSh = b->ShTxtStroke;
			doc->CurrTextFillSh = b->ShTxtFill;
			doc->CurrTextScale = b->TxtScale;
			emit TextFarben(doc->CurrTextStroke, doc->CurrTextFill, doc->CurrTextStrokeSh, doc->CurrTextFillSh);
			doc->CurrentStyle = b->TxTStyle;
			emit TextStil(doc->CurrentStyle);
			emit TextScale(doc->CurrTextScale);
			setStilvalue(doc->CurrentStyle);
		}
		break;
	default:
		importMenu->changeItem(fid2, tr("Get Text/Picture..."));
		importMenu->setItemEnabled(fid2, 0);
		importMenu->setItemEnabled(fid2aa, 0);
		exportMenu->setItemEnabled(fid3, 0);
		editMenu->setItemEnabled(edid1, 1);
		editMenu->setItemEnabled(edid2, 1);
		editMenu->setItemEnabled(edid4, 0);
		editMenu->setItemEnabled(Sear, 0);
		extraMenu->setItemEnabled(hyph, 0);
		menuBar()->setItemEnabled(Stm, 1);
		menuBar()->setItemEnabled(Obm, 1);
		StilMenu->clear();
		StilMenu->insertItem( tr("&Color"), ColorMenu);
		StilMenu->insertItem( tr("&Shade"), ShadeMenu);
		if (Nr == 6)
			ObjMenu->setItemEnabled(ShapeM, 1);
		WerkTools->KetteAus->setEnabled(false);
		WerkTools->KetteEin->setEnabled(false);
		if (Nr != 5)
			WerkTools->Rotiere->setEnabled(true);
		else
			WerkTools->Rotiere->setEnabled(false);
		break;
	}
	doc->CurrentSel = Nr;
	Mpal->RotationGroup->setButton(doc->RotMode);
	if (view->SelItem.count() > 1)
	{
		ObjMenu->setItemEnabled(DistM, 1);
		ObjMenu->setItemEnabled(PfadTP, 0);
		editMenu->setItemEnabled(Sear, 0);
		bool hPoly = true;
		bool isGroup = true;
		int firstElem = -1;
		if (b->Groups.count() != 0)
			firstElem = b->Groups.top();
		for (uint bx=0; bx<view->SelItem.count(); ++bx)
		{
			if (view->SelItem.at(bx)->PType != 6)
				hPoly = false;
			if (view->SelItem.at(bx)->Groups.count() != 0)
			{
				if (view->SelItem.at(bx)->Groups.top() != firstElem)
					isGroup = false;
			}
			else
				isGroup = false;
		}
		ObjMenu->setItemEnabled(Gr, !isGroup);
		ObjMenu->setItemEnabled(PfadV, hPoly);
		if (view->SelItem.count() == 2)
		{
			if (((b->PType == 4) || (view->SelItem.at(1)->PType == 4)) && ((b->PType == 7) || (view->SelItem.at(1)->PType == 7)))
			{
				PageItem* bx = view->SelItem.at(1);
				if ((b->NextBox == 0) && (b->BackBox == 0) && (bx->NextBox == 0) && (bx->BackBox == 0) && (b->Groups.count() == 0))
					ObjMenu->setItemEnabled(PfadT, 1);
			}
		}
	}
	else
	{
		ObjMenu->setItemEnabled(DistM, 0);
		ObjMenu->setItemEnabled(Gr, 0);
		ObjMenu->setItemEnabled(PfadT, 0);
		ObjMenu->setItemEnabled(PfadV, 0);
	}
	if (view->SelItem.count() != 0)
	{
		Mpal->Textflow->setChecked(b->Textflow);
		ObjMenu->setItemEnabled(LockOb, 1);
		if (b->Groups.count() != 0)
			ObjMenu->setItemEnabled(UnGr, 1);
		else
		{
			ObjMenu->setItemEnabled(UnGr, 0);
			if ((b->PType == 6) && (b->Segments.count() != 0))
				ObjMenu->setItemEnabled(PfadS, 1);
			else
				ObjMenu->setItemEnabled(PfadS, 0);
		}
		if (b->Locked)
		{
			ObjMenu->setItemEnabled(DistM, 0);
			ObjMenu->setItemEnabled(ShapeM, 0);
			ObjMenu->setItemEnabled(PfadTP, 0);
			ObjMenu->setItemEnabled(PfadS, 0);
			ObjMenu->setItemEnabled(PfadT, 0);
			ObjMenu->setItemEnabled(PfadDT, 0);
			ObjMenu->setItemEnabled(PfadV, 0);
			ObjMenu->setItemEnabled(Loesch, 0);
			ObjMenu->setItemEnabled(OBack, 0);
			ObjMenu->setItemEnabled(OFront, 0);
			ObjMenu->setItemEnabled(ORaise, 0);
			ObjMenu->setItemEnabled(OLower, 0);
			editMenu->setItemEnabled(edid1, 0);
			editMenu->setItemEnabled(edid4, 0);
			WerkTools->Rotiere->setEnabled(false);
			ObjMenu->changeItem(LockOb, tr("Un&lock"));
		}
		else
		{
			ObjMenu->changeItem(LockOb, tr("&Lock"));
			if ((b->isTableItem) && (b->isSingleSel))
			{
				ObjMenu->setItemEnabled(PfadTP, 0);
				ObjMenu->setItemEnabled(ODup, 0);
				ObjMenu->setItemEnabled(OMDup, 0);
				ObjMenu->setItemEnabled(Loesch, 0);
				ObjMenu->setItemEnabled(OBack, 0);
				ObjMenu->setItemEnabled(OFront, 0);
				ObjMenu->setItemEnabled(ORaise, 0);
				ObjMenu->setItemEnabled(OLower, 0);
			}
			else
			{
				ObjMenu->setItemEnabled(ODup, 1);
				ObjMenu->setItemEnabled(OMDup, 1);
				ObjMenu->setItemEnabled(Loesch, 1);
				ObjMenu->setItemEnabled(OBack, 1);
				ObjMenu->setItemEnabled(OFront, 1);
				ObjMenu->setItemEnabled(ORaise, 1);
				ObjMenu->setItemEnabled(OLower, 1);
			}
		}
	}
	Mpal->NewSel(Nr);
	if (Nr != -1)
	{
		Mpal->SetCurItem(b);
//		Tpal->slotShowSelect(b->OwnPage->PageNr, b->ItemNr);
		if (b->FrameType == 0)
			SCustom->setPixmap(SCustom->getIconPixmap(0));
		if (b->FrameType == 1)
			SCustom->setPixmap(SCustom->getIconPixmap(1));
		if (b->FrameType > 3)
			SCustom->setPixmap(SCustom->getIconPixmap(b->FrameType-2));
	}
}

void ScribusApp::slotDocCh(bool reb)
{
/*	if ((reb) && (!doc->TemplateMode) && (view->SelItem.count() != 0))
	{
		for (uint upd = 0; upd < view->SelItem.count(); ++upd)
			Tpal->slotUpdateElement(doc->currentPage->PageNr, view->SelItem.at(upd)->ItemNr);
	} */
	if (!doc->isModified())
		doc->setModified();
	ActWin->setCaption( doc->DocName + "*");
	fileMenu->setItemEnabled(fid4, 1);
	DatSav->setEnabled(true);
	fileMenu->setItemEnabled(M_SaveAs, 1);
	fileMenu->setItemEnabled(fid51, 1);
	if (!doc->TemplateMode)
	{
		DatClo->setEnabled(true);
		if (doc->hasName)
			fileMenu->setItemEnabled(fid52, 1);
	}
	ActWin->MenuStat[0] = DatSav->isEnabled();
	ActWin->MenuStat[1] = fileMenu->isItemEnabled(fid1);
	ActWin->MenuStat[2] = fileMenu->isItemEnabled(fid4);
	ActWin->MenuStat[3] = fileMenu->isItemEnabled(M_SaveAs);
	if (view->SelItem.count() != 0)
	{
		PageItem* b = view->SelItem.at(0);
		if (b->Locked)
			ObjMenu->changeItem(LockOb, tr("Un&lock"));
		else
			ObjMenu->changeItem(LockOb, tr("&Lock"));
	}
}

void ScribusApp::UpdateRecent(QString fn)
{
	recentMenu->clear();
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
	uint max = QMIN(Prefs.RecentDCount, RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		recentMenu->insertItem(RecentDocs[m]);
	}
}
 
void ScribusApp::RemoveRecent(QString fn)
{
	recentMenu->clear();
	if (RecentDocs.findIndex(fn) != -1)
	{
		RecentDocs.remove(fn);
		if (!fileWatcher->isActive())
			fileWatcher->removeFile(fn);
	}
	uint max = QMIN(Prefs.RecentDCount, RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		recentMenu->insertItem(RecentDocs[m]);
	}
}

void ScribusApp::LoadRecent(int id)
{
	QString fn = recentMenu->text(id);
	QFileInfo fd(fn);
	if (!fd.exists())
	{
		RecentDocs.remove(fn);
		fileWatcher->removeFile(fn);
		recentMenu->clear();
		uint max = QMIN(Prefs.RecentDCount, RecentDocs.count());
		for (uint m = 0; m < max; ++m)
		{
			recentMenu->insertItem(RecentDocs[m]);
		}
		return;
	}
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	LadeDoc(recentMenu->text(id));
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
}

bool ScribusApp::slotDocOpen()
{
	bool ret = false;
	PrefsContext* docContext = prefsFile->getContext("docdirs", false);
	QString docDir = ".";
	if (Prefs.DocDir != "")
		docDir = docContext->get("docsopen", Prefs.DocDir);
	else
		docDir = docContext->get("docsopen", ".");
	QString formats = "";
#ifdef HAVE_LIBZ
	formats += tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;");
#else
	formats += tr("Documents (*.sla *.scd);;");
#endif
	if (DLLexists(6))
		formats += tr("Postscript-Files (*.eps *.EPS *.ps *.PS);;");
	if (DLLexists(10))
#ifdef HAVE_LIBZ
		formats += tr("SVG-Images (*.svg *.svgz);;");
#else
		formats += tr("SVG-Images (*.svg);;");
#endif
	if (DLLexists(12))
		formats += tr("OpenOffice.org Draw (*.sxd);;All Files (*)");
	formats + tr("All Files (*)");
	QString fileName = CFileDialog( docDir, tr("Open"), formats);
	docContext->set("docsopen", fileName.left(fileName.findRev("/")));
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	ret = LadeDoc(fileName);
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	return ret;
}

bool ScribusApp::slotDocMerge()
{
	bool ret = false;
	MergeDoc *dia = new MergeDoc(this, false, doc->PageC, doc->currentPage->PageNr + 1);
	if (dia->exec())
	{
		FMess->setText(tr("Importing Pages..."));
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		std::vector<int> pageNs;
		parsePagesString(dia->getPageNumbers(), &pageNs, dia->getPageCounter());
		int startPage, nrToImport;
		bool doIt = true;
		if (doc->TemplateMode)
		{
			if (pageNs.size() > 1)
			{
				LadeSeite(dia->getFromDoc(), pageNs[0] - 1, false);
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
				startPage = doc->PageC + 1;
			addNewPages(dia->getImportWherePage(), importWhere, pageNs.size());
			nrToImport = pageNs.size();
		}
		else
		{
			startPage = doc->currentPage->PageNr + 1;
			nrToImport = pageNs.size();
			if (pageNs.size() > (doc->PageC - doc->currentPage->PageNr))
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
						addNewPages(doc->PageC, 2, pageNs.size() - (doc->PageC - doc->currentPage->PageNr));
					break;
					case QMessageBox::No:
						nrToImport = doc->PageC - doc->currentPage->PageNr;
					break;
					case QMessageBox::Cancel:
						doIt = false;
						FMess->setText("");
					break;
				}
				qApp->setOverrideCursor(QCursor(waitCursor), true);
			}
		}
		if (doIt)
		{
			if (nrToImport > 0)
			{
				FProg->reset();
				FProg->setTotalSteps(nrToImport);
				int counter = startPage;
				for (int i = 0; i < nrToImport; ++i)
				{
					view->GotoPa(counter);
					LadeSeite(dia->getFromDoc(), pageNs[i] - 1, false);
					counter++;
					FProg->setProgress(i + 1);
				}
				view->GotoPa(startPage);
				FProg->reset();
				FMess->setText(tr("Import done"));
			}
			else
			{
				FMess->setText(tr("Found nothing to import"));
				doIt = false;
			}
		}
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		ret = doIt;
	}
	delete dia;
	return ret;
}

bool ScribusApp::LadeSeite(QString fileName, int Nr, bool Mpa)
{
	bool ret = false;
	if (!fileName.isEmpty())
	{
		if (!Mpa)
			doc->OpenNodes = Tpal->buildReopenVals();
		doc->loading = true;
		ScriXmlDoc *ss = new ScriXmlDoc();
		uint cc = doc->Items.count();
		if(!ss->ReadPage(fileName, Prefs.AvailFonts, doc, view, Nr, Mpa))
		{
			delete ss;
			doc->loading = false;
			return false;
		}
		delete ss;
		if (CMSavail)
		{
			if (doc->CMSSettings.CMSinUse)
			{
				RecalcColors();
				view->RecalcPictures(&InputProfiles);
			}
		}
		for (uint azz = cc; azz < doc->Items.count(); ++azz)
		{
			PageItem *ite = doc->Items.at(azz);
			if ((ite->PType == 4) && (ite->isBookmark))
				BookPal->BView->AddPageItem(ite);
		}
		Mpal->Cpal->SetColors(doc->PageColors);
		Mpal->updateCList();
		Mpal->Spal->setFormats(doc);
		Mpal->SetLineFormats(doc);
		Mpal->startArrow->rebuildList(&doc->arrowStyles);
		Mpal->endArrow->rebuildList(&doc->arrowStyles);
		if (!Mpa)
		{
			Tpal->BuildTree(view);
			Tpal->reopenTree(doc->OpenNodes);
		}
		slotDocCh();
		view->LaMenu();
		Lpal->rebuildList();
		doc->loading = false;
		ret = true;
	}
	if (!Mpa)
		Sepal->Rebuild();
	view->DrawNew();
	return ret;
}

bool ScribusApp::LadeDoc(QString fileName)
{
	QFileInfo fi(fileName);
	if (!fi.exists())
		return false;
/*	if (HaveDoc)
		doc->OpenNodes = Tpal->buildReopenVals(); */
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
			QMessageBox::critical(this, tr("Fatal Error"), tr("File %1 \nis not in an acceptable format").arg(FName), tr("OK"));
			return false;
		}
		Prefs.AvailFonts.AddScalableFonts(fi.dirPath(true)+"/", FName);
		doc=new ScribusDoc(&Prefs);
		doc->appMode = NormalMode;
		doc->HasCMS = false;
		doc->ActiveLayer = 0;
		doc->OpenNodes.clear();
		doc->loading = true;
		FMess->setText( tr("Loading..."));
		FProg->reset();
		ScribusWin* w = new ScribusWin(wsp, doc);
		view = new ScribusView(w, doc, &Prefs);
		view->Scale = 1.0*Prefs.DisScale;
		w->setView(view);
		ActWin = w;
		doc->WinHan = w;
		w->setCentralWidget(view);
#ifdef HAVE_CMS
		w->SoftProofing = false;
		w->Gamut = false;
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
			FMess->setText("");
			FProg->reset();
			ActWin = NULL;
			if (windows.count() != 0)
				newActWin(ActWinOld);
			return false;
		}
		ScApp->ScriptRunning = false;
		FMess->setText("");
		FProg->reset();
#ifdef HAVE_CMS
		CMSuse = cmsu;
#endif
		HaveDoc++;
		if (doc->PDF_Optionen.LPISettings.count() == 0)
		{
			struct LPIData lpo;
			lpo.Frequency = 75;
			lpo.SpotFunc = 2;
			lpo.Angle = 105;
			doc->PDF_Optionen.LPISettings.insert("Cyan", lpo);
			lpo.Angle = 75;
			doc->PDF_Optionen.LPISettings.insert("Magenta", lpo);
			lpo.Angle = 90;
			doc->PDF_Optionen.LPISettings.insert("Yellow", lpo);
			lpo.Angle = 45;
			doc->PDF_Optionen.LPISettings.insert("Black", lpo);
		}
		connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
		if (!doc->HasCMS)
		{
			doc->CMSSettings.DefaultInputProfile = Prefs.DCMSset.DefaultInputProfile;
			doc->CMSSettings.DefaultInputProfile2 = Prefs.DCMSset.DefaultInputProfile2;
			doc->CMSSettings.DefaultMonitorProfile = Prefs.DCMSset.DefaultMonitorProfile;
			doc->CMSSettings.DefaultPrinterProfile = Prefs.DCMSset.DefaultPrinterProfile;
			doc->CMSSettings.DefaultIntentPrinter = Prefs.DCMSset.DefaultIntentPrinter;
			doc->CMSSettings.DefaultIntentMonitor = Prefs.DCMSset.DefaultIntentMonitor;
			doc->CMSSettings.DefaultIntentMonitor2 = Prefs.DCMSset.DefaultIntentMonitor2;
			doc->CMSSettings.SoftProofOn = Prefs.DCMSset.SoftProofOn;
			doc->CMSSettings.GamutCheck = Prefs.DCMSset.GamutCheck;
			doc->CMSSettings.BlackPoint = Prefs.DCMSset.BlackPoint;
			doc->CMSSettings.CMSinUse = false;
		}
		if (CMSavail)
		{
			bool cmsWarning = false;
			QStringList missing;
			QStringList replacement;
			if (!InputProfiles.contains(doc->CMSSettings.DefaultInputProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultInputProfile);
				replacement.append(Prefs.DCMSset.DefaultInputProfile);
				doc->CMSSettings.DefaultInputProfile = Prefs.DCMSset.DefaultInputProfile;
			}
			if (!InputProfiles.contains(doc->CMSSettings.DefaultInputProfile2))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultInputProfile2);
				replacement.append(Prefs.DCMSset.DefaultInputProfile2);
				doc->CMSSettings.DefaultInputProfile2 = Prefs.DCMSset.DefaultInputProfile2;
			}
			if (!MonitorProfiles.contains(doc->CMSSettings.DefaultMonitorProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultMonitorProfile);
				replacement.append(Prefs.DCMSset.DefaultMonitorProfile);
				doc->CMSSettings.DefaultMonitorProfile = Prefs.DCMSset.DefaultMonitorProfile;
			}
			if (!PrinterProfiles.contains(doc->CMSSettings.DefaultPrinterProfile))
			{
				cmsWarning = true;
				missing.append(doc->CMSSettings.DefaultPrinterProfile);
				replacement.append(Prefs.DCMSset.DefaultPrinterProfile);
				doc->CMSSettings.DefaultPrinterProfile = Prefs.DCMSset.DefaultPrinterProfile;
			}
			if (!PrinterProfiles.contains(doc->PDF_Optionen.PrintProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Optionen.PrintProf);
				replacement.append(Prefs.DCMSset.DefaultPrinterProfile);
				doc->PDF_Optionen.PrintProf = doc->CMSSettings.DefaultPrinterProfile;
			}
			if (!InputProfiles.contains(doc->PDF_Optionen.ImageProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Optionen.ImageProf);
				replacement.append(Prefs.DCMSset.DefaultInputProfile);
				doc->PDF_Optionen.ImageProf = doc->CMSSettings.DefaultInputProfile;
			}
			if (!InputProfiles.contains(doc->PDF_Optionen.SolidProf))
			{
				cmsWarning = true;
				missing.append(doc->PDF_Optionen.SolidProf);
				replacement.append(Prefs.DCMSset.DefaultInputProfile2);
				doc->PDF_Optionen.SolidProf = doc->CMSSettings.DefaultInputProfile2;
			}
			if ((cmsWarning) && (doc->HasCMS))
			{
				qApp->setOverrideCursor(QCursor(arrowCursor), true);
				QString mess = tr("Some ICC-Profiles used by this Document are not installed:")+"\n\n";
				for (uint m = 0; m < missing.count(); ++m)
				{
					mess += missing[m] + tr(" was replaced by: ")+replacement[m]+"\n";
				}
				QMessageBox::warning(this, tr("Warning"), mess, 1, 0, 0);
			}
#ifdef HAVE_CMS
			w->SoftProofing = doc->CMSSettings.SoftProofOn;
			w->Gamut = doc->CMSSettings.GamutCheck;
			CMSuse = doc->CMSSettings.CMSinUse;
			w->IntentPrinter = doc->CMSSettings.DefaultIntentPrinter;
			w->IntentMonitor = doc->CMSSettings.DefaultIntentMonitor;
			SoftProofing = doc->CMSSettings.SoftProofOn;
			Gamut = doc->CMSSettings.GamutCheck;
			IntentPrinter = doc->CMSSettings.DefaultIntentPrinter;
			IntentMonitor = doc->CMSSettings.DefaultIntentMonitor;
			w->OpenCMSProfiles(InputProfiles, MonitorProfiles, PrinterProfiles);
			CMSuse = doc->CMSSettings.CMSinUse;
			stdProof = w->stdProof;
			stdTrans = w->stdTrans;
			stdProofImg = w->stdProofImg;
			stdTransImg = w->stdTransImg;
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
			doc->PDF_Optionen.SComp = doc->CMSSettings.ComponentsInput2;
#endif
			if (doc->CMSSettings.CMSinUse)
			{
				RecalcColors();
				view->RecalcPictures(&InputProfiles);
			}
		}
		Mpal->Cpal->SetColors(doc->PageColors);
		Mpal->Cpal->ChooseGrad(0);
		if (fl->FileType > 1)
		{
			doc->DocName = FName+tr("(converted)");
			QFileInfo fi(doc->DocName);
			doc->DocName = fi.fileName();
		}
		else
			doc->DocName = FName;
		doc->MasterP = false;
		doc->Language = GetLang(doc->Language);
		HaveNewDoc();
		Mpal->updateCList();
		doc->hasName = true;
		if (doc->MasterPages.count() == 0)
		{
			doc->DocPages = doc->Pages;
			doc->Pages = doc->MasterPages;
			doc->PageC = doc->MasterPages.count();
			bool atf = doc->PageAT;
			doc->PageAT = false;
			slotNewPage(0);
			doc->PageAT = atf;
			doc->MasterNames["Normal"] = 0;
			doc->Pages.at(0)->PageNam = "Normal";
			doc->MasterPages = doc->Pages;
			doc->PageC = doc->DocPages.count();
			doc->Pages = doc->DocPages;
			doc->MasterP = false;
		}
		doc->loading = false;
		view->slotDoZoom();
		view->GotoPage(0);
		doc->DocItems = doc->Items;
		doc->RePos = true;
		QPixmap pgPix(10, 10);
		QRect rd = QRect(0,0,9,9);
		ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
		for (uint azz=0; azz<doc->MasterItems.count(); ++azz)
		{
			PageItem *ite = doc->MasterItems.at(azz);
			if ((ite->PType == 4) || (ite->PType == 8))
				ite->DrawObj(painter, rd);
		}
//		RestoreBookMarks();
		for (uint azz=0; azz<doc->Items.count(); ++azz)
		{
			PageItem *ite = doc->Items.at(azz);
			if ((ite->PType == 4) || (ite->PType == 8))
				ite->DrawObj(painter, rd);
/*			if (doc->OldBM)
			{
				if ((ite->PType == 4) && (ite->isBookmark))
					BookPal->BView->AddPageItem(ite);
			}
			else
			{
				if ((ite->PType == 4) && (ite->isBookmark))
					BookPal->BView->ChangeItem(ite->BMnr, ite->ItemNr);
			} */
		}
		delete painter;
//		if (doc->OldBM)
//			StoreBookmarks();
//		ActWin->NrItems = BookPal->BView->NrItems;
//		ActWin->First = BookPal->BView->First;
//		ActWin->Last = BookPal->BView->Last;
		doc->RePos = false;
		doc->setUnModified();
		UpdateRecent(FName);
		FMess->setText( tr("Ready"));
		ret = true;
		for (uint p = 0; p < doc->Pages.count(); ++p)
		{
			Apply_Temp(doc->Pages.at(p)->MPageNam, p, false);
		}
		if (fl->FileType > 1)
		{
			doc->hasName = false;
			slotFileSaveAs();
		}
		delete fl;
		if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
			w->showMaximized();
		else
			w->show();
		view->show();
		newActWin(w);
		connect(doc->ASaveTimer, SIGNAL(timeout()), w, SLOT(slotAutoSave()));
		connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
		connect(fileWatcher, SIGNAL(fileChanged(QString )), view, SLOT(updatePict(QString)));
		connect(fileWatcher, SIGNAL(fileDeleted(QString )), view, SLOT(removePict(QString)));
		connect(UndoManager::instance(), SIGNAL(undoRedoDone()), view, SLOT(DrawNew()));
		if (doc->AutoSave)
			doc->ASaveTimer->start(doc->AutoSaveTime);
		DatSav->setEnabled(false);
		fileMenu->setItemEnabled(fid4, 0);
	}
	else
	{
		Sepal->Vie = 0;
	}
//	Sepal->Rebuild();
	return ret;
}

void ScribusApp::slotFileOpen()
{
	if (view->SelItem.count() != 0)
	{
		PageItem *b = view->SelItem.at(0);
		if (b->PType == 2)
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
				if ((form1 == "jpg") || (form1 == "png") || (form1 == "xpm") || (form1 == "gif"))
				{
					formats += form2 + " (*."+form1+" *."+form2+");;";
					formatD += "*."+form1+" *."+form2+" ";
				}
			}
#ifdef HAVE_TIFF
			formats += "TIFF (*.tif *.TIF);;";
			formatD += "*.tif *.TIF";
#endif
			formats += "EPS (*.eps *.EPS);;PDF (*.pdf *.PDF);;" + tr("All Files (*)");
			formatD += " *.eps *.EPS *.pdf *.PDF";
			formatD += ");;"+formats;
			QString docDir = ".";
			if (Prefs.DocDir != "")
				docDir = dirs->get("images", Prefs.DocDir);
			else
				docDir = dirs->get("images", ".");
			QString fileName = CFileDialog( docDir, tr("Open"), formatD, "", true);
			if (!fileName.isEmpty())
			{
				dirs->set("images", fileName.left(fileName.findRev("/")));
				b->EmProfile = "";
				b->UseEmbedded = true;
				b->IProfile = doc->CMSSettings.DefaultInputProfile;
				b->IRender = doc->CMSSettings.DefaultIntentMonitor2;
				qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				view->LoadPict(fileName, b->ItemNr);
				view->AdjustPictScale(b);
				view->AdjustPreview(b);
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				qApp->restoreOverrideCursor();
				view->DrawNew();
				Mpal->Cpal->SetColors(doc->PageColors);
				Mpal->updateCList();
				Mpal->ShowCMS();
				slotDocCh();
			}
		}
		if (b->PType == 4)
		{
			gtGetText* gt = new gtGetText();
			gt->run(false);
			delete gt;
			if (doc->docHyphenator->AutoCheck)
				doc->docHyphenator->slotHyphenate(b);
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
	if ((doc->hasName) && (doc->isModified()) && (!doc->TemplateMode))
	{
		QString fn = doc->DocName;
		QFileInfo fi(fn);
		QDir::setCurrent(fi.dirPath(true));
		doc->setUnModified();
		slotFileClose();
		qApp->processEvents();
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		LadeDoc(fn);
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
}

void ScribusApp::slotAutoSaved()
{
	if (ActWin == sender())
	{
		fileMenu->setItemEnabled(fid4, 0);
		DatSav->setEnabled(false);
		ActWin->setCaption(doc->DocName);
	}
}

bool ScribusApp::slotFileSave()
{
	bool ret = false;
	if (doc->hasName)
	{
		QString fn = doc->DocName;
		ret = DoFileSave(fn);
		if (!ret)
			QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
	}
	else
		ret = slotFileSaveAs();
	return ret;
}

bool ScribusApp::slotFileSaveAs()
{
	bool ret = false;
	QString fna;
	PrefsContext* docContext = prefsFile->getContext("docdirs", false);
	QString wdir = ".";
	if (doc->hasName)
	{
		QFileInfo fi(doc->DocName);
		wdir = fi.dirPath();
		fna = fi.dirPath()+"/"+fi.baseName()+".sla";
	}
	else
	{
		if (Prefs.DocDir != "")
			wdir = docContext->get("save_as", Prefs.DocDir);
		else
			wdir = docContext->get("save_as", ".");
		if (wdir.right(1) != "/")
			fna = wdir + "/";
		else
			fna = wdir;
		fna += doc->DocName + ".sla";
	}
#ifdef HAVE_LIBZ
	QString fn = CFileDialog( wdir, tr("Save as"), tr("Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)"), fna, false, false, true);
#else
	QString fn = CFileDialog( wdir, tr("Save as"), tr("Documents (*.sla *.scd);;All Files (*)"), fna, false, false, false);
#endif
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
				QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
			else
				doc->PDF_Optionen.Datei = ""; // #1482 reset the pdf file name
		}
	}
	FMess->setText( tr("Ready"));
	return ret;
}

bool ScribusApp::DoFileSave(QString fn)
{
	bool ret = true;
	ReorgFonts();
	FMess->setText( tr("Saving..."));
	FProg->reset();
	QFileInfo fi(fn);
	QDir::setCurrent(fi.dirPath(true));
	ScriXmlDoc *ss = new ScriXmlDoc();
	qApp->processEvents();
	ret = ss->WriteDoc(fn, doc, FProg);
	delete ss;
	if (ret)
	{
		doc->setUnModified();
		ActWin->setCaption(fn);
		doc->DocName = fn;
		fileMenu->setItemEnabled(fid4, 0);
		fileMenu->setItemEnabled(fid52, 0);
		DatSav->setEnabled(false);
		UpdateRecent(fn);
		doc->hasName = true;
	}
	FMess->setText("");
	FProg->reset();
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
}

bool ScribusApp::DoFileClose()
{
	UndoManager::instance()->remove(doc->DocName);
	if(doc->TemplateMode)
	{
		ActWin->muster->close();
		qApp->processEvents();
	}
	setAppMode(NormalMode);
	doc->ASaveTimer->stop();
	disconnect(doc->ASaveTimer, SIGNAL(timeout()), doc->WinHan, SLOT(slotAutoSave()));
	disconnect(doc->WinHan, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	disconnect(fileWatcher, SIGNAL(fileChanged(QString )), view, SLOT(updatePict(QString)));
	disconnect(fileWatcher, SIGNAL(fileDeleted(QString )), view, SLOT(removePict(QString)));
	for (uint a = 0; a < doc->Items.count(); ++a)
	{
		PageItem *b = doc->Items.at(a);
		if (b->PicAvail)
			fileWatcher->removeFile(b->Pfile);
	}
	if ((doc->UnData.UnCode == 0) && (doc->UnDoValid))
		delete doc->UnData.Item;
	if (CMSavail)
		ActWin->CloseCMSProfiles();
//	Mpal->NewSel(-1);
	Mpal->UnsetDoc();
	Sepal->Vie = 0;
	Sepal->Rebuild();
	Mpal->Spal->setFormats(0);
	Mpal->SetLineFormats(0);
	if (doc->EditClip)
		Npal->doc = 0;
	BookPal->BView->clear();
	BookPal->BView->NrItems = 0;
	BookPal->BView->First = 1;
	BookPal->BView->Last = 0;
	if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
	{
		QValueList<int>::iterator itm;
		for (itm = MenuItemsFile.begin(); itm != MenuItemsFile.end(); ++itm)
		{
			fileMenu->setItemEnabled((*itm), 0);
		}
		importMenu->setItemEnabled(fid2, 0);
		importMenu->setItemEnabled(fid2aa, 0);
		exportMenu->setItemEnabled(fid3, 0);
		fileMenu->setItemEnabled(fid4, 0);
		fileMenu->setItemEnabled(M_SaveAs, 0);
		fileMenu->setItemEnabled(fid52, 0);
		fileMenu->setItemEnabled(fid11, 0);
		exportMenu->setItemEnabled(fid8, 0);
		importMenu->setItemEnabled(fid2a, 0);
		exportMenu->setItemEnabled(fid10, 0);
		editMenu->setItemEnabled(edUndo, 0);
		editMenu->setItemEnabled(edRedo, 0);
		editMenu->setItemEnabled(edid1, 0);
		editMenu->setItemEnabled(edid2, 0);
		editMenu->setItemEnabled(edid3, 0);
		editMenu->setItemEnabled(edid4, 0);
		editMenu->setItemEnabled(edid5, 0);
		editMenu->setItemEnabled(edid6, 0);
		editMenu->setItemEnabled(edid6a, 0);
		editMenu->setItemEnabled(Sear, 0);
		extraMenu->setItemEnabled(hyph, 0);
		menuBar()->setItemEnabled(ViMen, 0);
		menuBar()->setItemEnabled(WinMen, 0);
		viewMenu->setItemChecked(uGuide, false);
		viewMenu->setItemChecked(uRas, false);
		editMenu->setItemEnabled(tman, 0);
		editMenu->setItemEnabled(jman, 0);
		menuBar()->setItemEnabled(pgmm, 0);
		menuBar()->setItemEnabled(exmn, 0);
		menuBar()->setItemEnabled(Stm, 0);
		menuBar()->setItemEnabled(Obm, 0);
		WerkTools->setEnabled(false);
		WerkToolsP->setEnabled(false);
		ColorMenC->clear();
		Mpal->Cpal->SetColors(Prefs.DColors);
		Mpal->Cpal->ChooseGrad(0);
		FMess->setText( tr("Ready"));
		DatPri->setEnabled(false);
		DatPDF->setEnabled(false);
		DatSav->setEnabled(false);
		DatClo->setEnabled(false);
		PrinterUsed = false;
#ifdef HAVE_CMS
		CMSuse = false;
		SoftProofing = Prefs.DCMSset.SoftProofOn;
		IntentPrinter = Prefs.DCMSset.DefaultIntentPrinter;
		IntentMonitor = Prefs.DCMSset.DefaultIntentMonitor;
#endif
	}
	view->close();
	delete view;
	Tpal->PageObj.clear();
	Tpal->Seiten.clear();
	doc->loading = true;
	Tpal->ListView1->clear();
	Lpal->ClearInhalt();
	HaveDoc--;
	view = NULL;
	delete doc;
	doc = NULL;
	ActWin = NULL;
	return true;
}

void ScribusApp::slotFilePrint()
{
	//QString fna, prn, cmd, scmd, cc, data, SepNam;
	//int Nr;
	//bool fil, sep, farbe, PSfile, mirrorH, mirrorV, useICC, DoGCR;
	//PSfile = false;
	PrintOptions options;
	FMess->setText( tr("Printing..."));
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
	Druck *printer = new Druck(this, options.filename, options.printer, PDef.Command, Prefs.GCRMode);
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
				parsePagesString("*", &options.pageNumbers, doc->PageC);
			else
				parsePagesString(printer->PageNr->text(), &options.pageNumbers, doc->PageC);
		}
		options.copies = printer->numCopies();
		options.outputSeparations = printer->outputSeparations();
		options.separationName = printer->separationName();
		options.useColor = printer->color();
		options.mirrorH = printer->MirrorH;
		options.mirrorV = printer->MirrorV;
		options.useICC = printer->ICCinUse;
		options.doGCR = printer->DoGCR;
		options.PSLevel = printer->PSLevel;
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
			QMessageBox::warning(this, tr("Warning"), tr("Printing failed!"), tr("OK"));
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	delete printer;
	FMess->setText( tr("Ready"));
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
	QString filename = options->filename;
	ReallyUsed.clear();
	GetUsedFonts(&ReallyUsed);
	PSLib *dd = getPSDriver(true, Prefs.AvailFonts, ReallyUsed, doc->PageColors, false);
	if (dd != NULL)
	{
		bool PSfile = false;
		if (options->toFile)
			PSfile = dd->PS_set_file(filename);
		else
		{
			PSfile = dd->PS_set_file(PrefsPfad+"/tmp.ps");
			filename = PrefsPfad+"/tmp.ps";
		}
		if (PSfile)
		{
			// Write the PS to a file
			dd->CreatePS(doc, view, options->pageNumbers, options->outputSeparations, options->separationName,
			               options->useColor, options->mirrorH, options->mirrorV, options->useICC, options->doGCR);
			if (options->PSLevel != 3)
			{
				// use gs to convert our PS to a lower version
				QString tmp;
				QString opts = "-dDEVICEWIDTHPOINTS=";
				opts += tmp.setNum(doc->PageB);
				opts += " -dDEVICEHEIGHTPOINTS=";
				opts += tmp.setNum(doc->PageH);
				if (options->PSLevel == 1)
					system("ps2ps -dLanguageLevel=1 "+opts+" \""+filename+"\" \""+filename+".tmp\"");
				else
					system("ps2ps "+opts+" \""+filename+"\" \""+filename+".tmp\"");
				system("mv \""+filename+".tmp\" \""+filename+"\"");
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
		closePSDriver();
	}
	return retw;
}

void ScribusApp::slotFileQuit()
{
	Mpal->UnsetDoc();
	close();
}

void ScribusApp::slotEditCut()
{
	uint a;
	NoFrameEdit();
	QString BufferI = "";
	if ((HaveDoc) && (view->SelItem.count() != 0))
	{
		Buffer2 = "<SCRIBUSTEXT>";
		PageItem *b = view->SelItem.at(0);
		if (doc->appMode == EditMode)
		{
			if ((b->itemText.count() == 0) || (!b->HasSel))
				return;
			PageItem *nb = b;
			while (nb != 0)
			{
				if (nb->BackBox != 0)
					nb = nb->BackBox;
				else
					break;
			}
			while (nb != 0)
			{
				for (a = 0; a < nb->itemText.count(); ++a)
				{
					if (nb->itemText.at(a)->cselect)
					{
						if (nb->itemText.at(a)->ch == QChar(13))
						{
							Buffer2 += QChar(5);
							BufferI += QChar(10);
						}
						else if (nb->itemText.at(a)->ch == QChar(9))
						{
							Buffer2 += QChar(4);
							BufferI += QChar(9);
						}
						else
						{
							Buffer2 += nb->itemText.at(a)->ch;
							BufferI += nb->itemText.at(a)->ch;
						}
						Buffer2 += "\t";
						Buffer2 += nb->itemText.at(a)->cfont+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->csize)+"\t";
						Buffer2 += nb->itemText.at(a)->ccolor+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cextra)+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cshade)+'\t';
						Buffer2 += QString::number(nb->itemText.at(a)->cstyle)+'\t';
						Buffer2 += QString::number(nb->itemText.at(a)->cab)+'\t';
						Buffer2 += nb->itemText.at(a)->cstroke+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cshade2)+'\t';
						Buffer2 += QString::number(nb->itemText.at(a)->cscale)+'\n';
					}
				}
				DeleteSel(nb);
				nb = nb->NextBox;
			}
			view->RefreshItem(b);
		}
		else
		{
			if ((b->isTableItem) && (b->isSingleSel))
				return;
			ScriXmlDoc *ss = new ScriXmlDoc();
			BufferI = ss->WriteElem(&view->SelItem, doc, view);
			Buffer2 = BufferI;
			view->DeleteItem();
		}
		slotDocCh();
		BuFromApp = true;
		ClipB->setText(BufferI);
		editMenu->setItemEnabled(edid3, 1);
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
		PageItem *b = view->SelItem.at(0);
		if ((doc->appMode == EditMode) && (b->HasSel))
		{
			Buffer2 += "";
			PageItem *nb = b;
			while (nb != 0)
			{
				if (nb->BackBox != 0)
					nb = nb->BackBox;
				else
					break;
			}
			while (nb != 0)
			{
				for (a = 0; a < nb->itemText.count(); ++a)
				{
					if (nb->itemText.at(a)->cselect)
					{
						if (nb->itemText.at(a)->ch == QChar(13))
						{
							Buffer2 += QChar(5);
							BufferI += QChar(10);
						}
						else if (nb->itemText.at(a)->ch == QChar(9))
						{
							Buffer2 += QChar(4);
							BufferI += QChar(9);
						}
						else
						{
							Buffer2 += nb->itemText.at(a)->ch;
							BufferI += nb->itemText.at(a)->ch;
						}
						Buffer2 += "\t";
						Buffer2 += nb->itemText.at(a)->cfont+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->csize)+"\t";
						Buffer2 += nb->itemText.at(a)->ccolor+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cextra)+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cshade)+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cstyle)+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cab)+"\t";
						Buffer2 += nb->itemText.at(a)->cstroke+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cshade2)+"\t";
						Buffer2 += QString::number(nb->itemText.at(a)->cscale)+"\n";
					}
				}
				nb = nb->NextBox;
			}
		}
		else
		{
			if ((b->isTableItem) && (b->isSingleSel))
				return;
			ScriXmlDoc *ss = new ScriXmlDoc();
			BufferI = ss->WriteElem(&view->SelItem, doc, view);
			Buffer2 = BufferI;
			delete ss;
		}
		BuFromApp = true;
		ClipB->setText(BufferI);
		editMenu->setItemEnabled(edid3, 1);
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
		if (doc->appMode == EditMode)
		{
			PageItem *b = view->SelItem.at(0);
			if (b->HasSel)
				DeleteSel(b);
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
					hg->cfont = *it;
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
						hg->cscale = 100;
					else
						hg->cscale = (*it).toInt();
					b->itemText.insert(b->CPos, hg);
					b->CPos += 1;
					hg->PRot = 0;
					hg->PtransX = 0;
					hg->PtransY = 0;
				}
			}
			else
			{
				Serializer *ss = new Serializer("");
				ss->Objekt = Buffer2;
				int st = doc->currentParaStyle;
				if (st > 5)
					ss->GetText(b, st, doc->docParagraphStyles[st].Font, doc->docParagraphStyles[st].FontSize, true);
				else
					ss->GetText(b, st, b->IFont, b->ISize, true);
				delete ss;
				if (doc->docHyphenator->AutoCheck)
					doc->docHyphenator->slotHyphenate(b);
			}
			view->RefreshItem(b);
		}
		else
		{
			if (Buffer2.startsWith("<SCRIBUSELEM"))
			{
				view->Deselect(true);
				uint ac = doc->Items.count();
				slotElemRead(Buffer2, 0, 0, false, true, doc, view);
				for (uint as = ac; as < doc->Items.count(); ++as)
				{
					view->SelectItemNr(as);
				}
			}
		}
		slotDocCh(false);
	}
}

void ScribusApp::SelectAll()
{
	if (doc->appMode == EditMode)
	{
		PageItem *b = view->SelItem.at(0);
		PageItem *nb = b;
		while (nb != 0)
		{
			if (nb->BackBox != 0)
				nb = nb->BackBox;
			else
				break;
		}
		while (nb != 0)
		{
			for (uint a = 0; a < nb->itemText.count(); ++a)
			{
				nb->itemText.at(a)->cselect = true;
				nb->HasSel = true;
			}
			nb = nb->NextBox;
		}
		view->DrawNew();
		EnableTxEdit();
	}
	else
	{
		PageItem *b;
		view->Deselect();
		for (uint a = 0; a < doc->Items.count(); ++a)
		{
			b = doc->Items.at(a);
			if (b->LayerNr == doc->ActiveLayer)
			{
				if (!b->Select)
				{
					view->SelItem.append(b);
					b->Select = true;
					b->FrameOnly = true;
					b->paintObj();
				}
			}
		}
		if (view->SelItem.count() > 1)
		{
			view->setGroupRect();
			view->paintGroupRect();
			double x, y, w, h;
			view->getGroupRect(&x, &y, &w, &h);
			Mpal->setXY(x, y);
			Mpal->setXY(w, h);
		}
		if (view->SelItem.count() > 0)
		{
			b = view->SelItem.at(0);
			view->EmitValues(b);
			HaveNewSel(b->PType);
		}
	}
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
	editMenu->setItemEnabled(edid3, 0);
	if (!cc.isNull())
	{
		if (!BuFromApp)
			Buffer2 = cc;
		BuFromApp = false;
		if (HaveDoc)
		{
			if (cc.startsWith("<SCRIBUSELEM"))
			{
				if (doc->appMode != EditMode)
					editMenu->setItemEnabled(edid3, 1);
			}
			else
			{
				if (doc->appMode == EditMode)
					editMenu->setItemEnabled(edid3, 1);
			}
		}
	}
}

void ScribusApp::DeleteText()
{
	PageItem *b = view->SelItem.at(0);
	PageItem *nb = b;
	while (nb != 0)
	{
		if (nb->BackBox != 0)
			nb = nb->BackBox;
		else
			break;
	}
	while (nb != 0)
	{
		for (uint a = 0; a < nb->itemText.count(); ++a)
		{
			DeleteSel(nb);
			nb->CPos = 0;
		}
		nb = nb->NextBox;
	}
	view->RefreshItem(b);
	slotDocCh();
}

void ScribusApp::EnableTxEdit()
{
	editMenu->setItemEnabled(edid1, 1);
	editMenu->setItemEnabled(edid2, 1);
	editMenu->setItemEnabled(edid4, 1);
}

void ScribusApp::DisableTxEdit()
{
	editMenu->setItemEnabled(edid1, 0);
	editMenu->setItemEnabled(edid2, 0);
	editMenu->setItemEnabled(edid4, 0);
}

void ScribusApp::slotHelpAbout()
{
	void *mo;
	const char *error;
	typedef About* (*sdem)(QWidget *d);
	sdem demo;
	QString pfad = LIBDIR;
#if defined(__hpux)
	pfad += "libs/libabout.sl";
#else
	pfad += "libs/libabout.so";
#endif
	mo = dlopen(pfad, RTLD_LAZY);
	if (!mo)
	{
		std::cout << "Can't find Plug-in" << endl;
		return;
	}
	dlerror();
	demo = (sdem)dlsym(mo, "Run");
	if ((error = dlerror()) != NULL)
	{
		std::cout << "Can't find Symbol" << endl;
		dlclose(mo);
		return;
	}
	About* dia = (*demo)(this);
	dia->exec();
	delete dia;
	dlclose(mo);
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
	tipsOn = !tipsOn;
	helpMenu->setItemChecked(tip, tipsOn);
	QToolTip::setEnabled(tipsOn);
}

void ScribusApp::SaveText()
{
	LoadEnc = "";
	QString wdir = ".";
	if (Prefs.DocDir != "")
		wdir = dirs->get("save_text", Prefs.DocDir);
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
	Apply_Temp(name, doc->currentPage->PageNr);
	view->DrawNew();
	slotDocCh();
//	Sepal->Rebuild();
}

void ScribusApp::slotNewPageP(int wo, QString templ)
{
	NoFrameEdit();
	view->Deselect(true);
	slotNewPage(wo);
	applyNewMaster(templ);
	if (doc->TemplateMode)
		doc->MasterPages = doc->Pages;
	else
		doc->DocPages = doc->Pages;
//	Sepal->RebuildPage();
}

/** Erzeugt eine neue Seite */
void ScribusApp::slotNewPageM()
{
	NoFrameEdit();
	view->Deselect(true);
	InsPage *dia = new InsPage(this, doc, doc->currentPage->PageNr, doc->Pages.count(), doc->PageFP);
	if (dia->exec())
	{
		QString template2 = (doc->PageFP) ? dia->getTemplate2() : tr("Normal");

		addNewPages(dia->getWherePage(), 
		            dia->getWhere(),
		            dia->getCount(),
		            dia->getTemplate(),
		            template2);
	}
	delete dia;
}

void ScribusApp::addNewPages(int wo, int where, int numPages, QString based1, QString based2)
{
		int cc;
		int wot = wo;
		switch (where)
		{
		case 0:
			wot -= 1;
			for (cc = 0; cc < numPages; ++cc)
			{
				slotNewPage(wot);
				if (doc->PageFP)
				{
					if ((doc->currentPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
						applyNewMaster(based1);
					if ((doc->currentPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->currentPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->currentPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
						applyNewMaster(based1);
				}
				else
					applyNewMaster(based1);
				wot ++;
			}
			break;
		case 1:
			for (cc = 0; cc < numPages; ++cc)
			{
				slotNewPage(wot);
				if (doc->PageFP)
				{
					if ((doc->currentPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
						applyNewMaster(based1);
					if ((doc->currentPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->currentPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->currentPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
						applyNewMaster(based1);
				}
				else
					applyNewMaster(based1);
				wot ++;
			}
			break;
		case 2:
			for (cc = 0; cc < numPages; ++cc)
			{
				slotNewPage(doc->Pages.count());
				if (doc->PageFP)
				{
					if ((doc->currentPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
						applyNewMaster(based1);
					if ((doc->currentPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->currentPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->currentPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
						applyNewMaster(based1);
				}
				else
					applyNewMaster(based1);
			}
			break;
		}
//		Sepal->RebuildPage();
		view->DrawNew();
		if (doc->TemplateMode)
			doc->MasterPages = doc->Pages;
		else
			doc->DocPages = doc->Pages;
}

void ScribusApp::slotNewPageT(int w)
{
	if (doc->TemplateMode)
		slotNewPage(w);
}

void ScribusApp::slotNewPage(int w)
{
//	if ((!doc->loading) && (!doc->TemplateMode))
//		Tpal->slotAddPage(w);
	view->addPage(w);
	if (doc->Pages.count() > 1)
	{
		pageMenu->setItemEnabled(pgmd, 1);
		pageMenu->setItemEnabled(pgmv, 1);
	}
/*	if ((!doc->loading) && (!doc->TemplateMode))
	{
		AdjustBM();
		if ((doc->PageAT) && (doc->PageC != 1))
			Tpal->slotAddElement(w, 0);
	}
	slotDocCh(!doc->loading); */
}

/** Ansicht absolut zoomen */
void ScribusApp::slotZoomAbs(double z)
{
//	view->rememberPreviousSettings();
	view->Scale = z;
	view->slotDoZoom();
}

void ScribusApp::slotZoomFit()
{
	double dx = (view->width()-50) / (doc->PageB+30);
	double dy = (view->height()-70) / (doc->PageH+30);
//	view->rememberPreviousSettings();
	if (dx > dy)
		slotZoomAbs(dy);
	else
		slotZoomAbs(dx);
}

/** Ansicht 20 % */
void ScribusApp::slotZoom20()
{
	slotZoomAbs(0.2*Prefs.DisScale);
}

/** Ansicht 50 % */
void ScribusApp::slotZoom50()
{
	slotZoomAbs(0.5*Prefs.DisScale);
}

/** Ansicht 75 % */
void ScribusApp::slotZoom75()
{
	slotZoomAbs(0.75*Prefs.DisScale);
}

/** Ansicht 100 % */
void ScribusApp::slotZoom100()
{
	slotZoomAbs(1.0*Prefs.DisScale);
}

/** Ansicht 200 % */
void ScribusApp::slotZoom200()
{
	slotZoomAbs(2.0*Prefs.DisScale);
}

void ScribusApp::ToggleAllPalettes()
{
	if (PalettesStat[0])
	{
		PalettesStat[0] = false;
		setMpal(PalettesStat[1]);
		setTpal(PalettesStat[2]);
		setBpal(PalettesStat[3]);
		setLpal(PalettesStat[4]);
		setSepal(PalettesStat[5]);
		setBookpal(PalettesStat[6]);
		setMapal(PalettesStat[7]);
		setUndoPalette(PalettesStat[8]);
	}
	else
	{
		PalettesStat[1] = Mpal->isVisible();
		PalettesStat[2] = Tpal->isVisible();
		PalettesStat[3] = ScBook->isVisible();
		PalettesStat[4] = Lpal->isVisible();
		PalettesStat[5] = Sepal->isVisible();
		PalettesStat[6] = BookPal->isVisible();
		PalettesStat[7] = MaPal->isVisible();
		PalettesStat[8] = undoPalette->isVisible();
		setMapal(false);
		setMpal(false);
		setTpal(false);
		setBpal(false);
		setLpal(false);
		setSepal(false);
		setBookpal(false);
		setUndoPalette(false);
		PalettesStat[0] = true;
	}
}

void ScribusApp::setMapal(bool visible)
{
	if (visible)
	{
		MaPal->show();
		MaPal->move(Prefs.measurePalSettings.xPosition, Prefs.measurePalSettings.yPosition);
	}
	else
	{
		Prefs.measurePalSettings.xPosition = MaPal->pos().x();
		Prefs.measurePalSettings.yPosition = MaPal->pos().y();
		MaPal->hide();
	}
}

void ScribusApp::setUndoPalette(bool visible)
{
	visible ? undoPalette->show() : undoPalette->hide();
	toolMenu->setItemChecked(viewUndoPalette, visible);
}

void ScribusApp::setMpal(bool visible)
{
	if (visible)
	{
		Mpal->show();
		Mpal->move(Prefs.mPaletteSettings.xPosition, Prefs.mPaletteSettings.yPosition);
	}
	else
	{
		Prefs.mPaletteSettings.xPosition = Mpal->pos().x();
		Prefs.mPaletteSettings.yPosition = Mpal->pos().y();
		Mpal->hide();
	}
	toolMenu->setItemChecked(viewMpal, visible);
}

void ScribusApp::ToggleMpal()
{
	setMpal(!Mpal->isVisible());
	PalettesStat[0] = false;
}

void ScribusApp::setTpal(bool visible)
{
	if (visible)
	{
		Tpal->show();
		Tpal->move(Prefs.treePalSettings.xPosition, Prefs.treePalSettings.yPosition);
	}
	else
	{
		Prefs.treePalSettings.xPosition = Tpal->pos().x();
		Prefs.treePalSettings.yPosition = Tpal->pos().y();
		Tpal->hide();
	}
	toolMenu->setItemChecked(viewTpal, visible);
}

void ScribusApp::ToggleTpal()
{
	setTpal(!Tpal->isVisible());
	PalettesStat[0] = false;
}

void ScribusApp::setBpal(bool visible)
{
	if (visible)
	{
		ScBook->show();
		ScBook->move(Prefs.scrapPalSettings.xPosition, Prefs.scrapPalSettings.yPosition);
		ScBook->resize(Prefs.scrapPalSettings.width, Prefs.scrapPalSettings.height);
	}
	else
	{
		Prefs.scrapPalSettings.xPosition = ScBook->pos().x();
		Prefs.scrapPalSettings.yPosition = ScBook->pos().y();
		Prefs.scrapPalSettings.width = ScBook->size().width();
		Prefs.scrapPalSettings.height = ScBook->size().height();
		ScBook->hide();
	}
	toolMenu->setItemChecked(viewBpal, visible);
}

void ScribusApp::ToggleBpal()
{
	setBpal(!ScBook->isVisible());
	PalettesStat[0] = false;
}

void ScribusApp::setLpal(bool visible)
{
	if (visible)
	{
		if (HaveDoc)
			Lpal->setLayers(&doc->Layers, &doc->ActiveLayer);
		Lpal->show();
		Lpal->move(Prefs.layerPalSettings.xPosition, Prefs.layerPalSettings.yPosition);
	}
	else
	{
		Prefs.layerPalSettings.xPosition = Lpal->pos().x();
		Prefs.layerPalSettings.yPosition = Lpal->pos().y();
		Lpal->hide();
	}
	toolMenu->setItemChecked(viewLpal, visible);
}

void ScribusApp::ToggleLpal()
{
	setLpal(!Lpal->isVisible());
	PalettesStat[0] = false;
}

void ScribusApp::setSepal(bool visible)
{
	if (visible)
	{
		Sepal->show();
		Sepal->move(Prefs.pagePalSettings.xPosition, Prefs.pagePalSettings.yPosition);
	}
	else
	{
		Prefs.pagePalSettings.xPosition = Sepal->pos().x();
		Prefs.pagePalSettings.yPosition = Sepal->pos().y();
		Prefs.SepalT = Sepal->TemplList->Thumb;
		Prefs.SepalN = Sepal->PageView->Namen;
		Sepal->hide();
	}
	toolMenu->setItemChecked(viewSepal, visible);
}

void ScribusApp::ToggleSepal()
{
	setSepal(!Sepal->isVisible());
	PalettesStat[0] = false;
}

void ScribusApp::setBookpal(bool visible)
{
	if (visible)
	{
		BookPal->show();
		BookPal->move(Prefs.bookmPalSettings.xPosition, Prefs.bookmPalSettings.yPosition);
	}
	else
	{
		Prefs.bookmPalSettings.xPosition = BookPal->pos().x();
		Prefs.bookmPalSettings.yPosition = BookPal->pos().y();
		BookPal->hide();
	}
	toolMenu->setItemChecked(viewBopal, visible);
}

void ScribusApp::ToggleBookpal()
{
	setBookpal(!BookPal->isVisible());
	PalettesStat[0] = false;
}

void ScribusApp::ToggleUndoPalette()
{
	setUndoPalette(!undoPalette->isVisible());
	PalettesStat[0] = false;
}

void ScribusApp::setTools(bool visible)
{
	if (visible)
	{
		WerkTools->show();
		WerkTools->Sichtbar = true;
	}
	else
	{
		WerkTools->hide();
		WerkTools->Sichtbar = false;
	}
	toolMenu->setItemChecked(toolbarMenuTools, visible);
}

void ScribusApp::ToggleTools()
{
	setTools(!WerkTools->Sichtbar);
}

void ScribusApp::setPDFTools(bool visible)
{
	if (visible)
	{
		WerkToolsP->show();
		WerkToolsP->Sichtbar = true;
	}
	else
	{
		WerkToolsP->hide();
		WerkToolsP->Sichtbar = false;
	}
	toolMenu->setItemChecked(toolbarMenuPDFTools, visible);
}

void ScribusApp::TogglePDFTools()
{
	setPDFTools(!WerkToolsP->Sichtbar);
}

void ScribusApp::TogglePics()
{
	doc->guidesSettings.showPic = !doc->guidesSettings.showPic;
	viewMenu->setItemChecked(Bilder, doc->guidesSettings.showPic);
	for (uint b=0; b<doc->Items.count(); ++b)
	{
		if (doc->Items.at(b)->PType == 2)
			doc->Items.at(b)->PicArt = doc->guidesSettings.showPic;
	}
	view->DrawNew();
}

void ScribusApp::ToggleAllGuides()
{
	if (GuidesStat[0])
	{
		GuidesStat[0] = false;
		doc->guidesSettings.marginsShown = GuidesStat[1];
		doc->guidesSettings.framesShown = GuidesStat[2];
		doc->guidesSettings.gridShown = GuidesStat[3];
		doc->guidesSettings.guidesShown = GuidesStat[4];
		doc->guidesSettings.baseShown = GuidesStat[5];
		doc->guidesSettings.linkShown = GuidesStat[6];
		ToggleMarks();
		ToggleFrames();
		ToggleRaster();
		ToggleGuides();
		ToggleBase();
		ToggleTextLinks();
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
		doc->guidesSettings.marginsShown = false;
		doc->guidesSettings.framesShown = false;
		doc->guidesSettings.gridShown = false;
		doc->guidesSettings.guidesShown = false;
		doc->guidesSettings.baseShown = false;
		doc->guidesSettings.linkShown = false;
		viewMenu->setItemChecked(Markers, doc->guidesSettings.marginsShown);
		viewMenu->setItemChecked(FrameDr, doc->guidesSettings.framesShown);
		viewMenu->setItemChecked(Ras, doc->guidesSettings.gridShown);
		viewMenu->setItemChecked(Guide, doc->guidesSettings.guidesShown);
		viewMenu->setItemChecked(Base, doc->guidesSettings.baseShown);
		viewMenu->setItemChecked(textLinks, doc->guidesSettings.linkShown);
	}
	view->DrawNew();
}

void ScribusApp::ToggleMarks()
{
	GuidesStat[0] = false;
	doc->guidesSettings.marginsShown = !doc->guidesSettings.marginsShown;
	viewMenu->setItemChecked(Markers, doc->guidesSettings.marginsShown);
	view->DrawNew();
}

void ScribusApp::ToggleFrames()
{
	GuidesStat[0] = false;
	doc->guidesSettings.framesShown = !doc->guidesSettings.framesShown;
	viewMenu->setItemChecked(FrameDr, doc->guidesSettings.framesShown);
	view->DrawNew();
}

void ScribusApp::ToggleRaster()
{
	GuidesStat[0] = false;
	doc->guidesSettings.gridShown = !doc->guidesSettings.gridShown;
	viewMenu->setItemChecked(Ras, doc->guidesSettings.gridShown);
	view->DrawNew();
}

void ScribusApp::ToggleGuides()
{
	GuidesStat[0] = false;
	doc->guidesSettings.guidesShown = !doc->guidesSettings.guidesShown;
	viewMenu->setItemChecked(Guide, doc->guidesSettings.guidesShown);
	view->DrawNew();
}

void ScribusApp::ToggleBase()
{
	GuidesStat[0] = false;
	doc->guidesSettings.baseShown = !doc->guidesSettings.baseShown;
	viewMenu->setItemChecked(Base, doc->guidesSettings.baseShown);
	view->DrawNew();
}

void ScribusApp::ToggleTextLinks()
{
	GuidesStat[0] = false;
	doc->guidesSettings.linkShown = !doc->guidesSettings.linkShown;
	viewMenu->setItemChecked(textLinks, doc->guidesSettings.linkShown);
	view->DrawNew();
}

void ScribusApp::ToggleURaster()
{
	doc->useRaster = !doc->useRaster;
	viewMenu->setItemChecked(uRas, doc->useRaster);
}

void ScribusApp::ToggleUGuides()
{
	doc->SnapGuides = !doc->SnapGuides;
	viewMenu->setItemChecked(uGuide, doc->SnapGuides);
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
		Npal->setDoc(doc, view);
		Npal->MoveN();
		Npal->HaveNode(false, false);
		Npal->MoveNode->setOn(true);
		Npal->show();
		Npal->move(Prefs.nodePalSettings.xPosition, Prefs.nodePalSettings.yPosition);
		doc->EditClipMode = 0;
		doc->EditClip = true;
		WerkTools->Select->setEnabled(false);
		WerkTools->Rotiere->setEnabled(false);
		WerkTools->Textedit->setEnabled(false);
		WerkTools->Textedit2->setEnabled(false);
		WerkTools->Zoom->setEnabled(false);
		WerkTools->Texte->setEnabled(false);
		WerkTools->BildB->setEnabled(false);
		WerkTools->TableB->setEnabled(false);
		WerkTools->Rechteck->setEnabled(false);
		WerkTools->Linien->setEnabled(false);
		WerkTools->Polygon->setEnabled(false);
		WerkTools->KetteEin->setEnabled(false);
		WerkTools->KetteAus->setEnabled(false);
		WerkTools->Measure->setEnabled(false);
		WerkToolsP->PDFTool->setEnabled(false);
		WerkToolsP->PDFaTool->setEnabled(false);
		ObjMenu->setItemEnabled(Loesch, false);
		if (view->SelItem.count() != 0)
		{
			PageItem* b = view->SelItem.at(0);
			view->MarkClip(b);
			if (b->ContourLine.size() == 0)
				Npal->EditCont->setEnabled(false);
			else
				Npal->EditCont->setEnabled(true);
			Npal->ResetCont->setEnabled(false);
			Npal->PolyStatus(b->PType, b->PoLine.size());
		}
	}
	ShapeMenu->setItemChecked(ShapeEdit, doc->EditClip);
}

void ScribusApp::NoFrameEdit()
{
	Prefs.nodePalSettings.xPosition = Npal->pos().x();
	Prefs.nodePalSettings.yPosition = Npal->pos().y();
	Npal->hide();
	WerkTools->Select->setEnabled(true);
	WerkTools->Select->setOn(true);
	WerkTools->Zoom->setEnabled(true);
	WerkTools->Texte->setEnabled(true);
	WerkTools->BildB->setEnabled(true);
	WerkTools->TableB->setEnabled(true);
	WerkTools->Rechteck->setEnabled(true);
	WerkTools->Linien->setEnabled(true);
	WerkTools->Polygon->setEnabled(true);
	WerkToolsP->PDFTool->setEnabled(true);
	WerkToolsP->PDFaTool->setEnabled(true);
	WerkTools->Textedit->setOn(false);
	WerkTools->Textedit2->setOn(false);
	WerkTools->Measure->setEnabled(true);
	ObjMenu->setItemEnabled(Loesch, true);
	ShapeMenu->setItemChecked(ShapeEdit, false);
	if (HaveDoc)
	{
		doc->EditClip = false;
		view->EditContour = false;
		if (view->SelItem.count() != 0)
		{
			HaveNewSel(view->SelItem.at(0)->PType);
			view->DrawNew();
		}
		else
			HaveNewSel(-1);
	}
}

void ScribusApp::slotSelect()
{
	WerkTools->Select->setOn(true);
	WerkTools->Rotiere->setOn(false);
	WerkTools->Textedit->setOn(false);
	WerkTools->Textedit2->setOn(false);
	WerkTools->Zoom->setOn(false);
	WerkTools->Texte->setOn(false);
	WerkTools->BildB->setOn(false);
	WerkTools->TableB->setOn(false);
	WerkTools->Rechteck->setOn(false);
	WerkTools->Linien->setOn(false);
	WerkTools->Polygon->setOn(false);
	WerkTools->KetteEin->setOn(false);
	WerkTools->KetteAus->setOn(false);
	WerkToolsP->PDFTool->setOn(false);
	WerkToolsP->PDFaTool->setOn(false);
	WerkTools->Measure->setOn(false);
	setAppMode(NormalMode);
}

void ScribusApp::ModeFromTB(int m)
{
	if (m == StartStoryEditor)
	{
		slotStoryEditor();
		slotSelect();
		return;
	}
	if (m == LinkFrames)
		doc->ElemToLink = view->SelItem.at(0);
	if (doc->appMode == DrawBezierLine)
		return;
	setAppMode(m);
}

void ScribusApp::setAppMode(int mode)
{
	PageItem *b;
	setActiveWindow();
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
			b = view->SelItem.at(0);
		else
			b = 0;
		int oldMode = doc->appMode;
		doc->appMode = mode;
		if (oldMode == MeasurementTool)
			disconnect(view, SIGNAL(MVals(double, double, double, double, double, double, int )), MaPal, SLOT(setValues(double, double, double, double, double, double, int )));
		if (oldMode == EditMode)
		{
			disconnect(doc->CurTimer, SIGNAL(timeout()), view, SLOT(BlinkCurs()));
			doc->CurTimer->stop();
			view->LE->setFocusPolicy(QWidget::ClickFocus);
			view->PGS->PageCombo->setFocusPolicy(QWidget::ClickFocus);
			delete doc->CurTimer;
			doc->CurTimer = 0;
			editMenu->setItemEnabled(edid4, 0);
			if (PluginMap.contains(1))
				extraMenu->setItemEnabled(PluginMap[1].MenuID, 0);
			view->slotDoCurs(false);
			if (b != 0)
			{
				view->RefreshItem(b);
				menuBar()->setItemEnabled(Stm, 1);
				menuBar()->setItemEnabled(Obm, 1);
			}
			view->HR->ItemPosValid = false;
			view->HR->repX = false;
			view->HR->repaint();
		}
		if (mode == EditMode)
		{
			view->LE->setFocusPolicy(QWidget::NoFocus);
			view->PGS->PageCombo->setFocusPolicy(QWidget::NoFocus);
			if (b != 0)
			{
				if ((b->PType == 6) || (b->PType == 7) || (b->PType == 8))
				{
					doc->appMode = 1;
					ToggleFrameEdit();
					return;
				}
				setTBvals(b);
			}
			editMenu->setItemEnabled(edid3, 0);
			if (PluginMap.contains(1))
				extraMenu->setItemEnabled(PluginMap[1].MenuID, 1);
			if (!Buffer2.isNull())
			{
				if (!Buffer2.startsWith("<SCRIBUSELEM"))
				{
					BuFromApp = false;
					editMenu->setItemEnabled(edid3, 1);
				}
			}
			WerkTools->Select->setOn(false);
			WerkTools->Textedit->setOn(true);
			WerkTools->Textedit2->setOn(false);
			view->slotDoCurs(true);
			menuBar()->setItemEnabled(Obm, 0);
			menuBar()->setItemEnabled(Stm, 0);
			doc->CurTimer = new QTimer(view);
			connect(doc->CurTimer, SIGNAL(timeout()), view, SLOT(BlinkCurs()));
			doc->CurTimer->start(500);
			if (b != 0)
			{
				if (b->HasSel)
				{
					editMenu->setItemEnabled(edid1, 1);
					editMenu->setItemEnabled(edid2, 1);
					editMenu->setItemEnabled(edid4, 1);
				}
				else
				{
					editMenu->setItemEnabled(edid1, 0);
					editMenu->setItemEnabled(edid2, 0);
					editMenu->setItemEnabled(edid4, 0);
				}
				editMenu->setItemEnabled(Sear, 1);
				view->RefreshItem(b);
			}
		}
		if (mode == DrawBezierLine)
		{
			if (view->SelItem.count() != 0)
				view->Deselect(true);
			view->FirstPoly = true;
		}
		if (mode == EditGradientVectors)
			Mpal->Cpal->gradEditButton->setOn(true);
		if (mode == MeasurementTool)
		{
			MaPal->show();
			connect(view, SIGNAL(MVals(double, double, double, double, double, double, int)), MaPal, SLOT(setValues(double, double, double, double, double, double, int )));
		}
		if (mode == Magnifier)
			qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
		else
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		if (mode == DrawShapes)
		{
			doc->SubMode = WerkTools->SubMode;
			doc->ShapeValues = WerkTools->ShapeVals;
			doc->ValCount = WerkTools->ValCount;
			Mpal->SCustom->setPixmap(Mpal->SCustom->getIconPixmap(doc->SubMode));
			SCustom->setPixmap(SCustom->getIconPixmap(doc->SubMode));
		}
		else
			doc->SubMode = -1;
		if (mode == NormalMode)
		{
			WerkTools->Select->setOn(true);
			WerkTools->Rotiere->setOn(false);
			WerkTools->Textedit->setOn(false);
			WerkTools->Textedit2->setOn(false);
			WerkTools->Zoom->setOn(false);
			WerkTools->Texte->setOn(false);
			WerkTools->BildB->setOn(false);
			WerkTools->TableB->setOn(false);
			WerkTools->Rechteck->setOn(false);
			WerkTools->Linien->setOn(false);
			WerkTools->Polygon->setOn(false);
			WerkTools->KetteEin->setOn(false);
			WerkTools->KetteAus->setOn(false);
			WerkToolsP->PDFTool->setOn(false);
			WerkToolsP->PDFaTool->setOn(false);
			WerkTools->Measure->setOn(false);
			Mpal->Cpal->gradEditButton->setOn(false);
		}
		if ((mode == LinkFrames) || (mode == UnlinkFrames))
			view->updateContents();
	}
}

void ScribusApp::Aktiv()
{
	setActiveWindow();
	raise();
}

void ScribusApp::setItemTypeStyle(int id)
{
	int b = 0;
	int a = TypeStyleMenu->indexOf(id);
	TypeStyleMenu->setItemChecked(id, !TypeStyleMenu->isItemChecked(id));
	if (a > 0)
	{
		if (a == 4)
		{
			if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(4)))
				TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(5), false);
		}
		if (a == 5)
		{
			if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(5)))
				TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(4), false);
		}
		if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(0)))
			b = 0;
		if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(1)))
			b |= 8;
		if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(2)))
			b |= 16;
		if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(3)))
			b |= 64;
		if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(4)))
			b |= 1;
		if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(5)))
			b |= 2;
		if (TypeStyleMenu->isItemChecked(TypeStyleMenu->idAt(6)))
			b |= 4;
	}
	setItemHoch(b);
}

void ScribusApp::setStilvalue(int s)
{
	uint a;
	int c = s & 127;
	doc->CurrentStyle = c;
	for (a = 0; a < TypeStyleMenu->count(); ++a)
	{
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(a), false);
	}
	if (c == 0)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(0), true);
	if (c & 8)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(1), true);
	if (c & 16)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(2), true);
	if (c & 64)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(3), true);
	if (c & 1)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(4), true);
	if (c & 2)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(5), true);
	if (c & 4)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(6), true);
	emit TextStil(s);
}

void ScribusApp::setItemHoch(int h)
{
	if (view->SelItem.count() != 0)
	{
		setActiveWindow();
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
			QListViewItemIterator itn(BookPal->BView);
			for ( ; itn.current(); ++itn)
			{
				BookMItem *ite = (BookMItem*)itn.current();
				if (ite->ItemNr == it)
				{
//					ite->Seite = a;
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
/*	if (!doc->TemplateMode)
		disconnect(doc->currentPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint))); */
	view->SelItem.clear();
	for (uint d = 0; d < doc->Items.count(); ++d)
	{
		ite = doc->Items.at(d);
		if (ite->isBookmark)
			DelBookMark(ite);
		view->SelItem.append(ite);
	}
	if (view->SelItem.count() != 0)
		view->DeleteItem();
//	disconnect(view->Pages.at(pg), SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
	view->reformPages();
	view->delPage(pg);
//	AdjustBM();
//	if (!doc->TemplateMode)
//		connect(doc->currentPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
	view->DrawNew();
	doc->OpenNodes.clear();
//	Tpal->BuildTree(view);
	if (doc->Pages.count() == 1)
	{
		pageMenu->setItemEnabled(pgmd, 0);
		pageMenu->setItemEnabled(pgmv, 0);
	}
	slotDocCh();
//	Sepal->RebuildPage();
}

void ScribusApp::DeletePage()
{
	PageItem* ite;
	NoFrameEdit();
	view->Deselect(true);
	DelPages *dia = new DelPages(this, doc->currentPage->PageNr+1, doc->Pages.count());
	if (dia->exec())
	{
		view->SelItem.clear();
//		if (!doc->TemplateMode)
//			disconnect(doc->currentPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
		for (int a = dia->getToPage()-1; a >= dia->getFromPage()-1; a--)
		{
/*			disconnect(view->Pages.at(pg), SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint))); */
			for (uint d = 0; d < doc->Items.count(); ++d)
			{
				ite = doc->Items.at(d);
				if (ite->OwnPage == a)
				{
//					if (ite->isBookmark)
//						DelBookMark(ite);
					view->SelItem.append(ite);
				}
			}
//			AdjustBM();
		}
//		if (!doc->TemplateMode)
//			connect(doc->currentPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
		if (view->SelItem.count() != 0)
			view->DeleteItem();
		for (int a = dia->getToPage()-1; a >= dia->getFromPage()-1; a--)
			view->delPage(a);
		view->reformPages();
		view->DrawNew();
//		doc->OpenNodes.clear();
//		Tpal->BuildTree(view);
		if (doc->Pages.count() == 1)
		{
			pageMenu->setItemEnabled(pgmd, 0);
			pageMenu->setItemEnabled(pgmv, 0);
		}
		slotDocCh();
//		Sepal->RebuildPage();
	}
	delete dia;
}

void ScribusApp::MovePage()
{
//	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->currentPage->PageNr+1, doc->Pages.count(), true);
	if (dia->exec())
	{
//		doc->OpenNodes = Tpal->buildReopenVals();
		int from = dia->getFromPage();
		int to = dia->getToPage();
		int wie = dia->getWhere();
		int wo = dia->getWherePage();
		if (from != wo)
			view->movePage(from-1, to, wo-1, wie);
		slotDocCh();
		view->DrawNew();
/*		AdjustBM();
		Sepal->RebuildPage();
		Tpal->BuildTree(view);
		Tpal->reopenTree(doc->OpenNodes); */
	}
	delete dia;
}

void ScribusApp::CopyPage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->currentPage->PageNr+1, doc->Pages.count(), false);
	if (dia->exec())
	{
		doc->loading = true;
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
		QMap<int,int> TableID;
		QPtrList<PageItem> TableItems;
		TableID.clear();
		TableItems.clear();
		uint oldItems = doc->Items.count();
		for (uint ite = 0; ite < oldItems; ++ite)
		{
			CopyPageItem(&Buffer, doc->Items.at(ite));
			Buffer.Xpos = Buffer.Xpos - from->Xoffset + Ziel->Xoffset;
			Buffer.Ypos = Buffer.Ypos - from->Yoffset + Ziel->Yoffset;
			view->PasteItem(&Buffer, true, true);
//			if (doc->Items.at(ite)->isBookmark)
//				AddBookMark(doc->Items.at(doc->Items.count()-1));
			PageItem* Neu = doc->Items.at(doc->Items.count()-1);
			if (Neu->isTableItem)
			{
				TableItems.append(Neu);
				TableID.insert(ite, Neu->ItemNr);
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
		Ziel->MPageNam = from->MPageNam;
		view->Deselect(true);
		doc->loading = false;
		view->DrawNew();
		slotDocCh();
//		Sepal->RebuildPage();
//		Tpal->BuildTree(view);
//		AdjustBM();
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

void ScribusApp::SetNewFont(QString nf)
{
	Aktiv();
	int a;
	QString nf2 = nf;
	if (!doc->UsedFonts.contains(nf))
	{
		if (doc->AddFont(nf, Prefs.AvailFonts[nf]->Font))
		{
			a = FontMenu->insertItem(new FmItem(nf, Prefs.AvailFonts[nf]->Font));
			FontID.insert(a, Prefs.AvailFonts[nf]->SCName);
		}
		else
		{
			if (view->SelItem.count() != 0)
			{
				PageItem *b = view->SelItem.at(0);
				nf2 = b->IFont;
			}
			Mpal->Fonts->RebuildList(&Prefs, doc);
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
	Mpal->Fonts->setCurrentText(nf);
	for (uint a = 2; a < FontMenu->count(); ++a)
	{
		df = FontID[FontMenu->idAt(a)];
		if (df == nf)
			FontMenu->setItemChecked(FontMenu->idAt(a), true);
		else
			FontMenu->setItemChecked(FontMenu->idAt(a), false);
	}
}

void ScribusApp::setItemFSize(int id)
{
	int c = SizeTMenu->indexOf(id);
	bool ok = false;
	if (c > 0)
	{
		c = SizeTMenu->text(id).left(2).toInt() * 10;
		view->chFSize(c);
	}
	else
	{
		Query* dia = new Query(this, "New", 1, 0, tr("&Size:"), tr("Size"));
		if (dia->exec())
		{
			c = qRound(dia->getEditText().toDouble(&ok) * 10);
			if ((ok) && (c < 10250) && (c > 0))
				view->chFSize(c);
			delete dia;
		}
	}
	Mpal->setSize(c);
	slotDocCh();
}

void ScribusApp::setFSizeMenu(int size)
{
	for (uint a = 0; a < SizeTMenu->count(); ++a)
	{
		SizeTMenu->setItemChecked(SizeTMenu->idAt(a), false);
		if (SizeTMenu->text(SizeTMenu->idAt(a)).left(2).toInt() == size / 10)
		{
			SizeTMenu->setItemChecked(SizeTMenu->idAt(a), true);
		}
	}
}

void ScribusApp::setItemFarbe(int id)
{
	if (view->SelItem.count() != 0)
	{
		PageItem *b = view->SelItem.at(0);
		if ((b->PType == 4) || (b->PType == 8))
			view->ItemTextBrush(ColorMenC->text(id));
		else
			view->ItemBrush(ColorMenC->text(id));
	}
	ColorMenu->activateItemAt(0);
	slotDocCh();
}

void ScribusApp::setItemShade(int id)
{
	int c = ShadeMenu->indexOf(id);
	uint a;
	bool ok = false;
	for (a = 0; a < ShadeMenu->count(); ++a)
	{
		ShadeMenu->setItemChecked(ShadeMenu->idAt(a), false);
	}
	ShadeMenu->setItemChecked(id, true);
	if (view->SelItem.count() != 0)
	{
		PageItem *b = view->SelItem.at(0);
		if (c > 0)
		{
			if ((b->PType == 4) || (b->PType == 8))
				view->ItemTextBrushS((c-1) * 10);
			else
				view->ItemBrushShade((c-1) * 10);
		}
		else
		{
			Query* dia = new Query(this, "New", 1, 0, tr("&Shade:"), tr("Shade"));
			if (dia->exec())
			{
				c = dia->getEditText().toInt(&ok);
				if (ok)
				{
					if ((b->PType == 4) || (b->PType == 8))
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
	PageItem *b;
	if (view->SelItem.count() != 0)
	{
		b = view->SelItem.at(0);
		if ((b->PType == 4) || (b->PType == 8))
		{
			if ((doc->appMode == EditMode) && (b->itemText.count() != 0))
			{
				la = b->itemText.at(QMIN(b->CPos, static_cast<int>(b->itemText.count()-1)))->ccolor;
				lb = b->itemText.at(QMIN(b->CPos, static_cast<int>(b->itemText.count()-1)))->cshade;
			}
			else
			{
				la = b->TxtFill;
				lb = b->ShTxtFill;
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
	for (a = 0; a < ShadeMenu->count(); ++a)
	{
		ShadeMenu->setItemChecked(ShadeMenu->idAt(a), false);
	}
	ShadeMenu->setItemChecked(ShadeMenu->idAt(lb/10+1), true);
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
	if (doc->TemplateMode)
		doc->MasterItems = doc->Items;
	else
		doc->DocItems = doc->Items;
	PageItem* ite;
	doc->MLineStyles = dia->TempStyles;
	for (uint d = 0; d < doc->DocItems.count(); ++d)
	{
		ite = doc->DocItems.at(d);
		if (ite->NamedLStyle != "")
		{
			if (!doc->MLineStyles.contains(ite->NamedLStyle))
				ite->NamedLStyle = dia->Replacement[ite->NamedLStyle];
		}
	}
	for (uint d1 = 0; d1 < doc->MasterItems.count(); ++d1)
	{
		ite = doc->MasterItems.at(d1);
		if (ite->NamedLStyle != "")
		{
			if (!doc->MLineStyles.contains(ite->NamedLStyle))
				ite->NamedLStyle = dia->Replacement[ite->NamedLStyle];
		}
	}
	Mpal->SetLineFormats(doc);
	view->DrawNew();
}

void ScribusApp::slotEditStyles()
{
	if (HaveDoc)
	{
		StilFormate *dia = new StilFormate(this, doc, &Prefs);
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
	ers.clear();
	ers.append(0);
	ers.append(1);
	ers.append(2);
	ers.append(3);
	ers.append(4);
	if (doc->TemplateMode)
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
				if ((doc->docParagraphStyles[a].LineSpa == dia->TempVorl[b].LineSpa) &&
					(doc->docParagraphStyles[a].Indent == dia->TempVorl[b].Indent) &&
					(doc->docParagraphStyles[a].First == dia->TempVorl[b].First) &&
					(doc->docParagraphStyles[a].textAlignment == dia->TempVorl[b].textAlignment) &&
					(doc->docParagraphStyles[a].gapBefore == dia->TempVorl[b].gapBefore) &&
					(doc->docParagraphStyles[a].gapAfter == dia->TempVorl[b].gapAfter) &&
					(doc->docParagraphStyles[a].Font == dia->TempVorl[b].Font) &&
					(doc->docParagraphStyles[a].TabValues == dia->TempVorl[b].TabValues) &&
					(doc->docParagraphStyles[a].Drop == dia->TempVorl[b].Drop) &&
					(doc->docParagraphStyles[a].DropLin == dia->TempVorl[b].DropLin) &&
					(doc->docParagraphStyles[a].FontEffect == dia->TempVorl[b].FontEffect) &&
					(doc->docParagraphStyles[a].FColor == dia->TempVorl[b].FColor) &&
					(doc->docParagraphStyles[a].FShade == dia->TempVorl[b].FShade) &&
					(doc->docParagraphStyles[a].SColor == dia->TempVorl[b].SColor) &&
					(doc->docParagraphStyles[a].SShade == dia->TempVorl[b].SShade) &&
					(doc->docParagraphStyles[a].BaseAdj == dia->TempVorl[b].BaseAdj) &&
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
	for (uint d=0; d<doc->DocItems.count(); ++d)
	{
		ite = doc->Items.at(d);
		if (ite->PType == 4)
		{
			for (uint e=0; e<ite->itemText.count(); ++e)
			{
				int cabori = ite->itemText.at(e)->cab;
				int cabneu = ers[cabori];
				if (cabori > 4)
				{
					if (cabneu > 0)
					{
						if (ite->itemText.at(e)->cfont == doc->docParagraphStyles[cabori].Font)
							ite->itemText.at(e)->cfont = dia->TempVorl[cabneu].Font;
						if (ite->itemText.at(e)->csize == doc->docParagraphStyles[cabori].FontSize)
							ite->itemText.at(e)->csize = dia->TempVorl[cabneu].FontSize;
						if ((ite->itemText.at(e)->cstyle & 127 ) == doc->docParagraphStyles[cabori].FontEffect)
						{
							ite->itemText.at(e)->cstyle &= ~127;
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
					}
					else
					{
						ite->itemText.at(e)->ccolor = ite->TxtFill;
						ite->itemText.at(e)->cshade = ite->ShTxtFill;
						ite->itemText.at(e)->cstroke = ite->TxtStroke;
						ite->itemText.at(e)->cshade2 = ite->ShTxtStroke;
						ite->itemText.at(e)->csize = ite->ISize;
						ite->itemText.at(e)->cstyle &= ~127;
						ite->itemText.at(e)->cstyle |= ite->TxTStyle;
					}
					ite->itemText.at(e)->cab = cabneu;
				}
			}
		}
	}
	for (uint d=0; d<doc->MasterItems.count(); ++d)
	{
		ite = doc->MasterItems.at(d);
		if (ite->PType == 4)
		{
			for (uint e=0; e<ite->itemText.count(); ++e)
			{
				int cabori = ite->itemText.at(e)->cab;
				int cabneu = ers[cabori];
				if (cabori > 4)
				{
					if (cabneu > 0)
					{
						if (ite->itemText.at(e)->cfont == doc->docParagraphStyles[cabori].Font)
							ite->itemText.at(e)->cfont = dia->TempVorl[cabneu].Font;
						if (ite->itemText.at(e)->csize == doc->docParagraphStyles[cabori].FontSize)
							ite->itemText.at(e)->csize = dia->TempVorl[cabneu].FontSize;
						if ((ite->itemText.at(e)->cstyle & 127 ) == doc->docParagraphStyles[cabori].FontEffect)
						{
							ite->itemText.at(e)->cstyle &= ~127;
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
					}
					else
					{
						ite->itemText.at(e)->ccolor = ite->TxtFill;
						ite->itemText.at(e)->cshade = ite->ShTxtFill;
						ite->itemText.at(e)->cstroke = ite->TxtStroke;
						ite->itemText.at(e)->cshade2 = ite->ShTxtStroke;
						ite->itemText.at(e)->csize = ite->ISize;
						ite->itemText.at(e)->cstyle &= ~127;
						ite->itemText.at(e)->cstyle |= ite->TxTStyle;
					}
					ite->itemText.at(e)->cab = cabneu;
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
							if ((chars->at(e)->cstyle & 127 ) == doc->docParagraphStyles[cabori].FontEffect)
							{
								chars->at(e)->cstyle &= ~127;
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
						}
						else
						{
							chars->at(e)->ccolor = ite->TxtFill;
							chars->at(e)->cshade = ite->ShTxtFill;
							chars->at(e)->cstroke = ite->TxtStroke;
							chars->at(e)->cshade2 = ite->ShTxtStroke;
							chars->at(e)->csize = ite->ISize;
							chars->at(e)->cstyle &= ~127;
							chars->at(e)->cstyle |= ite->TxTStyle;
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
		if (doc->docParagraphStyles[a].Font != "")
		{
			QString nf = doc->docParagraphStyles[a].Font;
			if (!doc->UsedFonts.contains(nf))
			{
				if (doc->AddFont(nf, Prefs.AvailFonts[nf]->Font))
				{
					int ff = FontMenu->insertItem(new FmItem(nf, Prefs.AvailFonts[nf]->Font));
					FontID.insert(ff, Prefs.AvailFonts[nf]->SCName);
				}
				else
					doc->docParagraphStyles[a].Font = doc->Dfont;
			}
		}
	}
	Mpal->Spal->updateFormatList();
	view->DrawNew();
	slotDocCh();
}

void ScribusApp::setItemTextAli(int id)
{
	int a = AliMenu->indexOf(id);
	setNewAbStyle(a);
}

void ScribusApp::setNewAbStyle(int a)
{
	setActiveWindow();
	if (HaveDoc)
	{
		view->SetAbStyle(a);
		doc->currentParaStyle = a;
		Mpal->setAli(a);
		PageItem *b = view->SelItem.at(0);
		setTBvals(b);
		slotDocCh();
	}
}

void ScribusApp::setAbsValue(int a)
{
	doc->currentParaStyle = a;
	Mpal->setAli(a);
	for (int b = 0; b < 5; ++b)
	{
		AliMenu->setItemChecked(AliMenu->idAt(b), false);
	}
}

void ScribusApp::slotEditColors()
{
	int a;
	uint c, d;
	ColorList edc;
	QMap<QString,QString> ers;
	PageItem *ite;
	QColor tmpc;
	if (HaveDoc)
		edc = doc->PageColors;
	else
		edc = Prefs.DColors;
	Farbmanager* dia = new Farbmanager(this, edc, HaveDoc, Prefs.DColorSet, Prefs.CustomColorSets);
	if (dia->exec())
	{
		if (HaveDoc)
		{
			slotDocCh();
			doc->PageColors = dia->EditColors;
			Mpal->Cpal->SetColors(doc->PageColors);
			Mpal->updateCList();
			ColorList::Iterator it;
			ColorMenC->clear();
			QPixmap pm = QPixmap(15, 15);
			a = 0;
			ColorMenC->insertItem( tr("None"));
			for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
			{
				pm.fill(doc->PageColors[it.key()].getRGBColor());
				ColorMenC->insertItem(pm, it.key());
				if (it.key() == doc->Dbrush)
					ColorMenC->setCurrentItem(a);
				a++;
			}
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
						if ((ite->PType == 4) || (ite->PType == 8))
						{
							for (d=0; d<ite->itemText.count(); ++d)
							{
								if (it.key() == ite->itemText.at(d)->ccolor)
									ite->itemText.at(d)->ccolor = it.data();
								if (it.key() == ite->itemText.at(d)->cstroke)
									ite->itemText.at(d)->cstroke = it.data();
							}
						}
						if (it.key() == ite->Pcolor)
							ite->Pcolor = it.data();
						if (it.key() == ite->Pcolor2)
							ite->Pcolor2 = it.data();
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
						if ((ite->PType == 4) || (ite->PType == 8))
						{
							for (d=0; d<ite->itemText.count(); ++d)
							{
								if (it.key() == ite->itemText.at(d)->ccolor)
									ite->itemText.at(d)->ccolor = it.data();
								if (it.key() == ite->itemText.at(d)->cstroke)
									ite->itemText.at(d)->cstroke = it.data();
							}
						}
						if (it.key() == ite->Pcolor)
							ite->Pcolor = it.data();
						if (it.key() == ite->Pcolor2)
							ite->Pcolor2 = it.data();
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
			view->DrawNew();
		}
		else
		{
			Prefs.DColors = dia->EditColors;
			Prefs.DColorSet = dia->LoadColSet->text();
			Mpal->Cpal->SetColors(Prefs.DColors);
		}
	}
	if (!HaveDoc)
		Prefs.CustomColorSets = dia->CColSet;
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
	setActiveWindow();
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
	setActiveWindow();
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
			PageItem *b = view->SelItem.at(0);
			b->fill_gradient = Mpal->Cpal->gradEdit->Preview->fill_gradient;
			view->RefreshItem(b);
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
	PageItem *b = view->SelItem.at(0);
	switch (f)
	{
	case 0:
		view->SetRectFrame(b);
		break;
	case 1:
		view->SetOvalFrame(b);
		break;
	default:
		view->SetFrameShape(b, c, vals);
		b->FrameType = f+2;
		break;
	}
	Mpal->SetCurItem(b);
	view->RefreshItem(b);
	slotDocCh();
}

void ScribusApp::DeleteObjekt()
{
	if (!doc->EditClip)
		view->DeleteItem();
}

void ScribusApp::Objekt2Back()
{
	view->ToBack();
}

void ScribusApp::Objekt2Front()
{
	view->ToFront();
}

void ScribusApp::ObjektRaise()
{
	view->RaiseItem();
}

void ScribusApp::ObjektLower()
{
	view->LowerItem();
}

void ScribusApp::ObjektDup()
{
	slotSelect();
	slotEditCopy();
	view->Deselect(true);
	slotEditPaste();
	for (uint b=0; b<view->SelItem.count(); ++b)
	{
		view->SelItem.at(b)->Locked = false;
		view->MoveItem(DispX, DispY, view->SelItem.at(b));
	}
}

void ScribusApp::ObjektDupM()
{
	slotSelect();
	NoFrameEdit();
	Mdup *dia = new Mdup(this, DispX * UmReFaktor, DispY * UmReFaktor, doc->docUnitIndex);
	if (dia->exec())
	{
		int anz = dia->Ncopies->value();
		double dH = dia->ShiftH->value() / UmReFaktor;
		double dV = dia->ShiftV->value() / UmReFaktor;
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
					view->SelItem.at(b)->Locked = false;
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
	}
	delete dia;
}

void ScribusApp::SelectFromOutl(int Page, int Item)
{
	NoFrameEdit();
	setActiveWindow();
	view->Deselect(true);
	view->GotoPage(Page);
	view->SelectItemNr(Item);
	if (view->SelItem.count() != 0)
	{
		PageItem *b = view->SelItem.at(0);
	 // jjsa 23-05-2004 added for centering of rotated objects
		if ( b->Rot != 0.0 )
		{
			double y1 = sin(b->Rot/180.*M_PI) * b->Width;
			double x1 = cos(b->Rot/180.*M_PI) * b->Width;
			double y2 = sin((b->Rot+90.)/180*M_PI) * b->Height;
			double x2 = cos((b->Rot+90.)/180*M_PI) * b->Height;
			double mx = b->Xpos + ((x1 + x2)/2.0);
			double my = b->Ypos + ((y1 + y2)/2.0);
			view->SetCCPo(static_cast<int>(mx), static_cast<int>(my));
		}
		else
			view->SetCCPo(static_cast<int>(b->Xpos + b->Width/2), static_cast<int>(b->Ypos + b->Height/2));
	}
}

void ScribusApp::SelectFromOutlS(int Page)
{
	NoFrameEdit();
	setActiveWindow();
	view->Deselect(true);
	view->GotoPage(Page);
}

void ScribusApp::InfoDoc()
{
	DocInfos *dia = new DocInfos(this, doc);
	if (dia->exec())
	{
		doc->DocAutor = dia->authorEdit->text();
		doc->DocTitel = dia->titleEdit->text();
		doc->DocComments = dia->descriptionEdit->text();
		doc->DocKeyWords = dia->keywordsEdit->text();
		doc->DocPublisher = dia->publisherEdit->text();
		doc->DocDate = dia->dateEdit->text();
		doc->DocType = dia->typeEdit->text();
		doc->DocFormat = dia->formatEdit->text();
		doc->DocIdent = dia->identifierEdit->text();
		doc->DocSource = dia->sourceEdit->text();
		doc->DocLangInfo = dia->languageEdit->text();
		doc->DocRelation = dia->relationEdit->text();
		doc->DocCover = dia->coverageEdit->text();
		doc->DocRights = dia->rightsEdit->text();
		doc->DocContrib = dia->contributorsEdit->text();
		slotDocCh();
	}
	delete dia;
}

void ScribusApp::ObjektAlign()
{
	double xdp, ydp;
	bool xa, ya, Vth, Vtv;
	int xart, yart, ein;
	ein = doc->docUnitIndex;
	NoFrameEdit();
	view->BuildAObj();
	Align *dia = new Align(this, view->AObjects.count(), ein, doc, view);
	connect(dia, SIGNAL(ApplyDist(bool, bool, bool, bool, double, double, int, int)),
	        this, SLOT(DoAlign(bool, bool, bool, bool, double, double, int, int)));
	if (dia->exec())
	{
		xdp = dia->AHor->value() / UmReFaktor;
		xa = (dia->CheckH->isChecked() || dia->VerteilenH->isChecked());
		ydp = dia->AVert->value() / UmReFaktor;
		ya = (dia->CheckV->isChecked() || dia->VerteilenV->isChecked());
		xart = dia->VartH->currentItem();
		yart = dia->VartV->currentItem();
		Vth = dia->VerteilenH->isChecked();
		Vtv = dia->VerteilenV->isChecked();
		view->AlignObj(xa, ya, Vth, Vtv, xdp, ydp, xart, yart);
		slotDocCh();
		HaveNewSel(view->SelItem.at(0)->PType);
	}
	delete dia;
}

void ScribusApp::DoAlign(bool xa, bool ya, bool Vth, bool Vtv, double xdp, double ydp, int xart, int yart)
{
	view->AlignObj(xa, ya, Vth, Vtv, xdp, ydp, xart, yart);
	slotDocCh();
}

void ScribusApp::buildFontMenu()
{
	FontID.clear();
	FontMenu->clear();
	int a;
	QString b = " ";
	SCFontsIterator it(Prefs.AvailFonts);
	FontSub = new FontCombo(0, &Prefs);
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
			if (it3.key() == doc->Dfont)
				FontMenu->setItemChecked(a, true);
			FontID.insert(a, it3.key());
		}
	}
	connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
}

const bool ScribusApp::GetAllFonts()
{
	Prefs.AvailFonts.GetFonts(PrefsPfad);
	if (Prefs.AvailFonts.isEmpty())
		return true;
	return false;
}

void ScribusApp::slotFontOrg()
{
	uint a;
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor), true);
	FontPrefs *dia = new FontPrefs(this, Prefs.AvailFonts, HaveDoc, &Prefs, PrefsPfad);
	connect(dia, SIGNAL(ReReadPrefs()), this, SLOT(ReadPrefs()));
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
	if (dia->exec())
	{
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor), true);
		a = 0;
		SCFontsIterator it(Prefs.AvailFonts);
		for ( ; it.current() ; ++it)
		{
			it.current()->EmbedPS = dia->FlagsPS.at(a)->isChecked();
			it.current()->UseFont = dia->FlagsUse.at(a)->isChecked();
			it.current()->Subset = dia->FlagsSub.at(a)->isChecked();
			a++;
		}
		a = 0;
		QMap<QString,QString>::Iterator itfsu;
		Prefs.GFontSub.clear();
		for (itfsu = dia->RList.begin(); itfsu != dia->RList.end(); ++itfsu)
		{
			Prefs.GFontSub[itfsu.key()] = dia->FlagsRepl.at(a)->currentText();
			a++;
		}
		qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
	}
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor), true);
	if (HaveDoc)
	{
		FontSub->RebuildList(&Prefs, doc);
		Mpal->Fonts->RebuildList(&Prefs, doc);
	}
	else
	{
		FontSub->RebuildList(&Prefs, 0);
		Mpal->Fonts->RebuildList(&Prefs, 0);
	}
	disconnect(dia, SIGNAL(ReReadPrefs()), this, SLOT(ReadPrefs()));
	delete dia;
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
}

void ScribusApp::slotPrefsOrg()
{
	void *mo;
	const char *error;
	bool zChange = false;
	typedef Preferences* (*sdem)(QWidget *d, ApplicationPrefs *Vor);
	sdem demo;
	QString pfad = LIBDIR;
#if defined(__hpux)
	pfad += "libs/libpreferences.sl";
#else
	pfad += "libs/libpreferences.so";
#endif
	mo = dlopen(pfad, RTLD_LAZY);
	if (!mo)
	{
		std::cout << "Can't find Plug-in" << endl;
		return;
	}
	dlerror();
	demo = (sdem)dlsym(mo, "Run");
	if ((error = dlerror()) != NULL)
	{
		std::cout << "Can't find Symbol" << endl;
		dlclose(mo);
		return;
	}
	Preferences *dia = (*demo)(this, &Prefs);
	if (dia->exec())
	{
		Prefs.AppFontSize = dia->GFsize->value();
		Prefs.Wheelval = dia->SpinBox3->value();
		Prefs.RecentDCount = dia->Recen->value();
		Prefs.DocDir = dia->Docs->text();
		DocDir = Prefs.DocDir;
		Prefs.ProfileDir = dia->ProPfad->text();
		Prefs.ScriptDir = dia->ScriptPfad->text();
		Prefs.TemplateDir = dia->TemplateDir->text();
		switch (dia->PreviewSize->currentItem())
		{
		case 0:
			Prefs.PSize = 40;
			break;
		case 1:
			Prefs.PSize = 60;
			break;
		case 2:
			Prefs.PSize = 80;
			break;
		}
		Prefs.SaveAtQ = dia->SaveAtQuit->isChecked();
		ScBook->BibWin->RebuildView();
		ScBook->AdjustMenu();
		Prefs.guiLanguage=dia->selectedGUILang;
		if (Prefs.GUI != dia->GUICombo->currentText())
		{
			Prefs.GUI = dia->GUICombo->currentText();
			qApp->setStyle(QStyleFactory::create(Prefs.GUI));
		}
		QFont apf = qApp->font();
		apf.setPointSize(Prefs.AppFontSize);
		qApp->setFont(apf,true);
		dia->polyWidget->getValues(&Prefs.PolyC, &Prefs.PolyFd, &Prefs.PolyF, &Prefs.PolyS, &Prefs.PolyR);
		Prefs.PageFormat = dia->GZComboF->currentItem();
		Prefs.Ausrichtung = dia->GZComboO->currentItem();
		Prefs.PageBreite = dia->Pagebr;
		Prefs.PageHoehe = dia->Pageho;
		Prefs.RandOben = dia->RandT;
		Prefs.RandUnten = dia->RandB;
		Prefs.RandLinks = dia->RandL;
		Prefs.RandRechts = dia->RandR;
		Prefs.DoppelSeiten = dia->facingPages->isChecked();
		Prefs.ErsteLinks = dia->Linkszuerst->isChecked();
		Prefs.gimp_exe = dia->GimpName->text();
		Prefs.gs_antiGraph = dia->GSantiGraph->isChecked();
		Prefs.gs_antiText = dia->GSantiText->isChecked();
		Prefs.gs_exe = dia->GSName->text();
		Prefs.ClipMargin = dia->ClipMarg->isChecked();
		Prefs.GCRMode = dia->DoGCR->isChecked();
		Prefs.Before = dia->inBackground->isChecked();
		Prefs.marginColored = dia->checkUnprintable->isChecked();
		if (Prefs.DisScale != dia->DisScale)
		{
			Prefs.DisScale = dia->DisScale;
			zChange = true;
		}
		Mpal->Cpal->UseTrans(true);
		Prefs.docUnitIndex = dia->UnitCombo->currentItem();
		switch (Prefs.docUnitIndex)
		{
		case 0:
			UmReFaktor = 1.0;
			break;
		case 1:
			UmReFaktor = 1.0 / 72.0 * 25.4;
			break;
		case 2:
			UmReFaktor = 1.0 / 72.0;
			break;
		case 3:
			UmReFaktor = 1.0 / 12.0;
			break;
		}
		Prefs.ScratchBottom = dia->bottomScratch->value() / UmReFaktor;
		Prefs.ScratchLeft = dia->leftScratch->value() / UmReFaktor;
		Prefs.ScratchRight = dia->rightScratch->value() / UmReFaktor;
		Prefs.ScratchTop = dia->topScratch->value() / UmReFaktor;
		Prefs.DpapColor = dia->colorPaper;
		Prefs.DefFont = dia->fontComboText->currentText();
		Prefs.DefSize = dia->sizeComboText->currentText().left(2).toInt() * 10;
		Prefs.guidesSettings.marginsShown = dia->tabGuides->checkMargin->isChecked();
		Prefs.guidesSettings.framesShown = dia->tabGuides->checkFrame->isChecked();
		Prefs.guidesSettings.gridShown = dia->tabGuides->checkGrid->isChecked();
		Prefs.guidesSettings.guidesShown = dia->tabGuides->checkGuides->isChecked();
		Prefs.guidesSettings.baseShown = dia->tabGuides->checkBaseline->isChecked();
		Prefs.guidesSettings.showPic = dia->tabGuides->checkPictures->isChecked();
		Prefs.guidesSettings.linkShown = dia->tabGuides->checkLink->isChecked();
		Prefs.guidesSettings.grabRad = dia->tabGuides->grabDistance->value();
		Prefs.guidesSettings.guideRad = dia->tabGuides->snapDistance->value() / UmReFaktor;
		Prefs.guidesSettings.minorGrid = dia->tabGuides->minorSpace->value() / UmReFaktor;
		Prefs.guidesSettings.majorGrid = dia->tabGuides->majorSpace->value() / UmReFaktor;
		Prefs.guidesSettings.minorColor = dia->tabGuides->colorMinorGrid;
		Prefs.guidesSettings.majorColor = dia->tabGuides->colorMajorGrid;
		Prefs.guidesSettings.margColor = dia->tabGuides->colorMargin;
		Prefs.guidesSettings.guideColor = dia->tabGuides->colorGuides;
		Prefs.guidesSettings.baseColor = dia->tabGuides->colorBaselineGrid;
		Prefs.typographicSetttings.valueSuperScript = dia->tabTypo->superDisplacement->value();
		Prefs.typographicSetttings.scalingSuperScript = dia->tabTypo->superScaling->value();
		Prefs.typographicSetttings.valueSubScript = dia->tabTypo->subDisplacement->value();
		Prefs.typographicSetttings.scalingSubScript = dia->tabTypo->subScaling->value();
		Prefs.typographicSetttings.valueSmallCaps = dia->tabTypo->capsScaling->value();
		Prefs.typographicSetttings.autoLineSpacing = dia->tabTypo->autoLine->value();
		Prefs.typographicSetttings.valueBaseGrid = dia->tabTypo->baseGrid->value() / UmReFaktor;
		Prefs.typographicSetttings.offsetBaseGrid = dia->tabTypo->baseOffset->value() / UmReFaktor;
		Prefs.Dpen = dia->colorComboLineShape->currentText();
		if (Prefs.Dpen == tr("None"))
			Prefs.Dpen = "None";
		Prefs.DpenText = dia->colorComboText->currentText();
		if (Prefs.DpenText == tr("None"))
			Prefs.DpenText = "None";
		Prefs.DstrokeText = dia->colorComboStrokeText->currentText();
		if (Prefs.DstrokeText == tr("None"))
			Prefs.DstrokeText = "None";
		Prefs.DCols = dia->columnsText->value();
		Prefs.DGap = dia->gapText->value() / UmReFaktor;
		Prefs.Dbrush = dia->comboFillShape->currentText();
		if (Prefs.Dbrush == tr("None"))
			Prefs.Dbrush = "None";
		Prefs.Dshade = dia->shadingFillShape->value();
		Prefs.Dshade2 = dia->shadingLineShape->value();
		switch (dia->comboStyleShape->currentItem())
		{
		case 0:
			Prefs.DLineArt = SolidLine;
			break;
		case 1:
			Prefs.DLineArt = DashLine;
			break;
		case 2:
			Prefs.DLineArt = DotLine;
			break;
		case 3:
			Prefs.DLineArt = DashDotLine;
			break;
		case 4:
			Prefs.DLineArt = DashDotDotLine;
			break;
		}
		Prefs.Dwidth = dia->lineWidthShape->value();
		Prefs.DpenLine = dia->colorComboLine->currentText();
		if (Prefs.DpenLine == tr("None"))
			Prefs.DpenLine = "None";
		Prefs.DshadeLine = dia->shadingLine->value();
		switch (dia->comboStyleLine->currentItem())
		{
		case 0:
			Prefs.DLstyleLine = SolidLine;
			break;
		case 1:
			Prefs.DLstyleLine = DashLine;
			break;
		case 2:
			Prefs.DLstyleLine = DotLine;
			break;
		case 3:
			Prefs.DLstyleLine = DashDotLine;
			break;
		case 4:
			Prefs.DLstyleLine = DashDotDotLine;
			break;
		}
		Prefs.DwidthLine = dia->lineWidthLine->value();
		Prefs.DstartArrow = dia->startArrow->currentItem();
		Prefs.DendArrow = dia->endArrow->currentItem();
		Prefs.MagMin = dia->minimumZoom->value();
		Prefs.MagMax = dia->maximumZoom->value();
		Prefs.MagStep = dia->zoomStep->value();
		Prefs.DbrushPict = dia->comboFillImage->currentText();
		if (Prefs.DbrushPict == tr("None"))
			Prefs.DbrushPict = "None";
		Prefs.ShadePict = dia->shadingFillImage->value();
		Prefs.ScaleX = static_cast<double>(dia->scalingHorizontal->value()) / 100.0;
		Prefs.ScaleY = static_cast<double>(dia->scalingVertical->value()) / 100.0;
		Prefs.ScaleType = dia->buttonGroup3->isChecked();
		Prefs.AspectRatio = dia->checkRatioImage->isChecked();
		Prefs.AutoSave = dia->ASon->isChecked();
		Prefs.AutoSaveTime = dia->ASTime->value() * 60 * 1000;
		Prefs.MinWordLen = dia->tabHyphenator->wordLen->value();
		Prefs.Language = GetLang(dia->tabHyphenator->language->currentText());
		Prefs.Automatic = !dia->tabHyphenator->verbose->isChecked();
		Prefs.AutoCheck = dia->tabHyphenator->input->isChecked();
		Prefs.HyCount = dia->tabHyphenator->maxCount->value();
		if (CMSavail)
			dia->tabColorManagement->setValues();
		GetCMSProfiles();
		Prefs.KeyActions = dia->tabKeys->getNewKeyMap();
		SetShortCut();
		SavePrefs();
		QWidgetList windows = wsp->windowList();
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
		{
			QWidget* w = wsp->windowList().at( i );
			ScribusWin* swin = (ScribusWin*)w;
			swin->doc->Before = Prefs.Before;
			swin->doc->marginColored = Prefs.marginColored;
		}
	}
	delete dia;
	dlclose(mo);
}

void ScribusApp::SavePrefs()
{
	Prefs.mainWinSettings.xPosition = abs(pos().x());
	Prefs.mainWinSettings.yPosition = abs(pos().y());
	Prefs.mainWinSettings.width = size().width();
	Prefs.mainWinSettings.height = size().height();
	Prefs.mainToolBarSettings.visible = WerkTools->isVisible();
	Prefs.pdfToolBarSettings.visible = WerkToolsP->isVisible();
	Prefs.mPaletteSettings.visible = Mpal->isVisible();
	Prefs.treePalSettings.visible = Tpal->isVisible();
	Prefs.scrapPalSettings.visible = ScBook->isVisible();
	Prefs.layerPalSettings.visible = Lpal->isVisible();
	Prefs.pagePalSettings.visible = Sepal->isVisible();
	Prefs.bookmPalSettings.visible = BookPal->isVisible();
	if ((Prefs.nodePalSettings.xPosition > QApplication::desktop()->width()-100) || (Prefs.nodePalSettings.xPosition < 0))
		Prefs.nodePalSettings.xPosition = 0;
	if ((Prefs.nodePalSettings.yPosition > QApplication::desktop()->height()-100) || (Prefs.nodePalSettings.yPosition < 0))
		Prefs.nodePalSettings.yPosition = 0;
	if (MaPal->isVisible())
	{
		Prefs.measurePalSettings.xPosition = abs(MaPal->pos().x());
		Prefs.measurePalSettings.yPosition = abs(MaPal->pos().y());
	}
	if (Mpal->isVisible())
	{
		Prefs.mPaletteSettings.xPosition = abs(Mpal->pos().x());
		Prefs.mPaletteSettings.yPosition = abs(Mpal->pos().y());
	}
	if (Tpal->isVisible())
	{
		Prefs.treePalSettings.xPosition = abs(Tpal->pos().x());
		Prefs.treePalSettings.yPosition = abs(Tpal->pos().y());
	}
	if (ScBook->isVisible())
	{
		Prefs.scrapPalSettings.xPosition = abs(ScBook->pos().x());
		Prefs.scrapPalSettings.yPosition = abs(ScBook->pos().y());
		Prefs.scrapPalSettings.width = abs(ScBook->size().width());
		Prefs.scrapPalSettings.height = abs(ScBook->size().height());
	}
	if (Sepal->isVisible())
	{
		Prefs.pagePalSettings.xPosition = abs(Sepal->pos().x());
		Prefs.pagePalSettings.yPosition = abs(Sepal->pos().y());
	}
	if (BookPal->isVisible())
	{
		Prefs.bookmPalSettings.xPosition = abs(BookPal->pos().x());
		Prefs.bookmPalSettings.yPosition = abs(BookPal->pos().y());
	}
	if (Lpal->isVisible())
	{
		Prefs.layerPalSettings.xPosition = abs(Lpal->pos().x());
		Prefs.layerPalSettings.yPosition = abs(Lpal->pos().y());
	}
	Prefs.RecentDocs.clear();
	uint max = QMIN(Prefs.RecentDCount, RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		Prefs.RecentDocs.append(RecentDocs[m]);
	}
	Prefs.PrinterName = PDef.Pname;
	Prefs.PrinterFile = PDef.Dname;
	Prefs.PrinterCommand = PDef.Command;
	ScriXmlDoc *ss = new ScriXmlDoc();
	ss->WritePref(&Prefs, PrefsPfad+"/scribus13.rc");
	delete ss;

    SavePrefsXML();
}

void ScribusApp::SavePrefsXML()
{
    if (prefsFile) {
        PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
        if (userprefsContext) {
            userprefsContext->set("gui_language",Prefs.guiLanguage);
            //continue here...
            //Prefs."blah blah" =...
        }
        prefsFile->write();
    }
}


void ScribusApp::ReadPrefs()
{
	ScriXmlDoc *ss = new ScriXmlDoc();
	bool erg = ss->ReadPref(&Prefs, PrefsPfad+"/scribus13.rc", splashScreen);
	delete ss;
	if (!erg)
		return;
	PDef.Pname = Prefs.PrinterName;
	PDef.Dname = Prefs.PrinterFile;
	PDef.Command = Prefs.PrinterCommand;
	recentMenu->clear();
	uint max = QMIN(Prefs.RecentDCount, Prefs.RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		QFileInfo fd(QString::fromUtf8(Prefs.RecentDocs[m]));
		if (fd.exists())
		{
			RecentDocs.append(QString::fromUtf8(Prefs.RecentDocs[m]));
			fileWatcher->addFile(QString::fromUtf8(Prefs.RecentDocs[m]));
			recentMenu->insertItem(QString::fromUtf8(Prefs.RecentDocs[m]));
		}
	}
	MaPal->move(Prefs.measurePalSettings.xPosition, Prefs.measurePalSettings.yPosition);
	Mpal->move(Prefs.mPaletteSettings.xPosition, Prefs.mPaletteSettings.yPosition);
	Tpal->move(Prefs.treePalSettings.xPosition, Prefs.treePalSettings.yPosition);
	Lpal->move(Prefs.layerPalSettings.xPosition, Prefs.layerPalSettings.yPosition);
	Sepal->move(Prefs.pagePalSettings.xPosition, Prefs.pagePalSettings.yPosition);
	BookPal->move(Prefs.bookmPalSettings.xPosition, Prefs.bookmPalSettings.yPosition);
	ScBook->move(Prefs.scrapPalSettings.xPosition, Prefs.scrapPalSettings.yPosition);
	ScBook->resize(Prefs.scrapPalSettings.width, Prefs.scrapPalSettings.height);
	Npal->move(Prefs.nodePalSettings.xPosition, Prefs.nodePalSettings.yPosition);
	move(Prefs.mainWinSettings.xPosition, Prefs.mainWinSettings.yPosition);
	resize(Prefs.mainWinSettings.width, Prefs.mainWinSettings.height);
	ReadPrefsXML();
}

void ScribusApp::ReadPrefsXML()
{
    if (prefsFile) {
        PrefsContext* userprefsContext = prefsFile->getContext("user_preferences");
        if (userprefsContext) {
            Prefs.guiLanguage = userprefsContext->get("gui_language","");
            //continue here...
            //Prefs."blah blah" =...
        }
    }
}


void ScribusApp::ShowSubs()
{
	QString mess;
	if (HaveGS != 0)
	{
		mess = tr("The following Programs are missing:")+"\n\n";
		if (HaveGS != 0)
			mess += tr("Ghostscript : You cannot use EPS Images")+"\n\n";
		QMessageBox::warning(this, tr("Warning"), mess, 1, 0, 0);
	}
	setTools(Prefs.mainToolBarSettings.visible);
	setPDFTools(Prefs.pdfToolBarSettings.visible);
	setMapal(Prefs.measurePalSettings.visible);
	setMpal(Prefs.mPaletteSettings.visible);
	setTpal(Prefs.treePalSettings.visible);
	setBpal(Prefs.scrapPalSettings.visible);
	setLpal(Prefs.layerPalSettings.visible);
	setSepal(Prefs.pagePalSettings.visible);
	setBookpal(Prefs.bookmPalSettings.visible);
	setActiveWindow();
	raise();
}

PSLib* ScribusApp::getPSDriver(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, ColorList DocColors, bool pdf)
{
	const char *error;
	typedef PSLib* (*sdem)(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, ColorList DocColors, bool pdf);
	sdem demo;
	QString pfad = LIBDIR;
#if defined(__hpux)
	pfad += "libs/libpostscript.sl";
#else
	pfad += "libs/libpostscript.so";
#endif
	PSDriver = dlopen(pfad, RTLD_LAZY);
	if (!PSDriver)
	{
		std::cout << "Can't find Plugin" << endl;
		return NULL;
	}
	dlerror();
	demo = (sdem)dlsym(PSDriver, "Run");
	if ((error = dlerror()) != NULL)
	{
		std::cout << "Can't find Symbol" << endl;
		dlclose(PSDriver);
		return NULL;
	}
	PSLib *dia = (*demo)(psart, AllFonts, DocFonts, DocColors, pdf);
	return dia;
}

void ScribusApp::closePSDriver()
{
	dlclose(PSDriver);
}

bool ScribusApp::DoSaveAsEps(QString fn)
{
	bool return_value = true;
	std::vector<int> pageNs;
	pageNs.push_back(doc->currentPage->PageNr+1);
	ReOrderText(doc, view);
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QMap<QString,QFont> ReallyUsed;
	ReallyUsed.clear();
	GetUsedFonts(&ReallyUsed);
	PSLib *dd = getPSDriver(false, Prefs.AvailFonts, ReallyUsed, doc->PageColors, false);
	if (dd != NULL)
	{
		if (dd->PS_set_file(fn))
			dd->CreatePS(doc, view, pageNs, false, tr("All"), true, false, false, false, Prefs.GCRMode);
		else
			return_value = false;
		delete dd;
		closePSDriver();
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	return return_value;
}

void ScribusApp::SaveAsEps()
{
	QString fna;
	if (!doc->DocName.startsWith( tr("Document")))
	{
		QFileInfo fi(doc->DocName);
		fna = fi.dirPath()+"/"+fi.baseName()+".eps";
	}
	else
	{
		QDir di = QDir();
		fna = di.currentDirPath()+"/"+doc->DocName+".eps";
	}
	QString wdir = ".";
	if (Prefs.DocDir != "")
		wdir = dirs->get("eps", Prefs.DocDir);
	else
		wdir = dirs->get("eps", ".");
	QString fn = CFileDialog( wdir, tr("Save as"), tr("EPS-Files (*.eps);;All Files (*)"), "", false, false);
	if (!fn.isEmpty())
	{
		dirs->set("eps", fn.left(fn.findRev("/")));
		if (overwrite(this, fn))
		{
			if (!DoSaveAsEps(fn))
				QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
		}
	}
}

bool ScribusApp::getPDFDriver(QString fn, QString nam, int Components, std::vector<int> &pageNs, QMap<int,QPixmap> thumbs)
{
	bool ret = false;
	const char *error;
	void *PDFDriver;
	typedef bool (*sdem)(ScribusApp *plug, QString fn, QString nam, int Components, std::vector<int> &pageNs, QMap<int,QPixmap> thumbs, QProgressBar *dia2);
	sdem demo;
	QString pfad = LIBDIR;
#if defined(__hpux)
	pfad += "libs/libpdf.sl";
#else
	pfad += "libs/libpdf.so";
#endif
	PDFDriver = dlopen(pfad, RTLD_NOW);
	if (!PDFDriver)
	{
		std::cout << "Can't find Plugin" << endl;
		return false;
	}
	dlerror();
	demo = (sdem)dlsym(PDFDriver, "Run");
	if ((error = dlerror()) != NULL)
	{
		std::cout << "Can't find Symbol" << endl;
		dlclose(PDFDriver);
		return false;
	}
	ret = (*demo)(this, fn, nam, Components, pageNs, thumbs, FProg);
	dlclose(PDFDriver);
	return ret;
}

void ScribusApp::SaveAsPDF()
{
	QString fn;
	int Components = 3;
	QString nam = "";
/*	if (BookPal->BView->childCount() == 0)
		doc->PDF_Optionen.Bookmarks = false; */
	QMap<QString,QFont> ReallyUsed;
	ReallyUsed.clear();
	GetUsedFonts(&ReallyUsed);
	if (doc->PDF_Optionen.EmbedList.count() != 0)
	{
		QValueList<QString> tmpEm;
		QValueList<QString>::Iterator itef;
		for (itef = doc->PDF_Optionen.EmbedList.begin(); itef != doc->PDF_Optionen.EmbedList.end(); ++itef)
		{
			if (ReallyUsed.contains((*itef)))
				tmpEm.append((*itef));
		}
		doc->PDF_Optionen.EmbedList = tmpEm;
	}
	if (doc->PDF_Optionen.SubsetList.count() != 0)
	{
		QValueList<QString> tmpEm;
		QValueList<QString>::Iterator itef;
		for (itef = doc->PDF_Optionen.SubsetList.begin(); itef != doc->PDF_Optionen.SubsetList.end(); ++itef)
		{
			if (ReallyUsed.contains((*itef)))
				tmpEm.append((*itef));
		}
		doc->PDF_Optionen.SubsetList = tmpEm;
	}
	PDF_Opts *dia = new PDF_Opts(this, doc->DocName, ReallyUsed, view, &doc->PDF_Optionen, doc->PDF_Optionen.PresentVals, &PDFXProfiles, Prefs.AvailFonts);
	if (dia->exec())
	{
		std::vector<int> pageNs;
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		fn = dia->Datei->text();
		doc->PDF_Optionen.Datei = fn;
		doc->PDF_Optionen.Thumbnails = dia->CheckBox1->isChecked();
		doc->PDF_Optionen.Compress = dia->Compression->isChecked();
		doc->PDF_Optionen.CompressMethod = dia->CMethod->currentItem();
		doc->PDF_Optionen.Quality = dia->CQuality->currentItem();
		doc->PDF_Optionen.Resolution = dia->Resolution->value();
		doc->PDF_Optionen.EmbedList = dia->FontsToEmbed;
		doc->PDF_Optionen.SubsetList = dia->FontsToSubset;
		doc->PDF_Optionen.RecalcPic = dia->DSColor->isChecked();
		doc->PDF_Optionen.PicRes = dia->ValC->value();
		doc->PDF_Optionen.Bookmarks = dia->CheckBM->isChecked();
		doc->PDF_Optionen.Binding = dia->ComboBind->currentItem();
		doc->PDF_Optionen.MirrorH = dia->MirrorH->isOn();
		doc->PDF_Optionen.MirrorV = dia->MirrorV->isOn();
		doc->PDF_Optionen.RotateDeg = dia->RotateDeg->currentItem() * 90;
		doc->PDF_Optionen.PresentMode = dia->CheckBox10->isChecked();
		doc->PDF_Optionen.PresentVals = dia->EffVal;
		doc->PDF_Optionen.Articles = dia->Article->isChecked();
		doc->PDF_Optionen.Encrypt = dia->Encry->isChecked();
		doc->PDF_Optionen.UseLPI = dia->UseLPI->isChecked();
		if (dia->Encry->isChecked())
		{
			int Perm = -64;
			if (dia->ComboBox1->currentItem() == 1)
				Perm &= ~0x00240000;
			if (dia->PrintSec->isChecked())
				Perm += 4;
			if (dia->ModifySec->isChecked())
				Perm += 8;
			if (dia->CopySec->isChecked())
				Perm += 16;
			if (dia->AddSec->isChecked())
				Perm += 32;
			doc->PDF_Optionen.Permissions = Perm;
			doc->PDF_Optionen.PassOwner = dia->PassOwner->text();
			doc->PDF_Optionen.PassUser = dia->PassUser->text();
		}
		if (dia->ComboBox1->currentItem() == 0)
			doc->PDF_Optionen.Version = 13;
		if (dia->ComboBox1->currentItem() == 1)
			doc->PDF_Optionen.Version = 14;
		if (dia->ComboBox1->currentItem() == 2)
			doc->PDF_Optionen.Version = 12;
		if (dia->OutCombo->currentItem() == 0)
		{
			doc->PDF_Optionen.UseRGB = true;
			doc->PDF_Optionen.UseProfiles = false;
			doc->PDF_Optionen.UseProfiles2 = false;
		}
		else
		{
			doc->PDF_Optionen.UseRGB = false;
#ifdef HAVE_CMS
			if (CMSuse)
			{
				doc->PDF_Optionen.UseProfiles = dia->EmbedProfs->isChecked();
				doc->PDF_Optionen.UseProfiles2 = dia->EmbedProfs2->isChecked();
				doc->PDF_Optionen.Intent = dia->IntendS->currentItem();
				doc->PDF_Optionen.Intent2 = dia->IntendI->currentItem();
				doc->PDF_Optionen.EmbeddedI = dia->NoEmbedded->isChecked();
				doc->PDF_Optionen.SolidProf = dia->SolidPr->currentText();
				doc->PDF_Optionen.ImageProf = dia->ImageP->currentText();
				doc->PDF_Optionen.PrintProf = dia->PrintProfC->currentText();
				if (doc->PDF_Optionen.Version == 12)
				{
					const char *Descriptor;
					cmsHPROFILE hIn;
					hIn = cmsOpenProfileFromFile(PrinterProfiles[doc->PDF_Optionen.PrintProf], "r");
					Descriptor = cmsTakeProductDesc(hIn);
					nam = QString(Descriptor);
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
						Components = 3;
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
						Components = 4;
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmyData)
						Components = 3;
					cmsCloseProfile(hIn);
					doc->PDF_Optionen.Info = dia->InfoString->text();
					doc->PDF_Optionen.BleedTop = dia->BleedTop->value()/UmReFaktor;
					doc->PDF_Optionen.BleedLeft = dia->BleedLeft->value()/UmReFaktor;
					doc->PDF_Optionen.BleedRight = dia->BleedRight->value()/UmReFaktor;
					doc->PDF_Optionen.BleedBottom = dia->BleedBottom->value()/UmReFaktor;
					doc->PDF_Optionen.Encrypt = false;
					doc->PDF_Optionen.MirrorH = false;
					doc->PDF_Optionen.MirrorV = false;
					doc->PDF_Optionen.RotateDeg = 0;
					doc->PDF_Optionen.PresentMode = false;
					doc->PDF_Optionen.Encrypt = false;
				}
			}
			else
			{
				doc->PDF_Optionen.UseProfiles = false;
				doc->PDF_Optionen.UseProfiles2 = false;
			}
#else
			doc->PDF_Optionen.UseProfiles = false;
			doc->PDF_Optionen.UseProfiles2 = false;
#endif

		}
		if (dia->AllPages->isChecked())
			parsePagesString("*", &pageNs, doc->PageC);
		else
			parsePagesString(dia->PageNr->text(), &pageNs, doc->PageC);
		QMap<int,QPixmap> thumbs;
		for (uint ap = 0; ap < pageNs.size(); ++ap)
		{
			QPixmap pm(10,10);
			if (doc->PDF_Optionen.Thumbnails)
				pm = view->PageToPixmap(pageNs[ap]-1, 100);
			thumbs.insert(pageNs[ap], pm);
		}
		ReOrderText(doc, view);
		if (!getPDFDriver(fn, nam, Components, pageNs, thumbs))
			QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
	}
	delete dia;
}

void ScribusApp::AddBookMark(PageItem *ite)
{
	BookPal->BView->AddPageItem(ite);
	StoreBookmarks();
}

void ScribusApp::DelBookMark(PageItem *ite)
{
	BookPal->BView->DeleteItem(ite->BMnr);
	StoreBookmarks();
}

void ScribusApp::BookMarkTxT(PageItem *ite)
{
	BookPal->BView->ChangeText(ite);
	StoreBookmarks();
}

void ScribusApp::ChBookmarks(int s, int e, int n)
{
//	view->Pages.at(s)->Items.at(e)->BMnr = n;
}

void ScribusApp::RestoreBookMarks()
{
	QValueList<ScribusDoc::BookMa>::Iterator it2 = doc->BookMarks.begin();
	BookPal->BView->clear();
	if (doc->BookMarks.count() == 0)
		return;
	BookMItem* ip;
	BookMItem* ip2 = NULL;
	BookMItem* ip3 = NULL;
	BookMItem *ite = new BookMItem(BookPal->BView, &(*it2));
	++it2;
	for( ; it2 != doc->BookMarks.end(); ++it2 )
	{
		if ((*it2).Parent == 0)
			ite = new BookMItem(BookPal->BView, ite, &(*it2));
		else
		{
			QListViewItemIterator it3(BookPal->BView);
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
				(void) new BookMItem(ip2, &(*it2));
			else
			{
				QListViewItemIterator it4(BookPal->BView);
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
			}
		}
	}
}

void ScribusApp::StoreBookmarks()
{
	doc->BookMarks.clear();
	BookMItem* ip;
	QListViewItemIterator it(BookPal->BView);
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
	ActWin->NrItems = BookPal->BView->NrItems;
	ActWin->First = BookPal->BView->First;
	ActWin->Last = BookPal->BView->Last;
}

void ScribusApp::slotElemRead(QString Name, int x, int y, bool art, bool loca, ScribusDoc* docc, ScribusView* vie)
{
	if (doc == docc)
		NoFrameEdit();
	ScriXmlDoc *ss = new ScriXmlDoc();
	if(ss->ReadElem(Name, Prefs.AvailFonts, docc, x, y, art, loca, Prefs.GFontSub, &Prefs, vie))
	{
		vie->DrawNew();
		if (doc == docc)
		{
			doc->OpenNodes = Tpal->buildReopenVals();
			buildFontMenu();
			Mpal->Cpal->SetColors(docc->PageColors);
			Mpal->updateCList();
			Mpal->Spal->updateFormatList();
			Mpal->SetLineFormats(docc);
			Tpal->BuildTree(view);
			Tpal->reopenTree(doc->OpenNodes);
			slotDocCh();
		}
	}
	delete ss;
}

void ScribusApp::slotChangeUnit(int art, bool draw)
{
	doc->docUnitIndex = art;
	UmReFaktor = unitGetRatioFromIndex( doc->docUnitIndex );
	view->UN->setText( unitGetStrFromIndex( doc->docUnitIndex) );
	Mpal->UnitChange();
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

void ScribusApp::ManageTemp(QString temp)
{
	MusterPages *dia = new MusterPages(this, doc, view, temp);
	connect(dia, SIGNAL(createNew(int)), this, SLOT(slotNewPageT(int)));
	connect(dia, SIGNAL(loadPage(QString, int, bool)), this, SLOT(LadeSeite(QString, int, bool)));
	connect(dia, SIGNAL(finished()), this, SLOT(ManTempEnd()));
	for (uint a=0; a<5; ++a)
	{
		pageMenu->setItemEnabled(pageMenu->idAt(a), 0);
	}
	editMenu->setItemEnabled(tman, 0);
	ActWin->MenuStat[0] = DatSav->isEnabled();
	ActWin->MenuStat[1] = fileMenu->isItemEnabled(fid52);
	ActWin->MenuStat[2] = fileMenu->isItemEnabled(fid4);
	ActWin->MenuStat[3] = fileMenu->isItemEnabled(M_SaveAs);
	DatNeu->setEnabled(false);
	DatOpe->setEnabled(false);
	DatClo->setEnabled(false);
	fileMenu->setItemEnabled(M_NewFile, 0);
	fileMenu->setItemEnabled(fid13, 0);
	fileMenu->setItemEnabled(fid14, 0);
	fileMenu->setItemEnabled(fid1, 0);
	fileMenu->setItemEnabled(fid52, 0);
	fileMenu->setItemEnabled(fid7, 0);
	fileMenu->setItemEnabled(M_Print, 0);
	doc->TemplateMode = true;
	Sepal->DisablePal();
	dia->show();
	ActWin->muster = dia;
	doc->OpenNodes = Tpal->buildReopenVals();
}

void ScribusApp::ManTempEnd()
{
	view->HideTemplate();
	editMenu->setItemEnabled(tman, 1);
	pageMenu->setItemEnabled(pageMenu->idAt(0), 1);
	pageMenu->setItemEnabled(pageMenu->idAt(2), 1);
	pageMenu->setItemEnabled(pageMenu->idAt(4), 1);
	DatNeu->setEnabled(true);
	DatSav->setEnabled(doc->isModified());
	DatOpe->setEnabled(true);
	DatClo->setEnabled(true);
	fileMenu->setItemEnabled(M_NewFile, 1);
	fileMenu->setItemEnabled(fid13, 1);
	fileMenu->setItemEnabled(fid14, 1);
	fileMenu->setItemEnabled(fid1, 1);
	fileMenu->setItemEnabled(fid52, 1);
	fileMenu->setItemEnabled(fid4, doc->isModified());
	fileMenu->setItemEnabled(fid7, 1);
	fileMenu->setItemEnabled(M_Print, 1);
	int setter = doc->Pages.count() > 1 ? 1 : 0;
	pageMenu->setItemEnabled(pgmd, setter);
	pageMenu->setItemEnabled(pgmv, setter);
	for (uint c=0; c<doc->Pages.count(); ++c)
	{
		Apply_Temp(doc->Pages.at(c)->MPageNam, c, false);
	}
	doc->TemplateMode = false;
	Sepal->EnablePal();
//	Sepal->RebuildTemp();
	ActWin->muster = NULL;
	view->DrawNew();
//	Sepal->Rebuild();
//	Tpal->BuildTree(view);
//	Tpal->reopenTree(doc->OpenNodes);
//	slotDocCh();
}

void ScribusApp::ApplyTemp()
{
	QString mna;
	ApplyT *dia = new ApplyT(this, doc, doc->currentPage->MPageNam);
	if (dia->exec())
	{
		mna = dia->Templ->currentText();
		if (dia->SinglePage->isChecked())
			Apply_Temp(mna, doc->currentPage->PageNr, false);
		else if (dia->OddRange->isChecked())
		{
			for (int a = 0; a < doc->PageC; a +=2)
			{
				Apply_Temp(mna, a, false);
			}
		}
		else if (dia->EvenRange->isChecked())
		{
			for (int a = 1; a < doc->PageC; a +=2)
			{
				Apply_Temp(mna, a, false);
			}
		}
		else
		{
			int from = dia->FromPage->value()-1;
			int to = dia->ToPage->value();
			for (int a = from; a < to; ++a)
			{
				Apply_Temp(mna, a, false);
			}
		}
	}
	view->DrawNew();
	slotDocCh();
//	Sepal->Rebuild();
	delete dia;
}

void ScribusApp::Apply_Temp(QString in, int Snr, bool reb)
{
	PageItem* b;
	QString mna = in;
	if (mna == tr("Normal"))
		mna = "Normal";
	Page* Ap = doc->Pages.at(Snr);
	Ap->MPageNam = mna;
	int MpNr = doc->MasterNames[mna];
	Page* Mp = doc->MasterPages.at(MpNr);
	for (b = Ap->FromMaster.first(); b; b = Ap->FromMaster.next())
	{
		if (b->ChangedMasterItem)
		{
			Ap->FromMaster.remove(b);
			delete b;
		}
	}
	Ap->FromMaster.clear();
	for (b = doc->MasterItems.first(); b; b = doc->MasterItems.next())
	{
		if (b->OwnPage == MpNr)
			Ap->FromMaster.append(b);
	}
	if (Mp->YGuides.count() != 0)
	{
		for (uint y = 0; y < Mp->YGuides.count(); ++y)
		{
			if (Ap->YGuides.contains(Mp->YGuides[y]) == 0)
				Ap->YGuides.append(Mp->YGuides[y]);
		}
		qHeapSort(Ap->YGuides);
	}
	if (Mp->XGuides.count() != 0)
	{
		for (uint x = 0; x < Mp->XGuides.count(); ++x)
		{
			if (Ap->XGuides.contains(Mp->XGuides[x]) == 0)
				Ap->XGuides.append(Mp->XGuides[x]);
		}
		qHeapSort(Ap->XGuides);
	}
	if (reb)
	{
		view->DrawNew();
		slotDocCh();
//		Sepal->Rebuild();
	}
}

void ScribusApp::GroupObj()
{
	PageItem* b;
	PageItem* bb;
	double x, y, w, h;
	for (uint a=0; a<view->SelItem.count(); ++a)
	{
		b = view->SelItem.at(a);
		if (b->Locked)
		{
			int t = QMessageBox::warning(this, tr("Warning"),
			                             tr("Some Objects are locked."),
			                             tr("&Cancel"),
			                             tr("&Lock All"),
			                             tr("&Unlock All"), 0, 0);
			if (t != 0)
			{
				for (uint c=0; c<view->SelItem.count(); ++c)
				{
					bb = view->SelItem.at(c);
					if (t == 1)
					{
						bb->Locked = true;
						ObjMenu->changeItem(LockOb, tr("Un&lock"));
					}
					else
					{
						bb->Locked = false;
						ObjMenu->changeItem(LockOb, tr("&Lock"));
					}
				}
			}
		}
	}
	for (uint a=0; a<view->SelItem.count(); ++a)
	{
		b = view->SelItem.at(a);
		b->Groups.push(doc->GroupCounter);
	}
	doc->GroupCounter++;
	view->getGroupRect(&x, &y, &w, &h);
	view->updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
	slotDocCh();
	ObjMenu->setItemEnabled(PfadT, false);
	ObjMenu->setItemEnabled(Gr, false);
	ObjMenu->setItemEnabled(UnGr, true);
}

void ScribusApp::UnGroupObj()
{
	PageItem* b;
	for (uint a=0; a<view->SelItem.count(); ++a)
	{
		b = view->SelItem.at(a);
		b->Groups.pop();
		b->isTableItem = false;
		b->LeftLink = 0;
		b->RightLink = 0;
		b->TopLink = 0;
		b->BottomLink = 0;
	}
	slotDocCh();
	view->Deselect(true);
}

void ScribusApp::StatusPic()
{
	if (HaveDoc)
	{
		PicStatus *dia = new PicStatus(this, doc, view);
		connect(dia, SIGNAL(GotoSeite(int)), this, SLOT(SelectFromOutlS(int)));
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
	if (defNa != "")
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

void ScribusApp::RunPlug(int id)
{
	if (extraMenu->indexOf(id) > 2)
		CallDLLbyMenu(id);
}

void ScribusApp::RunImportPlug(int id)
{
	if (importMenu->indexOf(id) > 2)
	{
		if (HaveDoc)
			doc->OpenNodes = Tpal->buildReopenVals();
		CallDLLbyMenu(id);
		if (HaveDoc)
		{
			Tpal->BuildTree(view);
			Tpal->reopenTree(doc->OpenNodes);
			Mpal->updateCList();
		}
	}
}

void ScribusApp::RunExportPlug(int id)
{
	if (exportMenu->indexOf(id) > 2)
		CallDLLbyMenu(id);
}

void ScribusApp::RunHelpPlug(int id)
{
	if (helpMenu->indexOf(id) > 3)
		CallDLLbyMenu(id);
}

void ScribusApp::FinalizePlugs()
{
	const char *error;
	QMap<int, PlugData>::Iterator it;
	struct PlugData pda;
	typedef void (*sdem2)();
	sdem2 demo2;
	for (it = PluginMap.begin(); it != PluginMap.end(); ++it)
	{
		if (it.data().Typ > 3)
		{
			dlerror();
			demo2 = (sdem2)dlsym(it.data().Zeiger, "CleanUpPlug");
			if ((error = dlerror()) != NULL)
			{
				dlclose(it.data().Zeiger);
				continue;
			}
			else
			{
				(*demo2)();
			}
		}
	}
}

void ScribusApp::initPlugs()
{
	QString pfad = PLUGINDIR;
	QString nam = "";
	int id = 0;
	int ty = 0;
	int menid = 0;
	struct PlugData pda;
#if defined(__hpux)
	QDir d(pfad, "*.sl*", QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);
#else
	QDir d(pfad, "*.so*", QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);
#endif
	if ((d.exists()) && (d.count() != 0))
	{
		extraMenu->insertSeparator();
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			pda.Zeiger = 0;
			pda.Datei = "";
			pda.Name = "";
			pda.Typ = 0;
			pda.MenuID = 0;
			if (DLLName(d[dc], &nam, &ty, &pda.Zeiger, &id))
			{
				if (ty == 1)
					menid = extraMenu->insertItem(nam);
				if (ty == 2)
					menid = importMenu->insertItem(nam);
				if (ty == 3)
					menid = exportMenu->insertItem(nam);
				if (ty == 4)
					menid = helpMenu->insertItem(nam);
				pda.Name = nam;
				pda.Datei = d[dc];
				pda.Typ = ty;
				if (ty < 5)
					pda.MenuID = menid;
				PluginMap.insert(id, pda);
				if (splashScreen != NULL)
					splashScreen->setStatus( tr("Loading:")+" "+nam);
			}
		}
		connect(extraMenu, SIGNAL(activated(int)), this, SLOT(RunPlug(int)));
		connect(importMenu, SIGNAL(activated(int)), this, SLOT(RunImportPlug(int)));
		connect(exportMenu, SIGNAL(activated(int)), this, SLOT(RunExportPlug(int)));
		connect(helpMenu, SIGNAL(activated(int)), this, SLOT(RunHelpPlug(int)));
	}
}

void ScribusApp::CallDLLbyMenu(int id)
{
	QMap<int, PlugData>::Iterator it;
	struct PlugData pda;
	for (it = PluginMap.begin(); it != PluginMap.end(); ++it)
	{
		if (it.data().MenuID == id)
		{
			CallDLL(it.key());
			break;
		}
	}
}

void ScribusApp::CallDLL(int ident)
{
	void *mo;
	const char *error;
	struct PlugData pda;
	pda = PluginMap[ident];
	typedef void (*sdem)(QWidget *d, ScribusApp *plug);
	sdem demo;
	QString pfad = PLUGINDIR;
	if (pda.Typ < 4)
	{
		pfad += pda.Datei;
		mo = dlopen(pfad, RTLD_LAZY | RTLD_GLOBAL);
		if (!mo)
		{
			std::cout << "Can't find Plug-in" << endl;
			return;
		}
	}
	else
		mo = pda.Zeiger;
	dlerror();
	demo = (sdem)dlsym(mo, "Run");
	if ((error = dlerror()) != NULL)
	{
		std::cout << "Can't find Symbol" << endl;
		dlclose(mo);
		return;
	}
	(*demo)(this, this);
	if (pda.Typ < 4)
		dlclose(mo);
	if (HaveDoc)
		view->DrawNew();
}

bool ScribusApp::DLLexists(int ident)
{
	return PluginMap.contains(ident);
}

bool ScribusApp::DLLName(QString name, QString *PName, int *typ, void **Zeig, int *idNr)
{
	void *mo;
	const char *error;
	typedef QString (*sdem0)();
	typedef int (*sdem1)();
	typedef void (*sdem2)(QWidget *d, ScribusApp *plug);
	sdem0 demo;
	sdem1 demo1;
	sdem2 demo2;
	QString pfad = PLUGINDIR;
	pfad += name;
	mo = dlopen(pfad, RTLD_LAZY | RTLD_GLOBAL);
	if (!mo)
	{
		std::cout << dlerror() << endl;
		return false;
	}
	dlerror();
	demo = (sdem0)dlsym(mo, "Name");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*PName = (*demo)();
	dlerror();
	demo1 = (sdem1)dlsym(mo, "Type");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*typ = (*demo1)();
	*Zeig = mo;
	demo1 = (sdem1)dlsym(mo, "ID");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*idNr = (*demo1)();
	if (*typ < 4)
		dlclose(mo);
	else
	{
		dlerror();
		demo2 = (sdem2)dlsym(mo, "InitPlug");
		if ((error = dlerror()) != NULL)
		{
			dlclose(mo);
			return false;
		}
		(*demo2)(this, this);
	}
	return true;
}

void ScribusApp::GetCMSProfiles()
{
	MonitorProfiles.clear();
	PrinterProfiles.clear();
	InputProfiles.clear();
	QString pfad = LIBDIR;
	pfad += "profiles/";
	GetCMSProfilesDir(pfad);
	if (Prefs.ProfileDir != "")
	{
		if(Prefs.ProfileDir.right(1) != "/")
			Prefs.ProfileDir += "/";
		GetCMSProfilesDir(Prefs.ProfileDir);
	}
	if ((!PrinterProfiles.isEmpty()) && (!InputProfiles.isEmpty()) && (!MonitorProfiles.isEmpty()))
		CMSavail = true;
	else
		CMSavail = false;
}

void ScribusApp::initCMS()
{
	if (CMSavail)
	{
		ProfilesL::Iterator ip;
		if ((Prefs.DCMSset.DefaultInputProfile == "") || (!InputProfiles.contains(Prefs.DCMSset.DefaultInputProfile)))
		{
			ip = InputProfiles.begin();
			Prefs.DCMSset.DefaultInputProfile = ip.key();
		}
		if ((Prefs.DCMSset.DefaultInputProfile2 == "") || (!InputProfiles.contains(Prefs.DCMSset.DefaultInputProfile2)))
		{
			ip = InputProfiles.begin();
			Prefs.DCMSset.DefaultInputProfile2 = ip.key();
		}
		if ((Prefs.DCMSset.DefaultMonitorProfile == "") || (!MonitorProfiles.contains(Prefs.DCMSset.DefaultMonitorProfile)))
		{
			ip = MonitorProfiles.begin();
			Prefs.DCMSset.DefaultMonitorProfile = ip.key();
		}
		if ((Prefs.DCMSset.DefaultPrinterProfile == "") || (!PrinterProfiles.contains(Prefs.DCMSset.DefaultPrinterProfile)))
		{
			ip = PrinterProfiles.begin();
			Prefs.DCMSset.DefaultPrinterProfile = ip.key();
		}
#ifdef HAVE_CMS
		SoftProofing = Prefs.DCMSset.SoftProofOn;
		CMSuse = false;
		IntentPrinter = Prefs.DCMSset.DefaultIntentPrinter;
		IntentMonitor = Prefs.DCMSset.DefaultIntentMonitor;
#endif
	}
}

void ScribusApp::GetCMSProfilesDir(QString pfad)
{
#ifdef HAVE_CMS
	QString nam = "";
	const char *Descriptor;
	cmsHPROFILE hIn;
	QDir d(pfad, "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
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
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
						InputProfiles[nam] = pfad + d[dc];
					break;
				case icSigColorSpaceClass:
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
						InputProfiles[nam] = pfad + d[dc];
					break;
				case icSigDisplayClass:
					MonitorProfiles[nam] = pfad + d[dc];
					InputProfiles[nam] = pfad + d[dc];
					break;
				case icSigOutputClass:
					PrinterProfiles[nam] = pfad + d[dc];
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
						PDFXProfiles[nam] = pfad + d[dc];
					break;
				}
				cmsCloseProfile(hIn);
			}
		}
	}
#endif
}

void ScribusApp::RecalcColors(QProgressBar *dia)
{
	ColorList::Iterator it;
	if (HaveDoc)
	{
		if (doc->TemplateMode)
			doc->MasterPages = doc->Pages;
		else
			doc->DocPages = doc->Pages;
		ColorMenC->clear();
		QPixmap pm = QPixmap(15, 15);
		int a = 0;
		ColorMenC->insertItem( tr("None"));
		CMYKColor tmp;
		tmp.fromQColor(doc->papColor);
		tmp.RecalcRGB();
		doc->papColor = tmp.getRGBColor();
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			doc->PageColors[it.key()].RecalcRGB();
			pm.fill(doc->PageColors[it.key()].getRGBColor());
			ColorMenC->insertItem(pm, it.key());
			if (it.key() == doc->Dbrush)
				ColorMenC->setCurrentItem(a);
			a++;
			if (dia != NULL)
				dia->setProgress(a);
		}
		for (uint c=0; c<doc->Items.count(); ++c)
		{
			PageItem *ite = doc->Items.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				QColor tmpc = doc->PageColors[cstops.at(cst)->name].getRGBColor();
				ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
				cstops.at(cst)->color = tmpc;
			}
		}
		for (uint c=0; c<doc->MasterItems.count(); ++c)
		{
			PageItem *ite = doc->MasterItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				QColor tmpc = doc->PageColors[cstops.at(cst)->name].getRGBColor();
				ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
				cstops.at(cst)->color = tmpc;
			}
		}
		Mpal->Cpal->SetColors(doc->PageColors);
		Mpal->updateCList();
	}
}

void ScribusApp::ModifyAnnot()
{
	PageItem *b;
	if (view->SelItem.count() != 0)
	{
		b = view->SelItem.at(0);
		if ((b->AnType == 0) || (b->AnType == 1) || (b->AnType > 9))
		{
			int AnType = b->AnType;
			int AnActType = b->AnActType;
			QString AnAction = b->AnAction;
			QString An_Extern = b->An_Extern;
			Annota *dia = new Annota(this, b, doc->PageC, static_cast<int>(doc->PageB), static_cast<int>(doc->PageH), view);
			if (dia->exec())
				slotDocCh();
			else
			{
				b->AnType = AnType;
				b->AnActType = AnActType;
				b->AnAction = AnAction;
				b->An_Extern = An_Extern;
			}
			delete dia;
		}
		else
		{
			Annot *dia = new Annot(this, b, doc->PageC, static_cast<int>(doc->PageB), static_cast<int>(doc->PageH), doc->PageColors, view);
			if (dia->exec())
				slotDocCh();
			delete dia;
		}
	}
}

void ScribusApp::SetShortCut()
{
	uint a;
	for (a = 0; a < 9; ++a)
	{
		fileMenu->setAccel(Prefs.KeyActions[a].KeyID, Prefs.KeyActions[a].MenuID);
	}
	for (a = 9; a < 17; ++a)
	{
		editMenu->setAccel(Prefs.KeyActions[a].KeyID, Prefs.KeyActions[a].MenuID);
	}
	editMenu->setAccel(Prefs.KeyActions[19].KeyID, Prefs.KeyActions[19].MenuID);
	for (a = 20; a < 30; ++a)
	{
		ObjMenu->setAccel(Prefs.KeyActions[a].KeyID, Prefs.KeyActions[a].MenuID);
	}
	for (a = 30; a < 34; ++a)
	{
		pageMenu->setAccel(Prefs.KeyActions[a].KeyID, Prefs.KeyActions[a].MenuID);
	}
	pageMenu->setAccel(Prefs.KeyActions[61].KeyID, Prefs.KeyActions[61].MenuID);
	for (a = 34; a < 45; ++a)
	{
		viewMenu->setAccel(Prefs.KeyActions[a].KeyID, Prefs.KeyActions[a].MenuID);
	}
	for (a = 45; a < 49; ++a)
	{
		toolMenu->setAccel(Prefs.KeyActions[a].KeyID, Prefs.KeyActions[a].MenuID);
	}
	pageMenu->setAccel(Prefs.KeyActions[49].KeyID, Prefs.KeyActions[49].MenuID);
	extraMenu->setAccel(Prefs.KeyActions[50].KeyID, Prefs.KeyActions[50].MenuID);
	extraMenu->setAccel(Prefs.KeyActions[51].KeyID, Prefs.KeyActions[51].MenuID);
	for (a = 52; a < 56; ++a)
	{
		helpMenu->setAccel(Prefs.KeyActions[a].KeyID, Prefs.KeyActions[a].MenuID);
	}
	ObjMenu->setAccel(Prefs.KeyActions[61].KeyID, Prefs.KeyActions[61].MenuID);
	toolMenu->setAccel(Prefs.KeyActions[62].KeyID, Prefs.KeyActions[62].MenuID);
	editMenu->setAccel(Prefs.KeyActions[63].KeyID, Prefs.KeyActions[63].MenuID);
	editMenu->setAccel(Prefs.KeyActions[64].KeyID, Prefs.KeyActions[64].MenuID);
	editMenu->setAccel(Prefs.KeyActions[65].KeyID, Prefs.KeyActions[65].MenuID);
	toolMenu->setAccel(Prefs.KeyActions[66].KeyID, Prefs.KeyActions[66].MenuID);
	ObjMenu->setAccel(Prefs.KeyActions[67].KeyID, Prefs.KeyActions[67].MenuID);
	ObjMenu->setAccel(Prefs.KeyActions[68].KeyID, Prefs.KeyActions[68].MenuID);
	fileMenu->setAccel(Prefs.KeyActions[18].KeyID, Prefs.KeyActions[18].MenuID);
}

void ScribusApp::PutScrap(QString t)
{
	ScBook->ObjFromMenu(t);
}

void ScribusApp::UniteOb()
{
	view->UniteObj();
}

void ScribusApp::SplitUniteOb()
{
	view->SplitObj();
}

void ScribusApp::TraceText()
{
	NoFrameEdit();
	view->TextToPath();
}

void ScribusApp::Pfadtext()
{
	view->ToPathText();
}

void ScribusApp::noPfadtext()
{
	view->FromPathText();
}

void ScribusApp::changeLayer(int l)
{
	view->Deselect(true);
	view->setLayMenTxt(l);
	view->LaMenu();
	view->DrawNew();
}

void ScribusApp::showLayer()
{
	view->DrawNew();
}

void ScribusApp::LayerRemove(int l, bool dl)
{
	if (doc->TemplateMode)
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
				doc->DocItems.at(b)->Locked = false;
			}
			else
				doc->MasterItems.at(b)->LayerNr = 0;
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
				doc->DocItems.at(b)->Locked = false;
			}
			else
				doc->DocItems.at(b)->LayerNr = 0;
		}
	}
	if (view->SelItem.count() != 0)
		view->DeleteItem();
	view->LaMenu();
}

void ScribusApp::UnDoAction()
{
	UndoManager::instance()->undo(1);
}

void ScribusApp::RedoAction()
{
	UndoManager::instance()->redo(1);
}

void ScribusApp::refreshUndoRedoItems()
{
	toolMenu->setItemChecked(viewUndoPalette, undoPalette->isVisible());
	editMenu->setItemEnabled(edUndo, UndoManager::instance()->hasUndoActions());
	editMenu->setItemEnabled(edRedo, UndoManager::instance()->hasRedoActions());
}

void ScribusApp::initHyphenator()
{
	QString pfad = LIBDIR;
	QStringList L_German;
	QStringList L_Polish;
	QStringList L_English;
	QStringList L_Spanish;
	QStringList L_Italian;
	QStringList L_French;
	QStringList L_Russian;
	QStringList L_Danish;
	QStringList L_Slovak;
	QStringList L_Hungarian;
	QStringList L_Czech;
	QStringList L_Dutch;
	QStringList L_Portuguese;
	QStringList L_Ukrainian;
	QStringList L_Greek;
	QStringList L_Catalan;
	QStringList L_Finnish;
	QStringList L_Irish;
	QStringList L_Lithuanian;
	QStringList L_Swedish;
	QStringList L_Slovenian;
	QStringList L_Afrikaans;
	L_German.clear();
	L_Polish.clear();
	L_English.clear();
	L_Spanish.clear();
	L_Italian.clear();
	L_French.clear();
	L_Russian.clear();
	L_Danish.clear();
	L_Slovak.clear();
	L_Hungarian.clear();
	L_Czech.clear();
	L_Dutch.clear();
	L_Portuguese.clear();
	L_Ukrainian.clear();
	L_Greek.clear();
	L_Catalan.clear();
	L_Finnish.clear();
	L_Irish.clear();
	L_Lithuanian.clear();
	L_Swedish.clear();
	L_Slovenian.clear();
	L_Afrikaans.clear();
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
				translatedLang = trans->findMessage("ScribusApp", "German", "").translation();
				if (translatedLang != "")
					L_German.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Polish", "").translation();
				if (translatedLang != "")
					L_Polish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "English", "").translation();
				if (translatedLang != "")
					L_English.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Spanish", "").translation();
				if (translatedLang != "")
					L_Spanish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Italian", "").translation();
				if (translatedLang != "")
					L_Italian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "French", "").translation();
				if (translatedLang != "")
					L_French.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Russian", "").translation();
				if (translatedLang != "")
					L_Russian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Danish", "").translation();
				if (translatedLang != "")
					L_Danish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Slovak", "").translation();
				if (translatedLang != "")
					L_Slovak.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Hungarian", "").translation();
				if (translatedLang != "")
					L_Hungarian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Czech", "").translation();
				if (translatedLang != "")
					L_Czech.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Dutch", "").translation();
				if (translatedLang != "")
					L_Dutch.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Portuguese", "").translation();
				if (translatedLang != "")
					L_Portuguese.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Ukrainian", "").translation();
				if (translatedLang != "")
					L_Ukrainian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Greek", "").translation();
				if (translatedLang != "")
					L_Greek.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Catalan", "").translation();
				if (translatedLang != "")
					L_Catalan.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Finnish", "").translation();
				if (translatedLang != "")
					L_Finnish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Irish", "").translation();
				if (translatedLang != "")
					L_Irish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Lithuanian", "").translation();
				if (translatedLang != "")
					L_Lithuanian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Swedish", "").translation();
				if (translatedLang != "")
					L_Swedish.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Slovenian", "").translation();
				if (translatedLang != "")
					L_Slovenian.append(translatedLang);
				translatedLang = "";
				translatedLang = trans->findMessage("ScribusApp", "Afrikaans", "").translation();
				if (translatedLang != "")
					L_Afrikaans.append(translatedLang);
				delete trans;
			}
		}
	}
	InstLang.insert("German", L_German);
	InstLang.insert("Polish", L_Polish);
	InstLang.insert("English", L_English);
	InstLang.insert("Spanish", L_Spanish);
	InstLang.insert("Italian", L_Italian);
	InstLang.insert("French", L_French);
	InstLang.insert("Russian", L_Russian);
	InstLang.insert("Danish", L_Danish);
	InstLang.insert("Slovak", L_Slovak);
	InstLang.insert("Hungarian", L_Hungarian);
	InstLang.insert("Czech", L_Czech);
	InstLang.insert("Dutch", L_Dutch);
	InstLang.insert("Portuguese", L_Portuguese);
	InstLang.insert("Ukrainian", L_Ukrainian);
	InstLang.insert("Greek", L_Greek);
	InstLang.insert("Catalan", L_Catalan);
	InstLang.insert("Finnish", L_Finnish);
	InstLang.insert("Irish", L_Irish);
	InstLang.insert("Lithuanian", L_Lithuanian);
	InstLang.insert("Swedish", L_Swedish);
	InstLang.insert("Slovenian", L_Slovenian);
	InstLang.insert("Afrikaans", L_Afrikaans);
	QString datein = "";
	QString lang = QString(QTextCodec::locale()).left(2);
	LangTransl.clear();
	Prefs.Language = "English";
	pfad = LIBDIR;
	pfad += "dicts/";
	QDir d(pfad, "*.dic", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			if (d[dc] == "hyph_en.dic")
				datein = tr("English");
			if (d[dc] == "hyph_de.dic")
				datein = tr("German");
			if (d[dc] == "hyph_es.dic")
				datein = tr("Spanish");
			if (d[dc] == "hyph_it.dic")
				datein = tr("Italian");
			if (d[dc] == "hyph_fr.dic")
				datein = tr("French");
			if (d[dc] == "hyph_ru.dic")
				datein = tr("Russian");
			if (d[dc] == "hyph_da.dic")
				datein = tr("Danish");
			if (d[dc] == "hyph_sk.dic")
				datein = tr("Slovak");
			if (d[dc] == "hyph_hu.dic")
				datein = tr("Hungarian");
			if (d[dc] == "hyph_cs.dic")
				datein = tr("Czech");
			if (d[dc] == "hyph_nl.dic")
				datein = tr("Dutch");
			if (d[dc] == "hyph_pt.dic")
				datein = tr("Portuguese");
			if (d[dc] == "hyph_uk.dic")
				datein = tr("Ukrainian");
			if (d[dc] == "hyph_pl.dic")
				datein = tr("Polish");
			if (d[dc] == "hyph_el.dic")
				datein = tr("Greek");
			if (d[dc] == "hyph_ca.dic")
				datein = tr("Catalan");
			if (d[dc] == "hyph_fi.dic")
				datein = tr("Finnish");
			if (d[dc] == "hyph_ga.dic")
				datein = tr("Irish");
			if (d[dc] == "hyph_lt.dic")
				datein = tr("Lithuanian");
			if (d[dc] == "hyph_sv.dic")
				datein = tr("Swedish");
			if (d[dc] == "hyph_sl.dic")
				datein = tr("Slovenian");
			if (d[dc] == "hyph_af.dic")
				datein = tr("Afrikaans");
			QString tDatein = datein;
			datein = GetLang(datein);
			LangTransl.insert(datein, tDatein);
			Sprachen.insert(datein, d[dc]);
			if (d[dc] == "hyph_"+lang+".dic")
				Prefs.Language = datein;
		}
		if (datein == "")
			Prefs.Language = "English";
	}
	Mpal->fillLangCombo(LangTransl);
}

QString ScribusApp::GetLang(QString inLang)
{
	QMap<QString, QStringList>::Iterator itl;
	for (itl = InstLang.begin(); itl != InstLang.end(); ++itl)
	{
		QStringList::Iterator itlr;
		for (itlr = itl.data().begin(); itlr != itl.data().end(); ++itlr)
		{
			if ((*itlr) == inLang)
				return itl.key();
		}
	}
	return inLang;
}

void ScribusApp::doHyphenate()
{
	PageItem *b;
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			b = view->SelItem.at(0);
			if (doc->docHyphenator->Language != b->Language)
				doc->docHyphenator->slotNewDict(b->Language);
			doc->docHyphenator->slotHyphenate(b);
		}
	}
}

void ScribusApp::ToggleObjLock()
{
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			PageItem* b = view->SelItem.at(0);
			view->ToggleLock();
			if (b->Locked)
				ObjMenu->changeItem(LockOb, tr("Un&lock"));
			else
				ObjMenu->changeItem(LockOb, tr("&Lock"));
		}
	}
}

void ScribusApp::ManageGuides()
{
	if (HaveDoc)
	{
		GuideManager *dia = new GuideManager(
		                        this,
		                        doc->currentPage->XGuides,
		                        doc->currentPage->YGuides,
		                        doc->currentPage->Width,
		                        doc->currentPage->Height,
		                        doc->GuideLock,
		                        doc->docUnitIndex
		                    );
		if (dia->exec())
		{
			doc->currentPage->addXGuides(dia->LocVer);
			doc->currentPage->addYGuides(dia->LocHor);
			doc->GuideLock = dia->LocLocked;
			view->DrawNew();
//			slotDocCh();
		}
		delete dia;
	}
}

void ScribusApp::SetTranspar(double t)
{
	PageItem *b;
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			b = view->SelItem.at(0);
			b->Transparency = t;
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusApp::SetTransparS(double t)
{
	PageItem *b;
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			b = view->SelItem.at(0);
			b->TranspStroke = t;
			view->DrawNew();
			slotDocCh();
		}
	}
}

void ScribusApp::InvertPict()
{
	PageItem *b;
	if (HaveDoc)
	{
		if (view->SelItem.count() != 0)
		{
			b = view->SelItem.at(0);
			b->InvPict = !b->InvPict;
			view->DrawNew();
			slotDocCh();
		}
	}
}

QString ScribusApp::Collect(bool compress, bool withFonts)
{
	if (doc->TemplateMode)
		doc->MasterPages = doc->Pages;
	else
		doc->DocPages = doc->Pages;
	QString retVal = "";
	QString CurDirP = QDir::currentDirPath();
	bool compressR = compress;
	bool withFontsR = withFonts;
	QString wdir = ".";
	if (Prefs.DocDir != "")
		wdir = dirs->get("collect", Prefs.DocDir);
	else
		wdir = dirs->get("collect", ".");
	QString s = CFileDialog(wdir, tr("Choose a Directory"), "", "", false, false, false, false, true, &compressR, &withFontsR);
	if (s != "")
	{
		if(s.right(1) != "/")
			s += "/";
		dirs->set("collect", s.left(s.findRev("/",-2)));
		QFileInfo fi = QFileInfo(s);
		if (fi.exists())
		{
			if (fi.isDir() && fi.isWritable())
			{
				QString fn;
				if (doc->hasName)
				{
					QFileInfo fis(doc->DocName);
					fn = s + fis.fileName();
				}
				else
					fn = s + doc->DocName+".sla";
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
				for (uint b = 0; b < doc->MasterItems.count(); ++b)
				{
					PageItem* ite = doc->MasterItems.at(b);
					if (ite->PType == 2)
					{
						QFileInfo itf = QFileInfo(ite->Pfile);
						if (itf.exists())
						{
							copyFile(ite->Pfile, s + itf.fileName());
							ite->Pfile = s + itf.fileName();
						}
					}
					if (ite->PType == 4)
					{
						if (ite->isAnnotation)
						{
							QFileInfo itf;
							if (ite->Pfile != "")
							{
								itf = QFileInfo(ite->Pfile);
								if (itf.exists())
								{
									copyFile(ite->Pfile, s + itf.fileName());
									ite->Pfile = s + itf.fileName();
								}
							}
							if (ite->Pfile2 != "")
							{
								itf = QFileInfo(ite->Pfile2);
								if (itf.exists())
								{
									copyFile(ite->Pfile2, s + itf.fileName());
									ite->Pfile2 = s + itf.fileName();
								}
							}
							if (ite->Pfile3 != "")
							{
								itf = QFileInfo(ite->Pfile3);
								if (itf.exists())
								{
									copyFile(ite->Pfile3, s + itf.fileName());
									ite->Pfile3 = s + itf.fileName();
								}
							}
						}
					}
				}
				for (uint b = 0; b < doc->DocItems.count(); ++b)
				{
					PageItem* ite = doc->DocItems.at(b);
					if (ite->PType == 2)
					{
						QFileInfo itf = QFileInfo(ite->Pfile);
						if (itf.exists())
						{
							copyFile(ite->Pfile, s + itf.fileName());
							ite->Pfile = s + itf.fileName();
						}
					}
					if (ite->PType == 4)
					{
						if (ite->isAnnotation)
						{
							QFileInfo itf;
							if (ite->Pfile != "")
							{
								itf = QFileInfo(ite->Pfile);
								if (itf.exists())
								{
									copyFile(ite->Pfile, s + itf.fileName());
									ite->Pfile = s + itf.fileName();
								}
							}
							if (ite->Pfile2 != "")
							{
								itf = QFileInfo(ite->Pfile2);
								if (itf.exists())
								{
									copyFile(ite->Pfile2, s + itf.fileName());
									ite->Pfile2 = s + itf.fileName();
								}
							}
							if (ite->Pfile3 != "")
							{
								itf = QFileInfo(ite->Pfile3);
								if (itf.exists())
								{
									copyFile(ite->Pfile, s + itf.fileName());
									ite->Pfile3 = s + itf.fileName();
								}
							}
						}
					}
				}
				if (!DoFileSave(fn))
				{
					QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
					retVal = "";
				}
				if (withFontsR)
				{
					QMap<QString,QFont>::Iterator it3;
					for (it3 = doc->UsedFonts.begin(); it3 != doc->UsedFonts.end(); ++it3)
					{
						QFileInfo itf = QFileInfo(Prefs.AvailFonts[it3.key()]->Datei);
						copyFile(Prefs.AvailFonts[it3.key()]->Datei, s + itf.fileName());
					}
				}
			}
		}
	}
	QDir::setCurrent(CurDirP);
	return retVal;
}

void ScribusApp::ReorgFonts()
{
	PageItem* it;
	QMap<QString,QFont> Really;
	QMap<QString,QFont> DocF;
	DocF = doc->UsedFonts;
	if (!doc->TemplateMode)
		doc->DocPages = doc->Pages;
	for (uint d = 0; d < doc->MasterItems.count(); ++d)
	{
		it = doc->MasterItems.at(d);
		Really.insert(it->IFont, doc->UsedFonts[it->IFont]);
		if ((it->PType == 4) || (it->PType == 8))
		{
			for (uint e = 0; e < it->itemText.count(); ++e)
			{
				Really.insert(it->itemText.at(e)->cfont, doc->UsedFonts[it->itemText.at(e)->cfont]);
			}
		}
	}
	for (uint d = 0; d < doc->DocItems.count(); ++d)
	{
		it = doc->DocItems.at(d);
		Really.insert(it->IFont, doc->UsedFonts[it->IFont]);
		if ((it->PType == 4) || (it->PType == 8))
		{
			for (uint e = 0; e < it->itemText.count(); ++e)
			{
				Really.insert(it->itemText.at(e)->cfont, doc->UsedFonts[it->itemText.at(e)->cfont]);
			}
		}
	}
	QMap<QString,QFont>::Iterator itfo, itnext;
	for (itfo = doc->UsedFonts.begin(); itfo != doc->UsedFonts.end(); itfo = itnext)
	{
		itnext = itfo;
		++itnext;
		if (!Really.contains(itfo.key()))
		{
			FT_Done_Face(doc->FFonts[itfo.key()]);
			doc->FFonts.remove(itfo.key());
			doc->UsedFonts.remove(itfo);
		}
	}
	doc->AddFont(Prefs.DefFont, Prefs.AvailFonts[Prefs.DefFont]->Font);
	doc->AddFont(doc->Dfont, Prefs.AvailFonts[doc->Dfont]->Font);
	buildFontMenu();
}

void ScribusApp::GetUsedFonts(QMap<QString,QFont> *Really)
{
	PageItem* it;
	FPointArray gly;
	QString chx;
	for (uint d = 0; d < doc->MasterItems.count(); ++d)
	{
		it = doc->MasterItems.at(d);
		if ((it->PType == 4) || (it->PType == 8))
		{
			for (uint e = 0; e < it->itemText.count(); ++e)
			{
				Really->insert(it->itemText.at(e)->cfont, doc->UsedFonts[it->itemText.at(e)->cfont]);
				uint chr = it->itemText.at(e)->ch[0].unicode();
				if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 9))
					continue;
				if (it->itemText.at(e)->cstyle & 64)
				{
					chx = it->itemText.at(e)->ch;
					if (chx.upper() != it->itemText.at(e)->ch)
						chx = chx.upper();
					chr = chx[0].unicode();
				}
				if (chr == 30)
				{
					for (uint numco = 0x30; numco < 0x3A; ++numco)
					{
						if ((*doc->AllFonts)[it->itemText.at(e)->cfont]->CharWidth.contains(numco))
						{
							gly = (*doc->AllFonts)[it->itemText.at(e)->cfont]->GlyphArray[numco].Outlines.copy();
							(*doc->AllFonts)[it->itemText.at(e)->cfont]->RealGlyphs.insert(numco, gly);
						}
					}
					continue;
				}
				if ((*doc->AllFonts)[it->itemText.at(e)->cfont]->CharWidth.contains(chr))
				{
					gly = (*doc->AllFonts)[it->itemText.at(e)->cfont]->GlyphArray[chr].Outlines.copy();
					(*doc->AllFonts)[it->itemText.at(e)->cfont]->RealGlyphs.insert(chr, gly);
				}
			}
		}
	}
	for (uint d = 0; d < doc->Items.count(); ++d)
	{
		it = doc->Items.at(d);
		if ((it->PType == 4) || (it->PType == 8))
		{
			for (uint e = 0; e < it->itemText.count(); ++e)
			{
				Really->insert(it->itemText.at(e)->cfont, doc->UsedFonts[it->itemText.at(e)->cfont]);
				uint chr = it->itemText.at(e)->ch[0].unicode();
				if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 9))
					continue;
				if (it->itemText.at(e)->cstyle & 64)
				{
					chx = it->itemText.at(e)->ch;
					if (chx.upper() != it->itemText.at(e)->ch)
						chx = chx.upper();
					chr = chx[0].unicode();
				}
				if (chr == 30)
				{
					for (uint numco = 0x30; numco < 0x3A; ++numco)
					{
						if ((*doc->AllFonts)[it->itemText.at(e)->cfont]->CharWidth.contains(numco))
						{
							gly = (*doc->AllFonts)[it->itemText.at(e)->cfont]->GlyphArray[numco].Outlines.copy();
							(*doc->AllFonts)[it->itemText.at(e)->cfont]->RealGlyphs.insert(numco, gly);
						}
					}
					continue;
				}
				if ((*doc->AllFonts)[it->itemText.at(e)->cfont]->CharWidth.contains(chr))
				{
					gly = (*doc->AllFonts)[it->itemText.at(e)->cfont]->GlyphArray[chr].Outlines.copy();
					(*doc->AllFonts)[it->itemText.at(e)->cfont]->RealGlyphs.insert(chr, gly);
				}
			}
		}
	}
}

void ScribusApp::HaveRaster(bool art)
{
	if (view->SelItem.count() != 0)
	{
		PageItem *b = view->SelItem.at(0);
		if ((b->PType == 2) && (art))
		{
			StilMenu->clear();
			StilMenu->insertItem( tr("Color"), ColorMenu);
			StilMenu->insertItem( tr("Invert"), this, SLOT(InvertPict()));
		}
	}
}

void ScribusApp::slotStoryEditor()
{
	if (view->SelItem.count() != 0)
	{
		PageItem *b = view->SelItem.at(0);
		StoryEditor* dia = new StoryEditor(this, doc, b);
		CurrStED = dia;
		connect(dia, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
		connect(dia, SIGNAL(EditSt()), this, SLOT(slotEditStyles()));
		if (dia->exec())
		{
			if (dia->TextChanged)
				dia->updateTextFrame();
		}
		else
			view->DrawNew();
		buildFontMenu();
		CurrStED = NULL;
		delete dia;
	}
}

void ScribusApp::defaultCrashHandler (int sig)
{
	static int crashRecursionCounter = 0;
	crashRecursionCounter++;
	signal(SIGALRM, SIG_DFL);
	if (crashRecursionCounter < 2)
	{
		crashRecursionCounter++;
		QMessageBox::critical(ScApp, tr("Scribus Crash"), tr("Scribus crashes due to Signal #%1").arg(sig), tr("OK"));
		alarm(300);
		ScApp->emergencySave();
	}
	exit(255);
}

void ScribusApp::emergencySave()
{
	std::cout << "Calling Emergency Save" << std::endl;
	QWidgetList windows = ScApp->wsp->windowList();
	if (!windows.isEmpty())
	{
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
		{
			ActWin = (ScribusWin*)windows.at(i);
			doc = ActWin->doc;
			view = ActWin->view;
			doc->setUnModified();
			if (doc->hasName)
			{
				std::cout << "Saving: " << doc->DocName+".emergency" << std::endl;
				doc->ASaveTimer->stop();
				disconnect(ActWin, SIGNAL(Schliessen()), ScApp, SLOT(DoFileClose()));
				ScriXmlDoc *ss = new ScriXmlDoc();
				ss->WriteDoc(doc->DocName+".emergency", doc, 0);
				delete ss;
			}
			view->close();
			for (uint a = 0; a<doc->Pages.count(); ++a)
			{
				delete doc->Pages.at(a);
			}
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
			PageItem *b = view->SelItem.at(0);
			TabManager *dia = new TabManager(this, doc->docUnitIndex, b->TabValues, b->Width);
			if (dia->exec())
			{
				b->TabValues = dia->tmpTab;
				view->RefreshItem(b);
				slotDocCh();
			}
			delete dia;
		}
	}
}

void ScribusApp::SearchText()
{
	PageItem *b = view->SelItem.at(0);
	setAppMode(EditMode);
	b->CPos = 0;
	SearchReplace* dia = new SearchReplace(this, doc, &Prefs, b);
	connect(dia, SIGNAL(NewFont(QString)), this, SLOT(SetNewFont(QString)));
	connect(dia, SIGNAL(NewAbs(int)), this, SLOT(setAbsValue(int)));
	dia->exec();
	disconnect(dia, SIGNAL(NewFont(QString)), this, SLOT(SetNewFont(QString)));
	disconnect(dia, SIGNAL(NewAbs(int)), this, SLOT(setAbsValue(int)));
	delete dia;
	slotSelect();
}

void ScribusApp::GimpExited()
{
	int ex = 0;
	if ( ExternalApp != 0 )
	{
		ex = ExternalApp->exitStatus();
		delete ExternalApp;
	}
	ExternalApp = 0;
}

/* call gimp and wait uppon completion */
void ScribusApp::CallGimp()
{
	QStringList cmd;
	if (view->SelItem.count() != 0)
	{
		if (ExternalApp != 0)
		{
			QString mess = tr("The Program")+" "+Prefs.gimp_exe;
			mess += "\n" + tr("is already running!");
			QMessageBox::information(this, tr("Information"), mess, 1, 0, 0);
			return;
		}
		PageItem *b = view->SelItem.at(0);
		if (b->PicAvail)
		{
			b->pixmOrg = QImage();
			ExternalApp = new QProcess(NULL);
            cmd = QStringList::split(" ", Prefs.gimp_exe);
			cmd.append(b->Pfile);
			ExternalApp->setArguments(cmd);
			if ( !ExternalApp->start() )
			{
				delete ExternalApp;
				ExternalApp = 0;
				QString mess = tr("The Program")+" "+Prefs.gimp_exe;
				mess += "\n" + tr("is missing!");
				QMessageBox::critical(this, tr("Warning"), mess, 1, 0, 0);
				return;
			}
			connect(ExternalApp, SIGNAL(processExited()), this, SLOT(GimpExited()));
		}
	}
}

void ScribusApp::slotTest()
{
}

void ScribusApp::slotTest2()
{
}

