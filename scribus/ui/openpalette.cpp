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
#include "prefsfile.h"
#include "scpaths.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QCloseEvent>
#include <QRect>
#include <QDebug>

OpenPalette::OpenPalette(QWidget * parent)
#if QT_VERSION >= 0x040500
	: QDialog ( parent, Qt::Tool | Qt::CustomizeWindowHint
			| Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint
			| Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint )
#else
	: QDialog ( parent, Qt::Tool | Qt::CustomizeWindowHint
			| Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint
			| Qt::WindowSystemMenuHint | Qt::WindowType(0x08000000))
#endif
{
	setAcceptDrops(true);
//	setAttribute ( Qt::WA_DeleteOnClose );
	setWindowTitle(tr("empty palette"));
	if(parent)
		setGeometry(QRect(parent->x(),parent->y(), 300, 200));
	else
		setGeometry(QRect(64, 64 , 300, 200));
	mainLayout = new QVBoxLayout(this);
}

QStringList OpenPalette::hostedList() const
{
	QStringList retL;
	foreach(PageItemSetterBase  * b, hosted)
	{
		retL << b->objectName();
	}
	return retL;
}

void OpenPalette::setHosted(const QStringList& sList)
{
	foreach(PageItemSetterBase  * b, hosted)
	{
		delete b;
	}
	hosted.clear();

	foreach(QString oName, sList)
	{
		PageItemSetterBase * base(PageItemSettersManager::getClone(oName));
		if(base)
		{
			mainLayout->addWidget(base);
			hosted << base;
		}
	}
	QStringList gList;
	foreach(PageItemSetterBase  * b, hosted)
	{
		QStringList fn(b->objectName().split("."));
		if(fn.count() > 0)
		{
			if(!gList.contains(fn.first()))
				gList << fn.first();
		}
	}
	setWindowTitle(gList.join("/"));
}

void OpenPalette::closeEvent(QCloseEvent * event)
{
	event->ignore();
	hide();
}

void OpenPalette::showEvent(QShowEvent *event)
{
	QDialog::show();
	emit changed();
}

void OpenPalette::hideEvent(QHideEvent *event)
{
	QDialog::hide();
	emit changed();
}

void OpenPalette::dragEnterEvent( QDragEnterEvent *event )
{
	if (event->mimeData()->hasFormat("text/x-scribus-palette-item"))
	{
		// prevent copy of an item already present
		QString oName(QString::fromUtf8(event->mimeData()->data("text/x-scribus-palette-item")));
		foreach(PageItemSetterBase  * b, hosted)
		{
			if(b->objectName() == oName)
				return;
		}
		event->acceptProposedAction();
	}
	else
		qDebug()<<"Entered in Open Palette:"<<event->mimeData()->formats().join("; ");
}


void OpenPalette::dropEvent ( QDropEvent * event )
{
	QString oName(QString::fromUtf8(event->mimeData()->data("text/x-scribus-palette-item")));
	if(oName.isEmpty())
		qDebug()<<"Dropped a Null setter uuid on Open Palette";
	else
	{
		PageItemSetterBase * base(PageItemSettersManager::getClone(oName));
		if(base)
		{
			mainLayout->addWidget(base);
			hosted << base;
			QStringList gList;
			foreach(PageItemSetterBase  * b, hosted)
			{
				QStringList fn(b->objectName().split("."));
				if(fn.count() > 0)
				{
					if(!gList.contains(fn.first()))
						gList << fn.first();
				}
			}
			setWindowTitle(gList.join("/"));
		}
	}
}



