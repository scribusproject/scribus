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

#ifndef STYLEREADER_H
#define STYLEREADER_H

#include "scconfig.h"

#ifdef HAVE_XML26
 #include <libxml/SAX2.h>
#else
 #include <libxml/SAX.h>
#endif
#include <QMap>

#include <gtstyle.h>
#include <gtwriter.h>

typedef QMap<QString, gtStyle*> StyleMap;
typedef QMap<QString, QString> FontMap;
typedef QMap<QString, QString> SXWAttributesMap;
typedef QMap<QString, int> CounterMap;

class StyleReader
{
public:
	StyleReader(const QString& documentName, gtWriter *wr, bool textOnly, bool prefix, bool combineStyles = true);
	~StyleReader();
	
	void parse(const QString& fileName);

	static void startElement(void *user_data, const xmlChar * fullname, const xmlChar ** atts);
	static void endElement(void *user_data, const xmlChar * name);
	bool startElement(const QString &name, const SXWAttributesMap &attrs);
	bool endElement(const QString &name);

	gtStyle* getDefaultStyle();
	gtStyle* getStyle(const QString& name);
	void setStyle(const QString& name, gtStyle* style);
	QString getFont(const QString& key);

	bool updateStyle(gtStyle* style, gtStyle* parent2Style, const QString& key, const QString& value);

private:
	static StyleReader *sreader;

	gtWriter *writer { nullptr };
	bool importTextOnly { false };
	bool usePrefix { false };
	bool packStyles { false };
	bool readProperties { false };
	QString docname;
	StyleMap styles;
	StyleMap listParents;
	StyleMap attrsStyles;
	CounterMap pstyleCounts;
	FontMap fonts;
	gtStyle* currentStyle { nullptr };
	gtStyle* parentStyle { nullptr };
	bool inList { false };
	QString currentList;
	bool defaultStyleCreated { false };

	double getSize(const QString& s, double parentSize = -1);
	void styleProperties(const SXWAttributesMap& attrs);
	void defaultStyle(const SXWAttributesMap& attrs);
	void styleStyle(const SXWAttributesMap& attrs);
	void tabStop(const SXWAttributesMap& attrs);
	void setupFrameStyle();
};

#endif
