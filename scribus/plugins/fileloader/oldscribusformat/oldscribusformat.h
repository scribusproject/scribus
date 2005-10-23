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
		virtual bool run(QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool fileSupported(QIODevice* file) const;

		// Special features - .sla page extraction support
		bool loadPage(int pageNumber, bool Mpage);

	private:
		void registerFormats();
};

extern "C" PLUGIN_API int oldscribusformat_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* oldscribusformat_getPlugin();
extern "C" PLUGIN_API void oldscribusformat_freePlugin(ScPlugin* plugin);

#endif
