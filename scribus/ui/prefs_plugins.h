/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_PLUGINS_H
#define PREFS_PLUGINS_H

#include "ui_prefs_pluginsbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Plugins : public Prefs_Pane, Ui::Prefs_Plugins
{
	Q_OBJECT

	public:
		Prefs_Plugins(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Plugins();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
		//! \brief Apply changes to each plugin's PluginSettings
		void apply();
};

//! \brief Non editable QTableWidgetItem
class PluginPrefsTableItem : public QTableWidgetItem
{
	public:
		PluginPrefsTableItem(const QString & text);
};

#endif // PREFS_PLUGINS_H
