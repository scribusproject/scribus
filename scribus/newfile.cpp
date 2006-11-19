/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "newfile.h"
#include "newfile.moc"

#include <qtooltip.h>
#include <qobjectlist.h>
#include <qpoint.h>
#include <qiconview.h>

#include "fileloader.h"
#include "prefsfile.h"
#include "units.h"
#include "pagesize.h"
#include "marginWidget.h"
#include "scconfig.h"
#include "scribuscore.h"
#include "prefsmanager.h"
#include "pagelayout.h"
#include "pagestructs.h"
#include "commonstrings.h"
#include "mspinbox.h"
#include "customfdialog.h"
#include "sccombobox.h"


extern QPixmap loadIcon(QString nam);


NewDoc::NewDoc( QWidget* parent, const QStringList& recentDocs, bool startUp ) : QDialog( parent, "newDoc", true, 0 )
{
	prefsManager=PrefsManager::instance();
	tabSelected = 0;
	onStartup = startUp;
	unitIndex = prefsManager->appPrefs.docUnitIndex;
	unitSuffix = unitGetSuffixFromIndex(unitIndex);
	unitRatio = unitGetRatioFromIndex(unitIndex);
	precision = unitGetPrecisionFromIndex(unitIndex);
	Orient = 0;
	setCaption( tr( "New Document" ) );
	setIcon(loadIcon("AppIcon.png"));
	TabbedNewDocLayout = new QVBoxLayout( this, 10, 5, "Form1Layout");
	if (startUp)
		tabWidget = new QTabWidget( this, "tabWidget2" );
	createNewDocPage();
	if (startUp)
	{
		tabWidget->addTab(newDocFrame, tr("&New Document"));
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
	Layout1->setSpacing( 6 );
	Layout1->setMargin( 0 );
	if (startUp)
	{
		startUpDialog = new QCheckBox( tr( "Do not show this dialog again" ), this, "startUpDialog" );
		startUpDialog->setChecked(!prefsManager->appPrefs.showStartupDialog);
		Layout1->addWidget( startUpDialog );
	}
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );
	OKButton = new QPushButton( CommonStrings::tr_OK, this, "OKButton" );
	OKButton->setDefault( true );
	Layout1->addWidget( OKButton );
	CancelB = new QPushButton( CommonStrings::tr_Cancel, this, "CancelB" );
	CancelB->setAutoDefault( false );
	Layout1->addWidget( CancelB );
	TabbedNewDocLayout->addLayout( Layout1 );
	//tooltips
	QToolTip::add( pageSizeComboBox, tr( "Document page size, either a standard size or a custom size" ) );
	QToolTip::add( pageOrientationComboBox, tr( "Orientation of the document's pages" ) );
	QToolTip::add( widthMSpinBox, tr( "Width of the document's pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( heightMSpinBox, tr( "Height of the document's pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( pageCountSpinBox, tr( "Initial number of pages of the document" ) );
	QToolTip::add( unitOfMeasureComboBox, tr( "Default unit of measurement for document editing" ) );
	QToolTip::add( autoTextFrame, tr( "Create text frames automatically when new pages are added" ) );
	QToolTip::add( numberOfCols, tr( "Number of columns to create in automatically created text frames" ) );
	QToolTip::add( Distance, tr( "Distance between automatically created columns" ) );

	// signals and slots connections
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( ExitOK() ) );
	connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPGsize(const QString &)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(unitOfMeasureComboBox, SIGNAL(activated(int)), this, SLOT(setUnit(int)));
	connect(Distance, SIGNAL(valueChanged(int)), this, SLOT(setDist(int)));
	connect(autoTextFrame, SIGNAL(clicked()), this, SLOT(handleAutoFrame()));
	connect(layoutsView, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
	if (startUp)
		connect(recentDocListBox, SIGNAL(selected(int)), this, SLOT(recentDocListBox_doubleClicked(int)));

	setMinimumSize(minimumSizeHint());
 	setMaximumSize(minimumSizeHint());
	resize(minimumSizeHint());
	clearWState( WState_Polished );
}

void NewDoc::createNewDocPage()
{
	newDocFrame = new QFrame(this, "newDocFrame");

	pageSizeGroupBox = new QGroupBox(newDocFrame, "pageSizeGroupBox" );
	pageSizeGroupBox->setTitle( tr( "Page Layout" ) );
	pageSizeGroupBox->setColumnLayout(0, Qt::Vertical );
	pageSizeGroupBox->layout()->setSpacing( 5 );
	pageSizeGroupBox->layout()->setMargin( 10 );
	pageSizeGroupBoxLayout = new QGridLayout( pageSizeGroupBox->layout() );
	pageSizeGroupBoxLayout->setAlignment( Qt::AlignTop );

	layoutsView = new QIconView( pageSizeGroupBox, "layoutsView" );
	layoutsView->setHScrollBarMode( QIconView::Auto );
	layoutsView->setVScrollBarMode( QIconView::Auto );
	layoutsView->setArrangement(QIconView::LeftToRight);
	layoutsView->setItemsMovable(false);
	layoutsView->setSorting( false );
	layoutsView->setFocusPolicy(QWidget::NoFocus);
	layoutsView->setSelectionMode(QIconView::Single);
	layoutsView->clear();
	for (uint pg = 0; pg < prefsManager->appPrefs.pageSets.count(); ++pg)
	{
		QIconViewItem *ic;
		QString psname=CommonStrings::translatePageSetString(prefsManager->appPrefs.pageSets[pg].Name);
		if (pg == 0)
		{
			ic = new QIconViewItem( layoutsView, psname, loadIcon("32/page-simple.png") );
			ic->setDragEnabled(false);
		}
		else if (pg == 1)
		{
			ic = new QIconViewItem( layoutsView, psname, loadIcon("32/page-doublesided.png") );
			ic->setDragEnabled(false);
		}
		else if (pg == 2)
		{
			ic = new QIconViewItem( layoutsView, psname, loadIcon("32/page-3fold.png") );
			ic->setDragEnabled(false);
		}
		else if (pg == 3)
		{
			ic = new QIconViewItem( layoutsView, psname, loadIcon("32/page-4fold.png") );
			ic->setDragEnabled(false);
		}
		else
		{
			ic = new QIconViewItem( layoutsView, psname, loadIcon("32/page-simple.png") );
			ic->setDragEnabled(false);
		}
	}
	pageSizeGroupBoxLayout->addMultiCellWidget( layoutsView, 0, 4, 0, 0 );
	
	
	TextLabel1 = new QLabel( tr( "&Size:" ), pageSizeGroupBox, "TextLabel1" );
	pageSizeGroupBoxLayout->addWidget( TextLabel1, 0, 1 );
	PageSize ps(prefsManager->appPrefs.pageSize);
	pageSizeComboBox = new QComboBox( true, pageSizeGroupBox, "pageSizeComboBox" );
	pageSizeComboBox->insertStringList(ps.sizeTRList());
	pageSizeComboBox->insertItem( CommonStrings::trCustomPageSize );
	pageSizeComboBox->setEditable(false);
	TextLabel1->setBuddy(pageSizeComboBox);
	pageSizeGroupBoxLayout->addWidget(pageSizeComboBox, 0, 2 );
	TextLabel2 = new QLabel( tr( "Orie&ntation:" ), pageSizeGroupBox, "TextLabel2" );
	pageSizeGroupBoxLayout->addWidget( TextLabel2, 1, 1 );
	pageOrientationComboBox = new QComboBox( true, pageSizeGroupBox, "pageOrientationComboBox" );
	pageOrientationComboBox->insertItem( tr( "Portrait" ) );
	pageOrientationComboBox->insertItem( tr( "Landscape" ) );
	pageOrientationComboBox->setEditable(false);
	pageOrientationComboBox->setCurrentItem(prefsManager->appPrefs.pageOrientation);
	TextLabel2->setBuddy(pageOrientationComboBox);
	pageSizeGroupBoxLayout->addWidget( pageOrientationComboBox, 1, 2 );

	TextLabel1_2 = new QLabel( tr( "&Width:" ), pageSizeGroupBox, "TextLabel1_2" );
	pageSizeGroupBoxLayout->addWidget(TextLabel1_2, 2, 1 );
	widthMSpinBox = new MSpinBox( 1, 10000, pageSizeGroupBox, precision );
	widthMSpinBox->setSuffix(unitSuffix);
	TextLabel1_2->setBuddy(widthMSpinBox);
	pageSizeGroupBoxLayout->addWidget(widthMSpinBox, 2, 2 );
	TextLabel2_2 = new QLabel( tr( "&Height:" ), pageSizeGroupBox, "TextLabel2_2" );
	pageSizeGroupBoxLayout->addWidget(TextLabel2_2, 3, 1 );
	heightMSpinBox = new MSpinBox( 1, 10000, pageSizeGroupBox, precision );
	heightMSpinBox->setSuffix(unitSuffix);
	TextLabel2_2->setBuddy(heightMSpinBox);
	pageSizeGroupBoxLayout->addWidget(heightMSpinBox, 3, 2 );
	layoutLabel1 = new QLabel( pageSizeGroupBox, "layoutLabel1" );
	layoutLabel1->setText( tr( "First Page is:" ) );
	pageSizeGroupBoxLayout->addWidget( layoutLabel1, 4, 1 );
	firstPage = new ScComboBox( false, pageSizeGroupBox, "firstPage" );
	firstPage->clear();
	pageSizeGroupBoxLayout->addWidget( firstPage, 4, 2 );
	selectItem(prefsManager->appPrefs.FacingPages);
	firstPage->setCurrentItem(prefsManager->appPrefs.pageSets[prefsManager->appPrefs.FacingPages].FirstPage);

	struct MarginStruct marg;
	marg.Top = prefsManager->appPrefs.RandOben;
	marg.Bottom = prefsManager->appPrefs.RandUnten;
	marg.Left = prefsManager->appPrefs.RandLinks;
	marg.Right = prefsManager->appPrefs.RandRechts;
	marginGroup = new MarginWidget(newDocFrame,  tr( "Margin Guides" ), &marg, unitIndex );
	marginGroup->setPageWidthHeight(prefsManager->appPrefs.PageWidth, prefsManager->appPrefs.PageHeight);
	marginGroup->setFacingPages(!(prefsManager->appPrefs.FacingPages == singlePage));
	widthMSpinBox->setValue(prefsManager->appPrefs.PageWidth * unitRatio);
	heightMSpinBox->setValue(prefsManager->appPrefs.PageHeight * unitRatio);
	QStringList pageSizes=ps.sizeList();
	int sizeIndex=pageSizes.findIndex(ps.nameTR());
	if (sizeIndex!=-1)
		pageSizeComboBox->setCurrentItem(sizeIndex);
	else
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
	marginGroup->setPageSize(pageSizeComboBox->currentText());
	setDS(prefsManager->appPrefs.FacingPages);
	setSize(prefsManager->appPrefs.pageSize);
	setOrien(prefsManager->appPrefs.pageOrientation);
	widthMSpinBox->setValue(prefsManager->appPrefs.PageWidth * unitRatio);
	heightMSpinBox->setValue(prefsManager->appPrefs.PageHeight * unitRatio);
	marginGroup->setNewBleeds(prefsManager->appPrefs.BleedTop, prefsManager->appPrefs.BleedBottom, prefsManager->appPrefs.BleedLeft, prefsManager->appPrefs.BleedRight);

	optionsGroupBox = new QGroupBox( newDocFrame, "optionsGroupBox" );
	optionsGroupBox->setTitle( tr( "Options" ) );
	optionsGroupBox->setColumnLayout(0, Qt::Vertical );
	optionsGroupBox->layout()->setSpacing( 5 );
	optionsGroupBox->layout()->setMargin( 10 );
	optionsGroupBoxLayout = new QGridLayout( optionsGroupBox->layout() );
	optionsGroupBoxLayout->setAlignment( Qt::AlignTop );
	pageCountLabel = new QLabel( tr( "N&umber of Pages:" ), optionsGroupBox, "pageCountLabel" );
	
	pageCountSpinBox = new QSpinBox( optionsGroupBox, "pageCountSpinBox" );
	pageCountSpinBox->setMaxValue( 10000 );
	pageCountSpinBox->setMinValue( 1 );
	pageCountLabel->setBuddy(pageCountSpinBox);
	unitOfMeasureLabel = new QLabel( tr( "&Default Unit:" ), optionsGroupBox, "unitOfMeasureLabel" );
	unitOfMeasureComboBox = new QComboBox( true, optionsGroupBox, "unitOfMeasureComboBox" );
	unitOfMeasureComboBox->insertStringList(unitGetTextUnitList());
	unitOfMeasureComboBox->setCurrentItem(unitIndex);
	unitOfMeasureComboBox->setEditable(false);
	unitOfMeasureLabel->setBuddy(unitOfMeasureComboBox);
	optionsGroupBoxLayout->addWidget( pageCountLabel, 0, 0 );
	optionsGroupBoxLayout->addWidget( pageCountSpinBox, 0, 1 );
	optionsGroupBoxLayout->addWidget( unitOfMeasureLabel, 1, 0 );
	optionsGroupBoxLayout->addWidget( unitOfMeasureComboBox, 1, 1 );

	autoTextFrame = new QCheckBox( optionsGroupBox, "autoTextFrame" );
	autoTextFrame->setText( tr( "&Automatic Text Frames" ) );
	optionsGroupBoxLayout->addMultiCellWidget( autoTextFrame, 2, 2, 0, 1 );
	TextLabel3 = new QLabel( tr( "Colu&mns:" ), optionsGroupBox, "TextLabel3" );
	optionsGroupBoxLayout->addWidget( TextLabel3, 3, 0 );
	Distance = new MSpinBox( 0, 1000, optionsGroupBox, precision );
	Distance->setSuffix( unitSuffix );
	Distance->setValue(11 * unitRatio);
	Dist = 11;
	optionsGroupBoxLayout->addWidget( Distance, 3, 1);
	TextLabel4 = new QLabel( tr( "&Gap:" ), optionsGroupBox, "TextLabel4" );
	optionsGroupBoxLayout->addWidget( TextLabel4, 4, 0 );
	TextLabel4->setBuddy(Distance);
	numberOfCols = new QSpinBox( optionsGroupBox, "numberOfCols" );
	numberOfCols->setButtonSymbols( QSpinBox::UpDownArrows );
	numberOfCols->setMinValue( 1 );
	numberOfCols->setValue( 1 );
	TextLabel3->setBuddy(numberOfCols);
	optionsGroupBoxLayout->addWidget( numberOfCols, 4, 1);
	TextLabel3->setEnabled(false);
	TextLabel4->setEnabled(false);
	Distance->setEnabled(false);
	numberOfCols->setEnabled(false);
	startDocSetup = new QCheckBox( optionsGroupBox, "startDocSetup" );
	startDocSetup->setText( tr( "Open Document Settings After Creation" ) );
	startDocSetup->setChecked(false);
	optionsGroupBoxLayout->addMultiCellWidget( startDocSetup, 5, 5, 0, 1 );

	NewDocLayout = new QGridLayout( newDocFrame, 2, 2, 10, 5, "NewDocLayout");
	NewDocLayout->addWidget( marginGroup, 1, 0 );
	NewDocLayout->addWidget( optionsGroupBox, 1, 1 );
	NewDocLayout->addMultiCellWidget( pageSizeGroupBox, 0, 0, 0, 1);
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
	openDocFrame = new QFrame(this, "openDocFrame");
	QVBoxLayout* openDocLayout = new QVBoxLayout(openDocFrame, 5,5, "openDocLayout");
	fileDialog = new CustomFDialog(openDocFrame, docDir, tr("Open"), formats, fdNone);
	fileDialog->setSizeGripEnabled(false);
	fileDialog->setModal(false);
	QObjectList *l = fileDialog->queryList("QPushButton");
	QObjectListIt it(*l);
	QObject *obj;
	while ((obj = it.current()) != 0)
	{
		++it;
		((QPushButton*)obj)->hide();
	}
	delete l;
	QPoint point = QPoint(0,0);
	fileDialog->reparent(openDocFrame, point);
	openDocLayout->addWidget(fileDialog);
	connect(fileDialog, SIGNAL(fileSelected ( const QString & )), this, SLOT(openFile(const QString& )));
}

void NewDoc::openFile(const QString &)
{
	ExitOK();
}

void NewDoc::createRecentDocPage()
{
	recentDocFrame = new QFrame(this, "recentDocFrame");
	recentDocLayout = new QVBoxLayout(recentDocFrame, 5, 5, "recentDocLayout");
	recentDocListBox = new QListBox(recentDocFrame, "recentDocListBox");
	recentDocLayout->addWidget(recentDocListBox);
	uint max = QMIN(prefsManager->appPrefs.RecentDCount, recentDocList.count());
	for (uint m = 0; m < max; ++m)
		recentDocListBox->insertItem(recentDocList[m]);
}

void NewDoc::setWidth(int)
{
	pageWidth = widthMSpinBox->value() / unitRatio;
	marginGroup->setPageWidth(pageWidth);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
}

void NewDoc::setHeight(int)
{
	pageHeight = heightMSpinBox->value() / unitRatio;
	marginGroup->setPageHeight(pageHeight);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
}

void NewDoc::selectItem(uint nr)
{
	QIconViewItem* ic=0;
	uint cce;
	disconnect(layoutsView, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
	ic = layoutsView->firstItem();
	cce = layoutsView->count();
	for (uint cc = 0; cc < cce; ++cc)
	{
		if (cc == nr)
		{
			if (cc > 0)
			{
				firstPage->setEnabled(true);
				firstPage->clear();
				QStringList::Iterator pNames;
				for(pNames = prefsManager->appPrefs.pageSets[cc].pageNames.begin(); pNames != prefsManager->appPrefs.pageSets[cc].pageNames.end(); ++pNames )
				{
					firstPage->insertItem(CommonStrings::translatePageSetLocString((*pNames)));
				}
			}
			else
			{
				firstPage->clear();
				firstPage->insertItem(" ");
				firstPage->setEnabled(false);
			}
			layoutsView->setSelected(ic, true);
			break;
		}
		ic = ic->nextItem();
	}
	connect(layoutsView, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
}

void NewDoc::itemSelected(QIconViewItem* ic)
{
	if (ic == 0)
		return;
	selectItem(layoutsView->index(ic));
	setDS(layoutsView->index(ic));
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

void NewDoc::setDist(int)
{
	Dist = Distance->value() / unitRatio;
}

void NewDoc::setUnit(int newUnitIndex)
{
	disconnect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
	disconnect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));

	double oldUnitRatio = unitRatio;
	double val, oldB, oldBM, oldH, oldHM;
	int decimals;
	widthMSpinBox->getValues(&oldB, &oldBM, &decimals, &val);
	oldB /= oldUnitRatio;
	oldBM /= oldUnitRatio;
	heightMSpinBox->getValues(&oldH, &oldHM, &decimals, &val);
	oldH /= oldUnitRatio;
	oldHM /= oldUnitRatio;

	unitIndex = newUnitIndex;
	unitRatio = unitGetRatioFromIndex(newUnitIndex);
	decimals = unitGetDecimalsFromIndex(newUnitIndex);
	if (pageOrientationComboBox->currentItem() == portraitPage)
	{
		widthMSpinBox->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, pageWidth * unitRatio);
		heightMSpinBox->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, pageHeight * unitRatio);
	}
	else
	{
		widthMSpinBox->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, pageHeight * unitRatio);
		heightMSpinBox->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, pageWidth * unitRatio);
	}
	Distance->setValue(Dist * unitRatio);
	unitSuffix = unitGetSuffixFromIndex(newUnitIndex);
	widthMSpinBox->setSuffix(unitSuffix);
	heightMSpinBox->setSuffix(unitSuffix);
	Distance->setSuffix( unitSuffix );
	marginGroup->unitChange(unitRatio, decimals, unitSuffix);
	marginGroup->setPageHeight(pageHeight);
	marginGroup->setPageWidth(pageWidth);
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));

}

