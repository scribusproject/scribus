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

#include <qsize.h>
#include <qpainter.h>
#include <qfont.h>
#include <qstring.h>
#include <qmenudata.h>
/**
  *@author Franz Schmid
  */

class FmItem : public QCustomMenuItem {
public: 
	FmItem(const QString& s, const QFont& f);
	~FmItem() {};
	void paint(QPainter *p, const QColorGroup& cg, bool act, bool enabled, int x, int y, int w, int h);
	QSize sizeHint();
private:
	QString string;
	QFont font;
};

#endif
