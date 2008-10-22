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
	currentLanguage = prefs->getInt("currentLanguage", 0);
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
	// all shorts for one language
	QString shorts = "";
	// one line in cfg. file
	QString aRow;
	// cfg (doesn't) exists for the selected language indicator
	bool success = false;
	// path to the cfg. file
	QFile f;

	f.setFileName(filename);
	if (!f.exists())
	{
		qDebug("Short Words config file not found");
		return QStringList();
	}
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream t(&f);
		while (!t.atEnd())
		{
			aRow = t.readLine();
			if (aRow.left(2) == lang)
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
	//QStringList allShorts;
	if (QFile::exists(RC_PATH_USR))
		return getShortWordsFromFile(lang, RC_PATH_USR);
	return getShortWordsFromFile(lang, RC_PATH);
	/*if (userConfig && QFile::exists(RC_PATH_USR))
		return getShortWordsFromFile(lang, RC_PATH_USR);
	if (!userConfig && QFile::exists(RC_PATH_USR))
		allShorts = getShortWordsFromFile(lang, RC_PATH_USR);
	return allShorts + getShortWordsFromFile(lang, RC_PATH);*/
}

QStringList SWConfig::getAvailableLanguagesFromFile(QString filename)
{
	QStringList langs;
	QStringList nations;
	QString aRow;

	QFile f(filename);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream t(&f);
		while (!t.atEnd())
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
		return QStringList();
	}
// 	if (filename == RC_PATH_USR)
// 		return QObject::tr("Custom (optional) configuration: ", "short words plugin") + " " + nations.join(", ");
// 	if (filename == RC_PATH)
// 		return QObject::tr("Standard configuration: ", "short words plugin") + " " + nations.join(", ");
	return nations; //.join(", "); // save return only
}

QStringList SWConfig::getAvailableLanguagesList()
{
	QStringList allConfig;
	
	if (QFile::exists(RC_PATH_USR))
		allConfig << getAvailableLanguagesFromFile(RC_PATH_USR);
	else
		allConfig << getAvailableLanguagesFromFile(RC_PATH);
	return allConfig;
}

QString SWConfig::getAvailableLanguages()
{
	QStringList allConfig;
	allConfig << QObject::tr("Standard configuration: ", "short words plugin") << "<p>";
	allConfig << getAvailableLanguagesFromFile(RC_PATH).join(", ");
	if (QFile::exists(RC_PATH_USR))
	{
		allConfig << "<p>" << QObject::tr("Custom (optional) configuration: ", "short words plugin") << "<p>";
		allConfig << getAvailableLanguagesFromFile(RC_PATH_USR).join(", ");
	}
	return  allConfig.join("");
}

QString SWConfig::getLangCodeFromHyph(QString hyphenCode)
{
	hyphenCode.remove(0, 5);
	return hyphenCode.remove(2, 10);
}

QString SWConfig::getLangFromCode(QString code)
{
	QMap<QString,QString>::Iterator it;
	QString lang;
	LanguageManager langmgr;
	langmgr.init(false);
	return langmgr.getLangFromAbbrev(code, true);
}
