/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	copyright            : (C) 2010 by Marcus Holland-Moritz
	email                : scribus@mhxnet.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>
#include <QString>

#include "sclockedfile.h"
#include "scimagecachedir.h"
#include "scimagecachefile.h"
#include "scimagecachemanager.h"
#include "scpaths.h"

#define SC_DEBUG_FILE defined(DEBUG_SCIMAGECACHE)
#include "scdebug.h"

const QString ScImageCacheDir::accessFileName("access");

ScImageCacheDir::ScImageCacheDir(const QString & dir, ScImageCacheDir *parent, bool scanFiles, const QStringList & suffixList)
	: m_name(dir), m_suffix(suffixList), m_parent(parent), m_exists(false), m_lastAccessValid(false), m_dirs(0), m_files(0)
{
	while (m_name.endsWith('/'))
		m_name.chop(1);
	if (m_parent)
		m_parent->addDir(this);
	if (scanFiles)
	{
		m_files = new FileMap;
		Q_CHECK_PTR(m_files);
	}
}

ScImageCacheDir *ScImageCacheDir::newSubDir(const QString & dir, bool scanFiles, const QStringList & suffixList)
{
	ScImageCacheDir *d = new ScImageCacheDir(dir, this, scanFiles, suffixList);
	Q_CHECK_PTR(d);
	return d;
}

ScImageCacheDir::~ScImageCacheDir()
{
	if (m_dirs)
	{
		foreach (ScImageCacheDir *p, *m_dirs)
			delete p;
		delete m_dirs;
	}
	if (m_files)
	{
		foreach (ScImageCacheFile *p, *m_files)
			delete p;
		delete m_files;
	}
}

QString ScImageCacheDir::path(bool relative) const
{
	QString parent;
	if (m_parent)
		parent = m_parent->path(relative);
	else if (relative)
		return QString();
	return parent.isEmpty() ? m_name : parent + "/" + m_name;
}

QString ScImageCacheDir::path(const QString & file) const
{
	return path() + "/" + file;
}

bool ScImageCacheDir::isModified()
{
	if (!QFile::exists(path()))
	{
		if (m_exists)
		{
			m_exists = false;
			m_lastAccessValid = false;
			return true;
		}
		return false;
	}
	m_exists = true;
	AccessCounter access;
	bool valid = lastAccess(path(true), access);
	bool modified = true;
	if (m_lastAccessValid && valid && access == m_lastAccess)
		modified = false;
	m_lastAccess = access;
	m_lastAccessValid = valid;
	return modified;
}

bool ScImageCacheDir::addDir(ScImageCacheDir *dir)
{
	if (!m_dirs)
	{
		m_dirs = new DirMap;
		Q_CHECK_PTR(m_dirs);
		if (!m_dirs)
			return false;
	}
	if (m_dirs->contains(dir->name()))
		return false;
	m_dirs->insert(dir->name(), dir);
	return true;
}

void ScImageCacheDir::update()
{
	if (isModified())
	{
		scDebug() << path() << "is modified";
		if (m_dirs)
			foreach (ScImageCacheDir *p, *m_dirs)
				p->update();
		if (m_files)
			scan();
	}
}

void ScImageCacheDir::scan()
{
	if (!m_exists)
	{
		if (!m_files->isEmpty())
		{
			foreach (ScImageCacheFile *p, *m_files)
			{
				emit fileRemoved(p);
				delete p;
			}
			m_files->clear();
		}
		return;
	}

	QDirIterator di(path());
	QHash<QString, bool> current;

	while (di.hasNext())
	{
		di.next();

		QFileInfo info = di.fileInfo();

		if (info.isFile() && m_suffix.contains(info.suffix()))
		{
			FileMap::iterator i = m_files->find(info.fileName());
			if (i != m_files->end())
			{
				// possibly changed file
				ScImageCacheFile *p = *i;
				if (p->hasChanged(info))
					emit fileChanged(p, info);
			}
			else
			{
				// newly created file
				ScImageCacheFile *p = new ScImageCacheFile(info.fileName(), this);
				Q_CHECK_PTR(p);
				if (p == 0)
					return;
				emit fileCreated(p, info);
				m_files->insert(info.fileName(), p);
			}
			current[info.fileName()] = true;
		}
	}

	FileMap::iterator i = m_files->begin();
	while (i != m_files->end())
	{
		if (!current.contains(i.key()))
		{
			// removed file
			ScImageCacheFile *p = *i;
			i = m_files->erase(i);
			emit fileRemoved(p);
			delete p;
			
		}
		else
			i++;
	}
}

