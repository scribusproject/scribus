/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <algorithm>

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
	setSelection(TableSide::All);
	setStyle(TableStyle);
	setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Sunken);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMouseTracking(true);
}

QList<TableSide> TableSideSelector::selectionList() const
{
	QList<TableSide> sides;
	if (m_selection & TableSide::Left)
		sides.append(TableSide::Left);
	if (m_selection & TableSide::Right)
		sides.append(TableSide::Right);
	if (m_selection & TableSide::Top)
		sides.append(TableSide::Top);
	if (m_selection & TableSide::Bottom)
		sides.append(TableSide::Bottom);
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
	if (m_selection & TableSide::Left) painter.drawLine(m_left);
	if (m_selection & TableSide::Right) painter.drawLine(m_right);
	if (m_selection & TableSide::Top) painter.drawLine(m_top);
	if (m_selection & TableSide::Bottom) painter.drawLine(m_bottom);

	// Paint the dotted grid.
	painter.setPen(QPen(Qt::gray, edgeWidth/4, Qt::DotLine));
	painter.drawRect(QRectF(topLeft, bottomRight));
	painter.drawLine(m_left.pointAt(0.5), m_right.pointAt(0.5));
	painter.drawLine(m_top.pointAt(0.5), m_bottom.pointAt(0.5));

	// Paint highlighting.
	painter.setPen(QPen(QColor(255, 255, 255, 60), edgeWidth, Qt::SolidLine, Qt::RoundCap));
	switch (m_highlighted)
	{
	case TableSide::Left:
		painter.drawLine(m_left);
		break;
	case TableSide::Right:
		painter.drawLine(m_right);
		break;
	case TableSide::Top:
		painter.drawLine(m_top);
		break;
	case TableSide::Bottom:
		painter.drawLine(m_bottom);
		break;
	default:
		break;
	}
}

void TableSideSelector::mousePressEvent(QMouseEvent* event)
{
	m_selection ^= closestSide(event->localPos());
	update();
	emit selectionChanged();
}

void TableSideSelector::mouseMoveEvent(QMouseEvent* event)
{
	m_highlighted = closestSide(event->localPos());
	update();
}

void TableSideSelector::leaveEvent(QEvent* event)
{
	Q_UNUSED(event);
	m_highlighted = TableSide::None;
	update();
}

TableSide TableSideSelector::closestSide(const QPointF& point) const
{
	QList<QPair<double, TableSide> > distances;
	distances.append(QPair<double, TableSide>(QLineF(point, m_left.pointAt(0.5)).length(), TableSide::Left));
	distances.append(QPair<double, TableSide>(QLineF(point, m_right.pointAt(0.5)).length(), TableSide::Right));
	distances.append(QPair<double, TableSide>(QLineF(point, m_top.pointAt(0.5)).length(), TableSide::Top));
	distances.append(QPair<double, TableSide>(QLineF(point, m_bottom.pointAt(0.5)).length(), TableSide::Bottom));
	std::sort(distances.begin(), distances.end());

	return distances.first().second;
}
