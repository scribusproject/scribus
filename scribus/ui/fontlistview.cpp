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
	sortByColumn(FontListModel::SortIndex, Qt::AscendingOrder);
	// do not show default sorting to user
	hideColumn(FontListModel::SortIndex);
	setSortingEnabled(false);
	setAlternatingRowColors(true);
}

void FontListView::setModel(QAbstractItemModel * model)
{
	QTableView::setModel(model);
	resizeColumnsToContents();
}

void FontListView::setFonts(SCFonts f)
{
	qobject_cast<FontListModel*>(model())->setFonts(f.keys());
	if (!isSortingEnabled())
		sortByColumn(FontListModel::SortIndex, Qt::AscendingOrder);
}

QString FontListView::fontName(int i)
{
	FontListModel* fontModel = qobject_cast<FontListModel*>(model());
	QModelIndex cellIndex = fontModel->index(i, FontListModel::FontName);
	return fontModel->nameForIndex(cellIndex);
}

bool FontListView::isFontUsable(int i)
{
	FontListModel* fontModel = qobject_cast<FontListModel*>(model());
	QModelIndex cellIndex = fontModel->index(i, FontListModel::FontUsable);
	QVariant    checkData = fontModel->data(cellIndex, Qt::CheckStateRole);
	Qt::CheckState checkState = static_cast<Qt::CheckState>(checkData.toInt());
	return (checkState == Qt::Checked);
}

bool FontListView::isFontSubsetted(int i)
{
	FontListModel* fontModel = qobject_cast<FontListModel*>(model());
	QModelIndex cellIndex = fontModel->index(i, FontListModel::FontSubset);
	QVariant    checkData = fontModel->data(cellIndex, Qt::CheckStateRole);
	Qt::CheckState checkState = static_cast<Qt::CheckState>(checkData.toInt());
	return (checkState == Qt::Checked);
}

