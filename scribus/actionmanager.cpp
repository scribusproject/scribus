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
	scrActions->insert("fileNew", new ScrAction(QIconSet(loadIcon("DateiNeu16.png"), loadIcon("DateiNeu.xpm")), tr("&New"), CTRL+Key_N, ScApp, "fileNew"));
	scrActions->insert("fileOpen", new ScrAction(QIconSet(loadIcon("DateiOpen16.png"), loadIcon("DateiOpen.xpm")), tr("&Open..."), CTRL+Key_O, ScApp, "fileOpen"));
	scrActions->insert("fileClose", new ScrAction(QIconSet(loadIcon("DateiClos16.png"), loadIcon("DateiClose.png")), tr("&Close"), CTRL+Key_W, ScApp, "fileClose"));
	scrActions->insert("fileSave", new ScrAction(QIconSet(loadIcon("DateiSave16.png"), loadIcon("DateiSave2.png")), tr("&Save"), CTRL+Key_S, ScApp, "fileSave"));
	scrActions->insert("fileSaveAs", new ScrAction(QPixmap(loadIcon("filesaveas.png")), tr("Save &As..."), CTRL+SHIFT+Key_S, ScApp, "fileSaveAs"));
	scrActions->insert("fileRevert", new ScrAction(QPixmap(loadIcon("revert.png")), tr("Re&vert to Saved"), QKeySequence(), ScApp, "fileRevert"));
	scrActions->insert("fileCollect", new ScrAction(tr("Collect for O&utput..."), QKeySequence(), ScApp, "fileCollect"));
	//File Import Menu
	scrActions->insert("fileImportText", new ScrAction(tr("Get Text..."), QKeySequence(), ScApp, "fileImportText"));
	scrActions->insert("fileImportAppendText", new ScrAction(tr("Append &Text..."), QKeySequence(), ScApp, "fileImportAppendText"));
	scrActions->insert("fileImportImage", new ScrAction(tr("Get Image..."), QKeySequence(), ScApp, "fileImportImage"));

	//File Export Menu
	scrActions->insert("fileExportText", new ScrAction(tr("Save &Text..."), QKeySequence(), ScApp, "fileExportText"));
	scrActions->insert("fileExportAsEPS", new ScrAction(tr("Save Page as &EPS..."), QKeySequence(), ScApp, "fileExportAsEPS"));
	scrActions->insert("fileExportAsPDF", new ScrAction(loadIcon("acrobat.png"), tr("Save as P&DF..."), QKeySequence(), ScApp, "fileExportAsPDF"));
	//Rest of File Menu
	scrActions->insert("fileDocInfo", new ScrAction(loadIcon("documentinfo.png"), tr("Document &Information..."), CTRL+Key_I, ScApp, "fileDocInfo"));
	scrActions->insert("fileDocSetup", new ScrAction(tr("Document &Setup..."), QKeySequence(), ScApp, "fileDocSetup"));
	scrActions->insert("filePrint", new ScrAction(QIconSet(loadIcon("DateiPrint16.png"), loadIcon("DateiPrint.xpm")), tr("&Print..."), CTRL+Key_P, ScApp, "filePrint"));
	scrActions->insert("fileQuit", new ScrAction(QPixmap(loadIcon("exit.png")), tr("&Quit"), CTRL+Key_Q, ScApp, "fileQuit"));

	//Connect our signals and slots
	//File Menu
	connect( (*scrActions)["fileNew"], SIGNAL(activated()), ScApp, SLOT(slotFileNew()) );
	connect( (*scrActions)["fileOpen"], SIGNAL(activated()), ScApp, SLOT(slotDocOpen()) );
	connect( (*scrActions)["fileClose"], SIGNAL(activated()), ScApp, SLOT(slotFileClose()) );
	connect( (*scrActions)["filePrint"], SIGNAL(activated()), ScApp, SLOT(slotFilePrint()) );
	connect( (*scrActions)["fileSave"], SIGNAL(activated()), ScApp, SLOT(slotFileSave()) );
	connect( (*scrActions)["fileSaveAs"], SIGNAL(activated()), ScApp, SLOT(slotFileSaveAs()) );
	connect( (*scrActions)["fileDocInfo"], SIGNAL(activated()), ScApp, SLOT(InfoDoc()) );
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
	scrActions->insert("editUndoAction", new ScrAction(ScrAction::DataInt, QIconSet(loadIcon("u_undo16.png"), loadIcon("u_undo.png")), tr("&Undo"), CTRL+Key_Z, ScApp, "editUndoAction",1));
	scrActions->insert("editRedoAction", new ScrAction(ScrAction::DataInt, QIconSet(loadIcon("u_redo16.png"), loadIcon("u_redo.png")), tr("&Redo"), CTRL+SHIFT+Key_Z, ScApp, "editRedoAction", 1));
	scrActions->insert("editActionMode", new ScrAction(tr("&Item Action Mode"), QKeySequence(), ScApp, "editActionMode"));
	(*scrActions)["editActionMode"]->setToggleAction(true);
	scrActions->insert("editCut", new ScrAction(QIconSet(loadIcon("editcut.png"), loadIcon("editcut.png")), tr("Cu&t"), CTRL+Key_X, ScApp, "editCut"));
	scrActions->insert("editCopy", new ScrAction(QIconSet(loadIcon("editcopy.png"), loadIcon("editcopy.png")), tr("&Copy"), CTRL+Key_C, ScApp, "editCopy"));
	scrActions->insert("editPaste", new ScrAction(QIconSet(loadIcon("editpaste.png"), loadIcon("editpaste.png")), tr("&Paste"), CTRL+Key_V, ScApp, "editPaste"));
	scrActions->insert("editClearContents", new ScrAction(QIconSet(loadIcon("editdelete.png"), loadIcon("editdelete.png")), tr("C&lear Contents"), QKeySequence(), ScApp, "editClearContents"));
	scrActions->insert("editSelectAll", new ScrAction(tr("Select &All"), CTRL+Key_A, ScApp, "editSelectAll"));
	scrActions->insert("editDeselectAll", new ScrAction(tr("&Deselect All"), CTRL+SHIFT+Key_A, ScApp, "editDeselectAll"));
	scrActions->insert("editSearchReplace", new ScrAction(QIconSet(loadIcon("find16.png"), loadIcon("find16.png")),  tr("&Search/Replace..."), QKeySequence(CTRL+Key_F), ScApp, "editSearchReplace"));

	scrActions->insert("editColors", new ScrAction(tr("C&olors..."), QKeySequence(), ScApp, "editColors"));
	scrActions->insert("editParaStyles", new ScrAction(tr("&Paragraph Styles..."), QKeySequence(), ScApp, "editParaStyles"));
	scrActions->insert("editLineStyles", new ScrAction(tr("&Line Styles..."), QKeySequence(), ScApp, "editLineStyles"));
	scrActions->insert("editMasterPages", new ScrAction(tr("&Master Pages..."), QKeySequence(), ScApp, "editMasterPages"));
	scrActions->insert("editJavascripts", new ScrAction(tr("&Javascripts..."), QKeySequence(), ScApp, "editJavascripts"));
	scrActions->insert("editPreferences", new ScrAction(tr("P&references..."), QKeySequence(), ScApp, "editPreferences"));

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
	scrActions->insert("fontSizeOther", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Other..."), QKeySequence(), (*scrActionGroups)["fontSize"], "fontSizeOther", -1));
	connect( (*scrActions)["fontSizeOther"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemFSize(int)));

	int font_sizes[] = {7, 9, 10, 11, 12, 14, 18, 24, 36, 48, 60, 72};
	size_t f_size = sizeof(font_sizes) / sizeof(*font_sizes);
	for (uint s = 0; s < f_size; ++s)
	{
		QString fontSizeName=QString("fontSize%1").arg(font_sizes[s]);
		scrActions->insert(fontSizeName, new ScrAction(ScrAction::DataInt, QIconSet(), tr("%1 pt").arg(font_sizes[s]), QKeySequence(), (*scrActionGroups)["fontSize"], fontSizeName, font_sizes[s]));
		(*scrActions)[fontSizeName]->setToggleAction(true);
		connect( (*scrActions)[fontSizeName], SIGNAL(activatedData(int)), ScApp, SLOT(setItemFSize(int)));
	}

	//Alignment actions
	scrActions->insert("alignLeft", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Left"), QKeySequence(), ScApp, "alignLeft", 0));
	scrActions->insert("alignCenter", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Center"), QKeySequence(), ScApp, "alignCenter", 1));
	scrActions->insert("alignRight", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Right"), QKeySequence(), ScApp, "alignRight", 2));
	scrActions->insert("alignBlock", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Block"), QKeySequence(), ScApp, "alignBlock", 3));
	scrActions->insert("alignForced", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Forced"), QKeySequence(), ScApp, "alignForced", 4));

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
	scrActions->insert("shadeOther", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Other..."), QKeySequence(), (*scrActionGroups)["shade"], "shadeOther", -1));
	connect( (*scrActions)["shadeOther"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemShade(int)));
	for (uint i=0; i<=100 ; i+=10)
	{
		QString shadeName=QString("shade%1").arg(i);
		scrActions->insert(shadeName, new ScrAction(ScrAction::DataInt, QIconSet(), tr("&%1 %").arg(i), QKeySequence(), (*scrActionGroups)["shade"], shadeName, i));
		(*scrActions)[shadeName]->setToggleAction(true);
		connect( (*scrActions)[shadeName], SIGNAL(activatedData(int)), ScApp, SLOT(setItemShade(int)));
	}

	//Type Effects actions
	scrActionGroups->insert("typeEffects", new QActionGroup(ScApp, "typeEffects", false));
	scrActions->insert("typeEffectNormal", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Normal"), QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectNormal", 0));
	scrActions->insert("typeEffectUnderline", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Underline"), QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectUnderline", 1));
	scrActions->insert("typeEffectStrikeThrough", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Strike Through"), QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectStrikeThrough", 2));
	scrActions->insert("typeEffectSmallCaps", new ScrAction(ScrAction::DataInt, QIconSet(), tr("Small &Caps"), QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectSmallCaps", 3));
	scrActions->insert("typeEffectSuperscript", new ScrAction(ScrAction::DataInt, QIconSet(), tr("Su&perscript"), QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectSuperscript", 4));
	scrActions->insert("typeEffectSubscript", new ScrAction(ScrAction::DataInt, QIconSet(), tr("Su&bscript"), QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectSubscript", 5));
	scrActions->insert("typeEffectOutline", new ScrAction(ScrAction::DataInt, QIconSet(), tr("&Outline"), QKeySequence(), (*scrActionGroups)["typeEffects"], "typeEffectOutline", 6));
	(*scrActions)["typeEffectNormal"]->setToggleAction(true);
	(*scrActions)["typeEffectUnderline"]->setToggleAction(true);
	(*scrActions)["typeEffectStrikeThrough"]->setToggleAction(true);
	(*scrActions)["typeEffectSmallCaps"]->setToggleAction(true);
	(*scrActions)["typeEffectSuperscript"]->setToggleAction(true);
	(*scrActions)["typeEffectSubscript"]->setToggleAction(true);
	(*scrActions)["typeEffectOutline"]->setToggleAction(true);
	connect( (*scrActions)["typeEffectNormal"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectUnderline"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectStrikeThrough"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSmallCaps"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSuperscript"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectSubscript"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));
	connect( (*scrActions)["typeEffectOutline"], SIGNAL(activatedData(int)), ScApp, SLOT(setItemTypeStyle(int)));

	//Other Style menu items that get added in various places
	scrActions->insert("styleInvertPict", new ScrAction(tr("&Image Effects"), QKeySequence(), ScApp, "styleInvertPict"));
	scrActions->insert("styleTabulators", new ScrAction(tr("&Tabulators..."), QKeySequence(), ScApp, "styleTabulators"));
	connect( (*scrActions)["styleInvertPict"], SIGNAL(activated()), ScApp, SLOT(ImageEffects()));
	connect( (*scrActions)["styleTabulators"], SIGNAL(activated()), ScApp, SLOT(EditTabs()));

}

void ActionManager::initItemMenuActions()
{
	//Item Menu
	scrActions->insert("itemDuplicate", new ScrAction(tr("D&uplicate"), CTRL+Key_D, ScApp, "itemDuplicate"));
	scrActions->insert("itemMulDuplicate", new ScrAction(tr("&Multiple Duplicate"), QKeySequence(), ScApp, "itemMulDuplicate"));
	scrActions->insert("itemDelete", new ScrAction(tr("&Delete"), CTRL+Key_K, ScApp, "itemDelete"));
	scrActions->insert("itemGroup", new ScrAction(tr("&Group"), CTRL+Key_G, ScApp, "itemGroup"));
	scrActions->insert("itemUngroup", new ScrAction(tr("&Ungroup"), CTRL+Key_U, ScApp, "itemUngroup"));
	scrActions->insert("itemLock", new ScrAction(tr("Is &Locked"), CTRL+Key_L, ScApp, "itemLock"));
	scrActions->insert("itemLockSize", new ScrAction(tr("Si&ze is Locked"), CTRL+SHIFT+Key_L, ScApp, "itemLockSize"));
	(*scrActions)["itemLock"]->setToggleAction(true);
	(*scrActions)["itemLockSize"]->setToggleAction(true);
	scrActions->insert("itemSendToBack", new ScrAction(tr("Send to &Back"), QKeySequence(Key_End), ScApp, "itemSendToBack"));
	scrActions->insert("itemBringToFront", new ScrAction(tr("Bring to &Front"), QKeySequence(Key_Home), ScApp, "itemBringToFront"));
	scrActions->insert("itemLower", new ScrAction(tr("&Lower"), QKeySequence(Key_PageDown), ScApp, "itemLower"));
	scrActions->insert("itemRaise", new ScrAction(tr("&Raise"), QKeySequence(Key_PageUp), ScApp, "itemRaise"));
	scrActions->insert("itemAlignDist", new ScrAction(tr("Distribute/&Align..."), QKeySequence(), ScApp, "itemAlignDist"));
	scrActions->insert("itemSendToScrapbook", new ScrAction(tr("Send to S&crapbook"), QKeySequence(), ScApp, "itemSendToScrapbook"));
	
	scrActions->insert("itemAttributes", new ScrAction(tr("&Attributes..."), QKeySequence(), ScApp, "itemAttributes"));
	scrActions->insert("itemImageIsVisible", new ScrAction(tr("I&mage Visible"), QKeySequence(), ScApp, "itemImageIsVisible"));
	scrActions->insert("itemPDFIsBookmark", new ScrAction(tr("Is PDF &Bookmark"), QKeySequence(), ScApp, "itemPDFIsBookmark"));
	(*scrActions)["itemPDFIsBookmark"]->setToggleAction(true);
	scrActions->insert("itemPDFIsAnnotation", new ScrAction(tr("Is PDF A&nnotation"), QKeySequence(), ScApp, "itemPDFIsAnnotation"));
	(*scrActions)["itemPDFIsAnnotation"]->setToggleAction(true);
	scrActions->insert("itemPDFAnnotationProps", new ScrAction(tr("Annotation P&roperties"), QKeySequence(), ScApp, "itemPDFAnnotationProps"));
	scrActions->insert("itemPDFFieldProps", new ScrAction(tr("Field P&roperties"), QKeySequence(), ScApp, "itemPDFFieldProps"));
	
	//CB TODO Make these work
	(*scrActions)["itemPDFIsBookmark"]->setEnabled(false);
	(*scrActions)["itemPDFIsAnnotation"]->setEnabled(false);
	(*scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
	(*scrActions)["itemPDFFieldProps"]->setEnabled(false);
	
	(*scrActions)["itemImageIsVisible"]->setToggleAction(true);
		
	scrActions->insert("itemShapeEdit", new ScrAction(tr("&Edit Shape..."), QKeySequence(), ScApp, "itemShapeEdit"));
	(*scrActions)["itemShapeEdit"]->setToggleAction(true);
	scrActions->insert("itemAttachTextToPath", new ScrAction(tr("&Attach Text to Path"), QKeySequence(), ScApp, "itemAttachTextToPath"));
	scrActions->insert("itemDetachTextFromPath", new ScrAction(tr("&Detach Text from Path"), QKeySequence(), ScApp, "itemDetachTextFromPath"));
	scrActions->insert("itemCombinePolygons", new ScrAction(tr("&Combine Polygons"), QKeySequence(), ScApp, "itemCombinePolygons"));
	scrActions->insert("itemSplitPolygons", new ScrAction(tr("Split &Polygons"), QKeySequence(), ScApp, "itemSplitPolygons"));
	scrActions->insert("itemConvertToBezierCurve", new ScrAction(tr("&Bezier Curve"), QKeySequence(), ScApp, "itemConvertToBezierCurve"));
	scrActions->insert("itemConvertToImageFrame", new ScrAction(tr("&Image Frame"), QKeySequence(), ScApp, "itemConvertToImageFrame"));
	scrActions->insert("itemConvertToOutlines", new ScrAction(tr("&Outlines"), QKeySequence(), ScApp, "itemConvertToOutlines"));
	scrActions->insert("itemConvertToPolygon", new ScrAction(tr("&Polygon"), QKeySequence(), ScApp, "itemConvertToPolygon"));
	scrActions->insert("itemConvertToTextFrame", new ScrAction(tr("&Text Frame"), QKeySequence(), ScApp, "itemConvertToTextFrame"));

	connect( (*scrActions)["itemDuplicate"], SIGNAL(activated()), ScApp, SLOT(ObjektDup()) );
	connect( (*scrActions)["itemMulDuplicate"], SIGNAL(activated()), ScApp, SLOT(ObjektDupM()) );
	connect( (*scrActions)["itemDelete"], SIGNAL(activated()), ScApp, SLOT(DeleteObjekt()) );
	connect( (*scrActions)["itemGroup"], SIGNAL(activated()), ScApp, SLOT(GroupObj()) );
	connect( (*scrActions)["itemUngroup"], SIGNAL(activated()), ScApp, SLOT(UnGroupObj()) );
	connect( (*scrActions)["itemLock"], SIGNAL(activated()), ScApp, SLOT(ToggleObjLock()) );
	connect( (*scrActions)["itemLockSize"], SIGNAL(activated()), ScApp, SLOT(ToggleObjSizeLock()) );
	connect( (*scrActions)["itemPDFIsAnnotation"], SIGNAL(activated()), ScApp, SLOT(ToggleObjPDFAnnotation()) );
	connect( (*scrActions)["itemPDFIsBookmark"], SIGNAL(activated()), ScApp, SLOT(ToggleObjPDFBookmark()) );
	connect( (*scrActions)["itemPDFAnnotationProps"], SIGNAL(activated()), ScApp, SLOT(ModifyAnnot()) );
	connect( (*scrActions)["itemPDFFieldProps"], SIGNAL(activated()), ScApp, SLOT(ModifyAnnot()) );
	//connect( (*scrActions)["itemSendToBack"], SIGNAL(activated()), ScApp, SLOT(Objekt2Back()) );
	//connect( (*scrActions)["itemBringToFront"], SIGNAL(activated()), ScApp, SLOT(Objekt2Front()) );
	//connect( (*scrActions)["itemLower"], SIGNAL(activated()), ScApp, SLOT(ObjektLower()) );
	//connect( (*scrActions)["itemRaise"], SIGNAL(activated()), ScApp, SLOT(ObjektRaise()) );
	connect( (*scrActions)["itemAlignDist"], SIGNAL(activated()), ScApp, SLOT(ObjektAlign()) );
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
	scrActions->insert("insertGlyph", new ScrAction(tr("&Glyph..."), QKeySequence(), ScApp, "insertGlyph"));
	//(*scrActions)["insertGlyph"]->setText(tr("Insert Special Character"));
	scrActions->insert("insertSampleText", new ScrAction(tr("Sample Text"), QKeySequence(), ScApp, "insertSampleText"));
	
	connect( (*scrActions)["insertGlyph"], SIGNAL(activated()), ScApp, SLOT(slotCharSelect()) );
	connect( (*scrActions)["insertSampleText"], SIGNAL(activated()), ScApp, SLOT(insertSampleText()) );
}

void ActionManager::initPageMenuActions()
{
	//Page menu
	scrActions->insert("pageInsert", new ScrAction(tr("&Insert..."), QKeySequence(), ScApp, "pageInsert"));
	scrActions->insert("pageImport", new ScrAction(tr("Im&port..."), QKeySequence(), ScApp, "pageImport"));
	scrActions->insert("pageDelete", new ScrAction(tr("&Delete..."), QKeySequence(), ScApp, "pageDelete"));
	scrActions->insert("pageCopy", new ScrAction(tr("&Copy..."), QKeySequence(), ScApp, "pageCopy"));
	scrActions->insert("pageMove", new ScrAction(tr("&Move..."), QKeySequence(), ScApp, "pageMove"));
	scrActions->insert("pageApplyMasterPage", new ScrAction(tr("&Apply Master Page..."), QKeySequence(), ScApp, "pageApplyMasterPage"));
	scrActions->insert("pageManageGuides", new ScrAction(tr("Manage &Guides..."), QKeySequence(), ScApp, "pageManageGuides"));

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
	scrActions->insert("viewFitInWindow", new ScrAction(ScrAction::DataDouble, QIconSet(), tr("&Fit in window"), CTRL+Key_0, ScApp, "viewFitInWindow", 0, -100.0));
	scrActions->insert("viewFit50", new ScrAction(ScrAction::DataDouble, QIconSet(), tr("&50%"), QKeySequence(), ScApp, "viewFit50", 0, 50.0));
	scrActions->insert("viewFit75", new ScrAction(ScrAction::DataDouble, QIconSet(), tr("&75%"), QKeySequence(), ScApp, "viewFit75", 0, 75.0));
	scrActions->insert("viewFit100", new ScrAction(ScrAction::DataDouble, QIconSet(), tr("&100%"), CTRL+Key_1, ScApp, "viewFit100", 0, 100.0));
	scrActions->insert("viewFit200", new ScrAction(ScrAction::DataDouble, QIconSet(), tr("&200%"), QKeySequence(), ScApp, "viewFit200", 0, 200.0));
	scrActions->insert("viewFit20", new ScrAction(ScrAction::DataDouble, QIconSet(), tr("&Thumbnails"), QKeySequence(), ScApp, "viewFit20", 0, 20.0));
	scrActions->insert("viewShowMargins", new ScrAction(tr("Show &Margins"), QKeySequence(), ScApp, "viewShowMargins"));
	scrActions->insert("viewShowFrames", new ScrAction(tr("Show &Frames"), QKeySequence(), ScApp, "viewShowFrames"));
	scrActions->insert("viewShowImages", new ScrAction(tr("Show &Images"), QKeySequence(), ScApp, "viewShowImages"));
	scrActions->insert("viewShowGrid", new ScrAction(tr("Show &Grid"), QKeySequence(), ScApp, "viewShowGrid"));
	scrActions->insert("viewShowGuides", new ScrAction(tr("Show G&uides"), QKeySequence(), ScApp, "viewShowGuides"));
	scrActions->insert("viewShowBaseline", new ScrAction(tr("Show &Baseline Grid"), QKeySequence(), ScApp, "viewShowBaseline"));
	scrActions->insert("viewShowTextChain", new ScrAction(tr("Show &Text Chain"), QKeySequence(), ScApp, "viewShowTextChain"));
	scrActions->insert("viewSnapToGrid", new ScrAction(tr("Sn&ap to Grid"), QKeySequence(), ScApp, "viewSnapToGrid"));
	scrActions->insert("viewSnapToGuides", new ScrAction(tr("Sna&p to Guides"), QKeySequence(), ScApp, "viewSnapToGuides"));
//	scrActions->insert("viewNewView", new ScrAction(tr("New View"), QKeySequence(), ScApp, "viewNewView"));

	(*scrActions)["viewShowMargins"]->setToggleAction(true);
	(*scrActions)["viewShowFrames"]->setToggleAction(true);
	(*scrActions)["viewShowImages"]->setToggleAction(true);
	(*scrActions)["viewShowGrid"]->setToggleAction(true);
	(*scrActions)["viewShowGuides"]->setToggleAction(true);
	(*scrActions)["viewShowBaseline"]->setToggleAction(true);
	(*scrActions)["viewShowTextChain"]->setToggleAction(true);
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
	connect( (*scrActions)["viewSnapToGrid"], SIGNAL(activated()), ScApp, SLOT(ToggleURaster()) );
	connect( (*scrActions)["viewSnapToGuides"], SIGNAL(activated()), ScApp, SLOT(ToggleUGuides()) );
//	connect( (*scrActions)["viewNewView"], SIGNAL(activated()), ScApp, SLOT(newView()) );

}

void ActionManager::initToolsMenuActions()
{
	//Tool menu
	scrActions->insert("toolsProperties", new ScrAction(tr("&Properties"), QKeySequence(), ScApp, "toolsProperties"));
	scrActions->insert("toolsOutline", new ScrAction(tr("&Outline"), QKeySequence(), ScApp, "toolsOutline"));
	scrActions->insert("toolsScrapbook", new ScrAction(tr("&Scrapbook"), QKeySequence(), ScApp, "toolsScrapbook"));
	scrActions->insert("toolsLayers", new ScrAction(tr("&Layers"), QKeySequence(Key_F6), ScApp, "toolsLayers"));
	scrActions->insert("toolsPages", new ScrAction(tr("P&age Palette"), QKeySequence(), ScApp, "toolsPages"));
	scrActions->insert("toolsBookmarks", new ScrAction(tr("&Bookmarks"), QKeySequence(), ScApp, "toolsBookmarks"));
	scrActions->insert("toolsMeasurements", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("dist.png"), loadIcon("dist.png")), tr("&Measurements"), QKeySequence(), ScApp, "toolsMeasurements", MeasurementTool));
	scrActions->insert("toolsActionHistory", new ScrAction(tr("Action &History"), QKeySequence(), ScApp, "toolsActionHistory"));
	scrActions->insert("toolsPreflightVerifier", new ScrAction(QIconSet(loadIcon("launch16.png"), loadIcon("launch.png")),tr("Preflight &Verifier"), QKeySequence(), ScApp, "toolsPreflightVerifier"));
	scrActions->insert("toolsToolbarTools", new ScrAction(tr("&Tools"), QKeySequence(), ScApp, "toolsToolbarTools"));
	scrActions->insert("toolsToolbarPDF", new ScrAction(tr("P&DF Tools"), QKeySequence(), ScApp, "toolsToolbarPDF"));
	
	//toolbar only items
	scrActions->insert("toolsSelect", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Kreuz.xpm"), loadIcon("Kreuz.xpm")), tr("Select Item"), QKeySequence(Key_C), ScApp, "toolsSelect", NormalMode));
	
	scrActions->insert("toolsInsertTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Text16.xpm"), loadIcon("Text.xpm")), tr("&Text Frame"), QKeySequence(Key_T), ScApp, "toolsInsertTextFrame", DrawText));
	scrActions->insert("toolsInsertImageFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Bild16.xpm"), loadIcon("Bild.xpm")), tr("&Image Frame"), QKeySequence(Key_I), ScApp, "toolsInsertImageFrame", DrawPicture));
	scrActions->insert("toolsInsertTableFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("frame_table16.png"), loadIcon("frame_table.png")), tr("T&able"), QKeySequence(Key_A), ScApp, "toolsInsertTableFrame", DrawTable));
	scrActions->insert("toolsInsertShape", new ScrAction(ScrAction::DataInt,QIconSet(), tr("&Shape"), QKeySequence(Key_S), ScApp, "toolsInsertShape", DrawShapes));
	scrActions->insert("toolsInsertPolygon", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("spline16.png"), loadIcon("spline.png")), tr("&Polygon"), QKeySequence(Key_P), ScApp, "toolsInsertPolygon", DrawRegularPolygon));
	scrActions->insert("toolsInsertLine", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Stift16.xpm"), loadIcon("Stift.xpm")), tr("&Line"), QKeySequence(Key_L), ScApp, "toolsInsertLine", DrawLine));
	scrActions->insert("toolsInsertBezier", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("beziertool16.png"), loadIcon("beziertool.png")), tr("&Bezier Curve"), QKeySequence(Key_B), ScApp, "toolsInsertBezier", DrawBezierLine));
	scrActions->insert("toolsInsertFreehandLine", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Stiftalt16.xpm"), loadIcon("Stiftalt.xpm")), tr("&Freehand Line"), QKeySequence(Key_F), ScApp, "toolsInsertFreehandLine", DrawFreehandLine));
		
	(*scrActions)["toolsInsertTextFrame"]->setText(tr("Insert Text Frame"));
	(*scrActions)["toolsInsertImageFrame"]->setText(tr("Insert Image Frame"));
	(*scrActions)["toolsInsertTableFrame"]->setText(tr("Insert Table"));
	(*scrActions)["toolsInsertShape"]->setText(tr("Insert Shape"));
	(*scrActions)["toolsInsertPolygon"]->setText(tr("Insert Polygon"));
	(*scrActions)["toolsInsertLine"]->setText(tr("Insert Line"));
	(*scrActions)["toolsInsertBezier"]->setText(tr("Insert Bezier Curve"));
	(*scrActions)["toolsInsertFreehandLine"]->setText(tr("Insert Freehand Line"));
		
	scrActions->insert("toolsRotate", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Rotieren.xpm"), loadIcon("Rotieren.xpm")), tr("Rotate Item"), QKeySequence(Key_R), ScApp, "toolsRotate", Rotation));
	scrActions->insert("toolsZoom", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Lupe.xpm"), loadIcon("Lupe.xpm")), tr("Zoom in or out"), QKeySequence(Key_Z), ScApp, "toolsZoom", Magnifier));
	scrActions->insert("toolsZoomIn", new ScrAction(QIconSet(loadIcon("Gross.xpm"), loadIcon("Gross.xpm")), tr("Zoom in"), QKeySequence(CTRL+Key_Plus), ScApp, "toolsZoomIn"));
	scrActions->insert("toolsZoomOut", new ScrAction(QIconSet(loadIcon("Klein.xpm"), loadIcon("Klein.xpm")), tr("Zoom out"), QKeySequence(CTRL+Key_Minus), ScApp, "toolsZoomOut"));
	scrActions->insert("toolsEditContents", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Editm.xpm"), loadIcon("Editm.xpm")), tr("Edit Contents of Frame"), QKeySequence(Key_E), ScApp, "toolsEditContents", EditMode));
	scrActions->insert("toolsEditWithStoryEditor", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("signature.png"), loadIcon("signature.png")), tr("Text..."), QKeySequence(CTRL+Key_Y), ScApp, "toolsEditWithStoryEditor", StartStoryEditor));
	scrActions->insert("toolsLinkTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Lock.xpm"), loadIcon("Lock.xpm")), tr("Link Text Frames"), QKeySequence(Key_N), ScApp, "toolsLinkTextFrame", LinkFrames));
	scrActions->insert("toolsUnlinkTextFrame", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("Unlock.xpm"), loadIcon("Unlock.xpm")), tr("Unlink Text Frames"), QKeySequence(Key_U), ScApp, "toolsUnlinkTextFrame", UnlinkFrames));
	scrActions->insert("toolsEyeDropper", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("colorpicker.png"), loadIcon("colorpicker.png")), tr("&Eye Dropper"), QKeySequence(Key_Y), ScApp, "toolsEyeDropper", EyeDropper));
	scrActions->insert("toolsCopyProperties", new ScrAction(ScrAction::DataInt,QIconSet(loadIcon("wizard.png"), loadIcon("wizard.png")), tr("Copy Item Properties"), QKeySequence(), ScApp, "toolsCopyProperties", CopyProperties));
	
	(*scrActions)["toolsEditWithStoryEditor"]->setText(tr("Edit the text with the Story Editor"));

	(*scrActions)["toolsProperties"]->setToggleAction(true);
	(*scrActions)["toolsOutline"]->setToggleAction(true);
	(*scrActions)["toolsScrapbook"]->setToggleAction(true);
	(*scrActions)["toolsLayers"]->setToggleAction(true);
	(*scrActions)["toolsPages"]->setToggleAction(true);
	(*scrActions)["toolsBookmarks"]->setToggleAction(true);
	(*scrActions)["toolsMeasurements"]->setToggleAction(true);
	(*scrActions)["toolsActionHistory"]->setToggleAction(true);
	(*scrActions)["toolsPreflightVerifier"]->setToggleAction(true);
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

	*nonEditActionNames << "itemSendToBack" << "itemBringToFront" << "itemRaise" << "itemLower";
	
	connect( (*scrActions)["toolsActionHistory"], SIGNAL(toggled(bool)), ScApp, SLOT(setUndoPalette(bool)) );
	connect( (*scrActions)["toolsToolbarTools"], SIGNAL(toggled(bool)), ScApp, SLOT(setTools(bool)) );
	connect( (*scrActions)["toolsToolbarPDF"], SIGNAL(toggled(bool)), ScApp, SLOT(setPDFTools(bool)) );
		
	connectModeActions();
}

