/***************************************************************************
                          fmitem.cpp  -  description
                             -------------------
    begin                : Sun May 13 2001
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

#include "fmitem.h"
#include <qfontmetrics.h>
#include <qapplication.h>

FmItem::FmItem(const QString& s, const QFont& f)
{
	string = s;
	font = f;
	font.setPointSize(qApp->font().pointSize());
}

void FmItem::paint(QPainter *p, const QColorGroup&, bool, bool, int x, int y, int w, int h)
{
	p->setFont(font);
	p->drawText(x, y, w, h, AlignLeft | AlignVCenter | ShowPrefix | DontClip, string);
}

QSize FmItem::sizeHint()
{
	return QFontMetrics(font).size(AlignLeft | AlignVCenter | ShowPrefix | DontClip, string);
}
