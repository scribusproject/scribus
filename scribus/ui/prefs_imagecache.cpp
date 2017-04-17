/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFileDialog>
#include <QString>

#include "prefs_imagecache.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_ImageCache::Prefs_ImageCache(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_ImageCache::~Prefs_ImageCache()
{
}

void Prefs_ImageCache::languageChange()
{
	enableImageCacheCheckBox->setToolTip( "<qt>" + tr( "Enabling the image cache will significantly speed up the loading of images. Enable the cache if you are often working on large documents with lots of images and if you have plenty of disk space in your application data directory." ) + "</qt>" );
	cacheSizeLimitSpinBox->setToolTip( "<qt>"+ tr("Limit the total size of all files in the image cache directory to this amount.")+"</qt>" );
	cacheEntryLimitSpinBox->setToolTip( "<qt>" + tr( "Limit the number of cache entries to this number." ) + "</qt>" );
	compressionLevelSpinBox->setToolTip( "<qt>" + tr( "Set the level of compression for images in the cache. Higher values result in smaller cache files but also make writes to the cache slower." ) + "</qt>" );
}

void Prefs_ImageCache::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	enableImageCacheCheckBox->setChecked(prefsData->imageCachePrefs.cacheEnabled);
	cacheSizeLimitSpinBox->setValue(prefsData->imageCachePrefs.maxCacheSizeMiB);
	cacheEntryLimitSpinBox->setValue(prefsData->imageCachePrefs.maxCacheEntries);
	compressionLevelSpinBox->setValue(prefsData->imageCachePrefs.compressionLevel);
}

void Prefs_ImageCache::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->imageCachePrefs.cacheEnabled = enableImageCacheCheckBox->isChecked();
	prefsData->imageCachePrefs.maxCacheSizeMiB = cacheSizeLimitSpinBox->value();
	prefsData->imageCachePrefs.maxCacheEntries = cacheEntryLimitSpinBox->value();
	prefsData->imageCachePrefs.compressionLevel = compressionLevelSpinBox->value();
}

