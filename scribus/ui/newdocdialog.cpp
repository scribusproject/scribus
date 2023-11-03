/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "newdocdialog.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDir>
#include <QCheckBox>
#include <QComboBox>
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
#include <QToolTip>

#include "scconfig.h"

#include "commonstrings.h"
#include "fileloader.h"
#include "marginwidget.h"
#include "pagesize.h"
#include "scribuscore.h"
#include "pagelayout.h"
#include "pagestructs.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "filedialogeventcatcher.h"
#include "scrspinbox.h"
#include "units.h"
#include "iconmanager.h"

PageLayoutsWidget::PageLayoutsWidget(QWidget* parent) :
	QListWidget(parent)
{
	setDragEnabled(false);
	setViewMode(QListView::IconMode);
	setFlow(QListView::LeftToRight);
	setSortingEnabled(false);
	setWrapping(false);
	setWordWrap(true);
	setAcceptDrops(false);
	setDropIndicatorShown(false);
	setDragDropMode(QAbstractItemView::NoDragDrop);
	setResizeMode(QListView::Adjust);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setFocusPolicy(Qt::NoFocus);
	setIconSize(QSize(32, 32));
	clear();
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void PageLayoutsWidget::arrangeIcons()
{
	int startY = 5;
	int startX = 5;
	int maxSizeY = 0;

	setResizeMode(QListView::Fixed);

	for (int i = 0; i < count(); ++i)
	{
		QListWidgetItem* itemWidget = item(i);
		QRect itemRect = visualItemRect(itemWidget);
		setPositionForIndex(QPoint(startX, startY), indexFromItem(itemWidget));
		startX += itemRect.width() + 5;
		maxSizeY = qMax(maxSizeY, itemRect.height());
	}

	maxX = startX;
	maxY = maxSizeY + 10;
}

QSize PageLayoutsWidget::minimumSizeHint() const
{
	return QSize(maxX, maxY);
}

NewDocDialog::NewDocDialog(QWidget* parent, const QStringList& recentDocs, bool startUp, const QString& lang) : QDialog(parent),
	prefsManager(PrefsManager::instance()),
	m_onStartup(startUp)
{
	setObjectName(QString::fromLocal8Bit("NewDocumentWindow"));
	setModal(true);

	m_unitIndex = prefsManager.appPrefs.docSetupPrefs.docUnitIndex;
	m_unitRatio = unitGetRatioFromIndex(m_unitIndex);
	m_unitSuffix = unitGetSuffixFromIndex(m_unitIndex);
	m_orientation = prefsManager.appPrefs.docSetupPrefs.pageOrientation;

	setWindowTitle( tr( "New Document" ) );
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	if (startUp)
		setContentsMargins(0, 0, 0, 0);
	else
		setContentsMargins(9, 9, 9, 9);
	TabbedNewDocLayout = new QVBoxLayout( this );
	if (startUp)
		TabbedNewDocLayout->setContentsMargins(9, 9, 9, 9);
	else
		TabbedNewDocLayout->setContentsMargins(0, 0, 0, 0);
	TabbedNewDocLayout->setSpacing(6);
	createNewDocPage();
	if (startUp)
	{
		tabWidget = new QTabWidget( this );
		tabWidget->addTab(newDocFrame, tr("&New Document"));
		createNewFromTempPage();
		nftGui->setupSettings(lang);
		tabWidget->addTab(newFromTempFrame, tr("New &from Template"));
		createOpenDocPage();
		tabWidget->addTab(openDocFrame, tr("Open &Existing Document"));
		recentDocList = recentDocs;
 		createRecentDocPage();
 		tabWidget->addTab(recentDocFrame, tr("Open Recent &Document"));
 		TabbedNewDocLayout->addWidget(tabWidget);
	}
	else
		TabbedNewDocLayout->addWidget(newDocFrame);

	Layout1 = new QHBoxLayout;
	Layout1->setContentsMargins(0, 0, 0, 0);
	Layout1->setSpacing(6);
	if (startUp)
	{
		startUpDialog = new QCheckBox( tr( "Do not show this dialog again" ), this );
		startUpDialog->setChecked(!prefsManager.appPrefs.uiPrefs.showStartupDialog);
		Layout1->addWidget( startUpDialog );
	}
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );
	buttonBox = new QDialogButtonBox();
	Layout1->addWidget(buttonBox);
	okButton = new QPushButton( CommonStrings::tr_OK, this );
	okButton->setDefault( true );
	buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	cancelButton->setAutoDefault( false );
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);
	TabbedNewDocLayout->addLayout( Layout1 );
	//tooltips
	pageSizeComboBox->setToolTip( tr( "Document page size, either a standard size or a custom size" ) );
	pageOrientationComboBox->setToolTip( tr( "Orientation of the document's pages" ) );
	widthSpinBox->setToolTip( tr( "Width of the document's pages, editable if you have chosen a custom page size" ) );
	heightSpinBox->setToolTip( tr( "Height of the document's pages, editable if you have chosen a custom page size" ) );
	pageCountSpinBox->setToolTip( tr( "Initial number of pages of the document" ) );
	unitOfMeasureComboBox->setToolTip( tr( "Default unit of measurement for document editing" ) );
	autoTextFrame->setToolTip( tr( "Create text frames automatically when new pages are added" ) );
	numberOfCols->setToolTip( tr( "Number of columns to create in automatically created text frames" ) );
	Distance->setToolTip( tr( "Distance between automatically created columns" ) );

	// signals and slots connections
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(ExitOK()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize(const QString &)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrientation(int)));
	connect(unitOfMeasureComboBox, SIGNAL(activated(int)), this, SLOT(setUnit(int)));
	connect(Distance, SIGNAL(valueChanged(double)), this, SLOT(setDistance(double)));
	connect(autoTextFrame, SIGNAL(clicked()), this, SLOT(handleAutoFrame()));
	connect(layoutsView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	if (startUp)
	{
		connect(nftGui, SIGNAL(leaveOK()), this, SLOT(ExitOK()));
		connect(recentDocListBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(recentDocListBox_doubleClicked()));
		connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(adjustTitles(int)));
	}
}

