/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <algorithm>

#include <QDebug>
#include <QList>
#include <QMutableListIterator>
#include <QMutableSetIterator>
#include <QPointF>
#include <QRectF>
#include <QSet>
#include <QString>
#include <QTransform>
#include <QtAlgorithms>

#include "appmodes.h"
#include "cellarea.h"
#include "collapsedtablepainter.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "scpainter.h"
#include "scribusdoc.h"
#include "styles/tablestyle.h"
#include "tablehandle.h"
#include "tableutils.h"
#include "undomanager.h"
#include "undostate.h"

#include "pageitem_table.h"

#ifdef WANT_DEBUG
	#define ASSERT_VALID() assertValid(); qt_noop()
#else
	#define ASSERT_VALID() qt_noop()
#endif

// The minimum row height.
const double PageItem_Table::MinimumRowHeight = 3.0;

// The minimum column width.
const double PageItem_Table::MinimumColumnWidth = 3.0;

PageItem_Table::PageItem_Table(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline, int numRows, int numColumns) :
	PageItem(pa, PageItem::Table, x, y, w, h, w2, fill, outline),
	m_rows(0), m_columns(0), m_tablePainter(new CollapsedTablePainter(this))
{
	initialize(numRows, numColumns);

	doc()->dontResize = true;
	adjustTableToFrame();
	adjustFrameToTable();
	doc()->dontResize = false;

	ASSERT_VALID();
}

PageItem_Table::~PageItem_Table()
{
	delete m_tablePainter;
}

void PageItem_Table::adjustTable()
{
	doc()->dontResize = true;
	adjustTableToFrame();
	adjustFrameToTable();
	doc()->dontResize = false;
}

void PageItem_Table::currentTextProps(ParagraphStyle& parStyle) const
{
	if (m_Doc && (m_Doc->appMode == modeEditTable))
		m_activeCell.textFrame()->currentTextProps(parStyle);
	else
		parStyle = this->itemText.defaultStyle();
}

QList<PageItem*> PageItem_Table::getChildren() const
{
	QList<PageItem*> ret;

	int numRows = this->rows();
	int numColums = this->columns();
	for (int row = 0; row < numRows; ++row)
	{
		for (int col = 0; col < numColums; ++col)
		{
			TableCell cell = cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem* textFrame = cell.textFrame();
				ret.append(textFrame);
			}
		}
	}

	return ret;
}

void PageItem_Table::getNamedResources(ResourceCollection& lists) const
{
	TableBorder lborder = leftBorder();
	for (const TableBorderLine& line : lborder.borderLines())
	{
		if (line.color() == CommonStrings::None)
			continue;
		lists.collectColor(line.color());
	}

	TableBorder rborder = rightBorder();
	for (const TableBorderLine& line : rborder.borderLines())
	{
		if (line.color() == CommonStrings::None)
			continue;
		lists.collectColor(line.color());
	}

	TableBorder bborder = bottomBorder();
	for (const TableBorderLine& line : bborder.borderLines())
	{
		if (line.color() == CommonStrings::None)
			continue;
		lists.collectColor(line.color());
	}

	TableBorder tborder = topBorder();
	for (const TableBorderLine& line : tborder.borderLines())
	{
		if (line.color() == CommonStrings::None)
			continue;
		lists.collectColor(line.color());
	}

	QString tableStyleName = this->styleName();
	if (!tableStyleName.isEmpty())
		lists.collectTableStyle(tableStyleName);

	for (int row = 0; row < rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < columns(); col += colSpan)
		{
			TableCell cell = cellAt(row, col);
			PageItem_TextFrame* textFrame = cell.textFrame();
			textFrame->getNamedResources(lists);

			QString cellStyle = cell.styleName();
			if (!cellStyle.isEmpty())
				lists.collectCellStyle(cellStyle);

			QString cellFill = cell.fillColor();
			if (cellFill != CommonStrings::None)
				lists.collectColor(cellFill);

			lborder = cell.leftBorder();
			for (const TableBorderLine& line : lborder.borderLines())
			{
				if (line.color() == CommonStrings::None)
					continue;
				lists.collectColor(line.color());
			}

			rborder = cell.rightBorder();
			for (const TableBorderLine& line : rborder.borderLines())
			{
				if (line.color() == CommonStrings::None)
					continue;
				lists.collectColor(line.color());
			}

			bborder = cell.bottomBorder();
			for (const TableBorderLine& line : bborder.borderLines())
			{
				if (line.color() == CommonStrings::None)
					continue;
				lists.collectColor(line.color());
			}

			tborder = cell.topBorder();
			for (const TableBorderLine& line : tborder.borderLines())
			{
				if (line.color() == CommonStrings::None)
					continue;
				lists.collectColor(line.color());
			}

			colSpan = cell.columnSpan();
		}
	}

	PageItem::getNamedResources(lists);
}

void PageItem_Table::replaceNamedResources(ResourceCollection& newNames)
{
	QMap<QString, QString>::ConstIterator it;

	PageItem::replaceNamedResources(newNames);

	bool lborderChanged = false;
	TableBorder lborder = leftBorder();
	auto lborderLines = lborder.borderLines();
	for (int i = 0; i < lborderLines.count(); ++i)
	{
		TableBorderLine line = lborderLines.at(i);
		if (line.color() == CommonStrings::None)
			continue;
		it = newNames.colors().find(line.color());
		if (it != newNames.colors().end())
		{
			line.setColor(*it);
			lborder.replaceBorderLine(i, line);
			lborderChanged = true;
		}
	}

	if (lborderChanged)
		setLeftBorder(lborder);

	bool rborderChanged = false;
	TableBorder rborder = rightBorder();
	auto rborderLines = rborder.borderLines();
	for (int i = 0; i < rborderLines.count(); ++i)
	{
		TableBorderLine line = rborderLines.at(i);
		if (line.color() == CommonStrings::None)
			continue;
		it = newNames.colors().find(line.color());
		if (it != newNames.colors().end())
		{
			line.setColor(*it);
			rborder.replaceBorderLine(i, line);
			rborderChanged = true;
		}
	}

	if (rborderChanged)
		setRightBorder(rborder);

	bool bborderChanged = false;
	TableBorder bborder = bottomBorder();
	auto bborderLines = bborder.borderLines();
	for (int i = 0; i < bborderLines.count(); ++i)
	{
		TableBorderLine line = bborderLines.at(i);
		if (line.color() == CommonStrings::None)
			continue;
		it = newNames.colors().find(line.color());
		if (it != newNames.colors().end())
		{
			line.setColor(*it);
			bborder.replaceBorderLine(i, line);
			bborderChanged = true;
		}
	}

	if (bborderChanged)
		setBottomBorder(bborder);

	bool tborderChanged = false;
	TableBorder tborder = topBorder();
	auto tborderLines = tborder.borderLines();
	for (int i = 0; i < tborderLines.count(); ++i)
	{
		TableBorderLine line = tborderLines.at(i);
		if (line.color() == CommonStrings::None)
			continue;
		it = newNames.colors().find(line.color());
		if (it != newNames.colors().end())
		{
			line.setColor(*it);
			tborder.replaceBorderLine(i, line);
			tborderChanged = true;
		}
	}

	if (tborderChanged)
		setTopBorder(tborder);

	it = newNames.tableStyles().find(this->styleName());
	if (it != newNames.tableStyles().end())
		this->setStyle(*it);

	for (int row = 0; row < rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < columns(); col += colSpan)
		{
			TableCell cell = cellAt(row, col);
			PageItem_TextFrame* textFrame = cell.textFrame();
			textFrame->replaceNamedResources(newNames);

			it = newNames.cellStyles().find(cell.styleName());
			if (it != newNames.cellStyles().end())
				cell.setStyle(*it);

			it = newNames.colors().find(cell.fillColor());
			if (it != newNames.colors().end())
				cell.setFillColor(*it);

			lborderChanged = false;
			lborder = cell.leftBorder();
			lborderLines = lborder.borderLines();
			for (int i = 0; i < lborderLines.count(); ++i)
			{
				TableBorderLine line = lborderLines.at(i);
				if (line.color() == CommonStrings::None)
					continue;
				it = newNames.colors().find(line.color());
				if (it != newNames.colors().end())
				{
					line.setColor(*it);
					lborder.replaceBorderLine(i, line);
					lborderChanged = true;
				}
			}

			if (lborderChanged)
				setLeftBorder(lborder);

			rborderChanged = false;
			rborder = cell.rightBorder();
			rborderLines = rborder.borderLines();
			for (int i = 0; i < rborderLines.count(); ++i)
			{
				TableBorderLine line = rborderLines.at(i);
				if (line.color() == CommonStrings::None)
					continue;
				it = newNames.colors().find(line.color());
				if (it != newNames.colors().end())
				{
					line.setColor(*it);
					rborder.replaceBorderLine(i, line);
					rborderChanged = true;
				}
			}

			if (rborderChanged)
				setRightBorder(rborder);

			bborderChanged = false;
			bborder = cell.bottomBorder();
			bborderLines = bborder.borderLines();
			for (int i = 0; i < bborderLines.count(); ++i)
			{
				TableBorderLine line = bborderLines.at(i);
				if (line.color() == CommonStrings::None)
					continue;
				it = newNames.colors().find(line.color());
				if (it != newNames.colors().end())
				{
					line.setColor(*it);
					bborder.replaceBorderLine(i, line);
					bborderChanged = true;
				}
			}

			if (bborderChanged)
				setBottomBorder(bborder);

			tborderChanged = false;
			tborder = cell.topBorder();
			tborderLines = tborder.borderLines();
			for (int i = 0; i < tborderLines.count(); ++i)
			{
				TableBorderLine line = tborderLines.at(i);
				if (line.color() == CommonStrings::None)
					continue;
				it = newNames.colors().find(line.color());
				if (it != newNames.colors().end())
				{
					line.setColor(*it);
					tborder.replaceBorderLine(i, line);
					tborderChanged = true;
				}
			}

			if (tborderChanged)
				setTopBorder(tborder);

			colSpan = cell.columnSpan();
		}
	}
}

