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

#ifndef PREFSCONTEXT_H
#define PREFSCONTEXT_H

#include <string>
#include <QMap>
#include <QString>

#include "scribusapi.h"
#include "prefstable.h"

using AttributeMap =  QMap<QString, QString>;
using TableMap = QMap<QString, PrefsTable*>;

class SCRIBUS_API PrefsContext
{
public:
	PrefsContext() = default;
	PrefsContext(const QString& contextName, bool persistent = true, bool plugin = false);
	~PrefsContext();

	AttributeMap values;
	TableMap tables;

	QString getName() const;
	bool    isPersistent() const;
	bool    isPlugin() const;
	bool    isEmpty() const;
	bool    contains(const QString& key) const;
	bool    containsTable(const QString& key) const;
	QString get(const QString& key, const QString& defValue = "");
	int     getInt(const QString& key, int defValue = -1);
	uint    getUInt(const QString& key, uint defValue = 0);
	double  getDouble(const QString& key, double defValue = -1.0);
	bool    getBool(const QString& key, bool defValue = false);
	void    set(const QString& key, const char* value);
	void    set(const QString& key, const std::string& value);
	void    set(const QString& key, const QString& value);
	void    set(const QString& key, int value);
	void    set(const QString& key, uint value);
	void    set(const QString& key, double value);
	void    set(const QString& key, bool value);
	PrefsTable* getTable(const QString& m_name);
	void    removeTable(const QString& m_name);

private:
	QString m_name;
	bool m_isPlugin { false };
	bool m_isPersistent { false };
};

#endif // PREFSCONTEXTS_H
