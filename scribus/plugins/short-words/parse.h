/*! This is the Scribus Short Words plugin main mechanism.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>
with contributions by good people listed in AUTHORS file

This program is free software - see LICENSE file in the distribution
or documentation
*/

#ifndef _SCRIBUS_SHORTPARSE_H_
#define _SCRIBUS_SHORTPARSE_H_

#include "shortwords.h"

#include <scribus.h>
#include <qobject.h>

class Parse : public QObject
{
	Q_OBJECT

public:
	/** lightwiight constructor */
	Parse();
	/** nothing here to do */
	~Parse();

	/** count of the changes (one frame = one change) */
	uint modify;

	/** process one frame - base method! */
	void parseItem(PageItem *aFrame);
	/** selcted frames on the page */
	void parseSelection();
	/** one page - using actual page */
	void parsePage();
	/** one page */
	void parsePage(Page *page);
	/** all pages in the document */
	void parseAll();
};

#endif
