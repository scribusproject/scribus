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
#include <iostream>
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
#include "config.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "hysettings.h"
#include "guidemanager.h"
#include "mergedoc.h"
extern QPixmap loadIcon(QString nam);

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
float PolyF;
bool PolyS;
float PolyR;
float UmReFaktor;
QString DocDir;

ScribusApp::ScribusApp()
{
  setCaption(tr("Scribus " VERSION));
  setIcon(loadIcon("AppIcon.xpm"));
  initMenuBar();
  initStatusBar();
  WerkTools2 = new QToolBar(tr("File"), this);
  DatNeu = new QToolButton(loadIcon("DateiNeu.xpm"), tr("Creates a new Document"), QString::null, this, SLOT(slotFileNew()), WerkTools2);
  DatOpe = new QToolButton(loadIcon("DateiOpen.xpm"), tr("Opens a Document"), QString::null, this, SLOT(slotDocOpen()), WerkTools2);
  DatSav = new QToolButton(loadIcon("DateiSave.xpm"), tr("Saves the current Document"), QString::null, this, SLOT(slotFileSave()), WerkTools2);
  DatClo = new QToolButton(loadIcon("DateiClos.xpm"), tr("Closes the current Document"), QString::null, this, SLOT(slotFileClose()), WerkTools2);
  DatPri = new QToolButton(loadIcon("DateiPrint.xpm"), tr("Prints the current Document"), QString::null, this, SLOT(slotFilePrint()), WerkTools2);
  DatPDF = new QToolButton(loadIcon("acrobat.png"), tr("Saves the current Document as PDF"), QString::null, this, SLOT(SaveAsPDF()), WerkTools2);
	DatSav->setEnabled(false);
	DatClo->setEnabled(false);
	DatPri->setEnabled(false);
	DatPDF->setEnabled(false);
	DatOpe->setPopup(recentMenu);
	WerkTools = new WerkToolB(this);
	setDockEnabled(WerkTools, DockLeft, false);
	setDockEnabled(WerkTools, DockRight, false);
	WerkTools->setEnabled(false);
	QString Pff = QString(getenv("HOME"))+"/.scribus";
	QFileInfo Pffi = QFileInfo(Pff);
	if (Pffi.exists())
		{
		if (Pffi.isDir())
			PrefsPfad = Pff;
		else
			PrefsPfad = QString(getenv("HOME"));
		}
	else
		{
  	QDir di = QDir();
		di.mkdir(Pff);
		PrefsPfad = Pff;
		QString OldPR = QString(getenv("HOME"))+"/.scribus.rc";
		QFileInfo OldPi = QFileInfo(OldPR);
		if (OldPi.exists())
			{
			system("mv " + OldPR + " " + Pff+"/scribus.rc");
			}
		QString OldPR2 = QString(getenv("HOME"))+"/.scribusfont.rc";
		QFileInfo OldPi2 = QFileInfo(OldPR2);
		if (OldPi2.exists())
			{
			system("mv " + OldPR2 + " " + Pff+"/scribusfont.rc");
			}
		QString OldPR3 = QString(getenv("HOME"))+"/.scribusscrap.scs";
		QFileInfo OldPi3 = QFileInfo(OldPR3);
		if (OldPi3.exists())
			{
			system("mv " + OldPR3 + " " + Pff+"/scrap.scs");
			}
		}
	/** Erstelle Fontliste */
  NoFonts = false;
	BuFromApp = false;
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
  	BuildFontMenu();
		SCFontsIterator it(Prefs.AvailFonts);
		Prefs.DefFont = it.currentKey();
  	Prefs.DefSize = 12;
  	Prefs.AppFontSize = 12;
  	/** Default Farbenliste */
  	Prefs.DColors.clear();
	  Prefs.DColors.insert("White", CMYKColor(0, 0, 0, 0));
  	Prefs.DColors.insert("Black", CMYKColor(0, 0, 0, 255));
		Prefs.DColors.insert("Blue", CMYKColor(255, 255, 0, 0));
		Prefs.DColors.insert("Cyan", CMYKColor(255, 0, 0, 0));
		Prefs.DColors.insert("Green", CMYKColor(255, 0, 255, 0));
		Prefs.DColors.insert("Red", CMYKColor(0, 255, 255, 0));
		Prefs.DColors.insert("Yellow", CMYKColor(0, 0, 255, 0));
		Prefs.DColors.insert("Magenta", CMYKColor(0, 255, 0, 0));
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
	  Prefs.Mpalv = false;
	  Prefs.Tpalv = false;
	  Prefs.SCpalv = false;
	  Prefs.Lpalv = false;
		Prefs.Bopalv = false;
	  Prefs.PSize = 40;
	  Prefs.SaveAtQ = true;
	  Prefs.ShFrames = true;
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
		Prefs.Language = "";
		Prefs.Automatic = true;
		Prefs.AutoCheck = false;
		Prefs.PDFTransparency = false;
		Prefs.AutoSave = false;
		Prefs.AutoSaveTime = 600000;
		Prefs.DisScale = 1.0;
		Prefs.DocDir = QString(getenv("HOME"));
	  PDFavailable = true;
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
		Tpal = new Tree(this, WStyle_Customize | WStyle_DialogBorder);
		Mpal = new Mpalette(this, FontMenu);
		Mpal->Cpal->SetColors(Prefs.DColors);
		Npal = new NodePalette(this);
		Lpal = new LayerPalette(this);
		ScBook = new Biblio(this, Prefs.AvailFonts, &Prefs);
		Sepal = new SeitenPal(this);
		BookPal = new BookPalette(this);
		CMSavail = false;
		Prefs.DCMSset.DefaultMonitorProfile = "";
		Prefs.DCMSset.DefaultPrinterProfile = "";
		Prefs.DCMSset.DefaultInputProfile = "";
		Prefs.DCMSset.DefaultInputProfile2 = "";
		Prefs.DCMSset.CMSinUse = false;
		Prefs.DCMSset.SoftProofOn = false;
		Prefs.DCMSset.GamutCheck = false;
		Prefs.DCMSset.DefaultIntentMonitor = 1;
		Prefs.DCMSset.DefaultIntentMonitor2 = 1;
		Prefs.DCMSset.DefaultIntentPrinter = 0;
		Prefs.GFontSub.clear();
		SetKeyEntry(56, tr("Smart Hyphen"), 0, ALT+Key_Minus);
		SetKeyEntry(57, tr("Align Left"), 0, CTRL+Key_L);
		SetKeyEntry(58, tr("Align Right"), 0, CTRL+Key_R);
		SetKeyEntry(59, tr("Align Center"), 0, CTRL+Key_E);
		SetKeyEntry(60, tr("Insert Pagenumber"), 0, ALT+Key_NumberSign);
		SetKeyEntry(61, tr("Attach Text to Path"), PfadT, 0);
		SetKeyEntry(62, tr("Show Layers"), viewLpal, 0);
		SetKeyEntry(63, tr("JavaScripts..."), jman, 0);
		SetKeyEntry(64, tr("Undo"), edUndo, CTRL+Key_Z);
		SetKeyEntry(65, tr("Show Page Palette"), viewSepal, 0);
		SetKeyEntry(66, tr("Lock/Unlock"), LockOb, CTRL+Key_H);
		GetCMSProfiles();
		InitHyphenator();
		ReadPrefs();
		Mpal->Cpal->UseTrans(Prefs.PDFTransparency);
		DocDir = Prefs.DocDir;
		SetShortCut();
		if (CMSavail)
			{
			hymen->insertItem(tr("Color Management..."), this , SLOT(SetCMSPrefs()));
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
		QString SCf = PrefsPfad+"/scrap.scs";
		QFileInfo SCfi = QFileInfo(SCf);
		if (SCfi.exists())
			{
			ScBook->BibWin->ReadContents(SCf);
			ScBook->ScFilename = SCf;
			}
		ScBook->AdjustMenu();
		HaveGS = system("gs -h > /dev/null 2>&1");
		InitPlugs();
		ClipB = QApplication::clipboard();
		connect(WerkTools, SIGNAL(NewMode(int)), this, SLOT(ModeFromTB(int)));
		connect(WerkTools, SIGNAL(Schliessen()), this, SLOT(ToggleTools()));
		connect(Mpal, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
		connect(Mpal, SIGNAL(NewAbStyle(int)), this, SLOT(setNewAbStyle(int)));
		connect(Mpal, SIGNAL(BackHome()), this, SLOT(Aktiv()));
		connect(Mpal, SIGNAL(Stellung(int)), this, SLOT(setItemHoch(int)));
		connect(Mpal, SIGNAL(Schliessen()), this, SLOT(ToggleMpal()));
		connect(Mpal, SIGNAL(EditCL()), this, SLOT(ToggleFrameEdit()));
		connect(Mpal->Cpal, SIGNAL(NewPen(QString)), this, SLOT(setPenFarbe(QString)));
		connect(Mpal->Cpal, SIGNAL(NewBrush(QString)), this, SLOT(setBrushFarbe(QString)));
		connect(Mpal->Cpal, SIGNAL(NewPenShade(int)), this, SLOT(setPenShade(int)));
		connect(Mpal->Cpal, SIGNAL(NewBrushShade(int)), this, SLOT(setBrushShade(int)));
		connect(Mpal->Cpal, SIGNAL(NewTrans(float)), this, SLOT(SetTranspar(float)));
		connect(Mpal->Cpal, SIGNAL(NewGradient(int, QString, int, QString, int)), this, SLOT(setGradFill(int, QString, int, QString, int)));
		connect(Mpal->Cpal, SIGNAL(QueryItem()), this, SLOT(GetBrushPen()));
		connect(Tpal, SIGNAL(Schliessen()), this, SLOT(ToggleTpal()));
		connect(Tpal, SIGNAL(SelectElement(int, int)), this, SLOT(SelectFromOutl(int, int)));
		connect(Tpal, SIGNAL(SelectSeite(int)), this, SLOT(SelectFromOutlS(int)));
		connect(Mpal->Spal, SIGNAL(NewStyle(int)), this, SLOT(setNewAbStyle(int)));
		connect(Mpal->Spal, SIGNAL(EditSt()), this, SLOT(slotEditStyles()));
		connect(Npal, SIGNAL(Schliessen()), this, SLOT(NoFrameEdit()));
		connect(Lpal, SIGNAL(LayerActivated(int)), this, SLOT(changeLayer(int)));
		connect(Lpal, SIGNAL(LayerRemoved(int)), this, SLOT(LayerRemove(int)));
		connect(Lpal, SIGNAL(LayerChanged()), this, SLOT(showLayer()));
		connect(Lpal, SIGNAL(Schliessen()), this, SLOT(ToggleLpal()));
		connect(Sepal, SIGNAL(Schliessen()), this, SLOT(ToggleSepal()));
		connect(ScBook, SIGNAL(Schliessen()), this, SLOT(ToggleBpal()));
		connect(Sepal, SIGNAL(EditTemp(QString)), this, SLOT(ManageTemp(QString)));
		connect(Sepal->PageView, SIGNAL(UseTemp(QString, int)), this, SLOT(Apply_Temp(QString, int)));
		connect(Sepal->PageView, SIGNAL(NewPage(int, QString)), this, SLOT(slotNewPageP(int, QString)));
		connect(Sepal->Trash, SIGNAL(DelPage(int)), this, SLOT(DeletePage2(int)));
		connect(Sepal, SIGNAL(GotoSeite(int)), this, SLOT(SelectFromOutlS(int)));
		connect(BookPal->BView, SIGNAL(MarkMoved()), this, SLOT(StoreBookmarks()));
		connect(BookPal->BView, SIGNAL(ChangeBMNr(int, int, int)), this, SLOT(ChBookmarks(int, int, int)));
		connect(BookPal, SIGNAL(Schliessen()), this, SLOT(ToggleBookpal()));
		connect(recentMenu, SIGNAL(activated(int)), this, SLOT(LoadRecent(int)));
		connect(ColorMenu, SIGNAL(activated(int)), this, SLOT(setItemFarbe(int)));
	  connect(ShadeMenu, SIGNAL(activated(int)), this, SLOT(setItemShade(int)));
	  connect(FontMenu, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
	  connect(SizeTMenu, SIGNAL(activated(int)), this, SLOT(setItemFSize(int)));
	  connect(TypeStyleMenu, SIGNAL(activated(int)), this, SLOT(setItemTypeStyle(int)));
	  connect(AliMenu, SIGNAL(activated(int)), this, SLOT(setItemTextAli(int)));
		connect(this, SIGNAL(TextIFont(QString)), this, SLOT(AdjustFontMenu(QString)));
		connect(this, SIGNAL(TextISize(int)), this, SLOT(setFSizeMenu(int)));
		connect(this, SIGNAL(TextISize(int)), Mpal, SLOT(setSize(int)));
		connect(this, SIGNAL(TextUSval(float)), Mpal, SLOT(setExtra(float)));
		connect(this, SIGNAL(TextStil(int)), Mpal, SLOT(setStil(int)));
		connect(this, SIGNAL(TextFarben(QString, QString, int, int)), Mpal->Cpal, SLOT(setActFarben(QString, QString, int, int)));
		connect(ClipB, SIGNAL(dataChanged()), this, SLOT(ClipChange()));
	}
}

void ScribusApp::initMenuBar()
{
	int a;
	int MenID;
	QFont tmp;
	recentMenu = new QPopupMenu();
	RecentDocs.clear();
	fileMenu=new QPopupMenu();
	fid12 = fileMenu->insertItem(loadIcon("DateiNeu16.png"), tr("New"), this, SLOT(slotFileNew()), CTRL+Key_N);
	fid13 = fileMenu->insertItem(loadIcon("DateiOpen16.png"), tr("Open..."), this, SLOT(slotDocOpen()), CTRL+Key_O);
	fid14 = fileMenu->insertItem(tr("Recent Documents"), recentMenu);
	SetKeyEntry(0, tr("New"), fid12, CTRL+Key_N);
	SetKeyEntry(1, tr("Open..."), fid13, CTRL+Key_O);
	fileMenu->insertSeparator();
	fid1 = fileMenu->insertItem(loadIcon("DateiClos16.png"), tr("Close"), this, SLOT(slotFileClose()), CTRL+Key_W);
	SetKeyEntry(2, tr("Close"), fid1, CTRL+Key_W);
	fileMenu->setItemEnabled(fid1, 0);
	fid4 = fileMenu->insertItem(loadIcon("DateiSave16.png"), tr("Save"), this, SLOT(slotFileSave()), CTRL+Key_S);
	SetKeyEntry(3, tr("Save"), fid4, CTRL+Key_S);
	fileMenu->setItemEnabled(fid4, 0);
	fid5 = fileMenu->insertItem(tr("Save as..."), this, SLOT(slotFileSaveAs()));
	SetKeyEntry(4, tr("Save as..."), fid5, 0);
	fileMenu->setItemEnabled(fid5, 0);
	fid51 = fileMenu->insertItem(tr("Collect for Output..."), this, SLOT(Collect()));
	fileMenu->setItemEnabled(fid51, 0);
	fileMenu->insertSeparator();
	importMenu = new QPopupMenu();
	fid2 = importMenu->insertItem(tr("Get Text/Picture..."), this, SLOT(slotFileOpen()));
	importMenu->setItemEnabled(fid2, 0);
	fid2a = importMenu->insertItem(tr("Insert Page..."), this, SLOT(slotDocMerge()));
	importMenu->setItemEnabled(fid2a, 0);
	fileMenu->insertItem(tr("Import..."), importMenu);
	exportMenu = new QPopupMenu();
	fid3 = exportMenu->insertItem(tr("Save Text..."), this, SLOT(SaveText()));
	exportMenu->setItemEnabled(fid3, 0);
	fid8 = exportMenu->insertItem(tr("Save Page as EPS..."), this, SLOT(SaveAsEps()));
	exportMenu->setItemEnabled(fid8, 0);
	fid10 = exportMenu->insertItem(tr("Save as PDF..."), this, SLOT(SaveAsPDF()));
	exportMenu->setItemEnabled(fid10, 0);
	fid11 = fileMenu->insertItem(tr("Export..."), exportMenu);
	fileMenu->setItemEnabled(fid11, 0);
	fileMenu->insertSeparator();
	fid6 = fileMenu->insertItem(tr("Document Info..."), this, SLOT(InfoDoc()), CTRL+Key_I);
	fileMenu->setItemEnabled(fid6, 0);
	SetKeyEntry(5, tr("Document Info..."), fid6, CTRL+Key_I);
	fid7 = fileMenu->insertItem(tr("Document Setup..."), this, SLOT(SetupDoc()));
	fileMenu->setItemEnabled(fid7, 0);
	SetKeyEntry(6, tr("Document Setup..."), fid7, 0);
	fid9 = fileMenu->insertItem(loadIcon("DateiPrint16.png"), tr("Print..."), this, SLOT(slotFilePrint()), CTRL+Key_P);
	fileMenu->setItemEnabled(fid9, 0);
	SetKeyEntry(7, tr("Print..."), fid9, CTRL+Key_P);
	fileMenu->insertSeparator();
	MenID = fileMenu->insertItem(loadIcon("exit.png"), tr("Quit"), this, SLOT(slotFileQuit()), CTRL+Key_Q);
	SetKeyEntry(8, tr("Quit"), MenID, CTRL+Key_Q);
	editMenu = new QPopupMenu();
	edUndo = editMenu->insertItem(tr("Undo"), this, SLOT(UnDoAction()), CTRL+Key_Z);
	editMenu->insertSeparator();
	edid1 = editMenu->insertItem(loadIcon("editcut.png"), tr("Cut"), this , SLOT(slotEditCut()), CTRL+Key_X);
	edid2 = editMenu->insertItem(loadIcon("editcopy.png"), tr("Copy"), this , SLOT(slotEditCopy()), CTRL+Key_C);
	edid3 = editMenu->insertItem(loadIcon("editpaste.png"), tr("Paste"), this , SLOT(slotEditPaste()), CTRL+Key_V);
	edid4 = editMenu->insertItem(loadIcon("editdelete.png"), tr("Clear"), this, SLOT(DeleteText()));
	edid5 = editMenu->insertItem(tr("Select all"), this, SLOT(SelectAll()), CTRL+Key_A);
	SetKeyEntry(9, tr("Cut"), edid1, CTRL+Key_X);
	SetKeyEntry(10, tr("Copy"), edid2, CTRL+Key_C);
	SetKeyEntry(11, tr("Paste"), edid3, CTRL+Key_V);
	SetKeyEntry(12, tr("Clear"), edid4, 0);
	SetKeyEntry(13, tr("Select all"), edid5, CTRL+Key_A);
	editMenu->insertSeparator();
	MenID = editMenu->insertItem(tr("Colors..."), this , SLOT(slotEditColors()));
	SetKeyEntry(14, tr("Colors..."), MenID, 0);
	edid6 = editMenu->insertItem(tr("Styles..."), this , SLOT(slotEditStyles()));
	SetKeyEntry(15, tr("Styles..."), edid6, 0);
	tman = editMenu->insertItem(tr("Templates..."), this, SLOT(ManageTemp()));
	SetKeyEntry(16, tr("Templates..."), tman, 0);
	jman = editMenu->insertItem(tr("JavaScripts..."), this, SLOT(ManageJava()));
	hymen = new QPopupMenu();
	MenID = hymen->insertItem(tr("General..."), this , SLOT(slotPrefsOrg()));
	SetKeyEntry(18, tr("Preferences..."), MenID, 0);
	MenID = hymen->insertItem(tr("Fonts..."), this , SLOT(slotFontOrg()));
	SetKeyEntry(17, tr("Fonts..."), MenID, 0);
	hymen->insertItem(tr("Hyphenator..."), this, SLOT(configHyphenator()));
	editMenu->insertItem(tr("Preferences"), hymen);
	editMenu->setItemEnabled(edUndo, 0);
	editMenu->setItemEnabled(edid1, 0);
	editMenu->setItemEnabled(edid2, 0);
	editMenu->setItemEnabled(edid3, 0);
	editMenu->setItemEnabled(edid4, 0);
	editMenu->setItemEnabled(edid5, 0);
	editMenu->setItemEnabled(edid6, 0);
	editMenu->setItemEnabled(tman, 0);
	editMenu->setItemEnabled(jman, 0);
	StilMenu = new QPopupMenu();
	ObjMenu = new QPopupMenu();
	MenID = ObjMenu->insertItem(tr("Modify..."), this, SLOT(ModifyObject()), CTRL+Key_M);
	SetKeyEntry(19, tr("Modify..."), MenID, CTRL+Key_M);
	MenID = ObjMenu->insertItem(tr("Duplicate"), this, SLOT(ObjektDup()), CTRL+Key_D);
	SetKeyEntry(20, tr("Duplicate"), MenID, CTRL+Key_D);
	MenID = ObjMenu->insertItem(tr("Multiple Duplicate"), this, SLOT(ObjektDupM()));
	SetKeyEntry(21, tr("Multiple Duplicate"), MenID, 0);
	Loesch = ObjMenu->insertItem(tr("Delete"), this, SLOT(DeleteObjekt()), CTRL+Key_K);
	SetKeyEntry(22, tr("Delete"), Loesch, CTRL+Key_K);
	ObjMenu->insertSeparator();
	Gr = ObjMenu->insertItem(tr("Group"), this, SLOT(GroupObj()), CTRL+Key_G);
	SetKeyEntry(23, tr("Group"), Gr, CTRL+Key_G);
	UnGr = ObjMenu->insertItem(tr("Ungroup"), this, SLOT(UnGroupObj()), CTRL+Key_U);
	SetKeyEntry(24, tr("Ungroup"), UnGr, CTRL+Key_U);
	LockOb = ObjMenu->insertItem(tr("Lock"), this, SLOT(ToggleObjLock()), CTRL+Key_H);
	ObjMenu->insertSeparator();
	OBack = ObjMenu->insertItem(tr("Send to Back"), this, SLOT(Objekt2Back()));
	SetKeyEntry(25, tr("Send to Back"), OBack, 0);
	OFront = ObjMenu->insertItem(tr("Bring to Front"), this, SLOT(Objekt2Front()));
	SetKeyEntry(26, tr("Bring to Front"), OFront, 0);
	OLower = ObjMenu->insertItem(tr("Lower"), this, SLOT(ObjektLower()));
	SetKeyEntry(27, tr("Lower"), OLower, 0);
	ORaise = ObjMenu->insertItem(tr("Raise"), this, SLOT(ObjektRaise()));
	SetKeyEntry(28, tr("Raise"), ORaise, 0);
	DistM = ObjMenu->insertItem(tr("Distribute/Align..."), this, SLOT(ObjektAlign()));
	SetKeyEntry(29, tr("Distribute/Align..."), DistM, 0);
	ObjMenu->insertSeparator();
	ShapeMenu = new QPopupMenu();
	ShapeMenu->insertItem(tr("Rectangle"), this, SLOT(RectFrame()));
	ShapeMenu->insertItem(tr("Rounded Rectangle"), this, SLOT(RoundedFrame()));
	ShapeMenu->insertItem(tr("Oval"), this, SLOT(OvalFrame()));
	ShapeEdit = ShapeMenu->insertItem(tr("Edit Frame"), this, SLOT(ToggleFrameEdit()));
	ShapeM = ObjMenu->insertItem(tr("Shape"), ShapeMenu);
	PfadT = ObjMenu->insertItem(tr("Attach Text to Path"), this, SLOT(Pfadtext()));
	PfadV = ObjMenu->insertItem(tr("Combine Polygons"), this, SLOT(UniteOb()));
	PfadS = ObjMenu->insertItem(tr("Split Polygon"), this, SLOT(SplitUniteOb()));
#ifdef HAVE_FREETYPE
	PfadTP = ObjMenu->insertItem(tr("Convert to Polygons"), this, SLOT(TraceText()));
#endif
	ObjMenu->setItemEnabled(ShapeM, 0);
	ObjMenu->setItemEnabled(DistM, 0);
	ObjMenu->setItemEnabled(Gr, 0);
	ObjMenu->setItemEnabled(UnGr, 0);
	ObjMenu->setItemEnabled(PfadT, 0);
	ObjMenu->setItemEnabled(PfadV, 0);
	ObjMenu->setItemEnabled(PfadS, 0);
	ObjMenu->setItemEnabled(LockOb, 0);
#ifdef HAVE_FREETYPE
	ObjMenu->setItemEnabled(PfadTP, 0);
#endif
	pageMenu = new QPopupMenu();
	MenID = pageMenu->insertItem(tr("Insert..."), this, SLOT(slotNewPageM()));
	SetKeyEntry(30, tr("Insert..."), MenID, 0);
	pgmd = pageMenu->insertItem(tr("Delete..."), this, SLOT(DeletePage()));
	SetKeyEntry(31, tr("Delete..."), pgmd, 0);
	MenID = pageMenu->insertItem(tr("Copy")+"...", this, SLOT(CopyPage()));
	SetKeyEntry(61, tr("Copy")+"...", MenID, 0);
	pgmv = pageMenu->insertItem(tr("Move..."), this, SLOT(MovePage()));
	SetKeyEntry(32, tr("Move..."), pgmv, 0);
	MenID = pageMenu->insertItem(tr("Apply Template..."), this, SLOT(ApplyTemp()));
	SetKeyEntry(33, tr("Apply Template..."), MenID, 0);
	MenID = pageMenu->insertItem(tr("Manage Guides..."), this, SLOT(ManageGuides()));
	SetKeyEntry(49, tr("Manage Guides..."), MenID, 0);
	pageMenu->setItemEnabled(pgmd, 0);
	pageMenu->setItemEnabled(pgmv, 0);
	viewMenu=new QPopupMenu();
	MenID = viewMenu->insertItem(tr("Fit in Window"), this, SLOT(slotZoomFit()), CTRL+Key_0);
	SetKeyEntry(34, tr("Fit in Window"), MenID, CTRL+Key_0);
	MenID = viewMenu->insertItem("50%", this, SLOT(slotZoom50()));
	SetKeyEntry(35, tr("50%"), MenID, 0);
	MenID = viewMenu->insertItem("75%", this, SLOT(slotZoom75()));
	SetKeyEntry(36, tr("75%"), MenID, 0);
	MenID = viewMenu->insertItem(tr("Actual Size"), this, SLOT(slotZoom100()), CTRL+Key_1);
	SetKeyEntry(37, tr("Actual Size"), MenID, CTRL+Key_1);
	MenID = viewMenu->insertItem("200%", this, SLOT(slotZoom200()));
	SetKeyEntry(38, tr("200%"), MenID, 0);
	MenID = viewMenu->insertItem(tr("Thumbnails"), this, SLOT(slotZoom20()));
	SetKeyEntry(39, tr("Thumbnails"), MenID, 0);
	viewMenu->insertSeparator();
	Markers = viewMenu->insertItem(tr("Hide Margins"), this, SLOT(ToggleMarks()));
	SetKeyEntry(40, tr("Hide Margins"), Markers, 0);
	FrameDr = viewMenu->insertItem(tr("Hide Frames"), this, SLOT(ToggleFrames()));
	SetKeyEntry(41, tr("Hide Frames"), FrameDr, 0);
	Bilder = viewMenu->insertItem(tr("Hide Images"), this, SLOT(TogglePics()));
	SetKeyEntry(42, tr("Hide Images"), Bilder, 0);
	Ras = viewMenu->insertItem(tr("Show Grid"), this, SLOT(ToggleRaster()));
	SetKeyEntry(43, tr("Show Grid"), Ras, 0);
	uRas = viewMenu->insertItem(tr("Snap to Grid"), this, SLOT(ToggleURaster()));
	SetKeyEntry(44, tr("Snap to Grid"), uRas, 0);
	Guide = viewMenu->insertItem(tr("Hide Guides"), this, SLOT(ToggleGuides()));
	uGuide = viewMenu->insertItem(tr("Snap to Guides"), this, SLOT(ToggleUGuides()));
	for (a=0; a<6; ++a)
		{
		viewMenu->setItemEnabled(viewMenu->idAt(a), 0);
		}
	viewMenu->setItemEnabled(Markers, 0);
	viewMenu->setItemEnabled(FrameDr, 0);
	viewMenu->setItemEnabled(Bilder, 0);
	viewMenu->setItemEnabled(Ras, 0);
	viewMenu->setItemEnabled(uRas, 0);
	viewMenu->setItemEnabled(Guide, 0);
	viewMenu->setItemEnabled(uGuide, 0);
	toolMenu=new QPopupMenu();
	viewTools = toolMenu->insertItem(tr("Hide Tools"), this, SLOT(ToggleTools()));
	SetKeyEntry(45, tr("Hide Tools"), viewTools, 0);
	viewMpal = toolMenu->insertItem(tr("Show Measurements"), this, SLOT(ToggleMpal()));
	SetKeyEntry(46, tr("Show Measurements"), viewMpal, 0);
	viewTpal = toolMenu->insertItem(tr("Show Outline"), this, SLOT(ToggleTpal()));
	SetKeyEntry(47, tr("Show Outline"), viewTpal, 0);
	viewBpal = toolMenu->insertItem(tr("Show Scrapbook"), this, SLOT(ToggleBpal()));
	SetKeyEntry(48, tr("Show Scrapbook"), viewBpal, 0);
	viewLpal = toolMenu->insertItem(tr("Show Layers"), this, SLOT(ToggleLpal()));
	viewSepal = toolMenu->insertItem(tr("Show Page Palette"), this, SLOT(ToggleSepal()));
	viewBopal = toolMenu->insertItem(tr("Show Bookmarks"), this, SLOT(ToggleBookpal()));
	extraMenu=new QPopupMenu();
	MenID = extraMenu->insertItem(tr("Manage Pictures"), this, SLOT(StatusPic()));
	SetKeyEntry(51, tr("Manage Pictures"), MenID, 0);
	hyph = extraMenu->insertItem(tr("Hyphenate Text"), this, SLOT(doHyphenate()));
	extraMenu->setItemEnabled(hyph, 0);
	SetKeyEntry(50, tr("Hyphenate Text"), hyph, 0);
	windowsMenu = new QPopupMenu();
	windowsMenu->setCheckable( true );
	connect(windowsMenu, SIGNAL(aboutToShow()), this, SLOT(windowsMenuAboutToShow()));
	helpMenu=new QPopupMenu();
	MenID = helpMenu->insertItem(tr("About Scribus"), this, SLOT(slotHelpAbout()));
	SetKeyEntry(52, tr("About Scribus"), MenID, 0);
	MenID = helpMenu->insertItem(tr("About Qt"), this, SLOT(slotHelpAboutQt()));
	SetKeyEntry(53, tr("About Qt"), MenID, 0);
	helpMenu->insertSeparator();
	MenID = helpMenu->insertItem(tr("Online-Help..."), this, SLOT(slotOnlineHelp()));
	SetKeyEntry(54, tr("Online-Help..."), MenID, 0);
	tip = helpMenu->insertItem(tr("Tool-Tips"), this, SLOT(ToggleTips()));
	SetKeyEntry(55, tr("Tool-Tips"), tip, 0);
  tipsOn = true;
  helpMenu->setItemChecked(tip, tipsOn);
//	helpMenu->insertItem(tr("Test"), this, SLOT(slotTest()));
//	helpMenu->insertItem(tr("Test2"), this, SLOT(slotTest2()));
	menuBar()->insertItem(tr("File"), fileMenu);
	menuBar()->insertItem(tr("Edit"), editMenu);
	Stm = menuBar()->insertItem(tr("Style"), StilMenu);
	Obm = menuBar()->insertItem(tr("Item"), ObjMenu);
	pgmm = menuBar()->insertItem(tr("Page"), pageMenu);
	menuBar()->setItemEnabled(Obm, 0);
	menuBar()->setItemEnabled(pgmm, 0);
	menuBar()->insertItem(tr("View"), viewMenu);
	menuBar()->insertItem(tr("Tools"), toolMenu);
	exmn = menuBar()->insertItem(tr("Extras"), extraMenu);
	menuBar()->setItemEnabled(exmn, 0);
	menuBar()->insertItem( tr("Windows"), windowsMenu );
	menuBar()->insertSeparator();
	menuBar()->insertItem(tr("Help"), helpMenu);
	AliMenu = new QPopupMenu();
	AliMenu->insertItem(tr("Left"));
	AliMenu->insertItem(tr("Center"));
	AliMenu->insertItem(tr("Right"));
	AliMenu->insertItem(tr("Block"));
	AliMenu->insertItem(tr("Forced"));
	ColorMenu = new QPopupMenu();
	SizeTMenu = new QPopupMenu();
	SizeTMenu->insertItem(tr("Other..."));
	SizeTMenu->insertItem(" 7 pt");
	SizeTMenu->insertItem(" 9 pt");
	SizeTMenu->insertItem("10 pt");
	SizeTMenu->insertItem("12 pt");
	SizeTMenu->insertItem("14 pt");
	SizeTMenu->insertItem("18 pt");
	SizeTMenu->insertItem("24 pt");
	SizeTMenu->insertItem("36 pt");
	SizeTMenu->insertItem("48 pt");
	SizeTMenu->insertItem("60 pt");
	SizeTMenu->insertItem("72 pt");
	ShadeMenu = new QPopupMenu();
	ShadeMenu->insertItem(tr("Other..."));
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
	FStyleMenu = new QPopupMenu();
	FontMenu = new QPopupMenu();
	TypeStyleMenu = new QPopupMenu();
	TypeStyleMenu->insertItem(tr("Normal"));
	tmp = qApp->font();
	tmp = qApp->font();
	tmp.setUnderline(true);
	TypeStyleMenu->insertItem(new FmItem(tr("Underline"), tmp));
	tmp = qApp->font();
	tmp.setStrikeOut(true);
	TypeStyleMenu->insertItem(new FmItem(tr("Strikethru"), tmp));
	TypeStyleMenu->insertItem(tr("Small Caps"));
	TypeStyleMenu->insertItem(tr("Superscript"));
	TypeStyleMenu->insertItem(tr("Subscript"));
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
	XMess->setText(tr("X-Pos:"));
	statusBar()->addWidget(XMess, 0, true);
	XDat = new QLabel(statusBar(), "dt");
	statusBar()->addWidget(XDat, 1, true);
	XDat->setText("         ");
	YMess = new QLabel(statusBar(), "yt");
	YMess->setText(tr("Y-Pos:"));
	statusBar()->addWidget(YMess, 0, true);
	YDat = new QLabel(statusBar(), "ydt");
	statusBar()->addWidget(YDat, 1, true);
	YDat->setText("         ");	
}

void ScribusApp::ReportMP(float xp, float yp)
{
	QString tmp, tmp2;
	switch (doc->Einheit)
		{
		case 0:
			tmp2 = " pt";
			break;
		case 1:
			tmp2 = " mm";
			break;
		case 2:
			tmp2 = " in";
			break;
		case 3:
			tmp2 = " p";
			break;
		}
	XDat->setText(tmp.setNum(qRound(xp*UmReFaktor * 100) / 100.0, 'f', 2)+tmp2);
	YDat->setText(tmp.setNum(qRound(yp*UmReFaktor * 100) / 100.0, 'f', 2)+tmp2);
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
 	for (it = b->Ptext.first(); it != 0; it = b->Ptext.next())
 		{
 		if (it->cselect)
 			{
 			b->Ptext.remove();
 			it = b->Ptext.prev();
 			if (it == 0)
 				{
 				it = b->Ptext.first();
 				}
 			}
 		}
	if (b->Ptext.count() != 0)
		{
 		if (b->Ptext.first()->cselect)
 			{
 			b->Ptext.remove();
			b->CPos = 0;
 			}
		else
 			b->CPos = QMIN(b->CPos, doc->ActPage->oldCp);
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
		emit TextIFont(b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cfont);
 		emit TextISize(b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->csize);
 		emit TextUSval(b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cextra);
 		emit TextStil(b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cstyle & 127);
 		doc->CurrentStyle = b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cstyle & 127;
 		doc->CurrentABStil = b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cab;
 		setAbsValue(doc->CurrentABStil);
 		Mpal->setAli(doc->CurrentABStil);
 		b->IFont = b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cfont;
 		b->ISize = b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->csize;
		emit TextFarben(b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->ccolor,
										b->Pcolor,
										b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cshade,
										b->Shade);
		b->Pcolor2 = b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->ccolor;
		b->Shade2 = b->Ptext.at(QMIN(b->CPos, static_cast<int>(b->Ptext.count()-1)))->cshade;
 		}
}

void ScribusApp::wheelEvent(QWheelEvent *w)
{
	if (HaveDoc)
		{
		if (w->delta() < 0)
			view->scrollBy(0, Prefs.Wheelval);
		else
			view->scrollBy(0, -Prefs.Wheelval);
		w->accept();
		}
}

void ScribusApp::keyPressEvent(QKeyEvent *k)
{
	struct Pti *hg;
	int kk = k->key();
	int as = k->ascii();
	float altx, alty;
	QString uc = k->text();
	QString cr, Tcha, Twort;
	uint Tcoun;
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
		}
 	if ((HaveDoc) && (!view->LE->hasFocus()))
 		{
 		switch (kk)
 			{
 			case Key_Prior:
 				view->scrollBy(0, -Prefs.Wheelval);
 				return;
 				break;
 			case Key_Next:
 				view->scrollBy(0, Prefs.Wheelval);
 				return;
 				break;
 			}
 		if (doc->ActPage->SelItem.count() != 0)
 			{
 			PageItem *b = doc->ActPage->SelItem.at(0);
 			switch (doc->AppMode)
 				{
 				case 1:
 					switch (kk)
 						{
 						case Key_Delete:
 							if (!doc->EditClip)
								{
								if (!b->Locked)
 									doc->ActPage->DeleteItem();
								}
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
 								doc->ActPage->moveGroup(-1, 0);
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
 								doc->ActPage->moveGroup(1, 0);
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
 								doc->ActPage->moveGroup(0, -1);
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
 								doc->ActPage->moveGroup(0, 1);
 						default:
 							if (b->PType == 4)
 								{
 								if ((kk + KeyMod) == Prefs.KeyActions[59].KeyID)
 									{
									setNewAbStyle(1);
 									b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									}
 								if ((kk + KeyMod) == Prefs.KeyActions[58].KeyID)
 									{
									setNewAbStyle(2);
 									b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									}
 								if ((kk + KeyMod) == Prefs.KeyActions[57].KeyID)
 									{
									setNewAbStyle(0);
 									b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									}
 								}
 							break;
 						}
 					slotDocCh();
 					break;
 				case 7:
 					if (b->PType == 4)
 						{
 						doc->ActPage->slotDoCurs(false);
 						switch (kk)
 							{
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
 								setTBvals(b);
 								break;
							case Key_Home:
 								b->CPos = 0;
 								setTBvals(b);
 								break;
							case Key_End:
 								b->CPos = static_cast<int>(b->Ptext.count());
 								setTBvals(b);
 								break;
 							case Key_Left:
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
 								setTBvals(b);
 								break;
 							case Key_Right:
 								b->CPos += 1;
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
 								setTBvals(b);
 								break;
 							case Key_Delete:
 								if (b->CPos == static_cast<int>(b->Ptext.count())) { return; }
 								if (b->Ptext.count() == 0) { return; }
 								cr = b->Ptext.at(b->CPos)->ch;
 								if (b->HasSel)
 									{
 									DeleteSel(b);
 									}
 								else
 									{
 									b->Ptext.remove(b->CPos);
 									}
 								b->Tinput = false;
 								if ((cr == QChar(13)) && (b->Ptext.count() != 0))
 									{
 									doc->ActPage->chAbStyle(b, b->Ptext.at(QMAX(b->CPos-1,0))->cab);
 									b->Tinput = false;
 									}
 								setTBvals(b);
 								b->Dirty = true;
 								b->paintObj();
 								break;
 							case Key_Backspace:
 								if (b->CPos == 0)
 									return;
 								if (b->Ptext.count() == 0) { return; }
 								cr = b->Ptext.at(QMAX(b->CPos-1,0))->ch;
 								if (b->HasSel)
 									{
 									DeleteSel(b);
 									}
 								else
 									{
 									b->CPos -= 1;
 									b->Ptext.remove(b->CPos);
 									}
 								b->Tinput = false;
 								if ((cr == QChar(13)) && (b->Ptext.count() != 0))
 									{
 									doc->ActPage->chAbStyle(b, b->Ptext.at(QMAX(b->CPos-1,0))->cab);
 									b->Tinput = false;
 									}
 								setTBvals(b);
 								b->Dirty = true;
 								b->paintObj();
 								break;
 							default:
 								if ((b->HasSel) && (kk < 0x1000))
 									{
 									DeleteSel(b);
 									b->Dirty = true;
 									b->paintObj();
 									}
 								if ((kk + KeyMod) == Prefs.KeyActions[60].KeyID)
 									{
 									hg = new Pti;
 									hg->ch = QString(QChar(30));
 									hg->cfont = b->IFont;
 									hg->csize = b->ISize;
 									hg->ccolor = b->Pcolor2;
 									hg->cextra = 0;
 									hg->cshade = b->Shade2;
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
 									b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									break;
 									}
 								if ((kk + KeyMod) == Prefs.KeyActions[56].KeyID)
 									{
 									b->Ptext.at(QMAX(b->CPos-1,0))->cstyle ^= 128;
 									b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									break;
 									}
 								if ((kk + KeyMod) == Prefs.KeyActions[59].KeyID)
 									{
									setNewAbStyle(1);
 									b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									break;
 									}
 								if ((kk + KeyMod) == Prefs.KeyActions[57].KeyID)
 									{
									setNewAbStyle(0);
 									b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									break;
 									}
 								if ((kk + KeyMod) == Prefs.KeyActions[58].KeyID)
 									{
									setNewAbStyle(2);
 									b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									break;
 									}
#ifdef HAVE_FREETYPE
								if (((uc[0] > QChar(31)) || (as == 13) || (as == 30)) && ((*doc->AllFonts)[b->IFont]->CharWidth.contains(uc[0].unicode())))
#else
 								if ((uc[0] > QChar(31)) || (as == 13) || (as == 30))
#endif
 									{
 									hg = new Pti;
 									hg->ch = uc;
 									hg->cfont = b->IFont;
 									hg->csize = b->ISize;
 									hg->ccolor = b->Pcolor2;
 									hg->cextra = 0;
 									hg->cshade = b->Shade2;
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
											doc->Trenner->slotHyphenateWord(b, Twort, Tcoun);
 											b->Dirty = true;
											}
										}
 									if ((b->CPos < static_cast<int>(b->Ptext.count())) || (as == 30))
 										b->Dirty = true;
 									b->Tinput = true;
 									b->paintObj();
 									}
 								break;
 							}
 						if (b->Ptext.count() != 0)
 							if (b->Ptext.at(QMAX(b->CPos-1, 0))->yp != 0)
 								doc->ActPage->slotDoCurs(true);
 						if ((kk == Key_Left) || (kk == Key_Right) || (kk == Key_Up) || (kk == Key_Down))
 							return;
 						}
 					slotDocCh(false);
 					break;
 				}
 			}
 		}
}

