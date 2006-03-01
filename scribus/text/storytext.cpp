//FIXME: this include must go to sctextstruct.h !
#include <qvaluelist.h>

#include "fpoint.h"
#include "sctextstruct.h"
#include "storytext.h"
#include "scribus.h"
#include "scfonts.h"


//FIXME: move to proper place
const QString CharStyle::NOCOLOR; // cf. sctextstruct.h


QChar SpecialChars::OBJECT     = QChar(25);
QChar SpecialChars::TAB        = QChar(9);
QChar SpecialChars::PARSEP     = QChar(13);
QChar SpecialChars::LINEBREAK  = QChar(28);
QChar SpecialChars::COLBREAK   = QChar(26);
QChar SpecialChars::FRAMEBREAK = QChar(27);
QChar SpecialChars::SHYPHEN;
QChar SpecialChars::NBHYPHEN   = QChar(24);
QChar SpecialChars::NBSPACE    = QChar(29);
QChar SpecialChars::ZWNBSPACE;
QChar SpecialChars::ZWSPACE;
//QChar SpecialChars::SPACE      = QChar(32);



StoryText::StoryText(ScribusDoc * doc_) : doc(doc_)
{
	clear();
}

StoryText::StoryText(const StoryText & other) : QPtrList<ScText>(other), doc(other.doc)
{
	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;
	
	invalidateLayout();
}

StoryText::~StoryText()
{
}

StoryText& StoryText::operator= (const StoryText & other)
{
	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;

	invalidateLayout();
	return *this;
}


void StoryText::clear()
{
	selFirst = 0;
	selLast = -1;

	firstFrameItem = 0;
	lastFrameItem = -1;
	
	for (ScText *it = first(); it != 0; it = next())
	{
		if ((it->ch == SpecialChars::OBJECT) && (it->cembedded != 0)) {
			doc->FrameItems.remove(it->cembedded);
			it->cembedded = 0;
		}
	}
	
	QPtrList<ScText>::clear();
	invalidateAll();
}


void StoryText::removeChars(int pos, uint len)
{
	if (pos < 0)
		pos += length();

	assert( len > 0 );
	assert( pos >= 0 );
	assert( pos + static_cast<int>(len) <= length() );

	for ( uint i=0; i < len; ++i )
	{
		ScText *it = take(i);
		if ((it->ch == SpecialChars::OBJECT) && (it->cembedded != 0)) {
			doc->FrameItems.remove(it->cembedded);
			it->cembedded = 0;
		}
		delete it;
	}
	invalidate(pos, -1);
}

void StoryText::insertChars(int pos, QString txt) //, const CharStyle&
{
	if (pos < 0)
		pos += length();

	assert(txt.length() > 0);
	assert(pos >= 0);
	assert(pos <= length());
	
	const CharStyle style = charStyle(pos);
	
	for (uint i = 0; i < txt.length(); ++i) {
		ScText * item = new ScText();
		item->ch= txt.mid(i, 1);
		*static_cast<CharStyle *>(item) = style;
		insert(pos + i, item);
	}

	invalidate(pos, pos + txt.length());
}

void StoryText::replaceChar(int pos, QChar ch)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	at(pos)->ch = ch;

	invalidate(pos, pos + 1);
}

void StoryText::insertObject(int pos, PageItem* ob)
{
	if (pos < 0)
		pos += length();

	insertChars(pos, SpecialChars::OBJECT);
	//FIXME:NLS
}


int StoryText::length() const
{
	return count();
}

QChar StoryText::text(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	return const_cast<StoryText *>(this)->at(pos)->ch[0];
}

QString StoryText::text(int pos, uint len) const
{
	if (pos < 0)
		pos += length();

	assert(len > 0);
	assert(pos >= 0);
	assert(pos + signed(len) <= length());

	QString result;
	for (uint i = pos; i < pos+signed(len); ++i)
		result += const_cast<StoryText *>(this)->at(i)->ch;

	return result;
}

PageItem* StoryText::object(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);	
	if (that->at(pos)->cembedded != NULL)
		return that->at(pos)->cembedded;
	else
		return NULL;
}


const CharStyle & StoryText::charStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);
	return dynamic_cast<const CharStyle &> (*that->at(pos));
}

const ParagraphStyle & StoryText::paragraphStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	//FIXME:NLS
	static ParagraphStyle dummy = ParagraphStyle();
	return dummy;
}

void StoryText::applyStyle(int pos, uint len, const CharStyle& style )
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos + signed(len) <= length());

	if (len == 0)
		return;

	for (uint i=pos; i < pos+len; ++i)
		at(i)->applyStyle(style);

	invalidate(pos, pos + len);
}

