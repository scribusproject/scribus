/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "importpsplugin.h"
#include "importpsplugin.moc"
#include "importps.h"
#include "scribuscore.h"
#include "page.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "undomanager.h"
#include "customfdialog.h"
#include "scraction.h"
#include "menumanager.h"

int importps_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importps_getPlugin()
{
	ImportPSPlugin* plug = new ImportPSPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importps_freePlugin(ScPlugin* plugin)
{
	ImportPSPlugin* plug = dynamic_cast<ImportPSPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportPSPlugin::ImportPSPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QIconSet(), "", QKeySequence(), this, "ImportPS"))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	languageChange();
}

void ImportPSPlugin::addToMainWindowMenu(ScribusMainWindow *mw)
{
	importAction->setEnabled(true);
	connect( importAction, SIGNAL(activated()), SLOT(import()) );
	mw->scrMenuMgr->addMenuItem(importAction, "FileImport");
}

void ImportPSPlugin::languageChange()
{
	importAction->setMenuText( tr("Import &EPS/PS..."));
	// (Re)register file format support
	unregisterAll();
	registerFormats();
}

ImportPSPlugin::~ImportPSPlugin()
{
	unregisterAll();
};

const QString ImportPSPlugin::fullTrName() const
{
	return QObject::tr("PS/EPS Importer");
}


const ScActionPlugin::AboutData* ImportPSPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports EPS Files");
	about->description = tr("Imports most EPS files into the current document,\nconverting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportPSPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportPSPlugin::registerFormats()
{
	QString psName = tr("PostScript");
	FileFormat fmt(this);
	fmt.trName = psName; // Human readable name
	fmt.formatId = FORMATID_PSIMPORT;
	fmt.filter = psName + " (*.ps *.PS *.eps *.EPS *.epsi *.EPSI)"; // QFileDialog filter
	fmt.nameMatch = QRegExp("\\.(ps|eps|epsi)$", false);
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = QStringList("application/postscript"); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);

	QString pdfName = tr("PDF");
	FileFormat fmt2(this);
	fmt2.trName = pdfName; // Human readable name
	fmt2.formatId = FORMATID_PDFIMPORT;
	fmt2.filter = pdfName + " (*.pdf *.PDF)"; // QFileDialog filter
	fmt2.nameMatch = QRegExp("\\.pdf$", false);
	fmt2.load = true;
	fmt2.save = false;
	fmt2.mimeTypes = QStringList("application/pdf"); // MIME types
	fmt2.priority = 64; // Priority
	registerFormat(fmt2);
}

bool ImportPSPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	// TODO: check for %!PS-Adobe
	return true;
}

bool ImportPSPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportPSPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if (!(flags & lfInteractive))
		UndoManager::instance()->setUndoEnabled(false);
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importps");
		QString wdir = prefs->get("wdir", ".");
		QString formats = QObject::tr("All Supported Formats (*.eps *.EPS *.epsi *.EPSI *.ps *.PS);;");
		formats += "EPS (*.eps *.EPS);;EPSI (*.epsi *.EPSI);;PS (*.ps *.PS);;" + QObject::tr("All Files (*)");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), formats);
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.findRev("/")));
		}
		else
			return true;
	}
	m_Doc=ScCore->primaryMainWindow()->doc;
	if (UndoManager::undoEnabled() && m_Doc)
	{
		UndoManager::instance()->beginTransaction(m_Doc->currentPage()->getUName(),
												Um::IImageFrame,
												Um::ImportEPS,
												fileName, Um::IEPS);
	}
	else if (UndoManager::undoEnabled() && !m_Doc)
		UndoManager::instance()->setUndoEnabled(false);
	EPSPlug *dia = new EPSPlug(m_Doc, fileName, flags);
	Q_CHECK_PTR(dia);
	if (UndoManager::undoEnabled())
		UndoManager::instance()->commit();
	else
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return true;
}
