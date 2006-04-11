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
#include "vlnadialog.moc"
#include "scribus.h"
#include "scpaths.h"
#include "configuration.h"
#include "helpbrowser.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qinputdialog.h>
#include <qprocess.h>

#include "commonstrings.h"

extern ScribusMainWindow SCRIBUS_API *ScMW;


SWDialog::SWDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog(parent, name, modal, fl)
{
	if (!name)
		setName("SWDialog");
	cfg = new SWConfig();

	SWDialogLayout = new QGridLayout(this, 1, 1, 11, 6, "SWDialogLayout");

	layout4 = new QVBoxLayout(0, 0, 6, "layout4");

	layout3 = new QHBoxLayout(0, 0, 6, "layout3");

	buttonGroup = new QButtonGroup(this, "buttonGroup");

	QGridLayout *gridLayout = new QGridLayout(buttonGroup);
	gridLayout->setSpacing(6);
	gridLayout->setMargin(9);
	
	QVBoxLayout *vboxLayout = new QVBoxLayout();
	vboxLayout->setSpacing(6);
	vboxLayout->setMargin(0);

	frameRadio = new QRadioButton(buttonGroup, "frameRadio");
	vboxLayout->addWidget(frameRadio);

	pageRadio = new QRadioButton(buttonGroup, "pageRadio");
	vboxLayout->addWidget(pageRadio);

	allRadio = new QRadioButton(buttonGroup, "allRadio");
	vboxLayout->addWidget(allRadio);

	gridLayout->addLayout(vboxLayout, 0, 0);
	buttonGroup->setMinimumWidth(250); // these Germans withe their long words...
	buttonGroup->adjustSize();

	layout3->addWidget(buttonGroup);

	layout2 = new QVBoxLayout(0, 0, 6, "layout2");
	QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	layout2->addItem(spacer);

	layout1 = new QVBoxLayout(0, 0, 6, "layout1");

	okButton = new QPushButton(this, "okButton");
	okButton->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, okButton->sizePolicy().hasHeightForWidth()));
	layout1->addWidget(okButton);

	cancelButton = new QPushButton(this, "cancelButton");
	cancelButton->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, cancelButton->sizePolicy().hasHeightForWidth()));
	layout1->addWidget(cancelButton);

	layout2->addLayout(layout1);
	layout3->addLayout(layout2);
	layout4->addLayout(layout3);

	SWDialogLayout->addLayout(layout4, 0, 0);

	languageChange();
	resize(QSize(306, 193).expandedTo(minimumSizeHint()));
	clearWState(WState_Polished);

	selectAction(cfg->action);

	// signals and slots connections
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_pressed()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_pressed()));
	connect(buttonGroup, SIGNAL(clicked(int)), this, SLOT(buttonGroup_clicked(int)));
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
	setCaption(tr("Short Words", "short words plugin"));
	buttonGroup->setTitle(tr("Apply unbreakable space on:", "short words plugin"));
	frameRadio->setText(tr("&Selected frames", "short words plugin"));
	pageRadio->setText(tr("Active &page", "short words plugin"));
	allRadio->setText(tr("&All items", "short words plugin"));
	okButton->setText(CommonStrings::tr_OK);
	cancelButton->setText(CommonStrings::tr_Cancel);
	QToolTip::add(frameRadio, tr("Only selected frames processed.", "short words plugin"));
	QToolTip::add(pageRadio, tr("Only actual page processed.", "short words plugin"));
	QToolTip::add(allRadio, tr("All items in document processed.", "short words plugin"));
}

void SWDialog::okButton_pressed()
{
	actionSelected = buttonGroup->id(buttonGroup->selected());
	accept();
}

void SWDialog::cancelButton_pressed()
{
	reject();
}

void SWDialog::selectAction(int aAction)
{
	if (aAction!=0 && aAction!=1 && aAction!=2) {
			aAction = 0;
	}
	buttonGroup->setButton(aAction);
}

void SWDialog::buttonGroup_clicked(int key)
{
	buttonGroup->find(key)->setFocus();
}
