/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pdfversioncombo.h"
#include "pdfversionmodel.h"

PdfVersionCombo::PdfVersionCombo(QWidget* parent) : QComboBox(parent)
{
	m_isPdfXEnabled = false;
	m_pdfVersionModel = new PdfVersionModel(this);
	
	setEditable(false);
	setModel(m_pdfVersionModel);
}

PdfVersionCombo::~PdfVersionCombo()
{
	if (m_pdfVersionModel)
	{
		delete m_pdfVersionModel;
		m_pdfVersionModel = 0;
	}
}

PDFOptions::PDFVersion PdfVersionCombo::version() const
{
	int curIndex = currentIndex();

	if (curIndex == PdfVersionModel::ItemPDF_13)
		return PDFOptions::PDFVersion_13;
	if (curIndex == PdfVersionModel::ItemPDF_14)
		return PDFOptions::PDFVersion_14;
	if (curIndex == PdfVersionModel::ItemPDF_15)
		return PDFOptions::PDFVersion_15;
	if (curIndex == PdfVersionModel::ItemPDFX_1a)
		return PDFOptions::PDFVersion_X1a;
	if (curIndex == PdfVersionModel::ItemPDFX_3)
		return PDFOptions::PDFVersion_X3;
	if (curIndex == PdfVersionModel::ItemPDFX_4)
		return PDFOptions::PDFVersion_X4;
	return PDFOptions::PDFVersion_14;
}

bool PdfVersionCombo::versionIs(PDFOptions::PDFVersion version) const
{
	int curIndex = currentIndex();

	if (version == PDFOptions::PDFVersion_13)
		return (curIndex == PdfVersionModel::ItemPDF_13);
	if (version == PDFOptions::PDFVersion_14)
		return (curIndex == PdfVersionModel::ItemPDF_14);
	if (version == PDFOptions::PDFVersion_15)
		return (curIndex == PdfVersionModel::ItemPDF_15);
	if (version == PDFOptions::PDFVersion_X1a)
		return (curIndex == PdfVersionModel::ItemPDFX_1a);
	if (version == PDFOptions::PDFVersion_X3)
		return (curIndex == PdfVersionModel::ItemPDFX_3);
	if (version == PDFOptions::PDFVersion_X4)
		return (curIndex == PdfVersionModel::ItemPDFX_4);
	return false;
}

bool PdfVersionCombo::versionIsPDFX() const
{
	int curIndex = currentIndex();
	if (curIndex == PdfVersionModel::ItemPDFX_1a)
		return true;
	if (curIndex == PdfVersionModel::ItemPDFX_3)
		return true;
	if (curIndex == PdfVersionModel::ItemPDFX_4)
		return true;
	return false;
}

void PdfVersionCombo::setVersion(PDFOptions::PDFVersion version)
{
	if ((version == PDFOptions::PDFVersion_X1a) && !m_isPdfXEnabled)
	{
		setCurrentIndex(PdfVersionModel::ItemPDF_13);
		return;
	}
	if ((version == PDFOptions::PDFVersion_X3) && !m_isPdfXEnabled)
	{
		setCurrentIndex(PdfVersionModel::ItemPDF_13);
		return;
	}
	if ((version == PDFOptions::PDFVersion_X4) && !m_isPdfXEnabled)
	{
		setCurrentIndex(PdfVersionModel::ItemPDF_15);
		return;
	}
	
	if (version == PDFOptions::PDFVersion_13)
		setCurrentIndex(PdfVersionModel::ItemPDF_13);
	else if (version == PDFOptions::PDFVersion_14)
		setCurrentIndex(PdfVersionModel::ItemPDF_14);
	else if  (version == PDFOptions::PDFVersion_15)
		setCurrentIndex(PdfVersionModel::ItemPDF_15);
	else if  (version == PDFOptions::PDFVersion_X1a)
		setCurrentIndex(PdfVersionModel::ItemPDFX_1a);
	else if  (version == PDFOptions::PDFVersion_X3)
		setCurrentIndex(PdfVersionModel::ItemPDFX_3);
	else if  (version == PDFOptions::PDFVersion_X4)
		setCurrentIndex(PdfVersionModel::ItemPDFX_4);;
}

void PdfVersionCombo::setPDFXEnabled(bool enabled)
{
	if (m_isPdfXEnabled == enabled)
		return;
	m_isPdfXEnabled = enabled;
	
	PDFOptions::PDFVersion oldVersion = version();
	if (!enabled)
	{
		if ((oldVersion == PDFOptions::PDFVersion_X1a) && !enabled) 
			setCurrentIndex((int) PdfVersionModel::ItemPDF_13);
		if ((oldVersion == PDFOptions::PDFVersion_X3) && !enabled) 
			setCurrentIndex((int) PdfVersionModel::ItemPDF_13);
		if ((oldVersion == PDFOptions::PDFVersion_X4) && !enabled) 
			setCurrentIndex((int) PdfVersionModel::ItemPDF_15);
	}
	m_pdfVersionModel->setPdfXEnabled(enabled);
}
