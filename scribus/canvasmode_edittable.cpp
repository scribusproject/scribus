/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCursor>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QPointF>
#include <QSet>
#include <QTimer>

#include "appmodes.h"
#include "canvas.h"
#include "canvasgesture_cellselect.h"
#include "canvasgesture_columnresize.h"
#include "canvasgesture_rowresize.h"
#include "canvasgesture_tableresize.h"
#include "canvasmode_edittable.h"
#include "fpoint.h"
#include "iconmanager.h"
#include "pageitem_table.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "tablehandle.h"
#include "ui/contextmenu.h"
#include "ui/hruler.h"
#include "undomanager.h"

// Sizes, in screen pixels, and colours of the on-canvas table controls
constexpr double ControlDiameterPx = 14.0;
constexpr double ControlHitDiameterPx = 18.0;
constexpr double ResizeGripLongPx = 14.0;
constexpr double ResizeGripShortPx = 8.0;
constexpr double ResizeGripOffsetPx = 5.0;
constexpr double InsertControlOffsetPx = 22.0;
constexpr double DeleteControlOffsetPx = 18.0;
constexpr double CellActionButtonPx = 14.0;
constexpr double CellActionMinButtonPx = 8.0;
constexpr double CellActionHitPx = 18.0;
constexpr double CellActionIconPx = 10.0;
constexpr double CellActionMarginPx = 2.0;
constexpr double CanvasControlExtentPx = 36.0;
constexpr QColor ControlColor(50, 150, 220, 230);
constexpr QColor ControlGlyphColor(255, 255, 255);

CanvasMode_EditTable::CanvasMode_EditTable(ScribusView* view) : CanvasMode(view),
	m_canvasUpdateTimer(new QTimer(view)),
	m_selectRowCursor(IconManager::instance().loadCursor("cursor-select-row")),
	m_selectColumnCursor(IconManager::instance().loadCursor("cursor-select-column")),
	m_tableResizeGesture(new TableResize(this)),
	m_rowResizeGesture(new RowResize(this)),
	m_columnResizeGesture(new ColumnResize(this)),
	m_cellSelectGesture(new CellSelect(this)),
	m_ScMW(view->m_ScMW)
{
	connect(m_canvasUpdateTimer, SIGNAL(timeout()), this, SLOT(updateCanvas()));
}

CanvasMode_EditTable::~CanvasMode_EditTable()
{
	delete m_tableResizeGesture;
	delete m_rowResizeGesture;
	delete m_columnResizeGesture;
	delete m_cellSelectGesture;
}

void CanvasMode_EditTable::activate(bool fromGesture)
{
	CanvasMode::activate(fromGesture);

	m_canvas->resetRenderMode();

	PageItem *item = m_doc->m_Selection->itemAt(0);
	Q_ASSERT(item && item->isTable());
	m_table = item->asTable();
	m_hoverTableCanvasControl = TableCanvasControl();
	m_tableCanvasControlPressed = false;
	if (!fromGesture)
	{
		m_dragSelectionMode = CellSelect::Cells;
		// Reloaded on each entry so they follow icon set and screen changes
		m_mergeIcon = controlGlyph("chain-closed");
		m_unmergeIcon = controlGlyph("chain-open");
	}

	if (fromGesture)
		m_view->setCursor(Qt::ArrowCursor);

	PageItem_TextFrame* tf = m_table->activeCell().textFrame();
	m_lastCursorPos = tf ? tf->itemText.cursorPosition() : -1;
	m_blinkTime.start();
	m_canvasUpdateTimer->start(200);
	makeLongTextCursorBlink();

	m_view->m_ScMW->updateTableMenuActions();

	// Set up the horizontal ruler for the active cell on entry. HaveNewSel
	// runs while appMode is still modeNormal and so cannot do this itself.
	updateTextEditUIForActiveCell();
}

void CanvasMode_EditTable::deactivate(bool forGesture)
{
	if (!forGesture)
		m_canvasUpdateTimer->stop();
	m_hoverTableCanvasControl = TableCanvasControl();
	m_tableCanvasControlPressed = false;

	m_view->m_ScMW->updateTableMenuActions();
	CanvasMode::deactivate(forGesture);
}

