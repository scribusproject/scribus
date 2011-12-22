/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"
#include "customfdialog.h"
#include "importps.h"
#include "importpsplugin.h"
#include "menumanager.h"
#include "page.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"

int importps_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importps_getPlugin()
{
	ImportPSPlugin* plug = new ImportPSPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importps_freePlugin(ScPlugin* plugin)
{
	ImportPSPlugin* plug = dynamic_cast<ImportPSPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportPSPlugin::ImportPSPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	languageChange();
}
/*
void ImportPSPlugin::addToMainWindowMenu(ScribusMainWindow *mw)
{
	importAction->setEnabled(true);
	connect( importAction, SIGNAL(triggered()), SLOT(import()) );
	mw->scrMenuMgr->addMenuItem(importAction, "FileImport");
}
*/
void ImportPSPlugin::languageChange()
{
	importAction->setText( tr("Import PostScript..."));
	// (Re)register file format support
	unregisterAll();
	registerFormats();
}

ImportPSPlugin::~ImportPSPlugin()
{
	unregisterAll();
};

const QString ImportPSPlugin::fullTrName() const
{
	return QObject::tr("PostScript Importer");
}


const ScActionPlugin::AboutData* ImportPSPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports PostScript Files");
	about->description = tr("Imports most PostScript files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportPSPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportPSPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::PS); // Human readable name
	fmt.formatId = FORMATID_PSIMPORT;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::EPS|FormatsManager::PS);// QFileDialog filter
 	fmt.nameMatch = QRegExp("\\.("+FormatsManager::instance()->extensionListForFormat(FormatsManager::EPS|FormatsManager::PS, 1)+")$", Qt::CaseInsensitive);
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::PS); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);

	FileFormat fmt2(this);
	fmt2.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::PDF); // Human readable name
	fmt2.formatId = FORMATID_PDFIMPORT;
	fmt2.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::PDF);// QFileDialog filter
	fmt2.nameMatch = QRegExp("\\."+FormatsManager::instance()->extensionListForFormat(FormatsManager::PDF, 1)+"$", Qt::CaseInsensitive);
	fmt2.load = true;
	fmt2.save = false;
	fmt2.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::PDF); // MIME types
	fmt2.priority = 64; // Priority
	registerFormat(fmt2);
}

bool ImportPSPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	// TODO: check for %!PS-Adobe
	return true;
}

bool ImportPSPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportPSPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importps");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), FormatsManager::instance()->fileDialogFormatList(FormatsManager::EPS|FormatsManager::PS));
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
	trSettings.actionName   = Um::ImportEPS;
	trSettings.description  = fileName;
	trSettings.actionPixmap = Um::IEPS;
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(false);
	if (UndoManager::undoEnabled())
	{
		activeTransaction = new UndoTransaction(UndoManager::instance()->beginTransaction(trSettings));
	}
	EPSPlug *dia = new EPSPlug(m_Doc, flags);
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
