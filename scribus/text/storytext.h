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

#include <cassert>
#include <QObject>
#include <QString>
#include <QList>
#include <unicode/uversion.h>

#include "itextsource.h"
#include "marks.h"
#include "text/frect.h"
#include "text/specialchars.h"
#include "sctextstruct.h"
#include "style.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "desaxe/saxio.h"

class CharStyle;
class ParagraphStyle;
class PageItem;
class ScribusDoc;
class ScText_Shared;
class ResourceCollection;
class ShapedTextCache;

U_NAMESPACE_BEGIN
class BreakIterator;
U_NAMESPACE_END

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
class SCRIBUS_API StoryText : public QObject, public SaxIO, public ITextSource
{
	Q_OBJECT
	
public:
	StoryText(ScribusDoc *doc);
	StoryText();
	StoryText(const StoryText & other);
	StoryText& operator= (const StoryText & other);
	virtual ~StoryText();

	bool hasBulletOrNum() const;
	bool hasTextMarks() const;
	bool marksCountChanged() const;
	void resetMarksCountChanged();
	
	void setDoc(ScribusDoc *docin);
	
	static const Xml_string saxxDefaultElem;
	static void  desaxeRules(const Xml_string& prefixPattern, desaxe::Digester& ruleset, Xml_string elemtag = saxxDefaultElem);
	
	virtual void saxx(SaxHandler& handler, const Xml_string& elemtag) const;
	virtual void saxx(SaxHandler& handler)                     const { saxx(handler, saxxDefaultElem); }
	
	int  cursorPosition() const;
	void setCursorPosition(int pos, bool relative = false);
	void normalizeCursorPosition();
	int  normalizedCursorPosition();

	void moveCursorForward();
	void moveCursorBackward();
	void moveCursorLeft();
	void moveCursorRight();

	void moveCursorWordLeft();
	void moveCursorWordRight();

	void clear();
	StoryText copy() const;