void CanvasMode_EditTable::keyPressEvent(QKeyEvent* event)
{
	event->accept();

	bool textChanged = false;

	// Escape: cancel a cell selection, or exit table edit mode.
	if (event->key() == Qt::Key_Escape)
	{
		if (m_table->hasSelection())
		{
			m_table->clearSelection();
			m_view->m_ScMW->updateTableMenuActions();
			updateCanvas(true);
			return;
		}
		PageItem_TextFrame* activeFrame = m_table->activeCell().textFrame();
		activeFrame->itemText.deselectAll();
		activeFrame->HasSel = false;
		m_view->horizRuler->textMode(false);
		m_view->horizRuler->update();
		m_view->requestMode(modeNormal);
		return;
	}

	const int key = event->key();
	const Qt::KeyboardModifiers mods = event->modifiers();
	// Arrow keys may carry KeypadModifier on macOS and some Linux layouts; mask it out before modifier comparisons.
	const Qt::KeyboardModifiers effMods = mods & ~Qt::KeypadModifier;
	const bool isArrow = (key == Qt::Key_Left  || key == Qt::Key_Right || key == Qt::Key_Up    || key == Qt::Key_Down);

	// Insert a literal tab into the active cell instead of navigating.
	// On macOS, Qt maps Cmd to ControlModifier and Cmd+Tab is the system
	// app switcher, so use Option (Alt). Elsewhere Alt+Tab is the window
	// switcher, so use Ctrl.
#if defined(Q_OS_MACOS)
	const Qt::KeyboardModifiers insertTabMod = Qt::AltModifier;
#else
	const Qt::KeyboardModifiers insertTabMod = Qt::ControlModifier;
#endif
	if (key == Qt::Key_Tab && effMods == insertTabMod)
	{
		bool repeat;
		m_table->activeCell().textFrame()->handleModeEditKey(event, repeat);
	}
	else
	// Tab / Shift+Tab: move to next/previous cell. Tab in the last cell
	// appends a new row, matching Word/Writer.
	if (key == Qt::Key_Tab && effMods == Qt::NoModifier)
	{
		const TableCell active = m_table->activeCell();
		// Tab advances towards column 0 when the table is RTL, so that is the
		// column the last cell of the table sits in.
		const bool atLastColumn = m_table->effectiveRTL()
				? (active.column() == 0)
				: (active.column() + active.columnSpan() - 1 == m_table->columns() - 1);
		const bool atLastCell =
				(active.row() + active.rowSpan() - 1 == m_table->rows() - 1) && atLastColumn;
		if (atLastCell)
		{
			m_table->appendRows(1);
			updateCanvas();
		}
		navigateCells(Qt::Key_Right);
	}
	else if (key == Qt::Key_Backtab) // Qt delivers Shift+Tab as Backtab.
	{
		navigateCells(Qt::Key_Left);
	}
	// Plain arrow: caret movement within cell, escaping to next cell at edges.
	// Shift+arrow: extend text selection within cell, escaping to cell-range
	// selection at edges.
	// Plain arrow or Shift+arrow handling.
	else if (isArrow && (effMods == Qt::NoModifier || effMods == Qt::ShiftModifier))
	{
		const bool extending = (effMods == Qt::ShiftModifier);

		if (m_table->hasSelection())
		{
			// Already in cell-range selection mode.
			if (extending)
			{
				// Continue extending the cell range.
				extendCellSelection(key);
			}
			else
			{
				// Plain arrow collapses the cell selection and moves one cell.
				m_table->clearSelection();
				resetSelectionAnchor();
				navigateCells(key);
			}
		}
		else
		{
			// No cell selection: caret-level movement, escaping at edges.
			if (cursorAtCellBoundary(key))
			{
				if (extending)
					extendCellSelection(key);
				else
					navigateCells(key);
			}
			else
			{
				PageItem_TextFrame* cellFrame = m_table->activeCell().textFrame();
				const int lenBefore = cellFrame->itemText.length();
				bool repeat;
				cellFrame->handleModeEditKey(event, repeat);
				textChanged = (cellFrame->itemText.length() != lenBefore);
			}
		}
	}
	// Delete/Backspace with cell selection: clear content of selected cells.
	else if (m_table->hasSelection() && (key == Qt::Key_Delete || key == Qt::Key_Backspace) )
	{
		m_table->clearSelectedCellContent();
		textChanged = true;
	}
	// Everything else: pass to the active cell's text frame.
	else if (!m_table->hasSelection())
	{
		PageItem_TextFrame* cellFrame = m_table->activeCell().textFrame();
		const int lenBefore = cellFrame->itemText.length();
		bool repeat;
		cellFrame->handleModeEditKey(event, repeat);
		textChanged = (cellFrame->itemText.length() != lenBefore);
	}

	// Re-fit the active row to its content after a text-changing keystroke, so
	// the row height -- and every cell's text frame and cursor in that row --
	// tracks the text as it is typed. Skipped entirely for navigation keys, and
	// the full row scan is avoided unless the edited cell now needs more height
	// than the row provides (grow-only), to keep ordinary typing cheap.
	if (textChanged && m_table->activeCell().isValid())
	{
		TableCell active = m_table->activeCell();
		if (m_table->rowNeedsGrowthForCell(active.row(), active.column()))
		{
			// If this keystroke is coalescing into an insert-text undo state on
			// the active cell's frame, fold the row grow into that same state so
			// one Ctrl+Z reverses the text and the growth together and typing
			// keeps coalescing. Otherwise grow normally with its own entries.
			PageItem_TextFrame* cellFrame = active.textFrame();
			SimpleState* insertState = nullptr;
			if (UndoManager::undoEnabled())
			{
				SimpleState* ss = dynamic_cast<SimpleState*>(undoManager->getLastUndo());
				if (ss && ss->contains("INSERT_FRAMETEXT") && ss->undoObject() == cellFrame)
					insertState = ss;
			}

			if (insertState)
			{
				const double oldRowHeight = m_table->rowHeight(active.row());
				bool undoEnabled = undoManager->undoEnabled();
				undoManager->setUndoEnabled(false);
				const bool grew = m_table->adjustRowHeight(active.row(), true);
				if (grew)
					m_table->adjustFrameToTable();
				undoManager->setUndoEnabled(undoEnabled);

				if (grew)
				{
					if (!insertState->contains("CELL_ROW_GREW"))
					{
						insertState->set("CELL_ROW_GREW");
						insertState->set("CELL_ROW", active.row());
						insertState->set("CELL_ROW_OLD_HEIGHT", oldRowHeight);
						insertState->set("CELL_ROW_STRATEGY", 0); // MoveFollowing
					}
					insertState->set("CELL_ROW_NEW_HEIGHT", m_table->rowHeight(active.row()));
				}
			}
			else
			{
				if (m_table->adjustRowHeight(active.row(), true))
					m_table->adjustFrameToTable();
			}
		}
	}

	if (textChanged)
		m_doc->changedPagePreview();

	makeLongTextCursorBlink();
	updateCanvas(true);
}

void CanvasMode_EditTable::mouseMoveEvent(QMouseEvent* event)
{
	event->accept();

	const QPointF canvasPoint = m_canvas->globalToCanvas(event->globalPosition()).toQPointF();
	const TableCanvasControl canvasControl = hitTestTableCanvasControls(canvasPoint);

	if (event->buttons() & Qt::LeftButton)
	{
		// A click on +/- is an atomic structural operation, not the start of a
		// text/cell-selection drag.
		if (m_tableCanvasControlPressed)
			return;

		// Mouse is being dragged with left button pressed.
		handleMouseDrag(event);
		return;
	}

	setHoveredTableCanvasControl(canvasControl);

	// Direct-manipulation controls outside the table get first choice of cursor.
	QCursor cursor(Qt::ArrowCursor);
	switch (canvasControl.type)
	{
		case TableCanvasControl::RowResize:
			cursor = Qt::SizeVerCursor;
			break;
		case TableCanvasControl::ColumnResize:
			cursor = Qt::SizeHorCursor;
			break;
		case TableCanvasControl::InsertRow:
		case TableCanvasControl::InsertColumn:
		case TableCanvasControl::DeleteRows:
		case TableCanvasControl::DeleteColumns:
		case TableCanvasControl::MergeCells:
		case TableCanvasControl::UnmergeCells:
			cursor = Qt::PointingHandCursor;
			break;
		case TableCanvasControl::None:
			break;
	}

	if (canvasControl.type == TableCanvasControl::None)
	{
		const double threshold = m_doc->guidesPrefs().grabRadius / m_canvas->scale();
		const TableHandle handle = m_table->hitTest(canvasPoint, threshold);
		switch (handle.type())
		{
			case TableHandle::RowSelect:
				cursor = m_selectRowCursor;
				break;
			case TableHandle::RowResize:
				cursor = Qt::SizeVerCursor;
				break;
			case TableHandle::ColumnSelect:
				cursor = m_selectColumnCursor;
				break;
			case TableHandle::ColumnResize:
				cursor = Qt::SizeHorCursor;
				break;
			case TableHandle::TableResize:
				cursor = Qt::SizeFDiagCursor;
				break;
			case TableHandle::SelectAll:
				cursor = (handle.index() == 1) ? Qt::SizeBDiagCursor : Qt::SizeFDiagCursor;
				break;
			case TableHandle::CellSelect:
				cursor = Qt::IBeamCursor;
				break;
			case TableHandle::None:
				break;
			default:
				qWarning("Unknown hit target");
				break;
		}
	}

	m_view->setCursor(cursor);
}

