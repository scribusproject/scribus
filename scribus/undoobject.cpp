/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "undoobject.h"
#include "undostate.h"

ulong UndoObject::m_nextId = 1;


UndoObject::UndoObject() 
		  : m_objectPtr(this)
{
	m_id = m_nextId;
	++m_nextId;
	m_uname = "";
	m_upixmap = NULL;
}

UndoObject::UndoObject(const UndoObject& other)
		  : m_objectPtr(this)
{
	m_id      = other.m_id;
	m_uname   = other.m_uname;
	m_upixmap = other.m_upixmap;
}

UndoObject::UndoObject(const QString &objectName, QPixmap *objectIcon) 
		  : m_objectPtr(this)
{
	m_id = m_nextId;
	++m_nextId;
	m_uname = objectName;
	m_upixmap = objectIcon;
}

UndoObject::~UndoObject()
{
	m_objectPtr.nullify();
}

ulong UndoObject::getUId() const
{
	return m_id;
}

QString UndoObject::getUName()
{
	return m_uname;	
}

void UndoObject::setUName(QString newUName)
{
	m_uname = newUName;
}

QPixmap* UndoObject::getUPixmap()
{
	return m_upixmap;
}

void UndoObject::setUPixmap(QPixmap *newUPixmap)
{
	m_upixmap = newUPixmap;
}

const ScGuardedPtr<UndoObject>& UndoObject::undoObjectPtr()
{
	return m_objectPtr;
}
