/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLayout>
#include <QListWidget>
#include <QVBoxLayout>

#include "iconmanager.h"
#include "pluginmanager.h"
#include "prefs_pane.h"
#include "prefsmanager.h"
#include "scplugin.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "ui/preferences/preferencesdialog.h"
#include "units.h"

PreferencesDialog::PreferencesDialog(QWidget* parent, ApplicationPrefs& prefsData, ScribusDoc* doc)
	: ScDialog(parent, QString::fromLocal8Bit("PreferencesDialog")),
	m_Doc(doc)
{
	setupUi(this);

	exportButton->hide();

	if(m_Doc)
	{
		setWindowTitle( tr("Document Setup") );
		defaultsButton->hide();
	}
	else
		connect(defaultsButton, SIGNAL(clicked()), SLOT(restoreDefaults()));

	// Create Stack Widgets if required
	prefs_ColorManagement = new Prefs_ColorManagement(prefsStackWidget, m_Doc);
	prefs_Display = new Prefs_Display(prefsStackWidget, m_Doc);
	prefs_DocumentItemAttributes = new Prefs_DocumentItemAttributes(prefsStackWidget, m_Doc);
	prefs_DocumentSetup = new Prefs_DocumentSetup(prefsStackWidget, m_Doc);
	prefs_Fonts = new Prefs_Fonts(prefsStackWidget, m_Doc);
	prefs_Guides = new Prefs_Guides(prefsStackWidget, m_Doc);
	prefs_Hyphenator = new Prefs_Hyphenator(prefsStackWidget, m_Doc);
	prefs_ItemTools = new Prefs_ItemTools(prefsStackWidget, m_Doc);
	prefs_OperatorTools = new Prefs_OperatorTools(prefsStackWidget, m_Doc);
	prefs_PDFExport = new Prefs_PDFExport(prefsStackWidget, m_Doc);
	prefs_PreflightVerifier = new Prefs_PreflightVerifier(prefsStackWidget, m_Doc);
	prefs_Printer = new Prefs_Printer(prefsStackWidget, m_Doc);

	prefs_Typography = new Prefs_Typography(prefsStackWidget, m_Doc);
	if (m_Doc)
	{
		prefs_DocumentInformation = new Prefs_DocumentInformation(prefsStackWidget, m_Doc);
		prefs_DocumentSections  = new Prefs_DocumentSections(prefsStackWidget, m_Doc);
		prefs_Indexes = new Prefs_Indexes(prefsStackWidget, m_Doc);
		prefs_TableOfContents = new Prefs_TableOfContents(prefsStackWidget, m_Doc);
	}
	if (!m_Doc)
	{
		prefs_ExternalTools = new Prefs_ExternalTools(prefsStackWidget, m_Doc);
		prefs_ImageCache = new Prefs_ImageCache(prefsStackWidget, m_Doc);
		prefs_KeyboardShortcuts = new Prefs_KeyboardShortcuts(prefsStackWidget, m_Doc);
		prefs_Miscellaneous = new Prefs_Miscellaneous(prefsStackWidget, m_Doc);
		prefs_PageSizes = new Prefs_PageSizes(prefsStackWidget, m_Doc);
		prefs_Paths = new Prefs_Paths(prefsStackWidget, m_Doc);
		prefs_Plugins = new Prefs_Plugins(prefsStackWidget, m_Doc);
		prefs_Scrapbook = new Prefs_Scrapbook(prefsStackWidget, m_Doc);
//		prefs_Spelling = new Prefs_Spelling(prefsStackWidget, m_Doc);
		prefs_UserInterface = new Prefs_UserInterface(prefsStackWidget, m_Doc);
		prefs_Experimental = new Prefs_Experimental(prefsStackWidget, m_Doc);
	}
	createStackWidgetList();
	if (!m_Doc)
		addPluginsToStackWidgetList();
	addWidgetsToStack();

	//**********

	localPrefs = prefsData;
	initPreferenceValues();
	setupGui();


	if (preferencesTypeList->count()>0)
	{
		preferencesTypeList->item(0)->setSelected(true);
		itemSelected(preferencesTypeList->item(0));
	}

	connect(prefs_DocumentSetup, SIGNAL(changeToOtherSection(QString)), this, SLOT(setNewItemSelected(QString)));
	connect(prefs_DocumentSetup, SIGNAL(prefsChangeUnits(int)), this, SLOT(changeUnits(int)));
	connect(buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &PreferencesDialog::reject);
	connect(preferencesTypeList, SIGNAL(itemSelectionChanged()), this, SLOT(newItemSelected()));
	connect(searchField, SIGNAL(textEdited(QString)), this, SLOT(addWidgetsToStack()));
}

