/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qfiledialog.h>

void ExportForm::OutputDirectoryButton_pressed()
{
	QString d = QFileDialog::getExistingDirectory(
			QDir::currentDirPath(),
			this,
			"d",
			tr("Choose a Export Directory"),
			true);
	if (d.length()>0)
		OutputDirectory->setText(d);
}

void ExportForm::OkButton_pressed()
{
	bitmapType = BitmapType->currentText();
	writeConfig();
	accept();
}

void ExportForm::CancelButton_pressed()
{
	reject();
}

void ExportForm::IntervalPagesRadio_stateChanged(int)
{
	FromBox->setEnabled(TRUE);
	ToBox->setEnabled(TRUE);
}

void ExportForm::AllPagesRadio_stateChanged(int)
{
	FromBox->setEnabled(FALSE);
	ToBox->setEnabled(FALSE);
}

void ExportForm::OnePageRadio_stateChanged(int)
{
	FromBox->setEnabled(FALSE);
	ToBox->setEnabled(FALSE);
}

void ExportForm::ChFrom()
{
	disconnect(ToBox, SIGNAL(valueChanged(int)), this, SLOT(ChTo()));
	if (FromBox->value() > ToBox->value())
		ToBox->setValue(FromBox->value());
	connect(ToBox, SIGNAL(valueChanged(int)), this, SLOT(ChTo()));
}

void ExportForm::ChTo()
{
	disconnect(FromBox, SIGNAL(valueChanged(int)), this, SLOT(ChFrom()));
	if (ToBox->value() < FromBox->value())
		FromBox->setValue(ToBox->value());
	connect(FromBox, SIGNAL(valueChanged(int)), this, SLOT(ChFrom()));
}
