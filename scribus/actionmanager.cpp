/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Apr 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   mainWindow program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "actionmanager.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "text/storytext.h"
#include "undomanager.h"
#include "urllauncher.h"
#include "util_icon.h"


QMap<QString, QKeySequence> ActionManager::defKeys;
QVector< QPair<QString, QStringList> > ActionManager::defMenus;
QVector< QPair<QString, QStringList> > ActionManager::defNonMenuActions;

ActionManager::ActionManager ( QObject * parent ) :
	QObject ( parent),
	mainWindow(0)
{
}

ActionManager::~ActionManager()
{
	while (!scrActions->isEmpty())
	{
		ScrAction *value = (*scrActions->begin());
		scrActions->erase(scrActions->begin());
		delete value;
 	}
	scrActions->clear();
	delete modeActionNames;
	delete nonEditActionNames;
	delete unicodeCharActionNames;
}

void ActionManager::init(ScribusMainWindow *mw)
{
	mainWindow=mw;
	scrActions=&(mainWindow->scrActions);
	scrActionGroups=&(mainWindow->scrActionGroups);
	modeActionNames=new QStringList();
	nonEditActionNames=new QStringList();
	unicodeCharActionNames=new QStringList();
	undoManager = UndoManager::instance();

#ifdef Q_WS_MAC
	noIcon = loadIcon("noicon.xpm");
#endif

	createActions();
	languageChange();
}

void ActionManager::createActions()
{
	initFileMenuActions();
	initEditMenuActions();
	initStyleMenuActions();
	initItemMenuActions();
	initInsertMenuActions();
	initPageMenuActions();
	initViewMenuActions();
	initToolsMenuActions();
	initExtrasMenuActions();
	initWindowsMenuActions();
	initScriptMenuActions();
	initHelpMenuActions();
	initUnicodeActions(scrActions, mainWindow, unicodeCharActionNames);
	enableUnicodeActions(scrActions, false);
	initSpecialActions();
	
}

void ActionManager::initFileMenuActions()
{
	QString name;
	//File Menu
	name="fileNew";
	scrActions->insert(name, new ScrAction(loadIcon("16/document-new.png"), loadIcon("22/document-new.png"), "", defKeys[name], mainWindow));
	name="fileOpen";
	scrActions->insert(name, new ScrAction(loadIcon("16/document-open.png"), loadIcon("22/document-open.png"), "", defKeys[name], mainWindow));
	name="fileClose";
	scrActions->insert(name, new ScrAction(loadIcon("16/close.png"), loadIcon("22/close.png"), "", defKeys[name], mainWindow));
	name="fileSave";
	scrActions->insert(name, new ScrAction(loadIcon("16/document-save.png"), loadIcon("22/document-save.png"), "", defKeys[name], mainWindow));
	name="fileSaveAs";
	scrActions->insert(name, new ScrAction(loadIcon("16/document-save-as.png"), loadIcon("22/document-save-as.png"), "", defKeys[name], mainWindow));
	name="fileRevert";
	scrActions->insert(name, new ScrAction(loadIcon("revert.png"), QPixmap(), "", defKeys[name], mainWindow));
	name="fileCollect";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	//File Import Menu
	name="fileImportText";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="fileImportText2";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="fileImportAppendText";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="fileImportImage";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));

	//File Export Menu
	name="fileExportText";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="fileExportAsEPS";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="fileExportAsPDF";
	scrActions->insert(name, new ScrAction(loadIcon("acroread16.png"), loadIcon("acroread22.png"), "", defKeys[name], mainWindow));
	//Rest of File Menu
	name="fileDocSetup";
	scrActions->insert(name, new ScrAction(loadIcon("16/document-properties.png"), loadIcon("22/document-properties.png"), "", defKeys[name], mainWindow));
	name="filePreferences";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)[name]->setMenuRole(QAction::PreferencesRole);
	name="filePrint";
	scrActions->insert(name, new ScrAction(loadIcon("16/document-print.png"), loadIcon("22/document-print.png"), "", defKeys[name], mainWindow));
	name="PrintPreview";
	scrActions->insert(name, new ScrAction(loadIcon("16/document-print-preview.png"), loadIcon("22/document-print-preview.png"), "", defKeys[name], mainWindow));
	name="fileQuit";
	scrActions->insert(name, new ScrAction(loadIcon("exit.png"), QPixmap(), "", defKeys[name], mainWindow));
	(*scrActions)[name]->setMenuRole(QAction::QuitRole);

	//Connect our signals and slots
	//File Menu
	connect( (*scrActions)["fileNew"], SIGNAL(triggered()), mainWindow, SLOT(slotFileNew()) );
	connect( (*scrActions)["fileOpen"], SIGNAL(triggered()), mainWindow, SLOT(slotDocOpen()) );
	connect( (*scrActions)["fileClose"], SIGNAL(triggered()), mainWindow, SLOT(slotFileClose()) );
	connect( (*scrActions)["filePrint"], SIGNAL(triggered()), mainWindow, SLOT(slotFilePrint()) );
	connect( (*scrActions)["PrintPreview"], SIGNAL(triggered()), mainWindow, SLOT(printPreview()) );
	connect( (*scrActions)["fileSave"], SIGNAL(triggered()), mainWindow, SLOT(slotFileSave()) );
	connect( (*scrActions)["fileSaveAs"], SIGNAL(triggered()), mainWindow, SLOT(slotFileSaveAs()) );
	connect( (*scrActions)["fileDocSetup"], SIGNAL(triggered()), mainWindow, SLOT(slotDocSetup()) );
	connect( (*scrActions)["filePreferences"], SIGNAL(triggered()), mainWindow, SLOT(slotPrefsOrg()) );
	connect( (*scrActions)["fileRevert"], SIGNAL(triggered()), mainWindow, SLOT(slotFileRevert()) );
	connect( (*scrActions)["fileCollect"], SIGNAL(triggered()), mainWindow, SLOT(Collect()) );
	connect( (*scrActions)["fileQuit"], SIGNAL(triggered()), mainWindow, SLOT(slotFileQuit()) );
	//File Import Menu
	connect( (*scrActions)["fileImportText"], SIGNAL(triggered()), mainWindow, SLOT(slotGetContent()) );
	connect( (*scrActions)["fileImportText2"], SIGNAL(triggered()), mainWindow, SLOT(slotGetContent2()) );
	connect( (*scrActions)["fileImportAppendText"], SIGNAL(triggered()), mainWindow, SLOT(slotFileAppend()) );
	connect( (*scrActions)["fileImportImage"], SIGNAL(triggered()), mainWindow, SLOT(slotGetContent()) );
	//File Export Menu
	connect( (*scrActions)["fileExportText"], SIGNAL(triggered()), mainWindow, SLOT(SaveText()) );
	connect( (*scrActions)["fileExportAsEPS"], SIGNAL(triggered()), mainWindow, SLOT(SaveAsEps()) );
	connect( (*scrActions)["fileExportAsPDF"], SIGNAL(triggered()), mainWindow, SLOT(SaveAsPDF()) );
	//The rest are plugins
}

void ActionManager::initEditMenuActions()
{
	QString name;
	//Edit Menu
	name="editUndoAction";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/edit-undo.png"), loadIcon("22/edit-undo.png"), "", defKeys[name], mainWindow, 1));
	name="editRedoAction";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/edit-redo.png"), loadIcon("22/edit-redo.png"), "", defKeys[name], mainWindow, 1));
	name="editActionMode";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)["editActionMode"]->setToggleAction(true);

	name="editCut";
	scrActions->insert(name, new ScrAction(loadIcon("16/edit-cut.png"), loadIcon("22/edit-cut.png"), "", defKeys[name], mainWindow));
	name="editCopy";
	scrActions->insert(name, new ScrAction(loadIcon("16/edit-copy.png"), loadIcon("22/edit-copy.png"), "", defKeys[name], mainWindow));
	name="editPaste";
	scrActions->insert(name, new ScrAction(loadIcon("16/edit-paste.png"), loadIcon("22/edit-paste.png"), "", defKeys[name], mainWindow));
	name="editCopyContents";
	scrActions->insert(name, new ScrAction(loadIcon("16/edit-copy.png"), loadIcon("22/edit-copy.png"), "", defKeys[name], mainWindow));
	name="editPasteContents";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/edit-paste.png"), QPixmap(), "", defKeys[name], mainWindow, 0));
	name="editPasteContentsAbs";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/edit-paste.png"), QPixmap(), "", defKeys[name], mainWindow, 1));
	name="editClearContents";
	scrActions->insert(name, new ScrAction(loadIcon("16/edit-delete.png"), loadIcon("22/edit-delete.png"), "", defKeys[name], mainWindow));
	name="editSelectAll";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="editDeselectAll";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="editSearchReplace";
	scrActions->insert(name, new ScrAction(loadIcon("16/edit-find-replace.png"), loadIcon("22/edit-find-replace.png"), "", defKeys[name], mainWindow));
	name="editEditWithImageEditor";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="editEditWithLatexEditor";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="editColors";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="editPatterns";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="editStyles";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="editMasterPages";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="editJavascripts";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));

	(*scrActions)["editStyles"]->setToggleAction(true);

	connect( (*scrActions)["editUndoAction"], SIGNAL(triggeredData(int)) , undoManager, SLOT(undo(int)) );
	connect( (*scrActions)["editRedoAction"], SIGNAL(triggeredData(int)) , undoManager, SLOT(redo(int)) );
	connect( (*scrActions)["editActionMode"], SIGNAL(toggled(bool)), mainWindow, SLOT(setUndoMode(bool)) );
	connect( (*scrActions)["editCut"], SIGNAL(triggered()), mainWindow, SLOT(slotEditCut()) );
	connect( (*scrActions)["editCopy"], SIGNAL(triggered()), mainWindow, SLOT(slotEditCopy()) );
	connect( (*scrActions)["editPaste"], SIGNAL(triggered()), mainWindow, SLOT(slotEditPaste()) );
	connect( (*scrActions)["editCopyContents"], SIGNAL(triggered()), mainWindow, SLOT(slotEditCopyContents()) );
	connect( (*scrActions)["editPasteContents"], SIGNAL(triggeredData(int)), mainWindow, SLOT(slotEditPasteContents(int)) );
	connect( (*scrActions)["editPasteContentsAbs"], SIGNAL(triggeredData(int)), mainWindow, SLOT(slotEditPasteContents(int)) );
	connect( (*scrActions)["editSelectAll"], SIGNAL(triggered()), mainWindow, SLOT(SelectAll()) );
	connect( (*scrActions)["editDeselectAll"], SIGNAL(triggered()), mainWindow, SLOT(deselectAll()) );
	connect( (*scrActions)["editSearchReplace"], SIGNAL(triggered()), mainWindow, SLOT(SearchText()) );
	connect( (*scrActions)["editEditWithImageEditor"], SIGNAL(triggered()), mainWindow, SLOT(callImageEditor()) );
	connect( (*scrActions)["editEditWithLatexEditor"], SIGNAL(triggered()), mainWindow, SLOT(callImageEditor()) );
	connect( (*scrActions)["editColors"], SIGNAL(triggered()), mainWindow, SLOT(slotEditColors()) );
	connect( (*scrActions)["editPatterns"], SIGNAL(triggered()), mainWindow, SLOT(managePatterns()) );
	connect( (*scrActions)["editMasterPages"], SIGNAL(triggered()), mainWindow, SLOT(manageMasterPages()) );
	connect( (*scrActions)["editJavascripts"], SIGNAL(triggered()), mainWindow, SLOT(ManageJava()) );
}

