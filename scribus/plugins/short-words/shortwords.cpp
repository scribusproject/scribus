/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This is the Scribus Short Words plugin interface implementation.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>

This program is free software - see LICENSE file in the distribution
or documentation
*/

#include <QCheckBox>
#include <QCursor>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProgressBar>
#include <QTextCodec>
#include <QPixmap>

#include "configuration.h"
#include "iconmanager.h"
#include "parse.h"
#include "prefs_shortwords.h"
#include "scpage.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "shortwords.h"
#include "swdialog.h"
#include "version.h"


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

ShortWordsPlugin::ShortWordsPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

ShortWordsPlugin::~ShortWordsPlugin() = default;

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
	m_actionInfo.menuAfterName = "extrasDeHyphenateText";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
}

QString ShortWordsPlugin::fullTrName() const
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
			"Christoph Schäfer <christoph-schaefer@gmx.de>, "
			"intrigeri <intrigeri@boum.org>");
	about->shortDescription = tr("Short Words");
	about->description = tr("Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: ") + SWConfig::getAvailableLanguages();
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

bool ShortWordsPlugin::run(ScribusDoc* doc, const QString& target)
{
	if (doc == nullptr)
		return false;
	Q_ASSERT(target.isEmpty());

	uint originalPage = doc->currentPage()->pageNr();
	SWDialog *dlg = new SWDialog(doc->scMW());
	if (dlg->exec() == QDialog::Accepted) {
		SWParse *parse = new SWParse();
		QApplication::changeOverrideCursor(QCursor(Qt::WaitCursor));
		if (!dlg->useStyleLang())
			parse->lang = dlg->lang();
		else
			parse->lang.clear(); // get it from style

		doc->scMW()->setStatusBarInfoText(QObject::tr("Short Words processing. Wait please...", "short words plugin"));
		switch (dlg->actionSelected())
		{
			case 0:
				parse->parseSelection(doc);
				break;
			case 1:
				parse->parsePage(doc, doc->currentPage()->pageNr());
				break;
			case 2:
				parse->parseAll(doc);
				break;
		} // switch
		// enable "Save" icon
		if (parse->modify > 0)
			doc->changed();
		delete parse;
		// redraw document
		doc->view()->DrawNew();
		QApplication::changeOverrideCursor(Qt::ArrowCursor);
		doc->scMW()->setStatusBarInfoText(QObject::tr("Short Words processing. Done.", "short words plugin"));
		doc->scMW()->mainWindowProgressBar->reset();
		// set page where user calls vlna
		doc->view()->GotoPage(originalPage);
	} // action
	delete dlg;
	return true;
}

bool ShortWordsPlugin::newPrefsPanelWidget(QWidget* parent, Prefs_Pane*& panel)
{
	panel = new Prefs_ShortWords(parent);
	Q_CHECK_PTR(panel);
	return true;
}
