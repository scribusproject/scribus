/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CXFIMPORTDIALOG_H
#define CXFIMPORTDIALOG_H

#include <QDialog>
#include <QList>

#include "colormgmt/sccolormgmtstructs.h"
#include "ui_cxfimportdialogbase.h"

class CxfImportDialog : public QDialog, Ui::CxfImportDialogBase
{
	Q_OBJECT

public:
	CxfImportDialog(QWidget* parent = 0);

	QList<eColorSpaceType> priorities() const;

	void setDefaultPriorities();
	void setPriorities(const QList<eColorSpaceType>& priorities);

protected:
	void insertColorspaceLab();
	void insertColorspaceRGB();
	void insertColorspaceCMYK();

protected slots:
	void moveUp();
	void moveDown();
	void select(QListWidgetItem* item);
};

#endif // CXFIMPORTDIALOG_H
