/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include <unicode/brkiter.h>
#include "scworditerator.h"

using namespace icu;

ScWordIterator::ScWordIterator(StoryText& story)
	: m_story(story)
{
	m_wordIterator = getWordIterator();
}

ScWordIterator::~ScWordIterator()
{
	if (m_wordIterator)
	{
		delete m_wordIterator;
		m_wordIterator = nullptr;
	}
	if (m_unicodeString)
	{
		delete m_unicodeString;
		m_unicodeString = nullptr;
	}
}

BreakIterator* ScWordIterator::getWordIterator()
{
	if (m_wordIterator != nullptr)
		return m_wordIterator;
	
	UErrorCode status = U_ZERO_ERROR;
	m_wordIterator = BreakIterator::createWordInstance(Locale(), status);
	if (U_FAILURE(status))
	{
		delete m_wordIterator;
		m_wordIterator = nullptr;
		return  nullptr;
	}
	
	if (m_unicodeString)
	{
		delete m_unicodeString;
		m_unicodeString = nullptr;
	}
	m_unicodeString = new icu::UnicodeString((const UChar*) m_story.plainText().utf16());
	m_wordIterator->setText(*m_unicodeString);
	return m_wordIterator;
}

int ScWordIterator::firstWord()
{
	int len = m_story.length();
	int pos = m_wordIterator->first();

	while (pos < len)
	{
		if (m_story.text(pos).isLetter())
			break;
		++pos;
	}
	return pos;
}

int ScWordIterator::nextWord(int pos)
{
	if (!m_wordIterator)
		return pos;
	pos = m_wordIterator->following(pos);
	
	int len = m_story.length();
	while ((pos < len) && (m_story.text(pos).isSpace() || m_story.text(pos).isPunct()))
		++pos;
	return pos;
}

int ScWordIterator::prevWord(int pos)
{
	if (!m_wordIterator)
		return pos;
	pos = m_wordIterator->preceding(pos);
	return pos;
}

int ScWordIterator::endOfWord(int pos)
{
	if (!m_wordIterator)
		return pos;
	pos = m_wordIterator->following(pos);
	return pos;
}

QString ScWordIterator::word(int pos)
{
	if (m_wordIterator)
	{
		int end = m_wordIterator->following(pos);
		return m_story.text(pos, end - pos);
	}

	// Fallback to old code
	int eoWord = pos;
	int length =  m_story.length();
	while (eoWord < length)
	{
		QChar qChar = m_story.text(eoWord);
		if (qChar.isLetterOrNumber() || SpecialChars::isIgnorableCodePoint(qChar.unicode()))
			++eoWord;
		else
			break;
	}
	return m_story.text(pos, eoWord - pos);
}