/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABLEPAINTER_H
#define TABLEPAINTER_H

class PageItem_Table;
class ScPainter;

/**
 * The TablePainter class is an abstract base class for table painters.
 *
 * Table painters are responsible for painting a PageItem_Table. Subclasses must
 * implement the pure virtual function paintTable().
 */
class TablePainter
{
public:
	/// Creates a new table painter configured to paint @a table.
	explicit TablePainter(PageItem_Table *table) : m_table(table) {};
	virtual ~TablePainter() {};

	/// Paints the table using @a p.
	virtual void paintTable(ScPainter* p) = 0;

	/// Returns the table this table painter is configured to paint.
	PageItem_Table* table() const { return m_table; };

private:
	PageItem_Table* m_table;
};

#endif // TABLEPAINTER_H
