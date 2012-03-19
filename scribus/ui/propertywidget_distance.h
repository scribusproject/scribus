/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGET_DISTANCE_H
#define PROPERTYWIDGET_DISTANCE_H

#include "ui_propertywidget_distancebase.h"

#include "propertywidgetbase.h"

class ScribusDoc;
class ScribusMainWindow;

class PropertyWidget_Distance : public QFrame, public Ui::PropertyWidget_DistanceBase,
                                public PropertyWidgetBase
{
	Q_OBJECT

public:
	PropertyWidget_Distance(QWidget* parent);
	~PropertyWidget_Distance() {};

protected:
	void connectSignals();
	void disconnectSignals();

	PageItem *         m_item;
	ScribusMainWindow* m_ScMW;

	double m_unitRatio;
	int    m_unitIndex;

	void configureWidgets(void);
	void setCurrentItem(PageItem *item);

	virtual void changeEvent(QEvent *e);

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);

	void handleAppModeChanged(int oldMode, int mode);
	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void languageChange();
	void unitChange();

	void displayColumns(int r, double g);
	void displayTextDistances(double left, double top, double bottom, double right);

private slots:
	void handleColumns();
	void handleColumnGap();
	void handleGapSwitch();
	void handleTabs();
	void handleTextDistances();
};

#endif
