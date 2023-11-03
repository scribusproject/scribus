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

#include <QDebug>
#include <QObject>
#include <QMetaObject>
#include <QMenu>
#include <QList>
#include "scmenu.h"
#include "scraction.h"
#include "util_debug.h"
#include "iconmanager.h"


ScrPopupMenu::ScrPopupMenu ( QWidget * parent, const QString& pMenuName, const QString& pMenuText, const QString& parentName, bool pCheckable ) : QObject(parent)
{
	parentMenuName = parentName;
	parentMenuID = -1;
	menuText = pMenuText;
	menuName = pMenuName;
	menuBarID = -1;
	menuItemList.clear();
	localPopupMenu = new QMenu(parent);//, name);
//	localPopupMenu->setCheckable(pCheckable);
	enabled = true;
	checkable = pCheckable;
}

ScrPopupMenu::~ScrPopupMenu()
{
	delete localPopupMenu;
}

QString ScrPopupMenu::getMenuText()
{
	return menuText;
}

void ScrPopupMenu::setText(const QString& pMenuText)
{
	menuText = pMenuText;
	localPopupMenu->menuAction()->setText(menuText);
}

QIcon ScrPopupMenu::getMenuIcon()
{
	return menuIcon;
}

void ScrPopupMenu::setMenuIcon(const QIcon& pMenuIcon)
{
	menuIcon = pMenuIcon;
}

QString ScrPopupMenu::getMenuName()
{
	return menuName;
}

QString ScrPopupMenu::getParentMenuName()
{
	return parentMenuName;
}

// void ScrPopupMenu::setMenuBarID(int id)
// {
// 	menuBarID = id;
// }
//
// int ScrPopupMenu::getMenuBarID()
// {
// 	return menuBarID;
// }

QMenu *ScrPopupMenu::getLocalPopupMenu()
{
	return localPopupMenu;
}

bool ScrPopupMenu::hasSubMenu(ScrPopupMenu* subMenu)
{
	return menuItemList.contains(QPointer<QObject>(subMenu));
}

// void ScrPopupMenu::setParentMenuID(int id)
// {
// 	parentMenuID = id;
// }
//
//
// const int ScrPopupMenu::getParentMenuID()
// {
// 	return parentMenuID;
// }

bool ScrPopupMenu::insertSubMenu(ScrPopupMenu* newSubMenu)
{
	if (newSubMenu)
	{
		menuItemList.append(newSubMenu);
// 		localPopupMenu->insertItem(newSubMenu->getMenuIcon(), newSubMenu->getMenuText(), newSubMenu->getLocalPopupMenu());
		QAction *m = localPopupMenu->addMenu(newSubMenu->getLocalPopupMenu());
		if (m)
			m->setText(newSubMenu->getMenuText());
		return true;
	}
	return false;
}

bool ScrPopupMenu::removeSubMenu(ScrPopupMenu* subMenu)
{
	if (menuItemList.removeAll(subMenu))
	{
		repopulateLocalMenu();
		return true;
	}
	return false;
}

bool ScrPopupMenu::insertMenuItem(ScrAction *newMenuAction)
{
	if (newMenuAction)
	{

#ifdef Q_OS_MACOS
		bool menuListHasNoIcons = true;
		// look for ScrAction or ScrPopupMenu from the end of the list
//		QList< QPointer<QObject> >::Iterator it = menuItemList.end();
		int s = menuItemList.size() - 1;
		for (int i = s; i >= 0; --i) {
			QObject* menuItem = menuItemList[i];
			QString menuItemListClassName = menuItemList[i]->metaObject()->className();
			if (menuItemListClassName == "ScrAction")
			{
				ScrAction * act = qobject_cast<ScrAction *>(menuItem);
				if (act != nullptr)
					menuListHasNoIcons = act->icon().isNull();
				break;
			}
			if (menuItemListClassName == "ScrPopupMenu")
			{
				ScrPopupMenu * men = qobject_cast<ScrPopupMenu *>(menuItem);
				if (men != nullptr)
					menuListHasNoIcons = men->getMenuIcon().isNull();
				break;
			}
		}
		if (newMenuAction->icon().isNull() && ! menuListHasNoIcons)
			newMenuAction->setIcon(IconManager::instance().loadIcon("noicon.png"));
#endif

		menuItemList.append(newMenuAction);
		localPopupMenu->addAction(newMenuAction);
		return true;
	}
	return false;
}

/* Qt4
bool ScrPopupMenu::insertMenuItem(QWidget *widget)
{
	if (widget)
	{
		ScrAction *widgetAction = new ScrAction( nullptr, "widget_action" );
		menuItemList.append(widgetAction);
		localPopupMenu->insertItem(widget);
		return true;
	}
	else
		return false;
}
*/