void ActionManager::initStyleMenuActions()
{
	QString name;
	//Text Size actions
	(*scrActionGroups).insert("fontSize", new QActionGroup(mainWindow));
	name="fontSizeOther";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("fontSize"), -1));
	connect( (*scrActions)["fontSizeOther"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemFSize(int)));

	int font_sizes[] = {7, 9, 10, 11, 12, 14, 18, 24, 36, 48, 60, 72};
	size_t f_size = sizeof(font_sizes) / sizeof(*font_sizes);
	for (uint s = 0; s < f_size; ++s)
	{
		QString fontSizeName=QString("fontSize%1").arg(font_sizes[s]);
		scrActions->insert(fontSizeName, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("fontSize"), font_sizes[s]));
		(*scrActions)[fontSizeName]->setToggleAction(true);
		connect( (*scrActions)[fontSizeName], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemFSize(int)));
	}

	//Alignment actions
	name="alignLeft";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0));
	name="alignCenter";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 1));
	name="alignRight";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 2));
	name="alignBlock";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 3));
	name="alignForced";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 4));

	(*scrActions)["alignLeft"]->setToggleAction(true);
	(*scrActions)["alignCenter"]->setToggleAction(true);
	(*scrActions)["alignRight"]->setToggleAction(true);
	(*scrActions)["alignBlock"]->setToggleAction(true);
	(*scrActions)["alignForced"]->setToggleAction(true);

	connect( (*scrActions)["alignLeft"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setNewAlignment(int)));
	connect( (*scrActions)["alignCenter"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setNewAlignment(int)));
	connect( (*scrActions)["alignRight"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setNewAlignment(int)));
	connect( (*scrActions)["alignBlock"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setNewAlignment(int)));
	connect( (*scrActions)["alignForced"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setNewAlignment(int)));

	//Shade actions
	scrActionGroups->insert("shade", new QActionGroup(mainWindow));
	name="shadeOther";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("shade"), -1));
	connect( (*scrActions)["shadeOther"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemShade(int)));
	for (uint i=0; i<=100 ; i+=10)
	{
		QString shadeName=QString("shade%1").arg(i);
		scrActions->insert(shadeName, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("shade"), i));
		(*scrActions)[shadeName]->setToggleAction(true);
		connect( (*scrActions)[shadeName], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemShade(int)));
	}

	//Type Effects actions
	scrActionGroups->insert("typeEffects", new QActionGroup(mainWindow));
	name="typeEffectNormal";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 0));
	name="typeEffectUnderline";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 1));
	name="typeEffectUnderlineWords";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 8));
	name="typeEffectStrikeThrough";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 2));
	name="typeEffectAllCaps";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 7));
	name="typeEffectSmallCaps";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 3));
	name="typeEffectSuperscript";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 4));
	name="typeEffectSubscript";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 5));
	name="typeEffectOutline";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 6));
	name="typeEffectShadow";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], (*scrActionGroups).value("typeEffects"), 9));
	(*scrActions)["typeEffectNormal"]->setToggleAction(true);
	(*scrActions)["typeEffectUnderline"]->setToggleAction(true);
	(*scrActions)["typeEffectUnderlineWords"]->setToggleAction(true);
	(*scrActions)["typeEffectStrikeThrough"]->setToggleAction(true);
	(*scrActions)["typeEffectAllCaps"]->setToggleAction(true);
	(*scrActions)["typeEffectSmallCaps"]->setToggleAction(true);
	(*scrActions)["typeEffectSuperscript"]->setToggleAction(true);
	(*scrActions)["typeEffectSubscript"]->setToggleAction(true);
	(*scrActions)["typeEffectOutline"]->setToggleAction(true);
	(*scrActions)["typeEffectShadow"]->setToggleAction(true);
	connect( (*scrActions)["typeEffectNormal"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectUnderline"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectUnderlineWords"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectStrikeThrough"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSmallCaps"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectAllCaps"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSuperscript"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSubscript"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectOutline"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectShadow"], SIGNAL(triggeredData(int)), mainWindow, SLOT(setItemTypeStyle(int)));

	//Other Style menu items that get added in various places
	name="styleImageEffects";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="styleTabulators";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	connect( (*scrActions)["styleImageEffects"], SIGNAL(triggered()), mainWindow, SLOT(ImageEffects()));
	connect( (*scrActions)["styleTabulators"], SIGNAL(triggered()), mainWindow, SLOT(EditTabs()));

}

void ActionManager::initItemMenuActions()
{
	QString name;
	//Item Menu
	name="itemDuplicate";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemMulDuplicate";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemDelete";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemGroup";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemUngroup";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemLock";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemLockSize";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemPrintingEnabled";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemFlipH";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemFlipV";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)["itemLock"]->setToggleAction(true, true);
	(*scrActions)["itemLockSize"]->setToggleAction(true, true);
	(*scrActions)["itemPrintingEnabled"]->setToggleAction(true, true);
	(*scrActions)["itemFlipH"]->setToggleAction(true, true);
	(*scrActions)["itemFlipV"]->setToggleAction(true, true);
	name="itemLowerToBottom";
	scrActions->insert(name, new ScrAction(loadIcon("16/go-bottom.png"), loadIcon("22/go-bottom.png"), "", defKeys[name], mainWindow));
	name="itemRaiseToTop";
	scrActions->insert(name, new ScrAction(loadIcon("16/go-top.png"), loadIcon("22/go-top.png"), "", defKeys[name], mainWindow));
	name="itemLower";
	scrActions->insert(name, new ScrAction(loadIcon("16/go-down.png"), loadIcon("22/go-down.png"), "", defKeys[name], mainWindow));
	name="itemRaise";
	scrActions->insert(name, new ScrAction(loadIcon("16/go-up.png"), loadIcon("22/go-up.png"), "", defKeys[name], mainWindow));
	name="itemSendToScrapbook";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemSendToPattern";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemImageInfo";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemAttributes";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemImageIsVisible";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemUpdateImage";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemAdjustFrameToImage";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemExtendedImageProperties";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemPreviewLow";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 2));
	name="itemPreviewNormal";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 1));
	name="itemPreviewFull";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0));
	name="itemPDFIsBookmark";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)["itemPDFIsBookmark"]->setToggleAction(true);
	name="itemPDFIsAnnotation";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)["itemPDFIsAnnotation"]->setToggleAction(true);
	name="itemPDFAnnotationProps";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemPDFFieldProps";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));

	(*scrActions)["itemPDFIsBookmark"]->setEnabled(false);
	(*scrActions)["itemPDFIsAnnotation"]->setEnabled(false);
	(*scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
	(*scrActions)["itemPDFFieldProps"]->setEnabled(false);

	(*scrActions)["itemImageIsVisible"]->setToggleAction(true);
	(*scrActions)["itemPreviewLow"]->setToggleAction(true);
	(*scrActions)["itemPreviewNormal"]->setToggleAction(true);
	(*scrActions)["itemPreviewFull"]->setToggleAction(true);

	name="itemShapeEdit";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)["itemShapeEdit"]->setToggleAction(true);
	name="itemAttachTextToPath";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemDetachTextFromPath";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemCombinePolygons";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemSplitPolygons";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemConvertToBezierCurve";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemConvertToImageFrame";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemConvertToOutlines";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemConvertToPolygon";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="itemConvertToTextFrame";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));

	connect( (*scrActions)["itemDuplicate"], SIGNAL(triggered()), mainWindow, SLOT(duplicateItem()) );
	connect( (*scrActions)["itemMulDuplicate"], SIGNAL(triggered()), mainWindow, SLOT(duplicateItemMulti()) );
	connect( (*scrActions)["itemGroup"], SIGNAL(triggered()), mainWindow, SLOT(GroupObj()) );
	connect( (*scrActions)["itemUngroup"], SIGNAL(triggered()), mainWindow, SLOT(UnGroupObj()) );
	connect( (*scrActions)["itemPDFAnnotationProps"], SIGNAL(triggered()), mainWindow, SLOT(ModifyAnnot()) );
	connect( (*scrActions)["itemPDFFieldProps"], SIGNAL(triggered()), mainWindow, SLOT(ModifyAnnot()) );
	connect( (*scrActions)["itemSendToScrapbook"], SIGNAL(triggered()), mainWindow, SLOT(PutScrap()) );
	connect( (*scrActions)["itemSendToPattern"], SIGNAL(triggered()), mainWindow, SLOT(PutToPatterns()) );
	connect( (*scrActions)["itemAttributes"], SIGNAL(triggered()), mainWindow, SLOT(objectAttributes()) );
	connect( (*scrActions)["itemShapeEdit"], SIGNAL(triggered()), mainWindow, SLOT(toggleNodeEdit()) );
	connect( (*scrActions)["itemImageInfo"], SIGNAL(triggered()), mainWindow, SLOT(getImageInfo()) );
}

void ActionManager::initInsertMenuActions()
{
	QString name;
	//Insert Menu
	name="insertFrame";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="insertGlyph";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)["insertGlyph"]->setToggleAction(true);
	name="insertSampleText";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="stickyTools";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)["stickyTools"]->setToggleAction(true);

	connect( (*scrActions)["insertFrame"], SIGNAL(triggered()), mainWindow, SLOT(slotInsertFrame()) );
	connect( (*scrActions)["insertGlyph"], SIGNAL(triggered()), mainWindow, SLOT(slotCharSelect()) );
	connect( (*scrActions)["insertSampleText"], SIGNAL(triggered()), mainWindow, SLOT(insertSampleText()) );
	connect( (*scrActions)["stickyTools"], SIGNAL(triggered()), mainWindow, SLOT(ToggleStickyTools()) );
}

void ActionManager::initPageMenuActions()
{
	QString name;
	//Page menu
	name="pageInsert";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="pageImport";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="pageDelete";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="pageCopy";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="pageMove";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="pageApplyMasterPage";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="pageCopyToMasterPage";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="pageManageGuides";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)["pageManageGuides"]->setToggleAction(true);
	name="pageManageMargins";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));

	connect( (*scrActions)["pageInsert"], SIGNAL(triggered()), mainWindow, SLOT(slotNewPageM()) );
	connect( (*scrActions)["pageImport"], SIGNAL(triggered()), mainWindow, SLOT(slotPageImport()) );
	connect( (*scrActions)["pageDelete"], SIGNAL(triggered()), mainWindow, SLOT(DeletePage()) );
	connect( (*scrActions)["pageCopy"], SIGNAL(triggered()), mainWindow, SLOT(CopyPage()) );
	connect( (*scrActions)["pageMove"], SIGNAL(triggered()), mainWindow, SLOT(MovePage()) );
	connect( (*scrActions)["pageApplyMasterPage"], SIGNAL(triggered()), mainWindow, SLOT(ApplyMasterPage()) );
	connect( (*scrActions)["pageCopyToMasterPage"], SIGNAL(triggered()), mainWindow, SLOT(duplicateToMasterPage()) );
	connect( (*scrActions)["pageManageMargins"], SIGNAL(triggered()), mainWindow, SLOT(changePageMargins()) );
}

void ActionManager::initViewMenuActions()
{
	QString name;
	name="viewFitInWindow";
	scrActions->insert(name, new ScrAction(ScrAction::DataDouble, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, -100.0));
	name="viewFitWidth";
	scrActions->insert(name, new ScrAction(ScrAction::DataDouble, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, -200.0));
	name="viewFit50";
	scrActions->insert(name, new ScrAction(ScrAction::DataDouble, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 50.0));
	name="viewFit75";
	scrActions->insert(name, new ScrAction(ScrAction::DataDouble, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 75.0));
	name="viewFit100";
	scrActions->insert(name, new ScrAction(ScrAction::DataDouble, loadIcon("16/zoom-original.png"), loadIcon("22/zoom-original.png"), "", defKeys[name], mainWindow, 0, 100.0));
	name="viewFit200";
	scrActions->insert(name, new ScrAction(ScrAction::DataDouble, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 200.0));
	name="viewFit400";
	scrActions->insert(name, new ScrAction(ScrAction::DataDouble, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 400.0));
	name="viewFitPreview";
	scrActions->insert(name, new ScrAction(ScrAction::DataDouble, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 20.0));
	name="viewShowMargins";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowBleeds";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowFrames";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowLayerMarkers";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowImages";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowGrid";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowGuides";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowColumnBorders";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowBaseline";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowTextChain";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowTextControls";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowRulers";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewRulerMode";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewSnapToGrid";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewSnapToGuides";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="viewShowContextMenu";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
//	scrActions->insert("viewNewView", new ScrAction("", defKeys[name], mainWindow));

	(*scrActions)["viewFitPreview"]->setToggleAction(true);
	(*scrActions)["viewShowMargins"]->setToggleAction(true);
	(*scrActions)["viewShowBleeds"]->setToggleAction(true);
	(*scrActions)["viewShowFrames"]->setToggleAction(true);
	(*scrActions)["viewShowLayerMarkers"]->setToggleAction(true);
	(*scrActions)["viewShowImages"]->setToggleAction(true);
	(*scrActions)["viewShowGrid"]->setToggleAction(true);
	(*scrActions)["viewShowGuides"]->setToggleAction(true);
	(*scrActions)["viewShowColumnBorders"]->setToggleAction(true);
	(*scrActions)["viewShowBaseline"]->setToggleAction(true);
	(*scrActions)["viewShowTextChain"]->setToggleAction(true);
	(*scrActions)["viewShowTextControls"]->setToggleAction(true);
	(*scrActions)["viewShowRulers"]->setToggleAction(true);
	(*scrActions)["viewRulerMode"]->setToggleAction(true);
	(*scrActions)["viewSnapToGrid"]->setToggleAction(true);
	(*scrActions)["viewSnapToGuides"]->setToggleAction(true);

	(*scrActions)["viewFitPreview"]->setChecked(false);
	(*scrActions)["viewShowMargins"]->setChecked(true);
	(*scrActions)["viewShowBleeds"]->setChecked(true);
	(*scrActions)["viewShowFrames"]->setChecked(true);
	(*scrActions)["viewShowLayerMarkers"]->setChecked(false);
	(*scrActions)["viewShowImages"]->setChecked(true);
	(*scrActions)["viewShowGuides"]->setChecked(true);
	(*scrActions)["viewShowColumnBorders"]->setChecked(false);
	(*scrActions)["viewShowRulers"]->setChecked(true);
	(*scrActions)["viewRulerMode"]->setChecked(true);

	connect( (*scrActions)["viewFitInWindow"], SIGNAL(triggeredData(double)), mainWindow, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFitWidth"], SIGNAL(triggeredData(double)), mainWindow, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit50"], SIGNAL(triggeredData(double)), mainWindow, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit75"], SIGNAL(triggeredData(double)), mainWindow, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit100"], SIGNAL(triggeredData(double)), mainWindow, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit200"], SIGNAL(triggeredData(double)), mainWindow, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit400"], SIGNAL(triggeredData(double)), mainWindow, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewShowMargins"], SIGNAL(triggered()), mainWindow, SLOT(ToggleMarks()) );
	connect( (*scrActions)["viewShowBleeds"], SIGNAL(triggered()), mainWindow, SLOT(ToggleBleeds()) );
	connect( (*scrActions)["viewShowFrames"], SIGNAL(triggered()), mainWindow, SLOT(ToggleFrames()) );
	connect( (*scrActions)["viewShowLayerMarkers"], SIGNAL(triggered()), mainWindow, SLOT(ToggleLayerMarkers()) );
	connect( (*scrActions)["viewShowImages"], SIGNAL(triggered()), mainWindow, SLOT(TogglePics()) );
	connect( (*scrActions)["viewShowGrid"], SIGNAL(triggered()), mainWindow, SLOT(ToggleRaster()) );
	connect( (*scrActions)["viewShowGuides"], SIGNAL(triggered()), mainWindow, SLOT(ToggleGuides()) );
	connect( (*scrActions)["viewShowColumnBorders"], SIGNAL(triggered()), mainWindow, SLOT(ToggleColumnBorders()) );
	connect( (*scrActions)["viewShowBaseline"], SIGNAL(triggered()), mainWindow, SLOT(ToggleBase()) );
	connect( (*scrActions)["viewShowTextChain"], SIGNAL(triggered()), mainWindow, SLOT(ToggleTextLinks()) );
	connect( (*scrActions)["viewShowTextControls"], SIGNAL(triggered()), mainWindow, SLOT(ToggleTextControls()) );
	connect( (*scrActions)["viewShowRulers"], SIGNAL(triggered()), mainWindow, SLOT(ToggleRulers()) );
	connect( (*scrActions)["viewRulerMode"], SIGNAL(triggered()), mainWindow, SLOT(ToggleRulerMode()) );
	connect( (*scrActions)["viewSnapToGrid"], SIGNAL(triggered()), mainWindow, SLOT(ToggleURaster()) );
	connect( (*scrActions)["viewSnapToGuides"], SIGNAL(triggered()), mainWindow, SLOT(ToggleUGuides()) );
//	connect( (*scrActions)["viewNewView"], SIGNAL(triggered()), mainWindow, SLOT(newView()) );

}

