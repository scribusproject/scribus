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
	setupUi(this);
	gSk->setNewUnit(6);
	gSk->setValues(-89, 89, 2, 0);
	gSk_2->setNewUnit(6);
	gSk->setValues(-89, 89, 2, 0);
	gSc->setSuffix(" %");
	gSc->setValue( 100 );
	connect(gX1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gX2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gY1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gY2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gSk,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gX1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gX2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gY1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gY2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gFX,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gFY,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gSk_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gSc,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gC1X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC1Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC2X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC2Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC3X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC3Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC4X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC4Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	languageChange();
	selectLinear();
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

void GradientVectorDialog::selectLinear()
{
	stackedWidget->setCurrentIndex(0);
	resize(minimumSizeHint());
}

void GradientVectorDialog::selectRadial()
{
	stackedWidget->setCurrentIndex(1);
	resize(minimumSizeHint());
}

void GradientVectorDialog::selectFourColor()
{
	stackedWidget->setCurrentIndex(2);
	resize(minimumSizeHint());
}

void GradientVectorDialog::languageChange()
{
	retranslateUi(this);
	resize(minimumSizeHint());
}

void GradientVectorDialog::setValues(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk)
{
	disconnect(gX1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gX2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gY1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gY2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gSk,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gX1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gX2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gY1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gY2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gFX,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gFY,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gSk_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gSc,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gC1X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC1Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC2X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC2Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC3X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC3Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC4X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC4Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	gX1->setValue(x1);
	gX2->setValue(x2);
	gY1->setValue(y1);
	gY2->setValue(y2);
	gFX->setValue(fx);
	gFY->setValue(fy);
	gSk->setValue(sk);
	gSc->setValue(sg * 100.0);
	gX1_2->setValue(x1);
	gX2_2->setValue(x2);
	gY1_2->setValue(y1);
	gY2_2->setValue(y2);
	gSk_2->setValue(sk);
	gC1X->setValue(x1);
	gC1Y->setValue(y1);
	gC2X->setValue(x2);
	gC2Y->setValue(y2);
	gC3X->setValue(fx);
	gC3Y->setValue(fy);
	gC4X->setValue(sg);
	gC4Y->setValue(sk);
	connect(gX1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gX2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gY1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gY2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gSk,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gX1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gX2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gY1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gY2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gFX,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gFY,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gSk_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gSc,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gC1X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC1Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC2X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC2Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC3X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC3Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC4X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC4Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
}

void GradientVectorDialog::changeSpecialL()
{
	emit NewSpecial(gX1->value(), gY1->value(), gX2->value(), gY2->value(), gFX->value(), gFY->value(), 1, gSk->value());
}

void GradientVectorDialog::changeSpecialR()
{
	emit NewSpecial(gX1_2->value(), gY1_2->value(), gX2_2->value(), gY2_2->value(), gFX->value(), gFY->value(), gSc->value() / 100.0, gSk_2->value());
}

void GradientVectorDialog::changeSpecialF()
{
	emit NewSpecial(gC1X->value(), gC1Y->value(), gC2X->value(), gC2Y->value(), gC3X->value(), gC3Y->value(), gC4X->value(), gC4Y->value());
}

void GradientVectorDialog::unitChange(int unitIndex)
{
	disconnect(gX1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gX2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gY1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gY2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	disconnect(gX1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gX2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gY1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gY2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gFX,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gFY,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	disconnect(gC1X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC1Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC2X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC2Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC3X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC3Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC4X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	disconnect(gC4Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	gX1->setNewUnit(unitIndex);
	gY1->setNewUnit(unitIndex);
	gX2->setNewUnit(unitIndex);
	gY2->setNewUnit(unitIndex);
	gFX->setNewUnit(unitIndex);
	gFY->setNewUnit(unitIndex);
	gX1_2->setNewUnit(unitIndex);
	gY1_2->setNewUnit(unitIndex);
	gX2_2->setNewUnit(unitIndex);
	gY2_2->setNewUnit(unitIndex);
	gC1X->setNewUnit(unitIndex);
	gC1Y->setNewUnit(unitIndex);
	gC2X->setNewUnit(unitIndex);
	gC2Y->setNewUnit(unitIndex);
	gC3X->setNewUnit(unitIndex);
	gC3Y->setNewUnit(unitIndex);
	gC4X->setNewUnit(unitIndex);
	gC4Y->setNewUnit(unitIndex);
	connect(gX1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gX2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gY1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gY2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialL()));
	connect(gX1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gX2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gY1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gY2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gFX,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gFY,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialR()));
	connect(gC1X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC1Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC2X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC2Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC3X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC3Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC4X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
	connect(gC4Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialF()));
}
