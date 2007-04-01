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


//FIXME: this include must go to sctextstruct.h !
#include <q3valuelist.h>
#include <cassert>  //added to make Fedora-5 happy
#include "fpoint.h"
#include "scfonts.h"
#include "scribusdoc.h"
#include "sctext_shared.h"
#include "storytext.h"
//#include "storytext.moc"
#include "scribus.h"
#include "util.h"
#include "resourcecollection.h"
#include "desaxe/saxiohelper.h"
#include "desaxe/digester.h"
#include "desaxe/simple_actions.h"


StoryText::StoryText(ScribusDoc * doc_) : doc(doc_)
{
	if (doc_) {
		d = new ScText_Shared(&doc_->paragraphStyles());
		doc->paragraphStyles().connect(this, SLOT(invalidateAll()));
		doc->charStyles().connect(this, SLOT(invalidateAll()));
	}
	else {
		d = new ScText_Shared(NULL);
	}
	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;
	m_magicX = 0.0;
	m_lastMagicPos = -1;
	
	d->len = 0;
	invalidateAll();
}

StoryText::StoryText() : doc(NULL)
{
	d = new ScText_Shared(NULL);

	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;
	m_magicX = 0.0;
	m_lastMagicPos = -1;
}

StoryText::StoryText(const StoryText & other) : QObject(), SaxIO(), doc(other.doc)
{
	d = other.d;
	d->refs++;
	
	if(doc) {
		doc->paragraphStyles().connect(this, SLOT(invalidateAll()));
		doc->charStyles().connect(this, SLOT(invalidateAll()));
	}
	
	selFirst = 0;
	selLast = -1;
	
	firstFrameItem = 0;
	lastFrameItem = -1;
	m_magicX = 0.0;
	m_lastMagicPos = -1;

	invalidateLayout();
}

StoryText::~StoryText()
{
	if (doc)
	{
		doc->paragraphStyles().disconnect(this, SLOT(invalidateAll()));
		doc->charStyles().disconnect(this, SLOT(invalidateAll()));
	}
	d->refs--;
	if (d->refs == 0) {
		d->clear();
		d->len = 0;
		delete d;
	}	
}

StoryText StoryText::copy() const
{
	StoryText result(doc);
	*(result.d) = *d;
	return result;
//	qDebug(QString("StoryText::copy:"));
	Q3PtrListIterator<ScText> it( *(result.d) );
	ScText* elem;
	while ( (elem = it.current()) != NULL ) {
		++it;
//		qDebug(QString("\tchar '%1' size %2 (orig %3)").arg(elem->ch).arg(elem->fontSize()).arg(charStyle(i++).fontSize()));
	}
	
	return result;
}


StoryText& StoryText::operator= (const StoryText & other)
{
	other.d->refs++;
	
	d->refs--;
	if (d->refs == 0) {
		clear();
		delete d;
	}
	
	if(doc) {
		doc->paragraphStyles().disconnect(this, SLOT(invalidateAll()));
		doc->charStyles().disconnect(this, SLOT(invalidateAll()));
	}
	
	doc = other.doc; 
	d = other.d;
	
	if (doc) {
		doc->paragraphStyles().connect(this, SLOT(invalidateAll()));
		doc->charStyles().connect(this, SLOT(invalidateAll()));
	}
	
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
	
	d->defaultStyle.erase();
	d->trailingStyle.erase();
	
	d->clear();
	d->len = 0;
	invalidateAll();
}


