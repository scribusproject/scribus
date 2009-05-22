/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PLUGINMANAGERPREFS_H
#define PLUGINMANAGERPREFS_H

#include "prefspanel.h"
#include "ui_pluginmanagerprefsgui.h"
#include "scribus.h"


/*! \brief A GUI preferences interface for enabling/disabling plugins. */
class PluginManagerPrefsGui : public PrefsPanel, public Ui::PluginManagerPrefsGui
{
	Q_OBJECT

	public:
		PluginManagerPrefsGui(QWidget * parent, ScribusMainWindow* scMW);
		~PluginManagerPrefsGui();

	public slots:
		//! \brief Apply changes to each plugin's PluginSettings
		void apply();
};


//! \brief Non editable QTableWidgetItem
class PluginManagerTableItem : public QTableWidgetItem
{
	public:
		PluginManagerTableItem(const QString & text);
};

#endif
