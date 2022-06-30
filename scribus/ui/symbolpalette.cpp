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
#include <QDrag>
#include <QMimeData>
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
	m_delegate = new ScListWidgetDelegate(this, this);
	setItemDelegate(m_delegate);
	setIconSize(QSize(48, 48));
	connect(this, SIGNAL(customContextMenuRequested (const QPoint&)), this, SLOT(handleContextMenu(QPoint)));
}

void SymbolView::handleContextMenu(QPoint p)
{
	QListWidgetItem *item = itemAt(p);
	if (item)
		return;

	QMenu *pmenu = new QMenu();
	QAction* viewAct;
	viewAct = pmenu->addAction( tr("Display Icons only"));
	viewAct->setCheckable(true);
	viewAct->setChecked(m_delegate->iconOnly());
	connect(viewAct, SIGNAL(triggered()), this, SLOT(changeDisplay()));
	pmenu->exec(QCursor::pos());
	pmenu->deleteLater();
}

void SymbolView::changeDisplay()
{
	reset();
	m_delegate->setIconOnly(!m_delegate->iconOnly());
	repaint();
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
		QString text = e->mimeData()->text();
		if ((text.startsWith("<SCRIBUSELEM")) || (text.startsWith("SCRIBUSELEMUTF8")))
			emit objectDropped();
	}
	else
		e->ignore();
}


bool SymbolView::viewportEvent(QEvent *event)
{
	if (event != nullptr)
	{
		if (event->type() == QEvent::ToolTip)
		{
			QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
			QListWidgetItem* it = itemAt(helpEvent->pos());
			if (it != nullptr)
			{
				event->accept();
				QString tipText = it->text();
				QToolTip::showText(helpEvent->globalPos(), tipText, this);
				return true;
			}
		}
		else if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::RightButton)
				return true;
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::RightButton)
			{
				emit customContextMenuRequested(mouseEvent->pos());
				return true;
			}
		}
	}
	return QListWidget::viewportEvent(event);
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

SymbolPalette::SymbolPalette( QWidget* parent) : ScDockPalette(parent, "Symb", Qt::WindowFlags())
{
	setContentsMargins(3, 3, 3, 3);
	setMinimumSize( QSize( 220, 240 ) );
	setObjectName(QString::fromLocal8Bit("Symb"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	SymbolViewWidget = new SymbolView(this);
	SymbolViewWidget->clear();
	setWidget( SymbolViewWidget );

	unsetDoc();
	m_scMW  = nullptr;
	editItemNames.clear();
	languageChange();
	m_item = nullptr;
	connect(SymbolViewWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleDoubleClick(QListWidgetItem*)));
	connect(SymbolViewWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(handleContextMenu(QPoint)));
	connect(SymbolViewWidget, SIGNAL(objectDropped()), this, SIGNAL(objectDropped()));
}

void SymbolPalette::handleContextMenu(QPoint p)
{
	if (!m_doc)
		return;

	QListWidgetItem *item = SymbolViewWidget->itemAt(p);
	if (!item)
		return;
	m_item = item;

	QMenu *pmenu = new QMenu();
	QAction* editAct = pmenu->addAction( tr("Edit Item"));
	editAct->setEnabled(item->flags() & Qt::ItemIsEnabled);
	connect(editAct, SIGNAL(triggered()), this, SLOT(handleEditItem()));

	QAction* delAct = pmenu->addAction( tr("Remove Item"));
	delAct->setEnabled(item->flags() & Qt::ItemIsEnabled);
	connect(delAct, SIGNAL(triggered()), this, SLOT(handleDeleteItem()));

	pmenu->exec(QCursor::pos());
	pmenu->deleteLater();
}

void SymbolPalette::handleDoubleClick(QListWidgetItem *item)
{
	if (item)
		emit startEdit(item->text());
}

void SymbolPalette::handleEditItem()
{
	if (m_item != nullptr)
	{
		emit startEdit(m_item->text());
		m_item = nullptr;
	}
}

void SymbolPalette::handleDeleteItem()
{
	if (m_item == nullptr)
		return;

	if (m_doc->docPatterns.contains(m_item->text()))
	{
		m_doc->removePattern(m_item->text());
		updateSymbolList();
		m_doc->regionsChanged()->update(QRect());
	}
	m_item = nullptr;
}

void SymbolPalette::editingStart(QStringList names)
{
	editItemNames = names;
	for (int a = 0; a < editItemNames.count(); a++)
	{
		QList<QListWidgetItem*> items = SymbolViewWidget->findItems(names[a], Qt::MatchExactly);
		if (items.count() > 0)
			items[0]->setFlags(Qt::NoItemFlags);
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
	if (m_scMW == nullptr)
	{
		SymbolViewWidget->clear();
		disconnect(m_scMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
		return;
	}
	connect(m_scMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)), Qt::UniqueConnection);
}

void SymbolPalette::setDoc(ScribusDoc *newDoc)
{
	if (m_scMW == nullptr)
		m_doc = nullptr;
	else
		m_doc = newDoc;
	if (m_doc == nullptr)
	{
		SymbolViewWidget->clear();
		setEnabled(true);
	}
	else
	{
		setEnabled(!m_doc->drawAsPreview);
		updateSymbolList();
	}
}

void SymbolPalette::unsetDoc()
{
	m_doc = nullptr;
	SymbolViewWidget->clear();
	setEnabled(true);
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
	if (!m_doc)
		return;

	QStringList patK = m_doc->docPatterns.keys();
	patK.sort();
	for (int i = 0; i < patK.count(); ++i)
	{
		const QString& patternName = patK.at(i);
		ScPattern sp = m_doc->docPatterns.value(patternName);
		QPixmap pm;
		if (sp.getPattern()->width() >= sp.getPattern()->height())
			pm = QPixmap::fromImage(sp.getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm = QPixmap::fromImage(sp.getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, patternName, SymbolViewWidget);
		if (editItemNames.contains(patternName))
			item->setFlags(Qt::NoItemFlags);
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
		ScDockPalette::changeEvent(e);
}

void SymbolPalette::languageChange()
{
	setWindowTitle( tr( "Symbols" ) );
}

void SymbolPalette::keyPressEvent(QKeyEvent* e)
{
	if (!m_doc)
		return;
	switch (e->key())
	{
		case Qt::Key_Backspace:
		case Qt::Key_Delete:
			{
				QListWidgetItem* it = SymbolViewWidget->currentItem();
				if (it != nullptr)
				{
					if (m_doc->docPatterns.contains(it->text()))
					{
						m_doc->removePattern(it->text());
						updateSymbolList();
						m_doc->regionsChanged()->update(QRect());
						e->accept();
					}
				}
			}
			break;
		default:
			break;
	}
}
