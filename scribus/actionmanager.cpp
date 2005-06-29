/***************************************************************************
	begin                : Apr 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScApp program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "actionmanager.h"
#include "actionmanager.moc"

#include "scribus.h"
#include "scribusview.h"
#include "undomanager.h"

ActionManager::ActionManager ( QObject * parent, const char * name ) : QObject ( parent, name )
{
	ScApp=(ScribusApp *)parent;
	scrActions=&(ScApp->scrActions);
	scrActionGroups=&(ScApp->scrActionGroups);
	modeActionNames=new QStringList();
	nonEditActionNames=new QStringList();
	unicodeCharActionNames=new QStringList();
	undoManager = UndoManager::instance();

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
	initSpecialActions();
}

void ActionManager::initFileMenuActions()
{
	//File Menu
	scrActions->insert("fileNew", new ScrAction(QIconSet(loadIcon("DateiNeu16.png"), loadIcon("DateiNeu.xpm")), "", CTRL+Key_N, ScApp, "fileNew"));
	scrActions->insert("fileOpen", new ScrAction(QIconSet(loadIcon("DateiOpen16.png"), loadIcon("DateiOpen.xpm")), "", CTRL+Key_O, ScApp, "fileOpen"));
	scrActions->insert("fileClose", new ScrAction(QIconSet(loadIcon("DateiClos16.png"), loadIcon("DateiClose.png")), "", CTRL+Key_W, ScApp, "fileClose"));
	scrActions->insert("fileSave", new ScrAction(QIconSet(loadIcon("DateiSave16.png"), loadIcon("DateiSave2.png")), "", CTRL+Key_S, ScApp, "fileSave"));
	scrActions->insert("fileSaveAs", new ScrAction(QPixmap(loadIcon("filesaveas.png")), "", CTRL+SHIFT+Key_S, ScApp, "fileSaveAs"));
	scrActions->insert("fileRevert", new ScrAction(QPixmap(loadIcon("revert.png")), "", QKeySequence(), ScApp, "fileRevert"));
	scrActions->insert("fileCollect", new ScrAction(ScApp, "fileCollect"));
	//File Import Menu
	scrActions->insert("fileImportText", new ScrAction(ScApp, "fileImportText"));
	scrActions->insert("fileImportAppendText", new ScrAction(ScApp, "fileImportAppendText"));
	scrActions->insert("fileImportImage", new ScrAction(ScApp, "fileImportImage"));

	//File Export Menu
	scrActions->insert("fileExportText", new ScrAction(ScApp, "fileExportText"));
	scrActions->insert("fileExportAsEPS", new ScrAction(ScApp, "fileExportAsEPS"));
	scrActions->insert("fileExportAsPDF", new ScrAction(loadIcon("acrobat.png"), "", QKeySequence(), ScApp, "fileExportAsPDF"));
	//Rest of File Menu
	scrActions->insert("fileDocSetup", new ScrAction(QPixmap(loadIcon("documentinfo.png")), "", 0, ScApp, "fileDocSetup"));
	scrActions->insert("filePrint", new ScrAction(QIconSet(loadIcon("DateiPrint16.png"), loadIcon("DateiPrint.xpm")), "", CTRL+Key_P, ScApp, "filePrint"));
	scrActions->insert("fileQuit", new ScrAction(QPixmap(loadIcon("exit.png")), "", CTRL+Key_Q, ScApp, "fileQuit"));

	//Connect our signals and slots
	//File Menu
	connect( (*scrActions)["fileNew"], SIGNAL(activated()), ScApp, SLOT(slotFileNew()) );
	connect( (*scrActions)["fileOpen"], SIGNAL(activated()), ScApp, SLOT(slotDocOpen()) );
	connect( (*scrActions)["fileClose"], SIGNAL(activated()), ScApp, SLOT(slotFileClose()) );
	connect( (*scrActions)["filePrint"], SIGNAL(activated()), ScApp, SLOT(slotFilePrint()) );
	connect( (*scrActions)["fileSave"], SIGNAL(activated()), ScApp, SLOT(slotFileSave()) );
	connect( (*scrActions)["fileSaveAs"], SIGNAL(activated()), ScApp, SLOT(slotFileSaveAs()) );
	connect( (*scrActions)["fileDocSetup"], SIGNAL(activated()), ScApp, SLOT(SetupDoc()) );
	connect( (*scrActions)["fileRevert"], SIGNAL(activated()), ScApp, SLOT(slotFileRevert()) );
	connect( (*scrActions)["fileCollect"], SIGNAL(activated()), ScApp, SLOT(Collect()) );
	connect( (*scrActions)["fileQuit"], SIGNAL(activated()), ScApp, SLOT(slotFileQuit()) );
	//File Import Menu
	connect( (*scrActions)["fileImportText"], SIGNAL(activated()), ScApp, SLOT(slotFileOpen()) );
	connect( (*scrActions)["fileImportAppendText"], SIGNAL(activated()), ScApp, SLOT(slotFileAppend()) );
	connect( (*scrActions)["fileImportImage"], SIGNAL(activated()), ScApp, SLOT(slotFileOpen()) );
	//File Export Menu
	connect( (*scrActions)["fileExportText"], SIGNAL(activated()), ScApp, SLOT(SaveText()) );
	connect( (*scrActions)["fileExportAsEPS"], SIGNAL(activated()), ScApp, SLOT(SaveAsEps()) );
	connect( (*scrActions)["fileExportAsPDF"], SIGNAL(activated()), ScApp, SLOT(SaveAsPDF()) );
	//The rest are plugins
}

void ActionManager::initEditMenuActions()
{
	//Edit Menu
	scrActions->insert("editUndoAction", new ScrAction(ScrAction::DataInt, QIconSet(loadIcon("u_undo16.png"), loadIcon("u_undo.png")), "", CTRL+Key_Z, ScApp, "editUndoAction",1));
	scrActions->insert("editRedoAction", new ScrAction(ScrAction::DataInt, QIconSet(loadIcon("u_redo16.png"), loadIcon("u_redo.png")), "", CTRL+SHIFT+Key_Z, ScApp, "editRedoAction", 1));
	scrActions->insert("editActionMode", new ScrAction(ScApp, "editActionMode"));
	(*scrActions)["editActionMode"]->setToggleAction(true);
	scrActions->insert("editCut", new ScrAction(QIconSet(loadIcon("editcut.png"), loadIcon("editcut.png")), "", CTRL+Key_X, ScApp, "editCut"));
	scrActions->insert("editCopy", new ScrAction(QIconSet(loadIcon("editcopy.png"), loadIcon("editcopy.png")), "", CTRL+Key_C, ScApp, "editCopy"));
	scrActions->insert("editPaste", new ScrAction(QIconSet(loadIcon("editpaste.png"), loadIcon("editpaste.png")), "", CTRL+Key_V, ScApp, "editPaste"));
	scrActions->insert("editClearContents", new ScrAction(QIconSet(loadIcon("editdelete.png"), loadIcon("editdelete.png")), "", QKeySequence(), ScApp, "editClearContents"));
	scrActions->insert("editSelectAll", new ScrAction("", CTRL+Key_A, ScApp, "editSelectAll"));
	scrActions->insert("editDeselectAll", new ScrAction("", CTRL+SHIFT+Key_A, ScApp, "editDeselectAll"));
	scrActions->insert("editSearchReplace", new ScrAction(QIconSet(loadIcon("find16.png"), loadIcon("find16.png")), "", QKeySequence(CTRL+Key_F), ScApp, "editSearchReplace"));

	scrActions->insert("editEditWithImageEditor", new ScrAction(ScApp, "editEditWithImageEditor"));

	scrActions->insert("editColors", new ScrAction(ScApp, "editColors"));
	scrActions->insert("editParaStyles", new ScrAction(ScApp, "editParaStyles"));
	scrActions->insert("editLineStyles", new ScrAction(ScApp, "editLineStyles"));
	scrActions->insert("editMasterPages", new ScrAction(ScApp, "editMasterPages"));
	scrActions->insert("editJavascripts", new ScrAction(ScApp, "editJavascripts"));
	scrActions->insert("editPreferences", new ScrAction(ScApp, "editPreferences"));

	connect( (*scrActions)["editUndoAction"], SIGNAL(activatedData(int)) , undoManager, SLOT(undo(int)) );
	connect( (*scrActions)["editRedoAction"], SIGNAL(activatedData(int)) , undoManager, SLOT(redo(int)) );
	connect( (*scrActions)["editActionMode"], SIGNAL(toggled(bool)), ScApp, SLOT(setUndoMode(bool)) );
	connect( (*scrActions)["editCut"], SIGNAL(activated()), ScApp, SLOT(slotEditCut()) );
	connect( (*scrActions)["editCopy"], SIGNAL(activated()), ScApp, SLOT(slotEditCopy()) );
	connect( (*scrActions)["editPaste"], SIGNAL(activated()), ScApp, SLOT(slotEditPaste()) );
	connect( (*scrActions)["editClearContents"], SIGNAL(activated()), ScApp, SLOT(clearContents()) );
	connect( (*scrActions)["editSelectAll"], SIGNAL(activated()), ScApp, SLOT(SelectAll()) );
	connect( (*scrActions)["editDeselectAll"], SIGNAL(activated()), ScApp, SLOT(deselectAll()) );
	connect( (*scrActions)["editSearchReplace"], SIGNAL(activated()), ScApp, SLOT(SearchText()) );
	connect( (*scrActions)["editEditWithImageEditor"], SIGNAL(activated()), ScApp, SLOT(callImageEditor()) );
	connect( (*scrActions)["editColors"], SIGNAL(activated()), ScApp, SLOT(slotEditColors()) );
	connect( (*scrActions)["editParaStyles"], SIGNAL(activated()), ScApp, SLOT(slotEditStyles()) );
	connect( (*scrActions)["editLineStyles"], SIGNAL(activated()), ScApp, SLOT(slotEditLineStyles()) );
	connect( (*scrActions)["editMasterPages"], SIGNAL(activated()), ScApp, SLOT(manageMasterPages()) );
	connect( (*scrActions)["editJavascripts"], SIGNAL(activated()), ScApp, SLOT(ManageJava()) );
	connect( (*scrActions)["editPreferences"], SIGNAL(activated()), ScApp, SLOT(slotPrefsOrg()) );
}

void ActionManager::initStyleMenuActions()
{
	//Text Size actions
	(*scrActionGroups).insert("fontSize", new QActionGroup(ScApp, "fontSize", true));
	scrActions->insert("fontSizeOther", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["fontSize"], "fontSizeOther", -1));
	connect( (*scrActions)["fontSizeOther"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemFSize(int)));

	int font_sizes[] = {7, 9, 10, 11, 12, 14, 18, 24, 36, 48, 60, 72};
	size_t f_size = sizeof(font_sizes) / sizeof(*font_sizes);
	for (uint s = 0; s < f_size; ++s)
	{
		QString fontSizeName=QString("fontSize%1").arg(font_sizes[s]);
		scrActions->insert(fontSizeName, new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["fontSize"], fontSizeName, font_sizes[s]));
		(*scrActions)[fontSizeName]->setToggleAction(true);
		connect( (*scrActions)[fontSizeName], SIGNAL(activatedData(int)), ScApp, SLOT(setItemFSize(int)));
	}

	//Alignment actions
	scrActions->insert("alignLeft", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScApp, "alignLeft", 0));
	scrActions->insert("alignCenter", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScApp, "alignCenter", 1));
	scrActions->insert("alignRight", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScApp, "alignRight", 2));
	scrActions->insert("alignBlock", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScApp, "alignBlock", 3));
	scrActions->insert("alignForced", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScApp, "alignForced", 4));

	(*scrActions)["alignLeft"]->setToggleAction(true);
	(*scrActions)["alignCenter"]->setToggleAction(true);
	(*scrActions)["alignRight"]->setToggleAction(true);
	(*scrActions)["alignBlock"]->setToggleAction(true);
	(*scrActions)["alignForced"]->setToggleAction(true);

	connect( (*scrActions)["alignLeft"], SIGNAL(activatedData(int)), ScApp, SLOT(setNewAbStyle(int)));
	connect( (*scrActions)["alignCenter"], SIGNAL(activatedData(int)), ScApp, SLOT(setNewAbStyle(int)));
	connect( (*scrActions)["alignRight"], SIGNAL(activatedData(int)), ScApp, SLOT(setNewAbStyle(int)));
	connect( (*scrActions)["alignBlock"], SIGNAL(activatedData(int)), ScApp, SLOT(setNewAbStyle(int)));
	connect( (*scrActions)["alignForced"], SIGNAL(activatedData(int)), ScApp, SLOT(setNewAbStyle(int)));

	//Shade actions
	scrActionGroups->insert("shade", new QActionGroup(ScApp, "shade", true));
	scrActions->insert("shadeOther", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["shade"], "shadeOther", -1));
	connect( (*scrActions)["shadeOther"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemShade(int)));
	for (uint i=0; i<=100 ; i+=10)
	{
		QString shadeName=QString("shade%1").arg(i);
		scrActions->insert(shadeName, new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), (*scrActionGroups)["shade"], shadeName, i));
		(*scrActions)[shadeName]->setToggleAction(true);
		connect( (*scrActions)[shadeName], SIGNAL(activatedData(int)), ScApp, SLOT(setItemShade(int)));
	}

	//Type Effects actions
	scrActionGroups->insert("typeEffects", new QActionGroup(ScApp, "typeEffects", false));
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
	connect( (*scrActions)["typeEffectNormal"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectUnderline"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectUnderlineWords"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectStrikeThrough"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSmallCaps"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectAllCaps"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSuperscript"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSubscript"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectOutline"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectShadow"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));

	//Other Style menu items that get added in various places
	scrActions->insert("styleInvertPict", new ScrAction(ScApp, "styleInvertPict"));
	scrActions->insert("styleTabulators", new ScrAction(ScApp, "styleTabulators"));
	connect( (*scrActions)["styleInvertPict"], SIGNAL(activated()), ScApp, SLOT(ImageEffects()));
	connect( (*scrActions)["styleTabulators"], SIGNAL(activated()), ScApp, SLOT(EditTabs()));

}

void ActionManager::initItemMenuActions()
{
	//Item Menu
	scrActions->insert("itemDuplicate", new ScrAction("", CTRL+Key_D, ScApp, "itemDuplicate"));
	scrActions->insert("itemMulDuplicate", new ScrAction(ScApp, "itemMulDuplicate"));
	scrActions->insert("itemDelete", new ScrAction("", CTRL+Key_K, ScApp, "itemDelete"));
	scrActions->insert("itemGroup", new ScrAction("", CTRL+Key_G, ScApp, "itemGroup"));
	scrActions->insert("itemUngroup", new ScrAction("", CTRL+Key_U, ScApp, "itemUngroup"));
	scrActions->insert("itemLock", new ScrAction("", CTRL+Key_L, ScApp, "itemLock"));
	scrActions->insert("itemLockSize", new ScrAction("", CTRL+SHIFT+Key_L, ScApp, "itemLockSize"));
	(*scrActions)["itemLock"]->setToggleAction(true);
	(*scrActions)["itemLockSize"]->setToggleAction(true);
	scrActions->insert("itemLowerToBottom", new ScrAction(QIconSet(loadIcon("lower-to-bottom.png"), loadIcon("lower-to-bottom.png")), "", QKeySequence(Key_End), ScApp, "itemLowerToBottom"));
	scrActions->insert("itemRaiseToTop", new ScrAction(QIconSet(loadIcon("raise-to-top.png"), loadIcon("raise-to-top.png")), "", QKeySequence(Key_Home), ScApp, "itemRaiseToTop"));
	scrActions->insert("itemLower", new ScrAction(QIconSet(loadIcon("lower.png"), loadIcon("lower.png")), "", QKeySequence(CTRL+Key_End), ScApp, "itemLower"));
	scrActions->insert("itemRaise", new ScrAction(QIconSet(loadIcon("raise.png"), loadIcon("raise.png")), "", QKeySequence(CTRL+Key_Home), ScApp, "itemRaise"));
	
	scrActions->insert("itemSendToScrapbook", new ScrAction(ScApp, "itemSendToScrapbook"));

	scrActions->insert("itemAttributes", new ScrAction(ScApp, "itemAttributes"));
	scrActions->insert("itemImageIsVisible", new ScrAction(ScApp, "itemImageIsVisible"));
	scrActions->insert("itemUpdateImage", new ScrAction(ScApp, "itemUpdateImage"));
	scrActions->insert("itemAdjustFrameToImage", new ScrAction(ScApp, "itemAdjustFrameToImage"));
	scrActions->insert("itemExtendedImageProperties", new ScrAction(ScApp, "itemExtendedImageProperties"));
	scrActions->insert("itemPreviewLow", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScApp, "itemPreviewLow", 2));
	scrActions->insert("itemPreviewNormal", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScApp, "itemPreviewNormal", 1));
	scrActions->insert("itemPreviewFull", new ScrAction(ScrAction::DataInt, QIconSet(), "", QKeySequence(), ScApp, "itemPreviewFull", 0));
	scrActions->insert("itemPDFIsBookmark", new ScrAction(ScApp, "itemPDFIsBookmark"));
	(*scrActions)["itemPDFIsBookmark"]->setToggleAction(true);
	scrActions->insert("itemPDFIsAnnotation", new ScrAction(ScApp, "itemPDFIsAnnotation"));
	(*scrActions)["itemPDFIsAnnotation"]->setToggleAction(true);
	scrActions->insert("itemPDFAnnotationProps", new ScrAction(ScApp, "itemPDFAnnotationProps"));
	scrActions->insert("itemPDFFieldProps", new ScrAction(ScApp, "itemPDFFieldProps"));

	(*scrActions)["itemPDFIsBookmark"]->setEnabled(false);
	(*scrActions)["itemPDFIsAnnotation"]->setEnabled(false);
	(*scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
	(*scrActions)["itemPDFFieldProps"]->setEnabled(false);

	(*scrActions)["itemImageIsVisible"]->setToggleAction(true);
	(*scrActions)["itemPreviewLow"]->setToggleAction(true);
	(*scrActions)["itemPreviewNormal"]->setToggleAction(true);
	(*scrActions)["itemPreviewFull"]->setToggleAction(true);

	scrActions->insert("itemShapeEdit", new ScrAction(ScApp, "itemShapeEdit"));
	(*scrActions)["itemShapeEdit"]->setToggleAction(true);
	scrActions->insert("itemAttachTextToPath", new ScrAction(ScApp, "itemAttachTextToPath"));
	scrActions->insert("itemDetachTextFromPath", new ScrAction(ScApp, "itemDetachTextFromPath"));
	scrActions->insert("itemCombinePolygons", new ScrAction(ScApp, "itemCombinePolygons"));
	scrActions->insert("itemSplitPolygons", new ScrAction(ScApp, "itemSplitPolygons"));
	scrActions->insert("itemConvertToBezierCurve", new ScrAction(ScApp, "itemConvertToBezierCurve"));
	scrActions->insert("itemConvertToImageFrame", new ScrAction(ScApp, "itemConvertToImageFrame"));
	scrActions->insert("itemConvertToOutlines", new ScrAction(ScApp, "itemConvertToOutlines"));
	scrActions->insert("itemConvertToPolygon", new ScrAction(ScApp, "itemConvertToPolygon"));
	scrActions->insert("itemConvertToTextFrame", new ScrAction(ScApp, "itemConvertToTextFrame"));

	connect( (*scrActions)["itemDuplicate"], SIGNAL(activated()), ScApp, SLOT(ObjektDup()) );
	connect( (*scrActions)["itemMulDuplicate"], SIGNAL(activated()), ScApp, SLOT(ObjektDupM()) );
	connect( (*scrActions)["itemDelete"], SIGNAL(activated()), ScApp, SLOT(DeleteObjekt()) );
	connect( (*scrActions)["itemGroup"], SIGNAL(activated()), ScApp, SLOT(GroupObj()) );
	connect( (*scrActions)["itemUngroup"], SIGNAL(activated()), ScApp, SLOT(UnGroupObj()) );
	//connect( (*scrActions)["itemLock"], SIGNAL(activated()), ScApp, SLOT(ToggleObjLock()) );
	//connect( (*scrActions)["itemLockSize"], SIGNAL(activated()), ScApp, SLOT(ToggleObjSizeLock()) );
	//connect( (*scrActions)["itemPDFIsAnnotation"], SIGNAL(activated()), ScApp, SLOT(ToggleObjPDFAnnotation()) );
	//connect( (*scrActions)["itemPDFIsBookmark"], SIGNAL(activated()), ScApp, SLOT(ToggleObjPDFBookmark()) );
	connect( (*scrActions)["itemPDFAnnotationProps"], SIGNAL(activated()), ScApp, SLOT(ModifyAnnot()) );
	connect( (*scrActions)["itemPDFFieldProps"], SIGNAL(activated()), ScApp, SLOT(ModifyAnnot()) );
	//connect( (*scrActions)["itemLowerToBottom"], SIGNAL(activated()), ScApp, SLOT(Objekt2Back()) );
	//connect( (*scrActions)["itemRaiseToTop"], SIGNAL(activated()), ScApp, SLOT(Objekt2Front()) );
	//connect( (*scrActions)["itemLower"], SIGNAL(activated()), ScApp, SLOT(ObjektLower()) );
	//connect( (*scrActions)["itemRaise"], SIGNAL(activated()), ScApp, SLOT(ObjektRaise()) );
	connect( (*scrActions)["itemSendToScrapbook"], SIGNAL(activated()), ScApp, SLOT(PutScrap()) );
	connect( (*scrActions)["itemAttributes"], SIGNAL(activated()), ScApp, SLOT(objectAttributes()) );
	connect( (*scrActions)["itemShapeEdit"], SIGNAL(activated()), ScApp, SLOT(ToggleFrameEdit()) );
	//connect( (*scrActions)["itemAttachTextToPath"], SIGNAL(activated()), ScApp, SLOT(Pfadtext()) );
	//connect( (*scrActions)["itemDetachTextFromPath"], SIGNAL(activated()), ScApp, SLOT(noPfadtext()) );
	//connect( (*scrActions)["itemCombinePolygons"], SIGNAL(activated()), ScApp, SLOT(UniteOb()) );
	//connect( (*scrActions)["itemSplitPolygons"], SIGNAL(activated()), ScApp, SLOT(SplitUniteOb()) );
	//connect( (*scrActions)["itemConvertToBezierCurve"], SIGNAL(activated()), ScApp, SLOT(convertToBezierCurve()) );
	//connect( (*scrActions)["itemConvertToImageFrame"], SIGNAL(activated()), ScApp, SLOT(convertToImageFrame()) );
	//connect( (*scrActions)["itemConvertToOutlines"], SIGNAL(activated()), ScApp, SLOT(convertToOutlines()) );
	//connect( (*scrActions)["itemConvertToPolygon"], SIGNAL(activated()), ScApp, SLOT(convertToPolygon()) );
	//connect( (*scrActions)["itemConvertToTextFrame"], SIGNAL(activated()), ScApp, SLOT(convertToTextFrame()) );
}

void ActionManager::initInsertMenuActions()
{
	//Insert Menu
	scrActions->insert("insertGlyph", new ScrAction(ScApp, "insertGlyph"));
	scrActions->insert("insertSampleText", new ScrAction(ScApp, "insertSampleText"));

	connect( (*scrActions)["insertGlyph"], SIGNAL(activated()), ScApp, SLOT(slotCharSelect()) );
	connect( (*scrActions)["insertSampleText"], SIGNAL(activated()), ScApp, SLOT(insertSampleText()) );
}

void ActionManager::initPageMenuActions()
{
	//Page menu
	scrActions->insert("pageInsert", new ScrAction(ScApp, "pageInsert"));
	scrActions->insert("pageImport", new ScrAction(ScApp, "pageImport"));
	scrActions->insert("pageDelete", new ScrAction(ScApp, "pageDelete"));
	scrActions->insert("pageCopy", new ScrAction(ScApp, "pageCopy"));
	scrActions->insert("pageMove", new ScrAction(ScApp, "pageMove"));
	scrActions->insert("pageApplyMasterPage", new ScrAction(ScApp, "pageApplyMasterPage"));
	scrActions->insert("pageManageGuides", new ScrAction(ScApp, "pageManageGuides"));

	connect( (*scrActions)["pageInsert"], SIGNAL(activated()), ScApp, SLOT(slotNewPageM()) );
	connect( (*scrActions)["pageImport"], SIGNAL(activated()), ScApp, SLOT(slotPageImport()) );
	connect( (*scrActions)["pageDelete"], SIGNAL(activated()), ScApp, SLOT(DeletePage()) );
	connect( (*scrActions)["pageCopy"], SIGNAL(activated()), ScApp, SLOT(CopyPage()) );
	connect( (*scrActions)["pageMove"], SIGNAL(activated()), ScApp, SLOT(MovePage()) );
	connect( (*scrActions)["pageApplyMasterPage"], SIGNAL(activated()), ScApp, SLOT(ApplyMasterPage()) );
	connect( (*scrActions)["pageManageGuides"], SIGNAL(activated()), ScApp, SLOT(ManageGuides()) );
}

void ActionManager::initViewMenuActions()
{
	scrActions->insert("viewFitInWindow", new ScrAction(ScrAction::DataDouble, QIconSet(), "", CTRL+Key_0, ScApp, "viewFitInWindow", 0, -100.0));
	scrActions->insert("viewFit50", new ScrAction(ScrAction::DataDouble, QIconSet(), "", QKeySequence(), ScApp, "viewFit50", 0, 50.0));
	scrActions->insert("viewFit75", new ScrAction(ScrAction::DataDouble, QIconSet(), "", QKeySequence(), ScApp, "viewFit75", 0, 75.0));
	scrActions->insert("viewFit100", new ScrAction(ScrAction::DataDouble, QIconSet(), "", CTRL+Key_1, ScApp, "viewFit100", 0, 100.0));
	scrActions->insert("viewFit200", new ScrAction(ScrAction::DataDouble, QIconSet(), "", QKeySequence(), ScApp, "viewFit200", 0, 200.0));
	scrActions->insert("viewFit20", new ScrAction(ScrAction::DataDouble, QIconSet(), "", QKeySequence(), ScApp, "viewFit20", 0, 20.0));
	scrActions->insert("viewShowMargins", new ScrAction(ScApp, "viewShowMargins"));
	scrActions->insert("viewShowFrames", new ScrAction(ScApp, "viewShowFrames"));
	scrActions->insert("viewShowImages", new ScrAction(ScApp, "viewShowImages"));
	scrActions->insert("viewShowGrid", new ScrAction(ScApp, "viewShowGrid"));
	scrActions->insert("viewShowGuides", new ScrAction(ScApp, "viewShowGuides"));
	scrActions->insert("viewShowBaseline", new ScrAction(ScApp, "viewShowBaseline"));
	scrActions->insert("viewShowTextChain", new ScrAction(ScApp, "viewShowTextChain"));
	scrActions->insert("viewShowTextControls", new ScrAction(ScApp, "viewShowTextControls"));
	scrActions->insert("viewSnapToGrid", new ScrAction(ScApp, "viewSnapToGrid"));
	scrActions->insert("viewSnapToGuides", new ScrAction(ScApp, "viewSnapToGuides"));
//	scrActions->insert("viewNewView", new ScrAction(ScApp, "viewNewView"));

	(*scrActions)["viewShowMargins"]->setToggleAction(true);
	(*scrActions)["viewShowFrames"]->setToggleAction(true);
	(*scrActions)["viewShowImages"]->setToggleAction(true);
	(*scrActions)["viewShowGrid"]->setToggleAction(true);
	(*scrActions)["viewShowGuides"]->setToggleAction(true);
	(*scrActions)["viewShowBaseline"]->setToggleAction(true);
	(*scrActions)["viewShowTextChain"]->setToggleAction(true);
	(*scrActions)["viewShowTextControls"]->setToggleAction(true);
	(*scrActions)["viewSnapToGrid"]->setToggleAction(true);
	(*scrActions)["viewSnapToGuides"]->setToggleAction(true);

	(*scrActions)["viewShowMargins"]->setOn(true);
	(*scrActions)["viewShowFrames"]->setOn(true);
	(*scrActions)["viewShowImages"]->setOn(true);
	(*scrActions)["viewShowGuides"]->setOn(true);

	connect( (*scrActions)["viewFitInWindow"], SIGNAL(activatedData(double)), ScApp, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit50"], SIGNAL(activatedData(double)), ScApp, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit75"], SIGNAL(activatedData(double)), ScApp, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit100"], SIGNAL(activatedData(double)), ScApp, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit200"], SIGNAL(activatedData(double)), ScApp, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewFit20"], SIGNAL(activatedData(double)), ScApp, SLOT(slotZoom(double)) );
	connect( (*scrActions)["viewShowMargins"], SIGNAL(activated()), ScApp, SLOT(ToggleMarks()) );
	connect( (*scrActions)["viewShowFrames"], SIGNAL(activated()), ScApp, SLOT(ToggleFrames()) );
	connect( (*scrActions)["viewShowImages"], SIGNAL(activated()), ScApp, SLOT(TogglePics()) );
	connect( (*scrActions)["viewShowGrid"], SIGNAL(activated()), ScApp, SLOT(ToggleRaster()) );
	connect( (*scrActions)["viewShowGuides"], SIGNAL(activated()), ScApp, SLOT(ToggleGuides()) );
	connect( (*scrActions)["viewShowBaseline"], SIGNAL(activated()), ScApp, SLOT(ToggleBase()) );
	connect( (*scrActions)["viewShowTextChain"], SIGNAL(activated()), ScApp, SLOT(ToggleTextLinks()) );
	connect( (*scrActions)["viewShowTextControls"], SIGNAL(activated()), ScApp, SLOT(ToggleTextControls()) );
	connect( (*scrActions)["viewSnapToGrid"], SIGNAL(activated()), ScApp, SLOT(ToggleURaster()) );
	connect( (*scrActions)["viewSnapToGuides"], SIGNAL(activated()), ScApp, SLOT(ToggleUGuides()) );
//	connect( (*scrActions)["viewNewView"], SIGNAL(activated()), ScApp, SLOT(newView()) );

}

void ActionManager::initToolsMenuActions()
{
	//Tool menu
	scrActions->insert("toolsProperties", new ScrAction(ScApp, "toolsProperties"));
	scrActions->insert("toolsOutline", new ScrAction(ScApp, "toolsOutline"));
	scrActions->insert("toolsScrapbook", new ScrAction(ScApp, "toolsScrapbook"));
	scrActions->insert("toolsLayers", new ScrAction("", QKeySequence(Key_F6), ScApp, "toolsLayers"));
	scrActions->insert("toolsPages", new ScrAction(ScApp, "toolsPages"));
	scrActions->insert("toolsBookmarks", new ScrAction(ScApp, "toolsBookmarks"));
	scrActions->insert("toolsMeasurements", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("dist.png"), loadIcon("dist.png")), "", QKeySequence(), ScApp, "toolsMeasurements", modeMeasurementTool));
	scrActions->insert("toolsActionHistory", new ScrAction(ScApp, "toolsActionHistory"));
	scrActions->insert("toolsPreflightVerifier", new ScrAction(QIconSet(loadIcon("launch16.png"), loadIcon("launch.png")),"", QKeySequence(), ScApp, "toolsPreflightVerifier"));
	scrActions->insert("toolsAlignDistribute", new ScrAction(ScApp, "toolsAlignDistribute"));
	scrActions->insert("toolsToolbarTools", new ScrAction(ScApp, "toolsToolbarTools"));
	scrActions->insert("toolsToolbarPDF", new ScrAction(ScApp, "toolsToolbarPDF"));

	//toolbar only items
	scrActions->insert("toolsSelect", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Kreuz.xpm"), loadIcon("Kreuz.xpm")), "", QKeySequence(Key_C), ScApp, "toolsSelect", modeNormal));
	scrActions->insert("toolsInsertTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Text16.xpm"), loadIcon("Text.xpm")), "", QKeySequence(Key_T), ScApp, "toolsInsertTextFrame", modeDrawText));
	scrActions->insert("toolsInsertImageFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Bild16.xpm"), loadIcon("Bild.xpm")), "", QKeySequence(Key_I), ScApp, "toolsInsertImageFrame", modeDrawPicture));
	scrActions->insert("toolsInsertTableFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("frame_table16.png"), loadIcon("frame_table.png")), "", QKeySequence(Key_A), ScApp, "toolsInsertTableFrame", modeDrawTable));
	scrActions->insert("toolsInsertShape", new ScrAction(ScrAction::DataInt,QIconSet(), "", QKeySequence(Key_S), ScApp, "toolsInsertShape", modeDrawShapes));
	scrActions->insert("toolsInsertPolygon", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("spline16.png"), loadIcon("spline.png")), "", QKeySequence(Key_P), ScApp, "toolsInsertPolygon", modeDrawRegularPolygon));
	scrActions->insert("toolsInsertLine", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Stift16.xpm"), loadIcon("Stift.xpm")), "", QKeySequence(Key_L), ScApp, "toolsInsertLine", modeDrawLine));
	scrActions->insert("toolsInsertBezier", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("beziertool16.png"), loadIcon("beziertool.png")), "", QKeySequence(Key_B), ScApp, "toolsInsertBezier", modeDrawBezierLine));
	scrActions->insert("toolsInsertFreehandLine", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Stiftalt16.xpm"), loadIcon("Stiftalt.xpm")), "", QKeySequence(Key_F), ScApp, "toolsInsertFreehandLine", modeDrawFreehandLine));

	scrActions->insert("toolsRotate", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Rotieren.xpm"), loadIcon("Rotieren.xpm")), "", QKeySequence(Key_R), ScApp, "toolsRotate", modeRotation));
	scrActions->insert("toolsZoom", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Lupe.xpm"), loadIcon("Lupe.xpm")), "", QKeySequence(Key_Z), ScApp, "toolsZoom", modeMagnifier));
	scrActions->insert("toolsZoomIn", new ScrAction(QIconSet(loadIcon("Gross.xpm"), loadIcon("Gross.xpm")), "", QKeySequence(CTRL+Key_Plus), ScApp, "toolsZoomIn"));
	scrActions->insert("toolsZoomOut", new ScrAction(QIconSet(loadIcon("Klein.xpm"), loadIcon("Klein.xpm")), "", QKeySequence(CTRL+Key_Minus), ScApp, "toolsZoomOut"));
	scrActions->insert("toolsEditContents", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Editm.xpm"), loadIcon("Editm.xpm")), "", QKeySequence(Key_E), ScApp, "toolsEditContents", modeEdit));
	scrActions->insert("toolsEditWithStoryEditor", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("signature.png"), loadIcon("signature.png")), "", QKeySequence(CTRL+Key_Y), ScApp, "toolsEditWithStoryEditor", modeStoryEditor));
	scrActions->insert("toolsLinkTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Lock.xpm"), loadIcon("Lock.xpm")), "", QKeySequence(Key_N), ScApp, "toolsLinkTextFrame", modeLinkFrames));
	scrActions->insert("toolsUnlinkTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Unlock.xpm"), loadIcon("Unlock.xpm")), "", QKeySequence(Key_U), ScApp, "toolsUnlinkTextFrame", modeUnlinkFrames));
	scrActions->insert("toolsEyeDropper", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("colorpicker.png"), loadIcon("colorpicker.png")), "", QKeySequence(Key_Y), ScApp, "toolsEyeDropper", modeEyeDropper));
	scrActions->insert("toolsCopyProperties", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("wizard.png"), loadIcon("wizard.png")), "", QKeySequence(), ScApp, "toolsCopyProperties", modeCopyProperties));

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

	connect( (*scrActions)["toolsActionHistory"], SIGNAL(toggled(bool)), ScApp, SLOT(setUndoPalette(bool)) );
	connect( (*scrActions)["toolsToolbarTools"], SIGNAL(toggled(bool)), ScApp, SLOT(setTools(bool)) );
	connect( (*scrActions)["toolsToolbarPDF"], SIGNAL(toggled(bool)), ScApp, SLOT(setPDFTools(bool)) );

	connectModeActions();
}

void ActionManager::initExtrasMenuActions()
{
	scrActions->insert("extrasManagePictures", new ScrAction(ScApp, "extrasManagePictures"));
	scrActions->insert("extrasHyphenateText", new ScrAction(ScApp, "extrasHyphenateText"));
	scrActions->insert("extrasGenerateTableOfContents", new ScrAction(ScApp, "extrasGenerateTableOfContents"));

	connect( (*scrActions)["extrasManagePictures"], SIGNAL(activated()), ScApp, SLOT(StatusPic()) );
	connect( (*scrActions)["extrasHyphenateText"], SIGNAL(activated()), ScApp, SLOT(doHyphenate()) );
	connect( (*scrActions)["extrasGenerateTableOfContents"], SIGNAL(activated()), ScApp, SLOT(generateTableOfContents()) );
}


void ActionManager::initWindowsMenuActions()
{
}

void ActionManager::initScriptMenuActions()
{
}

void ActionManager::initHelpMenuActions()
{
	scrActions->insert("helpAboutScribus", new ScrAction(ScApp, "helpAboutScribus"));
	scrActions->insert("helpAboutQt", new ScrAction(ScApp, "helpAboutQt"));
	scrActions->insert("helpTooltips", new ScrAction(ScApp, "helpTooltips"));
	scrActions->insert("helpManual", new ScrAction("", Key_F1, ScApp, "helpManual"));

	(*scrActions)["helpTooltips"]->setToggleAction(true);
	(*scrActions)["helpTooltips"]->setOn(true);

	connect( (*scrActions)["helpAboutScribus"], SIGNAL(activated()), ScApp, SLOT(slotHelpAbout()) );
	connect( (*scrActions)["helpAboutQt"], SIGNAL(activated()), ScApp, SLOT(slotHelpAboutQt()) );
	connect( (*scrActions)["helpTooltips"], SIGNAL(activated()), ScApp, SLOT(ToggleTips()) );
	connect( (*scrActions)["helpManual"], SIGNAL(activated()), ScApp, SLOT(slotOnlineHelp()) );
}

void ActionManager::initSpecialActions()
{
	//typography
	scrActions->insert("specialSmartHyphen", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+SHIFT+Key_Minus, ScApp, "specialSmartHyphen",-1));
	scrActions->insert("specialNonBreakingHyphen", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+ALT+Key_Minus, ScApp, "specialNonBreakingHyphen",24));
	scrActions->insert("specialNonBreakingSpace", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+Key_Space, ScApp, "specialNonBreakingSpace",29));
	scrActions->insert("specialPageNumber", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+SHIFT+ALT+Key_P, ScApp, "specialPageNumber",30));
	scrActions->insert("specialNewLine", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", SHIFT+Key_Return, ScApp, "specialNewLine",28));
	scrActions->insert("specialFrameBreak", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+Key_Return, ScApp, "specialFrameBreak",27));
	scrActions->insert("specialColumnBreak", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", CTRL+SHIFT+Key_Return, ScApp, "specialColumnBreak",26));
	scrActions->insert("specialCopyRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialCopyRight",0x0A9));
	scrActions->insert("specialRegdTM", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialRegdTM",0x00AE));
	scrActions->insert("specialTM", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialTM",0x2122));
	scrActions->insert("specialBullet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialBullet",0x2022));
	scrActions->insert("specialDashEm", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialDashEm",0x2014));
	scrActions->insert("specialDashEn", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialDashEn",0x2013));
	scrActions->insert("specialDashFigure", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialDashFigure",0x2012));
	scrActions->insert("specialDashQuotation", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialDashQuotation",0x2015));

	scrActions->insert("specialQuoteApostrophe", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteApostrophe",0x0027));
	scrActions->insert("specialQuoteStraight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteStraight",0x0022));
	scrActions->insert("specialQuoteSingleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteSingleLeft",0x2018));
	scrActions->insert("specialQuoteSingleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteSingleRight",0x2019));
	scrActions->insert("specialQuoteDoubleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteDoubleLeft",0x201C));
	scrActions->insert("specialQuoteDoubleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteDoubleRight",0x201D));
	scrActions->insert("specialQuoteSingleReversed", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteSingleReversed",0x201B));
	scrActions->insert("specialQuoteDoubleReversed", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteDoubleReversed",0x201F));
	scrActions->insert("specialQuoteSingleLeftGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteSingleLeftGuillemet",0x2039));
	scrActions->insert("specialQuoteSingleRightGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteSingleRightGuillemet",0x203A));
	scrActions->insert("specialQuoteDoubleLeftGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteDoubleLeftGuillemet",0x00AB));
	scrActions->insert("specialQuoteDoubleRightGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteDoubleRightGuillemet",0x00BB));
	scrActions->insert("specialQuoteLowSingleComma", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteLowSingleComma",0x201A));
	scrActions->insert("specialQuoteLowDoubleComma", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteLowDoubleComma",0x201E));
	scrActions->insert("specialQuoteDoubleTurnedComma", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteDoubleTurnedComma",0x201C));
	scrActions->insert("specialQuoteCJKSingleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteCJKSingleLeft",0x300C));
	scrActions->insert("specialQuoteCJKSingleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteCJKSingleRight",0x300D));
	scrActions->insert("specialQuoteCJKDoubleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteCJKDoubleLeft",0x300E));
	scrActions->insert("specialQuoteCJKDoubleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), "", QKeySequence(), ScApp, "specialQuoteCJKDoubleRight",0x300F));

	//Spaces and special characters
	*unicodeCharActionNames << "specialSmartHyphen" << "specialNonBreakingHyphen" << "specialNonBreakingSpace" << "specialPageNumber";
	//Breaks
	*unicodeCharActionNames << "specialNewLine" << "specialFrameBreak" << "specialColumnBreak";
	//Copyrights and TMs
	*unicodeCharActionNames << "specialCopyRight" << "specialRegdTM" << "specialTM";
	//Bullets
	*unicodeCharActionNames << "specialBullet";
	//Dashes
	*unicodeCharActionNames << "specialDashEm" << "specialDashEn" << "specialDashFigure" << "specialDashQuotation";
	//Straight quotes
	*unicodeCharActionNames << "specialQuoteApostrophe" << "specialQuoteStraight";
	//Double quotes
	*unicodeCharActionNames << "specialQuoteDoubleLeft" << "specialQuoteDoubleRight" << "specialQuoteSingleLeft" << "specialQuoteSingleRight";
	//Alternative single quotes
	*unicodeCharActionNames << "specialQuoteSingleReversed" << "specialQuoteDoubleReversed";
	//French quotes
	*unicodeCharActionNames << "specialQuoteSingleLeftGuillemet" << "specialQuoteSingleRightGuillemet" << "specialQuoteDoubleLeftGuillemet" << "specialQuoteDoubleRightGuillemet";
	//German quotes
	*unicodeCharActionNames << "specialQuoteLowSingleComma" << "specialQuoteLowDoubleComma" << "specialQuoteDoubleTurnedComma";
	//CJK Quotes
	*unicodeCharActionNames << "specialQuoteCJKSingleLeft" << "specialQuoteCJKSingleRight" << "specialQuoteCJKDoubleLeft" << "specialQuoteCJKDoubleRight";

	for ( QStringList::Iterator it = unicodeCharActionNames->begin(); it != unicodeCharActionNames->end(); ++it )
		connect( (*scrActions)[*it], SIGNAL(activatedUnicodeShortcut(QString, int)), ScApp, SLOT(specialActionKeyEvent(QString, int)) );
	enableUnicodeActions(false);

	//GUI
	scrActions->insert("specialToggleAllPalettes", new ScrAction(ScrAction::DataQString, QIconSet(), "", Key_F10, ScApp, "specialToggleAllPalettes",0,0.0,"specialToggleAllPalettes"));
	scrActions->insert("specialToggleAllGuides", new ScrAction(ScrAction::DataQString, QIconSet(), "", Key_F11, ScApp, "specialToggleAllGuides",0,0.0,"specialToggleAllGuides"));

	connect( (*scrActions)["specialToggleAllPalettes"], SIGNAL(activated()), ScApp, SLOT(ToggleAllPalettes()) );
	connect( (*scrActions)["specialToggleAllGuides"], SIGNAL(activated()), ScApp, SLOT(ToggleAllGuides()) );
}

void ActionManager::disconnectModeActions()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
		disconnect( (*scrActions)[*it], SIGNAL(toggledData(bool, int)) , ScApp, SLOT(setAppModeByToggle(bool, int)) );
}

void ActionManager::connectModeActions()
{
	for ( QStringList::Iterator it = modeActionNames->begin(); it != modeActionNames->end(); ++it )
		connect( (*scrActions)[*it], SIGNAL(toggledData(bool, int)) , ScApp, SLOT(setAppModeByToggle(bool, int)) );
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
	connect( (*scrActions)["itemAdjustFrameToImage"], SIGNAL(activated()), currView, SLOT(FrameToPic()) );
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
			if (ScApp->HaveDoc && (*scrActions)[*it]->actionType()==ScrAction::UnicodeChar)
			{
				int charCode=(*scrActions)[*it]->actionInt();
				if(charCode==-1 ||
				   charCode==24 ||
				   charCode==26 ||
				   charCode==27 ||
				   charCode==28 ||
				   charCode==29 ||
				   charCode==30 ||
				   (*ScApp->doc->AllFonts)[ScApp->doc->CurrFont]->CharWidth.contains(charCode) )
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
	PageItem* currItem = currView->SelItem.at(0);
	if (currItem==NULL)
		return;

	disconnect( (*scrActions)["itemPDFIsBookmark"], 0, 0, 0);
	disconnect( (*scrActions)["itemPDFIsAnnotation"], 0, 0, 0);

	if (currItem->itemType()!=PageItem::TextFrame)
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
	(*scrActions)["itemPDFIsAnnotation"]->setOn(currItem->isAnnotation);
	(*scrActions)["itemPDFIsBookmark"]->setOn(currItem->isBookmark);
	if (currItem->isAnnotation)
	{
		bool setter=((currItem->AnType == 0) || (currItem->AnType == 1) || (currItem->AnType > 9));
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
	(*scrActions)["editJavascripts"]->setTexts( tr("&Javascripts..."));
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
	(*scrActions)["pageManageGuides"]->setTexts( tr("Manage &Guides..."));

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
	(*scrActions)["viewSnapToGrid"]->setTexts( tr("Sn&ap to Grid"));
	(*scrActions)["viewSnapToGuides"]->setTexts( tr("Sna&p to Guides"));
//	(*scrActions)["viewNewView"]->setTexts( tr("New View"));

	//Tool menu
	(*scrActions)["toolsProperties"]->setTexts( tr("&Properties"));
	(*scrActions)["toolsOutline"]->setTexts( tr("&Outline"));
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
	(*scrActions)["extrasGenerateTableOfContents"]->setTexts( tr("&Generate Table Of Contents"));

	//Help Menu
	(*scrActions)["helpAboutScribus"]->setTexts( tr("&About Scribus"));
	(*scrActions)["helpAboutQt"]->setTexts( tr("About &Qt"));
	(*scrActions)["helpTooltips"]->setTexts( tr("Toolti&ps"));
	(*scrActions)["helpManual"]->setTexts( tr("Scribus &Manual..."));


	//typography
	(*scrActions)["specialSmartHyphen"]->setMenuText( tr("Smart &Hyphen"));
	(*scrActions)["specialNonBreakingHyphen"]->setMenuText( tr("Non Breaking Dash"));
	(*scrActions)["specialNonBreakingSpace"]->setMenuText( tr("Non Breaking &Space"));
	(*scrActions)["specialPageNumber"]->setMenuText( tr("Page &Number"));
	(*scrActions)["specialNewLine"]->setMenuText( tr("New Line"));
	(*scrActions)["specialFrameBreak"]->setMenuText( tr("Frame Break"));
	(*scrActions)["specialColumnBreak"]->setMenuText( tr("Column Break"));
	(*scrActions)["specialCopyRight"]->setTexts( tr("Copyright"));
	(*scrActions)["specialRegdTM"]->setTexts( tr("Registered Trademark"));
	(*scrActions)["specialTM"]->setTexts( tr("Trademark"));
	(*scrActions)["specialBullet"]->setTexts( tr("Bullet"));
	(*scrActions)["specialDashEm"]->setTexts( tr("Em Dash"));
	(*scrActions)["specialDashEn"]->setTexts( tr("En Dash"));
	(*scrActions)["specialDashFigure"]->setTexts( tr("Figure Dash"));
	(*scrActions)["specialDashQuotation"]->setTexts( tr("Quotation Dash"));

	(*scrActions)["specialQuoteApostrophe"]->setTexts( tr("Apostrophe"));
	(*scrActions)["specialQuoteStraight"]->setTexts( tr("Straight Double"));
	(*scrActions)["specialQuoteSingleLeft"]->setTexts( tr("Single Left"));
	(*scrActions)["specialQuoteSingleRight"]->setTexts( tr("Single Right"));
	(*scrActions)["specialQuoteDoubleLeft"]->setTexts( tr("Double Left"));
	(*scrActions)["specialQuoteDoubleRight"]->setTexts( tr("Double Right"));
	(*scrActions)["specialQuoteSingleReversed"]->setTexts( tr("Single Reversed"));
	(*scrActions)["specialQuoteDoubleReversed"]->setTexts( tr("Double Reversed"));
	(*scrActions)["specialQuoteSingleLeftGuillemet"]->setTexts( tr("Single Left Guillemet"));
	(*scrActions)["specialQuoteSingleRightGuillemet"]->setTexts( tr("Single Right Guillemet"));
	(*scrActions)["specialQuoteDoubleLeftGuillemet"]->setTexts( tr("Double Left Guillemet"));
	(*scrActions)["specialQuoteDoubleRightGuillemet"]->setTexts( tr("Double Right Guillemet"));
	(*scrActions)["specialQuoteLowSingleComma"]->setTexts( tr("Low Single Comma"));
	(*scrActions)["specialQuoteLowDoubleComma"]->setTexts( tr("Low Double Comma"));
	(*scrActions)["specialQuoteDoubleTurnedComma"]->setTexts( tr("Double Turned Comma"));
	(*scrActions)["specialQuoteCJKSingleLeft"]->setTexts( tr("CJK Single Left"));
	(*scrActions)["specialQuoteCJKSingleRight"]->setTexts( tr("CJK Single Right"));
	(*scrActions)["specialQuoteCJKDoubleLeft"]->setTexts( tr("CJK Double Left"));
	(*scrActions)["specialQuoteCJKDoubleRight"]->setTexts( tr("CJK Double Right"));

	(*scrActions)["specialSmartHyphen"]->setText("Insert Smart Hyphen");
	(*scrActions)["specialNonBreakingHyphen"]->setText("Insert Non Breaking Dash");
	(*scrActions)["specialNonBreakingSpace"]->setText("Insert Non Breaking Space");
	(*scrActions)["specialPageNumber"]->setText("Insert Page Number");
	(*scrActions)["specialNewLine"]->setText("New Line");
	(*scrActions)["specialFrameBreak"]->setText("Frame Break");
	(*scrActions)["specialColumnBreak"]->setText( "Column Break");

	//GUI
	(*scrActions)["specialToggleAllPalettes"]->setTexts( tr("Toggle Palettes"));
	(*scrActions)["specialToggleAllGuides"]->setTexts( tr("Toggle Guides"));
}

