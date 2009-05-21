/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PICTUREBROWSERPLUGIN_H
#define PICTUREBROWSERPLUGIN_H

#include "pluginapi.h"
#include "scplugin.h"
#include "picturebrowser.h"

/*! \brief See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods do.
That documentatation is not duplicated here.
Please don't implement the functionality of your plugin here; do that
in mypluginimpl.h and mypluginimpl.cpp. */
class PLUGIN_API PictureBrowserPlugin : public ScActionPlugin
{
		Q_OBJECT

	public:
		//! \brief Standard plugin implementation
		PictureBrowserPlugin();
		virtual ~PictureBrowserPlugin();
		//! \brief main method to run the plug
		virtual bool run ( ScribusDoc* doc, QString target = QString::null );
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData ( const AboutData* about ) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu ( ScribusMainWindow * ) {};
		virtual void setDoc ( ScribusDoc* doc );
		virtual void unsetDoc();
		virtual void changedDoc ( ScribusDoc* doc );

	private:
		PictureBrowser *pictureBrowser;

	private slots:
		void pictureBrowserClosed();
};

extern "C" PLUGIN_API int picturebrowser_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* picturebrowser_getPlugin();
extern "C" PLUGIN_API void picturebrowser_freePlugin ( ScPlugin* plugin );

#endif
