/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scconfig.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <QList>
#endif

#include <QStringList>

#include "filewatcher.h"

FileWatcher::FileWatcher( QObject* parent) : QObject(parent)
{
	m_timeOut=10000;
	watchedFiles.clear();
	watchTimer = new QTimer(this);
	watchTimer->setSingleShot(true);
	connect(watchTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));
	watchTimer->start(m_timeOut);
	blockAddRemove = false;
	stopped = false;
	dying = false; 
}

FileWatcher::~FileWatcher()
{
	dying = true;
	watchTimer->stop();
	disconnect(watchTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));
	if (!blockAddRemove)
		watchedFiles.clear();
	delete watchTimer;
}

void FileWatcher::setTimeOut(const int newTimeOut, const bool restartTimer)
{
	m_timeOut=newTimeOut;
	if (restartTimer)
		start();
}

int FileWatcher::timeOut() const
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
		fi.pendingCount = 0;
		fi.pending = false;
		watchedFiles.insert(fileName, fi);
	}
	if (!stopped)
		watchTimer->start(m_timeOut);
}

void FileWatcher::removeFile(QString fileName)
{
	watchTimer->stop();
	if (watchedFiles.contains(fileName))
		watchedFiles.remove(fileName);
	if (!stopped)
		watchTimer->start(m_timeOut);
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
	stopped = false;
	watchTimer->start(m_timeOut);
}

void FileWatcher::stop()
{
	watchTimer->stop();
	stopped = true;
}

void FileWatcher::forceScan()
{
	checkFiles();
}

bool FileWatcher::isActive()
{
	return blockAddRemove;
}

QList<QString> FileWatcher::files()
{
	return watchedFiles.keys();
}

void FileWatcher::checkFiles()
{
	blockAddRemove = true;
	watchTimer->stop();
	stopped = true;
	QDateTime time;
	QStringList toRemove;
	
	QMap<QString, fileMod>::Iterator it;
	for ( it = watchedFiles.begin(); !dying && it != watchedFiles.end(); ++it )
	{
		it.value().info.refresh();
		if (!it.value().info.exists())
		{
			if (dying)
				break;
			if (!it.value().pending)
			{
				it.value().pendingCount = 5;
				it.value().pending = true;
				emit statePending(it.key());
				continue;
			}
			else
			{
				if (it.value().pendingCount != 0)
				{
					it.value().pendingCount--;
					continue;
				}
				else
				{
					it.value().pending = false;
					if (it.value().info.isDir())
						emit dirDeleted(it.key());
					else
						emit fileDeleted(it.key());
					if (dying)
						break;
					toRemove.append(it.key());
					continue;
				}
			}
		}
		else
		 {
			it.value().pending = false;
			time = it.value().info.lastModified();
			if (time != it.value().timeInfo)
			{
				if (it.value().info.isDir())
				{
					if (!dying)
						emit dirChanged(it.key());
				}
				else
				{
					uint sizeo = it.value().info.size();
			#ifndef _WIN32
					usleep(100);
			#else
					Sleep(1);
			#endif
					it.value().info.refresh();
					uint sizen = it.value().info.size();
					while (sizen != sizeo)
					{
						sizeo = sizen;
				#ifndef _WIN32
						usleep(100);
				#else
						Sleep(1);
				#endif
						it.value().info.refresh();
						sizen = it.value().info.size();
					}
					it.value().timeInfo = time;
					if (dying)
						break;
					emit fileChanged(it.key());
				}
			}
		}
	}
	if (dying)
		watchedFiles.clear();
	else
	{
		for( int i=0; i<toRemove.count(); ++i)
			watchedFiles.remove(toRemove[i]);
		blockAddRemove = false;
		stopped = false;
		watchTimer->start(m_timeOut);
	}
}

