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
#include <QGroupBox>
#include <QPushButton>
#include <QToolButton>
#include "scribusapi.h"
#include "gradienteditor.h"
#include "scribusdoc.h"
#include "ui/scrpalettebase.h"
#include "ui_colorpalette.h"

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

class SCRIBUS_API PatternPropsDialog : public QDialog
{
	Q_OBJECT

public:
	PatternPropsDialog(QWidget* parent, int unitIndex);
	~PatternPropsDialog() {};
	virtual void changeEvent(QEvent *e);
	ScrSpinBox* spinXoffset;
	ScrSpinBox* spinYoffset;
	ScrSpinBox* spinXscaling;
	ScrSpinBox* spinYscaling;
	ScrSpinBox* spinAngle;
	ScrSpinBox* spinXSkew;
	ScrSpinBox* spinYSkew;
	QToolButton* FlipH;
	QToolButton* FlipV;

public slots:
	void languageChange();
	void changePatternProps();
	void ToggleKette();
	void HChange();
	void VChange();

signals:
	void NewPatternProps(double, double, double, double, double, double, double, bool, bool);

protected:
	QGroupBox* groupOffset;
	QLabel* textLabel1;
	QLabel* textLabel2;
	QGroupBox* groupScale;
	QLabel* textLabel5;
	QLabel* textLabel6;
	LinkButton* keepScaleRatio;
	QGroupBox* groupRotation;
	QLabel* textLabel7;
	QGroupBox* groupSkew;
	QLabel* textLabel8;
	QLabel* textLabel9;
	QLabel* textLabel15;
	QVBoxLayout* frame3Layout;
	QHBoxLayout* groupOffsetLayout;
	QGridLayout* groupScaleLayout;
	QGridLayout* groupSkewLayout;
	QHBoxLayout* groupRotationLayout;
	QPushButton* buttonOk;
	QHBoxLayout* buttonLayout;
	QHBoxLayout* groupFlipLayout;
};

class SCRIBUS_API Cpalette : public QWidget, Ui::colorPalette
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
	void editLineColorSelectorButton();
	void editFillColorSelectorButton();
	void SetColors(ColorList newColorList);
	void setActTrans(double, double);
	void setActBlend(int, int);
	void setActOverprint(int);
	void slotTransS(int val);
	void slotTransF(int val);
	void updateCList();
	void ToggleColorDisplay();
	void SetPatterns(QMap<QString, ScPattern> *docPatterns);
	void SetGradients(QMap<QString, VGradient> *docGradients);
	void setNamedGradient(const QString &name);
	void setNamedGradientStroke(const QString &name);
	void selectPattern(QListWidgetItem *c);
	void selectPatternS(QListWidgetItem *c);
	void updatePatternList();
	void updateGradientList();
	void setActPattern(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);
	void setActPatternStroke(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);
	void setActFarben(QString p, QString b, int shp, int shb);
	void selectColorS(QListWidgetItem *c);
	void selectColorF(QListWidgetItem *c);
	void slotGradStroke(int number);
	void ChooseGrad(int nr);
	void slotGrad(int nr);
	void slotGradType(int type);
	void slotGradTypeStroke(int type);
	void setActGradient(int typ);
	void editGradientVector();
	void editGradientVectorStroke();
	void setActiveGradDia(bool active);
	void setSpecialGradient(double x1, double y1, double x2, double y2);
	void changePatternProps();
	void changePatternPropsStroke();
	void unitChange(double, double, int unitIndex);
signals:
	void NewPen(QString);
	void NewBrush(QString);
	void NewPenShade(int);
	void NewBrushShade(int);
	void NewGradient(int);
	void NewGradientS(int);
	void NewPattern(QString);
	void NewPatternProps(double, double, double, double, double, double, double, bool, bool);
	void NewPatternS(QString);
	void NewPatternPropsS(double, double, double, double, double, double, double, bool, bool);
	void NewSpecial(double, double, double, double);
	void NewTrans(double);
	void NewTransS(double);
	void NewBlend(int);
	void NewBlendS(int);
	void NewOverprint(int);
	void gradientChanged();
	void strokeGradientChanged();
	void editGradient(bool);

protected:
	GradientVectorDialog* CGradDia;
	QPointer<ScribusDoc> currentDoc;
	PageItem* currentItem;
	QString sFarbe;
	ColorList colorList;
	QMap<QString, ScPattern> *patternList;
	QMap<QString, VGradient> *gradientList;
	double m_Pattern_scaleX;
	double m_Pattern_scaleY;
	double m_Pattern_offsetX;
	double m_Pattern_offsetY;
	double m_Pattern_rotation;
	double m_Pattern_skewX;
	double m_Pattern_skewY;
	bool m_Pattern_mirrorX;
	bool m_Pattern_mirrorY;
	double m_Pattern_scaleXS;
	double m_Pattern_scaleYS;
	double m_Pattern_offsetXS;
	double m_Pattern_offsetYS;
	double m_Pattern_rotationS;
	double m_Pattern_skewXS;
	double m_Pattern_skewYS;
	bool m_Pattern_mirrorXS;
	bool m_Pattern_mirrorYS;
	int currentUnit;
	bool editStrokeGradient;
};

#endif
