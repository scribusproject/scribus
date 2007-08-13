/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smsccomboboxplugin.h"
#include "smsccombobox.h"


/* SMScComboBoxPlugin */
SMScComboBoxPlugin::SMScComboBoxPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMScComboBoxPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMScComboBoxPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMScComboBoxPlugin::createWidget(QWidget* parent)
{
	return new SMScComboBox(parent, "SMScComboBox");
}

QString SMScComboBoxPlugin::name() const
{
	return QLatin1String("SMScComboBox");
}

QString SMScComboBoxPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMScComboBoxPlugin::icon() const
{
	return QIcon();
}

QString SMScComboBoxPlugin::toolTip() const
{
	return QString();
}

QString SMScComboBoxPlugin::whatsThis() const
{
	return QString();
}

bool SMScComboBoxPlugin::isContainer() const
{
	return false;
}

QString SMScComboBoxPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMScComboBox\" name=\"SMScComboBox1\">\n"
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

QString SMScComboBoxPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMScComboBoxPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smsccomboboxplugin, SMScComboBoxPlugin)
