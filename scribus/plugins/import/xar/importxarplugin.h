/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTXARPLUGIN_H
#define IMPORTXARPLUGIN_H

#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "../../formatidlist.h"

class ScrAction;

class PLUGIN_API ImportXarPlugin : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ImportXarPlugin();
		virtual ~ImportXarPlugin();
		/*!
		\author Franz Schmid
		\date
		\brief Returns name of plugin
		\retval QString containing name of plugin: Import EPS/PDF/PS...
		*/
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		bool fileSupported(QIODevice* file, const QString& fileName = QString()) const override;
		bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0) override;
		QImage readThumbnail(const QString& fileName) override;
		bool readColors(const QString& fileName, ColorList & colors) override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};

	public slots:
		/*!
		\author Franz Schmid
		\date
		\brief Run the EPS import
		\param fileName input filename, or QString() to prompt.
		\retval bool always true
		 */
		virtual bool import(QString fileName = QString(), int flags = lfUseCurrentPage|lfInteractive);

	private:
		void registerFormats();
		ScrAction* importAction;
};

extern "C" PLUGIN_API int importxar_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* importxar_getPlugin();
extern "C" PLUGIN_API void importxar_freePlugin(ScPlugin* plugin);

#endif
