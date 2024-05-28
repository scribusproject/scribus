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

#include <QPainter>
#include <QPaintEvent>
#include <QApplication>

#include "iconmanager.h"
#include "gradientpreview.h"
#include "util_gui.h"

const int RADIUS = 6;
const int GRADIENT_HEIGHT = 20;

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

GradientPreview::GradientPreview(QWidget *parent)
	: QWidget{parent}
{
	int h = scaleRect().height() + canvasRect().height() + RADIUS * 3;

	setMinimumSize(QSize(200, h));
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);

	setup();
}

void GradientPreview::setup()
{
	m_activeStop = 0;
	m_tmpStop = 0;
	m_isEditable = true;
	m_mousePos = QPointF();

	QColor color;
	fill_gradient.clearStops();

	color = QColor(255,255,255);
	fill_gradient.addStop( color, 0.0, 0.5, 1.0 );

	color = QColor(0,0,0);
	fill_gradient.addStop( color, 1.0, 0.5, 1.0 );
}


/* ********************************************************************************* *
 *
 * Private Methods
 *
 * ********************************************************************************* */

int GradientPreview::stopAtPosition(QPoint position)
{
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	for (int i = 0; i < cstops.count(); ++i)
	{
		int stopPos = percentToPosition(cstops.at(i)->rampPoint);
		if (stopRect(stopPos).contains(position) )
			return i;
	}

	return -1;
}

double GradientPreview::percentFromPosition(QPointF position)
{
	double t = static_cast<double>((position.x() - RADIUS)) / static_cast<double>(saveAreaRect().width());
	return qBound(0., t, 1.);
}

int GradientPreview::percentToPosition(double t)
{
	return qRound(t * saveAreaRect().width() + RADIUS );
}


int GradientPreview::mapPositionToGrid(qreal pos)
{
	return static_cast<int>(pos * 100 * ratio() + .5);
}


void GradientPreview::addStop(QPoint mousePosition)
{
	QColor colorL = activeStop()->color;
	QString nameL = activeStop()->name;
	double opacityL = activeStop()->opacity;
	double shadeL = activeStop()->shade;

	QColor colorR = activeStop()->color;
	QString nameR = activeStop()->name;
	double opacityR = activeStop()->opacity;
	double shadeR = activeStop()->shade;

	double t = percentFromPosition(mousePosition);
	double mid = 0.5;

	// Add tmp stop
	fill_gradient.addStop(colorL, t, mid, opacityL, nameL, shadeL);
	sortStops();

	// Update values
	m_activeStop = stopAtPosition(mousePosition);
	m_tmpStop = m_activeStop;


	// The following code calculate the blend color between two neighbor stops of the new one.
	// Currently commented out because Scribus don't support calculated colors without ID (color name) yet.
/*
	VColorStop *stopL;
	VColorStop *stopR;

	// Get left stop
	if (m_activeStop - 1 >= 0)
		stopL = activeStop(-1);
	else
		stopL = activeStop(1);

	colorL =  stopL->color;
	shadeL =  stopL->shade;
	opacityL =  stopL->opacity;
	nameL = stopL->name;

	// Get right stop
	if (m_activeStop + 1 < fill_gradient.colorStops().count())
		stopR = activeStop(1);
	else
		stopR = activeStop(-1);

	colorR =  stopR->color;
	shadeR =  stopR->shade;
	opacityR =  stopR->opacity;
	nameR = stopR->name;


	VColorStop stop = computeInBetweenStop(stopL, stopR, t);
	fill_gradient.setStop(stop.color, t, mid, stop.opacity, stop.name, stop.shade);
*/

	// add color from active stop
	fill_gradient.setStop(colorL, t, mid, opacityL, nameL, shadeL);

	// Update UI
	repaint();

	if (m_activeStop > -1)
	{
		emitStop();
	}
}

void GradientPreview::removeStop()
{
	QList<VColorStop*> cstops = fill_gradient.colorStops();

	// Remove stop if it is not first or last one
	if (m_activeStop != -1 && cstops.count() > 2)
	{
		fill_gradient.removeStop(m_activeStop);
		m_activeStop = qMax(m_activeStop - 1, 0);
		m_tmpStop = m_activeStop;

		repaint();
		emitStop();
	}
}

