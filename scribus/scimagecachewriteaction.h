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

#ifndef SCIMAGECACHEWRITEACTION_H
#define SCIMAGECACHEWRITEACTION_H

#include <QHash>
#include <QString>
#include <QStringList>

#include "scribusapi.h"
#include "scimagecachedir.h"

class ScLockedFile;

/**
  * @brief Bracket for write accesses to the image cache
  * @author Marcus Holland-Moritz
  */
class ScImageCacheWriteAction
{
public:
	ScImageCacheWriteAction(bool haveMasterLock = false);
	~ScImageCacheWriteAction();

	bool start();
	bool add(const QString & file);
	bool commit();

private:
	typedef ScImageCacheDir::AccessCounter AccessCounter;
	typedef QHash<QString, ScLockedFile *> FileMap;
	QStringList m_files;
	FileMap m_access;
	bool m_locked;
	const bool m_haveMasterLock;

	void clear();
	bool unlock();
	bool update(const QString & dir, ScLockedFile *p, AccessCounter & from, AccessCounter & to);
};

#endif
