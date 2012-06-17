/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QAbstractButton>
#include <QCheckBox>
#include <QLineEdit>
#include "commonstrings.h"
#include "copypagetomasterpagedialog.h"

CopyPageToMasterPageDialog::CopyPageToMasterPageDialog(int existingMasterNamesCount, QStringList& pageLocations, int currentLocation, QWidget *parent)
 : QDialog(parent)
{
	setupUi(this);

	QList<QAbstractButton *> buttonList = buttonBox->buttons();
	foreach (QAbstractButton* b, buttonList)
	{
		if (buttonBox->buttonRole(b)==QDialogButtonBox::AcceptRole)
			b->setText(CommonStrings::tr_OK);
		else
		if (buttonBox->buttonRole(b)==QDialogButtonBox::RejectRole)
			b->setText(CommonStrings::tr_Cancel);
	}
	
	nameLineEdit->setText( tr("New Master Page %1").arg(existingMasterNamesCount));
	if (currentLocation==-1)
		pagePositionComboBox->hide();
	else
	{
		foreach (QString untranslatedPageName, pageLocations)
			pagePositionComboBox->addItem(CommonStrings::translatePageSetLocString(untranslatedPageName));
		pagePositionComboBox->setCurrentIndex(currentLocation);
	}
}


CopyPageToMasterPageDialog::~CopyPageToMasterPageDialog()
{
}

void CopyPageToMasterPageDialog::values(QString &pageName, bool &copyAppliedMaster, int& pageLocation) const
{
	pageName=nameLineEdit->text();
	copyAppliedMaster=(copyMasterItemsCheckBox->checkState()==Qt::Checked);
	pageLocation=pagePositionComboBox->currentIndex();
}
