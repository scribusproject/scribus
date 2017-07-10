/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#undef NDEBUG

#include <cassert>
#include "../styles/charstyle.h"
#include "prefsstructs.h"
#include "../styles/paragraphstyle.h"
#include "specialchars.h"
#include "storytext.h"
#include "textlayout.h"
#include "textlayoutpainter.h"
#include "screenpainter.h"
#include "boxes.h"
#include "itextcontext.h"



TextLayout::TextLayout(StoryText* text, ITextContext* frame)
{
	m_story = text;
	m_frame = frame;

	m_validLayout = false;
	m_magicX = 0.0;
	m_lastMagicPos = -1;

	m_box = new GroupBox(Box::D_Horizontal);
}

TextLayout::~TextLayout()
{
	delete m_box;
}

uint TextLayout::lines() const
{
	uint count = 0;
	foreach (const Box *box, m_box->boxes())
	{
		count += box->boxes().count();
	}
	return count;
}

const LineBox* TextLayout::line(uint i) const
{
	uint count = 0;
	foreach (const Box *box, m_box->boxes())
	{
		if (i < count + box->boxes().count())
			return dynamic_cast<const LineBox*>(box->boxes()[i - count]);
		count += box->boxes().count();
	}
	assert(false);
	return NULL;
}

const Box* TextLayout::box() const
{
	return m_box;
}

Box* TextLayout::box()
{
	return m_box;
}

void TextLayout::appendLine(LineBox* ls)
{
	assert(ls);
	assert(ls->firstChar() >= 0);
	assert(ls->firstChar() < story()->length());
	assert(ls->lastChar() < story()->length());
	assert(!m_box->boxes().empty());

	GroupBox* column = dynamic_cast<GroupBox*>(m_box->boxes().last());
	assert(column);
	if (ls->type() == Box::T_PathLine)
		ls->setAscent(ls->y() - column->naturalHeight());

	ls->setWidth(column->width());
	column->addBox(ls);
}

// Remove the last line from the list. Used when we need to backtrack on the layouting.
void TextLayout::removeLastLine ()
{
	QList<Box*>& boxes = m_box->boxes();
	if (boxes.isEmpty())
		return;

	GroupBox* column = dynamic_cast<GroupBox*>(boxes.last());
	assert(column);

	int lineCount = column->boxes().count();
	if (lineCount > 0)
		column->removeBox(lineCount - 1);
}

void TextLayout::render(ScreenPainter *p, ITextContext *ctx)
{
	p->save();
	m_box->render(p, ctx);
	p->restore();
}

void TextLayout::renderBackground(TextLayoutPainter *p)
{
	foreach (const Box* column, m_box->boxes())
	{
		const ParagraphStyle& style = m_story->paragraphStyle(column->firstChar());
		if (style.backgroundColor() != CommonStrings::None)
		{
			p->save();
			TextLayoutColor backColor(style.backgroundColor(), style.backgroundShade());
			p->setFillColor(backColor);
			p->setStrokeColor(backColor);
			p->drawRect(column->bbox());
			p->restore();
		}
	}
}

void TextLayout::render(TextLayoutPainter *p)
{
	p->save();
	m_box->render(p);
	p->restore();
}

void TextLayout::addColumn(double colLeft, double colWidth)
{
	GroupBox *newBox = new GroupBox(Box::D_Vertical);
	newBox->moveTo(colLeft, 0.0);
	newBox->setWidth(colWidth);
	newBox->setAscent(m_frame->height());
	m_box->addBox(newBox);

	// Update the box width and height, any better place to do this?
	m_box->setAscent(m_frame->height());
	m_box->setWidth(m_frame->width());
}

void TextLayout::clear() 
{
	delete m_box;
	m_box = new GroupBox(Box::D_Horizontal);
}

void TextLayout::setStory(StoryText *story)
{
	m_story = story;
	clear();
}

int TextLayout::startOfLine(int pos) const
{
	for (uint i=0; i < lines(); ++i) {
		const LineBox* ls = line(i);
		if (ls->firstChar() <= pos && pos <= ls->lastChar())
			return ls->firstChar();
	}
	return 0;
}

int TextLayout::endOfLine(int pos) const
{
	for (uint i=0; i < lines(); ++i) {
		const LineBox* ls = line(i);
		if (ls->containsPos(pos))
			return story()->text(ls->lastChar()) == SpecialChars::PARSEP ? ls->lastChar() :
				story()->text(ls->lastChar()) == ' ' ? ls->lastChar() : ls->lastChar() + 1;
	}
	return story()->length();
}

