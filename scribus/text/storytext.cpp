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

//#include <QDebug>
#include <unicode/brkiter.h>

//FIXME: this include must go to sctextstruct.h !
#include <QList>
#include <cassert>  //added to make Fedora-5 happy
#include "fpoint.h"
#include "notesstyles.h"
#include "scfonts.h"
#include "scribusdoc.h"
#include "sctext_shared.h"
#include "selection.h"
#include "storytext.h"
//
#include "util.h"
#include "resourcecollection.h"
#include "desaxe/saxiohelper.h"
#include "desaxe/digester.h"
#include "desaxe/simple_actions.h"
#include "shapedtextcache.h"

using namespace icu;

StoryText::StoryText(ScribusDoc * doc_) : m_doc(doc_)
{
	if (doc_)
	{
		d = new ScText_Shared(&doc_->paragraphStyles());
		m_doc->paragraphStyles().connect(this, SLOT(invalidateAll()));
		m_doc->charStyles().connect(this, SLOT(invalidateAll()));
	}
	else
		d = new ScText_Shared(nullptr);

	d->selFirst = 0;
	d->selLast = -1;
	
	m_shapedTextCache = new ShapedTextCache();
	
	d->len = 0;
	invalidateAll();
}

StoryText::StoryText() : m_doc(nullptr)
{
	d = new ScText_Shared(nullptr);

	d->selFirst = 0;
	d->selLast = -1;
	m_shapedTextCache = nullptr;
}

