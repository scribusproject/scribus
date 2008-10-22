/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This is the Scribus Short Words plugin main mechanism.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributors.

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include <QRegExp>

#include "shortwords.h"
#include "parse.h"
#include "version.h"
#include "configuration.h"

#include "scribus.h"
#include "page.h"
#include "pageitem.h"
#include "selection.h"

SWParse::SWParse()
{
	modify = 0;
}

void SWParse::parseItem(PageItem *aFrame)
{
	// the content of the frame - text itself
	QString content = QString();
	int changes = 0;
	// list of the short words
	QStringList shorts;
	// text with special space
	QString unbreak;
	// the regexp
	QRegExp rx(" ");
	// cfg
	SWConfig *cfg = new SWConfig();

	// just textframes processed
	if (!aFrame->asTextFrame())
		return;

	// an ugly hack to get the language code from the item language property
	if (lang.isNull() || lang.isEmpty())
	{
		lang = aFrame->itemText.charStyle(0).language();
		if (lang.isNull() || lang.isEmpty())
			qDebug("SWParse::parseItem - variable lang is still empty. No changes are made.");
	}

	QString langCode;
	if (aFrame->doc()->scMW()->Sprachen.contains(lang))
		langCode = cfg->getLangCodeFromHyph(aFrame->doc()->scMW()->Sprachen[lang]);

	// apply spaces after shorts
	shorts = cfg->getShortWords(langCode);
	if (shorts.count()==0)
		return; // no changes

	// get text from frame
	int i;
	for (i=0; i < aFrame->itemText.length() && ! aFrame->frameDisplays(i); ++i)
		;
	for (; i < aFrame->itemText.length() && aFrame->frameDisplays(i); ++i)
		content += aFrame->itemText.text(i,1);
	changes = content.count(UNBREAKABLE_SPACE);

	// for every config string, replace its spaces by nbsp's.
	for (QStringList::Iterator it = shorts.begin(); it != shorts.end(); ++it)
	{
		unbreak = (*it);
		// replace ' ' from cfg with '~' in the replacement string
		unbreak = unbreak.replace(SPACE, UNBREAKABLE_SPACE);
		/*
		Regexp used to find the config string (*it) in content.
		Cheat sheet:
		- \b is a "word boundary"; it matches at a *position*
		not a *character*
		- \W is a "non-word character"; it matches every character
		that is neither a letter, nor a number, nor '_';
		for example, it matches all kind of whitespace
		(including carriage return) and punctuation
		Example occurrences when (*it) == "Mr ":
			- "Mr Bla etc." : there's one of the word boundaries
			of the word "Mr" before the pattern, and one of the
			word boundaries of the word "Bla" after.
		Example occurrences when (*it) == " !":
			- "ugly hack ! No." : there's a word boundary before,
			and a whitespace is matched by \W after.
			- "» !" : '«' is matched by \W before, newline is
			matched by \W after.
		*/
		rx.setPattern("(\\b|\\W)" + rx.escape(*it) + "(\\b|\\W)");
		/*
		QString::replace works on the whole string in one pass.
		On every occurrence of our regexp, \1 and \2 are replaced
		by what has been matched (captured characters) in,
		respectively, the first and second capturing parentheses.
		*/
		content.replace(rx, "\\1" + unbreak + "\\2");
	}
	// return text into frame
	for (i=0; i < aFrame->itemText.length() && ! aFrame->frameDisplays(i); ++i)
		;
	for (; i < aFrame->itemText.length() && aFrame->frameDisplays(i); ++i)
		aFrame->itemText.replaceChar(i, content.at(i));
	if (content.count(UNBREAKABLE_SPACE) > changes)
		++modify;

	delete(cfg);
} // end of method

void SWParse::parseSelection(ScribusDoc* doc)
{
	uint docSelectionCount = doc->m_Selection->count();
	if (docSelectionCount == 0)
		return;
	doc->scMW()->mainWindowProgressBar->setMaximum(docSelectionCount);
	for (uint i=0; i < docSelectionCount; ++i)
	{
	doc->scMW()->mainWindowProgressBar->setValue(i);
		parseItem(doc->m_Selection->itemAt(i));
	} // for items
	doc->scMW()->mainWindowProgressBar->setValue(docSelectionCount);
}


void SWParse::parsePage(ScribusDoc* doc)
{
	parsePage(doc, doc->currentPageNumber());
}

void SWParse::parsePage(ScribusDoc* doc, int page)
{
	uint cnt = 0;
	uint docItemsCount=doc->Items->count();
	if (docItemsCount == 0)
		return;

	for (uint a = 0; a < docItemsCount; ++a)
	{
		PageItem* b = doc->Items->at(a);
		if (b->OwnPage == page)
			++cnt;
	}
	doc->scMW()->mainWindowProgressBar->setMaximum(cnt);
	doc->view()->GotoPage(page);
	uint i = 0;
	for (uint a = 0; a < docItemsCount; ++a)
	{
		PageItem* b = doc->Items->at(a);
		if (b->OwnPage == page)
		{
			doc->scMW()->mainWindowProgressBar->setValue(++i);
			parseItem(b);
		}
	}
	doc->scMW()->mainWindowProgressBar->setValue(cnt);
}

void SWParse::parseAll(ScribusDoc* doc)
{
	for (int i=0; i < doc->Pages->count(); ++i)
		parsePage(doc, i);
}
