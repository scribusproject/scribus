/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef LAYERPALETTE_H
#define LAYERPALETTE_H

#include <qtable.h>
#include <qptrlist.h>
#include <qvaluelist.h>
#include <qcheckbox.h>

#include "scribusapi.h"
#include "scrpalettebase.h"

class QPushButton;
class QLayout;
class QToolTip;
class QPixmap;
class QHeader;
class QSpinBox;
class QLabel;
class ScComboBox;


class SCRIBUS_API LayerTable : public QTable
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

	void setLayers(QValueList<Layer> *layin, int act);
	void rebuildList();
	
	QTable* Table;	//public for the event filter in scribus.cpp.. TODO

public slots:
	void updateName(int r);
	void addLayer();
	void removeLayer();
	void upLayer();
	void downLayer();
	void changeName(int row, int col);
	void visibleLayer();
	void printLayer();
	void lockLayer();
	void flowToggleLayer();
	void outlineToggleLayer();
	void markLayer();
	void changeOpacity();
	void changeBlendMode(int blend);
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
	QHBoxLayout* layout1;
	QLabel* textLabel1;
	ScComboBox* blendMode;
	QLabel* textLabel2;
	QSpinBox* opacitySpinBox;
	QHeader* Header;
	QPushButton* newLayerButton;
	QPushButton* deleteLayerButton;
	QPushButton* raiseLayerButton;
	QPushButton* lowerLayerButton;
	QPtrList<QCheckBox> flagsPrintable;
	QPtrList<QCheckBox> flagsVisible;
	QPtrList<QCheckBox> flagsLocked;
	QPtrList<QCheckBox> flagsFlow;
	QPtrList<QCheckBox> flagsOutline;
	QPtrList<QPushButton> flagsMarker;
	QValueList<Layer> *layers;
};

#endif // LAYERPALETTE_H
