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

#include "replacecolors.h"
#include "replaceonecolor.h"
#include "commonstrings.h"
#include "sccolorengine.h"
#include "util_color.h"
#include "util_icon.h"
#include <QHeaderView>

replaceColorsDialog::replaceColorsDialog(QWidget* parent, ColorList &colorList, ColorList &colorListUsed) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	EditColors = colorList;
	UsedColors = colorListUsed;
	selectedRow = -1;
	replaceMap.clear();
	alertIcon = loadIcon("alert.png", true);
	cmykIcon = loadIcon("cmyk.png", true);
	rgbIcon = loadIcon("rgb.png", true);
	spotIcon = loadIcon("spot.png", true);
	regIcon = loadIcon("register.png", true);
	replacementTable->horizontalHeader()->setMovable(false);
	replacementTable->horizontalHeader()->setClickable(false);
	replacementTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	replacementTable->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Original")));
	replacementTable->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Replacement")));
	replacementTable->verticalHeader()->setMovable(false);
	replacementTable->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	replacementTable->verticalHeader()->hide();
	replacementTable->setIconSize(QSize(60, 15));
	updateReplacementTable();
	removeButton->setEnabled(false);
	connect(addButton, SIGNAL(clicked()), this, SLOT(addColor()));
	connect(replacementTable, SIGNAL(cellClicked(int, int)), this, SLOT(selReplacement(int)));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(delReplacement()));
	connect(editButton, SIGNAL(clicked()), this, SLOT(editReplacement()));
}

void replaceColorsDialog::addColor()
{
	replaceColorDialog *dia = new replaceColorDialog(this, EditColors, UsedColors);
	if (dia->exec())
	{
		QString orig = dia->getOriginalColor();
		if (orig == CommonStrings::tr_NoneColor)
			orig = CommonStrings::None;
		QString repl = dia->getReplacementColor();
		if (repl == CommonStrings::tr_NoneColor)
			repl = CommonStrings::None;
		replaceMap.insert(orig, repl);
		updateReplacementTable();
	}
	delete dia;
}

void replaceColorsDialog::selReplacement(int sel)
{
	selectedRow = sel;
	removeButton->setEnabled(true);
	editButton->setEnabled(true);
}

void replaceColorsDialog::delReplacement()
{
	if (selectedRow > -1)
	{
		replaceMap.remove(replacementTable->item(selectedRow, 0)->text());
		replacementTable->removeRow(selectedRow);
		selectedRow = -1;
		removeButton->setEnabled(false);
		editButton->setEnabled(false);
	}
}

void replaceColorsDialog::editReplacement()
{
	if (selectedRow > -1)
	{
		replaceColorDialog *dia = new replaceColorDialog(this, EditColors, UsedColors);
		dia->setReplacementColor(replacementTable->item(selectedRow, 1)->text());
		dia->setOriginalColor(replacementTable->item(selectedRow, 0)->text());
		if (dia->exec())
		{
			replaceMap.remove(replacementTable->item(selectedRow, 0)->text());
			QString orig = dia->getOriginalColor();
			if (orig == CommonStrings::tr_NoneColor)
				orig = CommonStrings::None;
			QString repl = dia->getReplacementColor();
			if (repl == CommonStrings::tr_NoneColor)
				repl = CommonStrings::None;
			replaceMap.insert(orig, repl);
			updateReplacementTable();
		}
		delete dia;
	}
}

void replaceColorsDialog::updateReplacementTable()
{
	replacementTable->clearContents();
	replacementTable->setRowCount(0);
	selectedRow = -1;
	removeButton->setEnabled(false);
	editButton->setEnabled(false);
	if (replaceMap.count() > 0)
	{
		replacementTable->setRowCount(replaceMap.count());
		int row = 0;
		QMap<QString,QString>::Iterator it;
		for (it = replaceMap.begin(); it != replaceMap.end(); ++it)
		{
			QTableWidgetItem *tW;
			if (it.key() == CommonStrings::None)
				tW = new QTableWidgetItem(CommonStrings::tr_NoneColor);
			else
				tW = new QTableWidgetItem(getColorIcon(it.key()), it.key());
			tW->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			replacementTable->setItem(row, 0, tW);
			QTableWidgetItem *tW2;
			if (it.value() == CommonStrings::None)
				tW2 = new QTableWidgetItem(CommonStrings::tr_NoneColor);
			else
				tW2 = new QTableWidgetItem(getColorIcon(it.value()), it.value());
			tW2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			replacementTable->setItem(row, 1, tW2);
			row++;
		}
	}
}

QPixmap replaceColorsDialog::getColorIcon(QString color)
{
	QPixmap smallPix(15, 15);
	QPixmap pPixmap(60,15);
	pPixmap.fill(Qt::transparent);
	ScColor m_color = EditColors[color];
	QColor rgb = ScColorEngine::getDisplayColor(m_color, EditColors.document());
	smallPix.fill(rgb);
	QPainter painter(&smallPix);
	painter.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	painter.setPen(b);
	painter.drawRect(0, 0, 15, 15);
	painter.end();
	paintAlert(smallPix, pPixmap, 0, 0);
	bool isOutOfGamut = ScColorEngine::isOutOfGamut(m_color, EditColors.document());
	if (isOutOfGamut)
		paintAlert(alertIcon, pPixmap, 15, 0);
	if (m_color.getColorModel() == colorModelCMYK)
		paintAlert(cmykIcon, pPixmap, 30, 0);
	else
		paintAlert(rgbIcon, pPixmap, 30, 0);
	if (m_color.isSpotColor())
		paintAlert(spotIcon, pPixmap, 45, 0);
	if (m_color.isRegistrationColor())
		paintAlert(regIcon, pPixmap, 46, 0);
	return pPixmap;
}