void NewDoc::ExitOK()
{
	pageWidth = widthMSpinBox->value() / unitRatio;
	pageHeight = heightMSpinBox->value() / unitRatio;
	bleedBottom = marginGroup->bottomBleed();
	bleedTop = marginGroup->topBleed();
	bleedLeft = marginGroup->leftBleed();
	bleedRight = marginGroup->rightBleed();
	if (onStartup)
		tabSelected = tabWidget->currentPageIndex();
	else
		tabSelected = 0;
	accept();
}

void NewDoc::setOrien(int ori)
{
	double br;
	disconnect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
	disconnect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));
	if (ori != Orient)
	{
		br = widthMSpinBox->value();
		widthMSpinBox->setValue(heightMSpinBox->value());
		heightMSpinBox->setValue(br);
	}
	// #869 pv - defined constants added + code repeat (check w/h)
	(ori == portraitPage) ? Orient = portraitPage : Orient = landscapePage;
	if (pageSizeComboBox->currentText() == CommonStrings::trCustomPageSize)
	{
		if (widthMSpinBox->value() > heightMSpinBox->value())
			pageOrientationComboBox->setCurrentItem(landscapePage);
		else
			pageOrientationComboBox->setCurrentItem(portraitPage);
	}
	// end of #869
	marginGroup->setPageHeight(pageHeight);
	marginGroup->setPageWidth(pageWidth);
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));
}

