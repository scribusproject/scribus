/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_DOCUMENTSECTIONS_H
#define PREFS_DOCUMENTSECTIONS_H

#include "ui_prefs_documentsectionsbase.h"
#include "pagestructs.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_DocumentSections : public Prefs_Pane, Ui::Prefs_DocumentSections
{
	Q_OBJECT

	public:
		Prefs_DocumentSections(QWidget* parent, ScribusDoc* doc = nullptr);
		~Prefs_DocumentSections() = default;

		void restoreDefaults(struct ApplicationPrefs *prefsData) override;
		void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const override;

	public slots:
		void languageChange();

	protected:
		DocumentSectionMap m_localSections;
		ScribusDoc* m_doc { nullptr };
		uint m_maxPageIndex { 0 };
		virtual void updateTable();

	protected slots:
		virtual void tableItemChanged(int, int);
		virtual void addEntry();
		virtual void deleteEntry();
};

#endif // PREFS_DOCUMENTSECTIONS_H
