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
	~BezierMode() override = default;

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	
	void activate(bool) override;
	void deactivate(bool) override;
	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void drawControls(QPainter* p) override;
		
private:
	inline bool GetItem(PageItem** pi); 

	void finalizeItem(PageItem* item);
	void selectPage(QMouseEvent *m);

	bool m_inItemCreation;
	bool m_firstPoly;
	bool m_mouseButtonPressed;
	double m_xp, m_yp;
};


#endif
