/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"

#include "importfh.h"
#include "importfhplugin.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scpage.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"

#include "ui/customfdialog.h"
#include "ui/scmwmenumanager.h"

int importfh_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importfh_getPlugin()
{
	ImportFhPlugin* plug = new ImportFhPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importfh_freePlugin(ScPlugin* plugin)
{
	ImportFhPlugin* plug = dynamic_cast<ImportFhPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportFhPlugin::ImportFhPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportFhPlugin::languageChange()
{
	importAction->setText( tr("Import Freehand..."));
	FileFormat* fmt = getFormatByExt("fh*");
	fmt->trName = tr("Freehand");
	fmt->filter = tr("Freehand (*.fh* *.FH*)");
}

ImportFhPlugin::~ImportFhPlugin()
{
	unregisterAll();
}

const QString ImportFhPlugin::fullTrName() const
{
	return QObject::tr("Freehand Importer");
}


const ScActionPlugin::AboutData* ImportFhPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Freehand Files");
	about->description = tr("Imports most Freehand files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportFhPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportFhPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Freehand");
	fmt.filter = tr("Freehand (*.fh* *.FH*)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "fh*" << "fh" << "fh3" << "fh34" << "fh5" << "fh6" << "fh7" << "fh8" << "fh9" << "fh10" << "fh11";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = true;
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-coreldraw");
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportFhPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportFhPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportFhPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importfh");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.fh* *.FH*);;All Files (*)");
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
	FhPlug *dia = new FhPlug(m_Doc, flags);
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

QImage ImportFhPlugin::readThumbnail(const QString& fileName)
{
	if( fileName.isEmpty() )
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = NULL;
	FhPlug *dia = new FhPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
