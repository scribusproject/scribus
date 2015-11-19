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



#ifndef CANVAS_MODE_CREATE_H
#define CANVAS_MODE_CREATE_H

#include "canvasmode.h"
#include "fpointarray.h"
#include "text/frect.h"
#include "undotransaction.h"

class PageItem;
class ScribusMainWindow;
class ScribusView;

/**
  Handles the creation of new pageitems
*/
class CreateMode : public CanvasMode
{
	Q_OBJECT

public:
	explicit CreateMode(ScribusView* view);
	virtual ~CreateMode() {}

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	
	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void drawControls(QPainter* p);
	
// protected:
// 	void setResizeCursor(int);
	
private:
	inline bool GetItem(PageItem** pi); 
	void selectPage(QMouseEvent *m);
	void SetupDrawNoResize(int nr);

	PageItem* doCreateNewObject(void);
	bool      doOneClick(FPoint& startPoint, FPoint& endPoint);

	// We do not use PageItem::ItemType and PageItem::FrameType here
	// for the pure sake of not including pageitem.h
	void getFrameItemTypes(int& itemType, int& frameType);

	/**
	 * Convenience function which given two points calculates and returns the
	 * normalized rectangle between the two points, adjusted to grid and guides.
	 *
	 * @param firstPoint the first point
	 * @param secondPoint the second point
	 * @return a normalized rectangle adjusted to grid and guides
	 */
	FRect adjustedRect(FPoint &firstPoint, FPoint &secondPoint);

	bool   m_inItemCreation;
	bool   m_MouseButtonPressed;
	FPoint m_mouseGlobalCoord;
	FPoint m_canvasPressCoord;
	FPoint m_canvasCurrCoord;
	FPoint m_createObjectPos;
	int    m_createObjectMode;
	int    m_createObjectSubMode;
	Qt::KeyboardModifiers m_modifiers;
	UndoTransaction m_createTransaction;
};


#endif
