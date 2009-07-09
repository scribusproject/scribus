/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_FONTS_H
#define PREFS_FONTS_H

#include "ui_prefs_fontsbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_Fonts : public QWidget, Ui::Prefs_Fonts
{
	Q_OBJECT

	public:
		Prefs_Fonts(QWidget* parent=0);
		~Prefs_Fonts();
};

#endif // PREFS_FONTS_H
