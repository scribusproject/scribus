#include "reformdoc.h"
#include "reformdoc.moc"
#include <qtooltip.h>
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

ReformDoc::ReformDoc( QWidget* parent, double t, double l, double r, double b, double Pagebr, double Pageho, bool fp, bool fpe, int Einh)
		: QDialog( parent, "r", true, 0 )
{
	einheit = Einh;
	QString units[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	QString ein = units[Einh];
	int dp[] = {100, 1000, 10000, 100};
	int decimals = dp[Einh];
	Breite = Pagebr * UmReFaktor;
	Hoehe = Pageho * UmReFaktor;
	setCaption( tr( "Document Setup" ) );
	setIcon(loadIcon("AppIcon.png"));
	ReformDocLayout = new QVBoxLayout( this );
	ReformDocLayout->setSpacing( 6 );
	ReformDocLayout->setMargin( 10 );
	GroupBox7 = new QGroupBox( this, "GroupBox7" );
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
	TextLabel5 = new QLabel( GroupBox7, "TextLabel5" );
	TextLabel5->setText( tr( "Top:" ) );
	Layout4->addWidget( TextLabel5, 0, 0 );
	TopR = new MSpinBox( GroupBox7, 4 );
	TopR->setSuffix( ein );
	TopR->setDecimals( decimals );
	TopR->setMaxValue(Hoehe);
	TopR->setValue(t * UmReFaktor);
	Layout4->addWidget( TopR, 0, 1 );
	Links = new QLabel( GroupBox7, "Links" );
	Links->setText( tr( "Left:" ) );
	Layout4->addWidget( Links, 0, 2 );
	LeftR = new MSpinBox( GroupBox7, 4 );
	LeftR->setSuffix( ein );
	LeftR->setDecimals( decimals );
	LeftR->setMaxValue(Breite);
	LeftR->setValue(l * UmReFaktor);
	Layout4->addWidget( LeftR, 0, 3 );
	TextLabel7 = new QLabel( GroupBox7, "TextLabel7" );
	TextLabel7->setText( tr( "Bottom:" ) );
	Layout4->addWidget( TextLabel7, 1, 0 );
	BottomR = new MSpinBox( GroupBox7, 4 );
	BottomR->setSuffix( ein );
	BottomR->setDecimals( decimals );
	BottomR->setMaxValue(Hoehe);
	BottomR->setValue(b * UmReFaktor);
	Layout4->addWidget( BottomR, 1, 1 );
	Rechts = new QLabel( GroupBox7, "Rechts" );
	Rechts->setText( tr( "Right:" ) );
	Layout4->addWidget( Rechts, 1, 2 );
	RightR = new MSpinBox( GroupBox7, 4 );
	RightR->setSuffix( ein );
	RightR->setDecimals( decimals );
	RightR->setMaxValue(Breite);
	RightR->setValue(r * UmReFaktor);
	Layout4->addWidget( RightR, 1, 3 );
	Doppelseiten = new QCheckBox( GroupBox7, "Doppelseiten" );
	Doppelseiten->setText( tr( "Facing Pages" ) );
	Doppelseiten->setChecked( fp );
	Layout4->addMultiCellWidget( Doppelseiten, 2, 2, 0, 1 );
	ErsteSeite = new QCheckBox( GroupBox7, "n" );
	ErsteSeite->setText( tr( "Left Page first" ) );
	ErsteSeite->setChecked( fpe );
	Layout4->addMultiCellWidget( ErsteSeite, 2, 2, 2, 3 );
	if (!fp)
		ErsteSeite->setEnabled(false);
	setDS();
	GroupBox7Layout->addLayout( Layout4 );
	ReformDocLayout->addWidget( GroupBox7 );
	Layout3 = new QHBoxLayout;
	Layout3->setSpacing( 6 );
	Layout3->setMargin( 0 );
	OKButton = new QPushButton( this, "OKButton" );
	OKButton->setText( tr( "OK" ) );
	OKButton->setDefault( true );
	Layout3->addWidget( OKButton );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout3->addItem( spacer );
	CancelB = new QPushButton( this, "CancelB" );
	CancelB->setText( tr( "Cancel" ) );
	CancelB->setAutoDefault( false );
	Layout3->addWidget( CancelB );
	ReformDocLayout->addLayout( Layout3 );
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
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(TopR, SIGNAL(valueChanged(int)), this, SLOT(setTop(int)));
	connect(BottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom(int)));
	connect(LeftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft(int)));
	connect(RightR, SIGNAL(valueChanged(int)), this, SLOT(setRight(int)));
}

void ReformDoc::setTop(int v)
{
	BottomR->setMaxValue(Hoehe - TopR->value());
}

void ReformDoc::setBottom(int v)
{
	TopR->setMaxValue(Hoehe - BottomR->value());
}

void ReformDoc::setLeft(int v)
{
	RightR->setMaxValue(Breite - LeftR->value());
}

void ReformDoc::setRight(int v)
{
	LeftR->setMaxValue(Breite - RightR->value());
}

void ReformDoc::setDS()
{
	if (Doppelseiten->isChecked())
	{
		Links->setText( tr( "Inside:" ) );
		Rechts->setText( tr( "Outside:" ) );
		ErsteSeite->setEnabled(true);
	}
	else
	{
		Links->setText( tr( "Left:" ) );
		Rechts->setText( tr( "Right:" ) );
		ErsteSeite->setEnabled(false);
	}
}

