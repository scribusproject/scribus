#include "applytemplate.h"
#include "applytemplate.moc"
extern QPixmap loadIcon(QString nam);

ApplyT::ApplyT( QWidget* parent, ScribusView *view)
    : QDialog( parent, "apply", true, 0)
{
    setCaption( tr( "Apply Template" ) );
  	setIcon(loadIcon("AppIcon.xpm"));
    ApplyTLayout = new QVBoxLayout( this, 10, 8, "ApplyTLayout");
    layout3 = new QHBoxLayout( 0, 0, 6, "layout3");
    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Template:" ) );
    layout3->addWidget( TextLabel1 );
    Templ = new QComboBox( true, this, "Templ" );
    Templ->setMinimumSize( QSize( 120, 22 ) );
		Templ->setEditable(false);
		QMap<QString,int>::Iterator it;
		for (it = view->MasterNames.begin(); it != view->MasterNames.end(); ++it)
			{
			it.key() == "Normal" ? Templ->insertItem(tr("Normal")) : Templ->insertItem(it.key());
			}
    layout3->addWidget( Templ );
    ApplyTLayout->addLayout( layout3 );

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    SinglePage = new QRadioButton( buttonGroup1, "radioButton1" );
    SinglePage->setChecked( TRUE );
    SinglePage->setText( tr( "Apply to Actual Page" ) );
    buttonGroup1Layout->addWidget( SinglePage );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2");
    PageRange = new QRadioButton( buttonGroup1, "radioButton2" );
		PageRange->setText( tr( "Apply from Page:" ) );
    layout2->addWidget( PageRange );
    FromPage = new QSpinBox( buttonGroup1, "FromPage" );
    FromPage->setMinValue( 1 );
		FromPage->setMaxValue(view->Pages.count());
		FromPage->setValue(view->Doc->ActPage->PageNr+1);
    layout2->addWidget( FromPage );
    TextLabel2 = new QLabel( buttonGroup1, "textLabel1" );
    TextLabel2->setText( tr( "to:" ) );
    layout2->addWidget( TextLabel2 );
    ToPage = new QSpinBox( buttonGroup1, "toPage" );
    ToPage->setMinValue( 1 );
		ToPage->setMaxValue(view->Pages.count());
		ToPage->setValue(view->Pages.count());
    layout2->addWidget( ToPage );
    buttonGroup1Layout->addLayout( layout2 );
    ApplyTLayout->addWidget( buttonGroup1 );
		FromPage->setEnabled(false);
		ToPage->setEnabled(false);

    Layout4 = new QHBoxLayout; 
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer );
    OKButton = new QPushButton( this, "OKButton" );
    OKButton->setText( tr( "OK" ) );
    OKButton->setDefault( TRUE );
    Layout4->addWidget( OKButton );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer_2 );
    CancelB = new QPushButton( this, "CancelB" );
    CancelB->setText( tr( "Cancel" ) );
    Layout4->addWidget( CancelB );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer_3 );
    ApplyTLayout->addLayout( Layout4 );
    connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
		connect(SinglePage, SIGNAL(clicked()), this, SLOT(SingleSel()));
		connect(PageRange, SIGNAL(clicked()), this, SLOT(RangeSel()));
		connect(FromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
		connect(ToPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
}

void ApplyT::RangeSel()
{
	FromPage->setEnabled(true);
	ToPage->setEnabled(true);
}

void ApplyT::SingleSel()
{
	FromPage->setEnabled(false);
	ToPage->setEnabled(false);
}

void ApplyT::CheckRangeF()
{
	disconnect(FromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	disconnect(ToPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
	if (FromPage->value() > ToPage->value())
		ToPage->setValue(FromPage->value());
	connect(FromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	connect(ToPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
}

void ApplyT::CheckRangeT()
{
	disconnect(FromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	disconnect(ToPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
	if (ToPage->value() < FromPage->value())
		FromPage->setValue(ToPage->value());
	connect(FromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	connect(ToPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
}
