/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGET_OPENTYPEFONTFEATURES_H
#define PROPERTYWIDGET_OPENTYPEFONTFEATURES_H

#include "ui_propertywidget_fontfeaturesbase.h"
#include "propertywidgetbase.h"

class CharStyle;
class ParagraphStyle;
class ScribusDoc;
class ScribusMainWindow;

class PropertyWidget_FontFeatures : public QFrame, Ui::PropertyWidget_FontFeatures,
		public PropertyWidgetBase
{
	Q_OBJECT

public:
	PropertyWidget_FontFeatures(QWidget *parent);
	~PropertyWidget_FontFeatures() {}
	void enableFeatures(QStringList fontFeatures);

protected:
	void connectSignals();
	void disconnectSignals();
	void disableAllFeatures();
	PageItem* m_item;
	ScribusMainWindow* m_ScMW;

	void configureWidgets();
	void initWidgets();
	void setCurrentItem(PageItem *item);
	virtual void changeEvent(QEvent *e);

	quint64 featureFlags();

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);
	void handleSelectionChanged();
	void languageChange();
	void unitChange() {}
	void showFontFeatures(QString s, QStringList availableFeatures);
	void updateCharStyle(const CharStyle& charStyle);
	void updateStyle(const ParagraphStyle& newCurrent);

private slots:
	void handleFontFeatures();

signals:
	void needsRelayout();
};

#endif
