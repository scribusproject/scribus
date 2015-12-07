/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "csvdia.h"

#include "scribusapi.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QStringList>

#include "iconmanager.h"

CsvDialog::CsvDialog() : QDialog(0)
{
	setModal(true);
	setWindowTitle( tr("CSV Importer Options"));
	setWindowIcon(QIcon(IconManager::instance()->loadIcon ( "AppIcon.png" )));

	QBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	QBoxLayout* flayout = new QHBoxLayout;
	flayout->setMargin(5);
	flayout->setSpacing(5);
	QLabel* fdlabel = new QLabel( tr("Field delimiter:"), this);
	fdlabel->setMinimumWidth(120);
	flayout->addWidget(fdlabel,1);
	m_fdelimCombo = new QComboBox(this);
	m_fdelimCombo->setEditable(false);
	QStringList fdList(",");
	fdList << ";";
	fdList << tr("(TAB)");
	m_fdelimCombo->addItems(fdList);
	m_fdelimCombo->setMinimumWidth(120);
	flayout->addWidget(m_fdelimCombo,5);
	layout->addLayout(flayout);

	QBoxLayout* vlayout = new QHBoxLayout;
	vlayout->setMargin(5);
	vlayout->setSpacing(5);
	QLabel* vdlabel = new QLabel( tr("Value delimiter:"), this);
	vdlabel->setMinimumWidth(120);
	vlayout->addWidget(vdlabel,1);
	m_vdelimCombo = new QComboBox(this);
	m_vdelimCombo->setEditable(false);
	QStringList vdList("\"");
	vdList << "'" << tr("None", "delimiter");
	m_vdelimCombo->addItems(vdList);
	m_vdelimCombo->setMinimumWidth(120);
	vlayout->addWidget(m_vdelimCombo,5);
	layout->addLayout(vlayout);

	QBoxLayout* hlayout = new QHBoxLayout;
	hlayout->setMargin(5);
	hlayout->setSpacing(5);
	m_headerCheck = new QCheckBox( tr("First row is a header"), this);
	hlayout->addWidget(m_headerCheck);
	layout->addLayout(hlayout);

	QBoxLayout* blayout = new QHBoxLayout;
	blayout->setMargin(5);
	blayout->setSpacing(5);
	blayout->addStretch(10);
	m_okButton = new QPushButton( tr("OK"), this);
	blayout->addWidget(m_okButton);

	m_cancelButton = new QPushButton( tr("Cancel"), this);
	blayout->addWidget(m_cancelButton);
	layout->addLayout(blayout);

	connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QString CsvDialog::getFDelim()
{
	if (m_fdelimCombo->currentText() == tr("(TAB)"))
		return "\t";
	return m_fdelimCombo->currentText();
}

QString CsvDialog::getVDelim()
{
	return m_vdelimCombo->currentText();
}

bool CsvDialog::hasHeader()
{
	return m_headerCheck->isChecked();
}

bool CsvDialog::useVDelim()
{
	return m_vdelimCombo->currentIndex() != 2;
}

CsvDialog::~CsvDialog()
{

}
