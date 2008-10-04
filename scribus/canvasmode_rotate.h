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

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QEvent;
class QInputMethodEvent;
class QMouseEvent;
class QKeyEvent;
class QPainter;

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
	void   setResizeCursor(int how);
	void   createContextMenu(PageItem *currItem, double mx, double my);

	int    RotMode;
	double Mxp, Myp, Dxp, Dyp;;
	double SeRx, SeRy;
};

#endif
