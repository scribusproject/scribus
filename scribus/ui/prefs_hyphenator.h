/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_HYPHENATOR_H
#define PREFS_HYPHENATOR_H

#include "ui_prefs_hyphenatorbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Hyphenator : public Prefs_Pane, Ui::Prefs_Hyphenator
{
	Q_OBJECT

	public:
		Prefs_Hyphenator(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Hyphenator();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();

	protected slots:
		void addToIgnoreList();
		void editIgnoreListEntry();
		void removeIgnoreListEntry();
		void enableIgnoreButtons();
		void addToExceptList();
		void editExceptListEntry();
		void removeExceptListEntry();
		void enableExceptButtons();

};

#endif // PREFS_HYPHENATOR_H
