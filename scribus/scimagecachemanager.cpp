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
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QTemporaryFile>

#include "sclockedfile.h"
#include "scimagecachedir.h"
#include "scimagecachefile.h"
#include "scimagecachemanager.h"
#include "scimagecacheproxy.h"
#include "scimagecachewriteaction.h"
#include "scpaths.h"

#define SC_DEBUG_FILE defined(DEBUG_SCIMAGECACHE)
#include "scdebug.h"

/*!

\page imagecache Scribus Image Cache

This page gives some details about the Scribus image cache manager implemented
in ScImageCacheManager. The image cache manager, accompanied by a number of
helper classes, is responsible for caching low-resolution versions of images
used in Scribus documents.

As the loading of images and their conversion to low resolution consumes
a lot of time, the image cache helps to massively speed up the loading of
images that have been previously loaded under the same conditions. It will
also speed up operations like undoing or redoing image effects.

The image cache was designed to be accessible simultaneously by multiple
instances of Scribus. It should even be possible to share the cache over
a network drive, although this will surely degrade performance.


\section ic_filetypes File Types in the Image Cache

All files stored in the cache are either short XML documents or real image
files. PNG has been chosen as the image format, as it offers good compression
and is a lossless format. At low compression levels, it is also quite fast.

There are quite a lot of properties in Scribus that have an influence on
how an image will be rendered on the screen. These are mainly color management
and image effects. These properties will be called modifiers in this text.

Image information is properties directly associated with the on-disk image
file, e.g. resolution or EXIF data. As the original image is not read when
fetching images from the cache, this data needs to be cached as well.

Meta information, finally, is information describing the cache entry. It
contains properties like the the on-disk image file path, the image file size
or the last modification date. It is used to identify whether or not an image
can be fetched from the cache or must be reloaded from its original file.

\verbatim
-------------------------------------------------------------------------------

   Meta File (.xml)            Reference File (.ref)       Image File (.png)

  .-----------------.         .-----------------.         .-----------------.
  |meta information |-------->|reference count  |         |cached image     |
  |modifiers        |         |                 |-------->|                 |
  |image information|    .--->|                 |         |                 |
  '-----------------'    |    '-----------------'         '-----------------'
                         |
  .-----------------.    |
  |meta information |    |
  |modifiers        |----'
  |image information|
  '-----------------'

-------------------------------------------------------------------------------
\endverbatim

As different combinations of modifiers \em may end up producing exactly the same
image in the cache, multiple meta files may reference the same image file. To
keep track of the number of references, each cached image is accompanied by a
reference file that differs from the image file only by the file suffix.

To avoid races between multiple instances of Scribus, possibly even running on
different machines, all files must be accessed atomically when the cache is
being modified. Non-modifying, read-only accesses are always allowed.

This is usually achieved by the following mechanisms:

 - Lock files (with an additional suffix ".lock") are created by the instance
   that wishes to modify an entry in the cache. Only the instance that has
   successfully acquired all necessary lock files may modify the cache.
   As it is close to impossible to atomically create a \em file in a
   platform-independent way, the lock file is actually implemented as a lock
   directory. See ScLockedFile for details.

 - All files that are created or modified are created as temporary files first.
   Only when they have been written completely, the old version of the file is
   unlinked and the new version is renamed to its final name.

This ensures that an instance that only wishes to read from the cache can
safely do so even without caring about locked files.

Furthermore, in order to avoid any deadlocks or delays, locking only makes sure
that only one instance writes to the cache at a time. If another instance fails
to get the necessary locks, it will simply not not carry out the whole cache
access.


\section ic_dirstructure Directory Structure

Each cache file is uniquely indentified by a hexadecimal MD5 hash. The first
two hex digits represent two levels of subdirectories and the remainder forms
the start of the file's basename, for example:

\verbatim
  $(HOME)/.scribus/cache/img/a/e/15c5160668926e4a7c593a813a0d68.xml
\endverbatim

Within each folder of the cache structure, there is an additional \c access
file that keeps track of write accesses to this folder. The purpose of this
file is to notify other instances of Scribus when entries in the cache have
been modified. The file simply contains a counter that is incremented with
each write access to the cache. The file also serves as a lock for the
directory. Instead of locking individual files in the cache, locking the
\c access files is sufficient.


\section ic_housekeeping Cache Housekeeping

Each instance doing any write access to the cache will first create its own
lock file in:

\verbatim
  $SCRIBUS/cache/img/locks/
\endverbatim

The name does not matter. After successful creation of this file, the instance
checks for the presence of the master lock file

\verbatim
  $SCRIBUS/cache/img/locks/master.lock
\endverbatim

If this is present, the instance will remove its own lock file and will not
initiate any write accesses to the cache.

An instance wishing to do a cleanup will attempt to create the master lock
file. If it succeeds, it will check that no other lock files are present in
the lock directory. If other lock files are present, it will remove the master
lock file and not perform a cleanup. If no other lock files are present, the
instance has exclusive write access to all cache files.

After each write operation, a cache cleanup is performed if necessary. This
means, if the cache limits (number of meta files or total cache size) are
exceeded, the oldest meta files will be removed until the cache is within
the user defined limits again.

In the Scribus startup phase, if a master lock can be acquired, the instance
will also sanitize the cache. This includes operations like removing any
orphaned files or fixing reference counters.


\section ic_cacheimage Keeping the Cache Image up-to-date

The cache image is the cache manager's internal representation of all files
in the on-disk cache. It is a tree of ScImageCacheDir and ScImageCacheFile
objects. The ScImageCacheDir objects emit signals when files in the cache
are updated. These signals drive additional operations in the cache manager
like updating the total cache size or the meta age list that keeps track of
the oldest meta files in the cache.

Each time a Scribus instance performs a write to the cache, it attempts to
acquire a master lock in order to remove old files if necessary. Other
instances might also have modified the cache in between, so it is mandatory
to update the cache image before.

However, instead of rescanning the whole cache structure, the cache manager
only looks for changes to the \c access files. If a change has been detected
in one directory, its subdirectories are checked recursively. Only directories
that have been modified by other running instances of Scribus need to be
rescanned. So, in the most common case of only one Scribus instance running
at a time, no rescans have to be performed.

There is one case, however, where the cache image is not kept up-to-date.
Whenever a read-only access to the cache is performed, the corresponding
meta file is touched to prevent it from being deleted when the cache is
cleaned up. This operation does not directly trigger an update of the
cache image. Updating the modification timestamp is delayed until a cache
cleanup becomes necessary. Before the oldest metafile is actually deleted
from the cache, its timestamp is checked and it will only be removed if
it is still the oldest file in the cache. Otherwise, it's position in the
MetaAgeList will be updated. The main driver behind this is that cache
reads should be cheap and not require any locking. However, any changes
to the cache need to be reflected in the \c access files, which would in
turn require locking.


\section ic_accessing Accessing the Image Cache

To access the image cache, a ScImageCacheProxy object is needed. It provides
all necessary functionality to read and write images in the cache. See
pageitem.cpp and scimage.cpp for examples.

Internally, write accesses to the cache are bracketed with the help of an
ScImageCacheWriteAction object. This object is being notified of all files
that participate in the cache access and will carry out all necessary locking,
updating of the \c access files and notifying the cache manager of any changes.


\section ic_performance Performance Measurements

The following table shows wallclock and real CPU times for loading different
documents in Scribus. In most cases, documents have been loaded multiple
times. Before the first load, the filesystem cache was completely flushed.

As can be seen, there is no difference in load times if the cache is disabled
in the Scribus preferences. This is important for users who wish to disable
the cache (for whatever reasons).

Also, first load times are not severely longer if the cache is enabled. In the
worst case, the first load time was less than 20% longer. Most of that time is
spent compressing and writing the cache images, which can be seen in the last
rows of the table. If the images are already found in the cache and only the
meta files have to be created, the load time is almost equivalent to the load
time without cache support.

However, load times are significantly shorter for the second and third load of
the document. The reason for the third load time being even shorter is that
the cache files are likely to still be present in the filesystem cache.
Usually, re-loading a document is 20 to 50 times faster with the cache enabled.
All measurements were done with medium resolution (72dpi) cache image files and
with the default cache image compression level of 1. Raising the compression
level beyond 4 will mainly slow down the first load of images. Setting it to
zero will significantly increase the cache file size.

\verbatim
-------------------------------------------------------------------------------
                    trunk original      trunk with cache    trunk with cache
                    no cache support    cache disabled      cache enabled
-------------------------------------------------------------------------------
                    wall      real      wall      real      wall      real
-------------------------------------------------------------------------------

1 page document
5 small images
on local disc

  1. load             9.802     2.060     9.661     2.070     9.517     2.610
  2. load             1.802     1.640     1.812     1.750     0.585     0.530
  3. load             1.744     1.630     1.709     1.630     0.547     0.470

266 page document
2.6 GiB of TIFFs
on local disk
(CMS disabled)

  1. load           235.080   194.850   233.662   192.800   277.886   226.750
  2. load           226.178   191.510   225.340   191.920    11.276     9.270
  3. load           227.191   191.580                         7.632     7.320

160 page document
2.6 GiB of TIFFs
on network drive
(CMS enabled)

  1. load           979.028   602.100                      1011.878   631.290
  1. load [1]                                               985.161   608.530
  2. load           972.407   599.280                        34.296    25.860
  3. load                                                    18.605    18.310

-------------------------------------------------------------------------------
 [1] image files already found in cache
-------------------------------------------------------------------------------
\endverbatim

******************************************************************************/

