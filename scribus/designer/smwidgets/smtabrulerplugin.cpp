/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smtabrulerplugin.h"
#include "smtabruler.h"


/* SMTabrulerPlugin */
SMTabrulerPlugin::SMTabrulerPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMTabrulerPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMTabrulerPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMTabrulerPlugin::createWidget(QWidget* parent)
{
	return new SMTabruler(parent); // use constructor defaults
}

QString SMTabrulerPlugin::name() const
{
	return QLatin1String("SMTabruler");
}

QString SMTabrulerPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMTabrulerPlugin::icon() const
{
	return QIcon();
}

QString SMTabrulerPlugin::toolTip() const
{
	return QString();
}

QString SMTabrulerPlugin::whatsThis() const
{
	return QString();
}

bool SMTabrulerPlugin::isContainer() const
{
	return false;
}

QString SMTabrulerPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMTabruler\" name=\"SMTabruler1\">\n"
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

QString SMTabrulerPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMTabrulerPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smtabrulerplugin, SMTabrulerPlugin)
