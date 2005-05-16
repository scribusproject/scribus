/***************************************************************************
                          bookmwin.h  -  description
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

#ifndef BOOKMWIN_H
#define BOOKMWIN_H

#include <qdialog.h>
#include <qlistview.h>
#include <qevent.h>
#include <qpoint.h>
#include <qlayout.h>
#include "scribusview.h"
#include "scribusdoc.h"
#include "pageitem.h"

/**
*@author Franz Schmid
*/

class BookMItem : public QListViewItem
{
public:
	BookMItem(QListViewItem* parent, struct ScribusDoc::BookMa *Bm);
	BookMItem(QListViewItem* parent, QListViewItem* after, struct ScribusDoc::BookMa *Bm);
	BookMItem(QListView* parent, QListViewItem* after, struct ScribusDoc::BookMa *Bm);
	BookMItem(QListView* parent, struct ScribusDoc::BookMa *Bm);
	BookMItem(QListView* parent, QListViewItem* after, int nr, int s, int el);
	BookMItem(QListView* parent, int nr, int s, int el);
	~BookMItem() {};
	void SetUp(struct ScribusDoc::BookMa *Bm);
	virtual QString key(int, bool) const;
	int ItemNr;
	int Seite;
	int Element;
	int PdfObj;
	QString Action;
	QString Titel;
	int First;
	int Last;
	int Prev;
	int Next;
	int Pare;
};

class BookMView : public QListView
{
	Q_OBJECT

public:
	BookMView(QWidget* parent);
	~BookMView() {};
	void AddItem(QString text, QString Tit, int s, int el);
	void DeleteItem(int nr);
	void SetAction(int nr, QString Act);
	int NrItems;
	bool Mpressed;
	QPoint Mpos;
	BookMItem *DraggedI;
	int First;
	int Last;

public slots:
	void AddPageItem(PageItem* ite);
	void ChangeItem(int nr, int itnr);
	void ChangeText(PageItem *currItem);
	void languageChange();

signals:
	void MarkMoved();
	void ChangeBMNr(int, int, int);
	void SelectElement(int, int);

protected:
	void contentsMouseReleaseEvent(QMouseEvent *m);
	void contentsMousePressEvent(QMouseEvent* e);
	void contentsMouseMoveEvent(QMouseEvent* e);
	void contentsDropEvent(QDropEvent *e);
	void contentsDragMoveEvent(QDragMoveEvent *e);
	
private:
	int idBookMarkCol;
};

#endif
