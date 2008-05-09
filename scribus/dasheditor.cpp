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

#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPolygon>
#include <QToolTip>

#include "scpainter.h"
#include "fpoint.h"
#include "util_icon.h"

DashPreview::DashPreview(QWidget *pa) : QFrame(pa)
{
	setFrameShape( QFrame::Panel );
	setFrameShadow( QFrame::Sunken );
	setLineWidth( 2 );
	setMinimumSize(QSize(200, 35));
	setMaximumSize(QSize(3000, 35));
	setMouseTracking(true);
	Mpressed = false;
	outside = false;
	onlyselect = true;
	StopM.clear();
	ActStop = 0;
	DashValues.clear();
	DashValues.append(4.0);
	DashValues.append(2.0);
} 

void DashPreview::paintEvent(QPaintEvent *e)
{
	if (onlyselect)
		StopM.clear();
	int pWidth = width()-20;
	QImage pixm(pWidth, 10, QImage::Format_ARGB32);
	ScPainter *p = new ScPainter(&pixm, pWidth, 10);
	p->clear(QColor(128, 128, 128));
	double startX = 0.0;
	p->setLineWidth(0);
	p->setFillMode(1);
	p->setBrush(Qt::black);
	for (int a = 0; a < DashValues.count(); a++)
	{
		if (a % 2 == 0)
			p->setBrush(Qt::black);
		else
			p->setBrush(Qt::white);
		double w = DashValues[a] * 10;
		p->drawRect(startX, 0, w, 10);
		startX += w;
		if (onlyselect)
			StopM.append(startX);
	}
	p->setPen(Qt::black);
	p->setLineWidth(1);
	p->setFillMode(0);
	p->drawRect(0, 0, pWidth, 10);
	p->end();
	delete p;
	QPainter pw;
	pw.begin(this);
	pw.drawImage(10, 5, pixm);
	for (int a = 0; a < StopM.count(); ++a)
	{
		double center = StopM[a]+10;
		pw.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		if (ActStop == a)
		{
			emit currStep(DashValues[ActStop]);
			pw.setBrush(Qt::red);
		}
		else
			pw.setBrush(Qt::blue);
		QPolygon cr;
		cr.setPoints(3, qRound(center), 16, qRound(center-4), 29, qRound(center+4), 29);
		pw.drawPolygon(cr);
	}
	pw.end();
	QFrame::paintEvent(e);
	onlyselect = true;
}

void DashPreview::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	m_moveTimer.start();
	Mpressed = true;
	ActStop = -1;
	m->accept();
	for (int yg = 0; yg < StopM.count(); ++yg)
	{
		fpo = QRect(static_cast<int>(StopM[yg]) + 6, 16, 8, 13);
		if (fpo.contains(m->pos()))
		{
			ActStop = yg;
			emit currStep(DashValues[ActStop]);
			repaint();
			return;
		}
	}
}

void DashPreview::mouseReleaseEvent(QMouseEvent *m)
{
	m->accept();
	if ((Mpressed) && (StopM.count() > 2) && (outside || m->y() > 30))
	{
		StopM.removeAt(ActStop);
		DashValues.clear();
		double startX = 0.0;
		for (int yg = 0; yg < StopM.count(); ++yg)
		{
			double w = StopM[yg] / 10.0 - startX;
			DashValues.append(w);
			startX += w;
		}
		ActStop = 0;
		onlyselect = true;
		Mpressed = false;
		repaint();
		emit currStep(DashValues[ActStop]);
		emit dashChanged();
		return;
	}
	if ((m->y() < height()) && (m->y() > 16) && (m->x() > 0) && (m->x() < width()))
	{
		if (ActStop != -1)
		{
			if (m_moveTimer.elapsed() < 250)
			{
				Mpressed = false;
				return;
			}
			StopM[ActStop] = m->x()-10;
		}
		else
		{
			if (DashValues.count() < 10)
				StopM.append(m->x()-10);
			qSort(StopM.begin(), StopM.end());
			ActStop = 0;
			for (int yg = 0; yg < StopM.count(); ++yg)
			{
				QRect fpo = QRect(static_cast<int>(StopM[yg]) + 6, 16, 8, 13);
				if (fpo.contains(m->pos()))
				{
					ActStop = yg;
					break;
				}
			}
		}
		DashValues.clear();
		double startX = 0.0;
		for (int yg = 0; yg < StopM.count(); ++yg)
		{
			double w = StopM[yg] / 10.0 - startX;
			DashValues.append(w);
			startX += w;
		}
		onlyselect = true;
		repaint();
		emit currStep(DashValues[ActStop]);
		emit dashChanged();
	}
	Mpressed = false;
}

