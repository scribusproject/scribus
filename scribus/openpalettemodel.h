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

#ifndef OPENPALETTEMODEL_H
#define OPENPALETTEMODEL_H

#include <QAbstractItemModel>



/**
  * Provides a model to attach to a treeview representing
  * the state of open palettes configuration.
  * Its own data being actually hold by OpenPaletteManager,
  * so most of the implementations of virtual methods are in fact
  * forwarded to OpenPaletteManager.
  */

class OpenPaletteModel : public QAbstractItemModel
{
	Q_OBJECT
	OpenPaletteModel(){}
public:
	OpenPaletteModel( QObject * parent );

	/// Standard reimplementations for an abstract item model (read-only)
	QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	QModelIndex parent ( const QModelIndex & index ) const;
	Qt::ItemFlags flags ( const QModelIndex & index ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount ( const QModelIndex & parent = QModelIndex() ) const;

	// write
	bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );


private:
	bool isTopIndex(const QModelIndex & index) const;

public slots:
	void paletteChanged();

};

#endif // OPENPALETTEMODEL_H
