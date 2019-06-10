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



#ifndef CANVAS_MODE_NODEEDIT_H
#define CANVAS_MODE_NODEEDIT_H

#include "canvasmode.h"

#include <QTransform>
#include <QRect>

class QPainter;

class PageItem;
class RectSelect;
class ScribusMainWindow;

class SCRIBUS_API CanvasMode_NodeEdit : public CanvasMode
{
public:
	explicit CanvasMode_NodeEdit (ScribusView* view);
	
	void activate(bool fromgesture) override;
	void deactivate(bool forGesture) override;
	
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	
	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;

	void keyPressEvent(QKeyEvent *e) override;
	void keyReleaseEvent(QKeyEvent *e) override;

	bool handleKeyEvents() override { return true; }

	
	/**
		Draws the controls for this mode
	 */
	void drawControls(QPainter* p) override;
	
	
	~CanvasMode_NodeEdit() override = default;
	
	
private:
	inline bool GetItem(PageItem** pi); 
	void handleNodeEditPress(QMouseEvent*, QRect);
	void handleNodeEditDrag(QMouseEvent*, PageItem*);
	bool handleNodeEditMove(QMouseEvent*, QRect, PageItem*, const QTransform&);
	
	ScribusMainWindow* m_ScMW;
	
	RectSelect* m_rectangleSelect;
	int m_Mxp, m_Myp, m_Dxp, m_Dyp, m_GxM, m_GyM;
	bool m_MoveGX, m_MoveGY;
};


#endif
