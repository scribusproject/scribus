/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 /***************************************************************************
  *   Copyright (C) 2009 by Andreas Vox                                     *
  *   avox@scribus.info                                                     *
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

#ifndef TEXTWRITER_H
#define TEXTWRITER_H

#include <QString>
#include <QMap>
#include "scconfig.h"
#include "scribusapi.h"

#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "styles/styleset.h"

class ScribusDoc;
class PageItem;
class ScColor;

/**
 * Helper class for importing text
 */
class SCRIBUS_API TextWriter 
{
public:
	TextWriter(ScribusDoc* doc);
	/**
	 Sets the target for following set* and append operations.
	 Only one pageitem of a linked chain may be opened at the same time,
	 it's responsibility of the programmer to ensure that
	 */
	void open(PageItem* textframe);
	/**
	 Writes all pending information to the target. This method returns
	 the document into a consistent state. Failing to call it may cause
	 data loss or corruption.
	*/
	void close();
	/**
	Return the current character style
	*/
	CharStyle getCurrentCharStyle();
	/**
	Return the current character style
	*/
	ParagraphStyle getCurrentStyle();
	/**
	 Sets the character style for following append operations
	 */
	void setCharStyle(const CharStyle& cstyle);
	void setCharStyle(const QString& name);
	/**
	 Sets the paragraph style for following append operations, ie. the
	 style to be used when the next paragraph separator (\n) is appended.
	 The current paragraph style will not be changed
	 */
	void setStyle(const ParagraphStyle& pstyle);
	void setStyle(const QString& name);
	/**
	 Appends text using current character style and paragraph style
	 */
	void appendText(const QString& text);

	/**
	 Introduces a new named character style into the document
	 */
	void defineCharStyle(const QString name, const CharStyle& cstyle);
	/**
	 Introduces a new named paragraph style into the document
	 */
	void defineStyle(const QString& name, const ParagraphStyle& pstyle);
	/**
	 Introduces a new named font into the document.
	 If the document already uses a font 'name', use that. Otherwise find a match
	 using 'family', 'variant', 'bold' and 'italic'
	 */
	void defineFont(const QString& name, const QString& family, const QString& variant, bool bold=false, bool italic=false);
	/**
	 Introduces a new named color into the document
	 */
	void defineColor(const QString& name, const ScColor& color);
	ScribusDoc* currentDoc();

private:
	ScribusDoc* document;
	PageItem* frame;

	CharStyle currentCharStyle;
	ParagraphStyle currentParStyle;

	int styledUptoPos;
	void setStyles();

	StyleSet<CharStyle> charStyles;
	StyleSet<ParagraphStyle> parStyles;

	// for resolving name conflicts:
	QMap<QString, QString> parStyleMap;
	QMap<QString, QString> charStyleMap;
	QMap<QString, QString> fontMap;
	QMap<QString, QString> colorMap;
};

#endif