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
#include "ui/prefs_documentsetup.h"
#include "ui/prefs_display.h"
#include "ui/prefs_guides.h"
#include "ui/prefs_externaltools.h"
#include "ui/prefs_keyboardshortcuts.h"
#include "ui/prefs_paths.h"
#include "ui/prefs_printer.h"
#include "ui/prefs_hyphenator.h"
#include "ui/prefs_miscellaneous.h"
#include "ui/prefs_plugins.h"
#include "ui/prefs_scripter.h"
#include "ui/prefs_scrapbook.h"
#include "ui/prefs_shortwords.h"
#include "ui/prefs_typography.h"
#include "ui/prefs_itemtools.h"
#include "ui/prefs_operatortools.h"
#include "ui/prefs_fonts.h"
#include "ui/prefs_tableofcontents.h"
#include "ui/prefs_pdfexport.h"
#include "ui/prefs_documentitemattributes.h"

class PrefsManager;
class ScribusMainWindow;


/*! \brief The Scribus Preferences Dialog
*/
class SCRIBUS_API PreferencesDialog : public QDialog, Ui::PreferencesDialog
{
	Q_OBJECT

	public:
		PreferencesDialog(QWidget* parent);
		~PreferencesDialog();

		virtual void changeEvent(QEvent *e);
		ApplicationPrefs prefs() const { return localPrefs; }

	public slots:
		void itemSelected(QListWidgetItem* ic);
		void restoreDefaults();
		void setupGui();
		void saveGuiToPrefs();
		virtual void accept();

	protected slots:
		virtual void languageChange();
		void applyButtonClicked();


	private:
		int addItem(QString name, QPixmap icon, QWidget *tab);
		void setupListWidget();
		void arrangeIcons();
		void initPreferenceValues();

		Prefs_ColorManagement *prefs_ColorManagement;
		Prefs_UserInterface *prefs_UserInterface;
		Prefs_DocumentSetup *prefs_DocumentSetup;
		Prefs_Display *prefs_Display;
		Prefs_ExternalTools *prefs_ExternalTools;
		Prefs_Guides *prefs_Guides;
		Prefs_KeyboardShortcuts *prefs_KeyboardShortcuts;
		Prefs_Paths *prefs_Paths;
		Prefs_Printer *prefs_Printer;
		Prefs_Hyphenator *prefs_Hyphenator;
		Prefs_Miscellaneous *prefs_Miscellaneous;
		Prefs_Plugins *prefs_Plugins;
		Prefs_Scripter *prefs_Scripter;
		Prefs_Scrapbook *prefs_Scrapbook;
		Prefs_ShortWords *prefs_ShortWords;
		Prefs_Typography *prefs_Typography;
		Prefs_ItemTools *prefs_ItemTools;
		Prefs_OperatorTools *prefs_OperatorTools;
		Prefs_Fonts *prefs_Fonts;
		Prefs_TableOfContents *prefs_TableOfContents;
		Prefs_PDFExport *prefs_PDFExport;
		Prefs_DocumentItemAttributes *prefs_DocumentItemAttributes;

		QMap<QListWidgetItem*, int> stackWidgetMap;
		int counter;
		double unitRatio;
		int docUnitIndex;
		ScribusMainWindow* mainWin;
		PrefsManager* prefsManager;

		ApplicationPrefs localPrefs;
};

#endif


