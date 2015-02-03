#ifndef SCDLMANAGER_H
#define SCDLMANAGER_H

#include <QDialog>
#include <QFile>
#include <QList>
#include <QNetworkAccessManager>
#include <QObject>
#include <QQueue>
#include <QStringList>
#include <QThread>
#include <QTime>
#include <QUrl>

#include "scdlthread.h"
#include "scribusstructs.h"

class ScDLManager: public QObject
{
	Q_OBJECT
	public:
		ScDLManager(QObject *parent = 0);
		~ScDLManager();
//TODO: Add download groups so different parts of Scribus can be downloading at the same time

		void addURL(const QUrl &url, bool overwrite, const QString &downloadLocation, const QString& destinationLocation);
		void addURL(const QString &url, bool overwrite, const QString &downloadLocation, const QString& destinationLocation);
		void addURLs(const QStringList &urlList, bool overwrite, const QString &downloadLocation, const QString& destinationLocation);
		void startDownloads();

	public slots:
		void dlReceived(const QString& t);
		void dlFailed(const QString& t);

	protected slots:
		void moveFinishedDownloads();

	signals:
		void finished();
		void fileReceived(const QString& t);
		void fileFailed(const QString& t);

	private:
		ScDLThread *thread;
		int dlID;
		QList <DownloadData> fileList;
};

#endif
