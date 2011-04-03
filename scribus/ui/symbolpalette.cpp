/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          symbolpalette.cpp  -  description
                             -------------------
    begin                : Sat May 8 2010
    copyright            : (C) 2010 by Franz Schmid
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
#include "symbolpalette.h"
#include <QPainter>
#include <QByteArray>
#include "scpattern.h"
#include "scribusdoc.h"
#include "scribus.h"

SymbolView::SymbolView(QWidget* parent) : QListWidget(parent)
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
	setIconSize(QSize(48, 48));
}

void SymbolView::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void SymbolView::dragMoveEvent(QDragMoveEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void SymbolView::dropEvent(QDropEvent *e)
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

 void SymbolView::startDrag(Qt::DropActions supportedActions)
 {
	QMimeData *mimeData = new QMimeData;
	QByteArray data = currentItem()->text().toLocal8Bit();
	mimeData->setData("text/symbol", data);
	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(currentItem()->icon().pixmap(48, 48));
	drag->exec(Qt::CopyAction);
	clearSelection();
}

SymbolPalette::SymbolPalette( QWidget* parent) : ScrPaletteBase( parent, "Symb", false, 0 )
{
	setMinimumSize( QSize( 220, 240 ) );
	setMaximumSize( QSize( 800, 600 ) );
	SymbolViewWidget = new SymbolView(this);
	SymbolViewWidget->clear();

	QVBoxLayout* PaletteLayout = new QVBoxLayout;
	PaletteLayout->addWidget(SymbolViewWidget);
	setLayout( PaletteLayout );

	unsetDoc();
	m_scMW  = NULL;
	editItemNames.clear();
	languageChange();
	connect(SymbolViewWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
}

void SymbolPalette::handleDoubleClick(QListWidgetItem *item)
{
	if (item)
		emit startEdit(item->text());
}

void SymbolPalette::editingStart(QStringList names)
{
	editItemNames = names;
	for (int a = 0; a < editItemNames.count(); a++)
	{
		QList<QListWidgetItem*> items = SymbolViewWidget->findItems(names[a], Qt::MatchExactly);
		if (items.count() > 0)
			items[0]->setFlags(0);
	}
}

void SymbolPalette::editingFinished()
{
	editItemNames.clear();
	updateSymbolList();
}

void SymbolPalette::setMainWindow(ScribusMainWindow *mw)
{
	m_scMW = mw;
	if (m_scMW == NULL)
	{
		SymbolViewWidget->clear();
		disconnect(m_scMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
		return;
	}
	connect(m_scMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)), Qt::UniqueConnection);
}

void SymbolPalette::setDoc(ScribusDoc *newDoc)
{
	if (m_scMW == NULL)
		currDoc = NULL;
	else
		currDoc = newDoc;
	if (currDoc == NULL)
		SymbolViewWidget->clear();
	else
		updateSymbolList();
}

void SymbolPalette::unsetDoc()
{
	currDoc = NULL;
	SymbolViewWidget->clear();
}

void SymbolPalette::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqSymbolsUpdate)
		updateSymbolList();
}

void SymbolPalette::updateSymbolList()
{
	SymbolViewWidget->clear();
	SymbolViewWidget->setWordWrap(true);
	for (QMap<QString, ScPattern>::Iterator it = currDoc->docPatterns.begin(); it != currDoc->docPatterns.end(); ++it)
	{
		QPixmap pm;
		if (it.value().getPattern()->width() >= it.value().getPattern()->height())
			pm = QPixmap::fromImage(it.value().getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm = QPixmap::fromImage(it.value().getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, it.key(), SymbolViewWidget);
		if (editItemNames.contains(it.key()))
			item->setFlags(0);
		else
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
	}
}

void SymbolPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void SymbolPalette::languageChange()
{
	setWindowTitle( tr( "Symbols" ) );
}
