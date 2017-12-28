/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This is the Scribus Short Words configuratin implementation.
There will be interface for the future Scribus central plugin
config center. maybe :)

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributors.

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include "scconfig.h"
#include "configuration.h"
//#include "configuration.moc"
#include "scpaths.h"
#include "version.h"

#include "langmgr.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include <QDir>
#include <QStringList>


SWConfig::SWConfig()
{
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("short-words");
	action = prefs->getUInt("action", 0);
	//userConfig = prefs->getUInt("userConfig", 0);
	//editor = prefs->get("editor", "");
	useStyle = prefs->getBool("useStyle", true);
	currentLanguage = prefs->get("currentLanguage", "en");
}

void SWConfig::saveConfig()
{
	prefs->set("action", action);
	//prefs->set("userConfig", userConfig);
	//prefs->set("editor", editor);
	prefs->set("useStyle", useStyle);
	prefs->set("currentLanguage", currentLanguage);
}

QStringList SWConfig::getShortWordsFromFile(QString lang, QString filename)
{
	// path to the cfg. file
	QFile f(filename);
	if (!f.exists())
	{
		qDebug("Short Words config file not found");
		return QStringList();
	}

	// all shorts for one language
	QString shorts;
	// one line in cfg. file
	QString aRow;
	// cfg (doesn't) exists for the selected language indicator
	bool success = false;

	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream t(&f);
		while (!t.atEnd())
		{
			aRow = t.readLine();
			if (aRow.left(2) == lang.left(2))
			{
				success = true;
				shorts += aRow.remove(0, 3);
			}
		}
		f.close();
	}
	if (success)
		return shorts.split(",", QString::SkipEmptyParts);
	return QStringList();
}

QStringList SWConfig::getShortWords(QString lang)
{
	if (QFile::exists(RC_PATH_USR))
		return getShortWordsFromFile(lang, RC_PATH_USR);
	return getShortWordsFromFile(lang, RC_PATH);
}

QStringList SWConfig::getAvailableLanguageCodes(QString filename)
{
	QFile f(filename);
	if (!f.open(QIODevice::ReadOnly))
		return QStringList();

	QTextStream t(&f);
	QStringList nations;
	QString aRow, code;
	while (!t.atEnd())
	{
		aRow = t.readLine();
		code = aRow.left(2);
		if (aRow.left(1) != "#" && aRow.length() != 0 && aRow.left(1) != " " && !nations.contains(code))
		{
			nations.append(code);
		}
	}
	f.close();

	return nations;
}

QStringList SWConfig::getAvailableLanguagesList()
{
	QStringList allConfig;
	
	if (QFile::exists(RC_PATH_USR))
		allConfig = getAvailableLanguageCodes(RC_PATH_USR);
	else
		allConfig = getAvailableLanguageCodes(RC_PATH);
	return allConfig;
}

QString SWConfig::getAvailableLanguages()
{
	QStringList allCodes, allConfig;
	allCodes = getAvailableLanguageCodes(RC_PATH);
	allConfig << QObject::tr("Standard configuration: ", "short words plugin") << "<p>";
	allConfig << getLanguageStringsFromCodes(allCodes).join(", ");
	if (QFile::exists(RC_PATH_USR))
	{
		allCodes = getAvailableLanguageCodes(RC_PATH_USR);
		allConfig << "<p>" << QObject::tr("Custom (optional) configuration: ", "short words plugin") << "<p>";
		allConfig << getLanguageStringsFromCodes(allCodes).join(", ");
	}
	return allConfig.join("");
}

QStringList SWConfig::getLanguageStringsFromCodes(QStringList codes)
{
	QStringList languages;

	for (int i = 0; i < codes.count(); ++i)
	{
		QString code = codes.at(i);
		QString lang = LanguageManager::instance()->getLangFromAbbrev(code, true);
		if (lang.length() > 0)
			languages.append(lang);
	}

	return languages;
}
