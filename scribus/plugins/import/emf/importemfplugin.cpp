/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Jan 11 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "commonstrings.h"

#include "importemf.h"
#include "importemfplugin.h"
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

int importemf_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importemf_getPlugin()
{
	ImportEmfPlugin* plug = new ImportEmfPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importemf_freePlugin(ScPlugin* plugin)
{
	ImportEmfPlugin* plug = dynamic_cast<ImportEmfPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportEmfPlugin::ImportEmfPlugin() : LoadSavePlugin(), importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportEmfPlugin::languageChange()
{
	importAction->setText( tr("Import EMF..."));
	FileFormat* fmt = getFormatByExt("emf");
	fmt->trName = tr("EMF");
	fmt->filter = tr("EMF (*.emf *.EMF)");
}

ImportEmfPlugin::~ImportEmfPlugin()
{
	unregisterAll();
}

const QString ImportEmfPlugin::fullTrName() const
{
	return QObject::tr("EMF Importer");
}


const ScActionPlugin::AboutData* ImportEmfPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports EMF Files");
	about->description = tr("Imports most EMF files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportEmfPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportEmfPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("EMF");
	fmt.filter = tr("EMF (*.emf *.EMF)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "emf";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.mimeTypes = QStringList(); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportEmfPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportEmfPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportEmfPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importemf");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.emf *.EMF);;All Files (*)");
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
		}
		else
			return true;
	}
	if (m_Doc == 0)
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
	EmfPlug *dia = new EmfPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	dia->import(fileName, trSettings, flags, !(flags & lfScripted));
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

QImage ImportEmfPlugin::readThumbnail(const QString& fileName)
{
	if( fileName.isEmpty() )
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = NULL;
	EmfPlug *dia = new EmfPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
