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

#include "uiwidgets.h"
#include "formwidget_plugin.h"
#include "sectioncontainer_plugin.h"

UIWidgets::UIWidgets(QObject *parent)
		: QObject(parent)
{
	widgets.append(new FormWidgetPlugin(this));
	widgets.append(new SectionContainerPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> UIWidgets::customWidgets() const
{
	return widgets;
}
