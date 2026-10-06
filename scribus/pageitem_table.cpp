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
#include <QScopedValueRollback>
#include <QSet>
#include <QString>
#include <QTransform>
#include <QtAlgorithms>

#include "appmodes.h"
#include "cellarea.h"
#include "collapsedtablepainter.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scpainter.h"
#include "scribusdoc.h"
#include "styles/styleset.h"
#include "styles/tablestyle.h"
#include "tablehandle.h"
#include "tableutils.h"
#include "undomanager.h"
#include "undostate.h"
#include "util_text.h"

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

	// Also collect the resources the applied table style itself references --
	// in particular the cell styles its conditional areas are based on -- so
	// they travel with the table when copied/pasted to another document.
	if (m_Doc->tableStyles().contains(tableStyleName))
		m_Doc->tableStyles().get(tableStyleName).getNamedResources(lists);

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

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TableRowInsert, QString(), Um::ITable);
		ss->set("TABLE_INSERT_ROWS");
		ss->set("INDEX", index);
		ss->set("NUM_ROWS", numRows);
		undoManager->action(this, ss);
	}

	double rowHeight = m_rowHeights.at(qMax(index - 1, 0));
	double rowPosition = index == 0 ? 0.0 : m_rowPositions.at(index - 1) + rowHeight;

	UndoManager::instance()->setUndoEnabled(false);
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
		{
			cellRow.append(TableCell(row, col, this));
		}
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
	UndoManager::instance()->setUndoEnabled(true);

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::appendRows(int numRows)
{
	ASSERT_VALID();

	if (numRows < 1)
		return;

	const int index = rows();

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TableRowInsert, QString(), Um::ITable);
		ss->set("TABLE_APPEND_ROWS");
		ss->set("INDEX", index);
		ss->set("NUM_ROWS", numRows);
		ss->set("OLD_WIDTH", width());
		ss->set("OLD_HEIGHT", height());
		undoManager->action(this, ss);
	}

	UndoBlocker ub;

	// New rows inherit the last row's height (insertRows uses the row at
	// index-1). Grow the frame to fit rather than rescaling existing rows.
	insertRows(index, numRows);
	adjustFrameToTable();

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::removeRows(int index, int numRows)
{
	ASSERT_VALID();

	if (!validRow(index) || numRows < 1 || numRows >= rows() || index + numRows > rows())
		return;

	// Snapshot before destroying anything. The snapshot reads from live
	// state, so it must happen while everything is still intact.
	if (UndoManager::undoEnabled())
	{
		auto *is = new ScItemState<TableRowsSnapshot>(
			Um::TableRowRemove, QString(), Um::ITable);
		is->set("TABLE_REMOVE_ROWS");
		is->setItem(snapshotRows(index, numRows));
		undoManager->action(this, is);
	}

	UndoManager::instance()->setUndoEnabled(false);

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

	updateSpans(index, numRows, RowsRemoved);
	m_rows -= numRows;
	updateCells();

	QMutableSetIterator<TableCell> cellIt(m_selection);
	while (cellIt.hasNext())
		if (!cellIt.next().isValid())
			cellIt.remove();

	moveTo(cellAt(qMin(index + 1, rows() - 1), m_activeColumn));

	UndoManager::instance()->setUndoEnabled(true);

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::insertColumns(int index, int numColumns)
{
	ASSERT_VALID();

	if (index < 0 || index > columns() || numColumns < 1)
		return;

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TableColumnInsert, QString(), Um::ITable);
		ss->set("TABLE_INSERT_COLUMNS");
		ss->set("INDEX", index);
		ss->set("NUM_COLUMNS", numColumns);
		undoManager->action(this, ss);
	}

	double columnWidth = m_columnWidths.at(qMax(index - 1, 0));
	double columnPosition = index == 0 ? 0.0 : m_columnPositions.at(index - 1) + columnWidth;

	// Inherit borders from the adjacent existing column (the column to the
	// left of the insertion point, or column 0 if inserting at the start).
	// Table styles don't yet define borders; when they do, this should defer
	// to the style's border definitions.
	// const int templateCol = (index > 0) ? index - 1 : 0;
	// const bool haveTemplate = (columns() > 0);

	UndoManager::instance()->setUndoEnabled(false);
	for (int col = index; col < index + numColumns; ++col)
	{
		// Insert column width and position.
		m_columnWidths.insert(col, columnWidth);
		m_columnPositions.insert(col, columnPosition);
		columnPosition += columnWidth;

		// Insert a column of cells.
		for (int row = 0; row < rows(); ++row)
		{
			m_cellRows[row].insert(col, TableCell(row, col, this));
		}
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
	UndoManager::instance()->setUndoEnabled(true);

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::appendColumns(int numColumns)
{
	ASSERT_VALID();

	if (numColumns < 1)
		return;

	const int index = columns();

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TableColumnInsert, QString(), Um::ITable);
		ss->set("TABLE_APPEND_COLUMNS");
		ss->set("INDEX", index);
		ss->set("NUM_COLUMNS", numColumns);
		ss->set("OLD_WIDTH", width());
		ss->set("OLD_HEIGHT", height());
		undoManager->action(this, ss);
	}

	UndoBlocker ub;

	// New columns inherit the last column's width (insertColumns uses the
	// column at index-1). Grow the frame to fit rather than rescaling
	// existing columns.
	insertColumns(index, numColumns);
	adjustFrameToTable();

	emit changed();

	ASSERT_VALID();
}

