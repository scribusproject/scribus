/***************************************************************************
 *   Copyright (C) 2009 by Pierre Marchand                                 *
 *   pierre@oep-h.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "pageitemsetterbase.h"

#include <QApplication>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QPixmap>

PageItemSetterBase::PageItemSetterBase(QWidget * parent)
		:QWidget(parent)
{
}

void PageItemSetterBase::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
		startDragPoint = event->pos();

}

void PageItemSetterBase::mouseMoveEvent(QMouseEvent * event)
{
	if (!(event->buttons() & Qt::LeftButton))
		return;
	if ((event->pos() - startDragPoint).manhattanLength() < QApplication::startDragDistance())
		return;

	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	mimeData->setData("text/x-scribus-palette-item", objectName().toUtf8());
	drag->setPixmap(QPixmap::grabWidget(this, rect()));
	drag->setMimeData(mimeData);

	Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

}


void PageItemSetterBase::activate(const bool& act)
{
	foreach( QWidget * widget, findChildren<QWidget*>() )
	{
		widget->setEnabled(act);
	}
}

