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
	replacementTable->horizontalHeader()->setMovable(false);
	replacementTable->horizontalHeader()->setClickable(false);
	replacementTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	replacementTable->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Original")));
	replacementTable->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Replacement")));
	replacementTable->verticalHeader()->setMovable(false);
	replacementTable->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	replacementTable->verticalHeader()->hide();
	updateReplacementTable();
	removeButton->setEnabled(false);
	connect(addButton, SIGNAL(clicked()), this, SLOT(addColor()));
	connect(replacementTable, SIGNAL(cellClicked(int, int)), this, SLOT(selReplacement(int)));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(delReplacement()));
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
		if (replaceMap.values().contains(orig))
		{
			QMap<QString,QString>::Iterator it;
			for (it = replaceMap.begin(); it != replaceMap.end(); ++it)
			{
				if (it.value() == orig)
					it.value() = repl;
			}
		}
		replaceMap.insert(orig, repl);
		updateReplacementTable();
	}
	delete dia;
}

void replaceColorsDialog::selReplacement(int sel)
{
	selectedRow = sel;
	removeButton->setEnabled(true);
}

void replaceColorsDialog::delReplacement()
{
	if (selectedRow > -1)
	{
		replaceMap.remove(replacementTable->item(selectedRow, 0)->text());
		replacementTable->removeRow(selectedRow);
		selectedRow = -1;
	}
}

void replaceColorsDialog::updateReplacementTable()
{
	replacementTable->clearContents();
	replacementTable->setRowCount(0);
	selectedRow = -1;
	removeButton->setEnabled(false);
	if (replaceMap.count() > 0)
	{
		replacementTable->setRowCount(replaceMap.count());
		int row = 0;
		QMap<QString,QString>::Iterator it;
		for (it = replaceMap.begin(); it != replaceMap.end(); ++it)
		{
			QTableWidgetItem *tW = new QTableWidgetItem(it.key());
			tW->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			replacementTable->setItem(row, 0, tW);
			QTableWidgetItem *tW2 = new QTableWidgetItem(it.value());
			tW->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			replacementTable->setItem(row, 1, tW2);
			row++;
		}
	}
}
