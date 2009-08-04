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


#include "openpalettemodel.h"
#include "openpalettemanager.h"

#include <QStringList>

OpenPaletteModel::OpenPaletteModel( QObject * parent )
		:QAbstractItemModel(parent)
{
	OpenPaletteManager::subChanges(this, SLOT(paletteChanged()));
}

QModelIndex OpenPaletteModel::index ( int row, int column, const QModelIndex & parent ) const
{
	return createIndex(row, column, parent.isValid() ? parent.row() : 0xFFFF);
}

QModelIndex OpenPaletteModel::parent ( const QModelIndex & index ) const
{
	if(index.isValid())
	{
		if(index.internalId() != 0xFFFF)
			return createIndex(index.internalId() , 0,  0xFFFF);
	}
	return QModelIndex();
}

Qt::ItemFlags OpenPaletteModel::flags ( const QModelIndex & index ) const
{
	if(isTopIndex(index))
	{
		if(index.column() == 0)
		{
			return Qt::ItemIsEnabled | Qt::ItemIsEditable;
		}
		else if (index.column() == 1)
		{
			return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
		}
	}
	return Qt::ItemIsEnabled;
}

QVariant OpenPaletteModel::data ( const QModelIndex & index, int role ) const
{
	if(!index.isValid())
		return QVariant();
	if(index.column() == 0)
	{
		// palette name
		if(isTopIndex(index))
		{
			if(role == Qt::DisplayRole)
			{
				if(index.row() < OpenPaletteManager::count())
					return OpenPaletteManager::title(index.row());
			}
			return QVariant();
		}

		// setter name
		if(role == Qt::DisplayRole)
		{
			if(index.internalId() < OpenPaletteManager::count())
			{
				QStringList pList( OpenPaletteManager::items(index.internalId()) );
				if(index.row() < pList.count())
					return pList.at(index.row());
			}
		}
	}
	else if(index.column() == 1)
	{
		if( isTopIndex(index) && role == Qt::CheckStateRole )
		{
			if(OpenPaletteManager::paletteProperty(index.row(), QString::fromAscii("visible")).toBool())
				return Qt::Checked;
			return Qt::Unchecked;
		}
	}
	return QVariant();
}

QVariant OpenPaletteModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if(section == 0)
			return tr("Elements");
		else if( section == 2)
			return tr("Visible");
	}
	return QVariant();
}


int OpenPaletteModel::rowCount ( const QModelIndex & parent ) const
{
	if(!parent.isValid()) // root
		return OpenPaletteManager::count();
	else
	{
		if(parent.internalId() != 0xFFFF) // its a child item
		{
			return 0;
		}
		else // a top level item
		{
			if(parent.row() < OpenPaletteManager::count())
			{
				return OpenPaletteManager::items(parent.row()).count();
			}
		}
	}
	return 0;
}

int OpenPaletteModel::columnCount ( const QModelIndex & parent ) const
{
	return 2;
}

bool OpenPaletteModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (isTopIndex(index))
		{
		if( index.column() == 0
		&& (role == Qt::EditRole))
		{
			bool ret = OpenPaletteManager::paletteProperty(index.row(), QString::fromAscii("windowTitle") , value);
			if(ret)
			{
				emit dataChanged(index, index);
			}
			return ret;
		}
		else if( index.column() == 1
		&& (role == Qt::CheckStateRole))
		{
			bool ret = OpenPaletteManager::paletteProperty(index.row(), QString::fromAscii("visible") , value);
			if(ret)
			{
				emit dataChanged(index, index);
			}
			return ret;
		}
	}
	return false;

}

bool OpenPaletteModel::isTopIndex(const QModelIndex & index) const
{
	if(index.isValid()
		&& (!index.parent().isValid()))
	{
		return true;
	}
	return false;
}

void OpenPaletteModel::paletteChanged()
{
	emit layoutChanged();
}
