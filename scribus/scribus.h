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

#define VERS13x

// include from stl
#include <vector>

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
#include <qdict.h>
#include <qguardedptr.h>
#include <qfont.h>
#include <qtimer.h>
#include <qintdict.h>
#include <qprogressdialog.h>
#include <qprogressbar.h>
#include <qworkspace.h>
#include <qptrlist.h>
#include <qclipboard.h>
#include <qprocess.h>

// application specific includes
#include "scribusapi.h"
#include "scribusview.h"
#include "scribusdoc.h"
#include "scribuswin.h"

class NodePalette;
class LayerPalette;
class SeitenPal;
class BookPalette;
class SplashScreen;

#include "prefscontext.h"
#include "scribusstructs.h"

class Autoforms;
class FontCombo;
class StilFormate;
class LineFormate;
class Mpalette;
class Measurements;
class StoryEditor;
class PSLib;
class WerkToolB;
class WerkToolBP;
class UndoPalette;
class FileWatcher;
class UndoManager;
class UndoState;
class SimpleState;
class ScrAction;
class MenuManager;
class CheckDocument;
class Tree;
class Biblio;
class PluginManager;
class ActionManager;
class AlignDistributePalette;
class TOCGenerator;
class PrefsManager;

class ScribusApp;

extern ScribusApp* ScApp;

class ScribusApp;

extern ScribusApp* SCRIBUS_API ScApp;

/**
  * This Class is the base class for your application. It sets up the main
  * window and providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class ScribusView is
  * created which creates your view.
  */
class SCRIBUS_API ScribusApp : public QMainWindow, public UndoObject
{
	Q_OBJECT

public:
	/** constructor */
	ScribusApp();
	/** destructor */
	~ScribusApp() {};
	int initScribus(bool showSplash, bool showFontInfo, const QString newGuiLanguage);
	void showSplash(bool);
	bool splashShowing() const;
	void closeSplash();
	const QString getGuiLanguage();
	bool warningVersion(QWidget *parent);
	void SetShortCut();
	void startUpDialog();
	void setDefaultPrinter(const QString&, const QString&, const QString&);
	void getDefaultPrinter(QString*, QString*, QString*);
	const bool mainToolBarVisible();
	const bool pdfToolBarVisible();
	const bool editToolBarVisible();
	const bool fileToolBarVisible();
	const bool fileWatcherActive();
	
	bool doFileNew(double width, double h, double tpr, double lr, double rr, double br, double ab, double sp,
									bool atf, int fp, int einh, int firstleft, int Ori, int SNr, const QString&);
	bool DoFileSave(QString fn);
	void closeEvent(QCloseEvent *ce);
	void keyPressEvent(QKeyEvent *k);
	void keyReleaseEvent(QKeyEvent *k);
	void mouseReleaseEvent(QMouseEvent *m);
	void wheelEvent(QWheelEvent *w);
	void deleteSelectedTextFromFrame(PageItem *currItem);
	void setTBvals(PageItem *currItem);
	void ShowSubs();
	void applyNewMaster(QString name);
	void updateRecent(QString fn);
	QString GetLang(QString inLang);
	bool getPDFDriver(QString fn, QString nam, int Components, std::vector<int> &pageNs, QMap<int,QPixmap> thumbs);
	bool DoSaveAsEps(QString fn);
	QString CFileDialog(QString wDir = ".", QString caption = "", QString filter = "", QString defNa = "",
						bool Pre = false, bool mod = true, bool comp = false, bool cod = false,
						bool onlyDirs = false, bool *docom = 0, bool *doFont = 0);
	void GetCMSProfiles();
	void GetCMSProfilesDir(QString pfad);
	//Recalculate the colors after changing CMS settings. Call the appropriate document function and then update the GUI elements.
	void recalcColors(QProgressBar *dia = 0);
	void SwitchWin();
	void RestoreBookMarks();
	void AdjustBM();
	void ReorgFonts();

