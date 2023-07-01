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

#include <QElapsedTimer>
#include <QFrame>
#include <QHBoxLayout>
#include <QLayout>
#include <QList>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QVBoxLayout>

class QDoubleSpinBox;
class QEvent;
class QLabel;

#include "scribusapi.h"
#include "vgradient.h"

class SCRIBUS_API DashPreview : public QFrame
{
	Q_OBJECT

public:
	DashPreview(QWidget *pa);
	~DashPreview() {};

	const QVector<double>& dashValues() const { return m_dashValues; }
	void setDashValues(const QVector<double>& vals);

public slots:
	void setActStep(double t);

protected:
	void paintEvent(QPaintEvent* e) override;
	void mousePressEvent(QMouseEvent* m) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent* m) override;
	void leaveEvent(QEvent*) override;
	void enterEvent(QEvent*) override;

private:
	bool  m_onlySelect { true };
	bool  m_outside { false };
	bool  m_mousePressed { false };
	int   m_currentStop { 0 };
	QElapsedTimer m_moveTimer;

	QVector<double> m_dashValues;
	QList<double>   m_stops;

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
	
	void setDashValues(QVector<double> vals, double linewidth, double offset);
	QVector<double> getDashValues(double linewidth);
	
	DashPreview *Preview;
	QLabel *Desc;
	QDoubleSpinBox *Position;
	QLabel *Desc2;
	QDoubleSpinBox *Offset;

protected:
	void changeEvent(QEvent *e) override;

public slots:
	void setPos(double);
	void languageChange();

signals:
	void dashChanged();
};

#endif

