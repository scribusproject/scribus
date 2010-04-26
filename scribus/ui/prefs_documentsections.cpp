/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_documentsections.h"
#include "prefsstructs.h"

Prefs_DocumentSections::Prefs_DocumentSections(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_DocumentSections::~Prefs_DocumentSections()
{
}

void Prefs_DocumentSections::languageChange()
{
}

void Prefs_DocumentSections::restoreDefaults(struct ApplicationPrefs *prefsData)
{
//	autoSendCopiedToScrapCheckBox->setChecked(prefsData->scrapbookPrefs.doCopyToScrapbook);
}

void Prefs_DocumentSections::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
//	prefsData->scrapbookPrefs.doCopyToScrapbook=autoSendCopiedToScrapCheckBox->isChecked();
}

