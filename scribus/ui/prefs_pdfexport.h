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

#include "pdfoptions.h"
class ScribusDoc;

class SCRIBUS_API Prefs_PDFExport : public Prefs_Pane, Ui::Prefs_PDFExport
{
	Q_OBJECT

	public:
		Prefs_PDFExport(QWidget* parent=0);
		~Prefs_PDFExport();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData, ScribusDoc* doc, const ProfilesL & PDFXProfiles,
									 const QMap<QString, int> & DocFonts);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	signals:
		void noInfo();
		void hasInfo();

	public slots:
		void languageChange();
		void unitChange(int);

	protected slots:
		void enableRangeControls(bool);
		void enableSecurityControls(bool);
		void createPageNumberRange();
		void setMaximumResolution();
		void enableProfiles(int);
		void enableLPI(int);
		void enablePG();
		void enablePGI();
		void enablePGI2();
		void enablePDFX(int);

	protected:
		void setCustomRenderingWidgetsShown(bool);
		void setSolidsImagesWidgetsShown(bool);
		void enableCustomRenderingWidgets(bool);
		void enableSolidsImagesWidgets(bool);
		void enablePDFXWidgets(bool);
		bool cmsEnabled;
		ScribusDoc* m_doc;
		QString defaultSolidColorRGBProfile;
		QString defaultPrinterProfile;
		PDFOptions Opts;
};

#endif // PREFS_PDFEXPORT_H
