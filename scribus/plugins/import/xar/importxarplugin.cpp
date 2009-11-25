/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "importxar.h"
#include "importxarplugin.h"
#include "menumanager.h"
#include "page.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"


int importxar_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importxar_getPlugin()
{
	ImportXarPlugin* plug = new ImportXarPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importxar_freePlugin(ScPlugin* plugin)
{
	ImportXarPlugin* plug = dynamic_cast<ImportXarPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportXarPlugin::ImportXarPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	languageChange();
}

void ImportXarPlugin::languageChange()
{
	importAction->setText( tr("Import Pict..."));
	// (Re)register file format support
	unregisterAll();
	registerFormats();
}

ImportXarPlugin::~ImportXarPlugin()
{
	unregisterAll();
};

const QString ImportXarPlugin::fullTrName() const
{
	return QObject::tr("Xara Importer");
}


const ScActionPlugin::AboutData* ImportXarPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Xara Files");
	about->description = tr("Imports most Xara files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportXarPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportXarPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::XAR); // Human readable name
	fmt.formatId = FORMATID_XARIMPORT;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::XAR); // QFileDialog filter
	fmt.nameMatch = QRegExp("\\."+FormatsManager::instance()->extensionListForFormat(FormatsManager::XAR, 1)+"$", Qt::CaseInsensitive);
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::XAR); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportXarPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportXarPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportXarPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importxar");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.xar *.XAR);;All Files (*)");
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
		}
		else
			return true;
	}
	m_Doc=ScCore->primaryMainWindow()->doc;
	UndoTransaction* activeTransaction = NULL;
	bool emptyDoc = (m_Doc == NULL);
	bool hasCurrentPage = (m_Doc && m_Doc->currentPage());
	TransactionSettings trSettings;
	trSettings.targetName   = hasCurrentPage ? m_Doc->currentPage()->getUName() : "";
	trSettings.targetPixmap = Um::IImageFrame;
	trSettings.actionName   = Um::ImportXfig;
	trSettings.description  = fileName;
	trSettings.actionPixmap = Um::IXFIG;
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(false);
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(UndoManager::instance()->beginTransaction(trSettings));
	XarPlug *dia = new XarPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	dia->import(fileName, trSettings, flags);
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return true;
}
