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

using langPair = std::pair<QString, QString>;

class SCRIBUS_API LanguageManager
{
	LanguageManager() = default;
	~LanguageManager();

	void init(bool generateInstalledList = true);
	
public:
	static LanguageManager* instance();
	static void deleteInstance();

	void languageChange();
	QStringList languageList(bool getTranslated = true) const;
	
	QString getLangFromAbbrev(QString, bool getTranslated = true) const;
	QString getAbbrevFromLang(const QString&, bool useInstalled = true) const;
	QString getLangFromTransLang(const QString& transLang) const;
	QString getTransLangFromLang(const QString& lang) const;
	QString getShortAbbrevFromAbbrev(QString langAbbrev) const;
	QString getShortAbbrevFromAbbrevDecomposition(const QString& langAbbrev) const;
	QString getAlternativeAbbrevfromAbbrev(const QString& langAbbrev) const;
	QStringList   getAbbrevDecomposition(const QString& langAbbrev) const;
	void fillInstalledStringList(QStringList *stringListToFill) const;
	void fillInstalledGUIStringList(QStringList *stringListToFill) const;
	void fillInstalledHyphStringList(QStringList *stringListToFill) const;
	void fillInstalledSpellStringList(QStringList *stringListToFill) const;
	void printInstalledList() const;
	QString numericSequence(const QString& seq) const;
	bool findSpellingDictionaries(QStringList& sl) const;
	void findSpellingDictionarySets(const QStringList& dictionaryPaths, QMap<QString, QString>& dictionaryMap) const;
	bool findHyphDictionaries(QStringList& sl) const;
	void findHyphDictionarySets(const QStringList& dictionaryPaths, QMap<QString, QString>& dictionaryMap) const;
	
	QString getHyphFilename(const QString& langAbbrev) const;
	int langTableIndex(const QString& abbrev) const;

	bool isAvailableTranslation(QString langAbbrev) const;

private:
	static LanguageManager* m_instance;

	QList <LangDef> m_langTable;

	void generateLangList();
	void generateInstalledGUILangList();
	void generateInstalledHyphLangList();
	void generateInstalledSpellLangList();
};

#endif

