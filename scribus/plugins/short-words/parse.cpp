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

#include <qregexp.h>

#include "shortwords.h"
#include "parse.h"
#include "parse.moc"
#include "version.h"
#include "configuration.h"

#include "scribus.h"
#include "page.h"
#include "pageitem.h"
#include "selection.h"

extern ScribusMainWindow SCRIBUS_API *ScMW;


SWParse::SWParse()
{
	modify = 0;
}

void SWParse::parseItem(PageItem *aFrame)
{
	// the content of the frame - text itself
	QString content = QString();
	int changes = 0;
	// language of the frame
	QString lang;
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
	lang = aFrame->Language;
	if (ScMW->Sprachen.contains(lang))
		lang = cfg->getLangCodeFromHyph(ScMW->Sprachen[lang]);
	// apply spaces after shorts
	shorts = cfg->getShortWords(lang);
	if (shorts.count()==0)
		return; // no changes

	// get text from frame
	for (uint i=0; i<aFrame->MaxChars; ++i)
		content += aFrame->itemText.at(i)->ch;
	changes = content.contains(UNBREAKABLE_SPACE);

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
	// retrun text into frame
	for (uint i=0; i<aFrame->MaxChars; ++i)
		aFrame->itemText.at(i)->ch = content.at(i);
	if (content.contains(UNBREAKABLE_SPACE) > changes)
		++modify;

	delete(cfg);
} // end of method

void SWParse::parseSelection()
{
	//uint docSelectionCount = ScMW->view->SelItem.count();
	uint docSelectionCount = ScMW->doc->selection->count();
	ScMW->mainWindowProgressBar->setTotalSteps(docSelectionCount);
	for (uint i=0; i < docSelectionCount; ++i)
	{
		ScMW->mainWindowProgressBar->setProgress(i);
		//parseItem(ScMW->view->SelItem.at(i));
		parseItem(ScMW->doc->selection->itemAt(i));
	} // for items
	ScMW->mainWindowProgressBar->setProgress(docSelectionCount);
}


void SWParse::parsePage()
{
	parsePage(ScMW->doc->currentPageNumber());
}

void SWParse::parsePage(int page)
{
	uint cnt = 0;
	uint docItemsCount=ScMW->doc->Items->count();
	for (uint a = 0; a < docItemsCount; ++a)
	{
		PageItem* b = ScMW->doc->Items->at(a);
		if (b->OwnPage == page)
			++cnt;
	}
	ScMW->mainWindowProgressBar->setTotalSteps(cnt);
	ScMW->view->GotoPage(page);
	uint i = 0;
	for (uint a = 0; a < docItemsCount; ++a)
	{
		PageItem* b = ScMW->doc->Items->at(a);
		if (b->OwnPage == page)
		{
			ScMW->mainWindowProgressBar->setProgress(++i);
			parseItem(b);
		}
	}
	ScMW->mainWindowProgressBar->setProgress(cnt);
}

void SWParse::parseAll()
{
	for (uint i=0; i < ScMW->doc->Pages->count(); ++i)
		parsePage(i);
}
