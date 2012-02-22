/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>

#include "canvas.h"
#include "fpoint.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "scribusview.h"

#include "canvasgesture_rowresize.h"

void RowResize::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		// Cancel the resize.
		event->accept();
		m_view->stopGesture();
	}
	if (event->key() == Qt::Key_Shift)
	{
		// Re-copy row geometires since resizing behavior will change.
		m_rowHeights = table()->rowHeights();
		m_rowPositions = table()->rowPositions();
	}
}

void RowResize::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Shift)
	{
		// Re-copy row geometires since resizing behavior will change.
		m_rowHeights = table()->rowHeights();
		m_rowPositions = table()->rowPositions();
	}
}

void RowResize::mouseReleaseEvent(QMouseEvent* event)
{
	event->accept();

	QPointF gridPoint = globalToTableGrid(event->globalPos());

	// Perform the actual resize of the row.
	PageItem_Table::ResizeStrategy strategy;
	if (event->modifiers() & Qt::ShiftModifier)
		strategy = PageItem_Table::MoveFollowing;
	else
		strategy = PageItem_Table::ResizeFollowing;

	table()->doc()->dontResize = true;
	table()->resizeRow(m_row, gridPoint.y() - table()->rowPosition(m_row), strategy);
	if (strategy == PageItem_Table::MoveFollowing)
	{
		table()->adjustTableToFrame();
		table()->adjustFrameToTable();
	}
	table()->doc()->dontResize = false;
	table()->doc()->setRedrawBounding(table());
	table()->update();

	m_view->stopGesture();
}

void RowResize::mouseMoveEvent(QMouseEvent* event)
{
	event->accept();

	QPointF gridPoint = globalToTableGrid(event->globalPos());

	double requestedHeight = gridPoint.y() - m_rowPositions[m_row];
	double actualHeight = 0.0;

	if (event->modifiers() & Qt::ShiftModifier)
		actualHeight = resizeRowMoveFollowing(requestedHeight);
	else
		actualHeight = resizeRowResizeFollowing(requestedHeight);

	// Display height tooltip.
	m_canvas->displayDoubleHUD(event->globalPos(), tr("Height"), actualHeight);

	// Update canvas.
	m_canvas->update();
}

void RowResize::drawControls(QPainter* p)
{
	p->save();
	commonDrawControls(p, false);
	p->restore();

	// Paint the table outline using the changed row geometries.
	paintTableOutline(m_rowHeights, m_rowPositions,
		table()->columnWidths(), table()->columnPositions(), p);
}

void RowResize::setup(PageItem_Table* table, int row)
{
	Q_ASSERT(table);
	Q_ASSERT(row >= 0 && row < table->rows());

	setTable(table);
	m_row = row;

	// Make copies of the row geometries to be used during resize.
	m_rowHeights = table->rowHeights();
	m_rowPositions = table->rowPositions();
}

double RowResize::resizeRowMoveFollowing(double height)
{
	// Set row height.
	double newHeight = m_rowHeights[m_row] = qMax(PageItem_Table::MinimumRowHeight, height);

	// Move following rows.
	double rowPosition = m_rowPositions[m_row];
	for (int row = m_row; row < m_rowPositions.size(); ++row)
	{
		m_rowPositions[row] = rowPosition;
		rowPosition += m_rowHeights[row];
	}

	return newHeight;
}

double RowResize::resizeRowResizeFollowing(double height)
{
	double oldHeight = m_rowHeights[m_row];
	double newHeight = 0.0;

	if (m_row < table()->rows() - 1)
	{
		// Following row exists, so height is bounded at both ends.
		newHeight = m_rowHeights[m_row] = qBound(
			PageItem_Table::MinimumRowHeight, height,
			oldHeight + m_rowHeights[m_row + 1] - PageItem_Table::MinimumRowHeight);

		// Resize/move following row.
		double heightChange = newHeight - oldHeight;
		m_rowPositions[m_row + 1] += heightChange;
		m_rowHeights[m_row + 1] -= heightChange;
	}
	else
	{
		// Last row, so height only bounded by MinimumRowHeight.
		newHeight = m_rowHeights[m_row] = qMax(PageItem_Table::MinimumRowHeight, height);
	}

	return newHeight;
}