void PageItem_Table::removeColumns(int index, int numColumns)
{
	ASSERT_VALID();

	if (!validColumn(index) || numColumns < 1 || numColumns >= columns() || index + numColumns > columns())
		return;

	if (UndoManager::undoEnabled())
	{
		auto *is = new ScItemState<TableColumnsSnapshot>(
			Um::TableColumnRemove, QString(), Um::ITable);
		is->set("TABLE_REMOVE_COLUMNS");
		is->setItem(snapshotColumns(index, numColumns));
		undoManager->action(this, is);
	}

	UndoManager::instance()->setUndoEnabled(false);

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

	// Update cells.
	updateCells();

	// Remove any invalid cells from selection.
	QMutableSetIterator<TableCell> cellIt(m_selection);
	while (cellIt.hasNext())
		if (!cellIt.next().isValid())
			cellIt.remove();

	// Move to cell to the right.
	moveTo(cellAt(m_activeRow, qMin(m_activeColumn + 1, columns() - 1)));

	UndoManager::instance()->setUndoEnabled(true);

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

	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = undoManager->beginTransaction(getUName(), getUPixmap(), Um::TableDistributeRows, QString(), Um::IResize);

	const int numRows = endRow - startRow + 1;
	const double newHeight = (rowPosition(endRow) + rowHeight(endRow) - rowPosition(startRow)) / numRows;
	for (int row = startRow; row <= endRow; ++row)
		resizeRow(row, newHeight);

	if (trans)
		trans.commit();
}

void PageItem_Table::distributeColumns(int startColumn, int endColumn)
{
	if (startColumn < 0 || endColumn > columns() - 1 || startColumn > endColumn)
		return;

	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = undoManager->beginTransaction(getUName(), getUPixmap(), Um::TableDistributeColumns, QString(), Um::IResize);

	const int numColumns = endColumn - startColumn + 1;
	const double newWidth = (columnPosition(endColumn) + columnWidth(endColumn) - columnPosition(startColumn)) / numColumns;
	for (int column = startColumn; column <= endColumn; ++column)
		resizeColumn(column, newWidth);

	if (trans)
		trans.commit();
}

double PageItem_Table::rowPosition(int row) const
{
	if (!validRow(row))
		return 0.0;

	return m_rowPositions.at(row);
}

double PageItem_Table::naturalRowHeight(int row, bool* hasContent)
{
	if (!validRow(row))
	{
		if (hasContent)
			*hasContent = false;
		return rowHeight(row);
	}

	double maxHeight = 0.0;
	bool anyContent = false;
	const int columnCount = columns();

	for (int i = 0; i < columnCount; ++i)
	{
		TableCell cell = cellAt(row, i);
		if (cell.row() != row)
			continue;
		if (cell.rowSpan() > 1)
			continue;

		if (cell.textFrame()->itemText.length() > 0)
			anyContent = true;

		double frameHeight = cell.textFrame()->naturalContentHeight();
		double cellOverhead = cell.topPadding() + cell.bottomPadding() + cell.maxTopBorderWidth() / 2.0 + cell.maxBottomBorderWidth() / 2.0;
		double cellHeight = frameHeight + cellOverhead;
		maxHeight = qMax(maxHeight, cellHeight);
	}

	if (hasContent)
		*hasContent = anyContent;

	const double minHeight = 8.0;
	return qMax(maxHeight, minHeight);
}

