/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef COPYPAGETOMASTERPAGEDIALOG_H
#define COPYPAGETOMASTERPAGEDIALOG_H

#include "scribusapi.h"
#include "ui_copypagetomasterpage.h"


class SCRIBUS_API CopyPageToMasterPageDialog : public QDialog, public Ui::CopyPageToMasterPageBase
{
Q_OBJECT
public:
	CopyPageToMasterPageDialog(int existingMasterNamesCount, QStringList& pageLocations, int currentLocation = -1, QWidget *parent = 0);
	~CopyPageToMasterPageDialog();
	
	void values(QString &pageName, bool &copyAppliedMaster, int& pageLocation) const;

};

#endif
