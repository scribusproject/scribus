/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "importshape.h"
#include "importshapeplugin.h"
#include "ui/scmwmenumanager.h"
#include "page.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"


int importshape_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importshape_getPlugin()
{
	ImportShapePlugin* plug = new ImportShapePlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importshape_freePlugin(ScPlugin* plugin)
{
	ImportShapePlugin* plug = dynamic_cast<ImportShapePlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportShapePlugin::ImportShapePlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportShapePlugin::languageChange()
{
	importAction->setText( tr("Import Dia Shapes..."));
	FileFormat* fmt = getFormatByExt("sml");
	fmt->trName = tr("Dia Shapes \"*.shape\" File");
	fmt->filter = tr("Dia Shapes \"*.shape\" File (*.shape *.SHAPE)");
}

ImportShapePlugin::~ImportShapePlugin()
{
	unregisterAll();
};

const QString ImportShapePlugin::fullTrName() const
{
	return QObject::tr("Shape Importer");
}


const ScActionPlugin::AboutData* ImportShapePlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Dia shape Files");
	about->description = tr("Imports most Dia shapes into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportShapePlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportShapePlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Dia Shapes \"*.shape\" File"); // Human readable name
	fmt.formatId = 0;
	fmt.filter = tr("Dia shapes \"*.shape\" File (*.shape *.SHAPE)"); // QFileDialog filter
	fmt.nameMatch = QRegExp("\\.shape$", Qt::CaseInsensitive);
	fmt.fileExtensions = QStringList() << "shape";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = true;
	fmt.mimeTypes = QStringList(); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportShapePlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportShapePlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportShapePlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importshape");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.shape *.SHAPE);;All Files (*)");
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
	ShapePlug *dia = new ShapePlug(m_Doc, flags);
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

QImage ImportShapePlugin::readThumbnail(const QString& fileName)
{
	bool wasUndo = UndoManager::undoEnabled();
	if( fileName.isEmpty() )
		return QImage();
	if (UndoManager::undoEnabled())
		UndoManager::instance()->setUndoEnabled(false);
	m_Doc = NULL;
	ShapePlug *dia = new ShapePlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	if (wasUndo)
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
