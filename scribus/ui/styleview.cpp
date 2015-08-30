/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QPainter>

#include "styleview.h"


StyleView::StyleView(QWidget *parent) : QTreeWidget(parent)
{
}

// StyleView::~StyleView()
// {
// }

void StyleView::drawRow(QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	if (index.column() == 0)
	{
		QFont f(painter->font());
		f.setBold(static_cast<StyleViewItem*>(itemFromIndex(index))->isDirty());
// 		p->setFont(f);
		QStyleOptionViewItem newopt(option);
		newopt.font = f;
		QTreeView::drawRow(painter, newopt, index);
		return;
	}
	QTreeView::drawRow(painter, option, index);
}

// void StyleView::contentsMousePressEvent(QMouseEvent *e)
// {
// 	if (e->button() == t::RightButton)
// 		emit rightButtonClicked(itemAt(e->pos()), e->globalPos(), 0);
// 	else
// 		Q3ListView::contentsMousePressEvent(e);
// }

// void StyleView::contentsMouseDoubleClickEvent(QMouseEvent *e)
// {
// 	if (e->button() == Qt::LeftButton)
// 		emit doubleClicked(itemAt(e->pos()), e->globalPos(), 0);
// 	else
// 		Q3ListView::contentsMouseDoubleClickEvent(e);
// }

/*** StyleViewItem *******************************************************************/

StyleViewItem::StyleViewItem(QTreeWidget *parent, const QString &text)
	: QTreeWidgetItem(parent),
	m_isRoot(true), m_isDirty(false),
	m_parentName(QString::null), m_rootName(QString::null)
{
	setFlags(flags() & ~Qt::ItemIsSelectable);
	setText(0, text);
}

StyleViewItem::StyleViewItem(QTreeWidgetItem *parent, const QString &text, const QString &rootName)
	: QTreeWidgetItem(parent),
	m_isRoot(false), m_isDirty(false),
	m_parentName(parent->text(0)), m_rootName(rootName)
{
	setText(0, text);
}

// void StyleViewItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align)
// {
// 	if (column == 0)
// 	{
// 		QFont f(p->font());
// 		f.setBold(isDirty_);
// 		p->setFont(f);
// 	}
// 
// 	Q3ListViewItem::paintCell(p, cg, column, width, align);
// }

void StyleViewItem::setDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

bool StyleViewItem::isDirty()
{
	return m_isDirty;
}

bool StyleViewItem::isRoot()
{
	return m_isRoot;
}

QString StyleViewItem::parentName()
{
	return m_parentName;
}

QString StyleViewItem::rootName()
{
	return m_rootName;
}

// StyleViewItem::~StyleViewItem()
// {
// 
// }
