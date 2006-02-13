/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <qtooltip.h>
#include "scribusapi.h"
#include "scribusstructs.h"
#include "gradienteditor.h"

class QListBox;
class QListBoxItem;
class QPixmap;
class QRect;
class QPopupMenu;
class QFont;
class QLayout;
class QToolButton;
class QButtonGroup;
class QComboBox;
class QLabel;
class QSpinBox;
class DynamicTip;
class MSpinBox;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API Cpalette : public QWidget
{
	Q_OBJECT
	
friend class Mpalette;	
	
public:
	Cpalette(QWidget* parent);
	~Cpalette() {};


public slots:
	void InhaltButton();
	void InnenButton();
	void SetColors(ColorList newColorList);
	void updateCList();
	void updateBoxS(QString Farbe);
	void selectColor(QListBoxItem *c);
	QColor setColor(QString farbe, int shad);
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
	void unitChange(double old, double neww, int ein);
	void languageChange();

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
	void editGradient();

protected:
	QVBoxLayout* Form1Layout;
	QGridLayout* Layout1;
	QVBoxLayout* GradLayout;
	QGridLayout* freeGradientLayout;
	QPixmap alertIcon;
	QPixmap cmykIcon;
	QPixmap rgbIcon;
	QPixmap spotIcon;
	QPixmap regIcon;
	DynamicTip* dynTip;
	
	
	QListBox *colorListQLBox;
	QToolButton *Inhalt;
	QToolButton *Innen;
	QSpinBox *PM1;
	QComboBox* gradientQCombo;
	GradientEditor* gradEdit;
	QFrame* freeGradientQFrame;
	QLabel* GTextX1;
	QLabel* GTextY1;
	QLabel* GTextX2;
	QLabel* GTextY2;
	MSpinBox* gY1;
	MSpinBox* gX2;
	MSpinBox* gX1;
	MSpinBox* gY2;
	QToolButton *gradEditButton;
	QLabel* TransTxt;
	QLabel* ShadeTxt;
	QSpinBox* TransSpin;
	int Mode;
	QString sFarbe;
	ColorList colorList;
	bool CSichtbar;
	QString Color;
	int Shade;
	QString Color3;
	int Shade3;
	bool UseTransFeature;
	bool GradientMode;
};

#endif
