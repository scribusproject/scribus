/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@welho.com                                                      *
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

#include "gtwriter.h"

gtWriter::gtWriter(bool append)
{
	action = new gtAction(append);
	errorSet = false;
	action->setProgressInfo();
	setDefaultStyle();
	unSetCharacterStyle();
	unSetParagraphStyle();
	action->clearFrame();
}

gtFrameStyle* gtWriter::getDefaultStyle()
{
	return frameStyle;
}

void gtWriter::setFrameStyle(gtFrameStyle *fStyle)
{
	// @todo Update attributes of the text frame
	frameStyle = fStyle;
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

void gtWriter::unSetFrameStyle()
{
	frameStyle = defaultStyle;
}

void gtWriter::unSetParagraphStyle()
{
	paragraphStyle = NULL;
}

void gtWriter::unSetCharacterStyle()
{
	characterStyle = NULL;
}

void gtWriter::append(QString text)
{
	if (text == NULL)
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

void gtWriter::append(QString text, gtStyle *style)
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

void gtWriter::setDefaultStyle()
{
	// @todo Get the style of the text frame we are appending to.
	defaultStyle = new gtFrameStyle("Default style");
	action->getFrameStyle(defaultStyle);
	frameStyle = defaultStyle;
}

gtWriter::~gtWriter()
{
// 	action->flush();
	if (!errorSet)
		action->setProgressInfoDone();
	delete action;
	delete defaultStyle;
}
