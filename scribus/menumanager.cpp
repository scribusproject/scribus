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
#include <qdict.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include "scribus.h"
#include "menumanager.h"
#include "menumanager.moc"
#include "scmenu.h"
#include "scraction.h"

extern ScribusApp* ScApp;

MenuManager::MenuManager(QMenuBar *scrMenuBar, QObject *parent, const char *name) : QObject(parent, name)
{
	scribusMenuBar=scrMenuBar;
	menuList.clear();
}

MenuManager::~MenuManager()
{
}

bool MenuManager::createMenu(const QString &menuName, const QString &menuText, const QString parent)
{
	bool retVal=false;
		
	ScrPopupMenu *newMenu = new ScrPopupMenu(NULL, NULL, menuName, menuText, parent);
	if (newMenu)
	{
		menuList.insert(menuName, newMenu);
		if (parent!=QString::null && menuList[parent])
			retVal=menuList[parent]->insertSubMenu(newMenu);
	}
	else
		retVal=false;	
	
	return retVal;
}

bool MenuManager::addMenuToMenu(const QString &child, const QString &parent)
{
	bool retVal=false;
	if (child==parent)
	{
		qDebug(QString("Cant add %1 menu to %2 menu (itself)").arg(child, parent));
		return false;	
	}
	if (menuList[child] && menuList[parent])
	{
		menuList.insert(child, menuList[child]);
		retVal=menuList[parent]->insertSubMenu(menuList[child]);
	}
	else
		retVal=false;	
	
	return retVal;
}

bool MenuManager::clearMenu(const QString &menuName)
{
	bool retVal;
	if (menuList[menuName])
	{
		menuList[menuName]->clear();
		retVal=true;
	}
	else
		retVal=false;
	return retVal;
}

QPopupMenu *MenuManager::getLocalPopupMenu(const QString &menuName)
{
	if (menuList[menuName])
	{
		return menuList[menuName]->getLocalPopupMenu();
	}
	return NULL;
}

bool MenuManager::deleteMenu(const QString &menuName, const QString &parent)
{
	if (parent!=QString::null)
	{
		if (menuList[parent] && menuList[parent]->hasSubMenu(menuList[menuName]))
			menuList[parent]->removeSubMenu(menuList[menuName]);
	}
	menuList.remove(menuName);
	return true;
}

void MenuManager::setMenuEnabled(const QString &menuName, const bool enabled)
{
	if (menuList[menuName])
	{
		menuList[menuName]->setEnabled(enabled);
		int mainID=menuList[menuName]->getMenuBarID();
		if (mainID!=-1)
			scribusMenuBar->setItemEnabled(mainID, enabled);
	}
}

bool MenuManager::addMenuToMenuBar(const QString &menuName)
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

bool MenuManager::addMenuToMenuBarAfter(const QString &menuName, const QString &afterMenuName)
{
	bool retVal;
	if (menuList[menuName])
	{
		if (menuList[afterMenuName])
		{
			int afterID=menuList[afterMenuName]->getMenuBarID();
			if (afterID!=-1)
			{
				int id=scribusMenuBar->insertItem( menuList[menuName]->getMenuText(), menuList[menuName]->getLocalPopupMenu(), scribusMenuBar->indexOf(afterID)+1, scribusMenuBar->indexOf(afterID)+1);
				menuList[menuName]->setMenuBarID(id);
				retVal=true;
			}
		}
	}
	else
		retVal=false;
	return retVal;
}

bool MenuManager::removeMenuFromMenuBar(const QString &menuName)
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

bool MenuManager::addMenuToWidgetOfAction(const QString &menuName, ScrAction *action)
{
	bool retVal=false;
	if (menuList[menuName] && action!=NULL)
	{
		QWidget *w=action->getWidgetAddedTo();
		if (w)
		{
			QString menuItemListClassName=w->className();
			if (menuItemListClassName=="QToolButton")
			{
				QToolButton *toolButton=dynamic_cast<QToolButton *>(w);
				if (toolButton!=NULL)
				{
					toolButton->setPopup(menuList[menuName]->getLocalPopupMenu());
					retVal=true;
				}
			}
		}
	}
	return retVal;
}

bool MenuManager::addMenuItem(ScrAction *menuAction, const QString &parent)
{
	bool retVal;
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuItem(menuAction);
	else
		retVal=false;
	return retVal;
}

bool MenuManager::addMenuItem(QWidget *widget, const QString &parent)
{
	bool retVal;
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuItem(widget);
	else
		retVal=false;
	return retVal;
}

bool MenuManager::addMenuItemAfter(ScrAction *menuAction, const QString &parent, ScrAction *afterMenuAction)
{
	bool retVal;
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuItemAfter(menuAction, afterMenuAction);
	else
		retVal=false;
	return retVal;
}

bool MenuManager::addMenuItemAfter(ScrAction *menuAction, const QString &parent, const QString &afterMenuName)
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
	if (parent=="Help" && afterMenuName=="Manual")
		actionFromName=ScApp->scrActions["helpManual"];
		
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuItemAfter(menuAction, actionFromName);
	else
		retVal=false;
	return retVal;
}


bool MenuManager::addMenuSeparator(const QString &parent)
{
	bool retVal;
	if (menuList[parent])
		retVal=menuList[parent]->insertMenuSeparator();
	else
		retVal=false;
	return retVal;
	
}

bool MenuManager::removeMenuItem(ScrAction *menuAction, const QString &parent)
{
	bool retVal;
	if (menuList[parent])
		retVal=menuList[parent]->removeMenuItem(menuAction);
	else
		retVal=false;
	return retVal;
}

void MenuManager::runMenuAtPos(const QString &menuName, const QPoint position)
{
	if (menuList[menuName])
	{	
		QPopupMenu *popupmenu=menuList[menuName]->getLocalPopupMenu();
		if (popupmenu)
			popupmenu->exec(position);
	}
}

// Used to generate a list of entries from the menu system into the keyboard shortcut manager
void MenuManager::generateKeyManList(QStringList *actionNames)
{
	if (actionNames)
	{
		if (scribusMenuBar)
		{
			for (uint menuBarCount=0; menuBarCount<scribusMenuBar->count(); ++menuBarCount)
			{
				int menuBarMenuID=scribusMenuBar->idAt(menuBarCount);
				QMap<QString, ScrPopupMenu *>::Iterator menuListIt;
				bool menuBarItemFound=false;			
				for ( menuListIt = menuList.begin(); menuListIt!=menuList.end(); ++menuListIt)
				{
					if(menuListIt.data()->getMenuBarID()==menuBarMenuID)
					{
						menuBarItemFound=true;
						break;
					}
				}
				if (menuBarItemFound)
				{
					if (menuListIt.data())
					{
						ScrPopupMenu *currentMenu=menuListIt.data();
						currentMenu->generateEntryList(actionNames);
					}
				}
			}
		}
	}
}
