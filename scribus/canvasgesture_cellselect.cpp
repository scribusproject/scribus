/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <algorithm>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>

#include "canvas.h"
#include "fpoint.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "scribus.h"
#include "scribusview.h"
#include "tablecell.h"
#include "tableutils.h"
#include "ui/scmwmenumanager.h"

#include "canvasgesture_cellselect.h"

void CellSelect::deactivate(bool forGesture)
{
	table()->clearSelection();
}

void CellSelect::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		// Cancel the cell selection.
		event->accept();
		m_view->stopGesture();
	}
}

void CellSelect::mousePressEvent(QMouseEvent* event)
{
	event->accept();

	if (event->button() == Qt::RightButton)
	{
		// Show the table popup menu.
		qApp->changeOverrideCursor(Qt::ArrowCursor);
		m_view->m_ScMW->scrMenuMgr->runMenuAtPos("ItemTable", event->globalPos());
	}
	else
	{
		// Clear selection.
		table()->clearSelection();

		// Stop the gesture.
		m_view->stopGesture();

		// Pass the mouse press to the delegate (table editing canvas mode).
		delegate()->mousePressEvent(event);
	}
}

void CellSelect::mouseReleaseEvent(QMouseEvent* event)
{
	event->accept();

	// Reset start and end cells.
	m_startCell = TableCell();
	m_endCell = TableCell();

	m_view->m_ScMW->updateTableMenuActions();
}

void CellSelect::mouseMoveEvent(QMouseEvent* event)
{
	event->accept();

	TableCell newCell = table()->cellAt(m_canvas->globalToCanvas(event->globalPos()).toQPointF());

	if (newCell == m_endCell || !newCell.isValid() || !m_startCell.isValid())
		return;

	m_endCell = newCell;

	// Select the new area.
	table()->clearSelection();
	table()->selectCells(
		m_startCell.row(), m_startCell.column(), m_endCell.row(), m_endCell.column());

	m_canvas->update();
}

void CellSelect::drawControls(QPainter* p)
{
	p->save();
	commonDrawControls(p, false);
	p->restore();

	paintCellSelection(p);
}

void CellSelect::setup(PageItem_Table* table, const TableCell& cell)
{
	Q_ASSERT(table);
	Q_ASSERT(cell.isValid());

	setTable(table);

	m_startCell = cell;
	m_endCell = cell;

	table->selectCell(cell.row(), cell.column());
}
