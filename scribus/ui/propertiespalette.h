/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_H
#define PROPERTIESPALETTE_H

#include <QListWidgetItem>
#include <QLineEdit>

class QCloseEvent;
class QEvent;
class QFocusEvent;
class QToolBox;
class QVBoxLayout;
class QWidget;

#include "scribusapi.h"
#include "scdockpalette.h"
#include "scguardedptr.h"
#include "scrspinbox.h"
#include "pageitem.h"
#include "linkbutton.h"
#include "linecombo.h"
#include "scguardedptr.h"
#include "sclistboxpixmap.h"
#include "scpage.h"
#include "spalette.h"
#include "sctreewidget.h"

class ColorCombo;
class ColorPalette;
class NameWidget;
class PropertiesPalette_Group;
class PropertiesPalette_Line;
class PropertiesPalette_Shadow;
class PropertiesPalette_Shape;
class PropertiesPalette_Table;
class PropertiesPalette_XYZ;
class ScComboBox;
class ScribusDoc;
class ScribusMainWindow;
class Selection;
class UndoManager;
class TransparencyPalette;

class SCRIBUS_API PropertiesPalette : public ScDockPalette
{
	Q_OBJECT

public:
	PropertiesPalette(QWidget* parent);
	~PropertiesPalette() {}

	virtual void changeEvent(QEvent *e);
	virtual void closeEvent(QCloseEvent *closeEvent);

	void endPatchAdd();
	void updateColorSpecialGradient();
	void updateColorList();
	void setGradientEditMode(bool);
	void setTextFlowMode(PageItem::TextFlowMode mode);

	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // commmited

	PropertiesPalette_Group* groupPal {nullptr};
	PropertiesPalette_Line* linePal {nullptr};
	PropertiesPalette_Shadow* shadowPal {nullptr};
	PropertiesPalette_Shape* shapePal {nullptr};
	PropertiesPalette_Table* tablePal {nullptr};
	PropertiesPalette_XYZ* xyzPal {nullptr};
	ColorPalette *colorPalette {nullptr};
	TransparencyPalette *transparencyPalette {nullptr};

private:
	PageItem* currentItemFromSelection();
	
public slots:
	void setMainWindow(ScribusMainWindow *mw);
	
	void setDoc(ScribusDoc *d);
	void AppModeChanged();
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	
	void unitChange();
	void languageChange();

	void setLocked(bool);

private slots:
	void SelTab(int t);

	void NewLineMode(int mode);
	void handleGradientChanged();
	void handleNewShape(int frameType);
	void NewSpGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy);
	void toggleGradientEdit(int);
	void NewSpGradientM(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk);
	void toggleGradientEditM();
	void handleShapeEdit();

protected:
	ScribusMainWindow *m_ScMW {nullptr};
	ScTreeWidget* TabStack {nullptr};

	bool m_haveDoc {false};
	bool m_haveItem {false};
	double m_unitRatio {1.0};
	int m_unitIndex {0};
	PageItem* m_item {nullptr};
	UndoManager *undoManager {nullptr};
	
	ScGuardedPtr<ScribusDoc> m_doc;

	int idXYZItem {0};
	int idShapeItem {0};
	int idTableItem {0};
	int idLineItem {0};
	int idColorsItem {0};
	int idTransparencyItem {0};
	int idGroupItem {0};
	int idShadowItem {0};
};

#endif
