/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PLUGINMANAGERPREFS_H
#define PLUGINMANAGERPREFS_H

#include "qwidget.h"
#include "qmap.h"
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3CString>
#include <QLabel>
#include "prefspanel.h"
#include "qpixmap.h"

class Q3VBoxLayout;
class Q3GroupBox;
class Q3GridLayout;
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
		/*! \brief Set selected item(=plugin) un/loadable
		\author Petr Vanek
		\param item atual item
		\param column id of the column clicked 0=1st
		*/
		void updateSettings(Q3ListViewItem *item, const QPoint &, int column);

		//! \brief Apply changes to each plugin's PluginSettings
		void apply();

	private:
		QPixmap checkOn;
		QPixmap checkOff;

	protected:
		Q3VBoxLayout* pluginMainLayout;
		Q3GroupBox* plugGroupBox;
		Q3GridLayout* plugGroupBoxLayout;
		Q3VBoxLayout* plugLayout1;
		Q3ListView* pluginsList;
		QLabel* pluginWarning;

		/*! \brief Stores current settings for each plugin
		 * \param enableOnStartup - Load the plug-in on startup?
		 */
		struct PluginSettings
		{
			bool enableOnStartup;
		};

		//! \brief Store current settings about each plug-in
		QMap<Q3CString,PluginSettings> pluginSettings;
};

#endif
