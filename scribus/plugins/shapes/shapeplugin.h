/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SHAPEPLUG_H
#define SHAPEPLUG_H

#include <QMap>
#include <QPointer>

#include "scconfig.h"
#include "scplugin.h"
#include "pluginapi.h"
#include "shapepalette.h"

class ScrAction;
class ScribusMainWindow;

class PLUGIN_API ShapePlugin : public ScPersistentPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ShapePlugin();
		virtual ~ShapePlugin();
		virtual bool initPlugin();
		virtual bool cleanupPlugin();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *);
		ShapePalette* sc_palette;

	protected:
		QMap<QString, QPointer<ScrAction> > m_actions;
};

extern "C" PLUGIN_API int shapeplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* shapeplugin_getPlugin();
extern "C" PLUGIN_API void shapeplugin_freePlugin(ScPlugin* plugin);

#endif
