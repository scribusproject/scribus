/***************************************************************************
    begin                : Jan 2005
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
 
#include <iostream>

#include <qdir.h>
#include <qfileinfo.h>
#include <qmap.h>
#include <qobject.h>
#include <qstringlist.h> 

#include "scconfig.h"
#include "langmgr.h"
#include "scpaths.h"

void LanguageManager::init(bool generateInstalledList)
{
	//generateUntranslatedLangList();
	generateLangList();
	if (generateInstalledList)
		generateInstalledLangList();
}

/*
//CB Well we probably would like a QPair of these..but.. I'm lazy.
void LanguageManager::generateUntranslatedLangList()
{
	untranslatedLangList.insert("af",     "Afrikaans" );
	untranslatedLangList.insert("sq",     "Albanian" );
	untranslatedLangList.insert("eu",     "Basque" );
	untranslatedLangList.insert("bg",     "Bulgarian" );
	untranslatedLangList.insert("br",     "Brazilian" );
	untranslatedLangList.insert("ca",     "Catalan" );
	untranslatedLangList.insert("zh",     "Chinese" );
	untranslatedLangList.insert("hr",     "Croatian" );
	untranslatedLangList.insert("cs",     "Czech" );
	untranslatedLangList.insert("cs_CZ",  "Czech" );
	untranslatedLangList.insert("da",     "Danish" );
	untranslatedLangList.insert("da_DK",  "Danish" );
	untranslatedLangList.insert("nl",     "Dutch" );
	untranslatedLangList.insert("en",     "English" );
	untranslatedLangList.insert("en_GB",  "English (British)" );
	untranslatedLangList.insert("eo",     "Esperanto" );
	untranslatedLangList.insert("de",     "German" );
	untranslatedLangList.insert("fi",     "Finnish" );
	untranslatedLangList.insert("fr",     "French" );
	untranslatedLangList.insert("ga",     "Galician" );
	untranslatedLangList.insert("galician",  "Galician" );
	untranslatedLangList.insert("gl",     "Galician" );
	untranslatedLangList.insert("el",     "Greek" );
	untranslatedLangList.insert("hu",     "Hungarian" );
	untranslatedLangList.insert("id",     "Indonesian" );
	untranslatedLangList.insert("it",     "Italian" );
	untranslatedLangList.insert("ko",     "Korean" );
	untranslatedLangList.insert("lt",     "Lithuanian" );
	untranslatedLangList.insert("lt_LT",  "Lithuanian" );
	untranslatedLangList.insert("nb",     "Norwegian (Bokmaal)" );
	untranslatedLangList.insert("nb_NO",  "Norwegian (Bokmaal)" );
	untranslatedLangList.insert("nn",     "Norwegian (Nnyorsk)" );
	untranslatedLangList.insert("nn_NO",  "Norwegian (Nnyorsk)" );
	untranslatedLangList.insert("no",     "Norwegian" );
	untranslatedLangList.insert("no_NO",  "Norwegian" );
	untranslatedLangList.insert("pl",     "Polish" );
	untranslatedLangList.insert("pl_PL",  "Polish" );
	untranslatedLangList.insert("pt",     "Portuguese" );
	untranslatedLangList.insert("pt_BR",  "Portuguese (BR)" );
	untranslatedLangList.insert("ru",     "Russian" );
	untranslatedLangList.insert("se",     "Swedish" );
	untranslatedLangList.insert("es",     "Spanish" );
	untranslatedLangList.insert("es_ES",  "Spanish" );
	untranslatedLangList.insert("es_LA",  "Spanish (Latin)" );
	untranslatedLangList.insert("sk",     "Slovak" );
	untranslatedLangList.insert("sl",     "Slovenian" );
	untranslatedLangList.insert("sr",     "Serbian" );
	untranslatedLangList.insert("sv",     "Swedish" );
	untranslatedLangList.insert("tr",     "Turkish" );
	untranslatedLangList.insert("tr_TR",  "Turkish" );
	untranslatedLangList.insert("uk",     "Ukranian" );
	untranslatedLangList.insert("cy",     "Welsh" );
}
*/
void LanguageManager::generateLangList()
{
	langList.insert("af",    langPair("Afrikaans", QObject::tr( "Afrikaans" )) );
	langList.insert("sq",    langPair("Albanian", QObject::tr( "Albanian" )) );
	langList.insert("eu",    langPair("Basque", QObject::tr( "Basque" )) );
	langList.insert("bg",    langPair("Bulgarian", QObject::tr( "Bulgarian" )) );
	langList.insert("br",    langPair("Brazilian", QObject::tr( "Brazilian" )) );
	langList.insert("ca",    langPair("Catalan", QObject::tr( "Catalan" )) );
	langList.insert("zh",    langPair("Chinese", QObject::tr( "Chinese" )) );
	langList.insert("hr",    langPair("Croatian", QObject::tr( "Croatian" )) );
	langList.insert("cs",    langPair("Czech", QObject::tr( "Czech" )) );
	langList.insert("cs_CZ", langPair("Czech", QObject::tr( "Czech" )) );
	langList.insert("da",    langPair("Danish", QObject::tr( "Danish" )) );
	langList.insert("da_DK", langPair("Danish", QObject::tr( "Danish" )) );
	langList.insert("nl",    langPair("Dutch", QObject::tr( "Dutch" )) );
	langList.insert("en",    langPair("English", QObject::tr( "English" )) );
	langList.insert("en_GB", langPair("English (British)", QObject::tr( "English (British)" )) );
	langList.insert("eo",    langPair("Esperanto", QObject::tr( "Esperanto" )) );
	langList.insert("de",    langPair("German", QObject::tr( "German" )) );
	langList.insert("fi",    langPair("Finnish", QObject::tr( "Finnish" )) );
	langList.insert("fr",    langPair("French", QObject::tr( "French" )) );
	langList.insert("ga",    langPair("Galician", QObject::tr( "Galician" )) );
	langList.insert("galician", langPair("Galician", QObject::tr( "Galician" )) );
	langList.insert("gl",    langPair("Galician", QObject::tr( "Galician" )) );
	langList.insert("el",    langPair("Greek", QObject::tr( "Greek" )) );
	langList.insert("hu",    langPair("Hungarian", QObject::tr( "Hungarian" )) );
	langList.insert("id",    langPair("Indonesian", QObject::tr( "Indonesian" )) );
	langList.insert("it",    langPair("Italian", QObject::tr( "Italian" )) );
	langList.insert("ko",    langPair("Korean", QObject::tr( "Korean" )) );
	langList.insert("lt",    langPair("Lithuanian", QObject::tr( "Lithuanian" )) );
	langList.insert("lt_LT", langPair("Lithuanian", QObject::tr( "Lithuanian" )) );
	langList.insert("nb",    langPair("Norwegian (Bokmaal)", QObject::tr( "Norwegian (Bokmaal)" )) );
	langList.insert("nb_NO", langPair("Norwegian (Bokmaal)", QObject::tr( "Norwegian (Bokmaal)" )) );
	langList.insert("nn",    langPair("Norwegian (Nnyorsk)", QObject::tr( "Norwegian (Nnyorsk)" )) );
	langList.insert("nn_NO", langPair("Norwegian (Nnyorsk)", QObject::tr( "Norwegian (Nnyorsk)" )) );
	langList.insert("no",    langPair("Norwegian", QObject::tr( "Norwegian" )) );
	langList.insert("no_NO", langPair("Norwegian", QObject::tr( "Norwegian" )) );
	langList.insert("pl",    langPair("Polish", QObject::tr( "Polish" )) );
	langList.insert("pl_PL", langPair("Polish", QObject::tr( "Polish" )) );
	langList.insert("pt",    langPair("Portuguese", QObject::tr( "Portuguese" )) );
	langList.insert("pt_BR", langPair("Portuguese (BR)", QObject::tr( "Portuguese (BR)" )) );
	langList.insert("ru",    langPair("Russian", QObject::tr( "Russian" )) );
	langList.insert("se",    langPair("Swedish", QObject::tr( "Swedish" )) );
	langList.insert("es",    langPair("Spanish", QObject::tr( "Spanish" )) );
	langList.insert("es_ES", langPair("Spanish", QObject::tr( "Spanish" )) );
	langList.insert("es_LA", langPair("Spanish (Latin)", QObject::tr( "Spanish (Latin)" )) );
	langList.insert("sk",    langPair("Slovak", QObject::tr( "Slovak" )) );
	langList.insert("sl",    langPair("Slovenian", QObject::tr( "Slovenian" )) );
	langList.insert("sr",    langPair("Serbian", QObject::tr( "Serbian" )) );
	langList.insert("sv",    langPair("Swedish", QObject::tr( "Swedish" )) );
	langList.insert("tr",    langPair("Turkish", QObject::tr( "Turkish" )) );
	langList.insert("tr_TR", langPair("Turkish", QObject::tr( "Turkish" )) );
	langList.insert("uk",    langPair("Ukranian", QObject::tr( "Ukranian" )) );
	langList.insert("cy",    langPair("Welsh", QObject::tr( "Welsh" )) );
}

