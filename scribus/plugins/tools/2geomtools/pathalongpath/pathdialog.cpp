/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2007 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "pathdialog.h"

PathDialog::PathDialog(QWidget* parent, int unitIndex) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	effectType = 0;
	gap = 0.0;
	offset = 0.0;
	offsetY = 0.0;
	rotate = false;
	offsetXSpin->setNewUnit(unitIndex);
	offsetYSpin->setNewUnit(unitIndex);
	offsetYSpin->setMinimum(-1000);
	offsetYSpin->setMaximum(1000);
	gapSpin->setNewUnit(unitIndex);
	connect(offsetXSpin, SIGNAL(valueChanged(double)), this, SLOT(newOffset(double)));
	connect(offsetYSpin, SIGNAL(valueChanged(double)), this, SLOT(newOffsetY(double)));
	connect(gapSpin, SIGNAL(valueChanged(double)), this, SLOT(newGap(double)));
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(newType(int)));
	connect(previewCheck, SIGNAL(clicked()), this, SLOT(togglePreview()));
	connect(rotateCheckBox, SIGNAL(clicked()), this, SLOT(toggleRotate()));
}

void PathDialog::toggleRotate()
{
	rotate = rotateCheckBox->isChecked();
	if (previewCheck->isChecked())
		emit updateValues(effectType, offset, offsetY, gap, rotate);
}

void PathDialog::newOffsetY(double val)
{
	offsetY = val;
	if (previewCheck->isChecked())
		emit updateValues(effectType, offset, offsetY, gap, rotate);
}

void PathDialog::newOffset(double val)
{
	offset = val;
	if (previewCheck->isChecked())
		emit updateValues(effectType, offset, offsetY, gap, rotate);
}

void PathDialog::newGap(double val)
{
	gap = val;
	if (previewCheck->isChecked())
		emit updateValues(effectType, offset, offsetY, gap, rotate);
}

void PathDialog::newType(int val)
{
	effectType = val;
	if (previewCheck->isChecked())
		emit updateValues(effectType, offset, offsetY, gap, rotate);
}

void PathDialog::togglePreview()
{
	if (previewCheck->isChecked())
		emit updateValues(effectType, offset, offsetY, gap, rotate);
	else
		emit updateValues(-1, offset, offsetY, gap, rotate);
}
