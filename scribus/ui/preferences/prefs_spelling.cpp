/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QDebug>

#include "prefs_spelling.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_Spelling::Prefs_Spelling(QWidget* parent, ScribusDoc* /*doc*/)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	m_caption = tr("Spelling");
	m_icon = "pref-hyphenator";
}

Prefs_Spelling::~Prefs_Spelling() = default;

void Prefs_Spelling::languageChange()
{
	// No need to do anything here, the UI language cannot change while prefs dialog is opened
}

void Prefs_Spelling::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	checkSpellingWhileTypingCheckbox->setChecked(prefsData->spellCheckPrefs.liveSpellCheckEnabled);
	showMisspeltIndicatorCheckBox->setChecked(prefsData->spellCheckPrefs.showMisspeltIndicator);
	maxSpellingSuggestionsSpinBox->setValue(prefsData->spellCheckPrefs.maxSuggestions);
	spellingDelaySpinBox->setValue(prefsData->spellCheckPrefs.debounceDelay);
}

void Prefs_Spelling::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->spellCheckPrefs.liveSpellCheckEnabled = checkSpellingWhileTypingCheckbox->isChecked();
	prefsData->spellCheckPrefs.showMisspeltIndicator = showMisspeltIndicatorCheckBox->isChecked();
	prefsData->spellCheckPrefs.maxSuggestions = maxSpellingSuggestionsSpinBox->value();
	prefsData->spellCheckPrefs.debounceDelay = spellingDelaySpinBox->value();
}

