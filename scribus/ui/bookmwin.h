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

#include <QTreeWidget>

class PageItem;
#include "scribusapi.h"
#include "scribusdoc.h"

/*! \brief Tree widget item for PDF Bookmarks.
Secret items like Pare, First, Last etc. are PDF Outlines
attributes. See PDF specification for more info.
These attrs are recomputed in the BookmarkView::rebuildTree()
method.
@author Franz Schmid
*/
class SCRIBUS_API BookMItem : public QTreeWidgetItem
{
public:
	BookMItem(QTreeWidgetItem* parent, struct ScribusDoc::BookMa *Bm);
	BookMItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, struct ScribusDoc::BookMa *Bm);
	BookMItem(QTreeWidget* parent, QTreeWidgetItem* after, struct ScribusDoc::BookMa *Bm);
	BookMItem(QTreeWidget* parent, struct ScribusDoc::BookMa *Bm);
	BookMItem(QTreeWidget* parent, QTreeWidgetItem* after, int nr, PageItem *PObject);
	BookMItem(QTreeWidget* parent, int nr, PageItem *PObject);
	~BookMItem() {};

	int level();
	void setup(struct ScribusDoc::BookMa *Bm);
	virtual QString key(int, bool) const;

	PageItem *PageObject;
	int ItemNr;
	QString Action;
	QString Title;
	int First;
	int Last;
	int Prev;
	int Next;
	int Pare;
};


/*! \brief Tree widget for PDF Bookmarks.
It's a minimal tree implementation with D'n'D handled
in Qt4 itself. It could be rewritten into M/V/C stuff
later, but it looks it's simpler and quicker now.
MarkMoved() signal stores bookmarks in the ScribusDoc.
@author Franz Schmid
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API BookmarkView : public QTreeWidget
{
	Q_OBJECT

public:
	BookmarkView(QWidget* parent);
	~BookmarkView() {};

	void addItem(const QString& text, const QString& Tit, PageItem *PageObject);
	void deleteItem(PageItem *PageObject);
	void setAction(PageItem *currItem, const QString& Act);

	int NrItems;
	int First;
	int Last;

	/*! \brief Build scribus bookmark structure from tree items.
	It looks horrible as it performs full rescan of the tree, but
	it's pretty fast at all.
	\author Petr vanek <petr@scribus.info>
	*/
	void rebuildTree();

public slots:
	void addPageItem(PageItem* ite);
	void changeText(PageItem *currItem);

signals:
	void changed();
	void markMoved();
	void selectElement(PageItem *, bool);

protected:
	void dropEvent(QDropEvent *e);
	void getTextAndTitle(PageItem* item, QString& text, QString& title);

private slots:
	void selectPageItem(QTreeWidgetItem * current, int column);
};

#endif
