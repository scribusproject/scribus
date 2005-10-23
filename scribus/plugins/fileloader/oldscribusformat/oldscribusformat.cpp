#include "oldscribusformat.h"
#include "oldscribusformatimpl.h"

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

bool OldScribusFormat::run(QString target)
{
	OldScribusFormatImpl impl;
	return impl.run(target);
}

void OldScribusFormat::registerFormats()
{
	FormatSupport fmt;
	fmt.trName = tr("Scribus Document");
	fmt.internalName = "oldscribusformat";
	fmt.load = true;
	fmt.save = true;
#ifdef HAVE_LIBZ
	fmt.filter = fmt.trName + " (*.sla *.sla.gz *.scd *.scd.gz)";
	fmt.nameMatch = QRegExp("\.(sla|scd)(\.gz)?", false);
#else
	fmt.filter = fmt.trName + " (*.sla *.scd)";
	fmt.nameMatch = QRegExp("\.(sla|scd)", false);
#endif
	fmt.mimeTypes = QStringList();
	fmt.mimeTypes.append("application/x-scribus");
	fmt.priority = 64;
	registerFormat(fmt);
}

bool OldScribusFormat::fileSupported(QIODevice* /* file */) const
{
	return true;
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
