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

#ifndef SCIMAGECACHEDIR_H
#define SCIMAGECACHEDIR_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QStringList>

#include "scribusapi.h"

class ScImageCacheFile;
class QFileInfo;

/**
  * @brief Representation of a directory node in the image cache tree
  * @author Marcus Holland-Moritz
  */
class ScImageCacheDir : public QObject
{
	Q_OBJECT

public:
	typedef unsigned int AccessCounter;

	ScImageCacheDir(const QString & dir, ScImageCacheDir *parent = 0, bool scanFiles = false, const QStringList & suffixList = QStringList());
	~ScImageCacheDir();
	ScImageCacheDir *newSubDir(const QString & dir, bool scanFiles = false, const QStringList & suffixList = QStringList());
	const QString & name() const { return m_name; }
	QString path(bool relative = false) const;
	QString path(const QString & file) const;
	qint64 size() const;
	bool exists() const;
	void update();
	bool updateFile(const QString & file);
	bool updateAccess(const QString & dir, AccessCounter from, AccessCounter to);

	static bool lastAccess(const QString & dir, AccessCounter & access);

	static const QString accessFileName;

signals:
	void fileCreated(ScImageCacheFile * file, const QFileInfo & info);
	void fileChanged(ScImageCacheFile * file, const QFileInfo & info);
	void fileRemoved(ScImageCacheFile * file);

private:
	typedef QHash<QString, ScImageCacheDir *> DirMap;
	typedef QHash<QString, ScImageCacheFile *> FileMap;

	void scan();
	bool addDir(ScImageCacheDir *dir);
	bool isModified();
	bool updateFile(const QStringList & parts, int level = 0);
	bool updateAccess(const QStringList & parts, AccessCounter from, AccessCounter to, int level = 0);

	QString m_name;
	const QStringList m_suffix;
	ScImageCacheDir * const m_parent;
	bool m_exists;
	bool m_lastAccessValid;
	AccessCounter m_lastAccess;
	DirMap *m_dirs;
	FileMap *m_files;
};

#endif
