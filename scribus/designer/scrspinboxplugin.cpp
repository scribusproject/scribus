/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/


#include <QtPlugin>
#include "scrspinboxplugin.h"
#include "../ui/scrspinbox.h"


ScrSpinBoxPlugin::ScrSpinBoxPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void ScrSpinBoxPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool ScrSpinBoxPlugin::isInitialized() const
{
	return initialized;
}

QWidget* ScrSpinBoxPlugin::createWidget(QWidget* parent)
{
	return new ScrSpinBox(parent);
}

QString ScrSpinBoxPlugin::name() const
{
	return QLatin1String("ScrSpinBox");
}

QString ScrSpinBoxPlugin::group() const
{
	return QLatin1String("Scribus");
}

QIcon ScrSpinBoxPlugin::icon() const
{
	return QIcon();
}

QString ScrSpinBoxPlugin::toolTip() const
{
	return QString();
}

QString ScrSpinBoxPlugin::whatsThis() const
{
	return QString();
}

bool ScrSpinBoxPlugin::isContainer() const
{
	return false;
}

QString ScrSpinBoxPlugin::domXml() const
{
	return QLatin1String("<widget class=\"ScrSpinBox\" name=\"ScrSpinBox1\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>180</width>\n"
		"   <height>80</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n");
}

QString ScrSpinBoxPlugin::includeFile() const
{
	return QLatin1String("scrspinbox.h");
}

QString ScrSpinBoxPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_scrspinbox, ScrSpinBoxPlugin)

