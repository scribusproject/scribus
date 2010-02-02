/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_scripter.h"
#include "prefsstructs.h"

Prefs_Scripter::Prefs_Scripter(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);

}

Prefs_Scripter::~Prefs_Scripter()
{
}

void Prefs_Scripter::languageChange()
{
}

void Prefs_Scripter::restoreDefaults(struct ApplicationPrefs *prefsData)
{
}

void Prefs_Scripter::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
}

void Prefs_Scripter::apply()
{

}
