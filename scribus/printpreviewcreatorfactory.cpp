/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "printpreviewcreatorfactory.h"
#ifdef Q_OS_WIN32
#include "printpreviewcreator_gdi.h"
#endif
#include "printpreviewcreator_pdf.h"
#include "printpreviewcreator_ps.h"

PrintPreviewCreator* PrintPreviewCreatorFactory::create(ScribusDoc* doc, PrintLanguage prnLanguage)
{
#ifdef Q_OS_WIN32
	if (prnLanguage == PrintLanguage::WindowsGDI)
	{
		PrintPreviewCreator* generator = new PrintPreviewCreator_GDI(doc);
		return generator;
	}
#endif

	if (prnLanguage == PrintLanguage::PostScript1 || prnLanguage == PrintLanguage::PostScript2 || prnLanguage == PrintLanguage::PostScript3)
	{
		PrintPreviewCreator* generator = new PrintPreviewCreator_PS(doc);
		return generator;
	}

	if (prnLanguage == PrintLanguage::PDF)
	{
		PrintPreviewCreator* generator = new PrintPreviewCreator_PDF(doc);
		return generator;
	}

	return nullptr;
}
