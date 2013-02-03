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

#ifndef PREFSFILE_H
#define PREFSFILE_H

#include <QMap>
#include <QTextStream>

#include "scribusapi.h"
#include "prefscontext.h"

typedef QMap<QString, PrefsContext*> ContextMap;

class SCRIBUS_API PrefsFile
{
private:
	QString prefsFilePath;
	ContextMap contexts;
	ContextMap pluginContexts;
	ContextMap userprefsContexts;
	bool ioEnabled;
	void load();
	QString replaceIllegalChars(const QString& text);
	void writeContexts(ContextMap* contextMap, QTextStream& stream);
	void canWrite();
public:
	PrefsFile();
	PrefsFile(const QString& pFilePath, bool write = true);
	~PrefsFile();
	bool          hasContext(const QString& contextName) const;
	PrefsContext* getContext(const QString& contextName, bool persistent = true);
	PrefsContext* getPluginContext(const QString& contextName, bool persistent = true);
	PrefsContext* getUserPrefsContext(const QString& contextName, bool persistent = true);
	void write();
};

#endif // PREFSFILE_H