	void emergencySave();
	void parsePagesString(QString pages, std::vector<int>* pageNs, int sourcePageCount);
	/**
	 * @brief Returns true if an arrow key is pressed down.
	 * @return true if an arrow key is pressed down otherwise returns false
	 */
	bool arrowKeyDown();
	/**
	 * @brief Returns true if application is in object specific undo mode, other wise returns false.
	 * @return true if application is in object specific undo mode, other wise returns false
	 */
	bool isObjectSpecificUndo();
	void restore(UndoState* state, bool isUndo);
	void restoreGroupping(SimpleState *state, bool isUndo);
	void restoreUngroupping(SimpleState *state, bool isUndo);
	void restoreAddPage(SimpleState *state, bool isUndo);
	void restoreDeletePage(SimpleState *state, bool isUndo);
	struct CopyPasteBuffer Buffer;
	QString Buffer2;
	QString Buffer3;
	bool BuFromApp;
	ProfilesL InputProfiles;
	ProfilesL InputProfilesCMYK;
	ProfilesL MonitorProfiles;
	ProfilesL PrinterProfiles;
	ProfilesL PDFXProfiles;
	double DispX;
	double DispY;
	int HaveDoc;
	QString noneString;
	PrefsContext* dirs;
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
    /** the splash screen */
	SplashScreen *splashScreen;
	QLabel* mainWindowStatusLabel;
	QProgressBar* mainWindowProgressBar;
	QLabel* mainWindowXPosLabel;
	QLabel* mainWindowXPosDataLabel;
	QLabel* mainWindowYPosLabel;
	QLabel* mainWindowYPosDataLabel;
	Mpalette *propertiesPalette;
	NodePalette *nodePalette;
	Tree *outlinePalette;
	Biblio *scrapbookPalette;
	LayerPalette* layerPalette;
	SeitenPal *pagePalette;
	BookPalette *bookmarkPalette;
	Measurements* measurementPalette;
	CheckDocument * docCheckerPalette;
	UndoPalette* undoPalette;
	AlignDistributePalette *alignDistributePalette;
	StoryEditor* storyEditor;
	StoryEditor* CurrStED;
	QMap<QString,QString> Sprachen;
	QWorkspace *wsp;
	ScribusWin* ActWin;
	QString PrefsPfad;
	QClipboard *ClipB;
	QString LoadEnc;
	bool singleClose;
	bool ScriptRunning;
	Autoforms* SCustom;
	WerkToolB* mainToolBar;
	int HavePngAlpha;
	int HaveTiffSep;
	bool unicodeTextEditMode;
	int unicodeInputCount;
	QString unicodeInputString;

	QMap<QString, QStringList> InstLang;
	QMap<QString,QString> LangTransl;
	FileWatcher* fileWatcher;
	QProcess *ExternalApp;

