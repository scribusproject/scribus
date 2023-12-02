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

#include "iconmanager.h"
#include "util_gui.h"

/* ********************************************************************************* *
 *
 *
 * Dash Preview
 *
 *
 * ********************************************************************************* */

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

const int RADIUS = 6;
const double HAIRLINE = 0.001;

DashPreview::DashPreview(QWidget *pa) : QFrame(pa)
{
	int h = scaleRect().height() + canvasRect().height() + RADIUS * 3;

	setMinimumSize(QSize(200, h));
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);

	m_dashValues.clear();
	m_dashValues.append(4.0);
	m_dashValues.append(2.0);
} 

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void DashPreview::paintEvent(QPaintEvent *e)
{
	int pWidth = canvasRect().width();
	int pHeight = canvasRect().height();
	QImage pixm(pWidth, pHeight, QImage::Format_ARGB32_Premultiplied);
	QColor cHandle(palette().color(QPalette::WindowText));

	// Render line pattern
	QPainter *p = new QPainter(&pixm);
	p->fillRect(pixm.rect(), QColor(128, 128, 128));
	p->setPen(Qt::NoPen);
	int startX = 0;
	for (int i = 0; i < m_stops.count(); ++i)
	{
		if (i % 2 == 0)
			p->setBrush(Qt::black);
		else
			p->setBrush(Qt::white);

		int xPos = mapPositionToGrid(m_stops[i]);

		p->drawRect(startX, 0, xPos - startX, pHeight);
		startX = xPos;
	}
	p->end();
	delete p;


	QPainter pw;
	pw.begin(this);
	pw.setRenderHint(QPainter::Antialiasing, true);

	// Draw Scale
	QFont scaleFont(this->font());
	scaleFont.setPointSize(6);
	pw.setPen(QPen(palette().color(QPalette::WindowText)));

	//	constexpr int topline = 3;
	//	constexpr int bottomline = 15;
	//	constexpr int rulerheight = (bottomline - topline);
	//	constexpr int midline = (topline + rulerheight / 2);

	for (int i = 0; i < m_patternLength * 10; i++)
	{
		double xPos = mapPositionToGrid(ratio() / 10 * i) + saveAreaRect().left() + .5;

		if(i % 10 == 0 )
		{
			pw.drawLine(QPointF(xPos, scaleRect().center().y() +.5), QPointF(xPos, scaleRect().bottom() + .5));
			if(ratio() >= 12)
			{
				pw.setFont(scaleFont);
				pw.drawText(QPointF(xPos + 2, scaleRect().top() + 7), QString::number(i/10));
			}
		}
		else if( (i % 2 == 1 && ratio() >= 20) || (i % 2 == 0 && ratio() >= 16) )
		{
			pw.drawLine(QPointF(xPos, scaleRect().bottom() - 2.5), QPointF(xPos, scaleRect().bottom() + .5));
		}
	}

	// Draw Pattern
	pw.drawImage(canvasRect(), pixm);
	pw.setPen(palette().color(QPalette::Midlight));
	pw.setBrush(Qt::NoBrush);
	pw.drawRect(QRectF(canvasRect()).adjusted(.5, .5, -.5, -.5));

	// Draw handles
	for (int i = 0; i < m_stops.count(); ++i)
	{
		qreal sPos = m_stops[i];
		qreal yPos = handleRect(sPos).top() + 0.5;
		qreal xPos = mapPositionToGrid(sPos) + saveAreaRect().left() + .5;

		// Draw Step
		if(m_currentStop == i)
		{
			QPainterPath marker;
			marker.moveTo( QPointF(xPos - 5, canvasRect().top() - 5.5) );
			marker.lineTo( QPointF(xPos + 5, canvasRect().top() - 5.5) );
			marker.lineTo( QPointF(xPos, canvasRect().top()) );
			marker.closeSubpath();

			pw.setPen(QPen(palette().color(QPalette::Window), 1));
			pw.setBrush(palette().color(QPalette::Highlight));
			pw.drawPath(marker);

			cHandle = palette().color(QPalette::Highlight);
		}
		else
		{
			// Set error color for last handle
			if( (m_dashValues.count() % 2 != 0) && (i == m_stops.count() - 1) )
				cHandle = errorColor();
			else
				cHandle = palette().color(QPalette::WindowText);
		}

		renderPointerHandle(&pw, QPointF(xPos, yPos), RADIUS * 2 - 2, cHandle );

	}

	pw.end();

	QFrame::paintEvent(e);
}

