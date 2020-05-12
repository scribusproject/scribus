/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UNICONVPLUGIN_H
#define UNICONVPLUGIN_H

#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "../../formatidlist.h"
#include <QObject>

class ScrAction;
class ScribusMainWindow;

/**
* \brief A class providing the plugin interface implementation for the uniconvertor plugin
*/
class PLUGIN_API UniconvImportPlugin : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		UniconvImportPlugin();
		virtual ~UniconvImportPlugin();
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		bool fileSupported(QIODevice* file, const QString & fileName=QString()) const override;
		bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0) override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};

	public slots:
		/*!
		\brief Run UniConvertor and import results
		\param filename a file name to import
		\retval true for success
		*/
		virtual bool import(const QString& fileName = QString(), int flags = lfUseCurrentPage|lfInteractive);

	private:
		void registerFormats();

};

extern "C" PLUGIN_API int uniconvertorplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* uniconvertorplugin_getPlugin();
extern "C" PLUGIN_API void uniconvertorplugin_freePlugin(ScPlugin* plugin);

#endif
