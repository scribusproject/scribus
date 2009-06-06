/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QModelIndex>

#include "ui/scrspinbox.h"
#include "scribusdoc.h"
#include "guidesdelegate.h"
#include "units.h"


GuidesDelegate::GuidesDelegate(QObject *parent)
	: QItemDelegate(parent),
		m_doc(0)
{
}

// QWidget * GuidesDelegate::createEditor(QWidget *parent,
// 									   const QStyleOptionViewItem &/* option */,
// 									   const QModelIndex &/* index */) const
// {
// 	Q_ASSERT_X(m_doc != 0, "GuidesDelegate::createEditor",
// 			   "No reference to the doc");
// 	ScrSpinBox *editor = new ScrSpinBox(0, m_doc->currentPage()->height(),
// 										parent, m_doc->unitIndex());
// 	return editor;
// }

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
	// When user exit widget, editor value may not be commited at this point
	// so we have to get value from widget text
	double value = w->valueFromText(w->text());
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


// horizontals

GuidesHDelegate::GuidesHDelegate(QObject *parent)
	: GuidesDelegate(parent)
{
}

QWidget * GuidesHDelegate::createEditor(QWidget *parent,
									   const QStyleOptionViewItem &/* option */,
									   const QModelIndex &/* index */) const
{
	Q_ASSERT_X(m_doc != 0, "GuidesHDelegate::createEditor",
			   "No reference to the doc");
	double uix = unitGetRatioFromIndex(m_doc->unitIndex());
	double min = 0.0 - (m_doc->bleeds.Top * uix);
	double max = (m_doc->currentPage()->height() * uix)
	             + (m_doc->bleeds.Bottom * uix);
	ScrSpinBox *editor =
			new ScrSpinBox(min,
						   max,
						   parent, m_doc->unitIndex());
	return editor;
}

// verticals

GuidesVDelegate::GuidesVDelegate(QObject *parent)
	: GuidesDelegate(parent)
{
}

QWidget * GuidesVDelegate::createEditor(QWidget *parent,
									   const QStyleOptionViewItem &/* option */,
									   const QModelIndex &/* index */) const
{
	Q_ASSERT_X(m_doc != 0, "GuidesVDelegate::createEditor",
			   "No reference to the doc");
	double uix = unitGetRatioFromIndex(m_doc->unitIndex());
	double min = 0.0 - (m_doc->bleeds.Left * uix);
	double max = (m_doc->currentPage()->width() * uix)
	             + (m_doc->bleeds.Right * uix);
	ScrSpinBox *editor =
			new ScrSpinBox(min,
						   max,
						   parent, m_doc->unitIndex());
	return editor;
}
