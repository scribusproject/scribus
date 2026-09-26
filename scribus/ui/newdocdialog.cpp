/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "newdocdialog.h"

#include <utility>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QGroupBox>
#include <QLabel>
#include <QListWidgetItem>
#include <QPixmap>
#include <QPoint>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStandardPaths>
#include <QStringList>
#include <QTabWidget>
#include <QTabBar>
#include <QToolTip>
#include <QVBoxLayout>
#include <QWindow>

#include "commonstrings.h"
#include "filedialogeventcatcher.h"
#include "fileloader.h"
#include "iconmanager.h"
#include "newmarginwidget.h"
#include "pagestructs.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scmessagebox.h"
#include "scpaths.h"
#include "scrspinbox.h"
#include "units.h"
#include "util.h"
#include "ui/widgets/pagesizelist.h"


NewDocDialog::NewDocDialog(QWidget* parent, const QStringList& recentDocs, bool startUp, const QString& lang) : ScDialog(parent, "NewDocumentWindow"),
	prefsManager(PrefsManager::instance()),
	m_onStartup(startUp)
{
	setupUi(this);
	setProperty("modernDialog", true);
	setAttribute(Qt::WA_StyledBackground, true);
	setMinimumSize(820, 580);

	QByteArray applicationStyle;
	if (loadRawText(ScPaths::instance().libDir() + "scribus.css", applicationStyle))
		setStyleSheet(QString::fromUtf8(applicationStyle));

	for (SectionContainer* section : findChildren<SectionContainer*>())
	{
		section->setProperty("dialogSection", true);
		section->setAttribute(Qt::WA_StyledBackground, true);
	}
	if (QPushButton* primaryButton = buttonBox->button(QDialogButtonBox::Ok))
	{
		primaryButton->setProperty("primaryAction", true);
		primaryButton->setText(tr("Create"));
		primaryButton->setDefault(true);
	}

	IconManager &iconManager = IconManager::instance();

	setModal(true);
	setWindowTitle( tr( "New Document" ) );

	m_labelVisibity = prefsManager.appPrefs.uiPrefs.showLabels;
	m_unitIndex = prefsManager.appPrefs.docSetupPrefs.docUnitIndex;
	m_unitRatio = unitGetRatioFromIndex(m_unitIndex);
	m_unitSuffix = unitGetSuffixFromIndex(m_unitIndex);
	m_orientation = prefsManager.appPrefs.docSetupPrefs.pageOrientation;
	m_bindingDirection = prefsManager.appPrefs.docSetupPrefs.bindingDirection;
	m_choosenLayout = prefsManager.appPrefs.docSetupPrefs.pagePositioning;
	m_layoutFirstPage = prefsManager.appPrefs.pageSets.at(m_choosenLayout).FirstPage;

	buttonVertical->setIcon(iconManager.loadIcon("page-orientation-vertical"));
	buttonHorizontal->setIcon(iconManager.loadIcon("page-orientation-horizontal"));
	buttonSinglePage->setIcon(iconManager.loadIcon("page-simple"));
	buttonDoublePageLeft->setIcon(iconManager.loadIcon("page-first-left"));
	buttonDoublePageRight->setIcon(iconManager.loadIcon("page-first-right"));
	QIcon pageBindingIcon;
	pageBindingIcon.addPixmap(iconManager.loadPixmap("page-binding-left"), QIcon::Normal, QIcon::Off);
	pageBindingIcon.addPixmap(iconManager.loadPixmap("page-binding-right"), QIcon::Normal, QIcon::On);
	buttonBindingDirection->setIcon(pageBindingIcon);
	buttonSavePagePreset->setIcon(iconManager.loadIcon("save"));
	labelColumns->setPixmap(iconManager.loadPixmap("paragraph-columns"));
	labelName->setPixmap(iconManager.loadPixmap("name"));

	createNewDocPage();

	if (startUp)
	{
		nftGui->setupSettings(lang);
		createOpenDocPage();
		recentDocList = recentDocs;
		createRecentDocPage();
		createWelcomePage();
		startUpDialog->setChecked(!prefsManager.appPrefs.uiPrefs.showStartupDialog);
	}
	else
	{
		tabWidget->removeTab(3);
		tabWidget->removeTab(2);
		tabWidget->removeTab(1);
	}

	tabWidget->setCurrentIndex(0);
	startUpDialog->setVisible(startUp);
	adjustTitles(0);

	//tooltips
	listPageFormats->setToolTip( tr( "Document page size, either a standard size or a custom size" ) );
	buttonVertical->setToolTip( tr( "Vertical orientation of the document's pages" ) );
	buttonHorizontal->setToolTip( tr( "Horizontal orientation of the document's pages" ) );
	buttonSinglePage->setToolTip(tr("Single page document"));
	buttonDoublePageLeft->setToolTip(tr("A document with facing pages, with the first page on the left side"));
	buttonDoublePageRight->setToolTip(tr("A document with facing pages, with the first page on the right side"));
	buttonBindingDirection->setToolTip(tr("Bind the pages on the right (LTR) or left (RTL) side"));
	widthSpinBox->setToolTip( tr( "Width of the document's pages, editable if you have chosen a custom page size" ) );
	heightSpinBox->setToolTip( tr( "Height of the document's pages, editable if you have chosen a custom page size" ) );
	pageCountSpinBox->setToolTip( tr( "Initial number of pages of the document" ) );
	unitOfMeasureComboBox->setToolTip( tr( "Default unit of measurement for document editing" ) );
	autoTextFrame->setToolTip( tr( "Create text frames automatically when new pages are added" ) );
	numberOfCols->setToolTip( tr( "Number of columns to create in automatically created text frames" ) );
	Distance->setToolTip( tr( "Distance between automatically created columns" ) );

	// signals and slots connections
	connect(buttonBox, &QDialogButtonBox::accepted, this, &NewDocDialog::ExitOK);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &NewDocDialog::reject);

	connect(pageOrientationButtons, &QButtonGroup::idClicked, this, &NewDocDialog::setOrientation);
	connect(pageLayoutButtons, &QButtonGroup::idClicked, this, &NewDocDialog::setLayout);
	connect(buttonBindingDirection, &QToolButton::toggled, this, &NewDocDialog::setBindingDirection);
	connect(unitOfMeasureComboBox, &QComboBox::activated, this, &NewDocDialog::setUnit);
	connect(Distance, &ScrSpinBox::valueChanged, this, &NewDocDialog::setDistance);
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
	connect(autoTextFrame, &QCheckBox::checkStateChanged, this, &NewDocDialog::handleAutoFrame);
