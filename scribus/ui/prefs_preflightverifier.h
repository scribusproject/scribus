/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_PREFLIGHTVERIFIER_H
#define PREFS_PREFLIGHTVERIFIER_H

#include <QString>


#include "ui_prefs_preflightverifierbase.h"
#include "prefs_pane.h"
#include "prefsstructs.h"
#include "scribusapi.h"

class SCRIBUS_API Prefs_PreflightVerifier : public Prefs_Pane, Ui::Prefs_PreflightVerifier
{
	Q_OBJECT

	public:
		Prefs_PreflightVerifier(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_PreflightVerifier();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;
	public slots:
		void languageChange();



	protected slots:
		void putProfile();
		void setProfile(const QString& name);
		void addProf();
		void delProf();

	protected:
		QString tempNewProfileName;
		CheckerPrefsList checkerProfile;
		QString currentProfile;
		void updateProfile(const QString& name);
};

#endif // PREFS_PREFLIGHTVERIFIER_H
