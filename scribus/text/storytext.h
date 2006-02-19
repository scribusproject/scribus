#ifndef STORYTEXT_H_
#define STORYTEXT_H_

#include <qstring.h>
#include <qvaluevector.h>
#include <qptrvector.h>
#include <qptrlist.h>

//#include "text/paragraphlayout.h"
//#include "text/frect.h"

#ifdef NLS_CONFORMANCE
#define NLS_PRIVATE private
#else
#define NLS_PRIVATE public
#endif

class SpecialChars {
public:
	static QChar OBJECT;
	static QChar TAB;
	static QChar PARSEP;
	static QChar LINEBREAK;
	static QChar COLBREAK;
	static QChar FRAMEBREAK;
	static QChar SHYPHEN;
	static QChar NBHYPHEN;
	static QChar NBSPACE;
	static QChar ZWNBSPACE;
	static QChar ZWSPACE;
//	static QChar SPACE;
};

class CharStyle;
class ParagraphStyle;
class PageItem;
//class ScTextEngine;
//class ScScriptItem;
//class ScText_Shared;
class ScribusDoc;

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
 class StoryText : NLS_PRIVATE QPtrList<ScText> {
 public:
 	StoryText(ScribusDoc * doc);
 	StoryText(const StoryText & other);
 	StoryText& operator= (const StoryText & other);
 	virtual ~StoryText();
 	void clear();
 	void removeChars(int pos, uint len);
 	void insertChars(int pos, QString txt);
 	void insertObject(int pos, PageItem* obj);
 	void replaceChar(int pos, QChar ch);

 	int length() const;
 	QChar text(int pos) const;
 	QString text(int pos, uint len) const;
 	PageItem* object(int pos) const;

 	const CharStyle& charStyle(int pos) const;
 	const struct ParagraphStyle& paragraphStyle(int pos) const;
 	void applyStyle(int pos, uint len, const CharStyle& style);
 	void applyStyle(int pos, const ParagraphStyle& style);

 	uint nrOfParagraphs() const;
 	int startOfParagraph(uint index) const;
 	int endOfParagraph(uint index) const;

 	uint nrOfRuns() const;
 	int startOfRun(uint index) const;
 	int endOfRun(uint index) const;

// selection

 	void selectAll();
 	void deselectAll();
 	void removeSelection();
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
 	/// call this if some logical style changes (redos shaping and layout)
 	void invalidateAll();




// physical view

	/// layouts the text -- FIXME: better interface for this
 	int layout(int startItem);
 	uint nrOfItems() const;
 	ScText * item(uint index);
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
 	FPoint  boundingBox(int pos, uint len = 1) const;

 private:
	ScribusDoc * doc; 
	int selFirst, selLast;
	int firstFrameItem, lastFrameItem;

 	/// mark these runs as invalid, ie. need itemize and shaping
 	void invalidate(int firstRun, int lastRun);
 	
// 	int splitRun(int pos);
 	
 	/** bring physical view in sync with logical one. 
 	 *  This gets called automatically from all physical view methods
 	 */
// 	void validate();
 	/// private data structure
// 	ScText_Shared * d;
 	/// gives the physical view which was last given to the layouter
// 	uint layouterVersion;
 	/// is true after layout() has been exercised
// 	bool layouterValid;
 };
#endif /*STORYTEXT_H_*/