#else
	connect(autoTextFrame, &QCheckBox::stateChanged, this, &NewDocDialog::handleAutoFrame);
#endif
	connect(listPageFormats, &PageSizeList::clicked, this, &NewDocDialog::changePageSize);
	connect(listPageFormats, &PageSizeList::changedCategories, this, &NewDocDialog::updateCategorySelector);
	connect(pageSizeSelector, &PageSizeSelector::pageCategoryChanged, this, &NewDocDialog::changeCategory);
	connect(marginGroup, &NewMarginWidget::valuesChanged, this, &NewDocDialog::changeMargin);
	connect(bleedGroup, &NewMarginWidget::valuesChanged, this, &NewDocDialog::changeBleed);
	connect(comboSortSizes, &QComboBox::currentIndexChanged, this, &NewDocDialog::changeSortMode);
	connect(buttonSavePagePreset, &QToolButton::clicked, this, &NewDocDialog::savePagePreset);

	if (startUp)
	{
		connect(nftGui, SIGNAL(leaveOK()), this, SLOT(ExitOK()));
		connect(recentDocListBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(recentDocListBox_doubleClicked()));
		connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(adjustTitles(int)));
	}
}

void NewDocDialog::createWelcomePage()
{
	m_welcomePage = new QWidget(tabWidget);
	m_welcomePage->setObjectName(QStringLiteral("welcomePage"));
	m_welcomePage->setProperty("welcomeSurface", true);
	m_welcomePage->setAttribute(Qt::WA_StyledBackground, true);

	auto* pageLayout = new QVBoxLayout(m_welcomePage);
	pageLayout->setContentsMargins(22, 18, 22, 14);
	pageLayout->setSpacing(14);

	auto* header = new QHBoxLayout();
	header->setSpacing(10);
	auto* appIcon = new QLabel(m_welcomePage);
	appIcon->setObjectName(QStringLiteral("welcomeAppIcon"));
	appIcon->setPixmap(IconManager::instance().loadPixmap("app-icon", QSize(42, 42)));
	appIcon->setFixedSize(46, 46);
	header->addWidget(appIcon);
	auto* appName = new QLabel(tr("Scribus"), m_welcomePage);
	appName->setObjectName(QStringLiteral("welcomeAppName"));
	header->addWidget(appName);
	header->addStretch();

	auto* openHeaderButton = new QPushButton(tr("Open Existing\u2026"), m_welcomePage);
	openHeaderButton->setObjectName(QStringLiteral("welcomeOpenHeaderButton"));
	openHeaderButton->setIcon(IconManager::instance().loadIcon("document-open"));
	openHeaderButton->setAccessibleName(tr("Open an existing document"));
	header->addWidget(openHeaderButton);
	auto* newHeaderButton = new QPushButton(tr("New Document"), m_welcomePage);
	newHeaderButton->setObjectName(QStringLiteral("welcomeNewHeaderButton"));
	newHeaderButton->setProperty("primaryAction", true);
	newHeaderButton->setIcon(IconManager::instance().loadIcon("document-new"));
	newHeaderButton->setAccessibleName(tr("Create a new document"));
	header->addWidget(newHeaderButton);
	pageLayout->addLayout(header);

	auto* divider = new QFrame(m_welcomePage);
	divider->setObjectName(QStringLiteral("welcomeHeaderDivider"));
	divider->setFrameShape(QFrame::HLine);
	pageLayout->addWidget(divider);

	auto* content = new QHBoxLayout();
	content->setSpacing(26);
	auto* actionsColumn = new QVBoxLayout();
	actionsColumn->setSpacing(10);
	auto* title = new QLabel(tr("Publish beautifully."), m_welcomePage);
	title->setObjectName(QStringLiteral("welcomeTitle"));
	actionsColumn->addWidget(title);
	actionsColumn->addSpacing(10);

	auto createActionButton = [this, actionsColumn](const QString& objectName, const QString& titleText,
		const QString& detailText, const QString& iconName) {
		auto* button = new QPushButton(m_welcomePage);
		button->setObjectName(objectName);
		button->setProperty("welcomeAction", true);
		button->setIcon(IconManager::instance().loadIcon(iconName));
		button->setIconSize(QSize(24, 24));
		button->setText(titleText + QStringLiteral("\n") + detailText);
		button->setAccessibleName(titleText);
		button->setToolTip(detailText);
		button->setMinimumHeight(58);
		actionsColumn->addWidget(button);
		return button;
	};

	auto* newButton = createActionButton(QStringLiteral("welcomeNewButton"), tr("New Document"),
		tr("Create a new publication"), QStringLiteral("document-new"));
	auto* openButton = createActionButton(QStringLiteral("welcomeOpenButton"), tr("Open Document"),
		tr("Open an existing document"), QStringLiteral("document-open"));
	auto* templatesButton = createActionButton(QStringLiteral("welcomeTemplatesButton"), tr("Browse Templates"),
		tr("Explore document templates"), QStringLiteral("page-3fold"));
	actionsColumn->addStretch();
	content->addLayout(actionsColumn, 4);

	auto* contentDivider = new QFrame(m_welcomePage);
	contentDivider->setObjectName(QStringLiteral("welcomeContentDivider"));
	contentDivider->setFrameShape(QFrame::VLine);
	content->addWidget(contentDivider);

	auto* recentColumn = new QVBoxLayout();
	auto* recentHeader = new QHBoxLayout();
	auto* recentTitle = new QLabel(tr("Recent Documents"), m_welcomePage);
	recentTitle->setObjectName(QStringLiteral("welcomeSectionTitle"));
	recentHeader->addWidget(recentTitle);
	recentHeader->addStretch();
	auto* seeAllButton = new QPushButton(tr("See All"), m_welcomePage);
	seeAllButton->setObjectName(QStringLiteral("welcomeSeeAllButton"));
	seeAllButton->setProperty("linkAction", true);
	recentHeader->addWidget(seeAllButton);
	recentColumn->addLayout(recentHeader);

	m_welcomeRecentList = new QListWidget(m_welcomePage);
	m_welcomeRecentList->setObjectName(QStringLiteral("welcomeRecentList"));
	m_welcomeRecentList->setProperty("welcomeRecents", true);
	m_welcomeRecentList->setIconSize(QSize(34, 34));
	m_welcomeRecentList->setSpacing(6);
	const int maxRecent = qMin(4, recentDocList.count());
	for (int i = 0; i < maxRecent; ++i)
	{
		const QFileInfo fileInfo(recentDocList.at(i));
		auto* item = new QListWidgetItem(IconManager::instance().loadIcon("document-open"), fileInfo.fileName());
		item->setData(Qt::UserRole, QDir::toNativeSeparators(recentDocList.at(i)));
		item->setToolTip(QDir::toNativeSeparators(recentDocList.at(i)));
		item->setSizeHint(QSize(260, 54));
		m_welcomeRecentList->addItem(item);
	}
	if (maxRecent == 0)
	{
		auto* emptyItem = new QListWidgetItem(tr("No recent documents"));
		emptyItem->setFlags(Qt::NoItemFlags);
		emptyItem->setSizeHint(QSize(260, 54));
		m_welcomeRecentList->addItem(emptyItem);
	}
	recentColumn->addWidget(m_welcomeRecentList);
	content->addLayout(recentColumn, 5);
	pageLayout->addLayout(content, 1);

	auto* showOnStartup = new QCheckBox(tr("Show this window when Scribus opens"), m_welcomePage);
	showOnStartup->setObjectName(QStringLiteral("welcomeShowOnStartup"));
	showOnStartup->setChecked(prefsManager.appPrefs.uiPrefs.showStartupDialog);
	showOnStartup->setAccessibleName(tr("Show the Welcome window when Scribus opens"));
	pageLayout->addWidget(showOnStartup);

	tabWidget->insertTab(0, m_welcomePage, tr("Welcome"));
	tabWidget->tabBar()->hide();

	m_backButton = buttonBox->addButton(tr("Back"), QDialogButtonBox::ResetRole);
	m_backButton->setObjectName(QStringLiteral("welcomeBackButton"));
	m_backButton->setProperty("secondaryAction", true);
	m_backButton->setIcon(IconManager::instance().loadIcon("go-previous"));
	m_backButton->setAccessibleName(tr("Return to Welcome"));

	connect(newButton, &QPushButton::clicked, this, [this]() { tabWidget->setCurrentIndex(1); });
	connect(newHeaderButton, &QPushButton::clicked, this, [this]() { tabWidget->setCurrentIndex(1); });
	connect(openButton, &QPushButton::clicked, this, [this]() { tabWidget->setCurrentIndex(3); });
	connect(openHeaderButton, &QPushButton::clicked, this, [this]() { tabWidget->setCurrentIndex(3); });
	connect(templatesButton, &QPushButton::clicked, this, [this]() { tabWidget->setCurrentIndex(2); });
	connect(seeAllButton, &QPushButton::clicked, this, [this]() { tabWidget->setCurrentIndex(4); });
	connect(m_backButton, &QPushButton::clicked, this, &NewDocDialog::showWelcomePage);
	connect(showOnStartup, &QCheckBox::toggled, this, [this](bool checked) { startUpDialog->setChecked(!checked); });
	connect(m_welcomeRecentList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
		if (!item || !(item->flags() & Qt::ItemIsEnabled))
			return;
		m_selectedFile = QDir::fromNativeSeparators(item->data(Qt::UserRole).toString());
		m_tabSelected = NewDocDialog::OpenRecentTab;
		accept();
	});
}

