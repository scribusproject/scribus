/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smscrspinboxplugin.h"
#include "smscrspinbox.h"


/* SMScrSpinBoxPlugin */
SMScrSpinBoxPlugin::SMScrSpinBoxPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMScrSpinBoxPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMScrSpinBoxPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMScrSpinBoxPlugin::createWidget(QWidget* parent)
{
	return new SMScrSpinBox(parent, 0);
}

QString SMScrSpinBoxPlugin::name() const
{
	return QLatin1String("SMScrSpinBox");
}

QString SMScrSpinBoxPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMScrSpinBoxPlugin::icon() const
{
	return QIcon();
}

QString SMScrSpinBoxPlugin::toolTip() const
{
	return QString();
}

QString SMScrSpinBoxPlugin::whatsThis() const
{
	return QString();
}

bool SMScrSpinBoxPlugin::isContainer() const
{
	return false;
}

QString SMScrSpinBoxPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMScrSpinBox\" name=\"SMScrSpinBox1\">\n"
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

QString SMScrSpinBoxPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMScrSpinBoxPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smscrspinboxplugin, SMScrSpinBoxPlugin)
