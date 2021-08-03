/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "notesstyles.h"
#include "marks.h"
#include "textnote.h"
#include "util.h"

void TextNote::clearSaxedText()
{
	m_noteSaxedText.clear();
	textLen = 0;
}

QString TextNote::numString() const
{
	return notesStyle()->numString(m_number);
}
