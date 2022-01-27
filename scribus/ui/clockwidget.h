/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          clockwidget.h  -  description
                             -------------------
    begin                : Sat Oct 13
    copyright            : (C) 2012 by Franz Schmid
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

#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QMouseEvent>
#include <QPaintEvent>
#include <QTimer>
#include <QWidget>
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API ClockWidget : public QWidget
{
	Q_OBJECT

public:
	ClockWidget(QWidget *pa, ScribusDoc *doc);
	~ClockWidget() {}
	void paintEvent(QPaintEvent *e);
	void mouseReleaseEvent(QMouseEvent *);
	bool isExpanded();
private:
	bool m_expand;
	int m_time;
	int m_finalTime;
	QTimer m_timer;
	ScribusDoc* m_doc;
public slots:
	void resetTime();
	void updateDisplay();
signals:
	void clicked();
};
#endif
