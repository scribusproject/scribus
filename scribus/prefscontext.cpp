/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "prefscontext.h"
#include "scclocale.h"

PrefsContext::PrefsContext()
{
	name = "";
	ispersistent = false;
	isplugin = false;
}

PrefsContext::PrefsContext(QString contextName, bool persistent, bool plugin)
{
	name = contextName;
	ispersistent = persistent;
	isplugin = plugin;
}

QString PrefsContext::getName()
{
	return name;
}

bool PrefsContext::isPersistent()
{
	return ispersistent;
}

bool PrefsContext::isPlugin()
{
	return isplugin;
}

bool PrefsContext::isEmpty()
{
	return (values.size() == 0 && tables.size() == 0);
}

bool PrefsContext::contains(const QString& key)
{
	return values.contains(key);
}

bool PrefsContext::containsTable(const QString& key)
{
	return tables.contains(key);
}

QString PrefsContext::get(const QString& key, const QString& defValue)
{
	if (!values.contains(key))
		values[key] = defValue;

	return values[key];
}

void PrefsContext::set(const QString& key, const char* value)
{
	set(key, QString(value));
}

void PrefsContext::set(const QString& key, const std::string& value)
{
	set(key, QString(value.c_str()));
}

void PrefsContext::set(const QString& key, const QString& value)
{
	values[key] = value;
}

int PrefsContext::getInt(const QString& key, int defValue)
{
	if (!values.contains(key))
		set(key, defValue);
	QString stringValue = values[key];
	bool ok = false;
	int ivalue = stringValue.toInt(&ok);
	if (!ok)
		ivalue = defValue;
	return ivalue;
}

uint PrefsContext::getUInt(const QString& key, uint defValue)
{
	if (!values.contains(key))
		set(key, QString("%1").arg(defValue));
	QString stringValue = values[key];
	bool ok = false;
	int uivalue = stringValue.toUInt(&ok);
	if (!ok)
		uivalue = defValue;
	return uivalue;
}

void PrefsContext::set(const QString& key, int value)
{
	set(key, QString("%1").arg(value));
}

void PrefsContext::set(const QString& key, uint value)
{
	set(key, QString("%1").arg(value));
}

double PrefsContext::getDouble(const QString& key, double defValue)
{
	if (!values.contains(key))
		set(key, defValue);
	QString stringValue = values[key];
	bool ok = false;
	double dvalue = ScCLocale::toDoubleC(stringValue, &ok);
	if (!ok)
		dvalue = defValue;
	return dvalue;
}

void PrefsContext::set(const QString& key, double value)
{
	// Do not use QString::arg() here, arg() use the current locale
	// We want the C locale
	set(key, QString::number(value));
}

bool PrefsContext::getBool(const QString& key, bool defValue)
{
	if (!values.contains(key))
		set(key, defValue);
	QString stringValue = values[key];
	bool ok = false;
	int ivalue = stringValue.toInt(&ok);
	if (!ok)
		ivalue = defValue;
	return ivalue;
}

void PrefsContext::set(const QString& key, bool value)
{
	set(key, QString("%1").arg(value));
}

PrefsTable* PrefsContext::getTable(const QString& name)
{
	if (!tables.contains(name))
		tables[name] = new PrefsTable(name);
	return tables[name];
}

void PrefsContext::removeTable(const QString& name)
{
	tables.remove(name);
}

PrefsContext::~PrefsContext()
{
	TableMap::Iterator it;
	for (it = tables.begin(); it != tables.end(); ++it)
		delete it.value();
}
