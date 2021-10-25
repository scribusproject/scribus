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

#ifndef PREFSREADER_H
#define PREFSREADER_H

#include <vector>

#include <QString>
#include <QXmlDefaultHandler>

#include "prefscontext.h"
#include "prefsfile.h"
#include "prefstable.h"
#include "scribusapi.h"

class SCRIBUS_API PrefsReader : public QXmlDefaultHandler
{
public:
	PrefsReader(ContextMap *appContexts, ContextMap *pluginContexts);
	~PrefsReader() override = default;

	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
	bool characters (const QString& ch); 
	bool endElement(const QString&, const QString&, const QString &name);

private:
	ContextMap* m_aContexts { nullptr };
	ContextMap* m_pContexts { nullptr };
	PrefsContext* m_currentContext { nullptr };
	PrefsTable* m_currentTable { nullptr };
	bool m_inApp { false };
	bool m_inTable { false };
	int m_rowIndex { 0 };
	int m_colIndex { 0 };
};

#endif
