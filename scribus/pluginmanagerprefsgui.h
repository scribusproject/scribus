#ifndef PLUGINMANAGERPREFS_H
#define PLUGINMANAGERPREFS_H

#include "qwidget.h"
#include "pluginmanager.h"

class QVBoxLayout;
class QGroupBox;
class QGridLayout;
class QListView;
class QLabel;
class QListViewItem;

class PluginManagerPrefsGui : public QWidget
{
	Q_OBJECT

	public:
		PluginManagerPrefsGui(QWidget * parent);
		~PluginManagerPrefsGui();

	public slots:
		void changePluginLoad(QListViewItem *item, const QPoint &, int column);

	protected:
		QVBoxLayout* pluginMainLayout;
		QGroupBox* plugGroupBox;
		QGridLayout* plugGroupBoxLayout;
		QVBoxLayout* plugLayout1;
		QListView* pluginsList;
		QLabel* pluginWarning;

		// Friend of PluginManager that lets us poke in the protected
		// PluginManager::pluginMap directly.
		PluginManager::PluginMap & getPluginMap();
};

#endif
