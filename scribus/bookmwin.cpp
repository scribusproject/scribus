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
#include "util_icon.h"


BookMItem::BookMItem(QTreeWidgetItem* parent, struct ScribusDoc::BookMa *Bm)
	: QTreeWidgetItem(parent)
{
	SetUp(Bm);
}

BookMItem::BookMItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, struct ScribusDoc::BookMa *Bm)
	: QTreeWidgetItem(parent, after)
{
	SetUp(Bm);
}

BookMItem::BookMItem(QTreeWidget* parent, QTreeWidgetItem* after, struct ScribusDoc::BookMa *Bm)
	: QTreeWidgetItem(parent, after)
{
	SetUp(Bm);
}

BookMItem::BookMItem(QTreeWidget* parent, struct ScribusDoc::BookMa *Bm)
	: QTreeWidgetItem(parent)
{
	SetUp(Bm);
}

BookMItem::BookMItem(QTreeWidget* parent, QTreeWidgetItem* after, int nr, PageItem* PObject)
	: QTreeWidgetItem(parent, after)
{
	ItemNr = nr;
	PageObject = PObject;
	PdfObj = 0;
	Action = "";
	First = 0;
	Last = 0;
	Prev = 0;
	Next = 0;
	Pare = 0;
}

BookMItem::BookMItem(QTreeWidget* parent, int nr, PageItem* PObject)
	: QTreeWidgetItem(parent)
{
	ItemNr = nr;
	PageObject = PObject;
	PdfObj = 0;
	Action = "";
	First = 0;
	Last = 0;
	Prev = 0;
	Next = 0;
	Pare = 0;
}

void BookMItem::SetUp(struct ScribusDoc::BookMa *Bm)
{
	ItemNr = Bm->ItemNr;
	PdfObj = 0;
	Action = Bm->Aktion;
	First = Bm->First;
	Last = Bm->Last;
	Prev = Bm->Prev;
	Next = Bm->Next;
	Pare = Bm->Parent;
	Titel = Bm->Title;
	PageObject = Bm->PageObject;
	setText(0, Bm->Text);
}

QString BookMItem::key(int, bool) const
{
	QString tmp;
	tmp.sprintf("%5d", ItemNr);
	return tmp;
}

int BookMItem::level()
{
	int ret = 0; // 0 = top level
	QTreeWidgetItem * p = parent();
	while (p)
	{
		++ret;
		p = p->parent();
	}
	return ret;
}


BookMView::BookMView(QWidget* parent) : QTreeWidget(parent)
{
	NrItems = 0;
	First = 1;
	Last = 0;

	setAcceptDrops(true);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setRootIsDecorated(true);
	header()->hide();

	connect(this, SIGNAL(currentItemChanged( QTreeWidgetItem *, QTreeWidgetItem *)),
			this, SLOT(setPageItem(QTreeWidgetItem *, QTreeWidgetItem *)));
}

void BookMView::AddPageItem(PageItem* ite)
{
	QString bm = "";
	QString bm2 = "";
	QString cc;
	for (int d = 0; d < ite->itemText.length(); ++d)
	{
		cc = ite->itemText.text(d);
		if ((cc == QChar(13)) || (cc == QChar(10)))
			break;
		if (cc == QChar(29))
			cc = " ";
		if ((cc == "(") || (cc == ")") || (cc == "\\"))
			bm2 += "\\";
		bm += cc;
		bm2 += cc;
	}
	AddItem(bm, bm2, ite);
	Last = NrItems;
}

void BookMView::setPageItem(QTreeWidgetItem * /*old*/, QTreeWidgetItem * newItem)
{
	BookMItem * i = dynamic_cast<BookMItem*>(newItem);
	// something weird here at 1st call...
	if (i)
		emit SelectElement(i->PageObject);
}

void BookMView::dropEvent(QDropEvent *e)
{
	QTreeWidget::dropEvent(e);
//	QApplication::restoreOverrideCursor();
	rebuildTree();
}

// #include <QtDebug>
void BookMView::rebuildTree()
{
	QTreeWidgetItemIterator it(this, QTreeWidgetItemIterator::All);
	// level - parent : children
	QMap<int,QMap<int,QList<BookMItem*> > > levels;
	QMap<int,BookMItem*> items;

	Last = 0;

	while (*it)
	{
		BookMItem * currItem = (BookMItem*)(*it);
		// handle parents
		BookMItem * parent = (BookMItem*)currItem->parent();
		currItem->Pare = parent ? parent->ItemNr : 0;

		levels[currItem->level()][currItem->Pare].append(currItem);
		items[Last] = currItem;
		
		currItem->ItemNr = Last;
		++Last;
		++it;
	}

	NrItems = Last;

	foreach (int level, levels.keys())
	{
// 		qDebug() << "LEVEL: " << level;
		QMapIterator<int,QList<BookMItem*> > parent(levels[level]);
		while (parent.hasNext())
		{
			parent.next();
// 			qDebug() << "PARENT: " << parent.key();
			int cnt = parent.value().count();
			items[parent.key()]->First = cnt > 0 ? parent.value().at(0)->ItemNr : 0;
			items[parent.key()]->Last = cnt > 0 ? parent.value().at(cnt-1)->ItemNr : 0;
			int prev = parent.key();
			int ix = 0;
			foreach (BookMItem* item, parent.value())
			{
// 				qDebug() << "CHILD: " << item->ItemNr;
				item->Prev = prev;
				prev = item->ItemNr;
				if (ix < (cnt - 1))
					item->Next = parent.value()[ix+1]->ItemNr;
				++ix;
			}
		}
	}
/*
	foreach (BookMItem* i, items.values())
	{
		qDebug() << i->ItemNr << " " << i->text(0).left(10) << " " << i->Prev << " " << i->Next << " " << i->Pare << " " << i->First << " " << i->Last;
	}*/

	emit MarkMoved();
	emit changed();
}

void BookMView::AddItem(QString text, QString Tit, PageItem *PageObject)
{
	BookMItem * ite = new BookMItem(this, NrItems+1, PageObject);
	ite->setText(0, text);
	ite->Titel = Tit;
	rebuildTree();
}

void BookMView::DeleteItem(PageItem *pObject)
{
	QTreeWidgetItemIterator it(this);
	BookMItem * item;
	while (*it)
	{
		item = (BookMItem*)(*it);
		if (item->PageObject == pObject)
		{
			delete (*it);
			break;
		}
	}
	rebuildTree();
}

void BookMView::SetAction(PageItem *currItem, QString Act)
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

void BookMView::ChangeText(PageItem *currItem)
{
	BookMItem *ite;
	QString bm = "";
	QString bm2 = "";
	QString cc;
	for (int d = 0; d < currItem->itemText.length(); ++d)
	{
		cc = currItem->itemText.text(d);
		if ((cc == QChar(13)) || (cc == QChar(10)))
			break;
		if ((cc == "(") || (cc == ")") || (cc == "\\"))
			bm2 += "\\";
		bm += cc;
		bm2 += cc;
	}
	QTreeWidgetItemIterator it(this);
	while (*it)
	{
		ite = (BookMItem*)(*it);
		if (ite->PageObject == currItem)
		{
			ite->setText(0, bm);
			ite->Titel = bm2;
			break;
		}
		++it;
	}
	rebuildTree();
}
