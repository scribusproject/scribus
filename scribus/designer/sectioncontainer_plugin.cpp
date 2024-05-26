#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerContainerExtension>
#include <QtDesigner/QDesignerPropertySheetExtension>

#include <QIcon>
#include <QtPlugin>

#include "../ui/widgets/section_container.h"
#include "sectioncontainer_plugin.h"
#include "sectioncontainer_extensionfactory.h"


SectionContainerPlugin::SectionContainerPlugin(QObject *parent)
	: QObject(parent)
{
}

QString SectionContainerPlugin::name() const
{
	return QLatin1String("SectionContainer");
}

QString SectionContainerPlugin::group() const
{
	return QLatin1String("Scribus UI");
}

QString SectionContainerPlugin::toolTip() const
{
	return QString();
}

QString SectionContainerPlugin::whatsThis() const
{
	return QString();
}

QString SectionContainerPlugin::includeFile() const
{
	return QLatin1String("ui/widgets/section_container.h");
}

QIcon SectionContainerPlugin::icon() const
{
	return QIcon();
}

bool SectionContainerPlugin::isContainer() const
{
	return true;
}

QWidget *SectionContainerPlugin::createWidget(QWidget *parent)
{
	SectionContainer *widget = new SectionContainer(parent);
//	connect(widget, &SectionContainer::currentIndexChanged,
//			this, &SectionContainerPlugin::currentIndexChanged);
	return widget;
}

bool SectionContainerPlugin::isInitialized() const
{
	return initialized;
}

void SectionContainerPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
	Q_UNUSED(formEditor)
	if (initialized)
		return;

	QExtensionManager *manager = formEditor->extensionManager();
	QExtensionFactory *factory = new SectionContainerExtensionFactory(manager);

	Q_ASSERT(manager != 0);
	manager->registerExtensions(factory, Q_TYPEID(QDesignerContainerExtension));

	initialized = true;
}

QString SectionContainerPlugin::domXml() const
{
	return QLatin1String(R"(
<ui language="c++" displayname="Section Container">
	<widget class="SectionContainer" name="sectionContainer">
	 <widget class="QWidget" name="page">
	  <layout class="QVBoxLayout" name="verticalLayout"/>
	 </widget>
	</widget>
	<customwidgets>
		<customwidget>
			<class>SectionContainer</class>
			<extends>QWidget</extends>
			<addpagemethod>setWidget</addpagemethod>
		</customwidget>
	</customwidgets>
</ui>)");
}
