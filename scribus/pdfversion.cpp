/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pdfversion.h"

bool PDFVersion::isPDFX() const
{
	if (m_version == PDF_X1a)
		return true;
	if (m_version == PDF_X3)
		return true;
	if (m_version == PDF_X4)
		return true;
	return false;
}

bool PDFVersion::supports128BitsEncryption() const
{
	if (m_version == PDF_14)
		return true;
	if (m_version == PDF_15)
		return true;
	if (m_version == PDF_16)
		return true;
	return false;
}

bool PDFVersion::supportsEmbeddedOpenTypeFonts() const
{
	if (m_version == PDF_16)
		return true;
	if (m_version == PDF_X4)
		return true;
	return false;
}

bool PDFVersion::supportsOCGs() const
{
	if (m_version == PDF_15)
		return true;
	if (m_version == PDF_16)
		return true;
	if (m_version == PDF_X4)
		return true;
	return false;
}

bool PDFVersion::supportsTransparency() const
{
	if (m_version == PDF_14)
		return true;
	if (m_version == PDF_15)
		return true;
	if (m_version == PDF_16)
		return true;
	if (m_version == PDF_X4)
		return true;
	return false;
}

bool PDFVersion::supportsPDF15PresentationEffects() const
{
	return (m_version == PDF_15 || m_version == PDF_16);
}