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

void LanguageManager::deleteInstance()
{
	if (m_instance)
		delete m_instance;
	m_instance = 0;
}

void LanguageManager::languageChange()
{
	QList<LangDef> oldLangList = langTable;
	generateLangList();

	for (int i = 0; i < langTable.count(); ++i)
	{
		langTable[i].m_hyphAvailable = oldLangList[i].m_hyphAvailable;
		langTable[i].m_hyphFile = oldLangList[i].m_hyphFile;

		langTable[i].m_spellAvailable = oldLangList[i].m_spellAvailable;
		langTable[i].m_spellFile = oldLangList[i].m_spellFile;

		langTable[i].m_transAvailable = oldLangList[i].m_transAvailable;
		langTable[i].m_transFile = oldLangList[i].m_transFile;
	}
}

void LanguageManager::init(bool generateInstalledList)
{
	//generateUntranslatedLangList();
	generateLangList();
	if (generateInstalledList)
	{
		generateInstalledGUILangList();
		generateInstalledSpellLangList();
		generateInstalledHyphLangList();
	}
}

void LanguageManager::generateLangList()
{
	// So internally language would always be manipulated as a code and otherwise presented translated.
	//Build table;
	langTable.clear();
	langTable.append(LangDef("af",     "af_ZA", "Afrikaans",              QObject::tr( "Afrikaans" )));
	langTable.append(LangDef("an_ES",  "",      "Aragonese",              QObject::tr( "Aragonese" )) );
	langTable.append(LangDef("ar",     "",      "Arabic",                 QObject::tr( "Arabic" )) );
	langTable.append(LangDef("be_BY",  "",      "Belarusian",             QObject::tr( "Belarusian" )) );
	langTable.append(LangDef("bg",     "bg_BG", "Bulgarian",              QObject::tr( "Bulgarian" )) );
	langTable.append(LangDef("blo",    "",      "Anii",                   QObject::tr( "Anii" )) );
	langTable.append(LangDef("bn",     "",      "Bengali",                QObject::tr( "Bengali" )) );
	langTable.append(LangDef("bn_BD",  "",      "Bengali (Bangladesh)",   QObject::tr( "Bengali (Bangladesh)" )) );
	langTable.append(LangDef("br",     "br_FR", "Breton",                 QObject::tr( "Breton" )) );
	langTable.append(LangDef("ca",     "",      "Catalan",                QObject::tr( "Catalan" )) );
	langTable.append(LangDef("cs",     "cs_CZ", "Czech",                  QObject::tr( "Czech" )) );
	langTable.append(LangDef("cy",     "",      "Welsh",                  QObject::tr( "Welsh" )) );
	langTable.append(LangDef("da",     "da_DK", "Danish",                 QObject::tr( "Danish" )) );
	langTable.append(LangDef("de",     "de_DE", "German",                 QObject::tr( "German" )) );
	langTable.append(LangDef("de_1901","",      "German (Trad.)",         QObject::tr( "German (Trad.)" )) );
	langTable.append(LangDef("de_AT",  "",      "German (Austria)",       QObject::tr( "German (Austria)" )) );
	langTable.append(LangDef("de_CH",  "",      "German (Swiss)",         QObject::tr( "German (Swiss)" )) );
	langTable.append(LangDef("dz",     "",      "Dzongkha",               QObject::tr( "Dzongkha" )) );
	langTable.append(LangDef("el",     "",      "Greek",                  QObject::tr( "Greek" )) );
	langTable.append(LangDef("en_GB",  "en",    "English (UK)",           QObject::tr( "English (UK)" )) );
	langTable.append(LangDef("en_AU",  "",      "English (Australia)",    QObject::tr( "English (Australia)" )) );
	langTable.append(LangDef("en_CA",  "",      "English (Canada)",       QObject::tr( "English (Canada)" )) );
	langTable.append(LangDef("en_NZ",  "",      "English (New Zealand)",  QObject::tr( "English (New Zealand)" )) );
	langTable.append(LangDef("en_US",  "",      "English (US)",           QObject::tr( "English (US)" )) );
	langTable.append(LangDef("en_ZA",  "",      "English (South Africa)", QObject::tr( "English (South Africa)" )) );
	langTable.append(LangDef("eo",     "",      "Esperanto",              QObject::tr( "Esperanto" )) );
	langTable.append(LangDef("es",     "es_ES", "Spanish",                QObject::tr( "Spanish" )) );
	langTable.append(LangDef("es_AR",  "",      "Spanish (Argentina)",    QObject::tr( "Spanish (Argentina)" )) );
	langTable.append(LangDef("es_LA",  "",      "Spanish (Latin)",        QObject::tr( "Spanish (Latin)" )) );
	langTable.append(LangDef("et",     "et_EE", "Estonian",               QObject::tr( "Estonian" )) );
	langTable.append(LangDef("eu",     "",      "Basque",                 QObject::tr( "Basque" )) );
	langTable.append(LangDef("fi",     "",      "Finnish",                QObject::tr( "Finnish" )) );
	langTable.append(LangDef("fr",     "fr_FR", "French",                 QObject::tr( "French" )) );
	langTable.append(LangDef("gd",     "gd_GB", "Scottish Gaelic",        QObject::tr( "Scottish Gaelic" )) );
	langTable.append(LangDef("gl",     "gl_ES", "Galician",               QObject::tr( "Galician" )) );
	langTable.append(LangDef("gu",     "gu_IN", "Gujarati",               QObject::tr( "Gujarati" )) );
	langTable.append(LangDef("he",     "",      "Hebrew",                 QObject::tr( "Hebrew" )) );
	langTable.append(LangDef("hr",     "",      "Croatian",               QObject::tr( "Croatian" )) );
	langTable.append(LangDef("hu",     "hu_HU", "Hungarian",              QObject::tr( "Hungarian" )) );
	langTable.append(LangDef("ia",     "",      "Latin",                  QObject::tr( "Latin" )) );
	langTable.append(LangDef("id",     "",      "Indonesian",             QObject::tr( "Indonesian" )) );
	langTable.append(LangDef("is",     "",      "Icelandic",              QObject::tr( "Icelandic" )) );
	langTable.append(LangDef("it",     "it_IT", "Italian",                QObject::tr( "Italian" )) );
	langTable.append(LangDef("ja",     "",      "Japanese",               QObject::tr( "Japanese" )) );
	langTable.append(LangDef("km",     "",      "Khmer",                  QObject::tr( "Khmer" )) );
	langTable.append(LangDef("ko",     "",      "Korean",                 QObject::tr( "Korean" )) );
	langTable.append(LangDef("ku",     "",      "Kurdish",                QObject::tr( "Kurdish" )) );
	langTable.append(LangDef("la",     "",      "Latin",                  QObject::tr( "Latin" )) );
	langTable.append(LangDef("lb",     "",      "Luxembourgish",          QObject::tr( "Luxembourgish" )) );
	langTable.append(LangDef("lo",     "",      "Lao",                    QObject::tr( "Lao" )) );
	langTable.append(LangDef("lt",     "lt_LT", "Lithuanian",             QObject::tr( "Lithuanian" )) );
	langTable.append(LangDef("ne_NP",  "",      "Nepali",                  QObject::tr( "Nepali" )) );
	langTable.append(LangDef("nb",     "nb_NO", "Norwegian (Bokmål)",     QObject::trUtf8( "Norwegian (Bokm\303\245l)" )) );
	langTable.append(LangDef("nl",     "nl_NL", "Dutch",                  QObject::tr( "Dutch" )) );
	langTable.append(LangDef("nn",     "nn_NO", "Norwegian (Nnyorsk)",    QObject::tr( "Norwegian (Nnyorsk)" )) );
	langTable.append(LangDef("no",     "no_NO", "Norwegian",              QObject::tr( "Norwegian" )) );
	langTable.append(LangDef("pl",     "pl_PL", "Polish",                 QObject::tr( "Polish" )) );
	langTable.append(LangDef("pt",     "pt_PT", "Portuguese",             QObject::tr( "Portuguese" )) );
	langTable.append(LangDef("pt_BR",  "",      "Portuguese (BR)",        QObject::tr( "Portuguese (BR)" )) );
	langTable.append(LangDef("ro",     "",      "Romanian",               QObject::tr( "Romanian" )) );
	langTable.append(LangDef("ru",     "ru_RU", "Russian",                QObject::tr( "Russian" )) );
	langTable.append(LangDef("sa",     "",      "Sanskrit",               QObject::tr( "Sanskrit" )) );
	langTable.append(LangDef("sk",     "sk_SK", "Slovak",                 QObject::tr( "Slovak" )) );
	langTable.append(LangDef("sl",     "sl_SL", "Slovenian",              QObject::tr( "Slovenian" )) );
	langTable.append(LangDef("sq",     "",      "Albanian",               QObject::tr( "Albanian" )) );
	langTable.append(LangDef("sr",     "",      "Serbian",                QObject::tr( "Serbian" )) );
	langTable.append(LangDef("sv",     "",      "Swedish",                QObject::tr( "Swedish" )) );
	langTable.append(LangDef("th",     "th_TH", "Thai",                   QObject::tr( "Thai" )) );
	langTable.append(LangDef("tr",     "tr_TR", "Turkish",                QObject::tr( "Turkish" )) );
	langTable.append(LangDef("uk",     "uk_UA", "Ukranian",               QObject::tr( "Ukranian" )) );
	langTable.append(LangDef("vi",     "",      "Vietnamese",             QObject::tr( "Vietnamese" )) );
	langTable.append(LangDef("zh",     "",      "Chinese",                QObject::tr( "Chinese" )) );
	langTable.append(LangDef("zh_TW",  "",      "Chinese (Trad.)",        QObject::tr( "Chinese (Trad.)" )) );
}

