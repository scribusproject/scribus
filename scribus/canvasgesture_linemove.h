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

#include <QRectF>

#include "scribusapi.h"
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
class PageItem_Line;

/**
  This class realizes moving the endpoints of a line on behalf of its parent mode.
  The user presses the mousebutton at one endpoint of the line, drags it to a new position
  and releases the mousebutton.
 
  When the parent mode becomes active again, it can retrieve the new line with endPoint().
 
  If the constructur with a pageitem is used, the gesture will change the item directly.
 */
class SCRIBUS_API LineMove : public CanvasGesture
{
public:
	explicit LineMove (CanvasMode* parent): CanvasGesture(parent), m_bounds(0,0,0,0) {}
	~LineMove() override = default;
	
	/**
		Prepare a LineMove without attached PageItem
	 */
	void prepare(QPointF start, QPointF end);
	
	/** Prepare a LineMove gesture for a PageItem_Line. 
	    useOriginAsEndpoint allows to drag the origin (topleft) in the same way as the other end.
	*/
	void prepare(PageItem_Line* line, bool useOriginAsEndpoint = false);
	
	void clear();
	
	void activate(bool) override;
	void deactivate(bool) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	/**
		prepares the LineMove for the the current selection. Sets 'haveLineItem'
		to false if the current selection is not a single lineitem.
	 */
	void mousePressEvent(QMouseEvent *m) override;
	void drawControls(QPainter*) override;
	
	void setStartPoint(QPointF p);
	void setEndPoint(QPointF p);

	QPointF startPoint() const { return m_bounds.topLeft(); }
	QPointF endPoint() const { return m_bounds.bottomRight(); }

	bool haveLineItem() const { return m_haveLineItem; }
	
private:
	bool      m_haveLineItem {false};
	bool      m_useOriginAsEndpoint {false};
	QRectF    m_bounds;
	QRectF    m_initialBounds;
	PageItem* m_line {nullptr};
	UndoTransaction m_transaction;

	void adjustBounds(QMouseEvent* m, bool updateCanvas = true);
	void doResize();
	void setRotation(double rot);
	double rotation() const;
	double length() const;
};


#endif
