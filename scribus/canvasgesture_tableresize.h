/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CANVASGESTURE_TABLERESIZE_H
#define CANVASGESTURE_TABLERESIZE_H

#include <QList>

#include "canvasgesture_table.h"

class CanvasMode;
class PageItem_Table;
class QMouseEvent;
class QKeyEvent;
class QPainter;

/**
 * Handles resizing of tables on behalf of the table editing canvas mode.
 *
 * The user presses the mouse button at the bottom right corner of a table, drags it
 * to a new location and then releases the button. The resize may be cancelled by
 * pressing the Escape key.
 */
class SCRIBUS_API TableResize : public TableGesture
{
	Q_OBJECT

public:
	explicit TableResize(CanvasMode* parent) : TableGesture(parent),
		m_minHeight(0.0), m_minWidth(0.0) {}
	virtual ~TableResize() {}

	virtual void activate(bool fromGesture) {};
	virtual void deactivate(bool forGesture) {}
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual bool handleKeyEvents() { return true; }
	virtual void drawControls(QPainter* p);

	/// Sets up this table resize gesture to resize @a table.
	void setup(PageItem_Table* table);

private:
	/// List of row heights for the table outline.
	QList<qreal> m_rowHeights;
	/// List of row positions for the table outline.
	QList<qreal> m_rowPositions;
	/// List of column widths for the table outline.
	QList<qreal> m_columnWidths;
	/// List of column positions for the table outline.
	QList<qreal> m_columnPositions;
	/// The minimum row height before the gesture started.
	qreal m_minHeight;
	/// The minimum column width before the gesture started.
	qreal m_minWidth;
};

#endif // CANVASGESTURE_TABLERESIZE_H
