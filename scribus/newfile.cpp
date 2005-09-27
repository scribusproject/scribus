#include "newfile.h"
#include "newfile.moc"
#include <qtooltip.h>
#include <qobjectlist.h>
#include <qpoint.h>
#include "prefsfile.h"
#include "units.h"
#include "pagesize.h"
#include "marginWidget.h"
#include "scconfig.h"
#include "scribus.h"
#include "pluginmanager.h"
#include "prefsmanager.h"
#include "pagelayout.h"
#include "commonstrings.h"
#include "mspinbox.h"
#include "customfdialog.h"


extern QPixmap loadIcon(QString nam);


NewDoc::NewDoc( QWidget* parent, bool startUp ) : QDialog( parent, "newDoc", true, 0 )
{
	prefsManager=PrefsManager::instance();
	tabSelected = 0;
	onStartup = startUp;
	customText="Custom";
	customTextTR=QObject::tr( "Custom" );
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
		tabWidget->addTab(newDocFrame, tr("New Document"));
		createOpenDocPage();
		tabWidget->addTab(openDocFrame, tr("Open Document"));
		createRecentDocPage();
		tabWidget->addTab(recentDocFrame, tr("Recent Documents"));
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
	QToolTip::add( ComboBox1, tr( "Document page size, either a standard size or a custom size" ) );
	QToolTip::add( ComboBox2, tr( "Orientation of the document's pages" ) );
	QToolTip::add( Breite, tr( "Width of the document's pages, editable if you have chosen a custom page size" ) );
	QToolTip::add( Hoehe, tr( "Height of the document's pages, editable if you have chosen a custom page size" ) );
//	QToolTip::add( Doppelseiten, tr( "Enable single or spread based layout" ) );
//	QToolTip::add( ErsteSeite, tr( "Make the first page the left page of the document" ) );
	QToolTip::add( PgNr, tr( "First page number of the document" ) );
	QToolTip::add( PgNum, tr( "Initial number of pages of the document" ) );
	QToolTip::add( ComboBox3, tr( "Default unit of measurement for document editing" ) );
	QToolTip::add( AutoFrame, tr( "Create text frames automatically when new pages are added" ) );
	QToolTip::add( SpinBox10, tr( "Number of columns to create in automatically created text frames" ) );
	QToolTip::add( Distance, tr( "Distance between automatically created columns" ) );

	// signals and slots connections
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( ExitOK() ) );
	connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( docLayout, SIGNAL( selectedLayout(int) ), this, SLOT( setDS(int ) ) );
	connect(ComboBox1, SIGNAL(activated(const QString &)), this, SLOT(setPGsize(const QString &)));
	connect(ComboBox2, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(ComboBox3, SIGNAL(activated(int)), this, SLOT(setUnit(int)));
	connect(Distance, SIGNAL(valueChanged(int)), this, SLOT(setDist(int)));
	if (startUp)
		connect(recentDocList, SIGNAL(doubleClicked(QListBoxItem*)), this, SLOT(recentDocList_doubleClicked(QListBoxItem*)));

	setMinimumSize(minimumSizeHint());
	setMaximumSize(minimumSizeHint());
	resize(minimumSizeHint());
	clearWState( WState_Polished );
}

