/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "filewatcher.h"
#include "filewatcher.moc"

#include "scconfig.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

FileWatcher::FileWatcher( QWidget* parent) : QObject(parent)
{
	m_timeOut=10000;
	watchedFiles.clear();
	watchTimer = new QTimer(this);
	connect(watchTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));
	watchTimer->start(m_timeOut, true);
	blockAddRemove = false;
}

FileWatcher::~FileWatcher()
{
	watchTimer->stop();
	disconnect(watchTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));
	watchedFiles.clear();
	delete watchTimer;
}

void FileWatcher::setTimeOut(const int newTimeOut, const bool restartTimer)
{
	m_timeOut=newTimeOut;
	if (restartTimer)
		start();
}

const int FileWatcher::timeOut()
{
	return m_timeOut;
}

void FileWatcher::addFile(QString fileName)
{
	watchTimer->stop();
	if (!watchedFiles.contains(fileName))
	{
		struct fileMod fi;
		fi.info = QFileInfo(fileName);
		fi.timeInfo = fi.info.lastModified();
		watchedFiles.insert(fileName, fi);
	}
	watchTimer->start(m_timeOut, true);
}

void FileWatcher::removeFile(QString fileName)
{
	watchTimer->stop();
	watchedFiles.remove(fileName);
	watchTimer->start(m_timeOut, true);
}

void FileWatcher::addDir(QString fileName)
{
	addFile(fileName);
}

void FileWatcher::removeDir(QString fileName)
{
	removeFile(fileName);
}

void FileWatcher::start()
{
	watchTimer->stop();
	watchTimer->start(m_timeOut, true);
}

void FileWatcher::stop()
{
	watchTimer->stop();
}

void FileWatcher::forceScan()
{
	checkFiles();
}

bool FileWatcher::isActive()
{
	return blockAddRemove;
}

QValueList<QString> FileWatcher::files()
{
	return watchedFiles.keys();
}

void FileWatcher::checkFiles()
{
	blockAddRemove = true;
	watchTimer->stop();
	QDateTime time;
	QStringList toRemove;
	QMap<QString, fileMod>::Iterator it;
	for ( it = watchedFiles.begin(); it != watchedFiles.end(); ++it )
	{
		it.data().info.refresh();
		if (!it.data().info.exists())
		{
			if (it.data().info.isDir())
				emit dirDeleted(it.key());
			else
				emit fileDeleted(it.key());
			toRemove.append(it.key());
			continue;
		}
		time = it.data().info.lastModified();
		if (time != it.data().timeInfo)
		{
			if (it.data().info.isDir())
				emit dirChanged(it.key());
			else
			{
				uint sizeo = it.data().info.size();
			#ifndef _WIN32
				usleep(100);
			#else
				Sleep(1);
			#endif
				it.data().info.refresh();
				uint sizen = it.data().info.size();
				while (sizen != sizeo)
				{
					sizeo = sizen;
				#ifndef _WIN32
					usleep(100);
				#else
					Sleep(1);
				#endif
					it.data().info.refresh();
					sizen = it.data().info.size();
				}
				it.data().timeInfo = time;
				emit fileChanged(it.key());
			}
		}
	}
	for(unsigned int i=0; i<toRemove.count(); ++i)
		watchedFiles.remove(toRemove[i]);
	blockAddRemove = false;
	watchTimer->start(m_timeOut, true);
}

