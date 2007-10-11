/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scconfig.h"
#include "wmfimportplugin.h"

#include <QFile>
#include <QRegExp>
#include <QCursor>

#include "customfdialog.h"
#include "scribus.h"
#include "scribusXml.h"
#include "prefsfile.h"

#include <cmath>
#include "commonstrings.h"
#include "fpointarray.h"
#include "menumanager.h"
#include "prefsmanager.h"
#include "pageitem.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "selection.h"
#include "undomanager.h"
#include "loadsaveplugin.h"
#include "util.h"
#include "util_formats.h"
#include "fonts/scfontmetrics.h"

#include "wmfimport.h"

using namespace std;

int wmfimplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* wmfimplugin_getPlugin()
{
	WMFImportPlugin* plug = new WMFImportPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void wmfimplugin_freePlugin(ScPlugin* plugin)
{
	WMFImportPlugin* plug = dynamic_cast<WMFImportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

WMFImportPlugin::WMFImportPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file format
	// support.
	languageChange();
}

void WMFImportPlugin::addToMainWindowMenu(ScribusMainWindow *mw)
{
	importAction->setEnabled(true);
	connect( importAction, SIGNAL(triggered()), SLOT(import()) );
	mw->scrMenuMgr->addMenuItem(importAction, "FileImport");
}

WMFImportPlugin::~WMFImportPlugin()
{
	unregisterAll();
};

void WMFImportPlugin::languageChange()
{
	importAction->setText( tr("Import &WMF..."));
	// (Re)register file format support.
	unregisterAll();
	registerFormats();
}

const QString WMFImportPlugin::fullTrName() const
{
	return QObject::tr("WMF Import");
}

const ScActionPlugin::AboutData* WMFImportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Jean Ghali <jghali@scribus.info>";
	about->shortDescription = tr("Imports WMF Files");
	about->description = tr("Imports most WMF files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void WMFImportPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void WMFImportPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::WMF);
	fmt.formatId = FORMATID_WMFIMPORT;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::WMF);
	fmt.nameMatch = QRegExp("\\."+FormatsManager::instance()->extensionListForFormat(FormatsManager::WMF, 1)+"$", Qt::CaseInsensitive);
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::WMF);
	fmt.priority = 64;
	registerFormat(fmt);
}

bool WMFImportPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	// TODO: identify valid WMF
	return true;
}

bool WMFImportPlugin::loadFile(const QString & fileName, const FileFormat & /* fmt */, int flags, int /*index*/)
{
	// For now, "load file" and import are the same thing for this plugin
	return import(fileName, flags);
}

bool WMFImportPlugin::import(QString filename, int flags)
{
	if (!checkFlags(flags))
		return false;
	m_Doc=ScCore->primaryMainWindow()->doc;
	ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
	if (filename.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("WMFPlugin");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(mw, wdir, QObject::tr("Open"), FormatsManager::instance()->fileDialogFormatList(FormatsManager::WMF));
		if (diaf.exec())
		{
			filename = diaf.selectedFile();
			prefs->set("wdir", filename.left(filename.lastIndexOf("/")));
		}
		else
			return true;
	}
	
	/*if (UndoManager::undoEnabled() && m_Doc)
	{
		UndoManager::instance()->beginTransaction(m_Doc->currentPage()->getUName(),Um::IImageFrame,Um::ImportWMF, filename, Um::IWMF);
	}
	else if (UndoManager::undoEnabled() && !m_Doc)
		UndoManager::instance()->setUndoEnabled(false);*/
	WMFImport *dia = new WMFImport(mw, flags);
	dia->import(filename, flags);
	Q_CHECK_PTR(dia);
	/*if (UndoManager::undoEnabled())
		UndoManager::instance()->commit();
	else
		UndoManager::instance()->setUndoEnabled(true);*/
	if (dia->importCanceled)
	{
		if (dia->importFailed)
			QMessageBox::warning(mw, CommonStrings::trWarning, tr("The file could not be imported"), 1, 0, 0);
		else if (dia->unsupported)
			QMessageBox::warning(mw, CommonStrings::trWarning, tr("WMF file contains some unsupported features"), 1, 0, 0);
	}

	bool success = !dia->importFailed;
	delete dia;
	return success;
}
