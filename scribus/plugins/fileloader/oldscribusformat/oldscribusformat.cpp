/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "oldscribusformat.h"
//#include "oldscribusformat.moc"
#include "oldscribusformatimpl.h"

#include "scconfig.h"

// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in oldscribusformatimpl.h and oldscribusformatimpl.cpp .

OldScribusFormat::OldScribusFormat() :
	LoadSavePlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file formats.
	languageChange();
}

OldScribusFormat::~OldScribusFormat()
{
	unregisterAll();
};

void OldScribusFormat::languageChange()
{
	//(Re)register file formats.
	unregisterAll();
	registerFormats();
}

const QString OldScribusFormat::fullTrName() const
{
	return QObject::tr("Old .sla format support");
}

const ScActionPlugin::AboutData* OldScribusFormat::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	return about;
}

void OldScribusFormat::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void OldScribusFormat::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = tr("Scribus Document");
	fmt.formatId = 0;
	fmt.load = true;
	fmt.save = true;
	fmt.filter = fmt.trName + " (*.sla *.SLA *.sla.gz *.SLA.GZ *.scd *.SCD *.scd.gz *.SCD.GZ)";
	fmt.nameMatch = QRegExp("\\.(sla|scd)(\\.gz)?", false);
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-scribus");
	fmt.priority = 64;
	registerFormat(fmt);
	FileFormat fmt2(this);
	fmt2.trName = tr("Scribus 1.2.x Document");
	fmt2.formatId = 0;
	fmt2.load = true;
	fmt2.save = false;
	fmt2.filter = fmt.filter;
	fmt2.nameMatch = fmt.nameMatch;
	fmt2.mimeTypes.append("application/x-scribus");
	fmt2.priority = 63;
	registerFormat(fmt2);
}

bool OldScribusFormat::fileSupported(QIODevice* /* file */) const
{
	return true;
}

bool OldScribusFormat::loadFile(const QString & /* fileName */, const FileFormat & /* fmt */, int /* flags */, int /* index */)
{
	return false;
}

bool OldScribusFormat::saveFile(const QString & /* fileName */, const FileFormat & /* fmt */)
{
	return false;
}

// Low level plugin API
int oldscribusformat_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* oldscribusformat_getPlugin()
{
	OldScribusFormat* plug = new OldScribusFormat();
	Q_CHECK_PTR(plug);
	return plug;
}

void oldscribusformat_freePlugin(ScPlugin* plugin)
{
	OldScribusFormat* plug = dynamic_cast<OldScribusFormat*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}
