//FIXME: this include must go to sctextstruct.h !
#include <qvaluelist.h>
#include <cassert>  //added to make Fedora-5 happy
#include "fpoint.h"
#include "sctextstruct.h"
#include "storytext.h"
#include "scribus.h"
#include "scfonts.h"
#include "util.h"


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

class ScText_Shared : public QPtrList<ScText>
{
public:
	ParagraphStyle defaultStyle;
	uint refs;
	uint len;
	ScText_Shared() : QPtrList<ScText>(), defaultStyle(), refs(1), len(0)
	{ 
		setAutoDelete(true); 
	}
};


StoryText::StoryText(ScribusDoc * doc_) : doc(doc_)
{
	d = new ScText_Shared();
	clear();
}

StoryText::StoryText(const StoryText & other) : doc(other.doc)
{
	d = other.d;
	d->refs++;
	
	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;

	invalidateLayout();
}

StoryText::~StoryText()
{
	d->refs--;
	if (d->refs == 0) {
		for (ScText *it = d->first(); it != 0; it = d->next())
		{
			if ((it->ch[0] == SpecialChars::OBJECT) && (it->cembedded != 0)) {
				doc->FrameItems.remove(it->cembedded);
				delete it->cembedded;
				it->cembedded = 0;
			}
			else if ((it->ch[0] == SpecialChars::PARSEP)) {
				delete it->parstyle;
				it->parstyle = 0;
			}
		}
		
		d->clear();
		d->len = 0;
		delete d;
	}	
}

StoryText& StoryText::operator= (const StoryText & other)
{
	d->refs--;
	if (d->refs == 0) {
		clear();
		delete d;
	}
	
	doc = other.doc; 
	d = other.d;
	d->refs++;
	
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
	
	d->defaultStyle = ParagraphStyle();
	d->defaultStyle.setParent( & doc->docParagraphStyles[0]);
	
	for (ScText *it = d->first(); it != 0; it = d->next())
	{
		if ((it->ch[0] == SpecialChars::OBJECT) && (it->cembedded != 0)) {
			doc->FrameItems.remove(it->cembedded);
			delete it->cembedded;
			it->cembedded = 0;
		}
		else if ((it->ch[0] == SpecialChars::PARSEP)) {
			delete it->parstyle;
			it->parstyle = 0;
		}
	}
	
	d->clear();
	d->len = 0;
	invalidateAll();
}


void StoryText::append(const StoryText& other)
{
	CharStyle cstyle(charStyle(length()));
	ParagraphStyle pstyle(paragraphStyle(length()));
	int cstyleStart = length();
	for (int i=0; i < other.length(); ++i) {
		if (other.charStyle(i) == cstyle)
			continue;
		int pos = length();
		int len = i - cstyleStart;
		if (len > 0) {
			insertChars(length(), other.text(cstyleStart, len));
			applyCharStyle(pos, len, cstyle);
		}
		if (other.text(i) == SpecialChars::PARSEP) {
			insertChars(pos+len, SpecialChars::PARSEP);
			applyStyle(pos+len, other.paragraphStyle(i));
			cstyleStart = i+1;
		}
		else {
			cstyle = other.charStyle(i);
			cstyleStart = i;
		}
	}
	int pos = length();
	int len = other.length() - cstyleStart;
	if (len > 0) {
		insertChars(length(), other.text(cstyleStart, len));
		applyCharStyle(pos, len, cstyle);
	}
	setDefaultStyle(other.defaultStyle());
}



/**
     A CharStyle's parent is usually the default paragraphstyle, unless explicitly
     changed. This routines makes sure that all parent pointers to the default
     paragraphStyle are up-to-date
 */
static void replaceParentStyle(StoryText* that, int pos, const Style* oldP, const Style* newP)
{
	for(int i = pos-1; i >= 0; ++i) {
		ScText* it = that->item(i);
		if (it->ch[0] == SpecialChars::PARSEP)
			break;
		else if (it->parent() == NULL || it->parent() == oldP)
			it->setParent(newP);
	}	
}