void DashPreview::mousePressEvent(QMouseEvent *m)
{
	m_moveTimer.start();
	m_mousePressed = true;
	m_currentStop = -1;

	m->accept();
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));

	for (int i = 0; i < m_stops.count(); ++i)
	{
		if (handleRect(static_cast<int>(m_stops[i])).contains(m->pos()))
		{
			m_currentStop = i;
			emit currStep(m_dashValues[m_currentStop]);
			update();
			return;
		}
	}
}

void DashPreview::mouseReleaseEvent(QMouseEvent *m)
{
	m->accept();
	QApplication::restoreOverrideCursor();
	QPointF mousePos = m->position();

	// drag & outside
	if ((m_mousePressed) && (m_stops.count() > 2) && (m_outside || mousePos.y() > saveAreaRect().height()))
	{
		m_stops.removeAt(m_currentStop);
		updateDashValues();
		m_currentStop = 0;
		m_mousePressed = false;
		update();
		emit currStep(m_dashValues[m_currentStop]);
		emit dashChanged();
		return;
	}

	if (m_currentStop != -1)
	{
		if (m_moveTimer.elapsed() < 250)
		{
			m_mousePressed = false;
			return;
		}
		m_stops[m_currentStop] = clampStopPosition(mousePos.x() - canvasRect().left());
	}
	else
	{
		if (m_dashValues.count() < 10)
			m_stops.append(mousePos.x() - canvasRect().left());

		// sort stops
		std::sort(m_stops.begin(), m_stops.end());
		m_currentStop = 0;
		for (int i = 0; i < m_stops.count(); ++i)
		{
			if (handleRect(static_cast<int>(m_stops[i])).contains(m->pos()))
			{
				m_currentStop = i;
				break;
			}
		}
	}
	updateDashValues();
	update();
	emit currStep(m_dashValues[m_currentStop]);
	emit dashChanged();

	m_mousePressed = false;
}

