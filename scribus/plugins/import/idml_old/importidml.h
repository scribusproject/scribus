/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTIDML_H
#define IMPORTIDML_H

#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "scfonts.h"
#include "scribusstructs.h"
#include "styles/styleset.h"

#include <QString>
#include <QMap>
#include <QList>
#include <QProgressBar>


class PLUGIN_API ImportIdml : public LoadSavePlugin
{
	Q_OBJECT

	public:
		//! \brief Standard plugin implementation
		ImportIdml();
		virtual ~ImportIdml();
		//! \brief main method to run the plug
		virtual bool run(ScribusDoc* doc, QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool fileSupported(QIODevice* file, const QString & fileName=QString::null) const;
		virtual bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0);
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};
//		virtual bool import(QString fileName = QString::null, int flags = lfUseCurrentPage|lfInteractive);
		void registerFormats();
};

extern "C" PLUGIN_API int importidml_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* importidml_getPlugin();
extern "C" PLUGIN_API void importidml_freePlugin(ScPlugin* plugin);

#endif
