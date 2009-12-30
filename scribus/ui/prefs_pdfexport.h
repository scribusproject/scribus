/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_PDFEXPORT_H
#define PREFS_PDFEXPORT_H

#include "ui_prefs_pdfexportbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_PDFExport : public Prefs_Pane, Ui::Prefs_PDFExport
{
	Q_OBJECT

	public:
		Prefs_PDFExport(QWidget* parent=0);
		~Prefs_PDFExport();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		void unitChange(int);

	protected slots:
		void enableRangeControls(bool);
		void enableSecurityControls(bool);
		void createPageNumberRange();

	protected:
		bool cmsEnabled;
		ScribusDoc* m_doc;
};

#endif // PREFS_PDFEXPORT_H
