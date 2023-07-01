/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABLESIDESELECTOR_H
#define TABLESIDESELECTOR_H

#include <QFlags>
#include <QPen>
#include <QList>
#include <QFrame>

#include "scribusstructs.h"

class QPaintEvent;
class QMouseEvent;
class QEvent;

/**
 * Simple square shaped widget for picking different sides of a cell or table.
 *
 * Used when editing cell and table borders.
 */
class TableSideSelector : public QFrame
{
	Q_OBJECT

	Q_PROPERTY(TableSides selection READ selection WRITE setSelection RESET clearSelection NOTIFY selectionChanged)
	Q_PROPERTY(Style style READ style WRITE setStyle)

public:
	/**
	 * This enum describes two possible styles for the selector. If the cell style is used,
	 * the selector will resemble a cell. In the table style, a table.
	 */
	enum Style
	{
		CellStyle,  /**< The selector will resemble a cell. */
		TableStyle, /**< The selector will resemble table. */
	};

	/// Creates a new side selector with all sides selected.
	TableSideSelector(QWidget* parent);

	/// Returns the current selection as an ORed combination of enum flags.
	TableSides selection() const { return m_selection; }

	/// Returns the current selection as a list of enum values.
	QList<TableSide> selectionList() const;

	/// Sets the current selection to @a selection.
	void setSelection(TableSides selection) { m_selection = selection; emit selectionChanged(); }

	/// Clears the current selection.
	void clearSelection() { m_selection = TableSide::None; emit selectionChanged(); }

	/// Returns the current style of the selector.
	Style style() const { return m_style; }

	/// Sets the style of the selector to @a style
	void setStyle(Style style) { m_style = style; }

	/// Returns the size hint of the table side selector.
	QSize sizeHint() const { return QSize(80, 80); }

	/// Returns @a width, as the selector is supposed to be square shaped.
	int heightForWidth(int width) const { return width; }

signals:
	/// Emitted when the selections has changed.
	void selectionChanged();

protected:
	/// Paints the table side selector.
	void paintEvent(QPaintEvent* event) override;
	/// Toggles selection of the edge closest to mouse pointer.
	void mousePressEvent(QMouseEvent* event) override;
	/// Highlights the edge closest to mouse pointer.
	void mouseMoveEvent(QMouseEvent* event) override;
	/// Removes any highlighted edge.
	void leaveEvent(QEvent* event) override;

private:
	/// Utility function to return the side closest to @a point.
	TableSide closestSide(const QPointF& point) const;

private:
	/// The current selection.
	TableSides m_selection;

	/// The current style of the selector.
	Style m_style;

	/// The side currently closest to the mouse pointer.
	TableSide m_highlighted;

	/// The left side line.
	QLineF m_left;

	/// The right side line.
	QLineF m_right;

	/// The top side line.
	QLineF m_top;

	/// The bottom side line.
	QLineF m_bottom;
};

#endif // TABLESIDESELECTOR_H
