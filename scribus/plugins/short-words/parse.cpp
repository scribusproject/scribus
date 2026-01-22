/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This is the Scribus Short Words plugin main mechanism.

This code is based on the Scribus-Vlna plugin rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributors.

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include <memory>

#include <QProgressBar>
#include <QRegularExpression>

#include "parse.h"

#include "configuration.h"
#include "langmgr.h"
#include "pageitem.h"
#include "scpage.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "shortwords.h"
#include "version.h"


SWParse::SWParse() = default;

void SWParse::parseItem(PageItem *aFrame)
{
	// just textframes processed
	if (!aFrame->isTextFrame())
		return;

	// the content of the frame - text itself
	QString content;
	int changes = 0;
	// text with special space
	QString unbreak;
	// the regexp
	QRegularExpression rx(" ");
	// cfg
	auto cfg = std::make_unique<SWConfig>();

	// use character unicode properties when matching expression
	auto patternOptions = rx.patternOptions();
	patternOptions |= QRegularExpression::UseUnicodePropertiesOption;
	rx.setPatternOptions(patternOptions);

	// an ugly hack to get the language code from the item language property
	if (lang.isEmpty())
	{
		lang = aFrame->itemText.charStyle(0).language();
		if (lang.isEmpty())
			qDebug("SWParse::parseItem - variable lang is still empty. No changes are made.");
	}

	// apply spaces after shorts
	QStringList shorts = cfg->getShortWords(lang);
	if (shorts.isEmpty())
		return; // no changes

	// get text from frame
	int i;
	for (i = 0; i < aFrame->itemText.length() && !aFrame->frameDisplays(i); ++i)
		;
	for (; i < aFrame->itemText.length() && aFrame->frameDisplays(i); ++i)
		content += aFrame->itemText.text(i,1);
	changes = content.count(SpecialChars::NBSPACE);

	// for every config string, replace its spaces by nbsp's.
	for (const QString& shortWord : std::as_const(shorts))
	{
		unbreak = shortWord;
		// replace ' ' from cfg with '~' in the replacement string
		unbreak.replace(SPACE, SpecialChars::NBSPACE);
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
		rx.setPattern("(\\b|\\W)" + QRegularExpression::escape(shortWord) + "(\\b|\\W)");
		/*
		QString::replace works on the whole string in one pass.
		On every occurrence of our regexp, \1 and \2 are replaced
		by what has been matched (captured characters) in,
		respectively, the first and second capturing parentheses.
		*/
		content.replace(rx, "\\1" + unbreak + "\\2");
	}
	// return text into frame
	for (i = 0; i < aFrame->itemText.length() && !aFrame->frameDisplays(i); ++i)
		;
	for (; i < aFrame->itemText.length() && aFrame->frameDisplays(i); ++i)
		aFrame->itemText.replaceChar(i, content.at(i));
	if (content.count(SpecialChars::NBSPACE) > changes)
		++modify;
} // end of method

void SWParse::parseSelection(ScribusDoc* doc)
{
	int docSelectionCount = doc->m_Selection->count();
	if (docSelectionCount == 0)
		return;
	doc->scMW()->mainWindowProgressBar->setMaximum(docSelectionCount);
	for (int i = 0; i < docSelectionCount; ++i)
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
	int count = 0;
	int docItemsCount = doc->Items->count();
	if (docItemsCount == 0)
		return;

	for (int i = 0; i < docItemsCount; ++i)
	{
		const PageItem* pi = doc->Items->at(i);
		if (pi->OwnPage == page)
			++count;
	}
	doc->scMW()->mainWindowProgressBar->setMaximum(count);
	doc->view()->GotoPage(page);
	int j = 0;
	for (int i = 0; i < docItemsCount; ++i)
	{
		PageItem* pi = doc->Items->at(i);
		if (pi->OwnPage == page)
		{
			doc->scMW()->mainWindowProgressBar->setValue(++j);
			parseItem(pi);
		}
	}
	doc->scMW()->mainWindowProgressBar->setValue(count);
}

void SWParse::parseAll(ScribusDoc* doc)
{
	for (int i = 0; i < doc->Pages->count(); ++i)
		parsePage(doc, i);
}