void PageItem_Table::layout()
{
	int rowCount = rows();
	int columnCount = columns();

	for (int row = 0; row < rowCount; ++row)
	{
		for (int col = 0; col < columnCount; col ++)
		{
			TableCell cell = cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem* textFrame = cell.textFrame();
				textFrame->layout();
			}
		}
	}
}

void PageItem_Table::setLayer(int newLayerID)
{
	m_layerID = newLayerID;

	int rowCount = rows();
	int columnCount = columns();

	for (int row = 0; row < rowCount; ++row)
	{
		for (int col = 0; col < columnCount; col++)
		{
			TableCell cell = cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem* textFrame = cell.textFrame();
				textFrame->m_layerID = newLayerID;
			}
		}
	}
}

void PageItem_Table::setMasterPage(int page, const QString& mpName)
{
	PageItem::setMasterPage(page, mpName);

	int rowCount = rows();
	int columnCount = columns();

	for (int row = 0; row < rowCount; ++row)
	{
		for (int col = 0; col < columnCount; col++)
		{
			TableCell cell = cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem* textFrame = cell.textFrame();
				textFrame->OwnPage = page;
				textFrame->OnMasterPage = mpName;
			}
		}
	}
}

void PageItem_Table::setMasterPageName(const QString& mpName)
{
	PageItem::setMasterPageName(mpName);

	int rowCount = rows();
	int columnCount = columns();

	for (int row = 0; row < rowCount; ++row)
	{
		for (int col = 0; col < columnCount; col++)
		{
			TableCell cell = cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem* textFrame = cell.textFrame();
				textFrame->OnMasterPage = mpName;
			}
		}
	}
}

void PageItem_Table::setOwnerPage(int page)
{
	PageItem::setOwnerPage(page);

	int rowCount = rows();
	int columnCount = columns();

	for (int row = 0; row < rowCount; ++row)
	{
		for (int col = 0; col < columnCount; col++)
		{
			TableCell cell = cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem* textFrame = cell.textFrame();
				textFrame->OwnPage = page;
			}
		}
	}
}

