/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_EXPERIMENTAL_H
#define PREFS_EXPERIMENTAL_H

#include "ui_prefs_experimentalbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Experimental : public Prefs_Pane, Ui::Prefs_Experimental
{
	Q_OBJECT

	public:
		Prefs_Experimental(QWidget* parent, ScribusDoc* doc = nullptr);
		~Prefs_Experimental();

		void restoreDefaults(struct ApplicationPrefs *prefsData) override;
		void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const override;

	public slots:
		void languageChange();
};

#endif // PREFS_EXPERIMENTAL_H
