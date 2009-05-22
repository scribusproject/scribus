/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef HELPNAVIGATION_H
#define HELPNAVIGATION_H

#include "ui_helpbrowsernavigation.h"

/**
	HelpNavigation encapsulates the tools associated with the help browser in order
	to allow more flexible UI redesign in this place - thinks dockwidget/sidebar/etc.
  */
class HelpNavigation : public QWidget, public Ui::HelpBrowserNavigation
{
	public:
	HelpNavigation(QWidget * parent);
	~HelpNavigation();
};

#endif // HELPNAVIGATION_H
