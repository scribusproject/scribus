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
//#include "scfonts.h"
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
	action = new gtAction(append, pageitem);
	errorSet = false;
	action->setProgressInfo();
	setDefaultStyle();
	unsetCharacterStyle();
	unsetParagraphStyle();
}

gtFrameStyle* gtWriter::getDefaultStyle()
{
	return frameStyle;
}

void gtWriter::setFrameStyle(gtFrameStyle *fStyle)
{
	frameStyle = fStyle;
	action->applyFrameStyle(fStyle);
}

void gtWriter::setParagraphStyle(gtParagraphStyle *pStyle)
{
	// @todo Start a new paragraph and add the style to the Paragraph Styles
	paragraphStyle = pStyle;
}

void gtWriter::setCharacterStyle(gtStyle *cStyle)
{
	characterStyle = cStyle;
}

void gtWriter::unsetFrameStyle()
{
	frameStyle = defaultStyle;
}

void gtWriter::unsetParagraphStyle()
{
	paragraphStyle = NULL;
}

void gtWriter::unsetCharacterStyle()
{
	characterStyle = NULL;
}

double gtWriter::getPreferredLineSpacing(int fontSize)
{
	return action->getLineSpacing(fontSize);
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
	if (characterStyle != NULL)
	{
		action->write(text, characterStyle);
	}
	else if (paragraphStyle != NULL)
	{
		action->write(text, paragraphStyle);
	}
	else
	{
		action->write(text, frameStyle);
	}
}

void gtWriter::appendUnstyled(const QString& text)
{
	if (text.isEmpty())
		return;
	if (text.length() == 0)
		return;
	action->writeUnstyled(text);
}

double gtWriter::getFrameWidth()
{
	return action->getFrameWidth();
}

QString gtWriter::getFrameName()
{
	return action->getFrameName();
}

void gtWriter::append(const QString& text, gtStyle *style)
{
	if (style == NULL)
	{
		append(text);
		return;
	}

	currentStyle = style;
	action->write(text, style);
	currentStyle = NULL;
}

void gtWriter::append(const QString& text, gtStyle *style, bool updatePStyle)
{
	bool ups = action->getUpdateParagraphStyles();
	action->setUpdateParagraphStyles(updatePStyle);
	append(text, style);
	action->setUpdateParagraphStyles(ups);
}

void gtWriter::setDefaultStyle()
{
	// @todo Get the style of the text frame we are appending to.
	defaultStyle = new gtFrameStyle("Default style");
	action->getFrameStyle(defaultStyle);
	frameStyle = defaultStyle;
}

bool gtWriter::getUpdateParagraphStyles()
{
	return action->getUpdateParagraphStyles();
}

void gtWriter::setUpdateParagraphStyles(bool newUPS)
{
	action->setUpdateParagraphStyles(newUPS);
}

bool gtWriter::getOverridePStyleFont()
{
	return action->getOverridePStyleFont();
}

void gtWriter::setOverridePStyleFont(bool newOPSF)
{
	action->setOverridePStyleFont(newOPSF);
}

gtWriter::~gtWriter()
{
	if (!errorSet)
		action->setProgressInfoDone();
	delete action;
	delete defaultStyle;
}
