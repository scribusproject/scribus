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

#include <qlabel.h>
#include <qvaluelist.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "vgradient.h"

class GradientPreview : public QLabel
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
	QValueList<int> StopM;
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

class GradientEditor : public QLabel
{
	Q_OBJECT

public:
	GradientEditor(QWidget *pa);
	~GradientEditor() {};
	GradientPreview *Preview;
	QLabel *Desc;
	QSpinBox *Position;

public slots:
	void setPos(double);
	void changePos(int);

signals:
	void gradientChanged();

protected:
	QVBoxLayout* Form1Layout;
	QHBoxLayout* Layout1;
};

#endif

