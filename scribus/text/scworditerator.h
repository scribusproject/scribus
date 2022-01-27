/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef SCWORDITERATOR_H
#define SCWORDITERATOR_H

#include <QString>
#include <unicode/uversion.h>

#include "scribusapi.h"
#include "storytext.h"

U_NAMESPACE_BEGIN
class BreakIterator;
class UnicodeString;
U_NAMESPACE_END

class SCRIBUS_API ScWordIterator
{
public:
	ScWordIterator(StoryText& story);
	~ScWordIterator();
	
	int firstWord();
	int nextWord(int pos);
	int prevWord(int pos);
	int endOfWord(int pos);

	QString word(int pos);
	
private:
	icu::UnicodeString* m_unicodeString { nullptr };
	icu::BreakIterator* m_wordIterator { nullptr };
	StoryText& m_story;
	
	icu::BreakIterator* getWordIterator();
};

#endif