int TextLayout::prevLine(int pos) const
{
	bool isRTL = (m_story->paragraphStyle().direction() == ParagraphStyle::RTL);
	for (uint i=0; i < lines(); ++i)
	{
		// find line for pos
		const LineBox* ls = line(i);
		if (ls->containsPos(pos))
		{
			if (i == 0)
				return startOfLine(pos);
			// find current xpos
			qreal xpos = ls->positionToPoint(pos, *m_story).x1();
			if (pos != m_lastMagicPos || xpos > m_magicX)
				m_magicX = xpos;

			const LineBox* ls2 = line(i-1);
			// find new cpos
			for (int j = ls2->firstChar(); j <= ls2->lastChar(); ++j)
			{
				xpos = ls2->positionToPoint(j, *m_story).x1();
				if ((isRTL && xpos <= m_magicX) || (!isRTL && xpos >= m_magicX))
				{
					m_lastMagicPos = j;
					return j;
				}
			}
			m_lastMagicPos = ls2->lastChar();
			return ls2->lastChar();
		}
	}
	return m_box->firstChar();
}

int TextLayout::nextLine(int pos) const
{
	bool isRTL = (m_story->paragraphStyle().direction() == ParagraphStyle::RTL);
	for (uint i=0; i < lines(); ++i)
	{
		// find line for pos
		const LineBox* ls = line(i);
		if (ls->containsPos(pos))
		{
			if (i+1 == lines())
				return endOfLine(pos);
			// find current xpos
			qreal xpos = ls->positionToPoint(pos, *m_story).x1();

			if (pos != m_lastMagicPos || xpos > m_magicX)
				m_magicX = xpos;

			const LineBox* ls2 = line(i+1);
			// find new cpos
			for (int j = ls2->firstChar(); j <= ls2->lastChar(); ++j)
			{
				xpos = ls2->positionToPoint(j, *m_story).x1();
				if ((isRTL && xpos <= m_magicX) || (!isRTL && xpos >= m_magicX))
				{
					m_lastMagicPos = j;
					return j;
				}
			}
			m_lastMagicPos = ls2->lastChar() + 1;
			return ls2->lastChar() + 1;
		}
	}
	return m_box->lastChar();
}

int TextLayout::startOfFrame() const
{
	QList<Box*>& boxes = m_box->boxes();
	if (boxes.isEmpty())
		return 0;

	const GroupBox* column = dynamic_cast<const GroupBox*>(boxes.first());
	assert(column);

	return column->firstChar();
}

int TextLayout::endOfFrame() const
{
	QList<Box*>& boxes = m_box->boxes();
	if (boxes.isEmpty())
		return 0;

	const GroupBox* column = dynamic_cast<const GroupBox*>(boxes.last());
	assert(column);

	return column->lastChar() + 1;
}


int TextLayout::pointToPosition(QPointF coord) const
{
	int position = m_box->pointToPosition(coord, *m_story);
	return position;
}


QLineF TextLayout::positionToPoint(int pos) const
{
	QLineF result;
	bool isRTL = (m_story->paragraphStyle().direction() == ParagraphStyle::RTL);

	result = m_box->positionToPoint(pos, *m_story);
	if (result.isNull())
	{
		qreal x, y1, y2;
		if (lines() > 0)
		{
			// TODO: move this branch to GroupBox::positionToPoint()
			// last glyph box in last line
			Box* column = m_box->boxes().last();
			if (column->boxes().count() > 0)
			{
				Box* line = column->boxes().last();
				Box* glyph = line->boxes().empty() ? NULL : line->boxes().last();
				QChar ch = story()->text(line->lastChar());
				if (ch == SpecialChars::PARSEP || ch == SpecialChars::LINEBREAK)
				{
					// last character is a newline, draw the cursor on the next line.
					if (isRTL)
						x = line->width();
					else
						x = 1;
					y1 = line->y() + line->height();
					y2 = y1 + line->height();
				}
				else
				{
					// draw the cursor at the end of last line.
					if (isRTL || glyph == NULL)
						x = line->x();
					else
						x = line->x() + glyph->x() + glyph->width();
					y1 = line->y();
					y2 = y1 + line->height();
				}
			}
			else
			{
				const ParagraphStyle& pstyle(story()->paragraphStyle(qMin(pos, story()->length())));
				if (isRTL)
					x = column->width();
				else
					x = 1;
				y1 = 0;
				y2 = pstyle.lineSpacing();
			}
			result.setLine(x, y1, x, y2);
			result.translate(column->x(), column->y());
		}
		else
		{
			// rather the trailing style than a segfault.
			const ParagraphStyle& pstyle(story()->paragraphStyle(qMin(pos, story()->length())));
			if (isRTL)
				x = m_box->width();
			else
				x = 1;
			y1 = 0;
			y2 = pstyle.lineSpacing();
			result.setLine(x, y1, x, y2);
		}
		result.translate(m_box->x(), m_box->y());
	}

	return result;
}
