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

#include "bookmwin.h"
#include "bookmwin.moc"
#include <qpixmap.h>
#include <qdragobject.h>
#include <qpopupmenu.h>
#include <qcursor.h>
extern QPixmap loadIcon(QString nam);

BookMItem::BookMItem(QListViewItem* parent, struct ScribusDoc::BookMa *Bm) : QListViewItem(parent)
{
	SetUp(Bm);
}

BookMItem::BookMItem(QListViewItem* parent, QListViewItem* after, struct ScribusDoc::BookMa *Bm) : QListViewItem(parent, after)
{
	SetUp(Bm);
}

BookMItem::BookMItem(QListView* parent, QListViewItem* after, struct ScribusDoc::BookMa *Bm) : QListViewItem(parent, after)
{
	SetUp(Bm);
}

BookMItem::BookMItem(QListView* parent, struct ScribusDoc::BookMa *Bm) : QListViewItem(parent)
{
	SetUp(Bm);
}

BookMItem::BookMItem(QListView* parent, QListViewItem* after, int nr, int s, int el) : QListViewItem(parent, after)
{
	ItemNr = nr;
	Seite = s;
	Element = el;
	PdfObj = 0;
	Action = "";
	First = 0;
	Last = 0;
	Prev = 0;
	Next = 0;
	Pare = 0;
}

BookMItem::BookMItem(QListView* parent, int nr, int s, int el) : QListViewItem(parent)
{
	ItemNr = nr;
	Seite = s;
	Element = el;
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
	Seite = Bm->Seite;
	Element = Bm->Element;
	PdfObj = 0;
	Action = Bm->Aktion;
	First = Bm->First;
	Last = Bm->Last;
	Prev = Bm->Prev;
	Next = Bm->Next;
	Pare = Bm->Parent;
	Titel = Bm->Title;
	setText(0, Bm->Text);
}

QString BookMItem::key(int, bool) const
{
	QString tmp;
	tmp.sprintf("%5d", ItemNr);
	return tmp;
}


BookMView::BookMView(QWidget* parent) : QListView(parent)
{
	NrItems = 0;
	Mpressed = false;
	DraggedI = 0;
	First = 1;
	Last = 0;
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setRootIsDecorated(true);
	addColumn(tr("Bookmarks"));
	setResizeMode(QListView::AllColumns);
	setSelectionMode(QListView::NoSelection);
	setSorting(-1,1);
}

void BookMView::AddPageItem(PageItem* ite)
{
	QString bm = "";
	QString bm2 = "";
	QString cc;
	for (uint d = 0; d < ite->Ptext.count(); ++d)
		{
		cc = ite->Ptext.at(d)->ch;
		if ((cc == QChar(13)) || (cc == QChar(10)))
			break;
		if (cc == QChar(29))
			cc = " ";
		if ((cc == "(") || (cc == ")") || (cc == "\\"))
			bm2 += "\\";
			bm += cc;
			bm2 += cc;
		}
	AddItem(bm, bm2, ite->OwnPage->PageNr, ite->ItemNr);
	ite->BMnr = NrItems;
	Last = NrItems;
}

void BookMView::contentsMouseReleaseEvent(QMouseEvent *)
{
	Mpressed = false;
}

void BookMView::contentsMousePressEvent(QMouseEvent* e)
{
	QListView::contentsMousePressEvent(e);
	QListViewItem *i = itemAt(contentsToViewport(e->pos()));
	if (i)
		{
		Mpos = e->pos();
		Mpressed = true;
		}
}

void BookMView::contentsMouseMoveEvent(QMouseEvent* e)
{
	if ((Mpressed) && ((Mpos - e->pos()).manhattanLength() > 4))
		{
		Mpressed = false;
		QListViewItem *i = itemAt(contentsToViewport(Mpos));
		if (i)
			{
			DraggedI = (BookMItem*)i;
			QDragObject *dr = new QTextDrag(i->text(0), this, "BMD");
			dr->drag();
			}
		}
}

