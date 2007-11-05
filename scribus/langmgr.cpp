/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QStringList> 

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

void LanguageManager::generateLangList()
{
	langList.insert("af",       langPair("Afrikaans",           QObject::tr( "Afrikaans" )) );
	langList.insert("ar",       langPair("Arabic",              QObject::tr( "Arabic" )) );
	langList.insert("sq",       langPair("Albanian",            QObject::tr( "Albanian" )) );
	langList.insert("eu",       langPair("Basque",              QObject::tr( "Basque" )) );
	langList.insert("bg",       langPair("Bulgarian",           QObject::tr( "Bulgarian" )) );
	langList.insert("br",       langPair("Breton",              QObject::tr( "Breton" )) );
	langList.insert("ca",       langPair("Catalan",             QObject::tr( "Catalan" )) );
	langList.insert("zh",       langPair("Chinese",             QObject::tr( "Chinese" )) );
	langList.insert("zh_TW",    langPair("Chinese (Trad.)",     QObject::tr( "Chinese (Trad.)" )) );
	langList.insert("hr",       langPair("Croatian",            QObject::tr( "Croatian" )) );
	langList.insert("cs",       langPair("Czech",               QObject::tr( "Czech" )) );
	langList.insert("cs_CZ",    langPair("Czech",               QObject::tr( "Czech" )) );
	langList.insert("da",       langPair("Danish",              QObject::tr( "Danish" )) );
	langList.insert("da_DK",    langPair("Danish",              QObject::tr( "Danish" )) );
	langList.insert("dz",       langPair("Dzongkha",            QObject::tr( "Dzongkha" )) );
	langList.insert("nl",       langPair("Dutch",               QObject::tr( "Dutch" )) );
	langList.insert("en",       langPair("English",             QObject::tr( "English" )) );
	langList.insert("en_AU",    langPair("English (Australian)",QObject::tr( "English (Australian)" )) );
	langList.insert("en_GB",    langPair("English (British)",   QObject::tr( "English (British)" )) );
	langList.insert("en_US",    langPair("English (American)",  QObject::tr( "English (American)" )) );
	langList.insert("eo",       langPair("Esperanto",           QObject::tr( "Esperanto" )) );
	langList.insert("et",       langPair("Estonian",            QObject::tr( "Estonian" )) );
	langList.insert("de",       langPair("German",              QObject::tr( "German" )) );
	langList.insert("de_CH",    langPair("German (Swiss)",              QObject::tr( "German (Swiss)" )) );
	langList.insert("de_ol",    langPair("German (Trad.)",      QObject::tr( "German (Trad.)" )) );
	langList.insert("fi",       langPair("Finnish",             QObject::tr( "Finnish" )) );
	langList.insert("fr",       langPair("French",              QObject::tr( "French" )) );
	langList.insert("ga",       langPair("Galician",            QObject::tr( "Galician" )) );
	langList.insert("galician", langPair("Galician",            QObject::tr( "Galician" )) );
	langList.insert("gl",       langPair("Galician",            QObject::tr( "Galician" )) );
	langList.insert("el",       langPair("Greek",               QObject::tr( "Greek" )) );
	langList.insert("he",       langPair("Hebrew",              QObject::tr( "Hebrew" )) );
	langList.insert("hu",       langPair("Hungarian",           QObject::tr( "Hungarian" )) );
	langList.insert("ia",       langPair("Latin",               QObject::tr( "Latin" )) );
	langList.insert("is",       langPair("Icelandic",           QObject::tr( "Icelandic" )) );
	langList.insert("id",       langPair("Indonesian",          QObject::tr( "Indonesian" )) );
	langList.insert("it",       langPair("Italian",             QObject::tr( "Italian" )) );
	langList.insert("ja",       langPair("Japanese",            QObject::tr( "Japanese" )) );
	langList.insert("km",       langPair("Khmer",               QObject::tr( "Khmer" )) );
	langList.insert("ko",       langPair("Korean",              QObject::tr( "Korean" )) );
	langList.insert("lo",       langPair("Lao",                 QObject::tr( "Lao" )) );
	langList.insert("la",       langPair("Latin",               QObject::tr( "Latin" )) );
	langList.insert("lt",       langPair("Lithuanian",          QObject::tr( "Lithuanian" )) );
	langList.insert("lt_LT",    langPair("Lithuanian",          QObject::tr( "Lithuanian" )) );
	langList.insert("lu",       langPair("Luxembourgish",       QObject::tr( "Luxembourgish" )) );
	//We might need &#00E5; for this accented a
	langList.insert("nb",       langPair("Norwegian (Bokmål)",  QObject::trUtf8( "Norwegian (Bokm\225l)" )) );
	langList.insert("nb_NO",    langPair("Norwegian (Bokmål)",  QObject::trUtf8( "Norwegian (Bokm\225l)" )) );
	langList.insert("nn",       langPair("Norwegian (Nnyorsk)", QObject::tr( "Norwegian (Nnyorsk)" )) );
	langList.insert("nn_NO",    langPair("Norwegian (Nnyorsk)", QObject::tr( "Norwegian (Nnyorsk)" )) );
	langList.insert("no",       langPair("Norwegian",           QObject::tr( "Norwegian" )) );
	langList.insert("no_NO",    langPair("Norwegian",           QObject::tr( "Norwegian" )) );
	langList.insert("pl",       langPair("Polish",              QObject::tr( "Polish" )) );
	langList.insert("pl_PL",    langPair("Polish",              QObject::tr( "Polish" )) );
	langList.insert("pt",       langPair("Portuguese",          QObject::tr( "Portuguese" )) );
	langList.insert("pt_BR",    langPair("Portuguese (BR)",     QObject::tr( "Portuguese (BR)" )) );
	langList.insert("ro",       langPair("Romanian",            QObject::tr( "Romanian" )) );
	langList.insert("ru",       langPair("Russian",             QObject::tr( "Russian" )) );
	langList.insert("es",       langPair("Spanish",             QObject::tr( "Spanish" )) );
	langList.insert("es_ES",    langPair("Spanish",             QObject::tr( "Spanish" )) );
	langList.insert("es_LA",    langPair("Spanish (Latin)",     QObject::tr( "Spanish (Latin)" )) );
	langList.insert("sk",       langPair("Slovak",              QObject::tr( "Slovak" )) );
	langList.insert("sl",       langPair("Slovenian",           QObject::tr( "Slovenian" )) );
	langList.insert("sr",       langPair("Serbian",             QObject::tr( "Serbian" )) );
	langList.insert("sv",       langPair("Swedish",             QObject::tr( "Swedish" )) );
	langList.insert("th",       langPair("Thai",                QObject::tr( "Thai" )) );
	langList.insert("th_TH",    langPair("Thai",                QObject::tr( "Thai" )) );
	langList.insert("tr",       langPair("Turkish",             QObject::tr( "Turkish" )) );
	langList.insert("tr_TR",    langPair("Turkish",             QObject::tr( "Turkish" )) );
	langList.insert("uk",       langPair("Ukranian",            QObject::tr( "Ukranian" )) );
	langList.insert("vi",       langPair("Vietnamese",          QObject::tr( "Vietnamese" )) );
	langList.insert("cy",       langPair("Welsh",               QObject::tr( "Welsh" )) );
}

