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
*   ScMW program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "actionmanager.h"
#include "actionmanager.moc"

#include "scribus.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "util.h"

ActionManager::ActionManager ( QObject * parent, const char * name ) : QObject ( parent, name )
{
	ScMW=(ScribusMainWindow *)parent;
	scrActions=&(ScMW->scrActions);
	scrActionGroups=&(ScMW->scrActionGroups);
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
	initUnicodeActions(scrActions, ScMW, unicodeCharActionNames);
	enableUnicodeActions(false);
	initSpecialActions();
}

void ActionManager::initFileMenuActions()
{
	//File Menu
	scrActions->insert("fileNew", new ScrAction(QIconSet(loadIcon("DateiNeu16.png"), loadIcon("DateiNeu.xpm")), "", CTRL+Key_N, ScMW, "fileNew"));
	scrActions->insert("fileOpen", new ScrAction(QIconSet(loadIcon("DateiOpen16.png"), loadIcon("DateiOpen.xpm")), "", CTRL+Key_O, ScMW, "fileOpen"));
	scrActions->insert("fileClose", new ScrAction(QIconSet(loadIcon("DateiClos16.png"), loadIcon("DateiClose.png")), "", CTRL+Key_W, ScMW, "fileClose"));
	scrActions->insert("fileSave", new ScrAction(QIconSet(loadIcon("DateiSave16.png"), loadIcon("DateiSave2.png")), "", CTRL+Key_S, ScMW, "fileSave"));
	scrActions->insert("fileSaveAs", new ScrAction(loadIcon("filesaveas.png"), "", CTRL+SHIFT+Key_S, ScMW, "fileSaveAs"));
	scrActions->insert("fileRevert", new ScrAction(loadIcon("revert.png"), "", QKeySequence(), ScMW, "fileRevert"));
	scrActions->insert("fileCollect", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "fileCollect"));
	//File Import Menu
	scrActions->insert("fileImportText", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "fileImportText"));
	scrActions->insert("fileImportAppendText", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "fileImportAppendText"));
	scrActions->insert("fileImportImage", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "fileImportImage"));

	//File Export Menu
	scrActions->insert("fileExportText", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "fileExportText"));
	scrActions->insert("fileExportAsEPS", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "fileExportAsEPS"));
	scrActions->insert("fileExportAsPDF", new ScrAction(loadIcon("acrobat.png"), "", QKeySequence(), ScMW, "fileExportAsPDF"));
	//Rest of File Menu
	scrActions->insert("fileDocSetup", new ScrAction(QPixmap(loadIcon("scribusdoc16.png")), "", 0, ScMW, "fileDocSetup"));
	scrActions->insert("filePrint", new ScrAction(QIconSet(loadIcon("DateiPrint16.png"), loadIcon("DateiPrint.xpm")), "", CTRL+Key_P, ScMW, "filePrint"));
	scrActions->insert("PrintPreview", new ScrAction(QIconSet(noIcon), "", CTRL+ALT+Key_P, ScMW, "PrintPreview"));
	scrActions->insert("fileQuit", new ScrAction(QPixmap(loadIcon("exit.png")), "", CTRL+Key_Q, ScMW, "fileQuit"));

	//Connect our signals and slots
	//File Menu
	connect( (*scrActions)["fileNew"], SIGNAL(activated()), ScMW, SLOT(slotFileNew()) );
	connect( (*scrActions)["fileOpen"], SIGNAL(activated()), ScMW, SLOT(slotDocOpen()) );
	connect( (*scrActions)["fileClose"], SIGNAL(activated()), ScMW, SLOT(slotFileClose()) );
	connect( (*scrActions)["filePrint"], SIGNAL(activated()), ScMW, SLOT(slotFilePrint()) );
	connect( (*scrActions)["PrintPreview"], SIGNAL(activated()), ScMW, SLOT(printPreview()) );
	connect( (*scrActions)["fileSave"], SIGNAL(activated()), ScMW, SLOT(slotFileSave()) );
	connect( (*scrActions)["fileSaveAs"], SIGNAL(activated()), ScMW, SLOT(slotFileSaveAs()) );
	connect( (*scrActions)["fileDocSetup"], SIGNAL(activated()), ScMW, SLOT(slotDocSetup()) );
	connect( (*scrActions)["fileRevert"], SIGNAL(activated()), ScMW, SLOT(slotFileRevert()) );
	connect( (*scrActions)["fileCollect"], SIGNAL(activated()), ScMW, SLOT(Collect()) );
	connect( (*scrActions)["fileQuit"], SIGNAL(activated()), ScMW, SLOT(slotFileQuit()) );
	//File Import Menu
	connect( (*scrActions)["fileImportText"], SIGNAL(activated()), ScMW, SLOT(slotGetContent()) );
	connect( (*scrActions)["fileImportAppendText"], SIGNAL(activated()), ScMW, SLOT(slotFileAppend()) );
	connect( (*scrActions)["fileImportImage"], SIGNAL(activated()), ScMW, SLOT(slotGetContent()) );
	//File Export Menu
	connect( (*scrActions)["fileExportText"], SIGNAL(activated()), ScMW, SLOT(SaveText()) );
	connect( (*scrActions)["fileExportAsEPS"], SIGNAL(activated()), ScMW, SLOT(SaveAsEps()) );
	connect( (*scrActions)["fileExportAsPDF"], SIGNAL(activated()), ScMW, SLOT(SaveAsPDF()) );
	//The rest are plugins
}

void ActionManager::initEditMenuActions()
{
	//Edit Menu
	scrActions->insert("editUndoAction", new ScrAction(ScrAction::DataInt, QIconSet(loadIcon("u_undo16.png"), loadIcon("u_undo.png")), "", CTRL+Key_Z, ScMW, "editUndoAction",1));
	scrActions->insert("editRedoAction", new ScrAction(ScrAction::DataInt, QIconSet(loadIcon("u_redo16.png"), loadIcon("u_redo.png")), "", CTRL+SHIFT+Key_Z, ScMW, "editRedoAction", 1));
	scrActions->insert("editActionMode", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "editActionMode"));
	(*scrActions)["editActionMode"]->setToggleAction(true);
	scrActions->insert("editCut", new ScrAction(QIconSet(loadIcon("editcut.png"), loadIcon("editcut22.png")), "", CTRL+Key_X, ScMW, "editCut"));
	scrActions->insert("editCopy", new ScrAction(QIconSet(loadIcon("editcopy.png"), loadIcon("editcopy.png")), "", CTRL+Key_C, ScMW, "editCopy"));
	scrActions->insert("editPaste", new ScrAction(QIconSet(loadIcon("editpaste.png"), loadIcon("editpaste.png")), "", CTRL+Key_V, ScMW, "editPaste"));
	scrActions->insert("editClearContents", new ScrAction(QIconSet(loadIcon("editdelete.png"), loadIcon("editdelete22.png")), "", QKeySequence(), ScMW, "editClearContents"));
	scrActions->insert("editSelectAll", new ScrAction(QIconSet(noIcon), "", CTRL+Key_A, ScMW, "editSelectAll"));
	scrActions->insert("editDeselectAll", new ScrAction(QIconSet(noIcon), "", CTRL+SHIFT+Key_A, ScMW, "editDeselectAll"));
	scrActions->insert("editSearchReplace", new ScrAction(QIconSet(loadIcon("find16.png"), loadIcon("find16.png")), "", QKeySequence(CTRL+Key_F), ScMW, "editSearchReplace"));

	scrActions->insert("editEditWithImageEditor", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "editEditWithImageEditor"));

	scrActions->insert("editColors", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "editColors"));
	scrActions->insert("editParaStyles", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "editParaStyles"));
	scrActions->insert("editLineStyles", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "editLineStyles"));
	scrActions->insert("editMasterPages", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "editMasterPages"));
	scrActions->insert("editJavascripts", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "editJavascripts"));
	scrActions->insert("editPreferences", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "editPreferences"));

	connect( (*scrActions)["editUndoAction"], SIGNAL(activatedData(int)) , undoManager, SLOT(undo(int)) );
	connect( (*scrActions)["editRedoAction"], SIGNAL(activatedData(int)) , undoManager, SLOT(redo(int)) );
	connect( (*scrActions)["editActionMode"], SIGNAL(toggled(bool)), ScMW, SLOT(setUndoMode(bool)) );
	connect( (*scrActions)["editCut"], SIGNAL(activated()), ScMW, SLOT(slotEditCut()) );
	connect( (*scrActions)["editCopy"], SIGNAL(activated()), ScMW, SLOT(slotEditCopy()) );
	connect( (*scrActions)["editPaste"], SIGNAL(activated()), ScMW, SLOT(slotEditPaste()) );
	connect( (*scrActions)["editClearContents"], SIGNAL(activated()), ScMW, SLOT(clearContents()) );
	connect( (*scrActions)["editSelectAll"], SIGNAL(activated()), ScMW, SLOT(SelectAll()) );
	connect( (*scrActions)["editDeselectAll"], SIGNAL(activated()), ScMW, SLOT(deselectAll()) );
	connect( (*scrActions)["editSearchReplace"], SIGNAL(activated()), ScMW, SLOT(SearchText()) );
	connect( (*scrActions)["editEditWithImageEditor"], SIGNAL(activated()), ScMW, SLOT(callImageEditor()) );
	connect( (*scrActions)["editColors"], SIGNAL(activated()), ScMW, SLOT(slotEditColors()) );
	connect( (*scrActions)["editParaStyles"], SIGNAL(activated()), ScMW, SLOT(slotEditStyles()) );
	connect( (*scrActions)["editLineStyles"], SIGNAL(activated()), ScMW, SLOT(slotEditLineStyles()) );
	connect( (*scrActions)["editMasterPages"], SIGNAL(activated()), ScMW, SLOT(manageMasterPages()) );
	connect( (*scrActions)["editJavascripts"], SIGNAL(activated()), ScMW, SLOT(ManageJava()) );
	connect( (*scrActions)["editPreferences"], SIGNAL(activated()), ScMW, SLOT(slotPrefsOrg()) );
}