void ActionManager::initToolsMenuActions()
{
	QString name;
	//Tool menu
	name="toolsProperties";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsOutline";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsScrapbook";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsLayers";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsPages";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsBookmarks";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsMeasurements";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/measure.png"), loadIcon("22/measure.png"), "", defKeys[name], mainWindow, modeMeasurementTool));
	name="toolsActionHistory";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsPreflightVerifier";
	scrActions->insert(name, new ScrAction(loadIcon("16/preflight-verifier.png"), loadIcon("22/preflight-verifier.png"),"", defKeys[name], mainWindow));
	name="toolsAlignDistribute";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsToolbarTools";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="toolsToolbarPDF";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));

	//toolbar only items
	name="toolsSelect";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/pointer.png"), loadIcon("22/pointer.png"), "", defKeys[name], mainWindow, modeNormal));
	name="toolsInsertTextFrame";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/insert-text-frame.png"), loadIcon("22/insert-text-frame.png"), "", defKeys[name], mainWindow, modeDrawText));
	name="toolsInsertImageFrame";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/insert-image.png"), loadIcon("22/insert-image.png"), "", defKeys[name], mainWindow, modeDrawPicture));
	name="toolsInsertLatexFrame";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/insert-latex.png"), loadIcon("22/insert-latex.png"), "", defKeys[name], mainWindow, modeDrawLatex));
	name="toolsInsertTableFrame";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/insert-table.png"), loadIcon("22/insert-table.png"), "", defKeys[name], mainWindow, modeDrawTable));
	name="toolsInsertShape";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, modeDrawShapes));
	name="toolsInsertPolygon";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/draw-polygon.png"), loadIcon("22/draw-polygon.png"), "", defKeys[name], mainWindow, modeDrawRegularPolygon));
	name="toolsInsertLine";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("Stift16.xpm"), loadIcon("Stift.xpm"), "", defKeys[name], mainWindow, modeDrawLine));
	name="toolsInsertBezier";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/draw-path.png"), loadIcon("22/draw-path.png"), "", defKeys[name], mainWindow, modeDrawBezierLine));
	name="toolsInsertFreehandLine";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/draw-freehand.png"), loadIcon("22/draw-freehand.png"), "", defKeys[name], mainWindow, modeDrawFreehandLine));
	name="toolsRotate";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/transform-rotate.png"), loadIcon("22/transform-rotate.png"), "", defKeys[name], mainWindow, modeRotation));
	name="toolsZoom";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/zoom.png"), loadIcon("22/zoom.png"), "", defKeys[name], mainWindow, modeMagnifier));
	name="toolsZoomIn";
	scrActions->insert(name, new ScrAction(loadIcon("16/zoom-in.png"), loadIcon("22/zoom-in.png"), "", defKeys[name], mainWindow));
	name="toolsZoomOut";
	scrActions->insert(name, new ScrAction(loadIcon("16/zoom-out.png"), loadIcon("22/zoom-out.png"), "", defKeys[name], mainWindow));
	name="toolsEditContents";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("Editm16.png"), loadIcon("Editm.xpm"), "", defKeys[name], mainWindow, modeEdit));
	name="toolsEditWithStoryEditor";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/story-editor.png"), loadIcon("22/story-editor.png"), "", defKeys[name], mainWindow, modeStoryEditor));
	name="toolsLinkTextFrame";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/text-frame-link.png"), loadIcon("22/text-frame-link.png"), "", defKeys[name], mainWindow, modeLinkFrames));
	name="toolsUnlinkTextFrame";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/text-frame-unlink.png"), loadIcon("22/text-frame-unlink.png"), "", defKeys[name], mainWindow, modeUnlinkFrames));
	name="toolsEyeDropper";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/color-picker.png"), loadIcon("22/color-picker.png"), "", defKeys[name], mainWindow, modeEyeDropper));
	name="toolsCopyProperties";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("wizard16.png"), loadIcon("wizard.png"), "", defKeys[name], mainWindow, modeCopyProperties));

	//PDF toolbar
	name="toolsPDFPushButton";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/insert-button.png"), loadIcon("22/insert-button.png"), "", defKeys[name], mainWindow, modeInsertPDFButton));
	name="toolsPDFTextField";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/text-field.png"), loadIcon("22/text-field.png"), "", defKeys[name], mainWindow, modeInsertPDFTextfield));
	name="toolsPDFCheckBox";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/checkbox.png"), loadIcon("22/checkbox.png"), "", defKeys[name], mainWindow, modeInsertPDFCheckbox));
	name="toolsPDFComboBox";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/combobox.png"), loadIcon("22/combobox.png"), "", defKeys[name], mainWindow, modeInsertPDFCombobox));
	name="toolsPDFListBox";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/list-box.png"), loadIcon("22/list-box.png"), "", defKeys[name], mainWindow, modeInsertPDFListbox));
	name="toolsPDFAnnotText";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("16/pdf-annotations.png"), loadIcon("22/pdf-annotations.png"), "", defKeys[name], mainWindow, modeInsertPDFTextAnnotation));
	name="toolsPDFAnnotLink";
	scrActions->insert(name, new ScrAction(ScrAction::DataInt, loadIcon("goto16.png"), loadIcon("goto.png"), "", defKeys[name], mainWindow, modeInsertPDFLinkAnnotation));

	//Set the applicaton wide palette shortcuts
	(*scrActions)["toolsProperties"]->setShortcutContext(Qt::ApplicationShortcut);
	(*scrActions)["toolsScrapbook"]->setShortcutContext(Qt::ApplicationShortcut);
	(*scrActions)["toolsLayers"]->setShortcutContext(Qt::ApplicationShortcut);
	(*scrActions)["toolsPages"]->setShortcutContext(Qt::ApplicationShortcut);
	(*scrActions)["toolsBookmarks"]->setShortcutContext(Qt::ApplicationShortcut);
	(*scrActions)["toolsActionHistory"]->setShortcutContext(Qt::ApplicationShortcut);
	(*scrActions)["toolsPreflightVerifier"]->setShortcutContext(Qt::ApplicationShortcut);
	(*scrActions)["toolsAlignDistribute"]->setShortcutContext(Qt::ApplicationShortcut);


	(*scrActions)["toolsProperties"]->setToggleAction(true);
	(*scrActions)["toolsOutline"]->setToggleAction(true);
	(*scrActions)["toolsScrapbook"]->setToggleAction(true);
	(*scrActions)["toolsLayers"]->setToggleAction(true);
	(*scrActions)["toolsPages"]->setToggleAction(true);
	(*scrActions)["toolsBookmarks"]->setToggleAction(true);
	(*scrActions)["toolsMeasurements"]->setToggleAction(true);
	(*scrActions)["toolsActionHistory"]->setToggleAction(true);
	(*scrActions)["toolsPreflightVerifier"]->setToggleAction(true);
	(*scrActions)["toolsAlignDistribute"]->setToggleAction(true);
	(*scrActions)["toolsToolbarTools"]->setToggleAction(true);
	(*scrActions)["toolsToolbarPDF"]->setToggleAction(true);

	(*scrActions)["toolsSelect"]->setToggleAction(true);
	(*scrActions)["toolsInsertTextFrame"]->setToggleAction(true);
	(*scrActions)["toolsInsertImageFrame"]->setToggleAction(true);
	(*scrActions)["toolsInsertLatexFrame"]->setToggleAction(true);
	(*scrActions)["toolsInsertTableFrame"]->setToggleAction(true);
	(*scrActions)["toolsInsertShape"]->setToggleAction(true);
	(*scrActions)["toolsInsertPolygon"]->setToggleAction(true);
	(*scrActions)["toolsInsertLine"]->setToggleAction(true);
	(*scrActions)["toolsInsertBezier"]->setToggleAction(true);
	(*scrActions)["toolsInsertFreehandLine"]->setToggleAction(true);
	(*scrActions)["toolsRotate"]->setToggleAction(true);
	(*scrActions)["toolsZoom"]->setToggleAction(true);
	(*scrActions)["toolsEditContents"]->setToggleAction(true);
	(*scrActions)["toolsEditWithStoryEditor"]->setToggleAction(true);
	(*scrActions)["toolsLinkTextFrame"]->setToggleAction(true);
	(*scrActions)["toolsUnlinkTextFrame"]->setToggleAction(true);
	(*scrActions)["toolsEyeDropper"]->setToggleAction(true);
	(*scrActions)["toolsCopyProperties"]->setToggleAction(true);

	(*scrActions)["toolsPDFPushButton"]->setToggleAction(true);
	(*scrActions)["toolsPDFTextField"]->setToggleAction(true);
	(*scrActions)["toolsPDFCheckBox"]->setToggleAction(true);
	(*scrActions)["toolsPDFComboBox"]->setToggleAction(true);
	(*scrActions)["toolsPDFListBox"]->setToggleAction(true);
	(*scrActions)["toolsPDFAnnotText"]->setToggleAction(true);
	(*scrActions)["toolsPDFAnnotLink"]->setToggleAction(true);

	*modeActionNames << "toolsSelect" << "toolsInsertTextFrame" << "toolsInsertImageFrame" << "toolsInsertTableFrame";
	*modeActionNames << "toolsInsertShape" << "toolsInsertPolygon" << "toolsInsertLine" << "toolsInsertBezier";
	*modeActionNames << "toolsInsertFreehandLine" << "toolsInsertLatexFrame" << "toolsRotate" << "toolsZoom" << "toolsEditContents";
	*modeActionNames << "toolsEditWithStoryEditor" << "toolsLinkTextFrame" << "toolsUnlinkTextFrame";
	*modeActionNames << "toolsEyeDropper" << "toolsCopyProperties";
	*modeActionNames << "toolsPDFPushButton" << "toolsPDFTextField" << "toolsPDFCheckBox" << "toolsPDFComboBox" << "toolsPDFListBox" << "toolsPDFAnnotText" << "toolsPDFAnnotLink";

 	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
 		(*scrActions)[*it]->setEnabled(false);


	*nonEditActionNames << "itemLowerToBottom" << "itemRaiseToTop" << "itemRaise" << "itemLower";

	connect( (*scrActions)["toolsActionHistory"], SIGNAL(toggled(bool)), mainWindow, SLOT(setUndoPalette(bool)) );

	connectModeActions();
}

void ActionManager::initExtrasMenuActions()
{
	QString name;
	name="extrasManagePictures";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="extrasHyphenateText";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="extrasDeHyphenateText";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="extrasGenerateTableOfContents";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));

	connect( (*scrActions)["extrasManagePictures"], SIGNAL(triggered()), mainWindow, SLOT(StatusPic()) );
	connect( (*scrActions)["extrasGenerateTableOfContents"], SIGNAL(triggered()), mainWindow, SLOT(generateTableOfContents()) );
}


void ActionManager::initWindowsMenuActions()
{
	QString name;
	name="windowsCascade";
	scrActions->insert(name, new ScrAction( "", defKeys[name], mainWindow));
	name="windowsTile";
	scrActions->insert(name, new ScrAction( "", defKeys[name], mainWindow));
}

void ActionManager::initScriptMenuActions()
{
}

void ActionManager::initHelpMenuActions()
{
	QString name;
	name="helpAboutScribus";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)[name]->setMenuRole(QAction::ApplicationSpecificRole);
	name="helpAboutPlugins";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)[name]->setMenuRole(QAction::ApplicationSpecificRole);
	name="helpAboutQt";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)[name]->setMenuRole(QAction::AboutQtRole);
	name="helpTooltips";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	name="helpManual";
	scrActions->insert(name, new ScrAction(loadIcon("16/help-browser.png"), QPixmap(), "", defKeys[name], mainWindow));
	name="helpOnlineWWW";
	scrActions->insert(name, new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 0.0, "http://www.scribus.net"));
	name="helpOnlineDocs";
	scrActions->insert(name, new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 0.0, "http://docs.scribus.net"));
	name="helpOnlineWiki";
	scrActions->insert(name, new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 0.0, "http://wiki.scribus.net"));
	name="helpOnlineTutorial1";
	scrActions->insert(name, new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0, 0.0, ""));
	name="helpCheckUpdates";
	scrActions->insert(name, new ScrAction("", defKeys[name], mainWindow));
	(*scrActions)[name]->setMenuRole(QAction::ApplicationSpecificRole);

	(*scrActions)["helpTooltips"]->setToggleAction(true);
	(*scrActions)["helpTooltips"]->setChecked(true);

	connect( (*scrActions)["helpAboutScribus"], SIGNAL(triggered()), mainWindow, SLOT(slotHelpAbout()) );
	connect( (*scrActions)["helpAboutPlugins"], SIGNAL(triggered()), mainWindow, SLOT(slotHelpAboutPlugins()) );
	connect( (*scrActions)["helpAboutQt"], SIGNAL(triggered()), mainWindow, SLOT(slotHelpAboutQt()) );
	connect( (*scrActions)["helpTooltips"], SIGNAL(triggered()), mainWindow, SLOT(ToggleTips()) );
	connect( (*scrActions)["helpManual"], SIGNAL(triggered()), mainWindow, SLOT(slotOnlineHelp()) );
	connect( (*scrActions)["helpCheckUpdates"], SIGNAL(triggered()), mainWindow, SLOT(slotHelpCheckUpdates()) );
	UrlLauncher* ul=UrlLauncher::instance();
	connect( (*scrActions)["helpOnlineWWW"], SIGNAL(triggeredData(QString)), ul, SLOT(launchUrlExt(const QString)) );
	connect( (*scrActions)["helpOnlineDocs"], SIGNAL(triggeredData(QString)), ul, SLOT(launchUrlExt(const QString)) );
	connect( (*scrActions)["helpOnlineWiki"], SIGNAL(triggeredData(QString)), ul, SLOT(launchUrlExt(const QString)) );
	connect( (*scrActions)["helpOnlineTutorial1"], SIGNAL(triggeredData(QString)), ul, SLOT(launchUrlExt(const QString)) );
}