void CanvasMode_EditTable::mousePressEvent(QMouseEvent* event)
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = dynamic_cast<SimpleState*>(undoManager->getLastUndo());
		if (ss)
			ss->set("ETEA", QString(""));
		else
		{
			TransactionState *ts = dynamic_cast<TransactionState*>(undoManager->getLastUndo());
			if (ts)
				ss = dynamic_cast<SimpleState*>(ts->last());
			if (ss)
				ss->set("ETEA", QString(""));
		}
	}

	PageItem_TextFrame* activeFrame;

	event->accept();
	const QPointF canvasPoint = m_canvas->globalToCanvas(event->globalPosition()).toQPointF();
	const QPoint globalPos = event->globalPosition().toPoint();

	if (event->button() == Qt::LeftButton)
	{
		const TableCanvasControl canvasControl = hitTestTableCanvasControls(canvasPoint);
		if (canvasControl.type != TableCanvasControl::None)
		{
			switch (canvasControl.type)
			{
				case TableCanvasControl::RowResize:
					m_rowResizeGesture->setup(m_table, canvasControl.index);
					m_view->startGesture(m_rowResizeGesture);
					return;
				case TableCanvasControl::ColumnResize:
					m_columnResizeGesture->setup(m_table, canvasControl.index);
					m_view->startGesture(m_columnResizeGesture);
					return;
				case TableCanvasControl::InsertRow:
					m_tableCanvasControlPressed = true;
					insertTableRowAtBoundary(canvasControl.index);
					return;
				case TableCanvasControl::InsertColumn:
					m_tableCanvasControlPressed = true;
					insertTableColumnAtBoundary(canvasControl.index);
					return;
				case TableCanvasControl::DeleteRows:
					m_tableCanvasControlPressed = true;
					m_doc->itemSelection_DeleteTableRows();
					m_dragSelectionMode = CellSelect::Cells;
					resetSelectionAnchor();
					setHoveredTableCanvasControl(TableCanvasControl());
					// Update rulers if cells changed size
					updateTextEditUIForActiveCell();
					updateCanvas(true);
					return;
				case TableCanvasControl::DeleteColumns:
					m_tableCanvasControlPressed = true;
					m_doc->itemSelection_DeleteTableColumns();
					m_dragSelectionMode = CellSelect::Cells;
					resetSelectionAnchor();
					setHoveredTableCanvasControl(TableCanvasControl());
					// Update rulers if cells changed size
					updateTextEditUIForActiveCell();
					updateCanvas(true);
					return;
				case TableCanvasControl::MergeCells:
				case TableCanvasControl::UnmergeCells:
					m_tableCanvasControlPressed = true;
					if (canvasControl.type == TableCanvasControl::MergeCells)
						m_doc->itemSelection_MergeTableCells();
					else
						m_doc->itemSelection_SplitTableCells();
					m_dragSelectionMode = CellSelect::Cells;
					resetSelectionAnchor();
					setHoveredTableCanvasControl(TableCanvasControl());
					// Update rulers if cells changed size
					updateTextEditUIForActiveCell();
					updateCanvas(true);
					return;
				case TableCanvasControl::None:
					break;
			}
		}

		const double threshold = m_doc->guidesPrefs().grabRadius / m_canvas->scale();
		const TableHandle handle = m_table->hitTest(canvasPoint, threshold);
		switch (handle.type())
		{
			case TableHandle::RowSelect:
				m_dragSelectionMode = CellSelect::Rows;
				m_table->clearSelection();
				// Deselect text in active frame.
				activeFrame = m_table->activeCell().textFrame();
				activeFrame->itemText.deselectAll();
				activeFrame->HasSel = false;
				// Select the row indicated by the handle.
				m_table->moveTo(m_table->cellAt(handle.index(), 0));
				m_table->selectRow(handle.index());
				m_view->slotSetCurs(globalPos.x(), globalPos.y());
				m_lastCursorPos = -1;
				updateCanvas(true);
				break;
			case TableHandle::RowResize:
				// Start row resize gesture.
				m_rowResizeGesture->setup(m_table, handle.index());
				m_view->startGesture(m_rowResizeGesture);
				break;
			case TableHandle::ColumnSelect:
				m_dragSelectionMode = CellSelect::Columns;
				m_table->clearSelection();
				activeFrame = m_table->activeCell().textFrame();
				activeFrame->itemText.deselectAll();
				activeFrame->HasSel = false;
				m_table->moveTo(m_table->cellAt(0, handle.index()));
				m_table->selectColumn(handle.index());
				m_view->slotSetCurs(globalPos.x(), globalPos.y());
				m_lastCursorPos = -1;
				updateCanvas(true);
				break;
			case TableHandle::ColumnResize:
				// Start column resize gesture.
				m_columnResizeGesture->setup(m_table, handle.index());
				m_view->startGesture(m_columnResizeGesture);
				break;
			case TableHandle::TableResize:
				// Start table resize gesture.
				m_tableResizeGesture->setup(m_table);
				m_view->startGesture(m_tableResizeGesture);
				break;
			case TableHandle::CellSelect:
				m_dragSelectionMode = CellSelect::Cells;
				// Move to the pressed cell and position the text cursor.
				resetSelectionAnchor();
				m_table->clearSelection();
				m_table->moveTo(m_table->cellAt(canvasPoint));
				m_view->slotSetCurs(globalPos.x(), globalPos.y());
				m_lastCursorPos = m_table->activeCell().textFrame()->itemText.cursorPosition();
				m_view->m_ScMW->setTBvals(m_table->activeCell().textFrame());
				m_view->horizRuler->setItem(m_table->activeCell().textFrame());
				m_view->horizRuler->update();
				makeLongTextCursorBlink();
				updateCanvas(true);
				break;
			case TableHandle::SelectAll:
				m_dragSelectionMode = CellSelect::Cells;
				m_table->clearSelection();
				// Deselect text in active frame.
				activeFrame = m_table->activeCell().textFrame();
				activeFrame->itemText.deselectAll();
				activeFrame->HasSel = false;
				// Select every cell in the table.
				m_table->selectCells(0, 0, m_table->rows() - 1, m_table->columns() - 1);
				m_view->slotSetCurs(globalPos.x(), globalPos.y());
				m_lastCursorPos = -1;
				updateCanvas(true);
				break;
			case TableHandle::None:
				// Deselect text in active frame.
				activeFrame = m_table->activeCell().textFrame();
				activeFrame->itemText.deselectAll();
				activeFrame->HasSel = false;
				// Deselect the table and go back to normal mode.
				m_view->deselectItems(true);
				m_view->requestMode(modeNormal);
				m_view->canvasMode()->mousePressEvent(event);
				break;
			default:
				qWarning("Unknown hit target");
				break;
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		// Show the table popup menu.
		const FPoint mousePointDoc(m_canvas->globalToCanvas(event->globalPosition()));
		createContextMenu(m_table, mousePointDoc.x(), mousePointDoc.y());
	}
}