bool ScImageCacheDir::updateFile(const QString & file)
{
	return updateFile(file.split('/'));
}

bool ScImageCacheDir::updateFile(const QStringList & parts, int level)
{
	if (level < parts.count() - 1)
	{
		if (!m_dirs)
		{
			scDebug() << "no subdirs in" << path();
			return false;
		}
		const QString & dir = parts[level];
		DirMap::iterator i = m_dirs->find(dir);
		if (i == m_dirs->end())
		{
			scDebug() << "no directory" << dir << "in" << path() << "updateFile" << parts << level;
			return false;
		}
		return (*i)->updateFile(parts, level + 1);
	}
	if (!m_files)
	{
		scDebug() << "no m_files in" << path();
		return false;
	}
	const QString & file = parts[level];
	FileMap::iterator i = m_files->find(file);
	QFileInfo info(path(file));
	if (i != m_files->end())
	{
		ScImageCacheFile *p = *i;
		if (info.exists())
		{
			// update
			if (p->hasChanged(info))
				emit fileChanged(p, info);
		}
		else
		{
			// remove
			m_files->erase(i);
			emit fileRemoved(p);
			delete p;
		}
	}
	else
	{
		if (info.exists())
		{
			// create
			ScImageCacheFile *p = new ScImageCacheFile(file, this);
			Q_CHECK_PTR(p);
			if (p == 0)
				return false;
			emit fileCreated(p, info);
			m_files->insert(file, p);
		}
		else
		{
			// Don't make an assertion here. This *can* be a bug, but it
			// can also happen if someone else messes with the cache.
			scDebug() << "BUG: invalid update request for" << path(file);
			return false;
		}
	}
	return true;
}

bool ScImageCacheDir::updateAccess(const QString & dir, AccessCounter from, AccessCounter to)
{
	return updateAccess(dir.split('/'), from, to);
}

bool ScImageCacheDir::updateAccess(const QStringList & parts, AccessCounter from, AccessCounter to, int level)
{
	if (level < parts.count())
	{
		if (!m_dirs)
		{
			scDebug() << "no subdirs in" << path();
			return false;
		}
		const QString & dir = parts[level];
		DirMap::iterator i = m_dirs->find(dir);
		if (i == m_dirs->end())
		{
			scDebug() << "no directory" << dir << "in" << path() << "updateAccess" << parts << level;
			return false;
		}
		return (*i)->updateAccess(parts, from, to, level + 1);
	}
	if (!m_lastAccessValid)
	{
		scDebug() << "cannot update access counter for" << path() << "(lastAccess invalid)";
		return false;
	}
	if (m_lastAccess != from)
	{
		scDebug() << "cannot update access counter for" << path() << "(lastAccess =" << m_lastAccess << ", from =" << from << ", to =" << to << ")";
		return false;
	}
	scDebug() << "updating access counter for" << path() << "from" << from << "to" << to;
	m_lastAccess = to;
	return true;
}

bool ScImageCacheDir::lastAccess(const QString & dir, AccessCounter & access)
{
	ScLockedFileRO ro(ScImageCacheManager::absolutePath(dir) + "/" + accessFileName);
	if (!ro.exists())
	{
		scDebug() << ro.name() << "does not exist";
		return false;
	}
	if (!ro.open())
	{
		scDebug() << "failed to open" << ro.name() << "for reading";
		return false;
	}
	QTextStream in(ro.io());
	in >> access;
	return true;
}

