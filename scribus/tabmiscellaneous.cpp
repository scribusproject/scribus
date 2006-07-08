/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qcheckbox.h>
#include <qspinbox.h>

#include "tabmiscellaneous.h"
#include "tabmiscellaneous.moc"

TabMiscellaneous::TabMiscellaneous(QWidget* parent, const char* name)
	: TabMiscellaneousBase(parent, name, 0)
{
}

void TabMiscellaneous::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	AskForSubs->setChecked(prefsData->askBeforeSubstituite);
	stylePreview->setChecked(prefsData->haveStylePreview);
	startUpDialog->setChecked(prefsData->showStartupDialog);
	useStandardLI->setChecked(prefsData->useStandardLI);
	paragraphsLI->setValue(prefsData->paragraphsLI);
}
