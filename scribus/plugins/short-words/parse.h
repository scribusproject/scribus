/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _SCRIBUS_SHORTPARSE_H_
#define _SCRIBUS_SHORTPARSE_H_

#include "shortwords.h"

#include <qobject.h>

class PageItem;

/*! \brief This is the Scribus Short Words plugin main mechanism.

It walks trough the user specified objects and apply the spaces.
This code is based on the Scribus-Vlna plug in rewritten for
international use.

\author Petr Vanek <petr@yarpen.cz> with contributors.
*/
class SWParse : public QObject
{
	Q_OBJECT

public:
	/*! \brief lightweight constructor */
	SWParse();
	/*! \brief nothing here to do */
	~SWParse(){};

	/*! \brief count of the changes (one frame = one change) */
	uint modify;

	/*! \brief process one frame - base method!
	Replacing strings via regular expressions. See detailed
	description in the code of this method.
	\param aFrame pointer to the existing text frame */
	void parseItem(PageItem *aFrame);
	/*! \brief selcted frames on the page */
	void parseSelection();
	/*! \brief one page - using actual page */
	void parsePage();
	/*! \brief one page
	\param page page number*/
	void parsePage(int page);
	/*! \brief all pages in the document */
	void parseAll();
};

#endif
