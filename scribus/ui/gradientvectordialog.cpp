/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          gradientvectordialog.cpp  -  description
                             -------------------
    begin                : Tue Nov 17 2009
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

#include "gradientvectordialog.h"

GradientVectorDialog::GradientVectorDialog(QWidget* parent) : ScrPaletteBase( parent, "GradientVectorPalette", false, 0 )
{
	freeGradientLayout = new QGridLayout(this);
	freeGradientLayout->setMargin(5);
	freeGradientLayout->setSpacing(5);
	GTextX1 = new QLabel("X1:", this );
	freeGradientLayout->addWidget( GTextX1, 0, 0 );
	GTextY1 = new QLabel("Y1:", this );
	freeGradientLayout->addWidget( GTextY1, 0, 2 );
	gX1 = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gX1, 0, 1 );
	gY1 = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gY1, 0, 3 );
	GTextX2 = new QLabel("X2:", this );
	freeGradientLayout->addWidget( GTextX2, 1, 0 );
	GTextY2 = new QLabel("Y2:", this );
	freeGradientLayout->addWidget( GTextY2, 1, 2 );
	gX2 = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gX2, 1, 1 );
	gY2 = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gY2, 1, 3 );
	GTextFX = new QLabel("FX:", this );
	freeGradientLayout->addWidget( GTextFX, 2, 0 );
	GTextFY = new QLabel("FY:", this );
	freeGradientLayout->addWidget( GTextFY, 2, 2 );
	gFX = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gFX, 2, 1 );
	gFY = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gFY, 2, 3 );
	GTextSK = new QLabel("Skew:", this );
	freeGradientLayout->addWidget( GTextSK, 3, 0, 1, 2 );
	gSk = new ScrSpinBox( -89, 89, this, 6);
	gSk->setValue( 0 );
	freeGradientLayout->addWidget( gSk, 3, 2, 1, 2 );
	GTextSC = new QLabel("Factor:", this );
	freeGradientLayout->addWidget( GTextSC, 4, 0, 1, 2 );
	gSc = new ScrSpinBox( 0.01, 100, this, 0);
	gSc->setSuffix(" %");
	gSc->setValue( 100 );
	freeGradientLayout->addWidget( gSc, 4, 2, 1, 2 );
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gFX, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gFY, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gSk, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gSc, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	languageChange();
}

void GradientVectorDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void GradientVectorDialog::hideExtraWidgets()
{
	GTextFX->hide();
	GTextFY->hide();
	GTextSC->hide();
	gFX->hide();
	gFY->hide();
	gSc->hide();
	resize(minimumSizeHint());
}

void GradientVectorDialog::showExtraWidgets()
{
	GTextFX->show();
	GTextFY->show();
	GTextSC->show();
	gFX->show();
	gFY->show();
	gSc->show();
	resize(minimumSizeHint());
}

void GradientVectorDialog::languageChange()
{
	setWindowTitle( tr( "Gradient Vector" ));
	resize(minimumSizeHint());
}

void GradientVectorDialog::setValues(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk)
{
	disconnect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gFX, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gFY, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gSk, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gSc, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	gX1->setValue(x1);
	gX2->setValue(x2);
	gY1->setValue(y1);
	gY2->setValue(y2);
	gFX->setValue(fx);
	gFY->setValue(fy);
	gSk->setValue(sk);
	gSc->setValue(sg * 100.0);
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gFX, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gFY, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gSk, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gSc, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
}

void GradientVectorDialog::changeSpecial()
{
	emit NewSpecial(gX1->value(), gY1->value(), gX2->value(), gY2->value(), gFX->value(), gFY->value(), gSc->value() / 100.0, gSk->value());
}

void GradientVectorDialog::unitChange(int unitIndex)
{
	disconnect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gFX, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gFY, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	gX1->setNewUnit(unitIndex);
	gY1->setNewUnit(unitIndex);
	gX2->setNewUnit(unitIndex);
	gY2->setNewUnit(unitIndex);
	gFX->setNewUnit(unitIndex);
	gFY->setNewUnit(unitIndex);
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gFX, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gFY, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
}