void ActionManager::initStyleMenuActions()
{
	//Text Size actions
	(*scrActionGroups).insert("fontSize", new QActionGroup(ScMW, "fontSize", true));
	scrActions->insert("fontSizeOther", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["fontSize"], "fontSizeOther", -1));
	connect( (*scrActions)["fontSizeOther"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemFSize(int)));

	int font_sizes[] = {7, 9, 10, 11, 12, 14, 18, 24, 36, 48, 60, 72};
	size_t f_size = sizeof(font_sizes) / sizeof(*font_sizes);
	for (uint s = 0; s < f_size; ++s)
	{
		QString fontSizeName=QString("fontSize%1").arg(font_sizes[s]);
		scrActions->insert(fontSizeName, new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["fontSize"], fontSizeName, font_sizes[s]));
		(*scrActions)[fontSizeName]->setToggleAction(true);
		connect( (*scrActions)[fontSizeName], SIGNAL(activatedData(int)), ScMW, SLOT(setItemFSize(int)));
	}

	//Alignment actions
	scrActions->insert("alignLeft", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScMW, "alignLeft", 0));
	scrActions->insert("alignCenter", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScMW, "alignCenter", 1));
	scrActions->insert("alignRight", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScMW, "alignRight", 2));
	scrActions->insert("alignBlock", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScMW, "alignBlock", 3));
	scrActions->insert("alignForced", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScMW, "alignForced", 4));

	(*scrActions)["alignLeft"]->setToggleAction(true);
	(*scrActions)["alignCenter"]->setToggleAction(true);
	(*scrActions)["alignRight"]->setToggleAction(true);
	(*scrActions)["alignBlock"]->setToggleAction(true);
	(*scrActions)["alignForced"]->setToggleAction(true);

	connect( (*scrActions)["alignLeft"], SIGNAL(activatedData(int)), ScMW, SLOT(setNewAbStyle(int)));
	connect( (*scrActions)["alignCenter"], SIGNAL(activatedData(int)), ScMW, SLOT(setNewAbStyle(int)));
	connect( (*scrActions)["alignRight"], SIGNAL(activatedData(int)), ScMW, SLOT(setNewAbStyle(int)));
	connect( (*scrActions)["alignBlock"], SIGNAL(activatedData(int)), ScMW, SLOT(setNewAbStyle(int)));
	connect( (*scrActions)["alignForced"], SIGNAL(activatedData(int)), ScMW, SLOT(setNewAbStyle(int)));

	//Shade actions
	scrActionGroups->insert("shade", new QActionGroup(ScMW, "shade", true));
	scrActions->insert("shadeOther", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["shade"], "shadeOther", -1));
	connect( (*scrActions)["shadeOther"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemShade(int)));
	for (uint i=0; i<=100 ; i+=10)
	{
		QString shadeName=QString("shade%1").arg(i);
		scrActions->insert(shadeName, new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["shade"], shadeName, i));
		(*scrActions)[shadeName]->setToggleAction(true);
		connect( (*scrActions)[shadeName], SIGNAL(activatedData(int)), ScMW, SLOT(setItemShade(int)));
	}

	//Type Effects actions
	scrActionGroups->insert("typeEffects", new QActionGroup(ScMW, "typeEffects", false));
	scrActions->insert("typeEffectNormal", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectNormal", 0));
	scrActions->insert("typeEffectUnderline", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectUnderline", 1));
	scrActions->insert("typeEffectUnderlineWords", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectUnderlineWords", 8));
	scrActions->insert("typeEffectStrikeThrough", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectStrikeThrough", 2));
	scrActions->insert("typeEffectAllCaps", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectAllCaps", 7));
	scrActions->insert("typeEffectSmallCaps", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectSmallCaps", 3));
	scrActions->insert("typeEffectSuperscript", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectSuperscript", 4));
	scrActions->insert("typeEffectSubscript", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectSubscript", 5));
	scrActions->insert("typeEffectOutline", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectOutline", 6));
	scrActions->insert("typeEffectShadow", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectShadow", 9));
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
	connect( (*scrActions)["typeEffectNormal"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectUnderline"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectUnderlineWords"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectStrikeThrough"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSmallCaps"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectAllCaps"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSuperscript"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSubscript"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectOutline"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectShadow"], SIGNAL(activatedData(int)), ScMW, SLOT(setItemTypeStyle(int)));

	//Other Style menu items that get added in various places
	scrActions->insert("styleInvertPict", new ScrAction(ScMW, "styleInvertPict"));
	scrActions->insert("styleTabulators", new ScrAction(ScMW, "styleTabulators"));
	connect( (*scrActions)["styleInvertPict"], SIGNAL(activated()), ScMW, SLOT(ImageEffects()));
	connect( (*scrActions)["styleTabulators"], SIGNAL(activated()), ScMW, SLOT(EditTabs()));

}

void ActionManager::initItemMenuActions()
{
	//Item Menu
	scrActions->insert("itemDuplicate", new ScrAction("", CTRL+Key_D, ScMW, "itemDuplicate"));
	scrActions->insert("itemMulDuplicate", new ScrAction(ScMW, "itemMulDuplicate"));
	scrActions->insert("itemDelete", new ScrAction("", CTRL+Key_K, ScMW, "itemDelete"));
	scrActions->insert("itemGroup", new ScrAction("", CTRL+Key_G, ScMW, "itemGroup"));
	scrActions->insert("itemUngroup", new ScrAction("", CTRL+Key_U, ScMW, "itemUngroup"));
	scrActions->insert("itemLock", new ScrAction("", CTRL+Key_L, ScMW, "itemLock"));
	scrActions->insert("itemLockSize", new ScrAction("", CTRL+SHIFT+Key_L, ScMW, "itemLockSize"));
	(*scrActions)["itemLock"]->setToggleAction(true);
	(*scrActions)["itemLockSize"]->setToggleAction(true);
	scrActions->insert("itemLowerToBottom", new ScrAction(QIconSet(loadIcon("lower-to-bottom.png"), loadIcon("lower-to-bottom.png")), "", QKeySequence(Key_End), ScMW, "itemLowerToBottom"));
	scrActions->insert("itemRaiseToTop", new ScrAction(QIconSet(loadIcon("raise-to-top.png"), loadIcon("raise-to-top.png")), "", QKeySequence(Key_Home), ScMW, "itemRaiseToTop"));
	scrActions->insert("itemLower", new ScrAction(QIconSet(loadIcon("lower.png"), loadIcon("lower.png")), "", QKeySequence(CTRL+Key_End), ScMW, "itemLower"));
	scrActions->insert("itemRaise", new ScrAction(QIconSet(loadIcon("raise.png"), loadIcon("raise.png")), "", QKeySequence(CTRL+Key_Home), ScMW, "itemRaise"));
	
	scrActions->insert("itemSendToScrapbook", new ScrAction(ScMW, "itemSendToScrapbook"));

	scrActions->insert("itemImageInfo", new ScrAction(ScMW, "itemImageInfo"));
	scrActions->insert("itemAttributes", new ScrAction(ScMW, "itemAttributes"));
	scrActions->insert("itemImageIsVisible", new ScrAction(ScMW, "itemImageIsVisible"));
	scrActions->insert("itemUpdateImage", new ScrAction(ScMW, "itemUpdateImage"));
	scrActions->insert("itemAdjustFrameToImage", new ScrAction(ScMW, "itemAdjustFrameToImage"));
	scrActions->insert("itemExtendedImageProperties", new ScrAction(ScMW, "itemExtendedImageProperties"));
	scrActions->insert("itemPreviewLow", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScMW, "itemPreviewLow", 2));
	scrActions->insert("itemPreviewNormal", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScMW, "itemPreviewNormal", 1));
	scrActions->insert("itemPreviewFull", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScMW, "itemPreviewFull", 0));
	scrActions->insert("itemPDFIsBookmark", new ScrAction(ScMW, "itemPDFIsBookmark"));
	(*scrActions)["itemPDFIsBookmark"]->setToggleAction(true);
	scrActions->insert("itemPDFIsAnnotation", new ScrAction(ScMW, "itemPDFIsAnnotation"));
	(*scrActions)["itemPDFIsAnnotation"]->setToggleAction(true);
	scrActions->insert("itemPDFAnnotationProps", new ScrAction(ScMW, "itemPDFAnnotationProps"));
	scrActions->insert("itemPDFFieldProps", new ScrAction(ScMW, "itemPDFFieldProps"));

	(*scrActions)["itemPDFIsBookmark"]->setEnabled(false);
	(*scrActions)["itemPDFIsAnnotation"]->setEnabled(false);
	(*scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
	(*scrActions)["itemPDFFieldProps"]->setEnabled(false);

	(*scrActions)["itemImageIsVisible"]->setToggleAction(true);
	(*scrActions)["itemPreviewLow"]->setToggleAction(true);
	(*scrActions)["itemPreviewNormal"]->setToggleAction(true);
	(*scrActions)["itemPreviewFull"]->setToggleAction(true);

	scrActions->insert("itemShapeEdit", new ScrAction(ScMW, "itemShapeEdit"));
	(*scrActions)["itemShapeEdit"]->setToggleAction(true);
	scrActions->insert("itemAttachTextToPath", new ScrAction(ScMW, "itemAttachTextToPath"));
	scrActions->insert("itemDetachTextFromPath", new ScrAction(ScMW, "itemDetachTextFromPath"));
	scrActions->insert("itemCombinePolygons", new ScrAction(ScMW, "itemCombinePolygons"));
	scrActions->insert("itemSplitPolygons", new ScrAction(ScMW, "itemSplitPolygons"));
	scrActions->insert("itemConvertToBezierCurve", new ScrAction(ScMW, "itemConvertToBezierCurve"));
	scrActions->insert("itemConvertToImageFrame", new ScrAction(ScMW, "itemConvertToImageFrame"));
	scrActions->insert("itemConvertToOutlines", new ScrAction(ScMW, "itemConvertToOutlines"));
	scrActions->insert("itemConvertToPolygon", new ScrAction(ScMW, "itemConvertToPolygon"));
	scrActions->insert("itemConvertToTextFrame", new ScrAction(ScMW, "itemConvertToTextFrame"));

	connect( (*scrActions)["itemDuplicate"], SIGNAL(activated()), ScMW, SLOT(ObjektDup()) );
	connect( (*scrActions)["itemMulDuplicate"], SIGNAL(activated()), ScMW, SLOT(ObjektDupM()) );
	connect( (*scrActions)["itemDelete"], SIGNAL(activated()), ScMW, SLOT(DeleteObjekt()) );
	connect( (*scrActions)["itemGroup"], SIGNAL(activated()), ScMW, SLOT(GroupObj()) );
	connect( (*scrActions)["itemUngroup"], SIGNAL(activated()), ScMW, SLOT(UnGroupObj()) );
	//connect( (*scrActions)["itemLock"], SIGNAL(activated()), ScMW, SLOT(ToggleObjLock()) );
	//connect( (*scrActions)["itemLockSize"], SIGNAL(activated()), ScMW, SLOT(ToggleObjSizeLock()) );
	//connect( (*scrActions)["itemPDFIsAnnotation"], SIGNAL(activated()), ScMW, SLOT(ToggleObjPDFAnnotation()) );
	//connect( (*scrActions)["itemPDFIsBookmark"], SIGNAL(activated()), ScMW, SLOT(ToggleObjPDFBookmark()) );
	connect( (*scrActions)["itemPDFAnnotationProps"], SIGNAL(activated()), ScMW, SLOT(ModifyAnnot()) );
	connect( (*scrActions)["itemPDFFieldProps"], SIGNAL(activated()), ScMW, SLOT(ModifyAnnot()) );
	//connect( (*scrActions)["itemLowerToBottom"], SIGNAL(activated()), ScMW, SLOT(Objekt2Back()) );
	//connect( (*scrActions)["itemRaiseToTop"], SIGNAL(activated()), ScMW, SLOT(Objekt2Front()) );
	//connect( (*scrActions)["itemLower"], SIGNAL(activated()), ScMW, SLOT(ObjektLower()) );
	//connect( (*scrActions)["itemRaise"], SIGNAL(activated()), ScMW, SLOT(ObjektRaise()) );
	connect( (*scrActions)["itemSendToScrapbook"], SIGNAL(activated()), ScMW, SLOT(PutScrap()) );
	connect( (*scrActions)["itemAttributes"], SIGNAL(activated()), ScMW, SLOT(objectAttributes()) );
	connect( (*scrActions)["itemShapeEdit"], SIGNAL(activated()), ScMW, SLOT(ToggleFrameEdit()) );
	connect( (*scrActions)["itemImageInfo"], SIGNAL(activated()), ScMW, SLOT(getImageInfo()) );
	//connect( (*scrActions)["itemAttachTextToPath"], SIGNAL(activated()), ScMW, SLOT(Pfadtext()) );
	//connect( (*scrActions)["itemDetachTextFromPath"], SIGNAL(activated()), ScMW, SLOT(noPfadtext()) );
	//connect( (*scrActions)["itemCombinePolygons"], SIGNAL(activated()), ScMW, SLOT(UniteOb()) );
	//connect( (*scrActions)["itemSplitPolygons"], SIGNAL(activated()), ScMW, SLOT(SplitUniteOb()) );
	//connect( (*scrActions)["itemConvertToBezierCurve"], SIGNAL(activated()), ScMW, SLOT(convertToBezierCurve()) );
	//connect( (*scrActions)["itemConvertToImageFrame"], SIGNAL(activated()), ScMW, SLOT(convertToImageFrame()) );
	//connect( (*scrActions)["itemConvertToOutlines"], SIGNAL(activated()), ScMW, SLOT(convertToOutlines()) );
	//connect( (*scrActions)["itemConvertToPolygon"], SIGNAL(activated()), ScMW, SLOT(convertToPolygon()) );
	//connect( (*scrActions)["itemConvertToTextFrame"], SIGNAL(activated()), ScMW, SLOT(convertToTextFrame()) );
}

void ActionManager::initInsertMenuActions()
{
	//Insert Menu
	scrActions->insert("insertGlyph", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "insertGlyph"));
	scrActions->insert("insertSampleText", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "insertSampleText"));

	connect( (*scrActions)["insertGlyph"], SIGNAL(activated()), ScMW, SLOT(slotCharSelect()) );
	connect( (*scrActions)["insertSampleText"], SIGNAL(activated()), ScMW, SLOT(insertSampleText()) );
}

void ActionManager::initPageMenuActions()
{
	//Page menu
	scrActions->insert("pageInsert", new ScrAction(ScMW, "pageInsert"));
	scrActions->insert("pageImport", new ScrAction(ScMW, "pageImport"));
	scrActions->insert("pageDelete", new ScrAction(ScMW, "pageDelete"));
	scrActions->insert("pageCopy", new ScrAction(ScMW, "pageCopy"));
	scrActions->insert("pageMove", new ScrAction(ScMW, "pageMove"));
	scrActions->insert("pageApplyMasterPage", new ScrAction(ScMW, "pageApplyMasterPage"));
	scrActions->insert("pageCopyToMasterPage", new ScrAction(ScMW, "pageCopyToMasterPage"));
	scrActions->insert("pageManageGuides", new ScrAction(ScMW, "pageManageGuides"));
	scrActions->insert("pageManageMargins", new ScrAction(ScMW, "pageManageMargins"));

	connect( (*scrActions)["pageInsert"], SIGNAL(activated()), ScMW, SLOT(slotNewPageM()) );
	connect( (*scrActions)["pageImport"], SIGNAL(activated()), ScMW, SLOT(slotPageImport()) );
	connect( (*scrActions)["pageDelete"], SIGNAL(activated()), ScMW, SLOT(DeletePage()) );
	connect( (*scrActions)["pageCopy"], SIGNAL(activated()), ScMW, SLOT(CopyPage()) );
	connect( (*scrActions)["pageMove"], SIGNAL(activated()), ScMW, SLOT(MovePage()) );
	connect( (*scrActions)["pageApplyMasterPage"], SIGNAL(activated()), ScMW, SLOT(ApplyMasterPage()) );
	connect( (*scrActions)["pageCopyToMasterPage"], SIGNAL(activated()), ScMW, SLOT(duplicateToMasterPage()) );
	connect( (*scrActions)["pageManageGuides"], SIGNAL(activated()), ScMW, SLOT(ManageGuides()) );
	connect( (*scrActions)["pageManageMargins"], SIGNAL(activated()), ScMW, SLOT(changePageMargins()) );
}

void ActionManager::initViewMenuActions()
{
	scrActions->insert("viewFitInWindow", new ScrAction(ScrAction::DataDouble, QIconSet(), "", CTRL+Key_0, ScMW, "viewFitInWindow", 0, -100.0));
	scrActions->insert("viewFit50", new ScrAction(ScrAction::DataDouble, QIconSet(), "", QKeySequence(), ScMW, "viewFit50", 0, 50.0));
	scrActions->insert("viewFit75", new ScrAction(ScrAction::DataDouble, QIconSet(), "", QKeySequence(), ScMW, "viewFit75", 0, 75.0));
	scrActions->insert("viewFit100", new ScrAction(ScrAction::DataDouble, QIconSet(), "", CTRL+Key_1, ScMW, "viewFit100", 0, 100.0));
	scrActions->insert("viewFit200", new ScrAction(ScrAction::DataDouble, QIconSet(), "", QKeySequence(), ScMW, "viewFit200", 0, 200.0));
	scrActions->insert("viewFit20", new ScrAction(ScrAction::DataDouble, QIconSet(), "", QKeySequence(), ScMW, "viewFit20", 0, 20.0));
	scrActions->insert("viewShowMargins", new ScrAction(ScMW, "viewShowMargins"));
	scrActions->insert("viewShowFrames", new ScrAction(ScMW, "viewShowFrames"));
	scrActions->insert("viewShowImages", new ScrAction(ScMW, "viewShowImages"));
	scrActions->insert("viewShowGrid", new ScrAction(ScMW, "viewShowGrid"));
	scrActions->insert("viewShowGuides", new ScrAction(ScMW, "viewShowGuides"));
	scrActions->insert("viewShowBaseline", new ScrAction(ScMW, "viewShowBaseline"));
	scrActions->insert("viewShowTextChain", new ScrAction(ScMW, "viewShowTextChain"));
	scrActions->insert("viewShowTextControls", new ScrAction(ScMW, "viewShowTextControls"));
	scrActions->insert("viewRulerMode", new ScrAction(ScMW, "viewRulerMode"));
	scrActions->insert("viewSnapToGrid", new ScrAction(ScMW, "viewSnapToGrid"));
	scrActions->insert("viewSnapToGuides", new ScrAction(ScMW, "viewSnapToGuides"));
//	scrActions->insert("viewNewView", new ScrAction(ScMW, "viewNewView"));

	(*scrActions)["viewShowMargins"]->setToggleAction(true);
	(*scrActions)["viewShowFrames"]->setToggleAction(true);
	(*scrActions)["viewShowImages"]->setToggleAction(true);
	(*scrActions)["viewShowGrid"]->setToggleAction(true);
	(*scrActions)["viewShowGuides"]->setToggleAction(true);
	(*scrActions)["viewShowBaseline"]->setToggleAction(true);
	(*scrActions)["viewShowTextChain"]->setToggleAction(true);
	(*scrActions)["viewShowTextControls"]->setToggleAction(true);
	(*scrActions)["viewRulerMode"]->setToggleAction(true);
	(*scrActions)["viewSnapToGrid"]->setToggleAction(true);
	(*scrActions)["viewSnapToGuides"]->setToggleAction(true);

	(*scrActions)["viewShowMargins"]->setOn(true);
	(*scrActions)["viewShowFrames"]->setOn(true);
	(*scrActions)["viewShowImages"]->setOn(true);
	(*scrActions)["viewShowGuides"]->setOn(true);
	(*scrActions)["viewRulerMode"]->setOn(true);

	connect( (*scrActions)["viewFitInWindow"], SIGNAL(activatedData(double)), ScMW, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit50"], SIGNAL(activatedData(double)), ScMW, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit75"], SIGNAL(activatedData(double)), ScMW, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit100"], SIGNAL(activatedData(double)), ScMW, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit200"], SIGNAL(activatedData(double)), ScMW, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit20"], SIGNAL(activatedData(double)), ScMW, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewShowMargins"], SIGNAL(activated()), ScMW, SLOT(ToggleMarks()) );
	connect( (*scrActions)["viewShowFrames"], SIGNAL(activated()), ScMW, SLOT(ToggleFrames()) );
	connect( (*scrActions)["viewShowImages"], SIGNAL(activated()), ScMW, SLOT(TogglePics()) );
	connect( (*scrActions)["viewShowGrid"], SIGNAL(activated()), ScMW, SLOT(ToggleRaster()) );
	connect( (*scrActions)["viewShowGuides"], SIGNAL(activated()), ScMW, SLOT(ToggleGuides()) );
	connect( (*scrActions)["viewShowBaseline"], SIGNAL(activated()), ScMW, SLOT(ToggleBase()) );
	connect( (*scrActions)["viewShowTextChain"], SIGNAL(activated()), ScMW, SLOT(ToggleTextLinks()) );
	connect( (*scrActions)["viewShowTextControls"], SIGNAL(activated()), ScMW, SLOT(ToggleTextControls()) );
	connect( (*scrActions)["viewRulerMode"], SIGNAL(activated()), ScMW, SLOT(ToggleRuler()) );
	connect( (*scrActions)["viewSnapToGrid"], SIGNAL(activated()), ScMW, SLOT(ToggleURaster()) );
	connect( (*scrActions)["viewSnapToGuides"], SIGNAL(activated()), ScMW, SLOT(ToggleUGuides()) );
//	connect( (*scrActions)["viewNewView"], SIGNAL(activated()), ScMW, SLOT(newView()) );

}

void ActionManager::initToolsMenuActions()
{
	//Tool menu
	scrActions->insert("toolsProperties", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsProperties"));
	scrActions->insert("toolsOutline", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsOutline"));
	scrActions->insert("toolsScrapbook", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsScrapbook"));
	scrActions->insert("toolsLayers", new ScrAction(QIconSet(noIcon),"", QKeySequence(Key_F6), ScMW, "toolsLayers"));
	scrActions->insert("toolsPages", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsPages"));
	scrActions->insert("toolsBookmarks", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsBookmarks"));
	scrActions->insert("toolsMeasurements", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("dist.png"), loadIcon("dist22.png")), "", QKeySequence(), ScMW, "toolsMeasurements", modeMeasurementTool));
	scrActions->insert("toolsActionHistory", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsActionHistory"));
	scrActions->insert("toolsPreflightVerifier", new ScrAction(QIconSet(loadIcon("launch16.png"), loadIcon("launch.png")),"", QKeySequence(), ScMW, "toolsPreflightVerifier"));
	scrActions->insert("toolsAlignDistribute", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsAlignDistribute"));
	scrActions->insert("toolsToolbarTools", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsToolbarTools"));
	scrActions->insert("toolsToolbarPDF", new ScrAction(QIconSet(noIcon),"", QKeySequence(), ScMW, "toolsToolbarPDF"));

	//toolbar only items
	scrActions->insert("toolsSelect", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Kreuz.xpm"), loadIcon("Kreuz.xpm")), "", QKeySequence(Key_C), ScMW, "toolsSelect", modeNormal));
	scrActions->insert("toolsInsertTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Text16.xpm"), loadIcon("Text.xpm")), "", QKeySequence(Key_T), ScMW, "toolsInsertTextFrame", modeDrawText));
	scrActions->insert("toolsInsertImageFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Bild16.xpm"), loadIcon("Bild.xpm")), "", QKeySequence(Key_I), ScMW, "toolsInsertImageFrame", modeDrawPicture));
	scrActions->insert("toolsInsertTableFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("frame_table16.png"), loadIcon("frame_table.png")), "", QKeySequence(Key_A), ScMW, "toolsInsertTableFrame", modeDrawTable));
	scrActions->insert("toolsInsertShape", new ScrAction(ScrAction::DataInt,QIconSet(), "", QKeySequence(Key_S), ScMW, "toolsInsertShape", modeDrawShapes));
	scrActions->insert("toolsInsertPolygon", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("spline16.png"), loadIcon("spline.png")), "", QKeySequence(Key_P), ScMW, "toolsInsertPolygon", modeDrawRegularPolygon));
	scrActions->insert("toolsInsertLine", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Stift16.xpm"), loadIcon("Stift.xpm")), "", QKeySequence(Key_L), ScMW, "toolsInsertLine", modeDrawLine));
	scrActions->insert("toolsInsertBezier", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("beziertool16.png"), loadIcon("beziertool.png")), "", QKeySequence(Key_B), ScMW, "toolsInsertBezier", modeDrawBezierLine));
	scrActions->insert("toolsInsertFreehandLine", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Stiftalt16.xpm"), loadIcon("Stiftalt.xpm")), "", QKeySequence(Key_F), ScMW, "toolsInsertFreehandLine", modeDrawFreehandLine));

	scrActions->insert("toolsRotate", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Rotieren.xpm"), loadIcon("Rotieren.xpm")), "", QKeySequence(Key_R), ScMW, "toolsRotate", modeRotation));
	scrActions->insert("toolsZoom", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Lupe.xpm"), loadIcon("Lupe.xpm")), "", QKeySequence(Key_Z), ScMW, "toolsZoom", modeMagnifier));
	scrActions->insert("toolsZoomIn", new ScrAction(QIconSet(loadIcon("viewmagin.png"), loadIcon("viewmagin.png")), "", QKeySequence(CTRL+Key_Plus), ScMW, "toolsZoomIn"));
	scrActions->insert("toolsZoomOut", new ScrAction(QIconSet(loadIcon("viewmagout.png"), loadIcon("viewmagout.png")), "", QKeySequence(CTRL+Key_Minus), ScMW, "toolsZoomOut"));
	scrActions->insert("toolsEditContents", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Editm.xpm"), loadIcon("Editm.xpm")), "", QKeySequence(Key_E), ScMW, "toolsEditContents", modeEdit));
	scrActions->insert("toolsEditWithStoryEditor", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("signature2.png"), loadIcon("signature.png")), "", QKeySequence(CTRL+Key_Y), ScMW, "toolsEditWithStoryEditor", modeStoryEditor));
	scrActions->insert("toolsLinkTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Lock.xpm"), loadIcon("Lock.xpm")), "", QKeySequence(Key_N), ScMW, "toolsLinkTextFrame", modeLinkFrames));
	scrActions->insert("toolsUnlinkTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Unlock.xpm"), loadIcon("Unlock.xpm")), "", QKeySequence(Key_U), ScMW, "toolsUnlinkTextFrame", modeUnlinkFrames));
	scrActions->insert("toolsEyeDropper", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("colorpicker.png"), loadIcon("colorpicker.png")), "", QKeySequence(Key_Y), ScMW, "toolsEyeDropper", modeEyeDropper));
	scrActions->insert("toolsCopyProperties", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("wizard.png"), loadIcon("wizard.png")), "", QKeySequence(), ScMW, "toolsCopyProperties", modeCopyProperties));

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

	*modeActionNames << "toolsSelect" << "toolsInsertTextFrame" << "toolsInsertImageFrame" << "toolsInsertTableFrame";
	*modeActionNames << "toolsInsertShape" << "toolsInsertPolygon" << "toolsInsertLine" << "toolsInsertBezier";
	*modeActionNames << "toolsInsertFreehandLine" << "toolsRotate" << "toolsZoom" << "toolsEditContents";
	*modeActionNames << "toolsEditWithStoryEditor" << "toolsLinkTextFrame" << "toolsUnlinkTextFrame";
	*modeActionNames << "toolsEyeDropper" << "toolsCopyProperties";

	*nonEditActionNames << "itemLowerToBottom" << "itemRaiseToTop" << "itemRaise" << "itemLower";

	connect( (*scrActions)["toolsActionHistory"], SIGNAL(toggled(bool)), ScMW, SLOT(setUndoPalette(bool)) );

	connectModeActions();
}

