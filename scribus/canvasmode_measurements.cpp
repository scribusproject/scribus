/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "canvasmode_measurements.h"

#include <QApplication>
#include <QPainter>
#include <QPen>

#include "canvas.h"
#include "measurements.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "util_math.h"

MeasurementsMode::MeasurementsMode(ScribusView* view) : CanvasMode(view), m_start(0,0), m_current(0,0)
{
	m_palette = new Measurements(m_view);
	m_palette->startup();
	m_palette->hide();
	connect( m_palette, SIGNAL(paletteShown(bool)), this, SLOT(setActive(bool)));
	m_active = false;
	m_startDoc = FPoint(0,0);
	m_currentDoc = FPoint(0,0);
}

void MeasurementsMode::enterEvent(QEvent *)
{
	qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
}

void MeasurementsMode::leaveEvent(QEvent *)
{
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void MeasurementsMode::setActive(bool active)
{
	if (!active)
		m_view->requestMode(modeNormal);
}


void MeasurementsMode::activate(bool)
{
	m_palette->show();
}

void MeasurementsMode::deactivate(bool)
{
	m_palette->hide();
}

void MeasurementsMode::drawControls(QPainter* p)
{
	p->save();
	p->setRenderHint(QPainter::Antialiasing);
	p->setBrush(Qt::NoBrush);
	p->setPen(QPen(Qt::black, 1.0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	p->drawLine(m_start, m_current);
	p->restore();
}

void MeasurementsMode::mouseReleaseEvent(QMouseEvent *m)
{
	if (m_active)
		adjustPoint(m->pos());		
	m_active = false;
	showValues();
}

void MeasurementsMode::mouseMoveEvent(QMouseEvent *m)
{
	if (m_active)
	{
		adjustPoint(m->pos());
		showValues();
	}
}

void MeasurementsMode::mousePressEvent(QMouseEvent *m)
{
	m_active = true;
	adjustPoint(m->pos());
	m_start = m_current;
	m_startDoc = m_currentDoc;
	showValues();
}

void MeasurementsMode::adjustPoint(QPoint point)
{
	m_canvas->update(QRect(m_start, m_current).normalized().adjusted(-1,-1,1,1));
	m_current = point;
	m_currentDoc = m_canvas->localToCanvas(m_current) - FPoint(m_doc->currentPage()->xOffset(),m_doc->currentPage()->yOffset());
}

void MeasurementsMode::showValues()
{
	double dx = m_currentDoc.x() - m_startDoc.x();
	double dy = m_currentDoc.y() - m_startDoc.y();
	m_palette->setValues(m_startDoc.x(), m_startDoc.y(), m_currentDoc.x(), m_currentDoc.y(), 
						 -xy2Deg(dx, dy), sqrt(dx*dx + dy*dy), m_doc->unitIndex());
	m_canvas->update(QRect(m_start, m_current).normalized());
}
