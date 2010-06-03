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

#include "scribusapi.h"
#include "prefsstructs.h"
#include "ui_preferencesdialogbase.h"

#include "ui/prefs_colormanagement.h"
#include "ui/prefs_userinterface.h"
#include "ui/prefs_documentinformation.h"
#include "ui/prefs_documentsections.h"
#include "ui/prefs_documentsetup.h"
#include "ui/prefs_display.h"
#include "ui/prefs_guides.h"
#include "ui/prefs_externaltools.h"
#include "ui/prefs_keyboardshortcuts.h"
#include "ui/prefs_paths.h"
#include "ui/prefs_printer.h"
#include "ui/prefs_hyphenator.h"
#include "ui/prefs_miscellaneous.h"
#include "ui/prefs_pagesizes.h"
#include "ui/prefs_plugins.h"
#include "ui/prefs_preflightverifier.h"
#include "ui/prefs_scrapbook.h"
#include "ui/prefs_typography.h"
#include "ui/prefs_itemtools.h"
#include "ui/prefs_operatortools.h"
#include "ui/prefs_fonts.h"
#include "ui/prefs_tableofcontents.h"
#include "ui/prefs_pdfexport.h"
#include "ui/prefs_documentitemattributes.h"
#include "ui/prefs_imagecache.h"

class PrefsManager;
class ScribusMainWindow;
class ScribusDoc;


/*! \brief The Scribus Preferences Dialog
*/
class SCRIBUS_API PreferencesDialog : public QDialog, Ui::PreferencesDialog
{
	Q_OBJECT

	public:
		PreferencesDialog(QWidget* parent, ApplicationPrefs& prefsData, ScribusDoc *doc=NULL);
		~PreferencesDialog();

		virtual void changeEvent(QEvent *e);
		ApplicationPrefs prefs() const { return localPrefs; }
		void getResizeDocumentPages(bool &resizePages, bool &resizeMasterPages, bool &resizePageMargins, bool &resizeMasterPageMargins);

	public slots:
		/// \brief Overridden to emit accepted(), which plugin panels use
		virtual void accept();
		void itemSelected(QListWidgetItem* ic);
		void newItemSelected();
		void restoreDefaults();
		void setupGui();
		void saveGuiToPrefs();
		void setNewItemSelected(const QString &s);

	protected slots:
		virtual void languageChange();
		void applyButtonClicked();


	protected:
		int addItem(QString name, QPixmap icon, QWidget *tab);
		void setupListWidget();
		void arrangeIcons();
		void initPreferenceValues();
		/*! \brief Scans plugins for those that want to add a prefs widget and
		hooks them up to the dialog. */
		void addPlugins();

		Prefs_ColorManagement *prefs_ColorManagement;
		Prefs_Display *prefs_Display;
		Prefs_DocumentInformation *prefs_DocumentInformation;
		Prefs_DocumentItemAttributes *prefs_DocumentItemAttributes;
		Prefs_DocumentSections *prefs_DocumentSections;
		Prefs_DocumentSetup *prefs_DocumentSetup;
		Prefs_ExternalTools *prefs_ExternalTools;
		Prefs_Fonts *prefs_Fonts;
		Prefs_Guides *prefs_Guides;
		Prefs_Hyphenator *prefs_Hyphenator;
		Prefs_ImageCache *prefs_ImageCache;
		Prefs_ItemTools *prefs_ItemTools;
		Prefs_KeyboardShortcuts *prefs_KeyboardShortcuts;
		Prefs_Miscellaneous *prefs_Miscellaneous;
		Prefs_OperatorTools *prefs_OperatorTools;
		Prefs_PDFExport *prefs_PDFExport;
		Prefs_PageSizes *prefs_PageSizes;
		Prefs_Paths *prefs_Paths;
		Prefs_Plugins *prefs_Plugins;
		Prefs_PreflightVerifier *prefs_PreflightVerifier;
		Prefs_Printer *prefs_Printer;
		Prefs_Scrapbook *prefs_Scrapbook;
		Prefs_TableOfContents *prefs_TableOfContents;
		Prefs_Typography *prefs_Typography;
		Prefs_UserInterface *prefs_UserInterface;

		QMap<QListWidgetItem*, int> stackWidgetMap;
		int counter;
		double unitRatio;
		int docUnitIndex;
		ScribusMainWindow* mainWin;
		PrefsManager* prefsManager;
		ScribusDoc* m_Doc;
		ApplicationPrefs localPrefs;
};

#endif



