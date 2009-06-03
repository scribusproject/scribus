/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PREFS_H
#define PREFS_H

#include "scribusapi.h"
#include "scribus.h"
#include "prefsdialogbase.h"

class TabTypograpy;
class HySettings;
class CMSPrefs;
class TabGuides;
class TabTools;
class TabCheckDoc;
class TabPDFOptions;
class FontPrefs;
class DocumentItemAttributes;
class TOCIndexPrefs;
class MarginWidget;
class PrefsManager;
class PageLayouts;
class TabExternalToolsWidget;
class TabKeyboardShortcutsWidget;
class PluginManagerPrefsGui;
class TabGeneral;
class TabDisplay;
class TabDocument;
class TabMiscellaneous;
class TabScrapbook;
class TabPrinter;


/*! \brief A Preferences dialog.
It's modular now. You can see a lot of special widgets here
but no QWidget itself. Most of the "panels" are converted
into UI files with Scribus's standard convention:
foobase - foo inherited.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API Preferences : public PrefsDialogBase
{
	Q_OBJECT

public:
	
	/*!
	\author Franz Schmid
	\brief Constructor for Preferences dialog box
	\param parent QWidget pointer to parent window
	*/
	Preferences( QWidget* parent);
	~Preferences(){};

	void updatePreferences();

public slots:
	/// \brief Overridden to emit accepted(), which plugin panels use
	void accept();

	/*!
	\author Franz Schmid
	\brief Preferences (Document / Page Size), switches default between Facing Pages and swaps text labels for margin guides
	 */
	void setDS(int layout);
	/*!
	\author Franz Schmid
	\brief Preferences (General, Units). Sets scaling factors and units descriptions when default units are changed. Updates preference values
	*/
	void unitChange();

	void switchCMS(bool enable);

protected:
	TabGeneral* tabGeneral;
	TabGuides* tabGuides;
	TabTypograpy* tabTypo;
	TabDisplay* tabView;
	TabTools* tabTools;
	HySettings* tabHyphenator;
	FontPrefs* tabFonts;
	TabPrinter* tabPrinter;
	TabCheckDoc* tabDocChecker;
	CMSPrefs* tabColorManagement;
	TabPDFOptions* tabPDF;
	DocumentItemAttributes* tabDefaultItemAttributes;
	TOCIndexPrefs* tabDefaultTOCIndexPrefs;
	TabExternalToolsWidget* tabExtTools;
	TabKeyboardShortcutsWidget* tabKeyboardShortcuts;
	TabScrapbook* tabScrapbook;
	TabDocument* tabDocument;
	TabMiscellaneous* tabMiscellaneous;
	PluginManagerPrefsGui* pluginManagerPrefsGui;

	double unitRatio;
	int docUnitIndex;
	ScribusMainWindow *ap;
	PrefsManager* prefsManager;

	/*! \brief Scans plugins for those that want to add a prefs widget and
	hooks them up to the dialog. */
	void addPlugins();

	//! \brief Resets the PrefsManager. Then call setupGui().
	void restoreDefaults();
	//! \brief Setup the GUI widgets by values taken from PrefsManager.
	void setupGui();
	void enableSignals(bool);

protected slots:
	//! \brief Method to be called when user selects the icon.
	virtual void showWidgetInStack(QWidget *widgetToShow);
	void setActionHistoryLength();
	virtual void applyChangesButton_clicked();
	virtual void backToDefaults_clicked();
};

#endif // PREFS_H
