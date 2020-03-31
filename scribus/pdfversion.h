/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDFVERSION_H
#define PDFVERSION_H

#include "scribusapi.h"

class SCRIBUS_API PDFVersion
{
public:
	enum Version
	{
		PDF_13  = 13,
		PDF_14  = 14,
		PDF_15  = 15,
		PDF_16  = 16,
		PDF_X1a = 11,
		PDF_X3  = 12,
		PDF_X4  = 10,
		PDFVersion_Min = 10,
		PDFVersion_Max = 16,
	};
	
	PDFVersion() {};
	PDFVersion(PDFVersion::Version ver) { m_version = ver; }

	operator Version() const { return m_version; }
	Version version() const { return m_version; }
	
	PDFVersion& operator=(PDFVersion::Version ver) { m_version = ver; return *this; } 
	bool operator==(PDFVersion::Version ver) const { return (m_version == ver) ; }
	bool operator!=(PDFVersion::Version ver) const { return (m_version != ver) ; }

	bool operator>=(PDFVersion::Version ver) const { return (m_version >= ver) ; }
	bool operator>(PDFVersion::Version ver) const { return (m_version > ver) ; }

	bool operator<=(PDFVersion::Version ver) const { return (m_version <= ver) ; }
	bool operator<(PDFVersion::Version ver) const { return (m_version < ver) ; }

	bool isPDFX() const;

	bool supports128BitsEncryption() const;
	bool supportsEmbeddedOpenTypeFonts() const;
	bool supportsOCGs() const;
	bool supportsPDF15PresentationEffects() const;
	bool supportsTransparency() const;
	
private:
	Version m_version { PDF_14 };
};

#endif