void BookMView::contentsDropEvent(QDropEvent *e)
{
	QString text;
	int ins, mov, mret;
	QListViewItem *pp;
	QListViewItem *lv;
	BookMItem *ip;
	BookMItem *ip2;
	BookMItem *ite;
	if (!QTextDrag::decode(e, text))
		{
		e->ignore();
		return;
    }
	QListViewItem *item = itemAt(contentsToViewport(e->pos()));
	BookMItem *Bite = (BookMItem*)item;
  if ((item) && (DraggedI != 0))
  	{
  	if ((BookMItem*)item != DraggedI)
  		{
  		pp = item->parent();
  		while (pp)
  			{
  			if (pp == DraggedI)
  				return;
  			pp = pp->parent();
  			}
			QPopupMenu *pmenu = new QPopupMenu();
			mov = pmenu->insertItem(tr("Move Bookmark"));
			ins = pmenu->insertItem(tr("Insert Bookmark"));
			pmenu->insertItem(tr("Cancel"));
			mret = pmenu->exec(QCursor::pos());
			if (mret == ins)
				{
				lv = DraggedI->parent();
				lv = lv ? lv->firstChild() : firstChild();
				if (!DraggedI->Prev == 0)
					{
					while (lv)
						{
						ip = (BookMItem*)lv;
						if (ip->ItemNr == DraggedI->Prev)
							break;
						lv = lv->nextSibling();
						}
					ip = (BookMItem*)lv;
					lv = DraggedI->nextSibling();
					if (lv)
						{
						ip2 = (BookMItem*)lv;
						ip2->Prev = ip->ItemNr;
						ip->Next = ip2->ItemNr;
						}
					else
						ip->Next = 0;
					}
				lv = DraggedI->parent();
  			item->insertItem(DraggedI);
				lv ? lv = lv->firstChild() : lv = firstChild();
				if (DraggedI->Prev == 0)
					{
					ip = (BookMItem*)lv;
					ip->Prev = 0;
					}
				QListViewItemIterator it(this);
				for ( ; it.current(); ++it)
					{
					ite = (BookMItem*)it.current();
					if (ite->ItemNr == DraggedI->ItemNr)
						DraggedI = ite;
					}
  			DraggedI->Pare = Bite->ItemNr;
  			ip = (BookMItem*)item->firstChild();
  			Bite->First = ip->ItemNr;
  			lv = item->firstChild();
  			while (lv)
  				{
  				if (!lv->nextSibling())
  					{
  					ip = (BookMItem*)lv;
  					Bite->Last = ip->ItemNr;
  					break;
  					}
  				lv = lv->nextSibling();
  				}
  			if (DraggedI->nextSibling())
  				{
  				ip = (BookMItem*)DraggedI->nextSibling();
  				DraggedI->Next = ip->ItemNr;
  				ip->Prev = DraggedI->ItemNr;
  				}
  			else
  				DraggedI->Next = 0;
  			if (DraggedI != item->firstChild())
  				{
  				ip = (BookMItem*)DraggedI->itemAbove();
  				DraggedI->Prev = ip->ItemNr;
  				ip->Next = DraggedI->ItemNr;
  				}
  			else
  				DraggedI->Prev = 0;
  			}
			if (mret == mov)
				{
				lv = DraggedI->parent();
				lv ? lv = lv->firstChild() : lv = firstChild();
				if (DraggedI->Prev == 0)
					{
					ip = (BookMItem*)lv->nextSibling();
					ip->Prev = 0;
					}
				else
					{
					while (lv)
						{
						ip = (BookMItem*)lv;
						if (ip->ItemNr == DraggedI->Prev)
							break;
						lv = lv->nextSibling();
						}
					}
				ip = (BookMItem*)lv;
				lv = DraggedI->nextSibling();
				if (lv)
					{
					ip2 = (BookMItem*)lv;
					ip2->Prev = ip->ItemNr;
					ip->Next = ip2->ItemNr;
					}
				else
					ip->Next = 0;
				lv = DraggedI->parent();
  			DraggedI->moveItem(item);
				lv ? lv = lv->firstChild() : lv = firstChild();
				if (DraggedI->Prev == 0)
					{
					ip = (BookMItem*)lv;
					ip->Prev = 0;
					}
				QListViewItemIterator it2(this);
				for ( ; it2.current(); ++it2)
					{
					ite = (BookMItem*)it2.current();
					if (ite->ItemNr == DraggedI->ItemNr)
						DraggedI = ite;
					}
  			ip = (BookMItem*)item;
  			DraggedI->Pare = ip->Pare;
  			DraggedI->Prev = ip->ItemNr;
  			ip->Next = DraggedI->ItemNr;
  			if (DraggedI->nextSibling())
  				{
  				ip = (BookMItem*)DraggedI->nextSibling();
  				DraggedI->Next = ip->ItemNr;
  				ip->Prev = DraggedI->ItemNr;
  				}
  			else
  				DraggedI->Next = 0;  	
  			lv = item->parent();
				if (lv)
					{
					ip = (BookMItem*)lv->firstChild();
					ip2 = (BookMItem*)lv;
					ip2->First = ip->ItemNr;
  				lv = lv->firstChild();
  				while (lv)
  					{
  					if (!lv->nextSibling())
  						{
  						ip = (BookMItem*)lv;
  						ip2->Last = ip->ItemNr;
  						break;
  						}
  					lv = lv->nextSibling();
  					}
					}		
  			}
			delete pmenu;	
  		DraggedI = 0;
			emit MarkMoved();
  		}
  	}
}

