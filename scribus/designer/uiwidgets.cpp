#include "uiwidgets.h"
#include "formwidget_plugin.h"

UIWidgets::UIWidgets(QObject *parent)
		: QObject(parent)
{
	widgets.append(new FormWidgetPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> UIWidgets::customWidgets() const
{
	return widgets;
}
