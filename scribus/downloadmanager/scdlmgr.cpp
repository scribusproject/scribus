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

void ScDLManager::addURL(const QUrl &url, bool overwrite, const QString& location)
{
	thread->addURL(url, overwrite, location);
}

void ScDLManager::addURL(const QString &url, bool overwrite, const QString &location)
{
	thread->addURL(QUrl(url), overwrite, location);
}

void ScDLManager::addURLs(const QStringList &urlList, bool overwrite, const QString &location)
{
	thread->addURLs(urlList, overwrite, location);
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
