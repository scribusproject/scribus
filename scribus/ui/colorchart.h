/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          colorchart.h  -  description
                             -------------------
    begin                : Sat Sep 15 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef COLORCHART_H
#define COLORCHART_H

#include <QPixmap>
#include <QWidget>

#include "scribusapi.h"

class QMouseEvent;
class QPaintEvent;
class QResizeEvent;
class ScribusDoc;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API ColorChart : public QWidget
{
	Q_OBJECT

public:
	ColorChart(QWidget *parent);
	ColorChart(QWidget *parent, ScribusDoc* doc);
	~ColorChart() {};

	void setDoc(ScribusDoc* doc) { m_doc = doc; }
	void drawMark(int x, int y);
	void setMark(int h, int s);
	void drawPalette(int val);

	int drawMode {0};

protected:
	ScribusDoc* m_doc { nullptr };

	int m_currentValue { 255 };
	double m_markX { 0.0 };
	double m_markY { 0.0 };
	bool m_drawMark { false };
	
	QPixmap m_pixmap;

	void drawHsvColorMap(int val);
	void drawLabColorMap(int val);

	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

signals:
	void ColorVal(int h, int s, bool ende);
};

#endif
