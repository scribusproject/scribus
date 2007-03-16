/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pluginmanagerprefsgui.h"
#include "pluginmanager.h"
#include "scplugin.h"
#include "util.h"
#include "commonstrings.h"

#include <qlayout.h>
#include <q3listview.h>
#include <q3groupbox.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3GridLayout>
#include <Q3CString>
#include <Q3VBoxLayout>
#include <Q3ValueList>

extern QPixmap loadIcon(QString nam);


PluginManagerPrefsGui::PluginManagerPrefsGui(QWidget * parent)
	: PrefsPanel(parent, "pluginManagerPrefsWidget")
{
	PluginManager& pluginManager(PluginManager::instance());
	pluginMainLayout = new Q3VBoxLayout( this, 0, 5, "pluginMainLayout");
	pluginMainLayout->setAlignment( Qt::AlignTop );
	plugGroupBox = new Q3GroupBox( tr("Plugin Manager"), this, "plugGroupBox");
	plugGroupBox->setColumnLayout(0, Qt::Vertical);
	plugGroupBox->layout()->setSpacing(6);
	plugGroupBox->layout()->setMargin(11);
	plugGroupBoxLayout = new Q3GridLayout( plugGroupBox->layout() );
	plugGroupBoxLayout->setAlignment(Qt::AlignTop);
	plugLayout1 = new Q3VBoxLayout( 0, 0, 6, "plugLayout1");
	pluginsList = new Q3ListView(plugGroupBox, "pluginsList");
	pluginsList->setAllColumnsShowFocus(true);
	pluginsList->setShowSortIndicator(true);
	pluginsList->addColumn( tr("Plugin"));
	pluginsList->setColumnWidthMode(0, Q3ListView::Maximum);
	pluginsList->addColumn( tr("How to run"));
	pluginsList->setColumnWidthMode(1, Q3ListView::Maximum);
	pluginsList->addColumn( tr("Type"));
	pluginsList->setColumnWidthMode(2, Q3ListView::Maximum);
	pluginsList->addColumn( tr("Load it?"));
	pluginsList->setColumnWidthMode(3, Q3ListView::Maximum);
	pluginsList->addColumn( tr("Plugin ID"));
	pluginsList->setColumnWidthMode(4, Q3ListView::Maximum);
	pluginsList->addColumn( tr("File"));
	pluginsList->setColumnWidthMode(5, Q3ListView::Maximum);

	checkOn = getQCheckBoxPixmap(true, pluginsList->paletteBackgroundColor());
	checkOff = getQCheckBoxPixmap(false, pluginsList->paletteBackgroundColor());
	// Get a list of all internal plugin names, including those of disabled
	// plugins, then loop over them and add each one to the plugin list.
	QStringList pluginNames(pluginManager.pluginNames(true));
	for (QStringList::Iterator it = pluginNames.begin(); it != pluginNames.end(); ++it)
	{
		Q3ListViewItem *plugItem = new Q3ListViewItem(pluginsList);
		// Get the plugin, even if it's loaded but disabled
		ScPlugin* plugin = pluginManager.getPlugin(*it, true);
		Q_ASSERT(plugin); // all the returned names should represent loaded plugins
		plugItem->setText(0, plugin->fullTrName());
		if (plugin->inherits("ScActionPlugin"))
		{
			ScActionPlugin* ixplug = dynamic_cast<ScActionPlugin*>(plugin);
			Q_ASSERT(ixplug);
			ScActionPlugin::ActionInfo ai(ixplug->actionInfo());
			plugItem->setText(1, QString("%1 %2").arg(ai.menu).arg(ai.menuAfterName)); // menu path
		}
		else
		{
			// Resident plug-ins don't have predefined actions
			plugItem->setText(1, QString(""));
		}
		plugItem->setText(2, plugin->pluginTypeName());
		// load at start?
		bool onstart = pluginManager.enableOnStartup(*it);
		plugItem->setPixmap(3, onstart ? checkOn : checkOff);
		plugItem->setText(3, onstart ? CommonStrings::trYes : CommonStrings::trNo);
		plugItem->setText(4, QString("%1").arg(*it)); // plugname for developers
		plugItem->setText(5, pluginManager.getPluginPath(*it)); // file path for developers
		// Populate the working settings info for the plug-in
		pluginSettings[*it].enableOnStartup = onstart;
	}
	plugLayout1->addWidget(pluginsList);
	pluginWarning = new QLabel(plugGroupBox);
	pluginWarning->setText("<qt>" + tr("You need to restart the application to apply the changes.") + "</qt>");
	plugLayout1->addWidget(pluginWarning);
	plugGroupBoxLayout->addLayout(plugLayout1, 0, 0);
	pluginMainLayout->addWidget(plugGroupBox);

	connect(pluginsList, SIGNAL(clicked(Q3ListViewItem *, const QPoint &, int)),
			this, SLOT(updateSettings(Q3ListViewItem *, const QPoint &, int)));
}

PluginManagerPrefsGui::~PluginManagerPrefsGui()
{
}

void PluginManagerPrefsGui::updateSettings(Q3ListViewItem *item, const QPoint &, int column)
{
	if (column != 3)
		// Only the plugin enable/disabled checkbox is editable
		return;
	PluginSettings& settings = pluginSettings[item->text(4).latin1()];
	bool onstartup;
	if (item->text(3) == CommonStrings::trYes)
	{
		item->setPixmap(3, checkOff);
		item->setText(3, CommonStrings::trNo);
		// Update our stored settings with the new flag
		onstartup = false;
	}
	else
	{
		item->setPixmap(3, checkOn);
		item->setText(3, CommonStrings::trYes);
		onstartup = true;
	}
	// Store changed setting(s) into working setting info
	settings.enableOnStartup = onstartup;
}

void PluginManagerPrefsGui::apply()
{
	PluginManager& pluginManager(PluginManager::instance());
	// For each plugin:
	QMap<Q3CString,PluginSettings>::Iterator itEnd(pluginSettings.end());
	for ( QMap<Q3CString,PluginSettings>::Iterator it(pluginSettings.begin()) ; it != itEnd ; ++it )
	{
		// Save any changes from our working info to the plugin manager
		pluginManager.enableOnStartup(it.key()) = it.data().enableOnStartup;
	}
}

//#include "pluginmanagerprefsgui.moc"
