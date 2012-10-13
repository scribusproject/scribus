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

#ifndef LANGDEF_H
#define LANGDEF_H

#include <QString>

#include "scribusapi.h"

class SCRIBUS_API LangDef
{
	public:
		LangDef();
		LangDef(const QString& pri, const QString& alt, const QString& name, const QString& transName);
		~LangDef() {};

		QString m_priAbbrev;
		QString m_altAbbrev;
		QString m_name;
		QString m_transName;
		bool m_hyphAvailable;
		QString m_hyphFile;
		bool m_spellAvailable;
		QString m_spellFile;
		bool m_transAvailable;
		QString m_transFile;
};

#endif // LANGDEF_H
