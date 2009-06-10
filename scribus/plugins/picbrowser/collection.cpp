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

#include "collection.h"


#include <QFile>


imageCollection::imageCollection()
{
}


collections::collections ( QString collectionsName )
{
	name = collectionsName;
}


collectionReaderThread::collectionReaderThread ( QString &xmlFile2, bool importCollection )
{
	categoriesCount = 0;
	type = 0;
	restartThread = false;

	xmlFile = xmlFile2;
	import = importCollection;
}


void collectionReaderThread::readFile()
{
	QFile inputFile ( xmlFile );

	if ( !inputFile.open ( QFile::ReadOnly | QFile::Text ) )
	{
		return;
	}

	setDevice ( &inputFile );

	while ( !atEnd() )
	{
		readNext();

		if ( isStartElement() )
		{
			if ( name() == "picturebrowser" )
			{
				//we have a collectionsdbfile
				if ( attributes().value ( "type" ) == "collectionsset" )
				{
					readCollectionsDb();
					type = 1;
				}
				//we have a collectionfile
				else if ( attributes().value ( "type" ) == "collection" )
				{
					collection = new imageCollection;
					collection->file = xmlFile;

					QString name = attributes().value ( "name" ).toString();

					if ( !name.isEmpty() )
					{
						collection->name = name;
					}
					else
					{
						collection->name = xmlFile;
					}

					readCollectionFile();
					type = 2;
				}
			}
		}
	}
}


void collectionReaderThread::readCollectionsDb()
{
	while ( !atEnd() )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			if ( name() == "category" )
			{
				QString name = attributes().value ( "name" ).toString();
				collections *tmpCollections = new collections ( name );
				collectionsSet.append ( tmpCollections );

				readCategory();

				categoriesCount++;
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}


void collectionReaderThread::readCollectionFile()
{
	while ( !atEnd() && !restartThread )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			if ( name() == "image" )
			{
				QString tmpImageFile = attributes().value ( "file" ).toString();
				collection->imageFiles.append ( tmpImageFile );

				readImage();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}


void collectionReaderThread::readCategory()
{
	while ( !atEnd() )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			if ( name() == "collection" )
			{
				readCollection();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}


void collectionReaderThread::readCollection()
{
	collections *tmpCollections = collectionsSet.at ( categoriesCount );

	tmpCollections->collectionFiles.append ( attributes().value ( "file" ).toString() );
	tmpCollections->collectionNames.append ( readElementText() );
}


void collectionReaderThread::readImage()
{
	QStringList tmpTags;

	while ( !atEnd() && !restartThread )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			if ( name() == "tag" )
			{
				//read tag here
				tmpTags.append ( readElementText() );
			}
			else
			{
				readUnknownElement();
			}
		}
	}

	collection->tags.append ( tmpTags );
}


void collectionReaderThread::readUnknownElement()
{
	while ( !atEnd() )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			readUnknownElement();
		}
	}
}


void collectionReaderThread::run()
{
	readFile();
}


void collectionReaderThread::restart()
{
	restartThread = true;
}


collectionListReaderThread::collectionListReaderThread ( QStringList &xmlFiles2 )
{
	restartThread = false;

	xmlFiles = xmlFiles2;
}


void collectionListReaderThread::run()
{
	if ( xmlFiles.isEmpty() )
	{
		return;
	}

	xmlFile = xmlFiles.takeAt ( 0 );
	clrt = new collectionReaderThread ( xmlFile, false );
	connect ( clrt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadFinished() ) );
	clrt->start();

	exec();
}


void collectionListReaderThread::restart()
{
	restartThread = true;
}


void collectionListReaderThread::collectionReaderThreadFinished()
{
	readCollections.append ( clrt->collection );
	delete clrt;

	if ( xmlFiles.isEmpty() || restartThread )
	{
		quit();
	}
	else
	{
		xmlFile = xmlFiles.takeAt ( 0 );
		clrt = new collectionReaderThread ( xmlFile, false );
		connect ( clrt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadFinished() ) );
		clrt->start();
	}
}


collectionsWriterThread::collectionsWriterThread ( QString &xmlFile2, QList<collections *> saveCollections2 )
{
	xmlFile = xmlFile2;
	saveCollections = saveCollections2;
	restartThread = false;
}


void collectionsWriterThread::writeFile()
{
	QFile outputFile ( xmlFile );

	if ( !outputFile.open ( QFile::WriteOnly | QFile::Text ) )
	{
		return;
	}

	setDevice ( &outputFile );

	writeStartDocument();
	writeCharacters ( "\n" );
//writeDTD("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	writeStartElement ( "picturebrowser" );

	writeAttribute ( "type", "collectionsset" );

	writeCharacters ( "\n" );

	for ( int i = 0 ; i < saveCollections.size() && !restartThread ; ++i )
	{
		writeCategory ( saveCollections.at ( i ) );
	}

	writeEndDocument();
}


void collectionsWriterThread::writeCategory ( const collections *category )
{
	writeStartElement ( "category" );
	writeAttribute ( "name", category->name );
	writeCharacters ( "\n" );

	for ( int i = 0 ; i < category->collectionNames.size() && !restartThread ; ++i )
	{
		writeCollection ( category->collectionNames.at ( i ), category->collectionFiles.at ( i ) );
	}

	writeEndElement();
	writeCharacters ( "\n" );
}


void collectionsWriterThread::writeCollection ( const QString &collectionName, const QString &collectionFile )
{
	writeStartElement ( "collection" );
	writeAttribute ( "file", collectionFile );

	writeCharacters ( collectionName );

	writeEndElement();
	writeCharacters ( "\n" );
}


void collectionsWriterThread::run()
{
	writeFile();
}


void collectionsWriterThread::restart()
{
	restartThread = true;
}



collectionWriterThread::collectionWriterThread ( QString &xmlFile2, imageCollection &saveCollection2 )
{
	xmlFile = xmlFile2;
	saveCollection = saveCollection2;
}


void collectionWriterThread::writeFile()
{
	QFile outputFile ( xmlFile );

	if ( !outputFile.open ( QFile::WriteOnly | QFile::Text ) )
	{
		return;
	}

	setDevice ( &outputFile );

	writeStartDocument();
	writeCharacters ( "\n" );
//writeDTD("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	writeStartElement ( "picturebrowser" );

	writeAttribute ( "type", "collection" );

	if ( !saveCollection.name.isEmpty() )
	{
		writeAttribute ( "name", saveCollection.name );
	}

	writeCharacters ( "\n" );

	for ( int i = 0 ; i < saveCollection.imageFiles.size() ; ++i )
	{
		writeImage ( saveCollection.imageFiles.at ( i ), saveCollection.tags.at ( i ) );
	}

	writeEndDocument();
}


void collectionWriterThread::writeImage ( const QString &imageFile, const QStringList &tags )
{
	writeStartElement ( "image" );
	writeAttribute ( "file", imageFile );
	writeCharacters ( "\n" );

	writeTags ( tags );

	writeEndElement();
	writeCharacters ( "\n" );
}


void collectionWriterThread::writeTags ( const QStringList &tags )
{
	for ( int i = 0 ; i < tags.size() ; ++i )
	{
		writeStartElement ( "tag" );

		writeCharacters ( tags.at ( i ) );

		writeEndElement();
		writeCharacters ( "\n" );
	}
}


void collectionWriterThread::run()
{
	writeFile();
}



