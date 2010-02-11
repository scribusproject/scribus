/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          importdrwplugin.cpp  -  description
                             -------------------
    begin                : Mon Jan 11 2010
    copyright            : (C) 2010 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "importdrw.h"
#include "importdrwplugin.h"
#include "ui/scmwmenumanager.h"
#include "page.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"


int importdrw_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importdrw_getPlugin()
{
	ImportDrwPlugin* plug = new ImportDrwPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importdrw_freePlugin(ScPlugin* plugin)
{
	ImportDrwPlugin* plug = dynamic_cast<ImportDrwPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportDrwPlugin::ImportDrwPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportDrwPlugin::languageChange()
{
	importAction->setText( tr("Import DRW..."));
	FileFormat* fmt = getFormatByExt("drw");
	fmt->trName = tr("Micrografx DRW File");
	fmt->filter = tr("Micrografx DRW File (*.drw *.DRW)");
	fmt->fileExtensions = QStringList() << "drw";
}

ImportDrwPlugin::~ImportDrwPlugin()
{
	unregisterAll();
};

const QString ImportDrwPlugin::fullTrName() const
{
	return QObject::tr("DRW Importer");
}


const ScActionPlugin::AboutData* ImportDrwPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports DRW Files");
	about->description = tr("Imports most DRW files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportDrwPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportDrwPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Micrografx DRW File"); // Human readable name
	fmt.formatId = 0;
	fmt.filter = tr("Micrografx DRW File (*.drw *.DRW)"); // QFileDialog filter
	fmt.nameMatch = QRegExp("\\.drw$", Qt::CaseInsensitive);
	fmt.fileExtensions = QStringList() << "drw";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.mimeTypes = QStringList(); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportDrwPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportDrwPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportDrwPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importdrw");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.drw *.DRW);;All Files (*)");
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
	DrwPlug *dia = new DrwPlug(m_Doc, flags);
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

QImage ImportDrwPlugin::readThumbnail(const QString& fileName)
{
	bool wasUndo = false;
	if( fileName.isEmpty() )
		return QImage();
	if (UndoManager::undoEnabled())
	{
		UndoManager::instance()->setUndoEnabled(false);
		wasUndo = true;
	}
	m_Doc = NULL;
	DrwPlug *dia = new DrwPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	if (wasUndo)
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
