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
                 canvasmode_editspiral.h  -  description
                             -------------------
    begin                : Wed Jan 27 2011
    copyright            : (C) 2011 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef CANVAS_MODE_EDITSPIRAL_H
#define CANVAS_MODE_EDITSPIRAL_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"
#include "ui/spiralvectordialog.h"
#include "undotransaction.h"

class PageItem;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditSpiral :  public CanvasMode
{	
	Q_OBJECT
public:
	explicit CanvasMode_EditSpiral(ScribusView* view);
	~CanvasMode_EditSpiral() override = default;

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	
	void activate(bool) override;
	void deactivate(bool) override;
	void keyPressEvent(QKeyEvent *e) override;
	void keyReleaseEvent(QKeyEvent *e) override;
	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void drawControls(QPainter* p) override;
	void drawControlsSpiral(QPainter* psx, PageItem* currItem);

private:

	typedef enum 
	{
		noPointDefined,
		useControlStart,
		useControlEnd,
	} eSpiralPoint;

	inline bool GetItem(PageItem** pi);
	QPointF getSegment(double angle);
	double computeRealAngle(double angle, bool fromDia);

	QPointF m_endPoint;
	QPointF m_startPoint;
	ScribusMainWindow* m_ScMW {nullptr};
	SpiralVectorDialog* m_VectorDialog {nullptr};
	UndoTransaction m_transaction;
	double m_Mxp {-1.0};
	double m_Myp {-1.0};
	double m_endAngle {0.0};
	double m_startAngle {0.0};
	eSpiralPoint m_arcPoint {noPointDefined};
	int m_blockUpdateFromItem {0};

	void blockUpdateFromItem(bool block) { if (block) ++m_blockUpdateFromItem; else --m_blockUpdateFromItem; }
	bool updateFromItemBlocked() { return (m_blockUpdateFromItem > 0); }

public slots:
	void updateFromItem();

private slots:
	void endEditing(bool active);
	void endEditing();
	void applyValues(double start, double end, double factor);
};


#endif
