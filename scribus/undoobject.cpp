/***************************************************************************
 *   Copyright (C) 2005 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
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

#include "undoobject.h"

ulong UndoObject::nextId = 1;


UndoObject::UndoObject()
{
	_id = nextId;
	++nextId;
	uname = "";
}

UndoObject::UndoObject(QString objectName)
{
	_id = nextId;
	++nextId;
	uname = objectName;
}

ulong UndoObject::getUId()
{
	return _id;
}

QString UndoObject::getUName()
{
	return uname;	
}

void UndoObject::setUName(QString newUName)
{
	uname = newUName;
}
