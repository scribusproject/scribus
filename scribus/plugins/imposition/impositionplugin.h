/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Jun 2007
    copyright            : (C) 2007 by Mateusz Haligowski
    email                : halish@kofeina.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef IMPOSITION_H
#define IMPOSITION_H

#include "pluginapi.h"
#include "scplugin.h"

class ScribusDoc;

class PLUGIN_API ImpositionPlugin : public ScActionPlugin
{
  Q_OBJECT

  public:
    ImpositionPlugin();
    virtual ~ImpositionPlugin();
    
    virtual bool run(QWidget* parent, ScribusDoc* doc, QString target = QString::null);
    virtual bool run(ScribusDoc* doc, QString target = QString::null);
    virtual void languageChange();
    virtual const QString fullTrName() const;
    virtual const AboutData* getAboutData() const;
    virtual void deleteAboutData(const AboutData* about) const;
    virtual void addToMainWindowMenu(ScribusMainWindow *) {};
};

extern "C" PLUGIN_API int imposition_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* imposition_getPlugin();
extern "C" PLUGIN_API void imposition_freePlugin(ScPlugin* plugin);



#endif
   
