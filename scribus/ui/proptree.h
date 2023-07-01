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
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	virtual void destroyEditor(QWidget * editor, const QModelIndex & index) const;
	virtual void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const;

private slots:
	void valueHasChanged();

private:
	PropTreeWidget *m_parent;
	mutable QWidget *m_edit {nullptr};
};

class SCRIBUS_API PropTreeItem : public QObject, public QTreeWidgetItem
{
	Q_OBJECT

public:
	PropTreeItem(QTreeWidget* parent, int typ, const QString& title);
	PropTreeItem(PropTreeItem* parent, int typ, const QString& title);
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
		ColorComboBox = 5
	};

	int valueAsInt();
	double valueAsDouble();
	bool valueAsBool();
	QString valueAsString();
	void setIntValue(int value);
	void setDoubleValue(double value);
	void setBoolValue(bool value);
	void setStringValue(const QString& value);
	void setUnitValue(int unit);
	void setDecimalsValue(int unit);
	void setComboStrings(const QStringList& value);
	void setMinMaxValues(int min, int max);
	void setMinMaxValues(double min, double max);
	void setColorList(const ColorList& colors);

	int m_type;
	int m_unit = 0;
	int m_decimals = 0;
	double m_fmin = 0.0;
	double m_fmax = 0.0;
	int m_min = 0;
	int m_max = 0;
	int m_comboIndex = -1;
	ColorList m_colors;

signals:
	void valueChanged(int);
	void valueChanged(double);
	void valueChanged(bool);
	void valueChanged(QString);
	void editFinished();
	void editStarted();

public slots:
	void localeChange();
};

class SCRIBUS_API PropTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	PropTreeWidget(QWidget* pa);
	PropTreeItem* indexToItem(const QModelIndex &index);
	~PropTreeWidget() {};

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
	PropTreeItemDelegate *delegate { nullptr };

private slots:
	void handleMousePress(QTreeWidgetItem *item);
};

#endif // PROPTREE_H
