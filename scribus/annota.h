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

#include <qdialog.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwidgetstack.h>
#include <qlineedit.h>
#include <qframe.h>
#include "pageitem.h"
#include "navigator.h"
#include "scribusview.h"

class Annota : public QDialog
{
	Q_OBJECT

public:
	Annota(QWidget* parent, PageItem *it, int Seite, int b, int h, ScribusView* vie);
	~Annota() {};
	QWidgetStack* Fram;
	QLabel* TextLabel1;
	QComboBox* ComboBox1;
	QGroupBox* GroupBox1;
	QLabel* TextLabel3;
	QLabel* TextLabel4;
	QLabel* TextLabel5;
	QLineEdit* Destfile;
	QPushButton* ChFile;
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