void CanvasMode_EditTable::mouseReleaseEvent(QMouseEvent* event)
{
	m_lastCursorPos = -1;
	m_tableCanvasControlPressed = false;

	// Keep Cut/Copy in sync with the final selection. A cell-range selection
	// has no text-level cut/copy path yet, so disable there.
	if (m_table->hasSelection())
		m_view->m_ScMW->setCopyCutEnabled(false);
	else
	{
		PageItem_TextFrame* activeFrame = m_table->activeCell().textFrame();
		m_view->m_ScMW->setCopyCutEnabled(activeFrame && activeFrame->HasSel);
	}
}

void CanvasMode_EditTable::mouseDoubleClickEvent(QMouseEvent* event)
{
	TableCell hitCell = m_table->cellAt(m_canvas->globalToCanvas(event->globalPosition()).toQPointF());
	if (!hitCell.isValid())
		return;

	event->accept();

	PageItem_TextFrame* textFrame = hitCell.textFrame();
	if (event->modifiers() & Qt::ControlModifier)
	{
		int start = 0, end = 0;

		if (event->modifiers() & Qt::ShiftModifier)
		{
			// Double click with Ctrl+Shift to select multiple paragraphs.
			uint oldParNr = textFrame->itemText.nrOfParagraph(m_lastCursorPos);
			uint newParNr = textFrame->itemText.nrOfParagraph();

			start = textFrame->itemText.startOfParagraph(qMin(oldParNr, newParNr));
			end = textFrame->itemText.endOfParagraph(qMax(oldParNr, newParNr));
		}
		else
		{
			// Double click with Ctrl to select a single paragraph.
			m_lastCursorPos = textFrame->itemText.cursorPosition();
			uint parNr = textFrame->itemText.nrOfParagraph(m_lastCursorPos);
			start = textFrame->itemText.startOfParagraph(parNr);
			end = textFrame->itemText.endOfParagraph(parNr);
		}
		textFrame->itemText.deselectAll();
		textFrame->itemText.extendSelection(start, end);
		textFrame->itemText.setCursorPosition(end);
	}
	else
	{
		// Regular double click selects a word.
		m_lastCursorPos = textFrame->itemText.cursorPosition();
		textFrame->itemText.selectWord(textFrame->itemText.cursorPosition());
	}

	// The selection lives in itemText; HasSel must mirror it or
	// slotEditCut/slotEditCopy's `if (cItem->HasSel)` guard drops it.
	// Also sync Cut/Copy, matching CanvasMode_Edit's double-click.
	textFrame->HasSel = textFrame->itemText.hasSelection();
	m_view->m_ScMW->setCopyCutEnabled(textFrame->HasSel);

	updateCanvas(true);
}

void CanvasMode_EditTable::drawControls(QPainter* p)
{
	p->save();
	commonDrawControls(p, false);
	if (!m_table->hasSelection())
		drawTextCursor(p);
	p->restore();

	drawTableHandleHints(p);

	if (m_table->hasSelection())
		paintCellSelection(p);

	drawTableCanvasControls(p);
}

void CanvasMode_EditTable::drawTableHandleHints(QPainter* p)
{
	if (!m_table || !m_canvas || !p)
		return;

	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	p->setTransform(m_table->getTransform(), true);
	p->setRenderHint(QPainter::Antialiasing);

	const double threshold = m_doc->guidesPrefs().grabRadius / m_canvas->scale();
	const QPointF offset = m_table->gridOffset();
	const double tableW = m_table->tableWidth();
	const double tableH = m_table->tableHeight();
	const double tickLength = qMax(threshold * 2.0, 8.0 / m_canvas->scale());
	const double tickWidth = qMax(2.0 / m_canvas->scale(), 1.0);

	QColor markerColor(50, 150, 220, 220);
	p->setPen(QPen(markerColor, tickWidth, Qt::SolidLine, Qt::FlatCap));

	// Tick marks along the top edge (one per column boundary, including outer).
	for (int c = 0; c <= m_table->columns(); ++c)
	{
		double colX = (c < m_table->columns()) ? m_table->columnPosition(c) : m_table->columnPosition(c - 1) + m_table->columnWidth(c - 1);
		p->drawLine(QPointF(offset.x() + colX, offset.y() - tickLength), QPointF(offset.x() + colX, offset.y()));
	}

	// Tick marks along the left edge (one per row boundary, including outer).
	for (int r = 0; r <= m_table->rows(); ++r)
	{
		double rowY = (r < m_table->rows()) ? m_table->rowPosition(r) : m_table->rowPosition(r - 1) + m_table->rowHeight(r - 1);
		p->drawLine(QPointF(offset.x() - tickLength, offset.y() + rowY), QPointF(offset.x(), offset.y() + rowY));
	}

	// Tick marks along the bottom edge.
	for (int c = 0; c <= m_table->columns(); ++c)
	{
		double colX = (c < m_table->columns()) ? m_table->columnPosition(c) : m_table->columnPosition(c - 1) + m_table->columnWidth(c - 1);
		p->drawLine(QPointF(offset.x() + colX, offset.y() + tableH), QPointF(offset.x() + colX, offset.y() + tableH + tickLength));
	}

	// Tick marks along the right edge.
	for (int r = 0; r <= m_table->rows(); ++r)
	{
		double rowY = (r < m_table->rows()) ? m_table->rowPosition(r) : m_table->rowPosition(r - 1) + m_table->rowHeight(r - 1);
		p->drawLine(QPointF(offset.x() + tableW, offset.y() + rowY), QPointF(offset.x() + tableW + tickLength, offset.y() + rowY));
	}

	// Select-all corner marker. LTR: top-left corner; RTL: top-right corner.
	// Matches the corner hit-tested in PageItem_Table::hitTest for SelectAll.
	const bool rtl = m_table->effectiveRTL();
	const double cornerSize = qMax(threshold * 2.0, 10.0 / m_canvas->scale());
	const double cornerX = rtl ? (offset.x() + tableW - cornerSize) : offset.x();
	const double cornerY = offset.y();
	QRectF cornerRect(cornerX, cornerY, cornerSize, cornerSize);
	QColor cornerFill(50, 150, 220, 90);
	p->fillRect(cornerRect, cornerFill);
	p->setPen(QPen(markerColor, tickWidth, Qt::SolidLine, Qt::FlatCap));
	p->drawRect(cornerRect);

	p->restore();
}

CellSelect::SelectionMode CanvasMode_EditTable::structuralSelectionMode() const
{
	if (!m_table || !m_table->hasSelection())
		return CellSelect::Cells;

	const QSet<int> rows = m_table->selectedRows();
	const QSet<int> columns = m_table->selectedColumns();
	if (rows.isEmpty() || columns.isEmpty())
		return CellSelect::Cells;

	// Base the affordance on what the selection covers, not how it was made.
	// This lets an ordinary cell drag across a complete row/column expose the
	// same structural delete control as the dedicated row/column select strip.
	if (columns.size() == m_table->columns() && rows.size() < m_table->rows())
		return CellSelect::Rows;
	if (rows.size() == m_table->rows() && columns.size() < m_table->columns())
		return CellSelect::Columns;

	return CellSelect::Cells;
}

