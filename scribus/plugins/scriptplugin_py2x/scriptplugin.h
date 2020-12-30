/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIPTPLUG_H
#define SCRIPTPLUG_H

#include "cmdvar.h"
#include "scplugin.h"
#include "pluginapi.h"

#include <QByteArray>
#include <QPixmap>

class QString;
class QPixmap;
class QWidget;

class PLUGIN_API ScriptPlugin : public ScPersistentPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ScriptPlugin();
		~ScriptPlugin() override;
		bool initPlugin() override;
		bool cleanupPlugin() override;
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		bool newPrefsPanelWidget(QWidget* parent, Prefs_Pane*& panel) override;
		void addToMainWindowMenu(ScribusMainWindow *) override;

		// Special features (none)
		QByteArray pythonHome;
};

extern "C" PLUGIN_API int scriptplugin_py2x_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scriptplugin_py2x_getPlugin();
extern "C" PLUGIN_API void scriptplugin_py2x_freePlugin(ScPlugin* plugin);

/** Some useful Subroutines */
/*static */PyObject *scribus_py2x_retval(PyObject *self, PyObject* args);
/*static */PyObject *scribus_py2x_getval(PyObject *self);

#endif // CMSPLUG_H
