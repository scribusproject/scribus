/***************************************************************************
                          dynamictip.cpp  -  description
                             -------------------
    begin                : Wed Aug 31 2005
    copyright            : (C) 2005 by Franz Schmid
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

#include "dynamictip.h"
#include "seiten.h"

DynamicTip::DynamicTip( QListBox* parent, ColorList* pale ) : QToolTip( parent )
{
	colorList = pale;
	listB = parent;
	kind = 0;
}

DynamicTip::DynamicTip( QTable* parent ) : QToolTip( parent )
{
	table = parent;
	kind = 1;
}

void DynamicTip::maybeTip( const QPoint &pos )
{
	if (kind == 0)
	{
		QListBoxItem* it = listB->itemAt(pos);
		if (it != 0)
		{
			if (!colorList->contains(it->text()))
				return;
			QString tipText = "";
			ScColor col = (*colorList)[it->text()];
			if (col.getColorModel() == colorModelCMYK)
			{
				int c, m, y, k;
				col.getCMYK(&c, &m, &y, &k);
				tipText = QString("C = %1% M = %2% Y = %3% K = %4%").arg(qRound(c / 2.55)).arg(qRound(m / 2.55)).arg(qRound(y / 2.55)).arg(qRound(k / 2.55));
			}
			else
			{
				int r, g, b;
				col.getRawRGBColor(&r, &g, &b);
				tipText = QString("R = %1 G = %2 B = %3").arg(r).arg(g).arg(b);
			}
			tip(listB->itemRect(it), tipText);
		}
	}
	else
	{
		QPoint cp = table->viewportToContents( pos );
		int row = table->rowAt( cp.y() );
		int col = table->columnAt( cp.x() );
		QTableItem* ite = table->item(row, col);
		if (ite == 0)
			return;
		SeItem* it = (SeItem*)ite;
		QString tipString = it->pageName;
		QRect cr = table->cellGeometry( row, col );
		tip( cr, tipString );
	}
}
