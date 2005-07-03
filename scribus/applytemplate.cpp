/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "applytemplate.h"
#include "applytemplate.moc"
extern QPixmap loadIcon(QString nam);

ApplyT::ApplyT( QWidget* parent, ScribusView *view, QString Nam)
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

	SinglePage = new QRadioButton( buttonGroup1, "radioButton1" );
	/* PFJ - 28/02/04 - altered from TRUE to true */
	SinglePage->setChecked( true );
	SinglePage->setText( tr( "Apply to &Current Page" ) );
	buttonGroup1Layout->addWidget( SinglePage );
	EvenRange = new QRadioButton( buttonGroup1, "radioButton1" );
	EvenRange->setText( tr( "Apply to all &even Pages" ) );
	buttonGroup1Layout->addWidget( EvenRange );
	if (view->Pages.count() < 2)
		EvenRange->setEnabled(false);
	OddRange = new QRadioButton( buttonGroup1, "radioButton1" );
	OddRange->setText( tr( "Apply to all &odd Pages" ) );
	buttonGroup1Layout->addWidget( OddRange );
	layout2 = new QHBoxLayout( 0, 0, 6, "layout2");
	PageRange = new QRadioButton( buttonGroup1, "radioButton2" );
	PageRange->setText( tr( "Apply from &Page:" ) );
	layout2->addWidget( PageRange );
	FromPage = new QSpinBox( buttonGroup1, "FromPage" );
	FromPage->setMinValue( 1 );
	FromPage->setMaxValue(view->Pages.count());
	FromPage->setValue(view->Doc->ActPage->PageNr+1);
	layout2->addWidget( FromPage );
	TextLabel2 = new QLabel( buttonGroup1, "textLabel1" );
	TextLabel2->setText( tr( "To:" ) );
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
	OKButton = new QPushButton( tr( "&OK" ), this, "OKButton" );
	OKButton->setDefault( true );
	Layout4->addWidget( OKButton );
	CancelB = new QPushButton( tr( "&Cancel" ), this, "CancelB" );
	Layout4->addWidget( CancelB );
	ApplyTLayout->addLayout( Layout4 );
	connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect(SinglePage, SIGNAL(clicked()), this, SLOT(SingleSel()));
	connect(EvenRange, SIGNAL(clicked()), this, SLOT(SingleSel()));
	connect(OddRange, SIGNAL(clicked()), this, SLOT(SingleSel()));
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
