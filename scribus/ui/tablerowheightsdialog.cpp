/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pageitem_table.h"
#include "scribusdoc.h"

#include "tablerowheightsdialog.h"

TableRowHeightsDialog::TableRowHeightsDialog(ScribusDoc* doc, QWidget *parent) : QDialog(parent),
	m_doc(doc)
{
	Q_ASSERT(m_doc);

	setupUi(this);

	tableRowHeight->setNewUnit(m_doc->unitIndex());
	tableRowHeight->setMinimum(PageItem_Table::MinimumRowHeight * m_doc->unitRatio());
}

double TableRowHeightsDialog::rowHeight() const
{
	Q_ASSERT(m_doc);

	return tableRowHeight->getValue(m_doc->unitIndex());
}
