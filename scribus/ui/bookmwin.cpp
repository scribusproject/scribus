/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          bookmwin.cpp  -  description
                             -------------------
    begin                : Mon Feb 11 2002
    copyright            : (C) 2002 by Franz Schmid
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
#include <QApplication>
#include <QHeaderView>

#include "bookmwin.h"


BookMItem::BookMItem(QTreeWidgetItem* parent, struct ScribusDoc::BookMa *Bm)
	: QTreeWidgetItem(parent)
{
	setup(Bm);
}

BookMItem::BookMItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, struct ScribusDoc::BookMa *Bm)
	: QTreeWidgetItem(parent, after)
{
	setup(Bm);
}

BookMItem::BookMItem(QTreeWidget* parent, QTreeWidgetItem* after, struct ScribusDoc::BookMa *Bm)
	: QTreeWidgetItem(parent, after)
{
	setup(Bm);
}

BookMItem::BookMItem(QTreeWidget* parent, struct ScribusDoc::BookMa *Bm)
	: QTreeWidgetItem(parent)
{
	setup(Bm);
}

BookMItem::BookMItem(QTreeWidget* parent, QTreeWidgetItem* after, int nr, PageItem* PObject)
	: QTreeWidgetItem(parent, after), PageObject(PObject), ItemNr(nr)
{

}

BookMItem::BookMItem(QTreeWidget* parent, int nr, PageItem* PObject)
	: QTreeWidgetItem(parent), PageObject(PObject), ItemNr(nr)
{

}

void BookMItem::setup(struct ScribusDoc::BookMa *Bm)
{
	ItemNr = Bm->ItemNr;
	Action = Bm->Aktion;
	First = Bm->First;
	Last = Bm->Last;
	Prev = Bm->Prev;
	Next = Bm->Next;
	Pare = Bm->Parent;
	Title = Bm->Title;
	PageObject = Bm->PageObject;
	setText(0, Bm->Text);
}

QString BookMItem::key(int, bool) const
{
	QString tmp = QString::asprintf("%5d", ItemNr);
	return tmp;
}

int BookMItem::level() const
{
	int ret = 0; // 0 = top level
	const QTreeWidgetItem * p = parent();
	while (p)
	{
		++ret;
		p = p->parent();
	}
	return ret;
}


BookmarkView::BookmarkView(QWidget* parent) : QTreeWidget(parent)
{
	setAcceptDrops(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setRootIsDecorated(true);
	header()->hide();

	connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectPageItem(QTreeWidgetItem *, int)));
}

void BookmarkView::addPageItem(PageItem* ite)
{
	QString bm;
	QString bm2;
	getTextAndTitle(ite, bm, bm2);
	addItem(bm, bm2, ite);
	Last = NrItems;
}

void BookmarkView::getTextAndTitle(PageItem* item, QString& text, QString& title) const
{
	QChar ch;
	QString str;

	for (int i = 0; i < item->itemText.length(); ++i)
	{
		ch = item->itemText.text(i);
		if (SpecialChars::isBreak(ch) || (ch == QChar(10)))
			break;
		str = QString(ch);
		if (item->itemText.hasExpansionPoint(i))
		{
			str = item->expand(item->itemText.expansionPoint(i));
			if (str.isEmpty())
				str = SpecialChars::ZWNBSPACE;
		}
		for (int j = 0; j < str.length(); ++j)
		{
			ch = str.at(j);
			if (ch == SpecialChars::OLD_NBSPACE || ch == SpecialChars::OBJECT)
				ch = ' ';
			if ((ch == '(') || (ch == ')') || (ch == '\\'))
				title += "\\";
			text  += ch;
			title += ch;
		}
	}
}

void BookmarkView::selectPageItem(QTreeWidgetItem * current, int /*column*/)
{
	auto* bookmark = dynamic_cast<BookMItem*>(current);
	// something weird here at 1st call...
	if (bookmark)
		emit selectElement(bookmark->PageObject, false);
}

