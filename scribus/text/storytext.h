/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
pageitem.cpp  -  description
-------------------
    begin                : Sat Apr 7 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
	***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#ifndef STORYTEXT_H_
#define STORYTEXT_H_

#include <QObject>
#include <QString>
#include <QList>
#include <cassert>

//#include "text/paragraphlayout.h"
#include "text/frect.h"
#include "text/specialchars.h"
#include "sctextstruct.h"
#include "style.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "desaxe/saxio.h"

#ifdef NLS_CONFORMANCE
#define NLS_PRIVATE private
#else
#define NLS_PRIVATE public
#endif

class CharStyle;
class ParagraphStyle;
class PageItem;
//class ScTextEngine;
//class ScScriptItem;
class ScribusDoc;
class ScText_Shared;
class ResourceCollection;
 

struct LineSpec 
{
	double x;
	double y;
	double width;
	double ascent;
	double descent;
	
	int firstItem;
	int lastItem;
	double naturalWidth;
};

/**
 * This class holds the text of a Scribus textframe and pointers to its
 * styles and embedded objects.
 *
 * The logical view of the text consists of a sequence of Unicode chars.
 * Partition objects keep track of the positions of style changes,
 * paragraph ends and embedded objects.
 *
 * The physical view consists of a sequence of ScriptItems. Each ScriptItem
 * corresponds to a subsequence of Unicode chars in the original sequence
 * and associates this with an array of glyph indices. Metrics information
 * give the physical position of the ScriptItem in the textframe and its
 * bounding box. For each glyph there's also its advance and the relative
 * offsets to its basepoint. Other information in the ScriptItem is only
 * used by the layouter.
 */
class SCRIBUS_API StoryText : public QObject, public SaxIO
{
	Q_OBJECT
	
 public:
 	StoryText(ScribusDoc * doc);
 	StoryText();
 	StoryText(const StoryText & other);
 	StoryText& operator= (const StoryText & other);
 	virtual ~StoryText();
	
	static const Xml_string saxxDefaultElem;
	static void  desaxeRules(const Xml_string& prefixPattern, desaxe::Digester& ruleset, Xml_string elemtag = saxxDefaultElem);
	
	virtual void saxx(SaxHandler& handler, const Xml_string& elemtag) const;
	virtual void saxx(SaxHandler& handler)                     const { saxx(handler, saxxDefaultElem); }
	
 	void clear();
	StoryText copy() const;
	void insert(int pos, const StoryText& other, bool onlySelection = false);
	void append(const StoryText& other) { insert(length(), other, false); }
 	void removeChars(int pos, uint len);
 	void insertChars(int pos, QString txt, bool applyNeighbourStyle = false);
 	void insertObject(int pos, PageItem* obj);
 	void replaceChar(int pos, QChar ch);

	void hyphenateWord(int pos, uint len, char* hyphens);
	
 	int length() const;
 	QChar text(int pos) const;
 	QString text(int pos, uint len) const;
 	PageItem* object(int pos) const;
	
	int nextCharPos(int c);
	int prevCharPos(int c);
	int nextWordPos(int c);
	int prevWordPos(int c);
	int nextLinePos(int c, double oldX);
	int prevLinePos(int c, double oldX);
	int nextFramePos(int c);
	int prevFramePos(int c);
	
 	const CharStyle& charStyle(int pos) const;
 	const ParagraphStyle& paragraphStyle(int pos) const;
 	const ParagraphStyle& defaultStyle() const;
 	void setDefaultStyle(const ParagraphStyle& style);
 	void setCharStyle(int pos, uint len, const CharStyle& style);
 	void setStyle(int pos, const ParagraphStyle& style);
 	void applyCharStyle(int pos, uint len, const CharStyle& style);
 	void applyStyle(int pos, const ParagraphStyle& style);
 	void eraseCharStyle(int pos, uint len, const CharStyle& style);
 	void eraseStyle(int pos, const ParagraphStyle& style);
	void replaceStyles(QMap<QString,QString> newNameForOld);
	void replaceCharStyles(QMap<QString,QString> newNameForOld);
	
	void getNamedResources(ResourceCollection& lists) const;
	void replaceNamedResources(ResourceCollection& newNames);
	
 	uint nrOfParagraphs() const;
 	int startOfParagraph(uint index) const;
 	int endOfParagraph(uint index) const;
	int nrOfParagraph(uint index) const;

