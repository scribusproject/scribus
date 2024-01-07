/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pdfoptions.h"

PDFOptions::VerifyResults PDFOptions::verify()
{
	return verify(static_cast<QString*>(nullptr));
}

PDFOptions::VerifyResults PDFOptions::verify(QString* /*problemDescription*/)
{
	// TODO: implement this method
	return Verify_NoError;
}

bool PDFOptions::exportsLayers() const
{
	if (Version.supportsOCGs())
		return useLayers;
	return false;
}

bool PDFOptions::supportsEmbeddedOpenTypeFonts() const
{
	return Version.supportsEmbeddedOpenTypeFonts();
}

bool PDFOptions::supportsOCGs() const
{
	return Version.supportsOCGs();
}

bool PDFOptions::supportsTransparency() const
{
	return Version.supportsTransparency();
}