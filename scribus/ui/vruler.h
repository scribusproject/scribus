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

class QMouseEvent;
class QPaintEvent;

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
	~Vruler() {}
	
	double ruleSpacing() const;
	void shift(double pos) { m_offset = pos; }
	void shiftRel(double dist) { m_offset += dist; }

public slots: // Public slots
	/** \brief draw mark
	\param where where to draw */
	void draw(int where);
	void unitChange();

private:
	void paintEvent(QPaintEvent *e) override;
	void mousePressEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;

	void drawNumber(const QString& num, int starty, QPainter *p) const;

	RulerGesture* rulerGesture {nullptr};
	ScribusDoc* m_doc {nullptr};
	ScribusView* m_view {nullptr};
	bool m_drawMark {false};
	bool m_mousePressed {false};
	double m_cor {0.0};
	double m_iter {0.0};
	double m_iter2 {0.0};
	double m_offset {0.0};
	int m_oldMark {0};
	int m_whereToDraw {0};
};

#endif
