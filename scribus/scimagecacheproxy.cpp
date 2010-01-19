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

#include <QCryptographicHash>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "sclockedfile.h"
#include "scimagecacheproxy.h"
#include "scimagecachemanager.h"
#include "scimagecachewriteaction.h"
#include "scpaths.h"
#include "util_file.h"

#define SC_DEBUG_FILE defined(DEBUG_SCIMAGECACHE)
#include "scdebug.h"

// MD5 has been chosen as a hash algorithm as it less prone to collisions than MD4,
// but at the same time twice as fast to compute as SHA-1. Furthermore, it's 32 bits
// shorter than SHA-1, making the filenames at least a little shorter.

namespace {
	const QString CACHEFILE_VERSION("1");
	const QCryptographicHash::Algorithm HASH_ALGORITHM = QCryptographicHash::Md5;
	const int CACHEDIR_LEVELS = 2;
	const char * const imageFormat = "PNG";

	inline QString absolutePath(const QString & fn)
	{
		return ScImageCacheManager::absolutePath(fn);
	}
}

const QString ScImageCacheProxy::metaSuffix("xml");
const QString ScImageCacheProxy::referenceSuffix("ref");
const QString ScImageCacheProxy::imageSuffix("png");

ScImageCacheProxy::ScImageCacheProxy(const QString & fn)
	: m_filename(fn), m_isEnabled(ScImageCacheManager::instance().enabled())
{
	if (!m_isEnabled)
		return;

	QFileInfo imfo(m_filename);

	if (imfo.exists())
	{
		addMetadata("version", CACHEFILE_VERSION);
		addMetadata("path", m_filename);
		addMetadata("size", QString::number(imfo.size()));
		addMetadata("lastModifiedUTC", imfo.lastModified().toUTC().toString(Qt::ISODate));
	}
}

ScImageCacheProxy::~ScImageCacheProxy()
{
	// nothing :)
}

void ScImageCacheProxy::addMetadata(const QString & key, const QString & value)
{
	m_metadata[key] = value;
}

void ScImageCacheProxy::addModifier(const QString & key, const QString & value)
{
	m_modifier[key] = value;
	m_metanameCache.clear();
}

void ScImageCacheProxy::delModifier(const QString & key)
{
	m_modifier.remove(key);
	m_metanameCache.clear();
}

void ScImageCacheProxy::addInfo(const QString & key, const QString & value)
{
	m_imginfo[key] = value;
}

QString ScImageCacheProxy::getInfo(const QString & key) const
{
	return m_imginfo[key];
}

QString ScImageCacheProxy::imageFile(const QString & base)
{
	return base + "." + imageSuffix;
}

QString ScImageCacheProxy::referenceFile(const QString & base)
{
	return base + "." + referenceSuffix;
}

QString ScImageCacheProxy::getBaseName(const QString & metafile)
{
	QString base;
	return loadMetadata(metafile, 0, 0, 0, &base) ? base : QString();
}

bool ScImageCacheProxy::loadMetadata(ScLockedFile *file, MetaMap *meta, MetaMap *mod, MetaMap *info, QString *base)
{
	QXmlStreamReader xml(file->io());

	bool baseFound = false;
	bool metaFound = false;
	bool modFound = false;
	bool infoFound = false;

	while (!xml.atEnd())
	{
		if (xml.readNext() == QXmlStreamReader::StartElement)
		{
			QXmlStreamAttributes attr = xml.attributes();

			if (xml.name() == "cache")
			{
				if (attr.hasAttribute("base"))
				{
					if (base)
						*base = attr.value("base").toString();

					baseFound = true;
				}
			}
			else if (xml.name() == "metadata")
			{
				if (meta)
				{
					meta->clear();

					foreach (QXmlStreamAttribute a, attr)
						(*meta)[a.name().toString()] = a.value().toString();
				}

				metaFound = true;
			}
			else if (xml.name() == "modifier")
			{
				if (mod)
				{
					mod->clear();

					foreach (QXmlStreamAttribute a, attr)
						(*mod)[a.name().toString()] = a.value().toString();
				}

				modFound = true;
			}
			else if (xml.name() == "imginfo")
			{
				if (info)
				{
					info->clear();

					foreach (QXmlStreamAttribute a, attr)
						(*info)[a.name().toString()] = a.value().toString();
				}

				infoFound = true;
			}
		}
	}

	if (xml.hasError())
	{
		scDebug() << "error parsing" << file->name() << xml.errorString() << "in line" << xml.lineNumber() << "column" << xml.columnNumber();
		return false;
	}

	if (!baseFound) scDebug() << "base not found";
	if (!metaFound) scDebug() << "meta not found";
	if (!modFound) scDebug() << "mod not found";
	if (!infoFound) scDebug() << "info not found";

	return baseFound && metaFound && modFound && infoFound;
}

