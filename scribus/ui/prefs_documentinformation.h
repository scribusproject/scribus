/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_DOCUMENTINFORMATION_H
#define PREFS_DOCUMENTINFORMATION_H

#include "ui_prefs_documentinformationbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_DocumentInformation : public Prefs_Pane, Ui::Prefs_DocumentInformation
{
	Q_OBJECT

	public:
		Prefs_DocumentInformation(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_DocumentInformation();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
};

#endif // PREFS_DOCUMENTINFORMATION_H