void LanguageManager::generateInstalledLangList()
{
	QString path = ScPaths::instance().shareDir() + "translations/";
	QString langAbbrev;
	QMap<QString, langPair>::Iterator it;
	QDir dir(path , "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if (dir.exists() && (dir.count() != 0))
	{
		for (uint i = 0; i < dir.count(); ++i) 
		{
			QFileInfo file(path + dir[i]);
			if (file.suffix().toLower() == "qm")
			{
				langAbbrev = file.completeSuffix().remove(".qm");
				if ((it=langList.find(langAbbrev))!=langList.end())
						installedLangList.insert(it.key(), it.value().second);
			}
		}
	}
}

const QString LanguageManager::getLangFromAbbrev(QString langAbbrev, bool getTranslated)
{
	QMap<QString, langPair>::Iterator it;
	if ((it=langList.find(langAbbrev))!=langList.end())
	{
		if (getTranslated)
			return it.value().second;
		else
			return it.value().first;
	}
	else
		return "";	
}

const QString LanguageManager::getAbbrevFromLang(QString lang, bool getFromTranslated, bool useInstalled)
{
	QMap<QString, langPair>::Iterator it;
	if (useInstalled)
	{
		for (it=langList.begin();it!=langList.end();++it)
		{
			if (installedLangList.find(it.key()) != installedLangList.end())
			{
				if (getFromTranslated && it.value().second==lang)
					return it.key();
				if (!getFromTranslated && it.value().first==lang)
					return it.key();
			}
		}
	}
	else
	{
		for (it=langList.begin();it!=langList.end();++it)
		{
// 			if (installedLangList.find(it.key()) != installedLangList.end())
// 			{
			if (getFromTranslated && it.value().second==lang)
				return it.key();
			if (!getFromTranslated && it.value().first==lang)
				return it.key();
// 			}
		}
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
			stringListToFill->append(it.value());
	}
}

void LanguageManager::printInstalledList()
{
	QMap<QString, QString>::Iterator it;

	for (it=installedLangList.begin();it!=installedLangList.end();++it)
		std::cout << it.key().leftJustified(6).toStdString() << ": " << it.value().toStdString() << std::endl;
}

QString LanguageManager::numericSequence(QString seq)
{
	QString retSeq;
	const int nsBengali=0,nsDevanagari=1,nsGujarati=2,nsGurumukhi=3,nsKannada=4,nsMalayalam=5,nsOriya=6,nsTamil=7,nsTelugu=8,nsTibetan=9,nsLepcha=10;
	switch (1)
	{
		case nsBengali:
			retSeq+="";
			break;
		case nsDevanagari:
			retSeq+="०१२३४५६७८९";
			break;
		case nsGujarati:
			retSeq+="૦૧૨૩૪૫૬૭૮૯";
			break;
		case nsGurumukhi:
			retSeq+="੦੧੨੩੪੫੬੭੮੯";
			break;
		case nsKannada:
			retSeq+="";
			break;
		case nsMalayalam:
			retSeq+="";
			break;
		case nsOriya:
			retSeq+="";
			break;
		case nsTamil:
			retSeq+="";
			break;
		case nsTelugu:
			retSeq+="";
			break;
		case nsTibetan:
			retSeq+="";
			break;
		case nsLepcha:
			retSeq+="";
			break;
		default:
			retSeq="0123456789";
			break;
	}
	return retSeq;
}

LanguageManager::~LanguageManager()
{
	langList.clear();
	installedLangList.clear();
}

