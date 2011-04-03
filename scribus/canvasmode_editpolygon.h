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
/***************************************************************************
                 canvasmode_editpolygon.h  -  description
                             -------------------
    begin                : Wed Jan 12 2011
    copyright            : (C) 2011 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef CANVAS_MODE_EDITPOLYGON_H
#define CANVAS_MODE_EDITPOLYGON_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"
#include "ui/polyprops.h"

class PageItem;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditPolygon :  public CanvasMode
{
	Q_OBJECT
public:
	CanvasMode_EditPolygon(ScribusView* view);

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	
	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void drawControls(QPainter* p);
	void drawControlsPolygon(QPainter* pp, PageItem* currItem);

private:

	typedef enum 
	{
		noPointDefined,
		useControlOuter,
		useControlInner,
		useControlInnerCurve,
		useControlOuterCurve
	} ePolygonPoint;

	inline bool GetItem(PageItem** pi);
	double GetZeroFactor();
	double GetMaxFactor();
	double getUserValFromFactor(double factor);

	double Mxp, Myp;
	ScribusMainWindow* m_ScMW;
	bool m_cursorVisible;
	bool m_lastPosWasOverGuide;
	QPointF startPoint;
	QPointF endPoint;
	QPointF centerPoint;
	QPointF innerCPoint;
	QPointF outerCPoint;
	int polyCorners;
	bool polyUseFactor;
	double polyFactor;
	double polyRotation;
	double polyCurvature;
	double polyInnerRot;
	double polyOuterCurvature;
	ePolygonPoint m_polygonPoint;
	PolyVectorDialog *VectorDialog;

	int    m_blockUpdateFromItem;
	void   blockUpdateFromItem(bool block) { if (block) ++m_blockUpdateFromItem; else --m_blockUpdateFromItem; }
	bool   updateFromItemBlocked() { return (m_blockUpdateFromItem > 0); }

public slots:
	void applyValues(int polyC, double polyF, bool polyUseCF, double polyR, double polyCur, double polyIRot, double polyOCur);
	void updateFromItem();
	void endEditing(bool active);
	void endEditing();
};


#endif
