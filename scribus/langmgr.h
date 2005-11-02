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

#include <qmap.h>
#include <utility>

#include <qstring.h>
#include "scribusapi.h"

typedef std::pair<QString, QString> langPair;

class SCRIBUS_API LanguageManager
{
public:
	LanguageManager() {};
	~LanguageManager() {};
	void init(bool generateInstalledList = true);
	const QString getLangFromAbbrev(QString, bool getTranslated=true);
	const QString getAbbrevFromLang(QString, bool getTranslated=true);
	void fillInstalledStringList(QStringList *stringListToFill, bool addDefaults);
	void printInstalledList();

private:
	QMap<QString, langPair > langList;
	//QMap<QString, QString> untranslatedLangList;
	QMap<QString, QString> installedLangList;

	void generateLangList();
	void generateInstalledLangList();
	
};

#endif

