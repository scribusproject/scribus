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



#ifndef CANVAS_MODE_MEASUREMENTS_H
#define CANVAS_MODE_MEASUREMENTS_H

#include <QObject>
#include "canvasmode.h"
#include "fpoint.h"

class Measurements;
class ScribusMainWindow;
class ScribusView;


/**
  shows measurements between mouse press and mouse release point
*/
class MeasurementsMode : public CanvasMode
{
	Q_OBJECT
	
public:
	explicit MeasurementsMode(ScribusView* view);
	~MeasurementsMode() override = default;

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	
	void activate(bool) override;
	void deactivate(bool) override;
	void keyPressEvent(QKeyEvent *e) override;
	void keyReleaseEvent(QKeyEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void drawControls(QPainter* p) override;

public slots:
	void setActive(bool);

private:
	void adjustPoint(QPoint point);
	void showValues();
	bool m_active {false};
	QPoint m_start;
	QPoint m_current;
	FPoint m_startDoc;
	FPoint m_currentDoc;
	Measurements* m_palette {nullptr};
};


#endif
