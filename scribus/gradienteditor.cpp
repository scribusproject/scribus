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

#include "gradienteditor.h"

#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPolygon>
#include <QToolTip>


#include "scpainter.h"
#include "fpoint.h"
#include "util_icon.h"

GradientPreview::GradientPreview(QWidget *pa) : QLabel(pa)
{
	setFrameShape( QFrame::Panel );
	setFrameShadow( QFrame::Sunken );
	setLineWidth( 2 );
	setMinimumSize(QSize(200, 70));
	setMaximumSize(QSize(3000, 70));
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

	QList<VColorStop*> cstops = fill_gradient.colorStops();
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
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	StopM.clear();
	for (uint a = 0; a < fill_gradient.Stops(); ++a)
	{
		int center = qRound(cstops.at(a)->rampPoint * (width()-20))+10;
		StopM.append(center);
	}
	QImage pixm(width()-20, 37, QImage::Format_ARGB32);
	ScPainter *p = new ScPainter(&pixm, width()-20, 37);
	p->clear(Qt::white);
	p->setPen(Qt::black);
	p->setLineWidth(1);
	p->setFillMode(2);
	p->fill_gradient = fill_gradient;
	p->setGradient(VGradient::linear, FPoint(0,20), FPoint(width()-20,20));
	p->drawRect(0, 0, width()-20, 37);
	p->end();
	delete p;
	QPainter pw;
	pw.begin(this);
	pw.drawImage(10, 5, pixm);
	for (uint a = 0; a < fill_gradient.Stops(); ++a)
	{
		int center = qRound(cstops.at(a)->rampPoint * (width()-20))+10;
		pw.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		if (StopM[qMax(ActStop,0)] == center)
			pw.setBrush(Qt::red);
		else
			pw.setBrush(Qt::blue);
		QPolygon cr;
		cr.setPoints(3, qRound(center), 43, qRound(center-4), 56, qRound(center+4), 56);
		pw.drawPolygon(cr);
	}
	pw.end();
	QLabel::paintEvent(e);
}

void GradientPreview::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	Mpressed = true;
	ActStop = -1;
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
	{
		fpo = QRect(static_cast<int>(StopM[yg])-4, 43, 8, 13);
		if (fpo.contains(m->pos()))
		{
			ActStop = yg;
			emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
			emit currTrans(cstops.at(ActStop)->opacity);
			emit currStep(cstops.at(ActStop)->rampPoint);
			repaint();
			onlyselect = true;
			return;
		}
	}
}

void GradientPreview::mouseReleaseEvent(QMouseEvent *m)
{
	QRect fpo;
	if ((Mpressed) && (ActStop > 0) && (ActStop != static_cast<int>(StopM.count()-1)) && (outside || m->y() > 60))
	{
		onlyselect = false;
		fill_gradient.removeStop(ActStop);
		ActStop = 0;
		repaint();
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
		emit currTrans(cstops.at(ActStop)->opacity);
		emit currStep(cstops.at(ActStop)->rampPoint);
	}
	if ((m->y() < height()) && (m->y() > 43) && (m->x() > 0) && (m->x() < width()) && (ActStop == -1))
	{
		QList<VColorStop*> cstops = fill_gradient.colorStops();
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
				emit currStep(cstops.at(ActStop)->rampPoint);
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
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	if ((!Mpressed) && (m->y() < height()) && (m->y() > 43) && (m->x() > 9) && (m->x() < width()-9))
	{
		qApp->changeOverrideCursor(QCursor(loadIcon("AddPoint.png"), 1, 1));
		for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
		{
			fpo = QRect(static_cast<int>(StopM[yg])-4, 43, 8, 13);
			if (fpo.contains(m->pos()))
			{
				qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
		}
	}
	if ((Mpressed) && (m->y() < height()) && (m->y() > 43) && (m->x() > 9) && (m->x() < width()-9) && (ActStop != -1))
	{
		qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
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
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		cstops.at(ActStop)->rampPoint = newStop;
		emit currStep(cstops.at(ActStop)->rampPoint);
		qSort(cstops.begin(), cstops.end());
		onlyselect = false;
		repaint();
	}
	if ((Mpressed) && (outside || m->y() > 60) && (ActStop > 0) && (ActStop != static_cast<int>(StopM.count()-1)))
		qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png"), 1, 1));
}

void GradientPreview::leaveEvent(QEvent*)
{
	if ((Mpressed) && (ActStop > 0) && (ActStop != static_cast<int>(StopM.count()-1)))
		qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png"), 1, 1));
	else
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
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
	if (!fill_gradient.colorStops().isEmpty())
	{
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		emit selectedColor(cstops.at(ActStop)->name, cstops.at(ActStop)->shade);
		emit currTrans(cstops.at(ActStop)->opacity);
		emit currStep(cstops.at(ActStop)->rampPoint);
	}
}

void GradientPreview::setActColor(QColor c, QString n, int s)
{
	if (ActStop == -1)
		return;
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	cstops.at(ActStop)->color = c;
	cstops.at(ActStop)->name = n;
	cstops.at(ActStop)->shade = s;
	repaint();
}

void GradientPreview::setActTrans(double t)
{
	if (ActStop == -1)
		return;
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	cstops.at(ActStop)->opacity = t;
	repaint();
}

void GradientPreview::setActStep(double t)
{
	if (ActStop == -1)
		return;
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	cstops.at(ActStop)->rampPoint = t;
	repaint();
}

GradientEditor::GradientEditor(QWidget *pa) : QLabel(pa)
{
	Form1Layout = new QVBoxLayout( this );
	Form1Layout->setSpacing(0);
	Form1Layout->setMargin(0);
	Preview = new GradientPreview(this);
	Form1Layout->addWidget(Preview);
	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 4 );
	Layout1->setMargin( 0 );
	Position = new QSpinBox( this );
	Position->setMinimum(0);
	Position->setMaximum(100);
	Position->setSingleStep(1);
	Position->setValue(0);
	Desc = new QLabel( this );
	languageChange();
	Layout1->addWidget( Desc );
	Layout1->addWidget(Position);
	Form1Layout->addLayout(Layout1);
	connect(Position, SIGNAL(valueChanged(int)), this, SLOT(changePos(int)));
	connect(Preview, SIGNAL(currStep(double)), this, SLOT(setPos(double)));
	resize(minimumSizeHint());
}

void GradientEditor::setPos(double p)
{
	disconnect(Position, SIGNAL(valueChanged(int)), this, SLOT(changePos(int)));
	Position->setValue(qRound(p * 100));
	connect(Position, SIGNAL(valueChanged(int)), this, SLOT(changePos(int)));
}

void GradientEditor::changePos(int v)
{
	Preview->setActStep(static_cast<double>(v) / 100.0);
	emit gradientChanged();
}

void GradientEditor::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
}

void GradientEditor::languageChange()
{
	Desc->setText( tr( "Position:" ) );
	Position->setSuffix( tr(" %") );
}

bool GradientEditor::event(QEvent * event)
{
	if (event->type() == QEvent::ToolTip) 
	{
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		QToolTip::showText(helpEvent->globalPos(), tr( "Add, change or remove color stops here" ), Preview, QRect(10,43, Preview->width()-20, 13));
	}
	return QWidget::event(event);
}

