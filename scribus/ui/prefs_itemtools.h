/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_ITEMTOOLS_H
#define PREFS_ITEMTOOLS_H

#include "ui_prefs_itemtoolsbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_ItemTools : public Prefs_Pane, Ui::Prefs_ItemTools
{
	Q_OBJECT

	public:
		Prefs_ItemTools(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_ItemTools();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		void unitChange(int newIndex);
		void enableFontPreview(bool);

	protected slots:
		void enableSignals(bool on);
		void updateFontPreview();
		void toggleImagesScalingChain();
		void imageHorizontalScalingChange();
		void imageVerticalScalingChange();

	protected:
		ScribusDoc* m_doc;
		bool showFontPreview;

};

#endif // PREFS_ITEMTOOLS_H
