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

class PageItem;
class ResizeGesture;
class ScribusMainWindow;
class ScribusView;


/**
  Handles the creation of new pageitems
*/
class CreateMode : public CanvasMode
{
public:
	CreateMode(ScribusView* view);

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	
	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void drawControls(QPainter* p);
	
protected:
	void setResizeCursor(int);
	
private:
	inline bool GetItem(PageItem** pi); 
	void selectPage(QMouseEvent *m);
	void SetupDraw(int Nr);
	void SetupDrawNoResize(int nr);

	int Cp, oldCp;
	bool inItemCreation, shiftSelItems, FirstPoly;
	bool m_MouseButtonPressed;
	int frameResizeHandle;
	int RotMode;
	int dragConstrainInitPtX, dragConstrainInitPtY;
	double Mxp, Myp, Dxp, Dyp;
	int GxM, GyM;
	double SeRx, SeRy;
	bool MoveGX, MoveGY;
	FPointArray RecordP;
	ResizeGesture* resizeGesture;
};


#endif
