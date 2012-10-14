/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          clockwidget.cpp  -  description
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

#include "clockwidget.h"
#include <QPainter>
#include <QRect>
#include "scribusdoc.h"

ClockWidget::ClockWidget(QWidget *pa, ScribusDoc *doc) : QWidget(pa)
{
	setFixedSize(60, 60);
	m_doc = doc;
	m_time = 0;
	m_finalTime = 0;
	m_expand = true;
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
	m_timer.start(1000);
}

void ClockWidget::paintEvent(QPaintEvent *e)
{
	QPainter p;
	p.begin(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	if (m_expand)
	{
		p.setBrush(Qt::white);
		p.setPen(QPen(Qt::black, 1));
		p.drawRect(0, 0, width(), height());
		p.setPen(QPen(Qt::black, 2));
		p.save();
		p.translate(width() / 2, height() / 2);
		p.setPen(QPen(Qt::black, 2));
		for (int i = 0; i < 12; ++i)
		{
			p.drawLine(20, 0, 28, 0);
			p.rotate(30.0);
		}
		p.restore();
		if (m_finalTime != 0)
		{
			int startAngle = 90 * 16;
			int spanAngle = ((m_time * 360) / m_finalTime) * 16;
			QConicalGradient grad(QPointF(30, 30), 90);
			grad.setColorAt(1, QColor(255, 0, 0, 128));
			grad.setColorAt(0, Qt::red);
			p.setBrush(grad);
			p.setPen(Qt::NoPen);
			p.drawPie(QRect(0, 0, width(), height()), startAngle, -spanAngle);
		}
	}
	else
	{
		p.setBrush(Qt::white);
		p.setPen(QPen(Qt::black, 1));
		p.drawRect(0, 0, width(), height());
		p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, tr("A"));
	}
	p.end();
}

void ClockWidget::mouseReleaseEvent(QMouseEvent *m)
{
	m->accept();
	m_expand = !m_expand;
	QRect r = geometry();
	if (m_expand)
	{
		setGeometry(r.x(), r.y() - 45, 60, 60);
		setFixedSize(60, 60);
	}
	else
	{
		setGeometry(r.x(), r.y() + 45, 15, 15);
		setFixedSize(15, 15);
	}
	update();
	emit clicked();
}

void ClockWidget::resetTime()
{
	m_time = 0;
	if (m_expand)
		updateDisplay();
}

void ClockWidget::updateDisplay()
{
	if (m_doc->autoSave() && m_doc->autoSaveClockDisplay())
	{
		if (!isVisible())
			setVisible(true);
		m_finalTime = m_doc->autoSaveTime();
		m_time += 1000;
		if (m_time > m_finalTime)
			m_time -= m_finalTime;
		if (m_expand)
			update();
	}
	else
	{
		if (isVisible())
			setVisible(false);
	}
}

bool ClockWidget::isExpanded()
{
	return m_expand;
}