void StoryText::insert(int pos, const StoryText& other, bool onlySelection)
{
	if (pos < 0)
		pos += length()+1;
	
	CharStyle cstyle(charStyle(pos));
	ParagraphStyle pstyle(paragraphStyle(pos));
	
	// this style represents all differences between this and other's defaultstyles
	ParagraphStyle otherDefault(other.defaultStyle());
	otherDefault.eraseStyle(defaultStyle());
	
	int otherStart  = onlySelection? other.startOfSelection() : 0;
	int otherEnd    = onlySelection? other.endOfSelection() : other.length();
	int cstyleStart = otherStart;
	for (int i=otherStart; i < otherEnd; ++i) {
		if (other.charStyle(i) == cstyle 
			&& other.text(i) != SpecialChars::OBJECT
			&& other.text(i) != SpecialChars::PARSEP)
			continue;
		int len = i - cstyleStart;
		if (len > 0) {
			insertChars(pos, other.text(cstyleStart, len));
			applyCharStyle(pos, len, otherDefault.charStyle());
			applyCharStyle(pos, len, cstyle);
			pos += len;
		}
		if (other.text(i) == SpecialChars::PARSEP) {
			insertChars(pos, SpecialChars::PARSEP);
			applyStyle(pos, otherDefault);
			applyStyle(pos, other.paragraphStyle(i));
			cstyleStart = i+1;
			pos += 1;
		}
		else if (other.text(i) == SpecialChars::OBJECT) {
			insertChars(pos, SpecialChars::OBJECT);
			item(pos)->embedded = other.item(i)->embedded;
			cstyleStart = i+1;
			pos += 1;
		}
		else {
			cstyle = other.charStyle(i);
			cstyleStart = i;
		}
	}
	int len = otherEnd - cstyleStart;
	if (len > 0) {
		insertChars(pos, other.text(cstyleStart, len));
		applyCharStyle(pos, len, otherDefault.charStyle());
		applyCharStyle(pos, len, cstyle);
		pos += len;
		if (other.text(otherEnd-1) != SpecialChars::PARSEP) {
			applyStyle(pos, otherDefault);
			applyStyle(pos, other.paragraphStyle(otherEnd-1));
		}
	}
	invalidate(pos, length());
}


/**
    Make sure that the paragraph CharStyle's point to the new ParagraphStyle
 */
void StoryText::insertParSep(int pos)
{
	ScText* it = item_p(pos);
	if(!it->parstyle) {
		it->parstyle = new ParagraphStyle(paragraphStyle(pos+1));
		it->parstyle->setContext( & d->pstyleContext);
//		it->parstyle->setName("para"); // DONT TRANSLATE
//		it->parstyle->charStyle().setName("cpara"); // DONT TRANSLATE
//		it->parstyle->charStyle().setContext( d->defaultStyle.charStyleContext() );
	}
	d->replaceCharStyleContextInParagraph(pos, it->parstyle->charStyleContext());
}
/**
     need to remove the ParagraphStyle structure and replace all pointers
     to it...
 */
void StoryText::removeParSep(int pos)
{
	ScText* it = item_p(pos);
	if (it->parstyle) {
//		const CharStyle* oldP = & it->parstyle->charStyle();
//		const CharStyle* newP = & that->paragraphStyle(pos+1).charStyle();
//		d->replaceParentStyle(pos, oldP, newP);
		delete it->parstyle;
		it->parstyle = 0;
	}
	// demote this parsep so the assert code in replaceCharStyleContextInParagraph()
	// doesnt choke:
	it->ch = "";
	d->replaceCharStyleContextInParagraph(pos, paragraphStyle(pos+1).charStyleContext());	
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
		if ((it->ch[0] == SpecialChars::PARSEP)) {
			removeParSep(i);
		}
//		qDebug("remove char %d at %d", it->ch[0].unicode(), i);
		d->take(i);
		d->len--;
		delete it;
	}

	d->len = d->count();
	invalidate(pos, length());
}

void StoryText::insertChars(int pos, QString txt) //, const CharStyle&
{
	if (pos < 0)
		pos += length()+1;

	assert(pos >= 0);
	assert(pos <= length());
	
	if (txt.length() == 0)
		return;
	
	const StyleContext* cStyleContext = paragraphStyle(pos).charStyleContext();
	//	assert( !style.font().isNone() );
	
	for (int i = 0; i < txt.length(); ++i) {
		ScText * item = new ScText();
		item->ch= txt.mid(i, 1);
		item->setContext(cStyleContext);
		d->insert(pos + i, item);
		d->len++;
		if (item->ch[0] == SpecialChars::PARSEP) {
//			qDebug(QString("new PARSEP %2 at %1").arg(pos).arg(paragraphStyle(pos).name()));
			insertParSep(pos + i);
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
		removeParSep(pos);
	}
	item->ch = ch;
	if (d->at(pos)->ch[0] == SpecialChars::PARSEP) {
		insertParSep(pos);
	}
	
	invalidate(pos, pos + 1);
}

void StoryText::hyphenateWord(int pos, uint len, char* hyphens)
{
	assert(pos >= 0);
	assert(pos + signed(len) <= length());
	
//	QString dump("");
	for (int i=pos; i < pos+signed(len); ++i)
	{
//		dump += d->at(i)->ch;
		if(hyphens && hyphens[i-pos] & 1) {
			d->at(i)->setEffects(d->at(i)->effects() | ScStyle_HyphenationPossible);
//			dump += "-";
		}
		else {
			d->at(i)->setEffects(d->at(i)->effects() & ~ScStyle_HyphenationPossible);
		}
	}
//	qDebug(QString("st: %1").arg(dump));
	invalidate(pos, pos + len);
}

void StoryText::insertObject(int pos, PageItem* ob)
{
	if (pos < 0)
		pos += length()+1;

	insertChars(pos, SpecialChars::OBJECT);
	const_cast<StoryText *>(this)->d->at(pos)->embedded = InlineFrame(ob);
	ob->isEmbedded = true;   // this might not be enough...
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
	return that->d->at(pos)->embedded.getItem();
}


const CharStyle & StoryText::charStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	if (length() == 0) {
//		qDebug("storytext::charstyle: default");
		return defaultStyle().charStyle();
	}
	else if (pos == length()) {
		qDebug(QString("storytext::charstyle: access at end of text %1").arg(pos));
		--pos;
	}
	
	StoryText* that = const_cast<StoryText *>(this);
	return dynamic_cast<const CharStyle &> (*that->d->at(pos));
}

