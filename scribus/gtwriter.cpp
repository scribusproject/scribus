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

#include "gtwriter.h"
#include "gtframestyle.h"

#include "pageitem.h"
#include "scribusstructs.h"

// gtWriter::gtWriter(bool append)
// {
// 	action = new gtAction(append, ScMW->doc->m_Selection->itemAt(0));
// 	errorSet = false;
// 	action->setProgressInfo();
// 	setDefaultStyle();
// 	unsetCharacterStyle();
// 	unsetParagraphStyle();
// // 	if (!append)
// // 		action->clearFrame();
// // 	else
// // 		this->append("\n");
// }

gtWriter::gtWriter(bool append, PageItem *pageitem)
{
	m_action = new gtAction(append, pageitem);
	m_defaultStyle = NULL;
	m_currentStyle = NULL;
	m_errorSet = false;

	m_action->setProgressInfo();
	setDefaultStyle();
	unsetCharacterStyle();
	unsetParagraphStyle();
	inNote = false;
	inNoteBody = false;
}

gtFrameStyle* gtWriter::getDefaultStyle()
{
	return m_frameStyle;
}

void gtWriter::setFrameStyle(gtFrameStyle *fStyle)
{
	m_frameStyle = fStyle;
	m_action->applyFrameStyle(fStyle);
}

void gtWriter::setParagraphStyle(gtParagraphStyle *pStyle)
{
	// @todo Start a new paragraph and add the style to the Paragraph Styles
	m_paragraphStyle = pStyle;
}

void gtWriter::setCharacterStyle(gtStyle *cStyle)
{
	m_characterStyle = cStyle;
}

void gtWriter::unsetFrameStyle()
{
	m_frameStyle = m_defaultStyle;
}

void gtWriter::unsetParagraphStyle()
{
	m_paragraphStyle = NULL;
}

void gtWriter::unsetCharacterStyle()
{
	m_characterStyle = NULL;
}

double gtWriter::getPreferredLineSpacing(int fontSize)
{
	return m_action->getLineSpacing(fontSize);
}

double gtWriter::getPreferredLineSpacing(double fontSize)
{
	return getPreferredLineSpacing(static_cast<int>(fontSize * 10));
}

void gtWriter::append(const QString& text)
{
	if (text.isEmpty())
		return;
	if (text.length() == 0)
		return;
	if (inNote && !inNoteBody)
		return;
	if (m_characterStyle != NULL)
	{
		m_action->write(text, m_characterStyle, inNote);
	}
	else if (m_paragraphStyle != NULL)
	{
		m_action->write(text, m_paragraphStyle, inNote);
	}
	else
	{
		m_action->write(text, m_frameStyle, inNote);
	}
}

void gtWriter::appendUnstyled(const QString& text)
{
	if (text.isEmpty())
		return;
	if (text.length() == 0)
		return;
	if (inNote && !inNoteBody)
		return;
	m_action->writeUnstyled(text, inNote);
}

double gtWriter::getFrameWidth()
{
	return m_action->getFrameWidth();
}

QString gtWriter::getFrameName()
{
	return m_action->getFrameName();
}

void gtWriter::append(const QString& text, gtStyle *style)
{
	if (inNote && !inNoteBody)
		return;
	if (style == NULL)
	{
		append(text);
		return;
	}

	m_currentStyle = style;
	m_action->write(text, style, inNote);
	m_currentStyle = NULL;
}

void gtWriter::append(const QString& text, gtStyle *style, bool updatePStyle)
{
	bool ups = m_action->getUpdateParagraphStyles();
	m_action->setUpdateParagraphStyles(updatePStyle);
	append(text, style);
	m_action->setUpdateParagraphStyles(ups);
}

void gtWriter::setDefaultStyle()
{
	// @todo Get the style of the text frame we are appending to.
	m_defaultStyle = new gtFrameStyle("Default style");
	m_action->getFrameStyle(m_defaultStyle);
	m_frameStyle = m_defaultStyle;
}

bool gtWriter::getUpdateParagraphStyles()
{
	return m_action->getUpdateParagraphStyles();
}

void gtWriter::setUpdateParagraphStyles(bool newUPS)
{
	m_action->setUpdateParagraphStyles(newUPS);
}

bool gtWriter::getOverridePStyleFont()
{
	return m_action->getOverridePStyleFont();
}

void gtWriter::setOverridePStyleFont(bool newOPSF)
{
	m_action->setOverridePStyleFont(newOPSF);
}

gtWriter::~gtWriter()
{
	if (!m_errorSet)
		m_action->setProgressInfoDone();
	if (m_action)
		delete m_action;
	if (m_defaultStyle)
		delete m_defaultStyle;
}