namespace {
	const int LOCKFILE_MAX_AGE_SECONDS = 3600;
}


ScImageCacheManager::MetaAgeList::MetaAgeList()
{
}

void ScImageCacheManager::MetaAgeList::insert(ScImageCacheFile *p)
{
	m_fa.insert(qLowerBound(m_fa.begin(), m_fa.end(), p, ageLessThan), p);
}

void ScImageCacheManager::MetaAgeList::update(ScImageCacheFile *p, const QFileInfo & newInfo)
{
	remove(p);
	p->update(newInfo);
	insert(p);
}

void ScImageCacheManager::MetaAgeList::remove(ScImageCacheFile *p)
{
	FAL::iterator i = qBinaryFind(m_fa.begin(), m_fa.end(), p, ageLessThan);
	Q_ASSERT(i != m_fa.end());
	if (i != m_fa.end())
		m_fa.erase(i);
	else
		scDebug() << "BUG:" << p->path() << "not found in meta age list";
}

bool ScImageCacheManager::MetaAgeList::ageLessThan(const ScImageCacheFile *a, const ScImageCacheFile *b)
{
	return a->modified() < b->modified() || (a->modified() == b->modified() && a < b);
}

ScImageCacheFile *ScImageCacheManager::MetaAgeList::getOldest()
{
	return m_fa.isEmpty() ? 0 : m_fa.front();
}



