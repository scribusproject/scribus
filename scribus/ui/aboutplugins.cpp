/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "aboutplugins.h"

#include "pluginmanager.h"
#include "scplugin.h"
#include "commonstrings.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QTextBrowser>
#include <QFileInfo>
#include <QList>


AboutPlugins::AboutPlugins( QWidget* parent )
	: QDialog(parent),
	// Get a list of plugins, enabled or not
	pluginNames(PluginManager::instance().pluginNames(true))
{
	setupUi(this);
	// Populate the list with translated human-readable plugin names
	QStringList::const_iterator it(pluginNames.begin());
	QStringList::const_iterator itEnd(pluginNames.end());
	for ( ; it != itEnd; ++it )
	{
		ScPlugin* plugin = PluginManager::instance().getPlugin(*it, true);
		Q_ASSERT(plugin);
		pluginList->addItem(plugin->fullTrName());
	}
	// Hook up a connection to update the plugin info when
	// the selection changes
	connect(pluginList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), SLOT(displayPlugin(QListWidgetItem*,QListWidgetItem*)));
	// and select the first plugin, if any
	if (pluginList->count())
	{
		pluginList->setCurrentRow(0);
		displayPlugin(pluginList->currentItem(), pluginList->currentItem());
	}
}

AboutPlugins::~AboutPlugins()
{
}

void AboutPlugins::displayPlugin(QListWidgetItem* currItem, QListWidgetItem* prevItem)
{
	Q_UNUSED(prevItem);

	int sel = pluginList->row(currItem);
	// Look up the list entry to get the plugin name and use
	// it to grab the plugin instance and get its about data.
	PluginManager& pluginManager = PluginManager::instance();
	const QString& name(pluginNames[sel]);
	ScPlugin* plugin = pluginManager.getPlugin(name, true);
	Q_ASSERT(plugin);
	const ScPlugin::AboutData* about = plugin->getAboutData();
	Q_ASSERT(about);
	// Now set the text into the html "template"
	// Set the filename using the basename of the path
	QFileInfo fi(pluginManager.getPluginPath(name));
	QString html;
	html = QString("<html><body><h1>%1</h1>").arg(plugin->fullTrName());
	html += "<table>";
	html += QString("<tr><th>%1</th><td>%2</td></tr>").arg( tr("Filename:"), fi.completeBaseName());
	html += QString("<tr><th>%1</th><td>%2</td></tr>").arg( tr("Version:"), about->version);
// As we don't allow users to disable plugins, hide this
//	QString ena;
//	ena = pluginManager.enabled(name) ? CommonStrings::trYes : CommonStrings::trNo;
//	html += QString("<tr><th>%1</th><td>%2</td></tr>").arg( tr("Enabled:")).arg(ena);
	html += QString("<tr><th>%1</th><td>%2</td></tr>").arg( tr("Release Date:"), about->releaseDate.toString());
	html += "</table>";
	QString desc;
	if (!about->shortDescription.isNull())
		desc += QString("<p>%1</p>").arg(about->shortDescription);
	if (!about->description.isNull())
		desc += QString("<p>%1</p>").arg(about->description);
	html += QString("<h2>%1</h2>%2").arg( tr("Description:"), desc);
	html += QString("<h2>%1</h2><p>%2</p>").arg( tr("Author(s):"), htmlize(about->authors));
	html += QString("<h2>%1</h2><p>%2</p>").arg( tr("Copyright:"), htmlize(about->copyright));
	html += QString("<h2>%1</h2><p>%2</p>").arg( tr("License:"), htmlize(about->license));
	html += "</html>";
	infoBrowser->setHtml(html);
	// Use the plugin to delete the about info, so we don't
	// confuse win32's segmented memory.
	plugin->deleteAboutData(about);
}

QString AboutPlugins::htmlize(QString s)
{
	QString ret = s.replace('<', "&lt;");
	ret = ret.replace('>', "&gt;");
	return ret;
}
