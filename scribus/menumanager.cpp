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
#include <qmenubar.h>
#include <qdict.h>
#include "scribus.h"
#include "menumanager.h"
#include "scmenu.h"
#include "scraction.h"

extern ScribusApp* ScApp;

MenuManager::MenuManager(QMenuBar *scrMenuBar, QObject *parent, const char *name)
{
	scribusMenuBar=scrMenuBar;
	menuList.clear();
}

MenuManager::~MenuManager()
{
}

bool MenuManager::createMenu(const QString menuName, const QString menuText, const QString parent)
{
	bool retVal=false;
		
	ScrPopupMenu *newMenu = new ScrPopupMenu(NULL, NULL, menuName, menuText, parent);
	if (newMenu)
	{
		menuList.insert(menuName, newMenu);
		if (menuList[parent])
			retVal=menuList[parent]->insertSubMenu(newMenu);
	}
	else
		retVal=false;	
	
	return retVal;
}

bool MenuManager::clearMenu(const QString menuName)
{
	menuList[menuName]->clear();
}

bool MenuManager::deleteMenu(const QString menuName, const QString parent)
{
	if (parent!=QString::null)
	{
		if (menuList[parent] && menuList[parent]->hasSubMenu(menuList[menuName]))
			menuList[parent]->removeSubMenu(menuList[menuName]);
	}
	menuList.remove(menuName);
	return true;
}

void MenuManager::setMenuEnabled(const QString menuName, const bool enabled)
{
	if (menuList[menuName])
		menuList[menuName]->setEnabled(enabled);
}

bool MenuManager::addMenuToMenuBar(const QString menuName)
{
	bool retVal;
	if (menuList[menuName])
	{
		int id=scribusMenuBar->insertItem( menuList[menuName]->getMenuText(), menuList[menuName]->getLocalPopupMenu());
		menuList[menuName]->setMenuBarID(id);
		retVal=true;
	}
	else
		retVal=false;
	return retVal;
}

bool MenuManager::removeMenuFromMenuBar(const QString menuName)
{
	bool retVal;
	if (menuList[menuName])
	{
		int id=menuList[menuName]->getMenuBarID();
		if (id!=-1)
			scribusMenuBar->removeItem( id );
		retVal=true;
	}
	else
		retVal=false;
	return retVal;
}

bool MenuManager::addMenuToWidgetOfAction(const QString menuName, ScrAction *action)
{
	bool retVal;
	if (menuList[menuName] && action!=NULL)
	{
		QWidget *w=action->getWidgetAddedTo();
		QString menuItemListClassName=w->className();
		if (menuItemListClassName=="QToolButton")
		{
			qDebug(QString("blah %1 %2").arg(menuName));
			dynamic_cast<QToolButton *>(w)->setPopup(menuList[menuName]->getLocalPopupMenu());
		}
		else
			qDebug(QString("blah2 %1 %2").arg(menuName));
	}
}

bool MenuManager::addMenuItem(ScrAction *menuAction, const QString parent)
{
	bool retVal;
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuItem(menuAction);
	else
		retVal=false;
	return retVal;
}

bool MenuManager::addMenuItemAfter(ScrAction *menuAction, const QString parent, ScrAction *afterMenuAction)
{
	bool retVal;
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuItemAfter(menuAction, afterMenuAction);
	else
		retVal=false;
	return retVal;
}

bool MenuManager::addMenuItemAfter(ScrAction *menuAction, const QString parent, const QString afterMenuName)
{
	bool retVal;
	ScrAction *actionFromName=NULL;
	//quick hack to make this work for existing plugins for now
	if (parent=="File" && afterMenuName=="New")
		actionFromName=ScApp->scrActions["fileNew"];
	if (parent=="File" && afterMenuName=="Print")
		actionFromName=ScApp->scrActions["filePrint"];
	if (parent=="File" && afterMenuName=="SaveAs")
		actionFromName=ScApp->scrActions["fileSaveAs"];
	
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuItemAfter(menuAction, actionFromName);
	else
		retVal=false;
	return retVal;
}


bool MenuManager::addMenuSeparator(const QString parent)
{
	bool retVal;
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuSeparator();
	else
		retVal=false;
	return retVal;
	
}

void MenuManager::runMenuAtPos(const QString menuName, const QPoint position)
{
	if (menuList[menuName])
	{	
		menuList[menuName]->getLocalPopupMenu()->exec(position);
	}
}
