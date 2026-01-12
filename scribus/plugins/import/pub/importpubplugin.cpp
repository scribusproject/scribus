/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <memory>

#include "importpub.h"
#include "importpubplugin.h"

#include "commonstrings.h"
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

int importpub_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importpub_getPlugin()
{
	auto* plug = new ImportPubPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importpub_freePlugin(ScPlugin* plugin)
{
	ImportPubPlugin* plug = qobject_cast<ImportPubPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportPubPlugin::ImportPubPlugin() :
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportPubPlugin::languageChange()
{
	importAction->setText( tr("Import PUB..."));
	FileFormat* fmt = getFormatByExt("pub");
	fmt->trName = tr("MS Publisher");
	fmt->filter = tr("MS Publisher (*.pub *.PUB)");
}

ImportPubPlugin::~ImportPubPlugin()
{
	unregisterAll();
}

QString ImportPubPlugin::fullTrName() const
{
	return QObject::tr("PUB Importer");
}

const ScActionPlugin::AboutData* ImportPubPlugin::getAboutData() const
{
	auto* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports PUB Files");
	about->description = tr("Imports most MS Publisher files into the current document, converting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportPubPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportPubPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("MS Publisher");
	fmt.filter = tr("MS Publisher (*.pub *.PUB)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "pub";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = true;
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-mspublisher");
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportPubPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportPubPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call importFile(...)
	return importFile(fileName, flags);
}

bool ImportPubPlugin::importFile(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if (fileName.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("importpub");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.pub *.PUB);;All Files (*)");
		if (diaf.exec() != QDialog::Accepted)
			return true;
		fileName = diaf.selectedFile();
		prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
	}

	m_Doc = ScCore->primaryMainWindow()->doc;

	UndoTransaction activeTransaction;
	bool emptyDoc = (m_Doc == nullptr);
	bool hasCurrentPage = (m_Doc && m_Doc->currentPage());

	TransactionSettings trSettings;
	trSettings.targetName   = hasCurrentPage ? m_Doc->currentPage()->getUName() : "";
	trSettings.targetPixmap = Um::IImageFrame;
	trSettings.actionName   = Um::ImportPublisher;
	trSettings.description  = fileName;
	trSettings.actionPixmap = Um::IXFIG;
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(false);
	if (UndoManager::undoEnabled())
		activeTransaction = UndoManager::instance()->beginTransaction(trSettings);

	auto pPlug = std::make_shared<PubPlug>(m_Doc, flags);
	Q_CHECK_PTR(pPlug);
	pPlug->importFile(fileName, trSettings, flags, !(flags & lfScripted));

	if (activeTransaction)
		activeTransaction.commit();
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);

	return true;
}

QImage ImportPubPlugin::readThumbnail(const QString& fileName)
{
	if (fileName.isEmpty())
		return QImage();

	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = nullptr;
	auto pPlug = std::make_shared<PubPlug>(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(pPlug);
	QImage ret = pPlug->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);

	return ret;
}
