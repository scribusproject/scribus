/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pdfimportoptions.h"
#include "ui_pdfimportoptions.h"
#include "importpdf.h"
#include "ui/createrange.h"
#include "usertaskstructs.h"
#include "util_icon.h"

PdfImportOptions::PdfImportOptions(QWidget *parent) : QDialog(parent), ui(new Ui::PdfImportOptions)
{
	ui->setupUi(this);
	ui->pageSelectButton->setIcon(QIcon(loadIcon("ellipsis.png")));
	m_plugin = NULL;
	m_maxPage = 0;
	m_resized = false;
}

PdfImportOptions::~PdfImportOptions()
{
	delete ui;
}

void PdfImportOptions::resizeEvent(QResizeEvent *e)
{
	m_resized = true;
	QDialog::resizeEvent(e);
}

void PdfImportOptions::paintEvent(QPaintEvent *e)
{
	if (m_resized)
		updatePreview(ui->pgSelect->getCurrentPage());
	m_resized = false;
	QDialog::paintEvent(e);
}

QString PdfImportOptions::getPagesString()
{
	if (ui->allPages->isChecked())
		return "*";
	if (ui->singlePage->isChecked())
		return QString("%1").arg(ui->spinBox->value());
	return ui->pageRangeString->text();
}

int PdfImportOptions::getCropBox()
{
	int ret = 0;
	if (ui->cropGroup->isChecked())
		ret = ui->cropBox->currentIndex();
	return ret;
}

void PdfImportOptions::setUpOptions(QString fileName, int actPage, int numPages, bool interact, bool cropPossible, PdfPlug* plug)
{
	m_plugin = plug;
	ui->fileLabel->setText(fileName);
	ui->spinBox->setMaximum(numPages);
	ui->spinBox->setMinimum(actPage);
	ui->spinBox->setValue(actPage);
	ui->cropGroup->setVisible(cropPossible);
	ui->cropGroup->setChecked(cropPossible);
	if (interact)
	{
		ui->allPages->setChecked(false);
		ui->selectedPages->setChecked(false);
		ui->allPages->setEnabled(false);
		ui->selectedPages->setEnabled(false);
		ui->singlePage->setChecked(true);
		ui->spinBox->setEnabled(true);
	}
	else
		ui->allPages->setChecked(true);
	ui->pageRangeString->setText("");
	ui->pgSelect->setMaximum(numPages);
	ui->pgSelect->GotoPg(actPage - 1);
	m_maxPage = numPages;
	updatePreview(actPage);
	connect(ui->pgSelect, SIGNAL(GotoPage(int)), this, SLOT(updatePreview(int)));
	connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBox(int)));
	connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePreview(int)));
	connect(ui->pageSelectButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
	connect(ui->cropGroup, SIGNAL(clicked()), this, SLOT(updateFromCrop()));
	connect(ui->cropBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFromCrop()));
}

void PdfImportOptions::updateFromCrop()
{
	updatePreview(ui->pgSelect->getCurrentPage());
}

void PdfImportOptions::updateFromSpinBox(int pg)
{
	ui->pgSelect->GotoPg(pg - 1);
}

void PdfImportOptions::updatePreview(int pg)
{
	if (m_plugin)
	{
		int cb = 0;
		if (ui->cropGroup->isChecked())
			cb =  ui->cropBox->currentIndex();
		QImage img = m_plugin->readPreview(pg, ui->previewWidget->width(), ui->previewWidget->height(), cb);
		ui->previewWidget->setPixmap(QPixmap::fromImage(img));
		disconnect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBox(int)));
		disconnect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePreview(int)));
		ui->spinBox->setValue(pg);
		connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBox(int)));
		connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePreview(int)));
	}
}

void PdfImportOptions::createPageNumberRange()
{
	CreateRange cr(ui->pageRangeString->text(), m_maxPage, this);
	if (cr.exec())
	{
		CreateRangeData crData;
		cr.getCreateRangeData(crData);
		ui->pageRangeString->setText(crData.pageRange);
	}
}
