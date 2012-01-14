/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "importidml.h"
#include "importidmlimpl.h"

#include "../../formatidlist.h"
#include "commonstrings.h"
#include "ui/missing.h"
#include "hyphenator.h"
#include "pageitem_latexframe.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "scclocale.h"
#include "scconfig.h"
#include "sccolorengine.h"
#include "scpattern.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstream.h"
#include "scxmlstreamreader.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_formats.h"
#include "util_math.h"
#include "util_color.h"



ImportIdml::ImportIdml() : LoadSavePlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	registerFormats();
	languageChange();
}

ImportIdml::~ImportIdml() 
{
	unregisterAll();
};

void ImportIdml::languageChange()
{
	FileFormat* fmt = getFormatByExt("idml");
	fmt->trName = tr("Adobe Indesign IDML");
	fmt->filter = tr("Adobe Indesign IDML (*.idml *.IDML)");
}

const QString ImportIdml::fullTrName() const
{
	return QObject::tr("Adobe IDML Importer");
}

const ScActionPlugin::AboutData* ImportIdml::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Jain Basil Aliyas<jainbasil@gmail.com>";
	about->shortDescription = tr("Imports Adobe Idml Packages");
	about->description = tr("Imports Adobe Indesign's idml packages into Scribus\n converting Indesign objects into scribus pageitems; not all are supported in Scribus, but all those supported are being imported here");
	about->license = tr("GPL");
	Q_CHECK_PTR(about);
	return about;
}

void ImportIdml::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportIdml::registerFormats()
{
	FileFormat fmt(this);
	QString idmlName=tr("Adobe Indesign IDML");
	fmt.trName = idmlName;
	fmt.formatId = 0;
	fmt.filter =idmlName + "(*.idml *.IDML)"; // QFileDialog filter
	fmt.nameMatch = QRegExp("\\.idml$", Qt::CaseInsensitive);
	fmt.fileExtensions = QStringList() << "idml";
	fmt.load = true;
	fmt.save = false;
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/vnd.adobe.indesign-idml-package");
	fmt.priority = 64; // Priority
	registerFormat(fmt);
}

bool ImportIdml::run(ScribusDoc* doc, QString target)
{
	if(target.isEmpty())
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importidml");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.idml *.IDML);;All Files (*)");
		if(diaf.exec())
		{
			target = diaf.selectedFile();
			prefs->set("wdir", target.left(target.lastIndexOf("/")));
		}
		else
			return true;
	}
	m_Doc=ScCore->primaryMainWindow()->doc;
	ImportIdmlImpl *myPluginImpl = new ImportIdmlImpl();
	Q_CHECK_PTR(myPluginImpl);
	bool result = myPluginImpl->run(target, m_Doc);
	delete myPluginImpl;
	return result;
}

bool ImportIdml::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportIdml::loadFile(const QString & fileName, const FileFormat & /* fmt */, int  flags, int /* index */)
{
	return run(m_Doc,fileName);
}

// Low level plugin API
int importidml_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importidml_getPlugin()
{
	ImportIdml* plug = new ImportIdml();
	Q_CHECK_PTR(plug);
	return plug;
}

void importidml_freePlugin(ScPlugin* plugin)
{
	ImportIdml* plug = dynamic_cast<ImportIdml*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}
