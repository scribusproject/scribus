/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          fmitem.h  -  description
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

#ifndef FMITEM_H
#define FMITEM_H

#include <QSize>
#include <QPainter>
#include <QFont>
#include <QString>
#include <QMenuData>

#include "scfonts.h"
#include "scribusapi.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API FmItem : public QCustomMenuItem 
{
public: 
	FmItem(const QString& s, const ScFace& f);
	~FmItem() {};
	void paint(QPainter *p, const QColorGroup& cg, bool act, bool enabled, int x, int y, int w, int h);
	QSize sizeHint();
	QString Ctext();

private:
	QString stringy;
	QFont font;
};

#endif
