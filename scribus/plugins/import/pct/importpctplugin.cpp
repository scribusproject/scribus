/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"
#include "customfdialog.h"
#include "importpct.h"
#include "importpctplugin.h"
#include "menumanager.h"
#include "page.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"


int importpct_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importpct_getPlugin()
{
	ImportPctPlugin* plug = new ImportPctPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importpct_freePlugin(ScPlugin* plugin)
{
	ImportPctPlugin* plug = dynamic_cast<ImportPctPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportPctPlugin::ImportPctPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	languageChange();
}

void ImportPctPlugin::languageChange()
{
	importAction->setText( tr("Import Pict..."));
	// (Re)register file format support
	unregisterAll();
	registerFormats();
}

ImportPctPlugin::~ImportPctPlugin()
{
	unregisterAll();
};

const QString ImportPctPlugin::fullTrName() const
{
	return QObject::tr("Pict Importer");
}


const ScActionPlugin::AboutData* ImportPctPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Pict Files");
	about->description = tr("Imports most Mac Pict files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportPctPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportPctPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::PCT); // Human readable name
	fmt.formatId = FORMATID_PCTIMPORT;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::PCT); // QFileDialog filter
	fmt.nameMatch = QRegExp("\\."+FormatsManager::instance()->extensionListForFormat(FormatsManager::PCT, 1)+"$", Qt::CaseInsensitive);
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::PCT); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportPctPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportPctPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportPctPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importpct");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.pct *.PCT *.pict *.PICT);;All Files (*)");
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
	PctPlug *dia = new PctPlug(m_Doc, flags);
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
