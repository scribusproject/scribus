/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABLECOLUMNWIDTHSDIALOG_H
#define TABLECOLUMNWIDTHSDIALOG_H

#include "ui_tablecolumnwidthsdialog.h"

class ScribusDoc;

/**
 * This is the dialog used when setting column widths on a table.
 */
class TableColumnWidthsDialog : public QDialog, private Ui::TableColumnWidthsDialog
{
	Q_OBJECT
public:
	/// Constructs a new dialog. The dialog will use the unit of @a doc.
	explicit TableColumnWidthsDialog(ScribusDoc* doc, QWidget *parent = 0);

	/// Returns the column width the user entered.
	qreal columnWidth() const;

private:
	ScribusDoc* m_doc;
};

#endif // TABLECOLUMNWIDTHSDIALOG_H
