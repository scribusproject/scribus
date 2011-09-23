/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pageitem_table.h"
#include "scribusdoc.h"

#include "tablecolumnwidthsdialog.h"

TableColumnWidthsDialog::TableColumnWidthsDialog(ScribusDoc* doc, QWidget *parent) : QDialog(parent),
	m_doc(doc)
{
	Q_ASSERT(m_doc);

	setupUi(this);

	tableColumnWidth->setNewUnit(m_doc->unitIndex());
	tableColumnWidth->setMinimum(PageItem_Table::MinimumColumnWidth * m_doc->unitRatio());
}

qreal TableColumnWidthsDialog::columnWidth() const
{
	Q_ASSERT(m_doc);

	return tableColumnWidth->getValue(m_doc->unitIndex());
}
