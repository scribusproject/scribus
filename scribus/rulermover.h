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

#include <qwidget.h>
class ScribusView;

class RulerMover : public QWidget
{
	Q_OBJECT

public:
	RulerMover(ScribusView *pa);
	~RulerMover() {};
	 void mouseDoubleClickEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *m);
	void mouseMoveEvent(QMouseEvent *m);
	bool Mpressed;

private: // Private attributes
	ScribusView *currView;
};

#endif
