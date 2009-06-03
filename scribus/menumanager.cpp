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
#include <QMenuBar>
#include <QMenu>
#include "scribus.h"
#include "menumanager.h"
#include "scmenu.h"
#include "scraction.h"
#include <QDebug>


MenuManager::MenuManager(QMenuBar* mb, QObject *parent) : QObject(parent)
{
	scribusMenuBar=mb;
	menuList.clear();
}

MenuManager::~MenuManager()
{
}

bool MenuManager::createMenu(const QString &menuName, const QString &menuText, const QString parent, bool checkable)
{
	bool retVal=false;
		
	ScrPopupMenu *newMenu = new ScrPopupMenu(NULL, menuName, menuText, parent);
	if (newMenu)
	{
		menuList.insert(menuName, newMenu);
		if (!parent.isNull() && menuList[parent])
			retVal=menuList[parent]->insertSubMenu(newMenu);
	}
	return retVal;
}

bool MenuManager::addMenuToMenu(const QString &child, const QString &parent)
{
	bool retVal=false;
	if (child==parent)
	{
		qDebug("%s", QString("Cannot add %1 menu to %2 menu (itself)").arg(child, parent).toAscii().constData());
		return false;	
	}
	if ((menuList.contains(child) && menuList[child]!=NULL) &&
		(menuList.contains(parent) && menuList[parent]!=NULL))
	{
		menuList.insert(child, menuList[child]);
		retVal=menuList[parent]->insertSubMenu(menuList[child]);
	}
	
	return retVal;
}

bool MenuManager::clearMenu(const QString &menuName)
{
	bool retVal=false;
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		menuList[menuName]->clear();
		retVal=true;
	}
	return retVal;
}

void MenuManager::setText(const QString &menuName, const QString &menuText)
{
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		menuList[menuName]->setText(menuText);
		QString parent=menuList[menuName]->getParentMenuName();
		if (!parent.isNull())
			menuList[parent]->repopulateLocalMenu();
		
		/*
		
		int id = menuList[menuName]->getMenuBarID();
		if (id!=-1)
// 			scribusMenuBar->actions()[id]->setText(menuText);
		{
			QIcon menuIcon = menuList[menuName]->getMenuIcon();
			scribusMenuBar->changeItem(id, menuIcon, menuText);
		}*/
	}
}

void MenuManager::setMenuIcon(const QString &menuName, const QIcon &menuIcon)
{
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		menuList[menuName]->setMenuIcon(menuIcon);
		QString parent=menuList[menuName]->getParentMenuName();
		if (!parent.isNull())
			menuList[parent]->repopulateLocalMenu();
		/*
		int id=menuList[menuName]->getMenuBarID();
		if (id!=-1)
// 			scribusMenuBar->actions()[id]->setIcon(menuIcon);
		{
			QString menuText = menuList[menuName]->getMenuText();
			scribusMenuBar->changeItem(id, menuIcon, menuText);
		}
		*/
	}
}

QMenu *MenuManager::getLocalPopupMenu(const QString &menuName)
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
		/*
		int mainID=menuList[menuName]->getMenuBarID();
		if (mainID!=-1)
// 			scribusMenuBar->actions()[mainID]->setEnabled(enabled);
			scribusMenuBar->setItemEnabled(mainID, enabled);
		*/
	}
}

bool MenuManager::addMenuToMenuBar(const QString &menuName)
{
	bool retVal=false;
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
// 		qDebug() << "addMenuToMenuBar:" << menuName << " with text " << menuList[menuName]->getMenuText();
		QAction* t=scribusMenuBar->addMenu(menuList[menuName]->getLocalPopupMenu());
		if (t!=NULL)
		{
			t->setText(menuList[menuName]->getMenuText());
// 			qDebug() << "t" << t->text();
			retVal=true;
		}
		/*
		int id=scribusMenuBar->insertItem( menuList[menuName]->getMenuIcon(), menuList[menuName]->getMenuText(), menuList[menuName]->getLocalPopupMenu());
// 		menuList[menuName]->setMenuBarID(id);
		*/
	}
	return retVal;
}

