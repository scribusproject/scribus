#ifndef PROPERTYWIDGET_DropCap_H
#define PROPERTYWIDGET_DropCap_H

#include "ui_propertywidget_dropcapbase.h"

#include "propertywidgetbase.h"

class PageItem;
class ParagraphStyle;
class ScribusMainWindow;

class PropertyWidget_DropCap : public QFrame, private Ui::PropertyWidget_DropCapBase, public PropertyWidgetBase
{
    Q_OBJECT

public:
	PropertyWidget_DropCap(QWidget *parent = 0);
	~PropertyWidget_DropCap() {}

	void updateStyle(const ParagraphStyle& newCurrent);
	void updateCharStyles();

	void displayCharStyle(const QString& name);

protected:
	double m_unitRatio;
	int    m_unitIndex;

	PageItem *         m_item;
	ScribusMainWindow* m_ScMW;

	void configureWidgets();
	void setCurrentItem(PageItem *item);

	virtual void changeEvent(QEvent *e);

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *doc);

	void handleAppModeChanged(int oldMode, int mode);
	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void languageChange();
	void unitChange();

	void handleDropCapUse();
	void handleDropCapLines();
	void handleDropCapOffset();
	void handleDropCapCharStyle();

private:
	void connectSignals();
	void disconnectSignals();
	void enableDropCap(bool);
};

#endif // PROPERTYWIDGET_DropCap_H
