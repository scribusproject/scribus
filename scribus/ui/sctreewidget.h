/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          sctreewidget.h  -  description
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

#ifndef SCTREEWIDGET_H
#define SCTREEWIDGET_H

#include <QItemDelegate>
#include <QTreeWidget>
#include <QString>
#include <QWidget>
#include <QHash>
#include <QEvent>
#include "scribusapi.h"

class SCRIBUS_API ScTreeWidgetDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	ScTreeWidgetDelegate(QTreeWidget *view, QWidget *parent);
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const;

private:
	QTreeWidget *m_view;
};


class SCRIBUS_API ScTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	ScTreeWidget(QWidget* pa);
	~ScTreeWidget() {}

	QTreeWidgetItem* addWidget(const QString& title, QWidget* widget);
	void setToolBoxMode(bool enable);
	int addItem(QWidget* widget, const QString& title);
	QWidget* widget(int index);
	void setItemEnabled(int index, bool enable);
	bool isItemEnabled(int index);
	void setCurrentIndex(int index);
	int currentIndex();
	void setItemText(int index, const QString& text);

private slots:
	void handleMousePress(QTreeWidgetItem *item);

signals:
	void currentChanged2(int);

private:
	bool m_toolboxMode { false };
	QHash<int, QTreeWidgetItem*> keySList;

protected:
	bool event(QEvent *e);
};
#endif