void GradientPreview::emitStopPosition()
{
	if (!fill_gradient.colorStops().isEmpty())
		emit selectedPosition(activeStop()->rampPoint);
}

void GradientPreview::emitStop()
{
	if (!fill_gradient.colorStops().isEmpty())
		emit selectedStop(activeStop());
}

void GradientPreview::updateTmpStop(double t)
{
	// update stop position
	if(m_activeStop != -1)
	{
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		cstops.at(m_activeStop)->rampPoint = t;

		std::stable_sort(cstops.begin(), cstops.end(), compareStops);
		for(int i = 0; i < cstops.count(); i++)
		{
			VColorStop *stop = cstops.at(i);
			if(stop->rampPoint == t)
				m_tmpStop = i;
		}

		repaint();
	}
}

void GradientPreview::sortStops()
{
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	std::stable_sort(cstops.begin(), cstops.end(), compareStops);

	VGradient grad(fill_gradient);
	grad.clearStops();

	foreach(VColorStop *stop, cstops)
		grad.addStop(*stop);

	fill_gradient = grad;

}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void GradientPreview::paintEvent(QPaintEvent *e)
{

	QList<VColorStop*> cstops = fill_gradient.colorStops();
	std::stable_sort(cstops.begin(), cstops.end(), compareStops);

	QImage pixm(canvasRect().width(), canvasRect().height(), QImage::Format_ARGB32_Premultiplied);
	QColor cHandleActive(colorByRole(QPalette::Highlight, 1, isEnabled()));

	// Draw Background
	QPainter pb(&pixm);
	renderCheckerPattern(&pb, pixm.rect());
	pb.end();

	// Draw Gradient
	ScPainter *p = new ScPainter(&pixm, pixm.width(), pixm.height());
	p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setFillMode(ScPainter::Gradient);
	p->fill_gradient = fill_gradient;
	p->setGradient(VGradient::linear, FPoint(0, 0), FPoint(pixm.width(), 0), FPoint(0, 0), 1.0, 0.0);
	p->drawRect(0, 0, pixm.width(), pixm.height());
	p->end();
	delete p;

	// Draw UI
	QPainter pw(this);
	pw.drawImage(canvasRect().left(), canvasRect().top(), pixm);
	pw.setPen(palette().color(QPalette::Midlight));
	pw.setBrush(Qt::NoBrush);
	pw.drawRect(QRectF(canvasRect()).adjusted(.5, .5, -.5, -.5));

	if (isEditable())
	{
		pw.setRenderHint(QPainter::Antialiasing, true);

		// Draw Stops
		for (int i = 0; i < fill_gradient.stops(); ++i)
		{
			qreal sPos = cstops.at(i)->rampPoint;
			qreal xPos = mapPositionToGrid(sPos) + saveAreaRect().left() + .5;
			qreal yPos = handleRect(sPos).top() + 0.5;

			// Draw Marker
			if (m_tmpStop == i)
			{
				QPainterPath marker;
				marker.moveTo( QPointF(xPos - 5, canvasRect().top() - 5.5) );
				marker.lineTo( QPointF(xPos + 5, canvasRect().top() - 5.5) );
				marker.lineTo( QPointF(xPos, canvasRect().top()) );
				marker.closeSubpath();

				pw.setPen(QPen(palette().color(QPalette::Window), 1));
				pw.setBrush(cHandleActive);
				pw.drawPath(marker);
			}

			bool isEmpty = cstops.at(i)->name == CommonStrings::tr_NoneColor;
			drawPointerHandle(&pw, QPointF(xPos, yPos), RADIUS * 2 - 2, cstops.at(i)->color, isEnabled(), isEmpty);

		}
	}

	pw.end();

	QWidget::paintEvent(e);

}

void GradientPreview::keyPressEvent(QKeyEvent *e)
{
	if (isEditable())
		if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
			removeStop();
}

void GradientPreview::mousePressEvent(QMouseEvent *m)
{
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));

	if (isEditable()){

		if (m->button() == Qt::LeftButton)
		{
			m_activeStop = stopAtPosition(m->pos());
			m_tmpStop = m_activeStop;

			if (m_activeStop == -1)
				addStop(m->pos());
			else
			{
				repaint();
				emitStop();
			}

		}
	}
	else
		m_activeStop = m_tmpStop = -1;

}

