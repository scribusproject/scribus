/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PALETTELOADER_ADOBE_ACO_H
#define PALETTELOADER_ADOBE_ACO_H

#include <QHash>
#include <QString>

class ColorList;

#include "paletteloader.h"
#include "scribusapi.h"
#include "vgradient.h"

class PaletteLoader_Adobe_aco : public PaletteLoader
{
public:
	PaletteLoader_Adobe_aco();
	
	// Examine the passed file and test to see whether it appears to be
	// loadable with this loader. This test must be quick and simple.
	// It need not verify a file, just confirm that it looks like a 
	// supported file type
	virtual bool isFileSupported(const QString & fileName) const;
	
	// Import colors from specified file
	virtual bool importFile(const QString& fileName, bool merge);
};

#endif
