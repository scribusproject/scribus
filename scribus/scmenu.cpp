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
#include <qptrlist.h>
#include <qobject.h>
#include <qmetaobject.h>
#include <qpopupmenu.h>
#include "scmenu.h"
#include "scmenu.moc"
#include "scraction.h"
#include "util.h"

ScrPopupMenu::ScrPopupMenu ( QWidget * parent, const char * name, const QString pMenuName, const QString pMenuText, const QString parentName, bool pCheckable ) : QObject(parent, name)
{
	parentMenuName=parentName;
	parentMenuID=-1;
	menuText=pMenuText;
	menuName=pMenuName;
	menuBarID=-1;
	menuItemList.clear();
	localPopupMenu=new QPopupMenu(parent, name);
//	localPopupMenu->setCheckable(pCheckable);
	enabled=true;
	checkable = pCheckable;
}

ScrPopupMenu::ScrPopupMenu ( QWidget * parent, const char * name, const QString pMenuName, const QIconSet pMenuIcon, const QString pMenuText, const QString parentName, bool pCheckable ) : QObject(parent, name)
{
	parentMenuName=parentName;
	parentMenuID=-1;
	menuIcon=pMenuIcon;
	menuText=pMenuText;
	menuName=pMenuName;
	menuBarID=-1;
	menuItemList.clear();
	localPopupMenu=new QPopupMenu(parent, name);
//	localPopupMenu->setCheckable(pCheckable);
	enabled=true;
	checkable=pCheckable;
}

ScrPopupMenu::~ScrPopupMenu()
{
	delete localPopupMenu;
}

const QString ScrPopupMenu::getMenuText()
{
	return menuText;
}

void ScrPopupMenu::setMenuText(const QString pMenuText)
{
	menuText=pMenuText;
}

const QIconSet ScrPopupMenu::getMenuIcon()
{
	return menuIcon;
}

void ScrPopupMenu::setMenuIcon(const QIconSet pMenuIcon)
{
	menuIcon=pMenuIcon;
}

const QString ScrPopupMenu::getMenuName()
{
	return menuName;
}

