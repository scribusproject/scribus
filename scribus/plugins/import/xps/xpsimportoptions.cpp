/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "xpsimportoptions.h"
#include "ui_xpsimportoptions.h"
#include "ui/createrange.h"
#include "usertaskstructs.h"
#include "util_icon.h"

XpsImportOptions::XpsImportOptions(QWidget *parent) : QDialog(parent), ui(new Ui::XpsImportOptions)
{
	ui->setupUi(this);
	ui->pageSelectButton->setIcon(QIcon(loadIcon("ellipsis.png")));
	m_maxPage = 0;
}

XpsImportOptions::~XpsImportOptions()
{
	delete ui;
}

QString XpsImportOptions::getPagesString()
{
	if (ui->allPages->isChecked())
		return "*";
	if (ui->singlePage->isChecked())
		return QString("%1").arg(ui->spinBox->value());
	return ui->pageRangeString->text();
}

void XpsImportOptions::setUpOptions(QString fileName, int actPage, int numPages, bool interact)
{
	ui->fileLabel->setText(fileName);
	ui->spinBox->setMaximum(numPages);
	ui->spinBox->setMinimum(actPage);
	ui->spinBox->setValue(actPage);
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
	m_maxPage = numPages;
	connect(ui->pageSelectButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
}

void XpsImportOptions::createPageNumberRange()
{
	CreateRange cr(ui->pageRangeString->text(), m_maxPage, this);
	if (cr.exec())
	{
		CreateRangeData crData;
		cr.getCreateRangeData(crData);
		ui->pageRangeString->setText(crData.pageRange);
	}
}