/**
    Make sure that the paragraph CharStyle's point to the new ParagraphStyle
 */
static void insertParSep(StoryText* that, int pos)
{
	ScText* it = that->item(pos);
	const ParagraphStyle* oldP = & that->paragraphStyle(pos);
	ParagraphStyle* newP = new ParagraphStyle(*oldP);
	it->parstyle = newP;
	replaceParentStyle(that, pos, oldP, newP);
}
/**
     need to remove the ParagraphStyle structure and replace all pointers
     to it...
 */
static void removeParSep(StoryText* that, int pos)
{
	ScText* it = that->item(pos);
	if (it->parstyle) {
		const ParagraphStyle* oldP = it->parstyle;
		const ParagraphStyle* newP = & that->paragraphStyle(pos+1);
		replaceParentStyle(that, pos, oldP, newP);
		delete oldP;
	}
}

void StoryText::removeChars(int pos, uint len)
{
	if (pos < 0)
		pos += length();

	assert( len > 0 );
	assert( pos >= 0 );
	assert( pos + static_cast<int>(len) <= length() );

	for ( int i=pos + static_cast<int>(len) - 1; i >= pos; --i )
	{
		ScText *it = d->at(i);
		if ((it->ch[0] == SpecialChars::OBJECT) && (it->cembedded != 0)) {
			doc->FrameItems.remove(it->cembedded);
			it->cembedded = 0;
		}
		else if ((it->ch[0] == SpecialChars::PARSEP)) {
			removeParSep(this, i);
		}
//		qDebug("remove char %d at %d", it->ch[0].unicode(), i);
		d->take(i);
		d->len--;
		delete it;
	}

	d->len = d->count();
	invalidate(pos, -1);
}

void StoryText::insertChars(int pos, QString txt) //, const CharStyle&
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());
	
	if (txt.length() == 0)
		return;
	
	const CharStyle style = pos == 0 ? defaultStyle().charStyle() : charStyle(pos - 1);
	assert(style.font() != NULL);
	
//	const int paraStyle = pos == 0 ? 0 : at(pos - 1)->cab;
	
	for (uint i = 0; i < txt.length(); ++i) {
		ScText * item = new ScText();
		item->ch= txt.mid(i, 1);
		*static_cast<CharStyle *>(item) = style;
		d->insert(pos + i, item);
		d->len++;
		if (item->ch[0] == SpecialChars::PARSEP) {
//			qDebug(QString("new PARSEP %2 at %1").arg(pos).arg(paragraphStyle(pos).name()));
			insertParSep(this, pos + i);
		}
	}

	d->len = d->count();
	invalidate(pos, pos + txt.length());
}

void StoryText::replaceChar(int pos, QChar ch)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	ScText* item = d->at(pos);
	if (item->ch[0] == ch)
		return;
	
	if (d->at(pos)->ch[0] == SpecialChars::PARSEP) {
		removeParSep(this, pos);
	}
	item->ch = ch;
	if (d->at(pos)->ch[0] == SpecialChars::PARSEP) {
		insertParSep(this, pos);
	}
	
	invalidate(pos, pos + 1);
}

void StoryText::hyphenateWord(int pos, uint len, char* hyphens)
{
	assert(pos >= 0);
	assert(pos + signed(len) <= length());
	
	for (int i=pos; i < pos+signed(len); ++i)
		if(hyphens && hyphens[i] & 1)
			d->at(i)->setEffects(d->at(i)->effects() | ScStyle_HyphenationPossible);
		else
			d->at(i)->setEffects(d->at(i)->effects() & ~ScStyle_HyphenationPossible);

	invalidate(pos, pos + len);
}

void StoryText::insertObject(int pos, PageItem* ob)
{
	if (pos < 0)
		pos += length();

	insertChars(pos, SpecialChars::OBJECT);
	const_cast<StoryText *>(this)->d->at(pos)->cembedded = ob;
}


int StoryText::length() const
{
	return d->len;
}

QChar StoryText::text(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	return const_cast<StoryText *>(this)->d->at(pos)->ch[0];
}

