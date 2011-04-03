/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGET_TEXTCOLOR_H
#define PROPERTYWIDGET_TEXTCOLOR_H

#include "ui_propertywidget_textcolorbase.h"

#include "propertywidgetbase.h"

class ScribusDoc;
class ScribusMainWindow;

class PropertyWidget_TextColor : public QFrame, Ui::PropertyWidget_TextColorBase,
	                             public PropertyWidgetBase
{
	Q_OBJECT

public:
	PropertyWidget_TextColor(QWidget* parent);
	~PropertyWidget_TextColor() {};

	virtual void changeEvent(QEvent *e);

	void updateColorList();

protected:
	void connectSignals();
	void disconnectSignals();

	PageItem *         m_item;
	ScribusMainWindow* m_ScMW;

	void enableFromSelection(void);
	void setCurrentItem(PageItem *i);

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);

	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void languageChange();
	void unitChange() {};

	void displayOutlineW(double x);
	void displayShadowOffset(double x, double y);
	void displayStrikeThru(double p, double w);
	void displayTextColors(QString p, QString b, double shp, double shb);
	void displayTextEffects(int s);
	void displayUnderline(double p, double w);

	void updateCharStyle(const CharStyle& charStyle);
	void updateStyle(const ParagraphStyle& newCurrent);

private slots:
	void handleOutlineWidth();
	void handleShadowOffs();
	void handleStrikeThru();
	void handleTextFill();
	void handleTextShade();
	void handleTextStroke();
	void handleTextDirection();
	void handleTypeStyle(int s);
	void handleUnderline();
};

#endif
