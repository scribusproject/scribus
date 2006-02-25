/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "csvdia.h"
#include "csvdia.moc"

#include "scribusapi.h"

extern QPixmap SCRIBUS_API loadIcon(QString nam);

CsvDialog::CsvDialog() : QDialog(0, "csvdia", true, 0)
{
	setCaption(tr("CSV Importer Options"));
	setIcon(loadIcon("AppIcon.png"));

	QBoxLayout* layout = new QVBoxLayout(this);

	QBoxLayout* flayout = new QHBoxLayout(0, 5, 5, "flayout");
	QLabel* fdlabel = new QLabel(tr("Field delimiter:"), this, "fdlabel", 0);
	fdlabel->setMinimumWidth(120);
	flayout->addWidget(fdlabel,1);
	fdelimCombo = new QComboBox(true, this, "fdelimCombo");
	QStringList fdList(",");
	fdList << ";";
	fdList << tr("(TAB)");
	fdelimCombo->insertStringList(fdList);
	fdelimCombo->setMinimumWidth(120);
	flayout->addWidget(fdelimCombo,5);
	layout->addLayout(flayout);

	QBoxLayout* vlayout = new QHBoxLayout(0, 5, 5, "vlayout");
	QLabel* vdlabel = new QLabel(tr("Value delimiter:"), this, "fdlabel", 0);
	vdlabel->setMinimumWidth(120);
	vlayout->addWidget(vdlabel,1);
	vdelimCombo = new QComboBox(true, this, "vdelimCombo");
	QStringList vdList("\"");
	vdList << "'" << tr("None", "delimiter");
	vdelimCombo->insertStringList(vdList);
	vdelimCombo->setMinimumWidth(120);
	vlayout->addWidget(vdelimCombo,5);
	layout->addLayout(vlayout);

	QBoxLayout* hlayout = new QHBoxLayout(0, 5, 5, "hlayout");
	headerCheck = new QCheckBox(tr("First row is a header"), this, "headerCheck");
	hlayout->addWidget(headerCheck);
	layout->addLayout(hlayout);

	QBoxLayout* blayout = new QHBoxLayout(0, 5, 5, "blayout");
	blayout->addStretch(10);
	okButton = new QPushButton(tr("OK"), this, "okButton");
	blayout->addWidget(okButton);

	cancelButton = new QPushButton(tr("Cancel"), this, "cancelButton");
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
	return vdelimCombo->currentItem() != 2;
}

CsvDialog::~CsvDialog()
{

}
