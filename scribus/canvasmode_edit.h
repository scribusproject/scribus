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



#ifndef CANVAS_MODE_EDIT_H
#define CANVAS_MODE_EDIT_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"

class PageItem;
class PageItem_TextFrame;
class QTimer;
class ScribusMainWindow;
class ScribusView;

// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_Edit :  public CanvasMode
{
	Q_OBJECT
	
public:
	CanvasMode_Edit(ScribusView* view);

	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	
	virtual void activate(bool);
	virtual void deactivate(bool);
	
	virtual void mouseDoubleClickEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *m);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void mousePressEvent(QMouseEvent *m);
	
	virtual void keyPressEvent(QKeyEvent *e);
	
	virtual void drawControls(QPainter* p);

// protected:
// 	void setResizeCursor(int how, double rot = 0.0);

private:
	inline bool GetItem(PageItem** pi);
	void drawTextCursor(QPainter* p, PageItem_TextFrame* textframe);
	bool SeleItem(QMouseEvent *m);
	void createContextMenu(PageItem *currItem, double mx, double my);

	int  Cp, oldCp;
	int  frameResizeHandle;
	int  dragConstrainInitPtX, dragConstrainInitPtY;
	double Mxp, Myp, Dxp, Dyp;
	double SeRx, SeRy;
	ScribusMainWindow* m_ScMW;
	bool m_cursorVisible;
	QTime m_blinkTime;
	QTimer* m_blinker;
	bool m_lastPosWasOverGuide;
	double m_RulerGuide;
	bool m_longCursorTime;

private slots:
	void blinkTextCursor();
	void rulerPreview(double base, double xp);
};


#endif
