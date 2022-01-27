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

#include "dasheditor.h"

#include <algorithm>

#include <QApplication>
#include <QCursor>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPolygon>
#include <QSignalBlocker>
#include <QToolTip>

#include "scpainter.h"
#include "fpoint.h"
#include "iconmanager.h"

DashPreview::DashPreview(QWidget *pa) : QFrame(pa)
{
	setFrameShape( QFrame::Panel );
	setFrameShadow( QFrame::Sunken );
	setLineWidth( 2 );
	setMinimumSize(QSize(200, 35));
	setMaximumSize(QSize(3000, 35));
	setMouseTracking(true);

	m_dashValues.clear();
	m_dashValues.append(4.0);
	m_dashValues.append(2.0);
} 

void DashPreview::paintEvent(QPaintEvent *e)
{
	if (m_onlySelect)
		m_stops.clear();

	int pWidth = width() - 20;
	QImage pixm(pWidth, 10, QImage::Format_ARGB32_Premultiplied);

	ScPainter *p = new ScPainter(&pixm, pWidth, 10);
	p->clear(QColor(128, 128, 128));
	double startX = 0.0;
	p->setLineWidth(0);
	p->setFillMode(ScPainter::Solid);
	p->setBrush(Qt::black);
	for (int i = 0; i < m_dashValues.count(); ++i)
	{
		if (i % 2 == 0)
			p->setBrush(Qt::black);
		else
			p->setBrush(Qt::white);
		double w = m_dashValues[i] * 10;
		p->drawRect(startX, 0, w, 10);
		startX += w;
		if (m_onlySelect)
			m_stops.append(startX);
	}
	p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setFillMode(ScPainter::None);
	p->drawRect(0, 0, pWidth, 10);
	p->end();
	delete p;

	QPainter pw;
	pw.begin(this);
	pw.drawImage(10, 5, pixm);
	for (int i = 0; i < m_stops.count(); ++i)
	{
		double center = m_stops[i] + 10;
		pw.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		if (m_currentStop == i)
		{
			emit currStep(m_dashValues[m_currentStop]);
			pw.setBrush(Qt::red);
		}
		else
			pw.setBrush(Qt::blue);
		QPolygon cr;
		cr.setPoints(3, qRound(center), 16, qRound(center - 4), 29, qRound(center + 4), 29);
		pw.drawPolygon(cr);
	}
	pw.end();

	QFrame::paintEvent(e);
	m_onlySelect = true;
}

void DashPreview::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	m_moveTimer.start();
	m_mousePressed = true;
	m_currentStop = -1;

	m->accept();
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));

	for (int i = 0; i < m_stops.count(); ++i)
	{
		fpo = QRect(static_cast<int>(m_stops[i]) + 6, 16, 8, 13);
		if (fpo.contains(m->pos()))
		{
			m_currentStop = i;
			emit currStep(m_dashValues[m_currentStop]);
			repaint();
			return;
		}
	}
}

void DashPreview::mouseReleaseEvent(QMouseEvent *m)
{
	m->accept();
	qApp->restoreOverrideCursor();
	if ((m_mousePressed) && (m_stops.count() > 2) && (m_outside || m->y() > 30))
	{
		m_stops.removeAt(m_currentStop);
		m_dashValues.clear();
		double startX = 0.0;
		for (int i = 0; i < m_stops.count(); ++i)
		{
			double w = m_stops[i] / 10.0 - startX;
			m_dashValues.append(w);
			startX += w;
		}
		m_currentStop = 0;
		m_onlySelect = true;
		m_mousePressed = false;
		repaint();
		emit currStep(m_dashValues[m_currentStop]);
		emit dashChanged();
		return;
	}
	if ((m->y() < height()) && (m->y() > 16) && (m->x() > 9) && (m->x() < width() - 9))
	{
		if (m_currentStop != -1)
		{
			if (m_moveTimer.elapsed() < 250)
			{
				m_mousePressed = false;
				return;
			}
			m_stops[m_currentStop] = m->x() - 10;
		}
		else
		{
			if (m_dashValues.count() < 10)
				m_stops.append(m->x() - 10);
			std::sort(m_stops.begin(), m_stops.end());
			m_currentStop = 0;
			for (int i = 0; i < m_stops.count(); ++i)
			{
				QRect fpo = QRect(static_cast<int>(m_stops[i]) + 6, 16, 8, 13);
				if (fpo.contains(m->pos()))
				{
					m_currentStop = i;
					break;
				}
			}
		}
		m_dashValues.clear();
		double startX = 0.0;
		for (int i = 0; i < m_stops.count(); ++i)
		{
			double w = m_stops[i] / 10.0 - startX;
			m_dashValues.append(w);
			startX += w;
		}
		m_onlySelect = true;
		repaint();
		emit currStep(m_dashValues[m_currentStop]);
		emit dashChanged();
	}
	m_mousePressed = false;
}

