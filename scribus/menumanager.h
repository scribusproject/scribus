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
#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <qobject.h>
#include <qaction.h>
#include <qpopupmenu.h>
#include <qstringlist.h>
#include <qpoint.h>
#include "scmenu.h"

/**
@author Craig Bradney
*/
class MenuManager
{
public:
	MenuManager(QMenuBar *scrMenuBar, QObject *parent = 0, const char *name = 0);
	~MenuManager();

	enum MenuType {Normal, DLL};
		
	bool createMenu(const QString menuName, const QString menuText, const QString parent = QString::null);
	bool addMenuToMenu(const QString child, const QString parent);
	bool deleteMenu(const QString menuName, const QString parent = QString::null);
	bool clearMenu(const QString menuName);
	QPopupMenu *getLocalPopupMenu(const QString menuName);
	void setMenuEnabled(const QString menuName, const bool enabled);
	bool addMenuToMenuBar(const QString menuName);
	bool addMenuToMenuBarAfter(const QString, const QString);
	bool removeMenuFromMenuBar(const QString menuName);
	bool addMenuToWidgetOfAction(const QString menuName, ScrAction *action);
	
	bool addMenuItem(ScrAction *menuAction, const QString parent);
	bool addMenuItem(QWidget * widget, const QString parent);
	bool addMenuItemAfter(ScrAction *menuAction, const QString parent, ScrAction *afterMenuAction);
	bool addMenuItemAfter(ScrAction *menuAction, const QString parent, const QString afterMenuName);
	bool addMenuSeparator(const QString parent);
	
	bool removeMenuItem(ScrAction *menuAction, const QString parent);
	
	void runMenuAtPos(const QString, const QPoint);

private:
	QMenuBar *scribusMenuBar;
	QMap<QString, ScrPopupMenu *> menuList;
};

#endif