void LanguageManager::generateInstalledGUILangList()
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
				int j=langTableIndex(langAbbrev);
				if (j!=-1)
				{
					langTable[j].m_transAvailable=true;
					langTable[j].m_transFile=file.absoluteFilePath();
					//qDebug()<<"Found installed GUI translation file:"<<langAbbrev<<" : "<<langTable[j].m_transFile<<langTable[j].m_transName;
				}
			}
		}
	}
}

void LanguageManager::generateInstalledHyphLangList()
{
	//Build our list of hyphenation dictionaries we have in the install dir
	//Grab the language abbreviation from it, get the full language text
	//Insert the name as key and a new string list into the map
	QString hyphDirName = QDir::toNativeSeparators(ScPaths::instance().dictDir());
	QDir hyphDir(hyphDirName, "hyph*.dic", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if (!hyphDir.exists() || hyphDir.count() == 0)
	{
		qDebug()<<"No preinstalled hyphenation dictonaries or paths found";
		return;
	}

//	qDebug()<<"Installed Hyphenation Dictonaries:";
	for (uint i = 0; i < hyphDir.count(); ++i)
	{
		QFileInfo file(hyphDir[i]);
		QString langAbbrev=file.baseName().section('_', 1);
		int j=langTableIndex(langAbbrev);
		if (j!=-1)
		{
			langTable[j].m_hyphAvailable=true;
			langTable[j].m_hyphFile=hyphDirName+hyphDir[i];
			//qDebug()<<"Found installed hyphenation dictionary:"<<langAbbrev<<" : "<<hyphDirName+hyphDir[i];
		}
	}
}

void LanguageManager::generateInstalledSpellLangList()
{
	QStringList dictionaryPaths;
	bool dictPathFound=findSpellingDictionaries(dictionaryPaths);
	if (!dictPathFound)
	{
		qDebug()<<"No preinstalled spelling dictonaries or paths found";
		return;
	}
	QMap<QString, QString> dictionaryMap;
	findSpellingDictionarySets(dictionaryPaths, dictionaryMap);
	if (dictionaryMap.count()==0)
		return;

	QMap<QString, QString>::iterator it = dictionaryMap.begin();
//	qDebug()<<"Installed Spelling Dictonaries:";
	while (it != dictionaryMap.end())
	{
		int j=langTableIndex(it.key());
		if (j!=-1)
		{
			langTable[j].m_spellAvailable=true;
			langTable[j].m_spellFile=it.value();
//			qDebug()<<"Found installed spelling dictionary:"<<it.key()<<" : "<<it.value();
		}
		++it;
	}
}



int LanguageManager::langTableIndex(const QString &abbrev)
{
//	qDebug()<<"langTableIndex: Trying to find:"<<abbrev;
	for (int i = 0; i < langTable.size(); ++i)
	{
//		qDebug()<<abbrev<<langTable[i].m_priAbbrev<<langTable[i].m_altAbbrev;
		if (langTable[i].m_priAbbrev==abbrev || langTable[i].m_altAbbrev==abbrev)
			return i;
	}
	return -1;
}

const QString LanguageManager::getLangFromAbbrev(QString langAbbrev, bool getTranslated)
{
//	qDebug()<<"Trying to find:"<<langAbbrev;
	int i=langTableIndex(langAbbrev);
//	qDebug()<<"Index of"<<langAbbrev<<":"<<i;
	if (i==-1)
	{
		if (langAbbrev.length()>5)
		{
			langAbbrev.truncate(5);
			i=langTableIndex(langAbbrev);
		}
	}
	if (i!=-1)
	{
		if (getTranslated)
			return langTable[i].m_transName;
		else
			return langTable[i].m_name;
	}
	//qDebug()<<langAbbrev<<"not found";
	return "";
}

const QString LanguageManager::getAbbrevFromLang(QString lang, bool getFromTranslated, bool useInstalled)
{
	QMap<QString, langPair>::Iterator it;
	if (lang == "English" || lang == QObject::tr( "English"))
		useInstalled = false;
	for (int i = 0; i < langTable.size(); ++i)
	{
		if (useInstalled)
		{
			if (langTable[i].m_transAvailable && (langTable[i].m_name==lang || langTable[i].m_transName==lang))
				return langTable[i].m_priAbbrev;
		}
		else
		{
			//qDebug()<<lang<<langTable[i].m_priAbbrev<<langTable[i].m_name<<langTable[i].m_transName;
			if (langTable[i].m_name==lang || langTable[i].m_transName==lang)
				return langTable[i].m_priAbbrev;
		}
	}
	return "";
}

const QString LanguageManager::getLangFromTransLang(QString transLang)
{
	for (int i = 0; i < langTable.size(); ++i)
	{
		if (langTable[i].m_transName==transLang)
			return langTable[i].m_name;
	}
	return "";
}

const QString LanguageManager::getTransLangFromLang(QString lang)
{
	// Seems something is missing here!
	QString enLang(QObject::tr( "English"));
	if ((lang == "English") || (lang == enLang))
		return enLang;
	
	for (int i = 0; i < langTable.size(); ++i)
	{
		if (langTable[i].m_name==lang)
			return langTable[i].m_transName;
	}
	return "";
}

const QString LanguageManager::getShortAbbrevFromAbbrev(QString langAbbrev)
{
	//	qDebug()<<"Trying to find:"<<langAbbrev;
	int i = langTableIndex(langAbbrev);
	//	qDebug()<<"Index of"<<langAbbrev<<":"<<i;
	if (i == -1)
	{
		if (langAbbrev.length()>5)
		{
			langAbbrev.truncate(5);
			i=langTableIndex(langAbbrev);
		}
	}
	if (i != -1)
		return langTable[i].m_priAbbrev;
	//qDebug()<<langAbbrev<<"not found";
	return "";
}

const QString LanguageManager::getAlternativeAbbrevfromAbbrev(QString langAbbrev)
{
	int i=langTableIndex(langAbbrev);
	if (i!=-1)
		return langTable[i].m_altAbbrev;
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

		for (int i = 0; i < langTable.size(); ++i)
			stringListToFill->append(langTable[i].m_transName);
	}
}

