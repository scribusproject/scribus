/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include <QString>
#include <QCursor>
#include <QDir>
#include <QWidget>

#include "scribus.h"
#include "scribuscore.h"
#include "nftemplate.h"
#include "nftdialog.h"
#include "scraction.h"
#include "menumanager.h"
#include "undomanager.h"
#include "prefsmanager.h"

int newfromtemplateplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* newfromtemplateplugin_getPlugin()
{
	NewFromTemplatePlugin* plug = new NewFromTemplatePlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void newfromtemplateplugin_freePlugin(ScPlugin* plugin)
{
	NewFromTemplatePlugin* plug = dynamic_cast<NewFromTemplatePlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

NewFromTemplatePlugin::NewFromTemplatePlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

NewFromTemplatePlugin::~NewFromTemplatePlugin() {};

void NewFromTemplatePlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "NewFromDocumentTemplate";
	// Action text for menu, including accel
	m_actionInfo.text = tr("New &from Template...");
	// Shortcut
	m_actionInfo.keySequence = "Ctrl+Alt+N";
	// Menu
	m_actionInfo.menu = "File";
	m_actionInfo.menuAfterName = "New";
	m_actionInfo.enabledOnStartup = true;
	m_actionInfo.needsNumObjects = -1;
}

const QString NewFromTemplatePlugin::fullTrName() const
{
	return QObject::tr("New From Template");
}

const ScActionPlugin::AboutData* NewFromTemplatePlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Riku Leino <riku@scribus.info>");
	about->shortDescription = tr("Load documents with predefined layout");
	about->description = tr("Start a document from a template made by other users or "
	                        "yourself (f.e. for documents you have a constant style).");
    // about->version
    // about->releaseDate
    // about->copyright
	about->license = "GPL";
	return about;
}

void NewFromTemplatePlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool NewFromTemplatePlugin::run(ScribusDoc* doc, QString target)
{
	Q_ASSERT(target.isNull());
	Nft = new MenuNFT();
	Q_CHECK_PTR(Nft);
	Nft->RunNFTPlug(doc);
	return true;
}


void MenuNFT::RunNFTPlug(ScribusDoc* /*doc*/)
{
	ScribusMainWindow* mw=ScCore->primaryMainWindow();
	nftdialog* nftdia = new nftdialog(mw, ScCore->getGuiLanguage(), PrefsManager::instance()->appPrefs.documentTemplatesDir);
	if (nftdia->exec())
	{
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
		if (mw->loadDoc(QDir::cleanPath(nftdia->currentDocumentTemplate->file)))
		{
			mw->doc->hasName = false;
			UndoManager::instance()->renameStack(nftdia->currentDocumentTemplate->name);
			mw->doc->DocName = nftdia->currentDocumentTemplate->name;
			mw->updateActiveWindowCaption(QObject::tr("Document Template: ") + nftdia->currentDocumentTemplate->name);
			QDir::setCurrent(PrefsManager::instance()->documentDir());
			mw->removeRecent(QDir::cleanPath(nftdia->currentDocumentTemplate->file));
		}
		qApp->restoreOverrideCursor();
	}
	delete nftdia;
}
