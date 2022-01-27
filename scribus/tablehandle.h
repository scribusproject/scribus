/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABLEHANDLE_H
#define TABLEHANDLE_H

/**
  * This class represents handles on a table.
  *
  * These are areas that when clicked/dragged will initiate a resize or selection
  * of some parts of the table. To test for a handle at some point, use the hitTest()
  * function of PageItem_Table.
  */
class TableHandle
{
public:
	/// This enum specifies different types of handles on a table.
	enum Type
	{
		RowSelect,    /**< Row selection handle. */
		RowResize,    /**< Row resize handle. */
		ColumnSelect, /**< Column selection handle. */
		ColumnResize, /**< Column resize handle. */
		TableResize,  /**< Table resize handle. */
		CellSelect,   /**< Cell selection handle. */
		None          /**< No handle was hit. */
	};

	/// Returns the type of the handle.
	Type type() const { return m_type; }

	/**
	 * If type() is RowResize or ColumnResize, this function returns the index of the
	 * row or column the handle is for, otherwise its return value is undefined.
	 */
	int index() const { return m_index; }

private:
	/// The table is the only one who is allowed to construct/manipulate handles.
	friend class PageItem_Table;

	/// Creates a handle of type @a type.
	TableHandle(Type type) : m_type(type), m_index(-1) {}
	/// Creates a handle of type @a type and with row or column index @a index.
	TableHandle(Type type, int index) : m_type(type), m_index(index) {}

	/// Sets the type of the handle to @a type.
	void setType(Type type) { m_type = type; }
	/// Sets the row or column index of the handle to @a index.
	void setIndex(int index) { m_index = index; }

	/// The type of the handle.
	Type m_type;
	/// The row or column index, if applicable.
	int m_index;
};

#endif // TABLEHANDLE_H