void NewDocDialog::showWelcomePage()
{
	if (m_onStartup && m_welcomePage)
		tabWidget->setCurrentWidget(m_welcomePage);
}

int NewDocDialog::logicalTabIndex(int widgetTabIndex) const
{
	return m_onStartup ? widgetTabIndex - 1 : widgetTabIndex;
}

void NewDocDialog::createNewDocPage()
{
	double pageHeight = prefsManager.appPrefs.docSetupPrefs.pageHeight;
	double pageWidth = prefsManager.appPrefs.docSetupPrefs.pageWidth;

	comboSortSizes->addItem( tr("Name Asc"), PageSizeList::NameAsc);
	comboSortSizes->addItem( tr("Name Desc"), PageSizeList::NameDesc);
	comboSortSizes->addItem( tr("Size Asc"), PageSizeList::DimensionAsc);
	comboSortSizes->addItem( tr("Size Desc"), PageSizeList::DimensionDesc);
	comboSortSizes->setCurrentIndex(0);

	pageOrientationButtons = new QButtonGroup();
	pageOrientationButtons->addButton(buttonVertical, 0);
	pageOrientationButtons->addButton(buttonHorizontal, 1);
	pageOrientationButtons->button(m_orientation)->setChecked(true);

	pageLayoutButtons = new QButtonGroup();
	pageLayoutButtons->addButton(buttonSinglePage, 0);
	pageLayoutButtons->addButton(buttonDoublePageLeft, 1);
	pageLayoutButtons->addButton(buttonDoublePageRight, 2);
	if (m_choosenLayout == singlePage)
	{
		pageLayoutButtons->button(0)->setChecked(true);
		buttonBindingDirection->setDisabled(true);
	}
	else if (prefsManager.appPrefs.pageSets.at(m_choosenLayout).FirstPage == 0)
	{
		pageLayoutButtons->button(1)->setChecked(true);
		buttonBindingDirection->setDisabled(false);
	}
	else
	{
		pageLayoutButtons->button(2)->setChecked(true);
		buttonBindingDirection->setDisabled(false);
	}

	buttonBindingDirection->setChecked(m_bindingDirection);

	PageCollectionInfo pciPreferred = PagePresetManager::instance().categoryInfoPreferred();

	listPageFormats->setValues(QSizeF(pageWidth, pageHeight), m_orientation, pciPreferred.id, PageSizeList::NameAsc);
	listPageFormats->setIconSize(QSize(64, 64));
	listPageFormats->setGridSize(QSize(132, 124));
	listPageFormats->setFlow(QListView::LeftToRight);
	listPageFormats->setWrapping(false);
	listPageFormats->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	listPageFormats->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	listPageFormats->setMaximumHeight(142);

	QString name;
	if (listPageFormats->currentIndex().isValid())
		name = listPageFormats->currentIndex().data().toString();
	textPagePresetName->setText(name);

	pageSizeSelector->setHasFormatSelector(false);
	pageSizeSelector->setHasCustom(false);
	pageSizeSelector->setPageSize(pageWidth, pageHeight);
	pageSizeSelector->setCurrentCategory(pciPreferred.id);

	widthSpinBox->setMinimum(pts2value(1.0, m_unitIndex));
	widthSpinBox->setMaximum(16777215);
	widthSpinBox->setNewUnit(m_unitIndex);
	widthSpinBox->setSuffix(m_unitSuffix);
	widthSpinBox->setValue(pageWidth * m_unitRatio);

	heightSpinBox->setMinimum(pts2value(1.0, m_unitIndex));
	heightSpinBox->setMaximum(16777215);
	heightSpinBox->setNewUnit(m_unitIndex);
	heightSpinBox->setSuffix(m_unitSuffix);
	heightSpinBox->setValue(pageHeight * m_unitRatio);

	unitOfMeasureComboBox->addItems(unitGetTextUnitList());
	unitOfMeasureComboBox->setCurrentIndex(m_unitIndex);
	unitOfMeasureComboBox->setEditable(false);

	MarginStruct marg(prefsManager.appPrefs.docSetupPrefs.margins);
	marginGroup->setup(marg, !(m_choosenLayout == singlePage), m_unitIndex, NewMarginWidget::MarginWidgetFlags);
	marginGroup->toggleLabelVisibility(false);
	marginGroup->setPageHeight(pageHeight);
	marginGroup->setPageWidth(pageWidth);
	marginGroup->setFacingPages(!(m_choosenLayout == singlePage));
	marginGroup->setMarginPreset(prefsManager.appPrefs.docSetupPrefs.marginPreset);

	MarginStruct bleed(prefsManager.appPrefs.docSetupPrefs.bleeds);
	bleedGroup->setup(bleed, !(m_choosenLayout == singlePage), m_unitIndex, NewMarginWidget::BleedWidgetFlags);
	bleedGroup->toggleLabelVisibility(false);
	bleedGroup->setPageHeight(pageHeight);
	bleedGroup->setPageWidth(pageWidth);
	bleedGroup->setFacingPages(!(m_choosenLayout == singlePage));
	bleedGroup->setMarginPreset(prefsManager.appPrefs.docSetupPrefs.marginPreset);

	pageCountSpinBox->setMaximum( 10000 );
	pageCountSpinBox->setMinimum( 1 );

	IconManager &iconManager = IconManager::instance();
	pageCountLabel->setPixmap(iconManager.loadPixmap("panel-page"));

	setDocLayout(m_choosenLayout);
	setSize(QSizeF(pageWidth, pageHeight));
	setOrientation(m_orientation);

	numberOfCols->setButtonSymbols( QSpinBox::UpDownArrows );
	numberOfCols->setMinimum( 1 );
	numberOfCols->setValue( 1 );

	Distance->setMinimum(0);
	Distance->setMaximum(1000);
	Distance->setNewUnit(m_unitIndex);
	Distance->setValue(11 * m_unitRatio);

	labelColumns->setEnabled(false);
	labelGap->setEnabled(false);
	Distance->setEnabled(false);
	numberOfCols->setEnabled(false);

	startDocSetup->setText( tr( "Show Document Settings After Creation" ) );
	startDocSetup->setChecked(false);

	sectionPreview->collapse();
	sectionPreview->setCanSaveState(true);
	sectionPreview->restorePreferences();

	sectionDocument->expand();
	sectionDocument->setCanSaveState(true);
	sectionDocument->restorePreferences();

	sectionMargins->expand();
	sectionMargins->setCanSaveState(true);
	sectionMargins->restorePreferences();

	sectionBleeds->collapse();
	sectionBleeds->setCanSaveState(true);
	sectionBleeds->restorePreferences();

	sectionTextFrame->collapse();
	sectionTextFrame->setCanSaveState(true);
	sectionTextFrame->restorePreferences();

	labelColumns->setLabelVisibility(m_labelVisibity);
	labelGap->setLabelVisibility(m_labelVisibity);
	pageCountLabel->setLabelVisibility(m_labelVisibity);
	orientationLabel->setLabelVisibility(m_labelVisibity);
	pageLayoutLabel->setLabelVisibility(m_labelVisibity);

	// We have to install an event filter to resize the scroll container width based on the content width.
	// The content width can change after we calculated the initial ui layout.
	scrollAreaWidgetContents->installEventFilter(this);
	scrollAreaWidgetContents->adjustSize();

}

