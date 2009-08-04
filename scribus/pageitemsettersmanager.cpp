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

#include "pageitemsettersmanager.h"
#include "ui/pageitemsetterbase.h"
#include "pageitem.h"
#include "selection.h"
#include "scribusdoc.h"

#include <QCoreApplication>



PageItemSettersManager * PageItemSettersManager::instance = 0;

PageItemSettersManager::PageItemSettersManager(QObject * parent)
		:QObject(parent), selection(0), doc(0)
{
}

PageItemSettersManager * PageItemSettersManager::that()
{
	if(!instance)
	{
		instance = new PageItemSettersManager(QCoreApplication::instance());
		Q_ASSERT(instance);
	}
	return instance;
}

void PageItemSettersManager::registerSetter(PageItemSetterBase * base)
{
	if(base && !that()->setters.contains(base))
	{
		that()->setters.append(base);
		if(that()->selection)
			base->changeItem(that()->selection);
		connect(base, SIGNAL(destroyed()), that(), SLOT(UnRegisterSetter()));
	}
}

void PageItemSettersManager::UnRegisterSetter()
{
	PageItemSetterBase * base = reinterpret_cast<PageItemSetterBase*>(sender());
	setters.removeAll(base);
}

void PageItemSettersManager::setSelection(Selection * sel)
{
	if(that()->selection)
		disconnect(that()->selection, SIGNAL(selectionChanged()), that(), SLOT(updateSelection()));
	if(that()->doc)
		disconnect(that()->doc, SIGNAL(rotationMode(int)), that(), SLOT(rotationModeChanged(int)));

	that()->selection = sel;
	that()->doc = (sel && (sel->count() > 0)) ? sel->itemAt(0)->m_Doc : 0;

	if(that()->selection)
		connect(that()->selection, SIGNAL(selectionChanged()), that(), SLOT(updateSelection()));
	if(that()->doc)
		connect(that()->doc, SIGNAL(rotationMode(int)), that(), SLOT(rotationModeChanged(int)));

	foreach(PageItemSetterBase * base, that()->setters)
	{
		base->changeItem(sel);
	}
}

PageItemSetterBase* PageItemSettersManager::getClone(const QString& type)
{
	foreach(PageItemSetterBase* base, that()->setters)
	{
		if(base->objectName() == type)
			return base->clone();
	}
	return 0;
}

void PageItemSettersManager::rotationModeChanged(int)
{
	Selection * s = that()->selection ;
	foreach(PageItemSetterBase * base, that()->setters)
	{
		base->changeItem(s);
	}
}

void PageItemSettersManager::updateSelection()
{
	Selection * s = that()->selection ;
	foreach(PageItemSetterBase * base, that()->setters)
	{
		base->changeItem(s);
	}
}
