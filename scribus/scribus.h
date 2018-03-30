/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QActionGroup>
#include <QClipboard>
#include <QImage>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMap>
#include <QMultiHash>
#include <QPointer>
#include <QPushButton>
#include <QProcess>
#include <QString>

class QCloseEvent;
class QDragEnterEvent;
class QDropEvent;
class QEvent;
class QKeyEvent;
class QLabel;
class QMdiArea;
class QMdiSubWindow;
class QQuickView;

// application specific includes
#include "scribusapi.h"
#include "scribusdoc.h"
#include "styleoptions.h"
#include "ui/customfdialog.h"
#include "ui/scmessagebox.h"

class ActionManager;
class AlignDistributePalette;
class AppModeHelper;
class Autoforms;
class Biblio;
class BookPalette;
class CharSelect;
class CheckDocument;
class ColorCombo;
class DownloadsPalette;
class EditToolBar;
class FileToolBar;
class FontCombo;
class FormatsManager;
class GuideManager;
class HelpBrowser;
class InlinePalette;
class LayerPalette;
class MarksManager;
class Measurements;
class ModeToolBar;
class NodePalette;
class NotesStylesEditor;
class OutlinePalette;
class PDFToolBar;
class PSLib;
class PageItem;
class PagePalette;
class PageSelector;
class PrefsContext;
class PrefsManager;
class PropertiesPalette;
class ResourceManager;
class ScMWMenuManager;
class ScToolBar;
class ScrAction;
class ScrSpinBox;
class ScribusCore;
class ScribusDoc;
class ScribusMainWindow;
class ScribusQApp;
class ScribusWin;
class SimpleState;
class StoryEditor;
class StyleManager;
class SymbolPalette;
class TextPalette;
class TOCGenerator;
class UndoManager;
class UndoPalette;
class UndoState;
class ViewToolBar;

extern SCRIBUS_API ScribusQApp* ScQApp;

/**
  * \brief This Class is the base class for your application. It sets up the main
  * window and providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class ScribusView is
  * created which creates your view.
  */
class SCRIBUS_API ScribusMainWindow : public QMainWindow, public UndoObject
{
	Q_OBJECT

public:
	/** \brief constructor */
	ScribusMainWindow();
	/** \brief destructor */
	~ScribusMainWindow();
	/*!
	* \retval 0 - ok, 1 - no fonts, ...
	*/
	int initScMW(bool primaryMainwWindow);
	void addScToolBar(ScToolBar *tb, QString name);
	bool warningVersion(QWidget *parent);
	void SetShortCut();
	void startUpDialog();
	void setDefaultPrinter(const QString&, const QString&, const QString&);
	void getDefaultPrinter(QString& name, QString& file, QString& command);

	inline bool scriptIsRunning(void) const { return (m_ScriptRunning > 0); }
	inline void setScriptRunning(bool value) { m_ScriptRunning += (value ? 1 : -1); }

