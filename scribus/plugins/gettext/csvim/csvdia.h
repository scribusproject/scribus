#ifndef CSVDIA_H
#define CSVDIA_H

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qstringlist.h>

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
