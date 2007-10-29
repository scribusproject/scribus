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
	addItem(QIcon(createIcon(0)), "");
	addItem(QIcon(createIcon(1)), "");
	addItem(QIcon(createIcon(2)), "");
	addItem(QIcon(createIcon(3)), "");
	addItem(QIcon(createIcon(4)), "");
//	addItem(QIcon(createIcon(5)), "");
}

QPixmap LineCombo::createIcon(int type)
{
//	QPen pen;
//	QVector<qreal> dashes;
	QPixmap pmap(73, 7);
	pmap.fill(Qt::transparent);
	QPainter p;
	p.begin(&pmap);
	switch (type)
	{
		case 0:
			p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
			break;
		case 1:
			p.setPen(QPen(Qt::black, 3, Qt::DashLine));
			break;
		case 2:
			p.setPen(QPen(Qt::black, 3, Qt::DotLine));
			break;
		case 3:
			p.setPen(QPen(Qt::black, 3, Qt::DashDotLine));
			break;
		case 4:
			p.setPen(QPen(Qt::black, 3, Qt::DashDotDotLine));
			break;
/*		case 5:
			dashes << 4 << 2 << 1 << 2;
			pen.setDashPattern(dashes);
			pen.setColor(Qt::black);
			pen.setWidth(3);
			p.setPen(pen);
			break; */
	}
	p.drawLine(2, 3, 73, 3);
	p.end();
	return pmap;
}

