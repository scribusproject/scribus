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

const QString ScrPopupMenu::getMenuName()
{
	return menuName;
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

bool ScrPopupMenu::insertSubMenu(ScrPopupMenu* newSubMenu)
{
	if (newSubMenu)
	{
		menuItemList.append(newSubMenu);
		localPopupMenu->insertItem(newSubMenu->getMenuText(), newSubMenu->getLocalPopupMenu());
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
	
	for ( QValueList< QGuardedPtr<QObject> >::Iterator menuItemListIt = menuItemList.begin();
			 menuItemListIt!=menuItemList.end(); ++menuItemListIt ) 
	{
		//CB TODO make safer
		QObject *listObj=(*menuItemListIt);
		if (listObj==NULL)
		{
			menuItemList.remove(listObj);
			continue;
		}
		
		QString menuItemListClassName=listObj->className();
		if (menuItemListClassName=="ScrAction")
		{
			//qDebug("ScrAction found");
			if(QString(listObj->name())==QString("separator_action"))
				localPopupMenu->insertSeparator();
			else
				dynamic_cast<ScrAction *>(listObj)->addTo(localPopupMenu);
		}
		else 
		{
			if (menuItemListClassName=="ScrPopupMenu")
			{
				localPopupMenu->insertItem(dynamic_cast<ScrPopupMenu *>(listObj)->getMenuText(), dynamic_cast<ScrPopupMenu *>(listObj)->getLocalPopupMenu());
			}
			else
				qDebug(QString("Alien found: %1").arg((*menuItemListIt)->className()));
		}
	}
	return true;
}

bool ScrPopupMenu::generateEntryList(QStringList *actNames)
{
	for ( QValueList< QGuardedPtr<QObject> >::Iterator menuItemListIt = menuItemList.begin();
			 menuItemListIt!=menuItemList.end(); ++menuItemListIt ) 
	{
		//CB TODO make safer
		QObject *listObj=(*menuItemListIt);
		if (listObj==NULL)
		{
			menuItemList.remove(listObj);
			continue;
		}
			
		//qDebug(QString("%1").arg(listObj->name()));
		
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

