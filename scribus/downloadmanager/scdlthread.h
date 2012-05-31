#ifndef SCDLTHREAD_H
#define SCDLTHREAD_H

#include <QThread>
#include <QUrl>
#include <QStringList>
#include <QPair>
#include <QString>
#include <QQueue>
#include <QFile>
#include <QNetworkAccessManager>

class ScDLThread : public QThread
{
	Q_OBJECT
	public:
		ScDLThread(QObject * parent = 0);
		~ScDLThread();
		void run();

		void addURL(const QUrl &url, bool overwrite, const QString& location="");
		void addURLs(const QStringList &urlList, bool overwrite, const QString& location="");
		void startDownloads();
		QString saveFileName(const QUrl &url, const QString &location, bool overwrite);

	signals:
		void finished();
		void runSignal();
		void received(const QString &);

	private slots:
		void startNextDownload();
		void downloadFinished();
		void downloadReadyRead();
		void runSlot();

	private:
		bool urlOK(QUrl url);
		QStringList m_urlList;
		QFile output;
		QQueue<QPair<QUrl, QString> > downloadQueue;
		int downloadedCount;
		int totalCount;
		QNetworkReply *currentDownload;
		QNetworkAccessManager manager;
};
#endif
