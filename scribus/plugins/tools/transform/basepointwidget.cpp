/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "basepointwidget.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <QFrame>
#include <QGridLayout>
#include <QSpacerItem>

BasePointWidget::BasePointWidget( QWidget* parent ) : QGroupBox( parent )
{
	setTitle( tr("Origin"));
	Layout12a = new QGridLayout(this);
	Layout12a->setSpacing( 0 );
	Layout12a->setMargin( 5 );
	Layout12a->setAlignment( Qt::AlignTop );
	QSpacerItem* spacerT = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout12a->addItem( spacerT, 0, 1 );
	RotationGroup = new QButtonGroup(this);
	QSpacerItem* spacerL = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout12a->addItem( spacerL, 1, 0 );
	Layout12 = new QGridLayout();
	Layout12->setSpacing( 0 );
	Layout12->setMargin( 0 );
	TopLeft = new QRadioButton( this );
	TopLeft->setText( "" );
	TopLeft->setChecked( true );
	TopLeft->setLayoutDirection(Qt::RightToLeft);
	TopLeft->setMaximumSize( TopLeft->iconSize() );
	RotationGroup->addButton(TopLeft, 0);
	Layout12->addWidget( TopLeft, 0, 0, Qt::AlignCenter );
	Line1 = new QFrame( this );
	Line1->setMinimumSize( QSize( 20, 4 ) );
	Line1->setMaximumSize( QSize( 20, 4 ) );
	Line1->setFrameShape( QFrame::HLine );
	Line1->setFrameShadow( QFrame::Plain );
	Line1->setLineWidth( 3 );
	Layout12->addWidget( Line1, 0, 1, Qt::AlignCenter );
	TopRight = new QRadioButton( this );
	TopRight->setText( "" );
	TopRight->setMaximumSize( TopRight->iconSize() );
	RotationGroup->addButton(TopRight, 1);
	Layout12->addWidget( TopRight, 0, 2, Qt::AlignCenter );
	Line2 = new QFrame( this);
	Line2->setMinimumSize( QSize( 4, 20 ) );
	Line2->setMaximumSize( QSize( 4, 20 ) );
	Line2->setFrameShape( QFrame::VLine );
	Line2->setFrameShadow( QFrame::Plain );
	Line2->setLineWidth( 3 );
	Layout12->addWidget( Line2, 1, 0, Qt::AlignCenter );
	Center = new QRadioButton( this);
	Center->setText( "" );
	Center->setMaximumSize( Center->iconSize() );
	RotationGroup->addButton(Center, 2);
	Layout12->addWidget( Center, 1, 1, Qt::AlignCenter );
	Line4 = new QFrame( this );
	Line4->setMinimumSize( QSize( 4, 20 ) );
	Line4->setMaximumSize( QSize( 4, 20 ) );
	Line4->setFrameShadow( QFrame::Plain );
	Line4->setLineWidth( 3 );
	Line4->setFrameShape( QFrame::VLine );
	Layout12->addWidget( Line4, 1, 2, Qt::AlignCenter );
	BottomLeft = new QRadioButton( this );
	BottomLeft->setText( "" );
	BottomLeft->setLayoutDirection(Qt::RightToLeft);
	BottomLeft->setMaximumSize( BottomLeft->iconSize() );
	RotationGroup->addButton(BottomLeft, 3);
	Layout12->addWidget( BottomLeft, 2, 0, Qt::AlignCenter );
	Line5 = new QFrame( this );
	Line5->setMinimumSize( QSize( 20, 4 ) );
	Line5->setMaximumSize( QSize( 20, 4 ) );
	Line5->setFrameShape( QFrame::HLine );
	Line5->setFrameShadow( QFrame::Plain );
	Line5->setLineWidth( 3 );
	Layout12->addWidget( Line5, 2, 1, Qt::AlignCenter );
	BottomRight = new QRadioButton( this );
	BottomRight->setText( "" );
	BottomRight->setMaximumSize( BottomRight->iconSize() );
	RotationGroup->addButton(BottomRight, 4);
	Layout12->addWidget( BottomRight, 2, 2, Qt::AlignCenter );
	Layout12a->addLayout(Layout12, 1, 1);
	QSpacerItem* spacerR = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout12a->addItem( spacerR, 1, 2 );
	QSpacerItem* spacerB = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout12a->addItem( spacerB, 2, 1 );
}

