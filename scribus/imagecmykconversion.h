/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGECMYKCONVERSION_H
#define IMAGECMYKCONVERSION_H

#include "scribusapi.h"
#include <QString>
#include <optional>
#include "colormgmt/sccolormgmtstructs.h"

class PageItem;

struct ImageCMYKConversionOptions
{
	QString sourceProfileName;      // empty: retain each frame's profile choice
	QString destinationProfileName; // empty: document CMYK output profile
	std::optional<eRenderIntent> renderingIntent;
	std::optional<bool> blackPointCompensation;
};

SCRIBUS_API bool canExportImageAsCMYKCopy(PageItem* item,
	const ImageCMYKConversionOptions& options, QString* error = nullptr);

// Exports the image source at full resolution, leaving the source and frame
// unchanged. Rejects alpha, frame effects, non-RGB sources and existing files.
SCRIBUS_API bool exportImageAsCMYKCopy(PageItem* item, const QString& destination,
	QString* error = nullptr);
SCRIBUS_API bool exportImageAsCMYKCopy(PageItem* item, const QString& destination,
	const ImageCMYKConversionOptions& options, QString* error = nullptr);

#endif
