#include "applytemplate.h"
#include "applytemplate.moc"

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>

#include "scribusdoc.h"
#include "page.h"
extern QPixmap loadIcon(QString nam);

ApplyT::ApplyT( QWidget* parent, ScribusDoc *view, QString Nam)
		: QDialog( parent, "apply", true, 0)
{
	setCaption( tr( "Apply Template" ) );
	setIcon(loadIcon("AppIcon.png"));
	ApplyTLayout = new QVBoxLayout( this, 10, 8, "ApplyTLayout");
	layout3 = new QHBoxLayout( 0, 0, 6, "layout3");
	TextLabel1 = new QLabel( this, "TextLabel1" );
	TextLabel1->setText( tr( "&Template:" ) );
	layout3->addWidget( TextLabel1 );
	Templ = new QComboBox( true, this, "Templ" );
	Templ->setMinimumSize( QSize( 120, 22 ) );
	Templ->setEditable(false);
	TextLabel1->setBuddy(Templ);
	QString na = Nam == "Normal" ? tr("Normal") : Nam, in;
	int cc = 0;
	QMap<QString,int>::Iterator it;
	for (it = view->MasterNames.begin(); it != view->MasterNames.end(); ++it)
	{
		in = it.key() == "Normal" ? tr("Normal") : it.key();
		Templ->insertItem(in);
		if (in == na)
			Templ->setCurrentItem(cc);
		cc++;
	}
	layout3->addWidget( Templ );
	ApplyTLayout->addLayout( layout3 );

	buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
	buttonGroup1->setColumnLayout(0, Qt::Vertical );
	buttonGroup1->layout()->setSpacing( 6 );
	buttonGroup1->layout()->setMargin( 11 );
	buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
	buttonGroup1Layout->setAlignment( Qt::AlignTop );

	singlePage = new QRadioButton( buttonGroup1, "radioButton1" );
	singlePage->setChecked( true );
	singlePage->setText( tr( "Apply to &current page" ) );
	buttonGroup1Layout->addWidget( singlePage );
	evenRange = new QRadioButton( buttonGroup1, "radioButton1" );
	evenRange->setText( tr( "Apply to all &even pages" ) );
	buttonGroup1Layout->addWidget( evenRange );
	if (view->Pages.count() < 2)
		evenRange->setEnabled(false);
	oddRange = new QRadioButton( buttonGroup1, "radioButton1" );
	oddRange->setText( tr( "Apply to all &odd pages" ) );
	buttonGroup1Layout->addWidget( oddRange );
	layout2 = new QHBoxLayout( 0, 0, 6, "layout2");
	pageRange = new QRadioButton( buttonGroup1, "radioButton2" );
	pageRange->setText( tr( "Apply to &all pages:" ) );
	layout2->addWidget( pageRange );
	fromPage = new QSpinBox( buttonGroup1, "fromPage" );
	fromPage->setMinValue( 1 );
	fromPage->setMaxValue(view->Pages.count());
	fromPage->setValue(view->currentPage->PageNr+1);
	layout2->addWidget( fromPage );
	TextLabel2 = new QLabel( buttonGroup1, "textLabel1" );
	TextLabel2->setText( tr( "To:" ) );
	layout2->addWidget( TextLabel2 );
	toPage = new QSpinBox( buttonGroup1, "toPage" );
	toPage->setMinValue( 1 );
	toPage->setMaxValue(view->Pages.count());
	toPage->setValue(view->Pages.count());
	layout2->addWidget( toPage );
	buttonGroup1Layout->addLayout( layout2 );
	ApplyTLayout->addWidget( buttonGroup1 );
	fromPage->setEnabled(false);
	toPage->setEnabled(false);

	Layout4 = new QHBoxLayout;
	Layout4->setSpacing( 6 );
	Layout4->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout4->addItem( spacer );
	okButton = new QPushButton( tr( "&OK" ), this, "okButton" );
	okButton->setDefault( true );
	Layout4->addWidget( okButton );
	cancelButton = new QPushButton( tr( "&Cancel" ), this, "cancelButton" );
	Layout4->addWidget( cancelButton );
	ApplyTLayout->addLayout( Layout4 );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect(singlePage, SIGNAL(clicked()), this, SLOT(SingleSel()));
	connect(evenRange, SIGNAL(clicked()), this, SLOT(SingleSel()));
	connect(oddRange, SIGNAL(clicked()), this, SLOT(SingleSel()));
	connect(pageRange, SIGNAL(clicked()), this, SLOT(RangeSel()));
	connect(fromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	connect(toPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
}

void ApplyT::RangeSel()
{
	fromPage->setEnabled(true);
	toPage->setEnabled(true);
}

void ApplyT::SingleSel()
{
	fromPage->setEnabled(false);
	toPage->setEnabled(false);
}

void ApplyT::CheckRangeF()
{
	disconnect(fromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	disconnect(toPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
	if (fromPage->value() > toPage->value())
		toPage->setValue(fromPage->value());
	connect(fromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	connect(toPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
}

void ApplyT::CheckRangeT()
{
	disconnect(fromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	disconnect(toPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
	if (toPage->value() < fromPage->value())
		fromPage->setValue(toPage->value());
	connect(fromPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeF()));
	connect(toPage, SIGNAL(valueChanged(int)), this, SLOT(CheckRangeT()));
}
