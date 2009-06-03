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
#ifndef SCMENU_H
#define SCMENU_H

#include <QAction>
#include <QIcon>
#include <QPointer>
#include <QList>
#include <QMenu>

class QMenu;
class ScrAction;

#include "scribusapi.h"

class SCRIBUS_API ScrPopupMenu : public QObject
{
	Q_OBJECT
	public:
		ScrPopupMenu ( QWidget * parent = 0, const QString pMenuName = QString::null, const QString pMenuText = QString::null, const QString parentName = QString::null, bool checkable = false);
		~ScrPopupMenu ();
		
		const QString getMenuName();
		const QString getParentMenuName();
		const QString getMenuText();
		void setText(const QString pMenuText);
		const QIcon getMenuIcon();
		void setMenuIcon(const QIcon pMenuIcon);
		void setEnabled(bool menuEnabled);
		QMenu *getLocalPopupMenu();
		bool repopulateLocalMenu();
		bool clear();
		
		bool insertSubMenu(ScrPopupMenu *newSubMenu);
		bool insertMenuItem(ScrAction *newMenuAction);
		bool insertMenuItemAfter(ScrAction *newMenuAction, ScrAction *afterMenuAction);
		bool insertMenuSeparator();
		
		bool hasSubMenu(ScrPopupMenu *subMenu);
		bool removeSubMenu(ScrPopupMenu *subMenu);
		bool removeMenuItem(ScrAction *menuAction);
		bool setSubMenuText(ScrAction *menuAction);
		
		bool generateEntryList(QStringList *actNames);
		void setParentMenuName(const QString& newParentMenuName);
		
	protected:
		QMenu *localPopupMenu;
		QList< QPointer<QObject> > menuItemList;
		QString menuName;
		QString parentMenuName;
		int parentMenuID;
		QString menuText;
		QIcon menuIcon;
		int menuBarID;
		bool enabled;
		bool checkable;
};
#endif