void GradientPreview::mouseReleaseEvent(QMouseEvent *m)
{

	QApplication::restoreOverrideCursor();

	if (isEditable())
	{		
		if (m->button() == Qt::LeftButton)
		{
			if (isMouseOutside(m->pos()))
				removeStop();
			else
			{
				sortStops();
				m_activeStop = m_tmpStop;
				repaint();
				emitStop();
			}

		}
	}
}

void GradientPreview::mouseMoveEvent(QMouseEvent *m)
{
	QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));

	if (isEditable())
	{

		m_mousePos = m->position();

		// Set Hover Cursor
		if (saveAreaRect().contains(m_mousePos.toPoint()) )
		{
			if (stopAtPosition(m_mousePos.toPoint()) > -1)
				setCursor(QCursor(Qt::SizeHorCursor));
			else
				setCursor(IconManager::instance().loadCursor("AddPoint.png", 1, 1));
		}

		// Press Left Mouse
		if (m->buttons() & Qt::LeftButton)
		{
			double newPos = percentFromPosition(m_mousePos);
			updateTmpStop(newPos);
			emitStopPosition();

			QList<VColorStop*> cstops = fill_gradient.colorStops();
			// Set "remove" cursor if gradient has more than 2 stops
			if (isMouseOutside(m->pos()) && cstops.count() > 2)
				QApplication::changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));

		}
	}


}


/* ********************************************************************************* *
 *
 * Public Members
 *
 * ********************************************************************************* */

void GradientPreview::setActiveStopColor(const QColor &c, const QString &n, int s, double a)
{
	if (m_activeStop == -1)
		return;

	activeStop()->color = c;
	activeStop()->name = n;
	activeStop()->opacity = a;
	activeStop()->shade = s;
	repaint();
}

QColor GradientPreview::activeStopColor()
{
	if (m_activeStop == -1)
		return QColor();

	return activeStop()->color;

}

void GradientPreview::setActiveStopPosition(double t)
{
	if (m_activeStop == -1)
		return;

	updateTmpStop(t);
	repaint();
}


void GradientPreview::setIsEditable(bool isEditable)
{
	m_isEditable = isEditable;
	repaint();
}

bool GradientPreview::isEditable()
{
	return m_isEditable;
}

void GradientPreview::setGradient(const VGradient &gradient)
{
	if ((gradient.colorStops().count() == fill_gradient.colorStops().count()) && (m_activeStop >= 0))
	{
		int diffStops = 0;
		for (int i = 0; i < fill_gradient.colorStops().count(); ++i)
		{
			VColorStop* stop1 = gradient.colorStops().at(i);
			VColorStop* stop2 = fill_gradient.colorStops().at(i);
			if ((stop1->color != stop2->color) || (stop1->midPoint != stop2->midPoint) ||
					(stop1->name  != stop2->name)  || (stop1->opacity != stop2->opacity)   ||
					(stop1->rampPoint != stop2->rampPoint) || (stop1->shade != stop2->shade))
			{
				++diffStops;
			}
		}
		if (diffStops > 1)
			m_activeStop = 0;
	}

	if ((m_activeStop < 0) && (gradient.colorStops().count() > 0))
		m_activeStop = 0;

	if (m_activeStop >= gradient.colorStops().count())
		m_activeStop = 0;

	fill_gradient = gradient;

	repaint();
}

const VGradient &GradientPreview::gradient()
{

	//    QList<VColorStop*> cstops = fill_gradient.colorStops();
	//    std::stable_sort(cstops.begin(), cstops.end(), compareStops);

	//    VGradient &grad(fill_gradient);
	//    grad.clearStops();

	//    foreach(VColorStop *stop, cstops){
	//        grad.addStop(*stop);
	//    }

	return fill_gradient;
}

void GradientPreview::updateDisplay()
{
	repaint();

	if (!fill_gradient.colorStops().isEmpty())
		emit selectedStop(activeStop());
}

/* ********************************************************************************* *
 *
 * Private Members
 *
 * ********************************************************************************* */

QRect GradientPreview::saveAreaRect()
{
	return QRect(RADIUS, 0, width() - RADIUS * 2 - 2, height() - 4);
}

QRect GradientPreview::canvasRect()
{
	return QRect(RADIUS, scaleRect().bottom(), width() - RADIUS * 2 - 2, GRADIENT_HEIGHT);
}

