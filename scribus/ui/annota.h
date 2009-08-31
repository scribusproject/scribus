/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ANNOTA_H
#define ANNOTA_H

#include <QDialog>
class QHBoxLayout;
class QGridLayout;
class QVBoxLayout;
class QFrame;
class QLabel;
class QComboBox;
class QStackedWidget;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QCheckBox;

#include "scribusapi.h"

class PageItem;
class Navigator;
class ScribusView;

class SCRIBUS_API Annota : public QDialog
{
	Q_OBJECT

public:
	Annota(QWidget* parent, PageItem *it, int Seite, int b, int h, ScribusView* vie);
	~Annota() {};
	QStackedWidget* Fram;
	QLabel* TextLabel1;
	QComboBox* ComboBox1;
	QGroupBox* GroupBox1;
	QLabel* TextLabel3;
	QLabel* TextLabel4;
	QLabel* TextLabel5;
	QLineEdit* Destfile;
	QPushButton* ChFile;
	QCheckBox* useAbsolute;
	Navigator* Pg;
	QSpinBox* SpinBox1;
	QSpinBox* SpinBox2;
	QSpinBox* SpinBox3;
	QFrame* Frame9;
	QPushButton* PushButton1;
	QPushButton* PushButton2;
	PageItem* item;
	ScribusView* view;
	int Breite;
	int Hoehe;
	int OriBreite;
	int OriHoehe;
	int MaxSeite;

public slots:
	void SetCo(double x, double y);
	void SetPg(int v);
	void SetCross();
	void SetVals();
	void SetZiel(int i);
	void GetFile();

protected:
	QVBoxLayout* AnnotLayout;
	QHBoxLayout* Layout1;
	QGridLayout* GroupBox1Layout;
	QHBoxLayout* Layout1_2;
};

#endif // ANNOT_H
