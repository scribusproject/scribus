#include "pluginmanagerprefsgui.h"
#include "pluginmanager.h"
#include "scplugin.h"

#include "qlayout.h"
#include "qlistview.h"
#include "qgroupbox.h"
#include "qlabel.h"

PluginManagerPrefsGui::PluginManagerPrefsGui(QWidget * parent)
	: QWidget(parent, "pluginManagerWidget", 0)
{
	PluginManager& pluginManager(PluginManager::instance());
	pluginMainLayout = new QVBoxLayout( this, 0, 5, "pluginMainLayout");
	pluginMainLayout->setAlignment( Qt::AlignTop );
	plugGroupBox = new QGroupBox( tr("Plugin Manager"), this, "plugGroupBox");
	plugGroupBox->setColumnLayout(0, Qt::Vertical);
	plugGroupBox->layout()->setSpacing(6);
	plugGroupBox->layout()->setMargin(11);
	plugGroupBoxLayout = new QGridLayout( plugGroupBox->layout() );
	plugGroupBoxLayout->setAlignment(Qt::AlignTop);
	plugLayout1 = new QVBoxLayout( 0, 0, 6, "plugLayout1");
	pluginsList = new QListView(plugGroupBox, "pluginsList");
	pluginsList->setAllColumnsShowFocus(true);
	pluginsList->setShowSortIndicator(true);
	pluginsList->addColumn( tr("Plugin"));
	pluginsList->setColumnWidthMode(0, QListView::Maximum);
	pluginsList->addColumn( tr("How to run"));
	pluginsList->setColumnWidthMode(1, QListView::Maximum);
	pluginsList->addColumn( tr("Type"));
	pluginsList->setColumnWidthMode(2, QListView::Maximum);
	pluginsList->addColumn( tr("Load it?"));
	pluginsList->setColumnWidthMode(3, QListView::Maximum);
	pluginsList->addColumn( tr("Plugin ID"));
	pluginsList->setColumnWidthMode(4, QListView::Maximum);
	pluginsList->addColumn( tr("File"));
	pluginsList->setColumnWidthMode(5, QListView::Maximum);
	// Get a list of all internal plugin names, then loop over them and add
	// each one to the plugin list.
	QValueList<QCString> pluginNames(pluginManager.pluginNames());
	for (QValueList<QCString>::Iterator it = pluginNames.begin(); it != pluginNames.end(); ++it)
	{
		QListViewItem *plugItem = new QListViewItem(pluginsList);
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
		plugItem->setPixmap(3, onstart ? loadIcon("ok.png") : loadIcon("DateiClos16.png"));
		plugItem->setText(3, onstart ? tr("Yes") : tr("No"));
		plugItem->setText(4, QString("%1").arg(*it)); // plugname for developers
		plugItem->setText(5, pluginManager.getPluginPath(*it)); // file path for developers
	}
	plugLayout1->addWidget(pluginsList);
	pluginWarning = new QLabel(plugGroupBox);
	pluginWarning->setText("<qt>" + tr("You need to restart the application to apply the changes.") + "</qt>");
	plugLayout1->addWidget(pluginWarning);
	plugGroupBoxLayout->addLayout(plugLayout1, 0, 0);
	pluginMainLayout->addWidget(plugGroupBox);
	connect(pluginsList, SIGNAL(clicked(QListViewItem *, const QPoint &, int)),
			this, SLOT(changePluginLoad(QListViewItem *, const QPoint &, int)));
}

PluginManagerPrefsGui::~PluginManagerPrefsGui()
{
}

/*! Set selected item(=plugin) un/loadable
\author Petr Vanek
*/
void PluginManagerPrefsGui::changePluginLoad(QListViewItem *item, const QPoint &, int column)
{
	PluginManager& pluginManager(PluginManager::instance());
	if (column != 3)
		return;
	if (item->text(3) == tr("Yes"))
	{
		item->setPixmap(3, loadIcon("DateiClos16.png"));
		item->setText(3, tr("No"));
		pluginManager.enableOnStartup(item->text(4).latin1()) = false;
	}
	else
	{
		item->setPixmap(3, loadIcon("ok.png"));
		item->setText(3, tr("Yes"));
		pluginManager.enableOnStartup(item->text(4).latin1()) = true;
	}
}

#include "pluginmanagerprefsgui.moc"
