/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/



#ifndef CANVAS_MODE_PANNING_H
#define CANVAS_MODE_PANNING_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"

class PanGesture;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_Panning :  public CanvasMode
{
public:
	CanvasMode_Panning(ScribusView* view);

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	
	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void drawControls(QPainter* p);
};


#endif