void PageItem_Table::resize(double width, double height)
{
	ASSERT_VALID();

	/*
	 * Distribute width proportionally to columns, but don't let any column width below
	 * MinimumColumnWidth.
	 */
	double requestedWidthFactor = width / tableWidth();
	double oldMinWidth = *std::min_element(m_columnWidths.begin(), m_columnWidths.end());
	double newMinWidth = qMax(oldMinWidth * requestedWidthFactor, MinimumColumnWidth);
	double actualWidthFactor = newMinWidth / oldMinWidth;
	for (int col = 0; col < columns(); ++col)
	{
		m_columnWidths[col] *= actualWidthFactor;
		m_columnPositions[col] *= actualWidthFactor;
	}

	/*
	 * Distribute height proportionally to rows, but don't let any row height below
	 * MinimumRowHeight.
	 */
	double requestedHeightFactor = height / tableHeight();
	double oldMinHeight = *std::min_element(m_rowHeights.begin(), m_rowHeights.end());
	double newMinHeight = qMax(oldMinHeight * requestedHeightFactor, MinimumRowHeight);
	double actualHeightFactor = newMinHeight / oldMinHeight;
	for (int row = 0; row < rows(); ++row)
	{
		m_rowHeights[row] *= actualHeightFactor;
		m_rowPositions[row] *= actualHeightFactor;
	}

	// Update cells. TODO: Not for entire table.
	updateCells();

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::insertRows(int index, int numRows)
{
	ASSERT_VALID();

	if (index < 0 || index > rows() || numRows < 1)
		return;

	double rowHeight = m_rowHeights.at(qMax(index - 1, 0));
	double rowPosition = index == 0 ? 0.0 : m_rowPositions.at(index - 1) + rowHeight;

	for (int row = index; row < index + numRows; ++row)
	{
		// Insert row height and position.
		m_rowHeights.insert(row, rowHeight);
		m_rowPositions.insert(row, rowPosition);
		rowPosition += rowHeight;

		// Insert a row of cells.
		QList<TableCell> cellRow;
		cellRow.reserve(columns());
		for (int col = 0; col < columns(); ++col)
			cellRow.append(TableCell(row, col, this));
		m_cellRows.insert(row, cellRow);
	}

	// Adjust following rows.
	double insertedHeight = rowHeight * numRows;
	for (int nextRow = index + numRows; nextRow < rows() + numRows; ++nextRow)
	{
		// Adjust position of following row.
		m_rowPositions[nextRow] += insertedHeight;

		// "Move" cells in following row down.
		foreach (TableCell cell, m_cellRows[nextRow])
			cell.moveDown(numRows);
	}

	// Update row spans.
	updateSpans(index, numRows, RowsInserted);

	// Increase number of rows.
	m_rows += numRows;

	// Update cells. TODO: Not for entire table.
	updateCells();

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::removeRows(int index, int numRows)
{
	ASSERT_VALID();

	if (!validRow(index) || numRows < 1 || numRows >= rows() || index + numRows > rows())
		return;

	// Remove row heights, row positions and rows of cells.
	double removedHeight = 0.0;
	for (int i = 0; i < numRows; ++i)
	{
		// Remove row height and position.
		removedHeight += m_rowHeights.takeAt(index);
		m_rowPositions.removeAt(index);

		// Invalidate removed cells.
		foreach (TableCell removedCell, m_cellRows[index])
			removedCell.setValid(false);

		// Remove row of cells.
		m_cellRows.removeAt(index);
	}

	// Adjust following rows.
	for (int nextRow = index; nextRow < rows() - numRows; ++nextRow)
	{
		// Adjust position of following row.
		m_rowPositions[nextRow] -= removedHeight;

		// "Move" cells in following row up.
		foreach (TableCell cell, m_cellRows[nextRow])
			cell.moveUp(numRows);
	}

	// Update row spans.
	updateSpans(index, numRows, RowsRemoved);

	// Decrease number of rows.
	m_rows -= numRows;

	// Update cells. TODO: Not for entire table.
	updateCells();

	// Remove any invalid cells from selection.
	QMutableSetIterator<TableCell> cellIt(m_selection);
	while (cellIt.hasNext())
		if (!cellIt.next().isValid())
			cellIt.remove();

	// Move to cell below.
	moveTo(cellAt(qMin(index + 1, rows() - 1), m_activeColumn));

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::insertColumns(int index, int numColumns)
{
	ASSERT_VALID();

	if (index < 0 || index > columns() || numColumns < 1)
		return;

	double columnWidth = m_columnWidths.at(qMax(index - 1, 0));
	double columnPosition = index == 0 ? 0.0 : m_columnPositions.at(index - 1) + columnWidth;

	for (int col = index; col < index + numColumns; ++col)
	{
		// Insert column width and position.
		m_columnWidths.insert(col, columnWidth);
		m_columnPositions.insert(col, columnPosition);
		columnPosition += columnWidth;

		// Insert a column of cells.
		for (int row = 0; row < rows(); ++row)
			m_cellRows[row].insert(col, TableCell(row, col, this));
	}

	// Adjust following columns.
	double insertedWidth = columnWidth * numColumns;
	for (int nextColumn = index + numColumns; nextColumn < columns() + numColumns; ++nextColumn)
	{
		// Adjust position of following column.
		m_columnPositions[nextColumn] += insertedWidth;

		// "Move" cells in following column right.
		foreach (QList<TableCell> cellRow, m_cellRows)
			cellRow[nextColumn].moveRight(numColumns);
	}

	// Update column spans.
	updateSpans(index, numColumns, ColumnsInserted);

	// Increase number of columns.
	m_columns += numColumns;

	// Update cells. TODO: Not for entire table.
	updateCells();

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::removeColumns(int index, int numColumns)
{
	ASSERT_VALID();

	if (!validColumn(index) || numColumns < 1 || numColumns >= columns() || index + numColumns > columns())
		return;

	// Remove column widths, column positions and columns of cells.
	double removedWidth = 0.0;
	for (int i = 0; i < numColumns; ++i)
	{
		// Remove columns widths and positions.
		removedWidth += m_columnWidths.takeAt(index);
		m_columnPositions.removeAt(index);

		// Remove and invalidate cells.
		QMutableListIterator<QList<TableCell> > rowIt(m_cellRows);
		while (rowIt.hasNext())
			rowIt.next().takeAt(index).setValid(false);
	}

	// Adjust following columns.
	for (int nextColumn = index; nextColumn < columns() - numColumns; ++nextColumn)
	{
		// Adjust position of following column.
		m_columnPositions[nextColumn] -= removedWidth;

		// "Move" cells in following column left.
		foreach (QList<TableCell> cellRow, m_cellRows)
			cellRow[nextColumn].moveLeft(numColumns);
	}

	// Update column spans.
	updateSpans(index, numColumns, ColumnsRemoved);

	// Decrease number of columns.
	m_columns -= numColumns;

	// Update cells. TODO: Not for entire table.
	updateCells();

	// Remove any invalid cells from selection.
	QMutableSetIterator<TableCell> cellIt(m_selection);
	while (cellIt.hasNext())
		if (!cellIt.next().isValid())
			cellIt.remove();

	// Move to cell to the right.
	moveTo(cellAt(m_activeRow, qMin(m_activeColumn + 1, columns() - 1)));

	emit changed();

	ASSERT_VALID();
}

double PageItem_Table::rowHeight(int row) const
{
	if (!validRow(row))
		return 0.0;

	return m_rowHeights.at(row);
}

void PageItem_Table::resizeRow(int row, double height, ResizeStrategy strategy)
{
	ASSERT_VALID();

	if (!validRow(row))
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TableRowHeight, QString(), Um::IResize);
		ss->set("TABLE_ROW_HEIGHT");
		ss->set("ROW", row);
		ss->set("OLD_ROW_HEIGHT", rowHeight(row));
		ss->set("NEW_ROW_HEIGHT", height);
		ss->set("ROW_RESIZE_STRATEGY", strategy == MoveFollowing ? 0 : 1);
		undoManager->action(this, ss);
	}
	if (strategy == MoveFollowing)
		resizeRowMoveFollowing(row, height);
	else if (strategy == ResizeFollowing)
		resizeRowResizeFollowing(row, height);
	else
		qWarning("Unknown resize strategy!");

	// Update cells. TODO: Not for entire table.
	updateCells();

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::distributeRows(int startRow, int endRow)
{
	if (startRow < 0 || endRow > rows() - 1 || startRow > endRow)
		return;

	const int numRows = endRow - startRow + 1;
	const double newHeight = (rowPosition(endRow) + rowHeight(endRow) - rowPosition(startRow)) / numRows;

	for (int row = startRow; row <= endRow; ++row)
		resizeRow(row, newHeight);
}

void PageItem_Table::distributeColumns(int startColumn, int endColumn)
{
	if (startColumn < 0 || endColumn > columns() - 1 || startColumn > endColumn)
		return;

	const int numColumns = endColumn - startColumn + 1;
	const double newWidth = (columnPosition(endColumn) + columnWidth(endColumn) - columnPosition(startColumn)) / numColumns;

	for (int column = startColumn; column <= endColumn; ++column)
		resizeColumn(column, newWidth);
}

double PageItem_Table::rowPosition(int row) const
{
	if (!validRow(row))
		return 0.0;

	return m_rowPositions.at(row);
}

double PageItem_Table::columnWidth(int column) const
{
	if (!validColumn(column))
		return 0.0;

	return m_columnWidths.at(column);
}

void PageItem_Table::resizeColumn(int column, double width, ResizeStrategy strategy)
{
	ASSERT_VALID();

	if (!validColumn(column))
		return;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TableColumnWidth, QString(), Um::IResize);
		ss->set("TABLE_COLUMN_WIDTH");
		ss->set("COLUMN", column);
		ss->set("OLD_COLUMN_WIDTH", columnWidth(column));
		ss->set("NEW_COLUMN_WIDTH", width);
		ss->set("COLUMN_RESIZE_STRATEGY", strategy == MoveFollowing ? 0 : 1);
		undoManager->action(this, ss);
	}
	if (strategy == MoveFollowing)
		resizeColumnMoveFollowing(column, width);
	else if (strategy == ResizeFollowing)
		resizeColumnResizeFollowing(column, width);
	else
		qWarning("Unknown resize strategy!");

	// Update cells. TODO: Not for entire table.
	updateCells();

	emit changed();

	ASSERT_VALID();
}

double PageItem_Table::columnPosition(int column) const
{
	if (!validColumn(column))
		return 0.0;

	return m_columnPositions.at(column);
}

void PageItem_Table::mergeCells(int row, int column, int numRows, int numCols)
{
	ASSERT_VALID();

	if (!validCell(row, column) || !validCell(row + numRows - 1, column + numCols - 1))
		return;

	CellArea newArea(row, column, numCols, numRows);

	// Unite intersecting areas.
	QMutableListIterator<CellArea> areaIt(m_cellAreas);
	while (areaIt.hasNext())
	{
		CellArea oldArea = areaIt.next();
		if (newArea.intersects(oldArea))
		{
			// The two areas intersect, so unite them.
			newArea = newArea.united(oldArea);

			// Reset row/column span of old spanning cell, then remove old area.
			TableCell oldSpanningCell = cellAt(oldArea.row(), oldArea.column());
			oldSpanningCell.setRowSpan(1);
			oldSpanningCell.setColumnSpan(1);
			areaIt.remove();
		}
	}

	// Set row/column span of new spanning cell, and add new area.
	TableCell newSpanningCell = cellAt(newArea.row(), newArea.column());
	newSpanningCell.setRowSpan(newArea.height());
	newSpanningCell.setColumnSpan(newArea.width());
	m_cellAreas.append(newArea);

	// Update cells. TODO: Not for entire table.
	updateCells();

	// If merged area covers active position, move to the spanning cell.
	if (newArea.contains(m_activeRow, m_activeColumn))
		moveTo(newSpanningCell);

	// Remove all cells covered by the merged area from the selection.
	QMutableSetIterator<TableCell> cellIt(m_selection);
	while (cellIt.hasNext())
	{
		TableCell cell = cellIt.next();
		if (newArea.contains(cell.row(), cell.column()) &&
			!(cell.row() == newArea.row() && cell.column() == newArea.column()))
			cellIt.remove();
	}

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::splitCell(int row, int column, int numRows, int numCols)
{
	// Not implemented.
	emit changed();
}

QSet<int> PageItem_Table::selectedRows() const
{
	QSet<int> rows;
	for (const TableCell& cell : selectedCells())
	{
		const int startRow = cell.row();
		const int endRow = startRow + cell.rowSpan() - 1;
		for (int row = startRow; row <= endRow; ++row)
			rows.insert(row);
	}
	return rows;
}

QSet<int> PageItem_Table::selectedColumns() const
{
	QSet<int> columns;
	for (const TableCell& cell : selectedCells())
	{
		const int startColumn = cell.column();
		const int endColumn = startColumn + cell.columnSpan() - 1;
		for (int col = startColumn; col <= endColumn; ++col)
			columns.insert(col);
	}
	return columns;
}

void PageItem_Table::selectCell(int row, int column)
{
	if (!validCell(row, column))
		return;

	m_selection.insert(cellAt(row, column));
	emit selectionChanged();
}

void PageItem_Table::selectCells(int startRow, int startColumn, int endRow, int endColumn)
{
	if (!validCell(startRow, startColumn) || !validCell(endRow, endColumn))
		return;

	const TableCell startCell = cellAt(startRow, startColumn);
	const TableCell endCell = cellAt(endRow, endColumn);

	const int topRow = qMin(startCell.row(), endCell.row());
	const int bottomRow = qMax(startCell.row() + startCell.rowSpan() - 1,
		endCell.row() + endCell.rowSpan() - 1);

	const int leftCol = qMin(startCell.column(), endCell.column());
	const int rightCol = qMax(startCell.column() + startCell.columnSpan() - 1,
		endCell.column() + endCell.columnSpan() - 1);

	for (int row = topRow; row <= bottomRow; ++row)
		for (int col = leftCol; col <= rightCol; ++col)
			selectCell(row, col);
	emit selectionChanged();
}

void PageItem_Table::selectColumn(int column)
{
	if (!validCell(0, column))
		return;
	selectCells(0, column, rows() - 1, column);
}

void PageItem_Table::selectRow(int row)
{
	if (!validCell(row, 0))
		return;
	selectCells(row, 0, row, columns() - 1);
}

void PageItem_Table::clearSelection()
{
	m_selection.clear();
	emit selectionChanged();
}

TableCell PageItem_Table::cellAt(int row, int column) const
{
	if (!validCell(row, column))
		return TableCell();

	TableCell cell = m_cellRows[row][column];

	QList<CellArea>::const_iterator areaIt;
	for (areaIt = m_cellAreas.begin(); areaIt != m_cellAreas.end(); ++areaIt)
	{
		CellArea area = (*areaIt);
		if (area.contains(row, column))
		{
			// Cell was contained in merged area, so use spanning cell.
			cell = m_cellRows[area.row()][area.column()];
			break;
		}
	}

	return cell;
}

TableCell PageItem_Table::cellAt(const QPointF& point) const
{
	QPointF gridPoint = getTransform().inverted().map(point) - gridOffset();

	if (!QRectF(0, 0, tableWidth(), tableHeight()).contains(gridPoint))
		return TableCell(); // Outside table grid.

	return cellAt(
		std::upper_bound(m_rowPositions.begin(), m_rowPositions.end(), gridPoint.y()) - m_rowPositions.begin() - 1,
		std::upper_bound(m_columnPositions.begin(), m_columnPositions.end(), gridPoint.x()) - m_columnPositions.begin() - 1);
}

QSet<TableCell> PageItem_Table::cells() const
{
	QSet<TableCell> tableCells;

	for (int row = 0; row < m_rows; ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < m_columns; col += colSpan)
		{
			TableCell currentCell = cellAt(row, col);
			if (row == currentCell.row())
				tableCells.insert(currentCell);
			colSpan = currentCell.columnSpan();
		}
	}

	return tableCells;
}

void PageItem_Table::moveLeft()
{
	if (m_activeCell.column() < 1)
		return;

	// Move active position left and activate cell at new position.
//	m_activeColumn = m_activeCell.column() - 1;
	activateCell(cellAt(m_activeRow, m_activeCell.column() - 1));
}

void PageItem_Table::moveRight()
{
	if (m_activeCell.column() + m_activeCell.columnSpan() >= columns())
		return;

	// Move active position right and activate cell at new position.
//	m_activeColumn = m_activeCell.column() + m_activeCell.columnSpan();
	activateCell(cellAt(m_activeRow, m_activeCell.column() + m_activeCell.columnSpan()));
}

void PageItem_Table::moveUp()
{
	if (m_activeCell.row() < 1)
		return;

	// Move active position up and activate cell at new position.
//	m_activeRow = m_activeCell.row() - 1;
	activateCell(cellAt(m_activeCell.row() - 1, m_activeColumn));
}

void PageItem_Table::moveDown()
{
	if (m_activeCell.row() + m_activeCell.rowSpan() >= rows())
		return;
	activateCell(cellAt(m_activeCell.row() + m_activeCell.rowSpan(), m_activeColumn));

	// Deselect previous active cell and its text.
//	m_activeCell.textFrame()->setSelected(false);
//	m_activeCell.textFrame()->itemText.deselectAll();

	// Move active logical position down.
//	m_activeRow = m_activeCell.row() + m_activeCell.rowSpan();

	// Set the new active cell and select it.
//	m_activeCell = cellAt(m_activeRow, m_activeColumn);
//	m_activeCell.textFrame()->setSelected(true);
//	m_Doc->currentStyle = m_activeCell.textFrame()->currentStyle();
}

void PageItem_Table::moveTo(const TableCell& cell)
{
	// Activate the cell and move active position to its top left.
	activateCell(cell);
}

TableHandle PageItem_Table::hitTest(const QPointF& point, double threshold) const
{
	const QPointF framePoint = getTransform().inverted().map(point);
	const QPointF gridPoint = framePoint - gridOffset();
	const QRectF gridRect(0.0, 0.0, tableWidth(), tableHeight());

	// Test if hit is outside frame.
	if (!QRectF(0.0, 0.0, width(), height()).contains(framePoint))
		return TableHandle(TableHandle::None);

	// Test if hit is outside table.
	if (!gridRect.adjusted(-threshold, -threshold, threshold, threshold).contains(gridPoint))
		return TableHandle(TableHandle::None);

	const double tableHeight = this->tableHeight();
	const double tableWidth = this->tableWidth();
	const double x = gridPoint.x();
	const double y = gridPoint.y();

	// Test if hit is on left edge of table.
	if (x <= threshold)
		return TableHandle(TableHandle::RowSelect);

	// Test if hit is on top edge of table.
	if (y <= threshold)
		return TableHandle(TableHandle::ColumnSelect);

	// Test if hit is on bottom right corner of table.
	if (x >= tableWidth - threshold && y >= tableHeight - threshold)
		return TableHandle(TableHandle::TableResize);

	// Test if hit is on right edge of table.
	if (y >= tableHeight - threshold && y <= tableHeight + threshold)
		return TableHandle(TableHandle::RowResize, rows() - 1);

	// Test if hit is on bottom edge of table.
	if (x >= tableWidth - threshold && x <= tableWidth + threshold)
		return TableHandle(TableHandle::ColumnResize, columns() - 1);

	const TableCell hitCell = cellAt(point);
	const QRectF hitRect = hitCell.boundingRect();

	// Test if hit is on cell interior.
	if (hitRect.adjusted(threshold, threshold, -threshold, -threshold).contains(gridPoint))
		return TableHandle(TableHandle::CellSelect); // Hit interior of cell.

	const double toLeft = x - hitRect.left();
	const double toRight = hitRect.right() - x;
	const double toTop = y - hitRect.top();
	const double toBottom = hitRect.bottom() - y;
	TableHandle handle(TableHandle::None);

	// Test which side of the cell was hit.
	if (qMin(toLeft, toRight) < qMin(toTop, toBottom))
	{
		handle.setType(TableHandle::ColumnResize);
		handle.setIndex((toLeft < toRight ? hitCell.column() : hitCell.column() + hitCell.columnSpan()) - 1);
	}
	else
	{
		handle.setType(TableHandle::RowResize);
		handle.setIndex((toTop < toBottom ? hitCell.row() : hitCell.row() + hitCell.rowSpan()) - 1);
	}
	return handle;
}

void PageItem_Table::adjustTableToFrame()
{
	resize(width() - (maxLeftBorderWidth() + maxRightBorderWidth()) / 2,
		height() - (maxTopBorderWidth() + maxBottomBorderWidth()) / 2);
}

void PageItem_Table::adjustFrameToTable()
{
	if (!m_Doc)
		return;

	m_Doc->sizeItem(effectiveWidth(), effectiveHeight(), this);
}

void PageItem_Table::setFillColor(const QString& color)
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TableFillColor, QString(), Um::ITable);
		ss->set("SET_TABLE_FILLCOLOR");
		ss->set("OLD_COLOR", m_style.fillColor());
		ss->set("NEW_COLOR", color);
		undoManager->action(this, ss);
	}

	m_style.setFillColor(color);
	emit changed();
}

