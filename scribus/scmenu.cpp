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
#include <qptrlist.h>
#include <qobject.h>
#include <qmetaobject.h>
#include <qpopupmenu.h>
#include "scmenu.h"
#include "scmenu.moc"
#include "scraction.h"

ScrPopupMenu::ScrPopupMenu ( QWidget * parent, const char * name, const QString pMenuName, const QString pMenuText, const QString parentName ) : QObject(parent, name)
{
	parentMenuName=parentName;
	menuText=pMenuText;
	menuName=pMenuName;
	menuBarID=-1;
	menuItemList.clear();
	localPopupMenu=new QPopupMenu(parent, name);
	enabled=true;
}

ScrPopupMenu::~ScrPopupMenu()
{
	delete localPopupMenu;
}

const QString ScrPopupMenu::getMenuText()
{
	return menuText;
}

void ScrPopupMenu::setMenuBarID(int id)
{
	menuBarID=id;
}

int ScrPopupMenu::getMenuBarID()
{
	return menuBarID;
}

QPopupMenu *ScrPopupMenu::getLocalPopupMenu()
{
	return localPopupMenu;
}

bool ScrPopupMenu::insertSubMenu(ScrPopupMenu* newSubMenu)
{
	menuItemList.append(newSubMenu);
	localPopupMenu->insertItem(newSubMenu->getMenuText(), newSubMenu->getLocalPopupMenu());
	return true;
}

bool ScrPopupMenu::hasSubMenu(const ScrPopupMenu* subMenu)
{
	return menuItemList.containsRef(subMenu);
}

bool ScrPopupMenu::removeSubMenu(ScrPopupMenu* subMenu)
{
	if (menuItemList.remove(subMenu))
	{
		repopulateLocalMenu();
		return true;
	}
	return false;
}

bool ScrPopupMenu::insertMenuItem(ScrAction *newMenuAction)
{
	menuItemList.append(newMenuAction);
	newMenuAction->addTo(localPopupMenu);
	return true;
}

bool ScrPopupMenu::insertMenuItemAfter(ScrAction *newMenuAction, ScrAction *afterMenuAction)
{
	//Allow duplicate menu items ???
	//if (menuItemList.findRef(newMenuAction)!=-1)
	//	return false;
	
	int index=menuItemList.findRef(afterMenuAction);
	if (index==-1)
		return false;
	int newIndex=++index;
	bool insSuccess=menuItemList.insert(newIndex, newMenuAction);
	if (!insSuccess)
		return false;
	return repopulateLocalMenu();
}

bool ScrPopupMenu::removeMenuItem(ScrAction *menuAction)
{
	if (menuItemList.remove(menuAction))
	{
		repopulateLocalMenu();
		return true;
	}
	return false;
}
/*
bool ScrPopupMenu::insertMenuItemAfter(ScrAction *newMenuAction, const QString afterMenuName)
{
	bool retVal=true;
	bool found=false;
	QPtrListIterator<QObject> menuItemListIt(menuItemList);
	QObject *object;
	while ( (object = menuItemListIt.current()) != 0  ) 
	{
		QString menuItemListClassName=object->className();
		int index=-1;
		//index=menuItemListIt.at();
		if (index=-1)
		{
			retVal=false;
			break;
		}
		if (menuItemListClassName=="ScrAction")
		{
			qDebug("ScrAction found");
			if(QString(object->name())==QString("separator_action"))
				bool blah=true;//localPopupMenu->insertSeparator();
			else
			{
				//Grab the action from the list, break and call the other after insert function
				
				int newIndex=++index;
				bool insSuccess=menuItemList.insert(newIndex, newMenuAction);
				if (!insSuccess)
					return false;
				
			}
		}
	}
	if (!found)
		return false;
	if (retVal==false)
		return false;
	
	return repopulateLocalMenu();
}
*/
bool ScrPopupMenu::repopulateLocalMenu()
{
	localPopupMenu->clear();
	QPtrListIterator<QObject> menuItemListIt(menuItemList);
	QObject *object;
	while ( (object = menuItemListIt.current()) != 0  ) 
	{
		QString menuItemListClassName=object->className();
		if (menuItemListClassName=="ScrAction")
		{
			//qDebug("ScrAction found");
			if(QString(object->name())==QString("separator_action"))
				localPopupMenu->insertSeparator();
			else
				dynamic_cast<ScrAction *>(object)->addTo(localPopupMenu);
		}
		else if (menuItemListClassName=="ScrPopupMenu")
			{
				//qDebug("ScrPopupMenu found");
				localPopupMenu->insertItem(dynamic_cast<ScrPopupMenu *>(object)->getMenuText(), dynamic_cast<ScrPopupMenu *>(object)->getLocalPopupMenu());
			}
			else
				qDebug(QString("Alien found: %1").arg(object->className()));
		++menuItemListIt;
	}
	return true;
}

bool ScrPopupMenu::clear()
{
	menuItemList.clear(); //CB TODO leaking separators here ?
	localPopupMenu->clear();
}

bool ScrPopupMenu::insertMenuSeparator()
{
	ScrAction *sepAction = new ScrAction( NULL, "separator_action" );
	menuItemList.append(sepAction);
	localPopupMenu->insertSeparator();
	return true;
}

void ScrPopupMenu::setEnabled(bool menuEnabled)
{
	enabled=menuEnabled;
	localPopupMenu->setEnabled(enabled);
}
