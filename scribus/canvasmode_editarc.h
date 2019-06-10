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
                 canvasmode_editarc.h  -  description
                             -------------------
    begin                : Tue Jan 4 2011
    copyright            : (C) 2011 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef CANVAS_MODE_EDITARC_H
#define CANVAS_MODE_EDITARC_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"
#include "ui/arcvectordialog.h"

class PageItem;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditArc :  public CanvasMode
{	
	Q_OBJECT

public:
	explicit CanvasMode_EditArc(ScribusView* view);
	~CanvasMode_EditArc() override = default;

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	
	void activate(bool) override;
	void deactivate(bool) override;
	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void drawControls(QPainter* p) override;
	void drawControlsArc(QPainter* psx, PageItem* currItem);

private:

	typedef enum 
	{
		noPointDefined,
		useControlStart,
		useControlSweep,
		useControlHeight,
		useControlWidth
	} eArcPoint;

	inline bool GetItem(PageItem** pi);

	double m_Mxp, m_Myp;
	ScribusMainWindow* m_ScMW;
	QPointF m_startPoint;
	QPointF m_endPoint;
	QPointF m_centerPoint;
	QPointF m_widthPoint;
	QPointF m_heightPoint;
	double m_startAngle;
	double m_endAngle;
	eArcPoint m_arcPoint;
	ArcVectorDialog* vectorDialog;

	int    m_blockUpdateFromItem;
	void   blockUpdateFromItem(bool block) { if (block) ++m_blockUpdateFromItem; else --m_blockUpdateFromItem; }
	bool   updateFromItemBlocked() { return (m_blockUpdateFromItem > 0); }

public slots:
	void updateFromItem();

private slots:
	void endEditing(bool active);
	void endEditing();
	void applyValues(double start, double end, double height, double width);
};


#endif
