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

#include "canvasgesture_columnresize.h"

void ColumnResize::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		// Cancel the resize.
		event->accept();
		m_view->stopGesture();
	}
	if (event->key() == Qt::Key_Shift)
	{
		// Re-copy column geometires since resizing behavior will change.
		m_columnWidths = table()->columnWidths();
		m_columnPositions = table()->columnPositions();
	}
}

void ColumnResize::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Shift)
	{
		// Re-copy column geometires since resizing behavior will change.
		m_columnWidths = table()->columnWidths();
		m_columnPositions = table()->columnPositions();
	}
}

void ColumnResize::mouseReleaseEvent(QMouseEvent* event)
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
	table()->resizeColumn(m_column, gridPoint.x() - table()->columnPosition(m_column), strategy);
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

void ColumnResize::mouseMoveEvent(QMouseEvent* event)
{
	event->accept();

	QPointF gridPoint = globalToTableGrid(event->globalPos());

	double requestedWidth = gridPoint.x() - m_columnPositions[m_column];
	double actualWidth = 0.0;

	if (event->modifiers() & Qt::ShiftModifier)
		actualWidth = resizeColumnMoveFollowing(requestedWidth);
	else
		actualWidth = resizeColumnResizeFollowing(requestedWidth);

	// Display width tooltip.
	m_canvas->displayDoubleHUD(event->globalPos(), tr("Width"), actualWidth);

	// Update canvas.
	m_canvas->update();
}

void ColumnResize::drawControls(QPainter* p)
{
	p->save();
	commonDrawControls(p, false);
	p->restore();

	// Paint the table outline using the changed column geometries.
	paintTableOutline(table()->rowHeights(), table()->rowPositions(),
		m_columnWidths, m_columnPositions, p);
}

void ColumnResize::setup(PageItem_Table* table, int column)
{
	Q_ASSERT(table);
	Q_ASSERT(column >= 0 && column < table->columns());

	setTable(table);
	m_column = column;

	// Make copies of the column geometries to be used during resize.
	m_columnWidths = table->columnWidths();
	m_columnPositions = table->columnPositions();
}

double ColumnResize::resizeColumnMoveFollowing(double width)
{
	// Set column width.
	double newWidth = m_columnWidths[m_column] = qMax(PageItem_Table::MinimumColumnWidth, width);

	// Move following columns.
	double columnPosition = m_columnPositions[m_column];
	for (int column = m_column; column < m_columnPositions.size(); ++column)
	{
		m_columnPositions[column] = columnPosition;
		columnPosition += m_columnWidths[column];
	}

	return newWidth;
}

double ColumnResize::resizeColumnResizeFollowing(double width)
{
	double oldWidth = m_columnWidths[m_column];
	double newWidth = 0.0;

	if (m_column < table()->columns() - 1)
	{
		// Following column exists, so width is bounded at both ends.
		newWidth = m_columnWidths[m_column] = qBound(
			PageItem_Table::MinimumColumnWidth, width,
			oldWidth + m_columnWidths[m_column + 1] - PageItem_Table::MinimumColumnWidth);

		// Resize/move following column.
		double widthChange = newWidth - oldWidth;
		m_columnPositions[m_column + 1] += widthChange;
		m_columnWidths[m_column + 1] -= widthChange;
	}
	else
	{
		// Last column, so width only bounded by MinimumColumnWidth.
		newWidth = m_columnWidths[m_column] = qMax(PageItem_Table::MinimumColumnWidth, width);
	}

	return newWidth;
}
