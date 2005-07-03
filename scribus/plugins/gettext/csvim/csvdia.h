/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
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
