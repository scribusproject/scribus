/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UPGRADECHECKER_H
#define UPGRADECHECKER_H

#include <QFile>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QTextBrowser>

/**
	@author Craig Bradney <cbradney@zip.com.au>
*/
class UpgradeChecker : public QObject
{
	Q_OBJECT
	
public:
	UpgradeChecker();
	~UpgradeChecker();
	
	void fetch();
	bool process();
	void show(bool error);
	QStringList upgradeData();
	
private slots:
	void downloadFinished();
	void downloadReadyRead();
	
protected:
	void init();
	virtual void outputText(QString text, bool noLineFeed=false);
	void reportError(const QString& s);
	bool m_writeToConsole;
	QString m_version;
	QString m_stability;
	uint major, minor, m_revision1, m_revision2;
	bool m_isCVS;
	QString m_platform;
	QStringList m_updates;
	QString m_tempFile;
	bool m_fin;
	QNetworkAccessManager* m_networkManager;
	QNetworkReply *m_networkReply;
	QString m_message;
	QFile *m_file;
	bool m_errorReported;
};

class UpgradeCheckerGUI : public UpgradeChecker
{
	Q_OBJECT
	
public:
	UpgradeCheckerGUI(QTextBrowser *tb=0);
	~UpgradeCheckerGUI();
	
protected:
	virtual void outputText(QString text, bool noLineFeed=false);
	QTextBrowser *m_outputWidget;
};


#endif