void NewDoc::createNewDocPage()
{
	newDocFrame = new QFrame(this, "newDocFrame");
	NewDocLayout = new QHBoxLayout( newDocFrame, 10, 5, "NewDocLayout");
	docLayout = new PageLayouts(newDocFrame, prefsManager->appPrefs.pageSets);
	docLayout->selectItem(prefsManager->appPrefs.FacingPages);
	docLayout->firstPage->setCurrentItem(prefsManager->appPrefs.pageSets[prefsManager->appPrefs.FacingPages].FirstPage);
	NewDocLayout->addWidget( docLayout );
	Layout9 = new QVBoxLayout(0, 0, 5, "Layout9");
	ButtonGroup1_2 = new QButtonGroup(newDocFrame, "ButtonGroup1_2" );
	ButtonGroup1_2->setTitle( tr( "Page Size" ));
	ButtonGroup1_2->setColumnLayout(0, Qt::Vertical);
	ButtonGroup1_2->layout()->setSpacing(6);
	ButtonGroup1_2->layout()->setMargin(10);
	ButtonGroup1_2Layout = new QVBoxLayout(ButtonGroup1_2->layout());
	ButtonGroup1_2Layout->setAlignment(Qt::AlignTop);
	Layout6 = new QGridLayout(0, 1, 1, 0, 6, "Layout6");
	TextLabel1 = new QLabel( tr( "&Size:" ), ButtonGroup1_2, "TextLabel1" );
	Layout6->addWidget( TextLabel1, 0, 0 );
	PageSize *ps=new PageSize(prefsManager->appPrefs.pageSize);
	ComboBox1 = new QComboBox( true, ButtonGroup1_2, "ComboBox1" );
	ComboBox1->insertStringList(ps->getTrPageSizeList());
	ComboBox1->insertItem( customTextTR );
	ComboBox1->setEditable(false);
	TextLabel1->setBuddy(ComboBox1);
	Layout6->addWidget(ComboBox1, 0, 1 );
	TextLabel2 = new QLabel( tr( "Orie&ntation:" ), ButtonGroup1_2, "TextLabel2" );
	Layout6->addWidget( TextLabel2, 1, 0 );
	ComboBox2 = new QComboBox( true, ButtonGroup1_2, "ComboBox2" );
	ComboBox2->insertItem( tr( "Portrait" ) );
	ComboBox2->insertItem( tr( "Landscape" ) );
	ComboBox2->setEditable(false);
	ComboBox2->setCurrentItem(prefsManager->appPrefs.pageOrientation);
	TextLabel2->setBuddy(ComboBox2);
	Layout6->addWidget( ComboBox2, 1, 1 );
	ButtonGroup1_2Layout->addLayout( Layout6 );

	Layout5 = new QHBoxLayout( 0, 0, 6, "Layout5");
	TextLabel1_2 = new QLabel( tr( "&Width:" ), ButtonGroup1_2, "TextLabel1_2" );
	Layout5->addWidget( TextLabel1_2 );
	Breite = new MSpinBox( 1, 10000, ButtonGroup1_2, precision );
	Breite->setMinimumSize( QSize( 70, 20 ) );
	Breite->setSuffix(unitSuffix);
	TextLabel1_2->setBuddy(Breite);
	Layout5->addWidget( Breite );
	TextLabel2_2 = new QLabel( tr( "&Height:" ), ButtonGroup1_2, "TextLabel2_2" );
	Layout5->addWidget( TextLabel2_2 );
	Hoehe = new MSpinBox( 1, 10000, ButtonGroup1_2, precision );
	Hoehe->setMinimumSize( QSize( 70, 20 ) );
	Hoehe->setSuffix(unitSuffix);
	TextLabel2_2->setBuddy(Hoehe);
	Layout5->addWidget( Hoehe );
	ButtonGroup1_2Layout->addLayout( Layout5 );
	Layout9->addWidget( ButtonGroup1_2 );

	struct MarginStruct marg;
	marg.Top = prefsManager->appPrefs.RandOben;
	marg.Bottom = prefsManager->appPrefs.RandUnten;
	marg.Left = prefsManager->appPrefs.RandLinks;
	marg.Right = prefsManager->appPrefs.RandRechts;
	GroupRand = new MarginWidget(newDocFrame,  tr( "Margin Guides" ), &marg, precision, unitRatio, unitSuffix );
	GroupRand->setPageWidthHeight(prefsManager->appPrefs.PageWidth, prefsManager->appPrefs.PageHeight);
	GroupRand->setFacingPages(prefsManager->appPrefs.FacingPages == doublePage);
	Layout9->addWidget( GroupRand );
	NewDocLayout->addLayout( Layout9 );
	Breite->setValue(prefsManager->appPrefs.PageWidth * unitRatio);
	Hoehe->setValue(prefsManager->appPrefs.PageHeight * unitRatio);
	QStringList pageSizes=ps->getPageSizeList();
	int sizeIndex=pageSizes.findIndex(ps->getPageText());
	if (sizeIndex!=-1)
		ComboBox1->setCurrentItem(sizeIndex);
	else
		ComboBox1->setCurrentItem(ComboBox1->count()-1);
	bool hwEnabled=(ComboBox1->currentText()==customTextTR);
	Breite->setEnabled(hwEnabled);
	Hoehe->setEnabled(hwEnabled);
	setDS(prefsManager->appPrefs.FacingPages);
	setSize(prefsManager->appPrefs.pageSize);
	setOrien(prefsManager->appPrefs.pageOrientation);
	Breite->setValue(prefsManager->appPrefs.PageWidth * unitRatio);
	Hoehe->setValue(prefsManager->appPrefs.PageHeight * unitRatio);
	Layout10 = new QVBoxLayout( 0, 0, 6, "Layout10");

	GroupBox3 = new QGroupBox( newDocFrame, "GroupBox3" );
	GroupBox3->setTitle( tr( "Options" ) );
	GroupBox3->setColumnLayout(0, Qt::Vertical );
	GroupBox3->layout()->setSpacing( 5 );
	GroupBox3->layout()->setMargin( 10 );
	GroupBox3Layout = new QGridLayout( GroupBox3->layout() );
	GroupBox3Layout->setAlignment( Qt::AlignTop );
	TextLabel1_3 = new QLabel( tr( "F&irst Page Number:" ), GroupBox3, "TextLabel1_3" );
	GroupBox3Layout->addMultiCellWidget( TextLabel1_3, 0, 0, 0, 1 );
	PgNr = new QSpinBox( GroupBox3, "PgNr" );
	PgNr->setMaxValue( 10000 );
	PgNr->setMinValue( 1 );
	TextLabel1_3->setBuddy(PgNr);
	GroupBox3Layout->addWidget( PgNr, 0, 2, Qt::AlignRight );
	TextLabel1_3a = new QLabel( tr( "N&umber of Pages:" ), GroupBox3, "TextLabel1_3a" );
	GroupBox3Layout->addMultiCellWidget( TextLabel1_3a, 1, 1, 0, 1 );
	PgNum = new QSpinBox( GroupBox3, "PgNum" );
	PgNum->setMaxValue( 10000 );
	PgNum->setMinValue( 1 );
	TextLabel1_3a->setBuddy(PgNum);
	GroupBox3Layout->addWidget( PgNum, 1, 2, Qt::AlignRight );
	TextLabel2_3 = new QLabel( tr( "&Default Unit:" ), GroupBox3, "TextLabel2_3" );
	GroupBox3Layout->addWidget( TextLabel2_3, 2, 0 );
	ComboBox3 = new QComboBox( true, GroupBox3, "ComboBox3" );
	ComboBox3->insertStringList(unitGetTextUnitList());
	ComboBox3->setCurrentItem(unitIndex);
	ComboBox3->setEditable(false);
	TextLabel2_3->setBuddy(ComboBox3);
	GroupBox3Layout->addMultiCellWidget( ComboBox3, 2, 2, 1, 2 );
	Layout10->addWidget( GroupBox3 );

	AutoFrame = new QGroupBox( newDocFrame, "GroupBox4" );
	AutoFrame->setTitle( tr( "&Automatic Text Frames" ) );
	AutoFrame->setColumnLayout(0, Qt::Vertical );
	AutoFrame->layout()->setSpacing( 0 );
	AutoFrame->layout()->setMargin( 0 );
	AutoFrame->setCheckable( true );
	AutoFrame->setChecked(false);
	GroupBox4Layout = new QHBoxLayout( AutoFrame->layout() );
	GroupBox4Layout->setAlignment( Qt::AlignTop );
	GroupBox4Layout->setSpacing( 5 );
	GroupBox4Layout->setMargin( 10 );
	Layout2 = new QGridLayout;
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 5 );
	TextLabel4 = new QLabel( tr( "&Gap:" ), AutoFrame, "TextLabel4" );
	Layout2->addWidget( TextLabel4, 1, 0 );
	TextLabel3 = new QLabel( tr( "Colu&mns:" ), AutoFrame, "TextLabel3" );
	Layout2->addWidget( TextLabel3, 0, 0 );
	Distance = new MSpinBox( 0, 1000, AutoFrame, precision );
	Distance->setSuffix( unitSuffix );
	Distance->setValue(11 * unitRatio);
	Dist = 11;
	TextLabel4->setBuddy(Distance);
	Layout2->addWidget( Distance, 1, 1, Qt::AlignLeft );
	SpinBox10 = new QSpinBox( AutoFrame, "SpinBox10" );
	SpinBox10->setButtonSymbols( QSpinBox::UpDownArrows );
	SpinBox10->setMinValue( 1 );
	SpinBox10->setValue( 1 );
	TextLabel3->setBuddy(SpinBox10);
	Layout2->addWidget( SpinBox10, 0, 1, Qt::AlignLeft );
	GroupBox4Layout->addLayout( Layout2 );
	Layout10->addWidget( AutoFrame );
	NewDocLayout->addLayout( Layout10 );
	delete ps;
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
	QString formats = "";
