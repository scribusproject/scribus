#include "mdup.h"
#include "mdup.moc"

#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

Mdup::Mdup( QWidget* parent, double Dx, double Dy, int Ein )
    : QDialog( parent, "m", true, 0 )
{
    setCaption( tr( "Multiple Duplicate" ) );
  	setIcon(loadIcon("AppIcon.xpm"));
    MdupLayout = new QVBoxLayout( this );
    MdupLayout->setSpacing( 5 );
    MdupLayout->setMargin( 10 );
    Layout4 = new QGridLayout;
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );
    Ncopies = new QSpinBox( this, "Ncopies" );
    Ncopies->setMaxValue( 100 );
    Ncopies->setMinValue( 1 );
    Ncopies->setValue(1);
    Layout4->addWidget( Ncopies, 0, 1 );
    ShiftV = new MSpinBox( this, 2 );
    ShiftV->setMaxValue( 100000 );
    ShiftV->setMinValue( -100000 );
    ShiftV->setValue(qRound(Dy * 100));
    ShiftV->setLineStep(100);
    Layout4->addWidget( ShiftV, 2, 1 );
    ShiftH = new MSpinBox( this, 2 );
    ShiftH->setMaxValue( 100000 );
    ShiftH->setMinValue( -100000 );
    ShiftH->setValue(qRound(Dx  * 100));
    ShiftH->setLineStep(100);
    Layout4->addWidget( ShiftH, 1, 1 );
		switch (Ein)
			{
			case 0:
    		ShiftH->setSuffix( tr( " pt" ) );
    		ShiftV->setSuffix( tr( " pt" ) );
				break;
			case 1:
    		ShiftH->setSuffix( tr( " mm" ) );
    		ShiftV->setSuffix( tr( " mm" ) );
				break;
			case 2:
    		ShiftH->setSuffix( tr( " in" ) );
    		ShiftV->setSuffix( tr( " in" ) );
				break;
			case 3:
    		ShiftH->setSuffix( tr( " p" ) );
    		ShiftV->setSuffix( tr( " p" ) );
				break;
			}
    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Number of Copies:" ) );
    Layout4->addWidget( TextLabel1, 0, 0 );
    TextLabel1_2 = new QLabel( this, "TextLabel1_2" );
    TextLabel1_2->setText( tr( "Horizontal Shift:" ) );
    Layout4->addWidget( TextLabel1_2, 1, 0 );
    TextLabel1_2_2 = new QLabel( this, "TextLabel1_2_2" );
    TextLabel1_2_2->setText( tr( "Vertical Shift:" ) );
    Layout4->addWidget( TextLabel1_2_2, 2, 0 );
    MdupLayout->addLayout( Layout4 );
    Layout3 = new QHBoxLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );
    PushButton12 = new QPushButton( this, "PushButton12" );
    PushButton12->setText( tr( "OK" ) );
    PushButton12->setDefault( true );
    Layout3->addWidget( PushButton12 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer );
    PushButton13 = new QPushButton( this, "PushButton13" );
    PushButton13->setText( tr( "Cancel" ) );
    Layout3->addWidget( PushButton13 );
    MdupLayout->addLayout( Layout3 );
    setMaximumSize(sizeHint());

    // signals and slots connections
    connect( PushButton12, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( PushButton13, SIGNAL( clicked() ), this, SLOT( reject() ) );
}


