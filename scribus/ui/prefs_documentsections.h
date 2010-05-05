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
		Prefs_DocumentSections(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_DocumentSections();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();

	protected:
		DocumentSectionMap localSections;
		uint m_maxpageindex;
		QStringList styles;
		ScribusDoc* m_doc;
		virtual void updateTable();

	protected slots:
		virtual void tableItemChanged(int, int);
		virtual void addEntry();
		virtual void deleteEntry();
};

#endif // PREFS_DOCUMENTSECTIONS_H