void ActionManager::initExtrasMenuActions()
{
	scrActions->insert("extrasManagePictures", new ScrAction(ScMW, "extrasManagePictures"));
	scrActions->insert("extrasHyphenateText", new ScrAction(ScMW, "extrasHyphenateText"));
	scrActions->insert("extrasDeHyphenateText", new ScrAction(ScMW, "extrasDeHyphenateText"));
	scrActions->insert("extrasGenerateTableOfContents", new ScrAction(ScMW, "extrasGenerateTableOfContents"));

	connect( (*scrActions)["extrasManagePictures"], SIGNAL(activated()), ScMW, SLOT(StatusPic()) );
	connect( (*scrActions)["extrasHyphenateText"], SIGNAL(activated()), ScMW, SLOT(doHyphenate()) );
	connect( (*scrActions)["extrasDeHyphenateText"], SIGNAL(activated()), ScMW, SLOT(doDeHyphenate()) );
	connect( (*scrActions)["extrasGenerateTableOfContents"], SIGNAL(activated()), ScMW, SLOT(generateTableOfContents()) );
}


void ActionManager::initWindowsMenuActions()
{
	scrActions->insert("windowsCascade", new ScrAction( QIconSet(noIcon),"", QKeySequence(), ScMW, "windowsCascade"));
	scrActions->insert("windowsTile", new ScrAction( QIconSet(noIcon),"", QKeySequence(), ScMW, "windowstile"));
}

