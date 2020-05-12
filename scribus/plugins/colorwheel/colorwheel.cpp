/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "colorwheel.h"
#include "cwdialog.h"
#include "scribuscore.h"
#include "scribusdoc.h"

int colorwheel_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* colorwheel_getPlugin()
{
	auto* plug = new ColorWheelPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void colorwheel_freePlugin(ScPlugin* plugin)
{
	auto* plug = dynamic_cast<ColorWheelPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ColorWheelPlugin::ColorWheelPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

ColorWheelPlugin::~ColorWheelPlugin() = default;

void ColorWheelPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ColorWheel";
	// Action text for menu, including accel
	m_actionInfo.text = tr("&Color Wheel...");
	// Menu
	m_actionInfo.menu = "Extras";
	m_actionInfo.menuAfterName = "extrasManageImages";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
}

QString ColorWheelPlugin::fullTrName() const
{
	return QObject::tr("Color Wheel");
}

const ScActionPlugin::AboutData* ColorWheelPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Petr Van\xc4\x9bk <petr@scribus.info>");
	about->shortDescription = tr("Color setting helper");
	about->description = tr("Color selector with color theory included.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void ColorWheelPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

/** Create dialog. Everything else is handled in separated classes. */
bool ColorWheelPlugin::run(ScribusDoc* doc, const QString& target)
{
	Q_ASSERT(target.isNull());
	ScribusDoc* currDoc = doc;
	if (currDoc == nullptr)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc == nullptr)
		return false;

	auto *dlg = new CWDialog(currDoc->scMW(), currDoc, "dlg", true);
	if (dlg)
	{
		dlg->exec();
		delete dlg;
		return true;
	}
	return false;
}
