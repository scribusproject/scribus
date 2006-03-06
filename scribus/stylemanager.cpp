/***************************************************************************
 *   Copyright (C) 2005 by Riku Leino                                      *
 *   riku@scribus.info                                                     *
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

#include "stylemanager.h"
#include "stylemanager.moc"
#include "styleitem.h"
#include "scribusdoc.h"
#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qlineedit.h>
#include <qlayout.h>

StyleManager::StyleManager(QWidget *parent, const char *name) : SMBase(parent, name), item_(0), widget_(0)
{
	typeView->header()->hide();
	styleView->header()->hide();
	layout_ = new QGridLayout(mainFrame);
}

void StyleManager::currentDoc(ScribusDoc *doc)
{
	for (uint i = 0; i < items_.count(); ++i)
		items_.at(i)->currentDoc(doc);
	slotTypeChanged(typeView->currentItem()); // refresh the style view
}

void StyleManager::languageChange()
{

}

void StyleManager::addStyle(StyleItem *item)
{
	items_.append(item);
	new QListViewItem(typeView, item->typeName());
}

void StyleManager::slotApply()
{
	
}


void StyleManager::slotDelete()
{
	
}


void StyleManager::slotImport()
{
	
}


void StyleManager::slotClone()
{
	
}


void StyleManager::slotNew()
{
	
}

void StyleManager::slotStyleChanged()
{
	QStringList selected;
	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
	while (it.current()) {
		selected << it.current()->text(0);
		++it;
	}

	uint count = selected.count();
	disconnect(nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotNameChanged(const QString&)));
	if (count > 1) {
		cloneButton->setEnabled(true);
		deleteButton->setEnabled(true);
		nameEdit->setText(tr("More than one item selected"));
		nameEdit->setEnabled(false);
		widget_->setEnabled(true);
	} else if (count == 1) {
		cloneButton->setEnabled(true);
		deleteButton->setEnabled(true);
		nameEdit->setEnabled(true);
		nameEdit->setText(selected[0]);
		widget_->setEnabled(true);
	} else {
		cloneButton->setEnabled(false);
		deleteButton->setEnabled(false);
		nameEdit->setText("");
		nameEdit->setEnabled(false);
		widget_->setEnabled(false);
	}
	connect(nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotNameChanged(const QString&)));
	if (item_) {
		item_->selected(selected);
	}
}

// f.e. Line Styles --> Paragraph Styles
void StyleManager::slotTypeChanged(QListViewItem *item)
{
	if (item) {
		item_ = 0;
		for (uint i = 0; i < items_.count(); ++i)
		{ // search the selected StyleItem based on selection
			if (items_.at(i)->typeName() == item->text(0))
			{
				item_ = items_.at(i);
				break;
			}
		}
		styleView->clear();
		if (item_)
		{
			QStringList styles = item_->styles();
			for (uint i = 0; i < styles.count(); ++i) // set the list of styles of this type
				new QListViewItem(styleView, styles[i]);

			if (widget_)
			{   // remove the old style type's widget
				widget_->hide();
				layout_->remove(widget_);
				widget_->reparent(0,0, QPoint(0,0), false);
			}
			// show the widget for the new style type
			widget_ = item_->widget(); // show the widget for the style type
			widget_->reparent(mainFrame, 0, QPoint(0,0), true);
			layout_->addWidget(widget_, 0, 0);
		}
	}
}

void StyleManager::slotPageChanged(QWidget *widget)
{

}

void StyleManager::slotNameChanged(const QString& name)
{
	styleView->currentItem()->setText(0, name);
	if (item_)
		item_->nameChanged(name);
}

void StyleManager::hideEvent(QHideEvent *e)
{
	SMBase::hideEvent(e);
	emit closed();
}

StyleManager::~StyleManager()
{
	
}