void ActionManager::initExtrasMenuActions()
{
	scrActions->insert("extrasManagePictures", new ScrAction(tr("&Manage Pictures"), QKeySequence(), ScApp, "extrasManagePictures"));
	scrActions->insert("extrasHyphenateText", new ScrAction(tr("&Hyphenate Text"), QKeySequence(), ScApp, "extrasHyphenateText"));
	scrActions->insert("extrasGenerateTableOfContents", new ScrAction(tr("&Generate Table Of Contents"), QKeySequence(), ScApp, "extrasGenerateTableOfContents"));

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
	scrActions->insert("helpAboutScribus", new ScrAction(tr("&About Scribus"), QKeySequence(), ScApp, "helpAboutScribus"));
	scrActions->insert("helpAboutQt", new ScrAction(tr("About &Qt"), QKeySequence(), ScApp, "helpAboutQt"));
	scrActions->insert("helpTooltips", new ScrAction(tr("Toolti&ps"), QKeySequence(), ScApp, "helpTooltips"));
	scrActions->insert("helpManual", new ScrAction(tr("Scribus &Manual..."), Key_F1, ScApp, "helpManual"));

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
	scrActions->insert("specialSmartHyphen", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Smart &Hyphen"), CTRL+SHIFT+Key_Minus, ScApp, "specialSmartHyphen",-1));
	scrActions->insert("specialNonBreakingSpace", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Non Breaking &Space"), CTRL+Key_Space, ScApp, "specialNonBreakingSpace",29));
	scrActions->insert("specialPageNumber", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Page &Number"), CTRL+SHIFT+ALT+Key_P, ScApp, "specialPageNumber",30));
	scrActions->insert("specialCopyRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Copyright"), QKeySequence(), ScApp, "specialCopyRight",0x0A9));
	scrActions->insert("specialRegdTM", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Registered Trademark"), QKeySequence(), ScApp, "specialRegdTM",0x00AE));
	scrActions->insert("specialTM", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Trademark"), QKeySequence(), ScApp, "specialTM",0x2122));
	scrActions->insert("specialBullet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Bullet"), QKeySequence(), ScApp, "specialBullet",0x2022));
	scrActions->insert("specialDashEm", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Em Dash"), QKeySequence(), ScApp, "specialDashEm",0x2014));
	scrActions->insert("specialDashEn", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("En Dash"), QKeySequence(), ScApp, "specialDashEn",0x2013));
	scrActions->insert("specialDashFigure", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Figure Dash"), QKeySequence(), ScApp, "specialDashFigure",0x2012));
	scrActions->insert("specialDashQuotation", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Quotation Dash"), QKeySequence(), ScApp, "specialDashQuotation",0x2015));
	
	scrActions->insert("specialQuoteApostrophe", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Apostrophe"), QKeySequence(), ScApp, "specialQuoteApostrophe",0x0027));
	scrActions->insert("specialQuoteStraight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Straight Double"), QKeySequence(), ScApp, "specialQuoteStraight",0x0022));
	scrActions->insert("specialQuoteSingleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Single Left"), QKeySequence(), ScApp, "specialQuoteSingleLeft",0x2018));
	scrActions->insert("specialQuoteSingleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Single Right"), QKeySequence(), ScApp, "specialQuoteSingleRight",0x2019));
	scrActions->insert("specialQuoteDoubleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Double Left"), QKeySequence(), ScApp, "specialQuoteDoubleLeft",0x201C));
	scrActions->insert("specialQuoteDoubleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Double Right"), QKeySequence(), ScApp, "specialQuoteDoubleRight",0x201D));
	scrActions->insert("specialQuoteSingleReversed", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Single Reversed"), QKeySequence(), ScApp, "specialQuoteSingleReversed",0x201B));
	scrActions->insert("specialQuoteDoubleReversed", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Double Reversed"), QKeySequence(), ScApp, "specialQuoteDoubleReversed",0x201F));
	scrActions->insert("specialQuoteSingleLeftGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Single Left Guillemet"), QKeySequence(), ScApp, "specialQuoteSingleLeftGuillemet",0x2039));
	scrActions->insert("specialQuoteSingleRightGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Single Right Guillemet"), QKeySequence(), ScApp, "specialQuoteSingleRightGuillemet",0x203A));
	scrActions->insert("specialQuoteDoubleLeftGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Double Left Guillemet"), QKeySequence(), ScApp, "specialQuoteDoubleLeftGuillemet",0x00AB));
	scrActions->insert("specialQuoteDoubleRightGuillemet", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Double Right Guillemet"), QKeySequence(), ScApp, "specialQuoteDoubleRightGuillemet",0x00BB));
	scrActions->insert("specialQuoteLowSingleComma", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Low Single Comma"), QKeySequence(), ScApp, "specialQuoteLowSingleComma",0x201A));
	scrActions->insert("specialQuoteLowDoubleComma", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Low Double Comma"), QKeySequence(), ScApp, "specialQuoteLowDoubleComma",0x201E));
	scrActions->insert("specialQuoteDoubleTurnedComma", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("Double Turned Comma"), QKeySequence(), ScApp, "specialQuoteDoubleTurnedComma",0x201C));
	scrActions->insert("specialQuoteCJKSingleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("CJK Single Left"), QKeySequence(), ScApp, "specialQuoteCJKSingleLeft",0x300C));
	scrActions->insert("specialQuoteCJKSingleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("CJK Single Right"), QKeySequence(), ScApp, "specialQuoteCJKSingleRight",0x300D));
	scrActions->insert("specialQuoteCJKDoubleLeft", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("CJK Double Left"), QKeySequence(), ScApp, "specialQuoteCJKDoubleLeft",0x300E));
	scrActions->insert("specialQuoteCJKDoubleRight", new ScrAction(ScrAction::UnicodeChar, QIconSet(), tr("CJK Double Right"), QKeySequence(), ScApp, "specialQuoteCJKDoubleRight",0x300F));
	
	(*scrActions)["specialSmartHyphen"]->setText("Insert Smart Hyphen");
	(*scrActions)["specialNonBreakingSpace"]->setText("Insert Non Breaking Space");
	(*scrActions)["specialPageNumber"]->setText("Insert Page Number");

	*unicodeCharActionNames << "specialSmartHyphen" << "specialNonBreakingSpace" << "specialPageNumber";
	*unicodeCharActionNames << "specialCopyRight" << "specialRegdTM" << "specialTM";
	*unicodeCharActionNames << "specialBullet";
	*unicodeCharActionNames << "specialDashEm" << "specialDashEn" << "specialDashFigure" << "specialDashQuotation";
	*unicodeCharActionNames << "specialQuoteApostrophe" << "specialQuoteStraight";
	*unicodeCharActionNames << "specialQuoteDoubleLeft" << "specialQuoteDoubleRight" << "specialQuoteSingleLeft" << "specialQuoteSingleRight";
	*unicodeCharActionNames << "specialQuoteSingleReversed" << "specialQuoteDoubleReversed";
	*unicodeCharActionNames << "specialQuoteSingleLeftGuillemet" << "specialQuoteSingleRightGuillemet" << "specialQuoteDoubleLeftGuillemet" << "specialQuoteDoubleRightGuillemet";
	*unicodeCharActionNames << "specialQuoteLowSingleComma" << "specialQuoteLowDoubleComma" << "specialQuoteDoubleTurnedComma";
	*unicodeCharActionNames << "specialQuoteCJKSingleLeft" << "specialQuoteCJKSingleRight" << "specialQuoteCJKDoubleLeft" << "specialQuoteCJKDoubleRight";
	
	for ( QStringList::Iterator it = unicodeCharActionNames->begin(); it != unicodeCharActionNames->end(); ++it )
		connect( (*scrActions)[*it], SIGNAL(activatedUnicodeShortcut(QString, int)), ScApp, SLOT(specialActionKeyEvent(QString, int)) );
	enableUnicodeActions(false);

	//GUI
	scrActions->insert("specialToggleAllPalettes", new ScrAction(ScrAction::DataQString, QIconSet(), tr("Toggle Palettes"), Key_F10, ScApp, "specialToggleAllPalettes",0,0.0,"specialToggleAllPalettes"));
	scrActions->insert("specialToggleAllGuides", new ScrAction(ScrAction::DataQString, QIconSet(), tr("Toggle Guides"), Key_F11, ScApp, "specialToggleAllGuides",0,0.0,"specialToggleAllGuides"));

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
	disconnect( (*scrActions)["itemSendToBack"], 0, 0, 0);
	disconnect( (*scrActions)["itemImageIsVisible"], 0, 0, 0);
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
}
	
void ActionManager::connectNewViewActions(ScribusView *currView)
{
	if (currView==NULL)
		return;
	connect( (*scrActions)["toolsZoomIn"], SIGNAL(activated()) , currView, SLOT(slotZoomIn()) );
	connect( (*scrActions)["toolsZoomOut"], SIGNAL(activated()) , currView, SLOT(slotZoomOut()) );
	connect( (*scrActions)["itemSendToBack"], SIGNAL(activated()), currView, SLOT(ToBack()) );
	connect( (*scrActions)["itemBringToFront"], SIGNAL(activated()), currView, SLOT(ToFront()) );
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
}

void ActionManager::disconnectNewSelectionActions()
{
	disconnect( (*scrActions)["itemImageIsVisible"], 0, 0, 0);
}

void ActionManager::connectNewSelectionActions(ScribusView *currView)
{
	connect( (*scrActions)["itemImageIsVisible"], SIGNAL(toggled(bool)) , currView, SLOT(TogglePic()) );
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