ScImageCacheManager & ScImageCacheManager::instance()
{
	static ScImageCacheManager instance;
	return instance;
}

ScImageCacheManager::ScImageCacheManager()
	: m_isEnabled(false), m_haveMasterLock(false), m_inCleanup(false), m_writeLockCount(0),
	  m_compressionLevel(-1), m_maxEntries(0), m_maxSizeMiB(0), m_maxTotalSize(0),
	  m_totalCacheSize(0), m_writeLockFile(0), m_root(0)
{
}

ScImageCacheManager::~ScImageCacheManager()
{
	// no tryCleanup here, I guess we rather want Scribus to exit fast
	if (m_root)
		delete m_root;
}

QString ScImageCacheManager::absolutePath(const QString & fn)
{
	QString rv(ScPaths::getImageCacheDir() + fn);
	while (rv.endsWith('/'))
		rv.chop(1);
	return rv;
}

void ScImageCacheManager::cleanupLockDir()
{
	scDebug() << "cleaning up lock files";

	QDirIterator di(lockDir());
	QDateTime now = QDateTime::currentDateTime();

	QFileInfo masterInfo(masterLockFile());
	bool masterLockFound = false;

	while (di.hasNext())
	{
		di.next();

		QFileInfo info = di.fileInfo();

		if (info.suffix() == ScLockedFile::lockSuffix)
		{
			int age = info.lastModified().secsTo(now);
			if (age > LOCKFILE_MAX_AGE_SECONDS)
			{
				scDebug() << "removing old cache lock file" << info.filePath() << "(age =" << age << "seconds)";
				if (info.isFile())
				{
					if (!QFile::remove(info.filePath()))
						scDebug() << "could not remove file" << info.filePath();
				}
				else if (info.isDir())
				{
					if (info == masterInfo)
						masterLockFound = true;
				}
			}
		}
	}

	if (masterLockFound)
	{
		QString masterLockRemove = masterLockFile() + ".remove";
		QDir masterRemove(masterLockRemove);

		scDebug() << "safely removing" << masterLockFile();

		if (!masterRemove.mkdir(masterLockRemove))
		{
			scDebug() << "failed to create" << masterLockRemove;
			return;
		}

		// At this point, no other instance will attempt to delete the
		// master lock *except* for an instance that has just acquired
		// the lock after removing the stale lock before. So we check
		// again whether the master lock is still present and old
		// enough for us to safely remove it.

		// Should, for whatever reason, Scribus crash before the end of
		// this routine, the cache will be unusable unless someone cleans
		// up the lock directory manually.

		masterInfo.refresh();

		if (masterInfo.exists())
		{
			if (masterInfo.lastModified().secsTo(now) > LOCKFILE_MAX_AGE_SECONDS)
			{
				if (!masterRemove.rmdir(masterLockFile()))
					scDebug() << "could not remove directory" << masterLockFile();
			}
			else
				scDebug() << masterLockFile() << "has been reincarnated";
		}
		else
			scDebug() << masterLockFile() << "is already gone";

		if (!masterRemove.rmdir(masterLockRemove))
			scDebug() << "failed to remove" << masterLockRemove;
	}
}