void ScribusApp::closeEvent(QCloseEvent *ce)
{
	QWidgetList windows = wsp->windowList();
	ScribusWin* tw;
	if (!windows.isEmpty())
		{
		for ( int i = 0; i < static_cast<int>(windows.count()); ++i )
			{
			newActWin(windows.at(i));
	 		tw = ActWin;
			ActWin->close();
			if (tw == ActWin)
				{
				ce->ignore();
				return;
				}
			}
		SavePrefs();
		if (Prefs.SaveAtQ)
			{
			if (ScBook->ScFilename.isEmpty())
				ScBook->ScFilename = PrefsPfad+"/scrap.scs";
			ScBook->Save();
			}
		if (ScBook->BibWin->Objekte.count() == 0)
			system("rm -f " + PrefsPfad+"/scrap.scs");
		Prefs.AvailFonts.~SCFonts();
		FinalizePlugs();
		exit(0);
		}
	else
		{
		SavePrefs();
		if (Prefs.SaveAtQ)
			{
			if (ScBook->ScFilename.isEmpty())
				ScBook->ScFilename = PrefsPfad+"/scrap.scs";
			ScBook->Save();
			}
		if (ScBook->BibWin->Objekte.count() == 0)
			system("rm -f " + PrefsPfad+"/scrap.scs");
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		Prefs.AvailFonts.~SCFonts();
		FinalizePlugs();
		exit(0);
		}
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////
float ScribusApp::mm2pts(int mm)
{
	return mm / 25.4 * 72;
}

float ScribusApp::pts2mm(float pts)
{
	return pts / 72 * 25.4;
}

bool ScribusApp::slotFileNew()
{
	float b, h, tpr, lr, rr, br, sp, ab;
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
		if (dia->ComboBox3->currentItem() == 1)
			{
			b = mm2pts(qRound(pts2mm(dia->Pagebr)));
			h = mm2pts(qRound(pts2mm(dia->Pageho)));
			}
		ret = doFileNew(b, h, tpr, lr, rr, br, ab, sp, atf, fp, dia->ComboBox3->currentItem(),
										dia->ErsteSeite->isChecked(), dia->Orient, dia->PgNr->value());
  	FMess->setText(tr("Ready"));
		}
	else
		ret = false;
	delete dia;
	return ret;
}