void ActionManager::initUnicodeActions(QMap<QString, QPointer<ScrAction> > *actionMap, QWidget *actionParent, QStringList *actionNamesList)
{
	QString name;
	//typography
	name="unicodeSmartHyphen";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::SHYPHEN.unicode(), 0.0, name));
	name="unicodeNonBreakingHyphen";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::NBHYPHEN.unicode(), 0.0, name));
	name="unicodeNonBreakingSpace";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::NBSPACE.unicode(), 0.0, name));
	name="unicodePageNumber";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::PAGENUMBER.unicode(), 0.0, name));
	//Spaces
	name="unicodeSpaceEN";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2002, 0.0, name));
	name="unicodeSpaceEM";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2003, 0.0, name));
	name="unicodeSpaceThin";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2009, 0.0, name));
	name="unicodeSpaceThick";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2004, 0.0, name));
	name="unicodeSpaceMid";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2005, 0.0, name));
	name="unicodeSpaceHair";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x200A, 0.0, name));
	//Breaks
	name="unicodeNewLine";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::LINEBREAK.unicode(), 0.0, name));
	name="unicodeFrameBreak";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::FRAMEBREAK.unicode(), 0.0, name));
	name="unicodeColumnBreak";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::COLBREAK.unicode(), 0.0, name));
	name="unicodeZerowidthSpace";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::ZWSPACE.unicode(), 0.0, name));
	name="unicodeZerowidthNonBreakingSpace";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, SpecialChars::ZWNBSPACE.unicode(), 0.0, name));
	//Special
	name="unicodeCopyRight";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x0A9, 0.0, name));
	name="unicodeRegdTM";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x00AE, 0.0, name));
	name="unicodeTM";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2122, 0.0, name));
	name="unicodeBullet";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2022, 0.0, name));
	name="unicodeMidpoint";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x00B7, 0.0, name));
	name="unicodeSolidus";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2044, 0.0, name));
	//Dashes
	name="unicodeDashEm";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2014, 0.0, name));
	name="unicodeDashEn";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2013, 0.0, name));
	name="unicodeDashFigure";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2012, 0.0, name));
	name="unicodeDashQuotation";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2015, 0.0, name));
	//Quotes
	name="unicodeQuoteApostrophe";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x0027, 0.0, name));
	name="unicodeQuoteStraight";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x0022, 0.0, name));
	name="unicodeQuoteSingleLeft";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2018, 0.0, name));
	name="unicodeQuoteSingleRight";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2019, 0.0, name));
	name="unicodeQuoteDoubleLeft";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x201C, 0.0, name));
	name="unicodeQuoteDoubleRight";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x201D, 0.0, name));
	name="unicodeQuoteSingleReversed";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x201B, 0.0, name));
	name="unicodeQuoteDoubleReversed";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x201F, 0.0, name));
	name="unicodeQuoteSingleLeftGuillemet";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x2039, 0.0, name));
	name="unicodeQuoteSingleRightGuillemet";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x203A, 0.0, name));
	name="unicodeQuoteDoubleLeftGuillemet";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x00AB, 0.0, name));
	name="unicodeQuoteDoubleRightGuillemet";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x00BB, 0.0, name));
	name="unicodeQuoteLowSingleComma";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x201A, 0.0, name));
	name="unicodeQuoteLowDoubleComma";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x201E, 0.0, name));
	name="unicodeQuoteCJKSingleLeft";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x300C, 0.0, name));
	name="unicodeQuoteCJKSingleRight";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x300D, 0.0, name));
	name="unicodeQuoteCJKDoubleLeft";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x300E, 0.0, name));
	name="unicodeQuoteCJKDoubleRight";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0x300F, 0.0, name));
	//Ligatures
	name="unicodeLigature_ff";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0xFB00, 0.0, name));
	name="unicodeLigature_fi";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0xFB01, 0.0, name));
	name="unicodeLigature_fl";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0xFB02, 0.0, name));
	name="unicodeLigature_ffi";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0xFB03, 0.0, name));
	name="unicodeLigature_ffl";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0xFB04, 0.0, name));
	name="unicodeLigature_ft";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0xFB05, 0.0, name));
	name="unicodeLigature_st";
	actionMap->insert(name, new ScrAction(ScrAction::UnicodeChar, QPixmap(), QPixmap(), "", defKeys[name], actionParent, 0xFB06, 0.0, name));

	//Spaces and special characters

	*actionNamesList << "unicodeSmartHyphen" << "unicodeNonBreakingHyphen" << "unicodeNonBreakingSpace" << "unicodePageNumber";
	*actionNamesList << "unicodeSpaceEN" << "unicodeSpaceEM" << "unicodeSpaceThin" << "unicodeSpaceThick" << "unicodeSpaceMid" << "unicodeSpaceHair";
	//Breaks
	*actionNamesList << "unicodeNewLine" << "unicodeFrameBreak" << "unicodeColumnBreak" << "unicodeZerowidthSpace";
	//Copyrights and TMs
	*actionNamesList << "unicodeCopyRight" << "unicodeRegdTM" << "unicodeTM";
	//Slashes
	*actionNamesList << "unicodeSolidus";
	//Bullets
	*actionNamesList << "unicodeBullet" << "unicodeMidpoint";
	//Dashes
	*actionNamesList << "unicodeDashEm" << "unicodeDashEn" << "unicodeDashFigure" << "unicodeDashQuotation";
	//Straight quotes
	*actionNamesList << "unicodeQuoteApostrophe" << "unicodeQuoteStraight";
	//Double quotes
	*actionNamesList << "unicodeQuoteDoubleLeft" << "unicodeQuoteDoubleRight" << "unicodeQuoteSingleLeft" << "unicodeQuoteSingleRight";
	//Alternative single quotes
	*actionNamesList << "unicodeQuoteSingleReversed" << "unicodeQuoteDoubleReversed";
	//French quotes
	*actionNamesList << "unicodeQuoteSingleLeftGuillemet" << "unicodeQuoteSingleRightGuillemet" << "unicodeQuoteDoubleLeftGuillemet" << "unicodeQuoteDoubleRightGuillemet";
	//German quotes
	*actionNamesList << "unicodeQuoteLowSingleComma" << "unicodeQuoteLowDoubleComma";
	//CJK Quotes
	*actionNamesList << "unicodeQuoteCJKSingleLeft" << "unicodeQuoteCJKSingleRight" << "unicodeQuoteCJKDoubleLeft" << "unicodeQuoteCJKDoubleRight";
	//Ligatures
	*actionNamesList << "unicodeLigature_ff" << "unicodeLigature_fi" << "unicodeLigature_fl" << "unicodeLigature_ffi" << "unicodeLigature_ffl" << "unicodeLigature_ft" << "unicodeLigature_st";
	for ( QStringList::Iterator it = actionNamesList->begin(); it != actionNamesList->end(); ++it )
		connect( (*actionMap)[*it], SIGNAL(triggeredUnicodeShortcut(const QString&, int)), actionParent, SLOT(specialActionKeyEvent(const QString&, int)) );
}

void ActionManager::initSpecialActions()
{
	QString name;
	//GUI
	name="specialToggleAllPalettes";
	scrActions->insert(name, new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0,0.0,name));
	name="specialToggleAllGuides";
	scrActions->insert(name, new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "", defKeys[name], mainWindow, 0,0.0,name));

	connect( (*scrActions)["specialToggleAllPalettes"], SIGNAL(triggered()), mainWindow, SLOT(ToggleAllPalettes()) );
	connect( (*scrActions)["specialToggleAllGuides"], SIGNAL(triggered()), mainWindow, SLOT(ToggleAllGuides()) );
}

void ActionManager::disconnectModeActions()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
		disconnect( (*scrActions)[*it], SIGNAL(toggledData(bool, int)) , mainWindow, SLOT(setAppModeByToggle(bool, int)) );
}

void ActionManager::connectModeActions()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
		connect( (*scrActions)[*it], SIGNAL(toggledData(bool, int)) , mainWindow, SLOT(setAppModeByToggle(bool, int)) );
}

void ActionManager::disconnectNewDocActions()
{
	disconnect( (*scrActions)["itemAdjustFrameToImage"], 0, 0, 0 );
	disconnect( (*scrActions)["itemLock"], 0, 0, 0);
	disconnect( (*scrActions)["itemLockSize"], 0, 0, 0);
	disconnect( (*scrActions)["itemPrintingEnabled"], 0, 0, 0);
	disconnect( (*scrActions)["itemFlipH"], 0, 0, 0);
	disconnect( (*scrActions)["itemFlipV"], 0, 0, 0);
	disconnect( (*scrActions)["itemCombinePolygons"], 0, 0, 0);
	disconnect( (*scrActions)["itemSplitPolygons"], 0, 0, 0);
	disconnect( (*scrActions)["itemUpdateImage"], 0, 0, 0 );
	disconnect( (*scrActions)["itemDelete"], 0, 0, 0);
	disconnect( (*scrActions)["extrasHyphenateText"], 0, 0, 0 );
	disconnect( (*scrActions)["extrasDeHyphenateText"], 0, 0, 0 );

}

void ActionManager::connectNewDocActions(ScribusDoc *currDoc)
{
	if (currDoc==NULL)
		return;
	connect( (*scrActions)["itemLock"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_ToggleLock()) );
	connect( (*scrActions)["itemLockSize"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_ToggleSizeLock()));
	connect( (*scrActions)["itemPrintingEnabled"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_TogglePrintEnabled()));
	connect( (*scrActions)["itemFlipH"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_FlipH()));
	connect( (*scrActions)["itemFlipV"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_FlipV()));
	connect( (*scrActions)["itemCombinePolygons"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_UniteItems()) );
	connect( (*scrActions)["itemSplitPolygons"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_SplitItems()) );
	connect( (*scrActions)["itemUpdateImage"], SIGNAL(triggered()), currDoc, SLOT(updatePic()) );
	connect( (*scrActions)["extrasHyphenateText"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_DoHyphenate()) );
	connect( (*scrActions)["extrasDeHyphenateText"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_DoDeHyphenate()) );
	connect( (*scrActions)["itemDelete"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_DeleteItem()) );
	connect( (*scrActions)["itemAdjustFrameToImage"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_adjustFrametoImageSize()) );
}

void ActionManager::disconnectNewViewActions()
{
	disconnect( (*scrActions)["viewFitPreview"], 0, 0, 0);
	disconnect( (*scrActions)["toolsZoomIn"], 0, 0, 0);
	disconnect( (*scrActions)["toolsZoomOut"], 0, 0, 0);
	disconnect( (*scrActions)["itemLowerToBottom"], 0, 0, 0);
	disconnect( (*scrActions)["itemImageIsVisible"], 0, 0, 0);
	disconnect( (*scrActions)["itemPreviewLow"], SIGNAL(triggeredData(int)), 0, 0 );
	disconnect( (*scrActions)["itemPreviewNormal"], SIGNAL(triggeredData(int)), 0,0 );
	disconnect( (*scrActions)["itemPreviewFull"], SIGNAL(triggeredData(int)), 0, 0 );
	disconnect( (*scrActions)["itemRaise"], 0, 0, 0);
	disconnect( (*scrActions)["itemLower"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToBezierCurve"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToImageFrame"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToOutlines"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToPolygon"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToTextFrame"], 0, 0, 0);
	disconnect( (*scrActions)["itemAttachTextToPath"], 0, 0, 0);
	disconnect( (*scrActions)["itemDetachTextFromPath"], 0, 0, 0);
	disconnect( (*scrActions)["itemExtendedImageProperties"], 0, 0, 0 );
}

void ActionManager::connectNewViewActions(ScribusView *currView)
{
	if (currView==NULL)
		return;
	connect( (*scrActions)["viewFitPreview"], SIGNAL(triggered()), currView, SLOT(togglePreview()) );
	connect( (*scrActions)["toolsZoomIn"], SIGNAL(triggered()) , currView, SLOT(slotZoomIn()) );
	connect( (*scrActions)["toolsZoomOut"], SIGNAL(triggered()) , currView, SLOT(slotZoomOut()) );
	connect( (*scrActions)["itemLowerToBottom"], SIGNAL(triggered()), currView, SLOT(ToBack()) );
	connect( (*scrActions)["itemRaiseToTop"], SIGNAL(triggered()), currView, SLOT(ToFront()) );
	connect( (*scrActions)["itemRaise"], SIGNAL(triggered()), currView, SLOT(RaiseItem()) );
	connect( (*scrActions)["itemLower"], SIGNAL(triggered()), currView, SLOT(LowerItem()) );
	connect( (*scrActions)["itemConvertToBezierCurve"], SIGNAL(triggered()), currView, SLOT(ToBezierFrame()) );
	connect( (*scrActions)["itemConvertToImageFrame"], SIGNAL(triggered()), currView, SLOT(ToPicFrame()) );
	connect( (*scrActions)["itemConvertToOutlines"], SIGNAL(triggered()), currView, SLOT(TextToPath()) );
	connect( (*scrActions)["itemConvertToPolygon"], SIGNAL(triggered()), currView, SLOT(ToPolyFrame()) );
	connect( (*scrActions)["itemConvertToTextFrame"], SIGNAL(triggered()), currView, SLOT(ToTextFrame()) );
	connect( (*scrActions)["itemAttachTextToPath"], SIGNAL(triggered()), currView, SLOT(ToPathText()) );
	connect( (*scrActions)["itemDetachTextFromPath"], SIGNAL(triggered()), currView, SLOT(FromPathText()) );
	connect( (*scrActions)["itemExtendedImageProperties"], SIGNAL(triggered()), currView, SLOT(editExtendedImageProperties()) );
}

void ActionManager::disconnectNewSelectionActions()
{
	disconnect( (*scrActions)["itemImageIsVisible"], 0, 0, 0);
	//Only disconnect triggeredData for data based actions or you will disconnect the internal signal
	disconnect( (*scrActions)["itemPreviewLow"], SIGNAL(triggeredData(int)) , 0, 0);
	disconnect( (*scrActions)["itemPreviewNormal"], SIGNAL(triggeredData(int)) , 0, 0);
	disconnect( (*scrActions)["itemPreviewFull"], SIGNAL(triggeredData(int)) , 0, 0);
	disconnect( (*scrActions)["editClearContents"], 0, 0, 0);
}

void ActionManager::connectNewSelectionActions(ScribusView* /*currView*/, ScribusDoc* currDoc)
{
	connect( (*scrActions)["itemImageIsVisible"], SIGNAL(toggled(bool)), currDoc, SLOT(itemSelection_ToggleImageShown()) );
	connect( (*scrActions)["itemPreviewLow"], SIGNAL(triggeredData(int)), currDoc, SLOT(itemSelection_ChangePreviewResolution(int)) );
	connect( (*scrActions)["itemPreviewNormal"], SIGNAL(triggeredData(int)), currDoc, SLOT(itemSelection_ChangePreviewResolution(int)) );
	connect( (*scrActions)["itemPreviewFull"], SIGNAL(triggeredData(int)), currDoc, SLOT(itemSelection_ChangePreviewResolution(int)) );
	connect( (*scrActions)["editClearContents"], SIGNAL(triggered()), currDoc, SLOT(itemSelection_ClearItem()) );
}

void ActionManager::saveActionShortcutsPreEditMode()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
	{
		(*scrActions)[*it]->setShortcutContext(Qt::WidgetShortcut);  // in theory, this should be enough, but...
		(*scrActions)[*it]->saveShortcut();
#ifdef Q_WS_MAC
		if ((*scrActions)[*it]->menu() != NULL)
			(*scrActions)[*it]->setEnabled(false);
#endif		
	}
	for ( QStringList::Iterator it = nonEditActionNames->begin(); it != nonEditActionNames->end(); ++it )
	{
		(*scrActions)[*it]->setShortcutContext(Qt::WidgetShortcut);  // in theory, this should be enough, but...
		(*scrActions)[*it]->saveShortcut();
	}
}

void ActionManager::restoreActionShortcutsPostEditMode()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
	{
		(*scrActions)[*it]->setShortcutContext(Qt::WindowShortcut);  // see above
		(*scrActions)[*it]->restoreShortcut();
#ifdef Q_WS_MAC
		(*scrActions)[*it]->setEnabled(true);
#endif		
	}
	for ( QStringList::Iterator it = nonEditActionNames->begin(); it != nonEditActionNames->end(); ++it )
	{
		(*scrActions)[*it]->setShortcutContext(Qt::WindowShortcut);  // see above
		(*scrActions)[*it]->restoreShortcut();
	}
}

void ActionManager::enableActionStringList(QMap<QString, QPointer<ScrAction> > *actionMap, QStringList *list, bool enabled, bool checkingUnicode, const QString& fontName)
{
	for ( QStringList::Iterator it = list->begin(); it != list->end(); ++it )
	{
		if(!checkingUnicode)
			(*actionMap)[*it]->setEnabled(enabled);
		else
		{
			//For UnicodeChar actions, only enable when the current font has that character.
			if (mainWindow->HaveDoc && (*actionMap)[*it]->actionType()==ScrAction::UnicodeChar)
			{
				int charCode=(*actionMap)[*it]->actionInt();
				if(charCode==-1 ||
					charCode==24 ||
					charCode==26 ||
					charCode==27 ||
					charCode==28 ||
					charCode==29 ||
					charCode==30 ||
					((*mainWindow->doc->AllFonts)[fontName].usable() &&
					(*mainWindow->doc->AllFonts)[fontName].canRender(charCode)) )
						(*actionMap)[*it]->setEnabled(true);
				else
					(*actionMap)[*it]->setEnabled(false);
			}
		}
	}
}

void ActionManager::enableUnicodeActions(QMap<QString, QPointer<ScrAction> > *actionMap, bool enabled, const QString& fontName)
{
	enableActionStringList(actionMap, unicodeCharActionNames, enabled, enabled, fontName);
	(*actionMap)["insertGlyph"]->setEnabled(enabled);
}