void PageItem_Table::unsetFillColor()
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TableFillColorRst, QString(), Um::ITable);
		ss->set("UNSET_TABLE_FILLCOLOR");
		ss->set("OLD_COLOR", m_style.fillColor());
		undoManager->action(this, ss);
	}

	m_style.resetFillColor();
	emit changed();
}

QString PageItem_Table::fillColor() const
{
	return m_style.fillColor();
}

void PageItem_Table::setFillShade(const double& shade)
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TableFillShade, QString(), Um::ITable);
		ss->set("SET_TABLE_FILLSHADE");
		ss->set("OLD_SHADE", m_style.fillShade());
		ss->set("NEW_SHADE", shade);
		undoManager->action(this, ss);
	}

	m_style.setFillShade(shade);
	emit changed();
}

void PageItem_Table::unsetFillShade()
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TableFillShadeRst, QString(), Um::ITable);
		ss->set("UNSET_TABLE_FILLSHADE");
		ss->set("OLD_SHADE", m_style.fillShade());
		undoManager->action(this, ss);
	}

	m_style.resetFillShade();
	emit changed();
}

double PageItem_Table::fillShade() const
{
	return m_style.fillShade();
}

void PageItem_Table::setLeftBorder(const TableBorder& border)
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<TableBorder>(Um::TableLeftBorder, QString(), Um::ITable);
		ss->set("SET_TABLE_LEFTBORDER");
		ss->setStates(m_style.leftBorder(), border);
		undoManager->action(this, ss);
	}

	m_style.setLeftBorder(border);
	updateCells(0, 0, rows() - 1, 0);
	emit changed();
}

void PageItem_Table::unsetLeftBorder()
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScItemState<TableBorder>(Um::TableLeftBorderRst, QString(), Um::ITable);
		ss->set("UNSET_TABLE_LEFTBORDER");
		ss->setItem(m_style.leftBorder());
		undoManager->action(this, ss);
	}

	m_style.resetLeftBorder();
	updateCells(0, 0, rows() - 1, 0);
	emit changed();
}

TableBorder PageItem_Table::leftBorder() const
{
	return m_style.leftBorder();
}

void PageItem_Table::setRightBorder(const TableBorder& border)
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<TableBorder>(Um::TableRightBorder, QString(), Um::ITable);
		ss->set("SET_TABLE_RIGHTBORDER");
		ss->setStates(m_style.rightBorder(), border);
		undoManager->action(this, ss);
	}

	m_style.setRightBorder(border);
	updateCells(0, columns() - 1, rows() - 1, columns() - 1);
	emit changed();
}

void PageItem_Table::unsetRightBorder()
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScItemState<TableBorder>(Um::TableLeftBorderRst, QString(), Um::ITable);
		ss->set("UNSET_TABLE_RIGHTBORDER");
		ss->setItem(m_style.rightBorder());
		undoManager->action(this, ss);
	}

	m_style.resetRightBorder();
	updateCells(0, columns() - 1, rows() - 1, columns() - 1);
	emit changed();
}

TableBorder PageItem_Table::rightBorder() const
{
	return m_style.rightBorder();
}

