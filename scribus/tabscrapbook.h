/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABSCRAPBOOK_H
#define TABSCRAPBOOK_H

#include "ui_tabscrapbook.h"
#include "scribusapi.h"

struct ApplicationPrefs;
/*! \brief Scrapbook panel for preferences dialog.
This class is inherited from UI base class.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API TabScrapbook : public QWidget, public Ui::TabScrapbook
{
	Q_OBJECT

	public:
		TabScrapbook(QWidget* parent = 0);
		~TabScrapbook(){};
		void restoreDefaults(struct ApplicationPrefs *prefsData);
};

#endif
