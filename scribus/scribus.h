/***************************************************************************
                          scribus.h  -  description
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

#ifndef SCRIBUS_H
#define SCRIBUS_H

#define VERS09x

// include files for QT
#include <qapplication.h>
#include <qmainwindow.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qstatusbar.h>
#include <qtooltip.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qmap.h>
#include <qfont.h>
#include <qtimer.h>
#include <qintdict.h>
#include <qprogressdialog.h>
#include <qprogressbar.h>
#include <qworkspace.h>
#include <qptrlist.h>
#include <qclipboard.h>

// application specific includes
#include "scribusview.h"
#include "scribusdoc.h"
#include "scribuswin.h"
#include "tree.h"
#include "mpalette.h"
#include "frameedit.h"
#include "scrap.h"
#include "layers.h"
#include "libpostscript/pslib.h"
#include "werktoolb.h"
#include "seiten.h"
#include "bookpalette.h"
#include "splash.h"
class Autoforms;
class FontCombo;

/**
  * This Class is the base class for your application. It sets up the main
  * window and providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class ScribusView is
  * created which creates your view.
  */
class ScribusApp : public QMainWindow
{
	Q_OBJECT
  
public:
	/** constructor */
	ScribusApp();
	/** destructor */
	~ScribusApp() {};
    /** init methods */
	void initGui();
	/** initMenuBar creates the menu_bar and inserts the menuitems */
	void initMenuBar();
	/** setup the statusbar */
	void initStatusBar();
	void SetShortCut();
	void SetKeyEntry(int Nr, QString text, int Men, int KeyC);
	bool doFileNew(double b, double h, double tpr, double lr, double rr, double br, double ab, double sp,
									 bool atf, bool fp, int einh, bool firstleft, int Ori, int SNr);
	bool DoFileSave(QString fn);
	void closeEvent(QCloseEvent *ce);
	void keyPressEvent(QKeyEvent *k);
	void wheelEvent(QWheelEvent *w);
	void DeleteSel(PageItem *b);
	void setTBvals(PageItem *b);
	void SavePrefs();
	void ShowSubs();
	void applyNewMaster(QString name);
	void UpdateRecent(QString fn);
	void InitPlugs(SplashScreen *spl);
	void InitHyphenator();
	void FinalizePlugs();
	bool DLLName(QString name, QString *PName, int *typ, void **Zeig);
	void CallDLL(QString name);
	PSLib* getPSDriver(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, CListe DocColors, bool pdf);
	void closePSDriver();
	bool getPDFDriver(QString fn, QString nam, int Components, int frPa, int toPa, QMap<int,QPixmap> thumbs);
	bool DoSaveAsEps(QString fn);
	QString CFileDialog(QString caption = "", QString filter = "", QString defNa = "", bool Pre = false, 
						bool mod = true, bool comp = false, bool cod = false);
	void GetCMSProfiles();
	void GetCMSProfilesDir(QString pfad);
	void RecalcColors(QProgressBar *dia = 0);
	void SwitchWin();
	void RestoreBookMarks();
	void AdjustBM();
	void ReorgFonts();
	void GetUsedFonts(QMap<QString,QFont> *Really);
	static void defaultCrashHandler (int sig);
	void emergencySave();
	struct CLBuf Buffer;
	QString Buffer2;
	QString Buffer3;
	bool BuFromApp;
	ProfilesL MonitorProfiles;
	ProfilesL PrinterProfiles;
	ProfilesL PDFXProfiles;
	double DispX;
	double DispY;
	bool NoFonts;
	int HaveDoc;
	struct preV Prefs;
	/** view is the main widget which represents your working area. The View
	 * class should handle all events of the view widget.  It is kept empty so
	 * you can create your view according to your application's needs by
	 * changing the view class.
	 */
	ScribusView *view;
	/** doc represents your actual document and is created only once. It keeps
	 * information such as filename and does the serialization of your files.
	 */
	ScribusDoc *doc;
	QLabel* FMess;
	QProgressBar* FProg;
	QLabel* XMess;
	QLabel* XDat;
	QLabel* YMess;
	QLabel* YDat;
	Mpalette *Mpal;
	NodePalette *Npal;
	Tree *Tpal;
	Biblio *ScBook;
	LayerPalette* Lpal;
	SeitenPal *Sepal;
	BookPalette *BookPal;
	QMap<QString,QString> Sprachen;
	QWorkspace *wsp;
	QPopupMenu* windowsMenu;
	ScribusWin* ActWin;
	QString PrefsPfad;
	QClipboard *ClipB;
	QString LoadEnc;
	bool singleClose;
	bool ScriptRunning;
	Autoforms* SCustom;
	WerkToolB* WerkTools;
	int HavePngAlpha;

public slots:
	void slotStoryEditor();
	void InvertPict();
	void Collect();
	void ChBookmarks(int s, int e, int n);
	void AddBookMark(PageItem *ite);
	void DelBookMark(PageItem *ite);
	void BookMarkTxT(PageItem *ite);
	void StoreBookmarks();
	void ReadPrefs();
	void ManageGuides();
	void SetTranspar(double t);
	void SetTransparS(double t);
	void ReportMP(double xp, double yp);
	bool DoFileClose();
	void DoSaveClose();
	void windowsMenuAboutToShow();
	void newActWin(QWidget *w);
	void windowsMenuActivated(int id);
	void ToggleObjLock();
	void UnDoAction();
	void CanUndo();
	void configHyphenator();
	void doHyphenate();
	void slotTest();
	void slotTest2();
	void PutScrap(QString t);
	void Pfadtext();
	void noPfadtext();
	void UniteOb();
	void SplitUniteOb();
	void TraceText();
	void changeLayer(int l);
	void showLayer();
	void LayerRemove(int l);
	void SetCMSPrefs();
	void ManageJava();
	void ManageTemp(QString temp = "");
	void ManTempEnd();
	/** generate a new document in the actual view */
	bool slotFileNew();
	bool slotDocMerge();
	bool LadeSeite(QString fileName, int Nr, bool Mpa);
	/** open a document */
	void slotFileOpen();
	/** open a document */
	void LoadRecent(int id);
	bool slotDocOpen();
	bool LadeDoc(QString fileName);
	void slotAutoSaved();
	/** save a document */
	void slotFileSave();
	/** save a document under a different filename*/
	void slotFileSaveAs();
	void slotFileRevert();
	/** Sichert den Text eines Elements */
	void SaveText();
	/** close the actual file */
	bool slotFileClose();
	/** print the actual file */
	void slotFilePrint();
	/** exits the application */
	void slotFileQuit();
	/** put the marked text/object into the clipboard and remove
	 * it from the document */
	void slotEditCut();
	/** put the marked text/object into the clipboard*/
	void slotEditCopy();
	/** paste the clipboard into the document*/
	void slotEditPaste();
	void EnableTxEdit();
	void DisableTxEdit();
	void SelectAll();
	void ClipChange();
	void DeleteText();
	/** shows an about dlg*/
	void slotHelpAbout();
    void slotHelpAboutQt();
	void slotOnlineHelp();
	void ToggleTips();
	/** Erzeugt eine neue Seite */
	void slotNewPageP(int wo, QString templ);
	void slotNewPageM();
	void slotNewPageT(int w);
	void slotNewPage(int w);
	/** Loescht die aktuelle Seite */
	void DeletePage();
	void DeletePage2(int pg);
	/** Verschiebt Seiten */
	void MovePage();
	void CopyPage();
	/** Ansicht absolut zoomen */
	void slotZoomAbs(double z);
	/** Ansicht ganzes Blatt) */
	void slotZoomFit();
	/** Ansicht 20 % */
	void slotZoom20();
	/** Ansicht 50 % */
	void slotZoom50();
	/** Ansicht 75 % */
	void slotZoom75();
	/** Ansicht 100 % */
	void slotZoom100();
	/** Ansicht 200 % */
	void slotZoom200();
	/** Schaltet Raender ein/aus */
	void ToggleMarks();
	void ToggleFrames();
	/** Schaltet Werkzeuge ein/aus */
 	void setTools(bool visible);
	void ToggleTools();
 	void setPDFTools(bool visible);
	void TogglePDFTools();
	/** Schaltet Masspalette ein/aus */
 	void setMpal(bool visible);
	void ToggleMpal();
	/** Schaltet Uebersichtspalette ein/aus*/
	void ToggleTpal();
 	void setTpal(bool visible);
	void ToggleBpal();
	void setBpal(bool visible);
	void ToggleLpal();
	void setLpal(bool visible);
	void ToggleSepal();
	void setSepal(bool visible);
	void ToggleBookpal();
	void setBookpal(bool visible);
	/** Schaltet Bilder ein/aus */
	void TogglePics();
	/** Schaltet Raster ein/aus */
	void ToggleRaster();
	/** Schaltet Rasterbenutzung ein/aus */
	void ToggleURaster();
	/** Schaltet Rahmenbearbeitung ein/aus */
	void ToggleFrameEdit();
	void slotSelect();
	void ModeFromTB(int);
	/** Switch AppMode */
	void setAppMode(int mode);
	/** Neues Dokument erzeugt */
	void HaveNewDoc();
	/** Element ausgewaehlt */
	void HaveNewSel(int Nr);
	/** Dokument ist geaendert worden */
	void slotDocCh(bool reb = true);
	/** Setzt die Farbe */
	void setItemFarbe(int id);
	/** Setzt die Abstufung */
	void setItemShade(int id);
	/** Setzt den Font */
	void setItemFont(int id);
	void setItemFont2(int id);
	/** Korrigiert das FontMenu */
	void AdjustFontMenu(QString nf);
	void SetNewFont(QString nf);
	/** Setz die Zeichensatzgroesse */
	void setItemFSize(int id);
	void setFSizeMenu(int size);
	/** Farbeditor */
	void slotEditColors();
	/** Setzt den Pen-Tonwert */
	void setPenShade(int sh);
	/** Setzt den Brush-Tonwert */
	void setBrushShade(int sh);
	void setGradFill(int typ, QString col1, int sh1, QString col2, int sh2);
	/** Setzt die Pen-Farbe */
	void setPenFarbe(QString farbe);
	/** Setzt die Brush-Farbe */
	void setBrushFarbe(QString farbe);
	void setCSMenu(QString f, QString l, int fs, int ls);
	/** Fragt nach den Farben */
	void GetBrushPen();
	/** Erzeugt einen Rahmen */
	void MakeFrame(int f, int c, double *vals);
	/** Loescht ein Element */
	void DeleteObjekt();
	/** Setzt das Element in den Hintergrund */
	void Objekt2Back();
	/** Setzt das Element in den Vordergrund */
	void Objekt2Front();
	/** Bewegt das Element eine Ebene nach oben */
	void ObjektRaise();
	/** Bewegt das Element nach unten */
	void ObjektLower();
	/** Dupliziert das Element */
	void ObjektDup();
	/** Dupliziert das Element mehrfach*/
	void ObjektDupM();
	/** Setzt die Infos fuer das Dokument */
	void InfoDoc();
	/** Reformatiert das Dokument */
	bool SetupDoc();
	/** Richtet Objekte aus */
	void ObjektAlign();
	void DoAlign(bool xa, bool ya, bool Vth, bool Vtv, double xdp, double ydp, int xart, int yart);
	void GetAllFonts();
	void BuildFontMenu();
	void slotFontOrg();
	void slotPrefsOrg();
	void slotEditStyles();
	void slotEditLineStyles();
	void setNewAbStyle(int a);
	void setAbsValue(int a);
	void SelectFromOutl(int Page, int Item);
	void SelectFromOutlS(int Page);
	void SaveAsEps();
	void SaveAsPDF();
	void Aktiv();
	void setItemHoch(int h);
	void setStilvalue(int s);
	void setItemTypeStyle(int id);
	void slotElemRead(QString Name, int x, int y, bool art, bool loca, ScribusDoc* docc);
	void slotChangeUnit(int art, bool draw = true);
	void NoFrameEdit();
	void setItemTextAli(int id);
	void ApplyTemp();
	void Apply_Temp(QString in, int Snr, bool reb = true);
	void GroupObj();
	void UnGroupObj();
	void StatusPic();
	void RunPlug(int id);
	void RunImportPlug(int id);
	void RunExportPlug(int id);
	void RunHelpPlug(int id);
	void ModifyAnnot();
	void ToggleGuides();
	void ToggleUGuides();
	void HaveRaster(bool art);
	void EditTabs();
	void SearchText();

signals:
	void TextISize(int);
	void TextIFont(QString);
	void TextUSval(double);
	void TextStil(int);
	void TextFarben(QString, QString, int, int);
	void TextScale(int);

private:
    /** the splash screen */
	SplashScreen * splash;
    /** init methods */
	void initScribus();

