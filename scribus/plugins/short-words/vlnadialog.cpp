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

SWDialog::SWDialog(QWidget* parent) : QDialog(parent)
{
	setModal(true);
	cfg = new SWConfig();

	SWDialogLayout = new QGridLayout(this);
	SWDialogLayout->setMargin(10);
	SWDialogLayout->setSpacing(5);

	layout4 = new QVBoxLayout;
	layout4->setMargin(0);
	layout4->setSpacing(5);

	layout3 = new QHBoxLayout;
	layout3->setMargin(0);
	layout3->setSpacing(5);

	buttonGroup = new QGroupBox(this);

	QGridLayout *gridLayout = new QGridLayout(buttonGroup);
	gridLayout->setSpacing(5);
	gridLayout->setMargin(10);
	
	QVBoxLayout *vboxLayout = new QVBoxLayout;
	vboxLayout->setSpacing(5);
	vboxLayout->setMargin(0);

	frameRadio = new QRadioButton(buttonGroup);
	vboxLayout->addWidget(frameRadio);

	pageRadio = new QRadioButton(buttonGroup);
	vboxLayout->addWidget(pageRadio);

	allRadio = new QRadioButton(buttonGroup);
	vboxLayout->addWidget(allRadio);

	gridLayout->addLayout(vboxLayout, 0, 0);
//	buttonGroup->setMinimumWidth(250); // these Germans withe their long words...
	buttonGroup->adjustSize();

	layout3->addWidget(buttonGroup);

	layout2 = new QVBoxLayout;
	layout2->setMargin(0);
	layout2->setSpacing(5);
	QSpacerItem* spacer = new QSpacerItem(2, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);
	layout2->addItem(spacer);

	layout1 = new QVBoxLayout;
	layout1->setMargin(0);
	layout1->setSpacing(5);

	okButton = new QPushButton(this);
	okButton->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, okButton->sizePolicy().hasHeightForWidth()));
	layout1->addWidget(okButton);

	cancelButton = new QPushButton(this);
	cancelButton->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, cancelButton->sizePolicy().hasHeightForWidth()));
	layout1->addWidget(cancelButton);

	layout2->addLayout(layout1);
	layout3->addLayout(layout2);
	layout4->addLayout(layout3);

	SWDialogLayout->addLayout(layout4, 0, 0);

	languageChange();
	resize(minimumSizeHint());

	selectAction(cfg->action);

	// signals and slots connections
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_pressed()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_pressed()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
SWDialog::~SWDialog()
{
	cfg->action = actionSelected;
	cfg->saveConfig();
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SWDialog::languageChange()
{
	setWindowTitle( tr("Short Words", "short words plugin"));
	buttonGroup->setTitle( tr("Apply unbreakable space on:", "short words plugin"));
	frameRadio->setText( tr("&Selected frames", "short words plugin"));
	pageRadio->setText( tr("Active &page", "short words plugin"));
	allRadio->setText( tr("&All items", "short words plugin"));
	okButton->setText(CommonStrings::tr_OK);
	cancelButton->setText(CommonStrings::tr_Cancel);
	QToolTip::add(frameRadio, tr("Only selected frames processed.", "short words plugin"));
	QToolTip::add(pageRadio, tr("Only actual page processed.", "short words plugin"));
	QToolTip::add(allRadio, tr("All items in document processed.", "short words plugin"));
}

void SWDialog::okButton_pressed()
{
	if (frameRadio->isChecked())
		actionSelected = 0;
	else if (pageRadio->isChecked())
		actionSelected = 1;
	else if (allRadio->isChecked())
		actionSelected = 2;
	accept();
}

void SWDialog::cancelButton_pressed()
{
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
