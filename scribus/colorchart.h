/***************************************************************************
                          colorchart.h  -  description
                             -------------------
    begin                : Sat Sep 15 2001
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

#ifndef COLORCHART_H
#define COLORCHART_H

#include <qlabel.h>
#include <qpixmap.h>
#include "scribusapi.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API ColorChart : public QLabel  {

Q_OBJECT

public: 
	ColorChart(QWidget *parent);
	~ColorChart() {};
	void mouseMoveEvent(QMouseEvent *m);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *m);
	void paintEvent(QPaintEvent *e);
	void drawMark(int x, int y);
	void setMark(int h, int s);
	void drawPalette(int val);
	int Xp;
	int Yp;
	QPixmap pmx;

signals:
	void ColorVal(int h, int s, bool ende);
};

#endif
