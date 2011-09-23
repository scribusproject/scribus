/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CANVASGESTURE_CELLSELECT_H
#define CANVASGESTURE_CELLSELECT_H

#include "tablecell.h"

#include "canvasgesture_table.h"

class PageItem_Table;
class QMouseEvent;
class QKeyEvent;
class QPainter;

/**
 * Handles selection of cells on behalf of the table editing canvas mode.
 *
 * A single mouse click on a cell will select the cell. A single mouse click with
 * the Ctrl modifier on a cell will deselect the cell. Pressing the mouse button
 * on a cell and dragging it to another cell will select all cells that start in
 * the rectangle between the first and the second cell.
 *
 * The selection gesture may be cancelled by pressing the Escape key.
 */
class SCRIBUS_API CellSelect : public TableGesture
{
	Q_OBJECT

public:
	explicit CellSelect(CanvasMode* parent) : TableGesture(parent) {}
	virtual ~CellSelect() {}

	virtual void activate(bool fromGesture) {}
	virtual void deactivate(bool forGesture);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual bool handleKeyEvents() { return true; }
	virtual void drawControls(QPainter* p);

	/// Sets up this gesture to select cells in @a table, starting with @a cell.
	void setup(PageItem_Table* table, const TableCell& cell);

private:
	/// The start cell of the current selection.
	TableCell m_startCell;
	/// The end cell of the current selection.
	TableCell m_endCell;
};

#endif // CANVASGESTURE_CELLSELECT_H