#ifdef HAVE_LIBZ
	formats += tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;");
#else
	formats += tr("Documents (*.sla *.scd);;");
#endif
	formats += ScApp->pluginManager->getImportFilterString();
	formats += tr("All Files (*)");
	openDocFrame = new QFrame(this, "openDocFrame");
	QVBoxLayout* openDocLayout = new QVBoxLayout(openDocFrame, 5,5, "openDocLayout");
	fileDialog = new CustomFDialog(openDocFrame, docDir, tr("Open"), formats, false,  true, false, false, false);
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
	recentDocList = new QListBox(recentDocFrame, "recentDocList");
	recentDocLayout->addWidget(recentDocList);
	uint max = QMIN(prefsManager->appPrefs.RecentDCount, ScApp->RecentDocs.count());
	for (uint m = 0; m < max; ++m)
	{
		recentDocList->insertItem(ScApp->RecentDocs[m]);
	}
}

void NewDoc::setBreite(int)
{
	Pagebr = Breite->value() / unitRatio;
	GroupRand->setPageWidth(Pagebr);
}

void NewDoc::setHoehe(int)
{
	Pageho = Hoehe->value() / unitRatio;
	GroupRand->setPageHeight(Pageho);
}

void NewDoc::setDist(int)
{
	Dist = Distance->value() / unitRatio;
}

