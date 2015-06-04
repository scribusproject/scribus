/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_SHADOW_H
#define PROPERTIESPALETTE_SHADOW_H

#include "scribusapi.h"
#include "scguardedptr.h"

#include "proptree.h"

class PageItem;
class ScribusDoc;
class Selection;
class ScribusMainWindow;

class SCRIBUS_API PropertiesPalette_Shadow : public PropTreeWidget
{
	Q_OBJECT

public:
	PropertiesPalette_Shadow(QWidget* parent);
	~PropertiesPalette_Shadow() {};

	virtual void changeEvent(QEvent *e);
	void updateColorList();

private:

	PageItem* currentItemFromSelection();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *i);
	void unsetDoc();
	void unsetItem();

	void languageChange();
	void unitChange();
	void handleSelectionChanged();
	void handleUpdateRequest(int);

private slots:
	void handleNewValues();

protected slots:
	void updateSpinBoxConstants();

protected:
	PropTreeItem *hasSoftShadow;
	PropTreeItem *softShadowXOffset;
	PropTreeItem *softShadowYOffset;
	PropTreeItem *softShadowBlurRadius;
	PropTreeItem *colorProps;
	PropTreeItem *softShadowColor;
	PropTreeItem *softShadowShade;
	PropTreeItem *softShadowOpacity;
	PropTreeItem *softShadowBlendMode;
	PropTreeItem *softShadowErase;
	PropTreeItem *softShadowObjTrans;
	ScribusMainWindow *m_ScMW;

	bool      m_haveDoc;
	bool      m_haveItem;
	double    m_unitRatio;
	int       m_unitIndex;
	PageItem *m_item;
	ScGuardedPtr<ScribusDoc> m_doc;
};

#endif