bool ScImageCacheProxy::loadMetadata(const QString & fn, MetaMap *meta, MetaMap *mod, MetaMap *info, QString *base)
{
	ScLockedFileRO file(absolutePath(fn));
	if (!file.open())
	{
		scDebug() << "failed to open" << fn;
		return false;
	}
	return loadMetadata(&file, meta, mod, info, base);
}

bool ScImageCacheProxy::loadMetadata(MetaMap *meta, MetaMap *mod, MetaMap *info, QString *base) const
{
	return loadMetadata(metaName(), meta, mod, info, base);
}

void ScImageCacheProxy::saveMetadata(ScLockedFile *file, const MetaMap & meta, const MetaMap & mod, const MetaMap & info, const QString & base)
{
	QXmlStreamWriter xml(file->io());

	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement("cache");
	xml.writeAttribute("base", base);
	xml.writeStartElement("metadata");
	for (MetaMap::const_iterator i = meta.constBegin(); i != meta.constEnd(); i++)
		xml.writeAttribute(i.key(), i.value());
	xml.writeEndElement();
	xml.writeStartElement("modifier");
	for (MetaMap::const_iterator i = mod.constBegin(); i != mod.constEnd(); i++)
		xml.writeAttribute(i.key(), i.value());
	xml.writeEndElement();
	xml.writeStartElement("imginfo");
	for (MetaMap::const_iterator i = info.constBegin(); i != info.constEnd(); i++)
		xml.writeAttribute(i.key(), i.value());
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeEndDocument();
}

bool ScImageCacheProxy::canUseCachedImage() const
{
	if (!enabled())
		return false;

	MetaMap cmeta;  // cached metadata
	MetaMap cmod;   // cached modifiers
	QString base;

	if (m_metadata.isEmpty())
	{
		scDebug() << "cannot use cached image, no metadata";
		return false;
	}

	if (!loadMetadata(&cmeta, &cmod, 0, &base))
	{
		scDebug() << "cannot use cached image, load metadata failed";
		return false;
	}

	QString fn = absolutePath(imageFile(base));
	QFileInfo info(fn);

	if (!info.exists())
		return false;

	if (cmeta.size() != m_metadata.size())
		return false;

	if (cmod.size() != m_modifier.size())
		return false;

	for (MetaMap::const_iterator i = m_metadata.constBegin(); i != m_metadata.constEnd(); i++)
		if (cmeta[i.key()] != i.value())
			return false;

	for (MetaMap::const_iterator i = m_modifier.constBegin(); i != m_modifier.constEnd(); i++)
		if (cmod[i.key()] != i.value())
			return false;

	return true;
}

QString ScImageCacheProxy::addDirLevels(QString name)
{
	Q_ASSERT(name.size() > CACHEDIR_LEVELS);
	if (name.size() <= CACHEDIR_LEVELS)
	{
		scDebug() << "BUG: invalid name" << name << "passed to addDirLevels";
		return QString();
	}
	for (int i = CACHEDIR_LEVELS; i > 0; i--)
		name.insert(i, '/');
	return name;
}

