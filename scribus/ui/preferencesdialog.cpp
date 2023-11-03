/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QPushButton>
#include <QLayout>
#include <QListWidget>
#include <QVBoxLayout>

#include "commonstrings.h"
#include "iconmanager.h"
#include "pluginmanager.h"
#include "prefs_pane.h"
#include "prefsmanager.h"
#include "scplugin.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "ui/preferencesdialog.h"
#include "units.h"

PreferencesDialog::PreferencesDialog(QWidget* parent, ApplicationPrefs& prefsData, ScribusDoc* doc)
	: QDialog(parent),
	m_Doc(doc)
{
	setupUi(this);
	setObjectName(QString::fromLocal8Bit("PreferencesDialog"));
	setupListWidget();
	while (prefsStackWidget->currentWidget() != nullptr)
		prefsStackWidget->removeWidget(prefsStackWidget->currentWidget());

	exportButton->hide();

	if(doc)
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
	prefs_TableOfContents = new Prefs_TableOfContents(prefsStackWidget, m_Doc);
	prefs_Typography = new Prefs_Typography(prefsStackWidget, m_Doc);
	if (doc)
	{
		prefs_DocumentInformation = new Prefs_DocumentInformation(prefsStackWidget, m_Doc);
		prefs_DocumentSections  = new Prefs_DocumentSections(prefsStackWidget, m_Doc);
	}
	if (!doc)
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
	// Add Stack Widgets if required
	if (!doc)
	{
		addWidget(prefs_UserInterface);
		addWidget(prefs_Paths);
		addWidget(prefs_KeyboardShortcuts);
	}
	addWidget(prefs_DocumentSetup);
	if (!doc)
		addWidget(prefs_PageSizes);
	addWidget(prefs_Guides);
	if (doc)
		addWidget(prefs_DocumentInformation);
	if (doc)
		addWidget(prefs_DocumentSections);

	addWidget(prefs_ItemTools);
	addWidget(prefs_Fonts);
	addWidget(prefs_Typography);
	addWidget(prefs_Hyphenator);
	//if (!doc)
	//	addItem(prefs_Spelling);



	addWidget(prefs_ColorManagement);
	if (!doc)
		addWidget(prefs_ImageCache);
	addWidget(prefs_Display);
	addWidget(prefs_OperatorTools);
	if (!doc)
		addWidget(prefs_ExternalTools);
	if (!doc)
		addWidget(prefs_Scrapbook);
	addWidget(prefs_PreflightVerifier);
	addWidget(prefs_Printer);
	addWidget(prefs_PDFExport);
	if (!doc)
		addWidget(prefs_Miscellaneous);
	addWidget(prefs_DocumentItemAttributes);
	addWidget(prefs_TableOfContents);

	if (!doc)
		addWidget(prefs_Plugins);

	if (!doc)
		addWidget(prefs_Experimental);

	arrangeIcons();

	//**********

	localPrefs = prefsData;
	initPreferenceValues();
	setupGui();
	if (!doc)
		addPlugins();

	if (preferencesTypeList->count() > 0)
	{
		preferencesTypeList->item(0)->setSelected(true);
		itemSelected(preferencesTypeList->item(0));
	}

	connect(prefs_DocumentSetup, SIGNAL(changeToOtherSection(const QString&)), this, SLOT(setNewItemSelected(const QString&)));
	connect(prefs_DocumentSetup, SIGNAL(prefsChangeUnits(int)), this, SLOT(changeUnits(int)));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(preferencesTypeList, SIGNAL(itemSelectionChanged()), this, SLOT(newItemSelected()));
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
	if (prefs_UserInterface) prefs_UserInterface->restoreDefaults(&localPrefs);
	if (prefs_Paths) prefs_Paths->restoreDefaults(&localPrefs);
	if (prefs_DocumentSetup) prefs_DocumentSetup->restoreDefaults(&localPrefs);
	if (prefs_DocumentInformation) prefs_DocumentInformation->restoreDefaults(&localPrefs);
	if (prefs_Guides) prefs_Guides->restoreDefaults(&localPrefs);
	if (prefs_Typography) prefs_Typography->restoreDefaults(&localPrefs);
	if (prefs_ItemTools) prefs_ItemTools->restoreDefaults(&localPrefs);
	if (prefs_OperatorTools) prefs_OperatorTools->restoreDefaults(&localPrefs);
	if (prefs_Hyphenator) prefs_Hyphenator->restoreDefaults(&localPrefs);
	if (prefs_Fonts) prefs_Fonts->restoreDefaults(&localPrefs);
	if (prefs_Printer) prefs_Printer->restoreDefaults(&localPrefs);
	if (prefs_PDFExport) prefs_PDFExport->restoreDefaults(&localPrefs, ScCore->PDFXProfiles);
	if (prefs_PreflightVerifier) prefs_PreflightVerifier->restoreDefaults(&localPrefs);
	if (prefs_DocumentItemAttributes) prefs_DocumentItemAttributes->restoreDefaults(&localPrefs);
	if (prefs_TableOfContents) prefs_TableOfContents->restoreDefaults(&localPrefs);
	if (prefs_DocumentSections) prefs_DocumentSections->restoreDefaults(&localPrefs);
	if (prefs_KeyboardShortcuts) prefs_KeyboardShortcuts->restoreDefaults(&localPrefs);
	if (prefs_ColorManagement)
	{
		prefs_ColorManagement->setProfiles(&localPrefs, &ScCore->InputProfiles, &ScCore->InputProfilesCMYK, &ScCore->PrinterProfiles, &ScCore->MonitorProfiles);
		prefs_ColorManagement->restoreDefaults(&localPrefs);
	}
	if (prefs_Scrapbook) prefs_Scrapbook->restoreDefaults(&localPrefs);
//	if (prefs_Spelling) prefs_Spelling->restoreDefaults(&localPrefs);
	if (prefs_Display) prefs_Display->restoreDefaults(&localPrefs);
	if (prefs_ExternalTools) prefs_ExternalTools->restoreDefaults(&localPrefs);
	if (prefs_Plugins) prefs_Plugins->restoreDefaults(&localPrefs);
	if (prefs_Miscellaneous) prefs_Miscellaneous->restoreDefaults(&localPrefs);
	if (prefs_PageSizes) prefs_PageSizes->restoreDefaults(&localPrefs);
	if (prefs_ImageCache) prefs_ImageCache->restoreDefaults(&localPrefs);
	if (prefs_Experimental) prefs_Experimental->restoreDefaults(&localPrefs);
}