void ScImageCacheManager::removeMasterLock()
{
	extern bool emergencyActivated;
	Q_ASSERT(emergencyActivated);
	if (!emergencyActivated)
	{
		scDebug() << "NEVER call removeMasterLock() when not in an emergency";
		return;
	}
	if (!m_haveMasterLock)
	{
		scDebug() << "no master lock set by this instance";
		return;
	}
	scDebug() << "removing master lock file";
	QDir master(masterLockFile());
	if (!master.rmdir(masterLockFile()))
	{
		scDebug() << "failed to remove master lock directory";
		return;
	}
	m_haveMasterLock = false;
}

void ScImageCacheManager::sanitizeCache()
{
	scDebug() << "sanitizing image cache";

	QFileInfo masterInfo(masterLockFile());
	QDirIterator di(ScPaths::getImageCacheDir(), QDirIterator::Subdirectories);
	QDir dir(ScPaths::getImageCacheDir());

	QHash<QString, QString> metafile;       // meta-filename => base 
	QHash<QString, int> reffile;            // ref-filename  => refcount
	QHash<QString, int> imgfile;            // img-filename  => 0

	ScImageCacheWriteAction action(true);
	action.start();

	while (di.hasNext())
	{
		di.next();

		QFileInfo info = di.fileInfo();
		QString relFile = dir.relativeFilePath(info.filePath());

		if (info.suffix() == ScLockedFile::lockSuffix)
		{
			// any lock files outside the lock directory must be leftovers,
			// regardless of age, as we've acquired the master lock

			if (info != masterInfo)
			{
				scDebug() << "removing stale lock file" << relFile;
				if (dir.rmdir(info.filePath()))
					action.add(relFile);
				else
					scDebug() << "could not remove" << info.filePath();
			}
		}
		else if (info.isFile())
		{
			if (info.suffix() == ScImageCacheProxy::metaSuffix)
			{
				QString base = ScImageCacheProxy::getBaseName(relFile);
				if (base.isEmpty())
				{
					scDebug() << "removing invalid meta file" << relFile;
					if (QFile::remove(info.filePath()))
						action.add(relFile);
					else
						scDebug() << "could not remove" << info.filePath();
				}
				else
					metafile[relFile] = base;
			}
			else if (info.suffix() == ScImageCacheProxy::referenceSuffix)
			{
				int refcount;
				if (!ScImageCacheProxy::getRefCount(relFile, refcount))
				{
					scDebug() << "removing invalid reference file" << relFile;
					if (QFile::remove(info.filePath()))
						action.add(relFile);
					else
						scDebug() << "could not remove" << info.filePath();
				}
				else
					reffile[relFile] = refcount;
			}
			else if (info.suffix() == ScImageCacheProxy::imageSuffix)
				imgfile[relFile] = 0;
			else if (di.fileName() != ScImageCacheDir::accessFileName)
				scDebug() << "unknown file in cache" << di.fileName();
		}
	}

	QRegExp reImg(ScImageCacheProxy::imageSuffix + "$");
	QRegExp reRef(ScImageCacheProxy::referenceSuffix + "$");

	QHash<QString, int>::iterator isi;

	// delete all image files without reference file

	isi = imgfile.begin();
	while (isi != imgfile.end())
	{
		QString ref = isi.key();
		ref.replace(reImg, ScImageCacheProxy::referenceSuffix);
		if (!reffile.contains(ref))
		{
			scDebug() << "removing image file without reference" << isi.key();
			if (QFile::remove(absolutePath(isi.key())))
				action.add(isi.key());
			else
				scDebug() << "could not remove" << absolutePath(isi.key());
			isi = imgfile.erase(isi);
		}
		else
			isi++;
	}

	// delete all reference files without image file

	isi = reffile.begin();
	while (isi != reffile.end())
	{
		QString img = isi.key();
		img.replace(reRef, ScImageCacheProxy::imageSuffix);
		if (!imgfile.contains(img))
		{
			scDebug() << "removing reference file without image" << isi.key();
			if (QFile::remove(absolutePath(isi.key())))
				action.add(isi.key());
			else
			 	scDebug() << "could not remove" << absolutePath(isi.key());
			isi = reffile.erase(isi);
		}
		else
			isi++;
	}

	// find all metafiles that don't reference existing reference files
	// these can be directly deleted

	QHash<QString, QString>::iterator iss;
	QHash<QString, int> references; // ref-filename  => number of references

	iss = metafile.begin();
	while (iss != metafile.end())
	{
		QString ref = *iss + "." + ScImageCacheProxy::referenceSuffix;
		if (!reffile.contains(ref))
		{
			scDebug() << "removing orphaned meta file" << iss.key();
			if (QFile::remove(absolutePath(iss.key())))
				action.add(iss.key());
			else
			 	scDebug() << "could not remove" << iss.key();
			iss = metafile.erase(iss);
		}
		else
		{
			QFileInfo info(absolutePath(iss.key()));
			if (!references.contains(ref))
				references[ref] = 1;
			else
				references[ref]++;
			iss++;
		}
	}

	// find all reference files that are not referenced by any metafile
	// these, and their corresponding image files, can be directly deleted
	// fix reference counts for remaining reference files

	isi = reffile.begin();
	while (isi != reffile.end())
	{
		int newRefCount;

		if (!references.contains(isi.key()))
		{
			QString ref = isi.key();
			QString img = ref;
			img.replace(reRef, ScImageCacheProxy::imageSuffix);
			scDebug() << "removing orphaned reference/image files" << ref << img;
			if (QFile::remove(absolutePath(ref)))
				action.add(ref);
			else
			 	scDebug() << "could not remove" << absolutePath(ref);
			if (QFile::remove(absolutePath(img)))
				action.add(img);
			else
			 	scDebug() << "could not remove" << absolutePath(img);
		}
		else if (*isi != (newRefCount = references[isi.key()]))
		{
			scDebug() << "fixing reference count for" << isi.key() << "old =" << *isi << "new =" << newRefCount;
			if (ScImageCacheProxy::fixRefCount(isi.key(), newRefCount))
				action.add(isi.key());
			else
				scDebug() << "could not fix reference count for" << isi.key();
		}
		isi++;
	}

	action.commit();

	scDebug() << "finished sanitizing image cache";
}