void DashPreview::mouseMoveEvent(QMouseEvent *m)
{
	m->accept();
	QRect fpo;
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	if ((!Mpressed) && (m->y() < height()) && (m->y() > 16) && (m->x() > 9) && (m->x() < width()-9) && (DashValues.count() < 10))
	{
		qApp->changeOverrideCursor(QCursor(loadIcon("AddPoint.png"), 1, 1));
		for (int yg = 0; yg < StopM.count(); ++yg)
		{
			fpo = QRect(static_cast<int>(StopM[yg])+6, 16, 8, 13);
			if (fpo.contains(m->pos()))
			{
				qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
		}
	}
	if ((Mpressed) && (m->y() < height()) && (m->y() > 16) && (m->x() > 9) && (m->x() < width()-9) && (ActStop != -1))
	{
		qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
		if (ActStop > 1)
		{
			if (static_cast<int>(StopM[ActStop-1]+10)+2 >= m->x())
				return;
		}
		if (ActStop < static_cast<int>(StopM.count()-2))
		{
			if (static_cast<int>(StopM[ActStop+1]+10)-2 < m->x())
				return;
		}
		StopM[ActStop] = m->x()-10;
		DashValues.clear();
		double startX = 0.0;
		for (int yg = 0; yg < StopM.count(); ++yg)
		{
			double w = StopM[yg] / 10.0 - startX;
			DashValues.append(w);
			startX += w;
		}
		onlyselect = true;
		repaint();
		startX = 0.0;
		for (int yg = 0; yg < ActStop; ++yg)
		{
			startX += StopM[yg] / 10.0 - startX;
		}
		emit currStep(StopM[ActStop] / 10.0 - startX);
	}
	if ((Mpressed) && (outside || m->y() > 30) && (ActStop >= 0) && (StopM.count() > 2))
		qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png"), 1, 1));
}

void DashPreview::leaveEvent(QEvent*)
{
	if ((Mpressed) && (ActStop >= 0) && (StopM.count() > 2))
		qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png"), 1, 1));
	else
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	outside = true;
}

void DashPreview::enterEvent(QEvent*)
{
	outside = false;
}

void DashPreview::setActStep(double t)
{
	if (ActStop == -1)
		return;
	DashValues[ActStop] = t;
	onlyselect = true;
	repaint();
	emit dashChanged();
}

void DashPreview::setDashValues(QVector<double> vals)
{
	DashValues = vals;
	ActStop = 0;
	onlyselect = true;
	repaint();
	if (StopM.count() != 0)
		emit currStep(StopM[0] / 10.0);
}

DashEditor::DashEditor(QWidget *pa) : QFrame(pa)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum));
	setFrameShape( QFrame::Panel );
	setLineWidth( 1 );
	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(2);
	gridLayout->setMargin(2);

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
	disconnect(Position, SIGNAL(valueChanged(double)), Preview, SLOT(setActStep(double)));
	Position->setValue(p);
	connect(Position, SIGNAL(valueChanged(double)), Preview, SLOT(setActStep(double)));
}

void DashEditor::setDashValues(QVector<double> vals, double linewidth, double offset)
{
	QVector<double> tmp;
	for (int a = 0; a < vals.count(); a++)
	{
		tmp.append(vals[a] / linewidth);
	}
	Preview->setDashValues(tmp);
	disconnect(Offset, SIGNAL(valueChanged(double)), this, SIGNAL(dashChanged()));
	Offset->setValue(offset / linewidth);
	connect(Offset, SIGNAL(valueChanged(double)), this, SIGNAL(dashChanged()));
}

QVector<double> DashEditor::getDashValues(double linewidth)
{
	QVector<double> tmp;
	for (int a = 0; a < Preview->DashValues.count(); a++)
	{
		tmp.append(Preview->DashValues[a] * linewidth);
	}
	return tmp;
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
