/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qcheckbox.h>
#include <qspinbox.h>

#include "tabscrapbook.h"
//#include "tabscrapbook.moc"

TabScrapbook::TabScrapbook(QWidget* parent, const char* name)
	: TabScrapbookBase(parent, name, 0)
{
}

void TabScrapbook::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	useScrapBookasExtension->setChecked(prefsData->doCopyToScrapbook);
	persistentScrapbook->setChecked(prefsData->persistentScrapbook);
	numScrapCopies->setValue(prefsData->numScrapbookCopies);
}
