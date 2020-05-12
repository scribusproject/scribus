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

#ifndef CANVAS_GESTURE_PAN_H
#define CANVAS_GESTURE_PAN_H

#include "scribusapi.h"
#include "canvasgesture.h"
#include "canvasmode.h"

#include <QCursor>
#include <QPointF>

class SCRIBUS_API PanGesture : public CanvasGesture
{
	public:
		explicit PanGesture (CanvasMode* parent);
		~PanGesture() override = default;

		void activate(bool) override;
		void deactivate(bool) override;
		void mouseReleaseEvent(QMouseEvent *m) override;
		void mouseMoveEvent(QMouseEvent *m) override;

		void mousePressEvent(QMouseEvent *m) override;
		void drawControls(QPainter*) override;

	private:
		QCursor m_cursor;
		QPointF m_canvasRefPoint;
};

#endif
