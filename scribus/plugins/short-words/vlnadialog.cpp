/*! This is the Scribus Short Words GUI class implementation.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include "version.h"
#include "vlnadialog.h"
#include "scribus.h"
#include "config.h"
#include "configuration.h"

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

extern ScribusApp *ScApp;
extern ShortWords *shortWords;

/*!
Constructs a VlnaDialog as a child of 'parent', with the
name 'name' and widget flags set to 'f'.

The dialog will by default be modeless, unless you set 'modal' to
TRUE to construct a modal dialog.
*/
VlnaDialog::VlnaDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog(parent, name, modal, fl)
{
	if (!name)
		setName("VlnaDialog");

	VlnaDialogLayout = new QGridLayout(this, 1, 1, 11, 6, "VlnaDialogLayout");

	layout4 = new QVBoxLayout(0, 0, 6, "layout4");

	layout3 = new QHBoxLayout(0, 0, 6, "layout3");

	buttonGroup = new QButtonGroup(this, "buttonGroup");
	buttonGroup->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, buttonGroup->sizePolicy().hasHeightForWidth()));

	frameRadio = new QRadioButton(buttonGroup, "frameRadio");
	frameRadio->setGeometry(QRect(20, 30, 160, 22));

	pageRadio = new QRadioButton(buttonGroup, "pageRadio");
	pageRadio->setGeometry(QRect(20, 60, 140, 22));

	allRadio = new QRadioButton(buttonGroup, "allRadio");
	allRadio->setGeometry(QRect(20, 90, 140, 22));
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

	infoButton = new QPushButton(this, "infoButton");
	infoButton->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, infoButton->sizePolicy().hasHeightForWidth()));
	layout1->addWidget(infoButton);

	layout2->addLayout(layout1);
	layout3->addLayout(layout2);
	layout4->addLayout(layout3);

	userCheckBox = new QCheckBox(this, "userCheckBox");
	//userCheckBox->setGeometry(QRect(30, 140, 240, 22));
	//statusLabel = new QLabel(this, "statusLabel");
	//layout4->addWidget(statusLabel);
	layout4->addWidget(userCheckBox);

	VlnaDialogLayout->addLayout(layout4, 0, 0);

	QFile::exists(RC_PATH_USR) ? userCheckBox->setEnabled(TRUE) : userCheckBox->setEnabled(FALSE);

	languageChange();
	resize(QSize(306, 193).expandedTo(minimumSizeHint()));
	clearWState(WState_Polished);

	// signals and slots connections
	connect(okButton, SIGNAL(pressed()), this, SLOT(okButton_pressed()));
	connect(cancelButton, SIGNAL(pressed()), this, SLOT(cancelButton_pressed()));
	connect(infoButton, SIGNAL(pressed()), this, SLOT(infoButton_pressed()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
VlnaDialog::~VlnaDialog()
{
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void VlnaDialog::languageChange()
{
	setCaption(tr("Short Words", TR_INFO));
	buttonGroup->setTitle(tr("Apply unbreakable space on:", TR_INFO));
	frameRadio->setText(tr("&Selected frames", TR_INFO));
	pageRadio->setText(tr("Active &page", TR_INFO));
	allRadio->setText(tr("&All items", TR_INFO));
	okButton->setText(tr("&OK", TR_INFO));
	cancelButton->setText(tr("&Cancel", TR_INFO));
	infoButton->setText(tr("&Info and\nLanguages", TR_INFO));
	//statusLabel->setText(tr("Select action..."));
	userCheckBox->setText(tr("Replace defaults by user config", TR_INFO));
	// hints
	QToolTip::add(userCheckBox, tr("When the user config file exists \n(%1)\nyou can choose if you want to append your config\nto the global configuration by unchecked button.\n\nYou can replace predefined values by yours\nwith checked button too.", TR_INFO).arg(RC_PATH_USR));
	QToolTip::add(frameRadio, tr("Only selected frames processed.", TR_INFO));
	QToolTip::add(pageRadio, tr("Only actual page processed.", TR_INFO));
	QToolTip::add(allRadio, tr("All items in document processed.", TR_INFO));
}

void VlnaDialog::okButton_pressed()
{
	this->actionSelected = buttonGroup->id(buttonGroup->selected());
	accept();
}

void VlnaDialog::cancelButton_pressed()
{
	reject();
}

void VlnaDialog::infoButton_pressed()
{
	QString aMessage;
	aMessage += "<h1>";
	aMessage += tr("Short Words for Scribus", TR_INFO);
	aMessage += "</h1><b>";
	aMessage += tr("Available in the following languages", TR_INFO);
	aMessage += "</b><br>";
	aMessage += shortWords->cfg->getAvailableLanguages();
	QMessageBox::about(ScApp, tr("About Short Words", TR_INFO), aMessage);
	infoButton->setDown(FALSE);
}

void VlnaDialog::selectAction(int aAction)
{
	if (aAction!=0 and aAction!=1 and aAction!=2) {
			aAction = 0;
	}
	buttonGroup->setButton(aAction);
}