QRect GradientPreview::scaleRect()
{
	return QRect(RADIUS, 0, width() - RADIUS * 2 - 2, 6);
}

QRect GradientPreview::handleRect(int center)
{
	return QRect(center - RADIUS, canvasRect().bottom(), RADIUS * 2, saveAreaRect().bottom());
}

QRect GradientPreview::stopRect(int center)
{
	return QRect(center - RADIUS, saveAreaRect().top(), RADIUS * 2, saveAreaRect().bottom());
}

qreal GradientPreview::ratio()
{
	return saveAreaRect().width() / 100.0;
}

bool GradientPreview::isMouseOutside(QPoint mouse)
{
	return (mouse.y() <= rect().top() || mouse.y() >= rect().bottom());
}


VColorStop *GradientPreview::activeStop(int offset) const
{
	if(m_activeStop + offset < 0)
		return fill_gradient.colorStops().first();

	if(m_activeStop + offset >= fill_gradient.colorStops().count())
		return fill_gradient.colorStops().last();

	return fill_gradient.colorStops().at(m_activeStop + offset);
}

/*#include <algorithm>
#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPolygon>
#include <QToolTip>

#include "fpoint.h"
#include "gradientpreview.h"
#include "iconmanager.h"
#include "scpainter.h"


GradientPreview::GradientPreview(QWidget *pa) : QFrame(pa)
{
	setFrameShape( QFrame::Panel );
	setFrameShadow( QFrame::Sunken );
	setLineWidth( 2 );
	setMinimumSize(QSize(200, 70));
	setMaximumSize(QSize(3000, 70));
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);
	fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();

	QColor color;
	color = QColor(255,255,255);
	fill_gradient.addStop( color, 0.0, 0.5, 1.0 );
	color = QColor(0,0,0);
	fill_gradient.addStop( color, 1.0, 0.5, 1.0 );

	QList<VColorStop*> cstops = fill_gradient.colorStops();
	StopM.clear();
	for (int i = 0; i < fill_gradient.stops(); ++i)
	{
		int center = qRound(cstops.at(i)->rampPoint * (width() - 20)) + 10;
		StopM.append(center);
	}
} 

void GradientPreview::paintEvent(QPaintEvent *e)
{
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	StopM.clear();
	for (int i = 0; i < fill_gradient.stops(); ++i)
	{
		int center = qRound(cstops.at(i)->rampPoint * (width() - 20)) + 10;
		StopM.append(center);
	}
	QImage pixm(width() - 20, 37, QImage::Format_ARGB32_Premultiplied);
	QPainter pb;
	QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill.png"));
	pb.begin(&pixm);
	pb.fillRect(0, 0, pixm.width(), pixm.height(), b);
	pb.end();
	ScPainter *p = new ScPainter(&pixm, width() - 20, 37);
//	p->clear(Qt::white);
	p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setFillMode(ScPainter::Gradient);
	p->fill_gradient = fill_gradient;
	p->setGradient(VGradient::linear, FPoint(0, 20), FPoint(width() - 20, 20), FPoint(0, 0), 1.0, 0.0);
	p->drawRect(0, 0, width() - 20, 37);
	p->end();
	delete p;
	QPainter pw;
	pw.begin(this);
	pw.drawImage(10, 5, pixm);
	if (isEditable)
	{
		for (int i = 0; i < fill_gradient.stops(); ++i)
		{
			int center = qRound(cstops.at(i)->rampPoint * (width() - 20)) + 10;
			pw.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			if (StopM[qMax(ActStop,0)] == center)
				pw.setBrush(Qt::red);
			else
				pw.setBrush(Qt::blue);
			QPolygon cr;
			cr.setPoints(3, center, 43, center - 4, 56, center + 4, 56);
			pw.drawPolygon(cr);
		}
	}
	pw.end();
	QFrame::paintEvent(e);
}

void GradientPreview::keyPressEvent(QKeyEvent *e)
{
	if (isEditable)
	{
		if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
		{
			if ((ActStop > 0) && (ActStop != static_cast<int>(StopM.count()-1)))
			{
				onlyselect = false;
				fill_gradient.removeStop(ActStop);
				ActStop = 0;
				repaint();
				QList<VColorStop*> cstops = fill_gradient.colorStops();
				emit selectedStop(cstops.at(ActStop));
			}
		}
	}
}

void GradientPreview::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	Mpressed = true;
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));
	ActStop = -1;
	if (isEditable)
	{
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
		{
			fpo = QRect(static_cast<int>(StopM[yg]) - 4, 43, 8, 13);
			if (fpo.contains(m->pos()))
			{
				ActStop = yg;
				emit selectedStop(cstops.at(ActStop));
				repaint();
				onlyselect = true;
				return;
			}
		}
	}
}

void GradientPreview::mouseReleaseEvent(QMouseEvent *m)
{
	qApp->restoreOverrideCursor();
	QRect insideRect = QRect(10, 43, width() - 20, 13);
	if (isEditable)
	{
		QRect fpo;
		QPointF mousePos = m->position();
		if (m->button() == Qt::LeftButton)
		{
			if ((Mpressed) && (ActStop > 0) && (ActStop != static_cast<int>(StopM.count()-1)) && (outside || !insideRect.contains(m->pos())))
			{
				onlyselect = false;
				fill_gradient.removeStop(ActStop);
				ActStop = 0;
				repaint();
				QList<VColorStop*> cstops = fill_gradient.colorStops();
				emit selectedStop(cstops.at(ActStop));
			}
			if ((mousePos.y() < height()) && (mousePos.y() > 43) && (mousePos.x() > 0) && (mousePos.x() < width()) && (ActStop == -1))
			{
				QList<VColorStop*> cstops = fill_gradient.colorStops();
				double  newStop = static_cast<double>((mousePos.x() - 10)) / (static_cast<double>(width()) - 20);
				QColor  stopColor = (cstops.count() > 0) ? cstops.at(0)->color : QColor(255, 255, 255);
				QString stopName  = (cstops.count() > 0) ? cstops.at(0)->name  : QString("White");
				int     stopShade = (cstops.count() > 0) ? cstops.at(0)->shade : 100;
				fill_gradient.addStop(stopColor, newStop, 0.5, 1.0, stopName, stopShade);
				repaint();
				onlyselect = false;
				cstops = fill_gradient.colorStops();
				for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
				{
					fpo = QRect(static_cast<int>(StopM[yg]) - 4, 43, 8, 13);
					if (fpo.contains(m->pos()))
					{
						ActStop = yg;
						emit selectedStop(cstops.at(ActStop));
						repaint();
						break;
					}
				}
			}
		}
		else if (m->button() == Qt::RightButton)
		{
			Mpressed = false;
//			QList<VColorStop*> cstops = fill_gradient.colorStops();
			int stop = -1;
			for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
			{
				fpo = QRect(static_cast<int>(StopM[yg]) - 4, 43, 8, 13);
				if (fpo.contains(m->pos()))
				{
					stop = yg;
					break;
				}
			}
			contextStop = stop;
			mPos = m->pos();
			QMenu *pmen = new QMenu();
			setCursor(QCursor(Qt::ArrowCursor));
			pmen->addAction( tr("Add Stop"), this, SLOT(addStop()));
			if (stop != -1)
				pmen->addAction( tr("Remove Stop"), this, SLOT(removeStop()));
			pmen->exec(QCursor::pos());
			delete pmen;
		}
	}
	Mpressed = false;
	if ((!onlyselect) && (ActStop >= 0))
	{
		emit gradientChanged();
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		emit currStep(cstops.at(ActStop)->rampPoint);
	}
}

void GradientPreview::mouseMoveEvent(QMouseEvent *m)
{
	QRect insideRect = QRect(10, 43, width() - 20, 13);
	if (isEditable)
	{
		QRect fpo;
		QPointF mousePos = m->position();
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		if ((!Mpressed) && (mousePos.y() < height()) && (mousePos.y() > 43) && (mousePos.x() > 9) && (mousePos.x() < width()-9))
		{
			setCursor(IconManager::instance().loadCursor("AddPoint.png", 1, 1));
			for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
			{
				fpo = QRect(static_cast<int>(StopM[yg]) - 4, 43, 8, 13);
				if (fpo.contains(m->pos()))
				{
					setCursor(QCursor(Qt::SizeHorCursor));
					return;
				}
			}
		}
		if (m->buttons() & Qt::LeftButton)
		{
			if ((Mpressed) && (mousePos.y() < height()) && (mousePos.y() > 43) && (mousePos.x() > 9) && (mousePos.x() < width() - 9) && (ActStop != -1))
			{
				qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
				double newStop = static_cast<double>((mousePos.x() - 10)) / (static_cast<double>(width()) - 20);
				if (ActStop > 1)
				{
					if (StopM[ActStop-1]+2 >= mousePos.x())
						return;
				}
				if (ActStop < static_cast<int>(StopM.count()-2))
				{
					if (StopM[ActStop+1]-2 < mousePos.x())
						return;
				}
				StopM[ActStop] = mousePos.x();
				QList<VColorStop*> cstops = fill_gradient.colorStops();
				cstops.at(ActStop)->rampPoint = newStop;
				std::sort(cstops.begin(), cstops.end());
				onlyselect = false;
				repaint();
			}
			if ((Mpressed) && (outside || !insideRect.contains(m->pos())) && (ActStop > 0) && (ActStop != static_cast<int>(StopM.count()-1)))
				qApp->changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
		}
	}
}

void GradientPreview::leaveEvent(QEvent*)
{
	if (isEditable)
	{
		if (Mpressed)
		{
			if ((ActStop > 0) && (ActStop != static_cast<int>(StopM.count()-1)))
				qApp->changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
			else
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		}
		outside = true;
	}
}

void GradientPreview::enterEvent(QEnterEvent *)
{
	outside = false;
}

void GradientPreview::addStop()
{
	QList<VColorStop*> cstops = fill_gradient.colorStops();
	double  newStop = static_cast<double>((mPos.x() - 10)) / (static_cast<double>(width()) - 20);
	QColor  stopColor = (cstops.count() > 0) ? cstops.at(0)->color : QColor(255, 255, 255);
	QString stopName  = (cstops.count() > 0) ? cstops.at(0)->name  : QString("White");
	int     stopShade = (cstops.count() > 0) ? cstops.at(0)->shade : 100;
	fill_gradient.addStop(stopColor, newStop, 0.5, 1.0, stopName, stopShade);
	repaint();
	onlyselect = false;
	cstops = fill_gradient.colorStops();
	for (int yg = 0; yg < static_cast<int>(StopM.count()); ++yg)
	{
		QRect fpo = QRect(static_cast<int>(StopM[yg]) - 4, 43, 8, 13);
		if (fpo.contains(mPos))
		{
			ActStop = yg;
			emit selectedStop(cstops.at(ActStop));
			repaint();
			break;
		}
	}
}

void GradientPreview::removeStop()
{
	if ((contextStop > 0) && (contextStop != static_cast<int>(StopM.count()-1)))
	{
		onlyselect = false;
		fill_gradient.removeStop(contextStop);
		ActStop = 0;
		repaint();
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		emit selectedStop(cstops.at(ActStop));
	}
}

void GradientPreview::updateDisplay()
{
	repaint();
	if (!fill_gradient.colorStops().isEmpty())
	{
		QList<VColorStop*> cstops = fill_gradient.colorStops();
		emit selectedStop(cstops.at(ActStop));
	}
}

void GradientPreview::setActColor(const QColor& c, const QString& n, int s)
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

void GradientPreview::setGradient(const VGradient& gradient)
{
	if ((gradient.colorStops().count() == fill_gradient.colorStops().count()) && (ActStop >= 0))
	{
		int diffStops = 0;
		for (int i = 0; i < fill_gradient.colorStops().count(); ++i)
		{
			VColorStop* stop1 = gradient.colorStops().at(i);
			VColorStop* stop2 = fill_gradient.colorStops().at(i);
			if ((stop1->color != stop2->color) || (stop1->midPoint != stop2->midPoint) ||
				(stop1->name  != stop2->name)  || (stop1->opacity != stop2->opacity)   ||
				(stop1->rampPoint != stop2->rampPoint) || (stop1->shade != stop2->shade))
			{
				++diffStops;
			}
		}
		if (diffStops > 1)
			ActStop = 0;
	}
	if ((ActStop < 0) && (gradient.colorStops().count() > 0))
		ActStop = 0;
	if (ActStop >= gradient.colorStops().count())
		ActStop = 0;
	fill_gradient = gradient;
}

void GradientPreview::setGradientEditable(bool val)
{
	isEditable = val;
	repaint();
}
*/
