/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTPSPLUGIN_H
#define IMPORTPSPLUGIN_H

#include "pluginapi.h"
#include "loadsaveplugin.h"

class ScrAction;

#define FORMATID_PSIMPORT 4

class PLUGIN_API ImportPSPlugin : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ImportPSPlugin();
		virtual ~ImportPSPlugin();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool fileSupported(QIODevice* file) const;
		virtual bool loadFile(const QString & fileName, const FileFormat & fmt);

	public slots:
		// Import menu item
		virtual bool import(QString target = QString::null);

	private:
		void registerFormats();
		ScrAction* importAction;
};

extern "C" PLUGIN_API int importps_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* importps_getPlugin();
extern "C" PLUGIN_API void importps_freePlugin(ScPlugin* plugin);

#endif
