/****************************************************************************
** Form implementation generated from reading ui file 'Query.ui'
**
** Created: Mon Apr 23 20:49:10 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "newtemp.h"
extern QPixmap loadIcon(QString nam);

#include <qtooltip.h>

NewTm::NewTm( QWidget* parent, QString text, QString titel, ScribusDoc *doc)
    : QDialog( parent, "newt", true, 0 )
{
    setCaption( tr( titel ) );
  	setIcon(loadIcon("AppIcon.png"));
    QueryLayout = new QVBoxLayout( this );
    QueryLayout->setSpacing( 6 );
    QueryLayout->setMargin( 11 );
    Layout2 = new QHBoxLayout;
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );
    Frage = new QLabel( this, "Frage" );
    Frage->setFrameShape( QLabel::MShape );
    Frage->setFrameShadow( QLabel::MShadow );
    Frage->setText( tr( text ) );
    Frage->adjustSize();
    Layout2->addWidget( Frage );
    Answer = new QLineEdit( this, "Answer" );
    Layout2->addWidget( Answer );
    QueryLayout->addLayout( Layout2 );
    if (doc->PageFP)
    	{
    	Layout3 = new QHBoxLayout;
    	Layout3->setSpacing( 6 );
    	Layout3->setMargin( 0 );
    	Links = new QComboBox( true, this, "links" );
    	Links->insertItem(tr("Left Page"));
    	Links->insertItem(tr("Right Page"));
    	Links->setEditable(false);
    	Layout3->addWidget( Links );
    	QueryLayout->addLayout( Layout3 );
    	}
    Layout1 = new QHBoxLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );
    PushButton1 = new QPushButton( this, "PushButton1" );
    PushButton1->setText( tr( "OK" ) );
    PushButton1->setDefault( true );
    Layout1->addWidget( PushButton1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );
    PushButton2 = new QPushButton( this, "PushButton2" );
    PushButton2->setText( tr( "Cancel" ) );
    Layout1->addWidget( PushButton2 );
    QueryLayout->addLayout( Layout1 );
    setMaximumSize(sizeHint());
    Answer->setFocus();

    // signals and slots connections
    connect( PushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( PushButton2, SIGNAL( clicked() ), this, SLOT( reject() ) );

}

