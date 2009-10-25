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

		void setup(const MarginStruct& margs, int unitIndex=0, bool showPreset=true);
		void setPageWidth(double);
		void setPageHeight(double);
		void setPageSize(const QString&);
		void setNewUnitIndex(int);
		void setTop();
		void setBottom();
		void setLeft();
		void setRight();
		void setPreset();
		const MarginStruct & margins() const;
		void slotLinkMargins();

	public slots:
		void languageChange();

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
};

#endif // NEWMARGINWIDGET_H



