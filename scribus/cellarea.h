/*
 * Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>
 *
 * For general Scribus (>=1.3.2) copyright and licensing information please refer
 * to the COPYING file provided with the program. Following this notice may exist
 * a copyright and/or license notice that predates the release of Scribus 1.3.2
 * for which a new license (GPL+exception) is in place.
 */
#ifndef CELLAREA_H
#define CELLAREA_H

/**
 * The CellArea class is a simple representation of a rectangular area of table cells.
 *
 * CellArea is similar to QRect, but its contains() and intersects() semantics are a bit
 * different and it has a couple of methods that are specifically taylored for table layout
 * purposes. The class is used during table layout when keeping track of areas of merged
 * table cells.
 */
class CellArea
{
public:
	/// Constructs a new invalid area.
	CellArea();

	/// Constructs an area starting at @a row, @a column with the given @a width and @a height.
	CellArea(int row, int column, int width, int height);

	/// Constructs an area as a copy of the area @a other.
	CellArea(const CellArea& other);

	/// Returns <code>true</code> if the area is valid.
	bool isValid() const { return m_width >= 1 && m_height >= 1; }

	/// Returns the start row of the area.
	int row() const { return m_row; }
	/// Sets the start row of the area to @a row.
	void setRow(int row) { m_row = row; }

	/// Returns the start column of the area.
	int column() const { return m_column; }
	/// Sets the start column of the area to @a column.
	void setColumn(int column) { m_column = column; }

	/// Returns the width of the area.
	int width() const { return m_width; }
	/// Sets the width of the area to @a width.
	void setWidth(int width) { m_width = width; }

	/// Returns the height of the area.
	int height() const { return m_height; }
	/// Sets the height of the area to @a height.
	void setHeight(int height) { m_height = height; }

	/// Returns the bottom row of the area.
	int bottom() const { return m_row + m_height - 1; }
	/// Returns the right column of the area.
	int right() const { return m_column + m_width - 1; }

	/// Returns <code>true</code> if this area contains the cell at @a row, @a column.
	bool contains(int row, int column) const;
	/// Returns <code>true</code> if this area contains @a area.
	bool contains(const CellArea& area) const;

	/// Returns <code>true</code> if this area and @a area intersects.
	bool intersects(const CellArea& area);

	/// Returns a copy of this area that has been translated @a rows rows and @a columns columns.
	CellArea translated(int rows, int columns) const;
	/// Translates this area @a rows rows and @a columns columns.
	void translate(int rows, int columns);

	/// Returns a copy of this area with @a rows, @a columns, @a width, @a height added.
	CellArea adjusted(int rows, int columns, int width, int height) const;
	/// Adjusts this area by @a rows, @a columns, @a width, @a height.
	void adjust(int rows, int columns, int width, int height);

	/// Returns the bounding area of this area and @a area.
	CellArea united(CellArea& area) const;

	/**
	 * Returns a copy of this area adjusted for an insertion of @a numRows rows before the
	 * row at @a index.
	 *
	 * If rows are inserted above the area, the adjusted area is moved down. If rows are
	 * inserted inside the area, the adjusted area is increased in height. If rows are inserted
	 * below the area, the returned area is unaffected and a copy of this area.
	 */
	CellArea adjustedForRowInsertion(int index, int numRows);

	/**
	 * Returns a copy of this area adjusted for a removal of @a numRows rows starting with the
	 * row at @a index.
	 *
	 * If rows are removed above the area, the adjusted area is moved up. If rows are removed
	 * inside the area, the adjusted area is decreased in height. If rows are removed below the
	 * area, the returned area is unaffected and a copy of this area.
	 */
	CellArea adjustedForRowRemoval(int index, int numRows);

	/**
	 * Returns a copy of this area adjusted for an insertion of @a numColumns columns before the
	 * column at @a index.
	 *
	 * If columns are inserted left of the area, the adjusted area is moved right. If columns are
	 * inserted inside the area, the adjusted area is increased in width. If columns are inserted
	 * right of the area, the returned area is unaffected and a copy of this area.
	 */
	CellArea adjustedForColumnInsertion(int index, int numColumns);

	/**
	 * Returns a copy of this area adjusted for a removal of @a numColumns columns starting with the
	 * column at @a index.
	 *
	 * If columns are removed left of the area, the adjusted area is moved left. If columns are removed
	 * inside the area, the adjusted area is decreased in width. If columns are removed right of the
	 * area, the returned area is unaffected and a copy of this area.
	 */
	CellArea adjustedForColumnRemoval(int index, int numColumns);

private:
	int m_row;
	int m_column;
	int m_width;
	int m_height;
};
Q_DECLARE_TYPEINFO(CellArea, Q_MOVABLE_TYPE);

bool operator==(const CellArea& lhs, const CellArea& rhs);
bool operator!=(const CellArea& lhs, const CellArea& rhs);
QDebug operator<<(QDebug debug, const CellArea& area);

#endif // CELLAREA_H
