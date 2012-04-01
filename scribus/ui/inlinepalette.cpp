/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          inlinepalette.cpp  -  description
                             -------------------
    begin                : Tue Mar 27 2012
    copyright            : (C) 2012 by Franz Schmid
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
#include "inlinepalette.h"
#include <QPainter>
#include <QByteArray>
#include "pageitem.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "selection.h"

InlineView::InlineView(QWidget* parent) : QListWidget(parent)
{
	setDragEnabled(true);
	setViewMode(QListView::IconMode);
	setFlow(QListView::LeftToRight);
	setSortingEnabled(true);
	setWrapping(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setResizeMode(QListView::Adjust);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	delegate = new ScListWidgetDelegate(this, this);
	delegate->setIconOnly(true);
	setItemDelegate(delegate);
	setIconSize(QSize(50, 50));
}

void InlineView::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void InlineView::dragMoveEvent(QDragMoveEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void InlineView::dropEvent(QDropEvent *e)
{
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		if (e->source() == this)
			return;
	}
	else
		e->ignore();
}

 void InlineView::startDrag(Qt::DropActions supportedActions)
 {
	QMimeData *mimeData = new QMimeData;
	QByteArray data = currentItem()->text().toLocal8Bit();
	mimeData->setData("text/inline", data);
	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(currentItem()->icon().pixmap(48, 48));
	drag->exec(Qt::CopyAction);
	clearSelection();
}

InlinePalette::InlinePalette( QWidget* parent) : ScDockPalette( parent, "Inline", 0)
{
	setMinimumSize( QSize( 220, 240 ) );
	setObjectName(QString::fromLocal8Bit("Inline"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	InlineViewWidget = new InlineView(this);
	InlineViewWidget->clear();
	setWidget( InlineViewWidget );

	unsetDoc();
	m_scMW  = NULL;
	languageChange();
	connect(InlineViewWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	connect(InlineViewWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(handleContextMenue(QPoint)));
}

void InlinePalette::handleContextMenue(QPoint p)
{
	QListWidgetItem *item = InlineViewWidget->itemAt(p);
	if (item)
	{
		actItem = item->data(Qt::UserRole).toInt();
		bool txFrame = false;
		if (!currDoc->m_Selection->isEmpty())
		{
			PageItem* selItem = currDoc->m_Selection->itemAt(0);
			if ((selItem->isTextFrame() || selItem->isTable()))
				txFrame = true;
		}
		QMenu *pmenu = new QMenu();
		if (txFrame)
		{
			QAction* pasteAct = pmenu->addAction( tr("Paste to Item"));
			connect(pasteAct, SIGNAL(triggered()), this, SLOT(handlePasteToItem()));
		}
		if ((currDoc->appMode != modeEdit) && (currDoc->appMode != modeEditTable))
		{
			QAction* editAct = pmenu->addAction( tr("Edit Item"));
			connect(editAct, SIGNAL(triggered()), this, SLOT(handleEditItem()));
		}
		pmenu->exec(QCursor::pos());
		delete pmenu;
		actItem = -1;
	}
}

void InlinePalette::handlePasteToItem()
{
	PageItem* selItem = currDoc->m_Selection->itemAt(0);
	PageItem_TextFrame *currItem;
	if (selItem->isTable())
		currItem = selItem->asTable()->activeCell().textFrame();
	else
		currItem = selItem->asTextFrame();
	if (currItem->HasSel)
		currItem->deleteSelectedTextFromFrame();
	currItem->itemText.insertObject(actItem);
	if (selItem->isTable())
		selItem->asTable()->update();
	else
		currItem->update();
}

void InlinePalette::handleEditItem()
{
	emit startEdit(actItem);
}

void InlinePalette::handleDoubleClick(QListWidgetItem *item)
{
	if (item)
		emit startEdit(item->data(Qt::UserRole).toInt());
}

void InlinePalette::editingStart()
{
	for (int a = 0; a < InlineViewWidget->count(); a++)
	{
		QListWidgetItem* item = InlineViewWidget->item(a);
		if (item)
			item->setFlags(0);
	}
}

void InlinePalette::editingFinished()
{
	updateItemList();
}

void InlinePalette::setMainWindow(ScribusMainWindow *mw)
{
	m_scMW = mw;
	if (m_scMW == NULL)
	{
		InlineViewWidget->clear();
		disconnect(m_scMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
		return;
	}
	connect(m_scMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)), Qt::UniqueConnection);
}

void InlinePalette::setDoc(ScribusDoc *newDoc)
{
	if (m_scMW == NULL)
		currDoc = NULL;
	else
		currDoc = newDoc;
	if (currDoc == NULL)
		InlineViewWidget->clear();
	else
		updateItemList();
}

void InlinePalette::unsetDoc()
{
	currDoc = NULL;
	InlineViewWidget->clear();
}

void InlinePalette::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqInlinePalUpdate)
		updateItemList();
}

void InlinePalette::updateItemList()
{
	InlineViewWidget->clear();
	InlineViewWidget->setWordWrap(true);
	for (QHash<int, PageItem*>::iterator it = currDoc->FrameItems.begin(); it != currDoc->FrameItems.end(); ++it)
	{
		PageItem *currItem = it.value();
		QPixmap pm = QPixmap::fromImage(currItem->DrawObj_toImage(48));
		QPixmap pm2(50, 50);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(25 - pm.width() / 2, 25 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, currItem->itemName(), InlineViewWidget);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
		item->setData(Qt::UserRole, currItem->inlineCharID);
	}
}

void InlinePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		ScDockPalette::changeEvent(e);
}

void InlinePalette::languageChange()
{
	setWindowTitle( tr( "Inline Frames" ) );
}
