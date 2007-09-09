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

#include "util_icon.h"

CsvDialog::CsvDialog() : QDialog(0)
{
	setModal(true);
	setWindowTitle( tr("CSV Importer Options"));
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));

	QBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	QBoxLayout* flayout = new QHBoxLayout;
	flayout->setMargin(5);
	flayout->setSpacing(5);
	QLabel* fdlabel = new QLabel( tr("Field delimiter:"), this);
	fdlabel->setMinimumWidth(120);
	flayout->addWidget(fdlabel,1);
	fdelimCombo = new QComboBox(this);
	fdelimCombo->setEditable(false);
	QStringList fdList(",");
	fdList << ";";
	fdList << tr("(TAB)");
	fdelimCombo->addItems(fdList);
	fdelimCombo->setMinimumWidth(120);
	flayout->addWidget(fdelimCombo,5);
	layout->addLayout(flayout);

	QBoxLayout* vlayout = new QHBoxLayout;
	vlayout->setMargin(5);
	vlayout->setSpacing(5);
	QLabel* vdlabel = new QLabel( tr("Value delimiter:"), this);
	vdlabel->setMinimumWidth(120);
	vlayout->addWidget(vdlabel,1);
	vdelimCombo = new QComboBox(this);
	vdelimCombo->setEditable(false);
	QStringList vdList("\"");
	vdList << "'" << tr("None", "delimiter");
	vdelimCombo->addItems(vdList);
	vdelimCombo->setMinimumWidth(120);
	vlayout->addWidget(vdelimCombo,5);
	layout->addLayout(vlayout);

	QBoxLayout* hlayout = new QHBoxLayout;
	hlayout->setMargin(5);
	hlayout->setSpacing(5);
	headerCheck = new QCheckBox( tr("First row is a header"), this);
	hlayout->addWidget(headerCheck);
	layout->addLayout(hlayout);

	QBoxLayout* blayout = new QHBoxLayout;
	blayout->setMargin(5);
	blayout->setSpacing(5);
	blayout->addStretch(10);
	okButton = new QPushButton( tr("OK"), this);
	blayout->addWidget(okButton);

	cancelButton = new QPushButton( tr("Cancel"), this);
	blayout->addWidget(cancelButton);
	layout->addLayout(blayout);

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QString CsvDialog::getFDelim()
{
	if (fdelimCombo->currentText() == tr("(TAB)"))
		return "\t";
	return fdelimCombo->currentText();
}

QString CsvDialog::getVDelim()
{
	return vdelimCombo->currentText();
}

bool CsvDialog::hasHeader()
{
	return headerCheck->isChecked();
}

bool CsvDialog::useVDelim()
{
	return vdelimCombo->currentIndex() != 2;
}

CsvDialog::~CsvDialog()
{

}
