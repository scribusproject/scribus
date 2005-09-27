/* This is the Scribus Short Words configuratin implementation.
There will be interface for the future Scribus central plugin
config center. maybe :)

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributions by good people listed in AUTHORS file

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include "scconfig.h"
#include "configuration.h"
#include "configuration.moc"
#include "scpaths.h"
#include "version.h"

#include "scribus.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include <qdir.h>
#include <qstringlist.h>

extern ScribusApp SCRIBUS_API *ScApp;

SWConfig::SWConfig()
{
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("short-words");
	action = prefs->getUInt("action", 0);
	//userConfig = prefs->getUInt("userConfig", 0);
	//editor = prefs->get("editor", "");
}

void SWConfig::saveConfig()
{
	prefs->set("action", action);
	//prefs->set("userConfig", userConfig);
	//prefs->set("editor", editor);
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

	f.setName(filename);
	if (!f.exists())
	{
		qDebug("Short Words config file not found");
		return QStringList();
	}
	if (f.open(IO_ReadOnly))
	{
		QTextStream t(&f);
		while (!t.eof())
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
		return QStringList::split(",", shorts);
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

QString SWConfig::getAvailableLanguagesFromFile(QString filename)
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

QString SWConfig::getAvailableLanguages()
{
	QString allConfig = getAvailableLanguagesFromFile(RC_PATH);
	if (QFile::exists(RC_PATH_USR))
		allConfig += "<br>" + getAvailableLanguagesFromFile(RC_PATH_USR) + "";
	return  allConfig;
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

	for (it = ScApp->Sprachen.begin(); it != ScApp->Sprachen.end(); ++it)
	{
		lang = getLangCodeFromHyph(it.data());
		if (lang == code)
			return it.key();
	}
	return code;
}
