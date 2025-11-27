/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_EXTERNALTOOLS_H
#define PREFS_EXTERNALTOOLS_H

#include "ui_prefs_externaltoolsbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API Prefs_ExternalTools : public Prefs_Pane, Ui::Prefs_ExternalTools
{
	Q_OBJECT

	public:
		Prefs_ExternalTools(QWidget* parent, ScribusDoc* doc = nullptr);
		~Prefs_ExternalTools() = default;

		void restoreDefaults(struct ApplicationPrefs *prefsData) override;
		void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const override;

	public slots:
		void languageChange();

	protected:
		void insertConfigItem(const QString& config, int row = -1);
		void setConfigItemText(QListWidgetItem *item);

		QMap<QString, QString> commands;

	protected slots:
		void changePostScriptTool();
		void changeImageTool();
		void changeWebBrowser();
		void changePDFViewer();
		void changeUniconvertor();
		void changeLatexEditor();
		void rescanForTools();
		void upButtonPressed();
		void downButtonPressed();
		void addConfig();
		void deleteConfig();
		void changeLatexPath();

};

#endif // PREFS_EXTERNALTOOLS_H
