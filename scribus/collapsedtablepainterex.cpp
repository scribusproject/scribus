/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QColor>
#include <QLineF>
#include <QRectF>

#include "pageitem_table.h"
#include "prefsmanager.h"
#include "scpageoutput.h"
#include "scpainterexbase.h"
#include "scribusdoc.h"
#include "tablecell.h"
#include "tableborder.h"
#include "tableutils.h"

#include "collapsedtablepainterex.h"

using namespace TableUtils;

CollapsedTablePainterEx::CollapsedTablePainterEx(ScPageOutput* pageOutput, PageItem_Table* table)
{
	m_pageOutput = pageOutput;
	m_table = table;
}

void CollapsedTablePainterEx::paintTable(ScPainterExBase* p)
{
	p->save();
	p->translate(m_table->gridOffset());

	// Paint table fill.
	paintTableFill(p);

	/*
	 * We paint the table in five passes:
	 *
	 * 1) Cell fills.
	 * 2) Vertical borders.
	 * 3) Horizontal borders
	 * 4) Decorative grid lines.
	 * 5) Cell content.
	 */

	// Pass 1: Paint cell fills.
	for (int row = 0; row < m_table->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < m_table->columns(); col += colSpan)
		{
			TableCell cell = m_table->cellAt(row, col);
			if (row == cell.row())
				paintCellFill(cell, p);
			colSpan = cell.columnSpan();
		}
	}

	// Pass 2: Paint vertical borders.
	for (int row = 0; row < m_table->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < m_table->columns(); col += colSpan)
		{
			TableCell cell = m_table->cellAt(row, col);
			if (row == cell.row())
			{
				paintCellRightBorders(cell, p);
				if (col == 0)
					paintCellLeftBorders(cell, p);
			}
			colSpan = cell.columnSpan();
		}
	}

	// Pass 3: Paint horizontal borders.
	for (int row = 0; row < m_table->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < m_table->columns(); col += colSpan)
		{
			TableCell cell = m_table->cellAt(row, col);
			if (row == cell.row())
			{
				paintCellBottomBorders(cell, p);
				if (row == 0)
					paintCellTopBorders(cell, p);
			}
			colSpan = cell.columnSpan();
		}
	}

	// Pass 5: Paint cell content.
	for (int row = 0; row < m_table->rows(); ++row)
	{
		for (int col = 0; col < m_table->columns(); col ++)
		{
			TableCell cell = m_table->cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem_TextFrame* textFrame = cell.textFrame();
				m_pageOutput->drawItem(textFrame, p, QRect());
			}
		}
	}

	p->restore();
}

void CollapsedTablePainterEx::paintTableFill(ScPainterExBase* p) const
{
	QString colorName = m_table->fillColor();

	if (colorName == CommonStrings::None)
		return;

	p->save();

	int lastCol = m_table->columns() - 1;
	int lastRow = m_table->rows() - 1;

	double x = m_table->columnPosition(0);
	double y = m_table->rowPosition(0);
	double width = m_table->columnPosition(lastCol) + m_table->columnWidth(lastCol) - x;
	double height = m_table->rowPosition(lastRow) + m_table->rowHeight(lastRow) - y;

	ScribusDoc* doc = m_table->doc();
	ScColorShade colorShade(doc->PageColors[colorName], (int) m_table->fillShade());
	p->setBrush(colorShade);
	p->setFillMode(ScPainterExBase::Solid);
	p->setStrokeMode(ScPainterExBase::None);
	p->drawRect(x, y, width, height);

	p->restore();
}