void BookmarkView::dropEvent(QDropEvent *e)
{
	QTreeWidget::dropEvent(e);
//	QApplication::restoreOverrideCursor();
	rebuildTree();
}

// #include <QtDebug>
void BookmarkView::rebuildTree()
{
	Last = 0;

	// Reassign item ids first, some 1.3.4-1.3.7svn docs
	// have broken items IDs
	QTreeWidgetItemIterator it1(this, QTreeWidgetItemIterator::All);
	while (*it1)
	{
		auto* currItem = (BookMItem*) (*it1);
		currItem->ItemNr = ++Last;
		++it1;
	}

	int topLevelCount = this->topLevelItemCount();
	QTreeWidgetItemIterator it2(this, QTreeWidgetItemIterator::All);
	while (*it2)
	{
		auto* currItem = (BookMItem*) (*it2);
		// handle parents
		const auto* parent = (BookMItem*) currItem->parent();
		currItem->Pare = parent ? parent->ItemNr : 0;
		if (currItem->Pare == 0) // top level item
		{
			const BookMItem* prev = nullptr;
			const BookMItem* next = nullptr;
			int topLevelIndex = this->indexOfTopLevelItem(currItem);
			if (topLevelIndex >= 0)
			{
				prev = (topLevelIndex > 0) ? (BookMItem *) this->topLevelItem(topLevelIndex - 1) : nullptr;
				next = (topLevelIndex < (topLevelCount - 1)) ? (BookMItem *) this->topLevelItem(topLevelIndex + 1) : nullptr;
			}
			currItem->Prev = prev ? prev->ItemNr : 0;
			currItem->Next = next ? next->ItemNr : 0;
		}

		// handle children
		currItem->First = 0;
		currItem->Last  = 0;
		int childCount  = currItem->childCount();
		if (childCount > 0)
		{
			BookMItem* child = nullptr;
			const BookMItem* prev = nullptr;
			const BookMItem* next = nullptr;
			const auto* firstChild = (BookMItem*) currItem->child(0);
			const auto* lastChild = (BookMItem*) currItem->child(childCount - 1);
			currItem->First = firstChild->ItemNr;
			currItem->Last  = lastChild->ItemNr;
			for (int i = 0; i < childCount; ++i)
			{
				child = (BookMItem*) currItem->child(i);
				next  = (i < (childCount - 1)) ? (BookMItem*) currItem->child(i + 1) : nullptr;
				child->Prev = prev ? prev->ItemNr : 0;
				child->Next = next ? next->ItemNr : 0;
				prev = child;
			}
		}
		++it2;
	}

	NrItems = Last;

	emit markMoved();
	emit changed();
}

void BookmarkView::addItem(const QString& text, const QString& Tit, PageItem *PageObject)
{
	auto* ite = new BookMItem(this, NrItems + 1, PageObject);
	ite->setText(0, text);
	ite->Title = Tit;
	rebuildTree();
}

void BookmarkView::deleteItem(PageItem *pObject)
{
	QTreeWidgetItemIterator it(this);
	const BookMItem * item;
	while (*it)
	{
		item = (BookMItem*)(*it);
		if (item->PageObject == pObject)
		{
			delete (*it);
			break;
		}
		++it;
	}
	rebuildTree();
}

void BookmarkView::setAction(const PageItem *currItem, const QString& Act)
{
	BookMItem *ite;
	QTreeWidgetItemIterator it(this);
	while (*it)
	{
		ite = (BookMItem*)(*it);
		if (ite->PageObject == currItem)
		{
			ite->Action = Act;
			break;
		}
		++it;
	}
}

void BookmarkView::changeText(PageItem *currItem)
{
	BookMItem *ite;
	QString bm;
	QString bm2;

	getTextAndTitle(currItem, bm, bm2);

	QTreeWidgetItemIterator it(this);
	while (*it)
	{
		ite = (BookMItem*)(*it);
		if (ite->PageObject == currItem)
		{
			ite->setText(0, bm);
			ite->Title = bm2;
			break;
		}
		++it;
	}
	rebuildTree();
}
