#include "importpsplugin.h"
#include "importps.h"
#include "scribus.h"
#include "page.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "undomanager.h"
#include "customfdialog.h"

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

ImportPSPlugin::ImportPSPlugin() : LoadSavePlugin()
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	languageChange();
}

void ImportPSPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ImportPS";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Import &EPS/PS...");
	// Menu
	m_actionInfo.menu = "FileImport";
	m_actionInfo.enabledOnStartup = true;
	// (Re)register file format support
	unregisterAll();
	registerFormats();
}

ImportPSPlugin::~ImportPSPlugin()
{
	unregisterAll();
};

/*!
 \fn QString Name()
 \author Franz Schmid
 \date
 \brief Returns name of plugin
 \param None
 \retval QString containing name of plugin: Import EPS/PDF/PS...
 */
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
	FormatSupport fmt;
	fmt.trName = psName; // Human readable name
	fmt.internalName = "psimport"; // Internal name
	fmt.filter = psName + " (*.ps *.PS *.eps *.EPS)"; // QFileDialog filter
	fmt.nameMatch = QRegExp("\\.(ps|eps)$", false);
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = QStringList("application/postscript"); // MIME types
	fmt.priority = 64; // Priority
	fmt.plug = this;
	registerFormat(fmt);
}

bool ImportPSPlugin::fileSupported(QIODevice* /* file */) const
{
	// TODO: check for %!PS-Adobe
	return true;
}


/*!
 \fn void Run(QWidget *d, ScribusApp *plug)
 \author Franz Schmid
 \date
 \brief Run the EPS import
 \param fileNAme input filename, or QString::null to prompt.
 \retval None
 */
bool ImportPSPlugin::run(QString fileName)
{
	bool interactive = fileName.isEmpty();
	if (!interactive)
		UndoManager::instance()->setUndoEnabled(false);
	else
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importps");
		QString wdir = prefs->get("wdir", ".");
		QString formats = QObject::tr("All Supported Formats (*.eps *.EPS *.ps *.PS);;");
		formats += "EPS (*.eps *.EPS);;PS (*.ps *.PS);;" + QObject::tr("All Files (*)");
		CustomFDialog diaf(ScApp, wdir, QObject::tr("Open"), formats);
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.findRev("/")));
		}
		else
			return true;
	}
	if (UndoManager::undoEnabled() && ScApp->HaveDoc)
	{
		UndoManager::instance()->beginTransaction(ScApp->doc->currentPage->getUName(),Um::IImageFrame,Um::ImportEPS, fileName, Um::IEPS);
	}
	else if (UndoManager::undoEnabled() && !ScApp->HaveDoc)
		UndoManager::instance()->setUndoEnabled(false);
	EPSPlug *dia = new EPSPlug(fileName, interactive);
	Q_CHECK_PTR(dia);
	if (UndoManager::undoEnabled())
		UndoManager::instance()->commit();
	else
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return true;
}

#include "importpsplugin.moc"