void PageItem_Table::setTopBorder(const TableBorder& border)
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<TableBorder>(Um::TableTopBorder, QString(), Um::ITable);
		ss->set("SET_TABLE_TOPBORDER");
		ss->setStates(m_style.topBorder(), border);
		undoManager->action(this, ss);
	}

	m_style.setTopBorder(border);
	updateCells(0, 0, 0, columns() - 1);
	emit changed();
}

void PageItem_Table::unsetTopBorder()
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScItemState<TableBorder>(Um::TableTopBorderRst, QString(), Um::ITable);
		ss->set("UNSET_TABLE_TOPBORDER");
		ss->setItem(m_style.topBorder());
		undoManager->action(this, ss);
	}

	m_style.resetTopBorder();
	updateCells(0, 0, 0, columns() - 1);
	emit changed();
}

TableBorder PageItem_Table::topBorder() const
{
	return m_style.topBorder();
}

void PageItem_Table::setBottomBorder(const TableBorder& border)
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScOldNewState<TableBorder>(Um::TableBottomBorder, QString(), Um::ITable);
		ss->set("SET_TABLE_BOTTOMBORDER");
		ss->setStates(m_style.bottomBorder(), border);
		undoManager->action(this, ss);
	}

	m_style.setBottomBorder(border);
	updateCells(rows() - 1, 0, rows() - 1, columns() - 1);
	emit changed();
}

void PageItem_Table::unsetBottomBorder()
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new ScItemState<TableBorder>(Um::TableBottomBorderRst, QString(), Um::ITable);
		ss->set("UNSET_TABLE_BOTTOMBORDER");
		ss->setItem(m_style.bottomBorder());
		undoManager->action(this, ss);
	}

	m_style.resetBottomBorder();
	updateCells(rows() - 1, 0, rows() - 1, columns() - 1);
	emit changed();
}

TableBorder PageItem_Table::bottomBorder() const
{
	return m_style.bottomBorder();
}

void PageItem_Table::setBorders(const TableBorder& border, TableSides selectedSides)
{
	if (selectedSides == (int) TableSide::None)
		return;

	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
	{
		activeTransaction = UndoManager::instance()->beginTransaction(getUName(), getUPixmap(), Um::TableBorders, QString(), Um::ITable);
		auto borderTuple = std::make_tuple(m_style.leftBorder(), m_style.rightBorder(), m_style.bottomBorder(), m_style.topBorder(), border);
		auto *ss = new ScItemState<TableBorderTuple>(Um::TableBorders, QString(), Um::ITable);
		ss->set("SET_TABLE_BORDERS");
		ss->set("SIDES", (int) selectedSides);
		ss->setItem(borderTuple);
		undoManager->action(this, ss);
	}

	if (selectedSides & TableSide::Left)
		m_style.setLeftBorder(border);
	if (selectedSides & TableSide::Right)
		m_style.setRightBorder(border);
	if (selectedSides & TableSide::Top)
		m_style.setTopBorder(border);
	if (selectedSides & TableSide::Bottom)
		m_style.setBottomBorder(border);
	
	updateCells();
	adjustTable();

	if (activeTransaction)
		activeTransaction.commit();

	emit changed();
}

void PageItem_Table::setCellBorders(const TableBorder& border, TableSides selectedSides)
{
	QSet<TableCell> tableCells;

	if (m_Doc->appMode != modeEditTable)
		tableCells = this->cells();
	else
	{
		tableCells = this->selectedCells();
		if (tableCells.isEmpty())
			tableCells.insert(activeCell());
	}

	setCellBorders(tableCells, border, selectedSides);
}

void PageItem_Table::setCellBorders(const QSet<TableCell>& cells, const TableBorder& border, TableSides selectedSides)
{
	if (selectedSides == (int) TableSide::None)
		return;

	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
	{
		activeTransaction = UndoManager::instance()->beginTransaction(getUName(), getUPixmap(), Um::CellBorders, QString(), Um::ITable);

		QMap<TableCell, TableBorderTuple> cellBorderMap;
		for (auto cellIter = cells.begin(); cellIter != cells.end(); cellIter++)
		{
			TableCell currentCell(*cellIter);
			auto borderTuple = std::make_tuple(currentCell.leftBorder(), currentCell.rightBorder(), currentCell.bottomBorder(), currentCell.topBorder(), border);
			cellBorderMap.insert(currentCell, borderTuple);
		}
		auto *ss = new ScItemState< QMap<TableCell, TableBorderTuple> >(Um::CellBorders, QString(), Um::ITable);
		ss->set("SET_CELL_BORDERS");
		ss->set("SIDES", (int) selectedSides);
		ss->setItem(cellBorderMap);
		undoManager->action(this, ss);
	}

	for (auto cellIter = cells.begin(); cellIter != cells.end(); cellIter++)
	{
		TableCell currentCell(*cellIter);
		if (selectedSides & TableSide::Left)
			currentCell.setLeftBorder(border);
		if (selectedSides & TableSide::Right)
			currentCell.setRightBorder(border);
		if (selectedSides & TableSide::Top)
			currentCell.setTopBorder(border);
		if (selectedSides & TableSide::Bottom)
			currentCell.setBottomBorder(border);
	}

	adjustTable();

	if (activeTransaction)
		activeTransaction.commit();
}

void PageItem_Table::setCellFillColor(const QString& fillColor)
{
	QSet<TableCell> tableCells;

	if (m_Doc->appMode != modeEditTable)
		tableCells = this->cells();
	else
	{
		tableCells = this->selectedCells();
		if (tableCells.isEmpty())
			tableCells.insert(activeCell());
	}

	if (UndoManager::undoEnabled())
	{
		QMap<TableCell, QPair<QString, QString> > cellColorMap;
		for (auto cellIter = tableCells.begin(); cellIter != tableCells.end(); cellIter++)
		{
			TableCell currentCell(*cellIter);
			QString oldColor = currentCell.fillColor();
			cellColorMap.insert(currentCell, qMakePair(oldColor, fillColor));
		}
		auto *ss = new ScItemState< QMap<TableCell, QPair<QString, QString> > >(Um::CellFillColor, "", Um::ITable);
		ss->set("SET_CELL_FILLCOLOR");
		ss->setItem(cellColorMap);
		undoManager->action(this, ss);
	}

	m_Doc->dontResize = true;
	for (auto cellIter = tableCells.begin(); cellIter != tableCells.end(); cellIter++)
	{
		TableCell currentCell(*cellIter);
		currentCell.setFillColor(fillColor);
	}
	m_Doc->dontResize = false;

	emit changed();
}

void PageItem_Table::setCellFillShade(double fillShade)
{
	QSet<TableCell> tableCells;

	if (m_Doc->appMode != modeEditTable)
		tableCells = this->cells();
	else
	{
		tableCells = this->selectedCells();
		if (tableCells.isEmpty())
			tableCells.insert(activeCell());
	}

	if (UndoManager::undoEnabled())
	{
		QMap<TableCell, QPair<double, double> > cellShadeMap;
		for (auto cellIter = tableCells.begin(); cellIter != tableCells.end(); cellIter++)
		{
			TableCell currentCell(*cellIter);
			double oldShade = currentCell.fillShade();
			cellShadeMap.insert(currentCell, qMakePair(oldShade, fillShade));
		}
		auto *ss = new ScItemState< QMap<TableCell, QPair<double, double> > >(Um::CellFillShade, "", Um::ITable);
		ss->set("SET_CELL_FILLSHADE");
		ss->setItem(cellShadeMap);
		undoManager->action(this, ss);
	}

	m_Doc->dontResize = true;
	for (auto cellIter = tableCells.begin(); cellIter != tableCells.end(); cellIter++)
	{
		TableCell currentCell(*cellIter);
		currentCell.setFillShade(fillShade);
	}
	m_Doc->dontResize = false;

	emit changed();
}

void PageItem_Table::setCellStyle(const QString& cellStyle)
{
	QSet<TableCell> tableCells;

	if (m_Doc->appMode != modeEditTable)
		tableCells = this->cells();
	else
	{
		tableCells = this->selectedCells();
		if (tableCells.isEmpty())
			tableCells.insert(activeCell());
	}

	if (UndoManager::undoEnabled())
	{
		QMap<TableCell, QPair<QString, QString> > cellStyleMap;
		for (auto cellIter = tableCells.begin(); cellIter != tableCells.end(); cellIter++)
		{
			TableCell currentCell(*cellIter);
			QString oldStyle = currentCell.styleName();
			cellStyleMap.insert(currentCell, qMakePair(oldStyle, cellStyle));
		}
		auto *ss = new ScItemState< QMap<TableCell, QPair<QString, QString> > >(Um::CellStyle, "", Um::ITable);
		ss->set("SET_CELL_STYLE");
		ss->setItem(cellStyleMap);
		undoManager->action(this, ss);
	}

	m_Doc->dontResize = true;
	for (auto cellIter = tableCells.begin(); cellIter != tableCells.end(); cellIter++)
	{
		TableCell currentCell(*cellIter);
		currentCell.setStyle(cellStyle);
	}
	m_Doc->dontResize = false;

	emit changed();
}

