/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
/***************************************************************************
							 -------------------
	begin                : Sun Sep 29 2013
	copyright            : (C) 2013 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef IMPORTVIVAPLUGIN_H
#define IMPORTVIVAPLUGIN_H

#include "pluginapi.h"
#include "loadsaveplugin.h"

class ScrAction;

class PLUGIN_API ImportVivaPlugin : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ImportVivaPlugin();
		virtual ~ImportVivaPlugin();
		/*!
		\author Franz Schmid
		\date
		\brief Returns name of plugin
		\retval QString containing name of plugin: Import EPS/PDF/PS...
		*/
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual bool fileSupported(QIODevice* file, const QString & fileName=QString::null) const;
		virtual bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0);
		virtual QImage readThumbnail(const QString& fileName);
		virtual bool readColors(const QString& fileName, ColorList & colors);
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

	public slots:
		/*!
		\author Franz Schmid
		\date
		\brief Run the Viva import
		\param fileName input filename, or QString::null to prompt.
		\retval bool always true
		 */
		virtual bool import(QString fileName = QString::null, int flags = lfUseCurrentPage|lfInteractive);

	private:
		void registerFormats();
		ScrAction* importAction;
};

extern "C" PLUGIN_API int importviva_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* importviva_getPlugin();
extern "C" PLUGIN_API void importviva_freePlugin(ScPlugin* plugin);

#endif
