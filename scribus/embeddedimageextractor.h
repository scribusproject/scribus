/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EMBEDDEDIMAGEEXTRACTOR_H
#define EMBEDDEDIMAGEEXTRACTOR_H

#include <QSet>
#include <QString>

#include "scribusapi.h"

enum class ImageExtractionConflict
{
	KeepBoth,
	Replace,
	Skip
};

struct SCRIBUS_API ImageExtractionPath
{
	QString path;
	bool skipped { false };
	bool renamed { false };
};

SCRIBUS_API QString sanitizedImageBaseName(const QString& name);
SCRIBUS_API QString embeddedImageExtension(const QString& sourcePath);
SCRIBUS_API QString suggestedEmbeddedImageFileName(const QString& frameName,
	const QString& sourcePath, int fallbackNumber);
SCRIBUS_API ImageExtractionPath resolveImageExtractionPath(const QString& directory,
	const QString& fileName, ImageExtractionConflict conflict, QSet<QString>* reservedPaths = nullptr);
SCRIBUS_API bool copyEmbeddedImageBytes(const QString& sourcePath, const QString& destinationPath,
	bool replaceExisting, QString* errorMessage = nullptr);

#endif
