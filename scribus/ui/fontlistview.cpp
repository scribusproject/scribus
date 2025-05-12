/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fontlistview.h"
#include "fontlistmodel.h"

#include <QMenu>
#include <QModelIndex>
#include <QPoint>

FontListView::FontListView(QWidget * parent)
	: QTableView(parent),
	contextMenu(new QMenu(this)),
	activateAction(new QAction(tr("&Activate"), this)),
	deactivateAction(new QAction(tr("&Deactivate"), this)),
	subsetAction(new QAction(tr("&Subset"), this)),
	notSubsetAction(new QAction(tr("Do &not subset"), this))
{
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	sortByColumn(FontListModel::SortIndex, Qt::AscendingOrder);
	// do not show default sorting to user
	hideColumn(FontListModel::SortIndex);
	setSortingEnabled(false);
	setAlternatingRowColors(true);
	setContextMenuPolicy(Qt::CustomContextMenu);

	contextMenu->addAction(activateAction);
	contextMenu->addAction(deactivateAction);
	contextMenu->addAction(subsetAction);
	contextMenu->addAction(notSubsetAction);

	connect(activateAction, &QAction::triggered, this, &FontListView::activateSelection);
	connect(deactivateAction, &QAction::triggered, this, &FontListView::deactivateSelection);
	connect(subsetAction, &QAction::triggered, this, &FontListView::subsetSelection);
	connect(notSubsetAction, &QAction::triggered, this, &FontListView::notSubsetSelection);

	connect(this, &QTableView::customContextMenuRequested, this, &FontListView::customContextMenuRequested);
}

void FontListView::setModel(QAbstractItemModel * model)
{
	QTableView::setModel(model);
	resizeColumnsToContents();
}

void FontListView::setFonts(const SCFonts& f)
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

void FontListView::customContextMenuRequested(QPoint pos)
{
    contextMenu->popup(viewport()->mapToGlobal(pos));
}

void FontListView::activateSelection()
{
	FontListModel* fontModel = qobject_cast<FontListModel*>(model());
	for (const auto& selectedRow: selectionModel()->selectedRows())
	{
		fontModel->setData(selectedRow, Qt::Checked, Qt::CheckStateRole);
	}
}

void FontListView::deactivateSelection()
{
	FontListModel* fontModel = qobject_cast<FontListModel*>(model());
	for (const auto& selectedRow: selectionModel()->selectedRows())
	{
		fontModel->setData(selectedRow, Qt::Unchecked, Qt::CheckStateRole);
	}
}

void FontListView::subsetSelection()
{
	FontListModel* fontModel = qobject_cast<FontListModel*>(model());
	for (const auto& selectedRow: selectionModel()->selectedRows())
	{
		fontModel->setData(fontModel->index(selectedRow.row(), FontListModel::FontSubset), Qt::Checked, Qt::CheckStateRole);
	}
}

void FontListView::notSubsetSelection()
{
	FontListModel* fontModel = qobject_cast<FontListModel*>(model());
	for (const auto& selectedRow: selectionModel()->selectedRows())
	{
		fontModel->setData(fontModel->index(selectedRow.row(), FontListModel::FontSubset), Qt::Unchecked, Qt::CheckStateRole);
	}
}