void CollapsedTablePainterEx::paintCellLeftBorders(const TableCell& cell, ScPainterExBase* p) const
{
	/*
	 * We are going to paint the border marked # in the following setup.
	 *
	 *       +----------------------+----------------------+
	 *       |                      |                      |
	 *       |                      |                      |
	 *       |    topLeftCell      top     topRightCell    |
	 *       |                      |                      |
	 *       |                      |                      |
	 *       +--------topLeft-------+-------topRight-------+
	 *       |                      #                      |
	 *       |                      #                      |
	 *       |      leftCell     border        cell        |
	 *       |                      #                      |
	 *       |                      #                      |
	 *       +-------bottomLeft-----+------bottomRight-----+
	 *       |                      |                      |
	 *       |                      |                      |
	 *       |  bottomLeftCell   bottom   bottomRightCell  |
	 *       |                      |                      |
	 *       |                      |                      |
	 *       +----------------------+----------------------+
	 */

	// The cell ends in this row.
	const int lastRow = cell.row() + cell.rowSpan() - 1;
	// The cell starts in this column.
	const int firstCol = cell.column();
	// The X position of the border segments to paint.
	const double borderX = m_table->columnPosition(firstCol);

	// The start point of the border segment currently being painted.
	QPointF start(borderX, 0.0);
	// The end point of the border segment currently being painted.
	QPointF end(borderX, 0.0);
	// The start and end offset factors for the border segment currently being painted.
	QPointF startOffsetFactors, endOffsetFactors;
	// Start and end row of border segment currently being painted.
	int startRow, endRow;

	for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
	{
		// Get the neighboring cell to the left and determine border segment row interval.
		TableCell leftCell = m_table->cellAt(row, firstCol - 1);
		startRow = qMax(cell.row(), leftCell.row());
		endRow = qMin(lastRow, leftCell.isValid() ? leftCell.row() + leftCell.rowSpan() - 1 : lastRow);

		// Get the remaining neighboring cells.
		TableCell topLeftCell = m_table->cellAt(startRow - 1, firstCol - 1);
		TableCell topRightCell = m_table->cellAt(startRow - 1, firstCol);
		TableCell bottomRightCell = m_table->cellAt(lastRow + 1, firstCol);
		TableCell bottomLeftCell = m_table->cellAt(lastRow + 1, firstCol - 1);

		// Resolve borders between neighboring cells.
		TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
		resolveBordersVertical(topLeftCell, topRightCell, leftCell, cell, bottomLeftCell, bottomRightCell, &topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, m_table);

		if (border.isNull())
			continue; // Quit early if the border to paint is null.

		// Set initial coordinates.
		start.setY(m_table->rowPosition(startRow));
		end.setY(m_table->rowPosition(endRow) + m_table->rowHeight(endRow));

		// Adjust coordinates for joining.
		joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight,
			 &start, &end, &startOffsetFactors, &endOffsetFactors);

		// Paint the border segment.
		paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, p);
	}
}

void CollapsedTablePainterEx::paintCellRightBorders(const TableCell& cell, ScPainterExBase* p) const
{
	/*
	 * We are going to paint the border marked # in the following setup.
	 *
	 *       +----------------------+----------------------+
	 *       |                      |                      |
	 *       |                      |                      |
	 *       |    topLeftCell      top     topRightCell    |
	 *       |                      |                      |
	 *       |                      |                      |
	 *       +--------topLeft-------+-------topRight-------+
	 *       |                      #                      |
	 *       |                      #                      |
	 *       |         cell      border      rightCell     |
	 *       |                      #                      |
	 *       |                      #                      |
	 *       +-------bottomLeft-----+------bottomRight-----+
	 *       |                      |                      |
	 *       |                      |                      |
	 *       |  bottomLeftCell   bottom   bottomRightCell  |
	 *       |                      |                      |
	 *       |                      |                      |
	 *       +----------------------+----------------------+
	 */

	// The cell ends in this row.
	const int lastRow = cell.row() + cell.rowSpan() - 1;
	// The cell ends in this column.
	const int lastCol = cell.column() + cell.columnSpan() - 1;
	// The X position of the border segments to paint.
	const double borderX = m_table->columnPosition(lastCol) + m_table->columnWidth(lastCol);

	// The start point of the border segment currently being painted.
	QPointF start(borderX, 0.0);
	// The end point of the border segment currently being painted.
	QPointF end(borderX, 0.0);
	// The start and end offset factors for the border segment currently being painted.
	QPointF startOffsetFactors, endOffsetFactors;
	// The start and end row of border segment currently being painted.
	int startRow, endRow;

	for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
	{
		// Get the neighboring cell to the right and determine border segment row interval.
		TableCell rightCell = m_table->cellAt(row, lastCol + 1);
		startRow = qMax(cell.row(), rightCell.row());
		endRow = qMin(lastRow, rightCell.isValid() ? rightCell.row() + rightCell.rowSpan() - 1 : lastRow);

		// Get the remaining neighboring cells surrounding the segment.
		TableCell topLeftCell = m_table->cellAt(startRow - 1, lastCol);
		TableCell topRightCell = m_table->cellAt(startRow - 1, lastCol + 1);
		TableCell bottomRightCell = m_table->cellAt(endRow + 1, lastCol + 1);
		TableCell bottomLeftCell = m_table->cellAt(endRow + 1, lastCol);

		// Resolve borders between neighboring cells.
		TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
		resolveBordersVertical(topLeftCell, topRightCell, cell, rightCell, bottomLeftCell, bottomRightCell, &topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, m_table);

		if (border.isNull())
			continue; // Quit early if the border to paint is null.

		// Set initial coordinates.
		start.setY(m_table->rowPosition(startRow));
		end.setY(m_table->rowPosition(endRow) + m_table->rowHeight(endRow));

		// Adjust coordinates for joining.
		joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight,
			 &start, &end, &startOffsetFactors, &endOffsetFactors);

		// Paint the border.
		paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, p);
	}
}

