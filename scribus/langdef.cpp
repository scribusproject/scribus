/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Aug 2012
	copyright            : (C) 2012 by Craig Bradney
	email                : cbradney@scribus.info
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "langdef.h"

LangDef::LangDef()
{
	m_hyphAvailable=false;
	m_spellAvailable=false;
	m_transAvailable=false;
};

LangDef::LangDef(const QString& pri,
			 const QString& alt,
			 const QString& name,
			 const QString& transName)
{
	m_hyphAvailable=false;
	m_spellAvailable=false;
	m_transAvailable=false;

	m_priAbbrev=pri;
	m_altAbbrev=alt;
	m_name=name;
	m_transName=transName;
};