const ParagraphStyle & StoryText::paragraphStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	assert(d);
	
	StoryText * that = const_cast<StoryText *> (this);
//	assert( that->at(pos)->cab >= 0 );
//	assert( that->at(pos)->cab < doc->docParagraphStyles.count() );
//	return doc->docParagraphStyles[that->at(pos)->cab];
	
	that->d->at(pos);
	while (pos < length() && that->d->current()->ch[0] != SpecialChars::PARSEP) {
		++pos;
		that->d->next();
	}
	if (pos >= length()) {
		return that->d->trailingStyle;
	}
	else if ( !that->d->current()->parstyle ) {
		qDebug(QString("inserting default parstyle at %1").arg(pos));
		that->d->current()->parstyle = new ParagraphStyle();
		that->d->current()->parstyle->setContext( & d->pstyleContext);
//		that->d->current()->parstyle->setName( "para(paragraphStyle)" ); // DONT TRANSLATE
//		that->d->current()->parstyle->charStyle().setName( "cpara(paragraphStyle)" ); // DONT TRANSLATE
//		that->d->current()->parstyle->charStyle().setContext( d->defaultStyle.charStyleContext());
	}
	else {
//		qDebug(QString("using parstyle at %1").arg(pos));
	}
	assert (that->d->current()->parstyle);
	return *that->d->current()->parstyle;
}

const ParagraphStyle& StoryText::defaultStyle() const
{
	assert(d);
	return d->defaultStyle;
}


void StoryText::setDefaultStyle(const ParagraphStyle& style)
{
	const StyleContext * oldPContext = d->defaultStyle.context();
//	const StyleContext * oldCContext = d->defaultStyle.charStyle().context();
	d->defaultStyle = style;
	d->defaultStyle.setContext( oldPContext );
//	d->defaultStyle.setName( "storydefault" ); // DONT TRANSLATE
//	d->defaultStyle.charStyle().setName( "cstorydefault" ); // DONT TRANSLATE
//	qDebug(QString("defstyle %1 context %2 defcstyle %3 ccontext %4 newcontext %5")
//		   .arg((uint)&d->defaultStyle,16).arg((uint)oldPContext,16)
//		   .arg((uint)&d->defaultStyle.charStyle(),16).arg((uint)oldCContext,16)
//		   .arg((uint)d->defaultStyle.charStyle().context(),16));
//	d->defaultStyle.charStyle().setContext( oldCContext );
	invalidateAll();
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

	int i = pos;
	while (i < length() && d->at(i)->ch[0] != SpecialChars::PARSEP) {
		++i;
	}
	if (i < length()) {
		if (!d->at(i)->parstyle) {
			qDebug(QString("PARSEP without style at pos %1").arg(i));
			d->at(i)->parstyle = new ParagraphStyle();
			d->at(i)->parstyle->setContext( & d->pstyleContext);
//			d->at(i)->parstyle->setName( "para(applyStyle)" ); // DONT TRANSLATE
//			d->at(i)->parstyle->charStyle().setName( "cpara(applyStyle)" ); // DONT TRANSLATE
//			d->at(i)->parstyle->charStyle().setContext( d->defaultStyle.charStyleContext() );
		}
//		qDebug(QString("applying parstyle %2 at %1 for %3").arg(i).arg(paragraphStyle(pos).name()).arg(pos));
		d->at(i)->parstyle->applyStyle(style);
	}
	else {
		// not happy about this but inserting a new PARSEP makes more trouble
//		qDebug(QString("applying parstyle %1 as defaultstyle for %2").arg(paragraphStyle(pos).name()).arg(pos));
		d->trailingStyle.applyStyle(style);
	}
	invalidate(pos, qMin(i, length()));
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
			d->at(i)->parstyle = new ParagraphStyle();
			d->at(i)->parstyle->setContext( & d->pstyleContext);
//			d->at(i)->parstyle->setName( "para(eraseStyle)" ); // DONT TRANSLATE
//			d->at(i)->parstyle->charStyle().setName( "cpara(eraseStyle)" ); // DONT TRANSLATE
//			d->at(i)->parstyle->charStyle().setContext( d->defaultStyle.charStyleContext());
		}
		//		qDebug(QString("applying parstyle %2 at %1 for %3").arg(i).arg(paragraphStyle(pos).name()).arg(pos));
		d->at(i)->parstyle->eraseStyle(style);
	}
	else {
		// not happy about this but inserting a new PARSEP makes more trouble
		//		qDebug(QString("applying parstyle %1 as defaultstyle for %2").arg(paragraphStyle(pos).name()).arg(pos));
		d->trailingStyle.eraseStyle(style);
	}
	invalidate(pos, qMin(i, length()));
}


