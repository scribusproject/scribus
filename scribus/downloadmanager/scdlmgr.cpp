#include "scdlmgr.h"
#include "scdlthread.h"

#include <QDebug>

#include <QString>
#include <QStringList>
#include <QTimer>

#include <stdio.h>

ScDLManager::ScDLManager(QObject *parent)
	: QObject(parent)
{
	dlID=0;
	thread=new ScDLThread();
	connect(thread, SIGNAL(received(const QString &)), this, SLOT(dlReceived(const QString&)));
	connect(thread, SIGNAL(failed(const QString &)), this, SLOT(dlFailed(const QString&)));
	connect(thread, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(thread, SIGNAL(finished()), this, SLOT(moveFinishedDownloads()));
}

ScDLManager::~ScDLManager()
{
	// Per Qt doc, deleting a running thread will probably result in a program crash.
	if (thread && !thread->isRunning())
		delete thread;
}

void ScDLManager::addURL(const QUrl &url, bool overwrite, const QString& downloadLocation, const QString& destinationLocation)
{
	DownloadData d;
	d.id=dlID++;
	d.name=url.fileName();
	d.url=url;
	d.downloadLocation=downloadLocation;
	d.destinationLocation=destinationLocation;
	d.state=DownloadData::New;
	fileList.append(d);

	thread->addURL(url, overwrite, downloadLocation, destinationLocation);
}

void ScDLManager::addURL(const QString &url, bool overwrite, const QString &downloadLocation, const QString& destinationLocation)
{
	DownloadData d;
	d.id=dlID++;
	d.name=QUrl(url).fileName();
	d.url=url;
	d.downloadLocation=downloadLocation;
	d.destinationLocation=destinationLocation;
	d.state=DownloadData::New;
	fileList.append(d);

	thread->addURL(QUrl(url), overwrite, downloadLocation, destinationLocation);
}

void ScDLManager::addURLs(const QStringList &urlList, bool overwrite, const QString &downloadLocation, const QString& destinationLocation)
{
	foreach(QString s, urlList)
	{
		DownloadData d;
		d.id=dlID++;
		d.name=QUrl(s).fileName();
		d.url=s;
		d.downloadLocation=downloadLocation;
		d.destinationLocation=destinationLocation;
		d.state=DownloadData::New;
		fileList.append(d);
	}
	thread->addURLs(urlList, overwrite, downloadLocation, destinationLocation);
}

void ScDLManager::startDownloads()
{
	qDebug()<<"Manager starting downloads...";
	thread->startDownloads();
}

void ScDLManager::dlReceived(const QString& t)
{
	emit fileReceived(t);
	qDebug()<<"File Received:"<<t;
	QMutableListIterator<DownloadData> i(fileList);
	while (i.hasNext())
	{
		i.next();
		//if (d.state==DownloadData::Started && d.downloadLocation+d.name==t)
		if (i.value().downloadLocation+i.value().name==t)
		{
			qDebug()<<"success"<<i.value().downloadLocation+i.value().name<<t;
			i.value().state=DownloadData::Successful;
			break;
		}
	}
}

void ScDLManager::dlFailed(const QString& t)
{
	emit fileFailed(t);
	qDebug()<<"File Failed:"<<t;
	QMutableListIterator<DownloadData> i(fileList);
	while (i.hasNext())
	{
		i.next();
		//if (d.state==DownloadData::Started && d.downloadLocation+d.name==t)
		if (i.value().downloadLocation+i.value().name==t)
		{
			qDebug()<<"fail"<<i.value().downloadLocation+i.value().name<<t;
			i.value().state=DownloadData::Failed;
			break;
		}
	}
}

void ScDLManager::moveFinishedDownloads()
{
	QMutableListIterator<DownloadData> i(fileList);
	while (i.hasNext())
	{
		i.next();
		qDebug()<<"moveFinishedDownloads"<<i.value().name<<i.value().url;
		switch (i.value().state)
		{
			case DownloadData::Successful:
				{
					if (i.value().downloadLocation==i.value().destinationLocation)
						qDebug()<<i.value().name<<"is in"<<i.value().downloadLocation<<"which is the same as"<<i.value().destinationLocation;
					else
						qDebug()<<"Need to move"<<i.value().name<<"from"<<i.value().downloadLocation<<"to"<<i.value().destinationLocation;
				}
				break;
			case DownloadData::Failed:
				qDebug()<<i.value().name<<"failed :(.";
				break;
			default:
				qDebug()<<"case d state default"<<i.value().url;
				break;
		}
	}

	/*
	QStringList allFileList;
	foreach(DictData d, fileList)
	{
		allFileList += d.files.split(";", QString::SkipEmptyParts);
	}
	// Move downloaded files to destination
	foreach(DictData d, fileList)
	{
		QString basename = QFileInfo(d.url).fileName();
		QString filename = downloadLocation+basename;
		QStringList files = d.files.split(";", QString::SkipEmptyParts);
		QString affixFile = affixFileName(files);
		QString dictFile  = dictFileName(files);
		//qDebug()<<filename;
		if (d.filetype=="zip")
		{
			//qDebug()<<"zip data found"<<filename;
			ScZipHandler* fun = new ScZipHandler();
			if (fun->open(filename))
			{
				foreach (QString s, files)
				{
					//qDebug()<<"Unzipping"<<userDictDir+s;
					fun->extract(s, userDictDir);
					allFileList.removeOne(s);
				}
			}
			delete fun;
		}
		if (d.filetype=="plain")
		{
			foreach (QString s, files)
			{
				//qDebug()<<"plain data found"<<downloadLocation<<userDictDir<<s;
				QString dstName = s;
				if (dstName == affixFile)
					dstName = QFileInfo(downloadLocation+dictFile).baseName() + ".aff";
				allFileList.removeOne(s);
				if (allFileList.contains(s))
				{
					copyFile(downloadLocation+s, userDictDir+dstName);
					continue;
				}
				moveFile(downloadLocation+s, userDictDir+dstName);
			}
		}
	}
	*/
}
