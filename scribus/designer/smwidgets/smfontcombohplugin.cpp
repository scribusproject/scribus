/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "smfontcombohplugin.h"
#include "smfontcomboh.h"


/* SMFontComboHPlugin */
SMFontComboHPlugin::SMFontComboHPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void SMFontComboHPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool SMFontComboHPlugin::isInitialized() const
{
	return initialized;
}

QWidget* SMFontComboHPlugin::createWidget(QWidget* parent)
{
	return new SMFontComboH(parent);
}

QString SMFontComboHPlugin::name() const
{
	return QLatin1String("SMFontComboH");
}

QString SMFontComboHPlugin::group() const
{
	return QLatin1String("Scribus-SM");
}

QIcon SMFontComboHPlugin::icon() const
{
	return QIcon();
}

QString SMFontComboHPlugin::toolTip() const
{
	return QString();
}

QString SMFontComboHPlugin::whatsThis() const
{
	return QString();
}

bool SMFontComboHPlugin::isContainer() const
{
	return false;
}

QString SMFontComboHPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SMFontComboH\" name=\"SMFontComboH1\">\n"
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

QString SMFontComboHPlugin::includeFile() const
{
	return QLatin1String("smwidgets.h");
}

QString SMFontComboHPlugin::codeTemplate() const
{
	return QString();
}

Q_EXPORT_PLUGIN2(scribus_smfontcombohplugin, SMFontComboHPlugin)
