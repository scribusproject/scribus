/***************************************************************************
 *   Copyright (C) 2005 by Riku Leino                                      *
 *   riku@scribus.info                                                     *
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

#include "smreplacedia.h"
#include "smreplacedia.moc"

#include <qtable.h>
#include <qheader.h>

SMReplaceDia::SMReplaceDia(const QStringList &toBeDeleted, const QStringList &replaceOptions) : SMReplaceDiaBase()
{
	table->verticalHeader()->hide();
	table->setLeftMargin(0);
	table->setColumnReadOnly(0, true);
	table->setNumRows(toBeDeleted.count());

	QStringList options;
	for (uint i = 0; i < replaceOptions.count(); ++i)
	{
		if (!toBeDeleted.contains(replaceOptions[i]))
			options << replaceOptions[i];
	}
	
	for (uint i = 0; i < toBeDeleted.count(); ++i)
	{
		table->setText(i, 0, toBeDeleted[i]);
		table->setItem(i, 1, new QComboTableItem(table, options));
	}
}

QValueList<RemoveItem> SMReplaceDia::items()
{
	QValueList<RemoveItem> tmp;
	for (int i = 0; i < table->numRows(); ++i)
	{
		QString s1 = table->text(i, 1);
		QComboTableItem *qcti = dynamic_cast<QComboTableItem*>(table->item(i,1));
		QString s2 = qcti->text(qcti->currentItem());
		tmp.append(RemoveItem(s1, s2));
		
	}
	return tmp;
}

SMReplaceDia::~SMReplaceDia()
{
	for (int i = 0; i < table->numRows(); ++i)
		delete table->item(i, 1);
}
