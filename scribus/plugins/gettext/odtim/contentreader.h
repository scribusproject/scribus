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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CONTENTREADER_H
#define CONTENTREADER_H

#include "scconfig.h"

#include <utility>
#include <vector>
#ifdef HAVE_XML26
 #include <libxml/SAX2.h>
#else
 #include <libxml/SAX.h>
#endif
#include <QXmlAttributes>
#include <QMap>
#include <gtstyle.h>
#include <gtwriter.h>
#include "stylereader.h"

typedef std::vector<std::pair<QString, QString> > Properties;
typedef QMap<QString, Properties > TMap;

class ContentReader
{
private:
	static ContentReader *creader;
	TMap tmap;
	QString docname;
	StyleReader* sreader;
	gtWriter *writer;
	gtStyle *defaultStyle;
	gtStyle *currentStyle;
	gtStyle *lastStyle;
	gtStyle *pstyle;
	bool importTextOnly;
	bool append;
	bool inList;
	bool inNote;
	bool inNoteBody;
	bool inSpan;
	int listLevel;
	int listIndex;
	ListStyle *currentListStyle;
	std::vector<int> listIndex2;
	bool inT;
	std::vector<QString> styleNames;
	QString tName;
	QString currentList;
	void write(const QString& text);
	QString getName();
	void getStyle();
public:
	ContentReader(QString documentName, StyleReader* s, gtWriter *w, bool textOnly);
	~ContentReader();
	static void startElement(void *user_data, const xmlChar *fullname, const xmlChar ** atts);
	static void endElement(void *user_data, const xmlChar *name);
	static void characters(void *user_data, const xmlChar *ch, int len);
	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
	bool endElement(const QString&, const QString&, const QString &name);
	bool characters(const QString &ch);
	void parse(QString fileName);
};

#endif
