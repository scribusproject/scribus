#include "filewatcher.h"
#include "filewatcher.moc"
#include <unistd.h>

FileWatcher::FileWatcher( QWidget* parent) : QObject(parent)
{
	watchedFiles.clear();
	watchTimer = new QTimer(this);
	connect(watchTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));
	watchTimer->start(10000, true);
	blockAddRemove = false;
}

FileWatcher::~FileWatcher()
{
	watchTimer->stop();
	disconnect(watchTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));
	watchedFiles.clear();
	delete watchTimer;
}

void FileWatcher::addFile(QString fileName)
{
	while (blockAddRemove)
	{
		usleep(500);
	}
	struct fileMod fi;
	fi.info = QFileInfo(fileName);
	fi.timeInfo = fi.info.lastModified();
	watchedFiles.insert(fileName, fi);
}

void FileWatcher::removeFile(QString fileName)
{
	while (blockAddRemove)
	{
		usleep(500);
	}
	watchedFiles.remove(fileName);
}

QValueList<QString> FileWatcher::files()
{
	return watchedFiles.keys();
}

bool FileWatcher::isActive()
{
	return blockAddRemove;
}

void FileWatcher::checkFiles()
{
	blockAddRemove = true;
	watchTimer->stop();
	QDateTime time;
	QMap<QString, fileMod>::Iterator it;
	for ( it = watchedFiles.begin(); it != watchedFiles.end(); ++it )
	{
		it.data().info.refresh();
		if (!it.data().info.exists())
		{
			emit fileDeleted(it.key());
			watchedFiles.remove(it);
			continue;
		}
		time = it.data().info.lastModified();
		if (time != it.data().timeInfo)
		{
			uint sizeo = it.data().info.size();
			usleep(100);
			it.data().info.refresh();
			uint sizen = it.data().info.size();
			while (sizen != sizeo)
			{
				sizeo = sizen;
				usleep(100);
				it.data().info.refresh();
				sizen = it.data().info.size();
			}
			it.data().timeInfo = time;
			emit fileChanged(it.key());
		}
	}
	blockAddRemove = false;
	watchTimer->start(10000, true);
}

