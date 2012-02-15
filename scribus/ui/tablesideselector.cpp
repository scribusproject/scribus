/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QColor>
#include <QDebug>
#include <QFrame>
#include <QLineF>
#include <QList>
#include <QMouseEvent>
#include <QPainter>
#include <QPair>
#include <QPen>
#include <QPointF>
#include <QRectF>

#include "tablesideselector.h"

class QEvent;

TableSideSelector::TableSideSelector(QWidget* parent) : QFrame(parent)
{
	setSelection(All);
	setStyle(TableStyle);
	setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Sunken);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMouseTracking(true);
}

QList<TableSideSelector::Side> TableSideSelector::selectionList() const
{
	QList<Side> sides;
	if (m_selection & Left)
		sides.append(Left);
	if (m_selection & Right)
		sides.append(Right);
	if (m_selection & Top)
		sides.append(Top);
	if (m_selection & Bottom)
		sides.append(Bottom);
	return sides;
}

void TableSideSelector::paintEvent(QPaintEvent* event)
{
	double edgeWidth = 5;
	double inset = edgeWidth/2 + frameWidth()*2;

	QPointF topLeft(inset, inset);
	QPointF topRight(width() - inset, inset);
	QPointF bottomLeft(inset, height() - inset);
	QPointF bottomRight(width() - inset, height() - inset);

	m_left = QLineF(topLeft, bottomLeft);
	m_right = QLineF(topRight, bottomRight);
	m_top = QLineF(topRight, topLeft);
	m_bottom = QLineF(bottomRight, bottomLeft);

	QPainter painter(this);

	// Paint selection.
	painter.setPen(QPen(Qt::black, edgeWidth, Qt::SolidLine, Qt::RoundCap));
	if (m_selection & Left) painter.drawLine(m_left);
	if (m_selection & Right) painter.drawLine(m_right);
	if (m_selection & Top) painter.drawLine(m_top);
	if (m_selection & Bottom) painter.drawLine(m_bottom);

	// Paint the dotted grid.
	painter.setPen(QPen(Qt::gray, edgeWidth/4, Qt::DotLine));
	painter.drawRect(QRectF(topLeft, bottomRight));
	painter.drawLine(m_left.pointAt(0.5), m_right.pointAt(0.5));
	painter.drawLine(m_top.pointAt(0.5), m_bottom.pointAt(0.5));

	// Paint highlighting.
	painter.setPen(QPen(QColor(255, 255, 255, 60), edgeWidth, Qt::SolidLine, Qt::RoundCap));
	switch (m_highlighted)
	{
	case Left:
		painter.drawLine(m_left);
		break;
	case Right:
		painter.drawLine(m_right);
		break;
	case Top:
		painter.drawLine(m_top);
		break;
	case Bottom:
		painter.drawLine(m_bottom);
		break;
	default:
		break;
	}
}

void TableSideSelector::mousePressEvent(QMouseEvent* event)
{
	m_selection ^= closestSide(event->posF());
	update();
	emit selectionChanged();
}

void TableSideSelector::mouseMoveEvent(QMouseEvent* event)
{
	m_highlighted = closestSide(event->posF());
	update();
}

void TableSideSelector::leaveEvent(QEvent* event)
{
	Q_UNUSED(event);
	m_highlighted = None;
	update();
}

TableSideSelector::Side TableSideSelector::closestSide(const QPointF& point) const
{
	QList<QPair<double, Side> > distances;
	distances.append(QPair<double, Side>(QLineF(point, m_left.pointAt(0.5)).length(), Left));
	distances.append(QPair<double, Side>(QLineF(point, m_right.pointAt(0.5)).length(), Right));
	distances.append(QPair<double, Side>(QLineF(point, m_top.pointAt(0.5)).length(), Top));
	distances.append(QPair<double, Side>(QLineF(point, m_bottom.pointAt(0.5)).length(), Bottom));
	qSort(distances);

	return distances.first().second;
}
