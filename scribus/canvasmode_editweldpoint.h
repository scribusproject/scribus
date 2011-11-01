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
				 canvasmode_editweldpoint.h  -  description
							 -------------------
	begin                : Sun Okt 23 2011
	copyright            : (C) 2011 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef CANVASMODE_EDITWELDPOINT_H
#define CANVASMODE_EDITWELDPOINT_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"
#include "ui/weldedit.h"

class PageItem;
class PageItem_TextFrame;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditWeldPoint :  public CanvasMode
{
	Q_OBJECT
public:
	explicit CanvasMode_EditWeldPoint(ScribusView* view);
	virtual ~CanvasMode_EditWeldPoint() {}

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
	void drawControlsWeldPoint(QPainter* pp, PageItem* currItem);

private:
	void snapToEdgePoints(double &x, double &y);
	QRectF getUpdateRect();

	inline bool GetItem(PageItem** pi);

	double Mxp, Myp;
	int selectedPoint;
	ScribusMainWindow* m_ScMW;
	bool m_keyRepeat;
	PageItem *currItem;
	QList<PageItem*> weldToList;
	WeldEditDialog* ModeDialog;
	bool editWeldMode;
private slots:
	void endEditing(bool active);
	void endEditing();
	void setWeldMode();
	void setObjectMode();
};

#endif
