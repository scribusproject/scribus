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

#include "openpalettemanager.h"
#include "ui/openpalette.h"

#include <QCoreApplication>


OpenPaletteManager * OpenPaletteManager::instance = 0;
OpenPaletteManager::OpenPaletteManager(QObject *parent)
		:QObject(parent)
{
}


OpenPaletteManager * OpenPaletteManager::that()
{
	if(!instance)
	{
		instance = new OpenPaletteManager(QCoreApplication::instance());
		Q_ASSERT(instance);
	}
	return instance;
}

void OpenPaletteManager::savePaletteCollection(const QString& filepath)
{
	// TODO
}

void OpenPaletteManager::loadPaletteCollection(const QString& filepath)
{
	// TODO
}

void OpenPaletteManager::newPalette(QWidget * parent)
{
	OpenPalette * op(new OpenPalette(parent));
	that()->palettes << op;
	connect(op, SIGNAL(changed()), that(), SLOT(catchPalette()));
	connect(op, SIGNAL(destroyed()), that(), SLOT(unRegisterPalette()));
	op->show();
	emit that()->changed();
}

unsigned int OpenPaletteManager::count()
{
	return that()->palettes.count();
}

QString OpenPaletteManager::title(unsigned int idx)
{
	if(idx < count())
		return that()->palettes.at(idx)->windowTitle();
	// Should never happen
	return QString();
}

QVariant OpenPaletteManager::paletteProperty(unsigned int idx, const QString& prop)
{
	if(idx < count())
		return that()->palettes.at(idx)->property(prop.toAscii());
	return QVariant();
}

bool OpenPaletteManager::paletteProperty(unsigned int idx, const QString& prop, const QVariant& val)
{
	if(idx < count())
	{
		return that()->palettes.at(idx)->setProperty(prop.toAscii(), val);
	}
	return false;
}

QStringList OpenPaletteManager::items(unsigned int idx)
{
	if(idx < count())
		return that()->palettes.at(idx)->hostedList();
	return QStringList();
}

void OpenPaletteManager::setItems(unsigned int idx, const QStringList& ilist)
{
	if(idx < count())
	{
		that()->palettes.at(idx)->setHosted(ilist);
		emit that()->changed();
	}
}

void OpenPaletteManager::subChanges(QObject * subscriber, const char * method)
{
	connect(that(), SIGNAL(changed()), subscriber, method);
}

void OpenPaletteManager::unRegisterPalette()
{
	OpenPalette * op(reinterpret_cast<OpenPalette*>(sender()));
	palettes.removeAll(op);
	emit changed();
}

void OpenPaletteManager::catchPalette()
{
	emit changed();
}




