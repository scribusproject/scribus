/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
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
	setIcon(loadIcon("AppIcon.png"));
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
	ShiftH = new MSpinBox( -1000, 1000, this, 2 );
	ShiftH->setValue(Dx);
	Layout4->addWidget( ShiftH, 1, 1 );
	ShiftV = new MSpinBox( -1000, 1000, this, 2 );
	ShiftV->setValue(Dy);
	Layout4->addWidget( ShiftV, 2, 1 );
	QString Suffix[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	ShiftH->setSuffix(Suffix[Ein]);
	ShiftV->setSuffix(Suffix[Ein]);
	TextLabel1 = new QLabel( Ncopies, tr( "&Number of Copies:" ), this, "TextLabel1" );
	Layout4->addWidget( TextLabel1, 0, 0 );
	TextLabel1_2 = new QLabel( ShiftH, tr( "&Horizontal Shift:" ), this, "TextLabel1_2" );
	Layout4->addWidget( TextLabel1_2, 1, 0 );
	TextLabel1_2_2 = new QLabel( ShiftV, tr( "&Vertical Shift:" ), this, "TextLabel1_2_2" );
	Layout4->addWidget( TextLabel1_2_2, 2, 0 );
	MdupLayout->addLayout( Layout4 );
	Layout3 = new QHBoxLayout;
	Layout3->setSpacing( 6 );
	Layout3->setMargin( 0 );
	PushButton12 = new QPushButton( tr( "&OK" ), this, "PushButton12" );
	PushButton12->setDefault( true );
	Layout3->addWidget( PushButton12 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout3->addItem( spacer );
	PushButton13 = new QPushButton( tr( "&Cancel" ), this, "PushButton13" );
	Layout3->addWidget( PushButton13 );
	MdupLayout->addLayout( Layout3 );
	QWidget::setTabOrder ( Ncopies, ShiftH );
	QWidget::setTabOrder ( ShiftH, ShiftV );
	QWidget::setTabOrder ( ShiftV, PushButton12 );
	QWidget::setTabOrder ( PushButton12, PushButton13 );
	QWidget::setTabOrder ( PushButton13, Ncopies );
	Ncopies->setFocus();
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( PushButton12, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( PushButton13, SIGNAL( clicked() ), this, SLOT( reject() ) );
}