void CollapsedTablePainterEx::paintCellTopBorders(const TableCell& cell, ScPainterExBase* p) const
{
	/*
	 * We are going to paint the border marked # in the following setup.
	 *
	 *  +--------------------------+--------------------------+--------------------------+
	 *  |                          |                          |                          |
	 *  |                          |                          |                          |
	 *  |        topLeftCell   topLeft        topCell      topRight   topRightCell       |
	 *  |                          |                          |                          |
	 *  |                          |                          |                          |
	 *  +----------left------------+######### border-#########+----------right-----------+
	 *  |                          |                          |                          |
	 *  |                          |                          |                          |
	 *  |     bottomLeftCell   bottomLeft      cell      bottomRight  bottomRightCell    |
	 *  |                          |                          |                          |
	 *  |                          |                          |                          |
	 *  +--------------------------+--------------------------+--------------------------+
	 */
	// The cell starts in this row.
	const int firstRow = cell.row();
	// The cell ends in this column.
	const int lastCol = cell.column() + cell.columnSpan() - 1;
	// The Y position of the border segments to paint.
	const double borderY = m_table->rowPosition(firstRow);

	// The start point of the border segment currently being painted.
	QPointF start(0.0, borderY);
	// The end point of the border segment currently being painted.
	QPointF end(0.0, borderY);
	// The start and end offset factors for the border segment currently being painted.
	QPointF startOffsetFactors, endOffsetFactors;
	// Start and end column of border segment currently being painted.
	int startCol, endCol;

	for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
	{
		// Get the neighboring cell above and determine border segment column interval.
		TableCell topCell = m_table->cellAt(firstRow - 1, col);
		startCol = qMax(cell.column(), topCell.column());
		endCol = qMin(lastCol, topCell.isValid() ? topCell.column() + topCell.columnSpan() - 1 : lastCol);

		// Get the remaining neighboring cells.
		TableCell topLeftCell = m_table->cellAt(firstRow - 1, startCol - 1);
		TableCell topRightCell = m_table->cellAt(firstRow - 1, endCol + 1);
		TableCell bottomRightCell = m_table->cellAt(firstRow, endCol + 1);
		TableCell bottomLeftCell = m_table->cellAt(firstRow, startCol - 1);

		// Resolve borders between neighboring cells.
		TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
		resolveBordersHorizontal(topLeftCell, topCell, topRightCell, bottomLeftCell, cell, bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, m_table);

		if (border.isNull())
			continue; // Quit early if the border is null.

		// Set initial coordinates.
		start.setX(m_table->columnPosition(startCol));
		end.setX(m_table->columnPosition(endCol) + m_table->columnWidth(endCol));

		// Adjust coordinates for joining.
		joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight,
			 &start, &end, &startOffsetFactors, &endOffsetFactors);

		// Paint the border segment.
		paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, p);
	}
}

