/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CSVDIA_H
#define CSVDIA_H

#include <QDialog>
class QComboBox;
class QCheckBox;
class QPushButton;
class QString;

class CsvDialog : public QDialog
{
	Q_OBJECT

public:
	CsvDialog();
	~CsvDialog();
	QString getFDelim();
	QString getVDelim();
	bool hasHeader();
	bool useVDelim();

private:
	QComboBox* fdelimCombo;
	QComboBox* vdelimCombo;
	QCheckBox* headerCheck;
	QPushButton* okButton;
	QPushButton* cancelButton;
};

#endif // CSVDIA_H
