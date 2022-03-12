/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          rulermover.h  -  description
                             -------------------
    begin                : Sun Jul 24 2005
    copyright            : (C) 2005 by Franz Schmid
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

#ifndef RULERMOVER_H
#define RULERMOVER_H

#include <QWidget>
#include <QMouseEvent>

#include "scribusapi.h"
class RulerGesture;
class ScribusView;

class SCRIBUS_API RulerMover : public QWidget
{
	Q_OBJECT

public:
	RulerMover(ScribusView *pa);
	~RulerMover() {};

protected:
	void mouseDoubleClickEvent(QMouseEvent*) override;
	void mousePressEvent(QMouseEvent* m) override;
	void mouseReleaseEvent(QMouseEvent* m) override;
	void mouseMoveEvent(QMouseEvent* m) override;

private slots:
	void resetRulers();
	void moveRulerTopRight();
	void moveRulerBottomLeft();
	void moveRulerBottomRight();
	void moveRulerCenter();
	void moveRulerTopCenter();
	void moveRulerBottomCenter();

private: // Private attributes
	bool m_mousePressed { false };
	ScribusView *m_currView { nullptr };
	RulerGesture* m_rulerGesture { nullptr };
};

#endif
