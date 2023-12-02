#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerPropertySheetExtension>

#include <QIcon>
#include <QtPlugin>

#include "../ui/widgets/form_widget.h"
#include "formwidget_plugin.h"


FormWidgetPlugin::FormWidgetPlugin(QObject *parent)
	: QObject(parent)
{
}

QString FormWidgetPlugin::name() const
{
	return QLatin1String("FormWidget");
}

QString FormWidgetPlugin::group() const
{
	return QLatin1String("Scribus UI");
}

QString FormWidgetPlugin::toolTip() const
{
	return QString();
}

QString FormWidgetPlugin::whatsThis() const
{
	return QString();
}

QString FormWidgetPlugin::includeFile() const
{
	return QLatin1String("ui/widgets/form_widget.h");
}

QIcon FormWidgetPlugin::icon() const
{
	return QIcon();
}

bool FormWidgetPlugin::isContainer() const
{
	return true;
}

QWidget *FormWidgetPlugin::createWidget(QWidget *parent)
{
	FormWidget *widget = new FormWidget(parent);
	return widget;
}

bool FormWidgetPlugin::isInitialized() const
{
	return initialized;
}

void FormWidgetPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
	Q_UNUSED(formEditor)
	if (initialized)
		return;

	initialized = true;
}

QString FormWidgetPlugin::domXml() const
{
	return QLatin1String(R"(
<ui language="c++" displayname="Form Widget">
	<widget class="FormWidget" name="formWidget">
		<property name="sizePolicy">
		 <sizepolicy hsizetype="Preferred" vsizetype="Maximum">
		  <horstretch>0</horstretch>
		  <verstretch>0</verstretch>
		 </sizepolicy>
		</property>
		<property name="font">
		 <font>
		  <pointsize>8</pointsize>
		 </font>
		</property>
			<layout class="QHBoxLayout" name="horizontalLayout">
			  <property name="leftMargin">
			   <number>0</number>
			  </property>
			  <property name="topMargin">
			   <number>0</number>
			  </property>
			  <property name="rightMargin">
			   <number>0</number>
			  </property>
			  <property name="bottomMargin">
			   <number>0</number>
			  </property>
			</layout>
	</widget>
	<customwidgets>
		<customwidget>
			<class>FormWidget</class>
			<extends>QWidget</extends>
		</customwidget>
	</customwidgets>
</ui>)");
}
