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



#ifndef CANVAS_MODE_DRAWFREEHAND_H
#define CANVAS_MODE_DRAWFREEHAND_H

#include "canvasmode.h"
#include "fpointarray.h"

class PageItem;
class ScribusMainWindow;
class ScribusView;


/**
  Handles the creation of new pageitems
*/
class FreehandMode : public CanvasMode
{
	public:
		explicit FreehandMode(ScribusView* view);
		~FreehandMode() override = default;

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

	private:
		inline bool GetItem(PageItem** pi);
		void selectPage(QMouseEvent *m);

		FPointArray m_poly;
		bool m_mouseButtonPressed {false};
		double m_xp {-1.0};
		double m_yp {-1.0};
};


#endif
