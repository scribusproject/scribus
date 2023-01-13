/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PALETTELOADER_ADOBE_ACB_H
#define PALETTELOADER_ADOBE_ACB_H

#include <QHash>
#include <QString>

#include "paletteloader.h"
#include "scribusapi.h"
#include "vgradient.h"

class ColorList;

class PaletteLoader_Adobe_acb : public PaletteLoader
{
public:
	PaletteLoader_Adobe_acb() = default;
	
	// Examine the passed file and test to see whether it appears to be
	// loadable with this loader. This test must be quick and simple.
	// It need not verify a file, just confirm that it looks like a 
	// supported file type
	bool isFileSupported(const QString & fileName) const override;
	
	// Import colors from specified file
	bool importFile(const QString& fileName, bool merge) override;
};

#endif
