/****************************************************************************
** Form implementation generated from reading ui file 'MovePage.ui'
**
** Created: Thu Jun 14 18:48:22 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "movepage.h"
#include "movepage.moc"
extern QPixmap loadIcon(QString nam);

/* 
 *  Constructs a MovePages which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
MovePages::MovePages( QWidget* parent, int act, int max, bool art )
    : QDialog( parent, "MovePages", true, 0 )
{
		Wie = art;
		if (art)
    	setCaption( tr( "Move Pages" ) );
    else
    	setCaption( tr( "Copy Page" ) );
  	setIcon(loadIcon("AppIcon.xpm"));
    DLayout = new QVBoxLayout( this );
    DLayout->setSpacing( 5 );
    DLayout->setMargin( 10 );
    Layout4 = new QGridLayout;
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 5 );
    TextLabel1 = new QLabel( this, "TextLabel1" );
    if (art)
    	TextLabel1->setText( tr( "Move Page(s):" ) );
    else
    	TextLabel1->setText( tr("Copy Page")+":" );
    Layout4->addWidget( TextLabel1, 0, 0);
    FromPage = new QSpinBox( this, "FromPage" );
    FromPage->setMinValue( 1 );
    FromPage->setValue( act );
    FromPage->setMaxValue( max );
    Layout4->addWidget( FromPage, 0, 1);
    if (art)
    	{
    	TextLabel2 = new QLabel( this, "TextLabel2" );
    	TextLabel2->setText( tr( "to:" ) );
    	Layout4->addWidget( TextLabel2, 0, 2);
    	ToPage = new QSpinBox( this, "ToPage" );
    	ToPage->setMinValue( 1 );
    	ToPage->setValue( act );
    	ToPage->setMaxValue( max );
    	Layout4->addWidget( ToPage, 0, 3);
    	}
    Where = new QComboBox( true, this, "Where" );
    Where->insertItem(tr("before Page"));
    Where->insertItem(tr("after Page"));
    Where->insertItem(tr("To End"));
    Where->setEditable(false);
    Where->setCurrentItem(2);
		Layout4->addMultiCellWidget( Where, 1, 1, 0, 1 );
    ActualPage = new QSpinBox( this, "ActualPage" );
    ActualPage->setMinValue( 1 );
    ActualPage->setValue( act );
    ActualPage->setMaxValue( max );
		Layout4->addWidget( ActualPage, 1, 3 );
		Layout4->addColSpacing(0, TextLabel1->fontMetrics().width(tr( "Move Page(s):" )));
    DLayout->addLayout( Layout4 );
    Layout3 = new QHBoxLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );
    OK = new QPushButton( this, "OK" );
    OK->setText( tr( "OK" ) );
    OK->setDefault( true );
    Layout3->addWidget(OK);
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer );
    Cancel = new QPushButton( this, "Cancel" );
    Cancel->setText( tr( "Cancel" ) );
    Layout3->addWidget(Cancel);
    DLayout->addLayout( Layout3 );
    setMaximumSize(sizeHint());

    // signals and slots connections
    if (art)
    	connect( ToPage, SIGNAL( valueChanged(int) ), this, SLOT( ToChanged(int) ) );
    connect( FromPage, SIGNAL( valueChanged(int) ), this, SLOT( FromChanged(int) ) );
    connect( OK, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void MovePages::FromChanged(int nr)
{
	if (Wie)
		{
    if (nr > ToPage->value())
    	ToPage->setValue(nr);
    if ((nr == 1) && (ToPage->value() == ToPage->maxValue()))
    	ToPage->setValue(ToPage->maxValue()-1);
    }
}

void MovePages::ToChanged(int nr)
{
    if (nr < FromPage->value())
    	FromPage->setValue(nr);
    if ((FromPage->value() == 1) && (nr == ToPage->maxValue()))
    	FromPage->setValue(2);
}

