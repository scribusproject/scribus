/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_miscellaneous.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_Miscellaneous::Prefs_Miscellaneous(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_Miscellaneous::~Prefs_Miscellaneous()
{
}

void Prefs_Miscellaneous::languageChange()
{
}

void Prefs_Miscellaneous::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	confirmFontReplacementsCheckBox->setChecked(prefsData->fontPrefs.askBeforeSubstitute);
	previewParaStylesCheckBox->setChecked(prefsData->miscPrefs.haveStylePreview);
	useStandardLoremIpsumCheckBox->setChecked(prefsData->miscPrefs.useStandardLI);
	loremIpsumParaCountSpinBox->setValue(prefsData->miscPrefs.paragraphsLI);
}

void Prefs_Miscellaneous::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->fontPrefs.askBeforeSubstitute = confirmFontReplacementsCheckBox->isChecked();
	prefsData->miscPrefs.haveStylePreview = previewParaStylesCheckBox->isChecked();
	prefsData->miscPrefs.useStandardLI = useStandardLoremIpsumCheckBox->isChecked();
	prefsData->miscPrefs.paragraphsLI = loremIpsumParaCountSpinBox->value();
}

