/****************************************************************************
** Form implementation generated from reading ui file 'Cdel.ui'
**
** Created: Tue Apr 24 22:08:43 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "dcolor.h"
#include "dcolor.moc"
#include <qpixmap.h>
#include <qiconset.h>
extern QPixmap loadIcon(QString nam);

DelColor::DelColor( QWidget* parent, CListe farben, QString Fnam, bool HDoc)
    : QDialog( parent, "dd", true, 0 )
{
	setName( "DelColor" );
    setCaption( tr( "Delete Color" ) );
  	setIcon(loadIcon("AppIcon.png"));
    DLayout = new QVBoxLayout( this );
    DLayout->setSpacing( 5 );
    DLayout->setMargin( 10 );
    Layout4 = new QGridLayout;
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 5 );
    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Delete color:" ) );
    Layout4->addWidget( TextLabel1, 0, 0 );
    DColor = new QLabel( this, "DColor" );
    DColor->setText( Fnam );
    Layout4->addWidget( DColor, 0, 1 );
    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( tr( "?" ) );
    Layout4->addWidget( TextLabel3, 0, 2 );
	if (HDoc)
	{
    	TextLabel4 = new QLabel( this, "TextLabel4" );
    	TextLabel4->setText( tr( "Replace it with:" ) );
    	Layout4->addWidget( TextLabel4, 1, 0 );
    	Ersatz = new QComboBox(false, this);
		Ersatz->setEditable(false);
		CListe::Iterator it;
		QPixmap pm = QPixmap(15, 15);
		farben.remove(Fnam);
		for (it = farben.begin(); it != farben.end(); ++it)
		{
			pm.fill(farben[it.key()].getRGBColor());
			Ersatz->insertItem(pm, it.key());
		}
    	Layout4->addWidget( Ersatz, 1, 1 );
    	EFarbe = Ersatz->text(0);
	}
    DLayout->addLayout( Layout4 );
    Layout3 = new QHBoxLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );
    PushButton2 = new QPushButton( this, "PushButton12" );
    PushButton2->setText( tr( "OK" ) );
    Layout3->addWidget( PushButton2 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer );
    PushButton3 = new QPushButton( this, "PushButton13" );
    PushButton3->setText( tr( "Cancel" ) );
    PushButton3->setDefault( TRUE );
    Layout3->addWidget( PushButton3 );
    DLayout->addLayout( Layout3 );
    setMaximumSize(sizeHint());
    connect( PushButton2, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( PushButton3, SIGNAL( clicked() ), this, SLOT( reject() ) );
	if (HDoc)
    	connect( Ersatz, SIGNAL(activated(int)), this, SLOT( ReplaceColor(int) ) );
}

void DelColor::ReplaceColor(int id)
{
    EFarbe = Ersatz->text(id);
}

