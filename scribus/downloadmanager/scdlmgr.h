#ifndef SCDLMANAGER_H
#define SCDLMANAGER_H

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
#include <QDialog>

class ScDLManager: public QObject
{
	Q_OBJECT
	public:
		ScDLManager(QObject *parent = 0);
//TODO: Add download groups so different parts of Scribus can be downloading at the same time

		void addURL(const QUrl &url, bool overwrite, const QString &location="");
		void addURL(const QString &url, bool overwrite, const QString &location="");
		void addURLs(const QStringList &urlList, bool overwrite, const QString &location="");
		void startDownloads();

	public slots:
		void updateText(const QString& t);

	signals:
		void finished();
		void fileReceived(const QString& t);

	private:
		ScDLThread *thread;
};

#endif
