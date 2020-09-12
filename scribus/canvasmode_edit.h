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

#include <QElapsedTimer>
#include <QObject>

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
	explicit CanvasMode_Edit(ScribusView* view);
	~CanvasMode_Edit() override = default;

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	
	void activate(bool) override;
	void deactivate(bool) override;

	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	
	void keyPressEvent(QKeyEvent *e) override;

	void drawControls(QPainter* p) override;

// protected:
// 	void setResizeCursor(int how, double rot = 0.0);

private:
	inline bool GetItem(PageItem** pi);
	void drawTextCursor(QPainter* p, PageItem_TextFrame* textframe);
	bool SeleItem(QMouseEvent *m);
	void createContextMenu(PageItem *currItem, double mx, double my);

	QElapsedTimer m_blinkTime;
	QTimer* m_blinker;
	ScribusMainWindow* m_ScMW {nullptr};
	bool m_cursorVisible {false};
	bool m_keyRepeat {false};
	bool m_longCursorTime {false};
	double Dxp {-1};
	double Dyp {-1};
	double Mxp {-1};
	double Myp {-1};
	double SeRx {-1};
	double SeRy {-1};
	double mRulerGuide {-1.0};
	int Cp {-1};
	int frameResizeHandle {-1};
	int oldCp {-1};

private slots:
	void blinkTextCursor();
	void rulerPreview(double base, double xp);
};


#endif