void NewDoc::setPGsize(const QString &size)
{
	if (size == CommonStrings::trCustomPageSize)
		setSize(size);
	else
	{
		setSize(size);
		setOrien(pageOrientationComboBox->currentItem());
	}
	marginGroup->setPageSize(size);
}

void NewDoc::setSize(QString gr)
{
	pageWidth = widthMSpinBox->value() / unitRatio;
	pageHeight = heightMSpinBox->value() / unitRatio;

	disconnect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
	disconnect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));
	if (gr==CommonStrings::trCustomPageSize || gr==CommonStrings::customPageSize)
	{
		//widthMSpinBox->setEnabled(true);
		//heightMSpinBox->setEnabled(true);
	}
	else
	{
		PageSize *ps2=new PageSize(gr);
		if (pageOrientationComboBox->currentItem() == portraitPage)
		{
			pageWidth = ps2->width();
			pageHeight = ps2->height();
		} else {
			pageWidth = ps2->height();
			pageHeight = ps2->width();
		}
		delete ps2;
	}
	widthMSpinBox->setValue(pageWidth * unitRatio);
	heightMSpinBox->setValue(pageHeight * unitRatio);
	marginGroup->setPageHeight(pageHeight);
	marginGroup->setPageWidth(pageWidth);
	connect(widthMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
	connect(heightMSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));
}

void NewDoc::setDS(int layout)
{
	marginGroup->setFacingPages(!(layout == singlePage));
	choosenLayout = layout;
	firstPage->setCurrentItem(prefsManager->appPrefs.pageSets[choosenLayout].FirstPage);
}

void NewDoc::recentDocListBox_doubleClicked(int /*item*/)
{
	/* Yep. There is nothing to solve. ScribusMainWindow handles all
	openings etc. It's Franz's programming style ;) */
	ExitOK();
}
