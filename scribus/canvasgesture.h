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
		explicit CanvasGesture (CanvasMode* parent) : CanvasMode(parent->view()), m_delegate(parent) {};
		explicit CanvasGesture (ScribusView* view) : CanvasMode(view) {};

	public:
		~CanvasGesture() override = default;
		void enterEvent(QEvent * e) override { m_delegate->enterEvent(e); }
		void leaveEvent(QEvent * e) override { m_delegate->leaveEvent(e); }

		void dragEnterEvent(QDragEnterEvent *e) override { m_delegate->dragEnterEvent(e); }
		void dragMoveEvent(QDragMoveEvent *e) override { m_delegate->dragMoveEvent(e); }
		void dragLeaveEvent(QDragLeaveEvent *e) override { m_delegate->dragLeaveEvent(e); }
		void dropEvent(QDropEvent *e) override { m_delegate->dropEvent(e); }

		void mouseDoubleClickEvent(QMouseEvent *m) override { m_delegate->mouseDoubleClickEvent(m); }
		void mouseReleaseEvent(QMouseEvent *m) override { m_delegate->mouseReleaseEvent(m); }
		void mouseMoveEvent(QMouseEvent *m) override { m_delegate->mouseMoveEvent(m); }
		void mousePressEvent(QMouseEvent *m) override { m_delegate->mousePressEvent(m); }

		void keyPressEvent(QKeyEvent *e) override { m_delegate->keyPressEvent(e); }
		void keyReleaseEvent(QKeyEvent *e) override { m_delegate->keyReleaseEvent(e); }
		void inputMethodEvent(QInputMethodEvent *e) override { m_delegate->inputMethodEvent(e); }

		CanvasMode* delegate() override { return m_delegate; }
		void setDelegate(CanvasMode* delegate) { if (delegate) m_delegate = delegate; }

	protected:
		CanvasMode* m_delegate {nullptr};
};


#endif
