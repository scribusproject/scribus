#ifndef FONTPREVIEW_H
#define FONTPREVIEW_H

#include "pluginapi.h"
#include "scribus.h"
#include "scplugin.h"

class PLUGIN_API FontPreviewPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		FontPreviewPlugin();
		virtual ~FontPreviewPlugin();
		virtual bool run(QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();

		// Special features (none)
};

extern "C" PLUGIN_API int fontpreview_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* fontpreview_getPlugin();
extern "C" PLUGIN_API void fontpreview_freePlugin(ScPlugin* plugin);

#endif
