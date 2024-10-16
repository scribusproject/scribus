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
#include "fonts/scface.h"

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
	void enableFeatures(const QStringList& fontFeatures);

protected:
	void connectSignals();
	void disconnectSignals();
	void disableAllFeatures();

	void configureWidgets();
	void initWidgets();
	void setCurrentItem(PageItem *item);
	void changeEvent(QEvent *e) override;
	bool eventFilter(QObject *obj, QEvent *event) override;

	quint64 featureFlags() const;

	// bool m_blockUpdate {false};
	PageItem* m_item { nullptr };
	ScribusMainWindow* m_ScMW { nullptr };

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);
	void handleSelectionChanged();
	void iconSetChange();
	void languageChange();
	void localeChange();
	void unitChange() {}
	void toggleLabelVisibility(bool v);

	// void showTextColors(const QString& strokeCol, double strokeShd);
	void showFontFeatures(const QString& s, const QStringList& availableFeatures);
	// void showOutlineW(double x);
	// void showShadowOffset(double x, double y);
	// void showStrikeThru(double p, double w);
	// void showTextEffects(int s);
	// void showUnderline(double p, double w);

	void updateCharStyle(const CharStyle& charStyle);
	void updateStyle(const ParagraphStyle& newCurrent);

private slots:
	// void handleOutlineWidth();
	// void handleShadowOffs();
	// void handleStrikeThru();
	// void handleTypeStyle(int s);
	// void handleUnderline();
	// void handleTextOutline();
	void handleFontFeatures();

	void handleFontPreview(QObject *obj);

signals:
	void needsRelayout();
};

#endif
