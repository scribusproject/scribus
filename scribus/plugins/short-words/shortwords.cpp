/*! This is the Scribus Short Words plugin interface implementation.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include "shortwords.h"
#include "shortwords.moc"
#include "version.h"
#include "vlnadialog.h"

#include "scribus.h"
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qcursor.h>
#include <qprogressdialog.h>
#include <qfile.h>
#include <qdir.h>

ShortWords* shortWords;
extern ScribusApp *ScApp;

QString Name()
{
	return QObject::tr("Short &Words...", TR_INFO);
}


int Type()
{
	return 6;
}

int ID()
{
	return 11;
}

QString actionName()
{
	return "ShortWords";
}

QString actionKeySequence()
{
	return "";
}

QString actionMenu()
{
	return "Extras";
}

QString actionMenuAfterName()
{
	return "";
}

bool actionEnabledOnStartup()
{
	return true;
}

void Run(QWidget *d, ScribusApp *plug)
{
	ShortWords *sw = new ShortWords();
	/*delete sw;
	delete trans;*/
}

ShortWords::ShortWords()
{
	shortWords = this;
	originalPage = ScApp->doc->currentPage->PageNr;
	cfg = new Config();
	VlnaDialog *dlg = new VlnaDialog(ScApp, "dlg", TRUE, 0);

	cfg->userConfig ? dlg->userCheckBox->setChecked(TRUE) : dlg->userCheckBox->setChecked(FALSE);
	dlg->selectAction(cfg->action);
	if (dlg->exec() == QDialog::Accepted) {
		parse = new Parse();
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		ScApp->FMess->setText(QObject::tr("Short Words processing. Wait please...", TR_INFO));
		dlg->userCheckBox->isChecked() ? cfg->userConfig = 1 : cfg->userConfig = 0;
		cfg->action = dlg->actionSelected;
		switch (dlg->actionSelected) {
			case 0:
				parse->parseSelection();
				break;
			case 1:
				parse->parsePage(ScApp->doc->currentPage->PageNr);
				break;
			case 2:
				parse->parseAll();
				break;
		} // switch
		// enable "Save" icon
		if (parse->modify > 0)
			ScApp->slotDocCh(TRUE);
		// redraw document
		ScApp->view->DrawNew();
		QApplication::restoreOverrideCursor();
		ScApp->FMess->setText(QObject::tr("Short Words processing. Done.", TR_INFO));
		ScApp->FProg->reset();
		// set page where user calls vlna
		ScApp->view->GotoPage(originalPage);
	} // action
	delete dlg;
	cfg->saveConfig();
} // constructor

ShortWords::~ShortWords()
{
	delete cfg;
	delete parse;
}
