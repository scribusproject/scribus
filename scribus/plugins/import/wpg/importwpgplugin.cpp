/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"

#include "importwpg.h"
#include "importwpgplugin.h"
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

int importwpg_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importwpg_getPlugin()
{
	ImportWpgPlugin* plug = new ImportWpgPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importwpg_freePlugin(ScPlugin* plugin)
{
	ImportWpgPlugin* plug = dynamic_cast<ImportWpgPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportWpgPlugin::ImportWpgPlugin() :
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportWpgPlugin::languageChange()
{
	importAction->setText( tr("Import WordPerfect Graphics..."));
	FileFormat* fmt = getFormatByExt("wpg");
	fmt->trName = FormatsManager::instance()->nameOfFormat(FormatsManager::WPG);
	fmt->filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::WPG);
}

ImportWpgPlugin::~ImportWpgPlugin()
{
	unregisterAll();
}

QString ImportWpgPlugin::fullTrName() const
{
	return QObject::tr("WPG Importer");
}


const ScActionPlugin::AboutData* ImportWpgPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports WordPerfect Graphics Files");
	about->description = tr("Imports most WordPerfect Graphics files into the current document, converting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportWpgPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportWpgPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::WPG); // Human readable name
	fmt.formatId = 0;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::WPG); // QFileDialog filter
	fmt.fileExtensions = QStringList() << "wpg";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::WPG); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportWpgPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportWpgPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportWpgPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if (fileName.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("importwpg");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.wpg *.WPG);;All Files (*)");
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
	WpgPlug *dia = new WpgPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	dia->import(fileName, trSettings, flags, !(flags & lfScripted));
	if (activeTransaction)
		activeTransaction.commit();
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return true;
}

QImage ImportWpgPlugin::readThumbnail(const QString& fileName)
{
	if (fileName.isEmpty())
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = nullptr;
	WpgPlug *dia = new WpgPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
