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
//Added by qt3to4:
#include <QPixmap>

class QString;
class QPixmap;
class QWidget;
class PrefsPanel;

class PLUGIN_API ScriptPlugin : public ScPersistentPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ScriptPlugin();
		virtual ~ScriptPlugin();
		virtual bool initPlugin();
		virtual bool cleanupPlugin();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool newPrefsPanelWidget(QWidget* parent, PrefsPanel*& panel,
										 QString& caption, QPixmap& icon);
		virtual void addToMainWindowMenu(ScribusMainWindow *);

		// Special features (none)
};

extern "C" PLUGIN_API int scriptplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scriptplugin_getPlugin();
extern "C" PLUGIN_API void scriptplugin_freePlugin(ScPlugin* plugin);

/** Some useful Subroutines */
/*static */PyObject *scribus_retval(PyObject *self, PyObject* args);
/*static */PyObject *scribus_getval(PyObject *self);

#endif // CMSPLUG_H
