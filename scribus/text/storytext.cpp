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
#include <QList>
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
/* Code below is not needed imho, as all connections will be disconnected automatically
	by the QObject destructor. At least removing that code fixes a crash when closing
	documents */
/*	if (doc)
	{
		doc->paragraphStyles().disconnect(this, SLOT(invalidateAll()));
		doc->charStyles().disconnect(this, SLOT(invalidateAll()));
	} */
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
	QListIterator<ScText*> it( *(result.d) );
	ScText* elem;
	while ( it.hasNext() ) {
		elem = it.next();
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
			insertCharsWithSmartHyphens(pos, other.textWithSmartHyphens(cstyleStart, len));
			applyCharStyle(pos, len, otherDefault.charStyle());
			applyCharStyle(pos, len, cstyle);
			pos += len;
		}
		if (other.text(i) == SpecialChars::PARSEP) {
			insertChars(pos, SpecialChars::PARSEP);
			//#5845 : disable for now as it has nasty side effects when linking frames
			//applyStyle(pos, otherDefault);
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
		insertCharsWithSmartHyphens(pos, other.textWithSmartHyphens(cstyleStart, len));
		applyCharStyle(pos, len, otherDefault.charStyle());
		applyCharStyle(pos, len, cstyle);
		pos += len;
		if (other.text(otherEnd-1) != SpecialChars::PARSEP) {
			//#5845 : disable for now as it has nasty side effects when linking frames
			//applyStyle(pos, otherDefault);
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
		// #7432 : when inserting a paragraph separator, apply/erase the trailing Style
		if (pos >= signed(d->len - 1))
		{
			applyStyle(pos, d->trailingStyle);
			d->trailingStyle.erase();
		}
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
	it->ch = 0;
	d->replaceCharStyleContextInParagraph(pos, paragraphStyle(pos+1).charStyleContext());	
}

void StoryText::removeChars(int pos, uint len)
{
	if (pos < 0)
		pos += length();
	
	assert( len > 0 );
	assert( pos >= 0 );
	// This case can be handled more gracefully
	// assert( pos + static_cast<int>(len) <= length() );
	if (pos >= length())
		return;
	if (pos + static_cast<int>(len) > length())
		len = length() - pos;

	for ( int i=pos + static_cast<int>(len) - 1; i >= pos; --i )
	{
		ScText *it = d->at(i);
		if ((it->ch == SpecialChars::PARSEP)) {
			removeParSep(i);
		}
//		qDebug("remove char %d at %d", (int) it->ch.unicode(), i);
		d->takeAt(i);
		d->len--;
		delete it;
	}

	d->len = d->count();
	invalidate(pos, length());
}

void StoryText::insertChars(int pos, QString txt, bool applyNeighbourStyle) //, const CharStyle & charstyle)
{
	if (pos < 0)
		pos += length()+1;

	assert(pos >= 0);
	assert(pos <= length());
	
	if (txt.length() == 0)
		return;
	
	const StyleContext* cStyleContext = paragraphStyle(pos).charStyleContext();

	ScText clone;
	if (applyNeighbourStyle)
	{
		int referenceChar = qMax(0, qMin(pos, length()-1));
		clone.applyCharStyle(charStyle(referenceChar));
		clone.setEffects(ScStyle_Default);
	}

	for (int i = 0; i < txt.length(); ++i) {
		ScText * item = new ScText(clone);
		item->ch= txt.at(i);
		item->setContext(cStyleContext);
		d->insert(pos + i, item);
		d->len++;
		if (item->ch == SpecialChars::PARSEP) {
//			qDebug(QString("new PARSEP %2 at %1").arg(pos).arg(paragraphStyle(pos).name()));
			insertParSep(pos + i);
		}
	}

	d->len = d->count();
	invalidate(pos, pos + txt.length());
}

void StoryText::insertCharsWithSmartHyphens(int pos, QString txt, bool applyNeighbourStyle)
{
	if (pos < 0)
		pos += length()+1;

	assert(pos >= 0);
	assert(pos <= length());
	
	if (txt.length() == 0)
		return;
	
	const StyleContext* cStyleContext = paragraphStyle(pos).charStyleContext();

	ScText clone;
	if (applyNeighbourStyle)
	{
		int referenceChar = qMax(0, qMin(pos, length()-1));
		clone.applyCharStyle(charStyle(referenceChar));
		clone.setEffects(ScStyle_Default);
	}

	int inserted = 0;
	for (int i = 0; i < txt.length(); ++i) 
	{
		QChar ch = txt.at(i);
		int  index  = pos + inserted;
		bool insert = true; 
		if (ch == SpecialChars::SHYPHEN && index > 0) {
			ScText* lastItem = this->item(index - 1);
			// double SHY means user provided SHY, single SHY is automatic one
			if (lastItem->effects() & ScStyle_HyphenationPossible)
				lastItem->setEffects(lastItem->effects() & ~ScStyle_HyphenationPossible);
			else
			{
				lastItem->setEffects(lastItem->effects() | ScStyle_HyphenationPossible);
				insert = false;
			}
		}
		if (insert)
		{
			ScText * item = new ScText(clone);
			item->ch = ch;
			item->setContext(cStyleContext);
			d->insert(index, item);
			d->len++;
			if (item->ch == SpecialChars::PARSEP) {
				insertParSep(index);
			}
			++inserted;
		}
	}

	d->len = d->count();
	invalidate(pos, pos + inserted);
}

void StoryText::replaceChar(int pos, QChar ch)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	ScText* item = d->at(pos);
	if (item->ch == ch)
		return;
	
	if (d->at(pos)->ch == SpecialChars::PARSEP) {
		removeParSep(pos);
	}
	item->ch = ch;
	if (d->at(pos)->ch == SpecialChars::PARSEP) {
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

	return const_cast<StoryText *>(this)->d->at(pos)->ch;
}

QString StoryText::text(int pos, uint len) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos + signed(len) <= length());

	QString result;
	StoryText* that(const_cast<StoryText*>(this));
	for (int i = pos; i < pos+signed(len); ++i) {
		result += that->d->at(i)->ch;
	}

	return result;
}

