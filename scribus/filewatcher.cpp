/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scconfig.h"

#include <chrono>
#include <thread>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <QDebug>
#include <QDir>
#include <QStringList>

#include "filewatcher.h"

FileWatcher::FileWatcher( QObject* parent) : QObject(parent)
{
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

void FileWatcher::setTimeOut(int newTimeOut, bool restartTimer)
{
	m_timeOut = newTimeOut;
	if (restartTimer)
		start();
}

int FileWatcher::timeOut() const
{
	return m_timeOut;
}

void FileWatcher::addFile(const QString& fileName, bool fast, ScribusDoc* doc)
{
	if (fileName.isEmpty())
	{
		qDebug()<<"Attempt to add empty filename to filewatcher";
		return;
	}
	QString qtFileName = QDir::fromNativeSeparators(fileName);

	m_watchTimer->stop();
	if (!m_watchedFiles.contains(qtFileName))
	{
		struct fileMod fi;
		fi.info = QFileInfo(qtFileName);
		fi.timeInfo = fi.info.lastModified();
		fi.pendingCount = 0;
		fi.pending = false;
		fi.refCount = 1;
		fi.isDir = fi.info.isDir();
		fi.fast = fast;
		fi.doc = doc;
		m_watchedFiles.insert(qtFileName, fi);
	}
	else
		m_watchedFiles[qtFileName].refCount++;
	if (!(m_stateFlags & TimerStopped))
		m_watchTimer->start(m_timeOut);
}

void FileWatcher::removeFile(const QString& fileName)
{
	QString qtFileName = QDir::fromNativeSeparators(fileName);
	m_watchTimer->stop();
	if (m_watchedFiles.contains(qtFileName))
	{
		m_watchedFiles[qtFileName].refCount--;
		if (m_watchedFiles[qtFileName].refCount == 0)
			m_watchedFiles.remove(qtFileName);
	}
	if (!(m_stateFlags & TimerStopped))
		m_watchTimer->start(m_timeOut);
}

void FileWatcher::addDir(const QString& fileName, bool fast)
{
	addFile(fileName, fast);
}

void FileWatcher::removeDir(const QString& fileName)
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
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	m_stateFlags &= ~StopRequested;
	m_stateFlags |= TimerStopped;
}

void FileWatcher::forceScan()
{
	checkFiles();
}

bool FileWatcher::isActive() const
{
	return (m_stateFlags & AddRemoveBlocked);
}

bool FileWatcher::isWatching(const QString& fileName) const
{
	QString qtFileName = QDir::fromNativeSeparators(fileName);
	return m_watchedFiles.contains(qtFileName);
}

QList<QString> FileWatcher::files() const
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
				it.value().pendingCount = 5;
				it.value().pending = true;
				emit statePending(it.key());
				continue;
			}
			if (it.value().pendingCount != 0)
			{
				it.value().pendingCount--;
				continue;
			}
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
				qint64 sizeo = it.value().info.size();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				it.value().info.refresh();
				qint64 sizen = it.value().info.size();
				//					qDebug()<<"Size comparison"<<sizeo<<sizen<<it.key();
				while (sizen != sizeo)
				{
					sizeo = sizen;
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
	if (m_stateFlags & Dying)
		m_watchedFiles.clear();
	else
	{
		for (int i = 0; i<toRemove.count(); ++i)
			m_watchedFiles.remove(toRemove[i]);
		m_stateFlags &= ~AddRemoveBlocked;
		m_stateFlags &= ~TimerStopped;
		m_watchTimer->start(m_timeOut);
	}
	m_stateFlags &= ~FileCheckRunning;
}

