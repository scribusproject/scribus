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

class gtStyle;
class gtWriter;
class ListStyle;
class StyleReader;

typedef std::vector<std::pair<QString, QString> > Properties;
typedef QMap<QString, Properties > TMap;

class ContentReader
{
private:
	static ContentReader *m_creader;
	TMap m_tmap;
	QString m_docname;
	StyleReader* m_sreader;
	gtWriter *m_writer;
	gtStyle *m_defaultStyle;
	gtStyle *m_currentStyle;
	gtStyle *m_lastStyle;
	gtStyle *m_pstyle;
	bool m_importTextOnly;
	bool m_inList;
	bool m_inAnnotation;
	bool m_inSpan;
	int  m_append;
	int  m_listLevel;
	int  m_listIndex;
	ListStyle *m_currentListStyle;
	std::vector<int> m_listIndex2;
	bool m_inT;
	std::vector<QString> m_styleNames;
	QString m_tName;
	QString m_currentList;
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