StoryText::StoryText(const StoryText & other) : m_doc(other.m_doc)
{
	d = other.d;
	d->refs++;
	
	if (m_doc)
	{
		m_doc->paragraphStyles().connect(this, SLOT(invalidateAll()));
		m_doc->charStyles().connect(this, SLOT(invalidateAll()));
	}
	
	d->selFirst = 0;
	d->selLast = -1;
	m_shapedTextCache = nullptr;

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

bool StoryText::hasBulletOrNum() const
{
	if (!d)
		return false;

	bool lastWasPARSEP = true;
	for (int i = 0; i < length(); ++i)
	{
		lastWasPARSEP = (d->at(i)->ch == SpecialChars::PARSEP);
		if (!lastWasPARSEP)
			continue;
		const ParagraphStyle& paraStyle = paragraphStyle(i);
		if (paraStyle.hasBullet() || paraStyle.hasNum())
			return true;
	}

	const ParagraphStyle& trailingStyle = d->trailingStyle;
	if (trailingStyle.hasBullet() || trailingStyle.hasNum())
		return true;

	return false;
}

bool StoryText::hasTextMarks() const
{
	if (d)
		return (d->marksCount > 0);
	return false;
}

bool StoryText::marksCountChanged() const
{
	if (d)
		return d->marksCountChanged;
	return false;
}

void StoryText::resetMarksCountChanged()
{
	d->marksCountChanged = false;
}

void StoryText::setDoc(ScribusDoc *docin)
{
	if (m_doc)
	{
		m_doc->paragraphStyles().disconnect(this, SLOT(invalidateAll()));
		m_doc->charStyles().disconnect(this, SLOT(invalidateAll()));
	}

	m_doc = docin;

	if (m_doc)
	{
		m_doc->paragraphStyles().connect(this, SLOT(invalidateAll()));
		m_doc->charStyles().connect(this, SLOT(invalidateAll()));
	}
}

StoryText StoryText::copy() const
{
	StoryText result(m_doc);
	*(result.d) = *d;
	return result;
}

StoryText& StoryText::operator= (const StoryText & other)
{
	other.d->refs++;
	
	d->refs--;
	if (d->refs == 0)
	{
		clear();
		delete d;
	}
	
	if (m_doc)
	{
		m_doc->paragraphStyles().disconnect(this, SLOT(invalidateAll()));
		m_doc->charStyles().disconnect(this, SLOT(invalidateAll()));
	}
	
	m_doc = other.m_doc; 
	d = other.d;
	
	if (m_doc)
	{
		m_doc->paragraphStyles().connect(this, SLOT(invalidateAll()));
		m_doc->charStyles().connect(this, SLOT(invalidateAll()));
	}
	
	d->selFirst = 0;
	d->selLast = -1;

	invalidateLayout();
	return *this;
}

int StoryText::cursorPosition() const
{
	return d->cursorPosition;
}

void StoryText::setCursorPosition(int pos, bool relative)
{
	if (relative)
	{
		bool forward = (pos >= 0);
		pos += d->cursorPosition;
		if (isLowSurrogate(pos) && isHighSurrogate(pos - 1))
		{
			if (forward)
				pos += 1;
			else
				pos -= 1;
		}
	}
	d->cursorPosition = qMin((uint) qMax(pos, 0), d->len);
}

void StoryText::normalizeCursorPosition()
{
	d->cursorPosition = qMax((uint) 0, qMin(d->cursorPosition, d->len));
}

int StoryText::normalizedCursorPosition()
{
	return (int) qMax((uint) 0, qMin(d->cursorPosition, d->len));
}

void StoryText::moveCursorForward()
{
	BreakIterator* it = getGraphemeIterator();
	if (!it)
		return;

	it->setText((const UChar*) plainText().utf16());
	int pos = it->following(cursorPosition());
	if (pos != BreakIterator::DONE)
		setCursorPosition(pos);
}

void StoryText::moveCursorBackward()
{
	BreakIterator* it = getGraphemeIterator();
	if (!it)
		return;

	it->setText((const UChar*) plainText().utf16());
	int pos = it->preceding(cursorPosition());
	if (pos != BreakIterator::DONE)
		setCursorPosition(pos);
}

void StoryText::moveCursorLeft()
{
	if (paragraphStyle().direction() == ParagraphStyle::RTL)
		moveCursorForward();
	else
		moveCursorBackward();
}

void StoryText::moveCursorRight()
{
	if (paragraphStyle().direction() == ParagraphStyle::RTL)
		moveCursorBackward();
	else
		moveCursorForward();
}

void StoryText::moveCursorWordLeft()
{
	BreakIterator* it = getWordIterator();
	if (!it)
		return;
	it->setText((const UChar*) plainText().utf16());
	int pos = cursorPosition();
	if (paragraphStyle().direction() == ParagraphStyle::RTL)
	{
		pos = it->following(pos);
		if (pos != BreakIterator::DONE)
		{
			while (pos < length() && text(pos).isSpace())
				pos += 1;
		}
	}
	else
	{
		pos = cursorPosition();
		while (pos > 0 && text(pos - 1).isSpace())
			pos -= 1;
		pos = it->preceding(pos);
	}

	if (pos != BreakIterator::DONE)
		setCursorPosition(pos);
}

void StoryText::moveCursorWordRight()
{
	BreakIterator* it = getWordIterator();
	if (!it)
		return;

	it->setText((const UChar*) plainText().utf16());
	int pos = cursorPosition();
	if (paragraphStyle().direction() == ParagraphStyle::RTL)
	{
		pos = cursorPosition();
		while (pos > 0 && text(pos - 1).isSpace())
			pos -= 1;
		pos = it->preceding(pos);
	}
	else
	{
		pos = it->following(pos);
		if (pos != BreakIterator::DONE)
		{
			while (pos < length() && text(pos).isSpace())
				pos += 1;
		}
	}

	if (pos != BreakIterator::DONE)
		setCursorPosition(pos);
}

void StoryText::clear()
{
	d->selFirst = 0;
	d->selLast = -1;
	
	d->defaultStyle.erase();
	d->trailingStyle.erase();

	d->clear();
	d->len = 0;
	invalidateAll();
}

int StoryText::indexOf(const QString &str, int from, Qt::CaseSensitivity cs, int* pLen) const
{
	int foundIndex = -1;
	if (pLen)
		*pLen = 0;

	if (str.isEmpty() || (from < 0))
		return -1;

	QString qStr = str;
	if (cs == Qt::CaseInsensitive)
		qStr = qStr.toLower();
	QChar ch = qStr.at(0);

	int strLen   = qStr.length();
	int storyLen = length();
	if (cs == Qt::CaseSensitive)
	{
		int i = indexOf(ch, from, cs);
		while (i >= 0 && i < (int) d->len)
		{
			int index = 0;
			while ((index < strLen) && ((index + i) < storyLen))
			{
				if (qStr.at(index) != d->at(index + i)->ch)
					break;
				++index;
			}
			if (index == strLen)
			{
				foundIndex = i;
				if (pLen)
					*pLen = strLen;
				break;
			}
			i = indexOf(ch, i + 1, cs);
		}
	}
	else
	{
		bool qCharIsDiacritic, curCharIsDiacritic;
		int i = indexOf(ch, from, cs);
		while (i >= 0 && i < (int) d->len)
		{
			int index = 0;
			int diacriticsCounter = 0; //counter for diacritics
			while ((index < strLen) && ((index + i + diacriticsCounter) < storyLen))
			{
				const QChar &qChar = qStr.at(index);
				const QChar &curChar = d->at(index + diacriticsCounter + i)->ch;
				qCharIsDiacritic   = SpecialChars::isArabicModifierLetter(qChar.unicode()) | (qChar.category() == QChar::Mark_NonSpacing);
				curCharIsDiacritic = SpecialChars::isArabicModifierLetter(curChar.unicode()) | (curChar.category() == QChar::Mark_NonSpacing);
				if (qCharIsDiacritic || curCharIsDiacritic)
				{
					if (qCharIsDiacritic)
					{
						++index;
						--diacriticsCounter;
					}
					if (curCharIsDiacritic)
						++diacriticsCounter;
					continue;
				}
				if (qChar != curChar.toLower())
					break;
				++index;
			}
			if (index == strLen)
			{
				foundIndex = i;
				while ((index + i + diacriticsCounter) < storyLen)
				{
					const QChar &curChar = d->at(index + diacriticsCounter + i)->ch;
					if (!SpecialChars::isArabicModifierLetter(curChar.unicode()) && (curChar.category() != QChar::Mark_NonSpacing))
						break;
					++diacriticsCounter;
				}

				if (pLen)
					*pLen = strLen + diacriticsCounter;
				break;
			}
			i = indexOf(ch, i + 1, cs);
		}
	}
	return foundIndex;
}

int StoryText::indexOf(QChar ch, int from, Qt::CaseSensitivity cs) const
{
	int foundIndex = -1;
	int textLength = length();

	if (cs == Qt::CaseSensitive)
	{
		for (int i = from; i < textLength; ++i)
		{
			if (d->at(i)->ch == ch)
			{
				foundIndex = i;
				break;
			}
		}
	}
	else
	{
		for (int i = from; i < textLength; ++i)
		{
			if (d->at(i)->ch.toLower() == ch)
			{
				foundIndex = i;
				break;
			}
		}
	}
	return foundIndex;
}

void StoryText::insert(const StoryText& other, bool onlySelection)
{
	insert(d->cursorPosition, other, onlySelection);
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
	for (int i = otherStart; i < otherEnd; ++i)
	{
		if (other.charStyle(i) == cstyle 
			&& other.text(i) != SpecialChars::OBJECT
			&& other.text(i) != SpecialChars::PARSEP)
			continue;
		int len = i - cstyleStart;
		if (len > 0)
		{
			insertCharsWithSoftHyphens(pos, other.textWithSoftHyphens(cstyleStart, len));
			applyCharStyle(pos, len, otherDefault.charStyle());
			applyCharStyle(pos, len, cstyle);
			pos += len;
		}
		if (other.text(i) == SpecialChars::PARSEP)
		{
			insertChars(pos, SpecialChars::PARSEP);
			//#5845 : disable for now as it has nasty side effects when linking frames
			//applyStyle(pos, otherDefault);
			applyStyle(pos, other.paragraphStyle(i));
			cstyleStart = i+1;
			pos += 1;
		}
		else if (other.text(i) == SpecialChars::OBJECT)
		{
			insertChars(pos, SpecialChars::OBJECT);
			item(pos)->embedded = other.item(i)->embedded;
			item(pos)->mark = other.item(i)->mark;
			if (item(pos)->mark)
			{
				d->marksCount++;
				d->marksCountChanged = true;
			}
			applyCharStyle(pos, 1, other.charStyle(i));
			cstyleStart = i+1;
			pos += 1;
		}
		else
		{
			cstyle = other.charStyle(i);
			cstyleStart = i;
		}
	}
	int len = otherEnd - cstyleStart;
	if (len > 0)
	{
		insertCharsWithSoftHyphens(pos, other.textWithSoftHyphens(cstyleStart, len));
		applyCharStyle(pos, len, otherDefault.charStyle());
		applyCharStyle(pos, len, cstyle);
		pos += len;
		if (other.text(otherEnd-1) != SpecialChars::PARSEP)
		{
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
	ScText* it = item(pos);
	if (!it->parstyle)
	{
		it->parstyle = new ParagraphStyle(paragraphStyle(pos+1));
		it->parstyle->setContext( & d->pstyleContext);
		// #7432 : when inserting a paragraph separator, apply/erase the trailing Style
		if (pos >= signed(d->len - 1))
		{
			applyStyle(pos, d->trailingStyle);
			d->trailingStyle.erase();
		}
//		it->parstyle->setName("para"); // DON'T TRANSLATE
//		it->parstyle->charStyle().setName("cpara"); // DON'T TRANSLATE
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
	ScText* it = item(pos);
	if (it->parstyle) {
//		const CharStyle* oldP = & it->parstyle->charStyle();
//		const CharStyle* newP = & that->paragraphStyle(pos+1).charStyle();
//		d->replaceParentStyle(pos, oldP, newP);
		delete it->parstyle;
		it->parstyle = nullptr;
	}
	// demote this parsep so the assert code in replaceCharStyleContextInParagraph()
	// doesn't choke:
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

	uint oldMarksCount = d->marksCount;

	if ((pos == 0) && (len > 0) && (static_cast<int>(len) == length()))
	{
		int lastChar = length() - 1;
		while (lastChar > 0 && text(lastChar) == SpecialChars::PARSEP)
			--lastChar;
		d->orphanedCharStyle = charStyle(lastChar);
	}
	for (int i = pos + static_cast<int>(len) - 1; i >= pos; --i)
	{
		ScText *it = d->at(i);
		if (it->ch == SpecialChars::PARSEP)
			removeParSep(i);
		if ((it->ch == SpecialChars::OBJECT) && (it->mark != nullptr))
			d->marksCount--;
		d->takeAt(i);
		d->len--;
		delete it;
		// #9592 : adjust d->selFirst and d->selLast, those values have to be
		// consistent in functions such as select()
		if (i <= d->selLast)
			--d->selLast;
		if (i < d->selFirst)
			--d->selFirst;
		if (static_cast<uint>(i + 1) <= d->cursorPosition && d->cursorPosition > 0)
			d->cursorPosition -= 1;
	}

	if (oldMarksCount != d->marksCount)
		d->marksCountChanged = true;

	d->len = d->count();
	d->cursorPosition = qMin(d->cursorPosition, d->len);
	if (d->selFirst > d->selLast)
	{
		d->selFirst =  0;
		d->selLast  = -1;
	}
	invalidate(pos, length());
}

void StoryText::trim()
{
	if (length() < 2)
		return;
	int posCount = 0;
	int pos = static_cast<int>(length()) - 1;
	for ( int i = static_cast<int>(length()) - 1; i >= 0; --i )
	{
		ScText *it = d->at(i);
		if ((it->ch == SpecialChars::PARSEP) || (it->ch.isSpace()))
		{
			pos--;
			posCount++;
		}
		else
			break;
	}
	if (posCount > 0)
	{
		ParagraphStyle pst = paragraphStyle(pos);
		if (posCount > 1)
			removeChars(pos+1, posCount-1);
		applyStyle(pos, pst);
	}
}

void StoryText::insertChars(const QString& txt, bool applyNeighbourStyle) //, const CharStyle & charstyle)
{
	insertChars(d->cursorPosition, txt, applyNeighbourStyle);
}

void StoryText::insertChars(int pos, const QString& txt, bool applyNeighbourStyle) //, const CharStyle & charstyle)
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

	for (int i = 0; i < txt.length(); ++i)
	{
		ScText * item = new ScText(clone);
		item->ch= txt.at(i);
		item->setContext(cStyleContext);
		d->insert(pos + i, item);
		d->len++;
		if (item->ch == SpecialChars::PARSEP)
		{
//			qDebug() << QString("new PARSEP %2 at %1").arg(pos).arg(paragraphStyle(pos).name());
			insertParSep(pos + i);
		}
		if (d->cursorPosition >= static_cast<uint>(pos + i))
			d->cursorPosition += 1;
	}

	d->len = d->count();
	invalidate(pos, pos + txt.length());
}

void StoryText::insertCharsWithSoftHyphens(int pos, const QString& txt, bool applyNeighbourStyle)
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
		if (ch == SpecialChars::SHYPHEN && index > 0)
		{
			ScText* lastItem = this->item(index - 1);
			// qreal SHY means user provided SHY, single SHY is automatic one
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
			if (item->ch == SpecialChars::PARSEP)
				insertParSep(index);
			if (d->cursorPosition >= static_cast<uint>(index))
				d->cursorPosition += 1;
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

	uint oldMarksCount = d->marksCount;
	
	if (item->ch == SpecialChars::PARSEP)
		removeParSep(pos);
	if ((item->ch == SpecialChars::OBJECT) && (item->mark != nullptr))
		d->marksCount--;
	item->ch = ch;
	if (d->at(pos)->ch == SpecialChars::PARSEP)
		insertParSep(pos);

	if (oldMarksCount != d->marksCount)
		d->marksCountChanged = true;
	
	invalidate(pos, pos + 1);
}

void StoryText::replaceSelection(const QString& newText)
{
	if (selectionLength() <= 0)
		return;

	int selStart = startOfSelection();
	int selLength = selectionLength();

	int lengthDiff = newText.length() - selLength;
	if (lengthDiff == 0)
	{
		for (int i = 0; i < selLength; ++i)
			replaceChar(selStart + i, newText[i]);
	}
	else if (lengthDiff > 0)
	{
		for (int i = 0; i < selLength; ++i)
			replaceChar(selStart + i, newText[i]);
		for (int i = selLength; i < newText.length(); ++i)
			insertChars(selStart + i, newText.mid(i, 1), true);
	}
	else
	{
		for (int i = 0; i < newText.length(); ++i)
			replaceChar(selStart + i, newText[i]);
		removeChars(selStart + newText.length(), -lengthDiff);
	}

	deselectAll();
	if (newText.length() > 0)
		select(selStart, newText.length());
	setCursorPosition(selStart + newText.length());
}

int StoryText::replaceWord(int pos, QString newWord)
{
	int eoWord = pos;
	while (eoWord < length())
	{
		if (text(eoWord).isLetterOrNumber() || SpecialChars::isIgnorableCodePoint(text(eoWord).unicode()))
			++eoWord;
		else
			break;
	}
	QString word = text(pos, eoWord - pos);
	int lengthDiff = newWord.length() - word.length();
	if (lengthDiff == 0)
	{
		for (int j = 0; j < word.length(); ++j)
			replaceChar(pos + j, newWord[j]);
	}
	else if (lengthDiff > 0)
	{
		for (int j = 0; j < word.length(); ++j)
			replaceChar(pos + j, newWord[j]);
		for (int j = word.length(); j < newWord.length(); ++j)
			insertChars(pos + j, newWord.mid(j, 1), true);
	}
	else
	{
		for (int j = 0; j < newWord.length(); ++j)
			replaceChar(pos + j, newWord[j]);
		removeChars(pos + newWord.length(), -lengthDiff);
	}
	return lengthDiff;
}

void StoryText::hyphenateWord(int pos, uint len, const char* hyphens)
{
	assert(pos >= 0);
	assert(pos + signed(len) <= length());
	
//	QString dump("");
	for (int i=pos; i < pos+signed(len); ++i)
	{
//		dump += d->at(i)->ch;
		if (hyphens && hyphens[i-pos] & 1)
		{
			d->at(i)->setEffects(d->at(i)->effects() | ScStyle_HyphenationPossible);
//			dump += "-";
		}
		else {
			d->at(i)->setEffects(d->at(i)->effects() & ~ScStyle_HyphenationPossible);
		}
	}
//	qDebug() << QString("st: %1").arg(dump);
	invalidate(pos, pos + len);
}

void StoryText::insertObject(int ob)
{
	insertObject(d->cursorPosition, ob);
}

void StoryText::insertObject(int pos, int ob)
{
	if (pos < 0)
		pos += length()+1;

	insertChars(pos, SpecialChars::OBJECT);
	const_cast<StoryText *>(this)->d->at(pos)->embedded = ob;
	m_doc->FrameItems[ob]->isEmbedded = true;   // this might not be enough...
	m_doc->FrameItems[ob]->OwnPage = -1; // #10379: OwnPage is not meaningful for inline object
}

void StoryText::insertMark(Mark* mark, int pos)
{
	if (mark == nullptr)
		return;
	if (pos < 0)
		pos = d->cursorPosition;

	insertChars(pos, SpecialChars::OBJECT, false);
	d->at(pos)->mark = mark;
	if (mark)
	{
		d->marksCount++;
		d->marksCountChanged = true;
	}
}

void StoryText::replaceObject(int pos, int ob)
{
	if (pos < 0)
		pos += length()+1;

	replaceChar(pos, SpecialChars::OBJECT);
	const_cast<StoryText *>(this)->d->at(pos)->embedded = ob;
	m_doc->FrameItems[ob]->isEmbedded = true;   // this might not be enough...
	m_doc->FrameItems[ob]->OwnPage = -1; // #10379: OwnPage is not meaningful for inline object
}


int StoryText::length() const
{
	return d->len;
}

QString StoryText::plainText() const
{
	if (length() <= 0)
		return QString();

	QChar   ch;
	QString result;

	int len = length();
	result.reserve(len);

	StoryText* that(const_cast<StoryText*>(this));
	for (int i = 0; i < len; ++i) {
		ch = that->d->at(i)->ch;
		if (ch == SpecialChars::PARSEP)
			ch = QLatin1Char('\n');
		result += ch;
	}

	return result;
}
#if 0
QChar StoryText::text() const
{
	return text(d->cursorPosition);
}
#endif

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
	for (int i = pos; i < pos+signed(len); ++i)
		result += that->d->at(i)->ch;

	return result;
}


bool StoryText::isBlockStart(int pos) const 
{
	return pos == 0 || text(pos-1) == SpecialChars::PARSEP;
}


int StoryText::nextBlockStart(int pos) const
{
	int result = pos + 1;
	while (result < length() && !isBlockStart(result))
		++result;
	
	// lump empty (or small) paragraphs together
	while (result+1 < length() && isBlockStart(result+1))
		++result;
	
	return result;
}


InlineFrame StoryText::object(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);
	return InlineFrame(that->d->at(pos)->embedded);
}


bool StoryText::hasExpansionPoint(int pos) const
{
	return text(pos) == SpecialChars::PAGENUMBER || text(pos) == SpecialChars::PAGECOUNT || hasMark(pos);
}


ExpansionPoint StoryText::expansionPoint(int pos) const
{
	if (text(pos) == SpecialChars::PAGENUMBER)
		return ExpansionPoint(ExpansionPoint::PageNumber);
	if( text(pos) == SpecialChars::PAGECOUNT)
		return ExpansionPoint(ExpansionPoint::PageCount);
	if (hasMark(pos))
		return ExpansionPoint(mark(pos));
	return ExpansionPoint(ExpansionPoint::Invalid);
}


QString StoryText::sentence(int pos, int &posn)
{
	int sentencePos = qMax(0, prevSentence(pos));
	posn=sentencePos;
	int nextSentencePos = qMin(length(), endOfSentence(pos));
	return text(sentencePos, nextSentencePos-sentencePos);
}

QString StoryText::textWithSoftHyphens(int pos, uint len) const
{
	QString result;
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

bool StoryText::hasObject(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);
	if (that->d->at(pos)->ch == SpecialChars::OBJECT)
		return that->d->at(pos)->hasObject(m_doc);
	return false;
}


PageItem* StoryText::getItem(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);
	return that->d->at(pos)->getItem(m_doc);
}

