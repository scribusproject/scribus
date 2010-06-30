/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_scrapbook.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_Scrapbook::Prefs_Scrapbook(QWidget* parent, ScribusDoc* doc)
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
	autoSendCopiedToScrapCheckBox->setToolTip( "<qt>" + tr( "This enables the scrapbook to be used an extension to the copy/paste buffers. Simply copying an object or grouped object will send this to the Scrapbook automatically" ) + "</qt>");
	keepCopiedAcrossSessionsCheckBox->setToolTip( "<qt>" + tr( "This enables copied items to be kept permanently in the scrapbook" ) + "</qt>" );
	writePreviewsCheckBox->setToolTip( "<qt>" + tr("Save generated scrapbook previews in the scrapbook directory") + "</qt>");
	itemCountKeptInScrapbookSpinBox->setToolTip( "<qt>" + tr("The minimum number is 1; the maximum is 100") + "</qt>");
}

void Prefs_Scrapbook::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	autoSendCopiedToScrapCheckBox->setChecked(prefsData->scrapbookPrefs.doCopyToScrapbook);
	keepCopiedAcrossSessionsCheckBox->setChecked(prefsData->scrapbookPrefs.persistentScrapbook);
	writePreviewsCheckBox->setChecked(prefsData->scrapbookPrefs.writePreviews);
	itemCountKeptInScrapbookSpinBox->setValue(prefsData->scrapbookPrefs.numScrapbookCopies);
}

void Prefs_Scrapbook::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->scrapbookPrefs.doCopyToScrapbook=autoSendCopiedToScrapCheckBox->isChecked();
	prefsData->scrapbookPrefs.persistentScrapbook=keepCopiedAcrossSessionsCheckBox->isChecked();
	prefsData->scrapbookPrefs.writePreviews=writePreviewsCheckBox->isChecked();
	prefsData->scrapbookPrefs.numScrapbookCopies=itemCountKeptInScrapbookSpinBox->value();
}