CanvasMode_EditTable::TableCanvasControl::Type CanvasMode_EditTable::deleteControl(double controlScale, QPointF& center) const
{
	const QPointF offset = m_table->gridOffset();
	const CellSelect::SelectionMode selectionMode = structuralSelectionMode();
	if (selectionMode == CellSelect::Rows)
	{
		// Right of the selected rows
		int first = m_table->rows();
		int last = -1;
		for (int row : m_table->selectedRows())
		{
			first = qMin(first, row);
			last = qMax(last, row);
		}
		const double top = m_table->rowPosition(first);
		const double bottom = m_table->rowPosition(last) + m_table->rowHeight(last);
		center = QPointF(offset.x() + m_table->tableWidth() + DeleteControlOffsetPx * controlScale, offset.y() + (top + bottom) / 2.0);
		return TableCanvasControl::DeleteRows;
	}
	if (selectionMode == CellSelect::Columns)
	{
		// Below the selected columns
		int first = m_table->columns();
		int last = -1;
		for (int column : m_table->selectedColumns())
		{
			first = qMin(first, column);
			last = qMax(last, column);
		}
		const double left = m_table->columnPosition(first);
		const double right = m_table->columnPosition(last) + m_table->columnWidth(last);
		center = QPointF(offset.x() + (left + right) / 2.0, offset.y() + m_table->tableHeight() + DeleteControlOffsetPx * controlScale);
		return TableCanvasControl::DeleteColumns;
	}
	return TableCanvasControl::None;
}

CanvasMode_EditTable::TableCanvasControl::Type CanvasMode_EditTable::cellActionControl(QPointF& center, double& buttonSize, double& hitSize) const
{
	const QSet<TableCell>& selectedCells = m_table->selectedCells();
	if (selectedCells.isEmpty())
		return TableCanvasControl::None;

	// The button sits in the top-right corner of the selected area, sized to fit
	// the top-right cell
	TableCanvasControl::Type type = TableCanvasControl::MergeCells;
	QRectF area;
	QRectF anchorRect;
	if (selectedCells.size() == 1)
	{
		// One merged cell selected. Splitting works on the active cell, so
		// only offer it when that is the selected cell.
		const TableCell cell = *selectedCells.constBegin();
		if ((cell.rowSpan() == 1 && cell.columnSpan() == 1) || cell != m_table->activeCell())
			return TableCanvasControl::None;
		type = TableCanvasControl::UnmergeCells;
		area = cell.boundingRect();
		anchorRect = area;
	}
	else
	{
		int anchorRow = -1;
		int anchorEndColumn = -1;
		for (const TableCell& cell : selectedCells)
		{
			const QRectF cellRect = cell.boundingRect();
			area |= cellRect;
			const int endColumn = cell.column() + cell.columnSpan();
			if (anchorRow < 0 || cell.row() < anchorRow || (cell.row() == anchorRow && endColumn > anchorEndColumn))
			{
				anchorRect = cellRect;
				anchorRow = cell.row();
				anchorEndColumn = endColumn;
			}
		}
	}

	const double controlScale = 1.0 / m_canvas->scale();
	const double anchorMinSize = qMin(anchorRect.width(), anchorRect.height());
	buttonSize = qBound(CellActionMinButtonPx * controlScale, anchorMinSize / 2.0, CellActionButtonPx * controlScale);
	hitSize = qMax(buttonSize, qMin(CellActionHitPx * controlScale, anchorMinSize * 0.75));
	const double inset = buttonSize / 2.0 + CellActionMarginPx * controlScale;
	const QPointF offset = m_table->gridOffset();
	center = QPointF(offset.x() + area.right() - inset, offset.y() + area.top() + inset);
	return type;
}

CanvasMode_EditTable::TableCanvasControl CanvasMode_EditTable::hitTestTableCanvasControls(const QPointF& canvasPoint) const
{
	TableCanvasControl control;
	if (!m_table || !m_canvas)
		return control;

	// Controls keep their size on screen: one pixel in document units
	const double controlScale = 1.0 / m_canvas->scale();
	const double hitHalf = ControlHitDiameterPx * controlScale / 2.0;
	const double gripHalfLong = (ResizeGripLongPx / 2.0 + 2.0) * controlScale;
	const double gripHitDepth = 12.0 * controlScale;
	const QPointF framePoint = m_table->getTransform().inverted().map(canvasPoint);
	const QPointF offset = m_table->gridOffset();
	const double tableW = m_table->tableWidth();
	const double tableH = m_table->tableHeight();

	// Merge or unmerge, inside the top-right corner of the selected cells
	QPointF cellActionCenter;
	double cellActionButton = 0.0;
	double cellActionHit = 0.0;
	const TableCanvasControl::Type cellActionType = cellActionControl(cellActionCenter, cellActionButton, cellActionHit);
	if ((cellActionType != TableCanvasControl::None)
		&& QRectF(cellActionCenter.x() - cellActionHit / 2.0, cellActionCenter.y() - cellActionHit / 2.0, cellActionHit, cellActionHit).contains(framePoint))
	{
		control.type = cellActionType;
		return control;
	}

	// Contextual delete controls are visible whenever the selected cells cover
	// complete rows/columns, regardless of which selection gesture produced them.
	QPointF deleteCenter;
	const TableCanvasControl::Type deleteType = deleteControl(controlScale, deleteCenter);
	if ((deleteType != TableCanvasControl::None)
		&& QRectF(deleteCenter.x() - hitHalf, deleteCenter.y() - hitHalf, hitHalf * 2.0, hitHalf * 2.0).contains(framePoint))
	{
		control.type = deleteType;
		return control;
	}

	// Column boundaries: boundary N inserts at index N, and the boundary after
	// column N resizes column N. Boundary zero, the left edge, has no grip.
	for (int boundary = 0; boundary <= m_table->columns(); ++boundary)
	{
		const double x = (boundary < m_table->columns())
			? m_table->columnPosition(boundary)
			: tableW;
		const QPointF plusCenter(offset.x() + x,
							 offset.y() - InsertControlOffsetPx * controlScale);
		if (QRectF(plusCenter.x() - hitHalf, plusCenter.y() - hitHalf,
				   hitHalf * 2.0, hitHalf * 2.0).contains(framePoint))
		{
			control.type = TableCanvasControl::InsertColumn;
			control.index = boundary;
			return control;
		}

		if (boundary == 0)
			continue;
		const QPointF gripCenter(offset.x() + x,
							 offset.y() - ResizeGripOffsetPx * controlScale);
		if (QRectF(gripCenter.x() - gripHalfLong, offset.y() - gripHitDepth,
				   gripHalfLong * 2.0, gripHitDepth).contains(framePoint))
		{
			control.type = TableCanvasControl::ColumnResize;
			control.index = boundary - 1;
			return control;
		}
	}

	// Row boundaries use the same scheme on the left side of the table.
	for (int boundary = 0; boundary <= m_table->rows(); ++boundary)
	{
		const double y = (boundary < m_table->rows())
			? m_table->rowPosition(boundary)
			: tableH;
		const QPointF plusCenter(offset.x() - InsertControlOffsetPx * controlScale,
							 offset.y() + y);
		if (QRectF(plusCenter.x() - hitHalf, plusCenter.y() - hitHalf,
				   hitHalf * 2.0, hitHalf * 2.0).contains(framePoint))
		{
			control.type = TableCanvasControl::InsertRow;
			control.index = boundary;
			return control;
		}

		if (boundary == 0)
			continue;
		const QPointF gripCenter(offset.x() - ResizeGripOffsetPx * controlScale,
							 offset.y() + y);
		if (QRectF(offset.x() - gripHitDepth, gripCenter.y() - gripHalfLong,
				   gripHitDepth, gripHalfLong * 2.0).contains(framePoint))
		{
			control.type = TableCanvasControl::RowResize;
			control.index = boundary - 1;
			return control;
		}
	}

	return control;
}