void LanguageManager::fillInstalledGUIStringList(QStringList *stringListToFill, bool addDefaults)
{
	if (stringListToFill)
	{
		if (addDefaults)
		{
			stringListToFill->append("");
			stringListToFill->append( QObject::tr( "English" ));
		}

		for (int i = 0; i < langTable.size(); ++i)
		{
			//qDebug()<<langTable[i].m_transName<<langTable[i].m_transAvailable;
			if (langTable[i].m_transAvailable)
				stringListToFill->append(langTable[i].m_transName);
		}
	}
	stringListToFill->sort();
}

void LanguageManager::fillInstalledHyphStringList(QStringList *stringListToFill)
{
	if (stringListToFill)
	{
		for (int i = 0; i < langTable.size(); ++i)
		{
			//qDebug()<<langTable[i].m_transName<<langTable[i].m_hyphAvailable;
			if (langTable[i].m_hyphAvailable)
				stringListToFill->append(langTable[i].m_transName);
		}
	}
	stringListToFill->sort();
}

QStringList LanguageManager::languageList(bool getTranslated)
{
	QStringList sl;
	for (int i = 0; i < langTable.size(); ++i)
	{
		if (getTranslated)
			sl<<langTable[i].m_transName;
		else
			sl<<langTable[i].m_name;
	}
	sl.sort();
	return sl;
}