void NewDocDialog::createOpenDocPage()
{
	PrefsContext* docContext = prefsManager.prefsFile->getContext("docdirs", false);
	QString docDir = ".";
	QString prefsDocDir = prefsManager.documentDir();
	if (!prefsDocDir.isEmpty())
		docDir = docContext->get("docsopen", prefsDocDir);
	else
		docDir = docContext->get("docsopen", ".");
	QString formats(FileLoader::getLoadFilterString());
//	formats.remove("PDF (*.pdf *.PDF);;");
	QVBoxLayout *openDocLayout = new QVBoxLayout(tab_3);
	openDocLayout->setContentsMargins(0, 0, 0, 0);
	openDocLayout->setSpacing(4);
	m_selectedFile = "";

	// With Qt 5.15 we have to be in careful so that new document dialog doesn't display too large on startup.
	// To avoid this we have to use QFileDialog(QWidget *parent, Qt::WindowFlags flags) constructor, then
	// set the QFileDialog::DontUseNativeDialog option as early as possible, and nonetheless set again
	// the Qt::Widget window flag before adding the widget to layout.
	fileDialog = new QFileDialog(tab_3, Qt::Widget);
	fileDialog->setOption(QFileDialog::DontUseNativeDialog);
	fileDialog->setWindowTitle(tr("Open"));
	fileDialog->setDirectory(docDir);
	fileDialog->setNameFilter(formats);
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog->setIconProvider(new ImIconProvider());
	fileDialog->setOption(QFileDialog::HideNameFilterDetails, true);
	fileDialog->setOption(QFileDialog::ReadOnly, true);
	fileDialog->setSizeGripEnabled(false);
	fileDialog->setModal(false);
	QList<QPushButton *> pushButtons = fileDialog->findChildren<QPushButton *>();
	for (auto pushButton : std::as_const(pushButtons))
		pushButton->setVisible(false);
	fileDialog->setWindowFlags(Qt::Widget);
	auto* openContentLayout = new QHBoxLayout;
	openContentLayout->setContentsMargins(0, 0, 0, 0);
	openContentLayout->setSpacing(6);
	openContentLayout->addWidget(fileDialog, 1);
	m_openPreviewContainer = new QWidget(tab_3);
	auto* previewLayout = new QVBoxLayout(m_openPreviewContainer);
	previewLayout->setContentsMargins(0, 28, 0, 0);
	m_openPreview = new FDialogPreview(m_openPreviewContainer);
	previewLayout->addWidget(m_openPreview, 0, Qt::AlignTop);
	openContentLayout->addWidget(m_openPreviewContainer);
	openDocLayout->addLayout(openContentLayout);
	m_openPreviewCheck = new QCheckBox(tr("Show Preview"), tab_3);
	m_openPreviewCheck->setObjectName(QStringLiteral("openDocumentShowPreview"));
	m_openPreviewCheck->setToolTip(tr("Show a preview and information for the selected file"));
	openDocLayout->addWidget(m_openPreviewCheck, 0, Qt::AlignLeft);
	m_openPreviewContainer->hide();
	connect(m_openPreviewCheck, &QCheckBox::toggled, this, [this](bool checked) {
		m_openPreviewContainer->setVisible(checked);
		if (!checked)
		{
			m_openPreview->updatePix();
			return;
		}
		const QStringList selectedFiles = fileDialog->selectedFiles();
		if (!selectedFiles.isEmpty())
			m_openPreview->genPreview(QDir::fromNativeSeparators(selectedFiles.first()));
	});


	FileDialogEventCatcher* keyCatcher = new FileDialogEventCatcher(this);
	QList<QListView *> listViews = fileDialog->findChildren<QListView *>();
	for (auto listView : std::as_const(listViews))
		listView->installEventFilter(keyCatcher);
	connect(keyCatcher, SIGNAL(escapePressed()), this, SLOT(reject()));
	connect(keyCatcher, SIGNAL(dropLocation(QString)), this, SLOT(locationDropped(QString)));
	connect(keyCatcher, SIGNAL(desktopPressed()), this, SLOT(gotoDesktopDirectory()));
	connect(keyCatcher, SIGNAL(homePressed()), this, SLOT(gotoHomeDirectory()));
	connect(keyCatcher, SIGNAL(parentPressed()), this, SLOT(gotoParentDirectory()));
	connect(keyCatcher, SIGNAL(enterSelectedPressed()), this, SLOT(gotoSelectedDirectory()));
	connect(fileDialog, SIGNAL(currentChanged(QString)), this, SLOT(openFileDialogFileClicked(QString)));
	connect(fileDialog, SIGNAL(filesSelected(QStringList)), this, SLOT(openFile()));
	connect(fileDialog, SIGNAL(rejected()), this, SLOT(reject()));
}

