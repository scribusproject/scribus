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

#ifndef SCIMAGECACHEFILE_H
#define SCIMAGECACHEFILE_H

#include <QObject>
#include <QDateTime>
#include <QFileInfo>
#include <QString>

#include "scribusapi.h"

class ScImageCacheDir;

/**
  * @brief Representation of a file node in the image cache tree
  * @author Marcus Holland-Moritz
  */
class ScImageCacheFile : public QObject
{
	Q_OBJECT

public:
	ScImageCacheFile(const QString & name, ScImageCacheDir *parent = 0);
	~ScImageCacheFile();

	QString path(bool relative = false) const;
	qint64 size() const;
	const QDateTime & modified() const { return m_modified; };
	bool exists() const;
	bool hasChanged(const QFileInfo & info) const;
	bool hasChanged() const;
	bool update(const QFileInfo & info);
	bool update();

private:
	const QString m_name;
	ScImageCacheDir * const m_parent;
	QDateTime m_modified;
	qint64 m_size;
};

#endif
