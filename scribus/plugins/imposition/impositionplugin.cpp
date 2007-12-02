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
#include "impositionplugin.h"
//#include "impositionplugin.moc"
#include "imposition.h"
#include "scribuscore.h"
#include "scribusdoc.h"

ImpositionPlugin::ImpositionPlugin() : ScActionPlugin()
{
	languageChange();
}

ImpositionPlugin::~ImpositionPlugin() {};

void ImpositionPlugin::languageChange()
{
	m_actionInfo.name = "Imposition";
	m_actionInfo.text = tr("&Imposition...");
	m_actionInfo.menu = "Extras";
	m_actionInfo.enabledOnStartup = true;
	m_actionInfo.needsNumObjects = -1;
}

bool ImpositionPlugin::run(QWidget* parent, ScribusDoc* doc, QString target)
{
	if (doc==NULL)
		return false;
	Imposition *imp = new Imposition(parent,doc);
	imp->exec();
	
	return true;
}

bool ImpositionPlugin::run(ScribusDoc* doc, QString target)
{
	ScribusMainWindow* scmw=(doc==0)?ScCore->primaryMainWindow():doc->scMW();
	return run(scmw, doc, target);
}

const QString ImpositionPlugin::fullTrName() const
{
	return QObject::tr("Imposition");
}

const ScActionPlugin::AboutData* ImpositionPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Mateusz Haligowski <halish@kofeina.org>");
	about->shortDescription = tr("Imposition dialog");
	about->description = tr("Imposition on grids, booklets and folds");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void ImpositionPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

int imposition_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* imposition_getPlugin()
{
	ImpositionPlugin* plug = new ImpositionPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void imposition_freePlugin(ScPlugin* plugin)
{
	ImpositionPlugin* plug = dynamic_cast<ImpositionPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}
