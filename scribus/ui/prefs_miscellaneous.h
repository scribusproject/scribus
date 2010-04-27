/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_MISCELLANEOUS_H
#define PREFS_MISCELLANEOUS_H

#include "ui_prefs_miscellaneousbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Miscellaneous : public Prefs_Pane, Ui::Prefs_Miscellaneous
{
	Q_OBJECT

	public:
		Prefs_Miscellaneous(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Miscellaneous();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
};

#endif // PREFS_MISCELLANEOUS_H