void StoryText::setStyle(int pos, const ParagraphStyle& style)
{
	eraseStyle(pos, paragraphStyle(pos));
	applyStyle(pos, style);
}


void StoryText::setCharStyle(int pos, uint len, const CharStyle& style)
{
	// FIXME
	eraseCharStyle(pos, len, charStyle(pos));
	applyCharStyle(pos, len, style);
}



void StoryText::getNamedResources(ResourceCollection& lists) const
{
	defaultStyle().getNamedResources(lists);
	for (int i=0; i < length(); ++i)
	{
		if (text(i) == SpecialChars::PARSEP)
			paragraphStyle(i).getNamedResources(lists);
		else if (text(i) == SpecialChars::OBJECT)
			object(i)->getNamedResources(lists);
		else
			charStyle(i).getNamedResources(lists);
	}
}


void StoryText::replaceStyles(QMap<QString,QString> newNameForOld)
{
	ResourceCollection newnames;
	newnames.mapStyles(newNameForOld);
	replaceNamedResources(newnames);
}

void StoryText::replaceNamedResources(ResourceCollection& newNames)
{
	int len = length();

	d->defaultStyle.replaceNamedResources(newNames);
	d->trailingStyle.replaceNamedResources(newNames);
	
	if (len == 0)
		return;
	
	d->at(0);
	for (int i=0; i < len; ++i) {
		if (d->current()->parstyle)
			d->current()->parstyle->replaceNamedResources(newNames);
		else
			d->current()->replaceNamedResources(newNames);
		d->next();
	}
	
	invalidate(0, len);	
}


void StoryText::replaceCharStyles(QMap<QString,QString> newNameForOld)
{
	ResourceCollection newnames;
	newnames.mapCharStyles(newNameForOld);
	replaceNamedResources(newnames);
}


