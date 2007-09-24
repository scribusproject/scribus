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



#ifndef CANVAS_GESTURE_H
#define CANVAS_GESTURE_H

#include "scribusapi.h"
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

class  Canvas;
struct CanvasViewMode;
class  ScribusDoc;
class  ScribusView;



/**
  This class is a superclass for canvas gestures. CanvasGestures are
  temporary canvasmodes which have a pointer to the CanvasMode in which they
  started. ScribusView::stopGesture() will return control to that mode.
  By default, all events are delegated to the canvas mode.
 */
class SCRIBUS_API CanvasGesture : public CanvasMode
{
protected:
	CanvasGesture (CanvasMode* parent) : CanvasMode(parent->view()), m_delegate(parent) {};
	
public:	
	virtual void enterEvent(QEvent * e) { m_delegate->enterEvent(e); }
	virtual void leaveEvent(QEvent * e) { m_delegate->leaveEvent(e); }

	virtual void dragEnterEvent(QDragEnterEvent *e) { m_delegate->dragEnterEvent(e); }
	virtual void dragMoveEvent(QDragMoveEvent *e) { m_delegate->dragMoveEvent(e); }
	virtual void dragLeaveEvent(QDragLeaveEvent *e) { m_delegate->dragLeaveEvent(e); }
	virtual void dropEvent(QDropEvent *e) { m_delegate->dropEvent(e); }
	
	virtual void mouseDoubleClickEvent(QMouseEvent *m) { m_delegate->mouseDoubleClickEvent(m); }
	virtual void mouseReleaseEvent(QMouseEvent *m) { m_delegate->mouseReleaseEvent(m); }
	virtual void mouseMoveEvent(QMouseEvent *m) { m_delegate->mouseMoveEvent(m); }
	virtual void mousePressEvent(QMouseEvent *m) { m_delegate->mousePressEvent(m); }

	virtual void keyPressEvent(QKeyEvent *e) { m_delegate->keyPressEvent(e); }
	virtual void keyReleaseEvent(QKeyEvent *e) { m_delegate->keyReleaseEvent(e); }
	virtual void inputMethodEvent(QInputMethodEvent *e) { m_delegate->inputMethodEvent(e); }
	
	CanvasMode* delegate() { return m_delegate; }
	void setDelegate(CanvasMode* delegate) { if (delegate) m_delegate = delegate; }
	
protected:
	CanvasMode* m_delegate;
};


#endif