void NewDocDialog::openFile()
{
	ExitOK();
}

void NewDocDialog::createRecentDocPage()
{
	int max = qMin(prefsManager.appPrefs.uiPrefs.recentDocCount, recentDocList.count());
	for (int i = 0; i < max; ++i)
		recentDocListBox->addItem(QDir::toNativeSeparators(recentDocList[i]));
	if (max>0)
		recentDocListBox->setCurrentRow(0);
}

void NewDocDialog::setWidth(double)
{
	m_pageWidth = widthSpinBox->value() / m_unitRatio;
	marginGroup->setPageWidth(m_pageWidth);
	bleedGroup->setPageWidth(m_pageWidth);
	listPageFormats->clearSelection();
	listPageFormats->setDimensions(m_pageWidth, m_pageHeight);
	pagePreview->setPage(m_pageHeight, m_pageWidth, marginGroup->margins(), bleedGroup->margins(), m_choosenLayout, m_layoutFirstPage);

	int newOrientation = (widthSpinBox->value() > heightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != m_orientation)
	{
		m_orientation = newOrientation;

		QSignalBlocker sigOri(pageOrientationButtons);
		pageOrientationButtons->button(newOrientation)->setChecked(true);
	}

}

void NewDocDialog::setHeight(double)
{
	m_pageHeight = heightSpinBox->value() / m_unitRatio;
	marginGroup->setPageHeight(m_pageHeight);
	bleedGroup->setPageHeight(m_pageHeight);	
	listPageFormats->clearSelection();
	listPageFormats->setDimensions(m_pageWidth, m_pageHeight);
	pagePreview->setPage(m_pageHeight, m_pageWidth, marginGroup->margins(), bleedGroup->margins(), m_choosenLayout, m_layoutFirstPage);

	int newOrientation = (widthSpinBox->value() > heightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != m_orientation)
	{
		m_orientation = newOrientation;

		QSignalBlocker sigOri(pageOrientationButtons);
		pageOrientationButtons->button(newOrientation)->setChecked(true);
	}
}

