/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_scrapbook.h"
#include "prefsstructs.h"

Prefs_Scrapbook::Prefs_Scrapbook(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_Scrapbook::~Prefs_Scrapbook()
{
}

void Prefs_Scrapbook::languageChange()
{
	autoSendCopiedToScrapCheckBox->setToolTip( "<qt>" + tr( "" ) + "</qt>");
	keepCopiedAcrossSessionsCheckBox->setToolTip( "<qt>" + tr( "" ) + "</qt>" );
	itemCountKeptInScrapbookSpinBox->setToolTip( "<qt>" + tr("") + "</qt>");
}

void Prefs_Scrapbook::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	autoSendCopiedToScrapCheckBox->setChecked(prefsData->scrapbookPrefs.doCopyToScrapbook);
	keepCopiedAcrossSessionsCheckBox->setChecked(prefsData->scrapbookPrefs.persistentScrapbook);
	writePreviews->setChecked(prefsData->scrapbookPrefs.writePreviews);
	itemCountKeptInScrapbookSpinBox->setValue(prefsData->scrapbookPrefs.numScrapbookCopies);
}

void Prefs_Scrapbook::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->scrapbookPrefs.doCopyToScrapbook=autoSendCopiedToScrapCheckBox->isChecked();
	prefsData->scrapbookPrefs.persistentScrapbook=keepCopiedAcrossSessionsCheckBox->isChecked();
	prefsData->scrapbookPrefs.writePreviews=writePreviews->isChecked();
	prefsData->scrapbookPrefs.numScrapbookCopies=itemCountKeptInScrapbookSpinBox->value();
}

