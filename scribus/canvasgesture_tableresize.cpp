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
#include "scribusview.h"

#include "canvasgesture_tableresize.h"

void TableResize::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		// Cancel the resize.
		event->accept();
		m_view->stopGesture();
	}
}

void TableResize::mouseReleaseEvent(QMouseEvent* event)
{
	event->accept();

	QPointF gridPoint = globalToTableGrid(event->globalPos());

	// Perform the actual resize of the table.
	table()->resize(gridPoint.x(), gridPoint.y());
	table()->update();

	m_view->stopGesture();
}

void TableResize::mouseMoveEvent(QMouseEvent* event)
{
	event->accept();

	QPointF gridPoint = globalToTableGrid(event->globalPos());

	// Set the column  geometries for the table outline.
	double requestedWidthFactor = gridPoint.x() / table()->tableWidth();
	double newMinWidth = qMax(m_minWidth * requestedWidthFactor, PageItem_Table::MinimumColumnWidth);
	double actualWidthFactor = newMinWidth / m_minWidth;
	for (int col = 0; col < m_columnWidths.size(); ++col)
	{
		m_columnWidths[col] = table()->columnWidth(col) * actualWidthFactor;
		m_columnPositions[col] = table()->columnPosition(col) * actualWidthFactor;
	}

	// Set the row geometries for the table outline.
	double requestedHeightFactor = gridPoint.y() / table()->tableHeight();
	double newMinHeight = qMax(m_minHeight * requestedHeightFactor, PageItem_Table::MinimumRowHeight);
	double actualHeightFactor = newMinHeight / m_minHeight;
	for (int row = 0; row < m_rowHeights.size(); ++row)
	{
		m_rowHeights[row] = table()->rowHeight(row) * actualHeightFactor;
		m_rowPositions[row] = table()->rowPosition(row) * actualHeightFactor;
	}

	// Display size tooltip.
	double actualTableWidth = table()->tableWidth() * actualWidthFactor;
	double actualTableHeight = table()->tableHeight() * actualHeightFactor;
	m_canvas->displaySizeHUD(event->globalPos(), actualTableWidth, actualTableHeight, false);

	m_canvas->update();
}

void TableResize::drawControls(QPainter* p)
{
	p->save();
	commonDrawControls(p, false);
	p->restore();

	// Paint the table outline using the changed table geometries.
	paintTableOutline(m_rowHeights, m_rowPositions, m_columnWidths, m_columnPositions, p);
}

void TableResize::setup(PageItem_Table* table)
{
	Q_ASSERT(table);

	setTable(table);

	// Make copies of the row and column geometries to be used during resize.
	m_rowHeights = table->rowHeights();
	m_rowPositions = table->rowPositions();
	m_columnWidths = table->columnWidths();
	m_columnPositions = table->columnPositions();

	// Save the minimum row height and column width.
	m_minHeight = *std::min_element(m_rowHeights.begin(), m_rowHeights.end());
	m_minWidth = *std::min_element(m_columnWidths.begin(), m_columnWidths.end());
}
