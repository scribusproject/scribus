/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABLEBORDER_H
#define TABLEBORDER_H

#include <QList>
#include <QPen>
#include <QString>

#include "scribusapi.h"

/**
 * The TableBorderLine class represents a single line in a table border.
 */
class SCRIBUS_API TableBorderLine
{
public:
	/// Creates a new solid black 0 width table border line.
	TableBorderLine();

	/// Creates a new table border line with @a width, @a style and @a color.
	TableBorderLine(double width, Qt::PenStyle style, const QString& color);

	/// Returns the width of this table border line.
	double width() const { return m_width; }

	/// Sets the width of this table border line to @a width.
	void setWidth(double width) { m_width = width; }

	/// Returns the pen style of this table border line.
	Qt::PenStyle style() const { return m_style; }

	/// Sets the pen style of this table border line to @a style.
	void setStyle(Qt::PenStyle style) { m_style = style; }

	/// Returns the color of this table border line.
	QString color() const { return m_color; }

	/// Sets the color of this table border line to @a color.
	void setColor(const QString& color) { m_color = color; }

	/// Returns a string representation of the border line.
	QString asString() const;

	/// Returns <code>true</code> if this table border line is equal to @a other.
	bool operator==(const TableBorderLine& other) const
	{
		return width() == other.width() && color() == other.color() && style() == other.style();
	}

	/// Returns <code>true</code> if this table border line is not equal to @a other.
	bool operator!=(const TableBorderLine& other) const { return !(*this == other); }

	/// Returns <code>true</code> if this table border line is thinner than @a other.
	bool operator<(const TableBorderLine& other) const { return width() < other.width(); }

private:
	/// The width of the table border line.
	double m_width;
	/// The pen style of the table border line.
	Qt::PenStyle m_style;
	/// The color of the table border line.
	QString m_color;
};
Q_DECLARE_TYPEINFO(TableBorderLine, Q_MOVABLE_TYPE);

/**
 * The TableBorder class represents a border on a table or table cell.
 *
 * It consists of a list of table border lines kept in descending order by width.
 */
class SCRIBUS_API TableBorder
{
public:
	/// Creates a new null table border. Null borders are used to indicate the absence of a border.
	TableBorder() {}

	/// Creates a new table border with a single border line with @a width, @a style and @a color.
	TableBorder(double width, Qt::PenStyle style, const QString& color);

	/// Returns the width of the first table border line, or 0.0 if this is a null border.
	double width() const;

	/// Returns the list of border lines for this border in the order they should be painted.
	QList<TableBorderLine> borderLines() const { return m_borderLines; }

	/**
	 * Returns the border line at @a index from the list of border lines for this border, or
	 * a default constructed border line if there is no border line at @a index.
	 */
	TableBorderLine borderLine(int index) const;

	/// Adds @a borderLine to the list of border lines for this border.
	void addBorderLine(const TableBorderLine& borderLine);

	/**
	 * Removes the border line at @a index from the list of border lines for this border.
	 * If there is no border line at @a index, this method does nothing.
	 */
	void removeBorderLine(int index);

	/**
	 * Replaces the border line at @a index in the list of border lines for this border.
	 * If there is no border line at @a index, this method does nothing.
	 */
	void replaceBorderLine(int index, const TableBorderLine& borderLine);

	/**
	 * Returns <code>true</code> if this border can be joined with @a other.
	 * Two borders can be joined if they are both not null and their list of border lines are equal.
	 */
	bool joinsWith(const TableBorder& other) const { return !isNull() && !other.isNull() && borderLines() == other.borderLines(); }

	/// Returns <code>true</code> if this border has no border lines.
	bool isNull() const { return m_borderLines.size() == 0; }

	/// Returns a string representation of the border.
	QString asString() const;

private:
	/// List of border lines in the order they should be painted.
	QList<TableBorderLine> m_borderLines;
};

/**
 * Returns <code>true</code> if the two borders @a lfs and @a rhs are equal.
 *
 * Two borders are equal if they are both not null and they have their lists of border lines are equal.
 */
inline bool operator==(const TableBorder& lhs, const TableBorder& rhs)
{
	return !lhs.isNull() && !rhs.isNull() && (lhs.borderLines() == rhs.borderLines());
}

/**
 * Returns <code>true</code> if the two borders @a lfs and @a rhs are not equal.
 *
 * Two borders are equal if either of them are null, or if their lists of border lines are not equal.
 */
inline bool operator!=(const TableBorder& lhs, const TableBorder& rhs)
{
	return !(lhs == rhs);
}

#endif // TABLEBORDER_H