void PageItem_Table::setStyle(const QString& style)
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::TableStyle, "", Um::ITable);
		ss->set("SET_TABLE_STYLE");
		ss->set("OLD_STYLE", m_style.parent());
		ss->set("NEW_STYLE", style);
		undoManager->action(this, ss);
	}

	doc()->dontResize = true;
	m_style.setParent(style);
	updateCells();
	doc()->dontResize = false;
	emit changed();
}

void PageItem_Table::unsetStyle()
{
	if (UndoManager::undoEnabled())
	{
		auto *ss = new SimpleState(Um::ChangeFormula, "", Um::IChangeFormula);
		ss->set("UNSET_TABLE_STYLE");
		ss->set("OLD_STYLE", m_style.parent());
		ss->set("NEW_STYLE", QString());
		undoManager->action(this, ss);
	}

	doc()->dontResize = true;
	m_style.setParent("");
	updateCells();
	doc()->dontResize = false;
	emit changed();
}

void PageItem_Table::unsetDirectFormatting()
{
	doc()->dontResize = true;
	QString parentStyle = m_style.parent();
	m_style.setParent("");
	m_style.erase();
	m_style.setParent(parentStyle);
	adjustTableToFrame();
	adjustFrameToTable();
	updateCells();
	doc()->dontResize = false;
	emit changed();
}

const TableStyle& PageItem_Table::style() const
{
	return m_style;
}

QString PageItem_Table::styleName() const
{
	return m_style.parent();
}

void PageItem_Table::handleStyleChanged()
{
	doc()->dontResize = true;
	updateCells();
	doc()->dontResize = false;
}

void PageItem_Table::applicableActions(QStringList& actionList)
{
	const bool tableEdit = m_Doc->appMode == modeEditTable;
	const int tableRows = rows();
	const int tableColumns = columns();
	const int selectedRows = tableEdit ? this->selectedRows().size() : 0;
	const int selectedColumns = tableEdit ? this->selectedColumns().size() : 0;
	const int selectedCells = tableEdit ? this->selectedCells().size() : 0;

	if (!tableEdit || selectedCells<1)
		actionList << "tableInsertRows";
	if (!tableEdit || selectedCells<1)
		actionList << "tableInsertColumns";
	if (tableEdit && ((selectedRows < 1 && tableRows > 1) || (selectedRows > 0 && selectedRows < tableRows)))
		actionList << "tableDeleteRows";
	if ((selectedColumns < 1 && tableColumns > 1) || (selectedColumns > 0 && selectedColumns < tableColumns))
		actionList << "tableDeleteColumns";
	if (selectedCells > 1)
		actionList << "tableMergeCells";
	if (tableEdit)
		actionList << "tableSetRowHeights";
	if (tableEdit)
		actionList << "tableSetColumnWidths";
	if (!tableEdit || (tableEdit && selectedRows > 1))
		actionList << "tableDistributeRowsEvenly";
	if (!tableEdit || (tableEdit && selectedColumns > 1))
		actionList << "tableDistributeColumnsEvenly";
	actionList << "tableAdjustFrameToTable";
	actionList << "tableAdjustTableToFrame";
}

void PageItem_Table::DrawObj_Item(ScPainter *p, const QRectF& /*e*/)
{
	if (m_Doc->RePos)
		return;

	p->save();

	// Set the clip path.
	p->setupPolygon(&PoLine);
	p->setClipPath();

	// Paint the table.
	m_tablePainter->paintTable(p);

	p->restore();

	// Paint the overflow marker.
	if (isOverflowing())
		drawOverflowMarker(p);
}

void PageItem_Table::initialize(int numRows, int numColumns)
{
	Q_ASSERT(m_Doc);
	Q_ASSERT(numRows > 0);
	Q_ASSERT(numColumns > 0);

	// Internal style is in document-wide style context.
	m_style.setContext(&m_Doc->tableStyles());

	// Reserve space in lists.
	m_cellRows.reserve(numRows);
	m_rowHeights.reserve(numRows);
	m_rowPositions.reserve(numRows);
	m_columnWidths.reserve(numColumns);
	m_columnPositions.reserve(numColumns);

	// Initialize rows of cells.
	QList<TableCell> initialRow;
	initialRow.append(TableCell(0, 0, this));
	m_cellRows.append(initialRow);

	// Initialize row/column geometries.
	m_rowPositions.insert(0, 0.0);
	m_rowHeights.insert(0, 10.0);
	m_columnPositions.insert(0, 0.0);
	m_columnWidths.insert(0, 10.0);

	// Initialize row/column counts.
	m_rows = 1;
	m_columns = 1;

	// Insert any remaining rows and/or columns.
	insertRows(0, numRows - 1);
	insertColumns(0, numColumns - 1);

	// Listen to changes in the document-wide cell/table style contexts.
	m_Doc->tableStyles().connect(this, SLOT(handleStyleChanged()));
	m_Doc->cellStyles().connect(this, SLOT(handleStyleChanged()));

	m_activeCell = cellAt(0, 0);
	m_activeRow = 0;
	m_activeColumn = 0;
}

void PageItem_Table::activateCell(const TableCell& cell)
{
	ASSERT_VALID();

	TableCell newActiveCell = validCell(cell.row(), cell.column()) ? cell : cellAt(0, 0);

	// Deselect previous active cell and its text.
	PageItem_TextFrame* textFrame = m_activeCell.textFrame();
	textFrame->setSelected(false);
	textFrame->itemText.deselectAll();
	textFrame->HasSel = false;

	// Set current style context before assigning new active cell:
	// if old active cell ref count is 1, the old context might be deleted
	const ParagraphStyle& curStyle = newActiveCell.textFrame()->currentStyle();
	m_Doc->currentStyle.setContext(curStyle.context());
	m_Doc->currentStyle = newActiveCell.textFrame()->currentStyle();

	// Set the new active cell and select it.
	m_activeCell = newActiveCell;
	m_activeCell.textFrame()->setSelected(true);
	m_activeRow = m_activeCell.row();
	m_activeColumn = m_activeCell.column();
	emit selectionChanged();

	ASSERT_VALID();
}

double PageItem_Table::maxLeftBorderWidth() const
{
	double maxWidth = 0.0;
	TableCell cell;
	for (int row = 0; row < rows(); row += cell.rowSpan())
	{
		cell = cellAt(row, 0);
		maxWidth = qMax(maxWidth, TableUtils::collapseBorders(cell.leftBorder(), leftBorder()).width());
	}
	return maxWidth;
}

double PageItem_Table::maxRightBorderWidth() const
{
	double maxWidth = 0.0;
	TableCell cell;
	for (int row = 0; row < rows(); row += cell.rowSpan())
	{
		cell = cellAt(row, columns() - 1);
		maxWidth = qMax(maxWidth, TableUtils::collapseBorders(leftBorder(), cell.rightBorder()).width());
	}
	return maxWidth;
}

double PageItem_Table::maxTopBorderWidth() const
{
	double maxWidth = 0.0;
	TableCell cell;
	for (int col = 0; col < columns(); col += cell.columnSpan())
	{
		cell = cellAt(0, col);
		maxWidth = qMax(maxWidth, TableUtils::collapseBorders(cell.topBorder(), topBorder()).width());
	}
	return maxWidth;
}

double PageItem_Table::maxBottomBorderWidth() const
{
	double maxWidth = 0.0;
	TableCell cell;
	for (int col = 0; col < columns(); col += cell.columnSpan())
	{
		cell = cellAt(rows() - 1, col);
		maxWidth = qMax(maxWidth, TableUtils::collapseBorders(bottomBorder(), cell.bottomBorder()).width());
	}
	return maxWidth;
}

double PageItem_Table::resizeRowMoveFollowing(int row, double height)
{
	// Set row height.
	double newHeight = m_rowHeights[row] = qMax(MinimumRowHeight, height);

	// Move following rows.
	double rowPosition = m_rowPositions[row];
	for (int nextRow = row; nextRow < m_rowPositions.size(); ++nextRow)
	{
		m_rowPositions[nextRow] = rowPosition;
		rowPosition += m_rowHeights[nextRow];
	}

	return newHeight;
}

double PageItem_Table::resizeRowResizeFollowing(int row, double height)
{
	double oldHeight = m_rowHeights[row];
	double newHeight = oldHeight;

	if (row < rows() - 1)
	{
		// Following row exists, so height is bounded at both ends.
		newHeight = m_rowHeights[row] = qBound(
			PageItem_Table::MinimumRowHeight, height,
			oldHeight + m_rowHeights[row + 1] - MinimumRowHeight);

		// Resize/move following row.
		double heightChange = newHeight - oldHeight;
		m_rowPositions[row + 1] += heightChange;
		m_rowHeights[row + 1] -= heightChange;
	}
	else
	{
		// Last row, so height only bounded by MinimumRowHeight.
		newHeight = m_rowHeights[row] = qMax(MinimumRowHeight, height);
	}

	return newHeight;
}

