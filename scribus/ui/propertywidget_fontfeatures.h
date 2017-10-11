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
	quint64 featureFlagsFromList(QStringList features);

	// Cannot use an enum here (yet) as C++98 doesn't guarantee an enum
	// can reach 64bit in size
	static quint64 FeatureCommonLigatures;
	static quint64 FeatureContextualLigatures;
	static quint64 FeatureDiscretionaryLigatures;
	static quint64 FeatureHistoricalLigatures;
	static quint64 FeatureSmallCaps;
	static quint64 FeatureSmallCapsFromCaps;
	static quint64 FeaturePetiteCaps;
	static quint64 FeaturePetiteCapsFromCaps;
	static quint64 FeatureUnicaseCaps;
	static quint64 FeatureTiltingCaps;
	static quint64 FeatureLiningNumerals;
	static quint64 FeatureOldStyleNumerals;
	static quint64 FeatureProportionalNumeralWidth;
	static quint64 FeatureTabularNumeralWidth;
	static quint64 FeatureDiagonalFractions;
	static quint64 FeatureStackedFractions;
	static quint64 FeatureSubscript;
	static quint64 FeatureSuperscript;
	static quint64 FeatureOrdinals;
	static quint64 FeatureSlashedZero;
	static quint64 FeatureStyleSet01;
	static quint64 FeatureStyleSet02;
	static quint64 FeatureStyleSet03;
	static quint64 FeatureStyleSet04;
	static quint64 FeatureStyleSet05;
	static quint64 FeatureStyleSet06;
	static quint64 FeatureStyleSet07;
	static quint64 FeatureStyleSet08;
	static quint64 FeatureStyleSet09;
	static quint64 FeatureStyleSet10;
	static quint64 FeatureStyleSet11;
	static quint64 FeatureStyleSet12;
	static quint64 FeatureStyleSet13;
	static quint64 FeatureStyleSet14;
	static quint64 FeatureStyleSet15;
	static quint64 FeatureStyleSet16;
	static quint64 FeatureStyleSet17;
	static quint64 FeatureStyleSet18;
	static quint64 FeatureStyleSet19;
	static quint64 FeatureStyleSet20;

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
