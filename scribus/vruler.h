/***************************************************************************
                          vruler.h  -  description
                             -------------------
    begin                : Wed Apr 11 2001
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

#ifndef VRULER_H
#define VRULER_H

#include <qwidget.h>
#include <qpainter.h>
#include "scribusapi.h"
class ScribusDoc;
class ScribusView;
class PrefsManager;
/**Vertikales Lineal
  *@author Franz Schmid
  */

class ScribusView;

class SCRIBUS_API Vruler : public QWidget  
{
	Q_OBJECT

public: 
	Vruler(ScribusView *pa, ScribusDoc *doc);
	~Vruler() {};
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *m);
	void mouseMoveEvent(QMouseEvent *m);
	void drawNumber(QString num, int starty, QPainter *p);
	int offs;
	bool repainted;
	int oldMark;
	bool Mpressed;
private: // Private attributes
	/** Zeichensatz des Lineals */
	ScribusDoc *currDoc;
	ScribusView *currView;

public slots: // Public slots
  /** Zeichnet den Pfeil */
	void Draw(int wo);
	void unitChange();
	
private:
	double iter, iter2;
	int cor;
	PrefsManager *prefsManager;
};

#endif