bool PageItem_Table::adjustRowHeight(int row, bool growOnly)
{
	bool hasContent = false;
	double natural = naturalRowHeight(row, &hasContent);

	// Don't shrink rows that have no content -- the user's manual sizing
	// should be preserved when there's nothing to fit.
	if (!hasContent)
		return false;

	if (qFuzzyCompare(natural, rowHeight(row)))
		return false;

	// Grow-only: never shrink a row below its current height (e.g. when the
	// user has deliberately made it taller than its content). Only enlarge to
	// fit overflowing text.
	if (growOnly && natural < rowHeight(row))
		return false;

	resizeRow(row, natural, MoveFollowing);
	return true;
}

void PageItem_Table::adjustAllRowHeights()
{
	const int rowCount = rows();
	for (int i = 0; i < rowCount; ++i)
		adjustRowHeight(i);
}

bool PageItem_Table::rowNeedsGrowthForCell(int row, int column) const
{
	TableCell cell = cellAt(row, column);
	if (!cell.isValid())
		return false;
	double cellOverhead = cell.topPadding() + cell.bottomPadding()
		+ cell.maxTopBorderWidth() / 2.0 + cell.maxBottomBorderWidth() / 2.0;
	double cellNeeded = cell.textFrame()->naturalContentHeight() + cellOverhead;
	return cellNeeded > rowHeight(row);
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

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TableMergeCells, QString(), Um::ITable);
		ss->set("TABLE_MERGE_CELLS");
		ss->set("ROW", row);
		ss->set("COLUMN", column);
		ss->set("NUM_ROWS", numRows);
		ss->set("NUM_COLS", numCols);
		undoManager->action(this, ss);
	}

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
	// The menu label for this operation is "Unmerge Cells", but the
	// code keeps the "split" name. The numRows/numCols parameters are unused.
	// This only unmerges to the original cells. Word-style subdivision into
	// row x column split cells is not possible in Scribus's grid table model.

	Q_UNUSED(numRows);
	Q_UNUSED(numCols);

	ASSERT_VALID();

	if (!validCell(row, column))
		return;

	// Find the merged area to record what we're undoing.
	CellArea targetArea;
	bool foundArea = false;
	for (const CellArea& area : m_cellAreas)
	{
		if (area.contains(row, column))
		{
			targetArea = area;
			foundArea = true;
			break;
		}
	}
	if (!foundArea)
		return;

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::TableUnmergeCells, QString(), Um::ITable);
		ss->set("TABLE_UNMERGE_CELLS");
		ss->set("ROW", targetArea.row());
		ss->set("COLUMN", targetArea.column());
		ss->set("NUM_ROWS", targetArea.height());
		ss->set("NUM_COLS", targetArea.width());
		undoManager->action(this, ss);
	}

	// Now do the actual split (unchanged from before, using targetArea).

	// Find the merged area containing the target cell, if any.
	QMutableListIterator<CellArea> areaIt(m_cellAreas);
	while (areaIt.hasNext())
	{
		CellArea area = areaIt.next();
		if (area.row() == targetArea.row() && area.column() == targetArea.column())
		{
			TableCell spanningCell = cellAt(area.row(), area.column());
			spanningCell.setRowSpan(1);
			spanningCell.setColumnSpan(1);
			areaIt.remove();
			break;
		}
	}

	updateCells();

	emit changed();

	ASSERT_VALID();
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

	// Test if hit is in the bottom-right table-resize corner.
	// (Checked early because it can overlap with row/column resize on the right and bottom edges.)
	if (x >= tableWidth - threshold && y >= tableHeight - threshold)
		return TableHandle(TableHandle::TableResize);

	// Select-all corner. In LTR this is the top-left corner of the grid
	// (junction of the row-select and column-select strips). When RTL document/table
	// layout is detected, switch this to the top-right corner based on the
	// table's reading direction.
	const bool isRTL = effectiveRTL();
	const bool inSelectAllCorner = isRTL
			? (x >= tableWidth - threshold && x <= tableWidth && y >= 0.0 && y <= threshold) /* RTL: top-right */
			: (x >= 0.0 && x <= threshold && y >= 0.0 && y <= threshold); /* LTR: top-left */
	if (inSelectAllCorner)
		return TableHandle(TableHandle::SelectAll, isRTL ? 1 : 0);

	// Test if hit is in the row-select strip (inside left edge of grid).
	if (x >= 0.0 && x <= threshold)
	{
		int row = -1;
		for (int r = 0; r < rows(); ++r)
		{
			double rowTop = rowPosition(r);
			double rowBottom = rowTop + rowHeight(r);
			if (y >= rowTop && y < rowBottom)
			{
				row = r;
				break;
			}
		}
		if (row < 0)
			return TableHandle(TableHandle::None);
		return TableHandle(TableHandle::RowSelect, row);
	}

	// Test if hit is in the column-select strip (inside top edge of grid).
	if (y >= 0.0 && y <= threshold)
	{
		int col = -1;
		for (int c = 0; c < columns(); ++c)
		{
			double colLeft = columnPosition(c);
			double colRight = colLeft + columnWidth(c);
			if (x >= colLeft && x < colRight)
			{
				col = c;
				break;
			}
		}
		if (col < 0)
			return TableHandle(TableHandle::None);
		return TableHandle(TableHandle::ColumnSelect, col);
	}

	// Test if hit is on right edge of table (resize last column).
	if (x >= tableWidth - threshold)
		return TableHandle(TableHandle::ColumnResize, columns() - 1);

	// Test if hit is on bottom edge of table (resize last row).
	if (y >= tableHeight - threshold)
		return TableHandle(TableHandle::RowResize, rows() - 1);

	const TableCell hitCell = cellAt(point);
	const QRectF hitRect = hitCell.boundingRect();

	// Test if hit is on cell interior.
	if (hitRect.adjusted(threshold, threshold, -threshold, -threshold).contains(gridPoint))
		return TableHandle(TableHandle::CellSelect);

	const double toLeft = x - hitRect.left();
	const double toRight = hitRect.right() - x;
	const double toTop = y - hitRect.top();
	const double toBottom = hitRect.bottom() - y;
	TableHandle handle(TableHandle::None);

	// Test which side of the cell was hit.
	if (qMin(toLeft, toRight) < qMin(toTop, toBottom))
	{
		// Closer to a vertical edge: column resize.
		int colIndex = (toLeft < toRight ? hitCell.column() : hitCell.column() + hitCell.columnSpan()) - 1;
		if (colIndex < 0 || colIndex >= columns())
			return TableHandle(TableHandle::CellSelect);
		handle.setType(TableHandle::ColumnResize);
		handle.setIndex(colIndex);
	}
	else
	{
		// Closer to a horizontal edge: row resize.
		int rowIndex = (toTop < toBottom ? hitCell.row() : hitCell.row() + hitCell.rowSpan()) - 1;
		if (rowIndex < 0 || rowIndex >= rows())
			return TableHandle(TableHandle::CellSelect);
		handle.setType(TableHandle::RowResize);
		handle.setIndex(rowIndex);
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
	rebuildAreaStyles();
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
	rebuildAreaStyles();
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
	m_style.update(m_style.context());
	rebuildAreaStyles();
	adjustTableToFrame();
	adjustFrameToTable();
	updateCells();
	doc()->dontResize = false;
	emit changed();
}

