/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "multicombobox.h"

#include <QKeyEvent>
#include <QObject>
#include <QWidget>

MultiComboboxModel::MultiComboboxModel(QObject* parent) : QStandardItemModel(parent)
{
	insertColumn(0);
}

Qt::ItemFlags MultiComboboxModel::flags(const QModelIndex& index) const
{
	return QStandardItemModel::flags(index) | Qt::ItemIsUserCheckable;
}

MultiView::MultiView(QWidget* parent) : QListView(parent)
{
	parentMcb = nullptr;
}

MultiView::MultiView(MultiCombobox* parent) : QListView(parent)
{
	parentMcb = parent;
}

bool MultiView::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent* mouse = static_cast<QMouseEvent*> (event);
		QModelIndex index = indexAt(mouse->pos());

		if (index.isValid())
		{
			QModelIndexList selectedList = selectedIndexes();
			for (QModelIndex selectedIndex : std::as_const(selectedList))
				parentMcb->switchCheckState(selectedIndex.row());
			return true;
		}
	}
	else if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* key = static_cast<QKeyEvent*>(event);

		if ((key->key() == Qt::Key_Return) || (key->key() == Qt::Key_Enter))
		{
			QModelIndexList selectedList = selectedIndexes();
			for (QModelIndex selectedIndex : std::as_const(selectedList))
				parentMcb->switchCheckState(selectedIndex.row());
			return true;
		}
	}

	return false;
}

MultiCombobox::MultiCombobox(QWidget* parent) : QComboBox(parent)
{
	mcbModel = new MultiComboboxModel(this);
	mcbView = new MultiView(this);

	setModel(mcbModel);
	setView(mcbView);

	mcbView->installEventFilter(mcbView);
	mcbView->viewport()->installEventFilter(mcbView);

	view()->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void MultiCombobox::setCheckState(int index, int checked)
{
	int itemsCount = count();
	if (index < 0 || index >= itemsCount)
		return;

	if (checked == 1)
		QComboBox::setItemData(index, Qt::Checked, Qt::CheckStateRole);
	else if (checked == 2)
		QComboBox::setItemData(index, Qt::PartiallyChecked, Qt::CheckStateRole);
	else
		QComboBox::setItemData(index, Qt::Unchecked, Qt::CheckStateRole);
}

int MultiCombobox::checkState(int index) const
{
	int itemsCount = count();
	if (index < 0 || index >= itemsCount)
		return false;

	QVariant var = QComboBox::itemData(index, Qt::CheckStateRole);

	if (var == Qt::Checked)
		return 1;
	if (var == Qt::PartiallyChecked)
		return 2;
	return 0;
}

void MultiCombobox::switchCheckState(int row)
{
	if (checkState(row))
		setCheckState(row, 0);
	else
		setCheckState(row, 1);

	emit checkStateChanged(row);
}

int MultiCombobox::addItem(const QString& text, int checked)
{
	QComboBox::addItem(text);

	int itemsCount = count();
	setCheckState(itemsCount - 1, checked);

	return itemsCount - 1;
}
