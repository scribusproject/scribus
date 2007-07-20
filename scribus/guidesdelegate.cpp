/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QDoubleSpinBox>
#include <QModelIndex>

#include "guidesdelegate.h"


GuidesDelegate::GuidesDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

QWidget * GuidesDelegate::createEditor(QWidget *parent,
									   const QStyleOptionViewItem &/* option */,
									   const QModelIndex &/* index */) const
{
	QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
	editor->setRange(0, 1000);
	return editor;
}

void GuidesDelegate::setEditorData(QWidget *editor,
								   const QModelIndex &index) const
{
	double value = index.model()->data(index, Qt::DisplayRole).toDouble();
	QDoubleSpinBox *w = static_cast<QDoubleSpinBox*>(editor);
	w->setValue(value);
}

void GuidesDelegate::setModelData(QWidget *editor,
								  QAbstractItemModel *model,
								  const QModelIndex &index) const
{
	QDoubleSpinBox *w = static_cast<QDoubleSpinBox*>(editor);
	w->interpretText();
	double value = w->value();
	model->setData(index, value);
}

void GuidesDelegate::updateEditorGeometry(QWidget *editor,
										  const QStyleOptionViewItem &option,
									      const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}
