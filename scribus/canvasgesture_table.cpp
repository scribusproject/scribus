/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QTransform>

#include "canvas.h"
#include "fpoint.h"
#include "pageitem_table.h"
#include "scribusdoc.h"
#include "tablecell.h"

#include "canvasgesture_table.h"

QPointF TableGesture::globalToTableGrid(const QPoint point, bool snap)
{
	FPoint canvasPoint = m_canvas->globalToCanvas(point);

	if (snap)
	{
		// Snap to grid and guides.
		canvasPoint = m_doc->ApplyGridF(canvasPoint);
		m_doc->ApplyGuides(&canvasPoint);
	}

	// Return point mapped to table grid.
	return m_table->getTransform().inverted().map(canvasPoint.toQPointF()) - m_table->gridOffset();
}

void TableGesture::paintTableOutline(
	const QList<qreal>& rowHeights, const QList<qreal>& rowPositions,
	const QList<qreal>& columnWidths, const QList<qreal>& columnPositions, QPainter* p)
{
	if (!m_table || !m_canvas || !p)
		return;

	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	p->setTransform(m_table->getTransform(), true);
	p->setRenderHint(QPainter::Antialiasing);
	p->setPen(QPen(QColor(100, 200, 255), 3.0 / m_canvas->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

	QPointF offset = m_table->gridOffset();

	// First draw a rect around the table.
	p->drawRect(QRectF(offset.x(), offset.y(), columnPositions.last() + columnWidths.last(),
		rowPositions.last() + rowHeights.last()));

	/**
	 * cellAt() and cellRect() are fast, stroking is not. So we paint the
	 * left and top edges of the cells in two passes as this allows us to
	 * greatly minimize the number of strokes.
	 */

	TableCell cell;

	// Paint left edge(s) in each of the columns.
	for (int col = 1; col < m_table->columns(); ++col)
	{
		QPointF startPoint(columnPositions[col], 0.0);
		QPointF endPoint(startPoint);
		for (int row = 0; row < m_table->rows(); row += cell.rowSpan())
		{
			cell = m_table->cellAt(row, col);
			int endRow = cell.row() + cell.rowSpan() - 1;
			qreal bottom = rowPositions[endRow] + rowHeights[endRow];

			if (cell.column() == col)
				endPoint.setY(bottom);
			else
			{
				p->drawLine(startPoint + offset, endPoint + offset);
				startPoint.setY(bottom);
				endPoint.setY(bottom);
			}
		}
		if (endPoint.y() > startPoint.y())
			p->drawLine(startPoint + offset, endPoint + offset);
	}

	// Paint top edge(s) in each of the rows.
	for (int row = 1; row < m_table->rows(); ++row)
	{
		QPointF startPoint(0.0, rowPositions[row]);
		QPointF endPoint(startPoint);
		for (int col = 0; col < m_table->columns(); col += cell.columnSpan())
		{
			cell = m_table->cellAt(row, col);
			int endCol = cell.column() + cell.columnSpan() - 1;
			qreal right = columnPositions[endCol] + columnWidths[endCol];

			if (cell.row() == row)
				endPoint.setX(right);
			else
			{
				p->drawLine(startPoint + offset, endPoint + offset);
				startPoint.setX(right);
				endPoint.setX(right);
			}
		}
		if (endPoint.x() > startPoint.x())
			p->drawLine(startPoint + offset, endPoint + offset);
	}

	p->restore();
}

void TableGesture::paintCellSelection(QPainter* p)
{
	if (!m_table || !m_canvas || !p)
		return;

	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	p->setTransform(m_table->getTransform(), true);
	p->setRenderHint(QPainter::Antialiasing);
	p->setPen(QPen(QColor(100, 200, 255), 3.0 / m_canvas->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p->setBrush(QColor(100, 200, 255, 50));

	/*
	 * The code below makes selectionPath a union of the cell rectangles of the selected cells.
	 * Since the cell rectangles are adjacent, they must be expanded slightly (1.0) for the
	 * uniting to work. This may not be the fastest way to compose the path of the selection,
	 * but it makes for some very simple code. And the result looks good.
	 */

	const QPointF offset = m_table->gridOffset();
	QPainterPath selectionPath;

	foreach (const TableCell& cell, m_table->selectedCells())
	{
		QRectF cellRect = cell.boundingRect();
		cellRect.translate(offset);
		cellRect.adjust(-1.0, -1.0, 1.0, 1.0);
		QPainterPath cellPath;
		cellPath.addRect(cellRect);
		selectionPath = selectionPath.united(cellPath);
	}

	p->drawPath(selectionPath);
	p->restore();
}