int StoryText::findMark(const Mark* mrk, int startPos) const
{
	assert(startPos >= 0);

	int len = d->len;
	for (int i = startPos; i < len; ++i)
	{
		if (d->at(i)->ch != SpecialChars::OBJECT)
			continue;
		if (d->at(i)->hasMark(mrk))
			return i;
	}

	return -1;
}

bool StoryText::hasMark(int pos, const Mark* mrk) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);
	if (that->d->at(pos)->ch == SpecialChars::OBJECT)
		return that->d->at(pos)->hasMark(mrk);
	return false;
}

Mark* StoryText::mark(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);
	return that->d->at(pos)->mark;
}


void StoryText::applyMarkCharstyle(Mark* mrk, CharStyle& currStyle) const
{
	TextNote* note = mrk->getNotePtr();
	if (note == nullptr)
		return;
	
	NotesStyle* nStyle = note->notesStyle();
	Q_ASSERT(nStyle != nullptr);
	
	QString chsName = nStyle->marksChStyle();
	if (!chsName.isEmpty())
	{
		CharStyle marksStyle(m_doc->charStyle(chsName));
		if (!currStyle.equiv(marksStyle))
			currStyle.setParent(chsName);
	}
	
	StyleFlag s(currStyle.effects());
	if (mrk->isType(MARKNoteMasterType))
	{
		if (nStyle->isSuperscriptInMaster())
			s |= ScStyle_Superscript;
		else
			s &= ~ScStyle_Superscript;
	}
	else
	{
		if (nStyle->isSuperscriptInNote())
			s |= ScStyle_Superscript;
		else
			s &= ~ScStyle_Superscript;
	}
	if (s != currStyle.effects())
		currStyle.setFeatures(s.featureList());
}