void LanguageManager::printInstalledList()
{
	for (int i = 0; i < langTable.size(); ++i)
		qDebug() << langTable[i].m_priAbbrev.leftJustified(6) << ": " << langTable[i].m_name;
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

bool LanguageManager::findSpellingDictionaries(QStringList &sl)
{
	sl=ScPaths::instance().spellDirs();
	if (sl.count()==0)
		return false;
	return true;
}

void LanguageManager::findSpellingDictionarySets(QStringList &dictionaryPaths, QMap<QString, QString> &dictionaryMap)
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
				dictList.removeAll(dictName);
			else
			{
				if (!dictionaryMap.contains(dictName))
				{
					if (dictName.length()<=5)
					{
						//QString shortAbbrev(LanguageManager::getShortAbbrevFromAbbrev(dictName));
						//qDebug()<<"findSpellingDictionarySets"<<dictName<<shortAbbrev;
						dictionaryMap.insert(dictName, dictionaryPaths.at(i)+dictName);
						//dictionaryMap.insert(shortAbbrev, dictionaryPaths.at(i)+dictName);
					}
					//qDebug()<<"Spell Finder:"<<dictName<<dictionaryPaths.at(i)+dictName;
					if (dictName.length()>5)
					{
						QString shortAbbrev(LanguageManager::getShortAbbrevFromAbbrev(dictName));
						//qDebug()<<shortAbbrev;
						dictionaryMap.insert(shortAbbrev, dictionaryPaths.at(i)+dictName);
					}
				}
			}
		}
