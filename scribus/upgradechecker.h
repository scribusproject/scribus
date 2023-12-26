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
	
	void fetch();
	bool process();
	void show(bool error);
	QStringList upgradeData() const;
	
private slots:
	void downloadFinished();
	void downloadReadyRead();
	
protected:
	virtual void outputText(const QString& text, bool noLineFeed = false);
	void reportError(const QString& s);

	QFile *m_file {nullptr};
	QNetworkAccessManager* m_networkManager {nullptr};
	QNetworkReply *m_networkReply {nullptr};
	QString m_message;
	QString m_platform;
	QString m_stability;
	QString m_tempFile;
	QString m_version;
	QStringList m_updates;
	bool m_errorReported {false};
	bool m_fin {false};
	bool m_isSVN {false};
	bool m_writeToConsole {true};
	uint major, minor, m_patchLevel, m_versionSuffix;
};

class UpgradeCheckerGUI : public UpgradeChecker
{
	Q_OBJECT
	
public:
	UpgradeCheckerGUI(QTextBrowser *tb=0);
	~UpgradeCheckerGUI();
	
protected:
	virtual void outputText(const QString& text, bool noLineFeed = false);
	QTextBrowser *m_outputWidget;
};


#endif
