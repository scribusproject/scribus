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
 
#ifndef LOCALEMANAGER_H
#define LOCALEMANAGER_H

#include <QLocale>
#include <QMap>
#include <QString>

#include "scribusapi.h"

class SCRIBUS_API LocaleDef
{
	friend class LocaleManager;

	public:
		LocaleDef();
		LocaleDef(const QString& locale, const QString& unit, const QString& pageSize)
		{

				m_locale=locale;
				m_unit=unit;
				m_pageSize=pageSize;

		}
		~LocaleDef() {};

	private:
		QString m_locale;
		QString m_unit;
		QString m_pageSize;
};

class SCRIBUS_API LocaleManager
{
	LocaleManager() {};
	~LocaleManager();

	void init();
	
public:
	static LocaleManager* instance();
	static void deleteInstance();

	const QString getUnitFromLocale(QString);
	void printSelectedForLocale(const QString &locale);
	QString pageSizeForLocale(const QString &locale);
	QString unitForLocale(const QString &locale);

private:
	static LocaleManager* m_instance;

	QList <LocaleDef> localeTable;
	QLocale sysLocale;
	void generateLocaleList();
};

#endif

