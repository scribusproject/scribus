#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <qobject.h>
#include <qtimer.h>
#include <qmap.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <qvaluelist.h>

class FileWatcher : public QObject
{
	Q_OBJECT

public:
	FileWatcher(QWidget* parent);
	~FileWatcher();
	void addFile(QString fileName);
	void removeFile(QString fileName);
	void addDir(QString fileName);
	void removeDir(QString fileName);
	void start();
	void stop();
	void forceScan();
	bool isActive();
	QValueList<QString> files();

private:
	struct fileMod
	{
		QFileInfo info;
		QDateTime timeInfo;
	};
	QMap<QString, fileMod> watchedFiles;
	QTimer* watchTimer;
	bool blockAddRemove;

private slots:
	void checkFiles();

signals:
	void fileChanged(QString);
	void fileDeleted(QString);
	void dirChanged(QString);
	void dirDeleted(QString);

};

#endif
