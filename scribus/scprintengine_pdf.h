/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPRINTENGINE_PDF_H
#define SCPRINTENGINE_PDF_H

#include "pdfoptions.h"
#include "scprintengine.h"
#include "scribusapi.h"

class SCRIBUS_API ScPrintEngine_PDF : public ScPrintEngine
{
	friend class PrintPreviewCreator_PDF;

public:
	ScPrintEngine_PDF(ScribusDoc& doc);
	~ScPrintEngine_PDF() {}

	bool print(PrintOptions& options) override;

protected:
	PrefsManager& m_prefsManager;

	int  createPDFFile(const QString& fileName, const PrintOptions& options, QString& errorMessage);
	void setupFontEmbedding(PDFOptions& options);
};

#endif
