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
		NewMarginWidget(QWidget* parent=0);
		~NewMarginWidget();

		void setup(const MarginStruct& margs, int layoutType, int unitIndex=0, bool showPreset=true, bool showPrinterMargins=true);
		/*! \brief Setup the labels by facing pages option */
		void setFacingPages(bool facing, int pagetype = 0);
		/*! \brief Setup the spinboxes properties (min/max value etc.) by width */
		void setPageWidth(double);
		/*! \brief Setup the spinboxes properties (min/max value etc.) by height */
		void setPageHeight(double);
		/*! \brief Set the page size for margin getting from cups */
		void setPageSize(const QString&);
		void setNewUnitIndex(int);
		void setNewValues(const MarginStruct& margs);
		/*! \brief Setup the presetCombo without changing the margin values, only used by tabdocument */
		void setMarginPreset(int p);
		const MarginStruct & margins() const { return marginData; };

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

		MarginStruct marginData;
		MarginStruct savedMarginData;
		int savedPresetItem;
		bool facingPages;
		int m_unitIndex;
		double m_unitRatio;
		QString m_pageSize;
		double pageWidth;
		double pageHeight;
		int pageType;
		bool usingPreset;
		bool usingPrinterMargins;
};

#endif // NEWMARGINWIDGET_H



