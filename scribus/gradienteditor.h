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

#ifndef GRADEDITOR_H
#define GRADEDITOR_H

#include <QLabel>
#include <QSpinBox>
#include <QLayout>
#include <QList>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>

class QEvent;

#include "scribusapi.h"
#include "vgradient.h"

class SCRIBUS_API GradientPreview : public QLabel
{
	Q_OBJECT

public:
	GradientPreview(QWidget *pa);
	~GradientPreview() {};
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *m);
	void leaveEvent(QEvent*);
	void enterEvent(QEvent*);
	void updateDisplay();
	void setActColor(QColor c, QString n, int s);
	void setActTrans(double t);
	void setActStep(double t);
	VGradient fill_gradient;
	QList<int> StopM;
	bool Mpressed;
	bool outside;
	bool onlyselect;
	int ActStop;

signals:
	void selectedColor(QString, int);
	void currTrans(double);
	void currStep(double);
	void gradientChanged();
};

class SCRIBUS_API GradientEditor : public QLabel
{
	Q_OBJECT

public:
	GradientEditor(QWidget *pa);
	~GradientEditor() {};
	
	virtual void changeEvent(QEvent *e);
	
	GradientPreview *Preview;
	QLabel *Desc;
	QSpinBox *Position;

public slots:
	void setPos(double);
	void changePos(int);
	void languageChange();

signals:
	void gradientChanged();

protected:
	virtual bool event ( QEvent * event );
// 	QVBoxLayout* Form1Layout;
// 	QHBoxLayout* Layout1;
};

#endif

