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
	m_aContexts = appContexts;
	m_pContexts = pluginContexts;
	m_currentContext = 0;
	m_currentTable = 0;
	m_inApp = false;
	m_rowIndex = 0;
	m_colIndex = 0;
	m_inTable = false;
}

bool PrefsReader::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs)
{
	if (name == "context")
	{
		for (int i = 0; i < attrs.count(); ++i)
			if (attrs.localName(i) == "name")
				m_currentContext = new PrefsContext(attrs.value(i));
	}
	else if (name == "level")
	{
		bool found = false;
		for (int i = 0; i < attrs.count(); ++i)
			if ((attrs.localName(i) == "name") && (attrs.value(i) == "application"))
				found = true;
		m_inApp = found;
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
		m_currentContext->set(key, value);
	}
	else if (name == "table")
	{
		for (int i = 0; i < attrs.count(); ++i)
		{
			if (attrs.localName(i) == "name")
				m_currentTable = m_currentContext->getTable(attrs.value(i));
		}
	}
	else if (name == "col")
		m_inTable = true;

	return true;
}

bool PrefsReader::characters (const QString& ch)
{
	if (m_inTable)
		m_currentTable->set(m_rowIndex, m_colIndex, ch);
	return true;
} 

bool PrefsReader::endElement(const QString&, const QString&, const QString &name)
{
	if (name == "context")
	{
		if (m_inApp)
			(*m_aContexts)[m_currentContext->getName()] = m_currentContext;
		else
			(*m_pContexts)[m_currentContext->getName()] = m_currentContext;
	}
	else if (name == "row")
	{
		++m_rowIndex;
		m_colIndex = 0;
	}
	else if (name == "col")
	{
		++m_colIndex;
		m_inTable = false;
	}
	else if (name == "table")
	{
		m_rowIndex = 0;
		m_colIndex = 0;
	}
	return true;
}

PrefsReader::~PrefsReader()
{

}
