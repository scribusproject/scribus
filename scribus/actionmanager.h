/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Jan 2005
    copyright            : (C) 2005 by Craig Bradney
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
#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <qobject.h> 
#include <qstring.h>
#include <qmap.h>
#include <qguardedptr.h>
#include <qdict.h>

#include "scribusapi.h"
#include "scraction.h"

class ScribusDoc;
class ScribusMainWindow;
class ScribusQApp;
class ScribusView;
class UndoManager;
/**
@author Craig Bradney
*/
class SCRIBUS_API ActionManager : public QObject
{
	Q_OBJECT

	friend class StoryEditor;
	public:
		ActionManager ( QObject * parent, const char * name );	
		~ActionManager() {};
		
		void createActions();
		void disconnectModeActions();
		void connectModeActions();
		void disconnectNewViewActions();
		void connectNewViewActions(ScribusView *);
		void disconnectNewDocActions();
		void connectNewDocActions(ScribusDoc *);
		void disconnectNewSelectionActions();
		void connectNewSelectionActions(ScribusView *,ScribusDoc *);
		void saveActionShortcutsPreEditMode();
		void restoreActionShortcutsPostEditMode();
		void enableActionStringList(QStringList *list, bool enabled, bool checkingUnicode=false);
		void enableUnicodeActions(bool enabled);
		void setPDFActions(ScribusView *);
		
	public slots:
		void languageChange();
		
	protected:
		void initFileMenuActions();
		void initEditMenuActions();
		void initStyleMenuActions();
		void initItemMenuActions();
		void initInsertMenuActions();
		void initPageMenuActions();
		void initViewMenuActions();
		void initToolsMenuActions();
		void initExtrasMenuActions();
		void initWindowsMenuActions();
		void initScriptMenuActions();
		void initHelpMenuActions();
		static void initUnicodeActions(QMap<QString, QGuardedPtr<ScrAction> > *actionMap, QWidget *actionParent, QStringList *actionNamesList);
		void initSpecialActions();
		static void languageChangeUnicodeActions(QMap<QString, QGuardedPtr<ScrAction> > *actionMap);
	
		QPixmap noIcon;
		ScribusMainWindow *ScMW;
		ScribusQApp *ScQApp;
		UndoManager *undoManager;
		QMap<QString, QGuardedPtr<ScrAction> > *scrActions;
		QDict<QActionGroup> *scrActionGroups;
		QStringList *modeActionNames;
		QStringList *nonEditActionNames;
		QStringList *unicodeCharActionNames;
		
};

#endif
