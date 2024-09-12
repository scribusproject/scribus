/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGELAYERS_H
#define IMAGELAYERS_H

#include <QWidget>
#include <QList>
#include "ui_imagelayers.h"
#include "scribusapi.h"
#include "scimagestructs.h"

class QCheckBox;
class ScribusView;
class PageItem;

class SCRIBUS_API ImageLayers : public QWidget, Ui::ImageLayers
{
	Q_OBJECT

public:
	explicit ImageLayers(QWidget *parent = nullptr);
	~ImageLayers() {};

	void setCurrentItem(PageItem *item, ScribusView *view );

public slots:
	void updateLayerInfo();
	void selectLayer();

protected:

	ScribusView *m_view {nullptr};
	PageItem *m_item {nullptr};
	int currentLayer {-1};
	QMap<int, ImageLoadRequest> originalRequestProps;

	void addListItem(QList<PSDLayer>::iterator it, int layerID, ImageInfoRecord *info);
	void updateListItem(QList<PSDLayer>::iterator it, int layerID, ImageInfoRecord *info);
	int tableRow(int layer) { return layerTable->rowCount() - layer - 1; };

};

#endif // IMAGELAYERS_H
