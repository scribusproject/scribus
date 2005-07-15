/***************************************************************************
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
#include "prefsmanager.h"

PrefsManager* PrefsManager::_instance = 0;

PrefsManager::PrefsManager(QObject *parent, const char *name)
 : QObject(parent, name)
{
}

PrefsManager::~PrefsManager()
{
}

PrefsManager* PrefsManager::instance()
{
    if (_instance == 0)
        _instance = new PrefsManager();

    return _instance;
}

void PrefsManager::deleteInstance()
{
    if (_instance)
        delete _instance;
    _instance = 0;
}


ApplicationPrefs* PrefsManager::applicationPrefs()
{
	return &appPrefs;
}
