/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QModelIndex>

#include "scrspinbox.h"
#include "scribusdoc.h"
#include "guidesdelegate.h"


GuidesDelegate::GuidesDelegate(QObject *parent)
	: QItemDelegate(parent),
		m_doc(0)
{
}

QWidget * GuidesDelegate::createEditor(QWidget *parent,
									   const QStyleOptionViewItem &/* option */,
									   const QModelIndex &/* index */) const
{
	ScrSpinBox *editor = new ScrSpinBox(0, 1000, parent, m_doc?m_doc->unitIndex():0);
	return editor;
}

void GuidesDelegate::setEditorData(QWidget *editor,
								   const QModelIndex &index) const
{
	double value = index.model()->data(index, Qt::DisplayRole).toDouble();
	ScrSpinBox *w = static_cast<ScrSpinBox*>(editor);
	w->setValue(value);
}

void GuidesDelegate::setModelData(QWidget *editor,
								  QAbstractItemModel *model,
								  const QModelIndex &index) const
{
	ScrSpinBox *w = static_cast<ScrSpinBox*>(editor);
	double value = w->value();
	model->setData(index, value);
}

void GuidesDelegate::updateEditorGeometry(QWidget *editor,
										  const QStyleOptionViewItem &option,
									      const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}

void GuidesDelegate::setDoc(ScribusDoc * doc)
{
	m_doc = doc;
}
