/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  proptree.h  -  description
							 -------------------
	begin                : Mon Feb 3 2014
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

#ifndef PROPTREE_H
#define PROPTREE_H

#include <QItemDelegate>
#include <QTreeWidget>
#include <QString>
#include <QWidget>
#include <QHash>
#include <QEvent>
#include "scribusapi.h"
#include "sccolor.h"
class PropTreeWidget;

class SCRIBUS_API PropTreeItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	PropTreeItemDelegate(PropTreeWidget *parent);
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void destroyEditor(QWidget * editor, const QModelIndex & index) const;
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const;
private slots:
	void valueHasChanged();
private:
	PropTreeWidget *m_parent;
	mutable QWidget *m_edit;
};

class SCRIBUS_API PropTreeItem : public QObject, public QTreeWidgetItem
{
	Q_OBJECT

public:
	PropTreeItem(QTreeWidget* parent, int typ, QString title);
	PropTreeItem(PropTreeItem* parent, int typ, QString title);
//	PropTreeItem(QTreeWidget *parent, QString title, int value, int min, int max, int unit);
//	PropTreeItem(QTreeWidget* parent, QString title, double value, double min, double max, int decimals, int unit);
//	PropTreeItem(QTreeWidget *parent, QString title, QString value, QStringList values);
//	PropTreeItem(QTreeWidget* parent, QString title, bool value);
//	PropTreeItem(QTreeWidget *parent, QString title, QString value);
	~PropTreeItem() {};
	enum PropItemType
	{
		Title      = 0,
		IntSpinBox = 1,
		DoubleSpinBox = 2,
		ComboBox      = 3,
		CheckBox      = 4,
		ColorCombo    = 5
	};

	int valueAsInt();
	double valueAsDouble();
	bool valueAsBool();
	QString valueAsString();
	void setIntValue(int value);
	void setDoubleValue(double value);
	void setBoolValue(bool value);
	void setStringValue(QString value);
	void setUnitValue(int unit);
	void setDecimalsValue(int unit);
	void setComboStrings(QStringList value);
	void setMinMaxValues(int min, int max);
	void setMinMaxValues(double min, double max);
	void setColorList(ColorList colors);
	int m_type;
	int m_unit;
	int m_decimals;
	double m_fmin;
	double m_fmax;
	int m_min;
	int m_max;
	int m_comboIndex;
	ColorList m_colors;

signals:
	void valueChanged(int);
	void valueChanged(double);
	void valueChanged(bool);
	void valueChanged(QString);
	void editFinished();
	void editStarted();
};

class SCRIBUS_API PropTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	PropTreeWidget(QWidget* pa);
	PropTreeItem* indexToItem(const QModelIndex &index);
	~PropTreeWidget() {};
protected:
	void mousePressEvent(QMouseEvent *event);
	void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
	PropTreeItemDelegate *delegate;
private slots:
	void handleMousePress(QTreeWidgetItem *item);
};

#endif // PROPTREE_H
