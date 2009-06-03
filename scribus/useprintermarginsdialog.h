/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          useprintermarginsdialog.cpp  -  description
                             -------------------
    copyright            : (C) 2006 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

#ifndef USEPRINTERMARGINSDIALOG_H
#define USEPRINTERMARGINSDIALOG_H

#include "ui_useprintermarginsdialog.h"

#include "scribusapi.h"

class SCRIBUS_API UsePrinterMarginsDialog : public QDialog, Ui::UsePrinterMarginsDialog
{
	Q_OBJECT

	public:
		UsePrinterMarginsDialog(QWidget* parent, const QString& pageSize, double unitRatio, const QString& suffix);
		~UsePrinterMarginsDialog();
		/**
		 * Return the new printer sourced margins in PostScript points (72dpi).
		 * @param topMargin 
		 * @param bottomMargin 
		 * @param leftMargin 
		 * @param rightMargin 
		 */
		void getNewPrinterMargins(double &topMargin,double &bottomMargin,double &leftMargin,double &rightMargin);
	protected slots:
		void getPrinterMarginValues(const QString& printerName);
		
	protected:
		QString m_pageSize;
		double m_unitRatio;
		QString m_suffix;
		double m_ptsTopMargin,m_ptsBottomMargin,m_ptsLeftMargin,m_ptsRightMargin;
};

#endif
