/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef INSERTTABLEROWSDIALOG_H
#define INSERTTABLEROWSDIALOG_H

#include "ui_inserttablerowsdialog.h"

class PageItem_Table;

/**
 * This is the dialog used during insertion of rows into tables.
 */
class InsertTableRowsDialog : public QDialog, private Ui::InsertTableRowsDialog
{
	Q_OBJECT
public:
	/// This enum specifies positions at which the user can insert rows.
	enum Position
	{
		Before, /**< Inserts rows before the current row. */
		After   /**< Inserts rows after the current row. */
	};

public:
	/// Constructs a new dialog for inserting rows. @a appMode is the current application mode.
	explicit InsertTableRowsDialog(int appMode, QWidget *parent = 0);

	/// Returns the number of rows the user entered.
	int numberOfRows() const { return numRows->value(); }

	/// Returns the position the user picked.
	Position position() const { return static_cast<Position>(positions->checkedId()); }
};

#endif // INSERTTABLEROWSDIALOG_H
