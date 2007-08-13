/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smcolorcomboplugin.h"
#include "smcolorcombo.h"


/* SMColorComboPlugin */
SMColorComboPlugin::SMColorComboPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMColorComboPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMColorComboPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMColorComboPlugin::createWidget(QWidget* parent)
{
	return new SMColorCombo(parent, "SMColorCombo");
}

QString SMColorComboPlugin::name() const
{
	return QLatin1String("SMColorCombo");
}

QString SMColorComboPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMColorComboPlugin::icon() const
{
	return QIcon();
}

QString SMColorComboPlugin::toolTip() const
{
	return QString();
}

QString SMColorComboPlugin::whatsThis() const
{
	return QString();
}

bool SMColorComboPlugin::isContainer() const
{
	return false;
}

QString SMColorComboPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMColorCombo\" name=\"SMColorCombo1\">\n"
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

QString SMColorComboPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMColorComboPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smcolorcomboplugin, SMColorComboPlugin)
