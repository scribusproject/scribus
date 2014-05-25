/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  proptree.cpp  -  description
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

#include <QAbstractItemModel>
#include <QStyle>
#include <QPainter>
#include <QHeaderView>
#include <QLayout>

#include "proptree.h"

#include "colorcombo.h"
#include "scrspinbox.h"
#include "sccombobox.h"
#include <QCheckBox>
#include <QApplication>
#include "units.h"
#include "util.h"

PropTreeItemDelegate::PropTreeItemDelegate(PropTreeWidget *parent) : QItemDelegate(parent)
{
	m_parent = parent;
}

void PropTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem itemOpts = option;
	itemOpts.state &= ~QStyle::State_HasFocus;
	PropTreeItem* item = (PropTreeItem*)m_parent->indexToItem(index);
	if ((index.column() == 1) && (item->m_type == PropTreeItem::CheckBox) && !(option.state & QStyle::State_Editing))
	{
		QStyleOptionButton buttonOption;
		buttonOption.rect = option.rect;
		buttonOption.palette = option.palette;
		if (index.model()->data(index, Qt::UserRole).toBool())
			buttonOption.state |= QStyle::State_On;
		else
			buttonOption.state |= QStyle::State_Off;
		QApplication::style()->drawControl(QStyle::CE_CheckBox, &buttonOption, painter);
	}
	else if (index.column() == 0)
	{
		int i = 9; // ### hardcoded in qcommonstyle.cpp
		QRect r = option.rect;
		if (item && (item->childCount() > 0))
		{
			painter->save();
			if (option.state & QStyle::State_Enabled)
				painter->setBrush(Qt::black);
			else
				painter->setBrush(Qt::gray);
			painter->setPen(Qt::NoPen);
			QRect rect = QRect(r.left()+6, r.top()+6, r.height()-12, r.height()-12);
			QPolygon pa(3);
			if (item->isExpanded())
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
		}
		painter->save();
		if (item && (item->QTreeWidgetItem::parent()))
			i = 15;
		if (item && (item->treeWidget()->currentItem() == item))
		{
			itemOpts.font.setBold(true);
			painter->setFont(itemOpts.font);
		}
		QRect textrect = QRect(r.left() + i*2, r.top(), r.width() - (i * 2), r.height());
		QString text = itemOpts.fontMetrics.elidedText(index.model()->data(index, Qt::DisplayRole).toString(), Qt::ElideMiddle, itemOpts.rect.width(), Qt::TextShowMnemonic);
		QApplication::style()->drawItemText(painter, textrect, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextShowMnemonic, itemOpts.palette, (itemOpts.state & QStyle::State_Enabled), text, QPalette::Text);
		painter->restore();
	}
	else
		QItemDelegate::paint(painter, itemOpts, index);
	QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &option));
	painter->save();
	painter->setPen(QPen(color));
	painter->drawLine(option.rect.x(), option.rect.top(), option.rect.x(), option.rect.bottom());
	painter->restore();
}

