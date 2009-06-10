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

#ifndef FINDIMAGE_H
#define FINDIMAGE_H

#include <QDir>
#include <QString>
#include <QStringList>
#include <QThread>

//thread to search images in a folder and subfolders
class findImagesThread : public QThread
{
	public:
		//parameters:
		//const QString& path2: path to be searched
		//const QStringList& nameFilters2: namefilters for the images
		//QDir::SortFlags sort2: sortflags
		//bool searchSubfolders2: tells wether to search subfolders
		findImagesThread ( const QString& path2, const QStringList& nameFilters2, QDir::SortFlags sort2, bool searchSubfolders2 );
		//called after thread has been started
		void run();
		//called when the thread should be restarted
		void restart();

		//finds the images in a given folder
		void findFiles ( const QString& path );

		//contains the images which have been found
		QStringList imageFiles;
		//tells wether the thread should restart
		volatile bool restartThread;

	private:
		//path to start searching
		QString startPath;
		//namefilters for search
		QStringList nameFilters;
		//sortflags for search
		QDir::SortFlags sort;
		//tells wether to search subfolders
		bool searchSubfolders;
};


#endif // FINDIMAGE_H