QString StoryText::text(int pos, uint len) const
{
	if (pos < 0)
		pos += length();

	assert(len > 0);
	assert(pos >= 0);
	assert(pos + signed(len) <= length());

	QString result;
	StoryText* that(const_cast<StoryText*>(this));
	that->d->at(pos);
	for (int i = pos; i < pos+signed(len); ++i) {
		result += that->d->current()->ch;
		that->d->next();
	}

	return result;
}

PageItem* StoryText::object(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);	
	if (that->d->at(pos)->cembedded != NULL)
		return that->d->at(pos)->cembedded;
	else
		return NULL;
}


const CharStyle & StoryText::charStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	if (length() == 0)
		return defaultStyle().charStyle();
	else if (pos == length())
		--pos;
	
	StoryText* that = const_cast<StoryText *>(this);
	return dynamic_cast<const CharStyle &> (*that->d->at(pos));
}

const ParagraphStyle & StoryText::paragraphStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	StoryText * that = const_cast<StoryText *> (this);
//	assert( that->at(pos)->cab >= 0 );
//	assert( that->at(pos)->cab < doc->docParagraphStyles.count() );
//	return doc->docParagraphStyles[that->at(pos)->cab];
	
	that->d->at(pos);
	while (pos < length() && that->d->current()->ch[0] != SpecialChars::PARSEP) {
		++pos;
		that->d->next();
	}
	if (pos == length())
		return defaultStyle();
	else if ( !that->d->current()->parstyle ) {
		qDebug(QString("using default parstyle at %1").arg(pos));
		that->d->current()->parstyle = new ParagraphStyle(defaultStyle());
	}
	return *that->d->current()->parstyle;
}

const ParagraphStyle& StoryText::defaultStyle() const
{
	return d->defaultStyle;
}


void StoryText::setDefaultStyle(const ParagraphStyle& style)
{
	d->defaultStyle = style;
	if (!d->defaultStyle.parent())
		d->defaultStyle.setParent( & doc->docParagraphStyles[0] );
}



void StoryText::applyCharStyle(int pos, uint len, const CharStyle& style )
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos + signed(len) <= length());

	if (len == 0)
		return;

	d->at(pos);
	for (uint i=pos; i < pos+len; ++i) {
		d->current()->applyCharStyle(style);
		d->next();
	}

	invalidate(pos, pos + len);
}



void StoryText::eraseCharStyle(int pos, uint len, const CharStyle& style )
{
	if (pos < 0)
		pos += length();
	
	assert(pos >= 0);
	assert(pos + signed(len) <= length());
	
	if (len == 0)
		return;
	
	d->at(pos);
	for (uint i=pos; i < pos+len; ++i) {
		d->current()->eraseCharStyle(style);
		d->next();
	}
	
	invalidate(pos, pos + len);
}


void StoryText::applyStyle(int pos, const ParagraphStyle& style)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

/*	int paraStyle = QMAX(0, findParagraphStyle(doc, style)); //FIXME:NLS
	int i = pos - 1;
	if (length() > 0) {
		// find start of para
		while (i >= 0 && at(i)->ch[0] != SpecialChars::PARSEP) {
			--i;
		}
		// set 'cab' field
		do {
			++i;
			at(i)->cab = paraStyle;
		}
		while (i+1 < length() && d->at(i)->ch[0] != SpecialChars::PARSEP);
	} 
	++i; */
	
	int i = pos;
	while (i < length() && d->at(i)->ch[0] != SpecialChars::PARSEP) {
		++i;
	}
	if (i < length()) {
		if (!d->at(i)->parstyle) {
			qDebug(QString("PARSEP without style at pos %1").arg(i));
			d->at(i)->parstyle = new ParagraphStyle(defaultStyle());
		}
//		qDebug(QString("applying parstyle %2 at %1 for %3").arg(i).arg(paragraphStyle(pos).name()).arg(pos));
		d->at(i)->parstyle->applyStyle(style);
		if (!d->at(i)->parstyle->parent()) {
			d->at(i)->parstyle->setParent( & d->defaultStyle );
		}
	}
	else {
		// not happy about this but inserting a new PARSEP makes more trouble
//		qDebug(QString("applying parstyle %1 as defaultstyle for %2").arg(paragraphStyle(pos).name()).arg(pos));
		d->defaultStyle.applyStyle(style);
	}
}

