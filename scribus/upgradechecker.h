/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UPGRADECHECKER_H
#define UPGRADECHECKER_H

#include <QFile>
#include <QObject>
#include <QString>
#include <QStringList>
class QHttp;
class QHttpResponseHeader;

class ScTextBrowser;

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
	
public slots:
	void abort();
	
private slots:
	void requestFinished(int requestId, bool error);
	void responseHeaderReceived(const QHttpResponseHeader &responseHeader);
	void done(bool);
	
protected:
	void init();
	virtual void outputText(QString text, bool noLineFeed=false);
	void reportError(const QString& s);
	bool writeToConsole;
	QString version;
	QString stability;
	uint major, minor, revision1, revision2;
	bool isCVS;
	QString platform;
	QStringList updates;
	QString tempFile;
	bool fin;
	QHttp* getter;
	int getterID;
	QString message;
	int httpGetId;
	bool httpRequestAborted;
	QFile *rcvdFile;
	bool errorReported;
	bool userAbort;
};

class UpgradeCheckerGUI : public UpgradeChecker
{
	Q_OBJECT
	
public:
	UpgradeCheckerGUI(ScTextBrowser *tb=0);
	~UpgradeCheckerGUI();
	
protected:
	virtual void outputText(QString text, bool noLineFeed=false);
	ScTextBrowser *outputWidget;
};


#endif
