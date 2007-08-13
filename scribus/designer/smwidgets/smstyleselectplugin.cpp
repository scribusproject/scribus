/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smstyleselectplugin.h"
#include "smstyleselect.h"


/* SMStyleSelectPlugin */
SMStyleSelectPlugin::SMStyleSelectPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMStyleSelectPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMStyleSelectPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMStyleSelectPlugin::createWidget(QWidget* parent)
{
	return new SMStyleSelect(parent);
}

QString SMStyleSelectPlugin::name() const
{
	return QLatin1String("SMStyleSelect");
}

QString SMStyleSelectPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMStyleSelectPlugin::icon() const
{
	return QIcon();
}

QString SMStyleSelectPlugin::toolTip() const
{
	return QString();
}

QString SMStyleSelectPlugin::whatsThis() const
{
	return QString();
}

bool SMStyleSelectPlugin::isContainer() const
{
	return false;
}

QString SMStyleSelectPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMStyleSelect\" name=\"SMStyleSelect1\">\n"
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

QString SMStyleSelectPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMStyleSelectPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smstyleselectplugin, SMStyleSelectPlugin)
