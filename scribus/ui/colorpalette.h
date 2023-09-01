/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          colorpalette.h  -  description
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

#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QGroupBox>
#include <QPointer>
#include <QPushButton>
#include <QToolButton>
#include <QWidget>

#include "scribusapi.h"
#include "gradienteditor.h"
#include "scribusdoc.h"
#include "scpatterntransform.h"
#include "ui/scrpalettebase.h"
#include "ui_colorpalette.h"
#include "ui/gradientvectordialog.h"
#include "ui/patternpropsdialog.h"

class ColorListBox;
class PageItem;
class ScrSpinBox;
class ScPattern;
class UndoManager;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API ColorPalette : public QWidget, Ui::colorPalette
{
	Q_OBJECT

	friend class PropertiesPalette;

public:
	ColorPalette(QWidget* parent);
	~ColorPalette() = default;

	void setDocument(ScribusDoc* doc);

	void updateColorList();

	void setColors(const ColorList& newColorList);
	void setGradients(QHash<QString, VGradient> *docGradients);
	void setPatterns(QHash<QString, ScPattern> *docPatterns);

	void setCurrentPattern(const QString& pattern, const ScPatternTransform& patternTrans, bool mirrorX, bool mirrorY);
	void setCurrentPatternStroke(const QString& pattern, const ScStrokePatternTransform& patternTrans, bool mirrorX, bool mirrorY, bool pathF);
	void setSpecialGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy);

private:

	PageItem* currentItemFromSelection();
	UndoManager* undoManager {nullptr};

public slots:
	void handleSelectionChanged();
	void handleUpdateRequest(int);

	/*void editLineColorSelectorButton();
	void editFillColorSelectorButton();*/
	void fillStrokeSelector(int /*index*/);
	void handleFillShade(double);
	void handleStrokeShade(double);
	void handleOverprint(int);
	void handleFillGradient();
	void handleStrokeGradient();
	void handleStrokeGradientExtend(int val);
	void handleGradientExtend(int val);
	void showColorValues(const QString& stroke, const QString& fille, int sShade, int fShade);
	void showGradient(int nr);
	void showGradientStroke(int nr);
	void showOverprint(int);
	void toggleColorDisplay();
	void setNamedGradient(const QString &name);
	void setNamedGradientStroke(const QString &name);
	void selectPattern(QListWidgetItem *c);
	void selectPatternS(QListWidgetItem *c);
	void hideEditedPatterns(QStringList names);
	void selectColorS(int row);
	void selectColorF(int row);
	void slotGradStroke(int number);
	void slotGrad(int nr);
	void slotGradType(int type);
	void slotGradTypeStroke(int type);
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
	void setMeshPoint();
	void endPatchAdd();
	void snapToPatchGrid(bool val);
	void handleRemovePatch();
	void setMeshPatch();
	void setMeshPatchPoint();
	void updateMeshPoint();
	void changePatternProps();
	void changePatternPropsStroke();
	void toggleStrokePattern();
	void changeHatchProps();

signals:
	void NewPen(QString);
	void NewBrush(QString);
	void NewPenShade(double);
	void NewBrushShade(double);
	void NewGradient(int);
	void NewGradientS(int);
	void NewPattern(QString);
	void NewPatternProps(double, double, double, double, double, double, double, bool, bool);
	void NewPatternS(QString);
	void NewPatternTypeS(bool);
	void NewPatternPropsS(double, double, double, double, double, double, double, double, bool, bool);
	void NewSpecial(double, double, double, double, double, double, double, double, double, double);
	void NewOverprint(int);
	void editGradient(int);

protected:
	ColorList colorList;
	GradientVectorDialog* CGradDia { nullptr };
	PageItem* currentItem { nullptr };
	QHash<QString, ScPattern> *patternList { nullptr };
	QHash<QString, VGradient> *gradientList { nullptr };
	QPointer<ScribusDoc> currentDoc;

	ScPatternTransform m_patternTrans;
	ScStrokePatternTransform m_strokePatternTrans;
	bool   m_Pattern_mirrorX { false };
	bool   m_Pattern_mirrorXS { false };
	bool   m_Pattern_mirrorY { false };
	bool   m_Pattern_mirrorYS { false };
	int    currentUnit {0};
	int    editStrokeGradient {0};

	void   connectSignals();
	void   disconnectSignals();

	void   setCurrentItem(PageItem* item);
	void   updateFromItem();

	void   setGradientVectorValues();
	void   setGradientVectorStrokeValues();

	int    m_blockUpdates {0};
	void   blockUpdates(bool block) { if (block) ++m_blockUpdates; else --m_blockUpdates; }
	bool   updatesBlocked() { return (m_blockUpdates > 0); }

	void   enablePatterns(bool enable);

	void   updateCList();
	void   updateGradientList();
	void   updatePatternList();

	void   languageChange();
	void   unitChange(double, double, int unitIndex);
};

#endif