uint StoryText::nrOfParagraphs() const
{
	uint result = 0;
	StoryText* that = const_cast<StoryText *>(this);
	that->d->at(0);
	bool lastWasPARSEP = true;
	for (int i=0; i < length(); ++i) {
		lastWasPARSEP = that->d->current()->ch[0] == SpecialChars::PARSEP;
		if (lastWasPARSEP)
			++result;
		that->d->next();
	}
	return lastWasPARSEP? result : result + 1;
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
	return length();
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

// positioning. all positioning methods return char positions
// FIXME: make that methods use correct semantic boundaries

static QString wordBoundaries(" .,:;\"'!?\n");
static QString sentenceBoundaries(".:!?\n");

int StoryText::nextChar(int pos)
{
	if (pos < length())
		return pos+1;
	else
		return length();
}
int StoryText::prevChar(int pos)
{
	if (pos > 0)
		return pos - 1;
	else 
		return 0;
}
int StoryText::nextWord(int pos)
{
	int len = length();
	pos = qMin(len, pos+1);
	while (pos < len  && wordBoundaries.find(text(pos)) < 0)
		++pos;
	return pos < len ? pos + 1 : pos;
}
int StoryText::prevWord(int pos)
{
	pos = qMax(0, pos-1);
	while (pos > 0 && wordBoundaries.find(text(pos)) < 0)
		--pos;
	return wordBoundaries.find(text(pos)) < 0 ? pos + 1 : pos;
}
int StoryText::nextSentence(int pos)
{
	int len = length();
	pos = qMin(len, pos+1);
	while (pos < len && sentenceBoundaries.find(text(pos)) < 0)
		++pos;
	return pos < len ? pos + 1 : pos;
}
int StoryText::prevSentence(int pos)
{
	pos = qMax(0, pos-1);
	while (pos > 0 && sentenceBoundaries.find(text(pos)) < 0)
		--pos;
	return sentenceBoundaries.find(text(pos)) < 0 ? pos + 1 : pos;
}
int StoryText::nextParagraph(int pos)
{
	int len = length();
	pos = qMin(len, pos+1);
	while (pos < len && text(pos) != SpecialChars::PARSEP)
		++pos;
	return pos;
}
int StoryText::prevParagraph(int pos)
{
	pos = qMax(0, pos-1);
	while (pos > 0 && text(pos) != SpecialChars::PARSEP)
		--pos;
	return pos;
}

// these need valid layout:

int StoryText::startOfLine(int pos)
{
	for (int i=0; i < m_lines.count(); ++i) {
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem)
			return ls.firstItem;
	}
	return 0;
}
int StoryText::endOfLine(int pos)
{
	for (int i=0; i < m_lines.count(); ++i) {
		const LineSpec & ls(m_lines.at(i));
		if (ls.firstItem <= pos && pos <= ls.lastItem)
			return text(ls.lastItem) == SpecialChars::PARSEP ? ls.lastItem : 
				text(ls.lastItem) == ' ' ? ls.lastItem : ls.lastItem + 1;
	}
	return length();
}
int StoryText::prevLine(int pos)
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
			double xpos = 0.0;
			for (int j = ls.firstItem; j < pos; ++j)
				xpos += item(j)->glyph.wide();
			if (pos != m_lastMagicPos || xpos > m_magicX)
				m_magicX = xpos;
			const LineSpec & ls2(m_lines.at(i-1));
			// find new cpos
			xpos = 0.0;
			for (int j = ls2.firstItem; j <= ls2.lastItem; ++j) 
			{
				xpos += item(j)->glyph.wide();
				if (xpos > m_magicX) {
					m_lastMagicPos = j;
					return j;
				}
			}
			m_lastMagicPos = ls2.lastItem;
			return ls2.lastItem;
		}
	}
	return firstFrameItem;
}

int StoryText::nextLine(int pos)
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
			double xpos = 0.0;
			for (int j = ls.firstItem; j < pos; ++j)
				xpos += item(j)->glyph.wide();
			if (pos != m_lastMagicPos || xpos > m_magicX)
				m_magicX = xpos;
			const LineSpec & ls2(m_lines.at(i+1));
			// find new cpos
			xpos = 0.0;
			for (int j = ls2.firstItem; j <= ls2.lastItem; ++j) 
			{
				xpos += item(j)->glyph.wide();
				if (xpos > m_magicX) {
					m_lastMagicPos = j;
					return j;
				}
			}
			m_lastMagicPos = ls2.lastItem + 1;
			return ls2.lastItem + 1;
		}
	}
	return lastFrameItem;
}

int StoryText::startOfFrame(int pos) 
{
	return firstFrameItem;
}
int StoryText::endOfFrame(int pos)
{
	return lastFrameItem + 1;
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
			selLast = qMax(selLast, pos + static_cast<int>(len) - 1);
		else if (selected(pos + len))
			selFirst = qMin(selFirst, pos);
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
	d->pstyleContext.invalidate();
	invalidate(0, nrOfItems());
}

void StoryText::invalidate(int firstItem, int endItem)
{
	for (int i=firstItem; i < endItem; ++i) {
		ParagraphStyle* par = item(i)->parstyle;
		if (par)
			par->charStyleContext()->invalidate();
	}
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
	double maxx = coord.x() - 1.0;
	for (unsigned int i=0; i < lines(); ++i)
	{
		LineSpec ls = line(i);
//		qDebug(QString("screenToPosition: (%1,%2) -> y %3 - %4 + %5").arg(coord.x()).arg(coord.y()).arg(ls.y).arg(ls.ascent).arg(ls.descent));
		if (ls.y + ls.descent < coord.y())
			continue;
		double xpos = ls.x;
		for (int j = ls.firstItem; j <= ls.lastItem; ++j) {
//				qDebug(QString("screenToPosition: (%1,%2) -> x %3 + %4").arg(coord.x()).arg(coord.y()).arg(xpos).arg(item(j)->glyph.wide()));
			double width = item(j)->glyph.wide();
			xpos += width;
			if (xpos >= coord.x())
				return xpos - width/2 > coord.x() ? j : j+1;
		}
		if (xpos > maxx)
			maxx = xpos;
		if (xpos + 1.0 > coord.x()) // allow 1pt after end of line
			return ls.lastItem + 1;
		else if (xpos < ls.x + 0.01 && maxx >= coord.x()) // check for empty line
			return ls.firstItem;
	}
	return qMax(lastFrameItem+1, firstFrameItem);
}


