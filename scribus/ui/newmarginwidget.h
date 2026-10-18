/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef NEWMARGINWIDGET_H
#define NEWMARGINWIDGET_H

#include "ui_newmarginwidgetbase.h"
#include "scribusapi.h"

class SCRIBUS_API NewMarginWidget : public QWidget, Ui::NewMarginWidget
{
	Q_OBJECT

public:
	NewMarginWidget(QWidget* parent = nullptr);
	~NewMarginWidget() = default;

	enum SetupFlags
	{
		DistanceWidgetFlags = 0,
		ShowPreset          = 1,
		ShowPrinterMargins  = 2,
		MarginWidgetFlags   = 3,
		BleedWidgetFlags    = 4
	};

	void setup(const MarginStruct& margs, int layoutType, int unitIndex, int flags = MarginWidgetFlags);
	/*! \brief Setup the labels by facing pages option */
	void setFacingPages(bool facing, int pageType = 0);
	/*! \brief Setup the spinboxes properties (min/max value etc.) by width */
	void setPageWidth(double width);
	/*! \brief Setup the spinboxes properties (min/max value etc.) by height */
	void setPageHeight(double height);
	void setNewUnit(int unitIndex);
	void setNewValues(const MarginStruct& margs);
	/*! \brief Setup the presetCombo without changing the margin values */
	void setMarginPreset(int p);

	int marginPreset() const { return m_savedPresetItem; };

	const MarginStruct& margins() const { return m_marginData; };

public slots:
	void languageChange();
	void iconSetChange();
	void toggleLabelVisibility(bool v);

protected slots:

	void onSpinBoxChanged();
	void onLinkMarginsClicked();
	void onPrinterMarginsClicked();
	void onDefaultMarginsClicked();
	void setPreset();

protected:

	bool m_facingPages {false};
	double m_pageHeight {0.0};
	double m_pageWidth {0.0};
	double m_unitRatio {1.0};
	int m_flags {MarginWidgetFlags};
	int m_pageType {0};
	int m_savedPresetItem {PresetLayout::none};
	int m_unitIndex {0};

	MarginStruct m_marginData;
	MarginStruct m_savedMarginData;

	MarginStruct calculateMarginsForPreset(int preset, double w, double h, double refLeft) const;
	MarginStruct sanitizeMargins(const MarginStruct& margins, double w, double h) const;

	void setInternalState(double width, double height, const MarginStruct& margins, int presetIndex);
	void updateUIFromState();
	void updateSpinBoxLimits();
	void blockSpinBoxSignals(bool block);

signals:
	void valuesChanged(MarginStruct);
};

#endif // NEWMARGINWIDGET_H
