/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2010 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
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
#include "swatchcombo.h"
#include <QFontMetrics>
#include <QFileInfo>

SwatchCombo::SwatchCombo( QWidget* parent ) : QToolButton(parent)
{
	dataTree = new QTreeWidget(NULL);
	dataTree->setHeaderHidden(true);
	dataTree->setMinimumSize(QSize(140, 200));
	popUp = new QMenu();
	popUpAct = new QWidgetAction(this);
	popUpAct->setDefaultWidget(dataTree);
	popUp->addAction(popUpAct);
	setMenu(popUp);
	setPopupMode(QToolButton::MenuButtonPopup);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	connect(dataTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemActivated(QTreeWidgetItem*)));
}

void SwatchCombo::itemActivated(QTreeWidgetItem* item)
{
	if (item)
	{
		if (item->flags() & Qt::ItemIsSelectable)
		{
			menu()->hide();
			setText(item->text(0));
			QFontMetrics fm(font());
			QString elText = fm.elidedText(item->text(0), Qt::ElideMiddle, width());
			setText(elText);
			setToolTip(item->text(0));
			emit activated(item->text(0));
			emit activated(item);
		}
	}
}

QTreeWidgetItem* SwatchCombo::addTopLevelItem(QString name)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(dataTree);
	item->setFlags(Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	item->setText(0, name);
	return item;
}

QTreeWidgetItem* SwatchCombo::addSubItem(QString name, QTreeWidgetItem* parent, bool selectable)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
	if (selectable)
		item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	else
		item->setFlags(Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	item->setText(0, name);
	return item;
}

void SwatchCombo::setCurrentComboItem(QString text)
{
	QFontMetrics fm(font());
	setToolTip(text);
	QList<QTreeWidgetItem*> lg;
	if (text == "Scribus Small")
	{
		QString elText = fm.elidedText(text, Qt::ElideMiddle, width());
		setText(elText);
		lg = dataTree->findItems(text, Qt::MatchExactly | Qt::MatchRecursive);
		dataTree->setCurrentItem(lg[0]);
		return;
	}
	else
	{
		QFileInfo fo(text);
		QString txt = fo.baseName();
		QString elText = fm.elidedText(txt, Qt::ElideMiddle, width());
		setText(elText);
		QString dText = fo.absolutePath();
		lg = dataTree->findItems(txt, Qt::MatchExactly | Qt::MatchRecursive);
		if (lg.count() > 0)
		{
			for (int a = 0; a < lg.count(); a++)
			{
				if (dText == lg[a]->data(0, Qt::UserRole).toString())
				{
					dataTree->setCurrentItem(lg[0]);
					break;
				}
			}
		}
	}
}

QTreeWidgetItem* SwatchCombo::currentItem()
{
	return dataTree->currentItem();
}
