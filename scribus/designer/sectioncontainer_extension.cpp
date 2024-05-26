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

#include "sectioncontainer_extension.h"

#include "../ui/widgets/section_container.h"

SectionContainerExtension::SectionContainerExtension(SectionContainer *widget,
																	 QObject *parent)
	: QObject(parent)
	, myWidget(widget)
{
}

bool SectionContainerExtension::canAddWidget() const
{
    return !myWidget->hasWidget();
}

void SectionContainerExtension::addWidget(QWidget *widget)
{
	myWidget->setWidget(widget);
}

int SectionContainerExtension::count() const
{
	return 1;
}

int SectionContainerExtension::currentIndex() const
{
	return 0;
}

void SectionContainerExtension::insertWidget(int index, QWidget *widget)
{
	//myWidget->insertPage(index, widget);
	Q_UNUSED(index);
	Q_UNUSED(widget);
	//addWidget(widget);
}

bool SectionContainerExtension::canRemove(int index) const
{
	Q_UNUSED(index);
    return myWidget->hasWidget();
}

void SectionContainerExtension::remove(int index)
{
	//myWidget->removePage(index);
	Q_UNUSED(index);
	myWidget->removeWidget();
}

void SectionContainerExtension::setCurrentIndex(int index)
{
	Q_UNUSED(index);
}

QWidget* SectionContainerExtension::widget(int index) const
{
	Q_UNUSED(index);
	return myWidget->widget();
}

