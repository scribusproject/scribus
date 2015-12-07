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
#include <QXmlAttributes>
#include <gtstyle.h>
#include <gtwriter.h>

typedef QMap<QString, gtStyle*> StyleMap;
typedef QMap<QString, QString> FontMap;
typedef QMap<QString, int> CounterMap;

class StyleReader
{
private:
	static StyleReader *m_sreader;
	gtWriter *m_writer;
	bool m_importTextOnly;
	bool m_usePrefix;
	bool m_packStyles;
	bool m_readProperties;
	QString m_docname;
	StyleMap m_styles;
	StyleMap m_listParents;
	StyleMap m_attrsStyles;
	CounterMap m_pstyleCounts;
	FontMap m_fonts;
	gtStyle* m_currentStyle;
	gtStyle* m_parentStyle;
	bool m_inList;
	QString m_currentList;
	bool m_defaultStyleCreated;
	double getSize(QString s, double parentSize = -1);
	void styleProperties(const QXmlAttributes& attrs);
	void defaultStyle(const QXmlAttributes& attrs);
	void styleStyle(const QXmlAttributes& attrs);
	void tabStop(const QXmlAttributes& attrs);
	void setupFrameStyle();
public:
	StyleReader(QString documentName, gtWriter *wr, bool textOnly, bool prefix, bool combineStyles = true);
	~StyleReader();
	bool updateStyle(gtStyle* style, gtStyle* parent2Style, const QString& key, const QString& value);
	static void startElement(void *user_data, const xmlChar * fullname, const xmlChar ** atts);
	static void endElement(void *user_data, const xmlChar * name);
	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
	bool endElement(const QString&, const QString&, const QString &name);
	void parse(QString fileName);
	gtStyle* getDefaultStyle(void);
	gtStyle* getStyle(const QString& name);
	void setStyle(const QString& name, gtStyle* style);
	QString getFont(const QString& key);
};

#endif