void PageItem_Table::rebuildAreaStyles()
{
	m_areaStyles.clear(false);
	m_areaStyles.setContext(&m_Doc->cellStyles());

	const CellStyle* defCell = m_Doc->cellStyles().getDefault();
	const QString defCellName = defCell ? defCell->name() : QString();

	const QList<TableArea> areas = m_style.conditionalAreasResolved();
	for (TableArea area : areas)
	{
		CellStyle cs = m_style.conditionalStyleResolved(area);
		cs.setName(tableAreaToString(area));
		// Inherit unset attributes (e.g. the default 1pt border) from the
		// document's default cell style,
		// where an empty parent resolved to the doc cell-style default.
		// A user-chosen "based on" parent (later feature) is preserved.
		if (cs.parent().isEmpty() && !defCellName.isEmpty())
			cs.setParent(defCellName);
		m_areaStyles.create(cs);
	}
	m_areaStyles.invalidate();
}

const TableStyle& PageItem_Table::style() const
{
	return m_style;
}

QString PageItem_Table::styleName() const
{
	return m_style.parent();
}

TableArea PageItem_Table::areaAt(int row, int column) const
{
	const int headerRows = m_style.headerRows();
	const int totalRows = m_style.totalRows();

	const int lastRowIndex = rows() - 1;
	const int lastColIndex = columns() - 1;

	const bool headerRow = (row < headerRows);
	const bool totalRow  = (totalRows > 0) && (row > lastRowIndex - totalRows);
	const bool firstCol  = m_style.firstColumn() && (column == 0);
	const bool lastCol   = m_style.lastColumn() && (column == lastColIndex);

	// Corners — highest priority.
	if (headerRow && firstCol)
		return TableArea::TopLeftCell;
	if (headerRow && lastCol)
		return TableArea::TopRightCell;
	if (totalRow && firstCol)
		return TableArea::BottomLeftCell;
	if (totalRow && lastCol)
		return TableArea::BottomRightCell;

	// Edges.
	if (lastCol)
		return TableArea::LastColumn;
	if (firstCol)
		return TableArea::FirstColumn;
	if (totalRow)
		return TableArea::TotalRow;
	if (headerRow)
		return TableArea::HeaderRow;

	// Banding — count body rows/columns past the header so the first body row/column is the
	// odd band regardless of how many header rows/columns precede it.
	if (m_style.bandedRows())
	{
		const int bodyRow = row - headerRows;
		const TableArea rowArea = (bodyRow % 2 == 0)
				? TableArea::BandedRowOdd : TableArea::BandedRowEven;
		// If row banding is enabled but the resolved row-band fill is None,
		// and column banding is also on, fall through to column banding.
		// Matches Word's layered behaviour.
		if (m_style.bandedColumns() && m_style.conditionalStyleResolved(rowArea).fillColor() == CommonStrings::None)
		{
			const int bodyCol = column - (m_style.firstColumn() ? 1 : 0);
			return (bodyCol % 2 == 0) ? TableArea::BandedColOdd : TableArea::BandedColEven;
		}
		return rowArea;
	}
	if (m_style.bandedColumns())
	{
		const int bodyCol = column - (m_style.firstColumn() ? 1 : 0);
		return (bodyCol % 2 == 0) ? TableArea::BandedColOdd : TableArea::BandedColEven;
	}

	return TableArea::BodyCell;
}

