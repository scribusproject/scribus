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

#include <QFileInfo>

#include "scimagecachedir.h"
#include "scimagecachefile.h"

#define SC_DEBUG_FILE defined(DEBUG_SCIMAGECACHE)
#include "scdebug.h"

ScImageCacheFile::ScImageCacheFile(const QString & name, ScImageCacheDir *parent)
	:m_name(name), m_parent(parent)
{
	QFileInfo info(path());
	m_modified = info.lastModified();
	m_size = info.size();
}

ScImageCacheFile::~ScImageCacheFile()
{
}

QString ScImageCacheFile::path(bool relative) const
{
	return m_parent ? m_parent->path(relative) + "/" + m_name : m_name;
}

qint64 ScImageCacheFile::size() const
{
	return m_size;
}

bool ScImageCacheFile::hasChanged(const QFileInfo & info) const
{
	return info.lastModified() != m_modified || info.size() != m_size;
}

bool ScImageCacheFile::hasChanged() const
{
	QFileInfo info(path());
	return hasChanged(info);
}

bool ScImageCacheFile::update(const QFileInfo & info)
{
	bool changed = false;
	if (info.lastModified() != m_modified)
	{
		m_modified = info.lastModified();
		changed = true;
	}
	if (info.size() != m_size)
	{
		m_size = info.size();
		changed = true;
	}
	return changed;
}

bool ScImageCacheFile::update()
{
	QFileInfo info(path());
	return update(info);
}
