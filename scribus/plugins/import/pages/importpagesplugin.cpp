/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Mon Dez 30 2013
	copyright            : (C) 2013 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "commonstrings.h"

#include "importpages.h"
#include "importpagesplugin.h"
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

int importpages_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importpages_getPlugin()
{
	ImportPagesPlugin* plug = new ImportPagesPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importpages_freePlugin(ScPlugin* plugin)
{
	ImportPagesPlugin* plug = dynamic_cast<ImportPagesPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportPagesPlugin::ImportPagesPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportPagesPlugin::languageChange()
{
	importAction->setText( tr("Import Mac Pages..."));
	FileFormat* fmt = getFormatByExt("pages");
	fmt->trName = tr("Mac Pages");
	fmt->filter = tr("Mac Pages (*.pages *.PAGES)");
}

ImportPagesPlugin::~ImportPagesPlugin()
{
	unregisterAll();
}

const QString ImportPagesPlugin::fullTrName() const
{
	return QObject::tr("Pages Importer");
}


const ScActionPlugin::AboutData* ImportPagesPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Mac Pages Files");
	about->description = tr("Imports most Mac Pages files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportPagesPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportPagesPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Mac Pages");
	fmt.filter = tr("Mac Pages (*.pages *.PAGES)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "pages";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = false;
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("");
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportPagesPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportPagesPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportPagesPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importpages");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.pages *.PAGES);;All Files (*)");
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
	PagesPlug *dia = new PagesPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	bool ret = dia->import(fileName, trSettings, flags);
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}

QImage ImportPagesPlugin::readThumbnail(const QString& fileName)
{
	if( fileName.isEmpty() )
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = NULL;
	PagesPlug *dia = new PagesPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
