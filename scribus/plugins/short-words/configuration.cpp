/*! This is the Scribus Short Words configuratin implementation.
There will be interface for the future Scribus central plugin
config center. maybe :)

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributions by good people listed in AUTHORS file

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include "config.h"
#include "configuration.h"
#include "configuration.moc"
#include "version.h"
#include "shortwords.h"

#include "scribus.h"
#include "prefsfile.h"
#include <qdir.h>
#include <qstringlist.h>

extern ScribusApp *ScApp;
extern ShortWords *shortWords;
extern PrefsFile *prefsFile;

Config::Config()
{
	prefs = prefsFile->getPluginContext("short-words");
	action = prefs->getUInt("action", 0);
	userConfig = prefs->getUInt("userConfig", 0);
}

Config::~Config()
{
}

void Config::saveConfig()
{
	prefs->set("action", action);
	prefs->set("userConfig", userConfig);
}

QStringList Config::getShortWordsFromFile(QString lang, QString filename)
{
	// all shorts for one language
	QString shorts = "";
	// one line in cfg. file
	QString aRow;
	// cfg (doesn't) exists for the selected language indicator
	bool success = FALSE;
	// path to the cfg. file
	QFile f;

	f.setName(filename);
	if (!f.exists())
		return QStringList();
	if (f.open(IO_ReadOnly))
	{
		QTextStream t(&f);
		while (!t.eof())
		{
			aRow = t.readLine();
			if (aRow.left(2) == lang)
			{
				success = TRUE;
				shorts += aRow.remove(0, 3);
			}
		}
		f.close();
	}
	if (success)
		return QStringList::split(",", shorts);
	return QStringList();
}

QStringList Config::getShortWords(QString lang)
{
	QStringList allShorts;
	if (userConfig && QFile::exists(RC_PATH_USR))
		return getShortWordsFromFile(lang, RC_PATH_USR);
	if (!userConfig && QFile::exists(RC_PATH_USR))
		allShorts = getShortWordsFromFile(lang, RC_PATH_USR);
	return allShorts + getShortWordsFromFile(lang, RC_PATH);
}

QString Config::getAvailableLanguagesFromFile(QString filename)
{
	QStringList langs;
	QStringList nations;
	QString aRow;

	QFile f(filename);
	if (f.open(IO_ReadOnly))
	{
		QTextStream t(&f);
		while (!t.eof())
		{
			aRow = t.readLine();
			if (aRow.left(1) != "#" && aRow.length() != 0 && aRow.left(1) != " " && !langs.contains(aRow.left(2)))
			{
				nations.append(getLangFromCode(aRow.left(2)));
				langs.append(aRow.left(2));
			}
		}
		f.close();
	}
	else
	{
		return QString();
	}
	if (filename == RC_PATH_USR)
		return QObject::tr("Custom (optional) configuration: ", "short words plugin") + " " + nations.join(", ");
	if (filename == RC_PATH)
		return QObject::tr("Standard configuration: ", "short words plugin") + " " + nations.join(", ");
	return nations.join(", "); // save return only
}

QString Config::getAvailableLanguages()
{
	QString allConfig = getAvailableLanguagesFromFile(RC_PATH);
	if (QFile::exists(RC_PATH_USR))
		allConfig += "<br>" + getAvailableLanguagesFromFile(RC_PATH_USR) + "";
	return  allConfig;
}

QString Config::getLangCodeFromHyph(QString hyphenCode)
{
	hyphenCode.remove(0, 5);
	return hyphenCode.remove(2, 10);
}

QString Config::getLangFromCode(QString code)
{
	QMap<QString,QString>::Iterator it;
	QString lang;

	for (it = ScApp->Sprachen.begin(); it != ScApp->Sprachen.end(); ++it)
	{
		lang = getLangCodeFromHyph(it.data());
		if (lang == code)
			return it.key();
	}
	return code;
}
