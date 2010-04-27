/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_TYPOGRAPHY_H
#define PREFS_TYPOGRAPHY_H

#include "ui_prefs_typographybase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Typography : public Prefs_Pane, Ui::Prefs_Typography
{
	Q_OBJECT

	public:
		Prefs_Typography(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Typography();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
};

#endif // PREFS_TYPOGRAPHY_H