void CollapsedTablePainterEx::paintCellBottomBorders(const TableCell& cell, ScPainterExBase* p) const
{
	/*
	 * We are going to paint the border marked # in the following setup.
	 *
	 *  +--------------------------+--------------------------+--------------------------+
	 *  |                          |                          |                          |
	 *  |                          |                          |                          |
	 *  |        topLeftCell   topLeft         cell        topRight   topRightCell       |
	 *  |                          |                          |                          |
	 *  |                          |                          |                          |
	 *  +----------left------------+######### border-#########+----------right-----------+
	 *  |                          |                          |                          |
	 *  |                          |                          |                          |
	 *  |     bottomLeftCell   bottomLeft   bottomCell   bottomRight  bottomRightCell    |
	 *  |                          |                          |                          |
	 *  |                          |                          |                          |
	 *  +--------------------------+--------------------------+--------------------------+
	 */
	// The cell ends in this row.
	const int lastRow = cell.row() + cell.rowSpan() - 1;
	// The cell ends in this column.
	const int lastCol = cell.column() + cell.columnSpan() - 1;
	// The Y position of the border segments to paint.
	const double borderY = m_table->rowPosition(lastRow) + m_table->rowHeight(lastRow);

	// The start point of the border segment currently being painted.
	QPointF start(0.0, borderY);
	// The end point of the border segment currently being painted.
	QPointF end(0.0, borderY);
	// The start and end offset factors for the border segment currently being painted.
	QPointF startOffsetFactors, endOffsetFactors;
	// Start and end column of border segment currently being painted.
	int startCol, endCol;

	for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
	{
		// Get the neighboring cell below and determine border segment column interval.
		TableCell bottomCell = m_table->cellAt(lastRow + 1, col);
		startCol = qMax(cell.column(), bottomCell.column());
		endCol = qMin(lastCol, bottomCell.isValid() ? bottomCell.column() + bottomCell.columnSpan() - 1 : lastCol);

		// Get the remaining neighboring cells.
		TableCell topLeftCell = m_table->cellAt(lastRow, startCol - 1);
		TableCell topRightCell = m_table->cellAt(lastRow, endCol + 1);
		TableCell bottomRightCell = m_table->cellAt(lastRow + 1, endCol + 1);
		TableCell bottomLeftCell = m_table->cellAt(lastRow + 1, startCol - 1);

		// Resolve borders between neighboring cells.
		TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
		resolveBordersHorizontal(topLeftCell, cell, topRightCell, bottomLeftCell, bottomCell, bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, m_table);

		if (border.isNull())
			continue; // Quit early if the border is null.

		// Set initial coordinates.
		start.setX(m_table->columnPosition(startCol));
		end.setX(m_table->columnPosition(endCol) + m_table->columnWidth(endCol));

		// Adjust coordinates for joining.
		joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight,
			 &start, &end, &startOffsetFactors, &endOffsetFactors);

		// Paint the border segment.
		paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, p);
	}
}

void CollapsedTablePainterEx::paintCellFill(const TableCell& cell, ScPainterExBase* p) const
{
	QString colorName = cell.fillColor();

	if (colorName == CommonStrings::None)
		return;

	p->save();

	ScribusDoc* doc = m_table->doc();
	ScColorShade colorShade(doc->PageColors[colorName], (int) cell.fillShade());
	p->setBrush(colorShade);
	p->setFillMode(ScPainterExBase::Solid);
	p->setStrokeMode(ScPainterExBase::None);

	int row = cell.row();
	int col = cell.column();
	int lastRow = row + cell.rowSpan() - 1;
	int lastCol = col + cell.columnSpan() - 1;

	double x = m_table->columnPosition(col);
	double y = m_table->rowPosition(row);
	double width = m_table->columnPosition(lastCol) + m_table->columnWidth(lastCol) - x;
	double height = m_table->rowPosition(lastRow) + m_table->rowHeight(lastRow) - y;
	p->drawRect(x, y, width, height);

	p->restore();
}

void CollapsedTablePainterEx::paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end,
										const QPointF& startOffsetFactors, const QPointF& endOffsetFactors,
										ScPainterExBase *p) const
{
	ScribusDoc* doc = m_table->doc();

	p->save();
	p->setStrokeMode(ScPainterExBase::Solid);
	p->setFillMode(ScPainterExBase::None);

	QColor lineColor;
	QPointF lineStart, lineEnd;
	foreach (const TableBorderLine& line, border.borderLines())
	{
		// Adjust line start and ends by line width multipled by offset factors.
		lineStart.setX(start.x() + line.width() * startOffsetFactors.x());
		lineStart.setY(start.y() + line.width() * startOffsetFactors.y());
		lineEnd.setX(end.x() + line.width() * endOffsetFactors.x());
		lineEnd.setY(end.y() + line.width() * endOffsetFactors.y());
		// Set line color.
		if (line.color() == CommonStrings::None)
			continue;
		// Draw line.
		ScColorShade penColor(doc->PageColors[line.color()], (int) line.shade());
		p->setPen(penColor, line.width(), line.style(), Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(lineStart, lineEnd);
	}

	p->restore();
}
