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

#include "sclockedfile.h"
#include "scimagecachemanager.h"
#include "scimagecachewriteaction.h"

#define SC_DEBUG_FILE defined(DEBUG_SCIMAGECACHE)
#include "scdebug.h"

ScImageCacheWriteAction::ScImageCacheWriteAction(bool haveMasterLock)
	: m_locked(false), m_haveMasterLock(haveMasterLock)
{
}

ScImageCacheWriteAction::~ScImageCacheWriteAction()
{
	if (m_locked)
		unlock();  // don't commit!
	clear();
}

void ScImageCacheWriteAction::clear()
{
	for (FileMap::iterator i = m_access.begin(); i != m_access.end(); i++)
		delete *i;
	m_access.clear();
}

bool ScImageCacheWriteAction::start()
{
	Q_ASSERT(!m_locked);
	if (m_locked)
	{
		scDebug() << "BUG: attempt to start action twice";
		return false;
	}
	if (!ScImageCacheManager::instance().acquireWriteLock())
	{
		scDebug() << "failed to acquire cache write lock";
		return false;
	}
	m_locked = true;
	return true;
}

bool ScImageCacheWriteAction::unlock()
{
	bool rv = true;
	if (!m_haveMasterLock)
	{
		for (FileMap::iterator i = m_access.begin(); i != m_access.end(); i++)
		{
			ScLockedFile *p = *i;
			if (!p->unlock())
				rv = false;
		}
	}
	if (!ScImageCacheManager::instance().releaseWriteLock())
	{
		scDebug() << "failed to release cache write lock";
		rv = false;
	}
	m_locked = false;
	return rv;
}

bool ScImageCacheWriteAction::update(const QString & dir, ScLockedFile *p, AccessCounter & from, AccessCounter & to)
{
	AccessCounter counter = 0;
	if (p->exists() && !ScImageCacheDir::lastAccess(dir, counter))
	{
		scDebug() << "failed to read" << p->name() << "dir =" << dir;
		return false;
	}
	from = counter;
	counter++;
	if (!p->open())
	{
		scDebug() << "failed to open" << p->name() << "for writing";
		return false;
	}
	QTextStream out(p->io());
	out << counter;
	to = counter;
	return true;
}

bool ScImageCacheWriteAction::commit()
{
	bool rv = true;
	Q_ASSERT(m_locked);
	if (!m_locked)
	{
		scDebug() << "BUG: attempt to release non-locked cache write lock";
		return false;
	}
	ScImageCacheManager & scm = ScImageCacheManager::instance();
	foreach (QString file, m_files)
		scm.updateFile(file);
	for (FileMap::iterator i = m_access.begin(); i != m_access.end(); i++)
	{
		ScLockedFile *p = *i;
		AccessCounter from, to;
		if (update(i.key(), p, from, to))
		{
			if (p->commit())
			{
				scDebug() << "updated" << p->name() << "from" << from << "to" << to;
				scm.updateAccess(i.key(), from, to);
			}
			else
			{
				scDebug() << "failed to commit changes to" << p->name();
				rv = false;
			}
		}
		else
			rv = false;
	}
	if (!unlock())
		rv = false;
	clear();
	if (!m_haveMasterLock)
		scm.tryCleanup();
	return rv;
}

bool ScImageCacheWriteAction::add(const QString & file)
{
	Q_ASSERT(m_locked);
	if (!m_locked)
	{
		scDebug() << "BUG: attempt to add" << file << "to action without start";
		return false;
	}

	QStringList dirs = file.split('/');
	QStringList dl;

	while (dirs.count() > 1)
	{
		dirs.removeLast();
		QString d = dirs.join("/");
		if (!m_access.contains(d))
			dl << d;
	}

	while (!dl.isEmpty())
	{
		QString d = dl.takeLast();
		ScLockedFile *p = new ScLockedFileRW(ScImageCacheManager::absolutePath(d) + "/" + ScImageCacheDir::accessFileName);
		Q_CHECK_PTR(p);
		if (!p)
			return false;
		if (!m_haveMasterLock && !p->lock())
			return false;
		m_access[d] = p;
	}

	m_files << file;

	return true;
}

