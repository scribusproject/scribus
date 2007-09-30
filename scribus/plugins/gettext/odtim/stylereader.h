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


enum BulletType {
	Bullet,
	Number,
	LowerRoman,
	UpperRoman,
	LowerAlpha,
	UpperAlpha,
	Graphic
};

class ListLevel
{
public:
	ListLevel(uint level,
	          BulletType btype,
	          const QString &prefix,
	          const QString &suffix,
	          const QString &bullet,
	          uint displayLevels = 1,
	          uint startValue = 0);
	~ListLevel();
	QString bulletString();
	QString bullet();
	QString prefix();
	QString suffix();
	void advance();
	void reset();
	uint level();
	uint displayLevels();
private:
	uint m_level;
	BulletType m_btype;
	QString m_prefix;
	QString m_suffix;
	QString m_bullet;
	uint m_displayLevels;
	uint m_next;
	static const QString lowerUnits[10];
	static const QString lowerTens[10];
	static const QString lowerHundreds[10];
	static const QString lowerThousands[4];
	static const QString upperUnits[10];
	static const QString upperTens[10];
	static const QString upperHundreds[10];
	static const QString upperThousands[4];
	static const QString lowerAlphabets[27];
	static const QString upperAlphabets[27];
	QString lowerRoman(uint n);
	QString upperRoman(uint n);
	QString lowerAlpha(uint n);
	QString upperAlpha(uint n);
};

class ListStyle
{
public:
	ListStyle(const QString &name, bool consecutiveNumbering = false, uint currentLevel = 1);
	~ListStyle();
	void addLevel(uint level, ListLevel *llevel);
	QString bullet();
	void advance();
	void setLevel(uint level);
	void resetLevel();
	QString& name();
private:
	QString m_name;
	bool m_consecutiveNumbering;
	uint m_currentLevel;
	uint m_count;
	ListLevel* levels[11];
};

typedef QMap<QString, gtStyle*> StyleMap;
typedef QMap<QString, QString> FontMap;
typedef QMap<QString, int> CounterMap;
typedef QMap<QString, ListStyle*> ListMap;

class StyleReader
{
private:
	static StyleReader *sreader;
	gtWriter *writer;
	bool importTextOnly;
	bool usePrefix;
	bool packStyles;
	bool readProperties;
	QString docname;
	StyleMap styles;
	StyleMap listParents;
	StyleMap attrsStyles;
	CounterMap pstyleCounts;
	FontMap fonts;
	ListMap lists;
	gtStyle* currentStyle;
	gtStyle* parentStyle;
	bool inList;
	QString currentList;
	ListStyle *currentListStyle;
	bool defaultStyleCreated;
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
	ListStyle *getList(const QString &name);
};

#endif
