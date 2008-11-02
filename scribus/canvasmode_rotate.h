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

#ifndef CANVAS_MODE_ROTATE_H
#define CANVAS_MODE_ROTATE_H

#include "scribusapi.h"
#include "canvasmode.h"
#include "fpoint.h"

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QEvent;
class QInputMethodEvent;
class QMouseEvent;
class QKeyEvent;
class QPainter;

class PageItem;

class SCRIBUS_API CanvasMode_Rotate : public CanvasMode
{
public:
	CanvasMode_Rotate(ScribusView* view);

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);

	virtual void drawControls(QPainter* p);
	virtual void activate(bool);
	virtual void deactivate(bool);

	virtual void mousePressEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);

private:
	inline bool GetItem(PageItem** pi);
	
	void   createContextMenu(PageItem *currItem, double mx, double my);
	void   drawItemOutlines(QPainter*);
	void   getNewItemPosition(PageItem* item, FPoint& pos, double& rotation);
// 	void   setResizeCursor(int how);

	bool   m_inItemRotation;

	int    m_oldRotMode;
	FPoint m_oldRotCenter;
	FPoint m_canvasPressCoord;
	FPoint m_canvasCurrCoord;
	bool   m_angleConstrained;

	int    m_rotMode;
	FPoint m_rotCenter;
	double m_startAngle;
};

#endif
