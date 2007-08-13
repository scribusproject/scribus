/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smalignselectplugin.h"
#include "smalignselect.h"


/* SMAlignSelectPlugin */
SMAlignSelectPlugin::SMAlignSelectPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMAlignSelectPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMAlignSelectPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMAlignSelectPlugin::createWidget(QWidget* parent)
{
	return new SMAlignSelect(parent);
}

QString SMAlignSelectPlugin::name() const
{
	return QLatin1String("SMAlignSelect");
}

QString SMAlignSelectPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMAlignSelectPlugin::icon() const
{
	return QIcon();
}

QString SMAlignSelectPlugin::toolTip() const
{
	return QString();
}

QString SMAlignSelectPlugin::whatsThis() const
{
	return QString();
}

bool SMAlignSelectPlugin::isContainer() const
{
	return false;
}

QString SMAlignSelectPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMAlignSelect\" name=\"SMAlignSelect1\">\n"
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

QString SMAlignSelectPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMAlignSelectPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smalignselectplugin, SMAlignSelectPlugin)
