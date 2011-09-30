/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGEITEM_TABLE_H
#define PAGEITEM_TABLE_H

#include <QList>
#include <QPointF>
#include <QRectF>
#include <QSet>
#include <QString>

#include "cellarea.h"
#include "pageitem.h"
#include "scribusapi.h"
#include "styles/tablestyle.h"
#include "tablecell.h"
#include "tablehandle.h"

class ScPainter;
class ScribusDoc;
class TablePainter;

/**
 * The PageItem_Table class represents a table.
 * <p>
 * A table is a group of cells ordered into rows and columns. Each table contains at least
 * one row and one column.
 * <p>
 * A table's dimensions can be changed by using <code>insertRows()</code>,
 * <code>insertColumns()</code>, <code>removeRows()</code>, or <code>removeColumns()</code>.
 * Use <code>cellAt()</code> to retrieve table cells.
 * <p>
 * Rows and columns can be resized by using <code>resizeRow()</code> and <code>resizeColumn()</code>.
 * The entire table can be resized by using <code>resize()</code>
 * <p>
 * Cells in the table can be merged by using <code>mergeCells()</code>. Cells that have been
 * previously merged can be split up again by using <code>splitCells()</code>.
 * <p>
 * The style of the table can be controlled by setting a named style for the table using
 * <code>setStyle()</code>. Formatting properties can also be set directly on the table.
 * Directly set properties overrides properties from the style.
 */
class SCRIBUS_API PageItem_Table : public PageItem
{
	Q_OBJECT
	Q_ENUMS(ResizeStrategy HitTarget)

	Q_PROPERTY(int rows READ rows NOTIFY changed)
	Q_PROPERTY(int columns READ columns NOTIFY changed)
	Q_PROPERTY(QString fillColor READ fillColor WRITE setFillColor RESET unsetFillColor NOTIFY changed)
	Q_PROPERTY(TableBorder leftBorder READ leftBorder WRITE setLeftBorder RESET unsetLeftBorder NOTIFY changed)
	Q_PROPERTY(TableBorder rightBorder READ rightBorder WRITE setRightBorder RESET unsetRightBorder NOTIFY changed)
	Q_PROPERTY(TableBorder topBorder READ topBorder WRITE setTopBorder RESET unsetTopBorder NOTIFY changed)
	Q_PROPERTY(TableBorder bottomBorder READ bottomBorder WRITE setBottomBorder RESET unsetBottomBorder NOTIFY changed)
	Q_PROPERTY(QString style READ style WRITE setStyle RESET unsetStyle NOTIFY changed)

public:
	/**
	 * This enum specifies two different strategies for resizing rows and columns. With the
	 * <code>MoveFollowing</code> resize strategy, all following rows or columns are moved
	 * when a column is resized. With the <code>ResizeFollowing</code> strategy, the row or
	 * column that immediately follows the resized row or column is resized to match, but
	 * apart from that, all other rows or columns remain unaffected by the resize.
	 */
	enum ResizeStrategy
	{
		MoveFollowing,
		ResizeFollowing
	};

	/// The minimum row height.
	static const qreal MinimumRowHeight;

	/// The minimum column width.
	static const qreal MinimumColumnWidth;

public:
	/// Construct a new table item with @a numRows rows and @a numColumns columns.
	PageItem_Table(ScribusDoc *pa, double x, double y, double w, double h, double w2,
		QString fill, QString outline, int numRows = 1, int numColumns = 1);

	/// Destructor.
	~PageItem_Table();

	/// Returns the number of rows in the table.
	int rows() const { return m_rows; }

	/// Returns the number of columns in the table.
	int columns() const { return m_columns; }

	/**
	 * Returns the width of the table.
	 *
	 * This is the width of the table grid, not including any borders along the left and right side.
	 */
	qreal tableWidth() const { return m_columnPositions.last() + m_columnWidths.last(); }

	/**
	 * Returns the height of the table.
	 *
	 * This is the height of the table grid, not including any borders along the top and left side.
	 */
	qreal tableHeight() const { return m_rowPositions.last() + m_rowHeights.last(); }