void ScImageCacheManager::tryCleanup()
{
	if (m_inCleanup)
		return;

	scDebug() << "attempting to acquire master lock";

	if (!acquireMasterLock())
		return;

	cleanupCache();

	scDebug() << "releasing master lock";

	releaseMasterLock();
}

void ScImageCacheManager::updateCache()
{
	scDebug() << "updating cache";

	if (!m_root)
	{
		QStringList suffixes;
		suffixes << ScImageCacheProxy::metaSuffix << ScImageCacheProxy::referenceSuffix << ScImageCacheProxy::imageSuffix;

		m_root = new ScImageCacheDir(ScPaths::getImageCacheDir());
		Q_CHECK_PTR(m_root);

		if (!m_root)
			return;

		for (int j = 0; j < 16; j++)
		{
			ScImageCacheDir *d1 = m_root->newSubDir(QString::number(j, 16));

			if (!d1)
			{
				delete m_root;
				m_root = 0;
				return;
			}
		
			for (int k = 0; k < 16; k++)
			{
				ScImageCacheDir *d2 = d1->newSubDir(QString::number(k, 16), true, suffixes);

				if (!d2)
				{
					delete m_root;
					m_root = 0;
					return;
				}

				connect(d2, SIGNAL(fileCreated(ScImageCacheFile *, const QFileInfo &)), SLOT(fileCreated(ScImageCacheFile *, const QFileInfo &)));
				connect(d2, SIGNAL(fileChanged(ScImageCacheFile *, const QFileInfo &)), SLOT(fileChanged(ScImageCacheFile *, const QFileInfo &)));
				connect(d2, SIGNAL(fileRemoved(ScImageCacheFile *)), SLOT(fileRemoved(ScImageCacheFile *)));
			}
		}
	}

	m_root->update();
}

