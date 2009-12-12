/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          patternpropsdialog.cpp  -  description
                             -------------------
    begin                : Thu Nov 19 2009
    copyright            : (C) 2009 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "patternpropsdialog.h"
#include "util_icon.h"
#include "util_math.h"

PatternPropsDialog::PatternPropsDialog(QWidget* parent, int unitIndex, bool isStroke) : QDialog( parent )
{
	setModal(true);
	forStroke = isStroke;
	frame3Layout = new QVBoxLayout( this );
	frame3Layout->setMargin(0);
	frame3Layout->setSpacing(2);

	groupOffset = new QGroupBox( this );
	groupOffsetLayout = new QHBoxLayout( groupOffset );
	groupOffsetLayout->setSpacing( 2 );
	groupOffsetLayout->setMargin( 3 );
	groupOffsetLayout->setAlignment( Qt::AlignTop );
	textLabel1 = new QLabel( groupOffset );
	groupOffsetLayout->addWidget( textLabel1 );
	spinXoffset = new ScrSpinBox( -3000, 3000, groupOffset, 0);
	groupOffsetLayout->addWidget( spinXoffset );
	textLabel2 = new QLabel( groupOffset );
	groupOffsetLayout->addWidget( textLabel2 );
	spinYoffset = new ScrSpinBox( -3000, 3000, groupOffset, 0);
	groupOffsetLayout->addWidget( spinYoffset );
	frame3Layout->addWidget( groupOffset );

	groupScale = new QGroupBox( this );
	groupScaleLayout = new QGridLayout( groupScale );
	groupScaleLayout->setSpacing( 2 );
	groupScaleLayout->setMargin( 3 );
	groupScaleLayout->setAlignment( Qt::AlignTop );
	textLabel5 = new QLabel( groupScale );
	groupScaleLayout->addWidget( textLabel5, 0, 0 );
	spinXscaling = new ScrSpinBox( 0.01, 500, groupScale, 0);
	spinXscaling->setValue( 100 );
	groupScaleLayout->addWidget( spinXscaling, 0, 1 );
	textLabel6 = new QLabel( groupScale );
	groupScaleLayout->addWidget( textLabel6, 1, 0 );
	spinYscaling = new ScrSpinBox( 0.01, 500, groupScale, 0 );
	spinYscaling->setValue( 100 );
	groupScaleLayout->addWidget( spinYscaling, 1, 1 );
	keepScaleRatio = new LinkButton( groupScale );
	keepScaleRatio->setCheckable( true );
	keepScaleRatio->setAutoRaise( true );
	keepScaleRatio->setMaximumSize( QSize( 15, 32767 ) );
	groupScaleLayout->addWidget( keepScaleRatio, 0, 2, 2, 1 );
	frame3Layout->addWidget( groupScale );
	groupRotation = new QGroupBox( this );
	groupRotationLayout = new QHBoxLayout( groupRotation );
	groupRotationLayout->setSpacing( 2 );
	groupRotationLayout->setMargin( 3 );
	groupRotationLayout->setAlignment( Qt::AlignTop );
	textLabel7 = new QLabel( groupRotation );
	groupRotationLayout->addWidget( textLabel7 );
	spinAngle = new ScrSpinBox( -180, 180, groupRotation, 6 );
	groupRotationLayout->addWidget( spinAngle );
	frame3Layout->addWidget( groupRotation );

	groupSkew = new QGroupBox( this );
	groupSkewLayout = new QGridLayout( groupSkew );
	groupSkewLayout->setSpacing( 2 );
	groupSkewLayout->setMargin( 3 );
	groupSkewLayout->setAlignment( Qt::AlignTop );
	textLabel8 = new QLabel( groupSkew );
	groupSkewLayout->addWidget( textLabel8, 0, 0 );
	spinXSkew = new ScrSpinBox( -89, 89, groupSkew, 6);
	spinXSkew->setValue( 0 );
	groupSkewLayout->addWidget( spinXSkew, 0, 1 );
	textLabel9 = new QLabel( groupSkew );
	groupSkewLayout->addWidget( textLabel9, 1, 0 );
	spinYSkew = new ScrSpinBox( -89, 89, groupSkew, 6 );
	spinYSkew->setValue( 0 );
	groupSkewLayout->addWidget( spinYSkew, 1, 1 );
	frame3Layout->addWidget( groupSkew );

	if (forStroke)
	{
		groupSpace = new QGroupBox( this );
		groupSpaceLayout = new QHBoxLayout( groupSpace );
		groupSpaceLayout->setSpacing( 2 );
		groupSpaceLayout->setMargin( 3 );
		groupSpaceLayout->setAlignment( Qt::AlignTop );
		textLabel20 = new QLabel( groupSpace );
		groupSpaceLayout->addWidget( textLabel20 );
		spinSpacing = new ScrSpinBox( 1, 500, groupSpace, 0 );
		spinSpacing->setValue( 100 );
		groupSpaceLayout->addWidget( spinSpacing );
		frame3Layout->addWidget( groupSpace );
	}

	groupFlipLayout = new QHBoxLayout();
	groupFlipLayout->setSpacing( 2 );
	groupFlipLayout->setMargin( 3 );
	textLabel15 = new QLabel( this );
	groupFlipLayout->addWidget( textLabel15 );
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	groupFlipLayout->addItem( spacer );
	FlipH = new QToolButton( this );
	FlipH->setIcon(QIcon(loadIcon("16/flip-object-horizontal.png")));
	FlipH->setCheckable( true );
	groupFlipLayout->addWidget( FlipH );
	FlipV = new QToolButton( this );
	FlipV->setIcon(QIcon(loadIcon("16/flip-object-vertical.png")));
	FlipV->setCheckable( true );
	groupFlipLayout->addWidget( FlipV );
	frame3Layout->addLayout( groupFlipLayout );

	buttonLayout = new QHBoxLayout;
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(5);
	QSpacerItem* hspacing = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	buttonLayout->addItem(hspacing);
	buttonOk = new QPushButton( this );
	buttonOk->setAutoDefault( TRUE );
	buttonOk->setDefault( TRUE );
	buttonLayout->addWidget( buttonOk );
	frame3Layout->addLayout( buttonLayout );
	spinXoffset->setNewUnit(unitIndex);
	spinYoffset->setNewUnit(unitIndex);
	languageChange();
	connect(spinXoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinYoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinXSkew, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinYSkew, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinAngle, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(keepScaleRatio, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	connect(FlipH, SIGNAL(clicked()), this, SLOT(changePatternProps()));
	connect(FlipV, SIGNAL(clicked()), this, SLOT(changePatternProps()));
	connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));
	if (forStroke)
		connect(spinSpacing, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
}

void PatternPropsDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PatternPropsDialog::languageChange()
{
	setWindowTitle( tr( "Pattern Properties" ));
	QString ptSuffix=tr(" pt");
	QString pctSuffix=tr(" %");
	groupOffset->setTitle( tr( "Offsets" ) );
	textLabel1->setText( tr( "X:" ) );
	spinXoffset->setSuffix( ptSuffix );
	textLabel2->setText( tr( "Y:" ) );
	spinYoffset->setSuffix( ptSuffix );
	groupScale->setTitle( tr( "Scaling" ) );
	textLabel5->setText( tr( "X-Scale:" ) );
	spinXscaling->setSuffix( pctSuffix );
	textLabel6->setText( tr( "Y-Scale:" ) );
	spinYscaling->setSuffix( pctSuffix );
	groupRotation->setTitle( tr( "Rotation" ) );
	groupSkew->setTitle( tr( "Skewing" ) );
	if (forStroke)
	{
		groupSpace->setTitle( tr( "Spacing" ) );
		spinSpacing->setSuffix( pctSuffix );
		textLabel20->setText( tr( "Value:" ) );
	}
	textLabel8->setText( tr( "X-Skew:" ) );
	textLabel9->setText( tr( "Y-Skew:" ) );
	textLabel7->setText( tr( "Angle:" ) );
	textLabel15->setText( tr( "Flip:" ) );
	buttonOk->setText( tr("Close"));
	resize(minimumSizeHint());
}

void PatternPropsDialog::changePatternProps()
{
	double a    = M_PI / 180.0 * spinXSkew->value();
	double b    = M_PI / 180.0 * spinYSkew->value();
	double sina = tan(a);
	double sinb = tan(b);
	bool fH = FlipH->isChecked();
	bool fV = FlipV->isChecked();
	if (forStroke)
		emit NewPatternPropsS(spinXscaling->value(), spinYscaling->value(), spinXoffset->value(), spinYoffset->value(), spinAngle->value(), sina, sinb, spinSpacing->value() / 100.0, fH, fV);
	else
		emit NewPatternProps(spinXscaling->value(), spinYscaling->value(), spinXoffset->value(), spinYoffset->value(), spinAngle->value(), sina, sinb, fH, fV);
}

void PatternPropsDialog::ToggleKette()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isChecked())
	{
		spinYscaling->setValue(spinXscaling->value());
		changePatternProps();
		keepScaleRatio->setChecked(true);
	}
	else
		keepScaleRatio->setChecked(false);
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void PatternPropsDialog::HChange()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isChecked())
		spinYscaling->setValue(spinXscaling->value());
	changePatternProps();
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void PatternPropsDialog::VChange()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isChecked())
		spinXscaling->setValue(spinYscaling->value());
	changePatternProps();
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}
