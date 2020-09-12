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
 
#ifndef LANGMANAGER_H
#define LANGMANAGER_H

#include <QMap>
#include <utility>

#include <QString>
#include <QStringList>
#include "langdef.h"
#include "scribusapi.h"

typedef std::pair<QString, QString> langPair;

class SCRIBUS_API LanguageManager
{
	LanguageManager() = default;
	~LanguageManager();

	void init(bool generateInstalledList = true);
	
public:
	static LanguageManager* instance();
	static void deleteInstance();

	void languageChange();
	QStringList languageList(bool getTranslated=true);
	
	QString getLangFromAbbrev(QString, bool getTranslated=true);
	QString getAbbrevFromLang(const QString&, bool useInstalled=true);
	QString getLangFromTransLang(const QString& transLang);
	QString getTransLangFromLang(const QString& lang);
	QString getShortAbbrevFromAbbrev(QString langAbbrev);
	QString getShortAbbrevFromAbbrevDecomposition(const QString& langAbbrev);
	QString getAlternativeAbbrevfromAbbrev(const QString& langAbbrev);
	QStringList   getAbbrevDecomposition(const QString& langAbbrev);
	void fillInstalledStringList(QStringList *stringListToFill);
	void fillInstalledGUIStringList(QStringList *stringListToFill);
	void fillInstalledHyphStringList(QStringList *stringListToFill);
	void fillInstalledSpellStringList(QStringList *stringListToFill);
	void printInstalledList();
	QString numericSequence(const QString& seq);
	bool findSpellingDictionaries(QStringList& sl);
	void findSpellingDictionarySets(QStringList& dictionaryPaths, QMap<QString, QString>& dictionaryMap);
	bool findHyphDictionaries(QStringList& sl);
	void findHyphDictionarySets(QStringList& dictionaryPaths, QMap<QString, QString>& dictionaryMap);
	
	QString getHyphFilename(const QString& langAbbrev);
	int langTableIndex(const QString& abbrev);

	bool isAvailableTranslation(QString langAbbrev);

private:
	static LanguageManager* m_instance;

	QList <LangDef> m_langTable;

	void generateLangList();
	void generateInstalledGUILangList();
	void generateInstalledHyphLangList();
	void generateInstalledSpellLangList();
};

#endif

