/*! This is the Scribus Short Words GUI class implementation.

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
#include <qmenubar.h>
#include <qinputdialog.h>
#include <qprocess.h>

extern ScribusApp *ScApp;


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
	cfg = new Config();

	menuBar = new QMenuBar(this, "menuBar");
	QPopupMenu *editMenu = new QPopupMenu(this);
	editMenu->insertItem(tr("Edit &system configuration..."), this, SLOT(editSystemCfg()));
	editMenu->insertItem(tr("Edit &user configuration..."), this, SLOT(editUserCfg()));
	editMenu->insertSeparator();
	editMenu->insertItem(tr("S&etup editor..."), this, SLOT(setupEditor()));
	menuBar->insertItem(tr("&Edit"), editMenu);
	QPopupMenu *helpMenu = new QPopupMenu(this);
	helpMenu->insertItem(tr("&Info and Languages..."), this, SLOT(infoButton_pressed()));
	// FIXME: modal dialog + HelpBrowser navigation!
	//helpMenu->insertItem(tr("&Manual..."), this, SLOT(help()));
	menuBar->insertItem(tr("&Help"), helpMenu);

	VlnaDialogLayout = new QGridLayout(this, 1, 1, 11, 6, "VlnaDialogLayout");
	VlnaDialogLayout->setMenuBar(menuBar);

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

	/*infoButton = new QPushButton(this, "infoButton");
	infoButton->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, infoButton->sizePolicy().hasHeightForWidth()));
	layout1->addWidget(infoButton);*/

	layout2->addLayout(layout1);
	layout3->addLayout(layout2);
	layout4->addLayout(layout3);

	userCheckBox = new QCheckBox(this, "userCheckBox");
	layout4->addWidget(userCheckBox);

	VlnaDialogLayout->addLayout(layout4, 0, 0);

	QFile::exists(RC_PATH_USR) ? userCheckBox->setEnabled(TRUE) : userCheckBox->setEnabled(FALSE);

	languageChange();
	resize(QSize(306, 193).expandedTo(minimumSizeHint()));
	clearWState(WState_Polished);

	// cfg
	cfg->userConfig ? userCheckBox->setChecked(TRUE) : userCheckBox->setChecked(FALSE);
	selectAction(cfg->action);

	// signals and slots connections
	connect(okButton, SIGNAL(pressed()), this, SLOT(okButton_pressed()));
	connect(cancelButton, SIGNAL(pressed()), this, SLOT(cancelButton_pressed()));
	//connect(infoButton, SIGNAL(pressed()), this, SLOT(infoButton_pressed()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
VlnaDialog::~VlnaDialog()
{
	userCheckBox->isChecked() ? cfg->userConfig = 1 : cfg->userConfig = 0;
	cfg->action = actionSelected;
	cfg->saveConfig();
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void VlnaDialog::languageChange()
{
	setCaption(tr("Short Words", "short words plugin"));
	buttonGroup->setTitle(tr("Apply unbreakable space on:", "short words plugin"));
	frameRadio->setText(tr("&Selected frames", "short words plugin"));
	pageRadio->setText(tr("Active &page", "short words plugin"));
	allRadio->setText(tr("&All items", "short words plugin"));
	okButton->setText(tr("&OK", "short words plugin"));
	cancelButton->setText(tr("&Cancel", "short words plugin"));
	//infoButton->setText(tr("&Info and\nLanguages", "short words plugin"));
	//statusLabel->setText(tr("Select action..."));
	userCheckBox->setText(tr("Replace defaults by user config", "short words plugin"));
	// hints
	QToolTip::add(userCheckBox, tr("When the user config file exists \n(%1)\nyou can choose if you want to append your config\nto the global configuration by unchecked button.\n\nYou can replace predefined values by yours\nwith checked button too.", "short words plugin").arg(RC_PATH_USR));
	QToolTip::add(frameRadio, tr("Only selected frames processed.", "short words plugin"));
	QToolTip::add(pageRadio, tr("Only actual page processed.", "short words plugin"));
	QToolTip::add(allRadio, tr("All items in document processed.", "short words plugin"));
}

void VlnaDialog::okButton_pressed()
{
	actionSelected = buttonGroup->id(buttonGroup->selected());
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
	aMessage += tr("Short Words for Scribus", "short words plugin");
	aMessage += "</h1><b>";
	aMessage += tr("Available in the following languages", "short words plugin");
	aMessage += "</b><br>";
	aMessage += cfg->getAvailableLanguages();
	QMessageBox::about(ScApp, tr("About Short Words", "short words plugin"), aMessage);
	//infoButton->setDown(FALSE);
}

void VlnaDialog::selectAction(int aAction)
{
	if (aAction!=0 and aAction!=1 and aAction!=2) {
			aAction = 0;
	}
	buttonGroup->setButton(aAction);
}

void VlnaDialog::editSystemCfg()
{
	editFile(RC_PATH);
}

void VlnaDialog::editUserCfg()
{
	editFile(RC_PATH_USR);
}

void VlnaDialog::editFile(QString fName)
{
	if (cfg->editor == "")
		setupEditor();
	QFileInfo fi(fName);
	if (fi.isWritable())
	{
		QMessageBox::warning( this,
							  tr("Short Words"),
							  "<qt>" + tr("You are starting to edit read-only file.\n%1").arg(fName) + "</qt>");
	}
	QProcess *p = new QProcess(this);
	p->addArgument(cfg->editor);
	p->addArgument(fName);
	if (!p->start())
	{
		QMessageBox::warning( this,
							  tr("Short Words"),
							  "<qt>" + tr("Application '%1' error. Cannot be started.").arg(cfg->editor) + "</qt>");
		qDebug("SW4S: Error starting editor");
	}
}

void VlnaDialog::setupEditor()
{
	QString ed = QInputDialog::getText(tr("Short Words setup"), "<qt>" + tr("Enter name of the plain text editor executable:")+ "</qt>", QLineEdit::Normal, cfg->editor);
	if (ed.length() != 0)
		cfg->editor = ed;
}

void VlnaDialog::help()
{
	HelpBrowser *dia = new HelpBrowser(0, QObject::tr("Short Words Manual"), ScApp->getGuiLanguage(), "tutorials/short-words");
	dia->show();
}