void NewDoc::setUnit(int newUnitIndex)
{
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));

	double oldUnitRatio = unitRatio;
	double val, oldB, oldBM, oldH, oldHM;
	int decimals;
	Breite->getValues(&oldB, &oldBM, &decimals, &val);
	oldB /= oldUnitRatio;
	oldBM /= oldUnitRatio;
	Hoehe->getValues(&oldH, &oldHM, &decimals, &val);
	oldH /= oldUnitRatio;
	oldHM /= oldUnitRatio;

	unitIndex = newUnitIndex;
	unitRatio = unitGetRatioFromIndex(newUnitIndex);
	decimals = unitGetDecimalsFromIndex(newUnitIndex);
	if (ComboBox2->currentItem() == portraitPage)
	{
		Breite->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, Pagebr * unitRatio);
		Hoehe->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, Pageho * unitRatio);
	}
	else
	{
		Breite->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, Pageho * unitRatio);
		Hoehe->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, Pagebr * unitRatio);
	}
	Distance->setValue(Dist * unitRatio);
	unitSuffix = unitGetSuffixFromIndex(newUnitIndex);
	Breite->setSuffix(unitSuffix);
	Hoehe->setSuffix(unitSuffix);
	Distance->setSuffix( unitSuffix );
	GroupRand->unitChange(unitRatio, decimals, unitSuffix);
	GroupRand->setPageHeight(Pageho);
	GroupRand->setPageWidth(Pagebr);
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));

}

