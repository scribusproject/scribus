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
  *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
  ***************************************************************************/

#include "textwriter.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "text/storytext.h"

TextWriter::TextWriter(ScribusDoc *doc)
{
	m_document = doc;
}

void TextWriter::open(PageItem *textframe)
{
	m_frame = textframe;
	m_charStyleMap.clear();
	m_parStyleMap.clear();
	m_fontMap.clear();
	m_colorMap.clear();

	m_styledUptoPos = m_frame->itemText.length();

	m_currentCharStyle.erase();
	m_currentParStyle.erase();
}

void TextWriter::appendText(const QString &text)
{
	m_frame->itemText.insertChars(m_frame->itemText.length(), text);
}

void TextWriter::setStyles()
{
	StoryText& story(m_frame->itemText);

	if (story.length() > m_styledUptoPos)
	{
		story.setCharStyle(m_styledUptoPos, story.length() - m_styledUptoPos, m_currentCharStyle);
		for (int i = m_styledUptoPos; i < story.length()-1; ++i)
			if (story.text(i) == SpecialChars::PARSEP)
				story.setStyle(i+1, m_currentParStyle);
		m_styledUptoPos = story.length();
	}
}

void TextWriter::close()
{
	setStyles();
	m_document->redefineCharStyles(m_charStyles, false);
	m_document->redefineStyles(m_parStyles, false);
}

void TextWriter::setCharStyle(const CharStyle &cstyle)
{
	if (m_currentCharStyle != cstyle)
		setStyles();
	m_currentCharStyle = cstyle;
}

void TextWriter::setCharStyle(const QString &name)
{
	CharStyle cstyle;
	cstyle.setParent(name);
	setCharStyle(cstyle);
}

void TextWriter::setStyle(const ParagraphStyle &pstyle)
{
	if (m_currentParStyle != pstyle)
		setStyles();
	m_currentParStyle = pstyle;
}

void TextWriter::setStyle(const QString &name)
{
	ParagraphStyle pstyle;
	pstyle.setParent(name);
	setStyle(pstyle);
}

void TextWriter::defineCharStyle(const QString name, const CharStyle &cstyle)
{
	CharStyle* newStyle = m_charStyles.create(cstyle);
	newStyle->setName(name);
}

void TextWriter::defineStyle(const QString& name, const ParagraphStyle& pstyle)
{
	ParagraphStyle* newStyle = m_parStyles.create(pstyle);
	newStyle->setName(name);
}

void TextWriter::defineFont(const QString &name, const QString& family, const QString& variant, bool bold, bool italic)
{
	m_document->AddFont(name);
}

void TextWriter::defineColor(const QString &name, const ScColor& color)
{
	m_document->PageColors.insert(name, color);
}

CharStyle TextWriter::getCurrentCharStyle()
{
	return m_currentCharStyle;
}

ParagraphStyle TextWriter::getCurrentStyle()
{
	return m_currentParStyle;
}

ScribusDoc* TextWriter::currentDoc()
{
	return m_document;
}