void ActionManager::initScriptMenuActions()
{
}

void ActionManager::initHelpMenuActions()
{
	scrActions->insert("helpAboutScribus", new ScrAction(ScMW, "helpAboutScribus"));
	scrActions->insert("helpAboutPlugins", new ScrAction(ScMW, "helpAboutPlugins"));
	scrActions->insert("helpAboutQt", new ScrAction(ScMW, "helpAboutQt"));
	scrActions->insert("helpTooltips", new ScrAction(ScMW, "helpTooltips"));
	scrActions->insert("helpManual", new ScrAction("", Key_F1, ScMW, "helpManual"));

	(*scrActions)["helpTooltips"]->setToggleAction(true);
	(*scrActions)["helpTooltips"]->setOn(true);

	connect( (*scrActions)["helpAboutScribus"], SIGNAL(activated()), ScMW, SLOT(slotHelpAbout()) );
	connect( (*scrActions)["helpAboutPlugins"], SIGNAL(activated()), ScMW, SLOT(slotHelpAboutPlugins()) );
	connect( (*scrActions)["helpAboutQt"], SIGNAL(activated()), ScMW, SLOT(slotHelpAboutQt()) );
	connect( (*scrActions)["helpTooltips"], SIGNAL(activated()), ScMW, SLOT(ToggleTips()) );
	connect( (*scrActions)["helpManual"], SIGNAL(activated()), ScMW, SLOT(slotOnlineHelp()) );
}