	ScribusDoc *doFileNew(double width, double height, double topMargin, double leftMargin, double rightMargin, double bottomMargin, double columnDistance, double columnCount, bool autoTextFrames, int pageArrangement, int unitIndex, int firstPageLocation, int orientation, int firstPageNumber, const QString& defaultPageSize, bool requiresGUI, int pageCount=1, bool showView=true, int marginPreset=0);
	ScribusDoc *newDoc(double width, double height, double topMargin, double leftMargin, double rightMargin, double bottomMargin, double columnDistance, double columnCount, bool autoTextFrames, int pageArrangement, int unitIndex, int firstPageLocation, int orientation, int firstPageNumber, const QString& defaultPageSize, bool requiresGUI, int pageCount=1, bool showView=true, int marginPreset=0);
	bool DoFileSave(const QString& fileName, QString* savedFileName = NULL);
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *ce);
	void keyPressEvent(QKeyEvent *k);
	void keyReleaseEvent(QKeyEvent *k);
	void inputMethodEvent (QInputMethodEvent *event);
	QVariant inputMethodQuery ( Qt::InputMethodQuery query ) const ;
	void requestUpdate(int);
	void setTBvals(PageItem *currItem);
	int ShowSubs();
	void applyNewMaster(QString name);
	void updateRecent(QString fn);
	void doPasteRecent(QString data);
	bool getPDFDriver(const QString & filename, const QString & name, int components, const std::vector<int> & pageNumbers, const QMap<int, QImage> & thumbs, QString& error, bool* cancelled = NULL);
	bool DoSaveAsEps(QString fn, QString& error);
	QString CFileDialog(QString workingDirectory = ".", QString dialogCaption = "", QString fileFilter = "", QString defNa = "",
						int optionFlags = fdExistingFiles, bool *useCompression = 0, bool *useFonts = 0, bool *useProfiles = 0);
	/*! \brief Recalculate the colors after changing CMS settings.
	Call the appropriate document function and then update the GUI elements.
	\param dia optional progress widget */
	void recalcColors(QProgressBar *dia = 0);
	void SwitchWin();
	void RestoreBookMarks();
	QStringList  scrapbookNames();
	void updateLayerMenu();
	void emergencySave();
	QStringList findRecoverableFile();
	bool recoverFile(QStringList foundFiles);

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
	void restoreGrouping(SimpleState *state, bool isUndo);
	void restoreUngrouping(SimpleState *state, bool isUndo);
	void restoreAddPage(SimpleState *state, bool isUndo);
	void restoreDeletePage(SimpleState *state, bool isUndo);
	void setPreviewToolbar();
	void updateFromDrop();
	struct CopyContentsBuffer contentsBuffer;
	bool internalCopy;
	QString internalCopyBuffer;
	int  HaveDoc;
	PrefsContext* dirs;
	/** \brief view is the main widget which represents your working area. The View
	 * class should handle all events of the view widget.  It is kept empty so
	 * you can create your view according to your application's needs by
	 * changing the view class.
	 */
	ScribusView *view;
	/** \brief doc represents your actual document and is created only once. It keeps
	 * information such as filename and does the serialization of your files.
	 */
	ScribusDoc *doc;
	/** \brief private doc for managing default patterns. */
	ScribusDoc* m_doc;


	QProgressBar* mainWindowProgressBar;
	ScrSpinBox* zoomSpinBox; //zoom spinbox at bottom of view
	PageSelector* pageSelector; //Page selector at bottom of view
	QPushButton *zoomDefaultToolbarButton;
	QPushButton *zoomOutToolbarButton;
	QPushButton *zoomInToolbarButton;
	QComboBox *layerMenu; //Menu for layers at bottom of view
	QComboBox *unitSwitcher; //Menu for units at bottom of view
	EditToolBar *editToolBar;
	FileToolBar *fileToolBar;
	ModeToolBar* modeToolBar;
	PDFToolBar* pdfToolBar;
	ViewToolBar* viewToolBar;
	QLabel* mainWindowXPosLabel;
	QLabel* mainWindowXPosDataLabel;
	QLabel* mainWindowYPosLabel;
	QLabel* mainWindowYPosDataLabel;
	GuideManager *guidePalette;
	CharSelect *charPalette;
	PropertiesPalette *propertiesPalette;
	TextPalette *textPalette;
	MarksManager *marksManager;
	NotesStylesEditor *nsEditor;
	NodePalette *nodePalette;
	OutlinePalette *outlinePalette;
	Biblio *scrapbookPalette;
	LayerPalette* layerPalette;
	PagePalette *pagePalette;
	BookPalette *bookmarkPalette;
	DownloadsPalette *downloadsPalette;
	SymbolPalette *symbolPalette;
	InlinePalette *inlinePalette;
	Measurements* measurementPalette;
	CheckDocument * docCheckerPalette;
	UndoPalette* undoPalette;
	AlignDistributePalette *alignDistributePalette;
	ResourceManager *resourceManager;
	StoryEditor* storyEditor;
	StoryEditor* CurrStED;
	QMdiArea *mdiArea;
	ScribusWin* ActWin;
	QClipboard *ClipB;
	QString LoadEnc;
	AppModeHelper *appModeHelper;

	QMap<QString, QPointer<ScrAction> > scrActions;
	QMap<QString, QPointer<ScrAction> > scrRecentFileActions;
	QMap<QString, QPointer<ScrAction> > scrWindowsActions;
	QMap<QString, QPointer<ScrAction> > scrScrapActions;
	QMap<QString, QPointer<ScrAction> > scrLayersActions;
	QMap<QString, QPointer<ScrAction> > scrRecentPasteActions;
	QMap<QString, QPointer<ScToolBar> > scrToolBars;
	QMultiHash<QString, QActionGroup*> scrActionGroups;
	ScMWMenuManager* scrMenuMgr;
	ActionManager* actionManager;
	QStringList RecentDocs;
	QStringList patternsDependingOnThis;