void DashPreview::mouseMoveEvent(QMouseEvent *m)
{
	m->accept();

	QRect fpo;
	if ((!m_mousePressed) && (m->y() < height()) && (m->y() > 16) && (m->x() > 9) && (m->x() < width() - 9) && (m_dashValues.count() < 10))
	{
		setCursor(IconManager::instance().loadCursor("AddPoint.png", 1, 1));
		for (int i = 0; i < m_stops.count(); ++i)
		{
			fpo = QRect(static_cast<int>(m_stops[i]) + 6, 16, 8, 13);
			if (fpo.contains(m->pos()))
			{
				setCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
		}
	}
	if ((m_mousePressed) && (m->y() < height()) && (m->y() > 16) && (m->x() > 9) && (m->x() < width() - 9) && (m_currentStop != -1))
	{
		qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
		if (m_currentStop > 1)
		{
			if (static_cast<int>(m_stops[m_currentStop - 1] + 10) + 2 >= m->x())
				return;
		}
		if (m_currentStop < static_cast<int>(m_stops.count() - 2))
		{
			if (static_cast<int>(m_stops[m_currentStop + 1] + 10) - 2 < m->x())
				return;
		}
		m_stops[m_currentStop] = m->x() - 10;
		m_dashValues.clear();
		double startX = 0.0;
		for (int i = 0; i < m_stops.count(); ++i)
		{
			double w = m_stops[i] / 10.0 - startX;
			m_dashValues.append(w);
			startX += w;
		}
		m_onlySelect = true;
		repaint();
		startX = 0.0;
		for (int i = 0; i < m_currentStop; ++i)
		{
			startX += m_stops[i] / 10.0 - startX;
		}
		emit currStep(m_stops[m_currentStop] / 10.0 - startX);
	}
	if ((m_mousePressed) && (m_outside || m->y() > 30) && (m_currentStop >= 0) && (m_stops.count() > 2))
		qApp->changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
}

void DashPreview::leaveEvent(QEvent*)
{
	if (m_mousePressed)
	{
		if ((m_currentStop >= 0) && (m_stops.count() > 2))
			qApp->changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
		else
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	m_outside = true;
}

void DashPreview::enterEvent(QEvent*)
{
	m_outside = false;
}

void DashPreview::setActStep(double t)
{
	if (m_currentStop == -1)
		return;
	m_dashValues[m_currentStop] = t;
	m_onlySelect = true;
	repaint();
	emit dashChanged();
}

void DashPreview::setDashValues(const QVector<double>& vals)
{
	m_dashValues = vals;
	if ((m_currentStop >= vals.count()) || (m_currentStop == -1))
		m_currentStop = 0;
	m_onlySelect = true;
	if (m_dashValues.count() != 0)
	{
		if ((m_currentStop >= vals.count()) || (m_currentStop == -1))
			emit currStep(m_dashValues[0]);
		else
			emit currStep(m_dashValues[m_currentStop]);
	}
	update();
}

DashEditor::DashEditor(QWidget *pa) : QFrame(pa)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum));
	setFrameShape( QFrame::Panel );
	setLineWidth( 1 );
	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(3);
	gridLayout->setContentsMargins(6, 6, 6, 6);

	Preview = new DashPreview(this);
	gridLayout->addWidget(Preview, 0, 0, 1, 2);

	Position = new QDoubleSpinBox( this );
	Position->setDecimals(1);
	Position->setMinimum(0);
	Position->setMaximum(100);
	Position->setSingleStep(0.1);
	Position->setValue(0);
	Desc = new QLabel( this );

	Offset = new QDoubleSpinBox( this );
	Offset->setDecimals(1);
	Offset->setMinimum(0);
	Offset->setMaximum(100);
	Offset->setSingleStep(0.1);
	Offset->setValue(0);
	Desc2 = new QLabel( this );
	languageChange();

	gridLayout->addWidget(Desc, 1, 0, 1, 1);
	gridLayout->addWidget(Position, 1, 1, 1, 1);
	gridLayout->addWidget(Desc2, 2, 0, 1, 1);
	gridLayout->addWidget(Offset, 2, 1, 1, 1);

	connect(Position, SIGNAL(valueChanged(double)), Preview, SLOT(setActStep(double)));
	connect(Offset, SIGNAL(valueChanged(double)), this, SIGNAL(dashChanged()));
	connect(Preview, SIGNAL(currStep(double)), this, SLOT(setPos(double)));
	connect(Preview, SIGNAL(dashChanged()), this, SIGNAL(dashChanged()));
}

void DashEditor::setPos(double p)
{
	QSignalBlocker blocker(Position);
	Position->setValue(p);
}

void DashEditor::setDashValues(QVector<double> vals, double linewidth, double offset)
{
	QVector<double> tmp;
	for (int i = 0; i < vals.count(); i++)
		tmp.append(vals[i] / linewidth);
	Preview->setDashValues(tmp);

	disconnect(Offset, SIGNAL(valueChanged(double)), this, SIGNAL(dashChanged()));
	Offset->setValue(offset / linewidth);
	connect(Offset, SIGNAL(valueChanged(double)), this, SIGNAL(dashChanged()));
}

QVector<double> DashEditor::getDashValues(double linewidth)
{
	auto dashValues = Preview->dashValues();
	for (int i = 0; i < dashValues.count(); i++)
		dashValues[i] *= linewidth;
	return dashValues;
}

void DashEditor::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void DashEditor::languageChange()
{
	Desc->setText( tr( "Value:" ) );
	Desc2->setText( tr( "Offset:" ) );
}
