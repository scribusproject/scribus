/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_documentinformation.h"
#include "prefsstructs.h"

Prefs_DocumentInformation::Prefs_DocumentInformation(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_DocumentInformation::~Prefs_DocumentInformation()
{
}

void Prefs_DocumentInformation::languageChange()
{
}

void Prefs_DocumentInformation::restoreDefaults(struct ApplicationPrefs *prefsData)
{
//	autoSendCopiedToScrapCheckBox->setChecked(prefsData->scrapbookPrefs.doCopyToScrapbook);
}

void Prefs_DocumentInformation::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
//	prefsData->scrapbookPrefs.doCopyToScrapbook=autoSendCopiedToScrapCheckBox->isChecked();
}

