/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H


#include <QListWidgetItem>
#include <QMap>
#include <QStackedWidget>
#include <QStringList>

#include "prefsstructs.h"
#include "scribusapi.h"
#include "ui/scdialog.h"
#include "ui_preferencesdialogbase.h"

#include "ui/preferences/prefs_colormanagement.h"
#include "ui/preferences/prefs_display.h"
#include "ui/preferences/prefs_documentinformation.h"
#include "ui/preferences/prefs_documentitemattributes.h"
#include "ui/preferences/prefs_documentsections.h"
#include "ui/preferences/prefs_documentsetup.h"
#include "ui/preferences/prefs_experimental.h"
#include "ui/preferences/prefs_externaltools.h"
#include "ui/preferences/prefs_fonts.h"
#include "ui/preferences/prefs_guides.h"
#include "ui/preferences/prefs_hyphenator.h"
#include "ui/preferences/prefs_imagecache.h"
#include "ui/preferences/prefs_indexes.h"
#include "ui/preferences/prefs_itemtools.h"
#include "ui/preferences/prefs_keyboardshortcuts.h"
#include "ui/preferences/prefs_miscellaneous.h"
#include "ui/preferences/prefs_operatortools.h"
#include "ui/preferences/prefs_pagesizes.h"
#include "ui/preferences/prefs_paths.h"
#include "ui/preferences/prefs_pdfexport.h"
#include "ui/preferences/prefs_plugins.h"
#include "ui/preferences/prefs_preflightverifier.h"
#include "ui/preferences/prefs_printer.h"
#include "ui/preferences/prefs_scrapbook.h"
#include "ui/preferences/prefs_spelling.h"
#include "ui/preferences/prefs_tableofcontents.h"
#include "ui/preferences/prefs_typography.h"
#include "ui/preferences/prefs_userinterface.h"

class Prefs_Pane;
class ScribusMainWindow;
class ScribusDoc;


/*! \brief The Scribus Preferences Dialog
*/
class SCRIBUS_API PreferencesDialog : public ScDialog, Ui::PreferencesDialog
{
	Q_OBJECT

	public:
		PreferencesDialog(QWidget* parent, ApplicationPrefs& prefsData, ScribusDoc* doc = nullptr);
		~PreferencesDialog() = default;

		ApplicationPrefs prefs() const { return localPrefs; }
		void getResizeDocumentPages(bool &resizePages, bool &resizeMasterPages, bool &resizePageMargins, bool &resizeMasterPageMargins);

	public slots:
		/// \brief Overridden to emit accepted(), which plugin panels use
		void accept() override;
		void itemSelected(QListWidgetItem* ic);
		void newItemSelected();
		void restoreDefaults();
		void setupGui();
		void saveGuiToPrefs();
		void setNewItemSelected(const QString &s);
		void changeUnits(int u);

	protected slots:
		virtual void languageChange();
		void addWidgetsToStack();

	protected:
		void createStackWidgetList();
		void addWidget(Prefs_Pane* tab);
		void initPreferenceValues();
		/*! \brief Scans plugins for those that want to add a prefs widget and
		hooks them up to the dialog. */
		void addPluginsToStackWidgetList();

		void changeEvent(QEvent *e) override;

		Prefs_ColorManagement *prefs_ColorManagement {nullptr};
		Prefs_Display *prefs_Display {nullptr};
		Prefs_DocumentInformation *prefs_DocumentInformation {nullptr};
		Prefs_DocumentItemAttributes *prefs_DocumentItemAttributes {nullptr};
		Prefs_DocumentSections *prefs_DocumentSections {nullptr};
		Prefs_DocumentSetup *prefs_DocumentSetup {nullptr};
		Prefs_Experimental *prefs_Experimental {nullptr};
		Prefs_ExternalTools *prefs_ExternalTools {nullptr};
		Prefs_Fonts *prefs_Fonts {nullptr};
		Prefs_Guides *prefs_Guides {nullptr};
		Prefs_Hyphenator *prefs_Hyphenator {nullptr};
		Prefs_ImageCache *prefs_ImageCache {nullptr};
		Prefs_Indexes *prefs_Indexes {nullptr};
		Prefs_ItemTools *prefs_ItemTools {nullptr};
		Prefs_KeyboardShortcuts *prefs_KeyboardShortcuts {nullptr};
		Prefs_Miscellaneous *prefs_Miscellaneous {nullptr};
		Prefs_OperatorTools *prefs_OperatorTools {nullptr};
		Prefs_PDFExport *prefs_PDFExport {nullptr};
		Prefs_PageSizes *prefs_PageSizes {nullptr};
		Prefs_Paths *prefs_Paths {nullptr};
		Prefs_Plugins *prefs_Plugins {nullptr};
		Prefs_PreflightVerifier *prefs_PreflightVerifier {nullptr};
		Prefs_Printer *prefs_Printer {nullptr};
		Prefs_Scrapbook *prefs_Scrapbook {nullptr};
		//Prefs_Spelling *prefs_Spelling {nullptr};
		Prefs_TableOfContents *prefs_TableOfContents {nullptr};
		Prefs_Typography *prefs_Typography {nullptr};
		Prefs_UserInterface *prefs_UserInterface {nullptr};

		QMap<QListWidgetItem*, int> stackWidgetMap;
		double unitRatio {0.0};
		int docUnitIndex {0};
		ScribusMainWindow* mainWin {nullptr};
		ScribusDoc* m_Doc {nullptr};
		ApplicationPrefs localPrefs;

		struct PrefsTabData
		{
			Prefs_Pane* prefsPane {nullptr};
			bool forPrefs {true};
			bool forDoc {true};
			bool visible {true};
			QString prefsPaneName;
		};
		QList<PrefsTabData> stackDataList;
};

#endif



