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

#include "scribusdoc.h"
class SCRIBUS_API FileWatcher : public QObject
{
	Q_OBJECT

public:
	FileWatcher(QObject* parent);
	~FileWatcher() override;

	// Get if file check loop is running
	bool isActive() const;
	// Check if a file is currently being watched
	bool isWatching(const QString& fileName) const;
	// Set the timer length in milliseconds
	void setTimeOut(const int newTimeOut, const bool restartTimer = false);
	// Get the timer length
	int timeOut() const;
	// Get list of watched files and directories
	QList<QString> files() const;
	
public slots:
	//Add a file to the watch list for monitoring
	void addFile(const QString& fileName, bool fast = false, ScribusDoc* doc = nullptr); //see struct note for doc var
	//Remove a file from the watch list
	void removeFile(const QString& fileName);
	//Add a directory to the watch list for monitoring
	void addDir(const QString& fileName, bool fast = false);
	//Remove a directory from the watch list
	void removeDir(const QString& fileName);
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
		int pendingCount {};
		bool pending {};
		int refCount {};
		bool isDir {};
		bool fast {};
		ScribusDoc* doc{}; //CB Added as part of #9845 but unused for now, we could avoid scanning docs in updatePict() if we used this

	};

	enum StateFlags
	{
		AddRemoveBlocked  = 1,
		FileCheckRunning  = 2,
		StopRequested = 4,
		TimerStopped  = 8,
		Dying = 16,
		FileCheckMustStop = 20 //StopRequested + Dying
	};

	QMap<QString, fileMod> m_watchedFiles;
	QTimer* m_watchTimer { nullptr };
	int  m_stateFlags { 0 };
	int  m_timeOut { 10000 }; // milliseconds

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