void ScImageCacheManager::cleanupCache()
{
	m_inCleanup = true;

	updateCache();

	// remove oldest entries until limits are reached

	scDebug() << "removing old cache entries";

	scDebug() << "total size:" << m_totalCacheSize << "/ max:" << m_maxTotalSize;
	scDebug() << "meta count:" << m_metaAge.count() << "/ max:" << m_maxEntries;

	while (m_totalCacheSize > m_maxTotalSize || m_metaAge.count() > m_maxEntries)
	{
		ScImageCacheFile *p = getOldestCacheEntry();
		if (!ScImageCacheProxy::removeCacheEntry(p->path(true), true))
			break;
		scDebug() << "total size:" << m_totalCacheSize << "/ max:" << m_maxTotalSize;
		scDebug() << "meta count:" << m_metaAge.count() << "/ max:" << m_maxEntries;
	}

	m_inCleanup = false;
}

void ScImageCacheManager::initialize()
{
	scDebug() << "starting cache manager initialization";

	// no need to have a lock here, as we just create the basic cache structure

	if (enabled())
	{
		cleanupLockDir();

		scDebug() << "attempting to acquire master lock";

		if (acquireMasterLock())
		{
			sanitizeCache();
			cleanupCache();

			scDebug() << "releasing master lock";

			releaseMasterLock();
		}
	}

	scDebug() << "cache manager initialization finished";
}

void ScImageCacheManager::fileCreated(ScImageCacheFile * file, const QFileInfo & info)
{
	QString relFile = file->path(true);
	scDebug() << "created" << relFile;
	m_totalCacheSize += file->size();
	if (info.suffix() == ScImageCacheProxy::metaSuffix)
		m_metaAge.insert(file);
}

void ScImageCacheManager::fileChanged(ScImageCacheFile * file, const QFileInfo & info)
{
	QString relFile = file->path(true);
	scDebug() << "updated" << relFile;
	m_totalCacheSize -= file->size();
	if (info.suffix() == ScImageCacheProxy::metaSuffix)
		m_metaAge.update(file, info);
	else
		file->update(info);
	m_totalCacheSize += file->size();
}

void ScImageCacheManager::fileRemoved(ScImageCacheFile * file)
{
	QString relFile = file->path(true);
	scDebug() << "removed" << relFile;
	m_totalCacheSize -= file->size();
	if (relFile.section('.', -1) == ScImageCacheProxy::metaSuffix)
		m_metaAge.remove(file);
}

ScImageCacheFile *ScImageCacheManager::getOldestCacheEntry()
{
	ScImageCacheFile *file;

	while ((file = m_metaAge.getOldest()) != 0)
	{
		QFileInfo info(file->path());
		if (!info.exists())
			scDebug() << "BUG: oldest file" << file->path() << "already removed?";
		else if (info.lastModified() == file->modified())
			break;
		m_root->updateFile(file->path(true));
	}

	return file;
}
void ScImageCacheManager::setEnabled(bool enableCache)
{
	m_isEnabled = enableCache;
}

bool ScImageCacheManager::setMaxCacheSizeMiB(int maxCacheSizeMiB)
{
	if (maxCacheSizeMiB < 1)
		return false;
	m_maxSizeMiB = maxCacheSizeMiB;
	m_maxTotalSize = Q_INT64_C(1048576)*static_cast<qint64>(maxCacheSizeMiB);
	return true;
}

bool ScImageCacheManager::setMaxCacheEntries(int maxCacheEntries)
{
	if (maxCacheEntries < 1)
		return false;
	m_maxEntries = maxCacheEntries;
	return true;
}

bool ScImageCacheManager::setCompressionLevel(int level)
{
	if (-1 <= m_compressionLevel && m_compressionLevel <= 9)
	{
		m_compressionLevel = level;
		return true;
	}
	return false;
}