void PreferencesDialog::restoreDefaults()
{
	PrefsManager::instance().initDefaults();
	setupGui();
}

void PreferencesDialog::initPreferenceValues()
{
	mainWin = qobject_cast<ScribusMainWindow*>(parent());
	docUnitIndex = localPrefs.docSetupPrefs.docUnitIndex;
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
}

void PreferencesDialog::setupGui()
{
	for (auto it = stackDataList.cbegin(), end = stackDataList.cend(); it != end; ++it)
	{
		if ((*it).prefsPane == nullptr)
			continue;

		(*it).prefsPane->restoreDefaults(&localPrefs);
	}

	if (prefs_ColorManagement)
		prefs_ColorManagement->setProfiles(&localPrefs, &ScCore->InputProfiles, &ScCore->InputProfilesCMYK, &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
}


void PreferencesDialog::saveGuiToPrefs()
{
	for (auto it = stackDataList.cbegin(), end = stackDataList.cend(); it != end; ++it)
	{
		if ((*it).prefsPane == nullptr)
			continue;

		(*it).prefsPane->saveGuiToPrefs(&localPrefs);
	}
}

void PreferencesDialog::accept()
{
	saveGuiToPrefs();
	// #15583: emitting manually accepted should not be needed
	// QDialog::accept() will emit it by itself
	// emit accepted();
	QDialog::accept();
}

void PreferencesDialog::createStackWidgetList()
{
	if (!stackDataList.isEmpty())
		stackDataList.clear();
	PrefsTabData ptd;

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_UserInterface);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Paths);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_KeyboardShortcuts);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_DocumentSetup);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_PageSizes);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Guides);
	stackDataList.append(ptd);

	ptd.forPrefs = false;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_DocumentInformation);
	stackDataList.append(ptd);

	ptd.forPrefs = false;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_DocumentSections);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_ItemTools);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Fonts);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Typography);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Hyphenator);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_ColorManagement);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_ImageCache);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Display);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_OperatorTools);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_ExternalTools);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Scrapbook);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_PreflightVerifier);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Printer);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_PDFExport);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Miscellaneous);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_DocumentItemAttributes);
	stackDataList.append(ptd);

	ptd.forPrefs = false;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Indexes);
	stackDataList.append(ptd);

	ptd.forPrefs = false;
	ptd.forDoc = true;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_TableOfContents);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Plugins);
	stackDataList.append(ptd);

	ptd.forPrefs = true;
	ptd.forDoc = false;
	ptd.prefsPane = dynamic_cast<Prefs_Pane*>(prefs_Experimental);
	stackDataList.append(ptd);
}

void PreferencesDialog::addWidgetsToStack()
{
	QString searchText(searchField->text());
	bool searchIsEmpty = searchText.isEmpty();

	preferencesTypeList->clear();
	while (prefsStackWidget->currentWidget() != nullptr)
		prefsStackWidget->removeWidget(prefsStackWidget->currentWidget());

	for (auto it = stackDataList.cbegin(), end = stackDataList.cend(); it != end; ++it)
	{
		if ((*it).prefsPane == nullptr || !(*it).visible)
			continue;

		if (!searchIsEmpty)
		{
			QString prefsPaneCaption((*it).prefsPane->caption());
			QStringList prefsPaneKeywordResult = (*it).prefsPane->keywords().filter(searchText, Qt::CaseInsensitive);
			if(!prefsPaneCaption.contains(searchText, Qt::CaseInsensitive) && prefsPaneKeywordResult.isEmpty())
				continue;
		}
		(*it).prefsPane->highlightWidgets(searchText);
		if ((*it).forDoc && (*it).forPrefs)
			addWidget((*it).prefsPane);
		else if (m_Doc && (*it).forDoc)
			addWidget((*it).prefsPane);
		else if (!m_Doc && (*it).forPrefs)
			addWidget((*it).prefsPane);
	}
}