void BookMView::contentsDragMoveEvent(QDragMoveEvent *e)
{
	QString text;
	if (!QTextDrag::decode(e, text))
		{
		e->ignore();
		return;
    }
	QListViewItem *item = itemAt(contentsToViewport(e->pos()));
  if (item)
  	{
		setSelected(item, true);
		e->accept();
  	}
  else
		e->ignore();
}

void BookMView::AddItem(QString text, QString Tit, int s, int el)
{
  QListViewItem *lv = firstChild();
  while (lv)
  	{
  	if (!lv->nextSibling())
  		break;
  	lv = lv->nextSibling();
  	}
  BookMItem *ip;
	BookMItem *ite;
	if (lv)
		ite = new BookMItem(this, lv, NrItems+1, s, el);
	else
		ite = new BookMItem(this, NrItems+1, s, el);
	ite->setText(0, text);
	ite->Titel = Tit;
	ite->Next = 0;
  if (lv)
  	{
  	ip = (BookMItem*)lv;
  	ip->Next = ite->ItemNr;
  	ite->Prev = ip->ItemNr;
  	}
	NrItems++;
}

void BookMView::DeleteItem(int nr)
{
	BookMItem *ite;
	BookMItem *ite2 = 0;
	BookMItem *ite3;
	BookMItem *ite4;
	QListViewItemIterator it(this);
	for ( ; it.current(); ++it)
		{
		ite = (BookMItem*)it.current();
		if (ite->Next == nr)
			ite2 = ite;
		if (ite->ItemNr == nr)
			{
			QListViewItem *pp = ite->firstChild();
			QListViewItem *ppn = ite->nextSibling();
			if ((ite->parent()) && (ite2 == 0) && (ppn))
				{
				ite3 = (BookMItem*)ite->parent();
				ite4 = (BookMItem*)ppn;
				ite3->First = ite4->ItemNr;
				}
			while (pp)
				{
      	ite->parent() ? ite->parent()->insertItem(pp) :	pp->moveItem(ite);
				pp = ite->firstChild();
				}
			if (ppn)
				{
				ite3 = (BookMItem*)ppn;
				if (ite2)
					{
					ite2->Next = ite3->ItemNr;
					ite3->Prev = ite2->ItemNr;
					}
				else
					ite3->Prev = 0;
				}
			else
				if (ite2)
					ite2->Next = 0;
			delete ite;
			}
		}
	QMap<int,int> Tabl;
	Tabl.clear();
	Tabl[0] = 0;
	int Counter = 1;
	QListViewItemIterator itn(this);
	for ( ; itn.current(); ++itn)
		{
		ite = (BookMItem*)itn.current();
		Tabl[ite->ItemNr] = Counter;
		Counter++;
		}
	QListViewItemIterator itnf(this);
	for ( ; itnf.current(); ++itnf)
		{
		ite = (BookMItem*)itnf.current();
		ite->ItemNr = Tabl[ite->ItemNr];
		ite->Pare = Tabl[ite->Pare];
		ite->Next = Tabl[ite->Next];
		ite->Prev = Tabl[ite->Prev];
		ite->First = Tabl[ite->First];
		ite->Last = Tabl[ite->Last];
		emit ChangeBMNr(ite->Seite, ite->Element, ite->ItemNr);
		}
}

void BookMView::ChangeItem(int nr, int itnr)
{
	BookMItem *ite;
	QListViewItemIterator it(this);
	for ( ; it.current(); ++it)
		{
		ite = (BookMItem*)it.current();
		if (ite->ItemNr == nr)
			{
			ite->Element = itnr;
			break;
			}
		}
}

void BookMView::SetAction(int nr, QString Act)
{
	BookMItem *ite;
	QListViewItemIterator it(this);
	for ( ; it.current(); ++it)
		{
		ite = (BookMItem*)it.current();
		if (ite->ItemNr == nr)
			{
			ite->Action = Act;
			break;
			}
		}
}

void BookMView::ChangeText(PageItem *b)
{
	BookMItem *ite;
	QString bm = "";
	QString bm2 = "";
	QString cc;
	int nr = b->BMnr;
	for (uint d = 0; d < b->Ptext.count(); ++d)
		{
		cc = b->Ptext.at(d)->ch;
		if ((cc == QChar(13)) || (cc == QChar(10)))
			break;
		if ((cc == "(") || (cc == ")") || (cc == "\\"))
			bm2 += "\\";
			bm += cc;
			bm2 += cc;
		}
	QListViewItemIterator it(this);
	for ( ; it.current(); ++it)
		{
		ite = (BookMItem*)it.current();
		if (ite->ItemNr == nr)
			{
			ite->setText(0, bm);
			ite->Titel = bm2;
			break;
			}
		}
}