	/**
	 * Returns the offset of the table grid from the item origin.
	 */
	QPointF gridOffset() const { return QPointF(maxLeftBorderWidth()/2, maxTopBorderWidth()/2); }

	/**
	 * Returns the effective width of the table.
	 *
	 * The effective table width includes the width of the table grid plus half the width of the widest
	 * borders found along its left and right side.
	 */
	qreal effectiveWidth() const { return tableWidth() + (maxLeftBorderWidth() + maxRightBorderWidth()) / 2; }

	/**
	 * Returns the effective height of the table.
	 *
	 * The effective table height includes the height of the table grid plus half the width of the widest
	 * borders found along its top and bottom side.
	 */
	qreal effectiveHeight() const { return tableHeight() + (maxTopBorderWidth() + maxBottomBorderWidth()) / 2; }

	/**
	 * Resizes the table grid to @a width, @a height.
	 *
	 * The change in width and height will be distributed proportionally to columns and rows.
	 *
	 * If any row or column hits the limit MinimumRowHeight or MinimumColumnWidth, resizing in that
	 * direction will stop for the entire table.
	 */
	void resize(qreal width, qreal height);

	/**
	 * Inserts @a numRows rows before the row at @a index.
	 *
	 * If @a index is rows(), a row is appended to the table.
	 *
	 * If @a index is less than 0 or greater than rows(), or if @a numRows is less than 1,
	 * this method does nothing.
	 */
	void insertRows(int index, int numRows);

	/**
	 * Removes @a numRows rows from the table, starting with the row at @a index.
	 *
	 * If the specified range falls outside the table or the number of rows is
	 * less than 1 then this method does nothing.
	 */
	void removeRows(int index, int numRows);

	/**
	 * Returns the height of @a row, or 0 if @a row does not exist.
	 */
	qreal rowHeight(int row) const;

	/**
	 * Returns the list of row heights for the table.
	 */
	QList<qreal> rowHeights() const { return m_rowHeights; }

	/**
	 * Resizes @a row to @a height using resize strategy @a strategy.
	 *
	 * If @a row does not exists, this method does nothing.
	 */
	void resizeRow(int row, qreal height, ResizeStrategy strategy = MoveFollowing);

	/**
	 * Distributes the rows from @a startRow to @a endRow evenly.
	 *
	 * All rows in the range will have the same size while the height of the entire range will
	 * remain unaffected.
	 *
	 * If the range is not in the table, this method does nothing.
	 */
	void distributeRows(int startRow, int endRow);

	/**
	 * Returns the position of @a row, or 0 if @a row does not exist.
	 */
	qreal rowPosition(int row) const;

	/**
	 * Returns the list of row positions for the table.
	 */
	QList<qreal> rowPositions() const { return m_rowPositions; }

	/**
	 * Inserts @a numColumns columns before the column at @a index.
	 *
	 * If @a index is columns(), a column is appended to the table.
	 *
	 * If @a index is less than 0 or greater than columns(), or if @a numColumns is less than 1,
	 * this method does nothing.
	 */
	void insertColumns(int index, int numColumns);

	/**
	 * Removes @a numColumns columns from the table, starting with the column at @a index.
	 *
	 * If the specified range falls outside the table or the number of columns is
	 * less than 1 then this method does nothing.
	 */
	void removeColumns(int index, int numColumns);

	/**
	 * Returns the width of @a column, or 0 if @a column does not exist.
	 */
	qreal columnWidth(int column) const;

	/**
	 * Returns the list of column widths for the table.
	 */
	QList<qreal> columnWidths() const { return m_columnWidths; }

	/**
	 * Resizes @a column to @a width using resize strategy @a strategy.
	 *
	 * If @a column does not exists, this method does nothing.
	 */
	void resizeColumn(int column, qreal width, ResizeStrategy strategy = MoveFollowing);

	/**
	 * Distributes the columns from @a startColumn to @a endColumn evenly.
	 *
	 * All columns in the range will have the same size while the width of the entire range will
	 * remain unaffected.
	 *
	 * If the range is not in the table, this method does nothing.
	 */
	void distributeColumns(int startColumn, int endColumn);

