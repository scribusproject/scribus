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
#ifndef SCMENU_H
#define SCMENU_H

#include <qaction.h>
#include <qguardedptr.h>

class QPopupMenu;
class ScrAction;

#include "scribusapi.h"

class SCRIBUS_API ScrPopupMenu : public QObject
{
	Q_OBJECT
	public:
		ScrPopupMenu ( QWidget * parent = 0, const char * name = 0, const QString pMenuName = QString::null, const QString pMenuText = QString::null, const QString parentName = QString::null);
		~ScrPopupMenu ();
		
		const QString getMenuName();
		const QString getParentMenuName();
		const QString getMenuText();
		void setMenuText(const QString pMenuText);
		void setMenuBarID(int id);
		int getMenuBarID();
		void setEnabled(bool menuEnabled);
		QPopupMenu *getLocalPopupMenu();
		bool repopulateLocalMenu();
		bool clear();
		
		bool insertSubMenu(ScrPopupMenu *newSubMenu);
		bool insertMenuItem(ScrAction *newMenuAction);
		bool insertMenuItem(QWidget *widget);
		bool insertMenuItemAfter(ScrAction *newMenuAction, ScrAction *afterMenuAction);
		//bool insertMenuItemAfter(ScrAction *newMenuAction, const QString afterMenuName);
		bool insertMenuSeparator();
		
		bool hasSubMenu(ScrPopupMenu *subMenu);
		bool removeSubMenu(ScrPopupMenu *subMenu);
		bool removeMenuItem(ScrAction *menuAction);
		bool setSubMenuText(ScrAction *menuAction);
		
		bool generateEntryList(QStringList *actNames);
		void setParentMenuID(int);
		const int getParentMenuID();
		
	protected:
		QPopupMenu *localPopupMenu;
		QValueList< QGuardedPtr<QObject> > menuItemList;
		QString menuName;
		QString parentMenuName;
		int parentMenuID;
		QString menuText;
		int menuBarID;
		bool enabled;
};
#endif

