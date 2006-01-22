/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <aboutplugins.h>
#include <pluginmanager.h>
#include <scplugin.h>

#include <qlistbox.h>
#include <qstring.h>
#include <qcstring.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qfileinfo.h>

// This class implements only the non-GUI parts of the
// About Plug-ins dialog. Please use Qt Designer on
// ui/aboutpluginsbase.ui if you need to modify the layout,
// widget properties, etc.

AboutPlugins::AboutPlugins( QWidget* parent )
	: AboutPluginsBase( parent, "AboutPlugins" ),
	// Get a list of plugins, enabled or not
	pluginNames(PluginManager::instance().pluginNames(true))
{
	// Populate the list with translated human-readable plugin names
	QValueList<QCString>::const_iterator it(pluginNames.begin());
	QValueList<QCString>::const_iterator itEnd(pluginNames.end());
	for ( ; it != itEnd; ++it )
	{
		ScPlugin* plugin = PluginManager::instance().getPlugin(*it, true);
		Q_ASSERT(plugin);
		pluginList->insertItem(plugin->fullTrName());
	}
	// Hook up a connection to update the plugin info when
	// the selection changes
	connect(pluginList, SIGNAL(highlighted(int)),
			SLOT(displayPlugin(int)));
	// and select the first plugin, if any
	if (pluginList->count())
		pluginList->setCurrentItem(0);
}

AboutPlugins::~AboutPlugins()
{
}

void AboutPlugins::displayPlugin(int sel)
{
	// Look up the list entry to get the plugin name and use
	// it to grab the plugin instance and get its about data.
	PluginManager& pluginManager = PluginManager::instance();
	const QCString& name(pluginNames[sel]);
	ScPlugin* plugin = pluginManager.getPlugin(name, true);
	Q_ASSERT(plugin);
	const ScPlugin::AboutData* about = plugin->getAboutData();
	Q_ASSERT(about);
	// Now set the text of the various about widgets.
	authors->setText(about->authors);
	copyright->setText(about->copyright);
	if (about->description.isNull())
		description->setText(about->shortDescription);
	else
		description->setText(about->description);
	license->setText(about->license);
	version->setText(about->version);
	enabled->setText(pluginManager.enabled(name) ? tr("Yes") : tr("No"));
	// Set the filename using the basename of the path
	QFileInfo fi(pluginManager.getPluginPath(name));
	fileName->setText(fi.baseName(true));
	// Use the plugin to delete the about info, so we don't
	// confuse win32's segmented memory.
	plugin->deleteAboutData(about);
}
