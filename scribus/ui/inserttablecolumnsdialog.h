/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef INSERTTABLECOLUMNSDIALOG_H
#define INSERTTABLECOLUMNSDIALOG_H

#include "ui_inserttablecolumnsdialog.h"

class PageItem_Table;

/**
 * This is the dialog used during insertion of columns into tables.
 */
class InsertTableColumnsDialog : public QDialog, private Ui::InsertTableColumnsDialog
{
	Q_OBJECT
public:
	/// This enum specifies positions at which the user can insert columns.
	enum Position
	{
		Before, /**< Inserts columns before the current column. */
		After   /**< Inserts columns after the current column. */
	};

public:
	/// Constructs a new dialog for inserting columns. @a appMode is the current application mode.
	explicit InsertTableColumnsDialog(int appMode, QWidget *parent = 0);

	/// Returns the number of columns the user entered.
	int numberOfColumns() const { return numColumns->value(); }

	/// Returns the position the user picked.
	Position position() const { return static_cast<Position>(positions->checkedId()); }
};

#endif // INSERTTABLECOLUMNSDIALOG_H
