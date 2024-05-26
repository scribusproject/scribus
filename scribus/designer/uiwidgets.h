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

#ifndef UIWIDGETS_H
#define UIWIDGETS_H

#include <QtCore/qplugin.h>
#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>

class UIWidgets : public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "net.scribus.uiwidgets")
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
	UIWidgets(QObject *parent = 0);

	QList<QDesignerCustomWidgetInterface*> customWidgets() const override;

private:
	QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif // UIWIDGETS_H
