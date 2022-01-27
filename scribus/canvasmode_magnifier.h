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



#ifndef CANVAS_MODE_MAGNIFIER_H
#define CANVAS_MODE_MAGNIFIER_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"

class LineMove;
class PageItem;
class PageItem_TextFrame;
class QTimer;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_Magnifier :  public CanvasMode
{
	public:
		explicit CanvasMode_Magnifier(ScribusView* view);
		~CanvasMode_Magnifier() override = default;

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

		// protected:
		// 	void setResizeCursor(int);

	private:
		double m_Mxp {-1.0};
		double m_Myp {-1.0};
		double m_SeRx {0.0};
		double m_SeRy {0.0};
		bool  m_lastPosWasOverGuide {false};
};


#endif
