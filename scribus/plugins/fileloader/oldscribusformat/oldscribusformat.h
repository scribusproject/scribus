/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef OLDSCRIBUSFORMAT_H
#define OLDSCRIBUSFORMAT_H

#include "pluginapi.h"
#include "loadsaveplugin.h"

class PLUGIN_API OldScribusFormat : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		OldScribusFormat();
		virtual ~OldScribusFormat();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool fileSupported(QIODevice* file) const;

		virtual bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0);
		virtual bool saveFile(const QString & fileName, const FileFormat & fmt);
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features - .sla page extraction support
		bool loadPage(int pageNumber, bool Mpage);

	private:
		void registerFormats();
};

extern "C" PLUGIN_API int oldscribusformat_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* oldscribusformat_getPlugin();
extern "C" PLUGIN_API void oldscribusformat_freePlugin(ScPlugin* plugin);

#endif