QString PageItem_Table::areaStyleNameBare(TableArea area) const
{
	if (!m_style.hasConditionalStyleResolved(area))
		return QString();
	return tableAreaToString(area);
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

	actionList << "tableInsertRows";
	actionList << "tableInsertColumns";
	if (!tableEdit || selectedCells<1)
	{
		actionList << "fileImportText";
		actionList << "fileImportAppendText";
		actionList << "toolsEditWithStoryEditor";
		actionList << "insertSampleText";
	}
	if (tableEdit && ((selectedRows < 1 && tableRows > 1) || (selectedRows > 0 && selectedRows < tableRows)))
		actionList << "tableDeleteRows";
	if ((selectedColumns < 1 && tableColumns > 1) || (selectedColumns > 0 && selectedColumns < tableColumns))
		actionList << "tableDeleteColumns";
	if (selectedCells > 1)
		actionList << "tableMergeCells";
	// Split is the inverse of merge: enabled when the active cell is merged
	// (spans more than one row or column).
	if (tableEdit && selectedCells == 1)
	{
		const TableCell active = activeCell();
		if (active.isValid() && (active.rowSpan() > 1 || active.columnSpan() > 1))
			actionList << "tableSplitCells";
	}
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
	actionList << "tableAdjustRowHeights";
}

