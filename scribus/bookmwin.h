/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <Q3ListView>
#include <QDialog>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QLayout>
#include <QMouseEvent>
#include <QPoint>

#include "pageitem.h"
#include "scribusapi.h"
#include "scribusdoc.h"

/**
*@author Franz Schmid
*/

class SCRIBUS_API BookMItem : public Q3ListViewItem
{
public:
	BookMItem(Q3ListViewItem* parent, struct ScribusDoc::BookMa *Bm);
	BookMItem(Q3ListViewItem* parent, Q3ListViewItem* after, struct ScribusDoc::BookMa *Bm);
	BookMItem(Q3ListView* parent, Q3ListViewItem* after, struct ScribusDoc::BookMa *Bm);
	BookMItem(Q3ListView* parent, struct ScribusDoc::BookMa *Bm);
	BookMItem(Q3ListView* parent, Q3ListViewItem* after, int nr, PageItem *PObject);
	BookMItem(Q3ListView* parent, int nr, PageItem *PObject);
	~BookMItem() {};
	void SetUp(struct ScribusDoc::BookMa *Bm);
	virtual QString key(int, bool) const;
	PageItem *PageObject;
	int ItemNr;
	int PdfObj;
	QString Action;
	QString Titel;
	int First;
	int Last;
	int Prev;
	int Next;
	int Pare;
};

class SCRIBUS_API BookMView : public Q3ListView
{
	Q_OBJECT

public:
	BookMView(QWidget* parent);
	~BookMView() {};
	void AddItem(QString text, QString Tit, PageItem *PageObject);
	void DeleteItem(PageItem *PageObject);
	void SetAction(PageItem *currItem, QString Act);
	int NrItems;
	bool Mpressed;
	QPoint Mpos;
	BookMItem *DraggedI;
	int First;
	int Last;

public slots:
	void AddPageItem(PageItem* ite);
	void ChangeText(PageItem *currItem);
	void languageChange();

signals:
	void MarkMoved();
	void SelectElement(PageItem *);
	void changed();

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
