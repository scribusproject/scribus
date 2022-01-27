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

#ifndef SCIMAGECACHEPROXY_H
#define SCIMAGECACHEPROXY_H

#include "scconfig.h"
#include "scribusapi.h"

#include <QImage>
#include <QString>
#include <QMap>

class ScImage;
class ScLockedFile;
class ScImageCacheManager;

/**
  * @brief Scribus image cache proxy
  * @author Marcus Holland-Moritz
  */
class SCRIBUS_API ScImageCacheProxy
{
public:
	static const QString metaSuffix;         //!< Meta file suffix
	static const QString referenceSuffix;    //!< Reference file suffix
	static const QString imageSuffix;        //!< Cache image file suffix

	/**
	* @brief Construct a cache proxy object
	* @param fn Full path to the original image file
	*/
	ScImageCacheProxy(const QString & fn);
	~ScImageCacheProxy();

	/**
	* @brief Check if the image cache is enabled
	*/
	bool enabled() const { return m_isEnabled; }
	/**
	* @brief Get original image file name
	*/
	const QString & getFilename() const { return m_filename; }
	/**
	* @brief Load image from cache
	* @param image QImage object to which to load the cached image
	* @return \c true if the image could be loaded, \c false otherwise
	*/
	bool load(QImage & image);
	/**
	* @brief Save image to cache
	* @param image QImage object from which to save the cached image
	* @return \c true if the image could be saved, \c false otherwise
	*/
	bool save(const QImage & image);
	/**
	* @brief Touch an image in the cache
	* @return \c true if the image could be touched, \c false otherwise
	*/
	bool touch() const;
	/**
	* @brief Add or overwrite image metadata
	*/
	void addMetadata(const QString & key, const QString & value);
	/**
	* @brief Add or overwrite an image modifier
	*/
	void addModifier(const QString & key, const QString & value);
	/**
	* @brief Delete an image modifier
	*/
	void delModifier(const QString & key);
	/**
	* @brief Check if the cached image can be used in place of the original
	* @return \c true if the cached image can be used, \c false otherwise
	*/
	bool canUseCachedImage() const;

	/**
	* @brief Add image information
	*/
	void addInfo(const QString & key, const QString & value);
	/**
	* @brief Retrieve image information
	*/
	QString getInfo(const QString & key) const;

	/**
	* @brief Get base name of reference/image file from meta file
	* @param metafile Path to meta file relative to the cache root directory
	* @return Reference/image file basename, i.e. path and filename without the suffix
	*/
	static QString getBaseName(const QString & metafile);
	/**
	* @brief Get reference count from a reference file
	* @param reffile Path to reference file relative to the cache root directory
	* @param refcount Reference to the variable receiving the reference counter
	* @return \c true if the reference count could be read, \c false otherwise
	*/
	static bool getRefCount(const QString & reffile, int & refcount);
	/**
	* @brief Set reference count in a reference file
	* @param reffile Path to reference file relative to the cache root directory
	* @param refcount The reference count to write to the file
	* @return \c true if the reference count could be written, \c false otherwise
	*/
	static bool fixRefCount(const QString & reffile, int refcount);
	/**
	* @brief Remove an entry from the image cache
	*
	* Attempts to remove a meta file from the image cache. Decrements the
	* reference counter of the referenced image. If the reference counter
	* drops to zero, the image and its reference file will also be removed.
	* 
	* @param metafile Path to meta file relative to the cache root directory
	* @param haveMasterLock \c true if a master lock has already been acquired
	* @return \c true if the reference count could be written, \c false otherwise
	*/
	static bool removeCacheEntry(const QString & metafile, bool haveMasterLock = false);

private:
	// Don't turn this into a QHash, element order is important
	typedef QMap<QString, QString> MetaMap;

	const QString m_filename;
	const bool m_isEnabled;
	mutable QString m_metanameCache;
	MetaMap m_metadata;
	MetaMap m_modifier;
	MetaMap m_imginfo;

	static QString imageFile(const QString & base);
	static QString referenceFile(const QString & base);

	static bool createCacheDir();
	static QString addDirLevels(QString name);

	const QString & metaName() const;
	QString imageBaseName(const QImage & image) const;

	bool loadMetadata(MetaMap *meta, MetaMap *mod, MetaMap *info, QString *base) const;

	static bool loadMetadata(ScLockedFile *file, MetaMap *meta, MetaMap *mod, MetaMap *info, QString *base);
	static bool loadMetadata(const QString & fn, MetaMap *meta, MetaMap *mod, MetaMap *info, QString *base);
	static void saveMetadata(ScLockedFile *file, const MetaMap & map, const MetaMap & mod, const MetaMap & info, const QString & base);

	static bool getRefCountAbs(const QString & reffile, int & refcount);
	static bool loadRef(ScLockedFile *file, int & refcount);
	static void saveRef(ScLockedFile *file, int refcount);
	static bool refImage(ScLockedFile *file);
	static bool unrefImage(ScLockedFile *file, const QString & imageName);
};

#endif