	/** file_menu contains all items of the menubar entry "File" */
	QPopupMenu *fileMenu;
	/** edit_menu contains all items of the menubar entry "Edit" */
	QPopupMenu *editMenu;
	/** StilMenu enthaelt das Stilemenue */
	QPopupMenu *StilMenu;
	/** ObjMenu enthaelt das Objektemenue */
	QPopupMenu *ObjMenu;
	/** pageMenu enthaelt das Seitenmenue */
	QPopupMenu *pageMenu;
	/** view_menu contains all items of the menubar entry "View" */
	QPopupMenu *viewMenu;
	/** ColorMenu enthaelt die Farben des Dokuments */
	QPopupMenu *ColorMenu;
	QComboBox *ColorMenC;
	/** SizeTMenu enthaelt die Schriftgroessen */
	QPopupMenu *SizeTMenu;
	/** ShadeMenu enthaelt die Tonwerte */
	QPopupMenu *ShadeMenu;
	/** ShapeMenu enthaelt die Rahmenformen */
	QPopupMenu *ShapeMenu;
	/** FontMenu enthaelt die Fonts */
	QPopupMenu *FontMenu;
    FontCombo* FontSub;
	QPopupMenu *TypeStyleMenu;
	QPopupMenu *AliMenu;
	QPopupMenu *recentMenu;
	QPopupMenu *hymen;
	QToolBar *WerkTools2;
	WerkToolBP* WerkToolsP;
	QToolButton* DatOpe;
	QToolButton* DatSav;
	QToolButton* DatClo;
	QToolButton* DatPri;
	QToolButton* DatPDF;
	QToolButton* DatNeu;
	int KeyMod;
	int ShapeEdit;
	int ShapeM;
	int DistM;
	int PfadT;
	int PfadDT;
	int PfadS;
	int PfadV;
	int PfadTP;
	int pgmd;
	int pgmm;
	int pgmv;
	int Stm;
	int Obm;
	int Markers;
	int FrameDr;
	int Bilder;
	int Ras;
	int uRas;
	int Guide;
	int uGuide;
	int viewTools;
	int viewToolsP;
	int viewMpal;
	int viewTpal;
	int viewNpal;
	int viewBpal;
	int viewLpal;
	int viewSepal;
	int viewBopal;
	int fid1;
	int fid2;
	int fid2a;
	int fid3;
	int fid4;
	int fid5;
	int fid51;
	int fid52;
	int fid6;
	int fid7;
	int fid8;
	int fid9;
	int fid10;
	int fid11;
	int fid12;
	int fid13;
	int fid14;
	int edUndo;
	int edid1;
	int edid2;
	int edid3;
	int edid4;
	int edid5;
	int edid6;
	int edid6a;
	int Sear;
	int Loesch;
	int tman;
	int jman;
	int tip;
	int Gr;
	int UnGr;
	int LockOb;
	int exmn;
	int hyph;
	int ORaise;
	int OLower;
	int OBack;
	int OFront;
	bool tipsOn;
	bool keyrep;
	QPopupMenu *helpMenu;
	QPopupMenu *toolMenu;
	QPopupMenu *extraMenu;
	QPopupMenu *importMenu;
	QPopupMenu *exportMenu;
	double mm2pts(int mm);
	double pts2mm(double pts);
	QMap<int,QString> FontID;
	int HaveGS;
	void *PSDriver;
	int DocNr;
	QStringList RecentDocs;
	struct PlugData { 
						QString Datei;
					  	void *Zeiger;
					  	int Typ;
					} ;
	QMap<QString, PlugData> PluginMap;
	bool PrinterUsed;
	struct PDe { 
					QString Pname;
    			 	QString Dname;
				 	QString Command;
    			} PDef ;
};
#endif 
