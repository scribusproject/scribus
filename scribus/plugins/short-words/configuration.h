/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _SCRIBUS_SHORTCFG_H_
#define _SCRIBUS_SHORTCFG_H_

#include <QObject>

class QString;
class QStringList;
class PrefsContext;

/*! \brief This is the Scribus Short Words configuratin specification.
Methods of this class read config files and PrefsManager context informations.
Preferences GUI is in SWPrefsGui.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

\author Petr Vanek <petr@yarpen.cz> with contributors.
*/
class SWConfig : public QObject
{
	Q_OBJECT

public:
	/*! \brief Reads config from the ui cfg file */
	SWConfig();
	/*! \brief Writes config into the ui cfg file */
	~SWConfig(){};

	/*! \brief Id of the UI radiobutton  */
	uint action;
	//*! \brief UI checkbox */
	bool useStyle;
	QString currentLanguage;

	/*! \brief Returns all options for specified language.
	\param lang language of the ext frame */
	QStringList getShortWords(QString lang);
	/*! \brief available configs for UI about.
	\retval QString with languages in the configuration. */
	static QString getAvailableLanguages();
	static QStringList getAvailableLanguagesList();

	/*! \brief Save cfg. */
	void saveConfig();

private:
	/*! \brief Configuration structure */
	PrefsContext* prefs;
	/*! \brief getShortWords use this one. for each case from GUI select the right config file (or both).
	\param lang language
	\param filename configuration file.
	\retval QStringList parsed SW. Each SW in one string item. */
	QStringList getShortWordsFromFile(QString lang, QString filename);
	/*! \brief getAvailableLanguages use this one. available config in the specified file for UI about
	\param filename config file
	\retval QStringList all supported language codes */
	static QStringList getAvailableLanguageCodes(QString filename);
	/*! \brief Retrieve translated language strings from language codes
	\retval QStringList with languages. */
	static QStringList getLanguageStringsFromCodes(QStringList codes);
};

#endif