double PageItem_Table::resizeColumnMoveFollowing(int column, double width)
{
	// Set column width.
	double newWidth = m_columnWidths[column] = qMax(MinimumColumnWidth, width);

	// Move following columns.
	double columnPosition = m_columnPositions[column];
	for (int nextColumn = column; nextColumn < m_columnPositions.size(); ++nextColumn)
	{
		m_columnPositions[nextColumn] = columnPosition;
		columnPosition += m_columnWidths[nextColumn];
	}

	return newWidth;
}

double PageItem_Table::resizeColumnResizeFollowing(int column, double width)
{
	double oldWidth = m_columnWidths[column];
	double newWidth = oldWidth;

	if (column < columns() - 1)
	{
		// Following column exists, so width is bounded at both ends.
		newWidth = m_columnWidths[column] = qBound(
			PageItem_Table::MinimumColumnWidth, width,
			oldWidth + m_columnWidths[column + 1] - MinimumColumnWidth);

		// Resize/move following column.
		double widthChange = newWidth - oldWidth;
		m_columnPositions[column + 1] += widthChange;
		m_columnWidths[column + 1] -= widthChange;
	}
	else
	{
		// Last column, so width only bounded by MinimumColumnWidth.
		newWidth = m_columnWidths[column] = qMax(MinimumColumnWidth, width);
	}

	return newWidth;
}

void PageItem_Table::updateCells(int startRow, int startColumn, int endRow, int endColumn)
{
	if (startRow > endRow || startColumn > endColumn)
		return; // Invalid area.

	if (!validCell(startRow, startColumn) || !validCell(endRow, endColumn))
		return; // Invalid area.

	foreach (const QList<TableCell>& cellRow, m_cellRows)
		foreach (TableCell cell, cellRow)
			cell.updateContent();
}

void PageItem_Table::updateSpans(int index, int number, ChangeType changeType)
{
	// Loop through areas of merged cells.
	QMutableListIterator<CellArea> areaIt(m_cellAreas);
	while (areaIt.hasNext())
	{
		CellArea oldArea = areaIt.next();

		// Get a copy of the area adjusted to the change.
		CellArea newArea;
		switch (changeType)
		{
			case RowsInserted:
				newArea = oldArea.adjustedForRowInsertion(index, number);
				break;
			case RowsRemoved:
				newArea = oldArea.adjustedForRowRemoval(index, number);
				break;
			case ColumnsInserted:
				newArea = oldArea.adjustedForColumnInsertion(index, number);
				break;
			case ColumnsRemoved:
				newArea = oldArea.adjustedForColumnRemoval(index, number);
				break;
			default:
				break;
		}

		// Check if the area was affected by the change.
		if (newArea != oldArea)
		{
			if (newArea.height() < 1 || newArea.width() < 1)
			{
				// Adjusted area was annihilated, so remove it.
				areaIt.remove();
			}
			else if (newArea.height() == 1 && newArea.width() == 1)
			{
				// Adjusted area is 1x1, so remove it.
				areaIt.remove();

				// And reset row/column span of spanning cell to 1.
				TableCell oldSpanningCell = cellAt(oldArea.row(), oldArea.column());
				oldSpanningCell.setRowSpan(1);
				oldSpanningCell.setColumnSpan(1);
			}
			else
			{
				// Replace the area with the adjusted copy.
				areaIt.setValue(newArea);

				// And set row/column spanning of spanning cell.
				TableCell newSpanningCell = cellAt(newArea.row(), newArea.column());
				newSpanningCell.setRowSpan(newArea.height());
				newSpanningCell.setColumnSpan(newArea.width());
			}
		}
	}
}

void PageItem_Table::debug() const
{
	qDebug() << "-------------------------------------------------";
	qDebug() << "Table Debug";
	qDebug() << "-------------------------------------------------";
	qDebug() << "m_rows: " <<  m_rows;
	qDebug() << "m_columns: " <<  m_columns;
	qDebug() << "m_columnPositions: " <<  m_columnPositions;
	qDebug() << "m_columnWidths: " <<  m_columnWidths;
	qDebug() << "m_rowPositions: " <<  m_rowPositions;
	qDebug() << "m_rowHeights: " <<  m_rowHeights;
	qDebug() << "m_cellAreas: " <<  m_cellAreas;
	qDebug() << "m_cellRows: ";
	for (const QList<TableCell>& cellRow : m_cellRows)
		for (const TableCell& cell : cellRow)
			qDebug() << cell.asString();
	qDebug() << "-------------------------------------------------";
}

void PageItem_Table::assertValid() const
{
	// Check list sizes.
	Q_ASSERT(rows() == m_rowPositions.size());
	Q_ASSERT(rows() == m_rowHeights.size());
	Q_ASSERT(columns() == m_columnPositions.size());
	Q_ASSERT(columns() == m_columnWidths.size());
	Q_ASSERT(rows() == m_cellRows.size());
	for (const QList<TableCell>& cellRow : m_cellRows)
		Q_ASSERT(columns() == cellRow.size());

	for (int row = 0; row < rows(); ++row)
	{
		for (int col = 0; col < columns(); ++col)
		{
			TableCell cell = m_cellRows[row][col];

			// Check that the cell reports correct row and column.
			Q_ASSERT(cell.row() == row);
			Q_ASSERT(cell.column() == col);

			// Check that the row and column span is sane.
			Q_ASSERT(cell.rowSpan() >= 1 && cell.columnSpan() >= 1);

			if (cell.rowSpan() > 1 || cell.columnSpan() > 1)
			{
				// Check that there's exactly one matching cell area.
				CellArea expectedArea(cell.row(), cell.column(), cell.columnSpan(), cell.rowSpan());
				Q_ASSERT(m_cellAreas.count(expectedArea) == 1);
			}
		}
	}

	// Check that the active position is in this table.
	Q_ASSERT(validCell(m_activeRow, m_activeColumn));

	// Check that the active cell is valid.
	Q_ASSERT(m_activeCell.isValid());
	Q_ASSERT(validCell(m_activeCell.row(), m_activeCell.column()));

	// Check that selected cells are valid.
	for (const TableCell& cell : m_selection)
	{
		Q_ASSERT(cell.isValid());
		Q_ASSERT(validCell(cell.row(), cell.column()));
	}

	for (const CellArea& cellArea : m_cellAreas)
	{
		// Check that the active cell is not covered.
		if (cellArea.contains(m_activeCell.row(), m_activeCell.column()))
			Q_ASSERT(m_activeCell.row() == cellArea.row() && m_activeCell.column() == cellArea.column());

		// Check that the selected cells are not covered.
		for (const TableCell& cell : m_selection)
			if (cellArea.contains(cell.row(), cell.column()))
				Q_ASSERT(cell.row() == cellArea.row() && cell.column() == cellArea.column());
	}
}

