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
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include "scribusdoc.h"
#include "shadebutton.h"
#include "mspinbox.h"
#include "gradienteditor.h"

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
	GradientEditor* GradEdit;
	QFrame* frame8;
	QLabel* GTextX1;
	QLabel* GTextY1;
	QLabel* GTextX2;
	QLabel* GTextY2;
	MSpinBox* gY1;
	MSpinBox* gX2;
	MSpinBox* gX1;
	MSpinBox* gY2;
	QLabel* TransTxt;
	QLabel* ShadeTxt;
	QSpinBox* TransSpin;
	int Mode;
	QString sFarbe;
	CListe Farbliste;
	bool CSichtbar;
	QString Color;
	int Shade;
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
	QColor SetFarbe(QString farbe, int shad);
	void slotGrad(int nr);
	void slotColor(QString n, int s);
	void ChooseGrad(int nr);
	void setActFarben(QString p, QString b, int shp, int shb);
	void setActGradient(int typ);
	void setSpecialGradient(double x1, double y1, double x2, double y2, double w, double h);
	void changeSpecial();
	void setActShade();
	void setActTrans(double, double);
	void setGradTrans(double val);
	void slotTrans(int val);
	void UseTrans(bool b);
	void UnitChange(double old, double neww, int ein);

signals:
	void NewPen(QString);
	void NewBrush(QString);
	void NewPenShade(int);
	void NewBrushShade(int);
	void NewGradient(int);
	void NewSpecial(double, double, double, double);
	void NewTrans(double);
	void NewTransS(double);
	void QueryItem();
	void gradientChanged();

protected:
	QVBoxLayout* Form1Layout;
	QGridLayout* Layout1;
	QVBoxLayout* GradLayout;
	QGridLayout* frame8Layout;
};

#endif
