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
#include "scribus.h"
#include "menumanager.h"
#include "ui/scmenu.h"
#include "scraction.h"
#include <QDebug>


MenuManager::MenuManager(QMenuBar* mb, QObject *parent) : QObject(parent)
{
	scribusMenuBar=mb;
	menuList.clear();
	menuStrings.clear();
	recentFileMenu=NULL;
	editPasteRecentMenu=NULL;
	windowsMenu=NULL;
}

MenuManager::~MenuManager()
{
}

bool MenuManager::createMenu(const QString &menuName, const QString &menuText, const QString parent, bool checkable)
{
	bool retVal=false;
	QList<QString> menuEntries;
	menuStrings.insert(menuName, menuEntries);
	menuStringTexts.insert(menuName, menuText);
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
		qDebug("%s", QString("Cannot add %1 menu to %2 menu (itself)").arg(child, parent).toLatin1().constData());
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
	if (menuBarMenus.contains(menuName))
		menuBarMenus[menuName]->clear();
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
		menuList[menuName]->setEnabled(enabled);
}

bool MenuManager::addMenuToMenuBar(const QString &menuName)
{
	bool retVal=false;
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		QAction* t=scribusMenuBar->addMenu(menuList[menuName]->getLocalPopupMenu());
		if (t!=NULL)
		{
			t->setText(menuList[menuName]->getMenuText());
			retVal=true;
		}
	}
	return retVal;
}

bool MenuManager::addMenuStringToMenuBar(const QString &menuName)
{
	bool retVal=false;
	if (menuStrings.contains(menuName))
	{
		QMenu *m=scribusMenuBar->addMenu(menuStringTexts[menuName]);
		menuBarMenus.insert(menuName, m);
		if (menuName=="Windows" && m!=NULL)
		{
			windowsMenu=m;
		}
		retVal=true;
	}
	return retVal;
}

void MenuManager::addMenuItemStringstoMenuBar(const QString &menuName, const QMap<QString, QPointer<ScrAction> > &menuActions)
{
	if (menuStrings.contains(menuName) && menuBarMenus.contains(menuName))
	{
		for (int i=0; i<menuStrings[menuName].count();++i)
		{
			//Add Separators
			if (menuStrings[menuName].at(i)=="SEPARATOR")
				menuBarMenus[menuName]->addSeparator();
			else
			{
				//Add Menu Items
				if (menuActions.contains(menuStrings[menuName].at(i)))
				{
					menuBarMenus[menuName]->addAction(menuActions[menuStrings[menuName].at(i)]);
				}
				else
				//Add Sub Menus
				{
					if (menuStrings.contains(menuStrings[menuName].at(i)))
					{
						QMenu *subMenu=menuBarMenus[menuName]->addMenu(menuStringTexts[menuStrings[menuName].at(i)]);
						if (menuStrings[menuName].at(i)=="FileOpenRecent")
						{
							recentFileMenu=subMenu;
						}
						else if (menuStrings[menuName].at(i)=="EditPasteRecent")
						{
							editPasteRecentMenu=subMenu;
						}
						else if (menuStrings[menuName].at(i)=="ItemLayer")
						{
							itemLayerMenu=subMenu;
						}
						else if (menuStrings[menuName].at(i)=="itemSendToScrapbook")
						{
							itemSendtoScrapbookMenu=subMenu;
						}
						addMenuItemStringstoMenu(menuStrings[menuName].at(i), subMenu, menuActions);
					}
				}
			}
		}
	}
}


void MenuManager::addMenuItemStringstoMenu(const QString &menuName, QMenu *menuToAddTo, const QMap<QString, QPointer<ScrAction> > &menuActions)
{
	if (menuStrings.contains(menuName))
	{
		for (int i=0; i<menuStrings[menuName].count();++i)
		{
			//Add Separators
			if (menuStrings[menuName].at(i)=="SEPARATOR")
				menuToAddTo->addSeparator();
			else
			{
				//Add Menu Items
				if (menuActions.contains(menuStrings[menuName].at(i)))
					menuToAddTo->addAction(menuActions[menuStrings[menuName].at(i)]);
				else
				//Add Sub Menus
				{
					if (menuStrings.contains(menuStrings[menuName].at(i)))
					{
						QMenu *subMenu=menuToAddTo->addMenu(menuStringTexts[menuStrings[menuName].at(i)]);
						if (subMenu)
							addMenuItemStringstoMenu(menuStrings[menuName].at(i), subMenu, menuActions);
					}
				}
			}
		}
	}
}


void MenuManager::addMenuItemStringstoSpecialMenu(const QString &menuName, const QMap<QString, QPointer<ScrAction> > &menuActions)
{
	if (menuName=="FileOpenRecent" && recentFileMenu!=NULL)
	{
		for( QMap<QString, QPointer<ScrAction> >::ConstIterator it = menuActions.begin(); it!=menuActions.end(); ++it )
			recentFileMenu->addAction(*it);
	}
	else if (menuName=="Windows" && windowsMenu!=NULL)
	{
		for( QMap<QString, QPointer<ScrAction> >::ConstIterator it = menuActions.begin(); it!=menuActions.end(); ++it )
			windowsMenu->addAction(*it);
	}
}

void MenuManager::clearMenuStrings(const QString &menuName)
{
	if (menuName=="FileOpenRecent" && recentFileMenu!=NULL)
	{
		recentFileMenu->clear();
	}
	else if (menuName=="Windows" && windowsMenu!=NULL)
	{
		windowsMenu->clear();
	}
}

bool MenuManager::addMenuToMenuBarBefore(const QString &menuName, const QString &afterMenuName)
{
	bool retVal=false;
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{
		if (menuList[afterMenuName])
		{
			QAction* t=scribusMenuBar->insertMenu(menuList[afterMenuName]->getLocalPopupMenu()->menuAction(), menuList[menuName]->getLocalPopupMenu());
			if (t!=NULL)
			{
				t->setText(menuList[menuName]->getMenuText());
				retVal=true;
			}
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

bool MenuManager::addMenuItem(ScrAction *menuAction, const QString &parent, bool enabled)
{
	bool retVal=false;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
	{
		retVal=menuList[parent]->insertMenuItem(menuAction);
		if (retVal)
			menuAction->setEnabled(enabled);
	}
	return retVal;
}

void MenuManager::addMenuItemString(const QString& s, const QString &parent)
{
	if (menuStrings.contains(parent))
		menuStrings[parent].append(s);
}


void MenuManager::addMenuItemStringAfter(const QString& s, const QString& after, const QString &parent)
{
	if (menuStrings.contains(parent))
	{
		int i=menuStrings[parent].indexOf(after);
		menuStrings[parent].insert(i+1, s);
	}
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

bool MenuManager::addMenuItemAfter(ScrAction *menuAction, const QString &parent, bool enabled, ScrAction *afterMenuAction)
{
	bool retVal=false;
	if (menuList.contains(parent) && menuList[parent]!=NULL)
	{
		retVal=menuList[parent]->insertMenuItemAfter(menuAction, afterMenuAction);
		if (retVal)
			menuAction->setEnabled(enabled);
	}
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

void MenuManager::dumpMenuStrings()
{
	QMapIterator<QString, QList<QString> > i(menuStrings);
	while (i.hasNext()) {
		i.next();
		qDebug() << "Menu name:"<<i.key();// << ": " << i.value() << endl;

		QListIterator<QString> li (i.value());
		while (li.hasNext()) {
			qDebug() << "Menu entry:"<<li.next();
		}
	}
}