void ActionManager::initUnicodeActions(QMap<QString, QGuardedPtr<ScrAction> > *actionMap, QWidget *actionParent, QStringList *actionNamesList)
{
	//typography
	actionMap->insert("unicodeSmartHyphen", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+SHIFT+Key_Minus, actionParent, "unicodeSmartHyphen",-1));
	actionMap->insert("unicodeNonBreakingHyphen", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+ALT+Key_Minus, actionParent, "unicodeNonBreakingHyphen",24));
	actionMap->insert("unicodeNonBreakingSpace", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+Key_Space, actionParent, "unicodeNonBreakingSpace",29));
	actionMap->insert("unicodePageNumber", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+SHIFT+ALT+Key_P, actionParent, "unicodePageNumber",30));
	//Breaks
	actionMap->insert("unicodeNewLine", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", SHIFT+Key_Return, actionParent, "unicodeNewLine",28));
	actionMap->insert("unicodeFrameBreak", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+Key_Return, actionParent, "unicodeFrameBreak",27));
	actionMap->insert("unicodeColumnBreak", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+SHIFT+Key_Return, actionParent, "unicodeColumnBreak",26));
	//Special
	actionMap->insert("unicodeCopyRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeCopyRight",0x0A9));
	actionMap->insert("unicodeRegdTM", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeRegdTM",0x00AE));
	actionMap->insert("unicodeTM", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeTM",0x2122));
	actionMap->insert("unicodeBullet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeBullet",0x2022));
	//Dashes
	actionMap->insert("unicodeDashEm", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeDashEm",0x2014));
	actionMap->insert("unicodeDashEn", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeDashEn",0x2013));
	actionMap->insert("unicodeDashFigure", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeDashFigure",0x2012));
	actionMap->insert("unicodeDashQuotation", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeDashQuotation",0x2015));
	//Quotes
	actionMap->insert("unicodeQuoteApostrophe", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteApostrophe",0x0027));
	actionMap->insert("unicodeQuoteStraight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteStraight",0x0022));
	actionMap->insert("unicodeQuoteSingleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteSingleLeft",0x2018));
	actionMap->insert("unicodeQuoteSingleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteSingleRight",0x2019));
	actionMap->insert("unicodeQuoteDoubleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteDoubleLeft",0x201C));
	actionMap->insert("unicodeQuoteDoubleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteDoubleRight",0x201D));
	actionMap->insert("unicodeQuoteSingleReversed", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteSingleReversed",0x201B));
	actionMap->insert("unicodeQuoteDoubleReversed", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteDoubleReversed",0x201F));
	actionMap->insert("unicodeQuoteSingleLeftGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteSingleLeftGuillemet",0x2039));
	actionMap->insert("unicodeQuoteSingleRightGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteSingleRightGuillemet",0x203A));
	actionMap->insert("unicodeQuoteDoubleLeftGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteDoubleLeftGuillemet",0x00AB));
	actionMap->insert("unicodeQuoteDoubleRightGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteDoubleRightGuillemet",0x00BB));
	actionMap->insert("unicodeQuoteLowSingleComma", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteLowSingleComma",0x201A));
	actionMap->insert("unicodeQuoteLowDoubleComma", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteLowDoubleComma",0x201E));
	actionMap->insert("unicodeQuoteCJKSingleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteCJKSingleLeft",0x300C));
	actionMap->insert("unicodeQuoteCJKSingleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteCJKSingleRight",0x300D));
	actionMap->insert("unicodeQuoteCJKDoubleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteCJKDoubleLeft",0x300E));
	actionMap->insert("unicodeQuoteCJKDoubleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeQuoteCJKDoubleRight",0x300F));
	//Ligatures
	actionMap->insert("unicodeLigature_ff", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeLigature_ff",0xFB00));
	actionMap->insert("unicodeLigature_fi", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeLigature_fi",0xFB01));
	actionMap->insert("unicodeLigature_fl", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeLigature_fl",0xFB02));
	actionMap->insert("unicodeLigature_ffi", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeLigature_ffi",0xFB03));
	actionMap->insert("unicodeLigature_ffl", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeLigature_ffl",0xFB04));
	actionMap->insert("unicodeLigature_ft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeLigature_ft",0xFB05));
	actionMap->insert("unicodeLigature_st", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), actionParent, "unicodeLigature_st",0xFB06));
	
	//Spaces and special characters
	*actionNamesList << "unicodeSmartHyphen" << "unicodeNonBreakingHyphen" << "unicodeNonBreakingSpace" << "unicodePageNumber";
	//Breaks
	*actionNamesList << "unicodeNewLine" << "unicodeFrameBreak" << "unicodeColumnBreak";
	//Copyrights and TMs
	*actionNamesList << "unicodeCopyRight" << "unicodeRegdTM" << "unicodeTM";
	//Bullets
	*actionNamesList << "unicodeBullet";
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
		connect( (*actionMap)[*it], SIGNAL(activatedUnicodeShortcut(QString, int)), actionParent, SLOT(specialActionKeyEvent(QString, int)) );
}

void ActionManager::initSpecialActions()
{
	//GUI
	scrActions->insert("specialToggleAllPalettes", new ScrAction(ScrAction::DataQString, QIconSet(), "", Key_F10, ScMW, "specialToggleAllPalettes",0,0.0,"specialToggleAllPalettes"));
	scrActions->insert("specialToggleAllGuides", new ScrAction(ScrAction::DataQString, QIconSet(), "", Key_F11, ScMW, "specialToggleAllGuides",0,0.0,"specialToggleAllGuides"));

	connect( (*scrActions)["specialToggleAllPalettes"], SIGNAL(activated()), ScMW, SLOT(ToggleAllPalettes()) );
	connect( (*scrActions)["specialToggleAllGuides"], SIGNAL(activated()), ScMW, SLOT(ToggleAllGuides()) );
}

void ActionManager::disconnectModeActions()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
		disconnect( (*scrActions)[*it], SIGNAL(toggledData(bool, int)) , ScMW, SLOT(setAppModeByToggle(bool, int)) );
}

