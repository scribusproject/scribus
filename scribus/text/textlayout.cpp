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
#include "pageitem.h"
#include "prefsstructs.h"
#include "../styles/paragraphstyle.h"
#include "specialchars.h"
#include "storytext.h"
#include "textlayout.h"




TextLayout::TextLayout(StoryText* text, PageItem* frame)
{
	m_story = text;
	m_frame = frame;

	m_firstInFrame = 0;
	m_lastInFrame = -1;
	m_magicX = 0.0;
	m_lastMagicPos = -1;
}


uint TextLayout::lines() const
{
	return m_lines.count();
}

const LineSpec& TextLayout::line(uint i) const
{
	return m_lines[i];
}

const PathData& TextLayout::point(int pos) const
{
	return m_path[pos];
}

PathData& TextLayout::point(int pos)
{
	if (pos >= story()->length())
		m_path.resize(story()->length());
	return m_path[pos];
}


void TextLayout::appendLine(const LineSpec& ls)
	{ 
		assert( ls.firstItem >= 0 );
		assert( ls.firstItem < story()->length() );
		assert( ls.lastItem >= 0 && ls.firstItem - ls.lastItem < 1 );
		assert( ls.lastItem < story()->length() );
		m_lines.append(ls);
		if (m_lastInFrame < m_firstInFrame) {
			m_firstInFrame = ls.firstItem;
			m_lastInFrame = ls.lastItem;
		}
		else {
			m_firstInFrame = qMin(m_firstInFrame, ls.firstItem);
			m_lastInFrame = qMax(m_lastInFrame, ls.lastItem);
		}
	}

// Remove the last line from the list. Used when we need to backtrack on the layouting.
void TextLayout::removeLastLine ()
{
	if (m_lines.isEmpty()) return;
	LineSpec last = m_lines.takeLast ();
	if (m_lines.isEmpty()) {
		clear();
		return;
	}
	// fix lastFrameItem
	if (m_lastInFrame != last.lastItem) return;
	m_lastInFrame = m_lines.last().lastItem;
}



void TextLayout::clear() 
{
	m_lines.clear();
	m_path.clear();
	m_firstInFrame = 0;
	m_lastInFrame = -1;
	if (m_frame->asPathText() != NULL)
		m_path.resize(story()->length());
}

void TextLayout::setStory(StoryText *story)
{
	m_story = story;
	clear();
}

int TextLayout::startOfLine(int pos) const
{
	for (int i=0; i < m_lines.count(); ++i) {
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem)
			return ls.firstItem;
	}
	return 0;
}

int TextLayout::endOfLine(int pos) const
{
	for (int i=0; i < m_lines.count(); ++i) {
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem)
			return story()->text(ls.lastItem) == SpecialChars::PARSEP ? ls.lastItem :
				story()->text(ls.lastItem) == ' ' ? ls.lastItem : ls.lastItem + 1;
	}
	return story()->length();
}

int TextLayout::prevLine(int pos) const
{
	for (int i=0; i < m_lines.count(); ++i)
	{
		// find line for pos
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem)
		{
			if (i == 0)
				return startOfLine(pos);
			// find current xpos
			qreal xpos = 0.0;
			for (int j = ls.firstItem; j < pos; ++j)
				xpos += story()->getGlyphs(j)->wide();
			if (pos != m_lastMagicPos || xpos > m_magicX)
				m_magicX = xpos;
			const LineSpec & ls2(m_lines.at(i-1));
			// find new cpos
			xpos = 0.0;
			for (int j = ls2.firstItem; j <= ls2.lastItem; ++j)
			{
				xpos += story()->getGlyphs(j)->wide();
				if (xpos > m_magicX) {
					m_lastMagicPos = j;
					return j;
				}
			}
			m_lastMagicPos = ls2.lastItem;
			return ls2.lastItem;
		}
	}
	return m_firstInFrame;
}

int TextLayout::nextLine(int pos) const
{
	for (int i=0; i < m_lines.count(); ++i)
	{
		// find line for pos
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem)
		{
			if (i+1 == m_lines.count())
				return endOfLine(pos);
			// find current xpos
			qreal xpos = 0.0;
			for (int j = ls.firstItem; j < pos; ++j)
				xpos += story()->getGlyphs(j)->wide();
			if (pos != m_lastMagicPos || xpos > m_magicX)
				m_magicX = xpos;
			const LineSpec & ls2(m_lines.at(i+1));
			// find new cpos
			xpos = 0.0;
			for (int j = ls2.firstItem; j <= ls2.lastItem; ++j)
			{
				xpos += story()->getGlyphs(j)->wide();
				if (xpos > m_magicX) {
					m_lastMagicPos = j;
					return j;
				}
			}
			m_lastMagicPos = ls2.lastItem + 1;
			return ls2.lastItem + 1;
		}
	}
	return m_lastInFrame;
}

