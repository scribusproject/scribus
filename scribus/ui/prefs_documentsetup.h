/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_DOCUMENTSETUP_H
#define PREFS_DOCUMENTSETUP_H

#include "ui_prefs_documentsetupbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_DocumentSetup : public Prefs_Pane, Ui::Prefs_DocumentSetup
{
	Q_OBJECT

	public:
		Prefs_DocumentSetup(QWidget* parent=0);
		~Prefs_DocumentSetup();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();

	private:
		void unitChange();
		double unitRatio;
		int choosenLayout;
		double pageW;
		double pageH;
		QString prefsPageSizeName;
};

#endif // PREFS_DOCUMENTSETUP_H

