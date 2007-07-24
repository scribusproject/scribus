/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtPlugin>
#include "sctextbrowserplugin.h"
#include "sctextbrowser.h"

ScTextBrowserPlugin::ScTextBrowserPlugin(QObject* parent)
	: QObject(parent), initialized(false)
{
}

void ScTextBrowserPlugin::initialize(QDesignerFormEditorInterface* core)
{
	Q_UNUSED(core);
	if (!initialized)
		initialized = true;
}

bool ScTextBrowserPlugin::isInitialized() const
{
	return initialized;
}

QWidget* ScTextBrowserPlugin::createWidget(QWidget* parent)
{
	return new ScTextBrowser(parent);
}

QString ScTextBrowserPlugin::name() const
{
	return QLatin1String("ScTextBrowser");
}

QString ScTextBrowserPlugin::group() const
{
	return QLatin1String("Scribus");
}

QIcon ScTextBrowserPlugin::icon() const
{
	return QIcon();
}

QString ScTextBrowserPlugin::toolTip() const
{
	return QString();
}

QString ScTextBrowserPlugin::whatsThis() const
{
	return QString();
}

bool ScTextBrowserPlugin::isContainer() const
{
	return false;
}

QString ScTextBrowserPlugin::domXml() const
{
	return QLatin1String("<widget class=\"ScTextBrowser\" name=\"ScTextBrowser1\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>256</width>\n"
		"   <height>192</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n");
}

QString ScTextBrowserPlugin::includeFile() const
{
	return QLatin1String("sctextbrowser.h");
}

QString ScTextBrowserPlugin::codeTemplate() const
{
	return QString();
}

 Q_EXPORT_PLUGIN2(scribus_sctextbrowser, ScTextBrowserPlugin)

