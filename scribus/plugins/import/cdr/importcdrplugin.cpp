/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"

#include "importcdr.h"
#include "importcdrplugin.h"
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
#include <QMessageBox>

int importcdr_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importcdr_getPlugin()
{
	ImportCdrPlugin* plug = new ImportCdrPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importcdr_freePlugin(ScPlugin* plugin)
{
	ImportCdrPlugin* plug = dynamic_cast<ImportCdrPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportCdrPlugin::ImportCdrPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportCdrPlugin::languageChange()
{
	importAction->setText( tr("Import Corel Draw..."));
	FileFormat* fmt = getFormatByExt("cdr");
	fmt->trName = tr("Corel Draw");
	fmt->filter = tr("Corel Draw CDR (*.cdr *.CDR)");
}

ImportCdrPlugin::~ImportCdrPlugin()
{
	unregisterAll();
}

const QString ImportCdrPlugin::fullTrName() const
{
	return QObject::tr("Corel Draw Importer");
}


const ScActionPlugin::AboutData* ImportCdrPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Corel Draw Files");
	about->description = tr("Imports most Corel Draw files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportCdrPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportCdrPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Corel Draw");
	fmt.filter = tr("Corel Draw (*.cdr *.CDR)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "cdr";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = true;
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-coreldraw");
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportCdrPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportCdrPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportCdrPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importcdr");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.cdr *.CDR);;All Files (*)");
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
	CdrPlug *dia = new CdrPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	if (!dia->import(fileName, trSettings, flags))
	{
		ScribusMainWindow* mw=(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();
		//Import per Uniconverter
		qDebug() << "ERROR: Parsing with libcdr failed! Trying now Uniconverter.";
		const FileFormat *fmt = LoadSavePlugin::getFormatByExt("cdt");
		if (!fmt)
		{
			QMessageBox::warning(mw, CommonStrings::trWarning, tr("The Uniconverter Import plugin could not be found"), 1, 0, 0);
			return false;
		}
		flags &= ~LoadSavePlugin::lfCreateDoc;
		flags |= LoadSavePlugin::lfInsertPage;
		if (!fmt->loadFile(fileName, flags))
		{
			if (flags & LoadSavePlugin::lfCreateDoc)
			{
				QMessageBox::warning(mw, CommonStrings::trWarning, tr("Parsing failed!\n\nPlease submit your file (if possible) to the\nDocument Liberation Project http://www.documentliberation.org"), 1, 0, 0);
			}
			return false;
		}
	}
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

QImage ImportCdrPlugin::readThumbnail(const QString& fileName)
{
	if( fileName.isEmpty() )
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = NULL;
	CdrPlug *dia = new CdrPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