//		qDebug()<<"Number of dictionaries/AFFs found in"<<dictionaryPaths.at(i)<<":"<<dictList.count();
	}
	//Now rescan dictionary map for any extra languages we can support with the files we have
	QMap<QString, QString>::iterator it = dictionaryMap.begin();
	while (it != dictionaryMap.end())
	{
		QString lang(it.key());
		if (lang.length()==5)
		{
			QString shortAbbrev(LanguageManager::getShortAbbrevFromAbbrev(lang));
			if (!dictionaryMap.contains(shortAbbrev))
			{
				//qDebug()<<"Adding extra spelling definitions for:"<<lang<<":"<<shortAbbrev;
				dictionaryMap.insert(shortAbbrev, it.value());
			}
			//else
				//qDebug()<<"Short abbreviation:"<<shortAbbrev<<"already exists for:"<<lang;
		}
		if (lang.length()==2)
		{
			QString altAbbrev(LanguageManager::getAlternativeAbbrevfromAbbrev(lang));
			if (!dictionaryMap.contains(altAbbrev))
			{
				//qDebug()<<"Adding extra spelling definitions for:"<<lang<<":"<<altAbbrev;
				dictionaryMap.insert(altAbbrev, it.value());
			}
			//else
				//qDebug()<<"Alt. abbreviation:"<<altAbbrev<<"already exists for:"<<lang;
		}
		++it;
	}
}

LanguageManager::~LanguageManager()
{
	langTable.clear();
}

const QString LanguageManager::getHyphFilename(const QString & langAbbrev)
{
	int j=langTableIndex(langAbbrev);
	if (j!=-1 && langTable[j].m_hyphAvailable)
	{
//		qDebug()<<"Found requested hyphenation dictionary:"<<langAbbrev<<" : "<<langTable[j].m_hyphFile;
		return langTable[j].m_hyphFile;
	}
	return QString();
}

