/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_SCRIPTER_H
#define PREFS_SCRIPTER_H

#include "ui_prefs_scripterbase.h"
#include "ui/prefs_pane.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_Scripter : public Prefs_Pane, Ui::Prefs_Scripter
{
	Q_OBJECT

	public:
		Prefs_Scripter(QWidget* parent=0);
		~Prefs_Scripter();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		//! \brief Apply changes to prefs. Auto connected.
		void apply();
};

#endif // PREFS_SCRIPTER_H
