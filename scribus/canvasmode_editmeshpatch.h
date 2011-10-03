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
				 canvasmode_editmeshpatch.h  -  description
							 -------------------
	begin                : Sun Sep 25 2011
	copyright            : (C) 2011 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef CANVASMODE_EDITMESHPATCH_H
#define CANVASMODE_EDITMESHPATCH_H

#include <QObject>
#include <QTime>
#include <QPolygonF>

#include "canvasmode.h"
#include "fpointarray.h"

class meshPoint;
class PageItem;
class PageItem_TextFrame;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditMeshPatch :  public CanvasMode
{
public:
	explicit CanvasMode_EditMeshPatch(ScribusView* view);
	virtual ~CanvasMode_EditMeshPatch() {}

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);

	virtual void activate(bool);
	virtual void deactivate(bool);
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void keyPressEvent(QKeyEvent *e);
	virtual bool handleKeyEvents() { return true; }
	virtual void drawControls(QPainter* p);
	void drawControlsMeshPoint(QPainter* psx, meshPoint mp, bool isSelected);
	void drawControlsMeshPatch(QPainter* pp, PageItem* currItem);

private:

	typedef enum
	{
		noPointDefined,
		useTL,
		useTR,
		useBR,
		useBL
	} eMPatchPoint;

	typedef enum
	{
		noControlPointDefined,
		useControlT,
		useControlB,
		useControlL,
		useControlR,
		useControlC
	} eMGradientPoint;

	inline bool GetItem(PageItem** pi);

	double Mxp, Myp;
	ScribusMainWindow* m_ScMW;
	eMPatchPoint m_patchPoint;
	eMGradientPoint m_gradientPoint;
	bool m_keyRepeat;
	int m_click_count;
	QPolygonF m_clickPointPolygon;
	QPointF m_currentPoint;
	PageItem *currItem;
};

#endif