const QString ScrPopupMenu::getParentMenuName()
{
	return parentMenuName;
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

bool ScrPopupMenu::hasSubMenu(ScrPopupMenu* subMenu)
{
	return menuItemList.contains(QGuardedPtr<QObject>(subMenu));
}

void ScrPopupMenu::setParentMenuID(int id)
{
	parentMenuID=id;
}


const int ScrPopupMenu::getParentMenuID()
{
	return parentMenuID;
}

bool ScrPopupMenu::insertSubMenu(ScrPopupMenu* newSubMenu)
{
	if (newSubMenu)
	{
		menuItemList.append(newSubMenu);
		localPopupMenu->insertItem(newSubMenu->getMenuIcon(), newSubMenu->getMenuText(), newSubMenu->getLocalPopupMenu());
		return true;
	}
	else
		return false;
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
	if (newMenuAction)
	{
		
#ifdef Q_WS_MAC
		bool menuListHasNoIcons = true;
		// look for ScrAction or ScrPopupMenu from the end of the list
		QValueList< QGuardedPtr<QObject> >::Iterator it = menuItemList.end(); 
		for (--it; it != menuItemList.end(); --it) {
			QObject * menuItem = *it;
			QString menuItemListClassName = menuItem->className();
			if (menuItemListClassName == "ScrAction")
			{
				ScrAction * act = dynamic_cast<ScrAction *>(menuItem);
				menuListHasNoIcons = act->iconSet().isNull();
				break;
			}
			else if (menuItemListClassName == "ScrPopupMenu")
			{
				ScrPopupMenu * men = dynamic_cast<ScrPopupMenu *>(menuItem);
				menuListHasNoIcons = men->getMenuIcon().isNull();
				break;
			}
			
		}
		if (newMenuAction->iconSet().isNull() && ! menuListHasNoIcons)
			newMenuAction->setIconSet(loadIcon("noicon.xpm"));
#endif
		
		menuItemList.append(newMenuAction);
		newMenuAction->addTo(localPopupMenu);			
		return true;
	}
	else 
		return false;
}

bool ScrPopupMenu::insertMenuItem(QWidget *widget)
{
	if (widget)
	{
		ScrAction *widgetAction = new ScrAction( NULL, "widget_action" );
		menuItemList.append(widgetAction);
		localPopupMenu->insertItem(widget);
		return true;
	}
	else 
		return false;
}

bool ScrPopupMenu::insertMenuItemAfter(ScrAction *newMenuAction, ScrAction *afterMenuAction)
{
	//Allow duplicate menu items ???
	//if (menuItemList.findRef(newMenuAction)!=-1)
	//	return false;
	
#ifdef Q_WS_MAC
	if (newMenuAction->iconSet().isNull() && ! (afterMenuAction->iconSet().isNull()))
		newMenuAction->setIconSet(loadIcon("noicon.xpm"));
#endif
		
	QValueList< QGuardedPtr<QObject> >::Iterator it=menuItemList.find(QGuardedPtr<QObject>(afterMenuAction));
	menuItemList.insert(++it, QGuardedPtr<QObject>(newMenuAction));
	/*
	if (index==-1)
		return false;
	int newIndex=++index;
	bool insSuccess=menuItemList.insert(newIndex, newMenuAction);

	if (!insSuccess)
		return false;
	*/
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

//CB TODO Does NOT handle rebuilding with widgets
bool ScrPopupMenu::repopulateLocalMenu()
{
	localPopupMenu->clear();
	QValueList< QGuardedPtr<QObject> >::Iterator menuItemListIt = menuItemList.begin();
	while (menuItemListIt!=menuItemList.end())
	{
		QObject *listObj=(*menuItemListIt);
		if (listObj==NULL)
		{
			QValueList< QGuardedPtr<QObject> >::Iterator menuItemListItToDelete = menuItemListIt;
			++menuItemListIt;
			menuItemList.remove(*menuItemListItToDelete);
			continue;
		}
		
		QString menuItemListClassName=listObj->className();
		if (menuItemListClassName=="ScrAction")
		{
			//qDebug("ScrAction found");
			if(QString(listObj->name())==QString("separator_action"))
				localPopupMenu->insertSeparator();
			else {
				ScrAction * act = dynamic_cast<ScrAction *>(listObj);
				act->addTo(localPopupMenu);
			}
		}
		else 
		{
			if (menuItemListClassName=="ScrPopupMenu")
			{
				ScrPopupMenu * men = dynamic_cast<ScrPopupMenu *>(listObj);
				localPopupMenu->insertItem(men->getMenuIcon(), men->getMenuText(), men->getLocalPopupMenu());
			}
			else
				sDebug(QString("Alien found: %1").arg((*menuItemListIt)->className()));
		}
		++menuItemListIt;
	}
	return true;
}

bool ScrPopupMenu::generateEntryList(QStringList *actNames)
{
	QValueList< QGuardedPtr<QObject> >::Iterator menuItemListIt = menuItemList.begin();
	while (menuItemListIt!=menuItemList.end())
	{
		QObject *listObj=(*menuItemListIt);
		if (listObj==NULL)
		{
			QValueList< QGuardedPtr<QObject> >::Iterator menuItemListItToDelete = menuItemListIt;
			++menuItemListIt;
			menuItemList.remove(*menuItemListItToDelete);
			continue;
		}
		
		QString menuItemListClassName=listObj->className();
		if (menuItemListClassName=="ScrAction")
		{
			if(QString(listObj->name())==QString("separator_action"))
				;
			else
			{
				ScrAction *sca=dynamic_cast<ScrAction *>(listObj);
				if (sca)
					actNames->append(sca->name());
			}
		}
		else 
		if (menuItemListClassName=="ScrPopupMenu")
		{
			ScrPopupMenu *scp=dynamic_cast<ScrPopupMenu *>(listObj);
			if (scp)
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

void ScrPopupMenu::setParentMenuName(const QString& newParentMenuName)
{
	if (!parentMenuName.isEmpty())
		parentMenuName=newParentMenuName;
}