void NewDocDialog::changePageSize(const QModelIndex &ic)
{
	int unit = ic.data(PageSizeList::Unit).toInt();
	int layout = ic.data(PageSizeList::Layout).toInt();
	layout = layout > -1 ? layout : m_choosenLayout;//prefsManager.appPrefs.docSetupPrefs.pagePositioning;
	int firstPage = ic.data(PageSizeList::FirstPage).toInt();
	firstPage = firstPage > -1 ? firstPage : m_layoutFirstPage;//prefsManager.appPrefs.pageSets.at(layout).FirstPage;
	double width = ic.data(PageSizeList::Width).toDouble();
	double height = ic.data(PageSizeList::Height).toDouble();
	int bDirection = ic.data(PageSizeList::BindingDirection).toInt();
	bool bindingDirection = bDirection > -1 ? bDirection : m_bindingDirection;//prefsManager.appPrefs.docSetupPrefs.bindingDirection;
	textPagePresetName->setText(ic.data(Qt::DisplayRole).toString());

	// Margins
	MarginStruct margins(prefsManager.appPrefs.docSetupPrefs.margins);
	marginGroup->setPageWidth(width);
	marginGroup->setPageHeight(height);
	marginGroup->setMarginPreset(prefsManager.appPrefs.docSetupPrefs.marginPreset);
	QVariant dataMargins = ic.data(PageSizeList::Margins);
	if (dataMargins.canConvert<QList<double>>())
	{
		QList<double> m = dataMargins.value<QList<double>>();
		if (!QVariant(m.at(0)).toBool())
		{
			margins.set(m.at(1), m.at(2), m.at(3), m.at(4));
			marginGroup->setMarginPreset(ic.data(PageSizeList::MarginPreset).toInt());
		}
	}
	marginGroup->setNewValues(margins);

	// Bleeds
	MarginStruct bleeds(prefsManager.appPrefs.docSetupPrefs.bleeds);
	QVariant dataBleeds = ic.data(PageSizeList::Bleeds);
	if (dataBleeds.canConvert<QList<double>>())
	{
		QList<double> m = dataBleeds.value<QList<double>>();
		if (!QVariant(m.at(0)).toBool())
			bleeds.set(m.at(1), m.at(2), m.at(3), m.at(4));
	}
	bleedGroup->setNewValues(bleeds);

	// Text Frame
	autoTextFrame->setChecked(false);
	numberOfCols->setValue( 1 );
	Distance->setValue(11 * m_unitRatio);
	QVariant dataTextFrame = ic.data(PageSizeList::TextFrame);
	if (dataTextFrame.canConvert<QList<double>>())
	{
		QList<double> tf = dataTextFrame.value<QList<double>>();
		if (!tf.isEmpty())
		{
			autoTextFrame->setChecked(true);
			numberOfCols->setValue(QVariant(tf.at(0)).toInt());
			setDistance(QVariant(tf.at(1)).toDouble() * m_unitRatio);
		}
	}

	setPageSize(QSizeF(width, height));
	setUnit(unit);

	// Layout + First Page
	if (layout == 0)
		setLayout(0);
	else
	{
		if (firstPage == 0)
			setLayout(1);
		else
			setLayout(2);
	}

	// BindingDirection
	buttonBindingDirection->setChecked(bindingDirection);
	setBindingDirection(bindingDirection);
}

void NewDocDialog::changeSortMode(int ic)
{
	Q_UNUSED(ic);
	listPageFormats->setSortMode(static_cast<PageSizeList::SortMode>(comboSortSizes->currentData().toInt()));
}

void NewDocDialog::savePagePreset()
{
	if (textPagePresetName->text().isEmpty())
	{
		ScMessageBox::warning(this, tr("Empty Preset Name"), tr("The preset name must not be empty!\nEnter a preset name."), QMessageBox::Ok);
		return;
	}

	const QString presetUserFolder = QDir::toNativeSeparators(ScPaths::userPagePresetsDir(true));

	QString uuid;

	// Collection
	// If another user collection should be updated change the collection information here:
	PageCollectionInfo pci;
	pci.author = tr("User");
	pci.license = tr("Copyright by user");
	pci.name = tr("User");
	pci.filePath = presetUserFolder + "user.xml";

	PagePresetManager::instance().createOrUpdateCollection(pci.filePath, pci, uuid);

	// Page Preset
	PageSizeInfo psi;
	psi.pageUnitIndex = m_unitIndex;
	psi.name = textPagePresetName->text();
	psi.width = m_pageWidth;
	psi.height = m_pageHeight;
	psi.margins = marginGroup->margins();
	psi.bleeds = bleedGroup->margins();
	psi.layout = m_choosenLayout;
	psi.firstPage = m_layoutFirstPage;
	psi.marginPreset = marginGroup->marginPreset();
	psi.bindingDirection = m_bindingDirection;

	QList<double> textFrame;
	if (autoTextFrame->isChecked())
	{
		textFrame.append(numberOfCols->value());
		textFrame.append(m_distance);
	}
	psi.textFrame = textFrame;

	PagePresetManager::instance().addCollectionPage(pci.filePath, psi);

	// Refresh UI
	PagePresetManager::instance().reloadAllPresets();
	pageSizeSelector->setPageSize(m_pageWidth, m_pageHeight);
	pageSizeSelector->setCurrentCategory(uuid);
	updateCategory(uuid, true);
}

