/*
 * Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>
 *
 * For general Scribus (>=1.3.2) copyright and licensing information please refer
 * to the COPYING file provided with the program. Following this notice may exist
 * a copyright and/or license notice that predates the release of Scribus 1.3.2
 * for which a new license (GPL+exception) is in place.
 */

#include <QDebug>

#include "cellarea.h"

CellArea::CellArea() :
	m_row(-1), m_column(-1), m_width(-1), m_height(-1)
{
}

CellArea::CellArea(int row, int column, int width, int height) :
	m_row(row), m_column(column), m_width(width), m_height(height)
{
}

CellArea::CellArea(const CellArea& other) : m_row(other.m_row), m_column(other.m_column),
	m_width(other.m_width), m_height(other.m_height)
{
}

bool CellArea::contains(int row, int column) const
{
	if (!isValid())
		return false;

	return row >= this->row() && row <= bottom() &&
			column >= this->column() && column <= right();
}

bool CellArea::contains(const CellArea& area) const
{
	if (!isValid() && !area.isValid())
		return false;

	return area.column() >= column() && area.right() <= right() &&
			area.row() >= row() && area.bottom() <= bottom();
}

bool CellArea::intersects(const CellArea& area)
{
	if (!isValid() && !area.isValid())
		return false;

	// !(outside) || contains(area)
	return !(area.column() > right() || area.right() < column() ||
			 area.bottom() < row() || area.row() > bottom()) || contains(area);
}

CellArea CellArea::translated(int rows, int columns) const
{
	CellArea area(row() + rows, column() + columns, width(), height());
	return area;
}

void CellArea::translate(int rows, int columns)
{
	m_row += rows;
	m_column += columns;
}

CellArea CellArea::adjusted(int rows, int columns, int width, int height) const
{
	CellArea area(row() + rows, column() + columns, this->width() + width, this->height() + height);
	return area;
}

void CellArea::adjust(int rows, int columns, int width, int height)
{
	m_row += rows;
	m_column += columns;
	m_width += width;
	m_height += height;
}

CellArea CellArea::united(CellArea& area) const
{
	int newRow = qMin(row(), area.row());
	int newColumn = qMin(column(), area.column());
	int newWidth = qMax(right() - newColumn + 1, area.right() - newColumn + 1);
	int newHeight = qMax(bottom() - newRow + 1, area.bottom() - newRow + 1);
	CellArea unitedArea(newRow, newColumn, newWidth, newHeight);
	return unitedArea;
}

CellArea CellArea::adjustedForRowInsertion(int index, int numRows)
{
	CellArea adjustedArea(*this);

	if (numRows < 1 || index > bottom())
		return adjustedArea; // No rows inserted, or inserted after area.

	if (index <= row())
		adjustedArea.adjust(numRows, 0, 0, 0); // Inserted before area.
	else
		adjustedArea.adjust(0, 0, 0, numRows); // Inserted inside area.

	return adjustedArea;
}

CellArea CellArea::adjustedForRowRemoval(int index, int numRows)
{
	CellArea adjustedArea(*this);

	if (numRows < 1 || index > bottom())
		return adjustedArea; // No rows removed, or removed after area.

	int end = index + numRows - 1;
	if (end < row())
	{
		// Removed before area.
		adjustedArea.adjust(-numRows, 0, 0, 0);
	}
	else
	{
		// Removed inside area.
		int removedInsideArea = qMin(bottom(), end) - qMax(row(), index) + 1;
		adjustedArea.adjust(0, 0, 0, -removedInsideArea);
	}

	return adjustedArea;
}

CellArea CellArea::adjustedForColumnInsertion(int index, int numColumns)
{
	CellArea adjustedArea(*this);

	if (numColumns < 1 || index > right())
		return adjustedArea; // No columns inserted, or inserted after area.

	if (index <= column())
		adjustedArea.adjust(0, numColumns, 0, 0); // Inserted before area.
	else
		adjustedArea.adjust(0, 0, numColumns, 0); // Inserted inside area.

	return adjustedArea;
}

CellArea CellArea::adjustedForColumnRemoval(int index, int numColumns)
{
	CellArea adjustedArea(*this);

	if (numColumns < 1 || index > right())
		return adjustedArea; // No columns removed, or removed after area.

	int end = index + numColumns - 1;
	if (end < column())
	{
		// Removed before area.
		adjustedArea.adjust(0, -numColumns, 0, 0);
	}
	else
	{
		// Removed inside area.
		int removedInsideArea = qMin(right(), end) - qMax(column(), index) + 1;
		adjustedArea.adjust(0, 0, -removedInsideArea, 0);
	}

	return adjustedArea;
}

bool operator==(const CellArea& lhs, const CellArea& rhs)
{
	return lhs.row() == rhs.row() && lhs.column() == rhs.column() &&
			lhs.width() == rhs.width() && lhs.height() == rhs.height();
}

bool operator!=(const CellArea& lhs, const CellArea& rhs)
{
	return lhs.row() != rhs.row() || lhs.column() != rhs.column() ||
			lhs.width() != rhs.width() || lhs.height() != rhs.height();
}

QDebug operator<<(QDebug debug, const CellArea& area)
{
	debug.nospace() << "(" << area.row() << ", " << area.column() << " " << area.width() << "x" << area.height() << ")";
	return debug.space();
}