void StoryText::eraseStyle(int pos, const ParagraphStyle& style)
{
	if (pos < 0)
		pos += length();
	
	assert(pos >= 0);
	assert(pos <= length());
		
	int i = pos;
	while (i < length() && d->at(i)->ch[0] != SpecialChars::PARSEP) {
		++i;
	}
	if (i < length()) {
		if (!d->at(i)->parstyle) {
			qDebug(QString("PARSEP without style at pos %1").arg(i));
			d->at(i)->parstyle = new ParagraphStyle(defaultStyle());
		}
		//		qDebug(QString("applying parstyle %2 at %1 for %3").arg(i).arg(paragraphStyle(pos).name()).arg(pos));
		d->at(i)->parstyle->eraseStyle(style);
		if (!d->at(i)->parstyle->parent()) {
			d->at(i)->parstyle->setParent( & d->defaultStyle );
		}
	}
	else {
		// not happy about this but inserting a new PARSEP makes more trouble
		//		qDebug(QString("applying parstyle %1 as defaultstyle for %2").arg(paragraphStyle(pos).name()).arg(pos));
		d->defaultStyle.eraseStyle(style);
	}
}


uint StoryText::nrOfParagraphs() const
{
	uint result = 0;
	StoryText* that = const_cast<StoryText *>(this);
	that->d->at(0);
	for (int i=0; i < length(); ++i) {
		if (that->d->current()->ch[0] == SpecialChars::PARSEP)
			++result;
		that->d->next();
	}
	return result;
}

int StoryText::startOfParagraph(uint index) const
{
	if (index == 0)
		return 0;

	StoryText* that = const_cast<StoryText *>(this);
	that->d->at(0);
	for (int i=0; i < length(); ++i) {
		if (that->d->current()->ch[0] == SpecialChars::PARSEP && ! --index)
			return i + 1;
		that->d->next();
	}
	return -1;
}

int StoryText::endOfParagraph(uint index) const
{
	++index;
	StoryText* that = const_cast<StoryText *>(this);
	that->d->at(0);
	for (int i=0; i < length(); ++i) {
		if (that->d->current()->ch[0] == SpecialChars::PARSEP && ! --index)
			return i;
		that->d->next();
	}
	return length();
}

uint StoryText::nrOfRuns() const
{
	return length();
}

int StoryText::startOfRun(uint index) const
{
	return index;
}

int StoryText::endOfRun(uint index) const
{
	return index + 1;
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
//	       || (pos >= 0 && pos < length() && const_cast<StoryText*>(this)->d->at(pos)->cselect)
	;
}

void StoryText::select(int pos, uint len, bool on)
{
	if (pos < 0)
		pos += length();

	assert( pos >= 0 );
	assert( pos + signed(len) <= length() );

//	qDebug("old selection: %d - %d", selFirst, selLast);

//	StoryText* that = const_cast<StoryText *>(this);
//	for (int i=pos; i < pos+signed(len); ++i)
//		that->at(i)->cselect = on;

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
	
//	qDebug("new selection: %d - %d", selFirst, selLast);
}

void StoryText::selectAll()
{
/*	StoryText* that = const_cast<StoryText *>(this);
	that->at(0);
	for (int i=0; i < length(); ++i) {
		that->current()->cselect = true;
		that->next();
	}
*/
	selFirst = 0;
	selLast = length() - 1;
}

void StoryText::deselectAll()
{
/*	StoryText* that = const_cast<StoryText *>(this);
	that->at(0);
	for (int i=0; i < length(); ++i) {
		that->current()->cselect = false;
		that->next();
	}
*/	
	selFirst = 0;
	selLast = -1;
}

void StoryText::removeSelection()
{
//	qDebug("removeSelection: %d - %d", selFirst, selLast);
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
	return const_cast<StoryText *>(this)->d->at(itm);
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

