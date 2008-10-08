/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QDateTime>
#include <QFileInfo>
#include <QList>
#include <QMap>
#include <QObject>
#include <QTimer>

#include "scribusapi.h"

class SCRIBUS_API FileWatcher : public QObject
{
	Q_OBJECT

public:
	FileWatcher(QObject* parent);
	~FileWatcher();
	bool isActive();
	// Set the timer length in milliseconds
	void setTimeOut(const int newTimeOut, const bool restartTimer=false);
	// Get the timer length
	int timeOut() const;
	QList<QString> files();
	
public slots:
	//Add a file to the watch list for monitoring
	void addFile(QString fileName);
	//Remove a file from the watch list
	void removeFile(QString fileName);
	//Add a directory to the watch list for monitoring
	void addDir(QString fileName);
	//Remove a directory from the watch list
	void removeDir(QString fileName);
	//Start the watcher's timer for file monitoring
	void start();
	//Stop the watcher's timer
	void stop();
	//Force a scan of the watched item list
	void forceScan();

private:
	struct fileMod
	{
		QFileInfo info;
		QDateTime timeInfo;
		int pendingCount;
		bool pending;
		int refCount;
	};
	QMap<QString, fileMod> watchedFiles;
	QTimer* watchTimer;
	bool blockAddRemove;
	bool stopped;
	bool dying;
	int m_timeOut; // milliseconds

private slots:
	void checkFiles();

signals:
	void fileChanged(QString);
	void fileDeleted(QString);
	void dirChanged(QString);
	void dirDeleted(QString);
	void statePending(QString);

};

#endif
