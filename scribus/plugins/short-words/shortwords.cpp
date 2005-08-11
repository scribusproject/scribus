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
#include "pluginmanager.h"
#include "scpaths.h"
#include "scribus.h"
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qcursor.h>
#include <qprogressdialog.h>
#include <qfile.h>
#include <qdir.h>
#include <qcheckbox.h>

extern ScribusApp SCRIBUS_API *ScApp;


QString name()
{
	return QObject::tr("Short &Words...", "short words plugin");
}

PluginManager::PluginType type()
{
	return PluginManager::Standard;
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

void run(QWidget */*d*/, ScribusApp */*plug*/)
{
	ShortWords *sw = new ShortWords();
	/*delete sw;
	delete trans;*/
}

ShortWords::ShortWords()
{
	originalPage = ScApp->doc->currentPage->PageNr;
	VlnaDialog *dlg = new VlnaDialog(ScApp, "dlg", true, 0);
	if (dlg->exec() == QDialog::Accepted) {
		parse = new Parse();
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		ScApp->mainWindowStatusLabel->setText(QObject::tr("Short Words processing. Wait please...", "short words plugin"));
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
			ScApp->slotDocCh(true);
		// redraw document
		ScApp->view->DrawNew();
		QApplication::restoreOverrideCursor();
		ScApp->mainWindowStatusLabel->setText(QObject::tr("Short Words processing. Done.", "short words plugin"));
		ScApp->mainWindowProgressBar->reset();
		// set page where user calls vlna
		ScApp->view->GotoPage(originalPage);
	} // action
	delete dlg;
} // constructor

ShortWords::~ShortWords()
{
	delete parse;
}
