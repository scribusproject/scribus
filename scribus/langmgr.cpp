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
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QStringList> 

#include "scconfig.h"
#include "langmgr.h"
#include "scpaths.h"

LanguageManager * LanguageManager::m_instance = 0;
LanguageManager * LanguageManager::instance()
{
	if(!m_instance)
	{
		m_instance = new LanguageManager;
		Q_ASSERT(m_instance);
		m_instance->init();
	}
	return m_instance;
}

void LanguageManager::init(bool generateInstalledList)
{
	//generateUntranslatedLangList();
	generateLangList();
	if (generateInstalledList)
		generateInstalledLangList();
}

void LanguageManager::generateLangList()
{
	// TODO get rid of the redundant key, the english name.
	// So internally language would always be manipulated as a code and otherwise presented translated.
	langList.insert("af",       langPair("Afrikaans",           QObject::tr( "Afrikaans" )) );
	langList.insert("af_ZA",    langPair("Afrikaans",           QObject::tr( "Afrikaans" )) );
	langList.insert("an_ES",    langPair("Aragonese",           QObject::tr( "Aragonese" )) );
	langList.insert("ar",       langPair("Arabic",              QObject::tr( "Arabic" )) );
	langList.insert("be_BY",    langPair("Belarusian",          QObject::tr( "Belarusian" )) );
	langList.insert("bg",       langPair("Bulgarian",           QObject::tr( "Bulgarian" )) );
	langList.insert("bg_BG",    langPair("Bulgarian",           QObject::tr( "Bulgarian" )) );
	langList.insert("bn",       langPair("Bengali",             QObject::tr( "Bengali" )) );
	langList.insert("br",       langPair("Breton",              QObject::tr( "Breton" )) );
	langList.insert("ca",       langPair("Catalan",             QObject::tr( "Catalan" )) );
	langList.insert("cs",       langPair("Czech",               QObject::tr( "Czech" )) );
	langList.insert("cs_CZ",    langPair("Czech",               QObject::tr( "Czech" )) );
	langList.insert("cy",       langPair("Welsh",               QObject::tr( "Welsh" )) );
	langList.insert("da",       langPair("Danish",              QObject::tr( "Danish" )) );
	langList.insert("da_DK",    langPair("Danish",              QObject::tr( "Danish" )) );
	langList.insert("de",       langPair("German",              QObject::tr( "German" )) );
	langList.insert("de_1901",  langPair("German (Trad.)",      QObject::tr( "German (Trad.)" )) );
	langList.insert("de_AT",    langPair("German (Austria)",    QObject::tr( "German (Austria)" )) );
	langList.insert("de_CH",    langPair("German (Swiss)",      QObject::tr( "German (Swiss)" )) );
	langList.insert("de_DE",    langPair("German",              QObject::tr( "German" )) );
	langList.insert("dz",       langPair("Dzongkha",            QObject::tr( "Dzongkha" )) );
	langList.insert("el",       langPair("Greek",               QObject::tr( "Greek" )) );
	langList.insert("en",       langPair("English",             QObject::tr( "English" )) );
	langList.insert("en_AU",    langPair("English (Australia)", QObject::tr( "English (Australian)" )) );
	langList.insert("en_CA",    langPair("English (Canada)",    QObject::tr( "English (Canadian)" )) );
	langList.insert("en_GB",    langPair("English (UK)",        QObject::tr( "English (UK)" )) );
	langList.insert("en_NZ",    langPair("English (New Zealand)",QObject::tr( "English (New Zealand)" )) );
	langList.insert("en_US",    langPair("English (USA)",       QObject::tr( "English (USA)" )) );
	langList.insert("en_ZA",    langPair("English (South African)",  QObject::tr( "English (South African)" )) );
	langList.insert("eo",       langPair("Esperanto",           QObject::tr( "Esperanto" )) );
	langList.insert("es",       langPair("Spanish",             QObject::tr( "Spanish" )) );
	langList.insert("es_AR",    langPair("Spanish (Argentina)", QObject::tr( "Spanish (Argentina)" )) );
	langList.insert("es_ES",    langPair("Spanish",             QObject::tr( "Spanish" )) );
	langList.insert("es_LA",    langPair("Spanish (Latin)",     QObject::tr( "Spanish (Latin)" )) );
	langList.insert("et",       langPair("Estonian",            QObject::tr( "Estonian" )) );
	langList.insert("et_EE",    langPair("Estonian",            QObject::tr( "Estonian" )) );
	langList.insert("eu",       langPair("Basque",              QObject::tr( "Basque" )) );
	langList.insert("fi",       langPair("Finnish",             QObject::tr( "Finnish" )) );
	langList.insert("fr",       langPair("French",              QObject::tr( "French" )) );
	langList.insert("fr_FR",    langPair("French",              QObject::tr( "French" )) );
	langList.insert("gl",       langPair("Galician",            QObject::tr( "Galician" )) );
	langList.insert("he",       langPair("Hebrew",              QObject::tr( "Hebrew" )) );
	langList.insert("hr",       langPair("Croatian",            QObject::tr( "Croatian" )) );
	langList.insert("hu",       langPair("Hungarian",           QObject::tr( "Hungarian" )) );
	langList.insert("hu_HU",    langPair("Hungarian",           QObject::tr( "Hungarian" )) );
	langList.insert("ia",       langPair("Latin",               QObject::tr( "Latin" )) );
	langList.insert("id",       langPair("Indonesian",          QObject::tr( "Indonesian" )) );
	langList.insert("is",       langPair("Icelandic",           QObject::tr( "Icelandic" )) );
	langList.insert("it",       langPair("Italian",             QObject::tr( "Italian" )) );
	langList.insert("it_IT",    langPair("Italian",             QObject::tr( "Italian" )) );
	langList.insert("ja",       langPair("Japanese",            QObject::tr( "Japanese" )) );
	langList.insert("km",       langPair("Khmer",               QObject::tr( "Khmer" )) );
	langList.insert("ko",       langPair("Korean",              QObject::tr( "Korean" )) );
	langList.insert("ku",       langPair("Kurdish",             QObject::tr( "Kurdish" )) );
	langList.insert("la",       langPair("Latin",               QObject::tr( "Latin" )) );
	langList.insert("lb",       langPair("Luxembourgish",       QObject::tr( "Luxembourgish" )) );
	langList.insert("lo",       langPair("Lao",                 QObject::tr( "Lao" )) );
	langList.insert("lt",       langPair("Lithuanian",          QObject::tr( "Lithuanian" )) );
	langList.insert("lt_LT",    langPair("Lithuanian",          QObject::tr( "Lithuanian" )) );
	langList.insert("ne_NP",    langPair("Nepali",              QObject::tr( "Nepali" )) );
	langList.insert("nb",       langPair("Norwegian (Bokmål)",  QObject::trUtf8( "Norwegian (Bokm\303\245l)" )) );
	langList.insert("nb_NO",    langPair("Norwegian (Bokmål)",  QObject::trUtf8( "Norwegian (Bokm\303\245l)" )) );
	langList.insert("nl",       langPair("Dutch",               QObject::tr( "Dutch" )) );
	langList.insert("nl_NL",    langPair("Dutch",               QObject::tr( "Dutch" )) );
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
	langList.insert("ru_RU",    langPair("Russian",             QObject::tr( "Russian" )) );
	langList.insert("sa",       langPair("Sanskrit",            QObject::tr( "Sanskrit" )) );
	langList.insert("sk",       langPair("Slovak",              QObject::tr( "Slovak" )) );
	langList.insert("sk_SK",    langPair("Slovak",              QObject::tr( "Slovak" )) );
	langList.insert("sl",       langPair("Slovenian",           QObject::tr( "Slovenian" )) );
	langList.insert("sl_SL",    langPair("Slovenian",           QObject::tr( "Slovenian" )) );
	langList.insert("sq",       langPair("Albanian",            QObject::tr( "Albanian" )) );
	langList.insert("sr",       langPair("Serbian",             QObject::tr( "Serbian" )) );
	langList.insert("sv",       langPair("Swedish",             QObject::tr( "Swedish" )) );
	langList.insert("th",       langPair("Thai",                QObject::tr( "Thai" )) );
	langList.insert("th_TH",    langPair("Thai",                QObject::tr( "Thai" )) );
	langList.insert("tr",       langPair("Turkish",             QObject::tr( "Turkish" )) );
	langList.insert("tr_TR",    langPair("Turkish",             QObject::tr( "Turkish" )) );
	langList.insert("uk",       langPair("Ukranian",            QObject::tr( "Ukranian" )) );
	langList.insert("uk_UA",    langPair("Ukranian",            QObject::tr( "Ukranian" )) );
	langList.insert("vi",       langPair("Vietnamese",          QObject::tr( "Vietnamese" )) );
	langList.insert("zh",       langPair("Chinese",             QObject::tr( "Chinese" )) );
	langList.insert("zh_TW",    langPair("Chinese (Trad.)",     QObject::tr( "Chinese (Trad.)" )) );

}