int TextLayout::startOfFrame() const
{
	return m_firstInFrame;
}

int TextLayout::endOfFrame() const
{
	return m_lastInFrame + 1;
}


int TextLayout::screenToPosition(FPoint coord) const
{
	qreal maxx = coord.x() - 1.0;
	for (unsigned int i=0; i < lines(); ++i)
	{
		LineSpec ls = line(i);
//		qDebug() << QString("screenToPosition: (%1,%2) -> y %3 - %4 + %5").arg(coord.x()).arg(coord.y()).arg(ls.y).arg(ls.ascent).arg(ls.descent);
		if (ls.y + ls.descent < coord.y())
			continue;
		qreal xpos = ls.x;
		for (int j = ls.firstItem; j <= ls.lastItem; ++j)
		{
//				qDebug() << QString("screenToPosition: (%1,%2) -> x %3 + %4").arg(coord.x()).arg(coord.y()).arg(xpos).arg(item(j)->glyph.wide());
			qreal width = story()->getGlyphs(j)->wide();
			xpos += width;
			if (xpos >= coord.x())
			{
				if (story()->hasObject(j))
					return j;
				else
					return xpos - width/2 > coord.x() ? j : j+1;
			}
		}
		if (xpos > maxx)
			maxx = xpos;
		if (xpos + 1.0 > coord.x()) // allow 1pt after end of line
			return ls.lastItem + 1;
		else if (coord.x() <= ls.x + ls.width) // last line of paragraph?
			return ((ls.lastItem == m_lastInFrame) ? (ls.lastItem + 1) : ls.lastItem);
		else if (xpos < ls.x + 0.01 && maxx >= coord.x()) // check for empty line
			return ls.firstItem;
	}
	return qMax(m_lastInFrame+1, m_firstInFrame);
}


FRect TextLayout::boundingBox(int pos, uint len) const
{
	FRect result;
	LineSpec ls;
	for (uint i=0; i < lines(); ++i)
	{
		ls = line(i);
		if (ls.lastItem < pos)
			continue;
		if (ls.firstItem <= pos) {
			/*
			//if (ls.lastItem == pos && (item(pos)->effects() & ScLayout_SuppressSpace)  )
			{
				if (i+1 < lines())
				{
					ls = line(i+1);
					result.setRect(ls.x, ls.y - ls.ascent, 1, ls.ascent + ls.descent);
				}
				else
				{
					ls = line(lines()-1);
					const ParagraphStyle& pstyle(paragraphStyle(pos));
					result.setRect(ls.x, ls.y + pstyle.lineSpacing() - ls.ascent, 1, ls.ascent + ls.descent);
				}
			}
			else */
			{
				qreal xpos = ls.x;
				for (int j = ls.firstItem; j < pos; ++j)
				{
					if (story()->hasObject(j))
						xpos += (story()->object(j)->width() + story()->object(j)->lineWidth()) * story()->getGlyphs(j)->scaleH;
					else
						xpos += story()->getGlyphs(j)->wide();
				}
				qreal finalw = 1;
				if (story()->hasObject(pos))
					finalw = (story()->object(pos)->width() + story()->object(pos)->lineWidth()) * story()->getGlyphs(pos)->scaleH;
				else
					finalw = story()->getGlyphs(pos)->wide();
				const CharStyle& cs(story()->charStyle(pos));
				qreal desc = -cs.font().descent(cs.fontSize() / 10.0);
				qreal asce = cs.font().ascent(cs.fontSize() / 10.0);
				result.setRect(xpos, ls.y - asce, pos < story()->length()? finalw : 1, desc+asce);
			}
			return result;
		}
	}
	const ParagraphStyle& pstyle(story()->paragraphStyle(qMin(pos, story()->length()))); // rather the trailing style than a segfault.
	if (lines() > 0)
	{
		ls = line(lines()-1);
		result.setRect(ls.x, ls.y + pstyle.lineSpacing() - ls.ascent, 1, ls.ascent + ls.descent);
	}
	else
	{
		result.setRect(1, 1, 1, pstyle.lineSpacing());
	}
	return result;
}

