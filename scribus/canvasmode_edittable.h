/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CANVASMODE_EDITTABLE_H
#define CANVASMODE_EDITTABLE_H

#include <QCursor>
#include <QElapsedTimer>
#include <QObject>

#include "canvasgesture_cellselect.h"
#include "canvasmode.h"
#include "cellarea.h"

class CanvasGesture;
class CellSelect;
class ColumnResize;
class PageItem_Table;
class QMouseEvent;
class QPainter;
class QPointF;
class QTimer;
class RowResize;
class ScribusMainWindow;
class ScribusView;
class TableResize;

/**
 * This is a canvas mode for editing tables.
 */
class CanvasMode_EditTable :  public CanvasMode
{
	Q_OBJECT

public:
	explicit CanvasMode_EditTable(ScribusView* view);
	~CanvasMode_EditTable() override;

	void activate(bool fromGesture) override;
	void deactivate(bool forGesture) override;
	void keyPressEvent(QKeyEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void drawControls(QPainter* p) override;
	/// Draws faint hints showing where row/column-select strips and
	/// the table-resize corner are located.
	void drawTableHandleHints(QPainter* p);

protected:
	/**
	* Paints the selection of the currently edited table.
	*/
	void paintCellSelection(QPainter* p);

private slots:
	/**
	 * Updates the part of the canvas containing the table.
	 *
	 * If @a forceRedraw is <code>true</code>, an redraw of the canvas will be forced.
	 */
	void updateCanvas(bool force = false);

private:
	struct TableCanvasControl
	{
		enum Type
		{
			None,
			RowResize,
			ColumnResize,
			InsertRow,
			InsertColumn,
			DeleteRows,
			DeleteColumns
		};

		Type type {None};
		int index {-1};

		bool operator==(const TableCanvasControl& other) const { return type == other.type && index == other.index; }
		bool operator!=(const TableCanvasControl& other) const { return !(*this == other); }
	};

	/// Returns whether the current cell selection covers complete rows or columns.
	CellSelect::SelectionMode structuralSelectionMode() const;
	/// Returns DeleteRows or DeleteColumns with the centre of the - control in frame
	/// coordinates, or None when the selection does not cover complete rows or columns.
	TableCanvasControl::Type deleteControl(double controlScale, QPointF& center) const;
	/// Returns a direct-manipulation control outside the table, if any.
	TableCanvasControl hitTestTableCanvasControls(const QPointF& canvasPoint) const;
	/// Paints hover controls and contextual row/column deletion controls.
	void drawTableCanvasControls(QPainter* p);
	/// Draws a round + or - control at \a center, in table frame coordinates.
	void drawControlButton(QPainter* p, const QPointF& center, bool plus) const;
	/// Draws a resize grip: horizontal above a column boundary, vertical beside a row boundary.
	void drawResizeGrip(QPainter* p, const QPointF& center, Qt::Orientation orientation) const;
	/// Updates the currently hovered direct-manipulation control.
	void setHoveredTableCanvasControl(const TableCanvasControl& control);
	/// Inserts one row/column at a visual boundary (0..count).
	void insertTableRowAtBoundary(int boundary);
	void insertTableColumnAtBoundary(int boundary);

	/// Handles a mouse move event with the left button held down.
	void handleMouseDrag(QMouseEvent* event);
	/// Draws the text cursor for the currently active cell.
	void drawTextCursor(QPainter* p);
	/// Makes the next text cursor blink a long blink.
	void makeLongTextCursorBlink();
	/// Show the Context Menu
	void createContextMenu(PageItem *currItem, double mx, double my);
	/// Reset the selection anchor
	void resetSelectionAnchor();
	/// Move the cursor
	bool moveActiveCell(int key);
	/// Maps an arrow key to the column direction it moves in. Left and Right
	/// are swapped when the table is RTL, so the cell motion mirrors LTR.
	int logicalKey(int key) const;
	void navigateCells(int key);
	void extendCellSelection(int key);
	bool cursorAtCellBoundary(int key) const;
	bool tryRowWrap(int key, int beforeRow, int beforeCol);
	/// Refreshes the text toolbar and horizontal ruler for the active cell.
	void updateTextEditUIForActiveCell();

private:
	/// Table being edited.
	PageItem_Table *m_table {nullptr};

	/// Timer for canvas updates.
	QTimer* m_canvasUpdateTimer;

	/// Time since last text cursor blink.
	QElapsedTimer m_blinkTime;
	/// <code>true</code> if the text cursor should make a long blink.
	bool m_longBlink {false};
	/// <code>true</code> if the text cursor is visible.
	bool m_cursorVisible {false};

	// Last cursor position pressed with the mouse, or <code>-1</code>.
	int m_lastCursorPos {-1};

	/// The "select row" mouse cursor.
	QCursor m_selectRowCursor;
	/// The "select column" mouse cursor.
	QCursor m_selectColumnCursor;

	/// Gesture for resizing a table.
	TableResize* m_tableResizeGesture;
	/// Gesture for resizing a row.
	RowResize* m_rowResizeGesture;
	/// Gesture for resizing a column.
	ColumnResize* m_columnResizeGesture;
	/// Gesture for selecting cells.
	CellSelect* m_cellSelectGesture;

	///Parent for context menu
	ScribusMainWindow* m_ScMW;

	//Anchor cells for keyboard selection
	int m_selectionAnchorRow { -1 };
	int m_selectionAnchorColumn { -1 };

	/// The selection unit a subsequent drag should extend by.
	CellSelect::SelectionMode m_dragSelectionMode { CellSelect::Cells };

	/// Direct table control currently under the pointer.
	TableCanvasControl m_hoverTableCanvasControl;
	/// Prevents a click on +/- from turning into a cell-selection drag.
	bool m_tableCanvasControlPressed {false};
};

#endif // CANVASMODE_EDITTABLE_H
