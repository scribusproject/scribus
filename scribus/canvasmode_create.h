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
	~CreateMode() override  = default;

	void enterEvent(QEvent *e) override;
	void leaveEvent(QEvent *e) override;
	
	void activate(bool) override;
	void deactivate(bool) override;
	void keyPressEvent(QKeyEvent *e) override;
	void keyReleaseEvent(QKeyEvent *e) override;
	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void drawControls(QPainter* p) override;
	
// protected:
// 	void setResizeCursor(int);
	
private:
	inline bool GetItem(PageItem** pi); 
	void selectPage(QMouseEvent *m);
	void SetupDrawNoResize(int nr);

	PageItem* doCreateNewObject();
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

	FPoint canvasCurrCoord;
	FPoint canvasPressCoord;
	FPoint createObjectPos;
	FPoint mouseGlobalCoord;
	Qt::KeyboardModifiers modifiers {Qt::NoModifier};
	UndoTransaction m_createTransaction {nullptr};
	bool inItemCreation {false};
	bool m_MouseButtonPressed {false};
	int createObjectMode {0};
	int createObjectSubMode {0};
};


#endif