void CanvasMode_EditTable::drawTableCanvasControls(QPainter* p)
{
	if (!m_table || !m_canvas || !p)
		return;

	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	p->setTransform(m_table->getTransform(), true);
	p->setRenderHint(QPainter::Antialiasing);

	// Controls keep their size on screen: one pixel in document units
	const double controlScale = 1.0 / m_canvas->scale();
	const QPointF offset = m_table->gridOffset();
	const double tableW = m_table->tableWidth();
	const double tableH = m_table->tableHeight();

	// Hovering either part of a boundary control reveals both the drag grip
	// and the one-click insertion control for that boundary.
	if (m_hoverTableCanvasControl.type == TableCanvasControl::ColumnResize ||
		m_hoverTableCanvasControl.type == TableCanvasControl::InsertColumn)
	{
		const int boundary = (m_hoverTableCanvasControl.type == TableCanvasControl::ColumnResize)
			? m_hoverTableCanvasControl.index + 1 : m_hoverTableCanvasControl.index;
		if (boundary >= 0 && boundary <= m_table->columns())
		{
			const double x = (boundary < m_table->columns()) ? m_table->columnPosition(boundary) : tableW;
			if (boundary > 0)
				drawResizeGrip(p, QPointF(offset.x() + x, offset.y() - ResizeGripOffsetPx * controlScale), Qt::Horizontal);
			drawControlButton(p, QPointF(offset.x() + x, offset.y() - InsertControlOffsetPx * controlScale), true);
		}
	}
	else if (m_hoverTableCanvasControl.type == TableCanvasControl::RowResize ||
			 m_hoverTableCanvasControl.type == TableCanvasControl::InsertRow)
	{
		const int boundary = (m_hoverTableCanvasControl.type == TableCanvasControl::RowResize)
			? m_hoverTableCanvasControl.index + 1 : m_hoverTableCanvasControl.index;
		if (boundary >= 0 && boundary <= m_table->rows())
		{
			const double y = (boundary < m_table->rows()) ? m_table->rowPosition(boundary) : tableH;
			if (boundary > 0)
				drawResizeGrip(p, QPointF(offset.x() - ResizeGripOffsetPx * controlScale, offset.y() + y), Qt::Vertical);
			drawControlButton(p, QPointF(offset.x() - InsertControlOffsetPx * controlScale, offset.y() + y), true);
		}
	}

	// Merge or unmerge for the selected cells
	QPointF cellActionCenter;
	double cellActionButton = 0.0;
	double cellActionHit = 0.0;
	const TableCanvasControl::Type cellActionType = cellActionControl(cellActionCenter, cellActionButton, cellActionHit);
	if (cellActionType != TableCanvasControl::None)
		drawCellActionButton(p, cellActionCenter, cellActionType, cellActionButton);

	// A minus control is persistent while the selected cells cover complete
	// rows/columns. It disappears when deleting the selection would remove the
	// entire table dimension.
	QPointF deleteCenter;
	if (deleteControl(controlScale, deleteCenter) != TableCanvasControl::None)
		drawControlButton(p, deleteCenter, false);

	p->restore();
}

void CanvasMode_EditTable::drawControlButton(QPainter* p, const QPointF& center, bool plus) const
{
	const double controlScale = 1.0 / m_canvas->scale();
	const double radius = ControlDiameterPx * controlScale / 2.0;
	p->setPen(Qt::NoPen);
	p->setBrush(ControlColor);
	p->drawEllipse(center, radius, radius);

	p->setPen(QPen(ControlGlyphColor, 1.5 * controlScale, Qt::SolidLine, Qt::RoundCap));
	const double arm = 3.5 * controlScale;
	p->drawLine(QPointF(center.x() - arm, center.y()), QPointF(center.x() + arm, center.y()));
	if (plus)
		p->drawLine(QPointF(center.x(), center.y() - arm), QPointF(center.x(), center.y() + arm));
}

void CanvasMode_EditTable::drawResizeGrip(QPainter* p, const QPointF& center, Qt::Orientation orientation) const
{
	const double controlScale = 1.0 / m_canvas->scale();
	const bool horizontal = (orientation == Qt::Horizontal);
	const double width = (horizontal ? ResizeGripLongPx : ResizeGripShortPx) * controlScale;
	const double height = (horizontal ? ResizeGripShortPx : ResizeGripLongPx) * controlScale;
	p->setPen(Qt::NoPen);
	p->setBrush(ControlColor);
	p->drawRoundedRect(QRectF(center.x() - width / 2.0, center.y() - height / 2.0, width, height), 3.0 * controlScale, 3.0 * controlScale);

	p->setBrush(ControlGlyphColor);
	const double dotRadius = controlScale;
	const double step = 3.5 * controlScale;
	for (int i = -1; i <= 1; ++i)
	{
		const QPointF dot = horizontal ? QPointF(center.x() + i * step, center.y()) : QPointF(center.x(), center.y() + i * step);
		p->drawEllipse(dot, dotRadius, dotRadius);
	}
}

void CanvasMode_EditTable::drawCellActionButton(QPainter* p, const QPointF& center, TableCanvasControl::Type type, double buttonSize) const
{
	const double controlScale = 1.0 / m_canvas->scale();
	const QRectF buttonRect(center.x() - buttonSize / 2.0, center.y() - buttonSize / 2.0, buttonSize, buttonSize);
	const double cornerRadius = qMin(4.0 * controlScale, buttonSize / 3.0);
	p->setPen(Qt::NoPen);
	p->setBrush(ControlColor);
	p->drawRoundedRect(buttonRect, cornerRadius, cornerRadius);

	const QPixmap& icon = (type == TableCanvasControl::MergeCells) ? m_mergeIcon : m_unmergeIcon;
	if (icon.isNull())
		return;
	const double iconSize = buttonSize * CellActionIconPx / CellActionButtonPx;
	p->drawPixmap(QRectF(center.x() - iconSize / 2.0, center.y() - iconSize / 2.0, iconSize, iconSize), icon, QRectF(icon.rect()));
}

