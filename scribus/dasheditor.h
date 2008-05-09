/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          gradienteditor  -  description
                             -------------------
    begin                : Mit Mai 26 2004
    copyright            : (C) 2004 by Franz Schmid
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

#ifndef DASHEDITOR_H
#define DASHEDITOR_H

#include <QLabel>
#include <QFrame>
#include <QDoubleSpinBox>
#include <QLayout>
#include <QList>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTime>

class QEvent;

#include "scribusapi.h"
#include "vgradient.h"

class SCRIBUS_API DashPreview : public QFrame
{
	Q_OBJECT

public:
	DashPreview(QWidget *pa);
	~DashPreview() {};
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *m);
	void leaveEvent(QEvent*);
	void enterEvent(QEvent*);
	void setDashValues(QVector<double> vals);
	QVector<double> DashValues;
	QList<double> StopM;
	bool Mpressed;
	bool outside;
	bool onlyselect;
	int ActStop;
	QTime m_moveTimer;

public slots:
	void setActStep(double t);

signals:
	void currStep(double);
	void dashChanged();
};

class SCRIBUS_API DashEditor : public QFrame
{
	Q_OBJECT

public:
	DashEditor(QWidget *pa);
	~DashEditor() {};
	
	virtual void changeEvent(QEvent *e);
	void setDashValues(QVector<double> vals, double linewidth, double offset);
	QVector<double> getDashValues(double linewidth);
	
	DashPreview *Preview;
	QLabel *Desc;
	QDoubleSpinBox *Position;
	QLabel *Desc2;
	QDoubleSpinBox *Offset;

public slots:
	void setPos(double);
	void languageChange();

signals:
	void dashChanged();
};

#endif

