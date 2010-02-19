/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          navigator.h  -  description
                             -------------------
    begin                : Fri Feb 22 2002
    copyright            : (C) 2002 by Franz Schmid
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

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QLabel>

class QPaintEvent;
class QMouseEvent;
class QPixmap;

#include "scribusapi.h"
class ScribusView;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API Navigator : public QLabel
{
	Q_OBJECT

public: 
	Navigator(QWidget *parent, int Size, int Seite, ScribusView* vie, QString fn = "");
	~Navigator() {};
	void mouseMoveEvent(QMouseEvent *m);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *m);
	void paintEvent(QPaintEvent *e);
	void drawMark(int x, int y);
	bool SetPage(int Seite, int Size, QString fn = "");
	int Xp;
	int Yp;
	int Width;
	int Height;
	ScribusView* view;
	QPixmap pmx;
	
signals:
	void Coords(double x, double y);
};

#endif
