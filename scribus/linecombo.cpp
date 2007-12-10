/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          linecombo.cpp  -  description
                             -------------------
    begin                : Thu Jul 12 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include "linecombo.h"
#include "util.h"

/*!
 \fn LineCombo::LineCombo(QWidget* pa)
 \author Franz Schmid
 \date
 \brief Creates Combobox and inserts line type variations
 \param pa Parent Window
 \retval None
 */

LineCombo::LineCombo(QWidget* pa) : QComboBox(pa)
{
	setEditable(false);
	setIconSize(QSize(73, 7));
	updateList();
}

void LineCombo::updateList()
{
	for (int a = 0; a < 37; a++)
	{
		addItem(QIcon(createIcon(a)), "");
	}
}

QPixmap LineCombo::createIcon(int type)
{
	QPen pen;
	QList<double> m_array;
	QPixmap pmap(73, 7);
	pmap.fill(Qt::transparent);
	QPainter p;
	p.begin(&pmap);
	if (type == 0)
		pen = QPen(Qt::black, 3, Qt::SolidLine);
	else
	{
		getDashArray(type + 1, 1, m_array);
		pen.setDashPattern(m_array.toVector());
	}
	pen.setColor(Qt::black);
	pen.setWidth(3);
	pen.setCapStyle(Qt::FlatCap);
	p.setPen(pen);
	p.drawLine(2, 3, 73, 3);
	p.end();
	return pmap;
}

