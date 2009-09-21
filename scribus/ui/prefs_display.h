/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_DISPLAY_H
#define PREFS_DISPLAY_H

#include "ui_prefs_displaybase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_Display : public QWidget, Ui::Prefs_Display
{
	Q_OBJECT

	public:
		Prefs_Display(QWidget* parent=0);
		~Prefs_Display();
		void restoreDefaults(struct ApplicationPrefs *prefsData);

	protected:
		int docUnitIndex;
};

#endif // PREFS_DISPLAY_H
