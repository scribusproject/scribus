/***************************************************************************
                          pageback.cpp  -  description
                             -------------------
    begin                : Sun Apr 8 2001
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

#include "pageback.h"
#include <qpainter.h>
#include <qcolor.h>

PageBack::PageBack(QWidget *pa) : QWidget(pa, "s", WRepaintNoErase)
{
}

void PageBack::paintEvent(QPaintEvent *)
{
	QPainter p;
	p.begin(this);
	p.setPen(QColor(128, 128, 128));
	p.setBrush(QColor(128, 128, 128));
	p.drawRect(5, 5, width(), height());
	p.end();
}