void LanguageManager::generateInstalledLangList()
{
	QString path = ScPaths::instance().translationDir();
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

const QString LanguageManager::getAbbrevFromLang(QString lang, bool getFromTranslated, bool useInstalled, int abbrevNo)
{
	QMap<QString, langPair>::Iterator it;
	if (lang == "English" || lang == QObject::tr( "English"))
		useInstalled = false;
	int count=1;
	if (useInstalled)
	{
		for (it=langList.begin();it!=langList.end();++it)
		{
			if (installedLangList.find(it.key()) != installedLangList.end())
			{
				if (getFromTranslated && it.value().second==lang)
				{
					if (abbrevNo==1)
						return it.key();
					if (abbrevNo!=1 && count==abbrevNo)
						return it.key();
					++count;
					continue;			
				}
				if (!getFromTranslated && it.value().first==lang)
				{
					if (abbrevNo==1)
						return it.key();
					if (abbrevNo!=1 && count==abbrevNo)
						return it.key();
					++count;
					continue;		
				}
			}
		}
	}
	else
	{
		for (it=langList.begin();it!=langList.end();++it)
		{
			if (getFromTranslated && it.value().second==lang)
			{
				if (abbrevNo==1)
					return it.key();
				if (abbrevNo!=1 && count==abbrevNo)
					return it.key();
				++count;
				continue;
					
			}
			if (!getFromTranslated && it.value().first==lang)
			{
				if (abbrevNo==1)
					return it.key();
				if (abbrevNo!=1 && count==abbrevNo)
					return it.key();
				++count;
				continue;
			}
		}
	}
	return "";
}

const QString LanguageManager::getLangFromTransLang(QString lang)
{
	QMap<QString, langPair>::Iterator it;
	for (it=langList.begin();it!=langList.end();++it)
	{
		if (it.value().second==lang)
			return it.value().first;
	}
	return "";
}

const QString LanguageManager::getTransLangFromLang(QString lang)
{
	QMap<QString, langPair>::Iterator it;
	// Seems something is missing here!
	QString enLang(QObject::tr( "English"));
	if ((lang == "English") || (lang == enLang))
		return enLang;
	
	for (it=langList.begin();it!=langList.end();++it)
	{
		if (it.value().first==lang)
			return it.value().second;
	}
	return "";
}

const QString LanguageManager::getShortAbbrevFromAbbrev(QString langAbbrev)
{
	if (langList.contains(langAbbrev))
		return langAbbrev;

	QString joiner("_");
	QStringList langParts = langAbbrev.split(QChar('_'));
	if (langParts.count() > 2)
		langParts.removeLast();

	while (langParts.count() > 2)
	{
		QString abbrev = langParts.join(joiner);
		if (langList.contains(abbrev))
			return abbrev;
		langParts.removeLast();
	}

	if (langParts.count() == 2)
	{
		QString abbrev = langParts.join(joiner);
		if (langList.contains(abbrev))
			return abbrev;
		if (langParts.at(0) == langParts.at(1).toLower())
		{
			if (langList.contains(langParts.at(0)))
				return langParts.at(0);
		}
		langParts.removeLast();
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

bool LanguageManager::findDictionaries(QStringList &sl)
{
	sl=ScPaths::instance().spellDirs();
	if (sl.count()==0)
		return false;
	return true;
}

void LanguageManager::findDictionarySets(QStringList &dictionaryPaths, QMap<QString, QString> &dictionaryMap)
{
	for (int i=0; i<dictionaryPaths.count(); ++i)
	{
		// Find the dic and aff files in the location
		QDir dictLocation(dictionaryPaths.at(i));
		QStringList dictFilters("*.dic");
		QStringList dictList(dictLocation.entryList(dictFilters, QDir::Files, QDir::Name));
		dictList.replaceInStrings(".dic","");

		//Ensure we have aff+dic file pairs, remove any hyphenation dictionaries from the list
		QString dictName;
		foreach(dictName, dictList)
		{
			if (!QFile::exists(dictionaryPaths.at(i)+dictName+".aff"))
			{
				dictList.removeAll(dictName);
			}
			else
			{
				if (!dictionaryMap.contains(dictName))
				{
					if (dictName.length() <= 5 && langList.contains(dictName))
					{
						dictionaryMap.insert(dictName, dictionaryPaths.at(i)+dictName);
						continue;
					}
					QString shortAbbrev(LanguageManager::getShortAbbrevFromAbbrev(dictName));
					if (shortAbbrev.isEmpty())
						shortAbbrev = dictName;
					dictionaryMap.insert(shortAbbrev, dictionaryPaths.at(i)+dictName);
				}
			}
		}
//		qDebug()<<"Number of dictionaries/AFFs found in"<<dictionaryPaths.at(i)<<":"<<dictList.count();
	}
}

LanguageManager::~LanguageManager()
{
	langList.clear();
	installedLangList.clear();
	hyphLangList.clear();
}

void LanguageManager::addHyphLang(const QString & lang, const QString & filename)
{
	hyphLangList[lang] = filename;
}

const QString LanguageManager::getHyphFilename(const QString & lang, bool langIsAbbreviated)
{
	if(langIsAbbreviated)
		return hyphLangList.value(lang);
	return hyphLangList.value(getAbbrevFromLang(lang, false, false));
}

const QStringList LanguageManager::hyphLangs()
{
	return hyphLangList.keys();
}