QString ScImageCacheProxy::imageBaseName(const QImage & image) const
{
	if (!m_metadata.contains("size"))
	{
		scDebug() << "size not present in metadata";
		return QString();
	}
	QCryptographicHash hash(HASH_ALGORITHM);
	for (int i = 0; i < image.height(); i++)
		hash.addData(reinterpret_cast<const char *>(image.scanLine(i)), image.bytesPerLine());
	return addDirLevels(hash.result().toHex()) + "-" + m_metadata["size"];
}

const QString & ScImageCacheProxy::metaName() const
{
	if (m_metanameCache.isEmpty())
	{
		QCryptographicHash hash(HASH_ALGORITHM);
		hash.addData(m_filename.toUtf8());
		for (MetaMap::const_iterator i = m_modifier.constBegin(); i != m_modifier.constEnd(); i++)
		{
			hash.addData(i.key().toUtf8());
			hash.addData(i.value().toUtf8());
		}
		m_metanameCache = addDirLevels(hash.result().toHex()) + "." + metaSuffix;
	}
	return m_metanameCache;
}

bool ScImageCacheProxy::createCacheDir()
{
	QString cachedir = ScPaths::getImageCacheDir();
	QDir cdir(cachedir);

	if (!cdir.exists())
	{
		scDebug() << "creating" << cachedir;
		if (!cdir.mkpath(cachedir))
		{
			scDebug() << "could not create" << cachedir;
			return false;
		}
	}

	return true;
}

bool ScImageCacheProxy::load(QImage & image)
{
	if (!enabled())
		return false;

	QString base;

	if (!loadMetadata(&m_metadata, &m_modifier, &m_imginfo, &base))
	{
		scDebug() << "could not load metadata for" << m_filename;
		return false;
	}

	QString fn = absolutePath(imageFile(base));

	if (!image.load(fn))
	{
		scDebug() << "could not load cached image for" << m_filename;
		return false;
	}

	scDebug() << "successfully loaded" << m_filename << "from" << fn;
	return true;
}

