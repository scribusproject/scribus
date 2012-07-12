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



#ifndef CANVAS_MODE_EDITGRADIENT_H
#define CANVAS_MODE_EDITGRADIENT_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"

class PageItem;
class PageItem_TextFrame;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditGradient :  public CanvasMode
{	
public:
	explicit CanvasMode_EditGradient(ScribusView* view);
	virtual ~CanvasMode_EditGradient() {}

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	
	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void drawControls(QPainter* p);
	void drawControlsGradientVectors(QPainter* psx, PageItem *currItem);

private:

	typedef enum 
	{
		noPointDefined,
		useGradientStart,
		useGradientEnd,
		useGradientFocal,
		useGradientSkew,
		useControl1,
		useControl2,
		useControl3,
		useControl4,
		useControl5
	} eGradientPoint;

	inline bool GetItem(PageItem** pi);

	double Mxp, Myp;
	ScribusMainWindow* m_ScMW;
	eGradientPoint m_gradientPoint;

	//save of previous state
	double OldGrStartX;
	double OldGrStartY;
	double OldGrEndX;
	double OldGrEndY;
	double OldGrFocalX;
	double OldGrFocalY;
	double OldGrScale;
	double OldGrSkew;
	FPoint OldGrControl1;
	FPoint OldGrControl2;
	FPoint OldGrControl3;
	FPoint OldGrControl4;
	FPoint OldGrControl5;
	double OldGrStrokeStartX;
	double OldGrStrokeStartY;
	double OldGrStrokeEndX;
	double OldGrStrokeEndY;
	double OldGrStrokeFocalX;
	double OldGrStrokeFocalY;
	double OldGrStrokeScale;
	double OldGrStrokeSkew;
	double OldGrMaskStartX;
	double OldGrMaskStartY;
	double OldGrMaskEndX;
	double OldGrMaskEndY;
	double OldGrMaskFocalX;
	double OldGrMaskFocalY;
	double OldGrMaskScale;
	double OldGrMaskSkew;
};


#endif
