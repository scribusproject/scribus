/***************************************************************************
                          hruler.h  -  description
                             -------------------
    begin                : Tue Apr 10 2001
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

#ifndef HRULER_H
#define HRULER_H

#include <qwidget.h>
#include <qpainter.h>
#include <qscrollview.h>
#include "scribusdoc.h"

/**Waagrechtes Lineal
  *@author Franz Schmid
  */

class Hruler : public QWidget  
{
	Q_OBJECT

public:
	Hruler(QScrollView *pa, ScribusDoc *doc);
	~Hruler() {};
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *m);
	void UpdateTabList();
	int offs;
	int Markp;
	bool repX;
	bool Mpressed;
	QValueList<double> TabValues;
	double ItemPos;
	double ItemEndPos;
	int Cols;
	int ActCol;
	int ActTab;
	double ColGap;
	double RExtra;
	double Extra;
	double lineCorr;
	double Indent;
	double First;
	double Offset;
	double Scaling;
	int RulerCode;
	int MouseX;
	bool Revers;
	bool ItemPosValid;

private: // Private attributes
  /** Zeichensatz des Lineals */
  ScribusDoc *doku;

public slots: // Public slots
  /** Zeichnet den Pfeil */
  void Draw(int wo);
};

#endif
