/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          spiralvectordialog.cpp  -  description
                             -------------------
    begin                : Wed Jan 27 2011
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

#include "spiralvectordialog.h"

SpiralVectorDialog::SpiralVectorDialog(QWidget* parent) : ScrPaletteBase( parent, "SpiralVectorDialog", false, 0 )
{
	setupUi(this);
	startAngle->setNewUnit(6);
	startAngle->setValues(0, 36000, 1, 0);
	sweepAngle->setNewUnit(6);
	sweepAngle->setValues(0, 36000, 1, 0);
	connect(startAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(sweepAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(arcFactor,   SIGNAL(valueChanged(int)), this, SLOT(changeVectors()));
	connect(exitButton, SIGNAL(clicked()), this, SIGNAL(endEdit()));
	languageChange();
	resize(minimumSizeHint());
}

void SpiralVectorDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void SpiralVectorDialog::languageChange()
{
	retranslateUi(this);
	resize(minimumSizeHint());
}

void SpiralVectorDialog::setValues(double start, double sweep, double factor)
{
	disconnect(startAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	disconnect(sweepAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	disconnect(arcFactor,   SIGNAL(valueChanged(int)), this, SLOT(changeVectors()));
	startAngle->setValue(start);
	sweepAngle->setValue(sweep);
	startAngle->setMaximum(sweep - 1);
	sweepAngle->setMinimum(start + 1);
	arcFactor->setValue(qRound(factor * 100) - 100);
	connect(startAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(sweepAngle,   SIGNAL(valueChanged(double)), this, SLOT(changeVectors()));
	connect(arcFactor,   SIGNAL(valueChanged(int)), this, SLOT(changeVectors()));
}

void SpiralVectorDialog::changeVectors()
{
	startAngle->setMaximum(sweepAngle->value() - 1);
	sweepAngle->setMinimum(startAngle->value() + 1);
	emit NewVectors(startAngle->value(), sweepAngle->value(), (arcFactor->value() + 100) / 100.0);
}
