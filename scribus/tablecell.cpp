/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QRectF>
#include <QString>

#include "commonstrings.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribusdoc.h"
#include "scpainter.h"
#include "tableutils.h"

#include "tablecell.h"

using namespace TableUtils;

TableCell::TableCell(int row, int column, PageItem_Table *table) : d(new TableCellData)
{
	Q_ASSERT(table);
	d->table = table;

	// The context for the internal style is the document-wide context.
	d->style.setContext(&d->table->doc()->cellStyles());

	// Create a text frame for the cell.
	d->textFrame = new PageItem_TextFrame(d->table->m_Doc,
		0, 0, 0, 0, 0, CommonStrings::None, CommonStrings::None);

	setValid(true);
	setRow(row);
	setColumn(column);
	setRowSpan(1);
	setColumnSpan(1);
}

QRectF TableCell::boundingRect() const
{
	if (!isValid())
		return QRectF();

	const int endRow = row() + rowSpan() - 1;
	const int endCol = column() + columnSpan() - 1;

	const qreal x = d->table->columnPosition(column());
	const qreal y = d->table->rowPosition(row());
	const qreal width = d->table->columnPosition(endCol) + d->table->columnWidth(endCol) - x;
	const qreal height = d->table->rowPosition(endRow) + d->table->rowHeight(endRow) - y;

	return QRectF(x, y, width, height);
}

QRectF TableCell::contentRect() const
{
	if (!isValid())
		return QRectF();

	const qreal x = d->textFrame->xPos();
	const qreal y = d->textFrame->yPos();
	const qreal width = d->textFrame->width();
	const qreal height = d->textFrame->height();

	return QRectF(x, y, width, height);
}

void TableCell::setLeftBorder(const TableBorder& border)
{
	d->style.setLeftBorder(border);
	d->table->updateCells();
}

void TableCell::setRightBorder(const TableBorder& border)
{
	d->style.setRightBorder(border);
	d->table->updateCells();
}

void TableCell::setTopBorder(const TableBorder& border)
{
	d->style.setTopBorder(border);
	d->table->updateCells();
}

void TableCell::setBottomBorder(const TableBorder& border)
{
	d->style.setBottomBorder(border);
	d->table->updateCells();
}

void TableCell::setLeftPadding(qreal padding)
{
	d->style.setLeftPadding(padding);
	d->table->updateCells();
}

void TableCell::setRightPadding(qreal padding)
{
	d->style.setRightPadding(padding);
	d->table->updateCells();
}

void TableCell::setTopPadding(qreal padding)
{
	d->style.setTopPadding(padding);
	d->table->updateCells();
}

void TableCell::setBottomPadding(qreal padding)
{
	d->style.setBottomPadding(padding);
	d->table->updateCells();
}

void TableCell::setStyle(const QString& style)
{
	d->style.setParent(style);
	d->table->updateCells();
}

void TableCell::updateContent()
{
	QRectF contentRect = boundingRect();
	contentRect.setLeft(contentRect.left() + leftPadding() + maxLeftBorderWidth()/2);
	contentRect.setTop(contentRect.top() + topPadding() + maxTopBorderWidth()/2);
	contentRect.setWidth(qMax(contentRect.width() - (rightPadding() + maxRightBorderWidth()/2), 1.0));
	contentRect.setHeight(qMax(contentRect.height() - (bottomPadding() + maxBottomBorderWidth()/2), 1.0));

	d->textFrame->setXYPos(contentRect.x(), contentRect.y(), true);
	d->textFrame->setWidthHeight(contentRect.width(), contentRect.height(), true);
	d->textFrame->updateClip();
	d->textFrame->invalidateLayout();
}

void TableCell::setText(const QString& text)
{
	if (!isValid())
		return;

	d->textFrame->itemText.clear();
	d->textFrame->itemText.insertChars(0, text);
}

QString TableCell::asString() const
{
	QString str("cell(");
	str += QString("row=%1, column=%2, ").arg(row()).arg(column());
	str += QString("rowSpan=%1, columnSpan=%2, ").arg(rowSpan()).arg(columnSpan());
	str += QString("leftBorder=%1, rightBorder=%2, ").arg(leftBorder().asString()).arg(rightBorder().asString());
	str += QString("topBorder=%1, bottomBorder=%2, ").arg(topBorder().asString()).arg(bottomBorder().asString());
	str += QString("leftPadding=%1, rightPadding=%2, ").arg(leftPadding()).arg(rightPadding());
	str += QString("topPadding=%1, bottomPadding=%2, ").arg(topPadding()).arg(bottomPadding());
	str += QString("style=%1").arg(d->style.asString());
	str += QString(")");

	return str;
}

qreal TableCell::maxLeftBorderWidth() const
{
	const int endRow = row() + rowSpan() - 1;

	qreal maxWidth = 0.0;
	qreal currentWidth = 0.0;
	for (int currentRow = row(); currentRow <= endRow; ++currentRow)
	{
		TableCell cellLeft = d->table->cellAt(currentRow, column() - 1);
		if (cellLeft.isValid())
			currentWidth = collapseBorders(leftBorder(), cellLeft.rightBorder()).width();
		else
			currentWidth = collapseBorders(leftBorder(), d->table->leftBorder()).width();
		maxWidth = qMax(maxWidth, currentWidth);
	}

	return maxWidth;
}

qreal TableCell::maxRightBorderWidth() const
{
	const int endRow = row() + rowSpan() - 1;
	const int endCol = column() + columnSpan() - 1;

	qreal maxWidth = 0.0;
	qreal currentWidth = 0.0;
	for (int currentRow = row(); currentRow <= endRow; ++currentRow)
	{
		TableCell cellRight = d->table->cellAt(currentRow, endCol + 1);
		if (cellRight.isValid())
			currentWidth = collapseBorders(cellRight.leftBorder(), rightBorder()).width();
		else
			currentWidth = collapseBorders(d->table->rightBorder(), rightBorder()).width();
		maxWidth = qMax(maxWidth, currentWidth);
	}

	return maxWidth;
}

qreal TableCell::maxTopBorderWidth() const
{
	const int endCol = column() + columnSpan() - 1;

	qreal maxWidth = 0.0;
	qreal currentWidth = 0.0;
	for (int currentCol = column(); currentCol <= endCol; ++currentCol)
	{
		TableCell cellAbove = d->table->cellAt(row() - 1, currentCol);
		if (cellAbove.isValid())
			currentWidth = collapseBorders(cellAbove.bottomBorder(), topBorder()).width();
		else
			currentWidth = collapseBorders(topBorder(), d->table->topBorder()).width();
		maxWidth = qMax(maxWidth, currentWidth);
	}

	return maxWidth;
}

qreal TableCell::maxBottomBorderWidth() const
{
	const int endRow = row() + rowSpan() - 1;
	const int endCol = column() + columnSpan() - 1;

	qreal maxWidth = 0.0;
	qreal currentWidth = 0.0;
	for (int currentCol = column(); currentCol <= endCol; ++currentCol)
	{
		TableCell cellBelow = d->table->cellAt(endRow + 1, currentCol);
		if (cellBelow.isValid())
			currentWidth = collapseBorders(bottomBorder(), cellBelow.topBorder()).width();
		else
			currentWidth = collapseBorders(d->table->bottomBorder(), bottomBorder()).width();
		maxWidth = qMax(maxWidth, currentWidth);
	}

	return maxWidth;
}

QDebug operator<<(QDebug debug, const TableCell& cell)
{
	debug.nospace() << cell.asString();
	return debug.space();
}
