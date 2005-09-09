#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include "pluginapi.h"
#include "scplugin.h"

class PLUGIN_API MyPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		MyPlugin();
		virtual ~MyPlugin();
		virtual bool run(QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();

		// Special features (none)
};

extern "C" PLUGIN_API int myplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* myplugin_getPlugin();
extern "C" PLUGIN_API void myplugin_freePlugin(ScPlugin* plugin);

#endif