void ActionManager::setPDFActions(ScribusView *currView)
{
	if (currView==NULL)
		return;
	PageItem* currItem = mainWindow->doc->m_Selection->itemAt(0);
	if (currItem==NULL)
		return;

	disconnect( (*scrActions)["itemPDFIsBookmark"], 0, 0, 0);
	disconnect( (*scrActions)["itemPDFIsAnnotation"], 0, 0, 0);

	if (!currItem->asTextFrame())
	{
		(*scrActions)["itemPDFIsAnnotation"]->setEnabled(false);
		(*scrActions)["itemPDFIsBookmark"]->setEnabled(false);
		(*scrActions)["itemPDFIsAnnotation"]->setChecked(false);
		(*scrActions)["itemPDFIsBookmark"]->setChecked(false);
		(*scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
		(*scrActions)["itemPDFFieldProps"]->setEnabled(false);
		return;
	}

	(*scrActions)["itemPDFIsAnnotation"]->setEnabled(true);
	(*scrActions)["itemPDFIsBookmark"]->setEnabled(true);
	(*scrActions)["itemPDFIsAnnotation"]->setChecked(currItem->isAnnotation());
	(*scrActions)["itemPDFIsBookmark"]->setChecked(currItem->isBookmark);
	if (currItem->isAnnotation())
	{
		int aType=currItem->annotation().Type();
		bool setter=((aType == 0) || (aType == 1) || (aType > 9));
		(*scrActions)["itemPDFAnnotationProps"]->setEnabled(setter);
		(*scrActions)["itemPDFFieldProps"]->setEnabled(!setter);
	}
	else
	{
		(*scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
		(*scrActions)["itemPDFFieldProps"]->setEnabled(false);
	}
	connect( (*scrActions)["itemPDFIsAnnotation"], SIGNAL(triggered()), currView, SLOT(ToggleAnnotation()) );
	connect( (*scrActions)["itemPDFIsBookmark"], SIGNAL(triggered()), currView, SLOT(ToggleBookmark()) );
}

void ActionManager::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
}

void ActionManager::languageChange()
{
	//File Menu
	(*scrActions)["fileNew"]->setTexts( tr("&New"));
	(*scrActions)["fileOpen"]->setTexts( tr("&Open..."));
	(*scrActions)["fileClose"]->setTexts( tr("&Close"));
	(*scrActions)["fileSave"]->setTexts( tr("&Save"));
	(*scrActions)["fileSaveAs"]->setTexts( tr("Save &As..."));
	(*scrActions)["fileRevert"]->setTexts( tr("Re&vert to Saved"));
	(*scrActions)["fileCollect"]->setTexts( tr("Collect for O&utput..."));
	(*scrActions)["fileImportText"]->setTexts( tr("Get Text..."));
	(*scrActions)["fileImportAppendText"]->setTexts( tr("Append &Text..."));
	(*scrActions)["fileImportImage"]->setTexts( tr("Get Image..."));
	(*scrActions)["fileExportText"]->setTexts( tr("Save &Text..."));
	(*scrActions)["fileExportAsEPS"]->setTexts( tr("Save as &EPS..."));
	(*scrActions)["fileExportAsPDF"]->setTexts( tr("Save as P&DF..."));
	(*scrActions)["fileDocSetup"]->setTexts( tr("Document &Setup..."));
	(*scrActions)["filePreferences"]->setTexts( tr("P&references..."));
	(*scrActions)["filePrint"]->setTexts( tr("&Print..."));
	(*scrActions)["PrintPreview"]->setTexts( tr("Print Previe&w"));
	(*scrActions)["fileQuit"]->setTexts( tr("&Quit"));
	//Edit Menu
	(*scrActions)["editUndoAction"]->setTexts( tr("&Undo"));
	(*scrActions)["editRedoAction"]->setTexts( tr("&Redo"));
	(*scrActions)["editActionMode"]->setTexts( tr("&Item Action Mode"));
	(*scrActions)["editCut"]->setTexts( tr("Cu&t"));
	(*scrActions)["editCopy"]->setTexts( tr("&Copy"));
	(*scrActions)["editPaste"]->setTexts( tr("&Paste"));
	(*scrActions)["editCopyContents"]->setTexts( tr("&Copy"));
	(*scrActions)["editPasteContents"]->setTexts( tr("&Paste"));
	(*scrActions)["editPasteContentsAbs"]->setTexts( tr("Paste (&Absolute)"));
	(*scrActions)["editClearContents"]->setTexts( tr("C&lear"));
	(*scrActions)["editSelectAll"]->setTexts( tr("Select &All"));
	(*scrActions)["editDeselectAll"]->setTexts( tr("&Deselect All"));
	(*scrActions)["editSearchReplace"]->setTexts( tr("&Search/Replace..."));
	(*scrActions)["editEditWithImageEditor"]->setTexts( tr("Edit Image..."));
	(*scrActions)["editEditWithLatexEditor"]->setTexts( tr("Edit Latex Source..."));
	(*scrActions)["editColors"]->setTexts( tr("C&olors..."));
	(*scrActions)["editPatterns"]->setTexts( tr("Patterns..."));
	(*scrActions)["editStyles"]->setTexts( tr("S&tyles..."));
	(*scrActions)["editMasterPages"]->setTexts( tr("&Master Pages..."));
	(*scrActions)["editJavascripts"]->setTexts( tr("&JavaScripts..."));

	int font_sizes[] = {7, 9, 10, 11, 12, 14, 18, 24, 36, 48, 60, 72};
	size_t f_size = sizeof(font_sizes) / sizeof(*font_sizes);
	for (uint s = 0; s < f_size; ++s)
	{
		QString fontSizeName=QString("fontSize%1").arg(font_sizes[s]);
		(*scrActions)[fontSizeName]->setTexts( tr("%1 pt").arg(font_sizes[s]));
	}
	(*scrActions)["fontSizeOther"]->setTexts( tr("&Other..."));
	(*scrActions)["alignLeft"]->setTexts( tr("&Left"));
	(*scrActions)["alignCenter"]->setTexts( tr("&Center"));
	(*scrActions)["alignRight"]->setTexts( tr("&Right"));
	(*scrActions)["alignBlock"]->setTexts( tr("&Block"));
	(*scrActions)["alignForced"]->setTexts( tr("&Forced"));

	for (uint i=0; i<=100 ; i+=10)
	{
		QString shadeName=QString("shade%1").arg(i);
		(*scrActions)[shadeName]->setTexts( tr("&%1 %").arg(i));
	}

	(*scrActions)["shadeOther"]->setTexts( tr("&Other..."));
	(*scrActions)["typeEffectNormal"]->setTexts( tr("&Normal"));
	(*scrActions)["typeEffectUnderline"]->setTexts( tr("&Underline"));
	(*scrActions)["typeEffectUnderlineWords"]->setTexts( tr("Underline &Words"));
	(*scrActions)["typeEffectStrikeThrough"]->setTexts( tr("&Strike Through"));
	(*scrActions)["typeEffectAllCaps"]->setTexts( tr("&All Caps"));
	(*scrActions)["typeEffectSmallCaps"]->setTexts( tr("Small &Caps"));
	(*scrActions)["typeEffectSuperscript"]->setTexts( tr("Su&perscript"));
	(*scrActions)["typeEffectSubscript"]->setTexts( tr("Su&bscript"));
	(*scrActions)["typeEffectOutline"]->setTexts( tr("&Outline", "type effect"));
	(*scrActions)["typeEffectShadow"]->setTexts( tr("S&hadow"));

	(*scrActions)["styleImageEffects"]->setTexts( tr("&Image Effects"));
	(*scrActions)["styleTabulators"]->setTexts( tr("&Tabulators..."));

	//Item Menu
	(*scrActions)["itemDuplicate"]->setTexts( tr("D&uplicate"));
	(*scrActions)["itemMulDuplicate"]->setTexts( tr("&Multiple Duplicate"));
	(*scrActions)["itemDelete"]->setTexts( tr("&Delete"));
	(*scrActions)["itemGroup"]->setTexts( tr("&Group"));
	(*scrActions)["itemUngroup"]->setTexts( tr("&Ungroup"));
	(*scrActions)["itemLock"]->setTexts( tr("Is &Locked"));
	(*scrActions)["itemLockSize"]->setTexts( tr("Si&ze is Locked"));
	(*scrActions)["itemPrintingEnabled"]->setTexts( tr("&Printing Enabled"));
	(*scrActions)["itemFlipH"]->setTexts( tr("&Flip Horizontally"));
	(*scrActions)["itemFlipV"]->setTexts( tr("&Flip Vertically"));
	(*scrActions)["itemLowerToBottom"]->setTexts( tr("Lower to &Bottom"));
	(*scrActions)["itemRaiseToTop"]->setTexts( tr("Raise to &Top"));
	(*scrActions)["itemLower"]->setTexts( tr("&Lower"));
	(*scrActions)["itemRaise"]->setTexts( tr("&Raise"));
	(*scrActions)["itemSendToScrapbook"]->setTexts( tr("Send to S&crapbook"));
	(*scrActions)["itemSendToPattern"]->setTexts( tr("Send to Patterns"));
	(*scrActions)["itemAttributes"]->setTexts( tr("&Attributes..."));
	(*scrActions)["itemImageInfo"]->setTexts( tr("More Info..."));
	(*scrActions)["itemImageIsVisible"]->setTexts( tr("I&mage Visible"));
	(*scrActions)["itemUpdateImage"]->setTexts( tr("&Update Image"));
	(*scrActions)["itemAdjustFrameToImage"]->setTexts( tr("Adjust Frame to Image"));
	(*scrActions)["itemExtendedImageProperties"]->setTexts( tr("Extended Image Properties"));
	(*scrActions)["itemPreviewLow"]->setTexts( tr("&Low Resolution"));
	(*scrActions)["itemPreviewNormal"]->setTexts( tr("&Normal Resolution"));
	(*scrActions)["itemPreviewFull"]->setTexts( tr("&Full Resolution"));
	(*scrActions)["itemPDFIsBookmark"]->setTexts( tr("Is PDF &Bookmark"));
	(*scrActions)["itemPDFIsAnnotation"]->setTexts( tr("Is PDF A&nnotation"));
	(*scrActions)["itemPDFAnnotationProps"]->setTexts( tr("Annotation P&roperties"));
	(*scrActions)["itemPDFFieldProps"]->setTexts( tr("Field P&roperties"));
	(*scrActions)["itemShapeEdit"]->setTexts( tr("&Edit Shape..."));
	(*scrActions)["itemAttachTextToPath"]->setTexts( tr("&Attach Text to Path"));
	(*scrActions)["itemDetachTextFromPath"]->setTexts( tr("&Detach Text from Path"));
	(*scrActions)["itemCombinePolygons"]->setTexts( tr("&Combine Polygons"));
	(*scrActions)["itemSplitPolygons"]->setTexts( tr("Split &Polygons"));
	(*scrActions)["itemConvertToBezierCurve"]->setTexts( tr("&Bezier Curve"));
	(*scrActions)["itemConvertToImageFrame"]->setTexts( tr("&Image Frame"));
	(*scrActions)["itemConvertToOutlines"]->setTexts( tr("&Outlines", "Convert to oulines"));
	(*scrActions)["itemConvertToPolygon"]->setTexts( tr("&Polygon"));
	(*scrActions)["itemConvertToTextFrame"]->setTexts( tr("&Text Frame"));

	//Insert Menu
	(*scrActions)["insertFrame"]->setTexts( tr("&Frame..."));
	(*scrActions)["insertGlyph"]->setTexts( tr("&Glyph..."));
	(*scrActions)["insertSampleText"]->setTexts( tr("Sample Text"));
	(*scrActions)["stickyTools"]->setTexts( tr("Sticky Tools"));

	//Page menu
	(*scrActions)["pageInsert"]->setTexts( tr("&Insert..."));
	(*scrActions)["pageImport"]->setTexts( tr("Im&port..."));
	(*scrActions)["pageDelete"]->setTexts( tr("&Delete..."));
	(*scrActions)["pageCopy"]->setTexts( tr("&Copy..."));
	(*scrActions)["pageMove"]->setTexts( tr("&Move..."));
	(*scrActions)["pageApplyMasterPage"]->setTexts( tr("&Apply Master Page..."));
	(*scrActions)["pageCopyToMasterPage"]->setTexts( tr("Convert to Master Page..."));
	(*scrActions)["pageManageGuides"]->setTexts( tr("Manage &Guides..."));
	(*scrActions)["pageManageMargins"]->setTexts( tr("Manage Page Properties..."));

	//View Menu
	(*scrActions)["viewFitInWindow"]->setTexts( tr("&Fit to Height"));
	(*scrActions)["viewFitWidth"]->setTexts( tr("Fit to Width"));
	(*scrActions)["viewFit50"]->setTexts( tr("&50%"));
	(*scrActions)["viewFit75"]->setTexts( tr("&75%"));
	(*scrActions)["viewFit100"]->setTexts( tr("&100%"));
	(*scrActions)["viewFit200"]->setTexts( tr("&200%"));
	(*scrActions)["viewFit400"]->setTexts( tr("&400%"));
	(*scrActions)["viewFitPreview"]->setTexts( tr("Preview Mode"));
	(*scrActions)["viewShowMargins"]->setTexts( tr("Show &Margins"));
	(*scrActions)["viewShowBleeds"]->setTexts( tr("Show Bleeds"));
	(*scrActions)["viewShowFrames"]->setTexts( tr("Show &Frames"));
	(*scrActions)["viewShowLayerMarkers"]->setTexts( tr("Show Layer Indicators"));
	(*scrActions)["viewShowImages"]->setTexts( tr("Show &Images"));
	(*scrActions)["viewShowGrid"]->setTexts( tr("Show &Grid"));
	(*scrActions)["viewShowGuides"]->setTexts( tr("Show G&uides"));
	(*scrActions)["viewShowColumnBorders"]->setTexts( tr("Show Text Frame Columns"));
	(*scrActions)["viewShowBaseline"]->setTexts( tr("Show &Baseline Grid"));
	(*scrActions)["viewShowTextChain"]->setTexts( tr("Show &Text Chain"));
	(*scrActions)["viewShowTextControls"]->setTexts( tr("Show Control Characters"));
	(*scrActions)["viewShowRulers"]->setTexts( tr("Show Rulers"));
	(*scrActions)["viewRulerMode"]->setTexts( tr("Rulers relative to Page"));
	(*scrActions)["viewSnapToGrid"]->setTexts( tr("Sn&ap to Grid"));
	(*scrActions)["viewSnapToGuides"]->setTexts( tr("Sna&p to Guides"));
	(*scrActions)["viewShowContextMenu"]->setTexts( tr("Show Context Menu"));
//	(*scrActions)["viewNewView"]->setTexts( tr("New View"));

	//Tool menu
	(*scrActions)["toolsProperties"]->setTexts( tr("&Properties"));
	(*scrActions)["toolsOutline"]->setTexts( tr("&Outline", "Document Outline Palette"));
	(*scrActions)["toolsScrapbook"]->setTexts( tr("&Scrapbook"));
	(*scrActions)["toolsLayers"]->setTexts( tr("&Layers"));
	(*scrActions)["toolsPages"]->setTexts( tr("&Arrange Pages"));
	(*scrActions)["toolsBookmarks"]->setTexts( tr("&Bookmarks"));
	(*scrActions)["toolsMeasurements"]->setTexts( tr("&Measurements"));
	(*scrActions)["toolsActionHistory"]->setTexts( tr("Action &History"));
	(*scrActions)["toolsPreflightVerifier"]->setTexts( tr("Preflight &Verifier"));
	(*scrActions)["toolsAlignDistribute"]->setTexts( tr("&Align and Distribute"));
	(*scrActions)["toolsToolbarTools"]->setTexts( tr("&Tools"));
	(*scrActions)["toolsToolbarPDF"]->setTexts( tr("P&DF Tools"));

	//toolbar only items
	(*scrActions)["toolsSelect"]->setTexts( tr("Select Item"));
	(*scrActions)["toolsRotate"]->setTexts( tr("Rotate Item"));
	(*scrActions)["toolsZoom"]->setTexts( tr("Zoom in or out"));
	(*scrActions)["toolsZoomIn"]->setTexts( tr("Zoom in"));
	(*scrActions)["toolsZoomOut"]->setTexts( tr("Zoom out"));
	(*scrActions)["toolsEditContents"]->setTexts( tr("Edit Contents of Frame"));
	(*scrActions)["toolsEditWithStoryEditor"]->setText( tr("Edit Text..."));
	(*scrActions)["toolsLinkTextFrame"]->setTexts( tr("Link Text Frames"));
	(*scrActions)["toolsUnlinkTextFrame"]->setTexts( tr("Unlink Text Frames"));
	(*scrActions)["toolsEyeDropper"]->setTexts( tr("&Eye Dropper"));
	(*scrActions)["toolsCopyProperties"]->setTexts( tr("Copy Item Properties"));

	(*scrActions)["toolsInsertTextFrame"]->setText( tr("Insert &Text Frame"));
	(*scrActions)["toolsInsertImageFrame"]->setText( tr("Insert &Image Frame"));
	(*scrActions)["toolsInsertLatexFrame"]->setText( tr("Insert &Latex Frame"));
	(*scrActions)["toolsInsertTableFrame"]->setText( tr("Insert T&able"));
	(*scrActions)["toolsInsertShape"]->setText( tr("Insert &Shape"));
	(*scrActions)["toolsInsertPolygon"]->setText( tr("Insert &Polygon"));
	(*scrActions)["toolsInsertLine"]->setText( tr("Insert &Line"));
	(*scrActions)["toolsInsertBezier"]->setText( tr("Insert &Bezier Curve"));
	(*scrActions)["toolsInsertFreehandLine"]->setText( tr("Insert &Freehand Line"));

	(*scrActions)["toolsPDFPushButton"]->setTexts( tr("Insert PDF Push Button"));
	(*scrActions)["toolsPDFTextField"]->setTexts( tr("Insert PDF Text Field"));
	(*scrActions)["toolsPDFCheckBox"]->setTexts( tr("Insert PDF Check Box"));
	(*scrActions)["toolsPDFComboBox"]->setTexts( tr("Insert PDF Combo Box"));
	(*scrActions)["toolsPDFListBox"]->setTexts( tr("Insert PDF List Box"));
	(*scrActions)["toolsPDFAnnotText"]->setTexts( tr("Insert Text Annotation"));
	(*scrActions)["toolsPDFAnnotLink"]->setTexts( tr("Insert Link Annotation"));


	//Extras Menu
	(*scrActions)["extrasManagePictures"]->setTexts( tr("&Manage Pictures"));
	(*scrActions)["extrasHyphenateText"]->setTexts( tr("&Hyphenate Text"));
	(*scrActions)["extrasDeHyphenateText"]->setTexts( tr("Dehyphenate Text"));
	(*scrActions)["extrasGenerateTableOfContents"]->setTexts( tr("&Generate Table Of Contents"));

	//Windows Menu
	(*scrActions)["windowsCascade"]->setText( tr("&Cascade"));
	(*scrActions)["windowsTile"]->setText( tr("&Tile"));

	//Help Menu
	(*scrActions)["helpAboutScribus"]->setTexts( tr("&About Scribus"));
	(*scrActions)["helpAboutPlugins"]->setTexts( tr("&About Plug-ins"));
	(*scrActions)["helpAboutQt"]->setTexts( tr("About &Qt"));
	(*scrActions)["helpTooltips"]->setTexts( tr("Toolti&ps"));
	(*scrActions)["helpManual"]->setTexts( tr("Scribus &Manual..."));
	(*scrActions)["helpOnlineWWW"]->setTexts( tr("Scribus Homepage"));
	(*scrActions)["helpOnlineDocs"]->setTexts( tr("Scribus Online Documentation"));
	(*scrActions)["helpOnlineWiki"]->setTexts( tr("Scribus Wiki"));
	(*scrActions)["helpOnlineTutorial1"]->setTexts( tr("Getting Started with Scribus"));
	(*scrActions)["helpCheckUpdates"]->setTexts( tr("Check updates"));

	//GUI
	(*scrActions)["specialToggleAllPalettes"]->setTexts( tr("Toggle Palettes"));
	(*scrActions)["specialToggleAllGuides"]->setTexts( tr("Toggle Guides"));

	languageChangeUnicodeActions(scrActions);
	languageChangeActions();
}

void ActionManager::languageChangeUnicodeActions(QMap<QString, QPointer<ScrAction> > *actionMap)
{
	//typography
	(*actionMap)["unicodeSmartHyphen"]->setText( tr("Smart &Hyphen"));
	(*actionMap)["unicodeNonBreakingHyphen"]->setText( tr("Non Breaking Dash"));
	(*actionMap)["unicodeNonBreakingSpace"]->setText( tr("Non Breaking &Space"));
	(*actionMap)["unicodePageNumber"]->setText( tr("Page &Number"));
	(*actionMap)["unicodeNewLine"]->setText( tr("New Line"));
	(*actionMap)["unicodeFrameBreak"]->setText( tr("Frame Break"));
	(*actionMap)["unicodeColumnBreak"]->setText( tr("Column Break"));
	(*actionMap)["unicodeZerowidthSpace"]->setText( tr("&Zero Width Space"));
	(*actionMap)["unicodeZerowidthNonBreakingSpace"]->setText( tr("Zero Width NB Space"));
	(*actionMap)["unicodeCopyRight"]->setTexts( tr("Copyright"));
	(*actionMap)["unicodeRegdTM"]->setTexts( tr("Registered Trademark"));
	(*actionMap)["unicodeTM"]->setTexts( tr("Trademark"));
	(*actionMap)["unicodeSolidus"]->setTexts( tr("Solidus"));
	(*actionMap)["unicodeBullet"]->setTexts( tr("Bullet"));
	(*actionMap)["unicodeMidpoint"]->setTexts( tr("Middle Dot"));
	(*actionMap)["unicodeDashEm"]->setTexts( tr("Em Dash"));
	(*actionMap)["unicodeDashEn"]->setTexts( tr("En Dash"));
	(*actionMap)["unicodeDashFigure"]->setTexts( tr("Figure Dash"));
	(*actionMap)["unicodeDashQuotation"]->setTexts( tr("Quotation Dash"));

	(*actionMap)["unicodeQuoteApostrophe"]->setTexts( tr("Apostrophe", "Unicode 0x0027"));
	(*actionMap)["unicodeQuoteStraight"]->setTexts( tr("Straight Double", "Unicode 0x0022"));
	(*actionMap)["unicodeQuoteSingleLeft"]->setTexts( tr("Single Left", "Unicode 0x2018"));
	(*actionMap)["unicodeQuoteSingleRight"]->setTexts( tr("Single Right", "Unicode 0x2019"));
	(*actionMap)["unicodeQuoteDoubleLeft"]->setTexts( tr("Double Left", "Unicode 0x201C"));
	(*actionMap)["unicodeQuoteDoubleRight"]->setTexts( tr("Double Right", "Unicode 0x201D"));
	(*actionMap)["unicodeQuoteSingleReversed"]->setTexts( tr("Single Reversed", "Unicode 0x201B"));
	(*actionMap)["unicodeQuoteDoubleReversed"]->setTexts( tr("Double Reversed", "Unicode 0x201F"));
	(*actionMap)["unicodeQuoteSingleLeftGuillemet"]->setTexts( tr("Single Left Guillemet", "Unicode 0x2039"));
	(*actionMap)["unicodeQuoteSingleRightGuillemet"]->setTexts( tr("Single Right Guillemet", "Unicode 0x203A"));
	(*actionMap)["unicodeQuoteDoubleLeftGuillemet"]->setTexts( tr("Double Left Guillemet", "Unicode 0x00AB"));
	(*actionMap)["unicodeQuoteDoubleRightGuillemet"]->setTexts( tr("Double Right Guillemet", "Unicode 0x00BB"));
	(*actionMap)["unicodeQuoteLowSingleComma"]->setTexts( tr("Low Single Comma", "Unicode 0x201A"));
	(*actionMap)["unicodeQuoteLowDoubleComma"]->setTexts( tr("Low Double Comma", "Unicode 0x201E"));
	(*actionMap)["unicodeQuoteCJKSingleLeft"]->setTexts( tr("CJK Single Left", "Unicode 0x300C"));
	(*actionMap)["unicodeQuoteCJKSingleRight"]->setTexts( tr("CJK Single Right", "Unicode 0x300D"));
	(*actionMap)["unicodeQuoteCJKDoubleLeft"]->setTexts( tr("CJK Double Left", "Unicode 0x300E"));
	(*actionMap)["unicodeQuoteCJKDoubleRight"]->setTexts( tr("CJK Double Right", "Unicode 0x300F"));

	(*actionMap)["unicodeSpaceEN"]->setTexts( tr("En Space"));
	(*actionMap)["unicodeSpaceEM"]->setTexts( tr("Em Space"));
	(*actionMap)["unicodeSpaceThin"]->setTexts( tr("Thin Space"));
	(*actionMap)["unicodeSpaceThick"]->setTexts( tr("Thick Space"));
	(*actionMap)["unicodeSpaceMid"]->setTexts( tr("Mid Space"));
	(*actionMap)["unicodeSpaceHair"]->setTexts( tr("Hair Space"));

// 	(*actionMap)["unicodeSmartHyphen"]->setText( tr("Insert Smart Hyphen"));
// 	(*actionMap)["unicodeNonBreakingHyphen"]->setText( tr("Insert Non Breaking Dash"));
// 	(*actionMap)["unicodeNonBreakingSpace"]->setText( tr("Insert Non Breaking Space"));
// 	(*actionMap)["unicodePageNumber"]->setText( tr("Insert Page Number"));
// 	(*actionMap)["unicodeNewLine"]->setText( tr("New Line"));
// 	(*actionMap)["unicodeFrameBreak"]->setText( tr("Frame Break"));
// 	(*actionMap)["unicodeColumnBreak"]->setText( tr("Column Break"));
// 	(*actionMap)["unicodeZerowidthSpace"]->setText( tr("&Zero Width Space"));
// 	(*actionMap)["unicodeZerowidthNonBreakingSpace"]->setText( tr("Zero Width NB Space"));

	(*actionMap)["unicodeLigature_ff"]->setTexts( tr("ff"));
	(*actionMap)["unicodeLigature_fi"]->setTexts( tr("fi"));
	(*actionMap)["unicodeLigature_fl"]->setTexts( tr("fl"));
	(*actionMap)["unicodeLigature_ffi"]->setTexts( tr("ffi"));
	(*actionMap)["unicodeLigature_ffl"]->setTexts( tr("ffl"));
	(*actionMap)["unicodeLigature_ft"]->setTexts( tr("ft"));
	(*actionMap)["unicodeLigature_st"]->setTexts( tr("st"));
}

void ActionManager::createDefaultShortcuts()
{
	defKeys.clear();

	defKeys.insert("fileNew", QKeySequence::New);
	defKeys.insert("fileOpen", QKeySequence::Open);
	defKeys.insert("fileClose", QKeySequence::Close);
	defKeys.insert("fileSave", QKeySequence::Save);
	defKeys.insert("fileSaveAs", Qt::CTRL+Qt::SHIFT+Qt::Key_S);
	defKeys.insert("fileRevert", QKeySequence());
	defKeys.insert("fileCollect", QKeySequence());
	defKeys.insert("fileImportText", Qt::CTRL+Qt::Key_D);
	defKeys.insert("fileImportAppendText", QKeySequence());
	defKeys.insert("fileImportImage", Qt::CTRL+Qt::Key_D);
	defKeys.insert("fileExportText", QKeySequence());
	defKeys.insert("fileExportAsEPS", QKeySequence());
	defKeys.insert("fileExportAsPDF", QKeySequence());
	defKeys.insert("fileDocSetup", QKeySequence());
	defKeys.insert("filePreferences", QKeySequence());
	defKeys.insert("filePrint", QKeySequence::Print);
	defKeys.insert("PrintPreview", Qt::CTRL+Qt::ALT+Qt::Key_P);
	defKeys.insert("fileQuit", Qt::CTRL+Qt::Key_Q);
	//Edit Menu
	defKeys.insert("editUndoAction", QKeySequence::Undo);
	defKeys.insert("editRedoAction", QKeySequence::Redo);
	defKeys.insert("editActionMode", QKeySequence());
	defKeys.insert("editCut", QKeySequence::Cut);
	defKeys.insert("editCopy", QKeySequence::Copy);
	defKeys.insert("editPaste", QKeySequence::Paste);
	defKeys.insert("editCopyContents", Qt::CTRL+Qt::SHIFT+Qt::Key_C);
	defKeys.insert("editPasteContents", Qt::CTRL+Qt::SHIFT+Qt::Key_V);
	defKeys.insert("editPasteContentsAbs", Qt::CTRL+Qt::ALT+Qt::SHIFT+Qt::Key_V);
	defKeys.insert("editClearContents", QKeySequence());
	defKeys.insert("editSelectAll", Qt::CTRL+Qt::Key_A);
	defKeys.insert("editDeselectAll", Qt::CTRL+Qt::SHIFT+Qt::Key_A);
	defKeys.insert("editSearchReplace", Qt::CTRL+Qt::Key_F);
	defKeys.insert("editEditWithImageEditor", QKeySequence());
	defKeys.insert("editEditWithLatexEditor", QKeySequence());
	defKeys.insert("editColors", QKeySequence());
	defKeys.insert("editPatterns", QKeySequence());
	defKeys.insert("editStyles", QKeySequence(Qt::Key_F3));
	defKeys.insert("editMasterPages", QKeySequence());
	defKeys.insert("editJavascripts", QKeySequence());

	int font_sizes[] = {7, 9, 10, 11, 12, 14, 18, 24, 36, 48, 60, 72};
	size_t f_size = sizeof(font_sizes) / sizeof(*font_sizes);
	for (uint s = 0; s < f_size; ++s)
	{
		QString fontSizeName=QString("fontSize%1").arg(font_sizes[s]);
		defKeys.insert(fontSizeName, QKeySequence());
	}
	defKeys.insert("fontSizeOther", QKeySequence());
	defKeys.insert("alignLeft", QKeySequence());
	defKeys.insert("alignCenter", QKeySequence());
	defKeys.insert("alignRight", QKeySequence());
	defKeys.insert("alignBlock", QKeySequence());
	defKeys.insert("alignForced", QKeySequence());

	for (uint i=0; i<=100 ; i+=10)
	{
		QString shadeName=QString("shade%1").arg(i);
		defKeys.insert(shadeName, QKeySequence());
	}

	defKeys.insert("shadeOther", QKeySequence());
	defKeys.insert("typeEffectNormal", QKeySequence());
	defKeys.insert("typeEffectUnderline", QKeySequence());
	defKeys.insert("typeEffectUnderlineWords", QKeySequence());
	defKeys.insert("typeEffectStrikeThrough", QKeySequence());
	defKeys.insert("typeEffectAllCaps", QKeySequence());
	defKeys.insert("typeEffectSmallCaps", QKeySequence());
	defKeys.insert("typeEffectSuperscript", QKeySequence());
	defKeys.insert("typeEffectSubscript", QKeySequence());
	defKeys.insert("typeEffectOutline", QKeySequence());
	defKeys.insert("typeEffectShadow", QKeySequence());

	defKeys.insert("styleImageEffects", Qt::CTRL+Qt::Key_E);
	defKeys.insert("styleTabulators", QKeySequence());

	//Item Menu
	defKeys.insert("itemDuplicate", Qt::CTRL+Qt::ALT+Qt::SHIFT+Qt::Key_D);
	defKeys.insert("itemMulDuplicate", QKeySequence());
	defKeys.insert("itemDelete", Qt::CTRL+Qt::Key_K);
	defKeys.insert("itemGroup", Qt::CTRL+Qt::Key_G);
	defKeys.insert("itemUngroup", Qt::CTRL+Qt::SHIFT+Qt::Key_G);
	defKeys.insert("itemLock", Qt::CTRL+Qt::Key_L);
	defKeys.insert("itemLockSize", Qt::CTRL+Qt::SHIFT+Qt::Key_L);
	defKeys.insert("itemPrintingEnabled", QKeySequence());
	defKeys.insert("itemFlipH", QKeySequence());
	defKeys.insert("itemFlipV", QKeySequence());
	defKeys.insert("itemLowerToBottom", Qt::Key_End);
	defKeys.insert("itemRaiseToTop", Qt::Key_Home);
	defKeys.insert("itemLower", Qt::CTRL+Qt::Key_End);
	defKeys.insert("itemRaise", Qt::CTRL+Qt::Key_Home);
	defKeys.insert("itemSendToScrapbook", QKeySequence());
	defKeys.insert("itemSendToPattern", QKeySequence());
	defKeys.insert("itemAttributes", QKeySequence());
	defKeys.insert("itemImageInfo", QKeySequence());
	defKeys.insert("itemImageIsVisible", QKeySequence());
	defKeys.insert("itemUpdateImage", QKeySequence());
	defKeys.insert("itemAdjustFrameToImage", QKeySequence());
	defKeys.insert("itemExtendedImageProperties", QKeySequence());
	defKeys.insert("itemPreviewLow", QKeySequence());
	defKeys.insert("itemPreviewNormal", QKeySequence());
	defKeys.insert("itemPreviewFull", QKeySequence());
	defKeys.insert("itemPDFIsBookmark", QKeySequence());
	defKeys.insert("itemPDFIsAnnotation", QKeySequence());
	defKeys.insert("itemPDFAnnotationProps", QKeySequence());
	defKeys.insert("itemPDFFieldProps", QKeySequence());
	defKeys.insert("itemShapeEdit", QKeySequence());
	defKeys.insert("itemAttachTextToPath", QKeySequence());
	defKeys.insert("itemDetachTextFromPath", QKeySequence());
	defKeys.insert("itemCombinePolygons", QKeySequence());
	defKeys.insert("itemSplitPolygons", QKeySequence());
	defKeys.insert("itemConvertToBezierCurve", QKeySequence());
	defKeys.insert("itemConvertToImageFrame", QKeySequence());
	defKeys.insert("itemConvertToOutlines", QKeySequence());
	defKeys.insert("itemConvertToPolygon", QKeySequence());
	defKeys.insert("itemConvertToTextFrame", QKeySequence());

	//Insert Menu
	defKeys.insert("insertFrame", QKeySequence());
	defKeys.insert("insertGlyph", QKeySequence());
	defKeys.insert("insertSampleText", QKeySequence());
	defKeys.insert("stickyTools", QKeySequence());

	//Page menu
	defKeys.insert("pageInsert", QKeySequence());
	defKeys.insert("pageImport", QKeySequence());
	defKeys.insert("pageDelete", QKeySequence());
	defKeys.insert("pageCopy", QKeySequence());
	defKeys.insert("pageMove", QKeySequence());
	defKeys.insert("pageApplyMasterPage", QKeySequence());
	defKeys.insert("pageCopyToMasterPage", QKeySequence());
	defKeys.insert("pageManageGuides", QKeySequence());
	defKeys.insert("pageManageMargins", QKeySequence());

	//View Menu
	defKeys.insert("viewFitInWindow", Qt::CTRL+Qt::Key_0);
	defKeys.insert("viewFitWidth", QKeySequence());
	defKeys.insert("viewFit50", QKeySequence());
	defKeys.insert("viewFit75", QKeySequence());
	defKeys.insert("viewFit100", Qt::CTRL+Qt::Key_1);
	defKeys.insert("viewFit200", QKeySequence());
	defKeys.insert("viewFit400", QKeySequence());
	defKeys.insert("viewFitPreview", QKeySequence());
	defKeys.insert("viewShowMargins", QKeySequence());
	defKeys.insert("viewShowBleeds", QKeySequence());
	defKeys.insert("viewShowFrames", QKeySequence());
	defKeys.insert("viewShowLayerMarkers", QKeySequence());
	defKeys.insert("viewShowImages", QKeySequence());
	defKeys.insert("viewShowGrid", QKeySequence());
	defKeys.insert("viewShowGuides", QKeySequence());
	defKeys.insert("viewShowColumnBorders", QKeySequence());
	defKeys.insert("viewShowBaseline", QKeySequence());
	defKeys.insert("viewShowTextChain", QKeySequence());
	defKeys.insert("viewShowTextControls", QKeySequence());
	defKeys.insert("viewShowRulers", Qt::CTRL+Qt::SHIFT+Qt::Key_R);
	defKeys.insert("viewRulerMode", QKeySequence());
	defKeys.insert("viewSnapToGrid", QKeySequence());
	defKeys.insert("viewSnapToGuides", QKeySequence());
	defKeys.insert("viewShowContextMenu", Qt::Key_Menu); //Context menu key on Windows. Do we have one to use on Linux/OSX? Super_L ?
//	defKeys.insert("viewNewView", QKeySequence());

	//Tool menu
	defKeys.insert("toolsProperties", Qt::Key_F2);
	defKeys.insert("toolsOutline", QKeySequence());
	defKeys.insert("toolsScrapbook", QKeySequence());
	defKeys.insert("toolsLayers", Qt::Key_F6);
	defKeys.insert("toolsPages", QKeySequence());
	defKeys.insert("toolsBookmarks", QKeySequence());
	defKeys.insert("toolsMeasurements", QKeySequence());
	defKeys.insert("toolsActionHistory", QKeySequence());
	defKeys.insert("toolsPreflightVerifier", QKeySequence());
	defKeys.insert("toolsAlignDistribute", QKeySequence());
	defKeys.insert("toolsToolbarTools", QKeySequence());
	defKeys.insert("toolsToolbarPDF", QKeySequence());

	//toolbar only items
	defKeys.insert("toolsSelect", Qt::Key_C);
	defKeys.insert("toolsInsertTextFrame", QKeySequence(Qt::Key_T));
	defKeys.insert("toolsInsertImageFrame", Qt::Key_I);
	defKeys.insert("toolsInsertTableFrame", Qt::Key_A);
	defKeys.insert("toolsInsertShape", Qt::Key_S);
	defKeys.insert("toolsInsertPolygon", Qt::Key_P);
	defKeys.insert("toolsInsertLine", Qt::Key_L);
	defKeys.insert("toolsInsertBezier", Qt::Key_B);
	defKeys.insert("toolsInsertFreehandLine", Qt::Key_F);
	defKeys.insert("toolsInsertLatexFrame", Qt::Key_D); //TODO: First free key. Select a meaningful
	defKeys.insert("toolsRotate", Qt::Key_R);
	defKeys.insert("toolsZoom", Qt::Key_Z);
	defKeys.insert("toolsZoomIn", Qt::CTRL+Qt::Key_Plus);
	defKeys.insert("toolsZoomOut", Qt::CTRL+Qt::Key_Minus);
	defKeys.insert("toolsEditContents", Qt::Key_E);
	defKeys.insert("toolsEditWithStoryEditor", Qt::CTRL+Qt::Key_Y);
	defKeys.insert("toolsLinkTextFrame", Qt::Key_N);
	defKeys.insert("toolsUnlinkTextFrame", Qt::Key_U);
	defKeys.insert("toolsEyeDropper", Qt::Key_Y);
	defKeys.insert("toolsCopyProperties", QKeySequence());

	//PDF items
	defKeys.insert("toolsPDFPushButton", QKeySequence());
	defKeys.insert("toolsPDFTextField", QKeySequence());
	defKeys.insert("toolsPDFCheckBox", QKeySequence());
	defKeys.insert("toolsPDFComboBox", QKeySequence());
	defKeys.insert("toolsPDFListBox", QKeySequence());
	defKeys.insert("toolsPDFAnnotText", QKeySequence());
	defKeys.insert("toolsPDFAnnotLink", QKeySequence());

	//Extras Menu
	defKeys.insert("extrasManagePictures", QKeySequence());
	defKeys.insert("extrasHyphenateText", QKeySequence());
	defKeys.insert("extrasDeHyphenateText", QKeySequence());
	defKeys.insert("extrasGenerateTableOfContents", QKeySequence());

	//Windows Menu
	defKeys.insert("windowsCascade", QKeySequence());
	defKeys.insert("windowsTile", QKeySequence());

	//Help Menu
	defKeys.insert("helpAboutScribus", QKeySequence());
	defKeys.insert("helpAboutPlugins", QKeySequence());
	defKeys.insert("helpAboutQt", QKeySequence());
	defKeys.insert("helpTooltips", QKeySequence());
	defKeys.insert("helpManual", Qt::Key_F1);
	defKeys.insert("helpOnlineWWW", QKeySequence());
	defKeys.insert("helpOnlineDocs", QKeySequence());
	defKeys.insert("helpOnlineWiki", QKeySequence());
	defKeys.insert("helpOnlineTutorial1", QKeySequence());
	defKeys.insert("helpCheckUpdates", QKeySequence());

	//GUI
	defKeys.insert("specialToggleAllPalettes", Qt::Key_F12);
	defKeys.insert("specialToggleAllGuides", Qt::Key_F11);

	//typography
	defKeys.insert("unicodeSmartHyphen", QKeySequence());
	defKeys.insert("unicodeNonBreakingHyphen", QKeySequence());
	defKeys.insert("unicodeNonBreakingSpace", QKeySequence());
	defKeys.insert("unicodePageNumber", QKeySequence());
	defKeys.insert("unicodeNewLine", QKeySequence());
	defKeys.insert("unicodeFrameBreak", Qt::CTRL+Qt::Key_Return);
	defKeys.insert("unicodeColumnBreak", Qt::CTRL+Qt::SHIFT+Qt::Key_Return);
	defKeys.insert("unicodeZerowidthSpace", QKeySequence());
	defKeys.insert("unicodeZerowidthNonBreakingSpace", QKeySequence());
	defKeys.insert("unicodeCopyRight", QKeySequence());
	defKeys.insert("unicodeRegdTM", QKeySequence());
	defKeys.insert("unicodeTM", QKeySequence());
	defKeys.insert("unicodeSolidus", QKeySequence());
	defKeys.insert("unicodeBullet", QKeySequence());
	defKeys.insert("unicodeMidpoint", QKeySequence());
	defKeys.insert("unicodeDashEm", QKeySequence());
	defKeys.insert("unicodeDashEn", QKeySequence());
	defKeys.insert("unicodeDashFigure", QKeySequence());
	defKeys.insert("unicodeDashQuotation", QKeySequence());

	defKeys.insert("unicodeQuoteApostrophe", QKeySequence());
	defKeys.insert("unicodeQuoteStraight", QKeySequence());
	defKeys.insert("unicodeQuoteSingleLeft", QKeySequence());
	defKeys.insert("unicodeQuoteSingleRight", QKeySequence());
	defKeys.insert("unicodeQuoteDoubleLeft", QKeySequence());
	defKeys.insert("unicodeQuoteDoubleRight", QKeySequence());
	defKeys.insert("unicodeQuoteSingleReversed", QKeySequence());
	defKeys.insert("unicodeQuoteDoubleReversed", QKeySequence());
	defKeys.insert("unicodeQuoteSingleLeftGuillemet", QKeySequence());
	defKeys.insert("unicodeQuoteSingleRightGuillemet", QKeySequence());
	defKeys.insert("unicodeQuoteDoubleLeftGuillemet", QKeySequence());
	defKeys.insert("unicodeQuoteDoubleRightGuillemet", QKeySequence());
	defKeys.insert("unicodeQuoteLowSingleComma", QKeySequence());
	defKeys.insert("unicodeQuoteLowDoubleComma", QKeySequence());
	defKeys.insert("unicodeQuoteCJKSingleLeft", QKeySequence());
	defKeys.insert("unicodeQuoteCJKSingleRight", QKeySequence());
	defKeys.insert("unicodeQuoteCJKDoubleLeft", QKeySequence());
	defKeys.insert("unicodeQuoteCJKDoubleRight", QKeySequence());

	defKeys.insert("unicodeSpaceEN", QKeySequence());
	defKeys.insert("unicodeSpaceEM", QKeySequence());
	defKeys.insert("unicodeSpaceThin", QKeySequence());
	defKeys.insert("unicodeSpaceThick", QKeySequence());
	defKeys.insert("unicodeSpaceMid", QKeySequence());
	defKeys.insert("unicodeSpaceHair", QKeySequence());

	defKeys.insert("unicodeSmartHyphen", Qt::CTRL+Qt::SHIFT+Qt::Key_Minus);
	defKeys.insert("unicodeNonBreakingHyphen", Qt::CTRL+Qt::ALT+Qt::Key_Minus);
	defKeys.insert("unicodeNonBreakingSpace", Qt::CTRL+Qt::Key_Space);
	defKeys.insert("unicodePageNumber", Qt::CTRL+Qt::SHIFT+Qt::ALT+Qt::Key_P);
	defKeys.insert("unicodeNewLine", Qt::SHIFT+Qt::Key_Return);

	defKeys.insert("unicodeLigature_ff", QKeySequence());
	defKeys.insert("unicodeLigature_fi", QKeySequence());
	defKeys.insert("unicodeLigature_fl", QKeySequence());
	defKeys.insert("unicodeLigature_ffi", QKeySequence());
	defKeys.insert("unicodeLigature_ffl", QKeySequence());
	defKeys.insert("unicodeLigature_ft", QKeySequence());
	defKeys.insert("unicodeLigature_st", QKeySequence());

	//Plugins
	defKeys.insert("ExportAsImage", Qt::CTRL+Qt::SHIFT+Qt::Key_E);
	defKeys.insert("NewFromDocumentTemplate", Qt::CTRL+Qt::ALT+Qt::Key_N);
	defKeys.insert("SaveAsDocumentTemplate", Qt::CTRL+Qt::ALT+Qt::Key_S);
}

void ActionManager::createDefaultMenus()
{   //CB TODO use this to also create the menus
	defMenus.clear();
	defMenus.append(QPair<QString, QStringList>("File", QStringList()));
	defMenus.append(QPair<QString, QStringList>("Edit", QStringList()));
	defMenus.append(QPair<QString, QStringList>("Style", QStringList()));
	defMenus.append(QPair<QString, QStringList>("Item", QStringList()));
	defMenus.append(QPair<QString, QStringList>("Insert", QStringList()));
	defMenus.append(QPair<QString, QStringList>("Page", QStringList()));
	defMenus.append(QPair<QString, QStringList>("View", QStringList()));
	defMenus.append(QPair<QString, QStringList>("Extras", QStringList()));
	defMenus.append(QPair<QString, QStringList>("Window", QStringList()));
	defMenus.append(QPair<QString, QStringList>("Help", QStringList()));
// 	defMenus.append(QPair<QString, QStringList>("Other", QStringList()));

	QVector< QPair<QString, QStringList> >::Iterator itmenu = defMenus.begin();
	//File
	itmenu->second << "fileNew" << "fileOpen" << "fileClose" << "fileSave" << "fileSaveAs" << "fileRevert" << "fileCollect";
	itmenu->second << "fileImportText" << "fileImportText2" << "fileImportAppendText" << "fileImportImage" << "fileExportText" << "fileExportAsEPS" << "fileExportAsPDF";
	itmenu->second << "fileDocSetup" << "filePreferences" << "filePrint" << "PrintPreview" << "fileQuit";
	++itmenu;
	//Edit
	itmenu->second << "editUndoAction" << "editRedoAction" << "editActionMode" << "editCut" << "editCopy" << "editPaste" << "editCopyContents" << "editPasteContents" << "editPasteContentsAbs" << "editClearContents" << "editSelectAll" << "editDeselectAll" << "editSearchReplace" << "toolsEditWithStoryEditor" << "editEditWithImageEditor" << "editEditWithLatexEditor" << "editExtendedImageProperties" << "editColors" << "editPatterns" << "editStyles" << "editMasterPages" << "editJavascripts";
	//Style
	++itmenu;
	int font_sizes[] = {7, 9, 10, 11, 12, 14, 18, 24, 36, 48, 60, 72};
	size_t f_size = sizeof(font_sizes) / sizeof(*font_sizes);
	for (uint s = 0; s < f_size; ++s)
		itmenu->second << QString("fontSize%1").arg(font_sizes[s]);
	itmenu->second << "fontSizeOther";
	itmenu->second << "alignLeft" << "alignCenter" << "alignRight" << "alignBlock" << "alignForced";
	for (uint i=0; i<=100 ; i+=10)
		itmenu->second << QString("shade%1").arg(i);
	itmenu->second << "shadeOther";
	itmenu->second << "typeEffectNormal" << "typeEffectUnderline" << "typeEffectUnderlineWords" << "typeEffectStrikeThrough" << "typeEffectAllCaps" << "typeEffectSmallCaps" << "typeEffectSuperscript" << "typeEffectSubscript" << "typeEffectOutline" << "typeEffectShadow" << "styleImageEffects" << "styleTabulators";
	//Item
	++itmenu;
	itmenu->second << "itemDuplicate" << "itemMulDuplicate" << "itemDelete" << "itemGroup" << "itemUngroup" << "itemLock" << "itemLockSize" << "itemImageIsVisible" << "itemUpdateImage" << "itemAdjustFrameToImage" << "itemExtendedImageProperties" << "itemPreviewLow" << "itemPreviewNormal" << "itemPreviewFull" << "itemRaise" << "itemLower" << "itemRaiseToTop" << "itemLowerToBottom" << "itemSendToScrapbook" << "itemSendToPattern" << "itemAttributes" << "itemPDFIsAnnotation" << "itemPDFIsBookmark" << "itemPDFAnnotationProps" << "itemPDFFieldProps" << "itemShapeEdit" << "itemConvertToBezierCurve" << "itemConvertToImageFrame" << "itemConvertToOutlines" << "itemConvertToPolygon" << "itemConvertToTextFrame" << "itemAttachTextToPath" << "itemDetachTextFromPath" << "itemCombinePolygons" << "itemSplitPolygons";
	//Insert
	++itmenu;
	itmenu->second
		<< "insertFrame"
		<< "toolsInsertTextFrame"
		<< "toolsInsertImageFrame"
		<< "toolsInsertTableFrame"
		<< "toolsInsertShape"
		<< "toolsInsertPolygon"
		<< "toolsInsertLine"
		<< "toolsInsertBezier"
		<< "toolsInsertFreehandLine"
		<< "toolsInsertLatexFrame"
		<< "stickyTools"
		<< "insertGlyph"
		<< "insertSampleText";

	itmenu->second
		<< "unicodeSmartHyphen"
		<< "unicodeNonBreakingHyphen"
		<< "unicodeNonBreakingSpace"
		<< "unicodePageNumber"
		<< "unicodeNewLine"
		<< "unicodeFrameBreak"
		<< "unicodeColumnBreak"
		<< "unicodeZerowidthSpace"
		<< "unicodeCopyRight"
		<< "unicodeRegdTM"
		<< "unicodeTM"
		<< "unicodeSolidus"
		<< "unicodeBullet"
		<< "unicodeMidpoint"
		<< "unicodeDashEm"
		<< "unicodeDashEn"
		<< "unicodeDashFigure"
		<< "unicodeDashQuotation";

	 itmenu->second
		 << "unicodeQuoteApostrophe"
		 << "unicodeQuoteStraight"
		 << "unicodeQuoteSingleLeft"
		 << "unicodeQuoteSingleRight"
		 << "unicodeQuoteDoubleLeft"
		 << "unicodeQuoteDoubleRight"
		 << "unicodeQuoteSingleReversed"
		 << "unicodeQuoteDoubleReversed"
		 << "unicodeQuoteSingleLeftGuillemet"
		 << "unicodeQuoteSingleRightGuillemet"
		 << "unicodeQuoteDoubleLeftGuillemet"
		 << "unicodeQuoteDoubleRightGuillemet"
		 << "unicodeQuoteLowSingleComma"
		 << "unicodeQuoteLowDoubleComma"
		 << "unicodeQuoteCJKSingleLeft"
		 << "unicodeQuoteCJKSingleRight"
		 << "unicodeQuoteCJKDoubleLeft"
		 << "unicodeQuoteCJKDoubleRight";

	 itmenu->second
		 << "unicodeSpaceEN"
		 << "unicodeSpaceEM"
		 << "unicodeSpaceThin"
		 << "unicodeSpaceThick"
		 << "unicodeSpaceMid"
		 << "unicodeSpaceHair";

	 itmenu->second
		 << "unicodeSmartHyphen"
		 << "unicodeNonBreakingHyphen"
		 << "unicodeNonBreakingSpace"
		 << "unicodePageNumber"
		 << "unicodeNewLine"
		 << "unicodeFrameBreak"
		 << "unicodeColumnBreak";

	 itmenu->second
		 << "unicodeLigature_ff"
		 << "unicodeLigature_fi"
		 << "unicodeLigature_fl"
		 << "unicodeLigature_ffi"
		 << "unicodeLigature_ffl"
		 << "unicodeLigature_ft"
		 << "unicodeLigature_st";


	//Page
	++itmenu;
	itmenu->second << "pageInsert" << "pageImport" << "pageDelete" << "pageCopy" << "pageMove" << "pageApplyMasterPage" << "pageCopyToMasterPage" << "pageManageGuides" << "pageManageMargins" << "viewSnapToGrid" << "viewSnapToGuides";
	//View
	++itmenu;
	itmenu->second << "viewFitWidth" << "viewFitInWindow" << "viewFit50" << "viewFit75" << "viewFit100" << "viewFit200" << "viewFit400" << "viewFitPreview" << "viewShowMargins" << "viewShowBleeds" << "viewShowFrames" << "viewShowLayerMarkers" << "viewShowImages" << "viewShowGrid" << "viewShowGuides" << "viewShowColumnBorders" << "viewShowBaseline" << "viewShowTextChain" << "viewShowTextControls" << "viewShowRulers" << "viewRulerMode";
	//Extras
	++itmenu;
	itmenu->second << "extrasManagePictures" << "extrasHyphenateText" << "extrasDeHyphenateText" << "extrasGenerateTableOfContents";
	//Windows
	++itmenu;
	itmenu->second  << "windowsCascade" << "windowsTile" << "toolsProperties" << "toolsOutline" << "toolsScrapbook" << "toolsLayers" << "toolsPages" << "toolsBookmarks" << "toolsMeasurements" << "toolsActionHistory" << "toolsPreflightVerifier" << "toolsAlignDistribute" << "toolsToolbarTools" << "toolsToolbarPDF";
	//Help
	++itmenu;
	itmenu->second << "helpAboutScribus" << "helpAboutPlugins" << "helpAboutQt" << "helpTooltips" << "helpManual" << "helpOnlineWWW" << "helpOnlineDocs" << "helpOnlineWiki" << "helpOnlineTutorial1" << "helpCheckUpdates";
	//Other
// 	++itmenu;
// 	itmenu->second << "";
}

void ActionManager::createDefaultNonMenuActions()
{   //CB TODO use this to also create the menus
	defNonMenuActions.clear();
	defNonMenuActions.append(QPair<QString, QStringList>("Plugin Menu Items", QStringList()));
	defNonMenuActions.append(QPair<QString, QStringList>("Others", QStringList()));
	defNonMenuActions.append(QPair<QString, QStringList>("Unicode Characters", QStringList()));


	QVector< QPair<QString, QStringList> >::Iterator itnmenua = defNonMenuActions.begin();
	//Plugins
	itnmenua->second << "ExportAsImage";
	itnmenua->second << "NewFromDocumentTemplate";
	itnmenua->second << "SaveAsDocumentTemplate";
	
	//Others
	++itnmenua;
	itnmenua->second << "toolsSelect";
	itnmenua->second << "toolsRotate";
	itnmenua->second << "toolsZoom";
	itnmenua->second << "toolsZoomIn";
	itnmenua->second << "toolsZoomOut";
	itnmenua->second << "toolsEditContents";
	itnmenua->second << "toolsLinkTextFrame";
	itnmenua->second << "toolsUnlinkTextFrame";
	itnmenua->second << "toolsEyeDropper";
	itnmenua->second << "toolsCopyProperties";
	itnmenua->second << "toolsPDFPushButton";
	itnmenua->second << "toolsPDFTextField";
	itnmenua->second << "toolsPDFCheckBox";
	itnmenua->second << "toolsPDFListBox";
	itnmenua->second << "toolsPDFAnnotText";
	itnmenua->second << "toolsPDFAnnotLink";
	itnmenua->second << "specialToggleAllPalettes";
	itnmenua->second << "specialToggleAllGuides";
	itnmenua->second << "viewShowContextMenu";

	//Unicode
	++itnmenua;
	itnmenua->second << "unicodeSmartHyphen";
	itnmenua->second << "unicodeNonBreakingHyphen";
	itnmenua->second << "unicodeNonBreakingSpace";
	itnmenua->second << "unicodePageNumber";
	itnmenua->second << "unicodeNewLine";
	itnmenua->second << "unicodeFrameBreak";
	itnmenua->second << "unicodeColumnBreak";
	itnmenua->second << "unicodeZerowidthSpace";
	itnmenua->second << "unicodeCopyRight";
	itnmenua->second << "unicodeRegdTM";
	itnmenua->second << "unicodeTM";
	itnmenua->second << "unicodeSolidus";
	itnmenua->second << "unicodeBullet";
	itnmenua->second << "unicodeMidpoint";
	itnmenua->second << "unicodeDashEm";
	itnmenua->second << "unicodeDashEn";
	itnmenua->second << "unicodeDashFigure";
	itnmenua->second << "unicodeDashQuotation";

	itnmenua->second << "unicodeQuoteApostrophe";
	itnmenua->second << "unicodeQuoteStraight";
	itnmenua->second << "unicodeQuoteSingleLeft";
	itnmenua->second << "unicodeQuoteSingleRight";
	itnmenua->second << "unicodeQuoteDoubleLeft";
	itnmenua->second << "unicodeQuoteDoubleRight";
	itnmenua->second << "unicodeQuoteSingleReversed";
	itnmenua->second << "unicodeQuoteDoubleReversed";
	itnmenua->second << "unicodeQuoteSingleLeftGuillemet";
	itnmenua->second << "unicodeQuoteSingleRightGuillemet";
	itnmenua->second << "unicodeQuoteDoubleLeftGuillemet";
	itnmenua->second << "unicodeQuoteDoubleRightGuillemet";
	itnmenua->second << "unicodeQuoteLowSingleComma";
	itnmenua->second << "unicodeQuoteLowDoubleComma";
	itnmenua->second << "unicodeQuoteCJKSingleLeft";
	itnmenua->second << "unicodeQuoteCJKSingleRight";
	itnmenua->second << "unicodeQuoteCJKDoubleLeft";
	itnmenua->second << "unicodeQuoteCJKDoubleRight";

	itnmenua->second << "unicodeSpaceEN";
	itnmenua->second << "unicodeSpaceEM";
	itnmenua->second << "unicodeSpaceThin";
	itnmenua->second << "unicodeSpaceThick";
	itnmenua->second << "unicodeSpaceMid";
	itnmenua->second << "unicodeSpaceHair";

	itnmenua->second << "unicodeSmartHyphen";
	itnmenua->second << "unicodeNonBreakingHyphen";
	itnmenua->second << "unicodeNonBreakingSpace";
	itnmenua->second << "unicodePageNumber";
	itnmenua->second << "unicodeNewLine";

	itnmenua->second << "unicodeLigature_ff";
	itnmenua->second << "unicodeLigature_fi";
	itnmenua->second << "unicodeLigature_fl";
	itnmenua->second << "unicodeLigature_ffi";
	itnmenua->second << "unicodeLigature_ffl";
	itnmenua->second << "unicodeLigature_ft";
	itnmenua->second << "unicodeLigature_st";
}

void ActionManager::handleMultipleSelections(bool isMultiple)
{
	(*scrActions)["itemAttributes"]->setEnabled(!isMultiple);
}

void ActionManager::languageChangeActions()
{
	//Here we mangle the URL based on the current GUI language, returning English if we dont get one of these hard coded options.
	//CB TODO make more flexible one day.
	QString language="EN";
	QString langpref(ScCore->getGuiLanguage().left(2));
	if (langpref=="de" || langpref=="fr" || langpref=="po" || langpref=="pt" || langpref=="ru")
		language=langpref.toUpper();
	(*scrActions)["helpOnlineTutorial1"]->setActionQString("http://wiki.scribus.net/index.php/tutorial"+language);
}