QPixmap CanvasMode_EditTable::controlGlyph(const QString& name) const
{
	// Loaded at the screen's pixel density, so it stays sharp on HiDPI screens
	const int size = qRound(CellActionIconPx * m_view->devicePixelRatioF());
	QPixmap icon = IconManager::instance().loadPixmap(name, size);
	if (icon.isNull())
		return icon;

	// Painting detaches the pixmap, so the IconManager cache is left untouched
	QPainter painter(&icon);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(icon.rect(), ControlGlyphColor);
	painter.end();
	return icon;
}

void CanvasMode_EditTable::setHoveredTableCanvasControl(const TableCanvasControl& control)
{
	if (control == m_hoverTableCanvasControl)
		return;

	m_hoverTableCanvasControl = control;
	updateCanvas();
}

void CanvasMode_EditTable::insertTableRowAtBoundary(int boundary)
{
	if (!m_table || boundary < 0 || boundary > m_table->rows())
		return;

	m_doc->insertTableRows(m_table, boundary, 1);

	m_dragSelectionMode = CellSelect::Cells;
	resetSelectionAnchor();
	m_hoverTableCanvasControl = TableCanvasControl();
	// The active cell may have moved or changed size
	updateTextEditUIForActiveCell();
	updateCanvas(true);
}

void CanvasMode_EditTable::insertTableColumnAtBoundary(int boundary)
{
	if (!m_table || boundary < 0 || boundary > m_table->columns())
		return;

	m_doc->insertTableColumns(m_table, boundary, 1);

	m_dragSelectionMode = CellSelect::Cells;
	resetSelectionAnchor();
	m_hoverTableCanvasControl = TableCanvasControl();
	// The active cell may have moved or changed size
	updateTextEditUIForActiveCell();
	updateCanvas(true);
}

