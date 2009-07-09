/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_GUIDES_H
#define PREFS_GUIDES_H

#include "ui_prefs_guidesbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_Guides : public QWidget, Ui::Prefs_Guides
{
	Q_OBJECT

	public:
		Prefs_Guides(QWidget* parent=0);
		~Prefs_Guides();
};

#endif // PREFS_GUIDES_H
