/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "newfile.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QListWidgetItem>
#include <QFrame>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QTabWidget>
#include <QPushButton>
#include <QPixmap>
#include <QToolTip>
#include <QDir>
#include <QPoint>
#if QT_VERSION  >= 0x040300
	#include <QFileDialog>
#else
	#include "customfdialog.h"
#endif

#include "fileloader.h"
#include "prefsfile.h"
#include "units.h"
#include "pagesize.h"
#include "marginwidget.h"
#include "scconfig.h"
#include "scribuscore.h"
#include "prefsmanager.h"
#include "pagelayout.h"
#include "pagestructs.h"
#include "commonstrings.h"
#include "scrspinbox.h"
#include "sccombobox.h"
#include "util_icon.h"

PageLayoutsWidget::PageLayoutsWidget(QWidget* parent) : QListWidget(parent)
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
	QListWidgetItem* ic;
	int startY = 5;
	int startX = 5;
	setResizeMode(QListView::Fixed);
	int maxSizeY = 0;
	for (int cc = 0; cc < count(); ++cc)
	{
		ic = item(cc);
		QRect ir = visualItemRect(ic);
		setPositionForIndex(QPoint(startX, startY), indexFromItem(ic));
		startX += ir.width()+5;
		maxSizeY = qMax(maxSizeY, ir.height());
	}
	maxX = startX;
	maxY = maxSizeY+10;
}

const QSize PageLayoutsWidget::minimumSizeHint()
{
	return QSize(maxX, maxY);
}

NewDoc::NewDoc( QWidget* parent, const QStringList& recentDocs, bool startUp, QString lang, QString templateDir ) : QDialog( parent )
{
	setObjectName(QString::fromLocal8Bit("NewDocumentWindow"));
	setModal(true);
	prefsManager=PrefsManager::instance();
	m_tabSelected = 0;
	m_onStartup = startUp;
	m_unitIndex = prefsManager->appPrefs.docUnitIndex;
	m_unitRatio = unitGetRatioFromIndex(m_unitIndex);
	m_unitSuffix = unitGetSuffixFromIndex(m_unitIndex);
	m_orientation = prefsManager->appPrefs.pageOrientation;
	setWindowTitle( tr( "New Document" ) );
	setWindowIcon(QIcon(loadIcon("AppIcon.png")));
	TabbedNewDocLayout = new QVBoxLayout( this );
	TabbedNewDocLayout->setMargin(10);
	TabbedNewDocLayout->setSpacing(5);
	if (startUp)
		tabWidget = new QTabWidget( this );
	createNewDocPage();
	if (startUp)
	{
		tabWidget->addTab(newDocFrame, tr("&New Document"));
		createNewFromTempPage();
		nftGui->setupSettings(lang, templateDir);
		tabWidget->addTab(newFromTempFrame, tr("New &from Template"));
		createOpenDocPage();
		tabWidget->addTab(openDocFrame, tr("Open &Existing Document"));
		recentDocList=recentDocs;
 		createRecentDocPage();
 		tabWidget->addTab(recentDocFrame, tr("Open Recent &Document"));
 		TabbedNewDocLayout->addWidget(tabWidget);
	}
	else
		TabbedNewDocLayout->addWidget(newDocFrame);

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 0 );
	if (startUp)
	{
		startUpDialog = new QCheckBox( tr( "Do not show this dialog again" ), this );
		startUpDialog->setChecked(!prefsManager->appPrefs.showStartupDialog);
		Layout1->addWidget( startUpDialog );
	}
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );
	OKButton = new QPushButton( CommonStrings::tr_OK, this );
	OKButton->setDefault( true );
	Layout1->addWidget( OKButton );
	CancelB = new QPushButton( CommonStrings::tr_Cancel, this );
	CancelB->setAutoDefault( false );
	Layout1->addWidget( CancelB );
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
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( ExitOK() ) );
	connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize(const QString &)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrientation(int)));
	connect(unitOfMeasureComboBox, SIGNAL(activated(int)), this, SLOT(setUnit(int)));
	connect(Distance, SIGNAL(valueChanged(double)), this, SLOT(setDistance(double)));
	connect(autoTextFrame, SIGNAL(clicked()), this, SLOT(handleAutoFrame()));
	connect(layoutsView, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	if (startUp)
	{
		connect(nftGui, SIGNAL(leaveOK()), this, SLOT(ExitOK()));
		connect(recentDocListBox, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(recentDocListBox_doubleClicked()));
	}

// 	setMinimumSize(minimumSizeHint());
//  	setMaximumSize(minimumSizeHint());
// 	resize(minimumSizeHint());
}

