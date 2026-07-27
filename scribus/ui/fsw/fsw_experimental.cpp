/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "fsw_experimental.h"
#include "prefsstructs.h"
#include <QEvent>

FSW_Experimental::FSW_Experimental(QWidget* parent)
	: QWizardPage(parent)
{
	setupUi(this);
	experimentalCheck->setChecked(false);   // default off
}

void FSW_Experimental::restoreDefaults(const ApplicationPrefs* prefsData)
{
	if (!prefsData)
		return;
	experimentalCheck->setChecked(prefsData->experimentalFeaturePrefs.notesEnabled);
}

bool FSW_Experimental::experimentalEnabled() const
{
	return experimentalCheck->isChecked();
}

void FSW_Experimental::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange)
		retranslateUi(this);
	QWizardPage::changeEvent(e);
}
