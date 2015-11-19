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



#ifndef CANVAS_MODE_DRAWBEZIER_H
#define CANVAS_MODE_DRAWBEZIER_H

#include "canvasmode.h"
#include "fpointarray.h"

class PageItem;
class ScribusMainWindow;
class ScribusView;
class UndoTransaction;

/**
  Handles the creation of new pageitems
*/
class BezierMode : public CanvasMode
{
public:
	explicit BezierMode(ScribusView* view);
	virtual ~BezierMode() {}

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	
	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void drawControls(QPainter* p);
		
private:
	inline bool GetItem(PageItem** pi); 

	void finalizeItem(PageItem* item);
	void selectPage(QMouseEvent *m);

	bool m_inItemCreation, m_shiftSelItems, m_FirstPoly;
	bool m_MouseButtonPressed;
	double m_Mxp, m_Myp, m_Dxp, m_Dyp;
	double m_SeRx, m_SeRy;
	bool m_MoveGX, m_MoveGY;
	FPointArray m_RecordP;
};


#endif