void PageItem_Table::restore(UndoState *state, bool isUndo)
{
	auto *simpleState = dynamic_cast<SimpleState*>(state);
	if (!simpleState)
	{
		PageItem::restore(state, isUndo);
		return;
	}

	bool doUpdate = false;
	if (simpleState->contains("SET_CELL_BORDERS"))
	{
		restoreCellBorders(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_CELL_FILLCOLOR"))
	{
		restoreCellFillColor(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_CELL_FILLSHADE"))
	{
		restoreCellFillShade(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_CELL_STYLE"))
	{
		restoreCellStyle(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_TABLE_FILLCOLOR"))
	{
		restoreTableFillColor(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_TABLE_FILLSHADE"))
	{
		restoreTableFillShade(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_TABLE_BORDERS"))
	{
		restoreTableBorders(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_TABLE_LEFTBORDER"))
	{
		restoreTableLeftBorder(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_TABLE_RIGHTBORDER"))
	{
		restoreTableRightBorder(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_TABLE_BOTTOMBORDER"))
	{
		restoreTableBottomBorder(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_TABLE_TOPBORDER"))
	{
		restoreTableTopBorder(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("SET_TABLE_STYLE"))
	{
		restoreTableStyle(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("UNSET_TABLE_FILLCOLOR"))
	{
		restoreTableFillColorReset(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("UNSET_TABLE_FILLSHADE"))
	{
		restoreTableFillShadeReset(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("UNSET_TABLE_LEFTBORDER"))
	{
		restoreTableLeftBorderReset(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("UNSET_TABLE_RIGHTBORDER"))
	{
		restoreTableRightBorderReset(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("UNSET_TABLE_BOTTOMBORDER"))
	{
		restoreTableBottomBorderReset(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("UNSET_TABLE_TOPBORDER"))
	{
		restoreTableTopBorderReset(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("UNSET_TABLE_STYLE"))
	{
		restoreTableStyleReset(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_ROW_HEIGHT"))
	{
		restoreTableRowHeight(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_COLUMN_WIDTH"))
	{
		restoreTableColumnWidth(simpleState, isUndo);
		doUpdate = true;
	}
	else
	{
		PageItem::restore(state, isUndo);
	}

	if (doUpdate)
	{
		if (state->transactionCode == 0 || state->transactionCode == 2)
			this->update();
	}
}

void PageItem_Table::restoreCellBorders(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<QMap<TableCell, TableBorderTuple > > *>(state);
	if (!itemState)
		return;

	TableSides selectedSides;
	selectedSides |= (TableSide) itemState->getInt("SIDES");

	QMap<TableCell, TableBorderTuple> cellBorderMap = itemState->getItem();
	if (isUndo)
	{
		for (auto iter = cellBorderMap.begin(); iter != cellBorderMap.end(); ++iter)
		{
			TableCell currentCell = iter.key();
			TableBorderTuple currentBorders = iter.value();
			currentCell.setLeftBorder( std::get<0>(currentBorders) );
			currentCell.setRightBorder( std::get<1>(currentBorders) );
			currentCell.setBottomBorder( std::get<2>(currentBorders) );
			currentCell.setTopBorder( std::get<3>(currentBorders) );
		}
	}
	else
	{
		for (auto iter = cellBorderMap.begin(); iter != cellBorderMap.end(); ++iter)
		{
			TableCell currentCell = iter.key();
			TableBorder newBorder = std::get<4>(iter.value());
			if (selectedSides & TableSide::Left)
				currentCell.setLeftBorder(newBorder);
			if (selectedSides & TableSide::Right)
				currentCell.setRightBorder(newBorder);
			if (selectedSides & TableSide::Bottom)
				currentCell.setBottomBorder(newBorder);
			if (selectedSides & TableSide::Top)
				currentCell.setTopBorder(newBorder);
		}
	}

	adjustTable();
}

void PageItem_Table::restoreCellFillColor(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<QMap<TableCell, QPair<QString, QString> > > *>(state);
	if (!itemState)
		return;

	QMap<TableCell, QPair<QString, QString> > cellColorMap = itemState->getItem();
	for (auto iter = cellColorMap.begin(); iter != cellColorMap.end(); ++iter)
	{
		TableCell currentCell = iter.key();
		QString restoredColor = isUndo ? iter.value().first : iter.value().second;
		currentCell.setFillColor(restoredColor);
	}
}

void PageItem_Table::restoreCellFillShade(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<QMap<TableCell, QPair<double, double> > > *>(state);
	if (!itemState)
		return;

	QMap<TableCell, QPair<double, double> > cellColorMap = itemState->getItem();
	for (auto iter = cellColorMap.begin(); iter != cellColorMap.end(); ++iter)
	{
		TableCell currentCell = iter.key();
		double restoredShade = isUndo ? iter.value().first : iter.value().second;
		currentCell.setFillShade(restoredShade);
	}
}

void PageItem_Table::restoreCellStyle(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<QMap<TableCell, QPair<QString, QString> > > *>(state);
	if (!itemState)
		return;

	QMap<TableCell, QPair<QString, QString> > cellStyleMap = itemState->getItem();
	for (auto iter = cellStyleMap.begin(); iter != cellStyleMap.end(); ++iter)
	{
		TableCell currentCell = iter.key();
		QString restoredStyle = isUndo ? iter.value().first : iter.value().second;
		currentCell.setStyle(restoredStyle);
	}
}

void PageItem_Table::restoreTableFillColor(SimpleState *state, bool isUndo)
{
	QString restoredFillColor = state->get(isUndo ? "OLD_COLOR" : "NEW_COLOR");
	setFillColor(restoredFillColor);
}

void PageItem_Table::restoreTableFillColorReset(SimpleState *state, bool isUndo)
{
	if (isUndo)
	{
		QString restoredFillColor = state->get("OLD_COLOR");
		setFillColor(restoredFillColor);
	}
	else
	{
		unsetFillColor();
	}
}

void PageItem_Table::restoreTableFillShade(SimpleState *state, bool isUndo)
{
	double restoredFillShade = state->getDouble(isUndo ? "OLD_SHADE" : "NEW_SHADE");
	setFillShade(restoredFillShade);
}

void PageItem_Table::restoreTableFillShadeReset(SimpleState *state, bool isUndo)
{
	if (isUndo)
	{
		double restoredFillShade = state->getDouble("OLD_SHADE");
		setFillShade(restoredFillShade);
	}
	else
	{
		unsetFillShade();
	}
}

void PageItem_Table::restoreTableBorders(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<TableBorderTuple> *>(state);
	if (!itemState)
		return;

	TableSides selectedSides;
	selectedSides |= (TableSide) itemState->getInt("SIDES");

	TableBorderTuple borderTuple = itemState->getItem();
	if (isUndo)
	{
		m_style.setLeftBorder( std::get<0>(borderTuple) );
		m_style.setRightBorder( std::get<1>(borderTuple) );
		m_style.setBottomBorder( std::get<2>(borderTuple) );
		m_style.setTopBorder( std::get<3>(borderTuple) );
	}
	else
	{
		TableBorder newBorder = std::get<4>(borderTuple);
		if (selectedSides & TableSide::Left)
			m_style.setLeftBorder(newBorder);
		if (selectedSides & TableSide::Right)
			m_style.setRightBorder(newBorder);
		if (selectedSides & TableSide::Bottom)
			m_style.setBottomBorder(newBorder);
		if (selectedSides & TableSide::Top)
			m_style.setTopBorder(newBorder);
	}

	updateCells();
	adjustTable();
}

void PageItem_Table::restoreTableLeftBorder(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScOldNewState<TableBorder> *>(state);
	if (!itemState)
		return;

	const TableBorder& restoredBorder = isUndo ? itemState->getOldState() : itemState->getNewState();
	setLeftBorder(restoredBorder);

	adjustTable();
}

void PageItem_Table::restoreTableLeftBorderReset(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<TableBorder> *>(state);
	if (!itemState)
		return;

	if (isUndo)
	{
		TableBorder oldBorder = itemState->getItem();
		setLeftBorder(oldBorder);
	}
	else
	{
		unsetLeftBorder();
	}

	adjustTable();
}

void PageItem_Table::restoreTableRightBorder(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScOldNewState<TableBorder> *>(state);
	if (!itemState)
		return;

	const TableBorder& restoredBorder = isUndo ? itemState->getOldState() : itemState->getNewState();
	setRightBorder(restoredBorder);

	adjustTable();
}

void PageItem_Table::restoreTableRightBorderReset(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<TableBorder> *>(state);
	if (!itemState)
		return;

	if (isUndo)
	{
		TableBorder oldBorder = itemState->getItem();
		setRightBorder(oldBorder);
	}
	else
	{
		unsetRightBorder();
	}

	adjustTable();
}

void PageItem_Table::restoreTableBottomBorder(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScOldNewState<TableBorder> *>(state);
	if (!itemState)
		return;

	const TableBorder& restoredBorder = isUndo ? itemState->getOldState() : itemState->getNewState();
	setBottomBorder(restoredBorder);

	adjustTable();
}

void PageItem_Table::restoreTableBottomBorderReset(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<TableBorder> *>(state);
	if (!itemState)
		return;

	if (isUndo)
	{
		TableBorder oldBorder = itemState->getItem();
		setBottomBorder(oldBorder);
	}
	else
	{
		unsetBottomBorder();
	}

	adjustTable();
}

void PageItem_Table::restoreTableTopBorder(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScOldNewState<TableBorder> *>(state);
	if (!itemState)
		return;

	const TableBorder& restoredBorder = isUndo ? itemState->getOldState() : itemState->getNewState();
	setTopBorder(restoredBorder);

	adjustTable();
}

void PageItem_Table::restoreTableTopBorderReset(SimpleState *state, bool isUndo)
{
	const auto* itemState = dynamic_cast< ScItemState<TableBorder> *>(state);
	if (!itemState)
		return;

	if (isUndo)
	{
		TableBorder oldBorder = itemState->getItem();
		setTopBorder(oldBorder);
	}
	else
	{
		unsetTopBorder();
	}

	adjustTable();
}

void PageItem_Table::restoreTableStyle(SimpleState *state, bool isUndo)
{
	QString restoredStyle = state->get(isUndo ? "OLD_STYLE" : "NEW_STYLE");
	setStyle(restoredStyle);
}

void PageItem_Table::restoreTableStyleReset(SimpleState *state, bool isUndo)
{
	QString restoredStyle = state->get(isUndo ? "OLD_STYLE" : "NEW_STYLE");
	setStyle(restoredStyle);
}

void PageItem_Table::restoreTableRowHeight(SimpleState *state, bool isUndo)
{
	int row = state->getInt("ROW");
	ResizeStrategy strategy = state->getInt("ROW_RESIZE_STRATEGY") == 0 ? MoveFollowing : ResizeFollowing;
	if (isUndo)
	{
		double oldHeight = state->getDouble("OLD_ROW_HEIGHT");
		resizeRow(row, oldHeight, strategy);
	}
	else
	{
		double newHeight = state->getDouble("NEW_ROW_HEIGHT");
		resizeRow(row, newHeight, strategy);
	}
}

void PageItem_Table::restoreTableColumnWidth(SimpleState *state, bool isUndo)
{
	int column = state->getInt("COLUMN");
	ResizeStrategy strategy = state->getInt("COLUMN_RESIZE_STRATEGY") == 0 ? MoveFollowing : ResizeFollowing;
	if (isUndo)
	{
		double oldWidth = state->getDouble("OLD_COLUMN_WIDTH");
		resizeColumn(column, oldWidth, strategy);
	}
	else
	{
		double newWidth = state->getDouble("NEW_COLUMN_WIDTH");
		resizeColumn(column, newWidth, strategy);
	}
}
