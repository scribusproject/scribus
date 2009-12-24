/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_pdfexport.h"
#include "prefsstructs.h"

Prefs_PDFExport::Prefs_PDFExport(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);

}

Prefs_PDFExport::~Prefs_PDFExport()
{
}

void Prefs_PDFExport::languageChange()
{
}

void Prefs_PDFExport::restoreDefaults(struct ApplicationPrefs *prefsData)
{

}


void Prefs_PDFExport::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{

}
