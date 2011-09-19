/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This is the Scribus Short Words GUI class implementation.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include "version.h"
#include "vlnadialog.h"

#include "scribus.h"
#include "scpaths.h"
#include "configuration.h"
#include "helpbrowser.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QToolTip>

#include "commonstrings.h"
#include "langmgr.h"

SWDialog::SWDialog(QWidget* parent) : QDialog(parent)
{
	setupUi(this);

	cfg = new SWConfig();

	languageComboBox->addItems(cfg->getAvailableLanguagesList());

	languageChange();
	resize(minimumSizeHint());

	// signals and slots connections
	connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
			 this, SLOT(okButton_pressed()));
	connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
			 this, SLOT(cancelButton_pressed()));
	connect(styleCheckBox, SIGNAL(toggled(bool)),
			 languageComboBox, SLOT(setDisabled(bool)));

	selectAction(cfg->action);
	styleCheckBox->setChecked(cfg->useStyle);
	languageComboBox->setCurrentIndex(cfg->currentLanguage);
}

/*
 *  Destroys the object and frees any allocated resources
 */
SWDialog::~SWDialog()
{
}

int SWDialog::actionSelected()
{
	if (frameRadio->isChecked())
		return 0;
	else if (pageRadio->isChecked())
		return 1;
	else if (allRadio->isChecked())
		return 2;
	return 0;
}

bool SWDialog::useStyleLang()
{
	return styleCheckBox->isChecked();
}

QString SWDialog::lang()
{
	return LanguageManager::instance()->getLangFromTransLang(languageComboBox->currentText());
}

void SWDialog::savePrefs()
{
	cfg->action = actionSelected();
	cfg->useStyle = styleCheckBox->isChecked();
	cfg->currentLanguage = languageComboBox->currentIndex();
	cfg->saveConfig();
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SWDialog::languageChange()
{
	setWindowTitle( tr("Short Words", "short words plugin"));
	buttonGroup->setTitle( tr("Apply Unbreakable Space To:", "short words plugin"));
	frameRadio->setText( tr("&Selected Frames", "short words plugin"));
	pageRadio->setText( tr("Active &Page", "short words plugin"));
	allRadio->setText( tr("&All Items", "short words plugin"));
// 	okButton->setText(CommonStrings::tr_OK);
// 	cancelButton->setText(CommonStrings::tr_Cancel);
	frameRadio->setToolTip( tr("Only selected frames processed.", "short words plugin"));
	pageRadio->setToolTip( tr("Only actual page processed.", "short words plugin"));
	allRadio->setToolTip( tr("All items in document processed.", "short words plugin"));
}

void SWDialog::okButton_pressed()
{
	savePrefs();
	accept();
}

void SWDialog::cancelButton_pressed()
{
	savePrefs();
	reject();
}

void SWDialog::selectAction(int aAction)
{
	if (aAction!=0 && aAction!=1 && aAction!=2)
		aAction = 0;
	if (aAction == 0)
		frameRadio->setChecked(true);
	else if (aAction == 1)
		pageRadio->setChecked(true);
	else if (aAction == 2)
		allRadio->setChecked(true);
}
