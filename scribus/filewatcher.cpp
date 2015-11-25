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

#if defined(_WIN32) && !defined(usleep)
#include <windows.h>
#define usleep(t) Sleep((t > 1000) ? (t / 1000) : 1)
#endif

#include <QDebug>
#include <QStringList>

#include "filewatcher.h"

FileWatcher::FileWatcher( QObject* parent) : QObject(parent)
{
	m_stateFlags = 0;
	m_timeOut = 10000;
	m_watchedFiles.clear();
	m_watchTimer = new QTimer(this);
	m_watchTimer->setSingleShot(true);
	connect(m_watchTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));
	m_watchTimer->start(m_timeOut);
}

FileWatcher::~FileWatcher()
{
	m_stateFlags |= Dying;
	this->stop();
	disconnect(m_watchTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));
	if (!(m_stateFlags & AddRemoveBlocked))
		m_watchedFiles.clear();
	delete m_watchTimer;
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

void FileWatcher::addFile(QString fileName, bool fast, ScribusDoc* doc)
{
	if (fileName.isEmpty())
	{
		qDebug()<<"Attempt to add empty filename to filewatcher";
		return;
	}
	m_watchTimer->stop();
	if (!m_watchedFiles.contains(fileName))
	{
		struct fileMod fi;
		fi.info = QFileInfo(fileName);
		fi.timeInfo = fi.info.lastModified();
		fi.pendingCount = 0;
		fi.pending = false;
		fi.refCount = 1;
		fi.isDir = fi.info.isDir();
		fi.fast = fast;
		fi.doc = doc;
		m_watchedFiles.insert(fileName, fi);
	}
	else
		m_watchedFiles[fileName].refCount++;
	if (!(m_stateFlags & TimerStopped))
		m_watchTimer->start(m_timeOut);
}

void FileWatcher::removeFile(QString fileName)
{
	m_watchTimer->stop();
	if (m_watchedFiles.contains(fileName))
	{
		m_watchedFiles[fileName].refCount--;
		if (m_watchedFiles[fileName].refCount == 0)
			m_watchedFiles.remove(fileName);
	}
	if (!(m_stateFlags & TimerStopped))
		m_watchTimer->start(m_timeOut);
}

void FileWatcher::addDir(QString fileName, bool fast)
{
	addFile(fileName, fast);
}

void FileWatcher::removeDir(QString fileName)
{
	removeFile(fileName);
}

void FileWatcher::start()
{
	m_watchTimer->stop();
	m_stateFlags &= ~TimerStopped;
	m_watchTimer->start(m_timeOut);
}

void FileWatcher::stop()
{
	m_watchTimer->stop();
	m_stateFlags |= StopRequested;
	while ((m_stateFlags & FileCheckRunning))
		usleep(100);
	m_stateFlags &= ~StopRequested;
	m_stateFlags |= TimerStopped;
}

void FileWatcher::forceScan()
{
	checkFiles();
}

bool FileWatcher::isActive()
{
	return (m_stateFlags & AddRemoveBlocked);
}

QList<QString> FileWatcher::files()
{
	return m_watchedFiles.keys();
}

void FileWatcher::checkFiles()
{
	m_stateFlags |= AddRemoveBlocked;
	m_stateFlags |= FileCheckRunning;
	m_watchTimer->stop();
	m_stateFlags |= TimerStopped;
	QDateTime time;
	QStringList toRemove;
	
	QMap<QString, fileMod>::Iterator it;
//	qDebug()<<files();
	for ( it = m_watchedFiles.begin(); !(m_stateFlags & FileCheckMustStop) && it != m_watchedFiles.end(); ++it )
	{
		it.value().info.refresh();
		if (!it.value().info.exists())
		{
			if (m_stateFlags & FileCheckMustStop)
				break;
			if (!it.value().pending)
			{
				if (it.value().fast)
				{
					if (it.value().isDir)
						emit dirDeleted(it.key());
					else
						emit fileDeleted(it.key());
					if (m_stateFlags & FileCheckMustStop)
						break;
					it.value().refCount--;
					if (it.value().refCount == 0)
						toRemove.append(it.key());
					continue;
				}
				else
				{
					it.value().pendingCount = 5;
					it.value().pending = true;
					emit statePending(it.key());
					continue;
				}
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
					if (it.value().isDir)
						emit dirDeleted(it.key());
					else
						emit fileDeleted(it.key());
					if (m_stateFlags & FileCheckMustStop)
						break;
					it.value().refCount--;
					if (it.value().refCount == 0)
						toRemove.append(it.key());
					continue;
				}
			}
		}
		else
		{
			//qDebug()<<it.key();
			it.value().pending = false;
			time = it.value().info.lastModified();
			if (time != it.value().timeInfo)
			{
//				qDebug()<<"Times different: last modified:"<<time<<"\t recorded time:"<<it.value().timeInfo;
				if (it.value().isDir)
				{
//					qDebug()<<"dir, ignoring"<<it.key();
					it.value().timeInfo = time;
					if (!(m_stateFlags & FileCheckMustStop))
						emit dirChanged(it.key());
				}
				else
				{
					uint sizeo = it.value().info.size();
					usleep(100);
					it.value().info.refresh();
					uint sizen = it.value().info.size();
//					qDebug()<<"Size comparison"<<sizeo<<sizen<<it.key();
					while (sizen != sizeo)
					{
						sizeo = sizen;
						usleep(100);
						it.value().info.refresh();
						sizen = it.value().info.size();
					}
					it.value().timeInfo = time;
					if (m_stateFlags & FileCheckMustStop)
						break;
					emit fileChanged(it.key());
				}
			}
		}
	}
	if (m_stateFlags & Dying)
		m_watchedFiles.clear();
	else
	{
		for( int i=0; i<toRemove.count(); ++i)
			m_watchedFiles.remove(toRemove[i]);
		m_stateFlags &= ~AddRemoveBlocked;
		m_stateFlags &= ~TimerStopped;
		m_watchTimer->start(m_timeOut);
	}
	m_stateFlags &= ~FileCheckRunning;
}

