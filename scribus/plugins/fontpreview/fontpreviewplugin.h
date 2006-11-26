/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTPREVIEW_H
#define FONTPREVIEW_H

#include "pluginapi.h"
#include "scplugin.h"

class ScribusDoc;

class PLUGIN_API FontPreviewPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		FontPreviewPlugin();
		virtual ~FontPreviewPlugin();
		virtual bool run(ScribusDoc* doc, QString target = QString::null);
		virtual bool run(QWidget *parent, ScribusDoc* doc, QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features (none)
};

extern "C" PLUGIN_API int fontpreview_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* fontpreview_getPlugin();
extern "C" PLUGIN_API void fontpreview_freePlugin(ScPlugin* plugin);

#endif
