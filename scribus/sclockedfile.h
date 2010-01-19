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

#ifndef SCLOCKEDFILE_H
#define SCLOCKEDFILE_H

#include "scconfig.h"
#include "scribusapi.h"

#include <QFile>
#include <QString>
#include <QTemporaryFile>

class QFileInfo;

/**
  * @brief Base class for locked file access
  * @author Marcus Holland-Moritz
  */
class ScLockedFile
{
public:
	virtual ~ScLockedFile();

	void setName(const QString & name);
	const QString & name() const { return m_fileName; }
	bool createPath() const;

	bool lock();
	bool unlock();
	bool locked() const { return m_isLocked; }

	bool exists() const;
	bool remove() const;

	virtual bool open() = 0;
	virtual bool commit() = 0;

	virtual QIODevice *io() = 0;

	static const QString lockSuffix;

protected:
	ScLockedFile();
	ScLockedFile(const QString & name);

	QString m_fileName;
	bool m_isOpened;

private:
	bool m_isLocked;

	QString lockName() const;
};

/**
  * @brief Read-only locked file access
  * @author Marcus Holland-Moritz
  */
class ScLockedFileRO : public ScLockedFile
{
public:
	ScLockedFileRO();
	ScLockedFileRO(const QString & name);

	virtual bool open();
	virtual bool commit();

	virtual QIODevice *io() { return &m_file; }

private:
	QFile m_file;
};

/**
  * @brief Read/write locked file access
  * @author Marcus Holland-Moritz
  */
class ScLockedFileRW : public ScLockedFile
{
public:
	ScLockedFileRW();
	ScLockedFileRW(const QString & name);

	virtual bool open();
	virtual bool commit();

	virtual QIODevice *io() { return &m_file; }

private:
	QTemporaryFile m_file;

	static QString templateName(const QFileInfo & info);
};

#endif
