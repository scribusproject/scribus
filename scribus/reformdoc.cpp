#include "reformdoc.h"
#include "reformdoc.moc"
#include <qtooltip.h>
#include "scribusdoc.h"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

ReformDoc::ReformDoc( QWidget* parent, ScribusDoc* doc ) : PrefsDialogBase( parent )
{
	einheit = doc->Einheit;
	QString units[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	QString ein = units[doc->Einheit];
	int dp[] = {100, 1000, 10000, 100};
	int decimals = dp[doc->Einheit];
	int i=-1;
	Breite = doc->PageB * UmReFaktor;
	Hoehe = doc->PageH * UmReFaktor;
	setCaption( tr( "Document Setup" ) );
	prefsSelection->insertItem( tr("Page"));
	tabPage = new QWidget( prefsWidgets, "tab" );
	ReformDocLayout = new QVBoxLayout( tabPage );
	ReformDocLayout->setSpacing( 6 );
	ReformDocLayout->setMargin( 10 );
	dsGroupBox7 = new QGroupBox( tabPage, "GroupBox7" );
	dsGroupBox7->setTitle( tr( "Page Size" ) );
	dsGroupBox7->setColumnLayout(0, Qt::Vertical );
	dsGroupBox7->layout()->setSpacing( 0 );
	dsGroupBox7->layout()->setMargin( 0 );
	dsGroupBox7Layout = new QHBoxLayout( dsGroupBox7->layout() );
	dsGroupBox7Layout->setAlignment( Qt::AlignTop );
	dsGroupBox7Layout->setSpacing( 0 );
	dsGroupBox7Layout->setMargin( 10 );
	dsLayout4 = new QGridLayout;
	dsLayout4->setSpacing( 6 );
	dsLayout4->setMargin( 0 );
	sizeQComboBox = new QComboBox( true, dsGroupBox7, "sizeQComboBox" );
	sizeQLabel = new QLabel( sizeQComboBox, tr( "&Size:" ), dsGroupBox7, "sizeQLabel" );
	QString sizelist[] = {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3", "B4",
	                      "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", "Executive", "Folio",
	                      "Ledger", tr("Legal"), tr("Letter"), tr("Tabloid"), tr("Custom")};
	size_t const num_mappings = (sizeof sizelist)/(sizeof *sizelist);
	for (uint m = 0; m < num_mappings; ++m)
	{
		sizeQComboBox->insertItem(sizelist[m]);
		if (sizelist[m]==doc->PageSize)
			i=m;
	}
	//set Custom if we dont have one already as old docs wont have this attribute
	if (i==-1)
		i=num_mappings-1;
	sizeQComboBox->setEnabled(false);
	sizeQComboBox->setCurrentItem(i);
	dsLayout4->addWidget( sizeQLabel, 0, 0 );
	dsLayout4->addWidget( sizeQComboBox, 0, 1 );
	orientationQComboBox = new QComboBox( true, dsGroupBox7, "orientationQComboBox" );
	orientationQLabel = new QLabel( orientationQComboBox, tr( "Orie&ntation:" ), dsGroupBox7, "orientationQLabel" );
	orientationQComboBox->insertItem( tr( "Portrait" ) );
	orientationQComboBox->insertItem( tr( "Landscape" ) );
	orientationQComboBox->setEnabled(false);
	orientationQComboBox->setCurrentItem(doc->PageOri);
	dsLayout4->addWidget( orientationQLabel, 0, 2 );
	dsLayout4->addWidget( orientationQComboBox, 0, 3 );
	widthMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, 2 );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7, "widthLabel" );
	widthMSpinBox->setEnabled( false );
	widthMSpinBox->setSuffix(ein);
	widthMSpinBox->setValue(Breite);
	widthQLabel->setBuddy(widthMSpinBox);
	dsLayout4->addWidget( widthQLabel, 1, 0 );
	dsLayout4->addWidget( widthMSpinBox, 1, 1 );
	heightMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, 2 );
	heightQLabel = new QLabel( tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	heightMSpinBox->setEnabled( false );
	heightMSpinBox->setSuffix(ein);
	heightMSpinBox->setValue(Hoehe);
	heightQLabel->setBuddy(heightMSpinBox);
	dsLayout4->addWidget( heightQLabel, 1, 2 );
	dsLayout4->addWidget( heightMSpinBox, 1, 3 );
	dsGroupBox7Layout->addLayout( dsLayout4 );
	ReformDocLayout->addWidget( dsGroupBox7 );
	GroupBox7 = new QGroupBox( tabPage, "GroupBox7" );
	GroupBox7->setTitle( tr( "Margin Guides" ) );
	GroupBox7->setColumnLayout(0, Qt::Vertical );
	GroupBox7->layout()->setSpacing( 0 );
	GroupBox7->layout()->setMargin( 0 );
	GroupBox7Layout = new QHBoxLayout( GroupBox7->layout() );
	GroupBox7Layout->setAlignment( Qt::AlignTop );
	GroupBox7Layout->setSpacing( 0 );
	GroupBox7Layout->setMargin( 10 );
	Layout4 = new QGridLayout;
	Layout4->setSpacing( 6 );
	Layout4->setMargin( 0 );
	TopR = new MSpinBox( GroupBox7, 4 );
	TopR->setSuffix( ein );
	TopR->setDecimals( decimals );
	TopR->setMaxValue(Hoehe);
	TopR->setValue(doc->PageM.Top * UmReFaktor);
	Layout4->addWidget( TopR, 0, 1 );
	TextLabel5 = new QLabel( tr( "&Top:" ), GroupBox7, "TextLabel5" );
	TextLabel5->setBuddy(TopR);
	Layout4->addWidget( TextLabel5, 0, 0 );
	LeftR = new MSpinBox( GroupBox7, 4 );
	LeftR->setSuffix( ein );
	LeftR->setDecimals( decimals );
	LeftR->setMaxValue(Breite);
	LeftR->setValue(doc->PageM.Left * UmReFaktor);
	Layout4->addWidget( LeftR, 0, 3 );
	Links = new QLabel( tr( "&Left:" ), GroupBox7, "Links" );
	Links->setBuddy(LeftR);
	Layout4->addWidget( Links, 0, 2 );
	BottomR = new MSpinBox( GroupBox7, 4 );
	BottomR->setSuffix( ein );
	BottomR->setDecimals( decimals );
	BottomR->setMaxValue(Hoehe);
	BottomR->setValue(doc->PageM.Bottom * UmReFaktor);
	Layout4->addWidget( BottomR, 1, 1 );
	TextLabel7 = new QLabel( tr( "&Bottom:" ), GroupBox7, "TextLabel7" );
	TextLabel7->setBuddy(BottomR);
	Layout4->addWidget( TextLabel7, 1, 0 );
	RightR = new MSpinBox( GroupBox7, 4 );
	RightR->setSuffix( ein );
	RightR->setDecimals( decimals );
	RightR->setMaxValue(Breite);
	RightR->setValue(doc->PageM.Right * UmReFaktor);
	Layout4->addWidget( RightR, 1, 3 );
	Rechts = new QLabel( tr( "&Right:" ), GroupBox7, "Rechts" );
	Rechts->setBuddy(RightR);
	Layout4->addWidget( Rechts, 1, 2 );
	Doppelseiten = new QCheckBox( tr( "&Facing Pages" ),GroupBox7, "Doppelseiten" );
	Doppelseiten->setChecked( doc->PageFP );
	Layout4->addMultiCellWidget( Doppelseiten, 2, 2, 0, 1 );
	ErsteSeite = new QCheckBox( tr( "Left &Page First" ), GroupBox7, "n" );
	ErsteSeite->setChecked( doc->FirstPageLeft );
	Layout4->addMultiCellWidget( ErsteSeite, 2, 2, 2, 3 );
	if (!doc->PageFP)
		ErsteSeite->setEnabled(false);
	setDS();
	TextLabel1_3 = new QLabel( tr( "F&irst Page Number:" ), GroupBox7, "TextLabel1_3" );
	Layout4->addMultiCellWidget( TextLabel1_3, 3, 3, 0, 1 );
	PgNr = new QSpinBox( GroupBox7, "PgNr" );
	PgNr->setMaxValue( 1000 );
	PgNr->setMinValue( 1 );
	PgNr->setValue(doc->FirstPnum);
	Layout4->addWidget( PgNr, 3, 2, Qt::AlignRight );
	TextLabel1_3->setBuddy(PgNr);
	GroupBox7Layout->addLayout( Layout4 );
	ReformDocLayout->addWidget( GroupBox7 );
	prefsWidgets->addWidget( tabPage, 0 );

	prefsSelection->insertItem( tr("View"));
	tabView = new QWidget( prefsWidgets, "tabView" );
	tabViewLayout = new QVBoxLayout( tabView, 11, 6, "tabViewLayout");
	checkMargin = new QCheckBox( tabView, "checkMargin" );
	checkMargin->setText( tr( "Show Margins" ) );
	checkMargin->setChecked(doc->MarginsShown);
	tabViewLayout->addWidget( checkMargin );
	checkFrame = new QCheckBox( tabView, "checkFrame" );
	checkFrame->setText( tr( "Show Frames" ) );
	checkFrame->setChecked(doc->FramesShown);
	tabViewLayout->addWidget( checkFrame );
	checkPictures = new QCheckBox( tabView, "checkPictures" );
	checkPictures->setText( tr( "Show Pictures" ) );
	checkPictures->setChecked(doc->ShowPic);
	tabViewLayout->addWidget( checkPictures );
	checkGrid = new QCheckBox( tabView, "checkGrid" );
	checkGrid->setText( tr( "Show Grid" ) );
	checkGrid->setChecked(doc->GridShown);
	tabViewLayout->addWidget( checkGrid );
	checkGuides = new QCheckBox( tabView, "checkGuides" );
	checkGuides->setText( tr( "Show Guides" ) );
	checkGuides->setChecked(doc->GuidesShown);
	tabViewLayout->addWidget( checkGuides );
	checkBaseline = new QCheckBox( tabView, "checkBaseline" );
	checkBaseline->setText( tr( "Show Baseline Grid" ) );
	checkBaseline->setChecked(doc->BaseShown);
	tabViewLayout->addWidget( checkBaseline );
	prefsWidgets->addWidget( tabView, 1 );

	RightR->setMaxValue(Breite - LeftR->value());
	LeftR->setMaxValue(Breite - RightR->value());
	TopR->setMaxValue(Hoehe - BottomR->value());
	BottomR->setMaxValue(Hoehe - TopR->value());
	//tooltips
	QToolTip::add( Doppelseiten, tr( "Enable single or spread based layout" ) );
	QToolTip::add( ErsteSeite, tr( "Make the first page the left page of the document" ) );
	QToolTip::add( TopR, tr( "Distance between the top margin guide and the edge of the page" ) );
	QToolTip::add( BottomR, tr( "Distance between the bottom margin guide and the edge of the page" ) );
	QToolTip::add( LeftR, tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( RightR, tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );

	// signals and slots connections
	connect( Doppelseiten, SIGNAL( clicked() ), this, SLOT( setDS() ) );
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
	prefsWidgets->raiseWidget(0);
	resize( minimumSizeHint() );
	clearWState( WState_Polished );
}

void ReformDoc::setTop(int )
{
	BottomR->setMaxValue(Hoehe - TopR->value());
}

void ReformDoc::setBottom(int )
{
	TopR->setMaxValue(Hoehe - BottomR->value());
}

void ReformDoc::setLeft(int )
{
	RightR->setMaxValue(Breite - LeftR->value());
}

void ReformDoc::setRight(int )
{
	LeftR->setMaxValue(Breite - RightR->value());
}

void ReformDoc::setDS()
{
	if (Doppelseiten->isChecked())
	{
		Links->setText( tr( "&Inside:" ) );
		Rechts->setText( tr( "&Outside:" ) );
		ErsteSeite->setEnabled(true);
	}
	else
	{
		Links->setText( tr( "&Left:" ) );
		Rechts->setText( tr( "&Right:" ) );
		ErsteSeite->setEnabled(false);
	}
}
