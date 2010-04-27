/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_SCRAPBOOK_H
#define PREFS_SCRAPBOOK_H

#include "ui_prefs_scrapbookbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Scrapbook : public Prefs_Pane, Ui::Prefs_Scrapbook
{
	Q_OBJECT

	public:
		Prefs_Scrapbook(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Scrapbook();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
};

#endif // PREFS_SCRAPBOOK_H
