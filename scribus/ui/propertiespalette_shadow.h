/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_SHADOW_H
#define PROPERTIESPALETTE_SHADOW_H

#include "ui_propertiespalette_shadowbase.h"

#include "scribusapi.h"
#include "scguardedptr.h"

#include "units.h"

class PageItem;
class ScribusDoc;
class Selection;
class ScribusMainWindow;

class SCRIBUS_API PropertiesPalette_Shadow : public QWidget, Ui::PropertiesPalette_ShadowBase
{
	Q_OBJECT

public:
	PropertiesPalette_Shadow(QWidget* parent);
	~PropertiesPalette_Shadow() {};

	void updateColorList();

private:
	PageItem* currentItemFromSelection();

	void connectSlots();
	void disconnectSlots();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void languageChange();
	void iconSetChange();
	void unitChange();
	void localeChange();
	void toggleLabelVisibility(bool v);

	void handleSelectionChanged();
	void handleUpdateRequest(int);

private slots:
	void handleNewValues();
	void handleInheritOpacity();

protected slots:
	void updateSpinBoxConstants();

protected:
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
