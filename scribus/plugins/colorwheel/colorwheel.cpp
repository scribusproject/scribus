/* $Id$ */
#include "colorwheel.h"
#include "cwdialog.h"
#include "pluginmanager.h"
#include <qcursor.h>
#include <qlistview.h>
#include "scribus.h"

int colorwheel_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* colorwheel_getPlugin()
{
	ColorWheelPlugin* plug = new ColorWheelPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void colorwheel_freePlugin(ScPlugin* plugin)
{
	ColorWheelPlugin* plug = dynamic_cast<ColorWheelPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ColorWheelPlugin::ColorWheelPlugin() :
	ScActionPlugin(ScPlugin::PluginType_Action)
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

ColorWheelPlugin::~ColorWheelPlugin() {};

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
	m_actionInfo.enabledOnStartup = true;
}

const QString ColorWheelPlugin::fullTrName() const
{
	return QObject::tr("Color Wheel");
}

const ScActionPlugin::AboutData* ColorWheelPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	return about;
}

void ColorWheelPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

/** Create dialog. Everything else is handled in separated classes. */
bool ColorWheelPlugin::run(QString target)
{
	Q_ASSERT(target.isNull());
	ColorWheelDialog *dlg = new ColorWheelDialog(ScApp, "dlg", true, 0);
	if (dlg)
	{
		dlg->exec();
		delete dlg;
		return true;
	}
	else
		return false;
}