FRect StoryText::boundingBox(int pos, uint len) const
{
	FRect result;
	for (uint i=0; i < lines(); ++i)
	{
		LineSpec ls = line(i);
		if (ls.lastItem < pos)
			continue;
		if (ls.firstItem <= pos) {
			double xpos = ls.x;
			for (int j = ls.firstItem; j < pos; ++j)
				xpos += item(j)->glyph.wide();
			result.setRect(xpos, ls.y-ls.ascent, item(pos)->glyph.wide(), ls.ascent + ls.descent);
			break;
		}
	}
	return result;
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


const ScText*  StoryText::item(uint itm) const
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


using namespace desaxe;

void StoryText::saxx(SaxHandler& handler, Xml_string elemtag) const
{
	Xml_attr empty;
	Xml_attr pageno;
	pageno.insert("name", "pgno");

	handler.begin(elemtag, empty);
	defaultStyle().saxx(handler, "defaultstyle");

	CharStyle lastStyle(charStyle(0));
	bool lastWasPar = true;
	int lastPos = 0;
	handler.begin("p", empty);
	paragraphStyle(0).saxx(handler);
	handler.begin("span", empty);
	lastStyle.saxx(handler);
	for (int i=0; i < length(); ++i)
	{
		const QChar curr(text(i));
		const CharStyle& style(charStyle(i));
		
		if (curr == SpecialChars::OBJECT ||
			curr == SpecialChars::TAB ||
			curr == SpecialChars::PARSEP ||
			curr == SpecialChars::LINEBREAK ||
			curr == SpecialChars::COLBREAK ||
			curr == SpecialChars::FRAMEBREAK ||
			curr == SpecialChars::PAGENUMBER ||
			curr.unicode() < 32 || 
			(0xd800 <= curr.unicode() && curr.unicode() < 0xe000) ||
			curr.unicode() == 0xfffe || curr.unicode() == 0xffff ||
			style != lastStyle)
		{
			// something new, write pending chars
			if  (i - lastPos > 0)
			{
				handler.chars(text(lastPos, i-lastPos));
			}
			lastPos = i;
		}

		lastWasPar = (curr == SpecialChars::PARSEP);
		if (lastWasPar)
		{
			handler.end("span");
			handler.end("p");
			handler.begin("p", empty);
			paragraphStyle(i).saxx(handler);
			handler.begin("span", empty);
		}
		else if (curr == SpecialChars::OBJECT && object(i) != NULL)
		{
			object(i)->saxx(handler);
		}
		else if (curr == SpecialChars::TAB)
		{
			handler.beginEnd("tab", empty);
		}
		else if (curr == SpecialChars::LINEBREAK)
		{
			handler.beginEnd("breakline", empty);
		}
		else if (curr == SpecialChars::COLBREAK)
		{
			handler.beginEnd("breakcol", empty);
		}
		else if (curr == SpecialChars::FRAMEBREAK)
		{
			handler.beginEnd("breakframe", empty);
		}
		else if (curr == SpecialChars::PAGENUMBER)
		{
			handler.beginEnd("var", pageno);
		}
		else if (curr.unicode() < 32 || 
				 (0xd800 <= curr.unicode() && curr.unicode() < 0xe000) ||
				 curr.unicode() == 0xfffe || curr.unicode() == 0xffff)
		{
			Xml_attr unic;
			unic.insert("code", toXMLString(curr.unicode()));
			handler.beginEnd("unicode", unic);
		}
		else if (lastStyle != style)
		{
			handler.end("span");
			handler.begin("span", empty);
			style.saxx(handler);
			lastStyle = style;
			continue;
		}
		else
			continue;
		lastPos = i+1;
	}
	
	if  (length() - lastPos > 0)
		handler.chars(text(lastPos, length()-lastPos));
	handler.end("span");
	handler.end("p");
	
//	if (!lastWasPar)
//		paragraphStyle(length()-1).saxx(handler);
	
	handler.end(elemtag);

}


class AppendText_body : public Action_body
{
public:	
	void chars(const Xml_string txt)
	{
		StoryText* obj = this->dig->top<StoryText>();
		obj->insertChars(-1, txt ); 
	}
};

struct  AppendText : public MakeAction<AppendText_body> 
{};


class AppendSpecial_body : public Action_body
{
public:
	AppendSpecial_body(QChar sp) : chr(sp) {}
	
	void begin(const Xml_string tag, Xml_attr attr)
	{
		StoryText* obj = this->dig->top<StoryText>();
		Xml_attr::iterator code = attr.find("code");
		if (tag == "unicode" && code != attr.end())
			obj->insertChars(-1, QChar(parseUInt(Xml_data(code))));
		else
			obj->insertChars(-1, chr);
	}
private:
	QChar chr;
};

struct AppendSpecial : public MakeAction<AppendSpecial_body, QChar>
{
	AppendSpecial(QChar sp) : MakeAction<AppendSpecial_body, QChar>(sp) {}
	AppendSpecial() : MakeAction<AppendSpecial_body, QChar>(SpecialChars::BLANK) {}
};


class AppendInlineFrame_body : public Action_body
{
public:
	void end(const Xml_string tag) // this could be a setter if we had StoryText::appendObject() ...
	{
		StoryText* story = this->dig->top<StoryText>(1);
		PageItem* obj = this->dig->top<PageItem>(0);
		story->insertObject(-1, obj);
	}
};

struct AppendInlineFrame : public MakeAction<AppendInlineFrame_body>
{};

/*
class ApplyStyle_body : public Action_body
{
public:
	void end(const Xml_string tag) 
	{
		qDebug("storytext desaxe: apply style");
		StoryText* story = this->dig->top<StoryText>(1);
		ParagraphStyle* obj = this->dig->top<ParagraphStyle>(0);
		story->applyStyle(-1, *obj);
	}
};

struct ApplyStyle : public MakeAction<ApplyStyle_body>
{};


class ApplyCharStyle_body : public Action_body
{
public:
	ApplyCharStyle_body() : storyTag(StoryText::saxxDefaultElem), lastPos(0), lastStyle()
	{}
	ApplyCharStyle_body(const Xml_string& tag) : storyTag(tag), lastPos(0), lastStyle()
	{}
	
	void end(const Xml_string tag) 
	{
		qDebug("storytext desaxe: apply charstyle");
		if (tag == CharStyle::saxxDefaultElem)
		{
			StoryText* story = this->dig->top<StoryText>(1);
			CharStyle* obj = this->dig->top<CharStyle>(0);
			int len = story->length();
			if (len > lastPos && lastStyle != *obj)
			{
				story->applyCharStyle(lastPos, len - lastPos, lastStyle);
				lastPos = len;
				lastStyle = *obj;
			}
		}
		else if (tag == StoryText::saxxDefaultElem)
		{
			StoryText* story = this->dig->top<StoryText>();
			int len = story->length();
			if (len > lastPos)
			{
				story->applyCharStyle(lastPos, len - lastPos, lastStyle);
			}
		}
	}
private:
	Xml_string storyTag;
	int lastPos;
	CharStyle lastStyle;
};

struct ApplyCharStyle : public MakeAction<ApplyCharStyle_body, const Xml_string&>
{
	ApplyCharStyle() : MakeAction<ApplyCharStyle_body, const Xml_string&>() {}
	ApplyCharStyle(const Xml_string& tag) : MakeAction<ApplyCharStyle_body, const Xml_string&>(tag) {}
};

*/

class Paragraph_body : public Action_body
{
public:
	Paragraph_body() : lastPos(0), lastStyle(NULL)
	{}
	
	~Paragraph_body() 
	{
		if (lastStyle)
			delete lastStyle;
	}
	
	void begin(const Xml_string tag, Xml_attr attr)
	{
		if (tag == "p")
		{
			StoryText* story = this->dig->top<StoryText>();
			qDebug(QString("startpar: %1->%2 %3->NULL").arg(lastPos).arg(story->length()).arg((ulong)lastStyle));
			lastPos = story->length();
			if (lastPos > 0) {
				story->insertChars(-1, SpecialChars::PARSEP);
				++lastPos;
			}
			if (lastStyle)
				delete lastStyle;
			lastStyle = NULL;
		}
	}
	
	void end(const Xml_string tag) 
	{
		if (tag == ParagraphStyle::saxxDefaultElem)
		{
			if (lastStyle)
				delete lastStyle;
			lastStyle = this->dig->top<ParagraphStyle>(0);
			qDebug(QString("endstyle: %1 %2 %3").arg("?").arg(lastPos).arg((ulong)lastStyle));
		}
		else if (tag == "p")
		{
			StoryText* story = this->dig->top<StoryText>();
			int len = story->length();
			qDebug(QString("endpar: %1 %2 %3 %4").arg(len).arg(lastPos).arg((ulong)lastStyle).arg(lastStyle? lastStyle->parent() : QString()));
			if (len > lastPos && lastStyle)
			{
				story->applyStyle(lastPos, *lastStyle);
			}
		}
	}
private:
	int lastPos;
	ParagraphStyle* lastStyle;
};

struct Paragraph : public MakeAction<Paragraph_body>
{
	Paragraph() : MakeAction<Paragraph_body>() {}
};


class SpanAction_body : public Action_body
{
public:
	SpanAction_body() : lastPos(0), lastStyle(NULL)
	{}
	
	~SpanAction_body() 
	{
		if (lastStyle)
			delete lastStyle;
	}
	
	void begin(const Xml_string tag, Xml_attr attr)
	{
//		qDebug(QString("spanaction: begin %1").arg(tag));
		if (tag == "span")
		{
			StoryText* story = this->dig->top<StoryText>();
			lastPos = story->length();
			if (lastStyle)
				delete lastStyle;
			lastStyle = NULL;
		}
	}
	
	void end(const Xml_string tag) 
	{
		if (tag == CharStyle::saxxDefaultElem)
//			qDebug(QString("spanaction: end %1").arg(tag));
		{
			if (lastStyle)
				delete lastStyle;
			lastStyle = this->dig->top<CharStyle>(0);
		}
		else if (tag == "span")
		{
			StoryText* story = this->dig->top<StoryText>();
			int len = story->length();
			if (len > lastPos && lastStyle)
			{
				story->applyCharStyle(lastPos, len - lastPos, *lastStyle);
			}
		}
	}
private:
	int lastPos;
	CharStyle* lastStyle;
};

struct SpanAction : public MakeAction<SpanAction_body>
{
	SpanAction() : MakeAction<SpanAction_body>() {}
};


const Xml_string StoryText::saxxDefaultElem("story");

void StoryText::desaxeRules(Xml_string prefixPattern, Digester& ruleset, Xml_string elemtag)
{
	Xml_string storyPrefix(Digester::concat(prefixPattern, elemtag));
	ruleset.addRule(storyPrefix, Factory<StoryText>());

	ParagraphStyle::desaxeRules(storyPrefix, ruleset, "defaultstyle");
	ruleset.addRule(Digester::concat(storyPrefix, "defaultstyle"), SetterWithConversion<StoryText, const ParagraphStyle&, ParagraphStyle>( & StoryText::setDefaultStyle ));
	
	Paragraph paraAction;
	Xml_string paraPrefix(Digester::concat(storyPrefix, "p"));
	ruleset.addRule(paraPrefix, paraAction ); 
	ParagraphStyle::desaxeRules(paraPrefix, ruleset, ParagraphStyle::saxxDefaultElem);
	ruleset.addRule(Digester::concat(paraPrefix, ParagraphStyle::saxxDefaultElem), paraAction ); 
	
	SpanAction spanAction;
	Xml_string spanPrefix(Digester::concat(paraPrefix, "span"));
	ruleset.addRule(spanPrefix, spanAction );
	CharStyle::desaxeRules(spanPrefix, ruleset, CharStyle::saxxDefaultElem);
	ruleset.addRule(Digester::concat(spanPrefix, CharStyle::saxxDefaultElem), spanAction );
		
	ruleset.addRule(spanPrefix, AppendText());
	
	ruleset.addRule(Digester::concat(spanPrefix, "breakline"), AppendSpecial(SpecialChars::LINEBREAK) );
	ruleset.addRule(Digester::concat(spanPrefix, "breakcol"), AppendSpecial(SpecialChars::COLBREAK) );
	ruleset.addRule(Digester::concat(spanPrefix, "breakframe"), AppendSpecial(SpecialChars::FRAMEBREAK) );
	ruleset.addRule(Digester::concat(spanPrefix, "tab"), AppendSpecial(SpecialChars::TAB) ); 
	ruleset.addRule(Digester::concat(spanPrefix, "unicode"), AppendSpecial() ); 
	ruleset.addRule(Digester::concat(spanPrefix, "var"), AppendSpecial(SpecialChars::PAGENUMBER) ); 
	
	//PageItem::desaxeRules(storyPrefix, ruleset); argh, that would be recursive!
	ruleset.addRule(Digester::concat(spanPrefix, "item"), AppendInlineFrame() ); 
	
}
