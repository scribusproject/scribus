/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Jan 2010
	copyright            : (C) 2010 by Craig Bradney
	email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QDebug>

#include "actionmanager.h"
#include "ui/scmwmenumanager.h"

ScMWMenuManager::ScMWMenuManager(QMenuBar* mb, QObject *parent) :
	MenuManager(mb, parent)
{
}

void ScMWMenuManager::languageChange()
{

	//Important for TRANSLATORS:
	//keep the same exact translations as the ones already translated. This function is
	//used when UI language is changed and updates main menus and sub-menus strings.
	//see void ScribusMainWindow::initMenuBar() or ScribusMainWindow context in qt linguist

	setText("File", ActionManager::defaultMenuNameEntryTranslated("File"));
	setText("FileOpenRecent", tr("Open &Recent"));
	setText("EditPasteRecent", tr("Paste Recent"));
	setText("FileImport", tr("&Import"));
	setText("FileExport", tr("&Export"));
	setText("Edit", ActionManager::defaultMenuNameEntryTranslated("Edit"));
	setText("EditContents", tr("Contents"));
	setText("Item", ActionManager::defaultMenuNameEntryTranslated("Item"));
	setText("DuplicateTransform", tr("Duplicate/Transform"));
	setText("Grouping", tr("Grouping"));
	setText("Locking", tr("Locking"));
	setText("ItemLevel", tr("&Level"));
	setText("ItemLayer", tr("Send to Layer"));
	setText("SendTo", tr("Send to"));
	setText("ItemSendToScrapbook", tr("Scrapbook"));
	setText("Adjust", tr("Adjust"));
	setText("Image", tr("Image"));
	setText("ItemPreviewSettings", tr("Previe&w Settings"));
	setText("ItemPDFOptions", tr("&PDF Options"));
	setText("ItemConvertTo", tr("C&onvert to"));
	setText("TextLinking", tr("Text Frame Links"));
//			setText("ItemPathOps", tr("Path Tools"));
	setText("Weld", tr("Welding"));
	setText("Insert", ActionManager::defaultMenuNameEntryTranslated("Insert"));
	setText("InsertChar", tr("Character"));
	setText("InsertQuote", tr("Quote"));
	setText("InsertSpace", tr("Space"));
	setText("InsertLigature", tr("Liga&ture"));
	setText("InsertMark", tr("Marks"));
	setText("Page", ActionManager::defaultMenuNameEntryTranslated("Page"));
	setText("ItemTable", ActionManager::defaultMenuNameEntryTranslated("Table"));
	setText("Extras", ActionManager::defaultMenuNameEntryTranslated("Extras"));
	setText("View", ActionManager::defaultMenuNameEntryTranslated("View"));
	setText("ViewZoom", tr("Zoom"));
	setText("ViewPreview", tr("Preview"));
	setText("ViewMeasuring", tr("Measurement"));
	setText("ViewTextFrames", tr("Text Frames"));
	setText("ViewImageFrames", tr("Image Frames"));
	setText("ViewDocument", tr("Document"));
	setText("ViewGrids", tr("Grids and Guides"));
	setText("Windows", ActionManager::defaultMenuNameEntryTranslated("Windows"));
	setText("Help", ActionManager::defaultMenuNameEntryTranslated("Help"));
	setText("Alignment", tr("&Alignment"));
	setText("HelpOnlineTutorials", tr("Online &Tutorials"));
	MenuManager::languageChange();
}
