/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2008 by Franz Schmid                                    *
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

#include "pathconnectdialog.h"

PathConnectDialog::PathConnectDialog(QWidget* parent) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	firstLinePoint->setCurrentIndex(1);
	secondLinePoint->setCurrentIndex(0);
	modeCombo->setCurrentIndex(0);
	connect(firstLinePoint, SIGNAL(activated(int)), this, SLOT(pointsChanged()));
	connect(secondLinePoint, SIGNAL(activated(int)), this, SLOT(pointsChanged()));
	connect(modeCombo, SIGNAL(activated(int)), this, SLOT(pointsChanged()));
	connect(previewCheck, SIGNAL(clicked()), this, SLOT(togglePreview()));
}

void PathConnectDialog::pointsChanged()
{
	if (previewCheck->isChecked())
		emit updateValues(0, getFirstLinePoint(), getSecondLinePoint(), getMode());
}

void PathConnectDialog::togglePreview()
{
	if (previewCheck->isChecked())
		emit updateValues(0, getFirstLinePoint(), getSecondLinePoint(), getMode());
	else
		emit updateValues(-1, getFirstLinePoint(), getSecondLinePoint(), getMode());
}

int PathConnectDialog::getMode()
{
	return modeCombo->currentIndex();
}

int PathConnectDialog::getFirstLinePoint()
{
	return firstLinePoint->currentIndex();
}

int PathConnectDialog::getSecondLinePoint()
{
	return secondLinePoint->currentIndex();
}