void ActionManager::connectModeActions()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
		connect( (*scrActions)[*it], SIGNAL(toggledData(bool, int)) , ScMW, SLOT(setAppModeByToggle(bool, int)) );
}

void ActionManager::disconnectNewViewActions()
{
	disconnect( (*scrActions)["toolsZoomIn"], 0, 0, 0);
	disconnect( (*scrActions)["toolsZoomOut"], 0, 0, 0);
	disconnect( (*scrActions)["itemLowerToBottom"], 0, 0, 0);
	disconnect( (*scrActions)["itemImageIsVisible"], 0, 0, 0);
	disconnect( (*scrActions)["itemPreviewLow"], SIGNAL(activatedData(int)), 0, 0 );
	disconnect( (*scrActions)["itemPreviewNormal"], SIGNAL(activatedData(int)), 0,0 );
	disconnect( (*scrActions)["itemPreviewFull"], SIGNAL(activatedData(int)), 0, 0 );
	disconnect( (*scrActions)["itemRaise"], 0, 0, 0);
	disconnect( (*scrActions)["itemLower"], 0, 0, 0);
	disconnect( (*scrActions)["itemCombinePolygons"], 0, 0, 0);
	disconnect( (*scrActions)["itemSplitPolygons"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToBezierCurve"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToImageFrame"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToOutlines"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToPolygon"], 0, 0, 0);
	disconnect( (*scrActions)["itemConvertToTextFrame"], 0, 0, 0);
	disconnect( (*scrActions)["itemAttachTextToPath"], 0, 0, 0);
	disconnect( (*scrActions)["itemDetachTextFromPath"], 0, 0, 0);
	disconnect( (*scrActions)["itemLock"], 0, 0, 0);
	disconnect( (*scrActions)["itemLockSize"], 0, 0, 0);
	disconnect( (*scrActions)["itemAdjustFrameToImage"], 0, 0, 0 );
	disconnect( (*scrActions)["itemUpdateImage"], 0, 0, 0 );
	disconnect( (*scrActions)["itemExtendedImageProperties"], 0, 0, 0 );
}

void ActionManager::connectNewViewActions(ScribusView *currView)
{
	if (currView==NULL)
		return;
	connect( (*scrActions)["toolsZoomIn"], SIGNAL(activated()) , currView, SLOT(slotZoomIn()) );
	connect( (*scrActions)["toolsZoomOut"], SIGNAL(activated()) , currView, SLOT(slotZoomOut()) );
	connect( (*scrActions)["itemLowerToBottom"], SIGNAL(activated()), currView, SLOT(ToBack()) );
	connect( (*scrActions)["itemRaiseToTop"], SIGNAL(activated()), currView, SLOT(ToFront()) );
	connect( (*scrActions)["itemRaise"], SIGNAL(activated()), currView, SLOT(RaiseItem()) );
	connect( (*scrActions)["itemLower"], SIGNAL(activated()), currView, SLOT(LowerItem()) );
	connect( (*scrActions)["itemCombinePolygons"], SIGNAL(activated()), currView, SLOT(UniteObj()) );
	connect( (*scrActions)["itemSplitPolygons"], SIGNAL(activated()), currView, SLOT(SplitObj()) );
	connect( (*scrActions)["itemConvertToBezierCurve"], SIGNAL(activated()), currView, SLOT(ToBezierFrame()) );
	connect( (*scrActions)["itemConvertToImageFrame"], SIGNAL(activated()), currView, SLOT(ToPicFrame()) );
	connect( (*scrActions)["itemConvertToOutlines"], SIGNAL(activated()), currView, SLOT(TextToPath()) );
	connect( (*scrActions)["itemConvertToPolygon"], SIGNAL(activated()), currView, SLOT(ToPolyFrame()) );
	connect( (*scrActions)["itemConvertToTextFrame"], SIGNAL(activated()), currView, SLOT(ToTextFrame()) );
	connect( (*scrActions)["itemAttachTextToPath"], SIGNAL(activated()), currView, SLOT(ToPathText()) );
	connect( (*scrActions)["itemDetachTextFromPath"], SIGNAL(activated()), currView, SLOT(FromPathText()) );
	connect( (*scrActions)["itemLock"], SIGNAL(activated()), currView, SLOT(ToggleLock()) );
	connect( (*scrActions)["itemLockSize"], SIGNAL(activated()), currView, SLOT(ToggleSizeLock()) );
	connect( (*scrActions)["itemAdjustFrameToImage"], SIGNAL(activated()), currView, SLOT(adjustFrametoImageSize()) );
	connect( (*scrActions)["itemUpdateImage"], SIGNAL(activated()), currView, SLOT(UpdatePic()) );
	connect( (*scrActions)["itemExtendedImageProperties"], SIGNAL(activated()), currView, SLOT(editExtendedImageProperties()) );
}

void ActionManager::disconnectNewSelectionActions()
{
	disconnect( (*scrActions)["itemImageIsVisible"], 0, 0, 0);
	disconnect( (*scrActions)["itemPreviewLow"], SIGNAL(activatedData(int)) , 0, 0 );
	disconnect( (*scrActions)["itemPreviewNormal"], SIGNAL(activatedData(int)) , 0, 0 );
	disconnect( (*scrActions)["itemPreviewFull"], SIGNAL(activatedData(int)) , 0, 0 );
}

