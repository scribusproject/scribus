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

#include "textwriter.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "text/storytext.h"

TextWriter::TextWriter(ScribusDoc *doc)
{
	document = doc;
}

void TextWriter::open(PageItem *textframe)
{
	frame = textframe;
	charStyleMap.clear();
	parStyleMap.clear();
	fontMap.clear();
	colorMap.clear();

	styledUptoPos = frame->itemText.length();

	currentCharStyle.erase();
	currentParStyle.erase();
}

void TextWriter::appendText(const QString &text)
{
	frame->itemText.insertChars(frame->itemText.length(), text);
}

void TextWriter::setStyles()
{
	StoryText& story(frame->itemText);

	if (story.length() > styledUptoPos)
	{
		story.setCharStyle(styledUptoPos, story.length() - styledUptoPos, currentCharStyle);
		for (int i = styledUptoPos; i < story.length()-1; ++i)
			if (story.text(i) == SpecialChars::PARSEP)
				story.setStyle(i+1, currentParStyle);
		styledUptoPos = story.length();
	}
}

void TextWriter::close()
{
	setStyles();
	document->redefineCharStyles(charStyles, false);
	document->redefineStyles(parStyles, false);
}

void TextWriter::setCharStyle(const CharStyle &cstyle)
{
	if (currentCharStyle != cstyle)
		setStyles();
	currentCharStyle = cstyle;
}

void TextWriter::setCharStyle(const QString &name)
{
	CharStyle cstyle;
	cstyle.setParent(name);
	setCharStyle(cstyle);
}

void TextWriter::setStyle(const ParagraphStyle &pstyle)
{
	if (currentParStyle != pstyle)
		setStyles();
	currentParStyle = pstyle;
}

void TextWriter::setStyle(const QString &name)
{
	ParagraphStyle pstyle;
	pstyle.setParent(name);
	setStyle(pstyle);
}

void TextWriter::defineCharStyle(const QString name, const CharStyle &cstyle)
{
	CharStyle* newStyle = charStyles.create(cstyle);
	newStyle->setName(name);
}

void TextWriter::defineStyle(const QString& name, const ParagraphStyle& pstyle)
{
	ParagraphStyle* newStyle = parStyles.create(pstyle);
	newStyle->setName(name);
}

void TextWriter::defineFont(const QString &name, const QString& family, const QString& variant, bool bold, bool italic)
{
	document->AddFont(name);
}

void TextWriter::defineColor(const QString &name, const ScColor& color)
{
	document->PageColors.insert(name, color);
}

CharStyle TextWriter::getCurrentCharStyle()
{
	return currentCharStyle;
}

ParagraphStyle TextWriter::getCurrentStyle()
{
	return currentParStyle;
}

ScribusDoc* TextWriter::currentDoc()
{
	return document;
}