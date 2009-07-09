/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_KEYBOARDSHORTCUTS_H
#define PREFS_KEYBOARDSHORTCUTS_H

#include "ui_prefs_keyboardshortcutsbase.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_KeyboardShortcuts : public QWidget, Ui::Prefs_KeyboardShortcuts
{
	Q_OBJECT

	public:
		Prefs_KeyboardShortcuts(QWidget* parent=0);
		~Prefs_KeyboardShortcuts();
};

#endif // PREFS_KEYBOARDSHORTCUTS_H
