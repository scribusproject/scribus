/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDFVERSIONCOMBO_H
#define PDFVERSIONCOMBO_H

#include <QComboBox>

#include "scribusapi.h"
#include "pdfoptions.h"
#include "pdfversion.h"

class PdfVersionModel;

/*!
    \class PdfVersionCombo pdfversioncombo.h
    \brief The PdfVersionCombo widget is a combo box for displaying PDF versions
*/
class SCRIBUS_API PdfVersionCombo : public QComboBox 
{
	Q_OBJECT

public:
	PdfVersionCombo(QWidget* parent = nullptr);
	~PdfVersionCombo();

	PDFVersion version() const;

	bool versionIs(const PDFVersion& version) const;
	bool versionIsPDFX() const;

	void setVersion(const PDFVersion& version);
	void setPDFXEnabled(bool enabled);
	
private:
	bool m_isPdfXEnabled;
	PdfVersionModel* m_pdfVersionModel; 
};

#endif