void CanvasMode_EditTable::paintCellSelection(QPainter* p)
{
	if (!m_table || !m_canvas || !p)
			return;

	p->save();
	p->scale(m_canvas->scale(), m_canvas->scale());
	p->translate(-m_doc->minCanvasCoordinate.x(), -m_doc->minCanvasCoordinate.y());
	p->setTransform(m_table->getTransform(), true);
	p->setRenderHint(QPainter::Antialiasing);
	p->setPen(QPen(QColor(100, 200, 255), 3.0 / m_canvas->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p->setBrush(QColor(100, 200, 255, 50));

	/*
	* The code below makes selectionPath a union of the cell rectangles of the selected cells.
	* Since the cell rectangles are adjacent, they must be expanded slightly (1.0) for the
	* uniting to work. This may not be the fastest way to compose the path of the selection,
	* but it makes for some very simple code. And the result looks good.
	*/

	const QPointF offset = m_table->gridOffset();
	QPainterPath selectionPath;

	for (const TableCell& cell : m_table->selectedCells())
	{
		QRectF cellRect = cell.boundingRect();
		cellRect.translate(offset);
		cellRect.adjust(-1.0, -1.0, 1.0, 1.0);
		QPainterPath cellPath;
		cellPath.addRect(cellRect);
		selectionPath = selectionPath.united(cellPath);
	}

	p->drawPath(selectionPath);
	p->restore();
}

void CanvasMode_EditTable::updateCanvas(bool forceRedraw)
{
	// Do not let update timer cancel forced redraw otherwise
	// we get refresh issues  when typing or selecting text
	if (!m_canvas->isForcedRedraw())
		m_canvas->setForcedRedraw(forceRedraw);
	// Enlarge the update region to include the controls painted
	// outside the table as well as the cell-selection visual.
	QRectF tableRect = m_table->getBoundingRect();
	const double controlExtent = CanvasControlExtentPx / m_canvas->scale();
	tableRect.adjust(-controlExtent, -controlExtent, controlExtent, controlExtent);
	m_canvas->update(m_canvas->canvasToLocal(tableRect));
}

void CanvasMode_EditTable::handleMouseDrag(QMouseEvent* event)
{
	TableCell hitCell = m_table->cellAt(m_canvas->globalToCanvas(event->globalPosition()).toQPointF());
	TableCell activeCell = m_table->activeCell();
	PageItem_TextFrame* activeFrame = activeCell.textFrame();

	bool rangeSelecting = false;

	if ((!hitCell.isValid() || hitCell == activeCell) && m_lastCursorPos != -1)
	{
		// Drag within the active cell: select text in the cell's text frame.
		activeFrame->itemText.deselectAll();
		activeFrame->HasSel = false;

		QPoint globalPos = event->globalPosition().toPoint();
		m_view->slotSetCurs(globalPos.x(), globalPos.y());

		PageItem_TextFrame* newActiveFrame = m_table->activeCell().textFrame();
		if (activeFrame == newActiveFrame)
		{
			const int selectionStart = qMin(activeFrame->itemText.cursorPosition(), m_lastCursorPos);
			const int selectionLength = qAbs(activeFrame->itemText.cursorPosition() - m_lastCursorPos);

			activeFrame->itemText.select(selectionStart, selectionLength);
			activeFrame->HasSel = (selectionLength > 0);
			// Gate Edit > Cut/Copy on the cell's text selection. Their shortcuts
			// won't fire otherwise, so Ctrl+X/Ctrl+C are no-ops after a mouse
			// selection in a cell. The Shift+Arrow path already does this via
			// PageItem_TextFrame::ExpandSel().
			m_view->m_ScMW->setCopyCutEnabled(activeFrame->HasSel);
		}
		else
		{
			// Active cell changed mid-drag: start cell range selection.
			m_lastCursorPos = -1;
			m_cellSelectGesture->setup(m_table, activeCell, m_dragSelectionMode);
			m_view->startGesture(m_cellSelectGesture);
			rangeSelecting = true;
		}
	}
	else
	{
		// Drag moved into another cell: start cell range selection.
		activeFrame->itemText.deselectAll();
		activeFrame->HasSel = false;

		m_cellSelectGesture->setup(m_table, activeCell, m_dragSelectionMode);
		m_view->startGesture(m_cellSelectGesture);
		rangeSelecting = true;
	}

	// The ruler shows tab/indent markers only while editing within a single
	// cell. Clear it when a cell range selection is starting; otherwise keep
	// it on the active cell.
	if (rangeSelecting)
	{
		m_view->horizRuler->textMode(false);
		m_view->horizRuler->update();
	}
	else
	{
		updateTextEditUIForActiveCell();
	}

	updateCanvas(true);
}

void CanvasMode_EditTable::drawTextCursor(QPainter* p)
{
	if ((!m_longBlink && m_blinkTime.elapsed() > QApplication::cursorFlashTime() / 2)
			|| (m_longBlink && m_blinkTime.elapsed() > QApplication::cursorFlashTime()))
	{
		// Reset blink timer
		m_blinkTime.restart();
		m_longBlink = false;
		m_cursorVisible = !m_cursorVisible;
	}

	if (m_cursorVisible)
	{
		// Paint text cursor.
		p->save();
		p->setTransform(m_table->getTransform(), true);
		PageItem_TextFrame* cellFrame = m_table->activeCell().textFrame();
		commonDrawTextCursor(p, cellFrame, m_table->gridOffset(), cellFrame->height());
		p->restore();
	}
}

void CanvasMode_EditTable::makeLongTextCursorBlink()
{
	m_cursorVisible = true;
	m_longBlink = true;
	m_blinkTime.restart();
}

void CanvasMode_EditTable::createContextMenu(PageItem *currItem, double mx, double my)
{
	ContextMenu* cmen = nullptr;
	m_view->setCursor(QCursor(Qt::ArrowCursor));
	m_view->setObjectUndoMode();
	if (currItem != nullptr)
		cmen = new ContextMenu(*(m_doc->m_Selection), m_ScMW, m_doc);
	else
		cmen = new ContextMenu(m_ScMW, m_doc, mx, my);
	if (cmen)
		cmen->exec(QCursor::pos());
	m_view->setGlobalUndoMode();
	delete cmen;
}

void CanvasMode_EditTable::resetSelectionAnchor()
{
	m_selectionAnchorRow = -1;
	m_selectionAnchorColumn = -1;
}

int CanvasMode_EditTable::logicalKey(int key) const
{
	if (!m_table->effectiveRTL())
		return key;
	if (key == Qt::Key_Left)
		return Qt::Key_Right;
	if (key == Qt::Key_Right)
		return Qt::Key_Left;
	return key;
}

bool CanvasMode_EditTable::moveActiveCell(int key)
{
	// The key gives the direction on screen; logicalKey() turns it into the
	// column to move to, which is mirrored when the table is RTL.
	switch (logicalKey(key))
	{
		case Qt::Key_Left:
			m_table->moveLeft();
			return true;
		case Qt::Key_Right:
			m_table->moveRight();
			return true;
		case Qt::Key_Up:
			m_table->moveUp();
			return true;
		case Qt::Key_Down:
			m_table->moveDown();
			return true;
	}
	return false;
}

void CanvasMode_EditTable::navigateCells(int key)
{
	const int beforeRow = m_table->activeCell().row();
	const int beforeCol = m_table->activeCell().column();

	if (!moveActiveCell(key))
		return;

	tryRowWrap(key, beforeRow, beforeCol);

	resetSelectionAnchor();
	m_table->clearSelection();

	PageItem_TextFrame* tf = m_table->activeCell().textFrame();
	tf->itemText.deselectAll();
	tf->HasSel = false;
	tf->itemText.setCursorPosition((key == Qt::Key_Left || key == Qt::Key_Up) ? tf->itemText.length() : 0);

	updateTextEditUIForActiveCell();
}

void CanvasMode_EditTable::extendCellSelection(int key)
{
	if (!m_table->hasSelection())
	{
		m_selectionAnchorRow    = m_table->activeCell().row();
		m_selectionAnchorColumn = m_table->activeCell().column();
	}
	else if (m_selectionAnchorRow < 0)
	{
		// Selection exists (e.g. from mouse drag) but no keyboard anchor.
		// Use the corner of the bounding box opposite the active cell.
		int activeRow = m_table->activeCell().row();
		int activeCol = m_table->activeCell().column();
		int topRow = INT_MAX, bottomRow = INT_MIN;
		int leftCol = INT_MAX, rightCol = INT_MIN;
		for (const TableCell& cell : m_table->selectedCells())
		{
			topRow    = qMin(topRow,    cell.row());
			bottomRow = qMax(bottomRow, cell.row()    + cell.rowSpan()    - 1);
			leftCol   = qMin(leftCol,   cell.column());
			rightCol  = qMax(rightCol,  cell.column() + cell.columnSpan() - 1);
		}
		m_selectionAnchorRow    = (activeRow == bottomRow) ? topRow  : bottomRow;
		m_selectionAnchorColumn = (activeCol == rightCol)  ? leftCol : rightCol;
	}

	if (!moveActiveCell(key))
		return;

	// No row-wrap during selection extension: Word stops at row edges with
	// Shift+arrow, and a rectangular cell selection can't represent the
	// L-shaped region that row-wrap would imply.

	PageItem_TextFrame* tf = m_table->activeCell().textFrame();
	tf->itemText.deselectAll();
	tf->HasSel = false;

	m_table->clearSelection();
	m_table->selectCells(m_selectionAnchorRow, m_selectionAnchorColumn, m_table->activeCell().row(), m_table->activeCell().column());
	m_view->horizRuler->textMode(false);
	m_view->horizRuler->update();
}

bool CanvasMode_EditTable::cursorAtCellBoundary(int key) const
{
	PageItem_TextFrame* tf = m_table->activeCell().textFrame();
	const int len = tf->itemText.length();

	if (len == 0)
		return true;

	const int pos = tf->itemText.cursorPosition();
	bool result = false;

	switch (key)
	{
		case Qt::Key_Left:
			result = (pos <= 0);
			break;
		case Qt::Key_Right:
			result = (pos >= len);
			break;
		case Qt::Key_Up:
			result = tf->cursorOnFirstLine();
			break;
		case Qt::Key_Down:
			result = tf->cursorOnLastLine();
			break;
	}

	return result;
}

bool CanvasMode_EditTable::tryRowWrap(int key, int beforeRow, int beforeCol)
{
	// Returns true if the active cell didn't change after moveActiveCell()
	// and we successfully wrapped to a different row. Vertical motion does
	// not wrap.
	if (m_table->activeCell().row() != beforeRow ||
		m_table->activeCell().column() != beforeCol)
		return false;

	// The key here is the traversal direction, not a column direction:
	// Key_Right always means forward (Tab, and the arrow that moves forward in
	// reading order) and Key_Left always means back, in both LTR and RTL. It is
	// only the columns a row starts and ends at that mirror when RTL, so moving
	// forward off the end of a row always continues at the start of the next.
	const bool rtl = m_table->effectiveRTL();
	const int rowStartCol = rtl ? m_table->columns() - 1 : 0;
	const int rowEndCol   = rtl ? 0 : m_table->columns() - 1;

	if (key == Qt::Key_Right && beforeRow + 1 < m_table->rows())
	{
		m_table->moveTo(m_table->cellAt(beforeRow + 1, rowStartCol));
		return true;
	}
	if (key == Qt::Key_Left && beforeRow > 0)
	{
		m_table->moveTo(m_table->cellAt(beforeRow - 1, rowEndCol));
		return true;
	}
	return false;
}

void CanvasMode_EditTable::updateTextEditUIForActiveCell()
{
	PageItem_TextFrame* frame = m_table->activeCell().textFrame();
	if (!frame)
		return;

	m_view->m_ScMW->setTBvals(frame);
	m_view->horizRuler->setItem(frame);
	m_view->horizRuler->update();
}
