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
#include <qpointer.h>
#include <q3dict.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3ActionGroup>

#include "scribusapi.h"
#include "scraction.h"

class ScribusApp;
class ScribusQApp;
class ScribusView;
class UndoManager;
/**
@author Craig Bradney
*/
class SCRIBUS_API ActionManager : public QObject
{
	Q_OBJECT

	public:
		ActionManager ( QObject * parent, const char * name );	
		~ActionManager() {};
		
		void createActions();
		void disconnectModeActions();
		void connectModeActions();
		void disconnectNewViewActions();
		void connectNewViewActions(ScribusView *);
		void disconnectNewSelectionActions();
		void connectNewSelectionActions(ScribusView *);
		void saveActionShortcutsPreEditMode();
		void restoreActionShortcutsPostEditMode();
		void enableActionStringList(QStringList *list, bool enabled, bool checkingUnicode=false);
		void enableUnicodeActions(bool enabled);
		void setPDFActions(ScribusView *);
		
	public slots:
		void languageChange();
		
	private:
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
		void initSpecialActions();
	
		QPixmap noIcon;
		ScribusApp *ScApp;
		ScribusQApp *ScQApp;
		UndoManager *undoManager;
		QMap<QString, QPointer<ScrAction> > *scrActions;
		Q3Dict<Q3ActionGroup> *scrActionGroups;
		QStringList *modeActionNames;
		QStringList *nonEditActionNames;
		QStringList *unicodeCharActionNames;
		
};

#endif
