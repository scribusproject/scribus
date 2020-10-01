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
#include "units.h"

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

	void updateColorList();

private:
	PageItem* currentItemFromSelection();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void languageChange();
	void unitChange();
	void localeChange();

	void handleSelectionChanged();
	void handleUpdateRequest(int);

private slots:
	void handleNewValues();

protected slots:
	void updateSpinBoxConstants();

protected:
	PropTreeItem *hasSoftShadow {nullptr};
	PropTreeItem *softShadowXOffset {nullptr};
	PropTreeItem *softShadowYOffset {nullptr};
	PropTreeItem *softShadowBlurRadius {nullptr};
	PropTreeItem *softShadowColor {nullptr};
	PropTreeItem *softShadowShade {nullptr};
	PropTreeItem *softShadowOpacity {nullptr};
	PropTreeItem *softShadowBlendMode {nullptr};
	PropTreeItem *softShadowErase {nullptr};
	PropTreeItem *softShadowObjTrans {nullptr};
	bool m_haveDoc {false};
	bool m_haveItem {false};
	double m_unitRatio {1.0};
	int m_unitIndex {SC_PT};
	PageItem *m_item {nullptr};
	ScribusMainWindow* m_ScMW {nullptr};
	ScGuardedPtr<ScribusDoc> m_doc;

	void changeEvent(QEvent *e) override;
};

#endif
