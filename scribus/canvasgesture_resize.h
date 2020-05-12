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
#include "undotransaction.h"

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
class PageItem;

/**
  This class realizes resizing of selected items on behalf of its parent mode.
  The user presses the mousebutton at one of the framehandles, drags it to the end point
  and releases the mousebutton. The item or group selection is resized to the new dimensions.
  During the move the selected items are highlighted.
 */
class SCRIBUS_API ResizeGesture : public CanvasGesture
{
	public:
		explicit ResizeGesture (CanvasMode* parent);
		~ResizeGesture() override = default;

		/**
		Prepares the gesture for resizing the selection
	 */
		void prepare(Canvas::FrameHandle framehandle = Canvas::SOUTHEAST);
		void clear();

		void drawControls(QPainter* p) override;
		void activate(bool) override;
		void deactivate(bool) override;
		void mouseReleaseEvent(QMouseEvent *m) override;
		void mouseMoveEvent(QMouseEvent *m) override;
		/**
	  This method only sets the m_handle field.
	  If the correct value is set by prepare() (default = SOUTHEAST),
	  it's not necessary to call this method.
	 */
		void mousePressEvent(QMouseEvent *m) override;

		Canvas::FrameHandle frameHandle() const { return m_handle; }

	private:
		void adjustBounds(QMouseEvent *m);
		void doResize(bool scaleContent);

		FPoint applyGrid(const FPoint& docPoint);
		FPoint applyGuides(const FPoint& docPoint);

		Canvas::FrameHandle m_handle {Canvas::INSIDE};
		QPoint m_mousePressPoint;
		QRectF m_bounds;
		QRectF m_mousePressBounds;
		QRectF m_origBounds;
		UndoTransaction m_transaction;
		double m_extraHeight {0.0};
		double m_extraWidth {0.0};
		double m_extraX {0.0};
		double m_extraY {0.0};
		double m_origRatio {1.0};
		double m_rotation {0.0};
};


#endif
