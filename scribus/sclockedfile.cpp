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

#include <QDir>
#include <QFileInfo>

#include "sclockedfile.h"

#define SC_DEBUG_FILE defined(DEBUG_SCLOCKEDFILE)
#include "scdebug.h"

const QString ScLockedFile::lockSuffix("lock");

ScLockedFile::ScLockedFile()
	: m_fileName(), m_isOpened(false), m_isLocked(false)
{
}

ScLockedFile::ScLockedFile(const QString & name)
	: m_fileName(name), m_isOpened(false), m_isLocked(false)
{
}

ScLockedFile::~ScLockedFile()
{
	if (m_isLocked)
		unlock();
}

void ScLockedFile::setName(const QString & name)
{
	m_fileName = name;
}

QString ScLockedFile::lockName() const
{
	return m_fileName + "." + lockSuffix;
}

bool ScLockedFile::lock()
{
	Q_ASSERT(!m_isLocked);
	if (m_isLocked)
	{
		scDebug() << "BUG:" << m_fileName << "is already locked";
		return false;
	}
	QDir cdir;
	m_isLocked = cdir.mkdir(lockName());
	if (!m_isLocked)
		scDebug() << "cannot lock" << m_fileName;
	return m_isLocked;
}

bool ScLockedFile::unlock()
{
	Q_ASSERT(m_isLocked);
	if (!m_isLocked)
	{
		scDebug() << "BUG:" << m_fileName << "is not locked";
		return false;
	}
	QDir cdir;
	m_isLocked = !cdir.rmdir(lockName());
	if (m_isLocked)
		scDebug() << "cannot unlock" << m_fileName;
	return !m_isLocked;
}

bool ScLockedFile::exists() const
{
	return QFile::exists(m_fileName);
}

bool ScLockedFile::remove() const
{
	return QFile::remove(m_fileName);
}

bool ScLockedFile::createPath() const
{
	QFileInfo info(m_fileName);
	QString path = info.path();
	QDir dir(path);
	if (dir.exists())
		return true;
	scDebug() << "creating directory" << path;
	return dir.mkpath(path);
}


ScLockedFileRW::ScLockedFileRW()
{
}

ScLockedFileRW::ScLockedFileRW(const QString & name)
	: ScLockedFile(name)
{
}

QString ScLockedFileRW::templateName(const QFileInfo & info)
{
	return info.dir().path() + "/" + info.baseName() + "_XXXXXX." + info.completeSuffix();
}

bool ScLockedFileRW::open()
{
	Q_ASSERT(!m_fileName.isEmpty());

	if (m_fileName.isEmpty())
	{
		scDebug() << "BUG: file name not set";
		return false;
	}

	QFileInfo info(m_fileName);
	if (info.exists() && !info.isWritable())
	{
		scDebug() << "final file is not writeable, aborting open for" << m_fileName;
		return false;
	}

	m_file.setFileTemplate(templateName(info));
	m_isOpened = m_file.open();

	if (!m_isOpened)
		scDebug() << "could not open read/write" << m_fileName;

	return m_isOpened;
}

bool ScLockedFileRW::commit()
{
	Q_ASSERT(m_isOpened);

	if (!m_isOpened)
	{
		scDebug() << "BUG: cannot commit," << m_fileName << "is not opened";
		return false;
	}

	if (exists() && !remove())
	{
		scDebug() << "cannot commit," << m_fileName << "cannot be removed";
		return false;
	}
		
	m_isOpened = false;

	QString tmpName = m_file.fileName();

	if (!m_file.rename(m_fileName))
	{
		scDebug() << "cannot commit," << tmpName << "cannot be renamed to" << m_fileName;
		return false;
	}

	m_file.setAutoRemove(false);

	return true;
}


ScLockedFileRO::ScLockedFileRO()
{
}

ScLockedFileRO::ScLockedFileRO(const QString & name)
	: ScLockedFile(name)
{
}

bool ScLockedFileRO::open()
{
	Q_ASSERT(!m_fileName.isEmpty());

	if (m_fileName.isEmpty())
	{
		scDebug() << "BUG: file name not set";
		return false;
	}

	m_file.setFileName(m_fileName);
	m_isOpened = m_file.open(QIODevice::ReadOnly);

	if (!m_isOpened && exists())
		scDebug() << "could not open read-only" << m_fileName;

	return m_isOpened;
}

bool ScLockedFileRO::commit()
{
	Q_ASSERT(false);
	scDebug() << "BUG: cannot commit," << m_fileName << "is read-only";
	return false;
}
