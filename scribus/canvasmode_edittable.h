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

#include "canvasmode.h"
#include "cellarea.h"

class CanvasGesture;
class CellSelect;
class ColumnResize;
class PageItem_Table;
class QMouseEvent;
class QPainter;
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
	/// Handles a mouse move event with the left button held down.
	void handleMouseDrag(QMouseEvent* event);
	/// Draws the text cursor for the currently active cell.
	void drawTextCursor(QPainter* p);
	/// Makes the next text cursor blink a long blink.
	void makeLongTextCursorBlink();
	/// Show the Context Menu
	void createContextMenu(PageItem *currItem, double mx, double my);

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
};

#endif // CANVASMODE_EDITTABLE_H
