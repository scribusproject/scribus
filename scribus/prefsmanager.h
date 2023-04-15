/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QKeySequence>
#include <QMap>
#include <QObject>
#include <QStringList>

#include "prefsstructs.h"
#include "resourcecollection.h"
#include "scribusapi.h"

class PrefsFile;
class ScribusMainWindow;


/**
  * @author Craig Bradney
  * @brief Manage Scribus preferences here, and here alone
  * Start to move the preferences out of scribus.cpp and provide some
  * more features and hide some of the data within this class
  * TODO Move prefsFile in here from scribus.cpp and stop passing it in for convertOldPreferences
  * TODO Lots more :)
  */
class SCRIBUS_API PrefsManager : public QObject
{
Q_OBJECT
public:
	PrefsManager(PrefsManager const&) = delete;
	void operator=(PrefsManager const&) = delete;
	/**
	* @brief Returns a pointer to the PrefsManager instance
	* @return A pointer to the PrefsManager instance
	*/
	static PrefsManager& instance();

	void setup();
	/*!
	\brief Initialise the defaults for prefs in this class
	Only set the GUI font stuff up if we have a GUI!!!
	Must be run after: PrefsManager::GetAllFonts()
	*/
	void initDefaults();
	void initDefaultGUIFont(const QFont&);
	void initDefaultCheckerPrefs(CheckerPrefsList& cp);
	void initDefaultActionKeys();
	void applyLoadedShortCuts();
	void initArrowStyles();

	/*!
	\brief Insert checker profiles not available in specified list
	*/
	void insertMissingCheckerProfiles(CheckerPrefsList& cp);

	/*!
	\author Craig Bradney
	\date Thu 18 Nov 2004
	\brief Set the user's preference file location. Rename any existing old preferences files
	\retval QString Location of the user's preferences
	*/
	void setupPreferencesLocation();
	/*!
	\author Craig Bradney
	\date Sun 09 Jan 2005
	\brief Copy 1.2 prefs XML before loading, and copy rc files we don't yet convert
	\retval bool true if prefs were imported
	*/
	void copyOldAppConfigAndData();

	/*!
	\brief Get the prefs location property
	\retval QString Location of the user's preferences
	*/
	const QString& preferencesLocation() const;

	/*! \brief Read the preferences.
	\param fname optional filename with preferences. If is not given,
	the default is used. */
	void readPrefs();
	void readPrefsXML();

	/*! \brief Save the preferences.
	\param fname optional filename with preferences. If is not given,
	the default is used. */
	void savePrefs();
	void savePrefsXML();

	/*! \brief Writes the preferences physically to the file.
	Here is the XML file created itself.
	\param ho a file name to write
	\retval bool true on success, false on write error */
	bool writePref(const QString& filePath);
	/*! \brief Reads the preferences physically from the file.
	Here is the XML file parsed itself. Returns false on error. 
	It's the caller's job to make sure the prefs file actually exists.
	\param ho a file name to write
	\retval bool true on success, false on write error */
	bool readPref(const QString& filePath);