void PreferencesDialog::addWidget(Prefs_Pane* tab)
{
	QListWidgetItem* newItem = new QListWidgetItem(IconManager::instance().loadIcon(tab->icon(), 16), tab->caption(), preferencesTypeList);
	newItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	int i = prefsStackWidget->addWidget(tab);
	stackWidgetMap.insert(newItem, i);
}

void PreferencesDialog::newItemSelected()
{
	QList<QListWidgetItem*> items = preferencesTypeList->selectedItems();
	itemSelected((items.count() > 0) ? items.at(0) : nullptr);
}

void PreferencesDialog::itemSelected(QListWidgetItem* ic)
{
	if (ic == nullptr)
		return;
	if (stackWidgetMap.contains(ic))
	{
		prefsStackWidget->setCurrentIndex(stackWidgetMap[ic]);
		if (prefsStackWidget->currentWidget() == dynamic_cast<QWidget*>(prefs_DocumentSetup))
			prefs_DocumentSetup->setupPageSizes(&localPrefs);
		if (prefsStackWidget->currentWidget() == dynamic_cast<QWidget*>(prefs_ItemTools))
			prefs_ItemTools->enableFontPreview(true);
		if (prefsStackWidget->currentWidget() == dynamic_cast<QWidget*>(prefs_TableOfContents))
			prefs_TableOfContents->setupItemAttrs(prefs_DocumentItemAttributes->getDocAttributesNames());
		if (prefsStackWidget->currentWidget() == dynamic_cast<QWidget*>(prefs_PDFExport))
			prefs_PDFExport->enableCMS(prefs_ColorManagement->cmActive());
	}
}

void PreferencesDialog::setNewItemSelected(const QString &s)
{
	if (s == "Prefs_PageSizes" && prefs_PageSizes != nullptr)
	{
		int i = prefsStackWidget->indexOf(prefs_PageSizes);
		if (i != -1)
			preferencesTypeList->setCurrentRow(i);
	}
}

void PreferencesDialog::changeUnits(int u)
{
	prefs_Display->unitChange(u);
	prefs_Guides->unitChange(u);
	prefs_ItemTools->unitChange(u);
	prefs_OperatorTools->unitChange(u);
	prefs_PDFExport->unitChange(u);
	prefs_Printer->unitChange(u);
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QDialog::changeEvent(e);
}

void PreferencesDialog::languageChange()
{
	setWindowTitle(tr("Preferences"));
}

void PreferencesDialog::addPluginsToStackWidgetList()
{
	// Scan for plugins that provide a prefs widget, and add it to the
	// prefs dialog.
	// For each plugin, enabled or not:
	ScPlugin* plugin = nullptr;
	Prefs_Pane* panel = nullptr;

	PluginManager& pluginManager = PluginManager::instance();
	const QStringList pluginNames(pluginManager.pluginNames(true));

	for (const QString& pName : pluginNames)
	{
		// Ask the plugin manager for a plugin (skipping disabled plugins).
		plugin = pluginManager.getPlugin(pName, false);
		if (!plugin)
			continue;
		// If we got a plugin (which we know is enabled):
		// Ask the plugin for a prefs widget
		panel = nullptr;
		bool wantPanel = plugin->newPrefsPanelWidget(prefsStackWidget, panel);
		// If it gave us one...
		if (wantPanel && panel != nullptr)
		{
			PrefsTabData ptd;
			ptd.forPrefs = true;
			ptd.forDoc = false;
			ptd.prefsPane = dynamic_cast<Prefs_Pane*>(panel);
			stackDataList.append(ptd);

			// and connect a signal to tell it to save its settings.
			connect(this, SIGNAL(accepted()), panel, SLOT(apply()));
		}
	}
}

void PreferencesDialog::getResizeDocumentPages(bool &resizePages, bool &resizeMasterPages, bool &resizePageMargins, bool &resizeMasterPageMargins)
{
	prefs_DocumentSetup->getResizeDocumentPages(resizePages, resizeMasterPages, resizePageMargins, resizeMasterPageMargins);
}


