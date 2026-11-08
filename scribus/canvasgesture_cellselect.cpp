/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "canvasgesture_cellselect.h"

#include <algorithm>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>

#include "canvas.h"
#include "fpoint.h"
#include "pageitem_table.h"
#include "scribus.h"
#include "scribusview.h"
#include "tablecell.h"
#include "ui/scmwmenumanager.h"

void CellSelect::activate(bool fromGesture)
{
	CanvasGesture::activate(fromGesture);
	m_view->setCursor(Qt::IBeamCursor);
}

void CellSelect::deactivate(bool forGesture)
{
	// Don't clear selection here -- callers decide whether to clear based
	// on context. This lets right-click preserve the selection while the
	// gesture stops to allow context menu display.
	CanvasGesture::deactivate(forGesture);
}

void CellSelect::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		// Cancel the cell selection.
		event->accept();
		table()->clearSelection();
		m_view->stopGesture();
		return;
	}

	// Any other key ends the gesture and is forwarded to the canvas mode,
	// so navigation keys can extend or alter the selection.
	m_view->stopGesture();
	delegate()->keyPressEvent(event);
}

void CellSelect::mousePressEvent(QMouseEvent* event)
{
	event->accept();

	if (event->button() != Qt::RightButton)
	{
		// For non-right-click, clear the cell selection -- the user is
		// starting a fresh interaction.
		table()->clearSelection();
	}

	// Stop the gesture and forward to the canvas mode for handling.
	m_view->stopGesture();
	delegate()->mousePressEvent(event);
}

void CellSelect::mouseReleaseEvent(QMouseEvent* event)
{
	event->accept();

	// Reset start and end cells.
	m_startCell = TableCell();
	m_endCell = TableCell();

	m_view->m_ScMW->updateTableMenuActions();
	m_view->stopGesture();
}

void CellSelect::mouseMoveEvent(QMouseEvent* event)
{
	event->accept();

	TableCell newCell = table()->cellAt(m_canvas->globalToCanvas(event->globalPosition()).toQPointF());

	if (newCell == m_endCell || !newCell.isValid() || !m_startCell.isValid())
		return;

	m_endCell = newCell;

	// Select the new area, extending by the gesture's unit.
	table()->clearSelection();
	switch (m_mode)
	{
		case Columns:
			table()->selectCells(0, m_startCell.column(), table()->rows() - 1, m_endCell.column());
			break;
		case Rows:
			table()->selectCells(m_startCell.row(), 0, m_endCell.row(), table()->columns() - 1);
			break;
		case Cells:
		default:
			table()->selectCells(m_startCell.row(), m_startCell.column(), m_endCell.row(), m_endCell.column());
			break;
	}
	table()->moveTo(newCell);
	m_canvas->update();
}

void CellSelect::drawControls(QPainter* p)
{
	p->save();
	commonDrawControls(p, false);
	p->restore();

	paintCellSelection(p);
}

void CellSelect::setup(PageItem_Table* table, const TableCell& cell, SelectionMode mode)
{
	Q_ASSERT(table);
	Q_ASSERT(cell.isValid());

	setTable(table);

	m_startCell = cell;
	m_endCell = cell;
	m_mode = mode;

	switch (m_mode)
	{
		case Columns:
			table->selectColumn(cell.column());
			break;
		case Rows:
			table->selectRow(cell.row());
			break;
		case Cells:
		default:
			table->selectCell(cell.row(), cell.column());
			break;
	}
}