void LanguageManager::generateInstalledLangList()
{
	QString path = ScPaths::instance().libDir();
	QString langAbbrev;
	QMap<QString, langPair>::Iterator it;

	QDir dir(path , "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if (dir.exists() && (dir.count() != 0))
		for (uint i = 0; i < dir.count(); ++i) 
		{
			QFileInfo file(path + dir[i]);
			if (file.extension(false).lower() == "qm") 
			{
				langAbbrev = file.extension().remove(".qm");
				if ((it=langList.find(langAbbrev))!=langList.end())
						installedLangList.insert(it.key(), it.data().second);
			}
		}
}

const QString LanguageManager::getLangFromAbbrev(QString langAbbrev, bool getTranslated)
{
	QMap<QString, langPair>::Iterator it;
	if ((it=langList.find(langAbbrev))!=langList.end())
	{
		if (getTranslated)
			return it.data().second;
		else
			return it.data().first;
	}
	else
		return "";	
}

const QString LanguageManager::getAbbrevFromLang(QString lang, bool getTranslated)
{
	QMap<QString, langPair>::Iterator it;
	for (it=langList.begin();it!=langList.end();++it)
	{
		if (getTranslated && it.data().second==lang)
			return it.key();
		if (!getTranslated && it.data().first==lang)
			return it.key();
	}
	return "";	
}

void LanguageManager::fillInstalledStringList(QStringList *stringListToFill, bool addDefaults) 
{
	if (stringListToFill)
	{
		QMap<QString, QString>::Iterator it;

		if (addDefaults) 
		{
			stringListToFill->append("");
			stringListToFill->append( QObject::tr( "English" ));
		}

		for (it=installedLangList.begin();it!=installedLangList.end();++it)
			stringListToFill->append(it.data());
	}
}

void LanguageManager::printInstalledList()
{
	QMap<QString, QString>::Iterator it;

	for (it=installedLangList.begin();it!=installedLangList.end();++it)
		std::cout << it.key().leftJustify(6) << ": " << it.data() << std::endl;
}
