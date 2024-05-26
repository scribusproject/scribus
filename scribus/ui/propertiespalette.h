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

#include "pageitem.h"
#include "linecombo.h"
#include "linkbutton.h"
#include "scdockpalette.h"
#include "scguardedptr.h"
#include "scguardedptr.h"
#include "sclistboxpixmap.h"
#include "scpage.h"
#include "scribusapi.h"
#include "scrspinbox.h"
#include "sctreewidget.h"
#include "stylecombos.h"
#include "units.h"
#include "docks/dock_panelbase.h"
#include "widgets/section_container.h"

class ColorCombo;
class ColorPalette;
class NameWidget;
class PropertiesPalette_Attributes;
class PropertiesPalette_Group;
class PropertiesPalette_Fill;
class PropertiesPalette_Line;
class PropertiesPalette_Shadow;
class PropertiesPalette_Shape;
class PropertiesPalette_Table;
class PropertiesPalette_XYZ;
class ScribusDoc;
class ScribusMainWindow;
class Selection;
class UndoManager;
class TransparencyPalette;

class SCRIBUS_API PropertiesPalette : public DockPanelBase
{
	Q_OBJECT

public:
	PropertiesPalette(QWidget* parent);
	~PropertiesPalette() {}

	void endPatchAdd();
//	void updateColorSpecialGradient();
	void updateColorList();
	void setGradientEditMode(bool);
	void setTextFlowMode(PageItem::TextFlowMode mode);

	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // committed

	PropertiesPalette_Line* linePal {nullptr};
	PropertiesPalette_Fill* fillPal {nullptr};
	PropertiesPalette_Shadow* shadowPal {nullptr};
	PropertiesPalette_Shape* shapePal {nullptr};
	PropertiesPalette_XYZ* xyzPal {nullptr};
	PropertiesPalette_Attributes* attributesPal {nullptr};

protected:
	void changeEvent(QEvent *e) override;
	void closeEvent(QCloseEvent *closeEvent) override;

private:
	PageItem* currentItemFromSelection();
	void enablePalettes(bool enable);
	
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

	void handleNewShape(int frameType);
	void handleShapeEdit();

	// Gradient Properties
	void meshGradientToShape();
	void createNewMeshGradient();
	void resetMeshGradient();
	void resetOneControlPoint();
	void resetAllControlPoints();
	void snapToPatchGrid(bool val);
	void handleRemovePatch();

protected:

	SectionContainer * scXYZ {nullptr};
	SectionContainer * scShape {nullptr};
	SectionContainer * scLine {nullptr};
	SectionContainer * scFill {nullptr};
	SectionContainer * scShadow {nullptr};
	SectionContainer * scAttributes {nullptr};

	bool m_haveDoc {false};
	bool m_haveItem {false};
	double m_unitRatio {1.0};
	int m_unitIndex {SC_PT};
	PageItem *m_item {nullptr};
	ScribusMainWindow* m_ScMW {nullptr};
	UndoManager *undoManager {nullptr};
	
	ScGuardedPtr<ScribusDoc> m_doc;
};

#endif