	// Find text in story
	int indexOf(const QString &str, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive, int* pLen = 0) const;
	int indexOf(QChar ch, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
	
	// Add, change, replace
	// Insert chars from another StoryText object at current cursor position
	void insert(const StoryText& other, bool onlySelection = false);
	// Insert chars from another StoryText object at specific position
	void insert(int pos, const StoryText& other, bool onlySelection = false);
	// Append chars from another StoryText object
	void append(const StoryText& other) { insert(length(), other, false); }
	// Remove len chars at specific position
 	void removeChars(int pos, uint len);
	// Removes trailing empty paragraphs
	void trim();
	// Insert chars at current cursor position
	void insertChars(const QString& txt, bool applyNeighbourStyle = false);
	// Insert chars ar specific position
	void insertChars(int pos, const QString& txt, bool applyNeighbourStyle = false);
	// Insert inline object at current cursor position
	void insertObject(int obj);
	// Insert object at specific position
	void insertObject(int pos, int obj);
	// Insert mark at cursor or specific position
	void insertMark(Mark* mark, int pos = -1);
	// Replace a character
 	void replaceChar(int pos, QChar ch);
	// Replace current selection with specified text
	void replaceSelection(const QString& newText);
 	// Replaced a word, and return the difference in length between old and new
	int replaceWord(int pos, QString newWord);
	void replaceObject(int pos, int obj);

	void hyphenateWord(int pos, uint len, const char* hyphens);
	
 	// Retrieve length of story text
 	int length() const;

	// Get content at specific position as plain text
	// Internal paragraph separator are converted to 
	// unix new lines for better compatibility with
	// text editors
	QString plainText() const;

	// TextSource methods 

	virtual bool isBlockStart(int pos) const; 
	virtual int nextBlockStart(int pos) const;
	virtual InlineFrame object(int pos) const;
	virtual bool hasExpansionPoint(int pos) const;
	virtual ExpansionPoint expansionPoint(int pos) const;

	// Get char at current cursor position
//	QChar   text() const;
	// Get char at specific position
 	QChar   text(int pos) const;
	// Get text with len chars at specific position
	QString text(int pos, uint len) const;
 	//Get sentence at any position within it
	QString sentence(int pos, int &posn);

	bool hasObject(int pos) const;
	PageItem* getItem(int pos) const; // deprecated
	int  findMark(const Mark* mrk, int startPos = 0) const;
	bool hasMark(int pos, const Mark* mrk = nullptr) const;
	Mark *mark(int pos) const;
	void replaceMark(int pos, Mark* mrk);
	void applyMarkCharstyle(Mark* mrk, CharStyle& currStyle) const;

	bool isHighSurrogate(int pos) const;
	bool isLowSurrogate(int pos) const;

	// Get charstyle at current cursor position
	const CharStyle& charStyle() const;
	// Get charstyle at specific position
 	const CharStyle& charStyle(int pos) const;
	// Get paragraph style at current cursor position
	const ParagraphStyle& paragraphStyle() const;
	// Get paragraph style at specific position
	const ParagraphStyle& paragraphStyle(int pos) const;
	const ParagraphStyle& defaultStyle() const;
	void setDefaultStyle(const ParagraphStyle& style);
	void setCharStyle(int pos, uint len, const CharStyle& style);
	void setStyle(int pos, const ParagraphStyle& style);
	void applyCharStyle(int pos, uint len, const CharStyle& style);
	void applyStyle(int pos, const ParagraphStyle& style, bool rmDirectFormatting = false);
	void eraseCharStyle(int pos, uint len, const CharStyle& style);
	void eraseStyle(int pos, const ParagraphStyle& style);
	void replaceStyles(const QMap<QString,QString>& newNameForOld);
	void replaceCharStyles(QMap<QString,QString> newNameForOld);

	// Cleanup legacy formatting for whole story, ie remove direct
	// formatting for parameters already set at paragraph level
	void fixLegacyFormatting();
	
	// Cleanup legacy formatting for paragraph at position pos
	void fixLegacyFormatting(int pos);
	
	void getNamedResources(ResourceCollection& lists) const;
	void replaceNamedResources(ResourceCollection& newNames);
	
	uint nrOfParagraphs() const;
	int startOfParagraph() const;
	int startOfParagraph(uint index) const;
	int endOfParagraph() const;
	int endOfParagraph(uint index) const;
	uint nrOfParagraph() const;
	uint nrOfParagraph(int pos) const;

	uint nrOfRuns() const;
	int startOfRun(uint index) const;
	int endOfRun(uint index) const;

// positioning
	int nextChar(int pos);
	int prevChar(int pos);
	int firstWord();
	int nextWord(int pos);
	int prevWord(int pos);
	int endOfWord(int pos) const;
	int nextSentence(int pos);
	int prevSentence(int pos);
	int endOfSentence(int pos) const;
	int nextParagraph(int pos);
	int prevParagraph(int pos);

// these need valid layout:

//	int startOfLine(int pos);
//	int endOfLine(int pos);
//	int prevLine(int pos);
//	int nextLine(int pos);
//	int startOfFrame(int pos);
//	int endOfFrame(int pos);

// selection

	void selectAll();
	void deselectAll();
	void removeSelection();
	void extendSelection(int oldPos, int newPos);
	int selectWord(int pos);
	void select(int pos, int len, bool on = true);
	QString selectedText() const;
	bool selected(int pos) const;
	int startOfSelection() const;
	int endOfSelection() const;
	int selectionLength() const;
	bool isSelected() const;
	
	// break iterators
	static icu::BreakIterator* getGraphemeIterator();
	static icu::BreakIterator* getWordIterator();
	static icu::BreakIterator* getSentenceIterator();
	static icu::BreakIterator* getLineIterator();

// layout helpers

	ShapedTextCache* shapedTextCache() { return m_shapedTextCache; }

	LayoutFlags flags(int pos) const;
	bool hasFlag(int pos, LayoutFlags flag) const;
	void setFlag(int pos, LayoutFlags flag);
	void clearFlag(int pos, LayoutFlags flag);

//  when physical view doesn't match logical view any more:

	/// call this if the shape of an embedded object changes (redos layout)
	void invalidateObject(const PageItem* embedded);
	/// call this if the shape of the paragraph changes (redos layout)
	void invalidateLayout();

public slots:
	/// call this if some logical style changes (redos shaping and layout)
	void invalidateAll();

signals:
	void changed(int firstItem, int endItem);

private:
	ScText * item(int index);
	const ScText * item(int index) const;
	void fixSurrogateSelection();
	
private:
	ScribusDoc * m_doc;
	ShapedTextCache* m_shapedTextCache;
	static icu::BreakIterator* m_graphemeIterator;
	static icu::BreakIterator* m_wordIterator;
	static icu::BreakIterator* m_sentenceIterator;
	static icu::BreakIterator* m_lineIterator;

	QString textWithSoftHyphens (int pos, uint len) const;
	void    insertCharsWithSoftHyphens(int pos, const QString& txt, bool applyNeighbourStyle = false);
	
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
