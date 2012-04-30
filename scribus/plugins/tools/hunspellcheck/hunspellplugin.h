/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef HUNSPELLPLUGIN_H
#define HUNSPELLPLUGIN_H

#include "pluginapi.h"
#include "scplugin.h"

/*! \brief See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods do.
That documentatation is not duplicated here.
Please don't implement the functionality of your plugin here; do that
in mypluginimpl.h and mypluginimpl.cpp. */
class PLUGIN_API HunspellPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		//! \brief Standard plugin implementation
		HunspellPlugin();
		virtual ~HunspellPlugin();
		//! \brief main method to run the plug
		virtual bool run(ScribusDoc* doc, QString target = QString::null);
		virtual bool run(QWidget* p, ScribusDoc* doc, QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features (none)

		QWidget *m_parent;
};

extern "C" PLUGIN_API int hunspellplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* hunspellplugin_getPlugin();
extern "C" PLUGIN_API void hunspellplugin_freePlugin(ScPlugin* plugin);

#endif
