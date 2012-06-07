
#include "scdlmgr.h"
#include "scdlthread.h"
#include "scpaths.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>


ScDLThread::ScDLThread(QObject *parent) : QThread(parent),
	downloadedCount(0), totalCount(0)
{
	connect(this, SIGNAL(runSignal()), this, SLOT(runSlot()));
}

ScDLThread::~ScDLThread()
{
}

void ScDLThread::run()
{
	emit(runSignal());
}

void ScDLThread::addURL(const QUrl &url, bool overwrite, const QString& location)
{
	qDebug()<<"ScDLThread::addURL:"<<url;
	if (!urlOK(url))
		return;
	QString l(QDir::cleanPath(location));
	if (!l.endsWith("/"))
		l += "/";
	downloadQueue.enqueue(qMakePair(url, l));
	++totalCount;
}

void ScDLThread::addURLs(const QStringList &urlList, bool overwrite, const QString& location)
{
	qDebug()<<"ScDLThread::addURLs:"<<urlList;
	m_urlList=urlList;
	QString l(QDir::cleanPath(location));
	if (!l.endsWith("/"))
		l += "/";
	foreach (QString u, m_urlList)
	{
		QUrl url(u);
		if (!urlOK(u))
			return;
		downloadQueue.enqueue(qMakePair(url, l));
		++totalCount;
	}
}

void ScDLThread::startDownloads()
{
	if (downloadQueue.isEmpty())
	{
		qDebug()<<"No more downloads left";
		emit finished();
		return;
	}
	start();
}

QString ScDLThread::saveFileName(const QUrl &url, const QString& location, bool overwrite)
{
	QDir d(location);
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();
	if (location.isEmpty() || !d.exists())
	{
		if (basename.isEmpty())
			basename = "download";
		basename = ScPaths::downloadDir() + basename;
	}
	else
	{
		basename = location + basename;
	}
	if (!overwrite && QFile::exists(basename))
	{
		// already exists, don't overwrite
		int i = 0;
		basename += '.';
		while (QFile::exists(basename + QString::number(i)))
			++i;
		basename += QString::number(i);
	}
	return basename;
}

void ScDLThread::startNextDownload()
{
	if (downloadQueue.isEmpty())
	{
		qDebug()<<downloadedCount<<"/"<<totalCount<<"files downloaded successfully";
		downloadedCount=totalCount=0;
		emit finished();
		return;
	}
	QPair<QUrl, QString> urlPair=downloadQueue.dequeue();

	QString filename = saveFileName(urlPair.first, urlPair.second, true);
	if (filename.isEmpty())
	{
		qDebug()<<"File name empty for url:"<<urlPair.first.toEncoded().constData();
		return;
	}
	output.setFileName(filename);
	if (!output.open(QIODevice::WriteOnly))
	{
		qDebug()<<"Problem opening save file '"<<qPrintable(filename)<<"' for download '"
			   <<urlPair.first.toEncoded().constData()<<"': "<<qPrintable(output.errorString());

		startNextDownload();
		return;
	}

	QNetworkRequest request(urlPair.first);
	currentDownload = manager.get(request);
	connect(currentDownload, SIGNAL(finished()), SLOT(downloadFinished()));
	connect(currentDownload, SIGNAL(readyRead()), SLOT(downloadReadyRead()));

	qDebug()<<"Downloading:"<<urlPair.first.toEncoded().constData();
}

void ScDLThread::downloadFinished()
{
	output.close();

	if (currentDownload->error())
		qDebug()<<"Failed: "<<qPrintable(currentDownload->errorString());
	else
	{
		printf("Succeeded.\n");
		qDebug()<<"Saving file:"<<qPrintable(output.fileName());
		++downloadedCount;
	}
	emit (received(output.fileName()));
	currentDownload->deleteLater();
	startNextDownload();
}


void ScDLThread::downloadReadyRead()
{
	output.write(currentDownload->readAll());
}

void ScDLThread::runSlot()
{
	startNextDownload();
}

bool ScDLThread::urlOK(QUrl url)
{
	//TODO: Add some more URL checks
	if (!url.isValid() || url.isEmpty() || url.host().isEmpty())
	{
		qDebug()<<"URL invalid:"<<url;
		return false;
	}
	return true;
}



