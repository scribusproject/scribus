/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Feb  9 2014
	copyright            : (C) 2014 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "commonstrings.h"

#include "importodg.h"
#include "importodgplugin.h"
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

int importodg_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importodg_getPlugin()
{
	ImportOdgPlugin* plug = new ImportOdgPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importodg_freePlugin(ScPlugin* plugin)
{
	ImportOdgPlugin* plug = dynamic_cast<ImportOdgPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportOdgPlugin::ImportOdgPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportOdgPlugin::languageChange()
{
	importAction->setText( tr("Import ODF Document..."));
	FileFormat* fmt = getFormatByExt("odg");
	fmt->trName = tr("ODF Drawing", "Import/export format name");
	fmt->filter = tr("ODF Drawing (*.odg *.ODG *.fodg *.FODG)");
	FileFormat* fmt2 = getFormatByExt("odp");
	fmt2->trName = tr("ODF Presentation", "Import/export format name");
	fmt2->filter = tr("ODF Presentation (*.odp *.ODP *.fodp *.FODP)");
}

ImportOdgPlugin::~ImportOdgPlugin()
{
	unregisterAll();
}

const QString ImportOdgPlugin::fullTrName() const
{
	return QObject::tr("Open Document Importer");
}


const ScActionPlugin::AboutData* ImportOdgPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports ODF Drawing Files");
	about->description = tr("Imports most ODF Drawing files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportOdgPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportOdgPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("ODF Drawing", "Import/export format name");
	fmt.filter = tr("ODF Drawing (*.odg *.ODG *.fodg *.FODG)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "odg" << "fodg";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = false;
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/vnd.oasis.opendocument.graphics");
	fmt.priority = 64; // Priority
	registerFormat(fmt);
	FileFormat fmt2(this);
	fmt2.trName = tr("ODF Presentation", "Import/export format name");
	fmt2.filter = tr("ODF Presentation (*.odp *.ODP *.fodp *.FODP)");
	fmt2.formatId = 0;
	fmt2.fileExtensions = QStringList() << "odp" << "fodp";
	fmt2.load = true;
	fmt2.save = false;
	fmt2.thumb = true;
	fmt2.colorReading = false;
	fmt2.mimeTypes = QStringList();
	fmt2.mimeTypes.append("application/vnd.oasis.opendocument.presentation");
	fmt2.priority = 64; // Priority
	registerFormat(fmt2);
}

bool ImportOdgPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportOdgPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportOdgPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importodg");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.odg *.ODG *.fodg *.FODG *.odp *.ODP *.fodp *.FODP);;All Files (*)");
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
		}
		else
			return true;
	}
	m_Doc=ScCore->primaryMainWindow()->doc;
	UndoTransaction activeTransaction;
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
		activeTransaction = UndoManager::instance()->beginTransaction(trSettings);
	OdgPlug *dia = new OdgPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	bool ret = dia->import(fileName, trSettings, flags, !(flags & lfScripted));
	if (activeTransaction)
		activeTransaction.commit();
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}

QImage ImportOdgPlugin::readThumbnail(const QString& fileName)
{
	if( fileName.isEmpty() )
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = NULL;
	OdgPlug *dia = new OdgPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