 	uint nrOfRuns() const;
 	int startOfRun(uint index) const;
 	int endOfRun(uint index) const;

// positioning
	int nextChar(int pos);
	int prevChar(int pos);
	int nextWord(int pos);
	int prevWord(int pos);
	int nextSentence(int pos);
	int prevSentence(int pos);
	int nextParagraph(int pos);
	int prevParagraph(int pos);

// these need valid layout:

	int startOfLine(int pos);
	int endOfLine(int pos);
	int prevLine(int pos);
	int nextLine(int pos);
	int startOfFrame(int pos);
	int endOfFrame(int pos);

// selection

 	void selectAll();
 	void deselectAll();
 	void removeSelection();
 	void extendSelection(int oldPos, int newPos);
	int selectWord(int pos);
	void select(int pos, uint len, bool on = true);
 	bool selected(int pos) const;
	int startOfSelection() const;
	int endOfSelection() const;
	int lengthOfSelection() const;


//  when physical view doesn't match logical view any more:

	/// call this if the shape of an embedded object changes (redos layout)
 	void invalidateObject(const PageItem* embedded);
 	/// call this if the shape of the paragraph changes (redos layout)
 	void invalidateLayout();

public slots:
	/// call this if some logical style changes (redos shaping and layout)
 	void invalidateAll();

	signals:
		void changed();

public:
// physical view

	bool overflows() const;
	/// layouts the text -- FIXME: better interface for this
 	int layout(int startItem);
 	uint nrOfItems() const;
//private:
 	ScText * item(uint index);
 	const ScText * item(uint index) const;
public:
	ScText * item_p(uint index) { return item(index); }
//	void bidiReorder(uint firstItem, uint lastItem, uint indices[]) const;
  	/** returns the Unicode string which belongs to this ScScriptItem */
 	const QString itemText(uint index) const;
 	/** returns the CharStyle which belongs to this ScScriptItem */
 	const CharStyle itemStyle(uint index) const;
 	/// returns the character postion at the start of this item
 	int startOfItem(uint index) const;
 	/// returns the character position after this item
 	int endOfItem(uint index) const;
// 	const ScTextEngine * engineForRun(uint index) const;
// 	const ScTextEngine * engineForItem(uint index) const;

// 	ParagraphLayout layouter;

 	int screenToPosition(FPoint coord) const;
 	FRect  boundingBox(int pos, uint len = 1) const;

	uint lines() const { return (uint) m_lines.count(); }
	
	LineSpec line(uint i) const { return m_lines[i]; }
	
	void appendLine(const LineSpec& ls) 
	{ 
		assert( ls.firstItem >= 0 );
		assert( ls.firstItem < length() );
		assert( ls.lastItem >= 0 && ls.firstItem - ls.lastItem < 1 );
		assert( ls.lastItem < length() );
		m_lines.append(ls);
		if (lastFrameItem < firstFrameItem) {
			firstFrameItem = ls.firstItem;
			lastFrameItem = ls.lastItem;
		}
		else {
			firstFrameItem = qMin(firstFrameItem, ls.firstItem);
			lastFrameItem = qMax(lastFrameItem, ls.lastItem);
		}
	}
	
	void clearLines() 
	{ 
		m_lines.clear(); 
		firstFrameItem = 0; 
		lastFrameItem = -1; 
	}
	
	int firstInFrame() { return firstFrameItem; }
	int lastInFrame() { return lastFrameItem; }

private:
	ScribusDoc * doc; 
	int selFirst, selLast;
	int firstFrameItem, lastFrameItem;
	QList<LineSpec> m_lines;
	bool m_validLayout;
	double m_magicX;
	int m_lastMagicPos;

	QString textWithSmartHyphens (int pos, uint len) const;
	void    insertCharsWithSmartHyphens(int pos, QString txt, bool applyNeighbourStyle = false);
	
 	/// mark these runs as invalid, ie. need itemize and shaping
 	void invalidate(int firstRun, int lastRun);
 	void removeParSep(int pos);
 	void insertParSep(int pos);

	// 	int splitRun(int pos);
 	
 	/** bring physical view in sync with logical one. 
 	 *  This gets called automatically from all physical view methods
 	 */
// 	void validate();
 	/// private data structure
 	ScText_Shared * d;
 	/// gives the physical view which was last given to the layouter
// 	uint layouterVersion;
 	/// is true after layout() has been exercised
// 	bool layouterValid;
 };


#endif /*STORYTEXT_H_*/
