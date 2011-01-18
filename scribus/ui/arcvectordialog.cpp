/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          arcvectordialog.cpp  -  description
                             -------------------
    begin                : Mon Jan 17 2011
    copyright            : (C) 2011 by Franz Schmid
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

#include "arcvectordialog.h"

ArcVectorDialog::ArcVectorDialog(QWidget* parent) : ScrPaletteBase( parent, "ArcVectorDialog", false, 0 )
{
	setupUi(this);
	startAngle->setNewUnit(6);
	startAngle->setValues(-360, 360, 1, 0);
	sweepAngle->setNewUnit(6);
	sweepAngle->setValues(-360, 360, 1, 0);
	connect(startAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(sweepAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(arcHeight,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(arcWidth,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(exitButton, SIGNAL(clicked()), this, SIGNAL(endEdit()));
	languageChange();
	resize(minimumSizeHint());
}

void ArcVectorDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void ArcVectorDialog::languageChange()
{
	retranslateUi(this);
	resize(minimumSizeHint());
}

void ArcVectorDialog::setValues(double start, double sweep, double height, double width)
{
	disconnect(startAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	disconnect(sweepAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	disconnect(arcHeight,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	disconnect(arcWidth,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	startAngle->setValue(start);
	sweepAngle->setValue(sweep);
	arcHeight->setValue(height);
	arcWidth->setValue(width);
	connect(startAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(sweepAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(arcHeight,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(arcWidth,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
}

void ArcVectorDialog::changeVectors()
{
	emit NewVectors(startAngle->value(), sweepAngle->value(), arcHeight->value(), arcWidth->value());
}

void ArcVectorDialog::unitChange(int unitIndex)
{
	disconnect(arcHeight,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	disconnect(arcWidth,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	arcHeight->setNewUnit(unitIndex);
	arcWidth->setNewUnit(unitIndex);
	connect(arcHeight,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(arcWidth,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
}
