/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H


#include <QListWidgetItem>
#include <QDialogButtonBox>
#include <QMap>
#include <QStackedWidget>

#include "scribusapi.h"
#include "prefsstructs.h"
#include "ui_preferencesdialogbase.h"

#include "ui/prefs_colormanagement.h"
#include "ui/prefs_display.h"
#include "ui/prefs_documentinformation.h"
#include "ui/prefs_documentitemattributes.h"
#include "ui/prefs_documentsections.h"
#include "ui/prefs_documentsetup.h"
#include "ui/prefs_experimental.h"
#include "ui/prefs_externaltools.h"
#include "ui/prefs_fonts.h"
#include "ui/prefs_guides.h"
#include "ui/prefs_hyphenator.h"
#include "ui/prefs_imagecache.h"
#include "ui/prefs_itemtools.h"
#include "ui/prefs_keyboardshortcuts.h"
#include "ui/prefs_miscellaneous.h"
#include "ui/prefs_operatortools.h"
#include "ui/prefs_pagesizes.h"
#include "ui/prefs_paths.h"
#include "ui/prefs_pdfexport.h"
#include "ui/prefs_plugins.h"
#include "ui/prefs_preflightverifier.h"
#include "ui/prefs_printer.h"
#include "ui/prefs_scrapbook.h"
#include "ui/prefs_spelling.h"
#include "ui/prefs_tableofcontents.h"
#include "ui/prefs_typography.h"
#include "ui/prefs_userinterface.h"

class Prefs_Pane;
class ScribusMainWindow;
class ScribusDoc;


/*! \brief The Scribus Preferences Dialog
*/
class SCRIBUS_API PreferencesDialog : public QDialog, Ui::PreferencesDialog
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

	protected:
		void addWidget(Prefs_Pane* tab);
		void setupListWidget();
		void arrangeIcons();
		void initPreferenceValues();
		/*! \brief Scans plugins for those that want to add a prefs widget and
		hooks them up to the dialog. */
		void addPlugins();

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
		Prefs_Guides *prefs_Guides;
		Prefs_Hyphenator *prefs_Hyphenator {nullptr};
		Prefs_ImageCache *prefs_ImageCache {nullptr};
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
};

#endif



