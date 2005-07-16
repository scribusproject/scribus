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

class PrefsFile;

/**
  * @author Craig Bradney
  * @brief Manage Scribus preferences here, and here alone
  * Start to move the preferences out of scribus.cpp and provide some 
  * more features and hide some of the data within this class
  * TODO Move prefsFile in here from scribus.cpp and stop passing it in for convert12Preferences
  * TODO Lots more :)
  */
class PrefsManager : public QObject
{
Q_OBJECT
public:
	PrefsManager(QObject *parent = 0, const char *name = 0);
	~PrefsManager();
	
	/**
	* @brief Returns a pointer to the PrefsManager instance
	* @return A pointer to the PrefsManager instance
	*/
	static PrefsManager* instance();
	/**
	* @brief Deletes the PrefsManager Instance
	* Must be called when PrefsManager is no longer needed.
	*/
	static void deleteInstance();

	/**
	* @brief Initialise the defaults for prefs in this class
	* Only set the GUI font stuff up if we have a GUI!!!
	*/
	void initDefaults();
	void initDefaultGUIFont(const QFont&);
	void initArrowStyles();
	///Locate our preferences
	QString setupPreferencesLocation();
	/// copy 1.2 prefs XML before loading, and old .rc files that we don't yet convert
	bool copy12Preferences();
	/// convert 1.2 style preferences to new XML format
	void convert12Preferences(PrefsFile &);
	const QString preferencesLocation();

	ApplicationPrefs* applicationPrefs();
	
	
	struct ApplicationPrefs appPrefs;
private:
	/**
	* @brief The only instance of PrefsManager available.
	*
	* PrefsManager is singleton and the instance can be queried with the method
	* instance().
	*/
	static PrefsManager* _instance;
	
	QString prefsLocation;

};

#endif
