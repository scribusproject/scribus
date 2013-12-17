/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Oct 26 2013
	copyright            : (C) 2013 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "commonstrings.h"

#include "importxps.h"
#include "importxpsplugin.h"
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

int importxps_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importxps_getPlugin()
{
	ImportXpsPlugin* plug = new ImportXpsPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importxps_freePlugin(ScPlugin* plugin)
{
	ImportXpsPlugin* plug = dynamic_cast<ImportXpsPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportXpsPlugin::ImportXpsPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportXpsPlugin::languageChange()
{
	importAction->setText( tr("Import Xps..."));
	FileFormat* fmt = getFormatByExt("xps");
	fmt->trName = tr("Microsoft XPS");
	fmt->filter = tr("Microsoft XPS (*.xps *.XPS)");
	FileFormat* fmt2 = getFormatByExt("oxps");
	fmt2->trName = tr("Open XML Paper");
	fmt2->filter = tr("Open XML Paper (*.oxps *.OXPS)");
}

ImportXpsPlugin::~ImportXpsPlugin()
{
	unregisterAll();
}

const QString ImportXpsPlugin::fullTrName() const
{
	return QObject::tr("Xps Importer");
}


const ScActionPlugin::AboutData* ImportXpsPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports XPS and Open XML Paper Files");
	about->description = tr("Imports most XPS and Open XML Paper files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportXpsPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportXpsPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Microsoft XPS");
	fmt.filter = tr("Microsoft XPS (*.xps *.XPS)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "xps";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = false;
	fmt.mimeTypes = QStringList() << "application/vnd.ms-xpsdocument";
	fmt.mimeTypes.append("");
	fmt.priority = 64; // Priority
	registerFormat(fmt);
	FileFormat fmt2(this);
	fmt2.trName = tr("Open XML Paper");
	fmt2.filter = tr("Open XML Paper (*.oxps *.OXPS)");
	fmt2.formatId = 0;
	fmt2.fileExtensions = QStringList() << "oxps";
	fmt2.load = true;
	fmt2.save = false;
	fmt2.thumb = true;
	fmt2.colorReading = false;
	fmt2.mimeTypes = QStringList() << "application/oxps";
	fmt2.mimeTypes.append("");
	fmt2.priority = 64; // Priority
	registerFormat(fmt2);
}

bool ImportXpsPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportXpsPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportXpsPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importxps");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.oxps *.OXPS *.xps *.XPS);;All Files (*)");
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
	XpsPlug *dia = new XpsPlug(m_Doc, flags);
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

QImage ImportXpsPlugin::readThumbnail(const QString& fileName)
{
	if( fileName.isEmpty() )
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = NULL;
	XpsPlug *dia = new XpsPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
