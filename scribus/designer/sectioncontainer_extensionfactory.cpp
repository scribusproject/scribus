#include "sectioncontainer_extensionfactory.h"

#include "sectioncontainer_extension.h"
#include "../ui/widgets/section_container.h"


SectionContainerExtensionFactory::SectionContainerExtensionFactory(QExtensionManager *parent)
	: QExtensionFactory(parent)
{}

QObject *SectionContainerExtensionFactory::createExtension(QObject *object,
														  const QString &iid,
														  QObject *parent) const
{
	SectionContainer *widget = qobject_cast<SectionContainer*>(object);

	if (widget && (iid == Q_TYPEID(QDesignerContainerExtension)))
		return new SectionContainerExtension(widget, parent);
	return nullptr;
}
