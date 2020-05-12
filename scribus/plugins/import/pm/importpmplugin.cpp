/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"

#include "importpm.h"
#include "importpmplugin.h"
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

int importpm_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importpm_getPlugin()
{
	ImportPmPlugin* plug = new ImportPmPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importpm_freePlugin(ScPlugin* plugin)
{
	ImportPmPlugin* plug = dynamic_cast<ImportPmPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportPmPlugin::ImportPmPlugin() :
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportPmPlugin::languageChange()
{
	importAction->setText( tr("Import Pagemaker..."));
	FileFormat* fmt = getFormatByExt("pmd");
	fmt->trName = tr("Pagemaker");
	fmt->filter = tr("Pagemaker (*.pmd *.PMD *.pm *.PM *.pm3 *.PM3 *.pm4 *.PM4 *.pm5 *.PM5 *.pm6 *.PM6 *.p65 *.P65)");
}

ImportPmPlugin::~ImportPmPlugin()
{
	unregisterAll();
}

QString ImportPmPlugin::fullTrName() const
{
	return QObject::tr("Pagemaker Importer");
}


const ScActionPlugin::AboutData* ImportPmPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports Pagemaker Files");
	about->description = tr("Imports most Pagemaker files into the current document, converting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportPmPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportPmPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Pagemaker");
	fmt.filter = tr("Pagemaker (*.pmd *.PMD *.pm *.PM *.pm3 *.PM3 *.pm4 *.PM4 *.pm5 *.PM5 *.pm6 *.PM6 *.p65 *.P65)");
	fmt.formatId = 0;
	fmt.fileExtensions = QStringList() << "pmd" << "pm" << "pm3" << "pm4" << "pm5" << "pm6" << "p65";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.colorReading = true;
	fmt.mimeTypes = QStringList();
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportPmPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportPmPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportPmPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if (fileName.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("importpm");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.pmd *.PMD *.pm *.PM *.pm3 *.PM3 *.pm4 *.PM4 *.pm5 *.PM5 *.pm6 *.PM6 *.p65 *.P65);;All Files (*)");
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
		}
		else
			return true;
	}
	m_Doc=ScCore->primaryMainWindow()->doc;
	UndoTransaction* activeTransaction = nullptr;
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
		activeTransaction = new UndoTransaction(UndoManager::instance()->beginTransaction(trSettings));
	PmPlug *dia = new PmPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	dia->import(fileName, trSettings, flags, !(flags & lfScripted));
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = nullptr;
	}
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return true;
}

QImage ImportPmPlugin::readThumbnail(const QString& fileName)
{
	if (fileName.isEmpty())
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = nullptr;
	PmPlug *dia = new PmPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
