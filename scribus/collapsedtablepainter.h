/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef COLLAPSEDTABLEPAINTER_H
#define COLLAPSEDTABLEPAINTER_H

#include <QtGlobal>

#include "tablepainter.h"
#include "tableborder.h"

class PageItem_Table;
class TableCell;
class ScPainter;

/**
 * CollapsedTablePainter is a table painter for painting a table using the collapsed border model.
 */
class CollapsedTablePainter : public TablePainter
{
public:
	/// Creates a new collapsed table painter configured to paint @a table.
	explicit CollapsedTablePainter(PageItem_Table* table) : TablePainter(table) {}

	/// Paints the table using @a p and returns the table offset.
	virtual void paintTable(ScPainter* p);

private:
	/// Paints the fill of the table.
	void paintTableFill(ScPainter* p) const;
	/// Paints all of the borders along the left side of @a cell.
	void paintCellLeftBorders(const TableCell& cell, ScPainter* p) const;
	/// Paints all of the borders along the right side of @a cell.
	void paintCellRightBorders(const TableCell& cell, ScPainter* p) const;
	/// Paints all of the borders along the top side of @a cell.
	void paintCellTopBorders(const TableCell& cell, ScPainter* p) const;
	/// Paints all of the borders along the bottom side of @a cell.
	void paintCellBottomBorders(const TableCell& cell, ScPainter* p) const;
	/// Paints the fill of @a cell.
	void paintCellFill(const TableCell& cell, ScPainter* p) const;

	/**
	 * Paints @a border from @a start to @a end.
	 *
	 * @a startOffsetFactors and @a endOffsetFactors specifies offset factors for the individual
	 * border lines of @a border. Each start and end point of the border lines will be offset by
	 * their own width multiplied by these factors before being painted.
	 */
	void paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end,
		const QPointF& startOffsetFactors, const QPointF& endOffsetFactors, ScPainter *p) const;

	/// Paints a decorative thin blue line from @a start to @a end.
	void paintGridLine(const QPointF& start, const QPointF& end, ScPainter *p) const;

};

#endif // COLLAPSEDTABLEPAINTER_H