	/**
	 * Returns the position of @a column, or 0 if @a column does not exist.
	 */
	qreal columnPosition(int column) const;

	/**
	 * Returns the list of column positions for the table.
	 */
	QList<qreal> columnPositions() const { return m_columnPositions; }

	/**
	 * Merges the cell at the specified @a row and @a column with the adjacent cells into
	 * one cell.
	 *
	 * The new cell will span @a numRows rows and @a numCols columns. If @a numRows or
	 * @a numCols is less than the current number of rows or columns the cell spans then
	 * this method does nothing.
	 *
	 * If any of the merged cells are already merged with other cells, the two areas of
	 * merged cells will be united.
	 */
	void mergeCells(int row, int column, int numRows, int numCols);

	/**
	 * Splits the specified cell at @a row and @a column into an array of multiple cells with
	 * dimensions specified by @a numRows and @a numCols.
	 */
	void splitCell(int row, int column, int numRows, int numCols);

	/**
	 * Returns the set of selected cells.
	 */
	QSet<TableCell> selectedCells() const { return m_selection; }

	/**
	 * Returns the set of selected rows.
	 *
	 * Any row that is spanned by one of the selected cells is considered selected.
	 */
	QSet<int> selectedRows() const;

	/**
	 * Returns the set of selected columns.
	 *
	 * Any column that is spanned by one of the selected cells is considered selected.
	 */
	QSet<int> selectedColumns() const;

	/**
	 * Adds the cell at @a row, @column to the selection.
	 *
	 * If there's no cell at @a row, @a column, this function does nothing.
	 */
	void selectCell(int row, int column);

	/**
	 * Adds the cells in the area between two cells to the selection.
	 *
	 * This functions first fetches the two cells at @a startRow, @a startColumn and
	 * @a endRow, @a endColumn and then selects all cells that intersect the rectangular
	 * area of cells that encloses the two cells.
	 *
	 * If any of the specified cell locations are outside the table, this function does nothing.
	 */
	void selectCells(int startRow, int startColumn, int endRow, int endColumn);

	/**
	 * Clears the cell selection.
	 */
	void clearSelection();

	/**
	 * Returns the cell at @a row, @a column.
	 *
	 * If the cell is covered by a spanning cell, the spanning cell is returned. If the cell is
	 * not in this table, an invalid cell is returned. The table may later mark cells returned
	 * by this function as invalid, if the row or column containing the cell is removed.
	 */
	TableCell cellAt(int row, int column) const;

	/**
	 * Returns the cell at @a point, which is in canvas coordinates.
	 *
	 * If the cell at @a point is covered by a spanning cell, the spanning cell is returned.
	 * If @a point is outside the table grid, an invalid cell is returned. The table may later
	 * mark cells returned by this function as invalid, if the row or column containing the
	 * cell is removed.
	 */
	TableCell cellAt(const QPointF& point) const;

	/**
	 * Returns the currently active cell.
	 *
	 * A table always has a valid active cell. This is the cell containing the cursor and that
	 * will receive keyboard input during cell text editing.
	 */
	TableCell activeCell() const { return m_activeCell; }

	/**
	 * Moves left in the table, or do nothing if at the table end.
	 */
	void moveLeft();

	/**
	 * Moves right in the table, or do nothing if at the table end.
	 */
	void moveRight();

	/**
	 * Moves up in the table, or do nothing if at the table end.
	 */
	void moveUp();

	/**
	 * Moves down in the table, or do nothing if at the table end.
	 */
	void moveDown();

	/**
	 * Moves to @a cell, or the cell at row 0, column 0 if @a cell is invalid.
	 */
	void moveTo(const TableCell& cell);

	/**
	 * Performs a hit test at @a point, which is in canvas coordinates.
	 *
	 * The returned handle describes what was hit. @a threshold is a distance in points.
	 * @a point is considered to hit a handle if it is within @a threshold from it.
	 */
	TableHandle hitTest(const QPointF& point, qreal threshold) const;

