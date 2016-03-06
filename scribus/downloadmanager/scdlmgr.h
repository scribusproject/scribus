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

		void addURL(const QUrl &url, bool overwrite, const QString &downloadLocation, const QString& destinationLocation, const QString& destinationName="");
		void addURL(const QString &url, bool overwrite, const QString &downloadLocation, const QString& destinationLocation, const QString& destinationName="");
		void addURLs(const QStringList &urlList, bool overwrite, const QString &downloadLocation, const QString& destinationLocation);
		void startDownloads();

	public slots:
		void dlStarted(const QString& filename);
		void dlReceived(const QString& filename);
		void dlFailed(const QString& filename);

	protected slots:
		void moveFinishedDownloads();

	signals:
		void finished();
		void fileReceived(const QString& t);
		void fileFailed(const QString& t);
		void fileDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

	private:
		ScDLThread *m_thread;
		int m_dlID;
		QList <DownloadData> m_fileList;
};

#endif
