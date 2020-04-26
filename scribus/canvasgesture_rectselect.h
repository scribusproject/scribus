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



#ifndef CANVAS_GESTURE_RECTSELECT_H
#define CANVAS_GESTURE_RECTSELECT_H

#include <QRectF>

#include "scribusapi.h"
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
class SelectionRubberBand;

/**
  This class realizes rectangle-select on behalf of its parent mode.
  The user presses the mousebutton at a start point, drags it to the end point
  and releases the mousebutton. The area between the start and end point is used
  to modify the selection. 
  During the move the selected area is highlighted.
 
  When the parent mode becomes active again, it can retrieve the selection
  rectangle with result().
 */
class SCRIBUS_API RectSelect : public CanvasGesture
{
	public:
		explicit RectSelect (CanvasMode* parent) : CanvasGesture(parent), m_start(0,0), m_selectionRubberBand(nullptr) {};
		~RectSelect() override = default;

		void prepare(QPoint globalStartPos);
		void clear();

		void enterEvent(QEvent * e) override;
		void leaveEvent(QEvent * e) override;
		void activate(bool) override;
		void deactivate(bool) override;
		void mouseReleaseEvent(QMouseEvent *m) override;
		void mouseMoveEvent(QMouseEvent *m) override;
		/**
		Prepares rectangle select for the position from 'm'
	 */
		void mousePressEvent(QMouseEvent *m) override;
		void drawControls(QPainter*) override;

		QRectF result() const ;

	private:
		void setStart(QPoint globalPos);
		void setEnd(QPoint globalPos);

		QPoint m_start;
		SelectionRubberBand* m_selectionRubberBand;
};


#endif
