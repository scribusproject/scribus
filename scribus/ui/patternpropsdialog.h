/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          patternpropsdialog.h  -  description
                             -------------------
    begin                : Thu Nov 19 2009
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

#ifndef PATTERNPROPSDIALOG_H
#define PATTERNPROPSDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QEvent>
#include "scribusapi.h"
#include "scrspinbox.h"
#include "linkbutton.h"

class SCRIBUS_API PatternPropsDialog : public QDialog
{
	Q_OBJECT

public:
	PatternPropsDialog(QWidget* parent, int unitIndex, bool isStroke);
	~PatternPropsDialog() {};
	virtual void changeEvent(QEvent *e);
	ScrSpinBox* spinXoffset;
	ScrSpinBox* spinYoffset;
	ScrSpinBox* spinXscaling;
	ScrSpinBox* spinYscaling;
	ScrSpinBox* spinAngle;
	ScrSpinBox* spinXSkew;
	ScrSpinBox* spinYSkew;
	ScrSpinBox* spinSpacing;
	QToolButton* FlipH;
	QToolButton* FlipV;
	bool forStroke;

public slots:
	void languageChange();
	void changePatternProps();
	void ToggleKette();
	void HChange();
	void VChange();

signals:
	void NewPatternProps(double, double, double, double, double, double, double, bool, bool);
	void NewPatternPropsS(double, double, double, double, double, double, double, double, bool, bool);

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
	QGroupBox* groupSpace;
	QLabel* textLabel20;
	QVBoxLayout* frame3Layout;
	QHBoxLayout* groupOffsetLayout;
	QGridLayout* groupScaleLayout;
	QGridLayout* groupSkewLayout;
	QHBoxLayout* groupRotationLayout;
	QHBoxLayout* groupSpaceLayout;
	QPushButton* buttonOk;
	QHBoxLayout* buttonLayout;
	QHBoxLayout* groupFlipLayout;
};
#endif
