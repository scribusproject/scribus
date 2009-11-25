/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "ui/prefs_printer.h"
#include "ui/newmarginwidget.h"
#include "prefsstructs.h"

Prefs_Printer::Prefs_Printer(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_Printer::~Prefs_Printer()
{
}
void Prefs_Printer::languageChange()
{

}

void Prefs_Printer::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	MarginStruct bleeds;
	bleedsWidget->setup(bleeds, 0, 0, false, false);

}

void Prefs_Printer::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
}

