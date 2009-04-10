/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QCheckBox>
#include <QLabel>

#include "menumanager.h"
#include "pluginmanagerprefsgui.h"
#include "pluginmanager.h"
#include "scraction.h"
#include "scplugin.h"
#include "commonstrings.h"


PluginManagerPrefsGui::PluginManagerPrefsGui(QWidget * parent, ScribusMainWindow* scMW)
	: PrefsPanel(parent)	
{
	setupUi(this);
	PluginManager& pluginManager(PluginManager::instance());
	//Hide this stuff as we don't want it now we are not letting people turn plugins on or off
	pluginTable->setColumnHidden(3, true);
	restartLabel->setHidden(true);

	// Get a list of all internal plugin names, including those of disabled
	// plugins, then loop over them and add each one to the plugin list.
	QStringList pluginNames(pluginManager.pluginNames(true));
	pluginTable->setRowCount(pluginNames.count());
	ScPlugin* plugin;
	ScActionPlugin* ixplug;
	QString pName;
	for (int i = 0; i < pluginNames.count(); ++i)
	{
		pName = pluginNames.at(i);
		plugin = pluginManager.getPlugin(pName, true);
		Q_ASSERT(plugin); // all the returned names should represent loaded plugins

		PluginManagerTableItem *i0 = new PluginManagerTableItem(plugin->fullTrName());
		pluginTable->setItem(i, 0, i0);

		PluginManagerTableItem *i1 = new PluginManagerTableItem("");
		// Resident plugins don't have predefined actions
		if (plugin->inherits("ScActionPlugin"))
		{
			ixplug = dynamic_cast<ScActionPlugin*>(plugin);
			Q_ASSERT(ixplug);
			ScActionPlugin::ActionInfo ai(ixplug->actionInfo());
			// menu path
			QString men = "";
			if (!ai.parentMenu.isEmpty())
			{
				if (scMW->scrMenuMgr->menuExists(ai.parentMenu))
					men = scMW->scrMenuMgr->getLocalPopupMenu(ai.parentMenu)->title().remove(QRegExp("&(?!&)")) + " -> ";
			}
			if (scMW->scrMenuMgr->menuExists(ai.menu))
				men += scMW->scrMenuMgr->getLocalPopupMenu(ai.menu)->title().remove(QRegExp("&(?!&)")) + " -> ";
			i1->setText(men + QString("%1").arg(scMW->scrActions[ai.name]->text().remove(QRegExp("&(?!&)"))));
		}
		pluginTable->setItem(i, 1, i1);

		PluginManagerTableItem *i2 = new PluginManagerTableItem(plugin->pluginTypeName());
		pluginTable->setItem(i, 2, i2);
/* Don't need this column at all now we are not allowing users to turn plugins on or off
		// load at start?
		if (plugin->inherits("ScPersistentPlugin"))
		{
			bool onStart = pluginManager.enableOnStartup(pName);
			//		QCheckBox *onStartCheck = new QCheckBox(onStart ? CommonStrings::trYes : CommonStrings::trNo, this);
			QCheckBox *onStartCheck = new QCheckBox(this);
			pluginTable->setCellWidget(i, 3, onStartCheck);
			onStartCheck->setChecked(onStart);
		}
*/

		PluginManagerTableItem *i4 = new PluginManagerTableItem(pName);
		pluginTable->setItem(i, 4, i4); // plugname for developers
		PluginManagerTableItem *i5 = new PluginManagerTableItem(pluginManager.getPluginPath(pName));
		pluginTable->setItem(i, 5, i5); // file path for developers
	}
	pluginTable->resizeColumnsToContents();
}

PluginManagerPrefsGui::~PluginManagerPrefsGui()
{
}

void PluginManagerPrefsGui::apply()
{
	QString plugName;
//	PluginManager& pluginManager(PluginManager::instance());
//	bool enable;
	for (int i = 0; i < pluginTable->rowCount(); ++i)
	{
		plugName = pluginTable->item(i, 4)->text();
		/* Don't need this  at all now we are not allowing users to turn plugins on or off
		QCheckBox* onStartCheck=qobject_cast<QCheckBox*>(pluginTable->cellWidget(i, 3));
		if (onStartCheck)
		{
			enable = onStartCheck->isChecked();
			pluginManager.enableOnStartup(plugName) = enable;
		}
		*/
	}
}


PluginManagerTableItem::PluginManagerTableItem(const QString & text)
	: QTableWidgetItem(text)
{
	setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}
