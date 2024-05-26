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
