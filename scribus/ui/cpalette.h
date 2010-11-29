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
#include "ui/gradientvectordialog.h"
#include "ui/patternpropsdialog.h"

class PageItem;
class ColorListBox;
class ScrSpinBox;
class ScComboBox;
class ScPattern;

/**
  *@author Franz Schmid
  */

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
	void setActOverprint(int);
	void updateCList();
	void ToggleColorDisplay();
	void SetPatterns(QMap<QString, ScPattern> *docPatterns);
	void SetGradients(QMap<QString, VGradient> *docGradients);
	void setNamedGradient(const QString &name);
	void setNamedGradientStroke(const QString &name);
	void selectPattern(QListWidgetItem *c);
	void selectPatternS(QListWidgetItem *c);
	void hideEditedPatterns(QStringList names);
	void updatePatternList();
	void updateGradientList();
	void setActPattern(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);
	void setActPatternStroke(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY, double space, bool pathF);
	void setActFarben(QString p, QString b, int shp, int shb);
	void selectColorS(QListWidgetItem *c);
	void selectColorF(QListWidgetItem *c);
	void slotGradStroke(int number);
	void ChooseGrad(int nr);
	void slotGrad(int nr);
	void slotGradType(int type);
	void slotGradTypeStroke(int type);
	void setActGradient(int typ);
	void setGradientColors();
	void editMeshPointColor();
	void createNewMeshGradient();
	void resetMeshGradient();
	void meshGradientToShape();
	void resetOneControlPoint();
	void resetAllControlPoints();
	void editGradientVector();
	void editGradientVectorStroke();
	void setActiveGradDia(bool active);
	void setSpecialGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy);
	void setMeshPoint();
	void setMeshControlPoint();
	void updateMeshPoint();
	void changePatternProps();
	void changePatternPropsStroke();
	void toggleStrokePattern();
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
	void NewPatternTypeS(bool);
	void NewPatternPropsS(double, double, double, double, double, double, double, double, bool, bool);
	void NewSpecial(double, double, double, double, double, double, double, double, double, double);
	void NewOverprint(int);
	void gradientChanged();
	void strokeGradientChanged();
	void editGradient(int);

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
	double m_Pattern_spaceS;
	bool m_Pattern_mirrorXS;
	bool m_Pattern_mirrorYS;
	int currentUnit;
	int editStrokeGradient;
};

#endif
