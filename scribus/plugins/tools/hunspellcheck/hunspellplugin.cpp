/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "hunspellplugin.h"
#include "hunspellpluginimpl.h"
#include "scribuscore.h"
#include "ui/storyeditor.h"

// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in mypluginimpl.h and mypluginimpl.cpp .

HunspellPlugin::HunspellPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

HunspellPlugin::~HunspellPlugin() = default;

void HunspellPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "HunspellPlugin";
	// Action text for menu, including &accel
	m_actionInfo.text = tr("Check Spelling...");
	// Menu
	m_actionInfo.menu = "Extras";
	m_actionInfo.menuAfterName = "extrasDeHyphenateText";
	// Story Editor Menu
	m_actionInfo.seMenu = "Edit";
	// If needed, what item to add the menu item after
	//m_actionInfo.menuAfterName = "ColorWheel"
	// If needed, the keyboard shortcut for the plugin
	m_actionInfo.keySequence = "SHIFT+F7";
	// Should the menu item be enabled when the app starts
	// (even without a document open) ?
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.enabledForStoryEditor = true;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::Polygon);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.notSuitableFor.append(PageItem::PathText);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.notSuitableFor.append(PageItem::Symbol);
	m_actionInfo.notSuitableFor.append(PageItem::RegularPolygon);
	m_actionInfo.notSuitableFor.append(PageItem::Arc);
	m_actionInfo.notSuitableFor.append(PageItem::Spiral);
	m_actionInfo.needsNumObjects = 1;
}

QString HunspellPlugin::fullTrName() const
{
	return QObject::tr("Hunspell Plugin");
}

const ScActionPlugin::AboutData* HunspellPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	return about;
}

void HunspellPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool HunspellPlugin::run(ScribusDoc* doc, const QString& target)
{
	HunspellPluginImpl *hunspellPluginImpl = new HunspellPluginImpl();
	Q_CHECK_PTR(hunspellPluginImpl);
	bool result = hunspellPluginImpl->run(target, doc);
	delete hunspellPluginImpl;
	return result;
}

bool HunspellPlugin::run(QWidget *parent, ScribusDoc *doc, const QString& target)
{
	HunspellPluginImpl *hunspellPluginImpl = new HunspellPluginImpl();
	Q_CHECK_PTR(hunspellPluginImpl);
	bool result = false;
	if (parent)
	{
		StoryEditor* se = dynamic_cast<StoryEditor*>(parent);
		if (se)
			hunspellPluginImpl->setRunningForSE(true, se);
	}
	result = hunspellPluginImpl->run(target, doc);
	delete hunspellPluginImpl;
	return result;
}

// Low level plugin API
int hunspellplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* hunspellplugin_getPlugin()
{
	HunspellPlugin* plug = new HunspellPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void hunspellplugin_freePlugin(ScPlugin* plugin)
{
	HunspellPlugin* plug = dynamic_cast<HunspellPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}
