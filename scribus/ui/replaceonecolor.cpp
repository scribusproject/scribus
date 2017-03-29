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

#include "replaceonecolor.h"
#include "iconmanager.h"
#include "util.h"

replaceColorDialog::replaceColorDialog(QWidget* parent, ColorList &availableColors, ColorList &usedColors) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon.png" ));
	originalColor->setPixmapType(ColorCombo::fancyPixmaps);
	originalColor->setColors(usedColors, true);
	replacementColor->setPixmapType(ColorCombo::fancyPixmaps);
	replacementColor->setColors(availableColors, true);
}

const QString replaceColorDialog::getOriginalColor()
{
	return originalColor->currentText();
}

const QString replaceColorDialog::getReplacementColor()
{
	return replacementColor->currentText();
}

void replaceColorDialog::setOriginalColor(QString color)
{
	setCurrentComboItem(originalColor, color);
}

void replaceColorDialog::setReplacementColor(QString color)
{
	setCurrentComboItem(replacementColor, color);
}