void NewDoc::createNewDocPage()
{
	newDocFrame = new QFrame(this);

	pageSizeGroupBox = new QGroupBox(newDocFrame );
	pageSizeGroupBox->setTitle( tr( "Document Layout" ) );
	pageSizeGroupBoxLayout = new QGridLayout( pageSizeGroupBox );
	pageSizeGroupBoxLayout->setMargin(10);
	pageSizeGroupBoxLayout->setSpacing(5);
	pageSizeGroupBoxLayout->setAlignment( Qt::AlignTop );

	layoutsView = new PageLayoutsWidget( pageSizeGroupBox );
	layoutsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	for (int pg = 0; pg < prefsManager->appPrefs.pageSets.count(); ++pg)
	{
		QListWidgetItem *ic;
		QString psname=CommonStrings::translatePageSetString(prefsManager->appPrefs.pageSets[pg].Name);
		if (pg == 0)
		{
			ic = new QListWidgetItem( QIcon(loadIcon("32/page-simple.png")), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
		else if (pg == 1)
		{
			ic = new QListWidgetItem( QIcon(loadIcon("32/page-doublesided.png")), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
		else if (pg == 2)
		{
			ic = new QListWidgetItem( QIcon(loadIcon("32/page-3fold.png")), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
		else if (pg == 3)
		{
			ic = new QListWidgetItem( QIcon(loadIcon("32/page-4fold.png")), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
		else
		{
			ic = new QListWidgetItem( QIcon(loadIcon("32/page-simple.png")), psname, layoutsView );
			ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
	}
	layoutsView->arrangeIcons();
	pageSizeGroupBoxLayout->addWidget( layoutsView, 0, 0, 5, 1 );
	layoutsView->arrangeIcons();


	TextLabel1 = new QLabel( tr( "&Size:" ), pageSizeGroupBox );
	pageSizeGroupBoxLayout->addWidget( TextLabel1, 0, 1 );
	PageSize ps(prefsManager->appPrefs.pageSize);
	pageSizeComboBox = new QComboBox( pageSizeGroupBox );
	pageSizeComboBox->addItems(ps.sizeTRList());
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
	pageOrientationComboBox->setCurrentIndex(prefsManager->appPrefs.pageOrientation);
	TextLabel2->setBuddy(pageOrientationComboBox);
	pageSizeGroupBoxLayout->addWidget( pageOrientationComboBox, 1, 2 );

	TextLabel1_2 = new QLabel( tr( "&Width:" ), pageSizeGroupBox );
	pageSizeGroupBoxLayout->addWidget(TextLabel1_2, 2, 1 );
	widthSpinBox = new ScrSpinBox( 1, 16777215, pageSizeGroupBox, m_unitIndex );
	widthSpinBox->setSuffix(m_unitSuffix);
	TextLabel1_2->setBuddy(widthSpinBox);
	pageSizeGroupBoxLayout->addWidget(widthSpinBox, 2, 2 );
	TextLabel2_2 = new QLabel( tr( "&Height:" ), pageSizeGroupBox );
	pageSizeGroupBoxLayout->addWidget(TextLabel2_2, 3, 1 );
	heightSpinBox = new ScrSpinBox( 1, 16777215, pageSizeGroupBox, m_unitIndex );
	heightSpinBox->setSuffix(m_unitSuffix);
	TextLabel2_2->setBuddy(heightSpinBox);
	pageSizeGroupBoxLayout->addWidget(heightSpinBox, 3, 2 );
	layoutLabel1 = new QLabel( pageSizeGroupBox );
	layoutLabel1->setText( tr( "First Page is:" ) );
	pageSizeGroupBoxLayout->addWidget( layoutLabel1, 4, 1 );
	firstPage = new ScComboBox( pageSizeGroupBox );
	firstPage->clear();
	pageSizeGroupBoxLayout->addWidget( firstPage, 4, 2 );
	selectItem(prefsManager->appPrefs.FacingPages);
	firstPage->setCurrentIndex(prefsManager->appPrefs.pageSets[prefsManager->appPrefs.FacingPages].FirstPage);

	MarginStruct marg(prefsManager->appPrefs.margins);
	marginGroup = new MarginWidget(newDocFrame,  tr( "Margin Guides" ), &marg, m_unitIndex );
	marginGroup->setPageWidthHeight(prefsManager->appPrefs.PageWidth, prefsManager->appPrefs.PageHeight);
	marginGroup->setFacingPages(!(prefsManager->appPrefs.FacingPages == singlePage));
	widthSpinBox->setValue(prefsManager->appPrefs.PageWidth * m_unitRatio);
	heightSpinBox->setValue(prefsManager->appPrefs.PageHeight * m_unitRatio);
	QStringList pageSizes=ps.sizeList();
	int sizeIndex=pageSizes.indexOf(ps.nameTR());
	if (sizeIndex!=-1)
		pageSizeComboBox->setCurrentIndex(sizeIndex);
	else
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
	marginGroup->setPageSize(pageSizeComboBox->currentText());
	setDocLayout(prefsManager->appPrefs.FacingPages);
	setSize(prefsManager->appPrefs.pageSize);
	setOrientation(prefsManager->appPrefs.pageOrientation);
	marginGroup->setNewBleeds(prefsManager->appPrefs.bleeds);
	marginGroup->setMarginPreset(prefsManager->appPrefs.marginPreset);

	optionsGroupBox = new QGroupBox( newDocFrame );
	optionsGroupBox->setTitle( tr( "Options" ) );
	optionsGroupBoxLayout = new QFormLayout( optionsGroupBox );
	optionsGroupBoxLayout->setSpacing( 5 );
	optionsGroupBoxLayout->setMargin( 10 );
	optionsGroupBoxLayout->setAlignment( Qt::AlignTop );
	optionsGroupBoxLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
	optionsGroupBoxLayout->setLabelAlignment(Qt::AlignLeft);
	pageCountLabel = new QLabel( tr( "N&umber of Pages:" ), optionsGroupBox );

	pageCountSpinBox = new QSpinBox( optionsGroupBox );
	pageCountSpinBox->setMaximum( 10000 );
	pageCountSpinBox->setMinimum( 1 );
	pageCountLabel->setBuddy(pageCountSpinBox);
	unitOfMeasureLabel = new QLabel( tr( "&Default Unit:" ), optionsGroupBox );
	unitOfMeasureComboBox = new QComboBox( optionsGroupBox );
	unitOfMeasureComboBox->addItems(unitGetTextUnitList());
	unitOfMeasureComboBox->setCurrentIndex(m_unitIndex);
	unitOfMeasureComboBox->setEditable(false);
	unitOfMeasureLabel->setBuddy(unitOfMeasureComboBox);
	optionsGroupBoxLayout->addRow( pageCountLabel, pageCountSpinBox);
	optionsGroupBoxLayout->addRow( unitOfMeasureLabel, unitOfMeasureComboBox );

	autoTextFrame = new QCheckBox( optionsGroupBox );
	autoTextFrame->setText( tr( "&Automatic Text Frames" ) );
	optionsGroupBoxLayout->addRow( autoTextFrame );
	TextLabel3 = new QLabel( tr( "Colu&mns:" ), optionsGroupBox );
	numberOfCols = new QSpinBox( optionsGroupBox );
	numberOfCols->setButtonSymbols( QSpinBox::UpDownArrows );
	numberOfCols->setMinimum( 1 );
	numberOfCols->setValue( 1 );
	TextLabel3->setBuddy(numberOfCols);
	optionsGroupBoxLayout->addRow( TextLabel3, numberOfCols );

	TextLabel4 = new QLabel( tr( "&Gap:" ), optionsGroupBox );
	Distance = new ScrSpinBox( 0, 1000, optionsGroupBox, m_unitIndex );
	Distance->setValue(11 * m_unitRatio);
	m_distance = 11;
	optionsGroupBoxLayout->addRow( TextLabel4, Distance );
	TextLabel4->setBuddy(Distance);

	TextLabel3->setEnabled(false);
	TextLabel4->setEnabled(false);
	Distance->setEnabled(false);
	numberOfCols->setEnabled(false);
	startDocSetup = new QCheckBox( optionsGroupBox );
	startDocSetup->setText( tr( "Show Document Settings After Creation" ) );
	startDocSetup->setChecked(false);
	optionsGroupBoxLayout->addRow( startDocSetup );
	NewDocLayout = new QGridLayout( newDocFrame );
	NewDocLayout->setMargin(10);
	NewDocLayout->setSpacing(5);
	NewDocLayout->addWidget( marginGroup, 1, 0 );
	NewDocLayout->addWidget( optionsGroupBox, 1, 1 );
	NewDocLayout->addWidget( pageSizeGroupBox, 0, 0, 1, 2);
}

void NewDoc::createNewFromTempPage()
{
	newFromTempFrame = new QFrame(this);
	verticalLayout = new QVBoxLayout(newFromTempFrame);
	nftGui = new nftwidget(newFromTempFrame);
	verticalLayout->addWidget(nftGui);
}

void NewDoc::createOpenDocPage()
{
	PrefsContext* docContext = prefsManager->prefsFile->getContext("docdirs", false);
	QString docDir = ".";
	QString prefsDocDir=prefsManager->documentDir();
	if (!prefsDocDir.isEmpty())
		docDir = docContext->get("docsopen", prefsDocDir);
	else
		docDir = docContext->get("docsopen", ".");
	QString formats(FileLoader::getLoadFilterString());
	formats.remove("PDF (*.pdf *.PDF);;");
	openDocFrame = new QFrame(this);
	openDocLayout = new QVBoxLayout(openDocFrame);
	openDocLayout->setMargin(5);
	openDocLayout->setSpacing(5);
	m_selectedFile = "";
#if QT_VERSION  >= 0x040300
	fileDialog = new QFileDialog(openDocFrame, tr("Open"), docDir, formats);
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog->setReadOnly(true);
#else
	fileDialog = new CustomFDialog(openDocFrame, docDir, tr("Open"), formats, fdNone);
#endif
	fileDialog->setSizeGripEnabled(false);
	fileDialog->setModal(false);
	QList<QPushButton *> b = fileDialog->findChildren<QPushButton *>();
	QListIterator<QPushButton *> i(b);
	while (i.hasNext())
		i.next()->setVisible(false);
	fileDialog->setWindowFlags(Qt::Widget);
	openDocLayout->addWidget(fileDialog);
#if QT_VERSION  >= 0x040300
	connect(fileDialog, SIGNAL(filesSelected(const QStringList &)), this, SLOT(openFile()));
#else
	connect(fileDialog, SIGNAL(fileSelected (const QString &)), this, SLOT(openFile()));
#endif
}

void NewDoc::openFile()
{
	ExitOK();
}

void NewDoc::createRecentDocPage()
{
	recentDocFrame = new QFrame(this);
	recentDocLayout = new QVBoxLayout(recentDocFrame);
	recentDocLayout->setMargin(5);
	recentDocLayout->setSpacing(5);
	recentDocListBox = new QListWidget(recentDocFrame);
	recentDocLayout->addWidget(recentDocListBox);
	uint max = qMin(prefsManager->appPrefs.RecentDCount, recentDocList.count());
	for (uint m = 0; m < max; ++m)
		recentDocListBox->addItem( QDir::convertSeparators(recentDocList[m]) );
}

void NewDoc::setWidth(double)
{
	m_pageWidth = widthSpinBox->value() / m_unitRatio;
	marginGroup->setPageWidth(m_pageWidth);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
	int newOrientation = (widthSpinBox->value() > heightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != m_orientation)
	{
		pageOrientationComboBox->blockSignals(true);
		pageOrientationComboBox->setCurrentIndex(newOrientation);
		pageOrientationComboBox->blockSignals(false);
		m_orientation = newOrientation;
	}
}

void NewDoc::setHeight(double)
{
	m_pageHeight = heightSpinBox->value() / m_unitRatio;
	marginGroup->setPageHeight(m_pageHeight);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
	int newOrientation = (widthSpinBox->value() > heightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != m_orientation)
	{
		pageOrientationComboBox->blockSignals(true);
		pageOrientationComboBox->setCurrentIndex(newOrientation);
		pageOrientationComboBox->blockSignals(false);
		m_orientation = newOrientation;
	}
}

void NewDoc::selectItem(uint nr)
{
	disconnect(layoutsView, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	disconnect(layoutsView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	disconnect(layoutsView, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	disconnect(layoutsView, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	if (nr > 0)
	{
		firstPage->setEnabled(true);
		firstPage->clear();
		QStringList::Iterator pNames;
		for(pNames = prefsManager->appPrefs.pageSets[nr].pageNames.begin(); pNames != prefsManager->appPrefs.pageSets[nr].pageNames.end(); ++pNames )
		{
			firstPage->addItem(CommonStrings::translatePageSetLocString((*pNames)));
		}
	}
	else
	{
		firstPage->clear();
		firstPage->addItem(" ");
		firstPage->setEnabled(false);
	}
	layoutsView->setCurrentRow(nr);
	layoutsView->item(nr)->setSelected(true);
	connect(layoutsView, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	connect(layoutsView, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
}

void NewDoc::itemSelected(QListWidgetItem* ic)
{
	if (ic == 0)
		return;
	selectItem(layoutsView->row(ic));
	setDocLayout(layoutsView->row(ic));
}

void NewDoc::handleAutoFrame()
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

void NewDoc::setDistance(double)
{
	m_distance = Distance->value() / m_unitRatio;
}

void NewDoc::setUnit(int newUnitIndex)
{
	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
	widthSpinBox->setNewUnit(newUnitIndex);
	heightSpinBox->setNewUnit(newUnitIndex);
	Distance->setNewUnit(newUnitIndex);
	m_unitRatio = unitGetRatioFromIndex(newUnitIndex);
	m_unitIndex = newUnitIndex;
/*	
double oldUnitRatio = unitRatio;
	double val, oldB, oldBM, oldH, oldHM;
	int decimals;
	widthSpinBox->getValues(&oldB, &oldBM, &decimals, &val);
	oldB /= oldUnitRatio;
	oldBM /= oldUnitRatio;
	heightSpinBox->getValues(&oldH, &oldHM, &decimals, &val);
	oldH /= oldUnitRatio;
	oldHM /= oldUnitRatio;

	unitIndex = newUnitIndex;
	unitRatio = unitGetRatioFromIndex(newUnitIndex);
	decimals = unitGetDecimalsFromIndex(newUnitIndex);
	if (pageOrientationComboBox->currentItem() == portraitPage)
	{
		widthSpinBox->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, pageWidth * unitRatio);
		heightSpinBox->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, pageHeight * unitRatio);
	}
	else
	{
		widthSpinBox->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, pageHeight * unitRatio);
		heightSpinBox->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, pageWidth * unitRatio);
	}
	Distance->setValue(Dist * unitRatio);
	unitSuffix = unitGetSuffixFromIndex(newUnitIndex);
	widthSpinBox->setSuffix(unitSuffix);
	heightSpinBox->setSuffix(unitSuffix);
	Distance->setSuffix( unitSuffix );
*/
	marginGroup->setNewUnit(m_unitIndex);
	marginGroup->setPageHeight(m_pageHeight);
	marginGroup->setPageWidth(m_pageWidth);
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));

}

void NewDoc::ExitOK()
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
		if (m_tabSelected == NewDoc::NewFromTemplateTab) // new doc from template
		{
			if (nftGui->currentDocumentTemplate)
			{
				m_selectedFile = QDir::fromNativeSeparators(nftGui->currentDocumentTemplate->file);
				m_selectedFile = QDir::cleanPath(m_selectedFile);
			}
		}
		else if (m_tabSelected == NewDoc::OpenExistingTab) // open existing doc
		{
#if QT_VERSION  >= 0x040300
			QStringList files = fileDialog->selectedFiles();
			if (files.count() != 0)
				m_selectedFile = QDir::fromNativeSeparators(files[0]);
#else
			m_selectedFile = QDir::fromNativeSeparators(fileDialog->selectedFile());
#endif
		}
		else if (m_tabSelected == NewDoc::OpenRecentTab) // open recent doc
		{
			if (recentDocListBox->currentItem() != NULL)
			{
				QString fileName(recentDocListBox->currentItem()->text());
				if (!fileName.isEmpty())
					m_selectedFile = QDir::fromNativeSeparators(fileName);
			}
		}
	}
	else
		m_tabSelected = NewDoc::NewDocumentTab;
	accept();
}

void NewDoc::setOrientation(int ori)
{
	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
	if (ori != m_orientation)
	{
		double w = widthSpinBox->value(), h = heightSpinBox->value();
		widthSpinBox->setValue((ori == portraitPage) ? qMin(w, h) : qMax(w, h));
		heightSpinBox->setValue((ori == portraitPage) ? qMax(w, h) : qMin(w, h));
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

void NewDoc::setPageSize(const QString &size)
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

void NewDoc::setSize(QString gr)
{
	m_pageWidth = widthSpinBox->value() / m_unitRatio;
	m_pageHeight = heightSpinBox->value() / m_unitRatio;

	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
	if (gr==CommonStrings::trCustomPageSize || gr==CommonStrings::customPageSize)
	{
		widthSpinBox->setEnabled(true);
		heightSpinBox->setEnabled(true);
	}
	else
	{
		PageSize *ps2=new PageSize(gr);
		if (pageOrientationComboBox->currentIndex() == portraitPage)
		{
			m_pageWidth = ps2->width();
			m_pageHeight = ps2->height();
		} else {
			m_pageWidth = ps2->height();
			m_pageHeight = ps2->width();
		}
		delete ps2;
	}
	widthSpinBox->setValue(m_pageWidth * m_unitRatio);
	heightSpinBox->setValue(m_pageHeight * m_unitRatio);
	marginGroup->setPageHeight(m_pageHeight);
	marginGroup->setPageWidth(m_pageWidth);
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
}

void NewDoc::setDocLayout(int layout)
{
	marginGroup->setFacingPages(!(layout == singlePage));
	m_choosenLayout = layout;
	firstPage->setCurrentIndex(prefsManager->appPrefs.pageSets[m_choosenLayout].FirstPage);
}

void NewDoc::recentDocListBox_doubleClicked()
{
	/* Yep. There is nothing to solve. ScribusMainWindow handles all
	openings etc. It's Franz's programming style ;) */
	ExitOK();
}
