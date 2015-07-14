/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          shapeplugin.cpp  -  description
                             -------------------
    begin                : Fri Apr 3 08:00:00 CEST 2015
    copyright            : (C) 2015 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include <QWidget>
#include <QString>

#include "scribuscore.h"
#include "scribusapp.h"
#include "shapeplugin.h"
#include "scraction.h"
#include "ui/scmwmenumanager.h"


int shapeplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* shapeplugin_getPlugin()
{
	ShapePlugin* plug = new ShapePlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void shapeplugin_freePlugin(ScPlugin* plugin)
{
	ShapePlugin* plug = dynamic_cast<ShapePlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ShapePlugin::ShapePlugin() : ScPersistentPlugin()
{
	sc_palette = NULL;
}

ShapePlugin::~ShapePlugin() {};

void ShapePlugin::languageChange()
{
	if (sc_palette)
		sc_palette->languageChange();
}

void ShapePlugin::addToMainWindowMenu(ScribusMainWindow* mw)
{
	if (sc_palette)
	{
		sc_palette->setMainWindow(mw);
		languageChange();
		m_actions.insert("shapeShowPalette", new ScrAction(QObject::tr("Custom Shapes"), QKeySequence(), this));
		m_actions["shapeShowPalette"]->setToggleAction(true);
		m_actions["shapeShowPalette"]->setChecked(false);
		connect(m_actions["shapeShowPalette"], SIGNAL(toggled(bool)), sc_palette, SLOT(setPaletteShown(bool)));
		connect(sc_palette, SIGNAL(paletteShown(bool)), m_actions["shapeShowPalette"], SLOT(setChecked(bool)));
		mw->scrMenuMgr->addMenuItemStringAfter("shapeShowPalette", "toolsInline", "Windows");
		mw->scrMenuMgr->addMenuItemStringstoMenuBar("Windows", m_actions);
	}
}

const QString ShapePlugin::fullTrName() const
{
	return QObject::tr("Custom Shapes");
}

const ScActionPlugin::AboutData* ShapePlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <franz@scribus.info>, ");
	about->shortDescription = tr("Palette for Photoshop Custom Shapes.");
	return about;
}

void ShapePlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool ShapePlugin::initPlugin()
{
	sc_palette = new ShapePalette(ScCore->primaryMainWindow());
	sc_palette->startup();
	sc_palette->readFromPrefs();
	return true;
}

bool ShapePlugin::cleanupPlugin()
{
	if (sc_palette)
	{
		sc_palette->writeToPrefs();
		delete sc_palette;
		sc_palette = NULL;
	}
	return true;
}
