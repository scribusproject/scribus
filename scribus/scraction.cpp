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
#include <qpopupmenu.h>
#include <qiconset.h>
#include "scraction.h"
#include "scraction.moc"


ScrAction::ScrAction( QObject * parent, const char * name ) : QAction( parent, name )
{
	menuType=ScrAction::Normal;
}

ScrAction::ScrAction( const QString & menuText, QKeySequence accel, QObject * parent, const char * name ) : QAction( menuText, accel, parent, name )
{
	menuType=ScrAction::Normal;
}

ScrAction::ScrAction( MenuType mType, const QString & menuText, QKeySequence accel, QObject * parent, const char * name ) : QAction( menuText, accel, parent, name )
{
	menuType=mType;
	switch (mType)
	{
		case RecentFile:
			connect (this, SIGNAL(activated()), this, SLOT(activatedToActivatedRecentFile()));
			break;
		case DLL:
			qDebug("Wrong constructor for now!");
			break;
		case Normal:
		default:
			break;
	}
}

ScrAction::ScrAction( const QIconSet & icon, const QString & menuText, QKeySequence accel, QObject * parent, const char * name ) : QAction( icon, menuText, accel, parent, name )
{
	menuType=ScrAction::Normal;
	setIconSizes();
}

ScrAction::ScrAction( const int dllID, const QString & menuText, QKeySequence accel, QObject * parent, const char * name) : QAction( menuText, accel, parent, name )
{
	pluginID=dllID;
	menuType=ScrAction::DLL;
	connect (this, SIGNAL(activated()), this, SLOT(activatedToActivatedDLL()));
}


ScrAction::ScrAction( const int dllID, const QIconSet & icon, const QString & menuText, QKeySequence accel, QObject * parent, const char * name ) : QAction( icon, menuText, accel, parent, name )
{
	pluginID=dllID;
	menuType=ScrAction::DLL;
	setIconSizes();
	connect (this, SIGNAL(activated()), this, SLOT(activatedToActivatedDLL()));
}


ScrAction::~ScrAction()
{
}

void ScrAction::setIconSizes()
{
	QIconSet iconset=iconSet();
	if (!iconset.isNull())
	{
		iconset.setIconSize(QIconSet::Small, QSize(16,16));
		iconset.setIconSize(QIconSet::Large, QSize(22,22));
	}
}

void ScrAction::activatedToActivatedDLL()
{
	if (menuType==ScrAction::DLL)
		emit activatedDLL(pluginID);
}

void ScrAction::activatedToActivatedRecentFile()
{
	if (menuType==ScrAction::RecentFile)
		emit activatedRecentFile(menuText());
}

void ScrAction::addedTo ( int index, QPopupMenu * menu )
{
	menuIndex=index;
	popupMenuAddedTo=menu;
}


void ScrAction::addedTo( QWidget * actionWidget, QWidget * container )
{
	widgetAddedTo = actionWidget;
	containerWidgetAddedTo = container;
}


QWidget* ScrAction::getWidgetAddedTo()
{
	return widgetAddedTo;
}

const int ScrAction::getMenuIndex()
{
	return menuIndex;
}

bool ScrAction::addTo ( QWidget * w )
{
	widgetAddedTo=w;
	return (QAction::addTo(w));
}


