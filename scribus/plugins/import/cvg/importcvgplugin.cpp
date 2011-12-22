/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"
#include "customfdialog.h"
#include "importcvg.h"
#include "importcvgplugin.h"
#include "menumanager.h"
#include "page.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"


int importcvg_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importcvg_getPlugin()
{
	ImportCvgPlugin* plug = new ImportCvgPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importcvg_freePlugin(ScPlugin* plugin)
{
	ImportCvgPlugin* plug = dynamic_cast<ImportCvgPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportCvgPlugin::ImportCvgPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	languageChange();
}
/*
void ImportXfigPlugin::addToMainWindowMenu(ScribusMainWindow *mw)
{
	importAction->setEnabled(true);
	connect( importAction, SIGNAL(triggered()), SLOT(import()) );
	mw->scrMenuMgr->addMenuItem(importAction, "FileImport");
}
*/
void ImportCvgPlugin::languageChange()
{
	importAction->setText( tr("Import Cvg..."));
	// (Re)register file format support
	unregisterAll();
	registerFormats();
}

ImportCvgPlugin::~ImportCvgPlugin()
{
	unregisterAll();
};

const QString ImportCvgPlugin::fullTrName() const
{
	return QObject::tr("Cvg Importer");
}


const ScActionPlugin::AboutData* ImportCvgPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Cvg Files");
	about->description = tr("Imports most Cvg files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportCvgPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportCvgPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::CVG); // Human readable name
	fmt.formatId = FORMATID_CVGIMPORT;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::CVG); // QFileDialog filter
	fmt.nameMatch = QRegExp("\\."+FormatsManager::instance()->extensionListForFormat(FormatsManager::CVG, 1)+"$", Qt::CaseInsensitive);
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::CVG); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportCvgPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportCvgPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportCvgPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importcvg");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.cvg *.CVG);;All Files (*)");
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
	CvgPlug *dia = new CvgPlug(m_Doc, flags);
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
