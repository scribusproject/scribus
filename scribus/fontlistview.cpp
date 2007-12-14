/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fontlistview.h"
#include "fontlistmodel.h"


FontListView::FontListView(QWidget * parent)
	: QTableView(parent)
{
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSortingEnabled(false); // TODO
	setAlternatingRowColors(true);
}

void FontListView::setModel(QAbstractItemModel * model)
{
	QTableView::setModel(model);
	resizeColumnsToContents();
}

void FontListView::setFonts(SCFonts f)
{
	qobject_cast<FontListModel*>(model())->setFonts(f);
}