bool ScImageCacheProxy::save(const QImage & image)
{
	if (!enabled())
		return false;

	scDebug() << "saving" << m_filename << "to cache";

	Q_ASSERT(!m_metadata.isEmpty());
	Q_ASSERT(!m_imginfo.isEmpty());

	if (m_metadata.isEmpty())
	{
		scDebug() << "BUG: attempt to save cache without metadata";
		return false;
	}

	if (m_imginfo.isEmpty())
	{
		scDebug() << "BUG: attempt to save cache without image info";
		return false;
	}

	if (!createCacheDir())
		return false;

	// The cache write lock does not prevent other instances from writing to
	// the cache. It only prevents other instances from setting a master lock.

	ScImageCacheWriteAction action;

	if (!action.start())
		return false;

	// Computing the imageBaseName is rather longish, so do it before locking
	// the files in order to keep the lock time as short as possible.

	QString base = imageBaseName(image);

	Q_ASSERT(!base.isEmpty());

	if (base.isEmpty())
	{
		scDebug() << "BUG: could not create image base name";
		return false;
	}

	scDebug() << "storing as base" << base;

	QString refName = base + "." + referenceSuffix;
	QString imgName = base + "." + imageSuffix;
	QString oldBase;
	QString oldRefName;
	QString oldImgName;
	bool haveOldMeta = false;
	bool haveOldRef = false;

	ScLockedFileRW meta(absolutePath(metaName()));
	ScLockedFileRW ref(absolutePath(refName));
	ScLockedFileRW img(absolutePath(imgName));
	ScLockedFileRW oldRef;

	if (!meta.createPath())
	{
		scDebug() << "could not create path for" << meta.name();
		return false;
	}

	if (!ref.createPath())
	{
		scDebug() << "could not create path for" << ref.name();
		return false;
	}

	// Try to acquire necessary locks. Locks will be automatically cleaned up
	// upon destruction of the lock object, so we can safely return at any time.

	if (!action.add(metaName()))
	{
		scDebug() << "could not add lock for" << metaName();
		return false;
	}

	// This is a bit tricky... if the meta file already exists, it will most
	// probably point to a different reference file. We need to access this
	// "old" reference file as well in order to decrement its reference count.

	if (meta.exists())
	{
		if (!loadMetadata(0, 0, 0, &oldBase))
		{
			scDebug() << "could not read metadata from" << meta.name();
			return false;
		}

		haveOldMeta = true;
		oldRefName = oldBase + "." + referenceSuffix;
		oldImgName = oldBase + "." + imageSuffix;

		if (oldBase != base)
		{
			oldRef.setName(absolutePath(oldRefName));

			if (!action.add(oldRefName))
			{
				scDebug() << "could not add" << oldRefName << "to action";
				return false;
			}
			if (!action.add(oldImgName))
			{
				scDebug() << "could not add" << oldImgName << "to action";
				return false;
			}

			haveOldRef = oldRef.exists();

			if (!haveOldRef)
				oldRef.unlock();
		}
	}

	if (!action.add(refName))
	{
		scDebug() << "could not add" << refName << "to action";
		return false;
	}

	if (!action.add(imgName))
	{
		scDebug() << "could not add" << imgName << "to action";
		return false;
	}

	// The meta and reference files have both been locked now, so we're safe to
	// write to the cache. Locking the reference file implicitly also locks the
	// image file. We can also safely open all files already, as they are only
	// temporary files and don't conflict with other files in the cache.

	// cases:
	// * completely new entry, none of the files exist
	//   - create image file
	//   - create reference file with refcount 1
	//   - update meta file
	// * new meta file, but reference file exists
	//   - increment reference count
	//   - update meta file
	//   - keep image
	// * old meta file exists and reference files are identical
	//   - keep reference file
	//   - update meta file
	//   - keep image
	// * old meta file exists and reference files differ
	//   - decrement reference count of old reference file
	//   - continue as above

	// Open the metafile. If this fails, everything else is quite useless.

	if (!meta.open())
	{
		scDebug() << "could not open meta file" << meta.name();
		return false;
	}

	// Update the reference files if necessary.

	if (haveOldRef)
	{
		// we don't care if this fails
		// if there's any problem, the next cache cleanup will detect it
		unrefImage(&oldRef, oldImgName);
	}

	if (oldBase != base)
	{
		if (!refImage(&ref))
		{
			scDebug() << "could not reference new image" << ref.name();
			return false;
		}
	}

	// Write image file if necessary. Existing image files are *never* re-written
	// under the assumption that there are no collisions.

	if (img.exists())
	{
		scDebug() << "cached image for" << m_filename << "already exists in" << img.name();
	}
	else
	{
		if (!img.open())
		{
			scDebug() << "could not open image file" << img.name();
			return false;
		}
		int level = ScImageCacheManager::instance().compressionLevel();
		level = level < 0 ? level : 10*(9 - level);
		scDebug() << "compressing" << imageFormat << "image, quality =" << level;
		if (!image.save(img.io(), imageFormat, level))
		{
			scDebug() << "could not save image" << img.name();
			return false;
		}

		img.commit();

		scDebug() << "successfully stored" << m_filename << "in cache as" << img.name();
	}

	// Save the metadata. 

	saveMetadata(&meta, m_metadata, m_modifier, m_imginfo, base);
	meta.commit();

	// Explicit commit will also trigger access file update

	action.commit();

	return true;
}

bool ScImageCacheProxy::loadRef(ScLockedFile *file, int & refcount)
{
	QXmlStreamReader xml(file->io());
	bool refcountFound = false;

	while (!xml.atEnd())
	{
		if (xml.readNext() == QXmlStreamReader::StartElement)
		{
			QXmlStreamAttributes attr = xml.attributes();

			if (xml.name() == "reference")
				if (attr.hasAttribute("count"))
					refcount = attr.value("count").toString().toInt(&refcountFound);
		}
	}

	if (xml.hasError())
	{
		scDebug() << "error parsing" << file->name() << xml.errorString() << "in line" << xml.lineNumber() << "column" << xml.columnNumber();
		return false;
	}

	return refcountFound;
}

