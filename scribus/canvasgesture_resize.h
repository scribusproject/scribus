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



#ifndef CANVAS_GESTURE_RESIZE_H
#define CANVAS_GESTURE_RESIZE_H

#include <QRect>

#include "scribusapi.h"
#include "canvas.h"
#include "canvasgesture.h"
#include "canvasmode.h"

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QEvent;
class QInputMethodEvent;
class QMouseEvent;
class QKeyEvent;
class QPainter;
class QRubberBand;

/**
  This class realizes resizing of selected items on behalf of its parent mode.
  The user presses the mousebutton at one of the framehandles, drags it to the end point
  and releases the mousebutton. The item or group selection is resized to the new dimensions.
  During the move the selected items are highlighted.
 */
class SCRIBUS_API ResizeGesture : public CanvasGesture
{
public:
	ResizeGesture (CanvasMode* parent, Canvas::FrameHandle framehandle=Canvas::SOUTHEAST) : CanvasGesture(parent), m_handle(framehandle) {};
	

	virtual void drawControls(QPainter* p);
	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	/**
	  This method only sets the m_handle field.
	  If the correct value is set by the constructor (default = SOUTHEAST), 
	  it's not necessary to call this method.
	 */
	virtual void mousePressEvent(QMouseEvent *m);
	
	Canvas::FrameHandle frameHandle() const { return m_handle; }
	
private:
	void adjustBounds(QMouseEvent *m);
	void doResize(bool scaleTextInGroup);
	Canvas::FrameHandle m_handle;
	double m_rotation;
	double m_origRatio;
	QRectF m_bounds;
	QRectF m_origBounds;
};


#endif