bool ScribusApp::doFileNew(float b, float h, float tpr, float lr, float rr, float br, float ab, float sp,
													 bool atf, bool fp, int einh, bool firstleft, int Ori, int SNr)
{
	QString cc;
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
	doc->ShFrames = Prefs.ShFrames;
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
	doc->CMSSettings.CMSinUse = Prefs.DCMSset.CMSinUse;
	doc->PDF_Optionen.SolidProf = doc->CMSSettings.DefaultInputProfile2;
	doc->PDF_Optionen.ImageProf = doc->CMSSettings.DefaultInputProfile;
	doc->PDF_Optionen.PrintProf = doc->CMSSettings.DefaultPrinterProfile;
	doc->PDF_Optionen.Intent = doc->CMSSettings.DefaultIntentMonitor;
	doc->PDF_Optionen.Intent2 = doc->CMSSettings.DefaultIntentMonitor2;
	doc->ActiveLayer = 0;
	HaveDoc++;
	DocNr++;
	doc->Scale = 1.0*Prefs.DisScale;
	doc->AppMode = 1;
	doc->Language = Prefs.Language;
	doc->MinWordLen = Prefs.MinWordLen;
	doc->Automatic = Prefs.Automatic;
	doc->AutoCheck = Prefs.AutoCheck;
	doc->PageColors = Prefs.DColors;
	ScribusWin* w = new ScribusWin(wsp, doc);
	view = new ScribusView(w, doc, &Prefs);
	w->setView(view);
	ActWin = w;
	w->setCentralWidget(view);
	connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
	connect(w, SIGNAL(SaveAndClose()), this, SLOT(DoSaveClose()));
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
	slotNewPage(0);
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
	Tpal->BuildTree(view);
	Sepal->Rebuild();
	BookPal->BView->clear();
	if ( wsp->windowList().isEmpty() )
		w->showMaximized();
	else
		w->show();
	view->show();
	connect(doc->ASaveTimer, SIGNAL(timeout()), this, SLOT(slotAutoSave()));
	doc->AutoSave = Prefs.AutoSave;
	if (doc->AutoSave)
		doc->ASaveTimer->start(Prefs.AutoSaveTime);
	return true;
}

void ScribusApp::DoSaveClose()
{
	slotFileSaveAs();
	DoFileClose();
}

void ScribusApp::windowsMenuAboutToShow()
{
	windowsMenu->clear();
	int cascadeId = windowsMenu->insertItem(tr("Cascade"), wsp, SLOT(cascade() ) );
	int tileId = windowsMenu->insertItem(tr("Tile"), wsp, SLOT(tile() ) );
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
	Sepal->Rebuild();
	Tpal->BuildTree(view);
	BookPal->BView->NrItems = ActWin->NrItems;
	BookPal->BView->First = ActWin->First;
	BookPal->BView->Last = ActWin->Last;
	RestoreBookMarks();
}

void ScribusApp::windowsMenuActivated( int id )
{
	QWidget* w = wsp->windowList().at( id );
	if ( w )
		w->showNormal();
	newActWin(w);
}

