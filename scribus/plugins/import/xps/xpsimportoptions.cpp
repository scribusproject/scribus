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
#include "iconmanager.h"

XpsImportOptions::XpsImportOptions(QWidget *parent) : QDialog(parent), m_ui(new Ui::XpsImportOptions)
{
	m_ui->setupUi(this);
	m_ui->pageSelectButton->setIcon(IconManager::instance()->loadIcon("ellipsis.png"));
	m_maxPage = 0;
}

XpsImportOptions::~XpsImportOptions()
{
	delete m_ui;
}

QString XpsImportOptions::getPagesString()
{
	if (m_ui->allPages->isChecked())
		return "*";
	if (m_ui->singlePage->isChecked())
		return QString("%1").arg(m_ui->spinBox->value());
	return m_ui->pageRangeString->text();
}

void XpsImportOptions::setUpOptions(QString fileName, int actPage, int numPages, bool interact)
{
	m_ui->fileLabel->setText(fileName);
	m_ui->spinBox->setMaximum(numPages);
	m_ui->spinBox->setMinimum(actPage);
	m_ui->spinBox->setValue(actPage);
	if (interact)
	{
		m_ui->allPages->setChecked(false);
		m_ui->selectedPages->setChecked(false);
		m_ui->allPages->setEnabled(false);
		m_ui->selectedPages->setEnabled(false);
		m_ui->singlePage->setChecked(true);
		m_ui->spinBox->setEnabled(true);
	}
	else
		m_ui->allPages->setChecked(true);
	m_ui->pageRangeString->setText("");
	m_maxPage = numPages;
	connect(m_ui->pageSelectButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
}

void XpsImportOptions::createPageNumberRange()
{
	CreateRange cr(m_ui->pageRangeString->text(), m_maxPage, this);
	if (cr.exec())
	{
		CreateRangeData crData;
		cr.getCreateRangeData(crData);
		m_ui->pageRangeString->setText(crData.pageRange);
	}
}