void PreferencesDialog::saveGuiToPrefs()
{
	if (prefs_UserInterface) prefs_UserInterface->saveGuiToPrefs(&localPrefs);
	if (prefs_Paths) prefs_Paths->saveGuiToPrefs(&localPrefs);
	if (prefs_DocumentSetup) prefs_DocumentSetup->saveGuiToPrefs(&localPrefs);
	if (prefs_DocumentInformation) prefs_DocumentInformation->saveGuiToPrefs(&localPrefs);
	if (prefs_Guides) prefs_Guides->saveGuiToPrefs(&localPrefs);
	if (prefs_Typography) prefs_Typography->saveGuiToPrefs(&localPrefs);
	if (prefs_ItemTools) prefs_ItemTools->saveGuiToPrefs(&localPrefs);
	if (prefs_OperatorTools) prefs_OperatorTools->saveGuiToPrefs(&localPrefs);
	if (prefs_Hyphenator) prefs_Hyphenator->saveGuiToPrefs(&localPrefs);
	if (prefs_Fonts) prefs_Fonts->saveGuiToPrefs(&localPrefs);
	if (prefs_Printer) prefs_Printer->saveGuiToPrefs(&localPrefs);
	if (prefs_PDFExport) prefs_PDFExport->saveGuiToPrefs(&localPrefs);
	if (prefs_PreflightVerifier) prefs_PreflightVerifier->saveGuiToPrefs(&localPrefs);
	if (prefs_DocumentItemAttributes) prefs_DocumentItemAttributes->saveGuiToPrefs(&localPrefs);
	if (prefs_TableOfContents) prefs_TableOfContents->saveGuiToPrefs(&localPrefs);
	if (prefs_DocumentSections) prefs_DocumentSections->saveGuiToPrefs(&localPrefs);
	if (prefs_KeyboardShortcuts) prefs_KeyboardShortcuts->saveGuiToPrefs(&localPrefs);
	if (prefs_ColorManagement) prefs_ColorManagement->saveGuiToPrefs(&localPrefs);
	if (prefs_Scrapbook) prefs_Scrapbook->saveGuiToPrefs(&localPrefs);
//	if (prefs_Spelling) prefs_Spelling->saveGuiToPrefs(&localPrefs);
	if (prefs_Display) prefs_Display->saveGuiToPrefs(&localPrefs);
	if (prefs_ExternalTools) prefs_ExternalTools->saveGuiToPrefs(&localPrefs);
	if (prefs_Plugins) prefs_Plugins->saveGuiToPrefs(&localPrefs);
	if (prefs_Miscellaneous) prefs_Miscellaneous->saveGuiToPrefs(&localPrefs);
	if (prefs_PageSizes) prefs_PageSizes->saveGuiToPrefs(&localPrefs);
	if (prefs_ImageCache) prefs_ImageCache->saveGuiToPrefs(&localPrefs);
	if (prefs_Experimental) prefs_Experimental->saveGuiToPrefs(&localPrefs);
}

