/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGET_PATHTEXT_H
#define PROPERTYWIDGET_PATHTEXT_H

#include "ui_propertywidget_pathtextbase.h"

#include "propertywidgetbase.h"

class ScribusDoc;
class ScribusMainWindow;

class PropertyWidget_PathText : public QFrame, public Ui::PropertyWidget_PathTextBase,
                                public PropertyWidgetBase
{
	Q_OBJECT

public:
	PropertyWidget_PathText(QWidget* parent);
	~PropertyWidget_PathText() {};

protected:
	PageItem *         m_item;
	ScribusMainWindow* m_ScMW;

	double m_unitRatio;
	int    m_unitIndex;

	void connectSignals();
	void disconnectSignals();

	void configureWidgets(void);
	void setCurrentItem(PageItem *item);

	virtual void changeEvent(QEvent *e);

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);

	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void languageChange();
	void unitChange();

private slots:
	void handlePathType();
	void handlePathFlip();
	void handlePathLine();
	void handlePathDist();
	void handlePathOffs();
};

#endif
