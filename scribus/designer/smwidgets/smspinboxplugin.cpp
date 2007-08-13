/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smspinboxplugin.h"
#include "smspinbox.h"


/* SMSpinBoxPlugin */
SMSpinBoxPlugin::SMSpinBoxPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMSpinBoxPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMSpinBoxPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMSpinBoxPlugin::createWidget(QWidget* parent)
{
	return new SMSpinBox(parent, 0);
}

QString SMSpinBoxPlugin::name() const
{
	return QLatin1String("SMSpinBox");
}

QString SMSpinBoxPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMSpinBoxPlugin::icon() const
{
	return QIcon();
}

QString SMSpinBoxPlugin::toolTip() const
{
	return QString();
}

QString SMSpinBoxPlugin::whatsThis() const
{
	return QString();
}

bool SMSpinBoxPlugin::isContainer() const
{
	return false;
}

QString SMSpinBoxPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMSpinBox\" name=\"SMSpinBox1\">\n"
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

QString SMSpinBoxPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMSpinBoxPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smspinboxplugin, SMSpinBoxPlugin)