void NewDocDialog::createNewDocPage()
{
	newDocFrame = new QFrame(this);

	pageSizeGroupBox = new QGroupBox(newDocFrame );
	pageSizeGroupBox->setTitle( tr( "Document Layout" ) );
	pageSizeGroupBoxLayout = new QGridLayout( pageSizeGroupBox );
	pageSizeGroupBoxLayout->setContentsMargins(9, 9, 9, 9);
	pageSizeGroupBoxLayout->setSpacing(6);
	pageSizeGroupBoxLayout->setAlignment( Qt::AlignTop );

	layoutsView = new PageLayoutsWidget( pageSizeGroupBox );
	layoutsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	for (int i = 0; i < prefsManager.appPrefs.pageSets.count(); ++i)
	{
		QListWidgetItem *ic;
		QString psname = CommonStrings::translatePageSetString(prefsManager.appPrefs.pageSets[i].Name);
		if (i == 0)
		{
			ic = new QListWidgetItem( IconManager::instance().loadIcon("32/page-simple.png"), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
		else if (i == 1)
		{
			ic = new QListWidgetItem( IconManager::instance().loadIcon("32/page-doublesided.png"), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
		else if (i == 2 && prefsManager.appPrefs.docSetupPrefs.pagePositioning == 2)
		{
			ic = new QListWidgetItem( IconManager::instance().loadIcon("32/page-3fold.png"), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
		else if (i == 3 && prefsManager.appPrefs.docSetupPrefs.pagePositioning == 3)
		{
			ic = new QListWidgetItem( IconManager::instance().loadIcon("32/page-4fold.png"), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
	}
	layoutsView->arrangeIcons();
	pageSizeGroupBoxLayout->addWidget( layoutsView, 0, 0, 5, 1 );
	layoutsView->arrangeIcons();

	TextLabel1 = new QLabel( tr( "&Size:" ), pageSizeGroupBox );
	pageSizeGroupBoxLayout->addWidget( TextLabel1, 0, 1 );
	PageSize ps(prefsManager.appPrefs.docSetupPrefs.pageSize);
	pageSizeComboBox = new QComboBox( pageSizeGroupBox );
	pageSizeComboBox->addItems(ps.activeSizeTRList());
	pageSizeComboBox->addItem( CommonStrings::trCustomPageSize );
	pageSizeComboBox->setEditable(false);
	TextLabel1->setBuddy(pageSizeComboBox);
	pageSizeGroupBoxLayout->addWidget(pageSizeComboBox, 0, 2 );
	TextLabel2 = new QLabel( tr( "Orie&ntation:" ), pageSizeGroupBox );
	pageSizeGroupBoxLayout->addWidget( TextLabel2, 1, 1 );
	pageOrientationComboBox = new QComboBox( pageSizeGroupBox );
	pageOrientationComboBox->addItem( tr( "Portrait" ) );
	pageOrientationComboBox->addItem( tr( "Landscape" ) );
	pageOrientationComboBox->setEditable(false);
	pageOrientationComboBox->setCurrentIndex(prefsManager.appPrefs.docSetupPrefs.pageOrientation);
	TextLabel2->setBuddy(pageOrientationComboBox);
	pageSizeGroupBoxLayout->addWidget( pageOrientationComboBox, 1, 2 );

	TextLabel1_2 = new QLabel( tr( "&Width:" ), pageSizeGroupBox );
	pageSizeGroupBoxLayout->addWidget(TextLabel1_2, 2, 1 );
	widthSpinBox = new ScrSpinBox(pts2value(1.0, m_unitIndex), 16777215, pageSizeGroupBox, m_unitIndex );
	widthSpinBox->setSuffix(m_unitSuffix);
	TextLabel1_2->setBuddy(widthSpinBox);
	pageSizeGroupBoxLayout->addWidget(widthSpinBox, 2, 2 );
	TextLabel2_2 = new QLabel( tr( "&Height:" ), pageSizeGroupBox );
	pageSizeGroupBoxLayout->addWidget(TextLabel2_2, 3, 1 );
	heightSpinBox = new ScrSpinBox( pts2value(1.0, m_unitIndex), 16777215, pageSizeGroupBox, m_unitIndex );
	heightSpinBox->setSuffix(m_unitSuffix);
	TextLabel2_2->setBuddy(heightSpinBox);
	pageSizeGroupBoxLayout->addWidget(heightSpinBox, 3, 2 );

	unitOfMeasureLabel = new QLabel( tr( "&Default Unit:" ), pageSizeGroupBox );
	unitOfMeasureComboBox = new QComboBox( pageSizeGroupBox );
	unitOfMeasureComboBox->addItems(unitGetTextUnitList());
	unitOfMeasureComboBox->setCurrentIndex(m_unitIndex);
	unitOfMeasureComboBox->setEditable(false);
	unitOfMeasureLabel->setBuddy(unitOfMeasureComboBox);
	pageSizeGroupBoxLayout->addWidget( unitOfMeasureLabel, 4, 1 );
	pageSizeGroupBoxLayout->addWidget( unitOfMeasureComboBox, 4, 2 );

	MarginStruct marg(prefsManager.appPrefs.docSetupPrefs.margins);
	marginGroup = new MarginWidget(newDocFrame,  tr( "Margin Guides" ), &marg, m_unitIndex );
	marginGroup->setPageWidthHeight(prefsManager.appPrefs.docSetupPrefs.pageWidth, prefsManager.appPrefs.docSetupPrefs.pageHeight);
	marginGroup->setFacingPages(!(prefsManager.appPrefs.docSetupPrefs.pagePositioning == singlePage));
	widthSpinBox->setValue(prefsManager.appPrefs.docSetupPrefs.pageWidth * m_unitRatio);
	heightSpinBox->setValue(prefsManager.appPrefs.docSetupPrefs.pageHeight * m_unitRatio);
	QStringList pageSizes = ps.activeSizeTRList();
	int sizeIndex = pageSizes.indexOf(ps.nameTR());
	if (sizeIndex != -1)
		pageSizeComboBox->setCurrentIndex(sizeIndex);
	else
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count() - 1);
	marginGroup->setPageSize(pageSizeComboBox->currentText());
	marginGroup->setNewBleeds(prefsManager.appPrefs.docSetupPrefs.bleeds);
	marginGroup->setMarginPreset(prefsManager.appPrefs.docSetupPrefs.marginPreset);

	optionsGroupBox = new QGroupBox( newDocFrame );
	optionsGroupBox->setTitle( tr( "Options" ) );
	optionsGroupBoxLayout = new QGridLayout( optionsGroupBox );
	optionsGroupBoxLayout->setSpacing(6);
	optionsGroupBoxLayout->setContentsMargins(9, 9, 9, 9);
	optionsGroupBoxLayout->setAlignment( Qt::AlignTop );
	pageCountLabel = new QLabel( tr( "N&umber of Pages:" ), optionsGroupBox );

	pageCountSpinBox = new QSpinBox( optionsGroupBox );
	pageCountSpinBox->setMaximum( 10000 );
	pageCountSpinBox->setMinimum( 1 );
	pageCountLabel->setBuddy(pageCountSpinBox);
	optionsGroupBoxLayout->addWidget(pageCountLabel, 0, 0, Qt::AlignLeft);
	optionsGroupBoxLayout->addWidget(pageCountSpinBox, 0, 1);

	layoutLabel1 = new QLabel( optionsGroupBox );
	layoutLabel1->setText( tr( "First Page is:" ) );
	firstPage = new QComboBox( optionsGroupBox );
	firstPage->clear();
	selectItem(prefsManager.appPrefs.docSetupPrefs.pagePositioning);
	optionsGroupBoxLayout->addWidget(layoutLabel1, 1, 0, Qt::AlignLeft);
	optionsGroupBoxLayout->addWidget(firstPage, 1, 1);
	firstPage->setCurrentIndex(prefsManager.appPrefs.pageSets[prefsManager.appPrefs.docSetupPrefs.pagePositioning].FirstPage);

	setDocLayout(prefsManager.appPrefs.docSetupPrefs.pagePositioning);
	setSize(prefsManager.appPrefs.docSetupPrefs.pageSize);
	setOrientation(prefsManager.appPrefs.docSetupPrefs.pageOrientation);

	autoTextFrame = new QCheckBox( optionsGroupBox );
	autoTextFrame->setText( tr( "&Automatic Text Frames" ) );
	optionsGroupBoxLayout->addWidget(autoTextFrame, 2, 0, 1, 2, Qt::AlignLeft);

	TextLabel3 = new QLabel( tr( "Colu&mns:" ), optionsGroupBox );
	numberOfCols = new QSpinBox( optionsGroupBox );
	numberOfCols->setButtonSymbols( QSpinBox::UpDownArrows );
	numberOfCols->setMinimum( 1 );
	numberOfCols->setValue( 1 );
	TextLabel3->setBuddy(numberOfCols);
	optionsGroupBoxLayout->addWidget(TextLabel3, 3, 0, Qt::AlignLeft);
	optionsGroupBoxLayout->addWidget(numberOfCols, 3, 1);

	TextLabel4 = new QLabel( tr( "&Gap:" ), optionsGroupBox );
	Distance = new ScrSpinBox( 0, 1000, optionsGroupBox, m_unitIndex );
	Distance->setValue(11 * m_unitRatio);
	TextLabel4->setBuddy(Distance);
	optionsGroupBoxLayout->addWidget(TextLabel4, 4, 0, Qt::AlignLeft);
	optionsGroupBoxLayout->addWidget(Distance, 4, 1);

	firstPage->setMinimumWidth(Distance->width());

	TextLabel3->setEnabled(false);
	TextLabel4->setEnabled(false);
	Distance->setEnabled(false);
	numberOfCols->setEnabled(false);
	startDocSetup = new QCheckBox( optionsGroupBox );
	startDocSetup->setText( tr( "Show Document Settings After Creation" ) );
	startDocSetup->setChecked(false);
	optionsGroupBoxLayout->addWidget(startDocSetup, 5, 0, 1, 2, Qt::AlignLeft);

	NewDocLayout = new QGridLayout( newDocFrame );
	if (m_onStartup)
		NewDocLayout->setContentsMargins(9, 9, 9, 9);
	else
		NewDocLayout->setContentsMargins(0, 0, 0, 0);
	NewDocLayout->setSpacing(6);
	NewDocLayout->addWidget( marginGroup, 1, 0 );
	NewDocLayout->addWidget( optionsGroupBox, 1, 1 );
	NewDocLayout->addWidget( pageSizeGroupBox, 0, 0, 1, 2);
}

void NewDocDialog::createNewFromTempPage()
{
	newFromTempFrame = new QFrame(this);
	verticalLayout = new QVBoxLayout(newFromTempFrame);
	verticalLayout->setContentsMargins(9, 9, 9, 9);
	nftGui = new nftwidget(newFromTempFrame);
	verticalLayout->addWidget(nftGui);
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
	openDocFrame = new QFrame(this);
	openDocLayout = new QVBoxLayout(openDocFrame);
	openDocLayout->setContentsMargins(0, 0, 0, 0);
	openDocLayout->setSpacing(6);
	m_selectedFile = "";

	// With Qt 5.15 we have to be in careful so that new document dialog doesn't display too large on startup.
	// To avoid this we have to use QFileDialog(QWidget *parent, Qt::WindowFlags flags) constructor, then
	// set the QFileDialog::DontUseNativeDialog option as early as possible, and nonetheless set again
	// the Qt::Widget window flag before adding the widget to layout.
	fileDialog = new QFileDialog(openDocFrame, Qt::Widget);
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
	for (auto pushButton : qAsConst(pushButtons))
		pushButton->setVisible(false);
	fileDialog->setWindowFlags(Qt::Widget);
	openDocLayout->addWidget(fileDialog);

	FileDialogEventCatcher* keyCatcher = new FileDialogEventCatcher(this);
	QList<QListView *> listViews = fileDialog->findChildren<QListView *>();
	for (auto listView : qAsConst(listViews))
		listView->installEventFilter(keyCatcher);
	connect(keyCatcher, SIGNAL(escapePressed()), this, SLOT(reject()));
	connect(keyCatcher, SIGNAL(dropLocation(QString)), this, SLOT(locationDropped(QString)));
	connect(keyCatcher, SIGNAL(desktopPressed()), this, SLOT(gotoDesktopDirectory()));
	connect(keyCatcher, SIGNAL(homePressed()), this, SLOT(gotoHomeDirectory()));
	connect(keyCatcher, SIGNAL(parentPressed()), this, SLOT(gotoParentDirectory()));
	connect(keyCatcher, SIGNAL(enterSelectedPressed()), this, SLOT(gotoSelectedDirectory()));
	connect(fileDialog, SIGNAL(currentChanged(const QString &)), this, SLOT(openFileDialogFileClicked(const QString &)));
	connect(fileDialog, SIGNAL(filesSelected(const QStringList &)), this, SLOT(openFile()));
	connect(fileDialog, SIGNAL(rejected()), this, SLOT(reject()));
}

void NewDocDialog::openFile()
{
	ExitOK();
}

void NewDocDialog::createRecentDocPage()
{
	recentDocFrame = new QFrame(this);
	recentDocLayout = new QVBoxLayout(recentDocFrame);
	recentDocLayout->setContentsMargins(9, 9, 9, 9);
	recentDocLayout->setSpacing(6);
	recentDocListBox = new QListWidget(recentDocFrame);
	recentDocLayout->addWidget(recentDocListBox);
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
	QString psText = pageSizeComboBox->currentText();
	if (psText != CommonStrings::trCustomPageSize && psText != CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count() - 1);
	int newOrientation = (widthSpinBox->value() > heightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != m_orientation)
	{
		pageOrientationComboBox->blockSignals(true);
		pageOrientationComboBox->setCurrentIndex(newOrientation);
		pageOrientationComboBox->blockSignals(false);
		m_orientation = newOrientation;
	}
}

void NewDocDialog::setHeight(double)
{
	m_pageHeight = heightSpinBox->value() / m_unitRatio;
	marginGroup->setPageHeight(m_pageHeight);
	QString psText = pageSizeComboBox->currentText();
	if (psText != CommonStrings::trCustomPageSize && psText != CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count() - 1);
	int newOrientation = (widthSpinBox->value() > heightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != m_orientation)
	{
		pageOrientationComboBox->blockSignals(true);
		pageOrientationComboBox->setCurrentIndex(newOrientation);
		pageOrientationComboBox->blockSignals(false);
		m_orientation = newOrientation;
	}
}

void NewDocDialog::selectItem(uint nr)
{
	disconnect(layoutsView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	disconnect(layoutsView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	disconnect(layoutsView, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	disconnect(layoutsView, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	if (nr > 0)
	{
		const QStringList& pageNames = prefsManager.appPrefs.pageSets[nr].pageNames;
		firstPage->setEnabled(true);
		firstPage->clear();
		for (auto pNames = pageNames.begin(); pNames != pageNames.end(); ++pNames)
			firstPage->addItem(CommonStrings::translatePageSetLocString((*pNames)));
	}
	else
	{
		firstPage->clear();
		firstPage->addItem(" ");
		firstPage->setEnabled(false);
	}
	layoutsView->setCurrentRow(nr);
	layoutsView->item(nr)->setSelected(true);
	connect(layoutsView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(itemSelected(QListWidgetItem* )));
}

void NewDocDialog::itemSelected(QListWidgetItem* ic)
{
	if (ic == nullptr)
		return;
	selectItem(layoutsView->row(ic));
	setDocLayout(layoutsView->row(ic));
}

void NewDocDialog::handleAutoFrame()
{
	if (autoTextFrame->isChecked())
	{
		TextLabel3->setEnabled(true);
		TextLabel4->setEnabled(true);
		Distance->setEnabled(true);
		numberOfCols->setEnabled(true);
	}
	else
	{
		TextLabel3->setEnabled(false);
		TextLabel4->setEnabled(false);
		Distance->setEnabled(false);
		numberOfCols->setEnabled(false);
	}
}

void NewDocDialog::setDistance(double)
{
	m_distance = Distance->value() / m_unitRatio;
}

void NewDocDialog::setUnit(int newUnitIndex)
{
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
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
}

void NewDocDialog::ExitOK()
{
	m_pageWidth = widthSpinBox->value() / m_unitRatio;
	m_pageHeight = heightSpinBox->value() / m_unitRatio;
	m_bleedBottom = marginGroup->bottomBleed();
	m_bleedTop = marginGroup->topBleed();
	m_bleedLeft = marginGroup->leftBleed();
	m_bleedRight = marginGroup->rightBleed();
	if (m_onStartup)
	{
		m_tabSelected = tabWidget->currentIndex();
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
	if (ori != m_orientation)
	{
		double w  = widthSpinBox->value(), h = heightSpinBox->value();
		double pw = m_pageWidth, ph = m_pageHeight;
		widthSpinBox->setValue((ori == portraitPage) ? qMin(w, h) : qMax(w, h));
		heightSpinBox->setValue((ori == portraitPage) ? qMax(w, h) : qMin(w, h));
		m_pageWidth  = (ori == portraitPage) ? qMin(pw, ph) : qMax(pw, ph);
		m_pageHeight = (ori == portraitPage) ? qMax(pw, ph) : qMin(pw, ph);
	}
	// #869 pv - defined constants added + code repeat (check w/h)
	(ori == portraitPage) ? m_orientation = portraitPage : m_orientation = landscapePage;
	if (pageSizeComboBox->currentText() == CommonStrings::trCustomPageSize)
	{
		if (widthSpinBox->value() > heightSpinBox->value())
			pageOrientationComboBox->setCurrentIndex(landscapePage);
		else
			pageOrientationComboBox->setCurrentIndex(portraitPage);
	}
	// end of #869
	marginGroup->setPageHeight(m_pageHeight);
	marginGroup->setPageWidth(m_pageWidth);
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
}

void NewDocDialog::setPageSize(const QString &size)
{
	if (size == CommonStrings::trCustomPageSize)
		setSize(size);
	else
	{
		setSize(size);
		setOrientation(pageOrientationComboBox->currentIndex());
	}
	marginGroup->setPageSize(size);
}

void NewDocDialog::setSize(const QString& gr)
{
	m_pageWidth = widthSpinBox->value() / m_unitRatio;
	m_pageHeight = heightSpinBox->value() / m_unitRatio;

	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
	if (gr == CommonStrings::trCustomPageSize || gr == CommonStrings::customPageSize)
	{
		widthSpinBox->setEnabled(true);
		heightSpinBox->setEnabled(true);
	}
	else
	{
		PageSize ps2(gr);
		if (pageOrientationComboBox->currentIndex() == portraitPage)
		{
			m_pageWidth = ps2.width();
			m_pageHeight = ps2.height();
		}
		else
		{
			m_pageWidth = ps2.height();
			m_pageHeight = ps2.width();
		}
	}
	widthSpinBox->setValue(m_pageWidth * m_unitRatio);
	heightSpinBox->setValue(m_pageHeight * m_unitRatio);
	marginGroup->setPageHeight(m_pageHeight);
	marginGroup->setPageWidth(m_pageWidth);
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
}

void NewDocDialog::setDocLayout(int layout)
{
	marginGroup->setFacingPages(layout != singlePage);
	m_choosenLayout = layout;
	firstPage->setCurrentIndex(prefsManager.appPrefs.pageSets[m_choosenLayout].FirstPage);
}

void NewDocDialog::recentDocListBox_doubleClicked()
{
	/* Yep. There is nothing to solve. ScribusMainWindow handles all
	openings etc. It's Franz's programming style ;) */
	ExitOK();
}

void NewDocDialog::adjustTitles(int tab)
{
	if (tab == 0)
		setWindowTitle(tr("New Document"));
	else if (tab == 1)
		setWindowTitle(tr("New from Template"));
	else if (tab == 2)
		setWindowTitle(tr("Open Existing Document"));
	else if (tab == 3)
		setWindowTitle(tr("Open Recent Document"));
	else
		setWindowTitle(tr("New Document"));
	okButton->setEnabled(tab!=2);
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
	if (s.count() <= 0)
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
	okButton->setEnabled(!path.isEmpty());
}
