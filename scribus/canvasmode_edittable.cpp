/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "canvasmode_edittable.h"

#include <QCursor>
#include <QDebug>
#include <QPainter>
#include <QPointF>
#include <QTimer>

#include "appmodes.h"
#include "canvas.h"
#include "canvasgesture_cellselect.h"
#include "canvasgesture_columnresize.h"
#include "canvasgesture_rowresize.h"
#include "canvasgesture_tableresize.h"
#include "cellarea.h"
#include "ui/contextmenu.h"
#include "fpoint.h"
#include "pageitem_table.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "tablehandle.h"
#include "ui/scmwmenumanager.h"
#include "iconmanager.h"

// TODO: We should have a preference for this instead.
#ifdef Q_OS_MAC
	static const Qt::KeyboardModifiers CellNavigationModifiers = Qt::AltModifier | Qt::KeypadModifier;
#else
	static const Qt::KeyboardModifiers CellNavigationModifiers = Qt::AltModifier;
#endif

CanvasMode_EditTable::CanvasMode_EditTable(ScribusView* view) : CanvasMode(view),
	m_table(0),
	m_canvasUpdateTimer(new QTimer(view)),
	m_longBlink(false),
	m_cursorVisible(false),
	m_lastCursorPos(-1),
	m_selectRowCursor(IconManager::instance()->loadCursor("select_row.png")),
	m_selectColumnCursor(IconManager::instance()->loadCursor("select_column.png")),
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
	PageItem *item = m_doc->m_Selection->itemAt(0);
	Q_ASSERT(item && item->isTable());
	m_table = item->asTable();

	if (fromGesture)
		m_view->setCursor(Qt::ArrowCursor);

	m_lastCursorPos = -1;
	m_blinkTime.start();
	m_canvasUpdateTimer->start(200);
	makeLongTextCursorBlink();

	m_view->m_ScMW->updateTableMenuActions();
}

void CanvasMode_EditTable::deactivate(bool forGesture)
{
	if (!forGesture)
		m_canvasUpdateTimer->stop();

	m_view->m_ScMW->updateTableMenuActions();
}

void CanvasMode_EditTable::keyPressEvent(QKeyEvent* event)
{
	event->accept();

	// Handle some keys specific to this mode.
	if (event->key() == Qt::Key_Escape)
	{
		// Deselect text in active frame.
		PageItem_TextFrame* activeFrame = m_table->activeCell().textFrame();
		activeFrame->itemText.deselectAll();
		activeFrame->HasSel = false;
		// Go back to normal mode.
		m_view->requestMode(modeNormal);
	}

	// Long text cursor blink on PgUp/PgDown/Up/Down/Home/End.
	switch (event->key())
	{
		case Qt::Key_PageUp:
		case Qt::Key_PageDown:
		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Home:
		case Qt::Key_End:
			makeLongTextCursorBlink();
			break;
	}

	// Handle keyboard navigation between cells.
	if (event->modifiers() == CellNavigationModifiers)
	{
		switch (event->key())
		{
			case Qt::Key_Left:
				// Move left
				m_table->moveLeft();
				makeLongTextCursorBlink();
				return;
			case Qt::Key_Right:
				// Move right.
				m_table->moveRight();
				makeLongTextCursorBlink();
				return;
			case Qt::Key_Up:
				// Move up.
				m_table->moveUp();
				makeLongTextCursorBlink();
				return;
			case Qt::Key_Down:
				// Move down.
				m_table->moveDown();
				makeLongTextCursorBlink();
				return;
		}
	}

	// Pass all other keys to text frame of active cell.
	bool repeat;
	m_table->activeCell().textFrame()->handleModeEditKey(event, repeat);
	updateCanvas(true);
}

void CanvasMode_EditTable::mouseMoveEvent(QMouseEvent* event)
{
	event->accept();

	QPointF canvasPoint = m_canvas->globalToCanvas(event->globalPos()).toQPointF();
	double threshold = m_doc->guidesPrefs().grabRadius / m_canvas->scale();
	TableHandle handle = m_table->hitTest(canvasPoint, threshold);

	if (event->buttons() & Qt::LeftButton)
	{
		// Mouse is being dragged with left button pressed.
		handleMouseDrag(event);
	}
	else
	{
		// Set an appropriate cursor.
		QCursor cursor(Qt::ArrowCursor);
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
			case TableHandle::CellSelect:
				cursor = Qt::IBeamCursor;
				break;
			case TableHandle::None:
				break;
			default:
				qWarning("Unknown hit target");
				break;
		}
		m_view->setCursor(cursor);
	}
}

