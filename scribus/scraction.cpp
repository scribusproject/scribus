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
#include <QMenu>
#include <QIcon>
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"

ScrAction::ScrAction( QObject * parent ) : QAction( parent )
{
	initScrAction();
}

ScrAction::ScrAction( const QString & menuText, QKeySequence accel, QObject * parent ) : QAction( menuText, parent )
{
	setShortcut(accel);
	initScrAction();
}

ScrAction::ScrAction( ActionType aType, const QPixmap & icon16, const QPixmap & icon22, const QString & menuText, QKeySequence accel, QObject * parent, int extraInt, double extraDouble, QString extraQString ) : QAction( QIcon(icon16), menuText, parent )
{
	setShortcut(accel);
	initScrAction();
	icon().addPixmap(icon22, QIcon::Normal, QIcon::On);
	setIconSizes();
	_actionType=aType;

	if (_actionType!=Normal)
		connect (this, SIGNAL(activated()), this, SLOT(activatedToActivatedData()));
	switch (_actionType)
	{
		case DataInt:
			_dataInt=extraInt;
			break;
		case DataDouble:
			_dataDouble=extraDouble;
			break;
		case DataQString:
			_dataQString=extraQString;
			break;
		case RecentFile:
			_dataQString=extraQString;
			break;
		case RecentPaste:
			_dataQString=extraQString;
			break;
		case DLL:
			pluginID=extraInt;
			break;
		case Window:
			windowID=extraInt;
			break;
		case RecentScript:
			break;
		case UnicodeChar:
			_dataInt=extraInt;
			_dataQString=extraQString;
			break;
		case Layer:
			layerID=extraInt;
			break;
		case Normal:
		default:
			break;
	}
}

ScrAction::ScrAction( const QPixmap & icon16, const QPixmap & icon22, const QString & menuText, QKeySequence accel, QObject * parent ) : QAction( QIcon(icon16), menuText, parent )
{
	setShortcut(accel);
	initScrAction();
	icon().addPixmap(icon22, QIcon::Normal, QIcon::On);
	setIconSizes();
}

void ScrAction::initScrAction()
{
	_actionType=ScrAction::Normal;
	menuIndex=-1;
	savedKeySequence=QKeySequence("");
	shortcutSaved=false;
	fakeToggle=false;
}

ScrAction::~ScrAction()
{
}

void ScrAction::setIconSizes()
{
//qt4 fixme
/*
	QIcon iconset=iconSet();
	if (!iconset.isNull())
	{
		iconset.setIconSize(QIcon::Small, QSize(16,16));
		iconset.setIconSize(QIcon::Large, QSize(22,22));
	}
*/
}

void ScrAction::activatedToActivatedData()
{
	if (_actionType==ScrAction::DataInt)
		emit activatedData(_dataInt);
	if (_actionType==ScrAction::DataDouble)
		emit activatedData(_dataDouble);
	if (_actionType==ScrAction::DataQString)
		emit activatedData(_dataQString);
	if (_actionType==ScrAction::DLL)
		emit activatedData(pluginID);
	if (_actionType==ScrAction::Window)
		emit activatedData(windowID);
	if (_actionType==ScrAction::RecentFile)
		emit activatedData(_dataQString);
	if (_actionType==ScrAction::RecentPaste)
		emit activatedData(_dataQString);
	if (_actionType==ScrAction::RecentScript)
		emit activatedData(menuText());
	if (_actionType==ScrAction::UnicodeChar)
		activatedUnicodeShortcut(_dataQString, _dataInt);
	if (_actionType==ScrAction::Layer)
		emit activatedData(layerID);
	if (_actionType==ScrAction::ActionDLL)
		emit activatedData(((ScribusMainWindow*)parent())->doc);
}

void ScrAction::toggledToToggledData(bool ison)
{
	if (isToggleAction())
	{
		if (_actionType==ScrAction::DataInt)
			emit toggledData(ison, _dataInt);
		if (_actionType==ScrAction::DataDouble)
			emit toggledData(ison, _dataDouble);
		if (_actionType==ScrAction::DataQString)
			emit toggledData(ison, _dataQString);
		if (_actionType==ScrAction::DLL)
			emit toggledData(ison, pluginID);
		if (_actionType==ScrAction::Window)
			emit toggledData(ison, windowID);
		if (_actionType==ScrAction::RecentFile)
			emit toggledData(ison, _dataQString);
		if (_actionType==ScrAction::RecentPaste)
			emit toggledData(ison, _dataQString);
		if (_actionType==ScrAction::RecentScript)
			emit toggledData(ison, menuText());
		if (_actionType==ScrAction::Layer)
			emit toggledData(ison, layerID);
		// no toggle for UnicodeChar
	}
}

void ScrAction::addedTo ( int index, QMenu * menu )
{
	if (menuIndex==-1) // Add the first time, not for secondary popups.
	{
		menuIndex=index;
		popupMenuAddedTo=menu;
	}
}

QString ScrAction::cleanMenuText()
{
	return menuText().remove('&').remove("...");
}

const int ScrAction::getMenuIndex()
{
	return menuIndex;
}

const bool ScrAction::isDLLAction()
{
	return _actionType==ScrAction::DLL;
}

const int ScrAction::dllID()
{
	if (_actionType==ScrAction::DLL)
		return pluginID;
	return -1;
}

void ScrAction::setToggleAction(bool isToggle, bool isFakeToggle)
{
	if (_actionType!=Normal)
	{
		if (isToggle)
			connect(this, SIGNAL(toggled(bool)), this, SLOT(toggledToToggledData(bool)));
		else
			disconnect(this, SIGNAL(toggled(bool)), this, SLOT(toggledToToggledData(bool)));
	}
	QAction::setToggleAction(isToggle);
	fakeToggle=isFakeToggle;
	//if (fakeToggle)
		//connect(this, toggled(bool), this, activated());
}

void ScrAction::saveShortcut()
{
	if(!shortcutSaved)
	{
		savedKeySequence=accel();
		setAccel(QKeySequence(""));
		shortcutSaved=true;
	}
}

void ScrAction::restoreShortcut()
{
	if (shortcutSaved)
	{
		setAccel(savedKeySequence);
		savedKeySequence=QKeySequence("");
		shortcutSaved=false;
	}
}

ScrAction::ActionType ScrAction::actionType()
{
	return _actionType;
}

const int ScrAction::actionInt()
{
	return _dataInt;
}

const double ScrAction::actionDouble()
{
	return _dataDouble;
}

const QString ScrAction::actionQString()
{
	return _dataQString;
}

void ScrAction::setTexts(const QString &newText, bool setTextToo)
{
	QAction::setMenuText(newText);
	if (setTextToo)
		QAction::setText(cleanMenuText());
}

void ScrAction::toggle()
{
	QAction::toggle();
	if (fakeToggle)
		emit activated();
}
