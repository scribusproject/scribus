/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_shortwords.h"
#include "prefsstructs.h"

Prefs_ShortWords::Prefs_ShortWords(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_ShortWords::~Prefs_ShortWords()
{
}

void Prefs_ShortWords::languageChange()
{
}

void Prefs_ShortWords::restoreDefaults(struct ApplicationPrefs *prefsData)
{
}

void Prefs_ShortWords::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
}
