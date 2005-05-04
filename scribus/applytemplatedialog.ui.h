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

enum {
	CurrentPage,
	EvenPages,
	OddPages,
	AllPages
};

void ApplyMasterPageDialog::setup(ScribusDoc *view, QString Nam)
{
	QString na = Nam == "Normal" ? tr("Normal") : Nam, in;
	int cc = 0;
	QMap<QString,int>::Iterator it;
	for (it = view->MasterNames.begin(); it != view->MasterNames.end(); ++it)
	{
		in = it.key() == "Normal" ? tr("Normal") : it.key();
		masterPageComboBox->insertItem(in);
		if (in == na)
			masterPageComboBox->setCurrentItem(cc);
		++cc;
	}
	if (view->Pages.count() < 2)
		evenPagesRadioButton->setEnabled(false);
	fromPageSpinBox->setMaxValue(view->Pages.count());
	fromPageSpinBox->setValue(view->currentPage->PageNr+1);
	toPageSpinBox->setMaxValue(view->Pages.count());
	toPageSpinBox->setValue(view->Pages.count());
}


QString ApplyMasterPageDialog::getMasterPageName()
{
	return masterPageComboBox->currentText();
}


int ApplyMasterPageDialog::getPageSelection()
{
	if (currentPageRadioButton->isChecked())
		return CurrentPage;
	else if (evenPagesRadioButton->isChecked())
		return EvenPages;
	else if (oddPagesRadioButton->isChecked())
		return OddPages;
	else 
		return AllPages;
}

void ApplyMasterPageDialog::checkRangeFrom()
{
	disconnect(fromPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFrom()));
	disconnect(toPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeTo()));
	if (fromPageSpinBox->value() > toPageSpinBox->value())
		toPageSpinBox->setValue(fromPageSpinBox->value());
	connect(fromPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFrom()));
	connect(toPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeTo()));
}

void ApplyMasterPageDialog::checkRangeTo()
{
	disconnect(fromPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFrom()));
	disconnect(toPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeTo()));
	if (toPageSpinBox->value() < fromPageSpinBox->value())
		fromPageSpinBox->setValue(toPageSpinBox->value());
	connect(fromPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFrom()));
	connect(toPageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRangeTo()));
}


void ApplyMasterPageDialog::enableRange( bool enabled )
{
	fromPageSpinBox->setEnabled(enabled);
	toPageSpinBox->setEnabled(enabled);
}

void ApplyMasterPageDialog::rangeSelectable()
{
	useRangeCheckBox->setEnabled(true);
	enableRange(useRangeCheckBox->isChecked());
}

void ApplyMasterPageDialog::singleSelectable()
{
	useRangeCheckBox->setEnabled(false);
	fromPageSpinBox->setEnabled(false);
	toPageSpinBox->setEnabled(false);
}


bool ApplyMasterPageDialog::usingRange()
{
	return useRangeCheckBox->isChecked();
}


int ApplyMasterPageDialog::getFromPage()
{
	if (useRangeCheckBox->isChecked())
		return fromPageSpinBox->value();
	return -1;
}


int ApplyMasterPageDialog::getToPage()
{
	if (useRangeCheckBox->isChecked())
		return toPageSpinBox->value();
	return -1;
}
