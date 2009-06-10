/*****************************************************************
* Copyright (C) 2009  Pierre Marchand

* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
*
******************************************************************/

#include "findimage.h"


findImagesThread::findImagesThread ( const QString& path2, const QStringList& nameFilters2, QDir::SortFlags sort2, bool searchSubfolders2 )
{
	restartThread = false;

	startPath = path2;
	nameFilters = nameFilters2;
	sort = sort2;
	searchSubfolders = searchSubfolders2;
}


void findImagesThread::run()
{
	findFiles ( startPath );
}


void findImagesThread::restart()
{
	restartThread = true;
}


void findImagesThread::findFiles ( const QString& path )
{
	QDir dir ( path );

	if ( dir.exists() )
	{
		QFileInfoList list;
		dir.setFilter ( QDir::AllDirs|QDir::Drives|QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Hidden );
		dir.setNameFilters ( nameFilters );
		dir.setSorting ( sort );

		list = dir.entryInfoList();

		for ( int i = 0 ; ( i < list.size() ) && ( !restartThread ) ; ++i )
		{
			QFileInfo fileInfo = list.at ( i );

			if ( fileInfo.isDir() )
			{
				if ( searchSubfolders )
				{
					findFiles ( fileInfo.canonicalFilePath() );
				}
			}
			else
			{
				imageFiles.append ( fileInfo.canonicalFilePath() );
			}
		}
	}
}


