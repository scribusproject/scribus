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

#ifndef SCIMAGECACHEMANAGER_H
#define SCIMAGECACHEMANAGER_H

#include <QList>
#include <QString>
#include <QDebug>

#include "scribusapi.h"
#include "scimagecachedir.h"

class QTemporaryFile;
class QFileInfo;
class ScImageCacheFile;

/**
  * @brief Scribus image cache manager
  * @author Marcus Holland-Moritz
  */
class SCRIBUS_API ScImageCacheManager : public QObject
{
	Q_OBJECT

public:
	typedef ScImageCacheDir::AccessCounter AccessCounter;

	/**
	* @brief Get image cache manager instance
	* @return Reference to the singleton instance
	*/
	static ScImageCacheManager & instance();
	/**
	* @brief Convert relative to absolute path
	* @param fn Path relative to the image cache root directory
	* @return Absolute path
	*/
	static QString absolutePath(const QString & fn);

	/**
	* @brief Enable/disable the image cache
	* @param enableCache \c true if the cache should be enabled
	*/
	void setEnabled(bool enableCache);
	/**
	* @brief Check if the image cache is enabled
	* @return \c true if the cache is be enabled, \c false otherwise
	*/
	bool enabled(void) const { return m_isEnabled; }
	/**
	* @brief Set cache size limit
	* @param maxCacheSizeMiB Maximum cache size in MiB.
	* @return \c true if the cache size limit could be set, \c false otherwise
	*/
	bool setMaxCacheSizeMiB(int maxCacheSizeMiB);
	/**
	* @brief Set cache entry limit
	* @param maxCacheEntries Maximum number of meta files in the cache
	* @return \c true if the cache entry limit could be set, \c false otherwise
	*/
	bool setMaxCacheEntries(int maxCacheEntries);
	/**
	* @brief Set cache image file compression level
	* @param level Image compression level. -1 is the default compression level
	*        for PNG images. 0 is no compression, 1 is fastest comression and
	*        9 is best compression.
	* @return \c true if the compression level could be set, \c false otherwise
	*/
	bool setCompressionLevel(int level);
	/**
	* @brief Get cache image file compression level
	* @return Current compression level
	*/
	int compressionLevel() const;

	/**
	* @brief Initialize the cache manager
	*/
	void initialize();
	/**
	* @brief Try to run a cache cleanup
	*/
	void tryCleanup();
	/**
	* @brief Try to acquire a write lock
	* @return \c true if the write lock could be acquired, \c false otherwise
	*/
	bool acquireWriteLock();
	/**
	* @brief Release a write lock
	* @return \c true if the write lock could be released, \c false otherwise
	*/
	bool releaseWriteLock();
	/**
	* @brief Remove master lock
	*
	* This method should only be called if a Scribus crash is detected. It
	* will force the release of an existing master lock in order not to block
	* other Scribus instances.
	*/
	void removeMasterLock();

	/**
	* @brief Access update notification
	*
	* This method notifies the cache manager of an updated \c access file.
	*
	* @param dir Path of updated directory relative to the image cache root directory
	* @param from Previous access count
	* @param to new access count
	* @return \c true if the access count could be updated, \c false otherwise
	*/
	bool updateAccess(const QString & dir, AccessCounter from, AccessCounter to);
	/**
	* @brief File update notification
	*
	* This method notifies the cache manager of an updated (i.e. newly created,
	* changed or removed) file in the image cache.
	*
	* @param file Path of updated file relative to the image cache root directory
	* @return \c true if the file information could be updated, \c false otherwise
	*/
	bool updateFile(const QString & file);

private slots:
	void fileCreated(ScImageCacheFile * file, const QFileInfo & info);
	void fileChanged(ScImageCacheFile * file, const QFileInfo & info);
	void fileRemoved(ScImageCacheFile * file);

private:
	class MetaAgeList
	{
	public:
		MetaAgeList();
		void insert(ScImageCacheFile *p);
		void update(ScImageCacheFile *p, const QFileInfo & newInfo);
		void remove(ScImageCacheFile *p);
		ScImageCacheFile *getOldest();
		int count() const { return m_fa.size(); }

	private:
		typedef QList<ScImageCacheFile *> FAL;
		FAL m_fa;

		static bool ageLessThan(const ScImageCacheFile *a, const ScImageCacheFile *b);
	};

	ScImageCacheManager();
	~ScImageCacheManager();

	static void create();
	static void cleanupLockDir();
	static bool createLockDir();
	static QString lockDir();
	static QString masterLockFile();
	static QString writeLockTemplate();

	void sanitizeCache();
	void updateCache();
	void cleanupCache();
	ScImageCacheFile *getOldestCacheEntry();

	bool acquireMasterLock();
	bool releaseMasterLock();

	bool m_isEnabled;
	bool m_haveMasterLock;
	bool m_inCleanup;
	int m_writeLockCount;
	int m_compressionLevel;
	int m_maxEntries;
	int m_maxSizeMiB;
	qint64 m_maxTotalSize;
	qint64 m_totalCacheSize;

	MetaAgeList m_metaAge;

	QTemporaryFile *m_writeLockFile;
	ScImageCacheDir *m_root;
};

#endif