bool MenuManager::addMenuToMenuBarBefore(const QString &menuName, const QString &afterMenuName)
{
	bool retVal=false;
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		if (menuList[afterMenuName])
		{
// 			qDebug() << "addMenuToMenuBarAfter:" << menuName << " with text " << menuList[menuName]->getMenuText() << " after " << menuList[afterMenuName]->getMenuText();
			QAction* t=scribusMenuBar->insertMenu(menuList[afterMenuName]->getLocalPopupMenu()->menuAction(), menuList[menuName]->getLocalPopupMenu());
			if (t!=NULL)
			{
				t->setText(menuList[menuName]->getMenuText());
// 				qDebug() << "t" << t->text();
				retVal=true;
			}
			/*
			int afterID=menuList[afterMenuName]->getMenuBarID();
			if (afterID!=-1)
			{
				int id=scribusMenuBar->insertItem( menuList[menuName]->getMenuIcon(), menuList[menuName]->getMenuText(), menuList[menuName]->getLocalPopupMenu(), scribusMenuBar->indexOf(afterID)+1, scribusMenuBar->indexOf(afterID)+1);
// 				menuList[menuName]->setMenuBarID(id);
				retVal=true;
			}*/
		}
	}
	return retVal;
}

bool MenuManager::removeMenuFromMenuBar(const QString &menuName)
{
	bool retVal=false;
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		if (menuList[menuName]->getLocalPopupMenu()->menuAction())
			scribusMenuBar->removeAction(menuList[menuName]->getLocalPopupMenu()->menuAction());
		/*
		int id=menuList[menuName]->getMenuBarID();
		if (id!=-1)
			scribusMenuBar->removeItem( id );
		*/
		retVal=true;
	}
	return retVal;
}

bool MenuManager::addMenuToWidgetOfAction(const QString &menuName, ScrAction *action)
{
	bool retVal=false;
	if (menuList.contains(menuName) && menuList[menuName]!=NULL && action!=NULL)
	{
		/* //qt4 cb replace with qwidgetaction or similar
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
		*/
	}
	return retVal;
}

bool MenuManager::addMenuItem(ScrAction *menuAction, const QString &parent)
{
	bool retVal=false;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->insertMenuItem(menuAction);
	return retVal;
}

/* Qt4
bool MenuManager::addMenuItem(QWidget *widget, const QString &parent)
{
	bool retVal=false;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->insertMenuItem(widget);
	return retVal;
}
*/

bool MenuManager::addMenuItemAfter(ScrAction *menuAction, const QString &parent, ScrAction *afterMenuAction)
{
	bool retVal=false;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->insertMenuItemAfter(menuAction, afterMenuAction);
	return retVal;
}

// bool MenuManager::addMenuItemAfter(ScrAction *menuAction, const QString &parent, const QString &afterMenuName)
// {
// 	ScrAction *actionFromName=NULL;
// 	//quick hack to make this work for existing plugins for now
// // 	if (parent=="File" && afterMenuName=="New")
// // 		actionFromName=m_ScMW->scrActions["fileNew"];
// // 	if (parent=="File" && afterMenuName=="Print")
// // 		actionFromName=m_ScMW->scrActions["filePrint"];
// // 	if (parent=="File" && afterMenuName=="SaveAs")
// // 		actionFromName=m_ScMW->scrActions["fileSaveAs"];
// // 	if (parent=="Help" && afterMenuName=="Manual")
// // 		actionFromName=m_ScMW->scrActions["helpManual"];
// 	bool retVal=false;
// 	if (menuList.contains(parent) && menuList[parent]!=NULL)
// 		retVal=menuList[parent]->insertMenuItemAfter(menuAction, actionFromName);
// 	return retVal;
// }


bool MenuManager::addMenuSeparator(const QString &parent)
{
	bool retVal=false;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->insertMenuSeparator();
	return retVal;
	
}

bool MenuManager::removeMenuItem(ScrAction *menuAction, const QString &parent)
{
	bool retVal=false;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->removeMenuItem(menuAction);
	return retVal;
}

void MenuManager::runMenuAtPos(const QString &menuName, const QPoint position)
{
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{	
		QMenu *popupmenu=menuList[menuName]->getLocalPopupMenu();
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
			/*
			QMap<QString, ScrPopupMenu *>::Iterator menuListIt;
			for (uint menuBarCount=0; menuBarCount<scribusMenuBar->count(); ++menuBarCount)
			{
				int menuBarMenuID=scribusMenuBar->idAt(menuBarCount);
				bool menuBarItemFound=false;
				for ( menuListIt = menuList.begin(); menuListIt!=menuList.end(); ++menuListIt)
				{
					if(menuListIt.value()->getMenuBarID()==menuBarMenuID)
					{
						menuBarItemFound=true;
						break;
					}
				}
				if (menuBarItemFound)
				{
					if (menuListIt.value())
					{
						ScrPopupMenu *currentMenu=menuListIt.value();
						currentMenu->generateEntryList(actionNames);
					}
				}
			}*/
		}
	}
}

bool MenuManager::empty()
{
	return menuList.empty();
}

bool MenuManager::menuExists(const QString &menuName)
{
	return menuList.contains(menuName);
}
