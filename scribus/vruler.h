/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QWidget>
#include "scribusapi.h"

class QPaintEvent;
class QMouseEvent;
class PrefsManager;
class RulerGesture;
class ScribusDoc;
class ScribusView;

/** \brief Vertical ruler
\author Franz Schmid
*/
class SCRIBUS_API Vruler : public QWidget
{
	Q_OBJECT

public:
	Vruler(ScribusView *pa, ScribusDoc *doc);
	~Vruler() {};
	
	double offs;
	bool repainted;
	int oldMark;
	bool Mpressed;
	double ruleSpacing();

private: // Private attributes
	virtual void paintEvent(QPaintEvent *e);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);

	void drawNumber(QString num, int starty, QPainter *p);

	ScribusDoc *currDoc;
	ScribusView *currView;

public slots: // Public slots
	/** \brief draw mark
	\param where where to draw */
	void Draw(int where);
	void unitChange();
	void shift(double pos) { offs = pos; }
	void shiftRel(double dist) { offs += dist; }

private:
	double iter, iter2;
	double cor;
	bool drawMark;
	int whereToDraw;
	PrefsManager *prefsManager;
	RulerGesture* rulerGesture;
};

#endif