void ActionManager::connectNewSelectionActions(ScribusView *currView)
{
	connect( (*scrActions)["itemImageIsVisible"], SIGNAL(toggled(bool)) , currView, SLOT(TogglePic()) );
	connect( (*scrActions)["itemPreviewLow"], SIGNAL(activatedData(int)) , currView, SLOT(changePreview(int)) );
	connect( (*scrActions)["itemPreviewNormal"], SIGNAL(activatedData(int)) , currView, SLOT(changePreview(int)) );
	connect( (*scrActions)["itemPreviewFull"], SIGNAL(activatedData(int)) , currView, SLOT(changePreview(int)) );
}

void ActionManager::saveActionShortcutsPreEditMode()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
		(*scrActions)[*it]->saveShortcut();
	for ( QStringList::Iterator it = nonEditActionNames->begin(); it != nonEditActionNames->end(); ++it )
		(*scrActions)[*it]->saveShortcut();

}

void ActionManager::restoreActionShortcutsPostEditMode()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
		(*scrActions)[*it]->restoreShortcut();
	for ( QStringList::Iterator it = nonEditActionNames->begin(); it != nonEditActionNames->end(); ++it )
		(*scrActions)[*it]->restoreShortcut();
}

void ActionManager::enableActionStringList(QStringList *list, bool enabled, bool checkingUnicode)
{
	for ( QStringList::Iterator it = list->begin(); it != list->end(); ++it )
	{
		if(!checkingUnicode)
			(*scrActions)[*it]->setEnabled(enabled);
		else
		{
			//For UnicodeChar actions, only enable when the current font has that character.
			if (ScMW->HaveDoc && (*scrActions)[*it]->actionType()==ScrAction::UnicodeChar)
			{
				int charCode=(*scrActions)[*it]->actionInt();
				if(charCode==-1 ||
				   charCode==24 ||
				   charCode==26 ||
				   charCode==27 ||
				   charCode==28 ||
				   charCode==29 ||
				   charCode==30 ||
				   (*ScMW->doc->AllFonts)[ScMW->doc->CurrFont]->CharWidth.contains(charCode) )
					(*scrActions)[*it]->setEnabled(enabled);
			}
		}
	}
}

void ActionManager::enableUnicodeActions(bool enabled)
{
	enableActionStringList(unicodeCharActionNames, enabled, enabled);
}

