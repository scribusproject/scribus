/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CANVASGESTURE_TABLE_H
#define CANVASGESTURE_TABLE_H

#include <QList>
#include <QPoint>

#include "canvasgesture.h"
#include "canvasmode.h"

class PageItem_Table;
class QPainter;
class ScribusView;

/**
 * Base class for table editing canvas gestures.
 *
 * TableGesture is a base class for the gestures for table editing gestures used
 * by the table editing canvas mode. It simply holds the currently edited table
 * and provides some functions common to the table editing gestures. Apart from
 * that it leaves everything else up to the subclasses.
 */
class SCRIBUS_API TableGesture : public CanvasGesture
{
	Q_OBJECT

public:
	virtual ~TableGesture() {}

protected:
	explicit TableGesture(CanvasMode* parent) : CanvasGesture(parent->view()) {};
	explicit TableGesture(ScribusView* view) : CanvasGesture(view) {};

	/// Returns the currently edited table.
	PageItem_Table* table() const { return m_table; }

	/// Sets the currently edited table to @a table.
	void setTable(PageItem_Table* table) { m_table = table; }

	/**
	 * Returns a copy of @a point, which is in global coordinates, mapped to coordinates
	 * on the table grid of the currently edited table. If @a snap is <code>true</code>,
	 * @a point is first snapped to the page grid and guides, in that order.
	 */
	QPointF globalToTableGrid(const QPoint point, bool snap = true);

	/**
	 * Paints the outline of the currently edited table.
	 *
	 * Instead of using row and column geometries of the table, they are taken from the
	 * parameters @a rowHeights, @a rowPositions, @a columnWidths and @a columnPositions.
	 * The size of these vectors must match the dimensions of the currently edited table,
	 * or the result of this function is undefined.
	 */
	void paintTableOutline(const QList<qreal>& rowHeights, const QList<qreal>& rowPositions,
		const QList<qreal>& columnWidths, const QList<qreal>& columnPositions, QPainter* p);

	/**
	 * Paints the selection of the currently edited table.
	 */
	void paintCellSelection(QPainter* p);

private:
	/// The currently edited table.
	PageItem_Table* m_table;
};

#endif // CANVASGESTURE_TABLE_H
