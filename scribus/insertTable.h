/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef INSERTTABLE_H
#define INSERTTABLE_H

#include <QDialog>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "scribusapi.h"

class SCRIBUS_API InsertTable : public QDialog
{
	Q_OBJECT

public:
	InsertTable( QWidget* parent, int maxRow, int maxCol );
	~InsertTable() {};

	QSpinBox* Cols;
	QSpinBox* Rows;

protected:
	QVBoxLayout* InsertTableLayout;
	QGridLayout* layout2;
	QHBoxLayout* layout1;
	QLabel* Text1;
	QLabel* Text2;
	QPushButton* okButton;
	QPushButton* cancelButton;
};

#endif // INSERTTABLE_H
