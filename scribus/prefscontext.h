/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@welho.com                                                      *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PREFSCONTEXT_H
#define PREFSCONTEXT_H

#include <string>
#include <qmap.h>
#include <qstring.h>
#include "prefstable.h"

typedef QMap<QString, QString> AttributeMap;
typedef QMap<QString, PrefsTable*> TableMap;

class PrefsContext
{
private:
	QString name;
	bool isplugin;
	bool ispersistent;
public:
	AttributeMap values;
	TableMap tables;
	PrefsContext();
	PrefsContext(QString contextName, bool persistent = true, bool plugin = false);
	~PrefsContext();
	QString getName();
	bool    isPersistent();
	bool    isPlugin();
	bool    isEmpty();
	bool    contains(const QString& key);
	bool    containsTable(const QString& key);
	QString get(const QString& key, const QString& defValue = "");
	void    set(const QString& key, const char* value);
	void    set(const QString& key, const std::string& value);
	void    set(const QString& key, const QString& value);
	int     getInt(const QString& key, int defValue = -1);
	void    set(const QString& key, int value);
	uint    getUInt(const QString& key, uint defValue = 0);
	double  getDouble(const QString& key, double defValue = -1.0);
	void    set(const QString& key, double value);
	bool    getBool(const QString& key, bool defValue = false);
	void    set(const QString& key, bool value);
	PrefsTable* getTable(const QString& name);
};

#endif // PREFSCONTEXTS_H