void CanvasMode_EditTable::mousePressEvent(QMouseEvent* event)
{
	PageItem_TextFrame* activeFrame;

	event->accept();
	QPointF canvasPoint = m_canvas->globalToCanvas(event->globalPos()).toQPointF();
	double threshold = m_doc->guidesPrefs().grabRadius / m_canvas->scale();
	TableHandle handle = m_table->hitTest(canvasPoint, threshold);

	if (event->button() == Qt::LeftButton)
	{
		switch (handle.type())
		{
			case TableHandle::RowSelect:
				// Not implemented.
				break;
			case TableHandle::RowResize:
				// Start row resize gesture.
				m_rowResizeGesture->setup(m_table, handle.index());
				m_view->startGesture(m_rowResizeGesture);
				break;
			case TableHandle::ColumnSelect:
				// Not implemented.
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
				// Move to the pressed cell and position the text cursor.
				m_table->moveTo(m_table->cellAt(canvasPoint));
				m_view->slotSetCurs(event->globalPos().x(), event->globalPos().y());
				m_lastCursorPos = m_table->activeCell().textFrame()->itemText.cursorPosition();
				m_view->m_ScMW->setTBvals(m_table->activeCell().textFrame());
				makeLongTextCursorBlink();
				updateCanvas(true);
				break;
			case TableHandle::None:
				// Deselect text in active frame.
				activeFrame = m_table->activeCell().textFrame();
				activeFrame->itemText.deselectAll();
				activeFrame->HasSel = false;
				// Deselect the table and go back to normal mode.
				m_view->Deselect(true);
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
		//m_view->m_ScMW->scrMenuMgr->runMenuAtPos("ItemTable", event->globalPos());
		const FPoint mousePointDoc(m_canvas->globalToCanvas(event->globalPos()));
		createContextMenu(m_table, mousePointDoc.x(), mousePointDoc.y());
	}
}

void CanvasMode_EditTable::mouseReleaseEvent(QMouseEvent* event)
{
	m_lastCursorPos = -1;
}

void CanvasMode_EditTable::mouseDoubleClickEvent(QMouseEvent* event)
{
	TableCell hitCell = m_table->cellAt(m_canvas->globalToCanvas(event->globalPos()).toQPointF());
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
		uint pos = textFrame->itemText.selectWord(textFrame->itemText.cursorPosition());
		textFrame->itemText.setCursorPosition(pos);
	}

	updateCanvas(true);
}

void CanvasMode_EditTable::drawControls(QPainter* p)
{
	p->save();
	commonDrawControls(p, false);
	drawTextCursor(p);
	p->restore();
}

void CanvasMode_EditTable::updateCanvas(bool forceRedraw)
{
	// Do not let update timer cancel forced redraw otherwise
	// we get refresh issues  when typing or selecting text
	if (!m_canvas->isForcedRedraw())
		m_canvas->setForcedRedraw(forceRedraw);
	m_canvas->update(m_canvas->canvasToLocal(m_table->getBoundingRect()));
}

void CanvasMode_EditTable::handleMouseDrag(QMouseEvent* event)
{
	TableCell hitCell = m_table->cellAt(m_canvas->globalToCanvas(event->globalPos()).toQPointF());
	TableCell activeCell = m_table->activeCell();
	PageItem_TextFrame* activeFrame = activeCell.textFrame();

	if ((!hitCell.isValid() || hitCell == activeCell) && m_lastCursorPos != -1)
	{
		// Select text in active cell text frame.
		activeFrame->itemText.deselectAll();
		activeFrame->HasSel = false;
		m_view->slotSetCurs(event->globalPos().x(), event->globalPos().y());

		PageItem_TextFrame* newActiveFrame = m_table->activeCell().textFrame();
		if (activeFrame == newActiveFrame)
		{
			const int selectionStart = qMin(activeFrame->itemText.cursorPosition(), m_lastCursorPos);
			const int selectionLength = qAbs(activeFrame->itemText.cursorPosition() - m_lastCursorPos);

			activeFrame->itemText.select(selectionStart, selectionLength);
			activeFrame->HasSel = (selectionLength > 0);
		}
		else
		{
			m_lastCursorPos = -1;
			m_cellSelectGesture->setup(m_table, activeCell);
			m_view->startGesture(m_cellSelectGesture);
		}
		m_view->m_ScMW->setTBvals(newActiveFrame);
	}
	else
	{
		/*
		 * Mouse moved into another cell, so deselect all text and start
		 * cell selection gesture.
		 */
		activeFrame->itemText.deselectAll();
		activeFrame->HasSel = false;

		m_cellSelectGesture->setup(m_table, activeCell);
		m_view->startGesture(m_cellSelectGesture);
	}

	updateCanvas(true);
}

void CanvasMode_EditTable::drawTextCursor(QPainter* p)
{
	if ((!m_longBlink && m_blinkTime.elapsed() > qApp->cursorFlashTime() / 2)
		|| (m_longBlink && m_blinkTime.elapsed() > qApp->cursorFlashTime()))
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
		commonDrawTextCursor(p, m_table->activeCell().textFrame(), m_table->gridOffset());
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
	ContextMenu* cmen=NULL;
	m_view->setCursor(QCursor(Qt::ArrowCursor));
	m_view->setObjectUndoMode();
	if(currItem!=NULL)
		cmen = new ContextMenu(*(m_doc->m_Selection), m_ScMW, m_doc);
	else
		cmen = new ContextMenu(m_ScMW, m_doc, mx, my);
	if (cmen)
		cmen->exec(QCursor::pos());
	m_view->setGlobalUndoMode();
	delete cmen;
}
