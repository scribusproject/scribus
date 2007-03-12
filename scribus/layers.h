/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef LAYERPALETTE_H
#define LAYERPALETTE_H

#include <q3table.h>
#include <q3ptrlist.h>
#include <q3valuelist.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

#include "scribusapi.h"
#include "scrpalettebase.h"

class QToolButton;
class QPushButton;
class QLayout;
class QToolTip;
class QPixmap;
class Q3Header;
class QSpinBox;
class QLabel;
class ScComboBox;
class ScribusDoc;
class DynamicTip;

class SCRIBUS_API LayerLabel : public Q3TableItem
{
public:
	LayerLabel(Q3Table* parent);
	~LayerLabel() {};
	virtual void paint(QPainter * p, const QColorGroup &cg, const QRect &cr, bool selected);
};

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

	void setDoc(ScribusDoc* doc);
	void rebuildList();
	
	Q3Table* Table;	//public for the event filter in scribus.cpp.. TODO

public slots:
	void updateName(int r);
	void addLayer();
	void dupLayer();
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
	ScribusDoc* m_Doc;
	Q3VBoxLayout* LayerPaletteLayout;
	Q3HBoxLayout* Layout1;
	Q3HBoxLayout* layout1;
	QLabel* textLabel1;
	ScComboBox* blendMode;
	QLabel* textLabel2;
	QSpinBox* opacitySpinBox;
	Q3Header* Header;
	QPushButton* newLayerButton;
	QPushButton* duplicateLayerButton;
	QPushButton* deleteLayerButton;
	QPushButton* raiseLayerButton;
	QPushButton* lowerLayerButton;
	Q3PtrList<QCheckBox> flagsPrintable;
	Q3PtrList<QCheckBox> flagsVisible;
	Q3PtrList<QCheckBox> flagsLocked;
	Q3PtrList<QCheckBox> flagsFlow;
	Q3PtrList<QCheckBox> flagsOutline;
	Q3PtrList<QToolButton> flagsMarker;
	Q3ValueList<Layer> *layers;
	DynamicTip* dynTip;
};

#endif // LAYERPALETTE_H