void NewDocDialog::updateCategorySelector()
{
	PageCollectionInfo pciPreferred = PagePresetManager::instance().categoryInfoPreferred();
	pageSizeSelector->setPageSize(m_pageWidth, m_pageHeight);
	pageSizeSelector->setCurrentCategory(pciPreferred.id);
	updateCategory(pciPreferred.id, true);
}

bool NewDocDialog::eventFilter(QObject *object, QEvent *event)
{
	if (object->objectName() == "scrollAreaWidgetContents" && event->type() == QEvent::Resize)
	{
		int currentWidth = scrollArea->minimumWidth();
		scrollArea->setMinimumWidth(qMax(scrollAreaWidgetContents->sizeHint().width() + qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent), currentWidth));
		return true;
	}
	return false;
}

void NewDocDialog::handleAutoFrame()
{
	bool setter = autoTextFrame->isChecked();
	labelColumns->setEnabled(setter);
	labelGap->setEnabled(setter);
	Distance->setEnabled(setter);
	numberOfCols->setEnabled(setter);
}

void NewDocDialog::setDistance(double value)
{
	QSignalBlocker sig(Distance);
	Distance->setValue(value);

	m_distance = value / m_unitRatio;
}

void NewDocDialog::setUnit(int newUnitIndex)
{
	QSignalBlocker sig(unitOfMeasureComboBox);
	unitOfMeasureComboBox->setCurrentIndex(newUnitIndex);

	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
	widthSpinBox->setNewUnit(newUnitIndex);
	heightSpinBox->setNewUnit(newUnitIndex);
	Distance->setNewUnit(newUnitIndex);
	m_unitRatio = unitGetRatioFromIndex(newUnitIndex);
	m_unitIndex = newUnitIndex;
	widthSpinBox->setValue(m_pageWidth * m_unitRatio);
	heightSpinBox->setValue(m_pageHeight * m_unitRatio);

	marginGroup->setNewUnit(m_unitIndex);
	marginGroup->setPageHeight(m_pageHeight);
	marginGroup->setPageWidth(m_pageWidth);
	bleedGroup->setNewUnit(m_unitIndex);
	bleedGroup->setPageHeight(m_pageHeight);
	bleedGroup->setPageWidth(m_pageWidth);
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));


}

void NewDocDialog::ExitOK()
{
	m_pageWidth = widthSpinBox->value() / m_unitRatio;
	m_pageHeight = heightSpinBox->value() / m_unitRatio;
	m_bleedBottom = bleedGroup->margins().bottom();
	m_bleedTop = bleedGroup->margins().top();
	m_bleedLeft = bleedGroup->margins().left();
	m_bleedRight = bleedGroup->margins().right();
	if (m_onStartup)
	{
		m_tabSelected = logicalTabIndex(tabWidget->currentIndex());
		if (m_tabSelected < NewDocDialog::NewDocumentTab)
			return;
		if (m_tabSelected == NewDocDialog::NewFromTemplateTab) // new doc from template
		{
			if (nftGui->currentDocumentTemplate)
			{
				m_selectedFile = QDir::fromNativeSeparators(nftGui->currentDocumentTemplate->file);
				m_selectedFile = QDir::cleanPath(m_selectedFile);
			}
		}
		else if (m_tabSelected == NewDocDialog::OpenExistingTab) // open existing doc
		{
			QStringList files = fileDialog->selectedFiles();
			if (files.count() != 0)
				m_selectedFile = QDir::fromNativeSeparators(files[0]);
			QFileInfo fi(m_selectedFile);
			if (fi.isDir())
			{
				fileDialog->setDirectory(fi.absoluteFilePath());
				return;
			}
		}
		else if (m_tabSelected == NewDocDialog::OpenRecentTab) // open recent doc
		{
			if (recentDocListBox->currentItem() != nullptr)
			{
				QString fileName(recentDocListBox->currentItem()->text());
				if (!fileName.isEmpty())
					m_selectedFile = QDir::fromNativeSeparators(fileName);
			}
		}
	}
	else
		m_tabSelected = NewDocDialog::NewDocumentTab;
	accept();
}

void NewDocDialog::setOrientation(int ori)
{
	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));

	bool isPortrait = ori == portraitPage;
	double w = widthSpinBox->value(), h = heightSpinBox->value();
	double pw = m_pageWidth, ph = m_pageHeight;
	widthSpinBox->setValue(isPortrait ? qMin(w, h) : qMax(w, h));
	heightSpinBox->setValue(isPortrait ? qMax(w, h) : qMin(w, h));

	m_pageWidth = isPortrait ? qMin(pw, ph) : qMax(pw, ph);
	m_pageHeight = isPortrait ? qMax(pw, ph) : qMin(pw, ph);

	m_orientation = ori;

	marginGroup->setPageHeight(m_pageHeight);
	marginGroup->setPageWidth(m_pageWidth);
	bleedGroup->setPageHeight(m_pageHeight);
	bleedGroup->setPageWidth(m_pageWidth);
	pagePreview->setPage(m_pageHeight, m_pageWidth, marginGroup->margins(), bleedGroup->margins(), m_choosenLayout, m_layoutFirstPage);

	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
}

void NewDocDialog::setLayout(int layoutId)
{
	switch (layoutId)
	{
		case 0:
			setDocLayout(0);
			buttonBindingDirection->setDisabled(true);
			pageLayoutButtons->button(0)->setChecked(true); // single page
		break;
		case 1:
			setDocLayout(1);
			pagePreview->setFirstPage(0);
			setDocFirstPage(0);
			buttonBindingDirection->setDisabled(false);
			pageLayoutButtons->button(1)->setChecked(true); // double page + first page left
		break;
		case 2:
			setDocLayout(1);
			pagePreview->setFirstPage(1);
			setDocFirstPage(1);
			buttonBindingDirection->setDisabled(false);
			pageLayoutButtons->button(2)->setChecked(true); // double page + first page right
		break;
	}
}

