/*! This is the Scribus Short Words configuratin specification.
There will be interface for the future Scribus central plugin
config center. maybe :)

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributions by good people listed in AUTHORS file

This program is free software - see LICENSE file in the distribution
or documentation
*/

#ifndef _SCRIBUS_SHORTCFG_H_
#define _SCRIBUS_SHORTCFG_H_

#include <qstring.h>
#include <qstringlist.h>
#include "scribus.h"

class QString;
class QStringList;

class Config : public QObject
{
	Q_OBJECT

public:
	/** reads config from the ui cfg file */
	Config();
	/** writes config into the ui cfg file */
	~Config();

	/** id of the UI radiobutton  */
	uint action;
	/** UI checkbox */
	uint userConfig;
	/** User's text editor */
	QString editor;

	/** returns all options for specified language */
	QStringList getShortWords(QString lang);
	/** available configs for UI about */
	QString getAvailableLanguages();
	/** get lang code (en, cs, pl) from Scribus hyphenator configuration
	structure */
	QString getLangCodeFromHyph(QString hyphenCode);
	/** Save cfg. */
	void saveConfig();

private:
	/** Configuration structure */
	PrefsContext* prefs;
	/** getShortWords use this one. for each case from GUI select
	the right config file (or both) */
	QStringList getShortWordsFromFile(QString lang, QString filename);
	/** getAvailableLanguages use this one. available config in
	the specified file for UI about */
	QString getAvailableLanguagesFromFile(QString filename);
	/** returns full lang name by lang code (en -> English) */
	QString getLangFromCode(QString code);
};

#endif
