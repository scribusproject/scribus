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
#include "configuration.h"
#include "parse.h"
#include "scpaths.h"
#include "scribus.h"
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qcursor.h>
#include <qprogressdialog.h>
#include <qfile.h>
#include <qdir.h>
#include <qcheckbox.h>

int scribusshortwords_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scribusshortwords_getPlugin()
{
	ShortWordsPlugin* plug = new ShortWordsPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void scribusshortwords_freePlugin(ScPlugin* plugin)
{
	ShortWordsPlugin* plug = dynamic_cast<ShortWordsPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ShortWordsPlugin::ShortWordsPlugin() :
	ScActionPlugin(ScPlugin::PluginType_Action)
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

ShortWordsPlugin::~ShortWordsPlugin() {};

void ShortWordsPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ShortWords";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Short &Words...", "short words plugin");
	// Menu
	m_actionInfo.menu = "Extras";
	m_actionInfo.enabledOnStartup = true;
}

const QString ShortWordsPlugin::fullTrName() const
{
	return QObject::tr("Short Words");
}

const ScActionPlugin::AboutData* ShortWordsPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Petr Van\xc4\x9bk <petr@scribus.info>, "
			"Łukasz [DeeJay1] Jernaś <deejay1@nsj.srem.pl>, "
			"Maciej Hanski, Zdenko Podobny, Mikolaj Machowski <mikmach@wp.pl>, "
			"Ludi Maciel, Frederico Guimarães, "
			"Claudio Beccari <claudio.beccari@polito.it>, "
			"Christoph Schäfer <christoph-schaefer@gmx.de>");
	about->shortDescription = tr("Short Words");
	about->description = tr("Special plug-in for adding non-breaking spaces before or after so called short words.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void ShortWordsPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool ShortWordsPlugin::run(QString target)
{
	Q_ASSERT(target.isEmpty());
	ShortWords *sw = new ShortWords();
	/*delete sw;
	delete trans;*/
	return true;
}

ShortWords::ShortWords()
{
	originalPage = ScApp->doc->currentPage->pageNr();
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
				parse->parsePage(ScApp->doc->currentPage->pageNr());
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