	void setGhostscriptExecutable(const QString&);
	void setImageEditorExecutable(const QString&);
	void setExtBrowserExecutable(const QString&);
	void setUniconvExecutable(const QString&);
	void setLatexConfigs(const QStringList&);
	void setLatexCommands(const QMap<QString, QString>& commands);
	void setLatexEditorExecutable(const QString&);
	QString ghostscriptExecutable() const {return appPrefs.extToolPrefs.gs_exe;};
	QString imageEditorExecutable() const {return appPrefs.extToolPrefs.imageEditorExecutable;};
	QString extBrowserExecutable() const {return appPrefs.extToolPrefs.extBrowserExecutable;};
	QString uniconvExecutable() const {return appPrefs.extToolPrefs.uniconvExecutable;}
	QStringList latexConfigs() const {return appPrefs.extToolPrefs.latexConfigs;}
	QMap<QString, QString> latexCommands() const {return appPrefs.extToolPrefs.latexCommands;}
	bool renderFrameConfigured();
	QString latexEditorExecutable() const {return appPrefs.extToolPrefs.latexEditorExecutable;}
	bool latexStartWithEmptyFrames() const {return appPrefs.extToolPrefs.latexStartWithEmptyFrames;}
	//! \brief Get the users preferred preview resolution
	int gsResolution() const {return appPrefs.extToolPrefs.gs_Resolution;}
	int latexResolution() const {return appPrefs.extToolPrefs.latexResolution;}
	bool latexForceDpi() const {return appPrefs.extToolPrefs.latexForceDpi;}
	//! \brief Get the users preferred document directory
	const QString& documentDir() const;
	void setDocumentDir(const QString& dirname);
	int mouseWheelJump() const;
	//! \brief Get the user set display scale
	double displayScale() const;
	//! \brief Get the GUI language from preferences
	const QString& uiLanguage() const;
	//! \brief Get the GUI style from preferences
	const QString& guiStyle() const;
	const QString& guiSystemStyle() const;
	//! \brief Get the GUI icon set from preferences
	const QString& guiIconSet() const;
	//! \brief Get the GUI style from preferences
	const int& guiFontSize() const;
	const int& paletteFontSize() const;
	void setKeyEntry(const QString&, const QString&, const QKeySequence&, const int&);
	void setShowStartupDialog(const bool);
	bool showPageShadow() const;
	//! \brief Sets the preferences' color set
	void setColorSet(const ColorList&);
	//! \brief Sets the preferences' color set name
	void setColorSetName(const QString&);
	//! \brief Returns the preferences' color set
	const ColorList& colorSet() const;
	//! \brief Returns a pointer to the preferences' color set. Needed for now until colors are better defined
	ColorList* colorSetPtr();
	//! \brief Returns the preferences' color set name
	const QString& colorSetName() const;
	/*! \brief Return if a color belongs to tools prefs colors */
	bool isToolColor(const QString& name) const;
	static bool isToolColor(const ItemToolPrefs& settings, const QString& name);
	/*! \brief Return the list of used colors in tool prefs */
	QStringList toolColorNames() const;
	static QStringList toolColorNames(const ItemToolPrefs& settings);
	/*! \brief Replace used colors in tool prefs */
	void replaceToolColors(const QMap<QString, QString>& replaceMap);
	static void replaceToolColors(ItemToolPrefs& settings, const QMap<QString, QString>& replaceMap);
	/*! \brief Replace used resources in tool prefs */
	void replaceToolResources(const ResourceCollection& newNames);
	static void replaceToolResources(ItemToolPrefs& settings, const ResourceCollection& newNames);
	/*! \brief Finds the fonts on the system
	Must be run after: PrefsManager::setup()
	Must be run before: PrefsManager::initDefaults()
	\param showFontInfo flag to get extended fotn info
	\retval bool true on finding 1 or more fonts */
	bool GetAllFonts(bool showFontInfo);

	ApplicationPrefs* applicationPrefs();
	void setNewPrefs(const ApplicationPrefs& newPrefs);
	PrefsFile* applicationPrefsFile();

	//! \brief Return the last error reported by a PrefsManager method
	const QString & lastError() const;

	/*! \brief Display a GUI alert informing the user that saving prefs failed.
	This might want to move to another class later, perhaps being triggered
	by a signal emitted here. */
	void alertSavePrefsFailed() const;
	void alertLoadPrefsFailed() const;

	//! \brief Temporarily public while this class takes shape so progress can happen and cvs can build
	struct ApplicationPrefs appPrefs;
	PrefsFile* prefsFile {nullptr};
	bool firstTimeIgnoreOldPrefs() const;

public slots:
	void languageChange();

private:
	PrefsManager(QObject *parent = nullptr);
	~PrefsManager();

	QString m_prefsLocation;
	bool m_firstTimeIgnoreOldPrefs {false};

	/*! \brief The last error message generated by a method of this class.
	Do not write "success" messages to this. */
	QString m_lastError;

signals:
	void prefsChanged();
};

#endif
