/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "query.h"
#include "query.moc"
extern QPixmap loadIcon(QString nam);

#include <qtooltip.h>
/* 
 *  Constructs a Query which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Query::Query( QWidget* parent,  const char* name, bool modal, WFlags fl, QString text, QString titel )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
		setName( "Query" );
    setCaption( titel );
  	setIcon(loadIcon("AppIcon.png"));
    QueryLayout = new QVBoxLayout( this );
    QueryLayout->setSpacing( 6 );
    QueryLayout->setMargin( 11 );
    Layout2 = new QHBoxLayout;
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );
    Answer = new QLineEdit( this, "Answer" );
    Frage = new QLabel( Answer, text, this, "Frage" );
    Frage->setFrameShape( QLabel::MShape );
    Frage->setFrameShadow( QLabel::MShadow );
    Frage->adjustSize();
    Layout2->addWidget( Frage );
    Layout2->addWidget( Answer );
    QueryLayout->addLayout( Layout2 );
    Layout1 = new QHBoxLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );
    PushButton1 = new QPushButton( tr( "&OK" ), this, "PushButton1" );
    PushButton1->setDefault( true );
    Layout1->addWidget( PushButton1 );
    PushButton2 = new QPushButton( tr( "&Cancel"), this, "PushButton2" );
    Layout1->addWidget( PushButton2 );
    QueryLayout->addLayout( Layout1 );
    setMaximumSize(sizeHint());
    Answer->setFocus();

    // signals and slots connections
    connect( PushButton1, SIGNAL( clicked() ), this, SLOT( Leave() ) );
    connect( PushButton2, SIGNAL( clicked() ), this, SLOT( reject() ) );

    // tab order
    setTabOrder( Answer, PushButton1 );
    setTabOrder( PushButton1, PushButton2 );
}

void Query::Leave()
{
	if (Answer->text() == "")
		return;
	else
		accept();
}

