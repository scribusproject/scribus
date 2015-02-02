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
	thread=new ScDLThread();
	connect(thread, SIGNAL(received(const QString &)), this, SLOT(updateText(const QString&)));
	connect(thread, SIGNAL(finished()), this, SIGNAL(finished()));
}

ScDLManager::~ScDLManager()
{
	// Per Qt doc, deleting a running thread will probably result in a program crash.
	if (thread && !thread->isRunning())
		delete thread;
}

void ScDLManager::addURL(const QUrl &url, bool overwrite, const QString& downloadLocation, const QString& destinationLocation)
{
	thread->addURL(url, overwrite, downloadLocation, destinationLocation);
}

void ScDLManager::addURL(const QString &url, bool overwrite, const QString &downloadLocation, const QString& destinationLocation)
{
	thread->addURL(QUrl(url), overwrite, downloadLocation, destinationLocation);
}

void ScDLManager::addURLs(const QStringList &urlList, bool overwrite, const QString &downloadLocation, const QString& destinationLocation)
{
	thread->addURLs(urlList, overwrite, downloadLocation, destinationLocation);
}

void ScDLManager::startDownloads()
{
	qDebug()<<"Manager starting downloads...";
	thread->startDownloads();
}

void ScDLManager::updateText(const QString& t)
{
	emit fileReceived(t);
	qDebug()<<"File Received:"<<t;
}

void ScDLManager::moveFinishedDownloads()
{
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