public slots:
	void languageChange();
	void statusBarLanguageChange();
	void specialActionKeyEvent(int unicodevalue);
	void newView();
	void ToggleStickyTools();
	void ToggleAllGuides();
	void ToggleAllPalettes();
	void slotStoryEditor(bool fromTable);
	void slotCharSelect();
	void ImageEffects();
	QString fileCollect(const bool compress = false, const bool withFonts = false, const bool withProfiles = false, const QString& newDirectory=QString::null);
	void AddBookMark(PageItem *ite);
	void DelBookMark(PageItem *ite);
	void BookMarkTxT(PageItem *ite);
	void StoreBookmarks();
	void setStatusBarMousePosition(double xp, double yp);
	void setStatusBarTextPosition(double base, double xp);
	void setStatusBarTextSelectedItemInfo();
	void setTempStatusBarText(const QString &text);
	void setStatusBarInfoText(QString newText);
	bool DoFileClose();
	void windowsMenuAboutToShow();
	//! \brief Handle the Extras menu for its items availability.
	void extrasMenuAboutToShow();
	void newActWin(QMdiSubWindow *w);
	void closeActiveWindowMasterPageEditor();
	void updateActiveWindowCaption(const QString &newCaption);
	void windowsMenuActivated(int id);
	void PutScrap(int scID);
	void PutToInline(QString buffer);
	void PutToInline();
	void PutToPatterns();
	void ConvertToSymbol();
	void changeLayer(int);
	void setLayerMenuText(const QString &);
	void showLayer();
	void slotSetCurrentPage(int Seite);
	void setCurrentPage(int p);
	void ManageJava();
	void editSelectedSymbolStart();
	void editSymbolStart(QString temp);
	void editSymbolEnd();
	void editInlineStart(int id);
	void editInlineEnd();
	void editMasterPagesStart(QString temp = "");
	void editMasterPagesEnd();
	/** \brief generate a new document in the current view */
	bool slotFileNew();
	void newFileFromTemplate();
	bool slotPageImport();
	bool loadPage(QString fileName, int Nr, bool Mpa, const QString& renamedPageName=QString::null);
	void GotoLa(int l);
	void slotGetContent();
	void slotGetContent2(); // kk2006
	void slotGetClipboardImage();
	void toogleInlineState();
	/*!
	\author Franz Schmid
	\brief Appends a Textfile to the Text in the selected Textframe at the Cursorposition
	*/
	void slotFileAppend();

	void removeRecent(QString fn, bool fromFileWatcher = false);
	void removeRecentFromWatcher(QString filename);
	void loadRecent(QString filename);
	void rebuildRecentFileMenu();
	void rebuildRecentPasteMenu();
	void rebuildScrapbookMenu();
	void pasteRecent(QString fn);
	void pasteFromScrapbook(QString fn);
	void importVectorFile();
	void rebuildLayersList();
	bool slotFileOpen();
	bool loadDoc(QString);
	/**
	 * @brief Do post loading functions
	 */
	bool postLoadDoc();
	/** \brief save a document */
	bool slotFileSave();
	/** \brief save a document under a different filename*/
	bool slotFileSaveAs();
	void slotFileRevert();
	/** \brief Sichert den Text eines Elements */
	void SaveText();
	/** \brief close the actual file */
	bool slotFileClose();
	/** \brief print the actual file */
	void slotFilePrint();
	void slotReallyPrint();
	void slotEndSpecialEdit();
	/*!
	\author Franz Schmid
	\brief Generate and print PostScript from a doc
	\param options PrintOptions struct to control all settings
	\param error   Error Message in case of failure
	\sa ScribusMainWindow::slotFilePrint()
	\retval bool True for success */
	bool doPrint(PrintOptions &options, QString& error);
	/** \brief exits the application */
	void slotFileQuit();
	/** \brief put the marked text/object into the clipboard and remove
	 * it from the document */
	void slotEditCut();
	/** \brief put the marked text/object into the clipboard*/
	void slotEditCopy();
	/** \brief paste the clipboard into the document*/
	void slotEditPaste();
	void slotEditCopyContents();
	void slotEditPasteContents(int absolute=0);
	void EnableTxEdit();
	void DisableTxEdit();
	void SelectAll(bool docWideSelect=false);
	void SelectAllOnLayer();
	void deselectAll();
	void ClipChange();
	/** \brief shows an about dialog*/
	void slotHelpAbout();
	void slotHelpAboutPlugins();
    void slotHelpAboutQt();
	void slotHelpCheckUpdates();
	void slotRaiseOnlineHelp();
	void slotOnlineHelp(const QString & jumpToSection=QString::null, const QString & jumpToFile=QString::null);
	void slotOnlineHelpClosed();
	void slotResourceManager();
	void ToggleTips();
	void ToggleMouseTips();
	/** \brief Erzeugt eine neue Seite */
	void slotNewPageP(int wo, QString templ);
	void slotNewPageM();
	void slotNewMasterPage(int w, const QString &);
	void slotNewPage(int w, const QString& masterPageName=QString::null, bool mov = true);
	void duplicateToMasterPage();
	/** \brief Loescht die aktuelle Seite */
	void deletePage();
	/**
	 * \brief Delete pages
	 * @param from First page to delete
	 * @param to Last page to delete
	 */
	void deletePage(int from, int to);
	void deletePage2(int pg);
	/** \brief Verschiebt Seiten */
	void movePage();
	void copyPage();
	void changePageProperties();
	/*!
	\author Craig Bradney
	\date Sun 30 Jan 2005
	\brief Zoom the view.
	Take the ScMW zoom actions and pass the view a %. Actions have whole number values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
	\param zoomFactor Value stored in the ScrAction.
	 */
	void slotZoom(double zoomFactor); // 20, 50, 100, or -100 for Fit
	/** \brief Schaltet Raender ein/aus */
	void toggleMarks();
	void toggleBleeds();
	void toggleFrames();
	void toggleLayerMarkers();
	void toggleTextLinks();
	void toggleTextControls();
	void toggleColumnBorders();
	void toggleRulers();
	void toggleRulerMode();
	void togglePagePalette();
	void toggleUndoPalette();
	void setUndoPalette(bool visible);
	void toggleCheckPal();
	/** \brief Schaltet M_ViewShowImages ein/aus */
	void toggleImageVisibility();
	/** \brief Schaltet Raster ein/aus */
	void toggleGrid();
	/** \brief Schaltet Rasterbenutzung ein/aus */
	void toggleSnapGrid();
	/** \brief Schaltet Rahmenbearbeitung ein/aus */
	void toggleNodeEdit();
	void slotSelect();
	/** \brief Switch appMode
	\param mode TODO learn modes*/
	void setAppModeByToggle(bool isOn, int newMode);
	/** \brief Neues Dokument erzeugt */
	void HaveNewDoc();
	void HaveNewSel();
	/** Dokument ist geaendert worden */
	void slotDocCh(bool reb = true);
	/** Setzt die Abstufung */
	//void setItemShade(int id);
	/** Setzt den Font */
	/** Korrigiert das FontMenu */
	void SetNewFont(const QString& nf);
	/** Setz die Zeichensatzgroesse */
	void setItemFontSize(int fontSize);
	void setItemLanguage(QString language);
	/** Color Replacement */
	void slotReplaceColors();
	/** Style Manager */

	/** Erzeugt einen Rahmen */
	void MakeFrame(int f, int c, double *vals);
	/** Duplicate current item */
	void duplicateItem();
	/** Multiple duplicate current item*/
	void duplicateItemMulti();

	void objectAttributes();
	void getImageInfo();
	void generateTableOfContents();
	void updateDocument();

	void setNewAlignment(int i);
	void setNewDirection(int i);
	void setNewParStyle(const QString& name);
	void setNewCharStyle(const QString& name);
	void setAlignmentValue(int i);
	void setDirectionValue(int i);
	void editItemsFromOutlines(PageItem *ite);
	//0= center, 1 = top left.
	void selectItemsFromOutlines(PageItem *ite, bool single = false, int position = 0);
	void selectItemFromOutlines(PageItem *ite, bool single, int cPos);
	void selectPagesFromOutlines(int ScPage);
	void doPrintPreview();
	void printPreview();
	void SaveAsEps();
	void reallySaveAsEps();
	void SaveAsPDF();
	void doSaveAsPDF();
	void setMainWindowActive();
	void setItemEffects(int h);
	void setStyleEffects(int s);
	void setItemTypeStyle(int id);
	void slotElemRead(QString Name, double x, double y, bool art, bool loca, ScribusDoc* docc, ScribusView* vie);
	void slotChangeUnit(int art, bool draw = true);
	/*!
	 * @brief Apply master pages from the Apply Master Page dialog
	 * @todo Make this work with real page numbers, negative numbers and document sections when they are implemented
	*/
	void ApplyMasterPage();
	void Apply_MasterPage(QString pageName, int pageNumber, bool reb = true);
	void GroupObj(bool showLockDia = true);
	void UnGroupObj();
	void AdjustGroupObj();
	void StatusPic();
	void ModifyAnnot();
	void toggleGuides();
	void toggleBase();
	void toggleSnapGuides();
	void toggleSnapElements();
	void SetSnapElements(bool b);
	void EditTabs();
	void SearchText();
	/*! \brief call gimp and wait upon completion */
	void callImageEditor();
	void docCheckToggle(bool visible);
	//! \brief Scan a document for errors, return true on errors found
	bool scanDocument();
	void setUndoMode(bool isObjectSpecific);
	//! \brief Apply a Lorem Ipsum to the each item in a selection
	void insertSampleText();
	void updateItemLayerList();
	void updateColorLists();
	/*! \brief Change Preferences dialog.
	See prefsOrg for more info. It's very similar to docSetup/slotDocSetup. */
	void slotPrefsOrg();
	/** \brief Refromat the document when user click "OK" in ReformDoc dialog.
	See docSetup() for more info. */
	void slotDocSetup();
	//! \brief Insert a frame friendly dialog
	void slotInsertFrame();
	//! \brief Transform an item
	void slotItemTransform();
	//! \brief manages paints
	void manageColorsAndFills();
	//! \brief allow SE to get the SM for edit stlyes
	StyleManager *styleMgr() const {return m_styleManager;};
	//! \brief drawnew, call palettes to update for new page layout
	void updateGUIAfterPagesChanged();
	/**
	 * Enables/disables the actions in the "Table" menu.
	 *
	 * This has a functions of its own, since it needs to be called from the table edit
	 * canvas modes/gestures.
	 */
	void updateTableMenuActions();
	void emitUpdateRequest(int updateFlags) { emit UpdateRequest(updateFlags); }

	//inserting marks
	void slotInsertMark2Mark() { insertMark(MARK2MarkType); }
	void slotInsertMarkAnchor() { insertMark(MARKAnchorType); }
	void slotInsertMarkVariableText() { insertMark(MARKVariableTextType); }
	void slotInsertMarkItem() { insertMark(MARK2ItemType); }
	void slotInsertMarkNote();
	void slotInsertMarkIndex() { insertMark(MARKIndexType); }
	void slotEditMark();
	//connected to signal emitted by actions when "Update Marks" menu item is triggered
	void slotUpdateMarks();
	bool editMarkDlg(Mark *mrk, PageItem_TextFrame* currItem = NULL);
