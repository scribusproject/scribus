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
#include "pdfopts.h"
#include "inspage.h"
#include "delpages.h"
#include "movepage.h"
#include "helpbrowser.h"
#include "scribusXml.h"
#include "libabout/about.h"
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

#ifndef _MSC_VER   // jjsa 21-03-2004 (lint complains)
#define _MSC_VER -1
#endif

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#if (_MSC_VER == -1 )   // jjsa 21-03-2004 (lint complains)
#undef _MSC_VER
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


extern QPixmap loadIcon(QString nam);
extern bool overwrite(QWidget *parent, QString filename);
extern void CopyPageItem(struct CLBuf *Buffer, PageItem *b);
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
int PolyC;
int PolyFd;
double PolyF;
bool PolyS;
double PolyR;
double UmReFaktor;
QString DocDir;
ScribusApp* ScApp;

ScribusApp::ScribusApp()
{} // ScribusApp::ScribusApp()

void ScribusApp::initGui()
{
	splash = new SplashScreen();
	splash->setStatus(QObject::tr("Initializing..."));
	initScribus();
	splash->close();
	delete splash;
}

void ScribusApp::initScribus()
{
	ScApp = this;
	CurrStED = NULL;
	setCaption( tr("Scribus " VERSION));
	setKeyCompression(false);
	setIcon(loadIcon("AppIcon.png"));
	MenuItemsFile.clear();
	initMenuBar();
	initStatusBar();
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
	QString Pff = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus");
	QFileInfo Pffi = QFileInfo(Pff);
	if (Pffi.exists())
	{
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = QDir::homeDirPath();
	}
	else
	{
		QDir di = QDir();
		di.mkdir(Pff);
		PrefsPfad = Pff;
		QString OldPR = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus.rc");
		QFileInfo OldPi = QFileInfo(OldPR);
		if (OldPi.exists())
			moveFile(OldPR, Pff+"/scribus.rc");
		QString OldPR2 = QDir::convertSeparators(QDir::homeDirPath()+"/.scribusfont.rc");
		QFileInfo OldPi2 = QFileInfo(OldPR2);
		if (OldPi2.exists())
			moveFile(OldPR2, Pff+"/scribusfont.rc");
		QString OldPR3 = QDir::convertSeparators(QDir::homeDirPath()+"/.scribusscrap.scs");
		QFileInfo OldPi3 = QFileInfo(OldPR3);
		if (OldPi3.exists())
			moveFile(OldPR3, Pff+"/scrap.scs");
	}
	/** Erstelle Fontliste */
	NoFonts = false;
	BuFromApp = false;
	splash->setStatus( tr("Searching for Fonts"));
	GetAllFonts();
	if (NoFonts)
	{
		QString mess = tr("There are no Postscript-Fonts on your System");
		mess += "\n" + tr("Exiting now");
		QMessageBox::critical(this, tr("Fatal Error"), mess, 1, 0, 0);
	}
	else
	{
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
		BuildFontMenu();
		SCFontsIterator it(Prefs.AvailFonts);
		Prefs.DefFont = it.currentKey();
		Prefs.DefSize = 120;
		Prefs.AppFontSize = qApp->font().pointSize();
		/** Default Farbenliste */
		Prefs.DColors.clear();
		QString pfadC = PREL;
		QString pfadC2 = pfadC + "/lib/scribus/rgbscribus.txt";
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
		DispX = 10;
		DispY = 10;
		Prefs.Wheelval = 40;
		Prefs.GrabRad = 4;
		Prefs.GuideRad = 10;
		Prefs.DminGrid = 20;
		Prefs.DmajGrid = 100;
		DocNr = 1;
		Prefs.DminColor = QColor(green);
		Prefs.DmajColor = QColor(green);
		Prefs.DpapColor = QColor(white);
		Prefs.DmargColor = QColor(blue);
		Prefs.guideColor = QColor(darkBlue);
		Prefs.baseColor = QColor(lightGray);
		Prefs.DVHoch = 33;
		Prefs.DVHochSc = 100;
		Prefs.DVTief = 33;
		Prefs.DVTiefSc = 100;
		Prefs.DVKapit = 75;
		Prefs.GUI = "Default";
		Prefs.Dpen = "Black";
		Prefs.Dbrush = "Black";
		Prefs.Dshade = 100;
		Prefs.Dshade2 = 100;
		Prefs.DLineArt = SolidLine;
		Prefs.Dwidth = 1;
		Prefs.DpenLine = "Black";
		Prefs.DpenText = "Black";
		Prefs.DCols = 1;
		Prefs.DGap = 0.0;
		Prefs.DshadeLine = 100;
		Prefs.DLstyleLine = SolidLine;
		Prefs.DwidthLine = 1;
		Prefs.MagMin = 10;
		Prefs.MagMax = 800;
		Prefs.MagStep = 25;
		Prefs.DbrushPict = "White";
		Prefs.ShadePict = 100;
		Prefs.ScaleX = 1;
		Prefs.ScaleY = 1;
		Prefs.Before = true;
		Prefs.Einheit = 0;
		UmReFaktor = 1.0;
		PolyC = 4;
		PolyF = 0.5;
		PolyS = false;
		PolyFd = 0;
		PolyR = 0;
		Prefs.PolyC = PolyC;
		Prefs.PolyF = PolyF;
		Prefs.PolyFd = PolyFd;
		Prefs.PolyS = PolyS;
		Prefs.PolyR = PolyR;
		Prefs.Werkv = true;
		Prefs.WerkvP = true;
		Prefs.Mpalv = false;
		Prefs.Mapalv = false;
		Prefs.Tpalv = false;
		Prefs.SCpalv = false;
		Prefs.Lpalv = false;
		Prefs.Bopalv = false;
		Prefs.Mpalx = 0;
		Prefs.Mpaly = 0;
		Prefs.Mapalx = 0;
		Prefs.Mapaly = 0;
		Prefs.Tpalx = 0;
		Prefs.Tpaly = 0;
		Prefs.SCpalx = 0;
		Prefs.SCpaly = 0;
		Prefs.SCpalw = 100;
		Prefs.SCpalh = 200;
		Prefs.Sepalx = 0;
		Prefs.Sepaly = 0;
		Prefs.Bopalx = 0;
		Prefs.Bopaly = 0;
		Prefs.Lpalx = 0;
		Prefs.Lpaly = 0;
		Prefs.PSize = 40;
		Prefs.SaveAtQ = true;
		Prefs.FramesShown = true;
		Prefs.GridShown = false;
		Prefs.MarginsShown = true;
		Prefs.GuidesShown = true;
		Prefs.BaseShown = false;
		Prefs.ClipMargin = true;
		Prefs.GCRMode = true;
		Prefs.PagesSbS = true;
		Prefs.RecentDocs.clear();
		Prefs.RecentDCount = 5;
		Prefs.RandFarbig = false;
		Prefs.AutoLine = 20;
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
		Prefs.PDFTransparency = false;
		Prefs.AutoSave = false;
		Prefs.AutoSaveTime = 600000;
		Prefs.DisScale = 1.0;
		Prefs.DocDir = QDir::homeDirPath();
		Prefs.ProfileDir = "";
		Prefs.ScriptDir = "";
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
		Prefs.BaseGrid = 14.4;
		Prefs.BaseOffs = 0.0;
		Prefs.STEcolor = QColor(white);
		Prefs.STEfont = font().toString();
		PDef.Pname = "";
		PDef.Dname = "";
		PDef.Command = "";
		PrinterUsed = false;
		resize(610, 600);
		QVBox* vb = new QVBox( this );
		vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
		wsp = new QWorkspace( vb );
		setCentralWidget( vb );
		connect(wsp, SIGNAL(windowActivated(QWidget *)), this, SLOT(newActWin(QWidget *)));
		Tpal = new Tree(this, 0);
		Mpal = new Mpalette(this, &Prefs);
		Mpal->Cpal->SetColors(Prefs.DColors);
		Npal = new NodePalette(this);
		Lpal = new LayerPalette(this);
		ScBook = new Biblio(this, &Prefs);
		Sepal = new SeitenPal(this);
		BookPal = new BookPalette(this);
		MaPal = new Measurements(this);
		MaPal->hide();
		CMSavail = false;
		keyrep = false;
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
		SetKeyEntry(56, tr("Smart Hyphen"), 0, CTRL+Key_Minus);
		SetKeyEntry(57, tr("Align Left"), 0, CTRL+Key_L);
		SetKeyEntry(58, tr("Align Right"), 0, CTRL+Key_R);
		SetKeyEntry(59, tr("Align Center"), 0, CTRL+Key_E);
		SetKeyEntry(60, tr("Insert Page Number"), 0, CTRL+Key_NumberSign);
		SetKeyEntry(61, tr("Attach Text to Path"), PfadT, 0);
		SetKeyEntry(62, tr("Show Layers"), viewLpal, 0);
		SetKeyEntry(63, tr("Javascripts..."), jman, 0);
		SetKeyEntry(64, tr("Undo"), edUndo, CTRL+Key_Z);
		SetKeyEntry(65, tr("Show Page Palette"), viewSepal, 0);
		SetKeyEntry(66, tr("Lock/Unlock"), LockOb, CTRL+Key_F);
		SetKeyEntry(67, tr("Non Breaking Space"), 0, CTRL+Key_Space);
		splash->setStatus( tr("Reading Preferences"));
		ReadPrefs();
		if (Prefs.DefFont == "")
			Prefs.DefFont = it.currentKey();
		splash->setStatus( tr("Getting ICC Profiles"));
		GetCMSProfiles();
		splash->setStatus( tr("Init Hyphenator"));
		InitHyphenator();
		Mpal->Cpal->UseTrans(Prefs.PDFTransparency);
		Mpal->Fonts->RebuildList(&Prefs);
		DocDir = Prefs.DocDir;
		splash->setStatus("");
		splash->setStatus( tr("Setting up Shortcuts"));
		SetShortCut();
		if (CMSavail)
		{
			settingsMenu->insertItem( tr("&Color Management..."), this , SLOT(SetCMSPrefs()));
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
		splash->setStatus( tr("Reading Scrapbook"));
		QString SCf = PrefsPfad+"/scrap.scs";
		QFileInfo SCfi = QFileInfo(SCf);
		if (SCfi.exists())
			ScBook->BibWin->ReadContents(SCf);
		ScBook->ScFilename = SCf;
		ScBook->AdjustMenu();
		HaveGS = system(Prefs.gs_exe+" -h > /dev/null 2>&1");
		HavePngAlpha = system(Prefs.gs_exe+" -sDEVICE=pngalpha -c quit > /dev/null 2>&1");
		splash->setStatus( tr("Initializing Plugins"));
		InitPlugs(splash);
		ClipB = QApplication::clipboard();
		PalettesStat[0] = false;
		GuidesStat[0] = false;
		connect(WerkTools, SIGNAL(NewMode(int)), this, SLOT(ModeFromTB(int)));
		connect(WerkTools, SIGNAL(Schliessen()), this, SLOT(ToggleTools()));
		connect(WerkToolsP, SIGNAL(NewMode(int)), this, SLOT(ModeFromTB(int)));
		connect(WerkToolsP, SIGNAL(Schliessen()), this, SLOT(TogglePDFTools()));
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
		connect(Mpal->Cpal->GradEdit->Preview, SIGNAL(gradientChanged()), this, SLOT(updtGradFill()));
		connect(Mpal->Cpal, SIGNAL(gradientChanged()), this, SLOT(updtGradFill()));
		connect(Mpal->Cpal, SIGNAL(QueryItem()), this, SLOT(GetBrushPen()));
		connect(Tpal, SIGNAL(Schliessen()), this, SLOT(ToggleTpal()));
		connect(Tpal, SIGNAL(CloseMpal()), this, SLOT(ToggleMpal()));
		connect(Tpal, SIGNAL(CloseSpal()), this, SLOT(ToggleBpal()));
		connect(Tpal, SIGNAL(SelectElement(int, int)), this, SLOT(SelectFromOutl(int, int)));
		connect(Tpal, SIGNAL(SelectSeite(int)), this, SLOT(SelectFromOutlS(int)));
		connect(Tpal, SIGNAL(ToggleAllPalettes()), this, SLOT(ToggleAllPalettes()));
		connect(Mpal->Spal, SIGNAL(NewStyle(int)), this, SLOT(setNewAbStyle(int)));
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
		connect(ScBook, SIGNAL(Schliessen()), this, SLOT(ToggleBpal()));
		connect(ScBook->BibWin, SIGNAL(ToggleAllPalettes()), this, SLOT(ToggleAllPalettes()));
		connect(ScBook->BibWin, SIGNAL(Schliessen()), this, SLOT(ToggleBpal()));
		connect(ScBook->BibWin, SIGNAL(CloseTpal()), this, SLOT(ToggleTpal()));
		connect(ScBook->BibWin, SIGNAL(CloseMpal()), this, SLOT(ToggleMpal()));
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
		connect(recentMenu, SIGNAL(activated(int)), this, SLOT(LoadRecent(int)));
		connect(ColorMenC, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
		connect(ShadeMenu, SIGNAL(activated(int)), this, SLOT(setItemShade(int)));
		connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
		connect(SizeTMenu, SIGNAL(activated(int)), this, SLOT(setItemFSize(int)));
		connect(TypeStyleMenu, SIGNAL(activated(int)), this, SLOT(setItemTypeStyle(int)));
		connect(AliMenu, SIGNAL(activated(int)), this, SLOT(setItemTextAli(int)));
		connect(this, SIGNAL(TextIFont(QString)), this, SLOT(AdjustFontMenu(QString)));
		connect(this, SIGNAL(TextISize(int)), this, SLOT(setFSizeMenu(int)));
		connect(this, SIGNAL(TextISize(int)), Mpal, SLOT(setSize(int)));
		connect(this, SIGNAL(TextUSval(double)), Mpal, SLOT(setExtra(double)));
		connect(this, SIGNAL(TextStil(int)), Mpal, SLOT(setStil(int)));
		connect(this, SIGNAL(TextScale(int)), Mpal, SLOT(setTScale(int)));
		connect(this, SIGNAL(TextFarben(QString, QString, int, int)), Mpal, SLOT(setActFarben(QString, QString, int, int)));
		connect(ClipB, SIGNAL(dataChanged()), this, SLOT(ClipChange()));
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
}

void ScribusApp::initMenuBar()
{
	int MenID;
	QFont tmp;
	recentMenu = new QPopupMenu();
	RecentDocs.clear();
	fileMenu=new QPopupMenu();
	fid12 = fileMenu->insertItem(loadIcon("DateiNeu16.png"), tr("&New"), this, SLOT(slotFileNew()), CTRL+Key_N);
	fid13 = fileMenu->insertItem(loadIcon("DateiOpen16.png"), tr("&Open..."), this, SLOT(slotDocOpen()), CTRL+Key_O);
	fid14 = fileMenu->insertItem( tr("Open &Recent"), recentMenu);
	SetKeyEntry(0, tr("New"), fid12, CTRL+Key_N);
	SetKeyEntry(1, tr("Open..."), fid13, CTRL+Key_O);
	fileMenu->insertSeparator();
	fid1 = fileMenu->insertItem(loadIcon("DateiClos16.png"), tr("&Close"), this, SLOT(slotFileClose()), CTRL+Key_W);
	SetKeyEntry(2, tr("Close"), fid1, CTRL+Key_W);
	fileMenu->setItemEnabled(fid1, 0);
	MenuItemsFile.append(fid1);
	fid4 = fileMenu->insertItem(loadIcon("DateiSave16.png"), tr("&Save"), this, SLOT(slotFileSave()), CTRL+Key_S);
	SetKeyEntry(3, tr("Save"), fid4, CTRL+Key_S);
	fileMenu->setItemEnabled(fid4, 0);
	fid5 = fileMenu->insertItem( loadIcon("filesaveas.png"), tr("Save &As..."), this, SLOT(slotFileSaveAs()));
	SetKeyEntry(4, tr("Save as..."), fid5, 0);
	fileMenu->setItemEnabled(fid5, 0);
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
	fid9 = fileMenu->insertItem(loadIcon("DateiPrint16.png"), tr("&Print..."), this, SLOT(slotFilePrint()), CTRL+Key_P);
	fileMenu->setItemEnabled(fid9, 0);
	MenuItemsFile.append(fid9);
	SetKeyEntry(7, tr("Print..."), fid9, CTRL+Key_P);
	fileMenu->insertSeparator();
	MenID = fileMenu->insertItem(loadIcon("exit.png"), tr("&Quit"), this, SLOT(slotFileQuit()), CTRL+Key_Q);
	SetKeyEntry(8, tr("Quit"), MenID, CTRL+Key_Q);
	editMenu = new QPopupMenu();
	edUndo = editMenu->insertItem( tr("&Undo"), this, SLOT(UnDoAction()), CTRL+Key_Z);
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
	editMenu->setItemEnabled(edUndo, 0);
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
	extraMenu=new QPopupMenu();
	MenID = extraMenu->insertItem( tr("&Manage Pictures"), this, SLOT(StatusPic()));
	SetKeyEntry(51, tr("Manage Pictures"), MenID, 0);
	hyph = extraMenu->insertItem( tr("&Hyphenate Text"), this, SLOT(doHyphenate()));
	extraMenu->setItemEnabled(hyph, 0);
	SetKeyEntry(50, tr("Hyphenate Text"), hyph, 0);

	settingsMenu = new QPopupMenu();
	tip = settingsMenu->insertItem( tr("Toolti&ps"), this, SLOT(ToggleTips()));
	toolbarMenuTools = settingsMenu->insertItem( tr("&Tools"), this, SLOT(ToggleTools()));
	toolbarMenuPDFTools = settingsMenu->insertItem( tr("P&DF Tools"), this, SLOT(TogglePDFTools()));
	SetKeyEntry(45, tr("Tools"), toolbarMenuTools, 0);
	SetKeyEntry(55, tr("Tooltips"), tip, 0);
	tipsOn = true;
	settingsMenu->setItemChecked(tip, tipsOn);
	settingsMenu->insertSeparator();
	MenID = settingsMenu->insertItem( tr("P&references..."), this , SLOT(slotPrefsOrg()));
	MenID = settingsMenu->insertItem( tr("&Fonts..."), this , SLOT(slotFontOrg()));
	SetKeyEntry(17, tr("Fonts..."), MenID, 0);
	settingsMenu->insertItem( tr("&Hyphenator..."), this, SLOT(configHyphenator()));
	settingsMenu->insertItem( tr("&Keyboard Shortcuts..."), this, SLOT(DefKB()));

	windowsMenu = new QPopupMenu();
	windowsMenu->setCheckable( true );
	connect(windowsMenu, SIGNAL(aboutToShow()), this, SLOT(windowsMenuAboutToShow()));
	helpMenu=new QPopupMenu();
	MenID = helpMenu->insertItem( tr("&About Scribus"), this, SLOT(slotHelpAbout()));
	SetKeyEntry(52, tr("About Scribus"), MenID, 0);
	MenID = helpMenu->insertItem( tr("About &Qt"), this, SLOT(slotHelpAboutQt()));
	SetKeyEntry(53, tr("About Qt"), MenID, 0);
	helpMenu->insertSeparator();
	MenID = helpMenu->insertItem( tr("Scribus &Manual..."), this, SLOT(slotOnlineHelp()));
	SetKeyEntry(54, tr("Online-Help..."), MenID, 0);
	//	editMenu->insertItem( tr("Test"), this, SLOT(slotTest()));
	//	helpMenu->insertItem( tr("Test2"), this, SLOT(slotTest2()));
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
	SetMen = menuBar()->insertItem( tr("&Settings"), settingsMenu );
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
	char *ar_sizes[] = {" 7", " 9", "10", "11", "12", "14", "18", "24", "36", "48", "60", "72"};
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
}

void ScribusApp::initStatusBar()
{
	FMess = new QLabel(statusBar(), "ft");
	FMess->setText("           ");
	statusBar()->addWidget(FMess, 3, true);
	FProg = new QProgressBar(statusBar(), "p");
	FProg->setCenterIndicator(true);
	FProg->setFixedWidth( 100 );
	statusBar()->addWidget(FProg, 0, true);
	FProg->reset();
	XMess = new QLabel(statusBar(), "xt");
	XMess->setText( tr("X-Pos:"));
	statusBar()->addWidget(XMess, 0, true);
	XDat = new QLabel(statusBar(), "dt");
	statusBar()->addWidget(XDat, 1, true);
	XDat->setText("         ");
	YMess = new QLabel(statusBar(), "yt");
	YMess->setText( tr("Y-Pos:"));
	statusBar()->addWidget(YMess, 0, true);
	YDat = new QLabel(statusBar(), "ydt");
	statusBar()->addWidget(YDat, 1, true);
	YDat->setText("         ");
}

void ScribusApp::ReportMP(double xp, double yp)
{
	QString tmp, tmp2;
	int multiplier, precision;
	double divisor;
	switch (doc->Einheit)
	{
	case 0:
		tmp2 = tr(" pt");
		multiplier = 100;
		divisor = 100.0;
		precision = 2;
		break;
	case 1:
		tmp2 = tr(" mm");
		multiplier = 1000;
		divisor = 1000.0;
		precision = 3;
		break;
	case 2:
		tmp2 = tr(" in");
		multiplier = 10000;
		divisor = 10000.0;
		precision = 4;
		break;
	case 3:
		tmp2 = tr(" p");
		multiplier = 100;
		divisor = 100.0;
		precision = 2;
		break;
	default:  // jjsa 21-03-2004 added default (complains for lint)
		tmp2 = tr(" pt");
		multiplier = 100;
		divisor = 100.0;
		precision = 2;
		break;
	}
	XDat->setText(tmp.setNum(qRound(xp*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
	YDat->setText(tmp.setNum(qRound(yp*UmReFaktor * multiplier) / divisor, 'f', precision)+tmp2);
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
	Pti *it;
	int FirstSel = 0;
	bool first = false;
	for (it = b->Ptext.first(); it != 0; it = b->Ptext.next())
	{
		if (it->cselect)
		{
			first = true;
			b->Ptext.remove();
			it = b->Ptext.prev();
			if (it == 0)
				it = b->Ptext.first();
		}
		if (!first)
			FirstSel++;
	}
	if (b->Ptext.count() != 0)
	{
		if (b->Ptext.first()->cselect)
		{
			b->Ptext.remove();
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
	if (b->Ptext.count() != 0)
	{
		int ChPos = QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1));
		doc->CurrentStyle = b->Ptext.at(ChPos)->cstyle & 127;
		doc->CurrentABStil = b->Ptext.at(ChPos)->cab;
		setAbsValue(doc->CurrentABStil);
		Mpal->setAli(doc->CurrentABStil);
		doc->CurrFont = b->Ptext.at(ChPos)->cfont;
		doc->CurrFontSize = b->Ptext.at(ChPos)->csize;
		doc->CurrTextFill = b->Ptext.at(ChPos)->ccolor;
		doc->CurrTextFillSh = b->Ptext.at(ChPos)->cshade;
		doc->CurrTextStroke = b->Ptext.at(ChPos)->cstroke;
		doc->CurrTextStrokeSh = b->Ptext.at(ChPos)->cshade2;
		doc->CurrTextScale = b->Ptext.at(ChPos)->cscale;
		emit TextFarben(doc->CurrTextStroke, doc->CurrTextFill, doc->CurrTextStrokeSh, doc->CurrTextFillSh);
		emit TextIFont(doc->CurrFont);
		emit TextISize(doc->CurrFontSize);
		emit TextUSval(b->Ptext.at(ChPos)->cextra);
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
	struct Pti *hg;
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
		NoFrameEdit();
		slotSelect();
		return;
	}
	ButtonState buttonState = k->state();
	if ((HaveDoc) && (!view->LE->hasFocus()) && (!view->PGS->PageCombo->hasFocus()))
	{
		if ((doc->AppMode != 7) && (doc->ActPage->SelItem.count() == 0))
		{
			switch (kk)
			{
			case Key_Space:
				keyrep = false;
				if (doc->AppMode == 23)
					setAppMode(1);
				else
				{
					setAppMode(23);
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
		if (doc->ActPage->SelItem.count() != 0)
		{
			PageItem *b = doc->ActPage->SelItem.at(0);
			if (kk == Key_F9)
			{
				keyrep = false;
				if (doc->AppMode == 7)
					setAppMode(1);
				else
					setAppMode(7);
				return;
			}
			switch (doc->AppMode)
			{
			case 1:
				switch (kk)
				{
				case Key_Backspace:
				case Key_Delete:
					if (!doc->EditClip)
					{
						if ((!b->Locked) && (!((b->isTableItem) && (b->isSingleSel))))
							doc->ActPage->DeleteItem();
					}
					break;
				case Key_Prior:
					if (!b->Locked)
						doc->ActPage->RaiseItem();
					break;
				case Key_Next:
					if (!b->Locked)
						doc->ActPage->LowerItem();
					break;
				case Key_Left:
					if (!k->isAutoRepeat())
					{
						doc->ActPage->storeUndoInf(b);
						doc->UnData.UnCode = 1;
						doc->UnDoValid = true;
						CanUndo();
					}
					if (!b->Locked)
					{
						if ( buttonState & ShiftButton )
							doc->ActPage->moveGroup(-10, 0);
						else if ( buttonState & ControlButton )
							doc->ActPage->moveGroup(-0.1, 0);
						else
							doc->ActPage->moveGroup(-1, 0);
					}
					break;
				case Key_Right:
					if (!k->isAutoRepeat())
					{
						doc->ActPage->storeUndoInf(b);
						doc->UnData.UnCode = 1;
						doc->UnDoValid = true;
						CanUndo();
					}
					if (!b->Locked)
					{
						if ( buttonState & ShiftButton )
							doc->ActPage->moveGroup(10, 0);
						else if ( buttonState & ControlButton )
							doc->ActPage->moveGroup(0.1, 0);
						else
							doc->ActPage->moveGroup(1, 0);
					}
					break;
				case Key_Up:
					if (!k->isAutoRepeat())
					{
						doc->ActPage->storeUndoInf(b);
						doc->UnData.UnCode = 1;
						doc->UnDoValid = true;
						CanUndo();
					}
					if (!b->Locked)
					{
						if ( buttonState & ShiftButton )
							doc->ActPage->moveGroup(0, -10);
						else if ( buttonState & ControlButton )
							doc->ActPage->moveGroup(0, -0.1);
						else
							doc->ActPage->moveGroup(0, -1);
					}
					break;
				case Key_Down:
					if (!k->isAutoRepeat())
					{
						doc->ActPage->storeUndoInf(b);
						doc->UnData.UnCode = 1;
						doc->UnDoValid = true;
						CanUndo();
					}
					if (!b->Locked)
					{
						if ( buttonState & ShiftButton )
							doc->ActPage->moveGroup(0, 10);
						else if ( buttonState & ControlButton )
							doc->ActPage->moveGroup(0, 0.1);
						else
							doc->ActPage->moveGroup(0, 1);
					}
					break;
				default:
					if (b->PType == 4)
					{
						if ((kk + KeyMod) == Prefs.KeyActions[59].KeyID)
						{
							setNewAbStyle(1);
							b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b);
						}
						if ((kk + KeyMod) == Prefs.KeyActions[58].KeyID)
						{
							setNewAbStyle(2);
							b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b);
						}
						if ((kk + KeyMod) == Prefs.KeyActions[57].KeyID)
						{
							setNewAbStyle(0);
							b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b);
						}
					}
					break;
				}
				slotDocCh();
				break;
			case 7:
				int oldPos = b->CPos; // 15-mar-2004 jjsa for cursor movement with Shift + Arrow key
				doc->ActPage->oldCp = b->CPos;
				if (b->PType == 4)
				{
					doc->ActPage->slotDoCurs(false);
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
							doc->ActPage->deselectAll(b);
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
								hg = new Pti;
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
								hg->cab = doc->CurrentABStil;
								if (doc->Vorlagen[doc->CurrentABStil].Font != "")
								{
									hg->cfont = doc->Vorlagen[doc->CurrentABStil].Font;
									hg->csize = doc->Vorlagen[doc->CurrentABStil].FontSize;
								}
								hg->cextra = 0;
								hg->xp = 0;
								hg->yp = 0;
								hg->PRot = 0;
								hg->PtransX = 0;
								hg->PtransY = 0;
								b->Ptext.insert(b->CPos, hg);
								b->CPos += 1;
								b->Dirty = true;
								b->Tinput = true;
								setTBvals(b);
								doc->ActPage->RefreshItem(b, true);
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
						len = static_cast<int>(b->Ptext.count());
						if ( pos == len )
							pos--;
						if ( (buttonState & ControlButton) == 0 )
						{
							alty =  b->Ptext.at(pos)->yp;
							c = b->Ptext.at(pos)->ch.at(0).latin1();
							if ( c == 13 ) // new line, position is wrong
								if ( --pos > 0 )
									alty =  b->Ptext.at(pos)->yp;
							// check for yp at actual position
							if ( pos < len )
							{
								altx =  b->Ptext.at(pos)->yp;
								if ( altx > alty )
								{
									// we was at begin of line
									break;
								}
							}
							while (  pos > 0 && b->Ptext.at(pos-1)->yp == alty )
								pos--;
							if ( b->Ptext.at(pos)->ch.at(0).latin1() == 13 )
								pos++;
						}
						else
						{
							// paragraph begin
							if ( pos < len &&
							        b->Ptext.at(pos)->ch.at(0).latin1() == 13 )
								pos--;
							while(pos > 0 )
								if ( b->Ptext.at(pos)->ch.at(0).latin1() == 13 )
								{
									pos++;
									break;
								}
								else
									pos--;
						}
						b->CPos = pos;
						if ( buttonState & ShiftButton )
							doc->ActPage->ExpandSel(b, -1, oldPos);
						break;
					case Key_End:
						// go to end of line
						len = static_cast<int>(b->Ptext.count());
						if ( b->CPos >= len )
							break; // at end of frame
						if ( (buttonState & ControlButton) == 0 )
						{
							if ( b->CPos < len && b->Ptext.at(b->CPos)->ch.at(0).latin1() == 13 )
							{
								// at end of paragraph and therefore line
								break;
							}
							alty =  b->Ptext.at(b->CPos)->yp;
							while (  b->CPos < len-1 &&  b->Ptext.at(b->CPos+1)->yp == alty )
								b->CPos++;

							if ( b->CPos < len -1 )
								c = b->Ptext.at(b->CPos+1)->ch.at(0).latin1();
							else if ( b->CPos == len - 1 )
								c = 13;
							else
								c = 0;
							if ( c == 13 )
								b->CPos++;
						}
						else
						{
							// go to end of paragraph
							if ( b->Ptext.at(b->CPos)->ch.at(0).latin1() == 13 )
							{
								break;
							}
							pos = b->CPos;
							while ( pos < len )
							{
								if ( b->Ptext.at(pos)->ch.at(0).latin1() == 13 )
									break;
								else
									pos++;
							}
							b->CPos = pos;
						}
						if ( buttonState & ShiftButton )
							doc->ActPage->ExpandSel(b, 1, oldPos);
						break;
					case Key_Down:
						if (b->CPos != static_cast<int>(b->Ptext.count()))
						{
							alty = b->Ptext.at(b->CPos)->yp;
							altx = b->Ptext.at(b->CPos)->xp;
							do
							{
								b->CPos += 1;
								if (b->CPos == static_cast<int>(b->Ptext.count()))
									break;
								if (b->Ptext.at(b->CPos)->yp > alty)
								{
									if (b->Ptext.at(b->CPos)->xp >= altx)
										break;
								}
							}
							while (b->CPos < static_cast<int>(b->Ptext.count()));
							if ( buttonState & ShiftButton )
							{
								if ( buttonState & AltButton )
									b->CPos = b->Ptext.count();
								doc->ActPage->ExpandSel(b, 1, oldPos);
							}
							else
								if (b->CPos == static_cast<int>(b->Ptext.count()))
									if (b->NextBox != 0)
									{
										if (b->NextBox->Ptext.count() != 0)
										{
											b->OwnPage->Deselect(true);
											b->NextBox->CPos = 0;
											doc->ActPage = b->NextBox->OwnPage;
											b->NextBox->OwnPage->SelectItemNr(b->NextBox->ItemNr);
											b = b->NextBox;
										}
									}
						}
						else
						{
							if (b->NextBox != 0)
							{
								if (b->NextBox->Ptext.count() != 0)
								{
									b->OwnPage->Deselect(true);
									b->NextBox->CPos = 0;
									doc->ActPage = b->NextBox->OwnPage;
									b->NextBox->OwnPage->SelectItemNr(b->NextBox->ItemNr);
									b = b->NextBox;
								}
							}
						}
						if ( b->HasSel )
							doc->ActPage->RefreshItem(b, true);
						setTBvals(b);
						break;
					case Key_Up:
						if (b->CPos > 0)
						{
							if (b->CPos == static_cast<int>(b->Ptext.count()))
								b->CPos -= 1;
							alty = b->Ptext.at(b->CPos)->yp;
							altx = b->Ptext.at(b->CPos)->xp;
							if (b->CPos > 0)
							{
								do
								{
									b->CPos -= 1;
									if (b->CPos == 0)
										break;
									if (b->Ptext.at(b->CPos)->yp < alty)
									{
										if (b->Ptext.at(b->CPos)->xp <= altx)
											break;
									}
								}
								while (b->CPos > 0);
							}
							if ( buttonState & ShiftButton )
							{
								if ( buttonState & AltButton )
									b->CPos = 0;
								doc->ActPage->ExpandSel(b, -1, oldPos);
							}
							else
								if (b->CPos == 0)
								{
									if (b->BackBox != 0)
									{
										b->OwnPage->Deselect(true);
										b->BackBox->CPos = b->BackBox->Ptext.count();
										doc->ActPage = b->BackBox->OwnPage;
										b->BackBox->OwnPage->SelectItemNr(b->BackBox->ItemNr);
										b = b->BackBox;
									}
								}
						}
						else
						{
							b->CPos = 0;
							if (b->BackBox != 0)
							{
								b->OwnPage->Deselect(true);
								b->BackBox->CPos = b->BackBox->Ptext.count();
								doc->ActPage = b->BackBox->OwnPage;
								b->BackBox->OwnPage->SelectItemNr(b->BackBox->ItemNr);
								b = b->BackBox;
							}
						}
						if ( b->HasSel )
							doc->ActPage->RefreshItem(b);
						setTBvals(b);
						break;
					case Key_Prior:
						b->CPos = 0;
						if ( buttonState & ShiftButton )
							doc->ActPage->ExpandSel(b, -1, oldPos);
						setTBvals(b);
						break;
					case Key_Next:
						b->CPos = static_cast<int>(b->Ptext.count());
						if ( buttonState & ShiftButton )
							doc->ActPage->ExpandSel(b, 1, oldPos);
						setTBvals(b);
						break;
					case Key_Left:
						if ( buttonState & ControlButton )
						{
							doc->ActPage->setNewPos(b, oldPos, b->Ptext.count(),-1);
							if ( buttonState & ShiftButton )
								doc->ActPage->ExpandSel(b, -1, oldPos);
						}
						else if ( buttonState & ShiftButton )
						{
							b->CPos--;
							if ( b->CPos < 0 )
								b->CPos = 0;
							else
								doc->ActPage->ExpandSel(b, -1, oldPos);
						}
						else
						{
							b->CPos -= 1;
							if (b->CPos < 0)
							{
								b->CPos = 0;
								if (b->BackBox != 0)
								{
									b->OwnPage->Deselect(true);
									b->BackBox->CPos = b->BackBox->Ptext.count();
									doc->ActPage = b->BackBox->OwnPage;
									b->BackBox->OwnPage->SelectItemNr(b->BackBox->ItemNr);
									b = b->BackBox;
								}
							}
						}
						if (b->CPos == static_cast<int>(b->Ptext.count()))
						{
							if (b->Ptext.at(b->CPos-1)->cstyle & 256)
							{
								b->CPos -= 1;
								while (b->Ptext.at(b->CPos)->cstyle & 256)
								{
									b->CPos--;
									if (b->CPos == 0)
										break;
								}
							}
						}
						else
						{
							while (b->Ptext.at(b->CPos)->cstyle & 256)
							{
								b->CPos--;
								if (b->CPos == 0)
									break;
							}
						}
						if ( b->HasSel )
							doc->ActPage->RefreshItem(b, true);
						setTBvals(b);
						break;
					case Key_Right:
						if ( buttonState & ControlButton )
						{
							doc->ActPage->setNewPos(b, oldPos, b->Ptext.count(),1);
							if ( buttonState & ShiftButton )
								doc->ActPage->ExpandSel(b, 1, oldPos);
						}
						else if ( buttonState & ShiftButton )
						{
							b->CPos++;
							if ( b->CPos > static_cast<int>(b->Ptext.count()) )
								b->CPos--;
							else
								doc->ActPage->ExpandSel(b, 1, oldPos);
						}
						else
						{
							b->CPos += 1; // new position within text ?
							if (b->CPos > static_cast<int>(b->Ptext.count()))
							{
								b->CPos -= 1;
								if (b->NextBox != 0)
								{
									if (b->NextBox->Ptext.count() != 0)
									{
										b->OwnPage->Deselect(true);
										b->NextBox->CPos = 0;
										doc->ActPage = b->NextBox->OwnPage;
										b->NextBox->OwnPage->SelectItemNr(b->NextBox->ItemNr);
										b = b->NextBox;
									}
								}
							}
						}
						if ( b->HasSel )
							doc->ActPage->RefreshItem(b, true);
						setTBvals(b);
						break;
					case Key_Delete:
						if (b->CPos == static_cast<int>(b->Ptext.count()))
						{
							if (b->HasSel)
							{
								DeleteSel(b);
								setTBvals(b);
								b->Dirty = true;
								doc->ActPage->RefreshItem(b, true);
							}
							keyrep = false;
							return;
						}
						if (b->Ptext.count() == 0)
						{
							keyrep = false;
							return;
						}
						cr = b->Ptext.at(b->CPos)->ch;
						if (b->HasSel)
							DeleteSel(b);
						else
							b->Ptext.remove(b->CPos);
						b->Tinput = false;
						if ((cr == QChar(13)) && (b->Ptext.count() != 0))
						{
							doc->ActPage->chAbStyle(b, b->Ptext.at(QMAX(b->CPos-1,0))->cab);
							b->Tinput = false;
						}
						setTBvals(b);
						b->Dirty = true;
						doc->ActPage->RefreshItem(b, true);
						break;
					case Key_Backspace:
						if (b->CPos == 0)
						{
							if (b->HasSel)
							{
								DeleteSel(b);
								setTBvals(b);
								b->Dirty = true;
								doc->ActPage->RefreshItem(b, true);
							}
							keyrep = false;
							return;
						}
						if (b->Ptext.count() == 0)
						{
							keyrep = false;
							return;
						}
						cr = b->Ptext.at(QMAX(b->CPos-1,0))->ch;
						if (b->HasSel)
							DeleteSel(b);
						else
							b->CPos -= 1;
						b->Ptext.remove(b->CPos);
						b->Tinput = false;
						if ((cr == QChar(13)) && (b->Ptext.count() != 0))
						{
							doc->ActPage->chAbStyle(b, b->Ptext.at(QMAX(b->CPos-1,0))->cab);
							b->Tinput = false;
						}
						setTBvals(b);
						b->Dirty = true;
						doc->ActPage->RefreshItem(b, true);
						break;
					default:
						if ((b->HasSel) && (kk < 0x1000))
						{
							DeleteSel(b);
							b->Dirty = true;
//							doc->ActPage->RefreshItem(b, true);
						}
						if ((kk == Key_Tab)
						        || ((kk == Key_Return) && (buttonState & ShiftButton))
						        || ((kk + KeyMod) == Prefs.KeyActions[60].KeyID)
						        || ((kk + KeyMod) == Prefs.KeyActions[67].KeyID))
						{
							hg = new Pti;
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
							hg->cab = doc->CurrentABStil;
							if (doc->Vorlagen[doc->CurrentABStil].Font != "")
							{
								hg->cfont = doc->Vorlagen[doc->CurrentABStil].Font;
								hg->csize = doc->Vorlagen[doc->CurrentABStil].FontSize;
							}
							hg->cextra = 0;
							hg->xp = 0;
							hg->yp = 0;
							hg->PRot = 0;
							hg->PtransX = 0;
							hg->PtransY = 0;
							b->Ptext.insert(b->CPos, hg);
							b->CPos += 1;
							b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b, true);
							break;
						}
						if ((kk + KeyMod) == Prefs.KeyActions[56].KeyID)
						{
							b->Ptext.at(QMAX(b->CPos-1,0))->cstyle ^= 128;
							b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b, true);
							break;
						}
						if ((kk + KeyMod) == Prefs.KeyActions[59].KeyID)
						{
							setNewAbStyle(1);
							b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b, true);
							break;
						}
						if ((kk + KeyMod) == Prefs.KeyActions[57].KeyID)
						{
							setNewAbStyle(0);
							b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b, true);
							break;
						}
						if ((kk + KeyMod) == Prefs.KeyActions[58].KeyID)
						{
							setNewAbStyle(2);
							b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b, true);
							break;
						}
						if (((uc[0] > QChar(31)) || (as == 13) || (as == 30)) && ((*doc->AllFonts)[doc->CurrFont]->CharWidth.contains(uc[0].unicode())))
						{
							hg = new Pti;
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
							hg->cab = doc->CurrentABStil;
							if (doc->Vorlagen[doc->CurrentABStil].Font != "")
							{
								hg->cfont = doc->Vorlagen[doc->CurrentABStil].Font;
								hg->csize = doc->Vorlagen[doc->CurrentABStil].FontSize;
							}
							hg->xp = 0;
							hg->yp = 0;
							hg->PRot = 0;
							hg->PtransX = 0;
							hg->PtransY = 0;
							b->Ptext.insert(b->CPos, hg);
							b->CPos += 1;
							if ((doc->Trenner->AutoCheck) && (b->CPos > 1))
							{
								Twort = "";
								Tcoun = 0;
								for (int hych = b->CPos-1; hych > -1; hych--)
								{
									Tcha = b->Ptext.at(hych)->ch;
									if (Tcha == " ")
									{
										Tcoun = hych+1;
										break;
									}
									Twort.prepend(Tcha);
								}
								if (Twort != "")
								{
									if (doc->Trenner->Language != b->Language)
										doc->Trenner->slotNewDict(b->Language);
									doc->Trenner->slotHyphenateWord(b, Twort, Tcoun);
									b->Dirty = true;
								}
							}
							if ((b->CPos < static_cast<int>(b->Ptext.count())) || (as == 30))
								b->Dirty = true;
							b->Tinput = true;
							doc->ActPage->RefreshItem(b, true);
						}
						break;
					}
					if (b->Ptext.count() != 0)
						if (b->Ptext.at(QMAX(b->CPos-1, 0))->yp != 0)
							doc->ActPage->slotDoCurs(true);
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
		if ((Prefs.SaveAtQ) && (ScBook->Changed == true))
		{
			if (ScBook->ScFilename.isEmpty())
				ScBook->ScFilename = PrefsPfad+"/scrap.scs";
			ScBook->Save();
		}
		if (ScBook->BibWin->Objekte.count() == 0)
			unlink(PrefsPfad+"/scrap.scs");
		Prefs.AvailFonts.~SCFonts();
		FinalizePlugs();
		exit(0);
	}
	else
	{
		SavePrefs();
		if ((Prefs.SaveAtQ) && (ScBook->Changed == true))
		{
			if (ScBook->ScFilename.isEmpty())
				ScBook->ScFilename = PrefsPfad+"/scrap.scs";
			ScBook->Save();
		}
		if (ScBook->BibWin->Objekte.count() == 0)
			unlink(PrefsPfad+"/scrap.scs");
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		Prefs.AvailFonts.~SCFonts();
		FinalizePlugs();
		exit(0);
	}
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////
double ScribusApp::mm2pts(int mm)
{
	return mm / 25.4 * 72;
}

double ScribusApp::pts2mm(double pts)
{
	return pts / 72 * 25.4;
}

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
	double b, h, tpr, lr, rr, br, sp, ab;
	bool fp, atf, ret;
	NewDoc* dia = new NewDoc(this, &Prefs);
	if (dia->exec())
	{
		tpr = dia->Top;
		lr = dia->Left;
		rr = dia->Right;
		br = dia->Bottom;
		ab = dia->Dist;
		b = dia->Pagebr;
		h = dia->Pageho;
		sp = dia->SpinBox10->value();
		atf = dia->AutoFrame->isChecked();
		fp = dia->Doppelseiten->isChecked();
		ret = doFileNew(b, h, tpr, lr, rr, br, ab, sp, atf, fp, dia->ComboBox3->currentItem(),
		                dia->ErsteSeite->isChecked(), dia->Orient, dia->PgNr->value());
		FMess->setText( tr("Ready"));
	}
	else
		ret = false;
	delete dia;
	return ret;
}

bool ScribusApp::doFileNew(double b, double h, double tpr, double lr, double rr, double br, double ab, double sp,
                           bool atf, bool fp, int einh, bool firstleft, int Ori, int SNr)
{
	QString cc;
	if (HaveDoc)
		doc->OpenNodes = Tpal->buildReopenVals();
	doc = new ScribusDoc();
	doc->Einheit = einh;
	if (fp)
		doc->FirstPageLeft = firstleft;
	doc->PageOri = Ori;
	doc->FirstPnum = SNr;
	doc->AllFonts = &Prefs.AvailFonts;
	doc->AddFont(Prefs.DefFont, Prefs.AvailFonts[Prefs.DefFont]->Font);
	doc->Dfont = Prefs.DefFont;
	doc->Dsize = Prefs.DefSize;
	doc->GrabRad = Prefs.GrabRad;
	doc->GuideRad = Prefs.GuideRad;
	doc->minorGrid = Prefs.DminGrid;
	doc->majorGrid = Prefs.DmajGrid;
	doc->minorColor = Prefs.DminColor;
	doc->majorColor = Prefs.DmajColor;
	doc->papColor = Prefs.DpapColor;
	doc->margColor = Prefs.DmargColor;
	doc->guideColor = Prefs.guideColor;
	doc->baseColor = Prefs.baseColor;
	doc->VHoch = Prefs.DVHoch;
	doc->VHochSc = Prefs.DVHochSc;
	doc->VTief = Prefs.DVTief;
	doc->VTiefSc = Prefs.DVTiefSc;
	doc->VKapit = Prefs.DVKapit;
	doc->Dpen = Prefs.Dpen;
	doc->DpenText = Prefs.DpenText;
	doc->Dbrush = Prefs.Dbrush;
	doc->Dshade = Prefs.Dshade;
	doc->Dshade2 = Prefs.Dshade2;
	doc->DCols = Prefs.DCols;
	doc->DGap = Prefs.DGap;
	doc->DLineArt = PenStyle(Prefs.DLineArt);
	doc->Dwidth = Prefs.Dwidth;
	doc->DpenLine = Prefs.DpenLine;
	doc->DshadeLine = Prefs.DshadeLine;
	doc->DLstyleLine = PenStyle(Prefs.DLstyleLine);
	doc->DwidthLine = Prefs.DwidthLine;
	doc->MagMin = Prefs.MagMin;
	doc->MagMax = Prefs.MagMax;
	doc->MagStep = Prefs.MagStep;
	doc->DbrushPict = Prefs.DbrushPict;
	doc->ShadePict = Prefs.ShadePict;
	doc->ScaleX = Prefs.ScaleX;
	doc->ScaleY = Prefs.ScaleY;
	doc->ScaleType = Prefs.ScaleType;
	doc->AspectRatio = Prefs.AspectRatio;
	doc->Before = Prefs.Before;
	doc->PagesSbS = Prefs.PagesSbS;
	doc->RandFarbig = Prefs.RandFarbig;
	doc->AutoLine = Prefs.AutoLine;
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
	struct LPIset lpo;
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
	doc->Scale = 1.0*Prefs.DisScale;
	doc->AppMode = 1;
	doc->Language = Prefs.Language;
	doc->MinWordLen = Prefs.MinWordLen;
	doc->HyCount = Prefs.HyCount;
	doc->Automatic = Prefs.Automatic;
	doc->AutoCheck = Prefs.AutoCheck;
	doc->PageColors = Prefs.DColors;
	doc->BaseGrid = Prefs.BaseGrid;
	doc->BaseOffs = Prefs.BaseOffs;
	doc->loading = true;
	ScribusWin* w = new ScribusWin(wsp, doc);
	view = new ScribusView(w, doc, &Prefs);
	w->setView(view);
	ActWin = w;
	doc->WinHan = w;
	w->setCentralWidget(view);
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
	view->Pages.at(0)->parentWidget()->hide();
	view->DocPages = view->Pages;
	view->Pages = view->MasterPages;
	doc->PageC = view->MasterPages.count();
	bool atfb = doc->PageAT;
	doc->PageAT = false;
	slotNewPage(0);
	doc->PageAT = atfb;
	view->MasterNames["Normal"] = 0;
	view->Pages.at(0)->PageNam = "Normal";
	view->Pages.at(0)->parentWidget()->hide();
	view->MasterPages = view->Pages;
	doc->PageC = view->DocPages.count();
	view->Pages = view->DocPages;
	doc->MasterP = false;
	view->Pages.at(0)->MPageNam = "Normal";
	view->Pages.at(0)->parentWidget()->show();
	doc->setUnModified();
	doc->loading = false;
	doc->ActPage = view->Pages.at(0);
	doc->OpenNodes.clear();
	Tpal->BuildTree(view);
	Sepal->Rebuild();
	BookPal->BView->clear();
	if ( wsp->windowList().isEmpty() )
		w->showMaximized();
	else
		w->show();
	view->show();
	connect(doc->ASaveTimer, SIGNAL(timeout()), w, SLOT(slotAutoSave()));
	connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	doc->AutoSave = Prefs.AutoSave;
	if (doc->AutoSave)
		doc->ASaveTimer->start(Prefs.AutoSaveTime);
	DatSav->setEnabled(false);
	fileMenu->setItemEnabled(fid4, 0);
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
	ScribusWin* swin;
	if (w == NULL)
	{
		ActWin = NULL;
		return;
	}
	ActWin = (ScribusWin*)w;
	if (doc != NULL)
	{
		if ((HaveDoc) && (doc != ActWin->doc))
			doc->OpenNodes = Tpal->buildReopenVals();
	}
	doc = ActWin->doc;
	view = ActWin->view;
	Sepal->SetView(view);
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
		setAppMode(doc->AppMode);
	}
	w->setFocus();
	if (w->isMaximized())
		wsp->setScrollBarsEnabled(false);
	else
		wsp->setScrollBarsEnabled(true);
	if (!doc->TemplateMode)
		Sepal->Rebuild();
	Tpal->BuildTree(view);
	Tpal->reopenTree(doc->OpenNodes);
	BookPal->BView->NrItems = ActWin->NrItems;
	BookPal->BView->First = ActWin->First;
	BookPal->BView->Last = ActWin->Last;
	RestoreBookMarks();
	if (!doc->loading)
	{
		if (doc->ActPage->SelItem.count() != 0)
		{
			HaveNewSel(doc->ActPage->SelItem.at(0)->PType);
			doc->ActPage->EmitValues(doc->ActPage->SelItem.at(0));
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
	double tpr = doc->PageM.Top;
	double lr = doc->PageM.Left;
	double rr = doc->PageM.Right;
	double br = doc->PageM.Bottom;
	bool fp = doc->PageFP;
	bool fpe = doc->FirstPageLeft;
	double tpr2, lr2, rr2, br2;
	bool ret = false;
	ReformDoc* dia = new ReformDoc(this, tpr, lr, rr, br, doc->PageB, doc->PageH, fp, fpe, doc->Einheit);
	if (dia->exec())
	{
		tpr2 = dia->TopR->value() / UmReFaktor;
		lr2 = dia->LeftR->value() / UmReFaktor;
		rr2 = dia->RightR->value() / UmReFaktor;
		br2 = dia->BottomR->value() / UmReFaktor;
		fp = dia->Doppelseiten->isChecked();
		if (fp)
			doc->FirstPageLeft = dia->ErsteSeite->isChecked();
		doc->resetPage(tpr2, lr2, rr2, br2, fp);
		view->reformPages();
		view->GotoPage(doc->ActPage->PageNr);
		view->DrawNew();
		Sepal->RebuildPage();
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
	CListe::Iterator it;
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
	BuildFontMenu();
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
	Mpal->Spal->SetFormats(doc);
	Mpal->SetLineFormats(doc);
	Lpal->setLayers(&doc->Layers, &doc->ActiveLayer);
	view->LaMenu();
	view->setLayMenTxt(doc->ActiveLayer);
	doc->CurrentABStil = 0;
	slotChangeUnit(doc->Einheit, false);
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
		fileMenu->setItemEnabled(fid12, 0);
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
		fileMenu->setItemEnabled(fid5, ActWin->MenuStat[3]);
		fileMenu->setItemEnabled(fid51, ActWin->MenuStat[3]);
		fileMenu->setItemEnabled(fid52, 0);
		fileMenu->setItemEnabled(fid12, 1);
		fileMenu->setItemEnabled(fid13, 1);
		fileMenu->setItemEnabled(fid14, 1);
		if (view->Pages.count() > 1)
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
		fileMenu->setItemEnabled(fid5, 1);
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
	fileMenu->setItemEnabled(fid5, 1);
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
	if (view->Pages.count() > 1)
		setter = 1;
	pageMenu->setItemEnabled(pgmd, setter);
	pageMenu->setItemEnabled(pgmv, setter);
	editMenu->setItemEnabled(tman, 1);
	editMenu->setItemEnabled(jman, 1);
	CListe::Iterator it;
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
	Mpal->Spal->SetFormats(doc);
	Mpal->SetLineFormats(doc);
	Lpal->setLayers(&doc->Layers, &doc->ActiveLayer);
	view->LaMenu();
	view->setLayMenTxt(doc->ActiveLayer);
	doc->CurrentABStil = 0;
	slotChangeUnit(doc->Einheit);
	doc->Trenner = new Hyphenator(this, doc, this);
	BuildFontMenu();
	connect(view, SIGNAL(changeUN(int)), this, SLOT(slotChangeUnit(int)));
	connect(view, SIGNAL(changeLA(int)), Lpal, SLOT(MarkActiveLayer(int)));
	connect(view->HR, SIGNAL(MarkerMoved(double, double)), this, SLOT(ReportMP(double, double)));
	connect(view->HR, SIGNAL(DocChanged(bool)), this, SLOT(slotDocCh(bool)));
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
		b = doc->ActPage->SelItem.at(0);
	ObjMenu->setItemEnabled(PfadDT, 0);
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
		Mpal->Cpal->GradCombo->setCurrentItem(0);
		Tpal->slotShowSelect(doc->ActPage->PageNr, -1);
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
		if (b->Ptext.count() == 0)
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
			if ((b->BackBox != 0) && (b->Ptext.count() == 0))
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
		if (doc->AppMode == 7)
		{
			setTBvals(b);
			editMenu->setItemEnabled(edid5, 1);
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
			view->HR->First = doc->Vorlagen[doc->CurrentABStil].First;
			view->HR->Indent = doc->Vorlagen[doc->CurrentABStil].Indent;
			if ((b->flippedH % 2 != 0) || (b->Reverse))
				view->HR->Revers = true;
			else
				view->HR->Revers = false;
			view->HR->ItemPosValid = true;
			view->HR->repX = false;
			if (doc->CurrentABStil < 5)
				view->HR->TabValues = b->TabValues;
			else
				view->HR->TabValues = doc->Vorlagen[doc->CurrentABStil].TabValues;
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
		doc->Vorlagen[0].LineSpa = b->LineSp;
		doc->Vorlagen[0].Ausri = b->Ausrich;
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
		if (doc->AppMode == 7)
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
	if (doc->ActPage->SelItem.count() > 1)
	{
		ObjMenu->setItemEnabled(DistM, 1);
		ObjMenu->setItemEnabled(PfadTP, 0);
		editMenu->setItemEnabled(Sear, 0);
		bool hPoly = true;
		bool isGroup = true;
		int firstElem = -1;
		if (b->Groups.count() != 0)
			firstElem = b->Groups.top();
		for (uint bx=0; bx<doc->ActPage->SelItem.count(); ++bx)
		{
			if (doc->ActPage->SelItem.at(bx)->PType != 6)
				hPoly = false;
			if (doc->ActPage->SelItem.at(bx)->Groups.count() != 0)
			{
				if (doc->ActPage->SelItem.at(bx)->Groups.top() != firstElem)
					isGroup = false;
			}
			else
				isGroup = false;
		}
		ObjMenu->setItemEnabled(Gr, !isGroup);
		ObjMenu->setItemEnabled(PfadV, hPoly);
		if (doc->ActPage->SelItem.count() == 2)
		{
			if (((b->PType == 4) || (doc->ActPage->SelItem.at(1)->PType == 4)) && ((b->PType == 7) || (doc->ActPage->SelItem.at(1)->PType == 7)))
			{
				PageItem* bx = doc->ActPage->SelItem.at(1);
				if ((b->NextBox == 0) && (b->BackBox == 0) && (bx->NextBox == 0) && (bx->BackBox == 0))
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
	if (doc->ActPage->SelItem.count() != 0)
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
		Tpal->slotShowSelect(b->OwnPage->PageNr, b->ItemNr);
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
	if ((reb) && (!doc->TemplateMode) && (doc->ActPage->SelItem.count() != 0))
	{
		for (uint upd = 0; upd < doc->ActPage->SelItem.count(); ++upd)
			Tpal->slotUpdateElement(doc->ActPage->PageNr, doc->ActPage->SelItem.at(upd)->ItemNr);
	}
	if (!doc->isModified())
		doc->setModified();
	ActWin->setCaption( doc->DocName + "*");
	fileMenu->setItemEnabled(fid4, 1);
	DatSav->setEnabled(true);
	fileMenu->setItemEnabled(fid5, 1);
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
	ActWin->MenuStat[3] = fileMenu->isItemEnabled(fid5);
	if (doc->ActPage->SelItem.count() != 0)
	{
		PageItem* b = doc->ActPage->SelItem.at(0);
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
		RecentDocs.prepend(fn);
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
		RecentDocs.remove(fn);
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
#ifdef HAVE_LIBZ
	QString fileName = CFileDialog( tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	QString fileName = CFileDialog( tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	ret = LadeDoc(fileName);
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	return ret;
}

bool ScribusApp::slotDocMerge()
{
	bool ret = false;
	MergeDoc *dia = new MergeDoc(this, false, doc->PageC, doc->ActPage->PageNr + 1);
	if (dia->exec())
	{
		FMess->setText(tr("Importing Pages..."));
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		std::vector<int> pageNs;
		parsePagesString(dia->PageNr->text(), &pageNs, dia->Count);
		int startPage, nrToImport;
		bool doIt = true;
		if (doc->TemplateMode)
		{
			if (pageNs.size() > 1)
			{
				LadeSeite(dia->Filename->text(), pageNs[0] - 1, false);
			}
			doIt = false;
		}
		else if (dia->Create->isChecked())
		{
			if (dia->Where->currentItem() == 0)
				startPage = dia->ActualPage->value();
			else if (dia->Where->currentItem() == 1)
				startPage = dia->ActualPage->value() + 1;
			else
				startPage = doc->PageC + 1;
			addNewPages(dia->ActualPage->value(), dia->Where->currentItem(), pageNs.size());
			nrToImport = pageNs.size();
		}
		else
		{
			startPage = doc->ActPage->PageNr + 1;
			nrToImport = pageNs.size();
			if (pageNs.size() > (doc->PageC - doc->ActPage->PageNr))
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
						addNewPages(doc->PageC, 2, pageNs.size() - (doc->PageC - doc->ActPage->PageNr));
					break;
					case QMessageBox::No:
						nrToImport = doc->PageC - doc->ActPage->PageNr;
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
					LadeSeite(dia->Filename->text(), pageNs[i] - 1, false);
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
		for (uint azz = 0; azz < doc->ActPage->Items.count(); ++azz)
		{
			PageItem *ite = doc->ActPage->Items.at(azz);
			if ((ite->PType == 4) && (ite->isBookmark))
				BookPal->BView->AddPageItem(ite);
		}
		Mpal->Cpal->SetColors(doc->PageColors);
		Mpal->updateCList();
		Mpal->Spal->SetFormats(doc);
		Mpal->SetLineFormats(doc);
		if (!Mpa)
		{
			Tpal->BuildTree(view);
			Tpal->reopenTree(doc->OpenNodes);
		}
		slotDocCh();
		doc->loading = false;
		ret = true;
	}
	if (!Mpa)
		Sepal->Rebuild();
	doc->ActPage->update();
	return ret;
}

bool ScribusApp::LadeDoc(QString fileName)
{
	QFileInfo fi(fileName);
	if (!fi.exists())
		return false;
	if (HaveDoc)
		doc->OpenNodes = Tpal->buildReopenVals();
	bool ret = false;
	QWidgetList windows = wsp->windowList();
	ScribusWin* ActWinOld;
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
		doc=new ScribusDoc();
		doc->AllFonts = &Prefs.AvailFonts;
		doc->AddFont(Prefs.DefFont, Prefs.AvailFonts[Prefs.DefFont]->Font);
		doc->Dfont = Prefs.DefFont;
		doc->Dsize = Prefs.DefSize;
		doc->GrabRad = Prefs.GrabRad;
		doc->GuideRad = Prefs.GuideRad;
		doc->minorGrid = Prefs.DminGrid;
		doc->majorGrid = Prefs.DmajGrid;
		doc->minorColor = Prefs.DminColor;
		doc->majorColor = Prefs.DmajColor;
		doc->papColor = Prefs.DpapColor;
		doc->margColor = Prefs.DmargColor;
		doc->guideColor = Prefs.guideColor;
		doc->baseColor = Prefs.baseColor;
		doc->Dpen = Prefs.Dpen;
		doc->DpenText = Prefs.DpenText;
		doc->Dbrush = Prefs.Dbrush;
		doc->Dshade = Prefs.Dshade;
		doc->Dshade2 = Prefs.Dshade2;
		doc->DCols = Prefs.DCols;
		doc->DGap = Prefs.DGap;
		doc->DLineArt = PenStyle(Prefs.DLineArt);
		doc->Dwidth = Prefs.Dwidth;
		doc->DpenLine = Prefs.DpenLine;
		doc->DshadeLine = Prefs.DshadeLine;
		doc->DLstyleLine = PenStyle(Prefs.DLstyleLine);
		doc->DwidthLine = Prefs.DwidthLine;
		doc->MagMin = Prefs.MagMin;
		doc->MagMax = Prefs.MagMax;
		doc->MagStep = Prefs.MagStep;
		doc->DbrushPict = Prefs.DbrushPict;
		doc->ShadePict = Prefs.ShadePict;
		doc->ScaleX = Prefs.ScaleX;
		doc->ScaleY = Prefs.ScaleY;
		doc->ScaleType = Prefs.ScaleType;
		doc->AspectRatio = Prefs.AspectRatio;
		doc->Before = Prefs.Before;
		doc->Einheit = Prefs.Einheit;
		doc->PagesSbS = Prefs.PagesSbS;
		doc->RandFarbig = Prefs.RandFarbig;
		doc->AutoLine = Prefs.AutoLine;
		doc->Scale = 1.0*Prefs.DisScale;
		doc->AppMode = 1;
		doc->HasCMS = false;
		doc->ActiveLayer = 0;
		doc->Language = Prefs.Language;
		doc->MinWordLen = Prefs.MinWordLen;
		doc->HyCount = Prefs.HyCount;
		doc->Automatic = Prefs.Automatic;
		doc->AutoCheck = Prefs.AutoCheck;
		doc->BaseGrid = Prefs.BaseGrid;
		doc->BaseOffs = Prefs.BaseOffs;
		doc->OpenNodes.clear();
		doc->loading = true;
		FMess->setText( tr("Loading..."));
		FProg->reset();
		ScribusWin* w = new ScribusWin(wsp, doc);
		view = new ScribusView(w, doc, &Prefs);
		w->setView(view);
		ActWin = w;
		doc->WinHan = w;
		w->setCentralWidget(view);
		ScriXmlDoc *ss = new ScriXmlDoc();
		connect(ss, SIGNAL(NewPage(int)), this, SLOT(slotNewPage(int)));
#ifdef HAVE_CMS
		w->SoftProofing = false;
		w->Gamut = false;
		bool cmsu = CMSuse;
		CMSuse = false;
#endif
		ScApp->ScriptRunning = true;
		if(!ss->ReadDoc(fi.fileName(), Prefs.AvailFonts, doc, view, FProg))
		{
			view->close();
			disconnect(ss, SIGNAL(NewPage(int)), this, SLOT(slotNewPage(int)));
			delete ss;
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
		delete ss;
		ScApp->ScriptRunning = false;
		FMess->setText("");
		FProg->reset();
#ifdef HAVE_CMS
		CMSuse = cmsu;
#endif
		HaveDoc++;
		if (doc->PDF_Optionen.LPISettings.count() == 0)
		{
			struct LPIset lpo;
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
			if (!InputProfiles.contains(doc->CMSSettings.DefaultInputProfile))
				doc->CMSSettings.DefaultInputProfile = Prefs.DCMSset.DefaultInputProfile;
			if (!InputProfiles.contains(doc->CMSSettings.DefaultInputProfile2))
				doc->CMSSettings.DefaultInputProfile2 = Prefs.DCMSset.DefaultInputProfile2;
			if (!MonitorProfiles.contains(doc->CMSSettings.DefaultMonitorProfile))
				doc->CMSSettings.DefaultMonitorProfile = Prefs.DCMSset.DefaultMonitorProfile;
			if (!PrinterProfiles.contains(doc->CMSSettings.DefaultPrinterProfile))
				doc->CMSSettings.DefaultPrinterProfile = Prefs.DCMSset.DefaultPrinterProfile;
			if (!PrinterProfiles.contains(doc->PDF_Optionen.PrintProf))
				doc->PDF_Optionen.PrintProf = doc->CMSSettings.DefaultPrinterProfile;
			if (!InputProfiles.contains(doc->PDF_Optionen.ImageProf))
				doc->PDF_Optionen.ImageProf = doc->CMSSettings.DefaultInputProfile;
			if (!InputProfiles.contains(doc->PDF_Optionen.SolidProf))
				doc->PDF_Optionen.SolidProf = doc->CMSSettings.DefaultInputProfile2;
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
		doc->DocName = FName;
		doc->MasterP = false;
		doc->Language = GetLang(doc->Language);
		HaveNewDoc();
		Mpal->updateCList();
		doc->hasName = true;
		if (view->MasterPages.count() == 0)
		{
			for (uint ax=0; ax<view->Pages.count(); ++ax)
			{
				view->Pages.at(ax)->parentWidget()->hide();
			}
			view->DocPages = view->Pages;
			view->Pages = view->MasterPages;
			doc->PageC = view->MasterPages.count();
			bool atf = doc->PageAT;
			doc->PageAT = false;
			slotNewPage(0);
			doc->PageAT = atf;
			view->MasterNames["Normal"] = 0;
			view->Pages.at(0)->PageNam = "Normal";
			view->Pages.at(0)->parentWidget()->hide();
			view->MasterPages = view->Pages;
			doc->PageC = view->DocPages.count();
			view->Pages = view->DocPages;
			doc->MasterP = false;
			for (uint ay=0; ay<view->Pages.count(); ++ay)
			{
				view->Pages.at(ay)->parentWidget()->show();
			}
		}
		doc->loading = false;
		view->GotoPage(0);
		doc->RePos = true;
		QPixmap pgPix(10, 10);
		QRect rd = QRect(0,0,9,9);
		ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
		for (uint az=0; az<view->MasterPages.count(); az++)
		{
			for (uint azz=0; azz<view->MasterPages.at(az)->Items.count(); ++azz)
			{
				PageItem *ite = view->MasterPages.at(az)->Items.at(azz);
				if ((ite->PType == 4) || (ite->PType == 8))
					ite->DrawObj(painter, rd);
			}
		}
		RestoreBookMarks();
		for (uint az=0; az<view->Pages.count(); az++)
		{
			for (uint azz=0; azz<view->Pages.at(az)->Items.count(); ++azz)
			{
				PageItem *ite = view->Pages.at(az)->Items.at(azz);
				if ((ite->PType == 4) || (ite->PType == 8))
					ite->DrawObj(painter, rd);
				if (doc->OldBM)
				{
					if ((ite->PType == 4) && (ite->isBookmark))
						BookPal->BView->AddPageItem(ite);
				}
				else
				{
					if ((ite->PType == 4) && (ite->isBookmark))
						BookPal->BView->ChangeItem(ite->BMnr, ite->ItemNr);
				}
			}
		}
		delete painter;
		if (doc->OldBM)
			StoreBookmarks();
		ActWin->NrItems = BookPal->BView->NrItems;
		ActWin->First = BookPal->BView->First;
		ActWin->Last = BookPal->BView->Last;
		doc->RePos = false;
		doc->setUnModified();
		UpdateRecent(FName);
		FMess->setText( tr("Ready"));
		ret = true;
		if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
			w->showMaximized();
		else
			w->show();
		view->show();
		newActWin(w);
		connect(doc->ASaveTimer, SIGNAL(timeout()), w, SLOT(slotAutoSave()));
		connect(w, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
		doc->AutoSave = Prefs.AutoSave;
		if (doc->AutoSave)
			doc->ASaveTimer->start(Prefs.AutoSaveTime);
		DatSav->setEnabled(false);
		fileMenu->setItemEnabled(fid4, 0);
	}
	else
	{
		Sepal->Vie = 0;
	}
	Sepal->Rebuild();
	return ret;
}

void ScribusApp::slotFileOpen()
{
	if (doc->ActPage->SelItem.count() != 0)
	{
		PageItem *b = doc->ActPage->SelItem.at(0);
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
			QString fileName = CFileDialog( tr("Open"), formatD, "", true);
			if (!fileName.isEmpty())
			{
				b->EmProfile = "";
				b->UseEmbedded = true;
				b->IProfile = doc->CMSSettings.DefaultInputProfile;
				b->IRender = doc->CMSSettings.DefaultIntentMonitor2;
				qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				doc->ActPage->LoadPict(fileName, b->ItemNr);
				doc->ActPage->AdjustPictScale(b);
				doc->ActPage->AdjustPreview(b);
				qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput);
				qApp->restoreOverrideCursor();
				doc->ActPage->update();
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
			if (doc->Trenner->AutoCheck)
				doc->Trenner->slotHyphenate(b);
			doc->ActPage->update();
			slotDocCh();
/* 			LoadEnc = "";
 			QString fileName = CFileDialog( tr("Open"), tr("Text Files (*.txt);;All Files(*)"), "", false, true, false, true);
 			if (!fileName.isEmpty())
 			{
 				Serializer *ss = new Serializer(fileName);
 				if (ss->Read(LoadEnc))
 				{
 					int st = doc->CurrentABStil;
 					ss->GetText(b, st, doc->Vorlagen[st].Font, doc->Vorlagen[st].FontSize);
 				}
 				delete ss;
 				if (doc->Trenner->AutoCheck)
 					doc->Trenner->slotHyphenate(b);
 				doc->ActPage->update();
 				slotDocCh();
 			} */
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
	if (doc->ActPage->SelItem.count() != 0)
	{
		gtGetText* gt = new gtGetText();
		gt->run(true);
		delete gt;
		if (doc->Trenner->AutoCheck)
			doc->Trenner->slotHyphenate(doc->ActPage->SelItem.at(0));
		doc->ActPage->update();
		slotDocCh();
		/*
		PageItem *b = doc->ActPage->SelItem.at(0);
		LoadEnc = "";
		QString fileName = CFileDialog( tr("Open"), tr("Text Files (*.txt);;All Files(*)"), "", false, true, false, true);
		if (!fileName.isEmpty())
		{
			Serializer *ss = new Serializer(fileName);
			if (ss->Read(LoadEnc))
			{
				int st = doc->CurrentABStil;
				ss->GetText(b, st, doc->Vorlagen[st].Font, doc->Vorlagen[st].FontSize, true);
			}
			delete ss;
			if (doc->Trenner->AutoCheck)
				doc->Trenner->slotHyphenate(b);
			doc->ActPage->update();
			slotDocCh();
		} */
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
	if (doc->hasName)
	{
		QFileInfo fi(doc->DocName);
		fna = fi.dirPath()+"/"+fi.baseName()+".sla";
	}
	else
	{
		QDir di = QDir();
		fna = di.currentDirPath()+"/"+doc->DocName+".sla";
	}
#ifdef HAVE_LIBZ
	QString fn = CFileDialog( tr("Save as"), tr("Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)"), fna, false, false, true);
#else
	QString fn = CFileDialog( tr("Save as"), tr("Documents (*.sla *.scd);;All Files (*)"), fna, false, false, false);
#endif
	if (!fn.isEmpty())
	{
		if ((fn.endsWith(".sla")) || (fn.endsWith(".sla.gz")))
			fna = fn;
		else
			fna = fn+".sla";
		if (overwrite(this, fna))
		{
			ret = DoFileSave(fna);
			if (!ret)
				QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
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
	ret = ss->WriteDoc(fn, doc, view, FProg);
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
	if(doc->TemplateMode)
	{
		ActWin->muster->close();
		qApp->processEvents();
	}
	setAppMode(1);
	doc->ASaveTimer->stop();
	disconnect(doc->ASaveTimer, SIGNAL(timeout()), doc->WinHan, SLOT(slotAutoSave()));
	disconnect(doc->WinHan, SIGNAL(AutoSaved()), this, SLOT(slotAutoSaved()));
	if ((doc->UnData.UnCode == 0) && (doc->UnDoValid))
		delete doc->UnData.Item;
	if (CMSavail)
		ActWin->CloseCMSProfiles();
	Mpal->NewSel(-1);
	Mpal->UnsetDoc();
	Sepal->Vie = 0;
	Sepal->Rebuild();
	Mpal->Spal->SetFormats(0);
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
		fileMenu->setItemEnabled(fid5, 0);
		fileMenu->setItemEnabled(fid52, 0);
		fileMenu->setItemEnabled(fid11, 0);
		exportMenu->setItemEnabled(fid8, 0);
		importMenu->setItemEnabled(fid2a, 0);
		exportMenu->setItemEnabled(fid10, 0);
		editMenu->setItemEnabled(edUndo, 0);
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
	QString fna, prn, cmd, scmd, cc, data, SepNam;
	int Nr;
	bool fil, sep, farbe, PSfile, mirrorH, mirrorV, useICC, DoGCR;
	PSfile = false;
	FMess->setText( tr("Printing..."));
	if (PrinterUsed)
	{
		prn = PDef.Pname;
		fna = PDef.Dname;
	}
	else
	{
		prn = "";
		if (!doc->DocName.startsWith( tr("Document")))
		{
			QFileInfo fi(doc->DocName);
			fna = fi.dirPath()+"/"+fi.baseName()+".ps";
		}
		else
		{
			QDir di = QDir();
			fna = di.currentDirPath()+"/"+doc->DocName+".ps";
		}
	}
	scmd = PDef.Command;
	Druck *printer = new Druck(this, fna, prn, scmd, Prefs.GCRMode);
	printer->setMinMax(1, view->Pages.count(), doc->ActPage->PageNr+1);
	if (printer->exec())
	{
		std::vector<int> pageNs;
		ReOrderText(doc, view);
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		prn = printer->printerName();
		fna = printer->outputFileName();
		fil = printer->outputToFile();
		if (printer->CurrentPage->isChecked())
			pageNs.push_back(doc->ActPage->PageNr+1);
		else
		{
			if (printer->RadioButton1->isChecked())
				parsePagesString("*", &pageNs, doc->PageC);
			else
				parsePagesString(printer->PageNr->text(), &pageNs, doc->PageC);
		}
		Nr = printer->numCopies();
		sep = printer->outputSeparations();
		SepNam = printer->separationName();
		farbe = printer->color();
		mirrorH = printer->MirrorH;
		mirrorV = printer->MirrorV;
		useICC = printer->ICCinUse;
		DoGCR = printer->DoGCR;
		PDef.Pname = prn;
		PDef.Dname = fna;
		if (printer->OtherCom->isChecked())
			PDef.Command = printer->Command->text();
		PrinterUsed = true;
		QMap<QString,QFont> ReallyUsed;
		ReallyUsed.clear();
		GetUsedFonts(&ReallyUsed);
		PSLib *dd = getPSDriver(true, Prefs.AvailFonts, ReallyUsed, doc->PageColors, false);
		if (dd != NULL)
		{
			if (fil)
				PSfile = dd->PS_set_file(fna);
			else
			{
				PSfile = dd->PS_set_file(PrefsPfad+"/tmp.ps");
				fna = PrefsPfad+"/tmp.ps";
			}
			if (PSfile)
			{
				view->CreatePS(dd, pageNs, sep, SepNam, farbe, mirrorH, mirrorV, useICC, DoGCR);
				if (printer->PSLevel != 3)
				{
					QString tmp;
					QString opts = "-dDEVICEWIDTHPOINTS=";
					opts += tmp.setNum(doc->PageB);
					opts += " -dDEVICEHEIGHTPOINTS=";
					opts += tmp.setNum(doc->PageH);
					if (printer->PSLevel == 1)
						system("ps2ps -dLanguageLevel=1 "+opts+" \""+fna+"\" \""+fna+".tmp\"");
					else
						system("ps2ps "+opts+" \""+fna+"\" \""+fna+".tmp\"");
					system("mv \""+fna+".tmp\" \""+fna+"\"");
				}
				if (!fil)
				{
					if (printer->OtherCom->isChecked())
					{
						cmd = printer->Command->text()+ " "+fna;
						system(cmd);
					}
					else
					{
						cmd = "lpr -P" + prn;
						if (Nr > 1)
							cmd += " -#" + cc.setNum(Nr);
#ifdef HAVE_CUPS
						cmd += printer->PrinterOpts;
#endif
						cmd += " "+fna;
						system(cmd);
					}
					unlink(fna);
				}
			}
			else
				QMessageBox::warning(this, tr("Warning"), tr("Printing failed!"), tr("OK"));
			delete dd;
			closePSDriver();
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
		}
	}
	delete printer;
	FMess->setText( tr("Ready"));
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
	if ((HaveDoc) && (doc->ActPage->SelItem.count() != 0))
	{
		Buffer2 = "<SCRIBUSTEXT>";
		PageItem *b = doc->ActPage->SelItem.at(0);
		if (doc->AppMode == 7)
		{
			if ((b->Ptext.count() == 0) || (!b->HasSel))
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
				for (a = 0; a < nb->Ptext.count(); ++a)
				{
					if (nb->Ptext.at(a)->cselect)
					{
						if (nb->Ptext.at(a)->ch == QChar(13))
						{
							Buffer2 += QChar(5);
							BufferI += QChar(10);
						}
						else if (nb->Ptext.at(a)->ch == QChar(9))
						{
							Buffer2 += QChar(4);
							BufferI += QChar(9);
						}
						else
						{
							Buffer2 += nb->Ptext.at(a)->ch;
							BufferI += nb->Ptext.at(a)->ch;
						}
						Buffer2 += "\t";
						Buffer2 += nb->Ptext.at(a)->cfont+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->csize)+"\t";
						Buffer2 += nb->Ptext.at(a)->ccolor+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cextra)+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cshade)+'\t';
						Buffer2 += QString::number(nb->Ptext.at(a)->cstyle)+'\t';
						Buffer2 += QString::number(nb->Ptext.at(a)->cab)+'\t';
						Buffer2 += nb->Ptext.at(a)->cstroke+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cshade2)+'\t';
						Buffer2 += QString::number(nb->Ptext.at(a)->cscale)+'\n';
					}
				}
				DeleteSel(nb);
				nb->Dirty = true;
				nb = nb->NextBox;
			}
			doc->ActPage->RefreshItem(b);
		}
		else
		{
			if ((b->isTableItem) && (b->isSingleSel))
				return;
			ScriXmlDoc *ss = new ScriXmlDoc();
			BufferI = ss->WriteElem(&doc->ActPage->SelItem, doc);
			Buffer2 = BufferI;
			doc->ActPage->DeleteItem();
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
	if ((HaveDoc) && (doc->ActPage->SelItem.count() != 0))
	{
		Buffer2 = "<SCRIBUSTEXT>";
		PageItem *b = doc->ActPage->SelItem.at(0);
		if ((doc->AppMode == 7) && (b->HasSel))
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
				for (a = 0; a < nb->Ptext.count(); ++a)
				{
					if (nb->Ptext.at(a)->cselect)
					{
						if (nb->Ptext.at(a)->ch == QChar(13))
						{
							Buffer2 += QChar(5);
							BufferI += QChar(10);
						}
						else if (nb->Ptext.at(a)->ch == QChar(9))
						{
							Buffer2 += QChar(4);
							BufferI += QChar(9);
						}
						else
						{
							Buffer2 += nb->Ptext.at(a)->ch;
							BufferI += nb->Ptext.at(a)->ch;
						}
						Buffer2 += "\t";
						Buffer2 += nb->Ptext.at(a)->cfont+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->csize)+"\t";
						Buffer2 += nb->Ptext.at(a)->ccolor+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cextra)+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cshade)+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cstyle)+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cab)+"\t";
						Buffer2 += nb->Ptext.at(a)->cstroke+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cshade2)+"\t";
						Buffer2 += QString::number(nb->Ptext.at(a)->cscale)+"\n";
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
			BufferI = ss->WriteElem(&doc->ActPage->SelItem, doc);
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
	struct Pti *hg;
	NoFrameEdit();
	if (HaveDoc)
	{
		if (Buffer2.isNull())
			return;
		if (doc->AppMode == 7)
		{
			PageItem *b = doc->ActPage->SelItem.at(0);
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
					hg = new Pti;
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
					b->Ptext.insert(b->CPos, hg);
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
				int st = doc->CurrentABStil;
				ss->GetText(b, st, doc->Vorlagen[st].Font, doc->Vorlagen[st].FontSize, true);
				delete ss;
				if (doc->Trenner->AutoCheck)
					doc->Trenner->slotHyphenate(b);
			}
		if (b->CPos < static_cast<int>(b->Ptext.count())) { b->Dirty = true; }
			doc->ActPage->RefreshItem(b);
		}
		else
		{
			if (Buffer2.startsWith("<SCRIBUSELEM"))
			{
				doc->ActPage->Deselect(true);
				uint ac = doc->ActPage->Items.count();
				slotElemRead(Buffer2, 0, 0, false, true, doc);
				for (uint as = ac; as < doc->ActPage->Items.count(); ++as)
				{
					doc->ActPage->SelectItemNr(as);
				}
			}
		}
		slotDocCh(false);
	}
}

void ScribusApp::SelectAll()
{
	if (doc->AppMode == 7)
	{
		PageItem *b = doc->ActPage->SelItem.at(0);
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
			for (uint a = 0; a < nb->Ptext.count(); ++a)
			{
				nb->Ptext.at(a)->cselect = true;
				nb->HasSel = true;
				nb->Dirty = true;
			}
			nb = nb->NextBox;
		}
		view->DrawNew();
		EnableTxEdit();
	}
	else
	{
		PageItem *b;
		doc->ActPage->Deselect();
		for (uint a = 0; a < doc->ActPage->Items.count(); ++a)
		{
			b = doc->ActPage->Items.at(a);
			if (b->LayerNr == doc->ActiveLayer)
			{
				if (!b->Select)
				{
					doc->ActPage->SelItem.append(b);
					b->Select = true;
					b->FrameOnly = true;
					b->paintObj();
				}
			}
		}
		if (doc->ActPage->SelItem.count() > 1)
		{
			doc->ActPage->setGroupRect();
			doc->ActPage->paintGroupRect();
			double x, y, w, h;
			doc->ActPage->getGroupRect(&x, &y, &w, &h);
			Mpal->setXY(x, y);
			Mpal->setXY(w, h);
		}
		if (doc->ActPage->SelItem.count() > 0)
		{
			b = doc->ActPage->SelItem.at(0);
			doc->ActPage->EmitValues(b);
			HaveNewSel(b->PType);
		}
	}
}

void ScribusApp::ClipChange()
{
	QString cc;
#if QT_VERSION  >= 0x030100
	cc = ClipB->text(QClipboard::Clipboard);
	if (cc.isNull())
		cc = ClipB->text(QClipboard::Selection);
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
				if (doc->AppMode != 7)
					editMenu->setItemEnabled(edid3, 1);
			}
			else
			{
				if (doc->AppMode == 7)
					editMenu->setItemEnabled(edid3, 1);
			}
		}
	}
}

void ScribusApp::DeleteText()
{
	PageItem *b = doc->ActPage->SelItem.at(0);
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
		for (uint a = 0; a < nb->Ptext.count(); ++a)
		{
			DeleteSel(nb);
			nb->Dirty = true;
			nb->CPos = 0;
		}
		nb = nb->NextBox;
	}
	//	doc->ActPage->RefreshItem(b);
	view->DrawNew();
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
	QString pfad = PREL;
#if defined(__hpux)
	pfad += "/lib/scribus/libs/libabout.sl";
#else
	pfad += "/lib/scribus/libs/libabout.so";
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
	HelpBrowser *dia = new HelpBrowser(this, tr("Scribus Manual"));
	dia->show();
}

void ScribusApp::ToggleTips()
{
	tipsOn = !tipsOn;
	settingsMenu->setItemChecked(tip, tipsOn);
	QToolTip::setEnabled(tipsOn);
}

void ScribusApp::SaveText()
{
	LoadEnc = "";
	QString fn = CFileDialog( tr("Save as"), tr("Text Files (*.txt);;All Files(*)"), "", false, false, false, true);
	if (!fn.isEmpty())
	{
		Serializer *se = new Serializer(fn);
		se->PutText(doc->ActPage->SelItem.at(0));
		se->Write(LoadEnc);
		delete se;
	}
}

void ScribusApp::applyNewMaster(QString name)
{
	Apply_Temp(name, doc->ActPage->PageNr);
	view->DrawNew();
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
	Sepal->Rebuild();
}

void ScribusApp::slotNewPageP(int wo, QString templ)
{
	NoFrameEdit();
	doc->ActPage->Deselect(true);
	doc->UnDoValid = false;
	CanUndo();
	slotNewPage(wo);
	applyNewMaster(templ);
	if (doc->TemplateMode)
		view->MasterPages = view->Pages;
	else
		view->DocPages = view->Pages;
	Sepal->RebuildPage();
}

/** Erzeugt eine neue Seite */
void ScribusApp::slotNewPageM()
{
	NoFrameEdit();
	doc->ActPage->Deselect(true);
	InsPage *dia = new InsPage(this, view, doc->ActPage->PageNr, view->Pages.count(), doc->PageFP);
	if (dia->exec())
	{
		QString based2;
		if (doc->PageFP)
			based2 = dia->Based2->currentText();
		else
			based2 = tr("Normal");

		addNewPages(dia->ActualPage->value(), 
		            dia->Where->currentItem(), 
		            dia->NumPages->value(),
		            dia->Based->currentText(),
		            based2);
	}
	delete dia;
}

void ScribusApp::addNewPages(int wo, int where, int numPages, QString based1, QString based2)
{
		doc->UnDoValid = false;
		CanUndo();
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
					if ((doc->ActPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
						applyNewMaster(based1);
					if ((doc->ActPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->ActPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->ActPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
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
					if ((doc->ActPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
						applyNewMaster(based1);
					if ((doc->ActPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->ActPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->ActPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
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
				slotNewPage(view->Pages.count());
				if (doc->PageFP)
				{
					if ((doc->ActPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
						applyNewMaster(based1);
					if ((doc->ActPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->ActPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
						applyNewMaster(based2);
					if ((doc->ActPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
						applyNewMaster(based1);
				}
				else
					applyNewMaster(based1);
			}
			break;
		}
		Sepal->RebuildPage();
		view->DrawNew();
		if (doc->TemplateMode)
			view->MasterPages = view->Pages;
		else
			view->DocPages = view->Pages;
}

void ScribusApp::slotNewPageT(int w)
{
	if (doc->TemplateMode)
		slotNewPage(w);
}

void ScribusApp::slotNewPage(int w)
{
	if ((!doc->loading) && (!doc->TemplateMode))
		Tpal->slotAddPage(w);
	view->addPage(w);
	if (view->Pages.count() > 1)
	{
		pageMenu->setItemEnabled(pgmd, 1);
		pageMenu->setItemEnabled(pgmv, 1);
	}
	if ((!doc->loading) && (!doc->TemplateMode))
	{
		AdjustBM();
		if ((doc->PageAT) && (doc->PageC != 1))
			Tpal->slotAddElement(w, 0);
	}
	connect(doc->ActPage, SIGNAL(Amode(int)), this, SLOT(setAppMode(int)));
	connect(doc->ActPage, SIGNAL(PaintingDone()), this, SLOT(slotSelect()));
	connect(doc->ActPage, SIGNAL(HaveSel(int)), this, SLOT(HaveNewSel(int)));
	connect(doc->ActPage, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(doc->ActPage, SIGNAL(ClipPo(double, double)), Npal, SLOT(SetXY(double, double)));
	connect(doc->ActPage, SIGNAL(HavePoint(bool, bool)), Npal, SLOT(HaveNode(bool, bool)));
	connect(doc->ActPage, SIGNAL(PolyOpen()), Npal, SLOT(IsOpen()));
	connect(doc->ActPage, SIGNAL(PStatus(int, uint)), Npal, SLOT(PolyStatus(int, uint)));
	connect(doc->ActPage, SIGNAL(MousePos(double, double)), this, SLOT(ReportMP(double, double)));
	connect(doc->ActPage, SIGNAL(ItemPos(double, double)), Mpal, SLOT(setXY(double, double)));
	connect(doc->ActPage, SIGNAL(ItemGeom(double, double)), Mpal, SLOT(setBH(double, double)));
	connect(doc->ActPage, SIGNAL(SetAngle(double)), Mpal, SLOT(setR(double)));
	connect(doc->ActPage, SIGNAL(ItemRadius(double)), Mpal, SLOT(setRR(double)));
	connect(doc->ActPage, SIGNAL(ItemTextAttr(double)), Mpal, SLOT(setLsp(double)));
	connect(doc->ActPage, SIGNAL(ItemTextCols(int, double)), Mpal, SLOT(setCols(int, double)));
	connect(doc->ActPage, SIGNAL(ItemTextSize(int)), Mpal, SLOT(setSize(int)));
	connect(doc->ActPage, SIGNAL(ItemTextUSval(double)), Mpal, SLOT(setExtra(double)));
	connect(doc->ActPage, SIGNAL(SetDistValues(double, double, double, double)), Mpal, SLOT(setDvals(double, double, double, double)));
	connect(doc->ActPage, SIGNAL(SetLocalValues(double, double, double, double)), Mpal, SLOT(setLvalue(double, double, double, double)));
	connect(doc->ActPage, SIGNAL(SetSizeValue(double)), Mpal, SLOT(setSvalue(double)));
	connect(doc->ActPage, SIGNAL(ItemTextStil(int)), Mpal, SLOT(setStil(int)));
	connect(doc->ActPage, SIGNAL(ItemTextSca(int)), Mpal, SLOT(setTScale(int)));
	connect(doc->ActPage, SIGNAL(ItemTextAbs(int)), Mpal, SLOT(setAli(int)));
	connect(doc->ActPage, SIGNAL(SetLineArt(PenStyle, PenCapStyle, PenJoinStyle)), Mpal, SLOT( setLIvalue(PenStyle, PenCapStyle, PenJoinStyle)));
	connect(doc->ActPage, SIGNAL(ItemFarben(QString, QString, int, int)), this, SLOT(setCSMenu(QString, QString, int, int)));
	connect(doc->ActPage, SIGNAL(ItemFarben(QString, QString, int, int)), Mpal->Cpal, SLOT(setActFarben(QString, QString, int, int)));
	connect(doc->ActPage, SIGNAL(ItemGradient(int)), Mpal->Cpal, SLOT(setActGradient(int)));
	connect(doc->ActPage, SIGNAL(ItemTrans(double, double)), Mpal->Cpal, SLOT(setActTrans(double, double)));
	connect(doc->ActPage, SIGNAL(ItemTextFont(QString)), this, SLOT(AdjustFontMenu(QString)));
	connect(doc->ActPage, SIGNAL(ItemTextSize(int)), this, SLOT(setFSizeMenu(int)));
	connect(doc->ActPage, SIGNAL(ItemTextStil(int)), this, SLOT(setStilvalue(int)));
	connect(doc->ActPage, SIGNAL(ItemTextAbs(int)), this, SLOT(setAbsValue(int)));
	connect(doc->ActPage, SIGNAL(ItemTextFarben(QString, QString, int, int)), Mpal, SLOT(setActFarben(QString, QString, int, int)));
	connect(doc->ActPage, SIGNAL(HasTextSel()), this, SLOT(EnableTxEdit()));
	connect(doc->ActPage, SIGNAL(HasNoTextSel()), this, SLOT(DisableTxEdit()));
	connect(doc->ActPage, SIGNAL(CopyItem()), this, SLOT(slotEditCopy()));
	connect(doc->ActPage, SIGNAL(CutItem()), this, SLOT(slotEditCut()));
	connect(doc->ActPage, SIGNAL(LoadPic()), this, SLOT(slotFileOpen()));
	connect(doc->ActPage, SIGNAL(AppendText()), this, SLOT(slotFileAppend()));
	connect(doc->ActPage, SIGNAL(AnnotProps()), this, SLOT(ModifyAnnot()));
	connect(doc->ActPage, SIGNAL(ToScrap(QString)), this, SLOT(PutScrap(QString)));
	connect(doc->ActPage, SIGNAL(UndoAvail()), this, SLOT(CanUndo()));
	connect(doc->ActPage, SIGNAL(EditGuides()), this, SLOT(ManageGuides()));
	connect(doc->ActPage, SIGNAL(LoadElem(QString, int ,int, bool, bool, ScribusDoc *)), this, SLOT(slotElemRead(QString, int, int, bool, bool, ScribusDoc *)));
	connect(doc->ActPage, SIGNAL(AddBM(PageItem *)), this, SLOT(AddBookMark(PageItem *)));
	connect(doc->ActPage, SIGNAL(DelBM(PageItem *)), this, SLOT(DelBookMark(PageItem *)));
	connect(doc->ActPage, SIGNAL(ChBMText(PageItem *)), this, SLOT(BookMarkTxT(PageItem *)));
	connect(doc->ActPage, SIGNAL(NewBMNr(int, int)), BookPal->BView, SLOT(ChangeItem(int, int)));
	connect(doc->ActPage, SIGNAL(RasterPic(bool)), this, SLOT(HaveRaster(bool)));
	connect(doc->ActPage, SIGNAL(EditText()), this, SLOT(slotStoryEditor()));
	connect(doc->ActPage, SIGNAL(DoGroup()), this, SLOT(GroupObj()));
	connect(doc->ActPage, SIGNAL(DoUnGroup()), this, SLOT(UnGroupObj()));
	connect(doc->ActPage, SIGNAL(EndNodeEdit()), this, SLOT(ToggleFrameEdit()));
	connect(doc->ActPage, SIGNAL(LevelChanged(uint )), Mpal, SLOT(setLevel(uint)));
	if (!doc->TemplateMode)
	{
		connect(doc->ActPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
		connect(doc->ActPage, SIGNAL(AddObj(uint, uint)), Tpal, SLOT(slotAddElement(uint, uint)));
		connect(doc->ActPage, SIGNAL(UpdtObj(uint, uint)), Tpal, SLOT(slotUpdateElement(uint, uint)));
		connect(doc->ActPage, SIGNAL(MoveObj(uint, uint, uint)), Tpal, SLOT(slotMoveElement(uint, uint, uint)));
	}
	slotDocCh(!doc->loading);
}

/** Ansicht absolut zoomen */
void ScribusApp::slotZoomAbs(double z)
{
	view->rememberPreviousSettings();
	doc->Scale = z;
	view->slotDoZoom();
}

void ScribusApp::slotZoomFit()
{
	double dx = (view->width()-50) / (doc->PageB+30);
	double dy = (view->height()-70) / (doc->PageH+30);
	view->rememberPreviousSettings();
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
		setMapal(false);
		setMpal(false);
		setTpal(false);
		setBpal(false);
		setLpal(false);
		setSepal(false);
		setBookpal(false);
		PalettesStat[0] = true;
	}
}

void ScribusApp::setMapal(bool visible)
{
	if (visible)
	{
		MaPal->show();
		MaPal->move(Prefs.Mapalx, Prefs.Mapaly);
	}
	else
	{
		Prefs.Mapalx = MaPal->pos().x();
		Prefs.Mapaly = MaPal->pos().y();
		MaPal->hide();
	}
}

void ScribusApp::setMpal(bool visible)
{
	if (visible)
	{
		Mpal->show();
//		Mpal->TabStack->raiseWidget(0);
		Mpal->move(Prefs.Mpalx, Prefs.Mpaly);
	}
	else
	{
		Prefs.Mpalx = Mpal->pos().x();
		Prefs.Mpaly = Mpal->pos().y();
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
		Tpal->move(Prefs.Tpalx, Prefs.Tpaly);
	}
	else
	{
		Prefs.Tpalx = Tpal->pos().x();
		Prefs.Tpaly = Tpal->pos().y();
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
		ScBook->move(Prefs.SCpalx, Prefs.SCpaly);
		ScBook->resize(Prefs.SCpalw, Prefs.SCpalh);
	}
	else
	{
		Prefs.SCpalx = ScBook->pos().x();
		Prefs.SCpaly = ScBook->pos().y();
		Prefs.SCpalw = ScBook->size().width();
		Prefs.SCpalh = ScBook->size().height();
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
		Lpal->move(Prefs.Lpalx, Prefs.Lpaly);
	}
	else
	{
		Prefs.Lpalx = Lpal->pos().x();
		Prefs.Lpaly = Lpal->pos().y();
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
		Sepal->move(Prefs.Sepalx, Prefs.Sepaly);
	}
	else
	{
		Prefs.Sepalx = Sepal->pos().x();
		Prefs.Sepaly = Sepal->pos().y();
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
		BookPal->move(Prefs.Bopalx, Prefs.Bopaly);
	}
	else
	{
		Prefs.Bopalx = BookPal->pos().x();
		Prefs.Bopaly = BookPal->pos().y();
		BookPal->hide();
	}
	toolMenu->setItemChecked(viewBopal, visible);
}

void ScribusApp::ToggleBookpal()
{
	setBookpal(!BookPal->isVisible());
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
	settingsMenu->setItemChecked(toolbarMenuTools, visible);
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
	settingsMenu->setItemChecked(toolbarMenuPDFTools, visible);
}

void ScribusApp::TogglePDFTools()
{
	setPDFTools(!WerkToolsP->Sichtbar);
}

void ScribusApp::TogglePics()
{
	uint a, b;
	doc->ShowPic = !doc->ShowPic;
	viewMenu->setItemChecked(Bilder, doc->ShowPic);
	for (a=0; a<view->Pages.count(); ++a)
	{
		for (b=0; b<view->Pages.at(a)->Items.count(); ++b)
		{
			if (view->Pages.at(a)->Items.at(b)->PType == 2)
			{
				view->Pages.at(a)->Items.at(b)->PicArt = doc->ShowPic;
			}
		}
		view->Pages.at(a)->update();
	}
}

void ScribusApp::ToggleAllGuides()
{
	if (GuidesStat[0])
	{
		GuidesStat[0] = false;
		Prefs.MarginsShown = GuidesStat[1];
		Prefs.FramesShown = GuidesStat[2];
		Prefs.GridShown = GuidesStat[3];
		Prefs.GuidesShown = GuidesStat[4];
		Prefs.BaseShown = GuidesStat[5];
		ToggleMarks();
		ToggleFrames();
		ToggleRaster();
		ToggleGuides();
		ToggleBase();
	}
	else
	{
		GuidesStat[0] = true;
		GuidesStat[1] = !Prefs.MarginsShown;
		GuidesStat[2] = !Prefs.FramesShown;
		GuidesStat[3] = !Prefs.GridShown;
		GuidesStat[4] = !Prefs.GuidesShown;
		GuidesStat[5] = !Prefs.BaseShown;
		Prefs.MarginsShown = false;
		Prefs.FramesShown = false;
		Prefs.GridShown = false;
		Prefs.GuidesShown = false;
		Prefs.BaseShown = false;
		viewMenu->setItemChecked(Markers, Prefs.MarginsShown);
		viewMenu->setItemChecked(FrameDr, Prefs.FramesShown);
		viewMenu->setItemChecked(Ras, Prefs.GridShown);
		viewMenu->setItemChecked(Guide, Prefs.GuidesShown);
		viewMenu->setItemChecked(Base, Prefs.BaseShown);
	}
	view->DrawNew();
}

void ScribusApp::ToggleMarks()
{
	GuidesStat[0] = false;
	Prefs.MarginsShown = !Prefs.MarginsShown;
	viewMenu->setItemChecked(Markers, Prefs.MarginsShown);
	view->DrawNew();
}

void ScribusApp::ToggleFrames()
{
	GuidesStat[0] = false;
	Prefs.FramesShown = !Prefs.FramesShown;
	viewMenu->setItemChecked(FrameDr, Prefs.FramesShown);
	view->DrawNew();
}

void ScribusApp::ToggleRaster()
{
	GuidesStat[0] = false;
	Prefs.GridShown = !Prefs.GridShown;
	viewMenu->setItemChecked(Ras, Prefs.GridShown);
	view->DrawNew();
}

void ScribusApp::ToggleGuides()
{
	GuidesStat[0] = false;
	Prefs.GuidesShown = !Prefs.GuidesShown;
	viewMenu->setItemChecked(Guide, Prefs.GuidesShown);
	view->DrawNew();
}

void ScribusApp::ToggleBase()
{
	GuidesStat[0] = false;
	Prefs.BaseShown = !Prefs.BaseShown;
	viewMenu->setItemChecked(Base, Prefs.BaseShown);
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
		Npal->setDoc(doc);
		Npal->MoveN();
		Npal->HaveNode(false, false);
		Npal->MoveNode->setOn(true);
		Npal->show();
		Npal->move(Prefs.Npalx, Prefs.Npaly);
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
		if (doc->ActPage->SelItem.count() != 0)
		{
			PageItem* b = doc->ActPage->SelItem.at(0);
			doc->ActPage->MarkClip(b);
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
	Prefs.Npalx = Npal->pos().x();
	Prefs.Npaly = Npal->pos().y();
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
		doc->ActPage->EditContour = false;
		if (doc->ActPage->SelItem.count() != 0)
		{
			HaveNewSel(doc->ActPage->SelItem.at(0)->PType);
			doc->ActPage->update();
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
	setAppMode(1);
}

void ScribusApp::ModeFromTB(int m)
{
	if (m == 3)
	{
		slotStoryEditor();
		slotSelect();
		return;
	}
	if (m == 10)
		doc->ElemToLink = doc->ActPage->SelItem.at(0);
	if (doc->AppMode == 13)
		return;
	setAppMode(m);
}

void ScribusApp::setAppMode(int mode)
{
	PageItem *b;
	setActiveWindow();
	if (HaveDoc)
	{
		if (doc->ActPage->SelItem.count() != 0)
			b = doc->ActPage->SelItem.at(0);
		else
			b = 0;
		int oldMode = doc->AppMode;
		doc->AppMode = mode;
		if (oldMode == 24)
			disconnect(doc->ActPage, SIGNAL(MVals(double, double, double, double, double, double, int )), MaPal, SLOT(setValues(double, double, double, double, double, double, int )));
		if (oldMode == 7)
		{
			disconnect(doc->CurTimer, SIGNAL(timeout()), doc->ActPage, SLOT(BlinkCurs()));
			doc->CurTimer->stop();
			view->LE->setFocusPolicy(QWidget::ClickFocus);
			view->PGS->PageCombo->setFocusPolicy(QWidget::ClickFocus);
			delete doc->CurTimer;
			doc->CurTimer = 0;
			editMenu->setItemEnabled(edid4, 0);
			doc->ActPage->slotDoCurs(false);
			if (b != 0)
			{
				doc->ActPage->RefreshItem(b);
				menuBar()->setItemEnabled(Stm, 1);
				menuBar()->setItemEnabled(Obm, 1);
			}
			view->HR->ItemPosValid = false;
			view->HR->repX = false;
			view->HR->repaint();
		}
		if (mode == 7)
		{
			view->LE->setFocusPolicy(QWidget::NoFocus);
			view->PGS->PageCombo->setFocusPolicy(QWidget::NoFocus);
			if (b != 0)
			{
				if ((b->PType == 6) || (b->PType == 7) || (b->PType == 8))
				{
					doc->AppMode = 1;
					ToggleFrameEdit();
					return;
				}
				setTBvals(b);
			}
			editMenu->setItemEnabled(edid3, 0);
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
			doc->ActPage->slotDoCurs(true);
			menuBar()->setItemEnabled(Obm, 0);
			menuBar()->setItemEnabled(Stm, 0);
			doc->CurTimer = new QTimer(doc->ActPage);
			connect(doc->CurTimer, SIGNAL(timeout()), doc->ActPage, SLOT(BlinkCurs()));
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
				doc->ActPage->RefreshItem(b);
			}
		}
		if (mode == 13)
		{
			if (doc->ActPage->SelItem.count() != 0)
				doc->ActPage->Deselect(true);
		}
		if (mode == 24)
		{
			MaPal->show();
			connect(doc->ActPage, SIGNAL(MVals(double, double, double, double, double, double, int)), MaPal, SLOT(setValues(double, double, double, double, double, double, int )));
		}
		if (mode == 6)
			qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
		else
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		if (mode == 2)
		{
			doc->SubMode = WerkTools->SubMode;
			doc->ShapeValues = WerkTools->ShapeVals;
			doc->ValCount = WerkTools->ValCount;
			Mpal->SCustom->setPixmap(Mpal->SCustom->getIconPixmap(doc->SubMode));
			SCustom->setPixmap(SCustom->getIconPixmap(doc->SubMode));
		}
		else
			doc->SubMode = -1;
		if (mode == 1)
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
		}
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
	if (doc->ActPage->SelItem.count() != 0)
	{
		setActiveWindow();
		doc->CurrentStyle = h;
		setStilvalue(doc->CurrentStyle);
		doc->ActPage->chTyStyle(h);
		slotDocCh();
	}
}

void ScribusApp::AdjustBM()
{
	for (uint a = 0; a < view->Pages.count(); ++a)
	{
		for (uint b = 0; b < view->Pages.at(a)->Items.count(); ++b)
		{
			PageItem* bb = view->Pages.at(a)->Items.at(b);
			if (bb->isBookmark)
			{
				int it = bb->BMnr;
				QListViewItemIterator itn(BookPal->BView);
				for ( ; itn.current(); ++itn)
				{
					BookMItem *ite = (BookMItem*)itn.current();
					if (ite->ItemNr == it)
					{
						ite->Seite = a;
						break;
					}
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
	if (view->Pages.count() == 1)
		return;
	if (!doc->TemplateMode)
		disconnect(doc->ActPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
	view->Pages.at(pg)->SelItem.clear();
	for (uint d = 0; d < view->Pages.at(pg)->Items.count(); ++d)
	{
		ite = view->Pages.at(pg)->Items.at(d);
		if (ite->isBookmark)
			DelBookMark(ite);
		if (ite->PType == 4)
			view->Pages.at(pg)->SelItem.append(ite);
	}
	if (view->Pages.at(pg)->SelItem.count() != 0)
		view->Pages.at(pg)->DeleteItem();
	disconnect(view->Pages.at(pg), SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
	view->delPage(pg);
	AdjustBM();
	if (!doc->TemplateMode)
		connect(doc->ActPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
	view->reformPages();
	doc->OpenNodes.clear();
	Tpal->BuildTree(view);
	if (view->Pages.count() == 1)
	{
		pageMenu->setItemEnabled(pgmd, 0);
		pageMenu->setItemEnabled(pgmv, 0);
	}
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
	Sepal->RebuildPage();
}

void ScribusApp::DeletePage()
{
	int a, pg;
	PageItem* ite;
	NoFrameEdit();
	doc->ActPage->Deselect(true);
	DelPages *dia = new DelPages(this, doc->ActPage->PageNr+1, view->Pages.count());
	if (dia->exec())
	{
		if (!doc->TemplateMode)
			disconnect(doc->ActPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
		pg = dia->FromPage->value()-1;
		for (a = pg; a < dia->ToPage->value(); ++a)
		{
			disconnect(view->Pages.at(pg), SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
			view->Pages.at(pg)->SelItem.clear();
			for (uint d = 0; d < view->Pages.at(pg)->Items.count(); ++d)
			{
				ite = view->Pages.at(pg)->Items.at(d);
				if (ite->isBookmark)
					DelBookMark(ite);
				if (ite->PType == 4)
					view->Pages.at(pg)->SelItem.append(ite);
			}
			if (view->Pages.at(pg)->SelItem.count() != 0)
				view->Pages.at(pg)->DeleteItem();
			view->delPage(pg);
			AdjustBM();
		}
		if (!doc->TemplateMode)
			connect(doc->ActPage, SIGNAL(DelObj(uint, uint)), Tpal, SLOT(slotRemoveElement(uint, uint)));
		view->reformPages();
		doc->OpenNodes.clear();
		Tpal->BuildTree(view);
		if (view->Pages.count() == 1)
		{
			pageMenu->setItemEnabled(pgmd, 0);
			pageMenu->setItemEnabled(pgmv, 0);
		}
		slotDocCh();
		doc->UnDoValid = false;
		CanUndo();
		Sepal->RebuildPage();
	}
	delete dia;
}

void ScribusApp::MovePage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->ActPage->PageNr+1, view->Pages.count(), true);
	if (dia->exec())
	{
		doc->OpenNodes = Tpal->buildReopenVals();
		int from = dia->FromPage->value();
		int to = dia->ToPage->value();
		int wie = dia->Where->currentItem();
		int wo = dia->ActualPage->value();
		if (from != wo)
			view->movePage(from-1, to, wo-1, wie);
		slotDocCh();
		doc->UnDoValid = false;
		CanUndo();
		AdjustBM();
		Sepal->RebuildPage();
		Tpal->BuildTree(view);
		Tpal->reopenTree(doc->OpenNodes);
	}
	delete dia;
}

void ScribusApp::CopyPage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->ActPage->PageNr+1, view->Pages.count(), false);
	if (dia->exec())
	{
		doc->loading = true;
		Page* from = view->Pages.at(dia->FromPage->value()-1);
		int wo = dia->ActualPage->value();
		switch (dia->Where->currentItem())
		{
		case 0:
			slotNewPage(wo-1);
			break;
		case 1:
			slotNewPage(wo);
			break;
		case 2:
			slotNewPage(view->Pages.count());
			break;
		}
		Page* Ziel = doc->ActPage;
		QMap<int,int> TableID;
		QPtrList<PageItem> TableItems;
		TableID.clear();
		TableItems.clear();
		for (uint ite = 0; ite < from->Items.count(); ++ite)
		{
			CopyPageItem(&Buffer, from->Items.at(ite));
			Ziel->PasteItem(&Buffer, true, true);
			if (from->Items.at(ite)->isBookmark)
				AddBookMark(Ziel->Items.at(Ziel->Items.count()-1));
			PageItem* Neu = Ziel->Items.at(Ziel->Items.count()-1);
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
					ta->TopLink = Ziel->Items.at(TableID[ta->TopLinkID]);
				else
					ta->TopLink = 0;
				if (ta->LeftLinkID != -1)
					ta->LeftLink = Ziel->Items.at(TableID[ta->LeftLinkID]);
				else
					ta->LeftLink = 0;
				if (ta->RightLinkID != -1)
					ta->RightLink = Ziel->Items.at(TableID[ta->RightLinkID]);
				else
					ta->RightLink = 0;
				if (ta->BottomLinkID != -1)
					ta->BottomLink = Ziel->Items.at(TableID[ta->BottomLinkID]);
				else
					ta->BottomLink = 0;
			}
		}
		Ziel->MPageNam = from->MPageNam;
		Ziel->Deselect(true);
		doc->loading = false;
		view->DrawNew();
		slotDocCh();
		doc->UnDoValid = false;
		CanUndo();
		Sepal->RebuildPage();
		Tpal->BuildTree(view);
		AdjustBM();
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
			if (doc->ActPage->SelItem.count() != 0)
			{
				PageItem *b = doc->ActPage->SelItem.at(0);
				nf2 = b->IFont;
			}
			Mpal->Fonts->RebuildList(&Prefs);
			BuildFontMenu();
		}
	}
	AdjustFontMenu(nf2);
	doc->ActPage->ItemFont(nf2);
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
		doc->ActPage->chFSize(c);
	}
	else
	{
		Query* dia = new Query(this, "New", 1, 0, tr("&Size:"), tr("Size"));
		if (dia->exec())
		{
			c = qRound(dia->Answer->text().toDouble(&ok) * 10);
			if ((ok) && (c < 10250) && (c > 0))
				doc->ActPage->chFSize(c);
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
	if (doc->ActPage->SelItem.count() != 0)
	{
		PageItem *b = doc->ActPage->SelItem.at(0);
		if ((b->PType == 4) || (b->PType == 8))
			doc->ActPage->ItemTextBrush(ColorMenC->text(id));
		else
			doc->ActPage->ItemBrush(ColorMenC->text(id));
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
	if (doc->ActPage->SelItem.count() != 0)
	{
		PageItem *b = doc->ActPage->SelItem.at(0);
		if (c > 0)
		{
			if ((b->PType == 4) || (b->PType == 8))
				doc->ActPage->ItemTextBrushS((c-1) * 10);
			else
				doc->ActPage->ItemBrushShade((c-1) * 10);
		}
		else
		{
			Query* dia = new Query(this, "New", 1, 0, tr("&Shade:"), tr("Shade"));
			if (dia->exec())
			{
				c = dia->Answer->text().toInt(&ok);
				if (ok)
				{
					if ((b->PType == 4) || (b->PType == 8))
						doc->ActPage->ItemTextBrushS(c);
					else
						doc->ActPage->ItemBrushShade(c);
				}
				delete dia;
			}
		}
	}
	slotDocCh();
}

void ScribusApp::setCSMenu(QString k, QString l, int lk , int ls)
{
	uint a;
	QString la;
	int lb;
	PageItem *b;
	if (doc->ActPage->SelItem.count() != 0)
	{
		b = doc->ActPage->SelItem.at(0);
		if ((b->PType == 4) || (b->PType == 8))
		{
			if ((doc->AppMode == 7) && (b->Ptext.count() != 0))
			{
				la = b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->ccolor;
				lb = b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cshade;
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
		view->MasterPages = view->Pages;
	else
		view->DocPages = view->Pages;
	PageItem* ite;
	doc->MLineStyles = dia->TempStyles;
	for (uint c = 0; c < view->DocPages.count(); ++c)
	{
		for (uint d = 0; d < view->DocPages.at(c)->Items.count(); ++d)
		{
			ite = view->DocPages.at(c)->Items.at(d);
			if (ite->NamedLStyle != "")
			{
				if (!doc->MLineStyles.contains(ite->NamedLStyle))
					ite->NamedLStyle = dia->Replacement[ite->NamedLStyle];
			}
		}
	}
	for (uint c1 = 0; c1 < view->MasterPages.count(); ++c1)
	{
		for (uint d1 = 0; d1 < view->MasterPages.at(c1)->Items.count(); ++d1)
		{
			ite = view->MasterPages.at(c1)->Items.at(d1);
			if (ite->NamedLStyle != "")
			{
				if (!doc->MLineStyles.contains(ite->NamedLStyle))
					ite->NamedLStyle = dia->Replacement[ite->NamedLStyle];
			}
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
	PageItem* ite;
	bool ff;
	uint nr;
	ers.clear();
	ers.append(0);
	ers.append(1);
	ers.append(2);
	ers.append(3);
	ers.append(4);
	if (doc->TemplateMode)
		view->MasterPages = view->Pages;
	else
		view->DocPages = view->Pages;
	for (uint a=5; a<doc->Vorlagen.count(); ++a)
	{
		ff = false;
		nn = doc->Vorlagen[a].Vname;
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
				if ((doc->Vorlagen[a].LineSpa == dia->TempVorl[b].LineSpa) &&
					(doc->Vorlagen[a].Indent == dia->TempVorl[b].Indent) &&
					(doc->Vorlagen[a].First == dia->TempVorl[b].First) &&
					(doc->Vorlagen[a].Ausri == dia->TempVorl[b].Ausri) &&
					(doc->Vorlagen[a].Avor == dia->TempVorl[b].Avor) &&
					(doc->Vorlagen[a].Anach == dia->TempVorl[b].Anach) &&
					(doc->Vorlagen[a].Font == dia->TempVorl[b].Font) &&
					(doc->Vorlagen[a].TabValues == dia->TempVorl[b].TabValues) &&
					(doc->Vorlagen[a].Drop == dia->TempVorl[b].Drop) &&
					(doc->Vorlagen[a].DropLin == dia->TempVorl[b].DropLin) &&
					(doc->Vorlagen[a].FontEffect == dia->TempVorl[b].FontEffect) &&
					(doc->Vorlagen[a].FColor == dia->TempVorl[b].FColor) &&
					(doc->Vorlagen[a].FShade == dia->TempVorl[b].FShade) &&
					(doc->Vorlagen[a].SColor == dia->TempVorl[b].SColor) &&
					(doc->Vorlagen[a].SShade == dia->TempVorl[b].SShade) &&
					(doc->Vorlagen[a].BaseAdj == dia->TempVorl[b].BaseAdj) &&
					(doc->Vorlagen[a].FontSize == dia->TempVorl[b].FontSize))
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
	for (uint c=0; c<view->DocPages.count(); ++c)
	{
		for (uint d=0; d<view->DocPages.at(c)->Items.count(); ++d)
		{
			ite = view->DocPages.at(c)->Items.at(d);
			if (ite->PType == 4)
			{
				for (uint e=0; e<ite->Ptext.count(); ++e)
				{
					int cabori = ite->Ptext.at(e)->cab;
					int cabneu = ers[cabori];
					if (cabori > 4)
					{
						if (cabneu > 0)
						{
							if (ite->Ptext.at(e)->cfont == doc->Vorlagen[cabori].Font)
								ite->Ptext.at(e)->cfont = dia->TempVorl[cabneu].Font;
							if (ite->Ptext.at(e)->csize == doc->Vorlagen[cabori].FontSize)
								ite->Ptext.at(e)->csize = dia->TempVorl[cabneu].FontSize;
							if ((ite->Ptext.at(e)->cstyle & 127 ) == doc->Vorlagen[cabori].FontEffect)
							{
								ite->Ptext.at(e)->cstyle &= ~127;
								ite->Ptext.at(e)->cstyle |= dia->TempVorl[cabneu].FontEffect;
							}
							if (ite->Ptext.at(e)->ccolor == doc->Vorlagen[cabori].FColor)
								ite->Ptext.at(e)->ccolor = dia->TempVorl[cabneu].FColor;
							if (ite->Ptext.at(e)->cshade == doc->Vorlagen[cabori].FShade)
								ite->Ptext.at(e)->cshade = dia->TempVorl[cabneu].FShade;
							if (ite->Ptext.at(e)->cstroke == doc->Vorlagen[cabori].SColor)
								ite->Ptext.at(e)->cstroke = dia->TempVorl[cabneu].SColor;
							if (ite->Ptext.at(e)->cshade2 == doc->Vorlagen[cabori].SShade)
								ite->Ptext.at(e)->cshade2 = dia->TempVorl[cabneu].SShade;
						}
						else
						{
							ite->Ptext.at(e)->ccolor = ite->TxtFill;
							ite->Ptext.at(e)->cshade = ite->ShTxtFill;
							ite->Ptext.at(e)->cstroke = ite->TxtStroke;
							ite->Ptext.at(e)->cshade2 = ite->ShTxtStroke;
							ite->Ptext.at(e)->csize = ite->ISize;
							ite->Ptext.at(e)->cstyle &= ~127;
							ite->Ptext.at(e)->cstyle |= ite->TxTStyle;
						}
						ite->Ptext.at(e)->cab = cabneu;
					}
				}
			}
		}
	}
	for (uint c=0; c<view->MasterPages.count(); ++c)
	{
		for (uint d=0; d<view->MasterPages.at(c)->Items.count(); ++d)
		{
			ite = view->MasterPages.at(c)->Items.at(d);
			if (ite->PType == 4)
			{
				for (uint e=0; e<ite->Ptext.count(); ++e)
				{
					int cabori = ite->Ptext.at(e)->cab;
					int cabneu = ers[cabori];
					if (cabori > 4)
					{
						if (cabneu > 0)
						{
							if (ite->Ptext.at(e)->cfont == doc->Vorlagen[cabori].Font)
								ite->Ptext.at(e)->cfont = dia->TempVorl[cabneu].Font;
							if (ite->Ptext.at(e)->csize == doc->Vorlagen[cabori].FontSize)
								ite->Ptext.at(e)->csize = dia->TempVorl[cabneu].FontSize;
							if ((ite->Ptext.at(e)->cstyle & 127 ) == doc->Vorlagen[cabori].FontEffect)
							{
								ite->Ptext.at(e)->cstyle &= ~127;
								ite->Ptext.at(e)->cstyle |= dia->TempVorl[cabneu].FontEffect;
							}
							if (ite->Ptext.at(e)->ccolor == doc->Vorlagen[cabori].FColor)
								ite->Ptext.at(e)->ccolor = dia->TempVorl[cabneu].FColor;
							if (ite->Ptext.at(e)->cshade == doc->Vorlagen[cabori].FShade)
								ite->Ptext.at(e)->cshade = dia->TempVorl[cabneu].FShade;
							if (ite->Ptext.at(e)->cstroke == doc->Vorlagen[cabori].SColor)
								ite->Ptext.at(e)->cstroke = dia->TempVorl[cabneu].SColor;
							if (ite->Ptext.at(e)->cshade2 == doc->Vorlagen[cabori].SShade)
								ite->Ptext.at(e)->cshade2 = dia->TempVorl[cabneu].SShade;
						}
						else
						{
							ite->Ptext.at(e)->ccolor = ite->TxtFill;
							ite->Ptext.at(e)->cshade = ite->ShTxtFill;
							ite->Ptext.at(e)->cstroke = ite->TxtStroke;
							ite->Ptext.at(e)->cshade2 = ite->ShTxtStroke;
							ite->Ptext.at(e)->csize = ite->ISize;
							ite->Ptext.at(e)->cstyle &= ~127;
							ite->Ptext.at(e)->cstyle |= ite->TxTStyle;
						}
						ite->Ptext.at(e)->cab = cabneu;
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
							if (chars->at(e)->cfont == doc->Vorlagen[cabori].Font)
								chars->at(e)->cfont = dia->TempVorl[cabneu].Font;
							if (chars->at(e)->csize == doc->Vorlagen[cabori].FontSize)
								chars->at(e)->csize = dia->TempVorl[cabneu].FontSize;
							if ((chars->at(e)->cstyle & 127 ) == doc->Vorlagen[cabori].FontEffect)
							{
								chars->at(e)->cstyle &= ~127;
								chars->at(e)->cstyle |= dia->TempVorl[cabneu].FontEffect;
							}
							if (chars->at(e)->ccolor == doc->Vorlagen[cabori].FColor)
								chars->at(e)->ccolor = dia->TempVorl[cabneu].FColor;
							if (chars->at(e)->cshade == doc->Vorlagen[cabori].FShade)
								chars->at(e)->cshade = dia->TempVorl[cabneu].FShade;
							if (chars->at(e)->cstroke == doc->Vorlagen[cabori].SColor)
								chars->at(e)->cstroke = dia->TempVorl[cabneu].SColor;
							if (chars->at(e)->cshade2 == doc->Vorlagen[cabori].SShade)
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
			CurrStED->Editor->CurrentABStil = ers[CurrStED->Editor->CurrentABStil];
		}
	}
	doc->Vorlagen = dia->TempVorl;
	if (CurrStED != NULL)
	{
		if (CurrStED->Editor->StyledText.count() != 0)
			CurrStED->Editor->updateAll();
	}
	for (uint a=0; a<doc->Vorlagen.count(); ++a)
	{
		if (doc->Vorlagen[a].Font != "")
		{
			QString nf = doc->Vorlagen[a].Font;
			if (!doc->UsedFonts.contains(nf))
			{
				if (doc->AddFont(nf, Prefs.AvailFonts[nf]->Font))
				{
					int ff = FontMenu->insertItem(new FmItem(nf, Prefs.AvailFonts[nf]->Font));
					FontID.insert(ff, Prefs.AvailFonts[nf]->SCName);
				}
				else
					doc->Vorlagen[a].Font = doc->Dfont;
			}
		}
	}
	Mpal->Spal->updateFList();
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
		doc->ActPage->SetAbStyle(a);
		doc->CurrentABStil = a;
		Mpal->setAli(a);
		PageItem *b = doc->ActPage->SelItem.at(0);
		setTBvals(b);
		slotDocCh();
	}
}

void ScribusApp::setAbsValue(int a)
{
	doc->CurrentABStil = a;
	Mpal->setAli(a);
	for (int b = 0; b < 5; ++b)
	{
		AliMenu->setItemChecked(AliMenu->idAt(b), false);
	}
}

void ScribusApp::slotEditColors()
{
	int a;
	uint b, c, d;
	CListe edc;
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
			CListe::Iterator it;
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
				if (!ers.isEmpty())
				{
					QMap<QString,QString>::Iterator it;
					for (it = ers.begin(); it != ers.end(); ++it)
					{
						if (it.key() == doc->CurrTextFill)
							doc->CurrTextFill = it.data();
						if (it.key() == doc->CurrTextStroke)
							doc->CurrTextStroke = it.data();
						for (b=0; b<view->DocPages.count(); ++b)
						{
							for (c=0; c<view->DocPages.at(b)->Items.count(); ++c)
							{
								ite = view->DocPages.at(b)->Items.at(c);
								if ((ite->PType == 4) || (ite->PType == 8))
								{
									for (d=0; d<ite->Ptext.count(); ++d)
									{
										if (it.key() == ite->Ptext.at(d)->ccolor)
											ite->Ptext.at(d)->ccolor = it.data();
										if (it.key() == ite->Ptext.at(d)->cstroke)
											ite->Ptext.at(d)->cstroke = it.data();
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
					for (it = ers.begin(); it != ers.end(); ++it)
					{
						for (b=0; b<view->MasterPages.count(); ++b)
						{
							for (c=0; c<view->MasterPages.at(b)->Items.count(); ++c)
							{
								ite = view->MasterPages.at(b)->Items.at(c);
								if ((ite->PType == 4) || (ite->PType == 8))
								{
									for (d=0; d<ite->Ptext.count(); ++d)
									{
										if (it.key() == ite->Ptext.at(d)->ccolor)
											ite->Ptext.at(d)->ccolor = it.data();
										if (it.key() == ite->Ptext.at(d)->cstroke)
											ite->Ptext.at(d)->cstroke = it.data();
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
	setActiveWindow();
	if (HaveDoc)
	{
		doc->ActPage->ItemPen(farbe);
		slotDocCh();
	}
}

void ScribusApp::setPenShade(int sh)
{
	setActiveWindow();
	if (HaveDoc)
	{
		doc->ActPage->ItemPenShade(sh);
		slotDocCh();
	}
}

void ScribusApp::setBrushFarbe(QString farbe)
{
	setActiveWindow();
	if (HaveDoc)
	{
		doc->ActPage->ItemBrush(farbe);
		slotDocCh();
	}
}

void ScribusApp::setBrushShade(int sh)
{
	setActiveWindow();
	if (HaveDoc)
	{
		doc->ActPage->ItemBrushShade(sh);
		slotDocCh();
	}
}

void ScribusApp::setGradFill(int typ)
{
	if (HaveDoc)
	{
		doc->ActPage->ItemGradFill(typ);
		slotDocCh();
	}
}

void ScribusApp::updtGradFill()
{
	if (HaveDoc)
	{
		if (doc->ActPage->SelItem.count() != 0)
		{
			PageItem *b = doc->ActPage->SelItem.at(0);
			b->fill_gradient = Mpal->Cpal->GradEdit->Preview->fill_gradient;
			doc->ActPage->RefreshItem(b);
			slotDocCh();
		}
	}
}

void ScribusApp::GetBrushPen()
{
	setActiveWindow();
	if (HaveDoc)
	{
		doc->ActPage->QueryFarben();
		slotDocCh();
	}
}

void ScribusApp::MakeFrame(int f, int c, double *vals)
{
	PageItem *b = doc->ActPage->SelItem.at(0);
	switch (f)
	{
	case 0:
		doc->ActPage->SetRectFrame(b);
		break;
	case 1:
		doc->ActPage->SetOvalFrame(b);
		break;
	default:
		doc->ActPage->SetFrameShape(b, c, vals);
		b->FrameType = f+2;
		break;
	}
	Mpal->SetCurItem(b);
	doc->ActPage->RefreshItem(b);
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::DeleteObjekt()
{
	if (!doc->EditClip)
		doc->ActPage->DeleteItem();
}

void ScribusApp::Objekt2Back()
{
	doc->ActPage->ToBack();
}

void ScribusApp::Objekt2Front()
{
	doc->ActPage->ToFront();
}

void ScribusApp::ObjektRaise()
{
	doc->ActPage->RaiseItem();
}

void ScribusApp::ObjektLower()
{
	doc->ActPage->LowerItem();
}

void ScribusApp::ObjektDup()
{
	slotEditCopy();
	doc->ActPage->Deselect(true);
	slotEditPaste();
	for (uint b=0; b<doc->ActPage->SelItem.count(); ++b)
	{
		doc->ActPage->SelItem.at(b)->Locked = false;
		doc->ActPage->MoveItem(DispX, DispY, doc->ActPage->SelItem.at(b));
	}
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::ObjektDupM()
{
	NoFrameEdit();
	Mdup *dia = new Mdup(this, DispX * UmReFaktor, DispY * UmReFaktor, doc->Einheit);
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
			doc->ActPage->Deselect(true);
			for (a=0; a<anz; ++a)
			{
				slotEditPaste();
				for (uint b=0; b<doc->ActPage->SelItem.count(); ++b)
				{
					doc->ActPage->SelItem.at(b)->Locked = false;
					doc->ActPage->MoveItem(dH2, dV2, doc->ActPage->SelItem.at(b), true);
				}
				dH2 += dH;
				dV2 += dV;
			}
			DispX = dH;
			DispY = dV;
			slotDocCh();
			doc->UnDoValid = false;
			CanUndo();
			doc->ActPage->Deselect(true);
		}
	}
	delete dia;
}

void ScribusApp::SelectFromOutl(int Page, int Item)
{
	NoFrameEdit();
	setActiveWindow();
	doc->ActPage->Deselect(true);
	view->GotoPage(Page);
	doc->ActPage->SelectItemNr(Item);
	if (doc->ActPage->SelItem.count() != 0)
	{
		PageItem *b = doc->ActPage->SelItem.at(0);
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
	doc->ActPage->Deselect(true);
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
	ein = doc->Einheit;
	NoFrameEdit();
	doc->ActPage->BuildAObj();
	Align *dia = new Align(this, doc->ActPage->AObjects.count(), ein, doc);
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
		doc->ActPage->AlignObj(xa, ya, Vth, Vtv, xdp, ydp, xart, yart);
		slotDocCh();
		doc->UnDoValid = false;
		CanUndo();
		HaveNewSel(doc->ActPage->SelItem.at(0)->PType);
	}
	delete dia;
}

void ScribusApp::DoAlign(bool xa, bool ya, bool Vth, bool Vtv, double xdp, double ydp, int xart, int yart)
{
	doc->ActPage->AlignObj(xa, ya, Vth, Vtv, xdp, ydp, xart, yart);
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::BuildFontMenu()
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

void ScribusApp::GetAllFonts()
{
	Prefs.AvailFonts.GetFonts(PrefsPfad);
	if (Prefs.AvailFonts.isEmpty())
		NoFonts = true;
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
	FontSub->RebuildList(&Prefs);
	Mpal->Fonts->RebuildList(&Prefs);
	disconnect(dia, SIGNAL(ReReadPrefs()), this, SLOT(ReadPrefs()));
	delete dia;
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
}

void ScribusApp::slotPrefsOrg()
{
	void *mo;
	const char *error;
	bool zChange = false;
	typedef Preferences* (*sdem)(QWidget *d, preV *Vor);
	sdem demo;
	QString pfad = PREL;
#if defined(__hpux)
	pfad += "/lib/scribus/libs/libpreferences.sl";
#else
	pfad += "/lib/scribus/libs/libpreferences.so";
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
		SetShortCut();
		Prefs.AppFontSize = dia->GFsize->value();
		Prefs.Wheelval = dia->SpinBox3->value();
		Prefs.RecentDCount = dia->Recen->value();
		Prefs.DocDir = dia->Docs->text();
		DocDir = Prefs.DocDir;
		Prefs.ProfileDir = dia->ProPfad->text();
		Prefs.ScriptDir = dia->ScriptPfad->text();
		GetCMSProfiles();
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
		if (Prefs.GUI != dia->GUICombo->currentText())
		{
			Prefs.GUI = dia->GUICombo->currentText();
			qApp->setStyle(QStyleFactory::create(Prefs.GUI));
		}
		QFont apf = qApp->font();
		apf.setPointSize(Prefs.AppFontSize);
		qApp->setFont(apf,true);
		PolyC = dia->T6_Ecken->value();
		PolyF = dia->PFactor;
		PolyR = dia->T6_Faktor2->value();
		PolyS = dia->T6_Konvex->isChecked();
		PolyFd = dia->T6_Slider1->value();
		Prefs.PolyC = PolyC;
		Prefs.PolyF = PolyF;
		Prefs.PolyFd = PolyFd;
		Prefs.PolyS = PolyS;
		Prefs.PolyR = PolyR;
		Prefs.PageFormat = dia->GZComboF->currentItem();
		Prefs.Ausrichtung = dia->GZComboO->currentItem();
		Prefs.PageBreite = dia->Pagebr;
		Prefs.PageHoehe = dia->Pageho;
		Prefs.RandOben = dia->RandT;
		Prefs.RandUnten = dia->RandB;
		Prefs.RandLinks = dia->RandL;
		Prefs.RandRechts = dia->RandR;
		Prefs.DoppelSeiten = dia->Doppelseiten->isChecked();
		Prefs.ErsteLinks = dia->Linkszuerst->isChecked();
		Prefs.PDFTransparency = dia->UsePDFTrans->isChecked();
		Prefs.gimp_exe = dia->GimpName->text();
		Prefs.gs_antiGraph = dia->GSantiGraph->isChecked();
		Prefs.gs_antiText = dia->GSantiText->isChecked();
		Prefs.gs_exe = dia->GSName->text();
		Prefs.ClipMargin = dia->ClipMarg->isChecked();
		Prefs.GCRMode = dia->DoGCR->isChecked();
		Prefs.Before = dia->RadioButton6->isChecked();
		Prefs.PagesSbS = dia->SidebySide->isChecked();
		Prefs.RandFarbig = dia->RandFarb->isChecked();
		if (Prefs.DisScale != dia->DisScale)
		{
			Prefs.DisScale = dia->DisScale;
			zChange = true;
		}
		Mpal->Cpal->UseTrans(Prefs.PDFTransparency);
		Prefs.BaseShown = dia->RadioButton8->isChecked();
		if (Prefs.BaseShown)
			viewMenu->changeItem(Base, tr("Hide Baseline Grid"));
		else
			viewMenu->changeItem(Base, tr("Show Baseline Grid"));
		if (HaveDoc)
		{
			slotChangeUnit(dia->UnitCombo->currentItem(), false);
			if (zChange)
				slotZoomAbs(doc->Scale*Prefs.DisScale);
			doc->GrabRad = dia->SpinBox3_2->value();
			doc->GuideRad = dia->SpinBox2g->value() / UmReFaktor;
			doc->Dfont = dia->FontComb->currentText();
			doc->Dsize = dia->SizeCombo->currentText().left(2).toInt() * 10;
			doc->minorGrid = dia->SpinBox1->value() / UmReFaktor;
			doc->majorGrid = dia->SpinBox2->value() / UmReFaktor;
			doc->minorColor = dia->Cmin;
			doc->majorColor = dia->Cmax;
			doc->papColor = dia->Cpaper;
			doc->margColor = dia->Crand;
			doc->guideColor = dia->Cgui;
			doc->baseColor = dia->Cbase;
			doc->VHoch = dia->VHochW->value();
			doc->VHochSc = dia->VHochWSc->value();
			doc->VTief = dia->VTiefW->value();
			doc->VTiefSc = dia->VTiefWSc->value();
			doc->VKapit = dia->SmallCaps->value();
			doc->Dpen = dia->Foreground->currentText();
			if (doc->Dpen == tr("None"))
				doc->Dpen = "None";
			doc->DpenText = dia->ForegroundT->currentText();
			if (doc->DpenText == tr("None"))
				doc->DpenText = "None";
			doc->Dbrush = dia->Background->currentText();
			if (doc->Dbrush == tr("None"))
				doc->Dbrush = "None";
			doc->Dshade = dia->Shade->value();
			doc->Dshade2 = dia->Shade2->value();
			switch (dia->Linestyle->currentItem())
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
			doc->Dwidth = dia->LineW->value();
			doc->DpenLine = dia->Foreground2->currentText();
			if (doc->DpenLine == tr("None"))
				doc->DpenLine = "None";
			doc->DshadeLine = dia->Shade22->value();
			doc->DCols = dia->TextColVal->value();
			doc->DGap = dia->TextGapVal->value() / UmReFaktor;
			switch (dia->Linestyle2->currentItem())
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
			doc->DwidthLine = dia->LineW2->value();
			doc->MagMin = dia->MinMag->value();
			doc->MagMax = dia->MaxMag->value();
			doc->MagStep = dia->StepMag->value();
			doc->DbrushPict = dia->BackgroundP->currentText();
			if (doc->DbrushPict == tr("None"))
				doc->DbrushPict = "None";
			doc->ShadePict = dia->ShadeP->value();
			doc->ScaleX = static_cast<double>(dia->XScale->value()) / 100.0;
			doc->ScaleY = static_cast<double>(dia->YScale->value()) / 100.0;
			doc->ScaleType = dia->FreeScale->isChecked();
			doc->AspectRatio = dia->Aspect->isChecked();
			doc->AutoLine = dia->AutoLineV->value();
			doc->AutoSave = dia->ASon->isChecked();
			doc->BaseGrid = dia->BaseGrid->value();
			doc->BaseOffs = dia->BaseOffs->value();
			if (doc->AutoSave)
			{
				doc->ASaveTimer->stop();
				doc->ASaveTimer->start(dia->ASTime->value() * 60 * 1000);
			}
			view->reformPages();
			view->DrawNew();
		}
		else
		{
			Prefs.Einheit = dia->UnitCombo->currentItem();
			switch (Prefs.Einheit)
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
			Prefs.GrabRad = dia->SpinBox3_2->value();
			Prefs.GuideRad = dia->SpinBox2g->value() / UmReFaktor;
			Prefs.DefFont = dia->FontComb->currentText();
			Prefs.DefSize = dia->SizeCombo->currentText().left(2).toInt() * 10;
			Prefs.DminGrid = dia->SpinBox1->value() / UmReFaktor;
			Prefs.DmajGrid = dia->SpinBox2->value() / UmReFaktor;
			Prefs.DminColor = dia->Cmin;
			Prefs.DmajColor = dia->Cmax;
			Prefs.DpapColor = dia->Cpaper;
			Prefs.DmargColor = dia->Crand;
			Prefs.guideColor = dia->Cgui;
			Prefs.baseColor = dia->Cbase;
			Prefs.DVHoch = dia->VHochW->value();
			Prefs.DVHochSc = dia->VHochWSc->value();
			Prefs.DVTief = dia->VTiefW->value();
			Prefs.DVTiefSc = dia->VTiefWSc->value();
			Prefs.DVKapit = dia->SmallCaps->value();
			Prefs.Dpen = dia->Foreground->currentText();
			if (Prefs.Dpen == tr("None"))
				Prefs.Dpen = "None";
			Prefs.DpenText = dia->ForegroundT->currentText();
			if (Prefs.DpenText == tr("None"))
				Prefs.DpenText = "None";
			Prefs.DCols = dia->TextColVal->value();
			Prefs.DGap = dia->TextGapVal->value() / UmReFaktor;
			Prefs.Dbrush = dia->Background->currentText();
			if (Prefs.Dbrush == tr("None"))
				Prefs.Dbrush = "None";
			Prefs.Dshade = dia->Shade->value();
			Prefs.Dshade2 = dia->Shade2->value();
			switch (dia->Linestyle->currentItem())
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
			Prefs.Dwidth = dia->LineW->value();
			Prefs.DpenLine = dia->Foreground2->currentText();
			if (Prefs.DpenLine == tr("None"))
				Prefs.DpenLine = "None";
			Prefs.DshadeLine = dia->Shade22->value();
			switch (dia->Linestyle2->currentItem())
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
			Prefs.DwidthLine = dia->LineW2->value();
			Prefs.MagMin = dia->MinMag->value();
			Prefs.MagMax = dia->MaxMag->value();
			Prefs.MagStep = dia->StepMag->value();
			Prefs.DbrushPict = dia->BackgroundP->currentText();
			if (Prefs.DbrushPict == tr("None"))
				Prefs.DbrushPict = "None";
			Prefs.ShadePict = dia->ShadeP->value();
			Prefs.ScaleX = static_cast<double>(dia->XScale->value()) / 100.0;
			Prefs.ScaleY = static_cast<double>(dia->YScale->value()) / 100.0;
			Prefs.ScaleType = dia->FreeScale->isChecked();
			Prefs.AspectRatio = dia->Aspect->isChecked();
			Prefs.AutoLine = dia->AutoLineV->value();
			Prefs.AutoSave = dia->ASon->isChecked();
			Prefs.AutoSaveTime = dia->ASTime->value() * 60 * 1000;
			Prefs.BaseGrid = dia->BaseGrid->value();
			Prefs.BaseOffs = dia->BaseOffs->value();
		}
		SavePrefs();
		QWidgetList windows = wsp->windowList();
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
		{
			QWidget* w = wsp->windowList().at( i );
			ScribusWin* swin = (ScribusWin*)w;
			swin->doc->Before = Prefs.Before;
			swin->doc->PagesSbS = Prefs.PagesSbS;
			swin->doc->RandFarbig = Prefs.RandFarbig;
		}
	}
	delete dia;
	dlclose(mo);
}

void ScribusApp::SavePrefs()
{
	Prefs.MainX = abs(pos().x());
	Prefs.MainY = abs(pos().y());
	Prefs.MainW = size().width();
	Prefs.MainH = size().height();
	Prefs.Werkv = WerkTools->isVisible();
	Prefs.WerkvP = WerkToolsP->isVisible();
	Prefs.Mpalv = Mpal->isVisible();
	Prefs.Tpalv = Tpal->isVisible();
	Prefs.SCpalv = ScBook->isVisible();
	Prefs.Lpalv = Lpal->isVisible();
	Prefs.Sepalv = Sepal->isVisible();
	Prefs.Bopalv = BookPal->isVisible();
	if ((Prefs.Npalx > QApplication::desktop()->width()-100) || (Prefs.Npalx < 0))
		Prefs.Npalx = 0;
	if ((Prefs.Npaly > QApplication::desktop()->height()-100) || (Prefs.Npaly < 0))
		Prefs.Npaly = 0;
	if (MaPal->isVisible())
	{
		Prefs.Mapalx = abs(MaPal->pos().x());
		Prefs.Mapaly = abs(MaPal->pos().y());
	}
	if (Mpal->isVisible())
	{
		Prefs.Mpalx = abs(Mpal->pos().x());
		Prefs.Mpaly = abs(Mpal->pos().y());
	}
	if (Tpal->isVisible())
	{
		Prefs.Tpalx = abs(Tpal->pos().x());
		Prefs.Tpaly = abs(Tpal->pos().y());
	}
	if (ScBook->isVisible())
	{
		Prefs.SCpalx = abs(ScBook->pos().x());
		Prefs.SCpaly = abs(ScBook->pos().y());
		Prefs.SCpalw = abs(ScBook->size().width());
		Prefs.SCpalh = abs(ScBook->size().height());
	}
	if (Sepal->isVisible())
	{
		Prefs.Sepalx = abs(Sepal->pos().x());
		Prefs.Sepaly = abs(Sepal->pos().y());
	}
	if (BookPal->isVisible())
	{
		Prefs.Bopalx = abs(BookPal->pos().x());
		Prefs.Bopaly = abs(BookPal->pos().y());
	}
	if (Lpal->isVisible())
	{
		Prefs.Lpalx = abs(Lpal->pos().x());
		Prefs.Lpaly = abs(Lpal->pos().y());
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
	ss->WritePref(&Prefs, PrefsPfad+"/scribus.rc");
	delete ss;
}

void ScribusApp::ReadPrefs()
{
	ScriXmlDoc *ss = new ScriXmlDoc();
	bool erg = ss->ReadPref(&Prefs, PrefsPfad+"/scribus.rc");
	delete ss;
	if (!erg)
		return;
	PDef.Pname = Prefs.PrinterName;
	PDef.Dname = Prefs.PrinterFile;
	PDef.Command = Prefs.PrinterCommand;
	PolyC = Prefs.PolyC;
	PolyF = Prefs.PolyF;
	PolyFd = Prefs.PolyFd;
	PolyS = Prefs.PolyS;
	PolyR = Prefs.PolyR;
	recentMenu->clear();
	uint max = QMIN(Prefs.RecentDCount, Prefs.RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		QFileInfo fd(QString::fromUtf8(Prefs.RecentDocs[m]));
		if (fd.exists())
		{
			RecentDocs.append(QString::fromUtf8(Prefs.RecentDocs[m]));
			recentMenu->insertItem(QString::fromUtf8(Prefs.RecentDocs[m]));
		}
	}
	MaPal->move(Prefs.Mapalx, Prefs.Mapaly);
	Mpal->move(Prefs.Mpalx, Prefs.Mpaly);
	Tpal->move(Prefs.Tpalx, Prefs.Tpaly);
	Lpal->move(Prefs.Lpalx, Prefs.Lpaly);
	Sepal->move(Prefs.Sepalx, Prefs.Sepaly);
	BookPal->move(Prefs.Bopalx, Prefs.Bopaly);
	ScBook->move(Prefs.SCpalx, Prefs.SCpaly);
	ScBook->resize(Prefs.SCpalw, Prefs.SCpalh);
	Npal->move(Prefs.Npalx, Prefs.Npaly);
	move(Prefs.MainX, Prefs.MainY);
	resize(Prefs.MainW, Prefs.MainH);
	viewMenu->setItemChecked(Markers, Prefs.MarginsShown);
	viewMenu->setItemChecked(FrameDr, Prefs.FramesShown);
	viewMenu->setItemChecked(Ras, Prefs.GridShown);
	viewMenu->setItemChecked(Guide, Prefs.GuidesShown);
	viewMenu->setItemChecked(Base, Prefs.BaseShown);
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
	setTools(Prefs.Werkv);
	setPDFTools(Prefs.WerkvP);
	setMapal(Prefs.Mapalv);
	setMpal(Prefs.Mpalv);
	setTpal(Prefs.Tpalv);
	setBpal(Prefs.SCpalv);
	setLpal(Prefs.Lpalv);
	setSepal(Prefs.Sepalv);
	setBookpal(Prefs.Bopalv);
	setActiveWindow();
	raise();
}

PSLib* ScribusApp::getPSDriver(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, CListe DocColors, bool pdf)
{
	const char *error;
	typedef PSLib* (*sdem)(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, CListe DocColors, bool pdf);
	sdem demo;
	QString pfad = PREL;
#if defined(__hpux)
	pfad += "/lib/scribus/libs/libpostscript.sl";
#else
	pfad += "/lib/scribus/libs/libpostscript.so";
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
	pageNs.push_back(doc->ActPage->PageNr+1);
	ReOrderText(doc, view);
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QMap<QString,QFont> ReallyUsed;
	ReallyUsed.clear();
	GetUsedFonts(&ReallyUsed);
	PSLib *dd = getPSDriver(false, Prefs.AvailFonts, ReallyUsed, doc->PageColors, false);
	if (dd != NULL)
	{
		if (dd->PS_set_file(fn))
			view->CreatePS(dd, pageNs, false, tr("All"), true, false, false, false, Prefs.GCRMode);
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
	QString fn = CFileDialog( tr("Save as"), tr("EPS-Files (*.eps);;All Files (*)"), "", false, false);
	if (!fn.isEmpty())
	{
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
	QString pfad = PREL;
#if defined(__hpux)
	pfad += "/lib/scribus/libs/libpdf.sl";
#else
	pfad += "/lib/scribus/libs/libpdf.so";
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
	if (BookPal->BView->childCount() == 0)
		doc->PDF_Optionen.Bookmarks = false;
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
		doc->PDF_Optionen.RecalcPic = dia->DSColor->isChecked();
		doc->PDF_Optionen.PicRes = dia->ValC->value();
		doc->PDF_Optionen.Bookmarks = dia->CheckBM->isChecked();
		doc->PDF_Optionen.Binding = dia->ComboBind->currentItem();
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
	view->Pages.at(s)->Items.at(e)->BMnr = n;
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

void ScribusApp::slotElemRead(QString Name, int x, int y, bool art, bool loca, ScribusDoc* docc)
{
	if (doc == docc)
		NoFrameEdit();
	ScriXmlDoc *ss = new ScriXmlDoc();
	if(ss->ReadElem(Name, Prefs.AvailFonts, docc, x, y, art, loca, Prefs.GFontSub, &Prefs))
	{
		docc->ActPage->update();
		docc->UnDoValid = false;
		if (doc == docc)
		{
			doc->OpenNodes = Tpal->buildReopenVals();
			BuildFontMenu();
			Mpal->Cpal->SetColors(docc->PageColors);
			Mpal->updateCList();
			Mpal->Spal->updateFList();
			Mpal->SetLineFormats(docc);
			Tpal->BuildTree(view);
			Tpal->reopenTree(doc->OpenNodes);
			slotDocCh();
			CanUndo();
		}
	}
	delete ss;
}

void ScribusApp::slotChangeUnit(int art, bool draw)
{
	doc->Einheit = art;
	switch (art)
	{
	case 0:
		UmReFaktor = 1.0;
		view->UN->setText( tr("pt"));
		break;
	case 1:
		UmReFaktor = 0.3527777;
		view->UN->setText( tr("mm"));
		break;
	case 2:
		UmReFaktor = 1.0 / 72.0;
		view->UN->setText( tr("in"));
		break;
	case 3:
		UmReFaktor = 1.0 / 12.0;
		view->UN->setText( tr("p"));
		break;
	}
	Mpal->UnitChange();
	if (draw)
		view->DrawNew();
}

void ScribusApp::ManageJava()
{
	JavaDocs *dia = new JavaDocs(this, doc, view);
	dia->exec();
	delete dia;
}

void ScribusApp::ManageTemp(QString temp)
{
	MusterSeiten *dia = new MusterSeiten(this, doc, view, temp);
	connect(dia, SIGNAL(CreateNew(int)), this, SLOT(slotNewPageT(int)));
	connect(dia, SIGNAL(LoadPage(QString, int, bool)), this, SLOT(LadeSeite(QString, int, bool)));
	connect(dia, SIGNAL(Fertig()), this, SLOT(ManTempEnd()));
	for (uint a=0; a<5; ++a)
	{
		pageMenu->setItemEnabled(pageMenu->idAt(a), 0);
	}
	editMenu->setItemEnabled(tman, 0);
	ActWin->MenuStat[0] = DatSav->isEnabled();
	ActWin->MenuStat[1] = fileMenu->isItemEnabled(fid52);
	ActWin->MenuStat[2] = fileMenu->isItemEnabled(fid4);
	ActWin->MenuStat[3] = fileMenu->isItemEnabled(fid5);
	DatNeu->setEnabled(false);
	DatOpe->setEnabled(false);
	DatClo->setEnabled(false);
	fileMenu->setItemEnabled(fid12, 0);
	fileMenu->setItemEnabled(fid13, 0);
	fileMenu->setItemEnabled(fid14, 0);
	fileMenu->setItemEnabled(fid1, 0);
	fileMenu->setItemEnabled(fid52, 0);
	fileMenu->setItemEnabled(fid7, 0);
	fileMenu->setItemEnabled(fid9, 0);
	doc->TemplateMode = true;
	Sepal->DisablePal();
	doc->UnDoValid = false;
	CanUndo();
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
	fileMenu->setItemEnabled(fid12, 1);
	fileMenu->setItemEnabled(fid13, 1);
	fileMenu->setItemEnabled(fid14, 1);
	fileMenu->setItemEnabled(fid1, 1);
	fileMenu->setItemEnabled(fid52, 1);
	fileMenu->setItemEnabled(fid4, doc->isModified());
	fileMenu->setItemEnabled(fid7, 1);
	fileMenu->setItemEnabled(fid9, 1);
	int setter = view->Pages.count() > 1 ? 1 : 0;
	pageMenu->setItemEnabled(pgmd, setter);
	pageMenu->setItemEnabled(pgmv, setter);
	for (uint c=0; c<view->Pages.count(); ++c)
	{
		Apply_Temp(view->Pages.at(c)->MPageNam, c, false);
	}
	doc->TemplateMode = false;
	Sepal->EnablePal();
	Sepal->RebuildTemp();
	ActWin->muster = NULL;
	view->DrawNew();
	doc->UnDoValid = false;
	CanUndo();
	Sepal->Rebuild();
	Tpal->BuildTree(view);
	Tpal->reopenTree(doc->OpenNodes);
	slotDocCh();
}

void ScribusApp::ApplyTemp()
{
	QString mna;
	ApplyT *dia = new ApplyT(this, view, doc->ActPage->MPageNam);
	if (dia->exec())
	{
		mna = dia->Templ->currentText();
		if (dia->SinglePage->isChecked())
			Apply_Temp(mna, doc->ActPage->PageNr, false);
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
	doc->UnDoValid = false;
	CanUndo();
	Sepal->Rebuild();
	delete dia;
}

void ScribusApp::Apply_Temp(QString in, int Snr, bool reb)
{
	QString mna = in;
	if (mna == tr("Normal"))
		mna = "Normal";
	Page* Ap = view->Pages.at(Snr);
	Ap->MPageNam = mna;
	Page* Mp = view->MasterPages.at(view->MasterNames[mna]);
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
		doc->UnDoValid = false;
		CanUndo();
		Sepal->Rebuild();
	}
}

void ScribusApp::GroupObj()
{
	PageItem* b;
	PageItem* bb;
	double x, y, w, h;
	for (uint a=0; a<doc->ActPage->SelItem.count(); ++a)
	{
		b = doc->ActPage->SelItem.at(a);
		if (b->Locked)
		{
			int t = QMessageBox::warning(this, tr("Warning"),
			                             tr("Some Objects are locked."),
			                             tr("Cancel"),
			                             tr("Lock all"),
			                             tr("Unlock all"), 0, 0);
			if (t != 0)
			{
				for (uint c=0; c<doc->ActPage->SelItem.count(); ++c)
				{
					bb = doc->ActPage->SelItem.at(c);
					if (t == 1)
					{
						bb->Locked = true;
						ObjMenu->changeItem(LockOb, tr("Unlock"));
					}
					else
					{
						bb->Locked = false;
						ObjMenu->changeItem(LockOb, tr("Lock"));
					}
				}
			}
		}
	}
	for (uint a=0; a<doc->ActPage->SelItem.count(); ++a)
	{
		b = doc->ActPage->SelItem.at(a);
		b->Groups.push(doc->GroupCounter);
	}
	doc->GroupCounter++;
	doc->ActPage->getGroupRect(&x, &y, &w, &h);
	doc->ActPage->repaint(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
	ObjMenu->setItemEnabled(Gr, false);
	ObjMenu->setItemEnabled(UnGr, true);
}

void ScribusApp::UnGroupObj()
{
	PageItem* b;
	for (uint a=0; a<doc->ActPage->SelItem.count(); ++a)
	{
		b = doc->ActPage->SelItem.at(a);
		b->Groups.pop();
		b->isTableItem = false;
		b->LeftLink = 0;
		b->RightLink = 0;
		b->TopLink = 0;
		b->BottomLink = 0;
	}
	slotDocCh();
	doc->ActPage->Deselect(true);
	doc->UnDoValid = false;
	CanUndo();
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

QString ScribusApp::CFileDialog(QString caption, QString filter, QString defNa, bool Pre, bool mod, bool comp, bool cod, bool onlyDirs, bool *docom, bool *doFont)
{
	QString retval = "";
	CustomFDialog *dia = new CustomFDialog(this, caption, filter, Pre, mod, comp, cod, onlyDirs);
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
	int a = extraMenu->indexOf(id);
	if (a > 2)
		CallDLL(extraMenu->text(id));
}

void ScribusApp::RunImportPlug(int id)
{
	int a = importMenu->indexOf(id);
	if (a > 2)
	{
		if (HaveDoc)
			doc->OpenNodes = Tpal->buildReopenVals();
		CallDLL(importMenu->text(id));
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
	int a = exportMenu->indexOf(id);
	if (a > 2)
		CallDLL(exportMenu->text(id));
}

void ScribusApp::RunHelpPlug(int id)
{
	int a = helpMenu->indexOf(id);
	if (a > 3)
		CallDLL(helpMenu->text(id));
}

void ScribusApp::FinalizePlugs()
{
	const char *error;
	QMap<QString, PlugData>::Iterator it;
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

void ScribusApp::InitPlugs(SplashScreen *spl)
{
	QString pfad = PREL;
	QString nam = "";
	int ty = 0;
	struct PlugData pda;
	pfad += "/lib/scribus/plugins/";
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
			pda.Typ = 0;
			if (DLLName(d[dc], &nam, &ty, &pda.Zeiger))
			{
				if (ty == 1)
					extraMenu->insertItem(nam);
				if (ty == 2)
					importMenu->insertItem(nam);
				if (ty == 3)
					exportMenu->insertItem(nam);
				if (ty == 4)
					helpMenu->insertItem(nam);
				pda.Datei = d[dc];
				pda.Typ = ty;
				PluginMap.insert(nam, pda);
				spl->setStatus( tr("Loading:")+" "+nam);
			}
		}
		connect(extraMenu, SIGNAL(activated(int)), this, SLOT(RunPlug(int)));
		connect(importMenu, SIGNAL(activated(int)), this, SLOT(RunImportPlug(int)));
		connect(exportMenu, SIGNAL(activated(int)), this, SLOT(RunExportPlug(int)));
		connect(helpMenu, SIGNAL(activated(int)), this, SLOT(RunHelpPlug(int)));
	}
}

void ScribusApp::CallDLL(QString name)
{
	void *mo;
	const char *error;
	struct PlugData pda;
	pda = PluginMap[name];
	typedef void (*sdem)(QWidget *d, ScribusApp *plug);
	sdem demo;
	QString pfad = PREL;
	if (pda.Typ < 4)
	{
		pfad += "/lib/scribus/plugins/" + pda.Datei;
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
		doc->ActPage->update();
}

bool ScribusApp::DLLName(QString name, QString *PName, int *typ, void **Zeig)
{
	void *mo;
	const char *error;
	typedef QString (*sdem0)();
	typedef int (*sdem1)();
	typedef void (*sdem2)(QWidget *d, ScribusApp *plug);
	sdem0 demo;
	sdem1 demo1;
	sdem2 demo2;
	QString pfad = PREL;
	pfad += "/lib/scribus/plugins/" + name;
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
	QString pfad = PREL;
	pfad += "/lib/scribus/profiles/";
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

void ScribusApp::SetCMSPrefs()
{
	struct CMSset *CM;
	if (CMSavail)
	{
		if (HaveDoc)
			CM = &doc->CMSSettings;
		else
			CM = &Prefs.DCMSset;
		CMSPrefs *dia = new CMSPrefs(this, CM, &InputProfiles, &PrinterProfiles, &MonitorProfiles);
		if(dia->exec())
		{
			int cc = Prefs.DColors.count();
			FMess->setText( tr("Adjusting Colors"));
			FProg->reset();
			if (HaveDoc)
			{
				if (dia->Changed)
				{
					cc = doc->PageColors.count() + view->CountElements();
					FProg->setTotalSteps(cc);
					slotDocCh();
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
					view->DrawNew();
					Mpal->ShowCMS();
					FProg->setProgress(cc);
					qApp->setOverrideCursor(QCursor(arrowCursor), true);
				}
			}
			FMess->setText("");
			FProg->reset();
		}
		delete dia;
	}
}

void ScribusApp::RecalcColors(QProgressBar *dia)
{
	CListe::Iterator it;
	if (HaveDoc)
	{
		if (doc->TemplateMode)
			view->MasterPages = view->Pages;
		else
			view->DocPages = view->Pages;
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
		for (uint b=0; b<view->DocPages.count(); ++b)
		{
			for (uint c=0; c<view->DocPages.at(b)->Items.count(); ++c)
			{
				PageItem *ite = view->DocPages.at(b)->Items.at(c);
				QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
				for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
				{
					QColor tmpc = doc->PageColors[cstops.at(cst)->name].getRGBColor();
					ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
					cstops.at(cst)->color = tmpc;
				}
			}
		}
		for (uint b=0; b<view->MasterPages.count(); ++b)
		{
			for (uint c=0; c<view->MasterPages.at(b)->Items.count(); ++c)
			{
				PageItem *ite = view->MasterPages.at(b)->Items.at(c);
				QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
				for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
				{
					QColor tmpc = doc->PageColors[cstops.at(cst)->name].getRGBColor();
					ite->SetFarbe(&tmpc, cstops.at(cst)->name, cstops.at(cst)->shade);
					cstops.at(cst)->color = tmpc;
				}
			}
		}
		Mpal->Cpal->SetColors(doc->PageColors);
		Mpal->updateCList();
	}
}

void ScribusApp::ModifyAnnot()
{
	PageItem *b;
	if (doc->ActPage->SelItem.count() != 0)
	{
		b = doc->ActPage->SelItem.at(0);
		if ((b->AnType == 0) || (b->AnType == 1) || (b->AnType > 9))
		{
			int AnType = b->AnType;
			int AnActType = b->AnActType;
			QString AnAction = b->AnAction;
			QString An_Extern = b->An_Extern;
			Annota *dia = new Annota(this, b, doc->PageC, static_cast<int>(doc->PageB), static_cast<int>(doc->PageH), doc->PageColors, view);
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
	toolMenu->setAccel(Prefs.KeyActions[65].KeyID, Prefs.KeyActions[65].MenuID);
	ObjMenu->setAccel(Prefs.KeyActions[66].KeyID, Prefs.KeyActions[66].MenuID);
	ObjMenu->setAccel(Prefs.KeyActions[67].KeyID, Prefs.KeyActions[67].MenuID);
	fileMenu->setAccel(Prefs.KeyActions[18].KeyID, Prefs.KeyActions[18].MenuID);
}

void ScribusApp::PutScrap(QString t)
{
	ScBook->ObjFromMenu(t);
}

void ScribusApp::UniteOb()
{
	doc->ActPage->UniteObj();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::SplitUniteOb()
{
	doc->ActPage->SplitObj();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::TraceText()
{
	NoFrameEdit();
	doc->ActPage->TextToPath();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::Pfadtext()
{
	doc->ActPage->ToPathText();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::noPfadtext()
{
	doc->ActPage->FromPathText();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::changeLayer(int l)
{
	doc->ActPage->Deselect(true);
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
		view->MasterPages = view->Pages;
	else
		view->DocPages = view->Pages;
	for (uint a = 0; a < view->MasterPages.count(); ++a)
	{
		for (uint b = 0; b < view->MasterPages.at(a)->Items.count(); ++b)
		{
			view->MasterPages.at(a)->SelItem.clear();
			if (view->MasterPages.at(a)->Items.at(b)->LayerNr == l)
			{
				if (dl)
					view->MasterPages.at(a)->SelItem.append(view->MasterPages.at(a)->Items.at(b));
				else
					view->MasterPages.at(a)->Items.at(b)->LayerNr = 0;
			}
			if (view->MasterPages.at(a)->SelItem.count() != 0)
				view->MasterPages.at(a)->DeleteItem();
		}
	}
	for (uint a = 0; a < view->DocPages.count(); ++a)
	{
		view->DocPages.at(a)->SelItem.clear();
		for (uint b = 0; b < view->DocPages.at(a)->Items.count(); ++b)
		{
			if (view->DocPages.at(a)->Items.at(b)->LayerNr == l)
			{
				if (dl)
					view->DocPages.at(a)->SelItem.append(view->DocPages.at(a)->Items.at(b));
				else
					view->DocPages.at(a)->Items.at(b)->LayerNr = 0;
			}
		}
		if (view->DocPages.at(a)->SelItem.count() != 0)
			view->DocPages.at(a)->DeleteItem();
	}
	view->LaMenu();
}

void ScribusApp::UnDoAction()
{
	PageItem* b;
	uint a;
	bool mp = false;
	if (doc->UnDoValid)
	{
		doc->ActPage->Deselect(true);
		b = doc->UnData.Item;
		b->Select = false;
		view->Pages.at(doc->UnData.PageNr)->Deselect(true);
		switch (doc->UnData.UnCode)
		{
		case 0:
			b->NextBox = 0;
			b->BackBox = 0;
			b->isAutoText = false;
			view->Pages.at(doc->UnData.PageNr)->Items.insert(b->ItemNr, b);
			Tpal->slotAddElement(doc->UnData.PageNr, b->ItemNr);
			for (a = 0; a < view->Pages.at(doc->UnData.PageNr)->Items.count(); ++a)
			{
				view->Pages.at(doc->UnData.PageNr)->Items.at(a)->ItemNr = a;
			}
			Tpal->slotUpdateElement(doc->UnData.PageNr, b->ItemNr);
			break;
		case 1:
			b->Xpos = doc->UnData.Xpos;
			b->Ypos = doc->UnData.Ypos;
			Tpal->slotUpdateElement(doc->UnData.PageNr, b->ItemNr);
			break;
		case 2:
			b->Xpos = doc->UnData.Xpos;
			b->Ypos = doc->UnData.Ypos;
			b->Rot = doc->UnData.Rot;
			if (b->PType == 5)
				mp = true;
			view->Pages.at(doc->UnData.PageNr)->SizeItem(doc->UnData.Width, doc->UnData.Height, b->ItemNr, mp);
			Tpal->slotUpdateElement(doc->UnData.PageNr, b->ItemNr);
			break;
		case 3:
			b->Rot = doc->UnData.Rot;
			break;
		case 4:
			view->Pages.at(doc->UnData.PageNr)->Items.take(b->ItemNr);
			view->Pages.at(doc->UnData.PageNr)->Items.insert(doc->UnData.ItemNr, b);
			Tpal->slotMoveElement(doc->UnData.PageNr, b->ItemNr, doc->UnData.ItemNr);
			for (a = 0; a < view->Pages.at(doc->UnData.PageNr)->Items.count(); ++a)
			{
				view->Pages.at(doc->UnData.PageNr)->Items.at(a)->ItemNr = a;
			}
			break;
			Tpal->slotUpdateElement(doc->UnData.PageNr, b->ItemNr);
		}
		view->DrawNew();
		doc->UnDoValid = false;
		editMenu->setItemEnabled(edUndo, 0);
		slotDocCh();
	}
}

void ScribusApp::CanUndo()
{
	switch (doc->UnData.UnCode)
	{
	case 0:
		editMenu->changeItem(edUndo, tr("&Undo Delete Object"));
		break;
	case 1:
	case 4:
		editMenu->changeItem(edUndo, tr("&Undo Object Move"));
		break;
	case 2:
	case 3:
		editMenu->changeItem(edUndo, tr("&Undo Object Change"));
		break;
	}
	editMenu->setItemEnabled(edUndo, doc->UnDoValid ? 1 : 0);
}

void ScribusApp::InitHyphenator()
{
	QString pfad = PREL;
	pfad += "/lib/scribus/";
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
	QString datein = "";
	QString lang = QString(QTextCodec::locale()).left(2);
	LangTransl.clear();
	Prefs.Language = "English";
	pfad = PREL;
	pfad += "/lib/scribus/dicts/";
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

void ScribusApp::configHyphenator()
{
	HySettings *dia = new HySettings(this, &LangTransl);
	if (HaveDoc)
	{
		dia->Verbose->setChecked(doc->Trenner->Automatic);
		dia->Input->setChecked(doc->Trenner->AutoCheck);
		dia->Language->setCurrentText(LangTransl[doc->Trenner->Language]);
		dia->WordLen->setValue(doc->Trenner->MinWordLen);
		dia->MaxCount->setValue(doc->Trenner->HyCount);
	}
	else
	{
		dia->Verbose->setChecked(Prefs.Automatic);
		dia->Input->setChecked(Prefs.AutoCheck);
		dia->Language->setCurrentText(LangTransl[Prefs.Language]);
		dia->WordLen->setValue(Prefs.MinWordLen);
		dia->MaxCount->setValue(Prefs.HyCount);
	}
	if (dia->exec())
	{
		if (HaveDoc)
		{
			doc->Trenner->slotNewDict(dia->Language->currentText());
			doc->Trenner->slotNewSettings(dia->WordLen->value(), dia->Verbose->isChecked(), dia->Input->isChecked(),dia->MaxCount->value());
		}
		else
		{
			Prefs.MinWordLen = dia->WordLen->value();
			Prefs.Language = GetLang(dia->Language->currentText());
			Prefs.Automatic = dia->Verbose->isChecked();
			Prefs.AutoCheck = dia->Input->isChecked();
			Prefs.HyCount = dia->MaxCount->value();
		}
	}
	delete dia;
}

void ScribusApp::doHyphenate()
{
	PageItem *b;
	if (HaveDoc)
	{
		if (doc->ActPage->SelItem.count() != 0)
		{
			b = doc->ActPage->SelItem.at(0);
			if (doc->Trenner->Language != b->Language)
				doc->Trenner->slotNewDict(b->Language);
			doc->Trenner->slotHyphenate(b);
		}
	}
}

void ScribusApp::ToggleObjLock()
{
	if (HaveDoc)
	{
		if (doc->ActPage->SelItem.count() != 0)
		{
			PageItem* b = doc->ActPage->SelItem.at(0);
			doc->ActPage->ToggleLock();
			if (b->Locked)
				ObjMenu->changeItem(LockOb, tr("Unlock"));
			else
				ObjMenu->changeItem(LockOb, tr("Lock"));
		}
	}
}

void ScribusApp::ManageGuides()
{
	if (HaveDoc)
	{
		GuideManager *dia = new GuideManager(
		                        this,
		                        doc->ActPage->XGuides,
		                        doc->ActPage->YGuides,
		                        doc->PageB,
		                        doc->PageH,
		                        doc->GuideLock,
		                        doc->Einheit
		                    );
		if (dia->exec())
		{
			doc->ActPage->XGuides = dia->LocVer;
			doc->ActPage->YGuides = dia->LocHor;
			doc->GuideLock = dia->LocLocked;
			doc->ActPage->update();
			slotDocCh();
		}
		delete dia;
	}
}

void ScribusApp::SetTranspar(double t)
{
	PageItem *b;
	if (HaveDoc)
	{
		if (doc->ActPage->SelItem.count() != 0)
		{
			b = doc->ActPage->SelItem.at(0);
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
		if (doc->ActPage->SelItem.count() != 0)
		{
			b = doc->ActPage->SelItem.at(0);
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
		if (doc->ActPage->SelItem.count() != 0)
		{
			b = doc->ActPage->SelItem.at(0);
			b->InvPict = !b->InvPict;
			view->DrawNew();
			slotDocCh();
		}
	}
}

QString ScribusApp::Collect(bool compress, bool withFonts)
{
	if (doc->TemplateMode)
		view->MasterPages = view->Pages;
	else
		view->DocPages = view->Pages;
	QString retVal = "";
	QString CurDirP = QDir::currentDirPath();
	bool compressR = compress;
	bool withFontsR = withFonts;
	QString s = CFileDialog( tr("Choose a Directory"), "", "", false, false, false, false, true, &compressR, &withFontsR);
	if (s != "")
	{
		if(s.right(1) != "/")
			s += "/";
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
				for (uint a = 0; a < view->MasterPages.count(); ++a)
				{
					for (uint b = 0; b < view->MasterPages.at(a)->Items.count(); ++b)
					{
						PageItem* ite = view->MasterPages.at(a)->Items.at(b);
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
				}
				for (uint a = 0; a < view->DocPages.count(); ++a)
				{
					for (uint b = 0; b < view->DocPages.at(a)->Items.count(); ++b)
					{
						PageItem* ite = view->DocPages.at(a)->Items.at(b);
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
	Page* pg;
	PageItem* it;
	QMap<QString,QFont> Really;
	QMap<QString,QFont> DocF;
	DocF = doc->UsedFonts;
	if (!doc->TemplateMode)
		view->DocPages = view->Pages;
	for (uint c = 0; c < view->MasterPages.count(); ++c)
	{
		pg = view->MasterPages.at(c);
		for (uint d = 0; d < pg->Items.count(); ++d)
		{
			it = pg->Items.at(d);
			Really.insert(it->IFont, doc->UsedFonts[it->IFont]);
			if ((it->PType == 4) || (it->PType == 8))
			{
				for (uint e = 0; e < it->Ptext.count(); ++e)
				{
					Really.insert(it->Ptext.at(e)->cfont, doc->UsedFonts[it->Ptext.at(e)->cfont]);
				}
			}
		}
	}
	for (uint c = 0; c < view->DocPages.count(); ++c)
	{
		pg = view->DocPages.at(c);
		for (uint d = 0; d < pg->Items.count(); ++d)
		{
			it = pg->Items.at(d);
			Really.insert(it->IFont, doc->UsedFonts[it->IFont]);
			if ((it->PType == 4) || (it->PType == 8))
			{
				for (uint e = 0; e < it->Ptext.count(); ++e)
				{
					Really.insert(it->Ptext.at(e)->cfont, doc->UsedFonts[it->Ptext.at(e)->cfont]);
				}
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
	BuildFontMenu();
}

void ScribusApp::GetUsedFonts(QMap<QString,QFont> *Really)
{
	Page* pg;
	PageItem* it;
	FPointArray gly;
	QString chx;
	for (uint c = 0; c < view->MasterPages.count(); ++c)
	{
		pg = view->MasterPages.at(c);
		for (uint d = 0; d < pg->Items.count(); ++d)
		{
			it = pg->Items.at(d);
			if ((it->PType == 4) || (it->PType == 8))
			{
				for (uint e = 0; e < it->Ptext.count(); ++e)
				{
					Really->insert(it->Ptext.at(e)->cfont, doc->UsedFonts[it->Ptext.at(e)->cfont]);
					uint chr = it->Ptext.at(e)->ch[0].unicode();
					if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 9))
						continue;
					if (it->Ptext.at(e)->cstyle & 64)
					{
						chx = it->Ptext.at(e)->ch;
						if (chx.upper() != it->Ptext.at(e)->ch)
							chx = chx.upper();
						chr = chx[0].unicode();
					}
					if (chr == 30)
					{
						for (uint numco = 0x30; numco < 0x39; ++numco)
						{
							if ((*doc->AllFonts)[it->Ptext.at(e)->cfont]->CharWidth.contains(numco))
							{
								gly = (*doc->AllFonts)[it->Ptext.at(e)->cfont]->GlyphArray[numco].Outlines.copy();
								(*doc->AllFonts)[it->Ptext.at(e)->cfont]->RealGlyphs.insert(numco, gly);
							}
						}
						continue;
					}
					if ((*doc->AllFonts)[it->Ptext.at(e)->cfont]->CharWidth.contains(chr))
					{
						gly = (*doc->AllFonts)[it->Ptext.at(e)->cfont]->GlyphArray[chr].Outlines.copy();
						(*doc->AllFonts)[it->Ptext.at(e)->cfont]->RealGlyphs.insert(chr, gly);
					}
				}
			}
		}
	}
	for (uint c = 0; c < view->Pages.count(); ++c)
	{
		pg = view->Pages.at(c);
		for (uint d = 0; d < pg->Items.count(); ++d)
		{
			it = pg->Items.at(d);
			if ((it->PType == 4) || (it->PType == 8))
			{
				for (uint e = 0; e < it->Ptext.count(); ++e)
				{
					Really->insert(it->Ptext.at(e)->cfont, doc->UsedFonts[it->Ptext.at(e)->cfont]);
					uint chr = it->Ptext.at(e)->ch[0].unicode();
					if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 9))
						continue;
					if (it->Ptext.at(e)->cstyle & 64)
					{
						chx = it->Ptext.at(e)->ch;
						if (chx.upper() != it->Ptext.at(e)->ch)
							chx = chx.upper();
						chr = chx[0].unicode();
					}
					if ((*doc->AllFonts)[it->Ptext.at(e)->cfont]->CharWidth.contains(chr))
					{
						gly = (*doc->AllFonts)[it->Ptext.at(e)->cfont]->GlyphArray[chr].Outlines.copy();
						(*doc->AllFonts)[it->Ptext.at(e)->cfont]->RealGlyphs.insert(chr, gly);
					}
				}
			}
		}
	}
}

void ScribusApp::HaveRaster(bool art)
{
	if (doc->ActPage->SelItem.count() != 0)
	{
		PageItem *b = doc->ActPage->SelItem.at(0);
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
	if (doc->ActPage->SelItem.count() != 0)
	{
		PageItem *b = doc->ActPage->SelItem.at(0);
		StoryEditor* dia = new StoryEditor(this, doc, b);
		CurrStED = dia;
		connect(dia, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
		connect(dia, SIGNAL(EditSt()), this, SLOT(slotEditStyles()));
		if (dia->exec())
		{
			if (dia->TextChanged)
				dia->updateTextFrame();
		}
		BuildFontMenu();
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
				ss->WriteDoc(doc->DocName+".emergency", doc, view, 0);
				delete ss;
			}
			view->close();
			for (uint a = 0; a<view->Pages.count(); ++a)
			{
				delete view->Pages.at(a);
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
		if (doc->ActPage->SelItem.count() != 0)
		{
			PageItem *b = doc->ActPage->SelItem.at(0);
			TabManager *dia = new TabManager(this, doc->Einheit, b->TabValues, b->Width);
			if (dia->exec())
			{
				b->TabValues = dia->tmpTab;
				doc->ActPage->RefreshItem(b);
				slotDocCh();
			}
			delete dia;
		}
	}
}

void ScribusApp::SearchText()
{
	PageItem *b = doc->ActPage->SelItem.at(0);
	setAppMode(7);
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

/*!
 \fn void ScribusApp::DefKB()
 \author Franz Schmid
 \date
 \brief Preferences (General / Menus), Creates and opens KeyManager dialog for shortcut key preferences.
 \param None
 \retval None
 */
void ScribusApp::DefKB()
{
	KeyManager *dia = new KeyManager(this, Prefs.KeyActions);
	if (dia->exec())
	{
		Prefs.KeyActions = dia->KK;
		SetShortCut();
		SavePrefs();
	}
	delete dia;
}


void ScribusApp::slotTest()
{}

void ScribusApp::slotTest2()
{}
