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
#include "scribusstructs.h"

class SCRIBUS_API NewMarginWidget : public QWidget, Ui::NewMarginWidget
{
	Q_OBJECT

	public:
		NewMarginWidget(QWidget* parent = nullptr);
		~NewMarginWidget() = default;

		enum SetupFlags
		{
			ShowPreset         = 1,
			ShowPrinterMargins = 2,
			MarginWidgetFlags  = 3,
			BleedWidgetFlags   = 4,
		};

		void setup(const MarginStruct& margs, int layoutType, int unitIndex, int flags = MarginWidgetFlags);
		/*! \brief Setup the labels by facing pages option */
		void setFacingPages(bool facing, int pageType = 0);
		/*! \brief Setup the spinboxes properties (min/max value etc.) by width */
		void setPageWidth(double);
		/*! \brief Setup the spinboxes properties (min/max value etc.) by height */
		void setPageHeight(double);
		/*! \brief Set the page size for margin getting from cups */
		void setPageSize(const QString&);
		void setNewUnit(int unitIndex);
		void setNewValues(const MarginStruct& margs);
		/*! \brief Setup the presetCombo without changing the margin values, only used by tabdocument */
		void setMarginPreset(int p);
		const MarginStruct & margins() const { return m_marginData; };

	public slots:
		void languageChange();
		void setTop();
		void setBottom();
		void setLeft();
		void setRight();
		void slotLinkMargins();
		void setPreset();

	protected slots:
		void setMarginsToPrinterMargins();

	protected:
		void updateMarginSpinValues();

		MarginStruct m_marginData;
		MarginStruct m_savedMarginData;
		QString m_pageSize;
		bool   m_facingPages {false};
		double m_pageHeight {0.0};
		double m_pageWidth {0.0};
		double m_unitRatio {1.0};
		int    m_flags {MarginWidgetFlags};
		int    m_pageType {0};
		int    m_savedPresetItem {PresetLayout::none};
		int    m_unitIndex {0};
};

#endif // NEWMARGINWIDGET_H