void DashPreview::mouseMoveEvent(QMouseEvent *m)
{
	m->accept();

	QPointF mousePos = m->position();

	// hover
	if ((!m_mousePressed) && saveAreaRect().contains(mousePos.toPoint()) && (m_dashValues.count() < 10))
	{
		setCursor(IconManager::instance().loadCursor("AddPoint.png", 1, 1));
		for (int i = 0; i < m_stops.count(); ++i)
		{
			if (handleRect(static_cast<int>(m_stops[i])).contains(m->pos()))
			{
				setCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
		}
	}

	// drag
	if ((m_mousePressed) && saveAreaRect().contains(mousePos.toPoint()) && (m_currentStop != -1))
	{
		QApplication::changeOverrideCursor(QCursor(Qt::SizeHorCursor));

		if (m_currentStop > 1)
		{
			if (static_cast<int>(m_stops[m_currentStop - 1] + canvasRect().left()) + 2 >= mousePos.x())
				return;
		}

		if (m_currentStop < static_cast<int>(m_stops.count() - 2))
		{
			if (static_cast<int>(m_stops[m_currentStop + 1] + canvasRect().left()) - 2 < mousePos.x())
				return;
		}

		m_stops[m_currentStop] = clampStopPosition(mousePos.x() - canvasRect().left());
		updateDashValues();
		update();
		double startX = 0.0;

		for (int i = 0; i < m_currentStop; ++i)
		{
			startX += m_stops[i] / ratio() - startX;
		}

		emit currStep(m_stops[m_currentStop] / ratio() - startX);
	}

	if ((m_mousePressed) && (m_outside || mousePos.y() > saveAreaRect().height()) && (m_currentStop >= 0) && (m_stops.count() > 2))
		QApplication::changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
}

void DashPreview::leaveEvent(QEvent*)
{
	if (m_mousePressed)
	{
		if ((m_currentStop >= 0) && (m_stops.count() > 2))
			QApplication::changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
		else
			QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	m_outside = true;
}

void DashPreview::enterEvent(QEnterEvent*)
{
	m_outside = false;
}

void DashPreview::resizeEvent(QResizeEvent * e)
{
	updateStops();
	QFrame::resizeEvent(e);
}

/* ********************************************************************************* *
 *
 * Properties
 *
 * ********************************************************************************* */

void DashPreview::setDashValues(const QVector<double>& vals)
{
	m_dashValues = vals;
	if ((m_currentStop >= vals.count()) || (m_currentStop == -1))
		m_currentStop = 0;

	if (m_dashValues.count() != 0)
	{
		m_patternLength = 0;
		for(int i = 0; i < m_dashValues.count(); i++)
			m_patternLength += m_dashValues[i];
		m_patternLength += 1;

		if ((m_currentStop >= vals.count()) || (m_currentStop == -1))
			emit currStep(m_dashValues[0]);
		else
			emit currStep(m_dashValues[m_currentStop]);
	}
	updateStops();
	update();
}

QRect DashPreview::saveAreaRect()
{
	return QRect(RADIUS, 0, width() - RADIUS * 2 - 2, height() - 4);
}

QRect DashPreview::canvasRect()
{
	return QRect(RADIUS, 14, width() - RADIUS * 2 - 2, 10);
}

QRect DashPreview::scaleRect()
{
	return QRect(RADIUS, 0, width() - RADIUS * 2 - 2, 14);
}

QRect DashPreview::handleRect(int offset)
{
	return QRect(offset, canvasRect().bottom(), RADIUS * 2, saveAreaRect().bottom());
}

qreal DashPreview::ratio()
{
	return saveAreaRect().width() / m_patternLength;
}

QColor DashPreview::errorColor()
{
	if(isEnabled())
		return QColor(229, 85, 69);
	else
		return QColor(palette().color(QPalette::WindowText));
}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

qreal DashPreview::clampStopPosition(qreal xPos)
{
	// position of left neighbor + .1
	qreal xMin = m_currentStop != 0 ? m_stops[m_currentStop - 1] + .1 : 0;
	// position of right neighbor - .1
	qreal xMax = m_currentStop != m_stops.count() - 1 ? m_stops[m_currentStop + 1] -.1 : canvasRect().right();

	return qBound(xMin, xPos, xMax);
}

int DashPreview::mapPositionToGrid(qreal pos)
{
	qreal steps = ratio() / 10.0;
	return static_cast<int>(pos / steps + .5) * steps;
}

void DashPreview::updateDashValues()
{
	m_dashValues.clear();
	double startX = 0.0;
	for (int i = 0; i < m_stops.count(); ++i)
	{
		double w = m_stops[i] / ratio() - startX;
		m_dashValues.append(w);
		startX += w;
	}
}

void DashPreview::updateStops()
{
	m_stops.clear();
	double startX = 0.0;
	for (int i = 0; i < m_dashValues.count(); ++i)
	{
		startX += m_dashValues[i] * ratio();
		m_stops.append(startX);
	}
}

/* ********************************************************************************* *
 *
 * Public Slots
 *
 * ********************************************************************************* */

void DashPreview::setActStep(double t)
{
	if (m_currentStop == -1)
		return;
	m_dashValues[m_currentStop] = t;
	updateStops();
	update();
	emit dashChanged();
}



/* ********************************************************************************* *
 *
 *
 * Dash Editor
 *
 *
 * ********************************************************************************* */

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

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

/* ********************************************************************************* *
 *
 * Properties
 *
 * ********************************************************************************* */

void DashEditor::setPos(double p)
{
	QSignalBlocker blocker(Position);
	Position->setValue(p);
}

void DashEditor::setDashValues(QVector<double> vals, double linewidth, double offset)
{
	// We don't need to down scale the dash values for hairlines.
	isHairline = linewidth <= HAIRLINE;
	double hairlineWidth = isHairline ? 1.0 : linewidth;

	QVector<double> tmp;
	for (int i = 0; i < vals.count(); i++)
		tmp.append(vals[i] / hairlineWidth);
	Preview->setDashValues(tmp);

	QSignalBlocker sig(Offset);
	Offset->setValue(offset / linewidth);

}

QVector<double> DashEditor::getDashValues(double linewidth)
{
	double hairlineWidth = isHairline ? 1.0 : linewidth;

	auto dashValues = Preview->dashValues();
	for (int i = 0; i < dashValues.count(); i++)
		dashValues[i] *= hairlineWidth;
	return dashValues;
}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void DashEditor::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void DashEditor::languageChange()
{
	Desc->setText( tr( "Value:" ) );
	Desc2->setText( tr( "Offset:" ) );
}