//	void testQT_slot1(QString);
//	void testQT_slot2(double);
//	void testQT_slot3(int);
//	void testQT_slot4();
	void changePreviewQuality(int index);
	void enablePalettes(bool b);
	void ToggleFrameEdit();
	void NoFrameEdit();

signals:
	void TextStyle(const ParagraphStyle&);
//deprecated: (av)
	void TextEffects(int);
	void UpdateRequest(int updateFlags);
	void changeLayers(int);

protected:
	/*!
	\brief Receive key events from palettes such as palette hiding events. Possibly easier way but this is cleaner than before. No need to modify all those palettes and each new one in future.
	 */
	bool eventFilter( QObject *o, QEvent *e );
	virtual void dragEnterEvent( QDragEnterEvent* e);
	virtual void dropEvent( QDropEvent* e);

private:
    /** init methods */
	void initSplash(bool showSplash);
	void initMdiArea();
	void initMenuBar(); // initMenuBar creates the menu_bar and inserts the menuitems
	void createMenuBar();
	void addDefaultWindowMenuItems(); // addDefaultWindowMenuItems adds the basic Windows menu items, excluding the actual list of windows
	void initStatusBar(); // setup the statusbar
	void initToolBars(); // setup the toolbars
	void setStyleSheet(); //set stylesheet for app
	//Returns false when there are no fonts
	void initHyphenator();
	void initDefaultValues();
	void initKeyboardShortcuts();
	void initPalettes();
	void initScrapbook();

	void updateColorMenu(QProgressBar* progressBar=NULL);

	int m_ScriptRunning;

	QLabel* m_mainWindowStatusLabel;
	QString m_statusLabelText;
	//QPixmap noIcon;

	int m_toolbarMenuTools;
	int m_toolbarMenuPDFTools;
	int m_viewToolbars;
	int m_viewPropertiesPalette;
	int m_viewPropertiesPaletteText;
	int m_viewOutlinePalette;
	int m_viewNodePalette;
	int m_viewBpal;
	int m_viewLayerPalette;
	int m_viewPagePalette;
	int m_viewBopal;
	int m_viewUndoPalette;

	bool m_palettesStatus[12];
	bool m_guidesStatus[13];

	bool m_keyrep;
	/** @brief Tells if an arrow key is pressed down */
	bool m__arrowKeyDown;
	/** @brief tells the undo mode */
	bool m_objectSpecificUndo;

	//CB: #8212: add overrideMasterPageSizing, however default to true for compatibility with other calls.. for now
	void addNewPages(int wo, int where, int numPages, double height, double width, int orient, QString siz, bool mov, QStringList* basedOn = 0, bool overrideMasterPageSizing=true);

	int m_DocNr;
	bool m_PrinterUsed;
	struct PDe {
					QString Pname;
					QString Dname;
					QString Command;
				} PDef ;
	TOCGenerator *m_tocGenerator;
	int m_storedPageNum;
	int m_storedViewXCoor;
	int m_storedViewYCoor;
	double m_storedViewScale;
	StyleManager *m_styleManager;
	UndoManager *m_undoManager;
	PrefsManager *m_prefsManager;
	FormatsManager *m_formatsManager;

	QPointer<HelpBrowser> m_helpBrowser;
	QString m_osgFilterString;

	void insertMark(MarkType);
	bool insertMarkDialog(PageItem_TextFrame* item, MarkType mT, ScItemsState* &is);
	int m_marksCount; //remember marks count from last call
	bool m_WasAutoSave;
	bool m_pagePalVisible;

	//QQuickView *m_qqview;
};

#endif