void ScImageCacheProxy::saveRef(ScLockedFile *file, int refcount)
{
	QXmlStreamWriter xml(file->io());

	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement("reference");
	xml.writeAttribute("count", QString::number(refcount));
	xml.writeEndElement();
	xml.writeEndDocument();
}

bool ScImageCacheProxy::getRefCount(const QString & reffile, int & refcount)
{
	return getRefCountAbs(absolutePath(reffile), refcount);
}

bool ScImageCacheProxy::getRefCountAbs(const QString & reffile, int & refcount)
{
	ScLockedFileRO ro(reffile);
	if (!ro.open())
	{
		scDebug() << "could not open reference file" << ro.name();
		return false;
	}
	if (!loadRef(&ro, refcount))
	{
		scDebug() << "could not read reference file" << ro.name();
		return false;
	}
	return true;
}

bool ScImageCacheProxy::fixRefCount(const QString & reffile, int refcount)
{
	ScLockedFileRW rw(absolutePath(reffile));
	if (!rw.open())
	{
		scDebug() << "could not open reference file" << rw.name();
		return false;
	}
	saveRef(&rw, refcount);
	return rw.commit();
}

bool ScImageCacheProxy::removeCacheEntry(const QString & metafile, bool haveMasterLock)
{
	ScImageCacheWriteAction action(haveMasterLock);

	if (!action.start())
		return false;

	ScLockedFileRW meta(absolutePath(metafile));

	if (!action.add(metafile))
	{
		scDebug() << "could not add" << metafile;
		return false;
	}

	QString base = getBaseName(metafile);

	meta.remove();

	if (base.isEmpty())
	{
		scDebug() << "empty basename in" << metafile;
	}
	else
	{
		QString reffile = referenceFile(base);
		QString imgfile = imageFile(base);

		if (!action.add(reffile))
		{
			scDebug() << "could not add" << reffile;
			return false;
		}

		if (!action.add(imgfile))
		{
			scDebug() << "could not add" << imgfile;
			return false;
		}

		ScLockedFileRW ref(absolutePath(reffile));

		// we don't care if these fail
		// if there's any problem, the next cache cleanup will detect it
		unrefImage(&ref, absolutePath(imgfile));
	}

	action.commit();

	return true;
}

bool ScImageCacheProxy::refImage(ScLockedFile *file)
{
	int refcount = 0;

	if (file->exists() && !getRefCountAbs(file->name(), refcount))
		return false;

	refcount++;

	if (!file->open())
	{
		scDebug() << "could not open reference file for writing" << file->name();
		return false;
	}

	saveRef(file, refcount);

	return file->commit();
}

bool ScImageCacheProxy::unrefImage(ScLockedFile *file, const QString & imageName)
{
	int refcount = 0;

	if (file->exists())
	{
		if (!getRefCountAbs(file->name(), refcount))
			return false;
	}
	else
	{
		// could also happen if someone else is messing with the cache
		scDebug() << "BUG: attempt to unref non-existent reference file" << file->name();
		return false;
	}

	refcount--;

	if (refcount == 0)
	{
		bool rv = true;

		scDebug() << "refcount dropped to zero for" << file->name();

		if (!file->remove())
		{
			scDebug() << "could not remove reference file" << file->name();
			rv = false;
		}

		if (QFile::exists(imageName) && !QFile::remove(imageName))
		{
			scDebug() << "could not remove image file" << imageName;
			rv = false;
		}

		return rv;
	}

	if (!file->open())
	{
		scDebug() << "could not open reference file for writing" << file->name();
		return false;
	}

	saveRef(file, refcount);

	return file->commit();
}

bool ScImageCacheProxy::touch() const
{
	scDebug() << "touching metafile" << metaName();
	return touchFile(absolutePath(metaName()));
}
