/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_experimental.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_Experimental::Prefs_Experimental(QWidget* parent, ScribusDoc* /*doc*/)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Experimental");
	m_icon = "misc_16.png";
}

Prefs_Experimental::~Prefs_Experimental() = default;

void Prefs_Experimental::languageChange()
{
	// No need to do anything here, the UI language cannot change while prefs dialog is opened
}

void Prefs_Experimental::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	enableNotesCheckBox->setChecked(prefsData->experimentalFeaturePrefs.notesEnabled);
}

void Prefs_Experimental::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->experimentalFeaturePrefs.notesEnabled = enableNotesCheckBox->isChecked();
}

