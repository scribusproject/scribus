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

#include <QWidget>
#include <QPointer>
class QComboBox;
class QCheckBox;
class QEvent;
class QFrame;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QPixmap;
class QSpacerItem;
class QSpinBox;
class QToolButton;
class QVBoxLayout;
#include "scribusapi.h"
#include "gradienteditor.h"
#include "scribusdoc.h"
#include "scrpalettebase.h"

class PageItem;
class ColorListBox;
class ScrSpinBox;
class ScComboBox;
class ScPattern;
class LinkButton;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API GradientVectorDialog : public ScrPaletteBase
{
	Q_OBJECT

public:
	GradientVectorDialog( QWidget* parent);
	~GradientVectorDialog() {};
	virtual void changeEvent(QEvent *e);

public slots:
	void languageChange();
	void setValues(double x1, double y1, double x2, double y2);
	void changeSpecial();
	void unitChange(int unitIndex);

signals:
	void NewSpecial(double, double, double, double);

protected:
	QGridLayout* freeGradientLayout;
	QLabel* GTextX1;
	QLabel* GTextY1;
	QLabel* GTextX2;
	QLabel* GTextY2;
	ScrSpinBox* gY1;
	ScrSpinBox* gX2;
	ScrSpinBox* gX1;
	ScrSpinBox* gY2;
};

class SCRIBUS_API Cpalette : public QWidget
{
	Q_OBJECT

	friend class PropertiesPalette;

public:
	Cpalette(QWidget* parent);
	~Cpalette() {};
	
	virtual void changeEvent(QEvent *e);

	void setDocument(ScribusDoc* doc);
	void setCurrentItem(PageItem* item);
	void updateFromItem();

public slots:
	void InhaltButton();
	void InnenButton();
	void SetColors(ColorList newColorList);
	void updateCList();
	void SetPatterns(QMap<QString, ScPattern> *docPatterns);
	void selectPattern(QListWidgetItem *c);
	void changePatternProps();
	void ToggleKette();
	void HChange();
	void VChange();
	void updatePatternList();
	void updateBoxS(QString Farbe);
	void selectColor(QListWidgetItem *c);
	QColor setColor(QString farbe, int shad);
	void slotGrad(int nr);
	void slotColor(QString n, int s);
	void ChooseGrad(int nr);
	void setActFarben(QString p, QString b, int shp, int shb);
	void setActGradient(int typ);
	void setActPattern(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation);
	void setSpecialGradient(double x1, double y1, double x2, double y2);
	void editGradientVector();
	void setActiveGradDia(bool active);
	void setActShade();
	void setActTrans(double, double);
	void changeBlendMode(int);
	void setActBlend(int, int);
	void setGradTrans(double val);
	void slotTrans(int val);
	void unitChange(double old, double neww, int ein);
	void languageChange();
	void ToggleColorDisplay();

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
	QVBoxLayout* Form1Layout;
	QHBoxLayout* Layout1;
	QGridLayout* Layout1t;
	QVBoxLayout* GradLayout;
//	QGridLayout* freeGradientLayout;
	QPixmap alertIcon;
	QPixmap cmykIcon;
	QPixmap rgbIcon;
	QPixmap spotIcon;
	QPixmap regIcon;

	ColorListBox *colorListQLBox;
	QFrame* patternFrame;
	QListWidget *patternBox;
	QGroupBox* groupOffset;
	QLabel* textLabel1;
	ScrSpinBox* spinXoffset;
	QLabel* textLabel2;
	ScrSpinBox* spinYoffset;
	QGroupBox* groupScale;
	QLabel* textLabel5;
	ScrSpinBox* spinXscaling;
	QLabel* textLabel6;
	ScrSpinBox* spinYscaling;
	LinkButton* keepScaleRatio;
	QGroupBox* groupRotation;
	QLabel* textLabel7;
	ScrSpinBox* spinAngle;
	QVBoxLayout* frame3Layout;
	QHBoxLayout* groupOffsetLayout;
	QGridLayout* groupScaleLayout;
	QHBoxLayout* groupRotationLayout;

	QToolButton *Inhalt;
	QToolButton *Innen;
	QSpinBox *PM1;
	QComboBox* gradientQCombo;
	GradientEditor* gradEdit;
	GradientVectorDialog* CGradDia;
/*	QFrame* freeGradientQFrame;
	QLabel* GTextX1;
	QLabel* GTextY1;
	QLabel* GTextX2;
	QLabel* GTextY2;
	ScrSpinBox* gY1;
	ScrSpinBox* gX2;
	ScrSpinBox* gX1;
	ScrSpinBox* gY2; */
	QToolButton *gradEditButton;
	QLabel* TransTxt;
	QLabel* TransTxt2;
	QLabel* ShadeTxt;
	QGroupBox* TransGroup;
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
	QCheckBox* displayAllColors;
};

#endif