void PageItem_Table::DrawObj_Item(ScPainter *p, const QRectF& /*e*/)
{
	if (m_Doc->RePos)
		return;

	// Moving the table changes where cell text belongs but does not tell cells to re-layout
	if (!qFuzzyCompare(m_lastLayoutYPos, yPos()))
	{
		m_lastLayoutYPos = yPos();
		int rowCount = rows();
		int columnCount = columns();
		for (int row = 0; row < rowCount; ++row)
		{
			for (int col = 0; col < columnCount; ++col)
			{
				TableCell cell = cellAt(row, col);
				if (cell.row() == row && cell.column() == col)
					cell.textFrame()->invalidateLayout();
			}
		}
	}

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

	// A new table inherits its fill/shade from its table style rather than the
	// construction fill parameter. Parent to the default style and reset the
	// base-class-pinned fill so the style drives styling.
	if (m_style.parent().isEmpty())
		m_style.setParent(CommonStrings::DefaultTableStyle);
	m_style.resetFillColor();
	m_style.resetFillShade();

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

	// Re-derive each cell's structural area and splice the matching conditional
	// style as its transient parent. Done here, before the content pass, so
	// that row/column insertion and removal (which all route through
	// updateCells) reflow conditional styling automatically.
	for (int row = 0; row < m_rows; ++row)
	{
		for (int column = 0; column < m_columns; ++column)
		{
			QString an = areaStyleNameBare(areaAt(row, column));
			m_cellRows[row][column].applyAreaStyle(an);
		}
	}

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

TableRowsSnapshot PageItem_Table::snapshotRows(int index, int numRows) const
{
	TableRowsSnapshot snap;
	snap.index = index;
	snap.numRows = numRows;
	snap.numColumns = columns();
	snap.frameWidth = width();
	snap.frameHeight = height();
	snap.rowHeights = m_rowHeights;
	snap.cells.reserve(numRows * columns());

	for (int r = 0; r < numRows; ++r)
	{
		const int row = index + r;

		for (int col = 0; col < columns(); ++col)
		{
			TableCell cell = cellAt(row, col);
			TableRowsSnapshot::CellSnapshot cs;
			PageItem_TextFrame *tf = cell.textFrame();
			if (tf)
				cs.storyTextXml = saxedText(&tf->itemText);
			cs.style = cell.styleName();
			cs.fillColor = cell.fillColor();
			cs.fillShade = cell.fillShade();
			cs.leftBorder = cell.leftBorder();
			cs.rightBorder = cell.rightBorder();
			cs.topBorder = cell.topBorder();
			cs.bottomBorder = cell.bottomBorder();
			snap.cells.append(cs);
		}
	}

	// Capture every CellArea that intersects the row range [index, index+numRows).
	// We need the original areas, before updateSpans mutates them.
	for (const CellArea& area : m_cellAreas)
	{
		const int areaTop = area.row();
		const int areaBottom = area.row() + area.height() - 1;
		const int removeTop = index;
		const int removeBottom = index + numRows - 1;
		if (areaBottom < removeTop || areaTop > removeBottom)
			continue;
		snap.areas.append(area);
	}

	return snap;
}

void PageItem_Table::restoreRowsFromSnapshot(const TableRowsSnapshot& snap)
{
	// Caller has already re-inserted snap.numRows blank rows at snap.index.
	// We're called with undo disabled, so any setters here are quiet.

	m_rowHeights = snap.rowHeights;

	for (int r = 0; r < snap.numRows; ++r)
	{
		const int row = snap.index + r;

		for (int col = 0; col < snap.numColumns; ++col)
		{
			TableCell cell = cellAt(row, col);
			const auto& cs = snap.cells.at(r * snap.numColumns + col);

			PageItem_TextFrame *tf = cell.textFrame();
			if (tf && !cs.storyTextXml.isEmpty())
			{
				StoryText restored = desaxeString(m_Doc, cs.storyTextXml);
				tf->itemText.clear();
				tf->itemText.insert(0, restored, false);
			}

			cell.setStyle(cs.style);
			cell.setFillColor(cs.fillColor);
			cell.setFillShade(cs.fillShade);
			cell.setLeftBorder(cs.leftBorder);
			cell.setRightBorder(cs.rightBorder);
			cell.setTopBorder(cs.topBorder);
			cell.setBottomBorder(cs.bottomBorder);
		}
	}

	// Recompute row positions from the now-correct row heights.
	double pos = 0.0;
	for (int row = 0; row < rows(); ++row)
	{
		m_rowPositions[row] = pos;
		pos += m_rowHeights[row];
	}

	// Restore any merged areas that intersected the removed range.
	// mergeCells preserves the top-left cell's content:
	//   - if the merge's top-left was inside the removed range, we just
	//     restored that cell's content from the snapshot above;
	//   - if the merge's top-left was above the removed range, that content
	//     was never destroyed and is still in the table.
	for (const CellArea& area : snap.areas)
		mergeCells(area.row(), area.column(), area.height(), area.width());

	updateCells();
}

TableColumnsSnapshot PageItem_Table::snapshotColumns(int index, int numColumns) const
{
	TableColumnsSnapshot snap;
	snap.index = index;
	snap.numColumns = numColumns;
	snap.numRows = rows();
	snap.frameWidth = width();
	snap.frameHeight = height();
	snap.columnWidths = m_columnWidths;
	snap.cells.reserve(rows() * numColumns);

	for (int row = 0; row < rows(); ++row)
	{
		for (int c = 0; c < numColumns; ++c)
		{
			TableCell cell = cellAt(row, index + c);
			TableColumnsSnapshot::CellSnapshot cs;
			PageItem_TextFrame *tf = cell.textFrame();
			if (tf)
				cs.storyTextXml = saxedText(&tf->itemText);
			cs.style = cell.styleName();
			cs.fillColor = cell.fillColor();
			cs.fillShade = cell.fillShade();
			cs.leftBorder = cell.leftBorder();
			cs.rightBorder = cell.rightBorder();
			cs.topBorder = cell.topBorder();
			cs.bottomBorder = cell.bottomBorder();
			snap.cells.append(cs);
		}
	}

	// Capture every CellArea that intersects the column range [index, index+numColumns).
	// We need the original areas, before updateSpans mutates them.
	for (const CellArea& area : m_cellAreas)
	{
		const int areaLeft = area.column();
		const int areaRight = area.column() + area.width() - 1;
		const int removeLeft = index;
		const int removeRight = index + numColumns - 1;
		if (areaRight < removeLeft || areaLeft > removeRight)
			continue;
		snap.areas.append(area);
	}

	return snap;
}

void PageItem_Table::restoreColumnsFromSnapshot(const TableColumnsSnapshot& snap)
{
	// Caller has already re-inserted snap.numColumns blank columns at snap.index.
	// We're called with undo disabled, so any setters here are quiet.

	m_columnWidths = snap.columnWidths;

	for (int row = 0; row < snap.numRows; ++row)
	{
		for (int c = 0; c < snap.numColumns; ++c)
		{
			const int col = snap.index + c;
			TableCell cell = cellAt(row, col);
			const auto& cs = snap.cells.at(row * snap.numColumns + c);

			PageItem_TextFrame *tf = cell.textFrame();
			if (tf && !cs.storyTextXml.isEmpty())
			{
				StoryText restored = desaxeString(m_Doc, cs.storyTextXml);
				tf->itemText.clear();
				tf->itemText.insert(0, restored, false);
			}

			cell.setStyle(cs.style);
			cell.setFillColor(cs.fillColor);
			cell.setFillShade(cs.fillShade);
			cell.setLeftBorder(cs.leftBorder);
			cell.setRightBorder(cs.rightBorder);
			cell.setTopBorder(cs.topBorder);
			cell.setBottomBorder(cs.bottomBorder);
		}
	}

	// Recompute column positions from the now-correct column widths.
	double pos = 0.0;
	for (int col = 0; col < columns(); ++col)
	{
		m_columnPositions[col] = pos;
		pos += m_columnWidths[col];
	}

	// Restore any merged areas that intersected the removed range.
	// mergeCells preserves the top-left cell's content:
	//   - if the merge's top-left was inside the removed range, we just
	//     restored that cell's content from the snapshot above;
	//   - if the merge's top-left was above the removed range, that content
	//     was never destroyed and is still in the table.
	for (const CellArea& area : snap.areas)
		mergeCells(area.row(), area.column(), area.height(), area.width());

	updateCells();
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
	else if (simpleState->contains("TABLE_MERGE_CELLS"))
	{
		restoreTableMergeCells(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_UNMERGE_CELLS"))
	{
		restoreTableUnmergeCells(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_INSERT_ROWS"))
	{
		restoreTableInsertRows(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_INSERT_COLUMNS"))
	{
		restoreTableInsertColumns(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_APPEND_ROWS"))
	{
		restoreTableAppendRows(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_APPEND_COLUMNS"))
	{
		restoreTableAppendColumns(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_REMOVE_ROWS"))
	{
		restoreTableRemoveRows(simpleState, isUndo);
		doUpdate = true;
	}
	else if (simpleState->contains("TABLE_REMOVE_COLUMNS"))
	{
		restoreTableRemoveColumns(simpleState, isUndo);
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

bool PageItem_Table::effectiveRTL() const
{
	// A table style's RTL setting overrides the item's own flag, but only when
	// the style (or an ancestor) explicitly defines it; otherwise the item's
	// flag is used.
	if (m_Doc && m_Doc->tableStyles().contains(styleName()))
	{
		const TableStyle& ts = m_Doc->tableStyles().get(styleName());
		if (ts.isDefTableRTL())
			return ts.tableRTL();
	}
	return isRTL();
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

void PageItem_Table::restoreTableMergeCells(SimpleState *state, bool isUndo)
{
	int row = state->getInt("ROW");
	int column = state->getInt("COLUMN");
	int numRows = state->getInt("NUM_ROWS");
	int numCols = state->getInt("NUM_COLS");

	QScopedValueRollback<bool> rb(m_Doc->dontResize, true);
	UndoManager::instance()->setUndoEnabled(false);

	if (isUndo)
	{
		// Undo merge = split the merged cell back to its constituents.
		splitCell(row, column, numRows, numCols);
	}
	else
	{
		// Redo merge = merge again.
		mergeCells(row, column, numRows, numCols);
	}

	UndoManager::instance()->setUndoEnabled(true);
}

void PageItem_Table::restoreTableUnmergeCells(SimpleState *state, bool isUndo)
{
	int row = state->getInt("ROW");
	int column = state->getInt("COLUMN");
	int numRows = state->getInt("NUM_ROWS");
	int numCols = state->getInt("NUM_COLS");

	QScopedValueRollback<bool> rb(m_Doc->dontResize, true);
	UndoManager::instance()->setUndoEnabled(false);

	if (isUndo)
	{
		// Undo split = re-merge.
		mergeCells(row, column, numRows, numCols);
	}
	else
	{
		// Redo split = split again.
		splitCell(row, column, numRows, numCols);
	}

	UndoManager::instance()->setUndoEnabled(true);
}

void PageItem_Table::restoreTableInsertRows(SimpleState *state, bool isUndo)
{
	int index = state->getInt("INDEX");
	int numRows = state->getInt("NUM_ROWS");

	QScopedValueRollback<bool> rb(m_Doc->dontResize, true);
	UndoManager::instance()->setUndoEnabled(false);

	if (isUndo)
		removeRows(index, numRows);
	else
		insertRows(index, numRows);

	adjustTable();
	updateClip();

	UndoManager::instance()->setUndoEnabled(true);
}

void PageItem_Table::restoreTableInsertColumns(SimpleState *state, bool isUndo)
{
	int index = state->getInt("INDEX");
	int numColumns = state->getInt("NUM_COLUMNS");

	QScopedValueRollback<bool> rb(m_Doc->dontResize, true);
	UndoManager::instance()->setUndoEnabled(false);

	if (isUndo)
		removeColumns(index, numColumns);
	else
		insertColumns(index, numColumns);

	adjustTable();
	updateClip();

	UndoManager::instance()->setUndoEnabled(true);
}

void PageItem_Table::restoreTableAppendRows(SimpleState *state, bool isUndo)
{
	const int index = state->getInt("INDEX");
	const int numRows = state->getInt("NUM_ROWS");
	const double oldWidth = state->getDouble("OLD_WIDTH");
	const double oldHeight = state->getDouble("OLD_HEIGHT");

	UndoBlocker ub;

	if (isUndo)
	{
		// Remove the appended rows and shrink the frame back to its
		// pre-append size.
		removeRows(index, numRows);
		setWidthHeight(oldWidth, oldHeight);
	}
	else
	{
		// Re-append the rows (at the last row's height) and grow the frame
		// to fit — mirroring appendRows, not the squash-to-fit adjustTable.
		insertRows(index, numRows);
		adjustFrameToTable();
	}

	updateClip();
}

void PageItem_Table::restoreTableAppendColumns(SimpleState *state, bool isUndo)
{
	const int index = state->getInt("INDEX");
	const int numColumns = state->getInt("NUM_COLUMNS");
	const double oldWidth = state->getDouble("OLD_WIDTH");
	const double oldHeight = state->getDouble("OLD_HEIGHT");

	UndoBlocker ub;

	if (isUndo)
	{
		// Remove the appended columns and shrink the frame back to its
		// pre-append size.
		removeColumns(index, numColumns);
		setWidthHeight(oldWidth, oldHeight);
	}
	else
	{
		// Re-append the columns (at the last column's width) and grow the
		// frame to fit — mirroring appendColumns, not the squash-to-fit
		// adjustTable.
		insertColumns(index, numColumns);
		adjustFrameToTable();
	}

	updateClip();
}

// Note on undo-flag management: insertRows/insertColumns/removeRows/
// removeColumns all set UndoEnabled to false during their body and then
// unconditionally set it back to true at the end. To keep undo silenced
// across multiple of these calls in a single restore, we re-disable
// after each one.

void PageItem_Table::restoreTableRemoveRows(SimpleState *state, bool isUndo)
{
	auto *is = dynamic_cast<ScItemState<TableRowsSnapshot>*>(state);
	if (!is)
		return;
	const TableRowsSnapshot& snap = is->getItem();
	QScopedValueRollback<bool> rb(m_Doc->dontResize, true);
	UndoBlocker ub;

	if (isUndo)
	{
		insertRows(snap.index, snap.numRows);
		restoreRowsFromSnapshot(snap);
		m_Doc->sizeItem(snap.frameWidth, snap.frameHeight, this);
	}
	else
	{
		removeRows(snap.index, snap.numRows);
		adjustTable();
	}

	updateClip();
}

void PageItem_Table::restoreTableRemoveColumns(SimpleState *state, bool isUndo)
{
	auto *is = dynamic_cast<ScItemState<TableColumnsSnapshot>*>(state);
	if (!is)
		return;
	const TableColumnsSnapshot& snap = is->getItem();
	QScopedValueRollback<bool> rb(m_Doc->dontResize, true);
	UndoBlocker ub;

	if (isUndo)
	{
		insertColumns(snap.index, snap.numColumns);
		restoreColumnsFromSnapshot(snap);
		m_Doc->sizeItem(snap.frameWidth, snap.frameHeight, this);
	}
	else
	{
		removeColumns(snap.index, snap.numColumns);
		adjustTable();
	}

	updateClip();
}
