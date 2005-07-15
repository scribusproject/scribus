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
#ifndef PREFSMANAGER_H
#define PREFSMANAGER_H

#include <qobject.h>

#include "prefsstructs.h"

/**
  * @author Craig Bradney
  * @brief Manage Scribus preferences here, and here alone
  * Start to move the preferences out of scribus.cpp and provide some 
  * more features and hide some of the data within this class
  */
class PrefsManager : public QObject
{
Q_OBJECT
public:
	PrefsManager(QObject *parent = 0, const char *name = 0);

	~PrefsManager();
	
	ApplicationPrefs* applicationPrefs();
	/*
	 * @brief Returns a pointer to the UndoManager instance
     * @return A pointer to the UndoManager instance
     */
    static PrefsManager* instance();
    /**
     * @brief Deletes the PrefsManager Instance
     *
     * Must be called when PrefsManager is no longer needed.
     */
    static void deleteInstance();

	/**
	* @brief The only instance of PrefsManager available.
	*
	* PrefsManager is singleton and the instance can be queried with the method
	* instance().
	*/
	struct ApplicationPrefs appPrefs;
private:
	static PrefsManager* _instance;

};

#endif