void NewDocDialog::setBindingDirection(bool checked)
{
	m_bindingDirection = int(checked);
}

void NewDocDialog::setPageSize(QSizeF size)
{
	if (size.width() < size.height())
		pageOrientationButtons->button(portraitPage)->setChecked(true);
	else
		pageOrientationButtons->button(landscapePage)->setChecked(true);

	setSize(size);
}

void NewDocDialog::setSize(QSizeF size)
{
	m_pageWidth = widthSpinBox->value() / m_unitRatio;
	m_pageHeight = heightSpinBox->value() / m_unitRatio;

	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));

	m_pageWidth = size.width();
	m_pageHeight = size.height();

	widthSpinBox->setValue(m_pageWidth * m_unitRatio);
	heightSpinBox->setValue(m_pageHeight * m_unitRatio);
	marginGroup->setPageHeight(m_pageHeight);
	marginGroup->setPageWidth(m_pageWidth);
	bleedGroup->setPageHeight(m_pageHeight);
	bleedGroup->setPageWidth(m_pageWidth);
	pagePreview->setPage(m_pageHeight, m_pageWidth, marginGroup->margins(), bleedGroup->margins(), m_choosenLayout, m_layoutFirstPage);

	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));

}

void NewDocDialog::setDocLayout(int layout)
{
	marginGroup->setFacingPages(layout != singlePage);
	bleedGroup->setFacingPages(layout != singlePage);
	m_choosenLayout = layout;
	m_layoutFirstPage = prefsManager.appPrefs.pageSets.at(m_choosenLayout).FirstPage;
	pagePreview->setPage(m_pageHeight, m_pageWidth, marginGroup->margins(), bleedGroup->margins(), m_choosenLayout, m_layoutFirstPage);
}

void NewDocDialog::setDocFirstPage(int firstPage)
{
	m_layoutFirstPage = firstPage;
}

void NewDocDialog::recentDocListBox_doubleClicked()
{
	/* Yep. There is nothing to solve. ScribusMainWindow handles all
	openings etc. It's Franz's programming style ;) */
	ExitOK();
}

void NewDocDialog::adjustTitles(int tab)
{
	QPushButton* primaryButton = buttonBox->button(QDialogButtonBox::Ok);
	const int logicalTab = logicalTabIndex(tab);
	const bool isWelcome = m_onStartup && logicalTab < NewDocDialog::NewDocumentTab;
	buttonBox->setVisible(!isWelcome);
	startUpDialog->setVisible(false);
	if (m_backButton)
		m_backButton->setVisible(!isWelcome);
	if (isWelcome)
	{
		setWindowTitle(tr("Scribus"));
		return;
	}
	if (logicalTab == NewDocDialog::NewDocumentTab)
	{
		setWindowTitle(tr("New Document"));
		if (primaryButton)
			primaryButton->setText(tr("Create"));
	}
	else if (logicalTab == NewDocDialog::NewFromTemplateTab)
	{
		setWindowTitle(tr("New from Template"));
		if (primaryButton)
			primaryButton->setText(tr("Create"));
	}
	else if (logicalTab == NewDocDialog::OpenExistingTab)
	{
		setWindowTitle(tr("Open Existing Document"));
		if (primaryButton)
			primaryButton->setText(tr("Open"));
	}
	else if (logicalTab == NewDocDialog::OpenRecentTab)
	{
		setWindowTitle(tr("Open Recent Document"));
		if (primaryButton)
			primaryButton->setText(tr("Open"));
	}
	else
	{
		setWindowTitle(tr("New Document"));
		if (primaryButton)
			primaryButton->setText(tr("Create"));
	}
	//okButton->setEnabled(tab!=2);
}

void NewDocDialog::locationDropped(const QString& fileUrl)
{
	QFileInfo fi(fileUrl);
	if (fi.isDir())
		fileDialog->setDirectory(fi.absoluteFilePath());
	else
	{
		fileDialog->setDirectory(fi.absolutePath());
		fileDialog->selectFile(fi.fileName());
	}
}

void NewDocDialog::gotoParentDirectory()
{
	QDir d(fileDialog->directory());
	d.cdUp();
	fileDialog->setDirectory(d);
}


void NewDocDialog::gotoSelectedDirectory()
{
	QStringList s(fileDialog->selectedFiles());
	if (s.isEmpty())
		return;
	QFileInfo fi(s.first());
	if (fi.isDir())
		fileDialog->setDirectory(fi.absoluteFilePath());
}

void NewDocDialog::gotoDesktopDirectory()
{
	QString dp = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	QFileInfo fi(dp);
	if (fi.exists())
		fileDialog->setDirectory(dp);
}


void NewDocDialog::gotoHomeDirectory()
{
	QString dp = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
	QFileInfo fi(dp);
	if (fi.exists())
		fileDialog->setDirectory(dp);
}

void NewDocDialog::openFileDialogFileClicked(const QString& path)
{
	if (m_openPreviewCheck && m_openPreviewCheck->isChecked())
		m_openPreview->genPreview(path);
}

void NewDocDialog::changeMargin(MarginStruct margin)
{
	pagePreview->setMargins(margin);
}

void NewDocDialog::changeBleed(MarginStruct bleed)
{
	pagePreview->setBleeds(bleed);
}

void NewDocDialog::changeCategory(const QString &category)
{
	updateCategory(category);
}

void NewDocDialog::updateCategory(const QString &category, bool forceUpdate)
{
	if (listPageFormats->category() != category || forceUpdate)
		listPageFormats->setValues(QSizeF(m_pageWidth, m_pageHeight), listPageFormats->orientation(), category, listPageFormats->sortMode());
}
