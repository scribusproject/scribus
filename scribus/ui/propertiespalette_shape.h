/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_SHAPE_H
#define PROPERTIESPALETTE_SHAPE_H

#include "ui_propertiespalette_shapebase.h"

#include "scribusapi.h"
#include "scrspinbox.h"
#include "pageitem.h"
#include "linkbutton.h"
#include "sclistboxpixmap.h"
#include "scguardedptr.h"
#include "sctreewidget.h"

class Autoforms;
class ScribusDoc;
class ScribusMainWindow;
class Selection;

class SCRIBUS_API PropertiesPalette_Shape : public QWidget, Ui::PropertiesPalette_ShapeBase
{
	Q_OBJECT

public:
	PropertiesPalette_Shape(QWidget* parent);
	~PropertiesPalette_Shape() {};

	virtual void changeEvent(QEvent *e);

	void setCustomShapeIcon(int submode);
	void setLocked(bool isLocked);
	void setRoundRectEnabled(bool enabled);
	void displayTextFlowMode(PageItem::TextFlowMode mode);

protected:

	bool      m_haveDoc;
	bool      m_haveItem;
	double    m_unitRatio;
	int       m_unitIndex;
	PageItem *m_item;
	ScGuardedPtr<ScribusDoc> m_doc;

	Selection* m_tmpSelection;

private:

	PageItem* currentItemFromSelection();

protected:
	ScribusMainWindow *m_ScMW;
	
	void enableCustomShape();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *i);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void languageChange();
	void unitChange();

private slots:

	void handleTextFlow();
	void handleShapeEdit();
	void handleShapeEditEnded();
	void handleCornerRadius();
	void handleFillRule();
	void handleNewShape(int, int, qreal *);

signals:
	void DocChanged();
};

#endif
