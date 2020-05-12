/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "pluginapi.h"
#include "scplugin.h"

/*! \brief See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods do.
That documentatation is not duplicated here.
Please don't implement the functionality of your plugin here; do that
in mypluginimpl.h and mypluginimpl.cpp. */
class PLUGIN_API MyPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		//! \brief Standard plugin implementation
		MyPlugin();
		virtual ~MyPlugin();
		//! \brief main method to run the plug
		virtual bool run(ScribusDoc* doc, QString target = QString());
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};

		// Special features (none)
};

extern "C" PLUGIN_API int myplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* myplugin_getPlugin();
extern "C" PLUGIN_API void myplugin_freePlugin(ScPlugin* plugin);

#endif
