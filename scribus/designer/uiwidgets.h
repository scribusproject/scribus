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