	/// Resizes the table to fit the frame.
	void adjustTableToFrame();

	/// Resizes the frame to fit the table.
	void adjustFrameToTable();

	/// Sets the fill color of this table to @a color.
	void setFillColor(const QString& color);

	/// Unsets the fill color of this table.
	void unsetFillColor();

	/// Returns the fill color of this table.
	QString fillColor() const;

	/// Sets the left border of this table to @a border.
	void setLeftBorder(const TableBorder& border);

	/// Unsets the left border of this table.
	void unsetLeftBorder();

	/// Returns the left border of this table.
	TableBorder leftBorder() const;

	/// Sets the right border of this table to @a border.
	void setRightBorder(const TableBorder& border);

	/// Unsets the right border of this table.
	void unsetRightBorder();

	/// Returns the right border of this table.
	TableBorder rightBorder() const;

	/// Sets the top border of this table to @a border.
	void setTopBorder(const TableBorder& border);

	/// Unsets the top border of this table.
	void unsetTopBorder();

	/// Returns the top border of this table.
	TableBorder topBorder() const;

	/// Sets the bottom border of this table to @a border.
	void setBottomBorder(const TableBorder& border);

	/// Unsets the bottom border of this table.
	void unsetBottomBorder();

	/// Returns the bottom border of this table.
	TableBorder bottomBorder() const;

	/// Sets the table style of this table to @a style.
	void setStyle(const QString& style);

	/// Unsets the style of this table.
	void unsetStyle();

	/// Returns the style of this table.
	QString style() const;

	/// Updates the position and size of all cell text frames for this table.
	void updateCells() { updateCells(0, 0, rows() - 1, columns() - 1); }

	/// Updates the position and size of cell text frames for cells in the specified area.
	void updateCells(int startRow, int startColumn, int endRow, int endColumn);

	/// Returns <code>true</code> if the table is overflowing its frame.
	bool isOverflowing() const { return effectiveWidth() > width() || effectiveHeight() > height(); }

	/// Returns this item as a PageItem_Table.
	virtual PageItem_Table* asTable() { return this; }

	/// Returns <code>true</code>.
	virtual bool isTable() const { return true; }

	/// Returns PageItem::Table.
	virtual ItemType realItemType() const { return PageItem::Table; }

	/// Adds the applicable actions for this table to @a actionList.
	virtual void applicableActions(QStringList& actionList);

	/// Returns a textual description of this item.
	virtual QString infoDescription() { return QString(); }

	/// Returns the Cell Areas from this table
	QList<CellArea> cellAreas() const { return m_cellAreas; }

	/// Returns the rows of the table for writing to SLA
	QList<QList<TableCell> > cellRows() const { return m_cellRows; }

signals:
	/// This signal is emitted whenever the table changes.
	void changed();
	/// This signal is emitted whenever the cell selection changes.
	void selectionChanged();

private slots:
	/// Handles cell and table style changes in the document.
	void handleStyleChanged();

protected:
	/// Paints this item.
	virtual void DrawObj_Item(ScPainter *p, QRectF clipRect);

private:
	/// Enum describing types of changes on a table. For internal use.
	enum ChangeType
	{
		RowsInserted,    /**< Rows were inserted. */
		RowsRemoved,     /**< Rows were removed. */
		ColumnsInserted, /**< Columns were inserted. */
		ColumnsRemoved   /**< Columns were removed. */
	};

	/**
	 * Initializes the table with @a numRows rows and @a numColumns columns.
	 *
	 * Should be called once, and once only, during table construction.
	 */
	void initialize(int numRows, int numColumns);

	/// Activates the cell @a cell, or the cell at row 0, column 0 if @a cell is invalid.
	void activateCell(const TableCell& cell);

	/// Returns true if @a row is a row in this table.
	bool validRow(int row) const { return row >= 0 && row < m_rows; }
	/// Returns true if @a column is a column in this table.
	bool validColumn(int column) const { return column >= 0 && column < m_columns; }
	/// Returns true if there is a cell at @a row, @a column in this table.
	bool validCell(int row, int column) const { return validRow(row) && validColumn(column); }

