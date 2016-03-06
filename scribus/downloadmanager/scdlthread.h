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

		void addURL(const QUrl &url, bool overwrite, const QString& location, const QString& destinationLocation);
		void addURLs(const QStringList &urlList, bool overwrite, const QString& location, const QString& destinationLocation);
		void startDownloads();
		QString saveFileName(const QUrl &url, const QString &location, bool overwrite);

	signals:
		void finished();
		void runSignal();
		void fileStarted(const QString &);
		void fileReceived(const QString &);
		void fileFailed(const QString &);
		void fileDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

	private slots:
		void startNextDownload();
		void downloadFinished();
		void replyFinished(QNetworkReply* reply);
		void downloadReadyRead();
		void runSlot();

	private:
		bool urlOK(QUrl url);
		QStringList m_urlList;
		QFile m_output;
		QQueue<QPair<QUrl, QString> > m_downloadQueue;
		int m_downloadedCount;
		int m_totalCount;
		QNetworkReply *m_currentDownload;
		QNetworkAccessManager m_manager;
};
#endif
