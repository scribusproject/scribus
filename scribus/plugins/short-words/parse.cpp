/*! This is the Scribus Short Words plugin main mechanism.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributions by good people listed in AUTHORS file

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include "shortwords.h"
#include "version.h"

#include <scribus.h>
#include <qregexp.h>

extern ScribusApp *ScApp;
extern ShortWords *shortWords;

Parse::Parse()
{
	modify = 0;
}

Parse::~Parse()
{
}

void Parse::parseItem(PageItem *aFrame)
{
	// the content of the frame - text itself
	QString content = QString();
	uint changes = 0;
	// language of the frame
	QString lang;
	// list of the short words
	QStringList shorts;
	// text with special space
	QString unbreak;
	// the regexp
	QRegExp rx(" ");

	// just textframes processed
	if (aFrame->PType != 4)
		return;

	// an ugly hack to get the language code from the item language property
	lang = aFrame->Language;
	if (ScApp->Sprachen.contains(lang))
		lang = shortWords->cfg->getLangCodeFromHyph(ScApp->Sprachen[lang]);
	// apply spaces after shorts
	shorts = shortWords->cfg->getShortWords(lang);
	if (shorts.count()==0)
		return; // no changes

	// get text from frame
	for (uint i=0; i<aFrame->MaxChars; i++)
		content += aFrame->itemText.at(i)->ch;
	changes = content.contains(UNBREAKABLE_SPACE);
	for (QStringList::Iterator it = shorts.begin(); it != shorts.end(); ++it)
	{
		unbreak = (*it);
		// replace ' ' from cfg with '~'
		unbreak = unbreak.replace(SPACE, UNBREAKABLE_SPACE);
		//looking for pattern with word boundaries and more chars
		// replace hell needed too to remove regexp special chars
		rx.setPattern("(\\b)" + (*it).replace("*", "\\*").replace("+", "\\+").replace("-", "\\-").replace("$", "\\$").replace(".","\\.") + "(\\b)");
		// replacement loop
		int pos = 0;
		while ( pos >= 0 )
		{
			pos = rx.search(content, pos);
			if ( pos >= 0 )
			{
				QString s;
				//replace + keep word boundaries
				content.replace(rx, rx.cap(1) + unbreak + rx.cap(2));
				pos  += rx.matchedLength();
			}
		}
	}
	// retrun text into frame
	for (uint i=0; i<aFrame->MaxChars; i++)
		aFrame->itemText.at(i)->ch = content.at(i);
	if (content.contains(UNBREAKABLE_SPACE) > changes)
		++modify;
} // end of method

void Parse::parseSelection()
{
	uint cnt = ScApp->doc->currentPage->SelItem.count();
	ScApp->FProg->setTotalSteps(cnt);
	for (uint i=0; i < cnt; i++)
	{
		ScApp->FProg->setProgress(i);
		parseItem(ScApp->doc->currentPage->SelItem.at(i));
	} // for items
	ScApp->FProg->setProgress(cnt);
}


void Parse::parsePage()
{
	parsePage(ScApp->doc->currentPage);
}

void Parse::parsePage(Page *page)
{
	uint cnt = page->Items.count();
	ScApp->FProg->setTotalSteps(cnt);
	ScApp->view->GotoPage(page->PageNr);
	for (uint i=0; i<cnt; i++)
	{
		ScApp->FProg->setProgress(i);
		parseItem(page->Items.at(i));
	} // for items
	ScApp->FProg->setProgress(cnt);
}

void Parse::parseAll()
{
	for (uint i=0; i < ScApp->view->Pages.count(); i++)
		parsePage(ScApp->view->Pages.at(i));
}
