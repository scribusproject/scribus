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

#include "openpalette.h"
#include "pageitemsettersmanager.h"
#include "ui/pageitemsetterbase.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUuid>
#include <QRect>
#include <QDebug>

OpenPalette::OpenPalette(QWidget * parent)
		:ScrPaletteBase(parent)
{
	setAcceptDrops(true);
	setAttribute ( Qt::WA_DeleteOnClose );
	setWindowTitle(tr("empty palette"));
	if(parent)
		setGeometry(QRect(parent->x(),parent->y(), 300, 200));
	else
		setGeometry(QRect(64, 64 , 300, 200));
	mainLayout = new QVBoxLayout(this);
}


void OpenPalette::dragEnterEvent( QDragEnterEvent *event )
{
	if (event->mimeData()->hasFormat("text/x-scribus-palette-item"))
	{
		// prevent copy of an item already present
		QString uidText(event->mimeData()->data("text/x-scribus-palette-item"));
		QUuid pid(uidText);
		foreach(PageItemSetterBase  * b, hosted)
		{
			if(b->uuid() == pid)
				return;
		}
		event->acceptProposedAction();
	}
	else
		qDebug()<<"Entered in Open Palette:"<<event->mimeData()->formats().join("; ");
}


void OpenPalette::dropEvent ( QDropEvent * event )
{
	QString uidText(event->mimeData()->data("text/x-scribus-palette-item"));
	QUuid pid(uidText);
	if(pid.isNull())
		qDebug()<<"Dropped a Null setter uuid on Open Palette";
	else
	{
		PageItemSetterBase * base(PageItemSettersManager::getClone(pid));
		if(base)
		{
			mainLayout->addWidget(base);
			hosted << base;
			QStringList gList;
			foreach(PageItemSetterBase  * b, hosted)
			{
				if(!gList.contains(b->group()))
					gList << b->group();
			}
			setWindowTitle(gList.join("/"));
		}
	}
}



