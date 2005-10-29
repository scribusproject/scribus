/* This is the Scribus Short Words plugin main mechanism.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributors.

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include "shortwords.h"
#include "parse.h"
#include "parse.moc"
#include "version.h"
#include "configuration.h"

#include "scribus.h"
#include "page.h"
#include "pageitem.h"
#include <qregexp.h>

extern ScribusApp SCRIBUS_API *ScApp;


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
	if (ScApp->Sprachen.contains(lang))
		lang = cfg->getLangCodeFromHyph(ScApp->Sprachen[lang]);
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
	uint cnt = ScApp->view->SelItem.count();
	ScApp->mainWindowProgressBar->setTotalSteps(cnt);
	for (uint i=0; i < cnt; ++i)
	{
		ScApp->mainWindowProgressBar->setProgress(i);
		parseItem(ScApp->view->SelItem.at(i));
	} // for items
	ScApp->mainWindowProgressBar->setProgress(cnt);
}


void SWParse::parsePage()
{
	parsePage(ScApp->doc->currentPageNumber());
}

void SWParse::parsePage(int page)
{
	uint cnt = 0;
	uint docItemsCount=ScApp->doc->Items->count();
	for (uint a = 0; a < docItemsCount; ++a)
	{
		PageItem* b = ScApp->doc->Items->at(a);
		if (b->OwnPage == page)
			++cnt;
	}
	ScApp->mainWindowProgressBar->setTotalSteps(cnt);
	ScApp->view->GotoPage(page);
	uint i = 0;
	for (uint a = 0; a < docItemsCount; ++a)
	{
		PageItem* b = ScApp->doc->Items->at(a);
		if (b->OwnPage == page)
		{
			ScApp->mainWindowProgressBar->setProgress(++i);
			parseItem(b);
		}
	}
	ScApp->mainWindowProgressBar->setProgress(cnt);
}

void SWParse::parseAll()
{
	for (uint i=0; i < ScApp->doc->Pages->count(); ++i)
		parsePage(i);
}
