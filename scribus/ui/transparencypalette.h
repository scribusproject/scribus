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

#ifndef TPALETTE_H
#define TPALETTE_H

#include <QWidget>
#include <QPointer>
#include <QGroupBox>
#include <QPushButton>
#include <QToolButton>
#include "scribusapi.h"
#include "gradienteditor.h"
#include "scribusdoc.h"
#include "ui/scrpalettebase.h"
#include "ui_transparencypalette.h"
#include "ui/gradientvectordialog.h"
#include "ui/patternpropsdialog.h"

class PageItem;
class ColorListBox;
class ScrSpinBox;
class ScComboBox;
class ScPattern;
class LinkButton;

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
	void hideSelectionButtons();
	void updateFromItem();

	void updateColorList();

	void setColors(const ColorList& newColorList);
	void setGradients(QHash<QString, VGradient> *docGradients);
	void setPatterns(QHash<QString, ScPattern> *docPatterns);

public slots:
	void handleUpdateRequest(int);

	void editLineSelectorButton();
	void editFillSelectorButton();
	void slotGrad(int nr);
	void slotGradType(int type);
	void setNamedGradient(const QString &name);
	void switchGradientMode();
	void switchPatternMode();
	void editGradientVector();
	void setActiveGradDia(bool active);
	void hideEditedPatterns(QStringList names);
	void selectPattern(QListWidgetItem *c);
	void setActPattern(const QString& pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);
	void changePatternProps();
	void setSpecialGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk);
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
	double m_Pattern_scaleX {0.0};
	double m_Pattern_scaleY {0.0};
	double m_Pattern_offsetX {0.0};
	double m_Pattern_offsetY {0.0};
	double m_Pattern_rotation {0.0};
	double m_Pattern_skewX {0.0};
	double m_Pattern_skewY {0.0};
	bool m_Pattern_mirrorX {false};
	bool m_Pattern_mirrorY {false};

	void connectSignals();
	void disconnectSignals();
	void updateCList();
	void updateGradientList();
	void updatePatternList();
};
#endif
