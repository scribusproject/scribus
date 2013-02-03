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

#include "prefsreader.h"

PrefsReader::PrefsReader(ContextMap *appContexts, ContextMap *pluginContexts)
{
	aContexts = appContexts;
	pContexts = pluginContexts;
	inApp = false;
	rowIndex = 0;
	colIndex = 0;
	inTable = false;
}

bool PrefsReader::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs)
{
	if (name == "context")
	{
		for (int i = 0; i < attrs.count(); ++i)
			if (attrs.localName(i) == "name")
				currentContext = new PrefsContext(attrs.value(i));
	}
	else if (name == "level")
	{
		bool found = false;
		for (int i = 0; i < attrs.count(); ++i)
			if ((attrs.localName(i) == "name") && (attrs.value(i) == "application"))
				found = true;
		inApp = found;
	}
	else if (name == "attribute")
	{
		QString key = "";
		QString value = "";
		for (int i = 0; i < attrs.count(); ++i)
		{
			if (attrs.localName(i) == "key")
				key = attrs.value(i);
			else if (attrs.localName(i) == "value")
				value = attrs.value(i);
		}
		currentContext->set(key, value);
	}
	else if (name == "table")
	{
		for (int i = 0; i < attrs.count(); ++i)
		{
			if (attrs.localName(i) == "name")
				currentTable = currentContext->getTable(attrs.value(i));
		}
	}
	else if (name == "col")
		inTable = true;	

	return true;
}

bool PrefsReader::characters (const QString& ch)
{
	if (inTable)
		currentTable->set(rowIndex, colIndex, ch);
	return true;
} 

bool PrefsReader::endElement(const QString&, const QString&, const QString &name)
{
	if (name == "context")
	{
		if (inApp)
			(*aContexts)[currentContext->getName()] = currentContext;
		else
			(*pContexts)[currentContext->getName()] = currentContext;
	}
	else if (name == "row")
	{
		++rowIndex;
		colIndex = 0;
	}
	else if (name == "col")
	{
		++colIndex;
		inTable = false;
	}
	else if (name == "table")
	{
		rowIndex = 0;
		colIndex = 0;
	}
	return true;
}

PrefsReader::~PrefsReader()
{

}