QWidget *PropTreeItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &index) const
{
	QWidget *edito = NULL;
	PropTreeItem* item = (PropTreeItem*)m_parent->indexToItem(index);
	if (item->m_type == PropTreeItem::IntSpinBox)
	{
		ScrSpinBox *editor = new ScrSpinBox(parent, item->m_unit);
		int val = index.model()->data(index, Qt::UserRole).toInt();
		editor->setValues(item->m_min, item->m_max, 0, val);
		edito = editor;
		edito->setAutoFillBackground(true);
		connect(editor, SIGNAL(valueChanged(double)), item, SIGNAL(valueChanged(double)));
		connect(editor, SIGNAL(valueChanged(double)), this, SLOT(valueHasChanged()));
	}
	else if (item->m_type == PropTreeItem::DoubleSpinBox)
	{
		ScrSpinBox *editor = new ScrSpinBox(parent, item->m_unit);
		double val = index.model()->data(index, Qt::UserRole).toDouble();
		editor->setValues(item->m_fmin, item->m_fmax, item->m_decimals, val);
		edito = editor;
		edito->setAutoFillBackground(true);
		connect(editor, SIGNAL(valueChanged(double)), item, SIGNAL(valueChanged(double)));
		connect(editor, SIGNAL(valueChanged(double)), this, SLOT(valueHasChanged()));
	}
	else if (item->m_type == PropTreeItem::ComboBox)
	{
		ScComboBox *editor = new ScComboBox(parent);
		editor->addItems(index.model()->data(index, Qt::UserRole+1).toStringList());
		edito = editor;
		edito->setAutoFillBackground(true);
		connect(editor, SIGNAL(activated(int)), item, SIGNAL(valueChanged(int)));
		connect(editor, SIGNAL(activated(QString)), item, SIGNAL(valueChanged(QString)));
		connect(editor, SIGNAL(activated(int)), this, SLOT(valueHasChanged()));
	}
	else if (item->m_type == PropTreeItem::CheckBox)
	{
		QCheckBox *editor = new QCheckBox(parent);
		edito = editor;
		edito->setAutoFillBackground(true);
		connect(editor, SIGNAL(clicked(bool)), item, SIGNAL(valueChanged(bool)));
		connect(editor, SIGNAL(clicked(bool)), this, SLOT(valueHasChanged()));
	}
	else if (item->m_type == PropTreeItem::ColorCombo)
	{
		ColorCombo *editor = new ColorCombo(parent);
		editor->updateBox(item->m_colors, ColorCombo::fancyPixmaps, false);
		edito = editor;
		edito->setAutoFillBackground(true);
		connect(editor, SIGNAL(activated(int)), item, SIGNAL(valueChanged(int)));
		connect(editor, SIGNAL(activated(QString)), item, SIGNAL(valueChanged(QString)));
		connect(editor, SIGNAL(activated(int)), this, SLOT(valueHasChanged()));
	}
	else
		edito = 0;
	m_edit = edito;
	if (item)
		emit item->editStarted();
	return edito;
}

void PropTreeItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	PropTreeItem* item = (PropTreeItem*)m_parent->indexToItem(index);
	if (item->m_type == PropTreeItem::IntSpinBox)
	{
		ScrSpinBox *spinBox = static_cast<ScrSpinBox*>(editor);
		spinBox->setValue(index.model()->data(index, Qt::UserRole).toInt());
	}
	else if (item->m_type == PropTreeItem::DoubleSpinBox)
	{
		ScrSpinBox *spinBox = static_cast<ScrSpinBox*>(editor);
		spinBox->setValue(index.model()->data(index, Qt::UserRole).toDouble());
	}
	else if (item->m_type == PropTreeItem::ComboBox)
	{
		ScComboBox *comboBox = static_cast<ScComboBox*>(editor);
		setCurrentComboItem(comboBox ,index.model()->data(index, Qt::UserRole).toString());
	}
	else if (item->m_type == PropTreeItem::CheckBox)
	{
		QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
		checkBox->setChecked(index.model()->data(index, Qt::UserRole).toBool());
	}
	else if (item->m_type == PropTreeItem::ColorCombo)
	{
		ColorCombo *comboBox = static_cast<ColorCombo*>(editor);
		setCurrentComboItem(comboBox ,index.model()->data(index, Qt::UserRole).toString());
	}
}

void PropTreeItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const
{
	PropTreeItem* item = (PropTreeItem*)m_parent->indexToItem(index);
	if (item->m_type == PropTreeItem::IntSpinBox)
	{
		ScrSpinBox *spinBox = static_cast<ScrSpinBox*>(editor);
		QString value = QString("%1 %2").arg(qRound(spinBox->value())).arg(unitGetSuffixFromIndex(item->m_unit));
		int val = qRound(spinBox->value());
		model->setData(index, val, Qt::UserRole);
		model->setData(index, value, Qt::DisplayRole);
	}
	else if (item->m_type == PropTreeItem::DoubleSpinBox)
	{
		ScrSpinBox *spinBox = static_cast<ScrSpinBox*>(editor);
		QString value = QString("%1 %2").arg(spinBox->value(), 0, 'f', item->m_decimals).arg(unitGetSuffixFromIndex(item->m_unit));
		double val = spinBox->value();
		model->setData(index, val, Qt::UserRole);
		model->setData(index, value, Qt::DisplayRole);
	}
	else if (item->m_type == PropTreeItem::ComboBox)
	{
		ScComboBox *comboBox = static_cast<ScComboBox*>(editor);
		QString value = comboBox->currentText();
		int val = comboBox->currentIndex();
		item->m_comboIndex = val;
		model->setData(index, value, Qt::UserRole);
		model->setData(index, value, Qt::DisplayRole);
	}
	else if (item->m_type == PropTreeItem::CheckBox)
	{
		QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
		bool value = checkBox->isChecked();
		model->setData(index, value, Qt::UserRole);
		model->setData(index, value, Qt::DisplayRole);
	}
	else if (item->m_type == PropTreeItem::ColorCombo)
	{
		ColorCombo *comboBox = static_cast<ColorCombo*>(editor);
		QString value = comboBox->currentText();
		int val = comboBox->currentIndex();
		item->m_comboIndex = val;
		model->setData(index, value, Qt::UserRole);
		model->setData(index, value, Qt::DisplayRole);
	}
}

void PropTreeItemDelegate::destroyEditor(QWidget * editor, const QModelIndex & index) const
{
	m_edit = NULL;
	PropTreeItem* item = (PropTreeItem*)m_parent->indexToItem(index);
	if (item)
		emit item->editFinished();
	QAbstractItemDelegate::destroyEditor(editor, index);
}

void PropTreeItemDelegate::valueHasChanged()
{
	emit commitData(m_edit);
}

void PropTreeItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}

QSize PropTreeItemDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
	QSize sz = QItemDelegate::sizeHint(opt, index) + QSize(2, 6);
	return sz;
}

PropTreeItem::PropTreeItem(QTreeWidget *parent, int typ, QString title) : QObject(parent), QTreeWidgetItem(parent)
{
	setText(0, title);
	m_type = typ;
	m_comboIndex = -1;
	m_unit = 0;
	m_decimals = 0;
	setData(0, Qt::UserRole, 0);
	setData(1, Qt::UserRole, 0);
	if (typ > 0)
		setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	else
		setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
}

PropTreeItem::PropTreeItem(PropTreeItem *parent, int typ, QString title) : QObject(parent), QTreeWidgetItem(parent)
{
	setText(0, title);
	m_type = typ;
	m_comboIndex = -1;
	m_unit = 0;
	m_decimals = 0;
	setData(0, Qt::UserRole, 0);
	setData(1, Qt::UserRole, 0);
	if (typ > 0)
		setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	else
		setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
}

int PropTreeItem::valueAsInt()
{
	int val = 0;
	val = data(1, Qt::UserRole).toInt();
	return val;
}

double PropTreeItem::valueAsDouble()
{
	return data(1, Qt::UserRole).toDouble();
}

bool PropTreeItem::valueAsBool()
{
	return data(1, Qt::UserRole).toBool();
}

QString PropTreeItem::valueAsString()
{
	return data(1, Qt::UserRole).toString();
}

void PropTreeItem::setIntValue(int value)
{
	if (m_type == ComboBox)
	{
		m_comboIndex = value;
		QStringList vals = data(1, Qt::UserRole + 1).toStringList();
		if (value < vals.count())
		{
			setData(1, Qt::UserRole, vals[value]);
			setData(1, Qt::DisplayRole, vals[value]);
		}
	}
	else
	{
		setData(1, Qt::UserRole, value);
		setData(1, Qt::DisplayRole, QString("%1 %2").arg(value).arg(unitGetSuffixFromIndex(m_unit)));
	}
}

