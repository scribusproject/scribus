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



MenuManager::MenuManager(QMenuBar *scrMenuBar, QObject *parent, const char *name) : QObject(parent, name)
{
	scribusMenuBar=scrMenuBar;
	menuList.clear();
}

MenuManager::~MenuManager()
{
}

bool MenuManager::createMenu(const QString &menuName, const QString &menuText, const QString parent, bool checkable)
{
	bool retVal=false;
		
	ScrPopupMenu *newMenu = new ScrPopupMenu(NULL, NULL, menuName, menuText, parent, checkable);
	if (newMenu)
	{
		menuList.insert(menuName, newMenu);
		if (!parent.isNull() && menuList[parent])
			retVal=menuList[parent]->insertSubMenu(newMenu);
	}
	else
		retVal=false;	
	
	return retVal;
}


bool MenuManager::createMenu(const QString &menuName, const QIconSet menuIcon, const QString &menuText, const QString parent, bool checkable)
{
	bool retVal=false;
	
	ScrPopupMenu *newMenu = new ScrPopupMenu(NULL, NULL, menuName, menuIcon, menuText, parent, checkable);
	if (newMenu)
	{
		menuList.insert(menuName, newMenu);
		if (!parent.isNull() && menuList[parent])
			retVal=menuList[parent]->insertSubMenu(newMenu);
	}
	else
		retVal=false;	
	qDebug(QString("createMenu: parent=%1 (%2) newMenu=%3 (%4) checkable=%5 -> %6").arg(parent).arg(!parent.isNull()).arg(menuName).arg(!newMenu).arg(checkable).arg(retVal));
	
	return retVal;
}

bool MenuManager::addMenuToMenu(const QString &child, const QString &parent)
{
	bool retVal=false;
	if (child==parent)
	{
		qDebug("%s", QString("Cannot add %1 menu to %2 menu (itself)").arg(child, parent).ascii());
		return false;	
	}
	if ((menuList.contains(child) && menuList[child]!=NULL) &&
		(menuList.contains(parent) && menuList[parent]!=NULL))
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
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		menuList[menuName]->clear();
		retVal=true;
	}
	else
		retVal=false;
	return retVal;
}

void MenuManager::setMenuText(const QString &menuName, const QString &menuText)
{
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		menuList[menuName]->setMenuText(menuText);
		QString parent=menuList[menuName]->getParentMenuName();
		if (!parent.isNull())
			menuList[parent]->repopulateLocalMenu();
		
		int id=menuList[menuName]->getMenuBarID();
		if (id!=-1) {
			QIconSet menuIcon = menuList[menuName]->getMenuIcon();
			scribusMenuBar->changeItem(id, menuIcon, menuText);
		}
	}
}

void MenuManager::setMenuIcon(const QString &menuName, const QIconSet &menuIcon)
{
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		menuList[menuName]->setMenuIcon(menuIcon);
		QString parent=menuList[menuName]->getParentMenuName();
		if (!parent.isNull())
			menuList[parent]->repopulateLocalMenu();
		
		int id=menuList[menuName]->getMenuBarID();
		if (id!=-1) {
			QString menuText = menuList[menuName]->getMenuText();
			scribusMenuBar->changeItem(id, menuIcon, menuText);
		}
	}
}

QPopupMenu *MenuManager::getLocalPopupMenu(const QString &menuName)
{
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
		return menuList[menuName]->getLocalPopupMenu();
	return NULL;
}

bool MenuManager::deleteMenu(const QString &menuName, const QString &parent)
{
	if (!parent.isNull())
	{
		if (menuList[parent] && menuList[parent]->hasSubMenu(menuList[menuName]))
			menuList[parent]->removeSubMenu(menuList[menuName]);
	}
	menuList.remove(menuName);
	return true;
}

void MenuManager::setMenuEnabled(const QString &menuName, const bool enabled)
{
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
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
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		int id=scribusMenuBar->insertItem( menuList[menuName]->getMenuIcon(), menuList[menuName]->getMenuText(), menuList[menuName]->getLocalPopupMenu());
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
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		if (menuList[afterMenuName])
		{
			int afterID=menuList[afterMenuName]->getMenuBarID();
			if (afterID!=-1)
			{
				int id=scribusMenuBar->insertItem( menuList[menuName]->getMenuIcon(), menuList[menuName]->getMenuText(), menuList[menuName]->getLocalPopupMenu(), scribusMenuBar->indexOf(afterID)+1, scribusMenuBar->indexOf(afterID)+1);
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
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
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
	if (menuList.contains(menuName) && menuList[menuName]!=NULL && action!=NULL)
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
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->insertMenuItem(menuAction);
	else
		retVal=false;
	return retVal;
}

bool MenuManager::addMenuItem(QWidget *widget, const QString &parent)
{
	bool retVal;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->insertMenuItem(widget);
	else
		retVal=false;
	return retVal;
}

bool MenuManager::addMenuItemAfter(ScrAction *menuAction, const QString &parent, ScrAction *afterMenuAction)
{
	bool retVal;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
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
		
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->insertMenuItemAfter(menuAction, actionFromName);
	else
		retVal=false;
	return retVal;
}


bool MenuManager::addMenuSeparator(const QString &parent)
{
	bool retVal;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->insertMenuSeparator();
	else
		retVal=false;
	return retVal;
	
}

bool MenuManager::removeMenuItem(ScrAction *menuAction, const QString &parent)
{
	bool retVal;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->removeMenuItem(menuAction);
	else
		retVal=false;
	return retVal;
}

void MenuManager::runMenuAtPos(const QString &menuName, const QPoint position)
{
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
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

bool MenuManager::empty()
{
	return menuList.empty();
}