QString StoryText::textWithSmartHyphens(int pos, uint len) const
{
	QString result("");
	int lastPos = pos;

	len = qMin((uint) (length() - pos), len);
	for (int i = pos; i < pos+signed(len); ++i)
	{
		if (this->charStyle(i).effects() & ScStyle_HyphenationPossible 
			// duplicate SHYPHEN if already present to indicate a user provided SHYPHEN:
			|| this->text(i) == SpecialChars::SHYPHEN)
		{
			result += text(lastPos, i + 1 - lastPos);
			result += SpecialChars::SHYPHEN;
			lastPos = i+1;
		}
	}
	if (lastPos < pos+signed(len))
		result += text(lastPos, pos+signed(len) - lastPos);
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
		qDebug("storytext::charstyle: access at end of text %i", pos);
		--pos;
	}
	if (text(pos) == SpecialChars::PARSEP)
		return paragraphStyle(pos).charStyle();
	
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
	
	while (pos < length() && that->d->at(pos)->ch != SpecialChars::PARSEP) {
		++pos;
	}
	if (pos >= length()) {
		return that->d->trailingStyle;
	}
	else if ( !that->d->at(pos)->parstyle ) {
		ScText* current = that->d->at(pos);
		qDebug("inserting default parstyle at %i", pos);
		current->parstyle = new ParagraphStyle();
		current->parstyle->setContext( & d->pstyleContext);
//		current->parstyle->setName( "para(paragraphStyle)" ); // DONT TRANSLATE
//		current->parstyle->charStyle().setName( "cpara(paragraphStyle)" ); // DONT TRANSLATE
//		current->parstyle->charStyle().setContext( d->defaultStyle.charStyleContext());
	}
	else {
//		qDebug(QString("using parstyle at %1").arg(pos));
	}
	assert (that->d->at(pos)->parstyle);
	return *that->d->at(pos)->parstyle;
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

