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
		m_pdfVersionModel = nullptr;
	}
}

PDFVersion PdfVersionCombo::version() const
{
	int curIndex = currentIndex();

	if (curIndex == PdfVersionModel::ItemPDF_13)
		return PDFVersion::PDF_13;
	if (curIndex == PdfVersionModel::ItemPDF_14)
		return PDFVersion::PDF_14;
	if (curIndex == PdfVersionModel::ItemPDF_15)
		return PDFVersion::PDF_15;
	if (curIndex == PdfVersionModel::ItemPDF_16)
		return PDFVersion::PDF_16;
	if (curIndex == PdfVersionModel::ItemPDFX_1a)
		return PDFVersion::PDF_X1a;
	if (curIndex == PdfVersionModel::ItemPDFX_3)
		return PDFVersion::PDF_X3;
	if (curIndex == PdfVersionModel::ItemPDFX_4)
		return PDFVersion::PDF_X4;
	return PDFVersion::PDF_14;
}

bool PdfVersionCombo::versionIs(const PDFVersion& version) const
{
	int curIndex = currentIndex();

	if (version == PDFVersion::PDF_13)
		return (curIndex == PdfVersionModel::ItemPDF_13);
	if (version == PDFVersion::PDF_14)
		return (curIndex == PdfVersionModel::ItemPDF_14);
	if (version == PDFVersion::PDF_15)
		return (curIndex == PdfVersionModel::ItemPDF_15);
	if (version == PDFVersion::PDF_16)
		return (curIndex == PdfVersionModel::ItemPDF_16);
	if (version == PDFVersion::PDF_X1a)
		return (curIndex == PdfVersionModel::ItemPDFX_1a);
	if (version == PDFVersion::PDF_X3)
		return (curIndex == PdfVersionModel::ItemPDFX_3);
	if (version == PDFVersion::PDF_X4)
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

void PdfVersionCombo::setVersion(const PDFVersion& version)
{
	if ((version == PDFVersion::PDF_X1a) && !m_isPdfXEnabled)
	{
		setCurrentIndex(PdfVersionModel::ItemPDF_13);
		return;
	}
	if ((version == PDFVersion::PDF_X3) && !m_isPdfXEnabled)
	{
		setCurrentIndex(PdfVersionModel::ItemPDF_13);
		return;
	}
	if ((version == PDFVersion::PDF_X4) && !m_isPdfXEnabled)
	{
		setCurrentIndex(PdfVersionModel::ItemPDF_15);
		return;
	}
	
	if (version == PDFVersion::PDF_13)
		setCurrentIndex(PdfVersionModel::ItemPDF_13);
	else if (version == PDFVersion::PDF_14)
		setCurrentIndex(PdfVersionModel::ItemPDF_14);
	else if  (version == PDFVersion::PDF_15)
		setCurrentIndex(PdfVersionModel::ItemPDF_15);
	else if  (version == PDFVersion::PDF_16)
		setCurrentIndex(PdfVersionModel::ItemPDF_16);
	else if  (version == PDFVersion::PDF_X1a)
		setCurrentIndex(PdfVersionModel::ItemPDFX_1a);
	else if  (version == PDFVersion::PDF_X3)
		setCurrentIndex(PdfVersionModel::ItemPDFX_3);
	else if  (version == PDFVersion::PDF_X4)
		setCurrentIndex(PdfVersionModel::ItemPDFX_4);
}

void PdfVersionCombo::setPDFXEnabled(bool enabled)
{
	if (m_isPdfXEnabled == enabled)
		return;
	m_isPdfXEnabled = enabled;
	
	PDFVersion oldVersion = version();
	if (!enabled)
	{
		if ((oldVersion == PDFVersion::PDF_X1a) && !enabled) 
			setCurrentIndex((int) PdfVersionModel::ItemPDF_13);
		if ((oldVersion == PDFVersion::PDF_X3) && !enabled) 
			setCurrentIndex((int) PdfVersionModel::ItemPDF_13);
		if ((oldVersion == PDFVersion::PDF_X4) && !enabled) 
			setCurrentIndex((int) PdfVersionModel::ItemPDF_15);
	}
	m_pdfVersionModel->setPdfXEnabled(enabled);
}