void StoryText::replaceMark(int pos, Mark* mrk)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	ScText* textItem = this->d->at(pos);
	if (textItem->mark)
		this->d->marksCount--;
	this->d->at(pos)->mark = mrk;
	if (textItem->mark)
		this->d->marksCount++;

	// Set marksCountChanged unconditionnaly to force text relayout
	d->marksCountChanged = true;
}


bool StoryText::isHighSurrogate(int pos) const
{
	return pos >= 0 && pos < length() && text(pos).isHighSurrogate();
}

bool StoryText::isLowSurrogate(int pos) const
{
	return pos >= 0 && pos < length() && text(pos).isLowSurrogate();
}


LayoutFlags StoryText::flags(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	StoryText* that = const_cast<StoryText *>(this);
	return  static_cast<LayoutFlags>((*that->d->at(pos)).effects().value & ScStyle_NonUserStyles);
}

bool StoryText::hasFlag(int pos, LayoutFlags flags) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());
	assert((flags & ScStyle_UserStyles) == ScStyle_None);

	return (flags & d->at(pos)->effects().value) == flags;
}

void StoryText::setFlag(int pos, LayoutFlags flags)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());
	assert((flags & ScStyle_UserStyles) == ScStyle_None);

	d->at(pos)->setEffects(flags | d->at(pos)->effects().value);
}

void StoryText::clearFlag(int pos, LayoutFlags flags)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos < length());

	d->at(pos)->setEffects(~(flags & ScStyle_NonUserStyles) & d->at(pos)->effects().value);
}


const CharStyle & StoryText::charStyle() const
{
	return charStyle(d->cursorPosition);
}

const CharStyle & StoryText::charStyle(int pos) const
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	if (length() == 0)
	{
//		qDebug() << "storytext::charstyle: default";
		return d->orphanedCharStyle;
	}
	if (pos == length())
	{
		qDebug() << "storytext::charstyle: access at end of text %i" << pos;
		--pos;
	}
	
	if (text(pos) == SpecialChars::PARSEP)
		return paragraphStyle(pos).charStyle();
	
	StoryText* that = const_cast<StoryText *>(this);

	if (hasMark(pos))
	{
		Mark* mrk = mark(pos);
		applyMarkCharstyle(mrk, *that->d->at(pos)); // hack to keep note charstyles current
	}
	
	return dynamic_cast<const CharStyle &> (*that->d->at(pos));
}

const ParagraphStyle & StoryText::paragraphStyle() const
{
	return paragraphStyle(d->cursorPosition);
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
	
	while (pos < length() && that->d->at(pos)->ch != SpecialChars::PARSEP)
		++pos;

	if (pos >= length())
		return that->d->trailingStyle;
	if ( !that->d->at(pos)->parstyle )
	{
		ScText* current = that->d->at(pos);
		qDebug("inserting default parstyle at %i", pos);
		current->parstyle = new ParagraphStyle();
		current->parstyle->setContext( & d->pstyleContext);
//		current->parstyle->setName( "para(paragraphStyle)" ); // DON'T TRANSLATE
//		current->parstyle->charStyle().setName( "cpara(paragraphStyle)" ); // DON'T TRANSLATE
//		current->parstyle->charStyle().setContext( d->defaultStyle.charStyleContext());
	}
	else {
//		qDebug() << QString("using parstyle at %1").arg(pos);
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
//	d->defaultStyle.setName( "storydefault" ); // DON'T TRANSLATE
//	d->defaultStyle.charStyle().setName( "cstorydefault" ); // DON'T TRANSLATE
//	qDebug() << QString("defstyle %1 context %2 defcstyle %3 ccontext %4 newcontext %5")
//		   .arg((uint)&d->defaultStyle,16).arg((uint)oldPContext,16)
//		   .arg((uint)&d->defaultStyle.charStyle(),16).arg((uint)oldCContext,16)
//		   .arg((uint)d->defaultStyle.charStyle().context(),16);
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

//	int lastParStart = pos == 0? 0 : -1;
	ScText* itText;
	for (uint i=pos; i < pos+len; ++i)
	{
		itText = d->at(i);
		// #6165 : applying style on last character applies style on whole text on next open 
		/*if (itText->ch == SpecialChars::PARSEP && itText->parstyle != nullptr)
			itText->parstyle->charStyle().applyCharStyle(style);*/
		
		// Does not work well, do not reenable before checking #9337, #9376 and #9428
		// #9173 et. al.: move charstyle to parstyle if whole paragraph is affected
		/*if (itText->ch == SpecialChars::PARSEP && itText->parstyle != nullptr && lastParStart >= 0)
		{
			eraseCharStyle(lastParStart, i - lastParStart, style);
			itText->parstyle->charStyle().applyCharStyle(style);
			lastParStart = i + 1;
		}*/
		itText->applyCharStyle(style);
	}
	// Does not work well, do not reenable before checking #9337, #9376 and #9428
	/*if (pos + signed(len) == length() && lastParStart >= 0)
	{
		eraseCharStyle(lastParStart, length() - lastParStart, style);
		d->trailingStyle.charStyle().applyCharStyle(style);
	}*/
	
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
		// FIXME?? see #6165 : should we really erase charstyle of paragraph style??
		if (itText->ch == SpecialChars::PARSEP && itText->parstyle != nullptr)
			itText->parstyle->charStyle().eraseCharStyle(style);
		itText->eraseCharStyle(style);
	}
	// Does not work well, do not reenable before checking #9337, #9376 and #9428
	/*if (pos + signed(len) == length())
	{
		d->trailingStyle.charStyle().eraseCharStyle(style);
	}*/
	
	invalidate(pos, pos + len);
}