/* Why do we use an assert here instead of a gracefully return doing nothing ????? */
//	assert(pos >= 0);
//	assert(pos + signed(len) <= length());
	if (pos < 0)
		return;
	if (pos + signed(len) > length())
		return;

	if (len == 0)
		return;

	ScText* itText;
	for (uint i=pos; i < pos+len; ++i) {
		itText = d->at(i);
		// #6165 : applying style on last character applies style on whole text on next open 
		/*if (itText->ch == SpecialChars::PARSEP && itText->parstyle != NULL)
			itText->parstyle->charStyle().applyCharStyle(style);*/
		itText->applyCharStyle(style);
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
	
	ScText* itText;
	for (uint i=pos; i < pos+len; ++i) {
		itText = d->at(i);
		if (itText->ch == SpecialChars::PARSEP && itText->parstyle != NULL)
			itText->parstyle->charStyle().eraseCharStyle(style);
		itText->eraseCharStyle(style);
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
	while (i < length() && d->at(i)->ch != SpecialChars::PARSEP) {
		++i;
	}
	if (i < length()) {
		if (!d->at(i)->parstyle) {
			qDebug("PARSEP without style at pos %i", i);
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
	while (i < length() && d->at(i)->ch != SpecialChars::PARSEP) {
		++i;
	}
	if (i < length()) {
		if (!d->at(i)->parstyle) {
			qDebug("PARSEP without style at pos %i", i);
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
	if (pos < 0)
		pos += length();
	
	assert(pos >= 0);
	assert(len <= unsigned(length()));
	assert(pos + signed(len) <= length());
	
	if (len == 0)
		return;
	
	ScText* itText;
	for (uint i=pos; i < pos+len; ++i) {
		itText = d->at(i);
		// #6165 : applying style on last character applies style on whole text on next open 
		/*if (itText->ch == SpecialChars::PARSEP && itText->parstyle != NULL)
			itText->parstyle->charStyle() = style;*/
		itText->setStyle(style);
	}
	
	invalidate(pos, pos + len);
}



void StoryText::getNamedResources(ResourceCollection& lists) const
{
	d->defaultStyle.getNamedResources(lists);
	d->trailingStyle.getNamedResources(lists);

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
	
	ScText* itText;
	for (int i=0; i < len; ++i) {
		itText = d->at(i);
		if (itText->parstyle)
			itText->parstyle->replaceNamedResources(newNames);
		else
			itText->replaceNamedResources(newNames);
	}
	
	invalidate(0, len);	
}


void StoryText::replaceCharStyles(QMap<QString,QString> newNameForOld)
{
	ResourceCollection newnames;
	newnames.mapCharStyles(newNameForOld);
	replaceNamedResources(newnames);
}


int StoryText::nrOfParagraph(uint index) const
{
	int result = 0;
	StoryText* that = const_cast<StoryText *>(this);
	bool lastWasPARSEP = true;
	index = qMin(index, (uint) that->length());
	for (uint i=0; i < index; ++i) {
		lastWasPARSEP = that->d->at(i)->ch == SpecialChars::PARSEP;
		if (lastWasPARSEP)
			++result;
	}
	return result;
}

uint StoryText::nrOfParagraphs() const
{
	uint result = 0;
	StoryText* that = const_cast<StoryText *>(this);
	bool lastWasPARSEP = true;
	for (int i=0; i < length(); ++i) {
		lastWasPARSEP = that->d->at(i)->ch == SpecialChars::PARSEP;
		if (lastWasPARSEP)
			++result;
	}
	return lastWasPARSEP ? result : result + 1;
}

int StoryText::startOfParagraph(uint index) const
{
	if (index == 0)
		return 0;

	StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i) {
		if (that->d->at(i)->ch == SpecialChars::PARSEP && ! --index)
			return i + 1;
	}
	return length();
}

int StoryText::endOfParagraph(uint index) const
{
	++index;
	StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i) {
		if (that->d->at(i)->ch == SpecialChars::PARSEP && ! --index)
			return i;
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
	while (pos < len  && wordBoundaries.indexOf(text(pos)) < 0)
		++pos;
	return pos < len ? pos + 1 : pos;
}
int StoryText::prevWord(int pos)
{
	pos = qMax(0, pos-1);
	while (pos > 0 && wordBoundaries.indexOf(text(pos)) < 0)
		--pos;
	return wordBoundaries.indexOf(text(pos)) < 0 ? pos + 1 : pos;
}
int StoryText::nextSentence(int pos)
{
	int len = length();
	pos = qMin(len, pos+1);
	while (pos < len && sentenceBoundaries.indexOf(text(pos)) < 0)
		++pos;
	return pos < len ? pos + 1 : pos;
}
int StoryText::prevSentence(int pos)
{
	pos = qMax(0, pos-1);
	while (pos > 0 && sentenceBoundaries.indexOf(text(pos)) < 0)
		--pos;
	return sentenceBoundaries.indexOf(text(pos)) < 0 ? pos + 1 : pos;
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


int StoryText::selectWord(int pos)
{
	//Double click in a frame to select a word

	int a = pos;
	while(a > 0)
	{
		if (text(a-1).isLetterOrNumber())
			--a;
		else
			break;
	}
	int b = pos;
	while(b < length())
	{
		if (text(b).isLetterOrNumber())
			++b;
		else
			break;
	}
	select(a, b - a);
	return a;
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

void StoryText::extendSelection(int oldPos, int newPos)
{
	if (selFirst <= selLast)
	{
		// have selection
		if (selLast == oldPos - 1)
		{
			selLast = newPos - 1;
			return;
		}
		else if (selFirst == oldPos)
		{
			selFirst = newPos;
			return;
		}
		// can't extend, fall through
	}
	// no previous selection
	if (newPos > oldPos)
	{
		selFirst = oldPos;
		selLast = newPos - 1;
	}
	else
	{
		selFirst = newPos;
		selLast = oldPos - 1;
	}
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
		else if (coord.x() <= ls.x + ls.width) // last line of paragraph?
			return ls.lastItem;
		else if (xpos < ls.x + 0.01 && maxx >= coord.x()) // check for empty line
			return ls.firstItem;
	}
	return qMax(lastFrameItem+1, firstFrameItem);
}


FRect StoryText::boundingBox(int pos, uint len) const
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
			if (ls.lastItem == pos && (item(pos)->effects() & ScStyle_SuppressSpace)  )
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
				double xpos = ls.x;
				for (int j = ls.firstItem; j < pos; ++j)
				{
					if (item(j)->ch == SpecialChars::OBJECT)
						xpos += (object(j)->gWidth + object(j)->lineWidth()) * item(j)->glyph.scaleH;
					else
						xpos += item(j)->glyph.wide();
				}
				double finalw = 1;
				if (item(pos)->ch == SpecialChars::OBJECT)
					finalw = (object(pos)->gWidth + object(pos)->lineWidth()) * item(pos)->glyph.scaleH;
				else
					finalw = item(pos)->glyph.wide();
				result.setRect(xpos, ls.y - ls.ascent, pos < length()? finalw : 1, ls.ascent + ls.descent);
			}
			return result;
		}
	}
	const ParagraphStyle& pstyle(paragraphStyle(qMin(pos, length()))); // rather the trailing style than a segfault.
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

void StoryText::saxx(SaxHandler& handler, const Xml_string& elemtag) const
{
	Xml_attr empty;
	Xml_attr pageno;
	pageno.insert("name", "pgno");
	Xml_attr pageco;
	pageco.insert("name", "pgco");

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
			curr == SpecialChars::PAGECOUNT ||
			curr.unicode() < 32 || 
			(0xd800 <= curr.unicode() && curr.unicode() < 0xe000) ||
			curr.unicode() == 0xfffe || curr.unicode() == 0xffff ||
			style != lastStyle)
		{
			// something new, write pending chars
			if  (i - lastPos > 0)
			{
				handler.chars(textWithSmartHyphens(lastPos, i-lastPos));
			}
			lastPos = i;
		}

		lastWasPar = (curr == SpecialChars::PARSEP);
		if (lastWasPar)
		{
			handler.end("span");
			handler.end("p");
			handler.begin("p", empty);
			paragraphStyle(i+1).saxx(handler);
			handler.begin("span", empty);
			lastStyle.erase();
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
		else if (curr == SpecialChars::PAGECOUNT)
		{
			handler.beginEnd("var", pageco);
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
		handler.chars(textWithSmartHyphens(lastPos, length()-lastPos));
	handler.end("span");
	handler.end("p");
	
//	if (!lastWasPar)
//		paragraphStyle(length()-1).saxx(handler);
	
	handler.end(elemtag);

}


class AppendText_body : public Action_body
{
public:	
	void chars(const Xml_string& txt)
	{
		QChar chr;
		int   lastPos = 0, len;
		StoryText* obj = this->dig->top<StoryText>();
		for (int i = 0; i < txt.length(); ++i)
		{
			chr = txt.at(i);
			if (chr == SpecialChars::SHYPHEN && i > 0)
			{
				int toInsert = i - lastPos;
				if (toInsert > 0)
					obj->insertChars(obj->length(), txt.mid(lastPos, toInsert));
				len = obj->length();
				ScText* lastItem = obj->item(len-1);
				// double SHY means user provided SHY, single SHY is automatic one
				if (lastItem->effects() & ScStyle_HyphenationPossible)
				{
					lastItem->setEffects(lastItem->effects() & ~ScStyle_HyphenationPossible);
					obj->insertChars(len, QString(chr));
				}
				else
				{
					lastItem->setEffects(lastItem->effects() | ScStyle_HyphenationPossible);
				}
				lastPos = i + 1;
			} 
		}
		if (lastPos < txt.length())
		{
			QString ins = (lastPos == 0) ? txt : txt.right(txt.length() - lastPos);
			len = obj->length();
			obj->insertChars(len, ins);
		}
	}
};

struct  AppendText : public MakeAction<AppendText_body> 
{};


class AppendSpecial_body : public Action_body
{
public:
	AppendSpecial_body(QChar sp) : chr(sp) {}
	
	void begin(const Xml_string& tag, Xml_attr attr)
	{
		StoryText* obj = this->dig->top<StoryText>();
		Xml_attr::iterator code = attr.find("code");
		Xml_attr::iterator name = attr.find("name");
		if (tag == "unicode" && code != attr.end())
			obj->insertChars(-1, QChar(parseUInt(Xml_data(code))));
		else if (tag == "var" && name != attr.end())
		{
			if (Xml_data(name) == "pgno")
				obj->insertChars(-1, SpecialChars::PAGENUMBER);
			else
				obj->insertChars(-1, SpecialChars::PAGECOUNT);
		}
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
	void end(const Xml_string& tag) // this could be a setter if we had StoryText::appendObject() ...
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
	void end(const Xml_string& tag) 
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
	
	void end(const Xml_string& tag) 
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
	
	void begin(const Xml_string& tag, Xml_attr attr)
	{
		if (tag == "p")
		{
			StoryText* story = this->dig->top<StoryText>();
//			qDebug(QString("startpar: %1->%2 %3->NULL").arg(lastPos).arg(story->length()).arg((ulong)lastStyle));
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
	
	void end(const Xml_string& tag) 
	{
		if (tag == ParagraphStyle::saxxDefaultElem)
		{
			if (lastStyle)
				delete lastStyle;
			lastStyle = this->dig->top<ParagraphStyle>(0);
//			qDebug(QString("endstyle: %1 %2 %3").arg("?").arg(lastPos).arg((ulong)lastStyle));
		}
		else if (tag == "p")
		{
			StoryText* story = this->dig->top<StoryText>();
			int len = story->length();
//			qDebug(QString("endpar: %1 %2 %3 %4").arg(len).arg(lastPos).arg((ulong)lastStyle).arg(lastStyle? lastStyle->parent() : QString()));
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
	
	void begin(const Xml_string& tag, Xml_attr attr)
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
	
	void end(const Xml_string& tag) 
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

void StoryText::desaxeRules(const Xml_string& prefixPattern, Digester& ruleset, Xml_string elemtag)
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
	ruleset.addRule(Digester::concat(spanPrefix, "var"), AppendSpecial());
	
	//PageItem::desaxeRules(storyPrefix, ruleset); argh, that would be recursive!
	ruleset.addRule(Digester::concat(spanPrefix, "item"), AppendInlineFrame() );
	
}
