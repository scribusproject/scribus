#ifndef PLUGINMANAGERPREFS_H
#define PLUGINMANAGERPREFS_H

#include "qwidget.h"
#include "qmap.h"
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <Q3CString>
#include "prefspanel.h"

class QVBoxLayout;
class Q3GroupBox;
class QGridLayout;
class Q3ListView;
class QLabel;
class Q3ListViewItem;

class PluginManagerPrefsGui : public PrefsPanel
{
	Q_OBJECT

	public:
		PluginManagerPrefsGui(QWidget * parent);
		~PluginManagerPrefsGui();

	public slots:
		void updateSettings(Q3ListViewItem *item, const QPoint &, int column);

		/// Apply changes to each plugin's PluginSettings
		void apply();

	protected:
		QVBoxLayout* pluginMainLayout;
		Q3GroupBox* plugGroupBox;
		QGridLayout* plugGroupBoxLayout;
		QVBoxLayout* plugLayout1;
		Q3ListView* pluginsList;
		QLabel* pluginWarning;

		/* \brief Stores current settings for each plugin
		 * \param enableOnStartup - Load the plug-in on startup?
		 */
		struct PluginSettings
		{
			bool enableOnStartup;
		};

		/// Store current settings about each plug-in
		QMap<Q3CString,PluginSettings> pluginSettings;
};

#endif