void StoryText::applyStyle(int pos, const ParagraphStyle& style, bool rmDirectFormatting)
{
	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	int i = pos;
	while (i < length() && d->at(i)->ch != SpecialChars::PARSEP)
		++i;

	if (i < length())
	{
		if (!d->at(i)->parstyle) {
			qDebug("PARSEP without style at pos %i", i);
			d->at(i)->parstyle = new ParagraphStyle();
			d->at(i)->parstyle->setContext( & d->pstyleContext);
//			d->at(i)->parstyle->setName( "para(applyStyle)" ); // DON'T TRANSLATE
//			d->at(i)->parstyle->charStyle().setName( "cpara(applyStyle)" ); // DON'T TRANSLATE
//			d->at(i)->parstyle->charStyle().setContext( d->defaultStyle.charStyleContext() );
		}
//		qDebug() << QString("applying parstyle %2 at %1 for %3").arg(i).arg(paragraphStyle(pos).name()).arg(pos);
		d->at(i)->parstyle->applyStyle(style);
	}
	else {
		// not happy about this but inserting a new PARSEP makes more trouble
//		qDebug() << QString("applying parstyle %1 as defaultstyle for %2").arg(paragraphStyle(pos).name()).arg(pos);
		d->trailingStyle.applyStyle(style);
	}
	if (rmDirectFormatting)
	{
		--i;
		while (i >= 0 && d->at(i)->ch != SpecialChars::PARSEP)
		{
			d->at(i)->eraseDirectFormatting();
			--i;
		}
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
	while (i < length() && d->at(i)->ch != SpecialChars::PARSEP)
		++i;

	if (i < length())
	{
		if (!d->at(i)->parstyle) {
			qDebug("PARSEP without style at pos %i", i);
			d->at(i)->parstyle = new ParagraphStyle();
			d->at(i)->parstyle->setContext( & d->pstyleContext);
//			d->at(i)->parstyle->setName( "para(eraseStyle)" ); // DON'T TRANSLATE
//			d->at(i)->parstyle->charStyle().setName( "cpara(eraseStyle)" ); // DON'T TRANSLATE
//			d->at(i)->parstyle->charStyle().setContext( d->defaultStyle.charStyleContext());
		}
		//		qDebug() << QString("applying parstyle %2 at %1 for %3").arg(i).arg(paragraphStyle(pos).name()).arg(pos);
		d->at(i)->parstyle->eraseStyle(style);
	}
	else {
		// not happy about this but inserting a new PARSEP makes more trouble
		//		qDebug() << QString("applying parstyle %1 as defaultstyle for %2").arg(paragraphStyle(pos).name()).arg(pos);
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
	for (uint i=pos; i < pos+len; ++i)
	{
		itText = d->at(i);
		// #6165 : applying style on last character applies style on whole text on next open 
		/*if (itText->ch == SpecialChars::PARSEP && itText->parstyle != nullptr)
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
		else if (hasObject(i))
			getItem(i)->getNamedResources(lists);
		else
			charStyle(i).getNamedResources(lists);
	}
}


void StoryText::replaceStyles(const QMap<QString,QString>& newNameForOld)
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
	for (int i=0; i < len; ++i)
	{
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

void StoryText::fixLegacyFormatting()
{
	if (length() == 0)
		return;

	for (int i = 0; i < length(); ++ i)
	{
		if (d->at(i)->ch == SpecialChars::PARSEP)
			fixLegacyFormatting(i);
	}
	fixLegacyFormatting( length() );
}

void StoryText::fixLegacyFormatting(int pos)
{
	if (length() == 0)
		return;

	if (pos < 0)
		pos += length();

	assert(pos >= 0);
	assert(pos <= length());

	int i = pos;
	while (i > 0 && d->at(i - 1)->ch != SpecialChars::PARSEP)
		--i;

	const ParagraphStyle& parStyle = this->paragraphStyle(pos);
	parStyle.validate();

	if (parStyle.hasParent())
	{
		int start = i;
		while ((i < length()) && (d->at(i)->ch != SpecialChars::PARSEP))
		{
			d->at(i)->validate();
			d->at(i)->eraseCharStyle( parStyle.charStyle() );
			++i;
		}
		invalidate(start, qMin(i + 1, length()));
	}
}

uint StoryText::nrOfParagraph() const
{
	return nrOfParagraph(d->cursorPosition);
}

uint StoryText::nrOfParagraph(int pos) const
{
	uint result = 0;
	StoryText* that = const_cast<StoryText *>(this);
	bool lastWasPARSEP = true;
	pos = qMin(pos, that->length());
	for (int i=0; i < pos; ++i)
	{
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
	for (int i=0; i < length(); ++i)
	{
		lastWasPARSEP = that->d->at(i)->ch == SpecialChars::PARSEP;
		if (lastWasPARSEP)
			++result;
	}
	return lastWasPARSEP ? result : result + 1;
}

int StoryText::startOfParagraph() const
{
	return startOfParagraph(nrOfParagraph());
}

int StoryText::startOfParagraph(uint index) const
{
	if (index == 0)
		return 0;

	StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i)
	{
		if (that->d->at(i)->ch == SpecialChars::PARSEP && ! --index)
			return i + 1;
	}
	return length();
}

int StoryText::endOfParagraph() const
{
	return endOfParagraph(nrOfParagraph());
}

int StoryText::endOfParagraph(uint index) const
{
	++index;
	StoryText* that = const_cast<StoryText *>(this);
	for (int i=0; i < length(); ++i)
	{
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

int StoryText::nextChar(int pos)
{
	if (pos < length())
		return pos + 1;
	return length();
}

int StoryText::prevChar(int pos)
{
	if (pos > 0)
		return pos - 1;
	return 0;
}

int StoryText::firstWord()
{
	int len = length();
	int pos = 0;

	while (pos < len)
	{
		if (text(pos).isLetter())
			break;
		++pos;
	}
	return pos;
}

int StoryText::nextWord(int pos)
{
	BreakIterator* it = getWordIterator();
	if (!it)
		return pos;

	it->setText((const UChar*) plainText().utf16());
	pos = it->following(pos);
	
	int len = length();
	while ((pos < len) && (text(pos).isSpace() || text(pos).isPunct()))
		++pos;
	return pos;
}

int StoryText::prevWord(int pos)
{
	BreakIterator* it = getWordIterator();
	if (!it)
		return pos;

	it->setText((const UChar*) plainText().utf16());
	pos = it->preceding(pos);
	return pos;
}

int StoryText::endOfWord(int pos) const
{
	BreakIterator* it = getWordIterator();
	if (!it)
		return pos;

	it->setText((const UChar*) plainText().utf16());
	pos = it->following(pos);
	return pos;
}

int StoryText::endOfSentence(int pos) const
{
	BreakIterator* it = getSentenceIterator();
	if (!it)
		return pos;

	it->setText((const UChar*) plainText().utf16());
	int end = it->following(pos);
	return end;
}

int StoryText::nextSentence(int pos)
{
	BreakIterator* it = getSentenceIterator();
	if (!it)
		return pos;

	it->setText((const UChar*) plainText().utf16());
	pos = it->following(pos);
	pos = it->next();
	return pos;
}

int StoryText::prevSentence(int pos)
{
	//we cannot go before the first position so just return it.
	if (pos == 0)
		return 0;
	BreakIterator* it = getSentenceIterator();
	if (!it)
		return pos;

	it->setText((const UChar*) plainText().utf16());
	pos = it->preceding(pos);
	return pos;
}
int StoryText::nextParagraph(int pos)
{
	int len = length();
	pos = qMin(len, pos + 1);
	while (pos < len && text(pos) != SpecialChars::PARSEP)
		++pos;
	return pos;
}
int StoryText::prevParagraph(int pos)
{
	pos = qMax(0, pos - 1);
	while (pos > 0 && text(pos) != SpecialChars::PARSEP)
		--pos;
	return pos;
}



// selection

int StoryText::startOfSelection() const
{
	return d->selFirst <= d->selLast? d->selFirst : 0;
}

int StoryText::endOfSelection() const
{
	return d->selFirst <= d->selLast? d->selLast + 1 : -1;
}

int StoryText::selectionLength() const
{
	//FIX ME - sometimes I saw values equal or greater than length of text
	int last = d->selLast;
	if (d->selFirst >= length())
		return 0;
	if (d->selLast >= length())
		last = length() -1;
	return d->selFirst <= last? last - d->selFirst + 1 : 0;
}

bool StoryText::isSelected() const
{
	return selectionLength() > 0;
}


bool StoryText::selected(int pos) const
{
	return (d->selFirst <= pos && pos <= d->selLast) 
//	       || (pos >= 0 && pos < length() && const_cast<StoryText*>(this)->d->at(pos)->cselect)
	;
}

QString StoryText::selectedText() const
{
	int selLen = selectionLength();
	if (selLen <= 0)
		return QString();
	return text(startOfSelection(), selLen);
}

int StoryText::selectWord(int pos)
{
	//Double click in a frame to select a word
	BreakIterator* it = getWordIterator();
	if (!it)
		return pos;
	it->setText((const UChar*) plainText().utf16());
	int start = it->preceding(pos + 1);
	int end = it->next();
	int wordLength = end - start;
	if (wordLength > 0)
	{
		select(start, wordLength);
		setCursorPosition(endOfSelection());
	}
	else
	{
		deselectAll();
		setCursorPosition(start);
	}
	return start;
}


void StoryText::select(int pos, int len, bool on)
{
	if (pos < 0)
		pos += length();
	if (len < 0)
		len = 0;

	assert( pos >= 0 );
	assert( pos + signed(len) <= length() );

//	qDebug("old selection: %d - %d", d->selFirst, d->selLast);

//	StoryText* that = const_cast<StoryText *>(this);
//	for (int i=pos; i < pos+signed(len); ++i)
//		that->at(i)->cselect = on;

	if (on) {
		// extend if possible
		if (selected(pos - 1))
			d->selLast = qMax(d->selLast, pos + len - 1);
		else if (selected(pos + len))
			d->selFirst = qMin(d->selFirst, pos);
		else {
			d->selFirst = pos;
			d->selLast = pos + len - 1;
		}
	}
	else {
		if (pos <= d->selFirst && d->selLast < pos + signed(len))
			deselectAll();
		// shrink
		else if (!selected(pos - 1) && selected(pos + len - 1))
			d->selFirst = pos + len;
		else if (selected(pos) && !selected(pos + len))
			d->selLast = pos - 1;
		else if (selected(pos) || selected(pos + len - 1))
			// Grr, deselection splits selection
			d->selLast = pos - 1;
	}

	fixSurrogateSelection();
	
//	qDebug("new selection: %d - %d", d->selFirst, d->selLast);
}

void StoryText::extendSelection(int oldPos, int newPos)
{
	if (d->selFirst <= d->selLast)
	{
		// have selection
		if (d->selLast == oldPos - 1)
		{
			d->selLast = newPos - 1;
			return;
		}
		if (d->selFirst == oldPos)
		{
			d->selFirst = newPos;
			return;
		}
		// can't extend, fall through
	}
	// no previous selection
	if (newPos > oldPos)
	{
		d->selFirst = oldPos;
		d->selLast = newPos - 1;
	}
	else
	{
		d->selFirst = newPos;
		d->selLast = oldPos - 1;
	}

	fixSurrogateSelection();
}


void StoryText::fixSurrogateSelection()
{
	if (isLowSurrogate(d->selFirst) && isHighSurrogate(d->selFirst - 1))
		d->selFirst -= 1;
	if (isHighSurrogate(d->selLast) && isLowSurrogate(d->selLast + 1))
		d->selLast += 1;
}

BreakIterator* StoryText::m_graphemeIterator = nullptr;

BreakIterator* StoryText::getGraphemeIterator()
{
	UErrorCode status = U_ZERO_ERROR;
	if (m_graphemeIterator == nullptr)
		m_graphemeIterator = BreakIterator::createCharacterInstance(Locale(), status);

	if (U_FAILURE(status))
	{
		delete m_graphemeIterator;
		m_graphemeIterator = nullptr;
	}

	return m_graphemeIterator;
}

BreakIterator* StoryText::m_wordIterator = nullptr;

BreakIterator* StoryText::getWordIterator()
{
	UErrorCode status = U_ZERO_ERROR;
	if (m_wordIterator == nullptr)
		m_wordIterator = BreakIterator::createWordInstance(Locale(), status);

	if (U_FAILURE(status))
	{
		delete m_wordIterator;
		m_wordIterator = nullptr;
	}
	return m_wordIterator;
}

BreakIterator* StoryText::m_sentenceIterator = nullptr;

BreakIterator* StoryText::getSentenceIterator()
{
	UErrorCode status = U_ZERO_ERROR;
	if (m_sentenceIterator == nullptr)
		m_sentenceIterator = BreakIterator::createSentenceInstance(Locale(), status);

	if (U_FAILURE(status))
	{
		delete m_sentenceIterator;
		m_sentenceIterator = nullptr;
	}

	return m_sentenceIterator;
}

BreakIterator* StoryText::m_lineIterator = nullptr;

BreakIterator* StoryText::getLineIterator()
{
	UErrorCode status = U_ZERO_ERROR;
	if (m_lineIterator == nullptr)
		m_lineIterator = BreakIterator::createLineInstance(Locale(), status);

	if (U_FAILURE(status))
	{
		delete m_lineIterator;
		m_lineIterator = nullptr;
	}

	return m_lineIterator;
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
	d->selFirst = 0;
	d->selLast = length() - 1;
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
	d->selFirst = 0;
	d->selLast = -1;
}

void StoryText::removeSelection()
{
//	qDebug("removeSelection: %d - %d", d->selFirst, d->selLast);
	if (d->selFirst > d->selLast)
		return;

	assert( d->selFirst >= 0 );
	assert( d->selLast < length() );

	removeChars(d->selFirst, d->selLast - d->selFirst+1);
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
	invalidate(0, length());
}

void StoryText::invalidate(int firstItem, int endItem)
{
	for (int i = firstItem; i < endItem; ++i)
	{
		ParagraphStyle* par = item(i)->parstyle;
		if (par)
			par->charStyleContext()->invalidate();
	}
	if (!signalsBlocked())
		emit changed(firstItem, endItem);
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

ScText*  StoryText::item(int itm)
{
	assert( itm < length() );
	return const_cast<StoryText *>(this)->d->at(itm);
}

const ScText*  StoryText::item(int itm) const
{
	assert( itm < length() );
	return const_cast<StoryText *>(this)->d->at(itm);
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
				handler.chars(textWithSoftHyphens(lastPos, i-lastPos));
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
		else if (this->hasObject(i))
		{
			getItem(i)->saxx(handler);
		}
		else if (hasMark(i))
		{
			Mark* mrk = mark(i);
			if ((m_doc->m_Selection->itemAt(0)->isNoteFrame() && mrk->isType(MARKNoteMasterType))
				|| (!m_doc->m_Selection->itemAt(0)->isTextFrame() && mrk->isType(MARKNoteFrameType))
			    || mrk->isType(MARKBullNumType))
				continue; //do not insert notes marks into text frames nad vice versa
			Xml_attr mark_attr;
			mark_attr.insert("label", mrk->label);
			mark_attr.insert("typ", QString::number((int )mrk->getType()));
//			if (!mrk->isType(MARKBullNumType))
//			{
//				mark_attr.insert("strtxt", mrk->getString());
//				ParagraphStyle pstyle = this->paragraphStyle(i);
//				mark_attr.insert("style_peoffset", QString::number(pstyle.parEffectOffset(),'f',2));
//				mark_attr.insert("style_peindent", QString::number(pstyle.parEffectIndent(),'f',2));
//				mark_attr.insert("style_pecharstyle", pstyle.peCharStyleName());
//				mark_attr.insert("style_hasbul", pstyle.hasBullet() ? "1" : "0");
//				mark_attr.insert("style_bulletstr", pstyle.bulletStr());
//				mark_attr.insert("style_hasnum", pstyle.hasNum() ? "1" : "0");
//				mark_attr.insert("style_numname", pstyle.numName());
//				mark_attr.insert("style_numformat", QString::number( (int)pstyle.numFormat()));
//				mark_attr.insert("style_numprefix", pstyle.numPrefix());
//				mark_attr.insert("style_numsuffix", pstyle.numSuffix());
//				mark_attr.insert("style_numlevel", QString::number(pstyle.numLevel()));
//				mark_attr.insert("style_numstart", QString::number(pstyle.numStart()));
//				mark_attr.insert("style_numrestart", QString::number((int) pstyle.numRestart()));
//				mark_attr.insert("style_numother", pstyle.numOther() ? "1" : "0");
//				mark_attr.insert("style_numhigher", pstyle.numHigher() ? "1" : "0");
//			}
			if (mrk->isType(MARK2ItemType) && (mrk->getItemPtr() != nullptr))
				mark_attr.insert("item", mrk->getItemPtr()->itemName());
			else if (mrk->isType(MARK2MarkType))
			{
				QString l;
				MarkType t;
				mrk->getMark(l, t);
				if (m_doc->getMark(l,t) != nullptr)
				{
					mark_attr.insert("mark_l", l);
					mark_attr.insert("mark_t", QString::number((int) t));
				}
			}
			else if (mrk->isType(MARKNoteMasterType))
			{
				TextNote * note = mrk->getNotePtr();
				assert(note != nullptr);
				mark_attr.insert("nStyle", note->notesStyle()->name());
				mark_attr.insert("note",note->saxedText());
				//store noteframe name for inserting into note if it is non-auto-removable
				if (note->noteMark() && note->noteMark()->getItemPtr() && !note->noteMark()->getItemPtr()->isAutoNoteFrame())
					mark_attr.insert("noteframe", note->noteMark()->getItemPtr()->getUName());
			}
			handler.beginEnd("mark", mark_attr);
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
		handler.chars(textWithSoftHyphens(lastPos, length()-lastPos));
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
				// qreal SHY means user provided SHY, single SHY is automatic one
				if (obj->hasFlag(len-1, ScLayout_HyphenationPossible))
				{
					obj->clearFlag(len-1, ScLayout_HyphenationPossible);
					obj->insertChars(len, QString(chr));
				}
				else
				{
					obj->setFlag(len-1, ScLayout_HyphenationPossible);
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
		// FIXME : currently inline objects are added to doc items when parsing
		// We have to remove them from object list and add them to the FrameItems list
		// before inserting them in story object
		QList<PageItem*> *items = obj->doc()->Items;
		if (items->count() > 0)
		{
			if (items->last() == obj) // try a fast path first
				items->removeLast();
			else
				items->removeAll(obj);
		}
		story->insertObject(-1, obj->doc()->addToInlineFrames(obj));
	}
};

struct AppendInlineFrame : public MakeAction<AppendInlineFrame_body>
{};

//marks support
class AppendMark_body : public Action_body
{
public:
	void begin(const Xml_string& tag, Xml_attr attr)
	{
		StoryText* story = this->dig->top<StoryText>();
		QString l = "";
		MarkType t = MARKNoType;
		
		Mark* mrk = nullptr;
		
		if (tag == "mark")
		{
			Xml_attr::iterator lIt = attr.find("label");
			Xml_attr::iterator tIt = attr.find("typ");
			//			Xml_attr::iterator nf_It = attr.find("noteframe");
			if (lIt != attr.end())
				l = Xml_data(lIt);
			if (tIt != attr.end())
				t = (MarkType) parseInt(Xml_data(tIt));
			if (t != MARKBullNumType)
			{
				ScribusDoc* doc  = this->dig->lookup<ScribusDoc>("<scribusdoc>");
				//				ParagraphStyle* pstyle = nullptr;
				if (t == MARKVariableTextType)
					mrk = doc->getMark(l,t);
				//			else if (t == MARKBullNumType)
				//			{
				//				mrk = (Mark*) new BulNumMark();
				//				Xml_attr::iterator str_It = attr.find("strtxt");
				//				mrk->setString(Xml_data(str_It));
				//				pstyle = new ParagraphStyle();
				//				Xml_attr::iterator ite;
				//				ite = attr.find("style_peoffset");
				//				if (ite != attr.end())
				//					pstyle->setParEffectOffset(parseDouble(Xml_data(ite)));
				//				ite = attr.find("style_peindent");
				//				if (ite != attr.end())
				//					pstyle->setParEffectIndent(parseDouble(Xml_data(ite)));
				//				ite = attr.find("style_pecharstyle");
				//				if (ite != attr.end())
				//					pstyle->setPeCharStyleName(Xml_data(ite));
				//				ite = attr.find("style_hasbul");
				//				if (ite != attr.end())
				//					pstyle->setHasBullet(parseInt(Xml_data(ite)) == 1);
				//				ite = attr.find("style_bulletstr");
				//				if (ite != attr.end())
				//					pstyle->setBulletStr(Xml_data(ite));
				//				ite = attr.find("style_hasnum");
				//				if (ite != attr.end())
				//					pstyle->setHasNum(parseInt(Xml_data(ite)) == 1);
				//				ite = attr.find("style_numname");
				//				if (ite != attr.end())
				//					pstyle->setNumName(Xml_data(ite));
				//				ite = attr.find("style_numformat");
				//				if (ite != attr.end())
				//					pstyle->setNumFormat(parseInt(Xml_data(ite)));
				//				ite = attr.find("style_numprefix");
				//				if (ite != attr.end())
				//					pstyle->setNumPrefix(Xml_data(ite));
				//				ite = attr.find("style_numsuffix");
				//				if (ite != attr.end())
				//					pstyle->setNumSuffix(Xml_data(ite));
				//				ite = attr.find("style_numlevel");
				//				if (ite != attr.end())
				//					pstyle->setNumLevel(parseInt(Xml_data(ite)));
				//				ite = attr.find("style_numstart");
				//				if (ite != attr.end())
				//					pstyle->setNumStart(parseInt(Xml_data(ite)));
				//				ite = attr.find("style_numrestart");
				//				if (ite != attr.end())
				//					pstyle->setNumRestart(parseInt(Xml_data(ite)));
				//				ite = attr.find("style_numother");
				//				if (ite != attr.end())
				//					pstyle->setNumOther(parseInt(Xml_data(ite)) == 1);
				//				ite = attr.find("style_numhigher");
				//				if (ite != attr.end())
				//					pstyle->setNumHigher(parseInt(Xml_data(ite)) == 1);
				//			}
				else
				{
					mrk = doc->newMark();
					getUniqueName(l,doc->marksLabelsList(t), "_");
					mrk->label = l;
					mrk->OwnPage = doc->currentPage()->pageNr();
					mrk->setType(t);
					Xml_attr::iterator iIt = attr.find("item");
					Xml_attr::iterator m_lIt = attr.find("mark_l");
					Xml_attr::iterator m_tIt = attr.find("mark_t");
					if (mrk->isType(MARK2ItemType) && (iIt != attr.end()))
					{
						PageItem* item = doc->getItemFromName(Xml_data(iIt));
						mrk->setItemPtr(item);
						if (item == nullptr)
							mrk->setString("0");
						else
							mrk->setString(doc->getSectionPageNumberForPageIndex(item->OwnPage));
						mrk->setItemName(Xml_data(iIt));
					}
					if (mrk->isType(MARK2MarkType) && (m_lIt != attr.end()) && (m_tIt != attr.end()))
					{
						Mark* targetMark = doc->getMark(Xml_data(m_lIt), (MarkType) parseInt(Xml_data(m_tIt)));
						mrk->setMark(targetMark);
						if (targetMark == nullptr)
							mrk->setString("0");
						else
							mrk->setString(doc->getSectionPageNumberForPageIndex(targetMark->OwnPage));
						mrk->setItemName(Xml_data(m_lIt));
					}
					if (mrk->isType(MARKNoteMasterType))
					{
						Xml_attr::iterator nIt = attr.find("note");
						Xml_attr::iterator nsIt = attr.find("nStyle");
						NotesStyle* NS;
						if (nsIt == attr.end())
							NS = doc->m_docNotesStylesList.at(0);
						else
							NS = doc->getNotesStyle(Xml_data(nsIt));
						TextNote* note = doc->newNote(NS);
						note->setMasterMark(mrk);
						if (nIt != attr.end())
							note->setSaxedText(Xml_data(nIt));
						//					if (!NS->isAutoRemoveEmptyNotesFrames() && (nf_It != attr.end()))
						//					{
						//						PageItem_NoteFrame* nF = (PageItem_NoteFrame*) doc->getItemFromName(Xml_data(nf_It));
						//						if (nF != nullptr)
						//							doc->m_Selection->itemAt(0)->asTextFrame()->setNoteFrame(nF);
						//					}
						mrk->setNotePtr(note);
						doc->setNotesChanged(true);
					}
					doc->newMark(mrk);
				}
				story->insertMark(mrk);
				//			if (pstyle != nullptr)
				//			{
				//				int i = story->cursorPosition() -1;
				//				if (story->item(i)->parstyle == nullptr) {
				//					story->item(i)->parstyle = new ParagraphStyle(*pstyle);
				//					story->item(i)->parstyle->setContext( &doc->paragraphStyles());
				//				}
				//				else
				//					story->item(i)->parstyle->applyStyle(*pstyle);
				//			}
			}
		}
	}
};

struct AppendMark : public MakeAction<AppendMark_body>
{};

/*
class ApplyStyle_body : public Action_body
{
public:
	void end(const Xml_string& tag) 
	{
		qDebug() << "storytext desaxe: apply style";
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
		qDebug() << "storytext desaxe: apply charstyle";
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
	Paragraph_body() : lastPos(0), numPara(0), lastStyle(nullptr)
	{}
	
	~Paragraph_body() 
	{
		delete lastStyle;
	}

	virtual void reset()
	{
		numPara = 0;
	}
	
	void begin(const Xml_string& tag, Xml_attr attr)
	{
		if (tag == "text-content")
		{
			numPara = 0;
		}
		else if (tag == "p")
		{
			StoryText* story = this->dig->top<StoryText>();
//			qDebug() << QString("startpar: %1->%2 %3->nullptr").arg(lastPos).arg(story->length()).arg((ulong)lastStyle);
			lastPos = story->length();
			if (numPara > 0) {
				story->insertChars(-1, SpecialChars::PARSEP);
				++lastPos;
			}
			delete lastStyle;
			lastStyle = nullptr;
		}
	}
	
	void end(const Xml_string& tag) 
	{
		if (tag == ParagraphStyle::saxxDefaultElem)
		{
			delete lastStyle;
			lastStyle = this->dig->top<ParagraphStyle>(0);
//			qDebug() << QString("endstyle: %1 %2 %3").arg("?").arg(lastPos).arg((ulong)lastStyle);
		}
		else if (tag == "p")
		{
			StoryText* story = this->dig->top<StoryText>();
			int len = story->length();
//			qDebug() << QString("endpar: %1 %2 %3 %4").arg(len).arg(lastPos).arg((ulong)lastStyle).arg(lastStyle? lastStyle->parent() : QString());
			if (len >= lastPos && lastStyle)
			{
				story->applyStyle(lastPos, *lastStyle);
			}
			++numPara;
		}
	}
private:
	int lastPos;
	int numPara;
	ParagraphStyle* lastStyle;
};

struct Paragraph : public MakeAction<Paragraph_body>
{
	Paragraph() {}
};


class SpanAction_body : public Action_body
{
public:
	SpanAction_body() : lastPos(0), lastStyle(nullptr)
	{}
	
	~SpanAction_body() 
	{
		delete lastStyle;
	}
	
	void begin(const Xml_string& tag, Xml_attr attr)
	{
//		qDebug() << QString("spanaction: begin %1").arg(tag);
		if (tag == "span")
		{
			StoryText* story = this->dig->top<StoryText>();
			lastPos = story->length();
			delete lastStyle;
			lastStyle = nullptr;
		}
	}
	
	void end(const Xml_string& tag) 
	{
		if (tag == CharStyle::saxxDefaultElem)
//			qDebug() << QString("spanaction: end %1").arg(tag);
		{
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
	SpanAction() {}
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
	ruleset.addRule(storyPrefix, paraAction ); // needed to initialize some variables
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
	ruleset.addRule(Digester::concat(spanPrefix, "mark"), AppendMark() );
	
}
