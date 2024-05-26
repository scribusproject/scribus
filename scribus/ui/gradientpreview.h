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

#ifndef GRADIENTPREVIEW_H
#define GRADIENTPREVIEW_H

#include <QList>
#include <QFrame>
#include <QEnterEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>

class QEvent;
class ScColor;

#include "scribusapi.h"
#include "vgradient.h"


/*class SCRIBUS_API GradientPreview : public QFrame
{
	Q_OBJECT

public:
	GradientPreview(QWidget *pa);
	~GradientPreview() {};

	void updateDisplay();
	void setActColor(const QColor& c, const QString& n, int s);
	void setActTrans(double t);
	void setActStep(double t);
	void setGradient(const VGradient& gradient);
	void setGradientEditable(bool val);

	const VGradient& gradient() { return fill_gradient; }
	
	QList<int> StopM;
	bool Mpressed { false };
	bool outside { true };
	bool onlyselect { true };
	bool isEditable { true };
	int ActStop { 0 };
	int contextStop { 0 };
	QPoint mPos;

protected:
	void paintEvent(QPaintEvent* e) override;
	void keyPressEvent(QKeyEvent*) override;
	void mousePressEvent(QMouseEvent* m) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent* m) override;
	void leaveEvent(QEvent*) override;
	void enterEvent(QEnterEvent*) override;

private:
	VGradient fill_gradient;

private slots:
	void addStop();
	void removeStop();

signals:
	void selectedColor(QString, int);
	void selectedStop(VColorStop*);
	void currTrans(double);
	void currStep(double);
	void gradientChanged();
};
*/

class SCRIBUS_API GradientPreview : public QWidget
{
	Q_OBJECT
public:
	explicit GradientPreview(QWidget *parent = nullptr);
	~GradientPreview() {};
	void paintEvent(QPaintEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *m);
	void mouseMoveEvent(QMouseEvent *m);

	void setActiveStopColor(const QColor& c, const QString& n, int s, double a);
	QColor activeStopColor();

	void setActiveStopPosition(double t);

	void setIsEditable(bool isEditable);
	bool isEditable();

	void setGradient(const VGradient& gradient);
	const VGradient& gradient();

	void updateDisplay();

private:
	VGradient fill_gradient;
	QPointF m_mousePos;
	bool m_isEditable;
	int m_activeStop;
	int m_tmpStop;

	QRect saveAreaRect();
	QRect canvasRect();
	QRect scaleRect();
	QRect handleRect(int center);
	QRect stopRect(int center);
	qreal ratio();
	bool isMouseOutside(QPoint mouse);
	int mapPositionToGrid(qreal pos);
	int stopAtPosition(QPoint position);
	VColorStop *activeStop(int offset = 0) const;
	double percentFromPosition(QPointF position);
	int percentToPosition(double t);

	void setup();
	void addStop(QPoint mousePosition);
	void removeStop();
	void emitStopPosition();
	void emitStop();
	void updateTmpStop(double t);
	void sortStops();

signals:
	void selectedStop(VColorStop*);
	void selectedPosition(double);

};
#endif
