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

Collections::Collections(const QString& collectionsName)
{
	name = collectionsName;
}

CollectionReaderThread::CollectionReaderThread(const QString& xmlFile2, bool importCollection)
{
	categoriesCount = 0;
	collection = nullptr;
	type = 0;
	restartThread = false;

	xmlFile = xmlFile2;
	import = importCollection;
}

void CollectionReaderThread::readFile()
{
	QFile inputFile(xmlFile);
	if (!inputFile.open(QFile::ReadOnly | QFile::Text))
		return;

	setDevice(&inputFile);

	while (!atEnd())
	{
		readNext();

		if (!isStartElement())
			continue;

		if (name() == QLatin1String("picturebrowser"))
		{
			//we have a collectionsdbfile
			if (attributes().value("type") == QLatin1String("collectionsset"))
			{
				readCollectionsDb();
				type = 1;
			}
			//we have a collectionfile
			else if (attributes().value("type") == QLatin1String("collection"))
			{
				collection = new ImageCollection;
				collection->file = xmlFile;

				QString name = attributes().value("name").toString();
				if (!name.isEmpty())
					collection->name = name;
				else
					collection->name = xmlFile;

				readCollectionFile();
				type = 2;
			}
		}
	}
}

void CollectionReaderThread::readCollectionsDb()
{
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
			break;

		if (isStartElement())
		{
			if (name() == QLatin1String("category"))
			{
				QString name = attributes().value("name").toString();
				Collections* tmpCollections = new Collections(name);
				collectionsSet.append(tmpCollections);

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

void CollectionReaderThread::readCollectionFile()
{
	while (!atEnd() && !restartThread)
	{
		readNext();

		if (isEndElement())
			break;

		if (isStartElement())
		{
			if (name() == QLatin1String("image"))
			{
				QString tmpImageFile = attributes().value("file").toString();
				collection->imageFiles.append(tmpImageFile);

				readImage();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}

void CollectionReaderThread::readCategory()
{
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
			break;

		if (isStartElement())
		{
			if (name() == QLatin1String("collection"))
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

void CollectionReaderThread::readCollection()
{
	Collections *tmpCollections = collectionsSet.at ( categoriesCount );

	tmpCollections->collectionFiles.append ( attributes().value ( "file" ).toString() );
	tmpCollections->collectionNames.append ( readElementText() );
}

void CollectionReaderThread::readImage()
{
	QStringList tmpTags;

	while (!atEnd() && !restartThread)
	{
		readNext();

		if (isEndElement())
			break;

		if (isStartElement())
		{
			if (name() == QLatin1String("tag"))
			{
				//read tag here
				tmpTags.append(readElementText());
			}
			else
			{
				readUnknownElement();
			}
		}
	}

	collection->tags.append(tmpTags);
}

void CollectionReaderThread::readUnknownElement()
{
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
			break;

		if (isStartElement())
		{
			readUnknownElement();
		}
	}
}

void CollectionReaderThread::run()
{
	readFile();
}

void CollectionReaderThread::restart()
{
	restartThread = true;
}

CollectionListReaderThread::CollectionListReaderThread(const QStringList& xmlFiles2)
{
	m_clrt = nullptr;
	restartThread = false;

	xmlFiles = xmlFiles2;
}

void CollectionListReaderThread::run()
{
	if (xmlFiles.isEmpty())
		return;

	xmlFile = xmlFiles.takeAt(0);
	m_clrt = new CollectionReaderThread(xmlFile, false);
	connect(m_clrt, SIGNAL(finished()), this, SLOT(collectionReaderThreadFinished()));
	m_clrt->start();

	exec();
}

void CollectionListReaderThread::restart()
{
	restartThread = true;
}

void CollectionListReaderThread::collectionReaderThreadFinished()
{
	readCollections.append(m_clrt->collection);
	delete m_clrt;

	if (xmlFiles.isEmpty() || restartThread)
	{
		quit();
	}
	else
	{
		xmlFile = xmlFiles.takeAt(0);
		m_clrt = new CollectionReaderThread(xmlFile, false);
		connect(m_clrt, SIGNAL(finished()), this, SLOT(collectionReaderThreadFinished()));
		m_clrt->start();
	}
}

CollectionsWriterThread::CollectionsWriterThread(const QString& xmlFile2, const QList<Collections *>& saveCollections2)
{
	xmlFile = xmlFile2;
	saveCollections = saveCollections2;
	restartThread = false;
}

void CollectionsWriterThread::writeFile()
{
	QFile outputFile(xmlFile);
	if (!outputFile.open(QFile::WriteOnly | QFile::Text))
		return;

	setDevice(&outputFile);

	writeStartDocument();
	writeCharacters("\n");
	//writeDTD("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	writeStartElement("picturebrowser");

	writeAttribute("type", "collectionsset");

	writeCharacters("\n");

	for (int i = 0; i < saveCollections.size() && !restartThread; ++i)
	{
		writeCategory(saveCollections.at(i));
	}

	writeEndDocument();
}

void CollectionsWriterThread::writeCategory(const Collections *category)
{
	writeStartElement("category");
	writeAttribute("name", category->name);
	writeCharacters("\n");

	for (int i = 0; i < category->collectionNames.size() && !restartThread; ++i)
	{
		writeCollection(category->collectionNames.at(i), category->collectionFiles.at(i));
	}

	writeEndElement();
	writeCharacters("\n");
}

void CollectionsWriterThread::writeCollection ( const QString &collectionName, const QString &collectionFile )
{
	writeStartElement("collection");
	writeAttribute("file", collectionFile);

	writeCharacters(collectionName);

	writeEndElement();
	writeCharacters("\n");
}

void CollectionsWriterThread::run()
{
	writeFile();
}

void CollectionsWriterThread::restart()
{
	restartThread = true;
}

CollectionWriterThread::CollectionWriterThread(const QString& xmlFile2, const ImageCollection& saveCollection2)
{
	xmlFile = xmlFile2;
	saveCollection = saveCollection2;
}

void CollectionWriterThread::writeFile()
{
	QFile outputFile(xmlFile);
	if (!outputFile.open(QFile::WriteOnly | QFile::Text))
		return;

	setDevice(&outputFile);

	writeStartDocument();
	writeCharacters("\n");
	//writeDTD("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	writeStartElement("picturebrowser");

	writeAttribute("type", "collection");

	if (!saveCollection.name.isEmpty())
	{
		writeAttribute("name", saveCollection.name);
	}

	writeCharacters("\n");

	for (int i = 0; i < saveCollection.imageFiles.size(); ++i)
	{
		writeImage(saveCollection.imageFiles.at(i), saveCollection.tags.at(i));
	}

	writeEndDocument();
}

void CollectionWriterThread::writeImage(const QString& imageFile, const QStringList& tags)
{
	writeStartElement("image");
	writeAttribute("file", imageFile);
	writeCharacters("\n");

	writeTags(tags);

	writeEndElement();
	writeCharacters("\n");
}

void CollectionWriterThread::writeTags(const QStringList& tags)
{
	for (int i = 0; i < tags.size(); ++i)
	{
		writeStartElement("tag");

		writeCharacters(tags.at(i));

		writeEndElement();
		writeCharacters("\n");
	}
}

void CollectionWriterThread::run()
{
	writeFile();
}
