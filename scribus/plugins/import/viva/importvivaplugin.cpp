/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Sep 29 2013
	copyright            : (C) 2013 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "commonstrings.h"

#include "importviva.h"
#include "importvivaplugin.h"
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

int importviva_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importviva_getPlugin()
{
	ImportVivaPlugin* plug = new ImportVivaPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importviva_freePlugin(ScPlugin* plugin)
{
	ImportVivaPlugin* plug = dynamic_cast<ImportVivaPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportVivaPlugin::ImportVivaPlugin() :
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportVivaPlugin::languageChange()
{
	importAction->setText( tr("Import Viva..."));
	FileFormat* fmt = getFormatByExt("xml");
	fmt->trName = tr("Viva Designer XML");
	fmt->filter = tr("Viva Designer XML (*.xml *.XML)");
}

ImportVivaPlugin::~ImportVivaPlugin()
{
	unregisterAll();
}

QString ImportVivaPlugin::fullTrName() const
{
	return QObject::tr("Viva Importer");
}


const ScActionPlugin::AboutData* ImportVivaPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Viva Files");
	about->description = tr("Imports most Viva files into the current document, converting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportVivaPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportVivaPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Viva Designer XML");
	fmt.filter = tr("Viva Designer XML (*.xml *.XML)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "xml";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = true;
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("");
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportVivaPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportVivaPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportVivaPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if (fileName.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("importviva");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.xml *.XML);;All Files (*)");
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
	bool emptyDoc = (m_Doc == nullptr);
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
	VivaPlug *dia = new VivaPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	dia->import(fileName, trSettings, flags, !(flags & lfScripted));
	if (activeTransaction)
		activeTransaction.commit();
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return true;
}

QImage ImportVivaPlugin::readThumbnail(const QString& fileName)
{
	if (fileName.isEmpty())
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = nullptr;
	VivaPlug *dia = new VivaPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}

bool ImportVivaPlugin::readColors(const QString& fileName, ColorList &colors)
{
	if (fileName.isEmpty())
		return false;
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = nullptr;
	VivaPlug *dia = new VivaPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	bool ret = dia->readColors(fileName, colors);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
