/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

/* IMPORTANT! The variable string has been renamed stringy to avoid clashes with the STL
   string class */
 
#include "fmitem.h"
#include <QFontMetrics>
#include <QApplication>

FmItem::FmItem(const QString& s, const ScFace&)
{
	stringy = s;
//	font = f;
//	font.setPointSize(qApp->font().pointSize());
	font = qApp->font();
}

void FmItem::paint(QPainter *p, const QColorGroup&, bool, bool, int x, int y, int w, int h)
{
	p->setFont(font);
	p->drawText(x, y, w, h, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip, stringy);
}

QSize FmItem::sizeHint()
{
	return QFontMetrics(font).size(Qt::AlignLeft | Qt::AlignVCenter | Qt::ShowPrefix | Qt::DontClip, stringy);
}

QString FmItem::Ctext()
{
	return stringy;
}
