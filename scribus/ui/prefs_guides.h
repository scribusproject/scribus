/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_GUIDES_H
#define PREFS_GUIDES_H

#include "ui_prefs_guidesbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Guides : public Prefs_Pane, Ui::Prefs_Guides
{
	Q_OBJECT

	public:
		Prefs_Guides(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Guides();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		void unitChange(int);

	protected slots:
		virtual void changeMajorColor();
		virtual void changeMinorColor();
		virtual void changeBaselineColor();
		virtual void changeGuideColor();
		virtual void changeMarginColor();

	protected:
		QColor colorGuides;
		QColor colorMargin;
		QColor colorMajorGrid;
		QColor colorMinorGrid;
		QColor colorBaselineGrid;
};

#endif // PREFS_GUIDES_H
