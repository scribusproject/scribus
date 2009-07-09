/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_ITEMTOOLS_H
#define PREFS_ITEMTOOLS_H

#include "ui_prefs_itemtoolsbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_ItemTools : public QWidget, Ui::Prefs_ItemTools
{
	Q_OBJECT

	public:
		Prefs_ItemTools(QWidget* parent=0);
		~Prefs_ItemTools();
};

#endif // PREFS_ITEMTOOLS_H