	QMap<QString, QGuardedPtr<ScrAction> > scrActions;
	QMap<QString, QGuardedPtr<ScrAction> > scrRecentFileActions;
	QMap<QString, QGuardedPtr<ScrAction> > scrWindowsActions;
	QMap<QString, QGuardedPtr<ScrAction> > scrLayersActions;
	QDict<QActionGroup> scrActionGroups;
	MenuManager* scrMenuMgr;
	ActionManager* actionManager;
	PluginManager* pluginManager;
	QStringList RecentDocs;

public slots:
	void languageChange();
	void specialActionKeyEvent(QString actionName, int unicodevalue);
	void newView();
	void ToggleAllGuides();
	void ToggleAllPalettes();
	void slotStoryEditor();
	void slotCharSelect();
	void ImageEffects();
	QString Collect(const bool compress = false, const bool withFonts = false, const QString& newDirectory=QString::null);
	void ChBookmarks(int s, int e, int n);
	void AddBookMark(PageItem *ite);
	void DelBookMark(PageItem *ite);
	void BookMarkTxT(PageItem *ite);
	void StoreBookmarks();
	void ManageGuides();
	void setItemFillTransparency(double t);
	void setItemLineTransparency(double t);
	void setMousePositionOnStatusBar(double xp, double yp);
	bool DoFileClose();
	bool DoSaveClose();
	void windowsMenuAboutToShow();
	void newActWin(QWidget *w);
	void windowsMenuActivated(int id);
	void UnDoAction();
	void RedoAction();
	void doHyphenate();
	void doDeHyphenate();
	void slotTest();
	void slotTest2();
	void PutScrap();
	void changeLayer(int);
	void showLayer();
	void LayerRemove(int l, bool dl = false);
	void ManageJava();
	void manageMasterPages(QString temp = "");
	void manageMasterPagesEnd();
	/** generate a new document in the current view */
	bool slotFileNew();
	bool slotPageImport();
	bool loadPage(QString fileName, int Nr, bool Mpa);
	/** open a document */
	void slotFileOpen();
	void slotFileAppend();
	/** open a document */
	void removeRecent(QString fn);
	void loadRecent(QString fn);
	void rebuildRecentFileMenu();
	void rebuildLayersList();
	bool slotDocOpen();
	bool loadDoc(QString);
	void slotAutoSaved();
	/** save a document */
	bool slotFileSave();
	/** save a document under a different filename*/
	bool slotFileSaveAs();
	void slotFileRevert();
	/** Sichert den Text eines Elements */
	void SaveText();
	/** close the actual file */
	bool slotFileClose();
	/** print the actual file */
	void slotFilePrint();
	void slotReallyPrint();
	bool doPrint(PrintOptions *options);
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
	void deselectAll();
	void ClipChange();
	void clearContents();
	/** shows an about dlg*/
	void slotHelpAbout();
    void slotHelpAboutQt();
	void slotOnlineHelp();
	void ToggleTips();
	/** Erzeugt eine neue Seite */
	void slotNewPageP(int wo, QString templ);
	void slotNewPageM();
	void slotNewMasterPage(int w);
	void slotNewPage(int w, bool mov = true);
	void duplicateToMasterPage();
	/** Loescht die aktuelle Seite */
	void DeletePage();
	/**
	 * Delete pages
	 * @param from First page to delete
	 * @param to Last page to delete
	 */
	void DeletePage(int from, int to);
	void DeletePage2(int pg);
	/** Verschiebt Seiten */
	void MovePage();
	void CopyPage();
	void changePageMargins();
	/** Zoom the view */
	void slotZoom(double zoomFactor); // 20, 50, 100, or -100 for Fit
	/** Schaltet Raender ein/aus */
	void ToggleMarks();
	void ToggleFrames();
	void ToggleTextLinks();
	void ToggleTextControls();
	void ToggleRuler();
	/** Schaltet Werkzeuge ein/aus */
 	void setTools(bool visible);
	void ToggleTools();
 	void setPDFTools(bool visible);
	void TogglePDFTools();
	/** Schaltet Masspalette ein/aus */
	//void togglePropertiesPalette();
	/** Schaltet Uebersichtspalette ein/aus*/
	//void toggleOutlinePalette();
	//void toggleScrapbookPalette();
	//void toggleLayerPalette();
	void togglePagePalette();
	void setPagePalette(bool visible);
	//void toggleBookmarkPalette();
	void toggleUndoPalette();
	void setUndoPalette(bool visible);
	void toggleCheckPal();
	/** Schaltet M_ViewShowImages ein/aus */
	void TogglePics();
	/** Schaltet Raster ein/aus */
	void ToggleRaster();
	/** Schaltet Rasterbenutzung ein/aus */
	void ToggleURaster();
	/** Schaltet Rahmenbearbeitung ein/aus */
	void ToggleFrameEdit();
	void slotSelect();
	/** Switch appMode */
	void setAppMode(int mode);
	void setAppModeByToggle(bool isOn, int newMode);
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
	void SetNewFont(const QString& nf);
	/** Setz die Zeichensatzgroesse */
	void setItemFSize(int id);
	void setFSizeMenu(int size);
	/** Farbeditor */
	void slotEditColors();
	/** Setzt den Pen-Tonwert */
	void setPenShade(int sh);
	/** Setzt den Brush-Tonwert */
	void setBrushShade(int sh);
	void setGradFill(int typ);
	void updtGradFill();
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
	/** Dupliziert das Element */
	void ObjektDup();
	/** Dupliziert das Element mehrfach*/
	void ObjektDupM();
	/** Reformatiert das Dokument */
	bool SetupDoc();
	void objectAttributes();
	void generateTableOfContents();
	void buildFontMenu();
	void slotPrefsOrg();
	void slotEditStyles();
	void saveStyles(StilFormate *dia);
	void slotEditLineStyles();
	void saveLStyles(LineFormate *dia);
	void setNewAbStyle(int a);
	void setAbsValue(int a);
	void selectItemsFromOutlines(int Page, int Item, bool single = false);
	void selectPagesFromOutlines(int Page);
	void doPrintPreview();
	void printPreview();
	void SaveAsEps();
	void reallySaveAsEps();
	void SaveAsPDF();
	void doSaveAsPDF();
	void setMainWindowActive();
	void setItemHoch(int h);
	void setStilvalue(int s);
	void setItemTypeStyle(int id);
	void slotElemRead(QString Name, int x, int y, bool art, bool loca, ScribusDoc* docc, ScribusView* vie);
	void slotChangeUnit(int art, bool draw = true);
	void NoFrameEdit();
	void ApplyMasterPage();
	void Apply_MasterPage(QString in, int Snr, bool reb = true);
	void GroupObj(bool showLockDia = true);
	void UnGroupObj();
	void StatusPic();
	void ModifyAnnot();
	void ToggleGuides();
	void ToggleBase();
	void ToggleUGuides();
	void HaveRaster(bool art);
	void EditTabs();
	void SearchText();
	void imageEditorExited();
	void callImageEditor();
	void docCheckToggle(bool visible);
	void scanDocument();
	void setUndoMode(bool isObjectSpecific);
	//! Apply a Lorem Ipsum to the each item in a selection
	void insertSampleText();
	void sendToLayer(int layerNumber);
	void updateItemLayerList();

signals:
	void TextISize(int);
	void TextIFont(QString);
	void TextUSval(int);
	void TextStil(int);
	void TextFarben(QString, QString, int, int);
	void TextScale(int);
	void TextScaleV(int);
	void TextBase(int);
	void TextShadow(int, int);
	void TextOutline(int);
	void TextUnderline(int, int);
	void TextStrike(int, int);

