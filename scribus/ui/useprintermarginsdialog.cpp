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

#include "useprintermarginsdialog.h"

#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

#include "util_printer.h"



// This class implements only the non-GUI parts of the
// UsePrinterMargins dialog. Please use Qt Designer on
// useprintermarginsdialog.ui if you need to modify the layout,
// widget properties, etc.

UsePrinterMarginsDialog::UsePrinterMarginsDialog( QWidget* parent, const QString& pageSize, double unitRatio, const QString& suffix )
	: QDialog(parent)
{
	setupUi(this);
	m_ptsTopMargin=m_ptsBottomMargin=m_ptsLeftMargin=m_ptsRightMargin=0.0;
	m_pageSize=pageSize;
	m_unitRatio=unitRatio;
	m_suffix=suffix;
	QStringList printerNames=PrinterUtil::getPrinterNames();
	printerComboBox->clear();
	if (printerNames.count()>0 && !pageSize.isEmpty())
	{
		printerComboBox->insertItems(0, printerNames);
		getPrinterMarginValues(printerNames.first());
		marginsGroupBox->setTitle( tr("Minimum Margins for Page Size %1").arg(pageSize));
		connect(printerComboBox, SIGNAL(activated(const QString&)), this, SLOT(getPrinterMarginValues(const QString&)));
	}
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

UsePrinterMarginsDialog::~UsePrinterMarginsDialog()
{
}

void UsePrinterMarginsDialog::getNewPrinterMargins(double &topMargin,double &bottomMargin,double &leftMargin,double &rightMargin)
{
	topMargin=m_ptsTopMargin;
	bottomMargin=m_ptsBottomMargin;
	leftMargin=m_ptsLeftMargin;
	rightMargin=m_ptsRightMargin;
}

void UsePrinterMarginsDialog::getPrinterMarginValues(const QString& printerName)
{
	m_ptsTopMargin=m_ptsBottomMargin=m_ptsLeftMargin=m_ptsRightMargin=0.0;
	bool foundSize=PrinterUtil::getPrinterMarginValues(printerName, m_pageSize, m_ptsTopMargin, m_ptsBottomMargin, m_ptsLeftMargin, m_ptsRightMargin);

	topLineEdit->setEnabled(foundSize);
	bottomLineEdit->setEnabled(foundSize);
	leftLineEdit->setEnabled(foundSize);
	rightLineEdit->setEnabled(foundSize);
	okButton->setEnabled(foundSize);
	
	topLineEdit->setText(QString::number(m_ptsTopMargin * m_unitRatio,'g',4)+m_suffix);
	bottomLineEdit->setText(QString::number(m_ptsBottomMargin * m_unitRatio,'g',4)+m_suffix);
	leftLineEdit->setText(QString::number(m_ptsLeftMargin * m_unitRatio,'g',4)+m_suffix);
	rightLineEdit->setText(QString::number(m_ptsRightMargin * m_unitRatio,'g',4)+m_suffix);	
}
