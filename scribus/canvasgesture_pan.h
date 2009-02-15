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

#ifndef CANVAS_GESTURE_LINEMOVE_H
#define CANVAS_GESTURE_LINEMOVE_H

#include "scribusapi.h"
#include "canvasgesture.h"
#include "canvasmode.h"

#include <QCursor>
#include <QPointF>

class SCRIBUS_API PanGesture : public CanvasGesture
{
public:
	PanGesture (CanvasMode* parent);
	~PanGesture() {}
	
	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);

	virtual void mousePressEvent(QMouseEvent *m);
	virtual void drawControls(QPainter*);
	
private:
	bool    m_haveCursor;
	QCursor m_cursor;
	QPointF m_canvasRefPoint;
};

#endif
