#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <qobject.h>
#include <qtimer.h>
#include <qmap.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <qvaluelist.h>

#include "scribusapi.h"

class SCRIBUS_API FileWatcher : public QObject
{
	Q_OBJECT

public:
	FileWatcher(QWidget* parent);
	~FileWatcher();
	bool isActive();
	// Set the timer length in milliseconds
	void setTimeOut(const int);
	// Get the timer length
	const int timeOut();
	QValueList<QString> files();
	
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
	};
	QMap<QString, fileMod> watchedFiles;
	QTimer* watchTimer;
	bool blockAddRemove;
	int m_timeOut; // milliseconds

private slots:
	void checkFiles();

signals:
	void fileChanged(QString);
	void fileDeleted(QString);
	void dirChanged(QString);
	void dirDeleted(QString);

};

#endif
