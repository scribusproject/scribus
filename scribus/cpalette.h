/***************************************************************************
                          cpalette.h  -  description
                             -------------------
    begin                : Wed Apr 25 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CPALETTE_H
#define CPALETTE_H

#include <qlistbox.h>
#include <qpixmap.h>
#include <qrect.h>
#include <qpopupmenu.h>
#include <qfont.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include "scribusdoc.h"
#include "shadebutton.h"

/**
  *@author Franz Schmid
  */

class Cpalette : public QWidget
{
	Q_OBJECT
public:
	Cpalette(QWidget* parent);
	~Cpalette() {};

	QListBox *ListBox1;
	QToolButton *Inhalt;
	QToolButton *Innen;
	QSpinBox *PM1;
	QComboBox* GradCombo;
	QButtonGroup* GradGroup;
	QRadioButton* GrColor1;
	QRadioButton* GrColor2;
	QButtonGroup* TransGroup;
	QLabel* TransTxt;
	QLabel* ShadeTxt;
	QSpinBox* TransSpin;
	int Mode;
	QString sFarbe;
	CListe Farbliste;
	bool CSichtbar;
	QString Color;
	QString Color2;
	int Shade;
	int Shade2;
	QString Color3;
	int Shade3;
	bool UseTransFeature;
	bool GradientMode;

public slots:
	void InhaltButton();
	void InnenButton();
	void SetColors(CListe farben);
	void updateCList();
	void updateBoxS(QString Farbe);
	void selFarbe(QListBoxItem *c);
	void slotGrad(int nr);
	void slotColor();
	void ChooseGrad(int nr);
	void setActFarben(QString p, QString b, int shp, int shb);
	void setActGradient(QString p, QString b, int shp, int shb, int typ);
	void setActShade();
	void setActTrans(double, double);
	void slotTrans(int val);
	void UseTrans(bool b);

signals:
	void NewPen(QString);
	void NewBrush(QString);
	void NewPenShade(int);
	void NewBrushShade(int);
	void NewGradient(int, QString, int, QString, int);
	void NewTrans(double);
	void NewTransS(double);
	void QueryItem();

protected:
	QVBoxLayout* Form1Layout;
	QGridLayout* Layout1;
	QVBoxLayout* GradLayout;
	QHBoxLayout* GradGroupLayout;
	QHBoxLayout* TransGroupLayout;
};

#endif