bool ScribusApp::SetupDoc()
{
	float tpr = doc->PageM.Top;
	float lr = doc->PageM.Left;
	float rr = doc->PageM.Right;
	float br = doc->PageM.Bottom;	
	bool fp = doc->PageFP;
	bool fpe = doc->FirstPageLeft;
	float tpr2, lr2, rr2, br2;
	bool ret = false;
	ReformDoc* dia = new ReformDoc(this, tpr, lr, rr, br, fp, fpe, doc->Einheit);
	if (dia->exec())
		{
		tpr2 = dia->TopR->value() / UmReFaktor / 100.0;
		lr2 = dia->LeftR->value() / UmReFaktor / 100.0;
		rr2 = dia->RightR->value() / UmReFaktor / 100.0;
		br2 = dia->BottomR->value() / UmReFaktor / 100.0;
		fp = dia->Doppelseiten->isChecked();
		if (fp)
			doc->FirstPageLeft = dia->ErsteSeite->isChecked();
		doc->resetPage(tpr2, lr2, rr2, br2, fp);
		view->reformPages();
		doc->setModified();
		view->GotoPage(doc->ActPage->PageNr);
		view->DrawNew();
		Sepal->RebuildPage();
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
	ColorMenu->clear();
	ColorMenu->insertItem(tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		ColorMenu->insertItem(QIconSet(pm), it.key());
		if (it.key() == doc->Dbrush)
			{
			ColorMenu->setItemChecked(ColorMenu->idAt(a), true);
			}
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
	ActWin->setCaption(tr(doc->DocName));
	ShadeMenu->setItemChecked(ShadeMenu->idAt(11), true);
	Mpal->SetDoc(doc);
	Sepal->SetView(view);
	Mpal->Spal->SetFormats(doc);
	Lpal->setLayers(&doc->Layers, &doc->ActiveLayer);
	view->LaMenu();
	view->setLayMenTxt(doc->ActiveLayer);
	doc->CurrentABStil = 0;
	slotChangeUnit(doc->Einheit);
	if (doc->EditClip)
		{
		doc->EditClip = !doc->EditClip;
		ToggleFrameEdit();
		}
	if (doc->ShFrames)
		viewMenu->changeItem(FrameDr, tr("Hide Frames"));
	else
		viewMenu->changeItem(FrameDr, tr("Show Frames"));
	DatClo->setEnabled(true);
	if (doc->TemplateMode)
		{
		menuBar()->setItemEnabled(pgmm, 0);
		editMenu->setItemEnabled(tman, 0);
		DatNeu->setEnabled(false);
		DatSav->setEnabled(false);
		DatOpe->setEnabled(false);
		DatClo->setEnabled(false);
		fileMenu->setItemEnabled(fid1, 0);
		fileMenu->setItemEnabled(fid4, 0);
		fileMenu->setItemEnabled(fid5, 0);
		fileMenu->setItemEnabled(fid51, 0);
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
		fileMenu->setItemEnabled(fid1, 1);
		fileMenu->setItemEnabled(fid4, ActWin->MenuStat[2]);
		fileMenu->setItemEnabled(fid5, ActWin->MenuStat[3]);
		fileMenu->setItemEnabled(fid51, ActWin->MenuStat[3]);
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
			slotDocCh();
		else
			{
			fileMenu->setItemEnabled(fid4, 0);
			DatSav->setEnabled(false);
			}
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
	fileMenu->setItemEnabled(fid1, 1);
	fileMenu->setItemEnabled(fid4, 0);
	fileMenu->setItemEnabled(fid5, 1);
	fileMenu->setItemEnabled(fid51, 1);
	fileMenu->setItemEnabled(fid6, 1);
	fileMenu->setItemEnabled(fid7, 1);
	exportMenu->setItemEnabled(fid8, 1);
	importMenu->setItemEnabled(fid2a, 1);
	fileMenu->setItemEnabled(fid9, 1);
	if (PDFavailable)
		exportMenu->setItemEnabled(fid10, 1);
	fileMenu->setItemEnabled(fid11, 1);
	editMenu->setItemEnabled(edid1, 0);
	editMenu->setItemEnabled(edid2, 0);
	editMenu->setItemEnabled(edid3, 0);
	editMenu->setItemEnabled(edid6, 1);
	for (a=0; a<6; ++a)
		{
		viewMenu->setItemEnabled(viewMenu->idAt(a), 1);
		}
	viewMenu->setItemEnabled(Markers, 1);
	viewMenu->setItemEnabled(FrameDr, 1);
	viewMenu->setItemEnabled(Bilder, 1);
	viewMenu->setItemEnabled(Ras, 1);
	viewMenu->setItemEnabled(uRas, 1);
	viewMenu->setItemChecked(uRas, doc->useRaster);
	viewMenu->setItemEnabled(Guide, 1);
	viewMenu->setItemEnabled(uGuide, 1);
	viewMenu->setItemChecked(uGuide, doc->SnapGuides);
	menuBar()->setItemEnabled(pgmm, 1);
	menuBar()->setItemEnabled(exmn, 1);
	WerkTools->setEnabled(true);
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
	editMenu->setItemEnabled(tman, 1);
	editMenu->setItemEnabled(jman, 1);
	CListe::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	a = 0;
	ColorMenu->clear();
	ColorMenu->insertItem(tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		ColorMenu->insertItem(QIconSet(pm), it.key());
		if (it.key() == doc->Dbrush)
			{
			ColorMenu->setItemChecked(ColorMenu->idAt(a), true);
			}
		a++;
		}
	Mpal->Cpal->SetColors(doc->PageColors);
	Mpal->Cpal->ChooseGrad(0);
	ActWin->setCaption(tr(doc->DocName));
	ShadeMenu->setItemChecked(ShadeMenu->idAt(11), true);
	Mpal->SetDoc(doc);
	Sepal->SetView(view);
	Mpal->Spal->SetFormats(doc);
	Lpal->setLayers(&doc->Layers, &doc->ActiveLayer);
	view->LaMenu();
	view->setLayMenTxt(doc->ActiveLayer);
	doc->CurrentABStil = 0;
	slotChangeUnit(doc->Einheit);
	doc->Trenner = new Hyphenator(this, doc, this);
  BuildFontMenu();
	connect(view, SIGNAL(changeUN(int)), this, SLOT(slotChangeUnit(int)));
	connect(view, SIGNAL(changeLA(int)), Lpal, SLOT(MarkActiveLayer(int)));
	doc->PDF_Optionen.BleedBottom = doc->PageM.Bottom;
	doc->PDF_Optionen.BleedTop = doc->PageM.Top;
	doc->PDF_Optionen.BleedLeft = doc->PageM.Left;
	doc->PDF_Optionen.BleedRight = doc->PageM.Right;
}

void ScribusApp::HaveNewSel(int Nr)
{
	PageItem *b;
	switch (Nr)
		{
		case -1:
			importMenu->changeItem(fid2, tr("Get Text/Picture..."));
			importMenu->setItemEnabled(fid2, 0);
			exportMenu->setItemEnabled(fid3, 0);
			menuBar()->setItemEnabled(Obm, 0);
			ObjMenu->setItemEnabled(ShapeM, 0);
#ifdef HAVE_FREETYPE
			ObjMenu->setItemEnabled(PfadTP, 0);
#endif
			ObjMenu->setItemEnabled(LockOb, 0);
			editMenu->setItemEnabled(edid1, 0);
			editMenu->setItemEnabled(edid2, 0);
			editMenu->setItemEnabled(edid4, 0);
			editMenu->setItemEnabled(edid5, 0);
			extraMenu->setItemEnabled(hyph, 0);
			StilMenu->clear();
			WerkTools->KetteAus->setEnabled(false);
			WerkTools->KetteEin->setEnabled(false);
			WerkTools->Textedit->setEnabled(false);
			WerkTools->Rotiere->setEnabled(false);
			Mpal->Cpal->GradCombo->setCurrentItem(0);
			Mpal->Cpal->ChooseGrad(0);
			break;
		case 2:
			importMenu->changeItem(fid2, tr("Get Picture..."));
			importMenu->setItemEnabled(fid2, 1);
			editMenu->setItemEnabled(edid1, 1);
			editMenu->setItemEnabled(edid2, 1);
			editMenu->setItemEnabled(edid4, 0);
			editMenu->setItemEnabled(edid5, 0);
			extraMenu->setItemEnabled(hyph, 0);
			menuBar()->setItemEnabled(Obm, 1);
			ObjMenu->setItemEnabled(ShapeM, 1);
#ifdef HAVE_FREETYPE
			ObjMenu->setItemEnabled(PfadTP, 0);
#endif
			StilMenu->clear();
			StilMenu->insertItem(tr("Color"), ColorMenu);
			StilMenu->insertItem(tr("Invert"));
			WerkTools->KetteAus->setEnabled(false);
			WerkTools->KetteEin->setEnabled(false);
			WerkTools->Textedit->setEnabled(true);
			WerkTools->Rotiere->setEnabled(true);
 			Mpal->SetCurItem(doc->ActPage->SelItem.at(0));
			break;
		case 4:
			importMenu->changeItem(fid2, tr("Get Text..."));
			importMenu->setItemEnabled(fid2, 1);
			exportMenu->setItemEnabled(fid3, 1);
			editMenu->setItemEnabled(edid1, 1);
			editMenu->setItemEnabled(edid2, 1);
			editMenu->setItemEnabled(edid4, 0);
			editMenu->setItemEnabled(edid5, 0);
			extraMenu->setItemEnabled(hyph, 1);
			menuBar()->setItemEnabled(Obm, 1);
			ObjMenu->setItemEnabled(ShapeM, 1);
#ifdef HAVE_FREETYPE
			ObjMenu->setItemEnabled(PfadTP, 1);
#endif
			StilMenu->clear();
			StilMenu->insertItem(tr("Font"), FontMenu);
			StilMenu->insertItem(tr("Size"), SizeTMenu);
			StilMenu->insertItem(tr("Style"), TypeStyleMenu);
			StilMenu->insertItem(tr("Alignment"), AliMenu);
			StilMenu->insertItem(tr("Color"), ColorMenu);
			StilMenu->insertItem(tr("Shade"), ShadeMenu);
			WerkTools->Rotiere->setEnabled(true);
			b = doc->ActPage->SelItem.at(0);
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
 			Mpal->SetCurItem(b);
			doc->CurrentStyle = 0;
			doc->Vorlagen[0].LineSpa = b->LineSp;
			doc->Vorlagen[0].Ausri = b->Ausrich;
			break;
		case 8:
			importMenu->changeItem(fid2, tr("Get Text..."));
			importMenu->setItemEnabled(fid2, 1);
			exportMenu->setItemEnabled(fid3, 1);
			editMenu->setItemEnabled(edid1, 1);
			editMenu->setItemEnabled(edid2, 1);
			editMenu->setItemEnabled(edid4, 0);
			editMenu->setItemEnabled(edid5, 0);
			extraMenu->setItemEnabled(hyph, 0);
			menuBar()->setItemEnabled(Obm, 1);
			ObjMenu->setItemEnabled(ShapeM, 0);
#ifdef HAVE_FREETYPE
			ObjMenu->setItemEnabled(PfadTP, 0);
#endif
			StilMenu->clear();
			StilMenu->insertItem(tr("Font"), FontMenu);
			StilMenu->insertItem(tr("Size"), SizeTMenu);
			StilMenu->insertItem(tr("Style"), TypeStyleMenu);
			StilMenu->insertItem(tr("Color"), ColorMenu);
			StilMenu->insertItem(tr("Shade"), ShadeMenu);
			WerkTools->Rotiere->setEnabled(true);
			WerkTools->Textedit->setEnabled(false);
			WerkTools->KetteEin->setEnabled(false);
			WerkTools->KetteAus->setEnabled(false);
 			Mpal->SetCurItem(doc->ActPage->SelItem.at(0));
			doc->CurrentStyle = 0;
			break;
		default:
			importMenu->changeItem(fid2, tr("Get Text/Picture..."));
			importMenu->setItemEnabled(fid2, 0);
			exportMenu->setItemEnabled(fid3, 0);
			editMenu->setItemEnabled(edid1, 1);
			editMenu->setItemEnabled(edid2, 1);
			editMenu->setItemEnabled(edid4, 0);
			editMenu->setItemEnabled(edid5, 0);
			extraMenu->setItemEnabled(hyph, 0);
			menuBar()->setItemEnabled(Obm, 1);
			StilMenu->clear();
			StilMenu->insertItem(tr("Color"), ColorMenu);
			StilMenu->insertItem(tr("Shade"), ShadeMenu);
			WerkTools->KetteAus->setEnabled(false);
			WerkTools->KetteEin->setEnabled(false);
			if (Nr != 5)
				WerkTools->Rotiere->setEnabled(true);
			else
				WerkTools->Rotiere->setEnabled(false);
 			Mpal->SetCurItem(doc->ActPage->SelItem.at(0));
			break;
		}
	doc->CurrentSel = Nr;
	Mpal->RotationGroup->setButton(doc->RotMode);
	if (doc->ActPage->SelItem.count() > 1)
		{
		ObjMenu->setItemEnabled(DistM, 1);
		ObjMenu->setItemEnabled(Gr, 1);
#ifdef HAVE_FREETYPE
		ObjMenu->setItemEnabled(PfadTP, 0);
#endif
		bool hPoly = false;
		if (doc->ActPage->SelItem.at(0)->Groups.count() == 0)
			hPoly = true;
		for (uint b=0; b<doc->ActPage->SelItem.count(); ++b)
			{
			if (doc->ActPage->SelItem.at(b)->PType != 6)
				hPoly = false;
			}
		ObjMenu->setItemEnabled(PfadV, hPoly);
		if (doc->ActPage->SelItem.count() == 2)
			{
			if (((doc->ActPage->SelItem.at(0)->PType == 4) || (doc->ActPage->SelItem.at(1)->PType == 4)) && ((doc->ActPage->SelItem.at(0)->PType == 7) || (doc->ActPage->SelItem.at(1)->PType == 7)))
				ObjMenu->setItemEnabled(PfadT, 1);
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
		Mpal->Textflow->setChecked(doc->ActPage->SelItem.at(0)->Textflow);
		ObjMenu->setItemEnabled(LockOb, 1);
		if (doc->ActPage->SelItem.at(0)->Groups.count() != 0)
			ObjMenu->setItemEnabled(UnGr, 1);
		else
			{
			ObjMenu->setItemEnabled(UnGr, 0);
			if ((doc->ActPage->SelItem.at(0)->PType == 6) && (doc->ActPage->SelItem.at(0)->Segments.count() != 0))
				ObjMenu->setItemEnabled(PfadS, 1);
			else
				ObjMenu->setItemEnabled(PfadS, 0);
			}
		if (doc->ActPage->SelItem.at(0)->Locked)
			{
			ObjMenu->setItemEnabled(DistM, 0);
			ObjMenu->setItemEnabled(ShapeM, 0);
#ifdef HAVE_FREETYPE
			ObjMenu->setItemEnabled(PfadTP, 0);
#endif
			ObjMenu->setItemEnabled(PfadS, 0);
			ObjMenu->setItemEnabled(PfadT, 0);
			ObjMenu->setItemEnabled(PfadV, 0);
			ObjMenu->setItemEnabled(Loesch, 0);
			ObjMenu->setItemEnabled(OBack, 0);
			ObjMenu->setItemEnabled(OFront, 0);
			ObjMenu->setItemEnabled(ORaise, 0);
			ObjMenu->setItemEnabled(OLower, 0);
			editMenu->setItemEnabled(edid1, 0);
			editMenu->setItemEnabled(edid4, 0);
			WerkTools->Rotiere->setEnabled(false);
			ObjMenu->changeItem(LockOb, tr("Unlock"));
			}
		else
			{
			ObjMenu->changeItem(LockOb, tr("Lock"));
			ObjMenu->setItemEnabled(OBack, 1);
			ObjMenu->setItemEnabled(OFront, 1);
			ObjMenu->setItemEnabled(ORaise, 1);
			ObjMenu->setItemEnabled(OLower, 1);
			}
		}
}

void ScribusApp::slotDocCh(bool reb)
{
	if (reb)
		Tpal->BuildTree(view);
	if (!doc->isModified())
		doc->setModified();
	ActWin->setCaption(tr(doc->DocName) + "*");
	if ((!doc->DocName.startsWith("Document")) && (!doc->TemplateMode))
		{
		fileMenu->setItemEnabled(fid4, 1);
		DatSav->setEnabled(true);
		DatClo->setEnabled(true);
		}
	else
		{
		fileMenu->setItemEnabled(fid4, 0);
		DatSav->setEnabled(false);
		}
	if (!doc->TemplateMode)
		{		
		fileMenu->setItemEnabled(fid5, 1);
		fileMenu->setItemEnabled(fid51, 1);
		}
	ActWin->MenuStat[0] = DatSav->isEnabled();
	ActWin->MenuStat[1] = fileMenu->isItemEnabled(fid1);
	ActWin->MenuStat[2] = fileMenu->isItemEnabled(fid4);
	ActWin->MenuStat[3] = fileMenu->isItemEnabled(fid5);
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
	QString fileName = CFileDialog(tr("Open"),tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);; All Files (*)"));
#else
	QString fileName = CFileDialog(tr("Open"),tr("Documents (*.sla *.scd);; All Files (*)"));
#endif
  qApp->setOverrideCursor(QCursor(waitCursor), true);
	ret = LadeDoc(fileName);
  qApp->setOverrideCursor(QCursor(arrowCursor), true);
	return ret;
}

bool ScribusApp::slotDocMerge()
{
	bool ret = false;
	MergeDoc *dia = new MergeDoc(this);
	if (dia->exec())
		{
		qApp->setOverrideCursor(QCursor(waitCursor), true);
		ret = LadeSeite(dia->Filename->text(), dia->PageNr->value()-1);
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		ret = true;
		}
	delete dia;
	return ret;
}

bool ScribusApp::LadeSeite(QString fileName, int Nr)
{
	bool ret = false;
  if (!fileName.isEmpty())
  	{
		doc->loading = true;
  	ScriXmlDoc *ss = new ScriXmlDoc();
  	if(!ss->ReadPage(fileName, Prefs.AvailFonts, doc, view, Nr))
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
		slotDocCh();
		doc->loading = false;
		ret = true;
		}
	Sepal->Rebuild();
  doc->ActPage->update();
	return ret;
}
	
bool ScribusApp::LadeDoc(QString fileName)
{
	bool ret = false;
  if (!fileName.isEmpty())
  	{
  	QFileInfo fi(fileName);
  	QDir::setCurrent(fi.dirPath(true));
		doc=new ScribusDoc();
		doc->AllFonts = &Prefs.AvailFonts;
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
  	doc->Dpen = Prefs.Dpen;
  	doc->DpenText = Prefs.DpenText;
  	doc->Dbrush = Prefs.Dbrush;
  	doc->Dshade = Prefs.Dshade;
  	doc->Dshade2 = Prefs.Dshade2;
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
		doc->ShFrames = Prefs.ShFrames;
		doc->RandFarbig = Prefs.RandFarbig;
		doc->AutoLine = Prefs.AutoLine;
		doc->Scale = 1.0*Prefs.DisScale;
		doc->AppMode = 1;
		doc->HasCMS = false;
		doc->ActiveLayer = 0;
		doc->Language = Prefs.Language;
		doc->MinWordLen = Prefs.MinWordLen;
		doc->Automatic = Prefs.Automatic;
		doc->AutoCheck = Prefs.AutoCheck;
		doc->loading = true;
		FMess->setText(tr("Loading..."));
		FProg->reset();
		ScribusWin* w = new ScribusWin(wsp, doc);
		view = new ScribusView(w, doc, &Prefs);
		w->setView(view);
		ActWin = w;
		w->setCentralWidget(view);
  	ScriXmlDoc *ss = new ScriXmlDoc();
  	connect(ss, SIGNAL(NewPage(int)), this, SLOT(slotNewPage(int)));
#ifdef HAVE_CMS
		w->SoftProofing = false;
		w->Gamut = false;
		bool cmsu = CMSuse;
		CMSuse = false;
#endif
  	if(!ss->ReadDoc(fileName, Prefs.AvailFonts, doc, view, FProg))
  		{
//			w->close();
  		view->close();
  		disconnect(ss, SIGNAL(NewPage(int)), this, SLOT(slotNewPage(int)));
  		delete ss;
  		delete view;
  		delete doc;
			delete w;
			FMess->setText("");
			FProg->reset();
			ActWin = NULL;
  		return false;
  		}
  	delete ss;
		FMess->setText("");
		FProg->reset();
#ifdef HAVE_CMS
		CMSuse = cmsu;
#endif
		HaveDoc++;
		connect(w, SIGNAL(Schliessen()), this, SLOT(DoFileClose()));
		connect(w, SIGNAL(SaveAndClose()), this, SLOT(DoSaveClose()));
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
		doc->DocName = fileName;
		doc->MasterP = false;
		HaveNewDoc();
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
		doc->setUnModified();
		doc->loading = false;
		view->GotoPage(0);
		doc->RePos = true;
		for (uint az=0; az<view->MasterPages.count(); az++)
			{
			for (uint azz=0; azz<view->MasterPages.at(az)->Items.count(); ++azz)
				{
				PageItem *ite = view->MasterPages.at(az)->Items.at(azz);
				if (ite->PType == 4)
					ite->paintObj();
				}
			}
		for (uint az=0; az<view->Pages.count(); az++)
			{
			for (uint azz=0; azz<view->Pages.at(az)->Items.count(); ++azz)
				{
				PageItem *ite = view->Pages.at(az)->Items.at(azz);
				if (ite->PType == 4)
					ite->paintObj();
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
		if (doc->OldBM)
			StoreBookmarks();
		doc->RePos = false;
		UpdateRecent(fileName);
  	FMess->setText(tr("Ready"));
		ret = true;
		if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
			w->showMaximized();
		else
			w->show();
		view->show();
		newActWin(w);
		connect(doc->ASaveTimer, SIGNAL(timeout()), this, SLOT(slotAutoSave()));
		doc->AutoSave = Prefs.AutoSave;
		if (doc->AutoSave)
			doc->ASaveTimer->start(Prefs.AutoSaveTime);
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
			QString form = "";
			for ( uint i = 0; i < QImageIO::inputFormats().count(); ++i )
				{
				form = QString(QImageIO::inputFormats().at(i)).lower();
				if (form == "jpeg")
					form = "jpg";
        formats += "*."+form+" ";
				}
			formats += "*.tif";
  		QString fileName = CFileDialog(tr("Open"),tr("Images")+" ("+formats+tr(");;Postscript (*.eps);;All Files (*)"), "", true);
  		if (!fileName.isEmpty())
  			{
  			b->EmProfile = "";
  			b->UseEmbedded = true;
  			b->IProfile = doc->CMSSettings.DefaultInputProfile;
				b->IRender = doc->CMSSettings.DefaultIntentMonitor2;
    		doc->ActPage->LoadPict(fileName, b->ItemNr);
    		doc->ActPage->AdjustPictScale(b);
    		doc->ActPage->AdjustPreview(b);
    		doc->ActPage->update();
				Mpal->Cpal->SetColors(doc->PageColors);
				slotDocCh();
  			}
  		}
  	if (b->PType == 4)
  		{
  		QString fileName = CFileDialog(tr("Open"),tr("Textfiles (*.txt);;All Files (*)"));
  		if (!fileName.isEmpty())
  			{
  			Serializer *ss = new Serializer(fileName);
  			if (ss->Read())
  				{
  				ss->GetText(b, doc->CurrentABStil);
  				}
  			delete ss;
				if (doc->Trenner->AutoCheck)
					doc->Trenner->slotHyphenate(b);
    		doc->ActPage->update();
				slotDocCh();
  			}
  		}
  	}
}

void ScribusApp::slotAutoSave()
{
  if ((doc->hasName) && (doc->isModified()) && (!doc->TemplateMode))
  	{
		system("mv -f " + doc->DocName + " " + doc->DocName+".bak");
		DoFileSave(doc->DocName);
  	}
}

void ScribusApp::slotFileSave()
{
  QString fn = doc->DocName;
	if (!DoFileSave(fn))
		QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
}

void ScribusApp::slotFileSaveAs()
{
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
  QString fn = CFileDialog(tr("Save as"), tr("Documents (*.sla *.sla.gz *.scd *scd.gz);; All Files (*)"), fna, false, false, true);
#else
  QString fn = CFileDialog(tr("Save as"), tr("Documents (*.sla *.scd);; All Files (*)"), fna, false, false, false);
#endif
  if (!fn.isEmpty())
  	{
  	QFile f(fn);
  	if (f.exists())
  		{
  		int exit=QMessageBox::warning(this,
  																	tr("Warning"),
  																	tr("Do you really want to overwrite the File:\n%1 ?").arg(fn),
                                		tr("No"),
                                		tr("Yes"),
                                		0, 0, 1);
  		if (exit != 1)
  			return;
  		}
		if (!DoFileSave(fn))
			QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
  	}
  FMess->setText(tr("Ready"));
}

bool ScribusApp::DoFileSave(QString fn)
{
	bool ret = true;
	FMess->setText(tr("Saving..."));
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
		DatSav->setEnabled(false);
		UpdateRecent(fn);
		}
	FMess->setText("");
	FProg->reset();
	return ret;
}

bool ScribusApp::slotFileClose()
{
	ScribusWin* tw = ActWin;
	ActWin->close();
	if (tw == ActWin)
		return false;
	else
		return true;
}

bool ScribusApp::DoFileClose()
{
  uint a;
  doc->ASaveTimer->stop();
	disconnect(doc->ASaveTimer, SIGNAL(timeout()), this, SLOT(slotAutoSave()));
	if ((doc->UnData.UnCode == 0) && (doc->UnDoValid))
		delete doc->UnData.Item;
  if (CMSavail)
  	ActWin->CloseCMSProfiles();
	Mpal->NewSel(-1);
  Mpal->UnsetDoc();
	Sepal->Vie = 0;
	Sepal->Rebuild();
  Mpal->Spal->SetFormats(0);
	if (doc->EditClip)
		Npal->doc = 0;
	BookPal->BView->clear();
	BookPal->BView->NrItems = 0;
	BookPal->BView->First = 1;
	BookPal->BView->Last = 0;
	if ((wsp->windowList().isEmpty()) || (wsp->windowList().count() == 1))
		{
		fileMenu->setItemEnabled(fid1, 0);
		importMenu->setItemEnabled(fid2, 0);
		exportMenu->setItemEnabled(fid3, 0);
		fileMenu->setItemEnabled(fid4, 0);
		fileMenu->setItemEnabled(fid5, 0);
		fileMenu->setItemEnabled(fid51, 0);
		fileMenu->setItemEnabled(fid6, 0);
		fileMenu->setItemEnabled(fid7, 0);
		exportMenu->setItemEnabled(fid8, 0);
		importMenu->setItemEnabled(fid2a, 0);
		fileMenu->setItemEnabled(fid9, 0);
		exportMenu->setItemEnabled(fid10, 0);
		fileMenu->setItemEnabled(fid11, 0);
		editMenu->setItemEnabled(edUndo, 0);
		editMenu->setItemEnabled(edid1, 0);
		editMenu->setItemEnabled(edid2, 0);
		editMenu->setItemEnabled(edid3, 0);
		editMenu->setItemEnabled(edid4, 0);
		editMenu->setItemEnabled(edid5, 0);
		editMenu->setItemEnabled(edid6, 0);
		extraMenu->setItemEnabled(hyph, 0);
		for (int a=0; a<6; ++a)
			{
			viewMenu->setItemEnabled(viewMenu->idAt(a), 0);
			}
		viewMenu->setItemEnabled(Markers, 0);
		viewMenu->setItemEnabled(FrameDr, 0);
		viewMenu->setItemEnabled(Bilder, 0);
		viewMenu->setItemEnabled(Ras, 0);
		viewMenu->setItemEnabled(uRas, 0);
		viewMenu->setItemChecked(uRas, false);
		viewMenu->setItemEnabled(Guide, 0);
		viewMenu->setItemEnabled(uGuide, 0);
		viewMenu->setItemChecked(uGuide, false);
		viewMenu->changeItem(Markers, tr("Hide Margins"));
		viewMenu->changeItem(FrameDr, tr("Hide Frames"));
		viewMenu->changeItem(Bilder, tr("Hide Images"));
		viewMenu->changeItem(Ras, tr("Show Grid"));
		viewMenu->changeItem(Guide, tr("Hide Guides"));
		editMenu->setItemEnabled(tman, 0);
		editMenu->setItemEnabled(jman, 0);
		menuBar()->setItemEnabled(pgmm, 0);
		menuBar()->setItemEnabled(exmn, 0);
		menuBar()->setItemEnabled(Obm, 0);
		WerkTools->setEnabled(false);
		ColorMenu->clear();
		Mpal->Cpal->SetColors(Prefs.DColors);
		Mpal->Cpal->ChooseGrad(0);
	  FMess->setText(tr("Ready"));
	  Tpal->ListView1->clear();
		Lpal->ClearInhalt();
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
  for (a = 0; a<view->Pages.count(); ++a)
  	{
  	delete view->Pages.at(a);
  	}
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
  int Anf, Ende, Nr;
  bool fil, sep, farbe, PSfile, mirrorH, mirrorV, useICC;
	PSfile = false;
  FMess->setText(tr("Printing..."));
  if (PrinterUsed)
  	{
  	prn = PDef.Pname;
  	fna = PDef.Dname;
  	}
  else
  	{
  	prn = "";
  	if (!doc->DocName.startsWith("Document"))
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
  Druck *printer = new Druck(this, fna, prn, scmd);
  printer->setMinMax(1, view->Pages.count());
  printer->setFromTo(1, view->Pages.count());
  if (printer->exec())
  	{
  	qApp->setOverrideCursor(QCursor(waitCursor), true);
  	prn = printer->printerName();
  	fna = printer->outputFileName();
  	fil = printer->outputToFile();
  	Anf = printer->fromPage();
  	Ende = printer->toPage();
  	Nr = printer->numCopies();
  	sep = printer->outputSeparations();
  	SepNam = printer->separationName();
  	farbe = printer->color();
    mirrorH = printer->MirrorH;
    mirrorV = printer->MirrorV;
    useICC = printer->ICCinUse;
  	PDef.Pname = prn;
  	PDef.Dname = fna;
		if (printer->OtherCom->isChecked())
			PDef.Command = printer->Command->text();
  	PrinterUsed = true;
		PSLib *dd = getPSDriver(true, Prefs.AvailFonts, doc->UsedFonts, doc->PageColors, false);
		if (dd != NULL)
			{
			if (fil)
				PSfile = dd->PS_set_file(fna);
			else
				PSfile = dd->PS_set_file(PrefsPfad+"/tmp.ps");
			if (PSfile)
				{
 				if (printer->pageOrder() == 0)
					view->CreatePS(dd, Anf-1, Ende, 1, sep, SepNam, farbe, mirrorH, mirrorV, useICC);
				else
					view->CreatePS(dd, Ende-1, Anf-2, -1, sep, SepNam, farbe, mirrorH, mirrorV, useICC);
				if (!fil)
					{
					if (printer->OtherCom->isChecked())
						{
						cmd = printer->Command->text()+ " "+PrefsPfad+"/tmp.ps";
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
						cmd += " "+PrefsPfad+"/tmp.ps";
						system(cmd);
						}
					system("rm -f "+PrefsPfad+"/tmp.ps");
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
	FMess->setText(tr("Ready"));
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
		if ((doc->AppMode == 7) && (b->HasSel))
			{
			for (a = 0; a < b->Ptext.count(); ++a)
				{
				if (b->Ptext.at(a)->cselect)
					{
					if (b->Ptext.at(a)->ch == QChar(13))
						{
						Buffer2 += QChar(5);
						BufferI += QChar(10);
						}
					else
						{
						Buffer2 += b->Ptext.at(a)->ch;
						BufferI += b->Ptext.at(a)->ch;
						}
					Buffer2 += "\t";
					Buffer2 += b->Ptext.at(a)->cfont+"\t";
					Buffer2 += QString::number(b->Ptext.at(a)->csize)+"\t";
					Buffer2 += b->Ptext.at(a)->ccolor+"\t";
					Buffer2 += QString::number(b->Ptext.at(a)->cextra)+"\t";
					Buffer2 += QString::number(b->Ptext.at(a)->cshade)+'\t';
					Buffer2 += QString::number(b->Ptext.at(a)->cstyle)+'\t';
					Buffer2 += QString::number(b->Ptext.at(a)->cab)+'\n';
					}
				}
			DeleteSel(b);
			b->Dirty = true;
			b->paintObj();
			}
		else
			{
  		ScriXmlDoc *ss = new ScriXmlDoc();
			Buffer2 = ss->WriteElem(&doc->ActPage->SelItem, doc);
			doc->ActPage->DeleteItem();
			}
		slotDocCh();
		editMenu->setItemEnabled(edid3, 1);
		ClipB->setText(BufferI);
		BuFromApp = true;
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
			for (a = 0; a < b->Ptext.count(); ++a)
				{
				if (b->Ptext.at(a)->cselect)
					{
					if (b->Ptext.at(a)->ch == QChar(13))
						{
						Buffer2 += QChar(5);
						BufferI += QChar(10);
						}
					else
						{
						Buffer2 += b->Ptext.at(a)->ch;
						BufferI += b->Ptext.at(a)->ch;
						}
					Buffer2 += "\t";
					Buffer2 += b->Ptext.at(a)->cfont+"\t";
					Buffer2 += QString::number(b->Ptext.at(a)->csize)+"\t";
					Buffer2 += b->Ptext.at(a)->ccolor+"\t";
					Buffer2 += QString::number(b->Ptext.at(a)->cextra)+"\t";
					Buffer2 += QString::number(b->Ptext.at(a)->cshade)+'\t';
					Buffer2 += QString::number(b->Ptext.at(a)->cstyle)+'\t';
					Buffer2 += QString::number(b->Ptext.at(a)->cab)+'\n';
					}
				}
			}
		else
			{
  		ScriXmlDoc *ss = new ScriXmlDoc();
			Buffer2 = ss->WriteElem(&doc->ActPage->SelItem, doc);
			delete ss;
			}
		editMenu->setItemEnabled(edid3, 1);
		ClipB->setText(BufferI);
		BuFromApp = true;
		}
}

void ScribusApp::slotEditPaste()
{
	struct Pti *hg;
	NoFrameEdit();
	if (HaveDoc)
		{
		if (!BuFromApp)
			Buffer2 = ClipB->text(QClipboard::Clipboard);
		if (Buffer2.isNull())
			return;
		if (doc->AppMode == 7)
			{
			PageItem *b = doc->ActPage->SelItem.at(0);
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
  			ss->GetText(b, doc->CurrentABStil, true);
  			delete ss;
				if (doc->Trenner->AutoCheck)
					doc->Trenner->slotHyphenate(b);
				}
 			if (b->CPos < static_cast<int>(b->Ptext.count())) { b->Dirty = true; }
 			b->paintObj();
			}
		else
			{
			if (Buffer2.startsWith("<SCRIBUSELEM"))
				{
				doc->ActPage->Deselect(true);
				slotElemRead(Buffer2, 0, 0, false, true, doc);
				doc->ActPage->SelectItemNr(doc->ActPage->Items.count()-1);
				PageItem *b = doc->ActPage->SelItem.at(0);				
				b->AnName = tr("Copy of")+" "+b->AnName;
				}
			}
		slotDocCh(false);
		}
}

void ScribusApp::SelectAll()
{
	uint a;
	PageItem *b = doc->ActPage->SelItem.at(0);
	for (a = 0; a < b->Ptext.count(); ++a)
		{
		b->Ptext.at(a)->cselect = true;
		}
	b->HasSel = true;
	b->Dirty = true;
	b->paintObj();
	EnableTxEdit();
}

void ScribusApp::ClipChange()
{
	QString cc;
	cc = ClipB->text(QClipboard::Clipboard);
	editMenu->setItemEnabled(edid3, 0);
	if (!cc.isNull())
		{
		BuFromApp = false;
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

void ScribusApp::DeleteText()
{
	PageItem *b = doc->ActPage->SelItem.at(0);
	DeleteSel(b);
	b->Dirty = true;
	b->paintObj();
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
	char *error;
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
		std::cout << "Can't find Plugin" << endl;
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
  QMessageBox::aboutQt(this,tr("About Qt"));
}

void ScribusApp::slotOnlineHelp()
{
	HelpBrowser *dia = new HelpBrowser(this, tr("Scribus Manual"));
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
  QString fn = CFileDialog(tr("Save as"), tr("Textfiles (*.txt);;All Files (*)"), "", false, false);
  if (!fn.isEmpty())
  	{
    Serializer *se = new Serializer(fn);
    se->PutText(doc->ActPage->SelItem.at(0));
    se->Write();
    delete se;
  	}
}

void ScribusApp::applyNewMaster(QString name)
{
	Apply_Temp(name, doc->ActPage->PageNr);
}

void ScribusApp::slotNewPageP(int wo, QString templ)
{
	NoFrameEdit();
	doc->ActPage->Deselect(true);
	doc->UnDoValid = false;
	CanUndo();
	slotNewPage(wo);
	applyNewMaster(templ);
	Sepal->RebuildPage();
}

/** Erzeugt eine neue Seite */
void ScribusApp::slotNewPageM()
{
	int wo, cc;
	NoFrameEdit();
	doc->ActPage->Deselect(true);
	InsPage *dia = new InsPage(this, view, doc->ActPage->PageNr, view->Pages.count(), doc->PageFP);
	if (dia->exec())
		{
		doc->UnDoValid = false;
		CanUndo();
		wo = dia->ActualPage->value();
		switch (dia->Where->currentItem())
			{
			case 0:
				wo -= 1;
				for (cc = 0; cc < dia->NumPages->value(); ++cc)
					{
					slotNewPage(wo);
					if (doc->PageFP)
						{
 						if ((doc->ActPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
							applyNewMaster(dia->Based->currentText());
 						if ((doc->ActPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
							applyNewMaster(dia->Based2->currentText());
 						if ((doc->ActPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
							applyNewMaster(dia->Based2->currentText());
 						if ((doc->ActPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
							applyNewMaster(dia->Based->currentText());
						}
					else
						applyNewMaster(dia->Based->currentText());
					wo ++;
					}
				break;
			case 1:
				for (cc = 0; cc < dia->NumPages->value(); ++cc)
					{
					slotNewPage(wo);
					if (doc->PageFP)
						{
 						if ((doc->ActPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
							applyNewMaster(dia->Based->currentText());
 						if ((doc->ActPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
							applyNewMaster(dia->Based2->currentText());
 						if ((doc->ActPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
							applyNewMaster(dia->Based2->currentText());
 						if ((doc->ActPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
							applyNewMaster(dia->Based->currentText());
						}
					else
						applyNewMaster(dia->Based->currentText());
					wo ++;
					}
				break;
			case 2:
				for (cc = 0; cc < dia->NumPages->value(); ++cc)
					{
					slotNewPage(view->Pages.count());
					if (doc->PageFP)
						{
 						if ((doc->ActPage->PageNr % 2 == 0) && (doc->FirstPageLeft))
							applyNewMaster(dia->Based->currentText());
 						if ((doc->ActPage->PageNr % 2 == 1) && (doc->FirstPageLeft))
							applyNewMaster(dia->Based2->currentText());
 						if ((doc->ActPage->PageNr % 2 == 0) && (!doc->FirstPageLeft))
							applyNewMaster(dia->Based2->currentText());
 						if ((doc->ActPage->PageNr % 2 == 1) && (!doc->FirstPageLeft))
							applyNewMaster(dia->Based->currentText());
						}
					else
						applyNewMaster(dia->Based->currentText());
					}
				break;
			}
		Sepal->RebuildPage();
		}
	delete dia;
}

void ScribusApp::slotNewPageT(int w)
{
	if (doc->TemplateMode)
		slotNewPage(w);
}

void ScribusApp::slotNewPage(int w)
{
	view->addPage(w);
	if (view->Pages.count() > 1)
		{
		pageMenu->setItemEnabled(pgmd, 1);
		pageMenu->setItemEnabled(pgmv, 1);
		}
	AdjustBM();
	connect(doc->ActPage, SIGNAL(Amode(int)), this, SLOT(setAppMode(int)));
	connect(doc->ActPage, SIGNAL(PaintingDone()), this, SLOT(slotSelect()));
	connect(doc->ActPage, SIGNAL(HaveSel(int)), this, SLOT(HaveNewSel(int)));
	connect(doc->ActPage, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
	connect(doc->ActPage, SIGNAL(ClipPo(float, float)), Npal, SLOT(SetXY(float, float)));
	connect(doc->ActPage, SIGNAL(HavePoint(bool, bool)), Npal, SLOT(HaveNode(bool, bool)));
	connect(doc->ActPage, SIGNAL(PolyOpen()), Npal, SLOT(IsOpen()));
	connect(doc->ActPage, SIGNAL(PStatus(int, uint)), Npal, SLOT(PolyStatus(int, uint)));
	connect(doc->ActPage, SIGNAL(MousePos(float, float)), this, SLOT(ReportMP(float, float)));
	connect(doc->ActPage, SIGNAL(ItemPos(float, float)), Mpal, SLOT(setXY(float, float)));
	connect(doc->ActPage, SIGNAL(ItemGeom(float, float)), Mpal, SLOT(setBH(float, float)));
	connect(doc->ActPage, SIGNAL(SetAngle(float)), Mpal, SLOT(setR(float)));
	connect(doc->ActPage, SIGNAL(ItemRadius(float)), Mpal, SLOT(setRR(float)));
	connect(doc->ActPage, SIGNAL(ItemTextAttr(float)), Mpal, SLOT(setLsp(float)));
	connect(doc->ActPage, SIGNAL(ItemTextSize(int)), Mpal, SLOT(setSize(int)));
	connect(doc->ActPage, SIGNAL(ItemTextUSval(float)), Mpal, SLOT(setExtra(float)));
	connect(doc->ActPage, SIGNAL(HaveSel(int)), Mpal, SLOT(NewSel(int)));
	connect(doc->ActPage, SIGNAL(SetLocalValues(float, float, float, float)), Mpal, SLOT(setLvalue(float, float, float, float)));
	connect(doc->ActPage, SIGNAL(SetSizeValue(float)), Mpal, SLOT(setSvalue(float)));
	connect(doc->ActPage, SIGNAL(ItemTextStil(int)), Mpal, SLOT(setStil(int)));
	connect(doc->ActPage, SIGNAL(ItemTextAbs(int)), Mpal, SLOT(setAli(int)));
	connect(doc->ActPage, SIGNAL(RotMode(int)), Mpal, SLOT(setRM(int)));
	connect(doc->ActPage, SIGNAL(SetLineArt(PenStyle, PenCapStyle, PenJoinStyle)), Mpal, SLOT( setLIvalue(PenStyle, PenCapStyle, PenJoinStyle)));
	connect(doc->ActPage, SIGNAL(ItemFarben(QString, QString, int, int)), this, SLOT(setCSMenu(QString, QString, int, int)));
	connect(doc->ActPage, SIGNAL(ItemFarben(QString, QString, int, int)), Mpal->Cpal, SLOT(setActFarben(QString, QString, int, int)));
	connect(doc->ActPage, SIGNAL(ItemGradient(QString, QString, int, int, int)), Mpal->Cpal, SLOT(setActGradient(QString, QString, int, int, int)));
	connect(doc->ActPage, SIGNAL(ItemTrans(float)), Mpal->Cpal, SLOT(setActTrans(float)));
	connect(doc->ActPage, SIGNAL(ItemTextFont(QString)), this, SLOT(AdjustFontMenu(QString)));
	connect(doc->ActPage, SIGNAL(ItemTextSize(int)), this, SLOT(setFSizeMenu(int)));
	connect(doc->ActPage, SIGNAL(ItemTextStil(int)), this, SLOT(setStilvalue(int)));
	connect(doc->ActPage, SIGNAL(ItemTextAbs(int)), this, SLOT(setAbsValue(int)));
	connect(doc->ActPage, SIGNAL(HasTextSel()), this, SLOT(EnableTxEdit()));
	connect(doc->ActPage, SIGNAL(HasNoTextSel()), this, SLOT(DisableTxEdit()));
	connect(doc->ActPage, SIGNAL(CopyItem()), this, SLOT(slotEditCopy()));
	connect(doc->ActPage, SIGNAL(CutItem()), this, SLOT(slotEditCut()));
	connect(doc->ActPage, SIGNAL(LoadPic()), this, SLOT(slotFileOpen()));
	connect(doc->ActPage, SIGNAL(ModifyIt()), this, SLOT(ModifyObject()));
	connect(doc->ActPage, SIGNAL(AnnotProps()), this, SLOT(ModifyAnnot()));
	connect(doc->ActPage, SIGNAL(ToScrap(QString)), this, SLOT(PutScrap(QString)));
	connect(doc->ActPage, SIGNAL(UndoAvail()), this, SLOT(CanUndo()));
	connect(doc->ActPage, SIGNAL(EditGuides()), this, SLOT(ManageGuides()));
	connect(doc->ActPage, SIGNAL(LoadElem(QString, int ,int, bool, bool, ScribusDoc *)), this, SLOT(slotElemRead(QString, int, int, bool, bool, ScribusDoc *)));
	connect(doc->ActPage, SIGNAL(AddBM(PageItem *)), this, SLOT(AddBookMark(PageItem *)));
	connect(doc->ActPage, SIGNAL(DelBM(PageItem *)), this, SLOT(DelBookMark(PageItem *)));
	connect(doc->ActPage, SIGNAL(ChBMText(PageItem *)), this, SLOT(BookMarkTxT(PageItem *)));
	connect(doc->ActPage, SIGNAL(NewBMNr(int, int)), BookPal->BView, SLOT(ChangeItem(int, int)));
	slotDocCh(!doc->loading);
}

/** Ansicht absolut zoomen */
void ScribusApp::slotZoomAbs(float z)
{
	doc->Scale = z;
	view->slotDoZoom();
}

void ScribusApp::slotZoomFit()
{
	float dx = (view->width()-50) / (doc->PageB+30);
	float dy = (view->height()-70) / (doc->PageH+30);
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

void ScribusApp::ToggleMarks()
{
	if (doc->Marks)
	{
		doc->Marks = false;
		viewMenu->changeItem(Markers, tr("Show Margins"));
	}
	else
	{
		doc->Marks = true;
		viewMenu->changeItem(Markers, tr("Hide Margins"));
	}
	view->DrawNew();
}

void ScribusApp::ToggleFrames()
{
	if (doc->ShFrames)
	{
		doc->ShFrames = false;
		viewMenu->changeItem(FrameDr, tr("Show Frames"));
	}
	else
	{
		doc->ShFrames = true;
		viewMenu->changeItem(FrameDr, tr("Hide Frames"));
	}
	view->DrawNew();
}

void ScribusApp::ToggleMpal()
{
	if (Mpal->isVisible())
	{
  	Prefs.Mpalx = Mpal->pos().x();
  	Prefs.Mpaly = Mpal->pos().y();
		Mpal->hide();
		toolMenu->changeItem(viewMpal, tr("Show Measurements"));
	}
	else
	{
		Mpal->show();
		Mpal->TabStack->raiseWidget(0);
		toolMenu->changeItem(viewMpal, tr("Hide Measurements"));
	}
}

void ScribusApp::ToggleTpal()
{
	if (Tpal->isVisible())
		{
  	Prefs.Tpalx = Tpal->pos().x();
  	Prefs.Tpaly = Tpal->pos().y();
		Tpal->close();
		toolMenu->changeItem(viewTpal, tr("Show Outline"));
		}
	else
		{
		if (HaveDoc)
			Tpal->BuildTree(view);
		Tpal->show();
		toolMenu->changeItem(viewTpal, tr("Hide Outline"));
		}
}

void ScribusApp::ToggleBpal()
{
	if (ScBook->isVisible())
		{
  	Prefs.SCpalx = ScBook->pos().x();
  	Prefs.SCpaly = ScBook->pos().y();
  	Prefs.SCpalw = ScBook->size().width();
  	Prefs.SCpalh = ScBook->size().height();
		ScBook->close();
		toolMenu->changeItem(viewBpal, tr("Show Scrapbook"));
		}
	else
		{
		ScBook->show();
		toolMenu->changeItem(viewBpal, tr("Hide Scrapbook"));
		}
}

void ScribusApp::ToggleLpal()
{
	if (Lpal->isVisible())
		{
  	Prefs.Lpalx = Lpal->pos().x();
  	Prefs.Lpaly = Lpal->pos().y();
		Lpal->close();
		toolMenu->changeItem(viewLpal, tr("Show Layers"));
		}
	else
		{
		if (HaveDoc)
			Lpal->setLayers(&doc->Layers, &doc->ActiveLayer);
		Lpal->show();
		toolMenu->changeItem(viewLpal, tr("Hide Layers"));
		}
}

void ScribusApp::ToggleSepal()
{
	if (Sepal->isVisible())
		{
  	Prefs.Sepalx = Sepal->pos().x();
  	Prefs.Sepaly = Sepal->pos().y();
		Prefs.SepalT = Sepal->TemplList->Thumb;
		Prefs.SepalN = Sepal->PageView->Namen;
		Sepal->close();
		toolMenu->changeItem(viewSepal, tr("Show Page Palette"));
		}
	else
		{
		Sepal->show();
		toolMenu->changeItem(viewSepal, tr("Hide Page Palette"));
		}
}

void ScribusApp::ToggleBookpal()
{
	if (BookPal->isVisible())
	{
  	Prefs.Bopalx = BookPal->pos().x();
  	Prefs.Bopaly = BookPal->pos().y();
		BookPal->hide();
		toolMenu->changeItem(viewBopal, tr("Show Bookmarks"));
	}
	else
	{
		BookPal->show();
		toolMenu->changeItem(viewBopal, tr("Hide Bookmarks"));
	}
}

void ScribusApp::ToggleTools()
{
	if (WerkTools->Sichtbar)
	{
		WerkTools->hide();
		WerkTools->Sichtbar = false;
		toolMenu->changeItem(viewTools, tr("Show Tools"));
	}
	else
	{
		WerkTools->show();
		WerkTools->Sichtbar = true;
		toolMenu->changeItem(viewTools, tr("Hide Tools"));
	}
}

void ScribusApp::TogglePics()
{
	uint a, b;
	if (doc->ShowPic)
		{
		viewMenu->changeItem(Bilder, tr("Show Images"));
		doc->ShowPic = false;
		}
	else
		{
		viewMenu->changeItem(Bilder, tr("Hide Images"));
		doc->ShowPic = true;
		}
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

void ScribusApp::ToggleRaster()
{
	uint a;
	if (doc->Raster)
		{
		viewMenu->changeItem(Ras, tr("Show Grid"));
		doc->Raster = false;
		}
	else
		{
		viewMenu->changeItem(Ras, tr("Hide Grid"));
		doc->Raster = true;
		}
	for (a=0; a<view->Pages.count(); ++a)
		{
		view->Pages.at(a)->update();
		}
}

void ScribusApp::ToggleURaster()
{
	doc->useRaster = !doc->useRaster;
	viewMenu->setItemChecked(uRas, doc->useRaster);
}

void ScribusApp::ToggleGuides()
{
	uint a;
	if (doc->Guides)
		{
		viewMenu->changeItem(Guide, tr("Show Guides"));
		doc->Guides = false;
		}
	else
		{
		viewMenu->changeItem(Guide, tr("Hide Guides"));
		doc->Guides = true;
		}
	for (a=0; a<view->Pages.count(); ++a)
		{
		view->Pages.at(a)->update();
		}
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
		doc->EditClipMode = 0;
		doc->EditClip = true;
		WerkTools->Select->setEnabled(false);
		WerkTools->Rotiere->setEnabled(false);
		WerkTools->Textedit->setEnabled(false);
		WerkTools->Zoom->setEnabled(false);
		WerkTools->Texte->setEnabled(false);
		WerkTools->BildB->setEnabled(false);
		WerkTools->Rechteck->setEnabled(false);
		WerkTools->Kreis->setEnabled(false);
		WerkTools->Linien->setEnabled(false);
		WerkTools->Polygon->setEnabled(false);
		WerkTools->PolyLin->setEnabled(false);
		WerkTools->KetteEin->setEnabled(false);
		WerkTools->KetteAus->setEnabled(false);
		WerkTools->PDFTool->setEnabled(false);
		WerkTools->PDFaTool->setEnabled(false);
		ObjMenu->setItemEnabled(Loesch, false);
		if (doc->ActPage->SelItem.count() != 0)
			{
			doc->ActPage->MarkClip(doc->ActPage->SelItem.at(0));
			Npal->PolyStatus(doc->ActPage->SelItem.at(0)->PType, doc->ActPage->SelItem.at(0)->PoLine.size());
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
	WerkTools->Rechteck->setEnabled(true);
	WerkTools->Kreis->setEnabled(true);
	WerkTools->Linien->setEnabled(true);
	WerkTools->Polygon->setEnabled(true);
	WerkTools->PolyLin->setEnabled(true);
	WerkTools->PDFTool->setEnabled(true);
	WerkTools->PDFaTool->setEnabled(true);
	WerkTools->Textedit->setOn(false);
	ObjMenu->setItemEnabled(Loesch, true);
	ShapeMenu->setItemChecked(ShapeEdit, false);
	if (HaveDoc)
		{
		doc->EditClip = false;
		if (doc->ActPage->SelItem.count() != 0)
			{
			HaveNewSel(doc->ActPage->SelItem.at(0)->PType);
			doc->ActPage->RefreshItem(doc->ActPage->SelItem.at(0));
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
	WerkTools->Zoom->setOn(false);
	WerkTools->Texte->setOn(false);
	WerkTools->BildB->setOn(false);
	WerkTools->Rechteck->setOn(false);
	WerkTools->Kreis->setOn(false);
	WerkTools->Linien->setOn(false);
	WerkTools->Polygon->setOn(false);
	WerkTools->PolyLin->setOn(false);
	WerkTools->KetteEin->setOn(false);
	WerkTools->KetteAus->setOn(false);
	WerkTools->PDFTool->setOn(false);
	WerkTools->PDFaTool->setOn(false);
  setAppMode(1);
}

void ScribusApp::ModeFromTB(int m)
{
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
		if (oldMode == 7)
			{
			disconnect(CurTimer, SIGNAL(timeout()), doc->ActPage, SLOT(BlinkCurs()));
			CurTimer->stop();
			delete CurTimer;
			menuBar()->setItemEnabled(Obm, 1);
			editMenu->setItemEnabled(edid4, 0);
			editMenu->setItemEnabled(edid5, 0);
			doc->ActPage->slotDoCurs(false);
			if (b != 0)
				doc->ActPage->RefreshItem(b);
			}
		if (mode == 7)
			{
			if (b != 0)
				{
				if ((b->PType == 6) || (b->PType == 7) || (b->PType == 8))
					{
					doc->AppMode = 1;
					ToggleFrameEdit();
					return;
					}
				}
			QString cc;
			cc = ClipB->text(QClipboard::Clipboard);
			editMenu->setItemEnabled(edid3, 0);
			if (!cc.isNull())
				{
				if (!cc.startsWith("<SCRIBUSELEM"))
					{
					BuFromApp = false;
					editMenu->setItemEnabled(edid3, 1);
					}
				}
  		WerkTools->Select->setOn(false);
			WerkTools->Textedit->setOn(true);
			doc->ActPage->slotDoCurs(true);
			menuBar()->setItemEnabled(Obm, 0);
			CurTimer = new QTimer(doc->ActPage);
			connect(CurTimer, SIGNAL(timeout()), doc->ActPage, SLOT(BlinkCurs()));
			CurTimer->start(500);
			if (b != 0)
				{
				if (b->HasSel)
					{
					editMenu->setItemEnabled(edid1, 1);
					editMenu->setItemEnabled(edid2, 1);
					editMenu->setItemEnabled(edid4, 1);
					editMenu->setItemEnabled(edid5, 1);
					}
				else
					{
					editMenu->setItemEnabled(edid1, 0);
					editMenu->setItemEnabled(edid2, 0);
					editMenu->setItemEnabled(edid4, 0);
					editMenu->setItemEnabled(edid5, 1);
					}
				doc->ActPage->RefreshItem(b);
				}
			}
		if (mode == 13)
			{
			if (doc->ActPage->SelItem.count() != 0)
				doc->ActPage->Deselect(true);
			}
		if (mode == 6)
			qApp->setOverrideCursor(QCursor(loadIcon("LupeZ.xpm")), true);
		else
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		}
}

void ScribusApp::Aktiv()
{
	setActiveWindow();
	raise();
}

void ScribusApp::setItemTypeStyle(int id)
{
	int a = TypeStyleMenu->indexOf(id);
	int b = 0;
	switch (a)
		{
		case 0:
			b = 4;
			break;
		case 1:
			b = 8;
			break;
		case 2:
			b = 16;
			break;
		case 3:
			b = 64;
			break;
		case 4:
			b = 1;
			break;
		case 5:
			b = 2;
			break;
		}
	setItemHoch(b);
}

void ScribusApp::setStilvalue(int s)
{
	uint a;
	doc->CurrentStyle = s;
	for (a = 0; a < TypeStyleMenu->count(); ++a)
		{
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(a), false);
		}
	if (s & 4)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(0), true);
	if (s & 8)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(1), true);
	if (s & 16)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(2), true);
	if (s & 64)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(3), true);
	if (s & 1)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(4), true);
	if (s & 2)
		TypeStyleMenu->setItemChecked(TypeStyleMenu->idAt(5), true);
	emit TextStil(s);
}

void ScribusApp::setItemHoch(int h)
{
	if (doc->ActPage->SelItem.count() != 0)
		{
		setActiveWindow();
		if (h == 0)
			doc->CurrentStyle = 0;
		else
			{
			doc->CurrentStyle = doc->CurrentStyle ^ h;
			if ((h == 1) && (doc->CurrentStyle & 2))
				doc->CurrentStyle = doc->CurrentStyle & ~2;
			if ((h == 2) && (doc->CurrentStyle & 1))
				doc->CurrentStyle = doc->CurrentStyle & ~1;
			}
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
	NoFrameEdit();
	if (view->Pages.count() == 1)
		return;
	for (uint d = 0; d < view->Pages.at(pg)->Items.count(); ++d)
		{
		if (view->Pages.at(pg)->Items.at(d)->isBookmark)
			DelBookMark(view->Pages.at(pg)->Items.at(d));
		}
	view->delPage(pg);
	AdjustBM();
	view->reformPages();
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
	NoFrameEdit();
	DelPages *dia = new DelPages(this, doc->ActPage->PageNr+1, view->Pages.count());
	if (dia->exec())
		{
		pg = dia->FromPage->value()-1;
		for (a = pg; a < dia->ToPage->value(); ++a)
			{
			for (uint d = 0; d < view->Pages.at(pg)->Items.count(); ++d)
				{
				if (view->Pages.at(pg)->Items.at(d)->isBookmark)
					DelBookMark(view->Pages.at(pg)->Items.at(d));
				}
			view->delPage(pg);
			AdjustBM();
			}
		view->reformPages();
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
		}
	delete dia;
}

void ScribusApp::CopyPage()
{
	NoFrameEdit();
	MovePages *dia = new MovePages(this, doc->ActPage->PageNr+1, view->Pages.count(), false);
	if (dia->exec())
		{
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
		for (uint ite = 0; ite < from->Items.count(); ++ite)
			{
			from->Items.at(ite)->CopyIt(&Buffer);
			Ziel->PasteItem(&Buffer, true);
			if (from->Items.at(ite)->isBookmark)
				AddBookMark(Ziel->Items.at(Ziel->Items.count()-1));
			}
		Ziel->MPageNam = from->MPageNam;
		Ziel->Deselect(true);
		view->DrawNew();
		slotDocCh();
		doc->UnDoValid = false;
		CanUndo();
		Sepal->RebuildPage();
		AdjustBM();
		}
	delete dia;
}

void ScribusApp::setItemFont(int id)
{
	QString nf;
	nf = *FontID[long(id)];
	SetNewFont(nf);
}

void ScribusApp::SetNewFont(QString nf)
{
	Aktiv();
	int a;
	if (!doc->UsedFonts.contains(nf))
		{
		doc->AddFont(nf, Prefs.AvailFonts[nf]->Font);
		a = FontMenu->insertItem(new FmItem(nf, Prefs.AvailFonts[nf]->Font));
		FontID.insert(static_cast<long>(a), &Prefs.AvailFonts[nf]->SCName);
		}
	AdjustFontMenu(nf);
	doc->ActPage->ItemFont(nf);
	slotDocCh();
}

void ScribusApp::AdjustFontMenu(QString nf)
{
	uint a;
	QString df;
	Mpal->Fonts->setText(nf);
	for (a = 2; a < FontMenu->count(); ++a)
		{
		df = *FontID[long(FontMenu->idAt(a))];
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
		doc->ActPage->chFSize(SizeTMenu->text(id).left(2).toInt());
	else
		{
    Query* dia = new Query(this, "New", 1, 0, "Size:", "Size");
    if (dia->exec())
    	{
			c = dia->Answer->text().toInt(&ok);
			if ((ok) && (c < 513) && (c > 0))
				doc->ActPage->chFSize(c);
			delete dia;
     	}
		}
	slotDocCh();
}

void ScribusApp::setFSizeMenu(int size)
{
	uint a;
	for (a = 0; a < SizeTMenu->count(); ++a)
		{
		SizeTMenu->setItemChecked(SizeTMenu->idAt(a), false);
		if (SizeTMenu->text(SizeTMenu->idAt(a)).left(2).toInt() == size)
			{
			SizeTMenu->setItemChecked(SizeTMenu->idAt(a), true);
			}
		}
}

void ScribusApp::setItemFarbe(int id)
{
	uint a;
	for (a = 0; a < ColorMenu->count(); ++a)
		{
		ColorMenu->setItemChecked(ColorMenu->idAt(a), false);
		}
	ColorMenu->setItemChecked(id, true);
	doc->ActPage->ItemBrush(ColorMenu->text(id), true);
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
	if (c > 0)
		{		
		doc->ActPage->ItemBrushShade((c-1) * 10, true);
		}
	else
		{
    Query* dia = new Query(this, "New", 1, 0, "Shade:", "Shade");
    if (dia->exec())
    	{
			c = dia->Answer->text().toInt(&ok);
			if (ok)
				{		
				doc->ActPage->ItemBrushShade(c);
				}
			delete dia;
     	}
		}
	slotDocCh();
}

void ScribusApp::setCSMenu(QString k, QString l, int lk , int ls)
{
	uint a;
	QString la;
	int lb;
	if (doc->ActPage->SelItem.count() != 0)
		{
		if (doc->ActPage->SelItem.at(0)->PType == 4)
			{
			la = k;
			lb = lk;
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
	for (a = 0; a < ColorMenu->count(); ++a)
		{
		ColorMenu->setItemChecked(ColorMenu->idAt(a), false);
		if (ColorMenu->text(ColorMenu->idAt(a)) == la)
			ColorMenu->setItemChecked(ColorMenu->idAt(a), true);
		}
	for (a = 0; a < ShadeMenu->count(); ++a)
		{
		ShadeMenu->setItemChecked(ShadeMenu->idAt(a), false);
		}
	ShadeMenu->setItemChecked(ShadeMenu->idAt(lb/10+1), true);
}

void ScribusApp::slotEditStyles()
{
	QValueList<uint> ers;
	QString nn;
	bool ff;
	uint nr;
	ers.clear();
	if (HaveDoc)
		{
		StilFormate *dia = new StilFormate(this, doc, &Prefs.AvailFonts);
		if (dia->exec())
			{
			for (uint a=0; a<doc->Vorlagen.count(); ++a)
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
					ers.append(0);
				}
			doc->Vorlagen = dia->TempVorl;
			for (uint c=0; c<view->Pages.count(); ++c)
				{
				for (uint d=0; d<view->Pages.at(c)->Items.count(); ++d)
					{
					if (view->Pages.at(c)->Items.at(d)->PType == 4)
						{
						for (uint e=0; e<view->Pages.at(c)->Items.at(d)->Ptext.count(); ++e)
							{
							if (view->Pages.at(c)->Items.at(d)->Ptext.at(e)->cab > 4)
								view->Pages.at(c)->Items.at(d)->Ptext.at(e)->cab = ers[view->Pages.at(c)->Items.at(d)->Ptext.at(e)->cab];
							if (doc->Vorlagen[view->Pages.at(c)->Items.at(d)->Ptext.at(e)->cab].Font != "")
								{
								view->Pages.at(c)->Items.at(d)->Ptext.at(e)->cfont = doc->Vorlagen[view->Pages.at(c)->Items.at(d)->Ptext.at(e)->cab].Font;
								view->Pages.at(c)->Items.at(d)->Ptext.at(e)->csize = doc->Vorlagen[view->Pages.at(c)->Items.at(d)->Ptext.at(e)->cab].FontSize;
								}
							}
						}
					}
				}
			for (uint a=0; a<doc->Vorlagen.count(); ++a)
				{
				if (doc->Vorlagen[a].Font != "")
					SetNewFont(doc->Vorlagen[a].Font);
				}
			Mpal->Spal->updateFList();
			view->DrawNew();
			slotDocCh();
			}
		delete dia;
		}
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
	if (a < 5)
		{
		Mpal->Spal->setCurrentItem(0);
		AliMenu->setItemChecked(AliMenu->idAt(a), true);
		}
	else
		Mpal->Spal->setCurrentItem(a-4);
}

void ScribusApp::slotEditColors()
{
	int a;
	uint b, c, d;
	CListe edc;
	QMap<QString,QString> ers;
	if (HaveDoc)
		{
		edc = doc->PageColors;
		}
	else
		{
		edc = Prefs.DColors;
		}
	Farbmanager* dia = new Farbmanager(this, edc);
	if (dia->exec())
		{
		if (HaveDoc)
			{
			slotDocCh();
			doc->PageColors = dia->EditColors;
			Mpal->Cpal->SetColors(doc->PageColors);
			CListe::Iterator it;
			ColorMenu->clear();
			QPixmap pm = QPixmap(15, 15);
			a = 0;
			ColorMenu->insertItem(tr("None"));
			for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
				{
				pm.fill(doc->PageColors[it.key()].getRGBColor());
				ColorMenu->insertItem(QIconSet(pm), it.key());
				if (it.key() == doc->Dbrush)
					{
					ColorMenu->setItemChecked(ColorMenu->idAt(a), true);
					}
				a++;
				}
			ers = dia->Ersatzliste;
			if (!ers.isEmpty())
				{
				for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
					{
					ers.remove(it.key());
					}
				if (!ers.isEmpty())
					{
					QMap<QString,QString>::Iterator it;
					for (it = ers.begin(); it != ers.end(); ++it)
						{
						for (b=0; b<view->Pages.count(); ++b)
							{
							for (c=0; c<view->Pages.at(b)->Items.count(); ++c)
								{
								if (view->Pages.at(b)->Items.at(c)->PType == 4)
									{
									for (d=0; d<view->Pages.at(b)->Items.at(c)->Ptext.count(); ++d)
										{
										if (it.key() == view->Pages.at(b)->Items.at(c)->Ptext.at(d)->ccolor)
											{
											view->Pages.at(b)->Items.at(c)->Ptext.at(d)->ccolor = it.data();
											}
										}
									}
								if (it.key() == view->Pages.at(b)->Items.at(c)->Pcolor)
									{
									view->Pages.at(b)->Items.at(c)->Pcolor = it.data();
									}
								if (it.key() == view->Pages.at(b)->Items.at(c)->Pcolor2)
									{
									view->Pages.at(b)->Items.at(c)->Pcolor2 = it.data();
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
			Mpal->Cpal->SetColors(Prefs.DColors);
			}
		}
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

void ScribusApp::setGradFill(int typ, QString col1, int sh1, QString col2, int sh2)
{
	setActiveWindow();
	if (HaveDoc)
		{
		doc->ActPage->ItemGradFill(typ, col1, sh1, col2, sh2);
		slotDocCh();
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

void ScribusApp::RectFrame()
{
	doc->ActPage->SetFrameRect();
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::RoundedFrame()
{
	doc->ActPage->SetFrameRounded();
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::OvalFrame()
{
	doc->ActPage->SetFrameOval();
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
		float dH = static_cast<float>(dia->ShiftH->value()) / UmReFaktor / 100.0;
		float dV = static_cast<float>(dia->ShiftV->value()) / UmReFaktor / 100.0;
		int a;
		if (anz>0)
			{
			for (a=0; a<anz; ++a)
				{
				slotEditCopy();
				doc->ActPage->Deselect(true);
				slotEditPaste();
				for (uint b=0; b<doc->ActPage->SelItem.count(); ++b)
					{
					doc->ActPage->SelItem.at(b)->Locked = false;
					doc->ActPage->MoveItem(dH, dV, doc->ActPage->SelItem.at(b));
					}
				DispX = dH;
				DispY = dV;
				}
			slotDocCh();
			doc->UnDoValid = false;
			CanUndo();
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
	DocInfos *dia = new DocInfos(this, doc->DocAutor, doc->DocTitel, doc->DocComments);
	if (dia->exec())
		{
		doc->DocAutor = dia->AutorInfo->text();
		doc->DocTitel = dia->TitelInfo->text();
		doc->DocComments = dia->MultiLineEdit1->text();
		slotDocCh();
		}
	delete dia;
}

void ScribusApp::ObjektAlign()
{
	float xdp, ydp;
	bool xa, ya, Vth, Vtv;
	int xart, yart, ein;
	if (HaveDoc)
		ein = doc->Einheit;
	else
		ein = Prefs.Einheit;
	NoFrameEdit();
	Align *dia = new Align(this, doc->ActPage->SelItem.count(), ein);
	connect(dia, SIGNAL(ApplyDist(bool, bool, bool, bool, float, float, int, int)),
	        this, SLOT(DoAlign(bool, bool, bool, bool, float, float, int, int)));
	if (dia->exec())
		{
		xdp = float(dia->AHor->value()) / UmReFaktor / 100.0;
		xa = (dia->CheckH->isChecked() || dia->VerteilenH->isChecked());
		ydp = float(dia->AVert->value()) / UmReFaktor/ 100.0;
		ya = (dia->CheckV->isChecked() || dia->VerteilenV->isChecked());
		xart = dia->VartH->currentItem();
		yart = dia->VartV->currentItem();
		Vth = dia->VerteilenH->isChecked();
		Vtv = dia->VerteilenV->isChecked();
		doc->ActPage->AlignObj(xa, ya, Vth, Vtv, xdp, ydp, xart, yart);
		slotDocCh();
		doc->UnDoValid = false;
		CanUndo();
		}
	delete dia;
}

void ScribusApp::DoAlign(bool xa, bool ya, bool Vth, bool Vtv, float xdp, float ydp, int xart, int yart)
{
	doc->ActPage->AlignObj(xa, ya, Vth, Vtv, xdp, ydp, xart, yart);
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
}

void ScribusApp::BuildFontMenu()
{
	QPopupMenu *pm;
	FontID.clear();
	FontMenu->clear();
	FStyleMenu->clear();
	int a;
	QString b = " ";
	QMap<QString,QStrList> Fami;
	SCFontsIterator it(Prefs.AvailFonts);
	for ( ; it.current(); ++it)
	{
		QString family,style;
		if (it.current()->UseFont)
			{
			QString &fn=it.current()->SCName;
			int	pos=fn.find(" ");
			family=fn.left(pos);
			style=fn.right(fn.length()-pos);
			Fami[family].inSort(fn);
			}
	}

	QMapIterator<QString,QStrList> qmi;
	for (qmi = Fami.begin(); qmi != Fami.end(); ++qmi)
		{
		pm = new QPopupMenu();
		QString family=qmi.key();
		QStrList &qsl=qmi.data();
		for( QStrListIterator sli(qsl) ; sli.current() ; ++sli)
			{
			if (Prefs.AvailFonts[sli.current()]->UseFont)
				{
				QString &fn=Prefs.AvailFonts[sli.current()]->SCName;
				a = pm->insertItem(new FmItem(fn, Prefs.AvailFonts[fn]->Font));
				FontID.insert(long(a), &fn);
				}
			}
		connect(pm, SIGNAL(activated(int)), this, SLOT(setItemFont(int)));
		FStyleMenu->insertItem(family, pm);
		}
	a = FontMenu->insertItem(tr("Face"), FStyleMenu);
//	FontID.insert(long(a), &b);
	a = FontMenu->insertSeparator();
//	FontID.insert(long(a), &b);
	if (!HaveDoc)
		{
		it.toFirst();
		a = FontMenu->insertItem(new FmItem(it.currentKey(), it.current()->Font));
		FontMenu->setItemChecked(a, true);
		FontID.insert(long(a), &it.current()->SCName);
		}
	else
		{
		QMap<QString,QFont>::Iterator it3;
		for (it3 = doc->UsedFonts.begin(); it3 != doc->UsedFonts.end(); ++it3)
			{
			a = FontMenu->insertItem(new FmItem(it3.key(), it3.data()));
			if (it3.key() == doc->Dfont)
				{
				FontMenu->setItemChecked(a, true);
				}
			FontID.insert(long(a), &it3.key());
			}
		}
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
	FontPrefs *dia = new FontPrefs(this, Prefs.AvailFonts, HaveDoc, &Prefs, PrefsPfad);
	connect(dia, SIGNAL(ReReadPrefs()), this, SLOT(ReadPrefs()));
	if (dia->exec())
		{
		a = 0;
		SCFontsIterator it(Prefs.AvailFonts);
		for ( ; it.current() ; ++it)
			{
			it.current()->EmbedPS = dia->FlagsPS.at(a)->isChecked();
			it.current()->UseFont = dia->FlagsUse.at(a)->isChecked();
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
		}
	disconnect(dia, SIGNAL(ReReadPrefs()), this, SLOT(ReadPrefs()));
	delete dia;
}

void ScribusApp::slotPrefsOrg()
{
	void *mo;
	char *error;
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
		std::cout << "Can't find Plugin" << endl;
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
		Prefs.KeyActions = dia->KKC;
		SetShortCut();
		Prefs.AppFontSize = dia->GFsize->value();
		Prefs.Wheelval = dia->SpinBox3->value();
		Prefs.RecentDCount = dia->Recen->value();
		Prefs.DocDir = dia->Docs->text();
		DocDir = Prefs.DocDir;
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
		Prefs.RandOben = dia->TopR->value() / UmReFaktor / 100;
		Prefs.RandUnten = dia->BottomR->value() / UmReFaktor / 100;
		Prefs.RandLinks = dia->LeftR->value() / UmReFaktor / 100;
		Prefs.RandRechts = dia->RightR->value() / UmReFaktor / 100;
		Prefs.DoppelSeiten = dia->Doppelseiten->isChecked();
		Prefs.ErsteLinks = dia->Linkszuerst->isChecked();
		Prefs.PDFTransparency = dia->UsePDFTrans->isChecked();
		Prefs.DisScale = dia->DisScale;
		Mpal->Cpal->UseTrans(Prefs.PDFTransparency);
		if (HaveDoc)
			{
			slotZoomAbs(doc->Scale*Prefs.DisScale);
			doc->GrabRad = dia->SpinBox3_2->value();
			doc->GuideRad = dia->SpinBox2g->value() / UmReFaktor / 100;
			doc->Dfont = dia->FontCombo->currentText();
			doc->Dsize = dia->SizeCombo->currentText().left(2).toInt();
			doc->minorGrid = dia->SpinBox1->value() / UmReFaktor / 100;
			doc->majorGrid = dia->SpinBox2->value() / UmReFaktor / 100;
			doc->minorColor = dia->Cmin;
			doc->majorColor = dia->Cmax;
			doc->papColor = dia->Cpaper;
			doc->margColor = dia->Crand;
			doc->guideColor = dia->Cgui;
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
  		doc->Dwidth = dia->LineW->value()/10;
  		doc->DpenLine = dia->Foreground2->currentText();
			if (doc->DpenLine == tr("None"))
				doc->DpenLine = "None";
  		doc->DshadeLine = dia->Shade22->value();
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
  		doc->DwidthLine = dia->LineW2->value()/10;
  		doc->MagMin = dia->MinMag->value();
  		doc->MagMax = dia->MaxMag->value();
  		doc->MagStep = dia->StepMag->value();
  		doc->DbrushPict = dia->BackgroundP->currentText();
			if (doc->DbrushPict == tr("None"))
				doc->DbrushPict = "None";
			doc->ShadePict = dia->ShadeP->value();
			doc->ScaleX = static_cast<float>(dia->XScale->value()) / 100;
			doc->ScaleY = static_cast<float>(dia->YScale->value()) / 100;
			doc->ScaleType = dia->FreeScale->isChecked();
			doc->AspectRatio = dia->Aspect->isChecked();
			doc->Before = dia->RadioButton6->isChecked();
			doc->Einheit = dia->UnitCombo->currentItem();
			switch (doc->Einheit)
				{
				case 0:
					view->UN->setText("pt");
					break;
				case 1:
					view->UN->setText("mm");
					break;
				case 2:
					view->UN->setText("in");
					break;
				case 3:
					view->UN->setText("p");
					break;
				}
			doc->PagesSbS = dia->SidebySide->isChecked();
			doc->ShFrames = dia->FramesVisible->isChecked();
			doc->RandFarbig = dia->RandFarb->isChecked();
			doc->AutoLine = dia->AutoLineV->value();
			doc->AutoSave = dia->ASon->isChecked();
			if (doc->AutoSave)
				{
				doc->ASaveTimer->stop();
				doc->ASaveTimer->start(dia->ASTime->value() * 60 * 1000);
				}
			Mpal->UnitChange();
			view->reformPages();
			view->DrawNew();
			}
		else
			{
			Prefs.GrabRad = dia->SpinBox3_2->value();
			Prefs.GuideRad = dia->SpinBox2g->value() / UmReFaktor / 100;
			Prefs.DefFont = dia->FontCombo->currentText();
			Prefs.DefSize = dia->SizeCombo->currentText().left(2).toInt();
			Prefs.DminGrid = dia->SpinBox1->value() / UmReFaktor / 100;
			Prefs.DmajGrid = dia->SpinBox2->value() / UmReFaktor / 100;
			Prefs.DminColor = dia->Cmin;
			Prefs.DmajColor = dia->Cmax;
			Prefs.DpapColor = dia->Cpaper;
			Prefs.DmargColor = dia->Crand;
			Prefs.guideColor = dia->Cgui;
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
  		Prefs.Dwidth = dia->LineW->value()/10;
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
  		Prefs.DwidthLine = dia->LineW2->value()/10;
  		Prefs.MagMin = dia->MinMag->value();
  		Prefs.MagMax = dia->MaxMag->value();
  		Prefs.MagStep = dia->StepMag->value();
  		Prefs.DbrushPict = dia->BackgroundP->currentText();
			if (Prefs.DbrushPict == tr("None"))
				Prefs.DbrushPict = "None";
			Prefs.ShadePict = dia->ShadeP->value();
			Prefs.ScaleX = static_cast<float>(dia->XScale->value()) / 100;
			Prefs.ScaleY = static_cast<float>(dia->YScale->value()) / 100;
			Prefs.ScaleType = dia->FreeScale->isChecked();
			Prefs.AspectRatio = dia->Aspect->isChecked();
			Prefs.Before = dia->RadioButton6->isChecked();
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
			Prefs.PagesSbS = dia->SidebySide->isChecked();
			Prefs.ShFrames = dia->FramesVisible->isChecked();
			Prefs.RandFarbig = dia->RandFarb->isChecked();
			Prefs.AutoLine = dia->AutoLineV->value();
			Prefs.AutoSave = dia->ASon->isChecked();
			Prefs.AutoSaveTime = dia->ASTime->value() * 60 * 1000;
			}
		SavePrefs();
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
  Prefs.Mpalv = Mpal->isVisible();
  Prefs.Tpalv = Tpal->isVisible();
  Prefs.SCpalv = ScBook->isVisible();
  Prefs.Lpalv = Lpal->isVisible();
  Prefs.Sepalv = Sepal->isVisible();
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
		QFileInfo fd(Prefs.RecentDocs[m]);
		if (fd.exists())
			{
			RecentDocs.append(Prefs.RecentDocs[m]);
			recentMenu->insertItem(Prefs.RecentDocs[m]);
			}
		}
  if (!Prefs.Werkv)
		toolMenu->changeItem(viewTools, tr("Show Tools"));
	if (Prefs.Mpalv)
		toolMenu->changeItem(viewMpal, tr("Hide Measurements"));
	if (Prefs.Tpalv)
		toolMenu->changeItem(viewTpal, tr("Hide Outline"));
	if (Prefs.SCpalv)
		toolMenu->changeItem(viewBpal, tr("Hide Scrapbook"));
	if (Prefs.Lpalv)
		toolMenu->changeItem(viewLpal, tr("Hide Layers"));
	if (Prefs.Sepalv)
		toolMenu->changeItem(viewSepal, tr("Hide Page Palette"));
	if (Prefs.Bopalv)
		toolMenu->changeItem(viewBopal, tr("Hide Bookmarks"));
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
}

void ScribusApp::ShowSubs()
{
	QString mess;
	if (HaveGS != 0)
		{
		mess = tr("The following Programs are missing:")+"\n\n";
		if (HaveGS != 0)
			{
			mess += tr("Ghostscript : You cannot use EPS-Images")+"\n\n";
			PDFavailable = false;
			}
    QMessageBox::warning(this, tr("Warning"), mess, 1, 0, 0);
    }
	if (!Prefs.Werkv)
		WerkTools->hide();
	if (Prefs.Mpalv)
		{
		Mpal->show();
		Mpal->TabStack->raiseWidget(2);
		qApp->processEvents();
		Mpal->TabStack->raiseWidget(0);
		}
	if (Prefs.Tpalv)
		Tpal->show();
	if (Prefs.SCpalv)
		ScBook->show();
	if (Prefs.Lpalv)
		Lpal->show();
	if (Prefs.Sepalv)
		Sepal->show();
	if (Prefs.Bopalv)
		BookPal->show();
	setActiveWindow();
	raise();
}

PSLib* ScribusApp::getPSDriver(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, CListe DocColors, bool pdf)
{
	char *error;
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

PDFlib* ScribusApp::getPDFDriver()
{
	char *error;
	typedef PDFlib* (*sdem)();
	sdem demo;
	QString pfad = PREL;
#if defined(__hpux)
  pfad += "/lib/scribus/libs/libpdf.sl";
#else
	pfad += "/lib/scribus/libs/libpdf.so";
#endif
	PDFDriver = dlopen(pfad, RTLD_LAZY);
	if (!PDFDriver)
		{
		std::cout << "Can't find Plugin" << endl;
		return NULL;
		}
	dlerror();
	demo = (sdem)dlsym(PDFDriver, "Run");
	if ((error = dlerror()) != NULL)
		{
		std::cout << "Can't find Symbol" << endl;
		dlclose(PDFDriver);
		return NULL;
		}
	PDFlib *dia = (*demo)();
	return dia;
}

void ScribusApp::closePDFDriver()
{
	dlclose(PDFDriver);
}

bool ScribusApp::DoSaveAsEps(QString fn)
{
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	PSLib *dd = getPSDriver(false, Prefs.AvailFonts, doc->UsedFonts, doc->PageColors, false);
	if (dd != NULL)
		{
		if (dd->PS_set_file(fn))
			view->CreatePS(dd, doc->ActPage->PageNr, doc->ActPage->PageNr+1, 1, false, tr("All"), true, false, false, false);
		else
			{
			delete dd;
			closePSDriver();
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			return false;
			}
		closePSDriver();
		qApp->setOverrideCursor(QCursor(arrowCursor), true);
		return true;
		}
	else
		return false;
}

void ScribusApp::SaveAsEps()
{
	QString fna;
  if (!doc->DocName.startsWith("Document"))
  	{
  	QFileInfo fi(doc->DocName);
  	fna = fi.dirPath()+"/"+fi.baseName()+".eps";
  	}
  else
  	{
  	QDir di = QDir();
  	fna = di.currentDirPath()+"/"+doc->DocName+".eps";
  	}
  QString fn = CFileDialog(tr("Save as"), tr("EPS-Files (*.eps);; All Files (*)"), "", false, false);
  if (!fn.isEmpty())
  	{
		if (!DoSaveAsEps(fn))
			QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
  	}
}

void ScribusApp::SaveAsPDF()
{
	QString fn;
	uint frPa, toPa;
	int Components;
	QString nam = "";
	QString pfad = PREL;
	pfad += "/lib/scribus/profiles/";
	if (BookPal->BView->childCount() == 0)
		doc->PDF_Optionen.Bookmarks = false;
  PDF_Opts *dia = new PDF_Opts(this, doc->DocName, doc->UsedFonts, view, &doc->PDF_Optionen, doc->PDF_Optionen.PresentVals, &PDFXProfiles, Prefs.AvailFonts);
  if (dia->exec())
  	{
  	qApp->setOverrideCursor(QCursor(waitCursor), true);
  	fn = dia->Datei->text();
		doc->PDF_Optionen.Datei = fn;
		doc->PDF_Optionen.Thumbnails = dia->CheckBox1->isChecked();
		doc->PDF_Optionen.Compress = dia->Compression->isChecked();
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
				hIn = cmsOpenProfileFromFile(pfad+PrinterProfiles[doc->PDF_Optionen.PrintProf], "r");
  			Descriptor = cmsTakeProductDesc(hIn);
				nam = QString(Descriptor);
				if ((int) cmsGetColorSpace(hIn) == icSigRgbData)
					Components = 3;
				if ((int)cmsGetColorSpace(hIn) == icSigCmykData)
					Components = 4;
				if ((int)cmsGetColorSpace(hIn) == icSigCmyData)
					Components = 3;
				cmsCloseProfile(hIn);
				doc->PDF_Optionen.Info = dia->InfoString->text();
				doc->PDF_Optionen.BleedTop = float(dia->BleedTop->value())/UmReFaktor/100.0;
				doc->PDF_Optionen.BleedLeft = float(dia->BleedLeft->value())/UmReFaktor/100.0;
				doc->PDF_Optionen.BleedRight = float(dia->BleedRight->value())/UmReFaktor/100.0;
				doc->PDF_Optionen.BleedBottom = float(dia->BleedBottom->value())/UmReFaktor/100.0;
				doc->PDF_Optionen.Encrypt = false;
				doc->PDF_Optionen.PresentMode = false;
				doc->PDF_Optionen.Encrypt = false;
				}
#else
			doc->PDF_Optionen.UseProfiles = false;
			doc->PDF_Optionen.UseProfiles2 = false;
#endif
			}
		if (dia->AllPages->isChecked())
			{
			frPa = 0;
			toPa = view->Pages.count();
			}
		else
			{
			frPa = uint(dia->FirstPage->value()-1);
			toPa = uint(dia->LastPage->value());
			}
		QMap<int,QPixmap> thumbs;
		for (uint ap = frPa; ap < toPa; ++ap)
			{
			QPixmap pm(10,10);
			if (doc->PDF_Optionen.Thumbnails)
				pm = view->PageToPixmap(ap, 100);
			thumbs.insert(ap, pm);
			}
		PDFlib *pd = getPDFDriver();
		if (pd->PDF_Begin_Doc(fn, doc, view, &doc->PDF_Optionen, Prefs.AvailFonts, doc->UsedFonts, BookPal->BView))
			{
			for (uint ap = 0; ap < view->MasterPages.count(); ++ap)
				{
				if (view->MasterPages.at(ap)->Items.count() != 0)
					pd->PDF_TemplatePage(view->MasterPages.at(ap));
				}
			for (uint a = frPa; a < toPa; ++a)
				{
				pd->PDF_Begin_Page(view->Pages.at(a), thumbs[a]);
				pd->PDF_ProcessPage(view->Pages.at(a), a);
				pd->PDF_End_Page();
				}
			if (doc->PDF_Optionen.Version == 12)
				pd->PDF_End_Doc(pfad+PrinterProfiles[doc->PDF_Optionen.PrintProf], nam, Components);
			else
				pd->PDF_End_Doc();
			}
		else
			QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
		delete pd;
		closePDFDriver();
//		BuildFontMenu();
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
	BookMItem* ip;
	BookMItem* ip2;
	BookMItem* ip3;
	QValueList<ScribusDoc::BookMa>::Iterator it2;
	it2 = doc->BookMarks.begin();
	BookPal->BView->clear();
	if (doc->BookMarks.count() == 0)
		return;
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

void ScribusApp::ModifyObject()
{
	NoFrameEdit();
	PageItem* b = doc->ActPage->SelItem.at(0);
  ModObj *dia = new ModObj(this, b, doc->PageColors, doc, view, &InputProfiles);
	connect(dia, SIGNAL(DocChanged()), this, SLOT(slotDocCh()));
  dia->exec();
	delete dia;
	doc->UnDoValid = false;
	CanUndo();
	Mpal->setLvalue(b->LocalScX, b->LocalScY, b->LocalX, b->LocalY );
}

void ScribusApp::slotElemRead(QString Name, int x, int y, bool art, bool loca, ScribusDoc* docc)
{
	NoFrameEdit();
  ScriXmlDoc *ss = new ScriXmlDoc();
  if(ss->ReadElem(Name, Prefs.AvailFonts, docc, x, y, art, loca, Prefs.GFontSub))
  	{
  	BuildFontMenu();
		Mpal->Spal->updateFList();
  	docc->ActPage->update();
  	slotDocCh();
		docc->UnDoValid = false;
		CanUndo();
  	}
  delete ss;
}

void ScribusApp::slotChangeUnit(int art)
{
	doc->Einheit = art;
	switch (art)
		{
		case 0:
			UmReFaktor = 1.0;
			view->UN->setText("pt");
			break;
		case 1:
			UmReFaktor = 0.3527777;
			view->UN->setText("mm");
			break;
		case 2:
			UmReFaktor = 1.0 / 72.0;
			view->UN->setText("in");
			break;
		case 3:
			UmReFaktor = 1.0 / 12.0;
			view->UN->setText("p");
			break;
		}
	Mpal->UnitChange();
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
	connect(dia, SIGNAL(Fertig()), this, SLOT(ManTempEnd()));
	menuBar()->setItemEnabled(pgmm, 0);
	editMenu->setItemEnabled(tman, 0);
	ActWin->MenuStat[0] = DatSav->isEnabled();
	ActWin->MenuStat[1] = fileMenu->isItemEnabled(fid1);
	ActWin->MenuStat[2] = fileMenu->isItemEnabled(fid4);
	ActWin->MenuStat[3] = fileMenu->isItemEnabled(fid5);
	DatNeu->setEnabled(false);
	DatSav->setEnabled(false);
	DatOpe->setEnabled(false);
	DatClo->setEnabled(false);
/*	fileMenu->setItemEnabled(fid1, 0);
	fileMenu->setItemEnabled(fid4, 0);
	fileMenu->setItemEnabled(fid5, 0);
	fileMenu->setItemEnabled(fid51, 0);
	fileMenu->setItemEnabled(fid12, 0);
	fileMenu->setItemEnabled(fid13, 0);
	fileMenu->setItemEnabled(fid14, 0);   */
	fileMenu->setEnabled(false);
	doc->TemplateMode = true;
	Sepal->DisablePal();
	doc->UnDoValid = false;
	CanUndo();
	dia->show();
	ActWin->muster = dia;
}

void ScribusApp::ManTempEnd()
{
	view->HideTemplate();
	doc->TemplateMode = false;
	menuBar()->setItemEnabled(pgmm, 1);
	editMenu->setItemEnabled(tman, 1);
	DatNeu->setEnabled(true);
	DatSav->setEnabled(ActWin->MenuStat[0]);
	DatOpe->setEnabled(true);
	DatClo->setEnabled(true);
/*	fileMenu->setItemEnabled(fid1, ActWin->MenuStat[1]);
	fileMenu->setItemEnabled(fid4, ActWin->MenuStat[2]);
	fileMenu->setItemEnabled(fid5, ActWin->MenuStat[3]);
	fileMenu->setItemEnabled(fid12, 1);
	fileMenu->setItemEnabled(fid13, 1);
	fileMenu->setItemEnabled(fid14, 1);  */
	fileMenu->setEnabled(true);
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
		slotDocCh();
	Sepal->EnablePal();
	Sepal->RebuildTemp();
	ActWin->muster = NULL;
}

void ScribusApp::ApplyTemp()
{
	QString mna;
	ApplyT *dia = new ApplyT(this, view);
	if (dia->exec())
		{
		mna = dia->Templ->currentText();
		Apply_Temp(mna, doc->ActPage->PageNr);
		}
	delete dia;
}

void ScribusApp::Apply_Temp(QString in, int Snr)
{
	QString mna = in;
	if (mna == tr("Normal"))
		mna = "Normal";
	view->Pages.at(Snr)->MPageNam = mna;
	Page* Mp = view->MasterPages.at(view->MasterNames[mna]);
	if (Mp->YGuides.count() != 0)
		{
		for (uint y = 0; y < Mp->YGuides.count(); ++y)
      {
			if (view->Pages.at(Snr)->YGuides.contains(Mp->YGuides[y]) == 0)
				view->Pages.at(Snr)->YGuides.append(Mp->YGuides[y]);
			}
		qHeapSort(view->Pages.at(Snr)->YGuides);
		}
	if (Mp->XGuides.count() != 0)
		{
		for (uint x = 0; x < Mp->XGuides.count(); ++x)
      {
			if (view->Pages.at(Snr)->XGuides.contains(Mp->XGuides[x]) == 0)
				view->Pages.at(Snr)->XGuides.append(Mp->XGuides[x]);
			}
		qHeapSort(view->Pages.at(Snr)->XGuides);
		}
	view->DrawNew();
	slotDocCh();
	doc->UnDoValid = false;
	CanUndo();
	Sepal->Rebuild();
}

void ScribusApp::GroupObj()
{
	PageItem* b;
	float x, y, w, h;
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
}

void ScribusApp::UnGroupObj()
{
	PageItem* b;
	for (uint a=0; a<doc->ActPage->SelItem.count(); ++a)
		{
		b = doc->ActPage->SelItem.at(a);
		b->Groups.pop();
		}
	doc->ActPage->Deselect(true);
	slotDocCh();
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

QString ScribusApp::CFileDialog(QString caption, QString filter, QString defNa, bool Pre, bool mod, bool comp)
{
	CustomFDialog dia(this, caption, filter, Pre, mod, comp);
	if (defNa != "")
		dia.setSelection(defNa);
	if (dia.exec() == QDialog::Accepted)
		return dia.selectedFile();
	else
		return "";
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
	if (a > 1)
		CallDLL(importMenu->text(id));
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
	if (a > 4)
//	if (a > 5)
		CallDLL(helpMenu->text(id));
}

void ScribusApp::FinalizePlugs()
{
	char *error;
	QMap<QString, PlugData>::Iterator it;
	struct PlugData pda;
	typedef void (*sdem2)();
	sdem2 demo2;
	for (it = PluginMap.begin(); it != PluginMap.end(); ++it)
		{
		if (it.data().Typ == 4)
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

void ScribusApp::InitPlugs()
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
					extraMenu->insertItem(tr(nam));
				if (ty == 2)
					importMenu->insertItem(tr(nam));
				if (ty == 3)
					exportMenu->insertItem(tr(nam));
				if (ty == 4)
					helpMenu->insertItem(tr(nam));
				pda.Datei = d[dc];
				pda.Typ = ty;
				PluginMap.insert(tr(nam), pda);
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
	char *error;
	struct PlugData pda;
	pda = PluginMap[name];
	typedef void (*sdem)(QWidget *d, ScribusApp *plug);
	sdem demo;
	QString pfad = PREL;
	if (pda.Typ != 4)
		{
		pfad += "/lib/scribus/plugins/" + pda.Datei;
		mo = dlopen(pfad, RTLD_LAZY | RTLD_GLOBAL);
		if (!mo)
			{
			std::cout << "Can't find Plugin" << endl;
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
	if (pda.Typ != 4)
		dlclose(mo);
}

bool ScribusApp::DLLName(QString name, QString *PName, int *typ, void **Zeig)
{
	void *mo;
	char *error;
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
		return false;
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
	if (*typ != 4)
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
#ifdef HAVE_CMS
	QString pfad = PREL;
	QString nam = "";
	const char *Descriptor;
	cmsHPROFILE hIn;
	pfad += "/lib/scribus/profiles/";
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
  			Descriptor = cmsTakeProductDesc(hIn);
				nam = QString(Descriptor);
				switch ((int) cmsGetDeviceClass(hIn))
					{
					case icSigInputClass:
						if ((int) cmsGetColorSpace(hIn) == icSigRgbData)
							InputProfiles[nam] = pfad + d[dc];
						break;
					case icSigColorSpaceClass:
						if ((int) cmsGetColorSpace(hIn) == icSigRgbData)
							InputProfiles[nam] = pfad + d[dc];
						break;
					case icSigDisplayClass:
						MonitorProfiles[nam] = d[dc];
						InputProfiles[nam] = pfad + d[dc];
						break;
					case icSigOutputClass:
						PrinterProfiles[nam] = d[dc];
						if ((int) cmsGetColorSpace(hIn) == icSigCmykData)
							PDFXProfiles[nam] = d[dc];
						break;
					}
				cmsCloseProfile(hIn);
				}
			}
		if ((!PrinterProfiles.isEmpty()) && (!InputProfiles.isEmpty()) && (!MonitorProfiles.isEmpty()))
			CMSavail = true;
		}
	else
		CMSavail = false;
#else
	CMSavail = false;
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
			FMess->setText(tr("Adjusting Colors"));
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
		ColorMenu->clear();
		QPixmap pm = QPixmap(15, 15);
		int a = 0;
		ColorMenu->insertItem(tr("None"));
		CMYKColor tmp;
		tmp.fromQColor(doc->papColor);
		tmp.RecalcRGB();
		doc->papColor = tmp.getRGBColor();
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
			{
			doc->PageColors[it.key()].RecalcRGB();
			pm.fill(doc->PageColors[it.key()].getRGBColor());
			ColorMenu->insertItem(QIconSet(pm), it.key());
			if (it.key() == doc->Dbrush)
				{
				ColorMenu->setItemChecked(ColorMenu->idAt(a), true);
				}
			a++;
			if (dia != NULL)
				dia->setProgress(a);
			}
		Mpal->Cpal->SetColors(doc->PageColors);
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
  		Annota *dia = new Annota(this, b, doc->PageC, static_cast<int>(doc->PageB), static_cast<int>(doc->PageH), doc->PageColors, view);
  		if (dia->exec())
				slotDocCh();
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
	for (a = 9; a < 19; ++a)
		{
		editMenu->setAccel(Prefs.KeyActions[a].KeyID, Prefs.KeyActions[a].MenuID);
		}
	for (a = 19; a < 30; ++a)
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
#ifdef HAVE_FREETYPE
	NoFrameEdit();
	doc->ActPage->TextToPath();
	doc->UnDoValid = false;
	CanUndo();
#endif
}

void ScribusApp::Pfadtext()
{
	doc->ActPage->ToPathText();
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

void ScribusApp::LayerRemove(int l)
{
	for (uint a = 0; a < view->Pages.count(); ++a)
		{
		for (uint b = 0; b < view->Pages.at(a)->Items.count(); ++b)
			{
			if (view->Pages.at(a)->Items.at(b)->LayerNr == l)
				view->Pages.at(a)->Items.at(b)->LayerNr = 0;
			}
		}
	for (uint a = 0; a < view->MasterPages.count(); ++a)
		{
		for (uint b = 0; b < view->MasterPages.at(a)->Items.count(); ++b)
			{
			if (view->MasterPages.at(a)->Items.at(b)->LayerNr == l)
				view->MasterPages.at(a)->Items.at(b)->LayerNr = 0;
			}
		}
	for (uint a = 0; a < view->DocPages.count(); ++a)
		{
		for (uint b = 0; b < view->DocPages.at(a)->Items.count(); ++b)
			{
			if (view->DocPages.at(a)->Items.at(b)->LayerNr == l)
				view->DocPages.at(a)->Items.at(b)->LayerNr = 0;
			}
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
//		view->Pages.at(doc->UnData.PageNr)->SelItem.clear();
		view->Pages.at(doc->UnData.PageNr)->Deselect(true);
		switch (doc->UnData.UnCode)
			{
			case 0:
				view->Pages.at(doc->UnData.PageNr)->Items.insert(b->ItemNr, b);
				for (a = 0; a < view->Pages.at(doc->UnData.PageNr)->Items.count(); ++a)
					{
					view->Pages.at(doc->UnData.PageNr)->Items.at(a)->ItemNr = a;
					}
				break;
			case 1:
				b->Xpos = doc->UnData.Xpos;
				b->Ypos = doc->UnData.Ypos;
				break;
			case 2:
				b->Xpos = doc->UnData.Xpos;
				b->Ypos = doc->UnData.Ypos;
				b->Rot = doc->UnData.Rot;
				if (b->PType == 5)
					mp = true;
				view->Pages.at(doc->UnData.PageNr)->SizeItem(doc->UnData.Width, doc->UnData.Height, b->ItemNr, mp);
				break;
			case 3:
				b->Rot = doc->UnData.Rot;
				break;
			case 4:
				view->Pages.at(doc->UnData.PageNr)->Items.take(b->ItemNr);
				view->Pages.at(doc->UnData.PageNr)->Items.insert(doc->UnData.ItemNr, b);
				for (a = 0; a < view->Pages.at(doc->UnData.PageNr)->Items.count(); ++a)
					{
					view->Pages.at(doc->UnData.PageNr)->Items.at(a)->ItemNr = a;
					}
				break;
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
			editMenu->changeItem(edUndo, tr("Undo Delete Object"));
			break;
		case 1:
		case 4:
			editMenu->changeItem(edUndo, tr("Undo Object Move"));
			break;
		case 2:
		case 3:
			editMenu->changeItem(edUndo, tr("Undo Object Change"));
			break;
		}
	if (doc->UnDoValid)	
		editMenu->setItemEnabled(edUndo, 1);
	else
		editMenu->setItemEnabled(edUndo, 0);
}

void ScribusApp::InitHyphenator()
{
	QString datein = "";
	QString	lang = QString(QTextCodec::locale()).left(2);
	QString pfad = PREL;
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
			Sprachen.insert(datein, d[dc]);
			if (d[dc] == "hyph_"+lang+".dic")
				Prefs.Language = datein;
			}
		if (datein == "")
			Prefs.Language = tr("English");
		}
}

void ScribusApp::configHyphenator()
{
	HySettings *dia = new HySettings(this, &Sprachen);
	if (HaveDoc)
		{
		dia->Verbose->setChecked(doc->Trenner->Automatic);
		dia->Input->setChecked(doc->Trenner->AutoCheck);
		dia->Language->setCurrentText(doc->Trenner->Language);
		dia->WordLen->setValue(doc->Trenner->MinWordLen);
		}
	else
		{
		dia->Verbose->setChecked(Prefs.Automatic);
		dia->Input->setChecked(Prefs.AutoCheck);
		dia->Language->setCurrentText(Prefs.Language);
		dia->WordLen->setValue(Prefs.MinWordLen);
		}
	if (dia->exec())
		{
		if (HaveDoc)
			{
			doc->Trenner->slotNewDict(dia->Language->currentText());
			doc->Trenner->slotNewSettings(dia->WordLen->value(), dia->Verbose->isChecked(), dia->Input->isChecked());
			}
		else
			{
			Prefs.MinWordLen = dia->WordLen->value();
			Prefs.Language = dia->Language->currentText();
			Prefs.Automatic = dia->Verbose->isChecked();
			Prefs.AutoCheck = dia->Input->isChecked();
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
			doc->Trenner->slotHyphenate(b);
			}
		}
}

void ScribusApp::ToggleObjLock()
{
	PageItem *b;
	if (HaveDoc)
		{
  	if (doc->ActPage->SelItem.count() != 0)
			{
  		b = doc->ActPage->SelItem.at(0);
  		doc->ActPage->ToggleLock();
			HaveNewSel(b->PType);
			}
		}
}

void ScribusApp::ManageGuides()
{
	if (HaveDoc)
		{
		GuideManager *dia = new GuideManager(this, doc->ActPage, doc->Einheit);
		dia->exec();
		delete dia;
		}
}

void ScribusApp::SetTranspar(float t)
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

void ScribusApp::Collect()
{
	QString s = QFileDialog::getExistingDirectory(QDir::currentDirPath(), this, "d", tr("Choose a Directory"), true);
	if (s != "")
		{
		if(s.right(1) != "/")
	  	s += "/";
		QFileInfo fi = QFileInfo(s);
		if (fi.exists())
			{
			if (fi.isDir() && fi.isWritable())
				{
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
								QString cmd = "cp " + ite->Pfile + " " + s + itf.fileName();
								system(cmd);
								ite->Pfile = s + itf.fileName();
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
								QString cmd = "cp " + ite->Pfile + " " + s + itf.fileName();
								system(cmd);
								ite->Pfile = s + itf.fileName();
								}
							}
						}
					}
				QString fn;
				if (doc->hasName)
					{
  				QFileInfo fis(doc->DocName);
  				fn = s + fis.fileName();
					}
				else
					fn = s + doc->DocName+".scd";
				if (!DoFileSave(fn))
					QMessageBox::warning(this, tr("Warning"), tr("Can't write the File: \n%1").arg(fn), tr("OK"));
				}
			}
		}
}

void ScribusApp::slotTest()
{
}

void ScribusApp::slotTest2()
{
}
