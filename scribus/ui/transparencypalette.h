/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          transparencypalette.h  -  description
                             -------------------
    begin                : Tue Nov 17 2009
    copyright            : (C) 2009 by Franz Schmid
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

#ifndef TRANSPARENCYPALETTE_H
#define TRANSPARENCYPALETTE_H

#include <QPointer>
#include <QGroupBox>
#include <QPushButton>
#include <QToolButton>
#include <QWidget>

#include "scribusapi.h"
#include "gradienteditor.h"
#include "scribusdoc.h"
#include "ui/scrpalettebase.h"
#include "ui_transparencypalette.h"
#include "ui/gradientvectordialog.h"
#include "ui/patternpropsdialog.h"

class ColorListBox;
class LinkButton;
class PageItem;
class ScrSpinBox;
class ScPattern;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API TransparencyPalette : public QWidget, Ui::transparencyPalette
{
	Q_OBJECT

	friend class PropertiesPalette;
	friend class PropertiesPalette_Group;

public:
	TransparencyPalette(QWidget* parent);
	~TransparencyPalette() = default;

	void setDocument(ScribusDoc* doc);
	void setCurrentItem(PageItem* item);
	void showSelectionButtons();
	void hideSelectionButtons();
	void updateFromItem();

	void updateColorList();

	void setColors(const ColorList& newColorList);
	void setGradients(QHash<QString, VGradient> *docGradients);
	void setPatterns(QHash<QString, ScPattern> *docPatterns);

	void setCurrentMaskPattern(const QString& pattern, const ScMaskTransform& maskTrans, bool mirrorX, bool mirrorY);
	void setSpecialGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk);

public slots:
	void handleUpdateRequest(int);

	void editLineSelectorButton();
	void editFillSelectorButton();
	void slotGrad(int number);
	void slotGradType(int type);
	void setNamedGradient(const QString &name);
	void switchGradientMode();
	void switchPatternMode();
	void editGradientVector();
	void setActiveGradDia(bool active);
	void hideEditedPatterns(QStringList names);
	void selectPattern(QListWidgetItem *c);
	void changePatternProps();
	void setActTrans(double, double);
	void setActBlend(int, int);
	void slotTransS(double val);
	void slotTransF(double val);
	void unitChange(double, double, int unitIndex);

signals:
	void NewTrans(double);
	void NewTransS(double);
	void NewBlend(int);
	void NewBlendS(int);
	void gradientChanged();
	void editGradient();
	void NewGradient(int);
	void NewSpecial(double, double, double, double, double, double, double, double, double, double);
	void NewPattern(QString);
	void NewPatternProps(double, double, double, double, double, double, double, bool, bool);

protected:
	GradientVectorDialog* TGradDia {nullptr};
	QPointer<ScribusDoc> currentDoc;
	PageItem* currentItem {nullptr};
	ColorList colorList;
	int currentUnit {0};
	QHash<QString, ScPattern> *patternList {nullptr};
	QHash<QString, VGradient> *gradientList {nullptr};
	ScMaskTransform m_maskTransform;
	bool m_Pattern_mirrorX {false};
	bool m_Pattern_mirrorY {false};

	void connectSignals();
	void disconnectSignals();
	void updateCList();
	void updateGradientList();
	void updatePatternList();
};
#endif
