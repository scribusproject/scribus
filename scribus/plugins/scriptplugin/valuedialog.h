/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef VALUEDIALOG_H
#define VALUEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

/** Request for response from user - lightweight modal dialog 
09/24/2004 petr vanek
*/
class ValueDialog : public QDialog
{
	Q_OBJECT

public:
	ValueDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~ValueDialog();

	QLabel* dialogLabel;
	QLineEdit* valueEdit;
	QPushButton* okButton;

public slots:
	virtual void okButton_pressed();

protected:
	QGridLayout* ScripterValueLayout;
	QVBoxLayout* layout3;
	QVBoxLayout* layout2;
	QHBoxLayout* layout1;

protected slots:
	virtual void languageChange();

};

#endif // VALUEDIALOG_H