int ScImageCacheManager::compressionLevel() const
{
	return m_compressionLevel;
}

QString ScImageCacheManager::lockDir()
{
	return ScPaths::getImageCacheDir() + "locks/";
}

QString ScImageCacheManager::masterLockFile()
{
	return lockDir() + "master." + ScLockedFile::lockSuffix;
}

QString ScImageCacheManager::writeLockTemplate()
{
	return lockDir() + "XXXXXXXX." + ScLockedFile::lockSuffix;
}

bool ScImageCacheManager::createLockDir()
{
	QDir dir(lockDir());
	return dir.exists() || dir.mkpath(lockDir());
}

bool ScImageCacheManager::acquireMasterLock()
{
	Q_ASSERT(!m_haveMasterLock);
	Q_ASSERT(m_writeLockCount == 0);
	if (m_haveMasterLock)
	{
		scDebug() << "BUG: master lock already acquired";
		return false;
	}
	if (m_writeLockCount > 0)
	{
		scDebug() << "BUG: attempt to acquire master lock with active write locks";
		return false;
	}
	if (!createLockDir())
	{
		scDebug() << "failed to create lock directory";
		return false;
	}
	QDir master(masterLockFile());
	if (master.exists())
	{
		scDebug() << "master lock already active";
		return false;
	}
	if (!master.mkdir(masterLockFile()))
	{
		scDebug() << "failed to create master lock directory";
		return false;
	}
	QDirIterator di(lockDir());
	while (di.hasNext())
	{
		di.next();
		QFileInfo info = di.fileInfo();
		if (info.isFile() && info.suffix() == ScLockedFile::lockSuffix)
		{
			scDebug() << "lock file present, cannot acquire master lock";
			if (!master.rmdir(masterLockFile()))
				scDebug() << "failed to remove master lock directory";
			return false;
		}
	}
	m_haveMasterLock = true;
	return true;
}

bool ScImageCacheManager::releaseMasterLock()
{
	Q_ASSERT(m_haveMasterLock);
	Q_ASSERT(m_writeLockCount == 0);
	if (!m_haveMasterLock)
	{
		scDebug() << "BUG: master lock not acquired";
		return false;
	}
	if (m_writeLockCount > 0)
	{
		scDebug() << "BUG: release of master lock with active write locks";
		return false;
	}
	QDir master(masterLockFile());
	if (!master.rmdir(masterLockFile()))
	{
		scDebug() << "failed to remove master lock directory";
		return false;
	}
	m_haveMasterLock = false;
	return true;
}

bool ScImageCacheManager::acquireWriteLock()
{
	if (!m_haveMasterLock && m_writeLockCount == 0)
	{
		Q_ASSERT(m_writeLockFile == 0);
		if (!createLockDir())
		{
			scDebug() << "failed to create lock directory";
			return false;
		}
		QDir master(masterLockFile());
		if (master.exists())
		{
			scDebug() << "master lock active";
			return false;
		}
		m_writeLockFile = new QTemporaryFile(writeLockTemplate());
		Q_CHECK_PTR(m_writeLockFile);
		if (!m_writeLockFile)
			return false;
		if (!m_writeLockFile->open())
		{
			scDebug() << "failed to create write lock file";
			delete m_writeLockFile;
			m_writeLockFile = 0;
			return false;
		}
		if (master.exists())
		{
			scDebug() << "master lock active";
			delete m_writeLockFile;
			m_writeLockFile = 0;
			return false;
		}
	}
	m_writeLockCount++;
	return true;
}

bool ScImageCacheManager::releaseWriteLock()
{
	if (m_writeLockCount == 0)
	{
		Q_ASSERT(m_writeLockFile == 0);
		return false;
	}
	m_writeLockCount--;
	if (!m_haveMasterLock)
	{
		Q_ASSERT(m_writeLockFile != 0);
		if (m_writeLockCount == 0)
		{
			delete m_writeLockFile;
			m_writeLockFile = 0;
		}
	}
	return true;
}

bool ScImageCacheManager::updateAccess(const QString & dir, AccessCounter from, AccessCounter to)
{
	// don't propagate updates until we have scanned the cache at least once
	return m_root ? m_root->updateAccess(dir, from, to) : false;
}

bool ScImageCacheManager::updateFile(const QString & file)
{
	// don't propagate updates until we have scanned the cache at least once
	return m_root ? m_root->updateFile(file) : false;
}

