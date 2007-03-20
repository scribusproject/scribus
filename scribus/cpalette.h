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
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3Frame>
#include <QPixmap>
#include <QLabel>
#include <Q3PopupMenu>
#include <Q3VBoxLayout>
#include "scribusapi.h"
#include "scribusstructs.h"
#include "gradienteditor.h"
#include "scribusdoc.h"
#include <qpointer.h>

class PageItem;
class Q3ListBox;
class Q3ListBoxItem;
class Q3IconView;
class Q3IconViewItem;
class QPixmap;
class QRect;
class Q3PopupMenu;
class QFont;
class QLayout;
class QToolButton;
class Q3ButtonGroup;
class Q3GroupBox;
class QComboBox;
class QLabel;
class QSpacerItem;
class QSpinBox;
// class DynamicTip;
class ColorListBox;
class ScrSpinBox;
class ScComboBox;
class ScPattern;
class LinkButton;

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

	void setDocument(ScribusDoc* doc) { currentDoc = doc; }
	void setCurrentItem(PageItem* item);
	void updateFromItem();

public slots:
	void InhaltButton();
	void InnenButton();
	void SetColors(ColorList newColorList);
	void updateCList();
	void SetPatterns(QMap<QString, ScPattern> *docPatterns);
	void selectPattern(Q3IconViewItem *c);
	void changePatternProps();
	void ToggleKette();
	void HChange();
	void VChange();
	void updatePatternList();
	void updateBoxS(QString Farbe);
	void selectColor(Q3ListBoxItem *c);
	QColor setColor(QString farbe, int shad);
	void slotGrad(int nr);
	void slotColor(QString n, int s);
	void ChooseGrad(int nr);
	void setActFarben(QString p, QString b, int shp, int shb);
	void setActGradient(int typ);
	void setActPattern(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation);
	void setSpecialGradient(double x1, double y1, double x2, double y2);
	void changeSpecial();
	void setActShade();
	void setActTrans(double, double);
	void changeBlendMode(int);
	void setActBlend(int, int);
	void setGradTrans(double val);
	void slotTrans(int val);
	void unitChange(double old, double neww, int ein);
	void languageChange();

signals:
	void NewPen(QString);
	void NewBrush(QString);
	void NewPenShade(int);
	void NewBrushShade(int);
	void NewGradient(int);
	void NewPattern(QString);
	void NewPatternProps(double, double, double, double, double);
	void NewSpecial(double, double, double, double);
	void NewTrans(double);
	void NewTransS(double);
	void NewBlend(int);
	void NewBlendS(int);
	void modeChanged();
	void gradientChanged();
	void editGradient();

protected:
	Q3VBoxLayout* Form1Layout;
	Q3HBoxLayout* Layout1;
	Q3GridLayout* Layout1t;
	Q3VBoxLayout* GradLayout;
	Q3GridLayout* freeGradientLayout;
	QPixmap alertIcon;
	QPixmap cmykIcon;
	QPixmap rgbIcon;
	QPixmap spotIcon;
	QPixmap regIcon;
// 	DynamicTip* dynTip;

	ColorListBox *colorListQLBox;
	Q3Frame* patternFrame;
	Q3IconView *patternBox;
	Q3GroupBox* groupOffset;
	QLabel* textLabel1;
	ScrSpinBox* spinXoffset;
	QLabel* textLabel2;
	ScrSpinBox* spinYoffset;
	Q3GroupBox* groupScale;
	QLabel* textLabel5;
	ScrSpinBox* spinXscaling;
	QLabel* textLabel6;
	ScrSpinBox* spinYscaling;
	LinkButton* keepScaleRatio;
	Q3GroupBox* groupRotation;
	QLabel* textLabel7;
	ScrSpinBox* spinAngle;
	Q3VBoxLayout* frame3Layout;
	Q3HBoxLayout* groupOffsetLayout;
	Q3GridLayout* groupScaleLayout;
	Q3HBoxLayout* groupRotationLayout;

	QToolButton *Inhalt;
	QToolButton *Innen;
	QSpinBox *PM1;
	QComboBox* gradientQCombo;
	GradientEditor* gradEdit;
	Q3Frame* freeGradientQFrame;
	QLabel* GTextX1;
	QLabel* GTextY1;
	QLabel* GTextX2;
	QLabel* GTextY2;
	ScrSpinBox* gY1;
	ScrSpinBox* gX2;
	ScrSpinBox* gX1;
	ScrSpinBox* gY2;
	QToolButton *gradEditButton;
	QLabel* TransTxt;
	QLabel* TransTxt2;
	QLabel* ShadeTxt;
	Q3GroupBox* TransGroup;
	QSpinBox* TransSpin;
	QPointer<ScribusDoc> currentDoc;
	PageItem* currentItem;
	ScComboBox* blendMode;
	int Mode;
	QString sFarbe;
	ColorList colorList;
	QMap<QString, ScPattern> *patternList;
	bool CSichtbar;
	QString Color;
	int Shade;
	QString Color3;
	int Shade3;
	bool GradientMode;
	int currentGradient;
	QSpacerItem *selectorQSpacer;
};

#endif
