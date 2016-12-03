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

#ifndef TEXTLAYOUT_H
#define TEXTLAYOUT_H

#include <QList>

#include "scribusapi.h"

#include "fpoint.h"
#include "frect.h"
#include "scpainter.h"
#include "sctextstruct.h"

class StoryText;
class Box;
class GroupBox;
class LineBox;
class TextLayoutPainter;
class ScreenPainter;
class ITextContext;
/**
	This class manages the physical layout of a textframe, ie. its line 
	structure and the lines' glyph layouts. It will use some of the layouters above to create a Box.
    It listens to change events from the StoryText and the PageItem and will update the layout accordingly.
 */
class SCRIBUS_API TextLayout
{
public:
	TextLayout(StoryText* text, ITextContext* frame);
	~TextLayout();

	bool overflows() const;
	
	StoryText* story() { return m_story; }
	ITextContext*  frame() { return m_frame; }
	const StoryText* story() const { return m_story; }
	void setStory(StoryText* story);
	void render(ScreenPainter *p, ITextContext *item);
	void render(TextLayoutPainter *p);
	void renderBackground(TextLayoutPainter *p);
	int startOfLine(int pos) const;
	int endOfLine(int pos) const;
	int prevLine(int pos) const;
	int nextLine(int pos) const;
	int startOfFrame() const;
	int endOfFrame() const;

	int pointToPosition(QPointF coord) const;
	QLineF positionToPoint(int pos) const;

	uint lines() const;
	
	const LineBox*  line(uint i) const;
	const Box* box() const;
	Box* box();

	void appendLine(LineBox* ls);
	void removeLastLine ();
	void addColumn(double colLeft, double colWidth);

	void clear();

protected:
	friend class FrameControl;
	
	StoryText* m_story;
    ITextContext* m_frame;
	GroupBox* m_box;
	
	bool m_validLayout;
	mutable qreal m_magicX;
	mutable int m_lastMagicPos;

};

#endif
