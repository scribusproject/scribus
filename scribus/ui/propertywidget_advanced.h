/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGET_ADVANCED_H
#define PROPERTYWIDGET_ADVANCED_H

#include "ui_propertywidget_advancedbase.h"

#include "propertywidgetbase.h"

class CharStyle;
class ParagraphStyle;
class ScribusDoc;
class ScribusMainWindow;

class PropertyWidget_Advanced : public QFrame, Ui::PropertyWidget_AdvancedBase,
                                public PropertyWidgetBase
{
	Q_OBJECT

public:
	PropertyWidget_Advanced(QWidget* parent);
	~PropertyWidget_Advanced() {};

	virtual void changeEvent(QEvent *e);

protected:
	void connectSignals();
	void disconnectSignals();

	double m_unitRatio;
	int    m_unitIndex;

	PageItem*          m_item;
	ScribusMainWindow* m_ScMW;

	void enableFromSelection(void);
	void setCurrentItem(PageItem *i);

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);

	void handleSelectionChanged();

	void languageChange();
	void unitChange() {};

	void displayBaseLineOffset(double e);
	void displayTextScaleH(double e);
	void displayTextScaleV(double e);
	void displayTracking(double e);

	void updateCharStyle(const CharStyle& charStyle);
	void updateStyle(const ParagraphStyle& newCurrent);

private slots:
	void handleBaselineOffset();
	void handleMinWordTracking();
	void handleNormWordTracking();
	void handleMinGlyphExtension();
	void handleMaxGlyphExtension();
	void handleTextScaleH();
	void handleTextScaleV();
	void handleTracking();

};

#endif