	/// Returns the width of the widest border along the left side of this table.
	qreal maxLeftBorderWidth() const;

	/// Returns the width of the widest border along the right side of this table.
	qreal maxRightBorderWidth() const;

	/// Returns the width of the widest border along the top side of this table.
	qreal maxTopBorderWidth() const;

	/// Returns the width of the widest border along the bottom side of this table.
	qreal maxBottomBorderWidth() const;

	/// TODO: Turn these into strategies to be reused in resize gestures.
	/// Resizes @a row according to the MoveFollowing strategy and returns the new height.
	qreal resizeRowMoveFollowing(int row, qreal height);
	/// Resizes @a row according to the ResizeFollowing strategy and returns the new height.
	qreal resizeRowResizeFollowing(int row, qreal height);
	/// Resizes @a column according to the MoveFollowing strategy and returns the new width.
	qreal resizeColumnMoveFollowing(int row, qreal width);
	/// Resizes @a column according to the ResizeFollowing strategy and returns the new width.
	qreal resizeColumnResizeFollowing(int row, qreal width);

	/**
	 * Updates row and column spans following a change in rows or columns.
	 *
	 * If @a changeType is <code>RowsInserted</code> or <code>ColumnsInserted</code>, @a index
	 * and @a number specifies that @a number rows or columns were inserted before the row or
	 * or column at @a index. Similarly, if @a changeType is <code>RowsRemoved</code> or
	 * <code>ColumnsRemoved</code>, @a index and @a number specifies that @a number rows or
	 * columns were removed, starting with the row or column at @a index.
	 */
	void updateSpans(int index, int number, ChangeType changeType);

	/// Prints internal table information. For internal use.
	void debug() const;

	/// Table sanity check. Very slow. For internal use.
	void assertValid() const;

private:
	//<<Data we need to save
	/// List of rows of cells in the table.
	QList<QList<TableCell> > m_cellRows;

	/// Number of rows.
	int m_rows;
	/// Number of columns.
	int m_columns;

	/// Vertical positions of rows.
	QList<qreal> m_rowPositions;
	/// Height of rows.
	QList<qreal> m_rowHeights;

	/// Horizontal positions of columns.
	QList<qreal> m_columnPositions;
	/// Width of columns.
	QList<qreal> m_columnWidths;

	/// List of areas of merged cells.
	QList<CellArea> m_cellAreas;

	/// Style of the table.
	TableStyle m_style;
	//>>End of data we need to save
	//-----------------------------
	//<<Live working variables/data
	/// Set of selected cells.
	QSet<TableCell> m_selection;

	/// The table painter to paint the table with.
	TablePainter* m_tablePainter;

	/// Currently active cell.
	TableCell m_activeCell;

	/*
	 * The two members below describe the active "logical position" on the table grid.
	 * This position may or may not correspond with the position of the active cell.
	 *
	 * As an example, imagine the following scenario:
	 *
	 * +--------+--------+                            +--------+--------+
	 * |        |        |                            |        | |      |
	 * +--------+        |  --> User presses Tab -->  +--------+        |
	 * | |      |        |                            |        |        |
	 * +--------+--------+                            +--------+--------+
	 *  Cursor in (1, 0)                               Cursor in (0, 1)
	 *
	 * The active cell is now the spanning cell at (0, 1). If we would use the row and
	 * column of the active cell as the basis for keyboard navigation between cells,
	 * then pressing Shift+Tab after the scenario above would bring the cursor into
	 * cell (0, 0), not back to to (1, 0). Hence, in addition to the active cell, we
	 * keep track of a "logical position". This is the logical position on the table
	 * grid that is active. In the above scenario, the logical position would change
	 * from (1, 0) to (1, 1).
	 */

	/// The logical active row.
	int m_activeRow;

	/// The logical active column.
	int m_activeColumn;
	//>>End of live working variables/data
};

#endif // PAGEITEM_TABLE_H
