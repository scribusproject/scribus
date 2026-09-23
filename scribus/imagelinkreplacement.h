/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef IMAGELINKREPLACEMENT_H
#define IMAGELINKREPLACEMENT_H

#include <QString>

#include "scribusapi.h"

class ScribusDoc;

struct ImageLinkReplacementResult
{
	int matched {0};
	int replaced {0};
	int failed {0};
};

// Match the exact external image path, including images on master pages and
// inside groups. Embedded and empty frames are never candidates.
SCRIBUS_API ImageLinkReplacementResult replaceImageLinks(ScribusDoc* doc, const QString& sourcePath,
	const QString& replacementPath, bool dryRun = false);

#endif
