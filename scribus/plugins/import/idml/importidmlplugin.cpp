/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Jan 14 2012
	copyright            : (C) 2012 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "commonstrings.h"

#include "importidml.h"
#include "importidmlplugin.h"
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

int importidml_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importidml_getPlugin()
{
	ImportIdmlPlugin* plug = new ImportIdmlPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importidml_freePlugin(ScPlugin* plugin)
{
	ImportIdmlPlugin* plug = dynamic_cast<ImportIdmlPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportIdmlPlugin::ImportIdmlPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportIdmlPlugin::languageChange()
{
	importAction->setText( tr("Import Idml..."));
	FileFormat* fmt = getFormatByExt("idml");
	fmt->trName = tr("Adobe Indesign IDML");
	fmt->filter = tr("Adobe Indesign IDML (*.idml *.IDML)");
}

ImportIdmlPlugin::~ImportIdmlPlugin()
{
	unregisterAll();
};

const QString ImportIdmlPlugin::fullTrName() const
{
	return QObject::tr("Idml Importer");
}


const ScActionPlugin::AboutData* ImportIdmlPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports IDML Files");
	about->description = tr("Imports most IDML files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportIdmlPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportIdmlPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Adobe Indesign IDML");
	fmt.filter = tr("Adobe Indesign IDML (*.idml *.IDML)");
	fmt.formatId = 0;
	fmt.nameMatch = QRegExp("\\.idml$", Qt::CaseInsensitive);
	fmt.fileExtensions = QStringList() << "idml";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = true;
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/vnd.adobe.indesign-idml-package");
	fmt.priority = 64; // Priority
	registerFormat(fmt);

	FileFormat fmt2(this);
	fmt2.trName = tr("Adobe Indesign IDMS");
	fmt2.filter = tr("Adobe Indesign IDMS (*.idms *.IDMS)");
	fmt2.formatId = 0;
	fmt2.nameMatch = QRegExp("\\.idms$", Qt::CaseInsensitive);
	fmt2.fileExtensions = QStringList() << "idms";
	fmt2.load = true;
	fmt2.save = false;
	fmt2.thumb = true;
	fmt2.colorReading = true;
	fmt2.mimeTypes = QStringList();
	fmt2.mimeTypes.append("application/vnd.adobe.indesign-idms");
	fmt2.priority = 64; // Priority
	registerFormat(fmt2);
}

bool ImportIdmlPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportIdmlPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportIdmlPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importidml");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.idml *.IDML *.idms *.IDMS);;All Files (*)");
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
	IdmlPlug *dia = new IdmlPlug(m_Doc, flags);
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

QImage ImportIdmlPlugin::readThumbnail(const QString& fileName)
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
	IdmlPlug *dia = new IdmlPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	if (wasUndo)
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}

bool ImportIdmlPlugin::readColors(const QString& fileName, ColorList &colors)
{
	bool wasUndo = false;
	if( fileName.isEmpty() )
		return false;
	if (UndoManager::undoEnabled())
	{
		UndoManager::instance()->setUndoEnabled(false);
		wasUndo = true;
	}
	m_Doc = NULL;
	IdmlPlug *dia = new IdmlPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	bool ret = dia->readColors(fileName, colors);
	if (wasUndo)
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