void PreferencesDialog::accept()
{
	saveGuiToPrefs();
	// #15583: emitting manually accepted should not be needed
	// QDialog::accept() will emit it by itself
	// emit accepted();
	QDialog::accept();
}

void PreferencesDialog::setupListWidget()
{
	preferencesTypeList->setDragEnabled(false);
	preferencesTypeList->setViewMode(QListView::ListMode);
	preferencesTypeList->setFlow(QListView::TopToBottom);
	preferencesTypeList->setIconSize(QSize(32,32));
	preferencesTypeList->setSortingEnabled(false);
	preferencesTypeList->setWrapping(false);
	preferencesTypeList->setWordWrap(true);
	preferencesTypeList->setAcceptDrops(false);
	preferencesTypeList->setDropIndicatorShown(false);
	preferencesTypeList->setDragDropMode(QAbstractItemView::NoDragDrop);
	//preferencesTypeList->setResizeMode(QListView::Adjust);
	preferencesTypeList->setSelectionMode(QAbstractItemView::SingleSelection);
	preferencesTypeList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	preferencesTypeList->clear();
}

void PreferencesDialog::addWidget(Prefs_Pane* tab)
{
	//TODO: Can we avoid using this name and duplicating strings by getting it from the tab UIs
	QListWidgetItem* newItem = new QListWidgetItem(IconManager::instance().loadIcon(tab->icon()), tab->caption(), preferencesTypeList);
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
		//emit aboutToShow(prefsWidgets->widget(itemMap[ic]));
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
	setWindowTitle( tr( "Preferences" ) );
}

void PreferencesDialog::addPlugins()
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
			// plug it in to the dialog,
			addWidget(panel);
			// and connect a signal to tell it to save its settings.
			connect(this, SIGNAL(accepted()), panel, SLOT(apply()));
		}
	}
}

void PreferencesDialog::getResizeDocumentPages(bool &resizePages, bool &resizeMasterPages, bool &resizePageMargins, bool &resizeMasterPageMargins)
{
	prefs_DocumentSetup->getResizeDocumentPages(resizePages, resizeMasterPages, resizePageMargins, resizeMasterPageMargins);
}


void PreferencesDialog::arrangeIcons()
{/*
	int maxWidth = 0;
	QListWidgetItem* ic;
	int startY = 5;
	for (int cc = 0; cc < preferencesTypeList->count(); ++cc)
	{
		ic = preferencesTypeList->item(cc);
		QRect ir = preferencesTypeList->visualItemRect(ic);
		maxWidth = qMax(ir.width(), maxWidth);
	}
	preferencesTypeList->setMaximumWidth(maxWidth+16);
	preferencesTypeList->setResizeMode(QListView::Fixed);
#ifdef _WIN32
	int scrollBarWidth = 0;
	QList<QScrollBar*> scrollBars = preferencesTypeList->findChildren<QScrollBar*>();
	for (int cc = 0; cc < scrollBars.count(); ++cc)
	{
		if (scrollBars.at(cc)->orientation() == Qt::Vertical)
		{
			scrollBarWidth = scrollBars.at(cc)->height();
			break;
		}
	}
#else
	int scrollBarWidth = maxWidth;
#endif
	int startX = qMax((preferencesTypeList->viewport()->width() - scrollBarWidth) / 2, 0);
	for (int cc = 0; cc < preferencesTypeList->count(); ++cc)
	{
		ic = preferencesTypeList->item(cc);
		QRect ir = preferencesTypeList->visualItemRect(ic);

#ifdef _WIN32
		preferencesTypeList->setPositionForIndex(QPoint(qMax(startX - ir.width() / 2, 0), startY), preferencesTypeList->indexFromItem(ic));
#else
		int moveW = (maxWidth - ir.width()) / 2;
		preferencesTypeList->setPositionForIndex(QPoint(moveW + startX, startY), preferencesTypeList->indexFromItem(ic));
#endif

		startY += ir.height()+5;
	}*/
}