bool ScrPopupMenu::insertMenuItemAfter(ScrAction *newMenuAction, ScrAction *afterMenuAction)
{
	//Allow duplicate menu items ???
	//if (menuItemList.findRef(newMenuAction)!=-1)
	//	return false;

#ifdef Q_OS_MACOS
	if (newMenuAction && afterMenuAction)
		if (newMenuAction->icon().isNull() && ! (afterMenuAction->icon().isNull()))
			newMenuAction->setIcon(IconManager::instance().loadIcon("noicon.png"));
#endif

	int pos = menuItemList.indexOf(QPointer<QObject>(afterMenuAction));
	menuItemList.insert(++pos, QPointer<QObject>(newMenuAction));
	/*
	if (index == -1)
		return false;
	int newIndex = ++index;
	bool insSuccess = menuItemList.insert(newIndex, newMenuAction);

	if (!insSuccess)
		return false;
	*/
	return repopulateLocalMenu();
}

bool ScrPopupMenu::removeMenuItem(ScrAction *menuAction)
{
	if (menuItemList.removeAll(menuAction))
	{
		repopulateLocalMenu();
		return true;
	}
	return false;
}
/*
bool ScrPopupMenu::insertMenuItemAfter(ScrAction *newMenuAction, const QString afterMenuName)
{
	bool retVal = true;
	bool found = false;
	QPtrListIterator<QObject> menuItemListIt(menuItemList);
	QObject *object;
	while ( (object = menuItemListIt.current()) != 0  )
	{
		QString menuItemListClassName = object->className();
		int index = -1;
		//index = menuItemListIt.at();
		if (index == -1)
		{
			retVal = false;
			break;
		}
		if (menuItemListClassName == "ScrAction")
		{
			qDebug("ScrAction found");
			if (QString(object->name()) == QString("separator_action"))
				bool blah = true; //localPopupMenu->insertSeparator();
			else
			{
				//Grab the action from the list, break and call the other after insert function

				int newIndex = ++index;
				bool insSuccess = menuItemList.insert(newIndex, newMenuAction);
				if (!insSuccess)
					return false;
			}
		}
	}
	if (!found)
		return false;
	if (retVal == false)
		return false;

	return repopulateLocalMenu();
}
*/

//CB TODO Does NOT handle rebuilding with widgets
bool ScrPopupMenu::repopulateLocalMenu()
{
	localPopupMenu->clear();
	QList< QPointer<QObject> >::Iterator menuItemListIt = menuItemList.begin();
	while (menuItemListIt != menuItemList.end())
	{
		QObject *listObj = (*menuItemListIt);
		if (listObj == nullptr)
		{
			QList< QPointer<QObject> >::Iterator menuItemListItToDelete = menuItemListIt;
			++menuItemListIt;
			menuItemList.removeAll(*menuItemListItToDelete);
			continue;
		}

		QString menuItemListClassName = listObj->metaObject()->className();
		if (menuItemListClassName == "ScrAction")
		{
			ScrAction * act = qobject_cast<ScrAction *>(listObj);
			if (act != nullptr)
				localPopupMenu->addAction(act);
		}
		else if (menuItemListClassName == "ScrPopupMenu")
		{
			ScrPopupMenu * men = qobject_cast<ScrPopupMenu *>(listObj);
			if (men != nullptr)
			{
				//qDebug() << men->localPopupMenu->title() << men->localPopupMenu->;
// 				localPopupMenu->insertItem(men->getMenuIcon(), men->getMenuText(), men->getLocalPopupMenu());
				QAction *m = localPopupMenu->addMenu(men->getLocalPopupMenu());
				if (m)
					m->setText(men->getMenuText());
			}
		}
		else
			sDebug(QString("Alien found: %1").arg((*menuItemListIt)->metaObject()->className()));
		++menuItemListIt;
	}
	return true;
}

bool ScrPopupMenu::generateEntryList(QStringList *actNames)
{
	QList< QPointer<QObject> >::Iterator menuItemListIt = menuItemList.begin();
	while (menuItemListIt != menuItemList.end())
	{
		QObject *listObj = (*menuItemListIt);
		if (listObj == nullptr)
		{
			QList< QPointer<QObject> >::Iterator menuItemListItToDelete = menuItemListIt;
			++menuItemListIt;
			menuItemList.removeAll(*menuItemListItToDelete);
			continue;
		}

		QString menuItemListClassName = listObj->metaObject()->className();
		if (menuItemListClassName == "ScrAction")
		{
			auto *sca = qobject_cast<ScrAction *>(listObj);
			if (sca != nullptr)
				actNames->append(sca->objectName());
		}
		else if (menuItemListClassName == "ScrPopupMenu")
		{
			auto *scp = qobject_cast<ScrPopupMenu *>(listObj);
			if (scp != nullptr)
				scp->generateEntryList(actNames);
		}
		++menuItemListIt;
	}
	return true;

}

bool ScrPopupMenu::clear()
{
	menuItemList.clear(); //CB TODO leaking separators here ?
	localPopupMenu->clear();
	return true;
}

bool ScrPopupMenu::insertMenuSeparator()
{
	ScrAction *sepAction = new ScrAction( nullptr );
	sepAction->setSeparator(true);
	menuItemList.append(sepAction);
	localPopupMenu->addSeparator();
	return true;
}

void ScrPopupMenu::setEnabled(bool menuEnabled)
{
	enabled = menuEnabled;
	localPopupMenu->setEnabled(enabled);
}

void ScrPopupMenu::setParentMenuName(const QString& newParentMenuName)
{
	if (!parentMenuName.isEmpty())
		parentMenuName = newParentMenuName;
}