void ActionManager::setPDFActions(ScribusView *currView)
{
	if (currView==NULL)
		return;
	//PageItem* currItem = currView->SelItem.at(0);
	PageItem* currItem = ScMW->doc->selection->itemAt(0);
	if (currItem==NULL)
		return;

	disconnect( (*scrActions)["itemPDFIsBookmark"], 0, 0, 0);
	disconnect( (*scrActions)["itemPDFIsAnnotation"], 0, 0, 0);

	if (!currItem->asTextFrame())
	{
		(*scrActions)["itemPDFIsAnnotation"]->setEnabled(false);
		(*scrActions)["itemPDFIsBookmark"]->setEnabled(false);
		(*scrActions)["itemPDFIsAnnotation"]->setOn(false);
		(*scrActions)["itemPDFIsBookmark"]->setOn(false);
		(*scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
		(*scrActions)["itemPDFFieldProps"]->setEnabled(false);
		return;
	}

	(*scrActions)["itemPDFIsAnnotation"]->setEnabled(true);
	(*scrActions)["itemPDFIsBookmark"]->setEnabled(true);
	(*scrActions)["itemPDFIsAnnotation"]->setOn(currItem->isAnnotation());
	(*scrActions)["itemPDFIsBookmark"]->setOn(currItem->isBookmark);
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
	connect( (*scrActions)["itemPDFIsAnnotation"], SIGNAL(activated()), currView, SLOT(ToggleAnnotation()) );
	connect( (*scrActions)["itemPDFIsBookmark"], SIGNAL(activated()), currView, SLOT(ToggleBookmark()) );
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
	(*scrActions)["fileExportAsEPS"]->setTexts( tr("Save Page as &EPS..."));
	(*scrActions)["fileExportAsPDF"]->setTexts( tr("Save as P&DF..."));
	(*scrActions)["fileDocSetup"]->setTexts( tr("Document &Setup..."));
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
	(*scrActions)["editClearContents"]->setTexts( tr("C&lear Contents"));
	(*scrActions)["editSelectAll"]->setTexts( tr("Select &All"));
	(*scrActions)["editDeselectAll"]->setTexts( tr("&Deselect All"));
	(*scrActions)["editSearchReplace"]->setTexts( tr("&Search/Replace..."));
	(*scrActions)["editEditWithImageEditor"]->setTexts( tr("Edit Image..."));
	(*scrActions)["editColors"]->setTexts( tr("C&olors..."));
	(*scrActions)["editParaStyles"]->setTexts( tr("&Paragraph Styles..."));
	(*scrActions)["editLineStyles"]->setTexts( tr("&Line Styles..."));
	(*scrActions)["editMasterPages"]->setTexts( tr("&Master Pages..."));
	(*scrActions)["editJavascripts"]->setTexts( tr("&JavaScripts..."));
	(*scrActions)["editPreferences"]->setTexts( tr("P&references..."));

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
	(*scrActions)["typeEffectOutline"]->setTexts( tr("&Outline"));
	(*scrActions)["typeEffectShadow"]->setTexts( tr("S&hadow"));

	(*scrActions)["styleInvertPict"]->setTexts( tr("&Image Effects"));
	(*scrActions)["styleTabulators"]->setTexts( tr("&Tabulators..."));

	//Item Menu
	(*scrActions)["itemDuplicate"]->setTexts( tr("D&uplicate"));
	(*scrActions)["itemMulDuplicate"]->setTexts( tr("&Multiple Duplicate"));
	(*scrActions)["itemDelete"]->setTexts( tr("&Delete"));
	(*scrActions)["itemGroup"]->setTexts( tr("&Group"));
	(*scrActions)["itemUngroup"]->setTexts( tr("&Ungroup"));
	(*scrActions)["itemLock"]->setTexts( tr("Is &Locked"));
	(*scrActions)["itemLockSize"]->setTexts( tr("Si&ze is Locked"));
	(*scrActions)["itemLowerToBottom"]->setTexts( tr("Lower to &Bottom"));
	(*scrActions)["itemRaiseToTop"]->setTexts( tr("Raise to &Top"));
	(*scrActions)["itemLower"]->setTexts( tr("&Lower"));
	(*scrActions)["itemRaise"]->setTexts( tr("&Raise"));
	(*scrActions)["itemSendToScrapbook"]->setTexts( tr("Send to S&crapbook"));
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
	(*scrActions)["itemConvertToOutlines"]->setTexts( tr("&Outlines"));
	(*scrActions)["itemConvertToPolygon"]->setTexts( tr("&Polygon"));
	(*scrActions)["itemConvertToTextFrame"]->setTexts( tr("&Text Frame"));

	//Insert Menu
	(*scrActions)["insertGlyph"]->setTexts( tr("&Glyph..."));
	(*scrActions)["insertSampleText"]->setTexts( tr("Sample Text"));

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
	(*scrActions)["viewFitInWindow"]->setTexts( tr("&Fit in window"));
	(*scrActions)["viewFit50"]->setTexts( tr("&50%"));
	(*scrActions)["viewFit75"]->setTexts( tr("&75%"));
	(*scrActions)["viewFit100"]->setTexts( tr("&100%"));
	(*scrActions)["viewFit200"]->setTexts( tr("&200%"));
	(*scrActions)["viewFit20"]->setTexts( tr("&Thumbnails"));
	(*scrActions)["viewShowMargins"]->setTexts( tr("Show &Margins"));
	(*scrActions)["viewShowFrames"]->setTexts( tr("Show &Frames"));
	(*scrActions)["viewShowImages"]->setTexts( tr("Show &Images"));
	(*scrActions)["viewShowGrid"]->setTexts( tr("Show &Grid"));
	(*scrActions)["viewShowGuides"]->setTexts( tr("Show G&uides"));
	(*scrActions)["viewShowBaseline"]->setTexts( tr("Show &Baseline Grid"));
	(*scrActions)["viewShowTextChain"]->setTexts( tr("Show &Text Chain"));
	(*scrActions)["viewShowTextControls"]->setTexts( tr("Show Control Characters"));
	(*scrActions)["viewRulerMode"]->setTexts( tr("Rulers relative to Page"));
	(*scrActions)["viewSnapToGrid"]->setTexts( tr("Sn&ap to Grid"));
	(*scrActions)["viewSnapToGuides"]->setTexts( tr("Sna&p to Guides"));
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
	(*scrActions)["toolsInsertTextFrame"]->setMenuText( tr("&Text Frame"));
	(*scrActions)["toolsInsertImageFrame"]->setMenuText( tr("&Image Frame"));
	(*scrActions)["toolsInsertTableFrame"]->setMenuText( tr("T&able"));
	(*scrActions)["toolsInsertShape"]->setMenuText( tr("&Shape"));
	(*scrActions)["toolsInsertPolygon"]->setMenuText( tr("&Polygon"));
	(*scrActions)["toolsInsertLine"]->setMenuText( tr("&Line"));
	(*scrActions)["toolsInsertBezier"]->setMenuText( tr("&Bezier Curve"));
	(*scrActions)["toolsInsertFreehandLine"]->setMenuText( tr("&Freehand Line"));
	(*scrActions)["toolsRotate"]->setTexts( tr("Rotate Item"));
	(*scrActions)["toolsZoom"]->setTexts( tr("Zoom in or out"));
	(*scrActions)["toolsZoomIn"]->setTexts( tr("Zoom in"));
	(*scrActions)["toolsZoomOut"]->setTexts( tr("Zoom out"));
	(*scrActions)["toolsEditContents"]->setTexts( tr("Edit Contents of Frame"));
	(*scrActions)["toolsEditWithStoryEditor"]->setMenuText( tr("Edit Text..."));
	(*scrActions)["toolsLinkTextFrame"]->setTexts( tr("Link Text Frames"));
	(*scrActions)["toolsUnlinkTextFrame"]->setTexts( tr("Unlink Text Frames"));
	(*scrActions)["toolsEyeDropper"]->setTexts( tr("&Eye Dropper"));
	(*scrActions)["toolsCopyProperties"]->setTexts( tr("Copy Item Properties"));

	(*scrActions)["toolsEditWithStoryEditor"]->setText( tr("Edit the text with the Story Editor"));

	(*scrActions)["toolsInsertTextFrame"]->setText( tr("Insert Text Frame"));
	(*scrActions)["toolsInsertImageFrame"]->setText( tr("Insert Image Frame"));
	(*scrActions)["toolsInsertTableFrame"]->setText( tr("Insert Table"));
	(*scrActions)["toolsInsertShape"]->setText( tr("Insert Shape"));
	(*scrActions)["toolsInsertPolygon"]->setText( tr("Insert Polygon"));
	(*scrActions)["toolsInsertLine"]->setText( tr("Insert Line"));
	(*scrActions)["toolsInsertBezier"]->setText( tr("Insert Bezier Curve"));
	(*scrActions)["toolsInsertFreehandLine"]->setText( tr("Insert Freehand Line"));

	//Extras Menu
	(*scrActions)["extrasManagePictures"]->setTexts( tr("&Manage Pictures"));
	(*scrActions)["extrasHyphenateText"]->setTexts( tr("&Hyphenate Text"));
	(*scrActions)["extrasDeHyphenateText"]->setTexts( tr("Dehyphenate Text"));
	(*scrActions)["extrasGenerateTableOfContents"]->setTexts( tr("&Generate Table Of Contents"));

	//Windows Menu
	(*scrActions)["windowsCascade"]->setMenuText( tr("&Cascade"));
	(*scrActions)["windowsTile"]->setMenuText( tr("&Tile"));

	//Help Menu
	(*scrActions)["helpAboutScribus"]->setTexts( tr("&About Scribus"));
	(*scrActions)["helpAboutPlugins"]->setTexts( tr("&About Plug-ins"));
	(*scrActions)["helpAboutQt"]->setTexts( tr("About &Qt"));
	(*scrActions)["helpTooltips"]->setTexts( tr("Toolti&ps"));
	(*scrActions)["helpManual"]->setTexts( tr("Scribus &Manual..."));

	//GUI
	(*scrActions)["specialToggleAllPalettes"]->setTexts( tr("Toggle Palettes"));
	(*scrActions)["specialToggleAllGuides"]->setTexts( tr("Toggle Guides"));
	
	languageChangeUnicodeActions(scrActions);
}

void ActionManager::languageChangeUnicodeActions(QMap<QString, QGuardedPtr<ScrAction> > *actionMap)
{
	//typography
	(*actionMap)["unicodeSmartHyphen"]->setMenuText( tr("Smart &Hyphen"));
	(*actionMap)["unicodeNonBreakingHyphen"]->setMenuText( tr("Non Breaking Dash"));
	(*actionMap)["unicodeNonBreakingSpace"]->setMenuText( tr("Non Breaking &Space"));
	(*actionMap)["unicodePageNumber"]->setMenuText( tr("Page &Number"));
	(*actionMap)["unicodeNewLine"]->setMenuText( tr("New Line"));
	(*actionMap)["unicodeFrameBreak"]->setMenuText( tr("Frame Break"));
	(*actionMap)["unicodeColumnBreak"]->setMenuText( tr("Column Break"));
	(*actionMap)["unicodeCopyRight"]->setTexts( tr("Copyright"));
	(*actionMap)["unicodeRegdTM"]->setTexts( tr("Registered Trademark"));
	(*actionMap)["unicodeTM"]->setTexts( tr("Trademark"));
	(*actionMap)["unicodeBullet"]->setTexts( tr("Bullet"));
	(*actionMap)["unicodeDashEm"]->setTexts( tr("Em Dash"));
	(*actionMap)["unicodeDashEn"]->setTexts( tr("En Dash"));
	(*actionMap)["unicodeDashFigure"]->setTexts( tr("Figure Dash"));
	(*actionMap)["unicodeDashQuotation"]->setTexts( tr("Quotation Dash"));

	(*actionMap)["unicodeQuoteApostrophe"]->setTexts( tr("Apostrophe"));
	(*actionMap)["unicodeQuoteStraight"]->setTexts( tr("Straight Double"));
	(*actionMap)["unicodeQuoteSingleLeft"]->setTexts( tr("Single Left"));
	(*actionMap)["unicodeQuoteSingleRight"]->setTexts( tr("Single Right"));
	(*actionMap)["unicodeQuoteDoubleLeft"]->setTexts( tr("Double Left"));
	(*actionMap)["unicodeQuoteDoubleRight"]->setTexts( tr("Double Right"));
	(*actionMap)["unicodeQuoteSingleReversed"]->setTexts( tr("Single Reversed"));
	(*actionMap)["unicodeQuoteDoubleReversed"]->setTexts( tr("Double Reversed"));
	(*actionMap)["unicodeQuoteSingleLeftGuillemet"]->setTexts( tr("Single Left Guillemet"));
	(*actionMap)["unicodeQuoteSingleRightGuillemet"]->setTexts( tr("Single Right Guillemet"));
	(*actionMap)["unicodeQuoteDoubleLeftGuillemet"]->setTexts( tr("Double Left Guillemet"));
	(*actionMap)["unicodeQuoteDoubleRightGuillemet"]->setTexts( tr("Double Right Guillemet"));
	(*actionMap)["unicodeQuoteLowSingleComma"]->setTexts( tr("Low Single Comma"));
	(*actionMap)["unicodeQuoteLowDoubleComma"]->setTexts( tr("Low Double Comma"));
	(*actionMap)["unicodeQuoteCJKSingleLeft"]->setTexts( tr("CJK Single Left"));
	(*actionMap)["unicodeQuoteCJKSingleRight"]->setTexts( tr("CJK Single Right"));
	(*actionMap)["unicodeQuoteCJKDoubleLeft"]->setTexts( tr("CJK Double Left"));
	(*actionMap)["unicodeQuoteCJKDoubleRight"]->setTexts( tr("CJK Double Right"));

	(*actionMap)["unicodeSmartHyphen"]->setText( tr("Insert Smart Hyphen"));
	(*actionMap)["unicodeNonBreakingHyphen"]->setText( tr("Insert Non Breaking Dash"));
	(*actionMap)["unicodeNonBreakingSpace"]->setText( tr("Insert Non Breaking Space"));
	(*actionMap)["unicodePageNumber"]->setText( tr("Insert Page Number"));
	(*actionMap)["unicodeNewLine"]->setText( tr("New Line"));
	(*actionMap)["unicodeFrameBreak"]->setText( tr("Frame Break"));
	(*actionMap)["unicodeColumnBreak"]->setText( tr("Column Break"));
	
	(*actionMap)["unicodeLigature_ff"]->setTexts( tr("ff"));
	(*actionMap)["unicodeLigature_fi"]->setTexts( tr("fi"));
	(*actionMap)["unicodeLigature_fl"]->setTexts( tr("fl"));
	(*actionMap)["unicodeLigature_ffi"]->setTexts( tr("ffi"));
	(*actionMap)["unicodeLigature_ffl"]->setTexts( tr("ffl"));
	(*actionMap)["unicodeLigature_ft"]->setTexts( tr("ft"));
	(*actionMap)["unicodeLigature_st"]->setTexts( tr("st"));
}

