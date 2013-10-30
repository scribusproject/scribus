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
	menuStrings.clear();
	m_undoMenu=new QMenu("undo");
	m_redoMenu=new QMenu("redo");
	rememberedMenus.clear();
}

MenuManager::~MenuManager()
{
	m_undoMenu->deleteLater();
	m_redoMenu->deleteLater();
}

bool MenuManager::createMenu(const QString &menuName, const QString &menuText, const QString parent, bool checkable, bool rememberMenu)
{
	bool retVal=false;
	QList<QString> menuEntries;
	menuStrings.insert(menuName, menuEntries);
	menuStringTexts.insert(menuName, menuText);
	if (rememberMenu)
	{
		rememberedMenus.insert(menuName, NULL);
	}
	return retVal;
}


bool MenuManager::clearMenu(const QString &menuName)
{
	bool retVal=false;
	if (menuBarMenus.contains(menuName))
		menuBarMenus[menuName]->clear();
	return retVal;
}

void MenuManager::setText(const QString &menuName, const QString &menuText)
{
	if (menuStringTexts.contains(menuName))
		menuStringTexts.insert(menuName, menuText);
	//TODO rebuild all menus
//	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
//	{
//		menuList[menuName]->setText(menuText);
//		QString parent=menuList[menuName]->getParentMenuName();
//		if (!parent.isNull())
//			menuList[parent]->repopulateLocalMenu();
//	}
}

QMenu *MenuManager::getLocalPopupMenu(const QString &menuName)
{
	if (menuBarMenus.contains(menuName) && menuBarMenus.value(menuName)!=NULL)
		return menuBarMenus.value(menuName);
	return NULL;
}

void MenuManager::setMenuEnabled(const QString &menuName, const bool enabled)
{
	if (menuBarMenus.contains(menuName) && menuBarMenus.value(menuName)!=NULL)
		menuBarMenus.value(menuName)->setEnabled(enabled);
}

bool MenuManager::addMenuStringToMenuBar(const QString &menuName, bool rememberMenu)
{
	bool retVal=false;
	if (menuStrings.contains(menuName))
	{
		QMenu *m=scribusMenuBar->addMenu(menuStringTexts[menuName]);
		menuBarMenus.insert(menuName, m);
		if (rememberMenu)
		{
			rememberedMenus.insert(menuName, m);
		}
		retVal=true;
	}
	return retVal;
}

bool MenuManager::addMenuStringToMenuBarBefore(const QString &menuName, const QString &beforeMenuName)
{
	bool retVal=false;
	if (menuStrings.contains(menuName))
	{
		QList<QAction*> mba=scribusMenuBar->actions();
		QAction* beforeAct;
		foreach (beforeAct, mba) {
			if (beforeMenuName==beforeAct->text().remove('&').remove("..."))
				break;
		}
		QMenu *m=new QMenu(menuName);
		scribusMenuBar->insertMenu(beforeAct, m);
		menuBarMenus.insert(menuName, m);
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
						if (rememberedMenus.contains(menuStrings[menuName].at(i)))
						{
							rememberedMenus.insert(menuStrings[menuName].at(i), subMenu);
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
						{
							if (rememberedMenus.contains(menuStrings[menuName].at(i)))
							{
								rememberedMenus.insert(menuStrings[menuName].at(i), subMenu);
								addMenuItemStringstoMenu(menuStrings[menuName].at(i), subMenu, menuActions);
							}
						}
					}
				}
			}
		}
	}
}

void MenuManager::addMenuItemStringstoRememberedMenu(const QString &menuName, const QMap<QString, QPointer<ScrAction> > &menuActions)
{
	if (rememberedMenus.contains(menuName))
		if (rememberedMenus.value(menuName)!=NULL)
			addMenuItemStringstoMenu(menuName, rememberedMenus.value(menuName), menuActions);
}


void MenuManager::addMenuItemStringstoSpecialMenu(const QString &menuName, const QMap<QString, QPointer<ScrAction> > &menuActions)
{
	if (rememberedMenus.contains(menuName))
	{
		if (rememberedMenus.value(menuName)!=NULL)
		{
			if (menuName=="ItemSendToScrapbook")
			{
				for( QMap<QString, QPointer<ScrAction> >::ConstIterator it = menuActions.begin(); it!=menuActions.end(); ++it )
					rememberedMenus.value(menuName)->addAction(*it);
			}
			else if (menuName=="Windows")
			{
				for( QMap<QString, QPointer<ScrAction> >::ConstIterator it = menuActions.begin(); it!=menuActions.end(); ++it )
					rememberedMenus.value(menuName)->addAction(*it);
			}
			else if (menuName=="FileOpenRecent")
			{
				for( QMap<QString, QPointer<ScrAction> >::ConstIterator it = menuActions.begin(); it!=menuActions.end(); ++it )
					rememberedMenus.value(menuName)->addAction(*it);
			}
			else if (menuName=="EditPasteRecent")
			{
				for( QMap<QString, QPointer<ScrAction> >::ConstIterator it = menuActions.begin(); it!=menuActions.end(); ++it )
				{
					rememberedMenus.value(menuName)->addAction(*it);
				}
			}
			else if (menuName=="ItemLayer")
			{
				for( QMap<QString, QPointer<ScrAction> >::ConstIterator it = menuActions.begin(); it!=menuActions.end(); ++it )
				{
					rememberedMenus.value(menuName)->addAction(*it);
				}
			}
		}
	}
}

void MenuManager::clearMenuStrings(const QString &menuName)
{
	if (rememberedMenus.contains(menuName))
	{
		if (rememberedMenus.value(menuName)!=NULL)
			rememberedMenus.value(menuName)->clear();
	}
}



bool MenuManager::addMenuToWidgetOfAction(const QString &menuName, ScrAction *action)
{
	bool retVal=false;
	/*if (menuList.contains(menuName) && menuList[menuName]!=NULL && action!=NULL)
	{
		 //qt4 cb replace with qwidgetaction or similar
//		QWidget *w=action->getWidgetAddedTo();
//		if (w)
//		{
//			QString menuItemListClassName=w->className();
//			if (menuItemListClassName=="QToolButton")
//			{
//				QToolButton *toolButton=dynamic_cast<QToolButton *>(w);
//				if (toolButton!=NULL)
//				{
//					toolButton->setPopup(menuList[menuName]->getLocalPopupMenu());
//					retVal=true;
//				}
//			}
//		}

	}
*/
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

bool MenuManager::removeMenuItem(ScrAction *menuAction, const QString &parent)
{
	bool retVal=false;
	/*
	 if (menuList.contains(parent) && menuList[parent]!=NULL)
		retVal=menuList[parent]->removeMenuItem(menuAction);
	*/
	return retVal;
}

void MenuManager::runMenuAtPos(const QString &menuName, const QPoint position)
{
	/*
	if (menuList.contains(menuName) && menuList[menuName]!=NULL)
	{	
		QMenu *popupmenu=menuList[menuName]->getLocalPopupMenu();
		if (popupmenu)
			popupmenu->exec(position);
	}
	*/
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
	return menuStrings.empty();
}

bool MenuManager::menuExists(const QString &menuName)
{
	return menuStrings.contains(menuName);
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
