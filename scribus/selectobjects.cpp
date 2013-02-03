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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#include "selectobjects.h"
#include "commonstrings.h"
#include "util_icon.h"

selectDialog::selectDialog(QWidget* parent, ColorList &availableColors, int unitIndex) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	backgroundCombo->addItem(CommonStrings::tr_NoneColor);
	backgroundCombo->insertItems(availableColors, ColorCombo::fancyPixmaps);
	lineCombo->addItem(CommonStrings::tr_NoneColor);
	lineCombo->insertItems(availableColors, ColorCombo::fancyPixmaps);
	lineWidthSpin->setNewUnit(unitIndex);
	lineWidthSpin->setMinimum(0);
	lineWidthSpin->setMaximum(1000);
}

int selectDialog::getSelectionRange()
{
	int retval = 0;
	if (buttonCurrentPage->isChecked())
		retval = 0;
	else if (buttonCurrentLayer->isChecked())
		retval = 1;
	else if (buttonWorkspace->isChecked())
		retval = 2;
	return retval;
}

bool selectDialog::useAttributes()
{
	return attributeGroup->isChecked();
}

void selectDialog::getUsedAttributes(bool &Type, bool &Fill, bool &Line, bool &LWidth, bool &Print, bool &Locked, bool &Resize)
{
	Type = useItemType->isChecked();
	Fill = useFillColor->isChecked();
	Line = useLineColor->isChecked();
	LWidth = useLineWidth->isChecked();
	Print = usePrintState->isChecked();
	Locked = useLockedState->isChecked();
	Resize = useResizeState->isChecked();
}

void selectDialog::getUsedAttributesValues(int &Type, QString &Fill, QString &Line, double &LWidth, bool &Print, bool &Locked, bool &Resize)
{
	if (useItemType->isChecked())
	{
		Type = itemTypeCombo->currentIndex();
		if (Type == 0)
			Type = 4;
		else if (Type == 1)
			Type = 2;
		else if (Type == 2)
			Type = 6;
		else if (Type == 3)
			Type = 7;
		else if (Type == 4)
			Type = 5;
		else if (Type == 5)
			Type = 8;
		else if (Type == 6)
			Type = 9;
	}
	if (useFillColor->isChecked())
	{
		Fill = backgroundCombo->currentText();
		if (Fill == CommonStrings::tr_NoneColor)
			Fill = CommonStrings::None;
	}
	if (useLineColor->isChecked())
	{
		Line = lineCombo->currentText();
		if (Line == CommonStrings::tr_NoneColor)
			Line = CommonStrings::None;
	}
	if (useLineWidth->isChecked())
		LWidth = lineWidthSpin->value();
	if (usePrintState->isChecked())
	{
		if (printStateCombo->currentIndex() == 0)
			Print = true;
		else
			Print = false;
	}
	if (useLockedState->isChecked())
	{
		if (lockStateCombo->currentIndex() == 0)
			Locked = true;
		else
			Locked = false;
	}
	if (useResizeState->isChecked())
	{
		if (resizeStateCombo->currentIndex() == 0)
			Resize = false;
		else
			Resize = true;
	}
}
