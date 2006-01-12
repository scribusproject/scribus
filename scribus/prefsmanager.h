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

#include "scribusapi.h"
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
class SCRIBUS_API PrefsManager : public QObject
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

	void setup();
	/**
	* @brief Initialise the defaults for prefs in this class
	* Only set the GUI font stuff up if we have a GUI!!!
	*/
	void initDefaults();
	void initDefaultGUIFont(const QFont&);
	void initDefaultCheckerPrefs(CheckerPrefsList* cp);
	void initArrowStyles();
	//! \brief Locate our preferences
	QString setupPreferencesLocation();
	//! \brief copy 1.2 prefs XML before loading, and old .rc files that we do not yet convert
	bool copy12Preferences();
	//! \brief convert 1.2 style preferences to new XML format
	void convert12Preferences();
	const QString preferencesLocation();

	/*! \brief Read the preferences.
	\param fname optional filename with preferences. If is not given,
	the defualt is used. */
	void ReadPrefs(const QString & fname=QString::null);
	void ReadPrefsXML();

	/*! \brief Save the preferences.
	\param fname optional filename with preferences. If is not given,
	the defualt is used. */
	void SavePrefs(const QString & fname=QString::null);
	void SavePrefsXML();

	/*! \brief Writes the preferences physically to the file.
	Here is the XML file created itself.
	\param ho a file name to write
	\retval bool true on success, false on write error */
	bool WritePref(QString ho);
	/*! \brief Reads the preferences physically from the file.
	Here is the XML file parsed itself.
	\param ho a file name to write
	\retval bool true on success, false on write error */
	bool ReadPref(QString ho);

	void setGhostscriptExecutable(const QString&);
	void setImageEditorExecutable(const QString&);
	const QString ghostscriptExecutable();
	const QString imageEditorExecutable();
	//! \brief Get the users preferred preview resolution
	const int gsResolution();
	//! \brief Get the users preferred document directory
	const QString documentDir();
	void setDocumentDir(const QString& dirname);
	const int mouseWheelValue();
	//! \brief Get the user set display scale
	const double displayScale();
	//! \brief Get the GUI language from preferences
	const QString& guiLanguage();
	//! \brief Get the GUI style from preferences
	const QString& guiStyle();
	//! \brief Get the GUI style from preferences
	const int& guiFontSize();
	const int& paletteFontSize();
	void setKeyEntry(const QString&, const QString&, const QKeySequence&, const int&);
	void setShowStartupDialog(const bool);
	//! \brief Sets the preferences' color set
	void setColorSet(const ColorList&);
	//! \brief Sets the preferences' color set name
	void setColorSetName(const QString&);
	//! \brief Returns the preferences' color set
	const ColorList& colorSet();
	//! \brief Returns a pointer to the preferences' color set. Needed for now until colors are better defined
	ColorList* colorSetPtr();
	//! \brief Returns the preferences' color set name
	const QString& colorSetName();
	const bool GetAllFonts(bool showFontInfo);

	ApplicationPrefs* applicationPrefs();
	PrefsFile* applicationPrefsFile();
	const bool importingFrom12x();

	//! \brief Return the last error reported by a PrefsManager method
	const QString & lastError() const;

	/*! \brief Display a GUI alert informing the user that saving prefs failed.
	This might want to move to another class later, perhaps being triggered
	by a signal emitted here. */
	void alertSavePrefsFailed() const;
	void alertLoadPrefsFailed() const;

	//! \brief Temporarily public while this class takes shape so progress can happen and cvs can build
	struct ApplicationPrefs appPrefs;
	PrefsFile* prefsFile;
private:
	/**
	* @brief The only instance of PrefsManager available.
	*
	* PrefsManager is singleton and the instance can be queried with the method
	* instance().
	*/
	static PrefsManager* _instance;

	QString prefsLocation;
	bool importingFrom12;

	/*! \brief The last error message generated by a method of this class.
	Do not write "success" messages to this. */
	QString m_lastError;

};

#endif
