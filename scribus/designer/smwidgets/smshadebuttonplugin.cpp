/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smshadebuttonplugin.h"
#include "smshadebutton.h"


/* SMShadeButtonPlugin */
SMShadeButtonPlugin::SMShadeButtonPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMShadeButtonPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMShadeButtonPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMShadeButtonPlugin::createWidget(QWidget* parent)
{
	return new SMShadeButton(parent);
}

QString SMShadeButtonPlugin::name() const
{
	return QLatin1String("SMShadeButton");
}

QString SMShadeButtonPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMShadeButtonPlugin::icon() const
{
	return QIcon();
}

QString SMShadeButtonPlugin::toolTip() const
{
	return QString();
}

QString SMShadeButtonPlugin::whatsThis() const
{
	return QString();
}

bool SMShadeButtonPlugin::isContainer() const
{
	return false;
}

QString SMShadeButtonPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMShadeButton\" name=\"SMShadeButton1\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>22</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n");
}

QString SMShadeButtonPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMShadeButtonPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smshadebuttonplugin, SMShadeButtonPlugin)