void StoryText::applyStyle(int pos, const ParagraphStyle& style)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	//FIXME:NLS
}

uint StoryText::nrOfParagraphs() const
{
	uint result = 0;
        StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i)
		if (that->at(i)->ch == SpecialChars::PARSEP)
			++result;
	return result;
}

int StoryText::startOfParagraph(uint index) const
{
	if (index == 0)
		return 0;

        StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i)
		if (that->at(i)->ch == SpecialChars::PARSEP && ! --index)
			return i + 1;
	return -1;
}

int StoryText::endOfParagraph(uint index) const
{
	++index;
        StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i)
		if (that->at(i)->ch == SpecialChars::PARSEP && ! --index)
			return i + 1;
	return length();
}

uint StoryText::nrOfRuns() const
{
	return 1;
}

int StoryText::startOfRun(uint index) const
{
	return 0;
}

int StoryText::endOfRun(uint index) const
{
	return length();
}


// selection

int StoryText::startOfSelection() const
{
	return selFirst <= selLast? selFirst : 0;
}

int StoryText::endOfSelection() const
{
	return selFirst <= selLast? selLast + 1 : -1;
}

int StoryText::lengthOfSelection() const
{
	return selFirst <= selLast? selLast - selFirst + 1 : 0;
}


bool StoryText::selected(int pos) const
{
	return (selFirst <= pos && pos <= selLast) 
	       || (pos >= 0 && pos < length() && const_cast<StoryText*>(this)->at(pos)->cselect);
}

void StoryText::select(int pos, uint len, bool on)
{
	if (pos < 0)
		pos += length();

	assert( pos >= 0 );
	assert( pos + signed(len) <= length() );

        StoryText* that = const_cast<StoryText *>(this);
	for (int i=pos; i < pos+signed(len); ++i)
		that->at(i)->cselect = on;

	if (on) {
		// extend if possible
		if (selected(pos - 1))
			selLast = QMAX(selLast, pos + static_cast<int>(len) - 1);
		else if (selected(pos + len))
			selFirst = QMIN(selFirst, pos);
		else {
			selFirst = pos;
			selLast = pos + len - 1;
		}
	}
	else {
		if (pos <= selFirst && selLast < pos + signed(len))
			deselectAll();
		// shrink
		else if (!selected(pos - 1) && selected(pos + len - 1))
			selFirst = pos + len;
		else if (selected(pos) && !selected(pos + len))
			selLast = pos - 1;
		else if (selected(pos) || selected(pos + len - 1))
			// Grr, deselection splits selection
			selLast = pos - 1;
	}
}

void StoryText::selectAll()
{
        StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i)
		that->at(i)->cselect = true;

	selFirst = 0;
	selLast = length() - 1;
}

void StoryText::deselectAll()
{
        StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i)
		that->at(i)->cselect = false;

	selFirst = 0;
	selLast = -1;
}

void StoryText::removeSelection()
{
	if (selFirst > selLast)
		return;

	assert( selFirst >= 0 );
	assert( selLast < length() );

	removeChars(selFirst, selLast - selFirst+1);
	deselectAll();
}



void StoryText::invalidateObject(const PageItem * embedded)
{
}

void StoryText::invalidateLayout()
{
}

void StoryText::invalidateAll()
{
}

void StoryText::invalidate(int firstitem, int lastitem)
{
}


// physical view


/*
void StoryText::validate()
{
	static bool withinValidate = false;
	
	assert( !withinValidate );
	withinValidate = true;
	
	withinValidate = false;
}
*/

int StoryText::screenToPosition(FPoint coord) const
{
//FIXME:NLS	
	//FIXME: should be end of lastFrameItem
	return length();
}


FPoint StoryText::boundingBox(int pos, uint len) const
{
	return FPoint(); //FIXME
}


int StoryText::layout(int startItem)
{
	//FIXME:NLS
	return -1;
}


uint StoryText::nrOfItems() const
{
	return length();
}


ScText*  StoryText::item(uint itm)
{
	assert( static_cast<int>(itm) < length() );
	return const_cast<StoryText *>(this)->at(itm);
}

const QString StoryText::itemText(uint itm) const
{
	
	assert( static_cast<int>(itm) < length() );

	return text(itm, 1);
}


const CharStyle StoryText::itemStyle(uint itm) const
{
	assert( static_cast<int>(itm) < length() );

	return charStyle(itm);
}
	

int StoryText::startOfItem(uint itm) const
{
	assert( static_cast<int>(itm) < length() );

	return itm;
}

int StoryText::endOfItem(uint itm) const
{
	assert( static_cast<int>(itm) < length() );

	return itm + 1;
}

