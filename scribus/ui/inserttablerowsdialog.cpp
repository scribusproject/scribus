/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scribusstructs.h"

#include "inserttablerowsdialog.h"

InsertTableRowsDialog::InsertTableRowsDialog(int appMode, QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	positions->setId(beforeButton, Before);
	positions->setId(afterButton, After);

	if (appMode == modeEditTable)
	{
		// In table edit mode we insert before/after current row.
		beforeButton->setText(tr("Before"));
		afterButton->setText(tr("After"));
	}
	else
	{
		// Otherwise we insert at table beginning/end.
		beforeButton->setText(tr("Beginning"));
		afterButton->setText(tr("End"));
	}
}