void NewDoc::ExitOK()
{
	Pagebr = Breite->value() / unitRatio;
	Pageho = Hoehe->value() / unitRatio;
	if (onStartup)
		tabSelected = tabWidget->currentPageIndex();
	else
		tabSelected = 0;
	accept();
}

void NewDoc::setOrien(int ori)
{
	double br;
	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	if (ori != Orient)
	{
		br = Breite->value();
		Breite->setValue(Hoehe->value());
		Hoehe->setValue(br);
	}
	// #869 pv - defined constants added + code repeat (check w/h)
	(ori == portraitPage) ? Orient = portraitPage : Orient = landscapePage;
	if (ComboBox1->currentText() == customTextTR)
	{
		if (Breite->value() > Hoehe->value())
			ComboBox2->setCurrentItem(landscapePage);
		else
			ComboBox2->setCurrentItem(portraitPage);
	}
	// end of #869
	GroupRand->setPageHeight(Pageho);
	GroupRand->setPageWidth(Pagebr);
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
}

void NewDoc::setPGsize(const QString &size)
{
	//if (ComboBox1->currentItem() == USERFORMAT)
	if (size == customTextTR)
		setSize(size);
	else
	{
		setSize(size);
		setOrien(ComboBox2->currentItem());
	}
}

void NewDoc::setSize(QString gr)
{
	Pagebr = Breite->value() / unitRatio;
	Pageho = Hoehe->value() / unitRatio;

	disconnect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	disconnect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
	Breite->setEnabled(false);
	Hoehe->setEnabled(false);
	/*
	int page_x[] = {2380, 1684, 1190, 842, 595, 421, 297, 210, 148, 105, 2836, 2004, 1418, 1002, 709, 501,
	                355, 250, 178, 125, 89, 462, 298, 312, 542, 595, 1224, 612, 612, 792};
	int page_y[] = {3368, 2380, 1684, 1190, 842, 595, 421, 297, 210, 148, 4008, 2836, 2004, 1418, 1002, 709,
	                501, 355, 250, 178, 125, 649, 683, 624, 720, 935, 792, 1008, 792, 1225};
	*/

	//if (gr == USERFORMAT)
	if (gr==customTextTR || gr==customText)
	{
		Breite->setEnabled(true);
		Hoehe->setEnabled(true);
	}
	else
	{
		PageSize *ps2=new PageSize(gr);
		// pv - correct handling of the disabled spins
		if (ComboBox2->currentItem() == portraitPage)
		{
			//Pagebr = page_x[gr];
			//Pageho = page_y[gr];
			Pagebr = ps2->getPageWidth();
			Pageho = ps2->getPageHeight();
		} else {
			Pagebr = ps2->getPageHeight();
			Pageho = ps2->getPageWidth();
			//Pagebr = page_y[gr];
			//Pageho = page_x[gr];
		}
	}
	Breite->setValue(Pagebr * unitRatio);
	Hoehe->setValue(Pageho * unitRatio);
	GroupRand->setPageHeight(Pageho);
	GroupRand->setPageWidth(Pagebr);
	connect(Breite, SIGNAL(valueChanged(int)), this, SLOT(setBreite(int)));
	connect(Hoehe, SIGNAL(valueChanged(int)), this, SLOT(setHoehe(int)));
}

void NewDoc::setDS(int layout)
{
	GroupRand->setFacingPages(!(layout == singlePage));
	choosenLayout = layout;
	docLayout->firstPage->setCurrentItem(prefsManager->appPrefs.pageSets[choosenLayout].FirstPage);
}

void NewDoc::recentDocList_doubleClicked(QListBoxItem * /*item*/)
{
	/* Yep. There is nothing to solve. ScribusApp handles all
	openings etc. It's Franz's programming style ;) */
	ExitOK();
}
