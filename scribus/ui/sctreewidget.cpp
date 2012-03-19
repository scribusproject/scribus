/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          sctreewidget.cpp  -  description
                             -------------------
    begin                : Wed Jun 18 2008
    copyright            : (C) 2008 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QAbstractItemModel>
#include <QStyle>
#include <QPainter>
#include <QHeaderView>
#include <QLayout>
#include <QShortcutEvent>

#include "sctreewidget.h"


ScTreeWidgetDelegate::ScTreeWidgetDelegate(QTreeWidget *view, QWidget *parent) : QItemDelegate(parent), m_view(view)
{
}

void ScTreeWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	const QAbstractItemModel *model = index.model();
	Q_ASSERT(model);
	if (!model->parent(index).isValid())
	{
        // this is a top-level item.
		QStyleOptionButton buttonOption;
		buttonOption.state = option.state;
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
		buttonOption.state |= QStyle::State_Raised;
#endif
		buttonOption.state &= ~QStyle::State_HasFocus;
		buttonOption.rect = option.rect;
		buttonOption.palette = option.palette;
		m_view->style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, m_view);
		static const int i = 9; // ### hardcoded in qcommonstyle.cpp
		QRect r = option.rect;
		painter->save();
		if (option.state & QStyle::State_Enabled)
			painter->setBrush(Qt::black);
		else
			painter->setBrush(Qt::gray);
		painter->setPen(Qt::NoPen);
		QRect rect = QRect(r.left()+6, r.top()+6, r.height()-12, r.height()-12);
		QPolygon pa(3);
		if (m_view->isExpanded(index))
		{
			pa.setPoint(0, rect.left(), rect.top());
			pa.setPoint(1, rect.right(), rect.top());
			pa.setPoint(2, rect.center().x(), rect.bottom());
		}
		else
		{
			pa.setPoint(0, rect.left(), rect.top());
			pa.setPoint(1, rect.left(), rect.bottom());
			pa.setPoint(2, rect.right(), rect.center().y());
		}
		painter->setRenderHint(QPainter::Antialiasing, true);
		painter->drawPolygon(pa);
		painter->restore();
			// draw text
		QRect textrect = QRect(r.left() + i*2, r.top(), r.width() - ((5*i)/2), r.height());
		QString text = option.fontMetrics.elidedText(model->data(index, Qt::DisplayRole).toString(), Qt::ElideMiddle, textrect.width(), Qt::TextShowMnemonic);
		m_view->style()->drawItemText(painter, textrect, Qt::AlignCenter | Qt::TextShowMnemonic, option.palette, (option.state & QStyle::State_Enabled), text, QPalette::Text);
	}
	else
		QItemDelegate::paint(painter, option, index);
}

QSize ScTreeWidgetDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
	QSize sz = QItemDelegate::sizeHint(opt, index) + QSize(2, 6);
	return sz;
}

ScTreeWidget::ScTreeWidget(QWidget* pa) : QTreeWidget(pa)
{
	setFocusPolicy(Qt::NoFocus);
	setColumnCount(1);
	setItemDelegate(new ScTreeWidgetDelegate(this, this));
	setRootIsDecorated(false);
	setIndentation(0);
	header()->hide();
	header()->setResizeMode(QHeaderView::Stretch);
	viewport()->setBackgroundRole(QPalette::Window);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_toolbox_mode = false;
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(handleMousePress(QTreeWidgetItem*)));
}

bool ScTreeWidget::event(QEvent *e)
{
	if (e->type() == QEvent::Shortcut)
	{
		QShortcutEvent *se = static_cast<QShortcutEvent *>(e);
		if (se != NULL)
		{
			int k = se->shortcutId();
			QTreeWidgetItem *item1 = keySList.value(k);
			handleMousePress(item1);
			return true;
		}
	}
	return QTreeWidget::event(e);
}

QTreeWidgetItem* ScTreeWidget::addWidget(QString title, QWidget* widget)
{
	QTreeWidgetItem *item1 = new QTreeWidgetItem(this);
	item1->setText(0, title);
	item1->setFlags(Qt::ItemIsEnabled);
	QTreeWidgetItem *item2 = new QTreeWidgetItem(item1);
	item2->setFlags(Qt::ItemIsEnabled);
// hack to work around a bug in Qt-4.3.4
	widget->layout()->activate();
	widget->setMinimumSize(widget->layout()->minimumSize());
	item2->setSizeHint(0, widget->layout()->minimumSize());
// end hack
	setItemWidget(item2, 0, widget);
	QKeySequence newMnemonic = QKeySequence::mnemonic(title);
//	grabShortcut(newMnemonic);
	keySList.insert(grabShortcut(newMnemonic), item1);
	return item1;
}

