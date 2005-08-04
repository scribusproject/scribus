#ifndef LAYERPALETTE_H
#define LAYERPALETTE_H

#include <qtable.h>
#include <qptrlist.h>
#include <qvaluelist.h>

#include "scrpalettebase.h"

class QPushButton;
class QLayout;
class QToolTip;
class QPixmap;
class QCheckBox;
class QHeader;


class LayerTable : public QTable
{
	Q_OBJECT

public:
	LayerTable(QWidget* parent);
	~LayerTable() {};
	//void keyPressEvent(QKeyEvent *k);

protected:
	virtual void endEdit ( int row, int col, bool accept, bool replace );

signals:
	//void ToggleAllPalettes();
	//void Schliessen();
	void updtName(int);
};

class LayerPalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	LayerPalette(QWidget* parent);
	~LayerPalette() {};

	//void closeEvent(QCloseEvent *ce);
	void setLayers(QValueList<Layer> *layin, int act);
	void rebuildList();
	
	QTable* Table;	//public for the event filter in scribus.cpp.. TODO

public slots:
	void updateName(int r);
	void addLayer();
	void removeLayer();
	void removeLayer(bool deleteItems);
	void upLayer();
	void downLayer();
	void changeName(int row, int col);
	void visibleLayer();
	void printLayer();
	void printLayer(int layerNr, bool isPrintable);
	void setActiveLayer(int row);
	void ClearInhalt();
	void markActiveLayer(int layerNumber=-1);
	void languageChange();

signals:
	void LayerChanged();
	void LayerActivated(int);
	//void Schliessen();

protected:
	QVBoxLayout* LayerPaletteLayout;
	QHBoxLayout* Layout1;
	QHeader* Header;
	QPushButton* newLayerButton;
	QPushButton* deleteLayerButton;
	QPushButton* raiseLayerButton;
	QPushButton* lowerLayerButton;
	QPtrList<QCheckBox> flagsPrintable;
	QPtrList<QCheckBox> flagsVisible;
	QValueList<Layer> *layers;
		
protected slots:
	//virtual void reject();
};

#endif // LAYERPALETTE_H
