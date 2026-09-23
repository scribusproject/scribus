/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef COLLECTMANIFEST_H
#define COLLECTMANIFEST_H

#include <QString>

#include "scribusapi.h"

// Inventory the completed Collect for Output folder. Font permissions are
// deliberately not inferred from the file extension or copied font data.
SCRIBUS_API bool writeCollectManifest(const QString& directory, const QString& documentPath,
	QString* error = nullptr);

#endif
