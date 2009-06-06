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
		buttonOption.features = QStyleOptionButton::None;
		m_view->style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, m_view);
		QStyleOption branchOption;
		static const int i = 9; // ### hardcoded in qcommonstyle.cpp
		QRect r = option.rect;
		painter->save();
		painter->setBrush(Qt::black);
		painter->setPen(Qt::NoPen);
		QRect rect = QRect(r.left()+5, r.top()+5, r.height()-10, r.height()-10);
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
		QString text = elidedText(option.fontMetrics, textrect.width(), Qt::ElideMiddle, model->data(index, Qt::DisplayRole).toString());
		m_view->style()->drawItemText(painter, textrect, Qt::AlignCenter, option.palette, m_view->isEnabled(), text);
	}
	else
		QItemDelegate::paint(painter, option, index);
}

QSize ScTreeWidgetDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
	QStyleOptionViewItem option = opt;
	QSize sz = QItemDelegate::sizeHint(opt, index) + QSize(2, 2);
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
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(handleMousePress(QTreeWidgetItem*)));
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
	return item1;
}

void ScTreeWidget::handleMousePress(QTreeWidgetItem *item)
{
	if (item == 0)
		return;
	if (item->parent() == 0)
	{
		setItemExpanded(item, !isItemExpanded(item));
		int tops = topLevelItemCount();
		int wide = 0;
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
		if (wide != 0)
			setColumnWidth(0, wide);
		else
			resizeColumnToContents(0);
		return;
	}
}
