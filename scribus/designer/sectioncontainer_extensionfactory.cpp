/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/

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