void ScTreeWidget::setToolBoxMode(bool enable)
{
	m_toolbox_mode = enable;
}

int ScTreeWidget::addItem(QWidget* widget, QString title)
{
	QTreeWidgetItem *top = addWidget(title, widget);
	return indexOfTopLevelItem(top);
}

QWidget* ScTreeWidget::widget(int index)
{
	if ((index < 0) || (index >= topLevelItemCount()))
		return NULL;
	QTreeWidgetItem *top = topLevelItem(index);
	if (top->childCount() == 0)
		return NULL;
	QTreeWidgetItem *child = top->child(0);
	return itemWidget(child, 0);
}

void ScTreeWidget::setItemEnabled(int index, bool enable)
{
	if ((index < 0) || (index >= topLevelItemCount()))
		return;
	if (enable)
		topLevelItem(index)->setFlags(Qt::ItemIsEnabled);
	else
		topLevelItem(index)->setFlags(0);
	QTreeWidgetItem *child = topLevelItem(index)->child(0);
	if (child != NULL)
		itemWidget(child, 0)->setEnabled(enable);
}

bool ScTreeWidget::isItemEnabled(int index)
{
	if ((index < 0) || (index >= topLevelItemCount()))
		return false;
	return !topLevelItem(index)->isDisabled();
}

void ScTreeWidget::setCurrentIndex(int index)
{
	if ((index < 0) || (index >= topLevelItemCount()))
		return;
	int tops = topLevelItemCount();
	for (int t = 0; t < tops; t++)
	{
		setItemExpanded(topLevelItem(t), false);
	}
	QTreeWidgetItem *top = topLevelItem(index);
	setCurrentItem(top);
	setItemExpanded(top, true);
	int wide = 0;
	if (top->childCount() != 0)
	{
		QTreeWidgetItem *child = top->child(0);
		if (child != 0)
			wide = itemWidget(child, 0)->minimumSizeHint().width()+5;
	}
	if (wide != 0)
		setColumnWidth(0, wide);
	else
		resizeColumnToContents(0);
}

int ScTreeWidget::currentIndex()
{
	int index = -1;
	QTreeWidgetItem* item = currentItem();
	if (item->parent() == 0)
		index = indexOfTopLevelItem(item);
	else
		index = indexOfTopLevelItem(item->parent());
	return index;
}

void ScTreeWidget::setItemText(int index, QString text)
{
	if ((index < 0) || (index >= topLevelItemCount()))
		return;
	topLevelItem(index)->setText(0, text);
}

void ScTreeWidget::handleMousePress(QTreeWidgetItem *item)
{
	if (item == 0)
		return;
	if (item->parent() == 0)
	{
		if (item->isDisabled())
		{
			setItemExpanded(item, false);
			return;
		}
		int wide = 0;
		int tops = topLevelItemCount();
		if (m_toolbox_mode)
		{
			for (int t = 0; t < tops; t++)
			{
				setItemExpanded(topLevelItem(t), false);
			}
			setCurrentItem(item);
			setItemExpanded(item, true);
			if (item->childCount() != 0)
			{
				QTreeWidgetItem *child = item->child(0);
				if (child != 0)
					wide = itemWidget(child, 0)->minimumSizeHint().width()+5;
			}
			if (wide != 0)
				setColumnWidth(0, wide);
			else
				resizeColumnToContents(0);
		}
		else
		{
			setItemExpanded(item, !isItemExpanded(item));
			for (int t = 0; t < tops; t++)
			{
				QTreeWidgetItem *top = topLevelItem(t);
				if (isItemExpanded(top))
				{
					if (top->childCount() != 0)
					{
						QTreeWidgetItem *child = top->child(0);
						if (child != 0)
							wide = qMax(wide, itemWidget(child, 0)->minimumSizeHint().width()+5);
					}
				}
			}
		}
		if (wide != 0)
			setColumnWidth(0, wide);
		else
			resizeColumnToContents(0);
		emit currentChanged(indexOfTopLevelItem(item));
	}
}
