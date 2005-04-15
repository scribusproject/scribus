/****************************************************************************
** Form implementation generated from reading ui file 'DelPage.ui'
**
** Created: Thu Jun 14 11:37:58 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "delpages.h"
#include "delpages.moc"
extern QPixmap loadIcon(QString nam);

/*
 *  Constructs a DelPages which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DelPages::DelPages( QWidget* parent, int act, int max )
		: QDialog( parent, "DelPages", true, 0 )
{
	setCaption( tr( "Delete Pages" ) );
	setIcon(loadIcon("AppIcon.png"));
	DLayout = new QVBoxLayout( this );
	DLayout->setSpacing( 5 );
	DLayout->setMargin( 10 );
	Layout4 = new QHBoxLayout;
	Layout4->setSpacing( 6 );
	Layout4->setMargin( 5 );
	TextLabel1 = new QLabel( this, "TextLabel1" );
	TextLabel1->setText( tr( "Delete from:" ) );
	Layout4->addWidget( TextLabel1 );
	FromPage = new QSpinBox( 1, max, 1, this, "FromPage" );
	FromPage->setValue( act );
	Layout4->addWidget( FromPage );
	TextLabel2 = new QLabel( this, "TextLabel2" );
	TextLabel2->setText( tr( "to:" ) );
	Layout4->addWidget( TextLabel2 );
	ToPage = new QSpinBox( 1, max, 1, this, "ToPage" );
	ToPage->setValue( act );
	Layout4->addWidget( ToPage );
	DLayout->addLayout( Layout4 );
	Layout3 = new QHBoxLayout;
	Layout3->setSpacing( 6 );
	Layout3->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout3->addItem( spacer );
	OK = new QPushButton( tr( "&OK" ), this, "OK" );
	OK->setDefault( true );
	Layout3->addWidget(OK);
	Cancel = new QPushButton( tr( "&Cancel" ), this, "Cancel" );
	Cancel->setDefault( false );
	Layout3->addWidget(Cancel);
	DLayout->addLayout( Layout3 );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( OK, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( FromPage, SIGNAL( valueChanged(int) ), this, SLOT( FromChanged(int) ) );
	connect( ToPage, SIGNAL( valueChanged(int) ), this, SLOT( ToChanged(int) ) );
}

void DelPages::FromChanged(int nr)
{
	if (nr > ToPage->value())
		ToPage->setValue(nr);
	if ((nr == 1) && (ToPage->value() == ToPage->maxValue()))
		ToPage->setValue(ToPage->maxValue()-1);
}

void DelPages::ToChanged(int nr)
{
	if (nr < FromPage->value())
		FromPage->setValue(nr);
	if ((FromPage->value() == 1) && (nr == ToPage->maxValue()))
		FromPage->setValue(2);
}

