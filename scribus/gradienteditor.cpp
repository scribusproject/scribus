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

#include "gradienteditor.h"
#include "gradienteditor.moc"
#include <qpainter.h>
#include <qpixmap.h>
#include <qcursor.h>
#include "scpainter.h"
#include "fpoint.h"
extern QPixmap loadIcon(QString nam);

GradientPreview::GradientPreview(QWidget *pa) : QLabel(pa)
{
	setFrameShape( QFrame::Panel );
	setFrameShadow( QFrame::Sunken );
	setLineWidth( 2 );
	setEraseColor(QColor(255,255,255));
	setMinimumSize(QSize(200, 60));
	setMaximumSize(QSize(3000, 60));
	setMouseTracking(true);
	Mpressed = false;
	outside = true;
	onlyselect = true;
	fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();

	QColor color;
	color = QColor(255,255,255);
	fill_gradient.addStop( color, 0.0, 0.5, 1.0 );
	color = QColor(0,0,0);
	fill_gradient.addStop( color, 1.0, 0.5, 1.0 );

	QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
	StopM.clear();
	ActStop = 0;
	for (uint a = 0; a < fill_gradient.Stops(); ++a)
	{
		int center = qRound(cstops.at(a)->rampPoint * (width()-20))+10;
		StopM.append(center);
	}
} 

void GradientPreview::paintEvent(QPaintEvent *e)
{
	QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
	StopM.clear();
	for (uint a = 0; a < fill_gradient.Stops(); ++a)
	{
		int center = qRound(cstops.at(a)->rampPoint * (width()-20))+10;
		StopM.append(center);
	}
	QPixmap pixm(width()-20, 37);
	ScPainter *p = new ScPainter(&pixm, width()-20, 37);
	p->setPen(black);
	p->setLineWidth(1);
	p->setFillMode(2);
	p->fill_gradient = fill_gradient;
	p->setGradient(VGradient::linear, FPoint(0,20), FPoint(width()-20,20));
	p->drawRect(0, 0, width()-20, 37);
	p->end();
	delete p;
	QPainter pw;
	pw.begin(this);
	pw.drawPixmap(10, 5, pixm);
	for (uint a = 0; a < fill_gradient.Stops(); ++a)
	{
		int center = qRound(cstops.at(a)->rampPoint * (width()-20))+10;
		pw.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
		if (StopM[QMAX(ActStop,0)] == center)
			pw.setBrush(red);
		else
			pw.setBrush(blue);
		if ((a == 0) || ( a == fill_gradient.Stops()-1))
			pw.drawRect(QRect(center-4, 43, 8, 13));
		else
		{
			QPointArray cr;
			cr.setPoints(3, qRound(center), 43, qRound(center-4), 56, qRound(center+4), 56);
			pw.drawPolygon(cr);
		}
	}
	pw.end();
	QLabel::paintEvent(e);
}

void GradientPreview::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	Mpressed = true;
	ActStop = -1;
	QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
	for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
	{
		fpo = QRect(static_cast<int>(StopM[yg])-4, 43, 8, 13);
		if (fpo.contains(m->pos()))
		{
			ActStop = yg;
			emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
			emit currTrans(cstops.at(ActStop)->opacity);
			repaint();
			onlyselect = true;
			return;
		}
	}
}

void GradientPreview::mouseReleaseEvent(QMouseEvent *m)
{
	QRect fpo;
	if ((Mpressed) && (ActStop != 0) && (ActStop != static_cast<int>(StopM.count()-1)) && (ActStop != -1) && (outside))
	{
		onlyselect = false;
		fill_gradient.removeStop(ActStop);
		ActStop = 0;
		repaint();
		QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
		emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
		emit currTrans(cstops.at(ActStop)->opacity);
	}
	if ((m->y() < height()) && (m->y() > 43) && (m->x() > 0) && (m->x() < width()) && (ActStop == -1))
	{
		QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
		double newStop = static_cast<double>((m->x() - 10)) / (static_cast<double>(width())-20);
		fill_gradient.addStop(cstops.at(0)->color, newStop, 0.5, 1.0, cstops.at(0)->name, cstops.at(0)->shade);
		repaint();
		onlyselect = false;
		cstops = fill_gradient.colorStops();
		for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
		{
			fpo = QRect(static_cast<int>(StopM[yg])-4, 43, 8, 13);
			if (fpo.contains(m->pos()))
			{
				ActStop = yg;
				emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
				emit currTrans(cstops.at(ActStop)->opacity);
				repaint();
				break;
			}
		}
	}
	Mpressed = false;
	if (!onlyselect)
		emit gradientChanged();
}

void GradientPreview::mouseMoveEvent(QMouseEvent *m)
{
	QRect fpo;
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	if ((!Mpressed) && (m->y() < height()) && (m->y() > 43) && (m->x() > 10) && (m->x() < width()-10))
	{
		qApp->setOverrideCursor(QCursor(loadIcon("AddPoint.png"), 4, 3), true);
		for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
		{
			fpo = QRect(static_cast<int>(StopM[yg])-4, 43, 8, 13);
			if (fpo.contains(m->pos()))
			{
				if ((yg == 0) || (yg == static_cast<int>(StopM.count())-1))
					qApp->setOverrideCursor(QCursor(ArrowCursor), true);
				else
					qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
				return;
			}
		}
	}
	if ((Mpressed) && (m->y() < height()) && (m->y() > 43) && (m->x() > 10) && (m->x() < width()-10) && (ActStop != -1))
	{
		if ((ActStop == 0) || (ActStop == static_cast<int>(StopM.count()-1)))
			return;
		qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
		double newStop = static_cast<double>((m->x() - 10)) / (static_cast<double>(width())-20);
		if (ActStop > 1)
		{
			if (StopM[ActStop-1]+2 >= m->x())
				return;
		}
		if (ActStop < static_cast<int>(StopM.count()-2))
		{
			if (StopM[ActStop+1]-2 < m->x())
				return;
		}
		StopM[ActStop] = m->x();
		QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
		cstops.at(ActStop)->rampPoint = newStop;
		cstops.sort();
		onlyselect = false;
		repaint();
	}
	if ((Mpressed) && (outside) && (ActStop != 0) && (ActStop != static_cast<int>(StopM.count()-1)) && (ActStop != -1))
		qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
}

void GradientPreview::leaveEvent(QEvent*)
{
	if ((Mpressed) && (ActStop != 0) && (ActStop != static_cast<int>(StopM.count()-1)) && (ActStop != -1))
		qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
	else
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	outside = true;
}

void GradientPreview::enterEvent(QEvent*)
{
	outside = false;
}

void GradientPreview::updateDisplay()
{
	repaint();
	ActStop = 0;
	QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
	emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
	emit currTrans(cstops.at(ActStop)->opacity);
}

void GradientPreview::setActColor(QColor c, QString n, int s)
{
	if (ActStop == -1)
		return;
	QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
	cstops.at(ActStop)->color = c;
	cstops.at(ActStop)->name = n;
	cstops.at(ActStop)->shade = s;
	repaint();
}

void GradientPreview::setActTrans(double t)
{
	if (ActStop == -1)
		return;
	QPtrVector<VColorStop> cstops = fill_gradient.colorStops();
	cstops.at(ActStop)->opacity = t;
	repaint();
}
