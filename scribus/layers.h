#ifndef LAYERPALETTE_H
#define LAYERPALETTE_H

#include <q3table.h>
#include <q3ptrlist.h>
#include <q3valuelist.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribus.h"
#include "scribusapi.h"
#include "scrpalettebase.h"

class QPushButton;
class QLayout;
class QToolTip;
class QPixmap;
class Q3Header;


class SCRIBUS_API LayerTable : public Q3Table
{
	Q_OBJECT

public:
	LayerTable(QWidget* parent);
	~LayerTable() {};

protected:
	virtual void endEdit ( int row, int col, bool accept, bool replace );

signals:
	void updtName(int);
};

class SCRIBUS_API LayerPalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	LayerPalette(QWidget* parent);
	~LayerPalette() {};

	void setLayers(Q3ValueList<Layer> *layin, int act);
	void rebuildList();
	
	Q3Table* Table;	//public for the event filter in scribus.cpp.. TODO

public slots:
	void updateName(int r);
	void addLayer();
	void removeLayer();
	void upLayer();
	void downLayer();
	void changeName(int row, int col);
	void visibleLayer();
	void printLayer();
	void setActiveLayer(int row);
	void ClearInhalt();
	void markActiveLayer(int layerNumber=-1);
	void languageChange();

signals:
	void LayerChanged();
	void LayerActivated(int);

protected:
	QVBoxLayout* LayerPaletteLayout;
	QHBoxLayout* Layout1;
	Q3Header* Header;
	QPushButton* newLayerButton;
	QPushButton* deleteLayerButton;
	QPushButton* raiseLayerButton;
	QPushButton* lowerLayerButton;
	Q3PtrList<QCheckBox> flagsPrintable;
	Q3PtrList<QCheckBox> flagsVisible;
	Q3ValueList<Layer> *layers;
};

#endif // LAYERPALETTE_H
