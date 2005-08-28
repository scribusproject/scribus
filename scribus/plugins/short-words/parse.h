#ifndef _SCRIBUS_SHORTPARSE_H_
#define _SCRIBUS_SHORTPARSE_H_

#include "shortwords.h"

#include "scribus.h"
#include <qobject.h>

/*! \brief This is the Scribus Short Words plugin main mechanism.

It walks trough the user specified objects and apply the spaces.
This code is based on the Scribus-Vlna plug in rewritten for
international use.

\author Petr Vanek <petr@yarpen.cz>
with contributions by good people listed in AUTHORS file

This program is free software - see LICENSE file in the distribution
or documentation
 */
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
	void parsePage(int page);
	/** all pages in the document */
	void parseAll();
};

#endif
