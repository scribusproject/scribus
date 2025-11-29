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

#ifndef COLLECTION_H
#define COLLECTION_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class ImageCollection
{
	public:
		//name of the collection
		QString name;
		//path to the collectionfile
		QString file;
		//paths to the images in the collection
		QStringList imageFiles;
		//tags to each image;
		QList<QStringList> tags;
};

//manages a set of collections
class Collections
{
	public:
		//sets name
		explicit Collections(const QString& collectionsName);

		//contains name of the collections
		QString name;

		QStringList collectionNames;
		QStringList collectionFiles;
};

//a thread to read collectionfiles
class CollectionReaderThread : public QXmlStreamReader, public QThread
{
	public:
		//set file to read
		CollectionReaderThread(const QString& xmlFile2, bool importCollection);

		//starts reading the file
		void readFile();

		//is executed after starting the thread
		void run() override;
		//restarts the thread
		void restart();

		//true if thread should restart
		volatile bool restartThread { false };

		//hierarchy for collectionsbrowser qtreewidget
		QList<Collections *> collectionsSet;
		//contains the images read from the file
		ImageCollection *collection { nullptr };

		//tells which filetype it was, 0 for collectionsset, 1 for collection
		int type { 0 };
		//if it is an import
		bool import { false };
		//the file to read from
		QString xmlFile;
		QStringList addImages;

	private:
		//reads collectionsdatabase
		void readCollectionsDb();
		//reads a collectionfile
		void readCollectionFile();

		//reads the category of a collectionsdb
		void readCategory();
		//reads a collection from a collectionsdb
		void readCollection();

		//reads an image from a collectionfile
		void readImage();

		//called when an unknown element is encountered
		void readUnknownElement();

		//tells how many categories the collectionsdb contained
		int categoriesCount { 0 };
};

class CollectionListReaderThread : public QThread
{
		Q_OBJECT

	public:
		//set files to read
		explicit CollectionListReaderThread(const QStringList& xmlFiles2);

		//is executed after starting the thread
		void run() override;
		//restarts the thread
		void restart();

		//true if thread should restart
		volatile bool restartThread { false };

		QString xmlFile;
		QStringList xmlFiles;

		QList<ImageCollection *> readCollections;

	private slots:
		void collectionReaderThreadFinished();

	private:
		CollectionReaderThread *m_clrt { nullptr };
};

//thread to write a collectionssetfile
class CollectionsWriterThread : public QXmlStreamWriter, public QThread
{
	public:
		//sets initial values
		//parameters:
		//QString &xmlFile2: the file to write
		//int fileType: the type of file, 0 for collectionfile, 1 for collectionsdb
		//QList<Collections *> saveCollections2: the collectionsset to write to the file
		CollectionsWriterThread(const QString& xmlFile2, QList<Collections*> saveCollections2);
		
		//starts writing to the file
		void writeFile();

		//called after the thread has been started
		void run() override;
		//restarts the thread
		void restart();

		//true if thread should restart
		volatile bool restartThread { false };

	private:
		//writes a category into a collectionssetfile
		void writeCategory(const Collections* category);
		//writes a collection into a collectionssetfile
		void writeCollection(const QString& collectionName, const QString& collectionFile);

		//contains the path to the output file
		QString xmlFile;
		//the collection to write into a collectionfile
		QList<Collections *> saveCollections;
};

//thread to write a collectionfile
class CollectionWriterThread : public QXmlStreamWriter, public QThread
{
	public:
		//sets initial values
		//parameters:
		//QString &xmlFile2: the file to write
		//ImageCollection *saveCollection2: the collection to write to the file
		CollectionWriterThread(const QString& xmlFile2, const ImageCollection& saveCollection2);

		//starts writing to the file
		void writeFile();

		//called after the thread has been started
		void run() override;

	private:
		//writes an image into a collectionfile
		void writeImage(const QString& imageFile, const QStringList& tags);
		void writeTags(const QStringList& tags);

		//contains the path to the output file
		QString xmlFile;
		//the collection to write into a collectionfile
		ImageCollection saveCollection;
};

#endif // COLLECTION_H