	void prefsChanged();

protected:
	bool eventFilter( QObject *o, QEvent *e );

private:
    /** init methods */
	void initSplash(bool showSplash);
	void setSplashStatus(const QString&);
	void initMenuBar(); // initMenuBar creates the menu_bar and inserts the menuitems
	void addDefaultWindowMenuItems(); // addDefaultWindowMenuItems adds the basic Windows menu items, excluding the actual list of windows
	void initStatusBar(); // setup the statusbar
	void initToolBars(); // setup the toolbars
	const bool initFonts(const bool showFontInfo);
	void initHyphenator();
	void initDefaultValues();
	void initKeyboardShortcuts();
	void initPalettes();
	void initScrapbook();
	void initCMS();
	void updateColorMenu(QProgressBar* progressBar=NULL);

	QString guiLanguage;
	QString recentFileMenuName;
	QString layerMenuName;
	bool scribusInitialized;
	QComboBox *ColorMenC;
	/** ShapeMenu enthaelt die Rahmenformen */
	QPopupMenu *ShapeMenu;
	/** FontMenu enthaelt die Fonts */
	QPopupMenu *FontMenu;
	FontCombo* FontSub;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	WerkToolBP* pdfToolBar;
	QToolButton* DatOpe;
	QToolButton* DatSav;
	QToolButton* DatClo;
	QToolButton* DatPri;
	QToolButton* DatPDF;
	QToolButton* DatNeu;
	int KeyMod;
	int toolbarMenuTools;
	int toolbarMenuPDFTools;
	int viewToolbars;
	int viewPropertiesPalette;
	int viewOutlinePalette;
	int viewNodePalette;
	int viewBpal;
	int viewLayerPalette;
	int viewPagePalette;
	int viewBopal;
	int viewUndoPalette;

	bool PalettesStat[10];
	bool GuidesStat[9];

	bool keyrep;
	/** @brief Tells if an arrow key is pressed down */
	bool _arrowKeyDown;
	/** @brief tells the undo mode */
	bool objectSpecificUndo;

	void addNewPages(int wo, int where, int numPages, double height, double width, int orient, QString siz, bool mov, QStringList* basedOn = 0);
	QMap<int,QString> FontID;
	int HaveGS;
	void *PSDriver;
	int DocNr;
	UndoManager *undoManager;
	PrefsManager *prefsManager;
	bool PrinterUsed;
	struct PDe {
					QString Pname;
					QString Dname;
					QString Command;
				} PDef ;
	TOCGenerator *tocGenerator;
};

#endif