void PropTreeItem::setDoubleValue(double value)
{
	setData(1, Qt::UserRole, value);
	setData(1, Qt::DisplayRole, QString("%1 %2").arg(value, 0, 'f', m_decimals).arg(unitGetSuffixFromIndex(m_unit)));
}

void PropTreeItem::setBoolValue(bool value)
{
	setData(1, Qt::UserRole, value);
	setData(1, Qt::DisplayRole, value);
}

void PropTreeItem::setStringValue(QString value)
{
	setData(1, Qt::UserRole, value);
	setData(1, Qt::DisplayRole, value);
}

void PropTreeItem::setUnitValue(int unit)
{
	m_unit = unit;
	if (m_type == DoubleSpinBox)
		setData(1, Qt::DisplayRole, QString("%1 %2").arg(data(1, Qt::UserRole).toDouble(), 0, 'f', m_decimals).arg(unitGetSuffixFromIndex(m_unit)));
	else
		setData(1, Qt::DisplayRole, QString("%1 %2").arg(data(1, Qt::UserRole).toString()).arg(unitGetSuffixFromIndex(m_unit)));
}

void PropTreeItem::setDecimalsValue(int unit)
{
	m_decimals = unit;
	if (m_type == DoubleSpinBox)
		setData(1, Qt::DisplayRole, QString("%1 %2").arg(data(1, Qt::UserRole).toDouble(), 0, 'f', unit).arg(unitGetSuffixFromIndex(m_unit)));
	else
		setData(1, Qt::DisplayRole, QString("%1 %2").arg(data(1, Qt::UserRole).toString()).arg(unitGetSuffixFromIndex(m_unit)));
}

void PropTreeItem::setComboStrings(QStringList value)
{
	setData(1, Qt::UserRole + 1, value);
}

void PropTreeItem::setMinMaxValues(int min, int max)
{
	m_min = min;
	m_max = max;
}

void PropTreeItem::setMinMaxValues(double min, double max)
{
	m_fmin = min;
	m_fmax = max;
}

void PropTreeItem::setColorList(ColorList colors)
{
	m_colors = colors;
}

PropTreeWidget::PropTreeWidget(QWidget* pa) : QTreeWidget(pa)
{
	setFocusPolicy(Qt::NoFocus);
	setColumnCount(2);
	delegate = new PropTreeItemDelegate(this);
	setItemDelegate(delegate);
	setRootIsDecorated(false);
	setIndentation(0);
	header()->hide();
	header()->setSectionResizeMode(QHeaderView::Stretch);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(handleMousePress(QTreeWidgetItem*)));
}

void PropTreeWidget::handleMousePress(QTreeWidgetItem *item)
{
	if (item == NULL)
		return;
	if (item->parent() == NULL)
	{
		if (!(item->flags() & Qt::ItemIsEditable))
			setItemExpanded(item, !isItemExpanded(item));
	}
}

void PropTreeWidget::mousePressEvent(QMouseEvent *event)
{
	QTreeWidgetItem *item = itemAt(event->pos());
	if (item)
	{
		if (item->flags() & Qt::ItemIsEditable)
		{
			if ((event->button() == Qt::LeftButton) && (header()->logicalIndexAt(event->pos().x()) == 1))
				QTreeWidget::mousePressEvent(event);
			else
				event->ignore();
		}
		else
			QTreeWidget::mousePressEvent(event);
	}
	else
		QTreeWidget::mousePressEvent(event);
}

PropTreeItem* PropTreeWidget::indexToItem(const QModelIndex &index)
{
	return (PropTreeItem*)itemFromIndex(index);
}

void PropTreeWidget::drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV3 opt = option;
	QTreeWidget::drawRow(painter, opt, index);
	QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &opt));
	painter->save();
	painter->setPen(QPen(color));
	painter->drawLine(opt.rect.x(), opt.rect.bottom(), opt.rect.right(), opt.rect.bottom());
	painter->restore();
}
