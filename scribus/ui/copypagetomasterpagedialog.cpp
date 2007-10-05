/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCheckBox>
#include <QLineEdit>

#include "copypagetomasterpagedialog.h"

CopyPageToMasterPageDialog::CopyPageToMasterPageDialog(int existingMasterNamesCount, QStringList& pageLocations, int currentLocation, QWidget *parent)
 : QDialog(parent)
{
	setupUi(this);
	
	nameLineEdit->setText( tr("New Master Page %1").arg(existingMasterNamesCount));
	if (currentLocation==-1)
		pagePositionComboBox->hide();
	else
	{
		pagePositionComboBox->addItems(pageLocations);
		pagePositionComboBox->setCurrentIndex(currentLocation);
	}
}


CopyPageToMasterPageDialog::~CopyPageToMasterPageDialog()
{
}

void CopyPageToMasterPageDialog::values(QString &pageName, bool &copyAppliedMaster, int pageLocation) const
{
	pageName=nameLineEdit->text();
	copyAppliedMaster=(copyMasterItemsCheckBox->checkState()==Qt::Checked);
	pageLocation=pagePositionComboBox->currentIndex();
}
